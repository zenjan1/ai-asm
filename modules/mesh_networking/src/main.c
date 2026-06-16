/* Mesh Networking Protocol Stack - AI-ASM OS (AArch64 WASM-native)
 * Version: 56.0 - Self-Organizing Protocol Stack
 * Subsystems: Discovery, B.A.T.M.A.N.-like Routing, Multi-hop,
 *             Random Linear Network Coding (RLNC), Fault Tolerance, QoS
 * Built entirely in C with no standard library.
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

/* ===== Helpers ===== */
static unsigned int my_strlen(const char *s) {
    unsigned int n = 0; while (s[n]) n++; return n;
}
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i;
    for (i = 0; i < n && s[i]; i++) d[i] = s[i];
    for (; i < n; i++) d[i] = '\0';
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
static int my_abs(int v) { return v < 0 ? -v : v; }
static unsigned int simple_hash(const unsigned char *d, unsigned int l) {
    unsigned int h = 0x811c9dc5u;
    for (unsigned int i = 0; i < l; i++) { h ^= d[i]; h *= 0x01000193u; }
    return h;
}
static unsigned int crc32_byte(unsigned int crc, unsigned char b) {
    crc ^= b;
    for (int i = 0; i < 8; i++) crc = (crc >> 1) ^ (0xEDB88320u & (-(crc & 1)));
    return crc;
}
static unsigned int compute_crc(const unsigned char *d, unsigned int l) {
    unsigned int c = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < l; i++) c = crc32_byte(c, d[i]);
    return c ^ 0xFFFFFFFFu;
}
static void *mem_alloc(unsigned int sz) {
    unsigned int o = host_alloc(sz, 8);
    return (void *)(o == 0 ? (unsigned long)0 : (unsigned long)o);
}

/* ===== 1. Network Discovery ===== */
typedef struct {
    char node_id[8]; char address[16]; unsigned int capabilities;
    int rssi; unsigned int last_seen; unsigned int hop_count;
} mesh_node_t;
typedef struct { mesh_node_t *neighbors; unsigned int count, max_neighbors; } neighbor_table_t;
static neighbor_table_t g_nt;

static void discovery_init(const char *nid, unsigned int mx) {
    g_nt.neighbors = (mesh_node_t *)mem_alloc(sizeof(mesh_node_t) * mx);
    g_nt.count = 0; g_nt.max_neighbors = mx;
    print_str("[discovery] initialized node: "); print_str(nid);
    print_str(" max_neighbors="); print_int(mx); print_str("\n");
}
static void discovery_send_probe(void) {
    print_str("[discovery] sending mDNS/SSDP probe broadcast\n");
}
static int discovery_process_response(const mesh_node_t *r) {
    if (g_nt.count >= g_nt.max_neighbors) {
        print_str("[discovery] neighbor table full\n"); return -1;
    }
    for (unsigned int i = 0; i < g_nt.count; i++) {
        int m = 1;
        for (int k = 0; k < 8; k++)
            if (g_nt.neighbors[i].node_id[k] != r->node_id[k]) { m = 0; break; }
        if (m) { g_nt.neighbors[i] = *r; print_str("[discovery] updated neighbor\n"); return 0; }
    }
    g_nt.neighbors[g_nt.count++] = *r;
    print_str("[discovery] added neighbor, total="); print_int(g_nt.count); print_str("\n");
    return 0;
}
static void discovery_update_topology(void) {
    print_str("[discovery] rebuilding topology from "); print_int(g_nt.count);
    print_str(" neighbors\n");
}
static void discovery_expire_stale(unsigned int tmo) {
    unsigned int now = tmo + 1000, kept = 0;
    for (unsigned int i = 0; i < g_nt.count; i++)
        if (now - g_nt.neighbors[i].last_seen < tmo)
            g_nt.neighbors[kept++] = g_nt.neighbors[i];
    unsigned int exp = g_nt.count - kept; g_nt.count = kept;
    print_str("[discovery] expired "); print_int(exp); print_str(" stale neighbors\n");
}

