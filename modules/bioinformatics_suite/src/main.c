/* Bioinformatics Suite WASM Module
 * Gene analysis, protein folding, drug discovery, genome assembly,
 * evolutionary analysis, high-throughput data processing. */

__attribute__((import_module("host"), import_name("alloc")))
extern void *host_alloc(unsigned long size, unsigned long align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *msg);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(char *buf, int buf_len);

#define BUF_SIZE 512
#define MAX_SEQ  256
#define MAX_READ 128
#define MAX_LINE 256
#define NUM_AA   20
#define SCORE_MATCH 2
#define SCORE_MISMATCH -1
#define SCORE_GAP -2

/* ── String Helpers ─────────────────────────────────────────────────── */

static void str_copy(char *d, const char *s, int n) {
    int i = 0; while (i < n-1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static int str_cmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; } return (unsigned char)*a - (unsigned char)*b;
}
static int str_eq(const char *a, const char *b) { return str_cmp(a, b) == 0; }
static int str_len(const char *s) { int n = 0; while (s[n]) n++; return n; }
static int str_has(const char *s, char c) {
    while (*s) { if (*s == c) return 1; s++; } return 0;
}
static int to_upper(int c) { return (c >= 'a' && c <= 'z') ? c - 32 : c; }

/* ── Buffer Writer ──────────────────────────────────────────────────── */

typedef struct { char data[BUF_SIZE]; int pos; } Buffer;
static void buf_init(Buffer *b) { b->pos = 0; b->data[0] = '\0'; }
static void buf_putc(Buffer *b, char c) {
    if (b->pos < BUF_SIZE-1) { b->data[b->pos++] = c; b->data[b->pos] = '\0'; }
}
static void buf_puts(Buffer *b, const char *s) { while (*s) buf_putc(b, *s++); }
static void buf_puti(Buffer *b, int v) {
    char tmp[12]; int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) { buf_putc(b, '0'); return; }
    while (v > 0) { tmp[i++] = '0' + (v % 10); v /= 10; }
    if (neg) buf_putc(b, '-');
    while (i > 0) buf_putc(b, tmp[--i]);
}
static void buf_putf(Buffer *b, const char *s, int v) { buf_puts(b, s); buf_puti(b, v); }
static void buf_flush(Buffer *b) { host_print(b->data); buf_init(b); }

/* ── FASTA / FASTQ Parsing ──────────────────────────────────────────── */

typedef struct { char name[MAX_LINE]; char seq[MAX_SEQ]; int len; } FastaRec;

static int parse_fasta(const char *input, FastaRec *rec) {
    int i = 0, slen = 0;
    rec->name[0] = '\0'; rec->seq[0] = '\0'; rec->len = 0;
    if (!input || input[0] != '>') return -1;
    i = 1;
    while (input[i] && input[i] != '\n' && slen < MAX_LINE-1)
        rec->name[slen++] = input[i++];
    rec->name[slen] = '\0';
    if (input[i] == '\n') i++;
    slen = 0;
    while (input[i] && slen < MAX_SEQ-1) {
        if (input[i] == '\n' || input[i] == '\r') { i++; continue; }
        if (input[i] == '>') break;
        char c = (char)to_upper(input[i]);
        if (str_has("ATCGUNRYSWKMBDHV", c)) rec->seq[slen++] = c;
        i++;
    }
    rec->seq[slen] = '\0'; rec->len = slen; return 0;
}

static int parse_fastq(const char *input, FastaRec *rec, int *qual) {
    int i = 0, slen = 0, line = 0;
    rec->name[0] = '\0'; rec->seq[0] = '\0'; rec->len = 0;
    if (!input || input[0] != '@') return -1;
    i = 1;
    while (input[i] && input[i] != '\n' && slen < MAX_LINE-1)
        rec->name[slen++] = input[i++];
    rec->name[slen] = '\0';
    if (input[i] == '\n') i++;
    slen = 0; line = 1;
    while (input[i] && slen < MAX_SEQ-1) {
        if (input[i] == '\n' || input[i] == '\r') { i++; line++; continue; }
        if (line == 3) break;
        if (line == 1) {
            char c = (char)to_upper(input[i]);
            if (str_has("ATCGUN", c)) rec->seq[slen++] = c;
        }
        i++;
    }
    rec->seq[slen] = '\0'; rec->len = slen;
    while (input[i] && input[i] != '\n') i++;
    if (input[i] == '\n') i++;
    slen = 0;
    while (input[i] && input[i] != '\n' && slen < rec->len)
        qual[slen++] = input[i++] - 33;
    return 0;
}

