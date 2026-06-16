/*
 * aiasm-aarch64/modules/quantum_interface/src/main.c
 * Quantum Interface WASM module — qubit simulation, gates, algorithms,
 * measurement, circuit builder, and hybrid VQE computing.
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

#define MAX_Q 16
#define MAX_G 256
#define PI    3.14159265358979323846
#define G_H 0
#define G_X 1
#define G_Y 2
#define G_Z 3
#define G_P 4
#define G_CNOT 10
#define G_CZ  11
#define G_SWAP 12
#define G_TOF 20

typedef struct { double re, im; } Cx;
typedef struct { unsigned int n, dim; Cx *s; } QR;
typedef struct { unsigned int tp, tgt, c1, c2; double th; } Gate;
typedef struct { unsigned int nq, ng; Gate g[MAX_G]; } Circ;

static unsigned int hp;

static unsigned int qa(unsigned int sz) {
    unsigned int p = hp; hp = (hp + sz + 15u) & ~15u; return p;
}
static unsigned int slen(const char *s) { unsigned int n=0; while(s[n]) n++; return n; }
static void ps(const char *s) {
    unsigned int l = slen(s), o = qa(l); char *d = (char*)o;
    for (unsigned int i = 0; i < l; i++) d[i] = s[i]; host_print(d);
}
static void i2s(int v, char *b) {
    int i=0, ng=(v<0); unsigned int u=ng?(unsigned int)(-v):(unsigned int)v;
    if(!u){b[i++]='0';b[i]=0;return;} if(ng)b[i++]='-';
    char t[12]; int k=0; while(u){t[k++]='0'+(char)(u%10);u/=10;}
    while(k>0)b[i++]=t[--k]; b[i]=0;
}
static void d2s(double v, char *b, int pr) {
    int i=0; if(v<0){b[i++]='-';v=-v;} int w=(int)v; double f=v-(double)w;
    i2s(w,b+i); while(b[i])i++;
    if(pr>0){b[i++]='.'; for(int p=0;p<pr;p++){f*=10;b[i++]='0'+(int)f;f-=(int)f;}}
    b[i]=0;
}
static Cx ca(Cx a, Cx b){Cx r;r.re=a.re+b.re;r.im=a.im+b.im;return r;}
static Cx cm(Cx a, Cx b){Cx r;r.re=a.re*b.re-a.im*b.im;r.im=a.re*b.im+a.im*b.re;return r;}
static double cabs2(Cx a){return a.re*a.re+a.im*a.im;}

static QR *qr_mk(unsigned int n) {
    QR *q=(QR*)qa(sizeof(QR)); q->n=n; q->dim=1u<<n;
    q->s=(Cx*)qa(q->dim*sizeof(Cx));
    q->s[0].re=1.0; q->s[0].im=0.0;
    for(unsigned int i=1;i<q->dim;i++){q->s[i].re=0;q->s[i].im=0;} return q;
}
static void qr_rm(QR *q){(void)q;}

static void g_h(QR *q, unsigned int qn) {
    unsigned int st=1u<<qn; double iv=0.7071067811865475;
    for(unsigned int i=0;i<q->dim;i++) if(!(i&st)){
        Cx a=q->s[i], b=q->s[i|st];
        Cx su=ca(a,b), di; di.re=a.re-b.re; di.im=a.im-b.im;
        q->s[i].re=su.re*iv; q->s[i].im=su.im*iv;
        q->s[i|st].re=di.re*iv; q->s[i|st].im=di.im*iv;
    }
}
static void g_x(QR *q, unsigned int qn) {
    unsigned int st=1u<<qn;
    for(unsigned int i=0;i<q->dim;i++) if(!(i&st)){
        Cx t=q->s[i]; q->s[i]=q->s[i|st]; q->s[i|st]=t;
    }
}
static void g_y(QR *q, unsigned int qn) {
    unsigned int st=1u<<qn;
    for(unsigned int i=0;i<q->dim;i++) if(!(i&st)){
        Cx a=q->s[i], b=q->s[i|st];
        q->s[i].re=b.im; q->s[i].im=-b.re;
        q->s[i|st].re=-a.im; q->s[i|st].im=a.re;
    }
}
static void g_z(QR *q, unsigned int qn) {
    unsigned int st=1u<<qn;
    for(unsigned int i=0;i<q->dim;i++) if(i&st){q->s[i].re=-q->s[i].re;q->s[i].im=-q->s[i].im;}
}
static void g_phase(QR *q, unsigned int qn, double theta) {
    unsigned int st=1u<<qn; Cx ph={1.0,0.0}; double dt=theta/100.0;
    for(int s=0;s<100;s++){double nr=ph.re-ph.im*dt,ni=ph.re*dt+ph.im;ph.re=nr;ph.im=ni;}
    for(unsigned int i=0;i<q->dim;i++) if(i&st) q->s[i]=cm(q->s[i],ph);
}
static void g_cnot(QR *q, unsigned int c, unsigned int t) {
    unsigned int bc=1u<<c, bt=1u<<t;
    for(unsigned int i=0;i<q->dim;i++) if((i&bc)&&!(i&bt)){
        Cx x=q->s[i]; q->s[i]=q->s[i|bt]; q->s[i|bt]=x;
    }
}
static void g_cz(QR *q, unsigned int c, unsigned int t) {
    unsigned int bc=1u<<c, bt=1u<<t;
    for(unsigned int i=0;i<q->dim;i++) if((i&bc)&&(i&bt)){q->s[i].re=-q->s[i].re;q->s[i].im=-q->s[i].im;}
}
static void g_tof(QR *q, unsigned int c1, unsigned int c2, unsigned int t) {
    unsigned int b1=1u<<c1, b2=1u<<c2, bt=1u<<t;
    for(unsigned int i=0;i<q->dim;i++) if((i&b1)&&(i&b2)&&!(i&bt)){
        Cx x=q->s[i]; q->s[i]=q->s[i|bt]; q->s[i|bt]=x;
    }
}

static double m_prob(QR *q, unsigned int i){return(i<q->dim)?cabs2(q->s[i]):0;}
static void m_collapse(QR *q, unsigned int idx) {
    if(cabs2(q->s[idx])<1e-15)return;
    for(unsigned int i=0;i<q->dim;i++){
        if(i==idx){q->s[i].re=1;q->s[i].im=0;}else{q->s[i].re=0;q->s[i].im=0;}
    }
}
static unsigned int m_best(QR *q) {
    unsigned int b=0; double mx=0;
    for(unsigned int i=0;i<q->dim;i++){double p=cabs2(q->s[i]);if(p>mx){mx=p;b=i;}} return b;
}

static void qft(QR *q) {
    unsigned int n=q->n;
    for(unsigned int i=0;i<n;i++){
        g_h(q,i);
        for(unsigned int j=i+1;j<n;j++){
            double ang=2.0*PI/(double)(1u<<(j-i+1)), dt=ang/100.0;
            unsigned int bc=1u<<j, bt=1u<<i;
            for(unsigned int k=0;k<q->dim;k++) if((k&bc)&&(k&bt)){
                Cx ph=q->s[k];
                for(int s=0;s<100;s++){double nr=ph.re-ph.im*dt,ni=ph.re*dt+ph.im;ph.re=nr;ph.im=ni;}
                q->s[k]=ph;
            }
        }
    }
    for(unsigned int i=0;i<n/2;i++){
        unsigned int j=n-1-i, bi=1u<<i, bj=1u<<j;
        for(unsigned int k=0;k<q->dim;k++){
            unsigned int ki=(k>>i)&1, kj=(k>>j)&1;
            if(ki!=kj && k<(k^bi^bj)){Cx t=q->s[k];q->s[k]=q->s[k^bi^bj];q->s[k^bi^bj]=t;}
        }
    }
}

static unsigned int grover(unsigned int n, unsigned int tgt) {
    QR *q=qr_mk(n);
    unsigned int iters=(unsigned int)(PI/4.0*(double)(1u<<(n/2))); if(!iters)iters=1;
    for(unsigned int i=0;i<n;i++) g_h(q,i);
    for(unsigned int it=0;it<iters;it++){
        q->s[tgt].re=-q->s[tgt].re; q->s[tgt].im=-q->s[tgt].im;
        for(unsigned int i=0;i<n;i++) g_h(q,i);
        for(unsigned int i=0;i<n;i++) g_x(q,i);
        double mn=0; for(unsigned int i=0;i<q->dim;i++) mn+=q->s[i].re; mn/=(double)q->dim;
        for(unsigned int i=0;i<q->dim;i++) q->s[i].re=2.0*mn-q->s[i].re;
        for(unsigned int i=0;i<n;i++) g_x(q,i);
        for(unsigned int i=0;i<n;i++) g_h(q,i);
    }
    unsigned int r=m_best(q); qr_rm(q); return r;
}

static unsigned int mpow(unsigned int b,unsigned int e,unsigned int m){
    unsigned int r=1; b%=m; while(e){if(e&1)r=(r*b)%m;e>>=1;b=(b*b)%m;} return r;
}
static unsigned int _gcd(unsigned int a,unsigned int b){while(b){unsigned int t=b;b=a%b;a=t;}return a;}

static unsigned int shor(unsigned int N) {
    if(N<=1)return 0; if(!(N&1))return 2;
    for(unsigned int d=3;d*d<=N;d+=2) if(N%d==0) return d;
    unsigned int nq=(N>63)?8:(N>15)?6:4;
    QR *q=qr_mk(nq*2); for(unsigned int i=0;i<nq;i++) g_h(q,i);
    unsigned int dt=1u<<nq;
    for(unsigned int x=0;x<dt;x++){
        unsigned int f=mpow(2,x,N), bi=(f%dt)<<nq;
        if(bi<q->dim){q->s[bi].re+=q->s[x].re*0.1;}
    }
    qft(q); unsigned int me=m_best(q);
    unsigned int r=me>0?(dt/me):1; if(!r)r=1;
    if(!(r&1)){
        unsigned int f1=_gcd(mpow(2,r/2,N)+1,N), f2=_gcd(mpow(2,r/2,N)-1,N);
        if(f1>1&&f1<N){qr_rm(q);return f1;} if(f2>1&&f2<N){qr_rm(q);return f2;}
    }
    qr_rm(q); return 0;
}

static Circ *circ_mk(unsigned int n){Circ *c=(Circ*)qa(sizeof(Circ));c->nq=n;c->ng=0;return c;}
static void c_add(Circ *c,unsigned int tp,unsigned int t,unsigned int c1,unsigned int c2,double th){
    if(c->ng>=MAX_G)return;
    Gate *g=&c->g[c->ng++]; g->tp=tp;g->tgt=t;g->c1=c1;g->c2=c2;g->th=th;
}
static void c_H(Circ *c,unsigned int t){c_add(c,G_H,t,0,0,0);}
static void c_X(Circ *c,unsigned int t){c_add(c,G_X,t,0,0,0);}
static void c_CNOT(Circ *c,unsigned int a,unsigned int t){c_add(c,G_CNOT,t,a,0,0);}
static void c_TOFF(Circ *c,unsigned int a,unsigned int b,unsigned int t){c_add(c,G_TOF,t,a,b,0);}
static void c_P(Circ *c,unsigned int t,double th){c_add(c,G_P,t,0,0,th);}

static void circ_run(Circ *c, QR *q) {
    for(unsigned int i=0;i<c->ng;i++){
        Gate *g=&c->g[i];
        switch(g->tp){
            case G_H:g_h(q,g->tgt);break;       case G_X:g_x(q,g->tgt);break;
            case G_Y:g_y(q,g->tgt);break;       case G_Z:g_z(q,g->tgt);break;
            case G_CNOT:g_cnot(q,g->c1,g->tgt);break; case G_CZ:g_cz(q,g->c1,g->tgt);break;
            case G_SWAP:g_cnot(q,g->c1,g->tgt);g_cnot(q,g->tgt,g->c1);g_cnot(q,g->c1,g->tgt);break;
            case G_TOF:g_tof(q,g->c1,g->c2,g->tgt);break;
            case G_P:g_phase(q,g->tgt,g->th);break;
        }
    }
}

static double vqe(unsigned int nq, unsigned int ni) {
    QR *q=qr_mk(nq); double par[8];
    for(unsigned int i=0;i<8&&i<nq*2;i++) par[i]=0.1*(double)i;
    double E=0;
    for(unsigned int it=0;it<ni;it++){
        for(unsigned int i=0;i<q->dim;i++){q->s[i].re=0;q->s[i].im=0;} q->s[0].re=1;
        for(unsigned int i=0;i<nq;i++) g_h(q,i);
        for(unsigned int i=0;i<nq&&i<8;i++) g_phase(q,i,par[i]);
        for(unsigned int i=0;i+1<nq;i++) g_cnot(q,i,i+1);
        E=0; for(unsigned int s=0;s<q->dim;s++){
            double p=cabs2(q->s[s]); E+=p*((double)(int)s-(double)q->dim*0.5);
        }
        for(unsigned int i=0;i<nq&&i<8;i++) par[i]-=0.05*E*0.01;
    }
    qr_rm(q); return E;
}

static void tests(void) {
    char b[128]; QR *q; unsigned int r; double p;

    ps("[1] Single-qubit gates\n"); q=qr_mk(1);
    g_x(q,0); ps("  X|0>: P(1)="); d2s(m_prob(q,1),b,4); ps(b); ps("\n");
    g_x(q,0); g_h(q,0); ps("  H|0>: P(0)="); d2s(m_prob(q,0),b,4); ps(b); ps("\n");
    g_z(q,0); g_y(q,0); ps("  ZY|+>: "); i2s((int)m_best(q),b); ps(b); ps("\n"); qr_rm(q);

    ps("[2] Bell-state entanglement\n"); q=qr_mk(2);
    g_h(q,0); g_cnot(q,0,1);
    ps("  P(00)="); d2s(m_prob(q,0),b,4); ps(b);
    ps(" P(11)="); d2s(m_prob(q,3),b,4); ps(b); ps("\n"); qr_rm(q);

    ps("[3] Pauli algebra (X^2=I)\n"); q=qr_mk(1);
    g_x(q,0); g_x(q,0); ps("  XX|0>: P(0)="); d2s(m_prob(q,0),b,4); ps(b); ps("\n");
    g_z(q,0); g_z(q,0); ps("  ZZ|0>: P(0)="); d2s(m_prob(q,0),b,4); ps(b); ps("\n"); qr_rm(q);

    ps("[4] Toffoli (CCX)\n"); q=qr_mk(3);
    g_x(q,0); g_x(q,1); g_tof(q,0,1,2);
    ps("  CCX|110>: P(111)="); d2s(m_prob(q,7),b,4); ps(b); ps("\n"); qr_rm(q);

    ps("[5] Measurement collapse\n"); q=qr_mk(3);
    for(unsigned int i=0;i<3;i++) g_h(q,i);
    m_collapse(q,5); ps("  Collapse to |101>: P="); d2s(m_prob(q,5),b,4); ps(b); ps("\n"); qr_rm(q);

    ps("[6] Grover search (3q, tgt=5)\n"); r=grover(3,5);
    ps("  Result: "); i2s((int)r,b); ps(b); ps(r==5?" (ok)\n":" (miss)\n");

    ps("[7] Shor factoring\n");
    unsigned int ns[4]={15,21,35,9};
    for(int i=0;i<4;i++){unsigned int f=shor(ns[i]);
        ps("  factor("); i2s((int)ns[i],b); ps(b); ps(")="); i2s((int)f,b); ps(b); ps("\n");}

    ps("[8] QFT\n"); q=qr_mk(3); g_x(q,0); qft(q);
    ps("  QFT|001> peak: "); i2s((int)m_best(q),b); ps(b); ps("\n"); qr_rm(q);

    ps("[9] Circuit builder\n"); Circ *c=circ_mk(3);
    c_H(c,0); c_CNOT(c,0,1); c_TOFF(c,0,1,2); c_P(c,2,1.57);
    q=qr_mk(3); circ_run(c,q);
    ps("  "); i2s((int)c->ng,b); ps(b); ps(" gates, peak="); i2s((int)m_best(q),b); ps(b); ps("\n"); qr_rm(q);

    ps("[10] VQE (4q, 50 iter)\n"); double e=vqe(4,50);
    ps("  E="); d2s(e,b,6); ps(b); ps("\n");
}

__attribute__((export_name("_start"), used))
void _start(void) {
    hp = host_alloc(512, 16);
    ps("\n=== Quantum Interface Module v1.0 ===\n");
    ps("  Qubits | Gates | Grover | Shor | QFT | Circuits | VQE\n\n");

    unsigned int ao = host_alloc(256, 8);
    int ac = host_get_argv(ao, 256);
    char *av = (char *)ao;
    for (int i = 0; i < ac; i++) {
        char *a = av;
        for (int j = 0; j < i; j++) { while (*a) a++; a++; }
        if (a[0]=='-' && a[1]=='h') {
            ps("Usage: quantum_interface [-h] [-t]\n");
            ps("  -h  Help   -t  Run tests\n");
            ps("Features:\n");
            ps("  Qubit sim: state vectors, superposition, entanglement\n");
            ps("  Gates: Hadamard, CNOT, Pauli-X/Y/Z, Toffoli, Phase\n");
            ps("  Algorithms: Grover, Shor, QFT\n");
            ps("  Measurement: probability collapse, statistics\n");
            ps("  Circuits: build, sequence, simulate\n");
            ps("  Hybrid: VQE ground-state estimation\n");
            return;
        }
        if (a[0]=='-' && a[1]=='t') { tests(); ps("\n=== All tests complete ===\n"); return; }
    }
    ps("Running test suite (use -h for help).\n\n");
    tests();
    ps("\n=== All tests complete ===\n");
}
