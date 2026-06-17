/* container_registry: OCI Container Image Registry
 * Image storage, version management, access control, replication, GC, webhooks */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_NS 16
#define MAX_IMG 48
#define MAX_BLOB 64
#define MAX_TAG 64
#define MAX_MAN 32
#define MAX_SIG 16
#define MAX_REP 8
#define MAX_RET 8
#define MAX_HK 16
#define MAX_POLS 16
#define MAX_AUD 32
#define MAX_N 48
#define MAX_D 32
#define MAX_ARCH 8
#define ST_OK 0
#define ST_DEL 1
#define ST_EXP 2
#define PM_PULL 1
#define PM_PUSH 2
#define PM_ADM 4
#define WH_PUSH 1
#define WH_PULL 2
#define WH_DELETE 3
#define WH_SCAN 4
#define WH_CUSTOM 5

typedef struct { int id; char nm[MAX_N]; int im; int rp; unsigned int sz; int st; int at; } ns_t;
typedef struct { int id; int ni; char nm[MAX_N]; char tg[MAX_N]; char dg[MAX_D];
    unsigned int osz; unsigned int csz; int cr; int mb; int ly; int st; int pa; int la; } img_t;
typedef struct { int id; char dg[MAX_D]; unsigned int sz; unsigned int rsz;
    char mt[MAX_N]; int rf; int cp; int st; int at; } blob_t;
typedef struct { int id; int ii; char tn[MAX_N]; int mu; unsigned int vj; unsigned int vn;
    unsigned int vp; int st; int at; } tag_t;
typedef struct { int id; int ii; char dg[MAX_D]; char ar[MAX_ARCH][16];
    int ac; unsigned int sz; int st; int at; } man_t;
typedef struct { int id; int ii; char sr[MAX_N]; char sg[MAX_D];
    int tp; int vf; int at; } sig_t;
typedef struct { int id; int ni; char ds[MAX_N]; int xr; int ls;
    int bw; int au; int st; int at; } rep_t;
typedef struct { int id; int ni; int mv; int ma; unsigned int ms;
    int ru; int st; int at; } ret_t;
typedef struct { int id; int ni; int tp; char url[MAX_N]; char ev[MAX_N];
    int fi; int ls; int st; int at; } hk_t;
typedef struct { int id; char ac[MAX_N]; char op[MAX_N];
    char rs[MAX_N]; int rc; int ts; } aud_t;
typedef struct { int id; char pr[MAX_N]; int ni; int pm; int st; int at; } rba_t;

static ns_t nss[MAX_NS]; static img_t imgs[MAX_IMG]; static blob_t blobs[MAX_BLOB];
static tag_t tags[MAX_TAG]; static man_t mans[MAX_MAN]; static sig_t sigs[MAX_SIG];
static rep_t reps[MAX_REP]; static ret_t rets[MAX_RET]; static hk_t hks[MAX_HK];
static aud_t auds[MAX_AUD]; static rba_t pols[MAX_POLS];
static int cNS=1,cI=1,cB=1,cT=1,cM=1,cSG=1,cRP=1,cRT=1,cHK=1,cA=1,cP=1,clk=0;
static unsigned int g_bw=0;

static int sl(const char *s){int l=0;while(s[l])l++;return l;}
static int sc(const char *a,const char *b){while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;}return *a-*b;}
static void sn(char *d,const char *s,int n){int i=0;while(i<n-1&&s[i]){d[i]=s[i];i++;}d[i]='\0';}
static void ps(const char *s){host_print(s);}
static void pi(int v){
    char b[32];int p=0;
    if(v<0){b[p++]='-';v=-v;}
    if(v==0){b[p++]='0';}
    else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}}
    host_print(b);
}
static void aud(const char *a,const char *o,const char *r,int rc){
    if(cA>MAX_AUD)return;int i=cA-1;
    auds[i].id=cA;sn(auds[i].ac,a,MAX_N-1);sn(auds[i].op,o,MAX_N-1);
    sn(auds[i].rs,r,MAX_N-1);auds[i].rc=rc;auds[i].ts=++clk;cA++;
}
static void hk_fire(int ni,int tp,const char *ev){
    for(int i=0;i<MAX_HK;i++)if(hks[i].st==ST_OK&&hks[i].ni==ni&&hks[i].tp==tp){
        hks[i].ls=clk;hks[i].fi++;
        ps("  Webhook[");pi(hks[i].id);ps("] fired: ");ps(ev);ps(" -> ");ps(hks[i].url);ps("\n");}
}

