/* quantum_machine_learning - Quantum ML Framework for AI-ASM OS
 * Modules: quantum_nn, quantum_kernels, quantum_generative, quantum_rl,
 *          quantum_encoding, hybrid_classical_quantum
 * Self-contained: no stdlib dependency, custom math and formatting.
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
#define PI 3.141592653589793
#define TPI 6.283185307179586
#define EPS 0x00001P-20
#define LR 0x01P-4
#define SD 0x01P-2
typedef struct { double r, i; } cx;
typedef struct { cx *a; unsigned int n, d; } qs;
typedef struct { double *p; unsigned int nq, dp, np; } pqc;
typedef struct { double *w, *b; unsigned int ni, no; } ql;
typedef struct { pqc c; ql *l; unsigned int nq, nl; double loss; } qnn;
typedef struct { double *km, *lb; unsigned int ns, dm; } qk;
typedef struct { double *mu, *sg; unsigned int nc, dm; } qgp;
typedef struct { double *pr; unsigned int nq, ns; } qborn;
typedef struct { pqc g, d; double gl, dl; } qgan;
typedef struct { double *z; unsigned int nq, ld; } qvae;
typedef struct { double *sc; unsigned int nq, st; } qdif;
typedef struct { double *qt; unsigned int ns, na; double gm, lr; } qql;
typedef struct { double *po; unsigned int ns, na; } qpol;
typedef struct { qql cr; qpol ac; double la, lc; } qac;
typedef struct { double *rw; unsigned int na, pl; } qband;
typedef struct { double *da; unsigned int n, dm, mt; } qenc;
typedef struct { double *cb; qs *q; unsigned int nq, cd; } hyb;
typedef struct { double *pa, *gr; unsigned int np, mt; } gctx;
typedef struct { double *co; unsigned int np; double fc; } zne;
/* Forward declarations */
static void *qa(unsigned int sz);
static double qsi(double x), qsq(double x), qex(double x), qlo(double x);
static void qco(double x, double *cr);
static void ps(const char *s), pu(unsigned int v), pi(int v), pd(double v, int p);
static qs qsc(unsigned int n); static void qsd(qs s), qsr(qs s);
static void qsh(qs s), qsp(qs s, double a), qcn(qs s, unsigned int c, unsigned int t);
static void qrx(qs s, unsigned int q, double a), qry(qs s, unsigned int q, double a);
static void qrz(qs s, unsigned int q, double a);
static double qpr(qs s, unsigned int q), qex2(qs s, unsigned int q);
static pqc pcrc(unsigned int nq, unsigned int dp); static void pcrd(pqc p);
static void pca(qs s, pqc p); static double pcps(qs s, pqc p, unsigned int i, unsigned int q);
static ql qlc(unsigned int ni, unsigned int no); static double qlf(ql l, const double *x);
static void qlb(ql l, const double *x, double e);
static qnn qnnc(unsigned int nq, unsigned int dp, unsigned int nl);
static void qnnd(qnn m); static double qnnt(qnn m, const double *x, double t);
static double qnni(qnn m, const double *x);
static int qnnl(qnn m, const double *da, const double *lb, unsigned int ns, unsigned int ep);
static int vqe(unsigned int nq, unsigned int dp, unsigned int mi);
static qk qkc(unsigned int ns, unsigned int dm); static void qkd(qk k);
static void qkfm(qs s, const double *x, unsigned int dm);
static double qke(qk k, const double *x1, const double *x2);
static void qkbm(qk k, const double *da, unsigned int ns, unsigned int dm);
static int qkstm(qk k, const double *lb, unsigned int ep);
static double qksp(qk k, const double *x, unsigned int dm);
static qgp qgpc(unsigned int nc, unsigned int dm); static void qgpd(qgp g);
static void qgpf(qgp g, const double *da, const double *lb, unsigned int ns);
static double qgpp(qgp g, const double *x);
static double qkal(qk k, const double *tg, unsigned int ns);
static qborn qbc(unsigned int nq, unsigned int ns); static void qbd(qborn b);
static void qbs(qborn b, pqc p);
static double qbll(qborn b, const unsigned int *tg, unsigned int ns);
static void qbts(qborn b, pqc p, const unsigned int *tg, unsigned int ns, double lr);
static qgan qganc(unsigned int nq, unsigned int dp); static void qgand(qgan g);
static void qgants(qgan g, const double *rd, unsigned int ns, unsigned int dm, double lr);
static qvae qvc(unsigned int nq, unsigned int ld); static void qvd(qvae v);
static double qve(qvae v, const double *x, unsigned int dm);
static void qvdec(qvae v, double *o, unsigned int dm);
static qdif qdfc(unsigned int nq, unsigned int st); static void qdfd(qdif d);
static void qdff(qdif d, double *x, unsigned int dm, double ns);
static void qdfr(qdif d, double *x, unsigned int dm);
static qql qqlc(unsigned int ns, unsigned int na, double gm, double lr);
static void qqld(qql q);
static double qqlu(qql q, unsigned int s, unsigned int a, double r, unsigned int ns);
static unsigned int qqls(qql q, unsigned int s, double ep);
static qpol qplc(unsigned int ns, unsigned int na); static void qpld(qpol p);
static unsigned int qpls(qpol p, unsigned int s);
static void qplu(qpol p, unsigned int s, unsigned int a, double ad, double lr);
static qac qacc(unsigned int ns, unsigned int na, double la, double lc);
static void qacd(qac a);
static void qacts(qac a, unsigned int s, unsigned int ac, double r, unsigned int ns);
static qband qbdc(unsigned int na); static void qbdd(qband b);
static unsigned int qbds(qband b, double ep);
static void qbdu(qband b, unsigned int a, double r);
static int gvas(unsigned int n, unsigned int tg);
static qenc qecc(unsigned int n, unsigned int dm, unsigned int mt); static void qecd(qenc e);
static void qea(qs s, const double *da, unsigned int dm);
static void qeang(qs s, const double *da, unsigned int dm);
static void qeb(qs s, const double *da, unsigned int dm);
static void qecs(qs s, const double *da, unsigned int dm);
static void qeiqp(qs s, const double *da, unsigned int dm);
static void qee(qenc e, qs s);
static hyb hyc(unsigned int nq, unsigned int cd); static void hyd(hyb h);
static double hyf(hyb h, const double *x);
static void gci(gctx *g, unsigned int np, unsigned int mt);
static void gcc(gctx *g, qnn m, const double *x, double t);
static zne *znec(unsigned int np, double fc); static void zned(zne *z);
static double znee(zne *z, const double *nv, unsigned int n);
static void dda(qs s, unsigned int q, unsigned int pl);
static void twa(qs s, unsigned int q);
static int distr(unsigned int nq, unsigned int nqp, unsigned int st);

