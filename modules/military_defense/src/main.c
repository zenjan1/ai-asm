/* military_defense: Military defense management (v1.0)
 * Bases, units, personnel, equipment, operations
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

#define MAX_BASES      10
#define MAX_UNITS      14
#define MAX_PERSONNEL  18
#define MAX_EQUIPMENT  16
#define MAX_OPERATIONS 12

typedef struct {
    int    base_id;
    int    zone;
    int    type;
    int    capacity;
    int    personnel;
    int    runway;
    int    port;
    int    year_built;
    int    active;
} base_t;

typedef struct {
    int    unit_id;
    int    base_id;
    int    branch;
    int    size;
    int    readiness;
    int    commander_id;
    int    year_formed;
    int    active;
} unit_t;

typedef struct {
    int    personnel_id;
    int    unit_id;
    int    rank;
    int    years_service;
    int    specialty;
    int    fitness;
    int    clearance;
    int    active;
} personnel_t;

typedef struct {
    int    equip_id;
    int    type;
    int    unit_id;
    int    quantity;
    int    condition;
    int    maintenance_due;
    int    year_acquired;
    int    active;
} equipment_t;

typedef struct {
    int    op_id;
    int    type;
    int    zone;
    int    units_deployed;
    int    personnel_deployed;
    int    duration_days;
    int    status;
    int    casualties;
    int    year;
    int    active;
} operation_t;

typedef struct {
    int    n_bases;
    int    n_units;
    int    n_personnel;
    int    n_equipment;
    int    n_operations;
    int    total_personnel;
    int    total_equipment;
    int    total_operations;
    int    total_casualties;
    int    total_readiness;
} md_state_t;

static base_t bases[MAX_BASES];
static unit_t units[MAX_UNITS];
static personnel_t personnel[MAX_PERSONNEL];
static equipment_t equipment[MAX_EQUIPMENT];
static operation_t operations[MAX_OPERATIONS];
static md_state_t md;

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

int md_init(void) {
    if (initialized) return -1;
    md.n_bases = 0; md.n_units = 0; md.n_personnel = 0;
    md.n_equipment = 0; md.n_operations = 0;
    md.total_personnel = 0; md.total_equipment = 0;
    md.total_operations = 0; md.total_casualties = 0;
    md.total_readiness = 0;
    for (int i = 0; i < MAX_BASES; i++) bases[i].active = 0;
    for (int i = 0; i < MAX_UNITS; i++) units[i].active = 0;
    for (int i = 0; i < MAX_PERSONNEL; i++) personnel[i].active = 0;
    for (int i = 0; i < MAX_EQUIPMENT; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_OPERATIONS; i++) operations[i].active = 0;
    initialized = 1;
    print_str("[MD] Military defense initialized\n");
    return 0;
}

int md_add_base(int zone, int type, int capacity, int pers, int runway, int port, int year) {
    if (md.n_bases >= MAX_BASES) return -1;
    base_t* b = &bases[md.n_bases];
    b->base_id = md.n_bases;
    b->zone = zone;
    b->type = type;
    b->capacity = capacity;
    b->personnel = pers;
    b->runway = runway;
    b->port = port;
    b->year_built = year;
    b->active = 1;
    md.total_personnel += pers;
    md.n_bases++;
    print_str("[MD] Base "); print_int(md.n_bases - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str(" pers="); print_int(pers);
    print_str(" R"); print_int(runway); print_str(" P"); print_int(port); print_str("\n");
    return md.n_bases - 1;
}

int md_add_unit(int base_id, int branch, int size, int readiness, int year) {
    if (md.n_units >= MAX_UNITS) return -1;
    unit_t* u = &units[md.n_units];
    u->unit_id = md.n_units;
    u->base_id = base_id;
    u->branch = branch;
    u->size = size;
    u->readiness = readiness;
    u->commander_id = -1;
    u->year_formed = year;
    u->active = 1;
    md.total_readiness += readiness;
    md.n_units++;
    print_str("[MD] Unit "); print_int(md.n_units - 1);
    print_str(" base="); print_int(base_id);
    print_str(" br="); print_int(branch);
    print_str(" sz="); print_int(size);
    print_str(" rdy="); print_int(readiness); print_str("%\n");
    return md.n_units - 1;
}

int md_assign_commander(int unit_id, int personnel_id) {
    if (unit_id >= md.n_units) return -1;
    units[unit_id].commander_id = personnel_id;
    print_str("[MD] Cmdr U"); print_int(unit_id);
    print_str(" -> P"); print_int(personnel_id); print_str("\n");
    return 0;
}

int md_add_personnel(int unit_id, int rank, int years, int specialty, int fitness, int clearance) {
    if (md.n_personnel >= MAX_PERSONNEL) return -1;
    personnel_t* p = &personnel[md.n_personnel];
    p->personnel_id = md.n_personnel;
    p->unit_id = unit_id;
    p->rank = rank;
    p->years_service = years;
    p->specialty = specialty;
    p->fitness = fitness;
    p->clearance = clearance;
    p->active = 1;
    md.total_personnel++;
    md.n_personnel++;
    print_str("[MD] Pers "); print_int(md.n_personnel - 1);
    print_str(" unit="); print_int(unit_id);
    print_str(" rank="); print_int(rank);
    print_str(" yrs="); print_int(years);
    print_str(" spec="); print_int(specialty); print_str("\n");
    return md.n_personnel - 1;
}

int md_add_equipment(int type, int unit_id, int quantity, int condition, int maint_due, int year) {
    if (md.n_equipment >= MAX_EQUIPMENT) return -1;
    equipment_t* e = &equipment[md.n_equipment];
    e->equip_id = md.n_equipment;
    e->type = type;
    e->unit_id = unit_id;
    e->quantity = quantity;
    e->condition = condition;
    e->maintenance_due = maint_due;
    e->year_acquired = year;
    e->active = 1;
    md.total_equipment += quantity;
    md.n_equipment++;
    print_str("[MD] Equip "); print_int(md.n_equipment - 1);
    print_str(" type="); print_int(type);
    print_str(" unit="); print_int(unit_id);
    print_str(" qty="); print_int(quantity);
    print_str(" cond="); print_int(condition); print_str("%\n");
    return md.n_equipment - 1;
}

int md_schedule_maintenance(int equip_id) {
    if (equip_id >= md.n_equipment) return -1;
    equipment[equip_id].condition = 100;
    equipment[equip_id].maintenance_due = 0;
    print_str("[MD] Maint E"); print_int(equip_id); print_str("\n");
    return 0;
}

int md_launch_operation(int type, int zone, int units_dep, int pers_dep, int duration, int year) {
    if (md.n_operations >= MAX_OPERATIONS) return -1;
    operation_t* o = &operations[md.n_operations];
    o->op_id = md.n_operations;
    o->type = type;
    o->zone = zone;
    o->units_deployed = units_dep;
    o->personnel_deployed = pers_dep;
    o->duration_days = duration;
    o->status = 1;
    o->casualties = 0;
    o->year = year;
    o->active = 1;
    md.total_operations++;
    md.n_operations++;
    print_str("[MD] Op "); print_int(md.n_operations - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" units="); print_int(units_dep);
    print_str(" pers="); print_int(pers_dep);
    print_str(" days="); print_int(duration); print_str("\n");
    return md.n_operations - 1;
}

int md_report_casualties(int op_id, int casualties) {
    if (op_id >= md.n_operations) return -1;
    operations[op_id].casualties = casualties;
    md.total_casualties += casualties;
    print_str("[MD] Casualties Op"); print_int(op_id);
    print_str(" ="); print_int(casualties); print_str("\n");
    return 0;
}

int md_complete_operation(int op_id) {
    if (op_id >= md.n_operations) return -1;
    operations[op_id].status = 2;
    print_str("[MD] Complete Op"); print_int(op_id); print_str("\n");
    return 0;
}

void md_base_report(void) {
    print_str("[MD] Base report:\n");
    print_str("  Bases: "); print_int(md.n_bases); print_str("\n");
    print_str("  Total personnel: "); print_int(md.total_personnel); print_str("\n");
    for (int i = 0; i < md.n_bases; i++) {
        base_t* b = &bases[i];
        print_str("  B"); print_int(i);
        print_str(" Z"); print_int(b->zone);
        print_str(" T"); print_int(b->type);
        print_str(" cap="); print_int(b->capacity); print_str("\n");
    }
}

void md_unit_report(void) {
    print_str("[MD] Unit report:\n");
    print_str("  Units: "); print_int(md.n_units); print_str("\n");
    print_str("  Avg readiness: ");
    int avg = md.n_units > 0 ? md.total_readiness / md.n_units : 0;
    print_int(avg); print_str("%\n");
}

void md_operation_report(void) {
    print_str("[MD] Operation report:\n");
    print_str("  Total ops: "); print_int(md.total_operations); print_str("\n");
    print_str("  Casualties: "); print_int(md.total_casualties); print_str("\n");
    print_str("  Equipment: "); print_int(md.total_equipment); print_str("\n");
}

void md_print_state(void) {
    print_str("[MD] Bas="); print_int(md.n_bases);
    print_str(" Unt="); print_int(md.n_units);
    print_str(" Per="); print_int(md.n_personnel);
    print_str(" Eqp="); print_int(md.n_equipment);
    print_str(" Ops="); print_int(md.n_operations);
    print_str("\n");
}

int main(void) {
    print_str("=== Military Defense Demo ===\n\n");
    md_init();

    print_str("Adding bases...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int type = (i % 3) + 1;
        int cap = 500 + (i * 200);
        int pers = 300 + (i * 100);
        int rwy = (i % 3 == 0) ? 1 : 0;
        int port = (i % 4 == 0) ? 1 : 0;
        int year = 1990 + (i * 3);
        md_add_base(zone, type, cap, pers, rwy, port, year);
    }

    print_str("\nAdding units...\n");
    for (int i = 0; i < 14; i++) {
        int base = i % 10;
        int branch = (i % 3) + 1;
        int size = 100 + (i * 50);
        int rdy = 70 + (i % 30);
        int year = 2000 + (i % 10);
        md_add_unit(base, branch, size, rdy, year);
    }

    print_str("\nAssigning commanders...\n");
    for (int i = 0; i < 14; i++) {
        md_assign_commander(i, i % 18);
    }

    print_str("\nAdding personnel...\n");
    for (int i = 0; i < 18; i++) {
        int unit = i % 14;
        int rank = (i % 8) + 1;
        int yrs = 2 + (i % 20);
        int spec = (i % 6) + 1;
        int fit = 80 + (i % 20);
        int clr = (i % 4) + 1;
        md_add_personnel(unit, rank, yrs, spec, fit, clr);
    }

    print_str("\nAdding equipment...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int unit = i % 14;
        int qty = 5 + (i * 3);
        int cond = 60 + (i % 40);
        int maint = (i % 6 == 0) ? 1 : 0;
        int year = 2010 + (i % 10);
        md_add_equipment(type, unit, qty, cond, maint, year);
    }

    print_str("\nScheduling maintenance...\n");
    for (int i = 0; i < 5; i++) {
        md_schedule_maintenance(i * 3);
    }

    print_str("\nLaunching operations...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int units_dep = 2 + (i % 5);
        int pers_dep = 200 + (i * 50);
        int dur = 7 + (i * 3);
        int year = 2022 + (i % 3);
        md_launch_operation(type, zone, units_dep, pers_dep, dur, year);
    }

    print_str("\nReporting casualties...\n");
    for (int i = 0; i < 12; i++) {
        int cas = i % 5;
        md_report_casualties(i, cas);
    }

    print_str("\nCompleting operations...\n");
    for (int i = 0; i < 12; i++) {
        md_complete_operation(i);
    }

    print_str("\nBase report...\n");
    md_base_report();

    print_str("\nUnit report...\n");
    md_unit_report();

    print_str("\nOperation report...\n");
    md_operation_report();

    print_str("\nFinal state...\n");
    md_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
