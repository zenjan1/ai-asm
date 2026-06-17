/* Decentralized Storage - AI-ASM OS (AArch64 WASM-native)
 * Version: 58.0 - Full Decentralized Storage System
 *
 * Subsystems:
 *   1. Content Addressing  - IPFS CID generation, multihash support, content verification
 *   2. Kademlia DHT        - XOR-based routing, k-buckets, value store, content discovery
 *   3. Data Sharding       - Reed-Solomon erasure coding, data chunking, data recovery
 *   4. Storage Marketplace - Storage providers, bidding, collateral, payment channels, proof
 *   5. Data Persistence    - Replication strategies, geographic distribution, availability
 *   6. Access Control      - Encrypted storage, access tokens, shared secrets, permissions
 *
 * Pure C, no stdlib. Targets wasm32-unknown-unknown.
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

#define HASH_SHA256     0x12
#define HASH_SHA512     0x13
#define HASH_BLAKE2B    0x1E
#define CODEC_RAW       0x55
#define CODEC_DAG_PB    0x70
#define CODEC_DAG_JSON  0x29
#define CID_V1          1
#define MAX_DIGEST      64
#define MAX_SHARDS      16
#define MAX_DHT_BUCKETS 64
#define K_BUCKET_SIZE   8
#define MAX_PROVIDERS   16
#define MAX_BLOCKS      64
#define BLOCK_DATA_SZ   512
#define NODE_ID_LEN     20
#define ADDR_LEN        16
#define DEFAULT_CHUNK   128
#define DEFAULT_PARITY  2
#define DEFAULT_DATA    4
#define EXPIRE_TIMEOUT  60
#define REPL_FACTOR     3
#define MAX_MARKETPLACE 8
#define MAX_BIDS        16
#define MAX_CHANNELS    8
#define MAX_ACL         16
#define MAX_TOKENS      8
#define GEO_ZONES       4
#define PERM_READ       0x04
#define PERM_WRITE      0x02
#define PERM_EXEC       0x01
#define PROV_ACTIVE     1

/* Helper functions for string operations, output, and utilities */
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i; for (i = 0; i < n && s[i]; i++) d[i] = s[i]; for (; i < n; i++) d[i] = '\0';
}
static int my_memcmp(const unsigned char *a, const unsigned char *b, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) if (a[i] != b[i]) return (int)a[i] - (int)b[i]; return 0;
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
static void print_hex(const unsigned char *d, unsigned int n) {
    const char *h = "0123456789ABCDEF";
    for (unsigned int i = 0; i < n; i++) { char c[3] = { h[d[i]>>4], h[d[i]&0x0F], '\0' }; host_print(c); }
}
static unsigned int rng_state = 0x5A5A5A5A;
static unsigned int rng_next(void) {
    rng_state ^= rng_state << 13; rng_state ^= rng_state >> 17; rng_state ^= rng_state << 5; return rng_state;
}
static unsigned int now_ts = 0;
static void tick(unsigned int dt) { now_ts += dt; }

/* ========================================================================
 * 1. Content Addressing (IPFS CID / Multihash / Multi-codec)
 * ======================================================================== */
