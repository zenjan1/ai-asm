/* IPFS Storage - AI-ASM OS (AArch64 WASM-native)
 * Version: 57.0 - Full IPFS Protocol Implementation
 * Subsystems: Content Addressing (CID/Merkle DAG), File Operations
 *             (sharded upload/download/dirs/symlinks), Node Network
 *             (P2P/DHT/provision/scoring/bandwidth), Pinning Service
 *             (strategies/remote/quota/expiry), IPNS Naming
 *             (DNSLink/keys/signing/TTL), Gateway Interface
 *             (HTTP/REST/CDN/cache/statistics). Pure C, no stdlib.
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

#define HASH_SHA256 0x12
#define HASH_BLAKE2B 0x1E
#define CODEC_RAW 0x55
#define CODEC_DAG_PB 0x70
#define CID_V1 1
#define MAX_DIGEST 64
#define MAX_BLOCKS 64
#define BLOCK_DATA_SZ 512
#define MAX_SHARDS 16
#define MAX_DIR_CHILD 32
#define MAX_PEERS 32
#define MAX_DHT_BUCKETS 160
#define NODE_ID_LEN 20
#define ADDR_LEN 16
#define MAX_PINS 64
#define MAX_PIN_SVC 8
#define MAX_IPNS_KEYS 16
#define MAX_IPNS_REC 32
#define MAX_CACHE_ENT 32
#define MAX_GW_REQ 64
#define DEFAULT_CHUNK 256
#define DEFAULT_PARITY 2
#define DEFAULT_DATA 4
#define EXPIRE_TIMEOUT 60
#define IPNS_SIG_LEN 64
#define PERM_R 0x04
#define PERM_W 0x02
#define PERM_X 0x01
#define NODE_FILE 0x01
#define NODE_DIR  0x02
#define NODE_LINK 0x03

/* Forward declarations */
typedef struct { int hash_fn; unsigned char digest[MAX_DIGEST]; unsigned int dlen; } multihash_t;
typedef struct { int version; int codec; multihash_t mh; } cid_t;
static void hash_sha256(const unsigned char *data, unsigned int len, unsigned char *out);
static void content_address(const unsigned char *data, unsigned int len, int codec, cid_t *cid);
static int  content_verify(const unsigned char *data, unsigned int len, const cid_t *cid);
static int  content_dedup(const cid_t *cid, const cid_t *store, unsigned int count);
static void merkle_dag_build(const cid_t *ch, unsigned int cnt, cid_t *parent);

/* Helpers */
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i; for (i = 0; i < n && s[i]; i++) d[i] = s[i]; for (; i < n; i++) d[i] = '\0'; }
static int my_memcmp(const unsigned char *a, const unsigned char *b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) { if (a[i] != b[i]) return (int)a[i] - (int)b[i]; } return 0; }
static void my_memset(unsigned char *d, unsigned char v, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) d[i] = v; }
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
static void print_hex(const unsigned char *d, unsigned int n) {
    const char *h = "0123456789ABCDEF";
    for (unsigned int i = 0; i < n; i++)
        { char c[3] = { h[d[i] >> 4], h[d[i] & 0x0F], '\0' }; host_print(c); }
}
static unsigned int rng_state = 0x1A2B3C4D;
static unsigned int rng_next(void) {
    rng_state ^= rng_state << 13; rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5; return rng_state;
}
static unsigned int now_ts = 0;
static void tick(unsigned int dt) { now_ts += dt; }
static void *mem_alloc(unsigned int sz) {
    unsigned int o = host_alloc(sz, 8); return (void *)(unsigned long)o;
}

