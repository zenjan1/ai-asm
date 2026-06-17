/* notification_service - Multi-channel notification service for AI-ASM OS */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_CH 5
#define MAX_TPL 64
#define MAX_NOTIF 256
#define MAX_USR 128
#define MAX_GRP 32
#define MAX_PQ 64
#define MAX_RETRY 4
#define MAX_STATS 24
#define MAX_WS 64
#define MAX_BATCH 32
#define SB 64
#define TB 256

#define C_EMAIL 0
#define C_SMS 1
#define C_PUSH 2
#define C_INAPP 3
#define C_WS 4

#define S_PENDING 0
#define S_SENT 1
#define S_DELIVERED 2
#define S_READ 3
#define S_FAILED 4
#define S_RETRYING 5

#define ST_IMM 0
#define ST_SCHED 1
#define ST_BATCH 2
#define ST_PRIO 3

#define E_NONE 0
#define E_ALLOC 1
#define E_NOTFOUND 2
#define E_INVCHAN 3
#define E_NOTPL 4
#define E_NOSUB 5
#define E_DND 6
#define E_GW 7
#define E_RETRY 8
#define E_INVIN 9
#define E_QFULL 10
#define E_WSDC 11

typedef struct {
    unsigned int id, type, enabled;
    char name[SB], ep[SB];
} channel_t;

typedef struct {
    unsigned int id, ab_group, use_count;
    char key[SB], subj[SB], body[TB], lang[8];
} template_t;

typedef struct {
    unsigned int uid, chan_mask, dnd_s, dnd_e, grp_mask;
    char lang[8];
} upref_t;

typedef struct {
    unsigned int id, uid, tid, chan, pri, status, strat;
    unsigned int rcnt, rdel, t_cr, t_sched, t_sent, t_del, t_read;
    unsigned int ec;
    char fail[SB];
} notif_t;

typedef struct {
    unsigned int nids[MAX_PQ], pris[MAX_PQ], cnt;
} pqueue_t;

typedef struct {
    unsigned int gid, uids[MAX_USR], ucnt;
    char name[SB];
} grp_t;

typedef struct {
    unsigned int wid, uid, conn, pmsg;
} wsconn_t;

typedef struct {
    unsigned int hr, sent, del, fail, rd;
} stbucket_t;

typedef struct {
    unsigned int ts, td, tf, tr;
    unsigned int bych[MAX_CH], byst[6];
    stbucket_t hr[MAX_STATS];
} stats_t;
/* Forward declarations */
static unsigned int svc_init(void);
static unsigned int svc_shutdown(void);
static channel_t *chan_get(unsigned int id);
static unsigned int chan_reg(unsigned int type, const char *name, const char *ep);
static template_t *tpl_find(const char *key, const char *lang);
static unsigned int tpl_create(const char *key, const char *subj, const char *body,
                               const char *lang, unsigned int ab);
static unsigned int tpl_subst(const template_t *t, const char *v, char *o, unsigned int sz);
static unsigned int notif_create(unsigned int uid, unsigned int tid, unsigned int ch,
                                 unsigned int pri, unsigned int strat);
static unsigned int notif_send(unsigned int nid);
static unsigned int notif_send_ch(const notif_t *n, const char *r);
static unsigned int notif_retry(unsigned int nid);
static unsigned int user_get_pref(unsigned int uid, upref_t *out);
static unsigned int user_set_pref(unsigned int uid, unsigned int cm, const char *l,
                                  unsigned int ds, unsigned int de, unsigned int gm);
static unsigned int user_is_dnd(const upref_t *p, unsigned int h);
static unsigned int grp_create(const char *name);
static unsigned int grp_add(unsigned int gid, unsigned int uid);
static unsigned int grp_send(unsigned int gid, unsigned int tid, unsigned int ch, unsigned int pri);
static unsigned int q_push(pqueue_t *q, unsigned int nid, unsigned int pri);
static unsigned int q_pop(pqueue_t *q, unsigned int *nid);
static unsigned int q_process(pqueue_t *q);
static unsigned int batch_send(unsigned int *ids, unsigned int cnt);
static unsigned int ws_connect(unsigned int uid);
static unsigned int ws_disconnect(unsigned int wid);
static unsigned int ws_broadcast(unsigned int gid, const char *msg);
static unsigned int stat_rec(unsigned int ch, unsigned int st);
static unsigned int stat_summary(stats_t *out);
static unsigned int stat_hourly(stbucket_t *out, unsigned int max);
static unsigned int track_del(unsigned int nid, unsigned int st);
static unsigned int track_read(unsigned int nid);
static unsigned int analyze_fail(unsigned int *codes, unsigned int max);

