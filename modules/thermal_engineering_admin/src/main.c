/* thermal_engineering_admin: Thermal engineering administration system (v1.0)
 * Conduction, heat engines, refrigeration, heat exchangers, thermal systems
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

#define MAX_CONDUCTION    16
#define MAX_HEAT_ENGINE   14
#define MAX_REFRIG        12
#define MAX_HEAT_EXCH     10
#define MAX_THERMAL_SYS   10

typedef struct {
    int    cond_id;
    int    cond_type;
    int    cond_category;
    int    fourier;
    int    steady;
    int    transient;
    int    year;
    int    active;
} conduction_t;

typedef struct {
    int    eng_id;
    int    eng_type;
    int    eng_category;
    int    steam_engine;
    int    ic_engine;
    int    gas_turbine;
    int    year;
    int    active;
} heat_engine_t;

typedef struct {
    int    ref_id;
    int    ref_type;
    int    ref_category;
    int    comp_ref;
    int    abs_ref;
    int    jet_ref;
    int    year;
    int    active;
} refrigeration_t;

typedef struct {
    int    hex_id;
    int    hex_type;
    int    hex_category;
    int    shell_tube;
    int    plate;
    int    finned;
    int    year;
    int    active;
} heat_exchanger_t;

typedef struct {
    int    ths_id;
    int    ths_type;
    int    ths_category;
    int    steam_power;
    int    gas_power;
    int    combined_cycle;
    int    year;
    int    active;
} thermal_sys_t;

typedef struct {
    int    n_conduction;
    int    n_heat_engine;
    int    n_refrigeration;
    int    n_heat_exchanger;
    int    n_thermal_sys;
    int    total_fourier;
    int    total_steam_engine;
    int    total_comp_ref;
    int    total_shell_tube;
    int    total_steam_power;
} tea_state_t;

static conduction_t conductions[MAX_CONDUCTION];
static heat_engine_t heat_engines[MAX_HEAT_ENGINE];
static refrigeration_t refrigerations[MAX_REFRIG];
static heat_exchanger_t heat_exchangers[MAX_HEAT_EXCH];
static thermal_sys_t thermal_systems[MAX_THERMAL_SYS];
static tea_state_t tea;

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

int tea_init(void) {
    if (initialized) return -1;
    tea.n_conduction = 0; tea.n_heat_engine = 0; tea.n_refrigeration = 0;
    tea.n_heat_exchanger = 0; tea.n_thermal_sys = 0;
    tea.total_fourier = 0; tea.total_steam_engine = 0;
    tea.total_comp_ref = 0; tea.total_shell_tube = 0;
    tea.total_steam_power = 0;
    for (int i = 0; i < MAX_CONDUCTION; i++) conductions[i].active = 0;
    for (int i = 0; i < MAX_HEAT_ENGINE; i++) heat_engines[i].active = 0;
    for (int i = 0; i < MAX_REFRIG; i++) refrigerations[i].active = 0;
    for (int i = 0; i < MAX_HEAT_EXCH; i++) heat_exchangers[i].active = 0;
    for (int i = 0; i < MAX_THERMAL_SYS; i++) thermal_systems[i].active = 0;
    initialized = 1;
    print_str("[TEA] Thermal engineering initialized\n");
    return 0;
}

int tea_conduction(int cd_type, int cat, int four, int stdy, int trns, int year) {
    if (tea.n_conduction >= MAX_CONDUCTION) return -1;
    conduction_t* c = &conductions[tea.n_conduction];
    c->cond_id = tea.n_conduction;
    c->cond_type = cd_type;
    c->cond_category = cat;
    c->fourier = four;
    c->steady = stdy;
    c->transient = trns;
    c->year = year;
    c->active = 1;
    tea.total_fourier += four;
    tea.n_conduction++;
    print_str("[TEA] Conduction "); print_int(tea.n_conduction - 1);
    print_str(" type="); print_int(cd_type);
    print_str(" cat="); print_int(cat);
    print_str(" fur="); print_int(four);
    print_str(" std="); print_int(stdy);
    print_str(" trs="); print_int(trns); print_str("\n");
    return tea.n_conduction - 1;
}

int tea_heat_engine(int he_type, int cat, int stm, int ic_e, int gst, int year) {
    if (tea.n_heat_engine >= MAX_HEAT_ENGINE) return -1;
    heat_engine_t* h = &heat_engines[tea.n_heat_engine];
    h->eng_id = tea.n_heat_engine;
    h->eng_type = he_type;
    h->eng_category = cat;
    h->steam_engine = stm;
    h->ic_engine = ic_e;
    h->gas_turbine = gst;
    h->year = year;
    h->active = 1;
    tea.total_steam_engine += stm;
    tea.n_heat_engine++;
    print_str("[TEA] Heat engine "); print_int(tea.n_heat_engine - 1);
    print_str(" type="); print_int(he_type);
    print_str(" cat="); print_int(cat);
    print_str(" stm="); print_int(stm);
    print_str(" ic="); print_int(ic_e);
    print_str(" gst="); print_int(gst); print_str("\n");
    return tea.n_heat_engine - 1;
}

int tea_refrigeration(int rf_type, int cat, int comp_r, int abs_r, int jet_r, int year) {
    if (tea.n_refrigeration >= MAX_REFRIG) return -1;
    refrigeration_t* r = &refrigerations[tea.n_refrigeration];
    r->ref_id = tea.n_refrigeration;
    r->ref_type = rf_type;
    r->ref_category = cat;
    r->comp_ref = comp_r;
    r->abs_ref = abs_r;
    r->jet_ref = jet_r;
    r->year = year;
    r->active = 1;
    tea.total_comp_ref += comp_r;
    tea.n_refrigeration++;
    print_str("[TEA] Refrigeration "); print_int(tea.n_refrigeration - 1);
    print_str(" type="); print_int(rf_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(comp_r);
    print_str(" abs="); print_int(abs_r);
    print_str(" jet="); print_int(jet_r); print_str("\n");
    return tea.n_refrigeration - 1;
}

int tea_heat_exchanger(int hx_type, int cat, int sht, int plt, int fin, int year) {
    if (tea.n_heat_exchanger >= MAX_HEAT_EXCH) return -1;
    heat_exchanger_t* h = &heat_exchangers[tea.n_heat_exchanger];
    h->hex_id = tea.n_heat_exchanger;
    h->hex_type = hx_type;
    h->hex_category = cat;
    h->shell_tube = sht;
    h->plate = plt;
    h->finned = fin;
    h->year = year;
    h->active = 1;
    tea.total_shell_tube += sht;
    tea.n_heat_exchanger++;
    print_str("[TEA] Heat exchanger "); print_int(tea.n_heat_exchanger - 1);
    print_str(" type="); print_int(hx_type);
    print_str(" cat="); print_int(cat);
    print_str(" sht="); print_int(sht);
    print_str(" plt="); print_int(plt);
    print_str(" fin="); print_int(fin); print_str("\n");
    return tea.n_heat_exchanger - 1;
}

int tea_thermal_sys(int ts_type, int cat, int stm_p, int gas_p, int cmb, int year) {
    if (tea.n_thermal_sys >= MAX_THERMAL_SYS) return -1;
    thermal_sys_t* t = &thermal_systems[tea.n_thermal_sys];
    t->ths_id = tea.n_thermal_sys;
    t->ths_type = ts_type;
    t->ths_category = cat;
    t->steam_power = stm_p;
    t->gas_power = gas_p;
    t->combined_cycle = cmb;
    t->year = year;
    t->active = 1;
    tea.total_steam_power += stm_p;
    tea.n_thermal_sys++;
    print_str("[TEA] Thermal sys "); print_int(tea.n_thermal_sys - 1);
    print_str(" type="); print_int(ts_type);
    print_str(" cat="); print_int(cat);
    print_str(" stp="); print_int(stm_p);
    print_str(" gsp="); print_int(gas_p);
    print_str(" cmb="); print_int(cmb); print_str("\n");
    return tea.n_thermal_sys - 1;
}

void tea_conduction_report(void) {
    print_str("[TEA] Conduction report:\n");
    print_str("  Conduction categories: "); print_int(tea.n_conduction); print_str("\n");
    print_str("  Total Fourier: "); print_int(tea.total_fourier); print_str("\n");
}

void tea_heat_engine_report(void) {
    print_str("[TEA] Heat engine report:\n");
    print_str("  Heat engine categories: "); print_int(tea.n_heat_engine); print_str("\n");
    print_str("  Total steam engines: "); print_int(tea.total_steam_engine); print_str("\n");
}

void tea_thermal_sys_report(void) {
    print_str("[TEA] Thermal system report:\n");
    print_str("  Refrigeration categories: "); print_int(tea.n_refrigeration); print_str("\n");
    print_str("  Total compression ref: "); print_int(tea.total_comp_ref); print_str("\n");
    print_str("  Heat exchanger categories: "); print_int(tea.n_heat_exchanger); print_str("\n");
    print_str("  Total shell-tube: "); print_int(tea.total_shell_tube); print_str("\n");
    print_str("  Thermal system categories: "); print_int(tea.n_thermal_sys); print_str("\n");
    print_str("  Total steam power: "); print_int(tea.total_steam_power); print_str("\n");
}

void tea_print_state(void) {
    print_str("[TEA] Cd="); print_int(tea.n_conduction);
    print_str(" He="); print_int(tea.n_heat_engine);
    print_str(" Rf="); print_int(tea.n_refrigeration);
    print_str(" Hx="); print_int(tea.n_heat_exchanger);
    print_str(" Ts="); print_int(tea.n_thermal_sys);
    print_str("\n");
}

int main(void) {
    print_str("=== Thermal Engineering Admin Demo ===\n\n");
    tea_init();

    print_str("Conduction...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int fur = 52 + (i * 12);
        int stdy = 38 + (i * 9);
        int trns = 20 + (i * 5);
        int year = 2020 + (i % 5);
        tea_conduction(type, cat, fur, stdy, trns, year);
    }

    print_str("\nHeat engines...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stm = 46 + (i * 11);
        int ic_e = 32 + (i * 8);
        int gst = 18 + (i * 4);
        int year = 2021 + (i % 4);
        tea_heat_engine(type, cat, stm, ic_e, gst, year);
    }

    print_str("\nRefrigeration...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmp = 40 + (i * 9);
        int abs = 28 + (i * 6);
        int jet = 15 + (i * 3);
        int year = 2022 + (i % 3);
        tea_refrigeration(type, cat, cmp, abs, jet, year);
    }

    print_str("\nHeat exchangers...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sht = 35 + (i * 8);
        int plt = 24 + (i * 5);
        int fin = 12 + (i * 3);
        int year = 2023 + (i % 2);
        tea_heat_exchanger(type, cat, sht, plt, fin, year);
    }

    print_str("\nThermal systems...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stp = 30 + (i * 7);
        int gsp = 22 + (i * 5);
        int cmb = 10 + (i * 3);
        int year = 2024;
        tea_thermal_sys(type, cat, stp, gsp, cmb, year);
    }

    print_str("\nConduction report...\n");
    tea_conduction_report();

    print_str("\nHeat engine report...\n");
    tea_heat_engine_report();

    print_str("\nThermal system report...\n");
    tea_thermal_sys_report();

    print_str("\nFinal state...\n");
    tea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
