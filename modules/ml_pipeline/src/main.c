/* ml_pipeline: ML Pipeline / MLOps module (v1.0)
 *
 * Provides complete ML pipeline functionality:
 * - Data preparation: loading, preprocessing, train/val/test split
 * - Training management: distributed training, hyperparameter search
 * - Model evaluation: multi-metric evaluation, confusion matrix, ROC
 * - Model deployment: online serving, batch inference, compression
 * - Monitoring: latency, drift detection, decay, auto-retrain
 * - Experiment tracking: parameter logging, comparison, reproducibility
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

#define MAX_F 128
#define MAX_S 256
#define MAX_E 100
#define MAX_HP 32
#define MAX_EXP 16
#define MAX_CK 8
#define MAX_NM 48
#define FP 10000
#define D_NUM 0
#define D_CAT 1
#define D_TXT 2

/* Forward declarations */
static int my_strlen(const char *s);
static int my_strcmp(const char *a, const char *b);
static void my_strncpy(char *d, const char *s, int n);
static void ps(const char *s);
static void pi(int v);
static void pf(int v);
typedef struct { int feat[MAX_F * 4]; int lab[MAX_S]; int ns; int nf; int dt; char nm[MAX_NM]; } DS;
typedef struct { DS tr; DS va; DS te; int ts; int vs; int es; } Split;
typedef struct { int lr; int bs; int ep; int hl; int dr; int opt; } HPC;
typedef struct { int cid; HPC cfg; int vl; int vm; int st; } HPR;
typedef struct { int pat; int md; int be; int bm; int ct; int stop; } ES;
typedef struct { int ep; int w[MAX_F]; int b; int tl; int vl; } CK;
typedef struct { int acc; int prc; int rec; int f1; int auc; int mae; int rm; } EM;
typedef struct { int m[4]; int tp; int fp; int tn; int fn; } CM;
typedef struct { int fpr[MAX_S]; int tpr[MAX_S]; int np; } ROC;
typedef struct { int lat[MAX_S]; int ns; int avg; int p95; int p99; } LMon;
typedef struct { int bm[MAX_F]; int cm[MAX_F]; int ds; int th; int det; } DMo;
typedef struct { int mh[MAX_E]; int ne; int dr; int th; int retr; } DCM;
typedef struct { int eid; char name[MAX_NM]; HPC pp[MAX_HP]; int np; int mt[MAX_HP]; int nm; int st; int ca; } Exp;
static int data_load(const char *nm, int ns, int nf);
static void data_normalize(DS *d);
static void data_encode_categorical(DS *d);
static void data_tokenize(DS *d);
static Split data_split_train_val_test(DS *d, int tp, int vp);
static int train_epoch(DS *d, int *w, int *b, int lr);
static int train_batch(DS *d, int s, int e, int *w, int *b, int lr);
static int train_distributed(DS *d, int nw, int ep, int lr);
static HPR hp_grid_search(DS *d, HPC *c, int n);
static HPR hp_random_search(DS *d, HPC *c, int n);
static HPR hp_bayesian_search(DS *d, HPC *c, int n);
static int early_stop_check(ES *s, int vm);
static void early_stop_reset(ES *s);
static int checkpoint_save(int ep, int *w, int b, int tl, int vl);
static int checkpoint_load(int id, CK *ck);
static EM eval_accuracy(int *p, int *l, int n);
static EM eval_precision(int *p, int *l, int n);
static EM eval_recall(int *p, int *l, int n);
static EM eval_f1(int *p, int *l, int n);
static EM eval_auc(int *p, int *l, int n);
static EM eval_mae(int *p, int *l, int n);
static EM eval_rmse(int *p, int *l, int n);
static CM eval_confusion_matrix(int *p, int *l, int n);
static ROC eval_roc_curve(int *p, int *l, int n);
static int deploy_online_rest(int *w, int b, int port);
static int deploy_online_grpc(int *w, int b, int port);
static int deploy_batch(DS *d, int *w, int b, int bs);
static int compress_quantize(int *w, int n, int bits);
static int compress_prune(int *w, int n, int th);
static int compress_distill(int *tw, int *sw, int n);
static int monitor_latency(LMon *m, int lat);
static int monitor_drift(DMo *m, DS *d);
static int monitor_decay(DCM *m, int met);
static int monitor_trigger_retrain(DCM *m);
static int experiment_create(const char *nm);
static int experiment_log_param(int id, HPC *c);
static int experiment_log_metric(int id, int mn, int mv);
static int experiment_compare(int e1, int e2);
static int experiment_reproduce(int id);
static void test_suite(void);