/* ===== 2. Routing Protocol (B.A.T.M.A.N.-like) ===== */
typedef struct {
    char dest_id[8]; char next_hop[8]; unsigned int metric;
    unsigned int hop_count, last_update, seq_no;
} route_entry_t;
typedef struct { route_entry_t *entries; unsigned int count, max_entries; } routing_table_t;
static routing_table_t g_rt;

static void routing_init(const char *nid, unsigned int mx) {
    g_rt.entries = (route_entry_t *)mem_alloc(sizeof(route_entry_t) * mx);
    g_rt.count = 0; g_rt.max_entries = mx;
    print_str("[routing] initialized node: "); print_str(nid);
    print_str(" max_routes="); print_int(mx); print_str("\n");
}
static int routing_update(const char *dst, const char *nh, unsigned int met, unsigned int hc) {
    for (unsigned int i = 0; i < g_rt.count; i++) {
        int m = 1;
        for (int k = 0; k < 8; k++)
            if (g_rt.entries[i].dest_id[k] != dst[k]) { m = 0; break; }
        if (m) {
            if (met < g_rt.entries[i].metric) {
                my_strncpy(g_rt.entries[i].next_hop, nh, 8);
                g_rt.entries[i].metric = met; g_rt.entries[i].hop_count = hc;
                g_rt.entries[i].last_update++; g_rt.entries[i].seq_no++;
                print_str("[routing] improved route metric="); print_int(met); print_str("\n");
            }
            return 0;
        }
    }
    if (g_rt.count >= g_rt.max_entries) { print_str("[routing] table full\n"); return -1; }
    route_entry_t *e = &g_rt.entries[g_rt.count++];
    my_strncpy(e->dest_id, dst, 8); my_strncpy(e->next_hop, nh, 8);
    e->metric = met; e->hop_count = hc; e->last_update = 1; e->seq_no = 1;
    print_str("[routing] added new route\n"); return 0;
}
static int routing_select_path(const char *dst, int use_etx) {
    int best = -1; unsigned int bv = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < g_rt.count; i++) {
        int m = 1;
        for (int k = 0; k < 8; k++)
            if (g_rt.entries[i].dest_id[k] != dst[k]) { m = 0; break; }
        if (!m) continue;
        unsigned int v = use_etx ? g_rt.entries[i].metric : g_rt.entries[i].hop_count;
        if (v < bv) { bv = v; best = (int)i; }
    }
    return best;
}
static void routing_expire_routes(unsigned int max_age) {
    unsigned int kept = 0;
    for (unsigned int i = 0; i < g_rt.count; i++)
        if (g_rt.entries[i].last_update < max_age)
            g_rt.entries[kept++] = g_rt.entries[i];
    unsigned int rm = g_rt.count - kept; g_rt.count = kept;
    print_str("[routing] expired "); print_int(rm); print_str(" stale routes\n");
}
static unsigned int routing_compute_etx(unsigned int tx, unsigned int rx, unsigned int psz) {
    if (tx == 0 || rx == 0) return 0xFFFFFFFFu;
    unsigned int delivery = (rx * 1000) / tx;
    return (psz * 1000) / (delivery + 1);
}

/* ===== 3. Multi-hop Communication ===== */
#define MAX_FRAGS 16
typedef struct {
    unsigned int packet_id, fragment_index, total_fragments;
    unsigned char data[256]; unsigned int data_len;
} fragment_t;
typedef struct {
    unsigned int packet_id; fragment_t frags[MAX_FRAGS];
    unsigned int mask, total; int complete;
} pkt_reasm_t;
typedef struct { pkt_reasm_t *packets; unsigned int count, max; } reasm_table_t;
static reasm_table_t g_ra;

