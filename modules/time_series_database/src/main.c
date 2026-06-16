/* time_series_database: InfluxDB/TimescaleDB-style time-series database WASM module */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_MEAS 32
#define MAX_TAGS 8
#define MAX_FIELDS 16
#define MAX_PTS 256
#define MAX_WAL 512
#define MAX_TSM 128
#define MAX_CQS 16
#define MAX_RET 8
#define MAX_NAME 32
#define NANOS 1000000000
#define WAL_MAGIC 0x57414C01
#define TSM_MAGIC 0x54534D01

typedef struct { char key[24]; char val[32]; } tag_t;
typedef struct { char key[24]; long long val; } field_t;
typedef struct { long long ts; tag_t tags[MAX_TAGS]; int tc; field_t fields[MAX_FIELDS]; int fc; } point_t;
typedef struct { char name[MAX_NAME]; point_t pts[MAX_PTS]; int pc; int active; } meas_t;
typedef struct { long long ts; int mid; } wal_entry_t;
typedef struct { unsigned int magic; wal_entry_t entries[MAX_WAL]; int ec; int seq; } wal_log_t;
typedef struct { long long ts[MAX_PTS]; long long vals[MAX_PTS]; int cnt; } memtable_t;
typedef struct { unsigned int magic; long long min_ts; long long max_ts; long long vals[MAX_PTS]; long long ts[MAX_PTS]; int cnt; int comp; } tsm_block_t;
typedef struct { char name[MAX_NAME]; int mid; int win; int active; char agg[12]; } cq_t;
typedef struct { char name[MAX_NAME]; int dur; int tier; int active; } ret_t;
typedef struct { char name[12]; int min_age; int max_age; int pc; } tier_t;

static meas_t g_meas[MAX_MEAS]; static int g_mc = 0;
static wal_log_t g_wal;
static memtable_t g_mem;
static tsm_block_t g_tsm[MAX_TSM]; static int g_tc = 0;
static cq_t g_cqs[MAX_CQS]; static int g_cc = 0;
static ret_t g_ret[MAX_RET]; static int g_rc = 0;
static tier_t g_tiers[3]; static int g_tic = 0;

static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static void print_str(const char *s) { host_print(s); }
static void print_int(long long v) {
    char buf[24]; int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[i++] = '0';
    else { while (v > 0) { buf[i++] = '0' + (char)(v % 10); v /= 10; } }
    if (neg) buf[i++] = '-'; buf[i] = '\0';
    for (int j = 0, k = i - 1; j < k; j++, k--) { char t = buf[j]; buf[j] = buf[k]; buf[k] = t; }
    print_str(buf);
}
static void print_test(const char *name, int pass) {
    print_str(pass ? "  [PASS] " : "  [FAIL] "); print_str(name); print_str("\n");
}
static long long ts_now(void) { return (long long)1718000000 * NANOS; }

static int measurement_create(const char *name) {
    if (g_mc >= MAX_MEAS) return -1;
    int id = g_mc++;
    my_strncpy(g_meas[id].name, name, MAX_NAME);
    g_meas[id].pc = 0; g_meas[id].active = 1;
    return id;
}
static int measurement_find(const char *name) {
    for (int i = 0; i < g_mc; i++)
        if (g_meas[i].active && my_strcmp(g_meas[i].name, name) == 0) return i;
    return -1;
}
static void measurement_list(void) {
    print_str("Measurements:\n");
    for (int i = 0; i < g_mc; i++) {
        if (!g_meas[i].active) continue;
        print_str("  ["); print_int(i); print_str("] ");
        print_str(g_meas[i].name); print_str(" ("); print_int(g_meas[i].pc); print_str(" pts)\n");
    }
}
static int measurement_delete(const char *name) {
    int id = measurement_find(name);
    if (id < 0) return -1;
    g_meas[id].active = 0; g_meas[id].pc = 0;
    return 0;
}

