/*
 * config_management - Configuration Management Center for AI-ASM OS
 * Hierarchical KV store, dynamic push, multi-env, secrets, templates, ACL
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

#define MAX_CFG      256
#define MAX_HIST     16
#define MAX_ENV      8
#define MAX_LIS      64
#define MAX_TPL      32
#define MAX_SEC      64
#define MAX_ACL     128
#define MAX_AUDIT   256
#define MAX_KEY     128
#define MAX_VAL     512
#define MAX_NS       64
#define MAX_NAME     64
#define MAX_VER      16
#define MAX_CHILD    16

#define VT_STR 0
#define VT_INT  1
#define VT_BOOL 2
#define VT_SEC  3
#define VT_TPL  4

#define ENV_DEV 0
#define ENV_TST 1
#define ENV_STG 2
#define ENV_PRD 3

#define P_READ   0x01U
#define P_WRITE  0x02U
#define P_DEL    0x04U
#define P_ADMIN  0x08U
#define P_AUDIT  0x10U

#define ROT_MANUAL    0
#define ROT_SCHED     1
#define ROT_ACCESS    2

#define FMT_JSON  0
#define FMT_YAML  1
#define FMT_PROP  2

#define AO_RD   1
#define AO_WR   2
#define AO_DEL  3
#define AO_SA   4
#define AO_SR   5
#define AO_ES   6
#define AO_TR   7
#define AO_PC   8
#define AO_RB   9

#define ST_OK         0
#define ST_NOTFOUND   1
#define ST_PERM       2
#define ST_VERMISS    3
#define ST_EXPIRED    4
#define ST_OVERFLOW   5
#define NOTFOUND      0xFFFFFFFFU

typedef struct { unsigned int ts, actor; char old_v[MAX_VAL], new_v[MAX_VAL]; } ver_rec_t;

typedef struct {
    char ns[MAX_NS], grp[MAX_NS], key[MAX_KEY], val[MAX_VAL];
    unsigned int vtype, ver, env, locked, dirty;
    ver_rec_t hist[MAX_HIST];
    unsigned int hist_n;
} cfg_entry_t;

typedef struct {
    char id[MAX_NAME], ns[MAX_NS], grp[MAX_NS];
    unsigned int env, last_ver, active, gray_pct, confirmed;
} listener_t;

typedef struct {
    char name[16]; unsigned int id, parent, has_parent, active;
} env_t;

typedef struct {
    char sid[MAX_NAME], ns[MAX_NS], enc[MAX_VAL];
    unsigned int ver, rot_mode, created, expires, last_acc, acc_cnt, active;
    char prev[MAX_VER][MAX_VAL];
    unsigned int prev_n;
} secret_t;

typedef struct {
    char tid[MAX_NAME], body[MAX_VAL];
    unsigned int fmt, ver, active;
    char vn[MAX_CHILD][MAX_KEY], vd[MAX_CHILD][MAX_VAL];
    unsigned int vn_cnt;
} tmpl_t;

typedef struct {
    char princ[MAX_NAME], ns[MAX_NS];
    unsigned int perms, grantor, granted, active;
} acl_t;

typedef struct {
    unsigned int ts, actor, op, success;
    char ns[MAX_NS], key[MAX_KEY], detail[MAX_VAL];
} audit_t;

static cfg_entry_t g_cfg[MAX_CFG];       static unsigned int g_cfg_n;
static listener_t  g_lis[MAX_LIS];       static unsigned int g_lis_n;
static env_t       g_env[MAX_ENV];       static unsigned int g_env_n;
static secret_t    g_sec[MAX_SEC];       static unsigned int g_sec_n;
static tmpl_t      g_tpl[MAX_TPL];       static unsigned int g_tpl_n;
static acl_t       g_acl[MAX_ACL];       static unsigned int g_acl_n;
static audit_t     g_aud[MAX_AUDIT];     static unsigned int g_aud_n;
static unsigned int g_cur_env, g_clock;

/* Forward declarations */
static unsigned int cm_reg_cfg(const char *ns, const char *g, const char *k,
                               const char *v, unsigned int vt, unsigned int e);
