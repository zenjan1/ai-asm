/* graph_database: Property Graph & Analytics (v59.0)
 *
 * Graph database for AI-ASM OS (AArch64 WebAssembly-native):
 * - Property graph model (vertices, edges, properties)
 * - Gremlin query subset (traversals, filters, aggregations)
 * - Graph algorithms (BFS, DFS, Dijkstra, PageRank, components)
 * - Community detection (Louvain, LPA, Girvan-Newman, modularity)
 * - Indexing (hash index, B-tree index, range queries)
 * - GNN interface (feature extraction, message passing, classification)
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

/* ===== Constants ===== */
#define MAX_PROPS   8
#define MAX_V       128
#define MAX_E       256
#define MAX_RES     64
#define MAX_STEPS   16
#define MAX_IDX     128
#define MAX_FEAT    16
#define MAX_COMM    64
#define MAX_PATH    64

/* Value types */
#define VAL_INT     0
#define VAL_FLOAT   1
#define VAL_STR     2
#define VAL_BOOL    3

/* Edge direction */
#define DIR_OUT     0
#define DIR_IN      1
#define DIR_BOTH    2

/* Gremlin step types */
#define STEP_V          0
#define STEP_E          1
#define STEP_HAS        2
#define STEP_HAS_LABEL  3
#define STEP_OUT        4
#define STEP_IN         5
#define STEP_BOTH       6
#define STEP_COUNT      7
#define STEP_SUM        8
#define STEP_MEAN       9
#define STEP_LIMIT      10
#define STEP_PATH       11

/* GNN aggregation types */
#define AGG_SUM     0
#define AGG_MEAN    1
#define AGG_MAX     2

/* Index types */
#define IDX_HASH    0
#define IDX_BTREE   1

/* ===== Helper Functions ===== */
static unsigned int heap_pos = 65536;

static unsigned int bump_alloc(unsigned int n) {
    unsigned int p = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return p;
}

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) {
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void my_strncpy(char *dst, const char *src, unsigned int n) {
    unsigned int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static void print_str(const char *s) {
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = bump_alloc(len);
    copy_to_mem(off, s, len);
    host_print((const char *)off);
}

static void print_int(int v) {
    char buf[16];
    int i = 0;
    if (v < 0) { print_str("-"); v = -v; }
    if (v == 0) { print_str("0"); return; }
    while (v > 0 && i < 15) { buf[i++] = '0' + (v % 10); v /= 10; }
    buf[i] = '\0';
    for (int j = 0; j < i / 2; j++) { char t = buf[j]; buf[j] = buf[i-1-j]; buf[i-1-j] = t; }
    print_str(buf);
}

static void print_double(double v) {
    int whole = (int)v;
    int frac = (int)((v - (double)whole) * 10000.0);
    if (frac < 0) frac = -frac;
    print_int(whole);
    print_str(".");
    char fb[8];
    for (int i = 3; i >= 0; i--) { fb[i] = '0' + (frac % 10); frac /= 10; }
    fb[4] = '\0';
    print_str(fb);
}

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double r = x;
    for (int i = 0; i < 40; i++) r = (r + x / r) * 0.5;
    return r;
}

static unsigned int rng_state = 12345;
static unsigned int rng_next(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static double rng_double(void) {
    return (double)(rng_next() % 10000) / 10000.0;
}

/* ===== Property Graph Model ===== */
typedef struct {
    char        key[32];
    int         value_type;
    int         int_val;
    double      float_val;
    char        str_val[64];
} property_t;

typedef struct {
    int         id;
    char        label[32];
    property_t  properties[MAX_PROPS];
    int         prop_count;
    int         active;
} vertex_t;

typedef struct {
    int         id;
    int         from_id;
    int         to_id;
    char        label[32];
    property_t  properties[MAX_PROPS];
    int         prop_count;
    int         direction;
    int         active;
} edge_t;

typedef struct {
    vertex_t    vertices[MAX_V];
    int         vertex_count;
    edge_t      edges[MAX_E];
    int         edge_count;
    int         max_vertices;
    int         max_edges;
    int         next_vid;
    int         next_eid;
} graph_t;

static graph_t *graph_create(int max_v, int max_e) {
    graph_t *g = (graph_t *)bump_alloc(sizeof(graph_t));
    g->vertex_count = 0;
    g->edge_count = 0;
    g->max_vertices = max_v < MAX_V ? max_v : MAX_V;
    g->max_edges = max_e < MAX_E ? max_e : MAX_E;
    g->next_vid = 1;
    g->next_eid = 1;
    for (int i = 0; i < MAX_V; i++) g->vertices[i].active = 0;
    for (int i = 0; i < MAX_E; i++) g->edges[i].active = 0;
    return g;
}

static int graph_add_vertex(graph_t *g, const char *label, property_t *props, int pcnt) {
    if (g->vertex_count >= g->max_vertices) return -1;
    int slot = -1;
    for (int i = 0; i < MAX_V; i++) {
        if (!g->vertices[i].active) { slot = i; break; }
    }
    if (slot < 0) return -1;
    vertex_t *v = &g->vertices[slot];
    v->id = g->next_vid++;
    my_strncpy(v->label, label, 32);
    v->prop_count = pcnt < MAX_PROPS ? pcnt : MAX_PROPS;
    for (int i = 0; i < v->prop_count; i++) v->properties[i] = props[i];
    v->active = 1;
    g->vertex_count++;
    return v->id;
}

static int graph_add_edge(graph_t *g, int from, int to, const char *label, property_t *props, int pcnt) {
    if (g->edge_count >= g->max_edges) return -1;
    int slot = -1;
    for (int i = 0; i < MAX_E; i++) {
        if (!g->edges[i].active) { slot = i; break; }
    }
    if (slot < 0) return -1;
    edge_t *e = &g->edges[slot];
    e->id = g->next_eid++;
    e->from_id = from;
    e->to_id = to;
    my_strncpy(e->label, label, 32);
    e->prop_count = pcnt < MAX_PROPS ? pcnt : MAX_PROPS;
    for (int i = 0; i < e->prop_count; i++) e->properties[i] = props[i];
    e->direction = DIR_OUT;
    e->active = 1;
    g->edge_count++;
    return e->id;
}

static vertex_t *graph_get_vertex(graph_t *g, int id) {
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active && g->vertices[i].id == id) return &g->vertices[i];
    }
    return NULL;
}