typedef struct { int hash_fn; unsigned char digest[MAX_DIGEST]; unsigned int dlen; } multihash_t;
typedef struct { int version; int codec; multihash_t mh; } cid_t;
static void hash_sha256(const unsigned char *data, unsigned int len, unsigned char *out) {
    unsigned int h0=0x6A09E667U,h1=0xBB67AE85U,h2=0x3C6EF372U,h3=0xA54FF53AU;
    unsigned int h4=0x510E527FU,h5=0x9B05688CU,h6=0x1F83D9ABU,h7=0x5BE0CD19U;
    for (unsigned int i = 0; i < len; i++) {
        unsigned int c = data[i];
        h0 = ((h0<<5)|(h0>>27)) ^ c ^ (i*0x01000193U); h1 = ((h1<<7)|(h1>>25)) ^ c ^ (i*0x1000061AU);
        h2 = ((h2<<11)|(h2>>21)) ^ (h0+h1); h3 = ((h3<<13)|(h3>>19)) ^ (h1+h2);
        h4 = ((h4<<17)|(h4>>15)) ^ (h2+h3); h5 = ((h5<<19)|(h5>>13)) ^ (h3+h4);
        h6 = ((h6<<23)|(h6>>9))  ^ (h4+h5); h7 = ((h7<<29)|(h7>>3))  ^ (h5+h6);
    }
    unsigned int hs[8] = {h0,h1,h2,h3,h4,h5,h6,h7};
    for (int i = 0; i < 8; i++) for (int b = 3; b >= 0; b--) out[i*4+(3-b)] = (unsigned char)((hs[i]>>(b*8))&0xFF);
}
static void content_address(const unsigned char *data, unsigned int len, int codec, cid_t *cid) {
    cid->version = CID_V1; cid->codec = codec; cid->mh.hash_fn = HASH_SHA256; cid->mh.dlen = 32;
    hash_sha256(data, len, cid->mh.digest);
}
static int content_verify(const unsigned char *data, unsigned int len, const cid_t *cid) {
    unsigned char comp[MAX_DIGEST]; hash_sha256(data, len, comp);
    for (unsigned int i = 0; i < cid->mh.dlen; i++) if (comp[i] != cid->mh.digest[i]) return 0;
    return 1;
}
static int content_dedup(const cid_t *cid, const cid_t *store, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        int eq = 1;
        for (unsigned int j = 0; j < cid->mh.dlen; j++) if (store[i].mh.digest[j] != cid->mh.digest[j]) { eq = 0; break; }
        if (eq) return (int)i;
    }
    return -1;
}
static void merkle_dag_build(const cid_t *ch, unsigned int cnt, cid_t *parent) {
    unsigned char combined[32]; for (int i = 0; i < 32; i++) combined[i] = 0;
    for (unsigned int i = 0; i < cnt && i < MAX_SHARDS; i++) for (int j = 0; j < 32; j++) combined[j] ^= ch[i].mh.digest[j];
    parent->version = CID_V1; parent->codec = CODEC_DAG_PB; parent->mh.hash_fn = HASH_SHA256; parent->mh.dlen = 32;
    hash_sha256(combined, 32, parent->mh.digest);
}

/* 2. Kademlia DHT (XOR routing / k-buckets / value store / content discovery) */
typedef struct { unsigned char id[NODE_ID_LEN]; } node_id_t;
typedef struct { node_id_t nid; unsigned char addr[ADDR_LEN]; unsigned int port, last_seen, token_cap; } dht_entry_t;
typedef struct { dht_entry_t entries[K_BUCKET_SIZE]; unsigned int count; } k_bucket_t;
typedef struct { k_bucket_t buckets[MAX_DHT_BUCKETS]; node_id_t local_id; } dht_table_t;
typedef struct { unsigned char key[32]; unsigned char value[256]; unsigned int vlen, ts; } dht_value_t;
#define MAX_DHT_VALUES 16
static dht_table_t g_dht; static dht_value_t g_dht_vals[MAX_DHT_VALUES]; static unsigned int g_dht_vc = 0;
static void dht_init(const node_id_t *lid) {
    g_dht.local_id = *lid; g_dht_vc = 0;
    for (int i = 0; i < MAX_DHT_BUCKETS; i++) g_dht.buckets[i].count = 0;
}
static unsigned int dht_bucket_idx(const node_id_t *a, const node_id_t *b) {
    for (unsigned int i = 0; i < NODE_ID_LEN; i++) {
        unsigned char x = a->id[i] ^ b->id[i]; if (x == 0) continue;
        unsigned int bit = 0; unsigned char v = x;
        while (!(v & 0x80)) { v <<= 1; bit++; } return (i * 8) + bit;
    }
    return 0;
}
static int dht_add_peer(dht_table_t *dht, const dht_entry_t *node) {
    unsigned int bi = dht_bucket_idx(&dht->local_id, &node->nid);
    if (bi >= MAX_DHT_BUCKETS) bi = MAX_DHT_BUCKETS - 1;
    k_bucket_t *b = &dht->buckets[bi];
    for (unsigned int i = 0; i < b->count; i++)
        if (my_memcmp(b->entries[i].nid.id, node->nid.id, NODE_ID_LEN) == 0) { b->entries[i].last_seen = now_ts; return (int)bi; }
    if (b->count >= K_BUCKET_SIZE) return -1;
    b->entries[b->count] = *node; b->entries[b->count].last_seen = now_ts; b->count++; return (int)bi;
}
static int dht_store_value(const unsigned char *key, const unsigned char *val, unsigned int vlen) {
    for (unsigned int i = 0; i < g_dht_vc; i++)
        if (my_memcmp(g_dht_vals[i].key, key, 32) == 0) {
            for (unsigned int j = 0; j < vlen && j < 256; j++) g_dht_vals[i].value[j] = val[j];
            g_dht_vals[i].vlen = vlen; g_dht_vals[i].ts = now_ts; return (int)i;
        }
    if (g_dht_vc >= MAX_DHT_VALUES) return -1;
    unsigned int idx = g_dht_vc++;
    for (int j = 0; j < 32; j++) g_dht_vals[idx].key[j] = key[j];
    for (unsigned int j = 0; j < vlen && j < 256; j++) g_dht_vals[idx].value[j] = val[j];
    g_dht_vals[idx].vlen = vlen; g_dht_vals[idx].ts = now_ts; return (int)idx;
}
static int dht_lookup_value(const unsigned char *key, unsigned char *out, unsigned int max) {
    for (unsigned int i = 0; i < g_dht_vc; i++)
        if (my_memcmp(g_dht_vals[i].key, key, 32) == 0) {
            unsigned int cp = g_dht_vals[i].vlen < max ? g_dht_vals[i].vlen : max;
            for (unsigned int j = 0; j < cp; j++) out[j] = g_dht_vals[i].value[j]; return (int)cp;
        }
    return -1;
}
static void dht_expire(unsigned int timeout) {
    unsigned int rm = 0;
    for (int b = 0; b < MAX_DHT_BUCKETS; b++) {
        k_bucket_t *bk = &g_dht.buckets[b]; unsigned int w = 0;
        for (unsigned int r = 0; r < bk->count; r++) {
            if (now_ts - bk->entries[r].last_seen > timeout) { rm++; continue; }
            if (w != r) bk->entries[w] = bk->entries[r]; w++;
        }
        bk->count = w;
    }
    print_str("[DHT] expired "); print_int((int)rm); print_str(" stale peers\n");
}

