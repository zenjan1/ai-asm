/* Mesh Network Protocol System - AI-ASM OS (AArch64 WASM-native)
 * Subsystems: AODV, DSR, OLSR, BATMAN, Hybrid Routing, Self-Organizing,
 *             Multi-hop, Wireless Mesh 802.11s, IoT Mesh, Applications
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

static unsigned int mesh_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void mesh_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i; for (i = 0; i < n && s[i]; i++) d[i] = s[i]; for (; i < n; i++) d[i] = '\0';
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16], out[16]; int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[i++] = '0';
    while (v > 0 && i < 14) { buf[i++] = '0' + (v % 10); v /= 10; }
    if (neg) buf[i++] = '-';
    for (int j = 0; j < i; j++) out[j] = buf[i - 1 - j];
    out[i] = '\0'; host_print(out);
}
static void print_hex(unsigned int v) {
    char buf[10], out[10]; int i = 0;
    if (v == 0) buf[i++] = '0';
    while (v > 0 && i < 8) {
        unsigned int r = v & 0xFu; buf[i++] = (r < 10) ? ('0' + r) : ('A' + r - 10); v >>= 4;
    }
    for (int j = 0; j < i; j++) out[j] = buf[i - 1 - j]; out[i] = '\0'; host_print(out);
}
static unsigned int hash_bytes(const unsigned char *d, unsigned int l) {
    unsigned int h = 0x811C9DC5u; for (unsigned int i = 0; i < l; i++) { h ^= d[i]; h *= 0x01000193u; } return h;
}
static unsigned int crc32_compute(const unsigned char *d, unsigned int l) {
    unsigned int c = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < l; i++) { c ^= d[i]; for (int k = 0; k < 8; k++) c = (c >> 1) ^ (0xEDB88320u & (-(c & 1))); }
    return c ^ 0xFFFFFFFFu;
}
static void *mem_alloc(unsigned int sz) {
    unsigned int o = host_alloc(sz, 8); return (void *)(o == 0 ? (unsigned long)0 : (unsigned long)o);
}

/* ===== 1. AODV: Ad-hoc On-demand Distance Vector ===== */
typedef enum { AODV_RREQ = 0x01, AODV_RREP = 0x02, AODV_RERR = 0x03 } aodv_type_t;
typedef struct { unsigned int msg_type; unsigned int src_id; unsigned int dst_id;
    unsigned int hop_count; unsigned int seq_no; unsigned int lifetime; } aodv_ctrl_t;
typedef struct { unsigned int dst_id; unsigned int next_hop; unsigned int hop_count;
    unsigned int seq_no; unsigned int lifetime; int active; } aodv_route_t;
typedef struct { aodv_route_t *routes; unsigned int count; unsigned int max_routes; } aodv_table_t;
static aodv_table_t g_aodv;
static void aodv_init(unsigned int max_r) {
    g_aodv.routes = (aodv_route_t *)mem_alloc(sizeof(aodv_route_t) * max_r);
    g_aodv.count = 0; g_aodv.max_routes = max_r;
    print_str("[AODV] reactive routing init max="); print_int(max_r); print_str("\n");
}
static void aodv_send_rreq(unsigned int src, unsigned int dst) {
    print_str("[AODV] RREQ src=0x"); print_hex(src); print_str(" dst=0x"); print_hex(dst); print_str("\n");
}
static int aodv_process_rrep(const aodv_ctrl_t *m) {
    for (unsigned int i = 0; i < g_aodv.count; i++) {
        if (g_aodv.routes[i].dst_id == m->dst_id && g_aodv.routes[i].active) {
            if (m->hop_count < g_aodv.routes[i].hop_count) {
                g_aodv.routes[i].next_hop = m->src_id; g_aodv.routes[i].hop_count = m->hop_count;
                g_aodv.routes[i].seq_no = m->seq_no;
                print_str("[AODV] improved route hops="); print_int(m->hop_count); print_str("\n");
            } return 0;
        }
    }
    if (g_aodv.count >= g_aodv.max_routes) return -1;
    aodv_route_t *r = &g_aodv.routes[g_aodv.count++];
    r->dst_id = m->dst_id; r->next_hop = m->src_id; r->hop_count = m->hop_count;
    r->seq_no = m->seq_no; r->lifetime = m->lifetime; r->active = 1;
    print_str("[AODV] new route dst=0x"); print_hex(m->dst_id); print_str("\n"); return 0;
}
static void aodv_send_rerr(unsigned int unreach) {
    print_str("[AODV] RERR unreachable=0x"); print_hex(unreach); print_str("\n");
    for (unsigned int i = 0; i < g_aodv.count; i++)
        if (g_aodv.routes[i].next_hop == unreach) g_aodv.routes[i].active = 0;
}
static void aodv_maintain(unsigned int now) {
    unsigned int exp = 0;
    for (unsigned int i = 0; i < g_aodv.count; i++)
        if (g_aodv.routes[i].active && now > g_aodv.routes[i].lifetime) { g_aodv.routes[i].active = 0; exp++; }
    print_str("[AODV] maintenance expired="); print_int(exp); print_str("\n");
}