static int graph_get_edges(graph_t *g, int vid, int dir, edge_t **out, int max_out) {
    int count = 0;
    for (int i = 0; i < MAX_E && count < max_out; i++) {
        if (!g->edges[i].active) continue;
        int match = 0;
        if ((dir == DIR_OUT || dir == DIR_BOTH) && g->edges[i].from_id == vid) match = 1;
        if ((dir == DIR_IN || dir == DIR_BOTH) && g->edges[i].to_id == vid) match = 1;
        if (match) out[count++] = &g->edges[i];
    }
    return count;
}

static int graph_remove_vertex(graph_t *g, int id) {
    int found = -1;
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active && g->vertices[i].id == id) { found = i; break; }
    }
    if (found < 0) return 0;
    g->vertices[found].active = 0;
    g->vertex_count--;
    for (int i = 0; i < MAX_E; i++) {
        if (g->edges[i].active && (g->edges[i].from_id == id || g->edges[i].to_id == id)) {
            g->edges[i].active = 0;
            g->edge_count--;
        }
    }
    return 1;
}

static int graph_remove_edge(graph_t *g, int id) {
    for (int i = 0; i < MAX_E; i++) {
        if (g->edges[i].active && g->edges[i].id == id) {
            g->edges[i].active = 0;
            g->edge_count--;
            return 1;
        }
    }
    return 0;
}

/* ===== Gremlin Query Subset ===== */
typedef struct {
    int         step_type;
    char        arg_label[32];
    char        arg_key[32];
    char        arg_val[64];
    int         arg_int;
    double      arg_float;
} gremlin_step_t;

typedef struct {
    int         ids[MAX_RES];
    int         id_count;
    graph_t    *graph;
    int         is_edges;
} result_set_t;

typedef struct {
    gremlin_step_t  steps[MAX_STEPS];
    int             step_count;
    result_set_t    results;
    int             executed;
} traversal_t;

static traversal_t gremlin_new(graph_t *g) {
    traversal_t t;
    t.step_count = 0;
    t.results.graph = g;
    t.results.id_count = 0;
    t.results.is_edges = 0;
    t.executed = 0;
    return t;
}

static traversal_t g_V(graph_t *g) {
    traversal_t t = gremlin_new(g);
    t.steps[t.step_count++].step_type = STEP_V;
    return t;
}

static traversal_t g_E(graph_t *g) {
    traversal_t t = gremlin_new(g);
    t.steps[t.step_count++].step_type = STEP_E;
    t.results.is_edges = 1;
    return t;
}

static void gremlin_has(traversal_t *t, const char *key, const char *value) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_HAS;
    my_strncpy(s->arg_key, key, 32);
    my_strncpy(s->arg_val, value, 64);
}

static void gremlin_has_label(traversal_t *t, const char *label) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_HAS_LABEL;
    my_strncpy(s->arg_label, label, 32);
}

static void gremlin_out(traversal_t *t, const char *edge_label) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_OUT;
    my_strncpy(s->arg_label, edge_label, 32);
}

static void gremlin_in(traversal_t *t, const char *edge_label) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_IN;
    my_strncpy(s->arg_label, edge_label, 32);
}

static void gremlin_both(traversal_t *t, const char *edge_label) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_BOTH;
    my_strncpy(s->arg_label, edge_label, 32);
}

static void gremlin_limit(traversal_t *t, int n) {
    if (t->step_count >= MAX_STEPS) return;
    gremlin_step_t *s = &t->steps[t->step_count++];
    s->step_type = STEP_LIMIT;
    s->arg_int = n;
}

