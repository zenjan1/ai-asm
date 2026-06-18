/* animal_science_admin: Animal science administration system (v1.0)
 * Genetics, breeding, nutrition, production, grassland science
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

#define MAX_AN_GENET    16
#define MAX_AN_BRED     14
#define MAX_AN_NUTR     12
#define MAX_AN_PROD     10
#define MAX_GRASS_SC    10

typedef struct {
    int    ag_id;
    int    ag_type;
    int    ag_category;
    int    quant_gen;
    int    mol_gen;
    int    gen_sel;
    int    gen_mkr;
    int    year;
    int    active;
} an_genet_t;

typedef struct {
    int    ab_id;
    int    ab_type;
    int    ab_category;
    int    brd_sel;
    int    hybrid_use;
    int    brd_pln;
    int    perf_test;
    int    year;
    int    active;
} an_bred_t;

typedef struct {
    int    an_id;
    int    an_type;
    int    an_category;
    int    nut_need;
    int    feed_form;
    int    nut_met;
    int    feed_add;
    int    year;
    int    active;
} an_nutr_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    pig_prod;
    int    cattle_prod;
    int    poultry_prod;
    int    sheep_prod;
    int    year;
    int    active;
} an_prod_t;

typedef struct {
    int    gs_id;
    int    gs_type;
    int    gs_category;
    int    grass_res;
    int    grass_eco;
    int    grass_imp;
    int    forage_prod;
    int    year;
    int    active;
} grass_sc_t;

typedef struct {
    int    n_an_genet;
    int    n_an_bred;
    int    n_an_nutr;
    int    n_an_prod;
    int    n_grass_sc;
    int    total_quant_gen;
    int    total_brd_sel;
    int    total_nut_need;
    int    total_pig_prod;
    int    total_grass_res;
} asa_state_t;

static an_genet_t an_genets[MAX_AN_GENET];
static an_bred_t an_breds[MAX_AN_BRED];
static an_nutr_t an_nutrs[MAX_AN_NUTR];
static an_prod_t an_prods[MAX_AN_PROD];
static grass_sc_t grass_scs[MAX_GRASS_SC];
static asa_state_t asa;

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

int asa_init(void) {
    if (initialized) return -1;
    asa.n_an_genet = 0; asa.n_an_bred = 0; asa.n_an_nutr = 0;
    asa.n_an_prod = 0; asa.n_grass_sc = 0;
    asa.total_quant_gen = 0; asa.total_brd_sel = 0;
    asa.total_nut_need = 0; asa.total_pig_prod = 0;
    asa.total_grass_res = 0;
    for (int i = 0; i < MAX_AN_GENET; i++) an_genets[i].active = 0;
    for (int i = 0; i < MAX_AN_BRED; i++) an_breds[i].active = 0;
    for (int i = 0; i < MAX_AN_NUTR; i++) an_nutrs[i].active = 0;
    for (int i = 0; i < MAX_AN_PROD; i++) an_prods[i].active = 0;
    for (int i = 0; i < MAX_GRASS_SC; i++) grass_scs[i].active = 0;
    initialized = 1;
    print_str("[ASA] Animal science initialized\n");
    return 0;
}

int asa_an_genet(int gt_type, int cat, int qgn, int mgn, int gsl, int gmk, int year) {
    if (asa.n_an_genet >= MAX_AN_GENET) return -1;
    an_genet_t* a = &an_genets[asa.n_an_genet];
    a->ag_id = asa.n_an_genet;
    a->ag_type = gt_type;
    a->ag_category = cat;
    a->quant_gen = qgn;
    a->mol_gen = mgn;
    a->gen_sel = gsl;
    a->gen_mkr = gmk;
    a->year = year;
    a->active = 1;
    asa.total_quant_gen += qgn;
    asa.n_an_genet++;
    print_str("[ASA] An genet "); print_int(asa.n_an_genet - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" qgn="); print_int(qgn);
    print_str(" mgn="); print_int(mgn);
    print_str(" gsl="); print_int(gsl);
    print_str(" gmk="); print_int(gmk); print_str("\n");
    return asa.n_an_genet - 1;
}

int asa_an_bred(int bt_type, int cat, int bsl, int hbu, int bpl, int pft, int year) {
    if (asa.n_an_bred >= MAX_AN_BRED) return -1;
    an_bred_t* a = &an_breds[asa.n_an_bred];
    a->ab_id = asa.n_an_bred;
    a->ab_type = bt_type;
    a->ab_category = cat;
    a->brd_sel = bsl;
    a->hybrid_use = hbu;
    a->brd_pln = bpl;
    a->perf_test = pft;
    a->year = year;
    a->active = 1;
    asa.total_brd_sel += bsl;
    asa.n_an_bred++;
    print_str("[ASA] An bred "); print_int(asa.n_an_bred - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bsl="); print_int(bsl);
    print_str(" hbu="); print_int(hbu);
    print_str(" bpl="); print_int(bpl);
    print_str(" pft="); print_int(pft); print_str("\n");
    return asa.n_an_bred - 1;
}

int asa_an_nutr(int nt_type, int cat, int nnd, int ffm, int nmt, int fad, int year) {
    if (asa.n_an_nutr >= MAX_AN_NUTR) return -1;
    an_nutr_t* a = &an_nutrs[asa.n_an_nutr];
    a->an_id = asa.n_an_nutr;
    a->an_type = nt_type;
    a->an_category = cat;
    a->nut_need = nnd;
    a->feed_form = ffm;
    a->nut_met = nmt;
    a->feed_add = fad;
    a->year = year;
    a->active = 1;
    asa.total_nut_need += nnd;
    asa.n_an_nutr++;
    print_str("[ASA] An nutr "); print_int(asa.n_an_nutr - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nnd="); print_int(nnd);
    print_str(" ffm="); print_int(ffm);
    print_str(" nmt="); print_int(nmt);
    print_str(" fad="); print_int(fad); print_str("\n");
    return asa.n_an_nutr - 1;
}

int asa_an_prod(int pt_type, int cat, int pgp, int ctp, int plp, int shp, int year) {
    if (asa.n_an_prod >= MAX_AN_PROD) return -1;
    an_prod_t* a = &an_prods[asa.n_an_prod];
    a->ap_id = asa.n_an_prod;
    a->ap_type = pt_type;
    a->ap_category = cat;
    a->pig_prod = pgp;
    a->cattle_prod = ctp;
    a->poultry_prod = plp;
    a->sheep_prod = shp;
    a->year = year;
    a->active = 1;
    asa.total_pig_prod += pgp;
    asa.n_an_prod++;
    print_str("[ASA] An prod "); print_int(asa.n_an_prod - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pgp="); print_int(pgp);
    print_str(" ctp="); print_int(ctp);
    print_str(" plp="); print_int(plp);
    print_str(" shp="); print_int(shp); print_str("\n");
    return asa.n_an_prod - 1;
}

int asa_grass_sc(int st_type, int cat, int grs, int gec, int gim, int fpr, int year) {
    if (asa.n_grass_sc >= MAX_GRASS_SC) return -1;
    grass_sc_t* g = &grass_scs[asa.n_grass_sc];
    g->gs_id = asa.n_grass_sc;
    g->gs_type = st_type;
    g->gs_category = cat;
    g->grass_res = grs;
    g->grass_eco = gec;
    g->grass_imp = gim;
    g->forage_prod = fpr;
    g->year = year;
    g->active = 1;
    asa.total_grass_res += grs;
    asa.n_grass_sc++;
    print_str("[ASA] Grass sc "); print_int(asa.n_grass_sc - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" grs="); print_int(grs);
    print_str(" gec="); print_int(gec);
    print_str(" gim="); print_int(gim);
    print_str(" fpr="); print_int(fpr); print_str("\n");
    return asa.n_grass_sc - 1;
}

void asa_genet_report(void) {
    print_str("[ASA] Genetics report:\n");
    print_str("  Genetics categories: "); print_int(asa.n_an_genet); print_str("\n");
    print_str("  Total quantitative genetics: "); print_int(asa.total_quant_gen); print_str("\n");
}

void asa_bred_report(void) {
    print_str("[ASA] Breeding report:\n");
    print_str("  Breeding categories: "); print_int(asa.n_an_bred); print_str("\n");
    print_str("  Total breed selection: "); print_int(asa.total_brd_sel); print_str("\n");
}

void asa_full_report(void) {
    print_str("[ASA] Full report:\n");
    print_str("  Nutrition categories: "); print_int(asa.n_an_nutr); print_str("\n");
    print_str("  Total nutrition needs: "); print_int(asa.total_nut_need); print_str("\n");
    print_str("  Production categories: "); print_int(asa.n_an_prod); print_str("\n");
    print_str("  Total pig production: "); print_int(asa.total_pig_prod); print_str("\n");
    print_str("  Grassland categories: "); print_int(asa.n_grass_sc); print_str("\n");
    print_str("  Total grass resources: "); print_int(asa.total_grass_res); print_str("\n");
}

void asa_print_state(void) {
    print_str("[ASA] Gn="); print_int(asa.n_an_genet);
    print_str(" Br="); print_int(asa.n_an_bred);
    print_str(" Nt="); print_int(asa.n_an_nutr);
    print_str(" Pd="); print_int(asa.n_an_prod);
    print_str(" Gs="); print_int(asa.n_grass_sc);
    print_str("\n");
}

int main(void) {
    print_str("=== Animal Science Admin Demo ===\n\n");
    asa_init();

    print_str("Genetics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int qgn = 55 + (i * 13);
        int mgn = 40 + (i * 10);
        int gsl = 22 + (i * 5);
        int gmk = 15 + (i * 3);
        int year = 2020 + (i % 5);
        asa_an_genet(type, cat, qgn, mgn, gsl, gmk, year);
    }

    print_str("\nBreeding...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bsl = 48 + (i * 11);
        int hbu = 35 + (i * 8);
        int bpl = 20 + (i * 4);
        int pft = 12 + (i * 3);
        int year = 2021 + (i % 4);
        asa_an_bred(type, cat, bsl, hbu, bpl, pft, year);
    }

    print_str("\nNutrition...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nnd = 42 + (i * 10);
        int ffm = 28 + (i * 7);
        int nmt = 18 + (i * 4);
        int fad = 10 + (i * 2);
        int year = 2022 + (i % 3);
        asa_an_nutr(type, cat, nnd, ffm, nmt, fad, year);
    }

    print_str("\nProduction...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pgp = 35 + (i * 8);
        int ctp = 25 + (i * 6);
        int plp = 15 + (i * 3);
        int shp = 10 + (i * 2);
        int year = 2023 + (i % 2);
        asa_an_prod(type, cat, pgp, ctp, plp, shp, year);
    }

    print_str("\nGrassland...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grs = 30 + (i * 7);
        int gec = 22 + (i * 5);
        int gim = 12 + (i * 3);
        int fpr = 8 + (i * 2);
        int year = 2024;
        asa_grass_sc(type, cat, grs, gec, gim, fpr, year);
    }

    print_str("\nGenetics report...\n");
    asa_genet_report();

    print_str("\nBreeding report...\n");
    asa_bred_report();

    print_str("\nFull report...\n");
    asa_full_report();

    print_str("\nFinal state...\n");
    asa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
