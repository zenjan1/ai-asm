/* commerce_trade: Commerce and trade administration system (v1.0)
 * Trade management, market building, consumption promotion, service trade, circulation
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

#define MAX_TRADE        16
#define MAX_MARKET_CT    14
#define MAX_CONSUMPTION  12
#define MAX_SERVICE_TR   10
#define MAX_CIRCULATION  10

typedef struct {
    int    trade_id;
    int    trade_type;
    int    trade_region;
    int    trade_volume;
    int    trade_value;
    int    trade_growth;
    int    year;
    int    active;
} trade_t;

typedef struct {
    int    market_id;
    int    market_type;
    int    market_level;
    int    markets_planned;
    int    vendors_registered;
    int    transactions_completed;
    int    year;
    int    active;
} market_ct_t;

typedef struct {
    int    consumption_id;
    int    consumption_type;
    int    promotion_category;
    int    campaigns_launched;
    int    participants_reached;
    int    sales_generated;
    int    year;
    int    active;
} consumption_t;

typedef struct {
    int    service_tr_id;
    int    service_type;
    int    service_mode;
    int    export_volume;
    int    import_volume;
    int    cooperation_projects;
    int    year;
    int    active;
} service_tr_t;

typedef struct {
    int    circulation_id;
    int    circulation_type;
    int    logistics_category;
    int    distribution_centers;
    int    supply_chains_managed;
    int    efficiency_rate;
    int    year;
    int    active;
} circulation_t;

typedef struct {
    int    n_trade;
    int    n_market_ct;
    int    n_consumption;
    int    n_service_tr;
    int    n_circulation;
    int    total_trade_value;
    int    total_vendors;
    int    total_participants;
    int    total_export;
    int    total_centers;
} ct_state_t;

static trade_t trades[MAX_TRADE];
static market_ct_t markets_ct[MAX_MARKET_CT];
static consumption_t consumptions[MAX_CONSUMPTION];
static service_tr_t services_tr[MAX_SERVICE_TR];
static circulation_t circulations[MAX_CIRCULATION];
static ct_state_t ct;

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

int ct_init(void) {
    if (initialized) return -1;
    ct.n_trade = 0; ct.n_market_ct = 0; ct.n_consumption = 0;
    ct.n_service_tr = 0; ct.n_circulation = 0;
    ct.total_trade_value = 0; ct.total_vendors = 0;
    ct.total_participants = 0; ct.total_export = 0;
    ct.total_centers = 0;
    for (int i = 0; i < MAX_TRADE; i++) trades[i].active = 0;
    for (int i = 0; i < MAX_MARKET_CT; i++) markets_ct[i].active = 0;
    for (int i = 0; i < MAX_CONSUMPTION; i++) consumptions[i].active = 0;
    for (int i = 0; i < MAX_SERVICE_TR; i++) services_tr[i].active = 0;
    for (int i = 0; i < MAX_CIRCULATION; i++) circulations[i].active = 0;
    initialized = 1;
    print_str("[CT] Commerce trade initialized\n");
    return 0;
}

int ct_trade(int trade_type, int region, int volume, int value, int growth, int year) {
    if (ct.n_trade >= MAX_TRADE) return -1;
    trade_t* t = &trades[ct.n_trade];
    t->trade_id = ct.n_trade;
    t->trade_type = trade_type;
    t->trade_region = region;
    t->trade_volume = volume;
    t->trade_value = value;
    t->trade_growth = growth;
    t->year = year;
    t->active = 1;
    ct.total_trade_value += value;
    ct.n_trade++;
    print_str("[CT] Trade "); print_int(ct.n_trade - 1);
    print_str(" type="); print_int(trade_type);
    print_str(" rgn="); print_int(region);
    print_str(" vol="); print_int(volume);
    print_str(" val="); print_int(value);
    print_str(" grw="); print_int(growth); print_str("\n");
    return ct.n_trade - 1;
}

int ct_market(int mkt_type, int level, int planned, int vendors, int transactions, int year) {
    if (ct.n_market_ct >= MAX_MARKET_CT) return -1;
    market_ct_t* m = &markets_ct[ct.n_market_ct];
    m->market_id = ct.n_market_ct;
    m->market_type = mkt_type;
    m->market_level = level;
    m->markets_planned = planned;
    m->vendors_registered = vendors;
    m->transactions_completed = transactions;
    m->year = year;
    m->active = 1;
    ct.total_vendors += vendors;
    ct.n_market_ct++;
    print_str("[CT] Market "); print_int(ct.n_market_ct - 1);
    print_str(" type="); print_int(mkt_type);
    print_str(" lvl="); print_int(level);
    print_str(" pln="); print_int(planned);
    print_str(" vnd="); print_int(vendors);
    print_str(" trx="); print_int(transactions); print_str("\n");
    return ct.n_market_ct - 1;
}

int ct_consumption(int cmp_type, int promotion, int campaigns, int participants_cnt, int sales, int year) {
    if (ct.n_consumption >= MAX_CONSUMPTION) return -1;
    consumption_t* c = &consumptions[ct.n_consumption];
    c->consumption_id = ct.n_consumption;
    c->consumption_type = cmp_type;
    c->promotion_category = promotion;
    c->campaigns_launched = campaigns;
    c->participants_reached = participants_cnt;
    c->sales_generated = sales;
    c->year = year;
    c->active = 1;
    ct.total_participants += participants_cnt;
    ct.n_consumption++;
    print_str("[CT] Consumption "); print_int(ct.n_consumption - 1);
    print_str(" type="); print_int(cmp_type);
    print_str(" prm="); print_int(promotion);
    print_str(" cmp="); print_int(campaigns);
    print_str(" pcp="); print_int(participants_cnt);
    print_str(" sls="); print_int(sales); print_str("\n");
    return ct.n_consumption - 1;
}

int ct_service_trade(int svc_type, int mode, int export_vol, int import_vol, int cooperation, int year) {
    if (ct.n_service_tr >= MAX_SERVICE_TR) return -1;
    service_tr_t* st = &services_tr[ct.n_service_tr];
    st->service_tr_id = ct.n_service_tr;
    st->service_type = svc_type;
    st->service_mode = mode;
    st->export_volume = export_vol;
    st->import_volume = import_vol;
    st->cooperation_projects = cooperation;
    st->year = year;
    st->active = 1;
    ct.total_export += export_vol;
    ct.n_service_tr++;
    print_str("[CT] ServiceTrade "); print_int(ct.n_service_tr - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" mode="); print_int(mode);
    print_str(" exp="); print_int(export_vol);
    print_str(" imp="); print_int(import_vol);
    print_str(" cop="); print_int(cooperation); print_str("\n");
    return ct.n_service_tr - 1;
}

int ct_circulation(int circ_type, int logistics, int centers, int supply_chains, int efficiency, int year) {
    if (ct.n_circulation >= MAX_CIRCULATION) return -1;
    circulation_t* cr = &circulations[ct.n_circulation];
    cr->circulation_id = ct.n_circulation;
    cr->circulation_type = circ_type;
    cr->logistics_category = logistics;
    cr->distribution_centers = centers;
    cr->supply_chains_managed = supply_chains;
    cr->efficiency_rate = efficiency;
    cr->year = year;
    cr->active = 1;
    ct.total_centers += centers;
    ct.n_circulation++;
    print_str("[CT] Circulation "); print_int(ct.n_circulation - 1);
    print_str(" type="); print_int(circ_type);
    print_str(" log="); print_int(logistics);
    print_str(" ctr="); print_int(centers);
    print_str(" scm="); print_int(supply_chains);
    print_str(" eff="); print_int(efficiency); print_str("\n");
    return ct.n_circulation - 1;
}

void ct_trade_report(void) {
    print_str("[CT] Trade report:\n");
    print_str("  Trade categories: "); print_int(ct.n_trade); print_str("\n");
    print_str("  Total trade value: "); print_int(ct.total_trade_value); print_str("\n");
}

void ct_market_report(void) {
    print_str("[CT] Market report:\n");
    print_str("  Market categories: "); print_int(ct.n_market_ct); print_str("\n");
    print_str("  Total vendors registered: "); print_int(ct.total_vendors); print_str("\n");
}

void ct_circulation_report(void) {
    print_str("[CT] Circulation report:\n");
    print_str("  Consumption campaigns: "); print_int(ct.n_consumption); print_str("\n");
    print_str("  Total participants: "); print_int(ct.total_participants); print_str("\n");
    print_str("  Service trade units: "); print_int(ct.n_service_tr); print_str("\n");
    print_str("  Total export volume: "); print_int(ct.total_export); print_str("\n");
    print_str("  Circulation units: "); print_int(ct.n_circulation); print_str("\n");
    print_str("  Total distribution centers: "); print_int(ct.total_centers); print_str("\n");
}

void ct_print_state(void) {
    print_str("[CT] Tr="); print_int(ct.n_trade);
    print_str(" Mk="); print_int(ct.n_market_ct);
    print_str(" Cm="); print_int(ct.n_consumption);
    print_str(" St="); print_int(ct.n_service_tr);
    print_str(" Cr="); print_int(ct.n_circulation);
    print_str("\n");
}

int main(void) {
    print_str("=== Commerce Trade Demo ===\n\n");
    ct_init();

    print_str("Trade management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int vol = 1000 + (i * 200);
        int val = 5000 + (i * 1000);
        int grw = 5 + (i % 15);
        int year = 2020 + (i % 5);
        ct_trade(type, rgn, vol, val, grw, year);
    }

    print_str("\nMarket building...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 5) + 1;
        int pln = 20 + (i * 5);
        int vnd = 500 + (i * 100);
        int trx = 1000 + (i * 200);
        int year = 2021 + (i % 4);
        ct_market(type, lvl, pln, vnd, trx, year);
    }

    print_str("\nConsumption promotion...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int prm = (i % 5) + 1;
        int cmp = 10 + (i * 3);
        int pcp = 5000 + (i * 1200);
        int sls = 10000 + (i * 2500);
        int year = 2022 + (i % 3);
        ct_consumption(type, prm, cmp, pcp, sls, year);
    }

    print_str("\nService trade...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int mode = (i % 3) + 1;
        int exp = 2000 + (i * 500);
        int imp = 1500 + (i * 400);
        int cop = 20 + (i * 5);
        int year = 2023 + (i % 2);
        ct_service_trade(type, mode, exp, imp, cop, year);
    }

    print_str("\nCirculation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int log = (i % 5) + 1;
        int ctr = 15 + (i * 4);
        int scm = 30 + (i * 8);
        int eff = 80 + (i % 20);
        int year = 2024;
        ct_circulation(type, log, ctr, scm, eff, year);
    }

    print_str("\nTrade report...\n");
    ct_trade_report();

    print_str("\nMarket report...\n");
    ct_market_report();

    print_str("\nCirculation report...\n");
    ct_circulation_report();

    print_str("\nFinal state...\n");
    ct_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
