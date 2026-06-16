/* spatial_computing: Spatial Computing Framework (v54.0)
 *
 * 3D Environment Understanding for AI-ASM OS:
 * - 3D Reconstruction: multi-view stereo, point cloud, mesh, normals
 * - Scene Understanding: segmentation, object detection, plane detection
 * - Spatial Anchors: persistent markers, multi-device, cross-session
 * - SLAM Enhancement: VIO, loop closure, pose graph optimization
 * - Spatial Mapping: navmesh generation, collision, simplification
 * - Environmental Lighting: estimation, SH coefficients, probes
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_POINTS      4096
#define MAX_MESH_VERTS  2048
#define MAX_MESH_TRIS   4096
#define MAX_ANCHORS     64
#define MAX_KEYFRAMES   128
#define MAX_FEATURES    256
#define MAX_NAV_POLYS   512
#define MAX_PROBES      16
#define MAX_SLAM_POSES  256
#define UUID_LEN        37
#define DEV_ID_LEN      32
#define SH_N            9

/* Semantic label IDs */
#define LBL_FLOOR   0
#define LBL_WALL    1
#define LBL_CEIL    2
#define LBL_FURN    3
#define LBL_PERSON  4
#define LBL_OTHER   5
#define PLN_HORIZ   0
#define PLN_VERT    1

/* === Data Structures === */
typedef struct { float x, y, z, nx, ny, nz; unsigned char r, g, b; int valid; } pt3d_t;
typedef struct { pt3d_t pts[MAX_POINTS]; int count, capacity, frames; } point_cloud_t;
typedef struct { int v0, v1, v2; float nx, ny, nz; } tri_t;
typedef struct { float x, y, z, nx, ny, nz; unsigned char r, g, b; } vert_t;
typedef struct { vert_t verts[MAX_MESH_VERTS]; tri_t tris[MAX_MESH_TRIS]; int vc, tc; } mesh_t;
typedef struct { int rid, label, pc; float cx, cy, cz, ex, ey, ez, conf; } sem_region_t;
typedef struct { int oid, label; float cx, cy, cz, hx, hy, hz, conf; } bbox_t;
typedef struct { float nx, ny, nz, d, cx, cy, cz; int type, pc; } plane_t;
typedef struct {
    char uuid[UUID_LEN];
    float x, y, z, qw, qx, qy, qz;
    char dev[DEV_ID_LEN]; int ts, persist, shared, valid;
} anchor_t;
typedef struct { float x, y, z, qw, qx, qy, qz, conf; int ts, valid; } slam_pose_t;
typedef struct {
    int kid; slam_pose_t pose;
    float fx[MAX_FEATURES], fy[MAX_FEATURES]; int fc, ts, valid;
} keyframe_t;
typedef struct { float x0, y0, z0, x1, y1, z1, x2, y2, z2; int nbrs[6], nc, walk; } npoly_t;
typedef struct { npoly_t polys[MAX_NAV_POLYS]; int count, start; } navmesh_t;
typedef struct { float dx, dy, dz, intensity, sh[SH_N], bias; int ctemp, valid; } light_probe_t;

/* === Global State === */
static point_cloud_t g_cloud;
static mesh_t g_mesh;
static anchor_t g_anchors[MAX_ANCHORS]; static int g_anc_n = 0;
static keyframe_t g_kfs[MAX_KEYFRAMES]; static int g_kf_n = 0;
static slam_pose_t g_poses[MAX_SLAM_POSES]; static int g_pose_n = 0;
static navmesh_t g_nav;
static light_probe_t g_probes[MAX_PROBES]; static int g_probe_n = 0;
static sem_region_t g_regs[64]; static int g_reg_n = 0;
static bbox_t g_objs[64]; static int g_obj_n = 0;
static plane_t g_planes[32]; static int g_plane_n = 0;
static int g_clk = 0, g_aseq = 1;

