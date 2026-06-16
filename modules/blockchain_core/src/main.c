/*
 * Blockchain Core v57.0 - Decentralized Ledger Engine
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib
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

/* ====================== Helpers ====================== */

static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }

static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i = 0;
    while (i < n && s[i]) { d[i] = s[i]; i++; }
    while (i < n) d[i++] = 0;
}

static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char buf[16], out[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-v); } else uv = (unsigned int)v;
    if (uv == 0) buf[i++] = '0';
    while (uv > 0) { buf[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) buf[i++] = '-';
    int j = 0; while (i > 0) out[j++] = buf[--i]; out[j] = 0; print_str(out);
}

static void print_hex(const unsigned char *data, unsigned int len) {
    const char hx[] = "0123456789abcdef"; char buf[132]; unsigned int i;
    for (i = 0; i < len && i < 64; i++) {
        buf[i*2] = hx[(data[i] >> 4) & 0x0F]; buf[i*2+1] = hx[data[i] & 0x0F];
    }
    buf[i*2] = 0; print_str(buf);
}

static int my_abs(int x) { return x < 0 ? -x : x; }

static unsigned int g_rng = 0xDEADBEEF;
static unsigned int rng_next(void) {
    g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5;
    return g_rng;
}

static void *alloc_mem(unsigned int sz) { return (void *)(size_t)host_alloc(sz, 8); }

static void memzero(void *p, unsigned int n) {
    unsigned char *b = (unsigned char *)p;
    for (unsigned int i = 0; i < n; i++) b[i] = 0;
}

static void mcpy(void *d, const void *s, unsigned int n) {
    unsigned char *a = (unsigned char *)d; const unsigned char *b = (const unsigned char *)s;
    for (unsigned int i = 0; i < n; i++) a[i] = b[i];
}

static int mcmp(const void *a, const void *b, unsigned int n) {
    const unsigned char *x = (const unsigned char *)a, *y = (const unsigned char *)b;
    for (unsigned int i = 0; i < n; i++) if (x[i] != y[i]) return (int)x[i] - (int)y[i];
    return 0;
}

/* ====================== Hashing & Crypto ====================== */

static void sha256(const unsigned char *data, unsigned int len, unsigned char *out) {
    unsigned int h0=0x6a09e667, h1=0xbb67ae85, h2=0x3c6ef372, h3=0xa54ff53a;
    unsigned int h4=0x510e527f, h5=0x9b05688c, h6=0x1f83d9ab, h7=0x5be0cd19, seed = 0x428a2f98;
    for (unsigned int i = 0; i < len; i++) {
        seed ^= ((unsigned int)data[i]) << ((i & 3) * 8);
        seed = (seed << 5) | (seed >> 27); seed ^= 0x5a827999;
    }
    h0 ^= seed ^ (len * 0x01234567); h1 ^= seed ^ (len * 0x89abcdef);
    h2 ^= seed ^ (len * 0xfedcba98); h3 ^= seed ^ (len * 0x76543210);
    h4 ^= seed ^ (len * 0x13579bdf); h5 ^= seed ^ (len * 0xeca86420);
    h6 ^= seed ^ (len * 0xdeadbeef); h7 ^= seed ^ (len * 0xcafebabe);
    unsigned int *o = (unsigned int *)out;
    o[0]=h0; o[1]=h1; o[2]=h2; o[3]=h3; o[4]=h4; o[5]=h5; o[6]=h6; o[7]=h7;
}

static void hash_block_header(const void *hdr, unsigned int sz, unsigned char *out) {
    sha256((const unsigned char *)hdr, sz, out);
}

static void merkle_root(const unsigned char tx_h[][32], unsigned int cnt, unsigned char *out) {
    unsigned char buf[64], level[32][32]; unsigned int lc = cnt, i;
    if (cnt == 0) { memzero(out, 32); return; }
    for (i = 0; i < cnt && i < 32; i++) mcpy(level[i], tx_h[i], 32);
    while (lc > 1) {
        unsigned int nx = 0;
        for (i = 0; i + 1 < lc; i += 2) {
            mcpy(buf, level[i], 32); mcpy(buf + 32, level[i+1], 32);
            sha256(buf, 64, level[nx++]);
        }
        if (lc % 2 == 1) { mcpy(level[nx], level[lc-1], 32); nx++; }
        lc = nx;
    }
    mcpy(out, level[0], 32);
}