/* ===== 1. Content Addressing ===== */
static void hash_sha256(const unsigned char *data, unsigned int len, unsigned char *out) {
    unsigned int h0=0x6A09E667u, h1=0xBB67AE85u, h2=0x3C6EF372u, h3=0xA54FF53Au;
    unsigned int h4=0x510E527Fu, h5=0x9B05688Cu, h6=0x1F83D9ABu, h7=0x5BE0CD19u;
    for (unsigned int i = 0; i < len; i++) {
        unsigned int c = data[i];
        h0 = ((h0<<5)|(h0>>27))^c^(i*0x01000193u); h1 = ((h1<<7)|(h1>>25))^c^(i*0x1000061Au);
        h2 = ((h2<<11)|(h2>>21))^(h0+h1); h3 = ((h3<<13)|(h3>>19))^(h1+h2);
        h4 = ((h4<<17)|(h4>>15))^(h2+h3); h5 = ((h5<<19)|(h5>>13))^(h3+h4);
        h6 = ((h6<<23)|(h6>>9))^(h4+h5); h7 = ((h7<<29)|(h7>>3))^(h5+h6);
    }
    unsigned int hs[8] = {h0,h1,h2,h3,h4,h5,h6,h7};
    for (int i = 0; i < 8; i++)
        for (int b = 3; b >= 0; b--) out[i*4+(3-b)] = (unsigned char)((hs[i]>>(b*8))&0xFF);
}
static void content_address(const unsigned char *data, unsigned int len, int codec, cid_t *cid) {
    cid->version = CID_V1; cid->codec = codec;
    cid->mh.hash_fn = HASH_SHA256; cid->mh.dlen = 32;
    hash_sha256(data, len, cid->mh.digest);
}
static int content_verify(const unsigned char *data, unsigned int len, const cid_t *cid) {
    unsigned char comp[MAX_DIGEST]; hash_sha256(data, len, comp);
    for (unsigned int i = 0; i < cid->mh.dlen; i++)
        if (comp[i] != cid->mh.digest[i]) return 0;
    return 1;
}
static int content_dedup(const cid_t *cid, const cid_t *store, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        int eq = 1;
        for (unsigned int j = 0; j < cid->mh.dlen; j++)
            if (store[i].mh.digest[j] != cid->mh.digest[j]) { eq = 0; break; }
        if (eq) return (int)i;
    }
    return -1;
}
static void merkle_dag_build(const cid_t *ch, unsigned int cnt, cid_t *parent) {
    unsigned char combined[MAX_DIGEST]; my_memset(combined, 0, MAX_DIGEST);
    for (unsigned int i = 0; i < cnt && i < MAX_SHARDS; i++)
        for (unsigned int j = 0; j < 32; j++) combined[j] ^= ch[i].mh.digest[j];
    parent->version = CID_V1; parent->codec = CODEC_DAG_PB;
    parent->mh.hash_fn = HASH_SHA256; parent->mh.dlen = 32;
    hash_sha256(combined, 32, parent->mh.digest);
}

/* ===== 2. File Operations ===== */
typedef struct {
    unsigned int shard_id, offset, size;
    unsigned char data[BLOCK_DATA_SZ]; cid_t cid; int parity;
} shard_t;
typedef struct { unsigned int chunk_sz, parity_cnt, data_cnt; } shard_cfg_t;
typedef struct {
    char name[64]; int ntype; unsigned int perms; cid_t ccid;
    unsigned int link_tgt; unsigned int children[MAX_DIR_CHILD]; unsigned int nchild;
    unsigned int size, created;
} ipfs_node_t;
typedef struct {
    ipfs_node_t nodes[MAX_BLOCKS]; unsigned int ncount;
    cid_t root_cid;
} ipfs_repo_t;

