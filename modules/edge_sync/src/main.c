/*
 * Edge Sync Engine v1.0 - Edge-Cloud Synchronization Platform
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib - Data/Model/Config/Task/State sync with bandwidth optimization
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

#define BUF_S 64
#define BUF_M 128
#define BUF_L 256
#define MAX_EDGES 16
#define MAX_MODELS 16
#define MAX_CONFIGS 16
#define MAX_TASKS 32
#define MAX_EVENTS 32
#define MAX_FIELDS 8
#define MAX_VERSIONS 8
#define MAX_OFFLINE 16
#define MAX_RETRY 8
#define MAX_CHUNKS 8

#define SYNC_IDLE 0
#define SYNC_ACTIVE 1
#define SYNC_PAUSED 2
#define SYNC_ERROR 3
#define EDGE_ONLINE 0
#define EDGE_DEGRADED 1
#define EDGE_OFFLINE 2
#define CONFLICT_LWW 0
#define CONFLICT_MERGE 1
#define CONFLICT_SKIP 2
#define DIR_UPSTREAM 0
#define DIR_DOWNSTREAM 1
#define DIR_BIDIR 2
#define PRI_CRITICAL 0
#define PRI_HIGH 1
#define PRI_NORMAL 2
#define PRI_LOW 3
#define TASK_PENDING 0
#define TASK_RUNNING 1
#define TASK_DONE 2
#define TASK_FAILED 3
#define EVT_STATE 0
#define EVT_CMD 1
#define EVT_HEARTBEAT 2
#define EVT_CONFIG 3
#define EVT_MODEL 4

typedef struct { int id; char name[BUF_S]; char endpoint[BUF_M]; int status; int latency_ms;
    int last_hb; int sync_dir; int conflict_pol; int offline_mode; int priority;
    int data_synced; int models_synced; int cfg_synced; int tasks_done;
    unsigned int cksum; int version; } edge_node_t;
typedef struct { int id; char name[BUF_S]; char ver[BUF_S]; int size_kb; int chunk_count;
    unsigned int cksum; int active; int ab_group; int rollout_pct; int rollback_ver;
    int versions[MAX_VERSIONS]; int ver_count; int target_edges[MAX_EDGES]; int target_cnt;
    int distributed; int updated; } model_entry_t;
typedef struct { int id; char key[BUF_S]; char value[BUF_M]; int version; int gray_pct;
    int pushed; int confirmed; int rolled_back; int rb_version; int timestamp;
    int target_edges[MAX_EDGES]; int target_cnt; } config_entry_t;
typedef struct { int id; char name[BUF_S]; int edge_id; int status; int priority;
    int result; unsigned int result_ck; int retries; int max_retries; int timeout_ms;
    int created_at; int finished_at; int chunk_total; int chunks_done; } task_entry_t;
typedef struct { int id; int type; int source_edge; int target_edge; int timestamp;
    int payload[MAX_FIELDS]; int field_count; int ack; int dispatch; } event_entry_t;
typedef struct { int id; int edge_id; int data_size; unsigned int cksum; int timestamp;
    int chunks[MAX_CHUNKS]; int chunk_count; int resumed; int compressed_size; } offline_cache_t;
typedef struct { int edge_count; int model_count; int config_count; int task_count;
    int event_count; int offline_count; int total_bytes; int compressed_bytes;
    int data_synced; int models_synced; int cfg_synced;
    int conflicts; int retries; int heartbeats; int gray_active; int ab_active;
    int bw_saved_pct; int session_start; int status; } sync_stats_t;
typedef struct { int interval_ms; int timeout_ms; int max_bw_kbps; int compress_level;
    int diff_enabled; int priority_enabled; int cost_mode; int retry_max; int retry_backoff; } sync_policy_t;

static edge_node_t g_edges[MAX_EDGES]; static int g_edge_count = 0; static int g_next_edge_id = 1;
static model_entry_t g_models[MAX_MODELS]; static int g_model_count = 0; static int g_next_model_id = 1;
static config_entry_t g_configs[MAX_CONFIGS]; static int g_config_count = 0; static int g_next_cfg_id = 1;
static task_entry_t g_tasks[MAX_TASKS]; static int g_task_count = 0; static int g_next_task_id = 1;
static event_entry_t g_events[MAX_EVENTS]; static int g_event_count = 0; static int g_next_evt_id = 1;
static offline_cache_t g_offline[MAX_OFFLINE]; static int g_offline_count = 0; static int g_next_off_id = 1;
static sync_stats_t g_stats = {0};
static sync_policy_t g_policy = {5000, 30000, 1000, 6, 1, 1, 0, 3, 200};
static int g_time = 1000; static unsigned int g_rng = 0xED6E0001;

/* Forward declarations */
static unsigned int es_strlen(const char *s);
static void es_strncpy(char *d, const char *s, unsigned int n);
static int es_strcmp(const char *a, const char *b);
static void print_str(const char *s); static void print_int(int v);
static void *es_alloc(unsigned int sz); static void es_memzero(void *p, unsigned int n);
static unsigned int rng_next(void); static unsigned int compute_ck(const int *d, int n);
static int edge_add(const char *name, const char *ep, int dir, int pri);
static int edge_set_status(int eid, int st); static int edge_find(const char *name);
static int data_sync_bidir(int e1, int e2); static int data_sync_incremental(int eid);
static int data_conflict_resolve(int e1, int e2, int policy); static int data_offline_cache(int eid, int size);
static int data_resume_transfer(int cache_id); static int data_push(int eid, int *fids, int cnt);
static int model_distribute(int mid, int *eids, int cnt); static int model_version(int mid, const char *ver);
static int model_incremental(int mid, int eid); static int model_ab_test(int mid, int pct);
static int model_rollback(int mid); static int model_status(int mid);
static int config_push(int cid, int *eids, int cnt); static int config_runtime(int cid, const char *val);
static int config_gray(int cid, int pct); static int config_rollback(int cid);
static int config_confirm(int cid); static int config_effect(int cid);
static int task_create(const char *name, int eid, int pri, int timeout); static int task_distribute(int *eids, int cnt);
static int task_execute(int tid); static int task_aggregate(int *tids, int cnt);
static int task_retry(int tid); static int task_status(int tid);
static int state_heartbeat(int eid); static int state_report(int eid, int *vals, int cnt);
static int state_notify(int type, int src, int dst, int *vals, int cnt); static int state_dispatch(int eid);
static int state_bidir(int e1, int e2); static int state_poll(void);
static int bw_compress(int *data, int cnt, int level); static int bw_diff_sync(int eid);
static int bw_priority(int *tids, int cnt); static int bw_traffic_control(int kbps);
static int bw_cost_optimize(int budget); static int bw_report(void);
static void test_suite(void); static void print_help(void);