/* ===== 2. DSR: Dynamic Source Routing ===== */
#define DSR_MAX_PATH 16
typedef struct { unsigned int src_id; unsigned int dst_id; unsigned int path[DSR_MAX_PATH];
    unsigned int path_len; unsigned int seq_no; unsigned int lifetime; } dsr_route_t;
typedef struct { dsr_route_t *cache; unsigned int count; unsigned int max_cache; } dsr_cache_t;
static dsr_cache_t g_dsr;
static void dsr_init(unsigned int max_c) {
    g_dsr.cache = (dsr_route_t *)mem_alloc(sizeof(dsr_route_t) * max_c);
    g_dsr.count = 0; g_dsr.max_cache = max_c;
    print_str("[DSR] source routing cache init max="); print_int(max_c); print_str("\n");
}
static void dsr_add_route(unsigned int src, unsigned int dst, const unsigned int *p, unsigned int pl) {
    if (g_dsr.count >= g_dsr.max_cache) {
        for (unsigned int j = 1; j < g_dsr.count; j++) g_dsr.cache[j - 1] = g_dsr.cache[j]; g_dsr.count--;
    }
    dsr_route_t *r = &g_dsr.cache[g_dsr.count++];
    r->src_id = src; r->dst_id = dst; r->path_len = pl < DSR_MAX_PATH ? pl : DSR_MAX_PATH;
    for (unsigned int i = 0; i < r->path_len; i++) r->path[i] = p[i]; r->seq_no++; r->lifetime = 30000;
    print_str("[DSR] cached route len="); print_int(r->path_len); print_str("\n");
}
static const dsr_route_t *dsr_lookup(unsigned int dst) {
    for (unsigned int i = 0; i < g_dsr.count; i++) if (g_dsr.cache[i].dst_id == dst) return &g_dsr.cache[i];
    return (const dsr_route_t *)0;
}
static void dsr_discover(unsigned int src, unsigned int dst) {
    print_str("[DSR] discover src=0x"); print_hex(src); print_str(" dst=0x"); print_hex(dst); print_str("\n");
}

/* ===== 3. OLSR: Optimized Link State Routing ===== */
#define OLSR_MAX_NB 32
typedef struct { unsigned int node_id; int is_mpr; unsigned int will;
    unsigned int lq; unsigned int nq; unsigned int last_hello; } olsr_nb_t;
