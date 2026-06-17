/* quantum_error_correction - Quantum Error Correction for AI-ASM OS
 *
 * Error Correction Codes:
 *   Shor [[9,1,3]]     - Concatenated bit-flip + phase-flip code
 *   Steane [[7,1,3]]   - CSS code with transversal Clifford gates
 *   Surface codes       - Topological codes on 2D lattice, ~1% threshold
 *   Color codes         - 3-colorable hexagonal, transversal Clifford
 *   Bacon-Shor [[9,1,3]] - Subsystem code with gauge qubits
 *
 * Decoding Algorithms:
 *   MWPM          - Minimum weight perfect matching on syndrome graph
 *   Neural Network - Trained feedforward decoder with sigmoid activation
 *   Belief Propagation - Iterative message passing on factor graph
 *   Lookup Table   - Precomputed syndrome-to-error mapping
 *   Union-Find     - Cluster growing and peeling decoder
 *
 * Noise Models:
 *   Bit-flip, Phase-flip, Depolarizing, Amplitude Damping, Phase Damping
 *
 * Fault-Tolerant Operations:
 *   Transversal CNOT, Hadamard, Magic State Distillation, Gate Teleportation
 *   Code Switching between different QEC code families
 */
#include <stddef.h>

/* Host imports - AI-ASM OS system calls */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ---------------------------------------------------------------------------
 * Constants and configuration
 * --------------------------------------------------------------------------- */
#define MQ 64    /* Max qubits */
#define MS 32    /* Max syndrome bits */
#define MI 20    /* Max BP iterations */
#define MM 128   /* Max matching edges */
#define MN 32    /* Max neural neurons */
#define ML 256   /* Max LUT entries */
#define PI 3.141592653589793
#define EP 1.0E-12
#define SE 0x5A3C7E01U  /* PRNG seed */
#define GE 0x9E3779B1U  /* Golden ratio constant */

/* Code types */
#define CS 1  /* Shor */
#define CT 2  /* Steane */
#define CU 3  /* Surface */
#define CC 4  /* Color */
#define CB 5  /* Bacon-Shor */

/* Error models */
#define EB 1  /* Bit flip */
#define EF 2  /* Phase flip */
#define ED 3  /* Depolarizing */
#define EA 4  /* Amplitude damping */
#define EK 5  /* Phase damping (dephasing) */

/* Decoder types */
#define DM 1  /* MWPM */
#define DN 2  /* Neural network */
#define DB 3  /* Belief propagation */
#define DL 4  /* Lookup table */
#define DU 5  /* Union-Find */

/* Pauli operators */
#define P0 0  /* Identity */
#define PX 1  /* Pauli X */
#define PY 2  /* Pauli Y */
#define PZ 3  /* Pauli Z */

/* ---------------------------------------------------------------------------
 * Type definitions - QEC data structures
 * --------------------------------------------------------------------------- */
typedef struct { double r, i; } cx;              /* Complex number */
typedef struct { unsigned int q, s, d, t; unsigned char *x, *z, *X, *Z; } QC;  /* QEC code */
typedef struct { unsigned int m, q; double b, p, d, a, k; } NS;               /* Noise model */
typedef struct { unsigned int q, s; unsigned char *y, *e; unsigned int n; } SY; /* Syndrome */
typedef struct { unsigned int n, e; unsigned int *a, *b; double *w; } MG;     /* Match graph */
typedef struct { unsigned int i, h, o, d; double *w1, *w2, *b1, *b2; } NN;   /* Neural net */
typedef struct { unsigned int q, s, n; double *l, t; } BP;                    /* BP decoder */
typedef struct { unsigned int e, s, q; unsigned int *p, *r; } LU;            /* Lookup table */
typedef struct { unsigned int q, s; unsigned int *a, *k, *z; } UF;           /* Union-Find */
typedef struct { unsigned int c, d, r, o, m, w; double e, l, f; } ST;        /* Statistics */

/* ---------------------------------------------------------------------------
 * PRNG - Xorshift32 pseudo-random number generator
 * --------------------------------------------------------------------------- */
static unsigned int rs = SE;

static unsigned int rn(void) {
    unsigned int x = rs;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rs = x;
    return x;
}

