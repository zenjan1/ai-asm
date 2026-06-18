/* cross_border_ecommerce: Cross-border e-commerce administration system (v1.0)
 * E-commerce parks, overseas warehouses, customs clearance, payment settlement, compliance
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

#define MAX_PARK         16
#define MAX_OVERSEAS     14
#define MAX_CUSTOMS      12
#define MAX_PAYMENT      10
#define MAX_COMPLIANCE   10

typedef struct {
    int    park_id;
    int    park_type;
    int    park_category;
    int    park_planning;
    int    park_investment;
    int    park_services;
    int    year;
    int    active;
} park_t;

typedef struct {
    int    overseas_id;
    int    overseas_type;
    int    overseas_region;
    int    warehouse_construction;
    int    warehouse_operations;
    int    warehouse_delivery;
    int    year;
    int    active;
} overseas_t;

typedef struct {
    int    customs_id;
    int    customs_type;
    int    customs_category;
    int    customs_declaration;
    int    customs_inspection;
    int    customs_statistics;
    int    year;
    int    active;
} customs_t;

typedef struct {
    int    payment_id;
    int    payment_type;
    int    payment_category;
    int    cross_border_payments;
    int    forex_settlement;
    int    tax_refund;
    int    year;
    int    active;
} payment_t;

typedef struct {
    int    compliance_id;
    int    compliance_type;
    int    compliance_category;
    int    compliance_reviews;
    int    ip_protection;
    int    consumer_rights;
    int    year;
    int    active;
} compliance_t;

typedef struct {
    int    n_park;
    int    n_overseas;
    int    n_customs;
    int    n_payment;
    int    n_compliance;
    int    total_planning;
    int    total_construction;
    int    total_declaration;
    int    total_payments;
    int    total_reviews;
} cbec_state_t;

static park_t parks[MAX_PARK];
static overseas_t overseases[MAX_OVERSEAS];
static customs_t customs_list[MAX_CUSTOMS];
static payment_t payments[MAX_PAYMENT];
static compliance_t compliances[MAX_COMPLIANCE];
static cbec_state_t cbec;

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

int cbec_init(void) {
    if (initialized) return -1;
    cbec.n_park = 0; cbec.n_overseas = 0; cbec.n_customs = 0;
    cbec.n_payment = 0; cbec.n_compliance = 0;
    cbec.total_planning = 0; cbec.total_construction = 0;
    cbec.total_declaration = 0; cbec.total_payments = 0;
    cbec.total_reviews = 0;
    for (int i = 0; i < MAX_PARK; i++) parks[i].active = 0;
    for (int i = 0; i < MAX_OVERSEAS; i++) overseases[i].active = 0;
    for (int i = 0; i < MAX_CUSTOMS; i++) customs_list[i].active = 0;
    for (int i = 0; i < MAX_PAYMENT; i++) payments[i].active = 0;
    for (int i = 0; i < MAX_COMPLIANCE; i++) compliances[i].active = 0;
    initialized = 1;
    print_str("[CBEC] Cross-border e-commerce initialized\n");
    return 0;
}

int cbec_park(int pk_type, int cat, int planning, int investment, int services, int year) {
    if (cbec.n_park >= MAX_PARK) return -1;
    park_t* p = &parks[cbec.n_park];
    p->park_id = cbec.n_park;
    p->park_type = pk_type;
    p->park_category = cat;
    p->park_planning = planning;
    p->park_investment = investment;
    p->park_services = services;
    p->year = year;
    p->active = 1;
    cbec.total_planning += planning;
    cbec.n_park++;
    print_str("[CBEC] Park "); print_int(cbec.n_park - 1);
    print_str(" type="); print_int(pk_type);
    print_str(" cat="); print_int(cat);
    print_str(" pln="); print_int(planning);
    print_str(" inv="); print_int(investment);
    print_str(" svc="); print_int(services); print_str("\n");
    return cbec.n_park - 1;
}

int cbec_overseas(int os_type, int region, int construction, int operations, int delivery, int year) {
    if (cbec.n_overseas >= MAX_OVERSEAS) return -1;
    overseas_t* o = &overseases[cbec.n_overseas];
    o->overseas_id = cbec.n_overseas;
    o->overseas_type = os_type;
    o->overseas_region = region;
    o->warehouse_construction = construction;
    o->warehouse_operations = operations;
    o->warehouse_delivery = delivery;
    o->year = year;
    o->active = 1;
    cbec.total_construction += construction;
    cbec.n_overseas++;
    print_str("[CBEC] Overseas "); print_int(cbec.n_overseas - 1);
    print_str(" type="); print_int(os_type);
    print_str(" rgn="); print_int(region);
    print_str(" cst="); print_int(construction);
    print_str(" ops="); print_int(operations);
    print_str(" dlv="); print_int(delivery); print_str("\n");
    return cbec.n_overseas - 1;
}

int cbec_customs(int cst_type, int cat, int declaration, int inspection, int statistics, int year) {
    if (cbec.n_customs >= MAX_CUSTOMS) return -1;
    customs_t* c = &customs_list[cbec.n_customs];
    c->customs_id = cbec.n_customs;
    c->customs_type = cst_type;
    c->customs_category = cat;
    c->customs_declaration = declaration;
    c->customs_inspection = inspection;
    c->customs_statistics = statistics;
    c->year = year;
    c->active = 1;
    cbec.total_declaration += declaration;
    cbec.n_customs++;
    print_str("[CBEC] Customs "); print_int(cbec.n_customs - 1);
    print_str(" type="); print_int(cst_type);
    print_str(" cat="); print_int(cat);
    print_str(" dcl="); print_int(declaration);
    print_str(" insp="); print_int(inspection);
    print_str(" sts="); print_int(statistics); print_str("\n");
    return cbec.n_customs - 1;
}

int cbec_payment(int pym_type, int cat, int cross_border, int forex, int tax_refund, int year) {
    if (cbec.n_payment >= MAX_PAYMENT) return -1;
    payment_t* p = &payments[cbec.n_payment];
    p->payment_id = cbec.n_payment;
    p->payment_type = pym_type;
    p->payment_category = cat;
    p->cross_border_payments = cross_border;
    p->forex_settlement = forex;
    p->tax_refund = tax_refund;
    p->year = year;
    p->active = 1;
    cbec.total_payments += cross_border;
    cbec.n_payment++;
    print_str("[CBEC] Payment "); print_int(cbec.n_payment - 1);
    print_str(" type="); print_int(pym_type);
    print_str(" cat="); print_int(cat);
    print_str(" cpb="); print_int(cross_border);
    print_str(" frx="); print_int(forex);
    print_str(" txr="); print_int(tax_refund); print_str("\n");
    return cbec.n_payment - 1;
}

int cbec_compliance(int cmp_type, int cat, int reviews, int ip_protect, int consumer, int year) {
    if (cbec.n_compliance >= MAX_COMPLIANCE) return -1;
    compliance_t* c = &compliances[cbec.n_compliance];
    c->compliance_id = cbec.n_compliance;
    c->compliance_type = cmp_type;
    c->compliance_category = cat;
    c->compliance_reviews = reviews;
    c->ip_protection = ip_protect;
    c->consumer_rights = consumer;
    c->year = year;
    c->active = 1;
    cbec.total_reviews += reviews;
    cbec.n_compliance++;
    print_str("[CBEC] Compliance "); print_int(cbec.n_compliance - 1);
    print_str(" type="); print_int(cmp_type);
    print_str(" cat="); print_int(cat);
    print_str(" rvw="); print_int(reviews);
    print_str(" ipp="); print_int(ip_protect);
    print_str(" cns="); print_int(consumer); print_str("\n");
    return cbec.n_compliance - 1;
}

void cbec_park_report(void) {
    print_str("[CBEC] Park report:\n");
    print_str("  E-commerce park categories: "); print_int(cbec.n_park); print_str("\n");
    print_str("  Total park planning: "); print_int(cbec.total_planning); print_str("\n");
}

void cbec_overseas_report(void) {
    print_str("[CBEC] Overseas report:\n");
    print_str("  Overseas warehouse categories: "); print_int(cbec.n_overseas); print_str("\n");
    print_str("  Total warehouse construction: "); print_int(cbec.total_construction); print_str("\n");
}

void cbec_compliance_report(void) {
    print_str("[CBEC] Compliance report:\n");
    print_str("  Customs categories: "); print_int(cbec.n_customs); print_str("\n");
    print_str("  Total declarations: "); print_int(cbec.total_declaration); print_str("\n");
    print_str("  Payment categories: "); print_int(cbec.n_payment); print_str("\n");
    print_str("  Total cross-border payments: "); print_int(cbec.total_payments); print_str("\n");
    print_str("  Compliance categories: "); print_int(cbec.n_compliance); print_str("\n");
    print_str("  Total compliance reviews: "); print_int(cbec.total_reviews); print_str("\n");
}

void cbec_print_state(void) {
    print_str("[CBEC] Pk="); print_int(cbec.n_park);
    print_str(" Os="); print_int(cbec.n_overseas);
    print_str(" Cs="); print_int(cbec.n_customs);
    print_str(" Py="); print_int(cbec.n_payment);
    print_str(" Cm="); print_int(cbec.n_compliance);
    print_str("\n");
}

int main(void) {
    print_str("=== Cross-Border E-Commerce Demo ===\n\n");
    cbec_init();

    print_str("E-commerce parks...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pln = 10 + (i * 3);
        int inv = 200 + (i * 50);
        int svc = 30 + (i * 8);
        int year = 2020 + (i % 5);
        cbec_park(type, cat, pln, inv, svc, year);
    }

    print_str("\nOverseas warehouses...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int region = (i % 6) + 1;
        int cst = 5 + (i * 2);
        int ops = 15 + (i * 4);
        int dlv = 20 + (i * 5);
        int year = 2021 + (i % 4);
        cbec_overseas(type, region, cst, ops, dlv, year);
    }

    print_str("\nCustoms clearance...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dcl = 100 + (i * 25);
        int insp = 50 + (i * 12);
        int sts = 80 + (i * 20);
        int year = 2022 + (i % 3);
        cbec_customs(type, cat, dcl, insp, sts, year);
    }

    print_str("\nPayment settlement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cpb = 150 + (i * 40);
        int frx = 120 + (i * 30);
        int txr = 60 + (i * 15);
        int year = 2023 + (i % 2);
        cbec_payment(type, cat, cpb, frx, txr, year);
    }

    print_str("\nCompliance supervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rvw = 40 + (i * 10);
        int ipp = 20 + (i * 5);
        int cns = 30 + (i * 8);
        int year = 2024;
        cbec_compliance(type, cat, rvw, ipp, cns, year);
    }

    print_str("\nPark report...\n");
    cbec_park_report();

    print_str("\nOverseas report...\n");
    cbec_overseas_report();

    print_str("\nCompliance report...\n");
    cbec_compliance_report();

    print_str("\nFinal state...\n");
    cbec_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
