/* city_management: City management system (v1.0)
 * Municipal facilities, landscaping, sanitation, enforcement, digital mgmt
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

#define MAX_FACILITY     16
#define MAX_GREEN        14
#define MAX_SANITATION   12
#define MAX_ENFORCE      10
#define MAX_DIGITAL      10

typedef struct {
    int    facility_id;
    int    type;
    int    location_id;
    int    length_km;
    int    condition;
    int    last_maintenance;
    int    budget;
    int    year;
    int    active;
} facility_t;

typedef struct {
    int    green_id;
    int    type;
    int    location_id;
    int    area_m2;
    int    trees;
    int    maintenance_score;
    int    year;
    int    status;
    int    active;
} green_t;

typedef struct {
    int    sanit_id;
    int    type;
    int    zone_id;
    int    volume_tons;
    int    workers;
    int    coverage_pct;
    int    year;
    int    active;
} sanitation_t;

typedef struct {
    int    enforce_id;
    int    violation_type;
    int    location_id;
    int    offender_id;
    int    fine;
    int    corrected;
    int    year;
    int    status;
    int    active;
} enforcement_t;

typedef struct {
    int    case_id;
    int    type;
    int    reporter_id;
    int    location_id;
    int    assigned_to;
    int    status;
    int    resolution_time_hours;
    int    year;
    int    active;
} digital_case_t;

typedef struct {
    int    n_facility;
    int    n_green;
    int    n_sanitation;
    int    n_enforce;
    int    n_digital;
    int    total_length;
    int    total_green_area;
    int    total_waste;
    int    total_fines;
    int    total_resolved;
} cm_state_t;

static facility_t facilities[MAX_FACILITY];
static green_t greens[MAX_GREEN];
static sanitation_t sanitation[MAX_SANITATION];
static enforcement_t enforcement[MAX_ENFORCE];
static digital_case_t digital_cases[MAX_DIGITAL];
static cm_state_t cm;

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

int cm_init(void) {
    if (initialized) return -1;
    cm.n_facility = 0; cm.n_green = 0; cm.n_sanitation = 0;
    cm.n_enforce = 0; cm.n_digital = 0;
    cm.total_length = 0; cm.total_green_area = 0;
    cm.total_waste = 0; cm.total_fines = 0;
    cm.total_resolved = 0;
    for (int i = 0; i < MAX_FACILITY; i++) facilities[i].active = 0;
    for (int i = 0; i < MAX_GREEN; i++) greens[i].active = 0;
    for (int i = 0; i < MAX_SANITATION; i++) sanitation[i].active = 0;
    for (int i = 0; i < MAX_ENFORCE; i++) enforcement[i].active = 0;
    for (int i = 0; i < MAX_DIGITAL; i++) digital_cases[i].active = 0;
    initialized = 1;
    print_str("[CM] City management initialized\n");
    return 0;
}

int cm_add_facility(int type, int location, int length, int condition, int maint, int budget, int year) {
    if (cm.n_facility >= MAX_FACILITY) return -1;
    facility_t* f = &facilities[cm.n_facility];
    f->facility_id = cm.n_facility;
    f->type = type;
    f->location_id = location;
    f->length_km = length;
    f->condition = condition;
    f->last_maintenance = maint;
    f->budget = budget;
    f->year = year;
    f->active = 1;
    cm.total_length += length;
    cm.n_facility++;
    print_str("[CM] Facility "); print_int(cm.n_facility - 1);
    print_str(" type="); print_int(type);
    print_str(" loc="); print_int(location);
    print_str(" len="); print_int(length); print_str("km");
    print_str(" cond="); print_int(condition);
    print_str(" bud=$"); print_int(budget); print_str("\n");
    return cm.n_facility - 1;
}

int cm_add_green(int type, int location, int area, int trees, int score, int year) {
    if (cm.n_green >= MAX_GREEN) return -1;
    green_t* g = &greens[cm.n_green];
    g->green_id = cm.n_green;
    g->type = type;
    g->location_id = location;
    g->area_m2 = area;
    g->trees = trees;
    g->maintenance_score = score;
    g->year = year;
    g->status = 1;
    g->active = 1;
    cm.total_green_area += area;
    cm.n_green++;
    print_str("[CM] Green "); print_int(cm.n_green - 1);
    print_str(" type="); print_int(type);
    print_str(" loc="); print_int(location);
    print_str(" area="); print_int(area); print_str("m2");
    print_str(" trees="); print_int(trees);
    print_str(" score="); print_int(score); print_str("\n");
    return cm.n_green - 1;
}

int cm_add_sanitation(int type, int zone, int volume, int workers, int coverage, int year) {
    if (cm.n_sanitation >= MAX_SANITATION) return -1;
    sanitation_t* s = &sanitation[cm.n_sanitation];
    s->sanit_id = cm.n_sanitation;
    s->type = type;
    s->zone_id = zone;
    s->volume_tons = volume;
    s->workers = workers;
    s->coverage_pct = coverage;
    s->year = year;
    s->active = 1;
    cm.total_waste += volume;
    cm.n_sanitation++;
    print_str("[CM] Sanitation "); print_int(cm.n_sanitation - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" vol="); print_int(volume); print_str("t");
    print_str(" wrk="); print_int(workers);
    print_str(" cov="); print_int(coverage); print_str("%\n");
    return cm.n_sanitation - 1;
}

int cm_enforce(int violation, int location, int offender, int fine, int corrected, int year) {
    if (cm.n_enforce >= MAX_ENFORCE) return -1;
    enforcement_t* e = &enforcement[cm.n_enforce];
    e->enforce_id = cm.n_enforce;
    e->violation_type = violation;
    e->location_id = location;
    e->offender_id = offender;
    e->fine = fine;
    e->corrected = corrected;
    e->year = year;
    e->status = 1;
    e->active = 1;
    cm.total_fines += fine;
    cm.n_enforce++;
    print_str("[CM] Enforce "); print_int(cm.n_enforce - 1);
    print_str(" viol="); print_int(violation);
    print_str(" loc="); print_int(location);
    print_str(" off="); print_int(offender);
    print_str(" fine=$"); print_int(fine);
    print_str(" corr="); print_int(corrected); print_str("\n");
    return cm.n_enforce - 1;
}

int cm_digital_case(int type, int reporter, int location, int assigned, int status, int hours, int year) {
    if (cm.n_digital >= MAX_DIGITAL) return -1;
    digital_case_t* c = &digital_cases[cm.n_digital];
    c->case_id = cm.n_digital;
    c->type = type;
    c->reporter_id = reporter;
    c->location_id = location;
    c->assigned_to = assigned;
    c->status = status;
    c->resolution_time_hours = hours;
    c->year = year;
    c->active = 1;
    if (status == 3) cm.total_resolved++;
    cm.n_digital++;
    print_str("[CM] Case "); print_int(cm.n_digital - 1);
    print_str(" type="); print_int(type);
    print_str(" rpt="); print_int(reporter);
    print_str(" loc="); print_int(location);
    print_str(" asgn="); print_int(assigned);
    print_str(" hrs="); print_int(hours); print_str("\n");
    return cm.n_digital - 1;
}

void cm_facility_report(void) {
    print_str("[CM] Facility report:\n");
    print_str("  Facilities: "); print_int(cm.n_facility); print_str("\n");
    print_str("  Total length: "); print_int(cm.total_length); print_str(" km\n");
}

void cm_environment_report(void) {
    print_str("[CM] Environment report:\n");
    print_str("  Green spaces: "); print_int(cm.n_green); print_str("\n");
    print_str("  Total area: "); print_int(cm.total_green_area); print_str(" m2\n");
    print_str("  Total waste: "); print_int(cm.total_waste); print_str(" tons\n");
}

void cm_enforcement_report(void) {
    print_str("[CM] Enforcement report:\n");
    print_str("  Cases: "); print_int(cm.n_enforce); print_str("\n");
    print_str("  Total fines: $"); print_int(cm.total_fines); print_str("\n");
    print_str("  Digital cases resolved: "); print_int(cm.total_resolved); print_str("\n");
}

void cm_print_state(void) {
    print_str("[CM] Fc="); print_int(cm.n_facility);
    print_str(" Gr="); print_int(cm.n_green);
    print_str(" Sn="); print_int(cm.n_sanitation);
    print_str(" En="); print_int(cm.n_enforce);
    print_str(" Dg="); print_int(cm.n_digital);
    print_str("\n");
}

int main(void) {
    print_str("=== City Management Demo ===\n\n");
    cm_init();

    print_str("Adding facilities...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int loc = (i % 10) + 1;
        int len = 2 + (i % 8);
        int cond = 60 + (i * 3);
        int maint = 20230101 + (i * 30);
        int budget = 100000 + (i * 50000);
        int year = 2020 + (i % 5);
        cm_add_facility(type, loc, len, cond, maint, budget, year);
    }

    print_str("\nManaging green spaces...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 3) + 1;
        int loc = (i % 10) + 1;
        int area = 500 + (i * 200);
        int trees = 20 + (i * 10);
        int score = 60 + (i * 3);
        int year = 2022 + (i % 3);
        cm_add_green(type, loc, area, trees, score, year);
    }

    print_str("\nSanitation services...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 8) + 1;
        int vol = 10 + (i * 5);
        int wrk = 5 + (i * 2);
        int cov = 70 + (i * 2);
        int year = 2023 + (i % 2);
        cm_add_sanitation(type, zone, vol, wrk, cov, year);
    }

    print_str("\nEnforcement actions...\n");
    for (int i = 0; i < 10; i++) {
        int viol = (i % 4) + 1;
        int loc = (i % 10) + 1;
        int off = 7000 + (i * 11);
        int fine = 200 + (i * 100);
        int corr = (i % 3 == 0) ? 0 : 1;
        int year = 2024;
        cm_enforce(viol, loc, off, fine, corr, year);
    }

    print_str("\nDigital city cases...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int rpt = 8000 + (i * 7);
        int loc = (i % 10) + 1;
        int asgn = 900 + (i % 8);
        int st = (i % 4) + 1;
        int hrs = 2 + (i * 3);
        int year = 2024;
        cm_digital_case(type, rpt, loc, asgn, st, hrs, year);
    }

    print_str("\nFacility report...\n");
    cm_facility_report();

    print_str("\nEnvironment report...\n");
    cm_environment_report();

    print_str("\nEnforcement report...\n");
    cm_enforcement_report();

    print_str("\nFinal state...\n");
    cm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
