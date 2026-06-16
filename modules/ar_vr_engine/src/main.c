/* ar_vr_engine: AR/VR rendering engine module (v54.0)
 *
 * Provides immersive rendering capabilities:
 * - Stereoscopic dual-eye rendering with parallax and frustum culling
 * - Spatial audio with HRTF (Head-Related Transfer Function)
 * - Hand tracking with gesture recognition (21 keypoints)
 * - Eye tracking with gaze analysis and foveated rendering
 * - Hierarchical scene graph with LOD and occlusion culling
 * - Rigid body physics and cloth simulation (spring-mass)
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
#define MAX_EYES            2
#define MAX_AUDIO_SRC       16
#define MAX_HANDS           2
#define JOINTS_PER_HAND     21
#define MAX_NODES           64
#define MAX_BODIES          32
#define MAX_CLOTH           64
#define LOD_LEVELS          3
#define HASH_BUCKETS        16
#define EYE_LEFT            0
#define EYE_RIGHT           1
#define LOD_HIGH            0
#define LOD_MEDIUM          1
#define LOD_LOW             2
#define GESTURE_PINCH       0
#define GESTURE_GRAB        1
#define GESTURE_POINT       2
#define GESTURE_OPEN_HAND   3
#define GESTURE_FIST        4
#define MAT_RUBBER          0
#define MAT_METAL           1

/* ===== Data Structures ===== */

typedef struct { int x, y, z; } vec3_t;
typedef struct { int m[16]; } mat4_t;

typedef struct {
    int x, y, width, height;
    mat4_t projection, view;
    int ipd_offset;
} eye_viewport_t;

typedef struct {
    eye_viewport_t eyes[MAX_EYES];
    int hmd_width, hmd_height, refresh_rate, fov, ipd;
    int depth_buffer_size, foveation_level;
} vr_context_t;

typedef struct {
    int source_id, active;
    vec3_t position, velocity;
    int volume, pitch, distance;
    int hrtf_azimuth, hrtf_elevation, hrtf_gain;
    int reverb_level, reverb_decay;
} audio_source_t;

typedef struct {
    vec3_t position;
    int confidence, joint_type;
} hand_joint_t;

typedef struct {
    int hand_id, is_tracked, gesture, gesture_confidence;
    hand_joint_t joints[JOINTS_PER_HAND];
    vec3_t palm_position, palm_normal;
} hand_tracking_t;

typedef struct {
    int eye_id, fixation_duration, saccade_velocity;
    int pupil_diameter, blink_state, heatmap_x, heatmap_y, foveated_region;
    vec3_t gaze_point, gaze_direction;
} eye_tracking_t;

typedef struct {
    int node_id, parent_id, child_count, visible, culled, lod_level;
    int children[8];
    vec3_t position, rotation, scale;
    vec3_t bbox_min, bbox_max;
    mat4_t world_transform;
} scene_node_t;

typedef struct {
    int body_id, mass, inverse_mass, restitution, friction, material, is_static;
    vec3_t position, velocity, acceleration, force;
    vec3_t aabb_min, aabb_max;
} rigid_body_t;

typedef struct {
    vec3_t position, old_position, acceleration;
    int mass, pinned;
} cloth_point_t;

/* ===== Global State ===== */

static vr_context_t vr_ctx;
static audio_source_t audio_sources[MAX_AUDIO_SRC];
static hand_tracking_t hands[MAX_HANDS];
static eye_tracking_t eyes[MAX_EYES];
static scene_node_t scene_nodes[MAX_NODES];
static rigid_body_t rigid_bodies[MAX_BODIES];
static cloth_point_t cloth_points[MAX_CLOTH];
static int audio_src_count = 0, scene_node_count = 0;
static int rigid_body_count = 0, cloth_point_count = 0;
static int next_audio_id = 1, next_node_id = 1, next_body_id = 1;
static int sim_clock = 0, spatial_hash[HASH_BUCKETS];

/* ===== Helpers ===== */

static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; }
        pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}
static int abs_val(int x) { return x < 0 ? -x : x; }
static int clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }
static int isqrt(int x) {
    if (x <= 0) return 0;
    int r = x, n = (r + 1) / 2;
    while (n < r) { r = n; n = (r + x / r) / 2; }
    return r;
}

