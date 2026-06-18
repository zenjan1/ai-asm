/* police_department: Police department management (v1.0)
 * Stations, cases, patrols, traffic, personnel
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

#define MAX_STATIONS   12
#define MAX_OFFICERS   18
#define MAX_CASES      20
#define MAX_PATROLS    14
#define MAX_TRAFFIC    16

typedef struct {
    int    station_id;
    int    zone;
    int    capacity;
    int    officers;
    int    vehicles;
    int    jurisdiction;
    int    year_built;
    int    active;
} station_t;

typedef struct {
    int    officer_id;
    int    station_id;
    int    rank;
    int    badge_number;
    int    years_service;
    int    specialty;
    int    fitness_score;
    int    active;
} officer_t;

typedef struct {
    int    case_id;
    int    type;
    int    zone;
    int    severity;
    int    status;
    int    assigned_officer;
    int    evidence_count;
    int    witnesses;
    int    year;
    int    resolved;
    int    active;
} case_t;

typedef struct {
    int    patrol_id;
    int    officer_id;
    int    zone;
    int    route_type;
    int    duration_hours;
    int    incidents_found;
    int    arrests;
    int    year;
    int    active;
} patrol_t;

typedef struct {
    int    traffic_id;
    int    type;
    int    zone;
    int    severity;
    int    fine_amount;
    int    points;
    int    year;
    int    month;
    int    active;
} traffic_t;

typedef struct {
    int    n_stations;
    int    n_officers;
    int    n_cases;
    int    n_patrols;
    int    n_traffic;
    int    total_cases_filed;
    int    total_cases_resolved;
    int    total_patrols;
    int    total_arrests;
    int    total_traffic;
    int    total_fines;
    int    total_points;
} pd_state_t;

static station_t stations[MAX_STATIONS];
static officer_t officers[MAX_OFFICERS];
static case_t cases[MAX_CASES];
static patrol_t patrols[MAX_PATROLS];
static traffic_t traffic[MAX_TRAFFIC];
static pd_state_t pd;

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

int pd_init(void) {
    if (initialized) return -1;
    pd.n_stations = 0; pd.n_officers = 0; pd.n_cases = 0;
    pd.n_patrols = 0; pd.n_traffic = 0;
    pd.total_cases_filed = 0; pd.total_cases_resolved = 0;
    pd.total_patrols = 0; pd.total_arrests = 0;
    pd.total_traffic = 0; pd.total_fines = 0; pd.total_points = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_OFFICERS; i++) officers[i].active = 0;
    for (int i = 0; i < MAX_CASES; i++) cases[i].active = 0;
    for (int i = 0; i < MAX_PATROLS; i++) patrols[i].active = 0;
    for (int i = 0; i < MAX_TRAFFIC; i++) traffic[i].active = 0;
    initialized = 1;
    print_str("[PD] Police department initialized\n");
    return 0;
}

int pd_add_station(int zone, int capacity, int officer_count, int vehicles, int jurisdiction, int year) {
    if (pd.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[pd.n_stations];
    s->station_id = pd.n_stations;
    s->zone = zone;
    s->capacity = capacity;
    s->officers = officer_count;
    s->vehicles = vehicles;
    s->jurisdiction = jurisdiction;
    s->year_built = year;
    s->active = 1;
    pd.n_stations++;
    print_str("[PD] Station "); print_int(pd.n_stations - 1);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" off="); print_int(officer_count);
    print_str(" veh="); print_int(vehicles); print_str("\n");
    return pd.n_stations - 1;
}

int pd_add_officer(int station_id, int rank, int badge, int years, int specialty, int fitness) {
    if (pd.n_officers >= MAX_OFFICERS) return -1;
    officer_t* o = &officers[pd.n_officers];
    o->officer_id = pd.n_officers;
    o->station_id = station_id;
    o->rank = rank;
    o->badge_number = badge;
    o->years_service = years;
    o->specialty = specialty;
    o->fitness_score = fitness;
    o->active = 1;
    pd.n_officers++;
    print_str("[PD] Officer "); print_int(pd.n_officers - 1);
    print_str(" stn="); print_int(station_id);
    print_str(" rank="); print_int(rank);
    print_str(" badge="); print_int(badge);
    print_str(" yrs="); print_int(years); print_str("\n");
    return pd.n_officers - 1;
}

int pd_file_case(int type, int zone, int severity, int year) {
    if (pd.n_cases >= MAX_CASES) return -1;
    case_t* c = &cases[pd.n_cases];
    c->case_id = pd.n_cases;
    c->type = type;
    c->zone = zone;
    c->severity = severity;
    c->status = 1;
    c->assigned_officer = -1;
    c->evidence_count = 0;
    c->witnesses = 0;
    c->year = year;
    c->resolved = 0;
    c->active = 1;
    pd.total_cases_filed++;
    pd.n_cases++;
    print_str("[PD] Case "); print_int(pd.n_cases - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" sev="); print_int(severity);
    print_str(" "); print_int(year); print_str("\n");
    return pd.n_cases - 1;
}

int pd_assign_case(int case_id, int officer_id) {
    if (case_id >= pd.n_cases) return -1;
    cases[case_id].assigned_officer = officer_id;
    cases[case_id].status = 2;
    print_str("[PD] Assign C"); print_int(case_id);
    print_str(" -> O"); print_int(officer_id); print_str("\n");
    return 0;
}

int pd_add_evidence(int case_id, int evidence_count, int witnesses) {
    if (case_id >= pd.n_cases) return -1;
    cases[case_id].evidence_count += evidence_count;
    cases[case_id].witnesses += witnesses;
    print_str("[PD] Evidence C"); print_int(case_id);
    print_str(" ev="); print_int(cases[case_id].evidence_count);
    print_str(" wit="); print_int(cases[case_id].witnesses); print_str("\n");
    return 0;
}

int pd_resolve_case(int case_id) {
    if (case_id >= pd.n_cases) return -1;
    cases[case_id].status = 3;
    cases[case_id].resolved = 1;
    pd.total_cases_resolved++;
    print_str("[PD] Resolve C"); print_int(case_id); print_str("\n");
    return 0;
}

int pd_add_patrol(int officer_id, int zone, int route_type, int duration, int year) {
    if (pd.n_patrols >= MAX_PATROLS) return -1;
    patrol_t* p = &patrols[pd.n_patrols];
    p->patrol_id = pd.n_patrols;
    p->officer_id = officer_id;
    p->zone = zone;
    p->route_type = route_type;
    p->duration_hours = duration;
    p->incidents_found = 0;
    p->arrests = 0;
    p->year = year;
    p->active = 1;
    pd.total_patrols++;
    pd.n_patrols++;
    print_str("[PD] Patrol "); print_int(pd.n_patrols - 1);
    print_str(" off="); print_int(officer_id);
    print_str(" zone="); print_int(zone);
    print_str(" rt="); print_int(route_type);
    print_str(" hrs="); print_int(duration); print_str("\n");
    return pd.n_patrols - 1;
}

int pd_patrol_result(int patrol_id, int incidents, int arrests) {
    if (patrol_id >= pd.n_patrols) return -1;
    patrols[patrol_id].incidents_found = incidents;
    patrols[patrol_id].arrests = arrests;
    pd.total_arrests += arrests;
    print_str("[PD] Result P"); print_int(patrol_id);
    print_str(" inc="); print_int(incidents);
    print_str(" arr="); print_int(arrests); print_str("\n");
    return 0;
}

int pd_add_traffic(int type, int zone, int severity, int fine, int points, int year, int month) {
    if (pd.n_traffic >= MAX_TRAFFIC) return -1;
    traffic_t* t = &traffic[pd.n_traffic];
    t->traffic_id = pd.n_traffic;
    t->type = type;
    t->zone = zone;
    t->severity = severity;
    t->fine_amount = fine;
    t->points = points;
    t->year = year;
    t->month = month;
    t->active = 1;
    pd.total_traffic++;
    pd.total_fines += fine;
    pd.total_points += points;
    pd.n_traffic++;
    print_str("[PD] Traffic "); print_int(pd.n_traffic - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" fine=$"); print_int(fine);
    print_str(" pts="); print_int(points); print_str("\n");
    return pd.n_traffic - 1;
}

void pd_case_report(void) {
    print_str("[PD] Case report:\n");
    print_str("  Filed: "); print_int(pd.total_cases_filed); print_str("\n");
    print_str("  Resolved: "); print_int(pd.total_cases_resolved); print_str("\n");
    print_str("  Open: "); print_int(pd.total_cases_filed - pd.total_cases_resolved); print_str("\n");
}

void pd_patrol_report(void) {
    print_str("[PD] Patrol report:\n");
    print_str("  Total patrols: "); print_int(pd.total_patrols); print_str("\n");
    print_str("  Total arrests: "); print_int(pd.total_arrests); print_str("\n");
}

void pd_traffic_report(void) {
    print_str("[PD] Traffic report:\n");
    print_str("  Violations: "); print_int(pd.total_traffic); print_str("\n");
    print_str("  Total fines: $"); print_int(pd.total_fines); print_str("\n");
    print_str("  Total points: "); print_int(pd.total_points); print_str("\n");
}

void pd_print_state(void) {
    print_str("[PD] Stn="); print_int(pd.n_stations);
    print_str(" Off="); print_int(pd.n_officers);
    print_str(" Cas="); print_int(pd.n_cases);
    print_str(" Pat="); print_int(pd.n_patrols);
    print_str(" Trf="); print_int(pd.n_traffic);
    print_str("\n");
}

int main(void) {
    print_str("=== Police Department Demo ===\n\n");
    pd_init();

    print_str("Adding stations...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int cap = 30 + (i * 5);
        int off = 20 + (i * 3);
        int veh = 5 + (i % 8);
        int jur = 10 + (i * 5);
        int year = 1995 + (i * 2);
        pd_add_station(zone, cap, off, veh, jur, year);
    }

    print_str("\nAdding officers...\n");
    for (int i = 0; i < 18; i++) {
        int stn = i % 12;
        int rank = (i % 5) + 1;
        int badge = 1000 + (i * 100);
        int yrs = 1 + (i % 25);
        int spec = (i % 4) + 1;
        int fit = 75 + (i % 25);
        pd_add_officer(stn, rank, badge, yrs, spec, fit);
    }

    print_str("\nFiling cases...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 5) + 1;
        int zone = (i % 4) + 1;
        int sev = (i % 4) + 1;
        int year = 2022 + (i % 3);
        pd_file_case(type, zone, sev, year);
    }

    print_str("\nAssigning cases...\n");
    for (int i = 0; i < 20; i++) {
        int off = i % 18;
        pd_assign_case(i, off);
    }

    print_str("\nAdding evidence...\n");
    for (int i = 0; i < 20; i++) {
        int ev = 1 + (i % 5);
        int wit = i % 4;
        pd_add_evidence(i, ev, wit);
    }

    print_str("\nResolving cases...\n");
    for (int i = 0; i < 16; i++) {
        pd_resolve_case(i);
    }

    print_str("\nDispatching patrols...\n");
    for (int i = 0; i < 14; i++) {
        int off = i % 18;
        int zone = (i % 4) + 1;
        int rt = (i % 3) + 1;
        int dur = 4 + (i % 8);
        int year = 2024;
        pd_add_patrol(off, zone, rt, dur, year);
    }

    print_str("\nPatrol results...\n");
    for (int i = 0; i < 14; i++) {
        int inc = i % 5;
        int arr = i % 3;
        pd_patrol_result(i, inc, arr);
    }

    print_str("\nTraffic violations...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int zone = (i % 4) + 1;
        int sev = (i % 3) + 1;
        int fine = 50 + (i * 30);
        int pts = 1 + (i % 6);
        int year = 2024;
        int month = 1 + (i % 12);
        pd_add_traffic(type, zone, sev, fine, pts, year, month);
    }

    print_str("\nCase report...\n");
    pd_case_report();

    print_str("\nPatrol report...\n");
    pd_patrol_report();

    print_str("\nTraffic report...\n");
    pd_traffic_report();

    print_str("\nFinal state...\n");
    pd_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
