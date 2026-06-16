/* pipeline_runner: CI/CD pipeline runner module (v1.0)
 * Tekton/Actions/Jenkins-style: stages, steps, plugins, caching,
 * approval flows, artifact management, notification dispatch. */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ══════ Limits ══════ */
#define MAX_PIPES      8
#define MAX_STAGES     8
#define MAX_STEPS      8
#define MAX_PLUGINS   16
#define MAX_CACHE     32
#define MAX_APPROVE    8
#define MAX_ARTIFACTS 16
#define MAX_NOTIF     32
#define MAX_DEPN       8
#define MAX_NAME      64
#define MAX_CMD      128
#define MAX_PARAM     64
#define MAX_LOG      256

/* ══════ Pipeline States ══════ */
#define PIPE_INIT      0
#define PIPE_RUNNING   1
#define PIPE_SUCCESS   2
#define PIPE_FAILED    3
#define PIPE_CANCELLED 4

/* ══════ Stage / Step States ══════ */
#define ST_INIT    0
#define ST_RUN     1
#define ST_DONE    2
#define ST_FAIL    3
#define ST_SKIP    4

/* ══════ Plugin Types ══════ */
#define TASK_BUILD   0
#define TASK_TEST    1
#define TASK_DEPLOY  2

/* ══════ Condition Types ══════ */
#define COND_ALWAYS  0
#define COND_NEVER   1
#define COND_BRANCH  2
#define COND_EXPR    3

/* ══════ Approval States ══════ */
#define APR_WAIT     0
#define APR_APPROVED 1
#define APR_REJECTED 2
#define APR_TIMEOUT  3

/* ══════ Artifact Types ══════ */
#define ART_IMAGE  0
#define ART_HELM   1
#define ART_SBOM   2

/* ══════ Notification Types ══════ */
#define NOTIF_SLACK 0
#define NOTIF_EMAIL 1
#define NOTIF_GIT   2

/* ══════ Cache Status ══════ */
#define CCH_EMPTY 0
#define CCH_VALID 1

/* ══════ Structs ══════ */
typedef struct {
    int id, status;
    char name[MAX_NAME];
    int stage_count, step_count;
} pipeline_t;

typedef struct {
    int id, pipeline_id, parallel, condition_type;
    char name[MAX_NAME], condition_val[MAX_PARAM];
    int status, dep_mask;
} stage_t;

typedef struct {
    int id, stage_id, retries, timeout, status, attempt;
    char name[MAX_NAME], cmd[MAX_CMD];
} step_t;

typedef struct {
    int type, param_count;
    char name[MAX_NAME];
    char pkey[MAX_PARAM][MAX_NAME], pval[MAX_PARAM][MAX_CMD];
} plugin_t;

typedef struct {
    int status;
    char key[MAX_NAME], data[MAX_CMD];
} cache_t;

typedef struct {
    int id, pipeline_id, stage_id, status, timeout_sec, auto_flag;
} approve_t;

typedef struct {
    int type, signed_flag;
    char name[MAX_NAME], hash[MAX_NAME];
} artifact_t;

typedef struct {
    int type, sent;
    char target[MAX_NAME], msg[MAX_LOG];
} notif_t;

typedef struct {
    int node_count;
    char nodes[MAX_DEPN][MAX_NAME];
    int adj[MAX_DEPN][MAX_DEPN];
} depgraph_t;

/* ══════ Global State ══════ */
static pipeline_t g_pipes[MAX_PIPES];         static int g_pipe_n = 0;
static stage_t    g_stages[MAX_PIPES * MAX_STAGES]; static int g_stg_n = 0;
static step_t     g_steps[MAX_PIPES * MAX_STEPS];   static int g_step_n = 0;
static plugin_t   g_plugins[MAX_PLUGINS];     static int g_plug_n = 0;
static cache_t    g_cache[MAX_CACHE];         static int g_cache_n = 0;
static approve_t  g_approve[MAX_APPROVE];     static int g_aprv_n = 0;
static artifact_t g_arts[MAX_ARTIFACTS];      static int g_art_n = 0;
static notif_t    g_notifs[MAX_NOTIF];        static int g_notif_n = 0;
static depgraph_t g_graphs[MAX_DEPN];         static int g_graph_n = 0;
static int g_next_id = 1;

/* ══════ Utilities ══════ */
static int my_strlen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; } else {
        int d = 0, t = v; while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; }
    }
    host_print(b);
}
static void pn(void) { ps("\n"); }
static void pl(const char *s) { ps(s); pn(); }

/* Forward declarations */
static int stage_run(int idx);
static int step_run(int id);

