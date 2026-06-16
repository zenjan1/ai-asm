/* prometheus_metrics: Prometheus-style metrics monitoring module (v1.0)
 * - Metric types: Counter, Gauge, Histogram, Summary
 * - PromQL: instant/range vectors, aggregations, rate/irate
 * - Alerts: evaluation, Alertmanager, notification channels
 * - TSDB: chunking, compaction, retention, label indexing
 * - SD: static, DNS SRV, Consul, K8s SD, file SD
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
#define MAX_METRICS 64
#define MAX_SERIES 128
#define MAX_SAMPLES 256
#define MAX_LABELS 16
#define MAX_BUCKETS 16
#define MAX_ALERTS 32
#define MAX_CHUNKS 64
#define MAX_TARGETS 32
#define MAX_NAME 32
#define MAX_HELP 64
#define MAX_LABEL_VAL 32
#define MAX_NOTIFY_CH 4
#define ALERT_INACTIVE 0
#define ALERT_PENDING 1
#define ALERT_FIRING 2
#define CHUNK_OPEN 0
#define CHUNK_SEALED 1
#define CHUNK_COMPACTED 2
#define SD_STATIC 0
#define SD_DNS_SRV 1
#define SD_CONSUL 2
#define SD_K8S 3
#define SD_FILE 4
typedef struct { char key[MAX_NAME]; char value[MAX_LABEL_VAL]; } label_t;
typedef struct { int timestamp_ms; int value; } sample_t;
typedef struct { char name[MAX_NAME]; char help[MAX_HELP]; int value; label_t labels[MAX_LABELS]; int label_count; int active; } counter_t;
typedef struct { char name[MAX_NAME]; char help[MAX_HELP]; int value; label_t labels[MAX_LABELS]; int label_count; int active; } gauge_t;
typedef struct { int upper_bound; int count; } hist_bucket_t;
typedef struct { char name[MAX_NAME]; char help[MAX_HELP]; hist_bucket_t buckets[MAX_BUCKETS]; int bucket_count; int sample_count; int sample_sum; label_t labels[MAX_LABELS]; int label_count; int active; } histogram_t;
typedef struct { char name[MAX_NAME]; char help[MAX_HELP]; int sorted_samples[MAX_SAMPLES]; int sample_count; int sample_sum; label_t labels[MAX_LABELS]; int label_count; int active; } summary_t;
typedef struct { char metric_name[MAX_NAME]; label_t labels[MAX_LABELS]; int label_count; sample_t samples[MAX_SAMPLES]; int sample_count; int active; } time_series_t;
typedef struct { char name[MAX_NAME]; char expr[MAX_HELP]; int threshold; int duration_sec; int state; int active_since; int fired_at; char group[MAX_NAME]; } alert_rule_t;
typedef struct { int chunk_id; int min_time; int max_time; sample_t samples[MAX_SAMPLES]; int sample_count; int state; int series_ref; } tsdb_chunk_t;
typedef struct { char address[MAX_NAME]; int port; label_t labels[MAX_LABELS]; int label_count; int sd_type; int active; } sd_target_t;
typedef struct { char name[MAX_NAME]; int type; char target[MAX_NAME]; int active; } notify_channel_t;
static counter_t g_counters[MAX_METRICS];
static gauge_t g_gauges[MAX_METRICS];
static histogram_t g_histograms[MAX_METRICS];
static summary_t g_summaries[MAX_METRICS];
static time_series_t g_series[MAX_SERIES];
static alert_rule_t g_alerts[MAX_ALERTS];
static tsdb_chunk_t g_chunks[MAX_CHUNKS];
static sd_target_t g_targets[MAX_TARGETS];
static notify_channel_t g_notify[MAX_NOTIFY_CH];
static int g_chunk_seq = 0;
/* Forward declarations */
static void print_str(const char *s); static void print_int(int v);
static int str_eq(const char *a, const char *b);
static void str_copy(char *d, const char *s, int m);
static int counter_register(const char *n, const char *h);
static void counter_inc(int id); static int counter_get(int id);
static int gauge_register(const char *n, const char *h);
static void gauge_set(int id, int v); static void gauge_inc(int id);
static void gauge_dec(int id); static int gauge_get(int id);
static int histogram_register(const char *n, const char *h, int *b, int c);
static void histogram_observe(int id, int v);
static int histogram_quantile(int id, int p);
static int summary_register(const char *n, const char *h);
static void summary_observe(int id, int v);
static int summary_quantile(int id, int p);
static int series_register(const char *n, label_t *l, int lc);
static void series_append(int sid, int ts, int v);
static int series_query(const char *n, int ft, int tt);
static int promql_instant_query(const char *e);
static int promql_range_query(const char *e, int ft, int tt);
static int promql_rate(int sid, int w); static int promql_irate(int sid, int w);
static int promql_sum(int *v, int c); static int promql_avg(int *v, int c);
static int promql_min(int *v, int c); static int promql_max(int *v, int c);
static int promql_count(int *v, int c);
static int alert_rule_add(const char *n, const char *e, int t, int d);
static void alert_rule_evaluate(int a, int cv, int nw);
static void alert_fire(int a, int nw); static void alert_resolve(int a);
static void alert_group(int a, const char *g);
static void alert_inhibit(int s, int d); static void alert_silence(int a);
static void alert_notify(int a, int ci);
static int tsdb_chunk_new(int sr, int mt);
static void tsdb_chunk_append(int c, int t, int v);
static void tsdb_compact(int a, int b); static void tsdb_retention(int ma);
static void tsdb_label_index(const char *k, const char *v);
static int tsdb_query(const char *n, int ft, int tt);
static int sd_register_target(const char *a, int p, int st);
static int sd_static(const char *a, int p);
static int sd_dns_srv(const char *s, const char *p, const char *n);
static int sd_consul(const char *s, const char *d);
static int sd_k8s(const char *r, const char *ns); static int sd_file(const char *p);
static void test_suite(void);
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[12]; int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[i++] = '0';
    while (v > 0 && i < 11) { buf[i++] = '0' + (v % 10); v /= 10; }
    if (neg) buf[i++] = '-';
    buf[i] = '\0';
    int l = 0, r = i - 1;
    while (l < r) { char t = buf[l]; buf[l] = buf[r]; buf[r] = t; l++; r--; }
    print_str(buf);
}
static int str_eq(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return (*a == *b) ? 1 : 0;
}
static void str_copy(char *d, const char *s, int m) {
    int i = 0; while (i < m - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int counter_register(const char *name, const char *help) {
    int i;
    for (i = 0; i < MAX_METRICS; i++) if (!g_counters[i].active) {
        str_copy(g_counters[i].name, name, MAX_NAME);
        str_copy(g_counters[i].help, help, MAX_HELP);
        g_counters[i].value = 0; g_counters[i].label_count = 0;
        g_counters[i].active = 1; return i;
    }
    return -1;
}
static void counter_inc(int id) {
    if (id >= 0 && id < MAX_METRICS && g_counters[id].active) g_counters[id].value++;
}
static int counter_get(int id) {
    return (id >= 0 && id < MAX_METRICS && g_counters[id].active) ? g_counters[id].value : 0;
}
static int gauge_register(const char *name, const char *help) {
    int i;
    for (i = 0; i < MAX_METRICS; i++) if (!g_gauges[i].active) {
        str_copy(g_gauges[i].name, name, MAX_NAME);
        str_copy(g_gauges[i].help, help, MAX_HELP);
        g_gauges[i].value = 0; g_gauges[i].label_count = 0;
        g_gauges[i].active = 1; return i;
    }
    return -1;
}
static void gauge_set(int id, int v) { if (id >= 0 && id < MAX_METRICS && g_gauges[id].active) g_gauges[id].value = v; }
static void gauge_inc(int id) { if (id >= 0 && id < MAX_METRICS && g_gauges[id].active) g_gauges[id].value++; }
static void gauge_dec(int id) { if (id >= 0 && id < MAX_METRICS && g_gauges[id].active) g_gauges[id].value--; }
static int gauge_get(int id) { return (id >= 0 && id < MAX_METRICS && g_gauges[id].active) ? g_gauges[id].value : 0; }
static int histogram_register(const char *name, const char *help, int *bounds, int count) {
    int i, j;
    for (i = 0; i < MAX_METRICS; i++) if (!g_histograms[i].active) {
        str_copy(g_histograms[i].name, name, MAX_NAME);
        str_copy(g_histograms[i].help, help, MAX_HELP);
        g_histograms[i].bucket_count = (count < MAX_BUCKETS) ? count : MAX_BUCKETS;
        g_histograms[i].sample_count = 0; g_histograms[i].sample_sum = 0;
        g_histograms[i].label_count = 0; g_histograms[i].active = 1;
        for (j = 0; j < g_histograms[i].bucket_count; j++) {
            g_histograms[i].buckets[j].upper_bound = bounds[j];
            g_histograms[i].buckets[j].count = 0;
        }
        return i;
    }
    return -1;
}
static void histogram_observe(int id, int value) {
    int j;
    if (id < 0 || id >= MAX_METRICS || !g_histograms[id].active) return;
    g_histograms[id].sample_count++; g_histograms[id].sample_sum += value;
    for (j = 0; j < g_histograms[id].bucket_count; j++)
        if (value <= g_histograms[id].buckets[j].upper_bound)
            g_histograms[id].buckets[j].count++;
}
static int histogram_quantile(int id, int phi_pct) {
    if (id < 0 || id >= MAX_METRICS || !g_histograms[id].active) return 0;
    if (g_histograms[id].sample_count == 0) return 0;
    int target = (g_histograms[id].sample_count * phi_pct) / 100;
    int cumulative = 0, j;
    for (j = 0; j < g_histograms[id].bucket_count; j++) {
        cumulative += g_histograms[id].buckets[j].count;
        if (cumulative >= target) return g_histograms[id].buckets[j].upper_bound;
    }
    return g_histograms[id].buckets[g_histograms[id].bucket_count - 1].upper_bound;
}
static int summary_register(const char *name, const char *help) {
    int i;
    for (i = 0; i < MAX_METRICS; i++) if (!g_summaries[i].active) {
        str_copy(g_summaries[i].name, name, MAX_NAME);
        str_copy(g_summaries[i].help, help, MAX_HELP);
        g_summaries[i].sample_count = 0; g_summaries[i].sample_sum = 0;
        g_summaries[i].label_count = 0; g_summaries[i].active = 1; return i;
    }
    return -1;
}
static void summary_insert_sorted(int id, int value) {
    int n = g_summaries[id].sample_count, pos = n, i;
    for (i = 0; i < n; i++)
        if (value <= g_summaries[id].sorted_samples[i]) { pos = i; break; }
    i = n;
    while (i > pos) {
        g_summaries[id].sorted_samples[i] = g_summaries[id].sorted_samples[i-1]; i--;
    }
    g_summaries[id].sorted_samples[pos] = value;
}
static void summary_observe(int id, int value) {
    if (id < 0 || id >= MAX_METRICS || !g_summaries[id].active) return;
    if (g_summaries[id].sample_count >= MAX_SAMPLES) return;
    summary_insert_sorted(id, value);
    g_summaries[id].sample_count++; g_summaries[id].sample_sum += value;
}
static int summary_quantile(int id, int phi_pct) {
    if (id < 0 || id >= MAX_METRICS || !g_summaries[id].active) return 0;
    if (g_summaries[id].sample_count == 0) return 0;
    int idx = (g_summaries[id].sample_count * phi_pct) / 100;
    if (idx >= g_summaries[id].sample_count) idx = g_summaries[id].sample_count - 1;
    return g_summaries[id].sorted_samples[idx];
}
static int series_register(const char *name, label_t *labels, int lcount) {
    int i, j;
    for (i = 0; i < MAX_SERIES; i++) if (!g_series[i].active) {
        str_copy(g_series[i].metric_name, name, MAX_NAME);
        g_series[i].label_count = (lcount < MAX_LABELS) ? lcount : MAX_LABELS;
        for (j = 0; j < g_series[i].label_count; j++) {
            str_copy(g_series[i].labels[j].key, labels[j].key, MAX_NAME);
            str_copy(g_series[i].labels[j].value, labels[j].value, MAX_LABEL_VAL);
        }
        g_series[i].sample_count = 0; g_series[i].active = 1; return i;
    }
    return -1;
}
static void series_append(int sid, int ts_ms, int value) {
    if (sid < 0 || sid >= MAX_SERIES || !g_series[sid].active) return;
    if (g_series[sid].sample_count >= MAX_SAMPLES) return;
    int idx = g_series[sid].sample_count;
    g_series[sid].samples[idx].timestamp_ms = ts_ms;
    g_series[sid].samples[idx].value = value; g_series[sid].sample_count++;
}
static int series_query(const char *name, int from_ts, int to_ts) {
    int total = 0, i, j, ts;
    for (i = 0; i < MAX_SERIES; i++) {
        if (!g_series[i].active || !str_eq(g_series[i].metric_name, name)) continue;
        for (j = 0; j < g_series[i].sample_count; j++) {
            ts = g_series[i].samples[j].timestamp_ms;
            if (ts >= from_ts && ts <= to_ts) total++;
        }
    }
    return total;
}
static int promql_instant_query(const char *expr) {
    int i;
    for (i = 0; i < MAX_METRICS; i++)
        if (g_counters[i].active && str_eq(g_counters[i].name, expr))
            return g_counters[i].value;
    for (i = 0; i < MAX_METRICS; i++)
        if (g_gauges[i].active && str_eq(g_gauges[i].name, expr))
            return g_gauges[i].value;
    return 0;
}
static int promql_range_query(const char *expr, int from_ts, int to_ts) {
    return series_query(expr, from_ts, to_ts);
}
static int promql_rate(int sid, int window_ms) {
    (void)window_ms;
    if (sid < 0 || sid >= MAX_SERIES || !g_series[sid].active) return 0;
    if (g_series[sid].sample_count < 2) return 0;
    int last = g_series[sid].sample_count - 1;
    int td = g_series[sid].samples[last].timestamp_ms - g_series[sid].samples[0].timestamp_ms;
    if (td <= 0) return 0;
    return (g_series[sid].samples[last].value - g_series[sid].samples[0].value) * 1000 / td;
}
static int promql_irate(int sid, int window_ms) {
    (void)window_ms;
    if (sid < 0 || sid >= MAX_SERIES || !g_series[sid].active) return 0;
    int n = g_series[sid].sample_count;
    if (n < 2) return 0;
    int td = g_series[sid].samples[n-1].timestamp_ms - g_series[sid].samples[n-2].timestamp_ms;
    if (td <= 0) return 0;
    return (g_series[sid].samples[n-1].value - g_series[sid].samples[n-2].value) * 1000 / td;
}
static int promql_sum(int *vals, int count) {
    int t = 0, i; for (i = 0; i < count; i++) t += vals[i]; return t;
}
static int promql_avg(int *vals, int count) {
    return count == 0 ? 0 : promql_sum(vals, count) / count;
}
static int promql_min(int *vals, int count) {
    if (count == 0) return 0;
    int m = vals[0], i;
    for (i = 1; i < count; i++) if (vals[i] < m) m = vals[i];
    return m;
}
static int promql_max(int *vals, int count) {
    if (count == 0) return 0;
    int m = vals[0], i;
    for (i = 1; i < count; i++) if (vals[i] > m) m = vals[i];
    return m;
}
static int promql_count(int *vals, int count) { (void)vals; return count; }
static int alert_rule_add(const char *name, const char *expr, int threshold, int dur_sec) {
    int i;
    for (i = 0; i < MAX_ALERTS; i++)
        if (g_alerts[i].state == ALERT_INACTIVE && g_alerts[i].name[0] == '\0') {
            str_copy(g_alerts[i].name, name, MAX_NAME);
            str_copy(g_alerts[i].expr, expr, MAX_HELP);
            g_alerts[i].threshold = threshold; g_alerts[i].duration_sec = dur_sec;
            g_alerts[i].state = ALERT_INACTIVE;
            g_alerts[i].active_since = 0; g_alerts[i].fired_at = 0;
            g_alerts[i].group[0] = '\0'; return i;
        }
    return -1;
}
static void alert_rule_evaluate(int aid, int cv, int now_ms) {
    if (aid < 0 || aid >= MAX_ALERTS) return;
    if (cv > g_alerts[aid].threshold) {
        if (g_alerts[aid].state == ALERT_INACTIVE) {
            g_alerts[aid].state = ALERT_PENDING; g_alerts[aid].active_since = now_ms;
        }
        if (g_alerts[aid].state == ALERT_PENDING &&
            (now_ms - g_alerts[aid].active_since) / 1000 >= g_alerts[aid].duration_sec)
            alert_fire(aid, now_ms);
    } else if (g_alerts[aid].state != ALERT_INACTIVE) alert_resolve(aid);
}
static void alert_fire(int aid, int now_ms) {
    if (aid < 0 || aid >= MAX_ALERTS) return;
    g_alerts[aid].state = ALERT_FIRING; g_alerts[aid].fired_at = now_ms;
    print_str("  ALERT FIRED: "); print_str(g_alerts[aid].name); print_str("\n");
}
static void alert_resolve(int aid) {
    if (aid < 0 || aid >= MAX_ALERTS) return;
    g_alerts[aid].state = ALERT_INACTIVE;
    g_alerts[aid].active_since = 0; g_alerts[aid].fired_at = 0;
}
static void alert_group(int aid, const char *grp) {
    if (aid < 0 || aid >= MAX_ALERTS) return;
    str_copy(g_alerts[aid].group, grp, MAX_NAME);
}
static void alert_inhibit(int src, int dst) {
    if (src < 0 || src >= MAX_ALERTS || dst < 0 || dst >= MAX_ALERTS) return;
    if (g_alerts[src].state == ALERT_FIRING) {
        alert_resolve(dst);
        print_str("  Alert inhibited: "); print_str(g_alerts[dst].name); print_str("\n");
    }
}
static void alert_silence(int aid) {
    if (aid < 0 || aid >= MAX_ALERTS) return;
    g_alerts[aid].state = ALERT_INACTIVE;
    print_str("  Alert silenced: "); print_str(g_alerts[aid].name); print_str("\n");
}
static void alert_notify(int aid, int ci) {
    if (aid < 0 || aid >= MAX_ALERTS || ci < 0 || ci >= MAX_NOTIFY_CH) return;
    if (!g_notify[ci].active) return;
    print_str("  Notification via "); print_str(g_notify[ci].name);
    print_str(" -> "); print_str(g_notify[ci].target);
    print_str(" for alert: "); print_str(g_alerts[aid].name); print_str("\n");
}
static int tsdb_chunk_new(int sr, int mt) {
    int i;
    for (i = 0; i < MAX_CHUNKS; i++)
        if (g_chunks[i].sample_count == 0 || g_chunks[i].state == CHUNK_COMPACTED) {
            g_chunks[i].chunk_id = g_chunk_seq++; g_chunks[i].series_ref = sr;
            g_chunks[i].min_time = mt; g_chunks[i].max_time = mt;
            g_chunks[i].sample_count = 0; g_chunks[i].state = CHUNK_OPEN; return i;
        }
    return -1;
}
static void tsdb_chunk_append(int cid, int ts, int val) {
    if (cid < 0 || cid >= MAX_CHUNKS || g_chunks[cid].state != CHUNK_OPEN) return;
    if (g_chunks[cid].sample_count >= MAX_SAMPLES) return;
    int idx = g_chunks[cid].sample_count;
    g_chunks[cid].samples[idx].timestamp_ms = ts;
    g_chunks[cid].samples[idx].value = val; g_chunks[cid].sample_count++;
    if (ts > g_chunks[cid].max_time) g_chunks[cid].max_time = ts;
}
static void tsdb_compact(int a, int b) {
    if (a < 0 || a >= MAX_CHUNKS || b < 0 || b >= MAX_CHUNKS) return;
    if (g_chunks[a].state != CHUNK_OPEN || g_chunks[b].state != CHUNK_OPEN) return;
    int i;
    for (i = 0; i < g_chunks[b].sample_count; i++)
        if (g_chunks[a].sample_count < MAX_SAMPLES) {
            int idx = g_chunks[a].sample_count;
            g_chunks[a].samples[idx] = g_chunks[b].samples[i];
            g_chunks[a].sample_count++;
        }
    if (g_chunks[b].max_time > g_chunks[a].max_time)
        g_chunks[a].max_time = g_chunks[b].max_time;
    g_chunks[b].state = CHUNK_COMPACTED; g_chunks[b].sample_count = 0;
    print_str("  TSDB compacted chunk "); print_int(b);
    print_str(" into "); print_int(a); print_str("\n");
}
static void tsdb_retention(int ma) {
    int now = 100000, i, rem = 0;
    for (i = 0; i < MAX_CHUNKS; i++)
        if (g_chunks[i].sample_count > 0 && g_chunks[i].state == CHUNK_SEALED)
            if (now - g_chunks[i].max_time > ma) {
                g_chunks[i].state = CHUNK_COMPACTED;
                g_chunks[i].sample_count = 0; rem++;
            }
    print_str("  TSDB retention removed "); print_int(rem); print_str(" chunks\n");
}
static void tsdb_label_index(const char *lk, const char *lv) {
    print_str("  Label index scan: "); print_str(lk);
    print_str("="); print_str(lv); print_str(" -> matches: ");
    int m = 0, i, j;
    for (i = 0; i < MAX_SERIES; i++) {
        if (!g_series[i].active) continue;
        for (j = 0; j < g_series[i].label_count; j++)
            if (str_eq(g_series[i].labels[j].key, lk) &&
                str_eq(g_series[i].labels[j].value, lv)) { m++; break; }
    }
    print_int(m); print_str("\n");
}
static int tsdb_query(const char *name, int ft, int tt) {
    int total = 0, i, j;
    for (i = 0; i < MAX_CHUNKS; i++) {
        if (g_chunks[i].sample_count == 0) continue;
        if (g_chunks[i].series_ref < 0 || g_chunks[i].series_ref >= MAX_SERIES) continue;
        if (!str_eq(g_series[g_chunks[i].series_ref].metric_name, name)) continue;
        for (j = 0; j < g_chunks[i].sample_count; j++) {
            int ts = g_chunks[i].samples[j].timestamp_ms;
            if (ts >= ft && ts <= tt) total++;
        }
    }
    return total;
}
static int sd_register_target(const char *addr, int port, int st) {
    int i;
    for (i = 0; i < MAX_TARGETS; i++) if (!g_targets[i].active) {
        str_copy(g_targets[i].address, addr, MAX_NAME);
        g_targets[i].port = port; g_targets[i].sd_type = st;
        g_targets[i].label_count = 0; g_targets[i].active = 1; return i;
    }
    return -1;
}
static int sd_static(const char *addr, int port) {
    int tid = sd_register_target(addr, port, SD_STATIC);
    if (tid >= 0) { print_str("  SD static: "); print_str(addr); print_str(":"); print_int(port); print_str("\n"); } return tid;
}
static int sd_dns_srv(const char *service, const char *proto, const char *name) {
    int tid = sd_register_target(name, 8500, SD_DNS_SRV);
    if (tid >= 0) { print_str("  SD DNS SRV: _"); print_str(service); print_str("._"); print_str(proto); print_str(" "); print_str(name); print_str("\n"); }
    return tid;
}
static int sd_consul(const char *service, const char *dc) {
    int tid = sd_register_target(service, 8500, SD_CONSUL);
    if (tid >= 0) { print_str("  SD Consul: service="); print_str(service); print_str(" dc="); print_str(dc); print_str("\n"); }
    return tid;
}
static int sd_k8s(const char *role, const char *namespace) {
    int tid = sd_register_target(role, 443, SD_K8S);
    if (tid >= 0) { print_str("  SD K8s: role="); print_str(role); print_str(" ns="); print_str(namespace); print_str("\n"); }
    return tid;
}
static int sd_file(const char *path) {
    int tid = sd_register_target(path, 0, SD_FILE);
    if (tid >= 0) { print_str("  SD File: "); print_str(path); print_str("\n"); }
    return tid;
}
static void test_suite(void) {
    print_str("=== Prometheus Metrics Test Suite ===\n\n");
    print_str("[Test 1] Counter\n");
    int c1 = counter_register("http_requests_total", "Total HTTP requests");
    int c2 = counter_register("errors_total", "Total errors");
    counter_inc(c1); counter_inc(c1); counter_inc(c1); counter_inc(c2);
    print_str("  http_requests="); print_int(counter_get(c1));
    print_str(" errors="); print_int(counter_get(c2)); print_str("\n\n");
    print_str("[Test 2] Gauge\n");
    int g1 = gauge_register("cpu_usage", "CPU usage percent");
    gauge_set(g1, 75); print_str("  set="); print_int(gauge_get(g1));
    gauge_inc(g1); gauge_inc(g1); print_str(" +2="); print_int(gauge_get(g1));
    gauge_dec(g1); gauge_dec(g1); gauge_dec(g1);
    print_str(" -3="); print_int(gauge_get(g1)); print_str("\n\n");
    print_str("[Test 3] Histogram\n");
    int bounds[4]; bounds[0]=10; bounds[1]=50; bounds[2]=100; bounds[3]=500;
    int h1 = histogram_register("request_duration_ms", "Request duration", bounds, 4);
    histogram_observe(h1, 5); histogram_observe(h1, 25); histogram_observe(h1, 75);
    histogram_observe(h1, 200); histogram_observe(h1, 350);
    print_str("  p50="); print_int(histogram_quantile(h1, 50));
    print_str(" p90="); print_int(histogram_quantile(h1, 90));
    print_str(" p99="); print_int(histogram_quantile(h1, 99)); print_str("\n\n");
    print_str("[Test 4] Summary\n");
    int s1 = summary_register("response_size_bytes", "Response size");
    summary_observe(s1, 100); summary_observe(s1, 250); summary_observe(s1, 500);
    summary_observe(s1, 1000); summary_observe(s1, 2000);
    print_str("  p50="); print_int(summary_quantile(s1, 50));
    print_str(" p90="); print_int(summary_quantile(s1, 90)); print_str("\n\n");
    print_str("[Test 5] PromQL\n");
    label_t lbls[1];
    str_copy(lbls[0].key, "method", MAX_NAME);
    str_copy(lbls[0].value, "GET", MAX_LABEL_VAL);
    int sid = series_register("http_requests", lbls, 1);
    series_append(sid, 0, 10); series_append(sid, 15000, 25);
    series_append(sid, 30000, 55); series_append(sid, 45000, 100);
    print_str("  range="); print_int(promql_range_query("http_requests", 0, 45000));
    print_str(" rate="); print_int(promql_rate(sid, 45000));
    print_str(" irate="); print_int(promql_irate(sid, 15000)); print_str("\n");
    int aggr[4]; aggr[0]=10; aggr[1]=25; aggr[2]=55; aggr[3]=100;
    print_str("  sum="); print_int(promql_sum(aggr, 4));
    print_str(" avg="); print_int(promql_avg(aggr, 4));
    print_str(" min="); print_int(promql_min(aggr, 4));
    print_str(" max="); print_int(promql_max(aggr, 4));
    print_str(" count="); print_int(promql_count(aggr, 4)); print_str("\n");
    print_str("  instant="); print_int(promql_instant_query("http_requests_total")); print_str("\n\n");
    print_str("[Test 6] Alerts\n");
    int a1 = alert_rule_add("HighErrorRate", "errors_total > 5", 5, 30);
    int a2 = alert_rule_add("HighCPU", "cpu_usage > 90", 90, 60);
    alert_group(a1, "errors"); alert_group(a2, "infrastructure");
    alert_rule_evaluate(a1, 10, 0);
    print_str("  state="); print_int(g_alerts[a1].state);
    alert_rule_evaluate(a1, 10, 31000);
    g_notify[0].active = 1; str_copy(g_notify[0].name, "slack", MAX_NAME);
    str_copy(g_notify[0].target, "#alerts", MAX_NAME); g_notify[0].type = 1;
    alert_notify(a1, 0); alert_inhibit(a1, a2); alert_silence(a1);
    alert_rule_evaluate(a1, 2, 60000); print_str("\n");
    print_str("[Test 7] TSDB\n");
    int chunk1 = tsdb_chunk_new(sid, 0); int chunk2 = tsdb_chunk_new(sid, 5000);
    int k;
    for (k = 0; k < 5; k++) tsdb_chunk_append(chunk1, k*1000, k*10);
    for (k = 0; k < 3; k++) tsdb_chunk_append(chunk2, 5000+k*1000, 50+k*10);
    print_str("  c1="); print_int(g_chunks[chunk1].sample_count);
    print_str(" c2="); print_int(g_chunks[chunk2].sample_count); print_str("\n");
    tsdb_compact(chunk1, chunk2);
    print_str("  merged="); print_int(g_chunks[chunk1].sample_count); print_str("\n");
    tsdb_label_index("method", "GET");
    print_str("  query="); print_int(tsdb_query("http_requests", 0, 10000)); print_str("\n");
    g_chunks[chunk1].state = CHUNK_SEALED; tsdb_retention(200000); print_str("\n");
    print_str("[Test 8] SD\n");
    sd_static("192.168.1.10", 9090); sd_dns_srv("prometheus", "tcp", "consul.local");
    sd_consul("node-exporter", "dc1"); sd_k8s("pod", "monitoring");
    sd_file("/etc/prometheus/targets.json");
    print_str("\n=== Prometheus Metrics Test Complete ===\n");
}
void _start(void) {
    unsigned int buf_off = host_alloc(256, 4);
    int argc = host_get_argv(buf_off, 256); (void)argc;
    const char *argbuf = (const char *)(buf_off);
    int has_help = 0, has_test = 0, idx;
    for (idx = 0; idx < 250; idx++) {
        if (argbuf[idx] == '-' && argbuf[idx+1] == 'h') has_help = 1;
        if (argbuf[idx] == '-' && argbuf[idx+1] == 't') has_test = 1;
    }
    if (has_help) {
        print_str("prometheus_metrics v1.0\n  -h  Show this help\n  -t  Run test suite\n");
        print_str("Features: Counter/Gauge/Histogram/Summary, PromQL,\n  Alert rules, TSDB chunking, Service Discovery\n");
        host_exit(0); return;
    }
    if (has_test) { test_suite(); host_exit(0); return; }
    print_str("prometheus_metrics: use -h for help, -t for test\n");
}
