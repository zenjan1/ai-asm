/* public_finance_admin: Public finance administration system (v1.0)
 * Public revenue, public expenditure, government budget, fiscal policy, fiscal management
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

#define MAX_PUB_REVN   16
#define MAX_PUB_EXPN   14
#define MAX_GOV_BUD    12
#define MAX_FIS_POL    10
#define MAX_FIS_MAN    10

typedef struct {
    int    pr_id;
    int    pr_type;
    int    pr_category;
    int    tax_rev;
    int    ntax_rev;
    int    deb_rev;
    int    fee_rev;
    int    year;
    int    active;
} pub_revn_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    pub_exp;
    int    soc_sec;
    int    inf_exp;
    int    def_exp;
    int    year;
    int    active;
} pub_expn_t;

typedef struct {
    int    gb_id;
    int    gb_type;
    int    gb_category;
    int    bud_com;
    int    bud_exe;
    int    bud_sup;
    int    bud_dis;
    int    year;
    int    active;
} gov_bud_t;

typedef struct {
    int    fp_id;
    int    fp_type;
    int    fp_category;
    int    fis_the;
    int    fis_too;
    int    pol_eff;
    int    pol_cor;
    int    year;
    int    active;
} fis_pol_t;

typedef struct {
    int    fm_id;
    int    fm_type;
    int    fm_category;
    int    tre_man;
    int    gov_pro;
    int    tra_pay;
    int    fis_sup;
    int    year;
    int    active;
} fis_man_t;

typedef struct {
    int    n_pub_revn;
    int    n_pub_expn;
    int    n_gov_bud;
    int    n_fis_pol;
    int    n_fis_man;
    int    total_tax_rev;
    int    total_pub_exp;
    int    total_bud_com;
    int    total_fis_the;
    int    total_tre_man;
} pfa_state_t;

static pub_revn_t pub_revns[MAX_PUB_REVN];
static pub_expn_t pub_expns[MAX_PUB_EXPN];
static gov_bud_t gov_buds[MAX_GOV_BUD];
static fis_pol_t fis_pols[MAX_FIS_POL];
static fis_man_t fis_mans[MAX_FIS_MAN];
static pfa_state_t pfa;

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

int pfa_init(void) {
    if (initialized) return -1;
    pfa.n_pub_revn = 0; pfa.n_pub_expn = 0; pfa.n_gov_bud = 0;
    pfa.n_fis_pol = 0; pfa.n_fis_man = 0;
    pfa.total_tax_rev = 0; pfa.total_pub_exp = 0;
    pfa.total_bud_com = 0; pfa.total_fis_the = 0;
    pfa.total_tre_man = 0;
    for (int i = 0; i < MAX_PUB_REVN; i++) pub_revns[i].active = 0;
    for (int i = 0; i < MAX_PUB_EXPN; i++) pub_expns[i].active = 0;
    for (int i = 0; i < MAX_GOV_BUD; i++) gov_buds[i].active = 0;
    for (int i = 0; i < MAX_FIS_POL; i++) fis_pols[i].active = 0;
    for (int i = 0; i < MAX_FIS_MAN; i++) fis_mans[i].active = 0;
    initialized = 1;
    print_str("[PFA] Public finance initialized\n");
    return 0;
}

int pfa_pub_revn(int rt_type, int cat, int trev, int ntrev, int drev, int frev, int year) {
    if (pfa.n_pub_revn >= MAX_PUB_REVN) return -1;
    pub_revn_t* r = &pub_revns[pfa.n_pub_revn];
    r->pr_id = pfa.n_pub_revn;
    r->pr_type = rt_type;
    r->pr_category = cat;
    r->tax_rev = trev;
    r->ntax_rev = ntrev;
    r->deb_rev = drev;
    r->fee_rev = frev;
    r->year = year;
    r->active = 1;
    pfa.total_tax_rev += trev;
    pfa.n_pub_revn++;
    print_str("[PFA] Pub revn "); print_int(pfa.n_pub_revn - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" trev="); print_int(trev);
    print_str(" ntrev="); print_int(ntrev);
    print_str(" drev="); print_int(drev);
    print_str(" frev="); print_int(frev); print_str("\n");
    return pfa.n_pub_revn - 1;
}

int pfa_pub_expn(int et_type, int cat, int pexp, int ssec, int iexp, int dexp, int year) {
    if (pfa.n_pub_expn >= MAX_PUB_EXPN) return -1;
    pub_expn_t* e = &pub_expns[pfa.n_pub_expn];
    e->pe_id = pfa.n_pub_expn;
    e->pe_type = et_type;
    e->pe_category = cat;
    e->pub_exp = pexp;
    e->soc_sec = ssec;
    e->inf_exp = iexp;
    e->def_exp = dexp;
    e->year = year;
    e->active = 1;
    pfa.total_pub_exp += pexp;
    pfa.n_pub_expn++;
    print_str("[PFA] Pub expn "); print_int(pfa.n_pub_expn - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" pexp="); print_int(pexp);
    print_str(" ssec="); print_int(ssec);
    print_str(" iexp="); print_int(iexp);
    print_str(" dexp="); print_int(dexp); print_str("\n");
    return pfa.n_pub_expn - 1;
}

int pfa_gov_bud(int bt_type, int cat, int bcom, int bexe, int bsup, int bdis, int year) {
    if (pfa.n_gov_bud >= MAX_GOV_BUD) return -1;
    gov_bud_t* g = &gov_buds[pfa.n_gov_bud];
    g->gb_id = pfa.n_gov_bud;
    g->gb_type = bt_type;
    g->gb_category = cat;
    g->bud_com = bcom;
    g->bud_exe = bexe;
    g->bud_sup = bsup;
    g->bud_dis = bdis;
    g->year = year;
    g->active = 1;
    pfa.total_bud_com += bcom;
    pfa.n_gov_bud++;
    print_str("[PFA] Gov bud "); print_int(pfa.n_gov_bud - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bcom="); print_int(bcom);
    print_str(" bexe="); print_int(bexe);
    print_str(" bsup="); print_int(bsup);
    print_str(" bdis="); print_int(bdis); print_str("\n");
    return pfa.n_gov_bud - 1;
}

int pfa_fis_pol(int pt_type, int cat, int fthe, int ftoo, int peff, int pcor, int year) {
    if (pfa.n_fis_pol >= MAX_FIS_POL) return -1;
    fis_pol_t* f = &fis_pols[pfa.n_fis_pol];
    f->fp_id = pfa.n_fis_pol;
    f->fp_type = pt_type;
    f->fp_category = cat;
    f->fis_the = fthe;
    f->fis_too = ftoo;
    f->pol_eff = peff;
    f->pol_cor = pcor;
    f->year = year;
    f->active = 1;
    pfa.total_fis_the += fthe;
    pfa.n_fis_pol++;
    print_str("[PFA] Fis pol "); print_int(pfa.n_fis_pol - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" fthe="); print_int(fthe);
    print_str(" ftoo="); print_int(ftoo);
    print_str(" peff="); print_int(peff);
    print_str(" pcor="); print_int(pcor); print_str("\n");
    return pfa.n_fis_pol - 1;
}

int pfa_fis_man(int mt_type, int cat, int tman, int gpro, int tpay, int fsup, int year) {
    if (pfa.n_fis_man >= MAX_FIS_MAN) return -1;
    fis_man_t* m = &fis_mans[pfa.n_fis_man];
    m->fm_id = pfa.n_fis_man;
    m->fm_type = mt_type;
    m->fm_category = cat;
    m->tre_man = tman;
    m->gov_pro = gpro;
    m->tra_pay = tpay;
    m->fis_sup = fsup;
    m->year = year;
    m->active = 1;
    pfa.total_tre_man += tman;
    pfa.n_fis_man++;
    print_str("[PFA] Fis man "); print_int(pfa.n_fis_man - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tman="); print_int(tman);
    print_str(" gpro="); print_int(gpro);
    print_str(" tpay="); print_int(tpay);
    print_str(" fsup="); print_int(fsup); print_str("\n");
    return pfa.n_fis_man - 1;
}

void pfa_revenue_report(void) {
    print_str("[PFA] Public revenue report:\n");
    print_str("  Revenue categories: "); print_int(pfa.n_pub_revn); print_str("\n");
    print_str("  Total tax revenue: "); print_int(pfa.total_tax_rev); print_str("\n");
}

void pfa_expenditure_report(void) {
    print_str("[PFA] Public expenditure report:\n");
    print_str("  Expenditure categories: "); print_int(pfa.n_pub_expn); print_str("\n");
    print_str("  Total public expenditure: "); print_int(pfa.total_pub_exp); print_str("\n");
}

void pfa_full_report(void) {
    print_str("[PFA] Full report:\n");
    print_str("  Budget categories: "); print_int(pfa.n_gov_bud); print_str("\n");
    print_str("  Total budget compilation: "); print_int(pfa.total_bud_com); print_str("\n");
    print_str("  Policy categories: "); print_int(pfa.n_fis_pol); print_str("\n");
    print_str("  Total fiscal theory: "); print_int(pfa.total_fis_the); print_str("\n");
    print_str("  Management categories: "); print_int(pfa.n_fis_man); print_str("\n");
    print_str("  Total treasury management: "); print_int(pfa.total_tre_man); print_str("\n");
}

void pfa_print_state(void) {
    print_str("[PFA] Pr="); print_int(pfa.n_pub_revn);
    print_str(" Pe="); print_int(pfa.n_pub_expn);
    print_str(" Gb="); print_int(pfa.n_gov_bud);
    print_str(" Fp="); print_int(pfa.n_fis_pol);
    print_str(" Fm="); print_int(pfa.n_fis_man);
    print_str("\n");
}

int main(void) {
    print_str("=== Public Finance Admin Demo ===\n\n");
    pfa_init();

    print_str("Public revenue...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int trev = 55 + (i * 13);
        int ntrev = 40 + (i * 10);
        int drev = 22 + (i * 5);
        int frev = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pfa_pub_revn(type, cat, trev, ntrev, drev, frev, year);
    }

    print_str("\nPublic expenditure...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pexp = 48 + (i * 11);
        int ssec = 35 + (i * 8);
        int iexp = 20 + (i * 4);
        int dexp = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pfa_pub_expn(type, cat, pexp, ssec, iexp, dexp, year);
    }

    print_str("\nGovernment budget...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bcom = 42 + (i * 10);
        int bexe = 28 + (i * 7);
        int bsup = 18 + (i * 4);
        int bdis = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pfa_gov_bud(type, cat, bcom, bexe, bsup, bdis, year);
    }

    print_str("\nFiscal policy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fthe = 35 + (i * 8);
        int ftoo = 25 + (i * 6);
        int peff = 15 + (i * 3);
        int pcor = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pfa_fis_pol(type, cat, fthe, ftoo, peff, pcor, year);
    }

    print_str("\nFiscal management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tman = 30 + (i * 7);
        int gpro = 22 + (i * 5);
        int tpay = 12 + (i * 3);
        int fsup = 8 + (i * 2);
        int year = 2024;
        pfa_fis_man(type, cat, tman, gpro, tpay, fsup, year);
    }

    print_str("\nRevenue report...\n");
    pfa_revenue_report();

    print_str("\nExpenditure report...\n");
    pfa_expenditure_report();

    print_str("\nFull report...\n");
    pfa_full_report();

    print_str("\nFinal state...\n");
    pfa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