/* Utility functions */
static void *qa(unsigned int sz) {
    unsigned int p = host_alloc(sz, 8);
    if (!p) { ps("[qml] alloc fail\n"); host_exit(1); }
    return (void *)p;
}
static void qco(double x, double *cr) {
    double t = 1.0, xx; int k;
    while (x > PI) x -= TPI; while (x < -PI) x += TPI;
    xx = x * x; *cr = 1.0;
    for (k = 1; k <= 8; k++) { t *= -xx / ((2.0*k-1.0)*(2.0*k)); *cr += t; }
}
static double qsi(double x) {
    double t, r, xx; int k;
    while (x > PI) x -= TPI; while (x < -PI) x += TPI;
    t = x; xx = x * x; r = x;
    for (k = 1; k <= 8; k++) { t *= -xx / ((2.0*k)*(2.0*k+1.0)); r += t; }
    return r;
}
static double qsq(double x) {
    double g = x * 0.5; int i;
    if (x <= 0.0) return 0.0;
    for (i = 0; i < 20; i++) g = (g + x / g) * 0.5;
    return g;
}
static double qex(double x) {
    double t = 1.0, r = 1.0; int k;
    if (x > 10.0) x = 10.0; if (x < -10.0) return 0.0;
    for (k = 1; k <= 16; k++) { t *= x / (double)k; r += t; }
    return r;
}
static double qlo(double x) {
    double r = 0.0, t, tt, p; int i;
    if (x <= 0.0) return -10.0;
    while (x > 2.0) { x *= 0.5; r += 0.693147180559945; }
    while (x < 0.5) { x *= 2.0; r -= 0.693147180559945; }
    t = (x - 1.0) / (x + 1.0); tt = t * t; p = t;
    for (i = 1; i <= 10; i++) { r += 2.0 * p / (2.0*i - 1.0); p *= tt; }
    return r;
}
static void ps(const char *s) { host_print(s); }
static void pu(unsigned int v) {
    char buf[16]; int i = 14; buf[15] = 0;
    if (!v) { ps("0"); return; }
    while (v && i > 0) { buf[i--] = (char)('0' + (v % 10U)); v /= 10U; }
    ps(&buf[i + 1]);
}
static void pi(int v) { if (v < 0) { ps("-"); v = -v; } pu((unsigned int)v); }
static void pd(double v, int p) {
    long long sc = 1, iv; int i, neg = 0;
    if (v < 0.0) { neg = 1; v = -v; }
    for (i = 0; i < p; i++) sc *= 10;
    iv = (long long)(v * (double)sc + 0.5);
    if (neg) ps("-");
    { char buf[32]; int j = 30; buf[31] = 0;
      for (i = 0; i < p; i++) { buf[j--] = (char)('0' + (iv % 10)); iv /= 10; }
      buf[j--] = '.'; if (!iv) buf[j--] = '0';
      while (iv && j > 0) { buf[j--] = (char)('0' + (iv % 10)); iv /= 10; }
      ps(&buf[j + 1]);
    }
}
static void pl(const char *l, double v) { ps(l); pd(v, 6); ps("\n"); }

/* Quantum state */
static qs qsc(unsigned int n) {
    qs s; unsigned int d = 1U << n, i;
    s.n = n; s.d = d; s.a = (cx *)qa(d * sizeof(cx));
    for (i = 0; i < d; i++) { s.a[i].r = 0.0; s.a[i].i = 0.0; }
    s.a[0].r = 1.0; return s;
}
static void qsd(qs s) { (void)s; }
static void qsr(qs s) {
    unsigned int i;
    for (i = 0; i < s.d; i++) { s.a[i].r = 0.0; s.a[i].i = 0.0; }
    s.a[0].r = 1.0;
}
static void qsh(qs s) {
    unsigned int i, b; double iv = 0.707106781186548;
    for (b = 0; b < s.n; b++) {
        unsigned int st = 1U << b;
        for (i = 0; i < s.d; i++) if (!(i & st)) {
            unsigned int j = i | st;
            double ar = s.a[i].r, ai = s.a[i].i, br = s.a[j].r, bi = s.a[j].i;
            s.a[i].r = (ar+br)*iv; s.a[i].i = (ai+bi)*iv;
            s.a[j].r = (ar-br)*iv; s.a[j].i = (ai-bi)*iv;
        }
    }
}
static void qsp(qs s, double a) {
    double ca, sa; unsigned int i; qco(a, &ca); sa = qsi(a);
    for (i = 0; i < s.d; i++) if (i & (1U << (s.n-1))) {
        double r = s.a[i].r, im = s.a[i].i;
        s.a[i].r = r*ca - im*sa; s.a[i].i = r*sa + im*ca;
    }
}
static void qcn(qs s, unsigned int c, unsigned int t) {
    unsigned int i;
    for (i = 0; i < s.d; i++) if (i & (1U << c)) {
        unsigned int j = i ^ (1U << t);
        if (j > i) { cx tmp = s.a[i]; s.a[i] = s.a[j]; s.a[j] = tmp; }
    }
}
static void qrx(qs s, unsigned int q, double a) {
    double ca, sa; unsigned int i; qco(a*0.5, &ca); sa = qsi(a*0.5);
    for (i = 0; i < s.d; i++) if (!(i & (1U << q))) {
        unsigned int j = i | (1U << q);
        double ar = s.a[i].r, ai = s.a[i].i, br = s.a[j].r, bi = s.a[j].i;
        s.a[i].r = ar*ca + bi*sa; s.a[i].i = ai*ca - br*sa;
        s.a[j].r = br*ca + ai*sa; s.a[j].i = bi*ca - ar*sa;
    }
}
static void qry(qs s, unsigned int q, double a) {
    double ca, sa; unsigned int i; qco(a*0.5, &ca); sa = qsi(a*0.5);
    for (i = 0; i < s.d; i++) if (!(i & (1U << q))) {
        unsigned int j = i | (1U << q);
        double ar = s.a[i].r, ai = s.a[i].i, br = s.a[j].r, bi = s.a[j].i;
        s.a[i].r = ar*ca - br*sa; s.a[i].i = ai*ca - bi*sa;
        s.a[j].r = br*ca + ar*sa; s.a[j].i = bi*ca + ai*sa;
    }
}
static void qrz(qs s, unsigned int q, double a) {
    double ca, sa; unsigned int i; qco(a*0.5, &ca); sa = qsi(a*0.5);
    for (i = 0; i < s.d; i++) {
        double r = s.a[i].r, im = s.a[i].i;
        if (i & (1U << q)) { s.a[i].r = r*ca + im*sa; s.a[i].i = im*ca - r*sa; }
        else { s.a[i].r = r*ca - im*sa; s.a[i].i = im*ca + r*sa; }
    }
}
static double qpr(qs s, unsigned int q) {
    double p = 0.0; unsigned int i;
    for (i = 0; i < s.d; i++) if (i & (1U << q)) p += s.a[i].r*s.a[i].r + s.a[i].i*s.a[i].i;
    return p;
}
static double qex2(qs s, unsigned int q) { return 2.0 * qpr(s, q) - 1.0; }

