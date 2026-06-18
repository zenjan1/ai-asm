/* power_engineering_admin: Power engineering administration system (v1.0)
 * Boilers, steam turbines, thermal power, nuclear, new energy
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

#define MAX_BOILER      16
#define MAX_TURBINE     14
#define MAX_THERMAL     12
#define MAX_NUCLEAR     10
#define MAX_NEW_EN      10

typedef struct {
    int    bl_id;
    int    bl_type;
    int    bl_category;
    int    coal_boiler;
    int    cfb_boiler;
    int    hrv_boiler;
    int    year;
    int    active;
} boiler_t;

typedef struct {
    int    tb_id;
    int    tb_type;
    int    tb_category;
    int    impulse_tb;
    int    reaction_tb;
    int    tb_govern;
    int    year;
    int    active;
} turbine_t;

typedef struct {
    int    tp_id;
    int    tp_type;
    int    tp_category;
    int    thermal_pw;
    int    gas_comb;
    int    cogeneration;
    int    year;
    int    active;
} thermal_pw_t;

typedef struct {
    int    nc_id;
    int    nc_type;
    int    nc_category;
    int    pwr;
    int    bwr;
    int    hwr;
    int    year;
    int    active;
} nuclear_t;

typedef struct {
    int    ne_id;
    int    ne_type;
    int    ne_category;
    int    pv_power;
    int    wind_power;
    int    tidal_power;
    int    year;
    int    active;
} new_energy_t;

typedef struct {
    int    n_boiler;
    int    n_turbine;
    int    n_thermal_pw;
    int    n_nuclear;
    int    n_new_energy;
    int    total_coal_boiler;
    int    total_impulse_tb;
    int    total_thermal_pw;
    int    total_pwr;
    int    total_pv_power;
} pea_state_t;

static boiler_t boilers[MAX_BOILER];
static turbine_t turbines[MAX_TURBINE];
static thermal_pw_t thermal_pws[MAX_THERMAL];
static nuclear_t nuclears[MAX_NUCLEAR];
static new_energy_t new_energies[MAX_NEW_EN];
static pea_state_t pea;

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

int pea_init(void) {
    if (initialized) return -1;
    pea.n_boiler = 0; pea.n_turbine = 0; pea.n_thermal_pw = 0;
    pea.n_nuclear = 0; pea.n_new_energy = 0;
    pea.total_coal_boiler = 0; pea.total_impulse_tb = 0;
    pea.total_thermal_pw = 0; pea.total_pwr = 0;
    pea.total_pv_power = 0;
    for (int i = 0; i < MAX_BOILER; i++) boilers[i].active = 0;
    for (int i = 0; i < MAX_TURBINE; i++) turbines[i].active = 0;
    for (int i = 0; i < MAX_THERMAL; i++) thermal_pws[i].active = 0;
    for (int i = 0; i < MAX_NUCLEAR; i++) nuclears[i].active = 0;
    for (int i = 0; i < MAX_NEW_EN; i++) new_energies[i].active = 0;
    initialized = 1;
    print_str("[PEA] Power engineering initialized\n");
    return 0;
}

int pea_boiler(int bt_type, int cat, int clb, int cfb, int hrb, int year) {
    if (pea.n_boiler >= MAX_BOILER) return -1;
    boiler_t* b = &boilers[pea.n_boiler];
    b->bl_id = pea.n_boiler;
    b->bl_type = bt_type;
    b->bl_category = cat;
    b->coal_boiler = clb;
    b->cfb_boiler = cfb;
    b->hrv_boiler = hrb;
    b->year = year;
    b->active = 1;
    pea.total_coal_boiler += clb;
    pea.n_boiler++;
    print_str("[PEA] Boiler "); print_int(pea.n_boiler - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" clb="); print_int(clb);
    print_str(" cfb="); print_int(cfb);
    print_str(" hrb="); print_int(hrb); print_str("\n");
    return pea.n_boiler - 1;
}

int pea_turbine(int tt_type, int cat, int imp, int rct, int gov, int year) {
    if (pea.n_turbine >= MAX_TURBINE) return -1;
    turbine_t* t = &turbines[pea.n_turbine];
    t->tb_id = pea.n_turbine;
    t->tb_type = tt_type;
    t->tb_category = cat;
    t->impulse_tb = imp;
    t->reaction_tb = rct;
    t->tb_govern = gov;
    t->year = year;
    t->active = 1;
    pea.total_impulse_tb += imp;
    pea.n_turbine++;
    print_str("[PEA] Turbine "); print_int(pea.n_turbine - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" imp="); print_int(imp);
    print_str(" rct="); print_int(rct);
    print_str(" gov="); print_int(gov); print_str("\n");
    return pea.n_turbine - 1;
}

int pea_thermal_pw(int tp_type, int cat, int thp, int gsc, int cgn, int year) {
    if (pea.n_thermal_pw >= MAX_THERMAL) return -1;
    thermal_pw_t* t = &thermal_pws[pea.n_thermal_pw];
    t->tp_id = pea.n_thermal_pw;
    t->tp_type = tp_type;
    t->tp_category = cat;
    t->thermal_pw = thp;
    t->gas_comb = gsc;
    t->cogeneration = cgn;
    t->year = year;
    t->active = 1;
    pea.total_thermal_pw += thp;
    pea.n_thermal_pw++;
    print_str("[PEA] Thermal pw "); print_int(pea.n_thermal_pw - 1);
    print_str(" type="); print_int(tp_type);
    print_str(" cat="); print_int(cat);
    print_str(" thp="); print_int(thp);
    print_str(" gsc="); print_int(gsc);
    print_str(" cgn="); print_int(cgn); print_str("\n");
    return pea.n_thermal_pw - 1;
}

int pea_nuclear(int nt_type, int cat, int pwr_v, int bwr_v, int hwr_v, int year) {
    if (pea.n_nuclear >= MAX_NUCLEAR) return -1;
    nuclear_t* n = &nuclears[pea.n_nuclear];
    n->nc_id = pea.n_nuclear;
    n->nc_type = nt_type;
    n->nc_category = cat;
    n->pwr = pwr_v;
    n->bwr = bwr_v;
    n->hwr = hwr_v;
    n->year = year;
    n->active = 1;
    pea.total_pwr += pwr_v;
    pea.n_nuclear++;
    print_str("[PEA] Nuclear "); print_int(pea.n_nuclear - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pwr="); print_int(pwr_v);
    print_str(" bwr="); print_int(bwr_v);
    print_str(" hwr="); print_int(hwr_v); print_str("\n");
    return pea.n_nuclear - 1;
}

int pea_new_energy(int nt_type, int cat, int pvp, int wdp, int tdp, int year) {
    if (pea.n_new_energy >= MAX_NEW_EN) return -1;
    new_energy_t* n = &new_energies[pea.n_new_energy];
    n->ne_id = pea.n_new_energy;
    n->ne_type = nt_type;
    n->ne_category = cat;
    n->pv_power = pvp;
    n->wind_power = wdp;
    n->tidal_power = tdp;
    n->year = year;
    n->active = 1;
    pea.total_pv_power += pvp;
    pea.n_new_energy++;
    print_str("[PEA] New energy "); print_int(pea.n_new_energy - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pvp="); print_int(pvp);
    print_str(" wdp="); print_int(wdp);
    print_str(" tdp="); print_int(tdp); print_str("\n");
    return pea.n_new_energy - 1;
}

void pea_boiler_report(void) {
    print_str("[PEA] Boiler report:\n");
    print_str("  Boiler categories: "); print_int(pea.n_boiler); print_str("\n");
    print_str("  Total coal boilers: "); print_int(pea.total_coal_boiler); print_str("\n");
}

void pea_turbine_report(void) {
    print_str("[PEA] Turbine report:\n");
    print_str("  Turbine categories: "); print_int(pea.n_turbine); print_str("\n");
    print_str("  Total impulse turbines: "); print_int(pea.total_impulse_tb); print_str("\n");
}

void pea_full_report(void) {
    print_str("[PEA] Full report:\n");
    print_str("  Thermal power categories: "); print_int(pea.n_thermal_pw); print_str("\n");
    print_str("  Total thermal power: "); print_int(pea.total_thermal_pw); print_str("\n");
    print_str("  Nuclear categories: "); print_int(pea.n_nuclear); print_str("\n");
    print_str("  Total PWR: "); print_int(pea.total_pwr); print_str("\n");
    print_str("  New energy categories: "); print_int(pea.n_new_energy); print_str("\n");
    print_str("  Total PV power: "); print_int(pea.total_pv_power); print_str("\n");
}

void pea_print_state(void) {
    print_str("[PEA] Bl="); print_int(pea.n_boiler);
    print_str(" Tb="); print_int(pea.n_turbine);
    print_str(" Tp="); print_int(pea.n_thermal_pw);
    print_str(" Nc="); print_int(pea.n_nuclear);
    print_str(" Ne="); print_int(pea.n_new_energy);
    print_str("\n");
}

int main(void) {
    print_str("=== Power Engineering Admin Demo ===\n\n");
    pea_init();

    print_str("Boilers...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int clb = 55 + (i * 13);
        int cfb = 40 + (i * 10);
        int hrb = 22 + (i * 5);
        int year = 2020 + (i % 5);
        pea_boiler(type, cat, clb, cfb, hrb, year);
    }

    print_str("\nTurbines...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int imp = 48 + (i * 11);
        int rct = 35 + (i * 8);
        int gov = 20 + (i * 4);
        int year = 2021 + (i % 4);
        pea_turbine(type, cat, imp, rct, gov, year);
    }

    print_str("\nThermal power...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int thp = 42 + (i * 10);
        int gsc = 28 + (i * 7);
        int cgn = 18 + (i * 4);
        int year = 2022 + (i % 3);
        pea_thermal_pw(type, cat, thp, gsc, cgn, year);
    }

    print_str("\nNuclear...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pwr_v = 35 + (i * 8);
        int bwr_v = 25 + (i * 6);
        int hwr_v = 15 + (i * 3);
        int year = 2023 + (i % 2);
        pea_nuclear(type, cat, pwr_v, bwr_v, hwr_v, year);
    }

    print_str("\nNew energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pvp = 30 + (i * 7);
        int wdp = 22 + (i * 5);
        int tdp = 12 + (i * 3);
        int year = 2024;
        pea_new_energy(type, cat, pvp, wdp, tdp, year);
    }

    print_str("\nBoiler report...\n");
    pea_boiler_report();

    print_str("\nTurbine report...\n");
    pea_turbine_report();

    print_str("\nFull report...\n");
    pea_full_report();

    print_str("\nFinal state...\n");
    pea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
