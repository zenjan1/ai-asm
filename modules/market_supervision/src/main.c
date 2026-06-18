/* market_supervision: Market supervision administration (v1.0)
 * Business registration, quality, food safety, pricing, intellectual property
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

#define MAX_REGISTER     16
#define MAX_QUALITY      14
#define MAX_FOOD         12
#define MAX_PRICE        10
#define MAX_IP           10

typedef struct {
    int    register_id;
    int    business_type;
    int    owner_id;
    int    capital;
    int    employees;
    int    status;
    int    year;
    int    active;
} register_t;

typedef struct {
    int    quality_id;
    int    product_id;
    int    company_id;
    int    test_type;
    int    pass;
    int    standard_id;
    int    certification;
    int    year;
    int    active;
} quality_t;

typedef struct {
    int    food_id;
    int    business_id;
    int    food_type;
    int    inspection_score;
    int    violations;
    int    license_valid;
    int    year;
    int    active;
} food_t;

typedef struct {
    int    price_id;
    int    commodity_id;
    int    category;
    int    base_price;
    int    current_price;
    int    violation;
    int    year;
    int    active;
} price_t;

typedef struct {
    int    ip_id;
    int    owner_id;
    int    ip_type;
    int    registration;
    int    disputes;
    int    protection_score;
    int    year;
    int    active;
} ip_t;

typedef struct {
    int    n_register;
    int    n_quality;
    int    n_food;
    int    n_price;
    int    n_ip;
    int    total_capital;
    int    total_employees;
    int    total_passed;
    int    total_violations;
    int    total_registrations;
} ms_state_t;

static register_t registers[MAX_REGISTER];
static quality_t qualities[MAX_QUALITY];
static food_t foods[MAX_FOOD];
static price_t prices[MAX_PRICE];
static ip_t ips[MAX_IP];
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
    ms.n_register = 0; ms.n_quality = 0; ms.n_food = 0;
    ms.n_price = 0; ms.n_ip = 0;
    ms.total_capital = 0; ms.total_employees = 0;
    ms.total_passed = 0; ms.total_violations = 0;
    ms.total_registrations = 0;
    for (int i = 0; i < MAX_REGISTER; i++) registers[i].active = 0;
    for (int i = 0; i < MAX_QUALITY; i++) qualities[i].active = 0;
    for (int i = 0; i < MAX_FOOD; i++) foods[i].active = 0;
    for (int i = 0; i < MAX_PRICE; i++) prices[i].active = 0;
    for (int i = 0; i < MAX_IP; i++) ips[i].active = 0;
    initialized = 1;
    print_str("[MS] Market supervision initialized\n");
    return 0;
}

int ms_register(int business_type, int owner, int capital, int employees, int status, int year) {
    if (ms.n_register >= MAX_REGISTER) return -1;
    register_t* r = &registers[ms.n_register];
    r->register_id = ms.n_register;
    r->business_type = business_type;
    r->owner_id = owner;
    r->capital = capital;
    r->employees = employees;
    r->status = status;
    r->year = year;
    r->active = 1;
    ms.total_capital += capital;
    ms.total_employees += employees;
    ms.n_register++;
    print_str("[MS] Register "); print_int(ms.n_register - 1);
    print_str(" type="); print_int(business_type);
    print_str(" own="); print_int(owner);
    print_str(" cap=$"); print_int(capital);
    print_str(" emp="); print_int(employees);
    print_str(" st="); print_int(status); print_str("\n");
    return ms.n_register - 1;
}

int ms_quality(int product, int company, int test, int pass, int standard, int cert, int year) {
    if (ms.n_quality >= MAX_QUALITY) return -1;
    quality_t* q = &qualities[ms.n_quality];
    q->quality_id = ms.n_quality;
    q->product_id = product;
    q->company_id = company;
    q->test_type = test;
    q->pass = pass;
    q->standard_id = standard;
    q->certification = cert;
    q->year = year;
    q->active = 1;
    if (pass) ms.total_passed++;
    ms.n_quality++;
    print_str("[MS] Quality "); print_int(ms.n_quality - 1);
    print_str(" prd="); print_int(product);
    print_str(" co="); print_int(company);
    print_str(" tst="); print_int(test);
    print_str(" pass="); print_int(pass);
    print_str(" std="); print_int(standard); print_str("\n");
    return ms.n_quality - 1;
}

int ms_food(int business, int food_type, int score, int violations, int license, int year) {
    if (ms.n_food >= MAX_FOOD) return -1;
    food_t* f = &foods[ms.n_food];
    f->food_id = ms.n_food;
    f->business_id = business;
    f->food_type = food_type;
    f->inspection_score = score;
    f->violations = violations;
    f->license_valid = license;
    f->year = year;
    f->active = 1;
    ms.total_violations += violations;
    ms.n_food++;
    print_str("[MS] Food "); print_int(ms.n_food - 1);
    print_str(" biz="); print_int(business);
    print_str(" type="); print_int(food_type);
    print_str(" scr="); print_int(score);
    print_str(" viol="); print_int(violations);
    print_str(" lic="); print_int(license); print_str("\n");
    return ms.n_food - 1;
}

int ms_price(int commodity, int category, int base, int current, int violation, int year) {
    if (ms.n_price >= MAX_PRICE) return -1;
    price_t* p = &prices[ms.n_price];
    p->price_id = ms.n_price;
    p->commodity_id = commodity;
    p->category = category;
    p->base_price = base;
    p->current_price = current;
    p->violation = violation;
    p->year = year;
    p->active = 1;
    ms.n_price++;
    print_str("[MS] Price "); print_int(ms.n_price - 1);
    print_str(" cmd="); print_int(commodity);
    print_str(" cat="); print_int(category);
    print_str(" bas=$"); print_int(base);
    print_str(" cur=$"); print_int(current);
    print_str(" viol="); print_int(violation); print_str("\n");
    return ms.n_price - 1;
}

int ms_ip(int owner, int ip_type, int registration, int disputes, int protection, int year) {
    if (ms.n_ip >= MAX_IP) return -1;
    ip_t* ip = &ips[ms.n_ip];
    ip->ip_id = ms.n_ip;
    ip->owner_id = owner;
    ip->ip_type = ip_type;
    ip->registration = registration;
    ip->disputes = disputes;
    ip->protection_score = protection;
    ip->year = year;
    ip->active = 1;
    ms.total_registrations += registration;
    ms.n_ip++;
    print_str("[MS] IP "); print_int(ms.n_ip - 1);
    print_str(" own="); print_int(owner);
    print_str(" type="); print_int(ip_type);
    print_str(" reg="); print_int(registration);
    print_str(" dsp="); print_int(disputes);
    print_str(" prt="); print_int(protection); print_str("\n");
    return ms.n_ip - 1;
}

void ms_register_report(void) {
    print_str("[MS] Registration report:\n");
    print_str("  Businesses: "); print_int(ms.n_register); print_str("\n");
    print_str("  Total capital: $"); print_int(ms.total_capital); print_str("\n");
    print_str("  Total employees: "); print_int(ms.total_employees); print_str("\n");
}

void ms_quality_report(void) {
    print_str("[MS] Quality report:\n");
    print_str("  Products tested: "); print_int(ms.n_quality); print_str("\n");
    print_str("  Total passed: "); print_int(ms.total_passed); print_str("\n");
}

void ms_food_report(void) {
    print_str("[MS] Food safety report:\n");
    print_str("  Food businesses: "); print_int(ms.n_food); print_str("\n");
    print_str("  Total violations: "); print_int(ms.total_violations); print_str("\n");
    print_str("  Price monitoring: "); print_int(ms.n_price); print_str("\n");
    print_str("  IP registrations: "); print_int(ms.n_ip); print_str("\n");
    print_str("  Total IP registrations: "); print_int(ms.total_registrations); print_str("\n");
}

void ms_print_state(void) {
    print_str("[MS] Rg="); print_int(ms.n_register);
    print_str(" Qu="); print_int(ms.n_quality);
    print_str(" Fd="); print_int(ms.n_food);
    print_str(" Pr="); print_int(ms.n_price);
    print_str(" Ip="); print_int(ms.n_ip);
    print_str("\n");
}

int main(void) {
    print_str("=== Market Supervision Demo ===\n\n");
    ms_init();

    print_str("Business registration...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int own = 1000 + (i * 13);
        int cap = 100000 + (i * 50000);
        int emp = 5 + (i * 3);
        int st = (i % 3) + 1;
        int year = 2020 + (i % 5);
        ms_register(type, own, cap, emp, st, year);
    }

    print_str("\nQuality supervision...\n");
    for (int i = 0; i < 14; i++) {
        int prd = 2000 + (i * 11);
        int co = 3000 + (i * 7);
        int tst = (i % 4) + 1;
        int pass = (i % 3 == 0) ? 0 : 1;
        int std = 100 + (i % 8);
        int crt = (i % 4 == 0) ? 1 : 0;
        int year = 2021 + (i % 4);
        ms_quality(prd, co, tst, pass, std, crt, year);
    }

    print_str("\nFood safety...\n");
    for (int i = 0; i < 12; i++) {
        int biz = 4000 + (i * 17);
        int type = (i % 5) + 1;
        int scr = 60 + (i * 3);
        int viol = i % 4;
        int lic = (i % 3 == 0) ? 0 : 1;
        int year = 2022 + (i % 3);
        ms_food(biz, type, scr, viol, lic, year);
    }

    print_str("\nPrice supervision...\n");
    for (int i = 0; i < 10; i++) {
        int cmd = 500 + (i * 20);
        int cat = (i % 4) + 1;
        int bas = 50 + (i * 10);
        int cur = bas + (i % 5);
        int viol = (i % 5 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        ms_price(cmd, cat, bas, cur, viol, year);
    }

    print_str("\nIntellectual property...\n");
    for (int i = 0; i < 10; i++) {
        int own = 6000 + (i * 13);
        int type = (i % 3) + 1;
        int reg = 1 + (i % 4);
        int dsp = i % 3;
        int prt = 70 + (i * 3);
        int year = 2024;
        ms_ip(own, type, reg, dsp, prt, year);
    }

    print_str("\nRegistration report...\n");
    ms_register_report();

    print_str("\nQuality report...\n");
    ms_quality_report();

    print_str("\nFood safety report...\n");
    ms_food_report();

    print_str("\nFinal state...\n");
    ms_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
