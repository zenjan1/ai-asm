/* blockchain_integration: Full-stack blockchain module (v1.0)
 *
 * Features: EVM smart contracts (gas metering), consensus (PoW/PoS/DPoS/BFT),
 * transaction management (signing/mempool/block packing), HD wallets (multi-chain),
 * DApp event framework, cross-chain bridging.
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

/* Consensus types */
#define CONSENSUS_POW   0
#define CONSENSUS_POS   1
#define CONSENSUS_DPOS  2
#define CONSENSUS_BFT   3

/* Chain identifiers */
#define CHAIN_ETHEREUM  0
#define CHAIN_POLYGON   1
#define CHAIN_BSC       2
#define CHAIN_AVALANCHE 3
#define CHAIN_SOLANA    4

/* Transaction states */
#define TX_PENDING      0
#define TX_SIGNED       1
#define TX_IN_MEMPOOL   2
#define TX_INCLUDED     3

/* Limits */
#define MAX_CONTRACTS    8
#define MAX_TXS         16
#define MAX_BLOCKS       8
#define MAX_VALIDATORS   8
#define MAX_WALLETS      4
#define MAX_EVENTS       8
#define MAX_BRIDGES      4
#define MAX_MEMPOOL     32
#define MAX_BYTECODE   128
#define MAX_SIGNATURE   40
#define MAX_ADDRESS     28
#define MAX_HASH        28
#define MAX_MNEMONIC    80
#define MAX_TOPIC       36
#define MAX_ASSET_NAME  16

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static void my_strcpy(char *d, const char *s) { while (*s) *d++ = *s++; *d = '\0'; }

static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[32]; int p = 0;
    if (v < 0) { buf[p++] = '-'; v = -v; }
    if (v == 0) buf[p++] = '0';
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; buf[p] = '\0'; p--;
        while (v > 0) { buf[p--] = '0' + (v % 10); v /= 10; } }
    host_print(buf);
}
static void print_kv(const char *k, int v) { print_str(k); print_int(v); }
static void hex_nibble(char *out, int val) {
    *out = val < 10 ? '0' + val : 'a' + val - 10;
}

/* Data structures */
typedef struct {
    int id; char address[MAX_ADDRESS]; char bytecode[MAX_BYTECODE];
    int bytecode_len; int gas_used; int deployed;
} evm_contract_t;

typedef struct {
    int id; char from[MAX_ADDRESS]; char to[MAX_ADDRESS];
    int value; int gas_price; int nonce;
    char signature[MAX_SIGNATURE]; int state; int chain_id;
} transaction_t;

typedef struct {
    int number; char prev_hash[MAX_HASH]; char hash[MAX_HASH];
    int tx_count; int tx_ids[MAX_TXS]; int validator_id;
} block_t;

typedef struct {
    int id; char address[MAX_ADDRESS]; int stake; int delegated; int active;
} validator_t;

typedef struct {
    int id; char mnemonic[MAX_MNEMONIC]; int chain_id; int deriv_idx;
    char address[MAX_ADDRESS]; int balance; int active;
} hd_wallet_t;

typedef struct {
    int id; int contract_id; char topic[MAX_TOPIC]; int from_block; int active;
} event_sub_t;

typedef struct {
    int id; int src_chain; int dst_chain; char asset[MAX_ASSET_NAME];
    int locked; int minted; int active;
} bridge_t;

/* Global state */
static int g_consensus = CONSENSUS_POS, g_height = 0, g_gas = 0;
static evm_contract_t g_contracts[MAX_CONTRACTS]; static int g_n_contracts = 0;
static transaction_t g_txs[MAX_TXS];              static int g_n_txs = 0;
static transaction_t g_mempool[MAX_MEMPOOL];       static int g_pool_sz = 0;
static block_t g_blocks[MAX_BLOCKS];               static int g_n_blocks = 0;
static validator_t g_vals[MAX_VALIDATORS];         static int g_n_vals = 0;
static hd_wallet_t g_wallets[MAX_WALLETS];         static int g_n_wallets = 0;
static event_sub_t g_subs[MAX_EVENTS];             static int g_n_subs = 0;
static bridge_t g_bridges[MAX_BRIDGES];            static int g_n_bridges = 0;