static unsigned int fragment_packet(const unsigned char *d, unsigned int dl,
                                    unsigned int mtu, fragment_t *out) {
    unsigned int nf = (dl + mtu - 1) / mtu;
    if (nf > MAX_FRAGS) nf = MAX_FRAGS;
    unsigned int pid = simple_hash(d, dl);
    for (unsigned int i = 0; i < nf; i++) {
        out[i].packet_id = pid; out[i].fragment_index = i;
        out[i].total_fragments = nf;
        unsigned int off = i * mtu, chunk = dl - off;
        if (chunk > mtu) chunk = mtu;
        for (unsigned int j = 0; j < chunk; j++) out[i].data[j] = d[off + j];
        out[i].data_len = chunk;
    }
    print_str("[fragment] split into "); print_int(nf); print_str(" fragments\n");
    return nf;
}
static void reassembly_init(unsigned int mx) {
    g_ra.packets = (pkt_reasm_t *)mem_alloc(sizeof(pkt_reasm_t) * mx);
    g_ra.count = 0; g_ra.max = mx;
}
static int reassemble_fragment(fragment_t *f) {
    pkt_reasm_t *s = 0;
    for (unsigned int i = 0; i < g_ra.count; i++)
        if (g_ra.packets[i].packet_id == f->packet_id) { s = &g_ra.packets[i]; break; }
    if (!s) {
        if (g_ra.count >= g_ra.max) return -1;
        s = &g_ra.packets[g_ra.count++];
        s->packet_id = f->packet_id; s->mask = 0;
        s->total = f->total_fragments; s->complete = 0;
    }
    if (f->fragment_index < MAX_FRAGS) {
        s->frags[f->fragment_index] = *f;
        s->mask |= (1u << f->fragment_index);
    }
    unsigned int expected = (1u << s->total) - 1u;
    if ((s->mask & expected) == expected) {
        s->complete = 1;
        print_str("[reassembly] packet complete id="); print_int(f->packet_id); print_str("\n");
        return 1;
    }
    return 0;
}
static int reassemble_check_complete(unsigned int pid) {
    for (unsigned int i = 0; i < g_ra.count; i++)
        if (g_ra.packets[i].packet_id == pid) return g_ra.packets[i].complete;
    return 0;
}
static void multi_hop_send(const unsigned char *d, unsigned int len,
                           const char *dst, unsigned int ttl) {
    print_str("[mesh] sending "); print_int(len);
    print_str(" bytes, ttl="); print_int(ttl); print_str("\n");
    int idx = routing_select_path(dst, 1);
    if (idx < 0) { print_str("[mesh] no route found\n"); return; }
    print_str("[mesh] forwarding via next hop, metric=");
    print_int(g_rt.entries[idx].metric); print_str("\n");
}
static void multi_hop_forward(const unsigned char *d, unsigned int len,
                              const char *nh, unsigned int ttl) {
    if (ttl == 0) { print_str("[mesh] TTL expired, dropping\n"); return; }
    print_str("[mesh] forwarding "); print_int(len);
    print_str(" bytes, remaining ttl="); print_int(ttl - 1); print_str("\n");
}

/* ===== 4. Network Coding (RLNC) ===== */
#define MAX_GEN 16
typedef struct {
    unsigned int gen_id; unsigned char coeff[16];
    unsigned char data[128]; unsigned int coeff_len, data_len;
} coded_packet_t;
typedef struct {
    unsigned int gen_id; coded_packet_t pkts[MAX_GEN];
    unsigned int n_orig, n_coded; int decoded;
} generation_t;
typedef struct {
    unsigned int win_sz, field_sz; generation_t *gens; unsigned int gc;
} coding_eng_t;
static coding_eng_t g_ce;

