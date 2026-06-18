/* control_science_admin: Control science administration system (v1.0)
 * Control theory, pattern recognition, navigation guidance, systems engineering, intelligent control
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

#define MAX_CTRL_TH     16
#define MAX_PAT_RC      14
#define MAX_NAV_GD      12
#define MAX_SYS_EN      10
#define MAX_INTL_CTRL   10

typedef struct {
    int    ct_id;
    int    ct_type;
    int    ct_category;
    int    classic_ctrl;
    int    modern_ctrl;
    int    robust_ctrl;
    int    year;
    int    active;
} ctrl_th_t;

typedef struct {
    int    pr_id;
    int    pr_type;
    int    pr_category;
    int    img_recog;
    int    voice_recog;
    int    char_recog;
    int    year;
    int    active;
} pat_rc_t;

typedef struct {
    int    ng_id;
    int    ng_type;
    int    ng_category;
    int    inertial_nav;
    int    sat_nav;
    int    comb_nav;
    int    year;
    int    active;
} nav_gd_t;

typedef struct {
    int    se_id;
    int    se_type;
    int    se_category;
    int    sys_model;
    int    sys_optim;
    int    sys_sim;
    int    year;
    int    active;
} sys_en_t;

typedef struct {
    int    ic_id;
    int    ic_type;
    int    ic_category;
    int    fuzzy_ctrl;
    int    neural_ctrl;
    int    genetic_algo;
    int    year;
    int    active;
} intl_ctrl_t;

typedef struct {
    int    n_ctrl_th;
    int    n_pat_rc;
    int    n_nav_gd;
    int    n_sys_en;
    int    n_intl_ctrl;
    int    total_classic_ctrl;
    int    total_img_recog;
    int    total_inertial_nav;
    int    total_sys_model;
    int    total_fuzzy_ctrl;
} csa_state_t;

static ctrl_th_t ctrl_ths[MAX_CTRL_TH];
static pat_rc_t pat_rcs[MAX_PAT_RC];
static nav_gd_t nav_gds[MAX_NAV_GD];
static sys_en_t sys_ens[MAX_SYS_EN];
static intl_ctrl_t intl_ctrls[MAX_INTL_CTRL];
static csa_state_t csa;

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

int csa_init(void) {
    if (initialized) return -1;
    csa.n_ctrl_th = 0; csa.n_pat_rc = 0; csa.n_nav_gd = 0;
    csa.n_sys_en = 0; csa.n_intl_ctrl = 0;
    csa.total_classic_ctrl = 0; csa.total_img_recog = 0;
    csa.total_inertial_nav = 0; csa.total_sys_model = 0;
    csa.total_fuzzy_ctrl = 0;
    for (int i = 0; i < MAX_CTRL_TH; i++) ctrl_ths[i].active = 0;
    for (int i = 0; i < MAX_PAT_RC; i++) pat_rcs[i].active = 0;
    for (int i = 0; i < MAX_NAV_GD; i++) nav_gds[i].active = 0;
    for (int i = 0; i < MAX_SYS_EN; i++) sys_ens[i].active = 0;
    for (int i = 0; i < MAX_INTL_CTRL; i++) intl_ctrls[i].active = 0;
    initialized = 1;
    print_str("[CSA] Control science initialized\n");
    return 0;
}

int csa_ctrl_th(int ct_type, int cat, int cls, int mdn, int rbt, int year) {
    if (csa.n_ctrl_th >= MAX_CTRL_TH) return -1;
    ctrl_th_t* c = &ctrl_ths[csa.n_ctrl_th];
    c->ct_id = csa.n_ctrl_th;
    c->ct_type = ct_type;
    c->ct_category = cat;
    c->classic_ctrl = cls;
    c->modern_ctrl = mdn;
    c->robust_ctrl = rbt;
    c->year = year;
    c->active = 1;
    csa.total_classic_ctrl += cls;
    csa.n_ctrl_th++;
    print_str("[CSA] Ctrl th "); print_int(csa.n_ctrl_th - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cls="); print_int(cls);
    print_str(" mdn="); print_int(mdn);
    print_str(" rbt="); print_int(rbt); print_str("\n");
    return csa.n_ctrl_th - 1;
}

int csa_pat_rc(int pt_type, int cat, int imr, int vcr, int chr, int year) {
    if (csa.n_pat_rc >= MAX_PAT_RC) return -1;
    pat_rc_t* p = &pat_rcs[csa.n_pat_rc];
    p->pr_id = csa.n_pat_rc;
    p->pr_type = pt_type;
    p->pr_category = cat;
    p->img_recog = imr;
    p->voice_recog = vcr;
    p->char_recog = chr;
    p->year = year;
    p->active = 1;
    csa.total_img_recog += imr;
    csa.n_pat_rc++;
    print_str("[CSA] Pat rc "); print_int(csa.n_pat_rc - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" imr="); print_int(imr);
    print_str(" vcr="); print_int(vcr);
    print_str(" chr="); print_int(chr); print_str("\n");
    return csa.n_pat_rc - 1;
}

int csa_nav_gd(int nt_type, int cat, int ins, int sat, int cmb, int year) {
    if (csa.n_nav_gd >= MAX_NAV_GD) return -1;
    nav_gd_t* n = &nav_gds[csa.n_nav_gd];
    n->ng_id = csa.n_nav_gd;
    n->ng_type = nt_type;
    n->ng_category = cat;
    n->inertial_nav = ins;
    n->sat_nav = sat;
    n->comb_nav = cmb;
    n->year = year;
    n->active = 1;
    csa.total_inertial_nav += ins;
    csa.n_nav_gd++;
    print_str("[CSA] Nav gd "); print_int(csa.n_nav_gd - 1);
    print_str(" type="); print_int(nt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ins="); print_int(ins);
    print_str(" sat="); print_int(sat);
    print_str(" cmb="); print_int(cmb); print_str("\n");
    return csa.n_nav_gd - 1;
}

int csa_sys_en(int st_type, int cat, int mdl, int opt, int sim, int year) {
    if (csa.n_sys_en >= MAX_SYS_EN) return -1;
    sys_en_t* s = &sys_ens[csa.n_sys_en];
    s->se_id = csa.n_sys_en;
    s->se_type = st_type;
    s->se_category = cat;
    s->sys_model = mdl;
    s->sys_optim = opt;
    s->sys_sim = sim;
    s->year = year;
    s->active = 1;
    csa.total_sys_model += mdl;
    csa.n_sys_en++;
    print_str("[CSA] Sys en "); print_int(csa.n_sys_en - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" mdl="); print_int(mdl);
    print_str(" opt="); print_int(opt);
    print_str(" sim="); print_int(sim); print_str("\n");
    return csa.n_sys_en - 1;
}

int csa_intl_ctrl(int it_type, int cat, int fuz, int nrl, int gna, int year) {
    if (csa.n_intl_ctrl >= MAX_INTL_CTRL) return -1;
    intl_ctrl_t* ic = &intl_ctrls[csa.n_intl_ctrl];
    ic->ic_id = csa.n_intl_ctrl;
    ic->ic_type = it_type;
    ic->ic_category = cat;
    ic->fuzzy_ctrl = fuz;
    ic->neural_ctrl = nrl;
    ic->genetic_algo = gna;
    ic->year = year;
    ic->active = 1;
    csa.total_fuzzy_ctrl += fuz;
    csa.n_intl_ctrl++;
    print_str("[CSA] Intl ctrl "); print_int(csa.n_intl_ctrl - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" fuz="); print_int(fuz);
    print_str(" nrl="); print_int(nrl);
    print_str(" gna="); print_int(gna); print_str("\n");
    return csa.n_intl_ctrl - 1;
}

void csa_ctrl_th_report(void) {
    print_str("[CSA] Control theory report:\n");
    print_str("  Control theory categories: "); print_int(csa.n_ctrl_th); print_str("\n");
    print_str("  Total classic control: "); print_int(csa.total_classic_ctrl); print_str("\n");
}

void csa_pat_rc_report(void) {
    print_str("[CSA] Pattern recognition report:\n");
    print_str("  Pattern recognition categories: "); print_int(csa.n_pat_rc); print_str("\n");
    print_str("  Total image recognition: "); print_int(csa.total_img_recog); print_str("\n");
}

void csa_full_report(void) {
    print_str("[CSA] Full report:\n");
    print_str("  Navigation guidance categories: "); print_int(csa.n_nav_gd); print_str("\n");
    print_str("  Total inertial navigation: "); print_int(csa.total_inertial_nav); print_str("\n");
    print_str("  Systems engineering categories: "); print_int(csa.n_sys_en); print_str("\n");
    print_str("  Total system modeling: "); print_int(csa.total_sys_model); print_str("\n");
    print_str("  Intelligent control categories: "); print_int(csa.n_intl_ctrl); print_str("\n");
    print_str("  Total fuzzy control: "); print_int(csa.total_fuzzy_ctrl); print_str("\n");
}

void csa_print_state(void) {
    print_str("[CSA] Ct="); print_int(csa.n_ctrl_th);
    print_str(" Pr="); print_int(csa.n_pat_rc);
    print_str(" Ng="); print_int(csa.n_nav_gd);
    print_str(" Se="); print_int(csa.n_sys_en);
    print_str(" Ic="); print_int(csa.n_intl_ctrl);
    print_str("\n");
}

int main(void) {
    print_str("=== Control Science Admin Demo ===\n\n");
    csa_init();

    print_str("Control theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int cls = 55 + (i * 13);
        int mdn = 40 + (i * 10);
        int rbt = 22 + (i * 5);
        int year = 2020 + (i % 5);
        csa_ctrl_th(type, cat, cls, mdn, rbt, year);
    }

    print_str("\nPattern recognition...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int imr = 48 + (i * 11);
        int vcr = 35 + (i * 8);
        int chr = 20 + (i * 4);
        int year = 2021 + (i % 4);
        csa_pat_rc(type, cat, imr, vcr, chr, year);
    }

    print_str("\nNavigation guidance...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ins = 42 + (i * 10);
        int sat = 28 + (i * 7);
        int cmb = 18 + (i * 4);
        int year = 2022 + (i % 3);
        csa_nav_gd(type, cat, ins, sat, cmb, year);
    }

    print_str("\nSystems engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int mdl = 35 + (i * 8);
        int opt = 25 + (i * 6);
        int sim = 15 + (i * 3);
        int year = 2023 + (i % 2);
        csa_sys_en(type, cat, mdl, opt, sim, year);
    }

    print_str("\nIntelligent control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int fuz = 30 + (i * 7);
        int nrl = 22 + (i * 5);
        int gna = 12 + (i * 3);
        int year = 2024;
        csa_intl_ctrl(type, cat, fuz, nrl, gna, year);
    }

    print_str("\nControl theory report...\n");
    csa_ctrl_th_report();

    print_str("\nPattern recognition report...\n");
    csa_pat_rc_report();

    print_str("\nFull report...\n");
    csa_full_report();

    print_str("\nFinal state...\n");
    csa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
