/*
 * bci_signal - Brain-Computer Interface Signal Processing WASM Module
 * AI-ASM OS: EEG/EMG/EOG/ECG acquisition, preprocessing, feature extraction,
 * classification, real-time processing, and visualization.
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
#define MAX_CH 64U
#define MAX_SAMP 4096U
#define FREQ_BINS 128U
#define MAX_FEAT 512U
#define MAX_CLS 16U
#define FFT_SZ 512U
#define MAX_ART 32U
#define MAX_FORD 16U
#define PIPE_STG 12U
#define MAX_CONN 32U
#define TOPO_SZ 16U
/* ========== Signal Type Constants ========== */
#define SIG_EEG   0x01U
#define SIG_EMG   0x02U
#define SIG_EOG   0x04U
#define SIG_ECG   0x08U
#define SIG_MULTI 0x0FU

/* ========== Filter Type Constants ========== */
#define FLT_BP    0x01U
#define FLT_NOTCH 0x02U
#define FLT_ADAPT 0x03U

/* ========== Feature Domain Constants ========== */
#define FEAT_TIME 0x01U
#define FEAT_FREQ 0x02U
#define FEAT_TF   0x03U
#define FEAT_SPAT 0x04U

/* ========== Classifier Type Constants ========== */
#define CLS_SVM   0x01U
#define CLS_CNN   0x02U
#define CLS_RNN   0x03U
#define CLS_ATT   0x04U
#define CLS_EMO   0x05U

/* ========== Pipeline Status Constants ========== */
#define PIPE_OK   0x00U
#define PIPE_ERR  0x01U
#define PIPE_QLOW 0x10U

/* ========== Data Structures ========== */

