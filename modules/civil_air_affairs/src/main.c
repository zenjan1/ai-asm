/* civil_air_affairs: Civil aviation administration (v1.0)
 * Air transport, airworthiness, airports, safety, general aviation
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

#define MAX_TRANSPORT    16
#define MAX_AIRWORTH     14
#define MAX_AIRPORT      12
#define MAX_SAFETY       10
#define MAX_GENERAL      10

typedef struct {
    int    transport_id;
    int    airline_id;
    int    route_id;
    int    flights;
    int    passengers;
    int    cargo_tons;
    int    revenue;
    int    year;
    int    active;
} transport_t;

typedef struct {
    int    airworth_id;
    int    aircraft_id;
    int    type;
    int    inspection_type;
    int    hours_flown;
    int    maintenance;
    int    certified;
    int    year;
    int    active;
} airworth_t;

typedef struct {
    int    airport_id;
    int    location_id;
    int    tier;
    int    runways;
    int    passengers;
    int    cargo_tons;
    int    revenue;
    int    year;
    int    active;
} airport_t;

typedef struct {
    int    safety_id;
    int    incident_type;
    int    aircraft_id;
    int    severity;
    int    injuries;
    int    investigation;
    int    year;
    int    status;
    int    active;
} safety_t;

typedef struct {
    int    general_id;
    int    operator_id;
    int    aircraft_type;
    int    flight_hours;
    int    missions;
    int    revenue;
    int    year;
    int    active;
} general_t;

typedef struct {
    int    n_transport;
    int    n_airworth;
    int    n_airport;
    int    n_safety;
    int    n_general;
    int    total_passengers;
    int    total_cargo;
    int    total_revenue;
    int    total_hours;
    int    total_incidents;
} ca_state_t;

static transport_t transports[MAX_TRANSPORT];
static airworth_t airworthiness[MAX_AIRWORTH];
static airport_t airports[MAX_AIRPORT];
static safety_t safeties[MAX_SAFETY];
static general_t generals[MAX_GENERAL];
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
    ca.n_transport = 0; ca.n_airworth = 0; ca.n_airport = 0;
    ca.n_safety = 0; ca.n_general = 0;
    ca.total_passengers = 0; ca.total_cargo = 0;
    ca.total_revenue = 0; ca.total_hours = 0;
    ca.total_incidents = 0;
    for (int i = 0; i < MAX_TRANSPORT; i++) transports[i].active = 0;
    for (int i = 0; i < MAX_AIRWORTH; i++) airworthiness[i].active = 0;
    for (int i = 0; i < MAX_AIRPORT; i++) airports[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    for (int i = 0; i < MAX_GENERAL; i++) generals[i].active = 0;
    initialized = 1;
    print_str("[CA] Civil air affairs initialized\n");
    return 0;
}

int ca_transport(int airline, int route, int flights, int passengers, int cargo, int revenue, int year) {
    if (ca.n_transport >= MAX_TRANSPORT) return -1;
    transport_t* t = &transports[ca.n_transport];
    t->transport_id = ca.n_transport;
    t->airline_id = airline;
    t->route_id = route;
    t->flights = flights;
    t->passengers = passengers;
    t->cargo_tons = cargo;
    t->revenue = revenue;
    t->year = year;
    t->active = 1;
    ca.total_passengers += passengers;
    ca.total_cargo += cargo;
    ca.total_revenue += revenue;
    ca.n_transport++;
    print_str("[CA] Transport "); print_int(ca.n_transport - 1);
    print_str(" air="); print_int(airline);
    print_str(" rte="); print_int(route);
    print_str(" flt="); print_int(flights);
    print_str(" pax="); print_int(passengers);
    print_str(" cgo="); print_int(cargo); print_str("t\n");
    return ca.n_transport - 1;
}

int ca_airworth(int aircraft, int type, int inspection, int hours, int maint, int certified, int year) {
    if (ca.n_airworth >= MAX_AIRWORTH) return -1;
    airworth_t* a = &airworthiness[ca.n_airworth];
    a->airworth_id = ca.n_airworth;
    a->aircraft_id = aircraft;
    a->type = type;
    a->inspection_type = inspection;
    a->hours_flown = hours;
    a->maintenance = maint;
    a->certified = certified;
    a->year = year;
    a->active = 1;
    ca.n_airworth++;
    print_str("[CA] Airworth "); print_int(ca.n_airworth - 1);
    print_str(" ac="); print_int(aircraft);
    print_str(" type="); print_int(type);
    print_str(" ins="); print_int(inspection);
    print_str(" hrs="); print_int(hours);
    print_str(" mnt="); print_int(maint);
    print_str(" crt="); print_int(certified); print_str("\n");
    return ca.n_airworth - 1;
}

int ca_airport(int location, int tier, int runways, int passengers, int cargo, int revenue, int year) {
    if (ca.n_airport >= MAX_AIRPORT) return -1;
    airport_t* ap = &airports[ca.n_airport];
    ap->airport_id = ca.n_airport;
    ap->location_id = location;
    ap->tier = tier;
    ap->runways = runways;
    ap->passengers = passengers;
    ap->cargo_tons = cargo;
    ap->revenue = revenue;
    ap->year = year;
    ap->active = 1;
    ca.n_airport++;
    print_str("[CA] Airport "); print_int(ca.n_airport - 1);
    print_str(" loc="); print_int(location);
    print_str(" tier="); print_int(tier);
    print_str(" rwy="); print_int(runways);
    print_str(" pax="); print_int(passengers);
    print_str(" cgo="); print_int(cargo); print_str("t\n");
    return ca.n_airport - 1;
}

int ca_safety(int incident, int aircraft, int severity, int injuries, int investigation, int year) {
    if (ca.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[ca.n_safety];
    s->safety_id = ca.n_safety;
    s->incident_type = incident;
    s->aircraft_id = aircraft;
    s->severity = severity;
    s->injuries = injuries;
    s->investigation = investigation;
    s->year = year;
    s->status = 1;
    s->active = 1;
    ca.total_incidents++;
    ca.n_safety++;
    print_str("[CA] Safety "); print_int(ca.n_safety - 1);
    print_str(" inc="); print_int(incident);
    print_str(" ac="); print_int(aircraft);
    print_str(" sev="); print_int(severity);
    print_str(" inj="); print_int(injuries);
    print_str(" inv="); print_int(investigation); print_str("\n");
    return ca.n_safety - 1;
}

int ca_general(int operator_id, int aircraft_type, int hours, int missions, int revenue, int year) {
    if (ca.n_general >= MAX_GENERAL) return -1;
    general_t* g = &generals[ca.n_general];
    g->general_id = ca.n_general;
    g->operator_id = operator_id;
    g->aircraft_type = aircraft_type;
    g->flight_hours = hours;
    g->missions = missions;
    g->revenue = revenue;
    g->year = year;
    g->active = 1;
    ca.total_hours += hours;
    ca.n_general++;
    print_str("[CA] General "); print_int(ca.n_general - 1);
    print_str(" opr="); print_int(operator_id);
    print_str(" type="); print_int(aircraft_type);
    print_str(" hrs="); print_int(hours);
    print_str(" mis="); print_int(missions);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return ca.n_general - 1;
}

void ca_transport_report(void) {
    print_str("[CA] Transport report:\n");
    print_str("  Routes: "); print_int(ca.n_transport); print_str("\n");
    print_str("  Total passengers: "); print_int(ca.total_passengers); print_str("\n");
    print_str("  Total cargo: "); print_int(ca.total_cargo); print_str(" tons\n");
    print_str("  Total revenue: $"); print_int(ca.total_revenue); print_str("\n");
}

void ca_airworth_report(void) {
    print_str("[CA] Airworthiness report:\n");
    print_str("  Aircraft records: "); print_int(ca.n_airworth); print_str("\n");
    print_str("  Airports: "); print_int(ca.n_airport); print_str("\n");
}

void ca_safety_report(void) {
    print_str("[CA] Safety report:\n");
    print_str("  Safety incidents: "); print_int(ca.total_incidents); print_str("\n");
    print_str("  General aviation: "); print_int(ca.n_general); print_str("\n");
    print_str("  Total flight hours: "); print_int(ca.total_hours); print_str("\n");
}

void ca_print_state(void) {
    print_str("[CA] Tr="); print_int(ca.n_transport);
    print_str(" Aw="); print_int(ca.n_airworth);
    print_str(" Ap="); print_int(ca.n_airport);
    print_str(" Sf="); print_int(ca.n_safety);
    print_str(" Gn="); print_int(ca.n_general);
    print_str("\n");
}

int main(void) {
    print_str("=== Civil Air Affairs Demo ===\n\n");
    ca_init();

    print_str("Air transport...\n");
    for (int i = 0; i < 16; i++) {
        int air = 100 + (i * 5);
        int rte = 200 + (i * 7);
        int flt = 50 + (i * 20);
        int pax = 5000 + (i * 2000);
        int cgo = 50 + (i * 20);
        int rev = 500000 + (i * 200000);
        int year = 2020 + (i % 5);
        ca_transport(air, rte, flt, pax, cgo, rev, year);
    }

    print_str("\nAirworthiness...\n");
    for (int i = 0; i < 14; i++) {
        int ac = 1000 + (i * 11);
        int type = (i % 4) + 1;
        int ins = (i % 3) + 1;
        int hrs = 500 + (i * 200);
        int mnt = (i % 4 == 0) ? 1 : 0;
        int crt = (i % 3 == 0) ? 0 : 1;
        int year = 2021 + (i % 4);
        ca_airworth(ac, type, ins, hrs, mnt, crt, year);
    }

    print_str("\nAirport management...\n");
    for (int i = 0; i < 12; i++) {
        int loc = (i % 8) + 1;
        int tier = (i % 4) + 1;
        int rwy = 1 + (i % 3);
        int pax = 100000 + (i * 50000);
        int cgo = 1000 + (i * 500);
        int rev = 1000000 + (i * 500000);
        int year = 2022 + (i % 3);
        ca_airport(loc, tier, rwy, pax, cgo, rev, year);
    }

    print_str("\nFlight safety...\n");
    for (int i = 0; i < 10; i++) {
        int inc = (i % 4) + 1;
        int ac = 2000 + (i * 13);
        int sev = (i % 3) + 1;
        int inj = i % 5;
        int inv = (i % 3 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        ca_safety(inc, ac, sev, inj, inv, year);
    }

    print_str("\nGeneral aviation...\n");
    for (int i = 0; i < 10; i++) {
        int opr = 3000 + (i * 17);
        int type = (i % 5) + 1;
        int hrs = 100 + (i * 50);
        int mis = 10 + (i * 5);
        int rev = 50000 + (i * 20000);
        int year = 2024;
        ca_general(opr, type, hrs, mis, rev, year);
    }

    print_str("\nTransport report...\n");
    ca_transport_report();

    print_str("\nAirworthiness report...\n");
    ca_airworth_report();

    print_str("\nSafety report...\n");
    ca_safety_report();

    print_str("\nFinal state...\n");
    ca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