/* ===== Stereoscopic Rendering ===== */

void vr_init(int w, int h, int fov, int ipd) {
    vr_ctx.hmd_width = w; vr_ctx.hmd_height = h;
    vr_ctx.fov = fov; vr_ctx.ipd = ipd;
    vr_ctx.refresh_rate = 90; vr_ctx.depth_buffer_size = w * h * 4;
    vr_ctx.foveation_level = 2;
    for (int e = 0; e < MAX_EYES; e++) {
        eye_viewport_t *ev = &vr_ctx.eyes[e];
        ev->x = e * (w / 2); ev->y = 0;
        ev->width = w / 2; ev->height = h;
        ev->ipd_offset = e == EYE_LEFT ? -ipd / 2 : ipd / 2;
        for (int i = 0; i < 16; i++) { ev->projection.m[i] = 0; ev->view.m[i] = 0; }
        ev->projection.m[0] = 1000; ev->projection.m[5] = 1000;
        ev->projection.m[10] = -1;  ev->projection.m[15] = 1;
        ev->view.m[0] = 1; ev->view.m[5] = 1; ev->view.m[10] = 1; ev->view.m[15] = 1;
    }
}

int vr_parallax(int depth, int eye) {
    return (vr_ctx.eyes[eye].ipd_offset * depth) / 1000;
}

int vr_frustum_cull(vec3_t bmin, vec3_t bmax, int eye) {
    eye_viewport_t *ev = &vr_ctx.eyes[eye];
    if (bmax.x < ev->x || bmin.x > ev->x + ev->width) return 1;
    if (bmax.y < ev->y || bmin.y > ev->y + ev->height) return 1;
    return 0;
}

/* ===== Spatial Audio (HRTF) ===== */

int audio_create_source(vec3_t pos, int vol) {
    if (audio_src_count >= MAX_AUDIO_SRC) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_AUDIO_SRC; i++) if (!audio_sources[i].active) { idx = i; break; }
    if (idx < 0) return -1;
    audio_sources[idx].source_id = next_audio_id++;
    audio_sources[idx].position = pos;
    audio_sources[idx].velocity.x = 0; audio_sources[idx].velocity.y = 0; audio_sources[idx].velocity.z = 0;
    audio_sources[idx].volume = clamp(vol, 0, 100); audio_sources[idx].pitch = 100;
    audio_sources[idx].hrtf_azimuth = 0; audio_sources[idx].hrtf_elevation = 0;
    audio_sources[idx].hrtf_gain = 100; audio_sources[idx].distance = 0;
    audio_sources[idx].reverb_level = 20; audio_sources[idx].reverb_decay = 50;
    audio_sources[idx].active = 1; audio_src_count++;
    return audio_sources[idx].source_id;
}

void audio_update_hrtf(int sid, vec3_t listener) {
    for (int i = 0; i < MAX_AUDIO_SRC; i++) {
        if (audio_sources[i].source_id != sid || !audio_sources[i].active) continue;
        int dx = audio_sources[i].position.x - listener.x;
        int dy = audio_sources[i].position.y - listener.y;
        int dz = audio_sources[i].position.z - listener.z;
        audio_sources[i].distance = isqrt(dx * dx + dy * dy + dz * dz);
        audio_sources[i].hrtf_azimuth = dz != 0 ? clamp((dx * 180) / dz, -180, 180) : 0;
        int hdist = isqrt(dx * dx + dz * dz);
        audio_sources[i].hrtf_elevation = hdist != 0 ? clamp((dy * 180) / hdist, -90, 90) : 0;
        audio_sources[i].hrtf_gain = audio_sources[i].distance > 0 ?
            clamp(10000 / audio_sources[i].distance, 0, 100) : 100;
        break;
    }
}

/* ===== Hand Tracking ===== */