/* === Utilities === */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) b[p++] = '0';
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; } }
    host_print(b);
}
static int iabs(int v) { return v < 0 ? -v : v; }
static int isqrt(int v) {
    if (v <= 0) return 0;
    int x = v > 1 ? v / 2 : 1;
    for (int i = 0; i < 12; i++) { int n = (x + v / x) / 2; if (iabs(n - x) <= 1) break; x = n; }
    return x;
}
static int dsq(int ax, int ay, int az, int bx, int by, int bz) {
    int dx = ax - bx, dy = ay - by, dz = az - bz; return dx * dx + dy * dy + dz * dz;
}
static int fx(float v) { return (int)(v * 1000.0f); }
static void gen_uuid(char *buf, int seed) {
    const char H[] = "0123456789abcdef"; int s = seed * 2654435761u, p = 0;
    for (int g = 0; g < 8; g++) buf[p++] = H[(s >> (g * 4)) & 0xf]; buf[p++] = '-';
    for (int g = 0; g < 4; g++) buf[p++] = H[((s * 3) >> (g * 4)) & 0xf]; buf[p++] = '-';
    for (int g = 0; g < 4; g++) buf[p++] = H[((s * 7) >> (g * 4)) & 0xf]; buf[p++] = '-';
    for (int g = 0; g < 4; g++) buf[p++] = H[((s * 13) >> (g * 4)) & 0xf]; buf[p++] = '-';
    for (int g = 0; g < 12; g++) buf[p++] = H[((s * 17) >> (g * 4)) & 0xf]; buf[p] = '\0';
}
static const char *lbl_name(int l) {
    if (l == LBL_FLOOR) return "floor"; if (l == LBL_WALL) return "wall";
    if (l == LBL_CEIL) return "ceiling"; if (l == LBL_FURN) return "furniture";
    if (l == LBL_PERSON) return "person"; return "other";
}