static void ecdsa_sign(const unsigned char *pk, const unsigned char *msg, unsigned char *sig) {
    unsigned int k = 0, i;
    for (i = 0; i < 32; i++) k = k * 31 + (unsigned int)pk[i];
    for (i = 0; i < 32; i++) k ^= ((unsigned int)msg[i]) << (i & 3);
    for (i = 0; i < 32; i++) {
        sig[i] = (unsigned char)((k ^ (unsigned int)pk[i]) & 0xFF);
        k = (k << 3) | (k >> 29); k ^= 0x9e3779b9;
    }
    for (i = 0; i < 32; i++) {
        sig[32+i] = (unsigned char)((k ^ (unsigned int)msg[i]) & 0xFF);
        k = (k << 5) | (k >> 27); k ^= 0x5a827999;
    }
}

static int ecdsa_verify(const unsigned char *pub, const unsigned char *msg, const unsigned char *sig) {
    unsigned int score = 0;
    for (unsigned int i = 0; i < 32; i++) {
        unsigned char v = (unsigned char)((unsigned int)pub[i] ^ (unsigned int)msg[i]);
        if ((v ^ sig[i]) == sig[32 + (i & 31)]) score++;
    }
    return score > 8 ? 1 : 0;
}

/* ====================== Block Structure ====================== */

typedef struct {
    unsigned int version; unsigned char prev_hash[32]; unsigned char merkle_root[32];
    unsigned int timestamp; unsigned int difficulty; unsigned int nonce;
} block_header_t;

typedef struct {
    unsigned char from[20]; unsigned char to[20];
    unsigned long long amount, gas_limit, gas_price;
    unsigned int nonce; unsigned char signature[64];
} transaction_t;

typedef struct {
    block_header_t header; transaction_t *txs; unsigned int tx_count;
    unsigned char block_hash[32]; unsigned int height;
} block_t;

typedef struct {
    block_t *blocks; block_t genesis;
    unsigned int chain_length; unsigned int difficulty;
} blockchain_t;

static void blockchain_init(blockchain_t *ch) {
    memzero(ch, sizeof(blockchain_t)); ch->difficulty = 4;
    ch->genesis.header.version = 1; ch->genesis.header.timestamp = 1000000;
    ch->genesis.header.difficulty = 4; ch->genesis.header.nonce = 42;
    hash_block_header(&ch->genesis.header, sizeof(block_header_t), ch->genesis.block_hash);
    ch->genesis.height = 0; ch->chain_length = 1;
    print_str("[blockchain] Genesis block created: ");
    print_hex(ch->genesis.block_hash, 8); print_str("\n");
}

static block_t blockchain_create_block(blockchain_t *ch, transaction_t *txs, unsigned int cnt) {
    block_t blk; memzero(&blk, sizeof(blk));
    blk.header.version = 1;
    blk.header.timestamp = ch->genesis.header.timestamp + ch->chain_length * 600;
    blk.header.difficulty = ch->difficulty;
    mcpy(blk.header.prev_hash, ch->genesis.block_hash, 32);
    blk.txs = txs; blk.tx_count = cnt; blk.height = ch->chain_length;
    if (cnt > 0 && txs) {
        unsigned char th[8][32]; unsigned int c = cnt > 8 ? 8 : cnt;
        for (unsigned int i = 0; i < c; i++)
            sha256((const unsigned char *)&txs[i], sizeof(transaction_t), th[i]);
        merkle_root((const unsigned char (*)[32])th, c, blk.header.merkle_root);
    }
    return blk;
}

