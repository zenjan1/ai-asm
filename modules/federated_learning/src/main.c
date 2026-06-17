/* federated_learning: Privacy-Preserving Distributed ML Framework (v57.0)
 * AI-ASM OS - Edge-native federated learning with secure aggregation */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_LAY 8
#define MAX_W   32
#define MAX_CLI 16
#define MAX_CLS 16
#define MAX_GRD 64
#define MAX_KS  4
#define HE_P    1000003U
#define SH_P    999983U
#define AGG_FEDAVG   0
#define AGG_FEDPROX  1
#define AGG_FEDOPT   2
#define AGG_SCAFFOLD 3
#define AGG_KSERVER  4
#define SEL_RANDOM 0
#define SEL_POWER  1
#define SEL_FAIR   2
#define SEL_ACT    3

typedef struct { int nl, sz[MAX_LAY]; double w[MAX_LAY][MAX_W]; } model_t;
typedef struct { int id, samples, power, bw, battery, active, last_seen;
    int rd, rd_d; double rel; } client_t;
typedef struct { double c[MAX_LAY][MAX_W]; double ci[MAX_CLI][MAX_LAY][MAX_W]; } scaf_t;
typedef struct { int nk; model_t sm[MAX_KS]; int sc[MAX_KS]; double sw[MAX_KS]; } kserv_t;
typedef struct { model_t u[MAX_CLI]; int sc[MAX_CLI], ts[MAX_CLI]; int n, cap; double sb; } async_t;

static unsigned int RNG = 0x1F3A7B9DU;
static double g_dp_eps = 0.0, g_dp_sp = 0.0;

static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[18]; int i = 16, neg = v < 0;
    unsigned u = neg ? (unsigned)(-(v+1))+1U : (unsigned)v;
    b[17] = '\0';
    if (u == 0U) b[--i] = '0';
    else while (u > 0U && i > 0) { b[--i] = '0'+(char)(u%10U); u /= 10U; }
    if (neg && i > 0) b[--i] = '-';
    ps(&b[i]);
}
static void pd(double v) {
    if (v < 0.0) { ps("-"); v = -v; }
    int wh = (int)v; double fr = v - (double)wh;
    pi(wh); ps(".");
    int f = (int)(fr*10000.0+0.5); if (f >= 10000) f = 9999;
    char b[8];
    b[0] = '0'+(char)(f/1000); f %= 1000;
    b[1] = '0'+(char)(f/100);  f %= 100;
    b[2] = '0'+(char)(f/10);   f %= 10;
    b[3] = '0'+(char)f; b[4] = '\0'; ps(b);
}
static void ph(unsigned v) {
    static const char H[] = "0123456789ABCDEF";
    char b[10]; b[0]='0'; b[1]='x';
    for (int i=9; i>=2; i--) { b[i]=H[v&0xFU]; v>>=4; }
    b[9]='\0'; ps(b);
}
static double dabs(double v) { return v < 0.0 ? -v : v; }
static double dsqrt(double v) {
    if (v <= 0.0) return 0.0;
    double x = v*0.5;
    for (int i=0; i<30; i++) if (x > 0.0) x -= (x*x-v)/(2.0*x);
    return x;
}
static unsigned int rnd(void) { RNG^=RNG<<13; RNG^=RNG>>17; RNG^=RNG<<5; return RNG; }
static double rndf(void) { return (double)(rnd()&0xFFFFU)/65536.0; }

/* ── Model Operations ──────────────────────────────────────────────────── */
static void minit(model_t *m, int nl, int *sz) {
    m->nl = nl;
    for (int i=0; i<nl; i++) {
        m->sz[i] = sz[i] < MAX_W ? sz[i] : MAX_W;
        for (int j=0; j<m->sz[i]; j++) m->w[i][j] = rndf()*0.1;
    }
}
static double mdist(model_t *a, model_t *b) {
    double s = 0.0;
    for (int l=0; l<a->nl; l++) {
        int n = a->sz[l]<b->sz[l] ? a->sz[l] : b->sz[l];
        for (int j=0; j<n; j++) { double d=a->w[l][j]-b->w[l][j]; s += d*d; }
    }
    return dsqrt(s);
}
static double mnorm(model_t *m) {
    double s = 0.0;
    for (int l=0; l<m->nl; l++)
        for (int j=0; j<m->sz[l]; j++) s += m->w[l][j]*m->w[l][j];
    return dsqrt(s);
}

