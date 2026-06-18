/* sociology_admin: Sociology administration system (v1.0)
 * Theoretical sociology, applied sociology, demography, ethnology, social psychology
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

#define MAX_THEOR_S     16
#define MAX_APPLY_S     14
#define MAX_DEMOG       12
#define MAX_ETHNOL      10
#define MAX_SOC_PSY     10

typedef struct {
    int    ts_id;
    int    ts_type;
    int    ts_category;
    int    soc_theory;
    int    soc_method;
    int    soc_struct;
    int    soc_chg;
    int    year;
    int    active;
} theor_s_t;

typedef struct {
    int    ap_id;
    int    ap_type;
    int    ap_category;
    int    soc_surv;
    int    soc_stat;
    int    soc_pol;
    int    soc_work;
    int    year;
    int    active;
} apply_s_t;

typedef struct {
    int    dm_id;
    int    dm_type;
    int    dm_category;
    int    pop_theory;
    int    pop_stat;
    int    pop_migr;
    int    pop_policy;
    int    year;
    int    active;
} demog_t;

typedef struct {
    int    et_id;
    int    et_type;
    int    et_category;
    int    eth_theory;
    int    eth_cult;
    int    eth_rel;
    int    eth_policy;
    int    year;
    int    active;
} ethnol_t;

typedef struct {
    int    sp_id;
    int    sp_type;
    int    sp_category;
    int    grp_psy;
    int    soc_att;
    int    soc_beh;
    int    inter_rel;
    int    year;
    int    active;
} soc_psy_t;

typedef struct {
    int    n_theor_s;
    int    n_apply_s;
    int    n_demog;
    int    n_ethnol;
    int    n_soc_psy;
    int    total_soc_theory;
    int    total_soc_surv;
    int    total_pop_theory;
    int    total_eth_theory;
    int    total_grp_psy;
} sa_state_t;

static theor_s_t theor_ss[MAX_THEOR_S];
static apply_s_t apply_ss[MAX_APPLY_S];
static demog_t demogs[MAX_DEMOG];
static ethnol_t ethnols[MAX_ETHNOL];
static soc_psy_t soc_psys[MAX_SOC_PSY];
static sa_state_t sa;

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

int sa_init(void) {
    if (initialized) return -1;
    sa.n_theor_s = 0; sa.n_apply_s = 0; sa.n_demog = 0;
    sa.n_ethnol = 0; sa.n_soc_psy = 0;
    sa.total_soc_theory = 0; sa.total_soc_surv = 0;
    sa.total_pop_theory = 0; sa.total_eth_theory = 0;
    sa.total_grp_psy = 0;
    for (int i = 0; i < MAX_THEOR_S; i++) theor_ss[i].active = 0;
    for (int i = 0; i < MAX_APPLY_S; i++) apply_ss[i].active = 0;
    for (int i = 0; i < MAX_DEMOG; i++) demogs[i].active = 0;
    for (int i = 0; i < MAX_ETHNOL; i++) ethnols[i].active = 0;
    for (int i = 0; i < MAX_SOC_PSY; i++) soc_psys[i].active = 0;
    initialized = 1;
    print_str("[SA] Sociology initialized\n");
    return 0;
}

int sa_theor_s(int tt_type, int cat, int sct, int scm, int scs, int scc, int year) {
    if (sa.n_theor_s >= MAX_THEOR_S) return -1;
    theor_s_t* t = &theor_ss[sa.n_theor_s];
    t->ts_id = sa.n_theor_s;
    t->ts_type = tt_type;
    t->ts_category = cat;
    t->soc_theory = sct;
    t->soc_method = scm;
    t->soc_struct = scs;
    t->soc_chg = scc;
    t->year = year;
    t->active = 1;
    sa.total_soc_theory += sct;
    sa.n_theor_s++;
    print_str("[SA] Theor s "); print_int(sa.n_theor_s - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" sct="); print_int(sct);
    print_str(" scm="); print_int(scm);
    print_str(" scs="); print_int(scs);
    print_str(" scc="); print_int(scc); print_str("\n");
    return sa.n_theor_s - 1;
}

int sa_apply_s(int at_type, int cat, int scs, int sst, int scp, int scw, int year) {
    if (sa.n_apply_s >= MAX_APPLY_S) return -1;
    apply_s_t* a = &apply_ss[sa.n_apply_s];
    a->ap_id = sa.n_apply_s;
    a->ap_type = at_type;
    a->ap_category = cat;
    a->soc_surv = scs;
    a->soc_stat = sst;
    a->soc_pol = scp;
    a->soc_work = scw;
    a->year = year;
    a->active = 1;
    sa.total_soc_surv += scs;
    sa.n_apply_s++;
    print_str("[SA] Apply s "); print_int(sa.n_apply_s - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" scs="); print_int(scs);
    print_str(" sst="); print_int(sst);
    print_str(" scp="); print_int(scp);
    print_str(" scw="); print_int(scw); print_str("\n");
    return sa.n_apply_s - 1;
}

int sa_demog(int dt_type, int cat, int ppt, int pps, int ppm, int ppp, int year) {
    if (sa.n_demog >= MAX_DEMOG) return -1;
    demog_t* d = &demogs[sa.n_demog];
    d->dm_id = sa.n_demog;
    d->dm_type = dt_type;
    d->dm_category = cat;
    d->pop_theory = ppt;
    d->pop_stat = pps;
    d->pop_migr = ppm;
    d->pop_policy = ppp;
    d->year = year;
    d->active = 1;
    sa.total_pop_theory += ppt;
    sa.n_demog++;
    print_str("[SA] Demog "); print_int(sa.n_demog - 1);
    print_str(" type="); print_int(dt_type);
    print_str(" cat="); print_int(cat);
    print_str(" ppt="); print_int(ppt);
    print_str(" pps="); print_int(pps);
    print_str(" ppm="); print_int(ppm);
    print_str(" ppp="); print_int(ppp); print_str("\n");
    return sa.n_demog - 1;
}

int sa_ethnol(int et_type, int cat, int eet, int ect, int ert, int ept, int year) {
    if (sa.n_ethnol >= MAX_ETHNOL) return -1;
    ethnol_t* e = &ethnols[sa.n_ethnol];
    e->et_id = sa.n_ethnol;
    e->et_type = et_type;
    e->et_category = cat;
    e->eth_theory = eet;
    e->eth_cult = ect;
    e->eth_rel = ert;
    e->eth_policy = ept;
    e->year = year;
    e->active = 1;
    sa.total_eth_theory += eet;
    sa.n_ethnol++;
    print_str("[SA] Ethnol "); print_int(sa.n_ethnol - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" eet="); print_int(eet);
    print_str(" ect="); print_int(ect);
    print_str(" ert="); print_int(ert);
    print_str(" ept="); print_int(ept); print_str("\n");
    return sa.n_ethnol - 1;
}

int sa_soc_psy(int st_type, int cat, int gps, int sat, int sbh, int inr, int year) {
    if (sa.n_soc_psy >= MAX_SOC_PSY) return -1;
    soc_psy_t* s = &soc_psys[sa.n_soc_psy];
    s->sp_id = sa.n_soc_psy;
    s->sp_type = st_type;
    s->sp_category = cat;
    s->grp_psy = gps;
    s->soc_att = sat;
    s->soc_beh = sbh;
    s->inter_rel = inr;
    s->year = year;
    s->active = 1;
    sa.total_grp_psy += gps;
    sa.n_soc_psy++;
    print_str("[SA] Soc psy "); print_int(sa.n_soc_psy - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" gps="); print_int(gps);
    print_str(" sat="); print_int(sat);
    print_str(" sbh="); print_int(sbh);
    print_str(" inr="); print_int(inr); print_str("\n");
    return sa.n_soc_psy - 1;
}

void sa_theory_report(void) {
    print_str("[SA] Theoretical sociology report:\n");
    print_str("  Theory categories: "); print_int(sa.n_theor_s); print_str("\n");
    print_str("  Total sociology theory: "); print_int(sa.total_soc_theory); print_str("\n");
}

void sa_apply_report(void) {
    print_str("[SA] Applied sociology report:\n");
    print_str("  Applied categories: "); print_int(sa.n_apply_s); print_str("\n");
    print_str("  Total survey: "); print_int(sa.total_soc_surv); print_str("\n");
}

void sa_full_report(void) {
    print_str("[SA] Full report:\n");
    print_str("  Demography categories: "); print_int(sa.n_demog); print_str("\n");
    print_str("  Total population theory: "); print_int(sa.total_pop_theory); print_str("\n");
    print_str("  Ethnology categories: "); print_int(sa.n_ethnol); print_str("\n");
    print_str("  Total ethnology theory: "); print_int(sa.total_eth_theory); print_str("\n");
    print_str("  Social psychology categories: "); print_int(sa.n_soc_psy); print_str("\n");
    print_str("  Total group psychology: "); print_int(sa.total_grp_psy); print_str("\n");
}

void sa_print_state(void) {
    print_str("[SA] Ts="); print_int(sa.n_theor_s);
    print_str(" As="); print_int(sa.n_apply_s);
    print_str(" Dm="); print_int(sa.n_demog);
    print_str(" Et="); print_int(sa.n_ethnol);
    print_str(" Sp="); print_int(sa.n_soc_psy);
    print_str("\n");
}

int main(void) {
    print_str("=== Sociology Admin Demo ===\n\n");
    sa_init();

    print_str("Theoretical sociology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int sct = 55 + (i * 13);
        int scm = 40 + (i * 10);
        int scs = 22 + (i * 5);
        int scc = 15 + (i * 3);
        int year = 2020 + (i % 5);
        sa_theor_s(type, cat, sct, scm, scs, scc, year);
    }

    print_str("\nApplied sociology...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int scs = 48 + (i * 11);
        int sst = 35 + (i * 8);
        int scp = 20 + (i * 4);
        int scw = 12 + (i * 3);
        int year = 2021 + (i % 4);
        sa_apply_s(type, cat, scs, sst, scp, scw, year);
    }

    print_str("\nDemography...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ppt = 42 + (i * 10);
        int pps = 28 + (i * 7);
        int ppm = 18 + (i * 4);
        int ppp = 10 + (i * 2);
        int year = 2022 + (i % 3);
        sa_demog(type, cat, ppt, pps, ppm, ppp, year);
    }

    print_str("\nEthnology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int eet = 35 + (i * 8);
        int ect = 25 + (i * 6);
        int ert = 15 + (i * 3);
        int ept = 10 + (i * 2);
        int year = 2023 + (i % 2);
        sa_ethnol(type, cat, eet, ect, ert, ept, year);
    }

    print_str("\nSocial psychology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gps = 30 + (i * 7);
        int sat = 22 + (i * 5);
        int sbh = 12 + (i * 3);
        int inr = 8 + (i * 2);
        int year = 2024;
        sa_soc_psy(type, cat, gps, sat, sbh, inr, year);
    }

    print_str("\nTheory report...\n");
    sa_theory_report();

    print_str("\nApplied report...\n");
    sa_apply_report();

    print_str("\nFull report...\n");
    sa_full_report();

    print_str("\nFinal state...\n");
    sa_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
