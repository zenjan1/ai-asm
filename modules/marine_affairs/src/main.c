/* marine_affairs: Marine affairs management (v1.0)
 * Sea area management, marine monitoring, fishery, enforcement, ports
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

#define MAX_SEA_AREA     16
#define MAX_MONITOR      14
#define MAX_FISHERY      12
#define MAX_ENFORCE      10
#define MAX_PORT         10

typedef struct {
    int    sea_area_id;
    int    zone_type;
    int    region_id;
    int    area_km2;
    int    permit_holder;
    int    usage_fee;
    int    expiry_year;
    int    year;
    int    active;
} sea_area_t;

typedef struct {
    int    monitor_id;
    int    station_id;
    int    type;
    int    water_quality;
    int    ecosystem_score;
    int    pollution_index;
    int    alerts;
    int    year;
    int    active;
} monitor_t;

typedef struct {
    int    fishery_id;
    int    license_id;
    int    type;
    int    vessel_id;
    int    catch_volume;
    int    catch_value;
    int    aquaculture_area;
    int    year;
    int    active;
} fishery_t;

typedef struct {
    int    enforce_id;
    int    patrol_id;
    int    violation_type;
    int    vessel_id;
    int    fine;
    int    seized;
    int    year;
    int    status;
    int    active;
} enforcement_t;

typedef struct {
    int    port_id;
    int    type;
    int    region_id;
    int    berths;
    int    throughput_tons;
    int    revenue;
    int    channel_depth;
    int    year;
    int    active;
} port_t;

typedef struct {
    int    n_sea_area;
    int    n_monitor;
    int    n_fishery;
    int    n_enforce;
    int    n_port;
    int    total_area;
    int    total_catch;
    int    total_throughput;
    int    total_revenue;
    int    total_fines;
} ma_state_t;

static sea_area_t sea_areas[MAX_SEA_AREA];
static monitor_t monitors[MAX_MONITOR];
static fishery_t fisheries[MAX_FISHERY];
static enforcement_t enforcement[MAX_ENFORCE];
static port_t ports[MAX_PORT];
static ma_state_t ma;

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

int ma_init(void) {
    if (initialized) return -1;
    ma.n_sea_area = 0; ma.n_monitor = 0; ma.n_fishery = 0;
    ma.n_enforce = 0; ma.n_port = 0;
    ma.total_area = 0; ma.total_catch = 0;
    ma.total_throughput = 0; ma.total_revenue = 0;
    ma.total_fines = 0;
    for (int i = 0; i < MAX_SEA_AREA; i++) sea_areas[i].active = 0;
    for (int i = 0; i < MAX_MONITOR; i++) monitors[i].active = 0;
    for (int i = 0; i < MAX_FISHERY; i++) fisheries[i].active = 0;
    for (int i = 0; i < MAX_ENFORCE; i++) enforcement[i].active = 0;
    for (int i = 0; i < MAX_PORT; i++) ports[i].active = 0;
    initialized = 1;
    print_str("[MA] Marine affairs initialized\n");
    return 0;
}

int ma_sea_area(int zone, int region, int area, int holder, int fee, int expiry, int year) {
    if (ma.n_sea_area >= MAX_SEA_AREA) return -1;
    sea_area_t* sa = &sea_areas[ma.n_sea_area];
    sa->sea_area_id = ma.n_sea_area;
    sa->zone_type = zone;
    sa->region_id = region;
    sa->area_km2 = area;
    sa->permit_holder = holder;
    sa->usage_fee = fee;
    sa->expiry_year = expiry;
    sa->year = year;
    sa->active = 1;
    ma.total_area += area;
    ma.n_sea_area++;
    print_str("[MA] Sea area "); print_int(ma.n_sea_area - 1);
    print_str(" zone="); print_int(zone);
    print_str(" reg="); print_int(region);
    print_str(" area="); print_int(area); print_str("km2");
    print_str(" hld="); print_int(holder);
    print_str(" fee=$"); print_int(fee); print_str("\n");
    return ma.n_sea_area - 1;
}

int ma_monitor(int station, int type, int quality, int ecosystem, int pollution, int alerts, int year) {
    if (ma.n_monitor >= MAX_MONITOR) return -1;
    monitor_t* m = &monitors[ma.n_monitor];
    m->monitor_id = ma.n_monitor;
    m->station_id = station;
    m->type = type;
    m->water_quality = quality;
    m->ecosystem_score = ecosystem;
    m->pollution_index = pollution;
    m->alerts = alerts;
    m->year = year;
    m->active = 1;
    ma.n_monitor++;
    print_str("[MA] Monitor "); print_int(ma.n_monitor - 1);
    print_str(" stn="); print_int(station);
    print_str(" type="); print_int(type);
    print_str(" wq="); print_int(quality);
    print_str(" eco="); print_int(ecosystem);
    print_str(" pol="); print_int(pollution);
    print_str(" alr="); print_int(alerts); print_str("\n");
    return ma.n_monitor - 1;
}

int ma_fishery(int license, int type, int vessel, int volume, int value, int aqua_area, int year) {
    if (ma.n_fishery >= MAX_FISHERY) return -1;
    fishery_t* f = &fisheries[ma.n_fishery];
    f->fishery_id = ma.n_fishery;
    f->license_id = license;
    f->type = type;
    f->vessel_id = vessel;
    f->catch_volume = volume;
    f->catch_value = value;
    f->aquaculture_area = aqua_area;
    f->year = year;
    f->active = 1;
    ma.total_catch += volume;
    ma.n_fishery++;
    print_str("[MA] Fishery "); print_int(ma.n_fishery - 1);
    print_str(" lic="); print_int(license);
    print_str(" type="); print_int(type);
    print_str(" vsl="); print_int(vessel);
    print_str(" vol="); print_int(volume); print_str("t");
    print_str(" val=$"); print_int(value); print_str("\n");
    return ma.n_fishery - 1;
}

int ma_enforce(int patrol, int violation, int vessel, int fine, int seized, int year) {
    if (ma.n_enforce >= MAX_ENFORCE) return -1;
    enforcement_t* e = &enforcement[ma.n_enforce];
    e->enforce_id = ma.n_enforce;
    e->patrol_id = patrol;
    e->violation_type = violation;
    e->vessel_id = vessel;
    e->fine = fine;
    e->seized = seized;
    e->year = year;
    e->status = 1;
    e->active = 1;
    ma.total_fines += fine;
    ma.n_enforce++;
    print_str("[MA] Enforce "); print_int(ma.n_enforce - 1);
    print_str(" ptr="); print_int(patrol);
    print_str(" viol="); print_int(violation);
    print_str(" vsl="); print_int(vessel);
    print_str(" fine=$"); print_int(fine);
    print_str(" szd="); print_int(seized); print_str("\n");
    return ma.n_enforce - 1;
}

int ma_port(int type, int region, int berths, int throughput, int revenue, int depth, int year) {
    if (ma.n_port >= MAX_PORT) return -1;
    port_t* p = &ports[ma.n_port];
    p->port_id = ma.n_port;
    p->type = type;
    p->region_id = region;
    p->berths = berths;
    p->throughput_tons = throughput;
    p->revenue = revenue;
    p->channel_depth = depth;
    p->year = year;
    p->active = 1;
    ma.total_throughput += throughput;
    ma.total_revenue += revenue;
    ma.n_port++;
    print_str("[MA] Port "); print_int(ma.n_port - 1);
    print_str(" type="); print_int(type);
    print_str(" reg="); print_int(region);
    print_str(" bth="); print_int(berths);
    print_str(" thr="); print_int(throughput); print_str("t");
    print_str(" rev=$"); print_int(revenue); print_str("\n");
    return ma.n_port - 1;
}

void ma_sea_report(void) {
    print_str("[MA] Sea area report:\n");
    print_str("  Sea areas: "); print_int(ma.n_sea_area); print_str("\n");
    print_str("  Total area: "); print_int(ma.total_area); print_str(" km2\n");
}

void ma_fishery_report(void) {
    print_str("[MA] Fishery report:\n");
    print_str("  Fisheries: "); print_int(ma.n_fishery); print_str("\n");
    print_str("  Total catch: "); print_int(ma.total_catch); print_str(" tons\n");
}

void ma_port_report(void) {
    print_str("[MA] Port report:\n");
    print_str("  Ports: "); print_int(ma.n_port); print_str("\n");
    print_str("  Total throughput: "); print_int(ma.total_throughput); print_str(" tons\n");
    print_str("  Total revenue: $"); print_int(ma.total_revenue); print_str("\n");
    print_str("  Monitoring stations: "); print_int(ma.n_monitor); print_str("\n");
    print_str("  Enforcement actions: "); print_int(ma.n_enforce); print_str("\n");
    print_str("  Total fines: $"); print_int(ma.total_fines); print_str("\n");
}

void ma_print_state(void) {
    print_str("[MA] Sa="); print_int(ma.n_sea_area);
    print_str(" Mn="); print_int(ma.n_monitor);
    print_str(" Fs="); print_int(ma.n_fishery);
    print_str(" En="); print_int(ma.n_enforce);
    print_str(" Pt="); print_int(ma.n_port);
    print_str("\n");
}

int main(void) {
    print_str("=== Marine Affairs Demo ===\n\n");
    ma_init();

    print_str("Sea area management...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 4) + 1;
        int reg = (i % 6) + 1;
        int area = 10 + (i * 5);
        int hld = 1000 + (i * 13);
        int fee = 5000 + (i * 2000);
        int exp = 2028 + (i % 5);
        int year = 2020 + (i % 4);
        ma_sea_area(zone, reg, area, hld, fee, exp, year);
    }

    print_str("\nMarine monitoring...\n");
    for (int i = 0; i < 14; i++) {
        int stn = 200 + (i * 10);
        int type = (i % 5) + 1;
        int wq = 60 + (i * 3);
        int eco = 50 + (i * 4);
        int pol = 10 + (i * 5);
        int alr = i % 4;
        int year = 2022 + (i % 3);
        ma_monitor(stn, type, wq, eco, pol, alr, year);
    }

    print_str("\nFishery management...\n");
    for (int i = 0; i < 12; i++) {
        int lic = 3000 + (i * 11);
        int type = (i % 3) + 1;
        int vsl = 4000 + (i * 7);
        int vol = 50 + (i * 30);
        int val = 10000 + (i * 5000);
        int aqua = (i % 3 == 0) ? 100 + (i * 50) : 0;
        int year = 2023 + (i % 2);
        ma_fishery(lic, type, vsl, vol, val, aqua, year);
    }

    print_str("\nMarine enforcement...\n");
    for (int i = 0; i < 10; i++) {
        int ptr = 500 + (i * 5);
        int viol = (i % 4) + 1;
        int vsl = 6000 + (i * 13);
        int fine = 1000 + (i * 500);
        int szd = (i % 3 == 0) ? 1 : 0;
        int year = 2024;
        ma_enforce(ptr, viol, vsl, fine, szd, year);
    }

    print_str("\nPort management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 3) + 1;
        int reg = (i % 6) + 1;
        int bth = 5 + (i * 3);
        int thr = 5000 + (i * 3000);
        int rev = 100000 + (i * 50000);
        int dep = 10 + (i * 2);
        int year = 2024;
        ma_port(type, reg, bth, thr, rev, dep, year);
    }

    print_str("\nSea area report...\n");
    ma_sea_report();

    print_str("\nFishery report...\n");
    ma_fishery_report();

    print_str("\nPort report...\n");
    ma_port_report();

    print_str("\nFinal state...\n");
    ma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