void hand_tracking_init(void) {
    for (int h = 0; h < MAX_HANDS; h++) {
        hands[h].hand_id = h + 1; hands[h].is_tracked = 0;
        hands[h].gesture = GESTURE_OPEN_HAND; hands[h].gesture_confidence = 0;
        for (int j = 0; j < JOINTS_PER_HAND; j++) {
            hands[h].joints[j].position.x = 0; hands[h].joints[j].position.y = 0;
            hands[h].joints[j].position.z = 0; hands[h].joints[j].confidence = 0;
            hands[h].joints[j].joint_type = j < 5 ? 0 : (j < 9 ? 1 : (j < 13 ? 2 : (j < 17 ? 3 : 4)));
        }
    }
}

void hand_tracking_simulate(int hid, vec3_t palm) {
    if (hid < 1 || hid > MAX_HANDS) return;
    int idx = hid - 1;
    hands[idx].is_tracked = 1; hands[idx].palm_position = palm;
    hands[idx].palm_normal.x = 0; hands[idx].palm_normal.y = 100; hands[idx].palm_normal.z = 0;
    for (int j = 0; j < JOINTS_PER_HAND; j++) {
        hands[idx].joints[j].position.x = palm.x + (j * 10) % 100 - 50;
        hands[idx].joints[j].position.y = palm.y + (j * 15) % 80;
        hands[idx].joints[j].position.z = palm.z + (j * 5) % 50;
        hands[idx].joints[j].confidence = 85 + (j % 15);
    }
    int zdiff = abs_val(hands[idx].joints[4].position.z - hands[idx].joints[8].position.z);
    if (zdiff < 20) { hands[idx].gesture = GESTURE_PINCH; hands[idx].gesture_confidence = 92; }
    else if (zdiff > 80) { hands[idx].gesture = GESTURE_OPEN_HAND; hands[idx].gesture_confidence = 88; }
    else { hands[idx].gesture = GESTURE_POINT; hands[idx].gesture_confidence = 78; }
}

const char *gesture_name(int g) {
    if (g == GESTURE_PINCH) return "pinch"; if (g == GESTURE_GRAB) return "grab";
    if (g == GESTURE_POINT) return "point"; if (g == GESTURE_OPEN_HAND) return "open_hand";
    if (g == GESTURE_FIST) return "fist"; return "unknown";
}

/* ===== Eye Tracking ===== */

void eye_tracking_init(void) {
    for (int e = 0; e < MAX_EYES; e++) {
        eyes[e].eye_id = e + 1;
        eyes[e].gaze_point.x = 0; eyes[e].gaze_point.y = 0; eyes[e].gaze_point.z = 500;
        eyes[e].gaze_direction.x = 0; eyes[e].gaze_direction.y = 0; eyes[e].gaze_direction.z = 100;
        eyes[e].fixation_duration = 0; eyes[e].saccade_velocity = 0;
        eyes[e].pupil_diameter = 40; eyes[e].blink_state = 0;
        eyes[e].heatmap_x = 50; eyes[e].heatmap_y = 50; eyes[e].foveated_region = 2;
    }
}

void eye_tracking_update(int eid, vec3_t gaze_dir) {
    if (eid < 1 || eid > MAX_EYES) return;
    int idx = eid - 1;
    int dx_old = eyes[idx].gaze_direction.x, dy_old = eyes[idx].gaze_direction.y;
    eyes[idx].gaze_direction = gaze_dir;
    int dx = gaze_dir.x - dx_old, dy = gaze_dir.y - dy_old;
    eyes[idx].saccade_velocity = isqrt(dx * dx + dy * dy);
    eyes[idx].fixation_duration = eyes[idx].saccade_velocity < 10 ?
        eyes[idx].fixation_duration + 16 : 0;
    if (gaze_dir.z != 0) {
        eyes[idx].gaze_point.x = (gaze_dir.x * 500) / gaze_dir.z;
        eyes[idx].gaze_point.y = (gaze_dir.y * 500) / gaze_dir.z;
        eyes[idx].gaze_point.z = 500;
    }
    eyes[idx].heatmap_x = clamp(50 + eyes[idx].gaze_point.x / 20, 0, 100);
    eyes[idx].heatmap_y = clamp(50 + eyes[idx].gaze_point.y / 20, 0, 100);
    eyes[idx].foveated_region = eyes[idx].fixation_duration > 200 ? 3 :
        (eyes[idx].fixation_duration > 100 ? 2 : 1);
}

