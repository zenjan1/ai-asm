/* gene_engineering_admin: Gene engineering administration system (v1.0)
 * Gene editing, gene sequencing, gene synthesis, gene therapy, ethics regulation
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

#define MAX_EDITING      16
#define MAX_SEQUENCING   14
#define MAX_SYNTHESIS    12
#define MAX_THERAPY      10
#define MAX_ETHICS       10

typedef struct {
    int    editing_id;
    int    editing_type;
    int    editing_category;
    int    crispr;
    int    base_edit;
    int    prime_edit;
    int    year;
    int    active;
} editing_t;

typedef struct {
    int    seq_id;
    int    seq_type;
    int    seq_category;
    int    seq_technology;
    int    seq_analysis;
    int    genome_assembly;
    int    year;
    int    active;
} sequencing_t;

typedef struct {
    int    synthesis_id;
    int    synthesis_type;
    int    synthesis_category;
    int    dna_synthesis;
    int    gene_construct;
    int    genome_write;
    int    year;
    int    active;
} synthesis_t;

typedef struct {
    int    therapy_id;
    int    therapy_type;
    int    therapy_category;
    int    somatic;
    int    germ_cell;
    int    gene_enhance;
    int    year;
    int    active;
} therapy_t;

typedef struct {
    int    ethics_id;
    int    ethics_type;
    int    ethics_category;
    int    ethics_review;
    int    biosafety;
    int    genetic_resource;
    int    year;
    int    active;
} ethics_t;

typedef struct {
    int    n_editing;
    int    n_sequencing;
    int    n_synthesis;
    int    n_therapy;
    int    n_ethics;
    int    total_crispr;
    int    total_seq;
    int    total_dna;
    int    total_somatic;
    int    total_review;
} gea_state_t;

static editing_t edits[MAX_EDITING];
static sequencing_t seqs[MAX_SEQUENCING];
static synthesis_t syntheses[MAX_SYNTHESIS];
static therapy_t therapies[MAX_THERAPY];
static ethics_t ethics_list[MAX_ETHICS];
static gea_state_t gea;

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

int gea_init(void) {
    if (initialized) return -1;
    gea.n_editing = 0; gea.n_sequencing = 0; gea.n_synthesis = 0;
    gea.n_therapy = 0; gea.n_ethics = 0;
    gea.total_crispr = 0; gea.total_seq = 0;
    gea.total_dna = 0; gea.total_somatic = 0;
    gea.total_review = 0;
    for (int i = 0; i < MAX_EDITING; i++) edits[i].active = 0;
    for (int i = 0; i < MAX_SEQUENCING; i++) seqs[i].active = 0;
    for (int i = 0; i < MAX_SYNTHESIS; i++) syntheses[i].active = 0;
    for (int i = 0; i < MAX_THERAPY; i++) therapies[i].active = 0;
    for (int i = 0; i < MAX_ETHICS; i++) ethics_list[i].active = 0;
    initialized = 1;
    print_str("[GEA] Gene engineering initialized\n");
    return 0;
}

int gea_editing(int ed_type, int cat, int crisp, int base, int prime, int year) {
    if (gea.n_editing >= MAX_EDITING) return -1;
    editing_t* e = &edits[gea.n_editing];
    e->editing_id = gea.n_editing;
    e->editing_type = ed_type;
    e->editing_category = cat;
    e->crispr = crisp;
    e->base_edit = base;
    e->prime_edit = prime;
    e->year = year;
    e->active = 1;
    gea.total_crispr += crisp;
    gea.n_editing++;
    print_str("[GEA] Editing "); print_int(gea.n_editing - 1);
    print_str(" type="); print_int(ed_type);
    print_str(" cat="); print_int(cat);
    print_str(" crp="); print_int(crisp);
    print_str(" bse="); print_int(base);
    print_str(" prm="); print_int(prime); print_str("\n");
    return gea.n_editing - 1;
}

int gea_sequencing(int sq_type, int cat, int tech, int analyze, int assemble, int year) {
    if (gea.n_sequencing >= MAX_SEQUENCING) return -1;
    sequencing_t* s = &seqs[gea.n_sequencing];
    s->seq_id = gea.n_sequencing;
    s->seq_type = sq_type;
    s->seq_category = cat;
    s->seq_technology = tech;
    s->seq_analysis = analyze;
    s->genome_assembly = assemble;
    s->year = year;
    s->active = 1;
    gea.total_seq += tech;
    gea.n_sequencing++;
    print_str("[GEA] Sequencing "); print_int(gea.n_sequencing - 1);
    print_str(" type="); print_int(sq_type);
    print_str(" cat="); print_int(cat);
    print_str(" tch="); print_int(tech);
    print_str(" anl="); print_int(analyze);
    print_str(" asm="); print_int(assemble); print_str("\n");
    return gea.n_sequencing - 1;
}

int gea_synthesis(int sy_type, int cat, int dna, int construct, int write, int year) {
    if (gea.n_synthesis >= MAX_SYNTHESIS) return -1;
    synthesis_t* s = &syntheses[gea.n_synthesis];
    s->synthesis_id = gea.n_synthesis;
    s->synthesis_type = sy_type;
    s->synthesis_category = cat;
    s->dna_synthesis = dna;
    s->gene_construct = construct;
    s->genome_write = write;
    s->year = year;
    s->active = 1;
    gea.total_dna += dna;
    gea.n_synthesis++;
    print_str("[GEA] Synthesis "); print_int(gea.n_synthesis - 1);
    print_str(" type="); print_int(sy_type);
    print_str(" cat="); print_int(cat);
    print_str(" dna="); print_int(dna);
    print_str(" cst="); print_int(construct);
    print_str(" wrt="); print_int(write); print_str("\n");
    return gea.n_synthesis - 1;
}

int gea_therapy(int th_type, int cat, int somatic, int germ, int enhance, int year) {
    if (gea.n_therapy >= MAX_THERAPY) return -1;
    therapy_t* t = &therapies[gea.n_therapy];
    t->therapy_id = gea.n_therapy;
    t->therapy_type = th_type;
    t->therapy_category = cat;
    t->somatic = somatic;
    t->germ_cell = germ;
    t->gene_enhance = enhance;
    t->year = year;
    t->active = 1;
    gea.total_somatic += somatic;
    gea.n_therapy++;
    print_str("[GEA] Therapy "); print_int(gea.n_therapy - 1);
    print_str(" type="); print_int(th_type);
    print_str(" cat="); print_int(cat);
    print_str(" som="); print_int(somatic);
    print_str(" grm="); print_int(germ);
    print_str(" enh="); print_int(enhance); print_str("\n");
    return gea.n_therapy - 1;
}

int gea_ethics(int eth_type, int cat, int review, int safety, int resource, int year) {
    if (gea.n_ethics >= MAX_ETHICS) return -1;
    ethics_t* e = &ethics_list[gea.n_ethics];
    e->ethics_id = gea.n_ethics;
    e->ethics_type = eth_type;
    e->ethics_category = cat;
    e->ethics_review = review;
    e->biosafety = safety;
    e->genetic_resource = resource;
    e->year = year;
    e->active = 1;
    gea.total_review += review;
    gea.n_ethics++;
    print_str("[GEA] Ethics "); print_int(gea.n_ethics - 1);
    print_str(" type="); print_int(eth_type);
    print_str(" cat="); print_int(cat);
    print_str(" rvw="); print_int(review);
    print_str(" sft="); print_int(safety);
    print_str(" rsc="); print_int(resource); print_str("\n");
    return gea.n_ethics - 1;
}

void gea_editing_report(void) {
    print_str("[GEA] Editing report:\n");
    print_str("  Gene editing categories: "); print_int(gea.n_editing); print_str("\n");
    print_str("  Total CRISPR applications: "); print_int(gea.total_crispr); print_str("\n");
}

void gea_sequencing_report(void) {
    print_str("[GEA] Sequencing report:\n");
    print_str("  Gene sequencing categories: "); print_int(gea.n_sequencing); print_str("\n");
    print_str("  Total sequencing technology: "); print_int(gea.total_seq); print_str("\n");
}

void gea_ethics_report(void) {
    print_str("[GEA] Ethics report:\n");
    print_str("  Gene synthesis categories: "); print_int(gea.n_synthesis); print_str("\n");
    print_str("  Total DNA synthesis: "); print_int(gea.total_dna); print_str("\n");
    print_str("  Gene therapy categories: "); print_int(gea.n_therapy); print_str("\n");
    print_str("  Total somatic therapy: "); print_int(gea.total_somatic); print_str("\n");
    print_str("  Ethics regulation categories: "); print_int(gea.n_ethics); print_str("\n");
    print_str("  Total ethics review: "); print_int(gea.total_review); print_str("\n");
}

void gea_print_state(void) {
    print_str("[GEA] Ed="); print_int(gea.n_editing);
    print_str(" Sq="); print_int(gea.n_sequencing);
    print_str(" Sy="); print_int(gea.n_synthesis);
    print_str(" Th="); print_int(gea.n_therapy);
    print_str(" Et="); print_int(gea.n_ethics);
    print_str("\n");
}

int main(void) {
    print_str("=== Gene Engineering Admin Demo ===\n\n");
    gea_init();

    print_str("Gene editing...\n");
    for (int i = 0; i < 16; i++) {
        int type = (i % 5) + 1;
        int cat = (i % 4) + 1;
        int crp = 70 + (i * 17);
        int bse = 45 + (i * 11);
        int prm = 30 + (i * 7);
        int year = 2020 + (i % 5);
        gea_editing(type, cat, crp, bse, prm, year);
    }

    print_str("\nGene sequencing...\n");
    for (int i = 0; i < 14; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int tch = 55 + (i * 13);
        int anl = 40 + (i * 10);
        int assm = 25 + (i * 6);
        int year = 2021 + (i % 4);
        gea_sequencing(type, cat, tch, anl, assm, year);
    }

    print_str("\nGene synthesis...\n");
    for (int i = 0; i < 12; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int dna = 45 + (i * 11);
        int cst = 30 + (i * 7);
        int wrt = 20 + (i * 5);
        int year = 2022 + (i % 3);
        gea_synthesis(type, cat, dna, cst, wrt, year);
    }

    print_str("\nGene therapy...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int som = 35 + (i * 9);
        int grm = 22 + (i * 5);
        int enh = 15 + (i * 3);
        int year = 2023 + (i % 2);
        gea_therapy(type, cat, som, grm, enh, year);
    }

    print_str("\nEthics regulation...\n");
    for (int i = 0; i < 10; i++) {
        int type = (i % 4) + 1;
        int cat = (i % 5) + 1;
        int rvw = 28 + (i * 7);
        int sft = 20 + (i * 5);
        int rsc = 12 + (i * 3);
        int year = 2024;
        gea_ethics(type, cat, rvw, sft, rsc, year);
    }

    print_str("\nEditing report...\n");
    gea_editing_report();

    print_str("\nSequencing report...\n");
    gea_sequencing_report();

    print_str("\nEthics report...\n");
    gea_ethics_report();

    print_str("\nFinal state...\n");
    gea_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