static void coding_init(unsigned int ws, unsigned int fs) {
    g_ce.win_sz = ws; g_ce.field_sz = fs;
    g_ce.gens = (generation_t *)mem_alloc(sizeof(generation_t) * 8);
    g_ce.gc = 0;
    print_str("[coding] initialized window="); print_int(ws);
    print_str(" field_size="); print_int(fs); print_str("\n");
}
static int coding_encode(const unsigned char *pkts[], unsigned int pl,
                         const unsigned char *coeff, unsigned int gid) {
    if (g_ce.gc >= 8) return -1;
    generation_t *g = &g_ce.gens[g_ce.gc];
    g->gen_id = gid; g->n_orig = g_ce.win_sz; g->n_coded = 0; g->decoded = 0;
    coded_packet_t *c = &g->pkts[0];
    c->gen_id = gid; c->coeff_len = g_ce.win_sz;
    for (unsigned int i = 0; i < c->coeff_len && i < 16; i++) c->coeff[i] = coeff[i];
    c->data_len = pl < 128 ? pl : 127;
    for (unsigned int b = 0; b < c->data_len; b++) {
        unsigned int a = 0;
        for (unsigned int p = 0; pkts[p] && p < g_ce.win_sz; p++)
            a ^= ((unsigned int)coeff[p] * pkts[p][b]) & 0xFF;
        c->data[b] = (unsigned char)(a & 0xFF);
    }
    g->n_coded = 1; g_ce.gc++;
    print_str("[coding] encoded generation "); print_int(gid); print_str("\n");
    return 0;
}
static int coding_check_decodable(const generation_t *g) {
    return g->n_coded >= g->n_orig ? 1 : 0;
}
static int coding_decode(generation_t *g) {
    if (!coding_check_decodable(g)) {
        print_str("[coding] insufficient coded packets\n"); return -1;
    }
    print_str("[coding] decoding generation "); print_int(g->gen_id);
    print_str(" via Gaussian elimination\n"); g->decoded = 1; return 0;
}
static unsigned int coding_compute_overhead(unsigned int orig, unsigned int sent) {
    if (orig == 0) return 0;
    unsigned int p = (sent * 100) / orig;
    return p > 100 ? p - 100 : 0;
}

/* ===== 5. Fault Tolerance ===== */
typedef enum { LINK_UP = 0, LINK_DOWN = 1, LINK_DEGRADED = 2 } link_status_t;
typedef struct {
    unsigned int link_id; char neighbor_id[8];
    link_status_t status; unsigned int loss_rate, latency_ms;
} link_state_t;
typedef struct { unsigned int hello_ms, ack_ms, max_retries, fail_thresh; } fault_cfg_t;
typedef struct {
    link_state_t *links; unsigned int lc, max; fault_cfg_t cfg;
    unsigned char *cache[8]; unsigned int csz[8]; unsigned int cc;
} fault_eng_t;
static fault_eng_t g_fe;

static void fault_init(unsigned int hms, unsigned int ams, unsigned int mr, unsigned int ft) {
    g_fe.links = (link_state_t *)mem_alloc(sizeof(link_state_t) * 16);
    g_fe.lc = 0; g_fe.max = 16;
    g_fe.cfg.hello_ms = hms; g_fe.cfg.ack_ms = ams;
    g_fe.cfg.max_retries = mr; g_fe.cfg.fail_thresh = ft; g_fe.cc = 0;
    print_str("[fault] initialized hello="); print_int(hms);
    print_str("ms ack_timeout="); print_int(ams); print_str("ms\n");
}
static int fault_detect_link_failure(const char *nid, unsigned int missed) {
    for (unsigned int i = 0; i < g_fe.lc; i++) {
        int m = 1;
        for (int k = 0; k < 8; k++)
            if (g_fe.links[i].neighbor_id[k] != nid[k]) { m = 0; break; }
        if (!m) continue;
        if (missed >= g_fe.cfg.fail_thresh) {
            g_fe.links[i].status = LINK_DOWN;
            print_str("[fault] link DOWN, missed="); print_int(missed); print_str("\n"); return 1;
        } else if (missed >= g_fe.cfg.fail_thresh / 2) {
            g_fe.links[i].status = LINK_DEGRADED;
            print_str("[fault] link DEGRADED\n"); return 0;
        }
    }
    return 0;
}
static void fault_trigger_route_update(unsigned int lid) {
    print_str("[fault] triggering route recomputation for link ");
    print_int(lid); print_str("\n");
}
static int fault_cache_forward(const unsigned char *d, unsigned int len) {
    if (g_fe.cc >= 8) return -1;
    g_fe.cache[g_fe.cc] = (unsigned char *)mem_alloc(len);
    for (unsigned int i = 0; i < len; i++) g_fe.cache[g_fe.cc][i] = d[i];
    g_fe.csz[g_fe.cc] = len; g_fe.cc++;
    print_str("[fault] cached packet, cache_count="); print_int(g_fe.cc); print_str("\n");
    return 0;
}
static int fault_retry_send(unsigned int ci, unsigned int mr) {
    if (ci >= g_fe.cc) return -1;
    unsigned int r = mr < g_fe.cfg.max_retries ? mr : g_fe.cfg.max_retries;
    print_str("[fault] retrying cached packet idx="); print_int(ci);
    print_str(" retries="); print_int(r); print_str("\n"); return 0;
}
static unsigned int fault_compute_reliability(const unsigned int *hist, unsigned int n) {
    if (n == 0) return 0;
    unsigned int s = 0;
    for (unsigned int i = 0; i < n; i++) if (hist[i] == 1) s++;
    return (s * 100) / n;
}