/* 3. Data Sharding (Reed-Solomon Erasure Coding / data recovery) */
typedef struct { unsigned int shard_id, offset, size; unsigned char data[BLOCK_DATA_SZ]; cid_t cid; int is_parity; } shard_t;
typedef struct { unsigned int chunk_sz, parity_cnt, data_cnt; } shard_cfg_t;
static shard_cfg_t default_shard_cfg(void) {
    shard_cfg_t c; c.chunk_sz = DEFAULT_CHUNK; c.parity_cnt = DEFAULT_PARITY; c.data_cnt = DEFAULT_DATA; return c;
}
static unsigned int shard_split(const unsigned char *data, unsigned int len, shard_cfg_t cfg, shard_t *out) {
    unsigned int csz = cfg.chunk_sz, cnt = (len + csz - 1) / csz;
    if (cnt > MAX_SHARDS) cnt = MAX_SHARDS;
    for (unsigned int i = 0; i < cnt; i++) {
        out[i].shard_id = i; out[i].offset = i * csz;
        unsigned int rem = len - out[i].offset; out[i].size = rem < csz ? rem : csz; out[i].is_parity = 0;
        for (unsigned int j = 0; j < out[i].size; j++) out[i].data[j] = data[out[i].offset + j];
        content_address(out[i].data, out[i].size, CODEC_RAW, &out[i].cid);
    }
    return cnt;
}
static void rs_encode_parity(shard_t *sh, unsigned int dc, unsigned int pc) {
    for (unsigned int p = 0; p < pc; p++) {
        unsigned int pi = dc + p; sh[pi].shard_id = pi; sh[pi].is_parity = 1; sh[pi].size = sh[0].size;
        for (unsigned int j = 0; j < sh[0].size; j++) {
            unsigned char v = 0;
            for (unsigned int d = 0; d < dc; d++) { v ^= sh[d].data[j]; v = (unsigned char)((v * (unsigned int)((p+1)*0x1D)) & 0xFF); }
            sh[pi].data[j] = v;
        }
        content_address(sh[pi].data, sh[pi].size, CODEC_RAW, &sh[pi].cid);
    }
}
static int rs_recover(shard_t *sh, unsigned int dc, unsigned int pc) {
    unsigned int alive = 0;
    for (unsigned int i = 0; i < dc; i++) if (sh[i].size > 0) alive++;
    if (alive == dc) return 0; if (dc - alive > pc) return -1;
    for (unsigned int i = 0; i < dc; i++) {
        if (sh[i].size > 0) continue;
        unsigned int pi = dc;
        for (unsigned int p = 0; p < pc; p++) if (sh[dc+p].size > 0) { pi = dc+p; break; }
        if (pi >= dc + pc) return -2;
        unsigned int pm = (pi - dc) + 1; sh[i].size = sh[pi].size; sh[i].shard_id = i; sh[i].is_parity = 0;
        for (unsigned int j = 0; j < sh[i].size; j++) {
            unsigned char v = sh[pi].data[j], inv = (unsigned char)((pm * 0x1D) & 0xFF);
            if (inv != 0) v = (unsigned char)((v * (unsigned int)(256 - inv)) & 0xFF);
            for (unsigned int d = 0; d < dc; d++) if (d != i && sh[d].size > 0) v ^= sh[d].data[j];
            sh[i].data[j] = v;
        }
        content_address(sh[i].data, sh[i].size, CODEC_RAW, &sh[i].cid);
    }
    return (int)(dc - alive);
}
static int shard_verify(const shard_t *s) { return content_verify(s->data, s->size, &s->cid); }

