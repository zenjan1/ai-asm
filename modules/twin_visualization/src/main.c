/* twin_visualization - Digital Twin Visualization Module for AI-ASM OS */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_SCENES 16
#define MAX_OBJECTS 256
#define MAX_LIGHTS 32
#define MAX_MATERIALS 64
#define MAX_ANIMATIONS 128
#define MAX_USERS 32
#define MAX_ANNOTATIONS 256
#define MAX_COMMENTS 512
#define MAX_PRESETS 64
#define MAX_PATH_POINTS 1024
#define MAX_FRAMES 4096
#define MAX_STREAMLINES 256
#define MAX_VECTOR_FIELDS 64
#define MAX_SNAPSHOTS 128
#define FLAG_VISIBLE 0x01
#define FLAG_SELECTED 0x02
#define FLAG_LOCKED 0x04
#define FLAG_ANIMATED 0x08
#define FLAG_CAST_SHADOW 0x10
#define FLAG_RECEIVE_SHADOW 0x20
#define FLAG_COLLIDABLE 0x40
#define FLAG_MEASURABLE 0x80
#define RENDER_OPAQUE 0x00
#define RENDER_TRANSPARENT 0x01
#define RENDER_ADDITIVE 0x02
#define RENDER_WIREFRAME 0x03
#define LIGHT_DIRECTIONAL 0x00
#define LIGHT_POINT 0x01
#define LIGHT_SPOT 0x02
#define LIGHT_AMBIENT 0x03
#define ANIM_LINEAR 0x00
#define ANIM_EASE_IN 0x01
#define ANIM_EASE_OUT 0x02
#define ANIM_EASE_INOUT 0x03
#define ANIM_BEZIER 0x04
#define ANIM_SPRING 0x05
#define PERM_READ 0x01
#define PERM_WRITE 0x02
#define PERM_ANNOTATE 0x04
#define PERM_ADMIN 0x08
#define MODE_PERSPECTIVE 0x00
#define MODE_ORTHOGRAPHIC 0x01
#define MODE_AR 0x02
#define MODE_VR 0x03
#define EVT_NONE 0x00
#define EVT_CLICK 0x01
#define EVT_DRAG 0x02
#define EVT_ZOOM 0x03
#define EVT_ROTATE 0x04
#define EVT_PAN 0x05
#define EVT_GESTURE 0x06
#define EVT_SELECT 0x07
#define EVT_MEASURE 0x08

typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;
typedef struct { float m[16]; } Mat4;
typedef struct { float r, g, b, a; } Color;
typedef struct { Vec3 position, rotation, scale; } Transform;
typedef struct {
    unsigned int id; char name[32]; Color albedo;
    float metallic, roughness, ao, emissive;
    unsigned int albedo_tex, normal_tex, metallic_tex, roughness_tex, flags;
} Material;
typedef struct {
    unsigned int vertex_count, index_count, vertex_buf, index_buf, material_id;
} Mesh;
typedef struct {
    unsigned int id, flags, parent_id;
    char name[64]; Mesh mesh; Transform transform;
    Vec3 velocity, bounding_min, bounding_max;
} SceneObject;
typedef struct {
    unsigned int id, type, cast_shadows, shadow_map_id;
    Vec3 position, direction; Color color;
    float intensity, range, spot_angle;
} Light;
typedef struct {
    Vec3 position, target, up;
    float fov, near_plane, far_plane, ortho_size;
    unsigned int mode; Mat4 view_matrix, proj_matrix;
} Camera;
typedef struct { float time; Vec3 position, rotation, scale; } Keyframe;
typedef struct {
    unsigned int id, object_id, keyframe_count, interpolation, looping, playing;
    Keyframe keyframes[64]; float duration, speed, current_time;
} Animation;
typedef struct {
    unsigned int id, object_count, light_count, material_count, animation_count;
    char name[64];
    SceneObject objects[MAX_OBJECTS]; Light lights[MAX_LIGHTS];
    Material materials[MAX_MATERIALS]; Animation animations[MAX_ANIMATIONS];
    Vec3 ambient_color; float ambient_intensity;
    unsigned int skybox_tex, active;
} Scene;
typedef struct {
    unsigned int id, type, visible; Vec3 start, end; float distance; char label[64];
} Measurement;
typedef struct { unsigned int id, object_id, visible; Vec3 normal; float offset; } SectionPlane;
typedef struct { Vec3 center; float factor; unsigned int axis_x, axis_y, axis_z, active; } ExplodedView;
typedef struct { unsigned int object_id; float value; Color color; } DataPoint;
typedef struct {
    unsigned int id, point_count, visible; char name[32];
    DataPoint points[MAX_OBJECTS]; float min_value, max_value;
    Color cold_color, hot_color;
} Heatmap;
typedef struct {
    unsigned int id, seed_count, trail_length, visible;
    Vec3 seed_points[32]; Vec3 *trail_points, field_ref;
} Streamline;
typedef struct {
    unsigned int id, resolution_x, resolution_y, resolution_z, visible;
    Vec3 origin, extent; float *data;
} VectorField;
typedef struct {
    unsigned int id, vertex_count, visible; float threshold;
    VectorField *source_field; float *vertices;
} Isosurface;
typedef struct { unsigned int id, scene_id; char name[32]; Camera camera; } PresetView;
typedef struct { Vec3 position, target; float duration; } PathPoint;
typedef struct {
    unsigned int id, point_count, looping, active; char name[32];
    PathPoint points[MAX_PATH_POINTS]; float total_duration, current_time;
} RoamingPath;
typedef struct {
    float timestamp; unsigned int event_type, object_id, user_id; Vec3 param_a, param_b;
} FrameEvent;
typedef struct {
    unsigned int id, frame_count, recording, playing; char name[32];
    FrameEvent frames[MAX_FRAMES]; float duration, current_time;
} Recording;
typedef struct {
    unsigned int active, surface_detected, tracking_ok;
    float opacity, scale_factor; Vec3 anchor_position, anchor_rotation;
} ARConfig;
typedef struct {
    unsigned int active, hand_tracking, haptic_feedback, joint_count;
    unsigned int resolution_x, resolution_y;
    float ipd, refresh_rate; Vec3 head_position, head_rotation;
    Vec3 left_hand[25], right_hand[25];
} VRConfig;
typedef struct { unsigned int type; Vec3 position, direction; float confidence; unsigned int hand_id, finger_id; } Gesture;
typedef struct {
    Vec3 position, rotation, room_min, room_max;
    unsigned int space_type, calibrated; float floor_height;
} SpatialConfig;
typedef struct {
    unsigned int device_type, has_depth_sensor, has_imu, has_gps;
    unsigned int screen_width, screen_height, max_fps; float screen_dpi;
} DeviceCaps;
typedef struct {
    unsigned int id, color, permissions, online; char name[32];
    Vec3 position; float last_active;
} User;
typedef struct {
    unsigned int id, user_id, scene_id, object_id, visible; char text[128];
    Vec3 position; Color color; float created_at;
} Annotation;
typedef struct {
    unsigned int id, user_id, annotation_id, edited; char message[256]; float timestamp;
} Comment;
typedef struct {
    unsigned int id, permissions, user_count; char name[32]; unsigned int user_ids[MAX_USERS];
} PermissionGroup;
typedef struct {
    unsigned int id, scene_id, user_id, object_count, data_size;
    char name[64], description[128]; float timestamp;
} Snapshot;