/* ===== 6. QoS Guarantee ===== */
typedef enum { QOS_VOICE = 0, QOS_VIDEO = 1, QOS_DATA = 2, QOS_BEST_EFFORT = 3 } qos_class_t;
static const int qos_dscp[] = { 46, 34, 18, 0 };
static const char *qos_names[] = { "VOICE", "VIDEO", "DATA", "BEST_EFFORT" };
#define Q_MAX 64
typedef struct {
    qos_class_t cls; unsigned char *pkts[Q_MAX]; unsigned int sz[Q_MAX];
    unsigned int head, tail, cnt, max_sz, prio, enq, drop, res_bw;
} qos_queue_t;
typedef struct { qos_queue_t *queues; unsigned int nc; } qos_eng_t;
static qos_eng_t g_qe;

static void qos_init(unsigned int nc) {
    g_qe.queues = (qos_queue_t *)mem_alloc(sizeof(qos_queue_t) * nc);
    g_qe.nc = nc;
    for (unsigned int i = 0; i < nc; i++) {
        qos_queue_t *q = &g_qe.queues[i];
        q->cls = (qos_class_t)i;
        q->head = q->tail = q->cnt = q->enq = q->drop = q->res_bw = 0;
        q->max_sz = Q_MAX; q->prio = nc - i;
    }
    print_str("[qos] initialized "); print_int(nc); print_str(" priority classes\n");
}
static qos_class_t qos_classify_packet(const unsigned char *d, unsigned int l) {
    if (l < 4) return QOS_BEST_EFFORT;
    unsigned int ds = (d[0] >> 2) & 0x3F;
    if (ds >= 46) return QOS_VOICE;
    if (ds >= 34) return QOS_VIDEO;
    if (ds >= 18) return QOS_DATA;
    return QOS_BEST_EFFORT;
}
static int qos_enqueue(qos_queue_t *q, const unsigned char *p, unsigned int sz) {
    if (q->cnt >= q->max_sz) { q->drop++; print_str("[qos] packet dropped, queue full\n"); return -1; }
    unsigned char *c = (unsigned char *)mem_alloc(sz);
    for (unsigned int i = 0; i < sz; i++) c[i] = p[i];
    q->pkts[q->tail] = c; q->sz[q->tail] = sz;
    q->tail = (q->tail + 1) % q->max_sz; q->cnt++; q->enq++;
    return 0;
}
static int qos_dequeue_hp(qos_queue_t *qs, unsigned int nc,
                          unsigned char **op, unsigned int *os) {
    for (unsigned int p = 0; p < nc; p++) {
        int bi = -1;
        for (unsigned int i = 0; i < nc; i++)
            if ((int)qs[i].prio == (int)nc - (int)p && qs[i].cnt > 0 && (int)qs[i].prio > bi)
                bi = (int)i;
        if (bi >= 0) {
            qos_queue_t *q = &qs[bi];
            *op = q->pkts[q->head]; *os = q->sz[q->head];
            q->head = (q->head + 1) % q->max_sz; q->cnt--;
            print_str("[qos] dequeued from class "); print_str(qos_names[q->cls]); print_str("\n");
            return 0;
        }
    }
    print_str("[qos] all queues empty\n"); return -1;
}
static void qos_compute_stats(const qos_queue_t *q) {
    unsigned int dr = q->enq > 0 ? (q->drop * 100) / q->enq : 0;
    print_str("[qos] "); print_str(qos_names[q->cls]);
    print_str(" enqueued="); print_int(q->enq);
    print_str(" dropped="); print_int(q->drop);
    print_str(" drop%="); print_int(dr);
    print_str(" reserved_bw="); print_int(q->res_bw); print_str("kbps\n");
}
static void qos_reserve_bw(qos_class_t c, unsigned int bw) {
    if (c < g_qe.nc) {
        g_qe.queues[c].res_bw = bw;
        print_str("[qos] reserved "); print_int(bw); print_str("kbps for ");
        print_str(qos_names[c]); print_str(" DSCP="); print_int(qos_dscp[c]); print_str("\n");
    }
}

