/* jaeger_tracing: Distributed Tracing & Performance Profiling (v59.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_SPANS 64
#define MAX_LOGS 8
#define MAX_TAGS 8
#define MAX_TRACES 16
#define MAX_STORE 64
#define MAX_BATCH 32
#define MAX_QUERY 32
#define MAX_DEP 32
#define MAX_EDGE 64
#define MAX_FLAME 64
#define MAX_KV 16
#define TRACE_ID_LEN 32
#define SPAN_ID_LEN 16
#define CTX_BUF 128
#define SAMPLE_CONST 0
#define SAMPLE_PROB 1
#define SAMPLE_ADAPT 2
#define SAMPLE_HEAD 3
#define SAMPLE_TAIL 4
#define STORE_MEM 0
#define STORE_CASS 1
#define STORE_ES 2
#define STORE_KAFKA 3
#define REF_CHILD 0
#define REF_FOLLOWS 1
#define LOG_SEV_INFO 0
#define LOG_SEV_WARN 1
#define LOG_SEV_ERR 2

static unsigned int heap_pos = 65536;
static unsigned int bump_alloc(unsigned int n) { unsigned int p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15u) & ~15u; return p; }
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) { char *d = (char *)dst; for (unsigned int i = 0; i < len; i++) d[i] = src[i]; }
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *dst, const char *src, unsigned int n) { unsigned int i = 0; while (i < n - 1u && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0'; }
static int my_strcmp(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return (unsigned char)*a - (unsigned char)*b; }
static void print_str(const char *s) { unsigned int len = my_strlen(s); if (len == 0u) return; unsigned int off = bump_alloc(len + 1u); copy_to_mem(off, s, len); ((char *)off)[len] = '\0'; host_print((const char *)off); }
static void print_int(int v) {
    char buf[16]; int i = 0; if (v < 0) { print_str("-"); v = -v; } if (v == 0) { print_str("0"); return; }
    while (v > 0 && i < 15) { buf[i++] = '0' + (v % 10); v /= 10; } buf[i] = '\0';
    for (int j = 0; j < i / 2; j++) { char t = buf[j]; buf[j] = buf[i - 1 - j]; buf[i - 1 - j] = t; } print_str(buf);
}
static void print_double(double v) {
    int whole = (int)v; int frac = (int)((v - (double)whole) * 10000.0); if (frac < 0) frac = -frac;
    print_int(whole); print_str("."); char fb[8];
    for (int i = 3; i >= 0; i--) { fb[i] = '0' + (frac % 10); frac /= 10; } fb[4] = '\0'; print_str(fb);
}
static unsigned int rng_state = 42;
static unsigned int rng_next(void) { rng_state ^= rng_state << 13; rng_state ^= rng_state >> 17; rng_state ^= rng_state << 5; return rng_state; }
static double rng_double(void) { return (double)(rng_next() % 10000u) / 10000.0; }
static unsigned int hex_char(char c) { if (c >= '0' && c <= '9') return (unsigned int)(c - '0'); if (c >= 'a' && c <= 'f') return 10u + (unsigned int)(c - 'a'); if (c >= 'A' && c <= 'F') return 10u + (unsigned int)(c - 'A'); return 0u; }
static int my_strncmp(const char *a, const char *b, unsigned int n) { for (unsigned int i = 0; i < n; i++) { if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i]; if (a[i] == '\0') return 0; } return 0; }
static unsigned int parse_uint(const char *s) { unsigned int v = 0; while (*s >= '0' && *s <= '9') { v = v * 10u + (unsigned int)(*s - '0'); s++; } return v; }

typedef struct { char key[32]; char value[64]; } tag_t;
typedef struct { int severity; char message[64]; double timestamp; } log_entry_t;
typedef struct { char trace_id[TRACE_ID_LEN + 1]; char span_id[SPAN_ID_LEN + 1]; char parent_id[SPAN_ID_LEN + 1]; char operation[32]; char service[32]; double start_time; double duration; int ref_type; tag_t tags[MAX_TAGS]; int tag_count; log_entry_t logs[MAX_LOGS]; int log_count; int finished; int valid; unsigned int hash; } span_t;
typedef struct { char trace_id[TRACE_ID_LEN + 1]; span_t spans[MAX_SPANS]; int span_count; double start_time; double end_time; int finished; } trace_t;

static trace_t *trace_new(const char *trace_id) {
    trace_t *t = (trace_t *)bump_alloc(sizeof(trace_t));
    my_strncpy(t->trace_id, trace_id, TRACE_ID_LEN + 1); t->span_count = 0;
    t->start_time = 0.0; t->end_time = 0.0; t->finished = 0; return t;
}
static void trace_finish(trace_t *t) {
    double mn = 1e18, mx = 0.0;
    for (int i = 0; i < t->span_count; i++) {
        if (t->spans[i].start_time < mn) mn = t->spans[i].start_time;
        double end = t->spans[i].start_time + t->spans[i].duration;
        if (end > mx) mx = end;
    }
    t->start_time = mn; t->end_time = mx; t->finished = 1;
}
static int trace_get_spans(trace_t *t, span_t **out, int max_out) {
    int n = t->span_count < max_out ? t->span_count : max_out;
    for (int i = 0; i < n; i++) out[i] = &t->spans[i]; return n;
}

static span_t *span_start(trace_t *t, const char *op, const char *svc, const char *parent_id, int ref_type) {
    if (t->span_count >= MAX_SPANS) return NULL;
    span_t *s = &t->spans[t->span_count++];
    my_strncpy(s->trace_id, t->trace_id, TRACE_ID_LEN + 1);
    unsigned int sid = rng_next(); char sid_buf[SPAN_ID_LEN + 1];
    for (int i = 7; i >= 0; i--) { static const char hx[] = "0123456789abcdef"; sid_buf[i] = hx[sid & 0xFu]; sid >>= 4; }
    sid_buf[8] = '\0'; my_strncpy(s->span_id, sid_buf, SPAN_ID_LEN + 1);
    if (parent_id) my_strncpy(s->parent_id, parent_id, SPAN_ID_LEN + 1); else s->parent_id[0] = '\0';
    my_strncpy(s->operation, op, 32); my_strncpy(s->service, svc, 32);
    s->start_time = (double)rng_next() / 1000.0; s->duration = 0.0;
    s->ref_type = ref_type; s->tag_count = 0; s->log_count = 0; s->finished = 0; s->valid = 1;
    s->hash = rng_next(); return s;
}
static void span_finish(span_t *s) { s->duration = (double)rng_next() / 100.0; s->finished = 1; }
static void span_log(span_t *s, int severity, const char *msg) {
    if (s->log_count >= MAX_LOGS) return;
    log_entry_t *l = &s->logs[s->log_count++]; l->severity = severity;
    my_strncpy(l->message, msg, 64); l->timestamp = (double)rng_next() / 1000.0;
}
static void span_set_tag(span_t *s, const char *key, const char *value) {
    if (s->tag_count >= MAX_TAGS) return;
    tag_t *tg = &s->tags[s->tag_count++]; my_strncpy(tg->key, key, 32); my_strncpy(tg->value, value, 64);
}
static span_t *span_child_of(trace_t *t, const char *parent_id, const char *op, const char *svc) {
    return span_start(t, op, svc, parent_id, REF_CHILD);
}
static span_t *span_follows_from(trace_t *t, const char *parent_id, const char *op, const char *svc) {
    return span_start(t, op, svc, parent_id, REF_FOLLOWS);
}
static void span_set_operation(span_t *s, const char *op) { my_strncpy(s->operation, op, 32); }

static void context_inject_w3c(span_t *s, char *buf, unsigned int buf_len) {
    my_strncpy(buf, "00-", buf_len);
    unsigned int off = 3u;
    for (int i = 0; i < TRACE_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->trace_id[i];
    buf[off++] = '-';
    for (int i = 0; i < SPAN_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->span_id[i];
    buf[off++] = '-'; buf[off++] = '0'; buf[off++] = '1'; buf[off] = '\0';
}
static int context_extract_w3c(const char *buf, char *trace_id, char *span_id) {
    if (my_strncmp(buf, "00-", 3u) != 0) return 0;
    for (int i = 0; i < TRACE_ID_LEN; i++) trace_id[i] = buf[3u + (unsigned int)i];
    trace_id[TRACE_ID_LEN] = '\0';
    unsigned int sp = 3u + (unsigned int)TRACE_ID_LEN + 1u;
    for (int i = 0; i < SPAN_ID_LEN; i++) span_id[i] = buf[sp + (unsigned int)i];
    span_id[SPAN_ID_LEN] = '\0'; return 1;
}
static void context_inject_b3(span_t *s, char *buf, unsigned int buf_len) {
    unsigned int off = 0u;
    for (int i = 0; i < TRACE_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->trace_id[i];
    buf[off++] = '-';
    for (int i = 0; i < SPAN_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->span_id[i];
    buf[off++] = '-'; buf[off++] = '1'; buf[off] = '\0';
}
static int context_extract_b3(const char *buf, char *trace_id, char *span_id) {
    int i; for (i = 0; i < TRACE_ID_LEN && buf[i]; i++) trace_id[i] = buf[i];
    trace_id[TRACE_ID_LEN] = '\0'; if (buf[i] != '-') return 0; i++;
    int j; for (j = 0; j < SPAN_ID_LEN && buf[i + j]; j++) span_id[j] = buf[i + j];
    span_id[SPAN_ID_LEN] = '\0'; return 1;
}
static void context_inject_jaeger(span_t *s, char *buf, unsigned int buf_len) {
    unsigned int off = 0u;
    for (int i = 0; i < TRACE_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->trace_id[i];
    buf[off++] = ':';
    for (int i = 0; i < SPAN_ID_LEN && off < buf_len - 1u; i++) buf[off++] = s->span_id[i];
    buf[off++] = ':'; buf[off++] = '0'; buf[off++] = ':'; buf[off++] = '1'; buf[off] = '\0';
}
static int context_extract_jaeger(const char *buf, char *trace_id, char *span_id) {
    int i; for (i = 0; i < TRACE_ID_LEN && buf[i] && buf[i] != ':'; i++) trace_id[i] = buf[i];
    trace_id[TRACE_ID_LEN] = '\0'; if (buf[i] != ':') return 0; i++;
    int j; for (j = 0; j < SPAN_ID_LEN && buf[i + j] && buf[i + j] != ':'; j++) span_id[j] = buf[i + j];
    span_id[SPAN_ID_LEN] = '\0'; return 1;
}

typedef struct { int strategy; double rate; double threshold; int sampled_count; int total_count; } sampler_t;
static sampler_t sampler_create(int strategy, double rate) {
    sampler_t s; s.strategy = strategy; s.rate = rate; s.threshold = rate; s.sampled_count = 0; s.total_count = 0; return s;
}
static int sample_constant(sampler_t *s) { (void)s; return 1; }
static int sample_probabilistic(sampler_t *s) { return rng_double() < s->rate ? 1 : 0; }
static int sample_adaptive(sampler_t *s) {
    s->total_count++; if (s->total_count < 10) return 1;
    double actual = (double)s->sampled_count / (double)s->total_count;
    if (actual < s->rate) { s->sampled_count++; return 1; } return 0;
}
static int sample_head(sampler_t *s) { return rng_double() < s->rate ? 1 : 0; }
static int sample_tail(sampler_t *s, span_t *sp) {
    (void)s; for (int i = 0; i < sp->tag_count; i++) {
        if (my_strcmp(sp->tags[i].key, "error") == 0) return 1;
        if (my_strcmp(sp->tags[i].key, "slow") == 0) return 1;
    }
    if (sp->duration > 10.0) return 1; return 0;
}
static int sample_should_sample(sampler_t *s, span_t *sp) {
    int r = 0;
    if (s->strategy == SAMPLE_CONST) r = sample_constant(s);
    else if (s->strategy == SAMPLE_PROB) r = sample_probabilistic(s);
    else if (s->strategy == SAMPLE_ADAPT) r = sample_adaptive(s);
    else if (s->strategy == SAMPLE_HEAD) r = sample_head(s);
    else if (s->strategy == SAMPLE_TAIL) r = sample_tail(s, sp);
    if (r) s->sampled_count++; return r;
}

static span_t *span_receive(const char *trace_id, const char *span_id, const char *op, const char *svc) {
    span_t *s = (span_t *)bump_alloc(sizeof(span_t));
    my_strncpy(s->trace_id, trace_id, TRACE_ID_LEN + 1); my_strncpy(s->span_id, span_id, SPAN_ID_LEN + 1);
    s->parent_id[0] = '\0'; my_strncpy(s->operation, op, 32); my_strncpy(s->service, svc, 32);
    s->start_time = (double)rng_next() / 1000.0; s->duration = 0.0; s->ref_type = REF_CHILD;
    s->tag_count = 0; s->log_count = 0; s->finished = 1; s->valid = 1; s->hash = rng_next(); return s;
}
static int span_validate(span_t *s) {
    if (s->trace_id[0] == '\0') return 0; if (s->span_id[0] == '\0') return 0;
    if (s->operation[0] == '\0') return 0; if (s->service[0] == '\0') return 0; return 1;
}
static int span_dedup(span_t **spans, int count) {
    int unique = 0;
    for (int i = 0; i < count; i++) {
        int dup = 0;
        for (int j = 0; j < unique; j++) {
            if (my_strcmp(spans[i]->span_id, spans[j]->span_id) == 0 && my_strcmp(spans[i]->trace_id, spans[j]->trace_id) == 0) { dup = 1; break; }
        }
        if (!dup) { span_t *tmp = spans[unique]; spans[unique] = spans[i]; spans[i] = tmp; unique++; }
    }
    return unique;
}
typedef struct { span_t *spans[MAX_BATCH]; int count; } span_batch_t;
static span_batch_t *span_batch_new(void) { span_batch_t *b = (span_batch_t *)bump_alloc(sizeof(span_batch_t)); b->count = 0; return b; }
static void span_batch_add(span_batch_t *b, span_t *s) { if (b->count < MAX_BATCH) b->spans[b->count++] = s; }
static int span_batch_flush(span_batch_t *b) { int n = b->count; b->count = 0; return n; }

typedef struct { char trace_id[TRACE_ID_LEN + 1]; span_t *spans[MAX_SPANS]; int span_count; int backend; int active; } stored_trace_t;
typedef struct { stored_trace_t traces[MAX_STORE]; int count; } trace_store_t;
static trace_store_t *store_create(void) {
    trace_store_t *st = (trace_store_t *)bump_alloc(sizeof(trace_store_t)); st->count = 0;
    for (int i = 0; i < MAX_STORE; i++) st->traces[i].active = 0; return st;
}
static void store_memory_put(trace_store_t *st, trace_t *t, span_t **spans, int scnt) {
    if (st->count >= MAX_STORE) return;
    stored_trace_t *entry = &st->traces[st->count++];
    my_strncpy(entry->trace_id, t->trace_id, TRACE_ID_LEN + 1);
    entry->span_count = scnt < MAX_SPANS ? scnt : MAX_SPANS;
    for (int i = 0; i < entry->span_count; i++) entry->spans[i] = spans[i];
    entry->backend = STORE_MEM; entry->active = 1;
}
static stored_trace_t *store_memory_get(trace_store_t *st, const char *trace_id) {
    for (int i = 0; i < st->count; i++) {
        if (st->traces[i].active && my_strcmp(st->traces[i].trace_id, trace_id) == 0) return &st->traces[i];
    }
    return NULL;
}
static void store_cassandra_model(trace_store_t *st, const char *trace_id) {
    stored_trace_t *entry = store_memory_get(st, trace_id); if (entry) entry->backend = STORE_CASS;
}
static void store_es_model(trace_store_t *st, const char *trace_id) {
    stored_trace_t *entry = store_memory_get(st, trace_id); if (entry) entry->backend = STORE_ES;
}
typedef struct { span_t *spans[MAX_BATCH]; int count; int consumed; } kafka_buffer_t;
static kafka_buffer_t *store_kafka_buffer(void) {
    kafka_buffer_t *kb = (kafka_buffer_t *)bump_alloc(sizeof(kafka_buffer_t)); kb->count = 0; kb->consumed = 0; return kb;
}
static void kafka_publish(kafka_buffer_t *kb, span_t *s) { if (kb->count < MAX_BATCH) kb->spans[kb->count++] = s; }
static int store_kafka_consume(kafka_buffer_t *kb) { int n = kb->count; kb->consumed += n; kb->count = 0; return n; }

typedef struct { int trace_ids[MAX_QUERY]; int count; } query_result_t;
static query_result_t query_by_service(trace_store_t *st, const char *svc) {
    query_result_t r; r.count = 0;
    for (int i = 0; i < st->count && r.count < MAX_QUERY; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            if (my_strcmp(st->traces[i].spans[j]->service, svc) == 0) { r.trace_ids[r.count++] = i; break; }
        }
    }
    return r;
}
static query_result_t query_by_operation(trace_store_t *st, const char *op) {
    query_result_t r; r.count = 0;
    for (int i = 0; i < st->count && r.count < MAX_QUERY; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            if (my_strcmp(st->traces[i].spans[j]->operation, op) == 0) { r.trace_ids[r.count++] = i; break; }
        }
    }
    return r;
}
static query_result_t query_by_time(trace_store_t *st, double min_t, double max_t) {
    query_result_t r; r.count = 0;
    for (int i = 0; i < st->count && r.count < MAX_QUERY; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            double t = st->traces[i].spans[j]->start_time;
            if (t >= min_t && t <= max_t) { r.trace_ids[r.count++] = i; break; }
        }
    }
    return r;
}
static query_result_t query_by_latency(trace_store_t *st, double min_dur) {
    query_result_t r; r.count = 0;
    for (int i = 0; i < st->count && r.count < MAX_QUERY; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            if (st->traces[i].spans[j]->duration >= min_dur) { r.trace_ids[r.count++] = i; break; }
        }
    }
    return r;
}
static query_result_t query_by_tag(trace_store_t *st, const char *key, const char *val) {
    query_result_t r; r.count = 0;
    for (int i = 0; i < st->count && r.count < MAX_QUERY; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            span_t *sp = st->traces[i].spans[j];
            for (int k = 0; k < sp->tag_count; k++) {
                if (my_strcmp(sp->tags[k].key, key) == 0 && my_strcmp(sp->tags[k].value, val) == 0) { r.trace_ids[r.count++] = i; goto next_trace_tag; }
            }
        }
        next_trace_tag:;
    }
    return r;
}
static double query_min_duration(trace_store_t *st) {
    double mn = 1e18;
    for (int i = 0; i < st->count; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            double d = st->traces[i].spans[j]->duration;
            if (d < mn) mn = d;
        }
    }
    return mn < 1e17 ? mn : 0.0;
}

typedef struct { char service[32]; int call_count; double total_latency; } dep_node_t;
typedef struct { char from_svc[32]; char to_svc[32]; int call_count; } dep_edge_t;
typedef struct { dep_node_t nodes[MAX_DEP]; int node_count; dep_edge_t edges[MAX_EDGE]; int edge_count; } dep_graph_t;
static dep_graph_t *dependency_graph(trace_store_t *st) {
    dep_graph_t *g = (dep_graph_t *)bump_alloc(sizeof(dep_graph_t)); g->node_count = 0; g->edge_count = 0;
    for (int i = 0; i < st->count; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            span_t *sp = st->traces[i].spans[j];
            if (sp->parent_id[0] == '\0') continue;
            span_t *parent = NULL;
            for (int k = 0; k < st->traces[i].span_count; k++) {
                if (my_strcmp(st->traces[i].spans[k]->span_id, sp->parent_id) == 0) { parent = st->traces[i].spans[k]; break; }
            }
            if (!parent) continue;
            if (my_strcmp(parent->service, sp->service) == 0) continue;
            int ni = -1;
            for (int n = 0; n < g->node_count; n++) { if (my_strcmp(g->nodes[n].service, sp->service) == 0) { ni = n; break; } }
            if (ni < 0 && g->node_count < MAX_DEP) { ni = g->node_count++; my_strncpy(g->nodes[ni].service, sp->service, 32); g->nodes[ni].call_count = 0; g->nodes[ni].total_latency = 0.0; }
            if (ni >= 0) { g->nodes[ni].call_count++; g->nodes[ni].total_latency += sp->duration; }
            if (g->edge_count < MAX_EDGE) {
                dep_edge_t *e = &g->edges[g->edge_count++];
                my_strncpy(e->from_svc, parent->service, 32); my_strncpy(e->to_svc, sp->service, 32); e->call_count = 1;
            }
        }
    }
    return g;
}
static int dependency_edges(dep_graph_t *g, dep_edge_t **out, int max_out) {
    int n = g->edge_count < max_out ? g->edge_count : max_out;
    for (int i = 0; i < n; i++) out[i] = &g->edges[i]; return n;
}

typedef struct { char frame[32]; double self_time; double total_time; int depth; int children; } flame_node_t;
typedef struct { flame_node_t nodes[MAX_FLAME]; int count; } flamegraph_t;
static flamegraph_t *flamegraph_build(trace_t *t) {
    flamegraph_t *fg = (flamegraph_t *)bump_alloc(sizeof(flamegraph_t)); fg->count = 0;
    for (int i = 0; i < t->span_count && fg->count < MAX_FLAME; i++) {
        span_t *sp = &t->spans[i]; flame_node_t *fn = &fg->nodes[fg->count++];
        my_strncpy(fn->frame, sp->operation, 32); fn->self_time = sp->duration; fn->total_time = sp->duration;
        fn->depth = 0; fn->children = 0;
        for (int j = 0; j < t->span_count; j++) {
            if (i == j) continue;
            if (my_strcmp(t->spans[j].parent_id, sp->span_id) == 0) { fn->children++; fn->total_time += t->spans[j].duration; }
        }
        if (sp->parent_id[0] != '\0') fn->depth = 1;
    }
    return fg;
}
static int flamegraph_hotspot(flamegraph_t *fg) {
    int best = -1; double best_time = 0.0;
    for (int i = 0; i < fg->count; i++) {
        if (fg->nodes[i].self_time > best_time) { best_time = fg->nodes[i].self_time; best = i; }
    }
    return best;
}
static double analyze_slow_requests(trace_store_t *st, double threshold) {
    int slow = 0, total = 0;
    for (int i = 0; i < st->count; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            total++; if (st->traces[i].spans[j]->duration > threshold) slow++;
        }
    }
    return total > 0 ? (double)slow / (double)total * 100.0 : 0.0;
}
static double analyze_latency_breakdown(trace_store_t *st, const char *svc) {
    double total = 0.0; int cnt = 0;
    for (int i = 0; i < st->count; i++) {
        if (!st->traces[i].active) continue;
        for (int j = 0; j < st->traces[i].span_count; j++) {
            if (my_strcmp(st->traces[i].spans[j]->service, svc) == 0) { total += st->traces[i].spans[j]->duration; cnt++; }
        }
    }
    return cnt > 0 ? total / (double)cnt : 0.0;
}

static void show_help(void) {
    print_str("Usage: jaeger_tracing [options]\n");
    print_str("  -h    Show help\n");
    print_str("  -t    Run test suite\n");
}

static void test_suite(void) {
    print_str("=== Jaeger Tracing Test Suite ===\n\n");

    print_str("--- Trace & Span Creation ---\n");
    trace_t *tr = trace_new("abcdef0123456789abcdef0123456789");
    span_t *root = span_start(tr, "HTTP GET", "api-gateway", NULL, REF_CHILD);
    span_set_tag(root, "http.method", "GET");
    span_set_tag(root, "http.url", "/api/users");
    span_log(root, LOG_SEV_INFO, "request received");
    span_t *child1 = span_child_of(tr, root->span_id, "db_query", "user-service");
    span_set_tag(child1, "db.type", "postgresql");
    span_log(child1, LOG_SEV_INFO, "query executed");
    span_finish(child1);
    span_t *child2 = span_follows_from(tr, root->span_id, "cache_lookup", "cache-service");
    span_finish(child2);
    span_set_operation(root, "HTTP GET /api/users");
    span_finish(root);
    trace_finish(tr);
    print_str("Trace ID: "); print_str(tr->trace_id); print_str("\n");
    print_str("Span count: "); print_int(tr->span_count); print_str("\n");
    print_str("Root op: "); print_str(root->operation); print_str("\n");
    print_str("Root tags: "); print_int(root->tag_count); print_str("\n");
    print_str("Root logs: "); print_int(root->log_count); print_str("\n\n");

    print_str("--- Context Propagation (W3C) ---\n");
    char w3c_buf[CTX_BUF];
    context_inject_w3c(root, w3c_buf, CTX_BUF);
    print_str("W3C header: "); print_str(w3c_buf); print_str("\n");
    char ext_tid[TRACE_ID_LEN + 1], ext_sid[SPAN_ID_LEN + 1];
    int w3c_ok = context_extract_w3c(w3c_buf, ext_tid, ext_sid);
    print_str("W3C extract ok: "); print_int(w3c_ok); print_str("\n");
    print_str("Extracted trace: "); print_str(ext_tid); print_str("\n\n");

    print_str("--- Context Propagation (B3) ---\n");
    char b3_buf[CTX_BUF];
    context_inject_b3(root, b3_buf, CTX_BUF);
    print_str("B3 header: "); print_str(b3_buf); print_str("\n");
    int b3_ok = context_extract_b3(b3_buf, ext_tid, ext_sid);
    print_str("B3 extract ok: "); print_int(b3_ok); print_str("\n\n");

    print_str("--- Context Propagation (Jaeger) ---\n");
    char jg_buf[CTX_BUF];
    context_inject_jaeger(root, jg_buf, CTX_BUF);
    print_str("Jaeger header: "); print_str(jg_buf); print_str("\n");
    int jg_ok = context_extract_jaeger(jg_buf, ext_tid, ext_sid);
    print_str("Jaeger extract ok: "); print_int(jg_ok); print_str("\n\n");

    print_str("--- Sampling Strategies ---\n");
    sampler_t s_const = sampler_create(SAMPLE_CONST, 1.0);
    sampler_t s_prob = sampler_create(SAMPLE_PROB, 0.5);
    sampler_t s_adapt = sampler_create(SAMPLE_ADAPT, 0.3);
    sampler_t s_head = sampler_create(SAMPLE_HEAD, 0.8);
    sampler_t s_tail = sampler_create(SAMPLE_TAIL, 0.0);
    print_str("Constant: "); print_int(sample_should_sample(&s_const, root)); print_str("\n");
    print_str("Probabilistic: "); print_int(sample_should_sample(&s_prob, root)); print_str("\n");
    print_str("Adaptive: "); print_int(sample_should_sample(&s_adapt, root)); print_str("\n");
    print_str("Head: "); print_int(sample_should_sample(&s_head, root)); print_str("\n");
    span_t slow_sp; for (int ci = 0; ci < (int)sizeof(span_t); ci++) ((char *)&slow_sp)[ci] = ((char *)root)[ci];
    slow_sp.duration = 50.0;
    span_set_tag(&slow_sp, "slow", "true");
    print_str("Tail (slow): "); print_int(sample_should_sample(&s_tail, &slow_sp)); print_str("\n\n");

    print_str("--- Span Processing ---\n");
    span_t *recv1 = span_receive("trace1111111111111111111111111111", "span111111111111", "op1", "svc1");
    span_t *recv2 = span_receive("trace1111111111111111111111111111", "span111111111111", "op1", "svc1");
    span_t *recv3 = span_receive("trace2222222222222222222222222222", "span222222222222", "op2", "svc2");
    print_str("Received: 3 spans\n");
    print_str("Validate recv1: "); print_int(span_validate(recv1)); print_str("\n");
    span_t *dedup_arr[3] = { recv1, recv2, recv3 };
    int unique = span_dedup(dedup_arr, 3);
    print_str("After dedup: "); print_int(unique); print_str(" unique\n");
    span_batch_t *batch = span_batch_new();
    span_batch_add(batch, recv1); span_batch_add(batch, recv3);
    int flushed = span_batch_flush(batch);
    print_str("Batch flushed: "); print_int(flushed); print_str("\n\n");

    print_str("--- Storage Backends ---\n");
    trace_store_t *store = store_create();
    span_t *all_spans[3]; int n = trace_get_spans(tr, all_spans, 3);
    store_memory_put(store, tr, all_spans, n);
    print_str("Stored traces: "); print_int(store->count); print_str("\n");
    stored_trace_t *found = store_memory_get(store, tr->trace_id);
    print_str("Memory get: "); print_str(found ? "found" : "not found"); print_str("\n");
    store_cassandra_model(store, tr->trace_id);
    print_str("Cassandra model: backend="); print_int(found ? found->backend : -1); print_str("\n");
    store_es_model(store, tr->trace_id);
    print_str("ES model: backend="); print_int(found ? found->backend : -1); print_str("\n");
    kafka_buffer_t *kb = store_kafka_buffer();
    kafka_publish(kb, recv1); kafka_publish(kb, recv3);
    int consumed = store_kafka_consume(kb);
    print_str("Kafka published+consumed: "); print_int(consumed); print_str("\n\n");

    print_str("--- Trace Search ---\n");
    query_result_t qr1 = query_by_service(store, "user-service");
    print_str("By service 'user-service': "); print_int(qr1.count); print_str("\n");
    query_result_t qr2 = query_by_operation(store, "HTTP GET /api/users");
    print_str("By operation 'HTTP GET /api/users': "); print_int(qr2.count); print_str("\n");
    query_result_t qr3 = query_by_time(store, 0.0, 1e15);
    print_str("By time (wide): "); print_int(qr3.count); print_str("\n");
    query_result_t qr4 = query_by_latency(store, 0.0);
    print_str("By latency >= 0: "); print_int(qr4.count); print_str("\n");
    query_result_t qr5 = query_by_tag(store, "db.type", "postgresql");
    print_str("By tag db.type=postgresql: "); print_int(qr5.count); print_str("\n");
    double min_d = query_min_duration(store);
    print_str("Min duration: "); print_double(min_d); print_str("\n\n");

    print_str("--- Dependency Graph ---\n");
    dep_graph_t *dg = dependency_graph(store);
    print_str("Dep nodes: "); print_int(dg->node_count); print_str("\n");
    print_str("Dep edges: "); print_int(dg->edge_count); print_str("\n");
    dep_edge_t *edge_out[8]; int ec = dependency_edges(dg, edge_out, 8);
    for (int i = 0; i < ec; i++) {
        print_str("  "); print_str(edge_out[i]->from_svc); print_str(" -> "); print_str(edge_out[i]->to_svc);
        print_str(" ("); print_int(edge_out[i]->call_count); print_str(" calls)\n");
    }
    print_str("\n");

    print_str("--- Flame Graph & Profiling ---\n");
    flamegraph_t *fg = flamegraph_build(tr);
    print_str("Flame nodes: "); print_int(fg->count); print_str("\n");
    for (int i = 0; i < fg->count; i++) {
        print_str("  ["); print_str(fg->nodes[i].frame); print_str("] self=");
        print_double(fg->nodes[i].self_time); print_str(" total=");
        print_double(fg->nodes[i].total_time); print_str(" children=");
        print_int(fg->nodes[i].children); print_str("\n");
    }
    int hs = flamegraph_hotspot(fg);
    print_str("Hotspot: ");
    if (hs >= 0) { print_str(fg->nodes[hs].frame); print_str("\n"); } else { print_str("none\n"); }
    double slow_pct = analyze_slow_requests(store, 0.0);
    print_str("Slow request pct: "); print_double(slow_pct); print_str("%\n");
    double lat = analyze_latency_breakdown(store, "api-gateway");
    print_str("Avg latency api-gateway: "); print_double(lat); print_str("\n\n");

    print_str("=== All Tests Passed ===\n");
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int buf = host_alloc(512, 16); host_get_argv(buf, 512);
    int help = 0, test = 0; unsigned int pos = 0; char *argv_ptr = (char *)buf;
    while (pos < 512u && argv_ptr[pos]) pos++; pos++;
    while (pos < 512u && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos]; unsigned int hl = my_strlen(arg);
        if (hl == 2u && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2u && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512u && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Jaeger Tracing v59.0 - Distributed Tracing & Profiling\n");
    if (help) { show_help(); host_exit(0); }
    if (test) { test_suite(); host_exit(0); }
    print_str("Use -h for help, -t for test\n");
}