/* ===== Scene Graph ===== */

int scene_add_node(int parent, vec3_t pos) {
    if (scene_node_count >= MAX_NODES) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_NODES; i++) if (scene_nodes[i].node_id == 0) { idx = i; break; }
    if (idx < 0) return -1;
    int nid = next_node_id++;
    scene_nodes[idx].node_id = nid; scene_nodes[idx].parent_id = parent;
    scene_nodes[idx].child_count = 0; scene_nodes[idx].position = pos;
    scene_nodes[idx].rotation.x = 0; scene_nodes[idx].rotation.y = 0; scene_nodes[idx].rotation.z = 0;
    scene_nodes[idx].scale.x = 100; scene_nodes[idx].scale.y = 100; scene_nodes[idx].scale.z = 100;
    scene_nodes[idx].lod_level = LOD_HIGH; scene_nodes[idx].visible = 1; scene_nodes[idx].culled = 0;
    scene_nodes[idx].bbox_min.x = pos.x - 50; scene_nodes[idx].bbox_min.y = pos.y - 50;
    scene_nodes[idx].bbox_min.z = pos.z - 50; scene_nodes[idx].bbox_max.x = pos.x + 50;
    scene_nodes[idx].bbox_max.y = pos.y + 50; scene_nodes[idx].bbox_max.z = pos.z + 50;
    for (int i = 0; i < 16; i++) scene_nodes[idx].world_transform.m[i] = (i % 5 == 0) ? 100 : 0;
    if (parent > 0) {
        for (int i = 0; i < MAX_NODES; i++) {
            if (scene_nodes[i].node_id == parent && scene_nodes[i].child_count < 8) {
                scene_nodes[i].children[scene_nodes[i].child_count++] = nid; break;
            }
        }
    }
    scene_node_count++;
    return nid;
}

void scene_update_lod(int nid, int cam_dist) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (scene_nodes[i].node_id != nid) continue;
        scene_nodes[i].lod_level = cam_dist < 500 ? LOD_HIGH :
            (cam_dist < 1500 ? LOD_MEDIUM : LOD_LOW);
        break;
    }
}

void scene_occlusion_cull(void) {
    for (int i = 0; i < HASH_BUCKETS; i++) spatial_hash[i] = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        if (scene_nodes[i].node_id == 0) continue;
        int h = ((scene_nodes[i].position.x / 100) + (scene_nodes[i].position.y / 100) +
                 (scene_nodes[i].position.z / 100)) % HASH_BUCKETS;
        if (h < 0) h = -h;
        if (spatial_hash[h] > 0) { scene_nodes[i].culled = 1; }
        else { spatial_hash[h] = scene_nodes[i].node_id; scene_nodes[i].culled = 0; }
    }
}

/* ===== Physics Simulation ===== */

int physics_create_body(vec3_t pos, int mass, int mat) {
    if (rigid_body_count >= MAX_BODIES) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_BODIES; i++) if (rigid_bodies[i].body_id == 0) { idx = i; break; }
    if (idx < 0) return -1;
    int bid = next_body_id++;
    rigid_bodies[idx].body_id = bid; rigid_bodies[idx].position = pos;
    rigid_bodies[idx].velocity.x = 0; rigid_bodies[idx].velocity.y = 0; rigid_bodies[idx].velocity.z = 0;
    rigid_bodies[idx].acceleration.x = 0; rigid_bodies[idx].acceleration.y = -98;
    rigid_bodies[idx].acceleration.z = 0;
    rigid_bodies[idx].force.x = 0; rigid_bodies[idx].force.y = 0; rigid_bodies[idx].force.z = 0;
    rigid_bodies[idx].mass = mass; rigid_bodies[idx].inverse_mass = mass > 0 ? 100 / mass : 0;
    rigid_bodies[idx].restitution = mat == MAT_RUBBER ? 80 : 30;
    rigid_bodies[idx].friction = mat == MAT_RUBBER ? 90 : 40;
    rigid_bodies[idx].aabb_min.x = pos.x - 25; rigid_bodies[idx].aabb_min.y = pos.y - 25;
    rigid_bodies[idx].aabb_min.z = pos.z - 25; rigid_bodies[idx].aabb_max.x = pos.x + 25;
    rigid_bodies[idx].aabb_max.y = pos.y + 25; rigid_bodies[idx].aabb_max.z = pos.z + 25;
    rigid_bodies[idx].material = mat; rigid_bodies[idx].is_static = (mass == 0);
    rigid_body_count++;
    return bid;
}

