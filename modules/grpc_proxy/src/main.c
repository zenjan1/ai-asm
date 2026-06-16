/* grpc_proxy: gRPC proxy/gateway module (v1.0)
 * Service discovery, proxy forwarding, protocol translation,
 * auth integration, reflection service, retry & timeout. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_SVC 32
#define MAX_INST 64
#define MAX_METH 64
#define MAX_STR 32
#define MAX_MD 64
#define MAX_NM 32
#define MAX_HST 48
#define MAX_K 32
#define MAX_V 64
#define MAX_TOK 128
#define MAX_CERT 256
#define MAX_PATH 96
#define MAX_BODY 512
#define BUF_SZ 4096
#define HEAP0 65536
#define DEF_TIMEOUT 5000
#define DEF_RETRIES 3
#define DEF_BACKOFF 100
#define LB_RR 0
#define LB_RAND 1
#define LB_LR 2
#define ST_UNARY 0
#define ST_CLI 1
#define ST_SRV 2
#define ST_BIDI 3
#define S_IDLE 0
#define S_OPEN 1
#define S_HALF 2
#define S_CLOSE 3
#define A_NONE 0
#define A_MTLS 1
#define A_JWT 2
#define A_OAUTH 3
#define G_OK 0
#define G_CANCEL 1
#define G_DEAD 4
#define G_NOTFOUND 5
#define G_UNAVAIL 14

typedef struct { char k[MAX_K]; char v[MAX_V]; int use; } md_ent_t;
typedef struct { md_ent_t e[MAX_MD]; int n; } mdbg_t;
typedef struct { char svc[MAX_NM]; char host[MAX_HST]; int port; int ok; unsigned int act; } inst_t;
typedef struct {
    char name[MAX_NM]; inst_t inst[MAX_INST]; int ic; int lb; unsigned int rr; int use;
} gsvc_t;
typedef struct { char svc[MAX_NM]; char meth[MAX_NM]; int use; } meth_t;
typedef struct {
    int id; int stype; int state; char svc[MAX_NM]; char meth[MAX_NM];
    unsigned int bsent; unsigned int brecv; unsigned int cat; int dl_ms;
    int auth; mdbg_t md; int use;
} strm_t;
typedef struct { char path[MAX_PATH]; char hmeth[MAX_NM]; char body[MAX_BODY]; mdbg_t md; } hreq_t;
typedef struct { char svc[MAX_NM]; char meth[MAX_NM]; char subj[MAX_NM]; char spiffe[MAX_NM]; char pem[MAX_CERT]; int ok; int use; } cert_t;
typedef struct { char subj[MAX_NM]; char issuer[MAX_NM]; char tok[MAX_TOK]; unsigned int exp; int ok; } jwtk_t;
typedef struct {
    int max_att; unsigned int ibo; unsigned int mbo; int mult;
    int codes[8]; int cc;
} rpol_t;
typedef struct { int id; unsigned int dl; unsigned int cat; int att; int mr; int ls; int use; } rctx_t;
typedef struct { int en; unsigned int ival; unsigned int jit; } hcfg_t;

static unsigned int hp = HEAP0, clk = 0;
static gsvc_t svcs[MAX_SVC]; static meth_t meths[MAX_METH]; static strm_t strs[MAX_STR];
static cert_t certs[MAX_INST]; static jwtk_t toks[MAX_INST]; static rctx_t rcs[MAX_STR];
static rpol_t dpol; static hcfg_t dhed;
static int nsvc=0,nmeth=0,nstr=0,ncert=0,ntok=0,nrc=0;
static unsigned int nsid = 1;
static int gw_web = 0;

/* Forward decls */
static void rctx_clean(int i); static void hed_cancel(int i); static void svc_refresh(int i);