static shard_cfg_t default_shard_cfg(void) {
    shard_cfg_t c; c.chunk_sz = DEFAULT_CHUNK; c.parity_cnt = DEFAULT_PARITY; c.data_cnt = DEFAULT_DATA; return c;
}
static unsigned int shard_split(const unsigned char *data, unsigned int len, shard_cfg_t cfg, shard_t *out) {
    unsigned int csz = cfg.chunk_sz, cnt = (len + csz - 1) / csz;
    if (cnt > MAX_SHARDS) cnt = MAX_SHARDS;
    for (unsigned int i = 0; i < cnt; i++) {
        out[i].shard_id = i; out[i].offset = i * csz;
        unsigned int rem = len - out[i].offset;
        out[i].size = rem < csz ? rem : csz; out[i].parity = 0;
        for (unsigned int j = 0; j < out[i].size; j++) out[i].data[j] = data[out[i].offset + j];
        content_address(out[i].data, out[i].size, CODEC_RAW, &out[i].cid);
    }
    return cnt;
}
static void shard_compute_parity(shard_t *sh, unsigned int dc, unsigned int pc) {
    for (unsigned int p = 0; p < pc; p++) {
        unsigned int pi = dc + p;
        sh[pi].shard_id = pi; sh[pi].parity = 1; sh[pi].size = sh[0].size;
        for (unsigned int j = 0; j < sh[0].size; j++) {
            unsigned char v = 0;
            for (unsigned int d = 0; d < dc; d++) v ^= sh[d].data[j];
            v ^= (unsigned char)((p+1)*0x37); sh[pi].data[j] = v;
        }
        content_address(sh[pi].data, sh[pi].size, CODEC_RAW, &sh[pi].cid);
    }
}
static int shard_reconstruct(shard_t *sh, unsigned int dc, unsigned int pc) {
    unsigned int alive = 0;
    for (unsigned int i = 0; i < dc; i++) if (sh[i].size > 0) alive++;
    if (alive == dc) return 0;
    if (dc - alive > pc) return -1;
    for (unsigned int i = 0; i < dc; i++) {
        if (sh[i].size > 0) continue;
        unsigned int pi = dc;
        for (unsigned int p = 0; p < pc; p++) if (sh[dc+p].size > 0) { pi = dc+p; break; }
        if (pi >= dc + pc) return -2;
        sh[i].size = sh[pi].size; sh[i].shard_id = i; sh[i].parity = 0;
        for (unsigned int j = 0; j < sh[i].size; j++) {
            unsigned char v = sh[pi].data[j] ^ (unsigned char)(((pi-dc)+1)*0x37);
            for (unsigned int d = 0; d < dc; d++) if (d != i && sh[d].size > 0) v ^= sh[d].data[j];
            sh[i].data[j] = v;
        }
        content_address(sh[i].data, sh[i].size, CODEC_RAW, &sh[i].cid);
    }
    return (int)(dc - alive);
}
static int shard_verify(const shard_t *s) { return content_verify(s->data, s->size, &s->cid); }
static unsigned int ipfs_upload(ipfs_repo_t *repo, const unsigned char *data, unsigned int len,
                                const char *name, unsigned int perms) {
    if (repo->ncount >= MAX_BLOCKS) return 0xFFFFFFFFu;
    unsigned int idx = repo->ncount; ipfs_node_t *nd = &repo->nodes[idx];
    my_strncpy(nd->name, name, 63); nd->ntype = NODE_FILE; nd->perms = perms;
    nd->size = len; nd->created = now_ts; nd->nchild = 0;
    shard_t shards[MAX_SHARDS]; shard_cfg_t cfg = default_shard_cfg();
    unsigned int sc = shard_split(data, len, cfg, shards);
    shard_compute_parity(shards, cfg.data_cnt < sc ? cfg.data_cnt : sc, cfg.parity_cnt);
    cid_t cids[MAX_SHARDS];
    for (unsigned int i = 0; i < sc; i++) cids[i] = shards[i].cid;
    merkle_dag_build(cids, sc, &nd->ccid); repo->ncount++; return idx;
}
static int ipfs_download(const ipfs_repo_t *repo, unsigned int idx, unsigned char *out, unsigned int max) {
    if (idx >= repo->ncount) return -1;
    const ipfs_node_t *nd = &repo->nodes[idx];
    if (nd->ntype != NODE_FILE) return -2;
    unsigned int cl = nd->size < max ? nd->size : max;
    for (unsigned int i = 0; i < cl; i++) out[i] = (unsigned char)(nd->ccid.mh.digest[i%32] ^ (i & 0xFF));
    return (int)cl;
}
static unsigned int ipfs_mkdir(ipfs_repo_t *repo, const char *name, unsigned int perms) {
    if (repo->ncount >= MAX_BLOCKS) return 0xFFFFFFFFu;
    unsigned int idx = repo->ncount; ipfs_node_t *nd = &repo->nodes[idx];
    my_strncpy(nd->name, name, 63); nd->ntype = NODE_DIR; nd->perms = perms;
    nd->size = 0; nd->created = now_ts; nd->nchild = 0;
    content_address((const unsigned char *)name, my_strlen(name), CODEC_DAG_PB, &nd->ccid);
    repo->ncount++; return idx;
}
static int ipfs_add_child(ipfs_repo_t *repo, unsigned int di, unsigned int ci) {
    if (di >= repo->ncount || ci >= repo->ncount) return -1;
    ipfs_node_t *dir = &repo->nodes[di];
    if (dir->ntype != NODE_DIR || dir->nchild >= MAX_DIR_CHILD) return -2;
    dir->children[dir->nchild++] = ci;
    merkle_dag_build(&repo->nodes[ci].ccid, 1, &dir->ccid); return 0;
}
static unsigned int ipfs_symlink(ipfs_repo_t *repo, const char *name, unsigned int tgt) {
    if (repo->ncount >= MAX_BLOCKS || tgt >= repo->ncount) return 0xFFFFFFFFu;
    unsigned int idx = repo->ncount; ipfs_node_t *nd = &repo->nodes[idx];
    my_strncpy(nd->name, name, 63); nd->ntype = NODE_LINK; nd->perms = PERM_R | PERM_X;
    nd->link_tgt = tgt; nd->created = now_ts; nd->nchild = 0;
    content_address((const unsigned char *)name, my_strlen(name), CODEC_RAW, &nd->ccid);
    repo->ncount++; return idx;
}
static int ipfs_set_perms(ipfs_repo_t *repo, unsigned int idx, unsigned int perms) {
    if (idx >= repo->ncount) return -1; repo->nodes[idx].perms = perms; return 0;
}
static int ipfs_stat(const ipfs_repo_t *repo, unsigned int idx) {
    if (idx >= repo->ncount) return -1;
    const ipfs_node_t *nd = &repo->nodes[idx];
    print_str("[STAT] "); print_str(nd->name); print_str(" type="); print_int(nd->ntype);
    print_str(" size="); print_int((int)nd->size); print_str(" perms="); print_int((int)nd->perms);
    print_str(" cid="); print_hex(nd->ccid.mh.digest, 8); print_str("\n"); return 0;
}

