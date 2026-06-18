/* public_housing: Public housing management (v1.0)
 * Housing units, applications, allocations, rent, maintenance
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

#define MAX_COMPLEXES   10
#define MAX_UNITS       20
#define MAX_APPLICANTS  16
#define MAX_ALLOCATIONS 12
#define MAINT_REQS      14
#define MAX_STAFF       10

typedef struct {
    int    complex_id;
    int    type;
    int    zone;
    int    n_units;
    int    n_occupied;
    int    year_built;
    int    staff;
    int    active;
} complex_t;

typedef struct {
    int    unit_id;
    int    complex_id;
    int    bedrooms;
    int    floor;
    int    area_sqm;
    int    rent;
    int    status;
    int    tenant_id;
    int    active;
} housing_unit_t;

typedef struct {
    int    app_id;
    int    applicant_id;
    int    income;
    int    family_size;
    int    type;
    int    score;
    int    year;
    int    status;
    int    active;
} application_t;

typedef struct {
    int    alloc_id;
    int    app_id;
    int    unit_id;
    int    tenant_id;
    int    rent;
    int    start_year;
    int    lease_years;
    int    paid_months;
    int    total_paid;
    int    active;
} allocation_t;

typedef struct {
    int    req_id;
    int    unit_id;
    int    type;
    int    priority;
    int    cost;
    int    status;
    int    day;
    int    active;
} maint_req_t;

typedef struct {
    int    n_complexes;
    int    n_units;
    int    n_applications;
    int    n_allocations;
    int    n_maint_reqs;
    int    n_staff;
    double total_rent_collected;
    double total_maint_cost;
    double total_revenue;
    int    total_applications;
    int    total_allocated;
    int    total_maint_done;
} ph_state_t;

static complex_t complexes[MAX_COMPLEXES];
static housing_unit_t units[MAX_UNITS];
static application_t applications[MAX_APPLICANTS];
static allocation_t allocations[MAX_ALLOCATIONS];
static maint_req_t maint_reqs[MAINT_REQS];
static ph_state_t ph;

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

int ph_init(void) {
    if (initialized) return -1;
    ph.n_complexes = 0; ph.n_units = 0; ph.n_applications = 0;
    ph.n_allocations = 0; ph.n_maint_reqs = 0; ph.n_staff = 0;
    ph.total_rent_collected = 0.0; ph.total_maint_cost = 0.0;
    ph.total_revenue = 0.0;
    ph.total_applications = 0; ph.total_allocated = 0;
    ph.total_maint_done = 0;
    for (int i = 0; i < MAX_COMPLEXES; i++) complexes[i].active = 0;
    for (int i = 0; i < MAX_UNITS; i++) units[i].active = 0;
    for (int i = 0; i < MAX_APPLICANTS; i++) applications[i].active = 0;
    for (int i = 0; i < MAX_ALLOCATIONS; i++) allocations[i].active = 0;
    for (int i = 0; i < MAINT_REQS; i++) maint_reqs[i].active = 0;
    initialized = 1;
    print_str("[PH] Public housing initialized\n");
    return 0;
}

int ph_add_complex(int type, int zone, int n_units, int year, int staff) {
    if (ph.n_complexes >= MAX_COMPLEXES) return -1;
    complex_t* c = &complexes[ph.n_complexes];
    c->complex_id = ph.n_complexes;
    c->type = type;
    c->zone = zone;
    c->n_units = n_units;
    c->n_occupied = 0;
    c->year_built = year;
    c->staff = staff;
    c->active = 1;
    ph.n_staff += staff;
    ph.n_complexes++;
    print_str("[PH] Complex "); print_int(ph.n_complexes - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" units="); print_int(n_units);
    print_str(" built="); print_int(year);
    print_str(" staff="); print_int(staff); print_str("\n");
    return ph.n_complexes - 1;
}

int ph_add_unit(int complex_id, int bedrooms, int floor, int area, int rent) {
    if (ph.n_units >= MAX_UNITS || complex_id >= ph.n_complexes) return -1;
    housing_unit_t* u = &units[ph.n_units];
    u->unit_id = ph.n_units;
    u->complex_id = complex_id;
    u->bedrooms = bedrooms;
    u->floor = floor;
    u->area_sqm = area;
    u->rent = rent;
    u->status = 1;
    u->tenant_id = -1;
    u->active = 1;
    ph.n_units++;
    print_str("[PH] Unit "); print_int(ph.n_units - 1);
    print_str(" C"); print_int(complex_id);
    print_str(" bed="); print_int(bedrooms);
    print_str(" fl="); print_int(floor);
    print_str(" area="); print_int(area);
    print_str(" rent=$"); print_int(rent); print_str("\n");
    return ph.n_units - 1;
}

int ph_apply(int applicant, int income, int family_size, int type, int year) {
    if (ph.n_applications >= MAX_APPLICANTS) return -1;
    application_t* a = &applications[ph.n_applications];
    a->app_id = ph.n_applications;
    a->applicant_id = applicant;
    a->income = income;
    a->family_size = family_size;
    a->type = type;
    a->score = 100 - (income / 1000) + (family_size * 10);
    a->year = year;
    a->status = 1;
    a->active = 1;
    ph.total_applications++;
    ph.n_applications++;
    print_str("[PH] App "); print_int(ph.n_applications - 1);
    print_str(" P"); print_int(applicant);
    print_str(" inc=$"); print_int(income);
    print_str(" fam="); print_int(family_size);
    print_str(" type="); print_int(type);
    print_str(" score="); print_int(a->score); print_str("\n");
    return ph.n_applications - 1;
}

int ph_allocate(int app_id, int unit_id, int lease_years) {
    if (ph.n_allocations >= MAX_ALLOCATIONS || app_id >= ph.n_applications || unit_id >= ph.n_units) return -1;
    allocation_t* al = &allocations[ph.n_allocations];
    al->alloc_id = ph.n_allocations;
    al->app_id = app_id;
    al->unit_id = unit_id;
    al->tenant_id = applications[app_id].applicant_id;
    al->rent = units[unit_id].rent;
    al->start_year = 2024;
    al->lease_years = lease_years;
    al->paid_months = 0;
    al->total_paid = 0;
    al->active = 1;
    units[unit_id].status = 2;
    units[unit_id].tenant_id = applications[app_id].applicant_id;
    complexes[units[unit_id].complex_id].n_occupied++;
    applications[app_id].status = 2;
    ph.total_allocated++;
    ph.n_allocations++;
    print_str("[PH] Alloc "); print_int(ph.n_allocations - 1);
    print_str(" A"); print_int(app_id);
    print_str(" U"); print_int(unit_id);
    print_str(" P"); print_int(applications[app_id].applicant_id);
    print_str(" rent=$"); print_int(units[unit_id].rent);
    print_str(" lease="); print_int(lease_years); print_str("yr\n");
    return ph.n_allocations - 1;
}

int ph_collect_rent(int alloc_id, int months) {
    if (alloc_id >= ph.n_allocations) return -1;
    allocation_t* al = &allocations[alloc_id];
    int payment = al->rent * months;
    al->paid_months += months;
    al->total_paid += payment;
    ph.total_rent_collected += payment;
    ph.total_revenue += payment;
    print_str("[PH] Rent Al"); print_int(alloc_id);
    print_str(" months="); print_int(months);
    print_str(" $"); print_int(payment); print_str("\n");
    return 0;
}

int ph_request_maint(int unit_id, int type, int priority, int cost, int day) {
    if (ph.n_maint_reqs >= MAINT_REQS || unit_id >= ph.n_units) return -1;
    maint_req_t* m = &maint_reqs[ph.n_maint_reqs];
    m->req_id = ph.n_maint_reqs;
    m->unit_id = unit_id;
    m->type = type;
    m->priority = priority;
    m->cost = cost;
    m->status = 1;
    m->day = day;
    m->active = 1;
    ph.n_maint_reqs++;
    print_str("[PH] Maint "); print_int(ph.n_maint_reqs - 1);
    print_str(" U"); print_int(unit_id);
    print_str(" type="); print_int(type);
    print_str(" pri="); print_int(priority);
    print_str(" $"); print_int(cost); print_str("\n");
    return ph.n_maint_reqs - 1;
}

int ph_complete_maint(int req_id) {
    if (req_id >= ph.n_maint_reqs) return -1;
    maint_reqs[req_id].status = 2;
    ph.total_maint_cost += maint_reqs[req_id].cost;
    ph.total_maint_done++;
    print_str("[PH] Done M"); print_int(req_id);
    print_str(" $"); print_int(maint_reqs[req_id].cost); print_str("\n");
    return 0;
}

int ph_vacate(int alloc_id) {
    if (alloc_id >= ph.n_allocations) return -1;
    int unit_id = allocations[alloc_id].unit_id;
    units[unit_id].status = 1;
    units[unit_id].tenant_id = -1;
    complexes[units[unit_id].complex_id].n_occupied--;
    allocations[alloc_id].active = 0;
    print_str("[PH] Vacate Al"); print_int(alloc_id);
    print_str(" U"); print_int(unit_id); print_str("\n");
    return 0;
}

void ph_complex_report(void) {
    print_str("[PH] Complex report:\n");
    for (int i = 0; i < ph.n_complexes; i++) {
        complex_t* c = &complexes[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" zone="); print_int(c->zone);
        print_str(" units="); print_int(c->n_units);
        print_str(" occ="); print_int(c->n_occupied);
        print_str(" built="); print_int(c->year_built);
        print_str(" staff="); print_int(c->staff); print_str("\n");
    }
}

void ph_unit_report(void) {
    print_str("[PH] Unit report:\n");
    for (int i = 0; i < ph.n_units; i++) {
        housing_unit_t* u = &units[i];
        print_str("  U"); print_int(i);
        print_str(" C"); print_int(u->complex_id);
        print_str(" bed="); print_int(u->bedrooms);
        print_str(" rent=$"); print_int(u->rent);
        print_str(" st="); print_int(u->status);
        if (u->tenant_id >= 0) {
            print_str(" P"); print_int(u->tenant_id);
        }
        print_str("\n");
    }
}

void ph_financial_report(void) {
    print_str("[PH] Financial report:\n");
    print_str("  Rent collected: "); print_int((int)ph.total_rent_collected); print_str("\n");
    print_str("  Maint cost: "); print_int((int)ph.total_maint_cost); print_str("\n");
    print_str("  Net revenue: "); print_int((int)ph.total_revenue - (int)ph.total_maint_cost); print_str("\n");
    print_str("  Applications: "); print_int(ph.total_applications); print_str("\n");
    print_str("  Allocated: "); print_int(ph.total_allocated); print_str("\n");
    print_str("  Maint done: "); print_int(ph.total_maint_done); print_str("\n");
}

void ph_print_state(void) {
    print_str("[PH] Complexes="); print_int(ph.n_complexes);
    print_str(" Units="); print_int(ph.n_units);
    print_str(" Apps="); print_int(ph.n_applications);
    print_str(" Allocs="); print_int(ph.n_allocations);
    print_str(" Maint="); print_int(ph.n_maint_reqs);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ph.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Public Housing Demo ===\n\n");
    ph_init();

    print_str("Adding complexes...\n");
    ph_add_complex(1, 1, 4, 2010, 5);
    ph_add_complex(1, 2, 3, 2012, 4);
    ph_add_complex(2, 1, 4, 2008, 6);
    ph_add_complex(2, 3, 3, 2015, 4);
    ph_add_complex(3, 2, 3, 2018, 5);
    ph_add_complex(3, 4, 2, 2020, 3);
    ph_add_complex(1, 3, 3, 2011, 4);
    ph_add_complex(2, 4, 2, 2016, 3);
    ph_add_complex(3, 1, 4, 2019, 5);
    ph_add_complex(1, 4, 2, 2014, 3);

    print_str("\nAdding housing units...\n");
    for (int i = 0; i < 20; i++) {
        int complex = i % 10;
        int bedrooms = 1 + (i % 4);
        int floor = 1 + (i / 5);
        int area = 30 + bedrooms * 15;
        int rent = 800 + bedrooms * 400;
        ph_add_unit(complex, bedrooms, floor, area, rent);
    }

    print_str("\nReceiving applications...\n");
    for (int i = 0; i < 16; i++) {
        int applicant = 1000 + i;
        int income = 20000 + (i * 5000);
        int family = 1 + (i % 5);
        int type = (i % 3) + 1;
        int year = 2023 + (i % 2);
        ph_apply(applicant, income, family, type, year);
    }

    print_str("\nAllocating units...\n");
    for (int i = 0; i < 12; i++) {
        int lease = 1 + (i % 3);
        ph_allocate(i, i, lease);
    }

    print_str("\nCollecting rent...\n");
    for (int i = 0; i < 12; i++) {
        int months = 3 + (i % 6);
        ph_collect_rent(i, months);
    }

    print_str("\nMaintenance requests...\n");
    for (int i = 0; i < 14; i++) {
        int unit = i % 20;
        int type = (i % 4) + 1;
        int pri = (i % 3) + 1;
        int cost = 500 + (i * 300);
        int day = 1 + (i % 28);
        ph_request_maint(unit, type, pri, cost, day);
    }

    print_str("\nCompleting maintenance...\n");
    for (int i = 0; i < 14; i++) {
        ph_complete_maint(i);
    }

    print_str("\nVacating some units...\n");
    ph_vacate(0);
    ph_vacate(3);
    ph_vacate(7);
    ph_vacate(11);

    print_str("\nComplex report...\n");
    ph_complex_report();

    print_str("\nUnit report...\n");
    ph_unit_report();

    print_str("\nFinancial report...\n");
    ph_financial_report();

    print_str("\nFinal state...\n");
    ph_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