/* ── BLAST-style Alignment (Smith-Waterman) ─────────────────────────── */

typedef struct { int score; int start_a; int start_b; int length; } AlignRes;

static AlignRes smith_waterman(const char *a, int la, const char *b, int lb) {
    AlignRes res = {0, 0, 0, 0};
    static int dp[MAX_SEQ][MAX_SEQ], trace[MAX_SEQ][MAX_SEQ];
    int i, j, max_i = 0, max_j = 0;
    for (i = 0; i <= la; i++)
        for (j = 0; j <= lb; j++) { dp[i][j] = 0; trace[i][j] = 0; }
    for (i = 1; i <= la; i++) {
        for (j = 1; j <= lb; j++) {
            int m = dp[i-1][j-1] + (a[i-1] == b[j-1] ? SCORE_MATCH : SCORE_MISMATCH);
            int ga = dp[i-1][j] + SCORE_GAP, gb = dp[i][j-1] + SCORE_GAP, best = 0;
            if (m > best) { best = m; trace[i][j] = 1; }
            if (ga > best) { best = ga; trace[i][j] = 2; }
            if (gb > best) { best = gb; trace[i][j] = 3; }
            dp[i][j] = best;
            if (best > res.score) { res.score = best; max_i = i; max_j = j; }
        }
    }
    i = max_i; j = max_j;
    while (i > 0 && j > 0 && trace[i][j]) {
        if (trace[i][j] == 1) { i--; j--; }
        else if (trace[i][j] == 2) i--;
        else j--;
        res.length++;
    }
    res.start_a = i; res.start_b = j; return res;
}

/* ── Mutation Detection ─────────────────────────────────────────────── */

typedef struct { int snps, ins, dels, matches; } MutRpt;

static MutRpt detect_mutations(const char *ref, int lr, const char *qry, int lq) {
    MutRpt r = {0, 0, 0, 0};
    int i, ml = lr < lq ? lr : lq;
    for (i = 0; i < ml; i++) {
        if (ref[i] == qry[i]) r.matches++; else r.snps++;
    }
    if (lr > lq) r.dels = lr - lq; else if (lq > lr) r.ins = lq - lr;
    return r;
}

/* ── Protein Folding Prediction ─────────────────────────────────────── */

static const char *AA_NAMES = "ACDEFGHIKLMNPQRSTVWY";
static const int AA_HYDRO[20] = {2,3,0,0,3,1,1,4,0,4,2,0,0,0,0,0,1,3,3,2};

static int aa_index(char c) {
    int i; for (i = 0; i < NUM_AA; i++) if (AA_NAMES[i] == c) return i;
    return -1;
}

typedef struct { int helix, sheet, coil; } StructPred;

static StructPred predict_secondary(const char *seq, int len) {
    StructPred p = {0, 0, 0};
    int i, k;
    for (i = 0; i < len - 3; i++) {
        int hydro = 0, cnt = 0;
        for (k = 0; k < 4 && i+k < len; k++) {
            int idx = aa_index(seq[i+k]);
            if (idx >= 0) { hydro += AA_HYDRO[idx]; cnt++; }
        }
        if (!cnt) continue;
        int avg = (hydro * 10) / cnt;
        if (avg >= 25) p.helix++; else if (avg >= 15) p.sheet++; else p.coil++;
    }
    return p;
}

static void aa_composition(Buffer *b, const char *seq, int len) {
    int counts[20] = {0}, i;
    for (i = 0; i < len; i++) { int x = aa_index(seq[i]); if (x >= 0) counts[x]++; }
    buf_puts(b, "  AA composition:");
    for (i = 0; i < NUM_AA; i++) {
        if (counts[i]) {
            buf_putc(b, ' '); buf_putc(b, AA_NAMES[i]); buf_putc(b, ':');
            buf_puti(b, counts[i]);
            buf_putc(b, '('); buf_puti(b, counts[i]*100/(len?len:1));
            buf_puts(b, "%)");
        }
    }
    buf_puts(b, "\n");
}

/* ── Drug Discovery ─────────────────────────────────────────────────── */

typedef struct {
    int atoms, hb_donors, hb_acceptors, rot_bonds, arom_rings;
} MolProp;

