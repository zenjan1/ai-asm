/* digital_twin: Physical World Mirror Engine (v55.0)
 * Digital Twin Engine for AI-ASM OS - mirrors physical entities
 * with real-time sensor sync, predictive maintenance, scenario
 * simulation, multi-scale modeling, and dashboard visualization.
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

#define MAX_ENT 32
#define MAX_PROP 16
#define MAX_STREAMS 64
#define MAX_BUF 256
#define MAX_FM 8
#define MAX_ME 32
#define MAX_SC 16
#define MAX_MD 32
#define MAX_LNK 16
#define MAX_SUB 8
#define MAX_WG 24
#define MAX_TH 4
#define MAX_SMPL 1000
#define MAX_NM 32
#define VER "Digital Twin v55.0 - Physical World Mirror Engine"

/* ---- Helpers (no stdlib) ---- */
static int my_strlen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, int m) {
    int i = 0; while (i < m - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
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
static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0; double g = x * 0.5;
    for (int i = 0; i < 40; i++) g = (g + x / g) * 0.5; return g;
}
static double abs_d(double x) { return x < 0.0 ? -x : x; }
static double clamp(double v, double lo, double hi) {
    if (v < lo) return lo; if (v > hi) return hi; return v;
}
static unsigned int rng_state = 12345;
static double randf(void) {
    rng_state = rng_state * 1103515245u + 12345u;
    return (double)(rng_state & 0x7FFFFFFFu) / (double)0x7FFFFFFFu;
}
static void *twin_alloc(unsigned int sz) {
    unsigned int off = host_alloc(sz, 8); return (void *)off;
}

/* ============================================================
 * 1. Entity Modeling
 * ============================================================ */
#define E_MECH 0
#define E_ELEC 1
#define E_THER 2
#define E_CHEM 3
#define ST_NORM 0
#define ST_DEGR 1
#define ST_FAULT 2
#define ST_MAINT 3
#define ST_OFF 4

typedef struct { char key[24]; double val; } twin_prop_t;
typedef struct {
    int id; char name[MAX_NM]; int type;
    double pos[3], rot[4], scale, vel[3];
    twin_prop_t props[MAX_PROP]; int prop_cnt; int state;
} twin_entity_t;
static twin_entity_t g_entities[MAX_ENT];
static int g_entity_count = 0;

static const char *state_name(int s) {
    const char *n[] = {"NORMAL", "DEGRADED", "FAULT", "MAINTENANCE", "OFFLINE"};
    return (s >= 0 && s <= 4) ? n[s] : "UNKNOWN";
}
static twin_entity_t *entity_create(const char *name, int type) {
    if (g_entity_count >= MAX_ENT) return NULL;
    twin_entity_t *e = &g_entities[g_entity_count++];
    e->id = g_entity_count; my_strncpy(e->name, name, MAX_NM);
    e->type = type; e->state = ST_NORM; e->scale = 1.0; e->prop_cnt = 0;
    for (int i = 0; i < 3; i++) { e->pos[i] = 0; e->vel[i] = 0; }
    e->rot[0] = 0; e->rot[1] = 0; e->rot[2] = 0; e->rot[3] = 1;
    ps("[ENTITY] Created: "); ps(name); ps("\n"); return e;
}
static int entity_set_property(twin_entity_t *e, const char *key, double val) {
    if (!e) return -1;
    for (int i = 0; i < e->prop_cnt; i++)
        if (e->props[i].key[0] == key[0] && e->props[i].key[1] == key[1]) {
            e->props[i].val = val; return 0;
        }
    if (e->prop_cnt >= MAX_PROP) return -1;
    my_strncpy(e->props[e->prop_cnt].key, key, 24);
    e->props[e->prop_cnt].val = val; e->prop_cnt++; return 0;
}
static int entity_transition(twin_entity_t *e, int new_state) {
    if (!e) return -1; int old = e->state; e->state = new_state;
    ps("[STATE] "); ps(e->name); ps(": ");
    ps(state_name(old)); ps(" -> "); ps(state_name(new_state)); ps("\n"); return 0;
}

/* ============================================================
 * 2. Real-time Data Synchronization
 * ============================================================ */
#define D_TEMP 0
#define D_PRES 1
#define D_VOLT 2
#define D_RPM 3
#define D_VIB 4

typedef struct {
    int stream_id, entity_id, data_type;
    double sampling_rate, buffer[MAX_BUF];
    int buf_len, buf_pos; double timestamp;
    double kalman_p, kalman_x, kalman_k;
} sensor_stream_t;
static sensor_stream_t g_streams[MAX_STREAMS];
static int g_stream_count = 0;

static void sync_init(void) { g_stream_count = 0; ps("[SYNC] Engine initialized\n"); }
static sensor_stream_t *sync_create_stream(int eid, int dt, double rate) {
    if (g_stream_count >= MAX_STREAMS) return NULL;
    sensor_stream_t *s = &g_streams[g_stream_count++];
    s->stream_id = g_stream_count; s->entity_id = eid; s->data_type = dt;
    s->sampling_rate = rate; s->buf_len = 0; s->buf_pos = 0; s->timestamp = 0;
    s->kalman_p = 1.0; s->kalman_x = 0; s->kalman_k = 0; return s;
}
static void sync_ingest(sensor_stream_t *s, double val, double ts) {
    if (!s) return;
    s->buffer[s->buf_pos] = val;
    s->buf_pos = (s->buf_pos + 1) % MAX_BUF;
    if (s->buf_len < MAX_BUF) s->buf_len++; s->timestamp = ts;
}
static double sync_moving_average(sensor_stream_t *s, int window) {
    if (!s || s->buf_len == 0) return 0;
    int w = window < s->buf_len ? window : s->buf_len; double sum = 0;
    for (int i = 0; i < w; i++)
        sum += s->buffer[(s->buf_pos - 1 - i + MAX_BUF) % MAX_BUF];
    return sum / (double)w;
}
static void sync_kalman_filter(sensor_stream_t *s, double meas) {
    if (!s) return; double q = 0.01, r = 0.1;
    s->kalman_p += q; s->kalman_k = s->kalman_p / (s->kalman_p + r);
    s->kalman_x += s->kalman_k * (meas - s->kalman_x);
    s->kalman_p = (1.0 - s->kalman_k) * s->kalman_p;
}
static int sync_anomaly_detect(sensor_stream_t *s, double threshold) {
    if (!s || s->buf_len < 4) return 0;
    double mean = sync_moving_average(s, s->buf_len), var = 0;
    for (int i = 0; i < s->buf_len; i++) { double d = s->buffer[i] - mean; var += d * d; }
    double sd = my_sqrt(var / (double)s->buf_len);
    if (sd < 0.0001) return 0;
    double lat = s->buffer[(s->buf_pos - 1 + MAX_BUF) % MAX_BUF];
    return abs_d((lat - mean) / sd) > threshold ? 1 : 0;
}
static void sync_timestamp_align(void) {
    double latest = 0;
    for (int i = 0; i < g_stream_count; i++)
        if (g_streams[i].timestamp > latest) latest = g_streams[i].timestamp;
    ps("[SYNC] Aligned to "); pd(latest); ps("s across "); pi(g_stream_count); ps(" streams\n");
}

/* ============================================================
 * 3. Predictive Maintenance
 * ============================================================ */
#define FM_BEARING 0
#define FM_BATTERY 1
#define FM_MOTOR 2
#define FM_SENSOR 3
#define FM_GEARBOX 4

typedef struct { int mid, etype; char name[MAX_NM]; double mtbf; int sev; } failure_mode_t;
typedef struct { int eid, fm; double rul, conf, sched; } maintenance_event_t;
static failure_mode_t g_fail_modes[MAX_FM];
static int g_fm_count = 0;
static maintenance_event_t g_maint_ev[MAX_ME];
static int g_me_count = 0;

static void maintenance_init(void) {
    g_fm_count = 0; g_me_count = 0;
    struct { int et; const char *n; double m; int s; } fd[] = {
        {E_MECH,"Bearing Wear",8000,3}, {E_ELEC,"Battery Decay",5000,4},
        {E_MECH,"Motor Overheat",6000,5}, {E_ELEC,"Sensor Drift",3000,2},
        {E_MECH,"Gearbox Backlash",10000,3}
    };
    for (int i = 0; i < 5; i++) {
        g_fail_modes[g_fm_count].mid = g_fm_count;
        g_fail_modes[g_fm_count].etype = fd[i].et;
        my_strncpy(g_fail_modes[g_fm_count].name, fd[i].n, MAX_NM);
        g_fail_modes[g_fm_count].mtbf = fd[i].m;
        g_fail_modes[g_fm_count].sev = fd[i].s; g_fm_count++;
    }
    ps("[MAINT] Library loaded: "); pi(g_fm_count); ps(" modes\n");
}
static double maintenance_degradation_curve(double health, double elapsed, double mtbf) {
    double r = elapsed / mtbf;
    return health / (1.0 + r + r * r * 0.5);
}
static double maintenance_predict(twin_entity_t *e, double sval, double thr) {
    if (!e) return 0;
    double health = 1.0 - clamp(sval / thr, 0, 1), mtbf = 5000;
    for (int i = 0; i < g_fm_count; i++)
        if (g_fail_modes[i].etype == e->type) { mtbf = g_fail_modes[i].mtbf; break; }
    double dg = maintenance_degradation_curve(health, 100, mtbf);
    double rul = (dg > 0.05) ? mtbf * dg / (1.0 - health + 0.01) : 0;
    rul = clamp(rul, 0, mtbf);
    if (g_me_count < MAX_ME) {
        maintenance_event_t *me = &g_maint_ev[g_me_count++];
        me->eid = e->id; me->fm = e->type; me->rul = rul; me->conf = 0.85; me->sched = 0;
    }
    ps("[MAINT] "); ps(e->name); ps(" RUL="); pd(rul); ps("h h="); pd(health); ps("\n");
    return rul;
}
static void maintenance_schedule(double budget, double horizon) {
    ps("[MAINT] Schedule budget="); pd(budget); ps(" horizon="); pd(horizon); ps("h\n");
    for (int i = 0; i < g_me_count; i++)
        for (int j = i + 1; j < g_me_count; j++)
            if (g_maint_ev[j].rul < g_maint_ev[i].rul) {
                maintenance_event_t t = g_maint_ev[i];
                g_maint_ev[i] = g_maint_ev[j]; g_maint_ev[j] = t;
            }
    double spent = 0;
    for (int i = 0; i < g_me_count; i++) {
        double cost = 100 + (double)g_maint_ev[i].fm * 50;
        if (spent + cost <= budget && g_maint_ev[i].rul <= horizon) {
            g_maint_ev[i].sched = g_maint_ev[i].rul * 0.8; spent += cost;
            ps("[MAINT]   Ev#"); pi(i); ps(" at "); pd(g_maint_ev[i].sched);
            ps("h cost="); pd(cost); ps("\n");
        }
    }
}

/* ============================================================
 * 4. Scenario Simulation (Monte Carlo)
 * ============================================================ */
typedef struct { char nm[24]; double val, lo, hi; } scenario_param_t;
typedef struct {
    char name[MAX_NM], desc[64];
    scenario_param_t params[16]; int pc;
    double dur, res[MAX_SMPL]; int rc;
    double smean, sstd, smin, smax;
} scenario_t;
static scenario_t g_scenarios[MAX_SC];
static int g_scenario_count = 0;

static scenario_t *scenario_create(const char *name, const char *desc) {
    if (g_scenario_count >= MAX_SC) return NULL;
    scenario_t *sc = &g_scenarios[g_scenario_count++];
    my_strncpy(sc->name, name, MAX_NM); my_strncpy(sc->desc, desc, 64);
    sc->pc = 0; sc->rc = 0; sc->dur = 100;
    ps("[SCENARIO] Created: "); ps(name); ps("\n"); return sc;
}
static void scenario_add_parameter(scenario_t *sc, const char *pn, double v, double lo, double hi) {
    if (!sc || sc->pc >= 16) return;
    my_strncpy(sc->params[sc->pc].nm, pn, 24);
    sc->params[sc->pc].val = v; sc->params[sc->pc].lo = lo;
    sc->params[sc->pc].hi = hi; sc->pc++;
}
static double scenario_simulate_step(scenario_t *sc, double dt) {
    double b = 25;
    for (int i = 0; i < sc->pc; i++) {
        double n = (randf() - 0.5) * (sc->params[i].hi - sc->params[i].lo) * 0.1;
        b += sc->params[i].val * dt * 0.01 + n;
    }
    return b;
}
static void scenario_statistical_aggregate(scenario_t *sc) {
    if (!sc || !sc->rc) return;
    double sum = 0, mn = sc->res[0], mx = sc->res[0];
    for (int i = 0; i < sc->rc; i++) {
        sum += sc->res[i];
        if (sc->res[i] < mn) mn = sc->res[i];
        if (sc->res[i] > mx) mx = sc->res[i];
    }
    sc->smean = sum / (double)sc->rc; double var = 0;
    for (int i = 0; i < sc->rc; i++) { double d = sc->res[i] - sc->smean; var += d * d; }
    sc->sstd = my_sqrt(var / (double)sc->rc); sc->smin = mn; sc->smax = mx;
    ps("[SCENARIO] mean="); pd(sc->smean); ps(" std="); pd(sc->sstd);
    ps(" min="); pd(mn); ps(" max="); pd(mx); ps("\n");
}
static void scenario_run(scenario_t *sc) {
    if (!sc) return; sc->rc = 0;
    ps("[SCENARIO] Running: "); ps(sc->name); ps(" (Monte Carlo 1000)\n");
    for (int i = 0; i < MAX_SMPL; i++)
        sc->res[sc->rc++] = scenario_simulate_step(sc, sc->dur * randf());
    scenario_statistical_aggregate(sc);
}
static void scenario_parallel(int count) {
    ps("[SCENARIO] Parallel: "); pi(count); ps(" scenarios\n");
    for (int i = 0; i < count && i < g_scenario_count; i++) scenario_run(&g_scenarios[i]);
}

/* ============================================================
 * 5. Multi-scale Modeling
 * ============================================================ */
#define LEVEL_MACRO 0
#define LEVEL_MESO 1
#define LEVEL_MICRO 2

typedef struct {
    int eid, level; double params[8]; int pc;
    int subs[MAX_SUB]; int sc, par;
} twin_model_t;
static twin_model_t g_models[MAX_MD];
static int g_model_count = 0;
static int g_links[MAX_LNK][2];
static int g_link_count = 0;

static twin_model_t *model_create(int eid, int level) {
    if (g_model_count >= MAX_MD) return NULL;
    twin_model_t *m = &g_models[g_model_count++];
    m->eid = eid; m->level = level; m->pc = 0; m->sc = 0; m->par = -1;
    const char *ln[] = {"MACRO", "MESO", "MICRO"};
    ps("[MODEL] Created "); ps(ln[level]); ps(" for entity "); pi(eid); ps("\n"); return m;
}
static void model_set_param(twin_model_t *m, double v) {
    if (!m || m->pc >= 8) return; m->params[m->pc++] = v;
}
static void model_link(twin_model_t *parent, twin_model_t *child) {
    if (!parent || !child) return;
    int ci = (int)(child - g_models), pi2 = (int)(parent - g_models);
    if (parent->sc < MAX_SUB) parent->subs[parent->sc++] = ci;
    child->par = pi2;
    if (g_link_count < MAX_LNK) {
        g_links[g_link_count][0] = pi2; g_links[g_link_count][1] = ci; g_link_count++;
    }
    ps("[MODEL] Link #"); pi(pi2); ps(" -> #"); pi(ci); ps("\n");
}
static double model_propagate(twin_model_t *m, double input) {
    if (!m) return 0; double out = input;
    for (int i = 0; i < m->pc; i++) out *= m->params[i];
    if (m->level == LEVEL_MESO) out += input * 0.1;
    if (m->level == LEVEL_MICRO) out += input * 0.01;
    ps("[MODEL] Propagate L"); pi(m->level); ps(": "); pd(input); ps("->"); pd(out); ps("\n");
    for (int i = 0; i < m->sc; i++) model_propagate(&g_models[m->subs[i]], out);
    return out;
}
static double model_aggregate(twin_model_t *m) {
    if (!m || !m->sc) return 0; double sum = 0;
    for (int i = 0; i < m->sc; i++) {
        twin_model_t *c = &g_models[m->subs[i]]; double v = 0;
        for (int j = 0; j < c->pc; j++) v += c->params[j]; sum += v;
    }
    double avg = sum / (double)m->sc;
    ps("[MODEL] Aggregate L"); pi(m->level); ps(": ch="); pi(m->sc); ps(" val="); pd(avg); ps("\n");
    return avg;
}

/* ============================================================
 * 6. Dashboard & Visualization
 * ============================================================ */
#define W_GAUGE 0
#define W_CHART 1
#define W_ALERT 2
#define W_KPI 3

typedef struct {
    int type, eid; char metric[24];
    double mn, mx; double th[MAX_TH]; int tc;
} widget_t;
typedef struct { char name[MAX_NM]; int wc; widget_t wg[MAX_WG]; int interval; } dashboard_t;

static dashboard_t *dashboard_create(const char *name) {
    dashboard_t *d = (dashboard_t *)twin_alloc(sizeof(dashboard_t));
    if (!d) return NULL;
    my_strncpy(d->name, name, MAX_NM); d->wc = 0; d->interval = 1000;
    ps("[DASH] Created: "); ps(name); ps("\n"); return d;
}
static int dashboard_add_widget(dashboard_t *d, int tp, int eid, const char *met, double mn, double mx) {
    if (!d || d->wc >= MAX_WG) return -1;
    widget_t *w = &d->wg[d->wc++];
    w->type = tp; w->eid = eid; my_strncpy(w->metric, met, 24);
    w->mn = mn; w->mx = mx; w->tc = 0; return d->wc - 1;
}
static void widget_set_thresholds(dashboard_t *d, int idx, double warn, double crit) {
    if (!d || idx < 0 || idx >= d->wc) return;
    d->wg[idx].th[0] = warn; d->wg[idx].th[1] = crit; d->wg[idx].tc = 2;
}
static double dashboard_compute_kpi(widget_t *w, double up, double tot, double qual) {
    if (!w) return 0;
    double av = (tot > 0) ? up / tot : 0, perf = clamp(qual, 0, 1), oee = av * perf * qual;
    ps("[KPI] "); ps(w->metric); ps(" OEE="); pd(oee * 100); ps("% av="); pd(av * 100); ps("%\n");
    return oee;
}
static int dashboard_check_alerts(dashboard_t *d, int idx, double v) {
    if (!d || idx < 0 || idx >= d->wc) return 0;
    widget_t *w = &d->wg[idx];
    if (w->tc >= 2 && v >= w->th[1]) {
        ps("[ALERT] CRITICAL: "); ps(w->metric); ps("="); pd(v); ps("\n"); return 2;
    }
    if (w->tc >= 1 && v >= w->th[0]) {
        ps("[ALERT] WARNING: "); ps(w->metric); ps("="); pd(v); ps("\n"); return 1;
    }
    return 0;
}
static void dashboard_render(dashboard_t *d) {
    if (!d) return;
    ps("========================================\n");
    ps("  Dashboard: "); ps(d->name); ps("\n");
    ps("  Widgets: "); pi(d->wc); ps("  Interval: "); pi(d->interval); ps("ms\n");
    ps("----------------------------------------\n");
    for (int i = 0; i < d->wc; i++) {
        widget_t *w = &d->wg[i];
        const char *t = (w->type==W_GAUGE)?"GAUGE":(w->type==W_CHART)?"CHART":(w->type==W_ALERT)?"ALERT":"KPI";
        ps("  ["); ps(t); ps("] "); ps(w->metric); ps(" ent="); pi(w->eid);
        ps(" ["); pd(w->mn); ps(","); pd(w->mx); ps("]\n");
    }
    ps("========================================\n");
}

/* ============================================================
 * 7. Entry Point
 * ============================================================ */
static void show_help(void) {
    ps(VER); ps("\n\nUsage: digital_twin [options]\n");
    ps("  -h    Show this help\n  -t    Run test/demo mode\n");
}
static void run_tests(void) {
    ps("=== Digital Twin Test Suite ===\n\n");
    /* Entity Modeling */
    ps("--- Entity Modeling ---\n");
    twin_entity_t *motor = entity_create("Motor-01", E_MECH);
    twin_entity_t *bearing = entity_create("Bearing-01", E_MECH);
    twin_entity_t *battery = entity_create("Battery-01", E_ELEC);
    twin_entity_t *sensor = entity_create("Sensor-01", E_ELEC);
    entity_set_property(motor, "temperature", 72.5);
    entity_set_property(motor, "rpm", 3450);
    entity_set_property(bearing, "vibration", 4.2);
    entity_set_property(bearing, "temperature", 58);
    entity_set_property(battery, "voltage", 12.3);
    entity_set_property(battery, "charge", 85);
    entity_set_property(sensor, "sampling_hz", 100);
    ps("\n");
    /* Sensor Synchronization */
    ps("--- Sensor Synchronization ---\n");
    sync_init();
    sensor_stream_t *ts = sync_create_stream(bearing->id, D_TEMP, 100);
    sensor_stream_t *vs = sync_create_stream(bearing->id, D_VIB, 200);
    for (int i = 0; i < 50; i++) {
        double t = 55 + randf() * 6, v = 3.5 + randf() * 1.5;
        sync_ingest(ts, t, (double)i * 0.01);
        sync_ingest(vs, v, (double)i * 0.005);
        sync_kalman_filter(ts, t);
    }
    sync_ingest(ts, 95.0, 0.51); sync_ingest(vs, 15.0, 0.255);
    sync_timestamp_align();
    int ta2 = sync_anomaly_detect(ts, 2.5), va = sync_anomaly_detect(vs, 2.5);
    ps("[ANOMALY] Temp: "); ps(ta2 ? "DETECTED" : "OK"); ps("\n");
    ps("[ANOMALY] Vib: "); ps(va ? "DETECTED" : "OK"); ps("\n");
    ps("[KALMAN] Filtered: "); pd(ts->kalman_x); ps("\n\n");
    /* Predictive Maintenance */
    ps("--- Predictive Maintenance ---\n");
    maintenance_init();
    entity_transition(bearing, ST_DEGR);
    double rul = maintenance_predict(bearing, 4.2, 10);
    maintenance_predict(motor, 72.5, 100);
    maintenance_predict(battery, 3.5, 5);
    maintenance_schedule(5000, 10000);
    ps("[RUL] Bearing: "); pd(rul); ps("h\n\n");
    /* Scenario Simulation */
    ps("--- Scenario Simulation ---\n");
    scenario_t *s1 = scenario_create("temp_increase", "Temperature rise");
    scenario_add_parameter(s1, "ambient_temp", 35, 20, 50);
    scenario_add_parameter(s1, "load_factor", 1.5, 0.8, 2);
    scenario_t *s2 = scenario_create("high_load", "Overload test");
    scenario_add_parameter(s2, "rpm_target", 5000, 3000, 6000);
    scenario_add_parameter(s2, "duration_h", 24, 1, 48);
    scenario_parallel(2);
    ps("\n");
    /* Multi-scale Modeling */
    ps("--- Multi-scale Modeling ---\n");
    twin_model_t *sys = model_create(motor->id, LEVEL_MACRO); model_set_param(sys, 1.0);
    twin_model_t *comp = model_create(motor->id, LEVEL_MESO); model_set_param(comp, 0.95);
    twin_model_t *part = model_create(motor->id, LEVEL_MICRO); model_set_param(part, 0.99);
    model_link(sys, comp); model_link(comp, part);
    model_propagate(sys, 100); model_aggregate(sys);
    ps("[LINKS] Total: "); pi(g_link_count); ps("\n\n");
    /* Dashboard */
    ps("--- Dashboard ---\n");
    dashboard_t *dash = dashboard_create("Operations Overview");
    int w0 = dashboard_add_widget(dash, W_GAUGE, bearing->id, "Bearing Temp", 0, 120);
    widget_set_thresholds(dash, w0, 70, 90);
    int w1 = dashboard_add_widget(dash, W_KPI, motor->id, "Motor OEE", 0, 100);
    dashboard_add_widget(dash, W_CHART, sensor->id, "Sensor Feed", 0, 200);
    dashboard_add_widget(dash, W_ALERT, battery->id, "Battery Alert", 0, 100);
    dashboard_compute_kpi(&dash->wg[w1], 168, 200, 0.92);
    dashboard_check_alerts(dash, w0, 92.5);
    dashboard_render(dash);
    /* State Machine */
    ps("--- State Machine ---\n");
    entity_transition(motor, ST_NORM);
    entity_transition(bearing, ST_MAINT);
    entity_transition(battery, ST_NORM);
    entity_transition(sensor, ST_OFF);
    ps("\n=== Test Suite Complete ===\n");
}

__attribute__((export_name("_start")))
void _start(void) {
    char buf[128]; int test = 0, help = 0;
    int got = host_get_argv(0, sizeof(buf));
    if (got > 0) {
        host_get_argv(0, sizeof(buf));
        for (int i = 0; i < got && i < (int)sizeof(buf) - 1; i++) {
            if (buf[i] == '-' && i + 1 < got) {
                if (buf[i + 1] == 't') test = 1;
                if (buf[i + 1] == 'h') help = 1;
            }
        }
    }
    if (help) { show_help(); host_exit(0); return; }
    ps(VER); ps("\n");
    if (test) run_tests(); else ps("Use -h for help, -t for test mode\n");
    host_exit(0);
}