/* Generate hex address from seed data */
static void gen_hex_addr(char *out, const char *seed, int slen, int salt) {
    my_strcpy(out, "0x");
    for (int i = 0; i < 20; i++) {
        int c = (i < slen) ? seed[i] : i;
        hex_nibble(&out[2 + i], (c + salt * 5 + i * 3) & 0x0F);
    }
    out[22] = '\0';
}

/* ===== EVM Smart Contracts ===== */
int evm_deploy(const char *bytecode, int gas_limit) {
    if (g_n_contracts >= MAX_CONTRACTS) return -1;
    evm_contract_t *c = &g_contracts[g_n_contracts];
    c->id = g_n_contracts; c->gas_used = 0; c->deployed = 1;
    int bl = my_strlen(bytecode);
    if (bl > MAX_BYTECODE - 1) bl = MAX_BYTECODE - 1;
    my_strncpy(c->bytecode, bytecode, MAX_BYTECODE - 1);
    c->bytecode_len = bl;
    gen_hex_addr(c->address, bytecode, bl, g_n_contracts + 1);
    c->gas_used = 21000 + bl * 200;
    if (c->gas_used > gas_limit) {
        print_str("[evm] OUT OF GAS: need "); print_int(c->gas_used);
        print_str(" limit "); print_int(gas_limit); print_str("\n");
        c->deployed = 0; return -2;
    }
    g_gas += c->gas_used;
    int id = g_n_contracts++;
    print_str("[evm] deploy id="); print_int(id);
    print_str(" addr="); print_str(c->address);
    print_str(" gas="); print_int(c->gas_used);
    print_str("/"); print_int(gas_limit); print_str("\n");
    return id;
}

int evm_execute(int cid, int gas_limit) {
    if (cid < 0 || cid >= g_n_contracts) return -1;
    evm_contract_t *c = &g_contracts[cid];
    if (!c->deployed) return -2;
    int cost = 5000 + c->bytecode_len * 10;
    if (cost > gas_limit) { print_str("[evm] exec OUT OF GAS\n"); return -3; }
    c->gas_used += cost; g_gas += cost;
    print_str("[evm] exec contract "); print_kv(" gas=", cost);
    print_str(" total="); print_int(g_gas); print_str("\n");
    return 0;
}

/* ===== Consensus Mechanisms ===== */
void consensus_init(int type) {
    g_consensus = type;
    const char *names[] = {"PoW", "PoS", "DPoS", "BFT"};
    print_str("[consensus] init: "); print_str(names[type & 3]); print_str("\n");
}

int consensus_add_validator(const char *addr, int stake) {
    if (g_n_vals >= MAX_VALIDATORS) return -1;
    validator_t *v = &g_vals[g_n_vals];
    v->id = g_n_vals; my_strncpy(v->address, addr, MAX_ADDRESS - 1);
    v->stake = stake; v->delegated = 0; v->active = 1;
    int id = g_n_vals++;
    print_str("[consensus] validator "); print_kv("id=", id);
    print_str(" stake="); print_int(stake); print_str("\n");
    return id;
}

int consensus_select_proposer(void) {
    if (g_n_vals == 0) return -1;
    int p = -1;
    if (g_consensus == CONSENSUS_POW) {
        int best = -1;
        for (int i = 0; i < g_n_vals; i++) {
            int h = (g_height * 31 + i * 17 + 7) & 0xFFFF;
            if (h > best) { best = h; p = i; }
        }
    } else if (g_consensus == CONSENSUS_POS) {
        int total = 0;
        for (int i = 0; i < g_n_vals; i++) total += g_vals[i].stake;
        int pick = (g_height * 37 + 13) % (total > 0 ? total : 1);
        int run = 0;
        for (int i = 0; i < g_n_vals; i++) {
            run += g_vals[i].stake;
            if (pick < run) { p = i; break; }
        }
    } else if (g_consensus == CONSENSUS_DPOS) {
        int best = -1;
        for (int i = 0; i < g_n_vals; i++) {
            int tv = g_vals[i].stake + g_vals[i].delegated;
            if (tv > best) { best = tv; p = i; }
        }
    } else {
        p = g_height % g_n_vals;  /* BFT: round-robin */
    }
    print_str("[consensus] proposer: "); print_int(p);
    print_str(" type="); print_int(g_consensus); print_str("\n");
    return p;
}