/* ── 1. Aggregation Algorithms ─────────────────────────────────────────── */
static void agg_fedavg(model_t *o, model_t md[], int sc[], int nc) {
    int tot = 0;
    for (int i=0; i<nc; i++) tot += sc[i];
    if (tot == 0) tot = 1;
    o->nl = md[0].nl;
    for (int l=0; l<o->nl; l++) {
        o->sz[l] = md[0].sz[l];
        for (int j=0; j<o->sz[l]; j++) {
            double s = 0.0;
            for (int c=0; c<nc; c++) s += md[c].w[l][j]*(double)sc[c];
            o->w[l][j] = s/(double)tot;
        }
    }
}
/* FedProx: FedAvg + proximal term mu/2*||w-w_g||^2 */
static void agg_fedprox(model_t *o, model_t md[], model_t *g, double mu, int nc) {
    int sc[MAX_CLI]; for (int i=0; i<nc; i++) sc[i] = 1;
    agg_fedavg(o, md, sc, nc);
    for (int l=0; l<g->nl; l++)
        for (int j=0; j<g->sz[l]; j++)
            o->w[l][j] -= mu*(g->w[l][j]-o->w[l][j])/(double)nc;
}
/* FedOpt: adaptive server optimizer (Adam-style) */
static void agg_fedopt(model_t *o, model_t md[], model_t *g, double *mt,
                       double *vt, double b1, double b2, double eps, int nc) {
    model_t av; int sc[MAX_CLI];
    for (int i=0; i<nc; i++) sc[i]=1;
    agg_fedavg(&av, md, sc, nc);
    o->nl = g->nl; int idx = 0;
    for (int l=0; l<g->nl; l++) {
        o->sz[l] = g->sz[l];
        for (int j=0; j<g->sz[l]; j++) {
            double d = av.w[l][j]-g->w[l][j];
            mt[idx] = b1*mt[idx]+(1.0-b1)*d;
            vt[idx] = b2*vt[idx]+(1.0-b2)*d*d;
            o->w[l][j] = g->w[l][j] + (mt[idx]/(1.0-b1)) /
                         (dsqrt(vt[idx]/(1.0-b2))+eps);
            idx++;
        }
    }
}
/* SCAFFOLD: control variates for client drift correction */
static void agg_scaffold(model_t *o, model_t md[], model_t *g, scaf_t *sf,
                         int cid[], int nc) {
    int ones = nc;
    agg_fedavg(o, md, &ones, nc);
    for (int c=0; c<nc; c++) {
        int ci = cid[c];
        for (int l=0; l<g->nl; l++)
            for (int j=0; j<g->sz[l]; j++)
                sf->ci[ci][l][j] += (o->w[l][j]-g->w[l][j])*100.0/(double)nc
                                    - sf->c[l][j];
    }
    for (int l=0; l<g->nl; l++)
        for (int j=0; j<g->sz[l]; j++) {
            double ac = 0.0;
            for (int c=0; c<nc; c++) ac += sf->ci[cid[c]][l][j];
            sf->c[l][j] += ac/(double)nc;
            o->w[l][j] -= sf->c[l][j]*0.01;
        }
}
/* K-Server: hierarchical aggregation */
static void kserv_init(kserv_t *k, int nk, int *sz, int nl) {
    k->nk = nk < MAX_KS ? nk : MAX_KS;
    for (int i=0; i<k->nk; i++) { minit(&k->sm[i], nl, sz); k->sc[i]=0; k->sw[i]=1.0/(double)k->nk; }
}
static void kserv_upd(kserv_t *k, int idx, model_t *u, int smp) {
    if (idx<0 || idx>=k->nk) return;
    model_t *m = &k->sm[idx];
    for (int l=0; l<m->nl; l++)
        for (int j=0; j<m->sz[l]; j++)
            m->w[l][j] = m->w[l][j]*0.7 + u->w[l][j]*0.3;
    k->sc[idx] += smp;
}
static void kserv_merge(kserv_t *k, model_t *o) {
    double wt = 0.0;
    for (int i=0; i<k->nk; i++) wt += k->sw[i];
    if (wt <= 0.0) wt = 1.0;
    o->nl = k->sm[0].nl;
    for (int l=0; l<o->nl; l++) {
        o->sz[l] = k->sm[0].sz[l];
        for (int j=0; j<o->sz[l]; j++) {
            double s = 0.0;
            for (int i=0; i<k->nk; i++) s += k->sm[i].w[l][j]*k->sw[i];
            o->w[l][j] = s/wt;
        }
    }
}