static unsigned int cm_get_cfg(const char *ns, const char *g, const char *k,
                               unsigned int e, char *out, unsigned int osz);
static unsigned int cm_upd_cfg(const char *ns, const char *g, const char *k,
                               const char *v, unsigned int e, unsigned int a);
static unsigned int cm_rollback(const char *ns, const char *g, const char *k,
                                unsigned int tv, unsigned int e, unsigned int a);
static unsigned int cm_del_cfg(const char *ns, const char *g, const char *k,
                               unsigned int e, unsigned int a);
static unsigned int cm_reg_lis(const char *id, const char *ns, const char *g,
                               unsigned int e);
static unsigned int cm_poll_lis(void);
static unsigned int cm_confirm_lis(const char *id);
static unsigned int cm_set_gray(const char *id, unsigned int pct);
static unsigned int cm_init_envs(void);
static unsigned int cm_add_env(const char *nm, unsigned int par);
static unsigned int cm_sw_env(unsigned int e, unsigned int a);
static unsigned int cm_cmp_env(unsigned int a, unsigned int b);
static unsigned int cm_inherit(unsigned int par, unsigned int ch);
static unsigned int cm_store_sec(const char *sid, const char *ns,
                                 const char *enc, unsigned int rm, unsigned int ttl);
static unsigned int cm_acc_sec(const char *sid, char *out, unsigned int osz);
static unsigned int cm_rot_sec(const char *sid, const char *enc, unsigned int a);
static unsigned int cm_chk_sec(const char *sid);
static unsigned int cm_reg_tpl(const char *tid, const char *body, unsigned int fmt);
static unsigned int cm_render_tpl(const char *tid, char *out, unsigned int osz);
static unsigned int cm_grant(const char *pr, const char *ns, unsigned int pm,
                             unsigned int gr);
static unsigned int cm_chk_perm(const char *pr, const char *ns, unsigned int nd);
static void cm_log(unsigned int a, unsigned int op, const char *ns,
                   const char *k, const char *d, unsigned int ok);
static unsigned int cm_audit_rpt(char *out, unsigned int osz);
static unsigned int cm_process(void);