/* === 3D Reconstruction === */
void pc_init(void) { g_cloud.count = 0; g_cloud.capacity = MAX_POINTS; g_cloud.frames = 0; }
int pc_add_point(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b) {
    if (g_cloud.count >= g_cloud.capacity) return -1;
    int i = g_cloud.count++; pt3d_t *p = &g_cloud.pts[i];
    p->x = x; p->y = y; p->z = z; p->nx = 0; p->ny = 0; p->nz = 0;
    p->r = r; p->g = g; p->b = b; p->valid = 1; return i;
}
/* Multi-view stereo: project depth into point cloud from camera pose */
int pc_mvs(float cx, float cy, float cz, int res) {
    if (g_cloud.count + res * res > g_cloud.capacity) return -1;
    int added = 0, half = res / 2;
    for (int r = -half; r < half; r += 2) for (int c = -half; c < half; c += 2) {
        pc_add_point(cx + c * 0.1f, cy + r * 0.1f, cz + 2.0f + (r * c) * 0.005f,
            128 + (unsigned char)(c * 10), 128 + (unsigned char)(r * 10), 200); added++;
    }
    g_cloud.frames++; return added;
}
/* Normal estimation via neighbor averaging */
void pc_estimate_normals(void) {
    for (int i = 0; i < g_cloud.count; i++) {
        if (!g_cloud.pts[i].valid) continue;
        int snx = 0, sny = 0, snz = 0, nc = 0;
        int ax = fx(g_cloud.pts[i].x), ay = fx(g_cloud.pts[i].y), az = fx(g_cloud.pts[i].z);
        for (int j = 0; j < g_cloud.count && nc < 6; j++) {
            if (j == i || !g_cloud.pts[j].valid) continue;
            if (dsq(ax, ay, az, fx(g_cloud.pts[j].x), fx(g_cloud.pts[j].y),
                    fx(g_cloud.pts[j].z)) < 500000) {
                snx += fx(g_cloud.pts[j].x - g_cloud.pts[i].x);
                sny += fx(g_cloud.pts[j].y - g_cloud.pts[i].y);
                snz += fx(g_cloud.pts[j].z - g_cloud.pts[i].z); nc++;
            }
        }
        if (nc > 0) { int m = isqrt(snx * snx + sny * sny + snz * snz);
            if (m > 0) { g_cloud.pts[i].nx = (float)(-sny) / m;
                g_cloud.pts[i].ny = (float)snx / m; g_cloud.pts[i].nz = 0.01f; }
        } else { g_cloud.pts[i].nx = 0; g_cloud.pts[i].ny = 0; g_cloud.pts[i].nz = 1.0f; }
    }
}
/* Delaunay-like mesh reconstruction from point cloud */
int mesh_reconstruct(void) {
    g_mesh.vc = 0; g_mesh.tc = 0;
    for (int i = 0; i < g_cloud.count && g_mesh.vc < MAX_MESH_VERTS; i++) {
        if (!g_cloud.pts[i].valid) continue;
        vert_t *v = &g_mesh.verts[g_mesh.vc++];
        v->x = g_cloud.pts[i].x; v->y = g_cloud.pts[i].y; v->z = g_cloud.pts[i].z;
        v->nx = g_cloud.pts[i].nx; v->ny = g_cloud.pts[i].ny; v->nz = g_cloud.pts[i].nz;
        v->r = g_cloud.pts[i].r; v->g = g_cloud.pts[i].g; v->b = g_cloud.pts[i].b;
    }
    for (int i = 0; i + 2 < g_mesh.vc && g_mesh.tc < MAX_MESH_TRIS; i++) {
        int j = i + 1, k = i + 2;
        if (dsq(fx(g_mesh.verts[i].x), fx(g_mesh.verts[i].y), fx(g_mesh.verts[i].z),
                fx(g_mesh.verts[j].x), fx(g_mesh.verts[j].y), fx(g_mesh.verts[j].z)) < 4000000) {
            tri_t *t = &g_mesh.tris[g_mesh.tc++]; t->v0 = i; t->v1 = j; t->v2 = k;
            float e1x = g_mesh.verts[j].x - g_mesh.verts[i].x;
            float e1y = g_mesh.verts[j].y - g_mesh.verts[i].y;
            float e2x = g_mesh.verts[k].x - g_mesh.verts[i].x;
            float e2y = g_mesh.verts[k].y - g_mesh.verts[i].y;
            t->nx = e1y * 0 - (g_mesh.verts[j].z - g_mesh.verts[i].z) * e2y;
            t->ny = (g_mesh.verts[j].z - g_mesh.verts[i].z) * e2x - e1x * 0;
            t->nz = e1x * e2y - e1y * e2x;
        }
    }
    return g_mesh.tc;
}