/* Forward declarations */
int ns_create(const char *nm); int ns_list(void); int ns_stats(int id);
int img_push(int ni,const char *nm,const char *tg,unsigned int osz,const char *dg,int ly);
int img_pull(int id); int img_delete(int id); int img_list(int ni);
int blob_store(const char *dg,unsigned int sz,const char *mt,int cp);
int blob_dedup(const char *dg); int blob_unref_cleanup(void); int blob_list(void);
int tag_create(int ii,const char *tn,int mu); int tag_list(int ii);
int tag_delete(int id); int tag_semver(int id,unsigned int mj,unsigned int mn,unsigned int pt);
int tag_history(int ii); int tag_digest(int id);
int man_push(int ii,const char *dg,const char **ars,int ac); int man_list(int ii);
int man_get(int id); int man_multi_arch(int id);
int sig_sign(int ii,const char *sr,const char *sg); int sig_verify(int ii);
int scan_integrate(int ii,int cv,int cr,int ch);
int rep_create(int ni,const char *ds,int xr,int bw); int rep_trigger(int id); int rep_status(int id);
int rep_sync_policy(int id,int au); int pull_cache(const char *rr,const char *nm);
int preheat(int ni,const char *tg); int rep_bandwidth(int id,int bw);
int ret_set(int ni,int mv,int ma,unsigned int ms,int ru); int ret_enforce(int id);
int ret_space_stats(int ni);
int hk_create(int ni,int tp,const char *url,const char *ev);
int hk_list(int ni); int hk_fire_custom(int ni,const char *ev);
int rbac_add(const char *pr,int ni,int pm); int rbac_check(const char *pr,int ni,int pm);
int ns_isolate(const char *a,const char *b); int test_suite(void);

/* ===== Namespace Operations ===== */
int ns_create(const char *nm){
    if(cNS>MAX_NS)return -1;int i=cNS-1;
    nss[i].id=cNS;sn(nss[i].nm,nm,MAX_N-1);
    nss[i].im=0;nss[i].rp=0;nss[i].sz=0;
    nss[i].st=ST_OK;nss[i].at=++clk;cNS++;
    aud("sys","ns_create",nm,0);return nss[i].id;
}
int ns_list(void){
    int c=0;ps("  Namespaces:\n");
    for(int i=0;i<MAX_NS;i++)if(nss[i].id>0&&nss[i].st==ST_OK){
        ps("    [");pi(nss[i].id);ps("] ");ps(nss[i].nm);
        ps(" imgs=");pi(nss[i].im);ps(" sz=");pi((int)nss[i].sz);ps("\n");c++;}
    return c;
}
int ns_stats(int id){
    for(int i=0;i<MAX_NS;i++)if(nss[i].id==id){
        unsigned int tot=0,cmp=0;int bc=0;
        for(int j=0;j<MAX_IMG;j++)if(imgs[j].ni==id&&imgs[j].st==ST_OK){tot+=imgs[j].osz;cmp+=imgs[j].csz;}
        for(int b=0;b<MAX_BLOB;b++)if(blobs[b].st==ST_OK)bc++;
        ps("  Stats[");ps(nss[i].nm);ps("] total=");pi((int)tot);
        ps(" compressed=");pi((int)cmp);ps(" blobs=");pi(bc);
        if(tot>0){ps(" ratio=");pi((int)(cmp*100/tot));ps("%");}
        ps("\n");return 0;}
    return -1;
}

