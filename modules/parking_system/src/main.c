/* parking_system: Parking management system (v1.0)
 * Spaces, vehicles, billing, access control, analytics
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

#define MAX_SPACES     20
#define MAX_VEHICLES   14
#define MAX_ENTRIES    18
#define MAX_MEMBERS    10
#define MAX_VIOLATIONS 8

typedef struct {
    int    space_id;
    int    zone;
    int    type;
    int    status;
    int    vehicle_id;
    int    entry_hour;
    int    n_uses;
    double revenue;
    int    active;
} space_t;

typedef struct {
    int    vehicle_id;
    int    plate_hash;
    int    type;
    int    owner_id;
    int    is_member;
    int    n_entries;
    double total_paid;
    int    active;
} vehicle_t;

typedef struct {
    int    entry_id;
    int    vehicle_id;
    int    space_id;
    int    entry_hour;
    int    exit_hour;
    double fee;
    int    paid;
    int    active;
} entry_t;

typedef struct {
    int    member_id;
    int    type;
    int    vehicle_id;
    int    start_day;
    int    end_day;
    double monthly_fee;
    int    active;
} member_t;

typedef struct {
    int    violation_id;
    int    vehicle_id;
    int    space_id;
    int    type;
    double fine;
    int    day;
    int    paid;
    int    active;
} violation_t;

typedef struct {
    int    n_spaces;
    int    n_vehicles;
    int    n_entries;
    int    n_members;
    int    n_violations;
    double total_revenue;
    double hourly_revenue;
    double monthly_revenue;
    double fine_revenue;
    int    total_parking_events;
    int    occupied_spaces;
} ps_state_t;

static space_t spaces[MAX_SPACES];
static vehicle_t vehicles[MAX_VEHICLES];
static entry_t entries[MAX_ENTRIES];
static member_t members[MAX_MEMBERS];
static violation_t violations[MAX_VIOLATIONS];
static ps_state_t ps;

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

int ps_init(void) {
    if (initialized) return -1;
    ps.n_spaces = 0; ps.n_vehicles = 0; ps.n_entries = 0;
    ps.n_members = 0; ps.n_violations = 0;
    ps.total_revenue = 0.0; ps.hourly_revenue = 0.0;
    ps.monthly_revenue = 0.0; ps.fine_revenue = 0.0;
    ps.total_parking_events = 0; ps.occupied_spaces = 0;
    for (int i = 0; i < MAX_SPACES; i++) spaces[i].active = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_ENTRIES; i++) entries[i].active = 0;
    for (int i = 0; i < MAX_MEMBERS; i++) members[i].active = 0;
    for (int i = 0; i < MAX_VIOLATIONS; i++) violations[i].active = 0;
    initialized = 1;
    print_str("[PS] Parking system initialized\n");
    return 0;
}

int ps_add_space(int zone, int type) {
    if (ps.n_spaces >= MAX_SPACES) return -1;
    space_t* s = &spaces[ps.n_spaces];
    s->space_id = ps.n_spaces;
    s->zone = zone;
    s->type = type;
    s->status = 0;
    s->vehicle_id = -1;
    s->entry_hour = 0;
    s->n_uses = 0;
    s->revenue = 0.0;
    s->active = 1;
    ps.n_spaces++;
    print_str("[PS] Space "); print_int(ps.n_spaces - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type); print_str("\n");
    return ps.n_spaces - 1;
}

int ps_register_vehicle(int plate_hash, int type, int is_member) {
    if (ps.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[ps.n_vehicles];
    v->vehicle_id = ps.n_vehicles;
    v->plate_hash = plate_hash;
    v->type = type;
    v->owner_id = -1;
    v->is_member = is_member;
    v->n_entries = 0;
    v->total_paid = 0.0;
    v->active = 1;
    ps.n_vehicles++;
    print_str("[PS] Vehicle "); print_int(ps.n_vehicles - 1);
    print_str(" plate="); print_int(plate_hash);
    print_str(" type="); print_int(type);
    if (is_member) print_str(" [MEMBER]");
    print_str("\n");
    return ps.n_vehicles - 1;
}

int ps_add_member(int type, int vehicle_id, int start_day, int duration_days) {
    if (ps.n_members >= MAX_MEMBERS || vehicle_id >= ps.n_vehicles) return -1;
    member_t* m = &members[ps.n_members];
    m->member_id = ps.n_members;
    m->type = type;
    m->vehicle_id = vehicle_id;
    m->start_day = start_day;
    m->end_day = start_day + duration_days;
    double fees[3] = {150.0, 250.0, 400.0};
    m->monthly_fee = fees[type < 3 ? type : 0];
    m->active = 1;
    vehicles[vehicle_id].is_member = 1;
    vehicles[vehicle_id].owner_id = ps.n_members;
    ps.n_members++;
    ps.monthly_revenue += m->monthly_fee * (duration_days / 30);
    ps.total_revenue += m->monthly_fee * (duration_days / 30);
    print_str("[PS] Member "); print_int(ps.n_members - 1);
    print_str(" type="); print_int(type);
    print_str(" V"); print_int(vehicle_id);
    print_str(" day "); print_int(start_day); print_str("-"); print_int(m->end_day);
    print_str(" fee="); print_int((int)m->monthly_fee); print_str("/mo\n");
    return ps.n_members - 1;
}

int ps_enter(int vehicle_id, int space_id, int hour) {
    if (ps.n_entries >= MAX_ENTRIES) return -1;
    if (vehicle_id >= ps.n_vehicles || space_id >= ps.n_spaces) return -2;
    space_t* s = &spaces[space_id];
    if (s->status != 0) return -3;
    entry_t* e = &entries[ps.n_entries];
    e->entry_id = ps.n_entries;
    e->vehicle_id = vehicle_id;
    e->space_id = space_id;
    e->entry_hour = hour;
    e->exit_hour = 0;
    e->fee = 0.0;
    e->paid = 0;
    e->active = 1;
    s->status = 1;
    s->vehicle_id = vehicle_id;
    s->entry_hour = hour;
    s->n_uses++;
    vehicles[vehicle_id].n_entries++;
    ps.n_entries++;
    ps.occupied_spaces++;
    ps.total_parking_events++;
    print_str("[PS] Entry "); print_int(ps.n_entries - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" S"); print_int(space_id);
    print_str(" hr="); print_int(hour); print_str("\n");
    return ps.n_entries - 1;
}

int ps_exit(int entry_id, int exit_hour) {
    if (entry_id >= ps.n_entries) return -1;
    entry_t* e = &entries[entry_id];
    if (e->exit_hour != 0) return -2;
    e->exit_hour = exit_hour;
    int duration = exit_hour - e->entry_hour;
    if (duration < 0) duration += 24;
    vehicle_t* v = &vehicles[e->vehicle_id];
    if (v->is_member) {
        e->fee = 0.0;
    } else {
        double rates[3] = {2.0, 3.0, 5.0};
        int rate_type = v->type < 3 ? v->type : 0;
        e->fee = duration * rates[rate_type];
        if (duration <= 1) e->fee = rates[rate_type];
    }
    e->paid = 1;
    space_t* s = &spaces[e->space_id];
    s->status = 0;
    s->vehicle_id = -1;
    s->revenue += e->fee;
    v->total_paid += e->fee;
    ps.hourly_revenue += e->fee;
    ps.total_revenue += e->fee;
    ps.occupied_spaces--;
    print_str("[PS] Exit E"); print_int(entry_id);
    print_str(" V"); print_int(e->vehicle_id);
    print_str(" dur="); print_int(duration); print_str("hr");
    print_str(" fee="); print_int((int)e->fee); print_str("\n");
    return 0;
}

int ps_issue_violation(int vehicle_id, int space_id, int type, int day) {
    if (ps.n_violations >= MAX_VIOLATIONS) return -1;
    if (vehicle_id >= ps.n_vehicles || space_id >= ps.n_spaces) return -2;
    violation_t* v = &violations[ps.n_violations];
    v->violation_id = ps.n_violations;
    v->vehicle_id = vehicle_id;
    v->space_id = space_id;
    v->type = type;
    double fines[3] = {50.0, 100.0, 200.0};
    v->fine = fines[type < 3 ? type : 0];
    v->day = day;
    v->paid = 0;
    v->active = 1;
    ps.n_violations++;
    print_str("[PS] Violation "); print_int(ps.n_violations - 1);
    print_str(" V"); print_int(vehicle_id);
    print_str(" S"); print_int(space_id);
    print_str(" type="); print_int(type);
    print_str(" fine="); print_int((int)v->fine);
    print_str(" day="); print_int(day); print_str("\n");
    return ps.n_violations - 1;
}

void pay_violation(int violation_id) {
    if (violation_id >= ps.n_violations) return;
    violations[violation_id].paid = 1;
    ps.fine_revenue += violations[violation_id].fine;
    ps.total_revenue += violations[violation_id].fine;
    print_str("[PS] Violation "); print_int(violation_id); print_str(" paid\n");
}

void ps_utilization_report(void) {
    print_str("[PS] Utilization report:\n");
    int total = ps.n_spaces;
    int occupied = ps.occupied_spaces;
    double util = (total > 0) ? (double)occupied / total * 100.0 : 0.0;
    print_str("  Total spaces: "); print_int(total); print_str("\n");
    print_str("  Occupied: "); print_int(occupied); print_str("\n");
    print_str("  Available: "); print_int(total - occupied); print_str("\n");
    print_str("  Utilization: "); print_int((int)util); print_str("%\n");
    print_str("  Total events: "); print_int(ps.total_parking_events); print_str("\n");
}

void ps_zone_report(void) {
    print_str("[PS] Zone report:\n");
    for (int i = 0; i < ps.n_spaces; i++) {
        space_t* s = &spaces[i];
        print_str("  S"); print_int(i);
        print_str(" zone="); print_int(s->zone);
        print_str(" type="); print_int(s->type);
        print_str(" uses="); print_int(s->n_uses);
        print_str(" rev="); print_int((int)s->revenue); print_str("\n");
    }
}

void ps_financial_report(void) {
    print_str("[PS] Financial report:\n");
    print_str("  Hourly revenue: "); print_int((int)ps.hourly_revenue); print_str("\n");
    print_str("  Monthly revenue: "); print_int((int)ps.monthly_revenue); print_str("\n");
    print_str("  Fine revenue: "); print_int((int)ps.fine_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)ps.total_revenue); print_str("\n");
    print_str("  Members: "); print_int(ps.n_members); print_str("\n");
    print_str("  Violations: "); print_int(ps.n_violations); print_str("\n");
}

void ps_print_state(void) {
    print_str("[PS] Spaces="); print_int(ps.n_spaces);
    print_str(" Vehicles="); print_int(ps.n_vehicles);
    print_str(" Entries="); print_int(ps.n_entries);
    print_str(" Members="); print_int(ps.n_members);
    print_str(" Violations="); print_int(ps.n_violations);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)ps.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Parking System Demo ===\n\n");
    ps_init();

    print_str("Adding spaces...\n");
    for (int z = 0; z < 4; z++) {
        for (int i = 0; i < 5; i++) {
            ps_add_space(z + 1, (i < 2) ? 1 : (i < 4) ? 2 : 3);
        }
    }

    print_str("\nRegistering vehicles...\n");
    ps_register_vehicle(1001, 1, 0);
    ps_register_vehicle(1002, 1, 0);
    ps_register_vehicle(1003, 2, 0);
    ps_register_vehicle(1004, 2, 0);
    ps_register_vehicle(1005, 3, 0);
    ps_register_vehicle(1006, 1, 0);
    ps_register_vehicle(1007, 2, 0);
    ps_register_vehicle(1008, 3, 0);
    ps_register_vehicle(1009, 1, 0);
    ps_register_vehicle(1010, 2, 0);
    ps_register_vehicle(1011, 1, 0);
    ps_register_vehicle(1012, 3, 0);

    print_str("\nAdding members...\n");
    ps_add_member(0, 0, 1, 90);
    ps_add_member(1, 2, 1, 60);
    ps_add_member(2, 4, 1, 120);
    ps_add_member(1, 6, 1, 60);
    ps_add_member(0, 8, 1, 90);

    print_str("\nVehicle entries...\n");
    ps_enter(0, 0, 8);
    ps_enter(1, 1, 8);
    ps_enter(2, 5, 9);
    ps_enter(3, 6, 9);
    ps_enter(4, 10, 10);
    ps_enter(5, 11, 10);
    ps_enter(6, 15, 11);
    ps_enter(7, 16, 11);
    ps_enter(8, 2, 12);
    ps_enter(9, 7, 12);
    ps_enter(10, 12, 13);
    ps_enter(11, 17, 13);
    ps_enter(0, 3, 14);
    ps_enter(1, 8, 14);
    ps_enter(2, 13, 15);
    ps_enter(3, 18, 15);
    ps_enter(4, 4, 16);
    ps_enter(5, 9, 16);

    print_str("\nVehicle exits...\n");
    ps_exit(0, 17);
    ps_exit(1, 17);
    ps_exit(2, 18);
    ps_exit(3, 18);
    ps_exit(4, 19);
    ps_exit(5, 19);
    ps_exit(6, 20);
    ps_exit(7, 20);
    ps_exit(8, 21);
    ps_exit(9, 21);
    ps_exit(10, 22);
    ps_exit(11, 22);

    print_str("\nIssuing violations...\n");
    ps_issue_violation(1, 14, 0, 12);
    ps_issue_violation(3, 19, 1, 14);
    ps_issue_violation(5, 0, 0, 15);
    ps_issue_violation(7, 5, 2, 16);

    print_str("\nPaying violations...\n");
    pay_violation(0);
    pay_violation(1);
    pay_violation(2);

    print_str("\nUtilization report...\n");
    ps_utilization_report();

    print_str("\nZone report...\n");
    ps_zone_report();

    print_str("\nFinancial report...\n");
    ps_financial_report();

    print_str("\nFinal state...\n");
    ps_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