int physics_check_collision(int ba, int bb) {
    rigid_body_t *a = 0, *b = 0;
    for (int i = 0; i < MAX_BODIES; i++) {
        if (rigid_bodies[i].body_id == ba) a = &rigid_bodies[i];
        if (rigid_bodies[i].body_id == bb) b = &rigid_bodies[i];
    }
    if (!a || !b) return 0;
    if (a->aabb_max.x < b->aabb_min.x || a->aabb_min.x > b->aabb_max.x) return 0;
    if (a->aabb_max.y < b->aabb_min.y || a->aabb_min.y > b->aabb_max.y) return 0;
    if (a->aabb_max.z < b->aabb_min.z || a->aabb_min.z > b->aabb_max.z) return 0;
    return 1;
}

void physics_step(int dt) {
    for (int i = 0; i < MAX_BODIES; i++) {
        if (rigid_bodies[i].body_id == 0 || rigid_bodies[i].is_static) continue;
        int ax = rigid_bodies[i].acceleration.x +
            (rigid_bodies[i].force.x * rigid_bodies[i].inverse_mass) / 100;
        int ay = rigid_bodies[i].acceleration.y +
            (rigid_bodies[i].force.y * rigid_bodies[i].inverse_mass) / 100;
        int az = rigid_bodies[i].acceleration.z +
            (rigid_bodies[i].force.z * rigid_bodies[i].inverse_mass) / 100;
        rigid_bodies[i].velocity.x += (ax * dt) / 1000;
        rigid_bodies[i].velocity.y += (ay * dt) / 1000;
        rigid_bodies[i].velocity.z += (az * dt) / 1000;
        rigid_bodies[i].position.x += (rigid_bodies[i].velocity.x * dt) / 1000;
        rigid_bodies[i].position.y += (rigid_bodies[i].velocity.y * dt) / 1000;
        rigid_bodies[i].position.z += (rigid_bodies[i].velocity.z * dt) / 1000;
        rigid_bodies[i].aabb_min.x = rigid_bodies[i].position.x - 25;
        rigid_bodies[i].aabb_min.y = rigid_bodies[i].position.y - 25;
        rigid_bodies[i].aabb_min.z = rigid_bodies[i].position.z - 25;
        rigid_bodies[i].aabb_max.x = rigid_bodies[i].position.x + 25;
        rigid_bodies[i].aabb_max.y = rigid_bodies[i].position.y + 25;
        rigid_bodies[i].aabb_max.z = rigid_bodies[i].position.z + 25;
        rigid_bodies[i].force.x = 0; rigid_bodies[i].force.y = 0; rigid_bodies[i].force.z = 0;
    }
}

void cloth_init_grid(int cols, int rows, int spacing) {
    cloth_point_count = 0;
    for (int r = 0; r < rows && cloth_point_count < MAX_CLOTH; r++)
        for (int c = 0; c < cols && cloth_point_count < MAX_CLOTH; c++) {
            int i = cloth_point_count++;
            cloth_points[i].position.x = c * spacing; cloth_points[i].position.y = r * spacing;
            cloth_points[i].position.z = 0; cloth_points[i].old_position = cloth_points[i].position;
            cloth_points[i].acceleration.x = 0; cloth_points[i].acceleration.y = -98;
            cloth_points[i].acceleration.z = 0; cloth_points[i].mass = 10;
            cloth_points[i].pinned = (r == 0 && (c == 0 || c == cols - 1));
        }
}

