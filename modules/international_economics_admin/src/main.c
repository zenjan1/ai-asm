/* international_economics_admin: International economics administration system (v1.0)
 * International trade, international finance, multinational economics, open economy, development economics
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

#define MAX_INT_TRD   16
#define MAX_INT_FIN   14
#define MAX_MLT_ECON  12
#define MAX_OPN_ECON  10
#define MAX_DEV_ECON  10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    trd_the;
    int    trd_pol;
    int    trd_bar;
    int    trd_zon;
    int    year;
    int    active;
} int_trd_t;

typedef struct {
    int    if_id;
    int    if_type;
    int    if_category;
    int    mon_sys;
    int    exc_the;
    int    fex_mkt;
    int    cap_flo;
    int    year;
    int    active;
} int_fin_t;

typedef struct {
    int    me_id;
    int    me_type;
    int    me_category;
    int    mnc_eco;
    int    int_inv;
    int    int_cap;
    int    gbl_val;
    int    year;
    int    active;
} mlt_econ_t;

typedef struct {
    int    oe_id;
    int    oe_type;
    int    oe_category;
    int    op_mac;
    int    e_glo;
    int    int_cor;
    int    e_int;
    int    year;
    int    active;
} opn_econ_t;

typedef struct {
    int    de_id;
    int    de_type;
    int    de_category;
    int    dev_aid;
    int    nor_sou;
    int    emr_eco;
    int    int_deb;
    int    year;
    int    active;
} dev_econ_t;

typedef struct {
    int    n_int_trd;
    int    n_int_fin;
    int    n_mlt_econ;
    int    n_opn_econ;
    int    n_dev_econ;
    int    total_trd_the;
    int    total_mon_sys;
    int    total_mnc_eco;
    int    total_op_mac;
    int    total_dev_aid;
} in_state_t;

static int_trd_t int_trds[MAX_INT_TRD];
static int_fin_t int_fins[MAX_INT_FIN];
static mlt_econ_t mlt_econs[MAX_MLT_ECON];
static opn_econ_t opn_econs[MAX_OPN_ECON];
static dev_econ_t dev_econs[MAX_DEV_ECON];
static in_state_t in_st;

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

int in_init(void) {
    if (initialized) return -1;
    in_st.n_int_trd = 0; in_st.n_int_fin = 0; in_st.n_mlt_econ = 0;
    in_st.n_opn_econ = 0; in_st.n_dev_econ = 0;
    in_st.total_trd_the = 0; in_st.total_mon_sys = 0;
    in_st.total_mnc_eco = 0; in_st.total_op_mac = 0;
    in_st.total_dev_aid = 0;
    for (int i = 0; i < MAX_INT_TRD; i++) int_trds[i].active = 0;
    for (int i = 0; i < MAX_INT_FIN; i++) int_fins[i].active = 0;
    for (int i = 0; i < MAX_MLT_ECON; i++) mlt_econs[i].active = 0;
    for (int i = 0; i < MAX_OPN_ECON; i++) opn_econs[i].active = 0;
    for (int i = 0; i < MAX_DEV_ECON; i++) dev_econs[i].active = 0;
    initialized = 1;
    print_str("[IN] International economics initialized\n");
    return 0;
}

int in_int_trd(int it_type, int cat, int tthe, int tpol, int tvar, int tzon, int year) {
    if (in_st.n_int_trd >= MAX_INT_TRD) return -1;
    int_trd_t* t = &int_trds[in_st.n_int_trd];
    t->it_id = in_st.n_int_trd; t->it_type = it_type; t->it_category = cat;
    t->trd_the = tthe; t->trd_pol = tpol; t->trd_bar = tvar; t->trd_zon = tzon;
    t->year = year; t->active = 1;
    in_st.total_trd_the += tthe; in_st.n_int_trd++;
    print_str("[IN] Int trd "); print_int(in_st.n_int_trd - 1);
    print_str(" type="); print_int(it_type); print_str(" cat="); print_int(cat);
    print_str(" tthe="); print_int(tthe); print_str(" tpol="); print_int(tpol);
    print_str(" tvar="); print_int(tvar); print_str(" tzon="); print_int(tzon); print_str("\n");
    return in_st.n_int_trd - 1;
}

int in_int_fin(int if_type, int cat, int msys, int ethe, int fmkt, int cflo, int year) {
    if (in_st.n_int_fin >= MAX_INT_FIN) return -1;
    int_fin_t* f = &int_fins[in_st.n_int_fin];
    f->if_id = in_st.n_int_fin; f->if_type = if_type; f->if_category = cat;
    f->mon_sys = msys; f->exc_the = ethe; f->fex_mkt = fmkt; f->cap_flo = cflo;
    f->year = year; f->active = 1;
    in_st.total_mon_sys += msys; in_st.n_int_fin++;
    print_str("[IN] Int fin "); print_int(in_st.n_int_fin - 1);
    print_str(" type="); print_int(if_type); print_str(" cat="); print_int(cat);
    print_str(" msys="); print_int(msys); print_str(" ethe="); print_int(ethe);
    print_str(" fmkt="); print_int(fmkt); print_str(" cflo="); print_int(cflo); print_str("\n");
    return in_st.n_int_fin - 1;
}

int in_mlt_econ(int me_type, int cat, int mnc, int iinv, int icap, int gval, int year) {
    if (in_st.n_mlt_econ >= MAX_MLT_ECON) return -1;
    mlt_econ_t* m = &mlt_econs[in_st.n_mlt_econ];
    m->me_id = in_st.n_mlt_econ; m->me_type = me_type; m->me_category = cat;
    m->mnc_eco = mnc; m->int_inv = iinv; m->int_cap = icap; m->gbl_val = gval;
    m->year = year; m->active = 1;
    in_st.total_mnc_eco += mnc; in_st.n_mlt_econ++;
    print_str("[IN] Mlt econ "); print_int(in_st.n_mlt_econ - 1);
    print_str(" type="); print_int(me_type); print_str(" cat="); print_int(cat);
    print_str(" mnc="); print_int(mnc); print_str(" iinv="); print_int(iinv);
    print_str(" icap="); print_int(icap); print_str(" gval="); print_int(gval); print_str("\n");
    return in_st.n_mlt_econ - 1;
}

int in_opn_econ(int oe_type, int cat, int omac, int eglo, int icor, int eint, int year) {
    if (in_st.n_opn_econ >= MAX_OPN_ECON) return -1;
    opn_econ_t* o = &opn_econs[in_st.n_opn_econ];
    o->oe_id = in_st.n_opn_econ; o->oe_type = oe_type; o->oe_category = cat;
    o->op_mac = omac; o->e_glo = eglo; o->int_cor = icor; o->e_int = eint;
    o->year = year; o->active = 1;
    in_st.total_op_mac += omac; in_st.n_opn_econ++;
    print_str("[IN] Opn econ "); print_int(in_st.n_opn_econ - 1);
    print_str(" type="); print_int(oe_type); print_str(" cat="); print_int(cat);
    print_str(" omac="); print_int(omac); print_str(" eglo="); print_int(eglo);
    print_str(" icor="); print_int(icor); print_str(" eint="); print_int(eint); print_str("\n");
    return in_st.n_opn_econ - 1;
}

int in_dev_econ(int de_type, int cat, int daid, int nsou, int eco2, int ideb, int year) {
    if (in_st.n_dev_econ >= MAX_DEV_ECON) return -1;
    dev_econ_t* d = &dev_econs[in_st.n_dev_econ];
    d->de_id = in_st.n_dev_econ; d->de_type = de_type; d->de_category = cat;
    d->dev_aid = daid; d->nor_sou = nsou; d->emr_eco = eco2; d->int_deb = ideb;
    d->year = year; d->active = 1;
    in_st.total_dev_aid += daid; in_st.n_dev_econ++;
    print_str("[IN] Dev econ "); print_int(in_st.n_dev_econ - 1);
    print_str(" type="); print_int(de_type); print_str(" cat="); print_int(cat);
    print_str(" daid="); print_int(daid); print_str(" nsou="); print_int(nsou);
    print_str(" eco2="); print_int(eco2); print_str(" ideb="); print_int(ideb); print_str("\n");
    return in_st.n_dev_econ - 1;
}

void in_trade_report(void) {
    print_str("[IN] International trade report:\n");
    print_str("  Trade categories: "); print_int(in_st.n_int_trd); print_str("\n");
    print_str("  Total trade theory: "); print_int(in_st.total_trd_the); print_str("\n");
}

void in_finance_report(void) {
    print_str("[IN] International finance report:\n");
    print_str("  Finance categories: "); print_int(in_st.n_int_fin); print_str("\n");
    print_str("  Total monetary system: "); print_int(in_st.total_mon_sys); print_str("\n");
}

void in_full_report(void) {
    print_str("[IN] Full report:\n");
    print_str("  Multinational categories: "); print_int(in_st.n_mlt_econ); print_str("\n");
    print_str("  Total MNC economics: "); print_int(in_st.total_mnc_eco); print_str("\n");
    print_str("  Open economy categories: "); print_int(in_st.n_opn_econ); print_str("\n");
    print_str("  Total open macro: "); print_int(in_st.total_op_mac); print_str("\n");
    print_str("  Development categories: "); print_int(in_st.n_dev_econ); print_str("\n");
    print_str("  Total development aid: "); print_int(in_st.total_dev_aid); print_str("\n");
}

void in_print_state(void) {
    print_str("[IN] It="); print_int(in_st.n_int_trd);
    print_str(" If="); print_int(in_st.n_int_fin);
    print_str(" Me="); print_int(in_st.n_mlt_econ);
    print_str(" Oe="); print_int(in_st.n_opn_econ);
    print_str(" De="); print_int(in_st.n_dev_econ);
    print_str("\n");
}

int main(void) {
    print_str("=== International Economics Admin Demo ===\n\n");
    in_init();

    print_str("International trade...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1; int cat = (i % 4) + 1;
        int tthe = 68 + (i * 16); int tpol = 52 + (i * 12);
        int tvar = 29 + (i * 7); int tzon = 21 + (i * 5);
        int year = 2020 + (i % 5);
        in_int_trd(type, cat, tthe, tpol, tvar, tzon, year);
    }

    print_str("\nInternational finance...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int msys = 58 + (i * 14); int ethe = 44 + (i * 11);
        int fmkt = 26 + (i * 6); int cflo = 17 + (i * 4);
        int year = 2021 + (i % 4);
        in_int_fin(type, cat, msys, ethe, fmkt, cflo, year);
    }

    print_str("\nMultinational economics...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int mnc = 50 + (i * 12); int iinv = 36 + (i * 9);
        int icap = 23 + (i * 5); int gval = 15 + (i * 3);
        int year = 2022 + (i % 3);
        in_mlt_econ(type, cat, mnc, iinv, icap, gval, year);
    }

    print_str("\nOpen economy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int omac = 44 + (i * 10); int eglo = 32 + (i * 7);
        int icor = 19 + (i * 4); int eint = 13 + (i * 3);
        int year = 2023 + (i % 2);
        in_opn_econ(type, cat, omac, eglo, icor, eint, year);
    }

    print_str("\nDevelopment economics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1; int cat = (i % 5) + 1;
        int daid = 40 + (i * 9); int nsou = 30 + (i * 7);
        int eco2 = 17 + (i * 4); int ideb = 11 + (i * 2);
        int year = 2024;
        in_dev_econ(type, cat, daid, nsou, eco2, ideb, year);
    }

    print_str("\nTrade report...\n"); in_trade_report();
    print_str("\nFinance report...\n"); in_finance_report();
    print_str("\nFull report...\n"); in_full_report();
    print_str("\nFinal state...\n"); in_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