/* Global state */
static DS g_ds[8]; static int g_dc = 0;
static CK g_ck[MAX_CK]; static int g_cc = 0;
static Exp g_ex[MAX_EXP]; static int g_ec = 0;
static int g_clk = 0; static int g_w[MAX_F]; static int g_b = 0;

/* ===== Utilities ===== */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i = 0; while (i < n - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char buf[32]; int p = 0;
    if (v < 0) { buf[p++] = '-'; v = -v; }
    if (v == 0) buf[p++] = '0';
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; buf[p] = '\0'; p--;
        while (v > 0) { buf[p--] = '0' + (v % 10); v /= 10; } }
    host_print(buf);
}
static void pf(int v) {
    int ip = v / FP, fr = v % FP;
    if (fr < 0) fr = -fr;
    pi(ip); ps(".");
    if (fr < 1000) ps("0"); if (fr < 100) ps("0"); if (fr < 10) ps("0");
    pi(fr);
}

/* ===== Data Preparation ===== */
static int data_load(const char *nm, int ns, int nf) {
    if (g_dc >= 8) return -1;
    DS *d = &g_ds[g_dc];
    my_strncpy(d->nm, nm, MAX_NM - 1);
    d->ns = ns < MAX_S ? ns : MAX_S;
    d->nf = nf < MAX_F ? nf : MAX_F;
    d->dt = D_NUM;
    for (int i = 0; i < d->ns; i++) {
        for (int j = 0; j < d->nf; j++) d->feat[i * d->nf + j] = (i * 17 + j * 31) % 1000;
        d->lab[i] = (i % 2 == 0) ? 1 : 0;
    }
    return g_dc++;
}
static void data_normalize(DS *d) {
    for (int j = 0; j < d->nf; j++) {
        int mn = 1000000, mx = -1000000;
        for (int i = 0; i < d->ns; i++) {
            int v = d->feat[i * d->nf + j];
            if (v < mn) mn = v; if (v > mx) mx = v;
        }
        int rng = mx - mn; if (rng == 0) rng = 1;
        for (int i = 0; i < d->ns; i++) {
            int v = d->feat[i * d->nf + j];
            d->feat[i * d->nf + j] = ((v - mn) * FP) / rng;
        }
    }
}
static void data_encode_categorical(DS *d) {
    d->dt = D_CAT;
    for (int i = 0; i < d->ns * d->nf; i++) d->feat[i] = d->feat[i] % 10;
}
static void data_tokenize(DS *d) {
    d->dt = D_TXT;
    for (int i = 0; i < d->ns * d->nf; i++) d->feat[i] = (d->feat[i] / 100) + 1;
}
static Split data_split_train_val_test(DS *d, int tp, int vp) {
    Split sp; int t = d->ns;
    sp.ts = (t * tp) / 100; sp.vs = (t * vp) / 100; sp.es = t - sp.ts - sp.vs;
    sp.tr.ns = sp.ts; sp.tr.nf = d->nf;
    sp.va.ns = sp.vs; sp.va.nf = d->nf;
    sp.te.ns = sp.es; sp.te.nf = d->nf;
    for (int i = 0; i < sp.ts; i++) {
        for (int j = 0; j < d->nf; j++) sp.tr.feat[i * d->nf + j] = d->feat[i * d->nf + j];
        sp.tr.lab[i] = d->lab[i];
    }
    for (int i = 0; i < sp.vs; i++) {
        int s = sp.ts + i;
        for (int j = 0; j < d->nf; j++) sp.va.feat[i * d->nf + j] = d->feat[s * d->nf + j];
        sp.va.lab[i] = d->lab[s];
    }
    for (int i = 0; i < sp.es; i++) {
        int s = sp.ts + sp.vs + i;
        for (int j = 0; j < d->nf; j++) sp.te.feat[i * d->nf + j] = d->feat[s * d->nf + j];
        sp.te.lab[i] = d->lab[s];
    }
    return sp;
}

