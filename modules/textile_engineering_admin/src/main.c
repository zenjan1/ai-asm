/* textile_engineering_admin: Textile engineering administration system (v1.0)
 * Textile materials, spinning, weaving, dyeing/finishing, garment engineering
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

#define MAX_TEXTILE_MT  16
#define MAX_SPIN_EN     14
#define MAX_WEAVE_EN    12
#define MAX_DYE_FIN     10
#define MAX_GARMENT_EN  10

typedef struct {
    int    tm_id;
    int    tm_type;
    int    tm_category;
    int    nat_fiber;
    int    chem_fiber;
    int    func_textile;
    int    year;
    int    active;
} textile_mt_t;

typedef struct {
    int    sp_id;
    int    sp_type;
    int    sp_category;
    int    cotton_spin;
    int    wool_spin;
    int    silk_spin;
    int    year;
    int    active;
} spin_en_t;

typedef struct {
    int    wv_id;
    int    wv_type;
    int    wv_category;
    int    woven_fab;
    int    knit_fab;
    int    nonwoven;
    int    year;
    int    active;
} weave_en_t;

typedef struct {
    int    df_id;
    int    df_type;
    int    df_category;
    int    pretreat;
    int    dyeing;
    int    printing;
    int    year;
    int    active;
} dye_fin_t;

typedef struct {
    int    ge_id;
    int    ge_type;
    int    ge_category;
    int    garment_ds;
    int    garment_proc;
    int    garment_prod;
    int    year;
    int    active;
} garment_en_t;

typedef struct {
    int    n_textile_mt;
    int    n_spin_en;
    int    n_weave_en;
    int    n_dye_fin;
    int    n_garment_en;
    int    total_nat_fiber;
    int    total_cotton_spin;
    int    total_woven_fab;
    int    total_pretreat;
    int    total_garment_ds;
} tea_state_t;

static textile_mt_t textile_mts[MAX_TEXTILE_MT];
static spin_en_t spin_ens[MAX_SPIN_EN];
static weave_en_t weave_ens[MAX_WEAVE_EN];
static dye_fin_t dye_fins[MAX_DYE_FIN];
static garment_en_t garment_ens[MAX_GARMENT_EN];
static tea_state_t tea;

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

int tea_init(void) {
    if (initialized) return -1;
    tea.n_textile_mt = 0; tea.n_spin_en = 0; tea.n_weave_en = 0;
    tea.n_dye_fin = 0; tea.n_garment_en = 0;
    tea.total_nat_fiber = 0; tea.total_cotton_spin = 0;
    tea.total_woven_fab = 0; tea.total_pretreat = 0;
    tea.total_garment_ds = 0;
    for (int i = 0; i < MAX_TEXTILE_MT; i++) textile_mts[i].active = 0;
    for (int i = 0; i < MAX_SPIN_EN; i++) spin_ens[i].active = 0;
    for (int i = 0; i < MAX_WEAVE_EN; i++) weave_ens[i].active = 0;
    for (int i = 0; i < MAX_DYE_FIN; i++) dye_fins[i].active = 0;
    for (int i = 0; i < MAX_GARMENT_EN; i++) garment_ens[i].active = 0;
    initialized = 1;
    print_str("[TEA] Textile engineering initialized\n");
    return 0;
}

int tea_textile_mt(int tt_type, int cat, int ntf, int chf, int ftt, int year) {
    if (tea.n_textile_mt >= MAX_TEXTILE_MT) return -1;
    textile_mt_t* t = &textile_mts[tea.n_textile_mt];
    t->tm_id = tea.n_textile_mt;
    t->tm_type = tt_type;
    t->tm_category = cat;
    t->nat_fiber = ntf;
    t->chem_fiber = chf;
    t->func_textile = ftt;
    t->year = year;
    t->active = 1;
    tea.total_nat_fiber += ntf;
    tea.n_textile_mt++;
    print_str("[TEA] Textile mt "); print_int(tea.n_textile_mt - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ntf="); print_int(ntf);
    print_str(" chf="); print_int(chf);
    print_str(" ftt="); print_int(ftt); print_str("\n");
    return tea.n_textile_mt - 1;
}

int tea_spin_en(int st_type, int cat, int cts, int wls, int sls, int year) {
    if (tea.n_spin_en >= MAX_SPIN_EN) return -1;
    spin_en_t* s = &spin_ens[tea.n_spin_en];
    s->sp_id = tea.n_spin_en;
    s->sp_type = st_type;
    s->sp_category = cat;
    s->cotton_spin = cts;
    s->wool_spin = wls;
    s->silk_spin = sls;
    s->year = year;
    s->active = 1;
    tea.total_cotton_spin += cts;
    tea.n_spin_en++;
    print_str("[TEA] Spin en "); print_int(tea.n_spin_en - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" cts="); print_int(cts);
    print_str(" wls="); print_int(wls);
    print_str(" sls="); print_int(sls); print_str("\n");
    return tea.n_spin_en - 1;
}

int tea_weave_en(int wt_type, int cat, int wvf, int knf, int nwf, int year) {
    if (tea.n_weave_en >= MAX_WEAVE_EN) return -1;
    weave_en_t* w = &weave_ens[tea.n_weave_en];
    w->wv_id = tea.n_weave_en;
    w->wv_type = wt_type;
    w->wv_category = cat;
    w->woven_fab = wvf;
    w->knit_fab = knf;
    w->nonwoven = nwf;
    w->year = year;
    w->active = 1;
    tea.total_woven_fab += wvf;
    tea.n_weave_en++;
    print_str("[TEA] Weave en "); print_int(tea.n_weave_en - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wvf="); print_int(wvf);
    print_str(" knf="); print_int(knf);
    print_str(" nwf="); print_int(nwf); print_str("\n");
    return tea.n_weave_en - 1;
}

int tea_dye_fin(int dt_type, int cat, int prt, int dyg, int pnt, int year) {
    if (tea.n_dye_fin >= MAX_DYE_FIN) return -1;
    dye_fin_t* d = &dye_fins[tea.n_dye_fin];
    d->df_id = tea.n_dye_fin;
    d->df_type = dt_type;
    d->df_category = cat;
    d->pretreat = prt;
    d->dyeing = dyg;
    d->printing = pnt;
    d->year = year;
    d->active = 1;
    tea.total_pretreat += prt;
    tea.n_dye_fin++;
    print_str("[TEA] Dye fin "); print_int(tea.n_dye_fin - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" prt="); print_int(prt);
    print_str(" dyg="); print_int(dyg);
    print_str(" pnt="); print_int(pnt); print_str("\n");
    return tea.n_dye_fin - 1;
}

int tea_garment_en(int gt_type, int cat, int gds, int gpr, int gpd, int year) {
    if (tea.n_garment_en >= MAX_GARMENT_EN) return -1;
    garment_en_t* g = &garment_ens[tea.n_garment_en];
    g->ge_id = tea.n_garment_en;
    g->ge_type = gt_type;
    g->ge_category = cat;
    g->garment_ds = gds;
    g->garment_proc = gpr;
    g->garment_prod = gpd;
    g->year = year;
    g->active = 1;
    tea.total_garment_ds += gds;
    tea.n_garment_en++;
    print_str("[TEA] Garment en "); print_int(tea.n_garment_en - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gds="); print_int(gds);
    print_str(" gpr="); print_int(gpr);
    print_str(" gpd="); print_int(gpd); print_str("\n");
    return tea.n_garment_en - 1;
}

void tea_textile_mt_report(void) {
    print_str("[TEA] Textile materials report:\n");
    print_str("  Textile materials categories: "); print_int(tea.n_textile_mt); print_str("\n");
    print_str("  Total natural fiber: "); print_int(tea.total_nat_fiber); print_str("\n");
}

void tea_spin_en_report(void) {
    print_str("[TEA] Spinning engineering report:\n");
    print_str("  Spinning categories: "); print_int(tea.n_spin_en); print_str("\n");
    print_str("  Total cotton spinning: "); print_int(tea.total_cotton_spin); print_str("\n");
}

void tea_full_report(void) {
    print_str("[TEA] Full report:\n");
    print_str("  Weaving categories: "); print_int(tea.n_weave_en); print_str("\n");
    print_str("  Total woven fabric: "); print_int(tea.total_woven_fab); print_str("\n");
    print_str("  Dyeing/finishing categories: "); print_int(tea.n_dye_fin); print_str("\n");
    print_str("  Total pretreatment: "); print_int(tea.total_pretreat); print_str("\n");
    print_str("  Garment engineering categories: "); print_int(tea.n_garment_en); print_str("\n");
    print_str("  Total garment design: "); print_int(tea.total_garment_ds); print_str("\n");
}

void tea_print_state(void) {
    print_str("[TEA] Tm="); print_int(tea.n_textile_mt);
    print_str(" Sp="); print_int(tea.n_spin_en);
    print_str(" Wv="); print_int(tea.n_weave_en);
    print_str(" Df="); print_int(tea.n_dye_fin);
    print_str(" Ge="); print_int(tea.n_garment_en);
    print_str("\n");
}

int main(void) {
    print_str("=== Textile Engineering Admin Demo ===\n\n");
    tea_init();

    print_str("Textile materials...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ntf = 55 + (i * 13);
        int chf = 40 + (i * 10);
        int ftt = 22 + (i * 5);
        int year = 2020 + (i % 5);
        tea_textile_mt(type, cat, ntf, chf, ftt, year);
    }

    print_str("\nSpinning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cts = 48 + (i * 11);
        int wls = 35 + (i * 8);
        int sls = 20 + (i * 4);
        int year = 2021 + (i % 4);
        tea_spin_en(type, cat, cts, wls, sls, year);
    }

    print_str("\nWeaving...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wvf = 42 + (i * 10);
        int knf = 28 + (i * 7);
        int nwf = 18 + (i * 4);
        int year = 2022 + (i % 3);
        tea_weave_en(type, cat, wvf, knf, nwf, year);
    }

    print_str("\nDyeing/finishing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int prt = 35 + (i * 8);
        int dyg = 25 + (i * 6);
        int pnt = 15 + (i * 3);
        int year = 2023 + (i % 2);
        tea_dye_fin(type, cat, prt, dyg, pnt, year);
    }

    print_str("\nGarment...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gds = 30 + (i * 7);
        int gpr = 22 + (i * 5);
        int gpd = 12 + (i * 3);
        int year = 2024;
        tea_garment_en(type, cat, gds, gpr, gpd, year);
    }

    print_str("\nTextile materials report...\n");
    tea_textile_mt_report();

    print_str("\nSpinning report...\n");
    tea_spin_en_report();

    print_str("\nFull report...\n");
    tea_full_report();

    print_str("\nFinal state...\n");
    tea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
