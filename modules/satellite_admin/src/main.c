/* satellite_admin: Satellite administration system (v1.0)
 * Satellite manufacturing, launch vehicles, launch services, ground systems, satellite applications
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

#define MAX_SATELLITE    16
#define MAX_ROCKET       14
#define MAX_LAUNCH       12
#define MAX_GROUND       10
#define MAX_APPLICATION  10

typedef struct {
    int    satellite_id;
    int    satellite_type;
    int    satellite_category;
    int    comm_sat;
    int    nav_sat;
    int    remote_sat;
    int    year;
    int    active;
} satellite_t;

typedef struct {
    int    rocket_id;
    int    rocket_type;
    int    rocket_category;
    int    solid_rocket;
    int    liquid_rocket;
    int    reusable;
    int    year;
    int    active;
} rocket_t;

typedef struct {
    int    launch_id;
    int    launch_type;
    int    launch_category;
    int    launch_site;
    int    launch_task;
    int    measure_recover;
    int    year;
    int    active;
} launch_t;

typedef struct {
    int    ground_id;
    int    ground_type;
    int    ground_category;
    int    ground_station;
    int    measure_network;
    int    data_process;
    int    year;
    int    active;
} ground_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    sat_comm;
    int    sat_nav;
    int    sat_remote;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    n_satellite;
    int    n_rocket;
    int    n_launch;
    int    n_ground;
    int    n_app;
    int    total_comm;
    int    total_solid;
    int    total_site;
    int    total_station;
    int    total_sat_comm;
} sta_state_t;

static satellite_t satellites[MAX_SATELLITE];
static rocket_t rockets[MAX_ROCKET];
static launch_t launches[MAX_LAUNCH];
static ground_t grounds[MAX_GROUND];
static app_t apps[MAX_APPLICATION];
static sta_state_t sta;

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

int sta_init(void) {
    if (initialized) return -1;
    sta.n_satellite = 0; sta.n_rocket = 0; sta.n_launch = 0;
    sta.n_ground = 0; sta.n_app = 0;
    sta.total_comm = 0; sta.total_solid = 0;
    sta.total_site = 0; sta.total_station = 0;
    sta.total_sat_comm = 0;
    for (int i = 0; i < MAX_SATELLITE; i++) satellites[i].active = 0;
    for (int i = 0; i < MAX_ROCKET; i++) rockets[i].active = 0;
    for (int i = 0; i < MAX_LAUNCH; i++) launches[i].active = 0;
    for (int i = 0; i < MAX_GROUND; i++) grounds[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[STA] Satellite initialized\n");
    return 0;
}

int sta_satellite(int sat_type, int cat, int comm, int nav, int remote, int year) {
    if (sta.n_satellite >= MAX_SATELLITE) return -1;
    satellite_t* s = &satellites[sta.n_satellite];
    s->satellite_id = sta.n_satellite;
    s->satellite_type = sat_type;
    s->satellite_category = cat;
    s->comm_sat = comm;
    s->nav_sat = nav;
    s->remote_sat = remote;
    s->year = year;
    s->active = 1;
    sta.total_comm += comm;
    sta.n_satellite++;
    print_str("[STA] Satellite "); print_int(sta.n_satellite - 1);
    print_str(" type="); print_int(sat_type);
    print_str(" cat="); print_int(cat);
    print_str(" com="); print_int(comm);
    print_str(" nav="); print_int(nav);
    print_str(" rmt="); print_int(remote); print_str("\n");
    return sta.n_satellite - 1;
}

int sta_rocket(int rkt_type, int cat, int solid, int liquid, int reuse, int year) {
    if (sta.n_rocket >= MAX_ROCKET) return -1;
    rocket_t* r = &rockets[sta.n_rocket];
    r->rocket_id = sta.n_rocket;
    r->rocket_type = rkt_type;
    r->rocket_category = cat;
    r->solid_rocket = solid;
    r->liquid_rocket = liquid;
    r->reusable = reuse;
    r->year = year;
    r->active = 1;
    sta.total_solid += solid;
    sta.n_rocket++;
    print_str("[STA] Rocket "); print_int(sta.n_rocket - 1);
    print_str(" type="); print_int(rkt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sld="); print_int(solid);
    print_str(" lqd="); print_int(liquid);
    print_str(" rus="); print_int(reuse); print_str("\n");
    return sta.n_rocket - 1;
}

int sta_launch(int lnc_type, int cat, int site, int task, int measure, int year) {
    if (sta.n_launch >= MAX_LAUNCH) return -1;
    launch_t* l = &launches[sta.n_launch];
    l->launch_id = sta.n_launch;
    l->launch_type = lnc_type;
    l->launch_category = cat;
    l->launch_site = site;
    l->launch_task = task;
    l->measure_recover = measure;
    l->year = year;
    l->active = 1;
    sta.total_site += site;
    sta.n_launch++;
    print_str("[STA] Launch "); print_int(sta.n_launch - 1);
    print_str(" type="); print_int(lnc_type);
    print_str(" cat="); print_int(cat);
    print_str(" ste="); print_int(site);
    print_str(" tsk="); print_int(task);
    print_str(" msr="); print_int(measure); print_str("\n");
    return sta.n_launch - 1;
}

int sta_ground(int grd_type, int cat, int station, int network, int data, int year) {
    if (sta.n_ground >= MAX_GROUND) return -1;
    ground_t* g = &grounds[sta.n_ground];
    g->ground_id = sta.n_ground;
    g->ground_type = grd_type;
    g->ground_category = cat;
    g->ground_station = station;
    g->measure_network = network;
    g->data_process = data;
    g->year = year;
    g->active = 1;
    sta.total_station += station;
    sta.n_ground++;
    print_str("[STA] Ground "); print_int(sta.n_ground - 1);
    print_str(" type="); print_int(grd_type);
    print_str(" cat="); print_int(cat);
    print_str(" stn="); print_int(station);
    print_str(" ntw="); print_int(network);
    print_str(" dat="); print_int(data); print_str("\n");
    return sta.n_ground - 1;
}

int sta_app(int ap_type, int cat, int sat_com, int sat_nav, int sat_rmt, int year) {
    if (sta.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[sta.n_app];
    a->app_id = sta.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->sat_comm = sat_com;
    a->sat_nav = sat_nav;
    a->sat_remote = sat_rmt;
    a->year = year;
    a->active = 1;
    sta.total_sat_comm += sat_com;
    sta.n_app++;
    print_str("[STA] App "); print_int(sta.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" scc="); print_int(sat_com);
    print_str(" snv="); print_int(sat_nav);
    print_str(" srm="); print_int(sat_rmt); print_str("\n");
    return sta.n_app - 1;
}

void sta_satellite_report(void) {
    print_str("[STA] Satellite report:\n");
    print_str("  Satellite manufacturing categories: "); print_int(sta.n_satellite); print_str("\n");
    print_str("  Total comm satellites: "); print_int(sta.total_comm); print_str("\n");
}

void sta_rocket_report(void) {
    print_str("[STA] Rocket report:\n");
    print_str("  Launch vehicle categories: "); print_int(sta.n_rocket); print_str("\n");
    print_str("  Total solid rockets: "); print_int(sta.total_solid); print_str("\n");
}

void sta_app_report(void) {
    print_str("[STA] App report:\n");
    print_str("  Launch service categories: "); print_int(sta.n_launch); print_str("\n");
    print_str("  Total launch sites: "); print_int(sta.total_site); print_str("\n");
    print_str("  Ground system categories: "); print_int(sta.n_ground); print_str("\n");
    print_str("  Total ground stations: "); print_int(sta.total_station); print_str("\n");
    print_str("  Satellite app categories: "); print_int(sta.n_app); print_str("\n");
    print_str("  Total sat comm: "); print_int(sta.total_sat_comm); print_str("\n");
}

void sta_print_state(void) {
    print_str("[STA] St="); print_int(sta.n_satellite);
    print_str(" Rk="); print_int(sta.n_rocket);
    print_str(" Ln="); print_int(sta.n_launch);
    print_str(" Gr="); print_int(sta.n_ground);
    print_str(" Ap="); print_int(sta.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Satellite Admin Demo ===\n\n");
    sta_init();

    print_str("Satellite manufacturing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int com = 55 + (i * 13);
        int nav = 35 + (i * 8);
        int rmt = 25 + (i * 6);
        int year = 2020 + (i % 5);
        sta_satellite(type, cat, com, nav, rmt, year);
    }

    print_str("\nLaunch vehicles...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sld = 40 + (i * 10);
        int lqd = 30 + (i * 7);
        int rus = 20 + (i * 5);
        int year = 2021 + (i % 4);
        sta_rocket(type, cat, sld, lqd, rus, year);
    }

    print_str("\nLaunch services...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ste = 30 + (i * 8);
        int tsk = 22 + (i * 5);
        int msr = 15 + (i * 3);
        int year = 2022 + (i % 3);
        sta_launch(type, cat, ste, tsk, msr, year);
    }

    print_str("\nGround systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stn = 35 + (i * 9);
        int ntw = 25 + (i * 6);
        int dat = 18 + (i * 4);
        int year = 2023 + (i % 2);
        sta_ground(type, cat, stn, ntw, dat, year);
    }

    print_str("\nSatellite applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int scc = 28 + (i * 7);
        int snv = 20 + (i * 5);
        int srm = 14 + (i * 3);
        int year = 2024;
        sta_app(type, cat, scc, snv, srm, year);
    }

    print_str("\nSatellite report...\n");
    sta_satellite_report();

    print_str("\nRocket report...\n");
    sta_rocket_report();

    print_str("\nApp report...\n");
    sta_app_report();

    print_str("\nFinal state...\n");
    sta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