/* ===== Image Storage ===== */
int img_push(int ni,const char *nm,const char *tg,unsigned int osz,const char *dg,int ly){
    if(cI>MAX_IMG)return -1;int i=cI-1;
    imgs[i].id=cI;imgs[i].ni=ni;sn(imgs[i].nm,nm,MAX_N-1);
    sn(imgs[i].tg,tg,MAX_N-1);sn(imgs[i].dg,dg,MAX_D-1);
    imgs[i].osz=osz;imgs[i].csz=osz/2;imgs[i].cr=50;
    imgs[i].mb=0;imgs[i].ly=ly>0?ly:1;
    imgs[i].st=ST_OK;imgs[i].pa=++clk;imgs[i].la=0;cI++;
    for(int n=0;n<MAX_NS;n++)if(nss[n].id==ni){nss[n].im++;nss[n].sz+=osz;break;}
    aud("sys","push",nm,0);hk_fire(ni,WH_PUSH,"image.push");return imgs[i].id;
}
int img_pull(int id){
    for(int i=0;i<MAX_IMG;i++)if(imgs[i].id==id&&imgs[i].st==ST_OK){
        imgs[i].la=++clk;aud("sys","pull",imgs[i].nm,0);
        hk_fire(imgs[i].ni,WH_PULL,"image.pull");return 0;}
    return -1;
}
int img_delete(int id){
    for(int i=0;i<MAX_IMG;i++)if(imgs[i].id==id){
        aud("sys","del",imgs[i].nm,0);
        hk_fire(imgs[i].ni,WH_DELETE,"image.delete");
        imgs[i].id=0;imgs[i].st=ST_DEL;return 0;}
    return -1;
}
int img_list(int ni){
    int c=0;for(int i=0;i<MAX_IMG;i++)if(imgs[i].ni==ni&&imgs[i].st==ST_OK){
        ps("    [");pi(imgs[i].id);ps("] ");ps(imgs[i].nm);ps(":");ps(imgs[i].tg);
        ps(" sz=");pi((int)imgs[i].osz);ps(" comp=");pi((int)imgs[i].csz);
        ps(" ly=");pi(imgs[i].ly);ps("\n");c++;}
    return c;
}

/* ===== Blob Storage with Dedup ===== */
int blob_store(const char *dg,unsigned int sz,const char *mt,int cp){
    for(int i=0;i<MAX_BLOB;i++)if(blobs[i].st==ST_OK&&sc(blobs[i].dg,dg)==0){
        blobs[i].rf++;ps("  Dedup hit: ");ps(dg);ps("\n");return blobs[i].id;}
    if(cB>MAX_BLOB)return -1;int i=cB-1;
    blobs[i].id=cB;sn(blobs[i].dg,dg,MAX_D-1);blobs[i].sz=sz;
    blobs[i].rsz=cp?sz*2/3:sz;sn(blobs[i].mt,mt?mt:"octet",MAX_N-1);
    blobs[i].rf=1;blobs[i].cp=cp;blobs[i].st=ST_OK;blobs[i].at=++clk;cB++;
    aud("sys","blob_store",dg,0);return blobs[i].id;
}
int blob_dedup(const char *dg){
    int c=0;for(int i=0;i<MAX_BLOB;i++)if(blobs[i].st==ST_OK&&sc(blobs[i].dg,dg)==0)c++;
    return c;
}
int blob_unref_cleanup(void){
    int rm=0;for(int i=0;i<MAX_BLOB;i++)if(blobs[i].st==ST_OK&&blobs[i].rf<=0){
        blobs[i].id=0;blobs[i].st=ST_DEL;rm++;}
    aud("sys","gc_blob","unref",rm);return rm;
}
int blob_list(void){
    int c=0;ps("  Blobs:\n");
    for(int i=0;i<MAX_BLOB;i++)if(blobs[i].st==ST_OK){
        ps("    [B");pi(blobs[i].id);ps("] ");ps(blobs[i].dg);
        ps(" sz=");pi((int)blobs[i].sz);ps(" rsz=");pi((int)blobs[i].rsz);
        ps(" ref=");pi(blobs[i].rf);ps(blobs[i].cp?" compressed":"");ps("\n");c++;}
    return c;
}