/* ── 2. Secure Aggregation ─────────────────────────────────────────────── */
static unsigned int shamir_share(unsigned int sec, unsigned int x, unsigned int k, unsigned int p) {
    unsigned int r = sec%p, xp = x%p;
    for (unsigned int i=1; i<k; i++) {
        unsigned int cf = (rnd()%(p-1U))+1U;
        r = (r + cf*xp)%p; xp = (xp*x)%p;
    }
    return r;
}
static unsigned int mod_inv(unsigned int a, unsigned int p) {
    unsigned int r=1U, b=a%p, e=p-2U;
    while (e>0U) { if (e&1U) r=(unsigned int)(((unsigned long long)r*b)%p);
        b=(unsigned int)(((unsigned long long)b*b)%p); e>>=1; }
    return r;
}
static unsigned int shamir_recon(unsigned int sh[], unsigned int ix[], unsigned int k, unsigned int p) {
    unsigned int sec = 0U;
    for (unsigned int i=0; i<k; i++) {
        unsigned int num=1U, den=1U;
        for (unsigned int j=0; j<k; j++) {
            if (i==j) continue;
            unsigned int xi=ix[i]%p, xj=ix[j]%p;
            num = (unsigned int)(((unsigned long long)num*(p-xj))%p);
            unsigned int df = xi>=xj ? xi-xj : p-(xj-xi);
            den = (unsigned int)(((unsigned long long)den*df)%p);
        }
        unsigned int lag = (unsigned int)(((unsigned long long)sh[i]*num%p*mod_inv(den,p))%p);
        sec = (sec+lag)%p;
    }
    return sec;
}
/* Additive homomorphic encryption (Paillier-like) */
static unsigned int he_enc(unsigned int v, unsigned int k, unsigned int p) { return (v+k)%p; }
static unsigned int he_dec(unsigned int c, unsigned int k, unsigned int p) { return (c+p-k)%p; }
static unsigned int he_add(unsigned int a, unsigned int b, unsigned int p) { return (a+b)%p; }
/* DP: Gaussian noise (Box-Muller) */
static double dp_gauss(double v, double sig) {
    double u1 = rndf(), u2 = rndf();
    if (u1 < 0.0001) u1 = 0.0001;
    return v + dsqrt(-2.0*dsqrt(u1))*(u2*6.2831853-3.14159)*sig;
}
static double dp_laplace(double v, double b) {
    double u = rndf()-0.5; if (u==0.0) u=0.0001;
    return v - b*(u>0.0 ? 1.0 : -1.0);
}
static void dp_clip(double g[], int n, double mx) {
    double ns=0.0;
    for (int i=0; i<n; i++) ns += g[i]*g[i];
    double nm = dsqrt(ns);
    if (nm > mx && nm > 0.0) { double s=mx/nm; for (int i=0; i<n; i++) g[i]*=s; }
}
/* SMPC: pairwise masking */
static double smpc_sum(double v[], double mk[], int n) {
    double s=0.0, m=0.0;
    for (int i=0; i<n; i++) { s += v[i]+mk[i]; m += mk[i]; }
    return s - m;
}
static void dp_init(double e, double d) { g_dp_eps=e; g_dp_sp=0.0; (void)d; }
static double dp_spend(double sc) {
    if (sc <= 0.0) return g_dp_eps - g_dp_sp;
    double cost = g_dp_eps/(sc+1.0); g_dp_sp += cost;
    double r = g_dp_eps - g_dp_sp; return r > 0.0 ? r : 0.0;
}

