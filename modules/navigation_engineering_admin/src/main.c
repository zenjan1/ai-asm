/* navigation_engineering_admin: Navigation engineering administration system (v1.0)
 * Satellite navigation, inertial navigation, integrated navigation, map matching, terminals
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

#define MAX_SAT_NAV     16
#define MAX_INR_NAV     14
#define MAX_INT_NAV     12
#define MAX_MAP_MT      10
#define MAX_NAV_TM      10

typedef struct {
    int    sn_id;
    int    sn_type;
    int    sn_category;
    int    gps_nav;
    int    beidou_nav;
    int    glonass_nav;
    int    year;
    int    active;
} sat_nav_t;

typedef struct {
    int    in_id;
    int    in_type;
    int    in_category;
    int    gyroscope;
    int    accelerom;
    int    imu_unit;
    int    year;
    int    active;
} inr_nav_t;

typedef struct {
    int    ig_id;
    int    ig_type;
    int    ig_category;
    int    ins_gps;
    int    multi_fuse;
    int    filter_algo;
    int    year;
    int    active;
} int_nav_t;

typedef struct {
    int    mm_id;
    int    mm_type;
    int    mm_category;
    int    terrain_mt;
    int    scene_mt;
    int    geomag_mt;
    int    year;
    int    active;
} map_mt_t;

typedef struct {
    int    nt_id;
    int    nt_type;
    int    nt_category;
    int    vehicle_nav;
    int    ship_nav;
    int    aircraft_nav;
    int    year;
    int    active;
} nav_tm_t;

typedef struct {
    int    n_sat_nav;
    int    n_inr_nav;
    int    n_int_nav;
    int    n_map_mt;
    int    n_nav_tm;
    int    total_gps_nav;
    int    total_gyroscope;
    int    total_ins_gps;
    int    total_terrain_mt;
    int    total_vehicle_nav;
} nea_state_t;

static sat_nav_t sat_navs[MAX_SAT_NAV];
static inr_nav_t inr_navs[MAX_INR_NAV];
static int_nav_t int_navs[MAX_INT_NAV];
static map_mt_t map_mts[MAX_MAP_MT];
static nav_tm_t nav_tms[MAX_NAV_TM];
static nea_state_t nea;

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

int nea_init(void) {
    if (initialized) return -1;
    nea.n_sat_nav = 0; nea.n_inr_nav = 0; nea.n_int_nav = 0;
    nea.n_map_mt = 0; nea.n_nav_tm = 0;
    nea.total_gps_nav = 0; nea.total_gyroscope = 0;
    nea.total_ins_gps = 0; nea.total_terrain_mt = 0;
    nea.total_vehicle_nav = 0;
    for (int i = 0; i < MAX_SAT_NAV; i++) sat_navs[i].active = 0;
    for (int i = 0; i < MAX_INR_NAV; i++) inr_navs[i].active = 0;
    for (int i = 0; i < MAX_INT_NAV; i++) int_navs[i].active = 0;
    for (int i = 0; i < MAX_MAP_MT; i++) map_mts[i].active = 0;
    for (int i = 0; i < MAX_NAV_TM; i++) nav_tms[i].active = 0;
    initialized = 1;
    print_str("[NEA] Navigation engineering initialized\n");
    return 0;
}

int nea_sat_nav(int st_type, int cat, int gps, int bds, int glo, int year) {
    if (nea.n_sat_nav >= MAX_SAT_NAV) return -1;
    sat_nav_t* s = &sat_navs[nea.n_sat_nav];
    s->sn_id = nea.n_sat_nav;
    s->sn_type = st_type;
    s->sn_category = cat;
    s->gps_nav = gps;
    s->beidou_nav = bds;
    s->glonass_nav = glo;
    s->year = year;
    s->active = 1;
    nea.total_gps_nav += gps;
    nea.n_sat_nav++;
    print_str("[NEA] Sat nav "); print_int(nea.n_sat_nav - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" gps="); print_int(gps);
    print_str(" bds="); print_int(bds);
    print_str(" glo="); print_int(glo); print_str("\n");
    return nea.n_sat_nav - 1;
}

int nea_inr_nav(int it_type, int cat, int gyr, int acc, int imu_v, int year) {
    if (nea.n_inr_nav >= MAX_INR_NAV) return -1;
    inr_nav_t* n = &inr_navs[nea.n_inr_nav];
    n->in_id = nea.n_inr_nav;
    n->in_type = it_type;
    n->in_category = cat;
    n->gyroscope = gyr;
    n->accelerom = acc;
    n->imu_unit = imu_v;
    n->year = year;
    n->active = 1;
    nea.total_gyroscope += gyr;
    nea.n_inr_nav++;
    print_str("[NEA] Inr nav "); print_int(nea.n_inr_nav - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" gyr="); print_int(gyr);
    print_str(" acc="); print_int(acc);
    print_str(" imu="); print_int(imu_v); print_str("\n");
    return nea.n_inr_nav - 1;
}

int nea_int_nav(int ig_type, int cat, int igp, int mtf, int fla, int year) {
    if (nea.n_int_nav >= MAX_INT_NAV) return -1;
    int_nav_t* n = &int_navs[nea.n_int_nav];
    n->ig_id = nea.n_int_nav;
    n->ig_type = ig_type;
    n->ig_category = cat;
    n->ins_gps = igp;
    n->multi_fuse = mtf;
    n->filter_algo = fla;
    n->year = year;
    n->active = 1;
    nea.total_ins_gps += igp;
    nea.n_int_nav++;
    print_str("[NEA] Int nav "); print_int(nea.n_int_nav - 1);
    print_str(" type="); print_int(ig_type);
    print_str(" cat="); print_int(cat);
    print_str(" igp="); print_int(igp);
    print_str(" mtf="); print_int(mtf);
    print_str(" fla="); print_int(fla); print_str("\n");
    return nea.n_int_nav - 1;
}

int nea_map_mt(int mt_type, int cat, int trm, int scm, int gem, int year) {
    if (nea.n_map_mt >= MAX_MAP_MT) return -1;
    map_mt_t* m = &map_mts[nea.n_map_mt];
    m->mm_id = nea.n_map_mt;
    m->mm_type = mt_type;
    m->mm_category = cat;
    m->terrain_mt = trm;
    m->scene_mt = scm;
    m->geomag_mt = gem;
    m->year = year;
    m->active = 1;
    nea.total_terrain_mt += trm;
    nea.n_map_mt++;
    print_str("[NEA] Map mt "); print_int(nea.n_map_mt - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trm="); print_int(trm);
    print_str(" scm="); print_int(scm);
    print_str(" gem="); print_int(gem); print_str("\n");
    return nea.n_map_mt - 1;
}

int nea_nav_tm(int nt_type, int cat, int vcl, int shp, int acf, int year) {
    if (nea.n_nav_tm >= MAX_NAV_TM) return -1;
    nav_tm_t* n = &nav_tms[nea.n_nav_tm];
    n->nt_id = nea.n_nav_tm;
    n->nt_type = nt_type;
    n->nt_category = cat;
    n->vehicle_nav = vcl;
    n->ship_nav = shp;
    n->aircraft_nav = acf;
    n->year = year;
    n->active = 1;
    nea.total_vehicle_nav += vcl;
    nea.n_nav_tm++;
    print_str("[NEA] Nav tm "); print_int(nea.n_nav_tm - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" vcl="); print_int(vcl);
    print_str(" shp="); print_int(shp);
    print_str(" acf="); print_int(acf); print_str("\n");
    return nea.n_nav_tm - 1;
}

void nea_sat_nav_report(void) {
    print_str("[NEA] Satellite navigation report:\n");
    print_str("  Satellite navigation categories: "); print_int(nea.n_sat_nav); print_str("\n");
    print_str("  Total GPS navigation: "); print_int(nea.total_gps_nav); print_str("\n");
}

void nea_inr_nav_report(void) {
    print_str("[NEA] Inertial navigation report:\n");
    print_str("  Inertial navigation categories: "); print_int(nea.n_inr_nav); print_str("\n");
    print_str("  Total gyroscope: "); print_int(nea.total_gyroscope); print_str("\n");
}

void nea_full_report(void) {
    print_str("[NEA] Full report:\n");
    print_str("  Integrated navigation categories: "); print_int(nea.n_int_nav); print_str("\n");
    print_str("  Total INS/GPS: "); print_int(nea.total_ins_gps); print_str("\n");
    print_str("  Map matching categories: "); print_int(nea.n_map_mt); print_str("\n");
    print_str("  Total terrain matching: "); print_int(nea.total_terrain_mt); print_str("\n");
    print_str("  Navigation terminal categories: "); print_int(nea.n_nav_tm); print_str("\n");
    print_str("  Total vehicle navigation: "); print_int(nea.total_vehicle_nav); print_str("\n");
}

void nea_print_state(void) {
    print_str("[NEA] Sn="); print_int(nea.n_sat_nav);
    print_str(" In="); print_int(nea.n_inr_nav);
    print_str(" Ig="); print_int(nea.n_int_nav);
    print_str(" Mm="); print_int(nea.n_map_mt);
    print_str(" Nt="); print_int(nea.n_nav_tm);
    print_str("\n");
}

int main(void) {
    print_str("=== Navigation Engineering Admin Demo ===\n\n");
    nea_init();

    print_str("Satellite navigation...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int gps = 55 + (i * 13);
        int bds = 40 + (i * 10);
        int glo = 22 + (i * 5);
        int year = 2020 + (i % 5);
        nea_sat_nav(type, cat, gps, bds, glo, year);
    }

    print_str("\nInertial navigation...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gyr = 48 + (i * 11);
        int acc = 35 + (i * 8);
        int imu_v = 20 + (i * 4);
        int year = 2021 + (i % 4);
        nea_inr_nav(type, cat, gyr, acc, imu_v, year);
    }

    print_str("\nIntegrated navigation...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int igp = 42 + (i * 10);
        int mtf = 28 + (i * 7);
        int fla = 18 + (i * 4);
        int year = 2022 + (i % 3);
        nea_int_nav(type, cat, igp, mtf, fla, year);
    }

    print_str("\nMap matching...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trm = 35 + (i * 8);
        int scm = 25 + (i * 6);
        int gem = 15 + (i * 3);
        int year = 2023 + (i % 2);
        nea_map_mt(type, cat, trm, scm, gem, year);
    }

    print_str("\nNavigation terminal...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vcl = 30 + (i * 7);
        int shp = 22 + (i * 5);
        int acf = 12 + (i * 3);
        int year = 2024;
        nea_nav_tm(type, cat, vcl, shp, acf, year);
    }

    print_str("\nSatellite navigation report...\n");
    nea_sat_nav_report();

    print_str("\nInertial navigation report...\n");
    nea_inr_nav_report();

    print_str("\nFull report...\n");
    nea_full_report();

    print_str("\nFinal state...\n");
    nea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
