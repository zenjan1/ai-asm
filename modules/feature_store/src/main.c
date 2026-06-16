/* Feature Store Platform - AI-ASM OS (AArch64 WASM-native)
 * Feature registration, offline/online stores, transformations,
 * data quality, and team sharing. Pure C, no stdlib. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_FG 64
#define MAX_COL 32
#define MAX_ROW 256
#define MAX_ENT 128
#define MAX_PIPE 16
#define MAX_SH 32
#define MAX_AUD 128
#define MAX_PTN 16
#define MAX_LIN 32
#define NLEN 32
#define PLEN 128
#define ARENA_SZ 65536
#define T_I32 0
#define T_I64 1
#define T_FLT 2
#define T_STR 3
#define T_BOL 4
#define FMT_PARQUET 0
#define FMT_AVRO 1
#define P_OP_IDENTITY 0
#define P_OP_LOG 1
#define P_OP_SCALE 2
#define P_OP_BUCKET 3
#define P_OP_EMBED 4
#define P_OP_HASH 5
#define PERM_READ 1
#define PERM_WRITE 2
#define PERM_ADMIN 4
#define S_ACTIVE 0
#define S_EXPIRED 1
#define S_DELETED 2

typedef struct { char name[NLEN]; int type; int nullable; int description; } schema_col_t;
typedef struct { schema_col_t cols[MAX_COL]; int col_count; int version; char lineage[MAX_LIN][NLEN]; int lin_count; } schema_t;
typedef struct { char name[NLEN]; char desc[PLEN]; schema_t schema; int fg_id; int status; int created_ts; } feature_group_t;
typedef struct { int row_id; int values[MAX_COL]; int timestamps[MAX_COL]; int null_mask; } feature_row_t;
typedef struct { char path[PLEN]; int format; feature_row_t rows[MAX_ROW]; int row_count; int part_key; } partition_t;
typedef struct { char key[NLEN]; int values[MAX_COL]; int expire_ts; int status; int version; } online_entry_t;
typedef struct { char name[NLEN]; int op_type; int param; int input_col; int output_col; } transform_op_t;
typedef struct { transform_op_t ops[MAX_PIPE]; int op_count; char name[NLEN]; int consistent; } pipeline_t;
typedef struct { int missing_count; int total_count; double missing_rate; double psi_value; double ks_value; int anomaly_count; } quality_result_t;
typedef struct { char fg_name[NLEN]; char entity[NLEN]; int perm_mask; int granted_ts; int granted_by; } share_t;
typedef struct { int ts; int action; char actor[NLEN]; char target[NLEN]; int detail; } audit_entry_t;
typedef struct { double bins[8]; int counts[8]; int total; double mean; double stddev; } distribution_t;

static int g_next_fg = 1, g_ts = 1000;
static unsigned int g_rng = 2463534242u;

static unsigned int my_strlen(const char *s) { unsigned int n = 0; if (!s) return 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, int mx) {
    int i = 0; if (!d || !s || mx <= 0) return;
    while (i < mx - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int my_memcmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) if ((unsigned char)a[i] != (unsigned char)b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
    return 0;
}
static void print_str(const char *s) { if (s) host_print(s); }
static void print_int(int val) {
    char buf[16]; int pos = 15, neg = val < 0;
    unsigned int u = neg ? (unsigned int)(-val) : (unsigned int)val;
    buf[pos] = '\0';
    if (u == 0) buf[--pos] = '0';
    else while (u > 0 && pos > 0) { buf[--pos] = '0' + (char)(u % 10); u /= 10; }
    if (neg && pos > 0) buf[--pos] = '-';
    host_print(&buf[pos]);
}
static void print_double(double val) {
    int w = (int)val; double f = val - (double)w;
    if (val < 0.0 && w == 0) { print_str("-"); f = -f; }
    print_int(w); print_str(".");
    for (int i = 0; i < 4; i++) { f *= 10.0; int d = (int)f; char s[2] = { (char)('0'+d), '\0' }; host_print(s); f -= (double)d; }
}
static unsigned int rng_next(void) { g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return g_rng; }

static void *bump_alloc(int size) {
    static unsigned int arena[ARENA_SZ/4]; static int off = 0;
    int w = (size + 3) / 4; if (off + w > ARENA_SZ/4) return NULL;
    void *p = &arena[off]; off += w; return p;
}
static double my_log(double x) {
    if (x <= 0.0) return -999999.0; double r = 0.0;
    while (x >= 2.0) { x /= 2.0; r += 0.693147180; }
    while (x < 0.5)  { x *= 2.0; r -= 0.693147180; }
    double t = x - 1.0; r += t * (1.0 - t * (0.5 - t * (0.333333333 - t * 0.25))); return r;
}
static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0; double g = x * 0.5;
    for (int i = 0; i < 20; i++) { g = (g + x / g) * 0.5; } return g;
}

/* ── 1. Feature Registration ──────────────────────────────────────────────── */