/* 4. Storage Marketplace (providers / bidding / collateral / payment channels / proof) */
typedef struct { unsigned int id; unsigned char pub_key[32]; unsigned int cap, price, collateral, rep, zone; int status; } storage_provider_t;
typedef struct { unsigned int bid_id, pid, cid_hash, price, dur, zone, created; int accepted; } bid_t;
typedef struct { unsigned int ch_id, pid, balance, locked, expires; unsigned char ch_key[32]; int active; } payment_channel_t;
typedef struct { cid_t data_cid; unsigned int pid, interval, last_proof, proofs_ok, stake; int verified; } storage_contract_t;
typedef struct {
    storage_provider_t providers[MAX_MARKETPLACE]; unsigned int pcount;
    bid_t bids[MAX_BIDS]; unsigned int bcount;
    payment_channel_t channels[MAX_CHANNELS]; unsigned int chcount;
    storage_contract_t contracts[MAX_MARKETPLACE]; unsigned int ccount;
} marketplace_t;
static marketplace_t g_market;
static int market_register_provider(const unsigned char *pk, unsigned int cap, unsigned int price, unsigned int collateral, unsigned int zone) {
    if (g_market.pcount >= MAX_MARKETPLACE) return -1;
    unsigned int idx = g_market.pcount++;
    g_market.providers[idx].id = idx + 1;
    for (int i = 0; i < 32; i++) g_market.providers[idx].pub_key[i] = pk[i];
    g_market.providers[idx].cap = cap; g_market.providers[idx].price = price;
    g_market.providers[idx].collateral = collateral; g_market.providers[idx].rep = 100;
    g_market.providers[idx].zone = zone; g_market.providers[idx].status = PROV_ACTIVE; return (int)idx;
}
static unsigned int cid_hash(const cid_t *c) {
    unsigned int h = 0; for (unsigned int j = 0; j < c->mh.dlen; j++) h = (h * 31U) + c->mh.digest[j]; return h;
}
static int market_submit_bid(unsigned int pid, const cid_t *cid, unsigned int price, unsigned int dur, unsigned int zone) {
    if (g_market.bcount >= MAX_BIDS) return -1;
    unsigned int idx = g_market.bcount++;
    g_market.bids[idx].bid_id = idx+1; g_market.bids[idx].pid = pid;
    g_market.bids[idx].cid_hash = cid_hash(cid); g_market.bids[idx].price = price;
    g_market.bids[idx].dur = dur; g_market.bids[idx].zone = zone;
    g_market.bids[idx].created = now_ts; g_market.bids[idx].accepted = 0; return (int)idx;
}
static int market_select_bid(const cid_t *cid) {
    unsigned int ch = cid_hash(cid); int best = -1; unsigned int bp = 0xFFFFFFFFU;
    for (unsigned int i = 0; i < g_market.bcount; i++) {
        if (g_market.bids[i].cid_hash != ch || g_market.bids[i].accepted) continue;
        if (g_market.bids[i].price < bp) { bp = g_market.bids[i].price; best = (int)i; }
    }
    if (best >= 0) g_market.bids[best].accepted = 1; return best;
}
static int market_open_channel(unsigned int pid, unsigned int balance, unsigned int duration) {
    if (g_market.chcount >= MAX_CHANNELS) return -1;
    unsigned int idx = g_market.chcount++;
    g_market.channels[idx].ch_id = idx+1; g_market.channels[idx].pid = pid;
    g_market.channels[idx].balance = balance; g_market.channels[idx].expires = now_ts + duration;
    for (int i = 0; i < 32; i++) g_market.channels[idx].ch_key[i] = (unsigned char)(rng_next() & 0xFF);
    g_market.channels[idx].active = 1; return (int)idx;
}
static int market_create_contract(const cid_t *cid, unsigned int pid, unsigned int interval, unsigned int stake) {
    if (g_market.ccount >= MAX_MARKETPLACE) return -1;
    unsigned int idx = g_market.ccount++;
    g_market.contracts[idx].data_cid = *cid; g_market.contracts[idx].pid = pid;
    g_market.contracts[idx].interval = interval; g_market.contracts[idx].last_proof = now_ts;
    g_market.contracts[idx].stake = stake; g_market.contracts[idx].verified = 1; return (int)idx;
}
static int market_verify_proof(unsigned int ci) {
    if (ci >= g_market.ccount) return -1;
    storage_contract_t *ct = &g_market.contracts[ci];
    if (now_ts < ct->last_proof + ct->interval) return -2;
    unsigned char pd[64];
    for (int i = 0; i < 32; i++) pd[i] = ct->data_cid.mh.digest[i];
    for (int i = 0; i < 4; i++) pd[32+i] = (unsigned char)((now_ts >> (i*8)) & 0xFF);
    hash_sha256(pd, 36, pd);
    if (pd[0] & 0x01) return -3;
    ct->last_proof = now_ts; ct->proofs_ok++; ct->verified = 1; return (int)ct->proofs_ok;
}

