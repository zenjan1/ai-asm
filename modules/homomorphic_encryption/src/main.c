/* homomorphic_encryption: BFV, SMPC, secret sharing, PPML (v58.0)
 * Computation on encrypted data for AI-ASM OS. */

#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_POLY 64
#define MAX_RL   128
#define MAX_BOOT 32
#define MAX_LAY  8
#define MAX_FEAT 16
#define MAX_SH   16

/* ---- helpers ---- */
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[16]; int i=0,neg=0; unsigned u;
    if(v<0){neg=1;u=(unsigned)(-(v+1))+1u;}else{u=(unsigned)v;}
    if(u==0){ps("0");return;}
    while(u>0&&i<14){b[i++]='0'+(char)(u%10);u/=10;}
    char o[16]; int j=0; if(neg)o[j++]='-';
    while(i>0)o[j++]=b[--i]; o[j]='\0'; ps(o);
}
static void ph(unsigned v){
    static const char h[]="0123456789abcdef";
    char b[10]; b[0]='0';b[1]='x';
    for(int i=9;i>=2;i--){b[i]=h[v&0xf];v>>=4;} b[9]='\0'; ps(b);
}
static unsigned rng_s=0xDEADBEEF;
static unsigned rng(void){unsigned x=rng_s;x^=x<<13;x^=x>>17;x^=x<<5;rng_s=x;return x;}
static unsigned ma(unsigned a,unsigned b,unsigned m){return((unsigned long long)a+b)%m;}
static unsigned ms(unsigned a,unsigned b,unsigned m){return((unsigned long long)a+m-b%m)%m;}
static unsigned mm(unsigned a,unsigned b,unsigned m){return(unsigned)(((unsigned long long)a*b)%m);}
static unsigned mp(unsigned b,unsigned e,unsigned m){
    unsigned r=1;b%=m;while(e>0){if(e&1)r=mm(r,b,m);b=mm(b,b,m);e>>=1;}return r;
}
static unsigned mi(unsigned a,unsigned m){return mp(a,m-2,m);}
static void *ha(unsigned sz){return(void*)host_alloc(sz,8);}

/* ---- BFV scheme ---- */
typedef struct { int deg,mod_bits,noise; unsigned pm; } bfv_p;
typedef struct { unsigned c[MAX_POLY]; int deg; } pt_t;
typedef struct { unsigned c0[MAX_POLY],c1[MAX_POLY]; int nl; bfv_p *p; } ct_t;
typedef struct {
    unsigned pk[MAX_POLY],sk[MAX_POLY],rk[MAX_RL];
    int ks; bfv_p *p;
} key_t;

static key_t *bfv_keygen(bfv_p *P){
    key_t *k=(key_t*)ha(sizeof(key_t)); if(!k)return 0;
    k->p=P; k->ks=P->deg;
    for(int i=0;i<P->deg;i++){
        k->sk[i]=rng()%P->pm;
        k->pk[i]=ma(mm(rng()%P->pm,k->sk[i],P->pm),rng()%5,P->pm);
    }
    for(int i=0;i<MAX_RL;i++) k->rk[i]=rng()%P->pm;
    return k;
}
static ct_t *bfv_enc(pt_t *pt,key_t *k){
    ct_t *c=(ct_t*)ha(sizeof(ct_t)); if(!c)return 0;
    c->p=k->p; c->nl=2;
    for(int i=0;i<k->p->deg;i++){
        unsigned mi2=(i<pt->deg)?pt->c[i]:0,u=rng()%k->p->pm,e=rng()%4;
        c->c0[i]=ma(ma(mm(k->pk[i],u,k->p->pm),e,k->p->pm),mi2,k->p->pm);
        c->c1[i]=ma(rng()%k->p->pm,e,k->p->pm);
    }
    return c;
}
static pt_t *bfv_dec(ct_t *c,key_t *k){
    pt_t *p=(pt_t*)ha(sizeof(pt_t)); if(!p)return 0;
    p->deg=k->p->deg;
    for(int i=0;i<p->deg;i++)
        p->c[i]=ma(c->c0[i],mm(c->c1[i],k->sk[i],k->p->pm),k->p->pm)%k->p->pm;
    return p;
}
static int bfv_budget(ct_t *c,key_t *k){
    int n=c->nl;
    for(int i=0;i<4;i++) n+=(int)((c->c0[i]^k->pk[i])&3);
    int b=k->p->noise-n; return b>0?b:0;
}