static unsigned int es_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void es_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0; while (i < n && s[i]) { d[i] = s[i]; i++; } while (i < n) d[i++] = 0; }
static int es_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return (int)*a - (int)*b; a++; b++; } return (int)*a - (int)*b; }
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16], out[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = 0; print_str(out); }
static void *es_alloc(unsigned int sz) { return (void *)(size_t)host_alloc(sz, 8); }
static void es_memzero(void *p, unsigned int n) {
    unsigned char *b = (unsigned char *)p; for (unsigned int i = 0; i < n; i++) b[i] = 0; }
static unsigned int rng_next(void) {
    g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return g_rng; }
static unsigned int compute_ck(const int *d, int n) {
    unsigned int ck = 0xED6E0002; if (!d) return ck;
    for (int i = 0; i < n; i++) { ck ^= (unsigned int)d[i]; ck = (ck << 5) | (ck >> 27); ck += (unsigned int)(i + 1); }
    return ck; }

/* --- Data Sync: bidir, incremental, conflict, offline cache, resumable --- */
static int edge_add(const char *name, const char *ep, int dir, int pri) {
    if (g_edge_count >= MAX_EDGES) return -1;
    edge_node_t *e = &g_edges[g_edge_count]; es_memzero(e, sizeof(edge_node_t));
    e->id = g_next_edge_id++; e->status = EDGE_ONLINE; e->sync_dir = dir; e->priority = pri;
    e->conflict_pol = CONFLICT_LWW; e->latency_ms = 10 + (int)(rng_next() % 90);
    e->last_hb = g_time; es_strncpy(e->name, name, BUF_S - 1); es_strncpy(e->endpoint, ep, BUF_M - 1);
    g_edge_count++; return e->id; }
static int edge_set_status(int eid, int st) {
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) { g_edges[i].status = st; return 0; } } return -1; }
static int edge_find(const char *name) {
    for (int i = 0; i < g_edge_count; i++) { if (es_strcmp(g_edges[i].name, name) == 0) return g_edges[i].id; } return -1; }