static int tag_set(point_t *pt, const char *key, const char *val) {
    if (pt->tc >= MAX_TAGS) return -1;
    int idx = pt->tc++;
    my_strncpy(pt->tags[idx].key, key, 24);
    my_strncpy(pt->tags[idx].val, val, 32);
    return idx;
}
static int field_add(point_t *pt, const char *key, long long val) {
    if (pt->fc >= MAX_FIELDS) return -1;
    int idx = pt->fc++;
    my_strncpy(pt->fields[idx].key, key, 24);
    pt->fields[idx].val = val;
    return idx;
}

static int wal_write(int mid, point_t *pt) {
    if (g_wal.ec >= MAX_WAL) return -1;
    if (g_wal.magic != WAL_MAGIC) { g_wal.magic = WAL_MAGIC; g_wal.ec = 0; g_wal.seq = 0; }
    int idx = g_wal.ec++;
    g_wal.entries[idx].ts = pt->ts; g_wal.entries[idx].mid = mid;
    return idx;
}
static int wal_replay(void) {
    if (g_wal.magic != WAL_MAGIC) return 0;
    int r = 0;
    for (int i = 0; i < g_wal.ec; i++) {
        int mid = g_wal.entries[i].mid;
        if (mid >= 0 && mid < g_mc && g_meas[mid].active) r++;
    }
    return r;
}
static int wal_checkpoint(void) { g_wal.seq++; g_wal.ec = 0; return g_wal.seq; }

static int point_write(int mid, point_t *pt) {
    if (mid < 0 || mid >= g_mc || g_meas[mid].pc >= MAX_PTS) return -1;
    int rc = wal_write(mid, pt);
    if (rc < 0) return rc;
    int idx = g_meas[mid].pc++;
    g_meas[mid].pts[idx] = *pt;
    if (pt->fc > 0 && g_mem.cnt < MAX_PTS) {
        g_mem.ts[g_mem.cnt] = pt->ts; g_mem.vals[g_mem.cnt] = pt->fields[0].val; g_mem.cnt++;
    }
    return idx;
}
static int point_batch_write(int mid, point_t *pts, int count) {
    int w = 0;
    for (int i = 0; i < count; i++) if (point_write(mid, &pts[i]) >= 0) w++;
    return w;
}

static int memtable_put(long long ts, long long val) {
    if (g_mem.cnt >= MAX_PTS) return -1;
    int idx = g_mem.cnt++; g_mem.ts[idx] = ts; g_mem.vals[idx] = val;
    return idx;
}
static long long memtable_get(long long ts) {
    for (int i = 0; i < g_mem.cnt; i++) if (g_mem.ts[i] == ts) return g_mem.vals[i];
    return 0;
}
static int memtable_flush(void) {
    if (g_mem.cnt == 0 || g_tc >= MAX_TSM) return -1;
    tsm_block_t *blk = &g_tsm[g_tc];
    blk->magic = TSM_MAGIC; blk->cnt = g_mem.cnt; blk->comp = 0;
    blk->min_ts = g_mem.ts[0]; blk->max_ts = g_mem.ts[0];
    for (int i = 0; i < g_mem.cnt; i++) {
        blk->ts[i] = g_mem.ts[i]; blk->vals[i] = g_mem.vals[i];
        if (g_mem.ts[i] < blk->min_ts) blk->min_ts = g_mem.ts[i];
        if (g_mem.ts[i] > blk->max_ts) blk->max_ts = g_mem.ts[i];
    }
    int bid = g_tc++; g_mem.cnt = 0;
    return bid;
}

