/* transport_admin: Transport administration system (v1.0)
 * Road transport, waterway, public transit, freight logistics, safety
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

#define MAX_ROAD         16
#define MAX_WATERWAY     14
#define MAX_TRANSIT      12
#define MAX_FREIGHT      10
#define MAX_SAFETY       10

typedef struct {
    int    road_id;
    int    operator_id;
    int    vehicle_type;
    int    route_id;
    int    passengers;
    int    license_status;
    int    year;
    int    active;
} road_t;

typedef struct {
    int    waterway_id;
    int    vessel_id;
    int    port_id;
    int    cargo_type;
    int    tonnage;
    int    voyage_count;
    int    year;
    int    active;
} waterway_t;

typedef struct {
    int    transit_id;
    int    system_type;
    int    line_id;
    int    daily_riders;
    int    on_time_pct;
    int    fare_revenue;
    int    year;
    int    active;
} transit_t;

typedef struct {
    int    freight_id;
    int    logistics_id;
    int    hub_id;
    int    cargo_volume;
    int    delivery_time;
    int    cost_per_ton;
    int    year;
    int    active;
} freight_t;

typedef struct {
    int    safety_id;
    int    incident_type;
    int    region_id;
    int    vehicles_inspected;
    int    violations;
    int    accidents;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    n_road;
    int    n_waterway;
    int    n_transit;
    int    n_freight;
    int    n_safety;
    int    total_passengers;
    int    total_tonnage;
    int    total_riders;
    int    total_cargo;
    int    total_inspections;
} ta_state_t;

static road_t roads[MAX_ROAD];
static waterway_t waterways[MAX_WATERWAY];
static transit_t transits[MAX_TRANSIT];
static freight_t freights[MAX_FREIGHT];
static safety_t safeties[MAX_SAFETY];
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
    ta.n_road = 0; ta.n_waterway = 0; ta.n_transit = 0;
    ta.n_freight = 0; ta.n_safety = 0;
    ta.total_passengers = 0; ta.total_tonnage = 0;
    ta.total_riders = 0; ta.total_cargo = 0;
    ta.total_inspections = 0;
    for (int i = 0; i < MAX_ROAD; i++) roads[i].active = 0;
    for (int i = 0; i < MAX_WATERWAY; i++) waterways[i].active = 0;
    for (int i = 0; i < MAX_TRANSIT; i++) transits[i].active = 0;
    for (int i = 0; i < MAX_FREIGHT; i++) freights[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    initialized = 1;
    print_str("[TA] Transport admin initialized\n");
    return 0;
}

int ta_road(int operator_id, int vehicle_type, int route, int passengers, int license_status, int year) {
    if (ta.n_road >= MAX_ROAD) return -1;
    road_t* r = &roads[ta.n_road];
    r->road_id = ta.n_road;
    r->operator_id = operator_id;
    r->vehicle_type = vehicle_type;
    r->route_id = route;
    r->passengers = passengers;
    r->license_status = license_status;
    r->year = year;
    r->active = 1;
    ta.total_passengers += passengers;
    ta.n_road++;
    print_str("[TA] Road "); print_int(ta.n_road - 1);
    print_str(" opr="); print_int(operator_id);
    print_str(" vcl="); print_int(vehicle_type);
    print_str(" rte="); print_int(route);
    print_str(" pax="); print_int(passengers);
    print_str(" lic="); print_int(license_status); print_str("\n");
    return ta.n_road - 1;
}

int ta_waterway(int vessel, int port, int cargo_type, int tonnage, int voyages, int year) {
    if (ta.n_waterway >= MAX_WATERWAY) return -1;
    waterway_t* w = &waterways[ta.n_waterway];
    w->waterway_id = ta.n_waterway;
    w->vessel_id = vessel;
    w->port_id = port;
    w->cargo_type = cargo_type;
    w->tonnage = tonnage;
    w->voyage_count = voyages;
    w->year = year;
    w->active = 1;
    ta.total_tonnage += tonnage * voyages;
    ta.n_waterway++;
    print_str("[TA] Waterway "); print_int(ta.n_waterway - 1);
    print_str(" vsl="); print_int(vessel);
    print_str(" prt="); print_int(port);
    print_str(" cargo="); print_int(cargo_type);
    print_str(" ton="); print_int(tonnage);
    print_str(" voy="); print_int(voyages); print_str("\n");
    return ta.n_waterway - 1;
}

int ta_transit(int system_type, int line, int daily_riders, int on_time_pct, int fare_revenue, int year) {
    if (ta.n_transit >= MAX_TRANSIT) return -1;
    transit_t* t = &transits[ta.n_transit];
    t->transit_id = ta.n_transit;
    t->system_type = system_type;
    t->line_id = line;
    t->daily_riders = daily_riders;
    t->on_time_pct = on_time_pct;
    t->fare_revenue = fare_revenue;
    t->year = year;
    t->active = 1;
    ta.total_riders += daily_riders;
    ta.n_transit++;
    print_str("[TA] Transit "); print_int(ta.n_transit - 1);
    print_str(" sys="); print_int(system_type);
    print_str(" ln="); print_int(line);
    print_str(" rdr="); print_int(daily_riders);
    print_str(" otp="); print_int(on_time_pct); print_str("%");
    print_str(" fare=$"); print_int(fare_revenue); print_str("\n");
    return ta.n_transit - 1;
}

int ta_freight(int logistics_id, int hub, int cargo_volume, int delivery_time, int cost_per_ton, int year) {
    if (ta.n_freight >= MAX_FREIGHT) return -1;
    freight_t* f = &freights[ta.n_freight];
    f->freight_id = ta.n_freight;
    f->logistics_id = logistics_id;
    f->hub_id = hub;
    f->cargo_volume = cargo_volume;
    f->delivery_time = delivery_time;
    f->cost_per_ton = cost_per_ton;
    f->year = year;
    f->active = 1;
    ta.total_cargo += cargo_volume;
    ta.n_freight++;
    print_str("[TA] Freight "); print_int(ta.n_freight - 1);
    print_str(" lgs="); print_int(logistics_id);
    print_str(" hub="); print_int(hub);
    print_str(" vol="); print_int(cargo_volume); print_str("t");
    print_str(" dlr="); print_int(delivery_time); print_str("h");
    print_str(" cpt=$"); print_int(cost_per_ton); print_str("\n");
    return ta.n_freight - 1;
}

int ta_safety(int incident_type, int region, int inspected, int violations, int accidents, int year) {
    if (ta.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[ta.n_safety];
    s->safety_id = ta.n_safety;
    s->incident_type = incident_type;
    s->region_id = region;
    s->vehicles_inspected = inspected;
    s->violations = violations;
    s->accidents = accidents;
    s->year = year;
    s->active = 1;
    ta.total_inspections += inspected;
    ta.n_safety++;
    print_str("[TA] Safety "); print_int(ta.n_safety - 1);
    print_str(" type="); print_int(incident_type);
    print_str(" rgn="); print_int(region);
    print_str(" insp="); print_int(inspected);
    print_str(" vio="); print_int(violations);
    print_str(" acc="); print_int(accidents); print_str("\n");
    return ta.n_safety - 1;
}

void ta_road_report(void) {
    print_str("[TA] Road transport report:\n");
    print_str("  Operators: "); print_int(ta.n_road); print_str("\n");
    print_str("  Total passengers: "); print_int(ta.total_passengers); print_str("\n");
}

void ta_waterway_report(void) {
    print_str("[TA] Waterway report:\n");
    print_str("  Vessels: "); print_int(ta.n_waterway); print_str("\n");
    print_str("  Total tonnage: "); print_int(ta.total_tonnage); print_str(" tons\n");
}

void ta_transit_report(void) {
    print_str("[TA] Public transit report:\n");
    print_str("  Transit systems: "); print_int(ta.n_transit); print_str("\n");
    print_str("  Daily riders: "); print_int(ta.total_riders); print_str("\n");
    print_str("  Freight hubs: "); print_int(ta.n_freight); print_str("\n");
    print_str("  Total cargo: "); print_int(ta.total_cargo); print_str(" tons\n");
    print_str("  Safety inspections: "); print_int(ta.total_inspections); print_str("\n");
}

void ta_print_state(void) {
    print_str("[TA] Rd="); print_int(ta.n_road);
    print_str(" Wt="); print_int(ta.n_waterway);
    print_str(" Tr="); print_int(ta.n_transit);
    print_str(" Fr="); print_int(ta.n_freight);
    print_str(" Sf="); print_int(ta.n_safety);
    print_str("\n");
}

int main(void) {
    print_str("=== Transport Admin Demo ===\n\n");
    ta_init();

    print_str("Road transport...\n");
    for (int i = 0; i < 16; i++) {
        int opr = 1000 + (i * 11);
        int vcl = (i % 4) + 1;
        int rte = 100 + (i * 5);
        int pax = 500 + (i * 100);
        int lic = (i % 5 == 0) ? 0 : 1;
        int year = 2020 + (i % 5);
        ta_road(opr, vcl, rte, pax, lic, year);
    }

    print_str("\nWaterway transport...\n");
    for (int i = 0; i < 14; i++) {
        int vsl = 2000 + (i * 7);
        int prt = 300 + (i * 3);
        int cargo = (i % 4) + 1;
        int ton = 5000 + (i * 2000);
        int voy = 10 + (i * 3);
        int year = 2021 + (i % 4);
        ta_waterway(vsl, prt, cargo, ton, voy, year);
    }

    print_str("\nPublic transit...\n");
    for (int i = 0; i < 12; i++) {
        int sys = (i % 3) + 1;
        int ln = 1 + i;
        int rdr = 10000 + (i * 3000);
        int otp = 85 + (i % 12);
        int fare = 50000 + (i * 10000);
        int year = 2022 + (i % 3);
        ta_transit(sys, ln, rdr, otp, fare, year);
    }

    print_str("\nFreight logistics...\n");
    for (int i = 0; i < 10; i++) {
        int lgs = 4000 + (i * 13);
        int hub = 400 + (i * 5);
        int vol = 2000 + (i * 500);
        int dlr = 24 + (i * 4);
        int cpt = 50 + (i * 10);
        int year = 2023 + (i % 2);
        ta_freight(lgs, hub, vol, dlr, cpt, year);
    }

    print_str("\nTransport safety...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int rgn = (i % 8) + 1;
        int insp = 1000 + (i * 300);
        int vio = 50 + (i * 10);
        int acc = 5 + (i % 8);
        int year = 2024;
        ta_safety(type, rgn, insp, vio, acc, year);
    }

    print_str("\nRoad transport report...\n");
    ta_road_report();

    print_str("\nWaterway report...\n");
    ta_waterway_report();

    print_str("\nTransit report...\n");
    ta_transit_report();

    print_str("\nFinal state...\n");
    ta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
