/* energy_storage_admin: Energy storage administration system (v1.0)
 * Physical storage, electrochemical storage, thermal storage, storage applications, safety management
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

#define MAX_PHYSICAL     16
#define MAX_ELECTRO      14
#define MAX_THERMAL      12
#define MAX_APPLICATION  10
#define MAX_SAFETY       10

typedef struct {
    int    physical_id;
    int    physical_type;
    int    physical_category;
    int    pumped_hydro;
    int    compressed_air;
    int    flywheel;
    int    year;
    int    active;
} physical_t;

typedef struct {
    int    electro_id;
    int    electro_type;
    int    electro_category;
    int    lithium_battery;
    int    sodium_battery;
    int    flow_battery;
    int    year;
    int    active;
} electro_t;

typedef struct {
    int    thermal_id;
    int    thermal_type;
    int    thermal_category;
    int    molten_salt;
    int    phase_change;
    int    thermo_chemical;
    int    year;
    int    active;
} thermal_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    grid_side;
    int    user_side;
    int    renewable_match;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    safety_id;
    int    safety_type;
    int    safety_category;
    int    safety_monitor;
    int    safety_assess;
    int    emergency;
    int    year;
    int    active;
} safety_t;

typedef struct {
    int    n_physical;
    int    n_electro;
    int    n_thermal;
    int    n_app;
    int    n_safety;
    int    total_pumped;
    int    total_lithium;
    int    total_molten;
    int    total_grid;
    int    total_monitor;
} esa_state_t;

static physical_t physicals[MAX_PHYSICAL];
static electro_t electros[MAX_ELECTRO];
static thermal_t thermals[MAX_THERMAL];
static app_t apps[MAX_APPLICATION];
static safety_t safeties[MAX_SAFETY];
static esa_state_t esa;

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

int esa_init(void) {
    if (initialized) return -1;
    esa.n_physical = 0; esa.n_electro = 0; esa.n_thermal = 0;
    esa.n_app = 0; esa.n_safety = 0;
    esa.total_pumped = 0; esa.total_lithium = 0;
    esa.total_molten = 0; esa.total_grid = 0;
    esa.total_monitor = 0;
    for (int i = 0; i < MAX_PHYSICAL; i++) physicals[i].active = 0;
    for (int i = 0; i < MAX_ELECTRO; i++) electros[i].active = 0;
    for (int i = 0; i < MAX_THERMAL; i++) thermals[i].active = 0;
    for (int i = 0; i < MAX_APPLICATION; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_SAFETY; i++) safeties[i].active = 0;
    initialized = 1;
    print_str("[ESA] Energy storage initialized\n");
    return 0;
}

int esa_physical(int phy_type, int cat, int pumped, int air, int fly, int year) {
    if (esa.n_physical >= MAX_PHYSICAL) return -1;
    physical_t* p = &physicals[esa.n_physical];
    p->physical_id = esa.n_physical;
    p->physical_type = phy_type;
    p->physical_category = cat;
    p->pumped_hydro = pumped;
    p->compressed_air = air;
    p->flywheel = fly;
    p->year = year;
    p->active = 1;
    esa.total_pumped += pumped;
    esa.n_physical++;
    print_str("[ESA] Physical "); print_int(esa.n_physical - 1);
    print_str(" type="); print_int(phy_type);
    print_str(" cat="); print_int(cat);
    print_str(" pmp="); print_int(pumped);
    print_str(" air="); print_int(air);
    print_str(" fly="); print_int(fly); print_str("\n");
    return esa.n_physical - 1;
}

int esa_electro(int elc_type, int cat, int lithium, int sodium, int flow, int year) {
    if (esa.n_electro >= MAX_ELECTRO) return -1;
    electro_t* e = &electros[esa.n_electro];
    e->electro_id = esa.n_electro;
    e->electro_type = elc_type;
    e->electro_category = cat;
    e->lithium_battery = lithium;
    e->sodium_battery = sodium;
    e->flow_battery = flow;
    e->year = year;
    e->active = 1;
    esa.total_lithium += lithium;
    esa.n_electro++;
    print_str("[ESA] Electro "); print_int(esa.n_electro - 1);
    print_str(" type="); print_int(elc_type);
    print_str(" cat="); print_int(cat);
    print_str(" lth="); print_int(lithium);
    print_str(" sod="); print_int(sodium);
    print_str(" flw="); print_int(flow); print_str("\n");
    return esa.n_electro - 1;
}

int esa_thermal(int thm_type, int cat, int molten, int phase, int chem, int year) {
    if (esa.n_thermal >= MAX_THERMAL) return -1;
    thermal_t* t = &thermals[esa.n_thermal];
    t->thermal_id = esa.n_thermal;
    t->thermal_type = thm_type;
    t->thermal_category = cat;
    t->molten_salt = molten;
    t->phase_change = phase;
    t->thermo_chemical = chem;
    t->year = year;
    t->active = 1;
    esa.total_molten += molten;
    esa.n_thermal++;
    print_str("[ESA] Thermal "); print_int(esa.n_thermal - 1);
    print_str(" type="); print_int(thm_type);
    print_str(" cat="); print_int(cat);
    print_str(" mlt="); print_int(molten);
    print_str(" phs="); print_int(phase);
    print_str(" chm="); print_int(chem); print_str("\n");
    return esa.n_thermal - 1;
}

int esa_app(int ap_type, int cat, int grid_side, int user_side, int renew_match, int year) {
    if (esa.n_app >= MAX_APPLICATION) return -1;
    app_t* a = &apps[esa.n_app];
    a->app_id = esa.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->grid_side = grid_side;
    a->user_side = user_side;
    a->renewable_match = renew_match;
    a->year = year;
    a->active = 1;
    esa.total_grid += grid_side;
    esa.n_app++;
    print_str("[ESA] App "); print_int(esa.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" grd="); print_int(grid_side);
    print_str(" usr="); print_int(user_side);
    print_str(" rnm="); print_int(renew_match); print_str("\n");
    return esa.n_app - 1;
}

int esa_safety(int sft_type, int cat, int monitor, int assess, int emrg, int year) {
    if (esa.n_safety >= MAX_SAFETY) return -1;
    safety_t* s = &safeties[esa.n_safety];
    s->safety_id = esa.n_safety;
    s->safety_type = sft_type;
    s->safety_category = cat;
    s->safety_monitor = monitor;
    s->safety_assess = assess;
    s->emergency = emrg;
    s->year = year;
    s->active = 1;
    esa.total_monitor += monitor;
    esa.n_safety++;
    print_str("[ESA] Safety "); print_int(esa.n_safety - 1);
    print_str(" type="); print_int(sft_type);
    print_str(" cat="); print_int(cat);
    print_str(" mon="); print_int(monitor);
    print_str(" ass="); print_int(assess);
    print_str(" emr="); print_int(emrg); print_str("\n");
    return esa.n_safety - 1;
}

void esa_physical_report(void) {
    print_str("[ESA] Physical report:\n");
    print_str("  Physical storage categories: "); print_int(esa.n_physical); print_str("\n");
    print_str("  Total pumped hydro: "); print_int(esa.total_pumped); print_str("\n");
}

void esa_electro_report(void) {
    print_str("[ESA] Electro report:\n");
    print_str("  Electrochemical storage categories: "); print_int(esa.n_electro); print_str("\n");
    print_str("  Total lithium battery: "); print_int(esa.total_lithium); print_str("\n");
}

void esa_app_report(void) {
    print_str("[ESA] App report:\n");
    print_str("  Thermal storage categories: "); print_int(esa.n_thermal); print_str("\n");
    print_str("  Total molten salt: "); print_int(esa.total_molten); print_str("\n");
    print_str("  Storage application categories: "); print_int(esa.n_app); print_str("\n");
    print_str("  Total grid side: "); print_int(esa.total_grid); print_str("\n");
    print_str("  Safety management categories: "); print_int(esa.n_safety); print_str("\n");
    print_str("  Total safety monitor: "); print_int(esa.total_monitor); print_str("\n");
}

void esa_print_state(void) {
    print_str("[ESA] Ph="); print_int(esa.n_physical);
    print_str(" El="); print_int(esa.n_electro);
    print_str(" Th="); print_int(esa.n_thermal);
    print_str(" Ap="); print_int(esa.n_app);
    print_str(" Sf="); print_int(esa.n_safety);
    print_str("\n");
}

int main(void) {
    print_str("=== Energy Storage Admin Demo ===\n\n");
    esa_init();

    print_str("Physical storage...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pmp = 90 + (i * 22);
        int air = 50 + (i * 12);
        int fly = 25 + (i * 6);
        int year = 2020 + (i % 5);
        esa_physical(type, cat, pmp, air, fly, year);
    }

    print_str("\nElectrochemical storage...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lth = 75 + (i * 18);
        int sod = 40 + (i * 10);
        int flw = 28 + (i * 7);
        int year = 2021 + (i % 4);
        esa_electro(type, cat, lth, sod, flw, year);
    }

    print_str("\nThermal storage...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mlt = 45 + (i * 11);
        int phs = 30 + (i * 7);
        int chm = 18 + (i * 4);
        int year = 2022 + (i % 3);
        esa_thermal(type, cat, mlt, phs, chm, year);
    }

    print_str("\nStorage applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grd = 35 + (i * 9);
        int usr = 25 + (i * 6);
        int rnm = 18 + (i * 4);
        int year = 2023 + (i % 2);
        esa_app(type, cat, grd, usr, rnm, year);
    }

    print_str("\nSafety management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mon = 28 + (i * 7);
        int ass = 20 + (i * 5);
        int emr = 12 + (i * 3);
        int year = 2024;
        esa_safety(type, cat, mon, ass, emr, year);
    }

    print_str("\nPhysical report...\n");
    esa_physical_report();

    print_str("\nElectro report...\n");
    esa_electro_report();

    print_str("\nApp report...\n");
    esa_app_report();

    print_str("\nFinal state...\n");
    esa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