/* ══════ Pipeline Core ══════ */
static int pipeline_create(const char *name) {
    if (g_pipe_n >= MAX_PIPES) { pl("[pipe] limit reached"); return -1; }
    pipeline_t *p = &g_pipes[g_pipe_n];
    p->id = g_next_id++; p->status = PIPE_INIT;
    my_strncpy(p->name, name, MAX_NAME - 1);
    p->stage_count = 0; p->step_count = 0;
    int id = p->id; g_pipe_n++;
    ps("[pipe] created id="); pi(id); ps(" name="); ps(name); pn();
    return id;
}

static pipeline_t *pipe_find(int id) {
    for (int i = 0; i < g_pipe_n; i++)
        if (g_pipes[i].id == id) return &g_pipes[i];
    return (pipeline_t *)0;
}

static int pipeline_run(int id) {
    pipeline_t *p = pipe_find(id);
    if (!p) { ps("[pipe] not found: "); pi(id); pn(); return -1; }
    p->status = PIPE_RUNNING;
    ps("[pipe] running: "); ps(p->name); pn();
    for (int i = 0; i < g_stg_n; i++) {
        if (g_stages[i].pipeline_id != id) continue;
        stage_run(g_stages[i].id);
        if (g_stages[i].status == ST_FAIL) {
            p->status = PIPE_FAILED;
            ps("[pipe] failed at stage: "); ps(g_stages[i].name); pn();
            return -1;
        }
    }
    p->status = PIPE_SUCCESS;
    ps("[pipe] completed: "); ps(p->name); pn();
    return 0;
}

static int pipeline_cancel(int id) {
    pipeline_t *p = pipe_find(id);
    if (!p) return -1;
    if (p->status != PIPE_RUNNING) return -1;
    p->status = PIPE_CANCELLED;
    ps("[pipe] cancelled: "); ps(p->name); pn();
    return 0;
}

static int pipeline_status(int id) {
    pipeline_t *p = pipe_find(id);
    if (!p) return -1;
    const char *sn = p->status == PIPE_INIT ? "INIT" :
        p->status == PIPE_RUNNING ? "RUNNING" :
        p->status == PIPE_SUCCESS ? "SUCCESS" :
        p->status == PIPE_FAILED ? "FAILED" : "CANCELLED";
    ps("[pipe] id="); pi(p->id); ps(" name="); ps(p->name);
    ps(" status="); ps(sn); ps(" stages="); pi(p->stage_count); pn();
    return p->status;
}

/* ══════ Stage Management ══════ */
static int stage_add(int pipe_id, const char *name, int parallel, int cond) {
    pipeline_t *p = pipe_find(pipe_id);
    if (!p) return -1;
    if (p->stage_count >= MAX_STAGES) return -1;
    stage_t *s = &g_stages[g_stg_n];
    s->id = g_next_id++; s->pipeline_id = pipe_id;
    my_strncpy(s->name, name, MAX_NAME - 1);
    s->parallel = parallel; s->condition_type = cond;
    s->condition_val[0] = '\0'; s->status = ST_INIT; s->dep_mask = 0;
    int idx = g_stg_n; g_stg_n++; p->stage_count++;
    ps("[stage] added: "); ps(name); ps(" pipe="); pi(pipe_id); pn();
    return idx;
}

static int stage_run(int idx) {
    if (idx < 0 || idx >= g_stg_n) return -1;
    stage_t *s = &g_stages[idx];
    if (s->status == ST_SKIP) {
        ps("[stage] skipped: "); ps(s->name); pn(); return 0;
    }
    s->status = ST_RUN;
    ps("[stage] running: "); ps(s->name);
    if (s->parallel) ps(" [parallel]");
    pn();
    int ok = 1;
    for (int i = 0; i < g_step_n; i++) {
        if (g_steps[i].stage_id != s->id) continue;
        if (step_run(i) != 0) ok = 0;
    }
    s->status = ok ? ST_DONE : ST_FAIL;
    ps("[stage] "); ps(s->name); ps(ok ? " -> DONE" : " -> FAIL"); pn();
    return ok ? 0 : -1;
}

static int stage_skip(int idx) {
    if (idx < 0 || idx >= g_stg_n) return -1;
    g_stages[idx].status = ST_SKIP;
    ps("[stage] skip: "); ps(g_stages[idx].name); pn();
    return 0;
}

static int stage_parallel(int idx, int flag) {
    if (idx < 0 || idx >= g_stg_n) return -1;
    g_stages[idx].parallel = flag;
    ps("[stage] "); ps(g_stages[idx].name);
    ps(flag ? " parallel=on" : " parallel=off"); pn();
    return 0;
}