/* ===== Transaction Management ===== */
int tx_create(const char *from, const char *to, int value, int gas_price) {
    if (g_n_txs >= MAX_TXS) return -1;
    transaction_t *tx = &g_txs[g_n_txs];
    tx->id = g_n_txs;
    my_strncpy(tx->from, from, MAX_ADDRESS - 1);
    my_strncpy(tx->to, to, MAX_ADDRESS - 1);
    tx->value = value; tx->gas_price = gas_price;
    tx->nonce = g_n_txs; tx->state = TX_PENDING;
    tx->chain_id = CHAIN_ETHEREUM; tx->signature[0] = '\0';
    int id = g_n_txs++;
    print_str("[tx] create id="); print_int(id);
    print_str(" "); print_str(from); print_str("->"); print_str(to);
    print_str(" val="); print_int(value); print_str("\n");
    return id;
}

int tx_sign(int tid, const char *pkey) {
    if (tid < 0 || tid >= g_n_txs) return -1;
    transaction_t *tx = &g_txs[tid];
    if (tx->state != TX_PENDING) return -2;
    my_strcpy(tx->signature, "0xSIG:");
    int pos = 6;
    for (int i = 0; i < 24 && pkey[i]; i++)
        hex_nibble(&tx->signature[pos++], (pkey[i] + tid * 3) & 0x0F);
    tx->signature[pos] = '\0';
    tx->state = TX_SIGNED;
    print_str("[tx] sign id="); print_int(tid);
    print_str(" sig="); print_str(tx->signature); print_str("\n");
    return 0;
}

int tx_submit(int tid) {
    if (tid < 0 || tid >= g_n_txs) return -1;
    transaction_t *tx = &g_txs[tid];
    if (tx->state != TX_SIGNED) return -2;
    if (g_pool_sz >= MAX_MEMPOOL) return -3;
    g_mempool[g_pool_sz++] = *tx;
    tx->state = TX_IN_MEMPOOL;
    print_str("[tx] mempool id="); print_int(tid);
    print_str(" pool="); print_int(g_pool_sz); print_str("\n");
    return 0;
}

int tx_pack_block(int proposer) {
    if (g_pool_sz == 0 || g_n_blocks >= MAX_BLOCKS) return 0;
    block_t *b = &g_blocks[g_n_blocks];
    b->number = g_height + 1; b->validator_id = proposer;
    b->tx_count = 0;
    if (g_n_blocks > 0) my_strncpy(b->prev_hash, g_blocks[g_n_blocks - 1].hash, MAX_HASH - 1);
    else my_strcpy(b->prev_hash, "0x0000000000000000000000");
    int packed = 0;
    for (int i = 0; i < g_pool_sz && packed < MAX_TXS; i++) {
        if (g_mempool[i].state == TX_IN_MEMPOOL) {
            b->tx_ids[packed++] = g_mempool[i].id;
            g_txs[g_mempool[i].id].state = TX_INCLUDED;
        }
    }
    b->tx_count = packed;
    my_strcpy(b->hash, "0xBLK:");
    int pos = 6;
    for (int i = 0; i < 16; i++)
        hex_nibble(&b->hash[pos++], (b->number * 13 + packed * 7 + i) & 0x0F);
    b->hash[pos] = '\0';
    g_height = b->number; g_n_blocks++; g_pool_sz = 0;
    print_str("[block] #"); print_int(b->number);
    print_str(" txs="); print_int(packed);
    print_str(" hash="); print_str(b->hash); print_str("\n");
    return packed;
}

/* ===== Wallet Management ===== */
int wallet_create(const char *mnemonic, int chain_id) {
    if (g_n_wallets >= MAX_WALLETS) return -1;
    hd_wallet_t *w = &g_wallets[g_n_wallets];
    w->id = g_n_wallets;
    my_strncpy(w->mnemonic, mnemonic, MAX_MNEMONIC - 1);
    w->chain_id = chain_id; w->deriv_idx = 0;
    w->balance = 1000000; w->active = 1;
    int ml = my_strlen(mnemonic);
    gen_hex_addr(w->address, mnemonic, ml, chain_id + 1);
    int id = g_n_wallets++;
    const char *cnames[] = {"ETH", "MATIC", "BSC", "AVAX", "SOL"};
    print_str("[wallet] create id="); print_int(id);
    print_str(" chain="); print_str((chain_id >= 0 && chain_id <= 4) ? cnames[chain_id] : "???");
    print_str(" addr="); print_str(w->address); print_str("\n");
    return id;
}