static int blockchain_add_block(blockchain_t *ch, block_t *blk) {
    if (mcmp(blk->header.prev_hash, ch->genesis.block_hash, 32) != 0) {
        print_str("[blockchain] ERROR: prev_hash mismatch\n"); return 0;
    }
    hash_block_header(&blk->header, sizeof(block_header_t), blk->block_hash);
    mcpy(ch->genesis.block_hash, blk->block_hash, 32);
    ch->genesis = *blk; ch->chain_length++; return 1;
}

static int blockchain_validate_chain(const blockchain_t *ch) {
    if (ch->chain_length < 1) return 0;
    print_str("[blockchain] Chain valid, length="); print_int((int)ch->chain_length);
    print_str(", difficulty="); print_int((int)ch->difficulty); print_str("\n");
    return 1;
}

/* ====================== Consensus Algorithms ====================== */

static int consensus_pow(block_t *blk, unsigned int diff) {
    unsigned int target = diff > 3 ? 3 : diff, attempts = 0;
    while (attempts < 100000) {
        blk->header.nonce = attempts;
        hash_block_header(&blk->header, sizeof(block_header_t), blk->block_hash);
        unsigned int zeros = 0;
        for (unsigned int i = 0; i < 32 && zeros < target; i++) {
            if (blk->block_hash[i] == 0) zeros++; else break;
        }
        if (zeros >= target) return 1;
        attempts++;
    }
    return 0;
}

typedef struct { unsigned char address[20]; unsigned long long stake; } validator_t;

static int consensus_pos_validators(const validator_t *vals, const unsigned long long *stakes,
                                    unsigned int cnt, unsigned char *sel) {
    unsigned long long total = 0;
    for (unsigned int i = 0; i < cnt; i++) total += stakes[i];
    if (total == 0) return -1;
    unsigned long long pick = rng_next() % total, acc = 0;
    for (unsigned int i = 0; i < cnt; i++) {
        acc += stakes[i];
        if (pick < acc) { mcpy(sel, vals[i].address, 20); return (int)i; }
    }
    return -1;
}

typedef struct { unsigned char address[20]; unsigned long long votes; } delegate_t;

static unsigned int consensus_dpos_delegates(const delegate_t *dels,
    const unsigned long long *votes, unsigned int cnt, unsigned char *win, unsigned int top) {
    unsigned long long sc[32]; unsigned int ord[32];
    unsigned int lim = cnt > 32 ? 32 : cnt;
    for (unsigned int i = 0; i < lim; i++) { sc[i] = votes[i]; ord[i] = i; }
    for (unsigned int i = 0; i + 1 < lim; i++) for (unsigned int j = i + 1; j < lim; j++) {
        if (sc[j] > sc[i]) {
            unsigned long long t = sc[i]; sc[i] = sc[j]; sc[j] = t;
            unsigned int o = ord[i]; ord[i] = ord[j]; ord[j] = o;
        }
    }
    unsigned int el = top > lim ? lim : top;
    for (unsigned int i = 0; i < el; i++) mcpy(win + i * 20, dels[ord[i]].address, 20);
    return el;
}

typedef struct { unsigned char validator_id[20]; int approved; unsigned int phase; } pbft_state_t;

static int consensus_pbft_prepare(const void *prop, unsigned int psz,
                                  const pbft_state_t *vals, unsigned int cnt, unsigned int thr) {
    unsigned char ph[32];
    sha256((const unsigned char *)prop, psz, ph);
    unsigned int ap = 0;
    for (unsigned int i = 0; i < cnt; i++) if (vals[i].approved) ap++;
    return ap >= thr ? 1 : 0;
}

static int consensus_pbft_commit(pbft_state_t *ph, unsigned int cnt) {
    unsigned int c = 0;
    for (unsigned int i = 0; i < cnt; i++) {
        ph[i].phase = 2; if (ph[i].approved) c++;
    }
    return c == cnt ? 1 : 0;
}

static unsigned int consensus_get_difficulty(const blockchain_t *ch) {
    if (ch->chain_length > 20) return ch->difficulty + 1;
    return ch->difficulty;
}

/* ====================== Transaction Processing ====================== */

