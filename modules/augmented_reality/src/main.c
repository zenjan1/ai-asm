/* augmented_reality: AR tracking and rendering system (v1.0)
 * Marker tracking, spatial mapping, 3D registration, interaction, content
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

#define MAX_MARKERS   8
#define MAX_ANCHORS   16
#define MAX_OBJECTS   8
#define MAX_PLANES    8
#define MAX_FEATURES  64
#define MAX_MESH_VERT 32

#define TRACK_ARUCO   0x01
#define TRACK_APRIL   0x02
#define TRACK_MARKER  0x03
#define TRACK_SLAM    0x04
#define TRACK_PLANE   0x05

#define GESTURE_TAP     0x01
#define GESTURE_PINCH   0x02
#define GESTURE_SWIPE   0x03
#define GESTURE_GRAB    0x04
#define GESTURE_ROTATE  0x05

#define CONTENT_MODEL   0x01
#define CONTENT_IMAGE   0x02
#define CONTENT_INFO    0x03
#define CONTENT_NAV     0x04
#define CONTENT_AUDIO   0x05

typedef struct {
    double x, y;
    double corners[4][2];
    int    id;
    double confidence;
    double distance;
    int    active;
} marker_t;

typedef struct {
    double px, py, pz;
    double rx, ry, rz;
    double qw, qx, qy, qz;
    int    active;
} pose_t;

typedef struct {
    double x, y, z;
    double confidence;
    int    id;
    int    active;
} anchor_t;

typedef struct {
    double x, y, z;
    double rx, ry, rz;
    double sx, sy, sz;
    int    type;
    int    id;
    double distance;
    int    visible;
    int    active;
} ar_object_t;

typedef struct {
    double nx, ny, nz;
    double d;
    double cx, cy, cz;
    double extent_x, extent_y;
    int    id;
    int    active;
} plane_t;

typedef struct {
    double x, y;
    double descriptor[8];
    double score;
    int    id;
    int    matched;
    int    active;
} feature_t;

typedef struct {
    double x, y, z;
    int    id;
    int    active;
} vertex_t;

typedef struct {
    vertex_t verts[MAX_MESH_VERT];
    int    n_verts;
    double center_x, center_y, center_z;
    double extent;
    int    id;
    int    active;
} mesh_t;

typedef struct {
    double px, py, pz;
    double confidence;
    int    gesture_type;
    double gesture_param;
    int    hand_id;
    int    active;
} hand_tracking_t;

typedef struct {
    double gx, gy, gz;
    double enabled;
    int    active;
} gaze_tracking_t;

typedef struct {
    int    track_method;
    int    n_markers;
    int    n_anchors;
    int    n_objects;
    int    n_planes;
    int    n_features;
    int    n_meshes;
    int    tracking_active;
    int    mapping_active;
    int    interaction_active;
    int    fps;
    double tracking_accuracy;
} ar_state_t;

static marker_t markers[MAX_MARKERS];
static pose_t camera_pose;
static anchor_t anchors[MAX_ANCHORS];
static ar_object_t objects[MAX_OBJECTS];
static plane_t planes[MAX_PLANES];
static feature_t features[MAX_FEATURES];
static mesh_t meshes[4];
static hand_tracking_t hand;
static gaze_tracking_t gaze;
static ar_state_t ar;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}
static double drand(int seed) {
    int h = seed ^ 0x5DEECE66D;
    h = (h * 1103515245 + 12345) & 0x7FFFFFFF;
    return (double)(h & 0xFFFF) / 65536.0;
}

int ar_init(void) {
    if (initialized) return -1;
    ar.track_method = 0; ar.n_markers = 0; ar.n_anchors = 0;
    ar.n_objects = 0; ar.n_planes = 0; ar.n_features = 0;
    ar.n_meshes = 0; ar.tracking_active = 0; ar.mapping_active = 0;
    ar.interaction_active = 0; ar.fps = 30; ar.tracking_accuracy = 0.0;
    camera_pose.px = 0.0; camera_pose.py = 0.0; camera_pose.pz = 0.0;
    camera_pose.rx = 0.0; camera_pose.ry = 0.0; camera_pose.rz = 0.0;
    camera_pose.qw = 1.0; camera_pose.qx = 0.0; camera_pose.qy = 0.0; camera_pose.qz = 0.0;
    hand.active = 0; gaze.active = 0;
    for (int i = 0; i < MAX_MARKERS; i++) markers[i].active = 0;
    for (int i = 0; i < MAX_ANCHORS; i++) anchors[i].active = 0;
    for (int i = 0; i < MAX_OBJECTS; i++) objects[i].active = 0;
    for (int i = 0; i < MAX_PLANES; i++) planes[i].active = 0;
    for (int i = 0; i < MAX_FEATURES; i++) features[i].active = 0;
    for (int i = 0; i < 4; i++) meshes[i].active = 0;
    initialized = 1;
    print_str("[AR] Augmented reality system initialized\n");
    return 0;
}

int ar_add_marker(int id, double x, double y, double size) {
    if (ar.n_markers >= MAX_MARKERS) return -1;
    marker_t* m = &markers[ar.n_markers];
    m->id = id; m->x = x; m->y = y;
    m->corners[0][0] = x - size * 0.5; m->corners[0][1] = y - size * 0.5;
    m->corners[1][0] = x + size * 0.5; m->corners[1][1] = y - size * 0.5;
    m->corners[2][0] = x + size * 0.5; m->corners[2][1] = y + size * 0.5;
    m->corners[3][0] = x - size * 0.5; m->corners[3][1] = y + size * 0.5;
    m->confidence = 0.95; m->distance = 1.5;
    m->active = 1;
    ar.n_markers++;
    print_str("[AR] Marker added id="); print_int(id);
    print_str(" pos=("); print_int((int)(x * 100)); print_str(",");
    print_int((int)(y * 100)); print_str(")\n");
    return ar.n_markers - 1;
}

void ar_detect_markers(int method) {
    ar.track_method = method;
    ar.tracking_active = 1;
    print_str("[AR] Marker detection method="); print_int(method); print_str("\n");
    for (int i = 0; i < ar.n_markers; i++) {
        marker_t* m = &markers[i];
        m->confidence = 0.9 + drand(i * 13 + 7) * 0.1;
        m->distance = 1.0 + drand(i * 17 + 3) * 2.0;
        print_str("  Marker "); print_int(m->id);
        print_str(" conf="); print_int((int)(m->confidence * 100));
        print_str("% dist="); print_int((int)(m->distance * 100)); print_str("cm\n");
    }
}

int ar_add_anchor(double x, double y, double z) {
    if (ar.n_anchors >= MAX_ANCHORS) return -1;
    anchor_t* a = &anchors[ar.n_anchors];
    a->x = x; a->y = y; a->z = z;
    a->confidence = 0.98; a->id = ar.n_anchors;
    a->active = 1;
    ar.n_anchors++;
    print_str("[AR] Anchor added: ("); print_int((int)(x * 100));
    print_str(","); print_int((int)(y * 100)); print_str(",");
    print_int((int)(z * 100)); print_str(")\n");
    return ar.n_anchors - 1;
}

void ar_update_camera_pose(double dt) {
    camera_pose.px += 0.01 * dt;
    camera_pose.py += 0.005 * dt;
    camera_pose.ry += 0.02 * dt;
    double half_yaw = camera_pose.ry * 0.5;
    camera_pose.qw = dcos(half_yaw);
    camera_pose.qx = 0.0;
    camera_pose.qy = dsin(half_yaw);
    camera_pose.qz = 0.0;
    print_str("[AR] Camera pose: (");
    print_int((int)(camera_pose.px * 1000)); print_str(",");
    print_int((int)(camera_pose.py * 1000)); print_str(",");
    print_int((int)(camera_pose.pz * 1000)); print_str(")");
    print_str(" yaw="); print_int((int)(camera_pose.ry * 573)); print_str("deg\n");
}

int ar_detect_planes(void) {
    int detected = 0;
    ar.mapping_active = 1;
    for (int i = 0; i < 3 && ar.n_planes < MAX_PLANES; i++) {
        plane_t* p = &planes[ar.n_planes];
        p->nx = 0.0; p->ny = 1.0; p->nz = 0.0;
        p->d = -1.0 - i * 0.5;
        p->cx = drand(i * 11) * 2.0 - 1.0;
        p->cy = -1.0 - i * 0.5;
        p->cz = 2.0 + i * 0.5;
        p->extent_x = 1.0 + drand(i * 13) * 2.0;
        p->extent_y = 1.0 + drand(i * 17) * 2.0;
        p->id = ar.n_planes; p->active = 1;
        ar.n_planes++;
        detected++;
    }
    print_str("[AR] Plane detection: "); print_int(detected);
    print_str(" planes found, total="); print_int(ar.n_planes); print_str("\n");
    return detected;
}

int ar_extract_features(void) {
    int count = 0;
    for (int i = 0; i < 12 && ar.n_features < MAX_FEATURES; i++) {
        feature_t* f = &features[ar.n_features];
        f->x = drand(i * 7) * 4.0 - 2.0;
        f->y = drand(i * 11) * 3.0 - 1.5;
        for (int d = 0; d < 8; d++)
            f->descriptor[d] = drand(i * 13 + d * 3) * 2.0 - 1.0;
        f->score = 0.5 + drand(i * 19) * 0.5;
        f->id = ar.n_features; f->matched = 0; f->active = 1;
        ar.n_features++;
        count++;
    }
    print_str("[AR] Feature extraction: "); print_int(count);
    print_str(" features, total="); print_int(ar.n_features); print_str("\n");
    return count;
}

void ar_match_features(void) {
    int matched = 0;
    for (int i = 0; i < ar.n_features && i < ar.n_features; i++) {
        if (!features[i].active) continue;
        if (features[i].score > 0.7) {
            features[i].matched = 1;
            matched++;
        }
    }
    print_str("[AR] Feature matching: "); print_int(matched);
    print_str("/"); print_int(ar.n_features); print_str(" matched\n");
}

int ar_add_object(int type, double x, double y, double z) {
    if (ar.n_objects >= MAX_OBJECTS) return -1;
    ar_object_t* o = &objects[ar.n_objects];
    o->x = x; o->y = y; o->z = z;
    o->rx = 0.0; o->ry = 0.0; o->rz = 0.0;
    o->sx = 1.0; o->sy = 1.0; o->sz = 1.0;
    o->type = type; o->id = ar.n_objects;
    double dx = x - camera_pose.px;
    double dy = y - camera_pose.py;
    double dz = z - camera_pose.pz;
    o->distance = dsqrt(dx*dx + dy*dy + dz*dz);
    o->visible = o->distance < 5.0 ? 1 : 0;
    o->active = 1;
    ar.n_objects++;
    print_str("[AR] Object added type="); print_int(type);
    print_str(" pos=("); print_int((int)(x * 100));
    print_str(","); print_int((int)(y * 100));
    print_str(","); print_int((int)(z * 100));
    print_str(") dist="); print_int((int)(o->distance * 100)); print_str("cm\n");
    return ar.n_objects - 1;
}

void ar_render_objects(void) {
    int visible = 0;
    for (int i = 0; i < ar.n_objects; i++) {
        ar_object_t* o = &objects[i];
        double dx = o->x - camera_pose.px;
        double dy = o->y - camera_pose.py;
        double dz = o->z - camera_pose.pz;
        o->distance = dsqrt(dx*dx + dy*dy + dz*dz);
        o->visible = o->distance < 5.0 ? 1 : 0;
        if (o->visible) visible++;
    }
    print_str("[AR] Rendering: "); print_int(visible);
    print_str("/"); print_int(ar.n_objects); print_str(" objects visible\n");
    for (int i = 0; i < ar.n_objects; i++) {
        if (!objects[i].visible) continue;
        ar_object_t* o = &objects[i];
        print_str("  Obj "); print_int(o->id);
        print_str(" type="); print_int(o->type);
        print_str(" dist="); print_int((int)(o->distance * 100)); print_str("cm\n");
    }
}

int ar_build_mesh(double cx, double cy, double cz, double extent) {
    if (ar.n_meshes >= 4) return -1;
    mesh_t* m = &meshes[ar.n_meshes];
    m->center_x = cx; m->center_y = cy; m->center_z = cz;
    m->extent = extent; m->id = ar.n_meshes;
    m->n_verts = 0;
    for (int i = 0; i < MAX_MESH_VERT; i++) {
        double ang = 2.0 * 3.14159 * i / MAX_MESH_VERT;
        m->verts[i].x = cx + extent * dcos(ang);
        m->verts[i].y = cy;
        m->verts[i].z = cz + extent * dsin(ang);
        m->verts[i].id = i;
        m->verts[i].active = 1;
        m->n_verts++;
    }
    m->active = 1;
    ar.n_meshes++;
    print_str("[AR] Mesh built: center=(");
    print_int((int)(cx * 100)); print_str(",");
    print_int((int)(cy * 100)); print_str(",");
    print_int((int)(cz * 100));
    print_str(") verts="); print_int(m->n_verts); print_str("\n");
    return ar.n_meshes - 1;
}

void ar_hand_tracking(double hx, double hy, double hz, int gesture) {
    hand.px = hx; hand.py = hy; hand.pz = hz;
    hand.confidence = 0.92;
    hand.gesture_type = gesture;
    hand.gesture_param = 0.0;
    if (gesture == GESTURE_PINCH) hand.gesture_param = 0.3;
    else if (gesture == GESTURE_ROTATE) hand.gesture_param = 45.0;
    hand.hand_id = 0; hand.active = 1;
    ar.interaction_active = 1;
    print_str("[AR] Hand: (");
    print_int((int)(hx * 100)); print_str(",");
    print_int((int)(hy * 100)); print_str(",");
    print_int((int)(hz * 100));
    print_str(") gesture="); print_int(gesture);
    print_str(" conf="); print_int((int)(hand.confidence * 100)); print_str("%\n");
}

void ar_gaze_tracking(double gx, double gy, double gz) {
    gaze.gx = gx; gaze.gy = gy; gaze.gz = gz;
    gaze.enabled = 1; gaze.active = 1;
    print_str("[AR] Gaze: (");
    print_int((int)(gx * 1000)); print_str(",");
    print_int((int)(gy * 1000)); print_str(",");
    print_int((int)(gz * 1000)); print_str(")mm\n");
}

void ar_slam_tracking(void) {
    ar.track_method = TRACK_SLAM;
    ar.tracking_active = 1;
    ar_extract_features();
    ar_match_features();
    ar_update_camera_pose(1.0);
    double accuracy = 0.98 - drand(ar.n_features) * 0.02;
    ar.tracking_accuracy = accuracy;
    print_str("[AR] SLAM tracking: accuracy=");
    print_int((int)(accuracy * 1000)); print_str("/1000\n");
}

void ar_occlusion_handling(void) {
    int occluded = 0;
    for (int i = 0; i < ar.n_objects; i++) {
        if (!objects[i].active) continue;
        for (int p = 0; p < ar.n_planes; p++) {
            if (!planes[p].active) continue;
            double dot = objects[i].x * planes[p].nx +
                        objects[i].y * planes[p].ny +
                        objects[i].z * planes[p].nz + planes[p].d;
            if (dot < 0) { occluded++; break; }
        }
    }
    print_str("[AR] Occlusion: "); print_int(occluded);
    print_str("/"); print_int(ar.n_objects); print_str(" objects occluded\n");
}

void ar_light_estimation(void) {
    double ambient = 0.7;
    double dir_x = 0.3, dir_y = 0.8, dir_z = 0.5;
    print_str("[AR] Light estimation: ambient=");
    print_int((int)(ambient * 100)); print_str("%");
    print_str(" dir=("); print_int((int)(dir_x * 100));
    print_str(","); print_int((int)(dir_y * 100));
    print_str(","); print_int((int)(dir_z * 100)); print_str(")\n");
}

void ar_print_state(void) {
    print_str("[AR] Markers="); print_int(ar.n_markers);
    print_str(" Anchors="); print_int(ar.n_anchors);
    print_str(" Objects="); print_int(ar.n_objects);
    print_str(" Planes="); print_int(ar.n_planes);
    print_str(" Features="); print_int(ar.n_features);
    print_str(" Meshes="); print_int(ar.n_meshes);
    print_str(" Track="); print_int(ar.track_method);
    print_str(" FPS="); print_int(ar.fps);
    print_str("\n");
}

int main(void) {
    print_str("=== Augmented Reality System Demo ===\n\n");
    ar_init();

    print_str("Adding ArUco markers...\n");
    ar_add_marker(1, 0.0, 0.0, 0.15);
    ar_add_marker(2, 0.5, 0.0, 0.15);
    ar_add_marker(3, 0.0, 0.5, 0.15);

    print_str("\nAprilTag markers...\n");
    ar_add_marker(10, 1.0, 0.0, 0.1);
    ar_add_marker(11, 1.0, 0.5, 0.1);

    print_str("\nMarker detection (ArUco)...\n");
    ar_detect_markers(TRACK_ARUCO);

    print_str("\nMarker detection (AprilTag)...\n");
    ar_detect_markers(TRACK_APRIL);

    print_str("\nAdding spatial anchors...\n");
    ar_add_anchor(0.0, 0.0, 0.0);
    ar_add_anchor(1.0, 0.0, 0.0);
    ar_add_anchor(0.0, 1.0, 0.0);
    ar_add_anchor(0.0, 0.0, 1.0);

    print_str("\nSLAM tracking...\n");
    ar_slam_tracking();

    print_str("\nCamera pose update...\n");
    for (int i = 0; i < 5; i++) ar_update_camera_pose(0.1);

    print_str("\nPlane detection...\n");
    ar_detect_planes();

    print_str("\nFeature extraction and matching...\n");
    ar_extract_features();
    ar_match_features();

    print_str("\nAdding AR content...\n");
    ar_add_object(CONTENT_MODEL, 0.5, 0.0, 1.5);
    ar_add_object(CONTENT_INFO, 1.0, 0.5, 1.2);
    ar_add_object(CONTENT_NAV, 0.0, 1.0, 1.0);

    print_str("\nBuilding spatial mesh...\n");
    ar_build_mesh(0.0, 0.0, 2.0, 1.5);
    ar_build_mesh(1.0, 0.5, 2.5, 1.0);

    print_str("\nRendering AR objects...\n");
    ar_render_objects();

    print_str("\nOcclusion handling...\n");
    ar_occlusion_handling();

    print_str("\nLight estimation...\n");
    ar_light_estimation();

    print_str("\nHand tracking - tap gesture...\n");
    ar_hand_tracking(0.3, 0.2, 0.5, GESTURE_TAP);

    print_str("\nHand tracking - pinch gesture...\n");
    ar_hand_tracking(0.35, 0.25, 0.5, GESTURE_PINCH);

    print_str("\nHand tracking - rotate gesture...\n");
    ar_hand_tracking(0.4, 0.3, 0.5, GESTURE_ROTATE);

    print_str("\nGaze tracking...\n");
    ar_gaze_tracking(0.5, 0.3, 2.0);
    ar_gaze_tracking(0.6, 0.4, 2.2);

    ar_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