static int tsm_encode(int mid, tsm_block_t *blk) {
    if (mid < 0 || mid >= g_mc) return -1;
    blk->magic = TSM_MAGIC; blk->cnt = g_meas[mid].pc; blk->comp = 0;
    blk->min_ts = 0; blk->max_ts = 0;
    for (int i = 0; i < g_meas[mid].pc; i++) {
        blk->ts[i] = g_meas[mid].pts[i].ts;
        blk->vals[i] = (g_meas[mid].pts[i].fc > 0) ? g_meas[mid].pts[i].fields[0].val : 0;
        if (i == 0) { blk->min_ts = blk->ts[i]; blk->max_ts = blk->ts[i]; }
        if (blk->ts[i] < blk->min_ts) blk->min_ts = blk->ts[i];
        if (blk->ts[i] > blk->max_ts) blk->max_ts = blk->ts[i];
    }
    return blk->cnt;
}
static int tsm_decode(tsm_block_t *blk, long long *ts_out, long long *val_out, int maxn) {
    if (blk->magic != TSM_MAGIC) return -1;
    int n = blk->cnt; if (n > maxn) n = maxn;
    for (int i = 0; i < n; i++) { ts_out[i] = blk->ts[i]; val_out[i] = blk->vals[i]; }
    return n;
}
static int tsm_compress(tsm_block_t *blk) {
    if (blk->magic != TSM_MAGIC || blk->comp) return -1;
    for (int i = blk->cnt - 1; i > 0; i--) blk->vals[i] -= blk->vals[i - 1];
    blk->comp = 1;
    return 0;
}
static int tsm_read(int block_id, long long *ts_out, long long *val_out, int maxn) {
    if (block_id < 0 || block_id >= g_tc) return -1;
    return tsm_decode(&g_tsm[block_id], ts_out, val_out, maxn);
}

static int query_time_range(int mid, long long t_start, long long t_end,
                            long long *ts_out, long long *val_out, int maxn) {
    if (mid < 0 || mid >= g_mc) return -1;
    int found = 0;
    for (int i = 0; i < g_meas[mid].pc && found < maxn; i++) {
        if (g_meas[mid].pts[i].ts >= t_start && g_meas[mid].pts[i].ts <= t_end) {
            ts_out[found] = g_meas[mid].pts[i].ts;
            val_out[found] = (g_meas[mid].pts[i].fc > 0) ? g_meas[mid].pts[i].fields[0].val : 0;
            found++;
        }
    }
    return found;
}
static int query_filter(int mid, const char *tag_key, const char *tag_val,
                        long long *ts_out, long long *val_out, int maxn) {
    if (mid < 0 || mid >= g_mc) return -1;
    int found = 0;
    for (int i = 0; i < g_meas[mid].pc && found < maxn; i++) {
        for (int t = 0; t < g_meas[mid].pts[i].tc; t++) {
            if (my_strcmp(g_meas[mid].pts[i].tags[t].key, tag_key) == 0 &&
                my_strcmp(g_meas[mid].pts[i].tags[t].val, tag_val) == 0) {
                ts_out[found] = g_meas[mid].pts[i].ts;
                val_out[found] = (g_meas[mid].pts[i].fc > 0) ? g_meas[mid].pts[i].fields[0].val : 0;
                found++; break;
            }
        }
    }
    return found;
}
static int query_window(int mid, long long t_start, long long t_end, int win_secs,
                        long long *ts_out, long long *val_out, int maxn) {
    long long win_ns = (long long)win_secs * NANOS;
    if (win_ns == 0) return -1;
    int found = 0;
    for (long long ws = t_start; ws <= t_end && found < maxn; ws += win_ns) {
        long long we = ws + win_ns - 1, sum = 0; int cnt = 0;
        for (int i = 0; i < g_meas[mid].pc; i++) {
            if (g_meas[mid].pts[i].ts >= ws && g_meas[mid].pts[i].ts <= we) {
                sum += (g_meas[mid].pts[i].fc > 0) ? g_meas[mid].pts[i].fields[0].val : 0; cnt++;
            }
        }
        if (cnt > 0) { ts_out[found] = ws; val_out[found] = sum / cnt; found++; }
    }
    return found;
}
static int query_fill_null(long long *ts, long long *val, int *count, int maxn) {
    (void)ts; (void)maxn;
    for (int i = 0; i < *count; i++) if (val[i] == 0 && i > 0 && ts[i] != ts[i-1]) val[i] = 0;
    return *count;
}
static int query_fill_prev(long long *ts, long long *val, int *count, int maxn) {
    (void)ts; (void)maxn;
    for (int i = 1; i < *count; i++) if (val[i] == 0 && val[i-1] != 0) val[i] = val[i-1];
    return *count;
}
static int query_fill_linear(long long *ts, long long *val, int *count, int maxn) {
    (void)ts; (void)maxn;
    for (int i = 1; i < *count - 1; i++)
        if (val[i] == 0 && val[i-1] != 0 && val[i+1] != 0) val[i] = (val[i-1] + val[i+1]) / 2;
    return *count;
}

