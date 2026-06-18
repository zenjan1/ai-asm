/* surveying_engineering_admin: Surveying engineering administration system (v1.0)
 * Geodetic surveying, photogrammetry, engineering surveying, marine surveying, GIS
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

#define MAX_GEO_SV      16
#define MAX_PHT_GR      14
#define MAX_ENG_SV      12
#define MAX_MAR_SV      10
#define MAX_GEO_INF     10

typedef struct {
    int    gs_id;
    int    gs_type;
    int    gs_category;
    int    level_sv;
    int    triang_sv;
    int    gps_sv;
    int    year;
    int    active;
} geo_sv_t;

typedef struct {
    int    pg_id;
    int    pg_type;
    int    pg_category;
    int    aero_photo;
    int    sat_remote;
    int    stereo_map;
    int    year;
    int    active;
} pht_gr_t;

typedef struct {
    int    es_id;
    int    es_type;
    int    es_category;
    int    build_sv;
    int    road_sv;
    int    bridge_sv;
    int    year;
    int    active;
} eng_sv_t;

typedef struct {
    int    ms_id;
    int    ms_type;
    int    ms_category;
    int    channel_sv;
    int    depth_sv;
    int    tide_obs;
    int    year;
    int    active;
} mar_sv_t;

typedef struct {
    int    gi_id;
    int    gi_type;
    int    gi_category;
    int    gis_sys;
    int    spatial_an;
    int    map_carto;
    int    year;
    int    active;
} geo_inf_t;

typedef struct {
    int    n_geo_sv;
    int    n_pht_gr;
    int    n_eng_sv;
    int    n_mar_sv;
    int    n_geo_inf;
    int    total_level_sv;
    int    total_aero_photo;
    int    total_build_sv;
    int    total_channel_sv;
    int    total_gis_sys;
} sea_state_t;

static geo_sv_t geo_svs[MAX_GEO_SV];
static pht_gr_t pht_grs[MAX_PHT_GR];
static eng_sv_t eng_svs[MAX_ENG_SV];
static mar_sv_t mar_svs[MAX_MAR_SV];
static geo_inf_t geo_infs[MAX_GEO_INF];
static sea_state_t sea;

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

int sea_init(void) {
    if (initialized) return -1;
    sea.n_geo_sv = 0; sea.n_pht_gr = 0; sea.n_eng_sv = 0;
    sea.n_mar_sv = 0; sea.n_geo_inf = 0;
    sea.total_level_sv = 0; sea.total_aero_photo = 0;
    sea.total_build_sv = 0; sea.total_channel_sv = 0;
    sea.total_gis_sys = 0;
    for (int i = 0; i < MAX_GEO_SV; i++) geo_svs[i].active = 0;
    for (int i = 0; i < MAX_PHT_GR; i++) pht_grs[i].active = 0;
    for (int i = 0; i < MAX_ENG_SV; i++) eng_svs[i].active = 0;
    for (int i = 0; i < MAX_MAR_SV; i++) mar_svs[i].active = 0;
    for (int i = 0; i < MAX_GEO_INF; i++) geo_infs[i].active = 0;
    initialized = 1;
    print_str("[SEA] Surveying engineering initialized\n");
    return 0;
}

int sea_geo_sv(int gt_type, int cat, int lvs, int trs, int gpv, int year) {
    if (sea.n_geo_sv >= MAX_GEO_SV) return -1;
    geo_sv_t* g = &geo_svs[sea.n_geo_sv];
    g->gs_id = sea.n_geo_sv;
    g->gs_type = gt_type;
    g->gs_category = cat;
    g->level_sv = lvs;
    g->triang_sv = trs;
    g->gps_sv = gpv;
    g->year = year;
    g->active = 1;
    sea.total_level_sv += lvs;
    sea.n_geo_sv++;
    print_str("[SEA] Geo sv "); print_int(sea.n_geo_sv - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lvs="); print_int(lvs);
    print_str(" trs="); print_int(trs);
    print_str(" gpv="); print_int(gpv); print_str("\n");
    return sea.n_geo_sv - 1;
}

int sea_pht_gr(int pt_type, int cat, int aep, int sre, int stm, int year) {
    if (sea.n_pht_gr >= MAX_PHT_GR) return -1;
    pht_gr_t* p = &pht_grs[sea.n_pht_gr];
    p->pg_id = sea.n_pht_gr;
    p->pg_type = pt_type;
    p->pg_category = cat;
    p->aero_photo = aep;
    p->sat_remote = sre;
    p->stereo_map = stm;
    p->year = year;
    p->active = 1;
    sea.total_aero_photo += aep;
    sea.n_pht_gr++;
    print_str("[SEA] Pht gr "); print_int(sea.n_pht_gr - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" aep="); print_int(aep);
    print_str(" sre="); print_int(sre);
    print_str(" stm="); print_int(stm); print_str("\n");
    return sea.n_pht_gr - 1;
}

int sea_eng_sv(int et_type, int cat, int bsv, int rdv, int brv, int year) {
    if (sea.n_eng_sv >= MAX_ENG_SV) return -1;
    eng_sv_t* e = &eng_svs[sea.n_eng_sv];
    e->es_id = sea.n_eng_sv;
    e->es_type = et_type;
    e->es_category = cat;
    e->build_sv = bsv;
    e->road_sv = rdv;
    e->bridge_sv = brv;
    e->year = year;
    e->active = 1;
    sea.total_build_sv += bsv;
    sea.n_eng_sv++;
    print_str("[SEA] Eng sv "); print_int(sea.n_eng_sv - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" bsv="); print_int(bsv);
    print_str(" rdv="); print_int(rdv);
    print_str(" brv="); print_int(brv); print_str("\n");
    return sea.n_eng_sv - 1;
}

int sea_mar_sv(int mt_type, int cat, int chv, int dpv, int tdv, int year) {
    if (sea.n_mar_sv >= MAX_MAR_SV) return -1;
    mar_sv_t* m = &mar_svs[sea.n_mar_sv];
    m->ms_id = sea.n_mar_sv;
    m->ms_type = mt_type;
    m->ms_category = cat;
    m->channel_sv = chv;
    m->depth_sv = dpv;
    m->tide_obs = tdv;
    m->year = year;
    m->active = 1;
    sea.total_channel_sv += chv;
    sea.n_mar_sv++;
    print_str("[SEA] Mar sv "); print_int(sea.n_mar_sv - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" chv="); print_int(chv);
    print_str(" dpv="); print_int(dpv);
    print_str(" tdv="); print_int(tdv); print_str("\n");
    return sea.n_mar_sv - 1;
}

int sea_geo_inf(int gt_type, int cat, int gsv, int span, int mct, int year) {
    if (sea.n_geo_inf >= MAX_GEO_INF) return -1;
    geo_inf_t* g = &geo_infs[sea.n_geo_inf];
    g->gi_id = sea.n_geo_inf;
    g->gi_type = gt_type;
    g->gi_category = cat;
    g->gis_sys = gsv;
    g->spatial_an = span;
    g->map_carto = mct;
    g->year = year;
    g->active = 1;
    sea.total_gis_sys += gsv;
    sea.n_geo_inf++;
    print_str("[SEA] Geo inf "); print_int(sea.n_geo_inf - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gsv="); print_int(gsv);
    print_str(" spa="); print_int(span);
    print_str(" mct="); print_int(mct); print_str("\n");
    return sea.n_geo_inf - 1;
}

void sea_geo_sv_report(void) {
    print_str("[SEA] Geodetic surveying report:\n");
    print_str("  Geodetic surveying categories: "); print_int(sea.n_geo_sv); print_str("\n");
    print_str("  Total level surveying: "); print_int(sea.total_level_sv); print_str("\n");
}

void sea_pht_gr_report(void) {
    print_str("[SEA] Photogrammetry report:\n");
    print_str("  Photogrammetry categories: "); print_int(sea.n_pht_gr); print_str("\n");
    print_str("  Total aerial photography: "); print_int(sea.total_aero_photo); print_str("\n");
}

void sea_full_report(void) {
    print_str("[SEA] Full report:\n");
    print_str("  Engineering surveying categories: "); print_int(sea.n_eng_sv); print_str("\n");
    print_str("  Total building surveying: "); print_int(sea.total_build_sv); print_str("\n");
    print_str("  Marine surveying categories: "); print_int(sea.n_mar_sv); print_str("\n");
    print_str("  Total channel surveying: "); print_int(sea.total_channel_sv); print_str("\n");
    print_str("  Geographic info categories: "); print_int(sea.n_geo_inf); print_str("\n");
    print_str("  Total GIS system: "); print_int(sea.total_gis_sys); print_str("\n");
}

void sea_print_state(void) {
    print_str("[SEA] Gs="); print_int(sea.n_geo_sv);
    print_str(" Pg="); print_int(sea.n_pht_gr);
    print_str(" Es="); print_int(sea.n_eng_sv);
    print_str(" Ms="); print_int(sea.n_mar_sv);
    print_str(" Gi="); print_int(sea.n_geo_inf);
    print_str("\n");
}

int main(void) {
    print_str("=== Surveying Engineering Admin Demo ===\n\n");
    sea_init();

    print_str("Geodetic surveying...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lvs = 55 + (i * 13);
        int trs = 40 + (i * 10);
        int gpv = 22 + (i * 5);
        int year = 2020 + (i % 5);
        sea_geo_sv(type, cat, lvs, trs, gpv, year);
    }

    print_str("\nPhotogrammetry...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int aep = 48 + (i * 11);
        int sre = 35 + (i * 8);
        int stm = 20 + (i * 4);
        int year = 2021 + (i % 4);
        sea_pht_gr(type, cat, aep, sre, stm, year);
    }

    print_str("\nEngineering surveying...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bsv = 42 + (i * 10);
        int rdv = 28 + (i * 7);
        int brv = 18 + (i * 4);
        int year = 2022 + (i % 3);
        sea_eng_sv(type, cat, bsv, rdv, brv, year);
    }

    print_str("\nMarine surveying...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chv = 35 + (i * 8);
        int dpv = 25 + (i * 6);
        int tdv = 15 + (i * 3);
        int year = 2023 + (i % 2);
        sea_mar_sv(type, cat, chv, dpv, tdv, year);
    }

    print_str("\nGeographic info...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gsv = 30 + (i * 7);
        int span = 22 + (i * 5);
        int mct = 12 + (i * 3);
        int year = 2024;
        sea_geo_inf(type, cat, gsv, span, mct, year);
    }

    print_str("\nGeodetic surveying report...\n");
    sea_geo_sv_report();

    print_str("\nPhotogrammetry report...\n");
    sea_pht_gr_report();

    print_str("\nFull report...\n");
    sea_full_report();

    print_str("\nFinal state...\n");
    sea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