static channel_t   g_ch[MAX_CH];
static unsigned int g_chc;
static template_t  g_tpl[MAX_TPL];
static unsigned int g_tplc;
static notif_t     g_ntf[MAX_NOTIF];
static unsigned int g_ntfc;
static upref_t     g_upr[MAX_USR];
static unsigned int g_uprc;
static grp_t       g_grp[MAX_GRP];
static unsigned int g_grpc;
static pqueue_t    g_pq;
static wsconn_t    g_ws[MAX_WS];
static unsigned int g_wsc;
static stats_t     g_st;
static unsigned int g_nid = 1, g_tid = 1, g_init;

static unsigned int svc_init(void) {
    unsigned int i;
    if (g_init) return E_NONE;
    g_chc = g_tplc = g_ntfc = g_uprc = g_grpc = g_wsc = 0;
    g_nid = g_tid = 1;
    g_pq.cnt = 0;
    for (i = 0; i < MAX_CH; i++) { g_ch[i].id = g_ch[i].enabled = 0; }
    for (i = 0; i < MAX_NOTIF; i++) { g_ntf[i].id = 0; g_ntf[i].rcnt = 0; }
    for (i = 0; i < MAX_WS; i++) { g_ws[i].wid = g_ws[i].conn = g_ws[i].pmsg = 0; }
    g_st.ts = g_st.td = g_st.tf = g_st.tr = 0;
    for (i = 0; i < MAX_CH; i++) g_st.bych[i] = 0;
    for (i = 0; i < 6; i++) g_st.byst[i] = 0;
    for (i = 0; i < MAX_STATS; i++) {
        g_st.hr[i].hr = i; g_st.hr[i].sent = g_st.hr[i].del = g_st.hr[i].fail = g_st.hr[i].rd = 0;
    }
    g_init = 1;
    host_print("[notification_service] initialized\n");
    return E_NONE;
}

static unsigned int svc_shutdown(void) {
    if (!g_init) return E_NONE;
    host_print("[notification_service] shutting down\n");
    g_init = 0;
    return E_NONE;
}

static channel_t *chan_get(unsigned int id) {
    unsigned int i;
    for (i = 0; i < g_chc; i++) if (g_ch[i].id == id) return &g_ch[i];
    return (channel_t *)0;
}

static unsigned int chan_reg(unsigned int type, const char *name, const char *ep) {
    channel_t *c; unsigned int i;
    if (type > C_WS) return E_INVCHAN;
    if (g_chc >= MAX_CH) return E_QFULL;
    c = &g_ch[g_chc++];
    c->id = type + 1; c->type = type; c->enabled = 1;
    for (i = 0; i < SB && name[i]; i++) c->name[i] = name[i]; c->name[i] = '\0';
    for (i = 0; i < SB && ep[i]; i++) c->ep[i] = ep[i]; c->ep[i] = '\0';
    host_print("[notification_service] channel registered\n");
    return E_NONE;
}

static template_t *tpl_find(const char *key, const char *lang) {
    unsigned int i, k; template_t *fb = (template_t *)0;
    for (i = 0; i < g_tplc; i++) {
        unsigned int m = 1;
        for (k = 0; key[k]; k++) { if (g_tpl[i].key[k] != key[k]) { m = 0; break; } }
        if (m && g_tpl[i].key[k]) m = 0;
        if (!m) continue;
        if (lang[0]) {
            unsigned int lm = 1;
            for (k = 0; lang[k]; k++) { if (g_tpl[i].lang[k] != lang[k]) { lm = 0; break; } }
            if (lm && g_tpl[i].lang[k] == '\0') return &g_tpl[i];
        }
        if (!fb) fb = &g_tpl[i];
    }
    return fb;
}

