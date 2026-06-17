/*
 * Metaverse Economy WASM Module for AI-ASM OS
 * Implements: Currency, NFT Assets, Trading Market, Creator Economy,
 *             Financial Services, Governance
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
/* ── Constants ──────────────────────────────────────────────────────── */
#define MAX_USERS       0x400   /* max registered users              */
#define MAX_ASSETS      0x800   /* max NFT assets                    */
#define MAX_ORDERS      0x1000  /* max open orders in order book     */
#define MAX_PROPOSALS   0x100   /* max DAO proposals                 */
#define MAX_COINS       0x40    /* max currency tokens               */
#define INITIAL_SUPPLY  0x3B9ACA00  /* 1 billion base units          */
#define FEE_BPS         25      /* 0.25% protocol fee in basis pts   */
#define STAKING_APY_BPS 500     /* 5% annual staking yield           */
/* ── Enumerations ───────────────────────────────────────────────────── */
enum AssetType {
    ASSET_NFT_COLLECTIBLE = 0x01,
    ASSET_VIRTUAL_LAND    = 0x02,
    ASSET_EQUIPMENT       = 0x03,
    ASSET_ARTWORK         = 0x04,
    ASSET_COPYRIGHT       = 0x05
};
enum OrderSide   { ORDER_SIDE_BUY = 0x00, ORDER_SIDE_SELL = 0x01 };
enum OrderStatus { ORDER_OPEN = 0x00, ORDER_MATCHED = 0x01, ORDER_CANCELLED = 0x02 };
enum ProposalStatus {
    PROPOSAL_ACTIVE = 0x00, PROPOSAL_PASSED = 0x01,
    PROPOSAL_REJECTED = 0x02, PROPOSAL_EXECUTED = 0x03
};
/* ── Structures ─────────────────────────────────────────────────────── */
typedef struct {
    unsigned int id;
    unsigned int supply;
    unsigned int burned;
    unsigned int staked;
    unsigned int exchange_rate;
    unsigned int inflation_bps;
} Coin;

typedef struct {
    unsigned int id;
    unsigned int balance;
    unsigned int staked;
    unsigned int reputation;
    unsigned int creator_flag;
} User;

typedef struct {
    unsigned int id;
    unsigned int owner;
    unsigned int creator;
    unsigned int asset_type;
    unsigned int value;
    unsigned int royalty_bps;
    unsigned int licensed;
} NftAsset;

typedef struct {
    unsigned int id;
    unsigned int asset_id;
    unsigned int seller;
    unsigned int price;
    unsigned int amount;
    unsigned int side;
    unsigned int status;
} Order;

typedef struct {
    unsigned int id;
    unsigned int proposer;
    unsigned int yes_votes;
    unsigned int no_votes;
    unsigned int quorum;
    unsigned int status;
    unsigned int treasury_delta;
} Proposal;

typedef struct {
    unsigned int total_supply;
    unsigned int total_burned;
    unsigned int total_staked;
    unsigned int treasury;
    unsigned int community_fund;
    unsigned int fee_pool;
    unsigned int user_count;
    unsigned int asset_count;
    unsigned int order_count;
    unsigned int proposal_count;
} EconomyState;
/* ── Global state ───────────────────────────────────────────────────── */
static EconomyState g_economy;
static Coin         g_coins[MAX_COINS];
static User         g_users[MAX_USERS];
static NftAsset     g_assets[MAX_ASSETS];
static Order        g_orders[MAX_ORDERS];
static Proposal     g_proposals[MAX_PROPOSALS];
static unsigned int g_coin_count, g_user_count, g_asset_count, g_order_count, g_proposal_count;
/* ── Forward declarations ───────────────────────────────────────────── */
static void economy_init(void);
static int  currency_create(unsigned int rate, unsigned int inflation_bps);
static void currency_mint(unsigned int cid, unsigned int amt);
static void currency_burn(unsigned int cid, unsigned int amt);
static unsigned int currency_exchange(unsigned int from, unsigned int to, unsigned int amt);
static void staking_deposit(unsigned int uid, unsigned int cid, unsigned int amt);
static unsigned int staking_rewards(unsigned int uid, unsigned int cid);
static int  user_create(unsigned int reputation);
static void user_set_creator(unsigned int uid);
static int  nft_mint(unsigned int owner, unsigned int creator, unsigned int atype,
                     unsigned int val, unsigned int royalty_bps);
