/* taiwan_affairs: Taiwan affairs administration system (v1.0)
 * Cross-strait exchange, Taiwan compatriot services, Taiwan business, education, peaceful development
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

#define MAX_EXCHANGE     16
#define MAX_COMPATRIOT   14
#define MAX_BUSINESS     12
#define MAX_EDUCATION    10
#define MAX_DEVELOPMENT  10

typedef struct {
    int    exchange_id;
    int    exchange_type;
    int    category;
    int    visitors;
    int    events_held;
    int    agreements;
    int    year;
    int    active;
} exchange_t;

typedef struct {
    int    compatriot_id;
    int    region_id;
    int    service_type;
    int    registered;
    int    employed;
    int    services_provided;
    int    year;
    int    active;
} compatriot_t;

typedef struct {
    int    business_id;
    int    industry;
    int    investment_type;
    int    enterprises;
    int    investment_amount;
    int    employees;
    int    year;
    int    active;
} business_t;

typedef struct {
    int    education_id;
    int    institution_type;
    int    program_type;
    int    students;
    int    scholars;
    int    programs;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    development_id;
    int    forum_type;
    int    cooperation_area;
    int    forums_held;
    int    proposals;
    int    joint_projects;
    int    year;
    int    active;
} development_t;

typedef struct {
    int    n_exchange;
    int    n_compatriot;
    int    n_business;
    int    n_education;
    int    n_development;
    int    total_visitors;
    int    total_registered;
    int    total_investment;
    int    total_students;
    int    total_forums;
} ta_state_t;

static exchange_t exchanges[MAX_EXCHANGE];
static compatriot_t compatriots[MAX_COMPATRIOT];
static business_t businesses[MAX_BUSINESS];
static education_t educations[MAX_EDUCATION];
static development_t developments[MAX_DEVELOPMENT];
static ta_state_t ta;

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

int ta_init(void) {
    if (initialized) return -1;
    ta.n_exchange = 0; ta.n_compatriot = 0; ta.n_business = 0;
    ta.n_education = 0; ta.n_development = 0;
    ta.total_visitors = 0; ta.total_registered = 0;
    ta.total_investment = 0; ta.total_students = 0;
    ta.total_forums = 0;
    for (int i = 0; i < MAX_EXCHANGE; i++) exchanges[i].active = 0;
    for (int i = 0; i < MAX_COMPATRIOT; i++) compatriots[i].active = 0;
    for (int i = 0; i < MAX_BUSINESS; i++) businesses[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_DEVELOPMENT; i++) developments[i].active = 0;
    initialized = 1;
    print_str("[TA] Taiwan affairs initialized\n");
    return 0;
}

int ta_exchange(int exch_type, int category, int visitors, int events, int agreements, int year) {
    if (ta.n_exchange >= MAX_EXCHANGE) return -1;
    exchange_t* e = &exchanges[ta.n_exchange];
    e->exchange_id = ta.n_exchange;
    e->exchange_type = exch_type;
    e->category = category;
    e->visitors = visitors;
    e->events_held = events;
    e->agreements = agreements;
    e->year = year;
    e->active = 1;
    ta.total_visitors += visitors;
    ta.n_exchange++;
    print_str("[TA] Exchange "); print_int(ta.n_exchange - 1);
    print_str(" type="); print_int(exch_type);
    print_str(" cat="); print_int(category);
    print_str(" vis="); print_int(visitors);
    print_str(" evt="); print_int(events);
    print_str(" agr="); print_int(agreements); print_str("\n");
    return ta.n_exchange - 1;
}

int ta_compatriot(int region, int svc_type, int registered, int employed, int services, int year) {
    if (ta.n_compatriot >= MAX_COMPATRIOT) return -1;
    compatriot_t* c = &compatriots[ta.n_compatriot];
    c->compatriot_id = ta.n_compatriot;
    c->region_id = region;
    c->service_type = svc_type;
    c->registered = registered;
    c->employed = employed;
    c->services_provided = services;
    c->year = year;
    c->active = 1;
    ta.total_registered += registered;
    ta.n_compatriot++;
    print_str("[TA] Compatriot "); print_int(ta.n_compatriot - 1);
    print_str(" rgn="); print_int(region);
    print_str(" svc="); print_int(svc_type);
    print_str(" reg="); print_int(registered);
    print_str(" emp="); print_int(employed);
    print_str(" svcN="); print_int(services); print_str("\n");
    return ta.n_compatriot - 1;
}

int ta_business(int industry, int inv_type, int enterprises, int investment, int employees, int year) {
    if (ta.n_business >= MAX_BUSINESS) return -1;
    business_t* b = &businesses[ta.n_business];
    b->business_id = ta.n_business;
    b->industry = industry;
    b->investment_type = inv_type;
    b->enterprises = enterprises;
    b->investment_amount = investment;
    b->employees = employees;
    b->year = year;
    b->active = 1;
    ta.total_investment += investment;
    ta.n_business++;
    print_str("[TA] Business "); print_int(ta.n_business - 1);
    print_str(" ind="); print_int(industry);
    print_str(" type="); print_int(inv_type);
    print_str(" ent="); print_int(enterprises);
    print_str(" inv=$"); print_int(investment);
    print_str(" emp="); print_int(employees); print_str("\n");
    return ta.n_business - 1;
}

int ta_education(int inst_type, int prog_type, int students, int scholars, int programs, int year) {
    if (ta.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[ta.n_education];
    e->education_id = ta.n_education;
    e->institution_type = inst_type;
    e->program_type = prog_type;
    e->students = students;
    e->scholars = scholars;
    e->programs = programs;
    e->year = year;
    e->active = 1;
    ta.total_students += students;
    ta.n_education++;
    print_str("[TA] Education "); print_int(ta.n_education - 1);
    print_str(" inst="); print_int(inst_type);
    print_str(" prog="); print_int(prog_type);
    print_str(" stu="); print_int(students);
    print_str(" sch="); print_int(scholars);
    print_str(" prg="); print_int(programs); print_str("\n");
    return ta.n_education - 1;
}

int ta_development(int forum_type, int area, int forums, int proposals, int projects, int year) {
    if (ta.n_development >= MAX_DEVELOPMENT) return -1;
    development_t* d = &developments[ta.n_development];
    d->development_id = ta.n_development;
    d->forum_type = forum_type;
    d->cooperation_area = area;
    d->forums_held = forums;
    d->proposals = proposals;
    d->joint_projects = projects;
    d->year = year;
    d->active = 1;
    ta.total_forums += forums;
    ta.n_development++;
    print_str("[TA] Development "); print_int(ta.n_development - 1);
    print_str(" type="); print_int(forum_type);
    print_str(" area="); print_int(area);
    print_str(" frm="); print_int(forums);
    print_str(" prp="); print_int(proposals);
    print_str(" prj="); print_int(projects); print_str("\n");
    return ta.n_development - 1;
}

void ta_exchange_report(void) {
    print_str("[TA] Exchange report:\n");
    print_str("  Exchange categories: "); print_int(ta.n_exchange); print_str("\n");
    print_str("  Total visitors: "); print_int(ta.total_visitors); print_str("\n");
}

void ta_compatriot_report(void) {
    print_str("[TA] Compatriot report:\n");
    print_str("  Service regions: "); print_int(ta.n_compatriot); print_str("\n");
    print_str("  Total registered: "); print_int(ta.total_registered); print_str("\n");
}

void ta_business_report(void) {
    print_str("[TA] Business report:\n");
    print_str("  Taiwan enterprises: "); print_int(ta.n_business); print_str("\n");
    print_str("  Total investment: $"); print_int(ta.total_investment); print_str("\n");
    print_str("  Education programs: "); print_int(ta.n_education); print_str("\n");
    print_str("  Total students: "); print_int(ta.total_students); print_str("\n");
    print_str("  Development forums: "); print_int(ta.n_development); print_str("\n");
    print_str("  Total forums held: "); print_int(ta.total_forums); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Ex="); print_int(ta.n_exchange);
    print_str(" Cp="); print_int(ta.n_compatriot);
    print_str(" Bs="); print_int(ta.n_business);
    print_str(" Ed="); print_int(ta.n_education);
    print_str(" Dv="); print_int(ta.n_development);
    print_str("\n");
}

int main(void) {
    print_str("=== Taiwan Affairs Demo ===\n\n");
    ta_init();

    print_str("Cross-strait exchange...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 6) + 1;
        int vis = 500 + (i * 200);
        int evt = 10 + (i * 3);
        int agr = 2 + (i % 4);
        int year = 2020 + (i % 5);
        ta_exchange(type, cat, vis, evt, agr, year);
    }

    print_str("\nCompatriot services...\n");
    for (int i = 0; i < 14; i++) {
        int rgn = (i % 8) + 1;
        int svc = (i % 4) + 1;
        int reg = 1000 + (i * 300);
        int emp = reg / 3;
        int svcN = 500 + (i * 100);
        int year = 2021 + (i % 4);
        ta_compatriot(rgn, svc, reg, emp, svcN, year);
    }

    print_str("\nTaiwan business...\n");
    for (int i = 0; i < 12; i++) {
        int ind = (i % 6) + 1;
        int type = (i % 4) + 1;
        int ent = 100 + (i * 30);
        int inv = 3000000 + (i * 1000000);
        int emp = 50 + (i * 20);
        int year = 2022 + (i % 3);
        ta_business(ind, type, ent, inv, emp, year);
    }

    print_str("\nEducation cooperation...\n");
    for (int i = 0; i < 10; i++) {
        int inst = (i % 4) + 1;
        int prog = (i % 5) + 1;
        int stu = 200 + (i * 50);
        int sch = 20 + (i * 5);
        int prg = 10 + (i * 3);
        int year = 2023 + (i % 2);
        ta_education(inst, prog, stu, sch, prg, year);
    }

    print_str("\nPeaceful development...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int area = (i % 6) + 1;
        int frm = 3 + (i % 4);
        int prp = 10 + (i * 3);
        int prj = 5 + (i * 2);
        int year = 2024;
        ta_development(type, area, frm, prp, prj, year);
    }

    print_str("\nExchange report...\n");
    ta_exchange_report();

    print_str("\nCompatriot report...\n");
    ta_compatriot_report();

    print_str("\nBusiness report...\n");
    ta_business_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