static unsigned int tpl_create(const char *key, const char *subj, const char *body,
                               const char *lang, unsigned int ab) {
    template_t *t; unsigned int i;
    if (g_tplc >= MAX_TPL) return E_ALLOC;
    t = &g_tpl[g_tplc++];
    t->id = g_tid++; t->ab_group = ab; t->use_count = 0;
    for (i = 0; i < SB && key[i]; i++) t->key[i] = key[i]; t->key[i] = '\0';
    for (i = 0; i < SB && subj[i]; i++) t->subj[i] = subj[i]; t->subj[i] = '\0';
    for (i = 0; i < TB && body[i]; i++) t->body[i] = body[i]; t->body[i] = '\0';
    for (i = 0; i < 8 && lang[i]; i++) t->lang[i] = lang[i]; t->lang[i] = '\0';
    return E_NONE;
}

static unsigned int tpl_subst(const template_t *t, const char *v, char *o, unsigned int sz) {
    unsigned int si = 0, di = 0, vi;
    if (!t || !o) return E_INVIN;
    while (t->body[si] && di < sz - 1) {
        if (t->body[si] == '{' && t->body[si+1] == '{') {
            si += 2; vi = 0;
            if (v) while (v[vi] && di < sz - 1) o[di++] = v[vi++];
            while (t->body[si] && t->body[si] != '}') si++;
            if (t->body[si] == '}' && t->body[si+1] == '}') si += 2;
        } else if (t->body[si] == '%' && t->body[si+1] == '?') {
            unsigned int dep = 1; si += 2;
            if (!v || !v[0]) {
                while (t->body[si] && dep > 0) {
                    if (t->body[si] == '%' && t->body[si+1] == '?') { dep++; si += 2; }
                    else if (t->body[si] == '%' && t->body[si+1] == ':') { dep--; si += 2; }
                    else si++;
                }
            }
        } else { o[di++] = t->body[si++]; }
    }
    o[di] = '\0';
    return E_NONE;
}

static notif_t *ntf_byid(unsigned int id) {
    unsigned int i;
    for (i = 0; i < g_ntfc; i++) if (g_ntf[i].id == id) return &g_ntf[i];
    return (notif_t *)0;
}

static unsigned int notif_create(unsigned int uid, unsigned int tid, unsigned int ch,
                                 unsigned int pri, unsigned int strat) {
    notif_t *n;
    if (g_ntfc >= MAX_NOTIF) return E_QFULL;
    if (ch > C_WS) return E_INVCHAN;
    n = &g_ntf[g_ntfc++];
    n->id = g_nid++; n->uid = uid; n->tid = tid; n->chan = ch;
    n->pri = pri; n->strat = strat; n->status = S_PENDING;
    n->rcnt = 0; n->rdel = 1; n->ec = E_NONE;
    n->t_cr = 1; n->t_sched = n->t_sent = n->t_del = n->t_read = 0;
    n->fail[0] = '\0';
    if (strat == ST_PRIO) q_push(&g_pq, n->id, pri);
    return n->id;
}

static unsigned int notif_send_ch(const notif_t *n, const char *r) {
    unsigned int i, found;
    (void)r;
    switch (n->chan) {
    case C_EMAIL: host_print("[notification_service] sending email\n"); return E_NONE;
    case C_SMS:   host_print("[notification_service] sending sms\n"); return E_NONE;
    case C_PUSH:  host_print("[notification_service] sending push\n"); return E_NONE;
    case C_INAPP: host_print("[notification_service] sending in-app\n"); return E_NONE;
    case C_WS:
        found = 0;
        for (i = 0; i < g_wsc; i++)
            if (g_ws[i].uid == n->uid && g_ws[i].conn) { g_ws[i].pmsg++; found = 1; }
        if (!found) return E_WSDC;
        host_print("[notification_service] sending websocket\n");
        return E_NONE;
    default: return E_INVCHAN;
    }
}