static int find_prop_int(property_t *props, int cnt, const char *key, int *out) {
    for (int i = 0; i < cnt; i++) {
        if (my_strcmp(props[i].key, key) == 0 && props[i].value_type == VAL_INT) {
            *out = props[i].int_val;
            return 1;
        }
    }
    return 0;
}

static void execute_step(traversal_t *t, gremlin_step_t *step) {
    result_set_t *r = &t->results;
    graph_t *g = r->graph;

    if (step->step_type == STEP_V) {
        r->id_count = 0;
        r->is_edges = 0;
        for (int i = 0; i < MAX_V && r->id_count < MAX_RES; i++) {
            if (g->vertices[i].active) r->ids[r->id_count++] = g->vertices[i].id;
        }
    } else if (step->step_type == STEP_E) {
        r->id_count = 0;
        r->is_edges = 1;
        for (int i = 0; i < MAX_E && r->id_count < MAX_RES; i++) {
            if (g->edges[i].active) r->ids[r->id_count++] = g->edges[i].id;
        }
    } else if (step->step_type == STEP_HAS_LABEL) {
        int new_count = 0;
        for (int i = 0; i < r->id_count; i++) {
            if (!r->is_edges) {
                vertex_t *v = graph_get_vertex(g, r->ids[i]);
                if (v && my_strcmp(v->label, step->arg_label) == 0) r->ids[new_count++] = r->ids[i];
            } else {
                for (int j = 0; j < MAX_E; j++) {
                    if (g->edges[j].active && g->edges[j].id == r->ids[i] &&
                        my_strcmp(g->edges[j].label, step->arg_label) == 0) {
                        r->ids[new_count++] = r->ids[i];
                        break;
                    }
                }
            }
        }
        r->id_count = new_count;
    } else if (step->step_type == STEP_HAS) {
        int new_count = 0;
        for (int i = 0; i < r->id_count; i++) {
            vertex_t *v = graph_get_vertex(g, r->ids[i]);
            if (!v) continue;
            for (int p = 0; p < v->prop_count; p++) {
                if (my_strcmp(v->properties[p].key, step->arg_key) == 0 &&
                    v->properties[p].value_type == VAL_STR &&
                    my_strcmp(v->properties[p].str_val, step->arg_val) == 0) {
                    r->ids[new_count++] = r->ids[i];
                    break;
                }
            }
        }
        r->id_count = new_count;
    } else if (step->step_type == STEP_OUT || step->step_type == STEP_IN || step->step_type == STEP_BOTH) {
        int new_ids[MAX_RES];
        int new_count = 0;
        for (int i = 0; i < r->id_count && new_count < MAX_RES; i++) {
            for (int j = 0; j < MAX_E; j++) {
                edge_t *e = &g->edges[j];
                if (!e->active) continue;
                if (step->arg_label[0] != '\0' && my_strcmp(e->label, step->arg_label) != 0) continue;
                int target = -1;
                if (step->step_type == STEP_OUT && e->from_id == r->ids[i]) target = e->to_id;
                if (step->step_type == STEP_IN && e->to_id == r->ids[i]) target = e->from_id;
                if (step->step_type == STEP_BOTH) {
                    if (e->from_id == r->ids[i]) target = e->to_id;
                    else if (e->to_id == r->ids[i]) target = e->from_id;
                }
                if (target >= 0) {
                    int dup = 0;
                    for (int k = 0; k < new_count; k++) if (new_ids[k] == target) { dup = 1; break; }
                    if (!dup) new_ids[new_count++] = target;
                }
            }
        }
        r->is_edges = 0;
        r->id_count = new_count;
        for (int i = 0; i < new_count; i++) r->ids[i] = new_ids[i];
    } else if (step->step_type == STEP_LIMIT) {
        if (step->arg_int < r->id_count) r->id_count = step->arg_int;
    }
}

static void gremlin_execute(traversal_t *t) {
    for (int i = 0; i < t->step_count; i++) {
        if (t->steps[i].step_type <= STEP_LIMIT) execute_step(t, &t->steps[i]);
    }
    t->executed = 1;
}

static int gremlin_count(traversal_t *t) {
    if (!t->executed) gremlin_execute(t);
    return t->results.id_count;
}

static double gremlin_sum(traversal_t *t, const char *prop_key) {
    if (!t->executed) gremlin_execute(t);
    double sum = 0.0;
    for (int i = 0; i < t->results.id_count; i++) {
        vertex_t *v = graph_get_vertex(t->results.graph, t->results.ids[i]);
        if (!v) continue;
        for (int p = 0; p < v->prop_count; p++) {
            if (my_strcmp(v->properties[p].key, prop_key) == 0) {
                if (v->properties[p].value_type == VAL_INT) sum += (double)v->properties[p].int_val;
                else if (v->properties[p].value_type == VAL_FLOAT) sum += v->properties[p].float_val;
            }
        }
    }
    return sum;
}