/* ══════ Step Management ══════ */
static int step_add(int stg_id, const char *name, const char *cmd,
                    int retries, int timeout) {
    if (g_step_n >= MAX_PIPES * MAX_STEPS) return -1;
    for (int i = 0; i < g_step_n; i++)
        if (g_steps[i].stage_id == stg_id && !my_strcmp(g_steps[i].name, name))
            return -1;
    step_t *st = &g_steps[g_step_n];
    st->id = g_next_id++; st->stage_id = stg_id;
    my_strncpy(st->name, name, MAX_NAME - 1);
    my_strncpy(st->cmd, cmd, MAX_CMD - 1);
    st->retries = retries; st->timeout = timeout;
    st->status = ST_INIT; st->attempt = 0;
    int id = st->id; g_step_n++;
    ps("[step] added: "); ps(name); ps(" cmd="); ps(cmd); pn();
    return id;
}

static int step_run(int id) {
    for (int i = 0; i < g_step_n; i++) {
        if (g_steps[i].id != id) continue;
        step_t *st = &g_steps[i];
        int max_att = 1 + st->retries;
        ps("[step] run: "); ps(st->name);
        ps(" retries="); pi(st->retries); pn();
        int att;
        for (att = 0; att < max_att; att++) {
            st->attempt = att + 1;
            ps("[step]   attempt "); pi(att + 1); ps("/"); pi(max_att); pn();
            if (my_strcmp(st->cmd, "fail") == 0 && att < st->retries) {
                ps("[step]   retrying...\n");
                continue;
            }
            if (my_strcmp(st->cmd, "fail") == 0) {
                st->status = ST_FAIL;
                ps("[step]   FAILED\n");
                return -1;
            }
            st->status = ST_DONE;
            ps("[step]   OK\n");
            return 0;
        }
        st->status = ST_FAIL;
        return -1;
    }
    return -1;
}

static int step_retry(int id) {
    for (int i = 0; i < g_step_n; i++) {
        if (g_steps[i].id != id) continue;
        g_steps[i].status = ST_INIT;
        g_steps[i].attempt = 0;
        ps("[step] retry: "); ps(g_steps[i].name); pn();
        return step_run(id);
    }
    return -1;
}

/* ══════ Plugin System ══════ */
static int task_register_builder(const char *name) {
    if (g_plug_n >= MAX_PLUGINS) return -1;
    plugin_t *t = &g_plugins[g_plug_n];
    t->type = TASK_BUILD; my_strncpy(t->name, name, MAX_NAME - 1);
    t->param_count = 0;
    int idx = g_plug_n++;
    ps("[plugin] registered builder: "); ps(name); pn();
    return idx;
}

static int task_register_tester(const char *name) {
    if (g_plug_n >= MAX_PLUGINS) return -1;
    plugin_t *t = &g_plugins[g_plug_n];
    t->type = TASK_TEST; my_strncpy(t->name, name, MAX_NAME - 1);
    t->param_count = 0;
    int idx = g_plug_n++;
    ps("[plugin] registered tester: "); ps(name); pn();
    return idx;
}

static int task_register_deployer(const char *name) {
    if (g_plug_n >= MAX_PLUGINS) return -1;
    plugin_t *t = &g_plugins[g_plug_n];
    t->type = TASK_DEPLOY; my_strncpy(t->name, name, MAX_NAME - 1);
    t->param_count = 0;
    int idx = g_plug_n++;
    ps("[plugin] registered deployer: "); ps(name); pn();
    return idx;
}

static int task_invoke(const char *name) {
    for (int i = 0; i < g_plug_n; i++) {
        if (my_strcmp(g_plugins[i].name, name) == 0) {
            const char *tn = g_plugins[i].type == TASK_BUILD ? "builder" :
                g_plugins[i].type == TASK_TEST ? "tester" : "deployer";
            ps("[plugin] invoke: "); ps(name);
            ps(" type="); ps(tn);
            ps(" params="); pi(g_plugins[i].param_count); pn();
            return i;
        }
    }
    ps("[plugin] not found: "); ps(name); pn();
    return -1;
}

static int task_param_pass(const char *name, const char *key, const char *val) {
    for (int i = 0; i < g_plug_n; i++) {
        if (my_strcmp(g_plugins[i].name, name) != 0) continue;
        plugin_t *t = &g_plugins[i];
        if (t->param_count >= MAX_PARAM) return -1;
        int idx = t->param_count;
        my_strncpy(t->pkey[idx], key, MAX_NAME - 1);
        my_strncpy(t->pval[idx], val, MAX_CMD - 1);
        t->param_count++;
        ps("[plugin] param: "); ps(name); ps(" "); ps(key);
        ps("="); ps(val); pn();
        return 0;
    }
    return -1;
}

