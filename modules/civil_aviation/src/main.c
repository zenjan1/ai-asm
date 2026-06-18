/* civil_aviation: Civil aviation administration (v1.0)
 * Regulation, airworthiness, personnel, safety, statistics
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

#define MAX_REGULATIONS 12
#define MAX_AIRCRAFT    14
#define MAX_LICENSES    16
#define MAX_INCIDENTS   10
#define MAX_STATS       10

typedef struct {
    int    reg_id;
    int    type;
    int    category;
    int    zone;
    int    effective_year;
    int    compliance_rate;
    int    inspections;
    int    active;
} regulation_t;

typedef struct {
    int    aircraft_id;
    int    type;
    int    operator_id;
    int    registration;
    int    year_built;
    int    airworthiness;
    int    flight_hours;
    int    active;
} aircraft_t;

typedef struct {
    int    license_id;
    int    type;
    int    holder;
    int    zone;
    int    rating;
    int    hours_flown;
    int    valid_until;
    int    active;
} license_t;

typedef struct {
    int    incident_id;
    int    type;
    int    severity;
    int    aircraft_id;
    int    zone;
    int    injuries;
    int    year;
    int    active;
} incident_t;

typedef struct {
    int    stat_id;
    int    zone;
    int    flights;
    int    passengers;
    int    cargo_tons;
    int    revenue;
    int    year;
    int    active;
} statistic_t;

typedef struct {
    int    n_regulations;
    int    n_aircraft;
    int    n_licenses;
    int    n_incidents;
    int    n_stats;
    int    total_flights;
    int    total_passengers;
    int    total_cargo;
    int    total_revenue;
    int    total_injuries;
} ca_state_t;

static regulation_t regulations[MAX_REGULATIONS];
static aircraft_t aircraft[MAX_AIRCRAFT];
static license_t licenses[MAX_LICENSES];
static incident_t incidents[MAX_INCIDENTS];
static statistic_t statistics[MAX_STATS];
static ca_state_t ca;

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

int ca_init(void) {
    if (initialized) return -1;
    ca.n_regulations = 0; ca.n_aircraft = 0; ca.n_licenses = 0;
    ca.n_incidents = 0; ca.n_stats = 0;
    ca.total_flights = 0; ca.total_passengers = 0;
    ca.total_cargo = 0; ca.total_revenue = 0;
    ca.total_injuries = 0;
    for (int i = 0; i < MAX_REGULATIONS; i++) regulations[i].active = 0;
    for (int i = 0; i < MAX_AIRCRAFT; i++) aircraft[i].active = 0;
    for (int i = 0; i < MAX_LICENSES; i++) licenses[i].active = 0;
    for (int i = 0; i < MAX_INCIDENTS; i++) incidents[i].active = 0;
    for (int i = 0; i < MAX_STATS; i++) statistics[i].active = 0;
    initialized = 1;
    print_str("[CA] Civil aviation initialized\n");
    return 0;
}

int ca_add_regulation(int type, int category, int zone, int year, int compliance) {
    if (ca.n_regulations >= MAX_REGULATIONS) return -1;
    regulation_t* r = &regulations[ca.n_regulations];
    r->reg_id = ca.n_regulations;
    r->type = type;
    r->category = category;
    r->zone = zone;
    r->effective_year = year;
    r->compliance_rate = compliance;
    r->inspections = 0;
    r->active = 1;
    ca.n_regulations++;
    print_str("[CA] Reg "); print_int(ca.n_regulations - 1);
    print_str(" type="); print_int(type);
    print_str(" cat="); print_int(category);
    print_str(" zone="); print_int(zone);
    print_str(" "); print_int(year);
    print_str(" comp="); print_int(compliance); print_str("%\n");
    return ca.n_regulations - 1;
}

int ca_inspect(int reg_id) {
    if (reg_id >= ca.n_regulations) return -1;
    regulations[reg_id].inspections++;
    print_str("[CA] Inspect R"); print_int(reg_id); print_str("\n");
    return 0;
}

int ca_register_aircraft(int type, int operator_id, int registration, int year, int hours) {
    if (ca.n_aircraft >= MAX_AIRCRAFT) return -1;
    aircraft_t* a = &aircraft[ca.n_aircraft];
    a->aircraft_id = ca.n_aircraft;
    a->type = type;
    a->operator_id = operator_id;
    a->registration = registration;
    a->year_built = year;
    a->airworthiness = 100;
    a->flight_hours = hours;
    a->active = 1;
    ca.n_aircraft++;
    print_str("[CA] Aircraft "); print_int(ca.n_aircraft - 1);
    print_str(" type="); print_int(type);
    print_str(" op="); print_int(operator_id);
    print_str(" reg="); print_int(registration);
    print_str(" "); print_int(year);
    print_str(" hrs="); print_int(hours); print_str("\n");
    return ca.n_aircraft - 1;
}

int ca_issue_license(int type, int holder, int zone, int rating, int hours, int valid_until) {
    if (ca.n_licenses >= MAX_LICENSES) return -1;
    license_t* l = &licenses[ca.n_licenses];
    l->license_id = ca.n_licenses;
    l->type = type;
    l->holder = holder;
    l->zone = zone;
    l->rating = rating;
    l->hours_flown = hours;
    l->valid_until = valid_until;
    l->active = 1;
    ca.n_licenses++;
    print_str("[CA] License "); print_int(ca.n_licenses - 1);
    print_str(" type="); print_int(type);
    print_str(" holder="); print_int(holder);
    print_str(" zone="); print_int(zone);
    print_str(" hrs="); print_int(hours);
    print_str(" valid="); print_int(valid_until); print_str("\n");
    return ca.n_licenses - 1;
}

int ca_record_incident(int type, int severity, int aircraft_id, int zone, int injuries, int year) {
    if (ca.n_incidents >= MAX_INCIDENTS) return -1;
    incident_t* in = &incidents[ca.n_incidents];
    in->incident_id = ca.n_incidents;
    in->type = type;
    in->severity = severity;
    in->aircraft_id = aircraft_id;
    in->zone = zone;
    in->injuries = injuries;
    in->year = year;
    in->active = 1;
    ca.total_injuries += injuries;
    ca.n_incidents++;
    print_str("[CA] Incident "); print_int(ca.n_incidents - 1);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" ac="); print_int(aircraft_id);
    print_str(" inj="); print_int(injuries); print_str("\n");
    return ca.n_incidents - 1;
}

int ca_record_stat(int zone, int flights, int passengers, int cargo, int revenue, int year) {
    if (ca.n_stats >= MAX_STATS) return -1;
    statistic_t* s = &statistics[ca.n_stats];
    s->stat_id = ca.n_stats;
    s->zone = zone;
    s->flights = flights;
    s->passengers = passengers;
    s->cargo_tons = cargo;
    s->revenue = revenue;
    s->year = year;
    s->active = 1;
    ca.total_flights += flights;
    ca.total_passengers += passengers;
    ca.total_cargo += cargo;
    ca.total_revenue += revenue;
    ca.n_stats++;
    print_str("[CA] Stat "); print_int(ca.n_stats - 1);
    print_str(" zone="); print_int(zone);
    print_str(" fl="); print_int(flights);
    print_str(" pax="); print_int(passengers);
    print_str(" cargo="); print_int(cargo); print_str("t\n");
    return ca.n_stats - 1;
}

void ca_regulation_report(void) {
    print_str("[CA] Regulation report:\n");
    print_str("  Regulations: "); print_int(ca.n_regulations); print_str("\n");
}

void ca_fleet_report(void) {
    print_str("[CA] Fleet report:\n");
    print_str("  Aircraft: "); print_int(ca.n_aircraft); print_str("\n");
    print_str("  Licenses: "); print_int(ca.n_licenses); print_str("\n");
}

void ca_safety_report(void) {
    print_str("[CA] Safety report:\n");
    print_str("  Incidents: "); print_int(ca.n_incidents); print_str("\n");
    print_str("  Injuries: "); print_int(ca.total_injuries); print_str("\n");
}

void ca_traffic_report(void) {
    print_str("[CA] Traffic report:\n");
    print_str("  Flights: "); print_int(ca.total_flights); print_str("\n");
    print_str("  Passengers: "); print_int(ca.total_passengers); print_str("\n");
    print_str("  Cargo: "); print_int(ca.total_cargo); print_str(" tons\n");
    print_str("  Revenue: $"); print_int(ca.total_revenue); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Rg="); print_int(ca.n_regulations);
    print_str(" Ac="); print_int(ca.n_aircraft);
    print_str(" Lc="); print_int(ca.n_licenses);
    print_str(" In="); print_int(ca.n_incidents);
    print_str(" St="); print_int(ca.n_stats);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Aviation Demo ===\n\n");
    ca_init();

    print_str("Adding regulations...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int year = 2015 + (i % 10);
        int comp = 85 + (i % 15);
        ca_add_regulation(type, cat, zone, year, comp);
    }

    print_str("\nConducting inspections...\n");
    for (int i = 0; i < 20; i++) {
        ca_inspect(i % 12);
    }

    print_str("\nRegistering aircraft...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int op = (i % 5) + 1;
        int reg = 1000 + i;
        int year = 2010 + (i % 12);
        int hrs = 1000 + (i * 500);
        ca_register_aircraft(type, op, reg, year, hrs);
    }

    print_str("\nIssuing licenses...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 3) + 1;
        int holder = 200 + i;
        int zone = (i % 4) + 1;
        int rating = (i % 5) + 1;
        int hours = 500 + (i * 200);
        int valid = 2025 + (i % 5);
        ca_issue_license(type, holder, zone, rating, hours, valid);
    }

    print_str("\nRecording incidents...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int sev = (i % 4) + 1;
        int ac = i % 14;
        int zone = (i % 4) + 1;
        int inj = i % 3;
        int year = 2022 + (i % 3);
        ca_record_incident(type, sev, ac, zone, inj, year);
    }

    print_str("\nRecording statistics...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int flights = 5000 + (i * 1000);
        int pax = 500000 + (i * 100000);
        int cargo = 10000 + (i * 2000);
        int rev = 50000000 + (i * 10000000);
        int year = 2020 + i;
        ca_record_stat(zone, flights, pax, cargo, rev, year);
    }

    print_str("\nRegulation report...\n");
    ca_regulation_report();

    print_str("\nFleet report...\n");
    ca_fleet_report();

    print_str("\nSafety report...\n");
    ca_safety_report();

    print_str("\nTraffic report...\n");
    ca_traffic_report();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