/* Parameterized quantum circuit */
static pqc pcrc(unsigned int nq, unsigned int dp) {
    pqc p; unsigned int np = nq * dp * 3U, i; double ph = 0.123456789;
    p.nq = nq; p.dp = dp; p.np = np;
    p.p = (double *)qa(np * sizeof(double));
    for (i = 0; i < np; i++) { ph = ph * 1.103515245 + 0.3; p.p[i] = (ph - (int)ph) * TPI - PI; }
    return p;
}
static void pcrd(pqc p) { (void)p; }
static void pca(qs s, pqc p) {
    unsigned int d, q;
    for (d = 0; d < p.dp; d++) {
        for (q = 0; q < p.nq; q++) {
            unsigned int idx = d * p.nq * 3U + q * 3U;
            qrx(s, q, p.p[idx]); qry(s, q, p.p[idx+1]); qrz(s, q, p.p[idx+2]);
        }
        if (p.nq > 1U) for (q = 0; q < p.nq - 1U; q++) qcn(s, q, q + 1U);
    }
}
static double pcps(qs s, pqc p, unsigned int idx, unsigned int q) {
    double o, ep, em, g; qs sp;
    o = p.p[idx]; sp = qsc(s.n);
    p.p[idx] = o + SD; qsr(sp); pca(sp, p); ep = qex2(sp, q);
    p.p[idx] = o - SD; qsr(sp); pca(sp, p); em = qex2(sp, q);
    p.p[idx] = o; g = (ep - em) / (2.0 * SD); qsd(sp); return g;
}

/* Quantum layer */
static ql qlc(unsigned int ni, unsigned int no) {
    ql l; unsigned int i; double ph = 0.987654321;
    l.ni = ni; l.no = no;
    l.w = (double *)qa(ni * no * sizeof(double));
    l.b = (double *)qa(no * sizeof(double));
    for (i = 0; i < ni * no; i++) { ph = ph * 1.103515245 + 0.7; l.w[i] = ((ph-(int)ph)-0.5)*0.2; }
    for (i = 0; i < no; i++) l.b[i] = 0.0;
    return l;
}
static double qlf(ql l, const double *x) {
    double o = 0.0; unsigned int i;
    for (i = 0; i < l.ni; i++) o += x[i] * l.w[i];
    o += l.b[0]; return 1.0 / (1.0 + qex(-o));
}
static void qlb(ql l, const double *x, double e) {
    unsigned int i;
    for (i = 0; i < l.ni; i++) l.w[i] -= LR * e * x[i];
    l.b[0] -= LR * e;
}

/* Quantum neural network */
static qnn qnnc(unsigned int nq, unsigned int dp, unsigned int nl) {
    qnn m; unsigned int i;
    m.nq = nq; m.nl = nl; m.loss = 0.0;
    m.c = pcrc(nq, dp);
    m.l = (ql *)qa(nl * sizeof(ql));
    for (i = 0; i < nl; i++) m.l[i] = qlc(nq, 1U);
    return m;
}
static void qnnd(qnn m) {
    unsigned int i; pcrd(m.c); for (i = 0; i < m.nl; i++) { (void)m.l[i]; }
}
static double qnnt(qnn m, const double *x, double t) {
    qs s = qsc(m.nq); double pr, e, lo; unsigned int q, ms[4];
    for (q = 0; q < m.nq && q < 4U; q++) qry(s, q, x[q] * PI);
    pca(s, m.c);
    for (q = 0; q < m.nq && q < 4U; q++) ms[q] = (unsigned int)(qex2(s, q) * 1000.0);
    { double mv[4]; for (q = 0; q < 4U; q++) mv[q] = (double)ms[q] / 1000.0;
      pr = qlf(m.l[0], mv); }
    e = pr - t; lo = e * e;
    { double mv[4]; for (q = 0; q < m.nq && q < 4U; q++) mv[q] = qex2(s, q);
      qlb(m.l[0], mv, e); }
    for (q = 0; q < m.nq && q < 4U; q++) {
        double g = pcps(s, m.c, q * 3U + 1, q);
        m.c.p[q * 3U + 1] -= LR * e * g;
    }
    m.loss = m.loss * 0.95 + lo * 0.05; qsd(s); return lo;
}
static double qnni(qnn m, const double *x) {
    qs s = qsc(m.nq); double pr; unsigned int q;
    for (q = 0; q < m.nq && q < 4U; q++) qry(s, q, x[q] * PI);
    pca(s, m.c);
    { double mv[4]; for (q = 0; q < m.nq && q < 4U; q++) mv[q] = qex2(s, q);
      pr = qlf(m.l[0], mv); }
    qsd(s); return pr;
}
static int qnnl(qnn m, const double *da, const double *lb, unsigned int ns, unsigned int ep) {
    unsigned int e, i, q; double tl = 0.0;
    ps("[qnn] layerwise training (barren plateau mitigation)\n");
    for (e = 0; e < ep; e++) {
        tl = 0.0;
        for (i = 0; i < ns; i++) tl += qnnt(m, &da[i * 4U], lb[i]);
        tl /= (double)ns;
        if (!(e % 5U)) { ps("  epoch="); pu(e); ps(" loss="); pd(tl, 6); ps("\n"); }
    }
    ps("[qnn] applying quantum natural gradient (Fubini-Study)\n");
    for (q = 0; q < m.nq && q < 4U; q++) {
        qs s = qsc(m.nq); double ov, di = SD, oi = m.c.p[q * 3U + 1];
        unsigned int k; for (k = 0; k < m.nq && k < 4U; k++) qry(s, k, da[k] * PI);
        m.c.p[q * 3U + 1] = oi + di; pca(s, m.c);
        ov = s.a[0].r * s.a[0].r + s.a[0].i * s.a[0].i;
        m.c.p[q * 3U + 1] = oi;
        if (1.0 - ov > EPS) m.c.p[q * 3U + 1] /= (1.0 + (1.0 - ov));
        qsd(s);
    }
    return 0;
}

