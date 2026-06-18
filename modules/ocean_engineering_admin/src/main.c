/* ocean_engineering_admin: Ocean engineering administration system (v1.0)
 * Coastal engineering, offshore structures, subsea, marine resources, ocean energy
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

#define MAX_COASTAL     16
#define MAX_OFFSHORE    14
#define MAX_SUBSEA      12
#define MAX_RESOURCES   10
#define MAX_OCEAN_EN    10

typedef struct {
    int    co_id;
    int    co_type;
    int    co_category;
    int    breakwater;
    int    seawall;
    int    coast_prot;
    int    year;
    int    active;
} coastal_t;

typedef struct {
    int    of_id;
    int    of_type;
    int    of_category;
    int    fixed_plat;
    int    floating_plat;
    int    tlp;
    int    year;
    int    active;
} offshore_t;

typedef struct {
    int    sb_id;
    int    sb_type;
    int    sb_category;
    int    subsea_pipe;
    int    subsea_cable;
    int    subsea_prod;
    int    year;
    int    active;
} subsea_t;

typedef struct {
    int    rs_id;
    int    rs_type;
    int    rs_category;
    int    oil_gas;
    int    minerals;
    int    bio_res;
    int    year;
    int    active;
} resources_t;

typedef struct {
    int    oe_id;
    int    oe_type;
    int    oe_category;
    int    tidal_en;
    int    wave_en;
    int    current_en;
    int    year;
    int    active;
} ocean_energy_t;

typedef struct {
    int    n_coastal;
    int    n_offshore;
    int    n_subsea;
    int    n_resources;
    int    n_ocean_energy;
    int    total_breakwater;
    int    total_fixed_plat;
    int    total_subsea_pipe;
    int    total_oil_gas;
    int    total_tidal_en;
} oea_state_t;

static coastal_t coastals[MAX_COASTAL];
static offshore_t offshores[MAX_OFFSHORE];
static subsea_t subseas[MAX_SUBSEA];
static resources_t resources_list[MAX_RESOURCES];
static ocean_energy_t ocean_energies[MAX_OCEAN_EN];
static oea_state_t oea;

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

int oea_init(void) {
    if (initialized) return -1;
    oea.n_coastal = 0; oea.n_offshore = 0; oea.n_subsea = 0;
    oea.n_resources = 0; oea.n_ocean_energy = 0;
    oea.total_breakwater = 0; oea.total_fixed_plat = 0;
    oea.total_subsea_pipe = 0; oea.total_oil_gas = 0;
    oea.total_tidal_en = 0;
    for (int i = 0; i < MAX_COASTAL; i++) coastals[i].active = 0;
    for (int i = 0; i < MAX_OFFSHORE; i++) offshores[i].active = 0;
    for (int i = 0; i < MAX_SUBSEA; i++) subseas[i].active = 0;
    for (int i = 0; i < MAX_RESOURCES; i++) resources_list[i].active = 0;
    for (int i = 0; i < MAX_OCEAN_EN; i++) ocean_energies[i].active = 0;
    initialized = 1;
    print_str("[OEA] Ocean engineering initialized\n");
    return 0;
}

int oea_coastal(int ct_type, int cat, int bkw, int swl, int cpt, int year) {
    if (oea.n_coastal >= MAX_COASTAL) return -1;
    coastal_t* c = &coastals[oea.n_coastal];
    c->co_id = oea.n_coastal;
    c->co_type = ct_type;
    c->co_category = cat;
    c->breakwater = bkw;
    c->seawall = swl;
    c->coast_prot = cpt;
    c->year = year;
    c->active = 1;
    oea.total_breakwater += bkw;
    oea.n_coastal++;
    print_str("[OEA] Coastal "); print_int(oea.n_coastal - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" bkw="); print_int(bkw);
    print_str(" swl="); print_int(swl);
    print_str(" cpt="); print_int(cpt); print_str("\n");
    return oea.n_coastal - 1;
}

int oea_offshore(int of_type, int cat, int fxd, int flt, int tlp_v, int year) {
    if (oea.n_offshore >= MAX_OFFSHORE) return -1;
    offshore_t* o = &offshores[oea.n_offshore];
    o->of_id = oea.n_offshore;
    o->of_type = of_type;
    o->of_category = cat;
    o->fixed_plat = fxd;
    o->floating_plat = flt;
    o->tlp = tlp_v;
    o->year = year;
    o->active = 1;
    oea.total_fixed_plat += fxd;
    oea.n_offshore++;
    print_str("[OEA] Offshore "); print_int(oea.n_offshore - 1);
    print_str(" type="); print_int(of_type);
    print_str(" cat="); print_int(cat);
    print_str(" fxd="); print_int(fxd);
    print_str(" flt="); print_int(flt);
    print_str(" tlp="); print_int(tlp_v); print_str("\n");
    return oea.n_offshore - 1;
}

int oea_subsea(int sb_type, int cat, int pip, int cab, int prd, int year) {
    if (oea.n_subsea >= MAX_SUBSEA) return -1;
    subsea_t* s = &subseas[oea.n_subsea];
    s->sb_id = oea.n_subsea;
    s->sb_type = sb_type;
    s->sb_category = cat;
    s->subsea_pipe = pip;
    s->subsea_cable = cab;
    s->subsea_prod = prd;
    s->year = year;
    s->active = 1;
    oea.total_subsea_pipe += pip;
    oea.n_subsea++;
    print_str("[OEA] Subsea "); print_int(oea.n_subsea - 1);
    print_str(" type="); print_int(sb_type);
    print_str(" cat="); print_int(cat);
    print_str(" pip="); print_int(pip);
    print_str(" cab="); print_int(cab);
    print_str(" prd="); print_int(prd); print_str("\n");
    return oea.n_subsea - 1;
}

int oea_resources(int rs_type, int cat, int ogl, int mnr, int bio, int year) {
    if (oea.n_resources >= MAX_RESOURCES) return -1;
    resources_t* r = &resources_list[oea.n_resources];
    r->rs_id = oea.n_resources;
    r->rs_type = rs_type;
    r->rs_category = cat;
    r->oil_gas = ogl;
    r->minerals = mnr;
    r->bio_res = bio;
    r->year = year;
    r->active = 1;
    oea.total_oil_gas += ogl;
    oea.n_resources++;
    print_str("[OEA] Resources "); print_int(oea.n_resources - 1);
    print_str(" type="); print_int(rs_type);
    print_str(" cat="); print_int(cat);
    print_str(" ogl="); print_int(ogl);
    print_str(" mnr="); print_int(mnr);
    print_str(" bio="); print_int(bio); print_str("\n");
    return oea.n_resources - 1;
}

int oea_ocean_energy(int oe_type, int cat, int tid, int wav, int cur, int year) {
    if (oea.n_ocean_energy >= MAX_OCEAN_EN) return -1;
    ocean_energy_t* o = &ocean_energies[oea.n_ocean_energy];
    o->oe_id = oea.n_ocean_energy;
    o->oe_type = oe_type;
    o->oe_category = cat;
    o->tidal_en = tid;
    o->wave_en = wav;
    o->current_en = cur;
    o->year = year;
    o->active = 1;
    oea.total_tidal_en += tid;
    oea.n_ocean_energy++;
    print_str("[OEA] Ocean energy "); print_int(oea.n_ocean_energy - 1);
    print_str(" type="); print_int(oe_type);
    print_str(" cat="); print_int(cat);
    print_str(" tid="); print_int(tid);
    print_str(" wav="); print_int(wav);
    print_str(" cur="); print_int(cur); print_str("\n");
    return oea.n_ocean_energy - 1;
}

void oea_coastal_report(void) {
    print_str("[OEA] Coastal report:\n");
    print_str("  Coastal engineering categories: "); print_int(oea.n_coastal); print_str("\n");
    print_str("  Total breakwater: "); print_int(oea.total_breakwater); print_str("\n");
}

void oea_offshore_report(void) {
    print_str("[OEA] Offshore report:\n");
    print_str("  Offshore structure categories: "); print_int(oea.n_offshore); print_str("\n");
    print_str("  Total fixed platforms: "); print_int(oea.total_fixed_plat); print_str("\n");
}

void oea_full_report(void) {
    print_str("[OEA] Full report:\n");
    print_str("  Subsea engineering categories: "); print_int(oea.n_subsea); print_str("\n");
    print_str("  Total subsea pipelines: "); print_int(oea.total_subsea_pipe); print_str("\n");
    print_str("  Marine resources categories: "); print_int(oea.n_resources); print_str("\n");
    print_str("  Total oil and gas: "); print_int(oea.total_oil_gas); print_str("\n");
    print_str("  Ocean energy categories: "); print_int(oea.n_ocean_energy); print_str("\n");
    print_str("  Total tidal energy: "); print_int(oea.total_tidal_en); print_str("\n");
}

void oea_print_state(void) {
    print_str("[OEA] Co="); print_int(oea.n_coastal);
    print_str(" Of="); print_int(oea.n_offshore);
    print_str(" Sb="); print_int(oea.n_subsea);
    print_str(" Rs="); print_int(oea.n_resources);
    print_str(" Oe="); print_int(oea.n_ocean_energy);
    print_str("\n");
}

int main(void) {
    print_str("=== Ocean Engineering Admin Demo ===\n\n");
    oea_init();

    print_str("Coastal engineering...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int bkw = 55 + (i * 13);
        int swl = 40 + (i * 10);
        int cpt = 22 + (i * 5);
        int year = 2020 + (i % 5);
        oea_coastal(type, cat, bkw, swl, cpt, year);
    }

    print_str("\nOffshore structures...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fxd = 48 + (i * 11);
        int flt = 35 + (i * 8);
        int tlp_v = 20 + (i * 4);
        int year = 2021 + (i % 4);
        oea_offshore(type, cat, fxd, flt, tlp_v, year);
    }

    print_str("\nSubsea engineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pip = 42 + (i * 10);
        int cab = 28 + (i * 7);
        int prd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        oea_subsea(type, cat, pip, cab, prd, year);
    }

    print_str("\nMarine resources...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ogl = 35 + (i * 8);
        int mnr = 25 + (i * 6);
        int bio = 15 + (i * 3);
        int year = 2023 + (i % 2);
        oea_resources(type, cat, ogl, mnr, bio, year);
    }

    print_str("\nOcean energy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tid = 30 + (i * 7);
        int wav = 22 + (i * 5);
        int cur = 12 + (i * 3);
        int year = 2024;
        oea_ocean_energy(type, cat, tid, wav, cur, year);
    }

    print_str("\nCoastal report...\n");
    oea_coastal_report();

    print_str("\nOffshore report...\n");
    oea_offshore_report();

    print_str("\nFull report...\n");
    oea_full_report();

    print_str("\nFinal state...\n");
    oea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