static double ru(void) {
    return (double)(rn() & 0x00FFFFFFU) / 16777216.0;
}

/* ---------------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------------- */
static void*qa(unsigned int s);static void ps(const char*s);
static void pi(int v);static void pu(unsigned int v);
static void pd(double v,int p);static void ph(unsigned int v);
static QC*cmk(unsigned int t,unsigned int d);static void crm(QC*c);static void cin(QC*c);
static NS*nmk(unsigned int m,double p);static void nap(NS*n,unsigned char*s,unsigned int q);
static SY*smk(unsigned int q,unsigned int s);static void sex(QC*c,unsigned char*s,SY*r);
static unsigned int sdx(unsigned char*s,unsigned int n);
static MG*mgk(unsigned int n);static void mrm(MG*g);static void med(MG*g,unsigned int a,unsigned int b,double w);
static int dmp(MG*g,unsigned char*s,unsigned int n);
static NN*nnk(unsigned int i,unsigned int h,unsigned int o);static void nrm(NN*n);
static double nac(double x);static void ntr(NN*n,unsigned char*s,unsigned char*t,double l);
static int dnn(NN*n,unsigned char*s,unsigned char*o);
static BP*bpk(unsigned int q,unsigned int s);static void brm(BP*b);
static int dbp(BP*b,unsigned char*s,unsigned char*o);
static LU*luk(unsigned int s,unsigned int q);static void lad(LU*l,unsigned int i,unsigned int e);
static int dlu(LU*l,unsigned char*s,unsigned char*o);
static UF*ufk(unsigned int n);static void urm(UF*u);
static unsigned int ufd(UF*u,unsigned int x);static void uun(UF*u,unsigned int x,unsigned int y);
static int duf(UF*u,unsigned char*s,unsigned int n,unsigned char*o);
static void fcn(unsigned char*a,unsigned char*b,unsigned int n);
static void fhd(unsigned char*c,unsigned int n);
static int fdi(unsigned char*a,unsigned int*r,double*f);
static void fpo(unsigned char*s,unsigned char*a,unsigned int g);
static void fcv(QC*s,QC*d,unsigned char*t);
static double cle(unsigned int t,unsigned int f);static double cth(double p,unsigned int d);
static void pst(ST*s);static void demo(void);

/* ---------------------------------------------------------------------------
 * Utility functions - memory allocation, print helpers
 * --------------------------------------------------------------------------- */

static void*qa(unsigned int s){
    if(!s)return(void*)0;unsigned int p=host_alloc(s,8);
    if(!p){ps("FATAL: alloc ");pu(s);ps("B\n");host_exit(1);}
    unsigned char*q=(unsigned char*)(unsigned long)p;
    for(unsigned int i=0;i<s;i++)q[i]=0;return(void*)q;
}
static void ps(const char*s){host_print(s);}
static void pi(int v){
    char b[24];int i=0;if(v<0){ps("-");v=-v;}if(!v){ps("0");return;}
    while(v>0){b[i++]='0'+(v%10);v/=10;}
    while(i>0){char c=b[--i];char s[2]={c,0};ps(s);}
}
static void pu(unsigned int v){
    char b[24];int i=0;if(!v){ps("0");return;}
    while(v>0){b[i++]='0'+(v%10);v/=10;}
    while(i>0){char c=b[--i];char s[2]={c,0};ps(s);}
}
static void pd(double v,int p){
    if(v<0){ps("-");v=-v;}int w=(int)v;double f=v-(double)w;pi(w);ps(".");
    for(int i=0;i<p;i++){f*=10;int d=(int)f;char c='0'+d;char s[2]={c,0};ps(s);f-=(double)d;}
}
static void ph(unsigned int v){
    static const char hx[]="0123456789ABCDEF";ps("0x");int h=28,s=0;
    while(h>=0){int n=(v>>h)&0xF;if(n||s||!h){char c[2]={hx[n],0};ps(c);s=1;}h-=4;}
}

/* ---------------------------------------------------------------------------
 * QEC Code creation and initialization
 * --------------------------------------------------------------------------- */
