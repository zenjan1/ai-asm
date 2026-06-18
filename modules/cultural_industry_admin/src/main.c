/* cultural_industry_admin: Cultural industry administration system (v1.0)
 * Industry theory, cultural marketing, digital culture, cultural trade, creative industry
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

#define MAX_IND_THE    16
#define MAX_CUL_MKT    14
#define MAX_DIG_CUL    12
#define MAX_CUL_TRA    10
#define MAX_CRE_IND    10

typedef struct {
    int    it_id;
    int    it_type;
    int    it_category;
    int    ind_the;
    int    cul_econ;
    int    cul_man;
    int    cul_pol;
    int    year;
    int    active;
} ind_the_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    cul_bra;
    int    cul_pro;
    int    cul_mar;
    int    cul_con;
    int    year;
    int    active;
} cul_mkt_t;

typedef struct {
    int    dc_id;
    int    dc_type;
    int    dc_category;
    int    dig_con;
    int    net_cul;
    int    dig_pub;
    int    new_med;
    int    year;
    int    active;
} dig_cul_t;

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    cul_pro2;
    int    cul_ser;
    int    cul_exc;
    int    cul_exp;
    int    year;
    int    active;
} cul_tra_t;

typedef struct {
    int    ci_id;
    int    ci_type;
    int    ci_category;
    int    cre_des;
    int    cre_par;
    int    cre_tal;
    int    cre_inv;
    int    year;
    int    active;
} cre_ind_t;

typedef struct {
    int    n_ind_the;
    int    n_cul_mkt;
    int    n_dig_cul;
    int    n_cul_tra;
    int    n_cre_ind;
    int    total_ind_the;
    int    total_cul_bra;
    int    total_dig_con;
    int    total_cul_pro2;
    int    total_cre_des;
} cia_state_t;

static ind_the_t ind_thes[MAX_IND_THE];
static cul_mkt_t cul_mkts[MAX_CUL_MKT];
static dig_cul_t dig_culs[MAX_DIG_CUL];
static cul_tra_t cul_tras[MAX_CUL_TRA];
static cre_ind_t cre_inds[MAX_CRE_IND];
static cia_state_t cia;

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

int cia_init(void) {
    if (initialized) return -1;
    cia.n_ind_the = 0; cia.n_cul_mkt = 0; cia.n_dig_cul = 0;
    cia.n_cul_tra = 0; cia.n_cre_ind = 0;
    cia.total_ind_the = 0; cia.total_cul_bra = 0;
    cia.total_dig_con = 0; cia.total_cul_pro2 = 0;
    cia.total_cre_des = 0;
    for (int i = 0; i < MAX_IND_THE; i++) ind_thes[i].active = 0;
    for (int i = 0; i < MAX_CUL_MKT; i++) cul_mkts[i].active = 0;
    for (int i = 0; i < MAX_DIG_CUL; i++) dig_culs[i].active = 0;
    for (int i = 0; i < MAX_CUL_TRA; i++) cul_tras[i].active = 0;
    for (int i = 0; i < MAX_CRE_IND; i++) cre_inds[i].active = 0;
    initialized = 1;
    print_str("[CIA] Cultural industry initialized\n");
    return 0;
}

int cia_ind_the(int it_type, int cat, int ithe, int cecon, int cman, int cpol, int year) {
    if (cia.n_ind_the >= MAX_IND_THE) return -1;
    ind_the_t* b = &ind_thes[cia.n_ind_the];
    b->it_id = cia.n_ind_the;
    b->it_type = it_type;
    b->it_category = cat;
    b->ind_the = ithe;
    b->cul_econ = cecon;
    b->cul_man = cman;
    b->cul_pol = cpol;
    b->year = year;
    b->active = 1;
    cia.total_ind_the += ithe;
    cia.n_ind_the++;
    print_str("[CIA] Ind the "); print_int(cia.n_ind_the - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ithe="); print_int(ithe);
    print_str(" cecon="); print_int(cecon);
    print_str(" cman="); print_int(cman);
    print_str(" cpol="); print_int(cpol); print_str("\n");
    return cia.n_ind_the - 1;
}

int cia_cul_mkt(int mt_type, int cat, int cbra, int cpro, int cmap, int ccon, int year) {
    if (cia.n_cul_mkt >= MAX_CUL_MKT) return -1;
    cul_mkt_t* m = &cul_mkts[cia.n_cul_mkt];
    m->cm_id = cia.n_cul_mkt;
    m->cm_type = mt_type;
    m->cm_category = cat;
    m->cul_bra = cbra;
    m->cul_pro = cpro;
    m->cul_mar = cmap;
    m->cul_con = ccon;
    m->year = year;
    m->active = 1;
    cia.total_cul_bra += cbra;
    cia.n_cul_mkt++;
    print_str("[CIA] Cul mkt "); print_int(cia.n_cul_mkt - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cbra="); print_int(cbra);
    print_str(" cpro="); print_int(cpro);
    print_str(" cmap="); print_int(cmap);
    print_str(" ccon="); print_int(ccon); print_str("\n");
    return cia.n_cul_mkt - 1;
}

int cia_dig_cul(int dt_type, int cat, int dcon, int ncul, int dpub, int nmed, int year) {
    if (cia.n_dig_cul >= MAX_DIG_CUL) return -1;
    dig_cul_t* d = &dig_culs[cia.n_dig_cul];
    d->dc_id = cia.n_dig_cul;
    d->dc_type = dt_type;
    d->dc_category = cat;
    d->dig_con = dcon;
    d->net_cul = ncul;
    d->dig_pub = dpub;
    d->new_med = nmed;
    d->year = year;
    d->active = 1;
    cia.total_dig_con += dcon;
    cia.n_dig_cul++;
    print_str("[CIA] Dig cul "); print_int(cia.n_dig_cul - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dcon="); print_int(dcon);
    print_str(" ncul="); print_int(ncul);
    print_str(" dpub="); print_int(dpub);
    print_str(" nmed="); print_int(nmed); print_str("\n");
    return cia.n_dig_cul - 1;
}

int cia_cul_tra(int ct_type, int cat, int cpr2, int cser, int cexc, int cexp, int year) {
    if (cia.n_cul_tra >= MAX_CUL_TRA) return -1;
    cul_tra_t* t = &cul_tras[cia.n_cul_tra];
    t->ct_id = cia.n_cul_tra;
    t->ct_type = ct_type;
    t->ct_category = cat;
    t->cul_pro2 = cpr2;
    t->cul_ser = cser;
    t->cul_exc = cexc;
    t->cul_exp = cexp;
    t->year = year;
    t->active = 1;
    cia.total_cul_pro2 += cpr2;
    cia.n_cul_tra++;
    print_str("[CIA] Cul tra "); print_int(cia.n_cul_tra - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cpr2="); print_int(cpr2);
    print_str(" cser="); print_int(cser);
    print_str(" cexc="); print_int(cexc);
    print_str(" cexp="); print_int(cexp); print_str("\n");
    return cia.n_cul_tra - 1;
}

int cia_cre_ind(int ct_type2, int cat, int cds, int cpar, int ctal, int cinv, int year) {
    if (cia.n_cre_ind >= MAX_CRE_IND) return -1;
    cre_ind_t* c = &cre_inds[cia.n_cre_ind];
    c->ci_id = cia.n_cre_ind;
    c->ci_type = ct_type2;
    c->ci_category = cat;
    c->cre_des = cds;
    c->cre_par = cpar;
    c->cre_tal = ctal;
    c->cre_inv = cinv;
    c->year = year;
    c->active = 1;
    cia.total_cre_des += cds;
    cia.n_cre_ind++;
    print_str("[CIA] Cre ind "); print_int(cia.n_cre_ind - 1);
    print_str(" type="); print_int(ct_type2);
    print_str(" cat="); print_int(cat);
    print_str(" cds="); print_int(cds);
    print_str(" cpar="); print_int(cpar);
    print_str(" ctal="); print_int(ctal);
    print_str(" cinv="); print_int(cinv); print_str("\n");
    return cia.n_cre_ind - 1;
}

void cia_theory_report(void) {
    print_str("[CIA] Industry theory report:\n");
    print_str("  Theory categories: "); print_int(cia.n_ind_the); print_str("\n");
    print_str("  Total industry theory: "); print_int(cia.total_ind_the); print_str("\n");
}

void cia_marketing_report(void) {
    print_str("[CIA] Cultural marketing report:\n");
    print_str("  Marketing categories: "); print_int(cia.n_cul_mkt); print_str("\n");
    print_str("  Total cultural brands: "); print_int(cia.total_cul_bra); print_str("\n");
}

void cia_full_report(void) {
    print_str("[CIA] Full report:\n");
    print_str("  Digital culture categories: "); print_int(cia.n_dig_cul); print_str("\n");
    print_str("  Total digital content: "); print_int(cia.total_dig_con); print_str("\n");
    print_str("  Trade categories: "); print_int(cia.n_cul_tra); print_str("\n");
    print_str("  Total cultural products: "); print_int(cia.total_cul_pro2); print_str("\n");
    print_str("  Creative categories: "); print_int(cia.n_cre_ind); print_str("\n");
    print_str("  Total creative design: "); print_int(cia.total_cre_des); print_str("\n");
}

void cia_print_state(void) {
    print_str("[CIA] It="); print_int(cia.n_ind_the);
    print_str(" Cm="); print_int(cia.n_cul_mkt);
    print_str(" Dc="); print_int(cia.n_dig_cul);
    print_str(" Ct="); print_int(cia.n_cul_tra);
    print_str(" Ci="); print_int(cia.n_cre_ind);
    print_str("\n");
}

int main(void) {
    print_str("=== Cultural Industry Admin Demo ===\n\n");
    cia_init();

    print_str("Industry theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ithe = 55 + (i * 13);
        int cecon = 40 + (i * 10);
        int cman = 22 + (i * 5);
        int cpol = 15 + (i * 3);
        int year = 2020 + (i % 5);
        cia_ind_the(type, cat, ithe, cecon, cman, cpol, year);
    }

    print_str("\nCultural marketing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cbra = 48 + (i * 11);
        int cpro = 35 + (i * 8);
        int cmap = 20 + (i * 4);
        int ccon = 12 + (i * 3);
        int year = 2021 + (i % 4);
        cia_cul_mkt(type, cat, cbra, cpro, cmap, ccon, year);
    }

    print_str("\nDigital culture...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dcon = 42 + (i * 10);
        int ncul = 28 + (i * 7);
        int dpub = 18 + (i * 4);
        int nmed = 10 + (i * 2);
        int year = 2022 + (i % 3);
        cia_dig_cul(type, cat, dcon, ncul, dpub, nmed, year);
    }

    print_str("\nCultural trade...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cpr2 = 35 + (i * 8);
        int cser = 25 + (i * 6);
        int cexc = 15 + (i * 3);
        int cexp = 10 + (i * 2);
        int year = 2023 + (i % 2);
        cia_cul_tra(type, cat, cpr2, cser, cexc, cexp, year);
    }

    print_str("\nCreative industry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cds = 30 + (i * 7);
        int cpar = 22 + (i * 5);
        int ctal = 12 + (i * 3);
        int cinv = 8 + (i * 2);
        int year = 2024;
        cia_cre_ind(type, cat, cds, cpar, ctal, cinv, year);
    }

    print_str("\nTheory report...\n");
    cia_theory_report();

    print_str("\nMarketing report...\n");
    cia_marketing_report();

    print_str("\nFull report...\n");
    cia_full_report();

    print_str("\nFinal state...\n");
    cia_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