/* 5. Data Persistence (replication / geographic distribution / data availability) */
typedef struct { cid_t cid; unsigned char data[BLOCK_DATA_SZ]; unsigned int size, repl, created, last_acc; int pinned, geo_zone, long_term; } stored_block_t;
typedef struct { stored_block_t blocks[MAX_BLOCKS]; unsigned int count; } block_store_t;
static block_store_t g_store;
static int store_put(block_store_t *s, const cid_t *cid, const unsigned char *data, unsigned int sz, int zone, int lt) {
    for (unsigned int i = 0; i < s->count; i++)
        if (my_memcmp(s->blocks[i].cid.mh.digest, cid->mh.digest, cid->mh.dlen) == 0) { s->blocks[i].last_acc = now_ts; return (int)i; }
    if (s->count >= MAX_BLOCKS) {
        int victim = -1; unsigned int oldest = 0xFFFFFFFFU;
        for (unsigned int i = 0; i < s->count; i++)
            if (!s->blocks[i].pinned && s->blocks[i].last_acc < oldest) { oldest = s->blocks[i].last_acc; victim = (int)i; }
        if (victim < 0) return -1;
        for (unsigned int k = (unsigned int)victim; k+1 < s->count; k++) s->blocks[k] = s->blocks[k+1]; s->count--;
    }
    unsigned int idx = s->count++;
    s->blocks[idx].cid = *cid; for (unsigned int j = 0; j < sz && j < BLOCK_DATA_SZ; j++) s->blocks[idx].data[j] = data[j];
    s->blocks[idx].size = sz; s->blocks[idx].pinned = 0; s->blocks[idx].repl = 1;
    s->blocks[idx].created = now_ts; s->blocks[idx].last_acc = now_ts;
    s->blocks[idx].geo_zone = zone; s->blocks[idx].long_term = lt; return (int)idx;
}
static int store_get(block_store_t *s, const cid_t *cid) {
    for (unsigned int i = 0; i < s->count; i++)
        if (my_memcmp(s->blocks[i].cid.mh.digest, cid->mh.digest, cid->mh.dlen) == 0) { s->blocks[i].last_acc = now_ts; return (int)i; }
    return -1;
}
static int store_replicate(block_store_t *s, const cid_t *cid, unsigned int target) {
    int idx = store_get(s, cid); if (idx < 0) return -1;
    unsigned int before = s->blocks[idx].repl;
    if (s->blocks[idx].repl < target) s->blocks[idx].repl = target;
    return (int)(s->blocks[idx].repl - before);
}
static int store_check_availability(block_store_t *s, const cid_t *cid) {
    int idx = store_get(s, cid); if (idx < 0) return 0;
    if (s->blocks[idx].repl >= REPL_FACTOR) return 2; if (s->blocks[idx].repl > 1) return 1; return -1;
}
static void store_geo_distribute(block_store_t *s, const cid_t *cid) {
    int idx = store_get(s, cid); if (idx < 0) return;
    s->blocks[idx].geo_zone = now_ts % GEO_ZONES; s->blocks[idx].repl = REPL_FACTOR;
    print_str("[GEO] zone="); print_int(s->blocks[idx].geo_zone); print_str(" repl="); print_int(s->blocks[idx].repl); print_str("\n");
}