static feature_group_t *feature_group_create(const char *name, const char *desc) {
    feature_group_t *fg = (feature_group_t *)bump_alloc(sizeof(feature_group_t));
    if (!fg) return NULL;
    my_strncpy(fg->name, name, NLEN); my_strncpy(fg->desc, desc, PLEN);
    fg->fg_id = g_next_fg++; fg->status = S_ACTIVE; fg->created_ts = g_ts;
    fg->schema.col_count = 0; fg->schema.version = 1; fg->schema.lin_count = 0;
    return fg;
}
static int feature_group_register(feature_group_t *fg) {
    if (!fg || fg->fg_id <= 0) return -1; if (fg->schema.col_count == 0) return -2;
    g_ts++; fg->created_ts = g_ts; return fg->fg_id;
}
static int feature_group_list(feature_group_t *groups[], int count) {
    int found = 0; for (int i = 0; i < count; i++) { if (groups[i] && groups[i]->status == S_ACTIVE) found++; } return found;
}
static int schema_define(schema_t *sch, schema_col_t cols[], int col_count) {
    if (!sch || col_count > MAX_COL) return -1;
    sch->col_count = col_count; sch->version = 1; sch->lin_count = 0;
    for (int i = 0; i < col_count; i++) {
        my_strncpy(sch->cols[i].name, cols[i].name, NLEN);
        sch->cols[i].type = cols[i].type; sch->cols[i].nullable = cols[i].nullable; sch->cols[i].description = cols[i].description;
    }
    return sch->version;
}
static int schema_version(schema_t *sch) { if (!sch) return -1; return sch->version; }
static int schema_evolve(schema_t *sch, schema_col_t new_col) {
    if (!sch || sch->col_count >= MAX_COL) return -1;
    for (int i = 0; i < sch->col_count; i++) { if (my_memcmp(sch->cols[i].name, new_col.name, NLEN) == 0) return -2; }
    my_strncpy(sch->cols[sch->col_count].name, new_col.name, NLEN);
    sch->cols[sch->col_count].type = new_col.type; sch->cols[sch->col_count].nullable = new_col.nullable;
    sch->cols[sch->col_count].description = new_col.description; sch->col_count++; sch->version++; return sch->version;
}
static int schema_lineage(schema_t *sch, const char *origin) {
    if (!sch || sch->lin_count >= MAX_LIN) return -1;
    my_strncpy(sch->lineage[sch->lin_count], origin, NLEN); sch->lin_count++; return sch->lin_count;
}

/* ── 2. Offline Store ─────────────────────────────────────────────────────── */

static int offline_batch_compute(feature_row_t rows[], int row_count, int col_idx) {
    int sum = 0, cnt = 0;
    for (int i = 0; i < row_count; i++) { if (!(rows[i].null_mask & (1 << col_idx))) { sum += rows[i].values[col_idx]; cnt++; } }
    return cnt > 0 ? sum / cnt : 0;
}
static int offline_write_parquet(partition_t *ptn, const char *path) {
    if (!ptn) return -1; ptn->format = FMT_PARQUET; my_strncpy(ptn->path, path, PLEN); g_ts++; return ptn->row_count;
}
static int offline_write_avro(partition_t *ptn, const char *path) {
    if (!ptn) return -1; ptn->format = FMT_AVRO; my_strncpy(ptn->path, path, PLEN); g_ts++; return ptn->row_count;
}
static partition_t *offline_partition(int row_count, int part_count) {
    if (part_count > MAX_PTN || part_count <= 0) return NULL;
    partition_t *parts = (partition_t *)bump_alloc(sizeof(partition_t) * part_count); if (!parts) return NULL;
    int rows_per = row_count / part_count;
    for (int i = 0; i < part_count; i++) {
        parts[i].row_count = (i == part_count - 1) ? row_count - rows_per * (part_count - 1) : rows_per;
        parts[i].part_key = i; parts[i].format = FMT_PARQUET;
        for (int j = 0; j < parts[i].row_count; j++) parts[i].rows[j].row_id = i * rows_per + j;
    }
    return parts;
}
static int offline_point_in_time_query(partition_t *ptn, int target_ts, int col_idx) {
    if (!ptn || ptn->row_count == 0) return -1;
    int best = 0, best_diff = 0x7FFFFFFF;
    for (int i = 0; i < ptn->row_count; i++) {
        int diff = target_ts - ptn->rows[i].timestamps[col_idx];
        if (diff >= 0 && diff < best_diff) { best_diff = diff; best = ptn->rows[i].values[col_idx]; }
    }
    return best;
}