/* ══════ Condition System ══════ */
static int condition_evaluate(int ctype, const char *val) {
    if (ctype == COND_ALWAYS) return 1;
    if (ctype == COND_NEVER) return 0;
    if (ctype == COND_BRANCH) {
        if (my_strcmp(val, "main") == 0) return 1;
        if (my_strcmp(val, "master") == 0) return 1;
        return 0;
    }
    if (ctype == COND_EXPR) {
        if (my_strcmp(val, "success") == 0) return 1;
        return 0;
    }
    return 0;
}

static int condition_branch(int stg_idx, int ctype, const char *val) {
    if (stg_idx < 0 || stg_idx >= g_stg_n) return -1;
    int result = condition_evaluate(ctype, val);
    if (result) {
        ps("[cond] stage "); ps(g_stages[stg_idx].name);
        ps(": branch=EXECUTE\n");
        return 1;
    }
    g_stages[stg_idx].status = ST_SKIP;
    ps("[cond] stage "); ps(g_stages[stg_idx].name);
    ps(": branch=SKIP\n");
    return 0;
}

/* ══════ Dependency Graph ══════ */
static int dependency_graph_add(const char *name) {
    if (g_graph_n >= MAX_DEPN) return -1;
    depgraph_t *g = &g_graphs[g_graph_n];
    g->node_count = 0;
    for (int i = 0; i < MAX_DEPN; i++)
        for (int j = 0; j < MAX_DEPN; j++)
            g->adj[i][j] = 0;
    int idx = g_graph_n++;
    ps("[depgraph] created: "); ps(name); pn();
    return idx;
}

static int depgraph_add_node(int gid, const char *name) {
    if (gid < 0 || gid >= g_graph_n) return -1;
    depgraph_t *g = &g_graphs[gid];
    if (g->node_count >= MAX_DEPN) return -1;
    int idx = g->node_count;
    my_strncpy(g->nodes[idx], name, MAX_NAME - 1);
    g->node_count++;
    return idx;
}

static int depgraph_add_edge(int gid, int from, int to) {
    if (gid < 0 || gid >= g_graph_n) return -1;
    depgraph_t *g = &g_graphs[gid];
    if (from < 0 || from >= g->node_count) return -1;
    if (to < 0 || to >= g->node_count) return -1;
    g->adj[from][to] = 1;
    ps("[depgraph] edge: "); ps(g->nodes[from]);
    ps(" -> "); ps(g->nodes[to]); pn();
    return 0;
}

static int dependency_graph_resolve(int gid) {
    if (gid < 0 || gid >= g_graph_n) return -1;
    depgraph_t *g = &g_graphs[gid];
    int in_deg[MAX_DEPN];
    int i, j, e;
    for (i = 0; i < g->node_count; i++) {
        in_deg[i] = 0;
        for (j = 0; j < g->node_count; j++)
            if (g->adj[j][i]) in_deg[i]++;
    }
    int resolved = 0;
    for (i = 0; i < g->node_count; i++) {
        if (in_deg[i] == 0) {
            ps("[depgraph] root: "); ps(g->nodes[i]); pn();
            resolved++;
        }
    }
    ps("[depgraph] resolved: "); pi(resolved);
    ps("/"); pi(g->node_count); pn();
    return resolved;
}

static int dependency_graph_topo_sort(int gid) {
    if (gid < 0 || gid >= g_graph_n) return -1;
    depgraph_t *g = &g_graphs[gid];
    int in_deg[MAX_DEPN];
    int order[MAX_DEPN];
    int visited[MAX_DEPN];
    int i, j, cnt;
    for (i = 0; i < g->node_count; i++) {
        in_deg[i] = 0;
        visited[i] = 0;
        for (j = 0; j < g->node_count; j++)
            if (g->adj[j][i]) in_deg[i]++;
    }
    cnt = 0;
    while (cnt < g->node_count) {
        int found = 0;
        for (i = 0; i < g->node_count; i++) {
            if (!visited[i] && in_deg[i] == 0) {
                order[cnt++] = i;
                visited[i] = 1;
                found = 1;
                for (j = 0; j < g->node_count; j++)
                    if (g->adj[i][j]) in_deg[j]--;
                break;
            }
        }
        if (!found) break;
    }
    ps("[depgraph] topo: ");
    for (i = 0; i < cnt; i++) {
        if (i > 0) ps(" -> ");
        ps(g->nodes[order[i]]);
    }
    pn();
    return cnt;
}