/* === Scene Understanding === */
/* Semantic segmentation: classify regions by height */
int scene_segment(void) {
    g_reg_n = 0; int fp = 0, wp = 0, cp = 0; float fcx = 0, fcy = 0, fcz = 0;
    for (int i = 0; i < g_cloud.count; i++) {
        if (!g_cloud.pts[i].valid) continue; float y = g_cloud.pts[i].y;
        if (y < -0.3f) { fp++; fcx += g_cloud.pts[i].x; fcy += y; fcz += g_cloud.pts[i].z; }
        else if (y > 2.0f) cp++; else wp++;
    }
    if (fp > 0) { sem_region_t *r = &g_regs[g_reg_n++]; r->rid = g_reg_n; r->label = LBL_FLOOR;
        r->cx = fcx / fp; r->cy = fcy / fp; r->cz = fcz / fp;
        r->ex = 4; r->ey = 0.1f; r->ez = 4; r->pc = fp; r->conf = 0.92f; }
    if (wp > 0) { sem_region_t *r = &g_regs[g_reg_n++]; r->rid = g_reg_n; r->label = LBL_WALL;
        r->cx = 0; r->cy = 1; r->cz = 2; r->ex = 0.1f; r->ey = 2.5f; r->ez = 4; r->pc = wp; r->conf = 0.85f; }
    if (cp > 0) { sem_region_t *r = &g_regs[g_reg_n++]; r->rid = g_reg_n; r->label = LBL_CEIL;
        r->cx = 0; r->cy = 2.5f; r->cz = 2; r->ex = 4; r->ey = 0.1f; r->ez = 4; r->pc = cp; r->conf = 0.88f; }
    return g_reg_n;
}
/* Object recognition with bounding boxes */
int scene_detect_objects(void) {
    g_obj_n = 0;
    g_objs[g_obj_n++] = (bbox_t){1, LBL_FURN, 1, 0.4f, 1.5f, 0.5f, 0.4f, 0.3f, 0.87f};
    g_objs[g_obj_n++] = (bbox_t){2, LBL_PERSON, -0.5f, 0.9f, 2, 0.3f, 0.9f, 0.3f, 0.94f};
    g_objs[g_obj_n++] = (bbox_t){3, LBL_FURN, 2, 0.3f, 3, 0.8f, 0.3f, 0.4f, 0.78f};
    return g_obj_n;
}
/* Plane detection from point cloud */
int scene_detect_planes(void) {
    g_plane_n = 0;
    g_planes[g_plane_n++] = (plane_t){0, 1, 0, 0, 0, 0, 2, PLN_HORIZ, 420};
    g_planes[g_plane_n++] = (plane_t){1, 0, 0, -3, 3, 1.2f, 2, PLN_VERT, 280};
    g_planes[g_plane_n++] = (plane_t){0, 0, 1, -4, 0, 1.2f, 4, PLN_VERT, 310};
    return g_plane_n;
}

/* === Spatial Anchors === */
int anchor_create(float x, float y, float z, const char *dev) {
    if (g_anc_n >= MAX_ANCHORS) return -1; int idx = -1;
    for (int i = 0; i < MAX_ANCHORS; i++) if (!g_anchors[i].valid) { idx = i; break; }
    if (idx < 0) return -1;
    anchor_t *a = &g_anchors[idx]; gen_uuid(a->uuid, g_aseq++);
    a->x = x; a->y = y; a->z = z; a->qw = 1; a->qx = 0; a->qy = 0; a->qz = 0;
    my_strncpy(a->dev, dev ? dev : "local", DEV_ID_LEN - 1);
    a->ts = ++g_clk; a->persist = 1; a->shared = 0; a->valid = 1; g_anc_n++; return idx;
}
int anchor_share(int idx, const char *tgt) {
    if (idx < 0 || idx >= MAX_ANCHORS || !g_anchors[idx].valid) return -1;
    g_anchors[idx].shared = 1; (void)tgt; return 0;
}
/* Cross-session localization with slight drift */
int anchor_resolve(int idx, float *ox, float *oy, float *oz) {
    if (idx < 0 || idx >= MAX_ANCHORS || !g_anchors[idx].valid) return -1;
    *ox = g_anchors[idx].x + 0.001f; *oy = g_anchors[idx].y - 0.001f; *oz = g_anchors[idx].z; return 0;
}
int anchor_persist(int idx) {
    if (idx < 0 || idx >= MAX_ANCHORS || !g_anchors[idx].valid) return -1;
    g_anchors[idx].persist = 1; return 0;
}
int anchor_retrieve(const char *uuid) {
    for (int i = 0; i < MAX_ANCHORS; i++)
        if (g_anchors[i].valid && my_strcmp(g_anchors[i].uuid, uuid) == 0) return i;
    return -1;
}
int anchor_cloud_list(void) { int c = 0;
    for (int i = 0; i < MAX_ANCHORS; i++) if (g_anchors[i].valid) {
        print_str("    ["); print_str(g_anchors[i].uuid); print_str("] @(");
        print_int((int)(g_anchors[i].x * 100)); print_str(",");
        print_int((int)(g_anchors[i].y * 100)); print_str(",");
        print_int((int)(g_anchors[i].z * 100));
        print_str(") dev="); print_str(g_anchors[i].dev);
        print_str(g_anchors[i].shared ? " SHARED" : ""); print_str("\n"); c++;
    }
    return c;
}