static int data_sync_bidir(int e1, int e2) {
    int synced = 0; edge_node_t *a = 0, *b = 0;
    for (int i = 0; i < g_edge_count; i++) {
        if (g_edges[i].id == e1) a = &g_edges[i]; if (g_edges[i].id == e2) b = &g_edges[i]; }
    if (!a || !b || a->status == EDGE_OFFLINE || b->status == EDGE_OFFLINE) return -1;
    int a_to_b = 5 + (int)(rng_next() % 10); int b_to_a = 3 + (int)(rng_next() % 8);
    a->data_synced += b_to_a; b->data_synced += a_to_b; synced = a_to_b + b_to_a;
    a->last_hb = g_time; b->last_hb = g_time; g_stats.data_synced += synced;
    a->cksum ^= (unsigned int)synced; b->cksum ^= (unsigned int)(synced * 31);
    g_stats.total_bytes += (unsigned int)(synced * 48); return synced; }
static int data_sync_incremental(int eid) {
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            int delta = 2 + (int)(rng_next() % 5); g_edges[i].data_synced += delta;
            g_edges[i].version++; g_edges[i].cksum ^= (unsigned int)delta;
            g_stats.data_synced += delta; g_stats.total_bytes += (unsigned int)(delta * 24);
            return delta; } } return -1; }
static int data_conflict_resolve(int e1, int e2, int policy) {
    edge_node_t *a = 0, *b = 0;
    for (int i = 0; i < g_edge_count; i++) {
        if (g_edges[i].id == e1) a = &g_edges[i]; if (g_edges[i].id == e2) b = &g_edges[i]; }
    if (!a || !b) return -1; g_stats.conflicts++;
    if (policy == CONFLICT_LWW) {
        if (a->version >= b->version) { b->cksum = a->cksum; b->version = a->version; }
        else { a->cksum = b->cksum; a->version = b->version; } return 0; }
    if (policy == CONFLICT_MERGE) {
        unsigned int merged = (a->cksum + b->cksum) / 2;
        a->cksum = merged; b->cksum = merged; a->version++; b->version++; return 1; }
    return -1; }
static int data_offline_cache(int eid, int size) {
    if (g_offline_count >= MAX_OFFLINE) return -1;
    offline_cache_t *oc = &g_offline[g_offline_count]; es_memzero(oc, sizeof(offline_cache_t));
    oc->id = g_next_off_id++; oc->edge_id = eid; oc->data_size = size;
    oc->cksum = (unsigned int)(size * 7 + eid); oc->timestamp = g_time;
    oc->chunk_count = (size + 1023) / 1024; if (oc->chunk_count > MAX_CHUNKS) oc->chunk_count = MAX_CHUNKS;
    for (int i = 0; i < oc->chunk_count; i++) oc->chunks[i] = (int)rng_next() & 0xFFFF;
    oc->compressed_size = size * g_policy.compress_level / 10; g_offline_count++;
    g_stats.total_bytes += (unsigned int)size; return oc->id; }
static int data_resume_transfer(int cache_id) {
    for (int i = 0; i < g_offline_count; i++) { if (g_offline[i].id == cache_id) {
            offline_cache_t *oc = &g_offline[i]; if (oc->resumed) return -1;
            int transferred = oc->data_size * 3 / 4; oc->resumed = 1;
            for (int j = 0; j < g_edge_count; j++) {
                if (g_edges[j].id == oc->edge_id) { g_edges[j].data_synced += transferred; break; } }
            g_stats.data_synced += transferred; g_stats.total_bytes += (unsigned int)transferred;
            return transferred; } } return -1; }
static int data_push(int eid, int *fids, int cnt) {
    int pushed = 0;
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            int n = cnt > MAX_FIELDS ? MAX_FIELDS : cnt;
            unsigned int ck = compute_ck(fids, n); g_edges[i].cksum ^= ck;
            g_edges[i].data_synced += n; pushed = n; break; } }
    g_stats.data_synced += pushed; g_stats.total_bytes += (unsigned int)(pushed * 32); return pushed; }

/* --- Model Sync: distribution, version, incremental, AB testing, rollback --- */
static int model_distribute(int mid, int *eids, int cnt) {
    model_entry_t *m = 0;
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) { m = &g_models[i]; break; } }
    if (!m) return -1; int dist = 0;
    for (int t = 0; t < cnt && t < MAX_EDGES; t++) {
        for (int j = 0; j < g_edge_count; j++) { if (g_edges[j].id == eids[t]) {
                g_edges[j].models_synced++; m->target_edges[m->target_cnt++] = eids[t]; dist++; break; } } }
    m->distributed += dist; g_stats.models_synced += dist; g_stats.total_bytes += (unsigned int)(dist * m->size_kb * 1024);
    return dist; }
static int model_version(int mid, const char *ver) {
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) {
            if (g_models[i].ver_count >= MAX_VERSIONS) return -1;
            g_models[i].versions[g_models[i].ver_count++] = g_models[i].ver_count + 1;
            es_strncpy(g_models[i].ver, ver, BUF_S - 1); g_models[i].cksum ^= (unsigned int)g_models[i].ver_count;
            return g_models[i].ver_count; } } return -1; }
