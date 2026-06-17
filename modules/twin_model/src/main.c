/* twin_model: Digital Twin Modeling Engine (v1.0)
 * Twin Modeling Framework for AI-ASM OS - provides entity modeling,
 * relationship modeling, data binding, model versioning, template
 * library, and simulation parameter configuration.
 * Written in pure C with no standard library. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ENT 64
#define MAX_ATTR 16
#define MAX_REL 32
#define MAX_BIND 48
#define MAX_VER 16
#define MAX_TMPL 32
#define MAX_SIM 24
#define MAX_BRANCH 8
#define MAX_SWEEP 16
#define MAX_GEOM 12
#define MAX_RULE 8
#define MAX_HIST 64
#define MAX_BUF 256
#define MAX_CHILD 16
#define MAX_PARENT 4
#define VER "Twin Model Engine v1.0 - Digital Twin Modeling Framework"

/* Forward Declarations */
typedef struct entity_t entity_t;
typedef struct relation_t relation_t;
typedef struct binding_t binding_t;
typedef struct version_t version_t;
typedef struct tmpl_t tmpl_t;
typedef struct simparam_t simparam_t;
typedef struct geom_t geom_t;
typedef struct rule_t rule_t;
static int my_strlen(const char *s);
static void my_strncpy(char *d, const char *s, int m);
static int my_strcmp(const char *a, const char *b);
static void ps(const char *s);
static void pi(int v);
static void pd(double v);
static void nl(void);

/* Geometry */
struct geom_t {
    char name[32]; double dims[3]; double origin[3]; int shape;
};
/* Behavior Rule */
struct rule_t {
    char name[32]; char trigger[64]; char action[64];
    double threshold; int active;
};
/* Entity */
struct entity_t {
    char name[48]; char type[32]; int id; int parent_id;
    int child_ids[MAX_CHILD]; int child_count;
    double attrs[MAX_ATTR]; char attr_names[MAX_ATTR][32]; int attr_count;
    geom_t geom[MAX_GEOM]; int geom_count;
    rule_t rules[MAX_RULE]; int rule_count;
    double mass; double density; double friction; int active;
};
/* Relation */
struct relation_t {
    int src_id; int dst_id; int rel_type; double weight;
    double spatial_dist; int hierarchical; int causal;
    char label[48]; int active;
};
/* Data Binding */
struct binding_t {
    int entity_id; int attr_idx; char sensor_id[32];
    double scale; double offset; double last_val;
    double history[MAX_HIST]; int hist_pos; int hist_count;
    int stream_active; double noise_threshold; int cleanse_mode;
};
/* Version Snapshot */
struct version_t {
    int ver_id; char tag[32]; int parent_ver; int branch_id;
    int entity_count; int rel_count; double checksum;
    int timestamp; int active;
};
/* Template */
struct tmpl_t {
    char name[48]; char category[32]; int template_id;
    int entity_count; int rel_count;
    double params[8]; char param_names[8][32]; int param_count;
    int industry; int active;
};
/* Simulation Parameters */
struct simparam_t {
    char name[48]; double env_vars[8]; char env_names[8][32]; int env_count;
    double boundary[6]; double initial_state[8]; int init_count;
    unsigned int rng_seed; double sweep_vals[MAX_SWEEP];
    int sweep_count; double sweep_step; double sweep_min; double sweep_max;
    int active;
};

/* Global State */
static entity_t g_entities[MAX_ENT]; static int g_entity_count = 0;
static relation_t g_relations[MAX_REL]; static int g_rel_count = 0;
static binding_t g_bindings[MAX_BIND]; static int g_bind_count = 0;
static version_t g_versions[MAX_VER]; static int g_ver_count = 0;
static tmpl_t g_templates[MAX_TMPL]; static int g_tmpl_count = 0;
static simparam_t g_simparams[MAX_SIM]; static int g_sim_count = 0;
static int g_cur_branch = 0;