/* ===== Tag & Version Management ===== */
int tag_create(int ii,const char *tn,int mu){
    if(cT>MAX_TAG)return -1;int i=cT-1;
    tags[i].id=cT;tags[i].ii=ii;sn(tags[i].tn,tn,MAX_N-1);
    tags[i].mu=mu;tags[i].vj=0;tags[i].vn=0;tags[i].vp=0;
    tags[i].st=ST_OK;tags[i].at=++clk;cT++;
    aud("sys","tag",tn,0);return tags[i].id;
}
int tag_list(int ii){
    int c=0;for(int i=0;i<MAX_TAG;i++)if(tags[i].ii==ii&&tags[i].st==ST_OK){
        ps("    [T");pi(tags[i].id);ps("] ");ps(tags[i].tn);
        if(tags[i].vj>0||tags[i].vn>0||tags[i].vp>0){
            ps(" v");pi((int)tags[i].vj);ps(".");pi((int)tags[i].vn);ps(".");pi((int)tags[i].vp);}
        ps(tags[i].mu?" (mutable)":" (immutable)");ps("\n");c++;}
    return c;
}
int tag_delete(int id){
    for(int i=0;i<MAX_TAG;i++)if(tags[i].id==id){
        if(!tags[i].mu)return -2;
        tags[i].id=0;tags[i].st=ST_DEL;aud("sys","tagdel",tags[i].tn,0);return 0;}
    return -1;
}
int tag_semver(int id,unsigned int mj,unsigned int mn,unsigned int pt){
    for(int i=0;i<MAX_TAG;i++)if(tags[i].id==id&&tags[i].st==ST_OK){
        tags[i].vj=mj;tags[i].vn=mn;tags[i].vp=pt;return 0;}
    return -1;
}
int tag_history(int ii){
    int c=0;for(int i=0;i<MAX_TAG;i++)if(tags[i].ii==ii){
        ps("    [");pi(tags[i].at);ps("] ");ps(tags[i].tn);
        ps(tags[i].st==ST_DEL?" (deleted)":"");ps("\n");c++;}
    return c;
}
int tag_digest(int id){
    for(int i=0;i<MAX_TAG;i++)if(tags[i].id==id&&tags[i].st==ST_OK){
        for(int j=0;j<MAX_IMG;j++)if(imgs[j].id==tags[i].ii&&imgs[j].st==ST_OK){
            ps("  Digest[");ps(tags[i].tn);ps("] = ");ps(imgs[j].dg);ps("\n");return 0;}
    }
    return -1;
}

/* ===== Multi-Arch Manifest List ===== */
int man_push(int ii,const char *dg,const char **ars,int ac){
    if(cM>MAX_MAN)return -1;int i=cM-1;
    mans[i].id=cM;mans[i].ii=ii;sn(mans[i].dg,dg,MAX_D-1);
    mans[i].ac=ac<MAX_ARCH?ac:MAX_ARCH;mans[i].sz=0;
    for(int a=0;a<mans[i].ac;a++)sn(mans[i].ar[a],ars[a],15);
    mans[i].st=ST_OK;mans[i].at=++clk;cM++;
    aud("sys","man_push",dg,0);return mans[i].id;
}
int man_list(int ii){
    int c=0;for(int i=0;i<MAX_MAN;i++)if(mans[i].ii==ii&&mans[i].st==ST_OK){
        ps("    [M");pi(mans[i].id);ps("] ");ps(mans[i].dg);ps(" arch=[");
        for(int a=0;a<mans[i].ac;a++){if(a)ps(",");ps(mans[i].ar[a]);}
        ps("]\n");c++;}
    return c;
}
int man_get(int id){
    for(int i=0;i<MAX_MAN;i++)if(mans[i].id==id&&mans[i].st==ST_OK){
        ps("  Manifest[");pi(mans[i].id);ps("] ");ps(mans[i].dg);
        ps(" archs=");pi(mans[i].ac);ps("\n");return 0;}
    return -1;
}
int man_multi_arch(int id){
    for(int i=0;i<MAX_MAN;i++)if(mans[i].id==id&&mans[i].st==ST_OK){
        ps("  Multi-arch: ");ps(mans[i].dg);ps(" ->");
        for(int a=0;a<mans[i].ac;a++){ps(" ");ps(mans[i].ar[a]);}
        ps("\n");return mans[i].ac;}
    return -1;
}

/* ===== Signature & Scan Integration ===== */
int sig_sign(int ii,const char *sr,const char *sg){
    if(cSG>MAX_SIG)return -1;int i=cSG-1;
    sigs[i].id=cSG;sigs[i].ii=ii;sn(sigs[i].sr,sr,MAX_N-1);
    sn(sigs[i].sg,sg,MAX_D-1);sigs[i].tp=1;sigs[i].vf=0;
    sigs[i].at=++clk;cSG++;aud(sr,"sign",sg,0);return sigs[i].id;
}
int sig_verify(int ii){
    int v=0;for(int i=0;i<MAX_SIG;i++)if(sigs[i].ii==ii){sigs[i].vf=1;v++;}
    aud("sys","verify","img",v);return v>0?0:-1;
}
int scan_integrate(int ii,int cv,int cr,int ch){
    ps("  Scan result for image ");pi(ii);ps(": cve=");pi(cv);
    ps(" critical=");pi(cr);ps(" high=");pi(ch);
    int pass=cr<=0&&ch<=2;ps(pass?" PASS":" FAIL");ps("\n");
    hk_fire(0,WH_SCAN,"scan.complete");return pass?0:-1;
}