/* ── Utilities ── */
static void ps(const char *s) { host_print(s); }
static int slen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int scmp(const char *a, const char *b) {
    while(*a&&*b) { if(*a!=*b) return *a-*b; a++; b++; } return *a-*b;
}
static void scpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1&&s[i]) { d[i]=s[i]; i++; } d[i]='\0';
}
static void mzero(void *p, int n) { char *c=(char*)p; for(int i=0;i<n;i++) c[i]=0; }
static void pi(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;} if(v==0){b[p++]='0';}
    else { int d=0,t=v; while(t>0){d++;t/=10;} p+=d; b[p]='\0'; p--;
           while(v>0){b[p--]='0'+(v%10);v/=10;} }
    host_print(b);
}
static void pu(unsigned int v){pi((int)v);}
static void phdr(const char *t) { ps("== "); ps(t); ps(" ==\n"); }
static unsigned int shash(const char *s) {
    unsigned int h=5381; while(*s){h=((h<<5)+h)+(unsigned int)*s;s++;} return h;
}
static const char *lb_nm(int p) { return p==LB_RR?"ROUND_ROBIN":p==LB_RAND?"RANDOM":p==LB_LR?"LEAST_REQUEST":"?"; }
static const char *st_nm(int t) { return t==ST_UNARY?"UNARY":t==ST_CLI?"CLIENT_STREAM":t==ST_SRV?"SERVER_STREAM":t==ST_BIDI?"BIDI_STREAM":"?"; }
static const char *ss_nm(int s) { return s==S_IDLE?"IDLE":s==S_OPEN?"OPEN":s==S_HALF?"HALF_CLOSED":s==S_CLOSE?"CLOSED":"?"; }
static const char *au_nm(int a) { return a==A_MTLS?"MTLS":a==A_JWT?"JWT":a==A_OAUTH?"OAUTH":"NONE"; }
static const char *gs_nm(int c) { return c==G_OK?"OK":c==G_CANCEL?"CANCELLED":c==G_DEAD?"DEADLINE_EXCEEDED":c==G_NOTFOUND?"NOT_FOUND":c==G_UNAVAIL?"UNAVAILABLE":"?"; }

/* ── Service Discovery ── */
static int svc_reg(const char *nm, const char *h, int p, int lb) {
    if(nsvc>=MAX_SVC) return -1;
    int i=nsvc++; gsvc_t *s=&svcs[i];
    scpy(s->name,nm,MAX_NM); mzero(s->inst,sizeof(s->inst));
    s->ic=0; s->lb=lb; s->rr=0; s->use=1;
    if(s->ic<MAX_INST) { inst_t *q=&s->inst[s->ic++];
        scpy(q->svc,nm,MAX_NM); scpy(q->host,h,MAX_HST);
        q->port=p; q->ok=1; q->act=0; }
    ps("grpc_proxy: registered "); ps(nm); ps(" "); ps(h); ps(":"); pi(p);
    ps(" lb="); ps(lb_nm(lb)); ps("\n"); return i;
}
static int nr_lookup(const char *nm, char *oh, int *op) {
    for(int i=0;i<nsvc;i++) { if(!svcs[i].use) continue;
        if(scmp(svcs[i].name,nm)==0) {
            for(int j=0;j<svcs[i].ic;j++) if(svcs[i].inst[j].ok) {
                scpy(oh,svcs[i].inst[j].host,MAX_HST); *op=svcs[i].inst[j].port; return j; }
            return -1; } } return -1;
}
static int nr_update(const char *nm, const char *h, int p, int ok) {
    for(int i=0;i<nsvc;i++) { if(!svcs[i].use) continue;
        if(scmp(svcs[i].name,nm)!=0) continue;
        for(int j=0;j<svcs[i].ic;j++)
            if(scmp(svcs[i].inst[j].host,h)==0&&svcs[i].inst[j].port==p) { svcs[i].inst[j].ok=ok; return j; }
        if(svcs[i].ic<MAX_INST) { int j=svcs[i].ic++; inst_t *q=&svcs[i].inst[j];
            scpy(q->svc,nm,MAX_NM); scpy(q->host,h,MAX_HST); q->port=p; q->ok=ok; q->act=0; return j; }
        return -1; } return -1;
}
static int lb_sel(const char *nm) {
    for(int i=0;i<nsvc;i++) { if(!svcs[i].use) continue;
        if(scmp(svcs[i].name,nm)!=0) continue;
        gsvc_t *s=&svcs[i]; int c=s->ic; if(c<=0) return -1;
        if(s->lb==LB_RR) { int st=(int)(s->rr%(unsigned)c);
            for(int k=0;k<c;k++){int x=(st+k)%c; if(s->inst[x].ok){s->rr=(unsigned)((x+1)%c);return x;}} return -1; }
        if(s->lb==LB_RAND) { unsigned sd=shash(s->name)^clk;
            for(int k=0;k<c;k++){int x=(int)((sd+(unsigned)k)%(unsigned)c); if(s->inst[x].ok) return x;} return -1; }
        if(s->lb==LB_LR) { int b=-1; unsigned mn=0xFFFFFFFFu;
            for(int k=0;k<c;k++) if(s->inst[k].ok&&s->inst[k].act<mn){mn=s->inst[k].act;b=k;} return b; }
        return -1; } return -1;
}
static int svc_resolve(const char *nm) {
    char h[MAX_HST]; int p=0; int r=nr_lookup(nm,h,&p);
    if(r<0){ps("grpc_proxy: resolve fail ");ps(nm);ps("\n");return -1;}
    ps("grpc_proxy: resolved ");ps(nm);ps(" -> ");ps(h);ps(":");pi(p);ps("\n"); return r;
}
static void svc_list(void) {
    phdr("Registered Services");
    for(int i=0;i<nsvc;i++){if(!svcs[i].use) continue; gsvc_t *s=&svcs[i];
        ps("  [");pi(i);ps("] ");ps(s->name);ps(" lb=");ps(lb_nm(s->lb));
        ps(" inst=");pi(s->ic);ps("\n");
        for(int j=0;j<s->ic;j++){inst_t *q=&s->inst[j];
            ps("    -> ");ps(q->host);ps(":");pi(q->port);
            ps(q->ok?" [ok]":" [bad]");ps(" act=");pu(q->act);ps("\n");}}
}

