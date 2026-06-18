/* economics_admin: Economics administration system (v1.0)
 * Microeconomics, macroeconomics, international economics, econometrics, development
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

#define MAX_MICRO_E     16
#define MAX_MACRO_E     14
#define MAX_INTL_E      12
#define MAX_ECONO_M     10
#define MAX_DEV_E       10

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    cons_th;
    int    prod_th;
    int    mkt_str;
    int    game_th;
    int    year;
    int    active;
} micro_e_t;

typedef struct {
    int    ma_id;
    int    ma_type;
    int    ma_category;
    int    nat_inc;
    int    mon_th;
    int    fis_pol;
    int    mon_pol;
    int    year;
    int    active;
} macro_e_t;

typedef struct {
    int    ie_id;
    int    ie_type;
    int    ie_category;
    int    int_tra;
    int    int_fin;
    int    int_inv;
    int    glo_eco;
    int    year;
    int    active;
} intl_e_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    reg_ana;
    int    time_ser;
    int    pan_dat;
    int    cau_inf;
    int    year;
    int    active;
} econo_m_t;

typedef struct {
    int    de_id;
    int    de_type;
    int    de_category;
    int    eco_grw;
    int    povert;
    int    inc_dis;
    int    sustain;
    int    year;
    int    active;
} dev_e_t;

typedef struct {
    int    n_micro_e;
    int    n_macro_e;
    int    n_intl_e;
    int    n_econo_m;
    int    n_dev_e;
    int    total_cons_th;
    int    total_nat_inc;
    int    total_int_tra;
    int    total_reg_ana;
    int    total_eco_grw;
} eca_state_t;

static micro_e_t micro_es[MAX_MICRO_E];
static macro_e_t macro_es[MAX_MACRO_E];
static intl_e_t intl_es[MAX_INTL_E];
static econo_m_t econo_ms[MAX_ECONO_M];
static dev_e_t dev_es[MAX_DEV_E];
static eca_state_t eca;

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

int eca_init(void) {
    if (initialized) return -1;
    eca.n_micro_e = 0; eca.n_macro_e = 0; eca.n_intl_e = 0;
    eca.n_econo_m = 0; eca.n_dev_e = 0;
    eca.total_cons_th = 0; eca.total_nat_inc = 0;
    eca.total_int_tra = 0; eca.total_reg_ana = 0;
    eca.total_eco_grw = 0;
    for (int i = 0; i < MAX_MICRO_E; i++) micro_es[i].active = 0;
    for (int i = 0; i < MAX_MACRO_E; i++) macro_es[i].active = 0;
    for (int i = 0; i < MAX_INTL_E; i++) intl_es[i].active = 0;
    for (int i = 0; i < MAX_ECONO_M; i++) econo_ms[i].active = 0;
    for (int i = 0; i < MAX_DEV_E; i++) dev_es[i].active = 0;
    initialized = 1;
    print_str("[ECA] Economics initialized\n");
    return 0;
}

int eca_micro_e(int mt_type, int cat, int cst, int pth, int mks, int gth, int year) {
    if (eca.n_micro_e >= MAX_MICRO_E) return -1;
    micro_e_t* m = &micro_es[eca.n_micro_e];
    m->me_id = eca.n_micro_e;
    m->me_type = mt_type;
    m->me_category = cat;
    m->cons_th = cst;
    m->prod_th = pth;
    m->mkt_str = mks;
    m->game_th = gth;
    m->year = year;
    m->active = 1;
    eca.total_cons_th += cst;
    eca.n_micro_e++;
    print_str("[ECA] Micro e "); print_int(eca.n_micro_e - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cst="); print_int(cst);
    print_str(" pth="); print_int(pth);
    print_str(" mks="); print_int(mks);
    print_str(" gth="); print_int(gth); print_str("\n");
    return eca.n_micro_e - 1;
}

int eca_macro_e(int mt_type, int cat, int nin, int mth, int fpl, int mpl, int year) {
    if (eca.n_macro_e >= MAX_MACRO_E) return -1;
    macro_e_t* m = &macro_es[eca.n_macro_e];
    m->ma_id = eca.n_macro_e;
    m->ma_type = mt_type;
    m->ma_category = cat;
    m->nat_inc = nin;
    m->mon_th = mth;
    m->fis_pol = fpl;
    m->mon_pol = mpl;
    m->year = year;
    m->active = 1;
    eca.total_nat_inc += nin;
    eca.n_macro_e++;
    print_str("[ECA] Macro e "); print_int(eca.n_macro_e - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" nin="); print_int(nin);
    print_str(" mth="); print_int(mth);
    print_str(" fpl="); print_int(fpl);
    print_str(" mpl="); print_int(mpl); print_str("\n");
    return eca.n_macro_e - 1;
}

int eca_intl_e(int it_type, int cat, int itr, int ifn, int iiv, int gec, int year) {
    if (eca.n_intl_e >= MAX_INTL_E) return -1;
    intl_e_t* n = &intl_es[eca.n_intl_e];
    n->ie_id = eca.n_intl_e;
    n->ie_type = it_type;
    n->ie_category = cat;
    n->int_tra = itr;
    n->int_fin = ifn;
    n->int_inv = iiv;
    n->glo_eco = gec;
    n->year = year;
    n->active = 1;
    eca.total_int_tra += itr;
    eca.n_intl_e++;
    print_str("[ECA] Intl e "); print_int(eca.n_intl_e - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" itr="); print_int(itr);
    print_str(" ifn="); print_int(ifn);
    print_str(" iiv="); print_int(iiv);
    print_str(" gec="); print_int(gec); print_str("\n");
    return eca.n_intl_e - 1;
}

int eca_econo_m(int et_type, int cat, int ran, int tms, int pnd, int cin, int year) {
    if (eca.n_econo_m >= MAX_ECONO_M) return -1;
    econo_m_t* e = &econo_ms[eca.n_econo_m];
    e->em_id = eca.n_econo_m;
    e->em_type = et_type;
    e->em_category = cat;
    e->reg_ana = ran;
    e->time_ser = tms;
    e->pan_dat = pnd;
    e->cau_inf = cin;
    e->year = year;
    e->active = 1;
    eca.total_reg_ana += ran;
    eca.n_econo_m++;
    print_str("[ECA] Econo m "); print_int(eca.n_econo_m - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" ran="); print_int(ran);
    print_str(" tms="); print_int(tms);
    print_str(" pnd="); print_int(pnd);
    print_str(" cin="); print_int(cin); print_str("\n");
    return eca.n_econo_m - 1;
}

int eca_dev_e(int dt_type, int cat, int egr, int pvt, int ids, int sus, int year) {
    if (eca.n_dev_e >= MAX_DEV_E) return -1;
    dev_e_t* d = &dev_es[eca.n_dev_e];
    d->de_id = eca.n_dev_e;
    d->de_type = dt_type;
    d->de_category = cat;
    d->eco_grw = egr;
    d->povert = pvt;
    d->inc_dis = ids;
    d->sustain = sus;
    d->year = year;
    d->active = 1;
    eca.total_eco_grw += egr;
    eca.n_dev_e++;
    print_str("[ECA] Dev e "); print_int(eca.n_dev_e - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" egr="); print_int(egr);
    print_str(" pvt="); print_int(pvt);
    print_str(" ids="); print_int(ids);
    print_str(" sus="); print_int(sus); print_str("\n");
    return eca.n_dev_e - 1;
}

void eca_micro_report(void) {
    print_str("[ECA] Microeconomics report:\n");
    print_str("  Micro categories: "); print_int(eca.n_micro_e); print_str("\n");
    print_str("  Total consumer theory: "); print_int(eca.total_cons_th); print_str("\n");
}

void eca_macro_report(void) {
    print_str("[ECA] Macroeconomics report:\n");
    print_str("  Macro categories: "); print_int(eca.n_macro_e); print_str("\n");
    print_str("  Total national income: "); print_int(eca.total_nat_inc); print_str("\n");
}

void eca_full_report(void) {
    print_str("[ECA] Full report:\n");
    print_str("  International categories: "); print_int(eca.n_intl_e); print_str("\n");
    print_str("  Total international trade: "); print_int(eca.total_int_tra); print_str("\n");
    print_str("  Econometrics categories: "); print_int(eca.n_econo_m); print_str("\n");
    print_str("  Total regression analysis: "); print_int(eca.total_reg_ana); print_str("\n");
    print_str("  Development categories: "); print_int(eca.n_dev_e); print_str("\n");
    print_str("  Total economic growth: "); print_int(eca.total_eco_grw); print_str("\n");
}

void eca_print_state(void) {
    print_str("[ECA] Me="); print_int(eca.n_micro_e);
    print_str(" Ma="); print_int(eca.n_macro_e);
    print_str(" Ie="); print_int(eca.n_intl_e);
    print_str(" Em="); print_int(eca.n_econo_m);
    print_str(" De="); print_int(eca.n_dev_e);
    print_str("\n");
}

int main(void) {
    print_str("=== Economics Admin Demo ===\n\n");
    eca_init();

    print_str("Microeconomics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cst = 55 + (i * 13);
        int pth = 40 + (i * 10);
        int mks = 22 + (i * 5);
        int gth = 15 + (i * 3);
        int year = 2020 + (i % 5);
        eca_micro_e(type, cat, cst, pth, mks, gth, year);
    }

    print_str("\nMacroeconomics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nin = 48 + (i * 11);
        int mth = 35 + (i * 8);
        int fpl = 20 + (i * 4);
        int mpl = 12 + (i * 3);
        int year = 2021 + (i % 4);
        eca_macro_e(type, cat, nin, mth, fpl, mpl, year);
    }

    print_str("\nInternational economics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int itr = 42 + (i * 10);
        int ifn = 28 + (i * 7);
        int iiv = 18 + (i * 4);
        int gec = 10 + (i * 2);
        int year = 2022 + (i % 3);
        eca_intl_e(type, cat, itr, ifn, iiv, gec, year);
    }

    print_str("\nEconometrics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ran = 35 + (i * 8);
        int tms = 25 + (i * 6);
        int pnd = 15 + (i * 3);
        int cin = 10 + (i * 2);
        int year = 2023 + (i % 2);
        eca_econo_m(type, cat, ran, tms, pnd, cin, year);
    }

    print_str("\nDevelopment economics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int egr = 30 + (i * 7);
        int pvt = 22 + (i * 5);
        int ids = 12 + (i * 3);
        int sus = 8 + (i * 2);
        int year = 2024;
        eca_dev_e(type, cat, egr, pvt, ids, sus, year);
    }

    print_str("\nMicro report...\n");
    eca_micro_report();

    print_str("\nMacro report...\n");
    eca_macro_report();

    print_str("\nFull report...\n");
    eca_full_report();

    print_str("\nFinal state...\n");
    eca_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