static long long agg_min(long long *vals, int n) {
    long long m = vals[0]; for (int i = 1; i < n; i++) if (vals[i] < m) m = vals[i]; return m;
}
static long long agg_max(long long *vals, int n) {
    long long m = vals[0]; for (int i = 1; i < n; i++) if (vals[i] > m) m = vals[i]; return m;
}
static long long agg_mean(long long *vals, int n) {
    long long s = 0; for (int i = 0; i < n; i++) s += vals[i]; return s / n;
}
static long long agg_sum(long long *vals, int n) {
    long long s = 0; for (int i = 0; i < n; i++) s += vals[i]; return s;
}
static int agg_count(long long *vals, int n) { (void)vals; return n; }
static long long agg_median(long long *vals, int n) {
    long long sorted[MAX_PTS];
    for (int i = 0; i < n; i++) sorted[i] = vals[i];
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (sorted[j] < sorted[i]) { long long t = sorted[i]; sorted[i] = sorted[j]; sorted[j] = t; }
    return sorted[n / 2];
}
static long long agg_stddev(long long *vals, int n) {
    if (n <= 1) return 0;
    long long mean = agg_mean(vals, n), var_sum = 0;
    for (int i = 0; i < n; i++) { long long d = vals[i] - mean; var_sum += d * d; }
    long long var = var_sum / n, lo = 0, hi = var + 1;
    while (lo < hi) { long long mid = lo + (hi - lo) / 2; if (mid * mid >= var) hi = mid; else lo = mid + 1; }
    return lo;
}
static long long agg_percentile(long long *vals, int n, int pct) {
    long long sorted[MAX_PTS];
    for (int i = 0; i < n; i++) sorted[i] = vals[i];
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (sorted[j] < sorted[i]) { long long t = sorted[i]; sorted[i] = sorted[j]; sorted[j] = t; }
    int idx = (pct * (n - 1)) / 100;
    if (idx < 0) idx = 0; if (idx >= n) idx = n - 1;
    return sorted[idx];
}

static int cq_create(const char *name, int mid, int win, const char *agg) {
    if (g_cc >= MAX_CQS) return -1;
    int id = g_cc++;
    my_strncpy(g_cqs[id].name, name, MAX_NAME);
    g_cqs[id].mid = mid; g_cqs[id].win = win; g_cqs[id].active = 1;
    my_strncpy(g_cqs[id].agg, agg, 12);
    return id;
}
static int cq_downsample(int mid, int win_secs, long long *ts_out, long long *val_out, int maxn) {
    if (mid < 0 || mid >= g_mc || g_meas[mid].pc == 0) return 0;
    long long min_ts = g_meas[mid].pts[0].ts, max_ts = g_meas[mid].pts[0].ts;
    for (int i = 1; i < g_meas[mid].pc; i++) {
        if (g_meas[mid].pts[i].ts < min_ts) min_ts = g_meas[mid].pts[i].ts;
        if (g_meas[mid].pts[i].ts > max_ts) max_ts = g_meas[mid].pts[i].ts;
    }
    return query_window(mid, min_ts, max_ts, win_secs, ts_out, val_out, maxn);
}
static int cq_run(int cq_id) {
    if (cq_id < 0 || cq_id >= g_cc || !g_cqs[cq_id].active) return -1;
    long long ts_out[MAX_PTS], val_out[MAX_PTS];
    int n = cq_downsample(g_cqs[cq_id].mid, g_cqs[cq_id].win, ts_out, val_out, MAX_PTS);
    print_str("CQ "); print_str(g_cqs[cq_id].name); print_str(": "); print_int(n); print_str(" windows\n");
    return n;
}