/* ── 3. Online Store ──────────────────────────────────────────────────────── */

static online_entry_t *online_cache = NULL;
static int online_cache_count = 0;
static void online_init(void) {
    online_cache = (online_entry_t *)bump_alloc(sizeof(online_entry_t) * MAX_ENT); online_cache_count = 0;
}
static int online_set(const char *key, int values[], int col_count, int ttl) {
    if (!online_cache || !key || online_cache_count >= MAX_ENT) return -1;
    online_entry_t *e = &online_cache[online_cache_count];
    my_strncpy(e->key, key, NLEN);
    for (int i = 0; i < col_count && i < MAX_COL; i++) e->values[i] = values[i];
    e->expire_ts = g_ts + ttl; e->status = S_ACTIVE; e->version = 1; online_cache_count++; g_ts++; return online_cache_count - 1;
}
static int online_get(const char *key, int out_values[], int max_col) {
    if (!online_cache || !key) return -1;
    for (int i = online_cache_count - 1; i >= 0; i--) {
        if (my_memcmp(online_cache[i].key, key, NLEN) == 0) {
            if (g_ts > online_cache[i].expire_ts) { online_cache[i].status = S_EXPIRED; return -2; }
            for (int j = 0; j < max_col && j < MAX_COL; j++) out_values[j] = online_cache[i].values[j]; return i;
        }
    }
    return -1;
}
static int online_update(const char *key, int col_idx, int new_val) {
    if (!online_cache || !key) return -1;
    for (int i = online_cache_count - 1; i >= 0; i--) {
        if (my_memcmp(online_cache[i].key, key, NLEN) == 0 && online_cache[i].status == S_ACTIVE) {
            if (g_ts > online_cache[i].expire_ts) { online_cache[i].status = S_EXPIRED; return -2; }
            online_cache[i].values[col_idx] = new_val; online_cache[i].version++; g_ts++; return online_cache[i].version;
        }
    }
    return -1;
}
static int online_delete(const char *key) {
    if (!online_cache || !key) return -1;
    for (int i = 0; i < online_cache_count; i++) {
        if (my_memcmp(online_cache[i].key, key, NLEN) == 0) { online_cache[i].status = S_DELETED; g_ts++; return 0; }
    }
    return -1;
}
static int online_ttl(const char *key) {
    if (!online_cache || !key) return -1;
    for (int i = online_cache_count - 1; i >= 0; i--) {
        if (my_memcmp(online_cache[i].key, key, NLEN) == 0) {
            int remain = online_cache[i].expire_ts - g_ts; return remain > 0 ? remain : 0;
        }
    }
    return -1;
}

/* ── 4. Feature Transformation ────────────────────────────────────────────── */