/* ── Proxy Forwarding ── */
static int strm_alloc(const char *sv,const char *mt,int st,int dl,int au) {
    if(nstr>=MAX_STR) return -1;
    int i=nstr++; strm_t *x=&strs[i];
    x->id=(int)nsid++; x->stype=st; x->state=S_OPEN;
    scpy(x->svc,sv,MAX_NM); scpy(x->meth,mt,MAX_NM);
    x->bsent=0; x->brecv=0; x->cat=clk; x->dl_ms=dl; x->auth=au;
    mzero(&x->md,sizeof(x->md)); x->md.n=0; x->use=1;
    int li=lb_sel(sv);
    if(li>=0) for(int k=0;k<nsvc;k++) if(scmp(svcs[k].name,sv)==0&&li<svcs[k].ic){svcs[k].inst[li].act++;break;}
    return i;
}
static int fwd_unary(const char *sv,const char *mt,const char *pl,int pln,mdbg_t *md,int dl) {
    int i=strm_alloc(sv,mt,ST_UNARY,dl,A_NONE);
    if(i<0){ps("grpc_proxy: unary fail\n");return -1;}
    if(md) strs[i].md=*md; strs[i].bsent=(unsigned)pln;
    strs[i].brecv=(unsigned)(pln>0?pln/2:16); strs[i].state=S_CLOSE;
    ps("grpc_proxy: unary ");ps(sv);ps("/");ps(mt);
    ps(" sent=");pu(strs[i].bsent);ps(" recv=");pu(strs[i].brecv);ps(" dl=");pi(dl);ps("ms\n"); return i;
}
static int fwd_cli(const char *sv,const char *mt,int dl) {
    int i=strm_alloc(sv,mt,ST_CLI,dl,A_NONE); if(i<0) return -1;
    ps("grpc_proxy: client-stream ");ps(sv);ps("/");ps(mt);ps(" [");pi(strs[i].id);ps("]\n"); return i;
}
static int fwd_srv(const char *sv,const char *mt,int dl) {
    int i=strm_alloc(sv,mt,ST_SRV,dl,A_NONE); if(i<0) return -1;
    ps("grpc_proxy: server-stream ");ps(sv);ps("/");ps(mt);ps(" [");pi(strs[i].id);ps("]\n"); return i;
}
static int fwd_bidi(const char *sv,const char *mt,int dl) {
    int i=strm_alloc(sv,mt,ST_BIDI,dl,A_NONE); if(i<0) return -1;
    ps("grpc_proxy: bidi-stream ");ps(sv);ps("/");ps(mt);ps(" [");pi(strs[i].id);ps("]\n"); return i;
}
static int strm_send(int i,int l){if(i<0||i>=nstr||!strs[i].use||strs[i].state==S_CLOSE)return -1;
    strs[i].bsent+=(unsigned)l;ps("  strm[");pi(strs[i].id);ps("] send ");pi(l);ps(" tot=");pu(strs[i].bsent);ps("\n");return 0;}
static int strm_recv(int i,int l){if(i<0||i>=nstr||!strs[i].use||strs[i].state==S_CLOSE)return -1;
    strs[i].brecv+=(unsigned)l;ps("  strm[");pi(strs[i].id);ps("] recv ");pi(l);ps(" tot=");pu(strs[i].brecv);ps("\n");return 0;}
