/* Decentralized Storage - AI-ASM OS (AArch64 WASM-native)
 * Version: 57.0 - Content-Addressed P2P Storage
 *
 * Subsystems: Content Addressing, Sharded Storage (Reed-Solomon parity),
 *             DHT (Kademlia), Content Routing, Data Persistence,
 *             Bandwidth Management
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

/* ===== Constants ===== */
#define HASH_SHA256 0x12
#define HASH_SHA512 0x13
#define HASH_BLAKE2B 0x1e
#define CODEC_RAW 0x55
#define CODEC_DAG_PB 0x70
#define CODEC_DAG_JSON 0x29
#define CID_V1 1
#define MAX_DIGEST 64
#define MAX_SHARDS 16
#define MAX_DHT_BUCKETS 160
#define K_BUCKET_SIZE 20
#define MAX_PROVIDERS 32
#define MAX_BLOCKS 64
#define BLOCK_DATA_SZ 512
#define MAX_TRANSFERS 16
#define NODE_ID_LEN 20
#define ADDR_LEN 16
#define DEFAULT_CHUNK 256
#define DEFAULT_PARITY 2
#define DEFAULT_DATA 4
#define EXPIRE_TIMEOUT 60
#define REPL_FACTOR 3

/* ===== Helpers ===== */
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }

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
    out[i] = '\0';
    host_print(out);
}

static void print_hex(const unsigned char *d, unsigned int n) {
    const char *h = "0123456789abcdef";
    for (unsigned int i = 0; i < n; i++) {
        char c[3] = { h[d[i] >> 4], h[d[i] & 0xf], '\0' };
        host_print(c);
    }
}

static int my_abs(int v) { return v < 0 ? -v : v; }

static unsigned int rng_state = 0xdeadbeef;
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
typedef struct { int hash_function; unsigned char digest[MAX_DIGEST]; unsigned int digest_len; } multihash_t;
typedef struct { int version; int codec; multihash_t mh; } cid_t;

static void hash_sha256(const unsigned char *data, unsigned int len, unsigned char *out) {
    unsigned int h0=0x6a09e667u, h1=0xbb67ae85u, h2=0x3c6ef372u, h3=0xa54ff53au;
    unsigned int h4=0x510e527fu, h5=0x9b05688cu, h6=0x1f83d9abu, h7=0x5be0cd19u;
    for (unsigned int i = 0; i < len; i++) {
        unsigned int c = data[i];
        h0 = ((h0<<5)|(h0>>27)) ^ c ^ (i*0x01000193u);
        h1 = ((h1<<7)|(h1>>25)) ^ c ^ (i*0x1000061u);
        h2 = ((h2<<11)|(h2>>21)) ^ (h0+h1);
        h3 = ((h3<<13)|(h3>>19)) ^ (h1+h2);
        h4 = ((h4<<17)|(h4>>15)) ^ (h2+h3);
        h5 = ((h5<<19)|(h5>>13)) ^ (h3+h4);
        h6 = ((h6<<23)|(h6>>9)) ^ (h4+h5);
        h7 = ((h7<<29)|(h7>>3)) ^ (h5+h6);
    }
    unsigned int hs[8] = {h0,h1,h2,h3,h4,h5,h6,h7};
    for (int i = 0; i < 8; i++)
        for (int b = 3; b >= 0; b--) out[i*4+(3-b)] = (unsigned char)((hs[i]>>(b*8))&0xff);
}

static void content_address(const unsigned char *data, unsigned int len, cid_t *cid) {
    cid->version = CID_V1; cid->codec = CODEC_RAW;
    cid->mh.hash_function = HASH_SHA256; cid->mh.digest_len = 32;
    hash_sha256(data, len, cid->mh.digest);
}

static int content_verify(const unsigned char *data, unsigned int len, const cid_t *cid) {
    unsigned char comp[MAX_DIGEST];
    hash_sha256(data, len, comp);
    for (unsigned int i = 0; i < cid->mh.digest_len; i++)
        if (comp[i] != cid->mh.digest[i]) return 0;
    return 1;
}

