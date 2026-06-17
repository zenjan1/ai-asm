/* bioinformatics_pipeline: Genomic analysis toolkit (v1.0)
 * Sequence alignment, BLAST, variant detection, protein folding, pathway analysis
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

#define MAX_SEQ_LEN   1024
#define MAX_SEQUENCES 32
#define MAX_VARIANTS  64
#define MAX_GENES     32
#define MAX_PATHWAYS  16
#define MAX_SPECIES   8

#define SEQ_DNA  0x01
#define SEQ_RNA  0x02
#define SEQ_PROTEIN 0x03

#define ALIGN_GLOBAL  0x01
#define ALIGN_LOCAL   0x02
#define ALIGN_SEMIGLOBAL 0x03

#define VAR_SNP    0x01
#define VAR_INSERTION 0x02
#define VAR_DELETION 0x03
#define VAR_SV     0x04

#define FOLD_MC    0x01
#define FOLD_SA    0x02
#define FOLD_MD    0x03

#define PATH_METABOLIC  0x01
#define PATH_SIGNALING  0x02
#define PATH_REGULATORY 0x03

typedef struct {
    char   data[MAX_SEQ_LEN];
    int    length;
    int    type;
    char   name[64];
    double gc_content;
    double entropy;
    int    active;
} sequence_t;

typedef struct {
    int    seq1;
    int    seq2;
    int    score;
    int    matches;
    int    mismatches;
    int    gaps;
    int    identity;
    int    e_value;
    int    type;
} alignment_t;

typedef struct {
    int    position;
    int    seq_id;
    int    type;
    char   ref_base;
    char   alt_base;
    double quality;
    int    coverage;
    int    active;
} variant_t;

typedef struct {
    int    start;
    int    end;
    int    seq_id;
    int    strand;
    double score;
    char   gene_name[32];
    int    active;
} gene_t;

typedef struct {
    int    type;
    int    gene_count;
    int    genes[MAX_GENES];
    double flux;
    char   name[32];
} pathway_t;

typedef struct {
    double phi[MAX_SEQ_LEN];
    double psi[MAX_SEQ_LEN];
    double omega[MAX_SEQ_LEN];
    double energy;
    double rmsd;
    int    seq_id;
    int    method;
    int    converged;
} protein_fold_t;

typedef struct {
    int    seq_ids[MAX_SPECIES];
    int    species_count;
    int    tree_matrix[MAX_SPECIES][MAX_SPECIES];
    double distances[MAX_SPECIES][MAX_SPECIES];
} phylo_tree_t;

typedef struct {
    int    seq_count;
    int    var_count;
    int    gene_count;
    int    pathway_count;
    int    fold_count;
    double avg_identity;
    double avg_coverage;
    int    total_bases;
    int    total_variants;
    int    annotation_complete;
} bio_state_t;

static sequence_t sequences[MAX_SEQUENCES];
static alignment_t alignments[MAX_SEQUENCES * MAX_SEQUENCES];
static variant_t variants[MAX_VARIANTS];
static gene_t genes[MAX_GENES];
static pathway_t pathways[MAX_PATHWAYS];
static protein_fold_t folds[MAX_SEQUENCES];
static phylo_tree_t phylo;
static bio_state_t bio;

static int initialized = 0;
static int alignment_count = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static int imin(int a, int b) { return a < b ? a : b; }
static int imax(int a, int b) { return a > b ? a : b; }

int bio_init(void) {
    if (initialized) return -1;
    bio.seq_count = 0; bio.var_count = 0; bio.gene_count = 0;
    bio.pathway_count = 0; bio.fold_count = 0;
    bio.avg_identity = 0.0; bio.avg_coverage = 0.0;
    bio.total_bases = 0; bio.total_variants = 0;
    bio.annotation_complete = 0;
    for (int i = 0; i < MAX_SEQUENCES; i++) sequences[i].active = 0;
    for (int i = 0; i < MAX_VARIANTS; i++) variants[i].active = 0;
    for (int i = 0; i < MAX_GENES; i++) genes[i].active = 0;
    for (int i = 0; i < MAX_SPECIES; i++)
        for (int j = 0; j < MAX_SPECIES; j++) {
            phylo.tree_matrix[i][j] = 0;
            phylo.distances[i][j] = 0.0;
        }
    phylo.species_count = 0;
    alignment_count = 0;
    initialized = 1;
    print_str("[BIO] Pipeline initialized\n");
    return 0;
}

int bio_add_sequence(const char* seq, int length, int type, const char* name) {
    if (bio.seq_count >= MAX_SEQUENCES) return -1;
    sequence_t* s = &sequences[bio.seq_count];
    int len = length < MAX_SEQ_LEN ? length : MAX_SEQ_LEN - 1;
    for (int i = 0; i < len; i++) s->data[i] = seq[i];
    s->data[len] = '\0';
    s->length = len;
    s->type = type;
    for (int i = 0; i < 63 && name[i]; i++) s->name[i] = name[i];
    s->name[63] = '\0';
    int gc = 0;
    if (type == SEQ_DNA || type == SEQ_RNA) {
        for (int i = 0; i < len; i++)
            if (s->data[i] == 'G' || s->data[i] == 'C' || s->data[i] == 'g' || s->data[i] == 'c') gc++;
    }
    s->gc_content = len > 0 ? (double)gc / len : 0.0;
    double h = 0.0;
    for (int i = 0; i < len - 1; i++) {
        double p = 0.0;
        for (int j = i; j < i + 10 && j < len; j++)
            if (s->data[j] == s->data[i]) p += 0.1;
        if (p > 0.0 && p < 1.0) h -= p * dcos(p * 3.14159);
    }
    s->entropy = h / (len > 0 ? len : 1);
    s->active = 1;
    bio.total_bases += len;
    return bio.seq_count++;
}

static int score_match(char a, char b) {
    if (a == b) return 2;
    return -1;
}

alignment_t bio_align_pair(int s1, int s2, int type) {
    alignment_t aln;
    aln.seq1 = s1; aln.seq2 = s2; aln.type = type;
    if (s1 >= bio.seq_count || s2 >= bio.seq_count) {
        aln.score = 0; aln.matches = 0; aln.mismatches = 0;
        aln.gaps = 0; aln.identity = 0; aln.e_value = 999999;
        return aln;
    }
    sequence_t* seq1 = &sequences[s1];
    sequence_t* seq2 = &sequences[s2];
    int len1 = seq1->length, len2 = seq2->length;
    int max_len = imin(len1, len2);
    int matches = 0, mismatches = 0, gaps = 0, score = 0;
    if (type == ALIGN_GLOBAL) {
        for (int i = 0; i < max_len; i++) {
            int sc = score_match(seq1->data[i], seq2->data[i]);
            if (sc > 0) { matches++; score += sc; }
            else { mismatches++; score += sc; }
        }
        gaps = len1 > len2 ? len1 - len2 : len2 - len1;
        score -= gaps;
    } else if (type == ALIGN_LOCAL) {
        int best = 0, best_start = 0, best_len = 0;
        for (int off1 = 0; off1 < len1; off1++) {
            for (int off2 = 0; off2 < len2; off2++) {
                int run = 0;
                while (off1 + run < len1 && off2 + run < len2) {
                    if (seq1->data[off1 + run] == seq2->data[off2 + run]) run++;
                    else break;
                }
                if (run > best) { best = run; best_start = off1; best_len = run; }
            }
        }
        matches = best;
        score = best * 2;
        (void)best_start; (void)best_len;
    }
    aln.matches = matches;
    aln.mismatches = mismatches;
    aln.gaps = gaps;
    aln.score = score;
    int total = matches + mismatches;
    aln.identity = total > 0 ? (matches * 100) / total : 0;
    aln.e_value = score > 0 ? 1000 / (score + 1) : 999999;
    if (alignment_count < MAX_SEQUENCES * MAX_SEQUENCES) {
        alignments[alignment_count++] = aln;
    }
    bio.avg_identity = (bio.avg_identity * (alignment_count - 1) + aln.identity) / alignment_count;
    return aln;
}

void bio_blast_search(int query, int db_start, int db_end) {
    if (query >= bio.seq_count) return;
    int start = db_start < 0 ? 0 : db_start;
    int end = db_end > bio.seq_count ? bio.seq_count : db_end;
    print_str("[BIO] BLAST search query="); print_int(query);
    print_str(" db="); print_int(start); print_str("-"); print_int(end); print_str("\n");
    int hits = 0;
    for (int i = start; i < end; i++) {
        if (i == query) continue;
        alignment_t aln = bio_align_pair(query, i, ALIGN_LOCAL);
        if (aln.identity > 70 && aln.e_value < 100) {
            print_str("  Hit: seq "); print_int(i);
            print_str(" id="); print_int(aln.identity); print_str("%");
            print_str(" e="); print_int(aln.e_value);
            print_str(" score="); print_int(aln.score);
            print_str("\n");
            hits++;
        }
    }
    print_str("BLAST: "); print_int(hits); print_str(" significant hits\n");
}

void bio_detect_variants(int ref_id, int sample_id) {
    if (ref_id >= bio.seq_count || sample_id >= bio.seq_count) return;
    sequence_t* ref = &sequences[ref_id];
    sequence_t* sample = &sequences[sample_id];
    int min_len = imin(ref->length, sample->length);
    bio.var_count = 0;
    for (int i = 0; i < min_len && bio.var_count < MAX_VARIANTS; i++) {
        if (ref->data[i] != sample->data[i]) {
            variant_t* v = &variants[bio.var_count];
            v->position = i;
            v->seq_id = sample_id;
            v->ref_base = ref->data[i];
            v->alt_base = sample->data[i];
            v->quality = 30.0 + (i % 20);
            v->coverage = 20 + (i % 50);
            int run_len = 0;
            for (int j = i + 1; j < min_len && j < i + 5; j++) {
                if (ref->data[j] != sample->data[j]) run_len++;
            }
            if (run_len == 0) v->type = VAR_SNP;
            else if (ref->data[i] == '-') v->type = VAR_INSERTION;
            else if (sample->data[i] == '-') v->type = VAR_DELETION;
            else v->type = VAR_SNP;
            v->active = 1;
            bio.var_count++;
        }
    }
    bio.total_variants = bio.var_count;
    print_str("[BIO] Variants detected: "); print_int(bio.var_count); print_str("\n");
    for (int i = 0; i < bio.var_count && i < 5; i++) {
        variant_t* v = &variants[i];
        print_str("  "); print_int(v->position);
        print_str(": "); char b[2] = {v->ref_base, '\0'}; print_str(b);
        print_str(">"); b[0] = v->alt_base; print_str(b);
        print_str(" q="); print_int((int)v->quality);
        print_str(" cov="); print_int(v->coverage);
        print_str(" type="); print_int(v->type);
        print_str("\n");
    }
}

void bio_annotate_genes(void) {
    bio.gene_count = 0;
    for (int s = 0; s < bio.seq_count && bio.gene_count < MAX_GENES; s++) {
        if (!sequences[s].active || sequences[s].type != SEQ_DNA) continue;
        sequence_t* seq = &sequences[s];
        for (int i = 0; i < seq->length - 30 && bio.gene_count < MAX_GENES; i += 50) {
            int has_start = (i + 2 < seq->length &&
                            seq->data[i] == 'A' && seq->data[i+1] == 'T' && seq->data[i+2] == 'G');
            int has_stop = 0;
            int stop_pos = 0;
            for (int j = i + 30; j < i + 200 && j < seq->length - 2; j += 3) {
                if ((seq->data[j] == 'T' && seq->data[j+1] == 'A' && seq->data[j+2] == 'A') ||
                    (seq->data[j] == 'T' && seq->data[j+1] == 'A' && seq->data[j+2] == 'G') ||
                    (seq->data[j] == 'T' && seq->data[j+1] == 'G' && seq->data[j+2] == 'A')) {
                    has_stop = 1; stop_pos = j + 3; break;
                }
            }
            if (has_start && has_stop) {
                gene_t* g = &genes[bio.gene_count];
                g->start = i;
                g->end = stop_pos;
                g->seq_id = s;
                g->strand = 1;
                g->score = (stop_pos - i) / 10.0;
                g->active = 1;
                bio.gene_count++;
            }
        }
    }
    bio.annotation_complete = 1;
    print_str("[BIO] Gene annotation: "); print_int(bio.gene_count); print_str(" genes found\n");
    for (int i = 0; i < bio.gene_count && i < 4; i++) {
        gene_t* g = &genes[i];
        print_str("  Gene "); print_int(i+1);
        print_str(": "); print_int(g->start); print_str("-"); print_int(g->end);
        print_str(" score="); print_int((int)g->score);
        print_str(" seq="); print_int(g->seq_id);
        print_str("\n");
    }
}

void bio_fold_protein(int seq_id, int method) {
    if (seq_id >= bio.seq_count) return;
    if (sequences[seq_id].type != SEQ_PROTEIN && sequences[seq_id].type != SEQ_RNA) {
        print_str("[BIO] Skipping non-protein sequence "); print_int(seq_id); print_str("\n");
        return;
    }
    sequence_t* seq = &sequences[seq_id];
    protein_fold_t* f = &folds[bio.fold_count];
    f->seq_id = seq_id;
    f->method = method;
    int len = seq->length;
    double energy = 0.0;
    if (method == FOLD_MC) {
        for (int i = 0; i < len; i++) {
            f->phi[i] = ((i * 31 + 45) % 360 - 180) * 0.01745;
            f->psi[i] = ((i * 17 + 90) % 360 - 180) * 0.01745;
            f->omega[i] = 3.14159;
            double e = 0.5 * dcos(f->phi[i]) + 0.3 * dcos(f->psi[i]);
            energy += e;
        }
        for (int iter = 0; iter < 100; iter++) {
            int idx = (iter * 7 + 3) % len;
            double delta_phi = ((iter * 13) % 20 - 10) * 0.01745;
            double new_phi = f->phi[idx] + delta_phi;
            double new_e = 0.5 * dcos(new_phi) + 0.3 * dcos(f->psi[idx]);
            double old_e = 0.5 * dcos(f->phi[idx]) + 0.3 * dcos(f->psi[idx]);
            if (new_e < old_e || (iter % 5) == 0) {
                f->phi[idx] = new_phi;
                energy += (new_e - old_e);
            }
        }
    } else if (method == FOLD_SA) {
        for (int i = 0; i < len; i++) {
            f->phi[i] = ((i * 23 + 60) % 360 - 180) * 0.01745;
            f->psi[i] = ((i * 41 + 120) % 360 - 180) * 0.01745;
            f->omega[i] = 3.14159;
        }
        double temp = 1.0;
        for (int iter = 0; iter < 200; iter++) {
            temp *= 0.98;
            int idx = (iter * 11 + 5) % len;
            double delta = ((iter * 17) % 30 - 15) * 0.01745;
            double new_phi = f->phi[idx] + delta;
            double dE = dcos(new_phi) - dcos(f->phi[idx]);
            if (dE < 0.0 || temp > dabs(dE)) {
                f->phi[idx] = new_phi;
            }
        }
        for (int i = 0; i < len; i++) {
            energy += 0.5 * dcos(f->phi[i]) + 0.3 * dcos(f->psi[i]);
        }
    }
    double rmsd = 0.0;
    for (int i = 0; i < len; i++) {
        double d = dcos(f->phi[i] * i * 0.01) * 0.1;
        rmsd += d * d;
    }
    f->energy = energy;
    f->rmsd = dsqrt(rmsd / (len > 0 ? len : 1));
    f->converged = 1;
    bio.fold_count++;
    print_str("[BIO] Protein fold (method="); print_int(method);
    print_str("): E="); print_int((int)(energy * 100));
    print_str(" RMSD="); print_int((int)(f->rmsd * 1000));
    print_str("\n");
}

void bio_pathway_analysis(void) {
    bio.pathway_count = 3;
    for (int p = 0; p < bio.pathway_count; p++) {
        pathways[p].type = PATH_METABOLIC + p;
        pathways[p].gene_count = 3 + p * 2;
        for (int g = 0; g < pathways[p].gene_count && g < MAX_GENES; g++) {
            pathways[p].genes[g] = (g * 3 + p * 7) % (bio.gene_count > 0 ? bio.gene_count : 1);
        }
        pathways[p].flux = 0.5 + p * 0.3;
        pathways[p].name[0] = 'P'; pathways[p].name[1] = 'a';
        pathways[p].name[2] = 't'; pathways[p].name[3] = 'h';
        pathways[p].name[4] = '_'; pathways[p].name[5] = '0' + p;
        pathways[p].name[6] = '\0';
    }
    print_str("[BIO] Pathway analysis: "); print_int(bio.pathway_count); print_str(" pathways\n");
    for (int p = 0; p < bio.pathway_count; p++) {
        print_str("  "); print_str(pathways[p].name);
        print_str(": type="); print_int(pathways[p].type);
        print_str(" genes="); print_int(pathways[p].gene_count);
        print_str(" flux="); print_int((int)(pathways[p].flux * 100));
        print_str("\n");
    }
}

void bio_phylogenetic_tree(void) {
    phylo.species_count = bio.seq_count < MAX_SPECIES ? bio.seq_count : MAX_SPECIES;
    for (int i = 0; i < phylo.species_count; i++) {
        phylo.seq_ids[i] = i;
        for (int j = 0; j < phylo.species_count; j++) {
            if (i == j) { phylo.distances[i][j] = 0.0; phylo.tree_matrix[i][j] = 0; continue; }
            alignment_t aln = bio_align_pair(i, j, ALIGN_GLOBAL);
            phylo.distances[i][j] = 1.0 - aln.identity / 100.0;
            phylo.tree_matrix[i][j] = aln.identity;
        }
    }
    print_str("[BIO] Phylogenetic tree ("); print_int(phylo.species_count); print_str(" species):\n");
    for (int i = 0; i < phylo.species_count && i < 4; i++) {
        print_str("  Species "); print_int(i); print_str(": ");
        for (int j = 0; j < phylo.species_count && j < 4; j++) {
            print_int((int)(phylo.distances[i][j] * 100));
            if (j < phylo.species_count - 1 && j < 3) print_str(" ");
        }
        print_str("\n");
    }
}

void bio_print_state(void) {
    print_str("[BIO] Seqs="); print_int(bio.seq_count);
    print_str(" Vars="); print_int(bio.var_count);
    print_str(" Genes="); print_int(bio.gene_count);
    print_str(" Pathways="); print_int(bio.pathway_count);
    print_str(" Folds="); print_int(bio.fold_count);
    print_str(" Bases="); print_int(bio.total_bases);
    print_str(" AvgId="); print_int((int)bio.avg_identity);
    print_str("%\n");
}

int main(void) {
    print_str("=== Bioinformatics Pipeline Demo ===\n\n");
    bio_init();

    print_str("Loading genome reference...\n");
    char genome[MAX_SEQ_LEN];
    const char* bases = "ATCG";
    for (int i = 0; i < 512; i++) {
        genome[i] = bases[(i * 7 + 3) % 4];
    }
    bio_add_sequence(genome, 512, SEQ_DNA, "Reference");

    print_str("Loading sample genome...");
    char sample[MAX_SEQ_LEN];
    for (int i = 0; i < 512; i++) {
        sample[i] = genome[i];
        if (i % 47 == 0) sample[i] = bases[(i * 3 + 1) % 4];
    }
    bio_add_sequence(sample, 512, SEQ_DNA, "Sample");

    print_str("Loading RNA sequence...\n");
    char rna[MAX_SEQ_LEN];
    const char* rna_bases = "AUGC";
    for (int i = 0; i < 256; i++) {
        rna[i] = rna_bases[(i * 11 + 5) % 4];
    }
    bio_add_sequence(rna, 256, SEQ_RNA, "mRNA_seq1");

    print_str("Loading protein sequence...\n");
    char protein[MAX_SEQ_LEN];
    const char* aa = "ACDEFGHIKLMNPQRSTVWY";
    for (int i = 0; i < 128; i++) {
        protein[i] = aa[(i * 13 + 7) % 20];
    }
    bio_add_sequence(protein, 128, SEQ_PROTEIN, "Protein_X");

    print_str("\nPairwise alignment (global):\n");
    alignment_t aln = bio_align_pair(0, 1, ALIGN_GLOBAL);
    print_str("  Score="); print_int(aln.score);
    print_str(" Matches="); print_int(aln.matches);
    print_str(" Identity="); print_int(aln.identity); print_str("%\n");

    print_str("Local alignment:\n");
    aln = bio_align_pair(0, 1, ALIGN_LOCAL);
    print_str("  Score="); print_int(aln.score);
    print_str(" Matches="); print_int(aln.matches);
    print_str("\n");

    print_str("\nBLAST search:\n");
    bio_blast_search(2, 0, 4);

    print_str("\nVariant detection:\n");
    bio_detect_variants(0, 1);

    print_str("\nGene annotation:\n");
    bio_annotate_genes();

    print_str("\nProtein folding (Monte Carlo):\n");
    bio_fold_protein(3, FOLD_MC);
    print_str("Protein folding (Simulated Annealing):\n");
    bio_fold_protein(3, FOLD_SA);

    print_str("\nPathway analysis:\n");
    bio_pathway_analysis();

    print_str("\nPhylogenetic tree:\n");
    bio_phylogenetic_tree();

    bio_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