static int strm_close(int i){if(i<0||i>=nstr||!strs[i].use)return -1; strs[i].state=S_CLOSE;
    for(int k=0;k<nsvc;k++) if(scmp(svcs[k].name,strs[i].svc)==0)
        for(int j=0;j<svcs[k].ic;j++) if(svcs[k].inst[j].act>0){svcs[k].inst[j].act--;break;}
    ps("  strm[");pi(strs[i].id);ps("] closed\n"); return 0;}

/* ── Metadata ── */
static int md_add(mdbg_t *m,const char *k,const char *v) {
    if(!m||m->n>=MAX_MD) return -1; int i=m->n++;
    scpy(m->e[i].k,k,MAX_K); scpy(m->e[i].v,v,MAX_V); m->e[i].use=1; return i;
}
static const char *md_get(const mdbg_t *m,const char *k) {
    if(!m) return 0; for(int i=0;i<m->n;i++) if(m->e[i].use&&scmp(m->e[i].k,k)==0) return m->e[i].v; return 0;
}
static int md_prop(mdbg_t *d,const mdbg_t *s) {
    if(!d||!s) return -1; int c=0;
    for(int i=0;i<s->n;i++){if(!s->e[i].use||scmp(s->e[i].k,":authority")==0) continue;
        md_add(d,s->e[i].k,s->e[i].v);c++;} return c;
}
static void md_dump(const char *lb,const mdbg_t *m) {
    ps(lb); if(!m||m->n==0){ps(" (empty)\n");return;} ps(":\n");
    for(int i=0;i<m->n;i++) if(m->e[i].use){ps("  ");ps(m->e[i].k);ps("=");ps(m->e[i].v);ps("\n");}
}

/* ── Protocol Translation ── */
static int tx_g2h(const char *sv,const char *mt,const char *bd,hreq_t *o) {
    if(!o) return -1; int p=0;
    o->path[p++]='/';o->path[p++]='v';o->path[p++]='1';o->path[p++]='/';
    for(int i=0;i<MAX_PATH-8&&sv[i];i++) o->path[p++]=sv[i];
    o->path[p++]='/'; for(int i=0;i<MAX_PATH-8&&mt[i];i++) o->path[p++]=mt[i];
    o->path[p]='\0'; scpy(o->hmeth,"POST",MAX_NM);
    scpy(o->body,bd?bd:"{}",MAX_BODY); mzero(&o->md,sizeof(o->md)); o->md.n=0;
    md_add(&o->md,"content-type","application/json");
    md_add(&o->md,"x-grpc-svc",sv); md_add(&o->md,"x-grpc-meth",mt);
    ps("grpc_proxy: gRPC->HTTP ");ps(o->path);ps(" body=");pi(slen(o->body));ps("\n"); return 0;
}
static int tx_h2g(const hreq_t *r,char *os,char *om) {
    if(!r||!os||!om) return -1; const char *p=r->path;
    while(*p&&*p=='/')p++; while(*p&&*p!='/')p++; while(*p&&*p=='/')p++;
    int i=0; while(*p&&*p!='/'&&i<MAX_NM-1){os[i++]=*p++;} os[i]='\0';
    if(*p=='/')p++; i=0; while(*p&&*p!='/'&&*p!='?'&&i<MAX_NM-1){om[i++]=*p++;} om[i]='\0';
    ps("grpc_proxy: HTTP->gRPC ");ps(os);ps("/");ps(om);ps("\n"); return 0;
}
static void gw_enable(void){gw_web=1;ps("grpc_proxy: gRPC-Web enabled\n");}
static int gw_wrap(const char *fr,int fl,char *ob,int om) {
    if(!ob||om<fl+5) return -1;
    ob[0]=0; ob[1]=(char)((fl>>24)&0xFF); ob[2]=(char)((fl>>16)&0xFF);
    ob[3]=(char)((fl>>8)&0xFF); ob[4]=(char)(fl&0xFF);
    for(int i=0;i<fl&&(i+5)<om;i++) ob[i+5]=fr[i];
    ps("grpc_proxy: grpc-web wrap len=");pi(fl);ps("\n"); return fl+5;
}
static int gw_unwrap(const char *wf,int wl,char *ob,int om,int *ofl) {
    if(!wf||wl<5||!ob) return -1;
    *ofl=(int)(unsigned char)wf[0];
    int pl=((int)(unsigned char)wf[1]<<24)|((int)(unsigned char)wf[2]<<16)|
           ((int)(unsigned char)wf[3]<<8)|((int)(unsigned char)wf[4]);
    if(pl>om||pl+5>wl) return -1;
    for(int i=0;i<pl;i++) ob[i]=wf[i+5];
    ps("grpc_proxy: grpc-web unwrap fl=");pi(*ofl);ps(" pl=");pi(pl);ps("\n"); return pl;
}