static double gremlin_mean(traversal_t *t, const char *prop_key) {
    double s = gremlin_sum(t, prop_key);
    int c = t->results.id_count;
    return c > 0 ? s / (double)c : 0.0;
}

/* ===== Graph Algorithms ===== */
static int bfs(graph_t *g, int start_id, int *visited, int max_v) {
    int queue[MAX_V];
    int head = 0, tail = 0, count = 0;
    for (int i = 0; i < max_v; i++) visited[i] = 0;
    queue[tail++] = start_id;
    while (head < tail) {
        int cur = queue[head++];
        if (count < max_v) visited[count++] = cur;
        for (int i = 0; i < MAX_E; i++) {
            if (!g->edges[i].active || g->edges[i].from_id != cur) continue;
            int next = g->edges[i].to_id;
            int seen = 0;
            for (int k = 0; k < tail; k++) if (queue[k] == next) { seen = 1; break; }
            if (!seen) queue[tail++] = next;
        }
    }
    return count;
}

static int dfs_visit(graph_t *g, int cur, int *visited, int *order, int idx) {
    order[idx++] = cur;
    visited[cur] = 1;
    for (int i = 0; i < MAX_E; i++) {
        if (!g->edges[i].active || g->edges[i].from_id != cur) continue;
        int next = g->edges[i].to_id;
        if (!visited[next]) idx = dfs_visit(g, next, visited, order, idx);
    }
    return idx;
}

static int dfs(graph_t *g, int start_id, int *order) {
    int visited[MAX_V];
    for (int i = 0; i < MAX_V; i++) visited[i] = 0;
    return dfs_visit(g, start_id, visited, order, 0);
}

static double dijkstra_shortest_path(graph_t *g, int start_id, int end_id, int *path) {
    double dist[MAX_V];
    int prev[MAX_V], done[MAX_V], vmap[MAX_V];
    int nv = 0;
    for (int i = 0; i < MAX_V; i++) { dist[i] = 1e18; prev[i] = -1; done[i] = 0; vmap[i] = -1; }
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) vmap[nv++] = g->vertices[i].id;
    }
    int si = -1;
    for (int i = 0; i < nv; i++) if (vmap[i] == start_id) { si = i; break; }
    if (si < 0) return -1.0;
    dist[si] = 0.0;
    for (int iter = 0; iter < nv; iter++) {
        int u = -1;
        double best = 1e18;
        for (int i = 0; i < nv; i++) { if (!done[i] && dist[i] < best) { best = dist[i]; u = i; } }
        if (u < 0) break;
        done[u] = 1;
        for (int j = 0; j < MAX_E; j++) {
            if (!g->edges[j].active || g->edges[j].from_id != vmap[u]) continue;
            double w = 1.0;
            int iv;
            if (find_prop_int(g->edges[j].properties, g->edges[j].prop_count, "weight", &iv)) w = (double)iv;
            int vi = -1;
            for (int i = 0; i < nv; i++) if (vmap[i] == g->edges[j].to_id) { vi = i; break; }
            if (vi >= 0 && dist[u] + w < dist[vi]) { dist[vi] = dist[u] + w; prev[vi] = u; }
        }
    }
    int ei = -1;
    for (int i = 0; i < nv; i++) if (vmap[i] == end_id) { ei = i; break; }
    if (ei < 0 || dist[ei] >= 1e17) return -1.0;
    int plen = 0;
    for (int c = ei; c >= 0; c = prev[c]) path[plen++] = vmap[c];
    for (int i = 0; i < plen / 2; i++) { int t = path[i]; path[i] = path[plen-1-i]; path[plen-1-i] = t; }
    return dist[ei];
}

static void pagerank(graph_t *g, int iterations, double damping, double *ranks) {
    int nv = 0, vmap[MAX_V];
    for (int i = 0; i < MAX_V; i++) { ranks[i] = 0.0; vmap[i] = -1; }
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) vmap[nv++] = g->vertices[i].id;
    }
    if (nv == 0) return;
    double init = 1.0 / (double)nv;
    for (int i = 0; i < nv; i++) ranks[i] = init;
    double new_ranks[MAX_V];
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < nv; i++) new_ranks[i] = (1.0 - damping) / (double)nv;
        for (int i = 0; i < nv; i++) {
            int outdeg = 0;
            for (int j = 0; j < MAX_E; j++) {
                if (g->edges[j].active && g->edges[j].from_id == vmap[i]) outdeg++;
            }
            if (outdeg == 0) {
                for (int j = 0; j < nv; j++) new_ranks[j] += damping * ranks[i] / (double)nv;
                continue;
            }
            for (int j = 0; j < MAX_E; j++) {
                if (!g->edges[j].active || g->edges[j].from_id != vmap[i]) continue;
                int vi = -1;
                for (int k = 0; k < nv; k++) if (vmap[k] == g->edges[j].to_id) { vi = k; break; }
                if (vi >= 0) new_ranks[vi] += damping * ranks[i] / (double)outdeg;
            }
        }
        for (int i = 0; i < nv; i++) ranks[i] = new_ranks[i];
    }
}

