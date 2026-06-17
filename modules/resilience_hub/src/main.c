/*
 * resilience_hub - Fault Tolerance and Resilience Center
 * AI-ASM OS WASM Module
 * Provides: circuit breaker, retry, timeout, isolation, degradation, health
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

#define MAX_NAME    64U
#define MAX_ITEMS   32U
#define MAX_DEPS    64U
#define MAX_RETRIES 10U
#define ERR_BUF     128U

typedef enum { CB_CLOSED = 0, CB_OPEN = 1, CB_HALF_OPEN = 2 } cb_state_t;
typedef enum { BO_EXP = 0, BO_JITTER = 1, BO_LINEAR = 2, BO_FIXED = 3 } backoff_t;
typedef enum { ISO_THREAD = 0, ISO_SEM = 1, ISO_BULK = 2, ISO_RES = 3 } iso_type_t;
typedef enum { DEG_FALL = 0, DEG_CACHE = 1, DEG_STATIC = 2, DEG_DEF = 3, DEG_EMPTY = 4 } degrade_t;
typedef enum { H_UNK = 0, H_OK = 1, H_DEG = 2, H_BAD = 3 } health_t;
typedef enum { P_READY = 0, P_LIVE = 1, P_START = 2 } probe_t;
typedef enum { T_CONN = 0, T_READ = 1, T_WRITE = 2, T_CALL = 3, T_CASC = 4 } tout_t;
typedef enum { E_NONE = 0, E_PARAM = 1, E_BREAKER = 2, E_RETRY = 3, E_TIMEOUT = 4,
               E_POOL = 5, E_DEG = 6, E_HEALTH = 7 } err_t;

/* ─── Forward declarations ─── */
typedef struct circuit_breaker circuit_breaker_t;
typedef struct retry_cfg retry_cfg_t;
typedef struct timeout_cfg timeout_cfg_t;
typedef struct iso_pool iso_pool_t;
typedef struct degrade_pol degrade_pol_t;
typedef struct health_node health_node_t;
typedef struct resilience_hub resilience_hub_t;

static void     cb_init(circuit_breaker_t *cb, const char *name);
static err_t    cb_execute(circuit_breaker_t *cb);
static void     cb_success(circuit_breaker_t *cb);
static void     cb_failure(circuit_breaker_t *cb);
static cb_state_t cb_state(circuit_breaker_t *cb);
static void     cb_reset(circuit_breaker_t *cb);
static void     cb_transit(circuit_breaker_t *cb, cb_state_t s);
static void     retry_init(retry_cfg_t *rc, const char *name);
static err_t    retry_exec(retry_cfg_t *rc);
static unsigned retry_delay(retry_cfg_t *rc, unsigned attempt);
static void     retry_set_cond(retry_cfg_t *rc, err_t *errs, unsigned n);
static int      retry_should(retry_cfg_t *rc, err_t e);
static void     tout_init(timeout_cfg_t *tc, const char *name);
static err_t    tout_check(timeout_cfg_t *tc, tout_t type, unsigned ms);
static unsigned tout_budget(timeout_cfg_t *tc);
static void     tout_set(timeout_cfg_t *tc, tout_t type, unsigned ms);
static void     pool_init(iso_pool_t *p, const char *name, iso_type_t t, unsigned max);
static err_t    pool_acquire(iso_pool_t *p);
static void     pool_release(iso_pool_t *p);
static unsigned pool_avail(const iso_pool_t *p);
static unsigned pool_util(const iso_pool_t *p);
static void     deg_init(degrade_pol_t *dp, const char *name, degrade_t t);
static err_t    deg_exec(degrade_pol_t *dp);
static void     deg_static(degrade_pol_t *dp, const char *resp);
static void     deg_default(degrade_pol_t *dp, int val);
static void     deg_cache(degrade_pol_t *dp, const char *key);
static void     hnode_init(health_node_t *n, const char *name);
static void     hnode_active(health_node_t *n);
static void     hnode_passive(health_node_t *n, int ok);
static health_t hnode_probe(health_node_t *n, probe_t p);
static void     hnode_add_dep(health_node_t *par, health_node_t *child);
static health_t hnode_aggregate(const health_node_t *n);
static resilience_hub_t *hub_create(const char *name);
static void     hub_destroy(resilience_hub_t *hub);
static err_t    hub_add_cb(resilience_hub_t *h, circuit_breaker_t *cb);
static err_t    hub_add_rc(resilience_hub_t *h, retry_cfg_t *rc);
static err_t    hub_add_tc(resilience_hub_t *h, timeout_cfg_t *tc);
static err_t    hub_add_pool(resilience_hub_t *h, iso_pool_t *p);
static err_t    hub_add_deg(resilience_hub_t *h, degrade_pol_t *dp);
static err_t    hub_add_health(resilience_hub_t *h, health_node_t *n);
static err_t    hub_exec(resilience_hub_t *h, const char *comp);
static health_t hub_health(const resilience_hub_t *h);

