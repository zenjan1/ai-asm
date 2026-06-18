/* materials_science_admin: Materials science administration system (v1.0)
 * Metal materials, inorganic nonmetallic, polymer, composite, functional materials
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

#define MAX_METAL_MT    16
#define MAX_INORG_MT    14
#define MAX_POLY_MT     12
#define MAX_COMP_MT     10
#define MAX_FUNC_MT     10

typedef struct {
    int    mm_id;
    int    mm_type;
    int    mm_category;
    int    steel_mt;
    int    nonferrous;
    int    rare_metal;
    int    year;
    int    active;
} metal_mt_t;

typedef struct {
    int    in_id;
    int    in_type;
    int    in_category;
    int    ceramic_mt;
    int    glass_mt;
    int    cement_mt;
    int    year;
    int    active;
} inorg_mt_t;

typedef struct {
    int    po_id;
    int    po_type;
    int    po_category;
    int    plastic_mt;
    int    rubber_mt;
    int    fiber_mt;
    int    year;
    int    active;
} poly_mt_t;

typedef struct {
    int    cp_id;
    int    cp_type;
    int    cp_category;
    int    resin_comp;
    int    metal_comp;
    int    ceramic_comp;
    int    year;
    int    active;
} comp_mt_t;

typedef struct {
    int    fm_id;
    int    fm_type;
    int    fm_category;
    int    semicond_mt;
    int    magnetic_mt;
    int    supercond_mt;
    int    year;
    int    active;
} func_mt_t;

typedef struct {
    int    n_metal_mt;
    int    n_inorg_mt;
    int    n_poly_mt;
    int    n_comp_mt;
    int    n_func_mt;
    int    total_steel_mt;
    int    total_ceramic_mt;
    int    total_plastic_mt;
    int    total_resin_comp;
    int    total_semicond_mt;
} msa_state_t;

static metal_mt_t metal_mts[MAX_METAL_MT];
static inorg_mt_t inorg_mts[MAX_INORG_MT];
static poly_mt_t poly_mts[MAX_POLY_MT];
static comp_mt_t comp_mts[MAX_COMP_MT];
static func_mt_t func_mts[MAX_FUNC_MT];
static msa_state_t msa;

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

int msa_init(void) {
    if (initialized) return -1;
    msa.n_metal_mt = 0; msa.n_inorg_mt = 0; msa.n_poly_mt = 0;
    msa.n_comp_mt = 0; msa.n_func_mt = 0;
    msa.total_steel_mt = 0; msa.total_ceramic_mt = 0;
    msa.total_plastic_mt = 0; msa.total_resin_comp = 0;
    msa.total_semicond_mt = 0;
    for (int i = 0; i < MAX_METAL_MT; i++) metal_mts[i].active = 0;
    for (int i = 0; i < MAX_INORG_MT; i++) inorg_mts[i].active = 0;
    for (int i = 0; i < MAX_POLY_MT; i++) poly_mts[i].active = 0;
    for (int i = 0; i < MAX_COMP_MT; i++) comp_mts[i].active = 0;
    for (int i = 0; i < MAX_FUNC_MT; i++) func_mts[i].active = 0;
    initialized = 1;
    print_str("[MSA] Materials science initialized\n");
    return 0;
}

int msa_metal_mt(int mt_type, int cat, int stl, int nfr, int rar, int year) {
    if (msa.n_metal_mt >= MAX_METAL_MT) return -1;
    metal_mt_t* m = &metal_mts[msa.n_metal_mt];
    m->mm_id = msa.n_metal_mt;
    m->mm_type = mt_type;
    m->mm_category = cat;
    m->steel_mt = stl;
    m->nonferrous = nfr;
    m->rare_metal = rar;
    m->year = year;
    m->active = 1;
    msa.total_steel_mt += stl;
    msa.n_metal_mt++;
    print_str("[MSA] Metal mt "); print_int(msa.n_metal_mt - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" stl="); print_int(stl);
    print_str(" nfr="); print_int(nfr);
    print_str(" rar="); print_int(rar); print_str("\n");
    return msa.n_metal_mt - 1;
}

int msa_inorg_mt(int it_type, int cat, int crm, int gls, int cem, int year) {
    if (msa.n_inorg_mt >= MAX_INORG_MT) return -1;
    inorg_mt_t* n = &inorg_mts[msa.n_inorg_mt];
    n->in_id = msa.n_inorg_mt;
    n->in_type = it_type;
    n->in_category = cat;
    n->ceramic_mt = crm;
    n->glass_mt = gls;
    n->cement_mt = cem;
    n->year = year;
    n->active = 1;
    msa.total_ceramic_mt += crm;
    msa.n_inorg_mt++;
    print_str("[MSA] Inorg mt "); print_int(msa.n_inorg_mt - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" crm="); print_int(crm);
    print_str(" gls="); print_int(gls);
    print_str(" cem="); print_int(cem); print_str("\n");
    return msa.n_inorg_mt - 1;
}

int msa_poly_mt(int pt_type, int cat, int pls, int rbr, int fbr, int year) {
    if (msa.n_poly_mt >= MAX_POLY_MT) return -1;
    poly_mt_t* p = &poly_mts[msa.n_poly_mt];
    p->po_id = msa.n_poly_mt;
    p->po_type = pt_type;
    p->po_category = cat;
    p->plastic_mt = pls;
    p->rubber_mt = rbr;
    p->fiber_mt = fbr;
    p->year = year;
    p->active = 1;
    msa.total_plastic_mt += pls;
    msa.n_poly_mt++;
    print_str("[MSA] Poly mt "); print_int(msa.n_poly_mt - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pls="); print_int(pls);
    print_str(" rbr="); print_int(rbr);
    print_str(" fbr="); print_int(fbr); print_str("\n");
    return msa.n_poly_mt - 1;
}

int msa_comp_mt(int ct_type, int cat, int rsc, int mtc, int cec, int year) {
    if (msa.n_comp_mt >= MAX_COMP_MT) return -1;
    comp_mt_t* c = &comp_mts[msa.n_comp_mt];
    c->cp_id = msa.n_comp_mt;
    c->cp_type = ct_type;
    c->cp_category = cat;
    c->resin_comp = rsc;
    c->metal_comp = mtc;
    c->ceramic_comp = cec;
    c->year = year;
    c->active = 1;
    msa.total_resin_comp += rsc;
    msa.n_comp_mt++;
    print_str("[MSA] Comp mt "); print_int(msa.n_comp_mt - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" rsc="); print_int(rsc);
    print_str(" mtc="); print_int(mtc);
    print_str(" cec="); print_int(cec); print_str("\n");
    return msa.n_comp_mt - 1;
}

int msa_func_mt(int ft_type, int cat, int sem, int mag, int spc, int year) {
    if (msa.n_func_mt >= MAX_FUNC_MT) return -1;
    func_mt_t* f = &func_mts[msa.n_func_mt];
    f->fm_id = msa.n_func_mt;
    f->fm_type = ft_type;
    f->fm_category = cat;
    f->semicond_mt = sem;
    f->magnetic_mt = mag;
    f->supercond_mt = spc;
    f->year = year;
    f->active = 1;
    msa.total_semicond_mt += sem;
    msa.n_func_mt++;
    print_str("[MSA] Func mt "); print_int(msa.n_func_mt - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" sem="); print_int(sem);
    print_str(" mag="); print_int(mag);
    print_str(" spc="); print_int(spc); print_str("\n");
    return msa.n_func_mt - 1;
}

void msa_metal_mt_report(void) {
    print_str("[MSA] Metal materials report:\n");
    print_str("  Metal materials categories: "); print_int(msa.n_metal_mt); print_str("\n");
    print_str("  Total steel materials: "); print_int(msa.total_steel_mt); print_str("\n");
}

void msa_inorg_mt_report(void) {
    print_str("[MSA] Inorganic nonmetallic report:\n");
    print_str("  Inorganic categories: "); print_int(msa.n_inorg_mt); print_str("\n");
    print_str("  Total ceramic materials: "); print_int(msa.total_ceramic_mt); print_str("\n");
}

void msa_full_report(void) {
    print_str("[MSA] Full report:\n");
    print_str("  Polymer materials categories: "); print_int(msa.n_poly_mt); print_str("\n");
    print_str("  Total plastic materials: "); print_int(msa.total_plastic_mt); print_str("\n");
    print_str("  Composite materials categories: "); print_int(msa.n_comp_mt); print_str("\n");
    print_str("  Total resin composite: "); print_int(msa.total_resin_comp); print_str("\n");
    print_str("  Functional materials categories: "); print_int(msa.n_func_mt); print_str("\n");
    print_str("  Total semiconductor: "); print_int(msa.total_semicond_mt); print_str("\n");
}

void msa_print_state(void) {
    print_str("[MSA] Mm="); print_int(msa.n_metal_mt);
    print_str(" In="); print_int(msa.n_inorg_mt);
    print_str(" Po="); print_int(msa.n_poly_mt);
    print_str(" Cp="); print_int(msa.n_comp_mt);
    print_str(" Fm="); print_int(msa.n_func_mt);
    print_str("\n");
}

int main(void) {
    print_str("=== Materials Science Admin Demo ===\n\n");
    msa_init();

    print_str("Metal materials...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int stl = 55 + (i * 13);
        int nfr = 40 + (i * 10);
        int rar = 22 + (i * 5);
        int year = 2020 + (i % 5);
        msa_metal_mt(type, cat, stl, nfr, rar, year);
    }

    print_str("\nInorganic nonmetallic...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crm = 48 + (i * 11);
        int gls = 35 + (i * 8);
        int cem = 20 + (i * 4);
        int year = 2021 + (i % 4);
        msa_inorg_mt(type, cat, crm, gls, cem, year);
    }

    print_str("\nPolymer materials...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pls = 42 + (i * 10);
        int rbr = 28 + (i * 7);
        int fbr = 18 + (i * 4);
        int year = 2022 + (i % 3);
        msa_poly_mt(type, cat, pls, rbr, fbr, year);
    }

    print_str("\nComposite materials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rsc = 35 + (i * 8);
        int mtc = 25 + (i * 6);
        int cec = 15 + (i * 3);
        int year = 2023 + (i % 2);
        msa_comp_mt(type, cat, rsc, mtc, cec, year);
    }

    print_str("\nFunctional materials...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sem = 30 + (i * 7);
        int mag = 22 + (i * 5);
        int spc = 12 + (i * 3);
        int year = 2024;
        msa_func_mt(type, cat, sem, mag, spc, year);
    }

    print_str("\nMetal materials report...\n");
    msa_metal_mt_report();

    print_str("\nInorganic nonmetallic report...\n");
    msa_inorg_mt_report();

    print_str("\nFull report...\n");
    msa_full_report();

    print_str("\nFinal state...\n");
    msa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