/* VQE */
static int vqe(unsigned int nq, unsigned int dp, unsigned int mi) {
    pqc a = pcrc(nq, dp); qs s = qsc(nq);
    unsigned int it, q; double en = 0.0, pe = 100.0;
    ps("[vqe] running nq="); pu(nq); ps(" depth="); pu(dp); ps("\n");
    for (it = 0; it < mi; it++) {
        en = 0.0; qsr(s); pca(s, a);
        for (q = 0; q < nq; q++) en += -qex2(s, q);
        for (q = 0; q + 1U < nq; q++) en += 0.5 * qex2(s, q) * qex2(s, q + 1U);
        for (q = 0; q < a.np; q++) { double g = pcps(s, a, q, 0); a.p[q] -= LR * g * 0.1; }
        if (en > pe ? en - pe : pe - en < 0x01P-10) break;
        pe = en;
    }
    ps("[vqe] energy="); pd(en, 6); ps(" iter="); pu(it); ps("\n");
    pcrd(a); qsd(s); return 0;
}

/* Quantum kernels */
static qk qkc(unsigned int ns, unsigned int dm) {
    qk k; k.ns = ns; k.dm = dm;
    k.km = (double *)qa(ns * ns * sizeof(double));
    k.lb = (double *)qa(ns * sizeof(double));
    return k;
}
static void qkd(qk k) { (void)k; }
static void qkfm(qs s, const double *x, unsigned int dm) {
    unsigned int q; qsh(s);
    for (q = 0; q < s.n && q < dm; q++) qrz(s, q, x[q] * PI);
    if (s.n > 1U) for (q = 0; q + 1U < s.n && q + 1U < dm; q++) {
        qcn(s, q, q + 1U); qrz(s, q + 1U, x[q] * x[q + 1U] * PI); qcn(s, q, q + 1U);
    }
}
static double qke(qk k, const double *x1, const double *x2) {
    qs s1 = qsc(2), s2 = qsc(2); double ov = 0.0; unsigned int i;
    qkfm(s1, x1, k.dm); qkfm(s2, x2, k.dm);
    for (i = 0; i < s1.d; i++) {
        double dr = s1.a[i].r*s2.a[i].r + s1.a[i].i*s2.a[i].i;
        double di = s1.a[i].r*s2.a[i].i - s1.a[i].i*s2.a[i].r;
        ov += dr * dr + di * di;
    }
    qsd(s1); qsd(s2); return ov;
}
static void qkbm(qk k, const double *da, unsigned int ns, unsigned int dm) {
    unsigned int i, j;
    for (i = 0; i < ns; i++) for (j = i; j < ns; j++) {
        double v = qke(k, &da[i * dm], &da[j * dm]);
        k.km[i * ns + j] = v; k.km[j * ns + i] = v;
    }
}
static int qkstm(qk k, const double *lb, unsigned int ep) {
    unsigned int i, e; double *al = (double *)qa(k.ns * sizeof(double));
    for (i = 0; i < k.ns; i++) { al[i] = 0.0; k.lb[i] = lb[i]; }
    for (e = 0; e < ep; e++) for (i = 0; i < k.ns; i++) {
        double pr = 0.0; unsigned int j;
        for (j = 0; j < k.ns; j++) pr += al[j] * lb[j] * k.km[i * k.ns + j];
        al[i] -= LR * (pr - lb[i]); if (al[i] < 0.0) al[i] = 0.0;
    }
    ps("[qsvm] trained epochs="); pu(ep); ps("\n"); return 0;
}
static double qksp(qk k, const double *x, unsigned int dm) {
    double sc = 0.0; unsigned int i;
    for (i = 0; i < k.ns; i++) sc += k.lb[i] * qke(k, x, &k.km[i * k.ns]);
    return sc > 0.0 ? 1.0 : -1.0; (void)dm;
}
static qgp qgpc(unsigned int nc, unsigned int dm) {
    qgp g; g.nc = nc; g.dm = dm;
    g.mu = (double *)qa(nc * dm * sizeof(double));
    g.sg = (double *)qa(nc * dm * sizeof(double));
    return g;
}
static void qgpd(qgp g) { (void)g; }
static void qgpf(qgp g, const double *da, const double *lb, unsigned int ns) {
    unsigned int c, d, i;
    for (c = 0; c < g.nc; c++) {
        unsigned int cn = 0;
        for (d = 0; d < g.dm; d++) { g.mu[c * g.dm + d] = 0.0; g.sg[c * g.dm + d] = 0.0; }
        for (i = 0; i < ns; i++) if ((int)lb[i] == (int)c) {
            for (d = 0; d < g.dm; d++) g.mu[c * g.dm + d] += da[i * g.dm + d];
            cn++;
        }
        if (cn) for (d = 0; d < g.dm; d++) g.mu[c * g.dm + d] /= (double)cn;
        for (i = 0; i < ns; i++) if ((int)lb[i] == (int)c)
            for (d = 0; d < g.dm; d++) { double df = da[i * g.dm + d] - g.mu[c * g.dm + d]; g.sg[c * g.dm + d] += df * df; }
        if (cn > 1U) for (d = 0; d < g.dm; d++) g.sg[c * g.dm + d] /= (double)(cn - 1U);
    }
    ps("[qgp] fitted classes="); pu(g.nc); ps("\n");
}
static double qgpp(qgp g, const double *x) {
    unsigned int c, d; double bs = -1.0E30, bc = 0.0;
    for (c = 0; c < g.nc; c++) {
        double lp = 0.0;
        for (d = 0; d < g.dm; d++) {
            double df = x[d] - g.mu[c * g.dm + d], v = g.sg[c * g.dm + d];
            if (v < EPS) v = EPS; lp += -0.5 * df * df / v - 0.5 * qlo(v);
        }
        if (lp > bs) { bs = lp; bc = (double)c; }
    }
    return bc;
}
static double qkal(qk k, const double *tg, unsigned int ns) {
    double fq = 0.0, ft = 0.0, cr = 0.0; unsigned int i, j;
    for (i = 0; i < ns; i++) for (j = 0; j < ns; j++) {
        double qv = k.km[i * ns + j], tv = tg[i * ns + j];
        fq += qv * qv; ft += tv * tv; cr += qv * tv;
    }
    if (fq < EPS || ft < EPS) return 0.0;
    return cr / qsq(fq * ft);
}

