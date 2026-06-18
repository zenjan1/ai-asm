/* government_procurement: Government procurement administration system (v1.0)
 * Procurement plans, bidding management, contract management, supplier management, supervision
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

#define MAX_PROC_PLAN    16
#define MAX_BIDDING      14
#define MAX_CONTRACT     12
#define MAX_SUPPLIER     10
#define MAX_GP_SUPER     10

typedef struct {
    int    plan_id;
    int    plan_type;
    int    procurement_level;
    int    annual_plans;
    int    central_catalog;
    int    procurement_budget;
    int    year;
    int    active;
} proc_plan_t;

typedef struct {
    int    bidding_id;
    int    bidding_type;
    int    bidding_method;
    int    public_bids;
    int    invited_bids;
    int    competitive_negotiations;
    int    year;
    int    active;
} bidding_t;

typedef struct {
    int    contract_id;
    int    contract_type;
    int    contract_category;
    int    contracts_signed;
    int    contracts_fulfilled;
    int    contracts_accepted;
    int    year;
    int    active;
} contract_t;

typedef struct {
    int    supplier_id;
    int    supplier_type;
    int    supplier_category;
    int    suppliers_registered;
    int    qualifications_reviewed;
    int    performance_evaluated;
    int    year;
    int    active;
} supplier_t;

typedef struct {
    int    supervision_id;
    int    supervision_type;
    int    supervision_area;
    int    procurement_supervisors;
    int    complaints_handled;
    int    violations_penalized;
    int    year;
    int    active;
} gp_supervision_t;

typedef struct {
    int    n_plan;
    int    n_bidding;
    int    n_contract;
    int    n_supplier;
    int    n_supervision;
    int    total_plans;
    int    total_bids;
    int    total_contracts;
    int    total_suppliers;
    int    total_supervisions;
} gp_state_t;

static proc_plan_t plans[MAX_PROC_PLAN];
static bidding_t biddings[MAX_BIDDING];
static contract_t contracts[MAX_CONTRACT];
static supplier_t suppliers[MAX_SUPPLIER];
static gp_supervision_t supervisions[MAX_GP_SUPER];
static gp_state_t gp;

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

int gp_init(void) {
    if (initialized) return -1;
    gp.n_plan = 0; gp.n_bidding = 0; gp.n_contract = 0;
    gp.n_supplier = 0; gp.n_supervision = 0;
    gp.total_plans = 0; gp.total_bids = 0;
    gp.total_contracts = 0; gp.total_suppliers = 0;
    gp.total_supervisions = 0;
    for (int i = 0; i < MAX_PROC_PLAN; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_BIDDING; i++) biddings[i].active = 0;
    for (int i = 0; i < MAX_CONTRACT; i++) contracts[i].active = 0;
    for (int i = 0; i < MAX_SUPPLIER; i++) suppliers[i].active = 0;
    for (int i = 0; i < MAX_GP_SUPER; i++) supervisions[i].active = 0;
    initialized = 1;
    print_str("[GP] Government procurement initialized\n");
    return 0;
}

int gp_plan(int plan_type, int level, int annual, int catalog, int budget, int year) {
    if (gp.n_plan >= MAX_PROC_PLAN) return -1;
    proc_plan_t* p = &plans[gp.n_plan];
    p->plan_id = gp.n_plan;
    p->plan_type = plan_type;
    p->procurement_level = level;
    p->annual_plans = annual;
    p->central_catalog = catalog;
    p->procurement_budget = budget;
    p->year = year;
    p->active = 1;
    gp.total_plans += annual;
    gp.n_plan++;
    print_str("[GP] Plan "); print_int(gp.n_plan - 1);
    print_str(" type="); print_int(plan_type);
    print_str(" lvl="); print_int(level);
    print_str(" ann="); print_int(annual);
    print_str(" cat="); print_int(catalog);
    print_str(" bgt="); print_int(budget); print_str("\n");
    return gp.n_plan - 1;
}

int gp_bidding(int bid_type, int method, int public_cnt, int invited, int competitive, int year) {
    if (gp.n_bidding >= MAX_BIDDING) return -1;
    bidding_t* b = &biddings[gp.n_bidding];
    b->bidding_id = gp.n_bidding;
    b->bidding_type = bid_type;
    b->bidding_method = method;
    b->public_bids = public_cnt;
    b->invited_bids = invited;
    b->competitive_negotiations = competitive;
    b->year = year;
    b->active = 1;
    gp.total_bids += public_cnt;
    gp.n_bidding++;
    print_str("[GP] Bidding "); print_int(gp.n_bidding - 1);
    print_str(" type="); print_int(bid_type);
    print_str(" mth="); print_int(method);
    print_str(" pub="); print_int(public_cnt);
    print_str(" inv="); print_int(invited);
    print_str(" cmp="); print_int(competitive); print_str("\n");
    return gp.n_bidding - 1;
}

int gp_contract(int con_type, int con_cat, int signed_cnt, int fulfilled, int accepted, int year) {
    if (gp.n_contract >= MAX_CONTRACT) return -1;
    contract_t* c = &contracts[gp.n_contract];
    c->contract_id = gp.n_contract;
    c->contract_type = con_type;
    c->contract_category = con_cat;
    c->contracts_signed = signed_cnt;
    c->contracts_fulfilled = fulfilled;
    c->contracts_accepted = accepted;
    c->year = year;
    c->active = 1;
    gp.total_contracts += signed_cnt;
    gp.n_contract++;
    print_str("[GP] Contract "); print_int(gp.n_contract - 1);
    print_str(" type="); print_int(con_type);
    print_str(" cat="); print_int(con_cat);
    print_str(" sgn="); print_int(signed_cnt);
    print_str(" flf="); print_int(fulfilled);
    print_str(" acc="); print_int(accepted); print_str("\n");
    return gp.n_contract - 1;
}

int gp_supplier(int sup_type, int sup_cat, int registered, int reviewed, int evaluated, int year) {
    if (gp.n_supplier >= MAX_SUPPLIER) return -1;
    supplier_t* s = &suppliers[gp.n_supplier];
    s->supplier_id = gp.n_supplier;
    s->supplier_type = sup_type;
    s->supplier_category = sup_cat;
    s->suppliers_registered = registered;
    s->qualifications_reviewed = reviewed;
    s->performance_evaluated = evaluated;
    s->year = year;
    s->active = 1;
    gp.total_suppliers += registered;
    gp.n_supplier++;
    print_str("[GP] Supplier "); print_int(gp.n_supplier - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" cat="); print_int(sup_cat);
    print_str(" reg="); print_int(registered);
    print_str(" rvw="); print_int(reviewed);
    print_str(" evl="); print_int(evaluated); print_str("\n");
    return gp.n_supplier - 1;
}

int gp_supervision(int sup_type, int area, int supervisors, int complaints, int penalties, int year) {
    if (gp.n_supervision >= MAX_GP_SUPER) return -1;
    gp_supervision_t* sv = &supervisions[gp.n_supervision];
    sv->supervision_id = gp.n_supervision;
    sv->supervision_type = sup_type;
    sv->supervision_area = area;
    sv->procurement_supervisors = supervisors;
    sv->complaints_handled = complaints;
    sv->violations_penalized = penalties;
    sv->year = year;
    sv->active = 1;
    gp.total_supervisions += supervisors;
    gp.n_supervision++;
    print_str("[GP] Supervision "); print_int(gp.n_supervision - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" area="); print_int(area);
    print_str(" sup="); print_int(supervisors);
    print_str(" cmp="); print_int(complaints);
    print_str(" pnl="); print_int(penalties); print_str("\n");
    return gp.n_supervision - 1;
}

void gp_plan_report(void) {
    print_str("[GP] Plan report:\n");
    print_str("  Procurement plan categories: "); print_int(gp.n_plan); print_str("\n");
    print_str("  Total annual plans: "); print_int(gp.total_plans); print_str("\n");
}

void gp_bidding_report(void) {
    print_str("[GP] Bidding report:\n");
    print_str("  Bidding categories: "); print_int(gp.n_bidding); print_str("\n");
    print_str("  Total public bids: "); print_int(gp.total_bids); print_str("\n");
}

void gp_supervision_report(void) {
    print_str("[GP] Supervision report:\n");
    print_str("  Contract categories: "); print_int(gp.n_contract); print_str("\n");
    print_str("  Total contracts signed: "); print_int(gp.total_contracts); print_str("\n");
    print_str("  Supplier categories: "); print_int(gp.n_supplier); print_str("\n");
    print_str("  Total suppliers registered: "); print_int(gp.total_suppliers); print_str("\n");
    print_str("  Supervision categories: "); print_int(gp.n_supervision); print_str("\n");
    print_str("  Total supervisors: "); print_int(gp.total_supervisions); print_str("\n");
}

void gp_print_state(void) {
    print_str("[GP] Pl="); print_int(gp.n_plan);
    print_str(" Bd="); print_int(gp.n_bidding);
    print_str(" Cn="); print_int(gp.n_contract);
    print_str(" Sp="); print_int(gp.n_supplier);
    print_str(" Sv="); print_int(gp.n_supervision);
    print_str("\n");
}

int main(void) {
    print_str("=== Government Procurement Demo ===\n\n");
    gp_init();

    print_str("Procurement plans...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int lvl = (i % 4) + 1;
        int ann = 40 + (i * 10);
        int cat = 20 + (i * 5);
        int bgt = 1000 + (i * 250);
        int year = 2020 + (i % 5);
        gp_plan(type, lvl, ann, cat, bgt, year);
    }

    print_str("\nBidding management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int mth = (i % 3) + 1;
        int pub = 30 + (i * 8);
        int inv = 15 + (i * 4);
        int cmp = 10 + (i * 3);
        int year = 2021 + (i % 4);
        gp_bidding(type, mth, pub, inv, cmp, year);
    }

    print_str("\nContract management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sgn = 25 + (i * 6);
        int flf = 20 + (i * 5);
        int acc = 18 + (i * 4);
        int year = 2022 + (i % 3);
        gp_contract(type, cat, sgn, flf, acc, year);
    }

    print_str("\nSupplier management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int reg = 50 + (i * 12);
        int rvw = 40 + (i * 10);
        int evl = 30 + (i * 8);
        int year = 2023 + (i % 2);
        gp_supplier(type, cat, reg, rvw, evl, year);
    }

    print_str("\nProcurement supervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 5) + 1;
        int sup = 20 + (i * 5);
        int cmp = 15 + (i * 4);
        int pnl = 5 + (i * 2);
        int year = 2024;
        gp_supervision(type, area, sup, cmp, pnl, year);
    }

    print_str("\nPlan report...\n");
    gp_plan_report();

    print_str("\nBidding report...\n");
    gp_bidding_report();

    print_str("\nSupervision report...\n");
    gp_supervision_report();

    print_str("\nFinal state...\n");
    gp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
