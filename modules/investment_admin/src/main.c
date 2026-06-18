/* investment_admin: Investment administration system (v1.0)
 * Investment theory, securities investment, real investment, venture capital, intl investment
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

#define MAX_INV_THE    16
#define MAX_SEC_INV    14
#define MAX_REA_INV    12
#define MAX_VEN_CAP    10
#define MAX_INT_INV    10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    inv_pri;
    int    ast_pri;
    int    inv_por;
    int    risk_man;
    int    year;
    int    active;
} inv_the_t;

typedef struct {
    int    si_id;
    int    si_type;
    int    si_category;
    int    sto_inv;
    int    bon_inv;
    int    fun_inv;
    int    der_inv;
    int    year;
    int    active;
} sec_inv_t;

typedef struct {
    int    ri_id;
    int    ri_type;
    int    ri_category;
    int    rea_inv;
    int    inf_inv;
    int    com_inv;
    int    art_inv;
    int    year;
    int    active;
} rea_inv_t;

typedef struct {
    int    vc_id;
    int    vc_type;
    int    vc_category;
    int    ven_cap;
    int    pri_equ;
    int    ang_inv;
    int    ma_inv;
    int    year;
    int    active;
} ven_cap_t;

typedef struct {
    int    ii_id;
    int    ii_type;
    int    ii_category;
    int    cro_inv;
    int    for_exc;
    int    int_sec;
    int    fdi_inv;
    int    year;
    int    active;
} int_inv_t;

typedef struct {
    int    n_inv_the;
    int    n_sec_inv;
    int    n_rea_inv;
    int    n_ven_cap;
    int    n_int_inv;
    int    total_inv_pri;
    int    total_sto_inv;
    int    total_rea_inv;
    int    total_ven_cap;
    int    total_cro_inv;
} inv_state_t;

static inv_the_t inv_thes[MAX_INV_THE];
static sec_inv_t sec_invs[MAX_SEC_INV];
static rea_inv_t rea_invs[MAX_REA_INV];
static ven_cap_t ven_caps[MAX_VEN_CAP];
static int_inv_t int_invs[MAX_INT_INV];
static inv_state_t inva;

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

int inva_init(void) {
    if (initialized) return -1;
    inva.n_inv_the = 0; inva.n_sec_inv = 0; inva.n_rea_inv = 0;
    inva.n_ven_cap = 0; inva.n_int_inv = 0;
    inva.total_inv_pri = 0; inva.total_sto_inv = 0;
    inva.total_rea_inv = 0; inva.total_ven_cap = 0;
    inva.total_cro_inv = 0;
    for (int i = 0; i < MAX_INV_THE; i++) inv_thes[i].active = 0;
    for (int i = 0; i < MAX_SEC_INV; i++) sec_invs[i].active = 0;
    for (int i = 0; i < MAX_REA_INV; i++) rea_invs[i].active = 0;
    for (int i = 0; i < MAX_VEN_CAP; i++) ven_caps[i].active = 0;
    for (int i = 0; i < MAX_INT_INV; i++) int_invs[i].active = 0;
    initialized = 1;
    print_str("[IVA] Investment initialized\n");
    return 0;
}

int inva_inv_the(int it_type, int cat, int ipri, int apri, int ipor, int rman, int year) {
    if (inva.n_inv_the >= MAX_INV_THE) return -1;
    inv_the_t* t = &inv_thes[inva.n_inv_the];
    t->it_id = inva.n_inv_the;
    t->it_type = it_type;
    t->it_category = cat;
    t->inv_pri = ipri;
    t->ast_pri = apri;
    t->inv_por = ipor;
    t->risk_man = rman;
    t->year = year;
    t->active = 1;
    inva.total_inv_pri += ipri;
    inva.n_inv_the++;
    print_str("[IVA] Inv the "); print_int(inva.n_inv_the - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ipri="); print_int(ipri);
    print_str(" apri="); print_int(apri);
    print_str(" ipor="); print_int(ipor);
    print_str(" rman="); print_int(rman); print_str("\n");
    return inva.n_inv_the - 1;
}

int inva_sec_inv(int st_type, int cat, int stoi, int boni, int funi, int deri, int year) {
    if (inva.n_sec_inv >= MAX_SEC_INV) return -1;
    sec_inv_t* s = &sec_invs[inva.n_sec_inv];
    s->si_id = inva.n_sec_inv;
    s->si_type = st_type;
    s->si_category = cat;
    s->sto_inv = stoi;
    s->bon_inv = boni;
    s->fun_inv = funi;
    s->der_inv = deri;
    s->year = year;
    s->active = 1;
    inva.total_sto_inv += stoi;
    inva.n_sec_inv++;
    print_str("[IVA] Sec inv "); print_int(inva.n_sec_inv - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" stoi="); print_int(stoi);
    print_str(" boni="); print_int(boni);
    print_str(" funi="); print_int(funi);
    print_str(" deri="); print_int(deri); print_str("\n");
    return inva.n_sec_inv - 1;
}

int inva_rea_inv(int rt_type, int cat, int rei, int infi, int comi, int arti, int year) {
    if (inva.n_rea_inv >= MAX_REA_INV) return -1;
    rea_inv_t* r = &rea_invs[inva.n_rea_inv];
    r->ri_id = inva.n_rea_inv;
    r->ri_type = rt_type;
    r->ri_category = cat;
    r->rea_inv = rei;
    r->inf_inv = infi;
    r->com_inv = comi;
    r->art_inv = arti;
    r->year = year;
    r->active = 1;
    inva.total_rea_inv += rei;
    inva.n_rea_inv++;
    print_str("[IVA] Rea inv "); print_int(inva.n_rea_inv - 1);
    print_str(" type="); print_int(rt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rei="); print_int(rei);
    print_str(" infi="); print_int(infi);
    print_str(" comi="); print_int(comi);
    print_str(" arti="); print_int(arti); print_str("\n");
    return inva.n_rea_inv - 1;
}

int inva_ven_cap(int vt_type, int cat, int vcap, int pequ, int angv, int mainv, int year) {
    if (inva.n_ven_cap >= MAX_VEN_CAP) return -1;
    ven_cap_t* v = &ven_caps[inva.n_ven_cap];
    v->vc_id = inva.n_ven_cap;
    v->vc_type = vt_type;
    v->vc_category = cat;
    v->ven_cap = vcap;
    v->pri_equ = pequ;
    v->ang_inv = angv;
    v->ma_inv = mainv;
    v->year = year;
    v->active = 1;
    inva.total_ven_cap += vcap;
    inva.n_ven_cap++;
    print_str("[IVA] Ven cap "); print_int(inva.n_ven_cap - 1);
    print_str(" type="); print_int(vt_type);
    print_str(" cat="); print_int(cat);
    print_str(" vcap="); print_int(vcap);
    print_str(" pequ="); print_int(pequ);
    print_str(" angv="); print_int(angv);
    print_str(" mainv="); print_int(mainv); print_str("\n");
    return inva.n_ven_cap - 1;
}

int inva_int_inv(int it_type, int cat, int croi, int fexc, int isec, int fdi, int year) {
    if (inva.n_int_inv >= MAX_INT_INV) return -1;
    int_inv_t* b = &int_invs[inva.n_int_inv];
    b->ii_id = inva.n_int_inv;
    b->ii_type = it_type;
    b->ii_category = cat;
    b->cro_inv = croi;
    b->for_exc = fexc;
    b->int_sec = isec;
    b->fdi_inv = fdi;
    b->year = year;
    b->active = 1;
    inva.total_cro_inv += croi;
    inva.n_int_inv++;
    print_str("[IVA] Int inv "); print_int(inva.n_int_inv - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" croi="); print_int(croi);
    print_str(" fexc="); print_int(fexc);
    print_str(" isec="); print_int(isec);
    print_str(" fdi="); print_int(fdi); print_str("\n");
    return inva.n_int_inv - 1;
}

void inva_theory_report(void) {
    print_str("[IVA] Investment theory report:\n");
    print_str("  Theory categories: "); print_int(inva.n_inv_the); print_str("\n");
    print_str("  Total investment principles: "); print_int(inva.total_inv_pri); print_str("\n");
}

void inva_securities_report(void) {
    print_str("[IVA] Securities investment report:\n");
    print_str("  Securities categories: "); print_int(inva.n_sec_inv); print_str("\n");
    print_str("  Total stock investment: "); print_int(inva.total_sto_inv); print_str("\n");
}

void inva_full_report(void) {
    print_str("[IVA] Full report:\n");
    print_str("  Real investment categories: "); print_int(inva.n_rea_inv); print_str("\n");
    print_str("  Total real investment: "); print_int(inva.total_rea_inv); print_str("\n");
    print_str("  Venture capital categories: "); print_int(inva.n_ven_cap); print_str("\n");
    print_str("  Total venture capital: "); print_int(inva.total_ven_cap); print_str("\n");
    print_str("  International categories: "); print_int(inva.n_int_inv); print_str("\n");
    print_str("  Total cross-border investment: "); print_int(inva.total_cro_inv); print_str("\n");
}

void inva_print_state(void) {
    print_str("[IVA] It="); print_int(inva.n_inv_the);
    print_str(" Si="); print_int(inva.n_sec_inv);
    print_str(" Ri="); print_int(inva.n_rea_inv);
    print_str(" Vc="); print_int(inva.n_ven_cap);
    print_str(" Ii="); print_int(inva.n_int_inv);
    print_str("\n");
}

int main(void) {
    print_str("=== Investment Admin Demo ===\n\n");
    inva_init();

    print_str("Investment theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ipri = 55 + (i * 13);
        int apri = 40 + (i * 10);
        int ipor = 22 + (i * 5);
        int rman = 15 + (i * 3);
        int year = 2020 + (i % 5);
        inva_inv_the(type, cat, ipri, apri, ipor, rman, year);
    }

    print_str("\nSecurities investment...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int stoi = 48 + (i * 11);
        int boni = 35 + (i * 8);
        int funi = 20 + (i * 4);
        int deri = 12 + (i * 3);
        int year = 2021 + (i % 4);
        inva_sec_inv(type, cat, stoi, boni, funi, deri, year);
    }

    print_str("\nReal investment...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rei = 42 + (i * 10);
        int infi = 28 + (i * 7);
        int comi = 18 + (i * 4);
        int arti = 10 + (i * 2);
        int year = 2022 + (i % 3);
        inva_rea_inv(type, cat, rei, infi, comi, arti, year);
    }

    print_str("\nVenture capital...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int vcap = 35 + (i * 8);
        int pequ = 25 + (i * 6);
        int angv = 15 + (i * 3);
        int mainv = 10 + (i * 2);
        int year = 2023 + (i % 2);
        inva_ven_cap(type, cat, vcap, pequ, angv, mainv, year);
    }

    print_str("\nInternational investment...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int croi = 30 + (i * 7);
        int fexc = 22 + (i * 5);
        int isec = 12 + (i * 3);
        int fdi = 8 + (i * 2);
        int year = 2024;
        inva_int_inv(type, cat, croi, fexc, isec, fdi, year);
    }

    print_str("\nTheory report...\n");
    inva_theory_report();

    print_str("\nSecurities report...\n");
    inva_securities_report();

    print_str("\nFull report...\n");
    inva_full_report();

    print_str("\nFinal state...\n");
    inva_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
