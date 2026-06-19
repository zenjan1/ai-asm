/* education_economics_admin: Education economics administration system (v1.0)
 * Education economics, education planning, education management, education policy, education market
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

#define MAX_EDU_ECON   16
#define MAX_EDU_PLA    14
#define MAX_EDU_MAN    12
#define MAX_EDU_POL    10
#define MAX_EDU_MKT    10

typedef struct {
    int    ee_id;
    int    ee_type;
    int    ee_category;
    int    edu_inv;
    int    edu_ret;
    int    edu_eq;
    int    edu_pri;
    int    year;
    int    active;
} edu_econ_t;

typedef struct {
    int    ep_id;
    int    ep_type;
    int    ep_category;
    int    res_all;
    int    sch_lay;
    int    edu_scl;
    int    edu_str;
    int    year;
    int    active;
} edu_pla_t;

typedef struct {
    int    em_id;
    int    em_type;
    int    em_category;
    int    org_man;
    int    qua_eva;
    int    edu_hr;
    int    edu_it;
    int    year;
    int    active;
} edu_man_t;

typedef struct {
    int    epo_id;
    int    epo_type;
    int    epo_category;
    int    pol_ana;
    int    edu_ref;
    int    edu_law;
    int    edu_fin;
    int    year;
    int    active;
} edu_pol_t;

typedef struct {
    int    emk_id;
    int    emk_type;
    int    emk_category;
    int    edu_srv;
    int    pri_edu;
    int    edu_ind;
    int    int_edu;
    int    year;
    int    active;
} edu_mkt_t;

typedef struct {
    int    n_edu_econ;
    int    n_edu_pla;
    int    n_edu_man;
    int    n_edu_pol;
    int    n_edu_mkt;
    int    total_edu_inv;
    int    total_res_all;
    int    total_org_man;
    int    total_pol_ana;
    int    total_edu_srv;
} eea_state_t;

static edu_econ_t edu_econs[MAX_EDU_ECON];
static edu_pla_t  edu_plas[MAX_EDU_PLA];
static edu_man_t  edu_mans[MAX_EDU_MAN];
static edu_pol_t  edu_pols[MAX_EDU_POL];
static edu_mkt_t  edu_mkts[MAX_EDU_MKT];
static eea_state_t eea;

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

int eea_init(void) {
    if (initialized) return -1;
    eea.n_edu_econ = 0; eea.n_edu_pla = 0; eea.n_edu_man = 0;
    eea.n_edu_pol = 0; eea.n_edu_mkt = 0;
    eea.total_edu_inv = 0; eea.total_res_all = 0;
    eea.total_org_man = 0; eea.total_pol_ana = 0;
    eea.total_edu_srv = 0;
    for (int i = 0; i < MAX_EDU_ECON; i++) edu_econs[i].active = 0;
    for (int i = 0; i < MAX_EDU_PLA; i++) edu_plas[i].active = 0;
    for (int i = 0; i < MAX_EDU_MAN; i++) edu_mans[i].active = 0;
    for (int i = 0; i < MAX_EDU_POL; i++) edu_pols[i].active = 0;
    for (int i = 0; i < MAX_EDU_MKT; i++) edu_mkts[i].active = 0;
    initialized = 1;
    print_str("[EEA] Education economics initialized\n");
    return 0;
}

int eea_edu_econ(int ee_type, int cat, int inv, int ret, int eq, int pri, int year) {
    if (eea.n_edu_econ >= MAX_EDU_ECON) return -1;
    edu_econ_t* e = &edu_econs[eea.n_edu_econ];
    e->ee_id = eea.n_edu_econ;
    e->ee_type = ee_type;
    e->ee_category = cat;
    e->edu_inv = inv;
    e->edu_ret = ret;
    e->edu_eq = eq;
    e->edu_pri = pri;
    e->year = year;
    e->active = 1;
    eea.total_edu_inv += inv;
    eea.n_edu_econ++;
    print_str("[EEA] Edu econ "); print_int(eea.n_edu_econ - 1);
    print_str(" type="); print_int(ee_type);
    print_str(" cat="); print_int(cat);
    print_str(" inv="); print_int(inv);
    print_str(" ret="); print_int(ret);
    print_str(" eq="); print_int(eq);
    print_str(" pri="); print_int(pri); print_str("\n");
    return eea.n_edu_econ - 1;
}

int eea_edu_pla(int ep_type, int cat, int rall, int slay, int scl, int str, int year) {
    if (eea.n_edu_pla >= MAX_EDU_PLA) return -1;
    edu_pla_t* p = &edu_plas[eea.n_edu_pla];
    p->ep_id = eea.n_edu_pla;
    p->ep_type = ep_type;
    p->ep_category = cat;
    p->res_all = rall;
    p->sch_lay = slay;
    p->edu_scl = scl;
    p->edu_str = str;
    p->year = year;
    p->active = 1;
    eea.total_res_all += rall;
    eea.n_edu_pla++;
    print_str("[EEA] Edu pla "); print_int(eea.n_edu_pla - 1);
    print_str(" type="); print_int(ep_type);
    print_str(" cat="); print_int(cat);
    print_str(" rall="); print_int(rall);
    print_str(" slay="); print_int(slay);
    print_str(" scl="); print_int(scl);
    print_str(" str="); print_int(str); print_str("\n");
    return eea.n_edu_pla - 1;
}

int eea_edu_man(int em_type, int cat, int orgm, int quae, int ehr, int eit, int year) {
    if (eea.n_edu_man >= MAX_EDU_MAN) return -1;
    edu_man_t* m = &edu_mans[eea.n_edu_man];
    m->em_id = eea.n_edu_man;
    m->em_type = em_type;
    m->em_category = cat;
    m->org_man = orgm;
    m->qua_eva = quae;
    m->edu_hr = ehr;
    m->edu_it = eit;
    m->year = year;
    m->active = 1;
    eea.total_org_man += orgm;
    eea.n_edu_man++;
    print_str("[EEA] Edu man "); print_int(eea.n_edu_man - 1);
    print_str(" type="); print_int(em_type);
    print_str(" cat="); print_int(cat);
    print_str(" orgm="); print_int(orgm);
    print_str(" quae="); print_int(quae);
    print_str(" ehr="); print_int(ehr);
    print_str(" eit="); print_int(eit); print_str("\n");
    return eea.n_edu_man - 1;
}

int eea_edu_pol(int epo_type, int cat, int pana, int eref, int elaw, int efin, int year) {
    if (eea.n_edu_pol >= MAX_EDU_POL) return -1;
    edu_pol_t* p = &edu_pols[eea.n_edu_pol];
    p->epo_id = eea.n_edu_pol;
    p->epo_type = epo_type;
    p->epo_category = cat;
    p->pol_ana = pana;
    p->edu_ref = eref;
    p->edu_law = elaw;
    p->edu_fin = efin;
    p->year = year;
    p->active = 1;
    eea.total_pol_ana += pana;
    eea.n_edu_pol++;
    print_str("[EEA] Edu pol "); print_int(eea.n_edu_pol - 1);
    print_str(" type="); print_int(epo_type);
    print_str(" cat="); print_int(cat);
    print_str(" pana="); print_int(pana);
    print_str(" eref="); print_int(eref);
    print_str(" elaw="); print_int(elaw);
    print_str(" efin="); print_int(efin); print_str("\n");
    return eea.n_edu_pol - 1;
}

int eea_edu_mkt(int emk_type, int cat, int esrv, int pedu, int eind, int iedu, int year) {
    if (eea.n_edu_mkt >= MAX_EDU_MKT) return -1;
    edu_mkt_t* m = &edu_mkts[eea.n_edu_mkt];
    m->emk_id = eea.n_edu_mkt;
    m->emk_type = emk_type;
    m->emk_category = cat;
    m->edu_srv = esrv;
    m->pri_edu = pedu;
    m->edu_ind = eind;
    m->int_edu = iedu;
    m->year = year;
    m->active = 1;
    eea.total_edu_srv += esrv;
    eea.n_edu_mkt++;
    print_str("[EEA] Edu mkt "); print_int(eea.n_edu_mkt - 1);
    print_str(" type="); print_int(emk_type);
    print_str(" cat="); print_int(cat);
    print_str(" esrv="); print_int(esrv);
    print_str(" pedu="); print_int(pedu);
    print_str(" eind="); print_int(eind);
    print_str(" iedu="); print_int(iedu); print_str("\n");
    return eea.n_edu_mkt - 1;
}

void eea_econ_report(void) {
    print_str("[EEA] Education economics report:\n");
    print_str("  Economics categories: "); print_int(eea.n_edu_econ); print_str("\n");
    print_str("  Total education investment: "); print_int(eea.total_edu_inv); print_str("\n");
}

void eea_planning_report(void) {
    print_str("[EEA] Education planning report:\n");
    print_str("  Planning categories: "); print_int(eea.n_edu_pla); print_str("\n");
    print_str("  Total resource allocation: "); print_int(eea.total_res_all); print_str("\n");
}

void eea_full_report(void) {
    print_str("[EEA] Full report:\n");
    print_str("  Management categories: "); print_int(eea.n_edu_man); print_str("\n");
    print_str("  Total org management: "); print_int(eea.total_org_man); print_str("\n");
    print_str("  Policy categories: "); print_int(eea.n_edu_pol); print_str("\n");
    print_str("  Total policy analysis: "); print_int(eea.total_pol_ana); print_str("\n");
    print_str("  Market categories: "); print_int(eea.n_edu_mkt); print_str("\n");
    print_str("  Total education services: "); print_int(eea.total_edu_srv); print_str("\n");
}

void eea_print_state(void) {
    print_str("[EEA] Ee="); print_int(eea.n_edu_econ);
    print_str(" Ep="); print_int(eea.n_edu_pla);
    print_str(" Em="); print_int(eea.n_edu_man);
    print_str(" Epo="); print_int(eea.n_edu_pol);
    print_str(" Emk="); print_int(eea.n_edu_mkt);
    print_str("\n");
}

int main(void) {
    print_str("=== Education Economics Admin Demo ===\n\n");
    eea_init();

    print_str("Education economics...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int inv = 60 + (i * 14);
        int ret = 45 + (i * 11);
        int eq = 25 + (i * 6);
        int pri = 18 + (i * 4);
        int year = 2020 + (i % 5);
        eea_edu_econ(type, cat, inv, ret, eq, pri, year);
    }

    print_str("\nEducation planning...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rall = 50 + (i * 12);
        int slay = 38 + (i * 9);
        int scl = 22 + (i * 5);
        int str = 14 + (i * 3);
        int year = 2021 + (i % 4);
        eea_edu_pla(type, cat, rall, slay, scl, str, year);
    }

    print_str("\nEducation management...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int orgm = 44 + (i * 11);
        int quae = 30 + (i * 8);
        int ehr = 20 + (i * 5);
        int eit = 12 + (i * 3);
        int year = 2022 + (i % 3);
        eea_edu_man(type, cat, orgm, quae, ehr, eit, year);
    }

    print_str("\nEducation policy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pana = 38 + (i * 9);
        int eref = 28 + (i * 7);
        int elaw = 16 + (i * 4);
        int efin = 11 + (i * 3);
        int year = 2023 + (i % 2);
        eea_edu_pol(type, cat, pana, eref, elaw, efin, year);
    }

    print_str("\nEducation market...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int esrv = 32 + (i * 8);
        int pedu = 24 + (i * 6);
        int eind = 14 + (i * 3);
        int iedu = 9 + (i * 2);
        int year = 2024;
        eea_edu_mkt(type, cat, esrv, pedu, eind, iedu, year);
    }

    print_str("\nEconomics report...\n");
    eea_econ_report();

    print_str("\nPlanning report...\n");
    eea_planning_report();

    print_str("\nFull report...\n");
    eea_full_report();

    print_str("\nFinal state...\n");
    eea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