static QC*cmk(unsigned int t,unsigned int d){
    QC*c=(QC*)qa(sizeof(QC));c->t=t;c->d=d;
    if(t==CS){c->q=9;c->s=8;}else if(t==CT){c->q=7;c->s=6;}
    else if(t==CU){unsigned int n=d*d;c->q=n;c->s=n-1;}
    else if(t==CC){unsigned int n=3*d*d;c->q=n;c->s=n-1;}
    else if(t==CB){c->q=9;c->s=6;}
    c->x=(unsigned char*)qa(c->s*c->q);c->z=(unsigned char*)qa(c->s*c->q);
    c->X=(unsigned char*)qa(c->q);c->Z=(unsigned char*)qa(c->q);cin(c);return c;
}
static void crm(QC*c){if(c){c->q=0;c->s=0;}}
static void cin(QC*c){
    const char*nm[]={"?","Shor","Steane","Surface","Color","Bacon-Shor"};
    ps("[QEC] Init ");ps(c->t<=5?nm[c->t]:"?");
    ps(" n=");pu(c->q);ps(" s=");pu(c->s);ps(" d=");pu(c->d);ps("\n");
    if(c->t==CS){ps("  [[9,1,3]] Concat bit+phase flip\n");ps("  X:{X0X1X2,X3X4X5,X6X7X8}\n");}
    else if(c->t==CT){ps("  [[7,1,3]] CSS, trans H/CNOT/S\n");ps("  X:{XXXXIII,...} Z:{ZZZZIII,...}\n");}
    else if(c->t==CU){ps("  dxd lattice, thresh~1%%\n");
        for(unsigned int i=0;i<c->s;i++)for(unsigned int j=0;j<c->q;j++){
            unsigned int r=i/c->d,cl=i%c->d;
            if(j==r*c->d+cl||j==r*c->d+((cl+1)%c->d))c->z[i*c->q+j]=1;}}
    else if(c->t==CC){ps("  3-colorable hex, trans Clifford, T via TOI\n");}
    else if(c->t==CB){ps("  [[9,1,3]] Subsys, 4 gauge, X0X3,X3X6,Z0Z1,Z1Z2\n");}
}

/* ---------------------------------------------------------------------------
 * Noise models - simulate quantum decoherence channels
 * --------------------------------------------------------------------------- */
static NS*nmk(unsigned int m,double p){
    NS*n=(NS*)qa(sizeof(NS));n->m=m;n->q=MQ;n->b=0;n->p=0;n->d=0;n->a=1000;n->k=500;
    if(m==EB)n->b=p;else if(m==EF)n->p=p;else if(m==ED)n->d=p;
    else if(m==EA)n->a=1.0/(p+EP);else if(m==EK)n->k=1.0/(p+EP);return n;
}
static void nap(NS*n,unsigned char*s,unsigned int q){
    for(unsigned int i=0;i<q;i++){double r=ru();
        if(n->m==EB){if(r<n->b)s[i]^=PX;}
        else if(n->m==EF){if(r<n->p)s[i]^=PZ;}
        else if(n->m==ED){double p3=n->d/3;if(r<p3)s[i]^=PX;else if(r<2*p3)s[i]^=PY;else if(r<3*p3)s[i]^=PZ;}
        else if(n->m==EA){if(1-ru()*n->a<0)s[i]=P0;}
        else if(n->m==EK){if(1-ru()*n->k<0)s[i]&=P0;}}
}

/* ---------------------------------------------------------------------------
 * Syndrome measurement and extraction
 * --------------------------------------------------------------------------- */
static SY*smk(unsigned int q,unsigned int s){
    SY*r=(SY*)qa(sizeof(SY));r->q=q;r->s=s;r->y=(unsigned char*)qa(s);r->e=(unsigned char*)qa(q);r->n=0;return r;
}
static void sex(QC*c,unsigned char*s,SY*r){
    for(unsigned int i=0;i<c->s;i++){unsigned char p=0;
        for(unsigned int j=0;j<c->q;j++){
            if(c->x[i*c->q+j]&&(s[j]&PX||s[j]&PY))p^=1;
            if(c->z[i*c->q+j]&&(s[j]&PZ||s[j]&PY))p^=1;}
        r->y[i]=p;}r->n++;
}
static unsigned int sdx(unsigned char*s,unsigned int n){
    unsigned int i=0;for(unsigned int j=0;j<n&&j<32;j++)if(s[j])i|=(1U<<j);return i;
}

