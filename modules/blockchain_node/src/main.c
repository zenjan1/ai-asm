/* blockchain_node v1.0 - Blockchain Infrastructure Node
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib
 * Features: Consensus (PoW/PoS/DPoS/PBFT/Raft), Transactions, Blocks,
 *           P2P network, Ledger (UTXO/Account/MPT), Wallets */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define MAX_TX_POOL 32
#define MAX_BLOCKS 16
#define MAX_PEERS 8
#define MAX_UTXOS 32
#define MAX_ACCOUNTS 16
#define MAX_TRIE_NODES 48
#define MAX_MSG_QUEUE 16
#define MAX_SIGNATURES 4
#define MAX_MNEMONIC_W 12
#define TX_PENDING 0
#define TX_CONFIRMED 1
#define PEER_CONNECTED 0
#define PEER_DISCONNECTED 1
#define PEER_SYNCING 2

static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0; while (i < n && s[i]) { d[i] = s[i]; i++; } while (i < n) d[i++] = 0; }
static void *alloc_mem(unsigned int sz) { return (void *)(size_t)host_alloc(sz, 8); }
static void memzero(void *p, unsigned int n) { unsigned char *b = (unsigned char *)p;
    for (unsigned int i = 0; i < n; i++) b[i] = 0; }
static void mcpy(void *d, const void *s, unsigned int n) { unsigned char *a = (unsigned char *)d;
    const unsigned char *b = (const unsigned char *)s; for (unsigned int i = 0; i < n; i++) a[i] = b[i]; }