static MolProp analyze_mol(const char *smi) {
    MolProp m = {0,0,0,0,0}; int i, len = str_len(smi);
    for (i = 0; i < len; i++) {
        char c = smi[i];
        if ((c>='A'&&c<='Z')||(c>='a'&&c<='z')) m.atoms++;
        if (c=='O'||c=='N') m.hb_acceptors++;
        if (c=='N' && i+1<len && smi[i+1]=='H') m.hb_donors++;
        if (c=='-'||c=='#') m.rot_bonds++;
        if (c=='c'||c=='C') m.arom_rings++;
    }
    m.arom_rings /= 6; m.rot_bonds /= 2; return m;
}

static int docking_score(const char *lig, const char *rec) {
    int sc = 0, i, ll = str_len(lig), lr = str_len(rec), ml = ll<lr?ll:lr;
    for (i = 0; i < ml; i++) sc += (lig[i]==rec[i]) ? 3 : -1;
    return sc + (ll + lr) / 10;
}

typedef struct { int absorption, distribution, metabolism, excretion, toxicity; } ADMET;

static ADMET predict_admet(const char *smi) {
    ADMET r = {70,60,65,55,30}; int i, len = str_len(smi);
    if (len > 50) { r.absorption -= 20; r.toxicity += 15; }
    if (len < 15) { r.absorption += 10; r.excretion += 20; }
    for (i = 0; i < len; i++) {
        if (smi[i]=='O') r.metabolism += 3;
        if (smi[i]=='N') r.toxicity += 2;
        if (smi[i]=='S') r.excretion += 2;
    }
    if (r.absorption>100) r.absorption=100;
    if (r.toxicity>100) r.toxicity=100;
    return r;
}

/* ── Genome Assembly ────────────────────────────────────────────────── */

static int overlap(const char *a, int la, const char *b, int lb, int min_o) {
    int best = 0, mk = la<lb?la:lb, k, j;
    for (k = min_o; k <= mk; k++) {
        int ok = 1;
        for (j = 0; j < k; j++) if (a[la-k+j] != b[j]) { ok=0; break; }
        if (ok && k > best) best = k;
    }
    return best;
}

typedef struct { char contig[MAX_SEQ]; int length, coverage; } Contig;

static Contig assemble_reads(const char reads[][MAX_READ], int nr, int min_o) {
    Contig ct = {{0},0,0}; if (!nr) return ct;
    int used[MAX_READ] = {0}, i;
    str_copy(ct.contig, reads[0], MAX_SEQ);
    ct.length = str_len(reads[0]); used[0] = 1; ct.coverage = 1;
    int changed = 1;
    while (changed) {
        changed = 0; int bo = 0, bi = -1;
        for (i = 1; i < nr; i++) {
            if (used[i]) continue;
            int ov = overlap(ct.contig, ct.length, reads[i], str_len(reads[i]), min_o);
            if (ov > bo) { bo = ov; bi = i; }
        }
        if (bi >= 0) {
            int rl = str_len(reads[bi]), j;
            for (j = bo; j < rl && ct.length < MAX_SEQ-1; j++)
                ct.contig[ct.length++] = reads[bi][j];
            ct.contig[ct.length] = '\0'; used[bi] = 1; ct.coverage++; changed = 1;
        }
    }
    return ct;
}

/* ── Evolutionary Analysis ──────────────────────────────────────────── */

static int hamming(const char *a, const char *b, int len) {
    int d = 0, i; for (i = 0; i < len; i++) if (a[i]!=b[i]) d++; return d;
}

static void build_tree(Buffer *b, const char *seqs[], const char *names[],
                       int nt, int sl) {
    buf_puts(b, "  Phylogenetic tree (NJ summary):\n");
    int i, j, edges = 0;
    for (i = 0; i < nt; i++) {
        int md = 999999, mj = -1;
        for (j = i+1; j < nt; j++) {
            int d = hamming(seqs[i], seqs[j], sl);
            if (d < md) { md = d; mj = j; }
        }
        if (mj >= 0) {
            buf_puts(b, "    "); buf_puts(b, names[i]);
            buf_puts(b, " <-> "); buf_puts(b, names[mj]);
            buf_puts(b, " (d="); buf_puti(b, md); buf_puts(b, ")\n");
            edges++;
        }
    }
    buf_putf(b, "  Edges: ", edges); buf_puts(b, "\n");
}

static int mol_clock_rate(int mut, int time_mya) {
    if (time_mya <= 0) return 0;
    return (mut * 10000) / time_mya; /* rate x10^4, integer */
}

/* ── High-Throughput Data ───────────────────────────────────────────── */

typedef struct { int total, mapped, unmapped, counts[MAX_LINE], ngenes; } RNASeq;