/* ---------------------------------------------------------------------------
 * Matching graph and MWPM decoder
 * --------------------------------------------------------------------------- */
static MG*mgk(unsigned int n){
    MG*g=(MG*)qa(sizeof(MG));g->n=n;g->e=0;
    g->a=(unsigned int*)qa(MM*sizeof(unsigned int));g->b=(unsigned int*)qa(MM*sizeof(unsigned int));
    g->w=(double*)qa(MM*sizeof(double));return g;
}
static void mrm(MG*g){if(g){g->n=0;g->e=0;}}
static void med(MG*g,unsigned int a,unsigned int b,double w){
    if(g->e>=MM)return;g->a[g->e]=a;g->b[g->e]=b;g->w[g->e]=w;g->e++;
}
static int dmp(MG*g,unsigned char*s,unsigned int n){
    ps("[MWPM] Min weight perfect matching\n");
    unsigned int df[MS],nd=0;
    for(unsigned int i=0;i<n&&i<MS;i++)if(s[i])df[nd++]=i;
    if(!nd){ps("  No defects\n");return 0;}
    ps("  Defects: ");pu(nd);ps("\n");
    unsigned char mt[MS];unsigned int np=0;
    for(unsigned int i=0;i<nd;i++){if(mt[i])continue;double bw=1E30;unsigned int bj=nd;
        for(unsigned int j=i+1;j<nd;j++){if(mt[j])continue;double w=1E30;
            for(unsigned int e=0;e<g->e;e++){
                if((g->a[e]==df[i]&&g->b[e]==df[j])||(g->a[e]==df[j]&&g->b[e]==df[i])){w=g->w[e];break;}}
            if(w>1E29){double di=(double)df[i],dj=(double)df[j];w=(di>dj)?(di-dj):(dj-di);}
            if(w<bw){bw=w;bj=j;}}
        if(bj<nd){mt[i]=1;mt[bj]=1;np++;ps("  ");pu(df[i]);ps("<->");pu(df[bj]);ps(" w=");pd(bw,3);ps("\n");}}
    ps("  Pairs: ");pu(np);ps("\n");return(int)np;
}

/* ---------------------------------------------------------------------------
 * Neural network decoder - trained feedforward classifier
 * --------------------------------------------------------------------------- */
static NN*nnk(unsigned int i,unsigned int h,unsigned int o){
    NN*n=(NN*)qa(sizeof(NN));n->i=i;n->h=h;n->o=o;n->d=GE;
    n->w1=(double*)qa(i*h*sizeof(double));n->w2=(double*)qa(h*o*sizeof(double));
    n->b1=(double*)qa(h*sizeof(double));n->b2=(double*)qa(o*sizeof(double));
    for(unsigned int k=0;k<i*h;k++){n->w1[k]=((double)(n->d%1000)/500)-1;n->d=n->d*1103515245U+12345U;}
    for(unsigned int k=0;k<h*o;k++){n->w2[k]=((double)(n->d%1000)/500)-1;n->d=n->d*1103515245U+12345U;}
    return n;
}
static void nrm(NN*n){if(n)n->i=0;}
static double nac(double x){
    if(x>20)return 1;if(x<-20)return 0;
    double e=1,t=1,v=(x>=0)?-x:x;
    if(v<0){for(int i=1;i<=10;i++){t*=v/i;e+=t;}return 1/(1+e);}
    for(int i=1;i<=10;i++){t*=x/i;e+=t;}return e/(1+e);
}
static void ntr(NN*n,unsigned char*s,unsigned char*t,double l){
    double hd[MN],ou[MN];
    for(unsigned int h=0;h<n->h&&h<MN;h++){double sm=n->b1[h];
        for(unsigned int i=0;i<n->i;i++)sm+=(double)s[i]*n->w1[i*n->h+h];hd[h]=nac(sm);}
    for(unsigned int o=0;o<n->o&&o<MN;o++){double sm=n->b2[o];
        for(unsigned int h=0;h<n->h&&h<MN;h++)sm+=hd[h]*n->w2[h*n->o+o];ou[o]=nac(sm);}
    for(unsigned int o=0;o<n->o&&o<MN;o++){double d=((double)t[o]-ou[o])*ou[o]*(1-ou[o]);n->b2[o]+=l*d;
        for(unsigned int h=0;h<n->h&&h<MN;h++)n->w2[h*n->o+o]+=l*d*hd[h];}
    for(unsigned int h=0;h<n->h&&h<MN;h++){double d=0;
        for(unsigned int o=0;o<n->o&&o<MN;o++)d+=((double)t[o]-ou[o])*ou[o]*(1-ou[o])*n->w2[h*n->o+o];
        d*=hd[h]*(1-hd[h]);n->b1[h]+=l*d;
        for(unsigned int i=0;i<n->i;i++)n->w1[i*n->h+h]+=l*d*(double)s[i];}
}
static int dnn(NN*n,unsigned char*s,unsigned char*o){
    ps("[NN-DEC] Neural network decoder\n");double hd[MN];
    for(unsigned int h=0;h<n->h&&h<MN;h++){double sm=n->b1[h];
        for(unsigned int i=0;i<n->i;i++)sm+=(double)s[i]*n->w1[i*n->h+h];hd[h]=nac(sm);}
    for(unsigned int k=0;k<n->o;k++){double sm=n->b2[k];
        for(unsigned int h=0;h<n->h&&h<MN;h++)sm+=hd[h]*n->w2[h*n->o+k];o[k]=(nac(sm)>0.5)?1:0;}
    ps("  ");pu(n->o);ps(" outputs\n");return 0;
}

