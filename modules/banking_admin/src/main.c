/* banking_admin: Banking administration system (v1.0)
 * Commercial banking, central banking, investment banking, fintech, international finance
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

#define MAX_COM_BAN    16
#define MAX_CEN_BAN    14
#define MAX_INV_BAN    12
#define MAX_FIN_TECH   10
#define MAX_INT_FIN    10

typedef struct {
    int    cb_id;
    int    cb_type;
    int    cb_category;
    int    com_bus;
    int    com_man;
    int    com_opr;
    int    risk_man;
    int    year;
    int    active;
} com_ban_t;

typedef struct {
    int    cnb_id;
    int    cnb_type;
    int    cnb_category;
    int    mon_pol;
    int    fin_reg;
    int    pay_cle;
    int    for_exc;
    int    year;
    int    active;
} cen_ban_t;

typedef struct {
    int    ib_id;
    int    ib_type;
    int    ib_category;
    int    sec_iss;
    int    ma_rec;
    int    ast_man;
    int    pri_equ;
    int    year;
    int    active;
} inv_ban_t;

typedef struct {
    int    ft_id;
    int    ft_type;
    int    ft_category;
    int    dig_cur;
    int    blo_fin;
    int    int_adv;
    int    big_risk;
    int    year;
    int    active;
} fin_tech_t;

typedef struct {
    int    inf_id;
    int    inf_type;
    int    inf_category;
    int    int_set;
    int    int_fun;
    int    int_inv;
    int    exc_rate;
    int    year;
    int    active;
} int_fin_t;

typedef struct {
    int    n_com_ban;
    int    n_cen_ban;
    int    n_inv_ban;
    int    n_fin_tech;
    int    n_int_fin;
    int    total_com_bus;
    int    total_mon_pol;
    int    total_sec_iss;
    int    total_dig_cur;
    int    total_int_set;
} bka_state_t;

static com_ban_t com_bans[MAX_COM_BAN];
static cen_ban_t cen_bans[MAX_CEN_BAN];
static inv_ban_t inv_bans[MAX_INV_BAN];
static fin_tech_t fin_techs[MAX_FIN_TECH];
static int_fin_t int_fins[MAX_INT_FIN];
static bka_state_t bka;

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

int bka_init(void) {
    if (initialized) return -1;
    bka.n_com_ban = 0; bka.n_cen_ban = 0; bka.n_inv_ban = 0;
    bka.n_fin_tech = 0; bka.n_int_fin = 0;
    bka.total_com_bus = 0; bka.total_mon_pol = 0;
    bka.total_sec_iss = 0; bka.total_dig_cur = 0;
    bka.total_int_set = 0;
    for (int i = 0; i < MAX_COM_BAN; i++) com_bans[i].active = 0;
    for (int i = 0; i < MAX_CEN_BAN; i++) cen_bans[i].active = 0;
    for (int i = 0; i < MAX_INV_BAN; i++) inv_bans[i].active = 0;
    for (int i = 0; i < MAX_FIN_TECH; i++) fin_techs[i].active = 0;
    for (int i = 0; i < MAX_INT_FIN; i++) int_fins[i].active = 0;
    initialized = 1;
    print_str("[BKA] Banking initialized\n");
    return 0;
}

int bka_com_ban(int ct_type, int cat, int cbs, int cmn, int cop, int rsk, int year) {
    if (bka.n_com_ban >= MAX_COM_BAN) return -1;
    com_ban_t* c = &com_bans[bka.n_com_ban];
    c->cb_id = bka.n_com_ban;
    c->cb_type = ct_type;
    c->cb_category = cat;
    c->com_bus = cbs;
    c->com_man = cmn;
    c->com_opr = cop;
    c->risk_man = rsk;
    c->year = year;
    c->active = 1;
    bka.total_com_bus += cbs;
    bka.n_com_ban++;
    print_str("[BKA] Com ban "); print_int(bka.n_com_ban - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cbs="); print_int(cbs);
    print_str(" cmn="); print_int(cmn);
    print_str(" cop="); print_int(cop);
    print_str(" rsk="); print_int(rsk); print_str("\n");
    return bka.n_com_ban - 1;
}

int bka_cen_ban(int ct_type, int cat, int mpl, int frg, int pcl, int fex, int year) {
    if (bka.n_cen_ban >= MAX_CEN_BAN) return -1;
    cen_ban_t* c = &cen_bans[bka.n_cen_ban];
    c->cnb_id = bka.n_cen_ban;
    c->cnb_type = ct_type;
    c->cnb_category = cat;
    c->mon_pol = mpl;
    c->fin_reg = frg;
    c->pay_cle = pcl;
    c->for_exc = fex;
    c->year = year;
    c->active = 1;
    bka.total_mon_pol += mpl;
    bka.n_cen_ban++;
    print_str("[BKA] Cen ban "); print_int(bka.n_cen_ban - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" mpl="); print_int(mpl);
    print_str(" frg="); print_int(frg);
    print_str(" pcl="); print_int(pcl);
    print_str(" fex="); print_int(fex); print_str("\n");
    return bka.n_cen_ban - 1;
}

int bka_inv_ban(int it_type, int cat, int sis, int mar, int asmn, int pek, int year) {
    if (bka.n_inv_ban >= MAX_INV_BAN) return -1;
    inv_ban_t* b = &inv_bans[bka.n_inv_ban];
    b->ib_id = bka.n_inv_ban;
    b->ib_type = it_type;
    b->ib_category = cat;
    b->sec_iss = sis;
    b->ma_rec = mar;
    b->ast_man = asmn;
    b->pri_equ = pek;
    b->year = year;
    b->active = 1;
    bka.total_sec_iss += sis;
    bka.n_inv_ban++;
    print_str("[BKA] Inv ban "); print_int(bka.n_inv_ban - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" sis="); print_int(sis);
    print_str(" mar="); print_int(mar);
    print_str(" asm="); print_int(asmn);
    print_str(" pek="); print_int(pek); print_str("\n");
    return bka.n_inv_ban - 1;
}

int bka_fin_tech(int ft_type, int cat, int dcr, int blf, int iad, int brk, int year) {
    if (bka.n_fin_tech >= MAX_FIN_TECH) return -1;
    fin_tech_t* f = &fin_techs[bka.n_fin_tech];
    f->ft_id = bka.n_fin_tech;
    f->ft_type = ft_type;
    f->ft_category = cat;
    f->dig_cur = dcr;
    f->blo_fin = blf;
    f->int_adv = iad;
    f->big_risk = brk;
    f->year = year;
    f->active = 1;
    bka.total_dig_cur += dcr;
    bka.n_fin_tech++;
    print_str("[BKA] Fin tech "); print_int(bka.n_fin_tech - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" dcr="); print_int(dcr);
    print_str(" blf="); print_int(blf);
    print_str(" iad="); print_int(iad);
    print_str(" brk="); print_int(brk); print_str("\n");
    return bka.n_fin_tech - 1;
}

int bka_int_fin(int it_type, int cat, int iset, int ifun, int iinv, int exr, int year) {
    if (bka.n_int_fin >= MAX_INT_FIN) return -1;
    int_fin_t* f = &int_fins[bka.n_int_fin];
    f->inf_id = bka.n_int_fin;
    f->inf_type = it_type;
    f->inf_category = cat;
    f->int_set = iset;
    f->int_fun = ifun;
    f->int_inv = iinv;
    f->exc_rate = exr;
    f->year = year;
    f->active = 1;
    bka.total_int_set += iset;
    bka.n_int_fin++;
    print_str("[BKA] Int fin "); print_int(bka.n_int_fin - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" iset="); print_int(iset);
    print_str(" ifun="); print_int(ifun);
    print_str(" iinv="); print_int(iinv);
    print_str(" exr="); print_int(exr); print_str("\n");
    return bka.n_int_fin - 1;
}

void bka_commercial_report(void) {
    print_str("[BKA] Commercial banking report:\n");
    print_str("  Commercial categories: "); print_int(bka.n_com_ban); print_str("\n");
    print_str("  Total commercial business: "); print_int(bka.total_com_bus); print_str("\n");
}

void bka_central_report(void) {
    print_str("[BKA] Central banking report:\n");
    print_str("  Central categories: "); print_int(bka.n_cen_ban); print_str("\n");
    print_str("  Total monetary policy: "); print_int(bka.total_mon_pol); print_str("\n");
}

void bka_full_report(void) {
    print_str("[BKA] Full report:\n");
    print_str("  Investment categories: "); print_int(bka.n_inv_ban); print_str("\n");
    print_str("  Total securities issued: "); print_int(bka.total_sec_iss); print_str("\n");
    print_str("  Fintech categories: "); print_int(bka.n_fin_tech); print_str("\n");
    print_str("  Total digital currency: "); print_int(bka.total_dig_cur); print_str("\n");
    print_str("  International categories: "); print_int(bka.n_int_fin); print_str("\n");
    print_str("  Total international settlement: "); print_int(bka.total_int_set); print_str("\n");
}

void bka_print_state(void) {
    print_str("[BKA] Cb="); print_int(bka.n_com_ban);
    print_str(" Cnb="); print_int(bka.n_cen_ban);
    print_str(" Ib="); print_int(bka.n_inv_ban);
    print_str(" Ft="); print_int(bka.n_fin_tech);
    print_str(" Inf="); print_int(bka.n_int_fin);
    print_str("\n");
}

int main(void) {
    print_str("=== Banking Admin Demo ===\n\n");
    bka_init();

    print_str("Commercial banking...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cbs = 55 + (i * 13);
        int cmn = 40 + (i * 10);
        int cop = 22 + (i * 5);
        int rsk = 15 + (i * 3);
        int year = 2020 + (i % 5);
        bka_com_ban(type, cat, cbs, cmn, cop, rsk, year);
    }

    print_str("\nCentral banking...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mpl = 48 + (i * 11);
        int frg = 35 + (i * 8);
        int pcl = 20 + (i * 4);
        int fex = 12 + (i * 3);
        int year = 2021 + (i % 4);
        bka_cen_ban(type, cat, mpl, frg, pcl, fex, year);
    }

    print_str("\nInvestment banking...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sis = 42 + (i * 10);
        int mar = 28 + (i * 7);
        int asmn = 18 + (i * 4);
        int pek = 10 + (i * 2);
        int year = 2022 + (i % 3);
        bka_inv_ban(type, cat, sis, mar, asmn, pek, year);
    }

    print_str("\nFintech...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dcr = 35 + (i * 8);
        int blf = 25 + (i * 6);
        int iad = 15 + (i * 3);
        int brk = 10 + (i * 2);
        int year = 2023 + (i % 2);
        bka_fin_tech(type, cat, dcr, blf, iad, brk, year);
    }

    print_str("\nInternational finance...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int iset = 30 + (i * 7);
        int ifun = 22 + (i * 5);
        int iinv = 12 + (i * 3);
        int exr = 8 + (i * 2);
        int year = 2024;
        bka_int_fin(type, cat, iset, ifun, iinv, exr, year);
    }

    print_str("\nCommercial report...\n");
    bka_commercial_report();

    print_str("\nCentral report...\n");
    bka_central_report();

    print_str("\nFull report...\n");
    bka_full_report();

    print_str("\nFinal state...\n");
    bka_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