static RNASeq analyze_rnaseq(const char *reads[], const char *genome,
                             int nr, int glen) {
    RNASeq r = {0,0,0,{0},0}; r.total = nr; int i;
    for (i = 0; i < nr && i < MAX_LINE; i++) {
        int rl = str_len(reads[i]), j, found = 0;
        for (j = 0; j <= glen - rl; j++) {
            int ok = 1, k;
            for (k = 0; k < rl; k++) if (reads[i][k] != genome[j+k]) { ok=0; break; }
            if (ok) { found=1; r.counts[j/50]++; break; }
        }
        if (found) r.mapped++; else r.unmapped++;
    }
    r.ngenes = glen/50+1; return r;
}

typedef struct { int start, end, score; } Peak;

static Peak chipseq_peaks(const int cov[], int glen, int thresh) {
    Peak pk = {0,0,0}; int i, inp=0, cs=0, bs=0;
    for (i = 0; i < glen; i++) {
        if (cov[i] >= thresh) {
            if (!inp) { bs=i; cs=0; inp=1; } cs += cov[i];
        } else {
            if (inp && cs > pk.score) { pk.start=bs; pk.end=i; pk.score=cs; }
            inp=0; cs=0;
        }
    }
    if (inp && cs > pk.score) { pk.start=bs; pk.end=glen; pk.score=cs; }
    return pk;
}

typedef struct { char var[32]; int pos; char effect[32]; } VarAnn;

static VarAnn annotate_var(const char *ref, const char *alt, int pos) {
    VarAnn a; a.pos = pos;
    int rl = str_len(ref), al = str_len(alt);
    if (rl==1 && al==1) {
        str_copy(a.var, ref, 8); int vl = str_len(a.var);
        a.var[vl]='/'; a.var[vl+1]='\0'; str_copy(a.var+vl+1, alt, 8);
    } else if (rl > al) str_copy(a.var, "DEL", 32);
    else str_copy(a.var, "INS", 32);
    if (pos%3==0) str_copy(a.effect, "frameshift", 32);
    else if (rl==1 && al==1) str_copy(a.effect, "missense_SNV", 32);
    else str_copy(a.effect, "intergenic", 32);
    return a;
}

/* ── Test Suite ─────────────────────────────────────────────────────── */

