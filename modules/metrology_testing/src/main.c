/* metrology_testing: Metrology and testing administration system (v1.0)
 * National standards, legal metrology, industrial metrology, scientific, testing service
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

#define MAX_NATIONAL     16
#define MAX_LEGAL        14
#define MAX_INDUSTRIAL   12
#define MAX_SCIENTIFIC   10
#define MAX_TESTING_SVC  10

typedef struct {
    int    national_id;
    int    standard_type;
    int    measurement_field;
    int    national_standards;
    int    work_standards;
    int    value_transfer;
    int    year;
    int    active;
} national_t;

typedef struct {
    int    legal_id;
    int    legal_type;
    int    instrument_category;
    int    mandatory_verifications;
    int    type_approvals;
    int    supervision_cases;
    int    year;
    int    active;
} legal_t;

typedef struct {
    int    industrial_id;
    int    industry_type;
    int    process_category;
    int    industrial_measurements;
    int    process_controls;
    int    online_monitoring;
    int    year;
    int    active;
} industrial_t;

typedef struct {
    int    scientific_id;
    int    research_area;
    int    innovation_type;
    int    research_projects;
    int    technologies_developed;
    int    intl_cooperation;
    int    year;
    int    active;
} scientific_t;

typedef struct {
    int    testing_id;
    int    testing_type;
    int    service_category;
    int    commissioned_tests;
    int    arbitration_tests;
    int    technical_services;
    int    year;
    int    active;
} testing_svc_t;

typedef struct {
    int    n_national;
    int    n_legal;
    int    n_industrial;
    int    n_scientific;
    int    n_testing_svc;
    int    total_national;
    int    total_verifications;
    int    total_measurements;
    int    total_projects;
    int    total_tests;
} mt_state_t;

static national_t nationals[MAX_NATIONAL];
static legal_t legals[MAX_LEGAL];
static industrial_t industrials[MAX_INDUSTRIAL];
static scientific_t scientifics[MAX_SCIENTIFIC];
static testing_svc_t testing_svcs[MAX_TESTING_SVC];
static mt_state_t mt;

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

int mt_init(void) {
    if (initialized) return -1;
    mt.n_national = 0; mt.n_legal = 0; mt.n_industrial = 0;
    mt.n_scientific = 0; mt.n_testing_svc = 0;
    mt.total_national = 0; mt.total_verifications = 0;
    mt.total_measurements = 0; mt.total_projects = 0;
    mt.total_tests = 0;
    for (int i = 0; i < MAX_NATIONAL; i++) nationals[i].active = 0;
    for (int i = 0; i < MAX_LEGAL; i++) legals[i].active = 0;
    for (int i = 0; i < MAX_INDUSTRIAL; i++) industrials[i].active = 0;
    for (int i = 0; i < MAX_SCIENTIFIC; i++) scientifics[i].active = 0;
    for (int i = 0; i < MAX_TESTING_SVC; i++) testing_svcs[i].active = 0;
    initialized = 1;
    print_str("[MT] Metrology testing initialized\n");
    return 0;
}

int mt_national(int std_type, int field, int natl, int work, int transfer, int year) {
    if (mt.n_national >= MAX_NATIONAL) return -1;
    national_t* n = &nationals[mt.n_national];
    n->national_id = mt.n_national;
    n->standard_type = std_type;
    n->measurement_field = field;
    n->national_standards = natl;
    n->work_standards = work;
    n->value_transfer = transfer;
    n->year = year;
    n->active = 1;
    mt.total_national += natl;
    mt.n_national++;
    print_str("[MT] National "); print_int(mt.n_national - 1);
    print_str(" type="); print_int(std_type);
    print_str(" fld="); print_int(field);
    print_str(" ntl="); print_int(natl);
    print_str(" wrk="); print_int(work);
    print_str(" trf="); print_int(transfer); print_str("\n");
    return mt.n_national - 1;
}

int mt_legal(int legal_type, int instrument, int mandatory, int approvals, int supervision, int year) {
    if (mt.n_legal >= MAX_LEGAL) return -1;
    legal_t* l = &legals[mt.n_legal];
    l->legal_id = mt.n_legal;
    l->legal_type = legal_type;
    l->instrument_category = instrument;
    l->mandatory_verifications = mandatory;
    l->type_approvals = approvals;
    l->supervision_cases = supervision;
    l->year = year;
    l->active = 1;
    mt.total_verifications += mandatory;
    mt.n_legal++;
    print_str("[MT] Legal "); print_int(mt.n_legal - 1);
    print_str(" type="); print_int(legal_type);
    print_str(" ins="); print_int(instrument);
    print_str(" mnd="); print_int(mandatory);
    print_str(" apr="); print_int(approvals);
    print_str(" sup="); print_int(supervision); print_str("\n");
    return mt.n_legal - 1;
}

int mt_industrial(int ind_type, int process, int measurements, int controls, int online, int year) {
    if (mt.n_industrial >= MAX_INDUSTRIAL) return -1;
    industrial_t* ind = &industrials[mt.n_industrial];
    ind->industrial_id = mt.n_industrial;
    ind->industry_type = ind_type;
    ind->process_category = process;
    ind->industrial_measurements = measurements;
    ind->process_controls = controls;
    ind->online_monitoring = online;
    ind->year = year;
    ind->active = 1;
    mt.total_measurements += measurements;
    mt.n_industrial++;
    print_str("[MT] Industrial "); print_int(mt.n_industrial - 1);
    print_str(" type="); print_int(ind_type);
    print_str(" prc="); print_int(process);
    print_str(" msr="); print_int(measurements);
    print_str(" ctl="); print_int(controls);
    print_str(" onl="); print_int(online); print_str("\n");
    return mt.n_industrial - 1;
}

int mt_scientific(int research, int innovation, int projects, int tech, int intl, int year) {
    if (mt.n_scientific >= MAX_SCIENTIFIC) return -1;
    scientific_t* sc = &scientifics[mt.n_scientific];
    sc->scientific_id = mt.n_scientific;
    sc->research_area = research;
    sc->innovation_type = innovation;
    sc->research_projects = projects;
    sc->technologies_developed = tech;
    sc->intl_cooperation = intl;
    sc->year = year;
    sc->active = 1;
    mt.total_projects += projects;
    mt.n_scientific++;
    print_str("[MT] Scientific "); print_int(mt.n_scientific - 1);
    print_str(" rsh="); print_int(research);
    print_str(" inn="); print_int(innovation);
    print_str(" prj="); print_int(projects);
    print_str(" tch="); print_int(tech);
    print_str(" int="); print_int(intl); print_str("\n");
    return mt.n_scientific - 1;
}

int mt_testing(int test_type, int service, int commissioned, int arbitration, int technical, int year) {
    if (mt.n_testing_svc >= MAX_TESTING_SVC) return -1;
    testing_svc_t* t = &testing_svcs[mt.n_testing_svc];
    t->testing_id = mt.n_testing_svc;
    t->testing_type = test_type;
    t->service_category = service;
    t->commissioned_tests = commissioned;
    t->arbitration_tests = arbitration;
    t->technical_services = technical;
    t->year = year;
    t->active = 1;
    mt.total_tests += commissioned;
    mt.n_testing_svc++;
    print_str("[MT] Testing "); print_int(mt.n_testing_svc - 1);
    print_str(" type="); print_int(test_type);
    print_str(" svc="); print_int(service);
    print_str(" cms="); print_int(commissioned);
    print_str(" arb="); print_int(arbitration);
    print_str(" tch="); print_int(technical); print_str("\n");
    return mt.n_testing_svc - 1;
}

void mt_national_report(void) {
    print_str("[MT] National report:\n");
    print_str("  National standard categories: "); print_int(mt.n_national); print_str("\n");
    print_str("  Total national standards: "); print_int(mt.total_national); print_str("\n");
}

void mt_legal_report(void) {
    print_str("[MT] Legal report:\n");
    print_str("  Legal metrology categories: "); print_int(mt.n_legal); print_str("\n");
    print_str("  Total mandatory verifications: "); print_int(mt.total_verifications); print_str("\n");
}

void mt_testing_report(void) {
    print_str("[MT] Testing report:\n");
    print_str("  Industrial metrology units: "); print_int(mt.n_industrial); print_str("\n");
    print_str("  Total industrial measurements: "); print_int(mt.total_measurements); print_str("\n");
    print_str("  Scientific research units: "); print_int(mt.n_scientific); print_str("\n");
    print_str("  Total research projects: "); print_int(mt.total_projects); print_str("\n");
    print_str("  Testing service units: "); print_int(mt.n_testing_svc); print_str("\n");
    print_str("  Total commissioned tests: "); print_int(mt.total_tests); print_str("\n");
}

void mt_print_state(void) {
    print_str("[MT] Nt="); print_int(mt.n_national);
    print_str(" Lg="); print_int(mt.n_legal);
    print_str(" In="); print_int(mt.n_industrial);
    print_str(" Sc="); print_int(mt.n_scientific);
    print_str(" Ts="); print_int(mt.n_testing_svc);
    print_str("\n");
}

int main(void) {
    print_str("=== Metrology Testing Demo ===\n\n");
    mt_init();

    print_str("National standards...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int fld = (i % 6) + 1;
        int ntl = 5 + (i * 2);
        int wrk = 20 + (i * 5);
        int trf = 50 + (i * 12);
        int year = 2020 + (i % 5);
        mt_national(type, fld, ntl, wrk, trf, year);
    }

    print_str("\nLegal metrology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int ins = (i % 5) + 1;
        int mnd = 100 + (i * 25);
        int apr = 30 + (i * 8);
        int sup = 15 + (i * 4);
        int year = 2021 + (i % 4);
        mt_legal(type, ins, mnd, apr, sup, year);
    }

    print_str("\nIndustrial metrology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int prc = (i % 5) + 1;
        int msr = 300 + (i * 80);
        int ctl = 100 + (i * 25);
        int onl = 50 + (i * 12);
        int year = 2022 + (i % 3);
        mt_industrial(type, prc, msr, ctl, onl, year);
    }

    print_str("\nScientific metrology...\n");
    for (int i = 0; i < 10; i++) {
        int rsh = (i % 5) + 1;
        int inn = (i % 4) + 1;
        int prj = 15 + (i * 4);
        int tch = 8 + (i * 2);
        int intl = 3 + (i % 4);
        int year = 2023 + (i % 2);
        mt_scientific(rsh, inn, prj, tch, intl, year);
    }

    print_str("\nTesting service...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int svc = (i % 5) + 1;
        int cms = 200 + (i * 50);
        int arb = 20 + (i * 5);
        int tch = 80 + (i * 20);
        int year = 2024;
        mt_testing(type, svc, cms, arb, tch, year);
    }

    print_str("\nNational report...\n");
    mt_national_report();

    print_str("\nLegal report...\n");
    mt_legal_report();

    print_str("\nTesting report...\n");
    mt_testing_report();

    print_str("\nFinal state...\n");
    mt_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