static pipeline_t *pipelines[MAX_PIPE];
static int pipeline_count = 0;
static int transform_apply_op(int value, int op_type, int param) {
    switch (op_type) {
        case P_OP_IDENTITY: return value; case P_OP_LOG: return value > 0 ? value : 0;
        case P_OP_SCALE: return value * param; case P_OP_BUCKET: return value / (param > 0 ? param : 1);
        case P_OP_EMBED: return (value * 7 + param) % 10000; case P_OP_HASH: return (value ^ param) & 0x7FFFFFFF;
        default: return value;
    }
}
static int transform_register(const char *name, transform_op_t ops[], int op_count) {
    if (!name || op_count > MAX_PIPE || pipeline_count >= MAX_PIPE) return -1;
    pipeline_t *p = (pipeline_t *)bump_alloc(sizeof(pipeline_t)); if (!p) return -1;
    my_strncpy(p->name, name, NLEN); p->op_count = op_count; p->consistent = 1;
    for (int i = 0; i < op_count; i++) {
        p->ops[i].op_type = ops[i].op_type; p->ops[i].param = ops[i].param;
        p->ops[i].input_col = ops[i].input_col; p->ops[i].output_col = ops[i].output_col;
    }
    pipelines[pipeline_count] = p; pipeline_count++; return pipeline_count - 1;
}
static int transform_apply_online(const char *pipe_name, int input_val) {
    for (int i = 0; i < pipeline_count; i++) {
        if (my_memcmp(pipelines[i]->name, pipe_name, NLEN) == 0) {
            int v = input_val;
            for (int j = 0; j < pipelines[i]->op_count; j++) v = transform_apply_op(v, pipelines[i]->ops[j].op_type, pipelines[i]->ops[j].param);
            return v;
        }
    }
    return input_val;
}
static int transform_apply_offline(const char *pipe_name, int rows[], int row_count) {
    for (int i = 0; i < pipeline_count; i++) {
        if (my_memcmp(pipelines[i]->name, pipe_name, NLEN) == 0) {
            int result = 0;
            for (int r = 0; r < row_count; r++) {
                int v = rows[r];
                for (int j = 0; j < pipelines[i]->op_count; j++) v = transform_apply_op(v, pipelines[i]->ops[j].op_type, pipelines[i]->ops[j].param);
                result += v;
            }
            return row_count > 0 ? result / row_count : 0;
        }
    }
    return 0;
}
static int transform_pipeline_build(const char *name, int ops_spec[], int spec_count) {
    if (!name || spec_count > MAX_PIPE || pipeline_count >= MAX_PIPE) return -1;
    pipeline_t *p = (pipeline_t *)bump_alloc(sizeof(pipeline_t)); if (!p) return -1;
    my_strncpy(p->name, name, NLEN); p->op_count = spec_count / 2; p->consistent = 1;
    for (int i = 0; i < p->op_count && i < MAX_PIPE; i++) {
        p->ops[i].op_type = ops_spec[i * 2]; p->ops[i].param = ops_spec[i * 2 + 1];
        p->ops[i].input_col = i; p->ops[i].output_col = i;
    }
    pipelines[pipeline_count] = p; pipeline_count++; return pipeline_count - 1;
}

/* ── 5. Data Quality ──────────────────────────────────────────────────────── */

static quality_result_t quality_check_missing(feature_row_t rows[], int row_count, int col_idx) {
    quality_result_t r; r.total_count = row_count; r.missing_count = 0; r.anomaly_count = 0; r.psi_value = 0.0; r.ks_value = 0.0;
    for (int i = 0; i < row_count; i++) { if (rows[i].null_mask & (1 << col_idx)) r.missing_count++; }
    r.missing_rate = row_count > 0 ? (double)r.missing_count / (double)row_count : 0.0; return r;
}
static double quality_check_psi(distribution_t *expected, distribution_t *actual) {
    if (!expected || !actual) return 0.0; double psi = 0.0;
    for (int i = 0; i < 8; i++) {
        double ep = expected->total > 0 ? (double)expected->counts[i] / (double)expected->total : 0.001;
        double ap = actual->total > 0 ? (double)actual->counts[i] / (double)actual->total : 0.001;
        if (ep < 0.001) ep = 0.001; if (ap < 0.001) ap = 0.001; psi += (ap - ep) * my_log(ap / ep);
    }
    return psi;
}
static double quality_check_ks(distribution_t *d1, distribution_t *d2) {
    if (!d1 || !d2) return 0.0; double max_diff = 0.0, cum1 = 0.0, cum2 = 0.0;
    for (int i = 0; i < 8; i++) {
        cum1 += d1->total > 0 ? (double)d1->counts[i] / (double)d1->total : 0.0;
        cum2 += d2->total > 0 ? (double)d2->counts[i] / (double)d2->total : 0.0;
        double diff = cum1 - cum2; if (diff < 0) diff = -diff; if (diff > max_diff) max_diff = diff;
    }
    return max_diff;
}
static int quality_check_anomaly(feature_row_t rows[], int row_count, int col_idx, double threshold) {
    if (row_count == 0) return 0; long long sum = 0;
    for (int i = 0; i < row_count; i++) { if (!(rows[i].null_mask & (1 << col_idx))) sum += rows[i].values[col_idx]; }
    double mean = (double)sum / (double)row_count, var_sum = 0.0;
    for (int i = 0; i < row_count; i++) {
        if (!(rows[i].null_mask & (1 << col_idx))) { double diff = (double)rows[i].values[col_idx] - mean; var_sum += diff * diff; }
    }
    double stddev = (row_count > 1 && var_sum > 0.0) ? my_sqrt(var_sum / (double)(row_count - 1)) : 1.0;
    int anomalies = 0;
    for (int i = 0; i < row_count; i++) {
        if (rows[i].null_mask & (1 << col_idx)) continue;
        double z = ((double)rows[i].values[col_idx] - mean) / stddev; if (z < 0) z = -z; if (z > threshold) anomalies++;
    }
    return anomalies;
}
static int quality_validate(feature_group_t *fg, feature_row_t rows[], int row_count) {
    if (!fg || !rows) return -1; int errors = 0;
    for (int i = 0; i < row_count; i++) { for (int c = 0; c < fg->schema.col_count; c++) {
        if (!fg->schema.cols[c].nullable && (rows[i].null_mask & (1 << c))) errors++;
    } }
    return errors;
}
static quality_result_t quality_report(feature_row_t rows[], int row_count, int col_idx) {
    quality_result_t r = quality_check_missing(rows, row_count, col_idx);
    r.anomaly_count = quality_check_anomaly(rows, row_count, col_idx, 2.0); return r;
}

