/* aerodynamics_admin: Aerodynamics administration system (v1.0)
 * Low-speed, high-speed, airfoil theory, boundary layer, CFD
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

#define MAX_LOW_SPEED   16
#define MAX_HIGH_SPEED  14
#define MAX_AIRFOIL     12
#define MAX_BOUNDARY    10
#define MAX_CFD         10

typedef struct {
    int    ls_id;
    int    ls_type;
    int    ls_category;
    int    bernoulli;
    int    circulation;
    int    lifting_line;
    int    year;
    int    active;
} low_speed_t;

typedef struct {
    int    hs_id;
    int    hs_type;
    int    hs_category;
    int    compressible;
    int    shock_wave;
    int    expansion;
    int    year;
    int    active;
} high_speed_t;

typedef struct {
    int    af_id;
    int    af_type;
    int    af_category;
    int    thin_airfoil;
    int    finite_span;
    int    stall_char;
    int    year;
    int    active;
} airfoil_t;

typedef struct {
    int    bl_id;
    int    bl_type;
    int    bl_category;
    int    laminar_bl;
    int    turbulent_bl;
    int    flow_sep;
    int    year;
    int    active;
} boundary_t;

typedef struct {
    int    cf_id;
    int    cf_type;
    int    cf_category;
    int    finite_diff;
    int    finite_vol;
    int    finite_elem;
    int    year;
    int    active;
} cfd_t;

typedef struct {
    int    n_low_speed;
    int    n_high_speed;
    int    n_airfoil;
    int    n_boundary;
    int    n_cfd;
    int    total_bernoulli;
    int    total_compressible;
    int    total_thin_airfoil;
    int    total_laminar_bl;
    int    total_finite_diff;
} ada_state_t;

static low_speed_t low_speeds[MAX_LOW_SPEED];
static high_speed_t high_speeds[MAX_HIGH_SPEED];
static airfoil_t airfoils[MAX_AIRFOIL];
static boundary_t boundarys[MAX_BOUNDARY];
static cfd_t cfds[MAX_CFD];
static ada_state_t ada;

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

int ada_init(void) {
    if (initialized) return -1;
    ada.n_low_speed = 0; ada.n_high_speed = 0; ada.n_airfoil = 0;
    ada.n_boundary = 0; ada.n_cfd = 0;
    ada.total_bernoulli = 0; ada.total_compressible = 0;
    ada.total_thin_airfoil = 0; ada.total_laminar_bl = 0;
    ada.total_finite_diff = 0;
    for (int i = 0; i < MAX_LOW_SPEED; i++) low_speeds[i].active = 0;
    for (int i = 0; i < MAX_HIGH_SPEED; i++) high_speeds[i].active = 0;
    for (int i = 0; i < MAX_AIRFOIL; i++) airfoils[i].active = 0;
    for (int i = 0; i < MAX_BOUNDARY; i++) boundarys[i].active = 0;
    for (int i = 0; i < MAX_CFD; i++) cfds[i].active = 0;
    initialized = 1;
    print_str("[ADA] Aerodynamics initialized\n");
    return 0;
}

int ada_low_speed(int lt_type, int cat, int bern, int circ, int lft, int year) {
    if (ada.n_low_speed >= MAX_LOW_SPEED) return -1;
    low_speed_t* l = &low_speeds[ada.n_low_speed];
    l->ls_id = ada.n_low_speed;
    l->ls_type = lt_type;
    l->ls_category = cat;
    l->bernoulli = bern;
    l->circulation = circ;
    l->lifting_line = lft;
    l->year = year;
    l->active = 1;
    ada.total_bernoulli += bern;
    ada.n_low_speed++;
    print_str("[ADA] Low speed "); print_int(ada.n_low_speed - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" brn="); print_int(bern);
    print_str(" crc="); print_int(circ);
    print_str(" lfl="); print_int(lft); print_str("\n");
    return ada.n_low_speed - 1;
}

int ada_high_speed(int ht_type, int cat, int cmp, int shk, int exp, int year) {
    if (ada.n_high_speed >= MAX_HIGH_SPEED) return -1;
    high_speed_t* h = &high_speeds[ada.n_high_speed];
    h->hs_id = ada.n_high_speed;
    h->hs_type = ht_type;
    h->hs_category = cat;
    h->compressible = cmp;
    h->shock_wave = shk;
    h->expansion = exp;
    h->year = year;
    h->active = 1;
    ada.total_compressible += cmp;
    ada.n_high_speed++;
    print_str("[ADA] High speed "); print_int(ada.n_high_speed - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" cmp="); print_int(cmp);
    print_str(" shk="); print_int(shk);
    print_str(" exp="); print_int(exp); print_str("\n");
    return ada.n_high_speed - 1;
}

int ada_airfoil(int at_type, int cat, int thn, int fsp, int stl, int year) {
    if (ada.n_airfoil >= MAX_AIRFOIL) return -1;
    airfoil_t* a = &airfoils[ada.n_airfoil];
    a->af_id = ada.n_airfoil;
    a->af_type = at_type;
    a->af_category = cat;
    a->thin_airfoil = thn;
    a->finite_span = fsp;
    a->stall_char = stl;
    a->year = year;
    a->active = 1;
    ada.total_thin_airfoil += thn;
    ada.n_airfoil++;
    print_str("[ADA] Airfoil "); print_int(ada.n_airfoil - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" thn="); print_int(thn);
    print_str(" fsp="); print_int(fsp);
    print_str(" stl="); print_int(stl); print_str("\n");
    return ada.n_airfoil - 1;
}

int ada_boundary(int bt_type, int cat, int lmn, int trb, int sep, int year) {
    if (ada.n_boundary >= MAX_BOUNDARY) return -1;
    boundary_t* b = &boundarys[ada.n_boundary];
    b->bl_id = ada.n_boundary;
    b->bl_type = bt_type;
    b->bl_category = cat;
    b->laminar_bl = lmn;
    b->turbulent_bl = trb;
    b->flow_sep = sep;
    b->year = year;
    b->active = 1;
    ada.total_laminar_bl += lmn;
    ada.n_boundary++;
    print_str("[ADA] Boundary "); print_int(ada.n_boundary - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lmn="); print_int(lmn);
    print_str(" trb="); print_int(trb);
    print_str(" sep="); print_int(sep); print_str("\n");
    return ada.n_boundary - 1;
}

int ada_cfd(int ct_type, int cat, int fdf, int fvl, int fel, int year) {
    if (ada.n_cfd >= MAX_CFD) return -1;
    cfd_t* c = &cfds[ada.n_cfd];
    c->cf_id = ada.n_cfd;
    c->cf_type = ct_type;
    c->cf_category = cat;
    c->finite_diff = fdf;
    c->finite_vol = fvl;
    c->finite_elem = fel;
    c->year = year;
    c->active = 1;
    ada.total_finite_diff += fdf;
    ada.n_cfd++;
    print_str("[ADA] CFD "); print_int(ada.n_cfd - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" fd="); print_int(fdf);
    print_str(" fv="); print_int(fvl);
    print_str(" fe="); print_int(fel); print_str("\n");
    return ada.n_cfd - 1;
}

void ada_low_speed_report(void) {
    print_str("[ADA] Low speed report:\n");
    print_str("  Low speed categories: "); print_int(ada.n_low_speed); print_str("\n");
    print_str("  Total Bernoulli: "); print_int(ada.total_bernoulli); print_str("\n");
}

void ada_high_speed_report(void) {
    print_str("[ADA] High speed report:\n");
    print_str("  High speed categories: "); print_int(ada.n_high_speed); print_str("\n");
    print_str("  Total compressible: "); print_int(ada.total_compressible); print_str("\n");
}

void ada_full_report(void) {
    print_str("[ADA] Full report:\n");
    print_str("  Airfoil categories: "); print_int(ada.n_airfoil); print_str("\n");
    print_str("  Total thin airfoil: "); print_int(ada.total_thin_airfoil); print_str("\n");
    print_str("  Boundary layer categories: "); print_int(ada.n_boundary); print_str("\n");
    print_str("  Total laminar: "); print_int(ada.total_laminar_bl); print_str("\n");
    print_str("  CFD categories: "); print_int(ada.n_cfd); print_str("\n");
    print_str("  Total finite difference: "); print_int(ada.total_finite_diff); print_str("\n");
}

void ada_print_state(void) {
    print_str("[ADA] Ls="); print_int(ada.n_low_speed);
    print_str(" Hs="); print_int(ada.n_high_speed);
    print_str(" Af="); print_int(ada.n_airfoil);
    print_str(" Bl="); print_int(ada.n_boundary);
    print_str(" Cf="); print_int(ada.n_cfd);
    print_str("\n");
}

int main(void) {
    print_str("=== Aerodynamics Admin Demo ===\n\n");
    ada_init();

    print_str("Low speed...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int bern = 55 + (i * 13);
        int circ = 40 + (i * 10);
        int lft = 22 + (i * 5);
        int year = 2020 + (i % 5);
        ada_low_speed(type, cat, bern, circ, lft, year);
    }

    print_str("\nHigh speed...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cmp = 48 + (i * 11);
        int shk = 35 + (i * 8);
        int exp = 20 + (i * 4);
        int year = 2021 + (i % 4);
        ada_high_speed(type, cat, cmp, shk, exp, year);
    }

    print_str("\nAirfoil...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int thn = 42 + (i * 10);
        int fsp = 28 + (i * 7);
        int stl = 18 + (i * 4);
        int year = 2022 + (i % 3);
        ada_airfoil(type, cat, thn, fsp, stl, year);
    }

    print_str("\nBoundary layer...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lmn = 35 + (i * 8);
        int trb = 25 + (i * 6);
        int sep = 15 + (i * 3);
        int year = 2023 + (i % 2);
        ada_boundary(type, cat, lmn, trb, sep, year);
    }

    print_str("\nCFD...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fdf = 30 + (i * 7);
        int fvl = 22 + (i * 5);
        int fel = 12 + (i * 3);
        int year = 2024;
        ada_cfd(type, cat, fdf, fvl, fel, year);
    }

    print_str("\nLow speed report...\n");
    ada_low_speed_report();

    print_str("\nHigh speed report...\n");
    ada_high_speed_report();

    print_str("\nFull report...\n");
    ada_full_report();

    print_str("\nFinal state...\n");
    ada_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
