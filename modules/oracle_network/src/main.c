/*
 * Oracle Network Module for AI-ASM OS
 * Decentralized data oracle with consensus, verification, and subscriptions.
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

#define MAX_NODES          64U
#define MAX_DATA_SOURCES   32U
#define MAX_REQUESTS       64U
#define MAX_SUBSCRIPTIONS  16U
#define MAX_SIGNATURES      8U
#define MAX_PROOF_SIZE    128U
#define MIN_STAKE        1000U
#define REPUTATION_MAX    100U
#define ANOMALY_FACTOR      2U
#define GAS_LIMIT       500000U
#define BATCH_MAX          16U
#define ORACLE_VERSION 0x00010000U
#define NODE_ACTIVE    0x01U
#define NODE_INACTIVE  0x02U
#define NODE_SLASHED   0x04U
#define NODE_EXITING   0x08U
#define CON_MAJORITY   0x01U
#define CON_MEDIAN     0x02U
#define CON_WEIGHTED   0x03U
#define CON_ANOMALY    0x04U
#define PROOF_CRYPTO   0x01U
#define PROOF_TEE      0x02U
#define PROOF_ZK       0x03U
#define PROOF_MULTISIG 0x04U
#define PROOF_TS       0x05U
#define SUB_PUSH       0x01U
#define SUB_COND       0x02U
#define SUB_SCHED      0x03U
#define SUB_EVENT      0x04U
#define SUB_WEBHOOK    0x05U
#define DS_API       0x01U
#define DS_SENSOR    0x02U
#define DS_MANUAL    0x03U
#define DS_OFFCHAIN  0x04U
#define OK             0x00U
#define E_PARAM        0x02U
#define E_NODE         0x03U
#define E_REQ          0x04U
#define E_STAKE        0x05U
#define E_QUORUM       0x06U
#define E_SUB          0x08U
#define E_FULL         0x09U
#define E_SRC_FULL     0x0AU
#define E_NODE_FULL    0x0BU

typedef struct {
    unsigned int node_id, stake, reputation, status;
    unsigned int tasks_done, tasks_failed, last_active, registered, endpoint;
} oracle_node_t;
typedef struct {
    unsigned int source_id, source_type, endpoint, reliability;
    int value; unsigned int last_update, query_count, error_count;
} data_source_t;
typedef struct {
    unsigned int resp_id, node_id, proof_type, proof_len, timestamp, valid;
    int value; unsigned char proof_data[MAX_PROOF_SIZE];
} source_resp_t;
typedef struct {
    unsigned int req_id, data_type, consensus, min_nodes;
    unsigned int created, deadline, status, resp_count, gas_budget;
    int result; source_resp_t responses[MAX_NODES];
} oracle_req_t;
typedef struct {
    unsigned int sub_id, trigger, data_type, cond_val;
    unsigned int interval, callback, last_fired, active, fire_count;
} subscription_t;

static oracle_node_t   g_nodes[MAX_NODES];
static unsigned int    g_node_count = 0U;
static data_source_t   g_sources[MAX_DATA_SOURCES];
static unsigned int    g_src_count = 0U;
static oracle_req_t    g_reqs[MAX_REQUESTS];
static unsigned int    g_req_count = 0U;
static subscription_t  g_subs[MAX_SUBSCRIPTIONS];
static unsigned int    g_sub_count = 0U;
static unsigned int    g_next_node = 1U, g_next_req = 1U;
static unsigned int    g_next_src = 1U, g_next_sub = 1U;
static unsigned int    g_ts = 0U;
static unsigned int    g_total_rewards = 0U, g_total_penalties = 0U;

static unsigned int oracle_abs_i(int v);
static int oracle_median(int *a, unsigned int n);
static unsigned int count_active(void);
static void log_msg(const char *m);
static unsigned int ds_register(unsigned int t, unsigned int ep);
static int ds_query(unsigned int sid);
static void ds_update_rel(unsigned int sid, unsigned int ok);
static int ds_aggregate(void);
static int con_majority(oracle_req_t *r);
static int con_median(oracle_req_t *r);
static int con_weighted(oracle_req_t *r);
static int con_anomaly(oracle_req_t *r);
static int con_resolve(oracle_req_t *r);
static unsigned int node_find(unsigned int nid);
static unsigned int node_register(unsigned int stake, unsigned int ep);
static unsigned int node_deregister(unsigned int nid);
static void node_assign(unsigned int nid, unsigned int rid);
static void node_reward(unsigned int nid, unsigned int amt);
static void node_penalize(unsigned int nid, unsigned int amt);
static void node_rep_delta(unsigned int nid, int d);
static unsigned int req_create(unsigned int dt, unsigned int cm,
    unsigned int mn, unsigned int dl, unsigned int gb);
static unsigned int req_respond(unsigned int rid, unsigned int nid,
    int val, unsigned int pt, const unsigned char *pf, unsigned int pl);
static int req_validate(const oracle_req_t *r);
static unsigned int req_finalize(unsigned int rid);
static unsigned int req_batch(const unsigned int *dt, unsigned int n);
static unsigned int vfy_crypto(const unsigned char *d, unsigned int l);
static unsigned int vfy_tee(unsigned int nid, const unsigned char *a, unsigned int l);
static unsigned int vfy_zk(const unsigned char *c, unsigned int cl,
    const unsigned char *p, unsigned int pl);
static unsigned int vfy_multisig(const unsigned int *s, unsigned int n,
    const unsigned char *m, unsigned int ml);
static unsigned int vfy_timestamp(unsigned int ts, unsigned int w);
static unsigned int vfy_response(const source_resp_t *r);
static unsigned int sub_create(unsigned int tr, unsigned int dt,
    unsigned int cv, unsigned int iv, unsigned int cb);
static unsigned int sub_cancel(unsigned int sid);
static void sub_fire(unsigned int sid, int v);
static unsigned int sub_chk_cond(const subscription_t *s, int v);
static unsigned int sub_chk_sched(const subscription_t *s);
static void sub_process(int v);
static void oracle_init(void);

static unsigned int oracle_abs_i(int v) {
    return (v < 0) ? (unsigned int)(-v) : (unsigned int)v;
}
static int oracle_median(int *a, unsigned int n) {
    unsigned int i, j; int t;
    if (n == 0U) return 0;
    for (i = 0U; i < n; i++) for (j = i + 1U; j < n; j++)
        if (a[j] < a[i]) { t = a[i]; a[i] = a[j]; a[j] = t; }
    if ((n & 1U) != 0U) return a[n / 2U];
    return (a[(n / 2U) - 1U] + a[n / 2U]) / 2;
}
static unsigned int count_active(void) {
    unsigned int i, c = 0U;
    for (i = 0U; i < g_node_count; i++)
        if ((g_nodes[i].status & NODE_ACTIVE) != 0U) c++;
    return c;
}
static void log_msg(const char *m) { host_print(m); }

static unsigned int ds_register(unsigned int t, unsigned int ep) {
    unsigned int id; data_source_t *s;
    if (g_src_count >= MAX_DATA_SOURCES) { log_msg("ds: full"); return E_SRC_FULL; }
    if (t < DS_API || t > DS_OFFCHAIN) { log_msg("ds: bad type"); return E_PARAM; }
    id = g_next_src++;
    s = &g_sources[g_src_count++];
    s->source_id = id; s->source_type = t; s->endpoint = ep;
    s->reliability = 100U; s->value = 0;
    s->last_update = g_ts; s->query_count = 0U; s->error_count = 0U;
    return id;
}
static int ds_query(unsigned int sid) {
    unsigned int i;
    for (i = 0U; i < g_src_count; i++) if (g_sources[i].source_id == sid) {
        g_sources[i].query_count++; g_sources[i].last_update = g_ts;
        return g_sources[i].value;
    }
    return 0;
}
static void ds_update_rel(unsigned int sid, unsigned int ok) {
    unsigned int i;
    for (i = 0U; i < g_src_count; i++) if (g_sources[i].source_id == sid) {
        if (ok) { if (g_sources[i].reliability < 100U) g_sources[i].reliability++; }
        else { g_sources[i].error_count++;
               if (g_sources[i].reliability > 0U) g_sources[i].reliability--; }
        break;
    }
}
static int ds_aggregate(void) {
    unsigned int i, cnt = 0U; int total = 0;
    for (i = 0U; i < g_src_count; i++)
        if (g_sources[i].reliability >= 50U) { total += g_sources[i].value; cnt++; }
    return (cnt == 0U) ? 0 : total / (int)cnt;
}

static int con_majority(oracle_req_t *r) {
    unsigned int i, j, best_cnt = 0U, cur; int best = 0;
    for (i = 0U; i < r->resp_count; i++) {
        if (!r->responses[i].valid) continue; cur = 0U;
        for (j = 0U; j < r->resp_count; j++)
            if (r->responses[j].valid && r->responses[j].value == r->responses[i].value) cur++;
        if (cur > best_cnt) { best_cnt = cur; best = r->responses[i].value; }
    }
    return best;
}
static int con_median(oracle_req_t *r) {
    int vals[MAX_NODES]; unsigned int i, vc = 0U;
    for (i = 0U; i < r->resp_count; i++)
        if (r->responses[i].valid) vals[vc++] = r->responses[i].value;
    return oracle_median(vals, vc);
}
static int con_weighted(oracle_req_t *r) {
    long wsum = 0L; unsigned int tw = 0U, i, idx, w;
    for (i = 0U; i < r->resp_count; i++) {
        if (!r->responses[i].valid) continue;
        idx = node_find(r->responses[i].node_id);
        if (idx >= g_node_count) continue;
        w = g_nodes[idx].reputation; if (w == 0U) w = 1U;
        wsum += (long)r->responses[i].value * (long)w; tw += w;
    }
    return (tw == 0U) ? 0 : (int)(wsum / (long)tw);
}
static int con_anomaly(oracle_req_t *r) {
    int avg = 0; unsigned int i, vc = 0U, dev, thresh;
    for (i = 0U; i < r->resp_count; i++)
        if (r->responses[i].valid) { avg += r->responses[i].value; vc++; }
    if (vc == 0U) return 0;
    avg /= (int)vc; thresh = 100U * ANOMALY_FACTOR;
    for (i = 0U; i < r->resp_count; i++) {
        if (!r->responses[i].valid) continue;
        dev = oracle_abs_i(r->responses[i].value - avg);
        if (dev > thresh) { r->responses[i].valid = 0U; node_rep_delta(r->responses[i].node_id, -5); }
    }
    avg = 0; vc = 0U;
    for (i = 0U; i < r->resp_count; i++)
        if (r->responses[i].valid) { avg += r->responses[i].value; vc++; }
    return (vc == 0U) ? 0 : avg / (int)vc;
}
static int con_resolve(oracle_req_t *r) {
    switch (r->consensus) {
    case CON_MAJORITY: return con_majority(r);
    case CON_MEDIAN:   return con_median(r);
    case CON_WEIGHTED: return con_weighted(r);
    case CON_ANOMALY:  return con_anomaly(r);
    default: log_msg("con: unknown"); return 0;
    }
}

static unsigned int node_find(unsigned int nid) {
    unsigned int i;
    for (i = 0U; i < g_node_count; i++) if (g_nodes[i].node_id == nid) return i;
    return g_node_count;
}
static unsigned int node_register(unsigned int stake, unsigned int ep) {
    unsigned int id; oracle_node_t *nd;
    if (g_node_count >= MAX_NODES) { log_msg("node: full"); return E_NODE_FULL; }
    if (stake < MIN_STAKE) { log_msg("node: low stake"); return E_STAKE; }
    id = g_next_node++; nd = &g_nodes[g_node_count++];
    nd->node_id = id; nd->stake = stake; nd->reputation = 50U;
    nd->status = NODE_ACTIVE; nd->tasks_done = 0U; nd->tasks_failed = 0U;
    nd->last_active = g_ts; nd->registered = g_ts; nd->endpoint = ep;
    return id;
}
static unsigned int node_deregister(unsigned int nid) {
    unsigned int idx = node_find(nid);
    if (idx >= g_node_count) return E_NODE;
    g_nodes[idx].status = NODE_EXITING; return OK;
}
static void node_assign(unsigned int nid, unsigned int rid) {
    unsigned int idx = node_find(nid);
    if (idx < g_node_count) g_nodes[idx].last_active = g_ts;
    (void)rid;
}
static void node_reward(unsigned int nid, unsigned int amt) {
    unsigned int idx = node_find(nid);
    if (idx < g_node_count) {
        g_nodes[idx].stake += amt; g_nodes[idx].tasks_done++; g_total_rewards += amt;
    }
}
static void node_penalize(unsigned int nid, unsigned int amt) {
    unsigned int idx = node_find(nid);
    if (idx < g_node_count) {
        if (amt > g_nodes[idx].stake) amt = g_nodes[idx].stake;
        g_nodes[idx].stake -= amt; g_nodes[idx].tasks_failed++; g_total_penalties += amt;
        if (g_nodes[idx].stake < MIN_STAKE) g_nodes[idx].status = NODE_SLASHED;
    }
}
static void node_rep_delta(unsigned int nid, int d) {
    unsigned int idx = node_find(nid); int nr;
    if (idx >= g_node_count) return;
    nr = (int)g_nodes[idx].reputation + d;
    if (nr < 0) nr = 0;
    if ((unsigned int)nr > REPUTATION_MAX) nr = (int)REPUTATION_MAX;
    g_nodes[idx].reputation = (unsigned int)nr;
}

static unsigned int req_create(unsigned int dt, unsigned int cm,
    unsigned int mn, unsigned int dl, unsigned int gb) {
    unsigned int id; oracle_req_t *rq;
    if (g_req_count >= MAX_REQUESTS) { log_msg("req: full"); return 0U; }
    if (gb > GAS_LIMIT) gb = GAS_LIMIT;
    { unsigned int ac = count_active(); if (mn > ac) mn = ac; }
    id = g_next_req++; rq = &g_reqs[g_req_count++];
    rq->req_id = id; rq->data_type = dt; rq->consensus = cm;
    rq->min_nodes = mn; rq->created = g_ts;
    rq->deadline = g_ts + dl; rq->result = 0;
    rq->status = 0x01U; rq->resp_count = 0U; rq->gas_budget = gb;
    return id;
}
static unsigned int req_respond(unsigned int rid, unsigned int nid,
    int val, unsigned int pt, const unsigned char *pf, unsigned int pl) {
    unsigned int i; oracle_req_t *rq = NULL; source_resp_t *rsp; unsigned int k;
    for (i = 0U; i < g_req_count; i++)
        if (g_reqs[i].req_id == rid) { rq = &g_reqs[i]; break; }
    if (!rq) return E_REQ;
    if (rq->resp_count >= MAX_NODES) return E_FULL;
    rsp = &rq->responses[rq->resp_count];
    rsp->resp_id = rq->resp_count; rsp->node_id = nid;
    rsp->value = val; rsp->proof_type = pt;
    rsp->timestamp = g_ts; rsp->valid = 1U;
    if (pf && pl > 0U) {
        if (pl > MAX_PROOF_SIZE) pl = MAX_PROOF_SIZE;
        for (k = 0U; k < pl; k++) rsp->proof_data[k] = pf[k];
        rsp->proof_len = pl;
        if (!vfy_response(rsp)) rsp->valid = 0U;
    } else { rsp->proof_len = 0U; }
    rq->resp_count++; node_assign(nid, rid); return OK;
}
static int req_validate(const oracle_req_t *r) {
    unsigned int i, vc = 0U;
    if (r->resp_count < r->min_nodes) return 0;
    for (i = 0U; i < r->resp_count; i++) if (r->responses[i].valid) vc++;
    if (vc < r->min_nodes) return 0;
    if (r->deadline != 0U && g_ts > r->deadline) return 0;
    return 1;
}
static unsigned int req_finalize(unsigned int rid) {
    unsigned int i, j; oracle_req_t *rq = NULL;
    for (i = 0U; i < g_req_count; i++)
        if (g_reqs[i].req_id == rid) { rq = &g_reqs[i]; break; }
    if (!rq) return E_REQ;
    if (!req_validate(rq)) { rq->status = 0x04U; return E_QUORUM; }
    rq->result = con_resolve(rq); rq->status = 0x02U;
    for (j = 0U; j < rq->resp_count; j++) {
        if (rq->responses[j].valid) {
            node_reward(rq->responses[j].node_id, 10U);
            node_rep_delta(rq->responses[j].node_id, 1);
        } else {
            node_penalize(rq->responses[j].node_id, 5U);
            node_rep_delta(rq->responses[j].node_id, -2);
        }
    }
    sub_process(rq->result); return OK;
}
static unsigned int req_batch(const unsigned int *dt, unsigned int n) {
    unsigned int i, created = 0U, id;
    if (n > BATCH_MAX) n = BATCH_MAX;
    for (i = 0U; i < n; i++) {
        id = req_create(dt[i], CON_MEDIAN, 3U, 3600U, GAS_LIMIT);
        if (id) created++;
    }
    return created;
}

static unsigned int vfy_crypto(const unsigned char *d, unsigned int l) {
    unsigned int i, h = 0U;
    if (!d || !l) return 0U;
    for (i = 0U; i < l; i++) h = h * 31U + d[i];
    return h ? 1U : 0U;
}
static unsigned int vfy_tee(unsigned int nid, const unsigned char *a, unsigned int l) {
    unsigned int idx = node_find(nid), i, cs = 0U;
    if (idx >= g_node_count || !a || !l) return 0U;
    if (!(g_nodes[idx].status & NODE_ACTIVE)) return 0U;
    for (i = 0U; i < l; i++) cs = cs * 37U + a[i];
    return cs ? 1U : 0U;
}
static unsigned int vfy_zk(const unsigned char *c, unsigned int cl,
    const unsigned char *p, unsigned int pl) {
    unsigned int i, ch = 0U, ph = 0U;
    if (!c || !cl || !p || !pl) return 0U;
    for (i = 0U; i < cl; i++) ch = ch * 41U + c[i];
    for (i = 0U; i < pl; i++) ph = ph * 43U + p[i];
    return (ch && ph) ? 1U : 0U;
}
static unsigned int vfy_multisig(const unsigned int *s, unsigned int n,
    const unsigned char *m, unsigned int ml) {
    unsigned int i, vs = 0U, idx;
    if (!s || !n || !m || !ml) return 0U;
    if (n > MAX_SIGNATURES) n = MAX_SIGNATURES;
    for (i = 0U; i < n; i++) {
        idx = node_find(s[i]);
        if (idx < g_node_count && (g_nodes[idx].status & NODE_ACTIVE)
            && g_nodes[idx].reputation >= 20U) vs++;
    }
    return (vs > n / 2U) ? 1U : 0U;
}
static unsigned int vfy_timestamp(unsigned int ts, unsigned int w) {
    unsigned int diff;
    if (!w) w = 300U;
    diff = (ts > g_ts) ? (ts - g_ts) : (g_ts - ts);
    return (diff <= w) ? 1U : 0U;
}
static unsigned int vfy_response(const source_resp_t *r) {
    if (!r) return 0U;
    if (!vfy_timestamp(r->timestamp, 600U)) return 0U;
    switch (r->proof_type) {
    case PROOF_CRYPTO:  return vfy_crypto(r->proof_data, r->proof_len);
    case PROOF_TEE:     return vfy_tee(r->node_id, r->proof_data, r->proof_len);
    case PROOF_ZK:
        if (r->proof_len < 4U) return 0U;
        return vfy_zk(r->proof_data, r->proof_len / 2U,
                      r->proof_data + r->proof_len / 2U,
                      r->proof_len - r->proof_len / 2U);
    case PROOF_MULTISIG: return vfy_multisig(&r->node_id, 1U, r->proof_data, r->proof_len);
    case PROOF_TS:       return vfy_timestamp(r->timestamp, 300U);
    default:             return 1U;
    }
}

static unsigned int sub_create(unsigned int tr, unsigned int dt,
    unsigned int cv, unsigned int iv, unsigned int cb) {
    unsigned int id; subscription_t *sub;
    if (g_sub_count >= MAX_SUBSCRIPTIONS) { log_msg("sub: full"); return 0U; }
    if (tr < SUB_PUSH || tr > SUB_WEBHOOK) return 0U;
    id = g_next_sub++; sub = &g_subs[g_sub_count++];
    sub->sub_id = id; sub->trigger = tr; sub->data_type = dt;
    sub->cond_val = cv; sub->interval = iv; sub->callback = cb;
    sub->last_fired = g_ts; sub->active = 1U; sub->fire_count = 0U;
    return id;
}
static unsigned int sub_cancel(unsigned int sid) {
    unsigned int i;
    for (i = 0U; i < g_sub_count; i++)
        if (g_subs[i].sub_id == sid) { g_subs[i].active = 0U; return OK; }
    return E_SUB;
}
static void sub_fire(unsigned int sid, int v) {
    unsigned int i;
    for (i = 0U; i < g_sub_count; i++) if (g_subs[i].sub_id == sid) {
        g_subs[i].last_fired = g_ts; g_subs[i].fire_count++; (void)v; break;
    }
}
static unsigned int sub_chk_cond(const subscription_t *s, int v) {
    if (s->trigger != SUB_COND) return 0U;
    return ((unsigned int)v >= s->cond_val) ? 1U : 0U;
}
static unsigned int sub_chk_sched(const subscription_t *s) {
    if (s->trigger != SUB_SCHED || !s->interval) return 0U;
    return ((g_ts - s->last_fired) >= s->interval) ? 1U : 0U;
}
static void sub_process(int v) {
    unsigned int i, fire;
    for (i = 0U; i < g_sub_count; i++) {
        if (!g_subs[i].active) continue; fire = 0U;
        switch (g_subs[i].trigger) {
        case SUB_PUSH:    fire = 1U; break;
        case SUB_COND:    fire = sub_chk_cond(&g_subs[i], v); break;
        case SUB_SCHED:   fire = sub_chk_sched(&g_subs[i]); break;
        case SUB_EVENT:   fire = 1U; break;
        case SUB_WEBHOOK: fire = 1U; break;
        default: break;
        }
        if (fire) sub_fire(g_subs[i].sub_id, v);
    }
}

static void oracle_init(void) {
    g_node_count = 0U; g_src_count = 0U;
    g_req_count = 0U; g_sub_count = 0U;
    g_next_node = 1U; g_next_req = 1U;
    g_next_src = 1U; g_next_sub = 1U;
    g_ts = 0U; g_total_rewards = 0U; g_total_penalties = 0U;
    log_msg("oracle_network: initialized v1.0");
}

__attribute__((export_name("oracle_start")))
int oracle_start(void) {
    unsigned int n1, n2, s1, rid, sid, fin, bc, k;
    unsigned char dp[8]; unsigned int bt[4];
    oracle_init();
    s1 = ds_register(DS_API, 0x1000U);
    ds_register(DS_SENSOR, 0x2000U);
    ds_register(DS_OFFCHAIN, 0x3000U);
    ds_register(DS_MANUAL, 0x4000U);
    n1 = node_register(5000U, 0xA000U);
    n2 = node_register(3000U, 0xB000U);
    node_register(2000U, 0xC000U);
    rid = req_create(0x01U, CON_WEIGHTED, 2U, 3600U, 100000U);
    for (k = 0U; k < 8U; k++) dp[k] = (unsigned char)(k + 1U);
    req_respond(rid, n1, 42, PROOF_CRYPTO, dp, 8U);
    req_respond(rid, n2, 44, PROOF_CRYPTO, dp, 8U);
    fin = req_finalize(rid);
    sid = sub_create(SUB_PUSH, 0x01U, 0U, 0U, 0xF000U);
    bt[0] = 0x02U; bt[1] = 0x03U; bt[2] = 0x04U; bt[3] = 0x05U;
    bc = req_batch(bt, 4U);
    sub_process(43); sub_cancel(sid);
    (void)ds_aggregate(); (void)node_deregister(n2);
    (void)s1; (void)fin; (void)bc;
    log_msg("oracle_network: demo complete");
    return 0;
}
__attribute__((export_name("oracle_get_version")))
unsigned int oracle_get_version(void) { return ORACLE_VERSION; }
__attribute__((export_name("oracle_get_node_count")))
unsigned int oracle_get_node_count(void) { return g_node_count; }
__attribute__((export_name("oracle_get_request_count")))
unsigned int oracle_get_request_count(void) { return g_req_count; }
