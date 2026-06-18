/* airport_authority: Airport management (v1.0)
 * Terminals, runways, services, commercial, emergency
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

#define MAX_TERMINALS  10
#define MAX_RUNWAYS    8
#define MAX_GATES      16
#define MAX_SERVICES   12
#define MAX_EMERGENCY  8

typedef struct {
    int    terminal_id;
    int    zone;
    int    capacity;
    int    counters;
    int    shops;
    int    passengers;
    int    year_built;
    int    active;
} terminal_t;

typedef struct {
    int    runway_id;
    int    zone;
    int    length_m;
    int    width_m;
    int    surface;
    int    status;
    int    operations;
    int    active;
} runway_t;

typedef struct {
    int    gate_id;
    int    terminal_id;
    int    type;
    int    capacity;
    int    flights_handled;
    int    status;
    int    active;
} gate_t;

typedef struct {
    int    service_id;
    int    type;
    int    zone;
    int    provider;
    int    revenue;
    int    customers;
    int    year;
    int    active;
} service_t;

typedef struct {
    int    emergency_id;
    int    type;
    int    zone;
    int    severity;
    int    response_time;
    int    personnel;
    int    resolved;
    int    year;
    int    active;
} emergency_t;

typedef struct {
    int    n_terminals;
    int    n_runways;
    int    n_gates;
    int    n_services;
    int    n_emergency;
    int    total_passengers;
    int    total_operations;
    int    total_revenue;
    int    total_emergencies;
    int    total_resolved;
} aa_state_t;

static terminal_t terminals[MAX_TERMINALS];
static runway_t runways[MAX_RUNWAYS];
static gate_t gates[MAX_GATES];
static service_t services[MAX_SERVICES];
static emergency_t emergencies[MAX_EMERGENCY];
static aa_state_t aa;

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

int aa_init(void) {
    if (initialized) return -1;
    aa.n_terminals = 0; aa.n_runways = 0; aa.n_gates = 0;
    aa.n_services = 0; aa.n_emergency = 0;
    aa.total_passengers = 0; aa.total_operations = 0;
    aa.total_revenue = 0; aa.total_emergencies = 0;
    aa.total_resolved = 0;
    for (int i = 0; i < MAX_TERMINALS; i++) terminals[i].active = 0;
    for (int i = 0; i < MAX_RUNWAYS; i++) runways[i].active = 0;
    for (int i = 0; i < MAX_GATES; i++) gates[i].active = 0;
    for (int i = 0; i < MAX_SERVICES; i++) services[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY; i++) emergencies[i].active = 0;
    initialized = 1;
    print_str("[AA] Airport authority initialized\n");
    return 0;
}

int aa_add_terminal(int zone, int capacity, int counters, int shops, int passengers, int year) {
    if (aa.n_terminals >= MAX_TERMINALS) return -1;
    terminal_t* t = &terminals[aa.n_terminals];
    t->terminal_id = aa.n_terminals;
    t->zone = zone;
    t->capacity = capacity;
    t->counters = counters;
    t->shops = shops;
    t->passengers = passengers;
    t->year_built = year;
    t->active = 1;
    aa.total_passengers += passengers;
    aa.n_terminals++;
    print_str("[AA] Terminal "); print_int(aa.n_terminals - 1);
    print_str(" zone="); print_int(zone);
    print_str(" cap="); print_int(capacity);
    print_str(" cnt="); print_int(counters);
    print_str(" shops="); print_int(shops);
    print_str(" pax="); print_int(passengers); print_str("\n");
    return aa.n_terminals - 1;
}

int aa_add_runway(int zone, int length, int width, int surface) {
    if (aa.n_runways >= MAX_RUNWAYS) return -1;
    runway_t* r = &runways[aa.n_runways];
    r->runway_id = aa.n_runways;
    r->zone = zone;
    r->length_m = length;
    r->width_m = width;
    r->surface = surface;
    r->status = 1;
    r->operations = 0;
    r->active = 1;
    aa.n_runways++;
    print_str("[AA] Runway "); print_int(aa.n_runways - 1);
    print_str(" zone="); print_int(zone);
    print_str(" len="); print_int(length); print_str("m");
    print_str(" w="); print_int(width); print_str("m");
    print_str(" srf="); print_int(surface); print_str("\n");
    return aa.n_runways - 1;
}

int aa_record_operations(int runway_id, int operations) {
    if (runway_id >= aa.n_runways) return -1;
    runways[runway_id].operations += operations;
    aa.total_operations += operations;
    print_str("[AA] Ops Rw"); print_int(runway_id);
    print_str(" +="); print_int(operations); print_str("\n");
    return 0;
}

int aa_add_gate(int terminal_id, int type, int capacity) {
    if (aa.n_gates >= MAX_GATES) return -1;
    gate_t* g = &gates[aa.n_gates];
    g->gate_id = aa.n_gates;
    g->terminal_id = terminal_id;
    g->type = type;
    g->capacity = capacity;
    g->flights_handled = 0;
    g->status = 1;
    g->active = 1;
    aa.n_gates++;
    print_str("[AA] Gate "); print_int(aa.n_gates - 1);
    print_str(" tm="); print_int(terminal_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return aa.n_gates - 1;
}

int aa_handle_flights(int gate_id, int flights) {
    if (gate_id >= aa.n_gates) return -1;
    gates[gate_id].flights_handled += flights;
    print_str("[AA] Flights G"); print_int(gate_id);
    print_str(" +="); print_int(flights); print_str("\n");
    return 0;
}

int aa_add_service(int type, int zone, int provider, int revenue, int customers, int year) {
    if (aa.n_services >= MAX_SERVICES) return -1;
    service_t* s = &services[aa.n_services];
    s->service_id = aa.n_services;
    s->type = type;
    s->zone = zone;
    s->provider = provider;
    s->revenue = revenue;
    s->customers = customers;
    s->year = year;
    s->active = 1;
    aa.total_revenue += revenue;
    aa.n_services++;
    print_str("[AA] Service "); print_int(aa.n_services - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" rev=$"); print_int(revenue);
    print_str(" cust="); print_int(customers); print_str("\n");
    return aa.n_services - 1;
}

int aa_trigger_emergency(int type, int zone, int severity, int response, int personnel, int year) {
    if (aa.n_emergency >= MAX_EMERGENCY) return -1;
    emergency_t* e = &emergencies[aa.n_emergency];
    e->emergency_id = aa.n_emergency;
    e->type = type;
    e->zone = zone;
    e->severity = severity;
    e->response_time = response;
    e->personnel = personnel;
    e->resolved = 0;
    e->year = year;
    e->active = 1;
    aa.total_emergencies++;
    aa.n_emergency++;
    print_str("[AA] Emergency "); print_int(aa.n_emergency - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" sev="); print_int(severity);
    print_str(" rt="); print_int(response); print_str("min\n");
    return aa.n_emergency - 1;
}

int aa_resolve_emergency(int emergency_id) {
    if (emergency_id >= aa.n_emergency) return -1;
    emergencies[emergency_id].resolved = 1;
    aa.total_resolved++;
    print_str("[AA] Resolve Em"); print_int(emergency_id); print_str("\n");
    return 0;
}

void aa_terminal_report(void) {
    print_str("[AA] Terminal report:\n");
    print_str("  Terminals: "); print_int(aa.n_terminals); print_str("\n");
    print_str("  Passengers: "); print_int(aa.total_passengers); print_str("\n");
    print_str("  Gates: "); print_int(aa.n_gates); print_str("\n");
}

void aa_runway_report(void) {
    print_str("[AA] Runway report:\n");
    print_str("  Runways: "); print_int(aa.n_runways); print_str("\n");
    print_str("  Operations: "); print_int(aa.total_operations); print_str("\n");
}

void aa_revenue_report(void) {
    print_str("[AA] Revenue report:\n");
    print_str("  Services: "); print_int(aa.n_services); print_str("\n");
    print_str("  Revenue: $"); print_int(aa.total_revenue); print_str("\n");
}

void aa_print_state(void) {
    print_str("[AA] Tm="); print_int(aa.n_terminals);
    print_str(" Rw="); print_int(aa.n_runways);
    print_str(" Gt="); print_int(aa.n_gates);
    print_str(" Sv="); print_int(aa.n_services);
    print_str(" Em="); print_int(aa.n_emergency);
    print_str("\n");
}

int main(void) {
    print_str("=== Airport Authority Demo ===\n\n");
    aa_init();

    print_str("Adding terminals...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int cap = 5000000 + (i * 1000000);
        int cnt = 50 + (i * 10);
        int shops = 20 + (i * 5);
        int pax = 3000000 + (i * 500000);
        int year = 2000 + (i * 2);
        aa_add_terminal(zone, cap, cnt, shops, pax, year);
    }

    print_str("\nAdding runways...\n");
    for (int i = 0; i < 8; i++) {
        int zone = (i % 4) + 1;
        int len = 2500 + (i * 200);
        int w = 45 + (i * 5);
        int srf = (i % 3) + 1;
        aa_add_runway(zone, len, w, srf);
    }

    print_str("\nRecording operations...\n");
    for (int i = 0; i < 20; i++) {
        int rw = i % 8;
        int ops = 10 + (i % 15);
        aa_record_operations(rw, ops);
    }

    print_str("\nAdding gates...\n");
    for (int i = 0; i < 16; i++) {
        int tm = i % 10;
        int type = (i % 3) + 1;
        int cap = 100 + (i * 20);
        aa_add_gate(tm, type, cap);
    }

    print_str("\nHandling flights...\n");
    for (int i = 0; i < 30; i++) {
        int gate = i % 16;
        int fl = 2 + (i % 5);
        aa_handle_flights(gate, fl);
    }

    print_str("\nAdding services...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int prov = 100 + i;
        int rev = 500000 + (i * 100000);
        int cust = 10000 + (i * 5000);
        int year = 2024;
        aa_add_service(type, zone, prov, rev, cust, year);
    }

    print_str("\nTriggering emergencies...\n");
    for (int i = 0; i < 8; i++) {
        int type = (i % 3) + 1;
        int zone = (i % 4) + 1;
        int sev = (i % 4) + 1;
        int rt = 3 + (i % 8);
        int pers = 10 + (i * 5);
        int year = 2022 + (i % 3);
        aa_trigger_emergency(type, zone, sev, rt, pers, year);
    }

    print_str("\nResolving emergencies...\n");
    for (int i = 0; i < 6; i++) {
        aa_resolve_emergency(i);
    }

    print_str("\nTerminal report...\n");
    aa_terminal_report();

    print_str("\nRunway report...\n");
    aa_runway_report();

    print_str("\nRevenue report...\n");
    aa_revenue_report();

    print_str("\nFinal state...\n");
    aa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
