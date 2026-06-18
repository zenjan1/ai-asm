/* astronomy_admin: Astronomy administration system (v1.0)
 * General astronomy, stellar astronomy, galactic astronomy, cosmology, space exploration
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

#define MAX_GEN_ASTR    16
#define MAX_STEL_ASTR   14
#define MAX_GAL_ASTR    12
#define MAX_COSM        10
#define MAX_SPACE_D     10

typedef struct {
    int    ga_id;
    int    ga_type;
    int    ga_category;
    int    ast_mea;
    int    ast_mec;
    int    ast_phy;
    int    cos_bas;
    int    year;
    int    active;
} gen_astr_t;

typedef struct {
    int    sa_id;
    int    sa_type;
    int    sa_category;
    int    stel_phy;
    int    stel_evo;
    int    var_star;
    int    int_med;
    int    year;
    int    active;
} stel_astr_t;

typedef struct {
    int    gl_id;
    int    gl_type;
    int    gl_category;
    int    mil_way;
    int    ext_gal;
    int    act_gal;
    int    gal_evo;
    int    year;
    int    active;
} gal_astr_t;

typedef struct {
    int    co_id;
    int    co_type;
    int    co_category;
    int    big_bang;
    int    dark_mat;
    int    cos_mic;
    int    lar_str;
    int    year;
    int    active;
} cosm_t;

typedef struct {
    int    sd_id;
    int    sd_type;
    int    sd_category;
    int    sat_exp;
    int    pla_exp;
    int    spa_tel;
    int    dee_spa;
    int    year;
    int    active;
} space_d_t;

typedef struct {
    int    n_gen_astr;
    int    n_stel_astr;
    int    n_gal_astr;
    int    n_cosm;
    int    n_space_d;
    int    total_ast_mea;
    int    total_stel_phy;
    int    total_mil_way;
    int    total_big_bang;
    int    total_sat_exp;
} aro_state_t;

static gen_astr_t gen_astrs[MAX_GEN_ASTR];
static stel_astr_t stel_astrs[MAX_STEL_ASTR];
static gal_astr_t gal_astrs[MAX_GAL_ASTR];
static cosm_t cosms[MAX_COSM];
static space_d_t space_ds[MAX_SPACE_D];
static aro_state_t aro;

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

int aro_init(void) {
    if (initialized) return -1;
    aro.n_gen_astr = 0; aro.n_stel_astr = 0; aro.n_gal_astr = 0;
    aro.n_cosm = 0; aro.n_space_d = 0;
    aro.total_ast_mea = 0; aro.total_stel_phy = 0;
    aro.total_mil_way = 0; aro.total_big_bang = 0;
    aro.total_sat_exp = 0;
    for (int i = 0; i < MAX_GEN_ASTR; i++) gen_astrs[i].active = 0;
    for (int i = 0; i < MAX_STEL_ASTR; i++) stel_astrs[i].active = 0;
    for (int i = 0; i < MAX_GAL_ASTR; i++) gal_astrs[i].active = 0;
    for (int i = 0; i < MAX_COSM; i++) cosms[i].active = 0;
    for (int i = 0; i < MAX_SPACE_D; i++) space_ds[i].active = 0;
    initialized = 1;
    print_str("[ARO] Astronomy initialized\n");
    return 0;
}

int aro_gen_astr(int gt_type, int cat, int ame, int amc, int aph, int cba, int year) {
    if (aro.n_gen_astr >= MAX_GEN_ASTR) return -1;
    gen_astr_t* g = &gen_astrs[aro.n_gen_astr];
    g->ga_id = aro.n_gen_astr;
    g->ga_type = gt_type;
    g->ga_category = cat;
    g->ast_mea = ame;
    g->ast_mec = amc;
    g->ast_phy = aph;
    g->cos_bas = cba;
    g->year = year;
    g->active = 1;
    aro.total_ast_mea += ame;
    aro.n_gen_astr++;
    print_str("[ARO] Gen astr "); print_int(aro.n_gen_astr - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ame="); print_int(ame);
    print_str(" amc="); print_int(amc);
    print_str(" aph="); print_int(aph);
    print_str(" cba="); print_int(cba); print_str("\n");
    return aro.n_gen_astr - 1;
}

int aro_stel_astr(int st_type, int cat, int sph, int seo, int vst, int imd, int year) {
    if (aro.n_stel_astr >= MAX_STEL_ASTR) return -1;
    stel_astr_t* s = &stel_astrs[aro.n_stel_astr];
    s->sa_id = aro.n_stel_astr;
    s->sa_type = st_type;
    s->sa_category = cat;
    s->stel_phy = sph;
    s->stel_evo = seo;
    s->var_star = vst;
    s->int_med = imd;
    s->year = year;
    s->active = 1;
    aro.total_stel_phy += sph;
    aro.n_stel_astr++;
    print_str("[ARO] Stell astr "); print_int(aro.n_stel_astr - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sph="); print_int(sph);
    print_str(" seo="); print_int(seo);
    print_str(" vst="); print_int(vst);
    print_str(" imd="); print_int(imd); print_str("\n");
    return aro.n_stel_astr - 1;
}

int aro_gal_astr(int gt_type, int cat, int mlw, int exg, int agx, int gev, int year) {
    if (aro.n_gal_astr >= MAX_GAL_ASTR) return -1;
    gal_astr_t* g = &gal_astrs[aro.n_gal_astr];
    g->gl_id = aro.n_gal_astr;
    g->gl_type = gt_type;
    g->gl_category = cat;
    g->mil_way = mlw;
    g->ext_gal = exg;
    g->act_gal = agx;
    g->gal_evo = gev;
    g->year = year;
    g->active = 1;
    aro.total_mil_way += mlw;
    aro.n_gal_astr++;
    print_str("[ARO] Gal astr "); print_int(aro.n_gal_astr - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mlw="); print_int(mlw);
    print_str(" exg="); print_int(exg);
    print_str(" agx="); print_int(agx);
    print_str(" gev="); print_int(gev); print_str("\n");
    return aro.n_gal_astr - 1;
}

int aro_cosm(int ct_type, int cat, int bbg, int dma, int cmb, int lst, int year) {
    if (aro.n_cosm >= MAX_COSM) return -1;
    cosm_t* c = &cosms[aro.n_cosm];
    c->co_id = aro.n_cosm;
    c->co_type = ct_type;
    c->co_category = cat;
    c->big_bang = bbg;
    c->dark_mat = dma;
    c->cos_mic = cmb;
    c->lar_str = lst;
    c->year = year;
    c->active = 1;
    aro.total_big_bang += bbg;
    aro.n_cosm++;
    print_str("[ARO] Cosm "); print_int(aro.n_cosm - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" bbg="); print_int(bbg);
    print_str(" dma="); print_int(dma);
    print_str(" cmb="); print_int(cmb);
    print_str(" lst="); print_int(lst); print_str("\n");
    return aro.n_cosm - 1;
}

int aro_space_d(int st_type, int cat, int sex, int pex, int stel, int dsp, int year) {
    if (aro.n_space_d >= MAX_SPACE_D) return -1;
    space_d_t* s = &space_ds[aro.n_space_d];
    s->sd_id = aro.n_space_d;
    s->sd_type = st_type;
    s->sd_category = cat;
    s->sat_exp = sex;
    s->pla_exp = pex;
    s->spa_tel = stel;
    s->dee_spa = dsp;
    s->year = year;
    s->active = 1;
    aro.total_sat_exp += sex;
    aro.n_space_d++;
    print_str("[ARO] Space d "); print_int(aro.n_space_d - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sex="); print_int(sex);
    print_str(" pex="); print_int(pex);
    print_str(" stel="); print_int(stel);
    print_str(" dsp="); print_int(dsp); print_str("\n");
    return aro.n_space_d - 1;
}

void aro_gen_report(void) {
    print_str("[ARO] General astronomy report:\n");
    print_str("  General categories: "); print_int(aro.n_gen_astr); print_str("\n");
    print_str("  Total astrometry: "); print_int(aro.total_ast_mea); print_str("\n");
}

void aro_stel_report(void) {
    print_str("[ARO] Stellar astronomy report:\n");
    print_str("  Stellar categories: "); print_int(aro.n_stel_astr); print_str("\n");
    print_str("  Total stellar physics: "); print_int(aro.total_stel_phy); print_str("\n");
}

void aro_full_report(void) {
    print_str("[ARO] Full report:\n");
    print_str("  Galactic categories: "); print_int(aro.n_gal_astr); print_str("\n");
    print_str("  Total Milky Way studies: "); print_int(aro.total_mil_way); print_str("\n");
    print_str("  Cosmology categories: "); print_int(aro.n_cosm); print_str("\n");
    print_str("  Total Big Bang studies: "); print_int(aro.total_big_bang); print_str("\n");
    print_str("  Space exploration categories: "); print_int(aro.n_space_d); print_str("\n");
    print_str("  Total satellite exploration: "); print_int(aro.total_sat_exp); print_str("\n");
}

void aro_print_state(void) {
    print_str("[ARO] Ga="); print_int(aro.n_gen_astr);
    print_str(" Sa="); print_int(aro.n_stel_astr);
    print_str(" Gl="); print_int(aro.n_gal_astr);
    print_str(" Co="); print_int(aro.n_cosm);
    print_str(" Sd="); print_int(aro.n_space_d);
    print_str("\n");
}

int main(void) {
    print_str("=== Astronomy Admin Demo ===\n\n");
    aro_init();

    print_str("General astronomy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ame = 55 + (i * 13);
        int amc = 40 + (i * 10);
        int aph = 22 + (i * 5);
        int cba = 15 + (i * 3);
        int year = 2020 + (i % 5);
        aro_gen_astr(type, cat, ame, amc, aph, cba, year);
    }

    print_str("\nStellar astronomy...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sph = 48 + (i * 11);
        int seo = 35 + (i * 8);
        int vst = 20 + (i * 4);
        int imd = 12 + (i * 3);
        int year = 2021 + (i % 4);
        aro_stel_astr(type, cat, sph, seo, vst, imd, year);
    }

    print_str("\nGalactic astronomy...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mlw = 42 + (i * 10);
        int exg = 28 + (i * 7);
        int agx = 18 + (i * 4);
        int gev = 10 + (i * 2);
        int year = 2022 + (i % 3);
        aro_gal_astr(type, cat, mlw, exg, agx, gev, year);
    }

    print_str("\nCosmology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bbg = 35 + (i * 8);
        int dma = 25 + (i * 6);
        int cmb = 15 + (i * 3);
        int lst = 10 + (i * 2);
        int year = 2023 + (i % 2);
        aro_cosm(type, cat, bbg, dma, cmb, lst, year);
    }

    print_str("\nSpace exploration...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sex = 30 + (i * 7);
        int pex = 22 + (i * 5);
        int stel = 12 + (i * 3);
        int dsp = 8 + (i * 2);
        int year = 2024;
        aro_space_d(type, cat, sex, pex, stel, dsp, year);
    }

    print_str("\nGeneral report...\n");
    aro_gen_report();

    print_str("\nStellar report...\n");
    aro_stel_report();

    print_str("\nFull report...\n");
    aro_full_report();

    print_str("\nFinal state...\n");
    aro_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
