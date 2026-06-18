/* management_admin: Management administration system (v1.0)
 * Management principles, organizational behavior, HR, strategy, operations
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

#define MAX_MGM_PRI     16
#define MAX_ORG_BEH     14
#define MAX_HR_MGMT     12
#define MAX_STRAT_M     10
#define MAX_OP_MGM      10

typedef struct {
    int    mp_id;
    int    mp_type;
    int    mp_category;
    int    mgm_th;
    int    mgm_func;
    int    mgm_meth;
    int    mgm_ethic;
    int    year;
    int    active;
} mgm_pri_t;

typedef struct {
    int    ob_id;
    int    ob_type;
    int    ob_category;
    int    ind_beh;
    int    grp_beh;
    int    org_str;
    int    org_cult;
    int    year;
    int    active;
} org_beh_t;

typedef struct {
    int    hr_id;
    int    hr_type;
    int    hr_category;
    int    recruit;
    int    train;
    int    perf;
    int    compen;
    int    year;
    int    active;
} hr_mgmt_t;

typedef struct {
    int    sm_id;
    int    sm_type;
    int    sm_category;
    int    str_ana;
    int    str_for;
    int    str_imp;
    int    str_eva;
    int    year;
    int    active;
} strat_m_t;

typedef struct {
    int    om_id;
    int    om_type;
    int    om_category;
    int    prod_pln;
    int    qual_ctrl;
    int    sup_cha;
    int    proj_mgmt;
    int    year;
    int    active;
} op_mgm_t;

typedef struct {
    int    n_mgm_pri;
    int    n_org_beh;
    int    n_hr_mgmt;
    int    n_strat_m;
    int    n_op_mgm;
    int    total_mgm_th;
    int    total_ind_beh;
    int    total_recruit;
    int    total_str_ana;
    int    total_prod_pln;
} mga_state_t;

static mgm_pri_t mgm_pris[MAX_MGM_PRI];
static org_beh_t org_behs[MAX_ORG_BEH];
static hr_mgmt_t hr_mgms[MAX_HR_MGMT];
static strat_m_t strat_ms[MAX_STRAT_M];
static op_mgm_t op_mgms[MAX_OP_MGM];
static mga_state_t mga;

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

int mga_init(void) {
    if (initialized) return -1;
    mga.n_mgm_pri = 0; mga.n_org_beh = 0; mga.n_hr_mgmt = 0;
    mga.n_strat_m = 0; mga.n_op_mgm = 0;
    mga.total_mgm_th = 0; mga.total_ind_beh = 0;
    mga.total_recruit = 0; mga.total_str_ana = 0;
    mga.total_prod_pln = 0;
    for (int i = 0; i < MAX_MGM_PRI; i++) mgm_pris[i].active = 0;
    for (int i = 0; i < MAX_ORG_BEH; i++) org_behs[i].active = 0;
    for (int i = 0; i < MAX_HR_MGMT; i++) hr_mgms[i].active = 0;
    for (int i = 0; i < MAX_STRAT_M; i++) strat_ms[i].active = 0;
    for (int i = 0; i < MAX_OP_MGM; i++) op_mgms[i].active = 0;
    initialized = 1;
    print_str("[MGA] Management initialized\n");
    return 0;
}

int mga_mgm_pri(int mt_type, int cat, int mth, int mfc, int mmd, int met, int year) {
    if (mga.n_mgm_pri >= MAX_MGM_PRI) return -1;
    mgm_pri_t* m = &mgm_pris[mga.n_mgm_pri];
    m->mp_id = mga.n_mgm_pri;
    m->mp_type = mt_type;
    m->mp_category = cat;
    m->mgm_th = mth;
    m->mgm_func = mfc;
    m->mgm_meth = mmd;
    m->mgm_ethic = met;
    m->year = year;
    m->active = 1;
    mga.total_mgm_th += mth;
    mga.n_mgm_pri++;
    print_str("[MGA] Mgm pri "); print_int(mga.n_mgm_pri - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" mth="); print_int(mth);
    print_str(" mfc="); print_int(mfc);
    print_str(" mmd="); print_int(mmd);
    print_str(" met="); print_int(met); print_str("\n");
    return mga.n_mgm_pri - 1;
}

int mga_org_beh(int ot_type, int cat, int ibh, int gbh, int ostr, int ocu, int year) {
    if (mga.n_org_beh >= MAX_ORG_BEH) return -1;
    org_beh_t* o = &org_behs[mga.n_org_beh];
    o->ob_id = mga.n_org_beh;
    o->ob_type = ot_type;
    o->ob_category = cat;
    o->ind_beh = ibh;
    o->grp_beh = gbh;
    o->org_str = ostr;
    o->org_cult = ocu;
    o->year = year;
    o->active = 1;
    mga.total_ind_beh += ibh;
    mga.n_org_beh++;
    print_str("[MGA] Org beh "); print_int(mga.n_org_beh - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" ibh="); print_int(ibh);
    print_str(" gbh="); print_int(gbh);
    print_str(" ostr="); print_int(ostr);
    print_str(" ocu="); print_int(ocu); print_str("\n");
    return mga.n_org_beh - 1;
}

int mga_hr_mgmt(int ht_type, int cat, int rcr, int trn, int prf, int cmp, int year) {
    if (mga.n_hr_mgmt >= MAX_HR_MGMT) return -1;
    hr_mgmt_t* h = &hr_mgms[mga.n_hr_mgmt];
    h->hr_id = mga.n_hr_mgmt;
    h->hr_type = ht_type;
    h->hr_category = cat;
    h->recruit = rcr;
    h->train = trn;
    h->perf = prf;
    h->compen = cmp;
    h->year = year;
    h->active = 1;
    mga.total_recruit += rcr;
    mga.n_hr_mgmt++;
    print_str("[MGA] HR mgmt "); print_int(mga.n_hr_mgmt - 1);
    print_str(" type="); print_int(ht_type);
    print_str(" cat="); print_int(cat);
    print_str(" rcr="); print_int(rcr);
    print_str(" trn="); print_int(trn);
    print_str(" prf="); print_int(prf);
    print_str(" cmp="); print_int(cmp); print_str("\n");
    return mga.n_hr_mgmt - 1;
}

int mga_strat_m(int st_type, int cat, int san, int sfo, int sim, int sev, int year) {
    if (mga.n_strat_m >= MAX_STRAT_M) return -1;
    strat_m_t* s = &strat_ms[mga.n_strat_m];
    s->sm_id = mga.n_strat_m;
    s->sm_type = st_type;
    s->sm_category = cat;
    s->str_ana = san;
    s->str_for = sfo;
    s->str_imp = sim;
    s->str_eva = sev;
    s->year = year;
    s->active = 1;
    mga.total_str_ana += san;
    mga.n_strat_m++;
    print_str("[MGA] Strat m "); print_int(mga.n_strat_m - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" san="); print_int(san);
    print_str(" sfo="); print_int(sfo);
    print_str(" sim="); print_int(sim);
    print_str(" sev="); print_int(sev); print_str("\n");
    return mga.n_strat_m - 1;
}

int mga_op_mgm(int ot_type, int cat, int ppl, int qcl, int sch, int pjm, int year) {
    if (mga.n_op_mgm >= MAX_OP_MGM) return -1;
    op_mgm_t* o = &op_mgms[mga.n_op_mgm];
    o->om_id = mga.n_op_mgm;
    o->om_type = ot_type;
    o->om_category = cat;
    o->prod_pln = ppl;
    o->qual_ctrl = qcl;
    o->sup_cha = sch;
    o->proj_mgmt = pjm;
    o->year = year;
    o->active = 1;
    mga.total_prod_pln += ppl;
    mga.n_op_mgm++;
    print_str("[MGA] Op mgmt "); print_int(mga.n_op_mgm - 1);
    print_str(" type="); print_int(ot_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppl="); print_int(ppl);
    print_str(" qcl="); print_int(qcl);
    print_str(" sch="); print_int(sch);
    print_str(" pjm="); print_int(pjm); print_str("\n");
    return mga.n_op_mgm - 1;
}

void mga_principle_report(void) {
    print_str("[MGA] Management principles report:\n");
    print_str("  Principle categories: "); print_int(mga.n_mgm_pri); print_str("\n");
    print_str("  Total management theory: "); print_int(mga.total_mgm_th); print_str("\n");
}

void mga_org_report(void) {
    print_str("[MGA] Organizational behavior report:\n");
    print_str("  Org behavior categories: "); print_int(mga.n_org_beh); print_str("\n");
    print_str("  Total individual behavior: "); print_int(mga.total_ind_beh); print_str("\n");
}

void mga_full_report(void) {
    print_str("[MGA] Full report:\n");
    print_str("  HR management categories: "); print_int(mga.n_hr_mgmt); print_str("\n");
    print_str("  Total recruitment: "); print_int(mga.total_recruit); print_str("\n");
    print_str("  Strategy categories: "); print_int(mga.n_strat_m); print_str("\n");
    print_str("  Total strategy analysis: "); print_int(mga.total_str_ana); print_str("\n");
    print_str("  Operations categories: "); print_int(mga.n_op_mgm); print_str("\n");
    print_str("  Total production planning: "); print_int(mga.total_prod_pln); print_str("\n");
}

void mga_print_state(void) {
    print_str("[MGA] Mp="); print_int(mga.n_mgm_pri);
    print_str(" Ob="); print_int(mga.n_org_beh);
    print_str(" Hr="); print_int(mga.n_hr_mgmt);
    print_str(" Sm="); print_int(mga.n_strat_m);
    print_str(" Om="); print_int(mga.n_op_mgm);
    print_str("\n");
}

int main(void) {
    print_str("=== Management Admin Demo ===\n\n");
    mga_init();

    print_str("Management principles...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int mth = 55 + (i * 13);
        int mfc = 40 + (i * 10);
        int mmd = 22 + (i * 5);
        int met = 15 + (i * 3);
        int year = 2020 + (i % 5);
        mga_mgm_pri(type, cat, mth, mfc, mmd, met, year);
    }

    print_str("\nOrganizational behavior...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ibh = 48 + (i * 11);
        int gbh = 35 + (i * 8);
        int ostr = 20 + (i * 4);
        int ocu = 12 + (i * 3);
        int year = 2021 + (i % 4);
        mga_org_beh(type, cat, ibh, gbh, ostr, ocu, year);
    }

    print_str("\nHR management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rcr = 42 + (i * 10);
        int trn = 28 + (i * 7);
        int prf = 18 + (i * 4);
        int cmp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        mga_hr_mgmt(type, cat, rcr, trn, prf, cmp, year);
    }

    print_str("\nStrategic management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int san = 35 + (i * 8);
        int sfo = 25 + (i * 6);
        int sim = 15 + (i * 3);
        int sev = 10 + (i * 2);
        int year = 2023 + (i % 2);
        mga_strat_m(type, cat, san, sfo, sim, sev, year);
    }

    print_str("\nOperations management...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppl = 30 + (i * 7);
        int qcl = 22 + (i * 5);
        int sch = 12 + (i * 3);
        int pjm = 8 + (i * 2);
        int year = 2024;
        mga_op_mgm(type, cat, ppl, qcl, sch, pjm, year);
    }

    print_str("\nPrinciple report...\n");
    mga_principle_report();

    print_str("\nOrg report...\n");
    mga_org_report();

    print_str("\nFull report...\n");
    mga_full_report();

    print_str("\nFinal state...\n");
    mga_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