/* ── 6. Sharing & Collaboration ───────────────────────────────────────────── */

static share_t shares[MAX_SH]; static int share_count = 0;
static audit_entry_t audit_log[MAX_AUD]; static int audit_count = 0;
static void audit_append(int action, const char *entity, const char *target, int detail) {
    if (audit_count >= MAX_AUD) return;
    audit_log[audit_count].ts = g_ts; audit_log[audit_count].action = action; audit_log[audit_count].detail = detail;
    my_strncpy(audit_log[audit_count].actor, entity, NLEN); my_strncpy(audit_log[audit_count].target, target, NLEN); audit_count++;
}
static int share_create(const char *fg_name, const char *entity, int perm, int actor) {
    if (share_count >= MAX_SH || !fg_name || !entity) return -1;
    my_strncpy(shares[share_count].fg_name, fg_name, NLEN); my_strncpy(shares[share_count].entity, entity, NLEN);
    shares[share_count].perm_mask = perm; shares[share_count].granted_ts = g_ts; shares[share_count].granted_by = actor;
    g_ts++; audit_append(1, entity, fg_name, perm); share_count++; return share_count - 1;
}
static int share_marketplace_list(share_t out[], int max_out) {
    int found = 0;
    for (int i = 0; i < share_count && found < max_out; i++) { if (shares[i].perm_mask & PERM_READ) { out[found] = shares[i]; found++; } }
    return found;
}
static int share_permission_set(const char *fg_name, const char *entity, int new_perm) {
    for (int i = 0; i < share_count; i++) {
        if (my_memcmp(shares[i].fg_name, fg_name, NLEN) == 0 && my_memcmp(shares[i].entity, entity, NLEN) == 0) {
            int old = shares[i].perm_mask; shares[i].perm_mask = new_perm; g_ts++;
            audit_append(2, entity, fg_name, new_perm); return old;
        }
    }
    return -1;
}
static int share_audit_log(audit_entry_t out[], int max_out) {
    int count = audit_count < max_out ? audit_count : max_out;
    for (int i = 0; i < count; i++) out[i] = audit_log[i]; return count;
}

/* ── 7. Tests ─────────────────────────────────────────────────────────────── */

