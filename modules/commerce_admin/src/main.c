/* commerce_admin: Commerce administration system (v1.0)
 * Domestic trade, foreign trade, investment, e-commerce, services
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

#define MAX_DOMESTIC     16
#define MAX_FOREIGN      14
#define MAX_INVEST       12
#define MAX_ECOMMERCE    10
#define MAX_SERVICE      10

typedef struct {
    int    domestic_id;
    int    retailer_id;
    int    sector;
    int    sales_volume;
    int    revenue;
    int    employment;
    int    year;
    int    active;
} domestic_t;

typedef struct {
    int    foreign_id;
    int    partner_id;
    int    country_code;
    int    export_value;
    int    import_value;
    int    trade_balance;
    int    year;
    int    active;
} foreign_t;

typedef struct {
    int    invest_id;
    int    investor_id;
    int    origin_country;
    int    investment_amount;
    int    jobs_created;
    int    sector;
    int    year;
    int    active;
} invest_t;

typedef struct {
    int    ecommerce_id;
    int    platform_id;
    int    platform_type;
    int    gmv;
    int    orders;
    int    merchants;
    int    year;
    int    active;
} ecommerce_t;

typedef struct {
    int    service_id;
    int    provider_id;
    int    service_type;
    int    contract_value;
    int    international;
    int    growth_rate;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    n_domestic;
    int    n_foreign;
    int    n_invest;
    int    n_ecommerce;
    int    n_service;
    int    total_revenue;
    int    total_export;
    int    total_import;
    int    total_investment;
    int    total_gmv;
} cma_state_t;

static domestic_t domestics[MAX_DOMESTIC];
static foreign_t foreigns[MAX_FOREIGN];
static invest_t invests[MAX_INVEST];
static ecommerce_t ecommerces[MAX_ECOMMERCE];
static service_t services[MAX_SERVICE];
static cma_state_t cma;

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

int cma_init(void) {
    if (initialized) return -1;
    cma.n_domestic = 0; cma.n_foreign = 0; cma.n_invest = 0;
    cma.n_ecommerce = 0; cma.n_service = 0;
    cma.total_revenue = 0; cma.total_export = 0;
    cma.total_import = 0; cma.total_investment = 0;
    cma.total_gmv = 0;
    for (int i = 0; i < MAX_DOMESTIC; i++) domestics[i].active = 0;
    for (int i = 0; i < MAX_FOREIGN; i++) foreigns[i].active = 0;
    for (int i = 0; i < MAX_INVEST; i++) invests[i].active = 0;
    for (int i = 0; i < MAX_ECOMMERCE; i++) ecommerces[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    initialized = 1;
    print_str("[CMA] Commerce admin initialized\n");
    return 0;
}

int cma_domestic(int retailer, int sector, int sales, int revenue, int employment, int year) {
    if (cma.n_domestic >= MAX_DOMESTIC) return -1;
    domestic_t* d = &domestics[cma.n_domestic];
    d->domestic_id = cma.n_domestic;
    d->retailer_id = retailer;
    d->sector = sector;
    d->sales_volume = sales;
    d->revenue = revenue;
    d->employment = employment;
    d->year = year;
    d->active = 1;
    cma.total_revenue += revenue;
    cma.n_domestic++;
    print_str("[CMA] Domestic "); print_int(cma.n_domestic - 1);
    print_str(" rtl="); print_int(retailer);
    print_str(" sec="); print_int(sector);
    print_str(" sls="); print_int(sales);
    print_str(" rev=$"); print_int(revenue);
    print_str(" emp="); print_int(employment); print_str("\n");
    return cma.n_domestic - 1;
}

int cma_foreign(int partner, int country, int export_val, int import_val, int balance, int year) {
    if (cma.n_foreign >= MAX_FOREIGN) return -1;
    foreign_t* f = &foreigns[cma.n_foreign];
    f->foreign_id = cma.n_foreign;
    f->partner_id = partner;
    f->country_code = country;
    f->export_value = export_val;
    f->import_value = import_val;
    f->trade_balance = balance;
    f->year = year;
    f->active = 1;
    cma.total_export += export_val;
    cma.total_import += import_val;
    cma.n_foreign++;
    print_str("[CMA] Foreign "); print_int(cma.n_foreign - 1);
    print_str(" ptr="); print_int(partner);
    print_str(" cty="); print_int(country);
    print_str(" exp=$"); print_int(export_val);
    print_str(" imp=$"); print_int(import_val);
    print_str(" bal=$"); print_int(balance); print_str("\n");
    return cma.n_foreign - 1;
}

int cma_invest(int investor, int origin, int amount, int jobs, int sector, int year) {
    if (cma.n_invest >= MAX_INVEST) return -1;
    invest_t* inv = &invests[cma.n_invest];
    inv->invest_id = cma.n_invest;
    inv->investor_id = investor;
    inv->origin_country = origin;
    inv->investment_amount = amount;
    inv->jobs_created = jobs;
    inv->sector = sector;
    inv->year = year;
    inv->active = 1;
    cma.total_investment += amount;
    cma.n_invest++;
    print_str("[CMA] Invest "); print_int(cma.n_invest - 1);
    print_str(" inv="); print_int(investor);
    print_str(" org="); print_int(origin);
    print_str(" amt=$"); print_int(amount);
    print_str(" job="); print_int(jobs);
    print_str(" sec="); print_int(sector); print_str("\n");
    return cma.n_invest - 1;
}

int cma_ecommerce(int platform, int platform_type, int gmv, int orders, int merchants, int year) {
    if (cma.n_ecommerce >= MAX_ECOMMERCE) return -1;
    ecommerce_t* ec = &ecommerces[cma.n_ecommerce];
    ec->ecommerce_id = cma.n_ecommerce;
    ec->platform_id = platform;
    ec->platform_type = platform_type;
    ec->gmv = gmv;
    ec->orders = orders;
    ec->merchants = merchants;
    ec->year = year;
    ec->active = 1;
    cma.total_gmv += gmv;
    cma.n_ecommerce++;
    print_str("[CMA] ECommerce "); print_int(cma.n_ecommerce - 1);
    print_str(" plt="); print_int(platform);
    print_str(" type="); print_int(platform_type);
    print_str(" gmv=$"); print_int(gmv);
    print_str(" ord="); print_int(orders);
    print_str(" mrc="); print_int(merchants); print_str("\n");
    return cma.n_ecommerce - 1;
}

int cma_service(int provider, int service_type, int contract, int international, int growth, int year) {
    if (cma.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[cma.n_service];
    s->service_id = cma.n_service;
    s->provider_id = provider;
    s->service_type = service_type;
    s->contract_value = contract;
    s->international = international;
    s->growth_rate = growth;
    s->year = year;
    s->active = 1;
    cma.n_service++;
    print_str("[CMA] Service "); print_int(cma.n_service - 1);
    print_str(" prv="); print_int(provider);
    print_str(" type="); print_int(service_type);
    print_str(" cnt=$"); print_int(contract);
    print_str(" int="); print_int(international);
    print_str(" grw="); print_int(growth); print_str("%\n");
    return cma.n_service - 1;
}

void cma_domestic_report(void) {
    print_str("[CMA] Domestic trade report:\n");
    print_str("  Retailers: "); print_int(cma.n_domestic); print_str("\n");
    print_str("  Total revenue: $"); print_int(cma.total_revenue); print_str("\n");
}

void cma_foreign_report(void) {
    print_str("[CMA] Foreign trade report:\n");
    print_str("  Trade partners: "); print_int(cma.n_foreign); print_str("\n");
    print_str("  Total exports: $"); print_int(cma.total_export); print_str("\n");
    print_str("  Total imports: $"); print_int(cma.total_import); print_str("\n");
}

void cma_service_report(void) {
    print_str("[CMA] Service trade report:\n");
    print_str("  Foreign investment projects: "); print_int(cma.n_invest); print_str("\n");
    print_str("  Total investment: $"); print_int(cma.total_investment); print_str("\n");
    print_str("  E-commerce platforms: "); print_int(cma.n_ecommerce); print_str("\n");
    print_str("  Total GMV: $"); print_int(cma.total_gmv); print_str("\n");
    print_str("  Service providers: "); print_int(cma.n_service); print_str("\n");
}

void cma_print_state(void) {
    print_str("[CMA] Dm="); print_int(cma.n_domestic);
    print_str(" Fg="); print_int(cma.n_foreign);
    print_str(" Iv="); print_int(cma.n_invest);
    print_str(" Ec="); print_int(cma.n_ecommerce);
    print_str(" Sv="); print_int(cma.n_service);
    print_str("\n");
}

int main(void) {
    print_str("=== Commerce Admin Demo ===\n\n");
    cma_init();

    print_str("Domestic trade...\n");
    for (int i = 0; i < 16; i++) {
        int rtl = 1000 + (i * 11);
        int sec = (i % 5) + 1;
        int sls = 10000 + (i * 3000);
        int rev = sls * 5;
        int emp = 20 + (i * 5);
        int year = 2020 + (i % 5);
        cma_domestic(rtl, sec, sls, rev, emp, year);
    }

    print_str("\nForeign trade...\n");
    for (int i = 0; i < 14; i++) {
        int ptr = 2000 + (i * 7);
        int cty = 100 + (i % 20);
        int exp = 500000 + (i * 100000);
        int imp = 400000 + (i * 80000);
        int bal = exp - imp;
        int year = 2021 + (i % 4);
        cma_foreign(ptr, cty, exp, imp, bal, year);
    }

    print_str("\nForeign investment...\n");
    for (int i = 0; i < 12; i++) {
        int inv = 3000 + (i * 13);
        int org = 100 + (i % 15);
        int amt = 1000000 + (i * 500000);
        int job = 50 + (i * 20);
        int sec = (i % 4) + 1;
        int year = 2022 + (i % 3);
        cma_invest(inv, org, amt, job, sec, year);
    }

    print_str("\nE-commerce...\n");
    for (int i = 0; i < 10; i++) {
        int plt = 4000 + (i * 17);
        int type = (i % 3) + 1;
        int gmv = 2000000 + (i * 1000000);
        int ord = 100000 + (i * 50000);
        int mrc = 1000 + (i * 500);
        int year = 2023 + (i % 2);
        cma_ecommerce(plt, type, gmv, ord, mrc, year);
    }

    print_str("\nService trade...\n");
    for (int i = 0; i < 10; i++) {
        int prv = 5000 + (i * 11);
        int type = (i % 4) + 1;
        int cnt = 500000 + (i * 200000);
        int intl = (i % 2 == 0) ? 1 : 0;
        int grw = 5 + (i * 3);
        int year = 2024;
        cma_service(prv, type, cnt, intl, grw, year);
    }

    print_str("\nDomestic report...\n");
    cma_domestic_report();

    print_str("\nForeign report...\n");
    cma_foreign_report();

    print_str("\nService report...\n");
    cma_service_report();

    print_str("\nFinal state...\n");
    cma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
