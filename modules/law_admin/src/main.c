/* law_admin: Law administration system (v1.0)
 * Legal theory, constitutional/admin law, civil/commercial, criminal, international
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

#define MAX_LEGAL_TH    16
#define MAX_CONST_AL    14
#define MAX_CIVIL_CL    12
#define MAX_CRIM_LAW    10
#define MAX_INTL_LAW    10

typedef struct {
    int    lt_id;
    int    lt_type;
    int    lt_category;
    int    jurispru;
    int    legal_hist;
    int    comp_law;
    int    legal_phil;
    int    year;
    int    active;
} legal_th_t;

typedef struct {
    int    ca_id;
    int    ca_type;
    int    ca_category;
    int    const_law;
    int    admin_law;
    int    admin_lit;
    int    state_comp;
    int    year;
    int    active;
} const_al_t;

typedef struct {
    int    cc_id;
    int    cc_type;
    int    cc_category;
    int    civil_law;
    int    comm_law;
    int    ip_law;
    int    contract_law;
    int    year;
    int    active;
} civil_cl_t;

typedef struct {
    int    cl_id;
    int    cl_type;
    int    cl_category;
    int    crim_law;
    int    criminol;
    int    crim_proc;
    int    penalty_exec;
    int    year;
    int    active;
} crim_law_t;

typedef struct {
    int    il_id;
    int    il_type;
    int    il_category;
    int    intl_pub;
    int    intl_pri;
    int    intl_econ;
    int    wto_law;
    int    year;
    int    active;
} intl_law_t;

typedef struct {
    int    n_legal_th;
    int    n_const_al;
    int    n_civil_cl;
    int    n_crim_law;
    int    n_intl_law;
    int    total_jurispru;
    int    total_const_law;
    int    total_civil_law;
    int    total_crim_law;
    int    total_intl_pub;
} la_state_t;

static legal_th_t legal_ths[MAX_LEGAL_TH];
static const_al_t const_als[MAX_CONST_AL];
static civil_cl_t civil_cls[MAX_CIVIL_CL];
static crim_law_t crim_laws[MAX_CRIM_LAW];
static intl_law_t intl_laws[MAX_INTL_LAW];
static la_state_t la;

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

int la_init(void) {
    if (initialized) return -1;
    la.n_legal_th = 0; la.n_const_al = 0; la.n_civil_cl = 0;
    la.n_crim_law = 0; la.n_intl_law = 0;
    la.total_jurispru = 0; la.total_const_law = 0;
    la.total_civil_law = 0; la.total_crim_law = 0;
    la.total_intl_pub = 0;
    for (int i = 0; i < MAX_LEGAL_TH; i++) legal_ths[i].active = 0;
    for (int i = 0; i < MAX_CONST_AL; i++) const_als[i].active = 0;
    for (int i = 0; i < MAX_CIVIL_CL; i++) civil_cls[i].active = 0;
    for (int i = 0; i < MAX_CRIM_LAW; i++) crim_laws[i].active = 0;
    for (int i = 0; i < MAX_INTL_LAW; i++) intl_laws[i].active = 0;
    initialized = 1;
    print_str("[LA] Law initialized\n");
    return 0;
}

int la_legal_th(int lt_type, int cat, int jrp, int lgh, int cpl, int lgp, int year) {
    if (la.n_legal_th >= MAX_LEGAL_TH) return -1;
    legal_th_t* l = &legal_ths[la.n_legal_th];
    l->lt_id = la.n_legal_th;
    l->lt_type = lt_type;
    l->lt_category = cat;
    l->jurispru = jrp;
    l->legal_hist = lgh;
    l->comp_law = cpl;
    l->legal_phil = lgp;
    l->year = year;
    l->active = 1;
    la.total_jurispru += jrp;
    la.n_legal_th++;
    print_str("[LA] Legal th "); print_int(la.n_legal_th - 1);
    print_str(" type="); print_int(lt_type);
    print_str(" cat="); print_int(cat);
    print_str(" jrp="); print_int(jrp);
    print_str(" lgh="); print_int(lgh);
    print_str(" cpl="); print_int(cpl);
    print_str(" lgp="); print_int(lgp); print_str("\n");
    return la.n_legal_th - 1;
}

int la_const_al(int ct_type, int cat, int cnl, int adl, int adt, int stc, int year) {
    if (la.n_const_al >= MAX_CONST_AL) return -1;
    const_al_t* c = &const_als[la.n_const_al];
    c->ca_id = la.n_const_al;
    c->ca_type = ct_type;
    c->ca_category = cat;
    c->const_law = cnl;
    c->admin_law = adl;
    c->admin_lit = adt;
    c->state_comp = stc;
    c->year = year;
    c->active = 1;
    la.total_const_law += cnl;
    la.n_const_al++;
    print_str("[LA] Const al "); print_int(la.n_const_al - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cnl="); print_int(cnl);
    print_str(" adl="); print_int(adl);
    print_str(" adt="); print_int(adt);
    print_str(" stc="); print_int(stc); print_str("\n");
    return la.n_const_al - 1;
}

int la_civil_cl(int cv_type, int cat, int cvl, int cml, int ipl, int ctt, int year) {
    if (la.n_civil_cl >= MAX_CIVIL_CL) return -1;
    civil_cl_t* c = &civil_cls[la.n_civil_cl];
    c->cc_id = la.n_civil_cl;
    c->cc_type = cv_type;
    c->cc_category = cat;
    c->civil_law = cvl;
    c->comm_law = cml;
    c->ip_law = ipl;
    c->contract_law = ctt;
    c->year = year;
    c->active = 1;
    la.total_civil_law += cvl;
    la.n_civil_cl++;
    print_str("[LA] Civil cl "); print_int(la.n_civil_cl - 1);
    print_str(" type="); print_int(cv_type);
    print_str(" cat="); print_int(cat);
    print_str(" cvl="); print_int(cvl);
    print_str(" cml="); print_int(cml);
    print_str(" ipl="); print_int(ipl);
    print_str(" ctt="); print_int(ctt); print_str("\n");
    return la.n_civil_cl - 1;
}

int la_crim_law(int cr_type, int cat, int crl, int crm, int crp, int pex, int year) {
    if (la.n_crim_law >= MAX_CRIM_LAW) return -1;
    crim_law_t* c = &crim_laws[la.n_crim_law];
    c->cl_id = la.n_crim_law;
    c->cl_type = cr_type;
    c->cl_category = cat;
    c->crim_law = crl;
    c->criminol = crm;
    c->crim_proc = crp;
    c->penalty_exec = pex;
    c->year = year;
    c->active = 1;
    la.total_crim_law += crl;
    la.n_crim_law++;
    print_str("[LA] Crim law "); print_int(la.n_crim_law - 1);
    print_str(" type="); print_int(cr_type);
    print_str(" cat="); print_int(cat);
    print_str(" crl="); print_int(crl);
    print_str(" crm="); print_int(crm);
    print_str(" crp="); print_int(crp);
    print_str(" pex="); print_int(pex); print_str("\n");
    return la.n_crim_law - 1;
}

int la_intl_law(int it_type, int cat, int ipu, int ipr, int iec, int wtl, int year) {
    if (la.n_intl_law >= MAX_INTL_LAW) return -1;
    intl_law_t* n = &intl_laws[la.n_intl_law];
    n->il_id = la.n_intl_law;
    n->il_type = it_type;
    n->il_category = cat;
    n->intl_pub = ipu;
    n->intl_pri = ipr;
    n->intl_econ = iec;
    n->wto_law = wtl;
    n->year = year;
    n->active = 1;
    la.total_intl_pub += ipu;
    la.n_intl_law++;
    print_str("[LA] Intl law "); print_int(la.n_intl_law - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ipu="); print_int(ipu);
    print_str(" ipr="); print_int(ipr);
    print_str(" iec="); print_int(iec);
    print_str(" wtl="); print_int(wtl); print_str("\n");
    return la.n_intl_law - 1;
}

void la_theory_report(void) {
    print_str("[LA] Legal theory report:\n");
    print_str("  Theory categories: "); print_int(la.n_legal_th); print_str("\n");
    print_str("  Total jurisprudence: "); print_int(la.total_jurispru); print_str("\n");
}

void la_const_report(void) {
    print_str("[LA] Constitutional law report:\n");
    print_str("  Constitutional categories: "); print_int(la.n_const_al); print_str("\n");
    print_str("  Total constitutional law: "); print_int(la.total_const_law); print_str("\n");
}

void la_full_report(void) {
    print_str("[LA] Full report:\n");
    print_str("  Civil/commercial categories: "); print_int(la.n_civil_cl); print_str("\n");
    print_str("  Total civil law: "); print_int(la.total_civil_law); print_str("\n");
    print_str("  Criminal categories: "); print_int(la.n_crim_law); print_str("\n");
    print_str("  Total criminal law: "); print_int(la.total_crim_law); print_str("\n");
    print_str("  International categories: "); print_int(la.n_intl_law); print_str("\n");
    print_str("  Total international public: "); print_int(la.total_intl_pub); print_str("\n");
}

void la_print_state(void) {
    print_str("[LA] Th="); print_int(la.n_legal_th);
    print_str(" Ca="); print_int(la.n_const_al);
    print_str(" Cv="); print_int(la.n_civil_cl);
    print_str(" Cr="); print_int(la.n_crim_law);
    print_str(" Il="); print_int(la.n_intl_law);
    print_str("\n");
}

int main(void) {
    print_str("=== Law Admin Demo ===\n\n");
    la_init();

    print_str("Legal theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int jrp = 55 + (i * 13);
        int lgh = 40 + (i * 10);
        int cpl = 22 + (i * 5);
        int lgp = 15 + (i * 3);
        int year = 2020 + (i % 5);
        la_legal_th(type, cat, jrp, lgh, cpl, lgp, year);
    }

    print_str("\nConstitutional/admin...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cnl = 48 + (i * 11);
        int adl = 35 + (i * 8);
        int adt = 20 + (i * 4);
        int stc = 12 + (i * 3);
        int year = 2021 + (i % 4);
        la_const_al(type, cat, cnl, adl, adt, stc, year);
    }

    print_str("\nCivil/commercial...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cvl = 42 + (i * 10);
        int cml = 28 + (i * 7);
        int ipl = 18 + (i * 4);
        int ctt = 10 + (i * 2);
        int year = 2022 + (i % 3);
        la_civil_cl(type, cat, cvl, cml, ipl, ctt, year);
    }

    print_str("\nCriminal law...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int crl = 35 + (i * 8);
        int crm = 25 + (i * 6);
        int crp = 15 + (i * 3);
        int pex = 10 + (i * 2);
        int year = 2023 + (i % 2);
        la_crim_law(type, cat, crl, crm, crp, pex, year);
    }

    print_str("\nInternational law...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ipu = 30 + (i * 7);
        int ipr = 22 + (i * 5);
        int iec = 12 + (i * 3);
        int wtl = 8 + (i * 2);
        int year = 2024;
        la_intl_law(type, cat, ipu, ipr, iec, wtl, year);
    }

    print_str("\nTheory report...\n");
    la_theory_report();

    print_str("\nConstitutional report...\n");
    la_const_report();

    print_str("\nFull report...\n");
    la_full_report();

    print_str("\nFinal state...\n");
    la_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