static int retention_set(const char *name, int dur, int tier) {
    if (g_rc >= MAX_RET) return -1;
    int id = g_rc++;
    my_strncpy(g_ret[id].name, name, MAX_NAME);
    g_ret[id].dur = dur; g_ret[id].tier = tier; g_ret[id].active = 1;
    return id;
}
static int retention_expire(int mid, long long cutoff) {
    meas_t *m = &g_meas[mid]; int removed = 0, w = 0;
    for (int r = 0; r < m->pc; r++) {
        if (m->pts[r].ts < cutoff) removed++;
        else { if (w != r) m->pts[w] = m->pts[r]; w++; }
    }
    m->pc = w;
    return removed;
}
static int retention_enforce(int rp_id) {
    if (rp_id < 0 || rp_id >= g_rc || !g_ret[rp_id].active) return -1;
    long long cutoff = ts_now() - (long long)g_ret[rp_id].dur * NANOS;
    int expired = 0;
    for (int i = 0; i < g_mc; i++) if (g_meas[i].active) expired += retention_expire(i, cutoff);
    return expired;
}

static int tier_hot(const char *name, int max_age) {
    if (g_tic >= 3) return -1;
    int id = g_tic++; my_strncpy(g_tiers[id].name, name, 12);
    g_tiers[id].min_age = 0; g_tiers[id].max_age = max_age; g_tiers[id].pc = 0;
    return id;
}
static int tier_warm(const char *name, int min_age, int max_age) {
    if (g_tic >= 3) return -1;
    int id = g_tic++; my_strncpy(g_tiers[id].name, name, 12);
    g_tiers[id].min_age = min_age; g_tiers[id].max_age = max_age; g_tiers[id].pc = 0;
    return id;
}
static int tier_cold(const char *name, int min_age) {
    if (g_tic >= 3) return -1;
    int id = g_tic++; my_strncpy(g_tiers[id].name, name, 12);
    g_tiers[id].min_age = min_age; g_tiers[id].max_age = 999999999; g_tiers[id].pc = 0;
    return id;
}
static int tier_migrate(int from, int to, int mid) {
    if (from < 0 || from >= g_tic || to < 0 || to >= g_tic || mid < 0 || mid >= g_mc) return -1;
    int migrated = 0;
    long long now = ts_now(), to_min = (long long)g_tiers[to].min_age * NANOS, to_max = (long long)g_tiers[to].max_age * NANOS;
    for (int i = 0; i < g_meas[mid].pc; i++) {
        long long age = now - g_meas[mid].pts[i].ts;
        if (age >= to_min && age <= to_max) migrated++;
    }
    g_tiers[to].pc += migrated;
    if (g_tiers[from].pc >= migrated) g_tiers[from].pc -= migrated;
    return migrated;
}

static void show_help(void) {
    print_str("time_series_database v1.0\n");
    print_str("Usage: time_series_database [options]\n");
    print_str("  -h    Show this help\n  -t    Run test suite\n\n");
    print_str("Features:\n");
    print_str("  - Measurement/Tag/Field/Timestamp data model\n");
    print_str("  - WAL, memtable flush, TSM compressed storage\n");
    print_str("  - Time-range queries with window functions\n");
    print_str("  - Aggregations: min/max/mean/sum/count/median/stddev/percentile\n");
    print_str("  - Continuous queries for downsampling\n");
    print_str("  - Retention policies with tiered storage\n");
}