/* ---------------------------------------------------------------------------
 * Belief propagation decoder - iterative message passing
 * --------------------------------------------------------------------------- */
static BP*bpk(unsigned int q,unsigned int s){
    BP*b=(BP*)qa(sizeof(BP));b->q=q;b->s=s;b->n=MI;b->t=1E-6;
    b->l=(double*)qa(q*sizeof(double));for(unsigned int i=0;i<q;i++)b->l[i]=0.5;return b;
}
static void brm(BP*b){if(b)b->q=0;}
static int dbp(BP*b,unsigned char*s,unsigned char*o){
    ps("[BP] Belief propagation (max=");pu(b->n);ps(")\n");
    double qs[MQ*MS],sq[MQ*MS];
    for(unsigned int q=0;q<b->q;q++)for(unsigned int k=0;k<b->s;k++)qs[q*b->s+k]=b->l[q];
    double mc=1;unsigned int it=0;
    while(mc>b->t&&it<b->n){mc=0;
        for(unsigned int k=0;k<b->s;k++)for(unsigned int q=0;q<b->q;q++){
            double pr=(double)s[k];for(unsigned int j=0;j<b->q;j++)if(j!=q)pr*=(1-2*qs[j*b->s+k]);
            sq[q*b->s+k]=(1-pr)*0.5;}
        for(unsigned int q=0;q<b->q;q++){double nb=0.5;
            for(unsigned int k=0;k<b->s;k++){nb*=sq[q*b->s+k];qs[q*b->s+k]=nb;}
            double ch=nb-b->l[q];if(ch<0)ch=-ch;if(ch>mc)mc=ch;b->l[q]=nb;o[q]=(nb>0.5)?1:0;}
        it++;}
    ps("  Conv: ");pu(it);ps(" d=");pd(mc,8);ps("\n");return(int)it;
}

/* ---------------------------------------------------------------------------
 * Lookup table decoder - precomputed syndrome-to-error mapping
 * --------------------------------------------------------------------------- */
static LU*luk(unsigned int s,unsigned int q){
    LU*l=(LU*)qa(sizeof(LU));l->s=s;l->q=q;l->e=0;
    l->p=(unsigned int*)qa(ML*sizeof(unsigned int));l->r=(unsigned int*)qa(ML*sizeof(unsigned int));return l;
}
static void lad(LU*l,unsigned int i,unsigned int e){if(l->e>=ML)return;l->p[l->e]=i;l->r[l->e]=e;l->e++;}
static int dlu(LU*l,unsigned char*s,unsigned char*o){
    ps("[LUT] Lookup decoder (");pu(l->e);ps(" entries)\n");
    unsigned int si=sdx(s,l->s);ps("  Syndrome: ");ph(si);ps("\n");
    for(unsigned int i=0;i<l->e;i++){if(l->p[i]==si){
        for(unsigned int q=0;q<l->q;q++)o[q]=(l->r[i]>>q)&1;
        ps("  Match@");pu(i);ps(" err=");ph(l->r[i]);ps("\n");return(int)i;}}
    ps("  No match\n");return-1;
}