typedef struct { olsr_nb_t *nbs; unsigned int count; unsigned int *mprs; unsigned int mpr_cnt; unsigned int max_nb; } olsr_t;
static olsr_t g_olsr;
static void olsr_init(unsigned int mx) {
    g_olsr.nbs = (olsr_nb_t *)mem_alloc(sizeof(olsr_nb_t) * mx);
    g_olsr.mprs = (unsigned int *)mem_alloc(sizeof(unsigned int) * mx);
    g_olsr.count = 0; g_olsr.mpr_cnt = 0; g_olsr.max_nb = mx;
    print_str("[OLSR] proactive LS routing init\n");
}
static void olsr_hello(unsigned int sender, unsigned int *nb, unsigned int nc, unsigned int will) {
    olsr_nb_t *n = (olsr_nb_t *)0;
    for (unsigned int i = 0; i < g_olsr.count; i++) if (g_olsr.nbs[i].node_id == sender) { n = &g_olsr.nbs[i]; break; }
    if (!n) { if (g_olsr.count >= g_olsr.max_nb) return; n = &g_olsr.nbs[g_olsr.count++]; n->node_id = sender; n->is_mpr = 0; }
    n->will = will; n->last_hello++;
    print_str("[OLSR] HELLO from 0x"); print_hex(sender); print_str(" nbs="); print_int(nc); print_str("\n");
}
static void olsr_compute_mpr(void) {
    g_olsr.mpr_cnt = 0;
    for (unsigned int i = 0; i < g_olsr.count; i++)
        if (g_olsr.nbs[i].will > 0 && g_olsr.nbs[i].nq > 500) {
            g_olsr.nbs[i].is_mpr = 1; g_olsr.mprs[g_olsr.mpr_cnt++] = g_olsr.nbs[i].node_id;
        }
    print_str("[OLSR] MPR set size="); print_int(g_olsr.mpr_cnt); print_str("\n");
}
static void olsr_send_tc(void) { print_str("[OLSR] TC via "); print_int(g_olsr.mpr_cnt); print_str(" MPRs\n"); }

/* ===== 4. BATMAN: B.A.T.M.A.N. Advanced ===== */
typedef struct { unsigned int orig; unsigned int nh; unsigned int tq;
    unsigned int last_seen; unsigned int seq; int gw; } bat_orig_t;
typedef struct { bat_orig_t *orig; unsigned int count; unsigned int max_o; unsigned int gw_sel; } bat_t;
static bat_t g_bat;
static void bat_init(unsigned int mx) {
    g_bat.orig = (bat_orig_t *)mem_alloc(sizeof(bat_orig_t) * mx);
    g_bat.count = 0; g_bat.max_o = mx; g_bat.gw_sel = 0;
    print_str("[BATMAN] B.A.T.M.A.N. advanced init max="); print_int(mx); print_str("\n");
}
static void bat_process_ogm(unsigned int orig, unsigned int nh, unsigned int tq, unsigned int seq) {
    bat_orig_t *o = (bat_orig_t *)0;
    for (unsigned int i = 0; i < g_bat.count; i++) if (g_bat.orig[i].orig == orig) { o = &g_bat.orig[i]; break; }
    if (!o) { if (g_bat.count >= g_bat.max_o) return; o = &g_bat.orig[g_bat.count++]; o->orig = orig; o->gw = 0; }
    if (tq > o->tq || seq > o->seq) { o->nh = nh; o->tq = tq; o->seq = seq; o->last_seen++; }
    print_str("[BATMAN] OGM 0x"); print_hex(orig); print_str(" tq="); print_int(tq); print_str("\n");
}
static int bat_select_gw(void) {
    int best = -1; unsigned int bq = 0;
    for (unsigned int i = 0; i < g_bat.count; i++)
        if (g_bat.orig[i].gw && g_bat.orig[i].tq > bq) { bq = g_bat.orig[i].tq; best = (int)i; }
    if (best >= 0) { g_bat.gw_sel = g_bat.orig[best].orig; print_str("[BATMAN] gw=0x"); print_hex(g_bat.gw_sel); print_str("\n"); }
    else print_str("[BATMAN] no gateway\n");
    return best;
}

