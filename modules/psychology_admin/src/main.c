/* psychology_admin: Psychology administration system (v1.0)
 * Basic psychology, clinical psychology, educational, industrial, social psychology
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

#define MAX_BASIC_P     16
#define MAX_CLINIC_P    14
#define MAX_EDUC_P      12
#define MAX_INDUS_P     10
#define MAX_SOCIAL_P    10

typedef struct {
    int    bp_id;
    int    bp_type;
    int    bp_category;
    int    cog_psy;
    int    dev_psy;
    int    soc_psy;
    int    per_psy;
    int    year;
    int    active;
} basic_p_t;

typedef struct {
    int    cp_id;
    int    cp_type;
    int    cp_category;
    int    psy_assess;
    int    psy_diag;
    int    psy_ther;
    int    psy_consult;
    int    year;
    int    active;
} clinic_p_t;

typedef struct {
    int    ep_id;
    int    ep_type;
    int    ep_category;
    int    lrn_psy;
    int    tch_psy;
    int    stu_dev;
    int    edu_meas;
    int    year;
    int    active;
} educ_p_t;

typedef struct {
    int    ip_id;
    int    ip_type;
    int    ip_category;
    int    hr_psy;
    int    eng_psy;
    int    org_psy;
    int    con_psy;
    int    year;
    int    active;
} indust_p_t;

typedef struct {
    int    sp_id;
    int    sp_type;
    int    sp_category;
    int    grp_psy;
    int    attr_psy;
    int    soc_cog;
    int    att_chg;
    int    year;
    int    active;
} social_p_t;

typedef struct {
    int    n_basic_p;
    int    n_clinic_p;
    int    n_educ_p;
    int    n_indust_p;
    int    n_social_p;
    int    total_cog_psy;
    int    total_psy_assess;
    int    total_lrn_psy;
    int    total_hr_psy;
    int    total_grp_psy;
} psa_state_t;

static basic_p_t basic_ps[MAX_BASIC_P];
static clinic_p_t clinic_ps[MAX_CLINIC_P];
static educ_p_t educ_ps[MAX_EDUC_P];
static indust_p_t indust_ps[MAX_INDUS_P];
static social_p_t social_ps[MAX_SOCIAL_P];
static psa_state_t psa;

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

int psa_init(void) {
    if (initialized) return -1;
    psa.n_basic_p = 0; psa.n_clinic_p = 0; psa.n_educ_p = 0;
    psa.n_indust_p = 0; psa.n_social_p = 0;
    psa.total_cog_psy = 0; psa.total_psy_assess = 0;
    psa.total_lrn_psy = 0; psa.total_hr_psy = 0;
    psa.total_grp_psy = 0;
    for (int i = 0; i < MAX_BASIC_P; i++) basic_ps[i].active = 0;
    for (int i = 0; i < MAX_CLINIC_P; i++) clinic_ps[i].active = 0;
    for (int i = 0; i < MAX_EDUC_P; i++) educ_ps[i].active = 0;
    for (int i = 0; i < MAX_INDUS_P; i++) indust_ps[i].active = 0;
    for (int i = 0; i < MAX_SOCIAL_P; i++) social_ps[i].active = 0;
    initialized = 1;
    print_str("[PSA] Psychology initialized\n");
    return 0;
}

int psa_basic_p(int bt_type, int cat, int cgx, int dvx, int scx, int prx, int year) {
    if (psa.n_basic_p >= MAX_BASIC_P) return -1;
    basic_p_t* b = &basic_ps[psa.n_basic_p];
    b->bp_id = psa.n_basic_p;
    b->bp_type = bt_type;
    b->bp_category = cat;
    b->cog_psy = cgx;
    b->dev_psy = dvx;
    b->soc_psy = scx;
    b->per_psy = prx;
    b->year = year;
    b->active = 1;
    psa.total_cog_psy += cgx;
    psa.n_basic_p++;
    print_str("[PSA] Basic p "); print_int(psa.n_basic_p - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" cgx="); print_int(cgx);
    print_str(" dvx="); print_int(dvx);
    print_str(" scx="); print_int(scx);
    print_str(" prx="); print_int(prx); print_str("\n");
    return psa.n_basic_p - 1;
}

int psa_clinic_p(int ct_type, int cat, int pas, int pdg, int pth, int pct, int year) {
    if (psa.n_clinic_p >= MAX_CLINIC_P) return -1;
    clinic_p_t* c = &clinic_ps[psa.n_clinic_p];
    c->cp_id = psa.n_clinic_p;
    c->cp_type = ct_type;
    c->cp_category = cat;
    c->psy_assess = pas;
    c->psy_diag = pdg;
    c->psy_ther = pth;
    c->psy_consult = pct;
    c->year = year;
    c->active = 1;
    psa.total_psy_assess += pas;
    psa.n_clinic_p++;
    print_str("[PSA] Clinic p "); print_int(psa.n_clinic_p - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" pas="); print_int(pas);
    print_str(" pdg="); print_int(pdg);
    print_str(" pth="); print_int(pth);
    print_str(" pct="); print_int(pct); print_str("\n");
    return psa.n_clinic_p - 1;
}

int psa_educ_p(int et_type, int cat, int lrx, int tcx, int std, int edn, int year) {
    if (psa.n_educ_p >= MAX_EDUC_P) return -1;
    educ_p_t* e = &educ_ps[psa.n_educ_p];
    e->ep_id = psa.n_educ_p;
    e->ep_type = et_type;
    e->ep_category = cat;
    e->lrn_psy = lrx;
    e->tch_psy = tcx;
    e->stu_dev = std;
    e->edu_meas = edn;
    e->year = year;
    e->active = 1;
    psa.total_lrn_psy += lrx;
    psa.n_educ_p++;
    print_str("[PSA] Educ p "); print_int(psa.n_educ_p - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" lrx="); print_int(lrx);
    print_str(" tcx="); print_int(tcx);
    print_str(" std="); print_int(std);
    print_str(" edn="); print_int(edn); print_str("\n");
    return psa.n_educ_p - 1;
}

int psa_indust_p(int it_type, int cat, int hrx, int egx, int orx, int cnx, int year) {
    if (psa.n_indust_p >= MAX_INDUS_P) return -1;
    indust_p_t* n = &indust_ps[psa.n_indust_p];
    n->ip_id = psa.n_indust_p;
    n->ip_type = it_type;
    n->ip_category = cat;
    n->hr_psy = hrx;
    n->eng_psy = egx;
    n->org_psy = orx;
    n->con_psy = cnx;
    n->year = year;
    n->active = 1;
    psa.total_hr_psy += hrx;
    psa.n_indust_p++;
    print_str("[PSA] Indust p "); print_int(psa.n_indust_p - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" hrx="); print_int(hrx);
    print_str(" egx="); print_int(egx);
    print_str(" orx="); print_int(orx);
    print_str(" cnx="); print_int(cnx); print_str("\n");
    return psa.n_indust_p - 1;
}

int psa_social_p(int st_type, int cat, int grx, int atr, int scg, int ach, int year) {
    if (psa.n_social_p >= MAX_SOCIAL_P) return -1;
    social_p_t* s = &social_ps[psa.n_social_p];
    s->sp_id = psa.n_social_p;
    s->sp_type = st_type;
    s->sp_category = cat;
    s->grp_psy = grx;
    s->attr_psy = atr;
    s->soc_cog = scg;
    s->att_chg = ach;
    s->year = year;
    s->active = 1;
    psa.total_grp_psy += grx;
    psa.n_social_p++;
    print_str("[PSA] Social p "); print_int(psa.n_social_p - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" grx="); print_int(grx);
    print_str(" atr="); print_int(atr);
    print_str(" scg="); print_int(scg);
    print_str(" ach="); print_int(ach); print_str("\n");
    return psa.n_social_p - 1;
}

void psa_basic_report(void) {
    print_str("[PSA] Basic psychology report:\n");
    print_str("  Basic categories: "); print_int(psa.n_basic_p); print_str("\n");
    print_str("  Total cognitive psychology: "); print_int(psa.total_cog_psy); print_str("\n");
}

void psa_clinic_report(void) {
    print_str("[PSA] Clinical psychology report:\n");
    print_str("  Clinical categories: "); print_int(psa.n_clinic_p); print_str("\n");
    print_str("  Total psychology assessment: "); print_int(psa.total_psy_assess); print_str("\n");
}

void psa_full_report(void) {
    print_str("[PSA] Full report:\n");
    print_str("  Educational categories: "); print_int(psa.n_educ_p); print_str("\n");
    print_str("  Total learning psychology: "); print_int(psa.total_lrn_psy); print_str("\n");
    print_str("  Industrial categories: "); print_int(psa.n_indust_p); print_str("\n");
    print_str("  Total HR psychology: "); print_int(psa.total_hr_psy); print_str("\n");
    print_str("  Social categories: "); print_int(psa.n_social_p); print_str("\n");
    print_str("  Total group psychology: "); print_int(psa.total_grp_psy); print_str("\n");
}

void psa_print_state(void) {
    print_str("[PSA] Bp="); print_int(psa.n_basic_p);
    print_str(" Cp="); print_int(psa.n_clinic_p);
    print_str(" Ep="); print_int(psa.n_educ_p);
    print_str(" Ip="); print_int(psa.n_indust_p);
    print_str(" Sp="); print_int(psa.n_social_p);
    print_str("\n");
}

int main(void) {
    print_str("=== Psychology Admin Demo ===\n\n");
    psa_init();

    print_str("Basic psychology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cgx = 55 + (i * 13);
        int dvx = 40 + (i * 10);
        int scx = 22 + (i * 5);
        int prx = 15 + (i * 3);
        int year = 2020 + (i % 5);
        psa_basic_p(type, cat, cgx, dvx, scx, prx, year);
    }

    print_str("\nClinical psychology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pas = 48 + (i * 11);
        int pdg = 35 + (i * 8);
        int pth = 20 + (i * 4);
        int pct = 12 + (i * 3);
        int year = 2021 + (i % 4);
        psa_clinic_p(type, cat, pas, pdg, pth, pct, year);
    }

    print_str("\nEducational psychology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int lrx = 42 + (i * 10);
        int tcx = 28 + (i * 7);
        int std = 18 + (i * 4);
        int edn = 10 + (i * 2);
        int year = 2022 + (i % 3);
        psa_educ_p(type, cat, lrx, tcx, std, edn, year);
    }

    print_str("\nIndustrial psychology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hrx = 35 + (i * 8);
        int egx = 25 + (i * 6);
        int orx = 15 + (i * 3);
        int cnx = 10 + (i * 2);
        int year = 2023 + (i % 2);
        psa_indust_p(type, cat, hrx, egx, orx, cnx, year);
    }

    print_str("\nSocial psychology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int grx = 30 + (i * 7);
        int atr = 22 + (i * 5);
        int scg = 12 + (i * 3);
        int ach = 8 + (i * 2);
        int year = 2024;
        psa_social_p(type, cat, grx, atr, scg, ach, year);
    }

    print_str("\nBasic report...\n");
    psa_basic_report();

    print_str("\nClinical report...\n");
    psa_clinic_report();

    print_str("\nFull report...\n");
    psa_full_report();

    print_str("\nFinal state...\n");
    psa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