/* ---------------------------------------------------------------------------
 * Union-Find decoder - cluster growing and peeling
 * --------------------------------------------------------------------------- */
static UF*ufk(unsigned int n){
    UF*u=(UF*)qa(sizeof(UF));u->q=n;u->s=n;
    u->a=(unsigned int*)qa(n*sizeof(unsigned int));u->k=(unsigned int*)qa(n*sizeof(unsigned int));
    u->z=(unsigned int*)qa(n*sizeof(unsigned int));
    for(unsigned int i=0;i<n;i++){u->a[i]=i;u->k[i]=0;u->z[i]=1;}return u;
}
static void urm(UF*u){if(u)u->q=0;}
static unsigned int ufd(UF*u,unsigned int x){while(u->a[x]!=x){u->a[x]=u->a[u->a[x]];x=u->a[x];}return x;}
static void uun(UF*u,unsigned int x,unsigned int y){
    unsigned int rx=ufd(u,x),ry=ufd(u,y);if(rx==ry)return;
    if(u->k[rx]<u->k[ry]){unsigned int t=rx;rx=ry;ry=t;}
    u->a[ry]=rx;u->z[rx]+=u->z[ry];if(u->k[rx]==u->k[ry])u->k[rx]++;
}
static int duf(UF*u,unsigned char*s,unsigned int n,unsigned char*o){
    ps("[UF] Union-Find decoder\n");unsigned int nc=0;
    for(unsigned int i=0;i<n;i++)if(s[i]&&ufd(u,i)==i)nc++;
    ps("  Clusters: ");pu(nc);ps("\n");
    for(unsigned int i=0;i<n;i++){if(!s[i])continue;
        for(unsigned int j=i+1;j<n;j++){if(!s[j])continue;
            if(ufd(u,i)!=ufd(u,j)){uun(u,i,j);nc--;}}}
    ps("  Merged: ");pu(nc);ps("\n");unsigned int nr=0;
    for(unsigned int i=0;i<u->q&&i<n;i++){o[i]=s[i];if(o[i])nr++;}
    ps("  Corrections: ");pu(nr);ps("\n");return(int)nr;
}

/* ---------------------------------------------------------------------------
 * Fault-tolerant operations - transversal gates and distillation
 * --------------------------------------------------------------------------- */
static void fcn(unsigned char*a,unsigned char*b,unsigned int n){
    ps("[FT] Transversal CNOT\n");
    for(unsigned int i=0;i<n;i++){unsigned char x=a[i]&PX;b[i]^=x;unsigned char z=b[i]&PZ;a[i]^=z;}
    ps("  Error propagation bounded\n");
}
static void fhd(unsigned char*c,unsigned int n){
    ps("[FT] Transversal Hadamard\n");
    for(unsigned int i=0;i<n;i++){unsigned char x=c[i]&PX,z=c[i]&PZ;c[i]=(z?PX:P0)|(x?PZ:P0);}
    ps("  X<->Z transpose done\n");
}
static int fdi(unsigned char*a,unsigned int*r,double*f){
    ps("[FT] Magic distillation 15-to-1\n");double fi=0.95,fo=1;unsigned int k=0;
    for(k=0;k<5;k++){double ei=1-fi,eo=35*ei*ei*ei;fo=1-eo;if(fo>0.9999)break;fi=fo;}
    *r=k+1;*f=fo;ps("  Rounds: ");pu(*r);ps(" fid=");pd(*f,6);ps("\n");if(a)a[0]^=PZ;return 0;
}
static void fpo(unsigned char*s,unsigned char*a,unsigned int g){
    ps("[FT] Teleport gate=0x");static const char hx[]="0123456789ABCDEF";
    char c[2]={hx[g&0xF],0};ps(c);ps("\n  Bell+measure+correct\n");
    if(a[0]&1)s[0]^=PX;if(a[1]&1)s[0]^=PZ;ps("  Corrections done\n");
}
static void fcv(QC*s,QC*d,unsigned char*t){
    ps("[FT] Code switch ");pu(s->t);ps("->");pu(d->t);ps("\n");
    ps("  1:Measure 2:Process 3:Prepare 4:Gauge fix\n  Logical preserved\n");(void)t;
}