/* ===== 5. Hybrid Routing ===== */
typedef enum { HYB_PROACTIVE = 0, HYB_REACTIVE = 1 } hyb_mode_t;
typedef struct { unsigned int dst; unsigned int nh; unsigned int met; hyb_mode_t mode; unsigned int exp; } hyb_ent_t;
typedef struct { hyb_ent_t *tbl; unsigned int count; unsigned int max_e; } hyb_t;
static hyb_t g_hyb;
static void hyb_init(unsigned int mx) {
    g_hyb.tbl = (hyb_ent_t *)mem_alloc(sizeof(hyb_ent_t) * mx); g_hyb.count = 0; g_hyb.max_e = mx;
    print_str("[Hybrid] hybrid routing init\n");
}
static void hyb_insert(unsigned int dst, unsigned int nh, unsigned int met, hyb_mode_t mode) {
    if (g_hyb.count >= g_hyb.max_e) return;
    hyb_ent_t *e = &g_hyb.tbl[g_hyb.count++];
    e->dst = dst; e->nh = nh; e->met = met; e->mode = mode; e->exp = 60000;
    print_str("[Hybrid] insert "); print_str(mode == HYB_PROACTIVE ? "proactive" : "reactive");
    print_str(" met="); print_int(met); print_str("\n");
}
static void hyb_refresh(void) {
    unsigned int c = 0;
    for (unsigned int i = 0; i < g_hyb.count; i++) if (g_hyb.tbl[i].mode == HYB_PROACTIVE) { g_hyb.tbl[i].exp = 60000; c++; }
    print_str("[Hybrid] refreshed proactive="); print_int(c); print_str("\n");
}

/* ===== 6. Self-Organizing Network ===== */
typedef struct { unsigned int id; unsigned int caps; int rssi;
    unsigned int energy; unsigned int hello; int status; } sonode_t;
typedef struct { sonode_t *nodes; unsigned int count; unsigned int max_n; unsigned int ch; int cfg; } sonet_t;
static sonet_t g_so;
static void so_init(unsigned int mx) {
    g_so.nodes = (sonode_t *)mem_alloc(sizeof(sonode_t) * mx);
    g_so.count = 0; g_so.max_n = mx; g_so.ch = 0; g_so.cfg = 1;
    print_str("[SelfOrg] zero-config ad-hoc init\n");
}
static int so_add(unsigned int id, unsigned int caps, int rssi) {
    for (unsigned int i = 0; i < g_so.count; i++)
        if (g_so.nodes[i].id == id) { g_so.nodes[i].rssi = rssi; g_so.nodes[i].hello++; return 0; }
    if (g_so.count >= g_so.max_n) return -1;
    sonode_t *n = &g_so.nodes[g_so.count++];
    n->id = id; n->caps = caps; n->rssi = rssi; n->energy = 1000; n->hello = 1; n->status = 1;
    print_str("[SelfOrg] node 0x"); print_hex(id); print_str(" rssi="); print_int(rssi); print_str("\n");
    return 0;
}
static void so_select_ch(void) {
    int best = -1; unsigned int be = 0;
    for (unsigned int i = 0; i < g_so.count; i++)
        if (g_so.nodes[i].energy > be) { be = g_so.nodes[i].energy; best = (int)i; }
    if (best >= 0) { g_so.ch = g_so.nodes[best].id; print_str("[SelfOrg] cluster head 0x"); print_hex(g_so.ch); print_str("\n"); }
}
static void so_heal(void) {
    unsigned int h = 0;
    for (unsigned int i = 0; i < g_so.count; i++)
        if (g_so.nodes[i].status == 0 && g_so.nodes[i].energy > 100) { g_so.nodes[i].status = 1; h++; }
    print_str("[SelfOrg] healed="); print_int(h); print_str("\n");
}
static void so_expire(unsigned int tmo) {
    unsigned int kept = 0;
    for (unsigned int i = 0; i < g_so.count; i++) {
        if (g_so.nodes[i].hello < tmo) g_so.nodes[i].status = 0;
        g_so.nodes[kept++] = g_so.nodes[i];
    }
    g_so.count = kept; print_str("[SelfOrg] active="); print_int(kept); print_str("\n");
}
static void so_hierarchical(void) {
    print_str("[SelfOrg] hierarchical clusters="); print_int((g_so.count + 7) / 8);
    print_str(" nodes="); print_int(g_so.count); print_str("\n");
}

/* ===== 7. Multi-hop Communication ===== */
typedef struct { unsigned int la; unsigned int lb; unsigned int etx; int rssi;
    unsigned int snr; unsigned int tx; unsigned int rx; unsigned int load; unsigned int cap; } link_t;