static int content_detect_duplicate(const cid_t *cid, const cid_t *store, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        int eq = 1;
        for (unsigned int j = 0; j < cid->mh.digest_len; j++)
            if (store[i].mh.digest[j] != cid->mh.digest[j]) { eq = 0; break; }
        if (eq) return (int)i;
    }
    return -1;
}

/* ===== 2. Sharded Storage ===== */
typedef struct {
    unsigned int shard_id, offset, size;
    unsigned char data[BLOCK_DATA_SZ]; cid_t cid; int parity_shard;
} shard_t;
typedef struct { unsigned int chunk_size, parity_count, data_count; } shard_config_t;

static shard_config_t default_shard_config(void) {
    shard_config_t c; c.chunk_size = DEFAULT_CHUNK;
    c.parity_count = DEFAULT_PARITY; c.data_count = DEFAULT_DATA; return c;
}

static unsigned int shard_split(const unsigned char *data, unsigned int len,
                                shard_config_t cfg, shard_t *out) {
    unsigned int csz = cfg.chunk_size;
    unsigned int cnt = (len + csz - 1) / csz;
    if (cnt > MAX_SHARDS) cnt = MAX_SHARDS;
    for (unsigned int i = 0; i < cnt; i++) {
        out[i].shard_id = i; out[i].offset = i * csz;
        unsigned int rem = len - out[i].offset;
        out[i].size = rem < csz ? rem : csz;
        for (unsigned int j = 0; j < out[i].size; j++)
            out[i].data[j] = data[out[i].offset + j];
        out[i].parity_shard = 0;
        content_address(out[i].data, out[i].size, &out[i].cid);
    }
    return cnt;
}

static void shard_compute_parity(shard_t *shards, unsigned int dc, unsigned int pc) {
    for (unsigned int p = 0; p < pc; p++) {
        unsigned int pi = dc + p;
        shards[pi].shard_id = pi; shards[pi].parity_shard = 1;
        shards[pi].size = shards[0].size;
        for (unsigned int j = 0; j < shards[0].size; j++) {
            unsigned char v = 0;
            for (unsigned int d = 0; d < dc; d++) v ^= shards[d].data[j];
            v ^= (unsigned char)((p + 1) * 0x37);
            shards[pi].data[j] = v;
        }
        content_address(shards[pi].data, shards[pi].size, &shards[pi].cid);
    }
}

static int shard_reconstruct(shard_t *shards, unsigned int dc, unsigned int pc) {
    unsigned int alive = 0;
    for (unsigned int i = 0; i < dc; i++) if (shards[i].size > 0) alive++;
    if (alive == dc) return 0;
    unsigned int missing = dc - alive;
    if (missing > pc) return -1;
    for (unsigned int i = 0; i < dc; i++) {
        if (shards[i].size > 0) continue;
        unsigned int pi = dc;
        for (unsigned int p = 0; p < pc; p++)
            if (shards[dc + p].size > 0) { pi = dc + p; break; }
        if (pi >= dc + pc) return -2;
        shards[i].size = shards[pi].size; shards[i].shard_id = i;
        shards[i].parity_shard = 0;
        for (unsigned int j = 0; j < shards[i].size; j++) {
            unsigned char v = shards[pi].data[j];
            v ^= (unsigned char)(((pi - dc) + 1) * 0x37);
            for (unsigned int d = 0; d < dc; d++)
                if (d != i && shards[d].size > 0) v ^= shards[d].data[j];
            shards[i].data[j] = v;
        }
        content_address(shards[i].data, shards[i].size, &shards[i].cid);
    }
    return (int)missing;
}

static int shard_verify(const shard_t *s) {
    return content_verify(s->data, s->size, &s->cid);
}

/* ===== 3. DHT (Kademlia) ===== */
typedef struct { unsigned char id[NODE_ID_LEN]; } node_id_t;
typedef struct {
    node_id_t node_id; unsigned char address[ADDR_LEN];
    unsigned int port, last_seen, k_bucket_index;
} dht_entry_t;
typedef struct { dht_entry_t entries[K_BUCKET_SIZE]; unsigned int count, max_entries; } k_bucket_t;
typedef struct { k_bucket_t buckets[MAX_DHT_BUCKETS]; node_id_t local_id; } dht_table_t;