/* Helpers */
static int my_strlen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, int m) {
    int i = 0; while (i < m - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[16]; int i = 0, neg = 0; unsigned int u;
    if (v < 0) { neg = 1; u = (unsigned int)(-v); } else u = (unsigned int)v;
    if (u == 0) b[i++] = '0';
    while (u > 0) { b[i++] = '0' + (char)(u % 10); u /= 10; }
    char o[18]; int k = 0;
    if (neg) o[k++] = '-';
    while (i > 0) o[k++] = b[--i];
    o[k] = '\0'; host_print(o);
}
static void pd(double v) {
    int w = (int)v; double f = v - (double)w;
    if (f < 0.0) f = -f; pi(w); ps(".");
    int d = (int)(f * 10000.0 + 0.5);
    if (d < 1000) ps("0"); if (d < 100) ps("0"); if (d < 10) ps("0"); pi(d);
}
static void nl(void) { ps("\n"); }

/* ==== ENTITY MODELING ==== */
static int entity_create(const char *name, const char *type) {
    if (g_entity_count >= MAX_ENT) return -1;
    int id = g_entity_count++;
    entity_t *e = &g_entities[id];
    my_strncpy(e->name, name, 48); my_strncpy(e->type, type, 32);
    e->id = id; e->parent_id = -1; e->child_count = 0;
    e->attr_count = 0; e->geom_count = 0; e->rule_count = 0;
    e->mass = 0.0; e->density = 0.0; e->friction = 0.0; e->active = 1;
    return id;
}
static int entity_set_attr(int eid, const char *aname, double val) {
    if (eid < 0 || eid >= g_entity_count) return -1;
    entity_t *e = &g_entities[eid];
    for (int i = 0; i < e->attr_count; i++) {
        if (my_strcmp(e->attr_names[i], aname) == 0) { e->attrs[i] = val; return 0; }
    }
    if (e->attr_count >= MAX_ATTR) return -2;
    int idx = e->attr_count++;
    my_strncpy(e->attr_names[idx], aname, 32); e->attrs[idx] = val;
    return 0;
}
static int entity_set_geom(int eid, const char *gn, int sh, double dx, double dy, double dz) {
    if (eid < 0 || eid >= g_entity_count) return -1;
    entity_t *e = &g_entities[eid];
    if (e->geom_count >= MAX_GEOM) return -2;
    int idx = e->geom_count++;
    my_strncpy(e->geom[idx].name, gn, 32); e->geom[idx].shape = sh;
    e->geom[idx].dims[0] = dx; e->geom[idx].dims[1] = dy; e->geom[idx].dims[2] = dz;
    e->geom[idx].origin[0] = 0.0; e->geom[idx].origin[1] = 0.0; e->geom[idx].origin[2] = 0.0;
    return 0;
}
static int entity_set_physics(int eid, double mass, double density, double friction) {
    if (eid < 0 || eid >= g_entity_count) return -1;
    g_entities[eid].mass = mass; g_entities[eid].density = density;
    g_entities[eid].friction = friction; return 0;
}
static int entity_add_rule(int eid, const char *rn, const char *trig, const char *act, double thr) {
    if (eid < 0 || eid >= g_entity_count) return -1;
    entity_t *e = &g_entities[eid];
    if (e->rule_count >= MAX_RULE) return -2;
    int idx = e->rule_count++;
    my_strncpy(e->rules[idx].name, rn, 32); my_strncpy(e->rules[idx].trigger, trig, 64);
    my_strncpy(e->rules[idx].action, act, 64);
    e->rules[idx].threshold = thr; e->rules[idx].active = 1;
    return 0;
}
static void entity_print(int eid) {
    if (eid < 0 || eid >= g_entity_count) return;
    entity_t *e = &g_entities[eid];
    ps("  Entity["); pi(eid); ps("] "); ps(e->name); ps(" type="); ps(e->type);
    ps(" attrs="); pi(e->attr_count); ps(" geom="); pi(e->geom_count);
    ps(" rules="); pi(e->rule_count); ps(" mass="); pd(e->mass); nl();
}

/* ==== RELATIONSHIP MODELING ==== */
static int relation_create(int src, int dst, int rtype, double w, const char *label) {
    if (g_rel_count >= MAX_REL) return -1;
    if (src < 0 || src >= g_entity_count || dst < 0 || dst >= g_entity_count) return -2;
    int id = g_rel_count++; relation_t *r = &g_relations[id];
    r->src_id = src; r->dst_id = dst; r->rel_type = rtype;
    r->weight = w; r->spatial_dist = 0.0;
    r->hierarchical = 0; r->causal = 0;
    my_strncpy(r->label, label, 48); r->active = 1;
    return id;
}
static int relation_set_hierarchy(int rid, int is_hier) {
    if (rid < 0 || rid >= g_rel_count) return -1;
    g_relations[rid].hierarchical = is_hier;
    if (is_hier) {
        int src = g_relations[rid].src_id, dst = g_relations[rid].dst_id;
        g_entities[dst].parent_id = src;
        entity_t *p = &g_entities[src];
        if (p->child_count < MAX_CHILD) p->child_ids[p->child_count++] = dst;
    }
    return 0;
}
static int relation_set_spatial(int rid, double dist) {
    if (rid < 0 || rid >= g_rel_count) return -1;
    g_relations[rid].spatial_dist = dist; return 0;
}
static int relation_set_causal(int rid, int is_causal) {
    if (rid < 0 || rid >= g_rel_count) return -1;
    g_relations[rid].causal = is_causal; return 0;
}
static void relation_print(int rid) {
    if (rid < 0 || rid >= g_rel_count) return;
    relation_t *r = &g_relations[rid];
    ps("  Rel["); pi(rid); ps("] "); ps(r->label);
    ps(" src="); pi(r->src_id); ps(" dst="); pi(r->dst_id);
    ps(" type="); pi(r->rel_type); ps(" w="); pd(r->weight);
    ps(" hier="); pi(r->hierarchical); ps(" causal="); pi(r->causal); nl();
}

/* ==== DATA BINDING ==== */
static int binding_create(int eid, int aidx, const char *sensor, double scale, double offset) {
    if (g_bind_count >= MAX_BIND) return -1;
    int id = g_bind_count++; binding_t *b = &g_bindings[id];
    b->entity_id = eid; b->attr_idx = aidx;
    my_strncpy(b->sensor_id, sensor, 32);
    b->scale = scale; b->offset = offset; b->last_val = 0.0;
    b->hist_pos = 0; b->hist_count = 0;
    b->stream_active = 1; b->noise_threshold = 0.01; b->cleanse_mode = 1;
    return id;
}
static double binding_cleanse(binding_t *b, double raw) {
    if (!b->cleanse_mode) return raw;
    double val = raw * b->scale + b->offset;
    if (b->hist_count > 0) {
        double prev = b->history[(b->hist_pos - 1 + MAX_HIST) % MAX_HIST];
        double delta = val - prev; if (delta < 0.0) delta = -delta;
        if (delta > b->noise_threshold * 10.0) val = prev;
    }
    return val;
}
static int binding_push(int bid, double raw) {
    if (bid < 0 || bid >= g_bind_count) return -1;
    binding_t *b = &g_bindings[bid];
    double val = binding_cleanse(b, raw);
    b->last_val = val;
    b->history[b->hist_pos] = val;
    b->hist_pos = (b->hist_pos + 1) % MAX_HIST;
    if (b->hist_count < MAX_HIST) b->hist_count++;
    if (b->entity_id >= 0 && b->entity_id < g_entity_count) {
        entity_t *e = &g_entities[b->entity_id];
        if (b->attr_idx >= 0 && b->attr_idx < e->attr_count) e->attrs[b->attr_idx] = val;
    }
    return 0;
}
static double binding_history_avg(int bid) {
    if (bid < 0 || bid >= g_bind_count) return 0.0;
    binding_t *b = &g_bindings[bid];
    if (b->hist_count == 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < b->hist_count; i++) sum += b->history[i];
    return sum / (double)b->hist_count;
}
static void binding_print(int bid) {
    if (bid < 0 || bid >= g_bind_count) return;
    binding_t *b = &g_bindings[bid];
    ps("  Bind["); pi(bid); ps("] sensor="); ps(b->sensor_id);
    ps(" ent="); pi(b->entity_id); ps(" attr="); pi(b->attr_idx);
    ps(" last="); pd(b->last_val); ps(" avg="); pd(binding_history_avg(bid));
    ps(" hist="); pi(b->hist_count); nl();
}

/* ==== MODEL VERSION ==== */
static int version_snapshot(const char *tag) {
    if (g_ver_count >= MAX_VER) return -1;
    int id = g_ver_count++; version_t *v = &g_versions[id];
    v->ver_id = id; my_strncpy(v->tag, tag, 32);
    v->parent_ver = (id > 0) ? id - 1 : -1;
    v->branch_id = g_cur_branch; v->entity_count = g_entity_count;
    v->rel_count = g_rel_count; v->checksum = 0.0;
    for (int i = 0; i < g_entity_count; i++) v->checksum += g_entities[i].mass;
    v->timestamp = id * 1000; v->active = 1;
    return id;
}
static int version_diff(int va, int vb) {
    if (va < 0 || va >= g_ver_count || vb < 0 || vb >= g_ver_count) return -1;
    version_t *a = &g_versions[va], *b = &g_versions[vb];
    ps("    Diff v"); pi(va); ps("->v"); pi(vb);
    ps(": ent="); pi(b->entity_count - a->entity_count);
    ps(" rel="); pi(b->rel_count - a->rel_count);
    ps(" chk="); pd(b->checksum - a->checksum); nl();
    return 0;
}
static int version_rollback(int target) {
    if (target < 0 || target >= g_ver_count) return -1;
    version_t *v = &g_versions[target];
    ps("  Rollback to v"); pi(target); ps(" tag="); ps(v->tag); nl();
    while (g_entity_count > v->entity_count) { g_entity_count--; g_entities[g_entity_count].active = 0; }
    while (g_rel_count > v->rel_count) { g_rel_count--; g_relations[g_rel_count].active = 0; }
    return 0;
}
static int version_create_branch(const char *bname) {
    int bid = g_cur_branch + 1; g_cur_branch = bid;
    ps("  Branch id="); pi(bid); ps(" name="); ps(bname); nl();
    return bid;
}
static void version_print(int vid) {
    if (vid < 0 || vid >= g_ver_count) return;
    version_t *v = &g_versions[vid];
    ps("  Ver["); pi(vid); ps("] tag="); ps(v->tag);
    ps(" parent="); pi(v->parent_ver); ps(" branch="); pi(v->branch_id);
    ps(" ents="); pi(v->entity_count); ps(" rels="); pi(v->rel_count);
    ps(" chk="); pd(v->checksum); nl();
}

/* ==== TEMPLATE LIBRARY ==== */
static int tmpl_create(const char *name, const char *cat, int industry) {
    if (g_tmpl_count >= MAX_TMPL) return -1;
    int id = g_tmpl_count++; tmpl_t *t = &g_templates[id];
    my_strncpy(t->name, name, 48); my_strncpy(t->category, cat, 32);
    t->template_id = id; t->entity_count = 0; t->rel_count = 0;
    t->param_count = 0; t->industry = industry; t->active = 1;
    return id;
}
static int tmpl_add_param(int tid, const char *pname, double pval) {
    if (tid < 0 || tid >= g_tmpl_count) return -1;
    tmpl_t *t = &g_templates[tid];
    if (t->param_count >= 8) return -2;
    int idx = t->param_count++;
    my_strncpy(t->param_names[idx], pname, 32); t->params[idx] = pval;
    return 0;
}
static int tmpl_instantiate(int tid, const char *inst_name) {
    if (tid < 0 || tid >= g_tmpl_count) return -1;
    tmpl_t *t = &g_templates[tid];
    int eid = entity_create(inst_name, t->category);
    if (eid < 0) return -2;
    for (int i = 0; i < t->param_count; i++) entity_set_attr(eid, t->param_names[i], t->params[i]);
    ps("  Instantiate tmpl='"); ps(t->name); ps("' -> ent["); pi(eid); ps("] '"); ps(inst_name); ps("'"); nl();
    return eid;
}
static void tmpl_print(int tid) {
    if (tid < 0 || tid >= g_tmpl_count) return;
    tmpl_t *t = &g_templates[tid];
    ps("  Tmpl["); pi(tid); ps("] "); ps(t->name); ps(" cat="); ps(t->category);
    ps(" params="); pi(t->param_count); ps(" industry="); pi(t->industry); nl();
}

/* ==== SIMULATION PARAMETERS ==== */
static int simparam_create(const char *name, unsigned int seed) {
    if (g_sim_count >= MAX_SIM) return -1;
    int id = g_sim_count++; simparam_t *s = &g_simparams[id];
    my_strncpy(s->name, name, 48);
    s->env_count = 0; s->init_count = 0; s->rng_seed = seed;
    s->sweep_count = 0; s->sweep_step = 0.0;
    s->sweep_min = 0.0; s->sweep_max = 0.0;
    for (int i = 0; i < 6; i++) s->boundary[i] = 0.0;
    for (int i = 0; i < 8; i++) s->initial_state[i] = 0.0;
    s->active = 1; return id;
}
static int simparam_set_env(int sid, const char *en, double val) {
    if (sid < 0 || sid >= g_sim_count) return -1;
    simparam_t *s = &g_simparams[sid];
    if (s->env_count >= 8) return -2;
    int idx = s->env_count++;
    my_strncpy(s->env_names[idx], en, 32); s->env_vars[idx] = val;
    return 0;
}
static int simparam_set_boundary(int sid, double x0, double x1, double y0, double y1, double z0, double z1) {
    if (sid < 0 || sid >= g_sim_count) return -1;
    simparam_t *s = &g_simparams[sid];
    s->boundary[0] = x0; s->boundary[1] = x1;
    s->boundary[2] = y0; s->boundary[3] = y1;
    s->boundary[4] = z0; s->boundary[5] = z1;
    return 0;
}
static int simparam_set_initial(int sid, double *vals, int count) {
    if (sid < 0 || sid >= g_sim_count) return -1;
    simparam_t *s = &g_simparams[sid];
    int n = count > 8 ? 8 : count;
    for (int i = 0; i < n; i++) s->initial_state[i] = vals[i];
    s->init_count = n; return 0;
}
static int simparam_setup_sweep(int sid, double smin, double smax, double step) {
    if (sid < 0 || sid >= g_sim_count) return -1;
    simparam_t *s = &g_simparams[sid];
    s->sweep_min = smin; s->sweep_max = smax; s->sweep_step = step;
    s->sweep_count = 0; double v = smin;
    while (v <= smax && s->sweep_count < MAX_SWEEP) { s->sweep_vals[s->sweep_count++] = v; v += step; }
    return s->sweep_count;
}
static unsigned int simparam_rng_next(int sid) {
    if (sid < 0 || sid >= g_sim_count) return 0;
    simparam_t *s = &g_simparams[sid];
    s->rng_seed = s->rng_seed * 1103515245U + 12345U;
    return (s->rng_seed >> 16) & 0x7FFF;
}
static void simparam_print(int sid) {
    if (sid < 0 || sid >= g_sim_count) return;
    simparam_t *s = &g_simparams[sid];
    ps("  Sim["); pi(sid); ps("] "); ps(s->name);
    ps(" env="); pi(s->env_count); ps(" init="); pi(s->init_count);
    ps(" seed="); pi((int)s->rng_seed); ps(" sweep="); pi(s->sweep_count); nl();
}

/* ==== ENTRY ==== */
static void run_demo(void) {
    ps(VER); nl();
    ps("--- Entity Modeling ---"); nl();
    int motor = entity_create("Motor_A", "rotary_device");
    entity_set_attr(motor, "rpm", 3000.0);
    entity_set_attr(motor, "torque", 12.5);
    entity_set_attr(motor, "voltage", 48.0);
    entity_set_geom(motor, "cylinder_body", 1, 0.2, 0.2, 0.5);
    entity_set_physics(motor, 15.0, 7800.0, 0.3);
    entity_add_rule(motor, "overheat", "temp > 80", "shutdown", 80.0);
    entity_print(motor);
    int pump = entity_create("Pump_B", "centrifugal");
    entity_set_attr(pump, "flow_rate", 120.0);
    entity_set_attr(pump, "pressure", 5.5);
    entity_set_geom(pump, "housing", 2, 0.3, 0.3, 0.3);
    entity_set_physics(pump, 25.0, 3000.0, 0.2);
    entity_print(pump);
    int tank = entity_create("Tank_C", "vessel");
    entity_set_attr(tank, "capacity", 500.0);
    entity_set_attr(tank, "level", 75.0);
    entity_print(tank);
    int ctrl = entity_create("Controller_D", "PLC");
    entity_set_attr(ctrl, "scan_ms", 10.0);
    entity_print(ctrl);
    ps("--- Relationship Modeling ---"); nl();
    int r1 = relation_create(motor, pump, 1, 0.95, "drives");
    relation_set_spatial(r1, 0.5); relation_set_causal(r1, 1);
    relation_print(r1);
    int r2 = relation_create(pump, tank, 2, 0.88, "feeds");
    relation_set_hierarchy(r2, 1); relation_print(r2);
    int r3 = relation_create(ctrl, motor, 3, 1.0, "controls");
    relation_set_causal(r3, 1); relation_print(r3);
    ps("--- Data Binding ---"); nl();
    int b0 = binding_create(motor, 0, "SENS_RPM_01", 1.0, 0.0);
    int b1 = binding_create(motor, 1, "SENS_TRQ_02", 1.0, 0.0);
    int b2 = binding_create(pump, 0, "SENS_FLOW_03", 0.01, 0.0);
    int b3 = binding_create(tank, 1, "SENS_LVL_04", 1.0, 0.0);
    for (int i = 0; i < 20; i++) {
        binding_push(b0, 3000.0 + (double)(i * 5));
        binding_push(b1, 12.5 + (double)(i % 3) * 0.2);
        binding_push(b2, 120.0 + (double)(i % 5) * 2.0);
        binding_push(b3, 75.0 - (double)i * 0.3);
    }
    binding_print(b0); binding_print(b1);
    binding_print(b2); binding_print(b3);
    ps("--- Model Versioning ---"); nl();
    int v0 = version_snapshot("initial"); version_print(v0);
    entity_create("Sensor_E", "sensor");
    entity_set_attr(g_entity_count - 1, "accuracy", 0.99);
    int v1 = version_snapshot("added_sensor"); version_print(v1);
    version_diff(v0, v1);
    int branch = version_create_branch("experiment_1"); (void)branch;
    version_rollback(v0);
    ps("  Entities after rollback: "); pi(g_entity_count); nl();
    ps("--- Template Library ---"); nl();
    int t0 = tmpl_create("MotorTemplate", "rotary_device", 1);
    tmpl_add_param(t0, "rated_rpm", 1500.0);
    tmpl_add_param(t0, "rated_power", 5.0);
    tmpl_add_param(t0, "efficiency", 0.92);
    tmpl_print(t0);
    int t1 = tmpl_create("TankTemplate", "vessel", 2);
    tmpl_add_param(t1, "volume_m3", 10.0);
    tmpl_add_param(t1, "max_pressure", 6.0);
    tmpl_print(t1);
    tmpl_instantiate(t0, "Motor_E1");
    tmpl_instantiate(t0, "Motor_E2");
    tmpl_instantiate(t1, "Tank_F1");
    ps("--- Simulation Parameters ---"); nl();
    int s0 = simparam_create("thermal_sim", 0xDEADBEEF);
    simparam_set_env(s0, "ambient_temp", 25.0);
    simparam_set_env(s0, "humidity", 0.6);
    simparam_set_env(s0, "gravity", 9.81);
    simparam_set_boundary(s0, -10.0, 10.0, -10.0, 10.0, 0.0, 20.0);
    double init_vals[4] = { 100.0, 25.0, 0.0, 1.0 };
    simparam_set_initial(s0, init_vals, 4);
    int sw = simparam_setup_sweep(s0, 0.0, 1.0, 0.25);
    ps("  Sweep points: "); pi(sw); nl();
    ps("  RNG samples: ");
    for (int i = 0; i < 5; i++) { pi((int)simparam_rng_next(s0)); ps(" "); }
    nl(); simparam_print(s0);
    ps("--- Summary ---"); nl();
    ps("  Total entities: "); pi(g_entity_count); nl();
    ps("  Total relations: "); pi(g_rel_count); nl();
    ps("  Total bindings: "); pi(g_bind_count); nl();
    ps("  Total versions: "); pi(g_ver_count); nl();
    ps("  Total templates: "); pi(g_tmpl_count); nl();
    ps("  Total simparams: "); pi(g_sim_count); nl();
    ps("Twin model engine complete."); nl();
}

__attribute__((export_name("main")))
int twin_model_main(void) {
    run_demo();
    return 0;
}
