/* geological_survey: Geological survey administration system (v1.0)
 * Regional survey, mineral exploration, disaster prevention, environmental monitoring, information services
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

#define MAX_REGIONAL     16
#define MAX_MINERAL      14
#define MAX_DISASTER     12
#define MAX_ENVIRONMENT  10
#define MAX_INFORMATION  10

typedef struct {
    int    regional_id;
    int    survey_type;
    int    area_id;
    int    area_sqkm;
    int    samples_collected;
    int    mapping_scale;
    int    year;
    int    active;
} regional_t;

typedef struct {
    int    mineral_id;
    int    mineral_type;
    int    deposit_type;
    int    reserves;
    int    grade;
    int    mining_permits;
    int    year;
    int    active;
} mineral_t;

typedef struct {
    int    disaster_id;
    int    disaster_type;
    int    risk_level;
    int    monitoring_stations;
    int    warnings_issued;
    int    emergency_responses;
    int    year;
    int    active;
} disaster_t;

typedef struct {
    int    environment_id;
    int    monitoring_type;
    int    site_id;
    int    sampling_points;
    int    pollution_index;
    int    restoration_projects;
    int    year;
    int    active;
} environment_t;

typedef struct {
    int    information_id;
    int    data_type;
    int    service_type;
    int    datasets;
    int    maps_published;
    int    consultations;
    int    year;
    int    active;
} information_t;

typedef struct {
    int    n_regional;
    int    n_mineral;
    int    n_disaster;
    int    n_environment;
    int    n_information;
    int    total_area;
    int    total_reserves;
    int    total_warnings;
    int    total_restoration;
    int    total_datasets;
} gs_state_t;

static regional_t regionals[MAX_REGIONAL];
static mineral_t minerals[MAX_MINERAL];
static disaster_t disasters[MAX_DISASTER];
static environment_t environments[MAX_ENVIRONMENT];
static information_t informations[MAX_INFORMATION];
static gs_state_t gs;

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

int gs_init(void) {
    if (initialized) return -1;
    gs.n_regional = 0; gs.n_mineral = 0; gs.n_disaster = 0;
    gs.n_environment = 0; gs.n_information = 0;
    gs.total_area = 0; gs.total_reserves = 0;
    gs.total_warnings = 0; gs.total_restoration = 0;
    gs.total_datasets = 0;
    for (int i = 0; i < MAX_REGIONAL; i++) regionals[i].active = 0;
    for (int i = 0; i < MAX_MINERAL; i++) minerals[i].active = 0;
    for (int i = 0; i < MAX_DISASTER; i++) disasters[i].active = 0;
    for (int i = 0; i < MAX_ENVIRONMENT; i++) environments[i].active = 0;
    for (int i = 0; i < MAX_INFORMATION; i++) informations[i].active = 0;
    initialized = 1;
    print_str("[GS] Geological survey initialized\n");
    return 0;
}

int gs_regional(int survey_type, int area, int sqkm, int samples, int scale, int year) {
    if (gs.n_regional >= MAX_REGIONAL) return -1;
    regional_t* r = &regionals[gs.n_regional];
    r->regional_id = gs.n_regional;
    r->survey_type = survey_type;
    r->area_id = area;
    r->area_sqkm = sqkm;
    r->samples_collected = samples;
    r->mapping_scale = scale;
    r->year = year;
    r->active = 1;
    gs.total_area += sqkm;
    gs.n_regional++;
    print_str("[GS] Regional "); print_int(gs.n_regional - 1);
    print_str(" type="); print_int(survey_type);
    print_str(" area="); print_int(area);
    print_str(" sqkm="); print_int(sqkm);
    print_str(" smp="); print_int(samples);
    print_str(" scl="); print_int(scale); print_str("\n");
    return gs.n_regional - 1;
}

int gs_mineral(int min_type, int dep_type, int reserves, int grade, int permits, int year) {
    if (gs.n_mineral >= MAX_MINERAL) return -1;
    mineral_t* m = &minerals[gs.n_mineral];
    m->mineral_id = gs.n_mineral;
    m->mineral_type = min_type;
    m->deposit_type = dep_type;
    m->reserves = reserves;
    m->grade = grade;
    m->mining_permits = permits;
    m->year = year;
    m->active = 1;
    gs.total_reserves += reserves;
    gs.n_mineral++;
    print_str("[GS] Mineral "); print_int(gs.n_mineral - 1);
    print_str(" type="); print_int(min_type);
    print_str(" dep="); print_int(dep_type);
    print_str(" res="); print_int(reserves);
    print_str(" grd="); print_int(grade); print_str("%");
    print_str(" prmt="); print_int(permits); print_str("\n");
    return gs.n_mineral - 1;
}

int gs_disaster(int dis_type, int risk, int stations, int warnings, int responses, int year) {
    if (gs.n_disaster >= MAX_DISASTER) return -1;
    disaster_t* d = &disasters[gs.n_disaster];
    d->disaster_id = gs.n_disaster;
    d->disaster_type = dis_type;
    d->risk_level = risk;
    d->monitoring_stations = stations;
    d->warnings_issued = warnings;
    d->emergency_responses = responses;
    d->year = year;
    d->active = 1;
    gs.total_warnings += warnings;
    gs.n_disaster++;
    print_str("[GS] Disaster "); print_int(gs.n_disaster - 1);
    print_str(" type="); print_int(dis_type);
    print_str(" risk="); print_int(risk);
    print_str(" stn="); print_int(stations);
    print_str(" wrn="); print_int(warnings);
    print_str(" rsp="); print_int(responses); print_str("\n");
    return gs.n_disaster - 1;
}

int gs_environment(int mon_type, int site, int points, int pollution, int restoration, int year) {
    if (gs.n_environment >= MAX_ENVIRONMENT) return -1;
    environment_t* e = &environments[gs.n_environment];
    e->environment_id = gs.n_environment;
    e->monitoring_type = mon_type;
    e->site_id = site;
    e->sampling_points = points;
    e->pollution_index = pollution;
    e->restoration_projects = restoration;
    e->year = year;
    e->active = 1;
    gs.total_restoration += restoration;
    gs.n_environment++;
    print_str("[GS] Environment "); print_int(gs.n_environment - 1);
    print_str(" type="); print_int(mon_type);
    print_str(" site="); print_int(site);
    print_str(" pts="); print_int(points);
    print_str(" pol="); print_int(pollution);
    print_str(" rst="); print_int(restoration); print_str("\n");
    return gs.n_environment - 1;
}

int gs_information(int data_type, int svc_type, int datasets, int maps, int consultations, int year) {
    if (gs.n_information >= MAX_INFORMATION) return -1;
    information_t* inf = &informations[gs.n_information];
    inf->information_id = gs.n_information;
    inf->data_type = data_type;
    inf->service_type = svc_type;
    inf->datasets = datasets;
    inf->maps_published = maps;
    inf->consultations = consultations;
    inf->year = year;
    inf->active = 1;
    gs.total_datasets += datasets;
    gs.n_information++;
    print_str("[GS] Information "); print_int(gs.n_information - 1);
    print_str(" type="); print_int(data_type);
    print_str(" svc="); print_int(svc_type);
    print_str(" dts="); print_int(datasets);
    print_str(" maps="); print_int(maps);
    print_str(" csl="); print_int(consultations); print_str("\n");
    return gs.n_information - 1;
}

void gs_regional_report(void) {
    print_str("[GS] Regional survey report:\n");
    print_str("  Survey areas: "); print_int(gs.n_regional); print_str("\n");
    print_str("  Total area surveyed: "); print_int(gs.total_area); print_str(" sqkm\n");
}

void gs_mineral_report(void) {
    print_str("[GS] Mineral resources report:\n");
    print_str("  Mineral deposits: "); print_int(gs.n_mineral); print_str("\n");
    print_str("  Total reserves: "); print_int(gs.total_reserves); print_str("\n");
}

void gs_disaster_report(void) {
    print_str("[GS] Disaster prevention report:\n");
    print_str("  Disaster zones: "); print_int(gs.n_disaster); print_str("\n");
    print_str("  Total warnings: "); print_int(gs.total_warnings); print_str("\n");
    print_str("  Environmental sites: "); print_int(gs.n_environment); print_str("\n");
    print_str("  Total restoration: "); print_int(gs.total_restoration); print_str("\n");
    print_str("  Information services: "); print_int(gs.n_information); print_str("\n");
    print_str("  Total datasets: "); print_int(gs.total_datasets); print_str("\n");
}

void gs_print_state(void) {
    print_str("[GS] Rg="); print_int(gs.n_regional);
    print_str(" Mn="); print_int(gs.n_mineral);
    print_str(" Ds="); print_int(gs.n_disaster);
    print_str(" Ev="); print_int(gs.n_environment);
    print_str(" Inf="); print_int(gs.n_information);
    print_str("\n");
}

int main(void) {
    print_str("=== Geological Survey Demo ===\n\n");
    gs_init();

    print_str("Regional surveys...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int area = (i % 8) + 1;
        int sqkm = 1000 + (i * 500);
        int smp = 500 + (i * 100);
        int scl = 50000 + (i * 10000);
        int year = 2020 + (i % 5);
        gs_regional(type, area, sqkm, smp, scl, year);
    }

    print_str("\nMineral exploration...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 6) + 1;
        int dep = (i % 4) + 1;
        int res = 100000 + (i * 50000);
        int grd = 30 + (i * 4);
        int prmt = 5 + (i % 8);
        int year = 2021 + (i % 4);
        gs_mineral(type, dep, res, grd, prmt, year);
    }

    print_str("\nDisaster prevention...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 5) + 1;
        int risk = (i % 5) + 1;
        int stn = 20 + (i * 5);
        int wrn = 10 + (i * 3);
        int rsp = wrn - (i % 3);
        int year = 2022 + (i % 3);
        gs_disaster(type, risk, stn, wrn, rsp, year);
    }

    print_str("\nEnvironmental monitoring...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int site = 100 + (i * 11);
        int pts = 50 + (i * 10);
        int pol = 20 + (i * 5);
        int rst = 3 + (i % 5);
        int year = 2023 + (i % 2);
        gs_environment(type, site, pts, pol, rst, year);
    }

    print_str("\nInformation services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int svc = (i % 4) + 1;
        int dts = 100 + (i * 30);
        int maps = 20 + (i * 5);
        int csl = 50 + (i * 10);
        int year = 2024;
        gs_information(type, svc, dts, maps, csl, year);
    }

    print_str("\nRegional report...\n");
    gs_regional_report();

    print_str("\nMineral report...\n");
    gs_mineral_report();

    print_str("\nDisaster report...\n");
    gs_disaster_report();

    print_str("\nFinal state...\n");
    gs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