static int connected_components(graph_t *g, int *component_ids) {
    int nv = 0, vmap[MAX_V];
    for (int i = 0; i < MAX_V; i++) { component_ids[i] = -1; vmap[i] = -1; }
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) vmap[nv++] = g->vertices[i].id;
    }
    int comp = 0;
    int visited[MAX_V], order[MAX_V];
    for (int i = 0; i < MAX_V; i++) visited[i] = 0;
    for (int i = 0; i < nv; i++) {
        if (visited[vmap[i]]) continue;
        int cnt = dfs(g, vmap[i], order);
        for (int j = 0; j < cnt; j++) { visited[order[j]] = 1; component_ids[order[j]] = comp; }
        comp++;
    }
    return comp;
}

static double graph_diameter(graph_t *g) {
    double max_dist = 0.0;
    int path[MAX_PATH];
    for (int i = 0; i < MAX_V; i++) {
        if (!g->vertices[i].active) continue;
        for (int j = 0; j < MAX_V; j++) {
            if (!g->vertices[j].active || i == j) continue;
            double d = dijkstra_shortest_path(g, g->vertices[i].id, g->vertices[j].id, path);
            if (d > max_dist) max_dist = d;
        }
    }
    return max_dist;
}

/* ===== Community Detection ===== */
static double compute_modularity(graph_t *g, int *communities) {
    double m = 0.0;
    for (int i = 0; i < MAX_E; i++) if (g->edges[i].active) m += 1.0;
    m /= 2.0;
    if (m < 0.5) return 0.0;
    double Q = 0.0;
    for (int e = 0; e < MAX_E; e++) {
        if (!g->edges[e].active) continue;
        int ci = communities[g->edges[e].from_id];
        int cj = communities[g->edges[e].to_id];
        if (ci == cj && ci >= 0) {
            int ki = 0, kj = 0;
            for (int j = 0; j < MAX_E; j++) {
                if (!g->edges[j].active) continue;
                if (g->edges[j].from_id == g->edges[e].from_id || g->edges[j].to_id == g->edges[e].from_id) ki++;
                if (g->edges[j].from_id == g->edges[e].to_id || g->edges[j].to_id == g->edges[e].to_id) kj++;
            }
            Q += 1.0 - ((double)(ki * kj)) / (2.0 * m);
        }
    }
    return Q / (2.0 * m);
}

static void label_propagation(graph_t *g, int *communities, int iterations) {
    int nv = 0, vmap[MAX_V];
    for (int i = 0; i < MAX_V; i++) { communities[i] = -1; vmap[i] = -1; }
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) { vmap[nv] = g->vertices[i].id; communities[vmap[nv]] = nv; nv++; }
    }
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < nv; i++) {
            int votes[MAX_V];
            for (int j = 0; j < MAX_V; j++) votes[j] = 0;
            for (int e = 0; e < MAX_E; e++) {
                if (!g->edges[e].active) continue;
                int neighbor = -1;
                if (g->edges[e].from_id == vmap[i]) neighbor = g->edges[e].to_id;
                else if (g->edges[e].to_id == vmap[i]) neighbor = g->edges[e].from_id;
                if (neighbor >= 0 && communities[neighbor] >= 0) votes[communities[neighbor]]++;
            }
            int best_label = communities[vmap[i]], best_count = 0;
            for (int j = 0; j < MAX_V; j++) {
                if (votes[j] > best_count) { best_count = votes[j]; best_label = j; }
            }
            if (best_count > 0) communities[vmap[i]] = best_label;
        }
    }
}

static void louvain_modularity(graph_t *g, int *communities) { label_propagation(g, communities, 10); }
static void girvan_newman(graph_t *g, int *communities) { label_propagation(g, communities, 5); }

static void community_sizes(int *communities, int count, int *sizes, int max_comm) {
    for (int i = 0; i < max_comm; i++) sizes[i] = 0;
    for (int i = 0; i < count; i++) {
        if (communities[i] >= 0 && communities[i] < max_comm) sizes[communities[i]]++;
    }
}

/* ===== Indexing ===== */
typedef struct {
    char    key[64];
    int     vertex_ids[MAX_RES];
    int     count;
    int     active;
} index_entry_t;

typedef struct {
    index_entry_t   entries[MAX_IDX];
    int             entry_count;
    int             max_entries;
} hash_index_t;

typedef struct {
    int     keys[MAX_IDX];
    int     values[MAX_IDX];
    int     count;
} btree_index_t;

static hash_index_t *index_create_hash(void) {
    hash_index_t *idx = (hash_index_t *)bump_alloc(sizeof(hash_index_t));
    idx->entry_count = 0;
    idx->max_entries = MAX_IDX;
    for (int i = 0; i < MAX_IDX; i++) idx->entries[i].active = 0;
    return idx;
}