static int model_incremental(int mid, int eid) {
    model_entry_t *m = 0;
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) { m = &g_models[i]; break; } }
    if (!m) return -1; int updated = m->size_kb / 4; m->updated += updated;
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            g_edges[i].models_synced++; g_edges[i].cksum ^= m->cksum; break; } }
    g_stats.total_bytes += (unsigned int)(updated * 1024); return updated; }
static int model_ab_test(int mid, int pct) {
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) {
            g_models[i].ab_group = 1; g_models[i].rollout_pct = pct;
            g_stats.ab_active++; return pct; } } return -1; }
static int model_rollback(int mid) {
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) {
            if (g_models[i].ver_count < 2) return -1;
            g_models[i].rollback_ver = g_models[i].ver_count; g_models[i].ver_count--;
            g_models[i].cksum ^= (unsigned int)g_models[i].ver_count; return g_models[i].ver_count; } }
    return -1; }
static int model_status(int mid) {
    for (int i = 0; i < g_model_count; i++) { if (g_models[i].id == mid) {
            print_str("  Model: "); print_str(g_models[i].name); print_str(" v"); print_str(g_models[i].ver);
            print_str(", size="); print_int(g_models[i].size_kb); print_str("KB");
            print_str(", dist="); print_int(g_models[i].distributed);
            print_str(", vers="); print_int(g_models[i].ver_count);
            print_str(", AB="); print_int(g_models[i].ab_group);
            print_str(", roll%="); print_int(g_models[i].rollout_pct); print_str("\n");
            return 0; } } return -1; }

/* --- Config Sync: push, runtime update, gray push, rollback, confirm --- */
static int config_push(int cid, int *eids, int cnt) {
    config_entry_t *c = 0;
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) { c = &g_configs[i]; break; } }
    if (!c) return -1; int pushed = 0;
    for (int t = 0; t < cnt && t < MAX_EDGES; t++) {
        for (int j = 0; j < g_edge_count; j++) { if (g_edges[j].id == eids[t]) {
                g_edges[j].cfg_synced++; c->target_edges[c->target_cnt++] = eids[t]; pushed++; break; } } }
    c->pushed += pushed; c->timestamp = g_time; g_stats.cfg_synced += pushed; return pushed; }
static int config_runtime(int cid, const char *val) {
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) {
            es_strncpy(g_configs[i].value, val, BUF_M - 1); g_configs[i].version++;
            g_configs[i].timestamp = g_time; return g_configs[i].version; } } return -1; }
static int config_gray(int cid, int pct) {
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) {
            g_configs[i].gray_pct = pct; g_stats.gray_active++; return pct; } } return -1; }
static int config_rollback(int cid) {
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) {
            if (g_configs[i].version < 2) return -1;
            g_configs[i].rb_version = g_configs[i].version; g_configs[i].version--;
            g_configs[i].rolled_back = 1; g_configs[i].timestamp = g_time;
            return g_configs[i].version; } } return -1; }
static int config_confirm(int cid) {
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) {
            g_configs[i].confirmed = 1; return 0; } } return -1; }
static int config_effect(int cid) {
    for (int i = 0; i < g_config_count; i++) { if (g_configs[i].id == cid) {
            print_str("  Config: "); print_str(g_configs[i].key); print_str("="); print_str(g_configs[i].value);
            print_str(", v="); print_int(g_configs[i].version);
            print_str(", gray="); print_int(g_configs[i].gray_pct); print_str("%");
            print_str(", pushed="); print_int(g_configs[i].pushed);
            print_str(", confirmed="); print_int(g_configs[i].confirmed);
            print_str(", rb="); print_int(g_configs[i].rolled_back); print_str("\n");
            return 0; } } return -1; }

/* --- Task Scheduling: cloud orchestration, edge exec, distribution, aggregation, retry --- */
static int task_create(const char *name, int eid, int pri, int timeout) {
    if (g_task_count >= MAX_TASKS) return -1;
    task_entry_t *t = &g_tasks[g_task_count]; es_memzero(t, sizeof(task_entry_t));
    t->id = g_next_task_id++; t->edge_id = eid; t->priority = pri; t->timeout_ms = timeout;
    t->max_retries = g_policy.retry_max; t->created_at = g_time; t->status = TASK_PENDING;
    es_strncpy(t->name, name, BUF_S - 1); g_task_count++; return t->id; }