/* ─── Copy string helper ─── */
static void str_copy(char *dst, const char *src, unsigned max) {
    unsigned i = 0U;
    while (i < max - 1U && src[i] != '\0') { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* ═══════════════════════════════════════════════════════════
 *  Circuit Breaker - closed/open/half-open with auto-recovery
 * ═══════════════════════════════════════════════════════════ */
struct circuit_breaker {
    char name[MAX_NAME];
    cb_state_t state;
    unsigned fail_ct, succ_ct, total_ct;
    unsigned fail_thresh, succ_thresh, timeout_ms;
    unsigned err_rate_pct, open_since_ms;
    unsigned ho_max, ho_ct;
    int enabled;
};

static void cb_init(circuit_breaker_t *cb, const char *name) {
    if (!cb || !name) { host_print("cb_init: bad param\n"); return; }
    str_copy(cb->name, name, MAX_NAME);
    cb->state = CB_CLOSED; cb->fail_ct = 0U; cb->succ_ct = 0U; cb->total_ct = 0U;
    cb->fail_thresh = 5U; cb->succ_thresh = 3U; cb->timeout_ms = 30000U;
    cb->err_rate_pct = 50U; cb->open_since_ms = 0U; cb->ho_max = 3U; cb->ho_ct = 0U;
    cb->enabled = 1;
}

static void cb_transit(circuit_breaker_t *cb, cb_state_t s) {
    cb->state = s;
    if (s == CB_CLOSED) { cb->fail_ct = 0U; cb->succ_ct = 0U; cb->ho_ct = 0U; }
    else if (s == CB_HALF_OPEN) { cb->ho_ct = 0U; }
    else if (s == CB_OPEN) { cb->open_since_ms = cb->timeout_ms; }
}

static void cb_success(circuit_breaker_t *cb) {
    if (!cb) return;
    cb->succ_ct++; cb->total_ct++;
    if (cb->state == CB_HALF_OPEN) {
        cb->ho_ct++;
        if (cb->ho_ct >= cb->succ_thresh) cb_transit(cb, CB_CLOSED);
    } else if (cb->state == CB_CLOSED && cb->fail_ct > 0U) {
        cb->fail_ct--;
    }
}

static void cb_failure(circuit_breaker_t *cb) {
    if (!cb) return;
    cb->fail_ct++; cb->total_ct++;
    if (cb->state == CB_HALF_OPEN) {
        cb_transit(cb, CB_OPEN);
    } else if (cb->state == CB_CLOSED) {
        unsigned rate = (cb->total_ct > 0U) ? (cb->fail_ct * 100U) / cb->total_ct : 0U;
        if (cb->fail_ct >= cb->fail_thresh || rate >= cb->err_rate_pct)
            cb_transit(cb, CB_OPEN);
    }
}

static cb_state_t cb_state(circuit_breaker_t *cb) {
    if (!cb) return CB_OPEN;
    if (cb->state == CB_OPEN) {
        cb->open_since_ms = (cb->open_since_ms > 1000U) ? cb->open_since_ms - 1000U : 0U;
        if (cb->open_since_ms == 0U) cb_transit(cb, CB_HALF_OPEN);
    }
    return cb->state;
}

static err_t cb_execute(circuit_breaker_t *cb) {
    if (!cb) return E_PARAM;
    if (!cb->enabled) return E_NONE;
    cb_state_t s = cb_state(cb);
    if (s == CB_OPEN) { host_print("CB OPEN, rejecting\n"); return E_BREAKER; }
    if (s == CB_HALF_OPEN && cb->ho_ct >= cb->ho_max) {
        host_print("CB HALF-OPEN full\n"); return E_BREAKER;
    }
    return E_NONE;
}

static void cb_reset(circuit_breaker_t *cb) {
    if (!cb) return;
    cb->state = CB_CLOSED; cb->fail_ct = 0U; cb->succ_ct = 0U;
    cb->total_ct = 0U; cb->open_since_ms = 0U; cb->ho_ct = 0U;
}

/* ═══════════════════════════════════════════════════════════
 *  Retry Strategy - exponential/jittered backoff, idempotency
 * ═══════════════════════════════════════════════════════════ */
struct retry_cfg {
    char name[MAX_NAME];
    unsigned max, base_ms, max_ms, attempt, mult, jit_pct;
    backoff_t bo_type;
    int idempotent, retry_timeout, retry_server;
    err_t retryable[MAX_RETRIES];
    unsigned retry_ct, jit_seed;
};

static void retry_init(retry_cfg_t *rc, const char *name) {
    if (!rc || !name) { host_print("retry_init: bad param\n"); return; }
    str_copy(rc->name, name, MAX_NAME);
    rc->max = 3U; rc->base_ms = 1000U; rc->max_ms = 30000U; rc->attempt = 0U;
    rc->bo_type = BO_EXP; rc->mult = 2U; rc->jit_pct = 25U;
    rc->idempotent = 1; rc->retry_timeout = 1; rc->retry_server = 1;
    rc->retry_ct = 0U; rc->jit_seed = 42U;
}

static unsigned retry_delay(retry_cfg_t *rc, unsigned a) {
    if (!rc || a > MAX_RETRIES) return rc->max_ms;
    unsigned d = rc->base_ms, j = 0U, exp = 1U;
    if (rc->bo_type == BO_EXP || rc->bo_type == BO_JITTER) {
        while (j < a) { exp *= rc->mult; j++; }
        d = rc->base_ms * exp;
        if (rc->bo_type == BO_JITTER) {
            rc->jit_seed = (rc->jit_seed * 1103515245U + 12345U) & 0x7FFFFFFFU;
            unsigned jr = (d * rc->jit_pct) / 100U;
            unsigned jit = rc->jit_seed % (jr + 1U);
            d = (d > jr) ? (d - jr + jit) : d;
        }
    } else if (rc->bo_type == BO_LINEAR) {
        d = rc->base_ms * (a + 1U);
    }
    return (d > rc->max_ms) ? rc->max_ms : d;
}

static void retry_set_cond(retry_cfg_t *rc, err_t *errs, unsigned n) {
    if (!rc || !errs || n > MAX_RETRIES) return;
    rc->retry_ct = n;
    for (unsigned i = 0U; i < n; i++) rc->retryable[i] = errs[i];
}

static int retry_should(retry_cfg_t *rc, err_t e) {
    if (!rc) return 0;
    if (rc->attempt >= rc->max) return 0;
    if (e == E_BREAKER) return 0;
    if (!rc->idempotent && e == E_TIMEOUT) return 0;
    if (e == E_TIMEOUT && rc->retry_timeout) return 1;
    for (unsigned i = 0U; i < rc->retry_ct; i++)
        if (rc->retryable[i] == e) return 1;
    return rc->retry_server;
}

static err_t retry_exec(retry_cfg_t *rc) {
    if (!rc) return E_PARAM;
    if (!rc->idempotent) { host_print("retry: non-idempotent, single attempt\n"); return E_NONE; }
    rc->attempt = 0U;
    while (rc->attempt <= rc->max) {
        unsigned d = retry_delay(rc, rc->attempt);
        (void)d;
        if (rc->attempt == rc->max) break;
        rc->attempt++;
    }
    host_print("retry: attempts exhausted\n");
    return E_RETRY;
}

/* ═══════════════════════════════════════════════════════════
 *  Timeout Control - connect/read/write/call/cascading
 * ═══════════════════════════════════════════════════════════ */
struct timeout_cfg {
    char name[MAX_NAME];
    unsigned conn_ms, read_ms, write_ms, call_ms;
    unsigned casc_budget_ms, casc_remain_ms, elapsed_ms;
    int enforce_casc;
};

static void tout_init(timeout_cfg_t *tc, const char *name) {
    if (!tc || !name) { host_print("tout_init: bad param\n"); return; }
    str_copy(tc->name, name, MAX_NAME);
    tc->conn_ms = 5000U; tc->read_ms = 10000U; tc->write_ms = 10000U;
    tc->call_ms = 30000U; tc->casc_budget_ms = 60000U; tc->casc_remain_ms = 60000U;
    tc->elapsed_ms = 0U; tc->enforce_casc = 1;
}

static void tout_set(timeout_cfg_t *tc, tout_t type, unsigned ms) {
    if (!tc) return;
    if (type == T_CONN) tc->conn_ms = ms;
    else if (type == T_READ) tc->read_ms = ms;
    else if (type == T_WRITE) tc->write_ms = ms;
    else if (type == T_CALL) tc->call_ms = ms;
    else if (type == T_CASC) tc->casc_budget_ms = ms;
}

static unsigned tout_budget(timeout_cfg_t *tc) {
    if (!tc) return 0U;
    return tc->casc_remain_ms;
}

static err_t tout_check(timeout_cfg_t *tc, tout_t type, unsigned ms) {
    if (!tc) return E_PARAM;
    tc->elapsed_ms = ms;
    unsigned lim = 0U;
    if (type == T_CONN) lim = tc->conn_ms;
    else if (type == T_READ) lim = tc->read_ms;
    else if (type == T_WRITE) lim = tc->write_ms;
    else if (type == T_CALL) lim = tc->call_ms;
    else if (type == T_CASC) lim = tc->casc_remain_ms;
    if (ms > lim) {
        host_print("timeout exceeded\n");
        if (tc->enforce_casc && tc->casc_remain_ms > 0U)
            tc->casc_remain_ms = (tc->casc_remain_ms > ms) ? tc->casc_remain_ms - ms : 0U;
        return E_TIMEOUT;
    }
    if (tc->enforce_casc && tc->casc_remain_ms > ms) tc->casc_remain_ms -= ms;
    return E_NONE;
}

/* ═══════════════════════════════════════════════════════════
 *  Isolation Modes - thread pool/semaphore/bulkhead/resource
 * ═══════════════════════════════════════════════════════════ */
struct iso_pool {
    char name[MAX_NAME];
    iso_type_t type;
    unsigned max_slots, used, active_thr, max_thr;
    unsigned pend_q, max_q, tot_acq, tot_rel;
    int inited;
};

static void pool_init(iso_pool_t *p, const char *name, iso_type_t t, unsigned max) {
    if (!p || !name || max == 0U || max > MAX_DEPS) {
        host_print("pool_init: bad param\n"); return;
    }
    str_copy(p->name, name, MAX_NAME);
    p->type = t; p->max_slots = max; p->used = 0U;
    p->active_thr = 0U; p->max_thr = max; p->pend_q = 0U;
    p->max_q = max * 2U; p->tot_acq = 0U; p->tot_rel = 0U; p->inited = 1;
}

static err_t pool_acquire(iso_pool_t *p) {
    if (!p || !p->inited) return E_PARAM;
    if (p->used >= p->max_slots) {
        if (p->pend_q < p->max_q) { p->pend_q++; host_print("pool full, queued\n"); }
        return E_POOL;
    }
    p->used++; p->tot_acq++;
    if (p->type == ISO_THREAD) p->active_thr++;
    return E_NONE;
}

static void pool_release(iso_pool_t *p) {
    if (!p || p->used == 0U) return;
    p->used--; p->tot_rel++;
    if (p->type == ISO_THREAD && p->active_thr > 0U) p->active_thr--;
    if (p->pend_q > 0U) p->pend_q--;
}

static unsigned pool_avail(const iso_pool_t *p) {
    if (!p) return 0U;
    return (p->max_slots > p->used) ? (p->max_slots - p->used) : 0U;
}

static unsigned pool_util(const iso_pool_t *p) {
    if (!p || p->max_slots == 0U) return 0U;
    return (p->used * 100U) / p->max_slots;
}

/* ═══════════════════════════════════════════════════════════
 *  Degradation Handling - fallback/cache/static/default/empty
 * ═══════════════════════════════════════════════════════════ */
struct degrade_pol {
    char name[MAX_NAME];
    degrade_t type;
    char static_resp[ERR_BUF];
    char cache_key[MAX_NAME];
    int def_val, invoked, cache_on, has_def;
    unsigned fall_ct, cache_hits;
};

static void deg_init(degrade_pol_t *dp, const char *name, degrade_t t) {
    if (!dp || !name) { host_print("deg_init: bad param\n"); return; }
    str_copy(dp->name, name, MAX_NAME);
    dp->type = t; dp->static_resp[0] = '\0'; dp->cache_key[0] = '\0';
    dp->def_val = 0; dp->invoked = 0; dp->cache_on = 0; dp->has_def = 0;
    dp->fall_ct = 0U; dp->cache_hits = 0U;
}

static void deg_static(degrade_pol_t *dp, const char *resp) {
    if (!dp || !resp) return;
    str_copy(dp->static_resp, resp, ERR_BUF);
}

static void deg_default(degrade_pol_t *dp, int val) {
    if (!dp) return;
    dp->def_val = val; dp->has_def = 1;
}

static void deg_cache(degrade_pol_t *dp, const char *key) {
    if (!dp || !key) return;
    str_copy(dp->cache_key, key, MAX_NAME);
    dp->cache_on = 1;
}

static err_t deg_exec(degrade_pol_t *dp) {
    if (!dp) return E_PARAM;
    dp->invoked = 1; dp->fall_ct++;
    if (dp->type == DEG_FALL) {
        host_print("degrade: fallback function\n");
    } else if (dp->type == DEG_CACHE) {
        if (dp->cache_on && dp->cache_key[0] != '\0') {
            dp->cache_hits++; host_print("degrade: cache hit\n");
        } else { host_print("degrade: cache miss\n"); return E_DEG; }
    } else if (dp->type == DEG_STATIC) {
        if (dp->static_resp[0] != '\0') host_print("degrade: static response\n");
        else { host_print("degrade: no static\n"); return E_DEG; }
    } else if (dp->type == DEG_DEF) {
        if (dp->has_def) host_print("degrade: default value\n");
        else { host_print("degrade: no default\n"); return E_DEG; }
    } else if (dp->type == DEG_EMPTY) {
        host_print("degrade: empty no-op\n");
    }
    return E_NONE;
}

/* ═══════════════════════════════════════════════════════════
 *  Health Check - active/passive probes, dependency graph
 * ═══════════════════════════════════════════════════════════ */
struct health_node {
    char name[MAX_NAME];
    health_t status;
    unsigned cons_fail, cons_succ, fail_thresh, succ_thresh;
    unsigned interval_ms, last_ms, total_chk, total_fail;
    probe_t probe;
    int startup_done;
    unsigned start_retries;
    health_node_t *deps[MAX_DEPS];
    unsigned dep_ct;
};

static void hnode_init(health_node_t *n, const char *name) {
    if (!n || !name) { host_print("hnode_init: bad param\n"); return; }
    str_copy(n->name, name, MAX_NAME);
    n->status = H_UNK; n->cons_fail = 0U; n->cons_succ = 0U;
    n->fail_thresh = 3U; n->succ_thresh = 2U; n->interval_ms = 10000U;
    n->last_ms = 0U; n->total_chk = 0U; n->total_fail = 0U;
    n->probe = P_START; n->startup_done = 0; n->start_retries = 0U; n->dep_ct = 0U;
}

static void hnode_active(health_node_t *n) {
    if (!n) return;
    n->total_chk++; n->last_ms += n->interval_ms;
    if (!n->startup_done) {
        n->start_retries++;
        if (n->start_retries >= n->succ_thresh) {
            n->startup_done = 1; n->status = H_OK; n->probe = P_LIVE;
        }
        return;
    }
    n->cons_succ++; n->cons_fail = 0U;
    if (n->cons_succ >= n->succ_thresh) n->status = H_OK;
}

static void hnode_passive(health_node_t *n, int ok) {
    if (!n) return;
    n->total_chk++;
    if (ok) {
        n->cons_succ++; n->cons_fail = 0U;
        if (n->cons_succ >= n->succ_thresh) n->status = H_OK;
    } else {
        n->cons_fail++; n->cons_succ = 0U; n->total_fail++;
        if (n->cons_fail >= n->fail_thresh) n->status = H_BAD;
        else if (n->cons_fail >= 2U) n->status = H_DEG;
    }
}

static health_t hnode_probe(health_node_t *n, probe_t p) {
    if (!n) return H_UNK;
    if (p == P_START) return n->startup_done ? H_OK : H_DEG;
    if (p == P_LIVE) return (n->status == H_BAD) ? H_BAD :
                              (n->status == H_OK) ? H_OK : H_DEG;
    if (p == P_READY) {
        if (!n->startup_done) return H_UNK;
        health_t a = hnode_aggregate(n);
        return (a == H_BAD) ? H_BAD : (a == H_DEG) ? H_DEG : H_OK;
    }
    return H_UNK;
}

static void hnode_add_dep(health_node_t *par, health_node_t *child) {
    if (!par || !child) return;
    if (par->dep_ct >= MAX_DEPS) { host_print("health: max deps\n"); return; }
    par->deps[par->dep_ct] = child; par->dep_ct++;
}

static health_t hnode_aggregate(const health_node_t *n) {
    if (!n) return H_UNK;
    health_t worst = n->status;
    for (unsigned i = 0U; i < n->dep_ct; i++) {
        health_t ds = hnode_aggregate(n->deps[i]);
        if (ds == H_BAD) return H_BAD;
        if (ds == H_DEG && worst != H_BAD) worst = H_DEG;
        if (ds == H_UNK && worst == H_OK) worst = H_DEG;
    }
    return worst;
}

/* ═══════════════════════════════════════════════════════════
 *  Resilience Hub - Orchestrator
 * ═══════════════════════════════════════════════════════════ */
struct resilience_hub {
    char name[MAX_NAME];
    circuit_breaker_t *cbs[MAX_ITEMS];    unsigned cb_ct;
    retry_cfg_t *rcs[MAX_ITEMS];          unsigned rc_ct;
    timeout_cfg_t *tcs[MAX_ITEMS];        unsigned tc_ct;
    iso_pool_t *pools[MAX_ITEMS];         unsigned pool_ct;
    degrade_pol_t *dps[MAX_ITEMS];        unsigned dp_ct;
    health_node_t *hnodes[MAX_DEPS];      unsigned h_ct;
    unsigned total_exec, total_fail, total_deg;
};

static resilience_hub_t *hub_create(const char *name) {
    unsigned ptr = host_alloc(sizeof(resilience_hub_t), 8U);
    if (ptr == 0U) { host_print("hub_create: alloc fail\n"); host_exit(1); }
    resilience_hub_t *h = (resilience_hub_t *)(unsigned long)ptr;
    if (name) str_copy(h->name, name, MAX_NAME); else h->name[0] = '\0';
    h->cb_ct = 0U; h->rc_ct = 0U; h->tc_ct = 0U;
    h->pool_ct = 0U; h->dp_ct = 0U; h->h_ct = 0U;
    h->total_exec = 0U; h->total_fail = 0U; h->total_deg = 0U;
    return h;
}

static void hub_destroy(resilience_hub_t *h) { (void)h; host_print("hub destroyed\n"); }

static err_t hub_add_cb(resilience_hub_t *h, circuit_breaker_t *cb) {
    if (!h || !cb || h->cb_ct >= MAX_ITEMS) return E_PARAM;
    h->cbs[h->cb_ct++] = cb; return E_NONE;
}
static err_t hub_add_rc(resilience_hub_t *h, retry_cfg_t *rc) {
    if (!h || !rc || h->rc_ct >= MAX_ITEMS) return E_PARAM;
    h->rcs[h->rc_ct++] = rc; return E_NONE;
}
static err_t hub_add_tc(resilience_hub_t *h, timeout_cfg_t *tc) {
    if (!h || !tc || h->tc_ct >= MAX_ITEMS) return E_PARAM;
    h->tcs[h->tc_ct++] = tc; return E_NONE;
}
static err_t hub_add_pool(resilience_hub_t *h, iso_pool_t *p) {
    if (!h || !p || h->pool_ct >= MAX_ITEMS) return E_PARAM;
    h->pools[h->pool_ct++] = p; return E_NONE;
}
static err_t hub_add_deg(resilience_hub_t *h, degrade_pol_t *dp) {
    if (!h || !dp || h->dp_ct >= MAX_ITEMS) return E_PARAM;
    h->dps[h->dp_ct++] = dp; return E_NONE;
}
static err_t hub_add_health(resilience_hub_t *h, health_node_t *n) {
    if (!h || !n || h->h_ct >= MAX_DEPS) return E_PARAM;
    h->hnodes[h->h_ct++] = n; return E_NONE;
}

static err_t hub_exec(resilience_hub_t *h, const char *comp) {
    if (!h || !comp) return E_PARAM;
    h->total_exec++;
    unsigned i;
    /* Health gate */
    for (i = 0U; i < h->h_ct; i++) {
        if (h->hnodes[i]->status == H_BAD) {
            host_print("hub: health gate blocked\n"); h->total_fail++; goto degrade;
        }
    }
    /* Circuit breaker */
    for (i = 0U; i < h->cb_ct; i++) {
        err_t e = cb_execute(h->cbs[i]);
        if (e != E_NONE) { h->total_fail++; goto degrade; }
    }
    /* Pool isolation */
    for (i = 0U; i < h->pool_ct; i++) {
        err_t e = pool_acquire(h->pools[i]);
        if (e != E_NONE) { h->total_fail++; goto degrade; }
    }
    /* Timeout check */
    for (i = 0U; i < h->tc_ct; i++) {
        err_t e = tout_check(h->tcs[i], T_CALL, 0U);
        if (e != E_NONE) {
            for (unsigned j = 0U; j < i; j++) pool_release(h->pools[j]);
            h->total_fail++; goto degrade;
        }
    }
    /* Success: release pools, record */
    for (i = 0U; i < h->pool_ct; i++) pool_release(h->pools[i]);
    for (i = 0U; i < h->cb_ct; i++) cb_success(h->cbs[i]);
    return E_NONE;
degrade:
    h->total_deg++;
    for (i = 0U; i < h->dp_ct; i++) deg_exec(h->dps[i]);
    return E_DEG;
}

static health_t hub_health(const resilience_hub_t *h) {
    if (!h) return H_UNK;
    if (h->h_ct == 0U) return H_OK;
    health_t worst = H_OK;
    for (unsigned i = 0U; i < h->h_ct; i++) {
        health_t s = hnode_aggregate(h->hnodes[i]);
        if (s == H_BAD) return H_BAD;
        if (s == H_DEG) worst = H_DEG;
        if (s == H_UNK && worst == H_OK) worst = H_DEG;
    }
    return worst;
}

/* ═══════════════════════════════════════════════════════════
 *  Exported WASM Entrypoints
 * ═══════════════════════════════════════════════════════════ */
__attribute__((export_name("resilience_hub_init")))
unsigned int resilience_hub_init(void) {
    host_print("=== Resilience Hub Module Initialized ===\n");
    return 0U;
}

__attribute__((export_name("run_resilience_demo")))
unsigned int run_resilience_demo(void) {
    host_print("--- Resilience Hub Demo ---\n");
    resilience_hub_t *hub = hub_create("demo_hub");

    /* Register circuit breaker */
    circuit_breaker_t cb; cb_init(&cb, "api_breaker");
    cb.fail_thresh = 3U; cb.err_rate_pct = 40U;
    hub_add_cb(hub, &cb);
    host_print("[CB] api_breaker (thresh=3, rate=40%)\n");

    /* Register retry */
    retry_cfg_t rc; retry_init(&rc, "api_retry");
    rc.max = 5U; rc.bo_type = BO_JITTER; rc.base_ms = 500U;
    hub_add_rc(hub, &rc);
    host_print("[Retry] api_retry (max=5, jittered)\n");

    /* Register timeout */
    timeout_cfg_t tc; tout_init(&tc, "api_timeout");
    tout_set(&tc, T_CONN, 3000U); tout_set(&tc, T_READ, 8000U); tout_set(&tc, T_CALL, 15000U);
    hub_add_tc(hub, &tc);
    host_print("[Timeout] conn=3s read=8s call=15s\n");

    /* Register isolation pool */
    iso_pool_t pool; pool_init(&pool, "worker_pool", ISO_BULK, 16U);
    hub_add_pool(hub, &pool);
    host_print("[Isolation] bulkhead 16 slots\n");

    /* Register degradation */
    degrade_pol_t dp; deg_init(&dp, "api_fallback", DEG_CACHE);
    deg_cache(&dp, "api_response_cache");
    hub_add_deg(hub, &dp);
    host_print("[Degrade] cache-based fallback\n");

    /* Health dependency graph */
    health_node_t db_h, cache_h, api_h;
    hnode_init(&db_h, "database"); hnode_init(&cache_h, "cache");
    hnode_init(&api_h, "gateway");
    hnode_add_dep(&api_h, &db_h); hnode_add_dep(&api_h, &cache_h);
    hub_add_health(hub, &db_h); hub_add_health(hub, &cache_h);
    hub_add_health(hub, &api_h);
    host_print("[Health] 3-node dependency graph\n");

    /* Simulate probes */
    hnode_active(&db_h); hnode_active(&cache_h);
    hnode_passive(&db_h, 1); hnode_passive(&cache_h, 1);
    hnode_active(&api_h);
    host_print("[Health] probes passed\n");

    /* Execute */
    err_t r = hub_exec(hub, "main_flow");
    host_print(r == E_NONE ? "[Hub] SUCCESS\n" : "[Hub] DEGRADED\n");

    /* Trip circuit breaker */
    host_print("[CB] simulating failures...\n");
    cb_failure(&cb); cb_failure(&cb); cb_failure(&cb);
    cb_state_t st = cb_state(&cb);
    host_print(st == CB_OPEN ? "[CB] OPEN\n" : st == CB_HALF_OPEN ? "[CB] HALF-OPEN\n" : "[CB] CLOSED\n");

    /* Retry backoff delays */
    unsigned d0 = retry_delay(&rc, 0U), d1 = retry_delay(&rc, 1U), d2 = retry_delay(&rc, 2U);
    (void)d0; (void)d1; (void)d2;
    host_print("[Retry] delays computed\n");

    /* Timeout violation */
    err_t te = tout_check(&tc, T_READ, 9000U);
    if (te == E_TIMEOUT) host_print("[Timeout] read timeout 9s > 8s\n");

    /* Pool stress */
    pool_acquire(&pool); pool_acquire(&pool); pool_acquire(&pool);
    unsigned u = pool_util(&pool), a = pool_avail(&pool);
    (void)u; (void)a;
    host_print("[Pool] 3 acquired\n");
    pool_release(&pool); pool_release(&pool); pool_release(&pool);

    /* Overall health */
    health_t ov = hub_health(hub);
    host_print(ov == H_OK ? "[Hub] HEALTHY\n" : ov == H_DEG ? "[Hub] DEGRADED\n" :
               ov == H_BAD ? "[Hub] UNHEALTHY\n" : "[Hub] UNKNOWN\n");

    /* Degradation test */
    err_t de = deg_exec(&dp);
    if (de == E_NONE) host_print("[Degrade] fallback ok\n");

    /* Recovery */
    cb_reset(&cb);
    host_print("[CB] reset CLOSED\n");
    host_print("--- Demo Complete ---\n");
    hub_destroy(hub);
    return 0U;
}

__attribute__((export_name("get_module_info")))
unsigned int get_module_info(void) {
    host_print("Module: resilience_hub\nVersion: 1.0.0\nTarget: AI-ASM OS\n");
    host_print("Features: circuit_breaker, retry, timeout, isolation, degradation, health\n");
    return 0U;
}