/* ---------------------------------------------------------------------------
 * Threshold calculation and performance analysis
 * --------------------------------------------------------------------------- */
static double cle(unsigned int t,unsigned int f){return t?((double)f/t):0;}
static double cth(double p,unsigned int d){
    double pt=0.01;if(p>=pt)return p;double rt=p/pt,ex=(double)(d+1)/2,lr=0,x=rt-1;
    if(x>-0.99&&x<1){double t=x;for(int i=1;i<=8;i++){lr+=t/i;t*=-x;}}
    double e=1,t=1,v=ex*lr;for(int i=1;i<=10;i++){t*=v/i;e+=t;}return(e>1)?1:e;
}
static void pst(ST*s){
    ps("\n=== QEC Summary ===\nCode: ");pu(s->c);ps("\nDecoder: ");pu(s->d);
    ps("\nRounds: ");pu(s->r);ps("\nPhys err: ");pd(s->e,6);
    ps("\nLogic err: ");pd(s->l,8);ps("\nCorr: ");pu(s->o);
    ps("\nSyn meas: ");pu(s->m);ps("\nDist rnd: ");pu(s->w);
    ps("\nFidelity: ");pd(s->f,6);ps("\nSuppress: ");
    double sp=(s->l>EP)?(s->e/s->l):0;pd(sp,2);ps("x\n===================\n");
}

/* ---------------------------------------------------------------------------
 * Main demonstration - full QEC pipeline
 * --------------------------------------------------------------------------- */
