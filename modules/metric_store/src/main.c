/*
 * metric_store - Prometheus-style time-series metric store for AI-ASM OS
 * Ingestion, storage, querying, alerting, and visualization.
 */
#include <stddef.h>
/* Host imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* Constants */
#define MAX_METRICS    256
#define MAX_LABELS     8
#define MAX_SERIES     128
#define MAX_SAMPLES    128
#define MAX_ALERT_RULES 64
#define MAX_DASHBOARDS 16
#define MAX_SHARDS     8
#define MAX_QRESULT    512
#define MAX_STR        128
#define MAX_DS_RULES   8
#define MAX_ENDPOINTS  8
#define MAX_PANELS     8
#define MAX_PQUERIES   4
#define MAX_TPLVARS    4
#define MAX_COLLECT    512
#define FLAG_ACTIVE    0x0001
#define FLAG_COMPRESS  0x0002
#define FLAG_SILENCED  0x0004
#define FLAG_EXPORTED  0x0008
#define TYPE_GAUGE     0
#define TYPE_COUNTER   1
#define TYPE_HISTOGRAM 2
#define TYPE_SUMMARY   3
#define AGG_AVG  0
#define AGG_SUM   1
#define AGG_MIN   2
#define AGG_MAX   3
#define AGG_COUNT 4
#define AGG_RATE  5
#define AGG_IRATE 6
#define AGG_DELTA 7
#define ALERT_THRESHOLD  0
#define ALERT_TREND      1
#define ALERT_PREDICTIVE 2
#define SHARD_HASH  0
#define SHARD_RANGE 1
#define MATH_NONE 0
#define MATH_ADD  1
#define MATH_SUB  2
#define MATH_MUL  3
#define MATH_DIV  4
#define PROTO_PROM_PULL 0
#define PROTO_PROM_PUSH 1
#define PROTO_STATSD    2
#define PROTO_OTEL      3
#define PROTO_CUSTOM    4
/* Data types */
typedef struct { char key[MAX_STR]; char value[MAX_STR]; } label_t;
typedef struct { long long timestamp_ms; double value; } sample_t;
typedef struct {
    unsigned int id, metric_idx, num_labels;
    label_t      labels[MAX_LABELS];
    sample_t     samples[MAX_SAMPLES];
    unsigned int sample_count, flags, shard_id;
} time_series_t;

typedef struct {
    char name[MAX_STR]; unsigned int type;
    char help[MAX_STR]; unsigned int series_count, flags;
} metric_t;

typedef struct {
    long long *timestamps; double *values;
    unsigned int count, capacity, flags;
} columnar_block_t;

typedef struct { unsigned int interval_sec, retention_sec, agg_func; } ds_rule_t;
typedef struct { unsigned int shard_id, series_start, series_end, method, flags; } shard_cfg_t;

typedef struct {
    ds_rule_t      rules[MAX_DS_RULES];  unsigned int rule_count;
    shard_cfg_t    shards[MAX_SHARDS];   unsigned int shard_count;
    unsigned long long retention_ms;     unsigned int flags;
} storage_engine_t;

typedef struct {
    unsigned int protocol_id; char endpoint[MAX_STR];
    unsigned int port, flags;
} ingest_ep_t;

typedef struct {
    char metric_name[MAX_STR];
    long long range_start_ms, range_end_ms;
    unsigned int agg_func;
    char lfilter_key[MAX_STR], lfilter_val[MAX_STR];
    double math_operand; unsigned int math_op, flags;
} query_t;

typedef struct {
    long long timestamps[MAX_QRESULT]; double values[MAX_QRESULT];
    unsigned int count, flags;
} query_result_t;

typedef struct {
    unsigned int type; char metric_name[MAX_STR];
    double threshold, slope_per_sec, prediction_horizon_sec;
    unsigned int eval_window_sec, severity;
    char route_target[MAX_STR];
    unsigned int silence_dur_sec, inhibit_flags, flags;
} alert_rule_t;

typedef struct {
    unsigned int rule_id; long long fired_at_ms;
    double current_value; char message[MAX_STR];
    unsigned int resolved, flags;
} alert_inst_t;

