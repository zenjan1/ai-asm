/* customs_service: Customs management (v1.0)
 * Imports, exports, tariffs, inspections, trade statistics
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

#define MAX_DECLARATIONS 20
#define MAX_INSPECTIONS  12
#define MAX_TARIFFS      10
#define MAX_ORIGINS      8

typedef struct {
    int    decl_id;
    int    type;
    int    trader_id;
    int    country;
    double value;
    double weight_kg;
    int    n_items;
    double tariff_paid;
    int    status;
    int    day;
    int    active;
} declaration_t;

typedef struct {
    int    insp_id;
    int    decl_id;
    int    result;
    int    n_findings;
    double penalty;
    int    inspector_id;
    int    day;
    int    active;
} inspection_t;

typedef struct {
    int    tariff_id;
    int    category;
    double rate_pct;
    int    n_declarations;
    double revenue;
    int    active;
} tariff_t;

typedef struct {
    int    origin_id;
    int    country;
    int    agreement;
    double discount_pct;
    int    n_certificates;
    double trade_value;
    int    active;
} origin_t;

typedef struct {
    int    n_declarations;
    int    n_inspections;
    int    n_tariffs;
    int    n_origins;
    double import_value;
    double export_value;
    double total_trade;
    double tariff_revenue;
    double penalties;
    double total_revenue;
    int    total_imports;
    int    total_exports;
    int    total_inspections;
} cs_state_t;

static declaration_t declarations[MAX_DECLARATIONS];
static inspection_t inspections[MAX_INSPECTIONS];
static tariff_t tariffs[MAX_TARIFFS];
static origin_t origins[MAX_ORIGINS];
static cs_state_t cs;

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

int cs_init(void) {
    if (initialized) return -1;
    cs.n_declarations = 0; cs.n_inspections = 0;
    cs.n_tariffs = 0; cs.n_origins = 0;
    cs.import_value = 0.0; cs.export_value = 0.0;
    cs.total_trade = 0.0; cs.tariff_revenue = 0.0;
    cs.penalties = 0.0; cs.total_revenue = 0.0;
    cs.total_imports = 0; cs.total_exports = 0;
    cs.total_inspections = 0;
    for (int i = 0; i < MAX_DECLARATIONS; i++) declarations[i].active = 0;
    for (int i = 0; i < MAX_INSPECTIONS; i++) inspections[i].active = 0;
    for (int i = 0; i < MAX_TARIFFS; i++) tariffs[i].active = 0;
    for (int i = 0; i < MAX_ORIGINS; i++) origins[i].active = 0;
    initialized = 1;
    print_str("[CS] Customs service initialized\n");
    return 0;
}

int cs_add_tariff(int category, double rate_pct) {
    if (cs.n_tariffs >= MAX_TARIFFS) return -1;
    tariff_t* t = &tariffs[cs.n_tariffs];
    t->tariff_id = cs.n_tariffs;
    t->category = category;
    t->rate_pct = rate_pct;
    t->n_declarations = 0;
    t->revenue = 0.0;
    t->active = 1;
    cs.n_tariffs++;
    print_str("[CS] Tariff "); print_int(cs.n_tariffs - 1);
    print_str(" cat="); print_int(category);
    print_str(" rate="); print_int((int)(rate_pct * 100)); print_str("%\n");
    return cs.n_tariffs - 1;
}

int cs_add_origin(int country, int agreement, double discount) {
    if (cs.n_origins >= MAX_ORIGINS) return -1;
    origin_t* o = &origins[cs.n_origins];
    o->origin_id = cs.n_origins;
    o->country = country;
    o->agreement = agreement;
    o->discount_pct = discount;
    o->n_certificates = 0;
    o->trade_value = 0.0;
    o->active = 1;
    cs.n_origins++;
    print_str("[CS] Origin "); print_int(cs.n_origins - 1);
    print_str(" country="); print_int(country);
    print_str(" agree="); print_int(agreement);
    print_str(" disc="); print_int((int)(discount * 100)); print_str("%\n");
    return cs.n_origins - 1;
}

int cs_declare(int type, int trader, int country, double value, double weight, int items, int tariff_id, int day) {
    if (cs.n_declarations >= MAX_DECLARATIONS || tariff_id >= cs.n_tariffs) return -1;
    declaration_t* d = &declarations[cs.n_declarations];
    d->decl_id = cs.n_declarations;
    d->type = type;
    d->trader_id = trader;
    d->country = country;
    d->value = value;
    d->weight_kg = weight;
    d->n_items = items;
    double rate = tariffs[tariff_id].rate_pct / 100.0;
    d->tariff_paid = value * rate;
    d->status = 1;
    d->day = day;
    d->active = 1;
    tariffs[tariff_id].n_declarations++;
    tariffs[tariff_id].revenue += d->tariff_paid;
    cs.tariff_revenue += d->tariff_paid;
    cs.total_revenue += d->tariff_paid;
    if (type == 1) {
        cs.import_value += value;
        cs.total_imports++;
    } else {
        cs.export_value += value;
        cs.total_exports++;
    }
    cs.total_trade += value;
    cs.n_declarations++;
    print_str("[CS] Decl "); print_int(cs.n_declarations - 1);
    print_str(" type="); print_int(type);
    print_str(" cty="); print_int(country);
    print_str(" val=$"); print_int((int)value);
    print_str(" tariff=$"); print_int((int)d->tariff_paid); print_str("\n");
    return cs.n_declarations - 1;
}

int cs_inspect(int decl_id, int result, int findings, double penalty, int inspector) {
    if (cs.n_inspections >= MAX_INSPECTIONS || decl_id >= cs.n_declarations) return -1;
    inspection_t* ins = &inspections[cs.n_inspections];
    ins->insp_id = cs.n_inspections;
    ins->decl_id = decl_id;
    ins->result = result;
    ins->n_findings = findings;
    ins->penalty = penalty;
    ins->inspector_id = inspector;
    ins->day = declarations[decl_id].day + 1;
    ins->active = 1;
    cs.penalties += penalty;
    cs.total_revenue += penalty;
    cs.total_inspections++;
    cs.n_inspections++;
    print_str("[CS] Insp "); print_int(cs.n_inspections - 1);
    print_str(" D"); print_int(decl_id);
    print_str(" result="); print_int(result);
    print_str(" findings="); print_int(findings);
    print_str(" pen=$"); print_int((int)penalty); print_str("\n");
    return cs.n_inspections - 1;
}

int cs_clear(int decl_id) {
    if (decl_id >= cs.n_declarations) return -1;
    declarations[decl_id].status = 2;
    print_str("[CS] Clear D"); print_int(decl_id); print_str("\n");
    return 0;
}

int cs_issue_origin_cert(int origin_id, double value) {
    if (origin_id >= cs.n_origins) return -1;
    origins[origin_id].n_certificates++;
    origins[origin_id].trade_value += value;
    print_str("[CS] Cert O"); print_int(origin_id);
    print_str(" $"); print_int((int)value); print_str("\n");
    return 0;
}

void cs_trade_report(void) {
    print_str("[CS] Trade report:\n");
    print_str("  Imports: "); print_int((int)cs.import_value); print_str("\n");
    print_str("  Exports: "); print_int((int)cs.export_value); print_str("\n");
    print_str("  Total trade: "); print_int((int)cs.total_trade); print_str("\n");
    print_str("  Import count: "); print_int(cs.total_imports); print_str("\n");
    print_str("  Export count: "); print_int(cs.total_exports); print_str("\n");
    print_str("  Declarations: "); print_int(cs.n_declarations); print_str("\n");
    for (int i = 0; i < cs.n_tariffs; i++) {
        tariff_t* t = &tariffs[i];
        print_str("  T"); print_int(i);
        print_str(" cat="); print_int(t->category);
        print_str(" rate="); print_int((int)(t->rate_pct * 100));
        print_str("% decl="); print_int(t->n_declarations);
        print_str(" rev=$"); print_int((int)t->revenue); print_str("\n");
    }
}

void cs_revenue_report(void) {
    print_str("[CS] Revenue report:\n");
    print_str("  Tariff revenue: "); print_int((int)cs.tariff_revenue); print_str("\n");
    print_str("  Penalties: "); print_int((int)cs.penalties); print_str("\n");
    print_str("  Total revenue: "); print_int((int)cs.total_revenue); print_str("\n");
    print_str("  Inspections: "); print_int(cs.total_inspections); print_str("\n");
}

void cs_print_state(void) {
    print_str("[CS] Decls="); print_int(cs.n_declarations);
    print_str(" Insp="); print_int(cs.n_inspections);
    print_str(" Tariffs="); print_int(cs.n_tariffs);
    print_str(" Origins="); print_int(cs.n_origins);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)cs.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Customs Service Demo ===\n\n");
    cs_init();

    print_str("Adding tariffs...\n");
    cs_add_tariff(1, 5.0);
    cs_add_tariff(2, 10.0);
    cs_add_tariff(3, 15.0);
    cs_add_tariff(4, 8.0);
    cs_add_tariff(5, 20.0);
    cs_add_tariff(1, 6.0);
    cs_add_tariff(2, 12.0);
    cs_add_tariff(3, 18.0);
    cs_add_tariff(4, 7.0);
    cs_add_tariff(5, 25.0);

    print_str("\nAdding origins...\n");
    cs_add_origin(1, 1, 50.0);
    cs_add_origin(2, 2, 30.0);
    cs_add_origin(3, 1, 45.0);
    cs_add_origin(4, 3, 20.0);
    cs_add_origin(5, 2, 35.0);
    cs_add_origin(6, 1, 40.0);
    cs_add_origin(7, 3, 25.0);
    cs_add_origin(8, 2, 50.0);

    print_str("\nDeclaring shipments...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 2) + 1;
        int trader = i % 10;
        int country = (i % 8) + 1;
        double value = 50000.0 + (i * 25000);
        double weight = 1000 + (i * 500);
        int items = 5 + (i % 10);
        int tariff = i % 10;
        int day = 1 + (i % 28);
        cs_declare(type, trader, country, value, weight, items, tariff, day);
    }

    print_str("\nInspecting...\n");
    for (int i = 0; i < 12; i++) {
        int decl = i % 20;
        int result = (i % 3);
        int findings = i % 4;
        double penalty = findings * 2000.0;
        int inspector = i % 6;
        cs_inspect(decl, result, findings, penalty, inspector);
    }

    print_str("\nClearing shipments...\n");
    for (int i = 0; i < 20; i++) {
        cs_clear(i);
    }

    print_str("\nIssuing origin certificates...\n");
    for (int i = 0; i < 8; i++) {
        cs_issue_origin_cert(i, 100000.0 + (i * 50000));
    }

    print_str("\nTrade report...\n");
    cs_trade_report();

    print_str("\nRevenue report...\n");
    cs_revenue_report();

    print_str("\nFinal state...\n");
    cs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
