/* blockchain_finance: Blockchain finance system (v1.0)
 * DeFi protocols, smart contracts, token economics, cross-chain bridges, NFT marketplace
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

#define MAX_POOLS        8
#define MAX_CONTRACTS    8
#define MAX_TOKENS       8
#define MAX_NFTS         8
#define MAX_BRIDGES      4
#define MAX_SWAPS        16
#define MAX_EVENTS       16

typedef struct {
    int    pool_id;
    int    token_a;
    int    token_b;
    double reserve_a;
    double reserve_b;
    double total_lp;
    double fee_rate;
    int    active;
} pool_t;

typedef struct {
    int    contract_id;
    int    type;
    double tvl;
    double gas_used;
    int    calls;
    int    events_emitted;
    int    active;
} contract_t;

typedef struct {
    int    token_id;
    double total_supply;
    double circulating;
    double staked;
    double apy;
    int    decimals;
    int    active;
} token_t;

typedef struct {
    int    nft_id;
    int    collection_id;
    double price;
    double royalty_pct;
    int    owner;
    int    listed;
    int    active;
} nft_t;

typedef struct {
    int    bridge_id;
    int    src_chain;
    int    dst_chain;
    double volume;
    int    tx_count;
    double fee;
    int    active;
} bridge_t;

typedef struct {
    int    swap_id;
    int    pool_id;
    int    direction;
    double amount_in;
    double amount_out;
    double price_impact;
    int    active;
} swap_t;

typedef struct {
    int    event_id;
    int    contract_id;
    int    event_type;
    double value;
    int    active;
} event_t;

typedef struct {
    int    n_pools;
    int    n_contracts;
    int    n_tokens;
    int    n_nfts;
    int    n_bridges;
    int    n_swaps;
    int    n_events;
    double total_tvl;
    double total_volume;
    double total_fees;
    double gas_price;
} bf_state_t;

static pool_t pools[MAX_POOLS];
static contract_t contracts[MAX_CONTRACTS];
static token_t tokens[MAX_TOKENS];
static nft_t nfts[MAX_NFTS];
static bridge_t bridges[MAX_BRIDGES];
static swap_t swaps[MAX_SWAPS];
static event_t events[MAX_EVENTS];
static bf_state_t bf;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}

int bf_init(void) {
    if (initialized) return -1;
    bf.n_pools = 0; bf.n_contracts = 0; bf.n_tokens = 0;
    bf.n_nfts = 0; bf.n_bridges = 0; bf.n_swaps = 0; bf.n_events = 0;
    bf.total_tvl = 0.0; bf.total_volume = 0.0; bf.total_fees = 0.0;
    bf.gas_price = 20.0;
    for (int i = 0; i < MAX_POOLS; i++) pools[i].active = 0;
    for (int i = 0; i < MAX_CONTRACTS; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_TOKENS; i++) tokens[i].active = 0;
    for (int i = 0; i < MAX_NFTS; i++) nfts[i].active = 0;
    for (int i = 0; i < MAX_BRIDGES; i++) bridges[i].active = 0;
    for (int i = 0; i < MAX_SWAPS; i++) swaps[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    initialized = 1;
    print_str("[BF] Blockchain finance initialized\n");
    return 0;
}

int bf_deploy_contract(int type, double initial_value) {
    if (bf.n_contracts >= MAX_CONTRACTS) return -1;
    contract_t* c = &contracts[bf.n_contracts];
    c->contract_id = bf.n_contracts;
    c->type = type;
    c->tvl = initial_value;
    c->gas_used = 100000.0 + (double)(type * 50000);
    c->calls = 0;
    c->events_emitted = 0;
    c->active = 1;
    bf.n_contracts++;
    bf.total_tvl += initial_value;
    print_str("[BF] Contract "); print_int(bf.n_contracts - 1);
    print_str(" type="); print_int(type);
    print_str(" TVL="); print_int((int)initial_value);
    print_str(" gas="); print_int((int)c->gas_used); print_str("\n");
    return bf.n_contracts - 1;
}

int bf_create_pool(int token_a, int token_b, double amt_a, double amt_b) {
    if (bf.n_pools >= MAX_POOLS) return -1;
    pool_t* p = &pools[bf.n_pools];
    p->pool_id = bf.n_pools;
    p->token_a = token_a;
    p->token_b = token_b;
    p->reserve_a = amt_a;
    p->reserve_b = amt_b;
    p->total_lp = dsqrt(amt_a * amt_b);
    p->fee_rate = 0.003;
    p->active = 1;
    bf.n_pools++;
    print_str("[BF] Pool "); print_int(bf.n_pools - 1);
    print_str(" T"); print_int(token_a); print_str("/T"); print_int(token_b);
    print_str(" A="); print_int((int)amt_a);
    print_str(" B="); print_int((int)amt_b);
    print_str(" LP="); print_int((int)p->total_lp);
    print_str(" fee=30bp\n");
    return bf.n_pools - 1;
}

int bf_swap(int pool_id, int direction, double amount_in) {
    if (pool_id >= bf.n_pools || bf.n_swaps >= MAX_SWAPS) return -1;
    pool_t* p = &pools[pool_id];
    double reserve_in = direction ? p->reserve_b : p->reserve_a;
    double reserve_out = direction ? p->reserve_a : p->reserve_b;
    double amount_in_fee = amount_in * (1.0 - p->fee_rate);
    double k = reserve_in * reserve_out;
    double new_reserve_in = reserve_in + amount_in_fee;
    double new_reserve_out = k / new_reserve_in;
    double amount_out = reserve_out - new_reserve_out;
    double price_impact = ((amount_out / amount_in) / (reserve_out / reserve_in) - 1.0) * 100.0;
    if (direction) {
        p->reserve_b = new_reserve_in;
        p->reserve_a = new_reserve_out;
    } else {
        p->reserve_a = new_reserve_in;
        p->reserve_b = new_reserve_out;
    }
    swap_t* sw = &swaps[bf.n_swaps];
    sw->swap_id = bf.n_swaps;
    sw->pool_id = pool_id;
    sw->direction = direction;
    sw->amount_in = amount_in;
    sw->amount_out = amount_out;
    sw->price_impact = price_impact;
    sw->active = 1;
    bf.n_swaps++;
    bf.total_volume += amount_in;
    bf.total_fees += amount_in * p->fee_rate;
    print_str("[BF] Swap "); print_int(bf.n_swaps - 1);
    print_str(" pool="); print_int(pool_id);
    print_str(" dir="); print_int(direction);
    print_str(" in="); print_int((int)amount_in);
    print_str(" out="); print_int((int)amount_out);
    print_str(" impact="); print_int((int)(dabs(price_impact) * 100)); print_str("bp\n");
    return bf.n_swaps - 1;
}

int bf_add_liquidity(int pool_id, double amt_a, double amt_b) {
    if (pool_id >= bf.n_pools) return -1;
    pool_t* p = &pools[pool_id];
    double lp_minted = dsqrt(amt_a * amt_b);
    p->reserve_a += amt_a;
    p->reserve_b += amt_b;
    p->total_lp += lp_minted;
    bf.total_tvl += (amt_a + amt_b) * 0.5;
    print_str("[BF] Liquidity pool="); print_int(pool_id);
    print_str(" A="); print_int((int)amt_a);
    print_str(" B="); print_int((int)amt_b);
    print_str(" LP="); print_int((int)lp_minted); print_str(" minted\n");
    return 0;
}

int bf_lend(int token_id, double amount, int duration_blocks) {
    if (token_id >= bf.n_tokens) return -1;
    token_t* t = &tokens[token_id];
    double interest = amount * t->apy * 0.01 * (double)duration_blocks / 2628000.0;
    t->staked += amount;
    bf.total_tvl += amount;
    print_str("[BF] Lend T"); print_int(token_id);
    print_str(" amount="); print_int((int)amount);
    print_str(" blocks="); print_int(duration_blocks);
    print_str(" APY="); print_int((int)(t->apy * 100)); print_str("bp");
    print_str(" interest="); print_int((int)interest); print_str("\n");
    return 0;
}

int bf_yield_farm(int pool_id, double lp_amount) {
    if (pool_id >= bf.n_pools) return -1;
    pool_t* p = &pools[pool_id];
    double reward_rate = 10.0 + (double)(pool_id % 4) * 2.5;
    double rewards = lp_amount * reward_rate / p->total_lp * 100.0;
    print_str("[BF] Farm pool="); print_int(pool_id);
    print_str(" LP="); print_int((int)lp_amount);
    print_str(" rate="); print_int((int)(reward_rate * 100)); print_str("bp");
    print_str(" reward="); print_int((int)rewards); print_str("\n");
    return 0;
}

int bf_issue_token(double supply, int decimals, double apy) {
    if (bf.n_tokens >= MAX_TOKENS) return -1;
    token_t* t = &tokens[bf.n_tokens];
    t->token_id = bf.n_tokens;
    t->total_supply = supply;
    t->circulating = supply * 0.6;
    t->staked = supply * 0.25;
    t->apy = apy;
    t->decimals = decimals;
    t->active = 1;
    bf.n_tokens++;
    print_str("[BF] Token "); print_int(bf.n_tokens - 1);
    print_str(" supply="); print_int((int)supply);
    print_str(" dec="); print_int(decimals);
    print_str(" APY="); print_int((int)(apy * 100)); print_str("bp");
    print_str(" circ="); print_int((int)t->circulating); print_str("\n");
    return bf.n_tokens - 1;
}

int bf_vesting(int token_id, double amount, int cliff_blocks, int vest_blocks) {
    if (token_id >= bf.n_tokens) return -1;
    double released_now = (cliff_blocks > 0) ? 0.0 : amount * 0.1;
    double remaining = amount - released_now;
    double per_block = remaining / (double)(vest_blocks > 0 ? vest_blocks : 1);
    print_str("[BF] Vesting T"); print_int(token_id);
    print_str(" total="); print_int((int)amount);
    print_str(" cliff="); print_int(cliff_blocks);
    print_str(" vest="); print_int(vest_blocks);
    print_str(" released="); print_int((int)released_now);
    print_str(" per_block="); print_int((int)(per_block * 1000)); print_str("m\n");
    return 0;
}

int bf_create_bridge(int src_chain, int dst_chain, double initial_volume) {
    if (bf.n_bridges >= MAX_BRIDGES) return -1;
    bridge_t* b = &bridges[bf.n_bridges];
    b->bridge_id = bf.n_bridges;
    b->src_chain = src_chain;
    b->dst_chain = dst_chain;
    b->volume = initial_volume;
    b->tx_count = 0;
    b->fee = 0.001;
    b->active = 1;
    bf.n_bridges++;
    print_str("[BF] Bridge "); print_int(bf.n_bridges - 1);
    print_str(" chain"); print_int(src_chain);
    print_str("->"); print_int(dst_chain);
    print_str(" vol="); print_int((int)initial_volume);
    print_str(" fee=10bp\n");
    return bf.n_bridges - 1;
}

int bf_bridge_transfer(int bridge_id, double amount) {
    if (bridge_id >= bf.n_bridges) return -1;
    bridge_t* b = &bridges[bridge_id];
    double fee = amount * b->fee;
    double received = amount - fee;
    b->volume += amount;
    b->tx_count++;
    bf.total_volume += amount;
    bf.total_fees += fee;
    print_str("[BF] Bridge xfer "); print_int(bridge_id);
    print_str(" C"); print_int(b->src_chain); print_str("->C"); print_int(b->dst_chain);
    print_str(" amt="); print_int((int)amount);
    print_str(" fee="); print_int((int)fee);
    print_str(" recv="); print_int((int)received); print_str("\n");
    return 0;
}

int bf_mint_nft(int collection_id, double price, double royalty) {
    if (bf.n_nfts >= MAX_NFTS) return -1;
    nft_t* n = &nfts[bf.n_nfts];
    n->nft_id = bf.n_nfts;
    n->collection_id = collection_id;
    n->price = price;
    n->royalty_pct = royalty;
    n->owner = bf.n_nfts;
    n->listed = 1;
    n->active = 1;
    bf.n_nfts++;
    print_str("[BF] NFT "); print_int(bf.n_nfts - 1);
    print_str(" col="); print_int(collection_id);
    print_str(" price="); print_int((int)price);
    print_str(" royalty="); print_int((int)(royalty * 100)); print_str("%\n");
    return bf.n_nfts - 1;
}

int bf_trade_nft(int nft_id, int new_owner) {
    if (nft_id >= bf.n_nfts) return -1;
    nft_t* n = &nfts[nft_id];
    double royalty = n->price * n->royalty_pct;
    double net = n->price - royalty;
    int prev_owner = n->owner;
    n->owner = new_owner;
    bf.total_volume += n->price;
    print_str("[BF] NFT trade "); print_int(nft_id);
    print_str(" O"); print_int(prev_owner); print_str("->O"); print_int(new_owner);
    print_str(" price="); print_int((int)n->price);
    print_str(" royalty="); print_int((int)royalty);
    print_str(" net="); print_int((int)net); print_str("\n");
    return 0;
}

void bf_emit_event(int contract_id, int event_type, double value) {
    if (bf.n_events >= MAX_EVENTS || contract_id >= bf.n_contracts) return;
    event_t* e = &events[bf.n_events];
    e->event_id = bf.n_events;
    e->contract_id = contract_id;
    e->event_type = event_type;
    e->value = value;
    e->active = 1;
    bf.n_events++;
    contracts[contract_id].events_emitted++;
    contracts[contract_id].calls++;
    print_str("[BF] Event "); print_int(bf.n_events - 1);
    print_str(" contract="); print_int(contract_id);
    print_str(" type="); print_int(event_type);
    print_str(" val="); print_int((int)value); print_str("\n");
}

void bf_oracle_update(int token_id, double new_price) {
    if (token_id >= bf.n_tokens) return;
    print_str("[BF] Oracle T"); print_int(token_id);
    print_str(" price="); print_int((int)(new_price * 100)); print_str("c\n");
    bf_emit_event(token_id % bf.n_contracts, 1, new_price);
}

void bf_print_state(void) {
    print_str("[BF] Pools="); print_int(bf.n_pools);
    print_str(" Contracts="); print_int(bf.n_contracts);
    print_str(" Tokens="); print_int(bf.n_tokens);
    print_str(" NFTs="); print_int(bf.n_nfts);
    print_str(" Bridges="); print_int(bf.n_bridges);
    print_str("\n");
    print_str("  TVL="); print_int((int)bf.total_tvl);
    print_str(" Vol="); print_int((int)bf.total_volume);
    print_str(" Fees="); print_int((int)bf.total_fees);
    print_str(" Swaps="); print_int(bf.n_swaps);
    print_str(" Events="); print_int(bf.n_events);
    print_str("\n");
    for (int i = 0; i < bf.n_pools; i++) {
        print_str("  Pool "); print_int(i);
        print_str(" A="); print_int((int)pools[i].reserve_a);
        print_str(" B="); print_int((int)pools[i].reserve_b);
        print_str(" LP="); print_int((int)pools[i].total_lp); print_str("\n");
    }
}

int main(void) {
    print_str("=== Blockchain Finance System Demo ===\n\n");
    bf_init();

    print_str("Issuing tokens...\n");
    bf_issue_token(1000000.0, 18, 5.0);
    bf_issue_token(500000.0, 8, 8.0);
    bf_issue_token(2000000.0, 6, 3.5);

    print_str("\nDeploying contracts...\n");
    bf_deploy_contract(1, 50000.0);
    bf_deploy_contract(2, 30000.0);
    bf_deploy_contract(3, 80000.0);

    print_str("\nCreating liquidity pools...\n");
    bf_create_pool(0, 1, 10000.0, 20000.0);
    bf_create_pool(1, 2, 5000.0, 25000.0);
    bf_create_pool(0, 2, 15000.0, 30000.0);

    print_str("\nExecuting swaps...\n");
    bf_swap(0, 0, 100.0);
    bf_swap(0, 1, 200.0);
    bf_swap(1, 0, 50.0);
    bf_swap(2, 0, 300.0);
    bf_swap(1, 1, 150.0);

    print_str("\nAdding liquidity...\n");
    bf_add_liquidity(0, 5000.0, 10000.0);
    bf_add_liquidity(1, 2000.0, 10000.0);

    print_str("\nLending & yield farming...\n");
    bf_lend(0, 10000.0, 1000);
    bf_lend(1, 5000.0, 500);
    bf_yield_farm(0, 1000.0);
    bf_yield_farm(1, 500.0);
    bf_yield_farm(2, 2000.0);

    print_str("\nToken vesting...\n");
    bf_vesting(0, 100000.0, 1000, 10000);
    bf_vesting(1, 50000.0, 500, 5000);

    print_str("\nCreating bridges...\n");
    bf_create_bridge(1, 2, 10000.0);
    bf_create_bridge(1, 3, 8000.0);
    bf_create_bridge(2, 3, 5000.0);

    print_str("\nBridge transfers...\n");
    bf_bridge_transfer(0, 1000.0);
    bf_bridge_transfer(1, 500.0);
    bf_bridge_transfer(2, 800.0);

    print_str("\nOracle updates...\n");
    bf_oracle_update(0, 1.25);
    bf_oracle_update(1, 0.85);
    bf_oracle_update(2, 0.50);

    print_str("\nMinting NFTs...\n");
    bf_mint_nft(1, 5.0, 0.025);
    bf_mint_nft(1, 10.0, 0.05);
    bf_mint_nft(2, 2.5, 0.03);
    bf_mint_nft(1, 20.0, 0.075);
    bf_mint_nft(2, 15.0, 0.04);

    print_str("\nNFT trades...\n");
    bf_trade_nft(0, 5);
    bf_trade_nft(1, 7);
    bf_trade_nft(3, 9);

    print_str("\nContract events...\n");
    bf_emit_event(0, 2, 5000.0);
    bf_emit_event(1, 3, 3000.0);
    bf_emit_event(2, 2, 8000.0);

    print_str("\nFinal state...\n");
    bf_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