/* ══════ Cache System ══════ */
static int cache_save(const char *key, const char *data) {
    int i;
    for (i = 0; i < g_cache_n; i++) {
        if (my_strcmp(g_cache[i].key, key) == 0) {
            my_strncpy(g_cache[i].data, data, MAX_CMD - 1);
            g_cache[i].status = CCH_VALID;
            ps("[cache] updated: "); ps(key); pn();
            return i;
        }
    }
    if (g_cache_n >= MAX_CACHE) return -1;
    cache_t *c = &g_cache[g_cache_n];
    c->status = CCH_VALID;
    my_strncpy(c->key, key, MAX_NAME - 1);
    my_strncpy(c->data, data, MAX_CMD - 1);
    int idx = g_cache_n++;
    ps("[cache] saved: "); ps(key); pn();
    return idx;
}

static int cache_load(const char *key, char *buf, int max_len) {
    int i;
    for (i = 0; i < g_cache_n; i++) {
        if (my_strcmp(g_cache[i].key, key) == 0 &&
            g_cache[i].status == CCH_VALID) {
            my_strncpy(buf, g_cache[i].data, max_len - 1);
            ps("[cache] hit: "); ps(key); pn();
            return 0;
        }
    }
    buf[0] = '\0';
    ps("[cache] miss: "); ps(key); pn();
    return -1;
}

static int cache_invalidate(const char *key) {
    int i;
    for (i = 0; i < g_cache_n; i++) {
        if (my_strcmp(g_cache[i].key, key) == 0) {
            g_cache[i].status = CCH_EMPTY;
            ps("[cache] invalidated: "); ps(key); pn();
            return 0;
        }
    }
    return -1;
}

static int cache_workspace_share(const char *src_key, const char *dst_key) {
    int i;
    for (i = 0; i < g_cache_n; i++) {
        if (my_strcmp(g_cache[i].key, src_key) == 0 &&
            g_cache[i].status == CCH_VALID) {
            cache_save(dst_key, g_cache[i].data);
            ps("[cache] workspace share: "); ps(src_key);
            ps(" -> "); ps(dst_key); pn();
            return 0;
        }
    }
    ps("[cache] share failed: "); ps(src_key); pn();
    return -1;
}

/* ══════ Approval Flows ══════ */
static int approval_request(int pipe_id, int stg_id, int timeout_s, int auto_flag) {
    if (g_aprv_n >= MAX_APPROVE) return -1;
    approve_t *a = &g_approve[g_aprv_n];
    a->id = g_next_id++; a->pipeline_id = pipe_id;
    a->stage_id = stg_id; a->status = APR_WAIT;
    a->timeout_sec = timeout_s; a->auto_flag = auto_flag;
    int id = a->id; g_aprv_n++;
    ps("[approval] request id="); pi(id);
    ps(" pipe="); pi(pipe_id); ps(" stage="); pi(stg_id); pn();
    return id;
}

static int approval_manual(int aprv_id, int approved) {
    int i;
    for (i = 0; i < g_aprv_n; i++) {
        if (g_approve[i].id != aprv_id) continue;
        if (g_approve[i].status != APR_WAIT) return -1;
        g_approve[i].status = approved ? APR_APPROVED : APR_REJECTED;
        ps("[approval] manual id="); pi(aprv_id);
        ps(approved ? " APPROVED" : " REJECTED"); pn();
        return 0;
    }
    return -1;
}

static int approval_auto(int aprv_id) {
    int i;
    for (i = 0; i < g_aprv_n; i++) {
        if (g_approve[i].id != aprv_id) continue;
        if (!g_approve[i].auto_flag) return -1;
        g_approve[i].status = APR_APPROVED;
        ps("[approval] auto-approved id="); pi(aprv_id); pn();
        return 0;
    }
    return -1;
}

static int approval_timeout(int aprv_id) {
    int i;
    for (i = 0; i < g_aprv_n; i++) {
        if (g_approve[i].id != aprv_id) continue;
        if (g_approve[i].status != APR_WAIT) return -1;
        g_approve[i].status = APR_TIMEOUT;
        ps("[approval] timeout id="); pi(aprv_id); pn();
        return 0;
    }
    return -1;
}

/* ══════ Artifact Management ══════ */
static int artifact_image_build(const char *name, const char *tag) {
    if (g_art_n >= MAX_ARTIFACTS) return -1;
    artifact_t *a = &g_arts[g_art_n];
    a->type = ART_IMAGE; a->signed_flag = 0;
    my_strncpy(a->name, name, MAX_NAME - 1);
    my_strncpy(a->hash, tag, MAX_NAME - 1);
    int idx = g_art_n++;
    ps("[artifact] image built: "); ps(name); ps(":"); ps(tag); pn();
    return idx;
}