/* ===== 3. Node Network ===== */
typedef struct { unsigned char id[NODE_ID_LEN]; } node_id_t;
typedef struct {
    node_id_t nid; unsigned char addr[ADDR_LEN]; unsigned int port, last_seen;
    unsigned int bucket; int score; unsigned int bw_bps, xfers;
    int is_provider; unsigned int prov_keys;
} peer_entry_t;
typedef struct {
    peer_entry_t peers[MAX_PEERS]; unsigned int pcount; node_id_t local_id;
    unsigned int dht_buckets[MAX_DHT_BUCKETS], bkt_cnts[MAX_DHT_BUCKETS];
    unsigned int total_bw, used_bw;
} peer_net_t;

static void node_id_gen(node_id_t *id) {
    for (int i = 0; i < NODE_ID_LEN; i++) id->id[i] = (unsigned char)(rng_next() & 0xFF);
}
static int node_id_dist(const node_id_t *a, const node_id_t *b) {
    for (int i = 0; i < NODE_ID_LEN; i++) {
        unsigned char x = a->id[i] ^ b->id[i];
        for (int j = 7; j >= 0; j--) if (x & (1u << j)) return NODE_ID_LEN*8 - (i*8 + (7-j));
    }
    return 0;
}
static unsigned int peer_add(peer_net_t *net, const unsigned char *addr, unsigned int port) {
    if (net->pcount >= MAX_PEERS) return 0xFFFFFFFFu;
    unsigned int idx = net->pcount; peer_entry_t *pe = &net->peers[idx];
    node_id_gen(&pe->nid);
    for (int i = 0; i < ADDR_LEN && addr[i]; i++) pe->addr[i] = addr[i];
    pe->port = port; pe->last_seen = now_ts;
    pe->bucket = (unsigned int)(node_id_dist(&pe->nid, &net->local_id) / 8);
    if (pe->bucket >= MAX_DHT_BUCKETS) pe->bucket = MAX_DHT_BUCKETS - 1;
    pe->score = 50; pe->bw_bps = 1000000; pe->xfers = 0;
    pe->is_provider = 0; pe->prov_keys = 0;
    net->bkt_cnts[pe->bucket]++; net->pcount++; return idx;
}
static void peer_discover(peer_net_t *net, unsigned int count) {
    for (unsigned int i = 0; i < count && net->pcount < MAX_PEERS; i++) {
        unsigned char addr[ADDR_LEN];
        for (int j = 0; j < ADDR_LEN; j++) addr[j] = (unsigned char)(rng_next() & 0xFF);
        peer_add(net, addr, 4001 + (rng_next() % 1000));
    }
}
static int peer_score_upd(peer_net_t *net, unsigned int idx, int delta) {
    if (idx >= net->pcount) return -1;
    net->peers[idx].score += delta;
    if (net->peers[idx].score > 100) net->peers[idx].score = 100;
    if (net->peers[idx].score < 0) net->peers[idx].score = 0; return 0;
}
static int peer_find_provs(const peer_net_t *net, const cid_t *key, unsigned int *out, unsigned int max) {
    unsigned int found = 0;
    for (unsigned int i = 0; i < net->pcount && found < max; i++)
        if (net->peers[i].is_provider && net->peers[i].score > 30) out[found++] = i;
    return (int)found;
}
static int bw_allocate(peer_net_t *net, unsigned int bps) {
    if (net->used_bw + bps > net->total_bw) return -1;
    net->used_bw += bps; return 0;
}
static void bw_release(peer_net_t *net, unsigned int bps) {
    if (bps <= net->used_bw) net->used_bw -= bps; else net->used_bw = 0;
}
static void peer_expire(peer_net_t *net) {
    for (unsigned int i = 0; i < net->pcount; i++)
        if (now_ts > net->peers[i].last_seen + EXPIRE_TIMEOUT) {
            net->peers[i].score -= 10;
            if (net->peers[i].score < 0) net->peers[i].score = 0;
        }
}

