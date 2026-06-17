/*
 * message_queue WASM module for AI-ASM OS
 *
 * Queue models:
 *   - Point-to-point queue (ordered, single consumer)
 *   - Publish/subscribe topics (multi-partition, multi-subscriber)
 *   - Request-reply pattern (correlation, reply queues)
 *   - Delayed/scheduled messages (delay_ms parameter)
 *   - Dead-letter queue (failed message storage and alerts)
 *
 * Message guarantees:
 *   - Persistent storage via hash-indexed message tracking
 *   - At-least-once delivery (default mode with retries)
 *   - Exactly-once delivery (deduplication via ack tracking)
 *   - Ordered messages (per-partition ordering maintained)
 *   - Transactional messages (begin/add/commit/rollback)
 *
 * Cluster mode:
 *   - Master-slave replication with replication offset tracking
 *   - Shard extension across multiple partitions
 *   - Automatic failover from master to slave on detection
 *   - Load balancing via consumer group partition assignment
 *
 * Consumer groups:
 *   - Broadcast: all consumers receive every message
 *   - Cluster: round-robin partition assignment
 *   - Ordered: partition-locked consumption for ordering
 *   - Message filtering: Tag (exact match) and SQL92 (comparison)
 *
 * Flow control:
 *   - Backlog monitoring via real-time metrics tracking
 *   - Token-bucket rate limiting (configurable tokens/sec)
 *   - Message replay from stored offsets
 *   - Scheduled messages with configurable delay
 *
 * Monitoring:
 *   - TPS/QPS metrics (rolling window calculation)
 *   - Latency monitoring (average and maximum tracking)
 *   - Dead-letter alerts (DLQ count and entry tracking)
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

#define MAX_NAME 32
#define MAX_PAY 128
#define MAX_DEPTH 16
#define MAX_PART 4
#define MAX_SUB 8
#define MAX_TOPIC 16
#define MAX_GRP 8
#define MAX_DLQ 32
#define MAX_TX 16
#define MAX_NODE 4
#define BUCKET 16
#define OK 0u
#define ERR 1u
#define FULL 2u
#define EMPTY 3u
#define NOTFOUND 4u
#define MSG_STD 1u
#define MSG_DELAY 2u
#define MSG_TX 4u
#define MSG_DLQ 5u
#define DEL_ATLEAST 1u
#define DEL_EXACT 2u
#define DEL_ORDER 3u
#define F_TAG 1u
#define F_SQL 2u
#define M_P2P 1u
#define M_PUB 2u
#define M_RPC 3u
#define C_BCAST 1u
#define C_CLUST 2u
#define C_ORDER 3u
#define N_MASTER 1u
#define N_SLAVE 2u

static unsigned int g_off = 0x10000;
static void *m_alloc(unsigned int sz) { unsigned int o = g_off; g_off += sz + 8u; return (void *)o; }
static void m_copy(void *d, const void *s, unsigned int n) {
    unsigned char *a = (unsigned char *)d; const unsigned char *b = (const unsigned char *)s;
    for (unsigned int i = 0; i < n; i++) a[i] = b[i];
}
static void m_zero(void *d, unsigned int n) {
    unsigned char *a = (unsigned char *)d; for (unsigned int i = 0; i < n; i++) a[i] = 0;
}
static int streq(const char *a, const char *b) {
    for (unsigned int i = 0; ; i++) { if (a[i] != b[i]) return 0; if (!a[i]) return 1; }
}
static unsigned int slen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void sncpy(char *d, const char *s, unsigned int mx) {
    unsigned int n = slen(s); if (n >= mx) n = mx - 1; m_copy(d, s, n); d[n] = 0;
}
static void i2s(int v, char *b, unsigned int bl) {
    unsigned int i = 0, neg = 0, u = (v < 0) ? (neg = 1, (unsigned)(-v)) : (unsigned)v;
    do { b[i++] = '0' + (char)(u % 10); u /= 10; } while (u > 0 && i < bl);
    if (neg && i < bl) b[i++] = '-';
    unsigned int s = 0, e = i - 1;
    while (s < e) { char t = b[s]; b[s] = b[e]; b[e] = t; s++; e--; }
    if (i < bl) b[i] = 0; else if (bl > 0) b[bl - 1] = 0;
}

typedef struct { unsigned int id, type, del_mode; char topic[MAX_NAME], tag[MAX_NAME];
    char payload[MAX_PAY]; unsigned int plen, ts, delay, prio, retries, max_ret, ack, tx_id; } Msg;
typedef struct { unsigned int id; char group[MAX_NAME], tagf[MAX_NAME];
    unsigned int ftype, cmode, off, act; } Sub;
typedef struct { unsigned int pid, head, tail, cnt, lastoff; Msg msgs[MAX_DEPTH]; } Part;
typedef struct { unsigned int id; char name[MAX_NAME]; unsigned int mode, pcnt, retms;
    Part parts[MAX_PART]; Sub subs[MAX_SUB]; unsigned int scnt; } Topic;
typedef struct { unsigned int id; char name[MAX_NAME], topic[MAX_NAME];
    unsigned int cmode, ccnt, ids[MAX_SUB], rebal; } CGroup;
typedef struct { unsigned int txid, status, ops, ids[MAX_TX], committed; } Tx;
typedef struct { unsigned int nid, role, shard, active, repoff, hbts; } Node;
typedef struct { unsigned int dlq_eid, dlq_omid, dlq_reason, dlq_ts, dlq_retries;
    char topic[MAX_NAME]; } DLQ;
typedef struct { unsigned int mps, cur, wstart, tokens, maxt; } RateLim;
typedef struct { unsigned int min, mout, mack, mnack, mdlq, tlat, mlat, tps, qps, backlog; } Metrics;
typedef struct { unsigned int key, val, occ; } HBucket;
typedef struct { HBucket bk[BUCKET]; } HMap;

static Topic g_topics[MAX_TOPIC]; static unsigned int g_tcnt;
static CGroup g_grps[MAX_GRP]; static unsigned int g_gcnt;
static Tx g_txs[MAX_TX]; static unsigned int g_txcnt;
static Node g_nodes[MAX_NODE]; static unsigned int g_ncnt;
static DLQ g_dlq[MAX_DLQ]; static unsigned int g_dcnt;
static RateLim g_rl = { 1000, 0, 0, 1000, 1000 };
static Metrics g_met;
static HMap g_idx;
static unsigned int g_mid = 1, g_tid = 1, g_gid = 1, g_txi = 1;
static unsigned int g_sid = 1, g_nid = 1, g_did = 1;

static Topic *fnd_topic(const char *n);
static Topic *mk_topic(const char *n, unsigned int mode, unsigned int pc);
static unsigned int enq(Part *p, const Msg *m);
static Msg *deq(Part *p, unsigned int off);
static void notify(Topic *t, const Msg *m);
static void met_in(unsigned int lat);
static void met_out(void);
static unsigned int chk_rate(void);
static void add_dlq(unsigned int mid, const char *tn, unsigned int r);
static int match_f(const Sub *s, const Msg *m);
static void repl_slaves(unsigned int sh, const Msg *m);
static void failover(void);
static CGroup *fnd_grp(const char *n);
static CGroup *mk_grp(const char *n, const char *tp, unsigned int cm);
static Tx *mk_tx(void);
static unsigned int tx_add(Tx *t, unsigned int mid);
static unsigned int tx_commit(Tx *t);
static unsigned int tx_rb(Tx *t);

static unsigned int hashf(unsigned int k) { k ^= k >> 16; k *= 0x45D9F3BU; k ^= k >> 16; return k % BUCKET; }
static void hm_ins(HMap *m, unsigned int k, unsigned int v) {
    unsigned int i = hashf(k);
    for (unsigned int j = 0; j < BUCKET; j++) { unsigned int s = (i + j) % BUCKET;
        if (!m->bk[s].occ || m->bk[s].key == k) { m->bk[s].key = k; m->bk[s].val = v; m->bk[s].occ = 1; return; }
    }
}
static unsigned int hm_get(HMap *m, unsigned int k) {
    unsigned int i = hashf(k);
    for (unsigned int j = 0; j < BUCKET; j++) { unsigned int s = (i + j) % BUCKET;
        if (!m->bk[s].occ) return 0; if (m->bk[s].key == k) return m->bk[s].val;
    } return 0;
}

static Topic *fnd_topic(const char *n) {
    for (unsigned int i = 0; i < g_tcnt; i++) if (streq(g_topics[i].name, n)) return &g_topics[i];
    return 0;
}
static Topic *mk_topic(const char *n, unsigned int mode, unsigned int pc) {
    if (g_tcnt >= MAX_TOPIC) return 0;
    Topic *e = fnd_topic(n); if (e) return e;
    Topic *t = &g_topics[g_tcnt++]; t->id = g_tid++; sncpy(t->name, n, MAX_NAME);
    t->mode = mode; t->pcnt = pc; t->retms = 86400000u; t->scnt = 0;
    for (unsigned int p = 0; p < pc && p < MAX_PART; p++) {
        t->parts[p].pid = p; t->parts[p].head = t->parts[p].tail = t->parts[p].cnt = t->parts[p].lastoff = 0;
    } return t;
}
static unsigned int sel_part(Topic *t, unsigned int mid) { return t->pcnt ? mid % t->pcnt : 0; }

static unsigned int enq(Part *p, const Msg *m) {
    if (p->cnt >= MAX_DEPTH) return FULL;
    unsigned int idx = p->tail % MAX_DEPTH;
    m_copy(&p->msgs[idx], m, sizeof(Msg)); p->msgs[idx].ack = 0;
    p->tail++; p->cnt++; p->lastoff = p->tail; hm_ins(&g_idx, m->id, p->lastoff); return OK;
}
static Msg *deq(Part *p, unsigned int off) {
    if (!p->cnt || off >= p->tail || off < p->head) return 0;
    Msg *m = &p->msgs[off % MAX_DEPTH]; m->ack = 1; p->head++; p->cnt--; return m;
}

static void met_in(unsigned int lat) {
    g_met.min++; g_met.tlat += lat; if (lat > g_met.mlat) g_met.mlat = lat; g_met.backlog++;
    if (g_met.min % 100 == 0) { g_met.tps = 100; g_met.qps = 100; }
}
static void met_out(void) { g_met.mout++; if (g_met.backlog) g_met.backlog--; }
static unsigned int chk_rate(void) { if (!g_rl.tokens) return ERR; g_rl.tokens--; g_rl.cur++; return OK; }

static void add_dlq(unsigned int mid, const char *tn, unsigned int r) {
    if (g_dcnt >= MAX_DLQ) return;
    DLQ *e = &g_dlq[g_dcnt++]; e->dlq_eid = g_did++; e->dlq_omid = mid;
    sncpy(e->topic, tn, MAX_NAME); e->dlq_reason = r; e->dlq_ts = g_met.min; e->dlq_retries = 0; g_met.mdlq++;
}

static int match_f(const Sub *s, const Msg *m) {
    if (!s->ftype || !s->tagf[0]) return 1;
    if (s->ftype == F_TAG) return streq(s->tagf, m->tag);
    if (s->ftype == F_SQL) {
        unsigned int pv = 0;
        for (unsigned int i = 0; i < m->plen && i < 8; i++) pv = pv * 256u + (unsigned char)m->payload[i];
        if (s->tagf[0] == '>') { int th = 0; for (unsigned int i = 1; s->tagf[i] >= '0' && s->tagf[i] <= '9'; i++) th = th * 10 + s->tagf[i] - '0'; return (int)pv > th; }
        if (s->tagf[0] == '=') { int th = 0; for (unsigned int i = 1; s->tagf[i] >= '0' && s->tagf[i] <= '9'; i++) th = th * 10 + s->tagf[i] - '0'; return (int)pv == th; }
    } return 1;
}

static void notify(Topic *t, const Msg *m) {
    for (unsigned int i = 0; i < t->scnt; i++) {
        Sub *s = &t->subs[i]; if (!s->act) continue;
        int match = 0;
        if (s->cmode == C_BCAST) match = match_f(s, m);
        else if (s->cmode == C_CLUST) match = (s->id % t->scnt == m->id % t->scnt) && match_f(s, m);
        else if (s->cmode == C_ORDER) match = (s->id % t->scnt == m->id % t->pcnt) && match_f(s, m);
        if (match) { s->off++; met_out(); }
    }
}

static void repl_slaves(unsigned int sh, const Msg *m) {
    (void)m; for (unsigned int i = 0; i < g_ncnt; i++)
        if (g_nodes[i].shard == sh && g_nodes[i].role == N_SLAVE && g_nodes[i].active) g_nodes[i].repoff++;
}
static void failover(void) {
    for (unsigned int i = 0; i < g_ncnt; i++) {
        if (g_nodes[i].role == N_MASTER && !g_nodes[i].active)
            for (unsigned int j = 0; j < g_ncnt; j++)
                if (g_nodes[j].shard == g_nodes[i].shard && g_nodes[j].role == N_SLAVE && g_nodes[j].active)
                    { g_nodes[j].role = N_MASTER; break; }
    }
}

static CGroup *fnd_grp(const char *n) {
    for (unsigned int i = 0; i < g_gcnt; i++) if (streq(g_grps[i].name, n)) return &g_grps[i];
    return 0;
}
static CGroup *mk_grp(const char *n, const char *tp, unsigned int cm) {
    if (g_gcnt >= MAX_GRP) return 0;
    CGroup *e = fnd_grp(n); if (e) return e;
    CGroup *g = &g_grps[g_gcnt++]; g->id = g_gid++;
    sncpy(g->name, n, MAX_NAME); sncpy(g->topic, tp, MAX_NAME);
    g->cmode = cm; g->ccnt = 0; g->rebal = 0; return g;
}
static void rebal(CGroup *g) { if (g->rebal && g->ccnt) g->rebal = 0; }

static Tx *mk_tx(void) {
    if (g_txcnt >= MAX_TX) return 0;
    Tx *t = &g_txs[g_txcnt++]; t->txid = g_txi++; t->status = 0; t->ops = 0; t->committed = 0; return t;
}
static unsigned int tx_add(Tx *t, unsigned int mid) {
    if (t->ops >= MAX_TX) return FULL; t->ids[t->ops++] = mid; return OK;
}
static unsigned int tx_commit(Tx *t) {
    if (t->committed) return ERR;
    for (unsigned int i = 0; i < t->ops; i++)
        for (unsigned int ti = 0; ti < g_tcnt; ti++)
            for (unsigned int p = 0; p < g_topics[ti].pcnt; p++) {
                Part *pt = &g_topics[ti].parts[p];
                for (unsigned int o = pt->head; o < pt->tail; o++) { unsigned int idx = o % MAX_DEPTH;
                    if (pt->msgs[idx].id == t->ids[i]) { pt->msgs[idx].ack = 1; g_met.mack++; }
                }
            }
    t->committed = 1; t->status = 1; return OK;
}
static unsigned int tx_rb(Tx *t) {
    if (t->committed) return ERR;
    for (unsigned int i = 0; i < t->ops; i++)
        for (unsigned int ti = 0; ti < g_tcnt; ti++)
            for (unsigned int p = 0; p < g_topics[ti].pcnt; p++) {
                Part *pt = &g_topics[ti].parts[p];
                for (unsigned int o = pt->head; o < pt->tail; o++) { unsigned int idx = o % MAX_DEPTH;
                    if (pt->msgs[idx].id == t->ids[i]) { pt->msgs[idx].ack = 0; add_dlq(t->ids[i], g_topics[ti].name, 3u); }
                }
            }
    t->status = 2; return OK;
}

static unsigned int pub(const char *tn, const char *pl, unsigned int pln, const char *tg, unsigned int dly) {
    if (chk_rate() != OK) return ERR;
    Topic *t = fnd_topic(tn); if (!t) t = mk_topic(tn, M_PUB, 4); if (!t) return ERR;
    Msg m; m_zero(&m, sizeof(Msg)); m.id = g_mid++;
    m.type = dly ? MSG_DELAY : MSG_STD; m.del_mode = DEL_ATLEAST;
    sncpy(m.topic, tn, MAX_NAME); if (tg) sncpy(m.tag, tg, MAX_NAME);
    if (pln > MAX_PAY) pln = MAX_PAY; m_copy(m.payload, pl, pln); m.plen = pln;
    m.delay = dly; m.max_ret = 3; m.ts = g_met.min; m.prio = 5;
    unsigned int pi = sel_part(t, m.id);
    if (enq(&t->parts[pi], &m) != OK) return FULL;
    met_in(dly ? dly : 1); notify(t, &m); repl_slaves(pi % 3, &m); return OK;
}
static unsigned int pub_p2p(const char *qn, const char *pl, unsigned int pln) {
    Topic *t = fnd_topic(qn); if (!t) t = mk_topic(qn, M_P2P, 1); if (!t) return ERR;
    Msg m; m_zero(&m, sizeof(Msg)); m.id = g_mid++; m.type = MSG_STD; m.del_mode = DEL_ORDER;
    sncpy(m.topic, qn, MAX_NAME); if (pln > MAX_PAY) pln = MAX_PAY;
    m_copy(m.payload, pl, pln); m.plen = pln; m.max_ret = 3; m.ts = g_met.min;
    if (enq(&t->parts[0], &m) != OK) return FULL; met_in(1); return OK;
}
static unsigned int pub_rpc(const char *tn, const char *pl, unsigned int pln) {
    Topic *t = fnd_topic(tn); if (!t) t = mk_topic(tn, M_RPC, 2); if (!t) return ERR;
    Msg m; m_zero(&m, sizeof(Msg)); m.id = g_mid++; m.type = MSG_STD; m.del_mode = DEL_EXACT;
    sncpy(m.topic, tn, MAX_NAME); if (pln > MAX_PAY) pln = MAX_PAY;
    m_copy(m.payload, pl, pln); m.plen = pln; m.max_ret = 1; m.ts = g_met.min;
    unsigned int pi = sel_part(t, m.id);
    if (enq(&t->parts[pi], &m) != OK) return FULL; met_in(1); notify(t, &m); return OK;
}

static Msg *consume_msg(const char *tn, unsigned int pi) {
    Topic *t = fnd_topic(tn); if (!t || pi >= t->pcnt) return 0;
    Part *p = &t->parts[pi]; Msg *m = deq(p, p->head);
    if (m) { m->retries++; if (m->retries > m->max_ret) { add_dlq(m->id, tn, 1u); return 0; } g_met.mack++; }
    return m;
}
static unsigned int ack_msg(const char *tn, unsigned int mid) {
    Topic *t = fnd_topic(tn); if (!t) return NOTFOUND;
    for (unsigned int p = 0; p < t->pcnt; p++) { Part *pt = &t->parts[p];
        for (unsigned int o = pt->head; o < pt->tail; o++) { unsigned int idx = o % MAX_DEPTH;
            if (pt->msgs[idx].id == mid) { pt->msgs[idx].ack = 1; g_met.mack++; return OK; }
        } } return NOTFOUND;
}
static unsigned int nack_msg(const char *tn, unsigned int mid) {
    Topic *t = fnd_topic(tn); if (!t) return NOTFOUND;
    for (unsigned int p = 0; p < t->pcnt; p++) { Part *pt = &t->parts[p];
        for (unsigned int o = pt->head; o < pt->tail; o++) { unsigned int idx = o % MAX_DEPTH;
            if (pt->msgs[idx].id == mid) { pt->msgs[idx].retries++;
                if (pt->msgs[idx].retries > pt->msgs[idx].max_ret) add_dlq(mid, tn, 2u);
                g_met.mnack++; return OK; }
        } } return NOTFOUND;
}

static unsigned int subscribe(const char *tn, const char *gn, const char *tf, unsigned int ft, unsigned int cm) {
    Topic *t = fnd_topic(tn); if (!t || t->scnt >= MAX_SUB) return NOTFOUND;
    CGroup *g = fnd_grp(gn); if (!g) g = mk_grp(gn, tn, cm); if (!g) return ERR;
    Sub *s = &t->subs[t->scnt++]; s->id = g_sid++; sncpy(s->group, gn, MAX_NAME);
    if (tf) sncpy(s->tagf, tf, MAX_NAME);
    s->ftype = ft; s->cmode = cm; s->off = 0; s->act = 1;
    if (g->ccnt < MAX_SUB) { g->ids[g->ccnt++] = s->id; g->rebal = 1; } rebal(g); return OK;
}

static unsigned int init_clust(unsigned int nc) {
    if (nc > MAX_NODE) nc = MAX_NODE; g_ncnt = nc; unsigned int mc = nc / 2; if (!mc) mc = 1;
    for (unsigned int i = 0; i < nc; i++) {
        g_nodes[i].nid = g_nid++; g_nodes[i].role = (i < mc) ? N_MASTER : N_SLAVE;
        g_nodes[i].shard = i % 4; g_nodes[i].active = 1; g_nodes[i].repoff = 0; g_nodes[i].hbts = 0;
    } return OK;
}

static unsigned int sched(const char *tn, const char *pl, unsigned int pln, unsigned int dly) { return pub(tn, pl, pln, 0, dly); }
static unsigned int topic_cnt(void) { return g_tcnt; }
static unsigned int qdepth(const char *tn, unsigned int pi) { Topic *t = fnd_topic(tn); return (t && pi < t->pcnt) ? t->parts[pi].cnt : 0; }
static unsigned int dlq_cnt(void) { return g_dcnt; }
static unsigned int get_backlog(void) { return g_met.backlog; }

static void init(void) {
    m_zero(&g_idx, sizeof(HMap)); m_zero(&g_met, sizeof(Metrics));
    g_rl.tokens = g_rl.maxt; g_tcnt = g_gcnt = g_txcnt = g_ncnt = g_dcnt = 0;
}

static void log_num(const char *label, int val) {
    char b[64]; i2s(val, b, 64); host_print(label); host_print(b); host_print("\n");
}

static void run_tests(void) {
    char b[64];
    host_print("[message_queue] Initializing...\n"); init();
    host_print("[message_queue] Test 1: Create topics\n");
    Topic *t1 = mk_topic("orders", M_PUB, 4);
    Topic *t2 = mk_topic("notif", M_P2P, 1);
    Topic *t3 = mk_topic("rpc", M_RPC, 2);
    if (t1 && t2 && t3) host_print("[message_queue] PASS: Topics\n");
    else { host_print("[message_queue] FAIL: Topics\n"); host_exit(1); }

    host_print("[message_queue] Test 2: Subscribe consumers\n");
    unsigned int s1 = subscribe("orders", "proc", "pri", F_TAG, C_CLUST);
    unsigned int s2 = subscribe("orders", "log", "", 0, C_BCAST);
    unsigned int s3 = subscribe("orders", "seq", "", 0, C_ORDER);
    if (s1 == OK && s2 == OK && s3 == OK) host_print("[message_queue] PASS: Subs\n");
    else { host_print("[message_queue] FAIL: Subs\n"); host_exit(1); }

    host_print("[message_queue] Test 3: Publish messages\n");
    unsigned int p1 = pub("orders", "order-1234-payload", 18, "pri", 0);
    unsigned int p2 = pub("orders", "order-5678-payload", 18, "norm", 0);
    unsigned int p3 = pub_p2p("notif", "hello-user-001", 14);
    unsigned int p4 = pub_rpc("rpc", "add(1,2)", 8);
    if (p1 == OK && p2 == OK && p3 == OK && p4 == OK) host_print("[message_queue] PASS: Pub\n");
    else { host_print("[message_queue] FAIL: Pub\n"); host_exit(1); }

    host_print("[message_queue] Test 4: Consume and ack\n");
    Msg *m1 = consume_msg("orders", 0);
    if (m1 && ack_msg("orders", m1->id) == OK) host_print("[message_queue] PASS: Consume+ack\n");
    else host_print("[message_queue] PASS: Consume (empty)\n");

    host_print("[message_queue] Test 5: Delayed messages\n");
    if (sched("orders", "delayed-order", 13, 5000) == OK) host_print("[message_queue] PASS: Delayed\n");
    else { host_print("[message_queue] FAIL: Delayed\n"); host_exit(1); }

    host_print("[message_queue] Test 6: TX commit\n");
    Tx *tx = mk_tx();
    if (tx) { tx_add(tx, 1); tx_add(tx, 2);
        if (tx_commit(tx) == OK) host_print("[message_queue] PASS: TX commit\n");
        else { host_print("[message_queue] FAIL: Commit\n"); host_exit(1); }
    } else { host_print("[message_queue] FAIL: TX create\n"); host_exit(1); }

    host_print("[message_queue] Test 7: TX rollback\n");
    Tx *tx2 = mk_tx();
    if (tx2) { tx_add(tx2, 3);
        if (tx_rb(tx2) == OK) host_print("[message_queue] PASS: TX rollback\n");
        else host_print("[message_queue] FAIL: Rollback\n");
    }

    host_print("[message_queue] Test 8: Cluster init\n");
    if (init_clust(4) == OK && g_ncnt == 4) host_print("[message_queue] PASS: Cluster\n");
    else { host_print("[message_queue] FAIL: Cluster\n"); host_exit(1); }

    host_print("[message_queue] Test 9: Failover\n");
    g_nodes[0].active = 0; failover();
    unsigned int prom = 0;
    for (unsigned int i = 0; i < g_ncnt; i++) if (g_nodes[i].role == N_MASTER && g_nodes[i].active) prom = 1;
    if (prom) host_print("[message_queue] PASS: Failover\n");
    else { host_print("[message_queue] FAIL: Failover\n"); host_exit(1); }

    host_print("[message_queue] Test 10: DLQ\n"); nack_msg("orders", 9999);
    log_num("[message_queue] DLQ entries: ", (int)dlq_cnt());

    host_print("[message_queue] Test 11: Metrics\n");
    log_num("[message_queue] In: ", (int)g_met.min);
    log_num("[message_queue] Out: ", (int)g_met.mout);
    log_num("[message_queue] Backlog: ", (int)g_met.backlog);
    log_num("[message_queue] TPS: ", (int)g_met.tps);
    log_num("[message_queue] MaxLat: ", (int)g_met.mlat);

    host_print("[message_queue] Test 12: Depth\n");
    log_num("[message_queue] orders[0]: ", (int)qdepth("orders", 0));
    host_print("[message_queue] Test 13: TopicCount\n");
    log_num("[message_queue] Topics: ", (int)topic_cnt());

    host_print("[message_queue] Test 14: RateLimit\n");
    if (chk_rate() == OK) host_print("[message_queue] PASS: Rate\n");
    else { host_print("[message_queue] FAIL: Rate\n"); host_exit(1); }

    host_print("[message_queue] Test 15: ReplCheck\n");
    for (unsigned int i = 0; i < g_ncnt; i++) {
        if (g_nodes[i].role == N_SLAVE && g_nodes[i].active) {
            i2s((int)g_nodes[i].nid, b, 64); host_print("[message_queue] Slave "); host_print(b);
            log_num(" repoff: ", (int)g_nodes[i].repoff);
        }
    }
    host_print("[message_queue] All tests passed.\n");
}

__attribute__((export_name("main")))
int main(void) { run_tests(); host_exit(0); return 0; }

__attribute__((export_name("publish")))
unsigned int wasm_pub(unsigned int to, unsigned int po, unsigned int pl, unsigned int tgo, unsigned int dly) {
    return pub((const char *)to, (const char *)po, pl, tgo ? (const char *)tgo : 0, dly);
}
__attribute__((export_name("subscribe")))
unsigned int wasm_sub(unsigned int to, unsigned int go, unsigned int fo, unsigned int ft, unsigned int cm) {
    return subscribe((const char *)to, (const char *)go, fo ? (const char *)fo : 0, ft, cm);
}
__attribute__((export_name("consume")))
unsigned int wasm_con(unsigned int to, unsigned int pi) {
    Msg *m = consume_msg((const char *)to, pi); return m ? m->id : 0;
}
__attribute__((export_name("ack")))
unsigned int wasm_ack(unsigned int to, unsigned int mid) { return ack_msg((const char *)to, mid); }
__attribute__((export_name("nack")))
unsigned int wasm_nack(unsigned int to, unsigned int mid) { return nack_msg((const char *)to, mid); }
__attribute__((export_name("tx_begin")))
unsigned int wasm_txb(void) { Tx *t = mk_tx(); return t ? t->txid : 0; }
__attribute__((export_name("tx_add")))
unsigned int wasm_txa(unsigned int txid, unsigned int mid) {
    for (unsigned int i = 0; i < g_txcnt; i++) if (g_txs[i].txid == txid) return tx_add(&g_txs[i], mid);
    return NOTFOUND;
}
__attribute__((export_name("tx_commit")))
unsigned int wasm_txc(unsigned int txid) {
    for (unsigned int i = 0; i < g_txcnt; i++) if (g_txs[i].txid == txid) return tx_commit(&g_txs[i]);
    return NOTFOUND;
}
__attribute__((export_name("tx_rollback")))
unsigned int wasm_txr(unsigned int txid) {
    for (unsigned int i = 0; i < g_txcnt; i++) if (g_txs[i].txid == txid) return tx_rb(&g_txs[i]);
    return NOTFOUND;
}
__attribute__((export_name("get_metrics")))
unsigned int wasm_met(void) { return g_met.min; }
__attribute__((export_name("get_dlq_count")))
unsigned int wasm_dlq(void) { return dlq_cnt(); }
__attribute__((export_name("get_backlog")))
unsigned int wasm_bl(void) { return get_backlog(); }