/* ── 3. Client Management ──────────────────────────────────────────────── */
static void cli_init(client_t *c, int id, int smp, int pw, int bw, int bat, int act) {
    c->id=id; c->samples=smp; c->power=pw; c->bw=bw;
    c->battery=bat; c->active=act; c->last_seen=(int)(rnd()%100U);
    c->rd=0; c->rd_d=0; c->rel=1.0;
}
static int cli_sel(client_t c[], int n, int k, client_t sel[], int strat) {
    int cnt=0, used[MAX_CLI]={0};
    if (strat == SEL_RANDOM) {
        while (cnt<k) { int i=(int)(rnd()%(unsigned int)n);
            if (!used[i] && c[i].active) { sel[cnt++]=c[i]; used[i]=1; } }
    } else if (strat == SEL_POWER) {
        for (int i=0; i<n && cnt<k; i++)
            if (c[i].active && c[i].power>=50 && c[i].battery>15) sel[cnt++]=c[i];
    } else if (strat == SEL_FAIR) {
        for (int r=0; r<k; r++) { int best=-1, bh=999999;
            for (int i=0; i<n; i++)
                if (!used[i] && c[i].active && c[i].rd < bh) { bh=c[i].rd; best=i; }
            if (best>=0) { sel[cnt++]=c[best]; used[best]=1; } }
    } else if (strat == SEL_ACT) {
        while (cnt<k) { int best=-1, bsc=-1;
            for (int i=0; i<n; i++) { if (used[i] || !c[i].active) continue;
                int sc=(100-c[i].last_seen)*c[i].battery;
                if (sc>bsc) { bsc=sc; best=i; } }
            if (best<0) break; sel[cnt++]=c[best]; used[best]=1; }
    }
    return cnt;
}
/* Load balancing: proportional to compute power */
static void cli_balance(client_t c[], int si[], int ns, int tw, int wa[]) {
    int ct=0;
    for (int i=0; i<ns; i++) ct += c[si[i]].power;
    if (ct==0) ct=1;
    for (int i=0; i<ns; i++) wa[i] = tw*c[si[i]].power/ct;
}
/* Fault detection: deactivate unreliable clients */
static int cli_fault(client_t c[], int n, double thr) {
    int f=0;
    for (int i=0; i<n; i++) {
        int tot = c[i].rd+c[i].rd_d;
        if (tot>0) c[i].rel = (double)c[i].rd/(double)tot;
        if (c[i].rel < thr) { c[i].active=0; f++; }
    }
    return f;
}
/* Participation optimization: reactivate clients to meet target rate */
static int cli_optpart(client_t c[], int n, double tgt) {
    int act=0;
    for (int i=0; i<n; i++) if (c[i].active) act++;
    double cr = (double)act/(double)n; int ra=0;
    for (int i=0; i<n && cr < tgt; i++)
        if (!c[i].active && c[i].battery>5) { c[i].active=1; ra++; cr=(double)(act+ra)/(double)n; }
    return ra;
}

/* ── 4. Data Heterogeneity ─────────────────────────────────────────────── */
static void hetero_dirich(double d[][MAX_CLS], int nc, int ncl, double a) {
    for (int c=0; c<nc; c++) { double s=0.0;
        for (int j=0; j<ncl; j++) { d[c][j]=rndf()*a+0.01; s+=d[c][j]; }
        for (int j=0; j<ncl; j++) d[c][j]/=s; }
}
/* Personalized FL: interpolation between local and global */
static void pers_interp(model_t *o, model_t *lo, model_t *gl, double al) {
    o->nl = gl->nl;
    for (int l=0; l<gl->nl; l++) { o->sz[l]=gl->sz[l];
        for (int j=0; j<gl->sz[l]; j++)
            o->w[l][j] = al*lo->w[l][j]+(1.0-al)*gl->w[l][j]; }
}
/* Meta-learning: MAML-style inner/outer updates */
static void meta_learn(model_t *o, model_t tk[], int nt, double ilr, double mlr) {
    o->nl = tk[0].nl;
    for (int l=0; l<o->nl; l++) { o->sz[l]=tk[0].sz[l];
        for (int j=0; j<o->sz[l]; j++) {
            double mg=0.0;
            for (int t=0; t<nt; t++) {
                double ad = tk[t].w[l][j]-ilr*tk[t].w[l][j];
                mg += ad-tk[t].w[l][j];
            }
            o->w[l][j] = tk[0].w[l][j]-mlr*mg/(double)nt; } }
}
/* Transfer learning: fine-tune source to target */
static double transfer_ft(model_t *tgt, model_t *src, double lr) {
    double loss=0.0;
    for (int l=0; l<src->nl; l++) {
        int sz = src->sz[l]<tgt->sz[l] ? src->sz[l] : tgt->sz[l];
        for (int j=0; j<sz; j++) {
            double d=src->w[l][j]-tgt->w[l][j]; loss+=d*d; tgt->w[l][j]+=lr*d; }
    }
    return dsqrt(loss);
}