typedef struct { link_t *links; unsigned int count; unsigned int max_l; } link_tab_t;
static link_tab_t g_lk;
static void lk_init(unsigned int mx) {
    g_lk.links = (link_t *)mem_alloc(sizeof(link_t) * mx); g_lk.count = 0; g_lk.max_l = mx;
    print_str("[MultiHop] link state init\n");
}
static unsigned int comp_etx(unsigned int tx, unsigned int rx, unsigned int psz) {
    if (tx == 0 || rx == 0) return 0xFFFFFFFFu; return (psz * 1000) / ((rx * 1000 / tx) + 1);
}
static void lk_update(unsigned int a, unsigned int b, int rssi, unsigned int snr, unsigned int tx, unsigned int rx) {
    link_t *l = (link_t *)0;
    for (unsigned int i = 0; i < g_lk.count; i++)
        if (g_lk.links[i].la == a && g_lk.links[i].lb == b) { l = &g_lk.links[i]; break; }
    if (!l) { if (g_lk.count >= g_lk.max_l) return; l = &g_lk.links[g_lk.count++]; l->la = a; l->lb = b; l->load = 0; l->cap = 1000; }
    l->rssi = rssi; l->snr = snr; l->tx = tx; l->rx = rx; l->etx = comp_etx(tx, rx, 1024);
    print_str("[MultiHop] link etx=0x"); print_hex(l->etx); print_str(" rssi="); print_int(rssi); print_str("\n");
}
static int lk_best_path(unsigned int dst) {
    int best = -1; unsigned int be = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < g_lk.count; i++)
        if (g_lk.links[i].lb == dst && g_lk.links[i].etx < be) { be = g_lk.links[i].etx; best = (int)i; }
    return best;
}
static void lk_balance(void) {
    unsigned int ov = 0;
    for (unsigned int i = 0; i < g_lk.count; i++) if (g_lk.links[i].load > g_lk.links[i].cap * 80 / 100) ov++;
    print_str("[MultiHop] balance overloaded="); print_int(ov); print_str("\n");
}
static void lk_congest(void) {
    unsigned int th = 0;
    for (unsigned int i = 0; i < g_lk.count; i++)
        if (g_lk.links[i].load > g_lk.links[i].cap * 90 / 100) { g_lk.links[i].load = g_lk.links[i].cap * 70 / 100; th++; }
    print_str("[MultiHop] congestion throttled="); print_int(th); print_str("\n");
}
static void lk_forward(unsigned int src, unsigned int dst, unsigned int hops) {
    print_str("[MultiHop] forward src=0x"); print_hex(src); print_str(" dst=0x"); print_hex(dst);
    print_str(" hops="); print_int(hops); print_str("\n");
}

/* ===== 8. Wireless Mesh (802.11s) ===== */
typedef enum { MP = 0x01, MG = 0x02, MS = 0x04 } mrole_t;
typedef struct { unsigned int sta; mrole_t role; unsigned int chan; unsigned int bssid; int auth; int enc; } msta_t;
typedef struct { msta_t *stas; unsigned int count; unsigned int max_s; unsigned int mid; unsigned int gw; } mwifi_t;
static mwifi_t g_wf;
static void wf_init(unsigned int mid, unsigned int mx) {
    g_wf.stas = (msta_t *)mem_alloc(sizeof(msta_t) * mx);
    g_wf.count = 0; g_wf.max_s = mx; g_wf.mid = mid; g_wf.gw = 0;
    print_str("[802.11s] mesh init mid=0x"); print_hex(mid); print_str("\n");
}
static int wf_add_sta(unsigned int id, mrole_t r, unsigned int c) {
    if (g_wf.count >= g_wf.max_s) return -1;
    msta_t *s = &g_wf.stas[g_wf.count++];
    s->sta = id; s->role = r; s->chan = c; s->bssid = g_wf.mid; s->auth = 1; s->enc = 1;
    print_str("[802.11s] sta 0x"); print_hex(id); print_str(" role="); print_int((int)r);
    print_str(" chan="); print_int(c); print_str("\n"); return 0;
}
static void wf_set_gw(unsigned int gw) { g_wf.gw = gw; print_str("[802.11s] backhaul gw=0x"); print_hex(gw); print_str("\n"); }
static void wf_channels(unsigned int nrad) {
    unsigned int chans[] = {1, 6, 11, 36, 44, 52, 60};
    print_str("[802.11s] channels radios="); print_int(nrad); print_str("\n");
    for (unsigned int i = 0; i < nrad && i < 7; i++) { print_str("[802.11s] radio "); print_int(i); print_str(" -> "); print_int(chans[i]); print_str("\n"); }
}
static void wf_security(int a, int e) {
    print_str("[802.11s] security auth="); print_str(a == 1 ? "SAE" : "OPEN");
    print_str(" enc="); print_str(e == 1 ? "AES-CCMP" : "NONE"); print_str("\n");
}