/* ── Auth ── */
static int au_mtls(const char *cli,const char *srv) {
    if(ncert>=MAX_INST) return -1; int i=ncert++; cert_t *c=&certs[i];
    scpy(c->svc,srv,MAX_NM); scpy(c->meth,"mtls-hs",MAX_NM);
    scpy(c->subj,srv,MAX_NM); scpy(c->spiffe,"spiffe://cluster/",MAX_NM);
    int p=slen(c->spiffe); for(int j=0;srv[j]&&p<MAX_NM-1;j++) c->spiffe[p++]=srv[j];
    c->spiffe[p]='\0';
    scpy(c->pem,"-----BEGIN CERT-----\nMTLS-CERT\n-----END CERT-----\n",MAX_CERT);
    c->ok=1; c->use=1;
    ps("grpc_proxy: mTLS ");ps(cli);ps("->");ps(srv);ps(" spiffe=");ps(c->spiffe);ps("\n"); return i;
}
static int au_jwt(const char *tk,const char *sv) {
    if(ntok>=MAX_INST) return -1; int i=ntok++; jwtk_t *t=&toks[i];
    scpy(t->subj,sv,MAX_NM); scpy(t->issuer,"gateway",MAX_NM);
    scpy(t->tok,tk,MAX_TOK); t->exp=clk+3600000; t->ok=1;
    ps("grpc_proxy: JWT ");ps(sv);ps(" len=");pi(slen(tk));ps("\n"); return i;
}
static int au_oauth(const char *ci,const char *sc,char *ot,int om) {
    if(!ot||om<16) return -1;
    unsigned int h=shash(ci)^shash(sc)^clk; int p=0;
    const char *px="oauth_"; for(int i=0;px[i]&&p<om-1;i++) ot[p++]=px[i];
    if(h==0){ot[p++]='0';} else { char tmp[16]; int tl=0; unsigned int tv=h;
        while(tv>0&&tl<12){tmp[tl++]='0'+(char)(tv%10);tv/=10;}
        for(int i=tl-1;i>=0&&p<om-1;i--) ot[p++]=tmp[i]; }
    ot[p]='\0'; ps("grpc_proxy: OAuth ");ps(ci);ps(" scope=");ps(sc);
    ps(" tok=");pi(p);ps("\n"); return 0;
}

/* ── Reflection ── */
static int ref_reg(const char *sv,const char *mt) {
    if(nmeth>=MAX_METH) return -1; int i=nmeth++;
    scpy(meths[i].svc,sv,MAX_NM); scpy(meths[i].meth,mt,MAX_NM); meths[i].use=1; return i;
}
static int ref_list_svc(char *ob,int om) {
    int p=0; for(int i=0;i<nsvc&&p<om-MAX_NM-2;i++){if(!svcs[i].use) continue;
        for(int j=0;svcs[i].name[j]&&p<om-2;j++) ob[p++]=svcs[i].name[j]; ob[p++]='\n';}
    ob[p]='\0'; ps("grpc_proxy: reflection ListServices=");pi(nsvc);ps("\n"); return p;
}
static int ref_list_meth(const char *sv,char *ob,int om) {
    int p=0,f=0; for(int i=0;i<nmeth&&p<om-MAX_NM-2;i++){if(!meths[i].use) continue;
        if(scmp(meths[i].svc,sv)!=0) continue;
        for(int j=0;meths[i].meth[j]&&p<om-2;j++) ob[p++]=meths[i].meth[j]; ob[p++]='\n';f++;}
    ob[p]='\0'; ps("grpc_proxy: reflection ListMethods(");ps(sv);ps(")=");pi(f);ps("\n"); return f;
}
static int ref_file(const char *sym,char *ob,int om) {
    if(!ob||om<8) return -1; int p=0; const char *hd="FD:";
    for(int i=0;hd[i]&&p<om-1;i++) ob[p++]=hd[i];
    for(int i=0;sym[i]&&p<om-1;i++) ob[p++]=sym[i];
    ob[p]='\0'; ps("grpc_proxy: reflection File(");ps(sym);ps(")\n"); return p;
}