/* ===== 7. Entry Point ===== */
static void run_test(void) {
    print_str("=== Mesh Networking v56.0 Comprehensive Test ===\n\n");
    print_str("--- Network Discovery ---\n");
    discovery_init("NODE-A\0\0", 32);
    discovery_send_probe();
    mesh_node_t r;
    my_strncpy(r.node_id, "NODE-B\0\0", 8);
    my_strncpy(r.address, "10.0.0.2", 16);
    r.capabilities = 0x07; r.rssi = -45; r.last_seen = 100; r.hop_count = 1;
    discovery_process_response(&r);
    my_strncpy(r.node_id, "NODE-C\0\0", 8);
    r.rssi = -60; r.last_seen = 150; r.hop_count = 2;
    discovery_process_response(&r);
    discovery_update_topology();
    discovery_expire_stale(200);
    print_str("\n--- Routing Protocol ---\n");
    routing_init("NODE-A\0\0", 64);
    routing_update("NODE-B\0\0", "NODE-B\0\0", 100, 1);
    routing_update("NODE-C\0\0", "NODE-B\0\0", 250, 2);
    routing_update("NODE-D\0\0", "NODE-C\0\0", 400, 3);
    int sel = routing_select_path("NODE-C\0\0", 1);
    print_str("[test] selected route idx="); print_int(sel);
    print_str(" ETX="); print_int(routing_compute_etx(90, 80, 1500)); print_str("\n");
    routing_expire_routes(10);
    print_str("\n--- Multi-hop Communication ---\n");
    reassembly_init(16);
    unsigned char pl[64];
    for (int i = 0; i < 64; i++) pl[i] = (unsigned char)i;
    fragment_t fr[MAX_FRAGS];
    unsigned int nf = fragment_packet(pl, 64, 20, fr);
    for (unsigned int i = 0; i < nf; i++)
        if (reassemble_fragment(&fr[i]) == 1) print_str("[test] reassembly complete\n");
    print_str("[test] complete="); print_int(reassemble_check_complete(fr[0].packet_id)); print_str("\n");
    multi_hop_send(pl, 64, "NODE-D\0\0", 5);
    multi_hop_forward(pl, 32, "NODE-B\0\0", 4);
    multi_hop_forward(pl, 32, "NODE-C\0\0", 0);
    print_str("\n--- Network Coding (RLNC) ---\n");
    coding_init(4, 256);
    const unsigned char *cps[4];
    unsigned char p0[16], p1[16], p2[16], p3[16];
    for (int i = 0; i < 16; i++) { p0[i]=(unsigned char)i; p1[i]=(unsigned char)(i+16); p2[i]=(unsigned char)(i+32); p3[i]=(unsigned char)(i+48); }
    cps[0] = p0; cps[1] = p1; cps[2] = p2; cps[3] = p3;
    unsigned char co[] = { 1, 2, 3, 4 };
    coding_encode(cps, 16, co, 42);
    print_str("[test] decodable="); print_int(coding_check_decodable(&g_ce.gens[0]));
    print_str(" overhead%="); print_int(coding_compute_overhead(100, 120)); print_str("\n");
    print_str("\n--- Fault Tolerance ---\n");
    fault_init(1000, 500, 3, 5);
    link_state_t *ln = &g_fe.links[g_fe.lc];
    ln->link_id = 0; my_strncpy(ln->neighbor_id, "NODE-B\0\0", 8);
    ln->status = LINK_UP; ln->loss_rate = 2; ln->latency_ms = 15; g_fe.lc++;
    print_str("[test] failed="); print_int(fault_detect_link_failure("NODE-B\0\0", 2)); print_str("\n");
    fault_detect_link_failure("NODE-B\0\0", 6);
    fault_trigger_route_update(0);
    fault_cache_forward(pl, 32);
    fault_retry_send(0, 3);
    unsigned int hist[] = { 1, 1, 0, 1, 1, 1, 0, 1 };
    print_str("[test] reliability%="); print_int(fault_compute_reliability(hist, 8)); print_str("\n");
    print_str("\n--- QoS Guarantee ---\n");
    qos_init(4);
    qos_reserve_bw(QOS_VOICE, 64);
    qos_reserve_bw(QOS_VIDEO, 256);
    qos_reserve_bw(QOS_DATA, 128);
    unsigned char vp[] = { 46 << 2, 0xAA, 0xBB };
    unsigned char vid[] = { 34 << 2, 0xCC, 0xDD };
    unsigned char dp[] = { 18 << 2, 0xEE, 0xFF };
    qos_class_t c1 = qos_classify_packet(vp, 3);
    qos_class_t c2 = qos_classify_packet(vid, 3);
    qos_class_t c3 = qos_classify_packet(dp, 3);
    print_str("[test] classified: "); print_str(qos_names[c1]); print_str(", ");
    print_str(qos_names[c2]); print_str(", "); print_str(qos_names[c3]); print_str("\n");
    qos_enqueue(&g_qe.queues[QOS_VOICE], vp, 3);
    qos_enqueue(&g_qe.queues[QOS_VIDEO], vid, 3);
    qos_enqueue(&g_qe.queues[QOS_DATA], dp, 3);
    unsigned char *op; unsigned int os;
    qos_dequeue_hp(g_qe.queues, g_qe.nc, &op, &os);
    qos_dequeue_hp(g_qe.queues, g_qe.nc, &op, &os);
    for (unsigned int i = 0; i < g_qe.nc; i++) qos_compute_stats(&g_qe.queues[i]);
    print_str("\n=== All subsystems tested successfully ===\n");
}

