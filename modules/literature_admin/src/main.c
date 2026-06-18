/* literature_admin: Literature administration system (v1.0)
 * Literary theory, Chinese literature, world literature, linguistics, creative writing
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

#define MAX_LIT_TH      16
#define MAX_CHI_LIT     14
#define MAX_WOR_LIT     12
#define MAX_LING        10
#define MAX_CRE_WRI     10

typedef struct {
    int    lt_id;
    int    lt_type;
    int    lt_category;
    int    lit_pri;
    int    lit_cri;
    int    comp_lit;
    int    lit_his;
    int    year;
    int    active;
} lit_th_t;

typedef struct {
    int    cl_id;
    int    cl_type;
    int    cl_category;
    int    anc_lit;
    int    mod_lit;
    int    con_lit;
    int    folk_lit;
    int    year;
    int    active;
} chi_lit_t;

typedef struct {
    int    wl_id;
    int    wl_type;
    int    wl_category;
    int    anglo_lit;
    int    euro_lit;
    int    asia_lit;
    int    latam_lit;
    int    year;
    int    active;
} wor_lit_t;

typedef struct {
    int    li_id;
    int    li_type;
    int    li_category;
    int    lin_th;
    int    app_lin;
    int    transl;
    int    comp_lin;
    int    year;
    int    active;
} ling_t;

typedef struct {
    int    cw_id;
    int    cw_type;
    int    cw_category;
    int    novel;
    int    poetry;
    int    prose;
    int    screen;
    int    year;
    int    active;
} cre_wri_t;

typedef struct {
    int    n_lit_th;
    int    n_chi_lit;
    int    n_wor_lit;
    int    n_ling;
    int    n_cre_wri;
    int    total_lit_pri;
    int    total_anc_lit;
    int    total_anglo_lit;
    int    total_lin_th;
    int    total_novel;
} lta_state_t;

static lit_th_t lit_ths[MAX_LIT_TH];
static chi_lit_t chi_lits[MAX_CHI_LIT];
static wor_lit_t wor_lits[MAX_WOR_LIT];
static ling_t lings[MAX_LING];
static cre_wri_t cre_wris[MAX_CRE_WRI];
static lta_state_t lta;

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

int lta_init(void) {
    if (initialized) return -1;
    lta.n_lit_th = 0; lta.n_chi_lit = 0; lta.n_wor_lit = 0;
    lta.n_ling = 0; lta.n_cre_wri = 0;
    lta.total_lit_pri = 0; lta.total_anc_lit = 0;
    lta.total_anglo_lit = 0; lta.total_lin_th = 0;
    lta.total_novel = 0;
    for (int i = 0; i < MAX_LIT_TH; i++) lit_ths[i].active = 0;
    for (int i = 0; i < MAX_CHI_LIT; i++) chi_lits[i].active = 0;
    for (int i = 0; i < MAX_WOR_LIT; i++) wor_lits[i].active = 0;
    for (int i = 0; i < MAX_LING; i++) lings[i].active = 0;
    for (int i = 0; i < MAX_CRE_WRI; i++) cre_wris[i].active = 0;
    initialized = 1;
    print_str("[LTA] Literature initialized\n");
    return 0;
}

int lta_lit_th(int lt_type, int cat, int lpr, int lcr, int clt, int lhs, int year) {
    if (lta.n_lit_th >= MAX_LIT_TH) return -1;
    lit_th_t* l = &lit_ths[lta.n_lit_th];
    l->lt_id = lta.n_lit_th;
    l->lt_type = lt_type;
    l->lt_category = cat;
    l->lit_pri = lpr;
    l->lit_cri = lcr;
    l->comp_lit = clt;
    l->lit_his = lhs;
    l->year = year;
    l->active = 1;
    lta.total_lit_pri += lpr;
    lta.n_lit_th++;
    print_str("[LTA] Lit th "); print_int(lta.n_lit_th - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lpr="); print_int(lpr);
    print_str(" lcr="); print_int(lcr);
    print_str(" clt="); print_int(clt);
    print_str(" lhs="); print_int(lhs); print_str("\n");
    return lta.n_lit_th - 1;
}

int lta_chi_lit(int ct_type, int cat, int acl, int mdl, int cll, int fll, int year) {
    if (lta.n_chi_lit >= MAX_CHI_LIT) return -1;
    chi_lit_t* c = &chi_lits[lta.n_chi_lit];
    c->cl_id = lta.n_chi_lit;
    c->cl_type = ct_type;
    c->cl_category = cat;
    c->anc_lit = acl;
    c->mod_lit = mdl;
    c->con_lit = cll;
    c->folk_lit = fll;
    c->year = year;
    c->active = 1;
    lta.total_anc_lit += acl;
    lta.n_chi_lit++;
    print_str("[LTA] Chi lit "); print_int(lta.n_chi_lit - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" acl="); print_int(acl);
    print_str(" mdl="); print_int(mdl);
    print_str(" cll="); print_int(cll);
    print_str(" fll="); print_int(fll); print_str("\n");
    return lta.n_chi_lit - 1;
}

int lta_wor_lit(int wt_type, int cat, int agl, int eul, int asl, int ltl, int year) {
    if (lta.n_wor_lit >= MAX_WOR_LIT) return -1;
    wor_lit_t* w = &wor_lits[lta.n_wor_lit];
    w->wl_id = lta.n_wor_lit;
    w->wl_type = wt_type;
    w->wl_category = cat;
    w->anglo_lit = agl;
    w->euro_lit = eul;
    w->asia_lit = asl;
    w->latam_lit = ltl;
    w->year = year;
    w->active = 1;
    lta.total_anglo_lit += agl;
    lta.n_wor_lit++;
    print_str("[LTA] Wor lit "); print_int(lta.n_wor_lit - 1);
    print_str(" type="); print_int(wt_type);
    print_str(" cat="); print_int(cat);
    print_str(" agl="); print_int(agl);
    print_str(" eul="); print_int(eul);
    print_str(" asl="); print_int(asl);
    print_str(" ltl="); print_int(ltl); print_str("\n");
    return lta.n_wor_lit - 1;
}

int lta_ling(int lt_type, int cat, int lth, int apl, int trs, int cpl, int year) {
    if (lta.n_ling >= MAX_LING) return -1;
    ling_t* l = &lings[lta.n_ling];
    l->li_id = lta.n_ling;
    l->li_type = lt_type;
    l->li_category = cat;
    l->lin_th = lth;
    l->app_lin = apl;
    l->transl = trs;
    l->comp_lin = cpl;
    l->year = year;
    l->active = 1;
    lta.total_lin_th += lth;
    lta.n_ling++;
    print_str("[LTA] Ling "); print_int(lta.n_ling - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" lth="); print_int(lth);
    print_str(" apl="); print_int(apl);
    print_str(" trs="); print_int(trs);
    print_str(" cpl="); print_int(cpl); print_str("\n");
    return lta.n_ling - 1;
}

int lta_cre_wri(int ct_type, int cat, int nvl, int ptr, int prs, int scr, int year) {
    if (lta.n_cre_wri >= MAX_CRE_WRI) return -1;
    cre_wri_t* c = &cre_wris[lta.n_cre_wri];
    c->cw_id = lta.n_cre_wri;
    c->cw_type = ct_type;
    c->cw_category = cat;
    c->novel = nvl;
    c->poetry = ptr;
    c->prose = prs;
    c->screen = scr;
    c->year = year;
    c->active = 1;
    lta.total_novel += nvl;
    lta.n_cre_wri++;
    print_str("[LTA] Cre wri "); print_int(lta.n_cre_wri - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" nvl="); print_int(nvl);
    print_str(" ptr="); print_int(ptr);
    print_str(" prs="); print_int(prs);
    print_str(" scr="); print_int(scr); print_str("\n");
    return lta.n_cre_wri - 1;
}

void lta_theory_report(void) {
    print_str("[LTA] Literary theory report:\n");
    print_str("  Theory categories: "); print_int(lta.n_lit_th); print_str("\n");
    print_str("  Total literary principles: "); print_int(lta.total_lit_pri); print_str("\n");
}

void lta_chi_report(void) {
    print_str("[LTA] Chinese literature report:\n");
    print_str("  Chinese literature categories: "); print_int(lta.n_chi_lit); print_str("\n");
    print_str("  Total ancient literature: "); print_int(lta.total_anc_lit); print_str("\n");
}

void lta_full_report(void) {
    print_str("[LTA] Full report:\n");
    print_str("  World literature categories: "); print_int(lta.n_wor_lit); print_str("\n");
    print_str("  Total Anglo literature: "); print_int(lta.total_anglo_lit); print_str("\n");
    print_str("  Linguistics categories: "); print_int(lta.n_ling); print_str("\n");
    print_str("  Total linguistics theory: "); print_int(lta.total_lin_th); print_str("\n");
    print_str("  Creative writing categories: "); print_int(lta.n_cre_wri); print_str("\n");
    print_str("  Total novels: "); print_int(lta.total_novel); print_str("\n");
}

void lta_print_state(void) {
    print_str("[LTA] Lt="); print_int(lta.n_lit_th);
    print_str(" Cl="); print_int(lta.n_chi_lit);
    print_str(" Wl="); print_int(lta.n_wor_lit);
    print_str(" Lg="); print_int(lta.n_ling);
    print_str(" Cw="); print_int(lta.n_cre_wri);
    print_str("\n");
}

int main(void) {
    print_str("=== Literature Admin Demo ===\n\n");
    lta_init();

    print_str("Literary theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int lpr = 55 + (i * 13);
        int lcr = 40 + (i * 10);
        int clt = 22 + (i * 5);
        int lhs = 15 + (i * 3);
        int year = 2020 + (i % 5);
        lta_lit_th(type, cat, lpr, lcr, clt, lhs, year);
    }

    print_str("\nChinese literature...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int acl = 48 + (i * 11);
        int mdl = 35 + (i * 8);
        int cll = 20 + (i * 4);
        int fll = 12 + (i * 3);
        int year = 2021 + (i % 4);
        lta_chi_lit(type, cat, acl, mdl, cll, fll, year);
    }

    print_str("\nWorld literature...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int agl = 42 + (i * 10);
        int eul = 28 + (i * 7);
        int asl = 18 + (i * 4);
        int ltl = 10 + (i * 2);
        int year = 2022 + (i % 3);
        lta_wor_lit(type, cat, agl, eul, asl, ltl, year);
    }

    print_str("\nLinguistics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lth = 35 + (i * 8);
        int apl = 25 + (i * 6);
        int trs = 15 + (i * 3);
        int cpl = 10 + (i * 2);
        int year = 2023 + (i % 2);
        lta_ling(type, cat, lth, apl, trs, cpl, year);
    }

    print_str("\nCreative writing...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int nvl = 30 + (i * 7);
        int ptr = 22 + (i * 5);
        int prs = 12 + (i * 3);
        int scr = 8 + (i * 2);
        int year = 2024;
        lta_cre_wri(type, cat, nvl, ptr, prs, scr, year);
    }

    print_str("\nTheory report...\n");
    lta_theory_report();

    print_str("\nChinese report...\n");
    lta_chi_report();

    print_str("\nFull report...\n");
    lta_full_report();

    print_str("\nFinal state...\n");
    lta_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