static unsigned int notif_send(unsigned int nid) {
    notif_t *n; upref_t p; template_t *tpl; char rend[TB];
    unsigned int err, ret, ti;
    n = ntf_byid(nid);
    if (!n) return E_NOTFOUND;
    err = user_get_pref(n->uid, &p);
    if (err == E_NONE) {
        if (user_is_dnd(&p, 22)) { n->status = S_PENDING; return E_DND; }
        if (!(p.chan_mask & (1 << n->chan))) return E_NOSUB;
    }
    tpl = (template_t *)0;
    for (ti = 0; ti < g_tplc; ti++) if (g_tpl[ti].id == n->tid) { tpl = &g_tpl[ti]; break; }
    if (!tpl) return E_NOTPL;
    tpl->use_count++;
    err = tpl_subst(tpl, (const char *)0, rend, TB);
    if (err != E_NONE) return err;
    ret = notif_send_ch(n, rend);
    if (ret == E_NONE) { n->status = S_SENT; n->t_sent = 2; stat_rec(n->chan, S_SENT); }
    else { n->status = S_FAILED; n->ec = ret; stat_rec(n->chan, S_FAILED); }
    return ret;
}

static unsigned int notif_retry(unsigned int nid) {
    notif_t *n; unsigned int i, d = 1;
    n = ntf_byid(nid);
    if (!n) return E_NOTFOUND;
    if (n->rcnt >= MAX_RETRY) { n->status = S_FAILED; n->ec = E_RETRY; return E_RETRY; }
    for (i = 0; i < n->rcnt; i++) d *= 2;
    n->rcnt++; n->rdel = d; n->status = S_RETRYING;
    host_print("[notification_service] retry with backoff\n");
    return notif_send(nid);
}

static upref_t *upr_find(unsigned int uid) {
    unsigned int i;
    for (i = 0; i < g_uprc; i++) if (g_upr[i].uid == uid) return &g_upr[i];
    return (upref_t *)0;
}

static unsigned int user_get_pref(unsigned int uid, upref_t *out) {
    upref_t *p = upr_find(uid);
    if (!p) return E_NOTFOUND;
    *out = *p; return E_NONE;
}

static unsigned int user_set_pref(unsigned int uid, unsigned int cm, const char *l,
                                  unsigned int ds, unsigned int de, unsigned int gm) {
    upref_t *p; unsigned int i;
    p = upr_find(uid);
    if (!p) { if (g_uprc >= MAX_USR) return E_ALLOC; p = &g_upr[g_uprc++]; p->uid = uid; }
    p->chan_mask = cm; p->dnd_s = ds; p->dnd_e = de; p->grp_mask = gm;
    for (i = 0; i < 8 && l[i]; i++) p->lang[i] = l[i]; p->lang[i] = '\0';
    return E_NONE;
}

static unsigned int user_is_dnd(const upref_t *p, unsigned int h) {
    if (!p->dnd_s && !p->dnd_e) return 0;
    if (p->dnd_s < p->dnd_e) return (h >= p->dnd_s && h < p->dnd_e);
    return (h >= p->dnd_s || h < p->dnd_e);
}

static unsigned int grp_create(const char *name) {
    grp_t *g; unsigned int i;
    if (g_grpc >= MAX_GRP) return E_ALLOC;
    g = &g_grp[g_grpc++]; g->gid = g_grpc; g->ucnt = 0;
    for (i = 0; i < SB && name[i]; i++) g->name[i] = name[i]; g->name[i] = '\0';
    return g->gid;
}

static unsigned int grp_add(unsigned int gid, unsigned int uid) {
    unsigned int i;
    for (i = 0; i < g_grpc; i++) {
        if (g_grp[i].gid != gid) continue;
        if (g_grp[i].ucnt >= MAX_USR) return E_QFULL;
        g_grp[i].uids[g_grp[i].ucnt++] = uid; return E_NONE;
    }
    return E_NOTFOUND;
}

static unsigned int grp_send(unsigned int gid, unsigned int tid, unsigned int ch, unsigned int pri) {
    unsigned int i, j, nid, sent = 0;
    for (i = 0; i < g_grpc; i++) {
        if (g_grp[i].gid != gid) continue;
        for (j = 0; j < g_grp[i].ucnt; j++) {
            nid = notif_create(g_grp[i].uids[j], tid, ch, pri, ST_BATCH);
            if (nid > 0 && notif_send(nid) == E_NONE) sent++;
        }
    }
    return sent;
}

