/* unmanned_system_admin: Unmanned system administration system (v1.0)
 * UAV systems, unmanned ships, unmanned vehicles, ground stations, support systems
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

#define MAX_UAV          16
#define MAX_SHIP         14
#define MAX_VEHICLE      12
#define MAX_STATION      10
#define MAX_SUPPORT      10

typedef struct {
    int    uav_id;
    int    uav_type;
    int    uav_category;
    int    multi_rotor;
    int    fixed_wing;
    int    vtol;
    int    year;
    int    active;
} uav_t;

typedef struct {
    int    ship_id;
    int    ship_type;
    int    ship_category;
    int    surface_vessel;
    int    underwater_vehicle;
    int    hybrid;
    int    year;
    int    active;
} ship_t;

typedef struct {
    int    vehicle_id;
    int    vehicle_type;
    int    vehicle_category;
    int    delivery;
    int    patrol;
    int    mining;
    int    year;
    int    active;
} vehicle_t;

typedef struct {
    int    station_id;
    int    station_type;
    int    station_category;
    int    flight_control;
    int    mission_plan;
    int    comm_relay;
    int    year;
    int    active;
} station_t;

typedef struct {
    int    support_id;
    int    support_type;
    int    support_category;
    int    maintenance;
    int    training_sim;
    int    supply;
    int    year;
    int    active;
} support_t;

typedef struct {
    int    n_uav;
    int    n_ship;
    int    n_vehicle;
    int    n_station;
    int    n_support;
    int    total_rotor;
    int    total_surface;
    int    total_delivery;
    int    total_flight;
    int    total_maintenance;
} usa_state_t;

static uav_t uavs[MAX_UAV];
static ship_t ships[MAX_SHIP];
static vehicle_t vehicles[MAX_VEHICLE];
static station_t stations[MAX_STATION];
static support_t supports[MAX_SUPPORT];
static usa_state_t usa;

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

int usa_init(void) {
    if (initialized) return -1;
    usa.n_uav = 0; usa.n_ship = 0; usa.n_vehicle = 0;
    usa.n_station = 0; usa.n_support = 0;
    usa.total_rotor = 0; usa.total_surface = 0;
    usa.total_delivery = 0; usa.total_flight = 0;
    usa.total_maintenance = 0;
    for (int i = 0; i < MAX_UAV; i++) uavs[i].active = 0;
    for (int i = 0; i < MAX_SHIP; i++) ships[i].active = 0;
    for (int i = 0; i < MAX_VEHICLE; i++) vehicles[i].active = 0;
    for (int i = 0; i < MAX_STATION; i++) stations[i].active = 0;
    for (int i = 0; i < MAX_SUPPORT; i++) supports[i].active = 0;
    initialized = 1;
    print_str("[USA] Unmanned system initialized\n");
    return 0;
}

int usa_uav(int uav_type, int cat, int rotor, int wing, int vtol_cnt, int year) {
    if (usa.n_uav >= MAX_UAV) return -1;
    uav_t* u = &uavs[usa.n_uav];
    u->uav_id = usa.n_uav;
    u->uav_type = uav_type;
    u->uav_category = cat;
    u->multi_rotor = rotor;
    u->fixed_wing = wing;
    u->vtol = vtol_cnt;
    u->year = year;
    u->active = 1;
    usa.total_rotor += rotor;
    usa.n_uav++;
    print_str("[USA] UAV "); print_int(usa.n_uav - 1);
    print_str(" type="); print_int(uav_type);
    print_str(" cat="); print_int(cat);
    print_str(" rtr="); print_int(rotor);
    print_str(" wng="); print_int(wing);
    print_str(" vtl="); print_int(vtol_cnt); print_str("\n");
    return usa.n_uav - 1;
}

int usa_ship(int shp_type, int cat, int surface, int underwater, int hyb, int year) {
    if (usa.n_ship >= MAX_SHIP) return -1;
    ship_t* s = &ships[usa.n_ship];
    s->ship_id = usa.n_ship;
    s->ship_type = shp_type;
    s->ship_category = cat;
    s->surface_vessel = surface;
    s->underwater_vehicle = underwater;
    s->hybrid = hyb;
    s->year = year;
    s->active = 1;
    usa.total_surface += surface;
    usa.n_ship++;
    print_str("[USA] Ship "); print_int(usa.n_ship - 1);
    print_str(" type="); print_int(shp_type);
    print_str(" cat="); print_int(cat);
    print_str(" srf="); print_int(surface);
    print_str(" und="); print_int(underwater);
    print_str(" hyb="); print_int(hyb); print_str("\n");
    return usa.n_ship - 1;
}

int usa_vehicle(int vhc_type, int cat, int deliv, int patrol, int mining_cnt, int year) {
    if (usa.n_vehicle >= MAX_VEHICLE) return -1;
    vehicle_t* v = &vehicles[usa.n_vehicle];
    v->vehicle_id = usa.n_vehicle;
    v->vehicle_type = vhc_type;
    v->vehicle_category = cat;
    v->delivery = deliv;
    v->patrol = patrol;
    v->mining = mining_cnt;
    v->year = year;
    v->active = 1;
    usa.total_delivery += deliv;
    usa.n_vehicle++;
    print_str("[USA] Vehicle "); print_int(usa.n_vehicle - 1);
    print_str(" type="); print_int(vhc_type);
    print_str(" cat="); print_int(cat);
    print_str(" dlv="); print_int(deliv);
    print_str(" ptr="); print_int(patrol);
    print_str(" mng="); print_int(mining_cnt); print_str("\n");
    return usa.n_vehicle - 1;
}

int usa_station(int stn_type, int cat, int flight, int mission, int comm, int year) {
    if (usa.n_station >= MAX_STATION) return -1;
    station_t* st = &stations[usa.n_station];
    st->station_id = usa.n_station;
    st->station_type = stn_type;
    st->station_category = cat;
    st->flight_control = flight;
    st->mission_plan = mission;
    st->comm_relay = comm;
    st->year = year;
    st->active = 1;
    usa.total_flight += flight;
    usa.n_station++;
    print_str("[USA] Station "); print_int(usa.n_station - 1);
    print_str(" type="); print_int(stn_type);
    print_str(" cat="); print_int(cat);
    print_str(" flt="); print_int(flight);
    print_str(" msn="); print_int(mission);
    print_str(" cmr="); print_int(comm); print_str("\n");
    return usa.n_station - 1;
}

int usa_support(int sup_type, int cat, int maint, int train, int supply_cnt, int year) {
    if (usa.n_support >= MAX_SUPPORT) return -1;
    support_t* sp = &supports[usa.n_support];
    sp->support_id = usa.n_support;
    sp->support_type = sup_type;
    sp->support_category = cat;
    sp->maintenance = maint;
    sp->training_sim = train;
    sp->supply = supply_cnt;
    sp->year = year;
    sp->active = 1;
    usa.total_maintenance += maint;
    usa.n_support++;
    print_str("[USA] Support "); print_int(usa.n_support - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" cat="); print_int(cat);
    print_str(" mnt="); print_int(maint);
    print_str(" trn="); print_int(train);
    print_str(" sly="); print_int(supply_cnt); print_str("\n");
    return usa.n_support - 1;
}

void usa_uav_report(void) {
    print_str("[USA] UAV report:\n");
    print_str("  UAV system categories: "); print_int(usa.n_uav); print_str("\n");
    print_str("  Total multi-rotor: "); print_int(usa.total_rotor); print_str("\n");
}

void usa_ship_report(void) {
    print_str("[USA] Ship report:\n");
    print_str("  Unmanned ship categories: "); print_int(usa.n_ship); print_str("\n");
    print_str("  Total surface vessels: "); print_int(usa.total_surface); print_str("\n");
}

void usa_support_report(void) {
    print_str("[USA] Support report:\n");
    print_str("  Unmanned vehicle categories: "); print_int(usa.n_vehicle); print_str("\n");
    print_str("  Total delivery vehicles: "); print_int(usa.total_delivery); print_str("\n");
    print_str("  Ground station categories: "); print_int(usa.n_station); print_str("\n");
    print_str("  Total flight control stations: "); print_int(usa.total_flight); print_str("\n");
    print_str("  Support categories: "); print_int(usa.n_support); print_str("\n");
    print_str("  Total maintenance: "); print_int(usa.total_maintenance); print_str("\n");
}

void usa_print_state(void) {
    print_str("[USA] Uv="); print_int(usa.n_uav);
    print_str(" Sh="); print_int(usa.n_ship);
    print_str(" Vh="); print_int(usa.n_vehicle);
    print_str(" St="); print_int(usa.n_station);
    print_str(" Sp="); print_int(usa.n_support);
    print_str("\n");
}

int main(void) {
    print_str("=== Unmanned System Admin Demo ===\n\n");
    usa_init();

    print_str("UAV systems...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int rtr = 45 + (i * 11);
        int wng = 30 + (i * 7);
        int vtl = 20 + (i * 5);
        int year = 2020 + (i % 5);
        usa_uav(type, cat, rtr, wng, vtl, year);
    }

    print_str("\nUnmanned ships...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int srf = 35 + (i * 9);
        int und = 25 + (i * 6);
        int hyb = 15 + (i * 4);
        int year = 2021 + (i % 4);
        usa_ship(type, cat, srf, und, hyb, year);
    }

    print_str("\nUnmanned vehicles...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dlv = 40 + (i * 10);
        int ptr = 28 + (i * 7);
        int mng = 18 + (i * 4);
        int year = 2022 + (i % 3);
        usa_vehicle(type, cat, dlv, ptr, mng, year);
    }

    print_str("\nGround stations...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int flt = 30 + (i * 7);
        int msn = 22 + (i * 5);
        int cmr = 15 + (i * 3);
        int year = 2023 + (i % 2);
        usa_station(type, cat, flt, msn, cmr, year);
    }

    print_str("\nSupport systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mnt = 25 + (i * 6);
        int trn = 18 + (i * 4);
        int sly = 12 + (i * 3);
        int year = 2024;
        usa_support(type, cat, mnt, trn, sly, year);
    }

    print_str("\nUAV report...\n");
    usa_uav_report();

    print_str("\nShip report...\n");
    usa_ship_report();

    print_str("\nSupport report...\n");
    usa_support_report();

    print_str("\nFinal state...\n");
    usa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