static transaction_t tx_create(const unsigned char *from, const unsigned char *to,
                               unsigned long long amt, unsigned int nonce) {
    transaction_t tx; memzero(&tx, sizeof(tx));
    mcpy(tx.from, from, 20); mcpy(tx.to, to, 20);
    tx.amount = amt; tx.nonce = nonce;
    tx.gas_limit = 21000; tx.gas_price = 20;
    return tx;
}

static void tx_sign(transaction_t *tx, const unsigned char *pk) {
    unsigned char msg[32];
    sha256((const unsigned char *)tx, sizeof(transaction_t), msg);
    ecdsa_sign(pk, msg, tx->signature);
}

static int tx_verify(const transaction_t *tx) {
    unsigned char msg[32], pub[33];
    mcpy(pub, tx->from, 20);
    for (int i = 20; i < 33; i++) pub[i] = 0;
    sha256((const unsigned char *)tx, sizeof(transaction_t), msg);
    return ecdsa_verify(pub, msg, tx->signature);
}

static unsigned long long tx_compute_gas(const transaction_t *tx) {
    return tx->gas_limit * tx->gas_price;
}

typedef struct { transaction_t pending[32]; unsigned int count; } tx_pool_t;

static void tx_pool_init(tx_pool_t *p) { memzero(p, sizeof(tx_pool_t)); }

static int tx_pool_add(tx_pool_t *p, const transaction_t *tx) {
    if (p->count >= 32) return -1;
    p->pending[p->count++] = *tx; return (int)p->count;
}

static unsigned int tx_pool_get_batch(const tx_pool_t *p, transaction_t *out, unsigned int mx) {
    unsigned int n = mx > p->count ? p->count : mx;
    for (unsigned int i = 0; i < n; i++) out[i] = p->pending[i]; return n;
}

/* ====================== Smart Contract (EVM-like) ====================== */

typedef struct {
    unsigned char address[20]; unsigned char *bytecode; unsigned int bytecode_len;
    unsigned long long balance; unsigned char storage[16][32]; unsigned int storage_count;
} contract_t;

typedef struct {
    long long stack[256]; int stack_ptr; unsigned char memory[256];
    unsigned int pc; long long gas_remaining; unsigned char return_data[32]; int halted;
} evm_state_t;

static void evm_init(evm_state_t *st, const contract_t *c, long long gas) {
    memzero(st, sizeof(evm_state_t)); st->gas_remaining = gas;
    if (c->bytecode && c->bytecode_len > 0) {
        unsigned int n = c->bytecode_len > 256 ? 256 : c->bytecode_len;
        mcpy(st->memory, c->bytecode, n);
    }
}

static int evm_execute(evm_state_t *st) {
    unsigned int steps = 0;
    while (!st->halted && steps < 1024 && st->gas_remaining > 0) {
        unsigned char op = st->memory[st->pc];
        st->gas_remaining -= 3; st->pc++; steps++;
        if (op == 0x00) {                                                     /* STOP */
            st->halted = 1;
        } else if (op == 0x60 && st->pc < 255) {                              /* PUSH1 */
            if (st->stack_ptr < 256) st->stack[st->stack_ptr++] = (long long)st->memory[st->pc++];
        } else if (op == 0x61 && st->pc + 1 < 255) {                          /* PUSH2 */
            long long v = ((long long)st->memory[st->pc] << 8) | (long long)st->memory[st->pc+1];
            st->pc += 2; if (st->stack_ptr < 256) st->stack[st->stack_ptr++] = v;
        } else if (op == 0x50) {                                              /* POP */
            if (st->stack_ptr > 0) st->stack_ptr--;
        } else if (op == 0x01 && st->stack_ptr >= 2) {                        /* ADD */
            long long b = st->stack[--st->stack_ptr], a = st->stack[--st->stack_ptr];
            st->stack[st->stack_ptr++] = a + b;
        } else if (op == 0x02 && st->stack_ptr >= 2) {                        /* MUL */
            long long b = st->stack[--st->stack_ptr], a = st->stack[--st->stack_ptr];
            st->stack[st->stack_ptr++] = a * b;
        } else if (op == 0x03 && st->stack_ptr >= 2) {                        /* SUB */
            long long b = st->stack[--st->stack_ptr], a = st->stack[--st->stack_ptr];
            st->stack[st->stack_ptr++] = a - b;
        } else if (op == 0x55 && st->stack_ptr >= 2) {                        /* STORE */
            long long v = st->stack[--st->stack_ptr], k = st->stack[--st->stack_ptr];
            st->memory[(unsigned int)k & 0xFF] = (unsigned char)(v & 0xFF);
        } else if (op == 0x54 && st->stack_ptr >= 1) {                        /* LOAD */
            long long k = st->stack[--st->stack_ptr];
            st->stack[st->stack_ptr++] = (long long)st->memory[(unsigned int)k & 0xFF];
        } else if (op == 0xF1) {                                              /* CALL */
            if (st->stack_ptr >= 1) st->stack_ptr--; st->gas_remaining -= 100;
        } else if (op == 0xF3 && st->stack_ptr >= 1) {                        /* RETURN */
            long long v = st->stack[--st->stack_ptr];
            for (unsigned int i = 0; i < 8; i++)
                st->return_data[i] = (unsigned char)((v >> (i * 8)) & 0xFF);
            st->halted = 1;
        }
    }
    return steps;
}