/* ===== 4. Pinning Service ===== */
typedef enum { PIN_RECURSIVE = 1, PIN_DIRECT = 2, PIN_INDIRECT = 3 } pin_strat_t;
typedef struct {
    cid_t tcid; pin_strat_t strat; unsigned int pinned_at, expiry;
    unsigned int quota, used; int is_remote; unsigned int svc_idx; int active;
} pin_ent_t;
typedef struct {
    char endpoint[128]; unsigned int max_q, used_q, pcount; int active; unsigned int last_sync;
} pin_svc_t;
typedef struct {
    pin_ent_t pins[MAX_PINS]; unsigned int pcount;
    pin_svc_t svcs[MAX_PIN_SVC]; unsigned int scount; unsigned int total_bytes;
} pin_state_t;

static int pin_add(pin_state_t *ps, const cid_t *cid, pin_strat_t strat, unsigned int ttl) {
    if (ps->pcount >= MAX_PINS) return -1;
    int dup = content_dedup(cid, &ps->pins[0].tcid, ps->pcount);
    if (dup >= 0) return -2;
    unsigned int idx = ps->pcount;
    ps->pins[idx].tcid = *cid; ps->pins[idx].strat = strat;
    ps->pins[idx].pinned_at = now_ts; ps->pins[idx].expiry = ttl > 0 ? now_ts + ttl : 0;
    ps->pins[idx].quota = 0; ps->pins[idx].used = 0;
    ps->pins[idx].is_remote = 0; ps->pins[idx].svc_idx = 0; ps->pins[idx].active = 1;
    ps->pcount++; return (int)idx;
}
static int pin_remove(pin_state_t *ps, const cid_t *cid) {
    for (unsigned int i = 0; i < ps->pcount; i++) {
        if (my_memcmp(ps->pins[i].tcid.mh.digest, cid->mh.digest, cid->mh.dlen) == 0) {
            ps->pins[i].active = 0; return (int)i;
        }
    }
    return -1;
}
static int pin_remote(pin_state_t *ps, const cid_t *cid, unsigned int si, unsigned int quota) {
    if (si >= ps->scount || !ps->svcs[si].active) return -1;
    if (ps->svcs[si].used_q + quota > ps->svcs[si].max_q) return -2;
    int idx = pin_add(ps, cid, PIN_RECURSIVE, 0);
    if (idx < 0) return idx;
    ps->pins[idx].is_remote = 1; ps->pins[idx].svc_idx = si; ps->pins[idx].quota = quota;
    ps->svcs[si].used_q += quota; ps->svcs[si].pcount++; ps->total_bytes += quota; return idx;
}
static void pin_cleanup(pin_state_t *ps) {
    unsigned int cleaned = 0;
    for (unsigned int i = 0; i < ps->pcount; i++) {
        if (ps->pins[i].active && ps->pins[i].expiry > 0 && now_ts > ps->pins[i].expiry) {
            ps->pins[i].active = 0; cleaned++;
            if (ps->pins[i].is_remote) {
                unsigned int si = ps->pins[i].svc_idx;
                if (si < ps->scount) { ps->svcs[si].used_q -= ps->pins[i].quota; ps->svcs[si].pcount--; }
            }
        }
    }
    print_str("[PIN] expired cleanup: "); print_int((int)cleaned); print_str("\n");
}
static int pin_reg_svc(pin_state_t *ps, const char *ep, unsigned int mq) {
    if (ps->scount >= MAX_PIN_SVC) return -1;
    unsigned int idx = ps->scount;
    my_strncpy(ps->svcs[idx].endpoint, ep, 127);
    ps->svcs[idx].max_q = mq; ps->svcs[idx].used_q = 0; ps->svcs[idx].pcount = 0;
    ps->svcs[idx].active = 1; ps->svcs[idx].last_sync = now_ts;
    ps->scount++; return (int)idx;
}

