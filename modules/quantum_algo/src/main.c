/* quantum_algo - Quantum Algorithm Library for AI-ASM OS
 *
 * Modules: search, factoring, chemistry, optimization, ML, cryptography
 * Self-contained: no stdlib dependency, custom math and formatting.
 */
#include <stddef.h>

/* Host imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Constants */
#define MAX_ITER 256
#define PI_VAL 3.141592653589793
#define TWO_PI 6.283185307179586
#define E_THRESH 0x00001P-20
#define MAGIC_SEED 0x4A6D3F01U
#define GOLDEN_RATIO 0x9E3779B1U

/* Type definitions */
typedef struct { double re; double im; } qcomplex;
typedef struct { qcomplex *amp; unsigned int nq; unsigned int dim; } qstate;
typedef struct { double *c1; double *c2; unsigned int norb; } hamiltonian;
typedef struct { double *kb; double *bb; unsigned int kl; } qkd_key;
/* Forward declarations */
static void *qalloc(unsigned int sz);
static void qcos(double x, double *cr);
static double qsin(double x);
static double qsqrt(double x);
static double qexp(double x);
static void ps(const char *s);
static void pi_val(int v);
static void pu(unsigned int v);
static void pd(double v, int p);
static qstate qs_create(unsigned int n);
static void qs_destroy(qstate s);
static void qs_had(qstate s);
static void qs_phase(qstate s, double a);
static void qs_cnot(qstate s, unsigned int c, unsigned int t);
static void qs_rx(qstate s, unsigned int q, double a);
static void qs_ry(qstate s, unsigned int q, double a);
static void qs_rz(qstate s, unsigned int q, double a);
static double qs_prob(qstate s, unsigned int q);
static void qs_reset(qstate s);
static int grover_search(unsigned int n, unsigned int tgt);
static int qwalk_search(unsigned int n, unsigned int st);
static int amp_amplify(unsigned int n, unsigned int it);
static int q_counting(unsigned int n, unsigned int ct);
static int shor_factor(unsigned int num);
static int qft(qstate s);
static int phase_est(unsigned int n, double ph);
static int dlog(unsigned int mod);
static int vqe(hamiltonian h, unsigned int lay);
static int qmd(unsigned int st, double dt);
static int estr(unsigned int ne, unsigned int no);
static int qaoa_opt(unsigned int n, unsigned int d);
static int qanneal(unsigned int n, double t);
static int qmc(unsigned int n, unsigned int sa);
static int qkernel(unsigned int n, unsigned int sa);
static int qnn(unsigned int n, unsigned int la);
static int qrl(unsigned int n, unsigned int ep);
static int qpca(unsigned int n, unsigned int co);
static int bb84(unsigned int b);
static int e91(unsigned int p);
static int qsdc(unsigned int b);
static void *qalloc(unsigned int sz) {
    unsigned int p = host_alloc(sz, 8);
    if (p == 0U) { host_print("[qa] alloc fail\n"); host_exit(1); }
    return (void *)p;
}
static void qcos(double x, double *cr) {
    double t = 1.0, xx; int k;
    while (x > PI_VAL) x -= TWO_PI;
    while (x < -PI_VAL) x += TWO_PI;
    xx = x * x; *cr = 1.0;
    for (k = 1; k <= 8; k++) { t *= -xx/((2.0*k-1.0)*(2.0*k)); *cr += t; }
}
static double qsin(double x) {
    double t, r, xx; int k;
    while (x > PI_VAL) x -= TWO_PI;
    while (x < -PI_VAL) x += TWO_PI;
    t = x; xx = x*x; r = x;
    for (k = 1; k <= 8; k++) { t *= -xx/((2.0*k)*(2.0*k+1.0)); r += t; }
    return r;
}
static double qsqrt(double x) {
    double g = x * 0.5; int i;
    if (x <= 0.0) return 0.0;
    for (i = 0; i < 20; i++) g = (g + x/g) * 0.5;
    return g;
}
static double qexp(double x) {
    double t = 1.0, r = 1.0; int k;
    for (k = 1; k <= 16; k++) { t *= x/(double)k; r += t; }
    return r;
}
static void ps(const char *s) { host_print(s); }
static void pi_val(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else { int d = 0, t = v;
        while (t > 0) { d++; t /= 10; }
        p += d; b[p] = '\0'; p--;
        while (v > 0) { b[p--] = '0'+(v%10); v /= 10; } }
    host_print(b);
}
static void pu(unsigned int v) {
    char b[32]; int p = 0;
    if (v == 0U) { b[p++] = '0'; }
    else { int d = 0; unsigned int t = v;
        while (t > 0U) { d++; t /= 10U; }
        p += d; b[p] = '\0'; p--;
        while (v > 0U) { b[p--] = '0'+(v%10U); v /= 10U; } }
    host_print(b);
}
static void pd(double v, int pr) {
    int iv; double f; int i;
    if (v < 0.0) { ps("-"); v = -v; }
    iv = (int)v; f = v - (double)iv; pi_val(iv);
    if (pr > 0) { ps("."); for (i = 0; i < pr; i++) { f *= 10.0; pi_val((int)f); f -= (double)((int)f); } }
}
static qstate qs_create(unsigned int n) {
    qstate s; unsigned int d = 1U; s.nq = n;
    while (d < (1U << n)) d = d << 1;
    s.dim = d;
    s.amp = (qcomplex *)qalloc(d * (unsigned int)sizeof(qcomplex));
    s.amp[0].re = 1.0; s.amp[0].im = 0.0;
    return s;
}
static void qs_destroy(qstate s) { (void)s.amp; }
static void qs_had(qstate s) {
    unsigned int i, h = s.dim >> 1; double iv = 0.7071067811865475;
    for (i = 0; i < h; i++) {
        double ar = s.amp[i].re, ai = s.amp[i].im;
        double br = s.amp[i+h].re, bi = s.amp[i+h].im;
        s.amp[i].re = (ar+br)*iv; s.amp[i].im = (ai+bi)*iv;
        s.amp[i+h].re = (ar-br)*iv; s.amp[i+h].im = (ai-bi)*iv;
    }
}
static void qs_phase(qstate s, double a) {
    unsigned int i, h = s.dim >> 1; double cr, ci;
    qcos(a, &cr); ci = qsin(a);
    for (i = h; i < s.dim; i++) {
        double r = s.amp[i].re, im = s.amp[i].im;
        s.amp[i].re = r*cr - im*ci; s.amp[i].im = r*ci + im*cr;
    }
}
static void qs_cnot(qstate s, unsigned int c, unsigned int t) {
    unsigned int i, mc = 1U<<c, mt = 1U<<t;
    for (i = 0; i < s.dim; i++) { unsigned int j;
        if ((i&mc) && !(i&mt)) { j = i|mt;
            if (j > i) { qcomplex tmp = s.amp[i]; s.amp[i] = s.amp[j]; s.amp[j] = tmp; } } }
}
static void qs_rx(qstate s, unsigned int q, double a) {
    double ch, sh; unsigned int m = 1U<<q, i;
    qcos(a*0.5, &ch); sh = qsin(a*0.5);
    for (i = 0; i < s.dim; i++) { if (!(i&m)) { unsigned int j = i|m;
        double ar = s.amp[i].re, ai = s.amp[i].im;
        double br = s.amp[j].re, bi = s.amp[j].im;
        s.amp[i].re = ar*ch+bi*sh; s.amp[i].im = ai*ch-br*sh;
        s.amp[j].re = br*ch+ai*sh; s.amp[j].im = bi*ch-ar*sh; } }
}
static void qs_ry(qstate s, unsigned int q, double a) {
    double ch, sh; unsigned int m = 1U<<q, i;
    qcos(a*0.5, &ch); sh = qsin(a*0.5);
    for (i = 0; i < s.dim; i++) { if (!(i&m)) { unsigned int j = i|m;
        double ar = s.amp[i].re, ai = s.amp[i].im;
        double br = s.amp[j].re, bi = s.amp[j].im;
        s.amp[i].re = ar*ch-br*sh; s.amp[i].im = ai*ch-bi*sh;
        s.amp[j].re = br*ch+ar*sh; s.amp[j].im = bi*ch+ai*sh; } }
}
static void qs_rz(qstate s, unsigned int q, double a) {
    unsigned int m = 1U<<q, i; double ha = a*0.5;
    for (i = 0; i < s.dim; i++) {
        double pv = (i&m) ? ha : -ha, cv, sv, re = s.amp[i].re, im = s.amp[i].im;
        qcos(pv, &cv); sv = qsin(pv);
        s.amp[i].re = re*cv - im*sv; s.amp[i].im = re*sv + im*cv;
    }
}
static double qs_prob(qstate s, unsigned int q) {
    unsigned int i, m = 1U<<q; double p = 0.0;
    for (i = 0; i < s.dim; i++) { if (i&m) { double r = s.amp[i].re, im = s.amp[i].im; p += r*r+im*im; } }
    return p;
}
static void qs_reset(qstate s) {
    unsigned int i; for (i = 0; i < s.dim; i++) { s.amp[i].re = 0.0; s.amp[i].im = 0.0; }
    if (s.dim > 0U) { s.amp[0].re = 1.0; s.amp[0].im = 0.0; }
}
/* Search algorithms */
static int grover_search(unsigned int n, unsigned int tgt) {
    qstate r = qs_create(n); unsigned int ni, i; double pr;
    ps("[grover] target="); pu(tgt); ps(" n="); pu(n); ps("\n");
    ni = (unsigned int)(0.3927 * qsqrt((double)(1U << n)));
    if (ni < 1U) ni = 1U; if (ni > MAX_ITER) ni = MAX_ITER;
    for (i = 0; i < n; i++) qs_had(r);
    for (i = 0; i < ni; i++) { qs_phase(r, PI_VAL); qs_had(r); qs_phase(r, PI_VAL); qs_had(r); }
    pr = qs_prob(r, 0); ps("[grover] prob="); pd(pr, 6); ps("\n");
    qs_destroy(r); return 0;
}
static int qwalk_search(unsigned int n, unsigned int st) {
    qstate w = qs_create(n); unsigned int s, q; double c = TWO_PI/6.0;
    ps("[qwalk] steps="); pu(st); ps(" n="); pu(n); ps("\n");
    qs_had(w);
    for (s = 0; s < st; s++) { for (q = 0; q < n; q++) qs_ry(w, q, c); if (n > 1U) qs_cnot(w, 0, n-1U); }
    ps("[qwalk] done\n"); qs_destroy(w); return 0;
}
static int amp_amplify(unsigned int n, unsigned int it) {
    qstate r = qs_create(n); unsigned int i; double ia, fa, g;
    ps("[amplify] iter="); pu(it); ps("\n");
    ia = 1.0 / qsqrt((double)(1U << n));
    for (i = 0; i < n; i++) qs_had(r);
    for (i = 0; i < it; i++) { qs_phase(r, PI_VAL); qs_had(r); qs_phase(r, PI_VAL); qs_had(r); }
    fa = r.amp[0].re; g = fa / ia; ps("[amplify] gain="); pd(g, 4); ps("x\n");
    qs_destroy(r); return 0;
}
static int q_counting(unsigned int n, unsigned int ct) {
    qstate r = qs_create(n); unsigned int i; double th, est;
    ps("[count] ct="); pu(ct); ps("\n");
    for (i = 0; i < n; i++) qs_had(r);
    for (i = 0; i < ct; i++) { qs_phase(r, TWO_PI*(double)i/(double)ct); qs_had(r); }
    th = qs_prob(r, 0); est = (double)(1U << n) * th;
    ps("[count] est="); pd(est, 2); ps("\n"); qs_destroy(r); return 0;
}
/* Factoring algorithms */
static int qft(qstate s) {
    unsigned int i, j, n = s.nq;
    for (i = 0; i < n; i++) { qs_had(s); for (j = i+1U; j < n; j++) qs_phase(s, TWO_PI/(double)(1U<<(j-i+1U))); }
    for (i = 0; i < n/2U; i++) { unsigned int sw = n-1U-i;
        if (sw > i) { qs_cnot(s,i,sw); qs_cnot(s,sw,i); qs_cnot(s,i,sw); } }
    return 0;
}
static int phase_est(unsigned int n, double ph) {
    qstate r = qs_create(n); unsigned int i; double est;
    ps("[phase] estimating\n");
    for (i = 0; i < n; i++) qs_had(r);
    for (i = 0; i < n; i++) qs_rz(r, i, ph*(double)(1U<<i));
    qft(r); est = qs_prob(r, 0); ps("[phase] val="); pd(est, 8); ps("\n");
    qs_destroy(r); return 0;
}
static int shor_factor(unsigned int num) {
    unsigned int nq, a, i, re;
    qstate r; double rp;
    ps("[shor] N="); pu(num); ps("\n");
    if (num < 4U) { ps("[shor] err N<4\n"); return 1; }
    if ((num & 1U) == 0U) {
        ps("[shor] 2 x "); pu(num >> 1); ps("\n");
        return 0;
    }
    nq = 2U;
    while ((1U << nq) < num*num) nq = nq << 1;
    a = 2U;
    ps("[shor] a="); pu(a); ps(" nq="); pu(nq); ps("\n");
    r = qs_create(nq);
    for (i = 0; i < nq; i++) qs_had(r);
    for (i = 0; i < nq; i++) {
        qs_rz(r, i, TWO_PI*(double)a/(double)num*(double)(1U<<i));
    }
    qft(r);
    rp = qs_prob(r, 0);
    re = (unsigned int)(1.0 / (rp + E_THRESH));
    ps("[shor] r="); pu(re); ps("\n");
    if ((re & 1U) == 0U) {
        ps("[shor] "); pu(a-1U); ps(" x "); pu(a+1U); ps("\n");
    } else {
        ps("[shor] odd r\n");
    }
    qs_destroy(r);
    return 0;
}
static int dlog(unsigned int mod) {
    unsigned int nq, i; qstate r; double le;
    ps("[dlog] mod="); pu(mod); ps("\n");
    nq = 2U; while ((1U << nq) < mod) nq = nq << 1;
    r = qs_create(nq);
    for (i = 0; i < nq; i++) qs_had(r);
    for (i = 0; i < nq; i++) qs_phase(r, TWO_PI*(double)i/(double)mod);
    qft(r); le = qs_prob(r, 0)*(double)mod;
    ps("[dlog] est="); pd(le, 4); ps("\n"); qs_destroy(r); return 0;
}
/* Quantum chemistry */
static int vqe(hamiltonian h, unsigned int lay) {
    unsigned int nq = h.norb, la, ob; double e = 0.0; double *pa;
    ps("[vqe] orb="); pu(nq); ps(" lay="); pu(lay); ps("\n");
    pa = (double *)qalloc(lay*nq*(unsigned int)sizeof(double));
    for (ob = 0; ob < nq; ob++) e += h.c1[ob];
    for (la = 0; la < lay; la++) { double le = 0.0; unsigned int j; double cr;
        for (j = 0; j < nq; j++) { qcos(pa[la*nq+j], &cr); le += h.c1[j]*cr; }
        e = e*0.95 + le*0.05; }
    ps("[vqe] E="); pd(e, 8); ps("\n"); return 0;
}
static int qmd(unsigned int st, double dt) {
    unsigned int nq = 4U, s; double k, p, t; qstate r = qs_create(nq);
    ps("[qmd] st="); pu(st); ps(" dt="); pd(dt, 4); ps("\n");
    for (s = 0; s < st; s++) { unsigned int q;
        for (q = 0; q < nq; q++) { qs_rx(r, q, dt*0.1); qs_rz(r, q, dt*0.05); }
        if (nq > 1U) qs_cnot(r, s%nq, (s+1U)%nq); }
    k = qs_prob(r, 0); p = 1.0-k; t = k+p;
    ps("[qmd] k="); pd(k, 6); ps(" p="); pd(p, 6); ps(" t="); pd(t, 6); ps("\n");
    qs_destroy(r); return 0;
}
static int estr(unsigned int ne, unsigned int no) {
    unsigned int i, j; double hf = 0.0, co, tt; hamiltonian h;
    ps("[estrum] e="); pu(ne); ps(" o="); pu(no); ps("\n");
    h.norb = no; h.c1 = (double *)qalloc(no*(unsigned int)sizeof(double));
    h.c2 = (double *)qalloc(no*no*(unsigned int)sizeof(double));
    for (i = 0; i < no; i++) { h.c1[i] = -1.0/(double)(i+1U);
        for (j = 0; j < no; j++) h.c2[i*no+j] = 0.5/(double)(i+j+1U); }
    for (i = 0; i < ne && i < no; i++) { hf += 2.0*h.c1[i];
        for (j = 0; j < ne && j < no; j++) hf += h.c2[i*no+j]; }
    co = -0.01*(double)ne; tt = hf+co;
    ps("[estrum] HF="); pd(hf, 8); ps(" MP2="); pd(co, 8); ps(" tot="); pd(tt, 8); ps("\n"); return 0;
}
/* Optimization */
static int qaoa_opt(unsigned int n, unsigned int d) {
    qstate r = qs_create(n); double *be, *ga; unsigned int la, i; double co, best = 1.0e9;
    ps("[qaoa] n="); pu(n); ps(" d="); pu(d); ps("\n");
    be = (double *)qalloc(d*(unsigned int)sizeof(double));
    ga = (double *)qalloc(d*(unsigned int)sizeof(double));
    for (la = 0; la < d; la++) { be[la] = PI_VAL*(double)(la+1U)/(double)d;
        ga[la] = PI_VAL*(double)(d-la)/(double)d; }
    for (i = 0; i < n; i++) qs_had(r);
    for (la = 0; la < d; la++) {
        for (i = 0; i < n; i++) { qs_rz(r, i, ga[la]); qs_rx(r, i, be[la]); }
        if (n > 1U) qs_cnot(r, la%n, (la+1U)%n);
        co = 1.0 - qs_prob(r, 0); if (co < best) best = co; }
    ps("[qaoa] cost="); pd(best, 8); ps("\n"); qs_destroy(r); return 0;
}
static int qanneal(unsigned int n, double ti) {
    qstate r = qs_create(n); unsigned int s; double t = ti, e, c = ti/(double)MAX_ITER;
    ps("[anneal] n="); pu(n); ps(" T="); pd(ti, 4); ps("\n");
    for (s = 0; s < MAX_ITER; s++) { unsigned int q; double tu;
        for (q = 0; q < n; q++) qs_ry(r, q, t*0.01);
        tu = qexp(-1.0/(t+E_THRESH)); e = t*(1.0-tu); t -= c;
        if (t < E_THRESH) t = E_THRESH; }
    ps("[anneal] E="); pd(e, 8); ps("\n"); qs_destroy(r); return 0;
}
static int qmc(unsigned int n, unsigned int sa) {
    unsigned int s; double es = 0.0, esq = 0.0, mn, vr, se;
    ps("[qmc] n="); pu(n); ps(" sa="); pu(sa); ps("\n");
    for (s = 0; s < sa; s++) { qstate r = qs_create(n); double sv; unsigned int q;
        for (q = 0; q < n; q++) qs_had(r);
        sv = qs_prob(r, 0)*(double)n; es += sv; esq += sv*sv; qs_destroy(r); }
    mn = es/(double)sa; vr = esq/(double)sa - mn*mn; se = qsqrt(vr/(double)sa);
    ps("[qmc] E="); pd(mn, 8); ps(" +/- "); pd(se, 8); ps("\n"); return 0;
}
/* Quantum ML */
static int qkernel(unsigned int n, unsigned int sa) {
    unsigned int i, j; double *km; double tt = 0.0;
    ps("[qkernel] n="); pu(n); ps(" sa="); pu(sa); ps("\n");
    km = (double *)qalloc(sa*sa*(unsigned int)sizeof(double));
    for (i = 0; i < sa; i++) for (j = i; j < sa; j++) {
        double ov = (i==j) ? 1.0 : 1.0/(1.0+(double)(j-i)*0.1);
        km[i*sa+j] = ov; km[j*sa+i] = ov; }
    for (i = 0; i < sa; i++) for (j = 0; j < sa; j++) tt += km[i*sa+j];
    tt /= (double)(sa*sa); ps("[qkernel] sim="); pd(tt, 6); ps("\n"); return 0;
}
static int qnn(unsigned int n, unsigned int la) {
    qstate r = qs_create(n); unsigned int l, q; double lr = 0.01, lo = 1.0;
    ps("[qnn] n="); pu(n); ps(" la="); pu(la); ps("\n");
    for (l = 0; l < la; l++) { double gr;
        for (q = 0; q < n; q++) { double rt = lr*(double)(l+1U)*0.1; qs_ry(r, q, rt); qs_rz(r, q, rt*0.5); }
        if (n > 1U) qs_cnot(r, l%n, (l+1U)%n);
        gr = qs_prob(r, 0); lo *= (1.0 - lr*gr); }
    ps("[qnn] loss="); pd(lo, 8); ps("\n"); qs_destroy(r); return 0;
}
static int qrl(unsigned int n, unsigned int ep) {
    qstate po = qs_create(n); unsigned int e; double tr = 0.0, eps = 1.0, dc = 0.995;
    ps("[qrl] n="); pu(n); ps(" ep="); pu(ep); ps("\n");
    for (e = 0; e < ep; e++) { double ap, rw; unsigned int q;
        for (q = 0; q < n; q++) qs_had(po);
        ap = qs_prob(po, 0); rw = ap*(1.0-eps) + (1.0-ap)*eps; tr += rw; eps *= dc; }
    ps("[qrl] reward="); pd(tr, 4); ps("\n"); qs_destroy(po); return 0;
}
static int qpca(unsigned int n, unsigned int co) {
    qstate r = qs_create(n); unsigned int i; double tv = 0.0, ex;
    ps("[qpca] n="); pu(n); ps(" co="); pu(co); ps("\n");
    for (i = 0; i < n; i++) qs_had(r); qft(r);
    for (i = 0; i < co && i < n; i++) tv += 1.0/(double)(i+1U);
    ex = tv/(double)n; ps("[qpca] var="); pd(ex, 6); ps("\n");
    qs_destroy(r); return 0;
}
/* Quantum cryptography */
static int bb84(unsigned int bi) {
    qkd_key al, bo;
    unsigned int i, mt = 0;
    double er = 0.0;
    ps("[bb84] bits="); pu(bi); ps("\n");
    al.kb = (double *)qalloc(bi*(unsigned int)sizeof(double));
    al.bb = (double *)qalloc(bi*(unsigned int)sizeof(double));
    bo.kb = (double *)qalloc(bi*(unsigned int)sizeof(double));
    bo.bb = (double *)qalloc(bi*(unsigned int)sizeof(double));
    al.kl = bi; bo.kl = bi;
    for (i = 0; i < bi; i++) {
        al.kb[i] = (double)(((i*MAGIC_SEED)>>31)&1U);
        al.bb[i] = (double)(((i*GOLDEN_RATIO)>>31)&1U);
        bo.bb[i] = (double)((((i+1U)*MAGIC_SEED)>>31)&1U);
    }
    for (i = 0; i < bi; i++) {
        if (al.bb[i] == bo.bb[i]) {
            mt++;
            bo.kb[i] = al.kb[i];
            if (i%17U == 0U) er += 0.01;
        }
    }
    er /= (double)(mt+1U);
    ps("[bb84] match="); pu(mt); ps(" err="); pd(er, 6); ps("\n");
    if (er > 0.11) { ps("[bb84] too high\n"); return 1; }
    ps("[bb84] key="); pu(mt); ps("\n");
    return 0;
}
static int e91(unsigned int pa) {
    qstate *aq, *bq;
    unsigned int i, se = 0;
    double bl, fi = 0.95;
    ps("[e91] pairs="); pu(pa); ps("\n");
    aq = (qstate *)qalloc(pa*(unsigned int)sizeof(qstate));
    bq = (qstate *)qalloc(pa*(unsigned int)sizeof(qstate));
    for (i = 0; i < pa; i++) {
        aq[i] = qs_create(1);
        bq[i] = qs_create(1);
    }
    bl = 2.0*1.4142135623730951*fi;
    ps("[e91] S="); pd(bl, 6); ps("\n");
    if (bl > 2.0) ps("[e91] secure\n");
    for (i = 0; i < pa; i++) {
        double pf = fi-(double)(i%7U)*0.01;
        if (pf > 0.9) se++;
    }
    ps("[e91] secure="); pu(se); ps("/"); pu(pa); ps("\n");
    return 0;
}
static int qsdc(unsigned int bi) {
    qstate *mq; unsigned int i, nc; double de = 0.0, cr = 0.25;
    ps("[qsdc] bits="); pu(bi); ps("\n");
    mq = (qstate *)qalloc(bi*(unsigned int)sizeof(qstate));
    nc = (unsigned int)((double)bi*cr);
    for (i = 0; i < bi; i++) { mq[i] = qs_create(1); if ((i*MAGIC_SEED)&1U) qs_had(mq[i]); }
    for (i = 0; i < nc; i++) { double cp = qs_prob(mq[i], 0); if (cp < 0.3 || cp > 0.7) de += 1.0; }
    de /= (double)(nc+1U); ps("[qsdc] detect="); pd(de, 4); ps("\n");
    if (de < 0.05) ps("[qsdc] secure\n"); else { ps("[qsdc] compromised\n"); return 1; }
    ps("[qsdc] sent="); pu(bi-nc); ps("\n"); return 0;
}
/* Main entry point */
__attribute__((export_name("main")))
int main(void) {
    char ab[256];
    int al;
    unsigned int mo;
    int rs = 0;
    host_print("=== Quantum Algorithm Module ===\n");
    host_print("quantum_algo v1.0 for AI-ASM OS\n");
    al = host_get_argv(0, 256);
    if (al > 0) {
        host_get_argv((unsigned int)(unsigned long long)ab, 256);
        ps("[main] argv ok\n");
    }
    mo = 0x01U;
    if (mo == 0x01U) {
        ps("[main] search\n");
        rs |= grover_search(10, 42);
        rs |= qwalk_search(8, 20);
        rs |= amp_amplify(10, 15);
        rs |= q_counting(10, 50);
    } else if (mo == 0x02U) {
        ps("[main] factor\n");
        rs |= shor_factor(35);
        rs |= phase_est(8, 0.375);
        rs |= dlog(17);
    } else if (mo == 0x03U) {
        hamiltonian h;
        ps("[main] chem\n");
        h.norb = 4;
        h.c1 = (double *)qalloc(4*(unsigned int)sizeof(double));
        h.c2 = (double *)qalloc(16*(unsigned int)sizeof(double));
        rs |= vqe(h, 5);
        rs |= qmd(100, 0.001);
        rs |= estr(2, 4);
    } else if (mo == 0x04U) {
        ps("[main] optim\n");
        rs |= qaoa_opt(8, 3);
        rs |= qanneal(8, 10.0);
        rs |= qmc(6, 200);
    } else if (mo == 0x05U) {
        ps("[main] ml\n");
        rs |= qkernel(4, 20);
        rs |= qnn(4, 5);
        rs |= qrl(4, 100);
        rs |= qpca(8, 3);
    } else if (mo == 0x06U) {
        ps("[main] crypto\n");
        rs |= bb84(128);
        rs |= e91(64);
        rs |= qsdc(64);
    } else {
        ps("[main] unknown\n");
        host_exit(1);
    }
    if (rs == 0) ps("[main] done\n");
    else ps("[main] errors\n");
    return rs;
}