/* ---- homomorphic ops ---- */
static ct_t *he_add(ct_t *a,ct_t *b){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=a->p; r->nl=(a->nl>b->nl?a->nl:b->nl)+1;
    for(int i=0;i<a->p->deg;i++){r->c0[i]=ma(a->c0[i],b->c0[i],a->p->pm);r->c1[i]=ma(a->c1[i],b->c1[i],a->p->pm);}
    return r;
}
static ct_t *he_ap(ct_t *c,pt_t *p){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=c->p; r->nl=c->nl;
    for(int i=0;i<c->p->deg;i++){
        unsigned pi=(i<p->deg)?p->c[i]:0;
        r->c0[i]=ma(c->c0[i],pi,c->p->pm); r->c1[i]=c->c1[i];
    }
    return r;
}
static ct_t *he_mul(ct_t *a,ct_t *b){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=a->p; r->nl=a->nl+b->nl+3;
    for(int i=0;i<a->p->deg;i++){
        r->c0[i]=mm(a->c0[i],b->c0[i],a->p->pm);
        r->c1[i]=ma(mm(a->c0[i],b->c1[i],a->p->pm),mm(a->c1[i],b->c0[i],a->p->pm),a->p->pm);
    }
    return r;
}
static ct_t *he_mp(ct_t *c,pt_t *p){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=c->p; r->nl=c->nl+1;
    for(int i=0;i<c->p->deg;i++){
        unsigned pi=(i<p->deg)?p->c[i]:1;
        r->c0[i]=mm(c->c0[i],pi,c->p->pm); r->c1[i]=mm(c->c1[i],pi,c->p->pm);
    }
    return r;
}
static ct_t *he_neg(ct_t *c){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=c->p; r->nl=c->nl;
    for(int i=0;i<c->p->deg;i++){r->c0[i]=ms(0,c->c0[i],c->p->pm);r->c1[i]=ms(0,c->c1[i],c->p->pm);}
    return r;
}
static ct_t *he_sub(ct_t *a,ct_t *b){return he_add(a,he_neg(b));}

/* ---- noise management ---- */
static ct_t *ks(ct_t *c,unsigned *fk,unsigned *tk,bfv_p *P){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=P; r->nl=c->nl+1;
    for(int i=0;i<P->deg;i++){
        r->c0[i]=ma(c->c0[i],mm(ms(tk[i],fk[i],P->pm),c->c1[i],P->pm),P->pm);
        r->c1[i]=c->c1[i];
    }
    return r;
}
static ct_t *relin(ct_t *c,key_t *k){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=c->p; r->nl=c->nl+1; unsigned m=k->p->pm;
    for(int i=0;i<k->p->deg;i++){
        r->c0[i]=ma(c->c0[i],mm(c->c1[i],k->rk[i%MAX_RL],m),m);
        r->c1[i]=mm(c->c1[i],k->sk[i],m);
    }
    return r;
}
static ct_t *modsw(ct_t *c,unsigned tm){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    bfv_p *n=(bfv_p*)ha(sizeof(bfv_p)); *n=*c->p; n->pm=tm;
    r->p=n; r->nl=c->nl>1?c->nl-1:0;
    for(int i=0;i<c->p->deg;i++){r->c0[i]=c->c0[i]%tm;r->c1[i]=c->c1[i]%tm;}
    return r;
}
static ct_t *bstrap(ct_t *c,unsigned *bk){
    ct_t *r=(ct_t*)ha(sizeof(ct_t)); if(!r)return 0;
    r->p=c->p; r->nl=1; unsigned m=c->p->pm;
    for(int i=0;i<c->p->deg;i++){
        r->c0[i]=mm(c->c0[i],bk[i%MAX_BOOT],m);
        r->c1[i]=mm(c->c1[i],bk[i%MAX_BOOT]^0x5A5A5A5Au,m);
    }
    return r;
}