static Scene g_scenes[MAX_SCENES]; static unsigned int g_scene_count = 0, g_active_scene = 0;
static Camera g_camera; static unsigned int g_render_mode = MODE_PERSPECTIVE;
static Measurement g_measurements[MAX_ANNOTATIONS]; static unsigned int g_measurement_count = 0;
static SectionPlane g_section_planes[8]; static unsigned int g_section_count = 0;
static ExplodedView g_exploded_view;
static Heatmap g_heatmaps[8]; static unsigned int g_heatmap_count = 0;
static Streamline g_streamlines[MAX_STREAMLINES]; static unsigned int g_streamline_count = 0;
static VectorField g_vector_fields[MAX_VECTOR_FIELDS]; static unsigned int g_vector_field_count = 0;
static Isosurface g_isosurfaces[16]; static unsigned int g_isosurface_count = 0;
static PresetView g_presets[MAX_PRESETS]; static unsigned int g_preset_count = 0;
static RoamingPath g_roaming_paths[16]; static unsigned int g_roaming_path_count = 0;
static Recording g_recordings[16]; static unsigned int g_recording_count = 0;
static ARConfig g_ar_config; static VRConfig g_vr_config;
static SpatialConfig g_spatial_config; static DeviceCaps g_device_caps;
static User g_users[MAX_USERS]; static unsigned int g_user_count = 0;
static Annotation g_annotations[MAX_ANNOTATIONS]; static unsigned int g_annotation_count = 0;
static Comment g_comments[MAX_COMMENTS]; static unsigned int g_comment_count = 0;
static PermissionGroup g_perm_groups[16]; static unsigned int g_perm_group_count = 0;
static Snapshot g_snapshots[MAX_SNAPSHOTS]; static unsigned int g_snapshot_count = 0;
static unsigned int g_initialized = 0, g_frame_count = 0; static float g_delta_time = 0.0f;

static void init_default_camera(void);
static void init_default_device_caps(void);
static unsigned int create_scene_internal(const char *name);
static unsigned int add_object_internal(unsigned int scene_id, const char *name);
static unsigned int add_material_internal(unsigned int scene_id, const char *name);
static unsigned int add_light_internal(unsigned int scene_id, unsigned int type);
static void update_transform(Transform *xform, Vec3 pos, Vec3 rot, Vec3 scl);
static void compute_view_matrix(Camera *cam);
static void compute_proj_matrix(Camera *cam);
static void update_animations(Scene *scene, float dt);
static void update_streamlines(float dt);
static void update_roaming_path(float dt);
static void update_recording(float dt);
static void update_collaboration(float dt);
static int find_scene_by_name(const char *name);
static int find_object_by_name(unsigned int scene_id, const char *name);
static void log_msg(const char *msg);
static unsigned int safe_alloc(unsigned int size, unsigned int align);
static void vec3_set(Vec3 *v, float x, float y, float z);
static void color_set(Color *c, float r, float g, float b, float a);
static float vec3_length(Vec3 v);
static Vec3 vec3_sub(Vec3 a, Vec3 b);
static Vec3 vec3_add(Vec3 a, Vec3 b);
static Vec3 vec3_scale(Vec3 v, float s);
static Vec3 vec3_normalize(Vec3 v);
static Vec3 vec3_cross(Vec3 a, Vec3 b);
static float vec3_dot(Vec3 a, Vec3 b);
static void mat4_identity(Mat4 *m);
static int str_eq(const char *a, const char *b);
static unsigned int str_len(const char *s);
static void str_copy(char *dst, const char *src, unsigned int max_len);