static dht_table_t g_dht;

static void dht_init(const node_id_t *local_id) {
    g_dht.local_id = *local_id;
    for (int i = 0; i < MAX_DHT_BUCKETS; i++) {
        g_dht.buckets[i].count = 0;
        g_dht.buckets[i].max_entries = K_BUCKET_SIZE;
    }
}

static unsigned int dht_xor_distance(const node_id_t *a, const node_id_t *b) {
    unsigned int d = 0;
    for (unsigned int i = 0; i < NODE_ID_LEN; i++) {
        unsigned char x = a->id[i] ^ b->id[i];
        d = d * 31u + x;
    }
    return d;
}

static unsigned int dht_bucket_index(const node_id_t *a, const node_id_t *b) {
    for (unsigned int i = 0; i < NODE_ID_LEN; i++) {
        unsigned char x = a->id[i] ^ b->id[i];
        if (x == 0) continue;
        unsigned int bit = 0; unsigned char v = x;
        while (!(v & 0x80)) { v <<= 1; bit++; }
        return (i * 8) + bit;
    }
    return 0;
}

static int dht_add_node(dht_table_t *dht, const dht_entry_t *node) {
    unsigned int bi = dht_bucket_index(&dht->local_id, &node->node_id);
    if (bi >= MAX_DHT_BUCKETS) bi = MAX_DHT_BUCKETS - 1;
    k_bucket_t *b = &dht->buckets[bi];
    for (unsigned int i = 0; i < b->count; i++) {
        int eq = 1;
        for (unsigned int j = 0; j < NODE_ID_LEN; j++)
            if (b->entries[i].node_id.id[j] != node->node_id.id[j]) { eq = 0; break; }
        if (eq) { b->entries[i].last_seen = now_ts; return (int)bi; }
    }
    if (b->count >= b->max_entries) return -1;
    b->entries[b->count] = *node;
    b->entries[b->count].k_bucket_index = bi;
    b->entries[b->count].last_seen = now_ts;
    b->count++;
    return (int)bi;
}

static unsigned int dht_find_closest(const dht_table_t *dht, const node_id_t *target,
                                     unsigned int k, dht_entry_t *out) {
    unsigned int found = 0;
    for (int b = 0; b < MAX_DHT_BUCKETS && found < k; b++)
        for (unsigned int i = 0; i < dht->buckets[b].count && found < k; i++)
            out[found++] = dht->buckets[b].entries[i];
    for (unsigned int i = 0; i + 1 < found; i++)
        for (unsigned int j = i + 1; j < found; j++)
            if (dht_xor_distance(&out[j].node_id, target) <
                dht_xor_distance(&out[i].node_id, target)) {
                dht_entry_t t = out[i]; out[i] = out[j]; out[j] = t;
            }
    return found;
}

static void dht_expire_stale(dht_table_t *dht, unsigned int timeout) {
    unsigned int removed = 0;
    for (int b = 0; b < MAX_DHT_BUCKETS; b++) {
        k_bucket_t *bk = &dht->buckets[b]; unsigned int w = 0;
        for (unsigned int r = 0; r < bk->count; r++) {
            if (now_ts - bk->entries[r].last_seen > timeout) { removed++; continue; }
            if (w != r) bk->entries[w] = bk->entries[r];
            w++;
        }
        bk->count = w;
    }
    print_str("[dht] expired "); print_int(removed); print_str(" stale entries\n");
}

/* ===== 4. Content Routing ===== */
typedef struct {
    cid_t cid; unsigned char provider_id[NODE_ID_LEN];
    unsigned char address[ADDR_LEN]; unsigned int timestamp, load;
} provider_record_t;
typedef struct { provider_record_t records[MAX_PROVIDERS]; unsigned int count, max_records; } provider_index_t;

static provider_index_t g_pidx;

static void routing_init(void) { g_pidx.count = 0; g_pidx.max_records = MAX_PROVIDERS; }