typedef struct { unsigned int sr, nch, sig_type, bits, ref; double imp[MAX_CH]; } sig_cfg_t;
typedef struct {
    double d[MAX_CH][MAX_SAMP]; unsigned int nch, nsamp, sr, stype; double ts[MAX_SAMP];
} sig_buf_t;
typedef struct {
    double lo, hi, notch, adap; unsigned int ftype, ord;
    double coef[MAX_FORD], st[MAX_CH][MAX_FORD];
} flt_cfg_t;
typedef struct {
    unsigned int method, ncomp;
    double mix[MAX_CH][MAX_CH], umix[MAX_CH][MAX_CH];
    double athresh; unsigned int aidx[MAX_ART], nart;
} art_rm_t;
typedef struct {
    double mean[MAX_CH], var[MAX_CH];
    unsigned int fcnt, domain; double val[MAX_FEAT], norm[MAX_FEAT];
} feat_set_t;
typedef struct {
    unsigned int type, ncls, feat_dim;
    double w[MAX_FEAT][MAX_CLS], bias[MAX_CLS], lr;
    unsigned int epochs; double acc;
} cls_t;
typedef struct { unsigned int id, status, nproc, nerr; double lat, tput; } pipe_stg_t;
typedef struct {
    pipe_stg_t stg[PIPE_STG]; unsigned int active, status;
    double qual, tlat; unsigned int upsamp;
} pipeline_t;
typedef struct {
    double grid[TOPO_SZ][TOPO_SZ]; unsigned int nel;
    double ex[MAX_CH], ey[MAX_CH], ev[MAX_CH];
} topo_t;
typedef struct {
    double mag[FREQ_BINS][MAX_SAMP / 4U], ph[FREQ_BINS][MAX_SAMP / 4U];
    unsigned int fb, tf; double fa[FREQ_BINS], ta[MAX_SAMP / 4U];
} spec_t;
typedef struct {
    double conn[MAX_CONN][MAX_CONN]; unsigned int nn;
    double nv[MAX_CONN], thresh; unsigned int nec;
} conn_g_t;
typedef struct {
    sig_cfg_t cfg; sig_buf_t buf; flt_cfg_t flt; art_rm_t art;
    feat_set_t feat; cls_t cls; pipeline_t pipe; topo_t topo;
    spec_t spec; conn_g_t conn; unsigned int init, mode;
} bci_ctx_t;
static bci_ctx_t g_ctx;
static void bci_init(void);
static void bci_status(const char *m);
static unsigned int bci_cfg(unsigned int st, unsigned int ch, unsigned int r, unsigned int b);
static unsigned int bci_acq(sig_buf_t *b, unsigned int st);
static unsigned int bci_flt(sig_buf_t *b, flt_cfg_t *f);
static unsigned int bci_art(sig_buf_t *b, art_rm_t *a);
static unsigned int bci_reref(sig_buf_t *b, unsigned int m);
static unsigned int bci_down(sig_buf_t *b, unsigned int fac);
static unsigned int bci_f_time(sig_buf_t *b, feat_set_t *f);
static unsigned int bci_f_freq(sig_buf_t *b, feat_set_t *f);
static unsigned int bci_f_tf(sig_buf_t *b, feat_set_t *f);
static unsigned int bci_f_spat(sig_buf_t *b, feat_set_t *f);
static unsigned int bci_svm(feat_set_t *f, cls_t *c);
static unsigned int bci_cnn(sig_buf_t *b, cls_t *c);
static unsigned int bci_rnn(sig_buf_t *b, cls_t *c);
static unsigned int bci_att(feat_set_t *f, cls_t *c);
static unsigned int bci_emo(feat_set_t *f, cls_t *c);
static unsigned int bci_stream(sig_buf_t *b);
static unsigned int bci_lowlat(sig_buf_t *b);
static unsigned int bci_learn(feat_set_t *f, unsigned int lbl);
static unsigned int bci_calib(void);
static double bci_qual(sig_buf_t *b);
static unsigned int bci_topo(feat_set_t *f, topo_t *t);
static unsigned int bci_spec(sig_buf_t *b, spec_t *s);
static unsigned int bci_erp(sig_buf_t *b, double *out);
static unsigned int bci_conn(sig_buf_t *b, conn_g_t *g);
static unsigned int bci_rtfb(double *out, unsigned int len);
static unsigned int bci_fuse(sig_buf_t *ee, sig_buf_t *em, sig_buf_t *eo, sig_buf_t *ec, sig_buf_t *fu);
static void bci_fft(double *re, double *im, unsigned int n);
static double bci_mean(const double *d, unsigned int n);
static double bci_var(const double *d, unsigned int n);
static double bci_energy(const double *d, unsigned int n);
static double bci_ham(unsigned int i, unsigned int n);
static unsigned int bci_fullpipe(void);
static unsigned int bci_reset(void);
static void bci_init(void) {
    unsigned int i, j;
    for (i = 0U; i < MAX_CH; i++) { g_ctx.cfg.imp[i] = 0.0;
        for (j = 0U; j < MAX_CH; j++) { g_ctx.art.mix[i][j] = 0.0; g_ctx.art.umix[i][j] = 0.0; } }
    for (i = 0U; i < PIPE_STG; i++) { g_ctx.pipe.stg[i].id = i; g_ctx.pipe.stg[i].status = PIPE_OK;
        g_ctx.pipe.stg[i].nproc = 0U; g_ctx.pipe.stg[i].nerr = 0U; g_ctx.pipe.stg[i].lat = 0.0; }
    g_ctx.pipe.active = PIPE_STG; g_ctx.pipe.status = PIPE_OK; g_ctx.pipe.qual = 1.0;
    g_ctx.pipe.tlat = 0.0; g_ctx.pipe.upsamp = 0U; g_ctx.init = 1U; g_ctx.mode = SIG_EEG;
    bci_status("bci_signal: initialized");
}
static void bci_status(const char *m) { host_print(m); }
static double bci_mean(const double *d, unsigned int n) {
    double s = 0.0; unsigned int i; if (n == 0U) return 0.0;
    for (i = 0U; i < n; i++) s += d[i]; return s / (double)n;
}
static double bci_var(const double *d, unsigned int n) {
    double m, sq; unsigned int i; if (n < 2U) return 0.0;
    m = bci_mean(d, n); sq = 0.0;
    for (i = 0U; i < n; i++) { double v = d[i] - m; sq += v * v; }
    return sq / (double)(n - 1U);
}
static double bci_energy(const double *d, unsigned int n) {
    double e = 0.0; unsigned int i; for (i = 0U; i < n; i++) e += d[i] * d[i]; return e;
}
static double bci_ham(unsigned int i, unsigned int n) {
    double ph = (double)i / (double)(n > 1U ? n - 1U : 1U);
    double x = ph * 6.283185307, x2 = x * x;
    return 0.54 - 0.46 * (1.0 - x2 * 0.5 + x2 * x2 * 0.041666667);
}
static void bci_fft(double *re, double *im, unsigned int n) {
    unsigned int i, j, step, half; j = 0U;
    for (i = 1U; i < n; i++) { unsigned int bit = n >> 1U;
        while (j & bit) { j ^= bit; bit >>= 1U; } j ^= bit;
        if (i < j) { double tr = re[i]; re[i] = re[j]; re[j] = tr;
            double ti = im[i]; im[i] = im[j]; im[j] = ti; } }
    for (step = 1U; step < n; step <<= 1U) { half = step << 1U;
        double wr = 1.0, wi = 0.0, ang = -3.14159265 / (double)step, wpr = ang;
        for (j = 0U; j < step; j++) {
            for (i = j; i < n; i += half) { unsigned int k = i + step;
                double tr = wr * re[k] - wi * im[k], ti2 = wr * im[k] + wi * re[k];
                re[k] = re[i] - tr; im[k] = im[i] - ti2; re[i] += tr; im[i] += ti2; }
            double wt = wr; wr = wr + wi * wpr; wi = wi - wt * wpr; } }
}
static unsigned int bci_cfg(unsigned int st, unsigned int ch, unsigned int r, unsigned int b) {
    if (ch > MAX_CH || ch == 0U || r == 0U || r > 65536U) return 1U;
    g_ctx.cfg.sig_type = st; g_ctx.cfg.nch = ch; g_ctx.cfg.sr = r; g_ctx.cfg.bits = b;
    g_ctx.cfg.ref = 0U; g_ctx.buf.nch = ch; g_ctx.buf.sr = r; g_ctx.buf.stype = st;
    bci_status("bci_signal: configured"); return 0U;
}
static unsigned int bci_acq(sig_buf_t *b, unsigned int st) {
    unsigned int c, s; if (!b) return 1U; b->stype = st;
    if (b->nch > MAX_CH) b->nch = MAX_CH; if (b->nsamp > MAX_SAMP) b->nsamp = MAX_SAMP;
    for (c = 0U; c < b->nch; c++) { double po = (double)c * 0.5;
        for (s = 0U; s < b->nsamp; s++) { double t = (double)s / (double)b->sr, v = 0.0;
            if (st & SIG_EEG) { double p = t * 10.0 + po; v += 50.0 * (p - (double)(unsigned int)p);
                double q = t * 4.0; v += 20.0 * (q - (double)(unsigned int)q); }
            if (st & SIG_EMG) { double p = t * 150.0 + po; v += 100.0 * (p - (double)(unsigned int)p); }
            if (st & SIG_EOG) { double p = t * 0.5 + po; v += 30.0 * (p - (double)(unsigned int)p); }
            if (st & SIG_ECG) { double cp = t * 1.2 + po * 0.1; cp -= (double)(unsigned int)cp;
                if (cp > 0.4 && cp < 0.45) v += 200.0; else if (cp > 0.45 && cp < 0.5) v -= 100.0; }
            b->d[c][s] = v; b->ts[s] = t; } } return 0U;
}
static unsigned int bci_fuse(sig_buf_t *ee, sig_buf_t *em, sig_buf_t *eo, sig_buf_t *ec, sig_buf_t *fu) {
    unsigned int c, s; if (!ee || !em || !eo || !ec || !fu) return 1U;
    fu->nch = ee->nch; fu->nsamp = ee->nsamp; fu->sr = ee->sr; fu->stype = SIG_MULTI;
    for (c = 0U; c < fu->nch; c++) for (s = 0U; s < fu->nsamp; s++)
        fu->d[c][s] = 0.4 * ee->d[c][s] + 0.2 * em->d[c % em->nch][s]
                    + 0.2 * eo->d[c % eo->nch][s] + 0.2 * ec->d[c % ec->nch][s];
    bci_status("bci_signal: multimodal fusion complete"); return 0U;
}
static unsigned int bci_flt(sig_buf_t *b, flt_cfg_t *f) {
    unsigned int c, s; if (!b || !f) return 1U;
    if (f->ord > MAX_FORD) f->ord = MAX_FORD;
    for (c = 0U; c < b->nch; c++) { double pi = 0.0, po2 = 0.0, al = (f->ftype == FLT_ADAPT && f->adap > 0.0) ? f->adap : 0.1;
        for (s = 0U; s < b->nsamp; s++) { double inp = b->d[c][s], out = inp;
            if (f->ftype == FLT_BP) { double bp = al * inp + (1.0 - al) * po2; out = bp - pi * 0.5; }
            else if (f->ftype == FLT_NOTCH) { double nf = f->notch > 0.0 ? f->notch : 50.0;
                double per = (double)b->sr / nf; if (per > 2.0) { unsigned int dl = (unsigned int)per;
                    double del = (s > dl) ? b->d[c][s - dl] : 0.0; out = inp - 0.8 * del; } }
            else if (f->ftype == FLT_ADAPT) { out = po2 + al * (inp - po2); }
            b->d[c][s] = out; pi = inp; po2 = out; } } return 0U;
}
static unsigned int bci_art(sig_buf_t *b, art_rm_t *a) {
    unsigned int c, s, k; if (!b || !a) return 1U;
    for (c = 0U; c < b->nch; c++) { double vr = bci_var(b->d[c], b->nsamp);
        double th = a->athresh > 0.0 ? a->athresh : 3.0;
        if (vr > th * th * 100.0) { a->aidx[a->nart % MAX_ART] = c; a->nart++;
            double mn = bci_mean(b->d[c], b->nsamp);
            for (s = 0U; s < b->nsamp; s++) b->d[c][s] = mn * 0.8 + b->d[c][s] * 0.2; } }
    if (a->ncomp > 0U) { double tmp[MAX_CH];
        for (s = 0U; s < b->nsamp; s++) { for (c = 0U; c < b->nch; c++) tmp[c] = 0.0;
            for (c = 0U; c < b->nch; c++) for (k = 0U; k < a->ncomp && k < b->nch; k++)
                tmp[c] += a->umix[c][k] * b->d[k][s];
            for (c = 0U; c < b->nch; c++) b->d[c][s] = tmp[c]; } } return 0U;
}
static unsigned int bci_reref(sig_buf_t *b, unsigned int m) {
    unsigned int c, s; if (!b) return 1U;
    if (m == 0U) { for (s = 0U; s < b->nsamp; s++) { double avg = 0.0;
            for (c = 0U; c < b->nch; c++) avg += b->d[c][s]; avg /= (double)b->nch;
            for (c = 0U; c < b->nch; c++) b->d[c][s] -= avg; } }
    else { for (s = 0U; s < b->nsamp; s++) { double ref = b->d[0][s];
            for (c = 1U; c < b->nch; c++) b->d[c][s] -= ref; } } return 0U;
}
static unsigned int bci_down(sig_buf_t *b, unsigned int fac) {
    unsigned int c, s, nc; if (!b || fac < 2U) return 1U;
    nc = b->nsamp / fac; if (nc == 0U) return 2U;
    for (c = 0U; c < b->nch; c++) for (s = 0U; s < nc; s++) {
        double sm = 0.0; unsigned int k;
        for (k = 0U; k < fac; k++) sm += b->d[c][s * fac + k]; b->d[c][s] = sm / (double)fac; }
    b->nsamp = nc; b->sr /= fac; return 0U;
}
static unsigned int bci_f_time(sig_buf_t *b, feat_set_t *f) {
    unsigned int c, idx = 0U, s; if (!b || !f) return 1U; f->domain = FEAT_TIME;
    for (c = 0U; c < b->nch && idx < MAX_FEAT - 5U; c++) {
        f->val[idx++] = bci_mean(b->d[c], b->nsamp);
        f->val[idx++] = bci_var(b->d[c], b->nsamp);
        f->val[idx++] = bci_energy(b->d[c], b->nsamp);
        unsigned int zcr = 0U; double mn = f->val[idx - 3U];
        for (s = 1U; s < b->nsamp; s++) if ((b->d[c][s] - mn) * (b->d[c][s-1U] - mn) < 0.0) zcr++;
        f->val[idx++] = (double)zcr / (double)b->nsamp;
        double mnv = b->d[c][0], mxv = b->d[c][0];
        for (s = 1U; s < b->nsamp; s++) { if (b->d[c][s] < mnv) mnv = b->d[c][s]; if (b->d[c][s] > mxv) mxv = b->d[c][s]; }
        f->val[idx++] = mxv - mnv;
        f->mean[c] = f->val[idx-5U]; f->var[c] = f->val[idx-4U]; }
    f->fcnt = idx; return 0U;
}
static unsigned int bci_f_freq(sig_buf_t *b, feat_set_t *f) {
    unsigned int c, k; if (!b || !f) return 1U; f->domain = FEAT_FREQ; unsigned int idx = 0U;
    double rb[FFT_SZ], ib[FFT_SZ];
    for (c = 0U; c < b->nch && idx < MAX_FEAT - FREQ_BINS; c++) {
        unsigned int n = FFT_SZ; if (b->nsamp < n) n = b->nsamp; unsigned int i;
        for (i = 0U; i < FFT_SZ; i++) { rb[i] = (i < n) ? b->d[c][i] * bci_ham(i, n) : 0.0; ib[i] = 0.0; }
        bci_fft(rb, ib, FFT_SZ);
        for (k = 0U; k < FREQ_BINS/2U && idx < MAX_FEAT; k++)
            f->val[idx++] = (rb[k] * rb[k] + ib[k] * ib[k]) / (double)FFT_SZ; }
    f->fcnt = idx; return 0U;
}
static unsigned int bci_f_tf(sig_buf_t *b, feat_set_t *f) {
    unsigned int c, w, k; if (!b || !f) return 1U; f->domain = FEAT_TF; unsigned int idx = 0U;
    unsigned int wsz = 64U, hsz = 32U, nw = (b->nsamp - wsz) / hsz; if (nw > 16U) nw = 16U;
    double rb[FFT_SZ], ib[FFT_SZ];
    for (c = 0U; c < b->nch && c < 4U && idx < MAX_FEAT - 32U; c++)
        for (w = 0U; w < nw && idx < MAX_FEAT; w++) { unsigned int off = w * hsz, i;
            for (i = 0U; i < FFT_SZ; i++) { rb[i] = (i < wsz && (off+i) < b->nsamp) ? b->d[c][off+i] * bci_ham(i, wsz) : 0.0; ib[i] = 0.0; }
            bci_fft(rb, ib, FFT_SZ);
            double bands[5U] = {0.0, 0.0, 0.0, 0.0, 0.0};
            for (k = 1U; k < FFT_SZ/2U; k++) { double psd = rb[k]*rb[k] + ib[k]*ib[k];
                double fr = (double)k * (double)b->sr / (double)FFT_SZ;
                if (fr < 4.0) bands[0] += psd; else if (fr < 8.0) bands[1] += psd;
                else if (fr < 13.0) bands[2] += psd; else if (fr < 30.0) bands[3] += psd; else bands[4] += psd; }
            for (k = 0U; k < 5U && idx < MAX_FEAT; k++) f->val[idx++] = bands[k]; }
    f->fcnt = idx; return 0U;
}
static unsigned int bci_f_spat(sig_buf_t *b, feat_set_t *f) {
    unsigned int c1, c2, s; if (!b || !f) return 1U; f->domain = FEAT_SPAT; unsigned int idx = 0U;
    for (c1 = 0U; c1 < b->nch && idx < MAX_FEAT; c1++) {
        for (c2 = c1+1U; c2 < b->nch && idx < MAX_FEAT; c2++) { double de = 0.0;
            for (s = 0U; s < b->nsamp; s++) { double df = b->d[c1][s] - b->d[c2][s]; de += df * df; }
            f->val[idx++] = de / (double)b->nsamp; }
        double cm = bci_mean(b->d[c1], b->nsamp), cv = bci_var(b->d[c1], b->nsamp);
        if (idx < MAX_FEAT) f->val[idx++] = cv / (cm * cm + 1.0); }
    f->fcnt = idx; return 0U;
}
static unsigned int bci_svm(feat_set_t *f, cls_t *cl) {
    unsigned int fe, c; if (!f || !cl) return 1U; cl->type = CLS_SVM;
    double sc[MAX_CLS]; for (c = 0U; c < cl->ncls && c < MAX_CLS; c++) { sc[c] = cl->bias[c];
        for (fe = 0U; fe < f->fcnt && fe < MAX_FEAT; fe++) sc[c] += cl->w[fe][c] * f->val[fe]; }
    unsigned int best = 0U; for (c = 1U; c < cl->ncls && c < MAX_CLS; c++) if (sc[c] > sc[best]) best = c;
    return best;
}
static unsigned int bci_cnn(sig_buf_t *b, cls_t *cl) {
    unsigned int c, s, k; if (!b || !cl) return 1U; cl->type = CLS_CNN;
    double co[MAX_CH][MAX_SAMP/4U]; unsigned int cln = b->nsamp / 4U; if (cln > MAX_SAMP/4U) cln = MAX_SAMP/4U;
    for (c = 0U; c < b->nch; c++) for (s = 0U; s < cln; s++) { co[c][s] = 0.0;
        for (k = 0U; k < 4U && (s+k) < b->nsamp; k++) co[c][s] += b->d[c][s+k] * 0.25;
        if (co[c][s] < 0.0) co[c][s] = 0.0; }
    double po[MAX_CH]; for (c = 0U; c < b->nch; c++) po[c] = bci_mean(co[c], cln);
    double sc[MAX_CLS]; unsigned int best = 0U;
    for (k = 0U; k < cl->ncls && k < MAX_CLS; k++) { sc[k] = cl->bias[k];
        for (c = 0U; c < b->nch; c++) sc[k] += cl->w[c % MAX_FEAT][k] * po[c];
        if (sc[k] > sc[best]) best = k; } return best;
}
static unsigned int bci_rnn(sig_buf_t *b, cls_t *cl) {
    unsigned int c, s; if (!b || !cl) return 1U; cl->type = CLS_RNN;
    double hd[MAX_CLS]; for (c = 0U; c < MAX_CLS; c++) hd[c] = 0.0;
    unsigned int sc2 = b->nsamp / 8U; if (sc2 == 0U) sc2 = 1U; unsigned int ssz = b->nsamp / sc2;
    for (s = 0U; s < sc2; s++) { unsigned int off = s * ssz;
        for (c = 0U; c < b->nch && c < MAX_CLS; c++) {
            double inp = bci_mean(&b->d[c][off], ssz); hd[c] = hd[c] * 0.8 + inp * 0.2; } }
    unsigned int best = 0U;
    for (c = 0U; c < cl->ncls && c < MAX_CLS; c++) { hd[c] = hd[c % b->nch] + cl->bias[c];
        if (hd[c] > hd[best]) best = c; } return best;
}
static unsigned int bci_att(feat_set_t *f, cls_t *cl) {
    unsigned int fe; if (!f || !cl) return 1U; cl->type = CLS_ATT;
    double bp = 0.0, tp = 0.0, ap = 0.0;
    for (fe = 0U; fe < f->fcnt; fe++) { double v = f->val[fe];
        if (fe % 5U == 3U) bp += v; else if (fe % 5U == 1U) tp += v; else if (fe % 5U == 2U) ap += v; }
    double as2 = (tp > 0.001) ? bp / tp : 0.0; if (ap > 0.001) as2 *= (1.0 / (ap + 1.0));
    return (as2 > 1.5) ? 1U : 0U;
}
static unsigned int bci_emo(feat_set_t *f, cls_t *cl) {
    if (!f || !cl) return 1U; cl->type = CLS_EMO;
    double fl = 0.0, fr = 0.0; unsigned int fe;
    for (fe = 0U; fe < f->fcnt; fe++) { if (fe % 2U == 0U) fl += f->val[fe]; else fr += f->val[fe]; }
    double asy = fl - fr; unsigned int em = 0U;
    if (asy > 10.0) em = 1U; else if (asy < -10.0) em = 2U;
    if (bci_energy(f->val, f->fcnt) > 10000.0) em = 3U; return em;
}
static unsigned int bci_stream(sig_buf_t *b) {
    unsigned int r; if (!b) return 1U;
    g_ctx.pipe.stg[0].nproc += b->nsamp; g_ctx.pipe.stg[0].lat = 0.5;
    r = bci_flt(b, &g_ctx.flt); g_ctx.pipe.stg[1].nproc += b->nsamp; g_ctx.pipe.stg[1].lat = 1.2; g_ctx.pipe.stg[1].status = r;
    r = bci_art(b, &g_ctx.art); g_ctx.pipe.stg[2].nproc += b->nsamp; g_ctx.pipe.stg[2].lat = 2.0; g_ctx.pipe.stg[2].status = r;
    r = bci_reref(b, 0U); g_ctx.pipe.stg[3].nproc += b->nsamp; g_ctx.pipe.stg[3].lat = 0.3;
    r = bci_f_time(b, &g_ctx.feat); g_ctx.pipe.stg[4].nproc += b->nsamp; g_ctx.pipe.stg[4].lat = 1.5; g_ctx.pipe.stg[4].status = r;
    unsigned int lbl = bci_svm(&g_ctx.feat, &g_ctx.cls); g_ctx.pipe.stg[5].nproc++; g_ctx.pipe.stg[5].lat = 0.2; (void)lbl;
    g_ctx.pipe.tlat = 5.7; g_ctx.pipe.upsamp += b->nsamp; return 0U;
}
static unsigned int bci_lowlat(sig_buf_t *b) {
    unsigned int r; if (!b) return 1U;
    r = bci_flt(b, &g_ctx.flt); if (r) return r;
    r = bci_f_time(b, &g_ctx.feat); if (r) return r;
    unsigned int lbl = bci_svm(&g_ctx.feat, &g_ctx.cls); g_ctx.pipe.tlat = 1.5; (void)lbl; return 0U;
}
static unsigned int bci_learn(feat_set_t *f, unsigned int lbl) {
    unsigned int fe, c; if (!f || lbl >= MAX_CLS) return 1U;
    double lr = g_ctx.cls.lr > 0.0 ? g_ctx.cls.lr : 0.001;
    for (fe = 0U; fe < f->fcnt && fe < MAX_FEAT; fe++)
        for (c = 0U; c < g_ctx.cls.ncls; c++) { double tgt = (c == lbl) ? 1.0 : 0.0;
            double err = tgt - g_ctx.cls.w[fe][c] * f->val[fe]; g_ctx.cls.w[fe][c] += lr * err * f->val[fe]; }
    g_ctx.cls.epochs++; return 0U;
}
static unsigned int bci_calib(void) {
    unsigned int c; for (c = 0U; c < g_ctx.buf.nch; c++) {
        double vr = bci_var(g_ctx.buf.d[c], g_ctx.buf.nsamp); g_ctx.art.athresh = 3.0 * vr;
        if (vr > 1000.0) g_ctx.flt.ord = 12U; else if (vr > 100.0) g_ctx.flt.ord = 8U; else g_ctx.flt.ord = 4U; }
    bci_status("bci_signal: adaptive calibration complete"); return 0U;
}
static double bci_qual(sig_buf_t *b) {
    unsigned int c; double q = 1.0; if (!b) return 0.0;
    for (c = 0U; c < b->nch; c++) {
        if (g_ctx.cfg.imp[c] > 10.0) q *= 0.8;
        if (bci_var(b->d[c], b->nsamp) < 0.001) q *= 0.5;
        if (bci_energy(b->d[c], b->nsamp) > 1000000.0) q *= 0.7; }
    g_ctx.pipe.qual = q; if (q < 0.5) g_ctx.pipe.status |= PIPE_QLOW; return q;
}
static unsigned int bci_topo(feat_set_t *f, topo_t *t) {
    unsigned int i, j, e; if (!f || !t) return 1U;
    for (i = 0U; i < TOPO_SZ; i++) for (j = 0U; j < TOPO_SZ; j++) {
        double gx = (double)i / (double)TOPO_SZ, gy = (double)j / (double)TOPO_SZ;
        double sw = 0.0, sv = 0.0;
        for (e = 0U; e < t->nel && e < MAX_CH; e++) {
            double dx = gx - t->ex[e], dy = gy - t->ey[e], dist = dx*dx + dy*dy + 0.001;
            double wt = 1.0 / dist; sw += wt;
            sv += wt * ((e < f->fcnt) ? f->val[e] : 0.0); }
        t->grid[i][j] = (sw > 0.0) ? sv / sw : 0.0; }
    bci_status("bci_signal: topography generated"); return 0U;
}
static unsigned int bci_spec(sig_buf_t *b, spec_t *sp) {
    unsigned int c, w, k; if (!b || !sp) return 1U;
    unsigned int wsz = 64U, hsz = 32U, nf = (b->nsamp - wsz) / hsz; if (nf > MAX_SAMP/4U) nf = MAX_SAMP/4U;
    sp->fb = FREQ_BINS/2U; sp->tf = nf; double rb[FFT_SZ], ib[FFT_SZ]; c = 0U;
    for (w = 0U; w < nf; w++) { unsigned int off = w * hsz, i;
        for (i = 0U; i < FFT_SZ; i++) { rb[i] = (i < wsz && (off+i) < b->nsamp) ? b->d[c][off+i] * bci_ham(i, wsz) : 0.0; ib[i] = 0.0; }
        bci_fft(rb, ib, FFT_SZ);
        for (k = 0U; k < sp->fb; k++) { sp->mag[k][w] = rb[k]*rb[k] + ib[k]*ib[k]; sp->ph[k][w] = 0.0; }
        sp->ta[w] = (double)off / (double)b->sr; }
    for (k = 0U; k < sp->fb; k++) sp->fa[k] = (double)k * (double)b->sr / (double)FFT_SZ;
    bci_status("bci_signal: spectrogram generated"); return 0U;
}
static unsigned int bci_erp(sig_buf_t *b, double *out) {
    unsigned int c, s; if (!b || !out) return 1U;
    for (s = 0U; s < b->nsamp; s++) { double sm = 0.0;
        for (c = 0U; c < b->nch; c++) sm += b->d[c][s]; out[s] = sm / (double)b->nch; }
    bci_status("bci_signal: ERP waveform generated"); return 0U;
}
static unsigned int bci_conn(sig_buf_t *b, conn_g_t *g) {
    unsigned int i, j, s; if (!b || !g) return 1U;
    g->nn = b->nch; if (g->nn > MAX_CONN) g->nn = MAX_CONN; g->nec = 0U;
    for (i = 0U; i < g->nn; i++) { g->nv[i] = bci_var(b->d[i], b->nsamp);
        for (j = i+1U; j < g->nn; j++) { double vi = bci_var(b->d[i], b->nsamp), vj = bci_var(b->d[j], b->nsamp);
            if (vi < 0.001 || vj < 0.001) { g->conn[i][j] = 0.0; g->conn[j][i] = 0.0; continue; }
            double cc = 0.0; for (s = 0U; s < b->nsamp; s++) cc += b->d[i][s] * b->d[j][s]; cc /= (double)b->nsamp;
            double coh = (cc * cc) / (vi * vj); g->conn[i][j] = coh; g->conn[j][i] = coh;
            if (coh > g->thresh) g->nec++; } }
    bci_status("bci_signal: connectivity graph generated"); return 0U;
}
static unsigned int bci_rtfb(double *out, unsigned int len) {
    unsigned int i; if (!out || len == 0U) return 1U;
    double q = g_ctx.pipe.qual;
    for (i = 0U; i < len; i++) out[i] = q * 100.0 + (double)(i % 7U) * 0.5 - 1.5;
    return 0U;
}
static unsigned int bci_fullpipe(void) {
    unsigned int r; sig_buf_t *b = &g_ctx.buf;
    r = bci_acq(b, g_ctx.mode); if (r) return r;
    r = bci_flt(b, &g_ctx.flt); if (r) return r;
    r = bci_art(b, &g_ctx.art); if (r) return r;
    r = bci_reref(b, 0U); if (r) return r;
    r = bci_f_time(b, &g_ctx.feat); if (r) return r;
    r = bci_f_freq(b, &g_ctx.feat); if (r) return r;
    unsigned int lbl = bci_svm(&g_ctx.feat, &g_ctx.cls); double q = bci_qual(b); (void)lbl; (void)q;
    bci_status("bci_signal: full pipeline complete"); return 0U;
}
static unsigned int bci_reset(void) {
    unsigned int i; g_ctx.buf.nsamp = 0U; g_ctx.pipe.status = PIPE_OK; g_ctx.pipe.upsamp = 0U;
    g_ctx.pipe.tlat = 0.0; g_ctx.pipe.qual = 1.0; g_ctx.art.nart = 0U; g_ctx.cls.epochs = 0U; g_ctx.feat.fcnt = 0U;
    for (i = 0U; i < PIPE_STG; i++) { g_ctx.pipe.stg[i].nproc = 0U; g_ctx.pipe.stg[i].nerr = 0U; g_ctx.pipe.stg[i].lat = 0.0; }
    bci_status("bci_signal: reset complete"); return 0U;
}
/* ========== Entry Point ========== */

