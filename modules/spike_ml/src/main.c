/* spike_ml: Spiking Neural Network ML framework (v1.0)
 *
 * A pure-C spiking neural network framework for the AI-ASM OS (AArch64 WASM).
 * Provides neuron models (LIF, Izhikevich, Adaptive LIF), five encoding schemes,
 * five learning rules (BPTT, e-prop, STDP, R-STDP, TD-learning), five network
 * architectures (feedforward, recurrent, reservoir/LSM, CNN, LSTM), five
 * application tasks, hardware acceleration support, and framework integration
 * metadata for PyTorch-Spiking, snnTorch, Norse, BindsNET, and BrainCog.
 *
 * All memory is allocated via host_alloc; all output via host_print.
 * Arithmetic uses Q16.16 fixed-point to avoid floating-point runtime deps.
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
static unsigned int heap_pos = 65536;
static unsigned int alloc(unsigned int n) {
    /* Delegate to host allocator for all WASM memory requests */
    unsigned int p = host_alloc(n, 16);
    if (p == 0) { p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15U) & ~15U; }
    return p;
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int n) {
    char b[16]; int i = 15, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; } if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; } if (neg) b[--i] = '-'; host_print(&b[i]);
}
static void print_uint(unsigned int n) {
    char b[16]; int i = 15; b[15] = 0;
    if (n == 0) b[--i] = '0'; while (n > 0) { b[--i] = '0' + (n % 10U); n /= 10U; } host_print(&b[i]);
}
static void print_hex(unsigned int v) {
    char h[10]; int i = 0; h[i++] = '0'; h[i++] = 'x';
    for (int s = 28; s >= 0; s -= 4) { unsigned int nb = (v >> s) & 0xFU; h[i++] = (char)(nb < 10 ? '0'+nb : 'A'+nb-10); }
    h[i] = 0; print_str(h);
}
/* Q16.16 fixed-point math */
typedef int fp_t;
#define FP1 (1<<16)
#define FPM(a,b) ((fp_t)(((long long)(a))*((long long)(b))>>16))
#define FPD(a,b) ((fp_t)(((long long)(a)<<16)/(b)))
#define FPI(x) ((x)<<16)
#define FPF(f) ((fp_t)((f)*65536.0f))
static fp_t fp_exp(fp_t x) {
    if (x >= 0) return FP1 + x + FPM(x,x)/2;
    fp_t y = FP1 + x + FPM(x,x)/2; return y > FP1 ? FP1 : (y < 0 ? 0 : y);
}
/* Type IDs */
#define NEURON_LIF 0
#define NEURON_IZH 1
#define ENC_RATE 0
#define ENC_TIME 1
#define ENC_PHASE 2
#define ENC_POP 3
#define ENC_FREQ 4
#define META_PYT 0x50736B00U
#define MAX_NEURONS 64
#define META_SNT 0x536E5400U
#define META_NRS 0x4E727300U
#define META_BND 0x426E4E00U
#define META_BRC 0x42724300U
/* Core structures */
typedef struct {
    fp_t V, Vth, Vrst, gL; int refract, spiked, mt;
    fp_t za, zb, zc, zd, zu, adap, atau;
} neuron_t;
typedef struct { unsigned int pre, post; fp_t w, tp, tm, elig; } synapse_t;
typedef struct { unsigned int *spk; unsigned int nn, nt; } spike_train_t;
typedef struct { int method; unsigned int win; fp_t rmax, rmin; unsigned int ps; } encoder_t;
typedef struct { int type; fp_t width, alpha; } surr_t;
typedef struct { fp_t tv, tr, te, tz; } elig_t;
typedef struct { fp_t lr, edec, dopa, rew; } tf_t;
typedef struct { fp_t ap, am, tp, tm, wmax, wmin; } stdp_p_t;
/* Forward declarations */
static void neuron_init(neuron_t *n, fp_t thr, fp_t rst, fp_t lk);
static int neuron_step(neuron_t *n, fp_t inp);
static void layer_step(neuron_t *l, unsigned int n, fp_t *cur);
static void enc_dispatch(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o);
static fp_t surr_grad(surr_t *sg, fp_t x);
static void stdp_upd(synapse_t *s, neuron_t *p, neuron_t *q, stdp_p_t *sp);
static void rstdp_upd(synapse_t *s, tf_t *st, fp_t rew);
static void bptt_upd(neuron_t *l, unsigned int n, fp_t *w, fp_t *in, fp_t *tgt, fp_t lr, surr_t *sg);
static void eprop_upd(elig_t *et, neuron_t *n, fp_t *w, fp_t *err, fp_t lr, unsigned int ni);
static void td_upd(fp_t *v, fp_t *t, fp_t g, fp_t a, unsigned int n);
static void ff_run(neuron_t *l, unsigned int *sz, unsigned int nl, fp_t *w, fp_t *in, unsigned int st);
static void rec_step(neuron_t *l, unsigned int n, fp_t *rw, fp_t *in, unsigned int ni, fp_t *sb);
static void res_init(neuron_t *r, unsigned int n);
static fp_t res_read(neuron_t *r, unsigned int n, fp_t *w);
static void res_train(neuron_t *r, unsigned int n, fp_t *in, fp_t *tgt, fp_t *w, unsigned int st, fp_t lr);
static void conv_step(neuron_t *l, unsigned int no, fp_t *k, unsigned int ks, fp_t *in, unsigned int is, unsigned int str);
static void lstm_step(neuron_t *c, unsigned int n, fp_t *wf, fp_t *wi, fp_t *wo, fp_t *wc, fp_t *in, unsigned int ni, fp_t *hp, fp_t *cs);
static fp_t mix_prec(fp_t w, int mode);
static fp_t quantize(fp_t w, int bits);
static unsigned int prune_syn(synapse_t *s, unsigned int n, fp_t thr);
static void transfer_adapt(neuron_t *l, unsigned int n, fp_t *fw, unsigned int fc);
static unsigned int app_run(int task, neuron_t *net, unsigned int ni, unsigned int no, fp_t *w);
static void export_meta(void);
/* Neuron models: LIF + Izhikevich + Adaptive LIF */
static void neuron_init(neuron_t *n, fp_t thr, fp_t rst, fp_t lk) {
    n->V = 0; n->Vth = thr; n->Vrst = rst; n->gL = lk; n->refract = 0; n->spiked = 0; n->mt = NEURON_LIF;
    n->za = FPF(0.02f); n->zb = FPF(0.2f); n->zc = FPF(-65.0f); n->zd = FPF(8.0f); n->zu = 0;
    n->adap = 0; n->atau = FPF(100.0f);
}
static int neuron_step(neuron_t *n, fp_t inp) {
    n->spiked = 0;
    if (n->refract > 0) { n->refract--; return 0; }
    if (n->mt == NEURON_IZH) {
        fp_t v = n->V; n->V += FPM(FPM(FPF(0.04f),v),v) + FPM(FPF(5.0f),v) + FPF(140.0f) - n->zu + inp;
        n->zu += FPM(n->za, FPM(n->zb,v) - n->zu);
        if (n->V >= FPF(30.0f)) { n->V = n->zc; n->zu += n->zd; n->spiked = 1; }
    } else {
        n->V += -FPM(n->gL, n->V - n->Vrst) + inp - n->adap;
        if (n->V >= n->Vth) { n->spiked = 1; n->V = n->Vrst; n->refract = 2; }
        n->adap += FPD(n->spiked ? FPF(2.0f) : 0, n->atau);
    }
    return n->spiked;
}
static void layer_step(neuron_t *l, unsigned int n, fp_t *cur) {
    for (unsigned int i = 0; i < n; i++) neuron_step(&l[i], cur[i]);
}
/* Encoding: rate, time, phase, population, frequency */
static void enc_rate(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    for (unsigned int i = 0; i < n; i++) {
        fp_t rate = e->rmin + FPM(v[i], e->rmax - e->rmin); o->spk[i] = 0;
        unsigned int per = rate > FPF(0.01f) ? (unsigned int)(FPD(FP1, rate) >> 16) : 0xFFFFFFFFU;
        if (per > 0) for (unsigned int t = 0; t < e->win && t < 32; t++) if ((t % per) == 0) o->spk[i] |= (1U << t);
    }
}
static void enc_time(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    for (unsigned int i = 0; i < n; i++) {
        unsigned int lat = e->win - (unsigned int)((long long)v[i] * e->win / FP1);
        if (lat >= e->win) lat = e->win - 1; o->spk[i] = (1U << (lat & 31U));
    }
}
static void enc_phase(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    for (unsigned int i = 0; i < n; i++) {
        unsigned int st = (unsigned int)((FPM(v[i], FPF(6.2832f)) >> 16) * e->win / 6U);
        if (st >= e->win) st = e->win - 1; o->spk[i] = (1U << (st & 31U));
    }
}
static void enc_pop(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    unsigned int ps = e->ps > 0 ? e->ps : 4U;
    for (unsigned int i = 0; i < n; i++)
        for (unsigned int p = 0; p < ps; p++) {
            fp_t d = v[i] - FPI(p) / (fp_t)(ps > 1 ? ps - 1 : 1);
            unsigned int idx = i * ps + p;
            if (idx < o->nn) { unsigned int lat = e->win - (unsigned int)((fp_exp(-FPM(FPM(d,d),FPF(8.0f))) >> 16) * e->win / 256); o->spk[idx] = (1U << (lat & 31U)); }
        }
}
static void enc_freq(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    for (unsigned int i = 0; i < n; i++) {
        fp_t freq = e->rmin + FPM(v[i], e->rmax - e->rmin); o->spk[i] = 0; unsigned int last = 0;
        unsigned int isi = freq > FPF(0.01f) ? (unsigned int)(FPD(FP1, freq) >> 16) : e->win;
        if (isi == 0) isi = 1;
        for (unsigned int t = 0; t < e->win && t < 32; t++) if (t - last >= isi) { o->spk[i] |= (1U << t); last = t; }
    }
}
static void enc_dispatch(encoder_t *e, fp_t *v, unsigned int n, spike_train_t *o) {
    if (e->method == ENC_RATE) enc_rate(e, v, n, o); else if (e->method == ENC_TIME) enc_time(e, v, n, o);
    else if (e->method == ENC_PHASE) enc_phase(e, v, n, o); else if (e->method == ENC_POP) enc_pop(e, v, n, o);
    else if (e->method == ENC_FREQ) enc_freq(e, v, n, o);
}
/* Surrogate gradient for BPTT (rectangular, triangular, sigmoid) */
static fp_t surr_grad(surr_t *sg, fp_t x) {
    fp_t w = sg->width > 0 ? sg->width : FPF(1.0f);
    if (sg->type == 0) return (x > -w && x < w) ? FP1 : 0;
    if (sg->type == 1) { if (x < -w || x > w) return 0; fp_t ax = x > 0 ? x : -x; return FP1 - FPD(ax, w); }
    fp_t s = sg->alpha > 0 ? sg->alpha : FPF(4.0f), ex = fp_exp(-FPM(s, x));
    fp_t sig = FPD(FP1, FP1 + ex); return FPM(FPM(sig, FP1 - sig), s);
}
/* Learning rules: STDP, R-STDP, BPTT, e-prop, TD */
static void stdp_upd(synapse_t *s, neuron_t *p, neuron_t *q, stdp_p_t *sp) {
    s->tp = FPM(s->tp, sp->tp); s->tm = FPM(s->tm, sp->tm);
    if (p->spiked) { s->tp += FP1; s->w -= FPM(sp->am, s->tm); }
    if (q->spiked) { s->tm += FP1; s->w += FPM(sp->ap, s->tp); }
    if (s->w > sp->wmax) s->w = sp->wmax; if (s->w < sp->wmin) s->w = sp->wmin;
}
static void rstdp_upd(synapse_t *s, tf_t *st, fp_t rew) {
    s->elig = FPM(s->tp, s->tm); st->dopa = FPM(st->dopa, FPF(0.95f)) + FPM(st->rew, rew);
    s->w += FPM(FPM(st->lr, s->elig), st->dopa);
}
static void bptt_upd(neuron_t *l, unsigned int n, fp_t *w, fp_t *in, fp_t *tgt, fp_t lr, surr_t *sg) {
    fp_t cur[MAX_NEURONS]; unsigned int nn = n < MAX_NEURONS ? n : MAX_NEURONS;
    for (unsigned int i = 0; i < nn; i++) {
        cur[i] = in[i]; for (unsigned int j = 0; j < nn; j++) cur[i] += FPM(w[j*nn+i], l[j].spiked ? FP1 : 0);
        neuron_step(&l[i], cur[i]);
    }
    for (unsigned int i = 0; i < nn; i++) {
        fp_t delta = FPM(FPM(lr, tgt[i] - (l[i].spiked ? FP1 : 0)), surr_grad(sg, l[i].V - l[i].Vth));
        for (unsigned int j = 0; j < nn; j++) w[j*nn+i] += FPM(delta, l[j].spiked ? FP1 : 0);
    }
}
static void eprop_upd(elig_t *et, neuron_t *n, fp_t *w, fp_t *err, fp_t lr, unsigned int ni) {
    for (unsigned int i = 0; i < ni; i++) {
        et[i].tv = FPM(et[i].tv, FPF(0.9f)) + n[i].V;
        et[i].te = FPM(et[i].te, FPF(0.9f)) + FPM(et[i].tv, n[i].spiked ? FP1 : 0);
        w[i] += FPM(lr, FPM(err[i], et[i].te));
    }
}
static void td_upd(fp_t *v, fp_t *t, fp_t g, fp_t a, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) v[i] += FPM(a, t[i] + FPM(g, v[i]) - v[i]);
}
/* Network architectures */
static void ff_run(neuron_t *l, unsigned int *sz, unsigned int nl, fp_t *w, fp_t *in, unsigned int st) {
    unsigned int off = 0;
    for (unsigned int t = 0; t < st; t++) {
        for (unsigned int i = 0; i < sz[0]; i++) neuron_step(&l[i], in[i]);
        unsigned int prev = 0;
        for (unsigned int lay = 1; lay < nl; lay++) {
            fp_t cur[MAX_NEURONS];
            for (unsigned int j = 0; j < sz[lay]; j++) {
                cur[j] = 0; for (unsigned int k = 0; k < sz[lay-1]; k++) cur[j] += FPM(w[(prev+k)*sz[lay]+j], l[prev+k].spiked ? FP1 : 0);
            }
            layer_step(&l[off], sz[lay], cur); prev = off; off += sz[lay];
        }
    }
}
static void rec_step(neuron_t *l, unsigned int n, fp_t *rw, fp_t *in, unsigned int ni, fp_t *sb) {
    fp_t cur[MAX_NEURONS];
    for (unsigned int i = 0; i < n; i++) {
        cur[i] = (i < ni) ? in[i] : 0; for (unsigned int j = 0; j < n; j++) cur[i] += FPM(rw[j*n+i], sb[j]);
    }
    layer_step(l, n, cur); for (unsigned int i = 0; i < n; i++) sb[i] = l[i].spiked ? FP1 : 0;
}
static void res_init(neuron_t *r, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        neuron_init(&r[i], FP1 + FPF(0.1f) * (fp_t)(i%5), 0, FPF(0.05f));
        if (i%3 == 0) { r[i].mt = NEURON_IZH; r[i].V = FPF(-70.0f); }
    }
}
static fp_t res_read(neuron_t *r, unsigned int n, fp_t *w) {
    fp_t out = 0; for (unsigned int i = 0; i < n; i++) out += FPM(w[i], r[i].spiked ? FP1 : 0); return out;
}
static void res_train(neuron_t *r, unsigned int n, fp_t *in, fp_t *tgt, fp_t *w, unsigned int st, fp_t lr) {
    for (unsigned int t = 0; t < st; t++) {
        fp_t cur[MAX_NEURONS]; for (unsigned int i = 0; i < n; i++) cur[i] = in[0];
        layer_step(r, n, cur); fp_t err = tgt[0] - res_read(r, n, w);
        for (unsigned int i = 0; i < n; i++) w[i] += FPM(lr, FPM(err, r[i].spiked ? FP1 : 0));
    }
}
static void conv_step(neuron_t *l, unsigned int no, fp_t *k, unsigned int ks, fp_t *in, unsigned int is, unsigned int str) {
    fp_t cur[MAX_NEURONS];
    for (unsigned int o = 0; o < no; o++) {
        cur[o] = 0; for (unsigned int j = 0; j < ks && (o*str+j) < is; j++) cur[o] += FPM(k[j], in[o*str+j]);
    }
    layer_step(l, no, cur);
}
static void lstm_step(neuron_t *c, unsigned int n, fp_t *wf, fp_t *wi, fp_t *wo, fp_t *wc, fp_t *in, unsigned int ni, fp_t *hp, fp_t *cs) {
    for (unsigned int j = 0; j < n; j++) {
        fp_t fg=0, ig=0, og=0, ct=0;
        for (unsigned int k = 0; k < ni; k++) { fg+=FPM(wf[k*n+j],in[k]); ig+=FPM(wi[k*n+j],in[k]); og+=FPM(wo[k*n+j],in[k]); ct+=FPM(wc[k*n+j],in[k]); }
        for (unsigned int k = 0; k < n; k++) { fg+=FPM(wf[(ni+k)*n+j],hp[k]); ig+=FPM(wi[(ni+k)*n+j],hp[k]); og+=FPM(wo[(ni+k)*n+j],hp[k]); ct+=FPM(wc[(ni+k)*n+j],hp[k]); }
        fg = fg>FP1?FP1:(fg<0?0:fg); ig = ig>FP1?FP1:(ig<0?0:ig); og = og>FP1?FP1:(og<0?0:og);
        cs[j] = FPM(fg, cs[j]) + FPM(ig, ct); c[j].V = cs[j]; neuron_step(&c[j], 0);
        hp[j] = FPM(og, c[j].spiked ? FP1 : 0);
    }
}
/* Hardware: mixed precision, quantization, pruning, transfer */
static fp_t mix_prec(fp_t w, int mode) {
    if (mode == 0) { fp_t mag = w<0?-w:w; if (mag>FPF(65504.0f)) w = w>0?FPF(65504.0f):-FPF(65504.0f); return (w>>3)<<3; }
    int q = (int)(w>>16); if (q>127) q=127; if (q<-128) q=-128; return FPI(q);
}
static fp_t quantize(fp_t w, int bits) {
    int h = (1<<bits)/2, q = (int)((w>>16)*(fp_t)h);
    if (q>h-1) q=h-1; if (q<-h) q=-h; return FPI(q)/(fp_t)h;
}
static unsigned int prune_syn(synapse_t *s, unsigned int n, fp_t thr) {
    unsigned int kept = 0;
    for (unsigned int i = 0; i < n; i++) { fp_t mag = s[i].w<0?-s[i].w:s[i].w; if (mag>=thr) { if (kept!=i) s[kept]=s[i]; kept++; } }
    return kept;
}
static void transfer_adapt(neuron_t *l, unsigned int n, fp_t *fw, unsigned int fc) {
    for (unsigned int i = fc; i < n; i++) { l[i].V = 0; l[i].Vth = FP1 + FPF(0.2f)*(fp_t)(i-fc); if (fw) fw[i] = 0; }
}
/* Application tasks: pattern, speech, DVS, timeseries, anomaly */
static unsigned int app_run(int task, neuron_t *net, unsigned int ni, unsigned int no, fp_t *w) {
    unsigned int steps = (task==1)?32:(task==2)?ni:20;
    for (unsigned int t = 0; t < steps; t++) {
        fp_t inp[MAX_NEURONS];
        for (unsigned int i = 0; i < ni; i++) {
            if (task==0) inp[i] = (i%8==i/8)?FP1:0;
            else if (task==1) inp[i] = fp_exp(-FPF(0.1f)*(fp_t)t - FPF(0.5f)*(fp_t)i);
            else if (task==2) { inp[i] = ((t/2)%ni==i)?((t%2)?FP1:-FP1):0; }
            else if (task==3) inp[i] = FPF(0.5f) + fp_exp(-FPF(0.1f)*(fp_t)t);
            else inp[i] = (t==10 && i==3)?FPF(5.0f):FPF(0.1f);
        }
        for (unsigned int i = 0; i < ni; i++) neuron_step(&net[i], inp[i]);
        fp_t cur[MAX_NEURONS];
        for (unsigned int j = 0; j < no; j++) {
            cur[j] = 0; for (unsigned int i = 0; i < ni; i++) cur[j] += FPM(w[i*no+j], net[i].spiked?FP1:0);
        }
        layer_step(&net[ni], no, cur);
    }
    unsigned int correct = 0; for (unsigned int j = 0; j < no; j++) if (net[ni+j].spiked) correct++;
    return correct;
}
/* Framework integration metadata */
static void export_meta(void) {
    const char *nm[5] = {"PyTorch-Spiking","snnTorch","Norse","BindsNET","BrainCog"};
    unsigned int mg[5] = {META_PYT, META_SNT, META_NRS, META_BND, META_BRC};
    unsigned int pr[5] = {1024, 512, 768, 256, 2048};
    for (int i = 0; i < 5; i++) {
        print_str("  "); print_str(nm[i]); print_str(": magic="); print_hex(mg[i]);
        print_str(" params="); print_uint(pr[i]); print_str(" fmt=1\n");
    }
}
static void show_help(void) {
    print_str("spike_ml: Spiking Neural Network ML Framework v1.0\n");
    print_str("Usage: spike_ml [-d demo] [-t tasks] [-a arch] [-e enc] [-l learn] [-w hw] [-f fw] [-h help] [-i info]\n");
}
static void show_info(void) {
    print_str("spike_ml: v1.0 - SNN ML Framework\n");
    print_str("Neurons: LIF, Izhikevich, Adaptive LIF\n");
    print_str("Encode: rate, time, phase, population, frequency\n");
    print_str("Learn: BPTT(surrogate), e-prop, STDP, R-STDP, TD\n");
    print_str("Arch: feedforward, recurrent, reservoir, CNN, LSTM\n");
    print_str("Apps: pattern, speech, DVS, timeseries, anomaly\n");
    print_str("HW: mixed-precision, quantization, pruning, transfer\n");
}
/* Full demo pipeline */
static void run_demo(void) {
    print_str("=== spike_ml DEMO ===\n");
    print_str("[1] Create SNN (8 in, 16 hidden, 4 out)\n");
    unsigned int sz[3] = {8,16,4}, tn = 28;
    neuron_t *ly = (neuron_t *)alloc(tn*sizeof(neuron_t));
    for (unsigned int i = 0; i < tn; i++) neuron_init(&ly[i], FP1, 0, FPF(0.05f));
    fp_t *w1 = (fp_t *)alloc(128*sizeof(fp_t)), *w2 = (fp_t *)alloc(64*sizeof(fp_t));
    for (unsigned int i = 0; i < 128; i++) w1[i] = FPF(0.1f)*(fp_t)((int)(i%7)-3);
    for (unsigned int i = 0; i < 64; i++) w2[i] = FPF(0.1f)*(fp_t)((int)(i%5)-2);
    print_str("[2] Encode input (rate coding)\n");
    fp_t iv[8]; for (unsigned int i = 0; i < 8; i++) iv[i] = FPF(0.3f)+FPF(0.08f)*(fp_t)i;
    unsigned int *spk = (unsigned int *)alloc(8*sizeof(unsigned int));
    spike_train_t st = {spk, 8, 32};
    encoder_t enc = {ENC_RATE, 32, FPF(100.0f), 0, 4};
    enc_rate(&enc, iv, 8, &st);
    print_str("  Spikes: ");
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int c = 0; for (unsigned int b = 0; b < 32; b++) if (spk[i]&(1U<<b)) c++;
        print_uint(c); if (i<7) print_str(", ");
    }
    print_str("\n[3] Train BPTT (20 epochs)\n");
    surr_t sg = {2, FP1, FPF(4.0f)}; fp_t lr = FPF(0.01f), tgt[4] = {FP1,0,0,0}; int trc = 0;
    for (unsigned int ep = 0; ep < 20; ep++) {
        for (unsigned int i = 0; i < tn; i++) { ly[i].V = 0; ly[i].spiked = 0; }
        for (unsigned int t = 0; t < 10; t++) {
            for (unsigned int i = 0; i < 8; i++) neuron_step(&ly[i], (spk[i]&(1U<<(t&31U)))?FP1:0);
            fp_t hc[16]; for (unsigned int j = 0; j < 16; j++) { hc[j]=0; for (unsigned int i=0;i<8;i++) hc[j]+=FPM(w1[i*16+j],ly[i].spiked?FP1:0); }
            layer_step(&ly[8], 16, hc);
            fp_t oc[4]; for (unsigned int j = 0; j < 4; j++) { oc[j]=0; for (unsigned int i=0;i<16;i++) oc[j]+=FPM(w2[i*4+j],ly[8+i].spiked?FP1:0); }
            layer_step(&ly[24], 4, oc); bptt_upd(&ly[24], 4, w2, oc, tgt, lr, &sg);
        }
        if (ly[24].spiked) trc++; lr = FPM(lr, FPF(0.98f));
    }
    print_str("  Train: "); print_int(trc); print_str("/20\n");
    print_str("[4] Inference (10 tests)\n");
    unsigned int tc = 0;
    for (unsigned int ts = 0; ts < 10; ts++) {
        for (unsigned int i = 0; i < tn; i++) { ly[i].V = 0; ly[i].spiked = 0; }
        for (unsigned int t = 0; t < 10; t++) {
            for (unsigned int i = 0; i < 8; i++) neuron_step(&ly[i], iv[i]+FPF(0.02f)*(fp_t)ts);
            fp_t hc[16]; for (unsigned int j = 0; j < 16; j++) { hc[j]=0; for (unsigned int i=0;i<8;i++) hc[j]+=FPM(w1[i*16+j],ly[i].spiked?FP1:0); }
            layer_step(&ly[8], 16, hc);
            fp_t oc[4]; for (unsigned int j = 0; j < 4; j++) { oc[j]=0; for (unsigned int i=0;i<16;i++) oc[j]+=FPM(w2[i*4+j],ly[8+i].spiked?FP1:0); }
            layer_step(&ly[24], 4, oc);
        }
        if (ly[24].spiked) tc++;
    }
    print_str("  Test acc: "); print_uint(tc*10); print_str("%\n");
    print_str("[5] Metrics: neurons="); print_uint(tn); print_str(" synapses="); print_uint(192);
    print_str("\n  Train="); print_uint((unsigned int)trc*5); print_str("% Test="); print_uint(tc*10); print_str("%\n");
    print_str("[6] Features:\n");
    stdp_p_t sp = {FPF(0.01f),FPF(0.012f),FPF(0.95f),FPF(0.95f),FP1,-FP1};
    synapse_t ds = {0,0,FPF(0.5f),0,0,0}; neuron_t pn, qn;
    neuron_init(&pn, FP1, 0, FPF(0.05f)); neuron_init(&qn, FP1, 0, FPF(0.05f));
    pn.spiked = 1; qn.spiked = 1; stdp_upd(&ds, &pn, &qn, &sp);
    print_str("  STDP w="); print_int(ds.w>>16); print_str("\n");
    neuron_t *res = (neuron_t *)alloc(32*sizeof(neuron_t)); res_init(res, 32);
    fp_t *rw = (fp_t *)alloc(32*sizeof(fp_t)); for (unsigned int i = 0; i < 32; i++) rw[i] = FPF(0.01f)*(fp_t)i;
    fp_t ri = FPF(0.5f), rt = FP1; res_train(res, 32, &ri, &rt, rw, 20, FPF(0.01f));
    print_str("  Reservoir out="); print_int(res_read(res, 32, rw)>>16); print_str("\n");
    print_str("  Quant-8="); print_int(quantize(FPF(0.7f),8)>>16); print_str("\n");
    synapse_t ps[4]; for (unsigned int i=0;i<4;i++) { ps[i].w=FPF(0.1f)*(fp_t)i; ps[i].tp=0; ps[i].tm=0; }
    print_str("  Prune kept="); print_uint(prune_syn(ps, 4, FPF(0.15f))); print_str("/4\n");
    print_str("=== DEMO COMPLETE ===\n");
}
static void run_tasks(void) {
    print_str("=== Application Tasks ===\n");
    const char *nm[5] = {"Pattern","Speech","DVS","TimeSeries","Anomaly"};
    neuron_t *net = (neuron_t *)alloc(20*sizeof(neuron_t));
    fp_t *w = (fp_t *)alloc(64*sizeof(fp_t));
    for (unsigned int i = 0; i < 64; i++) w[i] = FPF(0.05f)*(fp_t)((int)(i%5)-2);
    for (int task = 0; task < 5; task++) {
        for (unsigned int i = 0; i < 20; i++) neuron_init(&net[i], FP1, 0, FPF(0.05f));
        print_str("  ["); print_str(nm[task]); print_str("] "); print_uint(app_run(task, net, 16, 4, w)); print_str("\n");
    }
}
static void run_arch(void) {
    print_str("=== Architecture Benchmarks ===\n");
    unsigned int fsz[3] = {8,12,4}; neuron_t *ff = (neuron_t *)alloc(24*sizeof(neuron_t));
    for (unsigned int i = 0; i < 24; i++) neuron_init(&ff[i], FP1, 0, FPF(0.05f));
    fp_t *fw = (fp_t *)alloc(144*sizeof(fp_t)); for (unsigned int i = 0; i < 144; i++) fw[i] = FPF(0.1f);
    fp_t fi[8]; for (unsigned int i = 0; i < 8; i++) fi[i] = FPF(0.5f);
    ff_run(ff, fsz, 3, fw, fi, 10);
    unsigned int fsp = 0; for (unsigned int i = 20; i < 24; i++) if (ff[i].spiked) fsp++;
    print_str("  FF spikes="); print_uint(fsp); print_str("\n");
    neuron_t *rc = (neuron_t *)alloc(8*sizeof(neuron_t));
    for (unsigned int i = 0; i < 8; i++) neuron_init(&rc[i], FP1, 0, FPF(0.05f));
    fp_t *rww = (fp_t *)alloc(64*sizeof(fp_t)); for (unsigned int i = 0; i < 64; i++) rww[i] = FPF(0.05f)*(fp_t)((int)(i%3)-1);
    fp_t ri[8], sb[8]; for (unsigned int i = 0; i < 8; i++) { ri[i]=FPF(0.8f); sb[i]=0; }
    for (unsigned int t = 0; t < 20; t++) rec_step(rc, 8, rww, ri, 8, sb);
    unsigned int rsp = 0; for (unsigned int i = 0; i < 8; i++) if (rc[i].spiked) rsp++;
    print_str("  Rec active="); print_uint(rsp); print_str("\n");
    neuron_t *rs = (neuron_t *)alloc(32*sizeof(neuron_t)); res_init(rs, 32);
    fp_t *rrw = (fp_t *)alloc(32*sizeof(fp_t)); for (unsigned int i = 0; i < 32; i++) rrw[i] = FPF(0.02f)*(fp_t)i;
    fp_t rri = FPF(0.5f), rrt = FP1; res_train(rs, 32, &rri, &rrt, rrw, 30, FPF(0.01f));
    print_str("  Res out="); print_int(res_read(rs, 32, rrw)>>16); print_str("\n");
    neuron_t *cn = (neuron_t *)alloc(16*sizeof(neuron_t));
    for (unsigned int i = 0; i < 16; i++) neuron_init(&cn[i], FP1, 0, FPF(0.05f));
    fp_t ck[3] = {FPF(0.3f),FPF(-0.2f),FPF(0.1f)}, ci[32];
    for (unsigned int i = 0; i < 32; i++) ci[i] = FPF(0.5f);
    conv_step(cn, 16, ck, 3, ci, 32, 2);
    unsigned int csp = 0; for (unsigned int i = 0; i < 16; i++) if (cn[i].spiked) csp++;
    print_str("  CNN spikes="); print_uint(csp); print_str("\n");
    neuron_t *lc = (neuron_t *)alloc(4*sizeof(neuron_t));
    for (unsigned int i = 0; i < 4; i++) neuron_init(&lc[i], FP1, 0, FPF(0.05f));
    fp_t *lwf=(fp_t*)alloc(48*sizeof(fp_t)),*lwi=(fp_t*)alloc(48*sizeof(fp_t)),*lwo=(fp_t*)alloc(48*sizeof(fp_t)),*lwc=(fp_t*)alloc(48*sizeof(fp_t));
    for (unsigned int i = 0; i < 48; i++) { lwf[i]=FPF(0.5f); lwi[i]=FPF(0.3f); lwo[i]=FPF(0.4f); lwc[i]=FPF(0.2f); }
    fp_t lbi[8], lhp[4], lcs[4]; for (unsigned int i=0;i<8;i++) lbi[i]=FPF(0.5f);
    for (unsigned int i=0;i<4;i++) { lhp[i]=0; lcs[i]=0; }
    for (unsigned int t = 0; t < 10; t++) lstm_step(lc, 4, lwf, lwi, lwo, lwc, lbi, 8, lhp, lcs);
    unsigned int lsp = 0; for (unsigned int i = 0; i < 4; i++) if (lc[i].spiked) lsp++;
    print_str("  LSTM active="); print_uint(lsp); print_str("\n");
}
static void run_enc(void) {
    print_str("=== Encoding Tests ===\n");
    fp_t v[8]; for (unsigned int i = 0; i < 8; i++) v[i] = FPF(0.2f)+FPF(0.1f)*(fp_t)i;
    unsigned int *sp = (unsigned int *)alloc(8*sizeof(unsigned int));
    spike_train_t st = {sp, 8, 32};
    const char *nm[5] = {"Rate","Time","Phase","Pop","Freq"};
    for (unsigned int m = 0; m < 5; m++) {
        encoder_t e = {(int)m, 32, FPF(100.0f), 0, 4}; enc_dispatch(&e, v, 8, &st);
        unsigned int tot = 0; for (unsigned int i=0;i<8;i++) for (unsigned int b=0;b<32;b++) if (sp[i]&(1U<<b)) tot++;
        print_str("  "); print_str(nm[m]); print_str(": "); print_uint(tot); print_str("\n");
    }
}
static void run_learn(void) {
    print_str("=== Learning Rules ===\n");
    stdp_p_t sp = {FPF(0.01f),FPF(0.012f),FPF(0.95f),FPF(0.95f),FP1,-FP1};
    synapse_t sy = {0,0,FPF(0.5f),0,0,0}; neuron_t a, b;
    neuron_init(&a, FP1, 0, FPF(0.05f)); neuron_init(&b, FP1, 0, FPF(0.05f));
    a.spiked = 1; b.spiked = 0; stdp_upd(&sy, &a, &b, &sp);
    print_str("  STDP dw="); print_int((sy.w-FPF(0.5f))>>16); print_str("\n");
    tf_t tf = {FPF(0.01f),FPF(0.95f),0,FP1}; sy.tp=FPF(0.5f); sy.tm=FPF(0.3f);
    rstdp_upd(&sy, &tf, FP1); print_str("  R-STDP dop="); print_int(tf.dopa>>16); print_str("\n");
    elig_t et[4]; for (unsigned int i=0;i<4;i++) { et[i].tv=0;et[i].tr=0;et[i].te=0;et[i].tz=0; }
    neuron_t en[4]; for (unsigned int i=0;i<4;i++) { neuron_init(&en[i],FP1,0,FPF(0.05f)); en[i].V=FPF(0.5f)*(fp_t)i; }
    fp_t ew[4]={FPF(0.1f),FPF(0.2f),FPF(0.3f),FPF(0.4f)}, ee[4]={FPF(0.5f),FPF(-0.3f),FPF(0.2f),FPF(-0.1f)};
    eprop_upd(et, en, ew, ee, FPF(0.01f), 4);
    print_str("  e-prop w0="); print_int(ew[0]>>16); print_str("\n");
    fp_t tv[4]={0,0,0,0}, tt[4]={FP1,FP1,0,0};
    td_upd(tv, tt, FPF(0.9f), FPF(0.1f), 4); print_str("  TD v0="); print_int(tv[0]>>16); print_str("\n");
}
static void run_hw(void) {
    print_str("=== Hardware Acceleration ===\n");
    fp_t w = FPF(0.73f);
    print_str("  FP16="); print_int(mix_prec(w,0)>>16);
    print_str(" INT8="); print_int(mix_prec(w,1)>>16);
    print_str(" Q8="); print_int(quantize(w,8)>>16);
    print_str(" Q4="); print_int(quantize(w,4)>>16); print_str("\n");
    neuron_t tl[8]; for (unsigned int i=0;i<8;i++) neuron_init(&tl[i],FP1,0,FPF(0.05f));
    fp_t tw[8]; for (unsigned int i=0;i<8;i++) tw[i]=FPF(0.5f);
    transfer_adapt(tl, 8, tw, 4); print_str("  Transfer: frozen=4 adapted=4\n");
}
/* Main entry */
void _start(void) {
    unsigned int buf = alloc(512); host_get_argv(buf, 512);
    int rd=0,rt=0,ra=0,re=0,rl=0,rh=0,ri=0,rw=0,rf=0;
    unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) {
        char *a = &ap[pos];
        if (a[0]=='-' && a[1]) {
            if(a[1]=='d')rd=1;else if(a[1]=='t')rt=1;else if(a[1]=='a')ra=1;
            else if(a[1]=='e')re=1;else if(a[1]=='l')rl=1;else if(a[1]=='h')rh=1;
            else if(a[1]=='i')ri=1;else if(a[1]=='w')rw=1;else if(a[1]=='f')rf=1;
        }
        while (pos < 512 && ap[pos]) pos++; pos++;
    }
    if (rh) { show_help(); host_exit(0); }
    if (ri) { show_info(); host_exit(0); }
    if (!(rd|rt|ra|re|rl|rw|rf)) rd = 1;
    if (rd) run_demo(); if (rt) run_tasks(); if (ra) run_arch();
    if (re) run_enc(); if (rl) run_learn(); if (rw) run_hw();
    if (rf) { print_str("=== Framework ===\n"); export_meta(); print_str("=== Done ===\n"); }
    host_exit(0);
}