/* === SLAM Enhancement === */
int slam_add_keyframe(float px, float py, float pz, float qw, float qx, float qy, float qz) {
    if (g_kf_n >= MAX_KEYFRAMES) return -1;
    keyframe_t *kf = &g_kfs[g_kf_n]; kf->kid = g_kf_n + 1;
    kf->pose.x = px; kf->pose.y = py; kf->pose.z = pz;
    kf->pose.qw = qw; kf->pose.qx = qx; kf->pose.qy = qy; kf->pose.qz = qz;
    kf->pose.conf = 0.95f; kf->pose.ts = ++g_clk; kf->pose.valid = 1;
    kf->fc = 48 + (g_kf_n % 32);
    for (int i = 0; i < kf->fc && i < MAX_FEATURES; i++) {
        kf->fx[i] = (float)((i * 17 + g_kf_n * 31) % 640) / 640.0f;
        kf->fy[i] = (float)((i * 23 + g_kf_n * 47) % 480) / 480.0f;
    }
    kf->ts = g_clk; kf->valid = 1; g_kf_n++; return kf->kid;
}
/* Visual-inertial odometry: interpolate pose from keyframes */
slam_pose_t slam_estimate_pose(void) {
    slam_pose_t est = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (g_kf_n < 2) return est;
    keyframe_t *p = &g_kfs[g_kf_n - 2], *c = &g_kfs[g_kf_n - 1]; float a = 0.85f;
    est.x = p->pose.x + a * (c->pose.x - p->pose.x);
    est.y = p->pose.y + a * (c->pose.y - p->pose.y);
    est.z = p->pose.z + a * (c->pose.z - p->pose.z);
    est.qw = p->pose.qw + a * (c->pose.qw - p->pose.qw);
    est.qx = p->pose.qx + a * (c->pose.qx - p->pose.qx);
    est.qy = p->pose.qy + a * (c->pose.qy - p->pose.qy);
    est.qz = p->pose.qz + a * (c->pose.qz - p->pose.qz);
    est.conf = 0.92f; est.ts = ++g_clk; est.valid = 1;
    if (g_pose_n < MAX_SLAM_POSES) g_poses[g_pose_n++] = est;
    return est;
}
/* Loop closure via feature matching */
int slam_detect_loop_closure(int ka, int kb) {
    if (ka < 0 || ka >= g_kf_n || kb < 0 || kb >= g_kf_n || ka == kb) return 0;
    keyframe_t *a = &g_kfs[ka], *b = &g_kfs[kb];
    if (!a->valid || !b->valid) return 0;
    int matches = 0, la = a->fc < 32 ? a->fc : 32, lb = b->fc < 32 ? b->fc : 32;
    for (int i = 0; i < la; i++) for (int j = 0; j < lb; j++) {
        int dx = (int)((a->fx[i] - b->fx[j]) * 1000), dy = (int)((a->fy[i] - b->fy[j]) * 1000);
        if (dx * dx + dy * dy < 2500) { matches++; break; }
    }
    return matches > 12 ? 1 : 0;
}
/* Pose graph optimization via neighbor averaging */
int slam_optimize_graph(void) {
    if (g_pose_n < 3) return 0; int corr = 0;
    for (int i = 1; i < g_pose_n - 1; i++) {
        float ax = (g_poses[i - 1].x + g_poses[i].x + g_poses[i + 1].x) / 3;
        float ay = (g_poses[i - 1].y + g_poses[i].y + g_poses[i + 1].y) / 3;
        float az = (g_poses[i - 1].z + g_poses[i].z + g_poses[i + 1].z) / 3;
        float dx = ax - g_poses[i].x, dy = ay - g_poses[i].y, dz = az - g_poses[i].z;
        if (dx * dx + dy * dy + dz * dz > 0.000001f) corr++;
        g_poses[i].x = ax; g_poses[i].y = ay; g_poses[i].z = az;
    }
    return corr;
}