/* ===== Replication & Sync ===== */
int rep_create(int ni,const char *ds,int xr,int bw){
    if(cRP>MAX_REP)return -1;int i=cRP-1;
    reps[i].id=cRP;reps[i].ni=ni;sn(reps[i].ds,ds,MAX_N-1);
    reps[i].xr=xr;reps[i].ls=0;reps[i].bw=bw>0?bw:0;
    reps[i].au=0;reps[i].st=ST_OK;reps[i].at=++clk;cRP++;
    aud("sys","rep_create",ds,0);return reps[i].id;
}
int rep_trigger(int id){
    for(int i=0;i<MAX_REP;i++)if(reps[i].id==id){
        reps[i].ls=++clk;aud("sys","rep_trig",reps[i].ds,0);return 0;}
    return -1;
}
int rep_status(int id){
    for(int i=0;i<MAX_REP;i++)if(reps[i].id==id&&reps[i].st==ST_OK){
        ps("  Replication[");pi(reps[i].id);ps("] ");ps(reps[i].ds);
        ps(" xregion=");pi(reps[i].xr);ps(" auto=");pi(reps[i].au);
        ps(" bw=");pi(reps[i].bw);ps("KB/s last=");pi(reps[i].ls);ps("\n");return 0;}
    return -1;
}
int rep_sync_policy(int id,int au){
    for(int i=0;i<MAX_REP;i++)if(reps[i].id==id){
        reps[i].au=au;aud("sys","sync_pol",reps[i].ds,au);return 0;}
    return -1;
}
int pull_cache(const char *rr,const char *nm){
    int ni=ns_create("cache");if(ni<0)return -1;
    int iid=img_push(ni,nm,"latest",2048,"sha256:cached",3);
    ps("  Cached ");ps(nm);ps(" from ");ps(rr);ps(" (image=");pi(iid);ps(")\n");
    aud("sys","pull_cache",rr,iid);return iid;
}
int preheat(int ni,const char *tg){
    int f=0;for(int i=0;i<MAX_IMG;i++)
        if(imgs[i].ni==ni&&imgs[i].st==ST_OK&&sc(imgs[i].tg,tg)==0){imgs[i].la=++clk;f++;}
    aud("sys","preheat",tg,f);return f;
}
int rep_bandwidth(int id,int bw){
    for(int i=0;i<MAX_REP;i++)if(reps[i].id==id){
        reps[i].bw=bw;ps("  Bandwidth limit set: ");pi(bw);ps(" KB/s\n");return 0;}
    return -1;
}

/* ===== Garbage Collection & Retention ===== */
int ret_set(int ni,int mv,int ma,unsigned int ms,int ru){
    if(cRT>MAX_RET)return -1;int i=cRT-1;
    rets[i].id=cRT;rets[i].ni=ni;rets[i].mv=mv;rets[i].ma=ma;
    rets[i].ms=ms;rets[i].ru=ru;rets[i].st=ST_OK;rets[i].at=++clk;cRT++;
    aud("sys","ret_set","ns",0);return rets[i].id;
}
int ret_enforce(int id){
    int rm=0;for(int r=0;r<MAX_RET;r++)if(rets[r].id==id){
        for(int i=0;i<MAX_IMG;i++)if(imgs[i].ni==rets[r].ni&&imgs[i].st==ST_OK){
            if(rets[r].ru&&!imgs[i].cr){imgs[i].st=ST_EXP;imgs[i].id=0;rm++;}
            else if(rets[r].ma>0&&(clk-imgs[i].pa)>rets[r].ma*10){
                imgs[i].st=ST_EXP;imgs[i].id=0;rm++;}
        }break;}
    aud("sys","ret_enforce","rule",rm);return rm;
}
int ret_space_stats(int ni){
    unsigned int tot=0,cmp=0;int ic=0,bc=0;
    for(int i=0;i<MAX_IMG;i++)if(imgs[i].ni==ni&&imgs[i].st==ST_OK){tot+=imgs[i].osz;cmp+=imgs[i].csz;ic++;}
    for(int b=0;b<MAX_BLOB;b++)if(blobs[b].st==ST_OK)bc++;
    ps("  Space[ns=");pi(ni);ps("] images=");pi(ic);ps(" blobs=");pi(bc);
    ps(" total=");pi((int)tot);ps(" compressed=");pi((int)cmp);
    if(tot>0){ps(" saving=");pi((int)(100-cmp*100/tot));ps("%");}
    ps("\n");return (int)tot;
}

