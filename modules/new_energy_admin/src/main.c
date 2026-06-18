/* new_energy_admin: New energy administration system (v1.0)
 * Solar energy, wind energy, nuclear energy, hydrogen energy, biomass energy
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

#define MAX_SOLAR        16
#define MAX_WIND         14
#define MAX_NUCLEAR      12
#define MAX_HYDROGEN     10
#define MAX_BIOMASS      10

typedef struct {
    int    solar_id;
    int    solar_type;
    int    solar_category;
    int    photovoltaic;
    int    solar_thermal;
    int    solar_building;
    int    year;
    int    active;
} solar_t;

typedef struct {
    int    wind_id;
    int    wind_type;
    int    wind_category;
    int    onshore;
    int    offshore;
    int    wind_equip;
    int    year;
    int    active;
} wind_t;

typedef struct {
    int    nuclear_id;
    int    nuclear_type;
    int    nuclear_category;
    int    nuclear_build;
    int    fuel_cycle;
    int    nuclear_safety;
    int    year;
    int    active;
} nuclear_t;

typedef struct {
    int    hydrogen_id;
    int    hydrogen_type;
    int    hydrogen_category;
    int    hydrogen_produce;
    int    hydrogen_store;
    int    fuel_cell;
    int    year;
    int    active;
} hydrogen_t;

typedef struct {
    int    biomass_id;
    int    biomass_type;
    int    biomass_category;
    int    biomass_power;
    int    biofuel;
    int    biomass_gas;
    int    year;
    int    active;
} biomass_t;

typedef struct {
    int    n_solar;
    int    n_wind;
    int    n_nuclear;
    int    n_hydrogen;
    int    n_biomass;
    int    total_pv;
    int    total_onshore;
    int    total_nuclear;
    int    total_h2;
    int    total_bio;
} nea_state_t;

static solar_t solars[MAX_SOLAR];
static wind_t winds[MAX_WIND];
static nuclear_t nuclears[MAX_NUCLEAR];
static hydrogen_t hydrogens[MAX_HYDROGEN];
static biomass_t biomasses[MAX_BIOMASS];
static nea_state_t nea;

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

int nea_init(void) {
    if (initialized) return -1;
    nea.n_solar = 0; nea.n_wind = 0; nea.n_nuclear = 0;
    nea.n_hydrogen = 0; nea.n_biomass = 0;
    nea.total_pv = 0; nea.total_onshore = 0;
    nea.total_nuclear = 0; nea.total_h2 = 0;
    nea.total_bio = 0;
    for (int i = 0; i < MAX_SOLAR; i++) solars[i].active = 0;
    for (int i = 0; i < MAX_WIND; i++) winds[i].active = 0;
    for (int i = 0; i < MAX_NUCLEAR; i++) nuclears[i].active = 0;
    for (int i = 0; i < MAX_HYDROGEN; i++) hydrogens[i].active = 0;
    for (int i = 0; i < MAX_BIOMASS; i++) biomasses[i].active = 0;
    initialized = 1;
    print_str("[NEA] New energy initialized\n");
    return 0;
}

int nea_solar(int slr_type, int cat, int pv, int thermal, int building, int year) {
    if (nea.n_solar >= MAX_SOLAR) return -1;
    solar_t* s = &solars[nea.n_solar];
    s->solar_id = nea.n_solar;
    s->solar_type = slr_type;
    s->solar_category = cat;
    s->photovoltaic = pv;
    s->solar_thermal = thermal;
    s->solar_building = building;
    s->year = year;
    s->active = 1;
    nea.total_pv += pv;
    nea.n_solar++;
    print_str("[NEA] Solar "); print_int(nea.n_solar - 1);
    print_str(" type="); print_int(slr_type);
    print_str(" cat="); print_int(cat);
    print_str(" pv="); print_int(pv);
    print_str(" thm="); print_int(thermal);
    print_str(" bld="); print_int(building); print_str("\n");
    return nea.n_solar - 1;
}

int nea_wind(int wnd_type, int cat, int onshr, int offshr, int equip, int year) {
    if (nea.n_wind >= MAX_WIND) return -1;
    wind_t* w = &winds[nea.n_wind];
    w->wind_id = nea.n_wind;
    w->wind_type = wnd_type;
    w->wind_category = cat;
    w->onshore = onshr;
    w->offshore = offshr;
    w->wind_equip = equip;
    w->year = year;
    w->active = 1;
    nea.total_onshore += onshr;
    nea.n_wind++;
    print_str("[NEA] Wind "); print_int(nea.n_wind - 1);
    print_str(" type="); print_int(wnd_type);
    print_str(" cat="); print_int(cat);
    print_str(" ons="); print_int(onshr);
    print_str(" ofs="); print_int(offshr);
    print_str(" equ="); print_int(equip); print_str("\n");
    return nea.n_wind - 1;
}

int nea_nuclear(int ncl_type, int cat, int build, int fuel, int safety, int year) {
    if (nea.n_nuclear >= MAX_NUCLEAR) return -1;
    nuclear_t* n = &nuclears[nea.n_nuclear];
    n->nuclear_id = nea.n_nuclear;
    n->nuclear_type = ncl_type;
    n->nuclear_category = cat;
    n->nuclear_build = build;
    n->fuel_cycle = fuel;
    n->nuclear_safety = safety;
    n->year = year;
    n->active = 1;
    nea.total_nuclear += build;
    nea.n_nuclear++;
    print_str("[NEA] Nuclear "); print_int(nea.n_nuclear - 1);
    print_str(" type="); print_int(ncl_type);
    print_str(" cat="); print_int(cat);
    print_str(" bld="); print_int(build);
    print_str(" ful="); print_int(fuel);
    print_str(" sft="); print_int(safety); print_str("\n");
    return nea.n_nuclear - 1;
}

int nea_hydrogen(int hdg_type, int cat, int produce, int store, int cell, int year) {
    if (nea.n_hydrogen >= MAX_HYDROGEN) return -1;
    hydrogen_t* h = &hydrogens[nea.n_hydrogen];
    h->hydrogen_id = nea.n_hydrogen;
    h->hydrogen_type = hdg_type;
    h->hydrogen_category = cat;
    h->hydrogen_produce = produce;
    h->hydrogen_store = store;
    h->fuel_cell = cell;
    h->year = year;
    h->active = 1;
    nea.total_h2 += produce;
    nea.n_hydrogen++;
    print_str("[NEA] Hydrogen "); print_int(nea.n_hydrogen - 1);
    print_str(" type="); print_int(hdg_type);
    print_str(" cat="); print_int(cat);
    print_str(" prd="); print_int(produce);
    print_str(" str="); print_int(store);
    print_str(" cel="); print_int(cell); print_str("\n");
    return nea.n_hydrogen - 1;
}

int nea_biomass(int bms_type, int cat, int power, int fuel, int gas, int year) {
    if (nea.n_biomass >= MAX_BIOMASS) return -1;
    biomass_t* b = &biomasses[nea.n_biomass];
    b->biomass_id = nea.n_biomass;
    b->biomass_type = bms_type;
    b->biomass_category = cat;
    b->biomass_power = power;
    b->biofuel = fuel;
    b->biomass_gas = gas;
    b->year = year;
    b->active = 1;
    nea.total_bio += power;
    nea.n_biomass++;
    print_str("[NEA] Biomass "); print_int(nea.n_biomass - 1);
    print_str(" type="); print_int(bms_type);
    print_str(" cat="); print_int(cat);
    print_str(" pwr="); print_int(power);
    print_str(" ful="); print_int(fuel);
    print_str(" gas="); print_int(gas); print_str("\n");
    return nea.n_biomass - 1;
}

void nea_solar_report(void) {
    print_str("[NEA] Solar report:\n");
    print_str("  Solar energy categories: "); print_int(nea.n_solar); print_str("\n");
    print_str("  Total photovoltaic: "); print_int(nea.total_pv); print_str("\n");
}

void nea_wind_report(void) {
    print_str("[NEA] Wind report:\n");
    print_str("  Wind energy categories: "); print_int(nea.n_wind); print_str("\n");
    print_str("  Total onshore: "); print_int(nea.total_onshore); print_str("\n");
}

void nea_nuclear_report(void) {
    print_str("[NEA] Nuclear report:\n");
    print_str("  Nuclear energy categories: "); print_int(nea.n_nuclear); print_str("\n");
    print_str("  Total nuclear build: "); print_int(nea.total_nuclear); print_str("\n");
    print_str("  Hydrogen categories: "); print_int(nea.n_hydrogen); print_str("\n");
    print_str("  Total hydrogen produce: "); print_int(nea.total_h2); print_str("\n");
    print_str("  Biomass categories: "); print_int(nea.n_biomass); print_str("\n");
    print_str("  Total biomass power: "); print_int(nea.total_bio); print_str("\n");
}

void nea_print_state(void) {
    print_str("[NEA] Sl="); print_int(nea.n_solar);
    print_str(" Wn="); print_int(nea.n_wind);
    print_str(" Nc="); print_int(nea.n_nuclear);
    print_str(" Hg="); print_int(nea.n_hydrogen);
    print_str(" Bm="); print_int(nea.n_biomass);
    print_str("\n");
}

int main(void) {
    print_str("=== New Energy Admin Demo ===\n\n");
    nea_init();

    print_str("Solar energy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pv = 80 + (i * 20);
        int thm = 50 + (i * 12);
        int bld = 30 + (i * 7);
        int year = 2020 + (i % 5);
        nea_solar(type, cat, pv, thm, bld, year);
    }

    print_str("\nWind energy...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ons = 70 + (i * 18);
        int ofs = 45 + (i * 11);
        int equ = 25 + (i * 6);
        int year = 2021 + (i % 4);
        nea_wind(type, cat, ons, ofs, equ, year);
    }

    print_str("\nNuclear energy...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bld = 40 + (i * 10);
        int ful = 25 + (i * 6);
        int sft = 18 + (i * 4);
        int year = 2022 + (i % 3);
        nea_nuclear(type, cat, bld, ful, sft, year);
    }

    print_str("\nHydrogen energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int prd = 35 + (i * 9);
        int str = 22 + (i * 5);
        int cel = 15 + (i * 3);
        int year = 2023 + (i % 2);
        nea_hydrogen(type, cat, prd, str, cel, year);
    }

    print_str("\nBiomass energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pwr = 30 + (i * 8);
        int ful = 20 + (i * 5);
        int gas = 12 + (i * 3);
        int year = 2024;
        nea_biomass(type, cat, pwr, ful, gas, year);
    }

    print_str("\nSolar report...\n");
    nea_solar_report();

    print_str("\nWind report...\n");
    nea_wind_report();

    print_str("\nNuclear report...\n");
    nea_nuclear_report();

    print_str("\nFinal state...\n");
    nea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