static int task_distribute(int *eids, int cnt) {
    int distributed = 0;
    for (int i = 0; i < g_task_count; i++) { if (g_tasks[i].status != TASK_PENDING) continue;
        for (int j = 0; j < cnt && j < g_edge_count; j++) {
            if (g_edges[j].id == eids[j % cnt]) {
                g_tasks[i].edge_id = eids[j % cnt]; g_tasks[i].status = TASK_RUNNING;
                distributed++; break; } } }
    g_stats.task_count += distributed; return distributed; }
static int task_execute(int tid) {
    for (int i = 0; i < g_task_count; i++) { if (g_tasks[i].id == tid) {
            if (g_tasks[i].status != TASK_RUNNING) return -1;
            int success = (int)(rng_next() % 10) > 2;
            if (success) { g_tasks[i].status = TASK_DONE; g_tasks[i].result = 0;
                g_tasks[i].finished_at = g_time; g_tasks[i].result_ck = compute_ck(&tid, 1);
                for (int j = 0; j < g_edge_count; j++) {
                    if (g_edges[j].id == g_tasks[i].edge_id) { g_edges[j].tasks_done++; break; } }
            } else { g_tasks[i].status = TASK_FAILED; g_tasks[i].result = -1;
                g_tasks[i].retries++; g_stats.retries++; }
            return g_tasks[i].status; } } return -1; }
static int task_aggregate(int *tids, int cnt) {
    int done = 0, failed = 0, total_result = 0;
    for (int t = 0; t < cnt; t++) {
        for (int i = 0; i < g_task_count; i++) { if (g_tasks[i].id == tids[t]) {
                if (g_tasks[i].status == TASK_DONE) { done++; total_result += g_tasks[i].result; }
                else if (g_tasks[i].status == TASK_FAILED) failed++; break; } } }
    return done * 100 / (cnt > 0 ? cnt : 1); }
static int task_retry(int tid) {
    for (int i = 0; i < g_task_count; i++) { if (g_tasks[i].id == tid) {
            if (g_tasks[i].retries >= g_tasks[i].max_retries) return -1;
            g_tasks[i].status = TASK_RUNNING; g_tasks[i].retries++; g_stats.retries++;
            g_time += g_policy.retry_backoff / 100; return g_tasks[i].retries; } } return -1; }
static int task_status(int tid) {
    for (int i = 0; i < g_task_count; i++) { if (g_tasks[i].id == tid) return g_tasks[i].status; } return -1; }

/* --- State Sync: heartbeat, state report, event notify, command dispatch, bidir --- */
static int state_heartbeat(int eid) {
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            g_edges[i].last_hb = g_time; g_edges[i].latency_ms = 5 + (int)(rng_next() % 50);
            g_stats.heartbeats++; return g_edges[i].latency_ms; } } return -1; }
static int state_report(int eid, int *vals, int cnt) {
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            unsigned int ck = compute_ck(vals, cnt); g_edges[i].cksum = ck;
            g_edges[i].version++; return 0; } } return -1; }
static int state_notify(int type, int src, int dst, int *vals, int cnt) {
    if (g_event_count >= MAX_EVENTS) return -1;
    event_entry_t *ev = &g_events[g_event_count]; es_memzero(ev, sizeof(event_entry_t));
    ev->id = g_next_evt_id++; ev->type = type; ev->source_edge = src; ev->target_edge = dst;
    ev->timestamp = g_time; int n = cnt > MAX_FIELDS ? MAX_FIELDS : cnt; ev->field_count = n;
    for (int i = 0; i < n; i++) ev->payload[i] = vals ? vals[i] : 0;
    g_event_count++; return ev->id; }
static int state_dispatch(int eid) {
    int dispatched = 0;
    for (int i = 0; i < g_event_count; i++) {
        if (g_events[i].target_edge == eid && !g_events[i].dispatch) {
            g_events[i].dispatch = 1; dispatched++; } }
    return dispatched; }
static int state_bidir(int e1, int e2) {
    edge_node_t *a = 0, *b = 0;
    for (int i = 0; i < g_edge_count; i++) {
        if (g_edges[i].id == e1) a = &g_edges[i]; if (g_edges[i].id == e2) b = &g_edges[i]; }
    if (!a || !b) return -1;
    int evts = 2 + (int)(rng_next() % 4);
    a->cksum ^= (unsigned int)evts; b->cksum ^= (unsigned int)(evts * 17);
    a->last_hb = g_time; b->last_hb = g_time; return evts; }
static int state_poll(void) {
    int polled = 0;
    for (int i = 0; i < g_edge_count; i++) {
        g_edges[i].last_hb = g_time; g_edges[i].latency_ms = 5 + (int)(rng_next() % 30);
        polled++; if (g_edges[i].latency_ms > 40) g_edges[i].status = EDGE_DEGRADED; }
    return polled; }