/* Utility */
static unsigned int streq(const char *a, const char *b) {
    unsigned int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; i++; }
    return a[i] == b[i];
}
static void strcp(char *d, const char *s, unsigned int mx) {
    unsigned int i = 0;
    while (i < mx - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}
static void mzero(void *p, unsigned int sz) {
    unsigned char *b = (unsigned char *)p; unsigned int i;
    for (i = 0; i < sz; i++) b[i] = 0;
}
static unsigned int find_cfg(const char *ns, const char *g, const char *k,
                             unsigned int e) {
    unsigned int i;
    for (i = 0; i < g_cfg_n; i++) {
        if (g_cfg[i].env == e && streq(g_cfg[i].ns, ns) &&
            streq(g_cfg[i].grp, g) && streq(g_cfg[i].key, k)) return i;
    }
    return NOTFOUND;
}
static unsigned int find_sec(const char *sid) {
    unsigned int i;
    for (i = 0; i < g_sec_n; i++)
        if (streq(g_sec[i].sid, sid) && g_sec[i].active) return i;
    return NOTFOUND;
}
static unsigned int find_acl(const char *pr, const char *ns) {
    unsigned int i;
    for (i = 0; i < g_acl_n; i++)
        if (g_acl[i].active && streq(g_acl[i].princ, pr) &&
            streq(g_acl[i].ns, ns)) return i;
    return NOTFOUND;
}

/* Init */
static unsigned int cm_init_envs(void) {
    const char *nm[4] = {"dev", "test", "staging", "prod"};
    unsigned int i;
    for (i = 0; i < 4; i++) {
        strcp(g_env[i].name, nm[i], 16);
        g_env[i].id = i; g_env[i].has_parent = (i > 0);
        g_env[i].parent = (i > 0) ? i - 1 : 0; g_env[i].active = 1;
    }
    g_env_n = 4; return ST_OK;
}
static void cm_init(void) {
    g_cfg_n = g_lis_n = g_env_n = g_sec_n = g_tpl_n = g_acl_n = g_aud_n = 0;
    g_cur_env = ENV_DEV; g_clock = 1000000U;
    cm_init_envs();
    host_print("[config_management] initialized\n");
}

/* Config Storage */
static unsigned int cm_reg_cfg(const char *ns, const char *g, const char *k,
                               const char *v, unsigned int vt, unsigned int e) {
    if (g_cfg_n >= MAX_CFG) return ST_OVERFLOW;
    if (find_cfg(ns, g, k, e) != NOTFOUND) return ST_OVERFLOW;
    cfg_entry_t *c = &g_cfg[g_cfg_n];
    mzero(c, sizeof(cfg_entry_t));
    strcp(c->ns, ns, MAX_NS); strcp(c->grp, g, MAX_NS);
    strcp(c->key, k, MAX_KEY); strcp(c->val, v, MAX_VAL);
    c->vtype = vt; c->ver = 1; c->env = e; c->locked = 0; c->dirty = 0; c->hist_n = 0;
    g_cfg_n++;
    host_print("[cm] registered: "); host_print(ns); host_print("/");
    host_print(g); host_print("/"); host_print(k); host_print("\n");
    return ST_OK;
}
static unsigned int cm_get_cfg(const char *ns, const char *g, const char *k,
                               unsigned int e, char *out, unsigned int osz) {
    unsigned int idx = find_cfg(ns, g, k, e);
    if (idx == NOTFOUND && g_env[e].has_parent)
        idx = find_cfg(ns, g, k, g_env[e].parent);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    strcp(out, g_cfg[idx].val, osz); return ST_OK;
}
static unsigned int cm_upd_cfg(const char *ns, const char *g, const char *k,
                               const char *v, unsigned int e, unsigned int actor) {
    unsigned int idx = find_cfg(ns, g, k, e);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    cfg_entry_t *c = &g_cfg[idx];
    if (c->locked) return ST_PERM;
    if (c->hist_n < MAX_HIST) {
        ver_rec_t *h = &c->hist[c->hist_n];
        h->ts = g_clock; h->actor = actor;
        strcp(h->old_v, c->val, MAX_VAL); strcp(h->new_v, v, MAX_VAL);
        c->hist_n++;
    }
    strcp(c->val, v, MAX_VAL); c->ver++; c->dirty = 1; g_clock++;
    host_print("[cm] updated\n"); return ST_OK;
}
static unsigned int cm_rollback(const char *ns, const char *g, const char *k,
                                unsigned int tv, unsigned int e, unsigned int actor) {
    unsigned int idx = find_cfg(ns, g, k, e);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    cfg_entry_t *c = &g_cfg[idx];
    if (tv >= c->ver || tv == 0) return ST_VERMISS;
    unsigned int hi = tv - 1;
    if (hi < c->hist_n) {
        strcp(c->val, c->hist[hi].new_v, MAX_VAL);
        c->ver++; c->dirty = 1;
        cm_log(actor, AO_RB, ns, k, "rollback", 1);
        host_print("[cm] rollback done\n"); return ST_OK;
    }
    return ST_VERMISS;
}
static unsigned int cm_del_cfg(const char *ns, const char *g, const char *k,
                               unsigned int e, unsigned int actor) {
    unsigned int idx = find_cfg(ns, g, k, e);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    unsigned int i;
    for (i = idx; i + 1 < g_cfg_n; i++) g_cfg[i] = g_cfg[i + 1];
    g_cfg_n--; cm_log(actor, AO_DEL, ns, k, "deleted", 1);
    host_print("[cm] deleted\n"); return ST_OK;
}

/* Dynamic Config: Listeners, Push, Gray Release */
static unsigned int cm_reg_lis(const char *id, const char *ns, const char *g,
                               unsigned int e) {
    if (g_lis_n >= MAX_LIS) return ST_OVERFLOW;
    listener_t *l = &g_lis[g_lis_n];
    strcp(l->id, id, MAX_NAME); strcp(l->ns, ns, MAX_NS); strcp(l->grp, g, MAX_NS);
    l->env = e; l->last_ver = 0; l->active = 1; l->gray_pct = 100; l->confirmed = 0;
    g_lis_n++;
    host_print("[cm] listener: "); host_print(id); host_print("\n"); return ST_OK;
}
static unsigned int cm_poll_lis(void) {
    unsigned int i, j, n = 0;
    for (i = 0; i < g_lis_n; i++) {
        listener_t *l = &g_lis[i];
        if (!l->active) continue;
        for (j = 0; j < g_cfg_n; j++) {
            if (g_cfg[j].env == l->env && streq(g_cfg[j].ns, l->ns) &&
                streq(g_cfg[j].grp, l->grp) && g_cfg[j].dirty &&
                g_cfg[j].ver > l->last_ver) {
                host_print("[cm] push -> "); host_print(l->id); host_print("\n");
                l->last_ver = g_cfg[j].ver; l->confirmed = 0; n++;
            }
        }
    }
    return n;
}
static unsigned int cm_confirm_lis(const char *id) {
    unsigned int i;
    for (i = 0; i < g_lis_n; i++)
        if (streq(g_lis[i].id, id)) { g_lis[i].confirmed = 1; return ST_OK; }
    return ST_NOTFOUND;
}
static unsigned int cm_set_gray(const char *id, unsigned int pct) {
    unsigned int i;
    for (i = 0; i < g_lis_n; i++)
        if (streq(g_lis[i].id, id)) { g_lis[i].gray_pct = (pct > 100) ? 100 : pct; return ST_OK; }
    return ST_NOTFOUND;
}

/* Environment Management */
static unsigned int cm_add_env(const char *nm, unsigned int par) {
    if (g_env_n >= MAX_ENV) return ST_OVERFLOW;
    if (par >= g_env_n) return ST_VERMISS;
    env_t *e = &g_env[g_env_n];
    strcp(e->name, nm, 16); e->id = g_env_n; e->parent = par;
    e->has_parent = 1; e->active = 1; g_env_n++;
    host_print("[cm] env added: "); host_print(nm); host_print("\n"); return ST_OK;
}
static unsigned int cm_sw_env(unsigned int e, unsigned int actor) {
    if (e >= g_env_n) return ST_VERMISS;
    g_cur_env = e; cm_log(actor, AO_ES, "", "", "switch", 1);
    host_print("[cm] env switched\n"); return ST_OK;
}
static unsigned int cm_cmp_env(unsigned int a, unsigned int b) {
    unsigned int i, diffs = 0;
    for (i = 0; i < g_cfg_n; i++) {
        if (g_cfg[i].env != a) continue;
        unsigned int j, found = 0;
        for (j = 0; j < g_cfg_n; j++) {
            if (g_cfg[j].env == b && streq(g_cfg[j].ns, g_cfg[i].ns) &&
                streq(g_cfg[j].grp, g_cfg[i].grp) && streq(g_cfg[j].key, g_cfg[i].key)) {
                if (!streq(g_cfg[i].val, g_cfg[j].val)) diffs++;
                found = 1; break;
            }
        }
        if (!found) diffs++;
    }
    host_print("[cm] env compare done\n"); return diffs;
}
static unsigned int cm_inherit(unsigned int par, unsigned int ch) {
    unsigned int i, n = 0;
    for (i = 0; i < g_cfg_n; i++) {
        if (g_cfg[i].env == par && find_cfg(g_cfg[i].ns, g_cfg[i].grp,
                                             g_cfg[i].key, ch) == NOTFOUND) {
            cm_reg_cfg(g_cfg[i].ns, g_cfg[i].grp, g_cfg[i].key,
                       g_cfg[i].val, g_cfg[i].vtype, ch);
            n++;
        }
    }
    host_print("[cm] inherit done\n"); return n;
}

/* Secret Management */
static unsigned int cm_store_sec(const char *sid, const char *ns,
                                 const char *enc, unsigned int rm, unsigned int ttl) {
    if (g_sec_n >= MAX_SEC) return ST_OVERFLOW;
    secret_t *s = &g_sec[g_sec_n]; mzero(s, sizeof(secret_t));
    strcp(s->sid, sid, MAX_NAME); strcp(s->ns, ns, MAX_NS);
    strcp(s->enc, enc, MAX_VAL);
    s->ver = 1; s->rot_mode = rm; s->created = g_clock;
    s->expires = g_clock + ttl; s->last_acc = 0; s->acc_cnt = 0;
    s->active = 1; s->prev_n = 0; g_sec_n++;
    host_print("[cm] secret: "); host_print(sid); host_print("\n"); return ST_OK;
}
static unsigned int cm_acc_sec(const char *sid, char *out, unsigned int osz) {
    unsigned int idx = find_sec(sid);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    secret_t *s = &g_sec[idx];
    if (g_clock > s->expires) return ST_EXPIRED;
    strcp(out, s->enc, osz); s->last_acc = g_clock; s->acc_cnt++;
    cm_log(0, AO_SA, s->ns, sid, "access", 1); return ST_OK;
}
static unsigned int cm_rot_sec(const char *sid, const char *enc, unsigned int actor) {
    unsigned int idx = find_sec(sid);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    secret_t *s = &g_sec[idx];
    if (s->prev_n < MAX_VER) {
        strcp(s->prev[s->prev_n], s->enc, MAX_VAL); s->prev_n++;
    }
    strcp(s->enc, enc, MAX_VAL); s->ver++; s->last_acc = g_clock;
    cm_log(actor, AO_SR, s->ns, sid, "rotated", 1);
    host_print("[cm] rotated: "); host_print(sid); host_print("\n"); return ST_OK;
}
static unsigned int cm_chk_sec(const char *sid) {
    unsigned int idx = find_sec(sid);
    if (idx == NOTFOUND) return ST_NOTFOUND;
    return (g_clock > g_sec[idx].expires) ? ST_EXPIRED : ST_OK;
}

/* Template Engine */
static unsigned int cm_reg_tpl(const char *tid, const char *body, unsigned int fmt) {
    if (g_tpl_n >= MAX_TPL) return ST_OVERFLOW;
    tmpl_t *t = &g_tpl[g_tpl_n]; mzero(t, sizeof(tmpl_t));
    strcp(t->tid, tid, MAX_NAME); strcp(t->body, body, MAX_VAL);
    t->fmt = fmt; t->ver = 1; t->vn_cnt = 0; t->active = 1; g_tpl_n++;
    host_print("[cm] template: "); host_print(tid); host_print("\n"); return ST_OK;
}
static unsigned int cm_render_tpl(const char *tid, char *out, unsigned int osz) {
    unsigned int idx = NOTFOUND, i;
    for (i = 0; i < g_tpl_n; i++)
        if (streq(g_tpl[i].tid, tid) && g_tpl[i].active) { idx = i; break; }
    if (idx == NOTFOUND) return ST_NOTFOUND;
    tmpl_t *t = &g_tpl[idx];
    unsigned int si = 0, di = 0;
    const char *b = t->body;
    while (b[si] && di < osz - 1) {
        if (b[si] == '$' && b[si + 1] == '{') {
            unsigned int vs = si + 2, ve = vs;
            while (b[ve] && b[ve] != '}') ve++;
            char vn[MAX_KEY]; unsigned int vl = ve - vs;
            if (vl >= MAX_KEY) vl = MAX_KEY - 1;
            unsigned int vi; for (vi = 0; vi < vl; vi++) vn[vi] = b[vs + vi];
            vn[vi] = '\0';
            char res[MAX_VAL]; unsigned int found = 0, ci;
            for (ci = 0; ci < g_cfg_n; ci++) {
                if (streq(g_cfg[ci].key, vn) && g_cfg[ci].env == g_cur_env) {
                    strcp(res, g_cfg[ci].val, MAX_VAL); found = 1; break;
                }
            }
            if (!found) {
                unsigned int di2;
                for (di2 = 0; di2 < t->vn_cnt; di2++)
                    if (streq(t->vn[di2], vn)) { strcp(res, t->vd[di2], MAX_VAL); found = 1; break; }
            }
            if (found) {
                unsigned int ri = 0;
                while (res[ri] && di < osz - 1) out[di++] = res[ri++];
            } else {
                while (si < ve && di < osz - 1) out[di++] = b[si++];
                if (di < osz - 1) out[di++] = '}';
            }
            si = (b[ve] == '}') ? ve + 1 : ve;
        } else { out[di++] = b[si++]; }
    }
    out[di] = '\0'; cm_log(0, AO_TR, "", tid, "render", 1); return ST_OK;
}

/* Permission & Audit */
static unsigned int cm_grant(const char *pr, const char *ns, unsigned int pm,
                             unsigned int gr) {
    unsigned int idx = find_acl(pr, ns);
    if (idx != NOTFOUND) {
        g_acl[idx].perms = pm; g_acl[idx].granted = g_clock;
        cm_log(gr, AO_PC, ns, "", "updated", 1); return ST_OK;
    }
    if (g_acl_n >= MAX_ACL) return ST_OVERFLOW;
    acl_t *a = &g_acl[g_acl_n];
    strcp(a->princ, pr, MAX_NAME); strcp(a->ns, ns, MAX_NS);
    a->perms = pm; a->grantor = gr; a->granted = g_clock; a->active = 1; g_acl_n++;
    cm_log(gr, AO_PC, ns, "", "granted", 1);
    host_print("[cm] perm granted\n"); return ST_OK;
}
static unsigned int cm_chk_perm(const char *pr, const char *ns, unsigned int nd) {
    unsigned int idx = find_acl(pr, ns);
    if (idx == NOTFOUND) return ST_PERM;
    return ((g_acl[idx].perms & nd) == nd) ? ST_OK : ST_PERM;
}
static void cm_log(unsigned int actor, unsigned int op, const char *ns,
                   const char *k, const char *d, unsigned int ok) {
    if (g_aud_n >= MAX_AUDIT) return;
    audit_t *a = &g_aud[g_aud_n];
    a->ts = g_clock; a->actor = actor; a->op = op;
    strcp(a->ns, ns, MAX_NS); strcp(a->key, k, MAX_KEY);
    strcp(a->detail, d, MAX_VAL); a->success = ok; g_aud_n++;
}
static unsigned int cm_audit_rpt(char *out, unsigned int osz) {
    unsigned int di = 0, i;
    for (i = 0; i < g_aud_n && di < osz - 1; i++) {
        unsigned int ts = g_aud[i].ts, digs[10], nd = 0;
        if (ts == 0) digs[nd++] = 0;
        while (ts > 0 && nd < 10) { digs[nd++] = ts % 10; ts /= 10; }
        unsigned int d; for (d = nd; d > 0 && di < osz - 1; d--) out[di++] = '0' + digs[d - 1];
        if (di < osz - 1) out[di++] = '|';
        if (g_aud[i].op < 10 && di < osz - 1) out[di++] = '0' + g_aud[i].op;
        if (di < osz - 1) out[di++] = '|';
        if (di < osz - 1) out[di++] = g_aud[i].success ? '1' : '0';
        if (di < osz - 1) out[di++] = '\n';
    }
    out[di] = '\0'; return di;
}

/* Main processing loop */
static unsigned int cm_process(void) {
    g_clock++;
    cm_poll_lis();
    unsigned int i;
    for (i = 0; i < g_sec_n; i++)
        if (g_sec[i].active && g_clock > g_sec[i].expires) {
            host_print("[cm] expired: "); host_print(g_sec[i].sid); host_print("\n");
            g_sec[i].active = 0;
        }
    for (i = 0; i < g_cfg_n; i++) g_cfg[i].dirty = 0;
    return ST_OK;
}

/* Exported entry point */
__attribute__((visibility("default")))
int run(void) {
    cm_init();
    cm_sw_env(ENV_DEV, 1);

    /* Register configs across environments */
    cm_reg_cfg("app", "server", "host", "0.0.0.0", VT_STR, ENV_DEV);
    cm_reg_cfg("app", "server", "port", "8080", VT_INT, ENV_DEV);
    cm_reg_cfg("app", "server", "port", "9090", VT_INT, ENV_PRD);
    cm_reg_cfg("app", "db", "url", "pg://dev:5432/app", VT_STR, ENV_DEV);
    cm_reg_cfg("app", "db", "url", "pg://prod:5432/app", VT_STR, ENV_PRD);
    cm_reg_cfg("app", "cache", "ttl", "300", VT_INT, ENV_DEV);
    cm_reg_cfg("app", "cache", "on", "true", VT_BOOL, ENV_DEV);

    char buf[MAX_VAL]; unsigned int rc;
    rc = cm_get_cfg("app", "server", "port", ENV_DEV, buf, MAX_VAL);
    if (rc == ST_OK) { host_print("[cm] dev port="); host_print(buf); host_print("\n"); }
    rc = cm_get_cfg("app", "server", "port", ENV_PRD, buf, MAX_VAL);
    if (rc == ST_OK) { host_print("[cm] prod port="); host_print(buf); host_print("\n"); }

    /* Update and rollback */
    cm_upd_cfg("app", "server", "host", "127.0.0.1", ENV_DEV, 1);
    cm_upd_cfg("app", "server", "host", "0.0.0.0", ENV_DEV, 1);
    cm_rollback("app", "server", "host", 1, ENV_DEV, 1);
    cm_get_cfg("app", "server", "host", ENV_DEV, buf, MAX_VAL);
    host_print("[cm] rolled-back host="); host_print(buf); host_print("\n");

    /* Listeners and push */
    cm_reg_lis("svc-a", "app", "server", ENV_DEV);
    cm_reg_lis("svc-b", "app", "db", ENV_DEV);
    cm_set_gray("svc-a", 50);
    cm_upd_cfg("app", "server", "port", "8081", ENV_DEV, 2);
    cm_poll_lis();
    cm_confirm_lis("svc-a");

    /* Environment inheritance and comparison */
    cm_inherit(ENV_DEV, ENV_STG);
    rc = cm_get_cfg("app", "server", "host", ENV_STG, buf, MAX_VAL);
    if (rc == ST_OK) { host_print("[cm] stg host="); host_print(buf); host_print("\n"); }
    cm_cmp_env(ENV_DEV, ENV_PRD);

    /* Secrets */
    cm_store_sec("db-pw", "app", "ENC(AES:7f3a9b2)", ROT_SCHED, 86400);
    cm_store_sec("api-key", "app", "ENC(AES:a1b2c3d)", ROT_MANUAL, 3600);
    cm_acc_sec("db-pw", buf, MAX_VAL);
    host_print("[cm] secret accessed\n");
    cm_rot_sec("db-pw", "ENC(AES:newval123)", 1);
    cm_acc_sec("db-pw", buf, MAX_VAL);
    cm_chk_sec("db-pw"); cm_chk_sec("api-key");

    /* Template */
    cm_reg_tpl("srv-cfg", "{\"host\":\"${host}\",\"port\":${port}}", FMT_JSON);
    tmpl_t *tp = &g_tpl[0];
    strcp(tp->vn[0], "host", MAX_KEY); strcp(tp->vd[0], "localhost", MAX_VAL);
    strcp(tp->vn[1], "port", MAX_KEY); strcp(tp->vd[1], "3000", MAX_VAL);
    tp->vn_cnt = 2;
    cm_render_tpl("srv-cfg", buf, MAX_VAL);
    host_print("[cm] tpl="); host_print(buf); host_print("\n");

    /* ACL */
    cm_grant("admin", "app", P_READ | P_WRITE | P_ADMIN, 0);
    cm_grant("viewer", "app", P_READ, 1);
    cm_grant("deploy", "app", P_READ | P_WRITE, 1);
    cm_chk_perm("admin", "app", P_WRITE);
    cm_chk_perm("viewer", "app", P_WRITE);

    /* Process and audit */
    cm_process();
    cm_audit_rpt(buf, MAX_VAL);
    host_print("[cm] audit report generated\n");

    cm_del_cfg("app", "cache", "ttl", ENV_DEV, 1);
    host_print("[config_management] all operations completed\n");
    return 0;
}