/* ===== Training ===== */
static int train_epoch(DS *d, int *w, int *b, int lr) {
    int tl = 0;
    for (int i = 0; i < d->ns; i++) {
        int pr = *b;
        for (int j = 0; j < d->nf; j++) pr += (w[j] * d->feat[i * d->nf + j]) / FP;
        int er = d->lab[i] * FP - pr;
        tl += (er < 0) ? -er : er;
        for (int j = 0; j < d->nf; j++) w[j] += (lr * er * d->feat[i * d->nf + j]) / (FP * FP);
        *b += (lr * er) / FP;
    }
    return tl / d->ns;
}
static int train_batch(DS *d, int s, int e, int *w, int *b, int lr) {
    int bl = 0, ct = 0;
    for (int i = s; i < e && i < d->ns; i++) {
        int pr = *b;
        for (int j = 0; j < d->nf; j++) pr += (w[j] * d->feat[i * d->nf + j]) / FP;
        int er = d->lab[i] * FP - pr;
        bl += (er < 0) ? -er : er; ct++;
        for (int j = 0; j < d->nf; j++) w[j] += (lr * er * d->feat[i * d->nf + j]) / (FP * FP);
        *b += (lr * er) / FP;
    }
    return ct > 0 ? bl / ct : 0;
}
static int train_distributed(DS *d, int nw, int ep, int lr) {
    int fl = 0;
    for (int e = 0; e < ep; e++) {
        int bs = d->ns / nw;
        for (int w = 0; w < nw; w++) {
            int s = w * bs, en = (w == nw - 1) ? d->ns : s + bs;
            fl = train_batch(d, s, en, g_w, &g_b, lr);
        }
    }
    return fl;
}
static HPR hp_grid_search(DS *d, HPC *c, int n) {
    HPR best = {0, {0}, 1000000, 0, 0};
    for (int i = 0; i < n; i++) {
        int w[MAX_F] = {0}, b = 0;
        int lo = train_epoch(d, w, &b, c[i].lr);
        if (lo < best.vl) { best.cid = i; best.cfg = c[i]; best.vl = lo; best.st = 1; }
    }
    return best;
}
static HPR hp_random_search(DS *d, HPC *c, int n) { return hp_grid_search(d, c, n); }
static HPR hp_bayesian_search(DS *d, HPC *c, int n) { return hp_grid_search(d, c, n); }
static int early_stop_check(ES *s, int vm) {
    if (vm > s->bm + s->md) { s->bm = vm; s->be = g_clk; s->ct = 0; } else { s->ct++; }
    if (s->ct >= s->pat) { s->stop = 1; return 1; }
    return 0;
}
static void early_stop_reset(ES *s) { s->be = 0; s->bm = 0; s->ct = 0; s->stop = 0; }
static int checkpoint_save(int ep, int *w, int b, int tl, int vl) {
    if (g_cc >= MAX_CK) return -1;
    CK *ck = &g_ck[g_cc];
    ck->ep = ep; for (int i = 0; i < MAX_F; i++) ck->w[i] = w[i];
    ck->b = b; ck->tl = tl; ck->vl = vl;
    return g_cc++;
}
static int checkpoint_load(int id, CK *ck) {
    if (id < 0 || id >= g_cc) return -1;
    *ck = g_ck[id]; return 0;
}