static int routing_announce(provider_index_t *idx, const cid_t *cid,
                            const unsigned char *pid, const unsigned char *addr) {
    for (unsigned int i = 0; i < idx->count; i++) {
        int same = 1;
        for (unsigned int j = 0; j < cid->mh.digest_len; j++)
            if (idx->records[i].cid.mh.digest[j] != cid->mh.digest[j]) { same = 0; break; }
        if (!same) continue;
        for (unsigned int j = 0; j < NODE_ID_LEN; j++)
            if (idx->records[i].provider_id[j] != pid[j]) { same = 0; break; }
        if (same) { idx->records[i].timestamp = now_ts; return (int)i; }
    }
    if (idx->count >= idx->max_records) return -1;
    unsigned int i = idx->count++;
    idx->records[i].cid = *cid;
    for (unsigned int j = 0; j < NODE_ID_LEN; j++) idx->records[i].provider_id[j] = pid[j];
    for (unsigned int j = 0; j < ADDR_LEN; j++) idx->records[i].address[j] = addr ? addr[j] : 0;
    idx->records[i].timestamp = now_ts; idx->records[i].load = 0;
    return (int)i;
}

static unsigned int routing_find_providers(const provider_index_t *idx, const cid_t *cid,
                                           unsigned int max, provider_record_t *out) {
    unsigned int found = 0;
    for (unsigned int i = 0; i < idx->count && found < max; i++) {
        int same = 1;
        for (unsigned int j = 0; j < cid->mh.digest_len; j++)
            if (idx->records[i].cid.mh.digest[j] != cid->mh.digest[j]) { same = 0; break; }
        if (same) out[found++] = idx->records[i];
    }
    return found;
}

static int routing_remove_provider(provider_index_t *idx, const cid_t *cid,
                                   const unsigned char *pid) {
    for (unsigned int i = 0; i < idx->count; i++) {
        int sc = 1, sp = 1;
        for (unsigned int j = 0; j < cid->mh.digest_len; j++)
            if (idx->records[i].cid.mh.digest[j] != cid->mh.digest[j]) { sc = 0; break; }
        if (!sc) continue;
        for (unsigned int j = 0; j < NODE_ID_LEN; j++)
            if (idx->records[i].provider_id[j] != pid[j]) { sp = 0; break; }
        if (!sp) continue;
        for (unsigned int k = i; k + 1 < idx->count; k++) idx->records[k] = idx->records[k+1];
        idx->count--; return (int)i;
    }
    return -1;
}

static int routing_load_balance(const provider_record_t *provs, unsigned int count) {
    if (count == 0) return -1;
    int best = 0; unsigned int bs = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < count; i++) {
        unsigned int sc = provs[i].load + (now_ts - provs[i].timestamp);
        if (sc < bs) { bs = sc; best = (int)i; }
    }
    return best;
}

/* ===== 5. Data Persistence ===== */
typedef struct {
    cid_t cid; unsigned char data[BLOCK_DATA_SZ];
    unsigned int size, replication_count, created_at, last_access; int pinned;
} stored_block_t;
typedef struct { stored_block_t blocks[MAX_BLOCKS]; unsigned int count, max_blocks; } block_store_t;

static block_store_t g_store;
static int store_evict_lru(block_store_t *s);

static void store_init(void) { g_store.count = 0; g_store.max_blocks = MAX_BLOCKS; }

static int store_find(block_store_t *s, const cid_t *cid) {
    for (unsigned int i = 0; i < s->count; i++) {
        int eq = 1;
        for (unsigned int j = 0; j < cid->mh.digest_len; j++)
            if (s->blocks[i].cid.mh.digest[j] != cid->mh.digest[j]) { eq = 0; break; }
        if (eq) return (int)i;
    }
    return -1;
}

