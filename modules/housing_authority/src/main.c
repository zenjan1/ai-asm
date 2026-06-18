/* housing_authority: Housing authority management (v1.0)
 * Social housing, applications, property mgmt, safety, provident fund
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

#define MAX_HOUSING      16
#define MAX_APPS         18
#define MAX_PROPERTY     12
#define MAX_SAFETY       10
#define MAX_FUND         10

typedef struct {
    int    housing_id;
    int    type;
    int    location_id;
    int    units;
    int    area_m2;
    int    rent;
    int    year_built;
    int    occupancy;
    int    active;
} housing_t;

typedef struct {
    int    app_id;
    int    applicant_id;
    int    income;
    int    family_size;
    int    housing_type;
    int    score;
    int    rank;
    int    year;
    int    active;
} application_t;

typedef struct {
    int    prop_id;
    int    housing_id;
    int    company_id;
    int    managed_units;
    int    maintenance_fund;
    int    owner_committee;
    int    satisfaction;
    int    year;
    int    active;
} property_t;

typedef struct {
    int    safety_id;
    int    building_id;
    int    type;
    int    age_years;
    int    risk_level;
    int    reinforcement;
    int    year_inspected;
    int    status;
    int    active;
} safety_t;

typedef struct {
    int    fund_id;
    int    participant_id;
    int    monthly_deposit;
    int    total_balance;
    int    loan_amount;
    int    withdrawal;
    int    year;
    int    active;
} fund_t;

typedef struct {
    int    n_housing;
    int    n_apps;
    int    n_property;
    int    n_safety;
    int    n_fund;
    int    total_units;
    int    total_allocated;
    int    total_maintenance;
    int    total_balance;
    int    total_loans;
} ha_state_t;

static housing_t housing_units[MAX_HOUSING];
static application_t applications[MAX_APPS];
static property_t properties[MAX_PROPERTY];
static safety_t safety_records[MAX_SAFETY];
static fund_t funds[MAX_FUND];
static ha_state_t ha;

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

int ha_init(void) {
    if (initialized) return -1;
    ha.n_housing = 0; ha.n_apps = 0; ha.n_property = 0;
    ha.n_safety = 0; ha.n_fund = 0;
    ha.total_units = 0; ha.total_allocated = 0;
    ha.total_maintenance = 0; ha.total_balance = 0;
    ha.total_loans = 0;
    for (int i = 0; i < MAX_HOUSING; i++) housing_units[i].active = 0;
    for (int i = 0; i < MAX_APPS; i++) applications[i].active = 0;
    for (int i = 0; i < MAX_PROPERTY; i++) properties[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safety_records[i].active = 0;
    for (int i = 0; i < MAX_FUND; i++) funds[i].active = 0;
    initialized = 1;
    print_str("[HA] Housing authority initialized\n");
    return 0;
}

int ha_add_housing(int type, int location, int units, int area, int rent, int year) {
    if (ha.n_housing >= MAX_HOUSING) return -1;
    housing_t* h = &housing_units[ha.n_housing];
    h->housing_id = ha.n_housing;
    h->type = type;
    h->location_id = location;
    h->units = units;
    h->area_m2 = area;
    h->rent = rent;
    h->year_built = year;
    h->occupancy = 0;
    h->active = 1;
    ha.total_units += units;
    ha.n_housing++;
    print_str("[HA] Housing "); print_int(ha.n_housing - 1);
    print_str(" type="); print_int(type);
    print_str(" loc="); print_int(location);
    print_str(" units="); print_int(units);
    print_str(" area="); print_int(area); print_str("m2");
    print_str(" rent=$"); print_int(rent); print_str("\n");
    return ha.n_housing - 1;
}

int ha_apply(int applicant, int income, int family, int housing_type, int score, int year) {
    if (ha.n_apps >= MAX_APPS) return -1;
    application_t* a = &applications[ha.n_apps];
    a->app_id = ha.n_apps;
    a->applicant_id = applicant;
    a->income = income;
    a->family_size = family;
    a->housing_type = housing_type;
    a->score = score;
    a->rank = ha.n_apps + 1;
    a->year = year;
    a->active = 1;
    ha.n_apps++;
    print_str("[HA] App "); print_int(ha.n_apps - 1);
    print_str(" app="); print_int(applicant);
    print_str(" inc=$"); print_int(income);
    print_str(" fam="); print_int(family);
    print_str(" type="); print_int(housing_type);
    print_str(" score="); print_int(score); print_str("\n");
    return ha.n_apps - 1;
}

int ha_allocate(int app_id, int housing_id) {
    if (app_id >= ha.n_apps || housing_id >= ha.n_housing) return -1;
    housing_units[housing_id].occupancy++;
    ha.total_allocated++;
    print_str("[HA] Allocate App "); print_int(app_id);
    print_str(" -> Housing "); print_int(housing_id); print_str("\n");
    return 0;
}

int ha_add_property(int housing, int company, int managed, int fund, int committee, int satisfaction, int year) {
    if (ha.n_property >= MAX_PROPERTY) return -1;
    property_t* p = &properties[ha.n_property];
    p->prop_id = ha.n_property;
    p->housing_id = housing;
    p->company_id = company;
    p->managed_units = managed;
    p->maintenance_fund = fund;
    p->owner_committee = committee;
    p->satisfaction = satisfaction;
    p->year = year;
    p->active = 1;
    ha.total_maintenance += fund;
    ha.n_property++;
    print_str("[HA] Property "); print_int(ha.n_property - 1);
    print_str(" hsg="); print_int(housing);
    print_str(" co="); print_int(company);
    print_str(" mgd="); print_int(managed);
    print_str(" fund=$"); print_int(fund);
    print_str(" sat="); print_int(satisfaction); print_str("%\n");
    return ha.n_property - 1;
}

int ha_inspect_safety(int building, int type, int age, int risk, int reinforcement, int year) {
    if (ha.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safety_records[ha.n_safety];
    s->safety_id = ha.n_safety;
    s->building_id = building;
    s->type = type;
    s->age_years = age;
    s->risk_level = risk;
    s->reinforcement = reinforcement;
    s->year_inspected = year;
    s->status = 1;
    s->active = 1;
    ha.n_safety++;
    print_str("[HA] Safety "); print_int(ha.n_safety - 1);
    print_str(" bld="); print_int(building);
    print_str(" type="); print_int(type);
    print_str(" age="); print_int(age); print_str("yr");
    print_str(" risk="); print_int(risk);
    print_str(" rnf="); print_int(reinforcement); print_str("\n");
    return ha.n_safety - 1;
}

int ha_add_fund(int participant, int deposit, int balance, int loan, int withdrawal, int year) {
    if (ha.n_fund >= MAX_FUND) return -1;
    fund_t* f = &funds[ha.n_fund];
    f->fund_id = ha.n_fund;
    f->participant_id = participant;
    f->monthly_deposit = deposit;
    f->total_balance = balance;
    f->loan_amount = loan;
    f->withdrawal = withdrawal;
    f->year = year;
    f->active = 1;
    ha.total_balance += balance;
    ha.total_loans += loan;
    ha.n_fund++;
    print_str("[HA] Fund "); print_int(ha.n_fund - 1);
    print_str(" ptc="); print_int(participant);
    print_str(" dep=$"); print_int(deposit);
    print_str(" bal=$"); print_int(balance);
    print_str(" loan=$"); print_int(loan); print_str("\n");
    return ha.n_fund - 1;
}

void ha_housing_report(void) {
    print_str("[HA] Housing report:\n");
    print_str("  Properties: "); print_int(ha.n_housing); print_str("\n");
    print_str("  Total units: "); print_int(ha.total_units); print_str("\n");
    print_str("  Allocated: "); print_int(ha.total_allocated); print_str("\n");
}

void ha_application_report(void) {
    print_str("[HA] Application report:\n");
    print_str("  Applications: "); print_int(ha.n_apps); print_str("\n");
    print_str("  Property mgmt: "); print_int(ha.n_property); print_str("\n");
    print_str("  Maintenance fund: $"); print_int(ha.total_maintenance); print_str("\n");
}

void ha_fund_report(void) {
    print_str("[HA] Fund report:\n");
    print_str("  Participants: "); print_int(ha.n_fund); print_str("\n");
    print_str("  Total balance: $"); print_int(ha.total_balance); print_str("\n");
    print_str("  Total loans: $"); print_int(ha.total_loans); print_str("\n");
}

void ha_print_state(void) {
    print_str("[HA] Hs="); print_int(ha.n_housing);
    print_str(" Ap="); print_int(ha.n_apps);
    print_str(" Pr="); print_int(ha.n_property);
    print_str(" Sf="); print_int(ha.n_safety);
    print_str(" Fn="); print_int(ha.n_fund);
    print_str("\n");
}

int main(void) {
    print_str("=== Housing Authority Demo ===\n\n");
    ha_init();

    print_str("Adding housing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 3) + 1;
        int loc = (i % 8) + 1;
        int units = 50 + (i * 20);
        int area = 40 + (i * 5);
        int rent = 500 + (i * 100);
        int year = 2010 + (i % 12);
        ha_add_housing(type, loc, units, area, rent, year);
    }

    print_str("\nReceiving applications...\n");
    for (int i = 0; i < 18; i++) {
        int app = 3000 + (i * 13);
        int inc = 2000 + (i * 200);
        int fam = 1 + (i % 5);
        int type = (i % 3) + 1;
        int score = 50 + (i * 3);
        int year = 2022 + (i % 3);
        ha_apply(app, inc, fam, type, score, year);
    }

    print_str("\nAllocating housing...\n");
    for (int i = 0; i < 14; i++) {
        ha_allocate(i, i % 16);
    }

    print_str("\nProperty management...\n");
    for (int i = 0; i < 12; i++) {
        int hsg = i % 16;
        int co = 4000 + (i * 5);
        int mgd = 30 + (i * 10);
        int fund = 50000 + (i * 20000);
        int cmte = (i % 3) + 1;
        int sat = 60 + (i * 3);
        int year = 2023 + (i % 2);
        ha_add_property(hsg, co, mgd, fund, cmte, sat, year);
    }

    print_str("\nSafety inspections...\n");
    for (int i = 0; i < 10; i++) {
        int bld = 5000 + (i * 100);
        int type = (i % 3) + 1;
        int age = 5 + (i * 3);
        int risk = (i % 4) + 1;
        int rnf = (i % 3 == 0) ? 1 : 0;
        int year = 2024;
        ha_inspect_safety(bld, type, age, risk, rnf, year);
    }

    print_str("\nProvident fund...\n");
    for (int i = 0; i < 10; i++) {
        int ptc = 6000 + (i * 11);
        int dep = 500 + (i * 100);
        int bal = 20000 + (i * 10000);
        int loan = (i % 3 == 0) ? 100000 + (i * 50000) : 0;
        int wth = (i % 4 == 0) ? 5000 + (i * 2000) : 0;
        int year = 2024;
        ha_add_fund(ptc, dep, bal, loan, wth, year);
    }

    print_str("\nHousing report...\n");
    ha_housing_report();

    print_str("\nApplication report...\n");
    ha_application_report();

    print_str("\nFund report...\n");
    ha_fund_report();

    print_str("\nFinal state...\n");
    ha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
