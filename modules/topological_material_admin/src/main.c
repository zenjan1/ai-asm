/* topological_material_admin: Topological materials administration system (v1.0)
 * Topological insulators, semimetals, superconductors, magnetic topological, applications
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

#define MAX_INSULATOR    16
#define MAX_SEMI         14
#define MAX_SC           12
#define MAX_MAGNETIC     10
#define MAX_APP          10

typedef struct {
    int    ins_id;
    int    ins_type;
    int    ins_category;
    int    bisb_ti;
    int    bi2te3_ti;
    int    bi2se3_ti;
    int    year;
    int    active;
} insulator_t;

typedef struct {
    int    semi_id;
    int    semi_type;
    int    semi_category;
    int    weyl_sm;
    int    dirac_sm;
    int    nodal_sm;
    int    year;
    int    active;
} semi_t;

typedef struct {
    int    sc_id;
    int    sc_type;
    int    sc_category;
    int    cu_bi2se3;
    int    nanowire;
    int    mbi2se3;
    int    year;
    int    active;
} sc_t;

typedef struct {
    int    mag_id;
    int    mag_type;
    int    mag_category;
    int    mag_ti;
    int    antiferro;
    int    mag_weyl;
    int    year;
    int    active;
} magnetic_t;

typedef struct {
    int    app_id;
    int    app_type;
    int    app_category;
    int    spintronics;
    int    quantum_comp;
    int    low_dissip;
    int    year;
    int    active;
} app_t;

typedef struct {
    int    n_insulator;
    int    n_semi;
    int    n_sc;
    int    n_magnetic;
    int    n_app;
    int    total_bisb;
    int    total_weyl;
    int    total_cu;
    int    total_mag_ti;
    int    total_spin;
} tms_state_t;

static insulator_t insulators[MAX_INSULATOR];
static semi_t semis[MAX_SEMI];
static sc_t scs[MAX_SC];
static magnetic_t magnetics[MAX_MAGNETIC];
static app_t apps[MAX_APP];
static tms_state_t tms;

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

int tms_init(void) {
    if (initialized) return -1;
    tms.n_insulator = 0; tms.n_semi = 0; tms.n_sc = 0;
    tms.n_magnetic = 0; tms.n_app = 0;
    tms.total_bisb = 0; tms.total_weyl = 0;
    tms.total_cu = 0; tms.total_mag_ti = 0;
    tms.total_spin = 0;
    for (int i = 0; i < MAX_INSULATOR; i++) insulators[i].active = 0;
    for (int i = 0; i < MAX_SEMI; i++) semis[i].active = 0;
    for (int i = 0; i < MAX_SC; i++) scs[i].active = 0;
    for (int i = 0; i < MAX_MAGNETIC; i++) magnetics[i].active = 0;
    for (int i = 0; i < MAX_APP; i++) apps[i].active = 0;
    initialized = 1;
    print_str("[TMS] Topological materials initialized\n");
    return 0;
}

int tms_insulator(int in_type, int cat, int bisb_v, int bi2te, int bi2se, int year) {
    if (tms.n_insulator >= MAX_INSULATOR) return -1;
    insulator_t* ins = &insulators[tms.n_insulator];
    ins->ins_id = tms.n_insulator;
    ins->ins_type = in_type;
    ins->ins_category = cat;
    ins->bisb_ti = bisb_v;
    ins->bi2te3_ti = bi2te;
    ins->bi2se3_ti = bi2se;
    ins->year = year;
    ins->active = 1;
    tms.total_bisb += bisb_v;
    tms.n_insulator++;
    print_str("[TMS] Insulator "); print_int(tms.n_insulator - 1);
    print_str(" type="); print_int(in_type);
    print_str(" cat="); print_int(cat);
    print_str(" bsb="); print_int(bisb_v);
    print_str(" bte="); print_int(bi2te);
    print_str(" bse="); print_int(bi2se); print_str("\n");
    return tms.n_insulator - 1;
}

int tms_semi(int sm_type, int cat, int weyl_v, int dirac_v, int nodal_v, int year) {
    if (tms.n_semi >= MAX_SEMI) return -1;
    semi_t* s = &semis[tms.n_semi];
    s->semi_id = tms.n_semi;
    s->semi_type = sm_type;
    s->semi_category = cat;
    s->weyl_sm = weyl_v;
    s->dirac_sm = dirac_v;
    s->nodal_sm = nodal_v;
    s->year = year;
    s->active = 1;
    tms.total_weyl += weyl_v;
    tms.n_semi++;
    print_str("[TMS] Semi "); print_int(tms.n_semi - 1);
    print_str(" type="); print_int(sm_type);
    print_str(" cat="); print_int(cat);
    print_str(" eyl="); print_int(weyl_v);
    print_str(" drs="); print_int(dirac_v);
    print_str(" ndl="); print_int(nodal_v); print_str("\n");
    return tms.n_semi - 1;
}

int tms_sc(int sc_type, int cat, int cu_bi, int nano_w, int mbi, int year) {
    if (tms.n_sc >= MAX_SC) return -1;
    sc_t* s = &scs[tms.n_sc];
    s->sc_id = tms.n_sc;
    s->sc_type = sc_type;
    s->sc_category = cat;
    s->cu_bi2se3 = cu_bi;
    s->nanowire = nano_w;
    s->mbi2se3 = mbi;
    s->year = year;
    s->active = 1;
    tms.total_cu += cu_bi;
    tms.n_sc++;
    print_str("[TMS] SC "); print_int(tms.n_sc - 1);
    print_str(" type="); print_int(sc_type);
    print_str(" cat="); print_int(cat);
    print_str(" cbi="); print_int(cu_bi);
    print_str(" nwr="); print_int(nano_w);
    print_str(" mbi="); print_int(mbi); print_str("\n");
    return tms.n_sc - 1;
}

int tms_magnetic(int mg_type, int cat, int mti, int afm, int mweyl, int year) {
    if (tms.n_magnetic >= MAX_MAGNETIC) return -1;
    magnetic_t* m = &magnetics[tms.n_magnetic];
    m->mag_id = tms.n_magnetic;
    m->mag_type = mg_type;
    m->mag_category = cat;
    m->mag_ti = mti;
    m->antiferro = afm;
    m->mag_weyl = mweyl;
    m->year = year;
    m->active = 1;
    tms.total_mag_ti += mti;
    tms.n_magnetic++;
    print_str("[TMS] Magnetic "); print_int(tms.n_magnetic - 1);
    print_str(" type="); print_int(mg_type);
    print_str(" cat="); print_int(cat);
    print_str(" mti="); print_int(mti);
    print_str(" afm="); print_int(afm);
    print_str(" mwl="); print_int(mweyl); print_str("\n");
    return tms.n_magnetic - 1;
}

int tms_app(int ap_type, int cat, int spin_v, int qcomp, int lowd, int year) {
    if (tms.n_app >= MAX_APP) return -1;
    app_t* a = &apps[tms.n_app];
    a->app_id = tms.n_app;
    a->app_type = ap_type;
    a->app_category = cat;
    a->spintronics = spin_v;
    a->quantum_comp = qcomp;
    a->low_dissip = lowd;
    a->year = year;
    a->active = 1;
    tms.total_spin += spin_v;
    tms.n_app++;
    print_str("[TMS] App "); print_int(tms.n_app - 1);
    print_str(" type="); print_int(ap_type);
    print_str(" cat="); print_int(cat);
    print_str(" spn="); print_int(spin_v);
    print_str(" qcm="); print_int(qcomp);
    print_str(" lwd="); print_int(lowd); print_str("\n");
    return tms.n_app - 1;
}

void tms_insulator_report(void) {
    print_str("[TMS] Insulator report:\n");
    print_str("  Topological insulator categories: "); print_int(tms.n_insulator); print_str("\n");
    print_str("  Total BiSb TI: "); print_int(tms.total_bisb); print_str("\n");
}

void tms_semi_report(void) {
    print_str("[TMS] Semi report:\n");
    print_str("  Topological semimetal categories: "); print_int(tms.n_semi); print_str("\n");
    print_str("  Total Weyl semimetal: "); print_int(tms.total_weyl); print_str("\n");
}

void tms_app_report(void) {
    print_str("[TMS] App report:\n");
    print_str("  Topological SC categories: "); print_int(tms.n_sc); print_str("\n");
    print_str("  Total Cu-doped Bi2Se3: "); print_int(tms.total_cu); print_str("\n");
    print_str("  Magnetic topological categories: "); print_int(tms.n_magnetic); print_str("\n");
    print_str("  Total magnetic TI: "); print_int(tms.total_mag_ti); print_str("\n");
    print_str("  Topological application categories: "); print_int(tms.n_app); print_str("\n");
    print_str("  Total spintronics: "); print_int(tms.total_spin); print_str("\n");
}

void tms_print_state(void) {
    print_str("[TMS] In="); print_int(tms.n_insulator);
    print_str(" Sm="); print_int(tms.n_semi);
    print_str(" Sc="); print_int(tms.n_sc);
    print_str(" Mg="); print_int(tms.n_magnetic);
    print_str(" Ap="); print_int(tms.n_app);
    print_str("\n");
}

int main(void) {
    print_str("=== Topological Materials Admin Demo ===\n\n");
    tms_init();

    print_str("Topological insulators...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int bsb = 58 + (i * 14);
        int bte = 42 + (i * 10);
        int bse = 25 + (i * 6);
        int year = 2020 + (i % 5);
        tms_insulator(type, cat, bsb, bte, bse, year);
    }

    print_str("\nTopological semimetals...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int eyl = 50 + (i * 12);
        int drs = 35 + (i * 8);
        int ndl = 22 + (i * 5);
        int year = 2021 + (i % 4);
        tms_semi(type, cat, eyl, drs, ndl, year);
    }

    print_str("\nTopological SC...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cbi = 45 + (i * 11);
        int nwr = 30 + (i * 7);
        int mbi = 18 + (i * 4);
        int year = 2022 + (i % 3);
        tms_sc(type, cat, cbi, nwr, mbi, year);
    }

    print_str("\nMagnetic topological...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mti = 38 + (i * 9);
        int afm = 25 + (i * 6);
        int mwl = 15 + (i * 3);
        int year = 2023 + (i % 2);
        tms_magnetic(type, cat, mti, afm, mwl, year);
    }

    print_str("\nTopological applications...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int spn = 32 + (i * 8);
        int qcm = 22 + (i * 5);
        int lwd = 12 + (i * 3);
        int year = 2024;
        tms_app(type, cat, spn, qcm, lwd, year);
    }

    print_str("\nInsulator report...\n");
    tms_insulator_report();

    print_str("\nSemi report...\n");
    tms_semi_report();

    print_str("\nApp report...\n");
    tms_app_report();

    print_str("\nFinal state...\n");
    tms_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