void cloth_simulate_step(int dt) {
    for (int i = 0; i < cloth_point_count; i++) {
        if (cloth_points[i].pinned) continue;
        int vx = cloth_points[i].position.x - cloth_points[i].old_position.x;
        int vy = cloth_points[i].position.y - cloth_points[i].old_position.y;
        int vz = cloth_points[i].position.z - cloth_points[i].old_position.z;
        cloth_points[i].old_position = cloth_points[i].position;
        int dt2 = dt * dt;
        cloth_points[i].position.x += vx + (cloth_points[i].acceleration.x * dt2) / 1000000;
        cloth_points[i].position.y += vy + (cloth_points[i].acceleration.y * dt2) / 1000000;
        cloth_points[i].position.z += vz + (cloth_points[i].acceleration.z * dt2) / 1000000;
    }
}

/* ===== CLI Entry ===== */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("AR/VR Engine v54.0 - Immersive Rendering System\n");
    if (help) {
        print_str("Usage: ar_vr_engine [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run comprehensive tests\n");
        print_str("Features: stereoscopic rendering, HRTF audio, hand tracking,\n");
        print_str("  eye tracking, scene graph, rigid body physics, cloth simulation\n");
        return;
    }
    if (test) {
        print_str("=== AR/VR Engine Test Suite ===\n\n");
        /* Test 1: VR Init */
        print_str("[Test 1] VR Context Init\n");
        vr_init(3840, 2160, 110, 64);
        print_str("  HMD: "); print_int(vr_ctx.hmd_width); print_str("x");
        print_int(vr_ctx.hmd_height); print_str(" FOV="); print_int(vr_ctx.fov);
        print_str(" IPD="); print_int(vr_ctx.ipd); print_str("\n");
        print_str("  Left: "); print_int(vr_ctx.eyes[EYE_LEFT].width); print_str("x");
        print_int(vr_ctx.eyes[EYE_LEFT].height);
        print_str(" Right: "); print_int(vr_ctx.eyes[EYE_RIGHT].width); print_str("x");
        print_int(vr_ctx.eyes[EYE_RIGHT].height); print_str("\n");
        print_str("  Parallax@1000: L="); print_int(vr_parallax(1000, EYE_LEFT));
        print_str(" R="); print_int(vr_parallax(1000, EYE_RIGHT)); print_str("\n\n");
        /* Test 2: Spatial Audio */
        print_str("[Test 2] Spatial Audio HRTF\n");
        vec3_t src_pos = {500, 200, 300};
        int asrc = audio_create_source(src_pos, 80);
        print_str("  Source "); print_int(asrc); print_str(" at (500,200,300) vol=80\n");
        vec3_t listen = {0, 0, 0};
        audio_update_hrtf(asrc, listen);
        print_str("  Dist="); print_int(audio_sources[0].distance);
        print_str(" Az="); print_int(audio_sources[0].hrtf_azimuth);
        print_str(" El="); print_int(audio_sources[0].hrtf_elevation);
        print_str(" Gain="); print_int(audio_sources[0].hrtf_gain); print_str("\n\n");
        /* Test 3: Hand Tracking */
        print_str("[Test 3] Hand Tracking\n");
        hand_tracking_init();
        vec3_t palm = {100, 200, 300};
        hand_tracking_simulate(1, palm);
        print_str("  Hand 1 tracked="); print_int(hands[0].is_tracked);
        print_str(" gesture="); print_str(gesture_name(hands[0].gesture));
        print_str(" conf="); print_int(hands[0].gesture_confidence); print_str("%\n");
        print_str("  Joints="); print_int(JOINTS_PER_HAND);
        print_str(" palm=("); print_int(hands[0].palm_position.x); print_str(",");
        print_int(hands[0].palm_position.y); print_str(",");
        print_int(hands[0].palm_position.z); print_str(")\n");
        print_str("  Joint[0] conf="); print_int(hands[0].joints[0].confidence);
        print_str(" Joint[20] conf="); print_int(hands[0].joints[20].confidence); print_str("\n\n");
        /* Test 4: Eye Tracking */
        print_str("[Test 4] Eye Tracking\n");
        eye_tracking_init();
        vec3_t gaze = {50, -30, 500};
        eye_tracking_update(1, gaze);
        print_str("  Eye 1 gaze=("); print_int(eyes[0].gaze_point.x); print_str(",");
        print_int(eyes[0].gaze_point.y); print_str(",");
        print_int(eyes[0].gaze_point.z); print_str(")\n");
        print_str("  Saccade="); print_int(eyes[0].saccade_velocity);
        print_str(" Fix="); print_int(eyes[0].fixation_duration);
        print_str("ms Fov="); print_int(eyes[0].foveated_region); print_str("\n");
        print_str("  Heatmap=("); print_int(eyes[0].heatmap_x); print_str(",");
        print_int(eyes[0].heatmap_y); print_str(")\n\n");
        /* Test 5: Scene Graph */
        print_str("[Test 5] Scene Graph\n");
        vec3_t rp = {0, 0, 0}; int root = scene_add_node(0, rp);
        print_str("  Root="); print_int(root); print_str("\n");
        vec3_t c1p = {200, 0, 500}; int c1 = scene_add_node(root, c1p);
        vec3_t c2p = {-200, 100, 600}; int c2 = scene_add_node(root, c2p);
        print_str("  Children: "); print_int(c1); print_str(", "); print_int(c2); print_str("\n");
        print_str("  Parent has "); print_int(scene_nodes[0].child_count); print_str(" children\n");
        scene_update_lod(c1, 300); scene_update_lod(c2, 1200);
        print_str("  LOD: n"); print_int(c1); print_str("=");
        print_int(scene_nodes[1].lod_level); print_str(" n");
        print_int(c2); print_str("="); print_int(scene_nodes[2].lod_level); print_str("\n");
        scene_occlusion_cull();
        print_str("  Cull pass: nodes="); print_int(scene_node_count); print_str("\n\n");
        /* Test 6: Physics */
        print_str("[Test 6] Physics Simulation\n");
        vec3_t b1p = {0, 1000, 0}; int b1 = physics_create_body(b1p, 50, MAT_RUBBER);
        vec3_t b2p = {100, 0, 0}; int b2 = physics_create_body(b2p, 0, MAT_METAL);
        print_str("  Body "); print_int(b1); print_str(" mass=50 rubber, Body ");
        print_int(b2); print_str(" static=metal\n");
        physics_step(16);
        print_str("  Step1 Y="); print_int(rigid_bodies[0].position.y);
        print_str(" VY="); print_int(rigid_bodies[0].velocity.y); print_str("\n");
        physics_step(16);
        print_str("  Step2 Y="); print_int(rigid_bodies[0].position.y);
        print_str(" VY="); print_int(rigid_bodies[0].velocity.y); print_str("\n");
        physics_step(16);
        print_str("  Step3 Y="); print_int(rigid_bodies[0].position.y);
        print_str(" VY="); print_int(rigid_bodies[0].velocity.y); print_str("\n");
        print_str("  Collision("); print_int(b1); print_str(","); print_int(b2);
        print_str(")="); print_int(physics_check_collision(b1, b2)); print_str("\n\n");
        /* Test 7: Cloth */
        print_str("[Test 7] Cloth Simulation\n");
        cloth_init_grid(5, 5, 50);
        print_str("  Grid: "); print_int(cloth_point_count); print_str(" pts, pinned corners\n");
        cloth_simulate_step(16);
        print_str("  Step1: pt[2] Y="); print_int(cloth_points[2].position.y); print_str("\n");
        cloth_simulate_step(16);
        print_str("  Step2: pt[2] Y="); print_int(cloth_points[2].position.y); print_str("\n");
        cloth_simulate_step(16);
        print_str("  Step3: pt[2] Y="); print_int(cloth_points[2].position.y); print_str("\n\n");
        /* Test 8: Frustum Culling */
        print_str("[Test 8] Frustum Culling\n");
        vec3_t in_min = {100, 100, 100}, in_max = {200, 200, 200};
        vec3_t out_min = {5000, 5000, 5000}, out_max = {6000, 6000, 6000};
        print_str("  Visible culled="); print_int(vr_frustum_cull(in_min, in_max, EYE_LEFT));
        print_str(" OutView culled=");
        print_int(vr_frustum_cull(out_min, out_max, EYE_LEFT)); print_str("\n\n");
        print_str("=== AR/VR Engine Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
