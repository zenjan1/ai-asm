/* veterans_affairs: Veterans affairs management (v1.0)
 * Placement, benefits, service management, rights, honors
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

#define MAX_PLACEMENT    16
#define MAX_BENEFIT      14
#define MAX_SERVICE      12
#define MAX_LEGAL        10
#define MAX_HONOR        10

typedef struct {
    int    placement_id;
    int    veteran_id;
    int    type;
    int    position;
    int    training_hours;
    int    salary;
    int    employer_id;
    int    year;
    int    active;
} placement_t;

typedef struct {
    int    benefit_id;
    int    veteran_id;
    int    category;
    int    pension;
    int    medical;
    int    housing_subsidy;
    int    disability_pct;
    int    year;
    int    active;
} benefit_t;

typedef struct {
    int    service_id;
    int    veteran_id;
    int    type;
    int    rank;
    int    service_years;
    int    decorations;
    int    transfer_status;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    legal_id;
    int    veteran_id;
    int    case_type;
    int    lawyer_id;
    int    status;
    int    resolution;
    int    year;
    int    active;
} legal_t;

typedef struct {
    int    honor_id;
    int    veteran_id;
    int    award_type;
    int    citation;
    int    ceremony_id;
    int    year;
    int    active;
} honor_t;

typedef struct {
    int    n_placement;
    int    n_benefit;
    int    n_service;
    int    n_legal;
    int    n_honor;
    int    total_pension;
    int    total_training;
    int    total_placed;
    int    total_medical;
    int    total_decorations;
} va_state_t;

static placement_t placements[MAX_PLACEMENT];
static benefit_t benefits[MAX_BENEFIT];
static service_t services[MAX_SERVICE];
static legal_t legal_cases[MAX_LEGAL];
static honor_t honors[MAX_HONOR];
static va_state_t va;

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

int va_init(void) {
    if (initialized) return -1;
    va.n_placement = 0; va.n_benefit = 0; va.n_service = 0;
    va.n_legal = 0; va.n_honor = 0;
    va.total_pension = 0; va.total_training = 0;
    va.total_placed = 0; va.total_medical = 0;
    va.total_decorations = 0;
    for (int i = 0; i < MAX_PLACEMENT; i++) placements[i].active = 0;
    for (int i = 0; i < MAX_BENEFIT; i++) benefits[i].active = 0;
    for (int i = 0; i < MAX_SERVICE; i++) services[i].active = 0;
    for (int i = 0; i < MAX_LEGAL; i++) legal_cases[i].active = 0;
    for (int i = 0; i < MAX_HONOR; i++) honors[i].active = 0;
    initialized = 1;
    print_str("[VA] Veterans affairs initialized\n");
    return 0;
}

int va_place(int veteran, int type, int position, int training, int salary, int employer, int year) {
    if (va.n_placement >= MAX_PLACEMENT) return -1;
    placement_t* p = &placements[va.n_placement];
    p->placement_id = va.n_placement;
    p->veteran_id = veteran;
    p->type = type;
    p->position = position;
    p->training_hours = training;
    p->salary = salary;
    p->employer_id = employer;
    p->year = year;
    p->active = 1;
    va.total_training += training;
    va.total_placed++;
    va.n_placement++;
    print_str("[VA] Placement "); print_int(va.n_placement - 1);
    print_str(" vet="); print_int(veteran);
    print_str(" type="); print_int(type);
    print_str(" pos="); print_int(position);
    print_str(" trn="); print_int(training); print_str("h");
    print_str(" sal=$"); print_int(salary); print_str("\n");
    return va.n_placement - 1;
}

int va_benefit(int veteran, int category, int pension, int medical, int housing, int disability, int year) {
    if (va.n_benefit >= MAX_BENEFIT) return -1;
    benefit_t* b = &benefits[va.n_benefit];
    b->benefit_id = va.n_benefit;
    b->veteran_id = veteran;
    b->category = category;
    b->pension = pension;
    b->medical = medical;
    b->housing_subsidy = housing;
    b->disability_pct = disability;
    b->year = year;
    b->active = 1;
    va.total_pension += pension;
    va.total_medical += medical;
    va.n_benefit++;
    print_str("[VA] Benefit "); print_int(va.n_benefit - 1);
    print_str(" vet="); print_int(veteran);
    print_str(" cat="); print_int(category);
    print_str(" pen=$"); print_int(pension);
    print_str(" med=$"); print_int(medical);
    print_str(" hsg=$"); print_int(housing);
    print_str(" dis="); print_int(disability); print_str("%\n");
    return va.n_benefit - 1;
}

int va_service_record(int veteran, int type, int rank, int years, int decorations, int transfer, int year) {
    if (va.n_service >= MAX_SERVICE) return -1;
    service_t* s = &services[va.n_service];
    s->service_id = va.n_service;
    s->veteran_id = veteran;
    s->type = type;
    s->rank = rank;
    s->service_years = years;
    s->decorations = decorations;
    s->transfer_status = transfer;
    s->year = year;
    s->active = 1;
    va.total_decorations += decorations;
    va.n_service++;
    print_str("[VA] Service "); print_int(va.n_service - 1);
    print_str(" vet="); print_int(veteran);
    print_str(" type="); print_int(type);
    print_str(" rank="); print_int(rank);
    print_str(" yrs="); print_int(years);
    print_str(" dec="); print_int(decorations); print_str("\n");
    return va.n_service - 1;
}

int va_legal(int veteran, int case_type, int lawyer, int status, int resolution, int year) {
    if (va.n_legal >= MAX_LEGAL) return -1;
    legal_t* l = &legal_cases[va.n_legal];
    l->legal_id = va.n_legal;
    l->veteran_id = veteran;
    l->case_type = case_type;
    l->lawyer_id = lawyer;
    l->status = status;
    l->resolution = resolution;
    l->year = year;
    l->active = 1;
    va.n_legal++;
    print_str("[VA] Legal "); print_int(va.n_legal - 1);
    print_str(" vet="); print_int(veteran);
    print_str(" type="); print_int(case_type);
    print_str(" lwy="); print_int(lawyer);
    print_str(" st="); print_int(status);
    print_str(" res="); print_int(resolution); print_str("\n");
    return va.n_legal - 1;
}

int va_honor(int veteran, int award, int citation, int ceremony, int year) {
    if (va.n_honor >= MAX_HONOR) return -1;
    honor_t* h = &honors[va.n_honor];
    h->honor_id = va.n_honor;
    h->veteran_id = veteran;
    h->award_type = award;
    h->citation = citation;
    h->ceremony_id = ceremony;
    h->year = year;
    h->active = 1;
    va.n_honor++;
    print_str("[VA] Honor "); print_int(va.n_honor - 1);
    print_str(" vet="); print_int(veteran);
    print_str(" awd="); print_int(award);
    print_str(" cit="); print_int(citation);
    print_str(" crm="); print_int(ceremony); print_str("\n");
    return va.n_honor - 1;
}

void va_placement_report(void) {
    print_str("[VA] Placement report:\n");
    print_str("  Placed: "); print_int(va.total_placed); print_str("\n");
    print_str("  Total training hours: "); print_int(va.total_training); print_str("\n");
}

void va_benefit_report(void) {
    print_str("[VA] Benefit report:\n");
    print_str("  Beneficiaries: "); print_int(va.n_benefit); print_str("\n");
    print_str("  Total pension: $"); print_int(va.total_pension); print_str("\n");
    print_str("  Total medical: $"); print_int(va.total_medical); print_str("\n");
}

void va_service_report(void) {
    print_str("[VA] Service report:\n");
    print_str("  Service records: "); print_int(va.n_service); print_str("\n");
    print_str("  Total decorations: "); print_int(va.total_decorations); print_str("\n");
    print_str("  Legal cases: "); print_int(va.n_legal); print_str("\n");
    print_str("  Honors: "); print_int(va.n_honor); print_str("\n");
}

void va_print_state(void) {
    print_str("[VA] Pl="); print_int(va.n_placement);
    print_str(" Bn="); print_int(va.n_benefit);
    print_str(" Sv="); print_int(va.n_service);
    print_str(" Lg="); print_int(va.n_legal);
    print_str(" Hn="); print_int(va.n_honor);
    print_str("\n");
}

int main(void) {
    print_str("=== Veterans Affairs Demo ===\n\n");
    va_init();

    print_str("Employment placement...\n");
    for (int i = 0; i < 16; i++) {
        int vet = 1000 + (i * 17);
        int type = (i % 4) + 1;
        int pos = 100 + (i * 5);
        int trn = 40 + (i * 20);
        int sal = 3000 + (i * 500);
        int emp = 2000 + (i * 7);
        int year = 2021 + (i % 4);
        va_place(vet, type, pos, trn, sal, emp, year);
    }

    print_str("\nBenefits distribution...\n");
    for (int i = 0; i < 14; i++) {
        int vet = 3000 + (i * 11);
        int cat = (i % 5) + 1;
        int pen = 1000 + (i * 300);
        int med = 500 + (i * 200);
        int hsg = (i % 3 == 0) ? 2000 + (i * 500) : 0;
        int dis = (i % 4) * 10;
        int year = 2022 + (i % 3);
        va_benefit(vet, cat, pen, med, hsg, dis, year);
    }

    print_str("\nService records...\n");
    for (int i = 0; i < 12; i++) {
        int vet = 5000 + (i * 13);
        int type = (i % 3) + 1;
        int rank = 1 + (i % 8);
        int yrs = 2 + (i % 15);
        int dec = (i % 4);
        int xfr = (i % 3 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        va_service_record(vet, type, rank, yrs, dec, xfr, year);
    }

    print_str("\nLegal assistance...\n");
    for (int i = 0; i < 10; i++) {
        int vet = 7000 + (i * 7);
        int ctype = (i % 4) + 1;
        int lwy = 800 + (i % 6);
        int st = (i % 3) + 1;
        int res = (i % 4 == 0) ? 0 : 1;
        int year = 2024;
        va_legal(vet, ctype, lwy, st, res, year);
    }

    print_str("\nHonors and awards...\n");
    for (int i = 0; i < 10; i++) {
        int vet = 9000 + (i * 11);
        int awd = (i % 5) + 1;
        int cit = 100 + (i * 10);
        int crm = 500 + (i % 4);
        int year = 2024;
        va_honor(vet, awd, cit, crm, year);
    }

    print_str("\nPlacement report...\n");
    va_placement_report();

    print_str("\nBenefit report...\n");
    va_benefit_report();

    print_str("\nService report...\n");
    va_service_report();

    print_str("\nFinal state...\n");
    va_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