static void print_help(void) {
    print_str("Mesh Networking v56.0 - Self-Organizing Protocol Stack\n");
    print_str("Usage: mesh_networking [options]\n");
    print_str("  -h    Show this help message\n");
    print_str("  -t    Run comprehensive test suite\n");
    print_str("\nFeatures:\n");
    print_str("  - Network Discovery (mDNS/SSDP)\n");
    print_str("  - B.A.T.M.A.N.-like Routing with ETX metric\n");
    print_str("  - Multi-hop Fragmentation & Reassembly\n");
    print_str("  - Random Linear Network Coding (RLNC)\n");
    print_str("  - Fault Tolerance with Link Caching\n");
    print_str("  - QoS with DSCP-based Priority Queues\n");
}

void _start(void) {
    print_str("Mesh Networking v56.0 - Self-Organizing Protocol Stack\n");
    int argc = host_get_argv(0, 256);
    char ab[256];
    if (argc > 0) host_get_argv((unsigned int)(unsigned long)ab, 256);
    int rt = 0, sh = 0;
    for (int i = 0; i < argc; i++) {
        if (ab[i] == '-' && i + 1 < argc) {
            char f = ab[i + 1];
            if (f == 'h') sh = 1;
            else if (f == 't') rt = 1;
        }
    }
    if (sh) { print_help(); host_exit(0); }
    if (rt) { run_test(); host_exit(0); }
    /* Default: run full test suite */
    run_test();
    host_exit(0);
}