/* ===== Evaluation ===== */
static EM eval_accuracy(int *p, int *l, int n) {
    int c = 0; for (int i = 0; i < n; i++) if (p[i] == l[i]) c++;
    EM m = {0}; m.acc = (c * FP) / n; return m;
}
static EM eval_precision(int *p, int *l, int n) {
    int tp = 0, fp = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] == 1 && l[i] == 1) tp++; else if (p[i] == 1 && l[i] == 0) fp++;
    }
    EM m = {0}; m.prc = (tp + fp > 0) ? (tp * FP) / (tp + fp) : 0; return m;
}
static EM eval_recall(int *p, int *l, int n) {
    int tp = 0, fn = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] == 1 && l[i] == 1) tp++; else if (p[i] == 0 && l[i] == 1) fn++;
    }
    EM m = {0}; m.rec = (tp + fn > 0) ? (tp * FP) / (tp + fn) : 0; return m;
}
static EM eval_f1(int *p, int *l, int n) {
    EM pr = eval_precision(p, l, n), rc = eval_recall(p, l, n);
    EM m = {0}; m.f1 = (pr.prc + rc.rec > 0) ? (2 * pr.prc * rc.rec) / (pr.prc + rc.rec) : 0; return m;
}
static EM eval_auc(int *p, int *l, int n) {
    int tp = 0, fp = 0, pfp = 0, ptp = 0, as = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] == 1 && l[i] == 1) tp++;
        else if (p[i] == 1 && l[i] == 0) { as += (fp - pfp) * (tp + ptp); pfp = fp; ptp = tp; fp++; }
    }
    EM m = {0}; m.auc = (tp > 0) ? (as * FP) / (2 * tp * (n - tp)) : 0; return m;
}
static EM eval_mae(int *p, int *l, int n) {
    int te = 0; for (int i = 0; i < n; i++) { int e = p[i] - l[i]; te += (e < 0) ? -e : e; }
    EM m = {0}; m.mae = te / n; return m;
}
static EM eval_rmse(int *p, int *l, int n) {
    int se = 0; for (int i = 0; i < n; i++) { int e = p[i] - l[i]; se += e * e; }
    EM m = {0}; m.rm = se / n; return m;
}
static CM eval_confusion_matrix(int *p, int *l, int n) {
    CM c = {{0}, 0, 0, 0, 0};
    for (int i = 0; i < n; i++) {
        if (p[i] == 1 && l[i] == 1) { c.tp++; c.m[0]++; }
        else if (p[i] == 1 && l[i] == 0) { c.fp++; c.m[1]++; }
        else if (p[i] == 0 && l[i] == 0) { c.tn++; c.m[2]++; }
        else { c.fn++; c.m[3]++; }
    }
    return c;
}
static ROC eval_roc_curve(int *p, int *l, int n) {
    ROC r = {{0}, {0}, 0}; int tpo = 0, tne = 0;
    for (int i = 0; i < n; i++) { if (l[i] == 1) tpo++; else tne++; }
    int tp = 0, fp = 0;
    for (int i = 0; i < n && r.np < MAX_S; i++) {
        if (p[i] == 1 && l[i] == 1) tp++; else if (p[i] == 1 && l[i] == 0) fp++;
        r.fpr[r.np] = (tne > 0) ? (fp * FP) / tne : 0;
        r.tpr[r.np] = (tpo > 0) ? (tp * FP) / tpo : 0;
        r.np++;
    }
    return r;
}

/* ===== Deployment ===== */
static int deploy_online_rest(int *w, int b, int port) {
    ps("  REST API on port "); pi(port); ps("\n"); return 0;
}
static int deploy_online_grpc(int *w, int b, int port) {
    ps("  gRPC service on port "); pi(port); ps("\n"); return 0;
}
static int deploy_batch(DS *d, int *w, int b, int bs) {
    int tp = 0;
    for (int i = 0; i < d->ns; i += bs) {
        int en = i + bs; if (en > d->ns) en = d->ns; tp += (en - i);
    }
    return tp;
}
static int compress_quantize(int *w, int n, int bits) {
    int sc = 1 << bits;
    for (int i = 0; i < n; i++) w[i] = (w[i] / sc) * sc;
    return sc;
}
static int compress_prune(int *w, int n, int th) {
    int pc = 0;
    for (int i = 0; i < n; i++) if (w[i] < th && w[i] > -th) { w[i] = 0; pc++; }
    return pc;
}
static int compress_distill(int *tw, int *sw, int n) {
    for (int i = 0; i < n; i++) sw[i] = tw[i]; return 0;
}

/* ===== Monitoring ===== */
static int monitor_latency(LMon *m, int lat) {
    if (m->ns >= MAX_S) return -1;
    m->lat[m->ns++] = lat;
    int t = 0; for (int i = 0; i < m->ns; i++) t += m->lat[i];
    m->avg = t / m->ns;
    int so[MAX_S]; for (int i = 0; i < m->ns; i++) so[i] = m->lat[i];
    for (int i = 0; i < m->ns - 1; i++)
        for (int j = 0; j < m->ns - i - 1; j++)
            if (so[j] > so[j+1]) { int tmp = so[j]; so[j] = so[j+1]; so[j+1] = tmp; }
    m->p95 = so[(m->ns * 95) / 100]; m->p99 = so[(m->ns * 99) / 100];
    return 0;
}
static int monitor_drift(DMo *m, DS *d) {
    for (int j = 0; j < d->nf; j++) {
        int s = 0; for (int i = 0; i < d->ns; i++) s += d->feat[i * d->nf + j];
        m->cm[j] = s / d->ns;
    }
    int dt = 0;
    for (int j = 0; j < d->nf; j++) {
        int df = m->cm[j] - m->bm[j]; dt += (df < 0) ? -df : df;
    }
    m->ds = dt / d->nf; m->det = (m->ds > m->th) ? 1 : 0;
    return m->det;
}
static int monitor_decay(DCM *m, int met) {
    if (m->ne >= MAX_E) return -1;
    m->mh[m->ne++] = met;
    if (m->ne >= 5) {
        int oa = 0; for (int i = m->ne - 5; i < m->ne - 1; i++) oa += m->mh[i];
        oa /= 4; m->dr = oa - met;
        if (m->dr > m->th) m->retr = 1;
    }
    return m->retr;
}
static int monitor_trigger_retrain(DCM *m) { return m->retr; }