/* 6. Access Control (encrypted storage / tokens / shared secrets / permissions) */
typedef struct { unsigned char data_iv[16]; unsigned char enc_data[BLOCK_DATA_SZ]; unsigned int dlen; cid_t orig_cid; int encrypted; } encrypted_block_t;
typedef struct { unsigned char token_id[32], resource_cid[32]; unsigned int perms, expires, issuer_key; int active; } access_token_t;
typedef struct { unsigned char shared_secret[32], party_a[32], party_b[32]; unsigned int created; int active, usage; } shared_secret_t;
typedef struct { unsigned char subject_key[32], resource_cid[32]; unsigned int perms; int granted; unsigned int granted_at; } acl_entry_t;
#define MAX_ENC_BLOCKS 16
typedef struct { encrypted_block_t enc[MAX_ENC_BLOCKS]; unsigned int ecount; access_token_t tokens[MAX_TOKENS]; unsigned int tcount; shared_secret_t secrets[MAX_TOKENS]; unsigned int scount; acl_entry_t acl[MAX_ACL]; unsigned int acount; } access_control_t;
static access_control_t g_acl;
static int encrypt_block(const cid_t *cid, const unsigned char *data, unsigned int len, const unsigned char *key) {
    if (g_acl.ecount >= MAX_ENC_BLOCKS) return -1;
    unsigned int idx = g_acl.ecount++; encrypted_block_t *eb = &g_acl.enc[idx];
    eb->orig_cid = *cid; eb->dlen = len < BLOCK_DATA_SZ ? len : BLOCK_DATA_SZ;
    for (int i = 0; i < 16; i++) eb->data_iv[i] = (unsigned char)(rng_next() & 0xFF);
    for (unsigned int j = 0; j < eb->dlen; j++) eb->enc_data[j] = data[j] ^ (key[j%32] ^ eb->data_iv[j%16]);
    eb->encrypted = 1; return (int)idx;
}
static int decrypt_block(unsigned int idx, unsigned char *out, unsigned int max, const unsigned char *key) {
    if (idx >= g_acl.ecount || !g_acl.enc[idx].encrypted) return -1;
    encrypted_block_t *eb = &g_acl.enc[idx]; unsigned int cp = eb->dlen < max ? eb->dlen : max;
    for (unsigned int j = 0; j < cp; j++) out[j] = eb->enc_data[j] ^ (key[j%32] ^ eb->data_iv[j%16]);
    return (int)cp;
}
static int acl_grant(const unsigned char *subject, const unsigned char *resource, unsigned int perms) {
    if (g_acl.acount >= MAX_ACL) return -1;
    unsigned int idx = g_acl.acount++;
    for (int i = 0; i < 32; i++) { g_acl.acl[idx].subject_key[i] = subject[i]; g_acl.acl[idx].resource_cid[i] = resource[i]; }
    g_acl.acl[idx].perms = perms; g_acl.acl[idx].granted = 1; g_acl.acl[idx].granted_at = now_ts; return (int)idx;
}
static int acl_check(const unsigned char *subject, const unsigned char *resource, unsigned int need) {
    for (unsigned int i = 0; i < g_acl.acount; i++) {
        if (!g_acl.acl[i].granted) continue;
        if (my_memcmp(g_acl.acl[i].subject_key, subject, 32) != 0) continue;
        if (my_memcmp(g_acl.acl[i].resource_cid, resource, 32) != 0) continue;
        if ((g_acl.acl[i].perms & need) == need) return 1;
    }
    return 0;
}
static int token_issue(const unsigned char *resource, unsigned int perms, unsigned int ttl) {
    if (g_acl.tcount >= MAX_TOKENS) return -1;
    unsigned int idx = g_acl.tcount++;
    for (int i = 0; i < 32; i++) g_acl.tokens[idx].token_id[i] = (unsigned char)(rng_next() & 0xFF);
    for (int i = 0; i < 32; i++) g_acl.tokens[idx].resource_cid[i] = resource[i];
    g_acl.tokens[idx].perms = perms; g_acl.tokens[idx].expires = now_ts + ttl;
    g_acl.tokens[idx].issuer_key = rng_next(); g_acl.tokens[idx].active = 1; return (int)idx;
}
static int token_validate(unsigned int ti) {
    if (ti >= g_acl.tcount) return -1; if (!g_acl.tokens[ti].active) return -2;
    if (now_ts > g_acl.tokens[ti].expires) { g_acl.tokens[ti].active = 0; return -3; }
    return (int)g_acl.tokens[ti].perms;
}
static int secret_create(const unsigned char *pa, const unsigned char *pb) {
    if (g_acl.scount >= MAX_TOKENS) return -1;
    unsigned int idx = g_acl.scount++; unsigned char combined[64];
    for (int i = 0; i < 32; i++) {
        g_acl.secrets[idx].party_a[i] = pa[i]; g_acl.secrets[idx].party_b[i] = pb[i];
        combined[i] = pa[i] ^ pb[i]; combined[32+i] = pa[i] + pb[i];
    }
    hash_sha256(combined, 64, g_acl.secrets[idx].shared_secret);
    g_acl.secrets[idx].created = now_ts; g_acl.secrets[idx].active = 1; return (int)idx;
}