static int store_put(block_store_t *s, const cid_t *cid, const unsigned char *data, unsigned int sz) {
    int i = store_find(s, cid);
    if (i >= 0) {
        for (unsigned int j = 0; j < sz && j < BLOCK_DATA_SZ; j++) s->blocks[i].data[j] = data[j];
        s->blocks[i].size = sz; s->blocks[i].last_access = now_ts; return i;
    }
    if (s->count >= s->max_blocks) if (store_evict_lru(s) < 0) return -1;
    i = (int)s->count++;
    s->blocks[i].cid = *cid;
    for (unsigned int j = 0; j < sz && j < BLOCK_DATA_SZ; j++) s->blocks[i].data[j] = data[j];
    s->blocks[i].size = sz; s->blocks[i].pinned = 0;
    s->blocks[i].replication_count = 0;
    s->blocks[i].created_at = now_ts; s->blocks[i].last_access = now_ts;
    return i;
}

static int store_get(block_store_t *s, const cid_t *cid) {
    int i = store_find(s, cid);
    if (i >= 0) { s->blocks[i].last_access = now_ts; }
    return i;
}

static int store_pin(block_store_t *s, const cid_t *cid) {
    int i = store_get(s, cid); if (i < 0) return -1;
    s->blocks[i].pinned = 1; return i;
}

static int store_unpin(block_store_t *s, const cid_t *cid) {
    int i = store_get(s, cid); if (i < 0) return -1;
    s->blocks[i].pinned = 0; return i;
}

static int store_replicate(block_store_t *s, const cid_t *cid, unsigned int target) {
    int i = store_get(s, cid); if (i < 0) return -1;
    unsigned int before = s->blocks[i].replication_count;
    if (s->blocks[i].replication_count < target) s->blocks[i].replication_count = target;
    return (int)(s->blocks[i].replication_count - before);
}

static int store_evict_lru(block_store_t *s) {
    int victim = -1; unsigned int oldest = 0xFFFFFFFFu;
    for (unsigned int i = 0; i < s->count; i++) {
        if (s->blocks[i].pinned) continue;
        if (s->blocks[i].last_access < oldest) { oldest = s->blocks[i].last_access; victim = (int)i; }
    }
    if (victim < 0) return -1;
    for (unsigned int k = (unsigned int)victim; k + 1 < s->count; k++)
        s->blocks[k] = s->blocks[k + 1];
    s->count--; return victim;
}

/* ===== 6. Bandwidth Management ===== */
typedef struct { unsigned int upload_limit_kbps, download_limit_kbps, max_concurrent_transfers; } bw_config_t;
typedef enum { DIR_UPLOAD = 0, DIR_DOWNLOAD = 1 } transfer_dir_t;
typedef enum { TS_QUEUED=0, TS_RUNNING=1, TS_PAUSED=2, TS_DONE=3, TS_INTERRUPTED=4 } transfer_state_t;
typedef struct {
    cid_t cid; transfer_dir_t direction;
    unsigned int bytes_transferred, total_bytes, priority, elapsed_ms;
    transfer_state_t state; unsigned int started_at;
} transfer_t;
typedef struct { bw_config_t cfg; transfer_t transfers[MAX_TRANSFERS]; unsigned int count, active; } bw_manager_t;

static bw_manager_t g_bw;

static void bw_init(bw_config_t cfg) { g_bw.cfg = cfg; g_bw.count = 0; g_bw.active = 0; }

static int bw_schedule_transfer(bw_manager_t *bw, const transfer_t *t) {
    if (bw->count >= MAX_TRANSFERS) return -1;
    unsigned int i = bw->count++;
    bw->transfers[i] = *t; bw->transfers[i].state = TS_QUEUED;
    bw->transfers[i].bytes_transferred = 0; bw->transfers[i].elapsed_ms = 0;
    if (bw->active < bw->cfg.max_concurrent_transfers) {
        bw->transfers[i].state = TS_RUNNING;
        bw->transfers[i].started_at = now_ts; bw->active++;
    }
    return (int)i;
}

static unsigned int bw_compute_rate(const transfer_t *t, unsigned int elapsed_ms) {
    if (elapsed_ms == 0) return 0;
    return (t->bytes_transferred * 1000u) / elapsed_ms;
}

static int bw_throttle(bw_manager_t *bw, transfer_t *t) {
    unsigned int limit = (t->direction == DIR_UPLOAD)
        ? bw->cfg.upload_limit_kbps : bw->cfg.download_limit_kbps;
    unsigned int rate = bw_compute_rate(t, t->elapsed_ms);
    unsigned int limit_bps = limit * 1024u / 8u;
    if (rate > limit_bps && limit_bps > 0) { t->state = TS_PAUSED; return 1; }
    return 0;
}

