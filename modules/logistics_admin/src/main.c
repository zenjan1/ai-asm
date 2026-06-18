/* logistics_admin: Logistics administration system (v1.0)
 * Logistics management, warehousing, distribution, procurement, international logistics
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

#define MAX_LOG_MAN     16
#define MAX_WAR_MAN     14
#define MAX_DIS_MAN     12
#define MAX_PUR_MAN     10
#define MAX_INT_LOG     10

typedef struct {
    int    lm_id;
    int    lm_type;
    int    lm_category;
    int    sup_cha;
    int    log_pla;
    int    log_ope;
    int    log_cos;
    int    year;
    int    active;
} log_man_t;

typedef struct {
    int    wm_id;
    int    wm_type;
    int    wm_category;
    int    war_pla;
    int    inv_con;
    int    goo_sto;
    int    war_equ;
    int    year;
    int    active;
} war_man_t;

typedef struct {
    int    dm_id;
    int    dm_type;
    int    dm_category;
    int    dis_cen;
    int    dis_rou;
    int    dis_tec;
    int    end_dis;
    int    year;
    int    active;
} dis_man_t;

typedef struct {
    int    pm_id;
    int    pm_type;
    int    pm_category;
    int    pur_str;
    int    sup_man;
    int    pur_cos;
    int    pur_con;
    int    year;
    int    active;
} pur_man_t;

typedef struct {
    int    il_id;
    int    il_type;
    int    il_category;
    int    int_fre;
    int    bon_log;
    int    cro_log;
    int    ovs_wrh;
    int    year;
    int    active;
} int_log_t;

typedef struct {
    int    n_log_man;
    int    n_war_man;
    int    n_dis_man;
    int    n_pur_man;
    int    n_int_log;
    int    total_sup_cha;
    int    total_war_pla;
    int    total_dis_cen;
    int    total_pur_str;
    int    total_int_fre;
} lga_state_t;

static log_man_t log_mans[MAX_LOG_MAN];
static war_man_t war_mans[MAX_WAR_MAN];
static dis_man_t dis_mans[MAX_DIS_MAN];
static pur_man_t pur_mans[MAX_PUR_MAN];
static int_log_t int_logs[MAX_INT_LOG];
static lga_state_t lga;

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

int lga_init(void) {
    if (initialized) return -1;
    lga.n_log_man = 0; lga.n_war_man = 0; lga.n_dis_man = 0;
    lga.n_pur_man = 0; lga.n_int_log = 0;
    lga.total_sup_cha = 0; lga.total_war_pla = 0;
    lga.total_dis_cen = 0; lga.total_pur_str = 0;
    lga.total_int_fre = 0;
    for (int i = 0; i < MAX_LOG_MAN; i++) log_mans[i].active = 0;
    for (int i = 0; i < MAX_WAR_MAN; i++) war_mans[i].active = 0;
    for (int i = 0; i < MAX_DIS_MAN; i++) dis_mans[i].active = 0;
    for (int i = 0; i < MAX_PUR_MAN; i++) pur_mans[i].active = 0;
    for (int i = 0; i < MAX_INT_LOG; i++) int_logs[i].active = 0;
    initialized = 1;
    print_str("[LGA] Logistics initialized\n");
    return 0;
}

int lga_log_man(int lt_type, int cat, int sch, int lpl, int lop, int lco, int year) {
    if (lga.n_log_man >= MAX_LOG_MAN) return -1;
    log_man_t* l = &log_mans[lga.n_log_man];
    l->lm_id = lga.n_log_man;
    l->lm_type = lt_type;
    l->lm_category = cat;
    l->sup_cha = sch;
    l->log_pla = lpl;
    l->log_ope = lop;
    l->log_cos = lco;
    l->year = year;
    l->active = 1;
    lga.total_sup_cha += sch;
    lga.n_log_man++;
    print_str("[LGA] Log man "); print_int(lga.n_log_man - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sch="); print_int(sch);
    print_str(" lpl="); print_int(lpl);
    print_str(" lop="); print_int(lop);
    print_str(" lco="); print_int(lco); print_str("\n");
    return lga.n_log_man - 1;
}

int lga_war_man(int wt_type, int cat, int wpl, int ico, int gst, int weq, int year) {
    if (lga.n_war_man >= MAX_WAR_MAN) return -1;
    war_man_t* w = &war_mans[lga.n_war_man];
    w->wm_id = lga.n_war_man;
    w->wm_type = wt_type;
    w->wm_category = cat;
    w->war_pla = wpl;
    w->inv_con = ico;
    w->goo_sto = gst;
    w->war_equ = weq;
    w->year = year;
    w->active = 1;
    lga.total_war_pla += wpl;
    lga.n_war_man++;
    print_str("[LGA] War man "); print_int(lga.n_war_man - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" wpl="); print_int(wpl);
    print_str(" ico="); print_int(ico);
    print_str(" gst="); print_int(gst);
    print_str(" weq="); print_int(weq); print_str("\n");
    return lga.n_war_man - 1;
}

int lga_dis_man(int dt_type, int cat, int dce, int dro, int dte, int edi, int year) {
    if (lga.n_dis_man >= MAX_DIS_MAN) return -1;
    dis_man_t* d = &dis_mans[lga.n_dis_man];
    d->dm_id = lga.n_dis_man;
    d->dm_type = dt_type;
    d->dm_category = cat;
    d->dis_cen = dce;
    d->dis_rou = dro;
    d->dis_tec = dte;
    d->end_dis = edi;
    d->year = year;
    d->active = 1;
    lga.total_dis_cen += dce;
    lga.n_dis_man++;
    print_str("[LGA] Dis man "); print_int(lga.n_dis_man - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dce="); print_int(dce);
    print_str(" dro="); print_int(dro);
    print_str(" dte="); print_int(dte);
    print_str(" edi="); print_int(edi); print_str("\n");
    return lga.n_dis_man - 1;
}

int lga_pur_man(int pt_type, int cat, int pst, int sma, int pco, int pco2, int year) {
    if (lga.n_pur_man >= MAX_PUR_MAN) return -1;
    pur_man_t* p = &pur_mans[lga.n_pur_man];
    p->pm_id = lga.n_pur_man;
    p->pm_type = pt_type;
    p->pm_category = cat;
    p->pur_str = pst;
    p->sup_man = sma;
    p->pur_cos = pco;
    p->pur_con = pco2;
    p->year = year;
    p->active = 1;
    lga.total_pur_str += pst;
    lga.n_pur_man++;
    print_str("[LGA] Pur man "); print_int(lga.n_pur_man - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pst="); print_int(pst);
    print_str(" sma="); print_int(sma);
    print_str(" pco="); print_int(pco);
    print_str(" pco2="); print_int(pco2); print_str("\n");
    return lga.n_pur_man - 1;
}

int lga_int_log(int it_type, int cat, int ifr, int bnl, int crl, int owh, int year) {
    if (lga.n_int_log >= MAX_INT_LOG) return -1;
    int_log_t* i = &int_logs[lga.n_int_log];
    i->il_id = lga.n_int_log;
    i->il_type = it_type;
    i->il_category = cat;
    i->int_fre = ifr;
    i->bon_log = bnl;
    i->cro_log = crl;
    i->ovs_wrh = owh;
    i->year = year;
    i->active = 1;
    lga.total_int_fre += ifr;
    lga.n_int_log++;
    print_str("[LGA] Int log "); print_int(lga.n_int_log - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ifr="); print_int(ifr);
    print_str(" bnl="); print_int(bnl);
    print_str(" crl="); print_int(crl);
    print_str(" owh="); print_int(owh); print_str("\n");
    return lga.n_int_log - 1;
}

void lga_management_report(void) {
    print_str("[LGA] Logistics management report:\n");
    print_str("  Management categories: "); print_int(lga.n_log_man); print_str("\n");
    print_str("  Total supply chain: "); print_int(lga.total_sup_cha); print_str("\n");
}

void lga_warehousing_report(void) {
    print_str("[LGA] Warehousing report:\n");
    print_str("  Warehousing categories: "); print_int(lga.n_war_man); print_str("\n");
    print_str("  Total warehouse planning: "); print_int(lga.total_war_pla); print_str("\n");
}

void lga_full_report(void) {
    print_str("[LGA] Full report:\n");
    print_str("  Distribution categories: "); print_int(lga.n_dis_man); print_str("\n");
    print_str("  Total distribution centers: "); print_int(lga.total_dis_cen); print_str("\n");
    print_str("  Procurement categories: "); print_int(lga.n_pur_man); print_str("\n");
    print_str("  Total procurement strategy: "); print_int(lga.total_pur_str); print_str("\n");
    print_str("  International categories: "); print_int(lga.n_int_log); print_str("\n");
    print_str("  Total international freight: "); print_int(lga.total_int_fre); print_str("\n");
}

void lga_print_state(void) {
    print_str("[LGA] Lm="); print_int(lga.n_log_man);
    print_str(" Wm="); print_int(lga.n_war_man);
    print_str(" Dm="); print_int(lga.n_dis_man);
    print_str(" Pm="); print_int(lga.n_pur_man);
    print_str(" Il="); print_int(lga.n_int_log);
    print_str("\n");
}

int main(void) {
    print_str("=== Logistics Admin Demo ===\n\n");
    lga_init();

    print_str("Logistics management...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sch = 55 + (i * 13);
        int lpl = 40 + (i * 10);
        int lop = 22 + (i * 5);
        int lco = 15 + (i * 3);
        int year = 2020 + (i % 5);
        lga_log_man(type, cat, sch, lpl, lop, lco, year);
    }

    print_str("\nWarehousing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int wpl = 48 + (i * 11);
        int ico = 35 + (i * 8);
        int gst = 20 + (i * 4);
        int weq = 12 + (i * 3);
        int year = 2021 + (i % 4);
        lga_war_man(type, cat, wpl, ico, gst, weq, year);
    }

    print_str("\nDistribution...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dce = 42 + (i * 10);
        int dro = 28 + (i * 7);
        int dte = 18 + (i * 4);
        int edi = 10 + (i * 2);
        int year = 2022 + (i % 3);
        lga_dis_man(type, cat, dce, dro, dte, edi, year);
    }

    print_str("\nProcurement...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pst = 35 + (i * 8);
        int sma = 25 + (i * 6);
        int pco = 15 + (i * 3);
        int pco2 = 10 + (i * 2);
        int year = 2023 + (i % 2);
        lga_pur_man(type, cat, pst, sma, pco, pco2, year);
    }

    print_str("\nInternational...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ifr = 30 + (i * 7);
        int bnl = 22 + (i * 5);
        int crl = 12 + (i * 3);
        int owh = 8 + (i * 2);
        int year = 2024;
        lga_int_log(type, cat, ifr, bnl, crl, owh, year);
    }

    print_str("\nManagement report...\n");
    lga_management_report();

    print_str("\nWarehousing report...\n");
    lga_warehousing_report();

    print_str("\nFull report...\n");
    lga_full_report();

    print_str("\nFinal state...\n");
    lga_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