static int artifact_helm_package(const char *name, const char *version) {
    if (g_art_n >= MAX_ARTIFACTS) return -1;
    artifact_t *a = &g_arts[g_art_n];
    a->type = ART_HELM; a->signed_flag = 0;
    my_strncpy(a->name, name, MAX_NAME - 1);
    my_strncpy(a->hash, version, MAX_NAME - 1);
    int idx = g_art_n++;
    ps("[artifact] helm packaged: "); ps(name);
    ps(" v"); ps(version); pn();
    return idx;
}

static int artifact_sbom_generate(const char *name) {
    if (g_art_n >= MAX_ARTIFACTS) return -1;
    artifact_t *a = &g_arts[g_art_n];
    a->type = ART_SBOM; a->signed_flag = 0;
    my_strncpy(a->name, name, MAX_NAME - 1);
    my_strncpy(a->hash, "sbom-sha256-0000", MAX_NAME - 1);
    int idx = g_art_n++;
    ps("[artifact] SBOM generated: "); ps(name); pn();
    return idx;
}

static int artifact_sign(int art_idx) {
    if (art_idx < 0 || art_idx >= g_art_n) return -1;
    g_arts[art_idx].signed_flag = 1;
    ps("[artifact] signed: "); ps(g_arts[art_idx].name); pn();
    return 0;
}

/* ══════ Notification System ══════ */
static int notif_add(int type, const char *target, const char *msg) {
    if (g_notif_n >= MAX_NOTIF) return -1;
    notif_t *n = &g_notifs[g_notif_n];
    n->type = type; n->sent = 0;
    my_strncpy(n->target, target, MAX_NAME - 1);
    my_strncpy(n->msg, msg, MAX_LOG - 1);
    int idx = g_notif_n++;
    return idx;
}

static int notify_slack(const char *channel, const char *msg) {
    int idx = notif_add(NOTIF_SLACK, channel, msg);
    ps("[notify] slack -> "); ps(channel); ps(": "); ps(msg); pn();
    return idx;
}

static int notify_email(const char *to, const char *msg) {
    int idx = notif_add(NOTIF_EMAIL, to, msg);
    ps("[notify] email -> "); ps(to); ps(": "); ps(msg); pn();
    return idx;
}

static int notify_git_status(const char *repo, const char *status) {
    int idx = notif_add(NOTIF_GIT, repo, status);
    ps("[notify] git-status -> "); ps(repo);
    ps(": "); ps(status); pn();
    return idx;
}

static int notify_dispatch_all(void) {
    int sent = 0, i;
    for (i = 0; i < g_notif_n; i++) {
        g_notifs[i].sent = 1;
        sent++;
    }
    ps("[notify] dispatched: "); pi(sent);
    ps("/"); pi(g_notif_n); pn();
    return sent;
}

/* ══════ Help ══════ */
static void show_help(void) {
    pl("pipeline_runner: CI/CD pipeline runner (v1.0)");
    pl("Usage: pipeline_runner [OPTIONS]");
    pl("  -h, --help     Show this help");
    pl("  -t, --test     Run test suite");
    pl("Features: stages, steps, plugins, caching, approval, artifacts, notify");
    pl("Like: Tekton / GitHub Actions / Jenkins");
}

/* ══════ Test Suite ══════ */
static void test_pipeline_create(void) {
    pl("\n--- Test: Pipeline Create ---");
    int p1 = pipeline_create("ci-pipeline");
    int p2 = pipeline_create("release-pipeline");
    if (p1 > 0 && p2 > 0) pl("[test] pipeline create: OK");
    else pl("[test] pipeline create: FAIL");
    pipeline_status(p1);
    pipeline_status(p2);
}

static void test_stage_step(void) {
    pl("\n--- Test: Stage/Step Execution ---");
    int p = pipeline_create("build-pipe");
    int s0 = stage_add(p, "build", 0, COND_ALWAYS);
    int s1 = stage_add(p, "test", 0, COND_ALWAYS);
    int s2 = stage_add(p, "deploy", 0, COND_ALWAYS);
    step_add(g_stages[s0].id, "compile", "gcc -o app main.c", 0, 60);
    step_add(g_stages[s0].id, "link", "ld -o app obj.o", 0, 30);
    step_add(g_stages[s1].id, "unit-test", "pytest tests/", 2, 120);
    step_add(g_stages[s2].id, "push", "docker push app:latest", 1, 90);
    if (s0 >= 0 && s1 >= 0 && s2 >= 0)
        pl("[test] stage/step add: OK");
    else pl("[test] stage/step add: FAIL");
    stage_run(s0);
    stage_run(s1);
    stage_run(s2);
    if (g_stages[s0].status == ST_DONE && g_stages[s1].status == ST_DONE)
        pl("[test] stage execution: OK");
    else pl("[test] stage execution: FAIL");
}