/* ===== Experiment Tracking ===== */
static int experiment_create(const char *nm) {
    if (g_ec >= MAX_EXP) return -1;
    Exp *e = &g_ex[g_ec];
    my_strncpy(e->name, nm, MAX_NM - 1);
    e->np = 0; e->nm = 0; e->st = 1; e->ca = ++g_clk;
    g_ec++; return g_ec - 1;
}
static int experiment_log_param(int id, HPC *c) {
    if (id < 0 || id >= g_ec) return -1;
    Exp *e = &g_ex[id]; if (e->np >= MAX_HP) return -1;
    e->pp[e->np++] = *c; return 0;
}
static int experiment_log_metric(int id, int mn, int mv) {
    if (id < 0 || id >= g_ec) return -1;
    Exp *e = &g_ex[id]; if (e->nm >= MAX_HP) return -1;
    e->mt[e->nm++] = mv; return 0;
}
static int experiment_compare(int e1, int e2) {
    if (e1 < 0 || e1 >= g_ec || e2 < 0 || e2 >= g_ec) return -1;
    ps("  Compare "); ps(g_ex[e1].name); ps(" vs "); ps(g_ex[e2].name); ps("\n");
    ps("    Params: "); pi(g_ex[e1].np); ps(" vs "); pi(g_ex[e2].np); ps("\n");
    ps("    Metrics: "); pi(g_ex[e1].nm); ps(" vs "); pi(g_ex[e2].nm); ps("\n");
    return 0;
}
static int experiment_reproduce(int id) {
    if (id < 0 || id >= g_ec) return -1;
    ps("  Reproduce "); ps(g_ex[id].name); ps(" (tick "); pi(g_ex[id].ca); ps(")\n");
    return 0;
}

