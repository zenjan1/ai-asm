/* water_transport: Water transportation management (v1.0)
 * Shipping companies, passenger ferries, cargo, vessels, channels
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

#define MAX_COMPANIES  10
#define MAX_VESSELS    16
#define MAX_ROUTES     14
#define MAX_CARGO      12
#define MAX_CHANNELS   10

typedef struct {
    int    company_id;
    int    zone;
    int    type;
    int    vessels;
    int    employees;
    int    revenue;
    int    year_founded;
    int    active;
} company_t;

typedef struct {
    int    vessel_id;
    int    company_id;
    int    type;
    int    tonnage;
    int    capacity;
    int    speed;
    int    year_built;
    int    status;
    int    active;
} vessel_t;

typedef struct {
    int    route_id;
    int    from_zone;
    int    to_zone;
    int    distance_nm;
    int    duration_hours;
    int    frequency;
    int    year_started;
    int    active;
} route_t;

typedef struct {
    int    cargo_id;
    int    route_id;
    int    vessel_id;
    int    weight_tons;
    int    volume_m3;
    int    revenue;
    int    year;
    int    active;
} cargo_t;

typedef struct {
    int    channel_id;
    int    zone;
    int    length_km;
    int    depth_m;
    int    width_m;
    int    traffic;
    int    year_maintained;
    int    active;
} channel_t;

typedef struct {
    int    n_companies;
    int    n_vessels;
    int    n_routes;
    int    n_cargo;
    int    n_channels;
    int    total_tonnage;
    int    total_cargo;
    int    total_revenue;
    int    total_traffic;
    int    total_channel_km;
} wt_state_t;

static company_t companies[MAX_COMPANIES];
static vessel_t vessels[MAX_VESSELS];
static route_t routes[MAX_ROUTES];
static cargo_t cargos[MAX_CARGO];
static channel_t channels[MAX_CHANNELS];
static wt_state_t wt;

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

int wt_init(void) {
    if (initialized) return -1;
    wt.n_companies = 0; wt.n_vessels = 0; wt.n_routes = 0;
    wt.n_cargo = 0; wt.n_channels = 0;
    wt.total_tonnage = 0; wt.total_cargo = 0;
    wt.total_revenue = 0; wt.total_traffic = 0;
    wt.total_channel_km = 0;
    for (int i = 0; i < MAX_COMPANIES; i++) companies[i].active = 0;
    for (int i = 0; i < MAX_VESSELS; i++) vessels[i].active = 0;
    for (int i = 0; i < MAX_ROUTES; i++) routes[i].active = 0;
    for (int i = 0; i < MAX_CARGO; i++) cargos[i].active = 0;
    for (int i = 0; i < MAX_CHANNELS; i++) channels[i].active = 0;
    initialized = 1;
    print_str("[WT] Water transport initialized\n");
    return 0;
}

int wt_add_company(int zone, int type, int vessels, int employees, int revenue, int year) {
    if (wt.n_companies >= MAX_COMPANIES) return -1;
    company_t* c = &companies[wt.n_companies];
    c->company_id = wt.n_companies;
    c->zone = zone;
    c->type = type;
    c->vessels = vessels;
    c->employees = employees;
    c->revenue = revenue;
    c->year_founded = year;
    c->active = 1;
    wt.total_revenue += revenue;
    wt.n_companies++;
    print_str("[WT] Company "); print_int(wt.n_companies - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type);
    print_str(" vsl="); print_int(vessels);
    print_str(" emp="); print_int(employees);
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return wt.n_companies - 1;
}

int wt_add_vessel(int company_id, int type, int tonnage, int capacity, int speed, int year) {
    if (wt.n_vessels >= MAX_VESSELS) return -1;
    vessel_t* v = &vessels[wt.n_vessels];
    v->vessel_id = wt.n_vessels;
    v->company_id = company_id;
    v->type = type;
    v->tonnage = tonnage;
    v->capacity = capacity;
    v->speed = speed;
    v->year_built = year;
    v->status = 1;
    v->active = 1;
    wt.total_tonnage += tonnage;
    wt.n_vessels++;
    print_str("[WT] Vessel "); print_int(wt.n_vessels - 1);
    print_str(" co="); print_int(company_id);
    print_str(" type="); print_int(type);
    print_str(" ton="); print_int(tonnage);
    print_str(" cap="); print_int(capacity);
    print_str(" spd="); print_int(speed); print_str("kt\n");
    return wt.n_vessels - 1;
}

int wt_add_route(int from, int to, int distance, int duration, int frequency, int year) {
    if (wt.n_routes >= MAX_ROUTES) return -1;
    route_t* r = &routes[wt.n_routes];
    r->route_id = wt.n_routes;
    r->from_zone = from;
    r->to_zone = to;
    r->distance_nm = distance;
    r->duration_hours = duration;
    r->frequency = frequency;
    r->year_started = year;
    r->active = 1;
    wt.n_routes++;
    print_str("[WT] Route "); print_int(wt.n_routes - 1);
    print_str(" Z"); print_int(from); print_str("->Z"); print_int(to);
    print_str(" dist="); print_int(distance); print_str("nm");
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" freq="); print_int(frequency); print_str("/wk\n");
    return wt.n_routes - 1;
}

int wt_add_cargo(int route_id, int vessel_id, int weight, int volume, int revenue, int year) {
    if (wt.n_cargo >= MAX_CARGO) return -1;
    cargo_t* c = &cargos[wt.n_cargo];
    c->cargo_id = wt.n_cargo;
    c->route_id = route_id;
    c->vessel_id = vessel_id;
    c->weight_tons = weight;
    c->volume_m3 = volume;
    c->revenue = revenue;
    c->year = year;
    c->active = 1;
    wt.total_cargo += weight;
    wt.total_revenue += revenue;
    wt.n_cargo++;
    print_str("[WT] Cargo "); print_int(wt.n_cargo - 1);
    print_str(" rt="); print_int(route_id);
    print_str(" vsl="); print_int(vessel_id);
    print_str(" wt="); print_int(weight); print_str("t");
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return wt.n_cargo - 1;
}

int wt_add_channel(int zone, int length, int depth, int width, int traffic, int year) {
    if (wt.n_channels >= MAX_CHANNELS) return -1;
    channel_t* ch = &channels[wt.n_channels];
    ch->channel_id = wt.n_channels;
    ch->zone = zone;
    ch->length_km = length;
    ch->depth_m = depth;
    ch->width_m = width;
    ch->traffic = traffic;
    ch->year_maintained = year;
    ch->active = 1;
    wt.total_traffic += traffic;
    wt.total_channel_km += length;
    wt.n_channels++;
    print_str("[WT] Channel "); print_int(wt.n_channels - 1);
    print_str(" zone="); print_int(zone);
    print_str(" len="); print_int(length); print_str("km");
    print_str(" depth="); print_int(depth); print_str("m");
    print_str(" trf="); print_int(traffic); print_str("\n");
    return wt.n_channels - 1;
}

void wt_company_report(void) {
    print_str("[WT] Company report:\n");
    print_str("  Companies: "); print_int(wt.n_companies); print_str("\n");
    print_str("  Vessels: "); print_int(wt.n_vessels); print_str("\n");
    print_str("  Total tonnage: "); print_int(wt.total_tonnage); print_str("\n");
}

void wt_cargo_report(void) {
    print_str("[WT] Cargo report:\n");
    print_str("  Routes: "); print_int(wt.n_routes); print_str("\n");
    print_str("  Total cargo: "); print_int(wt.total_cargo); print_str(" tons\n");
    print_str("  Total revenue: $"); print_int(wt.total_revenue); print_str("\n");
}

void wt_channel_report(void) {
    print_str("[WT] Channel report:\n");
    print_str("  Channels: "); print_int(wt.n_channels); print_str("\n");
    print_str("  Total length: "); print_int(wt.total_channel_km); print_str(" km\n");
    print_str("  Total traffic: "); print_int(wt.total_traffic); print_str("\n");
}

void wt_print_state(void) {
    print_str("[WT] Co="); print_int(wt.n_companies);
    print_str(" Vs="); print_int(wt.n_vessels);
    print_str(" Rt="); print_int(wt.n_routes);
    print_str(" Cg="); print_int(wt.n_cargo);
    print_str(" Ch="); print_int(wt.n_channels);
    print_str("\n");
}

int main(void) {
    print_str("=== Water Transport Demo ===\n\n");
    wt_init();

    print_str("Adding companies...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int type = (i % 3) + 1;
        int vsl = 5 + (i * 3);
        int emp = 100 + (i * 50);
        int rev = 10000000 + (i * 5000000);
        int year = 1990 + (i * 3);
        wt_add_company(zone, type, vsl, emp, rev, year);
    }

    print_str("\nAdding vessels...\n");
    for (int i = 0; i < 16; i++) {
        int co = i % 10;
        int type = (i % 4) + 1;
        int ton = 5000 + (i * 2000);
        int cap = 200 + (i * 50);
        int spd = 10 + (i * 2);
        int year = 2005 + (i % 15);
        wt_add_vessel(co, type, ton, cap, spd, year);
    }

    print_str("\nAdding routes...\n");
    for (int i = 0; i < 14; i++) {
        int from = (i % 4) + 1;
        int to = ((i + 2) % 4) + 1;
        int dist = 100 + (i * 50);
        int dur = 4 + (i * 2);
        int freq = 2 + (i % 5);
        int year = 2010 + (i % 10);
        wt_add_route(from, to, dist, dur, freq, year);
    }

    print_str("\nAdding cargo...\n");
    for (int i = 0; i < 12; i++) {
        int rt = i % 14;
        int vsl = i % 16;
        int wt_val = 1000 + (i * 500);
        int vol = 50 + (i * 20);
        int rev = 50000 + (i * 20000);
        int year = 2024;
        wt_add_cargo(rt, vsl, wt_val, vol, rev, year);
    }

    print_str("\nAdding channels...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int len = 20 + (i * 10);
        int depth = 8 + (i * 2);
        int width = 50 + (i * 20);
        int trf = 500 + (i * 200);
        int year = 2020 + (i % 5);
        wt_add_channel(zone, len, depth, width, trf, year);
    }

    print_str("\nCompany report...\n");
    wt_company_report();

    print_str("\nCargo report...\n");
    wt_cargo_report();

    print_str("\nChannel report...\n");
    wt_channel_report();

    print_str("\nFinal state...\n");
    wt_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