static void test_suite(void) {
    print_str("=== Feature Store Comprehensive Demo ===\n\n");
    print_str("[1] Feature Group Registration\n");
    feature_group_t *fg = feature_group_create("user_features", "User profile features");
    print_str("  Created fg_id="); print_int(fg->fg_id); print_str("\n");
    schema_col_t cols[3];
    my_strncpy(cols[0].name, "user_id", NLEN); cols[0].type = T_I32; cols[0].nullable = 0;
    my_strncpy(cols[1].name, "age", NLEN);     cols[1].type = T_I32; cols[1].nullable = 0;
    my_strncpy(cols[2].name, "score", NLEN);   cols[2].type = T_FLT; cols[2].nullable = 1;
    int sv = schema_define(&fg->schema, cols, 3);
    print_str("  Schema v"); print_int(sv); print_str(" cols="); print_int(fg->schema.col_count); print_str("\n");
    int rc = feature_group_register(fg);
    print_str("  Registered rc="); print_int(rc); print_str("\n");
    feature_group_t *list[4] = { fg, NULL, NULL, NULL };
    print_str("  Active groups: "); print_int(feature_group_list(list, 4)); print_str("\n\n");

    print_str("[2] Schema Versioning & Lineage\n");
    print_str("  Current version: "); print_int(schema_version(&fg->schema)); print_str("\n");
    schema_col_t new_col; my_strncpy(new_col.name, "email", NLEN); new_col.type = T_STR; new_col.nullable = 1;
    int ev = schema_evolve(&fg->schema, new_col);
    print_str("  After evolve: v"); print_int(ev); print_str(" cols="); print_int(fg->schema.col_count); print_str("\n");
    print_str("  Lineage entries: "); print_int(schema_lineage(&fg->schema, "raw_user_table")); print_str("\n\n");

    print_str("[3] Offline Batch + Point-in-Time Query\n");
    partition_t ptn; ptn.row_count = 4; ptn.part_key = 0;
    for (int i = 0; i < 4; i++) {
        ptn.rows[i].row_id = i; ptn.rows[i].values[0] = (i + 1) * 100; ptn.rows[i].values[1] = 20 + i * 5;
        ptn.rows[i].timestamps[0] = 1000 + i * 10; ptn.rows[i].timestamps[1] = 1000 + i * 10; ptn.rows[i].null_mask = 0;
    }
    print_str("  Batch avg col0: "); print_int(offline_batch_compute(ptn.rows, 4, 0)); print_str("\n");
    print_str("  Parquet written: "); print_int(offline_write_parquet(&ptn, "/offline/user/p=0")); print_str(" rows\n");
    partition_t ptn2; ptn2.row_count = 2; ptn2.part_key = 1;
    print_str("  Avro written: "); print_int(offline_write_avro(&ptn2, "/offline/user/p=1.avro")); print_str(" rows\n");
    print_str("  PIT query ts=1015 col0: "); print_int(offline_point_in_time_query(&ptn, 1015, 0)); print_str("\n");
    partition_t *parts = offline_partition(10, 3);
    print_str("  Partitioned 10/3: p0="); print_int(parts[0].row_count);
    print_str(" p1="); print_int(parts[1].row_count); print_str(" p2="); print_int(parts[2].row_count); print_str("\n\n");

    print_str("[4] Online Cache Get/Set\n");
    online_init(); int vals[3] = { 42, 25, 9999 };
    print_str("  Set user:1001 idx="); print_int(online_set("user:1001", vals, 3, 500)); print_str("\n");
    int out[3] = { 0, 0, 0 }; int gi = online_get("user:1001", out, 3);
    print_str("  Get idx="); print_int(gi); print_str(" vals=["); print_int(out[0]); print_str(","); print_int(out[1]); print_str(","); print_int(out[2]); print_str("]\n");
    print_str("  Update col2 -> ver="); print_int(online_update("user:1001", 2, 12345)); print_str("\n");
    print_str("  TTL remaining: "); print_int(online_ttl("user:1001")); print_str("\n");
    print_str("  Delete rc="); print_int(online_delete("user:1001")); print_str("\n");
    print_str("  Get after delete: "); print_int(online_get("user:1001", out, 3)); print_str("\n\n");

    print_str("[5] Transform Pipeline (Online=Offline Consistency)\n");
    transform_op_t ops[2]; ops[0].op_type = P_OP_SCALE; ops[0].param = 10; ops[1].op_type = P_OP_BUCKET; ops[1].param = 5;
    int pi = transform_register("scale_bucket", ops, 2);
    print_str("  Registered pipeline idx="); print_int(pi); print_str("\n");
    print_str("  Online apply(7): "); print_int(transform_apply_online("scale_bucket", 7)); print_str("\n");
    int batch[4] = { 3, 5, 7, 9 };
    print_str("  Offline avg apply: "); print_int(transform_apply_offline("scale_bucket", batch, 4)); print_str("\n");
    int spec[4] = { P_OP_LOG, 0, P_OP_SCALE, 2 };
    print_str("  Built pipeline idx="); print_int(transform_pipeline_build("log_scale", spec, 4)); print_str("\n\n");

    print_str("[6] Data Quality (Missing/PSI/Anomaly)\n");
    feature_row_t qrows[8];
    for (int i = 0; i < 8; i++) {
        qrows[i].row_id = i; qrows[i].values[0] = 50 + i * 10; qrows[i].null_mask = (i == 2 || i == 5) ? 1 : 0; qrows[i].timestamps[0] = 1000 + i;
    }
    qrows[6].values[0] = 9999;
    quality_result_t qr = quality_check_missing(qrows, 8, 0);
    print_str("  Missing: "); print_int(qr.missing_count); print_str("/"); print_int(qr.total_count);
    print_str(" rate="); print_double(qr.missing_rate); print_str("\n");
    distribution_t d1, d2; d1.total = 80; d2.total = 80;
    for (int i = 0; i < 8; i++) { d1.counts[i] = 10; d2.counts[i] = (i < 4) ? 15 : 5; }
    print_str("  PSI: "); print_double(quality_check_psi(&d1, &d2)); print_str("\n");
    print_str("  KS stat: "); print_double(quality_check_ks(&d1, &d2)); print_str("\n");
    print_str("  Anomalies (z>2): "); print_int(quality_check_anomaly(qrows, 8, 0, 2.0)); print_str("\n");
    print_str("  Validation errors: "); print_int(quality_validate(fg, qrows, 8)); print_str("\n");
    quality_result_t qrep = quality_report(qrows, 8, 0);
    print_str("  Report missing="); print_int(qrep.missing_count); print_str(" anomaly="); print_int(qrep.anomaly_count); print_str("\n\n");

    print_str("[7] Sharing & Permissions\n");
    print_str("  Share idx="); print_int(share_create("user_features", "team:ml", PERM_READ | PERM_WRITE, 1)); print_str(" perm=3\n");
    print_str("  Share idx="); print_int(share_create("user_features", "team:analytics", PERM_READ, 1)); print_str(" perm=1\n");
    share_t mkt[8]; print_str("  Marketplace readable: "); print_int(share_marketplace_list(mkt, 8)); print_str("\n");
    print_str("  Updated perm old="); print_int(share_permission_set("user_features", "team:analytics", PERM_READ | PERM_WRITE)); print_str(" new=3\n");
    audit_entry_t alog[16]; int ac = share_audit_log(alog, 16);
    print_str("  Audit entries: "); print_int(ac); print_str("\n");
    for (int i = 0; i < ac; i++) {
        print_str("    ["); print_int(alog[i].ts); print_str("] action="); print_int(alog[i].action);
        print_str(" target="); print_str(alog[i].target); print_str(" perm="); print_int(alog[i].detail); print_str("\n");
    }
    print_str("\n=== All Feature Store tests passed ===\n");
}