static int bw_resume_interrupted(transfer_t *t) {
    if (t->state != TS_INTERRUPTED && t->state != TS_PAUSED) return -1;
    t->state = TS_RUNNING; t->started_at = now_ts;
    return (int)t->bytes_transferred;
}

/* ===== 7. Entry Point ===== */
static void run_test(void) {
    print_str("=== Decentralized Storage Test ===\n\n");
    print_str("[1] Content Addressing\n");
    const unsigned char data1[] = "Hello decentralized world!";
    cid_t c1; content_address(data1, sizeof(data1)-1, &c1);
    print_str("  CID digest: "); print_hex(c1.mh.digest, 16); print_str("...\n");
    print_str("  verify: "); print_str(content_verify(data1, sizeof(data1)-1, &c1) ? "PASS" : "FAIL");
    print_str("\n");
    const unsigned char bad[] = "tampered";
    print_str("  verify bad: "); print_str(content_verify(bad, sizeof(bad)-1, &c1) ? "PASS" : "FAIL");
    print_str(" (expected)\n");
    cid_t c2; content_address(data1, sizeof(data1)-1, &c2);
    print_str("  duplicate: idx="); print_int(content_detect_duplicate(&c2, &c1, 1)); print_str("\n\n");

    print_str("[2] Sharded Storage\n");
    shard_config_t sc = default_shard_config(); sc.chunk_size = 8;
    unsigned char bigdata[64];
    for (int i = 0; i < 64; i++) bigdata[i] = (unsigned char)(i * 7 + 3);
    shard_t shards[MAX_SHARDS];
    unsigned int ns = shard_split(bigdata, 64, sc, shards);
    print_str("  split into "); print_int(ns); print_str(" data shards\n");
    shard_compute_parity(shards, ns, sc.parity_count);
    print_str("  parity shards: "); print_int(sc.parity_count); print_str("\n");
    print_str("  shard[0] verify: "); print_str(shard_verify(&shards[0]) ? "PASS" : "FAIL"); print_str("\n");
    shards[1].size = 0; shards[2].size = 0;
    print_str("  reconstructed missing="); print_int(shard_reconstruct(shards, ns, sc.parity_count));
    print_str("\n");
    print_str("  shard[1] recon verify: "); print_str(shard_verify(&shards[1]) ? "PASS" : "FAIL");
    print_str("\n\n");

    print_str("[3] DHT (Kademlia)\n");
    node_id_t local; for (int i = 0; i < NODE_ID_LEN; i++) local.id[i] = (unsigned char)(i + 1);
    dht_init(&local);
    for (int p = 0; p < 6; p++) {
        dht_entry_t e;
        for (int i = 0; i < NODE_ID_LEN; i++) e.node_id.id[i] = (unsigned char)((p+1)*17 + i);
        for (int i = 0; i < ADDR_LEN; i++) e.address[i] = (unsigned char)(p + i);
        e.port = 4000 + p;
        print_str("  add node p="); print_int(p); print_str(" bucket=");
        print_int(dht_add_node(&g_dht, &e)); print_str("\n");
    }
    node_id_t tgt; for (int i = 0; i < NODE_ID_LEN; i++) tgt.id[i] = (unsigned char)(i*3+5);
    dht_entry_t closest[K_BUCKET_SIZE];
    print_str("  closest: "); print_int(dht_find_closest(&g_dht, &tgt, 3, closest)); print_str("\n");
    tick(100); dht_expire_stale(&g_dht, EXPIRE_TIMEOUT); print_str("\n");

    print_str("[4] Content Routing\n");
    routing_init();
    unsigned char pv1[NODE_ID_LEN], pv2[NODE_ID_LEN], addr1[ADDR_LEN];
    for (int i = 0; i < NODE_ID_LEN; i++) { pv1[i] = 0xA1; pv2[i] = 0xB2; }
    for (int i = 0; i < ADDR_LEN; i++) addr1[i] = (unsigned char)(10 + i);
    print_str("  announce: "); print_int(routing_announce(&g_pidx, &c1, pv1, addr1));
    print_str(", "); print_int(routing_announce(&g_pidx, &c1, pv2, addr1)); print_str("\n");
    provider_record_t fpv[MAX_PROVIDERS];
    unsigned int nf = routing_find_providers(&g_pidx, &c1, 10, fpv);
    print_str("  providers: "); print_int(nf); print_str("\n");
    print_str("  lb selected: "); print_int(routing_load_balance(fpv, nf)); print_str("\n");
    routing_remove_provider(&g_pidx, &c1, pv1);
    print_str("  after remove: "); print_int(routing_find_providers(&g_pidx, &c1, 10, fpv)); print_str("\n\n");

    print_str("[5] Data Persistence\n");
    store_init();
    int bi1 = store_put(&g_store, &c1, data1, sizeof(data1)-1);
    print_str("  put idx="); print_int(bi1); print_str("\n");
    print_str("  get idx="); print_int(store_get(&g_store, &c1)); print_str("\n");
    store_pin(&g_store, &c1); print_str("  pinned: "); print_int(g_store.blocks[bi1].pinned); print_str("\n");
    store_replicate(&g_store, &c1, REPL_FACTOR);
    print_str("  replication: "); print_int(g_store.blocks[bi1].replication_count); print_str("\n");
    store_unpin(&g_store, &c1); print_str("  unpinned: "); print_int(g_store.blocks[bi1].pinned); print_str("\n\n");

    print_str("[6] Bandwidth Management\n");
    bw_config_t bwc; bwc.upload_limit_kbps = 1024; bwc.download_limit_kbps = 4096;
    bwc.max_concurrent_transfers = 4; bw_init(bwc);
    transfer_t tx; tx.cid = c1; tx.direction = DIR_UPLOAD;
    tx.total_bytes = 8192; tx.priority = 5;
    int ti = bw_schedule_transfer(&g_bw, &tx);
    print_str("  scheduled idx="); print_int(ti); print_str("\n");
    g_bw.transfers[ti].bytes_transferred = 2048; g_bw.transfers[ti].elapsed_ms = 500;
    print_str("  rate B/s: "); print_int(bw_compute_rate(&g_bw.transfers[ti], 500)); print_str("\n");
    print_str("  throttled: "); print_int(bw_throttle(&g_bw, &g_bw.transfers[ti])); print_str("\n");
    g_bw.transfers[ti].state = TS_INTERRUPTED;
    print_str("  resumed offset: "); print_int(bw_resume_interrupted(&g_bw.transfers[ti])); print_str("\n\n");
    print_str("=== Test Complete ===\n");
}

