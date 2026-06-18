/* public_health_admin: Public health administration system (v1.0)
 * Health policy, health management, health education, disease control, supervision
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

#define MAX_HLTH_POL    16
#define MAX_HLTH_MG     14
#define MAX_HLTH_ED     12
#define MAX_DIS_CTRL    10
#define MAX_HLTH_SUP    10

typedef struct {
    int    hp_id;
    int    hp_type;
    int    hp_category;
    int    policy_ana;
    int    sys_ref;
    int    med_sec;
    int    health_pln;
    int    year;
    int    active;
} hlth_pol_t;

typedef struct {
    int    hm_id;
    int    hm_type;
    int    hm_category;
    int    res_mgmt;
    int    svc_qual;
    int    econ_mgmt;
    int    hr_mgmt;
    int    year;
    int    active;
} hlth_mg_t;

typedef struct {
    int    he_id;
    int    he_type;
    int    he_category;
    int    edu_meth;
    int    health_pro;
    int    health_com;
    int    behav_int;
    int    year;
    int    active;
} hlth_ed_t;

typedef struct {
    int    dc_id;
    int    dc_type;
    int    dc_category;
    int    inf_ctrl;
    int    chr_mgmt;
    int    emerg_pub;
    int    dis_surv;
    int    year;
    int    active;
} dis_ctrl_t;

typedef struct {
    int    hs_id;
    int    hs_type;
    int    hs_category;
    int    health_sup;
    int    env_mon;
    int    food_sup;
    int    occ_sup;
    int    year;
    int    active;
} hlth_sup_t;

typedef struct {
    int    n_hlth_pol;
    int    n_hlth_mg;
    int    n_hlth_ed;
    int    n_dis_ctrl;
    int    n_hlth_sup;
    int    total_policy_ana;
    int    total_res_mgmt;
    int    total_edu_meth;
    int    total_inf_ctrl;
    int    total_health_sup;
} phsa_state_t;

static hlth_pol_t hlth_pols[MAX_HLTH_POL];
static hlth_mg_t hlth_mgs[MAX_HLTH_MG];
static hlth_ed_t hlth_eds[MAX_HLTH_ED];
static dis_ctrl_t dis_ctrls[MAX_DIS_CTRL];
static hlth_sup_t hlth_sups[MAX_HLTH_SUP];
static phsa_state_t phsa;

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

int phsa_init(void) {
    if (initialized) return -1;
    phsa.n_hlth_pol = 0; phsa.n_hlth_mg = 0; phsa.n_hlth_ed = 0;
    phsa.n_dis_ctrl = 0; phsa.n_hlth_sup = 0;
    phsa.total_policy_ana = 0; phsa.total_res_mgmt = 0;
    phsa.total_edu_meth = 0; phsa.total_inf_ctrl = 0;
    phsa.total_health_sup = 0;
    for (int i = 0; i < MAX_HLTH_POL; i++) hlth_pols[i].active = 0;
    for (int i = 0; i < MAX_HLTH_MG; i++) hlth_mgs[i].active = 0;
    for (int i = 0; i < MAX_HLTH_ED; i++) hlth_eds[i].active = 0;
    for (int i = 0; i < MAX_DIS_CTRL; i++) dis_ctrls[i].active = 0;
    for (int i = 0; i < MAX_HLTH_SUP; i++) hlth_sups[i].active = 0;
    initialized = 1;
    print_str("[PHSA] Public health initialized\n");
    return 0;
}

int phsa_hlth_pol(int pt_type, int cat, int pla, int sre, int msc, int hpl, int year) {
    if (phsa.n_hlth_pol >= MAX_HLTH_POL) return -1;
    hlth_pol_t* h = &hlth_pols[phsa.n_hlth_pol];
    h->hp_id = phsa.n_hlth_pol;
    h->hp_type = pt_type;
    h->hp_category = cat;
    h->policy_ana = pla;
    h->sys_ref = sre;
    h->med_sec = msc;
    h->health_pln = hpl;
    h->year = year;
    h->active = 1;
    phsa.total_policy_ana += pla;
    phsa.n_hlth_pol++;
    print_str("[PHSA] Hlth pol "); print_int(phsa.n_hlth_pol - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pla="); print_int(pla);
    print_str(" sre="); print_int(sre);
    print_str(" msc="); print_int(msc);
    print_str(" hpl="); print_int(hpl); print_str("\n");
    return phsa.n_hlth_pol - 1;
}

int phsa_hlth_mg(int mt_type, int cat, int rmg, int svq, int ecm, int hrm, int year) {
    if (phsa.n_hlth_mg >= MAX_HLTH_MG) return -1;
    hlth_mg_t* h = &hlth_mgs[phsa.n_hlth_mg];
    h->hm_id = phsa.n_hlth_mg;
    h->hm_type = mt_type;
    h->hm_category = cat;
    h->res_mgmt = rmg;
    h->svc_qual = svq;
    h->econ_mgmt = ecm;
    h->hr_mgmt = hrm;
    h->year = year;
    h->active = 1;
    phsa.total_res_mgmt += rmg;
    phsa.n_hlth_mg++;
    print_str("[PHSA] Hlth mg "); print_int(phsa.n_hlth_mg - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" rmg="); print_int(rmg);
    print_str(" svq="); print_int(svq);
    print_str(" ecm="); print_int(ecm);
    print_str(" hrm="); print_int(hrm); print_str("\n");
    return phsa.n_hlth_mg - 1;
}

int phsa_hlth_ed(int et_type, int cat, int edm, int hpr, int hcm, int bni, int year) {
    if (phsa.n_hlth_ed >= MAX_HLTH_ED) return -1;
    hlth_ed_t* h = &hlth_eds[phsa.n_hlth_ed];
    h->he_id = phsa.n_hlth_ed;
    h->he_type = et_type;
    h->he_category = cat;
    h->edu_meth = edm;
    h->health_pro = hpr;
    h->health_com = hcm;
    h->behav_int = bni;
    h->year = year;
    h->active = 1;
    phsa.total_edu_meth += edm;
    phsa.n_hlth_ed++;
    print_str("[PHSA] Hlth ed "); print_int(phsa.n_hlth_ed - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" edm="); print_int(edm);
    print_str(" hpr="); print_int(hpr);
    print_str(" hcm="); print_int(hcm);
    print_str(" bni="); print_int(bni); print_str("\n");
    return phsa.n_hlth_ed - 1;
}

int phsa_dis_ctrl(int dt_type, int cat, int ifc, int crm, int emp, int dsv, int year) {
    if (phsa.n_dis_ctrl >= MAX_DIS_CTRL) return -1;
    dis_ctrl_t* d = &dis_ctrls[phsa.n_dis_ctrl];
    d->dc_id = phsa.n_dis_ctrl;
    d->dc_type = dt_type;
    d->dc_category = cat;
    d->inf_ctrl = ifc;
    d->chr_mgmt = crm;
    d->emerg_pub = emp;
    d->dis_surv = dsv;
    d->year = year;
    d->active = 1;
    phsa.total_inf_ctrl += ifc;
    phsa.n_dis_ctrl++;
    print_str("[PHSA] Dis ctrl "); print_int(phsa.n_dis_ctrl - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ifc="); print_int(ifc);
    print_str(" crm="); print_int(crm);
    print_str(" emp="); print_int(emp);
    print_str(" dsv="); print_int(dsv); print_str("\n");
    return phsa.n_dis_ctrl - 1;
}

int phsa_hlth_sup(int st_type, int cat, int hsp, int evm, int fsp, int osp, int year) {
    if (phsa.n_hlth_sup >= MAX_HLTH_SUP) return -1;
    hlth_sup_t* h = &hlth_sups[phsa.n_hlth_sup];
    h->hs_id = phsa.n_hlth_sup;
    h->hs_type = st_type;
    h->hs_category = cat;
    h->health_sup = hsp;
    h->env_mon = evm;
    h->food_sup = fsp;
    h->occ_sup = osp;
    h->year = year;
    h->active = 1;
    phsa.total_health_sup += hsp;
    phsa.n_hlth_sup++;
    print_str("[PHSA] Hlth sup "); print_int(phsa.n_hlth_sup - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" hsp="); print_int(hsp);
    print_str(" evm="); print_int(evm);
    print_str(" fsp="); print_int(fsp);
    print_str(" osp="); print_int(osp); print_str("\n");
    return phsa.n_hlth_sup - 1;
}

void phsa_pol_report(void) {
    print_str("[PHSA] Health policy report:\n");
    print_str("  Policy categories: "); print_int(phsa.n_hlth_pol); print_str("\n");
    print_str("  Total policy analysis: "); print_int(phsa.total_policy_ana); print_str("\n");
}

void phsa_mgmt_report(void) {
    print_str("[PHSA] Health management report:\n");
    print_str("  Management categories: "); print_int(phsa.n_hlth_mg); print_str("\n");
    print_str("  Total resource management: "); print_int(phsa.total_res_mgmt); print_str("\n");
}

void phsa_full_report(void) {
    print_str("[PHSA] Full report:\n");
    print_str("  Education categories: "); print_int(phsa.n_hlth_ed); print_str("\n");
    print_str("  Total education methods: "); print_int(phsa.total_edu_meth); print_str("\n");
    print_str("  Disease control categories: "); print_int(phsa.n_dis_ctrl); print_str("\n");
    print_str("  Total infection control: "); print_int(phsa.total_inf_ctrl); print_str("\n");
    print_str("  Supervision categories: "); print_int(phsa.n_hlth_sup); print_str("\n");
    print_str("  Total health supervision: "); print_int(phsa.total_health_sup); print_str("\n");
}

void phsa_print_state(void) {
    print_str("[PHSA] Pl="); print_int(phsa.n_hlth_pol);
    print_str(" Mg="); print_int(phsa.n_hlth_mg);
    print_str(" Ed="); print_int(phsa.n_hlth_ed);
    print_str(" Dc="); print_int(phsa.n_dis_ctrl);
    print_str(" Sp="); print_int(phsa.n_hlth_sup);
    print_str("\n");
}

int main(void) {
    print_str("=== Public Health Admin Demo ===\n\n");
    phsa_init();

    print_str("Health policy...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int pla = 55 + (i * 13);
        int sre = 40 + (i * 10);
        int msc = 22 + (i * 5);
        int hpl = 15 + (i * 3);
        int year = 2020 + (i % 5);
        phsa_hlth_pol(type, cat, pla, sre, msc, hpl, year);
    }

    print_str("\nHealth management...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rmg = 48 + (i * 11);
        int svq = 35 + (i * 8);
        int ecm = 20 + (i * 4);
        int hrm = 12 + (i * 3);
        int year = 2021 + (i % 4);
        phsa_hlth_mg(type, cat, rmg, svq, ecm, hrm, year);
    }

    print_str("\nHealth education...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int edm = 42 + (i * 10);
        int hpr = 28 + (i * 7);
        int hcm = 18 + (i * 4);
        int bni = 10 + (i * 2);
        int year = 2022 + (i % 3);
        phsa_hlth_ed(type, cat, edm, hpr, hcm, bni, year);
    }

    print_str("\nDisease control...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ifc = 35 + (i * 8);
        int crm = 25 + (i * 6);
        int emp = 15 + (i * 3);
        int dsv = 10 + (i * 2);
        int year = 2023 + (i % 2);
        phsa_dis_ctrl(type, cat, ifc, crm, emp, dsv, year);
    }

    print_str("\nHealth supervision...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int hsp = 30 + (i * 7);
        int evm = 22 + (i * 5);
        int fsp = 12 + (i * 3);
        int osp = 8 + (i * 2);
        int year = 2024;
        phsa_hlth_sup(type, cat, hsp, evm, fsp, osp, year);
    }

    print_str("\nPolicy report...\n");
    phsa_pol_report();

    print_str("\nManagement report...\n");
    phsa_mgmt_report();

    print_str("\nFull report...\n");
    phsa_full_report();

    print_str("\nFinal state...\n");
    phsa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
