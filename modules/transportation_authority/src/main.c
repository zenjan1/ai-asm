/* transportation_authority: Transport management (v1.0)
 * Driver licenses, vehicle registration, violations, permits
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

#define MAX_LICENSES   20
#define MAX_VEHICLES   16
#define MAX_VIOLATIONS 18
#define MAX_PERMITS    10
#define MAX_INSPECTS   12

typedef struct {
    int    lic_id;
    int    person_id;
    int    class;
    int    points;
    int    issued_year;
    int    expiry_year;
    int    status;
    int    active;
} license_t;

typedef struct {
    int    veh_id;
    int    owner_id;
    int    type;
    int    year;
    int    plate_num;
    int    inspection_year;
    int    status;
    int    active;
} vehicle_t;

typedef struct {
    int    viol_id;
    int    veh_id;
    int    driver_id;
    int    type;
    int    points;
    int    fine;
    int    year;
    int    paid;
    int    active;
} violation_t;

typedef struct {
    int    permit_id;
    int    company_id;
    int    type;
    int    route_id;
    int    n_vehicles;
    int    issued_year;
    int    expiry_year;
    int    status;
    int    active;
} permit_t;

typedef struct {
    int    insp_id;
    int    veh_id;
    int    result;
    int    year;
    int    cost;
    int    next_year;
    int    active;
} inspection_t;

typedef struct {
    int    n_licenses;
    int    n_vehicles;
    int    n_violations;
    int    n_permits;
    int    n_inspections;
    int    total_fines;
    int    total_points;
    int    class_a;
    int    class_b;
    int    class_c;
    int    total_registered;
    int    total_active_permits;
} ta_state_t;

static license_t licenses[MAX_LICENSES];
static vehicle_t vehicles[MAX_VEHICLES];
static violation_t violations[MAX_VIOLATIONS];
static permit_t permits[MAX_PERMITS];
static inspection_t inspections[MAX_INSPECTS];
static ta_state_t ta;

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

int ta_init(void) {
    if (initialized) return -1;
    ta.n_licenses = 0; ta.n_vehicles = 0; ta.n_violations = 0;
    ta.n_permits = 0; ta.n_inspections = 0;
    ta.total_fines = 0; ta.total_points = 0;
    ta.class_a = 0; ta.class_b = 0; ta.class_c = 0;
    ta.total_registered = 0; ta.total_active_permits = 0;
    for (int i = 0; i < MAX_LICENSES; i++) licenses[i].active = 0;
    for (int i = 0; i < MAX_VEHICLES; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_VIOLATIONS; i++) violations[i].active = 0;
    for (int i = 0; i < MAX_PERMITS; i++) permits[i].active = 0;
    for (int i = 0; i < MAX_INSPECTS; i++) inspections[i].active = 0;
    initialized = 1;
    print_str("[TA] Transportation authority initialized\n");
    return 0;
}

int ta_issue_license(int person, int class, int year) {
    if (ta.n_licenses >= MAX_LICENSES) return -1;
    license_t* l = &licenses[ta.n_licenses];
    l->lic_id = ta.n_licenses;
    l->person_id = person;
    l->class = class;
    l->points = 12;
    l->issued_year = year;
    l->expiry_year = year + 6;
    l->status = 1;
    l->active = 1;
    if (class == 1) ta.class_a++;
    else if (class == 2) ta.class_b++;
    else ta.class_c++;
    ta.n_licenses++;
    print_str("[TA] License "); print_int(ta.n_licenses - 1);
    print_str(" P"); print_int(person);
    print_str(" class="); print_int(class);
    print_str(" pts="); print_int(l->points);
    print_str(" "); print_int(year); print_str("-"); print_int(l->expiry_year); print_str("\n");
    return ta.n_licenses - 1;
}

int ta_deduct_points(int lic_id, int points) {
    if (lic_id >= ta.n_licenses) return -1;
    licenses[lic_id].points -= points;
    ta.total_points += points;
    print_str("[TA] Deduct L"); print_int(lic_id);
    print_str(" -"); print_int(points); print_str("pts");
    print_str(" remain="); print_int(licenses[lic_id].points); print_str("\n");
    return 0;
}

int ta_renew_license(int lic_id, int new_expiry) {
    if (lic_id >= ta.n_licenses) return -1;
    licenses[lic_id].expiry_year = new_expiry;
    licenses[lic_id].points = 12;
    print_str("[TA] Renew L"); print_int(lic_id);
    print_str(" exp="); print_int(new_expiry); print_str("\n");
    return 0;
}

int ta_register_vehicle(int owner, int type, int year, int plate) {
    if (ta.n_vehicles >= MAX_VEHICLES) return -1;
    vehicle_t* v = &vehicles[ta.n_vehicles];
    v->veh_id = ta.n_vehicles;
    v->owner_id = owner;
    v->type = type;
    v->year = year;
    v->plate_num = plate;
    v->inspection_year = year;
    v->status = 1;
    v->active = 1;
    ta.total_registered++;
    ta.n_vehicles++;
    print_str("[TA] Vehicle "); print_int(ta.n_vehicles - 1);
    print_str(" P"); print_int(owner);
    print_str(" type="); print_int(type);
    print_str(" yr="); print_int(year);
    print_str(" plate="); print_int(plate); print_str("\n");
    return ta.n_vehicles - 1;
}

int ta_transfer_vehicle(int veh_id, int new_owner) {
    if (veh_id >= ta.n_vehicles) return -1;
    int old = vehicles[veh_id].owner_id;
    vehicles[veh_id].owner_id = new_owner;
    print_str("[TA] Transfer V"); print_int(veh_id);
    print_str(" P"); print_int(old);
    print_str("->P"); print_int(new_owner); print_str("\n");
    return 0;
}

int ta_record_violation(int veh, int driver, int type, int points, int fine, int year) {
    if (ta.n_violations >= MAX_VIOLATIONS || veh >= ta.n_vehicles) return -1;
    violation_t* v = &violations[ta.n_violations];
    v->viol_id = ta.n_violations;
    v->veh_id = veh;
    v->driver_id = driver;
    v->type = type;
    v->points = points;
    v->fine = fine;
    v->year = year;
    v->paid = 0;
    v->active = 1;
    ta.total_fines += fine;
    ta.n_violations++;
    print_str("[TA] Viol "); print_int(ta.n_violations - 1);
    print_str(" V"); print_int(veh);
    print_str(" D"); print_int(driver);
    print_str(" type="); print_int(type);
    print_str(" pts="); print_int(points);
    print_str(" $"); print_int(fine); print_str("\n");
    return ta.n_violations - 1;
}

int ta_pay_fine(int viol_id) {
    if (viol_id >= ta.n_violations) return -1;
    violations[viol_id].paid = 1;
    print_str("[TA] Pay Vl"); print_int(viol_id); print_str("\n");
    return 0;
}

int ta_issue_permit(int company, int type, int route, int n_veh, int year) {
    if (ta.n_permits >= MAX_PERMITS) return -1;
    permit_t* p = &permits[ta.n_permits];
    p->permit_id = ta.n_permits;
    p->company_id = company;
    p->type = type;
    p->route_id = route;
    p->n_vehicles = n_veh;
    p->issued_year = year;
    p->expiry_year = year + 2;
    p->status = 1;
    p->active = 1;
    ta.total_active_permits++;
    ta.n_permits++;
    print_str("[TA] Permit "); print_int(ta.n_permits - 1);
    print_str(" Co"); print_int(company);
    print_str(" type="); print_int(type);
    print_str(" route="); print_int(route);
    print_str(" veh="); print_int(n_veh);
    print_str(" "); print_int(year); print_str("-"); print_int(p->expiry_year); print_str("\n");
    return ta.n_permits - 1;
}

int ta_inspect_vehicle(int veh_id, int result, int year, int cost) {
    if (ta.n_inspections >= MAX_INSPECTS || veh_id >= ta.n_vehicles) return -1;
    inspection_t* ins = &inspections[ta.n_inspections];
    ins->insp_id = ta.n_inspections;
    ins->veh_id = veh_id;
    ins->result = result;
    ins->year = year;
    ins->cost = cost;
    ins->next_year = year + 1;
    ins->active = 1;
    vehicles[veh_id].inspection_year = year;
    ta.n_inspections++;
    print_str("[TA] Insp "); print_int(ta.n_inspections - 1);
    print_str(" V"); print_int(veh_id);
    print_str(" result="); print_int(result);
    print_str(" "); print_int(year);
    print_str(" $"); print_int(cost); print_str("\n");
    return ta.n_inspections - 1;
}

void ta_license_report(void) {
    print_str("[TA] License report:\n");
    print_str("  Class A: "); print_int(ta.class_a);
    print_str(" B: "); print_int(ta.class_b);
    print_str(" C: "); print_int(ta.class_c); print_str("\n");
    for (int i = 0; i < ta.n_licenses; i++) {
        license_t* l = &licenses[i];
        print_str("  L"); print_int(i);
        print_str(" P"); print_int(l->person_id);
        print_str(" cl="); print_int(l->class);
        print_str(" pts="); print_int(l->points);
        print_str(" "); print_int(l->issued_year); print_str("-"); print_int(l->expiry_year); print_str("\n");
    }
}

void ta_vehicle_report(void) {
    print_str("[TA] Vehicle report:\n");
    print_str("  Registered: "); print_int(ta.total_registered); print_str("\n");
    for (int i = 0; i < ta.n_vehicles; i++) {
        vehicle_t* v = &vehicles[i];
        print_str("  V"); print_int(i);
        print_str(" P"); print_int(v->owner_id);
        print_str(" type="); print_int(v->type);
        print_str(" plate="); print_int(v->plate_num);
        print_str(" insp="); print_int(v->inspection_year); print_str("\n");
    }
}

void ta_violation_report(void) {
    print_str("[TA] Violation report:\n");
    print_str("  Total violations: "); print_int(ta.n_violations); print_str("\n");
    print_str("  Total fines: "); print_int(ta.total_fines); print_str("\n");
    print_str("  Total points: "); print_int(ta.total_points); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Lic="); print_int(ta.n_licenses);
    print_str(" Veh="); print_int(ta.n_vehicles);
    print_str(" Viol="); print_int(ta.n_violations);
    print_str(" Permits="); print_int(ta.n_permits);
    print_str(" Insp="); print_int(ta.n_inspections);
    print_str("\n");
    print_str("  Fines: "); print_int(ta.total_fines); print_str("\n");
}

int main(void) {
    print_str("=== Transportation Authority Demo ===\n\n");
    ta_init();

    print_str("Issuing licenses...\n");
    for (int i = 0; i < 20; i++) {
        int person = 100 + i;
        int class = (i % 3) + 1;
        int year = 2020 + (i % 6);
        ta_issue_license(person, class, year);
    }

    print_str("\nRegistering vehicles...\n");
    for (int i = 0; i < 16; i++) {
        int owner = 100 + (i % 20);
        int type = (i % 4) + 1;
        int year = 2018 + (i % 7);
        int plate = 1000 + i * 100;
        ta_register_vehicle(owner, type, year, plate);
    }

    print_str("\nRecording violations...\n");
    for (int i = 0; i < 18; i++) {
        int veh = i % 16;
        int driver = 100 + (i % 20);
        int type = (i % 5) + 1;
        int points = 1 + (i % 4);
        int fine = 200 + (i * 100);
        int year = 2024;
        ta_record_violation(veh, driver, type, points, fine, year);
    }

    print_str("\nDeducting points...\n");
    for (int i = 0; i < 12; i++) {
        int pts = 1 + (i % 3);
        ta_deduct_points(i, pts);
    }

    print_str("\nPaying fines...\n");
    for (int i = 0; i < 18; i++) {
        ta_pay_fine(i);
    }

    print_str("\nRenewing licenses...\n");
    for (int i = 0; i < 8; i++) {
        ta_renew_license(i, 2030 + i);
    }

    print_str("\nTransferring vehicles...\n");
    ta_transfer_vehicle(0, 200);
    ta_transfer_vehicle(3, 201);
    ta_transfer_vehicle(7, 202);
    ta_transfer_vehicle(11, 203);

    print_str("\nIssuing permits...\n");
    for (int i = 0; i < 10; i++) {
        int company = 500 + i;
        int type = (i % 3) + 1;
        int route = (i % 5) + 1;
        int n_veh = 3 + (i % 8);
        int year = 2024;
        ta_issue_permit(company, type, route, n_veh, year);
    }

    print_str("\nVehicle inspections...\n");
    for (int i = 0; i < 12; i++) {
        int veh = i % 16;
        int result = (i % 3);
        int year = 2024;
        int cost = 200 + (i * 50);
        ta_inspect_vehicle(veh, result, year, cost);
    }

    print_str("\nLicense report...\n");
    ta_license_report();

    print_str("\nVehicle report...\n");
    ta_vehicle_report();

    print_str("\nViolation report...\n");
    ta_violation_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