/* ===== 9. IoT Mesh (6LoWPAN, Thread, Zigbee) ===== */
typedef struct { unsigned int id; unsigned int stype; unsigned int batt;
    int temp; unsigned int dcyc; int slp; unsigned int lrep; } iotn_t;
typedef struct { iotn_t *nodes; unsigned int count; unsigned int max_n; unsigned int br; unsigned int pan; } iotm_t;
static iotm_t g_iot;
static void iot_init(unsigned int pan, unsigned int mx) {
    g_iot.nodes = (iotn_t *)mem_alloc(sizeof(iotn_t) * mx);
    g_iot.count = 0; g_iot.max_n = mx; g_iot.pan = pan; g_iot.br = 0;
    print_str("[IoT] 6LoWPAN init pan=0x"); print_hex(pan); print_str("\n");
}
static int iot_add(unsigned int id, unsigned int st, unsigned int bt) {
    if (g_iot.count >= g_iot.max_n) return -1;
    iotn_t *n = &g_iot.nodes[g_iot.count++];
    n->id = id; n->stype = st; n->batt = bt; n->temp = 22; n->dcyc = 10; n->slp = 1; n->lrep = 0;
    print_str("[IoT] node 0x"); print_hex(id); print_str(" batt="); print_int(bt); print_str("mV\n"); return 0;
}
static void iot_set_br(unsigned int br) { g_iot.br = br; print_str("[IoT] border router 0x"); print_hex(br); print_str("\n"); }
static void iot_commission(unsigned int comm, unsigned int join) {
    print_str("[IoT] Thread commission comm=0x"); print_hex(comm); print_str(" join=0x"); print_hex(join); print_str("\n");
}
static void iot_compress(unsigned int src, unsigned int dst) {
    print_str("[IoT] 6LoWPAN compress src=0x"); print_hex(src); print_str(" dst=0x"); print_hex(dst); print_str(" ratio=40:7\n");
}
static void iot_aggregate(void) {
    unsigned int tot = 0;
    for (unsigned int i = 0; i < g_iot.count; i++) tot += (unsigned int)(g_iot.nodes[i].temp + 50);
    unsigned int avg = g_iot.count > 0 ? tot / g_iot.count : 0;
    print_str("[IoT] aggregate temp="); print_int(avg - 50); print_str("C nodes="); print_int(g_iot.count); print_str("\n");
}
static void iot_zigbee(unsigned int prof) { print_str("[IoT] Zigbee profile=0x"); print_hex(prof); print_str("\n"); }
static void iot_zwave(unsigned int hid) { print_str("[IoT] Z-Wave home=0x"); print_hex(hid); print_str("\n"); }
static void iot_sleep(void) {
    unsigned int s = 0;
    for (unsigned int i = 0; i < g_iot.count; i++) if (g_iot.nodes[i].slp) s++;
    print_str("[IoT] sleep="); print_int(s); print_str(" active="); print_int(g_iot.count - s); print_str("\n");
}