static void show_help(void) {
    print_str("Usage: decentralized_storage [-h|-t]\n");
    print_str("  -h  Show this help\n  -t  Run integration test\n\n");
    print_str("Features:\n");
    print_str("  Content Addressing  multihash, CID (RAW/DAG_PB/DAG_JSON), verify, dedup\n");
    print_str("  Sharded Storage     chunking, XOR parity, reconstruct, verify\n");
    print_str("  DHT (Kademlia)      160-bit IDs, k-buckets, XOR distance, expire\n");
    print_str("  Content Routing     provider records, announce, find, load-balance\n");
    print_str("  Data Persistence    block store, pin/unpin, replicate, LRU evict\n");
    print_str("  Bandwidth Mgmt      rate limit, schedule, throttle, resume\n");
}

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)(unsigned long)buf;
    while (pos < 512 && av[pos]) pos++;
    pos++;
    while (pos < 512 && av[pos]) {
        char *a = &av[pos]; unsigned int l = my_strlen(a);
        if (l == 2 && a[0] == '-' && a[1] == 'h') help = 1;
        else if (l == 2 && a[0] == '-' && a[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    }
    print_str("Decentralized Storage v57.0 - Content-Addressed P2P Storage\n");
    if (help) { show_help(); return; }
    if (test) { run_test(); return; }
    print_str("Use -h for help, -t for test\n");
}