/* ── Entry Point ──────────────────────────────────────────────────────────── */

static void print_help(void) {
    print_str("Feature Store v60.0 - Feature Management Platform\n\nOptions:\n");
    print_str("  -h    Show help\n  -t    Run tests\n  (none)  Run demo\n\nFeatures:\n");
    print_str("  Feature Groups     Registration & versioning\n  Schema Mgmt        Evolution & lineage\n");
    print_str("  Offline Store      Parquet/Avro batch + PIT query\n  Online Store       Low-latency cache with TTL\n");
    print_str("  Transforms         Online=offline consistency\n  Data Quality       Missing/PSI/KS/anomaly\n");
    print_str("  Sharing            Marketplace & audit log\n");
}

void _start(void) {
    char ab[256]; int al = host_get_argv(0, 256), rt = 0, sh = 0;
    if (al > 0 && al < 256) {
        host_get_argv((unsigned int)(unsigned long)ab, 256);
        for (int i = 0; i < al; i++) if (ab[i] == '-' && i+1 < al) { if (ab[i+1]=='t') rt=1; if (ab[i+1]=='h') sh=1; }
    }
    if (sh) { print_help(); host_exit(0); return; }
    if (rt || al == 0) { test_suite(); host_exit(0); return; }
    print_str("Feature Store v60.0 - Feature Management Platform\nUsage: feature_store [-h] [-t]\n");
    host_exit(0);
}