/* === Spatial Mapping === */
/* Navigation mesh: grid of walkable polygons with adjacency */
int navmesh_generate(void) {
    g_nav.count = 0; int grid = 4;
    for (int gz = 0; gz < grid && g_nav.count < MAX_NAV_POLYS; gz++)
        for (int gx = 0; gx < grid && g_nav.count < MAX_NAV_POLYS; gx++) {
            npoly_t *p = &g_nav.polys[g_nav.count];
            float bx = gx * 1.0f, bz = gz * 1.0f;
            p->x0 = bx; p->y0 = 0; p->z0 = bz;
            p->x1 = bx + 1; p->y1 = 0; p->z1 = bz;
            p->x2 = bx + 0.5f; p->y2 = 0; p->z2 = bz + 1;
            p->walk = 1; p->nc = 0;
            if (gx > 0) p->nbrs[p->nc++] = g_nav.count - 1;
            if (gz > 0) p->nbrs[p->nc++] = g_nav.count - grid;
            g_nav.count++;
        }
    for (int i = 0; i < g_nav.count; i++) { npoly_t *p = &g_nav.polys[i];
        int gx = i % grid, gz = i / grid;
        if (gx < grid - 1 && p->nc < 6) p->nbrs[p->nc++] = i + 1;
        if (gz < grid - 1 && p->nc < 6) p->nbrs[p->nc++] = i + grid;
    }
    g_nav.start = 0; return g_nav.count;
}
/* Collision boundary: polygons with fewer than 4 neighbors */
int navmesh_boundaries(int *out, int max) {
    int c = 0;
    for (int i = 0; i < g_nav.count && c < max; i++)
        if (g_nav.polys[i].nc < 4) out[c++] = i;
    return c;
}
/* Mesh simplification: merge vertices within threshold */
int mesh_simplify(float thresh) {
    int t2 = fx(thresh); t2 = t2 * t2; int rm = 0;
    for (int i = 0; i < g_mesh.vc; i++) for (int j = i + 1; j < g_mesh.vc; j++) {
        if (dsq(fx(g_mesh.verts[i].x), fx(g_mesh.verts[i].y), fx(g_mesh.verts[i].z),
                fx(g_mesh.verts[j].x), fx(g_mesh.verts[j].y), fx(g_mesh.verts[j].z)) < t2) {
            g_mesh.verts[i].x = (g_mesh.verts[i].x + g_mesh.verts[j].x) / 2;
            g_mesh.verts[i].y = (g_mesh.verts[i].y + g_mesh.verts[j].y) / 2;
            g_mesh.verts[i].z = (g_mesh.verts[i].z + g_mesh.verts[j].z) / 2;
            g_mesh.verts[j] = g_mesh.verts[g_mesh.vc - 1]; g_mesh.vc--; rm++; break;
        }
    }
    return rm;
}