/* ===== 10. Applications ===== */
typedef enum { APP_EMERG = 0x01, APP_CITY = 0x02, APP_IND = 0x04, APP_VANET = 0x08, APP_UAV = 0x10 } app_t;
typedef struct { app_t type; unsigned int parts; unsigned int rate; unsigned int cov; int active; } mapp_t;
typedef struct { mapp_t *apps; unsigned int count; unsigned int max_a; } appreg_t;
static appreg_t g_ap;
static void ap_init(unsigned int mx) {
    g_ap.apps = (mapp_t *)mem_alloc(sizeof(mapp_t) * mx); g_ap.count = 0; g_ap.max_a = mx;
    print_str("[Apps] registry init\n");
}
static int ap_reg(app_t t, unsigned int p) {
    if (g_ap.count >= g_ap.max_a) return -1;
    mapp_t *a = &g_ap.apps[g_ap.count++]; a->type = t; a->parts = p; a->rate = 100; a->cov = 1000; a->active = 1;
    print_str("[Apps] register type=0x"); print_hex((unsigned int)t); print_str(" parts="); print_int(p); print_str("\n"); return 0;
}
static void ap_launch(const char *name, const char *d1, const char *d2, const char *d3) {
    print_str("["); print_str(name); print_str("] "); print_str(d1); print_str("\n");
    print_str("["); print_str(name); print_str("] "); print_str(d2); print_str("\n");
    print_str("["); print_str(name); print_str("] "); print_str(d3); print_str("\n");
}

/* ===== Statistics ===== */
typedef struct { unsigned int nodes; unsigned int routes; unsigned int links;
    unsigned int pkts; unsigned int drops; unsigned int gws; unsigned int up_ms; } mstats_t;
static mstats_t g_st;
static void stats_collect(void) {
    g_st.nodes = g_so.count + g_iot.count; g_st.routes = g_aodv.count + g_dsr.count + g_hyb.count;
    g_st.links = g_lk.count; g_st.gws = g_bat.gw_sel ? 1 : 0; g_st.up_ms = 60000;
    print_str("[Stats] nodes="); print_int(g_st.nodes); print_str(" routes="); print_int(g_st.routes);
    print_str(" links="); print_int(g_st.links); print_str(" gws="); print_int(g_st.gws);
    print_str(" up="); print_int(g_st.up_ms); print_str("ms\n");
}

