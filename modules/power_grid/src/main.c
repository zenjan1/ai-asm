/* power_grid: Power grid management (v1.0)
 * Power plants, substations, distribution, demand response, renewables
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

#define MAX_PLANTS      10
#define MAX_SUBSTATIONS 12
#define MAX_LINES       16
#define MAX_RENEWABLES  10
#define MAX_EVENTS      14

typedef struct {
    int    plant_id;
    int    type;
    int    capacity_mw;
    int    output_mw;
    double cost_per_mwh;
    int    n_units;
    int    online;
    double revenue;
    int    active;
} plant_t;

typedef struct {
    int    sub_id;
    int    voltage_kv;
    int    capacity_mw;
    int    load_mw;
    int    n_transformers;
    int    n_feeders;
    int    status;
    int    active;
} substation_t;

typedef struct {
    int    line_id;
    int    from_sub;
    int    to_sub;
    int    length_km;
    int    capacity_mw;
    int    load_mw;
    int    faults;
    int    active;
} line_t;

typedef struct {
    int    ren_id;
    int    type;
    int    capacity_mw;
    int    output_mw;
    double cost_per_mwh;
    int    availability;
    double revenue;
    int    active;
} renewable_t;

typedef struct {
    int    event_id;
    int    type;
    int    severity;
    int    affected;
    int    resolved;
    int    day;
    int    active;
} event_t;

typedef struct {
    int    n_plants;
    int    n_substations;
    int    n_lines;
    int    n_renewables;
    int    n_events;
    double generation_revenue;
    double renewable_revenue;
    double total_revenue;
    double fuel_costs;
    double maintenance_costs;
    int    total_generation_mw;
    int    total_load_mw;
    int    total_faults;
} pg_state_t;

static plant_t plants[MAX_PLANTS];
static substation_t substations[MAX_SUBSTATIONS];
static line_t lines[MAX_LINES];
static renewable_t renewables[MAX_RENEWABLES];
static event_t events[MAX_EVENTS];
static pg_state_t pg;

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

int pg_init(void) {
    if (initialized) return -1;
    pg.n_plants = 0; pg.n_substations = 0; pg.n_lines = 0;
    pg.n_renewables = 0; pg.n_events = 0;
    pg.generation_revenue = 0.0; pg.renewable_revenue = 0.0;
    pg.total_revenue = 0.0; pg.fuel_costs = 0.0;
    pg.maintenance_costs = 0.0;
    pg.total_generation_mw = 0; pg.total_load_mw = 0;
    pg.total_faults = 0;
    for (int i = 0; i < MAX_PLANTS; i++) plants[i].active = 0;
    for (int i = 0; i < MAX_SUBSTATIONS; i++) substations[i].active = 0;
    for (int i = 0; i < MAX_LINES; i++) lines[i].active = 0;
    for (int i = 0; i < MAX_RENEWABLES; i++) renewables[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    initialized = 1;
    print_str("[PG] Power grid initialized\n");
    return 0;
}

int pg_add_plant(int type, int capacity, int output, double cost) {
    if (pg.n_plants >= MAX_PLANTS) return -1;
    plant_t* p = &plants[pg.n_plants];
    p->plant_id = pg.n_plants;
    p->type = type;
    p->capacity_mw = capacity;
    p->output_mw = output;
    p->cost_per_mwh = cost;
    p->n_units = 2 + (type % 3);
    p->online = 1;
    p->revenue = 0.0;
    p->active = 1;
    pg.total_generation_mw += output;
    pg.fuel_costs += cost * output * 0.3;
    pg.n_plants++;
    print_str("[PG] Plant "); print_int(pg.n_plants - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str("MW out="); print_int(output); print_str("MW\n");
    return pg.n_plants - 1;
}

int pg_add_substation(int voltage_kv, int capacity, int n_transformers, int n_feeders) {
    if (pg.n_substations >= MAX_SUBSTATIONS) return -1;
    substation_t* s = &substations[pg.n_substations];
    s->sub_id = pg.n_substations;
    s->voltage_kv = voltage_kv;
    s->capacity_mw = capacity;
    s->load_mw = 0;
    s->n_transformers = n_transformers;
    s->n_feeders = n_feeders;
    s->status = 1;
    s->active = 1;
    pg.n_substations++;
    print_str("[PG] Sub "); print_int(pg.n_substations - 1);
    print_str(" kV="); print_int(voltage_kv);
    print_str(" cap="); print_int(capacity);
    print_str("MW tr="); print_int(n_transformers);
    print_str(" fd="); print_int(n_feeders); print_str("\n");
    return pg.n_substations - 1;
}

int pg_add_line(int from_sub, int to_sub, int length_km, int capacity) {
    if (pg.n_lines >= MAX_LINES) return -1;
    if (from_sub >= pg.n_substations || to_sub >= pg.n_substations) return -2;
    line_t* l = &lines[pg.n_lines];
    l->line_id = pg.n_lines;
    l->from_sub = from_sub;
    l->to_sub = to_sub;
    l->length_km = length_km;
    l->capacity_mw = capacity;
    l->load_mw = 0;
    l->faults = 0;
    l->active = 1;
    pg.n_lines++;
    print_str("[PG] Line "); print_int(pg.n_lines - 1);
    print_str(" S"); print_int(from_sub); print_str(">S"); print_int(to_sub);
    print_str(" km="); print_int(length_km);
    print_str(" cap="); print_int(capacity); print_str("MW\n");
    return pg.n_lines - 1;
}

int pg_add_renewable(int type, int capacity, double cost) {
    if (pg.n_renewables >= MAX_RENEWABLES) return -1;
    renewable_t* r = &renewables[pg.n_renewables];
    r->ren_id = pg.n_renewables;
    r->type = type;
    r->capacity_mw = capacity;
    r->output_mw = capacity * 6 / 10;
    r->cost_per_mwh = cost;
    r->availability = 70 + (type * 5);
    r->revenue = 0.0;
    r->active = 1;
    pg.total_generation_mw += r->output_mw;
    pg.n_renewables++;
    print_str("[PG] Renewable "); print_int(pg.n_renewables - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str("MW out="); print_int(r->output_mw); print_str("MW\n");
    return pg.n_renewables - 1;
}

int pg_dispatch_load(int sub_id, int load_mw) {
    if (sub_id >= pg.n_substations) return -1;
    substation_t* s = &substations[sub_id];
    if (s->load_mw + load_mw > s->capacity_mw) return -2;
    s->load_mw += load_mw;
    pg.total_load_mw += load_mw;
    double rev = load_mw * 45.0;
    pg.generation_revenue += rev;
    pg.total_revenue += rev;
    print_str("[PG] Dispatch S"); print_int(sub_id);
    print_str(" load="); print_int(load_mw);
    print_str("MW $"); print_int((int)rev); print_str("\n");
    return 0;
}

int pg_record_fault(int line_id) {
    if (line_id >= pg.n_lines) return -1;
    line_t* l = &lines[line_id];
    l->faults++;
    pg.total_faults++;
    pg.maintenance_costs += 500.0;
    print_str("[PG] Fault L"); print_int(line_id);
    print_str(" total="); print_int(l->faults); print_str("\n");
    return 0;
}

int pg_record_event(int type, int severity, int affected, int day) {
    if (pg.n_events >= MAX_EVENTS) return -1;
    event_t* e = &events[pg.n_events];
    e->event_id = pg.n_events;
    e->type = type;
    e->severity = severity;
    e->affected = affected;
    e->resolved = 0;
    e->day = day;
    e->active = 1;
    pg.n_events++;
    print_str("[PG] Event "); print_int(pg.n_events - 1);
    print_str(" type="); print_int(type);
    print_str(" sev="); print_int(severity);
    print_str(" aff="); print_int(affected); print_str("\n");
    return pg.n_events - 1;
}

int pg_renewable_generate(int ren_id, int hours) {
    if (ren_id >= pg.n_renewables) return -1;
    renewable_t* r = &renewables[ren_id];
    double rev = r->output_mw * hours * 30.0;
    r->revenue += rev;
    pg.renewable_revenue += rev;
    pg.total_revenue += rev;
    print_str("[PG] Ren "); print_int(ren_id);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int pg_plant_generate(int plant_id, int hours) {
    if (plant_id >= pg.n_plants) return -1;
    plant_t* p = &plants[plant_id];
    double rev = p->output_mw * hours * 50.0;
    p->revenue += rev;
    pg.generation_revenue += rev;
    pg.total_revenue += rev;
    pg.fuel_costs += p->cost_per_mwh * p->output_mw * hours * 0.2;
    print_str("[PG] Plant "); print_int(plant_id);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int pg_resolve_event(int event_id) {
    if (event_id >= pg.n_events) return -1;
    events[event_id].resolved = 1;
    pg.maintenance_costs += 200.0;
    print_str("[PG] Resolve E"); print_int(event_id); print_str("\n");
    return 0;
}

void pg_grid_report(void) {
    print_str("[PG] Grid report:\n");
    for (int i = 0; i < pg.n_plants; i++) {
        plant_t* p = &plants[i];
        print_str("  P"); print_int(i);
        print_str(" type="); print_int(p->type);
        print_str(" out="); print_int(p->output_mw);
        print_str("/"); print_int(p->capacity_mw);
        print_str("MW rev="); print_int((int)p->revenue); print_str("\n");
    }
    for (int i = 0; i < pg.n_substations; i++) {
        substation_t* s = &substations[i];
        print_str("  S"); print_int(i);
        print_str(" kV="); print_int(s->voltage_kv);
        print_str(" load="); print_int(s->load_mw);
        print_str("/"); print_int(s->capacity_mw); print_str("MW\n");
    }
}

void pg_financial_report(void) {
    print_str("[PG] Financial report:\n");
    print_str("  Generation revenue: "); print_int((int)pg.generation_revenue); print_str("\n");
    print_str("  Renewable revenue: "); print_int((int)pg.renewable_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)pg.total_revenue); print_str("\n");
    print_str("  Fuel costs: "); print_int((int)pg.fuel_costs); print_str("\n");
    print_str("  Maintenance costs: "); print_int((int)pg.maintenance_costs); print_str("\n");
    double profit = pg.total_revenue - pg.fuel_costs - pg.maintenance_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Total gen: "); print_int(pg.total_generation_mw);
    print_str("MW Load: "); print_int(pg.total_load_mw);
    print_str("MW Faults: "); print_int(pg.total_faults); print_str("\n");
}

void pg_print_state(void) {
    print_str("[PG] Plants="); print_int(pg.n_plants);
    print_str(" Subs="); print_int(pg.n_substations);
    print_str(" Lines="); print_int(pg.n_lines);
    print_str(" Renew="); print_int(pg.n_renewables);
    print_str(" Events="); print_int(pg.n_events);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)pg.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Power Grid Demo ===\n\n");
    pg_init();

    print_str("Adding power plants...\n");
    pg_add_plant(1, 500, 420, 25.0);
    pg_add_plant(1, 600, 510, 28.0);
    pg_add_plant(2, 800, 700, 18.0);
    pg_add_plant(2, 1000, 850, 20.0);
    pg_add_plant(3, 400, 350, 15.0);
    pg_add_plant(4, 300, 280, 12.0);
    pg_add_plant(3, 450, 380, 16.0);
    pg_add_plant(5, 1200, 1100, 8.0);
    pg_add_plant(4, 350, 300, 14.0);
    pg_add_plant(5, 900, 820, 10.0);

    print_str("\nAdding substations...\n");
    pg_add_substation(220, 400, 3, 8);
    pg_add_substation(110, 250, 2, 6);
    pg_add_substation(220, 500, 4, 10);
    pg_add_substation(110, 300, 2, 8);
    pg_add_substation(330, 600, 3, 12);
    pg_add_substation(110, 200, 2, 5);
    pg_add_substation(220, 450, 3, 9);
    pg_add_substation(330, 550, 4, 11);
    pg_add_substation(110, 280, 2, 7);
    pg_add_substation(220, 420, 3, 8);
    pg_add_substation(110, 260, 2, 6);
    pg_add_substation(330, 580, 3, 10);

    print_str("\nAdding transmission lines...\n");
    pg_add_line(0, 1, 45, 300);
    pg_add_line(0, 2, 60, 400);
    pg_add_line(1, 3, 35, 250);
    pg_add_line(2, 4, 80, 500);
    pg_add_line(3, 5, 25, 200);
    pg_add_line(4, 6, 55, 350);
    pg_add_line(5, 7, 70, 450);
    pg_add_line(6, 8, 40, 280);
    pg_add_line(7, 9, 50, 380);
    pg_add_line(8, 10, 30, 220);
    pg_add_line(9, 11, 65, 420);
    pg_add_line(0, 4, 90, 350);
    pg_add_line(2, 6, 75, 400);
    pg_add_line(4, 8, 45, 300);
    pg_add_line(1, 5, 55, 250);
    pg_add_line(3, 7, 85, 380);

    print_str("\nAdding renewables...\n");
    pg_add_renewable(1, 200, 5.0);
    pg_add_renewable(1, 150, 6.0);
    pg_add_renewable(2, 300, 4.0);
    pg_add_renewable(2, 250, 4.5);
    pg_add_renewable(3, 100, 8.0);
    pg_add_renewable(3, 120, 7.5);
    pg_add_renewable(1, 180, 5.5);
    pg_add_renewable(2, 280, 4.2);
    pg_add_renewable(3, 90, 9.0);
    pg_add_renewable(1, 160, 5.8);

    print_str("\nDispatching load...\n");
    for (int i = 0; i < 12; i++) {
        pg_dispatch_load(i, 100 + (i * 20));
    }

    print_str("\nPlant generation...\n");
    for (int i = 0; i < 10; i++) {
        pg_plant_generate(i, 12 + (i % 6));
    }

    print_str("\nRenewable generation...\n");
    for (int i = 0; i < 10; i++) {
        pg_renewable_generate(i, 16 + (i % 8));
    }

    print_str("\nRecording faults...\n");
    pg_record_fault(0);
    pg_record_fault(3);
    pg_record_fault(7);
    pg_record_fault(11);
    pg_record_fault(5);
    pg_record_fault(2);

    print_str("\nGrid events...\n");
    pg_record_event(1, 2, 3, 10);
    pg_record_event(2, 3, 5, 11);
    pg_record_event(1, 1, 2, 12);
    pg_record_event(3, 4, 8, 13);
    pg_record_event(2, 2, 4, 14);
    pg_record_event(1, 3, 6, 15);
    pg_record_event(3, 1, 1, 16);
    pg_record_event(2, 2, 3, 17);
    pg_record_event(1, 4, 10, 18);
    pg_record_event(3, 2, 2, 19);
    pg_record_event(2, 1, 1, 20);
    pg_record_event(1, 3, 4, 21);
    pg_record_event(3, 2, 3, 22);
    pg_record_event(2, 3, 7, 23);

    print_str("\nResolving events...\n");
    for (int i = 0; i < 14; i++) {
        pg_resolve_event(i);
    }

    print_str("\nGrid report...\n");
    pg_grid_report();

    print_str("\nFinancial report...\n");
    pg_financial_report();

    print_str("\nFinal state...\n");
    pg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