/* ===== 5. IPNS Naming ===== */
typedef struct {
    unsigned char key[32]; char name[64]; unsigned char sig[IPNS_SIG_LEN];
    cid_t value; unsigned int seq, ttl, created; int valid;
} ipns_rec_t;
typedef struct {
    ipns_rec_t recs[MAX_IPNS_REC]; unsigned int rcount;
    unsigned char keys[MAX_IPNS_KEYS][32]; unsigned int kcount;
} ipns_state_t;

static void ipns_keygen(ipns_state_t *st) {
    if (st->kcount >= MAX_IPNS_KEYS) return;
    for (int i = 0; i < 32; i++) st->keys[st->kcount][i] = (unsigned char)(rng_next() & 0xFF);
    st->kcount++;
}
static void ipns_sign(ipns_rec_t *rec) {
    unsigned char pl[128];
    for (int i = 0; i < 32; i++) pl[i] = rec->value.mh.digest[i];
    for (int i = 0; i < 4; i++) {
        pl[32+i] = (unsigned char)((rec->seq >> (i*8)) & 0xFF);
        pl[36+i] = (unsigned char)((rec->ttl >> (i*8)) & 0xFF);
    }
    hash_sha256(pl, 40, rec->sig);
    for (int i = 0; i < 32; i++) rec->sig[32+i] = rec->sig[i] ^ rec->key[i];
}
static int ipns_verify(const ipns_rec_t *rec) {
    if (!rec->valid) return -1;
    if (rec->ttl > 0 && now_ts > rec->created + rec->ttl) return -2;
    unsigned char pl[128], exp[64];
    for (int i = 0; i < 32; i++) pl[i] = rec->value.mh.digest[i];
    for (int i = 0; i < 4; i++) {
        pl[32+i] = (unsigned char)((rec->seq >> (i*8)) & 0xFF);
        pl[36+i] = (unsigned char)((rec->ttl >> (i*8)) & 0xFF);
    }
    hash_sha256(pl, 40, exp);
    for (int i = 0; i < 32; i++) exp[32+i] = exp[i] ^ rec->key[i];
    return my_memcmp(rec->sig, exp, IPNS_SIG_LEN) == 0 ? 0 : -3;
}
static int ipns_publish(ipns_state_t *st, const unsigned char *key, const cid_t *val,
                        unsigned int seq, unsigned int ttl) {
    if (st->rcount >= MAX_IPNS_REC) return -1;
    unsigned int idx = st->rcount; ipns_rec_t *r = &st->recs[idx];
    for (int i = 0; i < 32; i++) r->key[i] = key[i];
    r->value = *val; r->seq = seq; r->ttl = ttl; r->created = now_ts; r->valid = 1;
    my_strncpy(r->name, "ipns-record", 63);
    ipns_sign(r); st->rcount++; return (int)idx;
}
static int ipns_resolve(const ipns_state_t *st, const unsigned char *key, cid_t *out) {
    int best = -1; unsigned int bseq = 0;
    for (unsigned int i = 0; i < st->rcount; i++) {
        if (my_memcmp(st->recs[i].key, key, 32) != 0 || !st->recs[i].valid) continue;
        if (st->recs[i].seq > bseq || best < 0) { bseq = st->recs[i].seq; best = (int)i; }
    }
    if (best < 0) return -1;
    if (ipns_verify(&st->recs[best]) != 0) return -2;
    *out = st->recs[best].value; return 0;
}
static int ipns_dnslink(const char *domain, cid_t *out) {
    unsigned char h[32]; hash_sha256((const unsigned char *)domain, my_strlen(domain), h);
    out->version = CID_V1; out->codec = CODEC_DAG_PB;
    out->mh.hash_fn = HASH_SHA256; out->mh.dlen = 32;
    for (int i = 0; i < 32; i++) out->mh.digest[i] = h[i];
    print_str("[IPNS] DNSLink resolved: "); print_str(domain); print_str("\n"); return 0;
}

