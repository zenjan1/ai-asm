/* marketing_admin: Marketing administration system (v1.0)
 * Marketing theory, brand management, digital marketing, market research, sales management
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

#define MAX_MKT_THE    16
#define MAX_BRD_MAN    14
#define MAX_DIG_MKT    12
#define MAX_MKT_RES    10
#define MAX_SAL_MAN    10

typedef struct {
    int    mt_id;
    int    mt_type;
    int    mt_category;
    int    mkt_pri;
    int    con_beh;
    int    mkt_seg;
    int    tar_mkt;
    int    year;
    int    active;
} mkt_the_t;

typedef struct {
    int    bm_id;
    int    bm_type;
    int    bm_category;
    int    bra_str;
    int    bra_com;
    int    bra_val;
    int    bra_main;
    int    year;
    int    active;
} brd_man_t;

typedef struct {
    int    dm_id;
    int    dm_type;
    int    dm_category;
    int    soc_med;
    int    sea_mar;
    int    con_mar;
    int    dat_ana;
    int    year;
    int    active;
} dig_mkt_t;

typedef struct {
    int    mr_id;
    int    mr_type;
    int    mr_category;
    int    res_met;
    int    dat_col;
    int    mkt_ana;
    int    pre_mod;
    int    year;
    int    active;
} mkt_res_t;

typedef struct {
    int    sm_id;
    int    sm_type;
    int    sm_category;
    int    sal_str;
    int    cha_man;
    int    cus_rel;
    int    sal_team;
    int    year;
    int    active;
} sal_man_t;

typedef struct {
    int    n_mkt_the;
    int    n_brd_man;
    int    n_dig_mkt;
    int    n_mkt_res;
    int    n_sal_man;
    int    total_mkt_pri;
    int    total_bra_str;
    int    total_soc_med;
    int    total_res_met;
    int    total_sal_str;
} mka_state_t;

static mkt_the_t mkt_thes[MAX_MKT_THE];
static brd_man_t brd_mans[MAX_BRD_MAN];
static dig_mkt_t dig_mkts[MAX_DIG_MKT];
static mkt_res_t mkt_ress[MAX_MKT_RES];
static sal_man_t sal_mans[MAX_SAL_MAN];
static mka_state_t mka;

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

int mka_init(void) {
    if (initialized) return -1;
    mka.n_mkt_the = 0; mka.n_brd_man = 0; mka.n_dig_mkt = 0;
    mka.n_mkt_res = 0; mka.n_sal_man = 0;
    mka.total_mkt_pri = 0; mka.total_bra_str = 0;
    mka.total_soc_med = 0; mka.total_res_met = 0;
    mka.total_sal_str = 0;
    for (int i = 0; i < MAX_MKT_THE; i++) mkt_thes[i].active = 0;
    for (int i = 0; i < MAX_BRD_MAN; i++) brd_mans[i].active = 0;
    for (int i = 0; i < MAX_DIG_MKT; i++) dig_mkts[i].active = 0;
    for (int i = 0; i < MAX_MKT_RES; i++) mkt_ress[i].active = 0;
    for (int i = 0; i < MAX_SAL_MAN; i++) sal_mans[i].active = 0;
    initialized = 1;
    print_str("[MKA] Marketing initialized\n");
    return 0;
}

int mka_mkt_the(int mt_type, int cat, int mpri, int cbeh, int mseg, int tmkt, int year) {
    if (mka.n_mkt_the >= MAX_MKT_THE) return -1;
    mkt_the_t* m = &mkt_thes[mka.n_mkt_the];
    m->mt_id = mka.n_mkt_the;
    m->mt_type = mt_type;
    m->mt_category = cat;
    m->mkt_pri = mpri;
    m->con_beh = cbeh;
    m->mkt_seg = mseg;
    m->tar_mkt = tmkt;
    m->year = year;
    m->active = 1;
    mka.total_mkt_pri += mpri;
    mka.n_mkt_the++;
    print_str("[MKA] Mkt the "); print_int(mka.n_mkt_the - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpri="); print_int(mpri);
    print_str(" cbeh="); print_int(cbeh);
    print_str(" mseg="); print_int(mseg);
    print_str(" tmkt="); print_int(tmkt); print_str("\n");
    return mka.n_mkt_the - 1;
}

int mka_brd_man(int bt_type, int cat, int bstr, int bcom, int bval, int bmain, int year) {
    if (mka.n_brd_man >= MAX_BRD_MAN) return -1;
    brd_man_t* b = &brd_mans[mka.n_brd_man];
    b->bm_id = mka.n_brd_man;
    b->bm_type = bt_type;
    b->bm_category = cat;
    b->bra_str = bstr;
    b->bra_com = bcom;
    b->bra_val = bval;
    b->bra_main = bmain;
    b->year = year;
    b->active = 1;
    mka.total_bra_str += bstr;
    mka.n_brd_man++;
    print_str("[MKA] Brd man "); print_int(mka.n_brd_man - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bstr="); print_int(bstr);
    print_str(" bcom="); print_int(bcom);
    print_str(" bval="); print_int(bval);
    print_str(" bmain="); print_int(bmain); print_str("\n");
    return mka.n_brd_man - 1;
}

int mka_dig_mkt(int dt_type, int cat, int smed, int smar, int cmar, int dana, int year) {
    if (mka.n_dig_mkt >= MAX_DIG_MKT) return -1;
    dig_mkt_t* d = &dig_mkts[mka.n_dig_mkt];
    d->dm_id = mka.n_dig_mkt;
    d->dm_type = dt_type;
    d->dm_category = cat;
    d->soc_med = smed;
    d->sea_mar = smar;
    d->con_mar = cmar;
    d->dat_ana = dana;
    d->year = year;
    d->active = 1;
    mka.total_soc_med += smed;
    mka.n_dig_mkt++;
    print_str("[MKA] Dig mkt "); print_int(mka.n_dig_mkt - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" smed="); print_int(smed);
    print_str(" smar="); print_int(smar);
    print_str(" cmar="); print_int(cmar);
    print_str(" dana="); print_int(dana); print_str("\n");
    return mka.n_dig_mkt - 1;
}

int mka_mkt_res(int rt_type, int cat, int rmet, int dcol, int mana, int pmod, int year) {
    if (mka.n_mkt_res >= MAX_MKT_RES) return -1;
    mkt_res_t* r = &mkt_ress[mka.n_mkt_res];
    r->mr_id = mka.n_mkt_res;
    r->mr_type = rt_type;
    r->mr_category = cat;
    r->res_met = rmet;
    r->dat_col = dcol;
    r->mkt_ana = mana;
    r->pre_mod = pmod;
    r->year = year;
    r->active = 1;
    mka.total_res_met += rmet;
    mka.n_mkt_res++;
    print_str("[MKA] Mkt res "); print_int(mka.n_mkt_res - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rmet="); print_int(rmet);
    print_str(" dcol="); print_int(dcol);
    print_str(" mana="); print_int(mana);
    print_str(" pmod="); print_int(pmod); print_str("\n");
    return mka.n_mkt_res - 1;
}

int mka_sal_man(int st_type, int cat, int sstr, int cman, int crel, int steam, int year) {
    if (mka.n_sal_man >= MAX_SAL_MAN) return -1;
    sal_man_t* s = &sal_mans[mka.n_sal_man];
    s->sm_id = mka.n_sal_man;
    s->sm_type = st_type;
    s->sm_category = cat;
    s->sal_str = sstr;
    s->cha_man = cman;
    s->cus_rel = crel;
    s->sal_team = steam;
    s->year = year;
    s->active = 1;
    mka.total_sal_str += sstr;
    mka.n_sal_man++;
    print_str("[MKA] Sal man "); print_int(mka.n_sal_man - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sstr="); print_int(sstr);
    print_str(" cman="); print_int(cman);
    print_str(" crel="); print_int(crel);
    print_str(" steam="); print_int(steam); print_str("\n");
    return mka.n_sal_man - 1;
}

void mka_theory_report(void) {
    print_str("[MKA] Marketing theory report:\n");
    print_str("  Theory categories: "); print_int(mka.n_mkt_the); print_str("\n");
    print_str("  Total marketing principles: "); print_int(mka.total_mkt_pri); print_str("\n");
}

void mka_brand_report(void) {
    print_str("[MKA] Brand management report:\n");
    print_str("  Brand categories: "); print_int(mka.n_brd_man); print_str("\n");
    print_str("  Total brand strength: "); print_int(mka.total_bra_str); print_str("\n");
}

void mka_full_report(void) {
    print_str("[MKA] Full report:\n");
    print_str("  Digital categories: "); print_int(mka.n_dig_mkt); print_str("\n");
    print_str("  Total social media: "); print_int(mka.total_soc_med); print_str("\n");
    print_str("  Research categories: "); print_int(mka.n_mkt_res); print_str("\n");
    print_str("  Total research methods: "); print_int(mka.total_res_met); print_str("\n");
    print_str("  Sales categories: "); print_int(mka.n_sal_man); print_str("\n");
    print_str("  Total sales strategy: "); print_int(mka.total_sal_str); print_str("\n");
}

void mka_print_state(void) {
    print_str("[MKA] Mt="); print_int(mka.n_mkt_the);
    print_str(" Bm="); print_int(mka.n_brd_man);
    print_str(" Dm="); print_int(mka.n_dig_mkt);
    print_str(" Mr="); print_int(mka.n_mkt_res);
    print_str(" Sm="); print_int(mka.n_sal_man);
    print_str("\n");
}

int main(void) {
    print_str("=== Marketing Admin Demo ===\n\n");
    mka_init();

    print_str("Marketing theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mpri = 55 + (i * 13);
        int cbeh = 40 + (i * 10);
        int mseg = 22 + (i * 5);
        int tmkt = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mka_mkt_the(type, cat, mpri, cbeh, mseg, tmkt, year);
    }

    print_str("\nBrand management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bstr = 48 + (i * 11);
        int bcom = 35 + (i * 8);
        int bval = 20 + (i * 4);
        int bmain = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mka_brd_man(type, cat, bstr, bcom, bval, bmain, year);
    }

    print_str("\nDigital marketing...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int smed = 42 + (i * 10);
        int smar = 28 + (i * 7);
        int cmar = 18 + (i * 4);
        int dana = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mka_dig_mkt(type, cat, smed, smar, cmar, dana, year);
    }

    print_str("\nMarket research...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rmet = 35 + (i * 8);
        int dcol = 25 + (i * 6);
        int mana = 15 + (i * 3);
        int pmod = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mka_mkt_res(type, cat, rmet, dcol, mana, pmod, year);
    }

    print_str("\nSales management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sstr = 30 + (i * 7);
        int cman = 22 + (i * 5);
        int crel = 12 + (i * 3);
        int steam = 8 + (i * 2);
        int year = 2024;
        mka_sal_man(type, cat, sstr, cman, crel, steam, year);
    }

    print_str("\nTheory report...\n");
    mka_theory_report();

    print_str("\nBrand report...\n");
    mka_brand_report();

    print_str("\nFull report...\n");
    mka_full_report();

    print_str("\nFinal state...\n");
    mka_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