typedef struct {
    char title[MAX_STR]; unsigned int chart_type;
    char queries[MAX_PQUERIES][MAX_STR]; unsigned int query_count;
    char tpl_vars[MAX_TPLVARS][MAX_STR]; unsigned int tpl_var_count;
    unsigned int flags;
} panel_t;

typedef struct {
    char name[MAX_STR]; panel_t panels[MAX_PANELS];
    unsigned int panel_count, flags;
} dashboard_t;
/* Global state */
static metric_t        g_metrics[MAX_METRICS];       static unsigned int g_metric_cnt = 0;
static time_series_t   g_series[MAX_SERIES];         static unsigned int g_series_cnt = 0;
static storage_engine_t g_storage;
static ingest_ep_t     g_endpoints[MAX_ENDPOINTS];   static unsigned int g_ep_cnt = 0;
static alert_rule_t    g_alert_rules[MAX_ALERT_RULES]; static unsigned int g_arule_cnt = 0;
static alert_inst_t    g_alert_insts[MAX_ALERT_RULES]; static unsigned int g_ainst_cnt = 0;
static dashboard_t     g_dashboards[MAX_DASHBOARDS]; static unsigned int g_dash_cnt = 0;
/* Forward declarations */
static unsigned int hash_str(const char *s);
static int find_metric(const char *name);
static unsigned int find_series(unsigned int midx, const label_t *l, unsigned int nl);
static int ingest_sample(unsigned int sid, long long ts, double val);
static query_result_t exec_query(const query_t *q);
static void eval_alerts(void);
static unsigned int compute_shard(const char *n, unsigned int lh);
static void init_storage(void);
static double agg_vals(const double *v, unsigned int c, unsigned int f);
/* Utilities */
static void mz(void *d, unsigned int n) {
    unsigned char *p = (unsigned char *)d;
    for (unsigned int i = 0; i < n; i++) p[i] = 0;
}
static void sc(char *d, const char *s, unsigned int mx) {
    unsigned int i = 0;
    while (i < mx - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = 0;
}
static int se(const char *a, const char *b) {
    unsigned int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; i++; }
    return a[i] == b[i];
}
static unsigned int hash_str(const char *s) {
    unsigned int h = 5381, i = 0;
    while (s[i]) { h = ((h << 5) + h) + (unsigned int)s[i]; i++; }
    return h;
}
static long long now_ms(void) { return 1718600000000LL; }
static void log_msg(const char *m) {
    host_print("[metric_store] "); host_print(m); host_print("\n");
}
/* Storage init */
static void init_storage(void) {
    mz(&g_storage, sizeof(storage_engine_t));
    g_storage.retention_ms = 30ULL * 24ULL * 3600ULL * 1000ULL;
    g_storage.rules[0] = (ds_rule_t){300, 86400, AGG_AVG};
    g_storage.rules[1] = (ds_rule_t){3600, 2592000, AGG_AVG};
    g_storage.rule_count = 2;
    g_storage.shards[0] = (shard_cfg_t){0, 0, MAX_SERIES, SHARD_HASH, FLAG_ACTIVE};
    g_storage.shard_count = 1;
    log_msg("storage engine initialized");
}
static unsigned int compute_shard(const char *n, unsigned int lh) {
    if (!g_storage.shard_count) return 0;
    return (hash_str(n) ^ lh) % g_storage.shard_count;
}
/* Metric/series creation */
static int find_metric(const char *name) {
    for (unsigned int i = 0; i < g_metric_cnt; i++)
        if (se(g_metrics[i].name, name)) return (int)i;
    if (g_metric_cnt >= MAX_METRICS) { log_msg("ERR: max metrics"); return -1; }
    unsigned int idx = g_metric_cnt++;
    mz(&g_metrics[idx], sizeof(metric_t));
    sc(g_metrics[idx].name, name, MAX_STR);
    g_metrics[idx].type = TYPE_GAUGE; g_metrics[idx].flags = FLAG_ACTIVE;
    return (int)idx;
}
static unsigned int find_series(unsigned int midx, const label_t *lbl, unsigned int nl) {
    for (unsigned int i = 0; i < g_series_cnt; i++) {
        if (g_series[i].metric_idx != midx || g_series[i].num_labels != nl) continue;
        unsigned int ok = 1;
        for (unsigned int l = 0; l < nl; l++)
            if (!se(g_series[i].labels[l].key, lbl[l].key) ||
                !se(g_series[i].labels[l].value, lbl[l].value)) { ok = 0; break; }
        if (ok) return i;
    }
    if (g_series_cnt >= MAX_SERIES) { log_msg("ERR: max series"); return (unsigned int)-1; }
    unsigned int sid = g_series_cnt++;
    mz(&g_series[sid], sizeof(time_series_t));
    g_series[sid].id = sid; g_series[sid].metric_idx = midx;
    g_series[sid].num_labels = nl; g_series[sid].flags = FLAG_ACTIVE;
    for (unsigned int l = 0; l < nl && l < MAX_LABELS; l++) {
        sc(g_series[sid].labels[l].key, lbl[l].key, MAX_STR);
        sc(g_series[sid].labels[l].value, lbl[l].value, MAX_STR);
    }
    unsigned int lh = 0;
    for (unsigned int l = 0; l < nl; l++) { lh ^= hash_str(lbl[l].key); lh ^= hash_str(lbl[l].value); }
    g_series[sid].shard_id = compute_shard(g_metrics[midx].name, lh);
    g_metrics[midx].series_count++;
    return sid;
}
/* Sample ingestion */
static int ingest_sample(unsigned int sid, long long ts, double val) {
    if (sid >= g_series_cnt) return -1;
    time_series_t *t = &g_series[sid];
    if (t->sample_count >= MAX_SAMPLES) {
        unsigned int h = t->sample_count / 2;
        for (unsigned int i = 0; i < h; i++) t->samples[i] = t->samples[h + i];
        t->sample_count -= h;
    }
    t->samples[t->sample_count].timestamp_ms = ts;
    t->samples[t->sample_count].value = val;
    t->sample_count++;
    return 0;
}
/* Ingestion protocols */
static int ing_prom_pull(const char *n, const label_t *l, unsigned int nl, double v) {
    int m = find_metric(n); if (m < 0) return -1;
    g_metrics[m].type = TYPE_GAUGE;
    unsigned int s = find_series((unsigned int)m, l, nl);
    return (s == (unsigned int)-1) ? -1 : ingest_sample(s, now_ms(), v);
}
static int ing_prom_push(const char *n, const label_t *l, unsigned int nl, long long ts, double v) {
    int m = find_metric(n); if (m < 0) return -1;
    unsigned int s = find_series((unsigned int)m, l, nl);
    return (s == (unsigned int)-1) ? -1 : ingest_sample(s, ts, v);
}
static int ing_statsd(const char *n, double v, unsigned int st) {
    label_t lb; sc(lb.key, "source", MAX_STR); sc(lb.value, "statsd", MAX_STR);
    int m = find_metric(n); if (m < 0) return -1;
    if (st == 0) g_metrics[m].type = TYPE_GAUGE;
    else if (st == 1) g_metrics[m].type = TYPE_COUNTER;
    unsigned int s = find_series((unsigned int)m, &lb, 1);
    return (s == (unsigned int)-1) ? -1 : ingest_sample(s, now_ms(), v);
}
static int ing_otel(const char *n, const label_t *l, unsigned int nl, long long ts, double v) {
    int m = find_metric(n); if (m < 0) return -1;
    unsigned int s = find_series((unsigned int)m, l, nl);
    return (s == (unsigned int)-1) ? -1 : ingest_sample(s, ts, v);
}
static int ing_custom(const char *n, double v, long long ts) {
    label_t lb; sc(lb.key, "source", MAX_STR); sc(lb.value, "custom", MAX_STR);
    int m = find_metric(n); if (m < 0) return -1;
    unsigned int s = find_series((unsigned int)m, &lb, 1);
    return (s == (unsigned int)-1) ? -1 : ingest_sample(s, ts, v);
}
/* Columnar compression */
static columnar_block_t compress_columnar(unsigned int sid) {
    columnar_block_t b; mz(&b, sizeof(b));
    if (sid >= g_series_cnt) return b;
    time_series_t *t = &g_series[sid];
    unsigned int cap = t->sample_count + 16;
    b.timestamps = (long long *)host_alloc(cap * sizeof(long long), 8);
    b.values = (double *)host_alloc(cap * sizeof(double), 8);
    if (!b.timestamps || !b.values) { log_msg("ERR: alloc"); return b; }
    for (unsigned int i = 0; i < t->sample_count; i++) {
        b.timestamps[i] = t->samples[i].timestamp_ms; b.values[i] = t->samples[i].value;
    }
    b.count = t->sample_count; b.capacity = cap; b.flags = FLAG_COMPRESS;
    return b;
}
/* Aggregation */
static double agg_vals(const double *v, unsigned int c, unsigned int f) {
    if (!c) return 0.0;
    double r = v[0], s = 0.0;
    switch (f) {
        case AGG_AVG: for (unsigned int i = 0; i < c; i++) s += v[i]; return s / (double)c;
        case AGG_SUM: for (unsigned int i = 0; i < c; i++) s += v[i]; return s;
        case AGG_MIN: for (unsigned int i = 1; i < c; i++) if (v[i] < r) r = v[i]; return r;
        case AGG_MAX: for (unsigned int i = 1; i < c; i++) if (v[i] > r) r = v[i]; return r;
        case AGG_COUNT: return (double)c;
        case AGG_RATE: return (c < 2) ? 0.0 : v[c-1] - v[0];
        case AGG_IRATE: case AGG_DELTA: return (c < 2) ? 0.0 : v[c-1] - v[c-2];
        default: return r;
    }
}
/* Downsampling */
static query_result_t downsample(unsigned int sid, unsigned int intv_sec, unsigned int af) {
    query_result_t res; mz(&res, sizeof(res));
    if (sid >= g_series_cnt) return res;
    time_series_t *t = &g_series[sid];
    if (!t->sample_count) return res;
    long long intv_ms = (long long)intv_sec * 1000LL;
    long long bstart = t->samples[0].timestamp_ms;
    double bv[MAX_SAMPLES]; unsigned int bc = 0;
    for (unsigned int i = 0; i < t->sample_count && res.count < MAX_QRESULT; i++) {
        if (t->samples[i].timestamp_ms - bstart >= intv_ms) {
            if (bc) { res.timestamps[res.count] = bstart; res.values[res.count] = agg_vals(bv, bc, af); res.count++; }
            bstart = t->samples[i].timestamp_ms; bc = 0;
        }
        bv[bc++] = t->samples[i].value;
    }
    if (bc && res.count < MAX_QRESULT) {
        res.timestamps[res.count] = bstart; res.values[res.count] = agg_vals(bv, bc, af); res.count++;
    }
    res.flags = FLAG_ACTIVE; return res;
}
/* Retention enforcement */
static void enforce_retention(void) {
    long long cutoff = now_ms() - (long long)g_storage.retention_ms;
    unsigned int pruned = 0;
    for (unsigned int s = 0; s < g_series_cnt; s++) {
        time_series_t *t = &g_series[s]; unsigned int kf = 0;
        for (unsigned int i = 0; i < t->sample_count; i++)
            if (t->samples[i].timestamp_ms >= cutoff) { kf = i; break; }
        if (kf > 0) {
            unsigned int nc = t->sample_count - kf;
            for (unsigned int i = 0; i < nc; i++) t->samples[i] = t->samples[kf + i];
            t->sample_count = nc; pruned++;
        }
    }
    if (pruned) log_msg("retention applied");
}
/* Query execution */
static query_result_t exec_query(const query_t *q) {
    query_result_t result; mz(&result, sizeof(result));
    int midx = -1;
    for (unsigned int i = 0; i < g_metric_cnt; i++)
        if (se(g_metrics[i].name, q->metric_name)) { midx = (int)i; break; }
    if (midx < 0) return result;
    unsigned int tm = (unsigned int)midx;
    double coll[MAX_COLLECT]; unsigned int cc = 0;
    for (unsigned int si = 0; si < g_series_cnt; si++) {
        time_series_t *t = &g_series[si];
        if (t->metric_idx != tm) continue;
        if (q->lfilter_key[0]) {
            unsigned int found = 0;
            for (unsigned int l = 0; l < t->num_labels; l++)
                if (se(t->labels[l].key, q->lfilter_key) && se(t->labels[l].value, q->lfilter_val))
                    { found = 1; break; }
            if (!found) continue;
        }
        for (unsigned int i = 0; i < t->sample_count; i++) {
            long long ts = t->samples[i].timestamp_ms;
            if (q->range_start_ms > 0 && ts < q->range_start_ms) continue;
            if (q->range_end_ms > 0 && ts > q->range_end_ms) continue;
            if (result.count < MAX_QRESULT) result.timestamps[result.count] = ts;
            if (cc < MAX_COLLECT) coll[cc++] = t->samples[i].value;
            if (result.count < MAX_QRESULT) result.count++;
        }
    }
    if (q->agg_func >= AGG_AVG && q->agg_func <= AGG_DELTA && cc > 0) {
        double av = agg_vals(coll, cc, q->agg_func);
        if (result.count > 0) { result.values[0] = av; result.count = 1; }
    } else {
        for (unsigned int i = 0; i < cc && i < result.count; i++) result.values[i] = coll[i];
    }
    if (q->math_op != MATH_NONE) for (unsigned int i = 0; i < result.count; i++) {
        switch (q->math_op) {
            case MATH_ADD: result.values[i] += q->math_operand; break;
            case MATH_SUB: result.values[i] -= q->math_operand; break;
            case MATH_MUL: result.values[i] *= q->math_operand; break;
            case MATH_DIV: result.values[i] = q->math_operand ? result.values[i] / q->math_operand : 0.0; break;
        }
    }
    result.flags = FLAG_ACTIVE; return result;
}
/* Alert system */
static void fire_alert(unsigned int rid, double cv, const char *msg) {
    if (g_ainst_cnt >= MAX_ALERT_RULES) return;
    unsigned int idx = g_ainst_cnt++;
    mz(&g_alert_insts[idx], sizeof(alert_inst_t));
    g_alert_insts[idx].rule_id = rid; g_alert_insts[idx].fired_at_ms = now_ms();
    g_alert_insts[idx].current_value = cv; sc(g_alert_insts[idx].message, msg, MAX_STR);
    g_alert_insts[idx].flags = FLAG_ACTIVE; log_msg("ALERT FIRED");
}
static void eval_alerts(void) {
    for (unsigned int r = 0; r < g_arule_cnt; r++) {
        alert_rule_t *ru = &g_alert_rules[r];
        if (!(ru->flags & FLAG_ACTIVE) || (ru->flags & FLAG_SILENCED)) continue;
        query_t q; mz(&q, sizeof(q));
        sc(q.metric_name, ru->metric_name, MAX_STR);
        q.range_start_ms = now_ms() - (long long)ru->eval_window_sec * 1000LL;
        q.range_end_ms = now_ms(); q.agg_func = AGG_AVG;
        query_result_t qr = exec_query(&q);
        if (!qr.count) continue;
        double lat = qr.values[qr.count - 1];
        if (ru->type == ALERT_THRESHOLD) {
            if (lat > ru->threshold) fire_alert(r, lat, "threshold exceeded");
        } else if (ru->type == ALERT_TREND && qr.count >= 2) {
            double sl = (lat - qr.values[0]) / (double)ru->eval_window_sec;
            if (sl > ru->slope_per_sec) fire_alert(r, lat, "trend exceeded");
        } else if (ru->type == ALERT_PREDICTIVE && qr.count >= 2) {
            double sl = (lat - qr.values[0]) / (double)ru->eval_window_sec;
            double pred = lat + sl * ru->prediction_horizon_sec;
            if (pred > ru->threshold) fire_alert(r, pred, "predicted breach");
        }
    }
}
static void silence_alert(unsigned int rid, unsigned int dur) {
    if (rid < g_arule_cnt) { g_alert_rules[rid].flags |= FLAG_SILENCED; g_alert_rules[rid].silence_dur_sec = dur; }
}
/* Dashboard/visualization */
static int create_dash(const char *name) {
    if (g_dash_cnt >= MAX_DASHBOARDS) { log_msg("ERR: max dashboards"); return -1; }
    unsigned int idx = g_dash_cnt++;
    mz(&g_dashboards[idx], sizeof(dashboard_t));
    sc(g_dashboards[idx].name, name, MAX_STR); g_dashboards[idx].flags = FLAG_ACTIVE;
    return (int)idx;
}
static int add_panel(int di, const char *title, unsigned int ct, const char **qs, unsigned int qc) {
    if (di < 0 || (unsigned int)di >= g_dash_cnt) return -1;
    dashboard_t *d = &g_dashboards[di];
    if (d->panel_count >= MAX_PANELS) return -1;
    unsigned int pi = d->panel_count++;
    mz(&d->panels[pi], sizeof(panel_t));
    sc(d->panels[pi].title, title, MAX_STR); d->panels[pi].chart_type = ct;
    for (unsigned int i = 0; i < qc && i < MAX_PQUERIES; i++)
        sc(d->panels[pi].queries[i], qs[i], MAX_STR);
    d->panels[pi].query_count = qc; d->panels[pi].flags = FLAG_ACTIVE;
    return (int)pi;
}
static void set_tplvar(int di, const char *vn) {
    if (di < 0 || (unsigned int)di >= g_dash_cnt || !g_dashboards[di].panel_count) return;
    panel_t *p = &g_dashboards[di].panels[g_dashboards[di].panel_count - 1];
    if (p->tpl_var_count < MAX_TPLVARS) sc(p->tpl_vars[p->tpl_var_count++], vn, MAX_STR);
}
static void export_dash(int di) {
    if (di >= 0 && (unsigned int)di < g_dash_cnt) { g_dashboards[di].flags |= FLAG_EXPORTED; log_msg("exported"); }
}
static query_result_t render_panel(int di, int pi) {
    query_result_t comb; mz(&comb, sizeof(comb));
    if (di < 0 || (unsigned int)di >= g_dash_cnt) return comb;
    dashboard_t *d = &g_dashboards[di];
    if (pi < 0 || (unsigned int)pi >= d->panel_count) return comb;
    panel_t *p = &d->panels[pi];
    for (unsigned int i = 0; i < p->query_count; i++) {
        query_t q; mz(&q, sizeof(q));
        sc(q.metric_name, p->queries[i], MAX_STR); q.agg_func = AGG_AVG;
        query_result_t qr = exec_query(&q);
        for (unsigned int j = 0; j < qr.count && comb.count < MAX_QRESULT; j++) {
            comb.timestamps[comb.count] = qr.timestamps[j]; comb.values[comb.count] = qr.values[j]; comb.count++;
        }
    }
    comb.flags = FLAG_ACTIVE; return comb;
}
/* Endpoint registration */
static int reg_endpoint(unsigned int proto, const char *ep, unsigned int port) {
    if (g_ep_cnt >= MAX_ENDPOINTS) return -1;
    unsigned int idx = g_ep_cnt++;
    g_endpoints[idx].protocol_id = proto; sc(g_endpoints[idx].endpoint, ep, MAX_STR);
    g_endpoints[idx].port = port; g_endpoints[idx].flags = FLAG_ACTIVE;
    log_msg("endpoint registered"); return (int)idx;
}
/* WASM exports */
__attribute__((export_name("init")))
void wasm_init(void) {
    log_msg("metric_store loading..."); init_storage();
    g_metric_cnt = g_series_cnt = g_arule_cnt = g_ainst_cnt = g_dash_cnt = g_ep_cnt = 0;
    log_msg("metric_store ready");
}
__attribute__((export_name("ingest_prom_pull")))
int wasm_ing_pp(unsigned int no, unsigned int lo, unsigned int nl, double v) {
    return ing_prom_pull((const char *)no, (const label_t *)lo, nl, v);
}
__attribute__((export_name("ingest_prom_push")))
int wasm_ing_pu(unsigned int no, unsigned int lo, unsigned int nl, long long ts, double v) {
    return ing_prom_push((const char *)no, (const label_t *)lo, nl, ts, v);
}
__attribute__((export_name("ingest_statsd")))
int wasm_ing_sd(unsigned int no, double v, unsigned int st) { return ing_statsd((const char *)no, v, st); }
__attribute__((export_name("ingest_otel")))
int wasm_ing_ot(unsigned int no, unsigned int lo, unsigned int nl, long long ts, double v) {
    return ing_otel((const char *)no, (const label_t *)lo, nl, ts, v);
}
__attribute__((export_name("ingest_custom")))
int wasm_ing_cu(unsigned int no, double v, long long ts) { return ing_custom((const char *)no, v, ts); }
__attribute__((export_name("query")))
unsigned int wasm_query(unsigned int no, long long rs, long long re, unsigned int af, unsigned int rbo) {
    query_t q; mz(&q, sizeof(q)); sc(q.metric_name, (const char *)no, MAX_STR);
    q.range_start_ms = rs; q.range_end_ms = re; q.agg_func = af;
    query_result_t qr = exec_query(&q);
    if (rbo) *(query_result_t *)rbo = qr;
    return qr.count;
}
__attribute__((export_name("add_alert_rule")))
int wasm_add_alert(unsigned int tp, unsigned int no, double th, unsigned int ew, unsigned int sv) {
    if (g_arule_cnt >= MAX_ALERT_RULES) return -1;
    unsigned int idx = g_arule_cnt++;
    mz(&g_alert_rules[idx], sizeof(alert_rule_t));
    g_alert_rules[idx].type = tp; sc(g_alert_rules[idx].metric_name, (const char *)no, MAX_STR);
    g_alert_rules[idx].threshold = th; g_alert_rules[idx].eval_window_sec = ew;
    g_alert_rules[idx].severity = sv; g_alert_rules[idx].flags = FLAG_ACTIVE;
    return (int)idx;
}
__attribute__((export_name("eval_alerts")))
void wasm_eval_alerts(void) { eval_alerts(); }
__attribute__((export_name("silence_alert")))
void wasm_silence(unsigned int rid, unsigned int dur) { silence_alert(rid, dur); }
__attribute__((export_name("create_dashboard")))
int wasm_mk_dash(unsigned int no) { return create_dash((const char *)no); }
__attribute__((export_name("add_panel")))
int wasm_add_panel(int di, unsigned int to, unsigned int ct, unsigned int qo, unsigned int qc) {
    return add_panel(di, (const char *)to, ct, (const char **)qo, qc);
}
__attribute__((export_name("set_template_var")))
void wasm_tplvar(int di, unsigned int vo) { set_tplvar(di, (const char *)vo); }
__attribute__((export_name("export_dashboard")))
void wasm_export(int di) { export_dash(di); }
__attribute__((export_name("render_panel")))
unsigned int wasm_render(int di, int pi, unsigned int rbo) {
    query_result_t qr = render_panel(di, pi);
    if (rbo) *(query_result_t *)rbo = qr;
    return qr.count;
}
__attribute__((export_name("register_endpoint")))
int wasm_reg_ep(unsigned int pr, unsigned int eo, unsigned int po) {
    return reg_endpoint(pr, (const char *)eo, po);
}
__attribute__((export_name("downsample")))
unsigned int wasm_ds(unsigned int sid, unsigned int is, unsigned int af, unsigned int rbo) {
    query_result_t qr = downsample(sid, is, af);
    if (rbo) *(query_result_t *)rbo = qr;
    return qr.count;
}
__attribute__((export_name("enforce_retention")))
void wasm_retention(void) { enforce_retention(); }
__attribute__((export_name("compress_columnar")))
unsigned int wasm_compress(unsigned int sid) { return compress_columnar(sid).count; }
__attribute__((export_name("get_metric_count")))
unsigned int wasm_gmc(void) { return g_metric_cnt; }
__attribute__((export_name("get_series_count")))
unsigned int wasm_gsc(void) { return g_series_cnt; }
__attribute__((export_name("get_alert_count")))
unsigned int wasm_gac(void) { return g_ainst_cnt; }
__attribute__((export_name("get_dashboard_count")))
unsigned int wasm_gdc(void) { return g_dash_cnt; }