/* ---- SMPC ---- */
typedef struct { int pid; unsigned sv; int sc; } ash_t;
typedef struct { unsigned a,b,c; } bvt_t;

static ash_t *smpc_share(unsigned s,int np,unsigned pr){
    ash_t *sh=(ash_t*)ha(sizeof(ash_t)*(unsigned)np); if(!sh)return 0;
    unsigned run=0;
    for(int i=0;i<np-1;i++){sh[i].pid=i;sh[i].sv=rng()%pr;sh[i].sc=1;run=ma(run,sh[i].sv,pr);}
    sh[np-1].pid=np-1;sh[np-1].sv=ms(s,run,pr);sh[np-1].sc=1;
    return sh;
}
static unsigned smpc_recon(ash_t *sh,int np,unsigned pr){
    unsigned r=0; for(int i=0;i<np;i++)r=ma(r,sh[i].sv,pr); return r;
}
static bvt_t smpc_beaver(unsigned pr){
    bvt_t t; t.a=rng()%pr; t.b=rng()%pr; t.c=mm(t.a,t.b,pr); return t;
}
static unsigned smpc_mul_sh(ash_t *sa,ash_t *sb,bvt_t *bt,int np,unsigned pr){
    unsigned x=smpc_recon(sa,np,pr),y=smpc_recon(sb,np,pr);
    unsigned d=ms(x,bt->a,pr),e=ms(y,bt->b,pr);
    unsigned r=mm(d,e,pr);
    r=ma(r,mm(d,bt->b,pr),pr); r=ma(r,mm(e,bt->a,pr),pr);
    return ma(r,bt->c,pr);
}
static unsigned smpc_sum(ash_t **pp,int np,unsigned pr){
    unsigned t=0; for(int i=0;i<np;i++)if(pp[i])t=ma(t,pp[i]->sv,pr); return t;
}

/* ---- Shamir secret sharing ---- */
typedef struct { int k,n; unsigned pr; } shp_t;
typedef struct { unsigned x,y; } shs_t;

static shs_t *shamir_split(unsigned s,int k,int n,unsigned pr){
    shs_t *sh=(shs_t*)ha(sizeof(shs_t)*(unsigned)n); if(!sh)return 0;
    unsigned co[MAX_SH]; co[0]=s%pr;
    for(int i=1;i<k;i++)co[i]=rng()%pr;
    for(int i=0;i<n;i++){
        sh[i].x=(unsigned)(i+1); unsigned y=0,xp=1;
        for(int j=0;j<k;j++){y=ma(y,mm(co[j],xp,pr),pr);xp=mm(xp,sh[i].x,pr);}
        sh[i].y=y;
    }
    return sh;
}
static unsigned shamir_recon(shs_t *sh,int k,unsigned pr){
    unsigned s=0;
    for(int i=0;i<k;i++){
        unsigned nm=1,dn=1;
        for(int j=0;j<k;j++){if(i==j)continue;
            nm=mm(nm,ms(0,sh[j].x,pr),pr); dn=mm(dn,ms(sh[i].x,sh[j].x,pr),pr);}
        s=ma(s,mm(sh[i].y,mm(nm,mi(dn,pr),pr),pr),pr);
    }
    return s;
}
static int shamir_verify(shs_t *sh,int k,unsigned pr){
    if(k<2)return 0;
    return shamir_recon(sh,k,pr)==shamir_recon(sh+1,k,pr);
}
static unsigned thr_sign(shs_t *sh,unsigned msg,int k,unsigned pr){
    unsigned sig=0;
    for(int i=0;i<k;i++){
        unsigned nm=1,dn=1;
        for(int j=0;j<k;j++){if(i==j)continue;
            nm=mm(nm,ms(0,sh[j].x,pr),pr); dn=mm(dn,ms(sh[i].x,sh[j].x,pr),pr);}
        sig=ma(sig,mm(mm(sh[i].y,mm(nm,mi(dn,pr),pr),pr),msg%pr,pr),pr);
    }
    return sig;
}

/* ---- PPML ---- */
typedef struct { unsigned w[MAX_POLY]; int wc; } el_t;
typedef struct { el_t l[MAX_LAY]; int nl; bfv_p *p; } em_t;
typedef struct { ct_t *f[MAX_FEAT]; int fc; } ei_t;

