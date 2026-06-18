/* digital_trade: Digital trade administration system (v1.0)
 * Digital products, digital platforms, digital tariffs, digital security, digital cooperation
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

#define MAX_PRODUCT      16
#define MAX_PLATFORM_D   14
#define MAX_TARIFF       12
#define MAX_SECURITY     10
#define MAX_COOPERATION  10

typedef struct {
    int    product_id;
    int    product_type;
    int    product_category;
    int    digital_content;
    int    digital_services;
    int    data_transactions;
    int    year;
    int    active;
} product_t;

typedef struct {
    int    platform_id;
    int    platform_type;
    int    platform_category;
    int    platform_supervision;
    int    data_governance;
    int    anti_monopoly;
    int    year;
    int    active;
} platform_d_t;

typedef struct {
    int    tariff_id;
    int    tariff_type;
    int    tariff_category;
    int    digital_service_tax;
    int    digital_tariff_stats;
    int    tax_coordination;
    int    year;
    int    active;
} tariff_t;

typedef struct {
    int    security_id;
    int    security_type;
    int    security_category;
    int    data_security_reviews;
    int    cross_border_data;
    int    privacy_protection;
    int    year;
    int    active;
} security_t;

typedef struct {
    int    cooperation_id;
    int    cooperation_type;
    int    cooperation_category;
    int    trade_agreements;
    int    intl_digital_rules;
    int    digital_standards;
    int    year;
    int    active;
} cooperation_t;

typedef struct {
    int    n_product;
    int    n_platform_d;
    int    n_tariff;
    int    n_security;
    int    n_cooperation;
    int    total_content;
    int    total_supervision;
    int    total_tax;
    int    total_reviews;
    int    total_agreements;
} dt_state_t;

static product_t products[MAX_PRODUCT];
static platform_d_t platform_ds[MAX_PLATFORM_D];
static tariff_t tariffs[MAX_TARIFF];
static security_t securities[MAX_SECURITY];
static cooperation_t cooperations[MAX_COOPERATION];
static dt_state_t dt;

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

int dt_init(void) {
    if (initialized) return -1;
    dt.n_product = 0; dt.n_platform_d = 0; dt.n_tariff = 0;
    dt.n_security = 0; dt.n_cooperation = 0;
    dt.total_content = 0; dt.total_supervision = 0;
    dt.total_tax = 0; dt.total_reviews = 0;
    dt.total_agreements = 0;
    for (int i = 0; i < MAX_PRODUCT; i++) products[i].active = 0;
    for (int i = 0; i < MAX_PLATFORM_D; i++) platform_ds[i].active = 0;
    for (int i = 0; i < MAX_TARIFF; i++) tariffs[i].active = 0;
    for (int i = 0; i < MAX_SECURITY; i++) securities[i].active = 0;
    for (int i = 0; i < MAX_COOPERATION; i++) cooperations[i].active = 0;
    initialized = 1;
    print_str("[DT] Digital trade initialized\n");
    return 0;
}

int dt_product(int prd_type, int cat, int content, int services, int data_txn, int year) {
    if (dt.n_product >= MAX_PRODUCT) return -1;
    product_t* p = &products[dt.n_product];
    p->product_id = dt.n_product;
    p->product_type = prd_type;
    p->product_category = cat;
    p->digital_content = content;
    p->digital_services = services;
    p->data_transactions = data_txn;
    p->year = year;
    p->active = 1;
    dt.total_content += content;
    dt.n_product++;
    print_str("[DT] Product "); print_int(dt.n_product - 1);
    print_str(" type="); print_int(prd_type);
    print_str(" cat="); print_int(cat);
    print_str(" cnt="); print_int(content);
    print_str(" svc="); print_int(services);
    print_str(" dtx="); print_int(data_txn); print_str("\n");
    return dt.n_product - 1;
}

int dt_platform_d(int plt_type, int cat, int supervision, int governance, int anti_monopoly, int year) {
    if (dt.n_platform_d >= MAX_PLATFORM_D) return -1;
    platform_d_t* p = &platform_ds[dt.n_platform_d];
    p->platform_id = dt.n_platform_d;
    p->platform_type = plt_type;
    p->platform_category = cat;
    p->platform_supervision = supervision;
    p->data_governance = governance;
    p->anti_monopoly = anti_monopoly;
    p->year = year;
    p->active = 1;
    dt.total_supervision += supervision;
    dt.n_platform_d++;
    print_str("[DT] Platform "); print_int(dt.n_platform_d - 1);
    print_str(" type="); print_int(plt_type);
    print_str(" cat="); print_int(cat);
    print_str(" spv="); print_int(supervision);
    print_str(" gvr="); print_int(governance);
    print_str(" ant="); print_int(anti_monopoly); print_str("\n");
    return dt.n_platform_d - 1;
}

int dt_tariff(int trf_type, int cat, int service_tax, int tariff_stats, int coordination, int year) {
    if (dt.n_tariff >= MAX_TARIFF) return -1;
    tariff_t* t = &tariffs[dt.n_tariff];
    t->tariff_id = dt.n_tariff;
    t->tariff_type = trf_type;
    t->tariff_category = cat;
    t->digital_service_tax = service_tax;
    t->digital_tariff_stats = tariff_stats;
    t->tax_coordination = coordination;
    t->year = year;
    t->active = 1;
    dt.total_tax += service_tax;
    dt.n_tariff++;
    print_str("[DT] Tariff "); print_int(dt.n_tariff - 1);
    print_str(" type="); print_int(trf_type);
    print_str(" cat="); print_int(cat);
    print_str(" stx="); print_int(service_tax);
    print_str(" tst="); print_int(tariff_stats);
    print_str(" crd="); print_int(coordination); print_str("\n");
    return dt.n_tariff - 1;
}

int dt_security(int scr_type, int cat, int reviews, int cross_border, int privacy, int year) {
    if (dt.n_security >= MAX_SECURITY) return -1;
    security_t* s = &securities[dt.n_security];
    s->security_id = dt.n_security;
    s->security_type = scr_type;
    s->security_category = cat;
    s->data_security_reviews = reviews;
    s->cross_border_data = cross_border;
    s->privacy_protection = privacy;
    s->year = year;
    s->active = 1;
    dt.total_reviews += reviews;
    dt.n_security++;
    print_str("[DT] Security "); print_int(dt.n_security - 1);
    print_str(" type="); print_int(scr_type);
    print_str(" cat="); print_int(cat);
    print_str(" rvw="); print_int(reviews);
    print_str(" cbd="); print_int(cross_border);
    print_str(" prv="); print_int(privacy); print_str("\n");
    return dt.n_security - 1;
}

int dt_cooperation(int cop_type, int cat, int agreements, int intl_rules, int standards, int year) {
    if (dt.n_cooperation >= MAX_COOPERATION) return -1;
    cooperation_t* c = &cooperations[dt.n_cooperation];
    c->cooperation_id = dt.n_cooperation;
    c->cooperation_type = cop_type;
    c->cooperation_category = cat;
    c->trade_agreements = agreements;
    c->intl_digital_rules = intl_rules;
    c->digital_standards = standards;
    c->year = year;
    c->active = 1;
    dt.total_agreements += agreements;
    dt.n_cooperation++;
    print_str("[DT] Cooperation "); print_int(dt.n_cooperation - 1);
    print_str(" type="); print_int(cop_type);
    print_str(" cat="); print_int(cat);
    print_str(" agr="); print_int(agreements);
    print_str(" irs="); print_int(intl_rules);
    print_str(" std="); print_int(standards); print_str("\n");
    return dt.n_cooperation - 1;
}

void dt_product_report(void) {
    print_str("[DT] Product report:\n");
    print_str("  Digital product categories: "); print_int(dt.n_product); print_str("\n");
    print_str("  Total digital content: "); print_int(dt.total_content); print_str("\n");
}

void dt_platform_report(void) {
    print_str("[DT] Platform report:\n");
    print_str("  Digital platform categories: "); print_int(dt.n_platform_d); print_str("\n");
    print_str("  Total platform supervision: "); print_int(dt.total_supervision); print_str("\n");
}

void dt_cooperation_report(void) {
    print_str("[DT] Cooperation report:\n");
    print_str("  Tariff categories: "); print_int(dt.n_tariff); print_str("\n");
    print_str("  Total digital service tax: "); print_int(dt.total_tax); print_str("\n");
    print_str("  Security categories: "); print_int(dt.n_security); print_str("\n");
    print_str("  Total security reviews: "); print_int(dt.total_reviews); print_str("\n");
    print_str("  Cooperation categories: "); print_int(dt.n_cooperation); print_str("\n");
    print_str("  Total trade agreements: "); print_int(dt.total_agreements); print_str("\n");
}

void dt_print_state(void) {
    print_str("[DT] Pd="); print_int(dt.n_product);
    print_str(" Pl="); print_int(dt.n_platform_d);
    print_str(" Tf="); print_int(dt.n_tariff);
    print_str(" Sc="); print_int(dt.n_security);
    print_str(" Cp="); print_int(dt.n_cooperation);
    print_str("\n");
}

int main(void) {
    print_str("=== Digital Trade Demo ===\n\n");
    dt_init();

    print_str("Digital products...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cnt = 100 + (i * 25);
        int svc = 80 + (i * 20);
        int dtx = 60 + (i * 15);
        int year = 2020 + (i % 5);
        dt_product(type, cat, cnt, svc, dtx, year);
    }

    print_str("\nDigital platforms...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spv = 40 + (i * 10);
        int gvr = 30 + (i * 8);
        int ant = 20 + (i * 5);
        int year = 2021 + (i % 4);
        dt_platform_d(type, cat, spv, gvr, ant, year);
    }

    print_str("\nDigital tariffs...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stx = 50 + (i * 12);
        int tst = 40 + (i * 10);
        int crd = 25 + (i * 6);
        int year = 2022 + (i % 3);
        dt_tariff(type, cat, stx, tst, crd, year);
    }

    print_str("\nDigital security...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rvw = 30 + (i * 8);
        int cbd = 20 + (i * 5);
        int prv = 25 + (i * 6);
        int year = 2023 + (i % 2);
        dt_security(type, cat, rvw, cbd, prv, year);
    }

    print_str("\nDigital cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int agr = 10 + (i * 3);
        int irs = 8 + (i * 2);
        int std = 12 + (i * 3);
        int year = 2024;
        dt_cooperation(type, cat, agr, irs, std, year);
    }

    print_str("\nProduct report...\n");
    dt_product_report();

    print_str("\nPlatform report...\n");
    dt_platform_report();

    print_str("\nCooperation report...\n");
    dt_cooperation_report();

    print_str("\nFinal state...\n");
    dt_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
