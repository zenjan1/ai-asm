/* comprehensive_governance: Comprehensive governance administration system (v1.0)
 * Public security, work safety, food/drug supervision, urban management, emergency
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

#define MAX_SECURITY_CG  16
#define MAX_SAFETY       14
#define MAX_FOODDRUG     12
#define MAX_URBAN        10
#define MAX_EMERGENCY_CG 10

typedef struct {
    int    security_id;
    int    security_type;
    int    region_id;
    int    patrol_operations;
    int    cases_resolved;
    int    conflicts_mediated;
    int    year;
    int    active;
} security_cg_t;

typedef struct {
    int    safety_id;
    int    industry_type;
    int    risk_level;
    int    inspections_conducted;
    int    hazards_identified;
    int    accidents_handled;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    fooddrug_id;
    int    supervision_type;
    int    product_category;
    int    inspections_done;
    int    quality_tests;
    int    violations_found;
    int    year;
    int    active;
} fooddrug_t;

typedef struct {
    int    urban_id;
    int    management_type;
    int    district_id;
    int    sanitation_operations;
    int    illegal_structures;
    int    city_beautification;
    int    year;
    int    active;
} urban_t;

typedef struct {
    int    emergency_id;
    int    emergency_type;
    int    response_level;
    int    plans_developed;
    int    drills_conducted;
    int    incidents_handled;
    int    year;
    int    active;
} emergency_cg_t;

typedef struct {
    int    n_security_cg;
    int    n_safety;
    int    n_fooddrug;
    int    n_urban;
    int    n_emergency_cg;
    int    total_patrols;
    int    total_inspections;
    int    total_tests;
    int    total_sanitation;
    int    total_plans;
} cg_state_t;

static security_cg_t securities_cg[MAX_SECURITY_CG];
static safety_t safeties[MAX_SAFETY];
static fooddrug_t fooddrugs[MAX_FOODDRUG];
static urban_t urbans[MAX_URBAN];
static emergency_cg_t emergencies_cg[MAX_EMERGENCY_CG];
static cg_state_t cg;

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

int cg_init(void) {
    if (initialized) return -1;
    cg.n_security_cg = 0; cg.n_safety = 0; cg.n_fooddrug = 0;
    cg.n_urban = 0; cg.n_emergency_cg = 0;
    cg.total_patrols = 0; cg.total_inspections = 0;
    cg.total_tests = 0; cg.total_sanitation = 0;
    cg.total_plans = 0;
    for (int i = 0; i < MAX_SECURITY_CG; i++) securities_cg[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    for (int i = 0; i < MAX_FOODDRUG; i++) fooddrugs[i].active = 0;
    for (int i = 0; i < MAX_URBAN; i++) urbans[i].active = 0;
    for (int i = 0; i < MAX_EMERGENCY_CG; i++) emergencies_cg[i].active = 0;
    initialized = 1;
    print_str("[CG] Comprehensive governance initialized\n");
    return 0;
}

int cg_security(int sec_type, int region, int patrols, int cases, int conflicts, int year) {
    if (cg.n_security_cg >= MAX_SECURITY_CG) return -1;
    security_cg_t* s = &securities_cg[cg.n_security_cg];
    s->security_id = cg.n_security_cg;
    s->security_type = sec_type;
    s->region_id = region;
    s->patrol_operations = patrols;
    s->cases_resolved = cases;
    s->conflicts_mediated = conflicts;
    s->year = year;
    s->active = 1;
    cg.total_patrols += patrols;
    cg.n_security_cg++;
    print_str("[CG] Security "); print_int(cg.n_security_cg - 1);
    print_str(" type="); print_int(sec_type);
    print_str(" rgn="); print_int(region);
    print_str(" ptr="); print_int(patrols);
    print_str(" cas="); print_int(cases);
    print_str(" cfl="); print_int(conflicts); print_str("\n");
    return cg.n_security_cg - 1;
}

int cg_safety(int industry, int risk, int inspections, int hazards, int accidents, int year) {
    if (cg.n_safety >= MAX_SAFETY) return -1;
    safety_t* sf = &safeties[cg.n_safety];
    sf->safety_id = cg.n_safety;
    sf->industry_type = industry;
    sf->risk_level = risk;
    sf->inspections_conducted = inspections;
    sf->hazards_identified = hazards;
    sf->accidents_handled = accidents;
    sf->year = year;
    sf->active = 1;
    cg.total_inspections += inspections;
    cg.n_safety++;
    print_str("[CG] Safety "); print_int(cg.n_safety - 1);
    print_str(" ind="); print_int(industry);
    print_str(" rsk="); print_int(risk);
    print_str(" ins="); print_int(inspections);
    print_str(" hzd="); print_int(hazards);
    print_str(" acc="); print_int(accidents); print_str("\n");
    return cg.n_safety - 1;
}

int cg_fooddrug(int sup_type, int product, int inspections_cnt, int tests, int violations, int year) {
    if (cg.n_fooddrug >= MAX_FOODDRUG) return -1;
    fooddrug_t* fd = &fooddrugs[cg.n_fooddrug];
    fd->fooddrug_id = cg.n_fooddrug;
    fd->supervision_type = sup_type;
    fd->product_category = product;
    fd->inspections_done = inspections_cnt;
    fd->quality_tests = tests;
    fd->violations_found = violations;
    fd->year = year;
    fd->active = 1;
    cg.total_tests += tests;
    cg.n_fooddrug++;
    print_str("[CG] FoodDrug "); print_int(cg.n_fooddrug - 1);
    print_str(" type="); print_int(sup_type);
    print_str(" prd="); print_int(product);
    print_str(" ins="); print_int(inspections_cnt);
    print_str(" tst="); print_int(tests);
    print_str(" vio="); print_int(violations); print_str("\n");
    return cg.n_fooddrug - 1;
}

int cg_urban(int mgmt_type, int district, int sanitation, int illegal, int beautification, int year) {
    if (cg.n_urban >= MAX_URBAN) return -1;
    urban_t* u = &urbans[cg.n_urban];
    u->urban_id = cg.n_urban;
    u->management_type = mgmt_type;
    u->district_id = district;
    u->sanitation_operations = sanitation;
    u->illegal_structures = illegal;
    u->city_beautification = beautification;
    u->year = year;
    u->active = 1;
    cg.total_sanitation += sanitation;
    cg.n_urban++;
    print_str("[CG] Urban "); print_int(cg.n_urban - 1);
    print_str(" type="); print_int(mgmt_type);
    print_str(" dst="); print_int(district);
    print_str(" sn="); print_int(sanitation);
    print_str(" ill="); print_int(illegal);
    print_str(" btf="); print_int(beautification); print_str("\n");
    return cg.n_urban - 1;
}

int cg_emergency(int emg_type, int level, int plans, int drills, int incidents, int year) {
    if (cg.n_emergency_cg >= MAX_EMERGENCY_CG) return -1;
    emergency_cg_t* e = &emergencies_cg[cg.n_emergency_cg];
    e->emergency_id = cg.n_emergency_cg;
    e->emergency_type = emg_type;
    e->response_level = level;
    e->plans_developed = plans;
    e->drills_conducted = drills;
    e->incidents_handled = incidents;
    e->year = year;
    e->active = 1;
    cg.total_plans += plans;
    cg.n_emergency_cg++;
    print_str("[CG] Emergency "); print_int(cg.n_emergency_cg - 1);
    print_str(" type="); print_int(emg_type);
    print_str(" lvl="); print_int(level);
    print_str(" pln="); print_int(plans);
    print_str(" drl="); print_int(drills);
    print_str(" inc="); print_int(incidents); print_str("\n");
    return cg.n_emergency_cg - 1;
}

void cg_security_report(void) {
    print_str("[CG] Security report:\n");
    print_str("  Security categories: "); print_int(cg.n_security_cg); print_str("\n");
    print_str("  Total patrol operations: "); print_int(cg.total_patrols); print_str("\n");
}

void cg_safety_report(void) {
    print_str("[CG] Safety report:\n");
    print_str("  Safety categories: "); print_int(cg.n_safety); print_str("\n");
    print_str("  Total inspections: "); print_int(cg.total_inspections); print_str("\n");
}

void cg_urban_report(void) {
    print_str("[CG] Urban report:\n");
    print_str("  Food/drug categories: "); print_int(cg.n_fooddrug); print_str("\n");
    print_str("  Total quality tests: "); print_int(cg.total_tests); print_str("\n");
    print_str("  Urban management units: "); print_int(cg.n_urban); print_str("\n");
    print_str("  Total sanitation ops: "); print_int(cg.total_sanitation); print_str("\n");
    print_str("  Emergency categories: "); print_int(cg.n_emergency_cg); print_str("\n");
    print_str("  Total emergency plans: "); print_int(cg.total_plans); print_str("\n");
}

void cg_print_state(void) {
    print_str("[CG] Sc="); print_int(cg.n_security_cg);
    print_str(" Sf="); print_int(cg.n_safety);
    print_str(" Fd="); print_int(cg.n_fooddrug);
    print_str(" Ur="); print_int(cg.n_urban);
    print_str(" Em="); print_int(cg.n_emergency_cg);
    print_str("\n");
}

int main(void) {
    print_str("=== Comprehensive Governance Demo ===\n\n");
    cg_init();

    print_str("Public security...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int rgn = (i % 8) + 1;
        int ptr = 200 + (i * 50);
        int cas = 50 + (i * 12);
        int cfl = 30 + (i * 8);
        int year = 2020 + (i % 5);
        cg_security(type, rgn, ptr, cas, cfl, year);
    }

    print_str("\nWork safety...\n");
    for (int i = 0; i < 14; i++) {
        int ind = (i % 6) + 1;
        int rsk = (i % 4) + 1;
        int ins = 100 + (i * 25);
        int hzd = 20 + (i * 5);
        int acc = 3 + (i % 5);
        int year = 2021 + (i % 4);
        cg_safety(ind, rsk, ins, hzd, acc, year);
    }

    print_str("\nFood & drug supervision...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int prd = (i % 5) + 1;
        int ins = 150 + (i * 35);
        int tst = 200 + (i * 45);
        int vio = 10 + (i * 3);
        int year = 2022 + (i % 3);
        cg_fooddrug(type, prd, ins, tst, vio, year);
    }

    print_str("\nUrban management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int dst = (i % 8) + 1;
        int sn = 100 + (i * 25);
        int ill = 15 + (i * 4);
        int btf = 50 + (i * 12);
        int year = 2023 + (i % 2);
        cg_urban(type, dst, sn, ill, btf, year);
    }

    print_str("\nEmergency management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int lvl = (i % 4) + 1;
        int pln = 20 + (i * 5);
        int drl = 10 + (i * 3);
        int inc = 5 + (i * 2);
        int year = 2024;
        cg_emergency(type, lvl, pln, drl, inc, year);
    }

    print_str("\nSecurity report...\n");
    cg_security_report();

    print_str("\nSafety report...\n");
    cg_safety_report();

    print_str("\nUrban report...\n");
    cg_urban_report();

    print_str("\nFinal state...\n");
    cg_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