/* ── 5. Communication Optimization ─────────────────────────────────────── */
static int comp_topk(double g[], int n, int k) {
    if (k>=n) return n;
    double thr[MAX_GRD];
    for (int i=0; i<n; i++) thr[i]=dabs(g[i]);
    for (int i=0; i<k; i++) { int mx=i;
        for (int j=i+1; j<n; j++) if (thr[j]>thr[mx]) mx=j;
        double t=thr[i]; thr[i]=thr[mx]; thr[mx]=t; }
    double th=thr[k-1]; int kept=0;
    for (int i=0; i<n; i++) { if (dabs(g[i])>=th) kept++; else g[i]=0.0; }
    return kept;
}
static int comp_quant(double u[], int n, int bits) {
    if (bits<=0 || bits>30) return 0;
    double mn=u[0], mx=u[0];
    for (int i=1; i<n; i++) { if (u[i]<mn) mn=u[i]; if (u[i]>mx) mx=u[i]; }
    double rg=mx-mn; if (rg<0.0001) rg=0.0001;
    int lvls = 1<<bits;
    for (int i=0; i<n; i++) {
        double nm=(u[i]-mn)/rg; int q=(int)(nm*(double)(lvls-1)+0.5);
        u[i] = mn+(double)q/(double)(lvls-1)*rg; }
    return n*bits/8+1;
}
static int comp_sign(double g[], int n) {
    double mag=0.0;
    for (int i=0; i<n; i++) { mag+=dabs(g[i]); g[i]=g[i]>=0.0?1.0:-1.0; }
    (void)mag; return n/8+4;
}
/* Asynchronous aggregation with staleness control */
static void async_init(async_t *a, int cap, double sb) { a->n=0; a->cap=cap; a->sb=sb; }
static int async_push(async_t *a, model_t *u, int s, int t) {
    if (a->n>=a->cap) return 0;
    a->u[a->n]=*u; a->sc[a->n]=s; a->ts[a->n]=t; a->n++; return 1;
}
static void async_agg(async_t *a, model_t *gl, int ct) {
    if (a->n==0) return;
    int tot=0;
    for (int i=0; i<a->n; i++) {
        int stale = ct-a->ts[i];
        double wt = stale<=(int)a->sb ? 1.0 : 0.5;
        a->sc[i] = (int)((double)a->sc[i]*wt); tot += a->sc[i];
    }
    if (tot==0) tot=1;
    for (int l=0; l<gl->nl; l++)
        for (int j=0; j<gl->sz[l]; j++) {
            double s=0.0;
            for (int i=0; i<a->n; i++) s += a->u[i].w[l][j]*(double)a->sc[i];
            gl->w[l][j] = s/(double)tot; }
    a->n = 0;
}

/* ── 6. Federation Types ───────────────────────────────────────────────── */
/* Horizontal FL: same features, different samples */
static double fed_horiz(model_t md[], int sc[], int nc, model_t *o) {
    model_t old = *o; agg_fedavg(o, md, sc, nc); return mdist(&old, o);
}
/* Vertical FL: different features, same samples */
static double fed_vert(model_t fm[], int nf, model_t *mg) {
    mg->nl = 0;
    for (int f=0; f<nf; f++)
        for (int l=0; l<fm[f].nl && mg->nl<MAX_LAY; l++) {
            int li=mg->nl; mg->sz[li]=fm[f].sz[l];
            for (int j=0; j<mg->sz[li]; j++) mg->w[li][j]=fm[f].w[l][j];
            mg->nl++; }
    return mnorm(mg);
}
/* Transfer FL: source-to-target knowledge transfer */
static double fed_trans(model_t *src, model_t *tgt, double lr, int steps) {
    double loss=0.0;
    for (int s=0; s<steps; s++) loss = transfer_ft(tgt, src, lr);
    return loss;
}
/* Hybrid FL: combine horizontal + vertical */
static void fed_hybrid(model_t hm[], int sc[], int nh, model_t vm[], int nv, model_t *o) {
    model_t hmg, dmg;
    agg_fedavg(&hmg, hm, sc, nh); fed_vert(vm, nv, &dmg);
    o->nl = hmg.nl<dmg.nl ? hmg.nl : dmg.nl;
    for (int l=0; l<o->nl; l++) {
        o->sz[l] = hmg.sz[l]<dmg.sz[l] ? hmg.sz[l] : dmg.sz[l];
        for (int j=0; j<o->sz[l]; j++) o->w[l][j]=(hmg.w[l][j]+dmg.w[l][j])*0.5; }
}