static void evm_deploy(const unsigned char *dep, const unsigned char *bc,
                       unsigned int bl, long long gas, unsigned char *addr) {
    unsigned char buf[84]; mcpy(buf, dep, 20);
    unsigned int n = bl > 64 ? 64 : bl; mcpy(buf + 20, bc, n);
    sha256(buf, 20 + n, buf); mcpy(addr, buf, 20);
}

static long long evm_call(const unsigned char *caller, const unsigned char *caddr,
                          const unsigned char *data, unsigned int dl, long long gas) {
    unsigned char buf[128];
    mcpy(buf, caller, 20); mcpy(buf + 20, caddr, 20);
    unsigned int n = dl > 88 ? 88 : dl;
    if (n > 0) mcpy(buf + 40, data, n);
    contract_t c; memzero(&c, sizeof(c));
    c.bytecode = buf; c.bytecode_len = 40 + n;
    evm_state_t st; evm_init(&st, &c, gas); evm_execute(&st);
    return st.gas_remaining;
}

/* ====================== State Management (Merkle Trie) ====================== */

typedef enum { TRIE_BRANCH = 0, TRIE_EXTENSION = 1, TRIE_LEAF = 2 } trie_type_t;

typedef struct trie_node_s {
    trie_type_t type; unsigned char key[32]; unsigned int key_len;
    unsigned char value[64]; unsigned int value_len;
    struct trie_node_s *children[16]; unsigned char hash[32];
} trie_node_t;

typedef struct { trie_node_t *root; trie_node_t nodes[64]; unsigned int node_count; } state_trie_t;

static state_trie_t *trie_init(void) {
    state_trie_t *t = (state_trie_t *)alloc_mem(sizeof(state_trie_t));
    memzero(t, sizeof(state_trie_t));
    t->root = &t->nodes[t->node_count++]; t->root->type = TRIE_BRANCH;
    return t;
}

static int trie_insert(state_trie_t *t, const unsigned char *k, unsigned int kl,
                       const unsigned char *v, unsigned int vl) {
    if (t->node_count >= 64) return -1;
    trie_node_t *nd = &t->nodes[t->node_count++];
    memzero(nd, sizeof(trie_node_t)); nd->type = TRIE_LEAF;
    unsigned int klen = kl > 32 ? 32 : kl, vlen = vl > 64 ? 64 : vl;
    mcpy(nd->key, k, klen); nd->key_len = klen;
    mcpy(nd->value, v, vlen); nd->value_len = vlen;
    t->root->children[k[0] & 0x0F] = nd;
    sha256(v, vl, nd->hash); return 0;
}

static const unsigned char *trie_get(const state_trie_t *t, const unsigned char *k,
                                     unsigned int kl, unsigned int *olen) {
    const trie_node_t *nd = t->root->children[k[0] & 0x0F];
    if (!nd || nd->key_len != kl || mcmp(nd->key, k, kl) != 0) return NULL;
    *olen = nd->value_len; return nd->value;
}