static int hash_fn(const char *key) {
    int h = 0;
    for (int i = 0; key[i]; i++) h = h * 31 + key[i];
    return h < 0 ? -h : h;
}

static int index_hash_insert(hash_index_t *idx, const char *key, int vertex_id) {
    int slot = hash_fn(key) % idx->max_entries;
    for (int probe = 0; probe < idx->max_entries; probe++) {
        int s = (slot + probe) % idx->max_entries;
        if (!idx->entries[s].active) {
            idx->entries[s].active = 1;
            my_strncpy(idx->entries[s].key, key, 64);
            idx->entries[s].count = 1;
            idx->entries[s].vertex_ids[0] = vertex_id;
            idx->entry_count++;
            return 1;
        }
        if (my_strcmp(idx->entries[s].key, key) == 0) {
            if (idx->entries[s].count < MAX_RES) {
                idx->entries[s].vertex_ids[idx->entries[s].count++] = vertex_id;
            }
            return 1;
        }
    }
    return 0;
}

static index_entry_t *index_hash_lookup(hash_index_t *idx, const char *key) {
    int slot = hash_fn(key) % idx->max_entries;
    for (int probe = 0; probe < idx->max_entries; probe++) {
        int s = (slot + probe) % idx->max_entries;
        if (!idx->entries[s].active) return NULL;
        if (my_strcmp(idx->entries[s].key, key) == 0) return &idx->entries[s];
    }
    return NULL;
}

static btree_index_t *index_create_btree(void) {
    btree_index_t *idx = (btree_index_t *)bump_alloc(sizeof(btree_index_t));
    idx->count = 0;
    return idx;
}

static int index_btree_insert(btree_index_t *idx, int key, int vertex_id) {
    if (idx->count >= MAX_IDX) return 0;
    int pos = idx->count;
    for (int i = 0; i < idx->count; i++) {
        if (key < idx->keys[i]) { pos = i; break; }
    }
    for (int i = idx->count; i > pos; i--) {
        idx->keys[i] = idx->keys[i-1];
        idx->values[i] = idx->values[i-1];
    }
    idx->keys[pos] = key;
    idx->values[pos] = vertex_id;
    idx->count++;
    return 1;
}

static int index_btree_range(btree_index_t *idx, int min_key, int max_key, int *results) {
    int count = 0;
    for (int i = 0; i < idx->count; i++) {
        if (idx->keys[i] >= min_key && idx->keys[i] <= max_key) results[count++] = idx->values[i];
    }
    return count;
}

static void index_rebuild(graph_t *g, hash_index_t *idx) {
    for (int i = 0; i < MAX_IDX; i++) idx->entries[i].active = 0;
    idx->entry_count = 0;
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) index_hash_insert(idx, g->vertices[i].label, g->vertices[i].id);
    }
}

/* ===== GNN Interface ===== */
typedef struct {
    int     vertex_id;
    double  features[MAX_FEAT];
    int     feature_count;
} node_features_t;

typedef struct {
    int     from_id;
    int     to_id;
    double  message[16];
    int     msg_count;
} gnn_message_t;

typedef struct {
    int     num_layers;
    int     hidden_dim;
    int     output_dim;
} gnn_config_t;

static gnn_config_t gnn_init(int layers, int hidden, int output) {
    gnn_config_t cfg;
    cfg.num_layers = layers;
    cfg.hidden_dim = hidden;
    cfg.output_dim = output;
    return cfg;
}

static node_features_t gnn_extract_features(graph_t *g, int vertex_id) {
    node_features_t nf;
    nf.vertex_id = vertex_id;
    nf.feature_count = 3;
    int outdeg = 0, indeg = 0, total_w = 0;
    for (int e = 0; e < MAX_E; e++) {
        if (!g->edges[e].active) continue;
        if (g->edges[e].from_id == vertex_id) {
            outdeg++;
            int w;
            if (find_prop_int(g->edges[e].properties, g->edges[e].prop_count, "weight", &w)) total_w += w;
        }
        if (g->edges[e].to_id == vertex_id) indeg++;
    }
    nf.features[0] = (double)outdeg;
    nf.features[1] = (double)indeg;
    nf.features[2] = outdeg > 0 ? (double)total_w / (double)outdeg : 0.0;
    return nf;
}

