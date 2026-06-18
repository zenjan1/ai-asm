/* port_authority: Port management (v1.0)
 * Berths, cargo, equipment, vessels, storage, revenue
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

#define MAX_BERTHS    10
#define MAX_CARGO     16
#define MAX_EQUIP     12
#define MAX_VESSELS   14
#define MAX_STORAGE   8

typedef struct {
    int    berth_id;
    int    type;
    int    length_m;
    int    depth_m;
    int    occupancy;
    int    vessel_id;
    double tariff_per_hr;
    double revenue;
    int    active;
} berth_t;

typedef struct {
    int    cargo_id;
    int    type;
    int    weight_tons;
    int    volume_cbm;
    double rate_per_ton;
    int    vessel_id;
    int    discharged;
    double revenue;
    int    active;
} cargo_t;

typedef struct {
    int    equip_id;
    int    type;
    int    capacity_tons;
    int    hours_used;
    double rate_per_hr;
    double maintenance_cost;
    int    operational;
    int    active;
} equipment_t;

typedef struct {
    int    vessel_id;
    int    type;
    int    dwt;
    int    loa;
    int    berth_id;
    int    arrival_day;
    int    departure_day;
    double port_dues;
    int    status;
    int    active;
} vessel_t;

typedef struct {
    int    stor_id;
    int    type;
    int    capacity_tons;
    int    used_tons;
    double rate_per_ton_day;
    int    days_stored;
    double revenue;
    int    active;
} storage_t;

typedef struct {
    int    n_berths;
    int    n_cargo;
    int    n_equip;
    int    n_vessels;
    int    n_storage;
    double berth_revenue;
    double cargo_revenue;
    double equipment_revenue;
    double storage_revenue;
    double total_revenue;
    double ops_costs;
    double labor_costs;
    int    total_vessels_served;
    int    total_cargo_tons;
} pa_state_t;

static berth_t berths[MAX_BERTHS];
static cargo_t cargos[MAX_CARGO];
static equipment_t equipment[MAX_EQUIP];
static vessel_t vessels[MAX_VESSELS];
static storage_t storage[MAX_STORAGE];
static pa_state_t pa;

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

int pa_init(void) {
    if (initialized) return -1;
    pa.n_berths = 0; pa.n_cargo = 0; pa.n_equip = 0;
    pa.n_vessels = 0; pa.n_storage = 0;
    pa.berth_revenue = 0.0; pa.cargo_revenue = 0.0;
    pa.equipment_revenue = 0.0; pa.storage_revenue = 0.0;
    pa.total_revenue = 0.0; pa.ops_costs = 0.0;
    pa.labor_costs = 0.0; pa.total_vessels_served = 0;
    pa.total_cargo_tons = 0;
    for (int i = 0; i < MAX_BERTHS; i++) berths[i].active = 0;
    for (int i = 0; i < MAX_CARGO; i++) cargos[i].active = 0;
    for (int i = 0; i < MAX_EQUIP; i++) equipment[i].active = 0;
    for (int i = 0; i < MAX_VESSELS; i++) vessels[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storage[i].active = 0;
    initialized = 1;
    print_str("[PA] Port authority initialized\n");
    return 0;
}

int pa_add_berth(int type, int length_m, int depth_m, double tariff) {
    if (pa.n_berths >= MAX_BERTHS) return -1;
    berth_t* b = &berths[pa.n_berths];
    b->berth_id = pa.n_berths;
    b->type = type;
    b->length_m = length_m;
    b->depth_m = depth_m;
    b->occupancy = 0;
    b->vessel_id = -1;
    b->tariff_per_hr = tariff;
    b->revenue = 0.0;
    b->active = 1;
    pa.labor_costs += 200.0;
    pa.n_berths++;
    print_str("[PA] Berth "); print_int(pa.n_berths - 1);
    print_str(" type="); print_int(type);
    print_str(" m="); print_int(length_m);
    print_str("x"); print_int(depth_m);
    print_str(" $"); print_int((int)tariff); print_str("/hr\n");
    return pa.n_berths - 1;
}

int pa_add_cargo(int type, int weight, int volume, double rate, int vessel_id) {
    if (pa.n_cargo >= MAX_CARGO) return -1;
    cargo_t* c = &cargos[pa.n_cargo];
    c->cargo_id = pa.n_cargo;
    c->type = type;
    c->weight_tons = weight;
    c->volume_cbm = volume;
    c->rate_per_ton = rate;
    c->vessel_id = vessel_id;
    c->discharged = 0;
    c->revenue = 0.0;
    c->active = 1;
    pa.n_cargo++;
    print_str("[PA] Cargo "); print_int(pa.n_cargo - 1);
    print_str(" type="); print_int(type);
    print_str(" tons="); print_int(weight);
    print_str(" V"); print_int(vessel_id);
    print_str(" $"); print_int((int)rate); print_str("/t\n");
    return pa.n_cargo - 1;
}

int pa_add_equipment(int type, int capacity, double rate) {
    if (pa.n_equip >= MAX_EQUIP) return -1;
    equipment_t* e = &equipment[pa.n_equip];
    e->equip_id = pa.n_equip;
    e->type = type;
    e->capacity_tons = capacity;
    e->hours_used = 0;
    e->rate_per_hr = rate;
    e->maintenance_cost = 0.0;
    e->operational = 1;
    e->active = 1;
    pa.ops_costs += 500.0;
    pa.n_equip++;
    print_str("[PA] Equip "); print_int(pa.n_equip - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str("t $"); print_int((int)rate); print_str("/hr\n");
    return pa.n_equip - 1;
}

int pa_add_vessel(int type, int dwt, int loa, int berth_id, int arrival) {
    if (pa.n_vessels >= MAX_VESSELS || berth_id >= pa.n_berths) return -1;
    vessel_t* v = &vessels[pa.n_vessels];
    v->vessel_id = pa.n_vessels;
    v->type = type;
    v->dwt = dwt;
    v->loa = loa;
    v->berth_id = berth_id;
    v->arrival_day = arrival;
    v->departure_day = 0;
    v->port_dues = 0.0;
    v->status = 1;
    v->active = 1;
    berths[berth_id].vessel_id = pa.n_vessels;
    berths[berth_id].occupancy = 1;
    pa.total_vessels_served++;
    pa.n_vessels++;
    print_str("[PA] Vessel "); print_int(pa.n_vessels - 1);
    print_str(" type="); print_int(type);
    print_str(" dwt="); print_int(dwt);
    print_str(" B"); print_int(berth_id);
    print_str(" day="); print_int(arrival); print_str("\n");
    return pa.n_vessels - 1;
}

int pa_add_storage(int type, int capacity, double rate) {
    if (pa.n_storage >= MAX_STORAGE) return -1;
    storage_t* s = &storage[pa.n_storage];
    s->stor_id = pa.n_storage;
    s->type = type;
    s->capacity_tons = capacity;
    s->used_tons = 0;
    s->rate_per_ton_day = rate;
    s->days_stored = 0;
    s->revenue = 0.0;
    s->active = 1;
    pa.n_storage++;
    print_str("[PA] Storage "); print_int(pa.n_storage - 1);
    print_str(" type="); print_int(type);
    print_str(" cap="); print_int(capacity);
    print_str("t $"); print_int((int)rate); print_str("/t/d\n");
    return pa.n_storage - 1;
}

int pa_discharge_cargo(int cargo_id, int tons) {
    if (cargo_id >= pa.n_cargo) return -1;
    cargo_t* c = &cargos[cargo_id];
    c->discharged += tons;
    double rev = tons * c->rate_per_ton;
    c->revenue += rev;
    pa.cargo_revenue += rev;
    pa.total_revenue += rev;
    pa.total_cargo_tons += tons;
    print_str("[PA] Discharge C"); print_int(cargo_id);
    print_str(" tons="); print_int(tons);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int pa_vessel_depart(int vessel_id, int day) {
    if (vessel_id >= pa.n_vessels) return -1;
    vessel_t* v = &vessels[vessel_id];
    v->departure_day = day;
    v->status = 2;
    int days = day - v->arrival_day;
    double dues = days * 2000.0 + v->dwt * 0.5;
    v->port_dues = dues;
    if (v->berth_id >= 0) {
        berth_t* b = &berths[v->berth_id];
        double berth_rev = days * b->tariff_per_hr * 24;
        b->revenue += berth_rev;
        pa.berth_revenue += berth_rev;
        pa.total_revenue += berth_rev;
        b->occupancy = 0;
        b->vessel_id = -1;
    }
    print_str("[PA] Depart V"); print_int(vessel_id);
    print_str(" days="); print_int(days);
    print_str(" dues="); print_int((int)dues); print_str("\n");
    return 0;
}

int pa_use_equipment(int equip_id, int hours) {
    if (equip_id >= pa.n_equip) return -1;
    equipment_t* e = &equipment[equip_id];
    e->hours_used += hours;
    double rev = hours * e->rate_per_hr;
    pa.equipment_revenue += rev;
    pa.total_revenue += rev;
    e->maintenance_cost += hours * 15.0;
    pa.ops_costs += hours * 15.0;
    print_str("[PA] Equip "); print_int(equip_id);
    print_str(" hrs="); print_int(hours);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

int pa_store_cargo(int stor_id, int tons, int days) {
    if (stor_id >= pa.n_storage) return -1;
    storage_t* s = &storage[stor_id];
    if (s->used_tons + tons > s->capacity_tons) return -2;
    s->used_tons += tons;
    s->days_stored += days;
    double rev = tons * days * s->rate_per_ton_day;
    s->revenue += rev;
    pa.storage_revenue += rev;
    pa.total_revenue += rev;
    print_str("[PA] Store St"); print_int(stor_id);
    print_str(" tons="); print_int(tons);
    print_str(" days="); print_int(days);
    print_str(" $"); print_int((int)rev); print_str("\n");
    return 0;
}

void pa_berth_report(void) {
    print_str("[PA] Berth report:\n");
    for (int i = 0; i < pa.n_berths; i++) {
        berth_t* b = &berths[i];
        print_str("  B"); print_int(i);
        print_str(" type="); print_int(b->type);
        print_str(" occ="); print_int(b->occupancy);
        print_str(" V"); print_int(b->vessel_id);
        print_str(" rev="); print_int((int)b->revenue); print_str("\n");
    }
}

void pa_financial_report(void) {
    print_str("[PA] Financial report:\n");
    print_str("  Berth revenue: "); print_int((int)pa.berth_revenue); print_str("\n");
    print_str("  Cargo revenue: "); print_int((int)pa.cargo_revenue); print_str("\n");
    print_str("  Equipment revenue: "); print_int((int)pa.equipment_revenue); print_str("\n");
    print_str("  Storage revenue: "); print_int((int)pa.storage_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)pa.total_revenue); print_str("\n");
    print_str("  Ops costs: "); print_int((int)pa.ops_costs); print_str("\n");
    print_str("  Labor costs: "); print_int((int)pa.labor_costs); print_str("\n");
    double profit = pa.total_revenue - pa.ops_costs - pa.labor_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Vessels served: "); print_int(pa.total_vessels_served);
    print_str(" Cargo: "); print_int(pa.total_cargo_tons); print_str("t\n");
}

void pa_print_state(void) {
    print_str("[PA] Berths="); print_int(pa.n_berths);
    print_str(" Cargo="); print_int(pa.n_cargo);
    print_str(" Equip="); print_int(pa.n_equip);
    print_str(" Vessels="); print_int(pa.n_vessels);
    print_str(" Storage="); print_int(pa.n_storage);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)pa.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Port Authority Demo ===\n\n");
    pa_init();

    print_str("Adding berths...\n");
    pa_add_berth(1, 400, 15, 500.0);
    pa_add_berth(1, 350, 14, 450.0);
    pa_add_berth(2, 250, 12, 350.0);
    pa_add_berth(2, 200, 10, 300.0);
    pa_add_berth(3, 300, 18, 600.0);
    pa_add_berth(1, 380, 15, 480.0);
    pa_add_berth(3, 280, 16, 550.0);
    pa_add_berth(2, 220, 11, 320.0);
    pa_add_berth(1, 360, 14, 460.0);
    pa_add_berth(3, 320, 20, 650.0);

    print_str("\nAdding cargo...\n");
    pa_add_cargo(1, 5000, 8000, 12.0, 0);
    pa_add_cargo(1, 4500, 7200, 11.5, 1);
    pa_add_cargo(2, 3000, 4000, 8.0, 2);
    pa_add_cargo(2, 2800, 3800, 7.5, 3);
    pa_add_cargo(3, 8000, 10000, 15.0, 4);
    pa_add_cargo(1, 4200, 6800, 11.0, 5);
    pa_add_cargo(3, 7500, 9500, 14.5, 6);
    pa_add_cargo(2, 2500, 3500, 7.0, 7);
    pa_add_cargo(1, 4800, 7800, 12.5, 8);
    pa_add_cargo(3, 9000, 11000, 16.0, 9);
    pa_add_cargo(1, 3800, 6200, 10.5, 0);
    pa_add_cargo(2, 2200, 3000, 6.5, 1);
    pa_add_cargo(3, 6500, 8500, 13.0, 2);
    pa_add_cargo(1, 4000, 6500, 10.0, 3);
    pa_add_cargo(2, 3200, 4200, 8.5, 4);
    pa_add_cargo(3, 7000, 9000, 14.0, 5);

    print_str("\nAdding equipment...\n");
    pa_add_equipment(1, 65, 250.0);
    pa_add_equipment(1, 55, 220.0);
    pa_add_equipment(2, 40, 180.0);
    pa_add_equipment(2, 35, 160.0);
    pa_add_equipment(3, 5000, 800.0);
    pa_add_equipment(3, 4000, 700.0);
    pa_add_equipment(1, 60, 240.0);
    pa_add_equipment(2, 38, 170.0);
    pa_add_equipment(3, 4500, 750.0);
    pa_add_equipment(1, 50, 200.0);
    pa_add_equipment(2, 42, 185.0);
    pa_add_equipment(3, 3500, 650.0);

    print_str("\nAdding vessels...\n");
    pa_add_vessel(1, 80000, 300, 0, 1);
    pa_add_vessel(1, 75000, 290, 1, 1);
    pa_add_vessel(2, 45000, 200, 2, 2);
    pa_add_vessel(2, 40000, 190, 3, 2);
    pa_add_vessel(3, 120000, 350, 4, 3);
    pa_add_vessel(1, 70000, 280, 5, 3);
    pa_add_vessel(3, 100000, 330, 6, 4);
    pa_add_vessel(2, 38000, 180, 7, 4);
    pa_add_vessel(1, 72000, 285, 8, 5);
    pa_add_vessel(3, 110000, 340, 9, 5);
    pa_add_vessel(1, 68000, 275, 0, 8);
    pa_add_vessel(2, 42000, 195, 1, 8);
    pa_add_vessel(3, 95000, 320, 2, 9);
    pa_add_vessel(1, 65000, 270, 3, 9);

    print_str("\nAdding storage...\n");
    pa_add_storage(1, 50000, 2.0);
    pa_add_storage(2, 30000, 3.5);
    pa_add_storage(3, 20000, 5.0);
    pa_add_storage(1, 45000, 1.8);
    pa_add_storage(2, 25000, 3.0);
    pa_add_storage(3, 15000, 4.5);
    pa_add_storage(1, 40000, 1.5);
    pa_add_storage(2, 28000, 3.2);

    print_str("\nDischarging cargo...\n");
    for (int i = 0; i < 16; i++) {
        pa_discharge_cargo(i, 500 + (i * 150));
    }

    print_str("\nVessel departures...\n");
    for (int i = 0; i < 14; i++) {
        pa_vessel_depart(i, 6 + (i % 5));
    }

    print_str("\nUsing equipment...\n");
    for (int i = 0; i < 12; i++) {
        pa_use_equipment(i, 8 + (i * 2));
    }

    print_str("\nStoring cargo...\n");
    for (int i = 0; i < 8; i++) {
        pa_store_cargo(i, 3000 + (i * 500), 5 + (i % 4));
    }

    print_str("\nBerth report...\n");
    pa_berth_report();

    print_str("\nFinancial report...\n");
    pa_financial_report();

    print_str("\nFinal state...\n");
    pa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
