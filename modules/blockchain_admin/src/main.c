/* blockchain_admin: Blockchain administration system (v1.0)
 * Chain infrastructure, consensus mechanisms, smart contracts, digital assets, cross-chain
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

#define MAX_INFRA        16
#define MAX_CONSENSUS    14
#define MAX_CONTRACT     12
#define MAX_ASSET        10
#define MAX_CROSSCHAIN   10

typedef struct {
    int    infra_id;
    int    infra_type;
    int    infra_category;
    int    public_chain;
    int    consortium_chain;
    int    private_chain;
    int    year;
    int    active;
} infra_t;

typedef struct {
    int    consensus_id;
    int    consensus_type;
    int    consensus_category;
    int    pow;
    int    pos;
    int    dpos;
    int    year;
    int    active;
} consensus_t;

typedef struct {
    int    contract_id;
    int    contract_type;
    int    contract_category;
    int    contract_deploy;
    int    contract_audit;
    int    contract_exec;
    int    year;
    int    active;
} contract_t;

typedef struct {
    int    asset_id;
    int    asset_type;
    int    asset_category;
    int    token;
    int    nft;
    int    wallet;
    int    year;
    int    active;
} asset_t;

typedef struct {
    int    cross_id;
    int    cross_type;
    int    cross_category;
    int    cross_bridge;
    int    cross_interop;
    int    cross_settle;
    int    year;
    int    active;
} cross_t;

typedef struct {
    int    n_infra;
    int    n_consensus;
    int    n_contract;
    int    n_asset;
    int    n_cross;
    int    total_public;
    int    total_pow;
    int    total_deploy;
    int    total_token;
    int    total_bridge;
} bca_state_t;

static infra_t infras[MAX_INFRA];
static consensus_t consensuses[MAX_CONSENSUS];
static contract_t contracts[MAX_CONTRACT];
static asset_t assets[MAX_ASSET];
static cross_t crosses[MAX_CROSSCHAIN];
static bca_state_t bca;

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

int bca_init(void) {
    if (initialized) return -1;
    bca.n_infra = 0; bca.n_consensus = 0; bca.n_contract = 0;
    bca.n_asset = 0; bca.n_cross = 0;
    bca.total_public = 0; bca.total_pow = 0;
    bca.total_deploy = 0; bca.total_token = 0;
    bca.total_bridge = 0;
    for (int i = 0; i < MAX_INFRA; i++) infras[i].active = 0;
    for (int i = 0; i < MAX_CONSENSUS; i++) consensuses[i].active = 0;
    for (int i = 0; i < MAX_CONTRACT; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_ASSET; i++) assets[i].active = 0;
    for (int i = 0; i < MAX_CROSSCHAIN; i++) crosses[i].active = 0;
    initialized = 1;
    print_str("[BCA] Blockchain initialized\n");
    return 0;
}

int bca_infra(int inf_type, int cat, int pub, int cons, int priv, int year) {
    if (bca.n_infra >= MAX_INFRA) return -1;
    infra_t* inf = &infras[bca.n_infra];
    inf->infra_id = bca.n_infra;
    inf->infra_type = inf_type;
    inf->infra_category = cat;
    inf->public_chain = pub;
    inf->consortium_chain = cons;
    inf->private_chain = priv;
    inf->year = year;
    inf->active = 1;
    bca.total_public += pub;
    bca.n_infra++;
    print_str("[BCA] Infra "); print_int(bca.n_infra - 1);
    print_str(" type="); print_int(inf_type);
    print_str(" cat="); print_int(cat);
    print_str(" pub="); print_int(pub);
    print_str(" con="); print_int(cons);
    print_str(" prv="); print_int(priv); print_str("\n");
    return bca.n_infra - 1;
}

int bca_consensus(int cs_type, int cat, int pw, int ps, int dp, int year) {
    if (bca.n_consensus >= MAX_CONSENSUS) return -1;
    consensus_t* c = &consensuses[bca.n_consensus];
    c->consensus_id = bca.n_consensus;
    c->consensus_type = cs_type;
    c->consensus_category = cat;
    c->pow = pw;
    c->pos = ps;
    c->dpos = dp;
    c->year = year;
    c->active = 1;
    bca.total_pow += pw;
    bca.n_consensus++;
    print_str("[BCA] Consensus "); print_int(bca.n_consensus - 1);
    print_str(" type="); print_int(cs_type);
    print_str(" cat="); print_int(cat);
    print_str(" pow="); print_int(pw);
    print_str(" pos="); print_int(ps);
    print_str(" dpo="); print_int(dp); print_str("\n");
    return bca.n_consensus - 1;
}

int bca_contract(int ct_type, int cat, int deploy, int audit, int exec, int year) {
    if (bca.n_contract >= MAX_CONTRACT) return -1;
    contract_t* c = &contracts[bca.n_contract];
    c->contract_id = bca.n_contract;
    c->contract_type = ct_type;
    c->contract_category = cat;
    c->contract_deploy = deploy;
    c->contract_audit = audit;
    c->contract_exec = exec;
    c->year = year;
    c->active = 1;
    bca.total_deploy += deploy;
    bca.n_contract++;
    print_str("[BCA] Contract "); print_int(bca.n_contract - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" dpl="); print_int(deploy);
    print_str(" aud="); print_int(audit);
    print_str(" exe="); print_int(exec); print_str("\n");
    return bca.n_contract - 1;
}

int bca_asset(int as_type, int cat, int tkn, int nft_cnt, int wlt, int year) {
    if (bca.n_asset >= MAX_ASSET) return -1;
    asset_t* a = &assets[bca.n_asset];
    a->asset_id = bca.n_asset;
    a->asset_type = as_type;
    a->asset_category = cat;
    a->token = tkn;
    a->nft = nft_cnt;
    a->wallet = wlt;
    a->year = year;
    a->active = 1;
    bca.total_token += tkn;
    bca.n_asset++;
    print_str("[BCA] Asset "); print_int(bca.n_asset - 1);
    print_str(" type="); print_int(as_type);
    print_str(" cat="); print_int(cat);
    print_str(" tkn="); print_int(tkn);
    print_str(" nft="); print_int(nft_cnt);
    print_str(" wlt="); print_int(wlt); print_str("\n");
    return bca.n_asset - 1;
}

int bca_cross(int cr_type, int cat, int bridge, int interop, int settle, int year) {
    if (bca.n_cross >= MAX_CROSSCHAIN) return -1;
    cross_t* c = &crosses[bca.n_cross];
    c->cross_id = bca.n_cross;
    c->cross_type = cr_type;
    c->cross_category = cat;
    c->cross_bridge = bridge;
    c->cross_interop = interop;
    c->cross_settle = settle;
    c->year = year;
    c->active = 1;
    bca.total_bridge += bridge;
    bca.n_cross++;
    print_str("[BCA] Cross "); print_int(bca.n_cross - 1);
    print_str(" type="); print_int(cr_type);
    print_str(" cat="); print_int(cat);
    print_str(" brd="); print_int(bridge);
    print_str(" iop="); print_int(interop);
    print_str(" stl="); print_int(settle); print_str("\n");
    return bca.n_cross - 1;
}

void bca_infra_report(void) {
    print_str("[BCA] Infra report:\n");
    print_str("  Chain infrastructure categories: "); print_int(bca.n_infra); print_str("\n");
    print_str("  Total public chains: "); print_int(bca.total_public); print_str("\n");
}

void bca_consensus_report(void) {
    print_str("[BCA] Consensus report:\n");
    print_str("  Consensus mechanism categories: "); print_int(bca.n_consensus); print_str("\n");
    print_str("  Total PoW: "); print_int(bca.total_pow); print_str("\n");
}

void bca_cross_report(void) {
    print_str("[BCA] Cross-chain report:\n");
    print_str("  Contract categories: "); print_int(bca.n_contract); print_str("\n");
    print_str("  Total contract deploy: "); print_int(bca.total_deploy); print_str("\n");
    print_str("  Asset categories: "); print_int(bca.n_asset); print_str("\n");
    print_str("  Total tokens: "); print_int(bca.total_token); print_str("\n");
    print_str("  Cross-chain categories: "); print_int(bca.n_cross); print_str("\n");
    print_str("  Total cross bridges: "); print_int(bca.total_bridge); print_str("\n");
}

void bca_print_state(void) {
    print_str("[BCA] If="); print_int(bca.n_infra);
    print_str(" Cs="); print_int(bca.n_consensus);
    print_str(" Ct="); print_int(bca.n_contract);
    print_str(" As="); print_int(bca.n_asset);
    print_str(" Cr="); print_int(bca.n_cross);
    print_str("\n");
}

int main(void) {
    print_str("=== Blockchain Admin Demo ===\n\n");
    bca_init();

    print_str("Chain infrastructure...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pub = 45 + (i * 11);
        int con = 30 + (i * 7);
        int prv = 20 + (i * 5);
        int year = 2020 + (i % 5);
        bca_infra(type, cat, pub, con, prv, year);
    }

    print_str("\nConsensus mechanisms...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pw = 35 + (i * 9);
        int ps = 25 + (i * 6);
        int dp = 18 + (i * 4);
        int year = 2021 + (i % 4);
        bca_consensus(type, cat, pw, ps, dp, year);
    }

    print_str("\nSmart contracts...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dpl = 40 + (i * 10);
        int aud = 28 + (i * 7);
        int exe = 20 + (i * 5);
        int year = 2022 + (i % 3);
        bca_contract(type, cat, dpl, aud, exe, year);
    }

    print_str("\nDigital assets...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tkn = 50 + (i * 12);
        int nft = 35 + (i * 8);
        int wlt = 25 + (i * 6);
        int year = 2023 + (i % 2);
        bca_asset(type, cat, tkn, nft, wlt, year);
    }

    print_str("\nCross-chain...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int brd = 30 + (i * 7);
        int iop = 22 + (i * 5);
        int stl = 15 + (i * 3);
        int year = 2024;
        bca_cross(type, cat, brd, iop, stl, year);
    }

    print_str("\nInfra report...\n");
    bca_infra_report();

    print_str("\nConsensus report...\n");
    bca_consensus_report();

    print_str("\nCross-chain report...\n");
    bca_cross_report();

    print_str("\nFinal state...\n");
    bca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