static unsigned int q_push(pqueue_t *q, unsigned int nid, unsigned int pri) {
    unsigned int i, ip = q->cnt;
    if (q->cnt >= MAX_PQ) return E_QFULL;
    for (i = 0; i < q->cnt; i++) if (pri > q->pris[i]) { ip = i; break; }
    for (i = q->cnt; i > ip; i--) { q->nids[i] = q->nids[i-1]; q->pris[i] = q->pris[i-1]; }
    q->nids[ip] = nid; q->pris[ip] = pri; q->cnt++;
    return E_NONE;
}

static unsigned int q_pop(pqueue_t *q, unsigned int *nid) {
    unsigned int i;
    if (!q->cnt) return E_NOTFOUND;
    *nid = q->nids[0];
    for (i = 0; i < q->cnt - 1; i++) { q->nids[i] = q->nids[i+1]; q->pris[i] = q->pris[i+1]; }
    q->cnt--;
    return E_NONE;
}

static unsigned int q_process(pqueue_t *q) {
    unsigned int nid, err, proc = 0;
    while (q->cnt > 0) {
        err = q_pop(q, &nid);
        if (err != E_NONE) break;
        err = notif_send(nid);
        if (err == E_DND || err == E_WSDC) { q_push(q, nid, 1); break; }
        proc++;
    }
    return proc;
}

static unsigned int batch_send(unsigned int *ids, unsigned int cnt) {
    unsigned int i, s = 0, bc = (cnt > MAX_BATCH) ? MAX_BATCH : cnt;
    for (i = 0; i < bc; i++) if (notif_send(ids[i]) == E_NONE) s++;
    return s;
}

static unsigned int ws_connect(unsigned int uid) {
    wsconn_t *w; unsigned int i;
    for (i = 0; i < g_wsc; i++) if (g_ws[i].uid == uid) { g_ws[i].conn = 1; return g_ws[i].wid; }
    if (g_wsc >= MAX_WS) return 0;
    w = &g_ws[g_wsc++]; w->wid = g_wsc; w->uid = uid; w->conn = 1; w->pmsg = 0;
    return w->wid;
}

static unsigned int ws_disconnect(unsigned int wid) {
    unsigned int i;
    for (i = 0; i < g_wsc; i++) if (g_ws[i].wid == wid) { g_ws[i].conn = 0; return E_NONE; }
    return E_NOTFOUND;
}

static unsigned int ws_broadcast(unsigned int gid, const char *msg) {
    unsigned int i, j, d = 0;
    for (i = 0; i < g_grpc; i++) {
        if (g_grp[i].gid != gid) continue;
        for (j = 0; j < g_grp[i].ucnt; j++) {
            unsigned int uid = g_grp[i].uids[j], k;
            for (k = 0; k < g_wsc; k++)
                if (g_ws[k].uid == uid && g_ws[k].conn) { g_ws[k].pmsg++; d++; }
        }
    }
    (void)msg; return d;
}

static unsigned int track_del(unsigned int nid, unsigned int st) {
    notif_t *n = ntf_byid(nid);
    if (!n) return E_NOTFOUND;
    if (st == S_DELIVERED) { n->status = S_DELIVERED; n->t_del = 3; stat_rec(n->chan, S_DELIVERED); }
    else if (st == S_FAILED) { n->status = S_FAILED; n->ec = E_GW; stat_rec(n->chan, S_FAILED); }
    return E_NONE;
}

static unsigned int track_read(unsigned int nid) {
    notif_t *n = ntf_byid(nid);
    if (!n) return E_NOTFOUND;
    n->status = S_READ; n->t_read = 4; stat_rec(n->chan, S_READ);
    return E_NONE;
}

static unsigned int analyze_fail(unsigned int *codes, unsigned int max) {
    unsigned int i, idx = 0;
    if (!codes || !max) return 0;
    for (i = 0; i < g_ntfc && idx < max; i++)
        if (g_ntf[i].status == S_FAILED) codes[idx++] = g_ntf[i].ec;
    return idx;
}

