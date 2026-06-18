/* disaster_prevention: Disaster prevention system (v1.0)
 * Disaster monitoring, risk assessment, relief, seismic, comprehensive
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
#define MAX_RISK         14
#define MAX_RELIEF       12
#define MAX_SEISMIC      10
#define MAX_COMPREH      10

typedef struct {
    int    monitor_id;
    int    type;
    int    station_id;
    int    region_id;
    int    intensity;
    int    frequency;
    int    alerts_issued;
    int    year;
    int    active;
} monitor_t;

typedef struct {
    int    risk_id;
    int    hazard_type;
    int    region_id;
    int    population_at_risk;
    int    asset_value;
    int    vulnerability;
    int    mitigation_score;
    int    year;
    int    active;
} risk_t;

typedef struct {
    int    relief_id;
    int    disaster_id;
    int    affected_population;
    int    displaced;
    int    relief_supplies;
    int    relief_funds;
    int    shelters;
    int    year;
    int    active;
} relief_t;

typedef struct {
    int    seismic_id;
    int    building_id;
    int    fortification_class;
    int    peak_ground_accel;
    int    retrofit_needed;
    int    retrofitted;
    int    year_assessed;
    int    status;
    int    active;
} seismic_t;

typedef struct {
    int    compre_id;
    int    category;
    int    region_id;
    int    communities;
    int    coverage_pct;
    int    budget;
    int    year;
    int    active;
} comprehensive_t;

typedef struct {
    int    n_monitor;
    int    n_risk;
    int    n_relief;
    int    n_seismic;
    int    n_compre;
    int    total_alerts;
    int    total_at_risk;
    int    total_affected;
    int    total_relief_funds;
    int    total_retrofit;
} dp_state_t;

static monitor_t monitors[MAX_MONITOR];
static risk_t risks[MAX_RISK];
static relief_t reliefs[MAX_RELIEF];
static seismic_t seismic_records[MAX_SEISMIC];
static comprehensive_t comprehensive[MAX_COMPREH];
static dp_state_t dp;

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

int dp_init(void) {
    if (initialized) return -1;
    dp.n_monitor = 0; dp.n_risk = 0; dp.n_relief = 0;
    dp.n_seismic = 0; dp.n_compre = 0;
    dp.total_alerts = 0; dp.total_at_risk = 0;
    dp.total_affected = 0; dp.total_relief_funds = 0;
    dp.total_retrofit = 0;
    for (int i = 0; i < MAX_MONITOR; i++) monitors[i].active = 0;
    for (int i = 0; i < MAX_RISK; i++) risks[i].active = 0;
    for (int i = 0; i < MAX_RELIEF; i++) reliefs[i].active = 0;
    for (int i = 0; i < MAX_SEISMIC; i++) seismic_records[i].active = 0;
    for (int i = 0; i < MAX_COMPREH; i++) comprehensive[i].active = 0;
    initialized = 1;
    print_str("[DP] Disaster prevention initialized\n");
    return 0;
}

int dp_monitor(int type, int station, int region, int intensity, int frequency, int alerts, int year) {
    if (dp.n_monitor >= MAX_MONITOR) return -1;
    monitor_t* m = &monitors[dp.n_monitor];
    m->monitor_id = dp.n_monitor;
    m->type = type;
    m->station_id = station;
    m->region_id = region;
    m->intensity = intensity;
    m->frequency = frequency;
    m->alerts_issued = alerts;
    m->year = year;
    m->active = 1;
    dp.total_alerts += alerts;
    dp.n_monitor++;
    print_str("[DP] Monitor "); print_int(dp.n_monitor - 1);
    print_str(" type="); print_int(type);
    print_str(" stn="); print_int(station);
    print_str(" reg="); print_int(region);
    print_str(" int="); print_int(intensity);
    print_str(" frq="); print_int(frequency);
    print_str(" alr="); print_int(alerts); print_str("\n");
    return dp.n_monitor - 1;
}

int dp_assess(int hazard, int region, int pop, int asset, int vulnerability, int mitigation, int year) {
    if (dp.n_risk >= MAX_RISK) return -1;
    risk_t* r = &risks[dp.n_risk];
    r->risk_id = dp.n_risk;
    r->hazard_type = hazard;
    r->region_id = region;
    r->population_at_risk = pop;
    r->asset_value = asset;
    r->vulnerability = vulnerability;
    r->mitigation_score = mitigation;
    r->year = year;
    r->active = 1;
    dp.total_at_risk += pop;
    dp.n_risk++;
    print_str("[DP] Risk "); print_int(dp.n_risk - 1);
    print_str(" hz="); print_int(hazard);
    print_str(" reg="); print_int(region);
    print_str(" pop="); print_int(pop);
    print_str(" ast=$"); print_int(asset);
    print_str(" vln="); print_int(vulnerability);
    print_str(" mtg="); print_int(mitigation); print_str("\n");
    return dp.n_risk - 1;
}

int dp_relief(int disaster, int affected, int displaced, int supplies, int funds, int shelters, int year) {
    if (dp.n_relief >= MAX_RELIEF) return -1;
    relief_t* rl = &reliefs[dp.n_relief];
    rl->relief_id = dp.n_relief;
    rl->disaster_id = disaster;
    rl->affected_population = affected;
    rl->displaced = displaced;
    rl->relief_supplies = supplies;
    rl->relief_funds = funds;
    rl->shelters = shelters;
    rl->year = year;
    rl->active = 1;
    dp.total_affected += affected;
    dp.total_relief_funds += funds;
    dp.n_relief++;
    print_str("[DP] Relief "); print_int(dp.n_relief - 1);
    print_str(" dis="); print_int(disaster);
    print_str(" aff="); print_int(affected);
    print_str(" dsp="); print_int(displaced);
    print_str(" sup="); print_int(supplies);
    print_str(" fnd=$"); print_int(funds);
    print_str(" shl="); print_int(shelters); print_str("\n");
    return dp.n_relief - 1;
}

int dp_fortify(int building, int fort_class, int pga, int retrofit_needed, int retrofitted, int year) {
    if (dp.n_seismic >= MAX_SEISMIC) return -1;
    seismic_t* s = &seismic_records[dp.n_seismic];
    s->seismic_id = dp.n_seismic;
    s->building_id = building;
    s->fortification_class = fort_class;
    s->peak_ground_accel = pga;
    s->retrofit_needed = retrofit_needed;
    s->retrofitted = retrofitted;
    s->year_assessed = year;
    s->status = 1;
    s->active = 1;
    if (retrofit_needed && !retrofitted) dp.total_retrofit++;
    dp.n_seismic++;
    print_str("[DP] Seismic "); print_int(dp.n_seismic - 1);
    print_str(" bld="); print_int(building);
    print_str(" cls="); print_int(fort_class);
    print_str(" pga="); print_int(pga);
    print_str(" rfn="); print_int(retrofit_needed);
    print_str(" rfd="); print_int(retrofitted); print_str("\n");
    return dp.n_seismic - 1;
}

int dp_add_comprehensive(int category, int region, int communities, int coverage, int budget, int year) {
    if (dp.n_compre >= MAX_COMPREH) return -1;
    comprehensive_t* c = &comprehensive[dp.n_compre];
    c->compre_id = dp.n_compre;
    c->category = category;
    c->region_id = region;
    c->communities = communities;
    c->coverage_pct = coverage;
    c->budget = budget;
    c->year = year;
    c->active = 1;
    dp.n_compre++;
    print_str("[DP] Comprehensive "); print_int(dp.n_compre - 1);
    print_str(" cat="); print_int(category);
    print_str(" reg="); print_int(region);
    print_str(" cmn="); print_int(communities);
    print_str(" cov="); print_int(coverage); print_str("%");
    print_str(" bgt=$"); print_int(budget); print_str("\n");
    return dp.n_compre - 1;
}

void dp_monitor_report(void) {
    print_str("[DP] Monitor report:\n");
    print_str("  Stations: "); print_int(dp.n_monitor); print_str("\n");
    print_str("  Total alerts: "); print_int(dp.total_alerts); print_str("\n");
}

void dp_relief_report(void) {
    print_str("[DP] Relief report:\n");
    print_str("  Relief ops: "); print_int(dp.n_relief); print_str("\n");
    print_str("  Total affected: "); print_int(dp.total_affected); print_str("\n");
    print_str("  Total funds: $"); print_int(dp.total_relief_funds); print_str("\n");
}

void dp_prevention_report(void) {
    print_str("[DP] Prevention report:\n");
    print_str("  Risk assessments: "); print_int(dp.n_risk); print_str("\n");
    print_str("  Population at risk: "); print_int(dp.total_at_risk); print_str("\n");
    print_str("  Seismic records: "); print_int(dp.n_seismic); print_str("\n");
    print_str("  Retrofits needed: "); print_int(dp.total_retrofit); print_str("\n");
    print_str("  Comprehensive programs: "); print_int(dp.n_compre); print_str("\n");
}

void dp_print_state(void) {
    print_str("[DP] Mn="); print_int(dp.n_monitor);
    print_str(" Rk="); print_int(dp.n_risk);
    print_str(" Rl="); print_int(dp.n_relief);
    print_str(" Sm="); print_int(dp.n_seismic);
    print_str(" Cp="); print_int(dp.n_compre);
    print_str("\n");
}

int main(void) {
    print_str("=== Disaster Prevention Demo ===\n\n");
    dp_init();

    print_str("Disaster monitoring...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int stn = 100 + (i * 10);
        int reg = (i % 8) + 1;
        int intns = 30 + (i * 5);
        int freq = 10 + (i * 3);
        int alr = (i % 4);
        int year = 2021 + (i % 4);
        dp_monitor(type, stn, reg, intns, freq, alr, year);
    }

    print_str("\nRisk assessments...\n");
    for (int i = 0; i < 14; i++) {
        int hz = (i % 5) + 1;
        int reg = (i % 8) + 1;
        int pop = 5000 + (i * 2000);
        int ast = 1000000 + (i * 500000);
        int vln = 40 + (i * 4);
        int mtg = 50 + (i * 3);
        int year = 2022 + (i % 3);
        dp_assess(hz, reg, pop, ast, vln, mtg, year);
    }

    print_str("\nDisaster relief...\n");
    for (int i = 0; i < 12; i++) {
        int dis = 3000 + (i * 13);
        int aff = 1000 + (i * 500);
        int dsp = 100 + (i * 50);
        int sup = 500 + (i * 200);
        int fnd = 50000 + (i * 25000);
        int shl = 5 + (i * 3);
        int year = 2022 + (i % 3);
        dp_relief(dis, aff, dsp, sup, fnd, shl, year);
    }

    print_str("\nSeismic fortification...\n");
    for (int i = 0; i < 10; i++) {
        int bld = 2000 + (i * 100);
        int cls = (i % 4) + 1;
        int pga = 100 + (i * 20);
        int rfn = (i % 3 == 0) ? 1 : 0;
        int rfd = (i % 4 == 0) ? 1 : 0;
        int year = 2023 + (i % 2);
        dp_fortify(bld, cls, pga, rfn, rfd, year);
    }

    print_str("\nComprehensive disaster reduction...\n");
    for (int i = 0; i < 10; i++) {
        int cat = (i % 4) + 1;
        int reg = (i % 8) + 1;
        int cmn = 10 + (i * 5);
        int cov = 60 + (i * 4);
        int bgt = 100000 + (i * 50000);
        int year = 2024;
        dp_add_comprehensive(cat, reg, cmn, cov, bgt, year);
    }

    print_str("\nMonitor report...\n");
    dp_monitor_report();

    print_str("\nRelief report...\n");
    dp_relief_report();

    print_str("\nPrevention report...\n");
    dp_prevention_report();

    print_str("\nFinal state...\n");
    dp_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