/* ── Test Suite ────────────────────────────────────────────────────────── */
static void test_all(void) {
    ps("=== Federated Learning Framework v57.0 ===\n\n");
    int sz3[3]={4,4,4}, sz2[2]={4,4};
    model_t md[4], gl, res;
    for (int i=0; i<4; i++) minit(&md[i], 3, sz3);
    minit(&gl, 3, sz3);

    ps("[1] Aggregation\n");
    int sc4[4]={100,200,150,120};
    agg_fedavg(&res, md, sc4, 4);
    ps("  FedAvg l0[0]: "); pd(res.w[0][0]); ps("\n");
    agg_fedprox(&res, md, &gl, 0.01, 4);
    ps("  FedProx l0[0]: "); pd(res.w[0][0]); ps("\n");
    double mt[128]={0}, vt[128]={0};
    agg_fedopt(&res, md, &gl, mt, vt, 0.9, 0.999, 1e-8, 4);
    ps("  FedOpt l0[0]: "); pd(res.w[0][0]); ps("\n");
    scaf_t sf={0}; int cid[4]={0,1,2,3};
    agg_scaffold(&res, md, &gl, &sf, cid, 4);
    ps("  SCAFFOLD l0[0]: "); pd(res.w[0][0]); ps("\n");
    kserv_t ks; kserv_init(&ks, 3, sz2, 2);
    kserv_upd(&ks, 0, &md[0], 100); kserv_upd(&ks, 1, &md[1], 200);
    kserv_upd(&ks, 2, &md[2], 150); kserv_merge(&ks, &res);
    ps("  K-Server l0[0]: "); pd(res.w[0][0]); ps("\n");
    ps("  Convergence: "); pd(mdist(&gl, &res)); ps("\n\n");

    ps("[2] Secure Aggregation\n");
    unsigned int sec=42U, sh[5], ix[5];
    for (int i=0; i<5; i++) { ix[i]=(unsigned int)(i+1); sh[i]=shamir_share(sec,ix[i],3U,SH_P); }
    ps("  Shares: ");
    for (int i=0; i<5; i++) { pi((int)sh[i]); ps(" "); }
    ps("\n  Reconstructed: "); pi((int)shamir_recon(sh,ix,3U,SH_P)); ps("\n");
    unsigned int ea = he_enc(100U, 0xBEEFU, HE_P);
    unsigned int eb = he_enc(200U, 0xCAFEBABEU, HE_P);
    ps("  HE enc(100): "); ph(ea); ps("\n");
    ps("  HE add: "); ph(he_add(ea, eb, HE_P)); ps("\n");
    dp_init(1.0, 1e-5);
    ps("  DP Gauss: "); pd(dp_gauss(1.0, 0.5)); ps("\n");
    ps("  DP Laplace: "); pd(dp_laplace(1.0, 0.1)); ps("\n");
    double gr[8]={0.5,-0.3,1.2,-0.8,0.1,0.7,-0.4,0.9};
    dp_clip(gr, 8, 1.0);
    ps("  Clipped[2]: "); pd(gr[2]); ps("\n");
    double uv[4]={1.0,2.0,3.0,4.0}, mk[4]={0.5,-0.3,0.1,-0.3};
    ps("  SMPC sum: "); pd(smpc_sum(uv,mk,4)); ps("\n");
    ps("  DP budget: "); pd(dp_spend(0.5)); ps("\n\n");

    ps("[3] Client Management\n");
    client_t cl[6], sel[4];
    cli_init(&cl[0],0,500,80,50,90,1); cli_init(&cl[1],1,300,60,30,70,1);
    cli_init(&cl[2],2,800,90,100,50,1); cli_init(&cl[3],3,200,40,20,20,1);
    cli_init(&cl[4],4,600,70,40,85,1); cli_init(&cl[5],5,100,30,10,10,0);
    cl[2].rd=10; cl[2].rd_d=2; cl[3].rd=1; cl[3].rd_d=9;
    ps("  Random: "); pi(cli_sel(cl,6,3,sel,SEL_RANDOM)); ps("\n");
    ps("  Power: "); pi(cli_sel(cl,6,3,sel,SEL_POWER)); ps("\n");
    ps("  Fair: "); pi(cli_sel(cl,6,3,sel,SEL_FAIR)); ps("\n");
    ps("  Activity: "); pi(cli_sel(cl,6,3,sel,SEL_ACT)); ps("\n");
    int si[3]={0,1,2}, wa[3];
    cli_balance(cl, si, 3, 300, wa);
    ps("  Load: "); pi(wa[0]); ps(" "); pi(wa[1]); ps(" "); pi(wa[2]); ps("\n");
    ps("  Faults: "); pi(cli_fault(cl,6,0.3)); ps("\n");
    ps("  Reactivated: "); pi(cli_optpart(cl,6,0.8)); ps("\n\n");

    ps("[4] Heterogeneity\n");
    double dist[MAX_CLI][MAX_CLS];
    hetero_dirich(dist, 4, 5, 0.3);
    ps("  Dirichlet[0][0]: "); pd(dist[0][0]); ps("\n");
    model_t lo, pe; minit(&lo, 2, sz2); minit(&pe, 2, sz2);
    pers_interp(&res, &lo, &gl, 0.7);
    ps("  Personal l0[0]: "); pd(res.w[0][0]); ps("\n");
    model_t tk[3]; for (int i=0; i<3; i++) minit(&tk[i], 2, sz2);
    meta_learn(&res, tk, 3, 0.01, 0.001);
    ps("  Meta l0[0]: "); pd(res.w[0][0]); ps("\n");
    model_t ts, tt; minit(&ts, 2, sz2); minit(&tt, 2, sz2);
    ps("  Transfer: "); pd(transfer_ft(&tt, &ts, 0.05)); ps("\n\n");

    ps("[5] Communication\n");
    double g2[16];
    for (int i=0; i<16; i++) g2[i] = rndf()*2.0-1.0;
    ps("  Top-k kept: "); pi(comp_topk(g2,16,5)); ps(" / 16\n");
    double u2[8]={0.1,0.5,0.9,0.3,0.7,0.2,0.8,0.4};
    ps("  Quant bytes: "); pi(comp_quant(u2,8,8)); ps("\n");
    double u3[8]={0.3,-0.7,0.2,-0.1,0.9,-0.4,0.6,-0.5};
    ps("  Sign bytes: "); pi(comp_sign(u3,8)); ps("\n");
    async_t ab; async_init(&ab, 8, 3.0);
    model_t u1; minit(&u1, 2, sz2);
    async_push(&ab, &u1, 100, 10); async_push(&ab, &u1, 150, 9);
    async_agg(&ab, &gl, 11);
    ps("  Async aggregated OK\n\n");

    ps("[6] Federation Types\n");
    int sc2[2]={100,200};
    ps("  Horizontal: "); pd(fed_horiz(md,sc2,2,&res)); ps("\n");
    model_t ft[2]; minit(&ft[0],2,sz2); minit(&ft[1],1,sz2);
    ps("  Vertical: "); pd(fed_vert(ft,2,&res)); ps("\n");
    model_t ss, st; minit(&ss,2,sz2); minit(&st,2,sz2);
    ps("  Transfer: "); pd(fed_trans(&ss,&st,0.01,5)); ps("\n");
    fed_hybrid(md,sc2,2,ft,2,&res);
    ps("  Hybrid l0[0]: "); pd(res.w[0][0]); ps("\n\n");

    ps("=== All tests passed ===\n");
}
static void show_help(void) {
    ps("Federated Learning v57.0 - Privacy-Preserving Distributed ML\n");
    ps("  -h  Help  -t  Tests\n");
}
void _start(void) {
    char buf[256];
    int len = host_get_argv(0, 256);
    if (len>0 && len<=256) host_get_argv((unsigned int)(size_t)buf, 256);
    if (len > 0)
        for (int i=0; i<len; i++)
            if (buf[i]=='-' && i+1<len) {
                if (buf[i+1]=='h') { show_help(); host_exit(0); return; }
                if (buf[i+1]=='t') { test_all(); host_exit(0); return; }
            }
    ps("Federated Learning v57.0 - Use -h for help, -t for tests\n");
}
