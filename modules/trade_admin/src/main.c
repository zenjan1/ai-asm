/* trade_admin: Trade administration system (v1.0)
 * International trade, domestic trade, e-commerce, service trade, border trade
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

#define MAX_INT_TRA    16
#define MAX_DOM_TRA    14
#define MAX_ECO_COM    12
#define MAX_SER_TRA    10
#define MAX_BOR_TRA    10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    tra_the;
    int    tra_pol;
    int    tra_pra;
    int    int_set;
    int    year;
    int    active;
} int_tra_t;

typedef struct {
    int    dt_id;
    int    dt_type;
    int    dt_category;
    int    dom_mar;
    int    com_dis;
    int    tra_reg;
    int    tra_fin;
    int    year;
    int    active;
} dom_tra_t;

typedef struct {
    int    ec_id;
    int    ec_type;
    int    ec_category;
    int    ecom_pla;
    int    ele_pay;
    int    net_mar;
    int    dat_ana;
    int    year;
    int    active;
} eco_com_t;

typedef struct {
    int    st_id;
    int    st_type;
    int    st_category;
    int    ser_the;
    int    ser_pol;
    int    ser_pra;
    int    ser_out;
    int    year;
    int    active;
} ser_tra_t;

typedef struct {
    int    bt_id;
    int    bt_type;
    int    bt_category;
    int    bor_the;
    int    bor_pol;
    int    bor_pra;
    int    por_man;
    int    year;
    int    active;
} bor_tra_t;

typedef struct {
    int    n_int_tra;
    int    n_dom_tra;
    int    n_eco_com;
    int    n_ser_tra;
    int    n_bor_tra;
    int    total_tra_the;
    int    total_dom_mar;
    int    total_ecom_pla;
    int    total_ser_the;
    int    total_bor_the;
} tda_state_t;

static int_tra_t int_tras[MAX_INT_TRA];
static dom_tra_t dom_tras[MAX_DOM_TRA];
static eco_com_t eco_coms[MAX_ECO_COM];
static ser_tra_t ser_tras[MAX_SER_TRA];
static bor_tra_t bor_tras[MAX_BOR_TRA];
static tda_state_t tda;

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

int tda_init(void) {
    if (initialized) return -1;
    tda.n_int_tra = 0; tda.n_dom_tra = 0; tda.n_eco_com = 0;
    tda.n_ser_tra = 0; tda.n_bor_tra = 0;
    tda.total_tra_the = 0; tda.total_dom_mar = 0;
    tda.total_ecom_pla = 0; tda.total_ser_the = 0;
    tda.total_bor_the = 0;
    for (int i = 0; i < MAX_INT_TRA; i++) int_tras[i].active = 0;
    for (int i = 0; i < MAX_DOM_TRA; i++) dom_tras[i].active = 0;
    for (int i = 0; i < MAX_ECO_COM; i++) eco_coms[i].active = 0;
    for (int i = 0; i < MAX_SER_TRA; i++) ser_tras[i].active = 0;
    for (int i = 0; i < MAX_BOR_TRA; i++) bor_tras[i].active = 0;
    initialized = 1;
    print_str("[TDA] Trade initialized\n");
    return 0;
}

int tda_int_tra(int it_type, int cat, int tth, int tpo, int tpr, int ist, int year) {
    if (tda.n_int_tra >= MAX_INT_TRA) return -1;
    int_tra_t* t = &int_tras[tda.n_int_tra];
    t->it_id = tda.n_int_tra;
    t->it_type = it_type;
    t->it_category = cat;
    t->tra_the = tth;
    t->tra_pol = tpo;
    t->tra_pra = tpr;
    t->int_set = ist;
    t->year = year;
    t->active = 1;
    tda.total_tra_the += tth;
    tda.n_int_tra++;
    print_str("[TDA] Int tra "); print_int(tda.n_int_tra - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" tth="); print_int(tth);
    print_str(" tpo="); print_int(tpo);
    print_str(" tpr="); print_int(tpr);
    print_str(" ist="); print_int(ist); print_str("\n");
    return tda.n_int_tra - 1;
}

int tda_dom_tra(int dt_type, int cat, int dmr, int cds, int trg, int tfn, int year) {
    if (tda.n_dom_tra >= MAX_DOM_TRA) return -1;
    dom_tra_t* d = &dom_tras[tda.n_dom_tra];
    d->dt_id = tda.n_dom_tra;
    d->dt_type = dt_type;
    d->dt_category = cat;
    d->dom_mar = dmr;
    d->com_dis = cds;
    d->tra_reg = trg;
    d->tra_fin = tfn;
    d->year = year;
    d->active = 1;
    tda.total_dom_mar += dmr;
    tda.n_dom_tra++;
    print_str("[TDA] Dom tra "); print_int(tda.n_dom_tra - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dmr="); print_int(dmr);
    print_str(" cds="); print_int(cds);
    print_str(" trg="); print_int(trg);
    print_str(" tfn="); print_int(tfn); print_str("\n");
    return tda.n_dom_tra - 1;
}

int tda_eco_com(int ec_type, int cat, int ecp, int epa, int nmr, int dna, int year) {
    if (tda.n_eco_com >= MAX_ECO_COM) return -1;
    eco_com_t* e = &eco_coms[tda.n_eco_com];
    e->ec_id = tda.n_eco_com;
    e->ec_type = ec_type;
    e->ec_category = cat;
    e->ecom_pla = ecp;
    e->ele_pay = epa;
    e->net_mar = nmr;
    e->dat_ana = dna;
    e->year = year;
    e->active = 1;
    tda.total_ecom_pla += ecp;
    tda.n_eco_com++;
    print_str("[TDA] Eco com "); print_int(tda.n_eco_com - 1);
    print_str(" type="); print_int(ec_type);
    print_str(" cat="); print_int(cat);
    print_str(" ecp="); print_int(ecp);
    print_str(" epa="); print_int(epa);
    print_str(" nmr="); print_int(nmr);
    print_str(" dna="); print_int(dna); print_str("\n");
    return tda.n_eco_com - 1;
}

int tda_ser_tra(int st_type, int cat, int sth, int spo, int spr, int sot, int year) {
    if (tda.n_ser_tra >= MAX_SER_TRA) return -1;
    ser_tra_t* s = &ser_tras[tda.n_ser_tra];
    s->st_id = tda.n_ser_tra;
    s->st_type = st_type;
    s->st_category = cat;
    s->ser_the = sth;
    s->ser_pol = spo;
    s->ser_pra = spr;
    s->ser_out = sot;
    s->year = year;
    s->active = 1;
    tda.total_ser_the += sth;
    tda.n_ser_tra++;
    print_str("[TDA] Ser tra "); print_int(tda.n_ser_tra - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" sth="); print_int(sth);
    print_str(" spo="); print_int(spo);
    print_str(" spr="); print_int(spr);
    print_str(" sot="); print_int(sot); print_str("\n");
    return tda.n_ser_tra - 1;
}

int tda_bor_tra(int bt_type, int cat, int bth, int bpo, int bpr, int pmn, int year) {
    if (tda.n_bor_tra >= MAX_BOR_TRA) return -1;
    bor_tra_t* b = &bor_tras[tda.n_bor_tra];
    b->bt_id = tda.n_bor_tra;
    b->bt_type = bt_type;
    b->bt_category = cat;
    b->bor_the = bth;
    b->bor_pol = bpo;
    b->bor_pra = bpr;
    b->por_man = pmn;
    b->year = year;
    b->active = 1;
    tda.total_bor_the += bth;
    tda.n_bor_tra++;
    print_str("[TDA] Bor tra "); print_int(tda.n_bor_tra - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" bth="); print_int(bth);
    print_str(" bpo="); print_int(bpo);
    print_str(" bpr="); print_int(bpr);
    print_str(" pmn="); print_int(pmn); print_str("\n");
    return tda.n_bor_tra - 1;
}

void tda_international_report(void) {
    print_str("[TDA] International trade report:\n");
    print_str("  Trade categories: "); print_int(tda.n_int_tra); print_str("\n");
    print_str("  Total trade theory: "); print_int(tda.total_tra_the); print_str("\n");
}

void tda_domestic_report(void) {
    print_str("[TDA] Domestic trade report:\n");
    print_str("  Domestic categories: "); print_int(tda.n_dom_tra); print_str("\n");
    print_str("  Total domestic market: "); print_int(tda.total_dom_mar); print_str("\n");
}

void tda_full_report(void) {
    print_str("[TDA] Full report:\n");
    print_str("  E-commerce categories: "); print_int(tda.n_eco_com); print_str("\n");
    print_str("  Total e-commerce platforms: "); print_int(tda.total_ecom_pla); print_str("\n");
    print_str("  Service trade categories: "); print_int(tda.n_ser_tra); print_str("\n");
    print_str("  Total service theory: "); print_int(tda.total_ser_the); print_str("\n");
    print_str("  Border trade categories: "); print_int(tda.n_bor_tra); print_str("\n");
    print_str("  Total border theory: "); print_int(tda.total_bor_the); print_str("\n");
}

void tda_print_state(void) {
    print_str("[TDA] It="); print_int(tda.n_int_tra);
    print_str(" Dt="); print_int(tda.n_dom_tra);
    print_str(" Ec="); print_int(tda.n_eco_com);
    print_str(" St="); print_int(tda.n_ser_tra);
    print_str(" Bt="); print_int(tda.n_bor_tra);
    print_str("\n");
}

int main(void) {
    print_str("=== Trade Admin Demo ===\n\n");
    tda_init();

    print_str("International trade...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int tth = 55 + (i * 13);
        int tpo = 40 + (i * 10);
        int tpr = 22 + (i * 5);
        int ist = 15 + (i * 3);
        int year = 2020 + (i % 5);
        tda_int_tra(type, cat, tth, tpo, tpr, ist, year);
    }

    print_str("\nDomestic trade...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dmr = 48 + (i * 11);
        int cds = 35 + (i * 8);
        int trg = 20 + (i * 4);
        int tfn = 12 + (i * 3);
        int year = 2021 + (i % 4);
        tda_dom_tra(type, cat, dmr, cds, trg, tfn, year);
    }

    print_str("\nE-commerce...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ecp = 42 + (i * 10);
        int epa = 28 + (i * 7);
        int nmr = 18 + (i * 4);
        int dna = 10 + (i * 2);
        int year = 2022 + (i % 3);
        tda_eco_com(type, cat, ecp, epa, nmr, dna, year);
    }

    print_str("\nService trade...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sth = 35 + (i * 8);
        int spo = 25 + (i * 6);
        int spr = 15 + (i * 3);
        int sot = 10 + (i * 2);
        int year = 2023 + (i % 2);
        tda_ser_tra(type, cat, sth, spo, spr, sot, year);
    }

    print_str("\nBorder trade...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bth = 30 + (i * 7);
        int bpo = 22 + (i * 5);
        int bpr = 12 + (i * 3);
        int pmn = 8 + (i * 2);
        int year = 2024;
        tda_bor_tra(type, cat, bth, bpo, bpr, pmn, year);
    }

    print_str("\nInternational report...\n");
    tda_international_report();

    print_str("\nDomestic report...\n");
    tda_domestic_report();

    print_str("\nFull report...\n");
    tda_full_report();

    print_str("\nFinal state...\n");
    tda_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
