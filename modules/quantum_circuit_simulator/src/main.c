/* quantum_circuit_simulator: Quantum circuit simulation engine (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* Limits */
#define MAX_QB 5
#define MAX_DIM 32
#define MAX_GT 32
#define MAX_CIR 4
#define MAX_SH 256
#define DM_DIM 8
#define NAME_LEN 16
#define PI 3.14159265358979
#define EPS 1.0E-9
/* Gate types */
#define G_ID 0x00
#define G_X  0x01
#define G_Y  0x02
#define G_Z  0x03
#define G_H  0x04
#define G_S  0x05
#define G_TG 0x06
#define G_RX 0x07
#define G_RY 0x08
#define G_RZ 0x09
#define G_P  0x0A
#define G_CX 0x10
#define G_CZ 0x11
#define G_SW 0x12
#define G_TOF 0x20
#define G_FR  0x21
/* Noise types */
#define N_DEP  0x01
#define N_AMP  0x02
#define N_PH   0x03
#define N_TH   0x04
/* States */
#define CS_EMPTY 0x00
#define CS_BUILT 0x01
#define CS_RUN   0x02
/* ======================== DATA STRUCTURES ======================== */
typedef struct { double re; double im; } cx_t;
typedef struct {
    int nq, dim, measured, cmask;
    cx_t amp[MAX_DIM];
} sv_t;
typedef struct {
    int type, tgt, ctrl, ctrl2;
    double theta, phi, lam;
} gate_t;
typedef struct {
    char name[NAME_LEN];
    int nq, dim, gcnt, nmeas, state, shots;
    gate_t gates[MAX_GT];
    int mq[MAX_QB];
    unsigned int hist[MAX_DIM];
} circ_t;
typedef struct { int nq, dim; cx_t mat[DM_DIM*DM_DIM]; } dm_t;
typedef struct { double x, y, z, pur; } bloch_t;
typedef struct { int np; double params[16], grad[16], energy; int iter; } var_t;
typedef struct { int type, qubit; double p, temp, t1, t2; } noise_t;
/* Global state */
static circ_t g_cir[MAX_CIR];
static int g_cc = 0;
static unsigned int g_rng = 0xDEADBEEF;
/* ======================== HELPERS ======================== */
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[24]; int i=0, neg=0; unsigned int u;
    if(v<0){neg=1; u=(unsigned int)(-(v+1))+1U;} else {u=(unsigned int)v;}
    if(u==0){buf[i++]='0';}
    while(u>0){buf[i++]='0'+(char)(u%10); u/=10;}
    if(neg) buf[i++]='-';
    while(i>0){char c=buf[--i]; host_print((char[]){c,0});}
}
static void print_fx(double v, int pr) {
    char buf[32]; int i=0; if(v<0.0){v=-v;}
    int iv=(int)v; double fr=v-(double)iv; char tmp[16]; int ti=0;
    if(iv==0) tmp[ti++]='0';
    while(iv>0){tmp[ti++]='0'+(iv%10); iv/=10;}
    while(ti>0) buf[i++]=tmp[--ti];
    if(pr>0){
        buf[i++]='.';
        for(int p=0;p<pr;p++){fr*=10.0; int d=(int)fr; buf[i++]='0'+d; fr-=(double)d;}
    }
    buf[i]='\0'; host_print(buf);
}
static unsigned int rng(void) {
    g_rng ^= g_rng<<13; g_rng ^= g_rng>>17; g_rng ^= g_rng<<5; return g_rng;
}
static double rng_d(void) { return (double)(rng()&0xFFFFFF)/16777216.0; }
/* ======================== COMPLEX MATH ======================== */
static cx_t cx(double r, double i) { cx_t c; c.re=r; c.im=i; return c; }
static cx_t cx_add(cx_t a, cx_t b) { return cx(a.re+b.re, a.im+b.im); }
static cx_t cx_mul(cx_t a, cx_t b) { return cx(a.re*b.re-a.im*b.im, a.re*b.im+a.im*b.re); }
static cx_t cx_sc(cx_t a, double s) { return cx(a.re*s, a.im*s); }
static double cx_n2(cx_t a) { return a.re*a.re+a.im*a.im; }
static cx_t cx_cj(cx_t a) { return cx(a.re, -a.im); }
static cx_t cx_ex(double ang) {
    double c=1.0, s=0.0, tc=1.0, ts=ang, a2=ang*ang;
    for(int k=1;k<=10;k++){
        tc*=-a2/((2.0*k-1.0)*(2.0*k)); c+=tc;
        ts*=-a2/((2.0*k)*(2.0*k+1.0)); s+=ts;
    }
    return cx(c, s);
}
static double cx_ab(cx_t a) {
    double n=cx_n2(a), x=n; if(n<EPS) return 0.0;
    for(int i=0;i<20;i++){double p=x; x=0.5*(x+n/x); if(x==p)break;}
    return x;
}
/* ======================== STATE VECTOR ======================== */
static sv_t sv_new(int nq) {
    sv_t s; s.nq=nq; s.dim=1<<nq; s.measured=0; s.cmask=0;
    for(int i=0;i<s.dim;i++) s.amp[i]=cx(0.0,0.0);
    s.amp[0]=cx(1.0,0.0); return s;
}
static sv_t sv_1q(sv_t s, cx_t u[2][2], int q) {
    sv_t o=s; int st=1<<q;
    for(int i=0;i<s.dim;i++){
        if(i&st) continue; int j=i|st;
        cx_t a=s.amp[i], b=s.amp[j];
        o.amp[i]=cx_add(cx_mul(u[0][0],a),cx_mul(u[0][1],b));
        o.amp[j]=cx_add(cx_mul(u[1][0],a),cx_mul(u[1][1],b));
    }
    return o;
}
static sv_t sv_cx(sv_t s, cx_t u[2][2], int c, int t) {
    sv_t o=s; int ts=1<<t, cb=1<<c;
    for(int i=0;i<s.dim;i++){
        if((i&cb)==0||(i&ts)) continue; int j=i|ts;
        cx_t a=s.amp[i], b=s.amp[j];
        o.amp[i]=cx_add(cx_mul(u[0][0],a),cx_mul(u[0][1],b));
        o.amp[j]=cx_add(cx_mul(u[1][0],a),cx_mul(u[1][1],b));
    }
    return o;
}
static double sv_p0(sv_t s, int q) {
    double p=0.0; int b=1<<q;
    for(int i=0;i<s.dim;i++) if(!(i&b)) p+=cx_n2(s.amp[i]); return p;
}
static sv_t sv_col(sv_t s, int q, int out) {
    sv_t r=s; int b=1<<q; double nsq=0.0;
    for(int i=0;i<s.dim;i++){
        if(((i&b)?1:0)!=out) r.amp[i]=cx(0.0,0.0);
        else nsq+=cx_n2(s.amp[i]);
    }
    double inv=1.0/cx_ab(cx(nsq,0.0));
    if(inv>EPS) for(int i=0;i<s.dim;i++) r.amp[i]=cx_sc(r.amp[i],inv);
    r.measured=1; r.cmask|=b; return r;
}
/* ======================== GATE MATRICES ======================== */
static void g_h(cx_t u[2][2]) {
    double s=0.7071067811865;
    u[0][0]=cx(s,0.0); u[0][1]=cx(s,0.0); u[1][0]=cx(s,0.0); u[1][1]=cx(-s,0.0);
}
static void g_x(cx_t u[2][2]) {
    u[0][0]=cx(0.0,0.0); u[0][1]=cx(1.0,0.0); u[1][0]=cx(1.0,0.0); u[1][1]=cx(0.0,0.0);
}
static void g_y(cx_t u[2][2]) {
    u[0][0]=cx(0.0,0.0); u[0][1]=cx(0.0,-1.0); u[1][0]=cx(0.0,1.0); u[1][1]=cx(0.0,0.0);
}
static void g_z(cx_t u[2][2]) {
    u[0][0]=cx(1.0,0.0); u[0][1]=cx(0.0,0.0); u[1][0]=cx(0.0,0.0); u[1][1]=cx(-1.0,0.0);
}
static void g_s(cx_t u[2][2]) {
    u[0][0]=cx(1.0,0.0); u[0][1]=cx(0.0,0.0); u[1][0]=cx(0.0,0.0); u[1][1]=cx(0.0,1.0);
}
static void g_tg(cx_t u[2][2]) {
    double s=0.7071067811865;
    u[0][0]=cx(1.0,0.0); u[0][1]=cx(0.0,0.0); u[1][0]=cx(0.0,0.0); u[1][1]=cx(s,s);
}
static void g_rx(cx_t u[2][2], double th) {
    double c=1.0,si=0.0,tc=1.0,ts=th*0.5,a2=th*0.5*th*0.5;
    for(int k=1;k<=10;k++){tc*=-a2/((2.0*k-1.0)*(2.0*k)); c+=tc;
        ts*=-a2/((2.0*k)*(2.0*k+1.0)); si+=ts;}
    u[0][0]=cx(c,0.0); u[0][1]=cx(0.0,-si); u[1][0]=cx(0.0,-si); u[1][1]=cx(c,0.0);
}
static void g_ry(cx_t u[2][2], double th) {
    double c=1.0,si=0.0,tc=1.0,ts=th*0.5,a2=th*0.5*th*0.5;
    for(int k=1;k<=10;k++){tc*=-a2/((2.0*k-1.0)*(2.0*k)); c+=tc;
        ts*=-a2/((2.0*k)*(2.0*k+1.0)); si+=ts;}
    u[0][0]=cx(c,0.0); u[0][1]=cx(-si,0.0); u[1][0]=cx(si,0.0); u[1][1]=cx(c,0.0);
}
static void g_rz(cx_t u[2][2], double th) {
    u[0][0]=cx_ex(-th*0.5); u[0][1]=cx(0.0,0.0); u[1][0]=cx(0.0,0.0); u[1][1]=cx_ex(th*0.5);
}
/* ======================== APPLY GATES ======================== */
static sv_t apply_g(sv_t s, gate_t gt) {
    cx_t u[2][2];
    switch(gt.type) {
    case G_X:  g_x(u);  return sv_1q(s,u,gt.tgt);
    case G_Y:  g_y(u);  return sv_1q(s,u,gt.tgt);
    case G_Z:  g_z(u);  return sv_1q(s,u,gt.tgt);
    case G_H:  g_h(u);  return sv_1q(s,u,gt.tgt);
    case G_S:  g_s(u);  return sv_1q(s,u,gt.tgt);
    case G_TG: g_tg(u); return sv_1q(s,u,gt.tgt);
    case G_RX: g_rx(u,gt.theta); return sv_1q(s,u,gt.tgt);
    case G_RY: g_ry(u,gt.theta); return sv_1q(s,u,gt.tgt);
    case G_RZ: g_rz(u,gt.theta); return sv_1q(s,u,gt.tgt);
    case G_P: {
        cx_t ep=cx_ex(gt.theta);
        u[0][0]=cx(1.0,0.0); u[0][1]=cx(0.0,0.0);
        u[1][0]=cx(0.0,0.0); u[1][1]=ep;
        return sv_1q(s,u,gt.tgt);
    }
    case G_CX: g_x(u); return sv_cx(s,u,gt.ctrl,gt.tgt);
    case G_CZ: g_z(u); return sv_cx(s,u,gt.ctrl,gt.tgt);
    case G_SW: {
        cx_t xn[2][2]; g_x(xn);
        sv_t s1=sv_cx(s,xn,gt.tgt,gt.ctrl);
        sv_t s2=sv_cx(s1,xn,gt.ctrl,gt.tgt);
        return sv_cx(s2,xn,gt.tgt,gt.ctrl);
    }
    case G_TOF: {
        cx_t hm[2][2],td[2][2],xm[2][2];
        g_h(hm); g_tg(td); g_x(xm);
        cx_t ta[2][2];
        ta[0][0]=cx(1.0,0.0); ta[0][1]=cx(0.0,0.0);
        ta[1][0]=cx(0.0,0.0); ta[1][1]=cx_cj(td[1][1]);
        sv_t r=sv_1q(s,hm,gt.tgt);
        r=sv_cx(r,xm,gt.ctrl2,gt.tgt);
        r=sv_1q(r,ta,gt.tgt); r=sv_cx(r,xm,gt.ctrl,gt.tgt);
        r=sv_1q(r,td,gt.tgt); r=sv_cx(r,xm,gt.ctrl2,gt.tgt);
        r=sv_1q(r,ta,gt.tgt); r=sv_cx(r,xm,gt.ctrl,gt.tgt);
        r=sv_1q(r,td,gt.tgt); r=sv_1q(r,hm,gt.tgt);
        return r;
    }
    case G_FR: {
        cx_t xn[2][2]; g_x(xn);
        sv_t s1=sv_cx(s,xn,gt.tgt+1,gt.tgt);
        gate_t tf; tf.type=G_TOF; tf.ctrl=gt.ctrl;
        tf.ctrl2=gt.tgt; tf.tgt=gt.tgt+1;
        tf.theta=0; tf.phi=0; tf.lam=0;
        sv_t s2=apply_g(s1,tf);
        return sv_cx(s2,xn,gt.tgt+1,gt.tgt);
    }
    default: return s;
    }
}
/* ======================== CIRCUIT OPS ======================== */
static int cir_new(const char *nm, int nq) {
    if(g_cc>=MAX_CIR||nq>MAX_QB) return -1;
    int id=g_cc++; circ_t *c=&g_cir[id];
    c->nq=nq; c->dim=1<<nq; c->gcnt=0; c->nmeas=0;
    c->state=CS_EMPTY; c->shots=0;
    for(int i=0;i<c->dim;i++) c->hist[i]=0;
    for(int i=0;i<NAME_LEN-1&&nm[i];i++) c->name[i]=nm[i];
    c->name[NAME_LEN-1]='\0'; return id;
}
static int cir_add(int ci, int type, int tgt, int c1, int c2, double th) {
    if(ci<0||ci>=g_cc||g_cir[ci].gcnt>=MAX_GT) return -1;
    circ_t *c=&g_cir[ci]; gate_t gt;
    gt.type=type; gt.tgt=tgt; gt.ctrl=c1; gt.ctrl2=c2;
    gt.theta=th; gt.phi=0.0; gt.lam=0.0;
    c->gates[c->gcnt++]=gt; c->state=CS_BUILT; return c->gcnt-1;
}
static sv_t cir_run(int ci) {
    circ_t *c=&g_cir[ci]; sv_t sv=sv_new(c->nq);
    for(int i=0;i<c->gcnt;i++) sv=apply_g(sv,c->gates[i]);
    c->state=CS_RUN; return sv;
}
static void cir_sample(int ci, int ns) {
    circ_t *c=&g_cir[ci]; c->shots=ns;
    for(int i=0;i<c->dim;i++) c->hist[i]=0;
    for(int s=0;s<ns;s++){
        sv_t sv=cir_run(ci); int res=0;
        for(int q=0;q<c->nq;q++){
            double p0=sv_p0(sv,q); int bit=(rng_d()<p0)?0:1;
            if(bit) res|=(1<<q); sv=sv_col(sv,q,bit);
        }
        c->hist[res]++;
    }
}
static int cir_opt(int ci) {
    circ_t *c=&g_cir[ci]; int rm=0;
    for(int i=0;i<c->gcnt-1;i++){
        if(c->gates[i].type==c->gates[i+1].type&&
           c->gates[i].tgt==c->gates[i+1].tgt&&
           (c->gates[i].type==G_X||c->gates[i].type==G_H)){
            for(int j=i;j<c->gcnt-2;j++) c->gates[j]=c->gates[j+2];
            c->gcnt-=2; rm+=2; i--;
        }
    }
    return rm;
}
/* ======================== MEASUREMENT ======================== */
static int meas_q(sv_t *sv, int q) {
    double p0=sv_p0(*sv,q); int out=(rng_d()<p0)?0:1;
    *sv=sv_col(*sv,q,out); return out;
}
/* ======================== DENSITY MATRIX ======================== */
static dm_t dm_from(sv_t sv) {
    dm_t d; d.nq=sv.nq; d.dim=sv.dim;
    for(int i=0;i<d.dim;i++)
        for(int j=0;j<d.dim;j++)
            d.mat[i*d.dim+j]=cx_mul(sv.amp[i],cx_cj(sv.amp[j]));
    return d;
}
static double dm_pur(dm_t d) {
    double p=0.0;
    for(int i=0;i<d.dim*d.dim;i++) p+=cx_n2(d.mat[i]);
    return p;
}
static double dm_tr(dm_t d) {
    double t=0.0;
    for(int i=0;i<d.dim;i++) t+=d.mat[i*d.dim+i].re;
    return t;
}
/* ======================== NOISE CHANNELS ======================== */
static dm_t n_dep(dm_t d, double p, int q) {
    dm_t o=d; int b=1<<q; double f=1.0-p;
    for(int i=0;i<d.dim;i++) for(int j=0;j<d.dim;j++){
        int ib=(i&b)?1:0, jb=(j&b)?1:0;
        if(i!=j) o.mat[i*d.dim+j]=cx_sc(d.mat[i*d.dim+j],f);
        else if(ib!=jb) o.mat[i*d.dim+j]=cx(0.0,0.0);
    }
    double dc=p/(double)d.dim;
    for(int i=0;i<d.dim;i++)
        o.mat[i*d.dim+i]=cx_add(cx_sc(o.mat[i*d.dim+i],f),cx(dc,0.0));
    return o;
}
static dm_t n_adamp(dm_t d, double gm, int q) {
    dm_t o=d; int b=1<<q; double sg=1.0-gm;
    for(int i=0;i<d.dim;i++) for(int j=0;j<d.dim;j++){
        int ib=(i&b)?1:0, jb=(j&b)?1:0;
        double sc=(ib&&jb)?sg:(ib||jb)?sg:1.0;
        o.mat[i*d.dim+j]=cx_sc(d.mat[i*d.dim+j],sc);
    }
    for(int i=0;i<d.dim;i++) if(!(i&b)){int ip=i|b;
        o.mat[i*d.dim+i]=cx_add(o.mat[i*d.dim+i],cx(gm*d.mat[ip*d.dim+ip].re,0.0));}
    return o;
}
static dm_t n_pdamp(dm_t d, double gm, int q) {
    dm_t o=d; int b=1<<q; double f=1.0-gm;
    for(int i=0;i<d.dim;i++) for(int j=0;j<d.dim;j++){
        int ib=(i&b)?1:0, jb=(j&b)?1:0;
        if(ib!=jb) o.mat[i*d.dim+j]=cx_sc(d.mat[i*d.dim+j],f);
    }
    return o;
}
static dm_t n_therm(dm_t d, double t1, double t2, double tmp, int q, double dt) {
    double g1=(t1>EPS)?dt/t1:0.0, g2=(t2>EPS)?dt/t2:0.0;
    double nth=(tmp>EPS)?tmp/(tmp+1.0):0.0;
    return n_pdamp(n_adamp(d,g1*(1.0-nth),q),g2*0.5,q);
}
/* ======================== BLOCH SPHERE ======================== */
static bloch_t bloch(sv_t sv, int q) {
    bloch_t bc; int b=1<<q;
    cx_t r00=cx(0.0,0.0), r11=cx(0.0,0.0), r01=cx(0.0,0.0);
    for(int i=0;i<sv.dim;i++){
        if(i&b) continue; int ip=i|b;
        r00=cx_add(r00,cx_mul(sv.amp[i],cx_cj(sv.amp[i])));
        r11=cx_add(r11,cx_mul(sv.amp[ip],cx_cj(sv.amp[ip])));
        r01=cx_add(r01,cx_mul(sv.amp[i],cx_cj(sv.amp[ip])));
    }
    bc.x=2.0*r01.re; bc.y=2.0*r01.im;
    bc.z=r00.re-r11.re; bc.pur=bc.x*bc.x+bc.y*bc.y+bc.z*bc.z;
    return bc;
}
/* ======================== VARIATIONAL CIRCUIT ======================== */
static var_t var_new(int np) {
    var_t v; v.np=np; v.energy=0.0; v.iter=0;
    for(int i=0;i<np;i++){v.params[i]=rng_d()*2.0*PI; v.grad[i]=0.0;} return v;
}
static sv_t var_run(var_t v, int nq) {
    sv_t s=sv_new(nq); cx_t hm[2][2]; g_h(hm);
    for(int q=0;q<nq;q++) s=sv_1q(s,hm,q);
    int pi=0;
    for(int la=0;la<2;la++){
        for(int q=0;q<nq&&pi<v.np;q++,pi++){
            cx_t u[2][2]; g_rz(u,v.params[pi]); s=sv_1q(s,u,q);}
        for(int q=0;q<nq-1;q++){cx_t xn[2][2]; g_x(xn); s=sv_cx(s,xn,q,q+1);}
        for(int q=0;q<nq&&pi<v.np;q++,pi++){
            cx_t u[2][2]; g_ry(u,v.params[pi]); s=sv_1q(s,u,q);}
    }
    return s;
}
static double var_energy(sv_t s, int nq) {
    double e=0.0; for(int q=0;q<nq;q++) e+=1.0-sv_p0(s,q); return e;
}
static void var_grad(var_t *v, int nq, double lr) {
    double d=0.01;
    for(int i=0;i<v->np;i++){
        double orig=v->params[i];
        v->params[i]=orig+d; double ep=var_energy(var_run(*v,nq),nq);
        v->params[i]=orig-d; double em=var_energy(var_run(*v,nq),nq);
        v->grad[i]=(ep-em)/(2.0*d); v->params[i]=orig-lr*v->grad[i];
    }
    v->iter++;
}
/* ======================== PRINTING ======================== */
static void print_sv(sv_t sv) {
    print_str("  SV("); print_int(sv.nq); print_str("q d="); print_int(sv.dim);
    print_str("):\n");
    for(int i=0;i<sv.dim;i++){
        double m=cx_ab(sv.amp[i]);
        if(m>EPS){
            print_str("    |"); print_int(i); print_str(">=");
            print_fx(sv.amp[i].re,4); print_str("+");
            print_fx(sv.amp[i].im,4); print_str("i p=");
            print_fx(cx_n2(sv.amp[i]),4); print_str("\n");
        }
    }
}
static void print_prob(sv_t sv) {
    print_str("  Probs:");
    for(int i=0;i<sv.dim;i++){double p=cx_n2(sv.amp[i]);
        if(p>EPS){print_str(" |"); print_int(i); print_str(">=");
            print_fx(p*100.0,2); print_str("%");}}
    print_str("\n");
}
static void print_bl(bloch_t bc, int q) {
    print_str("  Q"); print_int(q); print_str(" Bloch: x=");
    print_fx(bc.x,4); print_str(" y="); print_fx(bc.y,4);
    print_str(" z="); print_fx(bc.z,4);
    print_str(" |r|="); print_fx(bc.pur,4); print_str("\n");
}
static void print_hist(circ_t *c) {
    print_str("  Histogram("); print_int(c->shots); print_str(" shots):\n");
    for(int i=0;i<c->dim;i++){if(c->hist[i]>0){
        print_str("    |"); print_int(i); print_str(">: ");
        print_int(c->hist[i]); print_str(" (");
        print_fx((double)c->hist[i]/(double)c->shots*100.0,1);
        print_str("%)\n");
    }}
}
/* ======================== MAIN ======================== */
int main(void) {
    print_str("=== Quantum Circuit Simulator v1.0 ===\n");
    /* Demo 1: Bell state */
    print_str("\n[1] Bell State (EPR Pair):\n");
    int c1=cir_new("bell",2);
    cir_add(c1,G_H,0,-1,-1,0.0); cir_add(c1,G_CX,1,0,-1,0.0);
    sv_t sv1=cir_run(c1); print_sv(sv1); print_prob(sv1);
    print_bl(bloch(sv1,0),0);
    /* Demo 2: GHZ */
    print_str("\n[2] 3-qubit GHZ State:\n");
    int c2=cir_new("ghz3",3);
    cir_add(c2,G_H,0,-1,-1,0.0); cir_add(c2,G_CX,1,0,-1,0.0);
    cir_add(c2,G_CX,2,1,-1,0.0);
    sv_t sv2=cir_run(c2); print_sv(sv2); print_prob(sv2);
    /* Demo 3: Sampling */
    print_str("\n[3] Bell Sampling (512 shots):\n");
    cir_sample(c1,512); print_hist(&g_cir[c1]);
    /* Demo 4: Pauli sequence */
    print_str("\n[4] Pauli X->Y->Z:\n");
    int c4=cir_new("pauli",1);
    cir_add(c4,G_X,0,-1,-1,0.0); cir_add(c4,G_Y,0,-1,-1,0.0); cir_add(c4,G_Z,0,-1,-1,0.0);
    print_sv(cir_run(c4));
    /* Demo 5: Rotation gates */
    print_str("\n[5] Parametric RX/RY/RZ:\n");
    int c5=cir_new("rot",2);
    cir_add(c5,G_RX,0,-1,-1,PI/4.0); cir_add(c5,G_RY,1,-1,-1,PI/3.0);
    cir_add(c5,G_RZ,0,-1,-1,PI/6.0); cir_add(c5,G_CX,1,0,-1,0.0);
    sv_t sv5=cir_run(c5); print_sv(sv5); print_prob(sv5);
    /* Demo 6: S/T gates */
    print_str("\n[6] S/T Phase Gates:\n");
    int c6=cir_new("phase",1);
    cir_add(c6,G_H,0,-1,-1,0.0); cir_add(c6,G_S,0,-1,-1,0.0); cir_add(c6,G_TG,0,-1,-1,0.0);
    print_sv(cir_run(c6));
    /* Demo 7: SWAP */
    print_str("\n[7] SWAP Gate:\n");
    int c7=cir_new("swap",2);
    cir_add(c7,G_X,0,-1,-1,0.0); cir_add(c7,G_SW,1,0,-1,0.0);
    print_sv(cir_run(c7));
    /* Demo 8: Toffoli */
    print_str("\n[8] Toffoli (CCX):\n");
    int c8=cir_new("tof",3);
    cir_add(c8,G_X,0,-1,-1,0.0); cir_add(c8,G_X,1,-1,-1,0.0); cir_add(c8,G_TOF,2,0,1,0.0);
    print_sv(cir_run(c8));
    /* Demo 9: Fredkin */
    print_str("\n[9] Fredkin (CSWAP):\n");
    int c9=cir_new("frk",3);
    cir_add(c9,G_X,0,-1,-1,0.0); cir_add(c9,G_X,1,-1,-1,0.0); cir_add(c9,G_FR,1,0,-1,0.0);
    print_sv(cir_run(c9));
    /* Demo 10: Density matrix + noise */
    print_str("\n[10] Density Matrix & Noise:\n");
    int c10=cir_new("noisy",2);
    cir_add(c10,G_H,0,-1,-1,0.0); cir_add(c10,G_CX,1,0,-1,0.0);
    sv_t sv10=cir_run(c10);
    dm_t dm0=dm_from(sv10);
    print_str("  Pure purity="); print_fx(dm_pur(dm0),4);
    print_str(" trace="); print_fx(dm_tr(dm0),4); print_str("\n");
    dm_t dd=n_dep(dm0,0.3,0);
    print_str("  Depolar(p=0.3) pur="); print_fx(dm_pur(dd),4); print_str("\n");
    dm_t da=n_adamp(dm0,0.2,0);
    print_str("  AmpDamp(g=0.2) pur="); print_fx(dm_pur(da),4); print_str("\n");
    dm_t dp=n_pdamp(dm0,0.4,0);
    print_str("  PhsDamp(g=0.4) pur="); print_fx(dm_pur(dp),4); print_str("\n");
    dm_t dt=n_therm(dm0,50.0,30.0,0.1,0,10.0);
    print_str("  Thermal pur="); print_fx(dm_pur(dt),4); print_str("\n");
    /* Demo 11: Variational circuit */
    print_str("\n[11] Variational Circuit (VQE):\n");
    var_t vr=var_new(8);
    print_str("  Params:");
    for(int i=0;i<vr.np;i++){print_fx(vr.params[i],2); print_str(" ");}
    print_str("\n");
    sv_t svv=var_run(vr,3);
    print_str("  E0="); print_fx(var_energy(svv,3),6); print_str("\n");
    for(int ep=0;ep<5;ep++){
        var_grad(&vr,3,0.3);
        sv_t si=var_run(vr,3);
        print_str("  ep"); print_int(ep+1); print_str(" E=");
        print_fx(var_energy(si,3),6);
        print_str(" g0="); print_fx(vr.grad[0],4); print_str("\n");
    }
    /* Demo 12: Optimization */
    print_str("\n[12] Circuit Optimization:\n");
    int c12=cir_new("opt",2);
    cir_add(c12,G_H,0,-1,-1,0.0);
    cir_add(c12,G_X,0,-1,-1,0.0); cir_add(c12,G_X,0,-1,-1,0.0);
    cir_add(c12,G_H,1,-1,-1,0.0); cir_add(c12,G_CX,1,0,-1,0.0);
    cir_add(c12,G_H,1,-1,-1,0.0); cir_add(c12,G_H,1,-1,-1,0.0);
    print_str("  Before: "); print_int(g_cir[c12].gcnt); print_str(" gates\n");
    int rm=cir_opt(c12);
    print_str("  Removed "); print_int(rm); print_str(" gates\n");
    print_str("  After: "); print_int(g_cir[c12].gcnt); print_str(" gates\n");
    /* Demo 13: Partial measurement */
    print_str("\n[13] Partial Measurement:\n");
    int c13=cir_new("pmeas",3);
    cir_add(c13,G_H,0,-1,-1,0.0); cir_add(c13,G_H,1,-1,-1,0.0);
    cir_add(c13,G_H,2,-1,-1,0.0); cir_add(c13,G_CX,1,0,-1,0.0);
    sv_t sv13=cir_run(c13);
    print_str("  Before:"); print_prob(sv13);
    int m0=meas_q(&sv13,0);
    print_str("  meas(q0)="); print_int(m0); print_str("\n");
    print_str("  After:"); print_prob(sv13);
    /* Summary */
    print_str("\n=== Circuit Summary ===\n");
    for(int i=0;i<g_cc;i++){
        print_str("  ["); print_int(i); print_str("] ");
        print_str(g_cir[i].name);
        print_str(" q="); print_int(g_cir[i].nq);
        print_str(" g="); print_int(g_cir[i].gcnt);
        print_str(" st="); print_int(g_cir[i].state);
        print_str(" sh="); print_int(g_cir[i].shots); print_str("\n");
    }
    print_str("\n=== Quantum Circuit Simulator Complete ===\n");
    return 0;
}
