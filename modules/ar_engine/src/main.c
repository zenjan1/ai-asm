/* ar_engine - Augmented Reality Engine WASM Module - AI-ASM OS */
#include <stddef.h>
/* ─── Host Imports ──────────────────────────────────────── */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* ─── Constants ─────────────────────────────────────────── */
#define AR_MAX_PLANES   32U
#define AR_MAX_FEATURES 512U
#define AR_MAX_GESTURES 16U
#define AR_MAX_ANCHORS  64U
#define AR_MAX_MODELS   48U
#define AR_MAX_LIGHTS    8U
#define AR_MAX_USERS     4U
#define AR_MAX_MAP_PTS 4096U
#define AR_MAX_LANDMARKS 64U
#define AR_SCENE_NONE    0x00000000U
#define AR_SCENE_INDOOR  0x00000001U
#define AR_SCENE_OUTDOOR 0x00000002U
#define AR_SCENE_FACIAL  0x00000004U
#define AR_SCENE_BODY    0x00000008U
#define AR_SCENE_OBJECT  0x00000010U
#define AR_SCENE_CUSTOM  0x80000000U
#define AR_DEV_MOBILE     0x01U
#define AR_DEV_GLASSES    0x02U
#define AR_DEV_PROJECTION 0x04U
#define AR_DEV_HEADSET    0x08U
#define AR_GEST_TAP 0x01U
#define AR_GEST_PINCH 0x02U
#define AR_GEST_SWIPE 0x04U
#define AR_GEST_ROTATE 0x08U
#define AR_GEST_GRAB 0x10U
#define AR_GEST_WAVE 0x20U
#define AR_APP_TRYON 0x01U
#define AR_APP_NAV 0x02U
#define AR_APP_EDU 0x04U
#define AR_APP_GAME 0x08U
#define AR_APP_INDUSTRIAL 0x10U
#define AR_TRACK_OK 0x00U
#define AR_TRACK_LOST 0x01U
#define AR_TRACK_INIT 0x02U
#define AR_FL_DEPTH 0x00000001U
#define AR_FL_OCCLUSION 0x00000002U
#define AR_FL_SHADOW 0x00000004U
#define AR_FL_RECON 0x00000008U
#define AR_FL_MULTI 0x00000010U
/* ─── Data Structures ───────────────────────────────────── */
typedef struct { float x, y, z; }       Vec3;
typedef struct { float x, y, z, w; }    Quat;
typedef struct { Vec3 pos; Quat rot; float scale; } Pose;
typedef struct { Vec3 normal; float width, length; Pose pose; unsigned int flags; } ARPlane;
typedef struct { Vec3 position; float confidence; unsigned int id, descriptor; } ARFeature;
typedef struct { unsigned int id, type; Pose pose; float confidence; } ARAnchor;
typedef struct { float intensity, ambient, temperature; Vec3 direction, color; } ARLight;
typedef struct { Vec3 position; Quat rotation; float velocity[3]; unsigned int tracking_state, frame_count; } ARCamera;
typedef struct { unsigned int type, object_id; Pose pose; float confidence; } ARGesture;
typedef struct { unsigned int user_id, active, session_id; Pose pose; } ARPeer;
typedef struct { unsigned int scene_type, device_type, app_mode, frame_id, flags, fps; } ARState;
/* ─── Static State ──────────────────────────────────────── */
static ARPlane ar_planes[AR_MAX_PLANES]; static unsigned int ar_plane_count;
static ARFeature ar_features[AR_MAX_FEATURES]; static unsigned int ar_feature_count;
static ARAnchor ar_anchors[AR_MAX_ANCHORS]; static unsigned int ar_anchor_count;
static ARLight ar_lights[AR_MAX_LIGHTS]; static unsigned int ar_light_count;
static ARGesture ar_gestures[AR_MAX_GESTURES]; static unsigned int ar_gesture_count;
static ARPeer ar_peers[AR_MAX_USERS]; static unsigned int ar_peer_count;
static ARCamera ar_camera; static ARState ar_state;
static float ar_map_pts[AR_MAX_MAP_PTS * 3U]; static unsigned int ar_map_count;
static float ar_landmarks[AR_MAX_LANDMARKS * 3U]; static unsigned int ar_landmark_count;/* ─── Forward Declarations ──────────────────────────────── */
static float ar_sinf(float x), ar_cosf(float x), ar_sqrtf(float x);
static float vec3_dot(Vec3 a, Vec3 b), vec3_distance(Vec3 a, Vec3 b);
static Vec3 vec3_normalize(Vec3 v);
static Quat quat_from_axis_angle(Vec3 axis, float angle);
static unsigned int ar_detect_planes(unsigned int fp, unsigned int w, unsigned int h),
    ar_recognize_image(unsigned int ip, unsigned int len),
    ar_track_object(unsigned int oid, unsigned int fp),
    ar_reconstruct_scene(unsigned int fp, unsigned int len),
    ar_estimate_lighting(unsigned int fp),
    ar_render_model(unsigned int mid, Pose *pose),
    ar_apply_animation(unsigned int aid, float dt),
    ar_match_shadows(Pose *lp),
    ar_handle_occlusion(unsigned int aid, unsigned int dp),
    ar_composite_depth(unsigned int la, unsigned int lb),
    ar_recognize_gesture(unsigned int fp),
    ar_track_gaze(float gx, float gy),
    ar_process_spatial_tap(Vec3 *wp),
    ar_process_voice(unsigned int cp, unsigned int len),
    ar_physical_interaction(unsigned int oid, Vec3 *force),
    ar_visual_odometry(unsigned int fp),
    ar_match_features(unsigned int fa, unsigned int fb, unsigned int n),
    ar_build_map(void),
    ar_relocalize(unsigned int lp, unsigned int cnt),
    ar_sync_multiuser(unsigned int uid),
    ar_init_mobile(void), ar_init_glasses(void), ar_init_projection(void),
    ar_fuse_sensors(unsigned int mask),
    ar_optimize_performance(unsigned int tfps),
    ar_tryon_garment(unsigned int gid, unsigned int bid),
    ar_navigation_overlay(unsigned int rp, unsigned int steps),
    ar_education_scene(unsigned int tid),
    ar_gaming_spawn(unsigned int eid, Vec3 *pos),
    ar_industrial_inspect(unsigned int pid, unsigned int sp);