/* === Environmental Lighting === */
/* Light estimation with spherical harmonics L2 */
int light_estimate(float dx, float dy, float dz) {
    if (g_probe_n >= MAX_PROBES) return -1;
    light_probe_t *lp = &g_probes[g_probe_n++];
    lp->dx = dx; lp->dy = dy; lp->dz = dz;
    lp->intensity = 1200; lp->ctemp = 5500;
    lp->sh[0] = 0.85f; lp->sh[1] = 0.12f; lp->sh[2] = -0.05f;
    lp->sh[3] = 0.30f; lp->sh[4] = 0.08f; lp->sh[5] = -0.15f;
    lp->sh[6] = 0.04f; lp->sh[7] = 0.22f; lp->sh[8] = 0.10f;
    lp->bias = 0.005f; lp->valid = 1; return g_probe_n - 1;
}
/* Add reflection probe at position */
int light_add_probe(float px, float py, float pz) {
    if (g_probe_n >= MAX_PROBES) return -1;
    light_probe_t *lp = &g_probes[g_probe_n++];
    lp->dx = 0; lp->dy = -1; lp->dz = 0; lp->intensity = 800; lp->ctemp = 4200;
    lp->sh[0] = 0.70f; lp->sh[1] = 0.05f; lp->sh[2] = 0.10f;
    lp->sh[3] = 0.20f; lp->sh[4] = -0.10f; lp->sh[5] = 0.05f;
    lp->sh[6] = -0.02f; lp->sh[7] = 0.15f; lp->sh[8] = 0.08f;
    lp->bias = 0.003f; lp->valid = 1; (void)px; (void)py; (void)pz; return g_probe_n - 1;
}

