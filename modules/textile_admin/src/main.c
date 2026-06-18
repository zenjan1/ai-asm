/* textile_admin: Textile administration system (v1.0)
 * Textile materials, spinning, weaving, dyeing/finishing, apparel design
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

#define MAX_TXM_MAT     16
#define MAX_SPIN_ENG    14
#define MAX_WEA_ENG     12
#define MAX_DYE_FIN     10
#define MAX_APP_DES     10

typedef struct {
    int    tm_id;
    int    tm_type;
    int    tm_category;
    int    nat_fib;
    int    che_fib;
    int    fun_fib;
    int    sma_fib;
    int    year;
    int    active;
} txm_mat_t;

typedef struct {
    int    sp_id;
    int    sp_type;
    int    sp_category;
    int    cot_spin;
    int    woo_spin;
    int    fla_spin;
    int    sil_spin;
    int    year;
    int    active;
} spin_eng_t;

typedef struct {
    int    we_id;
    int    we_type;
    int    we_category;
    int    mac_wea;
    int    kni_wea;
    int    non_wea;
    int    bra_wea;
    int    year;
    int    active;
} wea_eng_t;

typedef struct {
    int    df_id;
    int    df_type;
    int    df_category;
    int    pre_tre;
    int    dye_pro;
    int    pri_pro;
    int    fin_pro;
    int    year;
    int    active;
} dye_fin_t;

typedef struct {
    int    ad_id;
    int    ad_type;
    int    ad_category;
    int    app_str;
    int    app_tec;
    int    app_cad;
    int    app_dis;
    int    year;
    int    active;
} app_des_t;

typedef struct {
    int    n_txm_mat;
    int    n_spin_eng;
    int    n_wea_eng;
    int    n_dye_fin;
    int    n_app_des;
    int    total_nat_fib;
    int    total_cot_spin;
    int    total_mac_wea;
    int    total_pre_tre;
    int    total_app_str;
} txa_state_t;

static txm_mat_t txm_mats[MAX_TXM_MAT];
static spin_eng_t spin_engs[MAX_SPIN_ENG];
static wea_eng_t wea_engs[MAX_WEA_ENG];
static dye_fin_t dye_fins[MAX_DYE_FIN];
static app_des_t app_dess[MAX_APP_DES];
static txa_state_t txa;

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

int txa_init(void) {
    if (initialized) return -1;
    txa.n_txm_mat = 0; txa.n_spin_eng = 0; txa.n_wea_eng = 0;
    txa.n_dye_fin = 0; txa.n_app_des = 0;
    txa.total_nat_fib = 0; txa.total_cot_spin = 0;
    txa.total_mac_wea = 0; txa.total_pre_tre = 0;
    txa.total_app_str = 0;
    for (int i = 0; i < MAX_TXM_MAT; i++) txm_mats[i].active = 0;
    for (int i = 0; i < MAX_SPIN_ENG; i++) spin_engs[i].active = 0;
    for (int i = 0; i < MAX_WEA_ENG; i++) wea_engs[i].active = 0;
    for (int i = 0; i < MAX_DYE_FIN; i++) dye_fins[i].active = 0;
    for (int i = 0; i < MAX_APP_DES; i++) app_dess[i].active = 0;
    initialized = 1;
    print_str("[TXA] Textile initialized\n");
    return 0;
}

int txa_txm_mat(int mt_type, int cat, int nfb, int cfb, int ffb, int sfb, int year) {
    if (txa.n_txm_mat >= MAX_TXM_MAT) return -1;
    txm_mat_t* t = &txm_mats[txa.n_txm_mat];
    t->tm_id = txa.n_txm_mat;
    t->tm_type = mt_type;
    t->tm_category = cat;
    t->nat_fib = nfb;
    t->che_fib = cfb;
    t->fun_fib = ffb;
    t->sma_fib = sfb;
    t->year = year;
    t->active = 1;
    txa.total_nat_fib += nfb;
    txa.n_txm_mat++;
    print_str("[TXA] Txm mat "); print_int(txa.n_txm_mat - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nfb="); print_int(nfb);
    print_str(" cfb="); print_int(cfb);
    print_str(" ffb="); print_int(ffb);
    print_str(" sfb="); print_int(sfb); print_str("\n");
    return txa.n_txm_mat - 1;
}

int txa_spin_eng(int st_type, int cat, int cst, int wsp, int fsp, int ssp, int year) {
    if (txa.n_spin_eng >= MAX_SPIN_ENG) return -1;
    spin_eng_t* s = &spin_engs[txa.n_spin_eng];
    s->sp_id = txa.n_spin_eng;
    s->sp_type = st_type;
    s->sp_category = cat;
    s->cot_spin = cst;
    s->woo_spin = wsp;
    s->fla_spin = fsp;
    s->sil_spin = ssp;
    s->year = year;
    s->active = 1;
    txa.total_cot_spin += cst;
    txa.n_spin_eng++;
    print_str("[TXA] Spin eng "); print_int(txa.n_spin_eng - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cst);
    print_str(" wsp="); print_int(wsp);
    print_str(" fsp="); print_int(fsp);
    print_str(" ssp="); print_int(ssp); print_str("\n");
    return txa.n_spin_eng - 1;
}

int txa_wea_eng(int wt_type, int cat, int mwe, int kwe, int nwe, int bwe, int year) {
    if (txa.n_wea_eng >= MAX_WEA_ENG) return -1;
    wea_eng_t* w = &wea_engs[txa.n_wea_eng];
    w->we_id = txa.n_wea_eng;
    w->we_type = wt_type;
    w->we_category = cat;
    w->mac_wea = mwe;
    w->kni_wea = kwe;
    w->non_wea = nwe;
    w->bra_wea = bwe;
    w->year = year;
    w->active = 1;
    txa.total_mac_wea += mwe;
    txa.n_wea_eng++;
    print_str("[TXA] Wea eng "); print_int(txa.n_wea_eng - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mwe="); print_int(mwe);
    print_str(" kwe="); print_int(kwe);
    print_str(" nwe="); print_int(nwe);
    print_str(" bwe="); print_int(bwe); print_str("\n");
    return txa.n_wea_eng - 1;
}

int txa_dye_fin(int dt_type, int cat, int prt, int dpr, int prp, int fpr, int year) {
    if (txa.n_dye_fin >= MAX_DYE_FIN) return -1;
    dye_fin_t* d = &dye_fins[txa.n_dye_fin];
    d->df_id = txa.n_dye_fin;
    d->df_type = dt_type;
    d->df_category = cat;
    d->pre_tre = prt;
    d->dye_pro = dpr;
    d->pri_pro = prp;
    d->fin_pro = fpr;
    d->year = year;
    d->active = 1;
    txa.total_pre_tre += prt;
    txa.n_dye_fin++;
    print_str("[TXA] Dye fin "); print_int(txa.n_dye_fin - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" prt="); print_int(prt);
    print_str(" dpr="); print_int(dpr);
    print_str(" prp="); print_int(prp);
    print_str(" fpr="); print_int(fpr); print_str("\n");
    return txa.n_dye_fin - 1;
}

int txa_app_des(int at_type, int cat, int ast, int ate, int acd, int adi, int year) {
    if (txa.n_app_des >= MAX_APP_DES) return -1;
    app_des_t* a = &app_dess[txa.n_app_des];
    a->ad_id = txa.n_app_des;
    a->ad_type = at_type;
    a->ad_category = cat;
    a->app_str = ast;
    a->app_tec = ate;
    a->app_cad = acd;
    a->app_dis = adi;
    a->year = year;
    a->active = 1;
    txa.total_app_str += ast;
    txa.n_app_des++;
    print_str("[TXA] App des "); print_int(txa.n_app_des - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" ast="); print_int(ast);
    print_str(" ate="); print_int(ate);
    print_str(" acd="); print_int(acd);
    print_str(" adi="); print_int(adi); print_str("\n");
    return txa.n_app_des - 1;
}

void txa_material_report(void) {
    print_str("[TXA] Material report:\n");
    print_str("  Material categories: "); print_int(txa.n_txm_mat); print_str("\n");
    print_str("  Total natural fibers: "); print_int(txa.total_nat_fib); print_str("\n");
}

void txa_spinning_report(void) {
    print_str("[TXA] Spinning report:\n");
    print_str("  Spinning categories: "); print_int(txa.n_spin_eng); print_str("\n");
    print_str("  Total cotton spinning: "); print_int(txa.total_cot_spin); print_str("\n");
}

void txa_full_report(void) {
    print_str("[TXA] Full report:\n");
    print_str("  Weaving categories: "); print_int(txa.n_wea_eng); print_str("\n");
    print_str("  Total machine weaving: "); print_int(txa.total_mac_wea); print_str("\n");
    print_str("  Dyeing categories: "); print_int(txa.n_dye_fin); print_str("\n");
    print_str("  Total pre-treatment: "); print_int(txa.total_pre_tre); print_str("\n");
    print_str("  Apparel categories: "); print_int(txa.n_app_des); print_str("\n");
    print_str("  Total apparel structures: "); print_int(txa.total_app_str); print_str("\n");
}

void txa_print_state(void) {
    print_str("[TXA] Tm="); print_int(txa.n_txm_mat);
    print_str(" Sp="); print_int(txa.n_spin_eng);
    print_str(" We="); print_int(txa.n_wea_eng);
    print_str(" Df="); print_int(txa.n_dye_fin);
    print_str(" Ad="); print_int(txa.n_app_des);
    print_str("\n");
}

int main(void) {
    print_str("=== Textile Admin Demo ===\n\n");
    txa_init();

    print_str("Textile materials...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int nfb = 55 + (i * 13);
        int cfb = 40 + (i * 10);
        int ffb = 22 + (i * 5);
        int sfb = 15 + (i * 3);
        int year = 2020 + (i % 5);
        txa_txm_mat(type, cat, nfb, cfb, ffb, sfb, year);
    }

    print_str("\nSpinning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cst = 48 + (i * 11);
        int wsp = 35 + (i * 8);
        int fsp = 20 + (i * 4);
        int ssp = 12 + (i * 3);
        int year = 2021 + (i % 4);
        txa_spin_eng(type, cat, cst, wsp, fsp, ssp, year);
    }

    print_str("\nWeaving...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mwe = 42 + (i * 10);
        int kwe = 28 + (i * 7);
        int nwe = 18 + (i * 4);
        int bwe = 10 + (i * 2);
        int year = 2022 + (i % 3);
        txa_wea_eng(type, cat, mwe, kwe, nwe, bwe, year);
    }

    print_str("\nDyeing/finishing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int prt = 35 + (i * 8);
        int dpr = 25 + (i * 6);
        int prp = 15 + (i * 3);
        int fpr = 10 + (i * 2);
        int year = 2023 + (i % 2);
        txa_dye_fin(type, cat, prt, dpr, prp, fpr, year);
    }

    print_str("\nApparel design...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ast = 30 + (i * 7);
        int ate = 22 + (i * 5);
        int acd = 12 + (i * 3);
        int adi = 8 + (i * 2);
        int year = 2024;
        txa_app_des(type, cat, ast, ate, acd, adi, year);
    }

    print_str("\nMaterial report...\n");
    txa_material_report();

    print_str("\nSpinning report...\n");
    txa_spinning_report();

    print_str("\nFull report...\n");
    txa_full_report();

    print_str("\nFinal state...\n");
    txa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