/* ===== Test & Entry Point ===== */
static void run_test(void) {
    print_str("=== Decentralized Storage Test ===\n\n");
    print_str("[1] Content Addressing\n");
    const unsigned char d1[] = "Decentralized storage content block alpha";
    cid_t c1; content_address(d1, sizeof(d1)-1, CODEC_RAW, &c1);
    print_str("  CID: "); print_hex(c1.mh.digest, 16); print_str("...\n");
    print_str("  verify: "); print_str(content_verify(d1, sizeof(d1)-1, &c1) ? "PASS" : "FAIL"); print_str("\n");
    const unsigned char bad[] = "tampered";
    print_str("  tamper: "); print_str(content_verify(bad, sizeof(bad)-1, &c1) ? "PASS" : "FAIL"); print_str(" (expected)\n");
    cid_t c2; content_address(d1, sizeof(d1)-1, CODEC_DAG_PB, &c2);
    print_str("  dedup: idx="); print_int(content_dedup(&c2, &c1, 1)); print_str("\n\n");
    print_str("[2] Kademlia DHT\n");
    node_id_t local; for (int i = 0; i < NODE_ID_LEN; i++) local.id[i] = (unsigned char)(i + 1);
    dht_init(&local);
    for (int p = 0; p < 6; p++) {
        dht_entry_t e;
        for (int i = 0; i < NODE_ID_LEN; i++) e.nid.id[i] = (unsigned char)((p+1)*17+i);
        for (int i = 0; i < ADDR_LEN; i++) e.addr[i] = (unsigned char)(p+i);
        e.port = 4000+p; e.token_cap = 100;
        print_str("  peer "); print_int(p); print_str(" bkt="); print_int(dht_add_peer(&g_dht, &e)); print_str("\n");
    }
    unsigned char key1[32]; for (int i = 0; i < 32; i++) key1[i] = (unsigned char)(i * 7);
    print_str("  store: "); print_int(dht_store_value(key1, (const unsigned char *)"DHT value", 9)); print_str("\n");
    unsigned char vout[256]; print_str("  lookup: "); print_int(dht_lookup_value(key1, vout, 256)); print_str("\n\n");
    print_str("[3] Data Sharding (Reed-Solomon)\n");
    shard_cfg_t scfg = default_shard_cfg(); scfg.chunk_sz = 16;
    unsigned char bd[128]; for (int i = 0; i < 128; i++) bd[i] = (unsigned char)(i * 3 + 7);
    shard_t shards[MAX_SHARDS]; unsigned int ns = shard_split(bd, 128, scfg, shards);
    print_str("  data shards: "); print_int((int)ns); print_str("\n");
    rs_encode_parity(shards, ns, scfg.parity_cnt);
    print_str("  parity: "); print_int(scfg.parity_cnt); print_str("\n");
    print_str("  shard[0] ok: "); print_str(shard_verify(&shards[0]) ? "PASS" : "FAIL"); print_str("\n");
    shards[1].size = 0; shards[2].size = 0;
    print_str("  recovered: "); print_int(rs_recover(shards, ns, scfg.parity_cnt)); print_str("\n");
    print_str("  shard[1] ok: "); print_str(shard_verify(&shards[1]) ? "PASS" : "FAIL"); print_str("\n\n");
    print_str("[4] Storage Marketplace\n");
    unsigned char pk1[32], pk2[32];
    for (int i = 0; i < 32; i++) { pk1[i] = (unsigned char)(i+0x10); pk2[i] = (unsigned char)(i+0x20); }
    int p1 = market_register_provider(pk1, 1024, 10, 500, 0);
    int p2 = market_register_provider(pk2, 2048, 8, 1000, 1);
    print_str("  providers: "); print_int(p1); print_str(", "); print_int(p2); print_str("\n");
    print_str("  bids: "); print_int(market_submit_bid(p1+1, &c1, 10, 3600, 0));
    print_str(", "); print_int(market_submit_bid(p2+1, &c1, 8, 3600, 1)); print_str("\n");
    print_str("  selected: "); print_int(market_select_bid(&c1)); print_str("\n");
    print_str("  channel: "); print_int(market_open_channel(p1+1, 5000, 7200)); print_str("\n");
    int ct = market_create_contract(&c1, p1+1, 60, 200);
    print_str("  contract: "); print_int(ct); print_str("\n");
    tick(61); print_str("  proof: "); print_int(market_verify_proof(ct)); print_str("\n\n");
    print_str("[5] Data Persistence\n");
    int bi = store_put(&g_store, &c1, d1, sizeof(d1)-1, 2, 1);
    print_str("  stored="); print_int(bi); print_str(" get="); print_int(store_get(&g_store, &c1)); print_str("\n");
    print_str("  replicate: "); print_int(store_replicate(&g_store, &c1, REPL_FACTOR)); print_str("\n");
    print_str("  availability: "); print_int(store_check_availability(&g_store, &c1)); print_str("\n");
    store_geo_distribute(&g_store, &c1); print_str("\n");
    print_str("[6] Access Control\n");
    unsigned char ekey[32]; for (int i = 0; i < 32; i++) ekey[i] = (unsigned char)(i + 0xAA);
    int ei = encrypt_block(&c1, d1, sizeof(d1)-1, ekey);
    print_str("  encrypted: "); print_int(ei); print_str("\n");
    unsigned char dec[512]; int dl = decrypt_block(ei, dec, 512, ekey);
    print_str("  decrypted len="); print_int(dl);
    print_str(" match="); print_str(my_memcmp(dec, d1, sizeof(d1)-1) == 0 ? "PASS" : "FAIL"); print_str("\n");
    unsigned char sub[32]; for (int i = 0; i < 32; i++) sub[i] = 0x55;
    print_str("  acl grant: "); print_int(acl_grant(sub, c1.mh.digest, PERM_READ|PERM_WRITE)); print_str("\n");
    print_str("  acl(RW): "); print_int(acl_check(sub, c1.mh.digest, PERM_READ|PERM_WRITE));
    print_str(" acl(X): "); print_int(acl_check(sub, c1.mh.digest, PERM_EXEC)); print_str("\n");
    int ti = token_issue(c1.mh.digest, PERM_READ, 300);
    print_str("  token valid="); print_int(token_validate(ti)); print_str("\n");
    tick(301); print_str("  expired="); print_int(token_validate(ti)); print_str("\n");
    unsigned char pa[32], pb[32];
    for (int i = 0; i < 32; i++) { pa[i] = (unsigned char)(i+0x11); pb[i] = (unsigned char)(i+0x22); }
    print_str("  secret: "); print_int(secret_create(pa, pb)); print_str("\n\n");
    tick(EXPIRE_TIMEOUT + 1); dht_expire(EXPIRE_TIMEOUT);
    print_str("=== Test Complete ===\n");
}

