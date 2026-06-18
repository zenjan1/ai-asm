/* overseas_chinese: Overseas Chinese affairs administration system (v1.0)
 * Overseas Chinese services, dependents, investment, education, cultural exchange
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

#define MAX_OVERSEAS     16
#define MAX_DEPENDENT    14
#define MAX_INVESTMENT   12
#define MAX_EDUCATION    10
#define MAX_CULTURAL     10

typedef struct {
    int    overseas_id;
    int    country_id;
    int    occupation;
    int    registered;
    int    returned;
    int    services_provided;
    int    year;
    int    active;
} overseas_t;

typedef struct {
    int    dependent_id;
    int    region_id;
    int    relationship;
    int    dependents_registered;
    int    assistance_cases;
    int    events_organized;
    int    year;
    int    active;
} dependent_t;

typedef struct {
    int    investment_id;
    int    investor_id;
    int    industry;
    int    investment_amount;
    int    employees;
    int    tax_contribution;
    int    year;
    int    active;
} investment_t;

typedef struct {
    int    education_id;
    int    school_id;
    int    country_id;
    int    students_enrolled;
    int    teachers_dispatched;
    int    materials_sent;
    int    year;
    int    active;
} education_t;

typedef struct {
    int    cultural_id;
    int    activity_type;
    int    country_id;
    int    participants;
    int    events_held;
    int    cultural_shows;
    int    year;
    int    active;
} cultural_t;

typedef struct {
    int    n_overseas;
    int    n_dependent;
    int    n_investment;
    int    n_education;
    int    n_cultural;
    int    total_overseas;
    int    total_dependents;
    int    total_investment;
    int    total_students;
    int    total_participants;
} oc_state_t;

static overseas_t overseas[MAX_OVERSEAS];
static dependent_t dependents[MAX_DEPENDENT];
static investment_t investments[MAX_INVESTMENT];
static education_t educations[MAX_EDUCATION];
static cultural_t cultural[MAX_CULTURAL];
static oc_state_t oc;

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

int oc_init(void) {
    if (initialized) return -1;
    oc.n_overseas = 0; oc.n_dependent = 0; oc.n_investment = 0;
    oc.n_education = 0; oc.n_cultural = 0;
    oc.total_overseas = 0; oc.total_dependents = 0;
    oc.total_investment = 0; oc.total_students = 0;
    oc.total_participants = 0;
    for (int i = 0; i < MAX_OVERSEAS; i++) overseas[i].active = 0;
    for (int i = 0; i < MAX_DEPENDENT; i++) dependents[i].active = 0;
    for (int i = 0; i < MAX_INVESTMENT; i++) investments[i].active = 0;
    for (int i = 0; i < MAX_EDUCATION; i++) educations[i].active = 0;
    for (int i = 0; i < MAX_CULTURAL; i++) cultural[i].active = 0;
    initialized = 1;
    print_str("[OC] Overseas Chinese admin initialized\n");
    return 0;
}

int oc_overseas(int country, int occupation, int registered, int returned, int services, int year) {
    if (oc.n_overseas >= MAX_OVERSEAS) return -1;
    overseas_t* o = &overseas[oc.n_overseas];
    o->overseas_id = oc.n_overseas;
    o->country_id = country;
    o->occupation = occupation;
    o->registered = registered;
    o->returned = returned;
    o->services_provided = services;
    o->year = year;
    o->active = 1;
    oc.total_overseas += registered;
    oc.n_overseas++;
    print_str("[OC] Overseas "); print_int(oc.n_overseas - 1);
    print_str(" cty="); print_int(country);
    print_str(" occ="); print_int(occupation);
    print_str(" reg="); print_int(registered);
    print_str(" rtn="); print_int(returned);
    print_str(" svc="); print_int(services); print_str("\n");
    return oc.n_overseas - 1;
}

int oc_dependent(int region, int relationship, int dep_count, int assistance, int events, int year) {
    if (oc.n_dependent >= MAX_DEPENDENT) return -1;
    dependent_t* d = &dependents[oc.n_dependent];
    d->dependent_id = oc.n_dependent;
    d->region_id = region;
    d->relationship = relationship;
    d->dependents_registered = dep_count;
    d->assistance_cases = assistance;
    d->events_organized = events;
    d->year = year;
    d->active = 1;
    oc.total_dependents += dep_count;
    oc.n_dependent++;
    print_str("[OC] Dependent "); print_int(oc.n_dependent - 1);
    print_str(" rgn="); print_int(region);
    print_str(" rel="); print_int(relationship);
    print_str(" dep="); print_int(dep_count);
    print_str(" ast="); print_int(assistance);
    print_str(" evt="); print_int(events); print_str("\n");
    return oc.n_dependent - 1;
}

int oc_investment(int investor, int industry, int amount, int employees, int tax, int year) {
    if (oc.n_investment >= MAX_INVESTMENT) return -1;
    investment_t* inv = &investments[oc.n_investment];
    inv->investment_id = oc.n_investment;
    inv->investor_id = investor;
    inv->industry = industry;
    inv->investment_amount = amount;
    inv->employees = employees;
    inv->tax_contribution = tax;
    inv->year = year;
    inv->active = 1;
    oc.total_investment += amount;
    oc.n_investment++;
    print_str("[OC] Investment "); print_int(oc.n_investment - 1);
    print_str(" inv="); print_int(investor);
    print_str(" ind="); print_int(industry);
    print_str(" amt=$"); print_int(amount);
    print_str(" emp="); print_int(employees);
    print_str(" tax=$"); print_int(tax); print_str("\n");
    return oc.n_investment - 1;
}

int oc_education(int school, int country, int students, int teachers, int materials, int year) {
    if (oc.n_education >= MAX_EDUCATION) return -1;
    education_t* e = &educations[oc.n_education];
    e->education_id = oc.n_education;
    e->school_id = school;
    e->country_id = country;
    e->students_enrolled = students;
    e->teachers_dispatched = teachers;
    e->materials_sent = materials;
    e->year = year;
    e->active = 1;
    oc.total_students += students;
    oc.n_education++;
    print_str("[OC] Education "); print_int(oc.n_education - 1);
    print_str(" sch="); print_int(school);
    print_str(" cty="); print_int(country);
    print_str(" stu="); print_int(students);
    print_str(" tch="); print_int(teachers);
    print_str(" mat="); print_int(materials); print_str("\n");
    return oc.n_education - 1;
}

int oc_cultural(int activity_type, int country, int participants, int events, int shows, int year) {
    if (oc.n_cultural >= MAX_CULTURAL) return -1;
    cultural_t* c = &cultural[oc.n_cultural];
    c->cultural_id = oc.n_cultural;
    c->activity_type = activity_type;
    c->country_id = country;
    c->participants = participants;
    c->events_held = events;
    c->cultural_shows = shows;
    c->year = year;
    c->active = 1;
    oc.total_participants += participants;
    oc.n_cultural++;
    print_str("[OC] Cultural "); print_int(oc.n_cultural - 1);
    print_str(" type="); print_int(activity_type);
    print_str(" cty="); print_int(country);
    print_str(" par="); print_int(participants);
    print_str(" evt="); print_int(events);
    print_str(" shw="); print_int(shows); print_str("\n");
    return oc.n_cultural - 1;
}

void oc_overseas_report(void) {
    print_str("[OC] Overseas report:\n");
    print_str("  Countries: "); print_int(oc.n_overseas); print_str("\n");
    print_str("  Total registered: "); print_int(oc.total_overseas); print_str("\n");
}

void oc_dependent_report(void) {
    print_str("[OC] Dependent report:\n");
    print_str("  Regions: "); print_int(oc.n_dependent); print_str("\n");
    print_str("  Total dependents: "); print_int(oc.total_dependents); print_str("\n");
}

void oc_investment_report(void) {
    print_str("[OC] Investment report:\n");
    print_str("  Chinese schools: "); print_int(oc.n_education); print_str("\n");
    print_str("  Total students: "); print_int(oc.total_students); print_str("\n");
    print_str("  Investments: "); print_int(oc.n_investment); print_str("\n");
    print_str("  Total investment: $"); print_int(oc.total_investment); print_str("\n");
    print_str("  Cultural exchanges: "); print_int(oc.n_cultural); print_str("\n");
    print_str("  Total participants: "); print_int(oc.total_participants); print_str("\n");
}

void oc_print_state(void) {
    print_str("[OC] Ov="); print_int(oc.n_overseas);
    print_str(" Dp="); print_int(oc.n_dependent);
    print_str(" Iv="); print_int(oc.n_investment);
    print_str(" Ed="); print_int(oc.n_education);
    print_str(" Cl="); print_int(oc.n_cultural);
    print_str("\n");
}

int main(void) {
    print_str("=== Overseas Chinese Demo ===\n\n");
    oc_init();

    print_str("Overseas Chinese services...\n");
    for (int i = 0; i < 16; i++) {
        int cty = 100 + (i * 7);
        int occ = (i % 8) + 1;
        int reg = 1000 + (i * 500);
        int rtn = reg / 10;
        int svc = 200 + (i * 50);
        int year = 2020 + (i % 5);
        oc_overseas(cty, occ, reg, rtn, svc, year);
    }

    print_str("\nDependent services...\n");
    for (int i = 0; i < 14; i++) {
        int rgn = (i % 10) + 1;
        int rel = (i % 4) + 1;
        int dep = 500 + (i * 100);
        int ast = 20 + (i * 5);
        int evt = 5 + (i * 2);
        int year = 2021 + (i % 4);
        oc_dependent(rgn, rel, dep, ast, evt, year);
    }

    print_str("\nInvestment services...\n");
    for (int i = 0; i < 12; i++) {
        int inv = 1000 + (i * 11);
        int ind = (i % 6) + 1;
        int amt = 5000000 + (i * 2000000);
        int emp = 100 + (i * 50);
        int tax = amt / 10;
        int year = 2022 + (i % 3);
        oc_investment(inv, ind, amt, emp, tax, year);
    }

    print_str("\nChinese education...\n");
    for (int i = 0; i < 10; i++) {
        int sch = 2000 + (i * 13);
        int cty = 100 + (i * 7);
        int stu = 200 + (i * 50);
        int tch = 10 + (i * 3);
        int mat = 500 + (i * 100);
        int year = 2023 + (i % 2);
        oc_education(sch, cty, stu, tch, mat, year);
    }

    print_str("\nCultural exchange...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int cty = 100 + (i * 11);
        int par = 100 + (i * 30);
        int evt = 3 + (i % 5);
        int shw = evt * 2;
        int year = 2024;
        oc_cultural(type, cty, par, evt, shw, year);
    }

    print_str("\nOverseas report...\n");
    oc_overseas_report();

    print_str("\nDependent report...\n");
    oc_dependent_report();

    print_str("\nInvestment report...\n");
    oc_investment_report();

    print_str("\nFinal state...\n");
    oc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