static void log_msg(const char *msg) { host_print(msg); }
static unsigned int safe_alloc(unsigned int size, unsigned int align) {
    if (size == 0 || align == 0) return 0;
    return host_alloc(size, align);
}
static void vec3_set(Vec3 *v, float x, float y, float z) { v->x = x; v->y = y; v->z = z; }
static void color_set(Color *c, float r, float g, float b, float a) { c->r = r; c->g = g; c->b = b; c->a = a; }
static float vec3_length(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
static Vec3 vec3_sub(Vec3 a, Vec3 b) { Vec3 r; r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; return r; }
static Vec3 vec3_add(Vec3 a, Vec3 b) { Vec3 r; r.x = a.x + b.x; r.y = a.y + b.y; r.z = a.z + b.z; return r; }
static Vec3 vec3_scale(Vec3 v, float s) { Vec3 r; r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; return r; }
static Vec3 vec3_normalize(Vec3 v) {
    float len = v.x * v.x + v.y * v.y + v.z * v.z; Vec3 r;
    if (len > 0.0001f) { float inv = 1.0f / len; r.x = v.x * inv; r.y = v.y * inv; r.z = v.z * inv; }
    else { r.x = 0.0f; r.y = 0.0f; r.z = 0.0f; } return r;
}
static Vec3 vec3_cross(Vec3 a, Vec3 b) {
    Vec3 r; r.x = a.y * b.z - a.z * b.y; r.y = a.z * b.x - a.x * b.z; r.z = a.x * b.y - a.y * b.x; return r;
}
static float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static void mat4_identity(Mat4 *m) {
    for (unsigned int i = 0; i < 16; i++) m->m[i] = 0.0f;
    m->m[0] = 1.0f; m->m[5] = 1.0f; m->m[10] = 1.0f; m->m[15] = 1.0f;
}
static int str_eq(const char *a, const char *b) {
    unsigned int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; i++; } return a[i] == b[i];
}
static unsigned int str_len(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void str_copy(char *dst, const char *src, unsigned int max_len) {
    unsigned int i = 0;
    while (i < max_len - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void init_default_camera(void) {
    vec3_set(&g_camera.position, 0.0f, 5.0f, 10.0f);
    vec3_set(&g_camera.target, 0.0f, 0.0f, 0.0f);
    vec3_set(&g_camera.up, 0.0f, 1.0f, 0.0f);
    g_camera.fov = 60.0f; g_camera.near_plane = 0.1f; g_camera.far_plane = 1000.0f;
    g_camera.ortho_size = 10.0f; g_camera.mode = MODE_PERSPECTIVE;
    mat4_identity(&g_camera.view_matrix); mat4_identity(&g_camera.proj_matrix);
    compute_view_matrix(&g_camera); compute_proj_matrix(&g_camera);
}
static void compute_view_matrix(Camera *cam) {
    Vec3 f = vec3_normalize(vec3_sub(cam->target, cam->position));
    Vec3 s = vec3_normalize(vec3_cross(f, cam->up));
    Vec3 u = vec3_cross(s, f);
    mat4_identity(&cam->view_matrix);
    cam->view_matrix.m[0] = s.x; cam->view_matrix.m[4] = s.y; cam->view_matrix.m[8] = s.z;
    cam->view_matrix.m[1] = u.x; cam->view_matrix.m[5] = u.y; cam->view_matrix.m[9] = u.z;
    cam->view_matrix.m[2] = -f.x; cam->view_matrix.m[6] = -f.y; cam->view_matrix.m[10] = -f.z;
    cam->view_matrix.m[12] = -vec3_dot(s, cam->position);
    cam->view_matrix.m[13] = -vec3_dot(u, cam->position);
    cam->view_matrix.m[14] = vec3_dot(f, cam->position);
}
static void compute_proj_matrix(Camera *cam) {
    mat4_identity(&cam->proj_matrix);
    if (cam->mode == MODE_ORTHOGRAPHIC) {
        float s = cam->ortho_size, n = cam->near_plane, f = cam->far_plane;
        cam->proj_matrix.m[0] = 1.0f / s; cam->proj_matrix.m[5] = 1.0f / s;
        cam->proj_matrix.m[10] = -2.0f / (f - n); cam->proj_matrix.m[14] = -(f + n) / (f - n);
    } else {
        float fov_rad = cam->fov * 0.01745329f, half_fov = fov_rad * 0.5f, tan_half = half_fov;
        float n = cam->near_plane, f = cam->far_plane, aspect = 1.777778f;
        cam->proj_matrix.m[0] = 1.0f / (tan_half * aspect); cam->proj_matrix.m[5] = 1.0f / tan_half;
        cam->proj_matrix.m[10] = -(f + n) / (f - n); cam->proj_matrix.m[14] = -(2.0f * f * n) / (f - n);
        cam->proj_matrix.m[11] = -1.0f; cam->proj_matrix.m[15] = 0.0f;
    }
}
static unsigned int create_scene_internal(const char *name) {
    if (g_scene_count >= MAX_SCENES) { log_msg("[twin] scene limit reached\n"); return 0xFFFFFFFF; }
    Scene *sc = &g_scenes[g_scene_count]; sc->id = g_scene_count;
    str_copy(sc->name, name, 64); sc->object_count = 0; sc->light_count = 0;
    sc->material_count = 0; sc->animation_count = 0;
    vec3_set(&sc->ambient_color, 0.2f, 0.2f, 0.2f); sc->ambient_intensity = 1.0f;
    sc->skybox_tex = 0; sc->active = 1; g_scene_count++; return sc->id;
}
static int find_scene_by_name(const char *name) {
    for (unsigned int i = 0; i < g_scene_count; i++) if (str_eq(g_scenes[i].name, name)) return (int)i;
    return -1;
}
static int find_object_by_name(unsigned int scene_id, const char *name) {
    if (scene_id >= g_scene_count) return -1;
    Scene *sc = &g_scenes[scene_id];
    for (unsigned int i = 0; i < sc->object_count; i++) if (str_eq(sc->objects[i].name, name)) return (int)i;
    return -1;
}
static unsigned int add_object_internal(unsigned int scene_id, const char *name) {
    if (scene_id >= g_scene_count) return 0xFFFFFFFF;
    Scene *sc = &g_scenes[scene_id];
    if (sc->object_count >= MAX_OBJECTS) return 0xFFFFFFFF;
    SceneObject *obj = &sc->objects[sc->object_count]; obj->id = sc->object_count;
    str_copy(obj->name, name, 64);
    vec3_set(&obj->transform.position, 0.0f, 0.0f, 0.0f);
    vec3_set(&obj->transform.rotation, 0.0f, 0.0f, 0.0f);
    vec3_set(&obj->transform.scale, 1.0f, 1.0f, 1.0f);
    obj->flags = FLAG_VISIBLE | FLAG_CAST_SHADOW | FLAG_RECEIVE_SHADOW; obj->parent_id = 0xFFFFFFFF;
    vec3_set(&obj->velocity, 0.0f, 0.0f, 0.0f);
    vec3_set(&obj->bounding_min, -0.5f, -0.5f, -0.5f);
    vec3_set(&obj->bounding_max, 0.5f, 0.5f, 0.5f);
    obj->mesh.vertex_count = 0; obj->mesh.index_count = 0;
    obj->mesh.vertex_buf = 0; obj->mesh.index_buf = 0; obj->mesh.material_id = 0;
    sc->object_count++; return obj->id;
}
static unsigned int add_material_internal(unsigned int scene_id, const char *name) {
    if (scene_id >= g_scene_count) return 0xFFFFFFFF;
    Scene *sc = &g_scenes[scene_id];
    if (sc->material_count >= MAX_MATERIALS) return 0xFFFFFFFF;
    Material *mat = &sc->materials[sc->material_count]; mat->id = sc->material_count;
    str_copy(mat->name, name, 32); color_set(&mat->albedo, 0.8f, 0.8f, 0.8f, 1.0f);
    mat->metallic = 0.0f; mat->roughness = 0.5f; mat->ao = 1.0f; mat->emissive = 0.0f;
    mat->albedo_tex = 0; mat->normal_tex = 0; mat->metallic_tex = 0;
    mat->roughness_tex = 0; mat->flags = 0; sc->material_count++; return mat->id;
}
static unsigned int add_light_internal(unsigned int scene_id, unsigned int type) {
    if (scene_id >= g_scene_count) return 0xFFFFFFFF;
    Scene *sc = &g_scenes[scene_id];
    if (sc->light_count >= MAX_LIGHTS) return 0xFFFFFFFF;
    Light *lt = &sc->lights[sc->light_count]; lt->id = sc->light_count; lt->type = type;
    vec3_set(&lt->position, 0.0f, 10.0f, 0.0f);
    vec3_set(&lt->direction, 0.0f, -1.0f, 0.0f);
    color_set(&lt->color, 1.0f, 1.0f, 1.0f, 1.0f);
    lt->intensity = 1.0f; lt->range = 50.0f; lt->spot_angle = 45.0f;
    lt->cast_shadows = 1; lt->shadow_map_id = 0; sc->light_count++; return lt->id;
}
static void update_transform(Transform *xform, Vec3 pos, Vec3 rot, Vec3 scl) {
    xform->position = pos; xform->rotation = rot; xform->scale = scl;
}
static void update_animations(Scene *scene, float dt) {
    for (unsigned int i = 0; i < scene->animation_count; i++) {
        Animation *anim = &scene->animations[i];
        if (!anim->playing || anim->keyframe_count < 2) continue;
        anim->current_time += dt * anim->speed;
        if (anim->current_time >= anim->duration) {
            if (anim->looping) anim->current_time -= anim->duration;
            else { anim->current_time = anim->duration; anim->playing = 0; }
        }
        unsigned int k0 = 0;
        for (unsigned int k = 0; k < anim->keyframe_count - 1; k++) {
            if (anim->current_time >= anim->keyframes[k].time &&
                anim->current_time <= anim->keyframes[k + 1].time) { k0 = k; break; }
        }
        Keyframe *kf0 = &anim->keyframes[k0], *kf1 = &anim->keyframes[k0 + 1];
        float seg_dur = kf1->time - kf0->time;
        float t = (seg_dur > 0.0001f) ? (anim->current_time - kf0->time) / seg_dur : 0.0f;
        float eased = t;
        if (anim->interpolation == ANIM_EASE_IN) eased = t * t;
        else if (anim->interpolation == ANIM_EASE_OUT) eased = 1.0f - (1.0f - t) * (1.0f - t);
        else if (anim->interpolation == ANIM_EASE_INOUT)
            eased = (t < 0.5f) ? 2.0f * t * t : 1.0f - 2.0f * (1.0f - t) * (1.0f - t);
        if (anim->object_id < scene->object_count) {
            SceneObject *obj = &scene->objects[anim->object_id];
            obj->transform.position.x = kf0->position.x + (kf1->position.x - kf0->position.x) * eased;
            obj->transform.position.y = kf0->position.y + (kf1->position.y - kf0->position.y) * eased;
            obj->transform.position.z = kf0->position.z + (kf1->position.z - kf0->position.z) * eased;
            obj->transform.rotation.x = kf0->rotation.x + (kf1->rotation.x - kf0->rotation.x) * eased;
            obj->transform.rotation.y = kf0->rotation.y + (kf1->rotation.y - kf0->rotation.y) * eased;
            obj->transform.rotation.z = kf0->rotation.z + (kf1->rotation.z - kf0->rotation.z) * eased;
        }
    }
}
static void update_streamlines(float dt) {
    for (unsigned int i = 0; i < g_streamline_count; i++) {
        Streamline *sl = &g_streamlines[i];
        if (!sl->visible || sl->trail_length == 0) continue; (void)dt;
    }
}
static void update_roaming_path(float dt) {
    for (unsigned int p = 0; p < g_roaming_path_count; p++) {
        RoamingPath *path = &g_roaming_paths[p];
        if (!path->active || path->point_count < 2) continue;
        path->current_time += dt;
        if (path->current_time >= path->total_duration) {
            if (path->looping) path->current_time -= path->total_duration;
            else { path->current_time = path->total_duration; path->active = 0; }
        }
        float accum = 0.0f;
        for (unsigned int i = 0; i < path->point_count - 1; i++) {
            float seg = path->points[i].duration;
            if (path->current_time <= accum + seg || i == path->point_count - 2) {
                float t = (seg > 0.0001f) ? (path->current_time - accum) / seg : 0.0f;
                if (t > 1.0f) t = 1.0f; if (t < 0.0f) t = 0.0f;
                PathPoint *p0 = &path->points[i], *p1 = &path->points[i + 1];
                g_camera.position.x = p0->position.x + (p1->position.x - p0->position.x) * t;
                g_camera.position.y = p0->position.y + (p1->position.y - p0->position.y) * t;
                g_camera.position.z = p0->position.z + (p1->position.z - p0->position.z) * t;
                g_camera.target.x = p0->target.x + (p1->target.x - p0->target.x) * t;
                g_camera.target.y = p0->target.y + (p1->target.y - p0->target.y) * t;
                g_camera.target.z = p0->target.z + (p1->target.z - p0->target.z) * t;
                compute_view_matrix(&g_camera); break;
            }
            accum += seg;
        }
    }
}
static void update_recording(float dt) {
    for (unsigned int i = 0; i < g_recording_count; i++) {
        Recording *rec = &g_recordings[i];
        if (rec->recording) rec->duration += dt;
        if (rec->playing) {
            rec->current_time += dt;
            if (rec->current_time >= rec->duration) { rec->playing = 0; rec->current_time = 0.0f; }
        }
    }
}
static void update_collaboration(float dt) {
    (void)dt;
    for (unsigned int i = 0; i < g_user_count; i++) { if (!g_users[i].online) continue; }
}
static void init_default_device_caps(void) {
    g_device_caps.device_type = 0; g_device_caps.has_depth_sensor = 0;
    g_device_caps.has_imu = 1; g_device_caps.has_gps = 0; g_device_caps.screen_dpi = 96.0f;
    g_device_caps.screen_width = 1920; g_device_caps.screen_height = 1080; g_device_caps.max_fps = 60;
    g_ar_config.active = 0; g_ar_config.opacity = 0.8f;
    vec3_set(&g_ar_config.anchor_position, 0.0f, 0.0f, 0.0f);
    vec3_set(&g_ar_config.anchor_rotation, 0.0f, 0.0f, 0.0f);
    g_ar_config.scale_factor = 1.0f; g_ar_config.surface_detected = 0; g_ar_config.tracking_ok = 0;
    g_vr_config.active = 0; g_vr_config.ipd = 0.064f; g_vr_config.refresh_rate = 90.0f;
    g_vr_config.resolution_x = 2160; g_vr_config.resolution_y = 2160;
    g_vr_config.hand_tracking = 1; g_vr_config.haptic_feedback = 1; g_vr_config.joint_count = 25;
    g_spatial_config.space_type = 0; g_spatial_config.floor_height = 0.0f;
    vec3_set(&g_spatial_config.room_min, -5.0f, 0.0f, -5.0f);
    vec3_set(&g_spatial_config.room_max, 5.0f, 3.0f, 5.0f); g_spatial_config.calibrated = 0;
    g_exploded_view.factor = 0.0f; g_exploded_view.axis_x = 1;
    g_exploded_view.axis_y = 1; g_exploded_view.axis_z = 1; g_exploded_view.active = 0;
    vec3_set(&g_exploded_view.center, 0.0f, 0.0f, 0.0f);
}

__attribute__((export_name("twin_init"))) unsigned int twin_init(void) {
    if (g_initialized) { log_msg("[twin] already initialized\n"); return 1; }
    g_scene_count = 0; g_active_scene = 0; g_frame_count = 0; g_delta_time = 0.016667f;
    g_measurement_count = 0; g_section_count = 0; g_heatmap_count = 0;
    g_streamline_count = 0; g_vector_field_count = 0; g_isosurface_count = 0;
    g_preset_count = 0; g_roaming_path_count = 0; g_recording_count = 0;
    g_user_count = 0; g_annotation_count = 0; g_comment_count = 0;
    g_perm_group_count = 0; g_snapshot_count = 0;
    init_default_camera(); init_default_device_caps();
    unsigned int default_sc = create_scene_internal("default");
    if (default_sc != 0xFFFFFFFF) {
        add_material_internal(default_sc, "default_mat");
        add_light_internal(default_sc, LIGHT_DIRECTIONAL);
        add_light_internal(default_sc, LIGHT_AMBIENT);
    }
    g_initialized = 1; log_msg("[twin] initialized\n"); return 0;
}
__attribute__((export_name("twin_update"))) void twin_update(float dt) {
    if (!g_initialized) return; g_delta_time = dt; g_frame_count++;
    if (g_active_scene < g_scene_count) {
        Scene *sc = &g_scenes[g_active_scene]; if (sc->active) update_animations(sc, dt);
    }
    update_streamlines(dt); update_roaming_path(dt); update_recording(dt); update_collaboration(dt);
}
__attribute__((export_name("twin_create_scene"))) unsigned int twin_create_scene(unsigned int name_off) {
    if (!g_initialized) return 0xFFFFFFFF; return create_scene_internal((const char *)name_off);
}
__attribute__((export_name("twin_switch_scene"))) unsigned int twin_switch_scene(unsigned int scene_id) {
    if (!g_initialized || scene_id >= g_scene_count) return 0xFFFFFFFF;
    g_active_scene = scene_id; log_msg("[twin] scene switched\n"); return 0;
}
__attribute__((export_name("twin_add_object"))) unsigned int twin_add_object(unsigned int scene_id, unsigned int name_off) {
    if (!g_initialized) return 0xFFFFFFFF; return add_object_internal(scene_id, (const char *)name_off);
}
__attribute__((export_name("twin_add_material"))) unsigned int twin_add_material(unsigned int scene_id, unsigned int name_off) {
    if (!g_initialized) return 0xFFFFFFFF; return add_material_internal(scene_id, (const char *)name_off);
}
__attribute__((export_name("twin_add_light"))) unsigned int twin_add_light(unsigned int scene_id, unsigned int type) {
    if (!g_initialized) return 0xFFFFFFFF; return add_light_internal(scene_id, type);
}
__attribute__((export_name("twin_set_object_transform"))) unsigned int twin_set_object_transform(
    unsigned int scene_id, unsigned int obj_id, float px, float py, float pz,
    float rx, float ry, float rz, float sx, float sy, float sz) {
    if (!g_initialized || scene_id >= g_scene_count) return 0xFFFFFFFF;
    Scene *sc = &g_scenes[scene_id]; if (obj_id >= sc->object_count) return 0xFFFFFFFF;
    Vec3 pos, rot, scl; vec3_set(&pos, px, py, pz); vec3_set(&rot, rx, ry, rz); vec3_set(&scl, sx, sy, sz);
    update_transform(&sc->objects[obj_id].transform, pos, rot, scl); return 0;
}
__attribute__((export_name("twin_set_object_flags"))) unsigned int twin_set_object_flags(
    unsigned int scene_id, unsigned int obj_id, unsigned int flags) {
    if (!g_initialized || scene_id >= g_scene_count) return 0xFFFFFFFF;
    Scene *sc = &g_scenes[scene_id]; if (obj_id >= sc->object_count) return 0xFFFFFFFF;
    sc->objects[obj_id].flags = flags; return 0;
}
__attribute__((export_name("twin_set_camera"))) unsigned int twin_set_camera(
    float px, float py, float pz, float tx, float ty, float tz, float fov, unsigned int mode) {
    if (!g_initialized) return 0xFFFFFFFF;
    vec3_set(&g_camera.position, px, py, pz); vec3_set(&g_camera.target, tx, ty, tz);
    g_camera.fov = fov; g_camera.mode = mode; g_render_mode = mode;
    compute_view_matrix(&g_camera); compute_proj_matrix(&g_camera); return 0;
}
__attribute__((export_name("twin_set_preset_view"))) unsigned int twin_set_preset_view(unsigned int preset_id) {
    if (!g_initialized || preset_id >= g_preset_count) return 0xFFFFFFFF;
    g_camera = g_presets[preset_id].camera;
    compute_view_matrix(&g_camera); compute_proj_matrix(&g_camera); return 0;
}
__attribute__((export_name("twin_add_preset"))) unsigned int twin_add_preset(unsigned int name_off) {
    if (!g_initialized || g_preset_count >= MAX_PRESETS) return 0xFFFFFFFF;
    PresetView *pv = &g_presets[g_preset_count]; pv->id = g_preset_count;
    str_copy(pv->name, (const char *)name_off, 32);
    pv->camera = g_camera; pv->scene_id = g_active_scene; g_preset_count++; return pv->id;
}
__attribute__((export_name("twin_handle_event"))) unsigned int twin_handle_event(
    unsigned int evt_type, float ax, float ay, float az, float bx, float by, float bz) {
    if (!g_initialized) return 0xFFFFFFFF;
    if (evt_type == EVT_ZOOM) {
        Vec3 dir = vec3_sub(g_camera.target, g_camera.position);
        float factor = 1.0f + ax * 0.1f;
        g_camera.position = vec3_add(g_camera.target, vec3_scale(dir, factor));
        compute_view_matrix(&g_camera);
    } else if (evt_type == EVT_ROTATE) {
        Vec3 right = { g_camera.view_matrix.m[0], g_camera.view_matrix.m[4], g_camera.view_matrix.m[8] };
        Vec3 up = { g_camera.view_matrix.m[1], g_camera.view_matrix.m[5], g_camera.view_matrix.m[9] };
        Vec3 delta = vec3_add(vec3_scale(right, ax), vec3_scale(up, ay));
        g_camera.target = vec3_add(g_camera.target, delta); compute_view_matrix(&g_camera);
    } else if (evt_type == EVT_PAN) {
        Vec3 right = { g_camera.view_matrix.m[0], g_camera.view_matrix.m[4], g_camera.view_matrix.m[8] };
        Vec3 up = { g_camera.view_matrix.m[1], g_camera.view_matrix.m[5], g_camera.view_matrix.m[9] };
        Vec3 delta = vec3_add(vec3_scale(right, -ax), vec3_scale(up, ay));
        g_camera.position = vec3_add(g_camera.position, delta);
        g_camera.target = vec3_add(g_camera.target, delta); compute_view_matrix(&g_camera);
    } else if (evt_type == EVT_SELECT) {
        if (g_active_scene < g_scene_count) {
            Scene *sc = &g_scenes[g_active_scene];
            for (unsigned int i = 0; i < sc->object_count; i++) sc->objects[i].flags &= ~FLAG_SELECTED;
            unsigned int obj_id = (unsigned int)ax;
            if (obj_id < sc->object_count) sc->objects[obj_id].flags |= FLAG_SELECTED;
        }
    } else if (evt_type == EVT_MEASURE) {
        if (g_measurement_count < MAX_ANNOTATIONS) {
            Measurement *m = &g_measurements[g_measurement_count]; m->id = g_measurement_count;
            vec3_set(&m->start, ax, ay, az); vec3_set(&m->end, bx, by, bz);
            Vec3 diff = vec3_sub(m->end, m->start); m->distance = vec3_length(diff);
            m->visible = 1; m->type = 0; str_copy(m->label, "measure", 64); g_measurement_count++;
        }
    }
    return 0;
}
__attribute__((export_name("twin_set_section_plane"))) unsigned int twin_set_section_plane(
    unsigned int idx, float nx, float ny, float nz, float off) {
    if (!g_initialized || idx >= 8) return 0xFFFFFFFF;
    g_section_planes[idx].id = idx; vec3_set(&g_section_planes[idx].normal, nx, ny, nz);
    g_section_planes[idx].offset = off; g_section_planes[idx].visible = 1;
    if (idx >= g_section_count) g_section_count = idx + 1; return 0;
}
__attribute__((export_name("twin_set_exploded_view"))) unsigned int twin_set_exploded_view(float factor, unsigned int active) {
    if (!g_initialized) return 0xFFFFFFFF;
    g_exploded_view.factor = factor; g_exploded_view.active = active; return 0;
}
__attribute__((export_name("twin_create_heatmap"))) unsigned int twin_create_heatmap(unsigned int name_off, float min_v, float max_v) {
    if (!g_initialized || g_heatmap_count >= 8) return 0xFFFFFFFF;
    Heatmap *hm = &g_heatmaps[g_heatmap_count]; hm->id = g_heatmap_count;
    str_copy(hm->name, (const char *)name_off, 32);
    hm->point_count = 0; hm->min_value = min_v; hm->max_value = max_v;
    color_set(&hm->cold_color, 0.0f, 0.0f, 1.0f, 1.0f);
    color_set(&hm->hot_color, 1.0f, 0.0f, 0.0f, 1.0f);
    hm->visible = 1; g_heatmap_count++; return hm->id;
}
__attribute__((export_name("twin_add_heatmap_point"))) unsigned int twin_add_heatmap_point(
    unsigned int hm_id, unsigned int obj_id, float value) {
    if (!g_initialized || hm_id >= g_heatmap_count) return 0xFFFFFFFF;
    Heatmap *hm = &g_heatmaps[hm_id];
    if (hm->point_count >= MAX_OBJECTS) return 0xFFFFFFFF;
    DataPoint *dp = &hm->points[hm->point_count]; dp->object_id = obj_id; dp->value = value;
    float range = hm->max_value - hm->min_value;
    float t = (range > 0.0001f) ? (value - hm->min_value) / range : 0.5f;
    if (t < 0.0f) t = 0.0f; if (t > 1.0f) t = 1.0f;
    color_set(&dp->color,
        hm->cold_color.r + (hm->hot_color.r - hm->cold_color.r) * t,
        hm->cold_color.g + (hm->hot_color.g - hm->cold_color.g) * t,
        hm->cold_color.b + (hm->hot_color.b - hm->cold_color.b) * t, 1.0f);
    hm->point_count++; return 0;
}
__attribute__((export_name("twin_create_vector_field"))) unsigned int twin_create_vector_field(
    unsigned int res_x, unsigned int res_y, unsigned int res_z) {
    if (!g_initialized || g_vector_field_count >= MAX_VECTOR_FIELDS) return 0xFFFFFFFF;
    VectorField *vf = &g_vector_fields[g_vector_field_count]; vf->id = g_vector_field_count;
    vf->resolution_x = res_x; vf->resolution_y = res_y; vf->resolution_z = res_z;
    unsigned int total = res_x * res_y * res_z * 3;
    vf->data = (float *)safe_alloc(total * sizeof(float), 16);
    vec3_set(&vf->origin, 0.0f, 0.0f, 0.0f); vec3_set(&vf->extent, 1.0f, 1.0f, 1.0f);
    vf->visible = 1; g_vector_field_count++; return vf->id;
}
__attribute__((export_name("twin_create_isosurface"))) unsigned int twin_create_isosurface(
    unsigned int field_id, float threshold) {
    if (!g_initialized || field_id >= g_vector_field_count) return 0xFFFFFFFF;
    if (g_isosurface_count >= 16) return 0xFFFFFFFF;
    Isosurface *iso = &g_isosurfaces[g_isosurface_count]; iso->id = g_isosurface_count;
    iso->threshold = threshold; iso->source_field = &g_vector_fields[field_id];
    iso->vertex_count = 0; iso->vertices = (float *)safe_alloc(4096 * sizeof(float), 16);
    iso->visible = 1; g_isosurface_count++; return iso->id;
}
__attribute__((export_name("twin_start_roaming"))) unsigned int twin_start_roaming(unsigned int path_id) {
    if (!g_initialized || path_id >= g_roaming_path_count) return 0xFFFFFFFF;
    g_roaming_paths[path_id].active = 1; g_roaming_paths[path_id].current_time = 0.0f; return 0;
}
__attribute__((export_name("twin_stop_roaming"))) unsigned int twin_stop_roaming(unsigned int path_id) {
    if (!g_initialized || path_id >= g_roaming_path_count) return 0xFFFFFFFF;
    g_roaming_paths[path_id].active = 0; return 0;
}
__attribute__((export_name("twin_start_recording"))) unsigned int twin_start_recording(unsigned int name_off) {
    if (!g_initialized || g_recording_count >= 16) return 0xFFFFFFFF;
    Recording *rec = &g_recordings[g_recording_count]; rec->id = g_recording_count;
    str_copy(rec->name, (const char *)name_off, 32);
    rec->frame_count = 0; rec->duration = 0.0f; rec->recording = 1;
    rec->playing = 0; rec->current_time = 0.0f; g_recording_count++; return rec->id;
}
__attribute__((export_name("twin_stop_recording"))) unsigned int twin_stop_recording(unsigned int rec_id) {
    if (!g_initialized || rec_id >= g_recording_count) return 0xFFFFFFFF;
    g_recordings[rec_id].recording = 0; return 0;
}
__attribute__((export_name("twin_play_recording"))) unsigned int twin_play_recording(unsigned int rec_id) {
    if (!g_initialized || rec_id >= g_recording_count) return 0xFFFFFFFF;
    g_recordings[rec_id].playing = 1; g_recordings[rec_id].current_time = 0.0f; return 0;
}
__attribute__((export_name("twin_enable_ar"))) unsigned int twin_enable_ar(unsigned int enable) {
    if (!g_initialized) return 0xFFFFFFFF;
    g_ar_config.active = enable; g_render_mode = enable ? MODE_AR : MODE_PERSPECTIVE;
    log_msg(enable ? "[twin] AR enabled\n" : "[twin] AR disabled\n"); return 0;
}
__attribute__((export_name("twin_enable_vr"))) unsigned int twin_enable_vr(unsigned int enable) {
    if (!g_initialized) return 0xFFFFFFFF;
    g_vr_config.active = enable; g_render_mode = enable ? MODE_VR : MODE_PERSPECTIVE;
    log_msg(enable ? "[twin] VR enabled\n" : "[twin] VR disabled\n"); return 0;
}
__attribute__((export_name("twin_set_ar_anchor"))) unsigned int twin_set_ar_anchor(
    float px, float py, float pz, float rx, float ry, float rz, float scale) {
    if (!g_initialized) return 0xFFFFFFFF;
    vec3_set(&g_ar_config.anchor_position, px, py, pz);
    vec3_set(&g_ar_config.anchor_rotation, rx, ry, rz);
    g_ar_config.scale_factor = scale; return 0;
}
__attribute__((export_name("twin_set_device_caps"))) unsigned int twin_set_device_caps(
    unsigned int dev_type, unsigned int width, unsigned int height, float dpi, unsigned int max_fps) {
    if (!g_initialized) return 0xFFFFFFFF;
    g_device_caps.device_type = dev_type; g_device_caps.screen_width = width;
    g_device_caps.screen_height = height; g_device_caps.screen_dpi = dpi; g_device_caps.max_fps = max_fps;
    return 0;
}
__attribute__((export_name("twin_process_gesture"))) unsigned int twin_process_gesture(
    unsigned int g_type, float px, float py, float pz, float confidence) {
    if (!g_initialized) return 0xFFFFFFFF;
    Gesture g; g.type = g_type; vec3_set(&g.position, px, py, pz);
    vec3_set(&g.direction, 0.0f, 0.0f, 1.0f); g.confidence = confidence;
    g.hand_id = 0; g.finger_id = 0;
    if (g.confidence < 0.5f) return 1;
    if (g.type == EVT_GESTURE) twin_handle_event(EVT_ROTATE, px * 0.01f, py * 0.01f, 0.0f, 0.0f, 0.0f, 0.0f);
    return 0;
}
__attribute__((export_name("twin_set_spatial_config"))) unsigned int twin_set_spatial_config(
    float floor_h, float rmin_x, float rmin_y, float rmin_z, float rmax_x, float rmax_y, float rmax_z) {
    if (!g_initialized) return 0xFFFFFFFF;
    g_spatial_config.floor_height = floor_h;
    vec3_set(&g_spatial_config.room_min, rmin_x, rmin_y, rmin_z);
    vec3_set(&g_spatial_config.room_max, rmax_x, rmax_y, rmax_z);
    g_spatial_config.calibrated = 1; return 0;
}
__attribute__((export_name("twin_add_user"))) unsigned int twin_add_user(
    unsigned int name_off, unsigned int color, unsigned int perms) {
    if (!g_initialized || g_user_count >= MAX_USERS) return 0xFFFFFFFF;
    User *u = &g_users[g_user_count]; u->id = g_user_count;
    str_copy(u->name, (const char *)name_off, 32);
    u->color = color; u->permissions = perms; u->online = 1; u->last_active = 0.0f;
    vec3_set(&u->position, 0.0f, 0.0f, 0.0f); g_user_count++; return u->id;
}
__attribute__((export_name("twin_add_annotation"))) unsigned int twin_add_annotation(
    unsigned int user_id, unsigned int obj_id, float px, float py, float pz, unsigned int text_off) {
    if (!g_initialized || g_annotation_count >= MAX_ANNOTATIONS) return 0xFFFFFFFF;
    Annotation *a = &g_annotations[g_annotation_count]; a->id = g_annotation_count;
    a->user_id = user_id; a->scene_id = g_active_scene; a->object_id = obj_id;
    vec3_set(&a->position, px, py, pz);
    str_copy(a->text, (const char *)text_off, 128);
    color_set(&a->color, 1.0f, 1.0f, 0.0f, 1.0f); a->visible = 1; a->created_at = 0.0f;
    g_annotation_count++; return a->id;
}
__attribute__((export_name("twin_add_comment"))) unsigned int twin_add_comment(
    unsigned int user_id, unsigned int annotation_id, unsigned int msg_off) {
    if (!g_initialized || g_comment_count >= MAX_COMMENTS) return 0xFFFFFFFF;
    Comment *c = &g_comments[g_comment_count]; c->id = g_comment_count;
    c->user_id = user_id; c->annotation_id = annotation_id;
    str_copy(c->message, (const char *)msg_off, 256); c->timestamp = 0.0f; c->edited = 0;
    g_comment_count++; return c->id;
}
__attribute__((export_name("twin_set_permissions"))) unsigned int twin_set_permissions(
    unsigned int group_id, unsigned int perms) {
    if (!g_initialized || group_id >= g_perm_group_count) return 0xFFFFFFFF;
    g_perm_groups[group_id].permissions = perms; return 0;
}
__attribute__((export_name("twin_create_snapshot"))) unsigned int twin_create_snapshot(
    unsigned int name_off, unsigned int desc_off) {
    if (!g_initialized || g_snapshot_count >= MAX_SNAPSHOTS) return 0xFFFFFFFF;
    Snapshot *snap = &g_snapshots[g_snapshot_count]; snap->id = g_snapshot_count;
    str_copy(snap->name, (const char *)name_off, 64);
    str_copy(snap->description, (const char *)desc_off, 128);
    snap->scene_id = g_active_scene; snap->timestamp = 0.0f; snap->user_id = 0;
    snap->object_count = 0; snap->data_size = 0;
    if (g_active_scene < g_scene_count) snap->object_count = g_scenes[g_active_scene].object_count;
    g_snapshot_count++; return snap->id;
}
__attribute__((export_name("twin_restore_snapshot"))) unsigned int twin_restore_snapshot(unsigned int snap_id) {
    if (!g_initialized || snap_id >= g_snapshot_count) return 0xFFFFFFFF;
    g_active_scene = g_snapshots[snap_id].scene_id; log_msg("[twin] snapshot restored\n"); return 0;
}
__attribute__((export_name("twin_get_scene_count"))) unsigned int twin_get_scene_count(void) { return g_scene_count; }
__attribute__((export_name("twin_get_object_count"))) unsigned int twin_get_object_count(unsigned int scene_id) {
    if (scene_id >= g_scene_count) return 0; return g_scenes[scene_id].object_count;
}
__attribute__((export_name("twin_get_frame_count"))) unsigned int twin_get_frame_count(void) { return g_frame_count; }
__attribute__((export_name("twin_get_active_scene"))) unsigned int twin_get_active_scene(void) { return g_active_scene; }
__attribute__((export_name("twin_get_user_count"))) unsigned int twin_get_user_count(void) { return g_user_count; }
__attribute__((export_name("twin_get_annotation_count"))) unsigned int twin_get_annotation_count(void) { return g_annotation_count; }
__attribute__((export_name("twin_get_snapshot_count"))) unsigned int twin_get_snapshot_count(void) { return g_snapshot_count; }
__attribute__((export_name("twin_shutdown"))) void twin_shutdown(void) {
    if (!g_initialized) return; log_msg("[twin] shutdown\n"); g_initialized = 0;
}
__attribute__((visibility("default"))) void _start(void) { /* WASM entry - no-op */ }
