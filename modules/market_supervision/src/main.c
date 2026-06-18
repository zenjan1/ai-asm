/* market_supervision: Market supervision administration system (v1.0)
 * Enterprise registration, market regulation, product quality, food safety, consumer rights
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

#define MAX_ENTERPRISE   16
#define MAX_MARKET       14
#define MAX_QUALITY      12
#define MAX_FOOD         10
#define MAX_CONSUMER     10

typedef struct {
    int    enterprise_id;
    int    registration_type;
    int    business_scope;
    int    registered_capital;
    int    employees;
    int    status;
    int    year;
    int    active;
} enterprise_t;

typedef struct {
    int    market_id;
    int    regulation_type;
    int    monopoly_cases;
    int    unfair_cases;
    int    inspections;
    int    penalties;
    int    year;
    int    active;
} market_t;

typedef struct {
    int    quality_id;
    int    product_category;
    int    certification_type;
    int    inspections;
    int    passed;
    int    failed;
    int    year;
    int    active;
} quality_t;

typedef struct {
    int    food_id;
    int    license_type;
    int    restaurant_category;
    int    licenses_issued;
    int    inspections;
    int    violations;
    int    year;
    int    active;
} food_t;

typedef struct {
    int    consumer_id;
    int    complaint_type;
    int    dispute_category;
    int    complaints;
    int    resolved;
    int    compensation;
    int    year;
    int    active;
} consumer_t;

typedef struct {
    int    n_enterprise;
    int    n_market;
    int    n_quality;
    int    n_food;
    int    n_consumer;
    int    total_enterprises;
    int    total_inspections;
    int    total_certifications;
    int    total_licenses;
    int    total_complaints;
} ms_state_t;

static enterprise_t enterprises[MAX_ENTERPRISE];
static market_t markets[MAX_MARKET];
static quality_t qualities[MAX_QUALITY];
static food_t foods[MAX_FOOD];
static consumer_t consumers[MAX_CONSUMER];
static ms_state_t ms;

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

int ms_init(void) {
    if (initialized) return -1;
    ms.n_enterprise = 0; ms.n_market = 0; ms.n_quality = 0;
    ms.n_food = 0; ms.n_consumer = 0;
    ms.total_enterprises = 0; ms.total_inspections = 0;
    ms.total_certifications = 0; ms.total_licenses = 0;
    ms.total_complaints = 0;
    for (int i = 0; i < MAX_ENTERPRISE; i++) enterprises[i].active = 0;
    for (int i = 0; i < MAX_MARKET; i++) markets[i].active = 0;
    for (int i = 0; i < MAX_QUALITY; i++) qualities[i].active = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = 0;
    for (int i = 0; i < MAX_CONSUMER; i++) consumers[i].active = 0;
    initialized = 1;
    print_str("[MS] Market supervision initialized\n");
    return 0;
}

int ms_enterprise(int reg_type, int scope, int capital, int employees, int status, int year) {
    if (ms.n_enterprise >= MAX_ENTERPRISE) return -1;
    enterprise_t* e = &enterprises[ms.n_enterprise];
    e->enterprise_id = ms.n_enterprise;
    e->registration_type = reg_type;
    e->business_scope = scope;
    e->registered_capital = capital;
    e->employees = employees;
    e->status = status;
    e->year = year;
    e->active = 1;
    ms.total_enterprises += employees;
    ms.n_enterprise++;
    print_str("[MS] Enterprise "); print_int(ms.n_enterprise - 1);
    print_str(" type="); print_int(reg_type);
    print_str(" scope="); print_int(scope);
    print_str(" cap="); print_int(capital);
    print_str(" emp="); print_int(employees);
    print_str(" st="); print_int(status); print_str("\n");
    return ms.n_enterprise - 1;
}

int ms_market(int regulation_type, int monopoly, int unfair, int inspections, int penalties, int year) {
    if (ms.n_market >= MAX_MARKET) return -1;
    market_t* m = &markets[ms.n_market];
    m->market_id = ms.n_market;
    m->regulation_type = regulation_type;
    m->monopoly_cases = monopoly;
    m->unfair_cases = unfair;
    m->inspections = inspections;
    m->penalties = penalties;
    m->year = year;
    m->active = 1;
    ms.total_inspections += inspections;
    ms.n_market++;
    print_str("[MS] Market "); print_int(ms.n_market - 1);
    print_str(" type="); print_int(regulation_type);
    print_str(" mono="); print_int(monopoly);
    print_str(" unf="); print_int(unfair);
    print_str(" insp="); print_int(inspections);
    print_str(" pen="); print_int(penalties); print_str("\n");
    return ms.n_market - 1;
}

int ms_quality(int product_cat, int cert_type, int inspections, int passed, int failed, int year) {
    if (ms.n_quality >= MAX_QUALITY) return -1;
    quality_t* q = &qualities[ms.n_quality];
    q->quality_id = ms.n_quality;
    q->product_category = product_cat;
    q->certification_type = cert_type;
    q->inspections = inspections;
    q->passed = passed;
    q->failed = failed;
    q->year = year;
    q->active = 1;
    ms.total_certifications += passed;
    ms.n_quality++;
    print_str("[MS] Quality "); print_int(ms.n_quality - 1);
    print_str(" prod="); print_int(product_cat);
    print_str(" cert="); print_int(cert_type);
    print_str(" insp="); print_int(inspections);
    print_str(" pass="); print_int(passed);
    print_str(" fail="); print_int(failed); print_str("\n");
    return ms.n_quality - 1;
}

int ms_food(int license_type, int restaurant_cat, int licenses, int inspections, int violations, int year) {
    if (ms.n_food >= MAX_FOOD) return -1;
    food_t* f = &foods[ms.n_food];
    f->food_id = ms.n_food;
    f->license_type = license_type;
    f->restaurant_category = restaurant_cat;
    f->licenses_issued = licenses;
    f->inspections = inspections;
    f->violations = violations;
    f->year = year;
    f->active = 1;
    ms.total_licenses += licenses;
    ms.n_food++;
    print_str("[MS] Food "); print_int(ms.n_food - 1);
    print_str(" lic="); print_int(license_type);
    print_str(" rest="); print_int(restaurant_cat);
    print_str(" iss="); print_int(licenses);
    print_str(" insp="); print_int(inspections);
    print_str(" viol="); print_int(violations); print_str("\n");
    return ms.n_food - 1;
}

int ms_consumer(int complaint_type, int dispute_cat, int complaints, int resolved, int compensation, int year) {
    if (ms.n_consumer >= MAX_CONSUMER) return -1;
    consumer_t* c = &consumers[ms.n_consumer];
    c->consumer_id = ms.n_consumer;
    c->complaint_type = complaint_type;
    c->dispute_category = dispute_cat;
    c->complaints = complaints;
    c->resolved = resolved;
    c->compensation = compensation;
    c->year = year;
    c->active = 1;
    ms.total_complaints += complaints;
    ms.n_consumer++;
    print_str("[MS] Consumer "); print_int(ms.n_consumer - 1);
    print_str(" type="); print_int(complaint_type);
    print_str(" disp="); print_int(dispute_cat);
    print_str(" cmp="); print_int(complaints);
    print_str(" res="); print_int(resolved);
    print_str(" comp=$"); print_int(compensation); print_str("\n");
    return ms.n_consumer - 1;
}

void ms_enterprise_report(void) {
    print_str("[MS] Enterprise report:\n");
    print_str("  Registrations: "); print_int(ms.n_enterprise); print_str("\n");
    print_str("  Total employees: "); print_int(ms.total_enterprises); print_str("\n");
}

void ms_market_report(void) {
    print_str("[MS] Market regulation report:\n");
    print_str("  Market cases: "); print_int(ms.n_market); print_str("\n");
    print_str("  Total inspections: "); print_int(ms.total_inspections); print_str("\n");
}

void ms_quality_report(void) {
    print_str("[MS] Quality report:\n");
    print_str("  Product categories: "); print_int(ms.n_quality); print_str("\n");
    print_str("  Total certifications: "); print_int(ms.total_certifications); print_str("\n");
    print_str("  Food licenses: "); print_int(ms.n_food); print_str("\n");
    print_str("  Total licenses issued: "); print_int(ms.total_licenses); print_str("\n");
    print_str("  Consumer complaints: "); print_int(ms.n_consumer); print_str("\n");
    print_str("  Total complaints: "); print_int(ms.total_complaints); print_str("\n");
}

void ms_print_state(void) {
    print_str("[MS] En="); print_int(ms.n_enterprise);
    print_str(" Mk="); print_int(ms.n_market);
    print_str(" Qu="); print_int(ms.n_quality);
    print_str(" Fd="); print_int(ms.n_food);
    print_str(" Cs="); print_int(ms.n_consumer);
    print_str("\n");
}

int main(void) {
    print_str("=== Market Supervision Demo ===\n\n");
    ms_init();

    print_str("Enterprise registration...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int scope = (i % 10) + 1;
        int cap = 1000000 + (i * 500000);
        int emp = 50 + (i * 20);
        int st = (i % 3) + 1;
        int year = 2020 + (i % 5);
        ms_enterprise(type, scope, cap, emp, st, year);
    }

    print_str("\nMarket regulation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int mono = 5 + (i * 2);
        int unf = 10 + (i * 3);
        int insp = 100 + (i * 50);
        int pen = 20 + (i * 5);
        int year = 2021 + (i % 4);
        ms_market(type, mono, unf, insp, pen, year);
    }

    print_str("\nProduct quality...\n");
    for (int i = 0; i < 12; i++) {
        int prod = (i % 8) + 1;
        int cert = (i % 4) + 1;
        int insp = 200 + (i * 100);
        int pass = insp - (i * 5);
        int fail = i * 5;
        int year = 2022 + (i % 3);
        ms_quality(prod, cert, insp, pass, fail, year);
    }

    print_str("\nFood safety...\n");
    for (int i = 0; i < 10; i++) {
        int lic = (i % 4) + 1;
        int rest = (i % 5) + 1;
        int iss = 500 + (i * 100);
        int insp = 1000 + (i * 200);
        int viol = 10 + (i * 3);
        int year = 2023 + (i % 2);
        ms_food(lic, rest, iss, insp, viol, year);
    }

    print_str("\nConsumer rights...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 6) + 1;
        int disp = (i % 4) + 1;
        int cmp = 300 + (i * 50);
        int res = cmp - (i * 2);
        int comp = 50000 + (i * 10000);
        int year = 2024;
        ms_consumer(type, disp, cmp, res, comp, year);
    }

    print_str("\nEnterprise report...\n");
    ms_enterprise_report();

    print_str("\nMarket report...\n");
    ms_market_report();

    print_str("\nQuality report...\n");
    ms_quality_report();

    print_str("\nFinal state...\n");
    ms_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