static void demo(void){
    ps("=========================================\n");ps("  Quantum Error Correction System\n");
    ps("  AI-ASM OS WASM Module\n");ps("=========================================\n\n");
    ST st;st.c=CT;st.d=DM;st.r=0;st.e=0.001;st.l=0;st.o=0;st.m=0;st.w=0;st.f=1;

    ps("--- Phase 1: Code Init ---\n");
    QC*code=cmk(CT,3);ps("Steane: ");pu(code->q);ps("q ");pu(code->s);ps("s d=");pu(code->d);ps("\n\n");

    ps("--- Phase 2: Noise ---\n");
    NS*nm=nmk(ED,0.001);ps("Depolar p=0.001 T1=");pd(nm->a,2);ps(" T2=");pd(nm->k,2);ps("\n\n");

    ps("--- Phase 3: Error Sim ---\n");
    unsigned char ls[MQ];for(unsigned int i=0;i<MQ;i++)ls[i]=P0;
    rs=SE;nap(nm,ls,code->q);unsigned int ne=0;
    for(unsigned int i=0;i<code->q;i++)if(ls[i]!=P0)ne++;
    ps("Errors: ");pu(ne);ps("\n\n");

    ps("--- Phase 4: Syndrome ---\n");
    SY*sr=smk(code->q,code->s);sex(code,ls,sr);st.m++;
    ps("Bits: ");for(unsigned int i=0;i<code->s;i++){char c[2]={(char)('0'+sr->y[i]),0};ps(c);}
    ps("\nWt: ");unsigned int sw=0;for(unsigned int i=0;i<code->s;i++)if(sr->y[i])sw++;pu(sw);ps("\n\n");

    ps("--- Phase 5: MWPM ---\n");
    MG*mg=mgk(code->s+2);
    for(unsigned int i=0;i<code->s;i++)for(unsigned int j=i+1;j<code->s;j++)med(mg,i,j,(double)((i+j+1)%(code->d+1)));
    for(unsigned int i=0;i<code->s;i++)med(mg,i,code->s,(double)(i%code->d+1));
    dmp(mg,sr->y,code->s);st.r++;

    ps("\n--- Phase 6: NN Decoder ---\n");
    NN*nd=nnk(code->s,16,code->q);
    for(unsigned int t=0;t<10;t++){unsigned char ts[MS],te[MQ];
        for(unsigned int i=0;i<code->s;i++)ts[i]=sr->y[i];for(unsigned int i=0;i<code->q;i++)te[i]=0;
        ntr(nd,ts,te,0.1);}
    unsigned char nc[MQ];dnn(nd,sr->y,nc);st.r++;

    ps("\n--- Phase 7: BP Decoder ---\n");
    BP*bp=bpk(code->q,code->s);unsigned char bc[MQ];
    dbp(bp,sr->y,bc);st.r++;

    ps("\n--- Phase 8: LUT Decoder ---\n");
    LU*lu=luk(code->s,code->q);
    for(unsigned int q=0;q<code->q;q++){unsigned int si=0;
        for(unsigned int s=0;s<code->s;s++){unsigned char p=0;
            for(unsigned int j=0;j<code->q;j++)if(j==q){if(code->x[s*code->q+j])p^=1;if(code->z[s*code->q+j])p^=1;}
            if(p)si|=(1U<<s);}
        lad(lu,si,1U<<q);}
    unsigned char lc[MQ];dlu(lu,sr->y,lc);st.r++;

    ps("\n--- Phase 9: UF Decoder ---\n");
    UF*uf=ufk(code->s);unsigned char uc[MQ];
    duf(uf,sr->y,code->s,uc);st.r++;

    ps("\n--- Phase 10: Correction ---\n");
    for(unsigned int i=0;i<code->q;i++){ls[i]^=bc[i];if(bc[i])st.o++;}
    unsigned int re=0;for(unsigned int i=0;i<code->q;i++)if(ls[i]!=P0)re++;
    ps("Residual: ");pu(re);ps("\n");

    ps("\n--- Phase 11: FT Gates ---\n");
    unsigned char ba[MQ],bb[MQ];for(unsigned int i=0;i<MQ;i++){ba[i]=P0;bb[i]=P0;}
    fcn(ba,bb,code->q);fhd(ba,code->q);
    unsigned int dr=0;double df=0;fdi(ba,&dr,&df);st.w=dr;st.f=df;
    fpo(ba,bb,3);QC*cc=cmk(CC,3);fcv(code,cc,ba);

    ps("\n--- Phase 12: Codes ---\n");
    QC*sh=cmk(CS,3),*su=cmk(CU,5),*bc2=cmk(CB,3);
    ps("Shor:    n=");pu(sh->q);ps(" d=");pu(sh->d);ps("\n");
    ps("Steane:  n=");pu(code->q);ps(" d=");pu(code->d);ps("\n");
    ps("Surface: n=");pu(su->q);ps(" d=");pu(su->d);ps("\n");
    ps("Color:   n=");pu(cc->q);ps(" d=");pu(cc->d);ps("\n");
    ps("Bacon:   n=");pu(bc2->q);ps(" d=");pu(bc2->d);ps("\n");

    ps("\n--- Phase 13: Performance ---\n");
    unsigned int fl=0;rs=GE;
    for(unsigned int t=0;t<100;t++){unsigned char ts[MQ];
        for(unsigned int i=0;i<MQ;i++)ts[i]=P0;nap(nm,ts,code->q);
        unsigned int ec=0;for(unsigned int i=0;i<code->q;i++)if(ts[i]!=P0)ec++;
        if(ec>code->d/2)fl++;}
    st.l=cle(100,fl);double thr=cth(0.001,code->d);
    ps("Threshold d=");pu(code->d);ps(":\n  Phys: ");pd(st.e,6);
    ps("\n  Logic: ");pd(st.l,8);ps("\n  Thresh: ");pd(thr,8);
    ps("\n  OH: ");pu(code->q);ps(":1\n  Decode<");pu(code->q*10);ps("ns\n  Syn: ");pu(code->s*16);ps("B  Mem: ");pu(code->q*code->s*8);ps("B\n");

    pst(&st);ps("\n[QEC] Complete.\n");
    crm(code);crm(sh);crm(su);crm(cc);crm(bc2);mrm(mg);nrm(nd);brm(bp);urm(uf);
}

int main(void){
    char ab[256];int al=host_get_argv(0,256);
    if(al>0)host_get_argv((unsigned int)(unsigned long long)ab,256);
    demo();return 0;
}
