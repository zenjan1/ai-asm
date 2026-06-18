/* biological_engineering_admin: Biological engineering administration system (v1.0)
 * Biotechnology, biochemical engineering, bioengineering, bioinformatics, synthetic biology
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

#define MAX_BIO_TECH    16
#define MAX_BIO_CHM     14
#define MAX_BIO_ENG     12
#define MAX_BIO_INF     10
#define MAX_SYNTH_BIO   10

typedef struct {
    int    bt_id;
    int    bt_type;
    int    bt_category;
    int    gene_eng;
    int    cell_eng;
    int    enzyme_eng;
    int    year;
    int    active;
} bio_tech_t;

typedef struct {
    int    bc_id;
    int    bc_type;
    int    bc_category;
    int    bio_cat;
    int    bio_sep;
    int    bio_mat;
    int    year;
    int    active;
} bio_chm_t;

typedef struct {
    int    be_id;
    int    be_type;
    int    be_category;
    int    bio_react;
    int    bio_proc;
    int    bio_prod;
    int    year;
    int    active;
} bio_eng_t;

typedef struct {
    int    bi_id;
    int    bi_type;
    int    bi_category;
    int    genomics;
    int    proteomics;
    int    metabolom;
    int    year;
    int    active;
} bio_inf_t;

typedef struct {
    int    sb_id;
    int    sb_type;
    int    sb_category;
    int    gene_circ;
    int    metabol_eng;
    int    art_cell;
    int    year;
    int    active;
} synth_bio_t;

typedef struct {
    int    n_bio_tech;
    int    n_bio_chm;
    int    n_bio_eng;
    int    n_bio_inf;
    int    n_synth_bio;
    int    total_gene_eng;
    int    total_bio_cat;
    int    total_bio_react;
    int    total_genomics;
    int    total_gene_circ;
} bea_state_t;

static bio_tech_t bio_techs[MAX_BIO_TECH];
static bio_chm_t bio_chms[MAX_BIO_CHM];
static bio_eng_t bio_engs[MAX_BIO_ENG];
static bio_inf_t bio_infs[MAX_BIO_INF];
static synth_bio_t synth_bios[MAX_SYNTH_BIO];
static bea_state_t bea;

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

int bea_init(void) {
    if (initialized) return -1;
    bea.n_bio_tech = 0; bea.n_bio_chm = 0; bea.n_bio_eng = 0;
    bea.n_bio_inf = 0; bea.n_synth_bio = 0;
    bea.total_gene_eng = 0; bea.total_bio_cat = 0;
    bea.total_bio_react = 0; bea.total_genomics = 0;
    bea.total_gene_circ = 0;
    for (int i = 0; i < MAX_BIO_TECH; i++) bio_techs[i].active = 0;
    for (int i = 0; i < MAX_BIO_CHM; i++) bio_chms[i].active = 0;
    for (int i = 0; i < MAX_BIO_ENG; i++) bio_engs[i].active = 0;
    for (int i = 0; i < MAX_BIO_INF; i++) bio_infs[i].active = 0;
    for (int i = 0; i < MAX_SYNTH_BIO; i++) synth_bios[i].active = 0;
    initialized = 1;
    print_str("[BEA] Biological engineering initialized\n");
    return 0;
}

int bea_bio_tech(int bt_type, int cat, int gen, int cll, int enz, int year) {
    if (bea.n_bio_tech >= MAX_BIO_TECH) return -1;
    bio_tech_t* b = &bio_techs[bea.n_bio_tech];
    b->bt_id = bea.n_bio_tech;
    b->bt_type = bt_type;
    b->bt_category = cat;
    b->gene_eng = gen;
    b->cell_eng = cll;
    b->enzyme_eng = enz;
    b->year = year;
    b->active = 1;
    bea.total_gene_eng += gen;
    bea.n_bio_tech++;
    print_str("[BEA] Bio tech "); print_int(bea.n_bio_tech - 1);
    print_str(" type="); print_int(bt_type);
    print_str(" cat="); print_int(cat);
    print_str(" gen="); print_int(gen);
    print_str(" cll="); print_int(cll);
    print_str(" enz="); print_int(enz); print_str("\n");
    return bea.n_bio_tech - 1;
}

int bea_bio_chm(int ct_type, int cat, int bct, int bsp, int bmt, int year) {
    if (bea.n_bio_chm >= MAX_BIO_CHM) return -1;
    bio_chm_t* b = &bio_chms[bea.n_bio_chm];
    b->bc_id = bea.n_bio_chm;
    b->bc_type = ct_type;
    b->bc_category = cat;
    b->bio_cat = bct;
    b->bio_sep = bsp;
    b->bio_mat = bmt;
    b->year = year;
    b->active = 1;
    bea.total_bio_cat += bct;
    bea.n_bio_chm++;
    print_str("[BEA] Bio chm "); print_int(bea.n_bio_chm - 1);
    print_str(" type="); print_int(ct_type);
    print_str(" cat="); print_int(cat);
    print_str(" bct="); print_int(bct);
    print_str(" bsp="); print_int(bsp);
    print_str(" bmt="); print_int(bmt); print_str("\n");
    return bea.n_bio_chm - 1;
}

int bea_bio_eng(int et_type, int cat, int brt, int bpc, int bpd, int year) {
    if (bea.n_bio_eng >= MAX_BIO_ENG) return -1;
    bio_eng_t* b = &bio_engs[bea.n_bio_eng];
    b->be_id = bea.n_bio_eng;
    b->be_type = et_type;
    b->be_category = cat;
    b->bio_react = brt;
    b->bio_proc = bpc;
    b->bio_prod = bpd;
    b->year = year;
    b->active = 1;
    bea.total_bio_react += brt;
    bea.n_bio_eng++;
    print_str("[BEA] Bio eng "); print_int(bea.n_bio_eng - 1);
    print_str(" type="); print_int(et_type);
    print_str(" cat="); print_int(cat);
    print_str(" brt="); print_int(brt);
    print_str(" bpc="); print_int(bpc);
    print_str(" bpd="); print_int(bpd); print_str("\n");
    return bea.n_bio_eng - 1;
}

int bea_bio_inf(int it_type, int cat, int gnm, int prm, int mtm, int year) {
    if (bea.n_bio_inf >= MAX_BIO_INF) return -1;
    bio_inf_t* b = &bio_infs[bea.n_bio_inf];
    b->bi_id = bea.n_bio_inf;
    b->bi_type = it_type;
    b->bi_category = cat;
    b->genomics = gnm;
    b->proteomics = prm;
    b->metabolom = mtm;
    b->year = year;
    b->active = 1;
    bea.total_genomics += gnm;
    bea.n_bio_inf++;
    print_str("[BEA] Bio inf "); print_int(bea.n_bio_inf - 1);
    print_str(" type="); print_int(it_type);
    print_str(" cat="); print_int(cat);
    print_str(" gnm="); print_int(gnm);
    print_str(" prm="); print_int(prm);
    print_str(" mtm="); print_int(mtm); print_str("\n");
    return bea.n_bio_inf - 1;
}

int bea_synth_bio(int st_type, int cat, int gnc, int mte, int arc, int year) {
    if (bea.n_synth_bio >= MAX_SYNTH_BIO) return -1;
    synth_bio_t* s = &synth_bios[bea.n_synth_bio];
    s->sb_id = bea.n_synth_bio;
    s->sb_type = st_type;
    s->sb_category = cat;
    s->gene_circ = gnc;
    s->metabol_eng = mte;
    s->art_cell = arc;
    s->year = year;
    s->active = 1;
    bea.total_gene_circ += gnc;
    bea.n_synth_bio++;
    print_str("[BEA] Synth bio "); print_int(bea.n_synth_bio - 1);
    print_str(" type="); print_int(st_type);
    print_str(" cat="); print_int(cat);
    print_str(" gnc="); print_int(gnc);
    print_str(" mte="); print_int(mte);
    print_str(" arc="); print_int(arc); print_str("\n");
    return bea.n_synth_bio - 1;
}

void bea_bio_tech_report(void) {
    print_str("[BEA] Biotechnology report:\n");
    print_str("  Biotechnology categories: "); print_int(bea.n_bio_tech); print_str("\n");
    print_str("  Total genetic engineering: "); print_int(bea.total_gene_eng); print_str("\n");
}

void bea_bio_chm_report(void) {
    print_str("[BEA] Biochemical engineering report:\n");
    print_str("  Biochemical categories: "); print_int(bea.n_bio_chm); print_str("\n");
    print_str("  Total biocatalysis: "); print_int(bea.total_bio_cat); print_str("\n");
}

void bea_full_report(void) {
    print_str("[BEA] Full report:\n");
    print_str("  Bioengineering categories: "); print_int(bea.n_bio_eng); print_str("\n");
    print_str("  Total bioreactor: "); print_int(bea.total_bio_react); print_str("\n");
    print_str("  Bioinformatics categories: "); print_int(bea.n_bio_inf); print_str("\n");
    print_str("  Total genomics: "); print_int(bea.total_genomics); print_str("\n");
    print_str("  Synthetic biology categories: "); print_int(bea.n_synth_bio); print_str("\n");
    print_str("  Total gene circuits: "); print_int(bea.total_gene_circ); print_str("\n");
}

void bea_print_state(void) {
    print_str("[BEA] Bt="); print_int(bea.n_bio_tech);
    print_str(" Bc="); print_int(bea.n_bio_chm);
    print_str(" Be="); print_int(bea.n_bio_eng);
    print_str(" Bi="); print_int(bea.n_bio_inf);
    print_str(" Sb="); print_int(bea.n_synth_bio);
    print_str("\n");
}

int main(void) {
    print_str("=== Biological Engineering Admin Demo ===\n\n");
    bea_init();

    print_str("Biotechnology...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int gen = 55 + (i * 13);
        int cll = 40 + (i * 10);
        int enz = 22 + (i * 5);
        int year = 2020 + (i % 5);
        bea_bio_tech(type, cat, gen, cll, enz, year);
    }

    print_str("\nBiochemical engineering...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int bct = 48 + (i * 11);
        int bsp = 35 + (i * 8);
        int bmt = 20 + (i * 4);
        int year = 2021 + (i % 4);
        bea_bio_chm(type, cat, bct, bsp, bmt, year);
    }

    print_str("\nBioengineering...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int brt = 42 + (i * 10);
        int bpc = 28 + (i * 7);
        int bpd = 18 + (i * 4);
        int year = 2022 + (i % 3);
        bea_bio_eng(type, cat, brt, bpc, bpd, year);
    }

    print_str("\nBioinformatics...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gnm = 35 + (i * 8);
        int prm = 25 + (i * 6);
        int mtm = 15 + (i * 3);
        int year = 2023 + (i % 2);
        bea_bio_inf(type, cat, gnm, prm, mtm, year);
    }

    print_str("\nSynthetic biology...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int gnc = 30 + (i * 7);
        int mte = 22 + (i * 5);
        int arc = 12 + (i * 3);
        int year = 2024;
        bea_synth_bio(type, cat, gnc, mte, arc, year);
    }

    print_str("\nBiotechnology report...\n");
    bea_bio_tech_report();

    print_str("\nBiochemical engineering report...\n");
    bea_bio_chm_report();

    print_str("\nFull report...\n");
    bea_full_report();

    print_str("\nFinal state...\n");
    bea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
