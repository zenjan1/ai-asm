/* environmental_protection: Environmental management (v1.0)
 * Pollution monitoring, emissions, waste, assessments, compliance
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

#define MAX_STATIONS   14
#define MAX_EMISSIONS  16
#define MAX_WASTE      12
#define MAX_ASSESS     10
#define MAX_VIOLATIONS 14

typedef struct {
    int    station_id;
    int    type;
    int    zone;
    int    reading;
    int    threshold;
    int    year;
    int    month;
    int    day;
    int    status;
    int    active;
} station_t;

typedef struct {
    int    emis_id;
    int    company_id;
    int    pollutant;
    int    amount;
    int    limit;
    int    permit_id;
    int    year;
    int    quarter;
    int    active;
} emission_t;

typedef struct {
    int    waste_id;
    int    type;
    int    source_id;
    int    amount_kg;
    int    facility_id;
    int    method;
    int    recycled;
    int    year;
    int    active;
} waste_t;

typedef struct {
    int    assess_id;
    int    project_id;
    int    type;
    int    impact_score;
    int    mitigation;
    int    approved;
    int    year;
    int    active;
} assessment_t;

typedef struct {
    int    viol_id;
    int    company_id;
    int    type;
    int    severity;
    int    fine;
    int    rectified;
    int    year;
    int    resolved;
    int    active;
} violation_t;

typedef struct {
    int    n_stations;
    int    n_emissions;
    int    n_waste;
    int    n_assessments;
    int    n_violations;
    int    total_readings;
    int    total_exceedances;
    int    total_emissions;
    int    total_waste;
    int    total_recycled;
    int    total_fines;
    int    total_resolved;
} ep_state_t;

static station_t stations[MAX_STATIONS];
static emission_t emissions[MAX_EMISSIONS];
static waste_t wastes[MAX_WASTE];
static assessment_t assessments[MAX_ASSESS];
static violation_t violations[MAX_VIOLATIONS];
static ep_state_t ep;

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

int ep_init(void) {
    if (initialized) return -1;
    ep.n_stations = 0; ep.n_emissions = 0; ep.n_waste = 0;
    ep.n_assessments = 0; ep.n_violations = 0;
    ep.total_readings = 0; ep.total_exceedances = 0;
    ep.total_emissions = 0; ep.total_waste = 0;
    ep.total_recycled = 0; ep.total_fines = 0; ep.total_resolved = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_EMISSIONS; i++) emissions[i].active = 0;
    for (int i = 0; i < MAX_WASTE; i++) wastes[i].active = 0;
    for (int i = 0; i < MAX_ASSESS; i++) assessments[i].active = 0;
    for (int i = 0; i < MAX_VIOLATIONS; i++) violations[i].active = 0;
    initialized = 1;
    print_str("[EP] Environmental protection initialized\n");
    return 0;
}

int ep_add_station(int type, int zone, int threshold) {
    if (ep.n_stations >= MAX_STATIONS) return -1;
    station_t* s = &stations[ep.n_stations];
    s->station_id = ep.n_stations;
    s->type = type;
    s->zone = zone;
    s->reading = 0;
    s->threshold = threshold;
    s->year = 2024;
    s->month = 1;
    s->day = 1;
    s->status = 1;
    s->active = 1;
    ep.n_stations++;
    print_str("[EP] Station "); print_int(ep.n_stations - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" thresh="); print_int(threshold); print_str("\n");
    return ep.n_stations - 1;
}

int ep_record_reading(int station_id, int reading, int year, int month, int day) {
    if (station_id >= ep.n_stations) return -1;
    stations[station_id].reading = reading;
    stations[station_id].year = year;
    stations[station_id].month = month;
    stations[station_id].day = day;
    ep.total_readings++;
    if (reading > stations[station_id].threshold) {
        stations[station_id].status = 2;
        ep.total_exceedances++;
        print_str("[EP] Exceed St"); print_int(station_id);
        print_str(" read="); print_int(reading);
        print_str(" thresh="); print_int(stations[station_id].threshold); print_str("!\n");
    } else {
        print_str("[EP] Reading St"); print_int(station_id);
        print_str(" ="); print_int(reading); print_str("\n");
    }
    return 0;
}

int ep_record_emission(int company, int pollutant, int amount, int limit, int permit, int year, int quarter) {
    if (ep.n_emissions >= MAX_EMISSIONS) return -1;
    emission_t* e = &emissions[ep.n_emissions];
    e->emis_id = ep.n_emissions;
    e->company_id = company;
    e->pollutant = pollutant;
    e->amount = amount;
    e->limit = limit;
    e->permit_id = permit;
    e->year = year;
    e->quarter = quarter;
    e->active = 1;
    ep.total_emissions += amount;
    ep.n_emissions++;
    print_str("[EP] Emis "); print_int(ep.n_emissions - 1);
    print_str(" Co"); print_int(company);
    print_str(" pol="); print_int(pollutant);
    print_str(" amt="); print_int(amount);
    print_str(" lim="); print_int(limit); print_str("\n");
    return ep.n_emissions - 1;
}

int ep_record_waste(int type, int source, int amount, int facility, int method, int year) {
    if (ep.n_waste >= MAX_WASTE) return -1;
    waste_t* w = &wastes[ep.n_waste];
    w->waste_id = ep.n_waste;
    w->type = type;
    w->source_id = source;
    w->amount_kg = amount;
    w->facility_id = facility;
    w->method = method;
    w->recycled = (method == 2) ? amount : 0;
    w->year = year;
    w->active = 1;
    ep.total_waste += amount;
    ep.total_recycled += w->recycled;
    ep.n_waste++;
    print_str("[EP] Waste "); print_int(ep.n_waste - 1);
    print_str(" type="); print_int(type);
    print_str(" src="); print_int(source);
    print_str(" kg="); print_int(amount);
    print_str(" method="); print_int(method); print_str("\n");
    return ep.n_waste - 1;
}

int ep_assess_project(int project, int type, int impact, int mitigation, int year) {
    if (ep.n_assessments >= MAX_ASSESS) return -1;
    assessment_t* a = &assessments[ep.n_assessments];
    a->assess_id = ep.n_assessments;
    a->project_id = project;
    a->type = type;
    a->impact_score = impact;
    a->mitigation = mitigation;
    a->approved = 0;
    a->year = year;
    a->active = 1;
    ep.n_assessments++;
    print_str("[EP] Assess "); print_int(ep.n_assessments - 1);
    print_str(" proj="); print_int(project);
    print_str(" type="); print_int(type);
    print_str(" impact="); print_int(impact);
    print_str(" mitig="); print_int(mitigation); print_str("\n");
    return ep.n_assessments - 1;
}

int ep_approve_assessment(int assess_id) {
    if (assess_id >= ep.n_assessments) return -1;
    assessments[assess_id].approved = 1;
    print_str("[EP] Approve A"); print_int(assess_id); print_str("\n");
    return 0;
}

int ep_record_violation(int company, int type, int severity, int fine, int year) {
    if (ep.n_violations >= MAX_VIOLATIONS) return -1;
    violation_t* v = &violations[ep.n_violations];
    v->viol_id = ep.n_violations;
    v->company_id = company;
    v->type = type;
    v->severity = severity;
    v->fine = fine;
    v->rectified = 0;
    v->year = year;
    v->resolved = 0;
    v->active = 1;
    ep.total_fines += fine;
    ep.n_violations++;
    print_str("[EP] Viol "); print_int(ep.n_violations - 1);
    print_str(" Co"); print_int(company);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" $"); print_int(fine); print_str("\n");
    return ep.n_violations - 1;
}

int ep_resolve_violation(int viol_id) {
    if (viol_id >= ep.n_violations) return -1;
    violations[viol_id].resolved = 1;
    violations[viol_id].rectified = 1;
    ep.total_resolved++;
    print_str("[EP] Resolve Vl"); print_int(viol_id); print_str("\n");
    return 0;
}

void ep_monitoring_report(void) {
    print_str("[EP] Monitoring report:\n");
    print_str("  Stations: "); print_int(ep.n_stations); print_str("\n");
    print_str("  Readings: "); print_int(ep.total_readings); print_str("\n");
    print_str("  Exceedances: "); print_int(ep.total_exceedances); print_str("\n");
    for (int i = 0; i < ep.n_stations; i++) {
        station_t* s = &stations[i];
        print_str("  St"); print_int(i);
        print_str(" type="); print_int(s->type);
        print_str(" zone="); print_int(s->zone);
        print_str(" read="); print_int(s->reading);
        print_str(" st="); print_int(s->status); print_str("\n");
    }
}

void ep_emission_report(void) {
    print_str("[EP] Emissions: "); print_int(ep.total_emissions); print_str("\n");
    print_str("  Records: "); print_int(ep.n_emissions); print_str("\n");
}

void ep_waste_report(void) {
    print_str("[EP] Waste report:\n");
    print_str("  Total waste: "); print_int(ep.total_waste); print_str(" kg\n");
    print_str("  Recycled: "); print_int(ep.total_recycled); print_str(" kg\n");
}

void ep_compliance_report(void) {
    print_str("[EP] Compliance report:\n");
    print_str("  Violations: "); print_int(ep.n_violations); print_str("\n");
    print_str("  Fines: "); print_int(ep.total_fines); print_str("\n");
    print_str("  Resolved: "); print_int(ep.total_resolved); print_str("\n");
}

void ep_print_state(void) {
    print_str("[EP] Stn="); print_int(ep.n_stations);
    print_str(" Emis="); print_int(ep.n_emissions);
    print_str(" Waste="); print_int(ep.n_waste);
    print_str(" Assess="); print_int(ep.n_assessments);
    print_str(" Viol="); print_int(ep.n_violations);
    print_str("\n");
}

int main(void) {
    print_str("=== Environmental Protection Demo ===\n\n");
    ep_init();

    print_str("Adding monitoring stations...\n");
    ep_add_station(1, 1, 100);
    ep_add_station(1, 2, 100);
    ep_add_station(1, 3, 100);
    ep_add_station(2, 1, 50);
    ep_add_station(2, 2, 50);
    ep_add_station(2, 4, 50);
    ep_add_station(3, 1, 70);
    ep_add_station(3, 3, 70);
    ep_add_station(4, 2, 60);
    ep_add_station(4, 4, 60);
    ep_add_station(1, 4, 100);
    ep_add_station(2, 3, 50);
    ep_add_station(3, 4, 70);
    ep_add_station(4, 1, 60);

    print_str("\nRecording readings...\n");
    for (int i = 0; i < 28; i++) {
        int st = i % 14;
        int reading = 50 + (i * 10);
        int year = 2024;
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        ep_record_reading(st, reading, year, month, day);
    }

    print_str("\nRecording emissions...\n");
    for (int i = 0; i < 16; i++) {
        int company = 100 + (i % 10);
        int pollutant = (i % 4) + 1;
        int amount = 500 + (i * 200);
        int limit = 1000 + (i * 100);
        int permit = i % 10;
        int year = 2024;
        int quarter = (i % 4) + 1;
        ep_record_emission(company, pollutant, amount, limit, permit, year, quarter);
    }

    print_str("\nRecording waste...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        int source = 200 + (i % 8);
        int amount = 1000 + (i * 500);
        int facility = (i % 4) + 1;
        int method = (i % 3) + 1;
        int year = 2024;
        ep_record_waste(type, source, amount, facility, method, year);
    }

    print_str("\nProject assessments...\n");
    for (int i = 0; i < 10; i++) {
        int project = 300 + i;
        int type = (i % 3) + 1;
        int impact = 30 + (i * 7);
        int mitigation = 20 + (i * 5);
        int year = 2024;
        ep_assess_project(project, type, impact, mitigation, year);
    }

    print_str("\nApproving assessments...\n");
    for (int i = 0; i < 10; i++) {
        ep_approve_assessment(i);
    }

    print_str("\nRecording violations...\n");
    for (int i = 0; i < 14; i++) {
        int company = 100 + (i % 10);
        int type = (i % 4) + 1;
        int severity = (i % 5) + 1;
        int fine = 5000 + (i * 2000);
        int year = 2024;
        ep_record_violation(company, type, severity, fine, year);
    }

    print_str("\nResolving violations...\n");
    for (int i = 0; i < 14; i++) {
        ep_resolve_violation(i);
    }

    print_str("\nMonitoring report...\n");
    ep_monitoring_report();

    print_str("\nEmission report...\n");
    ep_emission_report();

    print_str("\nWaste report...\n");
    ep_waste_report();

    print_str("\nCompliance report...\n");
    ep_compliance_report();

    print_str("\nFinal state...\n");
    ep_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
