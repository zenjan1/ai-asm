/* shipping_company: Shipping company management (v1.0)
 * Vessels, routes, containers, crew, fuel, revenue
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

#define MAX_VESSELS   12
#define MAX_ROUTES    10
#define MAX_CONTAINERS 20
#define MAX_CREW      16
#define MAX_VOYAGES   14

typedef struct {
    int    vessel_id;
    int    type;
    int    dwt;
    int    teu;
    int    speed_kn;
    int    fuel_cap_tons;
    int    fuel_onboard;
    double daily_cost;
    double revenue;
    int    active;
} vessel_t;

typedef struct {
    int    route_id;
    int    region;
    int    distance_nm;
    int    port_calls;
    double freight_rate;
    int    n_voyages;
    double revenue;
    int    active;
} route_t;

typedef struct {
    int    cont_id;
    int    type;
    int    weight_tons;
    int    route_id;
    int    vessel_id;
    double freight;
    int    status;
    int    active;
} container_t;

typedef struct {
    int    crew_id;
    int    rank;
    int    vessel_id;
    int    months_on;
    double salary;
    int    certifications;
    int    active;
} crew_t;

typedef struct {
    int    voyage_id;
    int    vessel_id;
    int    route_id;
    int    cargo_tons;
    int    fuel_used;
    int    days;
    double revenue;
    double fuel_cost;
    int    active;
} voyage_t;

typedef struct {
    int    n_vessels;
    int    n_routes;
    int    n_containers;
    int    n_crew;
    int    n_voyages;
    double freight_revenue;
    double surcharge_revenue;
    double demurrage_revenue;
    double total_revenue;
    double fuel_costs;
    double port_costs;
    double crew_costs;
    double maintenance_costs;
    int    total_cargo_tons;
    int    total_voyages_completed;
} sc_state_t;

static vessel_t vessels[MAX_VESSELS];
static route_t routes[MAX_ROUTES];
static container_t containers[MAX_CONTAINERS];
static crew_t crew[MAX_CREW];
static voyage_t voyages[MAX_VOYAGES];
static sc_state_t sc;

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

int sc_init(void) {
    if (initialized) return -1;
    sc.n_vessels = 0; sc.n_routes = 0; sc.n_containers = 0;
    sc.n_crew = 0; sc.n_voyages = 0;
    sc.freight_revenue = 0.0; sc.surcharge_revenue = 0.0;
    sc.demurrage_revenue = 0.0; sc.total_revenue = 0.0;
    sc.fuel_costs = 0.0; sc.port_costs = 0.0;
    sc.crew_costs = 0.0; sc.maintenance_costs = 0.0;
    sc.total_cargo_tons = 0; sc.total_voyages_completed = 0;
    for (int i = 0; i < MAX_VESSELS; i++) vessels[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_CONTAINERS; i++) containers[i].active = 0;
    for (int i = 0; i < MAX_CREW; i++) crew[i].active = 0;
    for (int i = 0; i < MAX_VOYAGES; i++) voyages[i].active = 0;
    initialized = 1;
    print_str("[SC] Shipping company initialized\n");
    return 0;
}

int sc_add_vessel(int type, int dwt, int teu, int speed, int fuel_cap, double daily_cost) {
    if (sc.n_vessels >= MAX_VESSELS) return -1;
    vessel_t* v = &vessels[sc.n_vessels];
    v->vessel_id = sc.n_vessels;
    v->type = type;
    v->dwt = dwt;
    v->teu = teu;
    v->speed_kn = speed;
    v->fuel_cap_tons = fuel_cap;
    v->fuel_onboard = fuel_cap;
    v->daily_cost = daily_cost;
    v->revenue = 0.0;
    v->active = 1;
    sc.maintenance_costs += 1000.0;
    sc.n_vessels++;
    print_str("[SC] Vessel "); print_int(sc.n_vessels - 1);
    print_str(" type="); print_int(type);
    print_str(" dwt="); print_int(dwt);
    print_str(" teu="); print_int(teu);
    print_str(" kn="); print_int(speed); print_str("\n");
    return sc.n_vessels - 1;
}

int sc_add_route(int region, int distance_nm, int port_calls, double rate) {
    if (sc.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[sc.n_routes];
    r->route_id = sc.n_routes;
    r->region = region;
    r->distance_nm = distance_nm;
    r->port_calls = port_calls;
    r->freight_rate = rate;
    r->n_voyages = 0;
    r->revenue = 0.0;
    r->active = 1;
    sc.n_routes++;
    print_str("[SC] Route "); print_int(sc.n_routes - 1);
    print_str(" reg="); print_int(region);
    print_str(" nm="); print_int(distance_nm);
    print_str(" ports="); print_int(port_calls);
    print_str(" $"); print_int((int)rate); print_str("/t\n");
    return sc.n_routes - 1;
}

int sc_add_container(int type, int weight, int route_id, int vessel_id, double freight) {
    if (sc.n_containers >= MAX_CONTAINERS) return -1;
    if (route_id >= sc.n_routes || vessel_id >= sc.n_vessels) return -2;
    container_t* c = &containers[sc.n_containers];
    c->cont_id = sc.n_containers;
    c->type = type;
    c->weight_tons = weight;
    c->route_id = route_id;
    c->vessel_id = vessel_id;
    c->freight = freight;
    c->status = 1;
    c->active = 1;
    sc.freight_revenue += freight;
    sc.total_revenue += freight;
    sc.total_cargo_tons += weight;
    sc.n_containers++;
    print_str("[SC] Cont "); print_int(sc.n_containers - 1);
    print_str(" type="); print_int(type);
    print_str(" t="); print_int(weight);
    print_str(" R"); print_int(route_id);
    print_str(" V"); print_int(vessel_id);
    print_str(" $"); print_int((int)freight); print_str("\n");
    return sc.n_containers - 1;
}

int sc_add_crew(int rank, int vessel_id, int months, double salary, int certs) {
    if (sc.n_crew >= MAX_CREW || vessel_id >= sc.n_vessels) return -1;
    crew_t* cr = &crew[sc.n_crew];
    cr->crew_id = sc.n_crew;
    cr->rank = rank;
    cr->vessel_id = vessel_id;
    cr->months_on = months;
    cr->salary = salary;
    cr->certifications = certs;
    cr->active = 1;
    sc.crew_costs += salary * months;
    sc.n_crew++;
    print_str("[SC] Crew "); print_int(sc.n_crew - 1);
    print_str(" rank="); print_int(rank);
    print_str(" V"); print_int(vessel_id);
    print_str(" mo="); print_int(months);
    print_str(" $"); print_int((int)salary); print_str("/mo\n");
    return sc.n_crew - 1;
}

int sc_create_voyage(int vessel_id, int route_id, int cargo_tons, int days) {
    if (sc.n_voyages >= MAX_VOYAGES) return -1;
    if (vessel_id >= sc.n_vessels || route_id >= sc.n_routes) return -2;
    voyage_t* v = &voyages[sc.n_voyages];
    v->voyage_id = sc.n_voyages;
    v->vessel_id = vessel_id;
    v->route_id = route_id;
    v->cargo_tons = cargo_tons;
    v->days = days;
    v->fuel_used = days * 40;
    v->fuel_cost = v->fuel_used * 450.0;
    v->revenue = 0.0;
    v->active = 1;
    sc.fuel_costs += v->fuel_cost;
    sc.port_costs += routes[route_id].port_calls * 15000.0;
    vessels[vessel_id].fuel_onboard -= v->fuel_used;
    sc.total_voyages_completed++;
    routes[route_id].n_voyages++;
    sc.n_voyages++;
    print_str("[SC] Voyage "); print_int(sc.n_voyages - 1);
    print_str(" V"); print_int(vessel_id);
    print_str(" R"); print_int(route_id);
    print_str(" t="); print_int(cargo_tons);
    print_str(" d="); print_int(days); print_str("\n");
    return sc.n_voyages - 1;
}

int sc_collect_freight(int voyage_id, double amount) {
    if (voyage_id >= sc.n_voyages) return -1;
    voyage_t* v = &voyages[voyage_id];
    v->revenue += amount;
    vessels[v->vessel_id].revenue += amount;
    routes[v->route_id].revenue += amount;
    sc.freight_revenue += amount;
    sc.total_revenue += amount;
    print_str("[SC] Freight Vy"); print_int(voyage_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int sc_add_surcharge(int voyage_id, double amount) {
    if (voyage_id >= sc.n_voyages) return -1;
    sc.surcharge_revenue += amount;
    sc.total_revenue += amount;
    voyages[voyage_id].revenue += amount;
    print_str("[SC] Surcharge Vy"); print_int(voyage_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int sc_add_demurrage(int vessel_id, double amount) {
    if (vessel_id >= sc.n_vessels) return -1;
    sc.demurrage_revenue += amount;
    sc.total_revenue += amount;
    vessels[vessel_id].revenue += amount;
    print_str("[SC] Demurrage V"); print_int(vessel_id);
    print_str(" $"); print_int((int)amount); print_str("\n");
    return 0;
}

int sc_refuel_vessel(int vessel_id, int tons) {
    if (vessel_id >= sc.n_vessels) return -1;
    vessel_t* v = &vessels[vessel_id];
    v->fuel_onboard += tons;
    double cost = tons * 450.0;
    sc.fuel_costs += cost;
    print_str("[SC] Refuel V"); print_int(vessel_id);
    print_str(" tons="); print_int(tons);
    print_str(" $"); print_int((int)cost); print_str("\n");
    return 0;
}

void sc_vessel_report(void) {
    print_str("[SC] Vessel report:\n");
    for (int i = 0; i < sc.n_vessels; i++) {
        vessel_t* v = &vessels[i];
        print_str("  V"); print_int(i);
        print_str(" type="); print_int(v->type);
        print_str(" dwt="); print_int(v->dwt);
        print_str(" teu="); print_int(v->teu);
        print_str(" fuel="); print_int(v->fuel_onboard);
        print_str(" rev="); print_int((int)v->revenue); print_str("\n");
    }
}

void sc_financial_report(void) {
    print_str("[SC] Financial report:\n");
    print_str("  Freight revenue: "); print_int((int)sc.freight_revenue); print_str("\n");
    print_str("  Surcharge revenue: "); print_int((int)sc.surcharge_revenue); print_str("\n");
    print_str("  Demurrage revenue: "); print_int((int)sc.demurrage_revenue); print_str("\n");
    print_str("  Total revenue: "); print_int((int)sc.total_revenue); print_str("\n");
    print_str("  Fuel costs: "); print_int((int)sc.fuel_costs); print_str("\n");
    print_str("  Port costs: "); print_int((int)sc.port_costs); print_str("\n");
    print_str("  Crew costs: "); print_int((int)sc.crew_costs); print_str("\n");
    print_str("  Maintenance: "); print_int((int)sc.maintenance_costs); print_str("\n");
    double total_costs = sc.fuel_costs + sc.port_costs + sc.crew_costs + sc.maintenance_costs;
    double profit = sc.total_revenue - total_costs;
    print_str("  Net profit: "); print_int((int)profit); print_str("\n");
    print_str("  Cargo: "); print_int(sc.total_cargo_tons);
    print_str("t Voyages: "); print_int(sc.total_voyages_completed); print_str("\n");
}

void sc_print_state(void) {
    print_str("[SC] Vessels="); print_int(sc.n_vessels);
    print_str(" Routes="); print_int(sc.n_routes);
    print_str(" Cont="); print_int(sc.n_containers);
    print_str(" Crew="); print_int(sc.n_crew);
    print_str(" Voyages="); print_int(sc.n_voyages);
    print_str("\n");
    print_str("  Revenue: "); print_int((int)sc.total_revenue); print_str("\n");
}

int main(void) {
    print_str("=== Shipping Company Demo ===\n\n");
    sc_init();

    print_str("Adding vessels...\n");
    sc_add_vessel(1, 80000, 6000, 22, 8000, 15000.0);
    sc_add_vessel(1, 75000, 5500, 21, 7500, 14000.0);
    sc_add_vessel(2, 50000, 0, 18, 5000, 10000.0);
    sc_add_vessel(2, 45000, 0, 17, 4500, 9500.0);
    sc_add_vessel(3, 120000, 0, 16, 12000, 20000.0);
    sc_add_vessel(1, 70000, 5000, 22, 7000, 13500.0);
    sc_add_vessel(3, 100000, 0, 15, 10000, 18000.0);
    sc_add_vessel(1, 65000, 4800, 21, 6500, 12500.0);
    sc_add_vessel(2, 42000, 0, 17, 4200, 9000.0);
    sc_add_vessel(4, 80000, 0, 19, 9000, 16000.0);
    sc_add_vessel(1, 72000, 5200, 22, 7200, 13800.0);
    sc_add_vessel(4, 75000, 0, 18, 8500, 15500.0);

    print_str("\nAdding routes...\n");
    sc_add_route(1, 8000, 5, 45.0);
    sc_add_route(1, 6500, 4, 42.0);
    sc_add_route(2, 10000, 6, 50.0);
    sc_add_route(2, 9000, 5, 48.0);
    sc_add_route(3, 7000, 4, 40.0);
    sc_add_route(3, 5500, 3, 38.0);
    sc_add_route(4, 12000, 7, 55.0);
    sc_add_route(4, 11000, 6, 52.0);
    sc_add_route(1, 7500, 4, 44.0);
    sc_add_route(2, 8500, 5, 46.0);

    print_str("\nAdding containers...\n");
    for (int i = 0; i < 20; i++) {
        int type = (i % 3) + 1;
        int weight = 15 + (i % 10);
        int route = i % 10;
        int vessel = i % 12;
        double freight = weight * (35.0 + (i * 2));
        sc_add_container(type, weight, route, vessel, freight);
    }

    print_str("\nAdding crew...\n");
    sc_add_crew(1, 0, 6, 8000.0, 5);
    sc_add_crew(2, 0, 6, 5000.0, 4);
    sc_add_crew(3, 0, 6, 4500.0, 3);
    sc_add_crew(4, 1, 6, 3500.0, 3);
    sc_add_crew(4, 1, 6, 3500.0, 2);
    sc_add_crew(5, 2, 6, 2500.0, 2);
    sc_add_crew(5, 2, 6, 2500.0, 2);
    sc_add_crew(5, 3, 6, 2500.0, 1);
    sc_add_crew(1, 4, 6, 8500.0, 5);
    sc_add_crew(2, 4, 6, 5500.0, 4);
    sc_add_crew(3, 5, 6, 4800.0, 3);
    sc_add_crew(4, 5, 6, 3800.0, 3);
    sc_add_crew(5, 6, 6, 2800.0, 2);
    sc_add_crew(5, 7, 6, 2800.0, 2);
    sc_add_crew(1, 8, 6, 7500.0, 4);
    sc_add_crew(2, 9, 6, 5200.0, 4);

    print_str("\nCreating voyages...\n");
    for (int i = 0; i < 14; i++) {
        int vessel = i % 12;
        int route = i % 10;
        int cargo = 30000 + (i * 3000);
        int days = 15 + (i % 10);
        sc_create_voyage(vessel, route, cargo, days);
    }

    print_str("\nCollecting freight...\n");
    for (int i = 0; i < 14; i++) {
        sc_collect_freight(i, 500000.0 + (i * 50000));
    }

    print_str("\nSurcharges...\n");
    for (int i = 0; i < 10; i++) {
        sc_add_surcharge(i % 14, 25000.0 + (i * 5000));
    }

    print_str("\nDemurrage...\n");
    for (int i = 0; i < 8; i++) {
        sc_add_demurrage(i % 12, 15000.0 + (i * 3000));
    }

    print_str("\nRefueling...\n");
    for (int i = 0; i < 12; i++) {
        sc_refuel_vessel(i, 2000 + (i * 200));
    }

    print_str("\nVessel report...\n");
    sc_vessel_report();

    print_str("\nFinancial report...\n");
    sc_financial_report();

    print_str("\nFinal state...\n");
    sc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