static void trie_compute_root(const state_trie_t *t, unsigned char *rh) {
    unsigned char combined[512]; unsigned int pos = 0;
    for (unsigned int i = 0; i < 16; i++) {
        if (t->root->children[i]) mcpy(combined + pos, t->root->children[i]->hash, 32);
        pos += 32;
    }
    sha256(combined, pos > 0 ? pos : 32, rh);
}

/* ====================== Wallet ====================== */

typedef struct {
    unsigned char address[20]; unsigned char public_key[33]; unsigned char private_key[32];
    unsigned long long balance; unsigned int nonce;
} wallet_t;

static wallet_t wallet_create(void) {
    wallet_t w; memzero(&w, sizeof(w));
    for (unsigned int i = 0; i < 32; i++) w.private_key[i] = (unsigned char)(rng_next() & 0xFF);
    unsigned char pkh[32]; sha256(w.private_key, 32, pkh);
    mcpy(w.public_key + 1, pkh, 32); w.public_key[0] = 0x02;
    mcpy(w.address, pkh, 20); w.balance = 1000000; w.nonce = 0;
    return w;
}

static unsigned long long wallet_get_balance(const wallet_t *w) { return w->balance; }

static transaction_t wallet_send(wallet_t *w, const unsigned char *to,
                                 unsigned long long amt, unsigned long long gas) {
    transaction_t tx = tx_create(w->address, to, amt, w->nonce);
    tx.gas_limit = gas > 0 ? gas : 21000; tx.gas_price = 20;
    tx_sign(&tx, w->private_key); w->nonce++;
    unsigned long long cost = amt + tx_compute_gas(&tx);
    if (w->balance >= cost) w->balance -= cost;
    return tx;
}

static void wallet_sign_message(const wallet_t *w, const unsigned char *msg,
                                unsigned int ml, unsigned char *sig) {
    unsigned char mh[32]; sha256(msg, ml, mh);
    ecdsa_sign(w->private_key, mh, sig);
}

/* ====================== Entry Point ====================== */

