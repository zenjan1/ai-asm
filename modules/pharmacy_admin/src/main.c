/* pharmacy_admin: Pharmacy administration system (v1.0)
 * Medicinal chemistry, pharmaceutics, pharmacology, Chinese medicine, pharma engineering
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

#define MAX_MED_CHE     16
#define MAX_PHM_ENG     14
#define MAX_PHM_COL     12
#define MAX_CHI_MED     10
#define MAX_PHM_FAC     10

typedef struct {
    int    mc_id;
    int    mc_type;
    int    mc_category;
    int    org_syn;
    int    nat_med;
    int    dru_des;
    int    dru_ana;
    int    year;
    int    active;
} med_che_t;

typedef struct {
    int    pe_id;
    int    pe_type;
    int    pe_category;
    int    pre_eng;
    int    bio_phm;
    int    dru_del;
    int    pre_equ;
    int    year;
    int    active;
} phm_eng_t;

typedef struct {
    int    pc_id;
    int    pc_type;
    int    pc_category;
    int    dru_eff;
    int    dru_kin;
    int    tox_sci;
    int    cli_phm;
    int    year;
    int    active;
} phm_col_t;

typedef struct {
    int    cm_id;
    int    cm_type;
    int    cm_category;
    int    chi_med;
    int    chi_proc;
    int    chi_pre;
    int    chi_idn;
    int    year;
    int    active;
} chi_med_t;

typedef struct {
    int    pf_id;
    int    pf_type;
    int    pf_category;
    int    phm_tec;
    int    phm_equ;
    int    dru_qua;
    int    dru_sup;
    int    year;
    int    active;
} phm_fac_t;

typedef struct {
    int    n_med_che;
    int    n_phm_eng;
    int    n_phm_col;
    int    n_chi_med;
    int    n_phm_fac;
    int    total_org_syn;
    int    total_pre_eng;
    int    total_dru_eff;
    int    total_chi_med;
    int    total_phm_tec;
} pha_state_t;

static med_che_t med_ches[MAX_MED_CHE];
static phm_eng_t phm_engs[MAX_PHM_ENG];
static phm_col_t phm_cols[MAX_PHM_COL];
static chi_med_t chi_meds[MAX_CHI_MED];
static phm_fac_t phm_facs[MAX_PHM_FAC];
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
    pha.n_med_che = 0; pha.n_phm_eng = 0; pha.n_phm_col = 0;
    pha.n_chi_med = 0; pha.n_phm_fac = 0;
    pha.total_org_syn = 0; pha.total_pre_eng = 0;
    pha.total_dru_eff = 0; pha.total_chi_med = 0;
    pha.total_phm_tec = 0;
    for (int i = 0; i < MAX_MED_CHE; i++) med_ches[i].active = 0;
    for (int i = 0; i < MAX_PHM_ENG; i++) phm_engs[i].active = 0;
    for (int i = 0; i < MAX_PHM_COL; i++) phm_cols[i].active = 0;
    for (int i = 0; i < MAX_CHI_MED; i++) chi_meds[i].active = 0;
    for (int i = 0; i < MAX_PHM_FAC; i++) phm_facs[i].active = 0;
    initialized = 1;
    print_str("[PHA] Pharmacy initialized\n");
    return 0;
}

int pha_med_che(int mt_type, int cat, int osy, int ntm, int dds, int dan, int year) {
    if (pha.n_med_che >= MAX_MED_CHE) return -1;
    med_che_t* m = &med_ches[pha.n_med_che];
    m->mc_id = pha.n_med_che;
    m->mc_type = mt_type;
    m->mc_category = cat;
    m->org_syn = osy;
    m->nat_med = ntm;
    m->dru_des = dds;
    m->dru_ana = dan;
    m->year = year;
    m->active = 1;
    pha.total_org_syn += osy;
    pha.n_med_che++;
    print_str("[PHA] Med che "); print_int(pha.n_med_che - 1);
    print_str(" type="); print_int(mt_type);
    print_str(" cat="); print_int(cat);
    print_str(" osy="); print_int(osy);
    print_str(" ntm="); print_int(ntm);
    print_str(" dds="); print_int(dds);
    print_str(" dan="); print_int(dan); print_str("\n");
    return pha.n_med_che - 1;
}

int pha_phm_eng(int pt_type, int cat, int pen, int bpm, int dde, int peq, int year) {
    if (pha.n_phm_eng >= MAX_PHM_ENG) return -1;
    phm_eng_t* p = &phm_engs[pha.n_phm_eng];
    p->pe_id = pha.n_phm_eng;
    p->pe_type = pt_type;
    p->pe_category = cat;
    p->pre_eng = pen;
    p->bio_phm = bpm;
    p->dru_del = dde;
    p->pre_equ = peq;
    p->year = year;
    p->active = 1;
    pha.total_pre_eng += pen;
    pha.n_phm_eng++;
    print_str("[PHA] Phm eng "); print_int(pha.n_phm_eng - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" pen="); print_int(pen);
    print_str(" bpm="); print_int(bpm);
    print_str(" dde="); print_int(dde);
    print_str(" peq="); print_int(peq); print_str("\n");
    return pha.n_phm_eng - 1;
}

int pha_phm_col(int ct_type, int cat, int def, int dki, int tox, int cph, int year) {
    if (pha.n_phm_col >= MAX_PHM_COL) return -1;
    phm_col_t* c = &phm_cols[pha.n_phm_col];
    c->pc_id = pha.n_phm_col;
    c->pc_type = ct_type;
    c->pc_category = cat;
    c->dru_eff = def;
    c->dru_kin = dki;
    c->tox_sci = tox;
    c->cli_phm = cph;
    c->year = year;
    c->active = 1;
    pha.total_dru_eff += def;
    pha.n_phm_col++;
    print_str("[PHA] Phm col "); print_int(pha.n_phm_col - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" def="); print_int(def);
    print_str(" dki="); print_int(dki);
    print_str(" tox="); print_int(tox);
    print_str(" cph="); print_int(cph); print_str("\n");
    return pha.n_phm_col - 1;
}

int pha_chi_med(int ct_type, int cat, int cme, int cpr, int cse, int cid, int year) {
    if (pha.n_chi_med >= MAX_CHI_MED) return -1;
    chi_med_t* c = &chi_meds[pha.n_chi_med];
    c->cm_id = pha.n_chi_med;
    c->cm_type = ct_type;
    c->cm_category = cat;
    c->chi_med = cme;
    c->chi_proc = cpr;
    c->chi_pre = cse;
    c->chi_idn = cid;
    c->year = year;
    c->active = 1;
    pha.total_chi_med += cme;
    pha.n_chi_med++;
    print_str("[PHA] Chi med "); print_int(pha.n_chi_med - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" cme="); print_int(cme);
    print_str(" cpr="); print_int(cpr);
    print_str(" cse="); print_int(cse);
    print_str(" cid="); print_int(cid); print_str("\n");
    return pha.n_chi_med - 1;
}

int pha_phm_fac(int ft_type, int cat, int pte, int peq, int dqu, int dsp, int year) {
    if (pha.n_phm_fac >= MAX_PHM_FAC) return -1;
    phm_fac_t* p = &phm_facs[pha.n_phm_fac];
    p->pf_id = pha.n_phm_fac;
    p->pf_type = ft_type;
    p->pf_category = cat;
    p->phm_tec = pte;
    p->phm_equ = peq;
    p->dru_qua = dqu;
    p->dru_sup = dsp;
    p->year = year;
    p->active = 1;
    pha.total_phm_tec += pte;
    pha.n_phm_fac++;
    print_str("[PHA] Phm fac "); print_int(pha.n_phm_fac - 1);
    print_str(" type="); print_int(ft_type);
    print_str(" cat="); print_int(cat);
    print_str(" pte="); print_int(pte);
    print_str(" peq="); print_int(peq);
    print_str(" dqu="); print_int(dqu);
    print_str(" dsp="); print_int(dsp); print_str("\n");
    return pha.n_phm_fac - 1;
}

void pha_chemistry_report(void) {
    print_str("[PHA] Medicinal chemistry report:\n");
    print_str("  Chemistry categories: "); print_int(pha.n_med_che); print_str("\n");
    print_str("  Total organic synthesis: "); print_int(pha.total_org_syn); print_str("\n");
}

void pha_pharmaceutics_report(void) {
    print_str("[PHA] Pharmaceutics report:\n");
    print_str("  Pharmaceutics categories: "); print_int(pha.n_phm_eng); print_str("\n");
    print_str("  Total preparation engineering: "); print_int(pha.total_pre_eng); print_str("\n");
}

void pha_full_report(void) {
    print_str("[PHA] Full report:\n");
    print_str("  Pharmacology categories: "); print_int(pha.n_phm_col); print_str("\n");
    print_str("  Total drug efficacy: "); print_int(pha.total_dru_eff); print_str("\n");
    print_str("  Chinese medicine categories: "); print_int(pha.n_chi_med); print_str("\n");
    print_str("  Total Chinese medicine: "); print_int(pha.total_chi_med); print_str("\n");
    print_str("  Pharma engineering categories: "); print_int(pha.n_phm_fac); print_str("\n");
    print_str("  Total pharma technology: "); print_int(pha.total_phm_tec); print_str("\n");
}

void pha_print_state(void) {
    print_str("[PHA] Mc="); print_int(pha.n_med_che);
    print_str(" Pe="); print_int(pha.n_phm_eng);
    print_str(" Pc="); print_int(pha.n_phm_col);
    print_str(" Cm="); print_int(pha.n_chi_med);
    print_str(" Pf="); print_int(pha.n_phm_fac);
    print_str("\n");
}

int main(void) {
    print_str("=== Pharmacy Admin Demo ===\n\n");
    pha_init();

    print_str("Medicinal chemistry...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int osy = 55 + (i * 13);
        int ntm = 40 + (i * 10);
        int dds = 22 + (i * 5);
        int dan = 15 + (i * 3);
        int year = 2020 + (i % 5);
        pha_med_che(type, cat, osy, ntm, dds, dan, year);
    }

    print_str("\nPharmaceutics...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pen = 48 + (i * 11);
        int bpm = 35 + (i * 8);
        int dde = 20 + (i * 4);
        int peq = 12 + (i * 3);
        int year = 2021 + (i % 4);
        pha_phm_eng(type, cat, pen, bpm, dde, peq, year);
    }

    print_str("\nPharmacology...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int def = 42 + (i * 10);
        int dki = 28 + (i * 7);
        int tox = 18 + (i * 4);
        int cph = 10 + (i * 2);
        int year = 2022 + (i % 3);
        pha_phm_col(type, cat, def, dki, tox, cph, year);
    }

    print_str("\nChinese medicine...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int cme = 35 + (i * 8);
        int cpr = 25 + (i * 6);
        int cse = 15 + (i * 3);
        int cid = 10 + (i * 2);
        int year = 2023 + (i % 2);
        pha_chi_med(type, cat, cme, cpr, cse, cid, year);
    }

    print_str("\nPharma engineering...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int pte = 30 + (i * 7);
        int peq = 22 + (i * 5);
        int dqu = 12 + (i * 3);
        int dsp = 8 + (i * 2);
        int year = 2024;
        pha_phm_fac(type, cat, pte, peq, dqu, dsp, year);
    }

    print_str("\nChemistry report...\n");
    pha_chemistry_report();

    print_str("\nPharmaceutics report...\n");
    pha_pharmaceutics_report();

    print_str("\nFull report...\n");
    pha_full_report();

    print_str("\nFinal state...\n");
    pha_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