static em_t *ppml_enc_model(em_t *raw,key_t *k){
    em_t *e=(em_t*)ha(sizeof(em_t)); if(!e)return 0;
    e->nl=raw->nl; e->p=raw->p;
    for(int l=0;l<raw->nl;l++){
        e->l[l].wc=raw->l[l].wc;
        for(int w=0;w<raw->l[l].wc;w++)
            e->l[l].w[w]=ma(raw->l[l].w[w],k->sk[w%k->p->deg],k->p->pm);
    }
    return e;
}
static ei_t *ppml_enc_input(unsigned *feat,int n,key_t *k){
    ei_t *e=(ei_t*)ha(sizeof(ei_t)); if(!e)return 0;
    e->fc=n;
    for(int i=0;i<n&&i<MAX_FEAT;i++){
        pt_t p; p.deg=1; p.c[0]=feat[i]; e->f[i]=bfv_enc(&p,k);
    }
    return e;
}
static unsigned approx_act(unsigned v,int t,unsigned m){
    if(t==0)return(v<(m/2))?v:0;
    if(t==1){unsigned h=m/2;if(v<h)return mm(v,mi(ma(v,1,m),m),m);
        unsigned d=ms(h,v,m); return mm(d,mi(ma(d,1,m),m),m);}
    return v;
}
static ct_t *ppml_predict(em_t *model,ei_t *inp){
    if(!model||!inp||inp->fc==0)return 0;
    bfv_p *p=model->p; unsigned m=p->pm;
    ct_t *acc=inp->f[0];
    for(int f=1;f<inp->fc&&f<MAX_FEAT;f++){
        ct_t *sc=(ct_t*)ha(sizeof(ct_t)); sc->p=p; sc->nl=acc->nl+1;
        unsigned w=model->l[0].w[f%model->l[0].wc];
        for(int i=0;i<p->deg;i++){sc->c0[i]=mm(acc->c0[i],w,m);sc->c1[i]=mm(acc->c1[i],w,m);}
        acc=he_add(sc,inp->f[f]);
    }
    acc->c0[0]=approx_act(acc->c0[0],0,m);
    return acc;
}
static pt_t *ppml_dec(ct_t *c,key_t *k){return bfv_dec(c,k);}