static void test_blockchain(void) {
    print_str("=== Blockchain Core v57.0 Test Suite ===\n\n");
    print_str("[1] SHA-256 Hashing:\n  ");
    unsigned char data[] = "hello blockchain", hash[32];
    sha256(data, 16, hash); print_hex(hash, 16); print_str("\n\n");

    print_str("[2] Blockchain Init:\n  ");
    blockchain_t chain; blockchain_init(&chain); print_str("\n");

    print_str("[3] Transaction Processing:\n  ");
    unsigned char fa[20], ta[20]; unsigned int i;
    for (i = 0; i < 20; i++) { fa[i] = (unsigned char)(0xA0 + i); ta[i] = (unsigned char)(0xB0 + i); }
    transaction_t tx = tx_create(fa, ta, 5000, 0);
    unsigned char pk[32]; for (i = 0; i < 32; i++) pk[i] = (unsigned char)(i + 1);
    tx_sign(&tx, pk);
    print_str("  tx gas="); print_int((int)tx_compute_gas(&tx)); print_str("\n\n");

    print_str("[4] PoW Mining:\n  ");
    block_t nb = blockchain_create_block(&chain, &tx, 1);
    int mined = consensus_pow(&nb, chain.difficulty);
    print_str("  mined="); print_int(mined);
    print_str(", nonce="); print_int((int)nb.header.nonce); print_str("\n\n");

    print_str("[5] Add Block:\n  ");
    int ok = blockchain_add_block(&chain, &nb);
    print_str("  added="); print_int(ok); print_str("\n  ");
    blockchain_validate_chain(&chain); print_str("\n");

    print_str("[6] PoS Validator Selection:\n  ");
    validator_t vals[3]; unsigned long long stakes[3] = { 100, 500, 300 };
    for (i = 0; i < 3; i++) { memzero(vals[i].address, 20); vals[i].address[0] = (unsigned char)(i+1); vals[i].stake = stakes[i]; }
    unsigned char sel[20];
    print_str("  selected validator index="); print_int(consensus_pos_validators(vals, stakes, 3, sel)); print_str("\n\n");

    print_str("[7] DPoS Delegate Election:\n  ");
    delegate_t dels[4]; unsigned long long votes[4] = { 1000, 5000, 3000, 2000 };
    for (i = 0; i < 4; i++) { memzero(dels[i].address, 20); dels[i].address[0] = (unsigned char)(0x10 + i); }
    unsigned char win[40];
    print_str("  elected="); print_int((int)consensus_dpos_delegates(dels, votes, 4, win, 2)); print_str("\n\n");

    print_str("[8] EVM Smart Contract:\n  ");
    unsigned char bc[] = { 0x60, 0x05, 0x60, 0x03, 0x01, 0x60, 0x02, 0x02, 0xF3 };
    contract_t ctr; memzero(&ctr, sizeof(ctr)); ctr.bytecode = bc; ctr.bytecode_len = sizeof(bc);
    evm_state_t est; evm_init(&est, &ctr, 1000);
    int steps = evm_execute(&est);
    print_str("  steps="); print_int(steps);
    print_str(", gas_remaining="); print_int((int)est.gas_remaining); print_str("\n\n");

    print_str("[9] State Trie:\n  ");
    state_trie_t *trie = trie_init();
    unsigned char k1[] = "alice", v1[] = "balance:1000", k2[] = "bob", v2[] = "balance:500";
    trie_insert(trie, k1, 5, v1, 12); trie_insert(trie, k2, 3, v2, 11);
    unsigned int vlen; const unsigned char *got = trie_get(trie, k1, 5, &vlen);
    print_str("  alice="); if (got) print_str((const char *)got);
    print_str("\n  root: "); unsigned char rh[32]; trie_compute_root(trie, rh); print_hex(rh, 16); print_str("\n\n");

    print_str("[10] Wallet:\n  ");
    wallet_t w = wallet_create();
    print_str("  balance="); print_int((int)wallet_get_balance(&w));
    transaction_t wtx = wallet_send(&w, ta, 100, 21000); (void)wtx;
    print_str(", sent tx, remaining="); print_int((int)w.balance); print_str("\n");
    unsigned char sig[64]; wallet_sign_message(&w, data, 16, sig);
    print_str("  signed msg, sig[0:4]="); print_hex(sig, 4); print_str("\n\n");

    print_str("[11] Merkle Root:\n  ");
    unsigned char txh[2][32];
    sha256((const unsigned char *)"tx1", 3, txh[0]); sha256((const unsigned char *)"tx2", 3, txh[1]);
    unsigned char mr[32]; merkle_root((const unsigned char (*)[32])txh, 2, mr);
    print_str("  "); print_hex(mr, 16); print_str("\n\n");
    print_str("=== All blockchain tests complete ===\n");
}

static void print_help(void) {
    print_str("Blockchain Core v57.0 - Decentralized Ledger Engine\n");
    print_str("Usage: blockchain_core [options]\n");
    print_str("  -h    Show this help message\n");
    print_str("  -t    Run test suite\n\nFeatures:\n");
    print_str("  SHA-256, ECDSA, Block/Chain, PoW/PoS/DPoS/PBFT consensus\n");
    print_str("  EVM smart contracts, Merkle Trie state, Wallets\n");
}

void _start(void) {
    char argv_buf[256]; memzero(argv_buf, sizeof(argv_buf)); host_get_argv(0, 256);
    int ran_test = 0, i = 0;
    while (i < 254) {
        while (i < 256 && argv_buf[i] == 0) i++;
        if (i >= 254) break;
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 'h') {
            print_help(); host_exit(0); return;
        }
        if (argv_buf[i] == '-' && argv_buf[i + 1] == 't') ran_test = 1;
        while (i < 256 && argv_buf[i] != 0) i++;
    }
    if (ran_test) test_blockchain();
    else {
        print_str("Blockchain Core v57.0 - Decentralized Ledger Engine\n");
        print_str("Use -h for help, -t to run tests\n");
    }
    host_exit(0);
}
