/* emergency_services: Emergency services management (v1.0)
 * Fire, ambulance, police, dispatch, disaster management
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
#define MAX_UNITS      16
#define MAX_CALLS      20
#define MAX_PERSONNEL  24
#define MAX_DISASTERS  8

typedef struct {
    int    station_id;
    int    type;
    int    zone;
    int    n_units;
    int    n_personnel;
    int    n_calls;
    double response_budget;
    int    active;
} station_t;

typedef struct {
    int    unit_id;
    int    station_id;
    int    type;
    int    capacity;
    int    status;
    int    calls_handled;
    double maintenance_cost;
    int    active;
} unit_t;

typedef struct {
    int    call_id;
    int    type;
    int    priority;
    int    zone;
    int    unit_id;
    int    response_min;
    int    resolved;
    int    day;
    int    active;
} call_t;

typedef struct {
    int    pers_id;
    int    station_id;
    int    role;
    int    shift;
    int    calls_worked;
    double salary;
    int    training_hours;
    int    active;
} personnel_t;

typedef struct {
    int    disaster_id;
    int    type;
    int    severity;
    int    zone;
    int    n_units_deployed;
    int    n_personnel_deployed;
    double cost;
    int    resolved;
    int    day;
    int    active;
} disaster_t;

typedef struct {
    int    n_stations;
    int    n_units;
    int    n_calls;
    int    n_personnel;
    int    n_disasters;
    double operating_budget;
    double personnel_costs;
    double maintenance_costs;
    double disaster_costs;
    double total_costs;
    int    total_calls_handled;
    int    avg_response_min;
    int    total_disasters;
} es_state_t;

static station_t stations[MAX_STATIONS];
static unit_t units[MAX_UNITS];
static call_t calls[MAX_CALLS];
static personnel_t personnel[MAX_PERSONNEL];
static disaster_t disasters[MAX_DISASTERS];
static es_state_t es;

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

int es_init(void) {
    if (initialized) return -1;
    es.n_stations = 0; es.n_units = 0; es.n_calls = 0;
    es.n_personnel = 0; es.n_disasters = 0;
    es.operating_budget = 0.0; es.personnel_costs = 0.0;
    es.maintenance_costs = 0.0; es.disaster_costs = 0.0;
    es.total_costs = 0.0; es.total_calls_handled = 0;
    es.avg_response_min = 0; es.total_disasters = 0;
    for (int i = 0; i < MAX_STATIONS; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_UNITS; i++) units[i].active = 0;
    for (int i = 0; i < MAX_CALLS; i++) calls[i].active = 0;
    for (int i = 0; i < MAX_PERSONNEL; i++) personnel[i].active = 0;
    for (int i = 0; i < MAX_DISASTERS; i++) disasters[i].active = 0;
    initialized = 1;
    print_str("[ES] Emergency services initialized\n");
    return 0;
}

int es_add_station(int type, int zone, int n_units, int n_pers, double budget) {
    if (es.n_stations >= MAX_STATIONS) return -1;
    station_t* st = &stations[es.n_stations];
    st->station_id = es.n_stations;
    st->type = type;
    st->zone = zone;
    st->n_units = n_units;
    st->n_personnel = n_pers;
    st->n_calls = 0;
    st->response_budget = budget;
    st->active = 1;
    es.operating_budget += budget;
    es.total_costs += budget;
    es.n_stations++;
    print_str("[ES] Station "); print_int(es.n_stations - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" units="); print_int(n_units);
    print_str(" pers="); print_int(n_pers);
    print_str(" $"); print_int((int)budget); print_str("\n");
    return es.n_stations - 1;
}

int es_add_unit(int station_id, int type, int capacity) {
    if (es.n_units >= MAX_UNITS || station_id >= es.n_stations) return -1;
    unit_t* u = &units[es.n_units];
    u->unit_id = es.n_units;
    u->station_id = station_id;
    u->type = type;
    u->capacity = capacity;
    u->status = 1;
    u->calls_handled = 0;
    u->maintenance_cost = 0.0;
    u->active = 1;
    es.n_units++;
    print_str("[ES] Unit "); print_int(es.n_units - 1);
    print_str(" St"); print_int(station_id);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity); print_str("\n");
    return es.n_units - 1;
}

int es_add_personnel(int station_id, int role, int shift, double salary) {
    if (es.n_personnel >= MAX_PERSONNEL || station_id >= es.n_stations) return -1;
    personnel_t* p = &personnel[es.n_personnel];
    p->pers_id = es.n_personnel;
    p->station_id = station_id;
    p->role = role;
    p->shift = shift;
    p->calls_worked = 0;
    p->salary = salary;
    p->training_hours = 0;
    p->active = 1;
    es.personnel_costs += salary;
    es.total_costs += salary;
    es.n_personnel++;
    print_str("[ES] Pers "); print_int(es.n_personnel - 1);
    print_str(" St"); print_int(station_id);
    print_str(" role="); print_int(role);
    print_str(" shift="); print_int(shift);
    print_str(" $"); print_int((int)salary); print_str("\n");
    return es.n_personnel - 1;
}

int es_receive_call(int type, int priority, int zone, int unit_id) {
    if (es.n_calls >= MAX_CALLS || unit_id >= es.n_units) return -1;
    call_t* c = &calls[es.n_calls];
    c->call_id = es.n_calls;
    c->type = type;
    c->priority = priority;
    c->zone = zone;
    c->unit_id = unit_id;
    c->response_min = 0;
    c->resolved = 0;
    c->day = 1 + es.n_calls % 28;
    c->active = 1;
    es.n_calls++;
    print_str("[ES] Call "); print_int(es.n_calls - 1);
    print_str(" type="); print_int(type);
    print_str(" pri="); print_int(priority);
    print_str(" zone="); print_int(zone);
    print_str(" U"); print_int(unit_id); print_str("\n");
    return es.n_calls - 1;
}

int es_dispatch(int call_id, int response_min) {
    if (call_id >= es.n_calls) return -1;
    call_t* c = &calls[call_id];
    c->response_min = response_min;
    c->resolved = 1;
    units[c->unit_id].calls_handled++;
    stations[units[c->unit_id].station_id].n_calls++;
    es.total_calls_handled++;
    es.avg_response_min += response_min;
    print_str("[ES] Dispatch C"); print_int(call_id);
    print_str(" min="); print_int(response_min); print_str("\n");
    return 0;
}

int es_maintain_unit(int unit_id, double cost) {
    if (unit_id >= es.n_units) return -1;
    units[unit_id].maintenance_cost += cost;
    es.maintenance_costs += cost;
    es.total_costs += cost;
    print_str("[ES] Maintain U"); print_int(unit_id);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

int es_record_disaster(int type, int severity, int zone, int n_units, int n_pers, double cost) {
    if (es.n_disasters >= MAX_DISASTERS) return -1;
    disaster_t* d = &disasters[es.n_disasters];
    d->disaster_id = es.n_disasters;
    d->type = type;
    d->severity = severity;
    d->zone = zone;
    d->n_units_deployed = n_units;
    d->n_personnel_deployed = n_pers;
    d->cost = cost;
    d->resolved = 0;
    d->day = 10 + es.n_disasters * 5;
    d->active = 1;
    es.disaster_costs += cost;
    es.total_costs += cost;
    es.total_disasters++;
    es.n_disasters++;
    print_str("[ES] Disaster "); print_int(es.n_disasters - 1);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" zone="); print_int(zone);
    print_str(" units="); print_int(n_units);
    print_str(" pers="); print_int(n_pers);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return es.n_disasters - 1;
}

int es_resolve_disaster(int disaster_id) {
    if (disaster_id >= es.n_disasters) return -1;
    disasters[disaster_id].resolved = 1;
    print_str("[ES] Resolve D"); print_int(disaster_id); print_str("\n");
    return 0;
}

int es_training(int pers_id, int hours) {
    if (pers_id >= es.n_personnel) return -1;
    personnel[pers_id].training_hours += hours;
    print_str("[ES] Train P"); print_int(pers_id);
    print_str(" hrs="); print_int(hours); print_str("\n");
    return 0;
}

void es_station_report(void) {
    print_str("[ES] Station report:\n");
    for (int i = 0; i < es.n_stations; i++) {
        station_t* st = &stations[i];
        print_str("  St"); print_int(i);
        print_str(" type="); print_int(st->type);
        print_str(" zone="); print_int(st->zone);
        print_str(" units="); print_int(st->n_units);
        print_str(" calls="); print_int(st->n_calls); print_str("\n");
    }
}

void es_response_report(void) {
    print_str("[ES] Response report:\n");
    for (int i = 0; i < es.n_calls; i++) {
        call_t* c = &calls[i];
        print_str("  C"); print_int(i);
        print_str(" type="); print_int(c->type);
        print_str(" pri="); print_int(c->priority);
        print_str(" U"); print_int(c->unit_id);
        print_str(" min="); print_int(c->response_min);
        print_str(" done="); print_int(c->resolved); print_str("\n");
    }
}

void es_cost_report(void) {
    print_str("[ES] Cost report:\n");
    print_str("  Operating budget: "); print_int((int)es.operating_budget); print_str("\n");
    print_str("  Personnel costs: "); print_int((int)es.personnel_costs); print_str("\n");
    print_str("  Maintenance: "); print_int((int)es.maintenance_costs); print_str("\n");
    print_str("  Disaster costs: "); print_int((int)es.disaster_costs); print_str("\n");
    print_str("  Total costs: "); print_int((int)es.total_costs); print_str("\n");
    print_str("  Calls handled: "); print_int(es.total_calls_handled); print_str("\n");
    if (es.total_calls_handled > 0) {
        print_str("  Avg response: "); print_int(es.avg_response_min / es.total_calls_handled); print_str("min\n");
    }
    print_str("  Disasters: "); print_int(es.total_disasters); print_str("\n");
}

void es_print_state(void) {
    print_str("[ES] Stations="); print_int(es.n_stations);
    print_str(" Units="); print_int(es.n_units);
    print_str(" Calls="); print_int(es.n_calls);
    print_str(" Pers="); print_int(es.n_personnel);
    print_str(" Disasters="); print_int(es.n_disasters);
    print_str("\n");
    print_str("  Costs: "); print_int((int)es.total_costs); print_str("\n");
}

int main(void) {
    print_str("=== Emergency Services Demo ===\n\n");
    es_init();

    print_str("Adding stations...\n");
    es_add_station(1, 1, 3, 15, 500000.0);
    es_add_station(1, 2, 2, 12, 450000.0);
    es_add_station(2, 1, 3, 10, 400000.0);
    es_add_station(2, 3, 2, 8, 350000.0);
    es_add_station(3, 2, 3, 12, 480000.0);
    es_add_station(3, 3, 2, 10, 420000.0);
    es_add_station(1, 4, 2, 10, 380000.0);
    es_add_station(2, 4, 2, 8, 340000.0);
    es_add_station(3, 1, 3, 14, 520000.0);
    es_add_station(1, 3, 2, 10, 400000.0);
    es_add_station(2, 2, 2, 10, 420000.0);
    es_add_station(3, 4, 2, 8, 360000.0);

    print_str("\nAdding units...\n");
    es_add_unit(0, 1, 4);
    es_add_unit(0, 2, 2);
    es_add_unit(1, 1, 4);
    es_add_unit(2, 3, 6);
    es_add_unit(2, 4, 4);
    es_add_unit(3, 3, 6);
    es_add_unit(4, 5, 8);
    es_add_unit(4, 6, 6);
    es_add_unit(5, 5, 8);
    es_add_unit(6, 1, 4);
    es_add_unit(7, 3, 6);
    es_add_unit(8, 5, 8);
    es_add_unit(8, 2, 2);
    es_add_unit(9, 1, 4);
    es_add_unit(10, 4, 4);
    es_add_unit(11, 6, 6);

    print_str("\nAdding personnel...\n");
    for (int i = 0; i < 24; i++) {
        int station = i % 12;
        int role = (i % 4) + 1;
        int shift = (i % 3) + 1;
        double salary = 40000.0 + (i * 2500);
        es_add_personnel(station, role, shift, salary);
    }

    print_str("\nReceiving calls...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 3) + 1;
        int pri = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int unit = i % 16;
        es_receive_call(type, pri, zone, unit);
    }

    print_str("\nDispatching...\n");
    for (int i = 0; i < 20; i++) {
        int response = 3 + (i % 10);
        es_dispatch(i, response);
    }

    print_str("\nUnit maintenance...\n");
    for (int i = 0; i < 16; i++) {
        es_maintain_unit(i, 5000.0 + (i * 1000));
    }

    print_str("\nRecording disasters...\n");
    es_record_disaster(1, 3, 1, 4, 20, 200000.0);
    es_record_disaster(2, 4, 2, 6, 30, 350000.0);
    es_record_disaster(3, 2, 3, 3, 15, 120000.0);
    es_record_disaster(1, 5, 1, 8, 40, 500000.0);
    es_record_disaster(4, 3, 4, 5, 25, 250000.0);
    es_record_disaster(2, 4, 2, 7, 35, 400000.0);
    es_record_disaster(3, 2, 3, 3, 12, 100000.0);
    es_record_disaster(1, 3, 4, 4, 18, 180000.0);

    print_str("\nResolving disasters...\n");
    for (int i = 0; i < 8; i++) {
        es_resolve_disaster(i);
    }

    print_str("\nTraining...\n");
    for (int i = 0; i < 24; i++) {
        es_training(i, 20 + (i % 30));
    }

    print_str("\nStation report...\n");
    es_station_report();

    print_str("\nResponse report...\n");
    es_response_report();

    print_str("\nCost report...\n");
    es_cost_report();

    print_str("\nFinal state...\n");
    es_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
