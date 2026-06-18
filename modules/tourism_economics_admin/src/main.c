/* tourism_economics_admin: Tourism economics administration system (v1.0)
 * Tourism theory, tourism planning, hotel management, travel services, tourism marketing
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

#define MAX_TOU_THE    16
#define MAX_TOU_PLA    14
#define MAX_HOT_MAN    12
#define MAX_TRA_SER    10
#define MAX_TOU_MKT    10

typedef struct {
    int    tt_id;
    int    tt_type;
    int    tt_category;
    int    tou_econ;
    int    tou_con;
    int    tou_mar;
    int    tou_pro;
    int    year;
    int    active;
} tou_the_t;

typedef struct {
    int    tp_id;
    int    tp_type;
    int    tp_category;
    int    tou_res;
    int    reg_pla;
    int    rou_des;
    int    sce_man;
    int    year;
    int    active;
} tou_pla_t;

typedef struct {
    int    hm_id;
    int    hm_type;
    int    hm_category;
    int    hot_opr;
    int    hot_mar;
    int    hot_ser;
    int    hot_hr;
    int    year;
    int    active;
} hot_man_t;

typedef struct {
    int    ts_id;
    int    ts_type;
    int    ts_category;
    int    tra_opr;
    int    gui_ser;
    int    tra_tra;
    int    tra_ins;
    int    year;
    int    active;
} tra_ser_t;

typedef struct {
    int    tm_id;
    int    tm_type;
    int    tm_category;
    int    tou_bra;
    int    tou_pro2;
    int    dig_tou;
    int    tou_eco;
    int    year;
    int    active;
} tou_mkt_t;

typedef struct {
    int    n_tou_the;
    int    n_tou_pla;
    int    n_hot_man;
    int    n_tra_ser;
    int    n_tou_mkt;
    int    total_tou_econ;
    int    total_tou_res;
    int    total_hot_opr;
    int    total_tra_opr;
    int    total_tou_bra;
} tea_state_t;

static tou_the_t tou_thes[MAX_TOU_THE];
static tou_pla_t tou_plas[MAX_TOU_PLA];
static hot_man_t hot_mans[MAX_HOT_MAN];
static tra_ser_t tra_sers[MAX_TRA_SER];
static tou_mkt_t tou_mkts[MAX_TOU_MKT];
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
    tea.n_tou_the = 0; tea.n_tou_pla = 0; tea.n_hot_man = 0;
    tea.n_tra_ser = 0; tea.n_tou_mkt = 0;
    tea.total_tou_econ = 0; tea.total_tou_res = 0;
    tea.total_hot_opr = 0; tea.total_tra_opr = 0;
    tea.total_tou_bra = 0;
    for (int i = 0; i < MAX_TOU_THE; i++) tou_thes[i].active = 0;
    for (int i = 0; i < MAX_TOU_PLA; i++) tou_plas[i].active = 0;
    for (int i = 0; i < MAX_HOT_MAN; i++) hot_mans[i].active = 0;
    for (int i = 0; i < MAX_TRA_SER; i++) tra_sers[i].active = 0;
    for (int i = 0; i < MAX_TOU_MKT; i++) tou_mkts[i].active = 0;
    initialized = 1;
    print_str("[TEA] Tourism initialized\n");
    return 0;
}

int tea_tou_the(int tt_type, int cat, int tecn, int tcon, int tmar, int tpro, int year) {
    if (tea.n_tou_the >= MAX_TOU_THE) return -1;
    tou_the_t* t = &tou_thes[tea.n_tou_the];
    t->tt_id = tea.n_tou_the;
    t->tt_type = tt_type;
    t->tt_category = cat;
    t->tou_econ = tecn;
    t->tou_con = tcon;
    t->tou_mar = tmar;
    t->tou_pro = tpro;
    t->year = year;
    t->active = 1;
    tea.total_tou_econ += tecn;
    tea.n_tou_the++;
    print_str("[TEA] Tou the "); print_int(tea.n_tou_the - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tecn="); print_int(tecn);
    print_str(" tcon="); print_int(tcon);
    print_str(" tmar="); print_int(tmar);
    print_str(" tpro="); print_int(tpro); print_str("\n");
    return tea.n_tou_the - 1;
}

int tea_tou_pla(int pt_type, int cat, int tres, int rpla, int rdes, int sman, int year) {
    if (tea.n_tou_pla >= MAX_TOU_PLA) return -1;
    tou_pla_t* p = &tou_plas[tea.n_tou_pla];
    p->tp_id = tea.n_tou_pla;
    p->tp_type = pt_type;
    p->tp_category = cat;
    p->tou_res = tres;
    p->reg_pla = rpla;
    p->rou_des = rdes;
    p->sce_man = sman;
    p->year = year;
    p->active = 1;
    tea.total_tou_res += tres;
    tea.n_tou_pla++;
    print_str("[TEA] Tou pla "); print_int(tea.n_tou_pla - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tres="); print_int(tres);
    print_str(" rpla="); print_int(rpla);
    print_str(" rdes="); print_int(rdes);
    print_str(" sman="); print_int(sman); print_str("\n");
    return tea.n_tou_pla - 1;
}

int tea_hot_man(int ht_type, int cat, int hopr, int hmar, int hser, int hhr, int year) {
    if (tea.n_hot_man >= MAX_HOT_MAN) return -1;
    hot_man_t* h = &hot_mans[tea.n_hot_man];
    h->hm_id = tea.n_hot_man;
    h->hm_type = ht_type;
    h->hm_category = cat;
    h->hot_opr = hopr;
    h->hot_mar = hmar;
    h->hot_ser = hser;
    h->hot_hr = hhr;
    h->year = year;
    h->active = 1;
    tea.total_hot_opr += hopr;
    tea.n_hot_man++;
    print_str("[TEA] Hot man "); print_int(tea.n_hot_man - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" hopr="); print_int(hopr);
    print_str(" hmar="); print_int(hmar);
    print_str(" hser="); print_int(hser);
    print_str(" hhr="); print_int(hhr); print_str("\n");
    return tea.n_hot_man - 1;
}

int tea_tra_ser(int tt_type2, int cat, int topr, int gser, int ttra, int tins, int year) {
    if (tea.n_tra_ser >= MAX_TRA_SER) return -1;
    tra_ser_t* t = &tra_sers[tea.n_tra_ser];
    t->ts_id = tea.n_tra_ser;
    t->ts_type = tt_type2;
    t->ts_category = cat;
    t->tra_opr = topr;
    t->gui_ser = gser;
    t->tra_tra = ttra;
    t->tra_ins = tins;
    t->year = year;
    t->active = 1;
    tea.total_tra_opr += topr;
    tea.n_tra_ser++;
    print_str("[TEA] Tra ser "); print_int(tea.n_tra_ser - 1);
    print_str(" type="); print_int(tt_type2);
    print_str(" cat="); print_int(cat);
    print_str(" topr="); print_int(topr);
    print_str(" gser="); print_int(gser);
    print_str(" ttra="); print_int(ttra);
    print_str(" tins="); print_int(tins); print_str("\n");
    return tea.n_tra_ser - 1;
}

int tea_tou_mkt(int mt_type, int cat, int tbra, int tpro2, int dtou, int teco, int year) {
    if (tea.n_tou_mkt >= MAX_TOU_MKT) return -1;
    tou_mkt_t* m = &tou_mkts[tea.n_tou_mkt];
    m->tm_id = tea.n_tou_mkt;
    m->tm_type = mt_type;
    m->tm_category = cat;
    m->tou_bra = tbra;
    m->tou_pro2 = tpro2;
    m->dig_tou = dtou;
    m->tou_eco = teco;
    m->year = year;
    m->active = 1;
    tea.total_tou_bra += tbra;
    tea.n_tou_mkt++;
    print_str("[TEA] Tou mkt "); print_int(tea.n_tou_mkt - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" tbra="); print_int(tbra);
    print_str(" tpro2="); print_int(tpro2);
    print_str(" dtou="); print_int(dtou);
    print_str(" teco="); print_int(teco); print_str("\n");
    return tea.n_tou_mkt - 1;
}

void tea_theory_report(void) {
    print_str("[TEA] Tourism theory report:\n");
    print_str("  Theory categories: "); print_int(tea.n_tou_the); print_str("\n");
    print_str("  Total tourism economics: "); print_int(tea.total_tou_econ); print_str("\n");
}

void tea_planning_report(void) {
    print_str("[TEA] Tourism planning report:\n");
    print_str("  Planning categories: "); print_int(tea.n_tou_pla); print_str("\n");
    print_str("  Total tourism resources: "); print_int(tea.total_tou_res); print_str("\n");
}

void tea_full_report(void) {
    print_str("[TEA] Full report:\n");
    print_str("  Hotel categories: "); print_int(tea.n_hot_man); print_str("\n");
    print_str("  Total hotel operations: "); print_int(tea.total_hot_opr); print_str("\n");
    print_str("  Travel categories: "); print_int(tea.n_tra_ser); print_str("\n");
    print_str("  Total travel operations: "); print_int(tea.total_tra_opr); print_str("\n");
    print_str("  Marketing categories: "); print_int(tea.n_tou_mkt); print_str("\n");
    print_str("  Total tourism brands: "); print_int(tea.total_tou_bra); print_str("\n");
}

void tea_print_state(void) {
    print_str("[TEA] Tt="); print_int(tea.n_tou_the);
    print_str(" Tp="); print_int(tea.n_tou_pla);
    print_str(" Hm="); print_int(tea.n_hot_man);
    print_str(" Ts="); print_int(tea.n_tra_ser);
    print_str(" Tm="); print_int(tea.n_tou_mkt);
    print_str("\n");
}

int main(void) {
    print_str("=== Tourism Economics Admin Demo ===\n\n");
    tea_init();

    print_str("Tourism theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int tecn = 55 + (i * 13);
        int tcon = 40 + (i * 10);
        int tmar = 22 + (i * 5);
        int tpro = 15 + (i * 3);
        int year = 2020 + (i % 5);
        tea_tou_the(type, cat, tecn, tcon, tmar, tpro, year);
    }

    print_str("\nTourism planning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tres = 48 + (i * 11);
        int rpla = 35 + (i * 8);
        int rdes = 20 + (i * 4);
        int sman = 12 + (i * 3);
        int year = 2021 + (i % 4);
        tea_tou_pla(type, cat, tres, rpla, rdes, sman, year);
    }

    print_str("\nHotel management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hopr = 42 + (i * 10);
        int hmar = 28 + (i * 7);
        int hser = 18 + (i * 4);
        int hhr = 10 + (i * 2);
        int year = 2022 + (i % 3);
        tea_hot_man(type, cat, hopr, hmar, hser, hhr, year);
    }

    print_str("\nTravel services...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int topr = 35 + (i * 8);
        int gser = 25 + (i * 6);
        int ttra = 15 + (i * 3);
        int tins = 10 + (i * 2);
        int year = 2023 + (i % 2);
        tea_tra_ser(type, cat, topr, gser, ttra, tins, year);
    }

    print_str("\nTourism marketing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tbra = 30 + (i * 7);
        int tpro2 = 22 + (i * 5);
        int dtou = 12 + (i * 3);
        int teco = 8 + (i * 2);
        int year = 2024;
        tea_tou_mkt(type, cat, tbra, tpro2, dtou, teco, year);
    }

    print_str("\nTheory report...\n");
    tea_theory_report();

    print_str("\nPlanning report...\n");
    tea_planning_report();

    print_str("\nFull report...\n");
    tea_full_report();

    print_str("\nFinal state...\n");
    tea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