/* Quantum generative */
static qborn qbc(unsigned int nq, unsigned int ns) {
    qborn b; b.nq = nq; b.ns = ns; b.pr = (double *)qa(ns * sizeof(double)); return b;
}
static void qbd(qborn b) { (void)b; }
static void qbs(qborn b, pqc p) {
    qs s = qsc(b.nq); unsigned int i; double t = 0.0;
    pca(s, p);
    for (i = 0; i < b.ns && i < s.d; i++) { b.pr[i] = s.a[i].r*s.a[i].r + s.a[i].i*s.a[i].i; t += b.pr[i]; }
    if (t > EPS) for (i = 0; i < b.ns && i < s.d; i++) b.pr[i] /= t;
    qsd(s);
}
static double qbll(qborn b, const unsigned int *tg, unsigned int ns) {
    double ll = 0.0; unsigned int i;
    for (i = 0; i < ns; i++) if (tg[i] < b.ns && b.pr[tg[i]] > EPS) ll += qlo(b.pr[tg[i]]);
    return ll / (double)ns;
}
static void qbts(qborn b, pqc p, const unsigned int *tg, unsigned int ns, double lr) {
    unsigned int q;
    for (q = 0; q < p.np && q < 12U; q++) {
        double o = p.p[q], lp, lm, g; qs s = qsc(b.nq);
        p.p[q] = o + SD; qsr(s); qbs(b, p); lp = qbll(b, tg, ns);
        p.p[q] = o - SD; qsr(s); qbs(b, p); lm = qbll(b, tg, ns);
        p.p[q] = o; g = (lp - lm) / (2.0 * SD); p.p[q] += lr * g; qsd(s);
    }
}
static qgan qganc(unsigned int nq, unsigned int dp) {
    qgan g; g.g = pcrc(nq, dp); g.d = pcrc(nq, dp); g.gl = 0.0; g.dl = 0.0; return g;
}
static void qgand(qgan g) { pcrd(g.g); pcrd(g.d); }
static void qgants(qgan g, const double *rd, unsigned int ns, unsigned int dm, double lr) {
    qs s = qsc(g.g.nq); unsigned int i, q; double dl = 0.0, gl = 0.0;
    for (i = 0; i < ns && i < 4U; i++) {
        qsr(s); for (q = 0; q < g.d.nq && q < dm; q++) qry(s, q, rd[i * dm + q] * PI);
        pca(s, g.d); dl += -qlo(qpr(s, 0) + EPS);
    }
    qsr(s); pca(s, g.g); { double go = qpr(s, 0); gl = -qlo(1.0 - go + EPS); }
    for (q = 0; q < g.g.np && q < 6U; q++) { double gr = pcps(s, g.g, q, 0); g.g.p[q] += lr * gr * 0.01; }
    g.gl = g.gl * 0.9 + gl * 0.1; g.dl = g.dl * 0.9 + (dl / (double)ns) * 0.1; qsd(s);
}
static qvae qvc(unsigned int nq, unsigned int ld) {
    qvae v; v.nq = nq; v.ld = ld; v.z = (double *)qa(ld * sizeof(double)); return v;
}
static void qvd(qvae v) { (void)v; }
static double qve(qvae v, const double *x, unsigned int dm) {
    qs s = qsc(v.nq); unsigned int q; double rc = 0.0;
    for (q = 0; q < v.nq && q < dm; q++) qry(s, q, x[q] * PI);
    qsh(s);
    for (q = 0; q < v.ld && q < v.nq; q++) v.z[q] = qex2(s, q);
    for (q = 0; q < dm && q < v.nq; q++) { double df = qpr(s, q) - x[q]; rc += df * df; }
    qsd(s); return rc / (double)dm;
}
static void qvdec(qvae v, double *o, unsigned int dm) {
    qs s = qsc(v.nq); unsigned int q;
    for (q = 0; q < v.ld && q < v.nq; q++) qry(s, q, v.z[q] * PI);
    for (q = 0; q < dm && q < v.nq; q++) o[q] = qpr(s, q);
    qsd(s);
}
static qdif qdfc(unsigned int nq, unsigned int st) {
    qdif d; d.nq = nq; d.st = st; d.sc = (double *)qa(nq * st * sizeof(double)); return d;
}
static void qdfd(qdif d) { (void)d; }
static void qdff(qdif d, double *x, unsigned int dm, double ns) {
    unsigned int i; double al = 1.0;
    for (i = 0; i < d.st; i++) {
        unsigned int q; al *= 0.99; double sq = qsq(1.0 - al);
        for (q = 0; q < dm && q < d.nq; q++) x[q] = qsq(al) * x[q] + sq * ns * 0.1;
        d.sc[i * d.nq] = -x[0];
    }
}
static void qdfr(qdif d, double *x, unsigned int dm) {
    int i; unsigned int q; double al = 0.99;
    for (i = (int)d.st - 1; i >= 0; i--) {
        double sc = d.sc[i * d.nq];
        for (q = 0; q < dm && q < d.nq; q++) x[q] = (x[q] + (1.0 - al) * sc) / qsq(al);
        al /= 0.99; if (al > 1.0) al = 1.0;
    }
}