static void ar_init(unsigned int dt, unsigned int am),
    ar_update(unsigned int fp, unsigned int w, unsigned int h),
    ar_shutdown(void);
/* ─── Math ──────────────────────────────────────────────── */
static float ar_sinf(float x) {
    float pi = 3.14159265f, tw = 6.28318530f;
    while (x > pi)  { x -= tw; }
    while (x < -pi) { x += tw; }
    float x2 = x*x, x3 = x2*x, x5 = x3*x2, x7 = x5*x2, x9 = x7*x2;
    return x - x3/6.0f + x5/120.0f - x7/5040.0f + x9/362880.0f;
}
static float ar_cosf(float x) { return ar_sinf(x + 1.57079632f); }
static float ar_sqrtf(float x) {
    if (x <= 0.0f) { return 0.0f; }
    float g = x * 0.5f;
    for (unsigned int i = 0U; i < 8U; i++) { g = (g + x/g) * 0.5f; }
    return g;
}
/* ─── Utility ───────────────────────────────────────────── */
static float vec3_dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static Vec3 vec3_normalize(Vec3 v) {
    float ls = vec3_dot(v, v);
    float inv = (ls > 0.0001f) ? (1.0f / ar_sqrtf(ls)) : 0.0f;
    Vec3 o = { v.x*inv, v.y*inv, v.z*inv };
    return o;
}
static float vec3_distance(Vec3 a, Vec3 b) {
    float dx = a.x-b.x, dy = a.y-b.y, dz = a.z-b.z;
    return ar_sqrtf(dx*dx + dy*dy + dz*dz);
}
static Quat quat_from_axis_angle(Vec3 axis, float angle) {
    Vec3 n = vec3_normalize(axis);
    float h = angle*0.5f, s = ar_sinf(h);
    Quat q = { n.x*s, n.y*s, n.z*s, ar_cosf(h) };
    return q;
}
/* ─── Scene Recognition ─────────────────────────────────── */
static unsigned int ar_detect_planes(unsigned int fp, unsigned int w, unsigned int h) {
    (void)fp;
    if (w == 0U || h == 0U || ar_plane_count >= AR_MAX_PLANES) { return 1U; }
    unsigned int idx = ar_plane_count;
    Vec3 up = { 0.0f, 1.0f, 0.0f };
    ar_planes[idx].normal = vec3_normalize(up);
    ar_planes[idx].width = 2.0f * (float)w / (float)h;
    ar_planes[idx].length = 2.0f;
    ar_planes[idx].pose.pos = (Vec3){ 0.0f, 0.0f, -1.5f };
    ar_planes[idx].pose.rot = quat_from_axis_angle(up, 0.0f);
    ar_planes[idx].pose.scale = 1.0f;
    ar_planes[idx].flags = 0x00000001U;
    ar_plane_count = idx + 1U;
    ar_state.scene_type |= AR_SCENE_INDOOR;
    host_print("[ar_engine] plane detected\n");
    return 0U;
}
static unsigned int ar_recognize_image(unsigned int ip, unsigned int len) {
    (void)ip;
    if (len == 0U || ar_anchor_count >= AR_MAX_ANCHORS) { return 1U; }
    unsigned int id = ar_anchor_count;
    ar_anchors[id].id = 0xA0000000U | id;
    ar_anchors[id].type = AR_SCENE_OBJECT;
    ar_anchors[id].pose = (Pose){ { 0.0f, 0.0f, -1.0f }, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    ar_anchors[id].confidence = 0.85f;
    ar_anchor_count = id + 1U;
    host_print("[ar_engine] image recognized\n");
    return 0U;
}
static unsigned int ar_track_object(unsigned int oid, unsigned int fp) {
    (void)fp;
    for (unsigned int i = 0U; i < ar_anchor_count; i++) {
        if (ar_anchors[i].id == oid) {
            ar_anchors[i].confidence += 0.01f;
            if (ar_anchors[i].confidence > 1.0f) { ar_anchors[i].confidence = 1.0f; }
            ar_anchors[i].pose.pos.z -= 0.001f;
            return 0U;
        }
    }
    return 1U;
}static unsigned int ar_reconstruct_scene(unsigned int fp, unsigned int len) {
    (void)fp;
    if (len == 0U) { return 1U; }
    ar_state.flags |= AR_FL_RECON;
    for (unsigned int i = 0U; i < 10U && ar_map_count < AR_MAX_MAP_PTS; i++) {
        unsigned int b = ar_map_count * 3U;
        ar_map_pts[b+0] = ((float)i - 5.0f) * 0.1f;
        ar_map_pts[b+1] = 0.0f; ar_map_pts[b+2] = -1.5f;
        ar_map_count++;
    }
    host_print("[ar_engine] scene reconstructed\n");
    return 0U;
}
static unsigned int ar_estimate_lighting(unsigned int fp) {
    (void)fp;
    if (ar_light_count >= AR_MAX_LIGHTS) { return 1U; }
    unsigned int idx = ar_light_count;
    ar_lights[idx].intensity = 1.0f;
    ar_lights[idx].direction = vec3_normalize((Vec3){ 0.3f, -1.0f, 0.2f });
    ar_lights[idx].color = (Vec3){ 1.0f, 0.95f, 0.9f };
    ar_lights[idx].ambient = 0.35f; ar_lights[idx].temperature = 5500.0f;
    ar_light_count = idx + 1U;
    host_print("[ar_engine] lighting estimated\n");
    return 0U;
}
/* ─── Content Rendering ─────────────────────────────────── */
static unsigned int ar_render_model(unsigned int mid, Pose *pose) {
    if (pose == NULL || mid >= AR_MAX_MODELS) { return 1U; }
    Vec3 v = { pose->pos.x - ar_camera.position.x,
               pose->pos.y - ar_camera.position.y,
               pose->pos.z - ar_camera.position.z };
    float d = ar_sqrtf(vec3_dot(v, v));
    if (d > 20.0f || d < 0.05f) { return 2U; }
    host_print("[ar_engine] model rendered\n");
    return 0U;
}
static unsigned int ar_apply_animation(unsigned int aid, float dt) {
    if (dt < 0.0f || dt > 1.0f) { return 1U; }
    float t = (float)aid * 0.1f + dt;
    (void)ar_sinf(t);
    host_print("[ar_engine] animation applied\n");
    return 0U;
}
static unsigned int ar_match_shadows(Pose *lp) {
    if (lp == NULL) { return 1U; }
    Vec3 ld = vec3_normalize(lp->pos);
    float sl = 1.0f / (ld.y + 0.001f);
    if (sl < 0.0f) { sl = -sl; }
    if (sl > 10.0f) { sl = 10.0f; }
    ar_state.flags |= AR_FL_SHADOW;
    host_print("[ar_engine] shadows matched\n");
    return 0U;
}
static unsigned int ar_handle_occlusion(unsigned int aid, unsigned int dp) {
    (void)dp;
    for (unsigned int i = 0U; i < ar_anchor_count; i++) {
        if (ar_anchors[i].id == aid) {
            if (ar_anchors[i].pose.pos.z > 0.0f) { ar_anchors[i].confidence *= 0.5f; }
            ar_state.flags |= AR_FL_OCCLUSION;
            return 0U;
        }
    }
    return 1U;
}
static unsigned int ar_composite_depth(unsigned int la, unsigned int lb) {
    if (la == lb) { return 1U; }
    float da = (float)(la & 0x0000FFFFU) / 65536.0f;
    float db = (float)(lb & 0x0000FFFFU) / 65536.0f;
    (void)((da < db) ? la : lb);
    ar_state.flags |= AR_FL_DEPTH;
    host_print("[ar_engine] depth composited\n");
    return 0U;
}
/* ─── Interaction Design ────────────────────────────────── */
static unsigned int ar_recognize_gesture(unsigned int fp) {
    (void)fp;
    if (ar_gesture_count >= AR_MAX_GESTURES) { return 1U; }
    unsigned int idx = ar_gesture_count;
    ar_gestures[idx].type = AR_GEST_TAP;
    ar_gestures[idx].pose = (Pose){ { 0.0f, 0.0f, -0.5f }, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    ar_gestures[idx].confidence = 0.92f; ar_gestures[idx].object_id = 0U;
    ar_gesture_count = idx + 1U;
    host_print("[ar_engine] gesture recognized\n");
    return 0U;
}
static unsigned int ar_track_gaze(float gx, float gy) {
    if (gx < -1.0f || gx > 1.0f || gy < -1.0f || gy > 1.0f) { return 1U; }
    Vec3 axis = { 0.0f, 1.0f, 0.0f };
    (void)quat_from_axis_angle(axis, gx * 0.5f);
    host_print("[ar_engine] gaze tracked\n");
    return 0U;
}
static unsigned int ar_process_spatial_tap(Vec3 *wp) {
    if (wp == NULL) { return 1U; }
    float min_d = 9999.0f; unsigned int near_id = 0xFFFFFFFFU;
    for (unsigned int i = 0U; i < ar_anchor_count; i++) {
        float d = vec3_distance(*wp, ar_anchors[i].pose.pos);
        if (d < min_d) { min_d = d; near_id = i; }
    }
    if (near_id == 0xFFFFFFFFU || min_d > 2.0f) { return 2U; }
    ar_anchors[near_id].confidence += 0.05f;
    host_print("[ar_engine] spatial tap processed\n");
    return 0U;
}
static unsigned int ar_process_voice(unsigned int cp, unsigned int len) {
    (void)cp;
    if (len == 0U || len > 256U) { return 1U; }
    host_print("[ar_engine] voice command processed\n");
    return 0U;
}
static unsigned int ar_physical_interaction(unsigned int oid, Vec3 *force) {
    if (force == NULL) { return 1U; }
    for (unsigned int i = 0U; i < ar_anchor_count; i++) {
        if (ar_anchors[i].id == oid) {
            float m = ar_sqrtf(vec3_dot(*force, *force));
            if (m > 10.0f) { return 2U; }
            ar_anchors[i].pose.pos.x += force->x * 0.01f;
            ar_anchors[i].pose.pos.y += force->y * 0.01f;
            ar_anchors[i].pose.pos.z += force->z * 0.01f;
            return 0U;
        }
    }
    return 3U;
}/* ─── SLAM Positioning ──────────────────────────────────── */
static unsigned int ar_visual_odometry(unsigned int fp) {
    (void)fp;
    ar_camera.frame_count++;
    ar_camera.position.x += 0.001f * ar_sinf((float)ar_camera.frame_count * 0.01f);
    ar_camera.position.z -= 0.002f;
    ar_camera.tracking_state = AR_TRACK_OK;
    return 0U;
}
static unsigned int ar_match_features(unsigned int fa, unsigned int fb, unsigned int n) {
    if (n == 0U || n > AR_MAX_FEATURES) { return 1U; }
    unsigned int matches = 0U;
    for (unsigned int i = 0U; i < n && i < AR_MAX_FEATURES; i++) {
        if ((float)((fa + i) ^ (fb + i)) / 65536.0f < 0.5f) { matches++; }
    }
    if (n > 0U && matches * 100U / n < 10U) { return 2U; }
    host_print("[ar_engine] features matched\n");
    return 0U;
}
static unsigned int ar_build_map(void) {
    if (ar_feature_count < 4U) { return 1U; }
    ar_map_count = 0U;
    for (unsigned int i = 0U; i < ar_feature_count && ar_map_count < AR_MAX_MAP_PTS; i++) {
        unsigned int b = ar_map_count * 3U;
        ar_map_pts[b+0] = ar_features[i].position.x; ar_map_pts[b+1] = ar_features[i].position.y;
        ar_map_pts[b+2] = ar_features[i].position.z; ar_map_count++;
    }
    host_print("[ar_engine] map built\n");
    return 0U;
}
static unsigned int ar_relocalize(unsigned int lp, unsigned int cnt) {
    (void)lp;
    if (cnt == 0U || cnt > AR_MAX_LANDMARKS) { return 1U; }
    ar_landmark_count = cnt;
    for (unsigned int i = 0U; i < cnt; i++) {
        ar_landmarks[i*3U+0] = (float)i * 0.25f;
        ar_landmarks[i*3U+1] = 0.0f; ar_landmarks[i*3U+2] = -1.0f;
    }
    ar_camera.tracking_state = AR_TRACK_OK;
    host_print("[ar_engine] relocalized\n");
    return 0U;
}
static unsigned int ar_sync_multiuser(unsigned int uid) {
    if (uid >= AR_MAX_USERS) { return 1U; }
    for (unsigned int i = 0U; i < ar_peer_count; i++) {
        if (ar_peers[i].user_id == uid) {
            ar_peers[i].pose.pos = ar_camera.position;
            ar_peers[i].active = 1U; ar_state.flags |= AR_FL_MULTI;
            host_print("[ar_engine] multi-user synced\n");
            return 0U;
        }
    }
    if (ar_peer_count < AR_MAX_USERS) {
        unsigned int idx = ar_peer_count;
        ar_peers[idx].user_id = uid;
        ar_peers[idx].pose.pos = ar_camera.position;
        ar_peers[idx].pose.rot = ar_camera.rotation;
        ar_peers[idx].active = 1U;
        ar_peers[idx].session_id = 0xB0000000U | uid;
        ar_peer_count = idx + 1U;
    }
    ar_state.flags |= AR_FL_MULTI;
    host_print("[ar_engine] multi-user synced\n");
    return 0U;
}
/* ─── Device Adaptation ─────────────────────────────────── */
static unsigned int ar_init_mobile(void) {
    ar_state.device_type = AR_DEV_MOBILE; ar_state.fps = 60U;
    ar_state.flags = AR_FL_DEPTH | AR_FL_OCCLUSION;
    ar_camera.tracking_state = AR_TRACK_INIT;
    host_print("[ar_engine] mobile initialized\n");
    return 0U;
}
static unsigned int ar_init_glasses(void) {
    ar_state.device_type = AR_DEV_GLASSES; ar_state.fps = 90U;
    ar_state.flags = AR_FL_DEPTH | AR_FL_OCCLUSION | AR_FL_SHADOW;
    ar_camera.tracking_state = AR_TRACK_INIT;
    host_print("[ar_engine] glasses initialized\n");
    return 0U;
}
static unsigned int ar_init_projection(void) {
    ar_state.device_type = AR_DEV_PROJECTION; ar_state.fps = 30U;
    ar_state.flags = AR_FL_OCCLUSION;
    ar_camera.tracking_state = AR_TRACK_INIT;
    host_print("[ar_engine] projection initialized\n");
    return 0U;
}
static unsigned int ar_fuse_sensors(unsigned int mask) {
    if (mask == 0U) { return 1U; }
    float gw = 0.6f, aw = 0.3f, cw = 0.1f;
    if (mask & 0x01U) { gw = 0.7f; aw = 0.2f; cw = 0.1f; }
    if (mask & 0x02U) { aw = 0.5f; gw = 0.4f; cw = 0.1f; }
    if (mask & 0x04U) { cw = 0.4f; gw = 0.4f; aw = 0.2f; }
    (void)gw; (void)aw; (void)cw;
    ar_camera.tracking_state = AR_TRACK_OK;
    host_print("[ar_engine] sensors fused\n");
    return 0U;
}
static unsigned int ar_optimize_performance(unsigned int tfps) {
    if (tfps == 0U || tfps > 120U) { return 1U; }
    unsigned int q = (tfps <= 30U) ? 50U : ((tfps <= 60U) ? 75U : 100U);
    ar_state.fps = tfps;
    if (q < 75U && ar_feature_count > AR_MAX_FEATURES / 2U) {
        ar_feature_count = AR_MAX_FEATURES / 2U;
    }
    host_print("[ar_engine] performance optimized\n");
    return 0U;
}
/* ─── Application Integration ───────────────────────────── */
static unsigned int ar_tryon_garment(unsigned int gid, unsigned int bid) {
    if (gid >= AR_MAX_MODELS) { return 1U; }
    Pose p = { { 0.0f, 1.0f, -1.5f }, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    (void)bid;
    unsigned int rc = ar_render_model(gid, &p);
    if (rc != 0U) { return 10U + rc; }
    ar_state.app_mode = AR_APP_TRYON;
    host_print("[ar_engine] garment try-on active\n");
    return 0U;
}
static unsigned int ar_navigation_overlay(unsigned int rp, unsigned int steps) {
    (void)rp;
    if (steps == 0U || steps > 64U) { return 1U; }
    for (unsigned int i = 0U; i < steps && ar_anchor_count < AR_MAX_ANCHORS; i++) {
        unsigned int id = ar_anchor_count;
        ar_anchors[id].id = 0xC0000000U | id; ar_anchors[id].type = AR_APP_NAV;
        ar_anchors[id].pose.pos = (Vec3){ (float)i*0.5f, 0.0f, -1.0f - (float)i*0.3f };
        ar_anchors[id].pose.rot = (Quat){ 0.0f, 0.0f, 0.0f, 1.0f };
        ar_anchors[id].pose.scale = 0.3f; ar_anchors[id].confidence = 1.0f;
        ar_anchor_count = id + 1U;
    }
    ar_state.app_mode = AR_APP_NAV;
    host_print("[ar_engine] navigation overlay active\n");
    return 0U;
}
static unsigned int ar_education_scene(unsigned int tid) {
    if (tid > 0xFFFFU) { return 1U; }
    Pose p = { { 0.0f, 0.5f, -1.2f },
               quat_from_axis_angle((Vec3){ 0.0f, 1.0f, 0.0f }, 0.5f), 1.0f };
    unsigned int rc = ar_render_model(tid & 0x1FU, &p);
    if (rc != 0U) { return 10U + rc; }
    ar_state.app_mode = AR_APP_EDU;
    host_print("[ar_engine] education scene active\n");
    return 0U;
}
static unsigned int ar_gaming_spawn(unsigned int eid, Vec3 *pos) {
    if (pos == NULL || eid >= AR_MAX_MODELS) { return 1U; }
    Pose p = { *pos, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    unsigned int rc = ar_render_model(eid, &p);
    if (rc != 0U) { return 10U + rc; }
    if (ar_anchor_count < AR_MAX_ANCHORS) {
        unsigned int id = ar_anchor_count;
        ar_anchors[id].id = 0xD0000000U | eid; ar_anchors[id].type = AR_APP_GAME;
        ar_anchors[id].pose = p; ar_anchors[id].confidence = 1.0f;
        ar_anchor_count = id + 1U;
    }
    ar_state.app_mode = AR_APP_GAME;
    host_print("[ar_engine] gaming entity spawned\n");
    return 0U;
}
static unsigned int ar_industrial_inspect(unsigned int pid, unsigned int sp) {
    (void)sp;
    if (pid >= AR_MAX_MODELS) { return 1U; }
    Pose p = { { 0.0f, 0.0f, -0.8f }, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    unsigned int rc = ar_render_model(pid, &p);
    if (rc != 0U) { return 10U + rc; }
    ar_state.app_mode = AR_APP_INDUSTRIAL;
    host_print("[ar_engine] industrial inspection active\n");
    return 0U;
}
/* ─── Lifecycle ─────────────────────────────────────────── */
static void ar_init(unsigned int dt, unsigned int am) {
    ar_plane_count = 0U; ar_feature_count = 0U; ar_anchor_count = 0U;
    ar_light_count = 0U; ar_gesture_count = 0U; ar_peer_count = 0U;
    ar_map_count = 0U; ar_landmark_count = 0U;
    ar_state.scene_type = AR_SCENE_NONE; ar_state.device_type = 0U;
    ar_state.app_mode = am; ar_state.frame_id = 0U;
    ar_state.flags = 0U; ar_state.fps = 0U;
    ar_camera.position = (Vec3){ 0.0f, 0.0f, 0.0f };
    ar_camera.rotation = (Quat){ 0.0f, 0.0f, 0.0f, 1.0f };
    ar_camera.velocity[0] = 0.0f; ar_camera.velocity[1] = 0.0f;
    ar_camera.velocity[2] = 0.0f;
    ar_camera.tracking_state = AR_TRACK_INIT; ar_camera.frame_count = 0U;
    unsigned int rc = 1U;
    if (dt == AR_DEV_MOBILE)      { rc = ar_init_mobile(); }
    else if (dt == AR_DEV_GLASSES)    { rc = ar_init_glasses(); }
    else if (dt == AR_DEV_PROJECTION) { rc = ar_init_projection(); }
    else if (dt == AR_DEV_HEADSET) {
        ar_state.device_type = AR_DEV_HEADSET; ar_state.fps = 72U;
        ar_state.flags = AR_FL_DEPTH | AR_FL_OCCLUSION | AR_FL_SHADOW;
        rc = 0U; host_print("[ar_engine] headset initialized\n");
    } else { host_print("[ar_engine] unknown device\n"); }
    if (rc != 0U) { host_print("[ar_engine] init failed\n"); host_exit(1); }
    host_print("[ar_engine] engine initialized\n");
}
static void ar_update(unsigned int fp, unsigned int w, unsigned int h) {
    ar_state.frame_id++;
    ar_visual_odometry(fp); ar_detect_planes(fp, w, h);
    ar_estimate_lighting(fp); ar_recognize_gesture(fp);
    if (ar_state.flags & AR_FL_MULTI) {
        for (unsigned int i = 0U; i < ar_peer_count; i++) {
            if (ar_peers[i].active) { ar_sync_multiuser(ar_peers[i].user_id); }
        }
    }
    if (ar_light_count > 0U) {
        Pose lp = { ar_lights[0].direction, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
        ar_match_shadows(&lp);
    }
    host_print("[ar_engine] frame updated\n");
}
static void ar_shutdown(void) {
    ar_plane_count = 0U; ar_feature_count = 0U; ar_anchor_count = 0U;
    ar_light_count = 0U; ar_gesture_count = 0U; ar_peer_count = 0U;
    ar_map_count = 0U; ar_landmark_count = 0U;
    ar_state.flags = 0U; ar_state.scene_type = AR_SCENE_NONE;
    host_print("[ar_engine] engine shutdown\n");
}
/* ─── Entry Point ───────────────────────────────────────── */
__attribute__((export_name("main")))
int main(void) {
    char buf[64]; int al = host_get_argv(0U, sizeof(buf));
    (void)al; (void)buf;
    ar_init(AR_DEV_MOBILE, AR_APP_TRYON);
    unsigned int df = 0xDEAD0001U, di = 0xBEEF0002U, dd = 0xCAFE0003U;
    ar_update(df, 1280U, 720U); ar_recognize_image(di, 256U);
    Vec3 tap = { 0.1f, 0.0f, -1.0f }; ar_process_spatial_tap(&tap);
    Vec3 force = { 0.5f, 0.0f, 0.0f };
    if (ar_anchor_count > 0U) { ar_physical_interaction(ar_anchors[0].id, &force); }
    ar_build_map(); ar_fuse_sensors(0x07U); ar_optimize_performance(60U);
    Pose mp = { { 0.0f, 0.5f, -1.5f }, (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f };
    ar_render_model(0U, &mp); ar_apply_animation(0U, 0.016f);
    ar_handle_occlusion(ar_anchor_count > 0U ? ar_anchors[0].id : 0U, dd);
    ar_composite_depth(0x00010000U, 0x00020000U);
    ar_track_gaze(0.1f, -0.2f); ar_process_voice(0xAABB0004U, 16U);
    ar_relocalize(0xCCDD0005U, 8U); ar_sync_multiuser(1U);
    ar_tryon_garment(2U, 0U); ar_navigation_overlay(0xEEFF0006U, 5U);
    ar_education_scene(3U);
    Vec3 sp = { 0.5f, 0.0f, -1.0f };
    ar_gaming_spawn(4U, &sp); ar_industrial_inspect(5U, 0x11220007U);
    ar_reconstruct_scene(df, 1024U);
    ar_track_object(ar_anchor_count > 0U ? ar_anchors[0].id : 0U, df);
    ar_shutdown(); host_print("[ar_engine] ar_engine complete\n");
    return 0;
}
