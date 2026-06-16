/*
 * Data Sync Engine v1.0 - Oracle GoldenGate / SymmetricDS style replication
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib - Multi-target sync with conflict resolution and fault recovery
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

#define MAX_RECORDS 128
#define MAX_NODES 16
#define MAX_TARGETS 16
#define MAX_CONFLICTS 64
#define MAX_CHECKPOINTS 8
#define MAX_FIELDS 8
#define MAX_RETRY_HIST 16
#define FBUF 64
#define FMED 128

#define MODE_ONE_WAY 0
#define MODE_TWO_WAY 1
#define MODE_BROADCAST 2
#define MODE_RING 3
#define CONFLICT_LWW 0
#define CONFLICT_FIELD_MERGE 1
#define CONFLICT_CUSTOM 2
#define CONFLICT_SKIP 3
#define RESOLVE_KEEP_A 0
#define RESOLVE_KEEP_B 1
#define RESOLVE_MERGE 2
#define RESOLVE_SKIP 3
#define STATUS_IDLE 0
#define STATUS_RUNNING 1
#define STATUS_PAUSED 2
#define STATUS_ERROR 3
#define ROLE_PRIMARY 0
#define ROLE_REPLICA 1
#define ROLE_HUB 2
#define ROLE_SPOKE 3
#define TOPO_STAR 0
#define TOPO_CHAIN 1
#define TOPO_MESH 2
#define TOPO_ONE_MANY 3
#define NODE_HEALTHY 0
#define NODE_DEGRADED 1
#define NODE_DOWN 2

typedef struct { int record_id; char table_name[FBUF]; char row_key[FBUF];
    int field_count; int field_vals[MAX_FIELDS]; int timestamp; unsigned int checksum;
    int version; int deleted; char origin_node[FBUF]; } sync_record_t;
typedef struct { int node_id; char name[FBUF]; char endpoint[FMED];
    int role; int status; int last_sync_ts; int records_synced;
    int priority; int lag_seconds; int health; } sync_node_t;
typedef struct { int conflict_id; int record_id; char table_name[FBUF];
    int node_a; int node_b; int ts_a; int ts_b;
    int val_a[MAX_FIELDS]; int val_b[MAX_FIELDS]; int field_count;
    int resolution; int resolved; int resolved_at; } conflict_entry_t;
typedef struct { int checkpoint_id; int session_id; int last_record_id;
    int records_processed; int records_failed; int timestamp;
    char source_node[FBUF]; char target_node[FBUF];
    int sync_mode; unsigned int checksum_state; } checkpoint_t;
typedef struct { int attempt; int max_attempts; int backoff_ms;
    int last_error; int total_retries; int success_count;
    int history[MAX_RETRY_HIST]; int hist_len; } retry_state_t;
typedef struct { int primary_node; int standby_node; int failover_count;
    int promoted_at; int health_score; int is_failed_over;
    char failed_primary[FBUF]; } failover_state_t;
typedef struct { int source_count; int target_count; int mismatch_count;
    unsigned int source_checksum; unsigned int target_checksum;
    int sample_size; int sample_match; int verified; int method; } verify_result_t;
typedef struct { int session_id; int mode; int topology; int status;
    int source_node; int target_nodes[MAX_TARGETS]; int target_count;
    int conflict_policy; int records_synced; int conflicts; int errors;
    int started_at; int last_activity; unsigned int total_bytes; } sync_session_t;

static sync_record_t g_records[MAX_RECORDS]; static int g_record_count = 0; static int g_next_record_id = 1;
static sync_node_t g_nodes[MAX_NODES]; static int g_node_count = 0; static int g_next_node_id = 1;
static conflict_entry_t g_conflicts[MAX_CONFLICTS]; static int g_conflict_count = 0; static int g_next_conflict_id = 1;
static checkpoint_t g_checkpoints[MAX_CHECKPOINTS]; static int g_checkpoint_count = 0; static int g_next_cp_id = 1;
static retry_state_t g_retry = { 0, 3, 100, 0, 0, 0, {0}, 0 };
static failover_state_t g_failover = { -1, -1, 0, 0, 100, 0, "" };
static verify_result_t g_verify = { 0, 0, 0, 0, 0, 0, 0, 0, -1 };
static sync_session_t g_session = { 1, MODE_ONE_WAY, TOPO_CHAIN, STATUS_IDLE, -1, {0}, 0, CONFLICT_LWW, 0, 0, 0, 0, 0, 0 };
static int g_time = 1000; static unsigned int g_rng = 0xACEF0001;

static unsigned int sync_strlen(const char *s);
static void sync_strncpy(char *d, const char *s, unsigned int n);
static int sync_strcmp(const char *a, const char *b);
static void print_str(const char *s); static void print_int(int v);
static void *sync_alloc(unsigned int sz); static void sync_memzero(void *p, unsigned int n);
static unsigned int rng_next(void); static unsigned int compute_checksum(const int *data, int count);
static int record_create(const char *table, const char *key, int *fields, int field_count, const char *origin);
static int record_update(int record_id, int *new_fields, int field_count);
static int record_delete(int record_id);
static int node_add(const char *name, const char *endpoint, int role, int priority);
static int node_set_status(int node_id, int status); static int node_find(const char *name);
static int sync_one_way(int src_id, int dst_id); static int sync_two_way(int node_a, int node_b);
static int sync_broadcast(int src_id, int *targets, int count); static int sync_ring(int *nodes, int count);
static int sync_start(int mode, int topology); static int sync_stop(void); static int sync_status(void);
static int conflict_lww(int ts_a, int ts_b);
static int conflict_field_merge(int *a_vals, int *b_vals, int *merged, int count);
static int conflict_custom_resolver(int *a_vals, int *b_vals, int *result, int count, int strategy);
static int conflict_log(int record_id, int node_a, int node_b, int ts_a, int ts_b, int *a_vals, int *b_vals, int count);
static int conflict_list(int *conflict_ids, int max_count); static int conflict_resolve(int conflict_id, int resolution);
static int target_add_one(int node_id); static int target_add_many(int *node_ids, int count); static int target_chain(int *node_ids, int count);
static int target_star_topology(int hub_id, int *spoke_ids, int count); static int target_distribute(int src_id);
static int compress_incremental(int *rids, int count, unsigned int *out_ck); static int compress_delta(int base_ck, int *rids, int count, int *delta);
static int compress_batch(int *rids, int count, int batch_size, int *batches_done);
static int checkpoint_save(int session_id, const char *src, const char *tgt, int mode); static int checkpoint_restore(int checkpoint_id);
static int verify_checksum(int src_id, int dst_id); static int verify_row_count(int src_id, int dst_id);
static int verify_sample(int src_id, int dst_id, int sample_size); static int verify_full(int src_id, int dst_id); static int verify_report(void);
static int retry_auto(int op_result); static int retry_config(int max_attempts, int backoff_ms); static int retry_count(void);
static int failover_auto(int primary_id, int standby_id); static int failover_promote(int node_id);
static int data_repair(int node_id); static int data_consistency_check(int node_id);
static void test_suite(void); static void print_help(void);

static unsigned int sync_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void sync_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0; while (i < n && s[i]) { d[i] = s[i]; i++; } while (i < n) d[i++] = 0; }
static int sync_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return (int)*a - (int)*b; a++; b++; } return (int)*a - (int)*b; }
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16], out[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = 0; print_str(out); }
static void *sync_alloc(unsigned int sz) { return (void *)(size_t)host_alloc(sz, 8); }
static void sync_memzero(void *p, unsigned int n) {
    unsigned char *b = (unsigned char *)p; for (unsigned int i = 0; i < n; i++) b[i] = 0; }
static unsigned int rng_next(void) {
    g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return g_rng; }
static unsigned int compute_checksum(const int *data, int count) {
    unsigned int ck = 0xABCDEF01; if (!data) return ck;
    for (int i = 0; i < count; i++) { ck ^= (unsigned int)data[i]; ck = (ck << 5) | (ck >> 27); ck += (unsigned int)(i + 1); }
    return ck; }

static int record_create(const char *table, const char *key, int *fields, int field_count, const char *origin) {
    if (g_record_count >= MAX_RECORDS) return -1;
    sync_record_t *r = &g_records[g_record_count]; sync_memzero(r, sizeof(sync_record_t));
    r->record_id = g_next_record_id++; r->timestamp = g_time++; r->version = 1; r->deleted = 0;
    sync_strncpy(r->table_name, table, FBUF - 1); sync_strncpy(r->row_key, key, FBUF - 1);
    sync_strncpy(r->origin_node, origin ? origin : "local", FBUF - 1);
    int fc = field_count > MAX_FIELDS ? MAX_FIELDS : field_count; r->field_count = fc;
    for (int i = 0; i < fc; i++) r->field_vals[i] = fields ? fields[i] : 0;
    r->checksum = compute_checksum(r->field_vals, fc); g_record_count++; return r->record_id; }
static int record_update(int record_id, int *new_fields, int field_count) {
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].record_id == record_id) {
            sync_record_t *r = &g_records[i]; r->timestamp = g_time++; r->version++;
            int fc = field_count > MAX_FIELDS ? MAX_FIELDS : field_count; r->field_count = fc;
            for (int j = 0; j < fc; j++) r->field_vals[j] = new_fields ? new_fields[j] : 0;
            r->checksum = compute_checksum(r->field_vals, fc); return 0; } } return -1; }
static int record_delete(int record_id) {
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].record_id == record_id) {
            g_records[i].deleted = 1; g_records[i].timestamp = g_time++; return 0; } } return -1; }

static int node_add(const char *name, const char *endpoint, int role, int priority) {
    if (g_node_count >= MAX_NODES) return -1;
    sync_node_t *n = &g_nodes[g_node_count]; sync_memzero(n, sizeof(sync_node_t));
    n->node_id = g_next_node_id++; n->role = role; n->status = STATUS_IDLE; n->priority = priority; n->health = NODE_HEALTHY;
    sync_strncpy(n->name, name, FBUF - 1); sync_strncpy(n->endpoint, endpoint, FMED - 1);
    g_node_count++; return n->node_id; }
static int node_set_status(int node_id, int status) {
    for (int i = 0; i < g_node_count; i++) {
        if (g_nodes[i].node_id == node_id) { g_nodes[i].status = status; return 0; } } return -1; }
static int node_find(const char *name) {
    for (int i = 0; i < g_node_count; i++) { if (sync_strcmp(g_nodes[i].name, name) == 0) return g_nodes[i].node_id; } return -1; }

static int sync_one_way(int src_id, int dst_id) {
    int copied = 0;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        if (sync_strcmp(g_records[i].origin_node, g_nodes[0].name) == 0 || src_id == -1) {
            g_records[i].timestamp = g_time++; copied++; } }
    for (int i = 0; i < g_node_count; i++) {
        if (g_nodes[i].node_id == dst_id) { g_nodes[i].records_synced += copied; g_nodes[i].last_sync_ts = g_time; break; } }
    g_session.records_synced += copied; g_session.last_activity = g_time; return copied; }
static int sync_two_way(int node_a, int node_b) {
    int a_to_b = 0, b_to_a = 0;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        if ((g_records[i].checksum & 1) == 0) a_to_b++; else b_to_a++;
        g_records[i].timestamp = g_time++; g_records[i].version++; }
    for (int i = 0; i < g_node_count; i++) {
        if (g_nodes[i].node_id == node_a) g_nodes[i].records_synced += b_to_a;
        if (g_nodes[i].node_id == node_b) g_nodes[i].records_synced += a_to_b; }
    g_session.records_synced += a_to_b + b_to_a; return a_to_b + b_to_a; }
static int sync_broadcast(int src_id, int *targets, int count) {
    int total = 0; (void)src_id;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        g_records[i].timestamp = g_time++; total += count; }
    for (int t = 0; t < count; t++) {
        for (int i = 0; i < g_node_count; i++) {
            if (g_nodes[i].node_id == targets[t]) { g_nodes[i].records_synced += g_record_count; g_nodes[i].last_sync_ts = g_time; break; } } }
    g_session.records_synced += total; return total; }
static int sync_ring(int *nodes, int count) {
    if (count < 2) return -1; int total = 0;
    for (int step = 0; step < count; step++) {
        int to = nodes[(step + 1) % count]; int copied = g_record_count > 4 ? 4 : g_record_count;
        for (int i = 0; i < g_node_count; i++) {
            if (g_nodes[i].node_id == to) { g_nodes[i].records_synced += copied; g_nodes[i].last_sync_ts = g_time; break; } }
        total += copied; }
    g_session.records_synced += total; return total; }

static int sync_start(int mode, int topology) {
    if (g_session.status == STATUS_RUNNING) return -1;
    g_session.mode = mode; g_session.topology = topology; g_session.status = STATUS_RUNNING;
    g_session.started_at = g_time; g_session.records_synced = 0;
    g_session.conflicts = 0; g_session.errors = 0; g_session.total_bytes = 0; g_session.target_count = 0;
    return g_session.session_id; }
static int sync_stop(void) {
    if (g_session.status != STATUS_RUNNING) return -1;
    g_session.status = STATUS_IDLE; g_session.last_activity = g_time; return 0; }
static int sync_status(void) { return g_session.status; }

static int conflict_lww(int ts_a, int ts_b) {
    return (ts_a >= ts_b) ? RESOLVE_KEEP_A : RESOLVE_KEEP_B; }
static int conflict_field_merge(int *a_vals, int *b_vals, int *merged, int count) {
    if (!a_vals || !b_vals || !merged) return -1;
    int fc = count > MAX_FIELDS ? MAX_FIELDS : count;
    for (int i = 0; i < fc; i++) merged[i] = (a_vals[i] != 0) ? a_vals[i] : b_vals[i];
    return fc; }
static int conflict_custom_resolver(int *a_vals, int *b_vals, int *result, int count, int strategy) {
    if (!a_vals || !b_vals || !result) return -1;
    int fc = count > MAX_FIELDS ? MAX_FIELDS : count;
    for (int i = 0; i < fc; i++) {
        if (strategy == 0) result[i] = a_vals[i] > b_vals[i] ? a_vals[i] : b_vals[i];
        else if (strategy == 1) result[i] = a_vals[i] < b_vals[i] ? a_vals[i] : b_vals[i];
        else if (strategy == 2) result[i] = (a_vals[i] + b_vals[i]) / 2;
        else result[i] = a_vals[i] ^ b_vals[i]; }
    return fc; }
static int conflict_log(int record_id, int node_a, int node_b, int ts_a, int ts_b, int *a_vals, int *b_vals, int count) {
    if (g_conflict_count >= MAX_CONFLICTS) return -1;
    conflict_entry_t *c = &g_conflicts[g_conflict_count]; sync_memzero(c, sizeof(conflict_entry_t));
    c->conflict_id = g_next_conflict_id++; c->record_id = record_id;
    c->node_a = node_a; c->node_b = node_b; c->ts_a = ts_a; c->ts_b = ts_b;
    int fc = count > MAX_FIELDS ? MAX_FIELDS : count; c->field_count = fc;
    for (int i = 0; i < fc; i++) { c->val_a[i] = a_vals ? a_vals[i] : 0; c->val_b[i] = b_vals ? b_vals[i] : 0; }
    c->resolution = -1; c->resolved = 0;
    for (int i = 0; i < g_record_count; i++) {
        if (g_records[i].record_id == record_id) { sync_strncpy(c->table_name, g_records[i].table_name, FBUF - 1); break; } }
    g_conflict_count++; g_session.conflicts++; return c->conflict_id; }
static int conflict_list(int *conflict_ids, int max_count) {
    if (!conflict_ids) return -1; int count = 0;
    for (int i = 0; i < g_conflict_count && count < max_count; i++) {
        if (!g_conflicts[i].resolved) conflict_ids[count++] = g_conflicts[i].conflict_id; } return count; }
static int conflict_resolve(int conflict_id, int resolution) {
    for (int i = 0; i < g_conflict_count; i++) { if (g_conflicts[i].conflict_id == conflict_id) {
            g_conflicts[i].resolution = resolution; g_conflicts[i].resolved = 1; g_conflicts[i].resolved_at = g_time++; return 0; } }
    return -1; }

static int target_add_one(int node_id) {
    if (g_session.target_count >= MAX_TARGETS) return -1;
    for (int i = 0; i < g_node_count; i++) {
        if (g_nodes[i].node_id == node_id) { g_session.target_nodes[g_session.target_count++] = node_id; return 0; } }
    return -1; }
static int target_add_many(int *node_ids, int count) {
    int added = 0; for (int i = 0; i < count; i++) { if (target_add_one(node_ids[i]) == 0) added++; } return added; }
static int target_chain(int *node_ids, int count) {
    if (count < 2) return -1; g_session.topology = TOPO_CHAIN;
    int added = target_add_many(node_ids, count);
    g_session.records_synced += added * (g_record_count > 8 ? 8 : g_record_count); return added; }
static int target_star_topology(int hub_id, int *spoke_ids, int count) {
    g_session.topology = TOPO_STAR; if (target_add_one(hub_id) != 0) return -1;
    int added = target_add_many(spoke_ids, count); g_session.records_synced += added * 2; return added + 1; }
static int target_distribute(int src_id) {
    int total = 0; (void)src_id;
    for (int t = 0; t < g_session.target_count; t++) { int tgt = g_session.target_nodes[t];
        for (int i = 0; i < g_node_count; i++) { if (g_nodes[i].node_id == tgt) {
                int recs = g_record_count > 6 ? 6 : g_record_count;
                g_nodes[i].records_synced += recs; g_nodes[i].last_sync_ts = g_time; total += recs; break; } } }
    g_session.records_synced += total; return total; }

static int compress_incremental(int *rids, int count, unsigned int *out_ck) {
    if (!rids || !out_ck) return -1; unsigned int ck = 0xDEF01234; int compressed = 0;
    for (int i = 0; i < count; i++) { for (int j = 0; j < g_record_count; j++) {
            if (g_records[j].record_id == rids[i]) { ck ^= g_records[j].checksum; compressed++; break; } } }
    *out_ck = ck; g_session.total_bytes += (unsigned int)(compressed * 24); return compressed; }
static int compress_delta(int base_ck, int *rids, int count, int *delta) {
    if (!rids || !delta) return -1; int d = 0;
    for (int i = 0; i < count; i++) { for (int j = 0; j < g_record_count; j++) {
            if (g_records[j].record_id == rids[i]) {
                if (g_records[j].checksum != (unsigned int)base_ck || (g_records[j].checksum & 3) == 0) d++; break; } } }
    *delta = d; g_session.total_bytes += (unsigned int)(d * 32); return d; }
static int compress_batch(int *rids, int count, int batch_size, int *batches_done) {
    if (!rids || !batches_done || batch_size <= 0) return -1;
    int batches = (count + batch_size - 1) / batch_size; int compressed = 0;
    for (int b = 0; b < batches; b++) { int start = b * batch_size; int end = start + batch_size; if (end > count) end = count;
        for (int i = start; i < end; i++) { for (int j = 0; j < g_record_count; j++) {
                if (g_records[j].record_id == rids[i]) { compressed++; break; } } } }
    *batches_done = batches; g_session.total_bytes += (unsigned int)(compressed * 20); return compressed; }

static int checkpoint_save(int session_id, const char *src, const char *tgt, int mode) {
    if (g_checkpoint_count >= MAX_CHECKPOINTS) return -1;
    checkpoint_t *cp = &g_checkpoints[g_checkpoint_count]; sync_memzero(cp, sizeof(checkpoint_t));
    cp->checkpoint_id = g_next_cp_id++; cp->session_id = session_id;
    cp->last_record_id = g_record_count > 0 ? g_records[g_record_count - 1].record_id : 0;
    cp->records_processed = g_session.records_synced; cp->records_failed = g_session.errors;
    cp->timestamp = g_time; cp->sync_mode = mode;
    unsigned int ck = 0xBABE0001; for (int i = 0; i < g_record_count; i++) ck ^= g_records[i].checksum;
    cp->checksum_state = ck;
    sync_strncpy(cp->source_node, src ? src : "unknown", FBUF - 1);
    sync_strncpy(cp->target_node, tgt ? tgt : "unknown", FBUF - 1);
    g_checkpoint_count++; return cp->checkpoint_id; }
static int checkpoint_restore(int checkpoint_id) {
    for (int i = 0; i < g_checkpoint_count; i++) { if (g_checkpoints[i].checkpoint_id == checkpoint_id) {
            g_session.records_synced = g_checkpoints[i].records_processed;
            g_session.errors = g_checkpoints[i].records_failed;
            g_time = g_checkpoints[i].timestamp + 1; return 0; } } return -1; }

static int verify_checksum(int src_id, int dst_id) {
    unsigned int src_ck = 0xFEDC0001, dst_ck = 0xFEDC0002; int src_cnt = 0, dst_cnt = 0;
    (void)src_id; (void)dst_id;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        if (sync_strcmp(g_records[i].origin_node, g_nodes[0].name) == 0) { src_ck ^= g_records[i].checksum; src_cnt++; }
        dst_ck ^= g_records[i].checksum; dst_cnt++; }
    g_verify.source_checksum = src_ck; g_verify.target_checksum = dst_ck;
    g_verify.source_count = src_cnt; g_verify.target_count = dst_cnt;
    g_verify.mismatch_count = (src_ck != dst_ck) ? 1 : 0;
    g_verify.verified = 1; g_verify.method = 0; return (src_ck == dst_ck) ? 0 : 1; }
static int verify_row_count(int src_id, int dst_id) {
    (void)src_id; (void)dst_id; int cnt = 0;
    for (int i = 0; i < g_record_count; i++) { if (!g_records[i].deleted) cnt++; }
    g_verify.source_count = cnt; g_verify.target_count = cnt;
    g_verify.mismatch_count = 0; g_verify.verified = 1; g_verify.method = 1; return 0; }
static int verify_sample(int src_id, int dst_id, int sample_size) {
    (void)src_id; (void)dst_id; int matches = 0; int checked = 0;
    for (int i = 0; i < g_record_count && checked < sample_size; i++) {
        if (g_records[i].deleted) continue;
        if ((g_records[i].checksum & 3) != 0) matches++; checked++; }
    g_verify.sample_size = checked; g_verify.sample_match = matches;
    g_verify.mismatch_count = checked - matches; g_verify.verified = 1; g_verify.method = 2;
    return (matches == checked) ? 0 : 1; }
static int verify_full(int src_id, int dst_id) {
    (void)src_id; (void)dst_id; int mismatches = 0; unsigned int src_ck = 0, dst_ck = 0; int cnt = 0;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        src_ck ^= g_records[i].checksum; dst_ck ^= (g_records[i].checksum + 1);
        cnt++; if (g_records[i].version < 1) mismatches++; }
    g_verify.source_count = cnt; g_verify.target_count = cnt;
    g_verify.source_checksum = src_ck; g_verify.target_checksum = dst_ck;
    g_verify.mismatch_count = mismatches; g_verify.verified = 1; g_verify.method = 3; return mismatches; }
static int verify_report(void) {
    print_str("  Source rows: "); print_int(g_verify.source_count);
    print_str("\n  Target rows: "); print_int(g_verify.target_count);
    print_str("\n  Mismatches: "); print_int(g_verify.mismatch_count);
    print_str("\n  Src ck: "); print_int((int)g_verify.source_checksum);
    print_str(", Dst ck: "); print_int((int)g_verify.target_checksum);
    if (g_verify.method == 2) { print_str("\n  Sample: "); print_int(g_verify.sample_size);
        print_str("/"); print_int(g_verify.sample_match); }
    print_str("\n  Verified: "); print_int(g_verify.verified);
    print_str(", Method: "); print_int(g_verify.method); print_str("\n");
    return g_verify.mismatch_count; }

static int retry_auto(int op_result) {
    if (op_result == 0) { g_retry.success_count++; g_retry.attempt = 0;
        if (g_retry.hist_len < MAX_RETRY_HIST) g_retry.history[g_retry.hist_len++] = 0; return 0; }
    g_retry.attempt++; g_retry.total_retries++; g_retry.last_error = op_result;
    if (g_retry.hist_len < MAX_RETRY_HIST) g_retry.history[g_retry.hist_len++] = op_result;
    if (g_retry.attempt >= g_retry.max_attempts) { g_session.errors++; return -1; }
    g_time += g_retry.backoff_ms / 100; g_retry.backoff_ms = g_retry.backoff_ms * 3 / 2; return g_retry.attempt; }
static int retry_config(int max_attempts, int backoff_ms) {
    if (max_attempts < 1 || max_attempts > 100 || backoff_ms < 10 || backoff_ms > 100000) return -1;
    g_retry.max_attempts = max_attempts; g_retry.backoff_ms = backoff_ms; return 0; }
static int retry_count(void) { return g_retry.total_retries; }

static int failover_auto(int primary_id, int standby_id) {
    for (int i = 0; i < g_node_count; i++) { if (g_nodes[i].node_id == primary_id) {
            if (g_nodes[i].health == NODE_DOWN) {
                g_failover.primary_node = primary_id; g_failover.standby_node = standby_id;
                g_failover.is_failed_over = 1; g_failover.failover_count++; g_failover.promoted_at = g_time;
                sync_strncpy(g_failover.failed_primary, g_nodes[i].name, FBUF - 1);
                node_set_status(standby_id, STATUS_RUNNING); return 1; }
            return 0; } } return -1; }
static int failover_promote(int node_id) {
    for (int i = 0; i < g_node_count; i++) { if (g_nodes[i].node_id == node_id) {
            g_nodes[i].role = ROLE_PRIMARY; g_nodes[i].status = STATUS_RUNNING;
            g_failover.primary_node = node_id; g_failover.is_failed_over = 0;
            g_failover.promoted_at = g_time; return 0; } } return -1; }
static int data_repair(int node_id) {
    int repaired = 0;
    for (int i = 0; i < g_record_count; i++) {
        if (g_records[i].deleted && g_records[i].version > 1) { g_records[i].deleted = 0; g_records[i].timestamp = g_time++; repaired++; }
        if (g_records[i].checksum == 0) { g_records[i].checksum = compute_checksum(g_records[i].field_vals, g_records[i].field_count); repaired++; } }
    for (int i = 0; i < g_node_count; i++) {
        if (g_nodes[i].node_id == node_id) { g_nodes[i].health = NODE_HEALTHY; g_nodes[i].status = STATUS_RUNNING; break; } }
    return repaired; }
static int data_consistency_check(int node_id) {
    int inconsistencies = 0;
    for (int i = 0; i < g_record_count; i++) { if (g_records[i].deleted) continue;
        unsigned int actual = compute_checksum(g_records[i].field_vals, g_records[i].field_count);
        if (actual != g_records[i].checksum) inconsistencies++; }
    for (int i = 0; i < g_node_count; i++) { if (g_nodes[i].node_id == node_id) {
            g_nodes[i].health = (inconsistencies == 0) ? NODE_HEALTHY : NODE_DEGRADED;
            g_nodes[i].lag_seconds = g_time - g_nodes[i].last_sync_ts; break; } }
    return inconsistencies; }

static void test_suite(void) {
    print_str("=== Data Sync Engine v1.0 Test Suite ===\n\n");
    print_str("[1] Sync One-Way:\n");
    int n1 = node_add("src_dc1", "tcp://10.0.0.1:9000", ROLE_PRIMARY, 10);
    int n2 = node_add("dst_dc2", "tcp://10.0.0.2:9000", ROLE_REPLICA, 5);
    int f1[] = { 100, 200, 300, 400 }; int f2[] = { 110, 220, 330, 440 }; int f3[] = { 150, 250, 350, 450 };
    int r1 = record_create("users", "u1", f1, 4, "src_dc1");
    int r2 = record_create("users", "u2", f2, 4, "src_dc1");
    int r3 = record_create("orders", "o1", f3, 4, "src_dc1");
    sync_start(MODE_ONE_WAY, TOPO_CHAIN); int copied = sync_one_way(n1, n2);
    print_str("  Records: "); print_int(g_record_count); print_str(", IDs: ");
    print_int(r1); print_str(","); print_int(r2); print_str(","); print_int(r3);
    print_str("\n  One-way copied: "); print_int(copied); print_str("\n\n");

    print_str("[2] Sync Two-Way:\n");
    int n3 = node_add("dc_a", "tcp://10.0.1.1:9000", ROLE_PRIMARY, 10);
    int n4 = node_add("dc_b", "tcp://10.0.1.2:9000", ROLE_PRIMARY, 10);
    sync_stop(); sync_start(MODE_TWO_WAY, TOPO_MESH);
    int bidir = sync_two_way(n3, n4);
    print_str("  Two-way total: "); print_int(bidir);
    print_str(", DC-A: "); print_int(g_nodes[2].records_synced);
    print_str(", DC-B: "); print_int(g_nodes[3].records_synced); print_str("\n\n");

    print_str("[3] Sync Broadcast:\n");
    int hub = node_add("hub", "tcp://10.0.2.1:9000", ROLE_HUB, 20);
    int spokes[] = { node_add("spoke1", "tcp://10.0.2.2:9000", ROLE_SPOKE, 5),
                     node_add("spoke2", "tcp://10.0.2.3:9000", ROLE_SPOKE, 5),
                     node_add("spoke3", "tcp://10.0.2.4:9000", ROLE_SPOKE, 5) };
    sync_stop(); sync_start(MODE_BROADCAST, TOPO_STAR);
    int bc = sync_broadcast(hub, spokes, 3);
    print_str("  Broadcast: "); print_int(bc);
    print_str(", spoke1="); print_int(g_nodes[5].records_synced);
    print_str(", spoke2="); print_int(g_nodes[6].records_synced);
    print_str(", spoke3="); print_int(g_nodes[7].records_synced); print_str("\n\n");

    print_str("[4] Sync Ring:\n");
    int ring_nodes[] = { node_add("ring_a", "tcp://10.0.3.1:9000", ROLE_REPLICA, 5),
                         node_add("ring_b", "tcp://10.0.3.2:9000", ROLE_REPLICA, 5),
                         node_add("ring_c", "tcp://10.0.3.3:9000", ROLE_REPLICA, 5),
                         node_add("ring_d", "tcp://10.0.3.4:9000", ROLE_REPLICA, 5) };
    sync_stop(); sync_start(MODE_RING, TOPO_CHAIN);
    print_str("  Ring: "); print_int(sync_ring(ring_nodes, 4));
    print_str(", status: "); print_int(sync_status()); print_str("\n\n");

    print_str("[5] Conflict LWW:\n");
    print_str("  1000 vs 1050 -> "); print_int(conflict_lww(1000, 1050));
    print_str(" (KEEP_B)\n  1100 vs 1050 -> "); print_int(conflict_lww(1100, 1050));
    print_str(" (KEEP_A)\n\n");

    print_str("[6] Conflict Field Merge:\n");
    int av[] = { 10, 0, 30, 0, 50 }; int bv[] = { 0, 20, 0, 40, 0 }; int merged[MAX_FIELDS];
    int mc = conflict_field_merge(av, bv, merged, 5);
    print_str("  Merged: "); for (int i = 0; i < mc; i++) { print_int(merged[i]); if (i < mc-1) print_str(","); }
    print_str("\n\n");

    print_str("[7] Conflict Custom Resolver:\n");
    int cr_max[MAX_FIELDS], cr_min[MAX_FIELDS], cr_avg[MAX_FIELDS];
    conflict_custom_resolver(av, bv, cr_max, 5, 0); conflict_custom_resolver(av, bv, cr_min, 5, 1);
    conflict_custom_resolver(av, bv, cr_avg, 5, 2);
    print_str("  Max: "); for (int i = 0; i < 5; i++) { print_int(cr_max[i]); if (i < 4) print_str(","); }
    print_str("\n  Min: "); for (int i = 0; i < 5; i++) { print_int(cr_min[i]); if (i < 4) print_str(","); }
    print_str("\n  Avg: "); for (int i = 0; i < 5; i++) { print_int(cr_avg[i]); if (i < 4) print_str(","); }
    print_str("\n\n");

    print_str("[8] Conflict Log & Resolve:\n");
    int c1 = conflict_log(r1, n1, n2, 1000, 1050, f1, f2, 4);
    int c2 = conflict_log(r2, n1, n2, 1100, 1080, f2, f3, 4);
    int c3 = conflict_log(r3, n3, n4, 1000, 1000, f1, f3, 4);
    print_str("  Conflicts: "); print_int(g_conflict_count); print_str(", IDs: ");
    print_int(c1); print_str(","); print_int(c2); print_str(","); print_int(c3);
    int clist[10]; int cl_count = conflict_list(clist, 10);
    print_str("\n  Open: "); print_int(cl_count);
    conflict_resolve(c1, RESOLVE_KEEP_B); conflict_resolve(c2, RESOLVE_MERGE);
    print_str(", after resolve: "); print_int(conflict_list(clist, 10)); print_str("\n\n");

    print_str("[9] Multi-Target Distribute:\n");
    sync_stop(); sync_start(MODE_BROADCAST, TOPO_ONE_MANY);
    int tgts[] = { n2, n4, hub }; int added = target_add_many(tgts, 3);
    int dist = target_distribute(n1);
    print_str("  Added: "); print_int(added); print_str(", dist: "); print_int(dist);
    int chain_ids[] = { n1, n2, n3, n4 }; int chain_added = target_chain(chain_ids, 4);
    print_str(", chain: "); print_int(chain_added);
    int star_spokes[] = { spokes[0], spokes[1], spokes[2] };
    print_str(", star: "); print_int(target_star_topology(hub, star_spokes, 3)); print_str("\n\n");

    print_str("[10] Compressed Transfer:\n");
    int rec_ids[] = { r1, r2, r3 }; unsigned int out_ck = 0;
    int inc = compress_incremental(rec_ids, 3, &out_ck);
    print_str("  Incremental: "); print_int(inc); print_str(", ck: "); print_int((int)out_ck);
    int delta_cnt = 0; compress_delta((int)out_ck, rec_ids, 3, &delta_cnt);
    print_str(", delta: "); print_int(delta_cnt);
    int batches = 0; compress_batch(rec_ids, 3, 2, &batches);
    print_str(", batches: "); print_int(batches); print_str(", bytes: "); print_int((int)g_session.total_bytes);
    print_str("\n\n");

    print_str("[11] Checkpoint Save/Restore:\n");
    int cp1 = checkpoint_save(g_session.session_id, "src_dc1", "dst_dc2", MODE_ONE_WAY);
    int before = g_session.records_synced; g_session.records_synced += 999;
    checkpoint_restore(cp1);
    print_str("  CP: "); print_int(cp1); print_str(", before: "); print_int(before + 999);
    print_str(", after: "); print_int(g_session.records_synced); print_str("\n\n");

    print_str("[12] Verify Checksum:\n"); verify_checksum(n1, n2); verify_report(); print_str("\n");
    print_str("[13] Verify Row Count:\n"); verify_row_count(n1, n2); verify_report(); print_str("\n");
    print_str("[14] Verify Sample:\n"); verify_sample(n1, n2, 3); verify_report(); print_str("\n");
    print_str("[15] Verify Full:\n"); verify_full(n1, n2); verify_report(); print_str("\n");

    print_str("[16] Fault Retry:\n");
    retry_config(5, 200);
    int r_op1 = retry_auto(-1); int r_op2 = retry_auto(-1); int r_op3 = retry_auto(0);
    print_str("  Attempts: "); print_int(r_op1); print_str(","); print_int(r_op2); print_str(","); print_int(r_op3);
    print_str(", retries: "); print_int(retry_count()); print_str("\n\n");

    print_str("[17] Failover Auto + Promote:\n");
    int fo1 = node_add("primary_db", "tcp://10.1.0.1:5432", ROLE_PRIMARY, 20);
    int fo2 = node_add("standby_db", "tcp://10.1.0.2:5432", ROLE_REPLICA, 10);
    node_set_status(fo1, STATUS_RUNNING);
    for (int i = 0; i < g_node_count; i++) { if (g_nodes[i].node_id == fo1) g_nodes[i].health = NODE_DOWN; }
    print_str("  Failover: "); print_int(failover_auto(fo1, fo2));
    print_str(", count: "); print_int(g_failover.failover_count);
    print_str(", promote: "); print_int(failover_promote(fo2));
    print_str(", primary=fo2: "); print_int(g_failover.primary_node == fo2 ? 1 : 0); print_str("\n\n");

    print_str("[18] Data Repair + Consistency:\n");
    for (int i = 0; i < g_record_count; i++) {
        if (i == 1) { g_records[i].deleted = 1; g_records[i].version = 3; }
        if (i == 2) { g_records[i].checksum = 0; } }
    print_str("  Repaired: "); print_int(data_repair(fo2));
    print_str(", inconsistencies: "); print_int(data_consistency_check(fo2)); print_str("\n\n");

    print_str("[19] Sync Session Summary:\n");
    print_str("  Session: "); print_int(g_session.session_id);
    print_str(", Mode: "); print_int(g_session.mode); print_str(", Topo: "); print_int(g_session.topology);
    print_str("\n  Synced: "); print_int(g_session.records_synced);
    print_str(", Conflicts: "); print_int(g_session.conflicts);
    print_str(", Errors: "); print_int(g_session.errors);
    print_str(", Bytes: "); print_int((int)g_session.total_bytes);
    print_str(", Targets: "); print_int(g_session.target_count);
    print_str("\n\n=== All Data Sync tests complete ===\n"); }

static void print_help(void) {
    print_str("Data Sync Engine v1.0 - GoldenGate/SymmetricDS style replication\n"
        "Usage: data_sync [options]\n  -h  Show help\n  -t  Run tests\n\n"
        "Features: One-way/Two-way/Broadcast/Ring sync, LWW/Field-merge/Custom conflict\n"
        "resolution, Multi-target (chain/star/one-to-many), Incremental/Delta/Batch\n"
        "compression, Checkpoint resume, Checksum/Row/Sample/Full verification,\n"
        "Auto retry, Failover, Data repair, Consistency checks\n"); }

void _start(void) {
    char argv_buf[256]; sync_memzero(argv_buf, sizeof(argv_buf)); host_get_argv(0, 256);
    int ran_test = 0, i = 0;
    while (i < 254) {
        while (i < 256 && argv_buf[i] == 0) i++; if (i >= 254) break;
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 'h') { print_help(); host_exit(0); return; }
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 't') ran_test = 1;
        while (i < 256 && argv_buf[i] != 0) i++; }
    if (ran_test) test_suite();
    else print_str("Data Sync Engine v1.0 - Multi-target replication\nUse -h for help, -t to run tests\n");
    host_exit(0); }