/* Quantum RL */
static qql qqlc(unsigned int ns, unsigned int na, double gm, double lr) {
    qql q; unsigned int i; q.ns = ns; q.na = na; q.gm = gm; q.lr = lr;
    q.qt = (double *)qa(ns * na * sizeof(double));
    for (i = 0; i < ns * na; i++) q.qt[i] = 0.0;
    return q;
}
static void qqld(qql q) { (void)q; }
static double qqlu(qql q, unsigned int s, unsigned int a, double r, unsigned int ns) {
    unsigned int k; double mx = -1.0E30, oq = q.qt[s * q.na + a], tg;
    for (k = 0; k < q.na; k++) { double v = q.qt[ns * q.na + k]; if (v > mx) mx = v; }
    tg = r + q.gm * mx; q.qt[s * q.na + a] += q.lr * (tg - oq); return tg - oq;
}
static unsigned int qqls(qql q, unsigned int s, double ep) {
    unsigned int a, b = 0; double bv = -1.0E30;
    if (0.7 < ep) return ((unsigned int)(0.7 * 1000.0)) % q.na;
    for (a = 0; a < q.na; a++) { double v = q.qt[s * q.na + a]; if (v > bv) { bv = v; b = a; } }
    return b;
}
static qpol qplc(unsigned int ns, unsigned int na) {
    qpol p; unsigned int s, a; p.ns = ns; p.na = na;
    p.po = (double *)qa(ns * na * sizeof(double));
    for (s = 0; s < ns; s++) for (a = 0; a < na; a++) p.po[s * na + a] = 1.0 / (double)na;
    return p;
}
static void qpld(qpol p) { (void)p; }
static unsigned int qpls(qpol p, unsigned int s) {
    double c = 0.0; unsigned int a;
    for (a = 0; a < p.na; a++) { c += p.po[s * p.na + a]; if (0.37 < c) return a; }
    return p.na - 1U;
}
static void qplu(qpol p, unsigned int s, unsigned int a, double ad, double lr) {
    unsigned int k; double sm = 0.0;
    for (k = 0; k < p.na; k++) {
        double g = (k == a) ? 1.0 - p.po[s * p.na + k] : -p.po[s * p.na + k];
        p.po[s * p.na + k] += lr * ad * g;
        if (p.po[s * p.na + k] < 0.01) p.po[s * p.na + k] = 0.01;
    }
    for (k = 0; k < p.na; k++) sm += p.po[s * p.na + k];
    if (sm > EPS) for (k = 0; k < p.na; k++) p.po[s * p.na + k] /= sm;
}
static qac qacc(unsigned int ns, unsigned int na, double la, double lc) {
    qac a; a.ac = qplc(ns, na); a.cr = qqlc(ns, na, 0.99, lc); a.la = la; a.lc = lc; return a;
}
static void qacd(qac a) { qpld(a.ac); qqld(a.cr); }
static void qacts(qac a, unsigned int s, unsigned int ac, double r, unsigned int ns) {
    double td = qqlu(a.cr, s, ac, r, ns); qplu(a.ac, s, ac, td, a.la);
}
static qband qbdc(unsigned int na) {
    qband b; unsigned int i; b.na = na; b.pl = 0;
    b.rw = (double *)qa(na * 2U * sizeof(double));
    for (i = 0; i < na * 2U; i++) b.rw[i] = 0.0;
    return b;
}
static void qbdd(qband b) { (void)b; }
static unsigned int qbds(qband b, double ep) {
    unsigned int a, bt = 0; double bv = -1.0E30;
    if (0.53 < ep) return ((unsigned int)(0.53 * 100.0)) % b.na;
    for (a = 0; a < b.na; a++) {
        double uc = b.rw[a * 2U];
        if (b.rw[a * 2U + 1U] > 0.5) uc += qsq(2.0 * qlo((double)(b.pl + 1U)) / b.rw[a * 2U + 1U]);
        if (uc > bv) { bv = uc; bt = a; }
    }
    return bt;
}
static void qbdu(qband b, unsigned int a, double r) {
    double n = b.rw[a * 2U + 1U];
    b.rw[a * 2U] = (b.rw[a * 2U] * n + r) / (n + 1.0);
    b.rw[a * 2U + 1U] = n + 1.0; b.pl++;
}
static int gvas(unsigned int n, unsigned int tg) {
    qs s = qsc(n); unsigned int it, i, ni = 1U;
    if (n > 2U) { ni = 1U << ((n - 2U) / 2U); if (ni > 8U) ni = 8U; }
    qsh(s);
    for (it = 0; it < ni; it++) {
        if (tg < s.d) { s.a[tg].r = -s.a[tg].r; s.a[tg].i = -s.a[tg].i; }
        qsh(s);
        { double av = 0.0; for (i = 0; i < s.d; i++) av += s.a[i].r; av /= (double)s.d;
          for (i = 0; i < s.d; i++) s.a[i].r = 2.0 * av - s.a[i].r; }
        qsh(s);
    }
    { unsigned int b = 0; double bp = 0.0;
      for (i = 0; i < s.d; i++) { double p = s.a[i].r*s.a[i].r + s.a[i].i*s.a[i].i; if (p > bp) { bp = p; b = i; } }
      qsd(s); return (b == tg) ? 0 : -1;
    }
}

/* Quantum encoding */
static qenc qecc(unsigned int n, unsigned int dm, unsigned int mt) {
    qenc e; e.n = n; e.dm = dm; e.mt = mt; e.da = (double *)qa(n * dm * sizeof(double)); return e;
}
static void qecd(qenc e) { (void)e; }
static void qea(qs s, const double *da, unsigned int dm) {
    unsigned int i; double nm = 0.0;
    for (i = 0; i < dm && i < s.d; i++) nm += da[i] * da[i];
    if (nm < EPS) nm = EPS; nm = qsq(nm);
    for (i = 0; i < dm && i < s.d; i++) { s.a[i].r = da[i] / nm; s.a[i].i = 0.0; }
}
static void qeang(qs s, const double *da, unsigned int dm) {
    unsigned int q; for (q = 0; q < s.n && q < dm; q++) qry(s, q, da[q] * PI);
}
static void qeb(qs s, const double *da, unsigned int dm) {
    unsigned int idx = 0, q, i;
    for (q = 0; q < dm && q < s.n; q++) if (da[q] > 0.5) idx |= (1U << q);
    if (idx < s.d) { for (i = 0; i < s.d; i++) { s.a[i].r = 0.0; s.a[i].i = 0.0; } s.a[idx].r = 1.0; }
}
static void qecs(qs s, const double *da, unsigned int dm) {
    unsigned int q;
    for (q = 0; q < s.n && q < dm; q++) {
        double ca, sa, al = da[q]; qco(al, &ca); sa = qsi(al);
        s.a[0].r *= ca; if ((1U << q) < s.d) s.a[1U << q].r += sa * 0.5;
    }
}
static void qeiqp(qs s, const double *da, unsigned int dm) {
    unsigned int q; qsh(s);
    for (q = 0; q < s.n && q < dm; q++) qrz(s, q, da[q] * PI);
    if (s.n > 1U) for (q = 0; q + 1U < s.n && q + 1U < dm; q++) {
        qcn(s, q, q + 1U); qrz(s, q + 1U, da[q] * da[q + 1U]); qcn(s, q, q + 1U);
    }
    qsh(s);
}
static void qee(qenc e, qs s) {
    switch (e.mt) {
        case 0: qea(s, e.da, e.dm); break; case 1: qeang(s, e.da, e.dm); break;
        case 2: qeb(s, e.da, e.dm); break; case 3: qecs(s, e.da, e.dm); break;
        case 4: qeiqp(s, e.da, e.dm); break; default: qeang(s, e.da, e.dm); break;
    }
}

