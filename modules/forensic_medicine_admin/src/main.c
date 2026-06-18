/* forensic_medicine_admin: Forensic medicine administration system (v1.0)
 * Forensic pathology, clinical, evidence, toxicology, psychiatry
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

#define MAX_FOR_PATH    16
#define MAX_FOR_CLIN    14
#define MAX_FOR_EVID    12
#define MAX_FOR_TOX     10
#define MAX_FOR_PSY     10

typedef struct {
    int    fp_id;
    int    fp_type;
    int    fp_category;
    int    death_id;
    int    inj_id;
    int    path_ana;
    int    tox_ana;
    int    year;
    int    active;
} for_path_t;

typedef struct {
    int    fc_id;
    int    fc_type;
    int    fc_category;
    int    inj_assess;
    int    dis_assess;
    int    lab_cap;
    int    med_disp;
    int    year;
    int    active;
} for_clin_t;

typedef struct {
    int    fe_id;
    int    fe_type;
    int    fe_category;
    int    blood_id;
    int    dna_ana;
    int    ind_id;
    int    pat_test;
    int    year;
    int    active;
} for_evid_t;

typedef struct {
    int    ft_id;
    int    ft_type;
    int    ft_category;
    int    tox_det;
    int    drug_ana;
    int    alc_det;
    int    drug_ab;
    int    year;
    int    active;
} for_tox_t;

typedef struct {
    int    fs_id;
    int    fs_type;
    int    fs_category;
    int    psy_id;
    int    beh_cap;
    int    cri_resp;
    int    psy_ev;
    int    year;
    int    active;
} for_psy_t;

typedef struct {
    int    n_for_path;
    int    n_for_clin;
    int    n_for_evid;
    int    n_for_tox;
    int    n_for_psy;
    int    total_death_id;
    int    total_inj_assess;
    int    total_blood_id;
    int    total_tox_det;
    int    total_psy_id;
} fma_state_t;

static for_path_t for_paths[MAX_FOR_PATH];
static for_clin_t for_clins[MAX_FOR_CLIN];
static for_evid_t for_evids[MAX_FOR_EVID];
static for_tox_t for_toxs[MAX_FOR_TOX];
static for_psy_t for_psys[MAX_FOR_PSY];
static fma_state_t fma;

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

int fma_init(void) {
    if (initialized) return -1;
    fma.n_for_path = 0; fma.n_for_clin = 0; fma.n_for_evid = 0;
    fma.n_for_tox = 0; fma.n_for_psy = 0;
    fma.total_death_id = 0; fma.total_inj_assess = 0;
    fma.total_blood_id = 0; fma.total_tox_det = 0;
    fma.total_psy_id = 0;
    for (int i = 0; i < MAX_FOR_PATH; i++) for_paths[i].active = 0;
    for (int i = 0; i < MAX_FOR_CLIN; i++) for_clins[i].active = 0;
    for (int i = 0; i < MAX_FOR_EVID; i++) for_evids[i].active = 0;
    for (int i = 0; i < MAX_FOR_TOX; i++) for_toxs[i].active = 0;
    for (int i = 0; i < MAX_FOR_PSY; i++) for_psys[i].active = 0;
    initialized = 1;
    print_str("[FMA] Forensic medicine initialized\n");
    return 0;
}

int fma_for_path(int pt_type, int cat, int dti, int ini, int pan, int tan, int year) {
    if (fma.n_for_path >= MAX_FOR_PATH) return -1;
    for_path_t* f = &for_paths[fma.n_for_path];
    f->fp_id = fma.n_for_path;
    f->fp_type = pt_type;
    f->fp_category = cat;
    f->death_id = dti;
    f->inj_id = ini;
    f->path_ana = pan;
    f->tox_ana = tan;
    f->year = year;
    f->active = 1;
    fma.total_death_id += dti;
    fma.n_for_path++;
    print_str("[FMA] For path "); print_int(fma.n_for_path - 1);
    print_str(" type="); print_int(pt_type);
    print_str(" cat="); print_int(cat);
    print_str(" dti="); print_int(dti);
    print_str(" ini="); print_int(ini);
    print_str(" pan="); print_int(pan);
    print_str(" tan="); print_int(tan); print_str("\n");
    return fma.n_for_path - 1;
}

int fma_for_clin(int ct_type, int cat, int ias, int das, int lcp, int mdp, int year) {
    if (fma.n_for_clin >= MAX_FOR_CLIN) return -1;
    for_clin_t* f = &for_clins[fma.n_for_clin];
    f->fc_id = fma.n_for_clin;
    f->fc_type = ct_type;
    f->fc_category = cat;
    f->inj_assess = ias;
    f->dis_assess = das;
    f->lab_cap = lcp;
    f->med_disp = mdp;
    f->year = year;
    f->active = 1;
    fma.total_inj_assess += ias;
    fma.n_for_clin++;
    print_str("[FMA] For clin "); print_int(fma.n_for_clin - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" ias="); print_int(ias);
    print_str(" das="); print_int(das);
    print_str(" lcp="); print_int(lcp);
    print_str(" mdp="); print_int(mdp); print_str("\n");
    return fma.n_for_clin - 1;
}

int fma_for_evid(int et_type, int cat, int bld, int dna, int idi, int ptt, int year) {
    if (fma.n_for_evid >= MAX_FOR_EVID) return -1;
    for_evid_t* f = &for_evids[fma.n_for_evid];
    f->fe_id = fma.n_for_evid;
    f->fe_type = et_type;
    f->fe_category = cat;
    f->blood_id = bld;
    f->dna_ana = dna;
    f->ind_id = idi;
    f->pat_test = ptt;
    f->year = year;
    f->active = 1;
    fma.total_blood_id += bld;
    fma.n_for_evid++;
    print_str("[FMA] For evid "); print_int(fma.n_for_evid - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" bld="); print_int(bld);
    print_str(" dna="); print_int(dna);
    print_str(" idi="); print_int(idi);
    print_str(" ptt="); print_int(ptt); print_str("\n");
    return fma.n_for_evid - 1;
}

int fma_for_tox(int tt_type, int cat, int txd, int dan, int ald, int dab, int year) {
    if (fma.n_for_tox >= MAX_FOR_TOX) return -1;
    for_tox_t* f = &for_toxs[fma.n_for_tox];
    f->ft_id = fma.n_for_tox;
    f->ft_type = tt_type;
    f->ft_category = cat;
    f->tox_det = txd;
    f->drug_ana = dan;
    f->alc_det = ald;
    f->drug_ab = dab;
    f->year = year;
    f->active = 1;
    fma.total_tox_det += txd;
    fma.n_for_tox++;
    print_str("[FMA] For tox "); print_int(fma.n_for_tox - 1);
    print_str(" type="); print_int(tt_type);
    print_str(" cat="); print_int(cat);
    print_str(" txd="); print_int(txd);
    print_str(" dan="); print_int(dan);
    print_str(" ald="); print_int(ald);
    print_str(" dab="); print_int(dab); print_str("\n");
    return fma.n_for_tox - 1;
}

int fma_for_psy(int st_type, int cat, int psi, int bcp, int crp, int pev, int year) {
    if (fma.n_for_psy >= MAX_FOR_PSY) return -1;
    for_psy_t* f = &for_psys[fma.n_for_psy];
    f->fs_id = fma.n_for_psy;
    f->fs_type = st_type;
    f->fs_category = cat;
    f->psy_id = psi;
    f->beh_cap = bcp;
    f->cri_resp = crp;
    f->psy_ev = pev;
    f->year = year;
    f->active = 1;
    fma.total_psy_id += psi;
    fma.n_for_psy++;
    print_str("[FMA] For psy "); print_int(fma.n_for_psy - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" psi="); print_int(psi);
    print_str(" bcp="); print_int(bcp);
    print_str(" crp="); print_int(crp);
    print_str(" pev="); print_int(pev); print_str("\n");
    return fma.n_for_psy - 1;
}

void fma_path_report(void) {
    print_str("[FMA] Forensic pathology report:\n");
    print_str("  Pathology categories: "); print_int(fma.n_for_path); print_str("\n");
    print_str("  Total death identification: "); print_int(fma.total_death_id); print_str("\n");
}

void fma_clin_report(void) {
    print_str("[FMA] Forensic clinical report:\n");
    print_str("  Clinical categories: "); print_int(fma.n_for_clin); print_str("\n");
    print_str("  Total injury assessment: "); print_int(fma.total_inj_assess); print_str("\n");
}

void fma_full_report(void) {
    print_str("[FMA] Full report:\n");
    print_str("  Evidence categories: "); print_int(fma.n_for_evid); print_str("\n");
    print_str("  Total blood identification: "); print_int(fma.total_blood_id); print_str("\n");
    print_str("  Toxicology categories: "); print_int(fma.n_for_tox); print_str("\n");
    print_str("  Total toxin detection: "); print_int(fma.total_tox_det); print_str("\n");
    print_str("  Psychiatry categories: "); print_int(fma.n_for_psy); print_str("\n");
    print_str("  Total psychiatric identification: "); print_int(fma.total_psy_id); print_str("\n");
}

void fma_print_state(void) {
    print_str("[FMA] Pt="); print_int(fma.n_for_path);
    print_str(" Cl="); print_int(fma.n_for_clin);
    print_str(" Ev="); print_int(fma.n_for_evid);
    print_str(" Tx="); print_int(fma.n_for_tox);
    print_str(" Ps="); print_int(fma.n_for_psy);
    print_str("\n");
}

int main(void) {
    print_str("=== Forensic Medicine Admin Demo ===\n\n");
    fma_init();

    print_str("Forensic pathology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int dti = 55 + (i * 13);
        int ini = 40 + (i * 10);
        int pan = 22 + (i * 5);
        int tan = 15 + (i * 3);
        int year = 2020 + (i % 5);
        fma_for_path(type, cat, dti, ini, pan, tan, year);
    }

    print_str("\nForensic clinical...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int ias = 48 + (i * 11);
        int das = 35 + (i * 8);
        int lcp = 20 + (i * 4);
        int mdp = 12 + (i * 3);
        int year = 2021 + (i % 4);
        fma_for_clin(type, cat, ias, das, lcp, mdp, year);
    }

    print_str("\nForensic evidence...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bld = 42 + (i * 10);
        int dna = 28 + (i * 7);
        int idi = 18 + (i * 4);
        int ptt = 10 + (i * 2);
        int year = 2022 + (i % 3);
        fma_for_evid(type, cat, bld, dna, idi, ptt, year);
    }

    print_str("\nForensic toxicology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int txd = 35 + (i * 8);
        int dan = 25 + (i * 6);
        int ald = 15 + (i * 3);
        int dab = 10 + (i * 2);
        int year = 2023 + (i % 2);
        fma_for_tox(type, cat, txd, dan, ald, dab, year);
    }

    print_str("\nForensic psychiatry...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int psi = 30 + (i * 7);
        int bcp = 22 + (i * 5);
        int crp = 12 + (i * 3);
        int pev = 8 + (i * 2);
        int year = 2024;
        fma_for_psy(type, cat, psi, bcp, crp, pev, year);
    }

    print_str("\nPathology report...\n");
    fma_path_report();

    print_str("\nClinical report...\n");
    fma_clin_report();

    print_str("\nFull report...\n");
    fma_full_report();

    print_str("\nFinal state...\n");
    fma_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
