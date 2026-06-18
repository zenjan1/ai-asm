/* geological_survey: Geological survey management (v1.0)
 * Surveys, minerals, hazards, groundwater, earthquakes
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

#define MAX_SURVEYS    12
#define MAX_MINERALS   14
#define MAX_HAZARDS    16
#define MAX_GROUNDWATER 10
#define MAX_QUAKES     12

typedef struct {
    int    survey_id;
    int    zone;
    int    type;
    int    area_sqkm;
    int    n_samples;
    int    year;
    int    status;
    int    active;
} survey_t;

typedef struct {
    int    mineral_id;
    int    type;
    int    zone;
    int    reserve;
    int    grade;
    int    permit_id;
    int    year_discovered;
    int    active;
} mineral_t;

typedef struct {
    int    hazard_id;
    int    type;
    int    zone;
    int    risk_level;
    int    affected_area;
    int    monitored;
    int    year;
    int    active;
} hazard_t;

typedef struct {
    int    gw_id;
    int    zone;
    int    depth_m;
    int    water_level;
    int    quality;
    int    recharge_rate;
    int    year;
    int    active;
} groundwater_t;

typedef struct {
    int    quake_id;
    int    zone;
    int    magnitude;
    int    depth_km;
    int    intensity;
    int    year;
    int    month;
    int    day;
    int    hour;
    int    active;
} quake_t;

typedef struct {
    int    n_surveys;
    int    n_minerals;
    int    n_hazards;
    int    n_groundwater;
    int    n_quakes;
    int    total_area_surveyed;
    int    total_samples;
    int    total_reserves;
    int    total_hazard_zones;
    int    max_magnitude;
    int    total_quakes;
} gs_state_t;

static survey_t surveys[MAX_SURVEYS];
static mineral_t minerals[MAX_MINERALS];
static hazard_t hazards[MAX_HAZARDS];
static groundwater_t groundwater[MAX_GROUNDWATER];
static quake_t quakes[MAX_QUAKES];
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
    gs.n_surveys = 0; gs.n_minerals = 0; gs.n_hazards = 0;
    gs.n_groundwater = 0; gs.n_quakes = 0;
    gs.total_area_surveyed = 0; gs.total_samples = 0;
    gs.total_reserves = 0; gs.total_hazard_zones = 0;
    gs.max_magnitude = 0; gs.total_quakes = 0;
    for (int i = 0; i < MAX_SURVEYS; i++) surveys[i].active = 0;
    for (int i = 0; i < MAX_MINERALS; i++) minerals[i].active = 0;
    for (int i = 0; i < MAX_HAZARDS; i++) hazards[i].active = 0;
    for (int i = 0; i < MAX_GROUNDWATER; i++) groundwater[i].active = 0;
    for (int i = 0; i < MAX_QUAKES; i++) quakes[i].active = 0;
    initialized = 1;
    print_str("[GS] Geological survey initialized\n");
    return 0;
}

int gs_add_survey(int zone, int type, int area, int samples, int year) {
    if (gs.n_surveys >= MAX_SURVEYS) return -1;
    survey_t* s = &surveys[gs.n_surveys];
    s->survey_id = gs.n_surveys;
    s->zone = zone;
    s->type = type;
    s->area_sqkm = area;
    s->n_samples = samples;
    s->year = year;
    s->status = 1;
    s->active = 1;
    gs.total_area_surveyed += area;
    gs.total_samples += samples;
    gs.n_surveys++;
    print_str("[GS] Survey "); print_int(gs.n_surveys - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(type);
    print_str(" area="); print_int(area);
    print_str(" samples="); print_int(samples); print_str("\n");
    return gs.n_surveys - 1;
}

int gs_complete_survey(int survey_id) {
    if (survey_id >= gs.n_surveys) return -1;
    surveys[survey_id].status = 2;
    print_str("[GS] Complete Sv"); print_int(survey_id); print_str("\n");
    return 0;
}

int gs_discover_mineral(int type, int zone, int reserve, int grade, int year) {
    if (gs.n_minerals >= MAX_MINERALS) return -1;
    mineral_t* m = &minerals[gs.n_minerals];
    m->mineral_id = gs.n_minerals;
    m->type = type;
    m->zone = zone;
    m->reserve = reserve;
    m->grade = grade;
    m->permit_id = -1;
    m->year_discovered = year;
    m->active = 1;
    gs.total_reserves += reserve;
    gs.n_minerals++;
    print_str("[GS] Mineral "); print_int(gs.n_minerals - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" res="); print_int(reserve);
    print_str(" grade="); print_int(grade); print_str("%\n");
    return gs.n_minerals - 1;
}

int gs_issue_permit(int mineral_id, int permit_id) {
    if (mineral_id >= gs.n_minerals) return -1;
    minerals[mineral_id].permit_id = permit_id;
    print_str("[GS] Permit M"); print_int(mineral_id);
    print_str(" ="); print_int(permit_id); print_str("\n");
    return 0;
}

int gs_register_hazard(int type, int zone, int risk, int area, int year) {
    if (gs.n_hazards >= MAX_HAZARDS) return -1;
    hazard_t* h = &hazards[gs.n_hazards];
    h->hazard_id = gs.n_hazards;
    h->type = type;
    h->zone = zone;
    h->risk_level = risk;
    h->affected_area = area;
    h->monitored = 0;
    h->year = year;
    h->active = 1;
    gs.total_hazard_zones++;
    gs.n_hazards++;
    print_str("[GS] Hazard "); print_int(gs.n_hazards - 1);
    print_str(" type="); print_int(type);
    print_str(" zone="); print_int(zone);
    print_str(" risk="); print_int(risk);
    print_str(" area="); print_int(area); print_str("\n");
    return gs.n_hazards - 1;
}

int gs_monitor_hazard(int hazard_id) {
    if (hazard_id >= gs.n_hazards) return -1;
    hazards[hazard_id].monitored = 1;
    print_str("[GS] Monitor H"); print_int(hazard_id); print_str("\n");
    return 0;
}

int gs_add_groundwater(int zone, int depth, int level, int quality, int recharge, int year) {
    if (gs.n_groundwater >= MAX_GROUNDWATER) return -1;
    groundwater_t* g = &groundwater[gs.n_groundwater];
    g->gw_id = gs.n_groundwater;
    g->zone = zone;
    g->depth_m = depth;
    g->water_level = level;
    g->quality = quality;
    g->recharge_rate = recharge;
    g->year = year;
    g->active = 1;
    gs.n_groundwater++;
    print_str("[GS] GW "); print_int(gs.n_groundwater - 1);
    print_str(" zone="); print_int(zone);
    print_str(" depth="); print_int(depth);
    print_str(" lvl="); print_int(level);
    print_str(" qual="); print_int(quality); print_str("\n");
    return gs.n_groundwater - 1;
}

int gs_record_quake(int zone, int mag, int depth, int intensity, int year, int month, int day, int hour) {
    if (gs.n_quakes >= MAX_QUAKES) return -1;
    quake_t* q = &quakes[gs.n_quakes];
    q->quake_id = gs.n_quakes;
    q->zone = zone;
    q->magnitude = mag;
    q->depth_km = depth;
    q->intensity = intensity;
    q->year = year;
    q->month = month;
    q->day = day;
    q->hour = hour;
    q->active = 1;
    gs.total_quakes++;
    if (mag > gs.max_magnitude) gs.max_magnitude = mag;
    gs.n_quakes++;
    print_str("[GS] Quake "); print_int(gs.n_quakes - 1);
    print_str(" zone="); print_int(zone);
    print_str(" M"); print_int(mag / 10); print_str("."); print_int(mag % 10);
    print_str(" depth="); print_int(depth);
    print_str(" I="); print_int(intensity); print_str("\n");
    return gs.n_quakes - 1;
}

void gs_survey_report(void) {
    print_str("[GS] Survey report:\n");
    print_str("  Area surveyed: "); print_int(gs.total_area_surveyed); print_str(" sqkm\n");
    print_str("  Samples: "); print_int(gs.total_samples); print_str("\n");
}

void gs_mineral_report(void) {
    print_str("[GS] Mineral report:\n");
    print_str("  Deposits: "); print_int(gs.n_minerals); print_str("\n");
    print_str("  Total reserves: "); print_int(gs.total_reserves); print_str("\n");
}

void gs_hazard_report(void) {
    print_str("[GS] Hazard report:\n");
    print_str("  Hazard zones: "); print_int(gs.total_hazard_zones); print_str("\n");
    for (int i = 0; i < gs.n_hazards; i++) {
        hazard_t* h = &hazards[i];
        print_str("  H"); print_int(i);
        print_str(" type="); print_int(h->type);
        print_str(" zone="); print_int(h->zone);
        print_str(" risk="); print_int(h->risk_level);
        print_str(" mon="); print_int(h->monitored); print_str("\n");
    }
}

void gs_quake_report(void) {
    print_str("[GS] Quake report:\n");
    print_str("  Total quakes: "); print_int(gs.total_quakes); print_str("\n");
    print_str("  Max magnitude: "); print_int(gs.max_magnitude / 10); print_str("."); print_int(gs.max_magnitude % 10); print_str("\n");
}

void gs_print_state(void) {
    print_str("[GS] Surv="); print_int(gs.n_surveys);
    print_str(" Min="); print_int(gs.n_minerals);
    print_str(" Haz="); print_int(gs.n_hazards);
    print_str(" GW="); print_int(gs.n_groundwater);
    print_str(" Qk="); print_int(gs.n_quakes);
    print_str("\n");
}

int main(void) {
    print_str("=== Geological Survey Demo ===\n\n");
    gs_init();

    print_str("Adding surveys...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int type = (i % 3) + 1;
        int area = 100 + (i * 50);
        int samples = 50 + (i * 20);
        int year = 2020 + (i % 5);
        gs_add_survey(zone, type, area, samples, year);
    }

    print_str("\nCompleting surveys...\n");
    for (int i = 0; i < 12; i++) {
        gs_complete_survey(i);
    }

    print_str("\nDiscovering minerals...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int zone = (i % 4) + 1;
        int reserve = 10000 + (i * 5000);
        int grade = 20 + (i * 5);
        int year = 2018 + (i % 7);
        gs_discover_mineral(type, zone, reserve, grade, year);
    }

    print_str("\nIssuing permits...\n");
    for (int i = 0; i < 10; i++) {
        gs_issue_permit(i, 100 + i);
    }

    print_str("\nRegistering hazards...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 4) + 1;
        int zone = (i % 4) + 1;
        int risk = (i % 5) + 1;
        int area = 50 + (i * 30);
        int year = 2020 + (i % 5);
        gs_register_hazard(type, zone, risk, area, year);
    }

    print_str("\nMonitoring hazards...\n");
    for (int i = 0; i < 16; i++) {
        gs_monitor_hazard(i);
    }

    print_str("\nAdding groundwater...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 4) + 1;
        int depth = 50 + (i * 20);
        int level = 20 + (i * 5);
        int quality = 70 + (i * 3);
        int recharge = 10 + (i * 2);
        int year = 2024;
        gs_add_groundwater(zone, depth, level, quality, recharge, year);
    }

    print_str("\nRecording earthquakes...\n");
    for (int i = 0; i < 12; i++) {
        int zone = (i % 4) + 1;
        int mag = 20 + (i * 5);
        int depth = 5 + (i * 3);
        int intensity = (i % 6) + 1;
        int year = 2023 + (i % 2);
        int month = 1 + (i % 12);
        int day = 1 + (i % 28);
        int hour = i * 2;
        gs_record_quake(zone, mag, depth, intensity, year, month, day, hour);
    }

    print_str("\nSurvey report...\n");
    gs_survey_report();

    print_str("\nMineral report...\n");
    gs_mineral_report();

    print_str("\nHazard report...\n");
    gs_hazard_report();

    print_str("\nQuake report...\n");
    gs_quake_report();

    print_str("\nFinal state...\n");
    gs_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
