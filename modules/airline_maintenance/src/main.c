/* airline_maintenance: Airline maintenance management (v1.0)
 * Aircraft, technicians, parts, work orders, compliance, costs
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

#define MAX_AIRCRAFT    10
#define MAX_TECHS       12
#define MAX_PARTS       14
#define MAX_WORKORDERS  16

typedef struct {
    int    aircraft_id;
    int    type;
    int    flight_hours;
    int    cycles;
    int    maint_status;
    int    n_workorders;
    double total_maint_cost;
    int    active;
} aircraft_t;

typedef struct {
    int    tech_id;
    int    rating;
    int    n_tasks;
    int    hours_worked;
    double hourly_rate;
    double total_pay;
    int    certified;
    int    active;
} tech_t;

typedef struct {
    int    part_id;
    int    type;
    int    stock;
    double unit_cost;
    int    reorder_level;
    int    n_used;
    double total_cost;
    int    active;
} part_t;

typedef struct {
    int    wo_id;
    int    aircraft_id;
    int    tech_id;
    int    type;
    int    hours;
    double labor_cost;
    double parts_cost;
    int    day;
    int    status;
    int    active;
} workorder_t;

typedef struct {
    int    n_aircraft;
    int    n_techs;
    int    n_parts;
    int    n_workorders;
    double labor_costs;
    double parts_costs;
    double downtime_costs;
    double total_costs;
    int    total_hours;
    int    n_certified;
} am_state_t;

static aircraft_t aircraft[MAX_AIRCRAFT];
static tech_t techs[MAX_TECHS];
static part_t parts[MAX_PARTS];
static workorder_t workorders[MAX_WORKORDERS];
static am_state_t am;

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

int am_init(void) {
    if (initialized) return -1;
    am.n_aircraft = 0; am.n_techs = 0; am.n_parts = 0; am.n_workorders = 0;
    am.labor_costs = 0.0; am.parts_costs = 0.0;
    am.downtime_costs = 0.0; am.total_costs = 0.0;
    am.total_hours = 0; am.n_certified = 0;
    for (int i = 0; i < MAX_AIRCRAFT; i++) aircraft[i].active = 0;
    for (int i = 0; i < MAX_TECHS; i++) techs[i].active = 0;
    for (int i = 0; i < MAX_PARTS; i++) parts[i].active = 0;
    for (int i = 0; i < MAX_WORKORDERS; i++) workorders[i].active = 0;
    initialized = 1;
    print_str("[AM] Maintenance initialized\n");
    return 0;
}

int am_add_aircraft(int type, int hours, int cycles) {
    if (am.n_aircraft >= MAX_AIRCRAFT) return -1;
    aircraft_t* a = &aircraft[am.n_aircraft];
    a->aircraft_id = am.n_aircraft;
    a->type = type;
    a->flight_hours = hours;
    a->cycles = cycles;
    a->maint_status = 1;
    a->n_workorders = 0;
    a->total_maint_cost = 0.0;
    a->active = 1;
    am.n_aircraft++;
    print_str("[AM] Aircraft "); print_int(am.n_aircraft - 1);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(hours);
    print_str(" cyc="); print_int(cycles); print_str("\n");
    return am.n_aircraft - 1;
}

int am_add_tech(int rating, double rate, int certified) {
    if (am.n_techs >= MAX_TECHS) return -1;
    tech_t* t = &techs[am.n_techs];
    t->tech_id = am.n_techs;
    t->rating = rating;
    t->n_tasks = 0;
    t->hours_worked = 0;
    t->hourly_rate = rate;
    t->total_pay = 0.0;
    t->certified = certified;
    t->active = 1;
    if (certified) am.n_certified++;
    am.n_techs++;
    print_str("[AM] Tech "); print_int(am.n_techs - 1);
    print_str(" rating="); print_int(rating);
    print_str(" $"); print_int((int)rate); print_str("/hr");
    if (certified) print_str(" [CERT]");
    print_str("\n");
    return am.n_techs - 1;
}

int am_add_part(int type, int stock, double cost, int reorder) {
    if (am.n_parts >= MAX_PARTS) return -1;
    part_t* p = &parts[am.n_parts];
    p->part_id = am.n_parts;
    p->type = type;
    p->stock = stock;
    p->unit_cost = cost;
    p->reorder_level = reorder;
    p->n_used = 0;
    p->total_cost = 0.0;
    p->active = 1;
    am.n_parts++;
    print_str("[AM] Part "); print_int(am.n_parts - 1);
    print_str(" type="); print_int(type);
    print_str(" stock="); print_int(stock);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return am.n_parts - 1;
}

int am_create_workorder(int aircraft_id, int tech_id, int type, int hours, int day) {
    if (am.n_workorders >= MAX_WORKORDERS) return -1;
    if (aircraft_id >= am.n_aircraft || tech_id >= am.n_techs) return -2;
    workorder_t* wo = &workorders[am.n_workorders];
    wo->wo_id = am.n_workorders;
    wo->aircraft_id = aircraft_id;
    wo->tech_id = tech_id;
    wo->type = type;
    wo->hours = hours;
    wo->labor_cost = techs[tech_id].hourly_rate * hours;
    wo->parts_cost = 0.0;
    wo->day = day;
    wo->status = 1;
    wo->active = 1;
    aircraft[aircraft_id].n_workorders++;
    techs[tech_id].n_tasks++;
    techs[tech_id].hours_worked += hours;
    techs[tech_id].total_pay += wo->labor_cost;
    am.labor_costs += wo->labor_cost;
    am.total_hours += hours;
    am.n_workorders++;
    print_str("[AM] WO "); print_int(am.n_workorders - 1);
    print_str(" Ac"); print_int(aircraft_id);
    print_str(" T"); print_int(tech_id);
    print_str(" type="); print_int(type);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)wo->labor_cost); print_str("\n");
    return am.n_workorders - 1;
}

int am_use_part(int workorder_id, int part_id, int qty) {
    if (workorder_id >= am.n_workorders || part_id >= am.n_parts) return -1;
    part_t* p = &parts[part_id];
    if (p->stock < qty) return -2;
    workorder_t* wo = &workorders[workorder_id];
    double cost = p->unit_cost * qty;
    p->stock -= qty;
    p->n_used += qty;
    p->total_cost += cost;
    wo->parts_cost += cost;
    aircraft[wo->aircraft_id].total_maint_cost += cost;
    am.parts_costs += cost;
    print_str("[AM] WO"); print_int(workorder_id);
    print_str(" used P"); print_int(part_id);
    print_str(" qty="); print_int(qty);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

int am_complete_workorder(int workorder_id) {
    if (workorder_id >= am.n_workorders) return -1;
    workorder_t* wo = &workorders[workorder_id];
    wo->status = 2;
    double total = wo->labor_cost + wo->parts_cost;
    aircraft[wo->aircraft_id].total_maint_cost += total;
    am.total_costs += total;
    print_str("[AM] WO"); print_int(workorder_id);
    print_str(" complete $"); print_int((int)total); print_str("\n");
    return 0;
}

int am_aircraft_downtime(int aircraft_id, int hours) {
    if (aircraft_id >= am.n_aircraft) return -1;
    double cost = hours * 500.0;
    aircraft[aircraft_id].maint_status = 2;
    am.downtime_costs += cost;
    am.total_costs += cost;
    print_str("[AM] Ac"); print_int(aircraft_id);
    print_str(" down "); print_int(hours); print_str("hrs");
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

int am_aircraft_return(int aircraft_id) {
    if (aircraft_id >= am.n_aircraft) return -1;
    aircraft[aircraft_id].maint_status = 1;
    print_str("[AM] Ac"); print_int(aircraft_id);
    print_str(" returned to service\n");
    return 0;
}

void am_aircraft_report(void) {
    print_str("[AM] Aircraft report:\n");
    for (int i = 0; i < am.n_aircraft; i++) {
        aircraft_t* a = &aircraft[i];
        print_str("  Ac"); print_int(i);
        print_str(" type="); print_int(a->type);
        print_str(" hrs="); print_int(a->flight_hours);
        print_str(" wo="); print_int(a->n_workorders);
        print_str(" maint$="); print_int((int)a->total_maint_cost);
        print_str(" status="); print_int(a->maint_status); print_str("\n");
    }
}

void am_cost_report(void) {
    print_str("[AM] Cost report:\n");
    print_str("  Labor costs: "); print_int((int)am.labor_costs); print_str("\n");
    print_str("  Parts costs: "); print_int((int)am.parts_costs); print_str("\n");
    print_str("  Downtime costs: "); print_int((int)am.downtime_costs); print_str("\n");
    print_str("  Total costs: "); print_int((int)am.total_costs); print_str("\n");
    print_str("  Total hours: "); print_int(am.total_hours); print_str("\n");
    print_str("  Certified techs: "); print_int(am.n_certified); print_str("\n");
}

void am_print_state(void) {
    print_str("[AM] Aircraft="); print_int(am.n_aircraft);
    print_str(" Techs="); print_int(am.n_techs);
    print_str(" Parts="); print_int(am.n_parts);
    print_str(" WOs="); print_int(am.n_workorders);
    print_str("\n");
    print_str("  Costs: "); print_int((int)am.total_costs); print_str("\n");
}

int main(void) {
    print_str("=== Airline Maintenance Demo ===\n\n");
    am_init();

    print_str("Adding aircraft...\n");
    am_add_aircraft(1, 15000, 8000);
    am_add_aircraft(1, 12000, 6500);
    am_add_aircraft(2, 20000, 10000);
    am_add_aircraft(2, 18000, 9000);
    am_add_aircraft(3, 25000, 12000);
    am_add_aircraft(3, 22000, 11000);
    am_add_aircraft(1, 8000, 4000);
    am_add_aircraft(2, 30000, 15000);
    am_add_aircraft(3, 10000, 5000);
    am_add_aircraft(1, 5000, 2500);

    print_str("\nAdding technicians...\n");
    am_add_tech(3, 65.0, 1);
    am_add_tech(2, 50.0, 1);
    am_add_tech(3, 70.0, 1);
    am_add_tech(1, 40.0, 0);
    am_add_tech(2, 55.0, 1);
    am_add_tech(3, 75.0, 1);
    am_add_tech(1, 35.0, 0);
    am_add_tech(2, 52.0, 1);
    am_add_tech(3, 68.0, 1);
    am_add_tech(1, 38.0, 0);
    am_add_tech(2, 48.0, 1);
    am_add_tech(3, 72.0, 1);

    print_str("\nAdding parts...\n");
    am_add_part(1, 50, 250.0, 10);
    am_add_part(1, 40, 180.0, 8);
    am_add_part(2, 30, 500.0, 5);
    am_add_part(2, 25, 420.0, 5);
    am_add_part(3, 20, 1200.0, 3);
    am_add_part(3, 15, 980.0, 3);
    am_add_part(4, 60, 75.0, 15);
    am_add_part(4, 50, 60.0, 12);
    am_add_part(5, 35, 350.0, 8);
    am_add_part(5, 28, 280.0, 6);
    am_add_part(1, 45, 200.0, 10);
    am_add_part(2, 32, 450.0, 7);
    am_add_part(3, 18, 1100.0, 4);
    am_add_part(4, 55, 85.0, 14);

    print_str("\nCreating work orders...\n");
    am_create_workorder(0, 0, 1, 4, 10);
    am_create_workorder(1, 1, 1, 3, 10);
    am_create_workorder(2, 2, 2, 8, 11);
    am_create_workorder(3, 3, 2, 6, 11);
    am_create_workorder(4, 4, 3, 12, 12);
    am_create_workorder(5, 5, 3, 10, 12);
    am_create_workorder(6, 6, 1, 4, 13);
    am_create_workorder(7, 7, 2, 8, 13);
    am_create_workorder(8, 8, 3, 14, 14);
    am_create_workorder(9, 9, 1, 3, 14);
    am_create_workorder(0, 10, 2, 6, 15);
    am_create_workorder(2, 11, 3, 10, 15);
    am_create_workorder(4, 0, 1, 5, 16);
    am_create_workorder(6, 2, 2, 7, 16);
    am_create_workorder(8, 4, 3, 12, 17);
    am_create_workorder(1, 6, 1, 4, 17);

    print_str("\nUsing parts...\n");
    am_use_part(0, 0, 2);
    am_use_part(1, 1, 1);
    am_use_part(2, 2, 3);
    am_use_part(3, 3, 2);
    am_use_part(4, 4, 4);
    am_use_part(5, 5, 3);
    am_use_part(6, 6, 2);
    am_use_part(7, 7, 1);
    am_use_part(8, 8, 2);
    am_use_part(9, 9, 1);
    am_use_part(10, 10, 2);
    am_use_part(11, 11, 3);
    am_use_part(12, 12, 2);
    am_use_part(13, 13, 1);
    am_use_part(14, 0, 1);
    am_use_part(15, 2, 2);

    print_str("\nCompleting work orders...\n");
    for (int i = 0; i < 16; i++) {
        am_complete_workorder(i);
    }

    print_str("\nAircraft downtime...\n");
    am_aircraft_downtime(0, 24);
    am_aircraft_downtime(2, 48);
    am_aircraft_downtime(4, 72);
    am_aircraft_downtime(6, 12);
    am_aircraft_downtime(8, 36);
    am_aircraft_return(0);
    am_aircraft_return(2);
    am_aircraft_return(4);
    am_aircraft_return(6);
    am_aircraft_return(8);

    print_str("\nAircraft report...\n");
    am_aircraft_report();

    print_str("\nCost report...\n");
    am_cost_report();

    print_str("\nFinal state...\n");
    am_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