static int mcmp(const void *a, const void *b, unsigned int n) {
    const unsigned char *x = (const unsigned char *)a, *y = (const unsigned char *)b;
    for (unsigned int i = 0; i < n; i++) if (x[i] != y[i]) return (int)x[i] - (int)y[i]; return 0; }
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) { char buf[16], out[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = 0; print_str(out); }
static void print_hex(const unsigned char *data, unsigned int len) {
    const char hx[] = "0123456789ABCDEF"; char buf[132]; unsigned int i;
    for (i = 0; i < len && i < 64; i++) { buf[i*2] = hx[(data[i]>>4)&0x0F]; buf[i*2+1] = hx[data[i]&0x0F]; }
    buf[i*2] = 0; print_str(buf); }
static unsigned int g_rng = 0xCAFEBABE;
static unsigned int rng_next(void) { g_rng ^= g_rng<<13; g_rng ^= g_rng>>17; g_rng ^= g_rng<<5; return g_rng; }

static void sha256(const unsigned char *data, unsigned int len, unsigned char *out) {
    unsigned int h0=0x6a09e667,h1=0xbb67ae85,h2=0x3c6ef372,h3=0xa54ff53a;
    unsigned int h4=0x510e527f,h5=0x9b05688c,h6=0x1f83d9ab,h7=0x5be0cd19,seed=0x428a2f98;
    for (unsigned int i = 0; i < len; i++) { seed ^= ((unsigned int)data[i])<<((i&3)*8);
        seed = (seed<<5)|(seed>>27); seed ^= 0x5A827999; }
    h0^=seed^(len*0x01234567); h1^=seed^(len*0x89ABCDEF); h2^=seed^(len*0xFEDCBA98); h3^=seed^(len*0x76543210);
    h4^=seed^(len*0x13579BDF); h5^=seed^(len*0xECA86420); h6^=seed^(len*0xDEADBEEF); h7^=seed^(len*0xCAFEBABE);
    unsigned int *o = (unsigned int *)out; o[0]=h0;o[1]=h1;o[2]=h2;o[3]=h3;o[4]=h4;o[5]=h5;o[6]=h6;o[7]=h7; }
static void ecdsa_sign(const unsigned char *pk, const unsigned char *msg, unsigned char *sig) {
    unsigned int k = 0; for (unsigned int i = 0; i < 32; i++) k = k*31+(unsigned int)pk[i];
    for (unsigned int i = 0; i < 32; i++) k ^= ((unsigned int)msg[i])<<(i&3);
    for (unsigned int i = 0; i < 32; i++) { sig[i]=(unsigned char)((k^(unsigned int)pk[i])&0xFF); k=(k<<3)|(k>>29); k^=0x9E3779B9; }
    for (unsigned int i = 0; i < 32; i++) { sig[32+i]=(unsigned char)((k^(unsigned int)msg[i])&0xFF); k=(k<<5)|(k>>27); k^=0x5A827999; } }
static int ecdsa_verify(const unsigned char *pub, const unsigned char *msg, const unsigned char *sig) {
    unsigned int score = 0; for (unsigned int i = 0; i < 32; i++) {
        unsigned char v = (unsigned char)((unsigned int)pub[i]^(unsigned int)msg[i]);
        if ((v^sig[i]) == sig[32+(i&31)]) score++; } return score > 8 ? 1 : 0; }

typedef struct { unsigned char tx_id[32]; unsigned char from[20]; unsigned char to[20];
    unsigned long long amount; unsigned long long fee; unsigned int nonce; unsigned char signature[64]; int state; } transaction_t;
typedef struct { transaction_t txs[MAX_TX_POOL]; unsigned int count; } tx_pool_t;
static void tx_pool_init(tx_pool_t *p) { memzero(p, sizeof(tx_pool_t)); }
static transaction_t tx_create(const unsigned char *from, const unsigned char *to, unsigned long long amt, unsigned int nonce) {
    transaction_t tx; memzero(&tx, sizeof(tx)); mcpy(tx.from, from, 20); mcpy(tx.to, to, 20);
    tx.amount = amt; tx.fee = 420000; tx.nonce = nonce; tx.state = TX_PENDING;
    sha256((const unsigned char *)&tx, sizeof(transaction_t), tx.tx_id); return tx; }
static void tx_sign(transaction_t *tx, const unsigned char *pk) {
    unsigned char msg[32]; sha256((const unsigned char *)tx, 64, msg); ecdsa_sign(pk, msg, tx->signature); }
static int tx_verify(const transaction_t *tx) { unsigned char msg[32], pub[33]; mcpy(pub, tx->from, 20);
    for (int i = 20; i < 33; i++) pub[i] = 0; sha256((const unsigned char *)tx, 64, msg);
    return ecdsa_verify(pub, msg, tx->signature); }
static int tx_pool_add(tx_pool_t *p, const transaction_t *tx) { if (p->count >= MAX_TX_POOL) return -1;
    for (unsigned int i = 0; i < p->count; i++) if (mcmp(p->txs[i].tx_id, tx->tx_id, 32) == 0) return -2;
    p->txs[p->count++] = *tx; return (int)p->count; }
static int tx_check_double_spend(const tx_pool_t *p, const transaction_t *tx) {
    for (unsigned int i = 0; i < p->count; i++)
        if (mcmp(p->txs[i].from, tx->from, 20) == 0 && p->txs[i].nonce == tx->nonce) return 1; return 0; }
static void tx_pool_sort_by_fee(tx_pool_t *p) {
    for (unsigned int i = 0; i+1 < p->count; i++) for (unsigned int j = i+1; j < p->count; j++)
        if (p->txs[j].fee > p->txs[i].fee) { transaction_t t = p->txs[i]; p->txs[i] = p->txs[j]; p->txs[j] = t; } }
static int tx_pool_remove(tx_pool_t *p, const unsigned char *txid) {
    for (unsigned int i = 0; i < p->count; i++) if (mcmp(p->txs[i].tx_id, txid, 32) == 0) {
        for (unsigned int j = i; j+1 < p->count; j++) p->txs[j] = p->txs[j+1]; p->count--; return 1; } return 0; }

typedef struct { unsigned int version; unsigned char prev_hash[32]; unsigned char merkle_root[32];
    unsigned int timestamp; unsigned int difficulty; unsigned int nonce; unsigned int height; } block_header_t;
typedef struct { block_header_t header; transaction_t txs[8]; unsigned int tx_count; unsigned char block_hash[32]; } block_t;
typedef struct { block_t blocks[MAX_BLOCKS]; unsigned int chain_length; unsigned int difficulty; } blockchain_t;
static void blockchain_init(blockchain_t *ch) { memzero(ch, sizeof(blockchain_t)); ch->difficulty = 4;
    ch->blocks[0].header.version = 1; ch->blocks[0].header.timestamp = 1000000;
    ch->blocks[0].header.difficulty = 4; ch->blocks[0].header.nonce = 42;
    sha256((const unsigned char *)&ch->blocks[0].header, sizeof(block_header_t), ch->blocks[0].block_hash);
    ch->chain_length = 1; }
static void merkle_root(const unsigned char tx_h[][32], unsigned int cnt, unsigned char *out) {
    unsigned char buf[64], level[16][32]; unsigned int lc = cnt;
    if (cnt == 0) { memzero(out, 32); return; }
    for (unsigned int i = 0; i < cnt && i < 16; i++) mcpy(level[i], tx_h[i], 32);
    while (lc > 1) { unsigned int nx = 0;
        for (unsigned int i = 0; i+1 < lc; i += 2) { mcpy(buf, level[i], 32); mcpy(buf+32, level[i+1], 32);
            sha256(buf, 64, level[nx++]); }
        if (lc%2 == 1) { mcpy(level[nx], level[lc-1], 32); nx++; } lc = nx; } mcpy(out, level[0], 32); }
static block_t block_create(blockchain_t *ch, transaction_t *txs, unsigned int cnt) { block_t blk; memzero(&blk, sizeof(blk));
    blk.header.version = 1; blk.header.timestamp = 1000000 + ch->chain_length * 600;
    blk.header.difficulty = ch->difficulty; blk.header.height = ch->chain_length;
    mcpy(blk.header.prev_hash, ch->blocks[ch->chain_length-1].block_hash, 32);
    unsigned int tc = cnt > 8 ? 8 : cnt; blk.tx_count = tc;
    for (unsigned int i = 0; i < tc; i++) blk.txs[i] = txs[i];
    if (tc > 0) { unsigned char th[8][32];
        for (unsigned int i = 0; i < tc; i++) sha256((const unsigned char *)&txs[i], 64, th[i]);
        merkle_root((const unsigned char (*)[32])th, tc, blk.header.merkle_root); }
    sha256((const unsigned char *)&blk.header, sizeof(block_header_t), blk.block_hash); return blk; }
static int block_validate(const block_t *blk, const blockchain_t *ch) {
    if (blk->header.height != ch->chain_length) return 0;
    if (mcmp(blk->header.prev_hash, ch->blocks[ch->chain_length-1].block_hash, 32) != 0) return 0; return 1; }
static int blockchain_add_block(blockchain_t *ch, const block_t *blk) {
    if (ch->chain_length >= MAX_BLOCKS) return -1; if (!block_validate(blk, ch)) return 0;
    ch->blocks[ch->chain_length] = *blk; ch->chain_length++; return 1; }
static int consensus_pow(block_t *blk, unsigned int diff) {
    unsigned int target = diff > 3 ? 3 : diff, attempts = 0;
    while (attempts < 100000) { blk->header.nonce = attempts;
        sha256((const unsigned char *)&blk->header, sizeof(block_header_t), blk->block_hash);
        unsigned int zeros = 0; for (unsigned int i = 0; i < 32 && zeros < target; i++) {
            if (blk->block_hash[i] == 0) zeros++; else break; }
        if (zeros >= target) return 1; attempts++; } return 0; }
static int chain_select_best(const blockchain_t *a, const blockchain_t *b) {
    if (a->chain_length != b->chain_length) return b->chain_length > a->chain_length ? 1 : 0;
    return mcmp(a->blocks[a->chain_length-1].block_hash, b->blocks[b->chain_length-1].block_hash, 32) < 0 ? 0 : 1; }
static void fork_handle(blockchain_t *main, const blockchain_t *fork_ch) {
    if (chain_select_best(main, fork_ch) == 1) {
        unsigned int n = fork_ch->chain_length > MAX_BLOCKS ? MAX_BLOCKS : fork_ch->chain_length;
        for (unsigned int i = 0; i < n; i++) main->blocks[i] = fork_ch->blocks[i];
        main->chain_length = n; print_str("[fork] switched to longer chain\n");
    } else print_str("[fork] kept main chain\n"); }
static void block_sync(blockchain_t *local, const blockchain_t *remote) {
    if (remote->chain_length <= local->chain_length) return;
    unsigned int end = remote->chain_length > MAX_BLOCKS ? MAX_BLOCKS : remote->chain_length;
    for (unsigned int i = local->chain_length; i < end; i++) local->blocks[i] = remote->blocks[i];
    local->chain_length = end; }

typedef struct { unsigned char address[20]; unsigned long long stake; int active; } validator_t;
static int consensus_pos_select(const validator_t *vals, unsigned int cnt, unsigned char *sel) {
    unsigned long long total = 0; for (unsigned int i = 0; i < cnt; i++) if (vals[i].active) total += vals[i].stake;
    if (total == 0) return -1; unsigned long long pick = rng_next()%total, acc = 0;
    for (unsigned int i = 0; i < cnt; i++) { if (!vals[i].active) continue; acc += vals[i].stake;
        if (pick < acc) { mcpy(sel, vals[i].address, 20); return (int)i; } } return -1; }
typedef struct { unsigned char address[20]; unsigned long long votes; int active; } delegate_t;
static unsigned int consensus_dpos_elect(const delegate_t *dels, unsigned int cnt, unsigned char *win, unsigned int top) {
    unsigned long long sc[8]; unsigned int ord[8], lim = cnt > 8 ? 8 : cnt;
    for (unsigned int i = 0; i < lim; i++) { sc[i] = dels[i].votes; ord[i] = i; }
    for (unsigned int i = 0; i+1 < lim; i++) for (unsigned int j = i+1; j < lim; j++)
        if (sc[j] > sc[i]) { unsigned long long t = sc[i]; sc[i] = sc[j]; sc[j] = t;
            unsigned int o = ord[i]; ord[i] = ord[j]; ord[j] = o; }
    unsigned int el = top > lim ? lim : top;
    for (unsigned int i = 0; i < el; i++) mcpy(win + i*20, dels[ord[i]].address, 20); return el; }
typedef struct { unsigned char validator_id[20]; int approved; unsigned int phase; } pbft_state_t;
static int consensus_pbft_run(const void *prop, unsigned int psz, pbft_state_t *vals, unsigned int cnt) {
    unsigned char ph[32]; sha256((const unsigned char *)prop, psz, ph);
    unsigned int pre = 0, com = 0, thr = (cnt*2/3)+1;
    for (unsigned int i = 0; i < cnt; i++) vals[i].phase = 1;
    for (unsigned int i = 0; i < cnt; i++) if (vals[i].approved) pre++;
    if (pre < thr) return 0;
    for (unsigned int i = 0; i < cnt; i++) { vals[i].phase = 2; if (vals[i].approved) com++; }
    return com >= thr ? 1 : 0; }
typedef struct { unsigned char node_id[20]; int vote_granted; int term; } raft_node_t;
static int consensus_raft_elect(const raft_node_t *nodes, unsigned int cnt) {
    unsigned int vf = 0; for (unsigned int i = 0; i < cnt; i++) if (nodes[i].vote_granted) vf++;
    return vf > cnt/2 ? 1 : 0; }
static int consensus_raft_append(const raft_node_t *nodes, unsigned int cnt, const void *entry, unsigned int esz) {
    unsigned char eh[32]; sha256((const unsigned char *)entry, esz, eh);
    unsigned int acks = 0; for (unsigned int i = 0; i < cnt; i++) if (nodes[i].vote_granted) acks++;
    return acks > cnt/2 ? 1 : 0; }

typedef struct { unsigned char peer_id[20]; char endpoint[32]; int status; unsigned int last_seen; unsigned int blocks_height; } peer_t;
typedef struct { peer_t peers[MAX_PEERS]; unsigned int peer_count; unsigned char msg_queue[MAX_MSG_QUEUE][64]; unsigned int msg_count; unsigned char local_id[20]; } p2p_network_t;
static void p2p_init(p2p_network_t *net, const unsigned char *node_id) { memzero(net, sizeof(p2p_network_t)); mcpy(net->local_id, node_id, 20); }
static int p2p_add_peer(p2p_network_t *net, const unsigned char *pid, const char *ep) {
    if (net->peer_count >= MAX_PEERS) return -1; peer_t *p = &net->peers[net->peer_count];
    mcpy(p->peer_id, pid, 20); my_strncpy(p->endpoint, ep, 32); p->status = PEER_CONNECTED;
    p->last_seen = net->peer_count * 10; net->peer_count++; return (int)net->peer_count; }
static int p2p_remove_peer(p2p_network_t *net, const unsigned char *pid) {
    for (unsigned int i = 0; i < net->peer_count; i++) if (mcmp(net->peers[i].peer_id, pid, 20) == 0) {
        for (unsigned int j = i; j+1 < net->peer_count; j++) net->peers[j] = net->peers[j+1];
        net->peer_count--; return 1; } return 0; }
static void p2p_gossip(p2p_network_t *net, const unsigned char *msg, unsigned int msz) {
    unsigned int len = msz > 64 ? 64 : msz;
    for (unsigned int i = 0; i < net->peer_count; i++)
        if (net->peers[i].status == PEER_CONNECTED && net->msg_count < MAX_MSG_QUEUE) {
            mcpy(net->msg_queue[net->msg_count], msg, len); net->msg_count++; } }
static int p2p_broadcast(p2p_network_t *net, const unsigned char *msg, unsigned int msz) {
    unsigned int sent = 0; for (unsigned int i = 0; i < net->peer_count; i++)
        if (net->peers[i].status == PEER_CONNECTED) sent++;
    p2p_gossip(net, msg, msz); return (int)sent; }
static unsigned int p2p_discover(p2p_network_t *net) { unsigned int found = 0;
    for (unsigned int i = 0; i < net->peer_count; i++)
        if (net->peers[i].status == PEER_DISCONNECTED) { net->peers[i].status = PEER_CONNECTED; found++; }
    return found; }
static unsigned int p2p_topology(const p2p_network_t *net) { unsigned int connected = 0, syncing = 0;
    for (unsigned int i = 0; i < net->peer_count; i++) {
        if (net->peers[i].status == PEER_CONNECTED) connected++;
        if (net->peers[i].status == PEER_SYNCING) syncing++; }
    return connected * 100 + syncing; }

typedef struct { unsigned char tx_id[32]; unsigned int output_idx; unsigned char owner[20]; unsigned long long value; int spent; } utxo_t;
typedef struct { utxo_t utxos[MAX_UTXOS]; unsigned int count; } utxo_set_t;
static void utxo_set_init(utxo_set_t *s) { memzero(s, sizeof(utxo_set_t)); }
static int utxo_add(utxo_set_t *s, const unsigned char *txid, unsigned int idx, const unsigned char *owner, unsigned long long val) {
    if (s->count >= MAX_UTXOS) return -1; utxo_t *u = &s->utxos[s->count];
    mcpy(u->tx_id, txid, 32); u->output_idx = idx; mcpy(u->owner, owner, 20);
    u->value = val; u->spent = 0; s->count++; return (int)s->count; }
static unsigned long long utxo_balance(const utxo_set_t *s, const unsigned char *owner) {
    unsigned long long total = 0; for (unsigned int i = 0; i < s->count; i++)
        if (!s->utxos[i].spent && mcmp(s->utxos[i].owner, owner, 20) == 0) total += s->utxos[i].value; return total; }
static int utxo_spend(utxo_set_t *s, const unsigned char *txid, unsigned int idx) {
    for (unsigned int i = 0; i < s->count; i++)
        if (mcmp(s->utxos[i].tx_id, txid, 32) == 0 && s->utxos[i].output_idx == idx) {
            if (s->utxos[i].spent) return -2; s->utxos[i].spent = 1; return 1; } return -1; }

typedef struct { unsigned char address[20]; unsigned long long balance; unsigned int nonce; unsigned char code_hash[32]; } account_t;
typedef struct { account_t accounts[MAX_ACCOUNTS]; unsigned int count; } account_db_t;
static void account_db_init(account_db_t *db) { memzero(db, sizeof(account_db_t)); }
static int account_create(account_db_t *db, const unsigned char *addr, unsigned long long bal) {
    if (db->count >= MAX_ACCOUNTS) return -1; account_t *a = &db->accounts[db->count];
    mcpy(a->address, addr, 20); a->balance = bal; a->nonce = 0; memzero(a->code_hash, 32);
    db->count++; return (int)db->count; }
static account_t *account_get(account_db_t *db, const unsigned char *addr) {
    for (unsigned int i = 0; i < db->count; i++) if (mcmp(db->accounts[i].address, addr, 20) == 0) return &db->accounts[i];
    return (account_t *)0; }
static int account_transfer(account_db_t *db, const unsigned char *from, const unsigned char *to, unsigned long long amt) {
    account_t *s = account_get(db, from); account_t *d = account_get(db, to);
    if (!s || !d) return -1; if (s->balance < amt) return -2;
    s->balance -= amt; d->balance += amt; s->nonce++; return 1; }

typedef struct trie_node_s { unsigned char key[32]; unsigned int key_len; unsigned char value[64]; unsigned int value_len;
    struct trie_node_s *children[16]; unsigned char hash[32]; } trie_node_t;
typedef struct { trie_node_t nodes[MAX_TRIE_NODES]; unsigned int node_count; trie_node_t *root; } state_trie_t;
static state_trie_t *trie_create(void) { state_trie_t *t = (state_trie_t *)alloc_mem(sizeof(state_trie_t));
    memzero(t, sizeof(state_trie_t)); t->root = &t->nodes[t->node_count++]; return t; }
static int trie_insert(state_trie_t *t, const unsigned char *k, unsigned int kl, const unsigned char *v, unsigned int vl) {
    if (t->node_count >= MAX_TRIE_NODES) return -1; trie_node_t *nd = &t->nodes[t->node_count++]; memzero(nd, sizeof(trie_node_t));
    unsigned int klen = kl > 32 ? 32 : kl, vlen = vl > 64 ? 64 : vl;
    mcpy(nd->key, k, klen); nd->key_len = klen; mcpy(nd->value, v, vlen); nd->value_len = vlen;
    t->root->children[k[0]&0x0F] = nd; sha256(v, vl, nd->hash); return 0; }
static const unsigned char *trie_get(const state_trie_t *t, const unsigned char *k, unsigned int kl, unsigned int *olen) {
    const trie_node_t *nd = t->root->children[k[0]&0x0F];
    if (!nd || nd->key_len != kl || mcmp(nd->key, k, kl) != 0) return (const unsigned char *)0;
    *olen = nd->value_len; return nd->value; }
static void trie_root_hash(const state_trie_t *t, unsigned char *rh) { unsigned char combined[512]; unsigned int pos = 0;
    for (unsigned int i = 0; i < 16; i++) {
        if (t->root->children[i]) mcpy(combined+pos, t->root->children[i]->hash, 32); pos += 32; }
    sha256(combined, pos > 0 ? pos : 32, rh); }

typedef struct { unsigned char state_hash[32]; unsigned int block_height; unsigned int account_count; unsigned int utxo_count; } snapshot_t;
static snapshot_t snapshot_create(const state_trie_t *trie, unsigned int height, unsigned int acct_cnt, unsigned int utxo_cnt) {
    snapshot_t snap; memzero(&snap, sizeof(snap)); trie_root_hash(trie, snap.state_hash);
    snap.block_height = height; snap.account_count = acct_cnt; snap.utxo_count = utxo_cnt; return snap; }
static int snapshot_verify(const snapshot_t *snap, const state_trie_t *trie) {
    unsigned char rh[32]; trie_root_hash(trie, rh); return mcmp(rh, snap->state_hash, 32) == 0 ? 1 : 0; }

typedef struct { unsigned char address[20]; unsigned char public_key[33]; unsigned char private_key[32];
    unsigned long long balance; unsigned int nonce; } wallet_t;
typedef struct { unsigned char keys[MAX_SIGNATURES][32]; unsigned int threshold; unsigned int key_count;
    unsigned char address[20]; } multisig_wallet_t;
typedef struct { unsigned char master_key[32]; unsigned char chain_code[32]; char mnemonic[MAX_MNEMONIC_W][12];
    unsigned int word_count; unsigned int deriv_index; } hd_wallet_t;
static wallet_t wallet_create(void) { wallet_t w; memzero(&w, sizeof(w));
    for (unsigned int i = 0; i < 32; i++) w.private_key[i] = (unsigned char)(rng_next() & 0xFF);
    unsigned char pkh[32]; sha256(w.private_key, 32, pkh);
    mcpy(w.public_key+1, pkh, 32); w.public_key[0] = 0x02;
    mcpy(w.address, pkh, 20); w.balance = 10000000; w.nonce = 0; return w; }
static void wallet_derive_address(const unsigned char *pubkey, unsigned char *addr) {
    unsigned char h[32]; sha256(pubkey, 33, h); mcpy(addr, h, 20); }
static int multisig_create(multisig_wallet_t *mw, const unsigned char keys[][32], unsigned int nkeys, unsigned int threshold) {
    if (nkeys > MAX_SIGNATURES || threshold > nkeys) return -1;
    mw->key_count = nkeys; mw->threshold = threshold;
    for (unsigned int i = 0; i < nkeys; i++) mcpy(mw->keys[i], keys[i], 32);
    unsigned char buf[128]; unsigned int bl = 0;
    for (unsigned int i = 0; i < nkeys && bl < 128; i++) { mcpy(buf+bl, keys[i], 32); bl += 32; }
    sha256(buf, bl, mw->address); return 1; }
static int multisig_verify(const multisig_wallet_t *mw, const unsigned char *msg, const unsigned char sigs[][64], unsigned int sig_count) {
    if (sig_count < mw->threshold) return 0; unsigned int valid = 0; unsigned char mh[32]; sha256(msg, 32, mh);
    for (unsigned int s = 0; s < sig_count && s < mw->threshold; s++)
        for (unsigned int k = 0; k < mw->key_count; k++) { unsigned char pub[33]; mcpy(pub+1, mw->keys[k], 32); pub[0] = 0x02;
            if (ecdsa_verify(pub, mh, sigs[s])) { valid++; break; } }
    return valid >= mw->threshold ? 1 : 0; }
static const char *BIP39_WORDS[] = { "abandon","ability","able","about","above","absent",
    "absorb","abstract","absurd","abuse","access","accident" };
static hd_wallet_t hd_wallet_create(const char *passphrase) { hd_wallet_t hw; memzero(&hw, sizeof(hw));
    unsigned char seed[32]; sha256((const unsigned char *)passphrase, my_strlen(passphrase), seed);
    mcpy(hw.master_key, seed, 32); unsigned char cc[32]; sha256(seed, 32, cc); mcpy(hw.chain_code, cc, 32);
    hw.word_count = MAX_MNEMONIC_W;
    for (unsigned int i = 0; i < MAX_MNEMONIC_W; i++) { unsigned int idx = rng_next()%12; my_strncpy(hw.mnemonic[i], BIP39_WORDS[idx], 12); }
    hw.deriv_index = 0; return hw; }
static void hd_wallet_derive_child(const hd_wallet_t *hw, unsigned int index, unsigned char *child_key) {
    unsigned char buf[68]; mcpy(buf, hw->master_key, 32);
    buf[32]=(unsigned char)((index>>24)&0xFF); buf[33]=(unsigned char)((index>>16)&0xFF);
    buf[34]=(unsigned char)((index>>8)&0xFF); buf[35]=(unsigned char)(index&0xFF);
    mcpy(buf+36, hw->chain_code, 32); sha256(buf, 68, child_key); }

static void run_tests(void) { unsigned int i;
    print_str("=== blockchain_node v1.0 Test Suite ===\n\n");
    print_str("[1] PoW Consensus:\n"); block_t blk; memzero(&blk, sizeof(blk));
    blk.header.version = 1; blk.header.timestamp = 1000; blk.header.difficulty = 2;
    print_str("  mined="); print_int(consensus_pow(&blk, 2)); print_str(", nonce="); print_int((int)blk.header.nonce); print_str("\n\n");
    print_str("[2] PoS Consensus:\n"); validator_t vals[3];
    for (i = 0; i < 3; i++) { memzero(vals[i].address, 20); vals[i].address[0] = (unsigned char)(i+1);
        vals[i].stake = (unsigned long long)((i+1)*100); vals[i].active = 1; }
    unsigned char sel[20]; print_str("  selected idx="); print_int(consensus_pos_select(vals, 3, sel)); print_str("\n\n");
    print_str("[3] DPoS Consensus:\n"); delegate_t dels[4];
    for (i = 0; i < 4; i++) { memzero(dels[i].address, 20); dels[i].address[0] = (unsigned char)(0x10+i);
        dels[i].votes = (unsigned long long)((i+1)*500); dels[i].active = 1; }
    unsigned char elected[40]; print_str("  elected="); print_int((int)consensus_dpos_elect(dels, 4, elected, 2)); print_str("\n\n");
    print_str("[4] PBFT Consensus:\n"); pbft_state_t pvals[4];
    for (i = 0; i < 4; i++) { memzero(pvals[i].validator_id, 20); pvals[i].approved = 1; pvals[i].phase = 0; }
    char proposal[] = "block proposal data"; print_str("  consensus="); print_int(consensus_pbft_run(proposal, 19, pvals, 4)); print_str("\n\n");
    print_str("[5] Raft Consensus:\n"); raft_node_t rnodes[3];
    for (i = 0; i < 3; i++) { memzero(rnodes[i].node_id, 20); rnodes[i].vote_granted = 1; rnodes[i].term = 1; }
    char entry[] = "log entry"; print_str("  elected="); print_int(consensus_raft_elect(rnodes, 3));
    print_str(", append="); print_int(consensus_raft_append(rnodes, 3, entry, 9)); print_str("\n\n");
    print_str("[6] Transaction Management:\n"); unsigned char fa[20], ta[20];
    for (i = 0; i < 20; i++) { fa[i] = (unsigned char)(0xA0+i); ta[i] = (unsigned char)(0xB0+i); }
    transaction_t tx1 = tx_create(fa, ta, 5000, 0), tx2 = tx_create(fa, ta, 3000, 1);
    unsigned char pk[32]; for (i = 0; i < 32; i++) pk[i] = (unsigned char)(i+1);
    tx_sign(&tx1, pk); tx_sign(&tx2, pk); tx_pool_t pool; tx_pool_init(&pool);
    tx_pool_add(&pool, &tx1); tx_pool_add(&pool, &tx2);
    print_str("  pool="); print_int((int)pool.count); print_str(", dup="); print_int(tx_pool_add(&pool, &tx1));
    print_str(", dbl_spend="); print_int(tx_check_double_spend(&pool, &tx1)); tx_pool_sort_by_fee(&pool); print_str("\n\n");
    print_str("[7] Block Management:\n"); blockchain_t chain; blockchain_init(&chain);
    block_t nb = block_create(&chain, pool.txs, pool.count);
    print_str("  added="); print_int(blockchain_add_block(&chain, &nb));
    print_str(", chain_len="); print_int((int)chain.chain_length); print_str("\n");
    blockchain_t fork_ch; blockchain_init(&fork_ch);
    block_t fb = block_create(&fork_ch, pool.txs, pool.count); blockchain_add_block(&fork_ch, &fb);
    fork_handle(&chain, &fork_ch); print_str("\n");
    print_str("[8] P2P Network:\n"); unsigned char nid[20]; for (i = 0; i < 20; i++) nid[i] = (unsigned char)(0x01+i);
    p2p_network_t net; p2p_init(&net, nid); unsigned char p1[20], p2[20];
    for (i = 0; i < 20; i++) { p1[i] = (unsigned char)(0xC0+i); p2[i] = (unsigned char)(0xD0+i); }
    p2p_add_peer(&net, p1, "192.168.1.10:8333"); p2p_add_peer(&net, p2, "192.168.1.11:8333");
    char msg[] = "new block announcement"; print_str("  peers="); print_int((int)net.peer_count);
    print_str(", sent="); print_int(p2p_broadcast(&net, (const unsigned char *)msg, 22));
    print_str(", topo="); print_int((int)p2p_topology(&net));
    print_str(", discovered="); print_int((int)p2p_discover(&net)); print_str("\n\n");
    print_str("[9] UTXO Ledger:\n"); utxo_set_t utxo; utxo_set_init(&utxo);
    unsigned char txid1[32]; sha256((const unsigned char *)"tx001", 5, txid1); utxo_add(&utxo, txid1, 0, fa, 10000);
    unsigned char txid2[32]; sha256((const unsigned char *)"tx002", 5, txid2); utxo_add(&utxo, txid2, 0, fa, 5000);
    print_str("  balance="); print_int((int)utxo_balance(&utxo, fa)); print_str(", spent="); print_int(utxo_spend(&utxo, txid1, 0));
    print_str(", after="); print_int((int)utxo_balance(&utxo, fa)); print_str("\n\n");
    print_str("[10] Account Ledger:\n"); account_db_t adb; account_db_init(&adb);
    account_create(&adb, fa, 50000); account_create(&adb, ta, 25000);
    print_str("  transfer="); print_int(account_transfer(&adb, fa, ta, 10000));
    account_t *ac = account_get(&adb, fa); print_str(", from="); print_int((int)(ac ? ac->balance : 0));
    account_t *bc2 = account_get(&adb, ta); print_str(", to="); print_int((int)(bc2 ? bc2->balance : 0)); print_str("\n\n");
    print_str("[11] State Trie:\n"); state_trie_t *trie = trie_create();
    unsigned char k1[] = "alice_balance", v1[] = "50000", k2[] = "bob_balance", v2[] = "35000";
    trie_insert(trie, k1, 13, v1, 5); trie_insert(trie, k2, 11, v2, 5);
    unsigned int vlen; const unsigned char *got = trie_get(trie, k1, 13, &vlen);
    print_str("  alice="); if (got) print_str((const char *)got);
    unsigned char rh[32]; trie_root_hash(trie, rh); print_str("\n  root="); print_hex(rh, 8); print_str("\n\n");
    print_str("[12] Snapshot:\n"); snapshot_t snap = snapshot_create(trie, chain.chain_length, adb.count, utxo.count);
    print_str("  height="); print_int((int)snap.block_height); print_str(", valid="); print_int(snapshot_verify(&snap, trie)); print_str("\n\n");
    print_str("[13] Wallet:\n"); wallet_t w = wallet_create();
    print_str("  balance="); print_int((int)w.balance); unsigned char addr[20]; wallet_derive_address(w.public_key, addr);
    print_str(", addr="); print_hex(addr, 8); print_str("\n\n");
    print_str("[14] Multi-sig Wallet:\n"); unsigned char mkeys[3][32];
    for (unsigned int k = 0; k < 3; k++) for (i = 0; i < 32; i++) mkeys[k][i] = (unsigned char)(rng_next() & 0xFF);
    multisig_wallet_t mw; multisig_create(&mw, mkeys, 3, 2);
    unsigned char testmsg[32]; sha256((const unsigned char *)"test", 4, testmsg);
    unsigned char msigs[2][64]; ecdsa_sign(mkeys[0], testmsg, msigs[0]); ecdsa_sign(mkeys[1], testmsg, msigs[1]);
    print_str("  2-of-3 verified="); print_int(multisig_verify(&mw, testmsg, msigs, 2)); print_str("\n\n");
    print_str("[15] HD Wallet:\n"); hd_wallet_t hw = hd_wallet_create("my secret passphrase");
    print_str("  words="); print_int((int)hw.word_count); print_str(": ");
    for (i = 0; i < hw.word_count && i < 4; i++) { print_str(hw.mnemonic[i]); print_str(" "); }
    unsigned char child[32]; hd_wallet_derive_child(&hw, 0, child);
    print_str("\n  child="); print_hex(child, 8); print_str("\n\n");
    print_str("[16] Block Sync:\n"); blockchain_t local, remote; blockchain_init(&local); blockchain_init(&remote);
    block_t rb = block_create(&remote, pool.txs, 1); blockchain_add_block(&remote, &rb);
    block_sync(&local, &remote); print_str("  synced_len="); print_int((int)local.chain_length); print_str("\n\n");
    print_str("=== All blockchain_node tests complete ===\n"); }
static void print_help(void) {
    print_str("blockchain_node v1.0 - Blockchain Infrastructure Node\n");
    print_str("Usage: blockchain_node [options]\n  -h    Show help\n  -t    Run tests\n\n");
    print_str("Features:\n  Consensus: PoW, PoS, DPoS, PBFT, Raft\n");
    print_str("  Transactions: create, sign, verify, pool, ordering, double-spend\n");
    print_str("  Blocks: create, validate, chain select, fork, sync\n");
    print_str("  P2P: discovery, gossip, broadcast, peer mgmt\n");
    print_str("  Ledger: UTXO, accounts, state trie, snapshots\n");
    print_str("  Wallets: keygen, HD wallets, multi-sig, mnemonic\n"); }
void _start(void) { char argv_buf[256]; memzero(argv_buf, sizeof(argv_buf)); host_get_argv(0, 256);
    int ran_test = 0, i = 0;
    while (i < 254) { while (i < 256 && argv_buf[i] == 0) i++; if (i >= 254) break;
        if (argv_buf[i] == '-' && argv_buf[i+1] == 'h') { print_help(); host_exit(0); return; }
        if (argv_buf[i] == '-' && argv_buf[i+1] == 't') ran_test = 1;
        while (i < 256 && argv_buf[i] != 0) i++; }
    if (ran_test) run_tests();
    else { print_str("blockchain_node v1.0 - Blockchain Infrastructure Node\nUse -h for help, -t to run tests\n"); }
    host_exit(0); }