/* ===== Main ===== */
int main(void) {
    char argbuf[64];
    int argc = host_get_argv(0, sizeof(argbuf));
    print_str("=== mesh_network starting ===\n");
    if (argc > 0) { host_get_argv((unsigned int)(unsigned long)argbuf, sizeof(argbuf)); print_str("argv: "); print_str(argbuf); print_str("\n"); }

    print_str("\n--- Routing Init ---\n");
    aodv_init(64); dsr_init(32); olsr_init(OLSR_MAX_NB); bat_init(64); hyb_init(48);

    print_str("\n--- Self-Organizing ---\n");
    so_init(128); so_add(0xA001, 0x0F, -45); so_add(0xA002, 0x07, -62);
    so_add(0xA003, 0x03, -78); so_add(0xA004, 0x0F, -55); so_add(0xA005, 0x01, -88);
    so_select_ch(); so_hierarchical();

    print_str("\n--- Multi-hop Links ---\n");
    lk_init(64); lk_update(0xA001, 0xA002, -45, 300, 1000, 950);
    lk_update(0xA002, 0xA003, -62, 220, 1000, 870); lk_update(0xA003, 0xA004, -78, 150, 1000, 720);
    lk_update(0xA001, 0xA004, -55, 280, 1000, 900);
    print_str("[Path] best="); print_int(lk_best_path(0xA004)); print_str("\n");
    lk_balance(); lk_congest();

    print_str("\n--- AODV Discovery ---\n");
    aodv_send_rreq(0xA001, 0xA005);
    aodv_ctrl_t r1 = { AODV_RREP, 0xA003, 0xA005, 2, 10, 30000 }; aodv_process_rrep(&r1);
    aodv_ctrl_t r2 = { AODV_RREP, 0xA004, 0xA005, 1, 11, 30000 }; aodv_process_rrep(&r2);
    aodv_maintain(1000);

    print_str("\n--- DSR Source Routing ---\n");
    dsr_discover(0xA001, 0xA005);
    unsigned int p1[] = {0xA001, 0xA003, 0xA005}; dsr_add_route(0xA001, 0xA005, p1, 3);
    const dsr_route_t *fd = dsr_lookup(0xA005);
    if (fd) { print_str("[DSR] route len="); print_int(fd->path_len); print_str("\n"); }

    print_str("\n--- OLSR Topology ---\n");
    unsigned int n1[] = {0xA002, 0xA003}; olsr_hello(0xA001, n1, 2, 3);
    unsigned int n2[] = {0xA001, 0xA004}; olsr_hello(0xA002, n2, 2, 2);
    g_olsr.nbs[0].nq = 800; g_olsr.nbs[1].nq = 600;
    olsr_compute_mpr(); olsr_send_tc();

    print_str("\n--- BATMAN Advanced ---\n");
    bat_process_ogm(0xA001, 0xA001, 200, 1); bat_process_ogm(0xA002, 0xA002, 180, 1);
    bat_process_ogm(0xA003, 0xA002, 150, 1); g_bat.orig[0].gw = 1; bat_select_gw();

    print_str("\n--- Hybrid Routing ---\n");
    hyb_insert(0xA005, 0xA003, 2, HYB_PROACTIVE); hyb_insert(0xA006, 0xA004, 3, HYB_REACTIVE);
    hyb_refresh();

    print_str("\n--- Wireless Mesh 802.11s ---\n");
    wf_init(0xABCD, 32); wf_add_sta(0xB001, MP, 36); wf_add_sta(0xB002, MP, 44);
    wf_add_sta(0xB003, MG, 36); wf_set_gw(0xB003); wf_channels(3); wf_security(1, 1);

    print_str("\n--- IoT Mesh ---\n");
    iot_init(0x1234, 64); iot_add(0xC001, 0x0001, 3300); iot_add(0xC002, 0x0002, 2900);
    iot_add(0xC003, 0x0003, 3100); iot_set_br(0xC001);
    iot_commission(0xC001, 0xC002); iot_compress(0xC001, 0xC003);
    iot_aggregate(); iot_zigbee(0x0104); iot_zwave(0xDEAD0001u); iot_sleep();

    print_str("\n--- Applications ---\n");
    ap_init(16); ap_reg(APP_EMERG, 50); ap_reg(APP_CITY, 200); ap_reg(APP_IND, 100);
    ap_reg(APP_VANET, 30); ap_reg(APP_UAV, 12);
    ap_launch("Emergency", "disaster recovery active", "first responder ad-hoc online", "store-and-forward fallback");
    ap_launch("SmartCity", "traffic mesh online", "sensor grid active", "aggregation=60s");
    ap_launch("Industrial", "factory automation online", "predictive maint active", "latency=10ms");
    ap_launch("VANET", "V2X mesh active", "vehicular ad-hoc online", "DSRC/802.11p enabled");
    ap_launch("UAV", "drone swarm active", "formation control online", "rendezvous enabled");

    print_str("\n--- Maintenance ---\n");
    so_heal(); so_expire(500); lk_forward(0xA001, 0xA005, 3);

    print_str("\n=== Statistics ===\n");
    stats_collect();
    print_str("[Stats] aodv="); print_int(g_aodv.count); print_str(" dsr="); print_int(g_dsr.count);
    print_str(" olsr="); print_int(g_olsr.count); print_str(" batman="); print_int(g_bat.count);
    print_str(" selforg="); print_int(g_so.count); print_str(" iot="); print_int(g_iot.count);
    print_str(" wifi="); print_int(g_wf.count); print_str(" apps="); print_int(g_ap.count);
    print_str("\n");
    unsigned int crc = crc32_compute((const unsigned char *)"mesh_network", 12);
    print_str("[Verify] crc32=0x"); print_hex(crc); print_str("\n");
    print_str("=== mesh_network complete ===\n");
    host_exit(0); return 0;
}