static void test_parallel(void) {
    pl("\n--- Test: Parallel Execution ---");
    int p = pipeline_create("parallel-pipe");
    int s0 = stage_add(p, "lint", 1, COND_ALWAYS);
    int s1 = stage_add(p, "scan", 1, COND_ALWAYS);
    int s2 = stage_add(p, "build", 1, COND_ALWAYS);
    stage_parallel(s0, 1);
    stage_parallel(s1, 1);
    stage_parallel(s2, 1);
    step_add(g_stages[s0].id, "eslint", "eslint src/", 0, 30);
    step_add(g_stages[s1].id, "trivy", "trivy image app", 0, 60);
    step_add(g_stages[s2].id, "make", "make all", 0, 120);
    stage_run(s0);
    stage_run(s1);
    stage_run(s2);
    if (g_stages[s0].parallel && g_stages[s1].parallel &&
        g_stages[s0].status == ST_DONE && g_stages[s1].status == ST_DONE)
        pl("[test] parallel execution: OK");
    else pl("[test] parallel execution: FAIL");
}

static void test_conditional(void) {
    pl("\n--- Test: Conditional Branch ---");
    int p = pipeline_create("cond-pipe");
    int s0 = stage_add(p, "main-build", 0, COND_BRANCH);
    int s1 = stage_add(p, "dev-only", 0, COND_NEVER);
    int s2 = stage_add(p, "on-success", 0, COND_EXPR);
    int r0 = condition_branch(s0, COND_BRANCH, "main");
    int r1 = condition_branch(s1, COND_NEVER, "");
    int r2 = condition_branch(s2, COND_EXPR, "success");
    if (r0 == 1 && r1 == 0 && r2 == 1)
        pl("[test] conditional branch: OK");
    else pl("[test] conditional branch: FAIL");
    if (g_stages[s1].status == ST_SKIP)
        pl("[test] skip non-matching: OK");
    else pl("[test] skip non-matching: FAIL");
}

static void test_plugin(void) {
    pl("\n--- Test: Plugin Registration ---");
    int b = task_register_builder("gcc-builder");
    int t = task_register_tester("pytest-runner");
    int d = task_register_deployer("helm-deployer");
    if (b >= 0 && t >= 0 && d >= 0)
        pl("[test] plugin register: OK");
    else pl("[test] plugin register: FAIL");
    task_param_pass("gcc-builder", "optimization", "O2");
    task_param_pass("gcc-builder", "target", "wasm32");
    task_param_pass("pytest-runner", "verbose", "true");
    int inv = task_invoke("gcc-builder");
    if (inv >= 0) pl("[test] plugin invoke: OK");
    else pl("[test] plugin invoke: FAIL");
    if (g_plugins[b].param_count == 2)
        pl("[test] param passing: OK");
    else pl("[test] param passing: FAIL");
}

static void test_cache(void) {
    pl("\n--- Test: Cache Hit/Miss ---");
    cache_save("deps-node", "node_modules-v18.tar.gz");
    cache_save("deps-pip", "pip-cache-3.11.tar.gz");
    char buf[MAX_CMD];
    int r0 = cache_load("deps-node", buf, MAX_CMD);
    if (r0 == 0 && my_strcmp(buf, "node_modules-v18.tar.gz") == 0)
        pl("[test] cache hit: OK");
    else pl("[test] cache hit: FAIL");
    int r1 = cache_load("nonexistent", buf, MAX_CMD);
    if (r1 == -1) pl("[test] cache miss: OK");
    else pl("[test] cache miss: FAIL");
    cache_invalidate("deps-node");
    int r2 = cache_load("deps-node", buf, MAX_CMD);
    if (r2 == -1) pl("[test] cache invalidate: OK");
    else pl("[test] cache invalidate: FAIL");
    cache_save("build-output", "dist/app.wasm");
    int r3 = cache_workspace_share("build-output", "deploy-input");
    if (r3 == 0) pl("[test] workspace share: OK");
    else pl("[test] workspace share: FAIL");
}

static void test_approval(void) {
    pl("\n--- Test: Approval Flow ---");
    int a0 = approval_request(1, 0, 300, 1);
    int a1 = approval_request(1, 1, 600, 0);
    if (a0 > 0 && a1 > 0) pl("[test] approval request: OK");
    else pl("[test] approval request: FAIL");
    approval_auto(a0);
    if (g_approve[0].status == APR_APPROVED)
        pl("[test] auto approval: OK");
    else pl("[test] auto approval: FAIL");
    approval_manual(a1, 1);
    if (g_approve[1].status == APR_APPROVED)
        pl("[test] manual approval: OK");
    else pl("[test] manual approval: FAIL");
    int a2 = approval_request(1, 2, 60, 0);
    approval_timeout(a2);
    if (g_approve[2].status == APR_TIMEOUT)
        pl("[test] approval timeout: OK");
    else pl("[test] approval timeout: FAIL");
}