/* ── Retry & Timeout ── */
static int rp_set(int ma,unsigned int ib,unsigned int mb,int mu) {
    dpol.max_att=ma; dpol.ibo=ib; dpol.mbo=mb; dpol.mult=mu; dpol.cc=0;
    dpol.codes[dpol.cc++]=G_UNAVAIL; dpol.codes[dpol.cc++]=G_DEAD;
    ps("grpc_proxy: retry max=");pi(ma);ps(" ibo=");pu(ib);ps(" mbo=");pu(mb);ps(" x");pi(mu);ps("\n"); return 0;
}
static int rp_should(int sc,int att,const rpol_t *r) {
    if(att>=r->max_att) return 0;
    for(int i=0;i<r->cc;i++) if(r->codes[i]==sc) return 1; return 0;
}
static unsigned int rp_backoff(int att,const rpol_t *r) {
    unsigned int b=r->ibo; for(int i=1;i<att&&b<r->mbo;i++) b*=(unsigned)r->mult;
    if(b>r->mbo) b=r->mbo; unsigned int j=b/5; if(j==0) j=1; return b+(j&0x0F);
}
static int retry_exec(const char *sv,const char *mt,int pln) {
    if(nrc>=MAX_STR) return -1; int i=nrc++; rctx_t *c=&rcs[i];
    c->id=i; c->dl=DEF_TIMEOUT; c->cat=clk; c->att=0; c->mr=dpol.max_att; c->ls=G_OK; c->use=1;
    ps("grpc_proxy: retry ");ps(sv);ps("/");ps(mt);ps(" max=");pi(c->mr);ps("\n");
    c->att=1; c->ls=G_UNAVAIL; unsigned int bo=rp_backoff(c->att,&dpol);
    ps("  att1: ");ps(gs_nm(c->ls));ps(" bo=");pu(bo);ps("ms\n");
    if(rp_should(c->ls,c->att,&dpol)){clk+=bo;c->att=2;c->ls=G_OK;
        ps("  att2: ");ps(gs_nm(c->ls));ps(" (ok)\n");}
    fwd_unary(sv,mt,"{}",pln,0,(int)c->dl); return i;
}
static int to_set(int si,unsigned int t){if(si<0||si>=nstr)return -1;
    strs[si].dl_ms=(int)t;ps("grpc_proxy: timeout strm[");pi(strs[si].id);ps("]=");pu(t);ps("ms\n");return 0;}
static int dl_prop(int si,int dm) {
    if(si<0||si>=nstr) return -1; strm_t *x=&strs[si];
    unsigned int el=clk-x->cat; int rem=x->dl_ms-(int)el; if(rem<0) rem=0;
    int pr=dm<rem?dm:rem;
    ps("grpc_proxy: dl_prop strm[");pi(x->id);ps("] rem=");pi(rem);
    ps("ms dm=");pi(dm);ps("ms->");pi(pr);ps("ms\n"); return pr;
}
static int dl_chk(int si) {
    if(si<0||si>=nstr) return -1; strm_t *x=&strs[si];
    unsigned int el=clk-x->cat;
    if((int)el>=x->dl_ms){ps("grpc_proxy: DEADLINE strm[");pi(x->id);ps("] el=");pu(el);
        ps("ms>");pi(x->dl_ms);ps("ms\n");return G_DEAD;} return G_OK;
}
static int hed_send(const char *sv,const char *mt,int pln) {
    int h1=fwd_unary(sv,mt,"{}",pln,0,DEF_TIMEOUT);
    clk+=dhed.jit>0?dhed.jit:10;
    int h2=fwd_unary(sv,mt,"{}",pln,0,DEF_TIMEOUT);
    ps("grpc_proxy: hedging ");ps(sv);ps("/");ps(mt);ps(" h1=");pi(h1);ps(" h2=");pi(h2);ps("\n"); return h1;
}
static void svc_refresh(int i){if(i<0||i>=nsvc) return; int li=lb_sel(svcs[i].name);
    if(li>=0){ps("grpc_proxy: refresh ");ps(svcs[i].name);ps(" ->");pi(li);ps("\n");}}
static void rctx_clean(int i){if(i>=0&&i<nrc) rcs[i].use=0;}
static void hed_cancel(int i){ps("grpc_proxy: hed cancel ");pi(i);ps("\n");}