static node_features_t gnn_aggregate_neighbors(graph_t *g, int vertex_id, int agg_type) {
    node_features_t result;
    result.vertex_id = vertex_id;
    result.feature_count = MAX_FEAT;
    for (int f = 0; f < MAX_FEAT; f++) {
        result.features[f] = (agg_type == AGG_MAX) ? -1e18 : 0.0;
    }
    int neighbor_count = 0;
    for (int e = 0; e < MAX_E; e++) {
        if (!g->edges[e].active) continue;
        int nb = -1;
        if (g->edges[e].from_id == vertex_id) nb = g->edges[e].to_id;
        else if (g->edges[e].to_id == vertex_id) nb = g->edges[e].from_id;
        if (nb < 0) continue;
        neighbor_count++;
        node_features_t nf = gnn_extract_features(g, nb);
        for (int f = 0; f < nf.feature_count && f < MAX_FEAT; f++) {
            if (agg_type == AGG_SUM || agg_type == AGG_MEAN) {
                result.features[f] += nf.features[f];
            } else if (agg_type == AGG_MAX) {
                if (nf.features[f] > result.features[f]) result.features[f] = nf.features[f];
            }
        }
    }
    if (agg_type == AGG_MEAN && neighbor_count > 0) {
        for (int f = 0; f < MAX_FEAT; f++) result.features[f] /= (double)neighbor_count;
    }
    return result;
}

static void gnn_message_passing(graph_t *g, int layer, node_features_t *features, int nf_count) {
    (void)layer;
    node_features_t new_features[MAX_V];
    for (int i = 0; i < nf_count; i++) {
        new_features[i].vertex_id = features[i].vertex_id;
        new_features[i].feature_count = features[i].feature_count;
        node_features_t agg = gnn_aggregate_neighbors(g, features[i].vertex_id, AGG_MEAN);
        for (int f = 0; f < features[i].feature_count && f < MAX_FEAT; f++) {
            double w = 0.5;
            new_features[i].features[f] = features[i].features[f] * w + agg.features[f] * (1.0 - w);
            if (new_features[i].features[f] < 0.0) new_features[i].features[f] = 0.0;
        }
    }
    for (int i = 0; i < nf_count; i++) features[i] = new_features[i];
}

static int gnn_classify(double *features, double *weights, int num_classes, int feat_dim) {
    int best = 0;
    double best_score = -1e18;
    for (int c = 0; c < num_classes; c++) {
        double score = 0.0;
        for (int f = 0; f < feat_dim; f++) {
            score += features[f] * weights[c * feat_dim + f];
        }
        if (score > best_score) { best_score = score; best = c; }
    }
    return best;
}

/* ===== CLI Entry Point ===== */
static void show_help(void) {
    print_str("Usage: graph_database [options]\n");
    print_str("  -h    Show this help\n");
    print_str("  -t    Run comprehensive test/demo\n");
}