/* --- Bandwidth Optimization: compression, diff sync, priority, traffic, cost --- */
static int bw_compress(int *data, int cnt, int level) {
    if (!data || cnt <= 0 || level < 1) return -1;
    int ratio = (level >= 8) ? 3 : (level >= 5) ? 5 : 7;
    int compressed = cnt * ratio / 10; g_stats.total_bytes += (unsigned int)(cnt * 4);
    g_stats.compressed_bytes += (unsigned int)(compressed * 4); return compressed; }
static int bw_diff_sync(int eid) {
    int saved = 0;
    for (int i = 0; i < g_edge_count; i++) { if (g_edges[i].id == eid) {
            int orig = g_edges[i].data_synced * 2; saved = orig * 6 / 10;
            g_stats.total_bytes += (unsigned int)orig; g_stats.compressed_bytes += (unsigned int)(orig - saved);
            return saved; } } return -1; }
static int bw_priority(int *tids, int cnt) {
    int sorted = 0;
    for (int i = 0; i < cnt - 1; i++) {
        for (int j = 0; j < cnt - i - 1; j++) {
            int pa = PRI_LOW, pb = PRI_LOW;
            for (int k = 0; k < g_task_count; k++) {
                if (g_tasks[k].id == tids[j]) pa = g_tasks[k].priority;
                if (g_tasks[k].id == tids[j + 1]) pb = g_tasks[k].priority; }
            if (pa > pb) { int tmp = tids[j]; tids[j] = tids[j + 1]; tids[j + 1] = tmp; sorted++; } } }
    return sorted; }
static int bw_traffic_control(int kbps) {
    if (kbps < 1 || kbps > 100000) return -1;
    g_policy.max_bw_kbps = kbps; return 0; }
static int bw_cost_optimize(int budget) {
    if (budget <= 0) return -1; g_policy.cost_mode = 1;
    int saved = g_stats.total_bytes * 4 / 10;
    g_stats.compressed_bytes += (unsigned int)saved; g_stats.bw_saved_pct = 40; return saved; }
static int bw_report(void) {
    print_str("  Total bytes: "); print_int((int)g_stats.total_bytes);
    print_str(", compressed: "); print_int((int)g_stats.compressed_bytes);
    int saved = (int)g_stats.total_bytes - (int)g_stats.compressed_bytes;
    print_str(", saved: "); print_int(saved);
    print_str(", BW cap: "); print_int(g_policy.max_bw_kbps); print_str(" kbps");
    print_str(", cost mode: "); print_int(g_policy.cost_mode); print_str("\n");
    return saved; }

