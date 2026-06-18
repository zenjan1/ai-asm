/* environment_admin: Environment administration system (v1.0)
 * Monitoring, pollution control, ecology, climate, enforcement
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

#define MAX_MONITOR      16
#define MAX_POLLUTION    14
#define MAX_ECOLOGY      12
#define MAX_CLIMATE      10
#define MAX_ENFORCE      10

typedef struct {
    int    monitor_id;
    int    station_id;
    int    monitor_type;
    int    reading;
    int    threshold;
    int    compliance;
    int    year;
    int    active;
} monitor_t;

typedef struct {
    int    pollution_id;
    int    source_id;
    int    pollution_type;
    int    emission_level;
    int    treatment_rate;
    int    reduction;
    int    year;
    int    active;
} pollution_t;

typedef struct {
    int    ecology_id;
    int    reserve_id;
    int    eco_type;
    int    area_hectares;
    int    species_count;
    int    health_score;
    int    year;
    int    active;
} ecology_t;

typedef struct {
    int    climate_id;
    int    region_id;
    int    program_type;
    int    carbon_tons;
    int    energy_saving;
    int    renewable_pct;
    int    year;
    int    active;
} climate_t;

typedef struct {
    int    enforce_id;
    int    case_id;
    int    violation_type;
    int    penalty;
    int    rectified;
    int    resolved;
    int    year;
    int    active;
} enforce_t;

typedef struct {
    int    n_monitor;
    int    n_pollution;
    int    n_ecology;
    int    n_climate;
    int    n_enforce;
    int    total_stations;
    int    total_emissions;
    int    total_area;
    int    total_carbon;
    int    total_penalty;
} ea_state_t;

static monitor_t monitors[MAX_MONITOR];
static pollution_t pollutions[MAX_POLLUTION];
static ecology_t ecologies[MAX_ECOLOGY];
static climate_t climates[MAX_CLIMATE];
static enforce_t enforces[MAX_ENFORCE];
static ea_state_t ea;

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

int ea_init(void) {
    if (initialized) return -1;
    ea.n_monitor = 0; ea.n_pollution = 0; ea.n_ecology = 0;
    ea.n_climate = 0; ea.n_enforce = 0;
    ea.total_stations = 0; ea.total_emissions = 0;
    ea.total_area = 0; ea.total_carbon = 0;
    ea.total_penalty = 0;
    for (int i = 0; i < MAX_MONITOR; i++) monitors[i].active = 0;
    for (int i = 0; i < MAX_POLLUTION; i++) pollutions[i].active = 0;
    for (int i = 0; i < MAX_ECOLOGY; i++) ecologies[i].active = 0;
    for (int i = 0; i < MAX_CLIMATE; i++) climates[i].active = 0;
    for (int i = 0; i < MAX_ENFORCE; i++) enforces[i].active = 0;
    initialized = 1;
    print_str("[EA] Environment admin initialized\n");
    return 0;
}

int ea_monitor(int station, int monitor_type, int reading, int threshold, int compliance, int year) {
    if (ea.n_monitor >= MAX_MONITOR) return -1;
    monitor_t* m = &monitors[ea.n_monitor];
    m->monitor_id = ea.n_monitor;
    m->station_id = station;
    m->monitor_type = monitor_type;
    m->reading = reading;
    m->threshold = threshold;
    m->compliance = compliance;
    m->year = year;
    m->active = 1;
    ea.total_stations++;
    ea.n_monitor++;
    print_str("[EA] Monitor "); print_int(ea.n_monitor - 1);
    print_str(" stn="); print_int(station);
    print_str(" type="); print_int(monitor_type);
    print_str(" rdg="); print_int(reading);
    print_str(" thr="); print_int(threshold);
    print_str(" cpl="); print_int(compliance); print_str("\n");
    return ea.n_monitor - 1;
}

int ea_pollution(int source, int pollution_type, int emission, int treatment, int reduction, int year) {
    if (ea.n_pollution >= MAX_POLLUTION) return -1;
    pollution_t* p = &pollutions[ea.n_pollution];
    p->pollution_id = ea.n_pollution;
    p->source_id = source;
    p->pollution_type = pollution_type;
    p->emission_level = emission;
    p->treatment_rate = treatment;
    p->reduction = reduction;
    p->year = year;
    p->active = 1;
    ea.total_emissions += emission;
    ea.n_pollution++;
    print_str("[EA] Pollution "); print_int(ea.n_pollution - 1);
    print_str(" src="); print_int(source);
    print_str(" type="); print_int(pollution_type);
    print_str(" emt="); print_int(emission); print_str("t");
    print_str(" trt="); print_int(treatment); print_str("%");
    print_str(" rdc="); print_int(reduction); print_str("t\n");
    return ea.n_pollution - 1;
}

int ea_ecology(int reserve, int eco_type, int area, int species, int health, int year) {
    if (ea.n_ecology >= MAX_ECOLOGY) return -1;
    ecology_t* e = &ecologies[ea.n_ecology];
    e->ecology_id = ea.n_ecology;
    e->reserve_id = reserve;
    e->eco_type = eco_type;
    e->area_hectares = area;
    e->species_count = species;
    e->health_score = health;
    e->year = year;
    e->active = 1;
    ea.total_area += area;
    ea.n_ecology++;
    print_str("[EA] Ecology "); print_int(ea.n_ecology - 1);
    print_str(" rsv="); print_int(reserve);
    print_str(" type="); print_int(eco_type);
    print_str(" area="); print_int(area); print_str("ha");
    print_str(" spc="); print_int(species);
    print_str(" hlt="); print_int(health); print_str("\n");
    return ea.n_ecology - 1;
}

int ea_climate(int region, int program_type, int carbon, int energy, int renewable, int year) {
    if (ea.n_climate >= MAX_CLIMATE) return -1;
    climate_t* c = &climates[ea.n_climate];
    c->climate_id = ea.n_climate;
    c->region_id = region;
    c->program_type = program_type;
    c->carbon_tons = carbon;
    c->energy_saving = energy;
    c->renewable_pct = renewable;
    c->year = year;
    c->active = 1;
    ea.total_carbon += carbon;
    ea.n_climate++;
    print_str("[EA] Climate "); print_int(ea.n_climate - 1);
    print_str(" rgn="); print_int(region);
    print_str(" type="); print_int(program_type);
    print_str(" co2="); print_int(carbon); print_str("t");
    print_str(" nrg="); print_int(energy); print_str("%");
    print_str(" rnb="); print_int(renewable); print_str("%\n");
    return ea.n_climate - 1;
}

int ea_enforce(int case_id, int violation_type, int penalty, int rectified, int resolved, int year) {
    if (ea.n_enforce >= MAX_ENFORCE) return -1;
    enforce_t* ef = &enforces[ea.n_enforce];
    ef->enforce_id = ea.n_enforce;
    ef->case_id = case_id;
    ef->violation_type = violation_type;
    ef->penalty = penalty;
    ef->rectified = rectified;
    ef->resolved = resolved;
    ef->year = year;
    ef->active = 1;
    ea.total_penalty += penalty;
    ea.n_enforce++;
    print_str("[EA] Enforce "); print_int(ea.n_enforce - 1);
    print_str(" cas="); print_int(case_id);
    print_str(" type="); print_int(violation_type);
    print_str(" pen=$"); print_int(penalty);
    print_str(" rct="); print_int(rectified);
    print_str(" res="); print_int(resolved); print_str("\n");
    return ea.n_enforce - 1;
}

void ea_monitor_report(void) {
    print_str("[EA] Monitoring report:\n");
    print_str("  Stations: "); print_int(ea.total_stations); print_str("\n");
    print_str("  Monitoring points: "); print_int(ea.n_monitor); print_str("\n");
}

void ea_pollution_report(void) {
    print_str("[EA] Pollution report:\n");
    print_str("  Pollution sources: "); print_int(ea.n_pollution); print_str("\n");
    print_str("  Total emissions: "); print_int(ea.total_emissions); print_str(" tons\n");
}

void ea_ecology_report(void) {
    print_str("[EA] Ecology report:\n");
    print_str("  Nature reserves: "); print_int(ea.n_ecology); print_str("\n");
    print_str("  Total area: "); print_int(ea.total_area); print_str(" hectares\n");
    print_str("  Climate programs: "); print_int(ea.n_climate); print_str("\n");
    print_str("  Total carbon reduced: "); print_int(ea.total_carbon); print_str(" tons\n");
    print_str("  Enforcement cases: "); print_int(ea.n_enforce); print_str("\n");
    print_str("  Total penalties: $"); print_int(ea.total_penalty); print_str("\n");
}

void ea_print_state(void) {
    print_str("[EA] Mn="); print_int(ea.n_monitor);
    print_str(" Pl="); print_int(ea.n_pollution);
    print_str(" Ec="); print_int(ea.n_ecology);
    print_str(" Cl="); print_int(ea.n_climate);
    print_str(" En="); print_int(ea.n_enforce);
    print_str("\n");
}

int main(void) {
    print_str("=== Environment Admin Demo ===\n\n");
    ea_init();

    print_str("Environmental monitoring...\n");
    for (int i = 0; i < 16; i++) {
        int stn = 100 + (i * 10);
        int type = (i % 4) + 1;
        int rdg = 50 + (i * 5);
        int thr = 100;
        int cpl = (rdg <= thr) ? 1 : 0;
        int year = 2020 + (i % 5);
        ea_monitor(stn, type, rdg, thr, cpl, year);
    }

    print_str("\nPollution control...\n");
    for (int i = 0; i < 14; i++) {
        int src = 200 + (i * 8);
        int type = (i % 3) + 1;
        int emt = 1000 + (i * 300);
        int trt = 60 + (i * 3);
        int rdc = emt * trt / 100;
        int year = 2021 + (i % 4);
        ea_pollution(src, type, emt, trt, rdc, year);
    }

    print_str("\nEcological protection...\n");
    for (int i = 0; i < 12; i++) {
        int rsv = 300 + (i * 12);
        int type = (i % 4) + 1;
        int area = 500 + (i * 200);
        int spc = 50 + (i * 15);
        int hlt = 70 + (i * 2);
        int year = 2022 + (i % 3);
        ea_ecology(rsv, type, area, spc, hlt, year);
    }

    print_str("\nClimate action...\n");
    for (int i = 0; i < 10; i++) {
        int rgn = (i % 8) + 1;
        int type = (i % 3) + 1;
        int co2 = 5000 + (i * 2000);
        int nrg = 10 + (i * 3);
        int rnb = 15 + (i * 5);
        int year = 2023 + (i % 2);
        ea_climate(rgn, type, co2, nrg, rnb, year);
    }

    print_str("\nEnvironmental enforcement...\n");
    for (int i = 0; i < 10; i++) {
        int cas = 400 + (i * 11);
        int type = (i % 4) + 1;
        int pen = 10000 + (i * 5000);
        int rct = (i % 3 == 0) ? 0 : 1;
        int res = (i % 4 == 0) ? 0 : 1;
        int year = 2024;
        ea_enforce(cas, type, pen, rct, res, year);
    }

    print_str("\nMonitoring report...\n");
    ea_monitor_report();

    print_str("\nPollution report...\n");
    ea_pollution_report();

    print_str("\nEcology report...\n");
    ea_ecology_report();

    print_str("\nFinal state...\n");
    ea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
