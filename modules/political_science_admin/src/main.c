/* political_science_admin: Political science administration system (v1.0)
 * Political theory, institutions, public administration, policy, international
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

#define MAX_POL_TH      16
#define MAX_POL_INST    14
#define MAX_POL_ADM     12
#define MAX_PUB_POL     10
#define MAX_INT_POL     10

typedef struct {
    int    pt_id;
    int    pt_type;
    int    pt_category;
    int    pol_pri;
    int    pol_hist;
    int    comp_pol;
    int    pol_phil;
    int    year;
    int    active;
} pol_th_t;

typedef struct {
    int    pi_id;
    int    pi_type;
    int    pi_category;
    int    state_th;
    int    gov_th;
    int    party_sys;
    int    elect_sys;
    int    year;
    int    active;
} pol_inst_t;

typedef struct {
    int    pa_id;
    int    pa_type;
    int    pa_category;
    int    adm_pri;
    int    adm_org;
    int    adm_dec;
    int    adm_sup;
    int    year;
    int    active;
} pol_adm_t;

typedef struct {
    int    pp_id;
    int    pp_type;
    int    pp_category;
    int    pol_ana;
    int    pol_mak;
    int    pol_exe;
    int    pol_eva;
    int    year;
    int    active;
} pub_pol_t;

typedef struct {
    int    ip_id;
    int    ip_type;
    int    ip_category;
    int    int_rel;
    int    int_org;
    int    int_sec;
    int    int_pe;
    int    year;
    int    active;
} int_pol_t;

typedef struct {
    int    n_pol_th;
    int    n_pol_inst;
    int    n_pol_adm;
    int    n_pub_pol;
    int    n_int_pol;
    int    total_pol_pri;
    int    total_state_th;
    int    total_adm_pri;
    int    total_pol_ana;
    int    total_int_rel;
} ps_state_t;

static pol_th_t pol_ths[MAX_POL_TH];
static pol_inst_t pol_insts[MAX_POL_INST];
static pol_adm_t pol_adms[MAX_POL_ADM];
static pub_pol_t pub_pols[MAX_PUB_POL];
static int_pol_t int_pols[MAX_INT_POL];
static ps_state_t ps;

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

int ps_init(void) {
    if (initialized) return -1;
    ps.n_pol_th = 0; ps.n_pol_inst = 0; ps.n_pol_adm = 0;
    ps.n_pub_pol = 0; ps.n_int_pol = 0;
    ps.total_pol_pri = 0; ps.total_state_th = 0;
    ps.total_adm_pri = 0; ps.total_pol_ana = 0;
    ps.total_int_rel = 0;
    for (int i = 0; i < MAX_POL_TH; i++) pol_ths[i].active = 0;
    for (int i = 0; i < MAX_POL_INST; i++) pol_insts[i].active = 0;
    for (int i = 0; i < MAX_POL_ADM; i++) pol_adms[i].active = 0;
    for (int i = 0; i < MAX_PUB_POL; i++) pub_pols[i].active = 0;
    for (int i = 0; i < MAX_INT_POL; i++) int_pols[i].active = 0;
    initialized = 1;
    print_str("[PS] Political science initialized\n");
    return 0;
}

int ps_pol_th(int pt_type, int cat, int ppp, int phh, int cpp, int pph, int year) {
    if (ps.n_pol_th >= MAX_POL_TH) return -1;
    pol_th_t* p = &pol_ths[ps.n_pol_th];
    p->pt_id = ps.n_pol_th;
    p->pt_type = pt_type;
    p->pt_category = cat;
    p->pol_pri = ppp;
    p->pol_hist = phh;
    p->comp_pol = cpp;
    p->pol_phil = pph;
    p->year = year;
    p->active = 1;
    ps.total_pol_pri += ppp;
    ps.n_pol_th++;
    print_str("[PS] Pol th "); print_int(ps.n_pol_th - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppp="); print_int(ppp);
    print_str(" phh="); print_int(phh);
    print_str(" cpp="); print_int(cpp);
    print_str(" pph="); print_int(pph); print_str("\n");
    return ps.n_pol_th - 1;
}

int ps_pol_inst(int it_type, int cat, int sth, int gth, int psy, int esy, int year) {
    if (ps.n_pol_inst >= MAX_POL_INST) return -1;
    pol_inst_t* p = &pol_insts[ps.n_pol_inst];
    p->pi_id = ps.n_pol_inst;
    p->pi_type = it_type;
    p->pi_category = cat;
    p->state_th = sth;
    p->gov_th = gth;
    p->party_sys = psy;
    p->elect_sys = esy;
    p->year = year;
    p->active = 1;
    ps.total_state_th += sth;
    ps.n_pol_inst++;
    print_str("[PS] Pol inst "); print_int(ps.n_pol_inst - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" sth="); print_int(sth);
    print_str(" gth="); print_int(gth);
    print_str(" psy="); print_int(psy);
    print_str(" esy="); print_int(esy); print_str("\n");
    return ps.n_pol_inst - 1;
}

int ps_pol_adm(int at_type, int cat, int apr, int aor, int adc, int asp, int year) {
    if (ps.n_pol_adm >= MAX_POL_ADM) return -1;
    pol_adm_t* p = &pol_adms[ps.n_pol_adm];
    p->pa_id = ps.n_pol_adm;
    p->pa_type = at_type;
    p->pa_category = cat;
    p->adm_pri = apr;
    p->adm_org = aor;
    p->adm_dec = adc;
    p->adm_sup = asp;
    p->year = year;
    p->active = 1;
    ps.total_adm_pri += apr;
    ps.n_pol_adm++;
    print_str("[PS] Pol adm "); print_int(ps.n_pol_adm - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" apr="); print_int(apr);
    print_str(" aor="); print_int(aor);
    print_str(" adc="); print_int(adc);
    print_str(" asp="); print_int(asp); print_str("\n");
    return ps.n_pol_adm - 1;
}

int ps_pub_pol(int pt_type, int cat, int pan, int pmk, int pex, int pev, int year) {
    if (ps.n_pub_pol >= MAX_PUB_POL) return -1;
    pub_pol_t* p = &pub_pols[ps.n_pub_pol];
    p->pp_id = ps.n_pub_pol;
    p->pp_type = pt_type;
    p->pp_category = cat;
    p->pol_ana = pan;
    p->pol_mak = pmk;
    p->pol_exe = pex;
    p->pol_eva = pev;
    p->year = year;
    p->active = 1;
    ps.total_pol_ana += pan;
    ps.n_pub_pol++;
    print_str("[PS] Pub pol "); print_int(ps.n_pub_pol - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pan="); print_int(pan);
    print_str(" pmk="); print_int(pmk);
    print_str(" pex="); print_int(pex);
    print_str(" pev="); print_int(pev); print_str("\n");
    return ps.n_pub_pol - 1;
}

int ps_int_pol(int it_type, int cat, int ire, int ior, int isc, int ipe, int year) {
    if (ps.n_int_pol >= MAX_INT_POL) return -1;
    int_pol_t* n = &int_pols[ps.n_int_pol];
    n->ip_id = ps.n_int_pol;
    n->ip_type = it_type;
    n->ip_category = cat;
    n->int_rel = ire;
    n->int_org = ior;
    n->int_sec = isc;
    n->int_pe = ipe;
    n->year = year;
    n->active = 1;
    ps.total_int_rel += ire;
    ps.n_int_pol++;
    print_str("[PS] Int pol "); print_int(ps.n_int_pol - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" ire="); print_int(ire);
    print_str(" ior="); print_int(ior);
    print_str(" isc="); print_int(isc);
    print_str(" ipe="); print_int(ipe); print_str("\n");
    return ps.n_int_pol - 1;
}

void ps_theory_report(void) {
    print_str("[PS] Political theory report:\n");
    print_str("  Theory categories: "); print_int(ps.n_pol_th); print_str("\n");
    print_str("  Total political principles: "); print_int(ps.total_pol_pri); print_str("\n");
}

void ps_inst_report(void) {
    print_str("[PS] Institutions report:\n");
    print_str("  Institution categories: "); print_int(ps.n_pol_inst); print_str("\n");
    print_str("  Total state theory: "); print_int(ps.total_state_th); print_str("\n");
}

void ps_full_report(void) {
    print_str("[PS] Full report:\n");
    print_str("  Administration categories: "); print_int(ps.n_pol_adm); print_str("\n");
    print_str("  Total admin principles: "); print_int(ps.total_adm_pri); print_str("\n");
    print_str("  Policy categories: "); print_int(ps.n_pub_pol); print_str("\n");
    print_str("  Total policy analysis: "); print_int(ps.total_pol_ana); print_str("\n");
    print_str("  International categories: "); print_int(ps.n_int_pol); print_str("\n");
    print_str("  Total international relations: "); print_int(ps.total_int_rel); print_str("\n");
}

void ps_print_state(void) {
    print_str("[PS] Th="); print_int(ps.n_pol_th);
    print_str(" In="); print_int(ps.n_pol_inst);
    print_str(" Ad="); print_int(ps.n_pol_adm);
    print_str(" Po="); print_int(ps.n_pub_pol);
    print_str(" Ip="); print_int(ps.n_int_pol);
    print_str("\n");
}

int main(void) {
    print_str("=== Political Science Admin Demo ===\n\n");
    ps_init();

    print_str("Political theory...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int ppp = 55 + (i * 13);
        int phh = 40 + (i * 10);
        int cpp = 22 + (i * 5);
        int pph = 15 + (i * 3);
        int year = 2020 + (i % 5);
        ps_pol_th(type, cat, ppp, phh, cpp, pph, year);
    }

    print_str("\nPolitical institutions...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int sth = 48 + (i * 11);
        int gth = 35 + (i * 8);
        int psy = 20 + (i * 4);
        int esy = 12 + (i * 3);
        int year = 2021 + (i % 4);
        ps_pol_inst(type, cat, sth, gth, psy, esy, year);
    }

    print_str("\nPublic administration...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int apr = 42 + (i * 10);
        int aor = 28 + (i * 7);
        int adc = 18 + (i * 4);
        int asp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        ps_pol_adm(type, cat, apr, aor, adc, asp, year);
    }

    print_str("\nPublic policy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pan = 35 + (i * 8);
        int pmk = 25 + (i * 6);
        int pex = 15 + (i * 3);
        int pev = 10 + (i * 2);
        int year = 2023 + (i % 2);
        ps_pub_pol(type, cat, pan, pmk, pex, pev, year);
    }

    print_str("\nInternational politics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ire = 30 + (i * 7);
        int ior = 22 + (i * 5);
        int isc = 12 + (i * 3);
        int ipe = 8 + (i * 2);
        int year = 2024;
        ps_int_pol(type, cat, ire, ior, isc, ipe, year);
    }

    print_str("\nTheory report...\n");
    ps_theory_report();

    print_str("\nInstitutions report...\n");
    ps_inst_report();

    print_str("\nFull report...\n");
    ps_full_report();

    print_str("\nFinal state...\n");
    ps_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
