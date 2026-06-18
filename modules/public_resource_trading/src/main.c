/* public_resource_trading: Public resource trading administration system (v1.0)
 * Land trading, mining rights, state assets, engineering bidding, franchise rights
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

#define MAX_LAND         16
#define MAX_MINING       14
#define MAX_STATE_ASSET  12
#define MAX_ENGINEERING  10
#define MAX_FRANCHISE    10

typedef struct {
    int    land_id;
    int    land_type;
    int    land_category;
    int    land_transfers;
    int    land_leases;
    int    land_conveyances;
    int    year;
    int    active;
} land_trading_t;

typedef struct {
    int    mining_id;
    int    mining_type;
    int    mineral_category;
    int    exploration_rights;
    int    extraction_rights;
    int    mining_right_transfers;
    int    year;
    int    active;
} mining_rights_t;

typedef struct {
    int    asset_id;
    int    asset_type;
    int    asset_category;
    int    asset_transfers;
    int    asset_leases;
    int    asset_disposals;
    int    year;
    int    active;
} state_asset_t;

typedef struct {
    int    engineering_id;
    int    project_type;
    int    project_category;
    int    project_bids;
    int    project_tenders;
    int    project_evaluations;
    int    year;
    int    active;
} engineering_t;

typedef struct {
    int    franchise_id;
    int    franchise_type;
    int    franchise_category;
    int    franchise_grants;
    int    franchise_transfers;
    int    franchise_supervisions;
    int    year;
    int    active;
} franchise_t;

typedef struct {
    int    n_land;
    int    n_mining;
    int    n_state_asset;
    int    n_engineering;
    int    n_franchise;
    int    total_land;
    int    total_mining;
    int    total_assets;
    int    total_engineering;
    int    total_franchise;
} prt_state_t;

static land_trading_t lands[MAX_LAND];
static mining_rights_t minings[MAX_MINING];
static state_asset_t state_assets[MAX_STATE_ASSET];
static engineering_t engineerings[MAX_ENGINEERING];
static franchise_t franchises[MAX_FRANCHISE];
static prt_state_t prt;

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

int prt_init(void) {
    if (initialized) return -1;
    prt.n_land = 0; prt.n_mining = 0; prt.n_state_asset = 0;
    prt.n_engineering = 0; prt.n_franchise = 0;
    prt.total_land = 0; prt.total_mining = 0;
    prt.total_assets = 0; prt.total_engineering = 0;
    prt.total_franchise = 0;
    for (int i = 0; i < MAX_LAND; i++) lands[i].active = 0;
    for (int i = 0; i < MAX_MINING; i++) minings[i].active = 0;
    for (int i = 0; i < MAX_STATE_ASSET; i++) state_assets[i].active = 0;
    for (int i = 0; i < MAX_ENGINEERING; i++) engineerings[i].active = 0;
    for (int i = 0; i < MAX_FRANCHISE; i++) franchises[i].active = 0;
    initialized = 1;
    print_str("[PRT] Public resource trading initialized\n");
    return 0;
}

int prt_land(int land_type, int land_cat, int transfers, int leases, int conveyances, int year) {
    if (prt.n_land >= MAX_LAND) return -1;
    land_trading_t* l = &lands[prt.n_land];
    l->land_id = prt.n_land;
    l->land_type = land_type;
    l->land_category = land_cat;
    l->land_transfers = transfers;
    l->land_leases = leases;
    l->land_conveyances = conveyances;
    l->year = year;
    l->active = 1;
    prt.total_land += transfers;
    prt.n_land++;
    print_str("[PRT] Land "); print_int(prt.n_land - 1);
    print_str(" type="); print_int(land_type);
    print_str(" cat="); print_int(land_cat);
    print_str(" trf="); print_int(transfers);
    print_str(" lse="); print_int(leases);
    print_str(" cnv="); print_int(conveyances); print_str("\n");
    return prt.n_land - 1;
}

int prt_mining(int min_type, int mineral, int exploration, int extraction, int transfers, int year) {
    if (prt.n_mining >= MAX_MINING) return -1;
    mining_rights_t* m = &minings[prt.n_mining];
    m->mining_id = prt.n_mining;
    m->mining_type = min_type;
    m->mineral_category = mineral;
    m->exploration_rights = exploration;
    m->extraction_rights = extraction;
    m->mining_right_transfers = transfers;
    m->year = year;
    m->active = 1;
    prt.total_mining += exploration;
    prt.n_mining++;
    print_str("[PRT] Mining "); print_int(prt.n_mining - 1);
    print_str(" type="); print_int(min_type);
    print_str(" min="); print_int(mineral);
    print_str(" exp="); print_int(exploration);
    print_str(" ext="); print_int(extraction);
    print_str(" trf="); print_int(transfers); print_str("\n");
    return prt.n_mining - 1;
}

int prt_assets(int asset_type, int asset_cat, int transfers, int leases, int disposals, int year) {
    if (prt.n_state_asset >= MAX_STATE_ASSET) return -1;
    state_asset_t* a = &state_assets[prt.n_state_asset];
    a->asset_id = prt.n_state_asset;
    a->asset_type = asset_type;
    a->asset_category = asset_cat;
    a->asset_transfers = transfers;
    a->asset_leases = leases;
    a->asset_disposals = disposals;
    a->year = year;
    a->active = 1;
    prt.total_assets += transfers;
    prt.n_state_asset++;
    print_str("[PRT] Asset "); print_int(prt.n_state_asset - 1);
    print_str(" type="); print_int(asset_type);
    print_str(" cat="); print_int(asset_cat);
    print_str(" trf="); print_int(transfers);
    print_str(" lse="); print_int(leases);
    print_str(" dsp="); print_int(disposals); print_str("\n");
    return prt.n_state_asset - 1;
}

int prt_engineering(int proj_type, int proj_cat, int bids, int tenders, int evaluations, int year) {
    if (prt.n_engineering >= MAX_ENGINEERING) return -1;
    engineering_t* e = &engineerings[prt.n_engineering];
    e->engineering_id = prt.n_engineering;
    e->project_type = proj_type;
    e->project_category = proj_cat;
    e->project_bids = bids;
    e->project_tenders = tenders;
    e->project_evaluations = evaluations;
    e->year = year;
    e->active = 1;
    prt.total_engineering += bids;
    prt.n_engineering++;
    print_str("[PRT] Engineering "); print_int(prt.n_engineering - 1);
    print_str(" type="); print_int(proj_type);
    print_str(" cat="); print_int(proj_cat);
    print_str(" bid="); print_int(bids);
    print_str(" tnd="); print_int(tenders);
    print_str(" evl="); print_int(evaluations); print_str("\n");
    return prt.n_engineering - 1;
}

int prt_franchise(int fr_type, int fr_cat, int grants, int transfers, int supervisions, int year) {
    if (prt.n_franchise >= MAX_FRANCHISE) return -1;
    franchise_t* f = &franchises[prt.n_franchise];
    f->franchise_id = prt.n_franchise;
    f->franchise_type = fr_type;
    f->franchise_category = fr_cat;
    f->franchise_grants = grants;
    f->franchise_transfers = transfers;
    f->franchise_supervisions = supervisions;
    f->year = year;
    f->active = 1;
    prt.total_franchise += grants;
    prt.n_franchise++;
    print_str("[PRT] Franchise "); print_int(prt.n_franchise - 1);
    print_str(" type="); print_int(fr_type);
    print_str(" cat="); print_int(fr_cat);
    print_str(" grt="); print_int(grants);
    print_str(" trf="); print_int(transfers);
    print_str(" sup="); print_int(supervisions); print_str("\n");
    return prt.n_franchise - 1;
}

void prt_land_report(void) {
    print_str("[PRT] Land report:\n");
    print_str("  Land trading categories: "); print_int(prt.n_land); print_str("\n");
    print_str("  Total land transfers: "); print_int(prt.total_land); print_str("\n");
}

void prt_mining_report(void) {
    print_str("[PRT] Mining report:\n");
    print_str("  Mining right categories: "); print_int(prt.n_mining); print_str("\n");
    print_str("  Total exploration rights: "); print_int(prt.total_mining); print_str("\n");
}

void prt_franchise_report(void) {
    print_str("[PRT] Franchise report:\n");
    print_str("  State asset categories: "); print_int(prt.n_state_asset); print_str("\n");
    print_str("  Total asset transfers: "); print_int(prt.total_assets); print_str("\n");
    print_str("  Engineering project categories: "); print_int(prt.n_engineering); print_str("\n");
    print_str("  Total project bids: "); print_int(prt.total_engineering); print_str("\n");
    print_str("  Franchise categories: "); print_int(prt.n_franchise); print_str("\n");
    print_str("  Total franchise grants: "); print_int(prt.total_franchise); print_str("\n");
}

void prt_print_state(void) {
    print_str("[PRT] Ld="); print_int(prt.n_land);
    print_str(" Mn="); print_int(prt.n_mining);
    print_str(" Sa="); print_int(prt.n_state_asset);
    print_str(" En="); print_int(prt.n_engineering);
    print_str(" Fr="); print_int(prt.n_franchise);
    print_str("\n");
}

int main(void) {
    print_str("=== Public Resource Trading Demo ===\n\n");
    prt_init();

    print_str("Land trading...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int trf = 10 + (i * 3);
        int lse = 20 + (i * 5);
        int cnv = 5 + (i * 2);
        int year = 2020 + (i % 5);
        prt_land(type, cat, trf, lse, cnv, year);
    }

    print_str("\nMining rights...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int min = (i % 5) + 1;
        int exp = 8 + (i * 2);
        int ext = 12 + (i * 3);
        int trf = 3 + (i % 4);
        int year = 2021 + (i % 4);
        prt_mining(type, min, exp, ext, trf, year);
    }

    print_str("\nState assets...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trf = 15 + (i * 4);
        int lse = 25 + (i * 6);
        int dsp = 5 + (i * 2);
        int year = 2022 + (i % 3);
        prt_assets(type, cat, trf, lse, dsp, year);
    }

    print_str("\nEngineering bidding...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bid = 20 + (i * 5);
        int tnd = 40 + (i * 10);
        int evl = 15 + (i * 4);
        int year = 2023 + (i % 2);
        prt_engineering(type, cat, bid, tnd, evl, year);
    }

    print_str("\nFranchise rights...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grt = 10 + (i * 3);
        int trf = 5 + (i * 2);
        int sup = 8 + (i * 2);
        int year = 2024;
        prt_franchise(type, cat, grt, trf, sup, year);
    }

    print_str("\nLand report...\n");
    prt_land_report();

    print_str("\nMining report...\n");
    prt_mining_report();

    print_str("\nFranchise report...\n");
    prt_franchise_report();

    print_str("\nFinal state...\n");
    prt_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
