/* forestry_engineering_admin: Forestry engineering administration system (v1.0)
 * Forest cultivation, protection, management, wood science, forest products chemistry
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

#define MAX_FOREST_CV   16
#define MAX_FOREST_PT   14
#define MAX_FOREST_MG   12
#define MAX_WOOD_SC     10
#define MAX_FOREST_PC   10

typedef struct {
    int    cv_id;
    int    cv_type;
    int    cv_category;
    int    afforest;
    int    nursery;
    int    tend_fst;
    int    year;
    int    active;
} forest_cv_t;

typedef struct {
    int    pt_id;
    int    pt_type;
    int    pt_category;
    int    forest_dis;
    int    forest_pest;
    int    forest_fire;
    int    year;
    int    active;
} forest_pt_t;

typedef struct {
    int    mg_id;
    int    mg_type;
    int    mg_category;
    int    forest_inv;
    int    forest_pln;
    int    forest_res;
    int    year;
    int    active;
} forest_mg_t;

typedef struct {
    int    ws_id;
    int    ws_type;
    int    ws_category;
    int    wood_anat;
    int    wood_phys;
    int    wood_mech;
    int    year;
    int    active;
} wood_sc_t;

typedef struct {
    int    pc_id;
    int    pc_type;
    int    pc_category;
    int    forest_chem;
    int    wood_proc;
    int    biomass_en;
    int    year;
    int    active;
} forest_pc_t;

typedef struct {
    int    n_forest_cv;
    int    n_forest_pt;
    int    n_forest_mg;
    int    n_wood_sc;
    int    n_forest_pc;
    int    total_afforest;
    int    total_forest_dis;
    int    total_forest_inv;
    int    total_wood_anat;
    int    total_forest_chem;
} fea_state_t;

static forest_cv_t forest_cvs[MAX_FOREST_CV];
static forest_pt_t forest_pts[MAX_FOREST_PT];
static forest_mg_t forest_mgs[MAX_FOREST_MG];
static wood_sc_t wood_scs[MAX_WOOD_SC];
static forest_pc_t forest_pcs[MAX_FOREST_PC];
static fea_state_t fea;

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

int fea_init(void) {
    if (initialized) return -1;
    fea.n_forest_cv = 0; fea.n_forest_pt = 0; fea.n_forest_mg = 0;
    fea.n_wood_sc = 0; fea.n_forest_pc = 0;
    fea.total_afforest = 0; fea.total_forest_dis = 0;
    fea.total_forest_inv = 0; fea.total_wood_anat = 0;
    fea.total_forest_chem = 0;
    for (int i = 0; i < MAX_FOREST_CV; i++) forest_cvs[i].active = 0;
    for (int i = 0; i < MAX_FOREST_PT; i++) forest_pts[i].active = 0;
    for (int i = 0; i < MAX_FOREST_MG; i++) forest_mgs[i].active = 0;
    for (int i = 0; i < MAX_WOOD_SC; i++) wood_scs[i].active = 0;
    for (int i = 0; i < MAX_FOREST_PC; i++) forest_pcs[i].active = 0;
    initialized = 1;
    print_str("[FEA] Forestry engineering initialized\n");
    return 0;
}

int fea_forest_cv(int ct_type, int cat, int aff, int nur, int tnd, int year) {
    if (fea.n_forest_cv >= MAX_FOREST_CV) return -1;
    forest_cv_t* f = &forest_cvs[fea.n_forest_cv];
    f->cv_id = fea.n_forest_cv;
    f->cv_type = ct_type;
    f->cv_category = cat;
    f->afforest = aff;
    f->nursery = nur;
    f->tend_fst = tnd;
    f->year = year;
    f->active = 1;
    fea.total_afforest += aff;
    fea.n_forest_cv++;
    print_str("[FEA] Forest cv "); print_int(fea.n_forest_cv - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" aff="); print_int(aff);
    print_str(" nur="); print_int(nur);
    print_str(" tnd="); print_int(tnd); print_str("\n");
    return fea.n_forest_cv - 1;
}

int fea_forest_pt(int pt_type, int cat, int fdi, int fst, int ffi, int year) {
    if (fea.n_forest_pt >= MAX_FOREST_PT) return -1;
    forest_pt_t* f = &forest_pts[fea.n_forest_pt];
    f->pt_id = fea.n_forest_pt;
    f->pt_type = pt_type;
    f->pt_category = cat;
    f->forest_dis = fdi;
    f->forest_pest = fst;
    f->forest_fire = ffi;
    f->year = year;
    f->active = 1;
    fea.total_forest_dis += fdi;
    fea.n_forest_pt++;
    print_str("[FEA] Forest pt "); print_int(fea.n_forest_pt - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fdi="); print_int(fdi);
    print_str(" fst="); print_int(fst);
    print_str(" ffi="); print_int(ffi); print_str("\n");
    return fea.n_forest_pt - 1;
}

int fea_forest_mg(int mt_type, int cat, int fin, int fpl, int frs, int year) {
    if (fea.n_forest_mg >= MAX_FOREST_MG) return -1;
    forest_mg_t* f = &forest_mgs[fea.n_forest_mg];
    f->mg_id = fea.n_forest_mg;
    f->mg_type = mt_type;
    f->mg_category = cat;
    f->forest_inv = fin;
    f->forest_pln = fpl;
    f->forest_res = frs;
    f->year = year;
    f->active = 1;
    fea.total_forest_inv += fin;
    fea.n_forest_mg++;
    print_str("[FEA] Forest mg "); print_int(fea.n_forest_mg - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fin="); print_int(fin);
    print_str(" fpl="); print_int(fpl);
    print_str(" frs="); print_int(frs); print_str("\n");
    return fea.n_forest_mg - 1;
}

int fea_wood_sc(int wt_type, int cat, int wan, int wph, int wmc, int year) {
    if (fea.n_wood_sc >= MAX_WOOD_SC) return -1;
    wood_sc_t* w = &wood_scs[fea.n_wood_sc];
    w->ws_id = fea.n_wood_sc;
    w->ws_type = wt_type;
    w->ws_category = cat;
    w->wood_anat = wan;
    w->wood_phys = wph;
    w->wood_mech = wmc;
    w->year = year;
    w->active = 1;
    fea.total_wood_anat += wan;
    fea.n_wood_sc++;
    print_str("[FEA] Wood sc "); print_int(fea.n_wood_sc - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wan="); print_int(wan);
    print_str(" wph="); print_int(wph);
    print_str(" wmc="); print_int(wmc); print_str("\n");
    return fea.n_wood_sc - 1;
}

int fea_forest_pc(int pt_type, int cat, int fch, int wpr, int ben, int year) {
    if (fea.n_forest_pc >= MAX_FOREST_PC) return -1;
    forest_pc_t* f = &forest_pcs[fea.n_forest_pc];
    f->pc_id = fea.n_forest_pc;
    f->pc_type = pt_type;
    f->pc_category = cat;
    f->forest_chem = fch;
    f->wood_proc = wpr;
    f->biomass_en = ben;
    f->year = year;
    f->active = 1;
    fea.total_forest_chem += fch;
    fea.n_forest_pc++;
    print_str("[FEA] Forest pc "); print_int(fea.n_forest_pc - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fch="); print_int(fch);
    print_str(" wpr="); print_int(wpr);
    print_str(" ben="); print_int(ben); print_str("\n");
    return fea.n_forest_pc - 1;
}

void fea_forest_cv_report(void) {
    print_str("[FEA] Forest cultivation report:\n");
    print_str("  Cultivation categories: "); print_int(fea.n_forest_cv); print_str("\n");
    print_str("  Total afforestation: "); print_int(fea.total_afforest); print_str("\n");
}

void fea_forest_pt_report(void) {
    print_str("[FEA] Forest protection report:\n");
    print_str("  Protection categories: "); print_int(fea.n_forest_pt); print_str("\n");
    print_str("  Total forest disease: "); print_int(fea.total_forest_dis); print_str("\n");
}

void fea_full_report(void) {
    print_str("[FEA] Full report:\n");
    print_str("  Forest management categories: "); print_int(fea.n_forest_mg); print_str("\n");
    print_str("  Total forest inventory: "); print_int(fea.total_forest_inv); print_str("\n");
    print_str("  Wood science categories: "); print_int(fea.n_wood_sc); print_str("\n");
    print_str("  Total wood anatomy: "); print_int(fea.total_wood_anat); print_str("\n");
    print_str("  Forest products categories: "); print_int(fea.n_forest_pc); print_str("\n");
    print_str("  Total forest chemistry: "); print_int(fea.total_forest_chem); print_str("\n");
}

void fea_print_state(void) {
    print_str("[FEA] Cv="); print_int(fea.n_forest_cv);
    print_str(" Pt="); print_int(fea.n_forest_pt);
    print_str(" Mg="); print_int(fea.n_forest_mg);
    print_str(" Ws="); print_int(fea.n_wood_sc);
    print_str(" Pc="); print_int(fea.n_forest_pc);
    print_str("\n");
}

int main(void) {
    print_str("=== Forestry Engineering Admin Demo ===\n\n");
    fea_init();

    print_str("Forest cultivation...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int aff = 55 + (i * 13);
        int nur = 40 + (i * 10);
        int tnd = 22 + (i * 5);
        int year = 2020 + (i % 5);
        fea_forest_cv(type, cat, aff, nur, tnd, year);
    }

    print_str("\nForest protection...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fdi = 48 + (i * 11);
        int fst = 35 + (i * 8);
        int ffi = 20 + (i * 4);
        int year = 2021 + (i % 4);
        fea_forest_pt(type, cat, fdi, fst, ffi, year);
    }

    print_str("\nForest management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fin = 42 + (i * 10);
        int fpl = 28 + (i * 7);
        int frs = 18 + (i * 4);
        int year = 2022 + (i % 3);
        fea_forest_mg(type, cat, fin, fpl, frs, year);
    }

    print_str("\nWood science...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wan = 35 + (i * 8);
        int wph = 25 + (i * 6);
        int wmc = 15 + (i * 3);
        int year = 2023 + (i % 2);
        fea_wood_sc(type, cat, wan, wph, wmc, year);
    }

    print_str("\nForest products...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fch = 30 + (i * 7);
        int wpr = 22 + (i * 5);
        int ben = 12 + (i * 3);
        int year = 2024;
        fea_forest_pc(type, cat, fch, wpr, ben, year);
    }

    print_str("\nCultivation report...\n");
    fea_forest_cv_report();

    print_str("\nProtection report...\n");
    fea_forest_pt_report();

    print_str("\nFull report...\n");
    fea_full_report();

    print_str("\nFinal state...\n");
    fea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