/* ── Tests ── */
static void t_svc(void) {
    phdr("Test: Service Discovery & LB");
    int s1=svc_reg("user-svc","10.0.0.1",50051,LB_RR);
    int s2=svc_reg("order-svc","10.0.0.2",50052,LB_LR);
    svc_reg("payment-svc","10.0.0.3",50053,LB_RAND);
    nr_update("user-svc","10.0.0.11",50051,1);
    nr_update("user-svc","10.0.0.12",50051,1);
    nr_update("user-svc","10.0.0.13",50051,0);
    ps("svcs: ");pi(nsvc);ps("\n");
    svc_resolve("user-svc"); svc_resolve("order-svc"); svc_resolve("payment-svc"); svc_resolve("unknown");
    int a=lb_sel("user-svc"),b=lb_sel("user-svc"),c=lb_sel("user-svc");
    ps("RR: ");pi(a);ps(",");pi(b);ps(",");pi(c);ps("\n");
    ps("LR: ");pi(lb_sel("order-svc"));ps("\n");
    svc_list(); ps(s1>=0&&s2>=0&&nsvc==3?"PASS: svc reg\n":"FAIL: svc reg\n"); ps("\n");
}
static void t_unary(void) {
    phdr("Test: Unary Forward");
    mdbg_t md; mzero(&md,sizeof(md)); md.n=0;
    md_add(&md,"x-req-id","req-001"); md_add(&md,"x-trace","abc"); md_add(&md,":authority","gw");
    int i=fwd_unary("user-svc","GetUser","{\"id\":42}",9,&md,3000);
    if(i>=0){ps("PASS: unary\n");md_dump("  md",&strs[i].md);} else ps("FAIL: unary\n"); ps("\n");
}
static void t_cli(void) {
    phdr("Test: Client Stream");
    int i=fwd_cli("order-svc","Upload",5000);
    if(i>=0){strm_send(i,128);strm_send(i,256);strm_send(i,64);strm_recv(i,48);strm_close(i);
        ps("PASS: client stream\n");} else ps("FAIL: client stream\n"); ps("\n");
}
static void t_srv(void) {
    phdr("Test: Server Stream");
    int i=fwd_srv("order-svc","List",10000);
    if(i>=0){strm_recv(i,512);strm_recv(i,512);strm_recv(i,256);strm_close(i);
        ps("PASS: server stream\n");} else ps("FAIL: server stream\n"); ps("\n");
}
static void t_bidi(void) {
    phdr("Test: Bidi Stream");
    int i=fwd_bidi("payment-svc","Process",15000);
    if(i>=0){strm_send(i,100);strm_recv(i,80);strm_send(i,100);strm_recv(i,80);strm_close(i);
        ps("PASS: bidi stream\n");} else ps("FAIL: bidi stream\n"); ps("\n");
}
static void t_tx(void) {
    phdr("Test: HTTP-JSON Transcode");
    hreq_t rq; mzero(&rq,sizeof(rq));
    tx_g2h("user-svc","GetUser","{\"id\":42}",&rq);
    ps("  path=");ps(rq.path);ps(" meth=");ps(rq.hmeth);ps("\n");
    md_dump("  headers",&rq.md);
    char os[MAX_NM],om[MAX_NM]; tx_h2g(&rq,os,om);
    ps("  recovered: ");ps(os);ps("/");ps(om);ps("\n");
    ps(scmp(os,"user-svc")==0&&scmp(om,"GetUser")==0?"PASS: transcode\n":"FAIL: transcode\n"); ps("\n");
}
static void t_gw(void) {
    phdr("Test: gRPC-Web");
    gw_enable(); char fr[128]; int w=gw_wrap("HELLO",5,fr,sizeof(fr));
    ps("  wrapped=");pi(w);ps("\n");
    char ob[128]; int fl=0; int u=gw_unwrap(fr,w,ob,sizeof(ob),&fl);
    ps("  unwrapped=");pi(u);ps(" fl=");pi(fl);ps("\n");
    ps(u==5&&fl==0?"PASS: grpc-web\n":"FAIL: grpc-web\n"); ps("\n");
}
static void t_au(void) {
    phdr("Test: Auth Integration");
    int c1=au_mtls("frontend","backend");
    ps("  mTLS idx=");pi(c1);ps("\n");
    ps(c1>=0&&certs[c1].ok?"PASS: mTLS\n":"FAIL: mTLS\n");
    int t1=au_jwt("eyJhbGciOiJSUzI1NiJ9.sig","user-svc");
    ps("  JWT idx=");pi(t1);ps("\n");
    ps(t1>=0&&toks[t1].ok?"PASS: JWT\n":"FAIL: JWT\n");
    char ot[64]; int rc=au_oauth("mobile","read:orders",ot,sizeof(ot));
    ps("  OAuth: ");ps(ot);ps("\n");
    ps(rc==0?"PASS: OAuth\n":"FAIL: OAuth\n"); ps("\n");
}
static void t_ref(void) {
    phdr("Test: Reflection");
    ref_reg("user-svc","GetUser"); ref_reg("user-svc","ListUsers"); ref_reg("user-svc","CreateUser");
    ref_reg("order-svc","CreateOrder"); ref_reg("order-svc","GetOrder");
    char bf[512]; int n=ref_list_svc(bf,sizeof(bf)); ps("  svcs:\n");ps(bf);
    int m=ref_list_meth("user-svc",bf,sizeof(bf)); ps("  methods:\n");ps(bf);
    int fd=ref_file("user.proto",bf,sizeof(bf)); ps("  file: ");ps(bf);ps("\n");
    ps(n>0&&m==3&&fd>0?"PASS: reflection\n":"FAIL: reflection\n"); ps("\n");
}
static void t_retry(void) {
    phdr("Test: Retry Policy");
    rp_set(3,100,2000,2);
    ps("  should(UNAVAIL,1)=");pi(rp_should(G_UNAVAIL,1,&dpol));ps("\n");
    ps("  should(OK,1)=");pi(rp_should(G_OK,1,&dpol));ps("\n");
    ps("  should(UNAVAIL,3)=");pi(rp_should(G_UNAVAIL,3,&dpol));ps("\n");
    int c=retry_exec("order-svc","Create",32);
    ps(c>=0&&rcs[c].ls==G_OK?"PASS: retry\n":"FAIL: retry\n");
    rctx_clean(c); ps("\n");
}
static void t_dl(void) {
    phdr("Test: Deadline & Timeout");
    int s1=fwd_srv("user-svc","Watch",5000); to_set(s1,5000);
    clk=100; int a=dl_chk(s1); ps("  chk@100: ");ps(gs_nm(a));ps("\n");
    int dm=dl_prop(s1,10000); ps("  downstream=");pi(dm);ps("ms\n");
    clk=6000; int b=dl_chk(s1); ps("  chk@6000: ");ps(gs_nm(b));ps("\n");
    ps(a==G_OK&&b==G_DEAD?"PASS: deadline\n":"FAIL: deadline\n");
    clk=0; dhed.en=1; dhed.ival=500; dhed.jit=50;
    int h=hed_send("user-svc","GetUser",16); hed_cancel(h); ps("\n");
}
static void t_md(void) {
    phdr("Test: Metadata Propagation");
    mdbg_t s,d; mzero(&s,sizeof(s));mzero(&d,sizeof(d));s.n=0;d.n=0;
    md_add(&s,"x-req-id","abc-123"); md_add(&s,"x-trace","456");
    md_add(&s,"x-custom","val"); md_add(&s,":authority","skip");
    int c=md_prop(&d,&s); ps("  copied=");pi(c);ps("\n");
    const char *v=md_get(&d,"x-req-id"); ps("  x-req-id=");ps(v?v:"(null)");ps("\n");
    const char *sk=md_get(&d,":authority"); ps("  :authority=");ps(sk?sk:"(null)");ps("\n");
    ps(c==3&&v&&scmp(v,"abc-123")==0&&!sk?"PASS: metadata\n":"FAIL: metadata\n"); ps("\n");
}