/* --- Test Suite --- */
static void test_suite(void) {
    print_str("=== Edge Sync Engine v1.0 Test Suite ===\n\n");

    print_str("[1] Data Sync - Bidirectional:\n");
    int e1 = edge_add("edge_nyc", "wss://nyc.edge.ai:8443", DIR_BIDIR, PRI_HIGH);
    int e2 = edge_add("edge_sfo", "wss://sfo.edge.ai:8443", DIR_BIDIR, PRI_HIGH);
    int e3 = edge_add("edge_tokyo", "wss://tyo.edge.ai:8443", DIR_UPSTREAM, PRI_NORMAL);
    int bidir = data_sync_bidir(e1, e2);
    print_str("  Bi-dir e1<->e2: "); print_int(bidir);
    print_str(", e1_data="); print_int(g_edges[0].data_synced);
    print_str(", e2_data="); print_int(g_edges[1].data_synced); print_str("\n\n");

    print_str("[2] Data Sync - Incremental + Offline Cache:\n");
    int inc = data_sync_incremental(e1);
    print_str("  Incremental e1: "); print_int(inc); print_str(", ver="); print_int(g_edges[0].version);
    int off1 = data_offline_cache(e3, 4096);
    print_str(", offline_id="); print_int(off1); print_str(", chunks="); print_int(g_offline[0].chunk_count);
    int resumed = data_resume_transfer(off1);
    print_str(", resumed="); print_int(resumed); print_str("\n\n");

    print_str("[3] Data Sync - Conflict Resolution:\n");
    g_edges[0].version = 5; g_edges[1].version = 3;
    int cr_lww = data_conflict_resolve(e1, e2, CONFLICT_LWW);
    print_str("  LWW: "); print_int(cr_lww); print_str(", e1v="); print_int(g_edges[0].version);
    print_str(", e2v="); print_int(g_edges[1].version);
    g_edges[0].version = 4; g_edges[1].version = 4;
    int cr_merge = data_conflict_resolve(e1, e2, CONFLICT_MERGE);
    print_str(", merge: "); print_int(cr_merge); print_str("\n\n");

    print_str("[4] Data Sync - Push + Resume:\n");
    int fids[] = {100, 200, 300, 400};
    int pushed = data_push(e3, fids, 4);
    print_str("  Push to e3: "); print_int(pushed);
    int off2 = data_offline_cache(e1, 8192); int res2 = data_resume_transfer(off2);
    print_str(", off2="); print_int(off2); print_str(", resume="); print_int(res2); print_str("\n\n");

    print_str("[5] Model Sync - Distribute + Version:\n");
    int m1 = g_next_model_id; (void)0;
    model_entry_t *mp = &g_models[g_model_count]; es_memzero(mp, sizeof(model_entry_t));
    mp->id = g_next_model_id++; mp->size_kb = 512; mp->active = 1;
    es_strncpy(mp->name, "llm-base-v3", BUF_S - 1); es_strncpy(mp->ver, "3.1.0", BUF_S - 1);
    mp->cksum = 0xABCD0001; g_model_count++;
    int tgt_edges[] = {e1, e2, e3}; int dist = model_distribute(m1, tgt_edges, 3);
    print_str("  Distributed: "); print_int(dist);
    int v2 = model_version(m1, "3.2.0"); print_str(", new ver: "); print_int(v2);
    int inc_m = model_incremental(m1, e1); print_str(", incremental: "); print_int(inc_m); print_str("KB\n\n");

    print_str("[6] Model Sync - AB Testing + Rollback:\n");
    int ab = model_ab_test(m1, 25);
    print_str("  AB rollout: "); print_int(ab); print_str("%");
    int rb = model_rollback(m1); print_str(", rollback to ver: "); print_int(rb);
    print_str("\n"); model_status(m1); print_str("\n");

    print_str("[7] Config Sync - Push + Runtime Update:\n");
    config_entry_t *cp = &g_configs[g_config_count]; es_memzero(cp, sizeof(config_entry_t));
    cp->id = g_next_cfg_id++; cp->version = 1; cp->timestamp = g_time;
    es_strncpy(cp->key, "max_concurrency", BUF_S - 1); es_strncpy(cp->value, "16", BUF_M - 1);
    g_config_count++; int c1 = cp->id;
    int cfg_tgts[] = {e1, e2}; int cpsh = config_push(c1, cfg_tgts, 2);
    print_str("  Config push: "); print_int(cpsh);
    int rt = config_runtime(c1, "32"); print_str(", runtime update v: "); print_int(rt); print_str("\n\n");

    print_str("[8] Config Sync - Gray Push + Rollback + Confirm:\n");
    int gray = config_gray(c1, 30); print_str("  Gray: "); print_int(gray); print_str("%");
    int crb = config_rollback(c1); print_str(", rollback v: "); print_int(crb);
    int cfm = config_confirm(c1); print_str(", confirm: "); print_int(cfm);
    print_str("\n"); config_effect(c1); print_str("\n");

    print_str("[9] Task Scheduling - Create + Distribute:\n");
    int t1 = task_create("infer_batch", e1, PRI_HIGH, 5000);
    int t2 = task_create("data_proc", e2, PRI_NORMAL, 10000);
    int t3 = task_create("model_eval", e3, PRI_LOW, 30000);
    int t4 = task_create("sync_flush", e1, PRI_CRITICAL, 2000);
    print_str("  Tasks: "); print_int(t1); print_str(","); print_int(t2);
    print_str(","); print_int(t3); print_str(","); print_int(t4);
    int dist_e[] = {e1, e2, e3}; int td = task_distribute(dist_e, 3);
    print_str(", distributed: "); print_int(td); print_str("\n\n");

    print_str("[10] Task Scheduling - Execute + Aggregate + Retry:\n");
    int r1 = task_execute(t1); int r2 = task_execute(t2); int r3 = task_execute(t3);
    print_str("  Results: "); print_int(r1); print_str(","); print_int(r2); print_str(","); print_int(r3);
    int tids[] = {t1, t2, t3, t4}; int agg = task_aggregate(tids, 4);
    print_str(", completion: "); print_int(agg); print_str("%");
    if (task_status(t4) == TASK_FAILED) { int rtr = task_retry(t4);
        print_str(", retry t4: "); print_int(rtr); } print_str("\n\n");

    print_str("[11] State Sync - Heartbeat + Report:\n");
    int hb1 = state_heartbeat(e1); int hb2 = state_heartbeat(e2); int hb3 = state_heartbeat(e3);
    print_str("  HB latency: "); print_int(hb1); print_str(","); print_int(hb2); print_str(","); print_int(hb3);
    print_str("ms, total HB: "); print_int(g_stats.heartbeats);
    int sv[] = {42, 84, 126}; int sr = state_report(e1, sv, 3);
    print_str(", report: "); print_int(sr); print_str("\n\n");

    print_str("[12] State Sync - Event Notify + Dispatch:\n");
    int ev1 = state_notify(EVT_HEARTBEAT, e1, e2, sv, 3);
    int ev2 = state_notify(EVT_CONFIG, e2, e3, sv, 2);
    int ev3 = state_notify(EVT_CMD, e1, e3, sv, 1);
    print_str("  Events: "); print_int(ev1); print_str(","); print_int(ev2); print_str(","); print_int(ev3);
    int dp = state_dispatch(e2); print_str(", dispatched to e2: "); print_int(dp);
    int dp2 = state_dispatch(e3); print_str(", to e3: "); print_int(dp2); print_str("\n\n");

    print_str("[13] State Sync - Bidir + Poll:\n");
    int sb = state_bidir(e1, e2); print_str("  Bi-dir events: "); print_int(sb);
    int polled = state_poll(); print_str(", polled: "); print_int(polled); print_str(" edges\n\n");

    print_str("[14] Bandwidth - Compression + Diff Sync:\n");
    int raw[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int comp = bw_compress(raw, 10, 6); print_str("  Compressed(lev6): "); print_int(comp); print_str("/10");
    int diff = bw_diff_sync(e1); print_str(", diff saved: "); print_int(diff); print_str("B\n\n");

    print_str("[15] Bandwidth - Priority Queue + Traffic Control:\n");
    int pq[] = {t3, t1, t4, t2}; int sorted = bw_priority(pq, 4);
    print_str("  Priority sort swaps: "); print_int(sorted);
    print_str(", order: "); for (int i = 0; i < 4; i++) { print_int(pq[i]); if (i < 3) print_str(","); }
    int tc = bw_traffic_control(500); print_str("\n  Traffic cap: "); print_int(tc); print_str(" kbps\n\n");

    print_str("[16] Bandwidth - Cost Optimization + Report:\n");
    int cost = bw_cost_optimize(1000); print_str("  Cost saved: "); print_int(cost); print_str("B");
    print_str("\n"); bw_report(); print_str("\n");

    print_str("[17] Session Summary:\n");
    print_str("  Edges: "); print_int(g_stats.edge_count = g_edge_count);
    print_str(", Models: "); print_int(g_stats.model_count = g_model_count);
    print_str(", Configs: "); print_int(g_stats.config_count = g_config_count);
    print_str(", Tasks: "); print_int(g_stats.task_count);
    print_str(", Events: "); print_int(g_stats.event_count = g_event_count);
    print_str("\n  Data synced: "); print_int(g_stats.data_synced);
    print_str(", Conflicts: "); print_int(g_stats.conflicts);
    print_str(", Retries: "); print_int(g_stats.retries);
    print_str(", Heartbeats: "); print_int(g_stats.heartbeats);
    print_str(", Gray: "); print_int(g_stats.gray_active);
    print_str(", AB: "); print_int(g_stats.ab_active); print_str("\n\n");

    print_str("=== All Edge Sync tests complete ===\n"); }

static void print_help(void) {
    print_str("Edge Sync Engine v1.0 - Edge-Cloud Synchronization Platform\n"
        "Usage: edge_sync [options]\n  -h  Show help\n  -t  Run tests\n\n"
        "Data Sync: bidirectional, incremental, conflict resolution, offline cache,\n"
        "  resumable transfer\n"
        "Model Sync: distribution, versioning, incremental update, AB testing, rollback\n"
        "Config Sync: push, runtime update, gray push, rollback, effect confirmation\n"
        "Task Schedule: cloud orchestration, edge execution, distribution, aggregation,\n"
        "  failure retry\n"
        "State Sync: heartbeat, state reporting, event notification, command dispatch,\n"
        "  bidirectional communication\n"
        "Bandwidth: compression, diff sync, priority queue, traffic control, cost opt\n"); }

void _start(void) {
    char argv_buf[BUF_L]; es_memzero(argv_buf, sizeof(argv_buf)); host_get_argv(0, BUF_L);
    int ran_test = 0, i = 0;
    while (i < BUF_L - 2) {
        while (i < BUF_L && argv_buf[i] == 0) i++; if (i >= BUF_L - 2) break;
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 'h') { print_help(); host_exit(0); return; }
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 't') ran_test = 1;
        while (i < BUF_L && argv_buf[i] != 0) i++; }
    if (ran_test) test_suite();
    else print_str("Edge Sync Engine v1.0 - Edge-Cloud Sync Platform\nUse -h for help, -t to run tests\n");
    host_exit(0); }
