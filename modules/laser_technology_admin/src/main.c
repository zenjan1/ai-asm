/* laser_technology_admin: Laser technology administration system (v1.0)
 * Gas lasers, solid-state lasers, semiconductor lasers, ultrafast lasers, laser applications
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

#define MAX_GAS        16
#define MAX_SOLID      14
#define MAX_SEMI       12
#define MAX_ULTRA      10
#define MAX_APPL       10

typedef struct {
    int    gas_id;
    int    gas_type;
    int    gas_category;
    int    he_neon;
    int    co2_laser;
    int    excimer;
    int    year;
    int    active;
} gas_t;

typedef struct {
    int    solid_id;
    int    solid_type;
    int    solid_category;
    int    ruby_laser;
    int    nd_yag;
    int    fiber_laser;
    int    year;
    int    active;
} solid_t;

typedef struct {
    int    semi_id;
    int    semi_type;
    int    semi_category;
    int    laser_diode;
    int    vcsel;
    int    qcl_laser;
    int    year;
    int    active;
} semi_t;

typedef struct {
    int    ultra_id;
    int    ultra_type;
    int    ultra_category;
    int    femtosecond;
    int    picosecond;
    int    attosecond;
    int    year;
    int    active;
} ultra_t;

typedef struct {
    int    appl_id;
    int    appl_type;
    int    appl_category;
    int    laser_cut;
    int    laser_weld;
    int    laser_medical;
    int    year;
    int    active;
} appl_t;

typedef struct {
    int    n_gas;
    int    n_solid;
    int    n_semi;
    int    n_ultra;
    int    n_appl;
    int    total_he_neon;
    int    total_ruby;
    int    total_diode;
    int    total_femto;
    int    total_cut;
} lta_state_t;

static gas_t gases[MAX_GAS];
static solid_t solids[MAX_SOLID];
static semi_t semis[MAX_SEMI];
static ultra_t ultras[MAX_ULTRA];
static appl_t appls[MAX_APPL];
static lta_state_t lta;

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

int lta_init(void) {
    if (initialized) return -1;
    lta.n_gas = 0; lta.n_solid = 0; lta.n_semi = 0;
    lta.n_ultra = 0; lta.n_appl = 0;
    lta.total_he_neon = 0; lta.total_ruby = 0;
    lta.total_diode = 0; lta.total_femto = 0;
    lta.total_cut = 0;
    for (int i = 0; i < MAX_GAS; i++) gases[i].active = 0;
    for (int i = 0; i < MAX_SOLID; i++) solids[i].active = 0;
    for (int i = 0; i < MAX_SEMI; i++) semis[i].active = 0;
    for (int i = 0; i < MAX_ULTRA; i++) ultras[i].active = 0;
    for (int i = 0; i < MAX_APPL; i++) appls[i].active = 0;
    initialized = 1;
    print_str("[LTA] Laser technology initialized\n");
    return 0;
}

int lta_gas(int g_type, int cat, int hen, int co2v, int excm, int year) {
    if (lta.n_gas >= MAX_GAS) return -1;
    gas_t* g = &gases[lta.n_gas];
    g->gas_id = lta.n_gas;
    g->gas_type = g_type;
    g->gas_category = cat;
    g->he_neon = hen;
    g->co2_laser = co2v;
    g->excimer = excm;
    g->year = year;
    g->active = 1;
    lta.total_he_neon += hen;
    lta.n_gas++;
    print_str("[LTA] Gas laser "); print_int(lta.n_gas - 1);
    print_str(" type="); print_int(g_type);
    print_str(" cat="); print_int(cat);
    print_str(" hne="); print_int(hen);
    print_str(" co2="); print_int(co2v);
    print_str(" exc="); print_int(excm); print_str("\n");
    return lta.n_gas - 1;
}

int lta_solid(int sl_type, int cat, int ruby, int yag, int fiber, int year) {
    if (lta.n_solid >= MAX_SOLID) return -1;
    solid_t* s = &solids[lta.n_solid];
    s->solid_id = lta.n_solid;
    s->solid_type = sl_type;
    s->solid_category = cat;
    s->ruby_laser = ruby;
    s->nd_yag = yag;
    s->fiber_laser = fiber;
    s->year = year;
    s->active = 1;
    lta.total_ruby += ruby;
    lta.n_solid++;
    print_str("[LTA] Solid laser "); print_int(lta.n_solid - 1);
    print_str(" type="); print_int(sl_type);
    print_str(" cat="); print_int(cat);
    print_str(" rby="); print_int(ruby);
    print_str(" yag="); print_int(yag);
    print_str(" fbr="); print_int(fiber); print_str("\n");
    return lta.n_solid - 1;
}

int lta_semi(int sm_type, int cat, int diode, int vcs, int qcl, int year) {
    if (lta.n_semi >= MAX_SEMI) return -1;
    semi_t* s = &semis[lta.n_semi];
    s->semi_id = lta.n_semi;
    s->semi_type = sm_type;
    s->semi_category = cat;
    s->laser_diode = diode;
    s->vcsel = vcs;
    s->qcl_laser = qcl;
    s->year = year;
    s->active = 1;
    lta.total_diode += diode;
    lta.n_semi++;
    print_str("[LTA] Semi laser "); print_int(lta.n_semi - 1);
    print_str(" type="); print_int(sm_type);
    print_str(" cat="); print_int(cat);
    print_str(" did="); print_int(diode);
    print_str(" vcs="); print_int(vcs);
    print_str(" qcl="); print_int(qcl); print_str("\n");
    return lta.n_semi - 1;
}

int lta_ultra(int ul_type, int cat, int femto, int pico, int atto, int year) {
    if (lta.n_ultra >= MAX_ULTRA) return -1;
    ultra_t* u = &ultras[lta.n_ultra];
    u->ultra_id = lta.n_ultra;
    u->ultra_type = ul_type;
    u->ultra_category = cat;
    u->femtosecond = femto;
    u->picosecond = pico;
    u->attosecond = atto;
    u->year = year;
    u->active = 1;
    lta.total_femto += femto;
    lta.n_ultra++;
    print_str("[LTA] Ultra laser "); print_int(lta.n_ultra - 1);
    print_str(" type="); print_int(ul_type);
    print_str(" cat="); print_int(cat);
    print_str(" fem="); print_int(femto);
    print_str(" pic="); print_int(pico);
    print_str(" att="); print_int(atto); print_str("\n");
    return lta.n_ultra - 1;
}

int lta_appl(int ap_type, int cat, int cut_v, int weld_v, int med_v, int year) {
    if (lta.n_appl >= MAX_APPL) return -1;
    appl_t* a = &appls[lta.n_appl];
    a->appl_id = lta.n_appl;
    a->appl_type = ap_type;
    a->appl_category = cat;
    a->laser_cut = cut_v;
    a->laser_weld = weld_v;
    a->laser_medical = med_v;
    a->year = year;
    a->active = 1;
    lta.total_cut += cut_v;
    lta.n_appl++;
    print_str("[LTA] Laser appl "); print_int(lta.n_appl - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" cut="); print_int(cut_v);
    print_str(" wld="); print_int(weld_v);
    print_str(" med="); print_int(med_v); print_str("\n");
    return lta.n_appl - 1;
}

void lta_gas_report(void) {
    print_str("[LTA] Gas laser report:\n");
    print_str("  Gas laser categories: "); print_int(lta.n_gas); print_str("\n");
    print_str("  Total He-Neon lasers: "); print_int(lta.total_he_neon); print_str("\n");
}

void lta_solid_report(void) {
    print_str("[LTA] Solid laser report:\n");
    print_str("  Solid-state laser categories: "); print_int(lta.n_solid); print_str("\n");
    print_str("  Total ruby lasers: "); print_int(lta.total_ruby); print_str("\n");
}

void lta_appl_report(void) {
    print_str("[LTA] Application report:\n");
    print_str("  Semiconductor laser categories: "); print_int(lta.n_semi); print_str("\n");
    print_str("  Total laser diodes: "); print_int(lta.total_diode); print_str("\n");
    print_str("  Ultrafast laser categories: "); print_int(lta.n_ultra); print_str("\n");
    print_str("  Total femtosecond lasers: "); print_int(lta.total_femto); print_str("\n");
    print_str("  Laser application categories: "); print_int(lta.n_appl); print_str("\n");
    print_str("  Total laser cutting: "); print_int(lta.total_cut); print_str("\n");
}

void lta_print_state(void) {
    print_str("[LTA] Gs="); print_int(lta.n_gas);
    print_str(" Sl="); print_int(lta.n_solid);
    print_str(" Sm="); print_int(lta.n_semi);
    print_str(" Ul="); print_int(lta.n_ultra);
    print_str(" Ap="); print_int(lta.n_appl);
    print_str("\n");
}

int main(void) {
    print_str("=== Laser Technology Admin Demo ===\n\n");
    lta_init();

    print_str("Gas lasers...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int hne = 55 + (i * 13);
        int co2 = 40 + (i * 10);
        int exc = 22 + (i * 5);
        int year = 2020 + (i % 5);
        lta_gas(type, cat, hne, co2, exc, year);
    }

    print_str("\nSolid-state lasers...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rby = 48 + (i * 12);
        int yag = 35 + (i * 8);
        int fbr = 20 + (i * 5);
        int year = 2021 + (i % 4);
        lta_solid(type, cat, rby, yag, fbr, year);
    }

    print_str("\nSemiconductor lasers...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int did = 42 + (i * 10);
        int vcs = 28 + (i * 7);
        int qcl = 18 + (i * 4);
        int year = 2022 + (i % 3);
        lta_semi(type, cat, did, vcs, qcl, year);
    }

    print_str("\nUltrafast lasers...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fem = 35 + (i * 8);
        int pic = 25 + (i * 6);
        int att = 15 + (i * 3);
        int year = 2023 + (i % 2);
        lta_ultra(type, cat, fem, pic, att, year);
    }

    print_str("\nLaser applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cut = 32 + (i * 8);
        int wld = 22 + (i * 5);
        int med = 12 + (i * 3);
        int year = 2024;
        lta_appl(type, cat, cut, wld, med, year);
    }

    print_str("\nGas laser report...\n");
    lta_gas_report();

    print_str("\nSolid laser report...\n");
    lta_solid_report();

    print_str("\nApplication report...\n");
    lta_appl_report();

    print_str("\nFinal state...\n");
    lta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