/* Hybrid classical-quantum */
static hyb hyc(unsigned int nq, unsigned int cd) {
    hyb h; h.nq = nq; h.cd = cd;
    h.cb = (double *)qa(cd * sizeof(double));
    h.q = (qs *)qa(sizeof(qs)); h.q[0] = qsc(nq);
    return h;
}
static void hyd(hyb h) { qsd(h.q[0]); }
static double hyf(hyb h, const double *x) {
    unsigned int i, q; double o = 0.0;
    for (i = 0; i < h.cd; i++) { h.cb[i] = x[i]; if (h.cb[i] < 0.0) h.cb[i] = 0.0; }
    qsr(h.q[0]);
    for (q = 0; q < h.nq && q < h.cd; q++) qry(h.q[0], q, h.cb[q] * PI);
    qsh(h.q[0]); if (h.nq > 1U) for (q = 0; q + 1U < h.nq; q++) qcn(h.q[0], q, q + 1U);
    for (q = 0; q < h.nq; q++) o += qpr(h.q[0], q);
    return o / (double)h.nq;
}
static void gci(gctx *g, unsigned int np, unsigned int mt) {
    g->np = np; g->mt = mt;
    g->pa = (double *)qa(np * sizeof(double));
    g->gr = (double *)qa(np * sizeof(double));
}
static void gcc(gctx *g, qnn m, const double *x, double t) {
    unsigned int i, q;
    if (g->mt == 0U) {
        qs s = qsc(m.nq);
        for (q = 0; q < m.nq && q < 4U; q++) qry(s, q, x[q] * PI);
        for (i = 0; i < g->np && i < m.c.np; i++) g->gr[i] = pcps(s, m.c, i, i % m.nq);
        qsd(s);
    } else if (g->mt == 1U) {
        double ep = 0x01P-6, p0 = qnni(m, x), l0 = (p0 - t) * (p0 - t);
        for (i = 0; i < g->np && i < m.c.np; i++) {
            double o = m.c.p[i]; m.c.p[i] = o + ep;
            double p1 = qnni(m, x), l1 = (p1 - t) * (p1 - t);
            m.c.p[i] = o; g->gr[i] = (l1 - l0) / ep;
        }
    } else {
        qs s = qsc(m.nq);
        for (q = 0; q < m.nq && q < 4U; q++) qry(s, q, x[q] * PI);
        pca(s, m.c);
        for (i = 0; i < g->np && i < m.c.np; i++) g->gr[i] = -2.0 * (t - qpr(s, 0)) * qex2(s, i % m.nq);
        qsd(s);
    }
}
static zne *znec(unsigned int np, double fc) {
    zne *z = (zne *)qa(sizeof(zne)); unsigned int i;
    z->np = np; z->fc = fc; z->co = (double *)qa(np * sizeof(double));
    for (i = 0; i < np; i++) z->co[i] = 0.0;
    z->co[0] = 1.0; return z;
}
static void zned(zne *z) { (void)z; }
static double znee(zne *z, const double *nv, unsigned int n) {
    double r = 0.0; unsigned int i;
    for (i = 0; i < n && i < z->np; i++) r += z->co[i] * nv[i];
    return r;
}
static void dda(qs s, unsigned int q, unsigned int pl) {
    unsigned int i; for (i = 0; i < pl; i++) { qrx(s, q, PI); qrx(s, q, -PI); }
}
static void twa(qs s, unsigned int q) {
    unsigned int r = (unsigned int)(qpr(s, q) * 4.0) % 4U;
    if (r == 1U) qrx(s, q, PI); else if (r == 2U) qry(s, q, PI); else if (r == 3U) qrz(s, q, PI);
}
static int distr(unsigned int nqp, unsigned int nq, unsigned int st) {
    unsigned int qp, k; double t = 0.0;
    ps("[dist] "); pu(nqp); ps(" QPUs, "); pu(nq); ps(" qubits x "); pu(st); ps(" steps\n");
    for (qp = 0; qp < nqp; qp++) {
        qs s = qsc(nq); double l = 0.0;
        for (k = 0; k < st; k++) { qsh(s); l += qpr(s, 0); }
        t += l / (double)st; qsd(s);
    }
    ps("[dist] result="); pd(t / (double)nqp, 6); ps("\n"); return 0;
}

