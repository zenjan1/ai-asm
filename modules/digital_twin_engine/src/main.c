/* digital_twin_engine - Digital Twin Engine WASM Module - AI-ASM OS */
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
#define DT_MAX_MODELS    48U
#define DT_MAX_ENTITIES  96U
#define DT_MAX_SCENES    16U
#define DT_MAX_NODES    256U
#define DT_MAX_STATES    64U
#define DT_MAX_RULES     32U
#define DT_MAX_LIGHTS    16U
#define DT_MAX_MATERIALS 32U
#define DT_MAX_VERTS   4096U
#define DT_MAX_TRIS    2048U
#define DT_MAX_LODS       4U
#define DT_MAX_GRID    1024U
#define DT_DT            0.016f
#define DT_FL_STATIC    0x00000001U
#define DT_FL_VISIBLE   0x00000004U
#define DT_FL_COLLIDE   0x00000008U
#define DT_FL_LOD_AUTO  0x00000020U
#define DT_FL_SHADOW    0x00000040U
#define DT_FL_SYNCED    0x00000100U
#define DT_MDL_MESH 0x01U
#define DT_MDL_SOLID 0x02U
#define DT_MDL_PARAM 0x03U
#define DT_BHV_IDLE 0x00U
#define DT_BHV_ACTIVE 0x01U
#define DT_BHV_ERROR 0x02U
#define DT_BHV_MAINT 0x03U
#define DT_MAT_LAMBERT 0x00U
#define DT_MAT_PHONG 0x01U
#define DT_MAT_PBR 0x02U
/* ─── Data Structures ───────────────────────────────────── */
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Quat;
typedef struct { Vec3 pos; Quat rot; float scale; } Transform;
typedef struct { float r, g, b, a; } Color;
typedef struct { unsigned int id, type, flags, vert_count, tri_count;
    float verts[DT_MAX_VERTS*3U]; unsigned int tris[DT_MAX_TRIS*3U];
    float bbox_min[3], bbox_max[3], surface_area, volume;
    unsigned int lod_count; float lod_thresholds[DT_MAX_LODS]; } DTMesh;
typedef struct { unsigned int id, model; Color albedo;
    float metallic, roughness, ao, emissive; } DTMaterial;
typedef struct { unsigned int id, type; Vec3 pos, dir;
    Color color; float intensity, range, spot_angle; } DTLight;
typedef struct { unsigned int id, parent, model_id, material_id;
    Transform local, world; unsigned int flags;
    float bbox_min[3], bbox_max[3]; } DTNode;
typedef struct { unsigned int id, node_id, behavior_state, rule_mask;
    float physical_props[8]; unsigned int sync_version;
    float last_sync_time, health, temperature, vibration; } DTEntity;
typedef struct { unsigned int id, entity_id, current_state, prev_state;
    unsigned int state_count, transition_count; float time_in_state; } DTStateMachine;
typedef struct { unsigned int id, entity_id, condition, action, active, fired_count;
    float threshold, value; } DTRule;
typedef struct { unsigned int id, node_count, light_count, flags;
    Transform origin; unsigned int nodes[DT_MAX_NODES], lights[DT_MAX_LIGHTS]; } DTScene;
typedef struct { unsigned int mode, width, height, draw_calls, tri_drawn, frame_id;
    float fps, frame_time; unsigned int shadows_on, post_fx; } DTRenderState;
typedef struct { float cpu_ms, gpu_ms, mem_kb, predicted_fail_hours;
    unsigned int bottleneck, opt_suggestions; } DTPerfMetrics;
typedef struct { unsigned int frame, entity_count, scene_count, model_count;
    unsigned int node_count, rule_count, sm_count, light_count, mat_count, running;
    float time, dt; } DTEngineState;
/* ─── Static State ──────────────────────────────────────── */
static DTMesh dt_meshes[DT_MAX_MODELS]; static DTMaterial dt_materials[DT_MAX_MATERIALS];
static DTLight dt_lights[DT_MAX_LIGHTS]; static DTNode dt_nodes[DT_MAX_NODES];
static DTEntity dt_entities[DT_MAX_ENTITIES]; static DTStateMachine dt_sms[DT_MAX_STATES];
static DTRule dt_rules[DT_MAX_RULES]; static DTScene dt_scenes[DT_MAX_SCENES];
static DTRenderState dt_render; static DTPerfMetrics dt_perf; static DTEngineState dt_eng;
static float dt_grid[DT_MAX_GRID];
/* ─── Forward Declarations ──────────────────────────────── */
static float dt_sinf(float x), dt_cosf(float x), dt_sqrtf(float x);
static float dt_fabsf(float x), dt_minf(float a, float b), dt_maxf(float a, float b);
static float vec3_dot(Vec3 a, Vec3 b), vec3_len(Vec3 v);
static Vec3 vec3_sub(Vec3 a, Vec3 b), vec3_norm(Vec3 v), vec3_scale(Vec3 v, float s);
static void dt_print_str(const char *s); static void dt_print_int(unsigned int v);
static void dt_print_float(float v);
static unsigned int dt_import_mesh(unsigned int t, unsigned int vc, unsigned int tc),
    dt_optimize_mesh(unsigned int m), dt_generate_lod(unsigned int m, float r),
    dt_compute_bbox(unsigned int m), dt_compute_normals(unsigned int m);
