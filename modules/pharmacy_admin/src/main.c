/* pharmacy_admin: Pharmacy administration system (v1.0)
 * Medicinal chemistry, pharmaceutics, pharmacology, drug analysis, pharmacognosy
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

#define MAX_MED_CHEM    16
#define MAX_PHARMAC     14
#define MAX_PHARMACOL   12
#define MAX_DRUG_ANA    10
#define MAX_PHARMCG     10

typedef struct {
    int    mc_id;
    int    mc_type;
    int    mc_category;
    int    org_drug;
    int    inorg_drug;
    int    nat_drug;
    int    drug_synth;
    int    year;
    int    active;
} med_chem_t;

typedef struct {
    int    pa_id;
    int    pa_type;
    int    pa_category;
    int    prepar;
    int    bio_pharm;
    int    ind_pharm;
    int    clin_pharm;
    int    year;
    int    active;
} pharmaceut_t;

typedef struct {
    int    pl_id;
    int    pl_type;
    int    pl_category;
    int    drug_eff;
    int    drug_met;
    int    clin_phar;
    int    toxicol;
    int    year;
    int    active;
} pharmacol_t;

typedef struct {
    int    da_id;
    int    da_type;
    int    da_category;
    int    qual_std;
    int    detect;
    int    instr_ana;
    int    drug_insp;
    int    year;
    int    active;
} drug_ana_t;

typedef struct {
    int    pg_id;
    int    pg_type;
    int    pg_category;
    int    chin_med;
    int    ethnic_med;
    int    med_plant;
    int    med_ident;
    int    year;
    int    active;
} pharmacg_t;

typedef struct {
    int    n_med_chem;
    int    n_pharmaceut;
    int    n_pharmacol;
    int    n_drug_ana;
    int    n_pharmacg;
    int    total_org_drug;
    int    total_prepar;
    int    total_drug_eff;
    int    total_qual_std;
    int    total_chin_med;
} pha_state_t;

static med_chem_t med_chems[MAX_MED_CHEM];
static pharmaceut_t pharmaceuts[MAX_PHARMAC];
static pharmacol_t pharmacols[MAX_PHARMACOL];
static drug_ana_t drug_anass[MAX_DRUG_ANA];
static pharmacg_t pharmacgs[MAX_PHARMCG];
static pha_state_t pha;

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

int pha_init(void) {
    if (initialized) return -1;
    pha.n_med_chem = 0; pha.n_pharmaceut = 0; pha.n_pharmacol = 0;
    pha.n_drug_ana = 0; pha.n_pharmacg = 0;
    pha.total_org_drug = 0; pha.total_prepar = 0;
    pha.total_drug_eff = 0; pha.total_qual_std = 0;
    pha.total_chin_med = 0;
    for (int i = 0; i < MAX_MED_CHEM; i++) med_chems[i].active = 0;
    for (int i = 0; i < MAX_PHARMAC; i++) pharmaceuts[i].active = 0;
    for (int i = 0; i < MAX_PHARMACOL; i++) pharmacols[i].active = 0;
    for (int i = 0; i < MAX_DRUG_ANA; i++) drug_anass[i].active = 0;
    for (int i = 0; i < MAX_PHARMCG; i++) pharmacgs[i].active = 0;
    initialized = 1;
    print_str("[PHA] Pharmacy initialized\n");
    return 0;
}

int pha_med_chem(int mt_type, int cat, int org, int ino, int nat, int syn, int year) {
    if (pha.n_med_chem >= MAX_MED_CHEM) return -1;
    med_chem_t* m = &med_chems[pha.n_med_chem];
    m->mc_id = pha.n_med_chem;
    m->mc_type = mt_type;
    m->mc_category = cat;
    m->org_drug = org;
    m->inorg_drug = ino;
    m->nat_drug = nat;
    m->drug_synth = syn;
    m->year = year;
    m->active = 1;
    pha.total_org_drug += org;
    pha.n_med_chem++;
    print_str("[PHA] Med chem "); print_int(pha.n_med_chem - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" org="); print_int(org);
    print_str(" ino="); print_int(ino);
    print_str(" nat="); print_int(nat);
    print_str(" syn="); print_int(syn); print_str("\n");
    return pha.n_med_chem - 1;
}

int pha_pharmaceut(int pt_type, int cat, int pre, int bio, int ind, int cli, int year) {
    if (pha.n_pharmaceut >= MAX_PHARMAC) return -1;
    pharmaceut_t* p = &pharmaceuts[pha.n_pharmaceut];
    p->pa_id = pha.n_pharmaceut;
    p->pa_type = pt_type;
    p->pa_category = cat;
    p->prepar = pre;
    p->bio_pharm = bio;
    p->ind_pharm = ind;
    p->clin_pharm = cli;
    p->year = year;
    p->active = 1;
    pha.total_prepar += pre;
    pha.n_pharmaceut++;
    print_str("[PHA] Pharmaceut "); print_int(pha.n_pharmaceut - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pre="); print_int(pre);
    print_str(" bio="); print_int(bio);
    print_str(" ind="); print_int(ind);
    print_str(" cli="); print_int(cli); print_str("\n");
    return pha.n_pharmaceut - 1;
}

int pha_pharmacol(int pl_type, int cat, int dfe, int dme, int cph, int tox, int year) {
    if (pha.n_pharmacol >= MAX_PHARMACOL) return -1;
    pharmacol_t* p = &pharmacols[pha.n_pharmacol];
    p->pl_id = pha.n_pharmacol;
    p->pl_type = pl_type;
    p->pl_category = cat;
    p->drug_eff = dfe;
    p->drug_met = dme;
    p->clin_phar = cph;
    p->toxicol = tox;
    p->year = year;
    p->active = 1;
    pha.total_drug_eff += dfe;
    pha.n_pharmacol++;
    print_str("[PHA] Pharmacol "); print_int(pha.n_pharmacol - 1);
    print_str(" type="); print_int(pl_type);
    print_str(" cat="); print_int(cat);
    print_str(" dfe="); print_int(dfe);
    print_str(" dme="); print_int(dme);
    print_str(" cph="); print_int(cph);
    print_str(" tox="); print_int(tox); print_str("\n");
    return pha.n_pharmacol - 1;
}

int pha_drug_ana(int at_type, int cat, int qst, int det, int ins, int din, int year) {
    if (pha.n_drug_ana >= MAX_DRUG_ANA) return -1;
    drug_ana_t* d = &drug_anass[pha.n_drug_ana];
    d->da_id = pha.n_drug_ana;
    d->da_type = at_type;
    d->da_category = cat;
    d->qual_std = qst;
    d->detect = det;
    d->instr_ana = ins;
    d->drug_insp = din;
    d->year = year;
    d->active = 1;
    pha.total_qual_std += qst;
    pha.n_drug_ana++;
    print_str("[PHA] Drug ana "); print_int(pha.n_drug_ana - 1);
    print_str(" type="); print_int(at_type);
    print_str(" cat="); print_int(cat);
    print_str(" qst="); print_int(qst);
    print_str(" det="); print_int(det);
    print_str(" ins="); print_int(ins);
    print_str(" din="); print_int(din); print_str("\n");
    return pha.n_drug_ana - 1;
}

int pha_pharmacg(int gt_type, int cat, int chm, int eth, int mpl, int mid, int year) {
    if (pha.n_pharmacg >= MAX_PHARMCG) return -1;
    pharmacg_t* g = &pharmacgs[pha.n_pharmacg];
    g->pg_id = pha.n_pharmacg;
    g->pg_type = gt_type;
    g->pg_category = cat;
    g->chin_med = chm;
    g->ethnic_med = eth;
    g->med_plant = mpl;
    g->med_ident = mid;
    g->year = year;
    g->active = 1;
    pha.total_chin_med += chm;
    pha.n_pharmacg++;
    print_str("[PHA] Pharmacg "); print_int(pha.n_pharmacg - 1);
    print_str(" type="); print_int(gt_type);
    print_str(" cat="); print_int(cat);
    print_str(" chm="); print_int(chm);
    print_str(" eth="); print_int(eth);
    print_str(" mpl="); print_int(mpl);
    print_str(" mid="); print_int(mid); print_str("\n");
    return pha.n_pharmacg - 1;
}

void pha_chem_report(void) {
    print_str("[PHA] Medicinal chemistry report:\n");
    print_str("  Chemistry categories: "); print_int(pha.n_med_chem); print_str("\n");
    print_str("  Total organic drugs: "); print_int(pha.total_org_drug); print_str("\n");
}

void pha_pharm_report(void) {
    print_str("[PHA] Pharmaceutics report:\n");
    print_str("  Pharmaceutics categories: "); print_int(pha.n_pharmaceut); print_str("\n");
    print_str("  Total preparations: "); print_int(pha.total_prepar); print_str("\n");
}

void pha_full_report(void) {
    print_str("[PHA] Full report:\n");
    print_str("  Pharmacology categories: "); print_int(pha.n_pharmacol); print_str("\n");
    print_str("  Total drug efficacy: "); print_int(pha.total_drug_eff); print_str("\n");
    print_str("  Drug analysis categories: "); print_int(pha.n_drug_ana); print_str("\n");
    print_str("  Total quality standards: "); print_int(pha.total_qual_std); print_str("\n");
    print_str("  Pharmacognosy categories: "); print_int(pha.n_pharmacg); print_str("\n");
    print_str("  Total Chinese medicine: "); print_int(pha.total_chin_med); print_str("\n");
}

void pha_print_state(void) {
    print_str("[PHA] Mc="); print_int(pha.n_med_chem);
    print_str(" Pt="); print_int(pha.n_pharmaceut);
    print_str(" Pl="); print_int(pha.n_pharmacol);
    print_str(" Da="); print_int(pha.n_drug_ana);
    print_str(" Pg="); print_int(pha.n_pharmacg);
    print_str("\n");
}

int main(void) {
    print_str("=== Pharmacy Admin Demo ===\n\n");
    pha_init();

    print_str("Medicinal chemistry...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int org = 55 + (i * 13);
        int ino = 40 + (i * 10);
        int nat = 22 + (i * 5);
        int syn = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pha_med_chem(type, cat, org, ino, nat, syn, year);
    }

    print_str("\nPharmaceutics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pre = 48 + (i * 11);
        int bio = 35 + (i * 8);
        int ind = 20 + (i * 4);
        int cli = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pha_pharmaceut(type, cat, pre, bio, ind, cli, year);
    }

    print_str("\nPharmacology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dfe = 42 + (i * 10);
        int dme = 28 + (i * 7);
        int cph = 18 + (i * 4);
        int tox = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pha_pharmacol(type, cat, dfe, dme, cph, tox, year);
    }

    print_str("\nDrug analysis...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int qst = 35 + (i * 8);
        int det = 25 + (i * 6);
        int ins = 15 + (i * 3);
        int din = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pha_drug_ana(type, cat, qst, det, ins, din, year);
    }

    print_str("\nPharmacognosy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int chm = 30 + (i * 7);
        int eth = 22 + (i * 5);
        int mpl = 12 + (i * 3);
        int mid = 8 + (i * 2);
        int year = 2024;
        pha_pharmacg(type, cat, chm, eth, mpl, mid, year);
    }

    print_str("\nChemistry report...\n");
    pha_chem_report();

    print_str("\nPharmaceutics report...\n");
    pha_pharm_report();

    print_str("\nFull report...\n");
    pha_full_report();

    print_str("\nFinal state...\n");
    pha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
