/* mathematics_admin: Mathematics administration system (v1.0)
 * Basic math, applied math, computational math, probability, operations research
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

#define MAX_BASIC_M     16
#define MAX_APP_M       14
#define MAX_COMP_M      12
#define MAX_PROB_S      10
#define MAX_OPER_R      10

typedef struct {
    int    bm_id;
    int    bm_type;
    int    bm_category;
    int    math_ana;
    int    high_alg;
    int    geo_top;
    int    num_theory;
    int    year;
    int    active;
} basic_m_t;

typedef struct {
    int    am_id;
    int    am_type;
    int    am_category;
    int    dif_eq;
    int    prob_st;
    int    comp_math;
    int    oper_res;
    int    year;
    int    active;
} app_m_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    num_ana;
    int    math_mod;
    int    sci_comp;
    int    alg_des;
    int    year;
    int    active;
} comp_m_t;

typedef struct {
    int    ps_id;
    int    ps_type;
    int    ps_category;
    int    prob_th;
    int    math_st;
    int    ran_pro;
    int    bay_st;
    int    year;
    int    active;
} prob_s_t;

typedef struct {
    int    or_id;
    int    or_type;
    int    or_category;
    int    lin_pro;
    int    int_pro;
    int    dyn_pro;
    int    graph_th;
    int    year;
    int    active;
} oper_r_t;

typedef struct {
    int    n_basic_m;
    int    n_app_m;
    int    n_comp_m;
    int    n_prob_s;
    int    n_oper_r;
    int    total_math_ana;
    int    total_dif_eq;
    int    total_num_ana;
    int    total_prob_th;
    int    total_lin_pro;
} maa_state_t;

static basic_m_t basic_ms[MAX_BASIC_M];
static app_m_t app_ms[MAX_APP_M];
static comp_m_t comp_ms[MAX_COMP_M];
static prob_s_t prob_ss[MAX_PROB_S];
static oper_r_t oper_rs[MAX_OPER_R];
static maa_state_t maa;

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

int maa_init(void) {
    if (initialized) return -1;
    maa.n_basic_m = 0; maa.n_app_m = 0; maa.n_comp_m = 0;
    maa.n_prob_s = 0; maa.n_oper_r = 0;
    maa.total_math_ana = 0; maa.total_dif_eq = 0;
    maa.total_num_ana = 0; maa.total_prob_th = 0;
    maa.total_lin_pro = 0;
    for (int i = 0; i < MAX_BASIC_M; i++) basic_ms[i].active = 0;
    for (int i = 0; i < MAX_APP_M; i++) app_ms[i].active = 0;
    for (int i = 0; i < MAX_COMP_M; i++) comp_ms[i].active = 0;
    for (int i = 0; i < MAX_PROB_S; i++) prob_ss[i].active = 0;
    for (int i = 0; i < MAX_OPER_R; i++) oper_rs[i].active = 0;
    initialized = 1;
    print_str("[MAA] Mathematics initialized\n");
    return 0;
}

int maa_basic_m(int bt_type, int cat, int man, int hal, int gto, int nth, int year) {
    if (maa.n_basic_m >= MAX_BASIC_M) return -1;
    basic_m_t* b = &basic_ms[maa.n_basic_m];
    b->bm_id = maa.n_basic_m;
    b->bm_type = bt_type;
    b->bm_category = cat;
    b->math_ana = man;
    b->high_alg = hal;
    b->geo_top = gto;
    b->num_theory = nth;
    b->year = year;
    b->active = 1;
    maa.total_math_ana += man;
    maa.n_basic_m++;
    print_str("[MAA] Basic m "); print_int(maa.n_basic_m - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" man="); print_int(man);
    print_str(" hal="); print_int(hal);
    print_str(" gto="); print_int(gto);
    print_str(" nth="); print_int(nth); print_str("\n");
    return maa.n_basic_m - 1;
}

int maa_app_m(int at_type, int cat, int deq, int pst, int cma, int ors, int year) {
    if (maa.n_app_m >= MAX_APP_M) return -1;
    app_m_t* a = &app_ms[maa.n_app_m];
    a->am_id = maa.n_app_m;
    a->am_type = at_type;
    a->am_category = cat;
    a->dif_eq = deq;
    a->prob_st = pst;
    a->comp_math = cma;
    a->oper_res = ors;
    a->year = year;
    a->active = 1;
    maa.total_dif_eq += deq;
    maa.n_app_m++;
    print_str("[MAA] App m "); print_int(maa.n_app_m - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" deq="); print_int(deq);
    print_str(" pst="); print_int(pst);
    print_str(" cma="); print_int(cma);
    print_str(" ors="); print_int(ors); print_str("\n");
    return maa.n_app_m - 1;
}

int maa_comp_m(int ct_type, int cat, int nan, int mam, int scl, int ald, int year) {
    if (maa.n_comp_m >= MAX_COMP_M) return -1;
    comp_m_t* c = &comp_ms[maa.n_comp_m];
    c->cm_id = maa.n_comp_m;
    c->cm_type = ct_type;
    c->cm_category = cat;
    c->num_ana = nan;
    c->math_mod = mam;
    c->sci_comp = scl;
    c->alg_des = ald;
    c->year = year;
    c->active = 1;
    maa.total_num_ana += nan;
    maa.n_comp_m++;
    print_str("[MAA] Comp m "); print_int(maa.n_comp_m - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" nan="); print_int(nan);
    print_str(" mam="); print_int(mam);
    print_str(" scl="); print_int(scl);
    print_str(" ald="); print_int(ald); print_str("\n");
    return maa.n_comp_m - 1;
}

int maa_prob_s(int pt_type, int cat, int pth, int mst, int rpr, int bst, int year) {
    if (maa.n_prob_s >= MAX_PROB_S) return -1;
    prob_s_t* p = &prob_ss[maa.n_prob_s];
    p->ps_id = maa.n_prob_s;
    p->ps_type = pt_type;
    p->ps_category = cat;
    p->prob_th = pth;
    p->math_st = mst;
    p->ran_pro = rpr;
    p->bay_st = bst;
    p->year = year;
    p->active = 1;
    maa.total_prob_th += pth;
    maa.n_prob_s++;
    print_str("[MAA] Prob s "); print_int(maa.n_prob_s - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pth="); print_int(pth);
    print_str(" mst="); print_int(mst);
    print_str(" rpr="); print_int(rpr);
    print_str(" bst="); print_int(bst); print_str("\n");
    return maa.n_prob_s - 1;
}

int maa_oper_r(int ot_type, int cat, int lpr, int ipr, int dpr, int gth, int year) {
    if (maa.n_oper_r >= MAX_OPER_R) return -1;
    oper_r_t* o = &oper_rs[maa.n_oper_r];
    o->or_id = maa.n_oper_r;
    o->or_type = ot_type;
    o->or_category = cat;
    o->lin_pro = lpr;
    o->int_pro = ipr;
    o->dyn_pro = dpr;
    o->graph_th = gth;
    o->year = year;
    o->active = 1;
    maa.total_lin_pro += lpr;
    maa.n_oper_r++;
    print_str("[MAA] Oper r "); print_int(maa.n_oper_r - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" lpr="); print_int(lpr);
    print_str(" ipr="); print_int(ipr);
    print_str(" dpr="); print_int(dpr);
    print_str(" gth="); print_int(gth); print_str("\n");
    return maa.n_oper_r - 1;
}

void maa_basic_report(void) {
    print_str("[MAA] Basic math report:\n");
    print_str("  Basic categories: "); print_int(maa.n_basic_m); print_str("\n");
    print_str("  Total math analysis: "); print_int(maa.total_math_ana); print_str("\n");
}

void maa_app_report(void) {
    print_str("[MAA] Applied math report:\n");
    print_str("  Applied categories: "); print_int(maa.n_app_m); print_str("\n");
    print_str("  Total diff equations: "); print_int(maa.total_dif_eq); print_str("\n");
}

void maa_full_report(void) {
    print_str("[MAA] Full report:\n");
    print_str("  Computational categories: "); print_int(maa.n_comp_m); print_str("\n");
    print_str("  Total numerical analysis: "); print_int(maa.total_num_ana); print_str("\n");
    print_str("  Probability categories: "); print_int(maa.n_prob_s); print_str("\n");
    print_str("  Total probability theory: "); print_int(maa.total_prob_th); print_str("\n");
    print_str("  Operations categories: "); print_int(maa.n_oper_r); print_str("\n");
    print_str("  Total linear programming: "); print_int(maa.total_lin_pro); print_str("\n");
}

void maa_print_state(void) {
    print_str("[MAA] Bm="); print_int(maa.n_basic_m);
    print_str(" Am="); print_int(maa.n_app_m);
    print_str(" Cm="); print_int(maa.n_comp_m);
    print_str(" Ps="); print_int(maa.n_prob_s);
    print_str(" Or="); print_int(maa.n_oper_r);
    print_str("\n");
}

int main(void) {
    print_str("=== Mathematics Admin Demo ===\n\n");
    maa_init();

    print_str("Basic math...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int man = 55 + (i * 13);
        int hal = 40 + (i * 10);
        int gto = 22 + (i * 5);
        int nth = 15 + (i * 3);
        int year = 2020 + (i % 5);
        maa_basic_m(type, cat, man, hal, gto, nth, year);
    }

    print_str("\nApplied math...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int deq = 48 + (i * 11);
        int pst = 35 + (i * 8);
        int cma = 20 + (i * 4);
        int ors = 12 + (i * 3);
        int year = 2021 + (i % 4);
        maa_app_m(type, cat, deq, pst, cma, ors, year);
    }

    print_str("\nComputational math...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nan = 42 + (i * 10);
        int mam = 28 + (i * 7);
        int scl = 18 + (i * 4);
        int ald = 10 + (i * 2);
        int year = 2022 + (i % 3);
        maa_comp_m(type, cat, nan, mam, scl, ald, year);
    }

    print_str("\nProbability & statistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pth = 35 + (i * 8);
        int mst = 25 + (i * 6);
        int rpr = 15 + (i * 3);
        int bst = 10 + (i * 2);
        int year = 2023 + (i % 2);
        maa_prob_s(type, cat, pth, mst, rpr, bst, year);
    }

    print_str("\nOperations research...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lpr = 30 + (i * 7);
        int ipr = 22 + (i * 5);
        int dpr = 12 + (i * 3);
        int gth = 8 + (i * 2);
        int year = 2024;
        maa_oper_r(type, cat, lpr, ipr, dpr, gth, year);
    }

    print_str("\nBasic report...\n");
    maa_basic_report();

    print_str("\nApplied report...\n");
    maa_app_report();

    print_str("\nFull report...\n");
    maa_full_report();

    print_str("\nFinal state...\n");
    maa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