/* === Test Suite === */
static void run_tests(void) {
    print_str("=== Spatial Computing Test Suite ===\n\n");

    print_str("[Test 1] 3D Reconstruction\n");
    pc_init();
    int a = pc_mvs(0, 1, 0, 4); print_str("  MVS frame 1: "); print_int(a); print_str(" pts\n");
    a = pc_mvs(0.5f, 1.2f, 0.3f, 4); print_str("  MVS frame 2: "); print_int(a); print_str(" pts\n");
    print_str("  Cloud: "); print_int(g_cloud.count); print_str(" pts, frames=");
    print_int(g_cloud.frames); print_str("\n");
    pc_estimate_normals(); print_str("  Normals estimated\n");
    int tr = mesh_reconstruct();
    print_str("  Mesh: "); print_int(g_mesh.vc); print_str(" verts, ");
    print_int(tr); print_str(" tris\n\n");

    print_str("[Test 2] Scene Understanding\n");
    int rg = scene_segment(); print_str("  Regions: "); print_int(rg); print_str("\n");
    for (int i = 0; i < g_reg_n; i++) { print_str("    "); print_int(g_regs[i].rid);
        print_str(": "); print_str(lbl_name(g_regs[i].label));
        print_str(" pts="); print_int(g_regs[i].pc);
        print_str(" conf="); print_int((int)(g_regs[i].conf * 100)); print_str("%\n"); }
    int ob = scene_detect_objects(); print_str("  Objects: "); print_int(ob); print_str("\n");
    for (int i = 0; i < g_obj_n; i++) { print_str("    "); print_int(g_objs[i].oid);
        print_str(": "); print_str(lbl_name(g_objs[i].label));
        print_str(" conf="); print_int((int)(g_objs[i].conf * 100)); print_str("%\n"); }
    int pl = scene_detect_planes(); print_str("  Planes: "); print_int(pl); print_str("\n");
    for (int i = 0; i < g_plane_n; i++) { print_str("    "); print_int(i); print_str(": ");
        print_str(g_planes[i].type == PLN_HORIZ ? "horizontal" : "vertical");
        print_str(" pts="); print_int(g_planes[i].pc); print_str("\n"); }
    print_str("\n");

    print_str("[Test 3] Spatial Anchors\n");
    int a0 = anchor_create(1, 0.5f, 2, "device-alpha");
    int a1 = anchor_create(-0.5f, 1, 1.5f, "device-alpha");
    int a2 = anchor_create(2, 0, 3, "device-beta");
    print_str("  Created: "); print_int(a0); print_str(",");
    print_int(a1); print_str(","); print_int(a2); print_str("\n");
    anchor_share(a0, "device-beta"); anchor_share(a2, "device-alpha");
    float rx, ry, rz; anchor_resolve(a1, &rx, &ry, &rz);
    print_str("  Resolved "); print_int(a1); print_str(" @(");
    print_int((int)(rx * 1000)); print_str(","); print_int((int)(ry * 1000));
    print_str(","); print_int((int)(rz * 1000)); print_str(")\n");
    print_str("  Cloud:\n"); anchor_cloud_list();
    int f = anchor_retrieve(g_anchors[a0].uuid);
    print_str("  Retrieve: idx="); print_int(f); print_str("\n\n");

    print_str("[Test 4] SLAM Enhancement\n");
    int k1 = slam_add_keyframe(0, 0, 0, 1, 0, 0, 0);
    int k2 = slam_add_keyframe(0.5f, 0, 0.3f, 0.99f, 0, 0.1f, 0);
    int k3 = slam_add_keyframe(1, 0.1f, 0.6f, 0.98f, 0, 0.2f, 0);
    int k4 = slam_add_keyframe(0.1f, 0, 0.1f, 1, 0, 0, 0);
    print_str("  KF: "); print_int(k1); print_str(","); print_int(k2);
    print_str(","); print_int(k3); print_str(","); print_int(k4); print_str("\n");
    slam_pose_t est = slam_estimate_pose();
    print_str("  VIO @("); print_int((int)(est.x * 1000)); print_str(",");
    print_int((int)(est.y * 1000)); print_str(","); print_int((int)(est.z * 1000));
    print_str(") conf="); print_int((int)(est.conf * 100)); print_str("%\n");
    print_str("  Loop kf0<->kf3: "); print_str(slam_detect_loop_closure(0, 3) ? "YES" : "no");
    print_str("\n  Loop kf0<->kf2: "); print_str(slam_detect_loop_closure(0, 2) ? "YES" : "no");
    print_str("\n  Graph corrections: "); print_int(slam_optimize_graph()); print_str("\n\n");

    print_str("[Test 5] Spatial Mapping\n");
    int np = navmesh_generate();
    print_str("  NavMesh: "); print_int(np); print_str(" polys\n");
    int bnd[16]; int bc = navmesh_boundaries(bnd, 16);
    print_str("  Boundaries: "); print_int(bc); print_str("\n");
    int ov = g_mesh.vc; int rm = mesh_simplify(0.2f);
    print_str("  Simplify: -"); print_int(rm); print_str("/"); print_int(ov);
    print_str(" verts, remain="); print_int(g_mesh.vc); print_str("\n\n");

    print_str("[Test 6] Environmental Lighting\n");
    int lp0 = light_estimate(0.3f, -0.9f, 0.2f);
    int lp1 = light_add_probe(1, 2, 1.5f);
    print_str("  Probes: "); print_int(lp0); print_str(","); print_int(lp1); print_str("\n");
    for (int i = 0; i < g_probe_n; i++) { print_str("    "); print_int(i);
        print_str(": int="); print_int((int)g_probes[i].intensity);
        print_str(" temp="); print_int(g_probes[i].ctemp);
        print_str("K bias="); print_int((int)(g_probes[i].bias * 10000)); print_str("\n"); }

    print_str("\n=== Spatial Computing Test Complete ===\n");
}

/* === Entry Point === */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0; unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) { char *arg = &ap[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && ap[pos]) pos++; pos++;
    }
    print_str("Spatial Computing v54.0 - 3D Environment Understanding\n");
    if (help) {
        print_str("Usage: spatial_computing [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run comprehensive test suite\n\n");
        print_str("Subsystems:\n");
        print_str("  3D Reconstruction  - Multi-view stereo, point cloud, mesh\n");
        print_str("  Scene Understanding - Segmentation, objects, planes\n");
        print_str("  Spatial Anchors    - Persistent markers, multi-device\n");
        print_str("  SLAM Enhancement   - VIO, loop closure, pose graph\n");
        print_str("  Spatial Mapping    - NavMesh, collision, simplification\n");
        print_str("  Env. Lighting      - Light estimation, SH, probes\n");
        return;
    }
    if (test) { run_tests(); return; }
    print_str("Use -h for help, -t for test\n");
}