static unsigned int dt_sync_entity(unsigned int e, unsigned int v),
    dt_incremental_sync(unsigned int e, unsigned int mask),
    dt_push_state(unsigned int e), dt_pull_state(unsigned int e);
static unsigned int dt_create_state_machine(unsigned int e),
    dt_sm_transition(unsigned int s, unsigned int ns, float d),
    dt_create_rule(unsigned int e, unsigned int c, float t, unsigned int a),
    dt_evaluate_rules(void), dt_predict_failure(unsigned int e);
static unsigned int dt_create_scene(void), dt_add_node(unsigned int s, unsigned int m),
    dt_update_transforms(unsigned int s), dt_cull_scene(unsigned int s, Vec3 cam),
    dt_check_collision(unsigned int a, unsigned int b);
static unsigned int dt_create_material(unsigned int m, Color c),
    dt_add_light(unsigned int t, Vec3 p, Color c, float i),
    dt_render_scene(unsigned int s, Vec3 cam), dt_apply_postfx(unsigned int mask);
static unsigned int dt_analyze_performance(void), dt_detect_bottleneck(void),
    dt_optimize_engine(unsigned int tfps), dt_predictive_maint(unsigned int e);
static void dt_init(void), dt_shutdown(void);
/* ─── Math helpers ──────────────────────────────────────── */
static float dt_sinf(float x) {
    float pi = 3.14159265f, tw = 6.28318530f;
    while (x > pi) { x -= tw; } while (x < -pi) { x += tw; }
    float x2 = x*x, x3 = x2*x, x5 = x3*x2, x7 = x5*x2, x9 = x7*x2;
    return x - x3/6.0f + x5/120.0f - x7/5040.0f + x9/362880.0f;
}
static float dt_cosf(float x) { return dt_sinf(x + 1.57079632f); }
static float dt_sqrtf(float x) {
    if (x <= 0.0f) { return 0.0f; } float g = x*0.5f;
    for (unsigned int i = 0U; i < 8U; i++) { g = (g + x/g)*0.5f; } return g;
}
static float dt_fabsf(float x) { return (x < 0.0f) ? -x : x; }
static float dt_minf(float a, float b) { return (a < b) ? a : b; }
static float dt_maxf(float a, float b) { return (a > b) ? a : b; }
static float vec3_dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static float vec3_len(Vec3 v) { return dt_sqrtf(vec3_dot(v, v)); }
static Vec3 vec3_sub(Vec3 a, Vec3 b) { Vec3 o = { a.x-b.x, a.y-b.y, a.z-b.z }; return o; }
static Vec3 vec3_scale(Vec3 v, float s) { Vec3 o = { v.x*s, v.y*s, v.z*s }; return o; }
static Vec3 vec3_norm(Vec3 v) {
    float l = vec3_len(v); float inv = (l > 0.0001f) ? (1.0f/l) : 0.0f;
    Vec3 o = { v.x*inv, v.y*inv, v.z*inv }; return o;
}
/* ─── Print helpers ─────────────────────────────────────── */
static void dt_print_str(const char *s) { host_print(s); }
static void dt_print_int(unsigned int v) {
    char buf[16]; unsigned int i = 0U;
    if (v == 0U) { buf[i++] = '0'; }
    else { while (v > 0U && i < 15U) { buf[i++] = '0'+(char)(v%10U); v /= 10U; } }
    for (unsigned int j = 0U; j < i/2U; j++) { char t = buf[j]; buf[j] = buf[i-1U-j]; buf[i-1U-j] = t; }
    buf[i] = '\0'; host_print(buf);
}
static void dt_print_float(float v) {
    int ip = (int)v; float fr = v - (float)ip; if (fr < 0.0f) { fr = -fr; }
    dt_print_int((unsigned int)(ip < 0 ? -ip : ip)); dt_print_str(".");
    unsigned int fi = (unsigned int)(fr * 1000.0f);
    if (fi < 100U) { dt_print_str("0"); } if (fi < 10U) { dt_print_str("0"); }
    dt_print_int(fi);
}
/* ─── Model Creation ────────────────────────────────────── */
static unsigned int dt_import_mesh(unsigned int type, unsigned int vc, unsigned int tc) {
    if (vc == 0U || tc == 0U || vc > DT_MAX_VERTS || tc > DT_MAX_TRIS) { return 0xFFFFFFFFU; }
    if (dt_eng.model_count >= DT_MAX_MODELS) { return 0xFFFFFFFFU; }
    unsigned int mid = dt_eng.model_count; DTMesh *m = &dt_meshes[mid];
    m->id = 0xD0000000U | mid; m->type = type;
    m->flags = DT_FL_STATIC | DT_FL_VISIBLE; m->vert_count = vc; m->tri_count = tc;
    for (unsigned int i = 0U; i < vc; i++) {
        float a = (float)i * 6.28318530f / (float)vc;
        m->verts[i*3U] = dt_cosf(a)*0.5f; m->verts[i*3U+1U] = dt_sinf(a)*0.5f;
        m->verts[i*3U+2U] = 0.0f;
    }
    for (unsigned int i = 0U; i < tc && i*3U+2U < vc; i++) {
        m->tris[i*3U] = i; m->tris[i*3U+1U] = i+1U; m->tris[i*3U+2U] = (i+2U)%vc;
    }
    m->lod_count = 1U; m->lod_thresholds[0] = 100.0f;
    dt_compute_bbox(mid); dt_eng.model_count = mid + 1U;
    dt_print_str("[digital_twin] imported mesh "); dt_print_int(mid);
    dt_print_str("\n"); return mid;
}
static unsigned int dt_compute_bbox(unsigned int mid) {
    if (mid >= dt_eng.model_count) { return 1U; }
    DTMesh *m = &dt_meshes[mid]; if (m->vert_count == 0U) { return 2U; }
    float mnx=m->verts[0], mxx=m->verts[0], mny=m->verts[1], mxy=m->verts[1];
    float mnz=m->verts[2], mxz=m->verts[2];
    for (unsigned int i = 1U; i < m->vert_count; i++) {
        float vx=m->verts[i*3U], vy=m->verts[i*3U+1U], vz=m->verts[i*3U+2U];
        if (vx<mnx) { mnx=vx; } if (vx>mxx) { mxx=vx; }
        if (vy<mny) { mny=vy; } if (vy>mxy) { mxy=vy; }
        if (vz<mnz) { mnz=vz; } if (vz>mxz) { mxz=vz; }
    }
    m->bbox_min[0]=mnx; m->bbox_min[1]=mny; m->bbox_min[2]=mnz;
    m->bbox_max[0]=mxx; m->bbox_max[1]=mxy; m->bbox_max[2]=mxz;
    float dx=mxx-mnx, dy=mxy-mny, dz=mxz-mnz;
    m->surface_area = 2.0f*(dx*dy + dy*dz + dz*dx); m->volume = dx*dy*dz;
    return 0U;
}
static unsigned int dt_compute_normals(unsigned int mid) {
    if (mid >= dt_eng.model_count) { return 1U; }
    DTMesh *m = &dt_meshes[mid];
    for (unsigned int i = 0U; i < m->tri_count; i++) {
        unsigned int i0=m->tris[i*3U], i1=m->tris[i*3U+1U], i2=m->tris[i*3U+2U];
        if (i0>=m->vert_count || i1>=m->vert_count || i2>=m->vert_count) { continue; }
        Vec3 v0={m->verts[i0*3U],m->verts[i0*3U+1U],m->verts[i0*3U+2U]};
        Vec3 v1={m->verts[i1*3U],m->verts[i1*3U+1U],m->verts[i1*3U+2U]};
        Vec3 v2={m->verts[i2*3U],m->verts[i2*3U+1U],m->verts[i2*3U+2U]};
        Vec3 e1=vec3_sub(v1,v0), e2=vec3_sub(v2,v0);
        Vec3 n = { e1.y*e2.z-e1.z*e2.y, e1.z*e2.x-e1.x*e2.z, e1.x*e2.y-e1.y*e2.x };
        (void)vec3_norm(n);
    }
    return 0U;
}
static unsigned int dt_optimize_mesh(unsigned int mid) {
    if (mid >= dt_eng.model_count) { return 1U; }
    DTMesh *m = &dt_meshes[mid]; unsigned int reduced = 0U;
    for (unsigned int i = 0U; i+1U < m->tri_count; i++) {
        if (m->tris[i*3U]==m->tris[(i+1U)*3U] && m->tris[i*3U+1U]==m->tris[(i+1U)*3U+1U]) { reduced++; }
    }
    if (m->tri_count > reduced) { m->tri_count -= reduced; }
    dt_print_str("[digital_twin] mesh optimized, tris=");
    dt_print_int(m->tri_count); dt_print_str("\n"); return 0U;
}
static unsigned int dt_generate_lod(unsigned int mid, float ratio) {
    if (mid>=dt_eng.model_count || ratio<=0.0f || ratio>=1.0f) { return 1U; }
    DTMesh *m = &dt_meshes[mid];
    if (m->lod_count >= DT_MAX_LODS) { return 2U; }
    unsigned int lvl = m->lod_count;
    m->lod_thresholds[lvl] = m->lod_thresholds[lvl-1U] * ratio;
    m->lod_count = lvl + 1U;
    dt_print_str("[digital_twin] LOD level="); dt_print_int(lvl);
    dt_print_str("\n"); return 0U;
}
/* ─── State Synchronization ─────────────────────────────── */
static unsigned int dt_sync_entity(unsigned int eid, unsigned int version) {
    if (eid >= dt_eng.entity_count) { return 1U; }
    DTEntity *e = &dt_entities[eid];
    if (version <= e->sync_version) { return 2U; }
    e->sync_version = version; e->last_sync_time = dt_eng.time;
    e->health = dt_minf(e->health + 0.01f, 1.0f);
    dt_print_str("[digital_twin] synced v="); dt_print_int(version);
    dt_print_str("\n"); return 0U;
}
static unsigned int dt_incremental_sync(unsigned int eid, unsigned int mask) {
    if (eid >= dt_eng.entity_count || mask == 0U) { return 1U; }
    DTEntity *e = &dt_entities[eid];
    if (mask & 0x01U) { e->temperature += 0.05f; }
    if (mask & 0x02U) { e->vibration += 0.01f; }
    if (mask & 0x04U) { e->health = dt_maxf(e->health - 0.02f, 0.0f); }
    e->sync_version++; e->last_sync_time = dt_eng.time; return 0U;
}
static unsigned int dt_push_state(unsigned int eid) {
    if (eid >= dt_eng.entity_count) { return 1U; }
    dt_entities[eid].sync_version++; dt_entities[eid].last_sync_time = dt_eng.time;
    dt_print_str("[digital_twin] state pushed\n"); return 0U;
}
static unsigned int dt_pull_state(unsigned int eid) {
    if (eid >= dt_eng.entity_count) { return 1U; }
    dt_entities[eid].health = dt_minf(dt_entities[eid].health + 0.005f, 1.0f);
    dt_print_str("[digital_twin] state pulled\n"); return 0U;
}
/* ─── Behavior Modeling ─────────────────────────────────── */
static unsigned int dt_create_state_machine(unsigned int eid) {
    if (eid>=dt_eng.entity_count || dt_eng.sm_count>=DT_MAX_STATES) { return 0xFFFFFFFFU; }
    unsigned int sid = dt_eng.sm_count; DTStateMachine *sm = &dt_sms[sid];
    sm->id = 0xDB000000U|sid; sm->entity_id = eid;
    sm->current_state = DT_BHV_IDLE; sm->prev_state = DT_BHV_IDLE;
    sm->state_count = 4U; sm->transition_count = 0U; sm->time_in_state = 0.0f;
    dt_eng.sm_count = sid + 1U;
    dt_print_str("[digital_twin] state machine entity="); dt_print_int(eid);
    dt_print_str("\n"); return sid;
}
static unsigned int dt_sm_transition(unsigned int smid, unsigned int new_state, float delay) {
    if (smid>=dt_eng.sm_count || new_state>=4U) { return 1U; }
    DTStateMachine *sm = &dt_sms[smid];
    if (sm->time_in_state < delay) { return 2U; }
    sm->prev_state = sm->current_state; sm->current_state = new_state;
    sm->transition_count++; sm->time_in_state = 0.0f;
    dt_print_str("[digital_twin] transition "); dt_print_int(sm->prev_state);
    dt_print_str(" -> "); dt_print_int(sm->current_state); dt_print_str("\n");
    return 0U;
}
static unsigned int dt_create_rule(unsigned int eid, unsigned int cond, float thr, unsigned int act) {
    if (eid>=dt_eng.entity_count || dt_eng.rule_count>=DT_MAX_RULES) { return 0xFFFFFFFFU; }
    unsigned int rid = dt_eng.rule_count; DTRule *r = &dt_rules[rid];
    r->id = 0xDC000000U|rid; r->entity_id = eid;
    r->condition = cond; r->threshold = thr; r->action = act;
    r->value = 0.0f; r->active = 1U; r->fired_count = 0U;
    dt_eng.rule_count = rid + 1U; return rid;
}
static unsigned int dt_evaluate_rules(void) {
    unsigned int fired = 0U;
    for (unsigned int i = 0U; i < dt_eng.rule_count; i++) {
        DTRule *r = &dt_rules[i];
        if (!r->active || r->entity_id>=dt_eng.entity_count) { continue; }
        DTEntity *e = &dt_entities[r->entity_id]; float val = 0.0f;
        if (r->condition == 0U) { val = e->temperature; }
        else if (r->condition == 1U) { val = e->vibration; }
        else { val = 1.0f - e->health; }
        r->value = val;
        if (val > r->threshold) {
            r->fired_count++;
            if (r->action == 0U) { e->health = dt_maxf(e->health - 0.05f, 0.0f); }
            else if (e->node_id < dt_eng.sm_count) {
                dt_sm_transition(e->node_id, DT_BHV_ERROR, 0.0f);
            }
            fired++;
        }
    }
    dt_print_str("[digital_twin] rules fired="); dt_print_int(fired);
    dt_print_str("\n"); return fired;
}
static unsigned int dt_predict_failure(unsigned int eid) {
    if (eid >= dt_eng.entity_count) { return 0U; }
    DTEntity *e = &dt_entities[eid];
    float risk = (1.0f-e->health)*0.5f + dt_fabsf(e->vibration)*2.0f;
    if (e->temperature > 80.0f) { risk += 0.2f; }
    if (risk > 1.0f) { risk = 1.0f; }
    float hours = (risk > 0.001f) ? (100.0f*(1.0f-risk)/risk) : 9999.0f;
    dt_perf.predicted_fail_hours = hours;
    dt_print_str("[digital_twin] failure pred hours="); dt_print_float(hours);
    dt_print_str("\n"); return (unsigned int)(risk * 100.0f);
}
/* ─── Scene Management ──────────────────────────────────── */
static unsigned int dt_create_scene(void) {
    if (dt_eng.scene_count >= DT_MAX_SCENES) { return 0xFFFFFFFFU; }
    unsigned int sid = dt_eng.scene_count; DTScene *s = &dt_scenes[sid];
    s->id = 0xDD000000U|sid; s->node_count = 0U; s->light_count = 0U; s->flags = 0U;
    s->origin.pos = (Vec3){ 0.0f, 0.0f, 0.0f };
    s->origin.rot = (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }; s->origin.scale = 1.0f;
    dt_eng.scene_count = sid + 1U;
    dt_print_str("[digital_twin] scene "); dt_print_int(sid);
    dt_print_str("\n"); return sid;
}
static unsigned int dt_add_node(unsigned int sid, unsigned int mid) {
    if (sid>=dt_eng.scene_count || mid>=dt_eng.model_count || dt_eng.node_count>=DT_MAX_NODES) {
        return 0xFFFFFFFFU;
    }
    DTScene *s = &dt_scenes[sid];
    if (s->node_count >= DT_MAX_NODES) { return 0xFFFFFFFFU; }
    unsigned int nid = dt_eng.node_count; DTNode *n = &dt_nodes[nid];
    n->id = 0xDE000000U|nid; n->parent = 0xFFFFFFFFU;
    n->model_id = mid; n->material_id = 0U;
    n->local.pos = (Vec3){ (float)nid*0.5f, 0.0f, -(float)nid*0.3f };
    n->local.rot = (Quat){ 0.0f, 0.0f, 0.0f, 1.0f }; n->local.scale = 1.0f;
    n->world = n->local; n->flags = DT_FL_VISIBLE | DT_FL_COLLIDE;
    for (unsigned int k = 0U; k < 3U; k++) {
        n->bbox_min[k] = n->local.pos.x + dt_meshes[mid].bbox_min[k];
        n->bbox_max[k] = n->local.pos.x + dt_meshes[mid].bbox_max[k];
    }
    s->nodes[s->node_count] = nid; s->node_count++;
    dt_eng.node_count = nid + 1U; return nid;
}
static void xform_combine(Transform *o, Transform *p, Transform *c) {
    o->pos.x = p->pos.x + c->pos.x*p->scale;
    o->pos.y = p->pos.y + c->pos.y*p->scale;
    o->pos.z = p->pos.z + c->pos.z*p->scale;
    o->scale = p->scale * c->scale;
    o->rot.w = p->rot.w*c->rot.w - p->rot.x*c->rot.x - p->rot.y*c->rot.y - p->rot.z*c->rot.z;
    o->rot.x = p->rot.w*c->rot.x + p->rot.x*c->rot.w + p->rot.y*c->rot.z - p->rot.z*c->rot.y;
    o->rot.y = p->rot.w*c->rot.y - p->rot.x*c->rot.z + p->rot.y*c->rot.w + p->rot.z*c->rot.x;
    o->rot.z = p->rot.w*c->rot.z + p->rot.x*c->rot.y - p->rot.y*c->rot.x + p->rot.z*c->rot.w;
}
static unsigned int dt_update_transforms(unsigned int sid) {
    if (sid >= dt_eng.scene_count) { return 1U; }
    DTScene *s = &dt_scenes[sid];
    for (unsigned int i = 0U; i < s->node_count; i++) {
        unsigned int nid = s->nodes[i]; DTNode *n = &dt_nodes[nid];
        if (n->parent != 0xFFFFFFFFU && n->parent < dt_eng.node_count) {
            xform_combine(&n->world, &dt_nodes[n->parent].world, &n->local);
        } else { n->world = n->local; }
    }
    return 0U;
}
static unsigned int dt_aabb_overlap(float *amn, float *amx, float *bmn, float *bmx) {
    for (unsigned int k = 0U; k < 3U; k++) {
        if (amx[k] < bmn[k] || amn[k] > bmx[k]) { return 0U; }
    }
    return 1U;
}
static unsigned int dt_check_collision(unsigned int na, unsigned int nb) {
    if (na>=dt_eng.node_count || nb>=dt_eng.node_count || na==nb) { return 0U; }
    return dt_aabb_overlap(dt_nodes[na].bbox_min, dt_nodes[na].bbox_max,
                           dt_nodes[nb].bbox_min, dt_nodes[nb].bbox_max);
}
static unsigned int dt_cull_scene(unsigned int sid, Vec3 cam) {
    if (sid >= dt_eng.scene_count) { return 0U; }
    DTScene *s = &dt_scenes[sid]; unsigned int visible = 0U; float frustum = 50.0f;
    for (unsigned int i = 0U; i < s->node_count; i++) {
        unsigned int nid = s->nodes[i]; DTNode *n = &dt_nodes[nid];
        Vec3 d = vec3_sub(n->world.pos, cam);
        if (vec3_len(d) > frustum) { n->flags &= ~DT_FL_VISIBLE; }
        else { n->flags |= DT_FL_VISIBLE; visible++; }
    }
    dt_print_str("[digital_twin] visible="); dt_print_int(visible);
    dt_print_str("\n"); return visible;
}
/* ─── Visualization ─────────────────────────────────────── */
static unsigned int dt_create_material(unsigned int model, Color albedo) {
    if (dt_eng.mat_count>=DT_MAX_MATERIALS || model>DT_MAT_PBR) { return 0xFFFFFFFFU; }
    unsigned int mid = dt_eng.mat_count; DTMaterial *m = &dt_materials[mid];
    m->id = 0xDF000000U|mid; m->model = model; m->albedo = albedo;
    m->metallic = (model==DT_MAT_PBR) ? 0.5f : 0.0f;
    m->roughness = (model==DT_MAT_PHONG) ? 0.3f : 0.7f;
    m->ao = 1.0f; m->emissive = 0.0f;
    dt_eng.mat_count = mid + 1U; return mid;
}
static unsigned int dt_add_light(unsigned int type, Vec3 pos, Color col, float inten) {
    if (dt_eng.light_count >= DT_MAX_LIGHTS) { return 0xFFFFFFFFU; }
    unsigned int lid = dt_eng.light_count; DTLight *l = &dt_lights[lid];
    l->id = 0xDA000000U|lid; l->type = type; l->pos = pos;
    l->dir = vec3_norm((Vec3){ 0.0f, -1.0f, 0.0f });
    l->color = col; l->intensity = inten;
    l->range = 20.0f; l->spot_angle = 0.5f;
    dt_eng.light_count = lid + 1U; return lid;
}
static float shade_lambert(Vec3 n, Vec3 ld, Color alb, float inten) {
    float ndl = vec3_dot(n, ld); if (ndl < 0.0f) { ndl = 0.0f; }
    return (alb.r*0.2f + alb.g*0.3f + alb.b*0.1f) * ndl * inten;
}
static float shade_phong(Vec3 n, Vec3 ld, Vec3 vd, Color alb, float inten) {
    float diff = vec3_dot(n, ld); if (diff < 0.0f) { diff = 0.0f; }
    Vec3 r = vec3_sub(vec3_scale(n, 2.0f*diff), ld);
    float spec = vec3_dot(r, vd); if (spec < 0.0f) { spec = 0.0f; }
    spec = spec*spec*spec*spec; return diff*inten + spec*0.5f;
}
static unsigned int dt_render_scene(unsigned int sid, Vec3 cam) {
    if (sid >= dt_eng.scene_count) { return 1U; }
    DTScene *s = &dt_scenes[sid];
    unsigned int draws = 0U, tri_total = 0U;
    dt_cull_scene(sid, cam);
    Vec3 ld = vec3_norm((Vec3){ 0.3f, -1.0f, 0.2f });
    Vec3 normal = { 0.0f, 1.0f, 0.0f };
    for (unsigned int i = 0U; i < s->node_count; i++) {
        unsigned int nid = s->nodes[i]; DTNode *n = &dt_nodes[nid];
        if (!(n->flags & DT_FL_VISIBLE) || n->model_id>=dt_eng.model_count) { continue; }
        DTMesh *mesh = &dt_meshes[n->model_id];
        tri_total += mesh->tri_count; draws++;
        Vec3 vd = vec3_norm(vec3_sub(cam, n->world.pos));
        Color alb = { 0.8f, 0.8f, 0.8f, 1.0f };
        if (n->material_id < dt_eng.mat_count) { alb = dt_materials[n->material_id].albedo; }
        (void)shade_lambert(normal, ld, alb, 1.0f);
        (void)shade_phong(normal, ld, vd, alb, 1.0f);
    }
    dt_render.draw_calls = draws; dt_render.tri_drawn = tri_total;
    dt_render.frame_id++;
    dt_print_str("[digital_twin] frame "); dt_print_int(dt_render.frame_id);
    dt_print_str(" draws="); dt_print_int(draws);
    dt_print_str(" tris="); dt_print_int(tri_total); dt_print_str("\n");
    return 0U;
}
static unsigned int dt_apply_postfx(unsigned int mask) {
    if (mask == 0U) { return 1U; }
    if (mask & 0x01U) { dt_render.shadows_on = 1U; }
    if (mask & 0x02U) { dt_render.post_fx |= 0x02U; }
    if (mask & 0x04U) { dt_render.post_fx |= 0x04U; }
    dt_print_str("[digital_twin] postfx applied\n"); return 0U;
}
/* ─── Analysis & Optimization ───────────────────────────── */
static unsigned int dt_analyze_performance(void) {
    float cpu_e = (float)dt_render.tri_drawn*0.0001f + (float)dt_render.draw_calls*0.05f;
    float gpu_e = (float)dt_render.tri_drawn*0.0002f;
    float mem_e = (float)(dt_eng.model_count*32U + dt_eng.node_count*8U);
    dt_perf.cpu_ms = cpu_e; dt_perf.gpu_ms = gpu_e; dt_perf.mem_kb = mem_e;
    dt_render.fps = (cpu_e+gpu_e > 0.001f) ? (1000.0f/(cpu_e+gpu_e)) : 60.0f;
    if (dt_render.fps > 144.0f) { dt_render.fps = 144.0f; }
    dt_render.frame_time = cpu_e + gpu_e;
    dt_print_str("[digital_twin] cpu="); dt_print_float(cpu_e);
    dt_print_str("ms gpu="); dt_print_float(gpu_e);
    dt_print_str("ms fps="); dt_print_float(dt_render.fps);
    dt_print_str("\n"); return 0U;
}
static unsigned int dt_detect_bottleneck(void) {
    unsigned int bn = 0U;
    if (dt_perf.cpu_ms > dt_perf.gpu_ms*2.0f) { bn = 1U; }
    else if (dt_perf.gpu_ms > dt_perf.cpu_ms*2.0f) { bn = 2U; }
    else if (dt_perf.mem_kb > 65536.0f) { bn = 3U; }
    else if (dt_render.draw_calls > 100U) { bn = 4U; }
    dt_perf.bottleneck = bn;
    dt_print_str("[digital_twin] bottleneck="); dt_print_int(bn);
    dt_print_str("\n"); return bn;
}
static unsigned int dt_optimize_engine(unsigned int target_fps) {
    if (target_fps == 0U || target_fps > 240U) { return 1U; }
    dt_perf.opt_suggestions = 0U;
    if (dt_render.fps < (float)target_fps) {
        if (dt_render.tri_drawn > 10000U) { dt_perf.opt_suggestions |= 0x01U; }
        if (dt_render.draw_calls > 50U)   { dt_perf.opt_suggestions |= 0x02U; }
        if (dt_render.shadows_on)         { dt_perf.opt_suggestions |= 0x04U; }
        if (dt_render.post_fx)            { dt_perf.opt_suggestions |= 0x08U; }
        for (unsigned int i = 0U; i < dt_eng.model_count; i++) {
            if (dt_meshes[i].tri_count > 100U) { dt_meshes[i].flags |= DT_FL_LOD_AUTO; }
        }
    }
    dt_print_str("[digital_twin] suggestions=");
    dt_print_int(dt_perf.opt_suggestions); dt_print_str("\n"); return 0U;
}
static unsigned int dt_predictive_maint(unsigned int eid) {
    if (eid >= dt_eng.entity_count) { return 1U; }
    unsigned int risk = dt_predict_failure(eid);
    if (risk > 70U) {
        DTEntity *e = &dt_entities[eid];
        if (e->node_id < dt_eng.sm_count) { dt_sm_transition(e->node_id, DT_BHV_MAINT, 0.0f); }
        dt_print_str("[digital_twin] ALERT: maint entity="); dt_print_int(eid);
        dt_print_str("\n");
    } else if (risk > 40U) {
        dt_print_str("[digital_twin] WARNING: risk entity="); dt_print_int(eid);
        dt_print_str("\n");
    } else {
        dt_print_str("[digital_twin] entity="); dt_print_int(eid);
        dt_print_str(" nominal\n");
    }
    return risk;
}
/* ─── Lifecycle ─────────────────────────────────────────── */
static void dt_init(void) {
    dt_eng.frame=0U; dt_eng.entity_count=0U; dt_eng.scene_count=0U;
    dt_eng.model_count=0U; dt_eng.node_count=0U; dt_eng.rule_count=0U;
    dt_eng.sm_count=0U; dt_eng.light_count=0U; dt_eng.mat_count=0U;
    dt_eng.time=0.0f; dt_eng.dt=DT_DT; dt_eng.running=1U;
    dt_render.mode=0U; dt_render.width=1920U; dt_render.height=1080U;
    dt_render.draw_calls=0U; dt_render.tri_drawn=0U; dt_render.frame_id=0U;
    dt_render.fps=0.0f; dt_render.frame_time=0.0f;
    dt_render.shadows_on=0U; dt_render.post_fx=0U;
    dt_perf.cpu_ms=0.0f; dt_perf.gpu_ms=0.0f; dt_perf.mem_kb=0.0f;
    dt_perf.bottleneck=0U; dt_perf.opt_suggestions=0U;
    dt_perf.predicted_fail_hours=9999.0f;
    for (unsigned int i = 0U; i < DT_MAX_GRID; i++) { dt_grid[i] = 0.0f; }
    dt_print_str("[digital_twin] engine initialized\n");
}
static void dt_shutdown(void) {
    dt_eng.running = 0U;
    dt_print_str("[digital_twin] engine shutdown\n");
}
/* ─── Entry Point ───────────────────────────────────────── */
__attribute__((export_name("main")))
int main(void) {
    char buf[64]; int al = host_get_argv(0U, sizeof(buf));
    (void)al; (void)buf;
    dt_print_str("=== Digital Twin Engine ===\n");
    dt_init();
    /* Model Creation */
    unsigned int m0 = dt_import_mesh(DT_MDL_MESH, 24U, 8U);
    unsigned int m1 = dt_import_mesh(DT_MDL_SOLID, 32U, 16U);
    unsigned int m2 = dt_import_mesh(DT_MDL_PARAM, 16U, 6U);
    dt_compute_normals(m0); dt_compute_normals(m1);
    dt_optimize_mesh(m0); dt_optimize_mesh(m1);
    dt_generate_lod(m0, 0.5f); dt_generate_lod(m0, 0.25f); dt_generate_lod(m1, 0.6f);
    /* Materials & Lights */
    Color c_gray = { 0.7f, 0.7f, 0.7f, 1.0f };
    Color c_red  = { 0.9f, 0.2f, 0.2f, 1.0f };
    Color c_blue = { 0.2f, 0.4f, 0.9f, 1.0f };
    unsigned int mat0 = dt_create_material(DT_MAT_PBR, c_gray);
    unsigned int mat1 = dt_create_material(DT_MAT_PHONG, c_red);
    (void)dt_create_material(DT_MAT_LAMBERT, c_blue);
    (void)dt_add_light(1U, (Vec3){5.0f,10.0f,5.0f}, (Color){1.0f,0.95f,0.9f,1.0f}, 1.2f);
    (void)dt_add_light(0U, (Vec3){-3.0f,5.0f,-3.0f}, (Color){0.8f,0.9f,1.0f,1.0f}, 0.6f);
    /* Scene + Nodes */
    unsigned int sc = dt_create_scene();
    unsigned int n0 = dt_add_node(sc, m0);
    unsigned int n1 = dt_add_node(sc, m1);
    unsigned int n2 = dt_add_node(sc, m2);
    if (n0 < dt_eng.node_count) { dt_nodes[n0].material_id = mat0; }
    if (n1 < dt_eng.node_count) { dt_nodes[n1].material_id = mat1; }
    dt_update_transforms(sc);
    /* Entities + state sync */
    for (unsigned int i = 0U; i < 4U; i++) {
        if (dt_eng.entity_count < DT_MAX_ENTITIES) {
            unsigned int eid = dt_eng.entity_count++;
            dt_entities[eid].id = 0xEC000000U|eid;
            dt_entities[eid].node_id = (i <= n2) ? i : 0U;
            dt_entities[eid].behavior_state = DT_BHV_IDLE;
            dt_entities[eid].rule_mask = 0U; dt_entities[eid].sync_version = 0U;
            dt_entities[eid].last_sync_time = 0.0f; dt_entities[eid].health = 1.0f;
            dt_entities[eid].temperature = 25.0f + (float)i*5.0f;
            dt_entities[eid].vibration = 0.01f*(float)i;
            dt_sync_entity(eid, 1U); dt_incremental_sync(eid, 0x03U);
        }
    }
    dt_push_state(0U); dt_pull_state(1U);
    /* Behavior modeling */
    unsigned int sm0 = dt_create_state_machine(0U);
    unsigned int sm1 = dt_create_state_machine(1U);
    dt_sm_transition(sm0, DT_BHV_ACTIVE, 0.0f);
    dt_sm_transition(sm0, DT_BHV_ERROR, 1.0f); (void)sm1;
    dt_create_rule(0U, 0U, 60.0f, 0U);
    dt_create_rule(1U, 1U, 0.05f, 1U);
    dt_create_rule(2U, 2U, 0.5f, 0U);
    dt_evaluate_rules();
    /* Collision detection */
    unsigned int col = dt_check_collision(n0, n1);
    dt_print_str("[digital_twin] collision="); dt_print_int(col); dt_print_str("\n");
    /* Visualization */
    Vec3 cam = { 0.0f, 2.0f, 5.0f };
    dt_render_scene(sc, cam); dt_apply_postfx(0x07U); dt_render_scene(sc, cam);
    /* Analysis */
    dt_analyze_performance(); dt_detect_bottleneck(); dt_optimize_engine(60U);
    /* Predictive maintenance */
    for (unsigned int i = 0U; i < dt_eng.entity_count; i++) {
        dt_entities[i].temperature += 15.0f; dt_entities[i].vibration += 0.02f;
        dt_predictive_maint(i);
    }
    dt_analyze_performance(); dt_shutdown();
    dt_print_str("=== Digital Twin Engine Complete ===\n");
    return 0;
}