static void test_all(Buffer *b) {
    /* Gene Analysis */
    buf_puts(b, "=== Gene Sequence Analysis ===\n");
    FastaRec fr; int qual[MAX_SEQ];
    if (!parse_fasta(">BRCA1_Human\nATGGATTTATCTGCTCTTCGCGTTGA", &fr)) {
        buf_puts(b, "  FASTA: "); buf_puts(b, fr.name);
        buf_putf(b, " len=", fr.len); buf_puts(b, "\n");
    }
    const char *sa = "ATCGATCGATCG", *sb = "ATCGTTCGATCG";
    AlignRes al = smith_waterman(sa, str_len(sa), sb, str_len(sb));
    buf_putf(b, "  Align score=", al.score);
    buf_putf(b, " len=", al.length); buf_puts(b, "\n");
    MutRpt mu = detect_mutations(sa, str_len(sa), sb, str_len(sb));
    buf_putf(b, "  SNPs=", mu.snps); buf_putf(b, " matches=", mu.matches);
    buf_puts(b, "\n");

    /* Protein Folding */
    buf_puts(b, "=== Protein Folding ===\n");
    const char *prot = "MVLSPADKTNVKAAWGKVGAHAGEYGAEALERMFLSFPTTKTYFPHFDLSH";
    aa_composition(b, prot, str_len(prot));
    StructPred sp = predict_secondary(prot, str_len(prot));
    buf_putf(b, "  Helix=", sp.helix); buf_putf(b, " Sheet=", sp.sheet);
    buf_putf(b, " Coil=", sp.coil); buf_puts(b, "\n");

    /* Drug Discovery */
    buf_puts(b, "=== Drug Discovery ===\n");
    const char *lig = "CC(=O)OC1=CC=CC=C1C(=O)O", *rec = "CCOHNC(=O)OC1=CC";
    MolProp mp = analyze_mol(lig);
    buf_putf(b, "  Atoms=", mp.atoms); buf_putf(b, " HBD=", mp.hb_donors);
    buf_putf(b, " HBA=", mp.hb_acceptors); buf_puts(b, "\n");
    buf_putf(b, "  Docking=", docking_score(lig, rec)); buf_puts(b, "\n");
    ADMET ad = predict_admet(lig);
    buf_putf(b, "  ADMET A=", ad.absorption); buf_putf(b, " T=", ad.toxicity);
    buf_puts(b, "\n");

    /* Genome Assembly */
    buf_puts(b, "=== Genome Assembly ===\n");
    static const char rds[5][MAX_READ] = {
        "ATCGATCGATCGATCG", "GATCGATCGATCGTAAA", "GATCGTAAATTTCCCCC",
        "TTTCCCCCCGGGGAAAA", "CCGGGGAAAATTTGGGG"
    };
    Contig ct = assemble_reads(rds, 5, 8);
    buf_putf(b, "  Contig len=", ct.length);
    buf_putf(b, " cov=", ct.coverage); buf_puts(b, "\n");

    /* Evolutionary Analysis */
    buf_puts(b, "=== Evolutionary Analysis ===\n");
    const char *seqs[] = {"ATCGATCGATCG","ATCGTTCGATCG","AACGATCGAACG","ATCGATCGTTTG"};
    const char *nms[] = {"Human","Chimp","Gorilla","Orangutan"};
    build_tree(b, seqs, nms, 4, 12);
    buf_putf(b, "  Mol clock rate(x10^-4)=", mol_clock_rate(42, 6));
    buf_puts(b, "\n");

    /* High-Throughput */
    buf_puts(b, "=== High-Throughput Data ===\n");
    const char *rr[] = {"ATCGATCG","TCGATCGA","GCTAGCTA","ZZZZZZZZZ"};
    const char *gn = "ATCGATCGATCGXYZGCTAGCTAXYZTCGATCGA";
    RNASeq rs = analyze_rnaseq(rr, gn, 4, str_len(gn));
    buf_putf(b, "  RNA-seq total=", rs.total);
    buf_putf(b, " mapped=", rs.mapped);
    buf_putf(b, " unmapped=", rs.unmapped); buf_puts(b, "\n");
    int cov[] = {1,2,5,8,12,15,10,5,2,1,1,3,8,12,14,10,5,2,1,1};
    Peak pk = chipseq_peaks(cov, 20, 8);
    buf_puts(b, "  ChIP-seq peak="); buf_puti(b, pk.start);
    buf_putc(b, '-'); buf_puti(b, pk.end);
    buf_puts(b, " score="); buf_puti(b, pk.score); buf_puts(b, "\n");
    VarAnn va = annotate_var("A", "G", 100);
    buf_puts(b, "  Var: "); buf_puts(b, va.var);
    buf_puts(b, " pos="); buf_puti(b, va.pos);
    buf_puts(b, " ("); buf_puts(b, va.effect); buf_puts(b, ")\n");
}

/* ── Entry Point ────────────────────────────────────────────────────── */

void _start(void) {
    void *mem = host_alloc(BUF_SIZE, 16);
    if (!mem) { host_exit(1); return; }

    char abuf[BUF_SIZE];
    int argc = host_get_argv(abuf, BUF_SIZE);

    Buffer buf; buf_init(&buf);
    int mode = 0; /* 0=help, 1=test */
    if (argc > 0) {
        int p = 0, a;
        for (a = 0; a < argc && p < BUF_SIZE; a++) {
            if (str_eq(abuf+p, "-t")) mode = 1;
            if (str_eq(abuf+p, "-h")) mode = 0;
            p += str_len(abuf+p) + 1;
        }
    }

    if (mode == 0) {
        buf_puts(&buf, "Bioinformatics Suite WASM Module\n");
        buf_puts(&buf, "Usage: -t = run tests, -h = help\n");
        buf_puts(&buf, "Modules:\n");
        buf_puts(&buf, "  1. Gene analysis (FASTA/FASTQ, alignment, mutations)\n");
        buf_puts(&buf, "  2. Protein folding (amino acid, secondary structure)\n");
        buf_puts(&buf, "  3. Drug discovery (docking, pharmacophore, ADMET)\n");
        buf_puts(&buf, "  4. Genome assembly (short-read, scaffolding)\n");
        buf_puts(&buf, "  5. Evolutionary (phylogenetic trees, molecular clock)\n");
        buf_puts(&buf, "  6. High-throughput (RNA-seq, ChIP-seq, variant ann.)\n");
        buf_flush(&buf);
    } else {
        buf_puts(&buf, "Running bioinformatics tests...\n\n"); buf_flush(&buf);
        test_all(&buf);
        buf_puts(&buf, "\nAll tests complete.\n"); buf_flush(&buf);
    }
    host_exit(0);
}