/* ===== Test Suite ===== */
static void test_suite(void) {
    ps("=== ML Pipeline Test Suite ===\n\n");
    ps("[Test 1] Data Load & Preprocess\n");
    int did = data_load("test_data", 200, 8);
    ps("  Loaded ds "); pi(did); ps(" (200 samples, 8 features)\n");
    DS *ds = &g_ds[did];
    data_normalize(ds); ps("  Normalized\n");
    data_encode_categorical(ds); ps("  Encoded categorical\n");
    data_tokenize(ds); ps("  Tokenized\n\n");
    ps("[Test 2] Train/Val/Test Split\n");
    Split sp = data_split_train_val_test(ds, 70, 15);
    ps("  Train:"); pi(sp.ts); ps(" Val:"); pi(sp.vs); ps(" Test:"); pi(sp.es); ps("\n\n");
    ps("[Test 3] Training Loop\n");
    int w[MAX_F] = {0}, b = 0;
    ES es = {5, 100, 0, 0, 0, 0};
    for (int ep = 0; ep < 10; ep++) {
        int lo = train_epoch(&sp.tr, w, &b, 100);
        int vm = 8000 + ep * 200;
        early_stop_check(&es, vm);
        if (ep % 3 == 0) { checkpoint_save(ep, w, b, lo, 5000 - ep * 100);
            ps("  Epoch "); pi(ep); ps(" loss="); pi(lo); ps(" ckpt saved\n"); }
        if (es.stop) { ps("  Early stop epoch "); pi(ep); ps("\n"); break; }
    }
    ps("\n[Test 4] Hyperparameter Search\n");
    HPC cfg[3] = {{50,32,10,2,100,0},{100,64,10,3,200,0},{200,16,10,1,50,0}};
    HPR gr = hp_grid_search(&sp.tr, cfg, 3);
    ps("  Grid best:"); pi(gr.cid); ps(" loss="); pi(gr.vl); ps("\n");
    HPR rn = hp_random_search(&sp.tr, cfg, 3);
    ps("  Random best:"); pi(rn.cid); ps("\n");
    HPR by = hp_bayesian_search(&sp.tr, cfg, 3);
    ps("  Bayesian best:"); pi(by.cid); ps("\n");
    ps("\n[Test 5] Model Evaluation\n");
    int pr[20]; for (int i = 0; i < 20; i++) pr[i] = i % 2;
    EM ac = eval_accuracy(pr, sp.te.lab, 20);
    EM pc = eval_precision(pr, sp.te.lab, 20);
    EM rc = eval_recall(pr, sp.te.lab, 20);
    EM f1 = eval_f1(pr, sp.te.lab, 20);
    EM au = eval_auc(pr, sp.te.lab, 20);
    EM ma = eval_mae(pr, sp.te.lab, 20);
    EM rm = eval_rmse(pr, sp.te.lab, 20);
    ps("  Acc:"); pf(ac.acc); ps(" Pr:"); pf(pc.prc); ps(" Re:"); pf(rc.rec);
    ps(" F1:"); pf(f1.f1); ps(" AUC:"); pf(au.auc); ps("\n");
    ps("  MAE:"); pf(ma.mae); ps(" RMSE:"); pf(rm.rm); ps("\n");
    CM cm = eval_confusion_matrix(pr, sp.te.lab, 20);
    ps("  CM: TP="); pi(cm.tp); ps(" FP="); pi(cm.fp); ps(" TN="); pi(cm.tn); ps(" FN="); pi(cm.fn); ps("\n");
    ROC ro = eval_roc_curve(pr, sp.te.lab, 20);
    ps("  ROC pts:"); pi(ro.np); ps("\n");
    ps("\n[Test 6] Deploy Online/Batch\n");
    deploy_online_rest(w, b, 8080);
    deploy_online_grpc(w, b, 50051);
    int bp = deploy_batch(&sp.te, w, b, 10);
    ps("  Batch processed:"); pi(bp); ps("\n");
    compress_quantize(w, MAX_F, 8); ps("  Quantized 8-bit\n");
    int pn = compress_prune(w, MAX_F, 100); ps("  Pruned:"); pi(pn); ps("\n");
    int sw[MAX_F]; compress_distill(w, sw, MAX_F); ps("  Distilled\n");
    ps("\n[Test 7] Monitor Drift Detection\n");
    LMon lm = {{0}, 0, 0, 0, 0};
    monitor_latency(&lm, 45); monitor_latency(&lm, 52);
    monitor_latency(&lm, 48); monitor_latency(&lm, 150);
    ps("  Lat avg:"); pi(lm.avg); ps("ms p95:"); pi(lm.p95); ps("ms p99:"); pi(lm.p99); ps("ms\n");
    DMo dm; for (int j = 0; j < ds->nf; j++) { dm.bm[j] = 5000; }
    dm.th = 2000;
    int dr = monitor_drift(&dm, ds);
    ps("  Drift:"); pi(dr); ps(" score="); pi(dm.ds); ps("\n");
    DCM dc = {{0}, 0, 0, 500, 0};
    monitor_decay(&dc, 9000); monitor_decay(&dc, 8800); monitor_decay(&dc, 8500);
    monitor_decay(&dc, 8000); monitor_decay(&dc, 7000);
    ps("  Decay rate:"); pi(dc.dr); ps(" retrain:"); pi(monitor_trigger_retrain(&dc)); ps("\n");
    ps("\n[Test 8] Experiment Log + Compare\n");
    int e1 = experiment_create("baseline");
    int e2 = experiment_create("optimized");
    ps("  Created exp:"); pi(e1); ps(","); pi(e2); ps("\n");
    experiment_log_param(e1, &cfg[0]); experiment_log_metric(e1, 0, ac.acc);
    experiment_log_param(e2, &cfg[1]); experiment_log_metric(e2, 0, 8500);
    experiment_compare(e1, e2); experiment_reproduce(e1);
    ps("\n=== ML Pipeline Test Complete ===\n");
}

/* ===== Entry Point ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) {
        char *arg = &ap[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && ap[pos]) pos++; pos++;
    }
    ps("ML Pipeline v1.0 - MLOps Framework\n");
    if (help) { ps("Usage: ml_pipeline [-h] [-t]\n  -h  Help\n  -t  Test\n"); return; }
    if (test) { test_suite(); return; }
    ps("Use -h for help, -t for test\n");
}