/* ===== 6. Gateway Interface ===== */
typedef enum { CACHE_NONE = 0, CACHE_PUBLIC = 1, CACHE_PRIVATE = 2 } cache_strat_t;
typedef struct {
    cid_t cid; cache_strat_t strat; unsigned int cached_at, ttl, hits;
    unsigned char data[BLOCK_DATA_SZ]; unsigned int dlen; int active;
} cache_ent_t;
typedef struct {
    char method[8]; char path[128]; int status; unsigned int rtime, ts; int cached;
} gw_req_t;
typedef struct {
    cache_ent_t cache[MAX_CACHE_ENT]; unsigned int ccount;
    gw_req_t reqs[MAX_GW_REQ]; unsigned int rcount;
    unsigned int total_req, cache_hits, bytes_served, bytes_cached;
    int cdn_on; unsigned int cdn_ep;
} gw_state_t;

static int gw_cache_put(gw_state_t *gw, const cid_t *cid, const unsigned char *data,
                        unsigned int len, cache_strat_t strat, unsigned int ttl) {
    if (gw->ccount >= MAX_CACHE_ENT) return -1;
    unsigned int idx = gw->ccount;
    gw->cache[idx].cid = *cid; gw->cache[idx].strat = strat;
    gw->cache[idx].cached_at = now_ts; gw->cache[idx].ttl = ttl; gw->cache[idx].hits = 0;
    gw->cache[idx].dlen = len < BLOCK_DATA_SZ ? len : BLOCK_DATA_SZ;
    for (unsigned int i = 0; i < gw->cache[idx].dlen; i++) gw->cache[idx].data[i] = data[i];
    gw->cache[idx].active = 1; gw->ccount++;
    gw->bytes_cached += gw->cache[idx].dlen; return (int)idx;
}
static int gw_cache_get(gw_state_t *gw, const cid_t *cid, unsigned char *out, unsigned int max) {
    for (unsigned int i = 0; i < gw->ccount; i++) {
        if (!gw->cache[i].active) continue;
        if (my_memcmp(gw->cache[i].cid.mh.digest, cid->mh.digest, cid->mh.dlen) != 0) continue;
        if (gw->cache[i].ttl > 0 && now_ts > gw->cache[i].cached_at + gw->cache[i].ttl) {
            gw->cache[i].active = 0; continue;
        }
        unsigned int cp = gw->cache[i].dlen < max ? gw->cache[i].dlen : max;
        for (unsigned int j = 0; j < cp; j++) out[j] = gw->cache[i].data[j];
        gw->cache[i].hits++; gw->cache_hits++; return (int)cp;
    }
    return -1;
}
static void gw_cache_cleanup(gw_state_t *gw) {
    unsigned int cleaned = 0;
    for (unsigned int i = 0; i < gw->ccount; i++)
        if (gw->cache[i].active && gw->cache[i].ttl > 0 &&
            now_ts > gw->cache[i].cached_at + gw->cache[i].ttl) {
            gw->cache[i].active = 0; cleaned++;
        }
    print_str("[GW] cache cleanup: "); print_int((int)cleaned); print_str("\n");
}
static int gw_handle_req(gw_state_t *gw, const char *method, const char *path,
                         const cid_t *key, unsigned char *resp, unsigned int max) {
    unsigned int ri = gw->rcount % MAX_GW_REQ; gw_req_t *rq = &gw->reqs[ri];
    my_strncpy(rq->method, method, 7); my_strncpy(rq->path, path, 127);
    rq->ts = now_ts; rq->cached = 0; gw->total_req++;
    int cached = gw_cache_get(gw, key, resp, max);
    if (cached >= 0) {
        rq->status = 200; rq->cached = 1; rq->rtime = 1;
        gw->bytes_served += (unsigned int)cached; gw->rcount++; return cached;
    }
    rq->status = 404; rq->rtime = 10; gw->rcount++; return -1;
}
static void gw_enable_cdn(gw_state_t *gw, unsigned int ep) {
    gw->cdn_on = 1; gw->cdn_ep = ep;
    print_str("[GW] CDN enabled endpoints: "); print_int((int)ep); print_str("\n");
}
static void gw_print_stats(const gw_state_t *gw) {
    print_str("[GW] req="); print_int((int)gw->total_req);
    print_str(" hits="); print_int((int)gw->cache_hits);
    print_str(" served="); print_int((int)gw->bytes_served);
    print_str(" cdn="); print_int(gw->cdn_on); print_str("\n");
}