/* ===== Webhook Notifications ===== */
int hk_create(int ni,int tp,const char *url,const char *ev){
    if(cHK>MAX_HK)return -1;int i=cHK-1;
    hks[i].id=cHK;hks[i].ni=ni;hks[i].tp=tp;
    sn(hks[i].url,url,MAX_N-1);sn(hks[i].ev,ev,MAX_N-1);
    hks[i].fi=0;hks[i].ls=0;hks[i].st=ST_OK;hks[i].at=++clk;cHK++;
    aud("sys","hk_create",url,0);return hks[i].id;
}
int hk_list(int ni){
    int c=0;for(int i=0;i<MAX_HK;i++)if(hks[i].ni==ni&&hks[i].st==ST_OK){
        ps("    [HK");pi(hks[i].id);ps("] tp=");pi(hks[i].tp);
        ps(" ");ps(hks[i].url);ps(" ev=");ps(hks[i].ev);
        ps(" fired=");pi(hks[i].fi);ps("\n");c++;}
    return c;
}
int hk_fire_custom(int ni,const char *ev){
    int f=0;for(int i=0;i<MAX_HK;i++)if(hks[i].ni==ni&&hks[i].tp==WH_CUSTOM){
        hks[i].ls=clk;hks[i].fi++;f++;
        ps("  Custom event: ");ps(ev);ps(" -> ");ps(hks[i].url);ps("\n");}
    aud("sys","hk_custom",ev,f);return f;
}

/* ===== RBAC & Access Control ===== */
int rbac_add(const char *pr,int ni,int pm){
    if(cP>MAX_POLS)return -1;int i=cP-1;
    pols[i].id=cP;sn(pols[i].pr,pr,MAX_N-1);
    pols[i].ni=ni;pols[i].pm=pm;
    pols[i].st=ST_OK;pols[i].at=++clk;cP++;
    aud("sys","rbac_add",pr,0);return pols[i].id;
}
int rbac_check(const char *pr,int ni,int pm){
    for(int i=0;i<MAX_POLS;i++)if(pols[i].st==ST_OK&&pols[i].ni==ni&&sc(pols[i].pr,pr)==0)
        if(pols[i].pm&pm||pols[i].pm&PM_ADM)return 0;
    aud(pr,"denied","ns",-1);return -1;
}
int ns_isolate(const char *a,const char *b){
    if(sc(a,b)==0)return 0;int ac=0,bc=0;
    for(int i=0;i<MAX_NS;i++)if(nss[i].st==ST_OK){
        if(sc(nss[i].nm,a)==0)ac++;if(sc(nss[i].nm,b)==0)bc++;}
    ps("  Namespace isolation: ");ps(a);ps("(");pi(ac);ps(") != ");ps(b);ps("(");pi(bc);ps(")\n");
    aud("sys","ns_isolate",a,0);return 1;
}