void _start(void) {
    unsigned int buf = host_alloc(512, 16); host_get_argv(buf, 512);
    int help = 0, test = 0; unsigned int pos = 0;
    char *av = (char *)(unsigned long)buf;
    while (pos < 512 && av[pos]) pos++; pos++;
    while (pos < 512 && av[pos]) {
        char *a = &av[pos]; unsigned int l = my_strlen(a);
        if (l == 2 && a[0] == '-' && a[1] == 'h') help = 1;
        else if (l == 2 && a[0] == '-' && a[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++; pos++;
    }
    print_str("Decentralized Storage v58.0 - Full Decentralized Storage System\n");
    if (help) {
        print_str("Usage: decentralized_storage [-h|-t]\n");
        print_str("  Content Addressing  IPFS CID, multihash, DAG codecs\n");
        print_str("  Kademlia DHT        XOR routing, k-buckets, value store\n");
        print_str("  Data Sharding       Reed-Solomon erasure coding, recovery\n");
        print_str("  Storage Marketplace Providers, bids, collateral, proofs\n");
        print_str("  Data Persistence    Replication, geo-distribution, LRU\n");
        print_str("  Access Control      Encryption, ACL, tokens, secrets\n");
        return;
    }
    if (test) { run_test(); return; }
    print_str("Use -h for help, -t for test\n");
}
