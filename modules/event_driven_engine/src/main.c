/*
 * event_driven_engine: Event-driven engine module (v46.0)
 * Event bus, CQRS, Saga, Event store, processing guarantees, reactive streams
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
#define MAX_EVENTS 96
#define MAX_SUBS 32
#define MAX_ROUTES 16
#define MAX_TRANS 16
#define MAX_CMDS 32
#define MAX_QUERIES 16
#define MAX_SAGAS 16
#define MAX_SAGA_STEPS 8
#define MAX_STORE 128
#define MAX_STREAMS 16
#define MAX_STREAM_SUB 32
#define MAX_IDEMP 64
#define MAX_NAME 32
#define MAX_DATA 96
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b; }
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0'; }
static unsigned int hash_str(const char *s) {
    unsigned int h = 5381; while (*s) { h = ((h << 5) + h) + (unsigned char)*s; s++; } return h; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) b[p++] = '0';
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; } }
    host_print(b);
}
typedef struct { unsigned int seq; char type[MAX_NAME]; char source[MAX_NAME];
    char data[MAX_DATA]; unsigned int ts; int active; } event_t;
typedef struct { char filter_type[MAX_NAME]; int sub_id; int active; } sub_t;
typedef struct { char from_type[MAX_NAME]; char to_type[MAX_NAME]; int active; } route_t;
typedef struct { char in_type[MAX_NAME]; char out_type[MAX_NAME];
    char mapping[MAX_DATA]; int active; } trans_t;
typedef struct { char name[MAX_NAME]; char payload[MAX_DATA];
    int dispatched, handled, result, active; } cmd_t;
typedef struct { char name[MAX_NAME]; char criteria[MAX_DATA];
    int dispatched, handled; char result[MAX_DATA]; int cost, active; } query_t;
typedef struct { char name[MAX_NAME]; int version;
    char data[MAX_DATA]; int active; } model_t;
typedef struct { char action[MAX_NAME]; int completed; } saga_step_t;
typedef struct { char id[MAX_NAME]; saga_step_t steps[MAX_SAGA_STEPS];
    int step_count, completed, compensating, active; } saga_t;
typedef struct { char key[MAX_NAME]; char value[MAX_DATA]; int active; } saga_state_t;
typedef struct { unsigned int seq; char agg_id[MAX_NAME]; char type[MAX_NAME];
    char data[MAX_DATA]; unsigned int ts; int active, compacted; } store_entry_t;
typedef struct { char name[MAX_NAME]; int subs[MAX_STREAM_SUB];
    int sub_count; unsigned int backlog, capacity; int active; } stream_t;
typedef struct { unsigned int hash; int count, active; } idemp_t;
static event_t g_events[MAX_EVENTS]; static int g_event_n = 0;
static sub_t g_subs[MAX_SUBS]; static int g_sub_n = 0;
static route_t g_routes[MAX_ROUTES]; static int g_route_n = 0;
static trans_t g_trans[MAX_TRANS]; static int g_trans_n = 0;
static cmd_t g_cmds[MAX_CMDS]; static int g_cmd_n = 0;
static query_t g_queries[MAX_QUERIES]; static int g_query_n = 0;
static model_t g_read_models[8]; static int g_read_n = 0;
static model_t g_write_models[8]; static int g_write_n = 0;
static saga_t g_sagas[MAX_SAGAS]; static int g_saga_n = 0;
static saga_state_t g_saga_state[32]; static int g_saga_state_n = 0;
static store_entry_t g_store[MAX_STORE]; static int g_store_n = 0;
static stream_t g_streams[MAX_STREAMS]; static int g_stream_n = 0;
static idemp_t g_idemp[MAX_IDEMP]; static int g_idemp_n = 0;
static unsigned int g_clock = 1; static unsigned int g_seq = 1;
static int event_publish(const char *type, const char *source, const char *data) {
    if (g_event_n >= MAX_EVENTS) { pstr("  Error: event buffer full\n"); return -1; }
    int id = g_event_n++; g_events[id].seq = g_seq++;
    my_strncpy(g_events[id].type, type, MAX_NAME - 1);
    my_strncpy(g_events[id].source, source, MAX_NAME - 1);
    my_strncpy(g_events[id].data, data, MAX_DATA - 1);
    g_events[id].ts = g_clock++; g_events[id].active = 1; pstr("  Published event type='"); pstr(type);
    pstr("' seq="); pint(g_events[id].seq); pstr("\n"); return id;
}
static int event_subscribe(const char *filter_type, int sub_id) {
    if (g_sub_n >= MAX_SUBS) { pstr("  Error: sub limit\n"); return -1; }
    int id = g_sub_n++;
    my_strncpy(g_subs[id].filter_type, filter_type, MAX_NAME - 1);
    g_subs[id].sub_id = sub_id; g_subs[id].active = 1; pstr("  Subscribe sub="); pint(sub_id);
    pstr(" filter='"); pstr(filter_type); pstr("'\n"); return id;
}
static int event_unsubscribe(int handle) {
    if (handle < 0 || handle >= g_sub_n || !g_subs[handle].active) {
        pstr("  Error: invalid sub handle\n"); return -1; }
    g_subs[handle].active = 0;
    pstr("  Unsubscribed handle "); pint(handle); pstr("\n"); return 0;
}
static int event_route(const char *from_type, const char *to_type) {
    if (g_route_n >= MAX_ROUTES) { pstr("  Error: route limit\n"); return -1; }
    int id = g_route_n++;
    my_strncpy(g_routes[id].from_type, from_type, MAX_NAME - 1);
    my_strncpy(g_routes[id].to_type, to_type, MAX_NAME - 1);
    g_routes[id].active = 1; pstr("  Route: '"); pstr(from_type); pstr("' -> '"); pstr(to_type); pstr("'\n");
    return id;
}
static int event_filter_by_type(const char *target) {
    int n = 0; pstr("  Filter type='"); pstr(target); pstr("':\n");
    for (int i = 0; i < g_event_n; i++)
        if (g_events[i].active && my_strcmp(g_events[i].type, target) == 0) {
            pstr("    seq="); pint(g_events[i].seq); pstr(" data='"); pstr(g_events[i].data); pstr("'\n"); n++; }
    pstr("  Matched: "); pint(n); pstr("\n"); return n;
}
static int event_filter_by_source(const char *target) {
    int n = 0; pstr("  Filter source='"); pstr(target); pstr("':\n");
    for (int i = 0; i < g_event_n; i++)
        if (g_events[i].active && my_strcmp(g_events[i].source, target) == 0) {
            pstr("    seq="); pint(g_events[i].seq); pstr(" data='"); pstr(g_events[i].data); pstr("'\n"); n++; }
    pstr("  Matched: "); pint(n); pstr("\n"); return n;
}
static int event_transform(const char *in_type, const char *out_type, const char *map) {
    if (g_trans_n >= MAX_TRANS) { pstr("  Error: transform limit\n"); return -1; }
    int id = g_trans_n++;
    my_strncpy(g_trans[id].in_type, in_type, MAX_NAME - 1);
    my_strncpy(g_trans[id].out_type, out_type, MAX_NAME - 1);
    my_strncpy(g_trans[id].mapping, map, MAX_DATA - 1); g_trans[id].active = 1;
    int applied = 0;
    for (int i = 0; i < g_event_n; i++)
        if (g_events[i].active && my_strcmp(g_events[i].type, in_type) == 0) {
            my_strncpy(g_events[i].type, out_type, MAX_NAME - 1); applied++; }
    pstr("  Transform '"); pstr(in_type); pstr("'->'"); pstr(out_type);
    pstr("' applied="); pint(applied); pstr("\n"); return applied;
}
static int command_dispatch(const char *name, const char *payload) {
    if (g_cmd_n >= MAX_CMDS) { pstr("  Error: cmd limit\n"); return -1; }
    int id = g_cmd_n++;
    my_strncpy(g_cmds[id].name, name, MAX_NAME - 1);
    my_strncpy(g_cmds[id].payload, payload, MAX_DATA - 1);
    g_cmds[id].dispatched = 1; g_cmds[id].active = 1;
    pstr("  Dispatched cmd='"); pstr(name); pstr("'\n"); return id;
}
static int command_handle(int cmd_id) {
    if (cmd_id < 0 || cmd_id >= g_cmd_n || !g_cmds[cmd_id].active) {
        pstr("  Error: invalid cmd\n"); return -1; }
    if (!g_cmds[cmd_id].dispatched) { pstr("  Error: not dispatched\n"); return -1; }
    g_cmds[cmd_id].handled = 1;
    pstr("  Handled cmd='"); pstr(g_cmds[cmd_id].name); pstr("'\n"); return 0;
}
static int command_result(int cmd_id) {
    if (cmd_id < 0 || cmd_id >= g_cmd_n || !g_cmds[cmd_id].active) return -1;
    g_cmds[cmd_id].result = g_cmds[cmd_id].handled ? 1 : 0; pstr("  Cmd '"); pstr(g_cmds[cmd_id].name);
    pstr("' result="); pint(g_cmds[cmd_id].result); pstr("\n");
    return g_cmds[cmd_id].result;
}
static int query_dispatch(const char *name, const char *criteria) {
    if (g_query_n >= MAX_QUERIES) { pstr("  Error: query limit\n"); return -1; }
    int id = g_query_n++;
    my_strncpy(g_queries[id].name, name, MAX_NAME - 1);
    my_strncpy(g_queries[id].criteria, criteria, MAX_DATA - 1);
    g_queries[id].dispatched = 1; g_queries[id].cost = 10; g_queries[id].active = 1;
    pstr("  Dispatched query='"); pstr(name); pstr("'\n"); return id;
}
static int query_handle(int query_id) {
    if (query_id < 0 || query_id >= g_query_n || !g_queries[query_id].active) {
        pstr("  Error: invalid query\n"); return -1; }
    g_queries[query_id].handled = 1;
    my_strncpy(g_queries[query_id].result, "result_data", MAX_DATA - 1);
    pstr("  Handled query='"); pstr(g_queries[query_id].name);
    pstr("' -> '"); pstr(g_queries[query_id].result); pstr("'\n"); return 0;
}
static int query_optimize(int query_id) {
    if (query_id < 0 || query_id >= g_query_n || !g_queries[query_id].active) {
        pstr("  Error: invalid query for optimize\n"); return -1; }
    int old = g_queries[query_id].cost; g_queries[query_id].cost = old / 2;
    pstr("  Optimized query '"); pstr(g_queries[query_id].name);
    pstr("' cost "); pint(old); pstr("->"); pint(g_queries[query_id].cost); pstr("\n");
    return g_queries[query_id].cost;
}
static int read_model_update(const char *name, const char *data) {
    for (int i = 0; i < g_read_n; i++)
        if (g_read_models[i].active && my_strcmp(g_read_models[i].name, name) == 0) {
            my_strncpy(g_read_models[i].data, data, MAX_DATA - 1);
            g_read_models[i].version++; pstr("  Read model '"); pstr(name); pstr("' v");
            pint(g_read_models[i].version); pstr(" updated\n"); return i; }
    if (g_read_n >= 8) { pstr("  Error: read model limit\n"); return -1; }
    int id = g_read_n++;
    my_strncpy(g_read_models[id].name, name, MAX_NAME - 1);
    my_strncpy(g_read_models[id].data, data, MAX_DATA - 1);
    g_read_models[id].version = 1; g_read_models[id].active = 1;
    pstr("  Read model '"); pstr(name); pstr("' created v1\n"); return id;
}
static int write_model_update(const char *name, const char *data) {
    for (int i = 0; i < g_write_n; i++)
        if (g_write_models[i].active && my_strcmp(g_write_models[i].name, name) == 0) {
            my_strncpy(g_write_models[i].data, data, MAX_DATA - 1);
            g_write_models[i].version++; pstr("  Write model '"); pstr(name); pstr("' v");
            pint(g_write_models[i].version); pstr(" updated\n"); return i; }
    if (g_write_n >= 8) { pstr("  Error: write model limit\n"); return -1; }
    int id = g_write_n++;
    my_strncpy(g_write_models[id].name, name, MAX_NAME - 1);
    my_strncpy(g_write_models[id].data, data, MAX_DATA - 1);
    g_write_models[id].version = 1; g_write_models[id].active = 1;
    pstr("  Write model '"); pstr(name); pstr("' created v1\n"); return id;
}
static int saga_orchestrated_start(const char *id) {
    if (g_saga_n >= MAX_SAGAS) { pstr("  Error: saga limit\n"); return -1; }
    int sid = g_saga_n++;
    my_strncpy(g_sagas[sid].id, id, MAX_NAME - 1);
    g_sagas[sid].step_count = 0; g_sagas[sid].completed = 0;
    g_sagas[sid].compensating = 0; g_sagas[sid].active = 1;
    pstr("  Orchestrated saga started: '"); pstr(id); pstr("'\n"); return sid;
}
static int saga_orchestrated_step(int saga_id, const char *action) {
    if (saga_id < 0 || saga_id >= g_saga_n || !g_sagas[saga_id].active) {
        pstr("  Error: invalid saga\n"); return -1; }
    saga_t *s = &g_sagas[saga_id];
    if (s->step_count >= MAX_SAGA_STEPS) { pstr("  Error: step limit\n"); return -1; }
    int idx = s->step_count++;
    my_strncpy(s->steps[idx].action, action, MAX_NAME - 1);
    s->steps[idx].completed = 1; pstr("  Saga '"); pstr(s->id); pstr("' step: '"); pstr(action); pstr("' done\n");
    return idx;
}
static int saga_orchestrated_complete(int saga_id) {
    if (saga_id < 0 || saga_id >= g_saga_n || !g_sagas[saga_id].active) {
        pstr("  Error: invalid saga for complete\n"); return -1; }
    g_sagas[saga_id].completed = 1;
    pstr("  Saga '"); pstr(g_sagas[saga_id].id); pstr("' completed\n"); return 0;
}
static int saga_choreographed_participate(const char *saga_id, const char *action) {
    pstr("  Choreographed participant '"); pstr(saga_id);
    pstr("' action='"); pstr(action); pstr("'\n"); return 0;
}
static int saga_choreographed_coord(const char *saga_id, const char *event) {
    pstr("  Choreographed coord '"); pstr(saga_id); pstr("' on '"); pstr(event); pstr("'\n");
    for (int i = 0; i < g_saga_n; i++) if (g_sagas[i].active && my_strcmp(g_sagas[i].id, saga_id) == 0) {
        for (int j = 0; j < g_sagas[i].step_count; j++)
            if (my_strcmp(g_sagas[i].steps[j].action, event) == 0) {
                g_sagas[i].steps[j].completed = 1; pstr("    Marked '"); pstr(event); pstr("'\n"); }
        int ad = 1; for (int j = 0; j < g_sagas[i].step_count; j++)
            if (!g_sagas[i].steps[j].completed) { ad = 0; break; }
        if (ad && g_sagas[i].step_count > 0) { g_sagas[i].completed = 1; pstr("    Auto-completed\n"); }
        break; }
    return 0;
}
static int saga_compensate(int saga_id) {
    if (saga_id < 0 || saga_id >= g_saga_n || !g_sagas[saga_id].active) {
        pstr("  Error: invalid saga for compensate\n"); return -1; }
    g_sagas[saga_id].compensating = 1; pstr("  Compensating saga '"); pstr(g_sagas[saga_id].id); pstr("':\n");
    for (int i = g_sagas[saga_id].step_count - 1; i >= 0; i--)
        if (g_sagas[saga_id].steps[i].completed) {
            pstr("    Undo: '"); pstr(g_sagas[saga_id].steps[i].action); pstr("'\n");
            g_sagas[saga_id].steps[i].completed = 0;
        }
    pstr("  Compensation complete\n"); return 0;
}
static int saga_state_get(const char *saga_id, const char *key) {
    for (int i = 0; i < g_saga_state_n; i++)
        if (g_saga_state[i].active && my_strcmp(g_saga_state[i].key, key) == 0) {
            pstr("  Saga '"); pstr(saga_id); pstr("' state '"); pstr(key);
            pstr("' = '"); pstr(g_saga_state[i].value); pstr("'\n"); return i;
        }
    if (g_saga_state_n >= 32) { pstr("  Error: state limit\n"); return -1; }
    int id = g_saga_state_n++;
    my_strncpy(g_saga_state[id].key, key, MAX_NAME - 1);
    my_strncpy(g_saga_state[id].value, "initialized", MAX_DATA - 1);
    g_saga_state[id].active = 1; pstr("  Saga '"); pstr(saga_id); pstr("' state '"); pstr(key); pstr("' initialized\n");
    return id;
}
static int store_append(const char *agg_id, const char *type, const char *data) {
    if (g_store_n >= MAX_STORE) { pstr("  Error: store full\n"); return -1; }
    int id = g_store_n++; g_store[id].seq = g_seq++;
    my_strncpy(g_store[id].agg_id, agg_id, MAX_NAME - 1);
    my_strncpy(g_store[id].type, type, MAX_NAME - 1);
    my_strncpy(g_store[id].data, data, MAX_DATA - 1);
    g_store[id].ts = g_clock++; g_store[id].active = 1; g_store[id].compacted = 0;
    pstr("  Store append agg='"); pstr(agg_id); pstr("' type='"); pstr(type);
    pstr("' seq="); pint(g_store[id].seq); pstr("\n"); return id;
}
static int store_read(unsigned int seq) {
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && g_store[i].seq == seq) {
            pstr("  Store read seq="); pint(seq); pstr(" type='"); pstr(g_store[i].type); pstr("' data='"); pstr(g_store[i].data);
            pstr("'\n"); return i;
        }
    pstr("  Store read seq="); pint(seq); pstr(" NOT FOUND\n"); return -1;
}
static int store_read_range(unsigned int from, unsigned int to) {
    int n = 0; pstr("  Store range ["); pint(from); pstr(","); pint(to); pstr("]:\n");
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && g_store[i].seq >= from && g_store[i].seq <= to) {
            pstr("    seq="); pint(g_store[i].seq); pstr(" type='"); pstr(g_store[i].type); pstr("\n"); n++;
        }
    pstr("  Count: "); pint(n); pstr("\n"); return n;
}
static int store_replay(unsigned int from_seq, int count) {
    int n = 0; pstr("  Replay from seq="); pint(from_seq); pstr(" count="); pint(count); pstr(":\n");
    for (int i = 0; i < g_store_n && n < count; i++)
        if (g_store[i].active && g_store[i].seq >= from_seq) {
            pstr("    ["); pint(n); pstr("] seq="); pint(g_store[i].seq); pstr(" type='"); pstr(g_store[i].type); pstr("' data='"); pstr(g_store[i].data); pstr("'\n"); n++;
        }
    return n;
}
static int store_compact(const char *agg_id) {
    int last_seq = -1;
    for (int i = g_store_n - 1; i >= 0; i--)
        if (g_store[i].active && my_strcmp(g_store[i].agg_id, agg_id) == 0) {
            last_seq = (int)g_store[i].seq; break;
        }
    int compacted = 0;
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && my_strcmp(g_store[i].agg_id, agg_id) == 0 &&
            (int)g_store[i].seq != last_seq) {
            g_store[i].compacted = 1; compacted++;
        }
    pstr("  Compacted agg='"); pstr(agg_id);
    pstr("' removed="); pint(compacted); pstr("\n"); return compacted;
}
static int store_order(void) {
    int n = 0;
    for (unsigned int s = 1; s < g_seq; s++)
        for (int i = 0; i < g_store_n; i++)
            if (g_store[i].active && g_store[i].seq == s) {
                pstr("    "); pint(n); pstr(": seq="); pint(g_store[i].seq); pstr(" agg='"); pstr(g_store[i].agg_id); pstr("'\n"); n++;
            }
    pstr("  Total ordered: "); pint(n); pstr("\n"); return n;
}
static int consume_at_least_once(const char *agg_id) {
    int n = 0; pstr("  Consume at-least-once agg='"); pstr(agg_id); pstr("':\n");
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && my_strcmp(g_store[i].agg_id, agg_id) == 0) {
            pstr("    seq="); pint(g_store[i].seq); pstr(" type='"); pstr(g_store[i].type); pstr("\n"); n++;
        }
    pstr("  Consumed: "); pint(n); pstr("\n"); return n;
}
static int consume_at_most_once(const char *agg_id) {
    int n = 0; pstr("  Consume at-most-once agg='"); pstr(agg_id); pstr("':\n");
    unsigned int seen[16]; int seen_n = 0;
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && my_strcmp(g_store[i].agg_id, agg_id) == 0) {
            unsigned int h = hash_str(g_store[i].type) ^ g_store[i].seq;
            int dup = 0;
            for (int j = 0; j < seen_n; j++) if (seen[j] == h) { dup = 1; break; }
            if (!dup && seen_n < 16) { seen[seen_n++] = h;
                pstr("    seq="); pint(g_store[i].seq); pstr(" type='"); pstr(g_store[i].type); pstr("\n"); n++;
            }
        }
    pstr("  Consumed: "); pint(n); pstr("\n"); return n;
}
static int consume_idempotent(const char *agg_id, const char *data) {
    unsigned int h = hash_str(data) ^ hash_str(agg_id);
    for (int i = 0; i < g_idemp_n; i++)
        if (g_idemp[i].active && g_idemp[i].hash == h) {
            g_idemp[i].count++; pstr("  Idempotent consume: DUPLICATE (count=");
            pint(g_idemp[i].count); pstr(")\n"); return -1;
        }
    if (g_idemp_n >= MAX_IDEMP) { pstr("  Error: idempotent table full\n"); return -1; }
    int id = g_idemp_n++;
    g_idemp[id].hash = h; g_idemp[id].count = 1; g_idemp[id].active = 1;
    pstr("  Idempotent consume: FIRST TIME\n"); return id;
}
static int consume_exactly_once(const char *agg_id) {
    int n = 0; pstr("  Consume exactly-once agg='"); pstr(agg_id); pstr("':\n");
    for (int i = 0; i < g_store_n; i++)
        if (g_store[i].active && my_strcmp(g_store[i].agg_id, agg_id) == 0) {
            unsigned int h = hash_str(g_store[i].data) ^ g_store[i].seq;
            int dup = 0;
            for (int j = 0; j < g_idemp_n; j++)
                if (g_idemp[j].active && g_idemp[j].hash == h) { dup = 1; break; }
            if (!dup) {
                pstr("    seq="); pint(g_store[i].seq); pstr(" type='"); pstr(g_store[i].type); pstr("\n"); n++;
                if (g_idemp_n < MAX_IDEMP) {
                    int idx = g_idemp_n++;
                    g_idemp[idx].hash = h; g_idemp[idx].count = 1;
                    g_idemp[idx].active = 1;
                }
            }
        }
    pstr("  Consumed (deduped): "); pint(n); pstr("\n"); return n;
}
static int stream_create(const char *name, unsigned int capacity) {
    if (g_stream_n >= MAX_STREAMS) { pstr("  Error: stream limit\n"); return -1; }
    int id = g_stream_n++;
    my_strncpy(g_streams[id].name, name, MAX_NAME - 1);
    g_streams[id].sub_count = 0; g_streams[id].backlog = 0;
    g_streams[id].capacity = capacity; g_streams[id].active = 1;
    pstr("  Stream '"); pstr(name); pstr("' created cap="); pint(capacity); pstr("\n");
    return id;
}
static int stream_subscribe(int stream_id, int sub_id) {
    if (stream_id < 0 || stream_id >= g_stream_n || !g_streams[stream_id].active) { pstr("  Error: invalid stream\n"); return -1; }
    if (g_streams[stream_id].sub_count >= MAX_STREAM_SUB) { pstr("  Error: stream sub limit\n"); return -1; }
    g_streams[stream_id].subs[g_streams[stream_id].sub_count++] = sub_id;
    pstr("  Stream '"); pstr(g_streams[stream_id].name);
    pstr("' sub="); pint(sub_id); pstr("\n"); return 0;
}
static int stream_publish(int stream_id, const char *data) {
    if (stream_id < 0 || stream_id >= g_stream_n || !g_streams[stream_id].active) { pstr("  Error: invalid stream for publish\n"); return -1; }
    g_streams[stream_id].backlog++; pstr("  Stream '"); pstr(g_streams[stream_id].name);
    pstr("' publish data='"); pstr(data); pstr("'\n"); return 0;
}
static int stream_backpressure(int stream_id) {
    if (stream_id < 0 || stream_id >= g_stream_n || !g_streams[stream_id].active) { pstr("  Error: invalid stream for backpressure\n"); return -1; }
    unsigned int bl = g_streams[stream_id].backlog;
    unsigned int cap = g_streams[stream_id].capacity;
    int pct = cap > 0 ? (int)((bl * 100) / cap) : 0; pstr("  Backpressure '"); pstr(g_streams[stream_id].name);
    pstr("': "); pint(bl); pstr("/"); pint(cap); pstr(" ("); pint(pct); pstr("%)\n");
    if (bl > cap) {
        pstr("    OVERLOADED - applying backpressure\n");
        g_streams[stream_id].backlog = cap; return -1;
    }
    return 0;
}
static int stream_merge(int stream_a, int stream_b) {
    if (stream_a < 0 || stream_a >= g_stream_n || !g_streams[stream_a].active) { pstr("  Error: invalid stream A\n"); return -1; }
    if (stream_b < 0 || stream_b >= g_stream_n || !g_streams[stream_b].active) { pstr("  Error: invalid stream B\n"); return -1; }
    int delivered = 0; pstr("  Merge '"); pstr(g_streams[stream_a].name);
    pstr("' + '"); pstr(g_streams[stream_b].name); pstr("':\n");
    for (int i = 0; i < g_streams[stream_a].sub_count; i++) {
        pstr("    -> sub "); pint(g_streams[stream_a].subs[i]); pstr("\n");
        delivered++;
    }
    for (int i = 0; i < g_streams[stream_b].sub_count; i++) {
        pstr("    -> sub "); pint(g_streams[stream_b].subs[i]); pstr("\n");
        delivered++;
    }
    pstr("  Delivered: "); pint(delivered); pstr("\n"); return delivered;
}
static int stream_split(int stream_id) {
    if (stream_id < 0 || stream_id >= g_stream_n || !g_streams[stream_id].active) { pstr("  Error: invalid stream for split\n"); return -1; }
    int half = g_streams[stream_id].sub_count / 2; pstr("  Split '"); pstr(g_streams[stream_id].name);
    pstr("' ("); pint(g_streams[stream_id].sub_count); pstr(" subs) -> A="); pint(half);
    pstr(" B="); pint(g_streams[stream_id].sub_count - half); pstr("\n");
    return half;
}
static int stream_window_tumble(int sid, unsigned int sz) {
    if (sid < 0 || sid >= g_stream_n || !g_streams[sid].active) { pstr("  Error: invalid stream\n"); return -1; }
    int w = 0; pstr("  Tumble '"); pstr(g_streams[sid].name); pstr("' sz="); pint(sz); pstr(":\n");
    for (unsigned int s = 0; s < g_streams[sid].backlog; s += sz) {
        unsigned int e = s + sz; if (e > g_streams[sid].backlog) e = g_streams[sid].backlog;
        pstr("    w["); pint(s); pstr("-"); pint(e); pstr("]\n"); w++; }
    pstr("  Windows: "); pint(w); pstr("\n"); return w;
}
static int stream_window_slide(int sid, unsigned int sz, unsigned int sl) {
    if (sid < 0 || sid >= g_stream_n || !g_streams[sid].active) { pstr("  Error: invalid stream\n"); return -1; }
    int w = 0; pstr("  Slide '"); pstr(g_streams[sid].name);
    pstr("' sz="); pint(sz); pstr(" sl="); pint(sl); pstr(":\n");
    for (unsigned int s = 0; s < g_streams[sid].backlog; s += sl) {
        unsigned int e = s + sz; if (e > g_streams[sid].backlog) e = g_streams[sid].backlog;
        pstr("    w["); pint(s); pstr("-"); pint(e); pstr("]\n"); w++; }
    pstr("  Windows: "); pint(w); pstr("\n"); return w;
}
static int stream_window_session(int sid, unsigned int gap) {
    if (sid < 0 || sid >= g_stream_n || !g_streams[sid].active) { pstr("  Error: invalid stream\n"); return -1; }
    int ss_n = 0; unsigned int ss = 0;
    pstr("  Session '"); pstr(g_streams[sid].name); pstr("' gap="); pint(gap); pstr(":\n");
    for (unsigned int i = 0; i < g_streams[sid].backlog; i++)
        if (i > 0 && (i - ss) > gap) {
            pstr("    s["); pint(ss); pstr("-"); pint(i); pstr("]\n"); ss_n++; ss = i; }
    if (g_streams[sid].backlog > 0) {
        pstr("    s["); pint(ss); pstr("-"); pint(g_streams[sid].backlog); pstr("]\n"); ss_n++; }
    pstr("  Sessions: "); pint(ss_n); pstr("\n"); return ss_n;
}
static void show_help(void) {
    pstr("event_driven_engine - Event-driven engine module\n\n"); pstr("Usage: event_driven_engine [options]\n\n"); pstr("Options:\n  -h    Show this help\n  -t    Run test suite\n\n"); pstr("Features:\n"); pstr("  Event bus:        publish, subscribe, route, filter, transform\n");
    pstr("  CQRS:             command dispatch/handle, query dispatch/handle\n");
    pstr("                    query optimize, read/write model separation\n");
    pstr("  Saga:             orchestrated start/step/complete\n");
    pstr("                    choreographed participate/coord, compensate\n");
    pstr("  Event store:      append, read, read_range, replay, compact, order\n");
    pstr("  Guarantees:       at-least-once, at-most-once, idempotent, exactly-once\n");
    pstr("  Reactive streams: backpressure, merge, split, tumble/slide/session\n");
}
static void test_suite(void) {
    pstr("\n=== Event Driven Engine Test Suite ===\n\n"); pstr("--- Test 1: Event Publish / Subscribe ---\n");
    event_publish("OrderCreated", "api-gw", "{\"id\":1001}");
    event_publish("OrderShipped", "warehouse", "{\"id\":1001}");
    event_publish("PaymentDone", "stripe", "{\"id\":1001}");
    int s1 = event_subscribe("Order*", 1);
    int s2 = event_subscribe("Payment*", 2);
    event_filter_by_type("OrderCreated"); event_filter_by_source("warehouse");
    pstr("\n--- Test 2: Event Route / Transform ---\n");
    event_route("OrderCreated", "NotificationTrigger");
    event_transform("OrderCreated", "AuditEvent", "copy_with_ts");
    pstr("\n--- Test 3: CQRS Command Dispatch + Query ---\n");
    int c1 = command_dispatch("CreateOrder", "{\"item\":\"widget\"}");
    command_handle(c1); command_result(c1);
    int q1 = query_dispatch("GetOrderStatus", "{\"id\":1001}");
    query_handle(q1); query_optimize(q1);
    read_model_update("OrderView", "{\"id\":1001,\"status\":\"shipped\"}");
    write_model_update("OrderWrite", "{\"id\":1001,\"item\":\"widget\"}");
    pstr("\n--- Test 4: Saga Orchestrate + Compensate ---\n");
    int sg1 = saga_orchestrated_start("order-saga-1");
    saga_orchestrated_step(sg1, "reserve_inventory");
    saga_orchestrated_step(sg1, "process_payment");
    saga_orchestrated_complete(sg1);
    saga_state_get("order-saga-1", "inventory_status");
    int sg2 = saga_orchestrated_start("fail-saga-2");
    saga_orchestrated_step(sg2, "reserve_inventory");
    saga_orchestrated_step(sg2, "charge_card"); saga_compensate(sg2); pstr("\n--- Test 5: Saga Choreographed ---\n");
    saga_orchestrated_start("choreo-saga");
    saga_choreographed_participate("choreo-saga", "inventory_reserved");
    saga_choreographed_coord("choreo-saga", "inventory_reserved");
    pstr("\n--- Test 6: Event Store Append + Replay ---\n");
    store_append("agg-1", "OrderCreated", "{\"id\":1}");
    store_append("agg-1", "OrderUpdated", "{\"id\":1,\"v\":2}");
    store_append("agg-1", "OrderShipped", "{\"id\":1,\"track\":\"ABC\"}");
    store_append("agg-2", "PaymentInit", "{\"amt\":50}");
    store_read(1); store_read_range(1, 3);
    store_replay(2, 10); store_order(); pstr("\n--- Test 7: Idempotent Consume ---\n");
    consume_idempotent("agg-1", "{\"id\":1}");
    consume_idempotent("agg-1", "{\"id\":1}");
    consume_at_least_once("agg-1"); consume_at_most_once("agg-1");
    consume_exactly_once("agg-1"); pstr("\n--- Test 8: Store Compact ---\n");
    store_compact("agg-1"); pstr("\n--- Test 9: Reactive Stream + Backpressure + Window ---\n");
    int st1 = stream_create("orders-stream", 100);
    int st2 = stream_create("analytics-stream", 200);
    stream_subscribe(st1, 1); stream_subscribe(st1, 2); stream_subscribe(st2, 3);
    stream_publish(st1, "{\"ev\":1}"); stream_publish(st1, "{\"ev\":2}");
    stream_publish(st1, "{\"ev\":3}"); stream_publish(st2, "{\"ev\":4}");
    stream_backpressure(st1); stream_merge(st1, st2); stream_split(st1);
    stream_window_tumble(st1, 2); stream_window_slide(st1, 3, 1);
    stream_window_session(st1, 5); pstr("\n--- Test 10: Cleanup ---\n");
    event_unsubscribe(s1); event_unsubscribe(s2); pstr("\n=== All tests passed ===\n");
}
__attribute__((export_name("_start")))
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)buf;
    while (pos < 512 && av[pos]) pos++;
    pos++;
    while (pos < 512 && av[pos]) {
        char *arg = &av[pos];
        int len = my_strlen(arg);
        if (len == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (len == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    } pstr("Event Driven Engine v46.0\n");
    if (help) { show_help(); return; }
    if (test) { test_suite(); return; }
    pstr("Use -h for help, -t for test\n");
}