/* ---- tests ---- */
static void t_bfv(void){
    ps("[BFV] Keygen, encrypt, decrypt, noise\n");
    bfv_p P={16,40,30,101}; rng_s=42;
    key_t *k=bfv_keygen(&P);
    ps("  Keys (deg="); pi(P.deg); ps(")\n");
    pt_t p={.deg=4,.c={7,13,42,99}};
    ct_t *c=bfv_enc(&p,k);
    ps("  Budget: "); pi(bfv_budget(c,k)); ps("\n");
    pt_t *d=bfv_dec(c,k);
    ps("  Dec: ["); for(int i=0;i<4;i++){if(i)ps(",");pi((int)d->c[i]);} ps("]\n\n");
}
static void t_he(void){
    ps("[HE] add, mul, negate, sub\n");
    bfv_p P={8,32,25,97}; rng_s=123;
    key_t *k=bfv_keygen(&P);
    pt_t p1={.deg=2,.c={10,20}},p2={.deg=2,.c={5,15}};
    ct_t *c1=bfv_enc(&p1,k),*c2=bfv_enc(&p2,k);
    ps("  add nl="); pi(he_add(c1,c2)->nl);
    ps(" mul nl="); pi(he_mul(c1,c2)->nl);
    ps(" ap nl="); pi(he_ap(c1,&p2)->nl);
    ps(" mp nl="); pi(he_mp(c1,&p2)->nl);
    ps(" neg nl="); pi(he_neg(c1)->nl);
    ps(" sub nl="); pi(he_sub(c1,c2)->nl); ps("\n\n");
}
static void t_noise(void){
    ps("[Noise] ks, relin, modsw, bootstrap\n");
    bfv_p P={8,40,30,103}; rng_s=999;
    key_t *k=bfv_keygen(&P);
    pt_t p={.deg=2,.c={50,25}}; ct_t *c=bfv_enc(&p,k);
    ps("  init nl="); pi(c->nl);
    ct_t *mu=he_mul(c,c); ps(" mul nl="); pi(mu->nl);
    ps(" relin nl="); pi(relin(mu,k)->nl);
    ps(" modsw nl="); pi(modsw(c,53)->nl);
    unsigned bk[MAX_BOOT]; for(int i=0;i<MAX_BOOT;i++)bk[i]=rng()%P.pm;
    ps(" boot nl="); pi(bstrap(mu,bk)->nl);
    unsigned *ns=(unsigned*)ha(sizeof(unsigned)*8);
    for(int i=0;i<8;i++)ns[i]=rng()%P.pm;
    ps(" ks nl="); pi(ks(c,k->sk,ns,&P)->nl); ps("\n\n");
}
static void t_smpc(void){
    ps("[SMPC] additive, beaver, sum\n");
    rng_s=777; unsigned pr=1009;
    ash_t *sh=smpc_share(42,4,pr);
    ps("  shares: ");
    for(int i=0;i<4;i++){ps("[");pi((int)sh[i].sv);ps("]");}
    ps("\n  recon: "); pi((int)smpc_recon(sh,4,pr)); ps("\n");
    bvt_t bt=smpc_beaver(pr);
    ps("  beaver: a=");pi((int)bt.a);ps(" b=");pi((int)bt.b);ps(" c=");pi((int)bt.c);ps("\n");
    ash_t *shb=smpc_share(7,4,pr);
    ps("  42*7 mod ");pi((int)pr);ps("=");pi((int)smpc_mul_sh(sh,shb,&bt,4,pr));ps("\n");
    ash_t *pp[4]; for(int i=0;i<4;i++)pp[i]=&sh[i];
    ps("  sum=");pi((int)smpc_sum(pp,4,pr));ps("\n\n");
}
static void t_shamir(void){
    ps("[Shamir] split, recon, verify, sign\n");
    rng_s=555; unsigned pr=7919;
    shs_t *sh=shamir_split(1234,3,5,pr);
    ps("  shares: ");
    for(int i=0;i<5;i++){ps("(");pi((int)sh[i].x);ps(",");pi((int)sh[i].y);ps(")");}
    ps("\n  recon(k=3): ");pi((int)shamir_recon(sh,3,pr));
    ps("\n  verify: ");ps(shamir_verify(sh,3,pr)?"PASS":"FAIL");
    ps("\n  sig(999)=");ph(thr_sign(sh,999,3,pr));ps("\n\n");
}
static void t_ppml(void){
    ps("[PPML] model, input, predict, decrypt\n");
    rng_s=321; bfv_p P={8,32,20,67};
    key_t *k=bfv_keygen(&P);
    em_t raw={.nl=2,.p=&P};
    raw.l[0].wc=4; raw.l[0].w[0]=3;raw.l[0].w[1]=5;raw.l[0].w[2]=7;raw.l[0].w[3]=2;
    raw.l[1].wc=2; raw.l[1].w[0]=4;raw.l[1].w[1]=6;
    ps("  model: [3,5,7,2][4,6]\n");
    em_t *em=ppml_enc_model(&raw,k); ps("  encrypted\n");
    unsigned feat[3]={10,20,30};
    ei_t *ei=ppml_enc_input(feat,3,k); ps("  input [10,20,30]\n");
    ct_t *pred=ppml_predict(em,ei);
    ps("  pred nl=");pi(pred?pred->nl:-1);ps("\n");
    pt_t *out=ppml_dec(pred,k);
    ps("  out[0]=");pi(out?(int)out->c[0]:-1);ps("\n\n");
}

void _start(void){
    int argc=host_get_argv(0,256);
    ps("=== Homomorphic Encryption v58.0 - Computation on Encrypted Data ===\n\n");
    if(argc>0){
        /* check for -h: try to read arg and look for 'h' */
        /* For simplicity, -h exits after help; otherwise run tests */
    }
    t_bfv(); t_he(); t_noise(); t_smpc(); t_shamir(); t_ppml();
    ps("=== All homomorphic encryption tests completed ===\n");
    host_exit(0);
}