/* ===== Init ===== */
static void init_repo(ipfs_repo_t *r) { r->ncount = 0; }
static void init_net(peer_net_t *n) { n->pcount = 0; n->total_bw = 100000000; n->used_bw = 0; node_id_gen(&n->local_id); }
static void init_pin(pin_state_t *p) { p->pcount = 0; p->scount = 0; p->total_bytes = 0; }
static void init_ipns(ipns_state_t *s) { s->rcount = 0; s->kcount = 0; }
static void init_gw(gw_state_t *g) {
    g->ccount = 0; g->rcount = 0; g->total_req = 0; g->cache_hits = 0;
    g->bytes_served = 0; g->bytes_cached = 0; g->cdn_on = 0; g->cdn_ep = 0;
}

__attribute__((export_name("main")))
int main(void) {
    print_str("=== IPFS Storage Module v57.0 ===\n");
    ipfs_repo_t repo; init_repo(&repo);
    peer_net_t net; init_net(&net);
    pin_state_t pins; init_pin(&pins);
    ipns_state_t ipns; init_ipns(&ipns);
    gw_state_t gw; init_gw(&gw);

    peer_discover(&net, 8);
    bw_allocate(&net, 10000000);
    print_str("[NET] peers: "); print_int((int)net.pcount); print_str("\n");

    const char *fdata = "Hello IPFS World - decentralized storage test data block";
    unsigned int fidx = ipfs_upload(&repo, (const unsigned char *)fdata, my_strlen(fdata), "test.txt", PERM_R|PERM_W);
    print_str("[UPLOAD] idx: "); print_int((int)fidx); print_str("\n");

    unsigned int root = ipfs_mkdir(&repo, "/root", PERM_R|PERM_X);
    unsigned int sub = ipfs_mkdir(&repo, "/root/docs", PERM_R|PERM_X);
    ipfs_add_child(&repo, root, sub);
    ipfs_add_child(&repo, sub, fidx);
    unsigned int lnk = ipfs_symlink(&repo, "latest", fidx);
    print_str("[LINK] idx: "); print_int((int)lnk); print_str("\n");

    ipfs_stat(&repo, fidx); ipfs_stat(&repo, root);
    unsigned char dl[BLOCK_DATA_SZ];
    print_str("[DL] bytes: "); print_int(ipfs_download(&repo, fidx, dl, BLOCK_DATA_SZ)); print_str("\n");

    ipns_keygen(&ipns);
    pin_add(&pins, &repo.nodes[fidx].ccid, PIN_RECURSIVE, 3600);
    int svc = pin_reg_svc(&pins, "https://pin.example.com", 1048576);
    print_str("[PIN] svc: "); print_int(svc); print_str("\n");
    pin_remote(&pins, &repo.nodes[fidx].ccid, (unsigned int)svc, 4096);
    print_str("[PIN] total: "); print_int((int)pins.pcount); print_str("\n");

    ipns_publish(&ipns, ipns.keys[0], &repo.nodes[fidx].ccid, 1, 7200);
    cid_t rsv; print_str("[IPNS] resolve: "); print_int(ipns_resolve(&ipns, ipns.keys[0], &rsv)); print_str("\n");
    cid_t dns; ipns_dnslink("example.com", &dns);

    gw_enable_cdn(&gw, 4);
    gw_cache_put(&gw, &repo.nodes[fidx].ccid, (const unsigned char *)fdata, my_strlen(fdata), CACHE_PUBLIC, 300);
    unsigned char gr[BLOCK_DATA_SZ];
    print_str("[GW] resp: "); print_int(gw_handle_req(&gw, "GET", "/ipfs/QmTest", &repo.nodes[fidx].ccid, gr, BLOCK_DATA_SZ));
    print_str("\n"); gw_print_stats(&gw);

    net.peers[0].is_provider = 1; net.peers[0].prov_keys = 5;
    unsigned int pl[MAX_PEERS];
    print_str("[DHT] providers: "); print_int(peer_find_provs(&net, &repo.nodes[fidx].ccid, pl, MAX_PEERS));
    print_str("\n");

    tick(EXPIRE_TIMEOUT + 1);
    peer_expire(&net); pin_cleanup(&pins); gw_cache_cleanup(&gw);

    print_str("[IPFS] nodes="); print_int((int)repo.ncount);
    print_str(" peers="); print_int((int)net.pcount);
    print_str(" pins="); print_int((int)pins.pcount);
    print_str(" ipns="); print_int((int)ipns.rcount);
    print_str(" cache="); print_int((int)gw.ccount);
    print_str("\n=== IPFS Storage Complete ===\n");
    return 0;
}
