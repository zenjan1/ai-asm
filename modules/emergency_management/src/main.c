/* emergency_management: Emergency management system (v1.0)
 * Emergency plans, response, command, rescue forces, logistics
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

#define MAX_PLANS        16
#define MAX_RESPONSES    14
#define MAX_COMMAND      12
#define MAX_FORCES       10
#define MAX_LOGISTICS    10

typedef struct {
    int    plan_id;
    int    level;
    int    category;
    int    region_id;
    int    scenarios;
    int    resources;
    int    year_draft;
    int    last_review;
    int    active;
} plan_t;

typedef struct {
    int    response_id;
    int    incident_id;
    int    response_level;
    int    activated_units;
    int    deployed_personnel;
    int    duration_hours;
    int    casualties;
    int    year;
    int    active;
} response_t;

typedef struct {
    int    command_id;
    int    incident_id;
    int    commander_id;
    int    decisions_made;
    int    resources_dispatched;
    int    coordination_meetings;
    int    year;
    int    status;
    int    active;
} command_t;

typedef struct {
    int    force_id;
    int    type;
    int    unit_id;
    int    personnel;
    int    equipment;
    int    training_hours;
    int    readiness_pct;
    int    year;
    int    active;
} force_t;

typedef struct {
    int    logistics_id;
    int    category;
    int    warehouse_id;
    int    stock_quantity;
    int    stock_value;
    int    distributed;
    int    year;
    int    active;
} logistics_t;

typedef struct {
    int    n_plans;
    int    n_responses;
    int    n_command;
    int    n_forces;
    int    n_logistics;
    int    total_personnel;
    int    total_deployed;
    int    total_equipment;
    int    total_stock_value;
    int    total_training;
} em_state_t;

static plan_t plans[MAX_PLANS];
static response_t responses[MAX_RESPONSES];
static command_t commands[MAX_COMMAND];
static force_t forces[MAX_FORCES];
static logistics_t logistics[MAX_LOGISTICS];
static em_state_t em;

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

int em_init(void) {
    if (initialized) return -1;
    em.n_plans = 0; em.n_responses = 0; em.n_command = 0;
    em.n_forces = 0; em.n_logistics = 0;
    em.total_personnel = 0; em.total_deployed = 0;
    em.total_equipment = 0; em.total_stock_value = 0;
    em.total_training = 0;
    for (int i = 0; i < MAX_PLANS; i++) plans[i].active = 0;
    for (int i = 0; i < MAX_RESPONSES; i++) responses[i].active = 0;
    for (int i = 0; i < MAX_COMMAND; i++) commands[i].active = 0;
    for (int i = 0; i < MAX_FORCES; i++) forces[i].active = 0;
    for (int i = 0; i < MAX_LOGISTICS; i++) logistics[i].active = 0;
    initialized = 1;
    print_str("[EM] Emergency management initialized\n");
    return 0;
}

int em_plan(int level, int category, int region, int scenarios, int resources, int year) {
    if (em.n_plans >= MAX_PLANS) return -1;
    plan_t* p = &plans[em.n_plans];
    p->plan_id = em.n_plans;
    p->level = level;
    p->category = category;
    p->region_id = region;
    p->scenarios = scenarios;
    p->resources = resources;
    p->year_draft = year;
    p->last_review = year + 1;
    p->active = 1;
    em.n_plans++;
    print_str("[EM] Plan "); print_int(em.n_plans - 1);
    print_str(" lvl="); print_int(level);
    print_str(" cat="); print_int(category);
    print_str(" reg="); print_int(region);
    print_str(" scen="); print_int(scenarios);
    print_str(" res="); print_int(resources); print_str("\n");
    return em.n_plans - 1;
}

int em_respond(int incident, int level, int units, int personnel, int duration, int casualties, int year) {
    if (em.n_responses >= MAX_RESPONSES) return -1;
    response_t* r = &responses[em.n_responses];
    r->response_id = em.n_responses;
    r->incident_id = incident;
    r->response_level = level;
    r->activated_units = units;
    r->deployed_personnel = personnel;
    r->duration_hours = duration;
    r->casualties = casualties;
    r->year = year;
    r->active = 1;
    em.total_deployed += personnel;
    em.n_responses++;
    print_str("[EM] Response "); print_int(em.n_responses - 1);
    print_str(" inc="); print_int(incident);
    print_str(" lvl="); print_int(level);
    print_str(" units="); print_int(units);
    print_str(" pers="); print_int(personnel);
    print_str(" dur="); print_int(duration); print_str("h\n");
    return em.n_responses - 1;
}

int em_command(int incident, int commander, int decisions, int dispatched, int meetings, int year) {
    if (em.n_command >= MAX_COMMAND) return -1;
    command_t* c = &commands[em.n_command];
    c->command_id = em.n_command;
    c->incident_id = incident;
    c->commander_id = commander;
    c->decisions_made = decisions;
    c->resources_dispatched = dispatched;
    c->coordination_meetings = meetings;
    c->year = year;
    c->status = 1;
    c->active = 1;
    em.n_command++;
    print_str("[EM] Command "); print_int(em.n_command - 1);
    print_str(" inc="); print_int(incident);
    print_str(" cmd="); print_int(commander);
    print_str(" dec="); print_int(decisions);
    print_str(" disp="); print_int(dispatched);
    print_str(" mtg="); print_int(meetings); print_str("\n");
    return em.n_command - 1;
}

int em_add_force(int type, int unit, int personnel, int equipment, int training, int readiness, int year) {
    if (em.n_forces >= MAX_FORCES) return -1;
    force_t* f = &forces[em.n_forces];
    f->force_id = em.n_forces;
    f->type = type;
    f->unit_id = unit;
    f->personnel = personnel;
    f->equipment = equipment;
    f->training_hours = training;
    f->readiness_pct = readiness;
    f->year = year;
    f->active = 1;
    em.total_personnel += personnel;
    em.total_equipment += equipment;
    em.total_training += training;
    em.n_forces++;
    print_str("[EM] Force "); print_int(em.n_forces - 1);
    print_str(" type="); print_int(type);
    print_str(" unit="); print_int(unit);
    print_str(" pers="); print_int(personnel);
    print_str(" equip="); print_int(equipment);
    print_str(" read="); print_int(readiness); print_str("%\n");
    return em.n_forces - 1;
}

int em_add_logistics(int category, int warehouse, int quantity, int value, int distributed, int year) {
    if (em.n_logistics >= MAX_LOGISTICS) return -1;
    logistics_t* l = &logistics[em.n_logistics];
    l->logistics_id = em.n_logistics;
    l->category = category;
    l->warehouse_id = warehouse;
    l->stock_quantity = quantity;
    l->stock_value = value;
    l->distributed = distributed;
    l->year = year;
    l->active = 1;
    em.total_stock_value += value;
    em.n_logistics++;
    print_str("[EM] Logistics "); print_int(em.n_logistics - 1);
    print_str(" cat="); print_int(category);
    print_str(" wh="); print_int(warehouse);
    print_str(" qty="); print_int(quantity);
    print_str(" val=$"); print_int(value);
    print_str(" dist="); print_int(distributed); print_str("\n");
    return em.n_logistics - 1;
}

void em_plan_report(void) {
    print_str("[EM] Plan report:\n");
    print_str("  Plans: "); print_int(em.n_plans); print_str("\n");
    print_str("  Responses: "); print_int(em.n_responses); print_str("\n");
    print_str("  Total deployed: "); print_int(em.total_deployed); print_str("\n");
}

void em_force_report(void) {
    print_str("[EM] Force report:\n");
    print_str("  Forces: "); print_int(em.n_forces); print_str("\n");
    print_str("  Total personnel: "); print_int(em.total_personnel); print_str("\n");
    print_str("  Total equipment: "); print_int(em.total_equipment); print_str("\n");
}

void em_logistics_report(void) {
    print_str("[EM] Logistics report:\n");
    print_str("  Warehouses: "); print_int(em.n_logistics); print_str("\n");
    print_str("  Total stock value: $"); print_int(em.total_stock_value); print_str("\n");
    print_str("  Total training hours: "); print_int(em.total_training); print_str("\n");
}

void em_print_state(void) {
    print_str("[EM] Pl="); print_int(em.n_plans);
    print_str(" Rs="); print_int(em.n_responses);
    print_str(" Cm="); print_int(em.n_command);
    print_str(" Fc="); print_int(em.n_forces);
    print_str(" Lg="); print_int(em.n_logistics);
    print_str("\n");
}

int main(void) {
    print_str("=== Emergency Management Demo ===\n\n");
    em_init();

    print_str("Creating emergency plans...\n");
    for (int i = 0; i < 16; i++) {
        int level = (i % 4) + 1;
        int cat = (i % 6) + 1;
        int region = (i % 8) + 1;
        int scenarios = 5 + (i % 10);
        int resources = 100 + (i * 50);
        int year = 2019 + (i % 5);
        em_plan(level, cat, region, scenarios, resources, year);
    }

    print_str("\nEmergency responses...\n");
    for (int i = 0; i < 14; i++) {
        int incident = 100 + (i * 7);
        int level = (i % 4) + 1;
        int units = 2 + (i % 5);
        int personnel = 20 + (i * 10);
        int duration = 4 + (i * 3);
        int casualties = i % 5;
        int year = 2021 + (i % 4);
        em_respond(incident, level, units, personnel, duration, casualties, year);
    }

    print_str("\nCommand operations...\n");
    for (int i = 0; i < 12; i++) {
        int incident = 200 + (i * 5);
        int commander = 500 + (i % 8);
        int decisions = 3 + (i % 6);
        int dispatched = 10 + (i * 5);
        int meetings = 2 + (i % 4);
        int year = 2022 + (i % 3);
        em_command(incident, commander, decisions, dispatched, meetings, year);
    }

    print_str("\nRescue forces...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int unit = 600 + (i * 10);
        int personnel = 30 + (i * 15);
        int equipment = 10 + (i * 5);
        int training = 100 + (i * 50);
        int readiness = 70 + (i * 3);
        int year = 2023 + (i % 2);
        em_add_force(type, unit, personnel, equipment, training, readiness, year);
    }

    print_str("\nLogistics stockpiles...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 5) + 1;
        int wh = 700 + (i * 7);
        int qty = 500 + (i * 200);
        int value = 50000 + (i * 30000);
        int dist = 100 + (i * 50);
        int year = 2024;
        em_add_logistics(cat, wh, qty, value, dist, year);
    }

    print_str("\nPlan report...\n");
    em_plan_report();

    print_str("\nForce report...\n");
    em_force_report();

    print_str("\nLogistics report...\n");
    em_logistics_report();

    print_str("\nFinal state...\n");
    em_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