int wallet_derive(int wid) {
    if (wid < 0 || wid >= g_n_wallets) return -1;
    hd_wallet_t *w = &g_wallets[wid];
    w->deriv_idx++;
    int ml = my_strlen(w->mnemonic);
    gen_hex_addr(w->address, w->mnemonic, ml, w->chain_id + w->deriv_idx);
    print_str("[wallet] derive id="); print_int(wid);
    print_str(" idx="); print_int(w->deriv_idx);
    print_str(" addr="); print_str(w->address); print_str("\n");
    return w->deriv_idx;
}

/* ===== DApp Framework ===== */
int dapp_subscribe(int cid, const char *topic) {
    if (g_n_subs >= MAX_EVENTS || cid < 0 || cid >= g_n_contracts) return -1;
    event_sub_t *s = &g_subs[g_n_subs];
    s->id = g_n_subs; s->contract_id = cid;
    my_strncpy(s->topic, topic, MAX_TOPIC - 1);
    s->from_block = g_height; s->active = 1;
    int id = g_n_subs++;
    print_str("[dapp] sub id="); print_int(id);
    print_str(" contract="); print_int(cid);
    print_str(" topic="); print_str(topic); print_str("\n");
    return id;
}

void dapp_sync(void) {
    print_str("[dapp] sync block="); print_int(g_height); print_str("\n");
    int fired = 0;
    for (int i = 0; i < g_n_subs; i++) {
        if (g_subs[i].active && g_height > g_subs[i].from_block) {
            print_str("  event: "); print_str(g_subs[i].topic);
            print_str(" block="); print_int(g_height); print_str("\n");
            fired++;
        }
    }
    print_str("[dapp] sync done events="); print_int(fired); print_str("\n");
}

/* ===== Cross-Chain Bridging ===== */
int bridge_create(int src, int dst, const char *asset) {
    if (g_n_bridges >= MAX_BRIDGES) return -1;
    bridge_t *b = &g_bridges[g_n_bridges];
    b->id = g_n_bridges; b->src_chain = src; b->dst_chain = dst;
    my_strncpy(b->asset, asset, MAX_ASSET_NAME - 1);
    b->locked = 0; b->minted = 0; b->active = 1;
    int id = g_n_bridges++;
    print_str("[bridge] create id="); print_int(id);
    print_str(" "); print_int(src); print_str("->"); print_int(dst);
    print_str(" asset="); print_str(asset); print_str("\n");
    return id;
}

int bridge_transfer(int bid, int amount) {
    if (bid < 0 || bid >= g_n_bridges || amount <= 0) return -1;
    bridge_t *b = &g_bridges[bid];
    if (!b->active) return -2;
    b->locked += amount; b->minted += amount;
    print_str("[bridge] xfer id="); print_int(bid);
    print_str(" amt="); print_int(amount);
    print_str(" locked="); print_int(b->locked);
    print_str(" minted="); print_int(b->minted); print_str("\n");
    return 0;
}