static void test_artifact(void) {
    pl("\n--- Test: Artifact Publish ---");
    int i0 = artifact_image_build("myapp", "v1.2.3");
    int i1 = artifact_helm_package("myapp-chart", "0.5.0");
    int i2 = artifact_sbom_generate("myapp");
    if (i0 >= 0 && i1 >= 0 && i2 >= 0)
        pl("[test] artifact create: OK");
    else pl("[test] artifact create: FAIL");
    artifact_sign(i0);
    artifact_sign(i1);
    if (g_arts[i0].signed_flag && g_arts[i1].signed_flag)
        pl("[test] artifact sign: OK");
    else pl("[test] artifact sign: FAIL");
    if (!g_arts[i2].signed_flag)
        pl("[test] unsigned remains: OK");
    else pl("[test] unsigned remains: FAIL");
}

static void test_notify(void) {
    pl("\n--- Test: Notification Dispatch ---");
    notify_slack("#builds", "Build #42 SUCCESS");
    notify_email("dev@team.io", "Deploy v1.2.3 complete");
    notify_git_status("org/repo", "success");
    if (g_notif_n == 3) pl("[test] notification queue: OK");
    else pl("[test] notification queue: FAIL");
    int sent = notify_dispatch_all();
    if (sent == 3 && g_notifs[0].sent && g_notifs[1].sent && g_notifs[2].sent)
        pl("[test] dispatch all: OK");
    else pl("[test] dispatch all: FAIL");
}

static void test_depgraph(void) {
    pl("\n--- Test: Dependency Graph ---");
    int g0 = dependency_graph_add("build-graph");
    int n0 = depgraph_add_node(g0, "fetch-deps");
    int n1 = depgraph_add_node(g0, "compile");
    int n2 = depgraph_add_node(g0, "test");
    int n3 = depgraph_add_node(g0, "package");
    int n4 = depgraph_add_node(g0, "publish");
    depgraph_add_edge(g0, n0, n1);
    depgraph_add_edge(g0, n1, n2);
    depgraph_add_edge(g0, n1, n3);
    depgraph_add_edge(g0, n2, n4);
    depgraph_add_edge(g0, n3, n4);
    int resolved = dependency_graph_resolve(g0);
    if (resolved == 1) pl("[test] dep resolve roots: OK");
    else pl("[test] dep resolve roots: FAIL");
    int sorted = dependency_graph_topo_sort(g0);
    if (sorted == 5) pl("[test] topo sort: OK");
    else pl("[test] topo sort: FAIL");
}

static void test_retry(void) {
    pl("\n--- Test: Step Retry ---");
    int p = pipeline_create("retry-pipe");
    int s0 = stage_add(p, "flaky-stage", 0, COND_ALWAYS);
    int st = step_add(g_stages[s0].id, "flaky-step", "fail", 2, 30);
    stage_run(s0);
    if (g_steps[0].attempt == 3 && g_steps[0].status == ST_FAIL)
        pl("[test] step retry exhausted: OK");
    else pl("[test] step retry exhausted: FAIL");
}

static void run_tests(void) {
    pl("=== Pipeline Runner Test Suite ===");
    test_pipeline_create();
    test_stage_step();
    test_parallel();
    test_conditional();
    test_plugin();
    test_cache();
    test_approval();
    test_artifact();
    test_notify();
    test_depgraph();
    test_retry();
    pl("\n--- Summary ---");
    ps("Pipelines: "); pi(g_pipe_n);
    ps(" Stages: "); pi(g_stg_n);
    ps(" Steps: "); pi(g_step_n);
    ps(" Plugins: "); pi(g_plug_n);
    ps(" Cache: "); pi(g_cache_n);
    ps(" Approvals: "); pi(g_aprv_n);
    ps(" Artifacts: "); pi(g_art_n);
    ps(" Notifs: "); pi(g_notif_n); pn();
    pl("[test] all tests complete");
}

/* ══════ Entry Point ══════ */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)buf;
    while (pos < 512 && av[pos]) pos++;
    pos++;
    while (pos < 512 && av[pos]) {
        char *a = &av[pos];
        if (my_strcmp(a, "-h") == 0 || my_strcmp(a, "--help") == 0) help = 1;
        else if (my_strcmp(a, "-t") == 0 || my_strcmp(a, "--test") == 0) test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    }
    if (help) { show_help(); host_exit(0); }
    if (test) { run_tests(); host_exit(0); }
    ps("pipeline_runner: use -h for help, -t for test\n");
    host_exit(0);
}