static int  nft_transfer(unsigned int aid, unsigned int new_owner);
static unsigned int nft_valuate(unsigned int aid);
static int  order_create(unsigned int aid, unsigned int seller, unsigned int price,
                         unsigned int amt, unsigned int side);
static int  order_match(unsigned int oid, unsigned int buyer, unsigned int qty);
static void order_cancel(unsigned int oid);
static unsigned int order_book_depth(unsigned int aid, unsigned int side);
static void liquidity_provide(unsigned int uid, unsigned int ca, unsigned int cb,
                              unsigned int aa, unsigned int ab);
static int  proposal_create(unsigned int proposer, unsigned int quorum, int tdelta);
static void proposal_vote(unsigned int pid, unsigned int voter, unsigned int weight, int yes);
static int  proposal_finalize(unsigned int pid);
static void proposal_execute(unsigned int pid);
static unsigned int creator_revenue(unsigned int cid, unsigned int aid, unsigned int price);
static int  ip_license(unsigned int aid, unsigned int licensee, unsigned int fee);
static int  lending_issue(unsigned int borrower, unsigned int cid, unsigned int amt);
static int  lending_repay(unsigned int borrower, unsigned int cid, unsigned int amt);
static int  insurance_buy(unsigned int uid, unsigned int aid, unsigned int premium);
static int  insurance_claim(unsigned int uid, unsigned int aid, unsigned int claim);
static unsigned int yield_farm(unsigned int uid, unsigned int cid, unsigned int amt);
static void treasury_deposit(unsigned int amt);
static void treasury_withdraw(unsigned int pid, unsigned int amt);
static void community_fund_grant(unsigned int uid, unsigned int amt);
static unsigned int compute_fee(unsigned int amt);
static void log_event(const char *msg);
/* ── Utility ────────────────────────────────────────────────────────── */
static unsigned int compute_fee(unsigned int amt) { return (amt * FEE_BPS) / 10000; }
static void log_event(const char *msg) { host_print(msg); host_print("\n"); }
/* ── Economy lifecycle ──────────────────────────────────────────────── */
static void economy_init(void) {
    g_economy.total_supply = INITIAL_SUPPLY; g_economy.total_burned = 0;
    g_economy.total_staked = 0; g_economy.treasury = 0;
    g_economy.community_fund = 0; g_economy.fee_pool = 0;
    g_economy.user_count = 0; g_economy.asset_count = 0;
    g_economy.order_count = 0; g_economy.proposal_count = 0;
    g_coin_count = g_user_count = g_asset_count = g_order_count = g_proposal_count = 0;
    log_event("[ECONOMY] initialized");
}
/* ── Currency system ────────────────────────────────────────────────── */
static int currency_create(unsigned int rate, unsigned int inflation_bps) {
    if (g_coin_count >= MAX_COINS) return -1;
    unsigned int id = g_coin_count++;
    g_coins[id].id = id; g_coins[id].supply = 0; g_coins[id].burned = 0;
    g_coins[id].staked = 0; g_coins[id].exchange_rate = rate;
    g_coins[id].inflation_bps = inflation_bps;
    log_event("[CURRENCY] created"); return (int)id;
}
static void currency_mint(unsigned int cid, unsigned int amt) {
    if (cid >= g_coin_count) return;
    g_coins[cid].supply += amt; g_economy.total_supply += amt;
    log_event("[CURRENCY] minted");
}
static void currency_burn(unsigned int cid, unsigned int amt) {
    if (cid >= g_coin_count || g_coins[cid].supply < amt) return;
    g_coins[cid].supply -= amt; g_coins[cid].burned += amt;
    g_economy.total_supply -= amt; g_economy.total_burned += amt;
    log_event("[CURRENCY] burned");
}
static unsigned int currency_exchange(unsigned int from, unsigned int to, unsigned int amt) {
    if (from >= g_coin_count || to >= g_coin_count) return 0;
    if (g_coins[from].supply < amt || g_coins[to].exchange_rate == 0) return 0;
    unsigned int converted = (amt * g_coins[from].exchange_rate) / g_coins[to].exchange_rate;
    unsigned int fee = compute_fee(converted);
    g_coins[from].supply -= amt;
    g_coins[to].supply += (converted - fee); g_economy.fee_pool += fee;
    log_event("[CURRENCY] exchanged"); return converted - fee;
}
static void staking_deposit(unsigned int uid, unsigned int cid, unsigned int amt) {
    if (uid >= g_user_count || cid >= g_coin_count || g_users[uid].balance < amt) return;
    g_users[uid].balance -= amt; g_users[uid].staked += amt;
    g_coins[cid].staked += amt; g_economy.total_staked += amt;
    log_event("[STAKING] deposited");
}
static unsigned int staking_rewards(unsigned int uid, unsigned int cid) {
    if (uid >= g_user_count || cid >= g_coin_count || g_users[uid].staked == 0) return 0;
    unsigned int reward = (g_users[uid].staked * STAKING_APY_BPS) / 10000;
    g_users[uid].balance += reward; currency_mint(cid, reward);
    log_event("[STAKING] rewards claimed"); return reward;
}
/* ── User management ────────────────────────────────────────────────── */
static int user_create(unsigned int reputation) {
    if (g_user_count >= MAX_USERS) return -1;
    unsigned int id = g_user_count++;
    g_users[id].id = id; g_users[id].balance = 0; g_users[id].staked = 0;
    g_users[id].reputation = reputation; g_users[id].creator_flag = 0;
    g_economy.user_count = g_user_count;
    log_event("[USER] created"); return (int)id;
}
static void user_set_creator(unsigned int uid) {
    if (uid >= g_user_count) return;
    g_users[uid].creator_flag = 1;
    log_event("[USER] promoted to creator");
}
/* ── NFT assets ─────────────────────────────────────────────────────── */
static int nft_mint(unsigned int owner, unsigned int creator, unsigned int atype,
                    unsigned int val, unsigned int royalty_bps) {
    if (g_asset_count >= MAX_ASSETS || owner >= g_user_count) return -1;
    unsigned int id = g_asset_count++;
    g_assets[id].id = id; g_assets[id].owner = owner; g_assets[id].creator = creator;
    g_assets[id].asset_type = atype; g_assets[id].value = val;
    g_assets[id].royalty_bps = royalty_bps; g_assets[id].licensed = 0;
    g_economy.asset_count = g_asset_count;
    log_event("[NFT] minted"); return (int)id;
}
static int nft_transfer(unsigned int aid, unsigned int new_owner) {
    if (aid >= g_asset_count || new_owner >= g_user_count) return -1;
    g_assets[aid].owner = new_owner;
    log_event("[NFT] transferred"); return 0;
}
static unsigned int nft_valuate(unsigned int aid) {
    if (aid >= g_asset_count) return 0;
    unsigned int base = g_assets[aid].value;
    return base + (base * g_assets[aid].royalty_bps) / 10000;
}
/* ── Trading market ─────────────────────────────────────────────────── */
static int order_create(unsigned int aid, unsigned int seller, unsigned int price,
                        unsigned int amt, unsigned int side) {
    if (g_order_count >= MAX_ORDERS || aid >= g_asset_count || seller >= g_user_count)
        return -1;
    unsigned int id = g_order_count++;
    g_orders[id].id = id; g_orders[id].asset_id = aid; g_orders[id].seller = seller;
    g_orders[id].price = price; g_orders[id].amount = amt;
    g_orders[id].side = side; g_orders[id].status = ORDER_OPEN;
    g_economy.order_count = g_order_count;
    log_event("[MARKET] order created"); return (int)id;
}
static int order_match(unsigned int oid, unsigned int buyer, unsigned int qty) {
    if (oid >= g_order_count || buyer >= g_user_count) return -1;
    if (g_orders[oid].status != ORDER_OPEN || g_orders[oid].amount < qty) return -1;
    unsigned int total = g_orders[oid].price * qty;
    unsigned int fee   = compute_fee(total);
    if (g_users[buyer].balance < total) return -1;
    g_users[buyer].balance -= total;
    unsigned int seller_id = g_orders[oid].seller;
    unsigned int creator   = g_assets[g_orders[oid].asset_id].creator;
    unsigned int royalty   = (total * g_assets[g_orders[oid].asset_id].royalty_bps) / 10000;
    g_users[seller_id].balance += (total - fee - royalty);
    g_users[creator].balance   += royalty;
    g_economy.fee_pool         += fee;
    g_orders[oid].amount -= qty;
    if (g_orders[oid].amount == 0) g_orders[oid].status = ORDER_MATCHED;
    log_event("[MARKET] order matched"); return 0;
}
static void order_cancel(unsigned int oid) {
    if (oid >= g_order_count) return;
    g_orders[oid].status = ORDER_CANCELLED;
    log_event("[MARKET] order cancelled");
}
static unsigned int order_book_depth(unsigned int aid, unsigned int side) {
    unsigned int depth = 0;
    for (unsigned int i = 0; i < g_order_count; i++) {
        if (g_orders[i].asset_id == aid && g_orders[i].side == side &&
            g_orders[i].status == ORDER_OPEN) depth += g_orders[i].amount;
    }
    return depth;
}
static void liquidity_provide(unsigned int uid, unsigned int ca, unsigned int cb,
                              unsigned int aa, unsigned int ab) {
    if (uid >= g_user_count || ca >= g_coin_count || cb >= g_coin_count) return;
    if (g_coins[ca].supply < aa || g_coins[cb].supply < ab) return;
    g_coins[ca].supply -= aa; g_coins[cb].supply -= ab;
    g_economy.fee_pool += (aa + ab);
    log_event("[MARKET] liquidity provided");
}
/* ── Creator economy ────────────────────────────────────────────────── */
static unsigned int creator_revenue(unsigned int cid, unsigned int aid, unsigned int price) {
    if (cid >= g_user_count || aid >= g_asset_count) return 0;
    unsigned int royalty  = (price * g_assets[aid].royalty_bps) / 10000;
    unsigned int platform = compute_fee(price);
    unsigned int net      = (price > (royalty + platform)) ? (price - royalty - platform) : 0;
    g_users[cid].balance += net; g_economy.fee_pool += platform;
    log_event("[CREATOR] revenue distributed"); return net;
}
static int ip_license(unsigned int aid, unsigned int licensee, unsigned int fee) {
    if (aid >= g_asset_count || licensee >= g_user_count) return -1;
    if (g_users[licensee].balance < fee) return -1;
    g_users[licensee].balance -= fee;
    g_users[g_assets[aid].creator].balance += fee;
    g_assets[aid].licensed = 1;
    log_event("[CREATOR] IP licensed"); return 0;
}
/* ── Financial services ─────────────────────────────────────────────── */
static int lending_issue(unsigned int borrower, unsigned int cid, unsigned int amt) {
    if (borrower >= g_user_count || cid >= g_coin_count || g_coins[cid].supply < amt)
        return -1;
    g_coins[cid].supply -= amt; g_users[borrower].balance += amt;
    log_event("[FINANCE] loan issued"); return 0;
}
static int lending_repay(unsigned int borrower, unsigned int cid, unsigned int amt) {
    if (borrower >= g_user_count || cid >= g_coin_count || g_users[borrower].balance < amt)
        return -1;
    g_users[borrower].balance -= amt; g_coins[cid].supply += amt;
    log_event("[FINANCE] loan repaid"); return 0;
}
static int insurance_buy(unsigned int uid, unsigned int aid, unsigned int premium) {
    if (uid >= g_user_count || aid >= g_asset_count || g_users[uid].balance < premium)
        return -1;
    g_users[uid].balance -= premium; g_economy.treasury += premium;
    log_event("[FINANCE] insurance purchased"); return 0;
}
static int insurance_claim(unsigned int uid, unsigned int aid, unsigned int claim) {
    if (uid >= g_user_count || aid >= g_asset_count || g_economy.treasury < claim)
        return -1;
    g_economy.treasury -= claim; g_users[uid].balance += claim;
    log_event("[FINANCE] insurance claimed"); return 0;
}
static unsigned int yield_farm(unsigned int uid, unsigned int cid, unsigned int amt) {
    if (uid >= g_user_count || cid >= g_coin_count || g_users[uid].balance < amt) return 0;
    unsigned int apy = (amt * 750) / 10000;
    g_users[uid].balance -= amt;
    g_users[uid].balance += (amt + apy);
    currency_mint(cid, apy);
    log_event("[FINANCE] yield farmed"); return apy;
}
/* ── Governance ─────────────────────────────────────────────────────── */
static int proposal_create(unsigned int proposer, unsigned int quorum, int tdelta) {
    if (g_proposal_count >= MAX_PROPOSALS || proposer >= g_user_count) return -1;
    unsigned int id = g_proposal_count++;
    g_proposals[id].id = id; g_proposals[id].proposer = proposer;
    g_proposals[id].yes_votes = 0; g_proposals[id].no_votes = 0;
    g_proposals[id].quorum = quorum; g_proposals[id].status = PROPOSAL_ACTIVE;
    g_proposals[id].treasury_delta = (tdelta > 0) ? (unsigned int)tdelta : 0;
    g_economy.proposal_count = g_proposal_count;
    log_event("[GOV] proposal created"); return (int)id;
}
static void proposal_vote(unsigned int pid, unsigned int voter, unsigned int weight, int yes) {
    if (pid >= g_proposal_count || voter >= g_user_count) return;
    if (g_proposals[pid].status != PROPOSAL_ACTIVE) return;
    if (yes) g_proposals[pid].yes_votes += weight;
    else     g_proposals[pid].no_votes  += weight;
    log_event("[GOV] vote cast");
}
static int proposal_finalize(unsigned int pid) {
    if (pid >= g_proposal_count || g_proposals[pid].status != PROPOSAL_ACTIVE) return -1;
    unsigned int yes = g_proposals[pid].yes_votes, no = g_proposals[pid].no_votes;
    if ((yes + no) < g_proposals[pid].quorum) {
        g_proposals[pid].status = PROPOSAL_REJECTED;
        log_event("[GOV] proposal failed quorum"); return -1;
    }
    if (yes > no) { g_proposals[pid].status = PROPOSAL_PASSED; log_event("[GOV] passed"); return 1; }
    g_proposals[pid].status = PROPOSAL_REJECTED;
    log_event("[GOV] rejected"); return 0;
}
static void proposal_execute(unsigned int pid) {
    if (pid >= g_proposal_count || g_proposals[pid].status != PROPOSAL_PASSED) return;
    unsigned int delta = g_proposals[pid].treasury_delta;
    if (g_economy.treasury >= delta) {
        g_economy.treasury -= delta; g_economy.community_fund += delta;
    }
    g_proposals[pid].status = PROPOSAL_EXECUTED;
    log_event("[GOV] executed");
}
/* ── Treasury & community ───────────────────────────────────────────── */
static void treasury_deposit(unsigned int amt) {
    g_economy.treasury += amt; log_event("[TREASURY] deposit");
}
static void treasury_withdraw(unsigned int pid, unsigned int amt) {
    if (pid >= g_proposal_count || g_proposals[pid].status != PROPOSAL_EXECUTED) return;
    if (g_economy.treasury < amt) return;
    g_economy.treasury -= amt; log_event("[TREASURY] withdrawal");
}
static void community_fund_grant(unsigned int uid, unsigned int amt) {
    if (uid >= g_user_count || g_economy.community_fund < amt) return;
    g_economy.community_fund -= amt; g_users[uid].balance += amt;
    log_event("[COMMUNITY] grant issued");
}
/* ── WASM exports ───────────────────────────────────────────────────── */
__attribute__((export_name("economy_init")))
void wasm_economy_init(void) { economy_init(); }
__attribute__((export_name("create_coin")))
int wasm_create_coin(unsigned int rate, unsigned int inflation) {
    return currency_create(rate, inflation);
}
__attribute__((export_name("mint_coin")))
void wasm_mint_coin(unsigned int cid, unsigned int amt) { currency_mint(cid, amt); }
__attribute__((export_name("burn_coin")))
void wasm_burn_coin(unsigned int cid, unsigned int amt) { currency_burn(cid, amt); }
__attribute__((export_name("exchange_coin")))
unsigned int wasm_exchange_coin(unsigned int from, unsigned int to, unsigned int amt) {
    return currency_exchange(from, to, amt);
}
__attribute__((export_name("stake_deposit")))
void wasm_stake_deposit(unsigned int uid, unsigned int cid, unsigned int amt) {
    staking_deposit(uid, cid, amt);
}
__attribute__((export_name("stake_claim")))
unsigned int wasm_stake_claim(unsigned int uid, unsigned int cid) {
    return staking_rewards(uid, cid);
}
__attribute__((export_name("register_user")))
int wasm_register_user(unsigned int rep) { return user_create(rep); }
__attribute__((export_name("promote_creator")))
void wasm_promote_creator(unsigned int uid) { user_set_creator(uid); }
__attribute__((export_name("mint_nft")))
int wasm_mint_nft(unsigned int owner, unsigned int creator, unsigned int atype,
                  unsigned int val, unsigned int royalty) {
    return nft_mint(owner, creator, atype, val, royalty);
}
__attribute__((export_name("transfer_nft")))
int wasm_transfer_nft(unsigned int aid, unsigned int new_owner) {
    return nft_transfer(aid, new_owner);
}
__attribute__((export_name("value_nft")))
unsigned int wasm_value_nft(unsigned int aid) { return nft_valuate(aid); }
__attribute__((export_name("create_order")))
int wasm_create_order(unsigned int aid, unsigned int seller, unsigned int price,
                      unsigned int amt, unsigned int side) {
    return order_create(aid, seller, price, amt, side);
}
__attribute__((export_name("match_order")))
int wasm_match_order(unsigned int oid, unsigned int buyer, unsigned int qty) {
    return order_match(oid, buyer, qty);
}
__attribute__((export_name("cancel_order")))
void wasm_cancel_order(unsigned int oid) { order_cancel(oid); }
__attribute__((export_name("book_depth")))
unsigned int wasm_book_depth(unsigned int aid, unsigned int side) {
    return order_book_depth(aid, side);
}
__attribute__((export_name("add_liquidity")))
void wasm_add_liquidity(unsigned int uid, unsigned int ca, unsigned int cb,
                        unsigned int aa, unsigned int ab) {
    liquidity_provide(uid, ca, cb, aa, ab);
}
__attribute__((export_name("claim_revenue")))
unsigned int wasm_claim_revenue(unsigned int cid, unsigned int aid, unsigned int price) {
    return creator_revenue(cid, aid, price);
}
__attribute__((export_name("license_ip")))
int wasm_license_ip(unsigned int aid, unsigned int licensee, unsigned int fee) {
    return ip_license(aid, licensee, fee);
}
__attribute__((export_name("issue_loan")))
int wasm_issue_loan(unsigned int borrower, unsigned int cid, unsigned int amt) {
    return lending_issue(borrower, cid, amt);
}
__attribute__((export_name("repay_loan")))
int wasm_repay_loan(unsigned int borrower, unsigned int cid, unsigned int amt) {
    return lending_repay(borrower, cid, amt);
}
__attribute__((export_name("buy_insurance")))
int wasm_buy_insurance(unsigned int uid, unsigned int aid, unsigned int premium) {
    return insurance_buy(uid, aid, premium);
}
__attribute__((export_name("claim_insurance")))
int wasm_claim_insurance(unsigned int uid, unsigned int aid, unsigned int amt) {
    return insurance_claim(uid, aid, amt);
}
__attribute__((export_name("farm_yield")))
unsigned int wasm_farm_yield(unsigned int uid, unsigned int cid, unsigned int amt) {
    return yield_farm(uid, cid, amt);
}
__attribute__((export_name("create_proposal")))
int wasm_create_proposal(unsigned int proposer, unsigned int quorum, int tdelta) {
    return proposal_create(proposer, quorum, tdelta);
}
__attribute__((export_name("vote_proposal")))
void wasm_vote_proposal(unsigned int pid, unsigned int voter, unsigned int weight, int yes) {
    proposal_vote(pid, voter, weight, yes);
}
__attribute__((export_name("finalize_proposal")))
int wasm_finalize_proposal(unsigned int pid) { return proposal_finalize(pid); }
__attribute__((export_name("execute_proposal")))
void wasm_execute_proposal(unsigned int pid) { proposal_execute(pid); }
__attribute__((export_name("deposit_treasury")))
void wasm_deposit_treasury(unsigned int amt) { treasury_deposit(amt); }
__attribute__((export_name("withdraw_treasury")))
void wasm_withdraw_treasury(unsigned int pid, unsigned int amt) {
    treasury_withdraw(pid, amt);
}
__attribute__((export_name("community_grant")))
void wasm_community_grant(unsigned int uid, unsigned int amt) {
    community_fund_grant(uid, amt);
}
__attribute__((export_name("get_total_supply")))
unsigned int wasm_get_total_supply(void) { return g_economy.total_supply; }
__attribute__((export_name("get_total_burned")))
unsigned int wasm_get_total_burned(void) { return g_economy.total_burned; }
__attribute__((export_name("get_treasury")))
unsigned int wasm_get_treasury(void) { return g_economy.treasury; }
__attribute__((export_name("get_fee_pool")))
unsigned int wasm_get_fee_pool(void) { return g_economy.fee_pool; }
__attribute__((export_name("get_user_count")))
unsigned int wasm_get_user_count(void) { return g_economy.user_count; }
__attribute__((export_name("get_asset_count")))
unsigned int wasm_get_asset_count(void) { return g_economy.asset_count; }
__attribute__((export_name("get_order_count")))
unsigned int wasm_get_order_count(void) { return g_economy.order_count; }
__attribute__((export_name("get_proposal_count")))
unsigned int wasm_get_proposal_count(void) { return g_economy.proposal_count; }
/* ── Demo entry point ───────────────────────────────────────────────── */
__attribute__((export_name("run")))
int wasm_run(void) {
    economy_init();
    int c1 = currency_create(1000, 200);
    if (c1 < 0) { host_exit(1); return -1; }
    currency_mint(0, INITIAL_SUPPLY);
    int u1 = user_create(100), u2 = user_create(50);
    if (u1 < 0 || u2 < 0) { host_exit(1); return -1; }
    user_set_creator((unsigned int)u1);
    int nft1 = nft_mint((unsigned int)u1, (unsigned int)u1, ASSET_ARTWORK, 5000, 500);
    if (nft1 < 0) { host_exit(1); return -1; }
    int ord1 = order_create((unsigned int)nft1, (unsigned int)u1, 1000, 1, ORDER_SIDE_SELL);
    if (ord1 < 0) { host_exit(1); return -1; }
    int p1 = proposal_create((unsigned int)u1, 10, 500);
    if (p1 < 0) { host_exit(1); return -1; }
    proposal_vote((unsigned int)p1, (unsigned int)u2, 20, 1);
    proposal_finalize((unsigned int)p1);
    treasury_deposit(10000);
    log_event("[DEMO] metaverse economy simulation complete");
    return 0;
}