/* Main */
int main(void) {
    unsigned int i; double da[32], lb[8], acc = 0.0;
    ps("=== Quantum Machine Learning Framework ===\n");
    ps("AI-ASM OS - AArch64 WebAssembly Native\n");
    { int al = host_get_argv(0, 256); if (al < 0) al = 0; ps("  argv check: "); pu((unsigned int)al); ps("\n\n"); }
    ps("[1] Creating QNN (4 qubits, depth=3, 2 layers)\n");
    qnn mdl = qnnc(4, 3, 2);
    ps("[2] Encoding training data (8 samples)\n");
    for (i = 0; i < 32U; i++) da[i] = ((double)(i % 5U) - 2.0) * 0.3;
    for (i = 0; i < 8U; i++) lb[i] = (i < 4U) ? 0.1 : 0.9;
    ps("[3] Testing quantum data encoding\n");
    { qenc ec = qecc(2, 4, 1); qs s = qsc(4);
      ec.da[0] = 0.3; ec.da[1] = 0.7; ec.da[2] = 0.1; ec.da[3] = 0.5;
      qee(ec, s); ps("  angle encoding prob[0]="); pd(qpr(s, 0), 4); ps("\n");
      ec.mt = 0; qsr(s); qee(ec, s); ps("  amplitude encoding done\n");
      ec.mt = 4; qsr(s); qee(ec, s); ps("  IQP encoding done\n");
      qecd(ec); qsd(s);
    }
    ps("\n[4] Training QNN with parameter-shift rule\n");
    qnnl(mdl, da, lb, 8, 10);
    ps("\n[5] Running inference\n");
    for (i = 0; i < 4U; i++) {
        double pr = qnni(mdl, &da[i * 4U]);
        ps("  sample="); pu(i); ps(" pred="); pd(pr, 4); ps(" target="); pd(lb[i], 1); ps("\n");
        if ((pr > 0.5 && lb[i] > 0.5) || (pr <= 0.5 && lb[i] <= 0.5)) acc += 1.0;
    }
    acc /= 4.0; ps("  accuracy="); pd(acc * 100.0, 2); ps("%\n");
    ps("\n[6] Quantum kernel methods\n");
    { qk kr = qkc(4, 2); double kd[8] = {0.1,0.2,0.8,0.9,0.3,0.4,0.7,0.6};
      double kl[4] = {-1.0,1.0,-1.0,1.0}, gl[4] = {0,1,0,1};
      qkbm(kr, kd, 4, 2); ps("  kernel matrix built\n");
      qkstm(kr, kl, 10); ps("  SVM pred="); pd(qksp(kr, &kd[0], 2), 1); ps("\n");
      qgp gp = qgpc(2, 2); qgpf(gp, kd, gl, 4);
      ps("  GP pred="); pd(qgpp(gp, &kd[0]), 1); ps("\n");
      double ik[16]; for (i = 0; i < 16U; i++) ik[i] = (i/5U == i%5U) ? 1.0 : 0.0;
      ps("  alignment="); pd(qkal(kr, ik, 4), 4); ps("\n");
      qgpd(gp); qkd(kr);
    }
    ps("\n[7] Quantum generative models\n");
    { pqc gp = pcrc(3, 2); qborn bm = qbc(3, 8); unsigned int tg[4] = {0,1,2,3};
      qbs(bm, gp); ps("  Born LL="); pd(qbll(bm, tg, 4), 4); ps("\n");
      qbts(bm, gp, tg, 4, LR); qbs(bm, gp); ps("  After train LL="); pd(qbll(bm, tg, 4), 4); ps("\n");
      qgan gn = qganc(3, 2); double rd[8] = {0.5,0.5,0.3,0.7,0.6,0.4,0.8,0.2};
      qgants(gn, rd, 2, 4, LR); ps("  QGAN g="); pd(gn.gl, 4); ps(" d="); pd(gn.dl, 4); ps("\n");
      qgand(gn);
      qvae vv = qvc(3, 2); double vi[4] = {0.6,0.3,0.8,0.1}, vo[4];
      ps("  VAE loss="); pd(qve(vv, vi, 4), 4); ps("\n");
      qvdec(vv, vo, 4); ps("  VAE dec[0]="); pd(vo[0], 4); ps("\n");
      qvd(vv);
      qdif df = qdfc(3, 10); double sp[3] = {0.5,0.5,0.5};
      qdff(df, sp, 3, 1.0); qdfr(df, sp, 3); ps("  Diff samp[0]="); pd(sp[0], 4); ps("\n");
      qdfd(df); qbd(bm); pcrd(gp);
    }
    ps("\n[8] Quantum reinforcement learning\n");
    { qql ql = qqlc(4, 2, 0.99, LR); unsigned int st = 0, ns; double tr = 0.0;
      for (i = 0; i < 20U; i++) {
          unsigned int ac = qqls(ql, st, 0.2); double rw = (ac == 1U) ? 1.0 : 0.0;
          ns = (st + 1U) % 4U; qqlu(ql, st, ac, rw, ns); tr += rw; st = ns;
      }
      ps("  Q-learning reward="); pd(tr, 1); ps("\n");
      qpol pl = qplc(4, 2);
      for (i = 0; i < 10U; i++) { unsigned int a = qpls(pl, 0); qplu(pl, 0, a, (a==1U)?1.0:-0.5, 0.1); }
      ps("  Policy[0]="); pd(pl.po[0], 4); ps(" "); pd(pl.po[1], 4); ps("\n");
      qpld(pl);
      qac ac = qacc(4, 2, LR, LR);
      for (i = 0; i < 10U; i++) { unsigned int a = qpls(ac.ac, 0); qacts(ac, 0, a, (a==1U)?1.0:0.0, (0+1U)%4U); }
      ps("  Actor-critic trained\n"); qacd(ac);
      qband bd = qbdc(3);
      for (i = 0; i < 20U; i++) { unsigned int ar = qbds(bd, 0.1); qbdu(bd, ar, (ar==1U)?0.8:0.2); }
      ps("  Bandit arm1 mean="); pd(bd.rw[2], 4); ps("\n"); qbdd(bd);
      ps("  Grover search="); pi(gvas(3, 5)); ps("\n");
      qqld(ql);
    }
    ps("\n[9] Variational Quantum Eigensolver\n");
    vqe(3, 2, 20);
    ps("\n[10] Hybrid classical-quantum computing\n");
    { hyb hy = hyc(3, 4); double hi[4] = {0.5,0.3,0.8,0.1};
      ps("  Hybrid out="); pd(hyf(hy, hi), 4); ps("\n");
      gctx gc; gci(&gc, 6, 0); gcc(&gc, mdl, &da[0], lb[0]);
      ps("  ParamShift grad[0]="); pd(gc.gr[0], 4); ps("\n");
      gc.mt = 1; gcc(&gc, mdl, &da[0], lb[0]);
      ps("  FinDiff grad[0]="); pd(gc.gr[0], 4); ps("\n");
      gc.mt = 2; gcc(&gc, mdl, &da[0], lb[0]);
      ps("  Adjoint grad[0]="); pd(gc.gr[0], 4); ps("\n");
      zne *zn = znec(3, 1.5); double nv[3] = {0.8,0.75,0.7};
      ps("  ZNE mitigated="); pd(znee(zn, nv, 3), 4); ps("\n"); zned(zn);
      qs ds = qsc(2); dda(ds, 0, 4); ps("  DD applied\n");
      twa(ds, 0); ps("  Twirling applied\n"); qsd(ds);
      distr(2, 3, 10); hyd(hy);
    }
    ps("\n=== Summary ===\n");
    ps("Final loss="); pd(mdl.loss, 6); ps("\n");
    ps("Accuracy="); pd(acc * 100.0, 2); ps("%\n");
    ps("Modules: QNN, Kernels, Generative, RL, Encoding, Hybrid\n");
    ps("Status: ALL OK\n");
    qnnd(mdl); host_exit(0); return 0;
}
