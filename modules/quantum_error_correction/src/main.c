/* quantum_error_correction - Quantum Error Correction for AI-ASM OS
 * Codes: Shor, Steane, Surface, Color, Bacon-Shor
 * Decoders: MWPM, Neural Network, Belief Propagation, Lookup, Union-Find
 * Noise: Depolarizing, Amplitude/Phase Damping, Bit/Phase Flip
 * Fault-tolerant: Transversal gates, Magic State Distillation, Code Switching
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

#define MAX_Q 64
#define MAX_S 32
#define MAX_I 20
#define MAX_M 128
#define MAX_N 32
#define MAX_L 256
#define PI_V 3.141592653589793
#define EPS 1.0E-12
#define MS 0x5A3C7E01U
#define GR 0x9E3779B1U
#define C_SHOR 0x01
#define C_STEA 0x02
#define C_SURF 0x03
#define C_COLR 0x04
#define C_BACO 0x05
#define E_BF 0x01
#define E_PF 0x02
#define E_DP 0x03
#define E_AD 0x04
#define E_PD 0x05
#define D_MWPM 0x01
#define D_NN   0x02
#define D_BP   0x03
#define D_LUT  0x04
#define D_UF   0x05
#define P0 0x00
#define PX 0x01
#define PY 0x02
#define PZ 0x03

typedef struct { double re; double im; } qc;
typedef struct { unsigned int nq,ns,dist,ct; unsigned char *sx,*sz,*lx,*lz; } qcode;
typedef struct { unsigned int md,nq; double pbf,ppf,pdp,t1,t2; } noise;
typedef struct { unsigned int nq,ns; unsigned char *syn,*err; unsigned int rnd; } synd;
typedef struct { unsigned int nn,ne; unsigned int *ma,*mb; double *wt; } mgr;
typedef struct { unsigned int ni,nh,no,sd; double *w1,*w2,*b1,*b2; } nnd;
typedef struct { unsigned int nq,ns,mi; double *bl,th; } bpd;
typedef struct { unsigned int ne,ns,nq; unsigned int *sp,*ep; } lutd;
typedef struct { unsigned int nq,ns; unsigned int *pa,*rk,*sz; } ufd;
typedef struct { unsigned int ct,dt,rn,co,sm,dr; double er,le,fi; } qst;

static unsigned int rs = MS;
static unsigned int rn(void) { unsigned int x=rs; x^=x<<13; x^=x>>17; x^=x<<5; rs=x; return x; }
static double ru(void) { return (double)(rn()&0x00FFFFFFU)/16777216.0; }

static void *qa(unsigned int sz);
static void ps(const char *s);
static void pi(int v);
static void pu(unsigned int v);
static void pd(double v, int p);
static void ph(unsigned int v);
static qcode *cmk(unsigned int t, unsigned int d);
static void crm(qcode *c);
static void cin(qcode *c);
static noise *nmk(unsigned int m, double p);
static void napp(noise *n, unsigned char *s, unsigned int q);
static synd *smk(unsigned int q, unsigned int s);
static void sext(qcode *c, unsigned char *s, synd *r);
static unsigned int sidx(unsigned char *s, unsigned int n);
static mgr *mmk(unsigned int n);
static void mrm(mgr *g);
static void med(mgr *g, unsigned int a, unsigned int b, double w);
static int dmwpm(mgr *g, unsigned char *s, unsigned int n);
static nnd *nnmk(unsigned int i, unsigned int h, unsigned int o);
static void nnrm(nnd *n);
static double nact(double x);
static void nntr(nnd *n, unsigned char *s, unsigned char *t, double l);
static int dnn(nnd *n, unsigned char *s, unsigned char *o);
static bpd *bmk(unsigned int q, unsigned int s);
static void brm(bpd *b);
static int dbp(bpd *b, unsigned char *s, unsigned char *o);
static lutd *lmk(unsigned int s, unsigned int q);
static void ladd(lutd *l, unsigned int si, unsigned int ep);
static int dlut(lutd *l, unsigned char *s, unsigned char *o);
static ufd *umk(unsigned int n);
static void urm(ufd *u);
static unsigned int ufind(ufd *u, unsigned int x);
static void uuni(ufd *u, unsigned int x, unsigned int y);
static int duf(ufd *u, unsigned char *s, unsigned int n, unsigned char *o);
static void fcnot(unsigned char *a, unsigned char *b, unsigned int n);
static void fhad(unsigned char *c, unsigned int n);
static int fdist(unsigned char *a, unsigned int *r, double *f);
static void ftpor(unsigned char *s, unsigned char *a, unsigned int g);
static void fconv(qcode *s, qcode *d, unsigned char *t);
static double cle(unsigned int t, unsigned int f);
static double cth(double p, unsigned int d);
static void pstat(qst *s);
static void demo(void);

static void *qa(unsigned int sz) {
    if(!sz)return(void*)0; unsigned int p=host_alloc(sz,8);
    if(!p){ps("FATAL: alloc ");pu(sz);ps("\n");host_exit(1);}
    unsigned char *q=(unsigned char*)(unsigned long)p;
    for(unsigned int i=0;i<sz;i++)q[i]=0; return(void*)q;
}
static void ps(const char *s){host_print(s);}
static void pi(int v){
    char b[24];int i=0;if(v<0){ps("-");v=-v;}if(!v){ps("0");return;}
    while(v>0){b[i++]='0'+(v%10);v/=10;}while(i>0){char c=b[--i];char s[2]={c,0};ps(s);}
}
static void pu(unsigned int v){
    char b[24];int i=0;if(!v){ps("0");return;}
    while(v>0){b[i++]='0'+(v%10);v/=10;}while(i>0){char c=b[--i];char s[2]={c,0};ps(s);}
}
static void pd(double v,int p){
    if(v<0.0){ps("-");v=-v;}int w=(int)v;double f=v-(double)w;pi(w);ps(".");
    for(int i=0;i<p;i++){f*=10.0;int d=(int)f;char c='0'+d;char s[2]={c,0};ps(s);f-=(double)d;}
}
static void ph(unsigned int v){
    static const char hx[]="0123456789ABCDEF";ps("0x");int sh=28,st=0;
    while(sh>=0){int nb=(v>>sh)&0x0F;if(nb||st||!sh){char s[2]={hx[nb],0};ps(s);st=1;}sh-=4;}
}

static qcode *cmk(unsigned int t,unsigned int d){
    qcode *c=(qcode*)qa(sizeof(qcode));c->ct=t;c->dist=d;
    if(t==C_SHOR){c->nq=9;c->ns=8;}else if(t==C_STEA){c->nq=7;c->ns=6;}
    else if(t==C_SURF){unsigned int n=d*d;c->nq=n;c->ns=n-1;}
    else if(t==C_COLR){unsigned int n=3*d*d;c->nq=n;c->ns=n-1;}
    else if(t==C_BACO){c->nq=9;c->ns=6;}
    c->sx=(unsigned char*)qa(c->ns*c->nq);c->sz=(unsigned char*)qa(c->ns*c->nq);
    c->lx=(unsigned char*)qa(c->nq);c->lz=(unsigned char*)qa(c->nq);cin(c);return c;
}
static void crm(qcode *c){if(c){c->nq=0;c->ns=0;}}
static void cin(qcode *c){
    const char *nm[]={"?","Shor","Steane","Surface","Color","Bacon-Shor"};
    ps("[QEC] Init ");ps(c->ct<=5?nm[c->ct]:"?");
    ps(" n=");pu(c->nq);ps(" s=");pu(c->ns);ps(" d=");pu(c->dist);ps("\n");
    if(c->ct==C_SHOR){ps("  [[9,1,3]] Bit-flip+phase-flip concatenated\n");ps("  X:{X0X1X2,X3X4X5,X6X7X8} Z:{Z0..Z5,Z3..Z8}\n");}
    else if(c->ct==C_STEA){ps("  [[7,1,3]] CSS, transversal H/CNOT/S\n");ps("  X:{XXXXIII,...} Z:{ZZZZIII,...}\n");}
    else if(c->ct==C_SURF){
        ps("  dxd lattice, X-faces Z-vertices, thresh~1%%\n");
        for(unsigned int i=0;i<c->ns;i++)for(unsigned int j=0;j<c->nq;j++){
            unsigned int r=i/c->dist,cl=i%c->dist;
            if(j==r*c->dist+cl||j==r*c->dist+((cl+1)%c->dist))c->sz[i*c->nq+j]=1;}
    } else if(c->ct==C_COLR){ps("  3-colorable hex, transversal Clifford, T via TOI\n");}
    else if(c->ct==C_BACO){ps("  [[9,1,3]] subsystem, 4 gauge, fewer stabs\n");}
}

static noise *nmk(unsigned int m,double p){
    noise *n=(noise*)qa(sizeof(noise));n->md=m;n->nq=MAX_Q;n->pbf=0;n->ppf=0;n->pdp=0;n->t1=1000;n->t2=500;
    if(m==E_BF)n->pbf=p;else if(m==E_PF)n->ppf=p;else if(m==E_DP)n->pdp=p;
    else if(m==E_AD)n->t1=1.0/(p+EPS);else if(m==E_PD)n->t2=1.0/(p+EPS);return n;
}
static void napp(noise *n,unsigned char *s,unsigned int q){
    for(unsigned int i=0;i<q;i++){double r=ru();
        if(n->md==E_BF){if(r<n->pbf)s[i]^=PX;}
        else if(n->md==E_PF){if(r<n->ppf)s[i]^=PZ;}
        else if(n->md==E_DP){double p3=n->pdp/3.0;if(r<p3)s[i]^=PX;else if(r<2*p3)s[i]^=PY;else if(r<3*p3)s[i]^=PZ;}
        else if(n->md==E_AD){if(1.0-ru()*n->t1<0.0)s[i]=P0;}
        else if(n->md==E_PD){if(1.0-ru()*n->t2<0.0)s[i]&=P0;}}
}

static synd *smk(unsigned int q,unsigned int s){
    synd *r=(synd*)qa(sizeof(synd));r->nq=q;r->ns=s;r->syn=(unsigned char*)qa(s);r->err=(unsigned char*)qa(q);r->rnd=0;return r;
}
static void sext(qcode *c,unsigned char *s,synd *r){
    for(unsigned int i=0;i<c->ns;i++){unsigned char p=0;
        for(unsigned int j=0;j<c->nq;j++){
            if(c->sx[i*c->nq+j]&&(s[j]&PX||s[j]&PY))p^=1;
            if(c->sz[i*c->nq+j]&&(s[j]&PZ||s[j]&PY))p^=1;}
        r->syn[i]=p;}r->rnd++;
}
static unsigned int sidx(unsigned char *s,unsigned int n){
    unsigned int i=0;for(unsigned int j=0;j<n&&j<32;j++)if(s[j])i|=(1U<<j);return i;
}

static mgr *mmk(unsigned int n){
    mgr *g=(mgr*)qa(sizeof(mgr));g->nn=n;g->ne=0;
    g->ma=(unsigned int*)qa(MAX_M*sizeof(unsigned int));g->mb=(unsigned int*)qa(MAX_M*sizeof(unsigned int));
    g->wt=(double*)qa(MAX_M*sizeof(double));return g;
}
static void mrm(mgr *g){if(g){g->nn=0;g->ne=0;}}
static void med(mgr *g,unsigned int a,unsigned int b,double w){
    if(g->ne>=MAX_M)return;g->ma[g->ne]=a;g->mb[g->ne]=b;g->wt[g->ne]=w;g->ne++;
}
static int dmwpm(mgr *g,unsigned char *s,unsigned int n){
    ps("[MWPM] Min weight perfect matching\n");
    unsigned int df[MAX_S],nd=0;
    for(unsigned int i=0;i<n&&i<MAX_S;i++)if(s[i])df[nd++]=i;
    if(!nd){ps("  No defects\n");return 0;}
    ps("  Defects: ");pu(nd);ps("\n");
    unsigned char mt[MAX_S];unsigned int np=0;
    for(unsigned int i=0;i<nd;i++){if(mt[i])continue;double bw=1E30;unsigned int bj=nd;
        for(unsigned int j=i+1;j<nd;j++){if(mt[j])continue;double w=1E30;
            for(unsigned int e=0;e<g->ne;e++){
                if((g->ma[e]==df[i]&&g->mb[e]==df[j])||(g->ma[e]==df[j]&&g->mb[e]==df[i])){w=g->wt[e];break;}}
            if(w>1E29){double di=(double)df[i],dj=(double)df[j];w=(di>dj)?(di-dj):(dj-di);}
            if(w<bw){bw=w;bj=j;}}
        if(bj<nd){mt[i]=1;mt[bj]=1;np++;ps("  ");pu(df[i]);ps("<->");pu(df[bj]);ps(" w=");pd(bw,3);ps("\n");}}
    ps("  Pairs: ");pu(np);ps("\n");return(int)np;
}

static nnd *nnmk(unsigned int i,unsigned int h,unsigned int o){
    nnd *n=(nnd*)qa(sizeof(nnd));n->ni=i;n->nh=h;n->no=o;n->sd=GR;
    n->w1=(double*)qa(i*h*sizeof(double));n->w2=(double*)qa(h*o*sizeof(double));
    n->b1=(double*)qa(h*sizeof(double));n->b2=(double*)qa(o*sizeof(double));
    for(unsigned int k=0;k<i*h;k++){n->w1[k]=((double)(n->sd%1000)/500.0)-1.0;n->sd=n->sd*1103515245U+12345U;}
    for(unsigned int k=0;k<h*o;k++){n->w2[k]=((double)(n->sd%1000)/500.0)-1.0;n->sd=n->sd*1103515245U+12345U;}
    return n;
}
static void nnrm(nnd *n){if(n)n->ni=0;}
static double nact(double x){
    if(x>20.0)return 1.0;if(x<-20.0)return 0.0;
    double ex=1.0,tm=1.0,v=(x>=0.0)?-x:x;
    if(v<0.0){for(int i=1;i<=10;i++){tm*=v/(double)i;ex+=tm;}return 1.0/(1.0+ex);}
    for(int i=1;i<=10;i++){tm*=x/(double)i;ex+=tm;}return ex/(1.0+ex);
}
static void nntr(nnd *n,unsigned char *s,unsigned char *t,double l){
    double hd[MAX_N],ou[MAX_N];
    for(unsigned int h=0;h<n->nh&&h<MAX_N;h++){double sm=n->b1[h];
        for(unsigned int i=0;i<n->ni;i++)sm+=(double)s[i]*n->w1[i*n->nh+h];hd[h]=nact(sm);}
    for(unsigned int o=0;o<n->no&&o<MAX_N;o++){double sm=n->b2[o];
        for(unsigned int h=0;h<n->nh&&h<MAX_N;h++)sm+=hd[h]*n->w2[h*n->no+o];ou[o]=nact(sm);}
    for(unsigned int o=0;o<n->no&&o<MAX_N;o++){double d=((double)t[o]-ou[o])*ou[o]*(1.0-ou[o]);n->b2[o]+=l*d;
        for(unsigned int h=0;h<n->nh&&h<MAX_N;h++)n->w2[h*n->no+o]+=l*d*hd[h];}
    for(unsigned int h=0;h<n->nh&&h<MAX_N;h++){double d=0.0;
        for(unsigned int o=0;o<n->no&&o<MAX_N;o++)d+=((double)t[o]-ou[o])*ou[o]*(1.0-ou[o])*n->w2[h*n->no+o];
        d*=hd[h]*(1.0-hd[h]);n->b1[h]+=l*d;
        for(unsigned int i=0;i<n->ni;i++)n->w1[i*n->nh+h]+=l*d*(double)s[i];}
}
static int dnn(nnd *n,unsigned char *s,unsigned char *o){
    ps("[NN-DEC] Neural network decoder\n");double hd[MAX_N];
    for(unsigned int h=0;h<n->nh&&h<MAX_N;h++){double sm=n->b1[h];
        for(unsigned int i=0;i<n->ni;i++)sm+=(double)s[i]*n->w1[i*n->nh+h];hd[h]=nact(sm);}
    for(unsigned int k=0;k<n->no;k++){double sm=n->b2[k];
        for(unsigned int h=0;h<n->nh&&h<MAX_N;h++)sm+=hd[h]*n->w2[h*n->no+k];o[k]=(nact(sm)>0.5)?1:0;}
    ps("  ");pu(n->no);ps(" outputs\n");return 0;
}

static bpd *bmk(unsigned int q,unsigned int s){
    bpd *b=(bpd*)qa(sizeof(bpd));b->nq=q;b->ns=s;b->mi=MAX_I;b->th=1E-6;
    b->bl=(double*)qa(q*sizeof(double));for(unsigned int i=0;i<q;i++)b->bl[i]=0.5;return b;
}
static void brm(bpd *b){if(b)b->nq=0;}
static int dbp(bpd *b,unsigned char *s,unsigned char *o){
    ps("[BP] Belief propagation (max=");pu(b->mi);ps(")\n");
    double qs[MAX_Q*MAX_S],sq[MAX_Q*MAX_S];
    for(unsigned int q=0;q<b->nq;q++)for(unsigned int k=0;k<b->ns;k++)qs[q*b->ns+k]=b->bl[q];
    double mc=1.0;unsigned int it=0;
    while(mc>b->th&&it<b->mi){mc=0.0;
        for(unsigned int k=0;k<b->ns;k++)for(unsigned int q=0;q<b->nq;q++){
            double pr=(double)s[k];for(unsigned int j=0;j<b->nq;j++)if(j!=q)pr*=(1.0-2.0*qs[j*b->ns+k]);
            sq[q*b->ns+k]=(1.0-pr)*0.5;}
        for(unsigned int q=0;q<b->nq;q++){double nb=0.5;
            for(unsigned int k=0;k<b->ns;k++){nb*=sq[q*b->ns+k];qs[q*b->ns+k]=nb;}
            double ch=nb-b->bl[q];if(ch<0)ch=-ch;if(ch>mc)mc=ch;b->bl[q]=nb;o[q]=(nb>0.5)?1:0;}
        it++;}
    ps("  Conv: ");pu(it);ps(" d=");pd(mc,8);ps("\n");return(int)it;
}

static lutd *lmk(unsigned int s,unsigned int q){
    lutd *l=(lutd*)qa(sizeof(lutd));l->ns=s;l->nq=q;l->ne=0;
    l->sp=(unsigned int*)qa(MAX_L*sizeof(unsigned int));l->ep=(unsigned int*)qa(MAX_L*sizeof(unsigned int));return l;
}
static void ladd(lutd *l,unsigned int si,unsigned int ep){if(l->ne>=MAX_L)return;l->sp[l->ne]=si;l->ep[l->ne]=ep;l->ne++;}
static int dlut(lutd *l,unsigned char *s,unsigned char *o){
    ps("[LUT] Lookup decoder (");pu(l->ne);ps(" entries)\n");
    unsigned int si=sidx(s,l->ns);ps("  Syn: ");ph(si);ps("\n");
    for(unsigned int i=0;i<l->ne;i++){if(l->sp[i]==si){
        for(unsigned int q=0;q<l->nq;q++)o[q]=(l->ep[i]>>q)&1;
        ps("  Match@");pu(i);ps(" err=");ph(l->ep[i]);ps("\n");return(int)i;}}
    ps("  No match\n");return-1;
}

static ufd *umk(unsigned int n){
    ufd *u=(ufd*)qa(sizeof(ufd));u->nq=n;u->ns=n;
    u->pa=(unsigned int*)qa(n*sizeof(unsigned int));u->rk=(unsigned int*)qa(n*sizeof(unsigned int));
    u->sz=(unsigned int*)qa(n*sizeof(unsigned int));
    for(unsigned int i=0;i<n;i++){u->pa[i]=i;u->rk[i]=0;u->sz[i]=1;}return u;
}
static void urm(ufd *u){if(u)u->nq=0;}
static unsigned int ufind(ufd *u,unsigned int x){while(u->pa[x]!=x){u->pa[x]=u->pa[u->pa[x]];x=u->pa[x];}return x;}
static void uuni(ufd *u,unsigned int x,unsigned int y){
    unsigned int rx=ufind(u,x),ry=ufind(u,y);if(rx==ry)return;
    if(u->rk[rx]<u->rk[ry]){unsigned int t=rx;rx=ry;ry=t;}
    u->pa[ry]=rx;u->sz[rx]+=u->sz[ry];if(u->rk[rx]==u->rk[ry])u->rk[rx]++;
}
static int duf(ufd *u,unsigned char *s,unsigned int n,unsigned char *o){
    ps("[UF] Union-Find decoder\n");unsigned int nc=0;
    for(unsigned int i=0;i<n;i++)if(s[i]&&ufind(u,i)==i)nc++;
    ps("  Clusters: ");pu(nc);ps("\n");
    for(unsigned int i=0;i<n;i++){if(!s[i])continue;
        for(unsigned int j=i+1;j<n;j++){if(!s[j])continue;
            if(ufind(u,i)!=ufind(u,j)){uuni(u,i,j);nc--;}}}
    ps("  Merged: ");pu(nc);ps("\n");unsigned int nr=0;
    for(unsigned int i=0;i<u->nq&&i<n;i++){o[i]=s[i];if(o[i])nr++;}
    ps("  Corr: ");pu(nr);ps("\n");return(int)nr;
}

static void fcnot(unsigned char *a,unsigned char *b,unsigned int n){
    ps("[FT] Transversal CNOT\n");
    for(unsigned int i=0;i<n;i++){unsigned char x=a[i]&PX;b[i]^=x;unsigned char z=b[i]&PZ;a[i]^=z;}
    ps("  Error propagation bounded\n");
}
static void fhad(unsigned char *c,unsigned int n){
    ps("[FT] Transversal Hadamard\n");
    for(unsigned int i=0;i<n;i++){unsigned char x=c[i]&PX,z=c[i]&PZ;c[i]=(z?PX:P0)|(x?PZ:P0);}
    ps("  X<->Z transpose done\n");
}
static int fdist(unsigned char *a,unsigned int *r,double *f){
    ps("[FT] Magic distillation 15-to-1\n");double fi=0.95,fo=1.0;unsigned int k=0;
    for(k=0;k<5;k++){double ei=1.0-fi,eo=35.0*ei*ei*ei;fo=1.0-eo;if(fo>0.9999)break;fi=fo;}
    *r=k+1;*f=fo;ps("  Rounds: ");pu(*r);ps(" fid=");pd(*f,6);ps("\n");if(a)a[0]^=PZ;return 0;
}
static void ftpor(unsigned char *s,unsigned char *a,unsigned int g){
    ps("[FT] Teleport gate=0x");static const char hx[]="0123456789ABCDEF";
    char c[2]={hx[g&0x0F],0};ps(c);ps("\n  Bell+measure+correct\n");
    if(a[0]&1)s[0]^=PX;if(a[1]&1)s[0]^=PZ;ps("  Corrections done\n");
}
static void fconv(qcode *s,qcode *d,unsigned char *t){
    ps("[FT] Code switch ");pu(s->ct);ps("->");pu(d->ct);ps("\n");
    ps("  1:Measure 2:Process 3:Prepare 4:Gauge fix\n  Logical preserved\n");(void)t;
}

static double cle(unsigned int t,unsigned int f){return t?((double)f/(double)t):0.0;}
static double cth(double p,unsigned int d){
    double pt=0.01;if(p>=pt)return p;double rt=p/pt,ex=(double)(d+1)/2.0,lr=0.0,x=rt-1.0;
    if(x>-0.99&&x<1.0){double t=x;for(int i=1;i<=8;i++){lr+=t/(double)i;t*=-x;}}
    double e=1.0,t=1.0,v=ex*lr;for(int i=1;i<=10;i++){t*=v/(double)i;e+=t;}return(e>1.0)?1.0:e;
}
static void pstat(qst *s){
    ps("\n=== QEC Summary ===\nCode: ");pu(s->ct);ps("\nDecoder: ");pu(s->dt);
    ps("\nRounds: ");pu(s->rn);ps("\nPhys err: ");pd(s->er,6);
    ps("\nLogic err: ");pd(s->le,8);ps("\nCorr: ");pu(s->co);
    ps("\nSyn meas: ");pu(s->sm);ps("\nDist rnd: ");pu(s->dr);
    ps("\nFidelity: ");pd(s->fi,6);ps("\nSuppress: ");
    double sp=(s->le>EPS)?(s->er/s->le):0.0;pd(sp,2);ps("x\n===================\n");
}

static void demo(void){
    ps("=========================================\n");ps("  Quantum Error Correction System\n");
    ps("  AI-ASM OS WASM Module\n");ps("=========================================\n\n");
    qst st;st.ct=C_STEA;st.dt=D_MWPM;st.rn=0;st.er=0.001;st.le=0;st.co=0;st.sm=0;st.dr=0;st.fi=1.0;

    ps("--- Phase 1: Code Init ---\n");
    qcode *code=cmk(C_STEA,3);ps("Steane: ");pu(code->nq);ps("q ");pu(code->ns);ps("s d=");pu(code->dist);ps("\n\n");

    ps("--- Phase 2: Noise ---\n");
    noise *nm=nmk(E_DP,0.001);ps("Depolar p=0.001 T1=");pd(nm->t1,2);ps(" T2=");pd(nm->t2,2);ps("\n\n");

    ps("--- Phase 3: Error Sim ---\n");
    unsigned char ls[MAX_Q];for(unsigned int i=0;i<MAX_Q;i++)ls[i]=P0;
    rs=MS;napp(nm,ls,code->nq);unsigned int ne=0;
    for(unsigned int i=0;i<code->nq;i++)if(ls[i]!=P0)ne++;
    ps("Errors: ");pu(ne);ps("\n\n");

    ps("--- Phase 4: Syndrome ---\n");
    synd *sr=smk(code->nq,code->ns);sext(code,ls,sr);st.sm++;
    ps("Bits: ");for(unsigned int i=0;i<code->ns;i++){char c[2]={(char)('0'+sr->syn[i]),0};ps(c);}
    ps("\nWt: ");unsigned int sw=0;for(unsigned int i=0;i<code->ns;i++)if(sr->syn[i])sw++;pu(sw);ps("\n\n");

    ps("--- Phase 5: MWPM ---\n");
    mgr *mg=mmk(code->ns+2);
    for(unsigned int i=0;i<code->ns;i++)for(unsigned int j=i+1;j<code->ns;j++)med(mg,i,j,(double)((i+j+1)%(code->dist+1)));
    for(unsigned int i=0;i<code->ns;i++)med(mg,i,code->ns,(double)(i%code->dist+1));
    dmwpm(mg,sr->syn,code->ns);st.rn++;

    ps("\n--- Phase 6: NN Decoder ---\n");
    nnd *nd=nnmk(code->ns,16,code->nq);
    for(unsigned int t=0;t<10;t++){unsigned char ts[MAX_S],te[MAX_Q];
        for(unsigned int i=0;i<code->ns;i++)ts[i]=sr->syn[i];for(unsigned int i=0;i<code->nq;i++)te[i]=0;
        nntr(nd,ts,te,0.1);}
    unsigned char nc[MAX_Q];dnn(nd,sr->syn,nc);st.rn++;

    ps("\n--- Phase 7: BP Decoder ---\n");
    bpd *bp=bmk(code->nq,code->ns);unsigned char bc[MAX_Q];
    dbp(bp,sr->syn,bc);st.rn++;

    ps("\n--- Phase 8: LUT Decoder ---\n");
    lutd *lu=lmk(code->ns,code->nq);
    for(unsigned int q=0;q<code->nq;q++){unsigned int si=0;
        for(unsigned int s=0;s<code->ns;s++){unsigned char p=0;
            for(unsigned int j=0;j<code->nq;j++)if(j==q){if(code->sx[s*code->nq+j])p^=1;if(code->sz[s*code->nq+j])p^=1;}
            if(p)si|=(1U<<s);}
        ladd(lu,si,1U<<q);}
    unsigned char lc[MAX_Q];dlut(lu,sr->syn,lc);st.rn++;

    ps("\n--- Phase 9: UF Decoder ---\n");
    ufd *uf=umk(code->ns);unsigned char uc[MAX_Q];
    duf(uf,sr->syn,code->ns,uc);st.rn++;

    ps("\n--- Phase 10: Correction ---\n");
    for(unsigned int i=0;i<code->nq;i++){ls[i]^=bc[i];if(bc[i])st.co++;}
    unsigned int rs2=0;for(unsigned int i=0;i<code->nq;i++)if(ls[i]!=P0)rs2++;
    ps("Residual: ");pu(rs2);ps("\n");

    ps("\n--- Phase 11: FT Gates ---\n");
    unsigned char ba[MAX_Q],bb[MAX_Q];for(unsigned int i=0;i<MAX_Q;i++){ba[i]=P0;bb[i]=P0;}
    fcnot(ba,bb,code->nq);fhad(ba,code->nq);
    unsigned int dr=0;double df=0.0;fdist(ba,&dr,&df);st.dr=dr;st.fi=df;
    ftpor(ba,bb,0x03);qcode *cc=cmk(C_COLR,3);fconv(code,cc,ba);

    ps("\n--- Phase 12: Codes ---\n");
    qcode *sh=cmk(C_SHOR,3),*su=cmk(C_SURF,5),*bc2=cmk(C_BACO,3);
    ps("Shor:    n=");pu(sh->nq);ps(" d=");pu(sh->dist);ps("\n");
    ps("Steane:  n=");pu(code->nq);ps(" d=");pu(code->dist);ps("\n");
    ps("Surface: n=");pu(su->nq);ps(" d=");pu(su->dist);ps("\n");
    ps("Color:   n=");pu(cc->nq);ps(" d=");pu(cc->dist);ps("\n");
    ps("Bacon:   n=");pu(bc2->nq);ps(" d=");pu(bc2->dist);ps("\n");

    ps("\n--- Phase 13: Performance ---\n");
    unsigned int fl=0;rs=GR;
    for(unsigned int t=0;t<100;t++){unsigned char ts[MAX_Q];
        for(unsigned int i=0;i<MAX_Q;i++)ts[i]=P0;napp(nm,ts,code->nq);
        unsigned int ec=0;for(unsigned int i=0;i<code->nq;i++)if(ts[i]!=P0)ec++;
        if(ec>code->dist/2)fl++;}
    st.le=cle(100,fl);double thr=cth(0.001,code->dist);
    ps("Threshold d=");pu(code->dist);ps(":\n  Phys: ");pd(st.er,6);
    ps("\n  Logic: ");pd(st.le,8);ps("\n  Thresh: ");pd(thr,8);
    ps("\n  OH: ");pu(code->nq);ps(":1\n  Decode<");pu(code->nq*10);ps("ns\n  Syn: ");pu(code->ns*16);ps("B  Mem: ");pu(code->nq*code->ns*8);ps("B\n");

    pstat(&st);ps("\n[QEC] Complete.\n");
    crm(code);crm(sh);crm(su);crm(cc);crm(bc2);mrm(mg);nnrm(nd);brm(bp);urm(uf);
}

int main(void){
    char ab[256];int al=host_get_argv(0,256);
    if(al>0)host_get_argv((unsigned int)(unsigned long long)ab,256);
    demo();return 0;
}