static void test_suite(void) {
    int passed = 0, total = 0;
    print_str("\n=== Time Series Database Test Suite ===\n");

    print_str("\n--- measurement_create ---\n");
    total++; int cpu_id = measurement_create("cpu_usage"); int mem_id = measurement_create("mem_usage");
    int ok = (cpu_id == 0 && mem_id == 1); if (ok) passed++; print_test("create measurements", ok);
    total++; ok = (measurement_find("cpu_usage") == 0 && measurement_find("mem_usage") == 1);
    if (ok) passed++; print_test("find measurements", ok);

    print_str("\n--- tag_set and field_add ---\n");
    point_t pt; pt.ts = ts_now(); pt.tc = 0; pt.fc = 0;
    total++; ok = (tag_set(&pt, "host", "server01") >= 0 && tag_set(&pt, "region", "us-east") >= 0);
    if (ok) passed++; print_test("set tags", ok);
    total++; ok = (field_add(&pt, "value", 42) >= 0 && field_add(&pt, "load", 85) >= 0);
    if (ok) passed++; print_test("add fields", ok);

    print_str("\n--- point_write ---\n");
    total++; ok = (point_write(cpu_id, &pt) >= 0); if (ok) passed++; print_test("write single point", ok);

    print_str("\n--- point_batch_write ---\n");
    point_t batch[5];
    for (int i = 0; i < 5; i++) {
        batch[i].ts = ts_now() + (long long)(i + 1) * NANOS; batch[i].tc = 0; batch[i].fc = 0;
        tag_set(&batch[i], "host", "server01"); field_add(&batch[i], "value", 50 + (long long)i * 10);
    }
    total++; int written = point_batch_write(cpu_id, batch, 5);
    ok = (written == 5); if (ok) passed++; print_test("batch write 5 points", ok);

    print_str("\n--- wal_write and wal_replay ---\n");
    total++; ok = (g_wal.ec > 0); if (ok) passed++; print_test("wal has entries", ok);
    total++; int replayed = wal_replay(); ok = (replayed > 0);
    if (ok) passed++; print_test("wal replay", ok);
    total++; int seq = wal_checkpoint(); ok = (seq == 1 && g_wal.ec == 0);
    if (ok) passed++; print_test("wal checkpoint", ok);

    print_str("\n--- memtable and flush ---\n");
    total++; ok = (g_mem.cnt > 0); if (ok) passed++; print_test("memtable has data", ok);
    total++; int bid = memtable_flush(); ok = (bid >= 0 && g_mem.cnt == 0);
    if (ok) passed++; print_test("memtable flush", ok);

    print_str("\n--- tsm_compress ---\n");
    total++; ok = (g_tc > 0); if (ok) passed++; print_test("tsm block exists", ok);
    total++; ok = (tsm_compress(&g_tsm[0]) == 0 && g_tsm[0].comp == 1);
    if (ok) passed++; print_test("tsm compress delta", ok);

    print_str("\n--- query_time_range ---\n");
    long long qts[MAX_PTS], qval[MAX_PTS]; long long base = ts_now();
    total++; int qn = query_time_range(cpu_id, base, base + (long long)3 * NANOS, qts, qval, MAX_PTS);
    ok = (qn > 0); if (ok) passed++; print_test("time range query returns results", ok);

    print_str("\n--- aggregations ---\n");
    long long aggv[6] = {10, 20, 30, 40, 50, 60};
    total++; ok = (agg_min(aggv, 6) == 10); if (ok) passed++; print_test("agg_min = 10", ok);
    total++; ok = (agg_max(aggv, 6) == 60); if (ok) passed++; print_test("agg_max = 60", ok);
    total++; ok = (agg_mean(aggv, 6) == 35); if (ok) passed++; print_test("agg_mean = 35", ok);
    total++; ok = (agg_sum(aggv, 6) == 210); if (ok) passed++; print_test("agg_sum = 210", ok);
    total++; ok = (agg_count(aggv, 6) == 6); if (ok) passed++; print_test("agg_count = 6", ok);
    total++; long long med = agg_median(aggv, 6); ok = (med == 30); if (ok) passed++; print_test("agg_median = 30", ok);
    total++; long long p90 = agg_percentile(aggv, 6, 90); ok = (p90 == 60);
    if (ok) passed++; print_test("agg_percentile(90) = 60", ok);
    total++; long long sd = agg_stddev(aggv, 6); ok = (sd > 0);
    if (ok) passed++; print_test("agg_stddev > 0", ok);

    print_str("\n--- cq_downsample ---\n");
    total++; int cq_id = cq_create("cq_5s", cpu_id, 5, "mean");
    ok = (cq_id >= 0); if (ok) passed++; print_test("cq_create", ok);
    total++; int cqn = cq_run(cq_id); ok = (cqn >= 0);
    if (ok) passed++; print_test("cq_run", ok);

    print_str("\n--- retention_enforce ---\n");
    total++; int rp_id = retention_set("rp_7d", 604800, 0);
    ok = (rp_id >= 0); if (ok) passed++; print_test("retention_set", ok);
    total++; int exp = retention_enforce(rp_id); ok = (exp >= 0);
    if (ok) passed++; print_test("retention_enforce", ok);

    print_str("\n--- tiered_storage ---\n");
    total++; int hot_id = tier_hot("hot", 3600); int warm_id = tier_warm("warm", 3600, 86400);
    int cold_id = tier_cold("cold", 86400);
    ok = (hot_id >= 0 && warm_id >= 0 && cold_id >= 0);
    if (ok) passed++; print_test("create tiers", ok);
    total++; int mig = tier_migrate(hot_id, warm_id, cpu_id); ok = (mig >= 0);
    if (ok) passed++; print_test("tier_migrate hot->warm", ok);

    print_str("\n--- measurement_list and delete ---\n");
    measurement_list();
    total++; ok = (measurement_delete("mem_usage") == 0 && measurement_find("mem_usage") < 0);
    if (ok) passed++; print_test("delete measurement", ok);

    print_str("\n--- fill_strategies ---\n");
    long long fts[5] = {1, 2, 3, 4, 5}; long long fval[5] = {10, 0, 30, 0, 50}; int fcnt = 5;
    total++; query_fill_prev(fts, fval, &fcnt, 5); ok = (fval[1] == 10);
    if (ok) passed++; print_test("fill_prev", ok);
    long long fval2[5] = {10, 0, 30, 0, 50};
    total++; query_fill_linear(fts, fval2, &fcnt, 5); ok = (fval2[1] == 20 && fval2[3] == 40);
    if (ok) passed++; print_test("fill_linear", ok);
    long long fval3[5] = {10, 0, 30, 0, 50};
    total++; query_fill_null(fts, fval3, &fcnt, 5); ok = (fval3[1] == 0);
    if (ok) passed++; print_test("fill_null", ok);

    print_str("\n--- query_filter ---\n");
    total++; long long fts2[MAX_PTS], fval4[MAX_PTS];
    int fn = query_filter(cpu_id, "host", "server01", fts2, fval4, MAX_PTS);
    ok = (fn > 0); if (ok) passed++; print_test("query_filter by tag", ok);

    print_str("\n--- memtable and tsm extras ---\n");
    total++; int mp = memtable_put(ts_now() + (long long)99 * NANOS, 999);
    ok = (mp >= 0); if (ok) passed++; print_test("memtable_put", ok);
    total++; long long mv = memtable_get(ts_now() + (long long)99 * NANOS);
    ok = (mv == 999); if (ok) passed++; print_test("memtable_get", ok);
    total++; tsm_block_t enc_blk; int en = tsm_encode(cpu_id, &enc_blk);
    ok = (en > 0); if (ok) passed++; print_test("tsm_encode", ok);
    total++; long long rts[MAX_PTS], rval[MAX_PTS];
    if (g_tc > 0) { int rn = tsm_read(0, rts, rval, MAX_PTS); ok = (rn >= 0); }
    else ok = 1;
    if (ok) passed++; print_test("tsm_read", ok);
    total++; ok = (my_strlen("hello") == 5); if (ok) passed++; print_test("my_strlen", ok);

    print_str("\ntime_series_database: "); print_int(passed); print_str("/");
    print_int(total); print_str(" passed\n");
}

__attribute__((export_name("_start"), used))
void _start(void) {
    char argv_buf[256];
    int argc = host_get_argv(0, 256);
    if (argc > 0) host_get_argv((unsigned int)(unsigned long long)(char *)argv_buf, 256);

    if (argc >= 2 && argv_buf[0] == '-' && argv_buf[1] == 'h') { show_help(); host_exit(0); }
    if (argc >= 2 && argv_buf[0] == '-' && argv_buf[1] == 't') { test_suite(); host_exit(0); }

    print_str("time_series_database v1.0\n");
    print_str("Usage: -h help, -t test\n");
    host_exit(0);
}