static void test_suite(void) {
    ps("=== gRPC Proxy Test Suite ===\n\n"); clk=0;
    t_svc(); t_unary(); t_cli(); t_srv(); t_bidi();
    t_tx(); t_gw(); t_au(); t_ref(); t_retry(); t_dl(); t_md();
    svc_list(); ps("\n=== All tests completed ===\n");
}
static void print_help(void) {
    ps("grpc_proxy v1.0 - gRPC Proxy/Gateway\n");
    ps("Usage: grpc_proxy [options]\n");
    ps("  -h    Show help\n"); ps("  -t    Run tests\n\n");
    ps("Features: service discovery, LB, unary/stream proxy,\n");
    ps("  gRPC<->HTTP/JSON, gRPC-Web, mTLS/JWT/OAuth,\n");
    ps("  reflection, retry/hedging, deadline propagation\n");
}
void _start(void) {
    unsigned int buf=host_alloc(BUF_SZ,16);
    int argc=host_get_argv(buf,BUF_SZ); char *args=(char*)buf;
    if(argc<=0){test_suite();return;}
    int ran=0;
    for(int i=0;i<argc;i++){if(args[i]=='\0') continue;
        if(scmp(&args[i],"-h")==0||scmp(&args[i],"--help")==0){print_help();ran=1;}
        else if(scmp(&args[i],"-t")==0||scmp(&args[i],"--test")==0){test_suite();ran=1;}
        while(args[i]!='\0') i++;}
    if(!ran) test_suite();
}