static unsigned int stat_rec(unsigned int ch, unsigned int st) {
    unsigned int hi;
    if (ch < MAX_CH) g_st.bych[ch]++;
    if (st < 6) g_st.byst[st]++;
    if (st == S_SENT) g_st.ts++;
    else if (st == S_DELIVERED) g_st.td++;
    else if (st == S_FAILED) g_st.tf++;
    else if (st == S_READ) g_st.tr++;
    hi = g_st.ts % MAX_STATS;
    g_st.hr[hi].sent++;
    if (st == S_DELIVERED) g_st.hr[hi].del++;
    else if (st == S_FAILED) g_st.hr[hi].fail++;
    else if (st == S_READ) g_st.hr[hi].rd++;
    return E_NONE;
}

static unsigned int stat_summary(stats_t *out) {
    if (!out) return E_INVIN;
    *out = g_st; return E_NONE;
}

static unsigned int stat_hourly(stbucket_t *out, unsigned int max) {
    unsigned int i, c;
    if (!out) return E_INVIN;
    c = (max > MAX_STATS) ? MAX_STATS : max;
    for (i = 0; i < c; i++) out[i] = g_st.hr[i];
    return c;
}

/* WASM exports */
/* WASM exports */
#define EX(name) __attribute__((export_name(name)))
EX("notifs_svc_init")
unsigned int exp_init(void) { return svc_init(); }
EX("notifs_svc_shutdown")
unsigned int exp_shutdown(void) { return svc_shutdown(); }
EX("notifs_chan_register")
unsigned int exp_chan_reg(unsigned int t, unsigned int no, unsigned int eo) {
    return chan_reg(t, (const char *)(size_t)no, (const char *)(size_t)eo); }
EX("notifs_tpl_create")
unsigned int exp_tpl_cr(unsigned int ko, unsigned int so, unsigned int bo,
                         unsigned int lo, unsigned int ab) {
    return tpl_create((const char *)(size_t)ko, (const char *)(size_t)so,
                      (const char *)(size_t)bo, (const char *)(size_t)lo, ab); }
EX("notifs_create")
unsigned int exp_notif_cr(unsigned int uid, unsigned int tid, unsigned int ch,
                           unsigned int pri, unsigned int strat) {
    return notif_create(uid, tid, ch, pri, strat); }
EX("notifs_send")
unsigned int exp_send(unsigned int nid) { return notif_send(nid); }
EX("notifs_retry")
unsigned int exp_retry(unsigned int nid) { return notif_retry(nid); }
EX("notifs_user_set_pref")
unsigned int exp_upref(unsigned int uid, unsigned int cm, unsigned int lo,
                        unsigned int ds, unsigned int de, unsigned int gm) {
    return user_set_pref(uid, cm, (const char *)(size_t)lo, ds, de, gm); }
EX("notifs_group_create")
unsigned int exp_grp_cr(unsigned int no) { return grp_create((const char *)(size_t)no); }
EX("notifs_group_add_user")
unsigned int exp_grp_add(unsigned int gid, unsigned int uid) { return grp_add(gid, uid); }
EX("notifs_group_send")
unsigned int exp_grp_send(unsigned int gid, unsigned int tid, unsigned int ch, unsigned int pri) {
    return grp_send(gid, tid, ch, pri); }
EX("notifs_queue_process")
unsigned int exp_qp(void) { return q_process(&g_pq); }
EX("notifs_ws_connect")
unsigned int exp_wsc(unsigned int uid) { return ws_connect(uid); }
EX("notifs_ws_disconnect")
unsigned int exp_wsd(unsigned int wid) { return ws_disconnect(wid); }
EX("notifs_track_delivery")
unsigned int exp_tdel(unsigned int nid, unsigned int st) { return track_del(nid, st); }
EX("notifs_track_read")
unsigned int exp_trd(unsigned int nid) { return track_read(nid); }
EX("notifs_stats_summary")
unsigned int exp_sts(unsigned int oo) { return stat_summary((stats_t *)(size_t)oo); }
EX("notifs_stats_hourly")
unsigned int exp_sth(unsigned int oo, unsigned int mx) {
    return stat_hourly((stbucket_t *)(size_t)oo, mx); }
EX("notifs_analyze_failures")
unsigned int exp_afail(unsigned int oo, unsigned int mx) {
    return analyze_fail((unsigned int *)(size_t)oo, mx); }

int main(void) { svc_init(); host_print("[notification_service] module loaded\n"); return 0; }