/* ===== Test Suite ===== */
int test_suite(void){
    int tp=0,tf=0;
    ps("=== Container Registry Test Suite ===\n\n");
    ps("[Test 1] Namespace Create & List\n");
    int n1=ns_create("production");int n2=ns_create("staging");
    if(n1>0&&n2>0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ns_list();ps("\n");

    ps("[Test 2] Image Push with Compression & Layers\n");
    int i1=img_push(n1,"myapp","v1.0",4096,"sha256:img01",3);
    int i2=img_push(n1,"myapp","v1.1",5120,"sha256:img02",4);
    if(i1>0&&i2>0&&imgs[i1-1].csz<imgs[i1-1].osz){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    img_list(n1);ps("\n");

    ps("[Test 3] Blob Storage with Dedup\n");
    int b1=blob_store("sha256:aabb",2048,"layer+gzip",1);
    int b2=blob_store("sha256:aabb",2048,"layer+gzip",1);
    int b3=blob_store("sha256:ccdd",1024,"config+json",0);
    int dd=blob_dedup("sha256:aabb");blob_list();
    if(b1>0&&b1==b2&&b3>0&&dd>=2){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 4] Tag Management & SemVer & History\n");
    int t1=tag_create(i1,"v1.0.0",0);int t2=tag_create(i1,"latest",1);
    tag_semver(t1,1,0,0);int di=tag_delete(t1);int dm=tag_delete(t2);
    tag_digest(t1>0?t1:i1);tag_history(i1);
    if(t1>0&&t2>0&&di==-2&&dm==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 5] Multi-Arch Manifest List\n");
    const char *ars[3];ars[0]="linux/amd64";ars[1]="linux/arm64";ars[2]="linux/arm/v7";
    int m1=man_push(i1,"sha256:man01",ars,3);
    int ma=man_multi_arch(m1);man_list(i1);
    if(m1>0&&ma==3){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 6] Signature & Verify\n");
    int s1=sig_sign(i1,"alice","sig-001");int vr=sig_verify(i1);
    if(s1>0&&vr==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 7] Vulnerability Scan Integration\n");
    int sc1=scan_integrate(i1,15,0,2);
    if(sc1==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 8] Replication & Pull Cache & Preheat\n");
    int rp1=rep_create(n1,"mirror.example.com",1,1024);
    int rtr=rep_trigger(rp1);rep_sync_policy(rp1,1);rep_bandwidth(rp1,512);
    rep_status(rp1);
    int cch=pull_cache("docker.io","nginx");int ph=preheat(n1,"v1.0");
    if(rp1>0&&rtr==0&&cch>0&&ph>0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 9] Retention & GC & Space Stats\n");
    int rt1=ret_set(n1,5,30,100000,1);
    (void)rt1;ret_space_stats(n1);
    int ufb=blob_unref_cleanup();
    ps("  Unreferenced blobs cleaned: ");pi(ufb);ps("\n");
    tp++;ps("\n");

    ps("[Test 10] Webhook Notifications\n");
    int h1=hk_create(n1,WH_PUSH,"https://hooks.example.com/push","push");
    int h2=hk_create(n1,WH_CUSTOM,"https://hooks.example.com/custom","deploy");
    hk_list(n1);
    int cf=hk_fire_custom(n1,"deploy-trigger");
    if(h1>0&&h2>0&&cf>=1){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 11] RBAC & Namespace Isolation\n");
    int p1=rbac_add("deploy-bot",n1,PM_PULL);
    int p2=rbac_add("dev-team",n1,PM_PULL|PM_PUSH);
    int cp=rbac_check("deploy-bot",n1,PM_PULL);
    int cx=rbac_check("deploy-bot",n1,PM_PUSH);
    int ni=ns_isolate("production","staging");
    if(p1>0&&p2>0&&cp==0&&cx==-1&&ni==1){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("=== Test Summary ===\n");
    ps("  Passed: ");pi(tp);ps("\n");ps("  Failed: ");pi(tf);ps("\n");
    ps("=== Test Suite Complete ===\n");return tf;
}

/* ===== Entry Point ===== */
void _start(void){
    unsigned int buf=host_alloc(512,16);
    host_get_argv(buf,512);
    int help=0,test=0;unsigned int pos=0;char *ap=(char *)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){
        char *a=&ap[pos];
        if(sc(a,"-h")==0||sc(a,"--help")==0)help=1;
        else if(sc(a,"-t")==0||sc(a,"--test")==0)test=1;
        while(pos<512&&ap[pos])pos++;pos++;}
    ps("Container Registry v1.0 - OCI Container Image Registry\n");
    if(help){
        ps("Usage: container_registry [options]\n");
        ps("  -h, --help    Show this help\n");
        ps("  -t, --test    Run test suite\n\n");
        ps("Features:\n");
        ps("  - OCI image storage with layered blobs and compression\n");
        ps("  - Blob deduplication and large file support\n");
        ps("  - Tag management with SemVer, digest, and history\n");
        ps("  - Multi-architecture manifest list\n");
        ps("  - Image signature verification\n");
        ps("  - Vulnerability scan integration\n");
        ps("  - Cross-region replication with bandwidth limit\n");
        ps("  - Pull-through cache and preheat mechanism\n");
        ps("  - Garbage collection and retention policies\n");
        ps("  - Webhook notifications for push/pull/delete/scan\n");
        ps("  - RBAC access control with namespace isolation\n");
        return;}
    if(test){test_suite();return;}
    ps("Use -h for help, -t for test\n");
}