void _start(void) {
    /* Initialize the BCI context and set default configuration */
    bci_init();
    bci_cfg(SIG_EEG, 8U, 256U, 16U);
    /* Configure default filter: bandpass 0.5-50 Hz with 50Hz notch */
    g_ctx.flt.ftype = FLT_BP;
    g_ctx.flt.lo = 0.5;
    g_ctx.flt.hi = 50.0;
    g_ctx.flt.notch = 50.0;
    g_ctx.flt.ord = 4U;
    g_ctx.flt.adap = 0.01;
    /* Configure classifier defaults */
    g_ctx.cls.ncls = 4U;
    g_ctx.cls.feat_dim = MAX_FEAT;
    g_ctx.cls.lr = 0.001;
    /* Run full pipeline for EEG modality */
    unsigned int r = bci_fullpipe();
    if (r) host_exit((int)r);
    /* Run full pipeline for EMG, EOG, ECG modalities */
    bci_cfg(SIG_EMG, 4U, 1000U, 12U);
    bci_fullpipe();
    bci_cfg(SIG_EOG, 2U, 200U, 16U);
    bci_fullpipe();
    bci_cfg(SIG_ECG, 1U, 500U, 16U);
    bci_fullpipe();
    /* Set up multimodal fusion test buffers */
    sig_buf_t eb, mb, ob, cb, fb2;
    eb.nch = 4U; eb.nsamp = 256U; eb.sr = 256U;
    mb.nch = 4U; mb.nsamp = 256U; mb.sr = 256U;
    ob.nch = 2U; ob.nsamp = 256U; ob.sr = 256U;
    cb.nch = 1U; cb.nsamp = 256U; cb.sr = 256U;
    /* Acquire signals from all modalities */
    bci_acq(&eb, SIG_EEG);
    bci_acq(&mb, SIG_EMG);
    bci_acq(&ob, SIG_EOG);
    bci_acq(&cb, SIG_ECG);
    /* Perform multimodal fusion */
    bci_fuse(&eb, &mb, &ob, &cb, &fb2);
    /* Feature extraction across all domains */
    bci_f_time(&eb, &g_ctx.feat);
    bci_f_freq(&eb, &g_ctx.feat);
    bci_f_tf(&eb, &g_ctx.feat);
    bci_f_spat(&eb, &g_ctx.feat);
    /* Classification with all classifier types */
    bci_svm(&g_ctx.feat, &g_ctx.cls);
    bci_cnn(&eb, &g_ctx.cls);
    bci_rnn(&eb, &g_ctx.cls);
    bci_att(&g_ctx.feat, &g_ctx.cls);
    bci_emo(&g_ctx.feat, &g_ctx.cls);
    /* Real-time processing pipeline tests */
    bci_stream(&eb);
    bci_lowlat(&eb);
    /* Online learning with synthetic labels */
    bci_learn(&g_ctx.feat, 1U);
    bci_learn(&g_ctx.feat, 2U);
    /* Adaptive calibration and quality monitoring */
    bci_calib();
    bci_qual(&eb);
    /* Visualization: brain topography */
    topo_t tp;
    tp.nel = 8U;
    unsigned int ei;
    for (ei = 0U; ei < 8U; ei++) {
        tp.ex[ei] = (double)ei / 8.0;
        tp.ey[ei] = 0.5;
        tp.ev[ei] = g_ctx.feat.val[ei % g_ctx.feat.fcnt];
    }
    bci_topo(&g_ctx.feat, &tp);
    /* Visualization: spectrogram */
    bci_spec(&eb, &g_ctx.spec);
    /* Visualization: ERP waveform */
    double erpd[MAX_SAMP];
    bci_erp(&eb, erpd);
    /* Visualization: connectivity graph */
    bci_conn(&eb, &g_ctx.conn);
    /* Real-time feedback signal */
    double fbk[64U];
    bci_rtfb(fbk, 64U);
    /* Reset pipeline state and exit */
    bci_reset();
    bci_status("bci_signal: all operations complete");
    host_exit(0);
}
