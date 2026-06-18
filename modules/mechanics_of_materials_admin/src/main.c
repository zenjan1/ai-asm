/* mechanics_of_materials_admin: Mechanics of materials administration (v1.0)
 * Axial loading, torsion, bending, shear, combined deformation
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

#define MAX_AXIAL       16
#define MAX_TORSION     14
#define MAX_BENDING     12
#define MAX_SHEAR       10
#define MAX_COMBINED    10

typedef struct {
    int    ax_id;
    int    ax_type;
    int    ax_category;
    int    stress_strain;
    int    hooke_law;
    int    strength_cond;
    int    year;
    int    active;
} axial_t;

typedef struct {
    int    ts_id;
    int    ts_type;
    int    ts_category;
    int    torque;
    int    shear_stress;
    int    twist_angle;
    int    year;
    int    active;
} torsion_t;

typedef struct {
    int    bn_id;
    int    bn_type;
    int    bn_category;
    int    bending_moment;
    int    shear_force;
    int    deflection;
    int    year;
    int    active;
} bending_t;

typedef struct {
    int    sh_id;
    int    sh_type;
    int    sh_category;
    int    shear_stress_sh;
    int    bearing_stress;
    int    conn_strength;
    int    year;
    int    active;
} shear_t;

typedef struct {
    int    cb_id;
    int    cb_type;
    int    cb_category;
    int    tension_bend;
    int    bend_torsion;
    int    stress_state;
    int    year;
    int    active;
} combined_t;

typedef struct {
    int    n_axial;
    int    n_torsion;
    int    n_bending;
    int    n_shear;
    int    n_combined;
    int    total_stress_strain;
    int    total_torque;
    int    total_bending_moment;
    int    total_shear_stress;
    int    total_tension_bend;
} mma_state_t;

static axial_t axials[MAX_AXIAL];
static torsion_t torsions[MAX_TORSION];
static bending_t bendings[MAX_BENDING];
static shear_t shears[MAX_SHEAR];
static combined_t combineds[MAX_COMBINED];
static mma_state_t mma;

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

int mma_init(void) {
    if (initialized) return -1;
    mma.n_axial = 0; mma.n_torsion = 0; mma.n_bending = 0;
    mma.n_shear = 0; mma.n_combined = 0;
    mma.total_stress_strain = 0; mma.total_torque = 0;
    mma.total_bending_moment = 0; mma.total_shear_stress = 0;
    mma.total_tension_bend = 0;
    for (int i = 0; i < MAX_AXIAL; i++) axials[i].active = 0;
    for (int i = 0; i < MAX_TORSION; i++) torsions[i].active = 0;
    for (int i = 0; i < MAX_BENDING; i++) bendings[i].active = 0;
    for (int i = 0; i < MAX_SHEAR; i++) shears[i].active = 0;
    for (int i = 0; i < MAX_COMBINED; i++) combineds[i].active = 0;
    initialized = 1;
    print_str("[MMA] Mechanics of materials initialized\n");
    return 0;
}

int mma_axial(int ax_type, int cat, int str_s, int huk, int stc, int year) {
    if (mma.n_axial >= MAX_AXIAL) return -1;
    axial_t* a = &axials[mma.n_axial];
    a->ax_id = mma.n_axial;
    a->ax_type = ax_type;
    a->ax_category = cat;
    a->stress_strain = str_s;
    a->hooke_law = huk;
    a->strength_cond = stc;
    a->year = year;
    a->active = 1;
    mma.total_stress_strain += str_s;
    mma.n_axial++;
    print_str("[MMA] Axial "); print_int(mma.n_axial - 1);
    print_str(" type="); print_int(ax_type);
    print_str(" cat="); print_int(cat);
    print_str(" str="); print_int(str_s);
    print_str(" huk="); print_int(huk);
    print_str(" stc="); print_int(stc); print_str("\n");
    return mma.n_axial - 1;
}

int mma_torsion(int tr_type, int cat, int trq, int shs, int twa, int year) {
    if (mma.n_torsion >= MAX_TORSION) return -1;
    torsion_t* t = &torsions[mma.n_torsion];
    t->ts_id = mma.n_torsion;
    t->ts_type = tr_type;
    t->ts_category = cat;
    t->torque = trq;
    t->shear_stress = shs;
    t->twist_angle = twa;
    t->year = year;
    t->active = 1;
    mma.total_torque += trq;
    mma.n_torsion++;
    print_str("[MMA] Torsion "); print_int(mma.n_torsion - 1);
    print_str(" type="); print_int(tr_type);
    print_str(" cat="); print_int(cat);
    print_str(" trq="); print_int(trq);
    print_str(" shs="); print_int(shs);
    print_str(" twa="); print_int(twa); print_str("\n");
    return mma.n_torsion - 1;
}

int mma_bending(int bn_type, int cat, int bmt, int shf, int dfl, int year) {
    if (mma.n_bending >= MAX_BENDING) return -1;
    bending_t* b = &bendings[mma.n_bending];
    b->bn_id = mma.n_bending;
    b->bn_type = bn_type;
    b->bn_category = cat;
    b->bending_moment = bmt;
    b->shear_force = shf;
    b->deflection = dfl;
    b->year = year;
    b->active = 1;
    mma.total_bending_moment += bmt;
    mma.n_bending++;
    print_str("[MMA] Bending "); print_int(mma.n_bending - 1);
    print_str(" type="); print_int(bn_type);
    print_str(" cat="); print_int(cat);
    print_str(" bmt="); print_int(bmt);
    print_str(" shf="); print_int(shf);
    print_str(" dfl="); print_int(dfl); print_str("\n");
    return mma.n_bending - 1;
}

int mma_shear(int sh_type, int cat, int shs_s, int brs, int cst, int year) {
    if (mma.n_shear >= MAX_SHEAR) return -1;
    shear_t* s = &shears[mma.n_shear];
    s->sh_id = mma.n_shear;
    s->sh_type = sh_type;
    s->sh_category = cat;
    s->shear_stress_sh = shs_s;
    s->bearing_stress = brs;
    s->conn_strength = cst;
    s->year = year;
    s->active = 1;
    mma.total_shear_stress += shs_s;
    mma.n_shear++;
    print_str("[MMA] Shear "); print_int(mma.n_shear - 1);
    print_str(" type="); print_int(sh_type);
    print_str(" cat="); print_int(cat);
    print_str(" shs="); print_int(shs_s);
    print_str(" brs="); print_int(brs);
    print_str(" cst="); print_int(cst); print_str("\n");
    return mma.n_shear - 1;
}

int mma_combined(int cb_type, int cat, int tnb, int bnt, int sts, int year) {
    if (mma.n_combined >= MAX_COMBINED) return -1;
    combined_t* c = &combineds[mma.n_combined];
    c->cb_id = mma.n_combined;
    c->cb_type = cb_type;
    c->cb_category = cat;
    c->tension_bend = tnb;
    c->bend_torsion = bnt;
    c->stress_state = sts;
    c->year = year;
    c->active = 1;
    mma.total_tension_bend += tnb;
    mma.n_combined++;
    print_str("[MMA] Combined "); print_int(mma.n_combined - 1);
    print_str(" type="); print_int(cb_type);
    print_str(" cat="); print_int(cat);
    print_str(" tnb="); print_int(tnb);
    print_str(" bnt="); print_int(bnt);
    print_str(" sts="); print_int(sts); print_str("\n");
    return mma.n_combined - 1;
}

void mma_axial_report(void) {
    print_str("[MMA] Axial report:\n");
    print_str("  Axial loading categories: "); print_int(mma.n_axial); print_str("\n");
    print_str("  Total stress-strain: "); print_int(mma.total_stress_strain); print_str("\n");
}

void mma_torsion_report(void) {
    print_str("[MMA] Torsion report:\n");
    print_str("  Torsion categories: "); print_int(mma.n_torsion); print_str("\n");
    print_str("  Total torque: "); print_int(mma.total_torque); print_str("\n");
}

void mma_full_report(void) {
    print_str("[MMA] Full report:\n");
    print_str("  Bending categories: "); print_int(mma.n_bending); print_str("\n");
    print_str("  Total bending moment: "); print_int(mma.total_bending_moment); print_str("\n");
    print_str("  Shear categories: "); print_int(mma.n_shear); print_str("\n");
    print_str("  Total shear stress: "); print_int(mma.total_shear_stress); print_str("\n");
    print_str("  Combined deformation categories: "); print_int(mma.n_combined); print_str("\n");
    print_str("  Total tension-bending: "); print_int(mma.total_tension_bend); print_str("\n");
}

void mma_print_state(void) {
    print_str("[MMA] Ax="); print_int(mma.n_axial);
    print_str(" Ts="); print_int(mma.n_torsion);
    print_str(" Bn="); print_int(mma.n_bending);
    print_str(" Sh="); print_int(mma.n_shear);
    print_str(" Cb="); print_int(mma.n_combined);
    print_str("\n");
}

int main(void) {
    print_str("=== Mechanics of Materials Admin Demo ===\n\n");
    mma_init();

    print_str("Axial loading...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int str_s = 55 + (i * 13);
        int huk = 40 + (i * 10);
        int stc = 22 + (i * 5);
        int year = 2020 + (i % 5);
        mma_axial(type, cat, str_s, huk, stc, year);
    }

    print_str("\nTorsion...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int trq = 48 + (i * 11);
        int shs = 35 + (i * 8);
        int twa = 20 + (i * 4);
        int year = 2021 + (i % 4);
        mma_torsion(type, cat, trq, shs, twa, year);
    }

    print_str("\nBending...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bmt = 42 + (i * 10);
        int shf = 28 + (i * 7);
        int dfl = 18 + (i * 4);
        int year = 2022 + (i % 3);
        mma_bending(type, cat, bmt, shf, dfl, year);
    }

    print_str("\nShear...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int shs_s = 35 + (i * 8);
        int brs = 25 + (i * 6);
        int cst = 15 + (i * 3);
        int year = 2023 + (i % 2);
        mma_shear(type, cat, shs_s, brs, cst, year);
    }

    print_str("\nCombined deformation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tnb = 30 + (i * 7);
        int bnt = 22 + (i * 5);
        int sts = 12 + (i * 3);
        int year = 2024;
        mma_combined(type, cat, tnb, bnt, sts, year);
    }

    print_str("\nAxial report...\n");
    mma_axial_report();

    print_str("\nTorsion report...\n");
    mma_torsion_report();

    print_str("\nFull report...\n");
    mma_full_report();

    print_str("\nFinal state...\n");
    mma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