/* ===== Status ===== */
void blockchain_status(void) {
    print_str("=== Blockchain Status ===\n");
    print_str("  consensus="); print_int(g_consensus); print_str("\n");
    print_str("  height="); print_int(g_height); print_str("\n");
    print_str("  contracts="); print_int(g_n_contracts); print_str("\n");
    print_str("  txs="); print_int(g_n_txs); print_str("\n");
    print_str("  validators="); print_int(g_n_vals); print_str("\n");
    print_str("  wallets="); print_int(g_n_wallets); print_str("\n");
    print_str("  bridges="); print_int(g_n_bridges); print_str("\n");
    print_str("  gas_total="); print_int(g_gas); print_str("\n");
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Blockchain Integration v1.0\n");
    if (help) {
        print_str("Usage: blockchain_integration [-h] [-t]\n");
        print_str("  -h  Show help\n  -t  Run test suite\n\n");
        print_str("Features: EVM contracts, PoW/PoS/DPoS/BFT consensus,\n");
        print_str("  tx management, HD wallets, DApp events, cross-chain bridge\n");
        return;
    }
    if (test) {
        print_str("=== Blockchain Test Suite ===\n\n");

        /* Test 1: Consensus setup */
        print_str("[Test 1] Consensus (PoS)\n");
        consensus_init(CONSENSUS_POS);
        consensus_add_validator("0xval0_aaaaaaaaaaaaaa", 1000);
        consensus_add_validator("0xval1_bbbbbbbbbbbbbb", 2000);
        consensus_add_validator("0xval2_cccccccccccccccc", 1500);
        print_str("\n");

        /* Test 2: HD Wallets (multi-chain) */
        print_str("[Test 2] HD Wallets\n");
        int w0 = wallet_create("abandon ability able about above absent absorb abstract", CHAIN_ETHEREUM);
        int w1 = wallet_create("zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong", CHAIN_POLYGON);
        wallet_derive(w0); wallet_derive(w0); wallet_derive(w1);
        print_str("\n");

        /* Test 3: EVM contracts */
        print_str("[Test 3] EVM Contracts\n");
        int c0 = evm_deploy("6080604052348015600f57600080fd5b5060", 500000);
        int c1 = evm_deploy("60016000803373ffffffffffffffffffff", 300000);
        evm_execute(c0, 100000); evm_execute(c1, 100000);
        print_str("\n");

        /* Test 4: Transaction lifecycle */
        print_str("[Test 4] Transactions\n");
        int t0 = tx_create("0xalice_addr123456789", "0xbob_addr1234567890a", 1000, 20);
        int t1 = tx_create("0xcarol_addr123456789", "0xdave_addr123456789a", 500, 30);
        int t2 = tx_create("0xeve_addr1234567890a", "0xbob_addr1234567890a", 250, 25);
        tx_sign(t0, "privkey_alice_001");
        tx_sign(t1, "privkey_carol_002");
        tx_sign(t2, "privkey_eve_003");
        tx_submit(t0); tx_submit(t1); tx_submit(t2);
        print_str("\n");

        /* Test 5: Block packing */
        print_str("[Test 5] Block Packing\n");
        int prop = consensus_select_proposer();
        tx_pack_block(prop); dapp_sync();
        print_str("\n");

        /* Test 6: DApp events */
        print_str("[Test 6] DApp Events\n");
        if (c0 >= 0) { dapp_subscribe(c0, "Transfer(address,address,uint256)");
                       dapp_subscribe(c0, "Approval(address,address,uint256)"); }
        if (c1 >= 0) dapp_subscribe(c1, "Swap(address,uint256,uint256)");
        print_str("\n");

        /* Test 7: Cross-chain bridge */
        print_str("[Test 7] Cross-Chain Bridge\n");
        int b0 = bridge_create(CHAIN_ETHEREUM, CHAIN_POLYGON, "ETH");
        int b1 = bridge_create(CHAIN_ETHEREUM, CHAIN_BSC, "USDC");
        bridge_transfer(b0, 10000); bridge_transfer(b0, 5000);
        bridge_transfer(b1, 50000);
        print_str("\n");

        /* Test 8: Second block */
        print_str("[Test 8] Second Block\n");
        int t3 = tx_create("0xbob_addr1234567890a", "0xalice_addr123456789", 200, 15);
        tx_sign(t3, "privkey_bob_004"); tx_submit(t3);
        prop = consensus_select_proposer();
        tx_pack_block(prop); dapp_sync();
        print_str("\n");

        /* Test 9: Consensus variants */
        print_str("[Test 9] Consensus Variants\n");
        consensus_init(CONSENSUS_POW); consensus_select_proposer();
        consensus_init(CONSENSUS_DPOS); consensus_select_proposer();
        consensus_init(CONSENSUS_BFT); consensus_select_proposer();
        print_str("\n");

        /* Test 10: Status */
        print_str("[Test 10] Final Status\n");
        blockchain_status();
        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
