/* ocean_admin: Ocean administration system (v1.0)
 * Ocean planning, environmental protection, ocean economy, ocean technology, maritime rights
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

#define MAX_PLANNING     16
#define MAX_ENVIRONMENT  14
#define MAX_ECONOMY      12
#define MAX_TECHNOLOGY   10
#define MAX_RIGHTS       10

typedef struct {
    int    planning_id;
    int    zone_type;
    int    sea_area_id;
    int    area_sqkm;
    int    usage_permits;
    int    island_protection;
    int    year;
    int    active;
} planning_t;

typedef struct {
    int    environment_id;
    int    monitoring_type;
    int    sea_region;
    int    monitoring_stations;
    int    pollution_index;
    int    restoration_projects;
    int    year;
    int    active;
} environment_t;

typedef struct {
    int    economy_id;
    int    industry_type;
    int    sector;
    int    output_value;
    int    employment;
    int    investment;
    int    year;
    int    active;
} economy_t;

typedef struct {
    int    technology_id;
    int    research_type;
    int    exploration_area;
    int    surveys_conducted;
    int    deep_sea_missions;
    int    polar_expeditions;
    int    year;
    int    active;
} technology_t;

typedef struct {
    int    rights_id;
    int    maritime_zone;
    int    delimitation_type;
    int    negotiations;
    int    agreements_signed;
    int    enforcement_actions;
    int    year;
    int    active;
} rights_t;

typedef struct {
    int    n_planning;
    int    n_environment;
    int    n_economy;
    int    n_technology;
    int    n_rights;
    int    total_area;
    int    total_stations;
    int    total_output;
    int    total_surveys;
    int    total_agreements;
} oa_state_t;

static planning_t plannings[MAX_PLANNING];
static environment_t environments[MAX_ENVIRONMENT];
static economy_t economies[MAX_ECONOMY];
static technology_t technologies[MAX_TECHNOLOGY];
static rights_t rights[MAX_RIGHTS];
static oa_state_t oa;

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

int oa_init(void) {
    if (initialized) return -1;
    oa.n_planning = 0; oa.n_environment = 0; oa.n_economy = 0;
    oa.n_technology = 0; oa.n_rights = 0;
    oa.total_area = 0; oa.total_stations = 0;
    oa.total_output = 0; oa.total_surveys = 0;
    oa.total_agreements = 0;
    for (int i = 0; i < MAX_PLANNING; i++) plannings[i].active = 0;
    for (int i = 0; i < MAX_ENVIRONMENT; i++) environments[i].active = 0;
    for (int i = 0; i < MAX_ECONOMY; i++) economies[i].active = 0;
    for (int i = 0; i < MAX_TECHNOLOGY; i++) technologies[i].active = 0;
    for (int i = 0; i < MAX_RIGHTS; i++) rights[i].active = 0;
    initialized = 1;
    print_str("[OA] Ocean admin initialized\n");
    return 0;
}

int oa_planning(int zone_type, int sea_area, int sqkm, int permits, int islands, int year) {
    if (oa.n_planning >= MAX_PLANNING) return -1;
    planning_t* p = &plannings[oa.n_planning];
    p->planning_id = oa.n_planning;
    p->zone_type = zone_type;
    p->sea_area_id = sea_area;
    p->area_sqkm = sqkm;
    p->usage_permits = permits;
    p->island_protection = islands;
    p->year = year;
    p->active = 1;
    oa.total_area += sqkm;
    oa.n_planning++;
    print_str("[OA] Planning "); print_int(oa.n_planning - 1);
    print_str(" zone="); print_int(zone_type);
    print_str(" sea="); print_int(sea_area);
    print_str(" sqkm="); print_int(sqkm);
    print_str(" prmt="); print_int(permits);
    print_str(" isl="); print_int(islands); print_str("\n");
    return oa.n_planning - 1;
}

int oa_environment(int mon_type, int region, int stations, int pollution, int restoration, int year) {
    if (oa.n_environment >= MAX_ENVIRONMENT) return -1;
    environment_t* e = &environments[oa.n_environment];
    e->environment_id = oa.n_environment;
    e->monitoring_type = mon_type;
    e->sea_region = region;
    e->monitoring_stations = stations;
    e->pollution_index = pollution;
    e->restoration_projects = restoration;
    e->year = year;
    e->active = 1;
    oa.total_stations += stations;
    oa.n_environment++;
    print_str("[OA] Environment "); print_int(oa.n_environment - 1);
    print_str(" type="); print_int(mon_type);
    print_str(" rgn="); print_int(region);
    print_str(" stn="); print_int(stations);
    print_str(" pol="); print_int(pollution);
    print_str(" rst="); print_int(restoration); print_str("\n");
    return oa.n_environment - 1;
}

int oa_economy(int industry, int sector, int output, int employment, int investment, int year) {
    if (oa.n_economy >= MAX_ECONOMY) return -1;
    economy_t* ec = &economies[oa.n_economy];
    ec->economy_id = oa.n_economy;
    ec->industry_type = industry;
    ec->sector = sector;
    ec->output_value = output;
    ec->employment = employment;
    ec->investment = investment;
    ec->year = year;
    ec->active = 1;
    oa.total_output += output;
    oa.n_economy++;
    print_str("[OA] Economy "); print_int(oa.n_economy - 1);
    print_str(" ind="); print_int(industry);
    print_str(" sec="); print_int(sector);
    print_str(" out=$"); print_int(output);
    print_str(" emp="); print_int(employment);
    print_str(" inv=$"); print_int(investment); print_str("\n");
    return oa.n_economy - 1;
}

int oa_technology(int res_type, int exploration, int surveys, int deep_sea, int polar, int year) {
    if (oa.n_technology >= MAX_TECHNOLOGY) return -1;
    technology_t* t = &technologies[oa.n_technology];
    t->technology_id = oa.n_technology;
    t->research_type = res_type;
    t->exploration_area = exploration;
    t->surveys_conducted = surveys;
    t->deep_sea_missions = deep_sea;
    t->polar_expeditions = polar;
    t->year = year;
    t->active = 1;
    oa.total_surveys += surveys;
    oa.n_technology++;
    print_str("[OA] Technology "); print_int(oa.n_technology - 1);
    print_str(" type="); print_int(res_type);
    print_str(" area="); print_int(exploration);
    print_str(" srv="); print_int(surveys);
    print_str(" deep="); print_int(deep_sea);
    print_str(" polar="); print_int(polar); print_str("\n");
    return oa.n_technology - 1;
}

int oa_rights(int zone, int delimit_type, int negotiations, int agreements, int enforcement, int year) {
    if (oa.n_rights >= MAX_RIGHTS) return -1;
    rights_t* r = &rights[oa.n_rights];
    r->rights_id = oa.n_rights;
    r->maritime_zone = zone;
    r->delimitation_type = delimit_type;
    r->negotiations = negotiations;
    r->agreements_signed = agreements;
    r->enforcement_actions = enforcement;
    r->year = year;
    r->active = 1;
    oa.total_agreements += agreements;
    oa.n_rights++;
    print_str("[OA] Rights "); print_int(oa.n_rights - 1);
    print_str(" zone="); print_int(zone);
    print_str(" type="); print_int(delimit_type);
    print_str(" neg="); print_int(negotiations);
    print_str(" agr="); print_int(agreements);
    print_str(" enf="); print_int(enforcement); print_str("\n");
    return oa.n_rights - 1;
}

void oa_planning_report(void) {
    print_str("[OA] Planning report:\n");
    print_str("  Ocean zones: "); print_int(oa.n_planning); print_str("\n");
    print_str("  Total area managed: "); print_int(oa.total_area); print_str(" sqkm\n");
}

void oa_environment_report(void) {
    print_str("[OA] Environment report:\n");
    print_str("  Monitoring regions: "); print_int(oa.n_environment); print_str("\n");
    print_str("  Total stations: "); print_int(oa.total_stations); print_str("\n");
}

void oa_economy_report(void) {
    print_str("[OA] Economy report:\n");
    print_str("  Ocean industries: "); print_int(oa.n_economy); print_str("\n");
    print_str("  Total output: $"); print_int(oa.total_output); print_str("\n");
    print_str("  Technology surveys: "); print_int(oa.n_technology); print_str("\n");
    print_str("  Total surveys: "); print_int(oa.total_surveys); print_str("\n");
    print_str("  Maritime rights: "); print_int(oa.n_rights); print_str("\n");
    print_str("  Total agreements: "); print_int(oa.total_agreements); print_str("\n");
}

void oa_print_state(void) {
    print_str("[OA] Pl="); print_int(oa.n_planning);
    print_str(" Ev="); print_int(oa.n_environment);
    print_str(" Ec="); print_int(oa.n_economy);
    print_str(" Tc="); print_int(oa.n_technology);
    print_str(" Rt="); print_int(oa.n_rights);
    print_str("\n");
}

int main(void) {
    print_str("=== Ocean Admin Demo ===\n\n");
    oa_init();

    print_str("Ocean planning...\n");
    for (int i = 0; i < 16; i++) {
        int zone = (i % 6) + 1;
        int sea = (i % 8) + 1;
        int sqkm = 1000 + (i * 500);
        int prmt = 20 + (i * 5);
        int isl = 3 + (i % 5);
        int year = 2020 + (i % 5);
        oa_planning(zone, sea, sqkm, prmt, isl, year);
    }

    print_str("\nOcean environment...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 7) + 1;
        int stn = 15 + (i * 3);
        int pol = 30 + (i * 5);
        int rst = 2 + (i % 4);
        int year = 2021 + (i % 4);
        oa_environment(type, rgn, stn, pol, rst, year);
    }

    print_str("\nOcean economy...\n");
    for (int i = 0; i < 12; i++) {
        int ind = (i % 6) + 1;
        int sec = (i % 4) + 1;
        int out = 5000000 + (i * 2000000);
        int emp = 1000 + (i * 300);
        int inv = 3000000 + (i * 1000000);
        int year = 2022 + (i % 3);
        oa_economy(ind, sec, out, emp, inv, year);
    }

    print_str("\nOcean technology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 5) + 1;
        int area = (i % 6) + 1;
        int srv = 10 + (i * 3);
        int deep = 2 + (i % 4);
        int polar = 1 + (i % 3);
        int year = 2023 + (i % 2);
        oa_technology(type, area, srv, deep, polar, year);
    }

    print_str("\nMaritime rights...\n");
    for (int i = 0; i < 10; i++) {
        int zone = (i % 5) + 1;
        int type = (i % 4) + 1;
        int neg = 5 + (i * 2);
        int agr = 2 + (i % 3);
        int enf = 10 + (i * 3);
        int year = 2024;
        oa_rights(zone, type, neg, agr, enf, year);
    }

    print_str("\nPlanning report...\n");
    oa_planning_report();

    print_str("\nEnvironment report...\n");
    oa_environment_report();

    print_str("\nEconomy report...\n");
    oa_economy_report();

    print_str("\nFinal state...\n");
    oa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