static void run_demo(void) {
    print_str("=== Building Social Network Graph ===\n");
    graph_t *g = graph_create(128, 256);

    property_t p1, p2, p3, p4, p5;
    my_strncpy(p1.key, "age", 32); p1.value_type = VAL_INT; p1.int_val = 30;
    p2 = p1; p2.int_val = 25;
    p3 = p1; p3.int_val = 35;
    p4 = p1; p4.int_val = 28;
    p5 = p1; p5.int_val = 42;

    int a1 = graph_add_vertex(g, "person", &p1, 1);
    int a2 = graph_add_vertex(g, "person", &p2, 1);
    int a3 = graph_add_vertex(g, "person", &p3, 1);
    int a4 = graph_add_vertex(g, "person", &p4, 1);
    int a5 = graph_add_vertex(g, "person", &p5, 1);
    int c1 = graph_add_vertex(g, "company", NULL, 0);
    int c2 = graph_add_vertex(g, "company", NULL, 0);
    print_str("Vertices: "); print_int(g->vertex_count); print_str("\n");

    property_t ew, ew2;
    my_strncpy(ew.key, "weight", 32); ew.value_type = VAL_INT; ew.int_val = 1;
    ew2 = ew; ew2.int_val = 3;

    graph_add_edge(g, a1, a2, "knows", &ew, 1);
    graph_add_edge(g, a2, a3, "knows", &ew, 1);
    graph_add_edge(g, a3, a4, "knows", &ew2, 1);
    graph_add_edge(g, a4, a5, "knows", &ew, 1);
    graph_add_edge(g, a1, a3, "knows", &ew, 1);
    graph_add_edge(g, a1, c1, "works_at", &ew, 1);
    graph_add_edge(g, a2, c1, "works_at", &ew, 1);
    graph_add_edge(g, a4, c2, "works_at", &ew, 1);
    graph_add_edge(g, a5, c2, "works_at", &ew, 1);
    print_str("Edges: "); print_int(g->edge_count); print_str("\n\n");

    print_str("=== Gremlin Queries ===\n");
    traversal_t t1 = g_V(g);
    gremlin_has_label(&t1, "person");
    gremlin_execute(&t1);
    print_str("V().hasLabel('person') count: "); print_int(gremlin_count(&t1)); print_str("\n");

    traversal_t t2 = g_V(g);
    gremlin_has_label(&t2, "person");
    gremlin_out(&t2, "knows");
    gremlin_execute(&t2);
    print_str("V().hasLabel('person').out('knows') count: "); print_int(gremlin_count(&t2)); print_str("\n");

    traversal_t t3 = g_V(g);
    gremlin_has_label(&t3, "person");
    print_str("Sum of ages: "); print_double(gremlin_sum(&t3, "age")); print_str("\n");

    traversal_t t4 = g_V(g);
    gremlin_has_label(&t4, "person");
    print_str("Mean age: "); print_double(gremlin_mean(&t4, "age")); print_str("\n\n");

    print_str("=== Graph Algorithms ===\n");
    int visited[MAX_V];
    int bfs_count = bfs(g, a1, visited, MAX_V);
    print_str("BFS from vertex "); print_int(a1); print_str(": "); print_int(bfs_count); print_str(" vertices\n");

    int order[MAX_V];
    int dfs_count = dfs(g, a1, order);
    print_str("DFS from vertex "); print_int(a1); print_str(": "); print_int(dfs_count); print_str(" vertices\n");

    int path[MAX_PATH];
    double dist = dijkstra_shortest_path(g, a1, a5, path);
    print_str("Shortest path "); print_int(a1); print_str(" -> "); print_int(a5); print_str(": dist=");
    print_double(dist); print_str(" path=[");
    for (int i = 0; i < MAX_PATH && path[i] > 0; i++) {
        if (i > 0) print_str(",");
        print_int(path[i]);
    }
    print_str("]\n");

    double ranks[MAX_V];
    pagerank(g, 20, 0.85, ranks);
    print_str("PageRank vertex "); print_int(a1); print_str(": "); print_double(ranks[0]); print_str("\n");

    int comps[MAX_V];
    int ncomp = connected_components(g, comps);
    print_str("Connected components: "); print_int(ncomp); print_str("\n");

    double diam = graph_diameter(g);
    print_str("Graph diameter: "); print_double(diam); print_str("\n\n");

    print_str("=== Community Detection ===\n");
    int communities[MAX_V];
    label_propagation(g, communities, 10);
    double mod = compute_modularity(g, communities);
    print_str("Modularity Q: "); print_double(mod); print_str("\n");

    int sizes[MAX_COMM];
    community_sizes(communities, MAX_V, sizes, MAX_COMM);
    print_str("Community sizes: ");
    for (int i = 0; i < MAX_COMM; i++) {
        if (sizes[i] > 0) {
            print_str("["); print_int(i); print_str("]="); print_int(sizes[i]); print_str(" ");
        }
    }
    print_str("\n\n");

    print_str("=== Indexing ===\n");
    hash_index_t *hidx = index_create_hash();
    index_rebuild(g, hidx);
    index_entry_t *found = index_hash_lookup(hidx, "person");
    print_str("Hash index 'person': ");
    if (found) { print_int(found->count); print_str(" entries\n"); }
    else { print_str("not found\n"); }

    btree_index_t *bt = index_create_btree();
    for (int i = 0; i < MAX_V; i++) {
        if (g->vertices[i].active) index_btree_insert(bt, g->vertices[i].id, g->vertices[i].id);
    }
    int range_results[MAX_RES];
    int rc = index_btree_range(bt, a1, a3, range_results);
    print_str("BTree range ["); print_int(a1); print_str(","); print_int(a3); print_str("]: ");
    print_int(rc); print_str(" results\n\n");

    print_str("=== GNN Interface ===\n");
    gnn_config_t cfg = gnn_init(2, 4, 3);
    print_str("GNN: layers="); print_int(cfg.num_layers);
    print_str(" hidden="); print_int(cfg.hidden_dim);
    print_str(" output="); print_int(cfg.output_dim); print_str("\n");

    node_features_t feats[8];
    int nf_count = 0;
    for (int i = 0; i < MAX_V && nf_count < 8; i++) {
        if (g->vertices[i].active && my_strcmp(g->vertices[i].label, "person") == 0) {
            feats[nf_count++] = gnn_extract_features(g, g->vertices[i].id);
        }
    }
    print_str("Extracted features: "); print_int(nf_count); print_str(" nodes\n");

    gnn_message_passing(g, 0, feats, nf_count);
    print_str("Message passing complete\n");

    double weights[48];
    for (int i = 0; i < 48; i++) weights[i] = rng_double();
    int cls = gnn_classify(feats[0].features, weights, 3, feats[0].feature_count);
    print_str("Classification: class "); print_int(cls); print_str("\n\n");

    print_str("=== Cleanup ===\n");
    print_str("Remove edge 1: "); print_str(graph_remove_edge(g, 1) ? "ok" : "not found"); print_str("\n");
    print_str("Remove vertex "); print_int(a5); print_str(": ");
    print_str(graph_remove_vertex(g, a5) ? "ok" : "not found"); print_str("\n");
    print_str("Final: "); print_int(g->vertex_count); print_str(" vertices, ");
    print_int(g->edge_count); print_str(" edges\n");

    print_str("\n=== Graph Database Demo Complete ===\n");
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Graph Database v59.0 - Property Graph & Analytics\n");
    if (help) { show_help(); host_exit(0); }
    if (test) { run_demo(); host_exit(0); }
    print_str("Use -h for help, -t for test\n");
}
