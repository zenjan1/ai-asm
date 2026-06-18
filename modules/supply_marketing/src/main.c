/* supply_marketing: Supply and marketing cooperative administration system (v1.0)
 * Supply management, sales, cooperation, storage logistics, comprehensive services
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

#define MAX_SUPPLY       16
#define MAX_SALES        14
#define MAX_COOPERATION  12
#define MAX_LOGISTICS    10
#define MAX_COMPREHENSIVE 10

typedef struct {
    int    supply_id;
    int    supply_type;
    int    material_category;
    int    supplies_distributed;
    int    agricultural_materials;
    int    daily_necessities;
    int    year;
    int    active;
} supply_t;

typedef struct {
    int    sales_id;
    int    sales_channel;
    int    product_category;
    int    products_sold;
    int    sales_volume;
    int    brand_value;
    int    year;
    int    active;
} sales_t;

typedef struct {
    int    cooperation_id;
    int    coop_type;
    int    member_category;
    int    cooperatives_registered;
    int    members_enrolled;
    int    profit_distributed;
    int    year;
    int    active;
} cooperation_t;

typedef struct {
    int    logistics_id;
    int    logistics_type;
    int    storage_category;
    int    warehouses_operated;
    int    cold_chain_capacity;
    int    deliveries_completed;
    int    year;
    int    active;
} logistics_t;

typedef struct {
    int    comprehensive_id;
    int    service_type;
    int    service_domain;
    int    services_provided;
    int    technical_support;
    int    financial_services;
    int    year;
    int    active;
} comprehensive_t;

typedef struct {
    int    n_supply;
    int    n_sales;
    int    n_cooperation;
    int    n_logistics;
    int    n_comprehensive;
    int    total_supplies;
    int    total_sales;
    int    total_cooperatives;
    int    total_warehouses;
    int    total_services;
} sm_state_t;

static supply_t supplies[MAX_SUPPLY];
static sales_t sales_list[MAX_SALES];
static cooperation_t cooperations[MAX_COOPERATION];
static logistics_t logistics_list[MAX_LOGISTICS];
static comprehensive_t comprehensives[MAX_COMPREHENSIVE];
static sm_state_t sm;

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

int sm_init(void) {
    if (initialized) return -1;
    sm.n_supply = 0; sm.n_sales = 0; sm.n_cooperation = 0;
    sm.n_logistics = 0; sm.n_comprehensive = 0;
    sm.total_supplies = 0; sm.total_sales = 0;
    sm.total_cooperatives = 0; sm.total_warehouses = 0;
    sm.total_services = 0;
    for (int i = 0; i < MAX_SUPPLY; i++) supplies[i].active = 0;
    for (int i = 0; i < MAX_SALES; i++) sales_list[i].active = 0;
    for (int i = 0; i < MAX_COOPERATION; i++) cooperations[i].active = 0;
    for (int i = 0; i < MAX_LOGISTICS; i++) logistics_list[i].active = 0;
    for (int i = 0; i < MAX_COMPREHENSIVE; i++) comprehensives[i].active = 0;
    initialized = 1;
    print_str("[SM] Supply & marketing initialized\n");
    return 0;
}

int sm_supply(int sup_type, int material, int distributed, int agri_mat, int daily, int year) {
    if (sm.n_supply >= MAX_SUPPLY) return -1;
    supply_t* s = &supplies[sm.n_supply];
    s->supply_id = sm.n_supply;
    s->supply_type = sup_type;
    s->material_category = material;
    s->supplies_distributed = distributed;
    s->agricultural_materials = agri_mat;
    s->daily_necessities = daily;
    s->year = year;
    s->active = 1;
    sm.total_supplies += distributed;
    sm.n_supply++;
    print_str("[SM] Supply "); print_int(sm.n_supply - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" mat="); print_int(material);
    print_str(" dst="); print_int(distributed);
    print_str(" agr="); print_int(agri_mat);
    print_str(" dly="); print_int(daily); print_str("\n");
    return sm.n_supply - 1;
}

int sm_sales(int channel, int product, int sold, int volume, int brand, int year) {
    if (sm.n_sales >= MAX_SALES) return -1;
    sales_t* sl = &sales_list[sm.n_sales];
    sl->sales_id = sm.n_sales;
    sl->sales_channel = channel;
    sl->product_category = product;
    sl->products_sold = sold;
    sl->sales_volume = volume;
    sl->brand_value = brand;
    sl->year = year;
    sl->active = 1;
    sm.total_sales += sold;
    sm.n_sales++;
    print_str("[SM] Sales "); print_int(sm.n_sales - 1);
    print_str(" chn="); print_int(channel);
    print_str(" prd="); print_int(product);
    print_str(" sld="); print_int(sold);
    print_str(" vol="); print_int(volume);
    print_str(" brd="); print_int(brand); print_str("\n");
    return sm.n_sales - 1;
}

int sm_cooperation(int coop_type, int member_cat, int registered, int enrolled, int profit, int year) {
    if (sm.n_cooperation >= MAX_COOPERATION) return -1;
    cooperation_t* c = &cooperations[sm.n_cooperation];
    c->cooperation_id = sm.n_cooperation;
    c->coop_type = coop_type;
    c->member_category = member_cat;
    c->cooperatives_registered = registered;
    c->members_enrolled = enrolled;
    c->profit_distributed = profit;
    c->year = year;
    c->active = 1;
    sm.total_cooperatives += registered;
    sm.n_cooperation++;
    print_str("[SM] Cooperation "); print_int(sm.n_cooperation - 1);
    print_str(" type="); print_int(coop_type);
    print_str(" cat="); print_int(member_cat);
    print_str(" reg="); print_int(registered);
    print_str(" enr="); print_int(enrolled);
    print_str(" prf="); print_int(profit); print_str("\n");
    return sm.n_cooperation - 1;
}

int sm_logistics(int log_type, int storage, int warehouses_cnt, int cold_chain, int deliveries, int year) {
    if (sm.n_logistics >= MAX_LOGISTICS) return -1;
    logistics_t* l = &logistics_list[sm.n_logistics];
    l->logistics_id = sm.n_logistics;
    l->logistics_type = log_type;
    l->storage_category = storage;
    l->warehouses_operated = warehouses_cnt;
    l->cold_chain_capacity = cold_chain;
    l->deliveries_completed = deliveries;
    l->year = year;
    l->active = 1;
    sm.total_warehouses += warehouses_cnt;
    sm.n_logistics++;
    print_str("[SM] Logistics "); print_int(sm.n_logistics - 1);
    print_str(" type="); print_int(log_type);
    print_str(" str="); print_int(storage);
    print_str(" whs="); print_int(warehouses_cnt);
    print_str(" cld="); print_int(cold_chain);
    print_str(" dlv="); print_int(deliveries); print_str("\n");
    return sm.n_logistics - 1;
}

int sm_comprehensive(int svc_type, int domain, int provided, int technical, int financial, int year) {
    if (sm.n_comprehensive >= MAX_COMPREHENSIVE) return -1;
    comprehensive_t* cp = &comprehensives[sm.n_comprehensive];
    cp->comprehensive_id = sm.n_comprehensive;
    cp->service_type = svc_type;
    cp->service_domain = domain;
    cp->services_provided = provided;
    cp->technical_support = technical;
    cp->financial_services = financial;
    cp->year = year;
    cp->active = 1;
    sm.total_services += provided;
    sm.n_comprehensive++;
    print_str("[SM] Comprehensive "); print_int(sm.n_comprehensive - 1);
    print_str(" type="); print_int(svc_type);
    print_str(" dom="); print_int(domain);
    print_str(" prd="); print_int(provided);
    print_str(" tch="); print_int(technical);
    print_str(" fin="); print_int(financial); print_str("\n");
    return sm.n_comprehensive - 1;
}

void sm_supply_report(void) {
    print_str("[SM] Supply report:\n");
    print_str("  Supply categories: "); print_int(sm.n_supply); print_str("\n");
    print_str("  Total supplies distributed: "); print_int(sm.total_supplies); print_str("\n");
}

void sm_sales_report(void) {
    print_str("[SM] Sales report:\n");
    print_str("  Sales channels: "); print_int(sm.n_sales); print_str("\n");
    print_str("  Total products sold: "); print_int(sm.total_sales); print_str("\n");
}

void sm_comprehensive_report(void) {
    print_str("[SM] Comprehensive report:\n");
    print_str("  Cooperatives registered: "); print_int(sm.n_cooperation); print_str("\n");
    print_str("  Total cooperatives: "); print_int(sm.total_cooperatives); print_str("\n");
    print_str("  Logistics units: "); print_int(sm.n_logistics); print_str("\n");
    print_str("  Total warehouses: "); print_int(sm.total_warehouses); print_str("\n");
    print_str("  Service categories: "); print_int(sm.n_comprehensive); print_str("\n");
    print_str("  Total services: "); print_int(sm.total_services); print_str("\n");
}

void sm_print_state(void) {
    print_str("[SM] Sp="); print_int(sm.n_supply);
    print_str(" Sl="); print_int(sm.n_sales);
    print_str(" Cp="); print_int(sm.n_cooperation);
    print_str(" Lg="); print_int(sm.n_logistics);
    print_str(" Cm="); print_int(sm.n_comprehensive);
    print_str("\n");
}

int main(void) {
    print_str("=== Supply & Marketing Demo ===\n\n");
    sm_init();

    print_str("Supply management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int mat = (i % 4) + 1;
        int dst = 1000 + (i * 250);
        int agr = 500 + (i * 120);
        int dly = 300 + (i * 80);
        int year = 2020 + (i % 5);
        sm_supply(type, mat, dst, agr, dly, year);
    }

    print_str("\nSales management...\n");
    for (int i = 0; i < 14; i++) {
        int chn = (i % 4) + 1;
        int prd = (i % 6) + 1;
        int sld = 2000 + (i * 500);
        int vol = 5000 + (i * 1200);
        int brd = 100 + (i * 25);
        int year = 2021 + (i % 4);
        sm_sales(chn, prd, sld, vol, brd, year);
    }

    print_str("\nCooperation management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int reg = 30 + (i * 8);
        int enr = 200 + (i * 50);
        int prf = 50 + (i * 12);
        int year = 2022 + (i % 3);
        sm_cooperation(type, cat, reg, enr, prf, year);
    }

    print_str("\nStorage & logistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int str = (i % 5) + 1;
        int whs = 20 + (i * 5);
        int cld = 10 + (i * 3);
        int dlv = 500 + (i * 120);
        int year = 2023 + (i % 2);
        sm_logistics(type, str, whs, cld, dlv, year);
    }

    print_str("\nComprehensive services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int dom = (i % 5) + 1;
        int prd = 100 + (i * 25);
        int tch = 50 + (i * 12);
        int fin = 30 + (i * 8);
        int year = 2024;
        sm_comprehensive(type, dom, prd, tch, fin, year);
    }

    print_str("\nSupply report...\n");
    sm_supply_report();

    print_str("\nSales report...\n");
    sm_sales_report();

    print_str("\nComprehensive report...\n");
    sm_comprehensive_report();

    print_str("\nFinal state...\n");
    sm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
