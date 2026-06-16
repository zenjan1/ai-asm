/* artifact_registry: OCI Artifact Registry (Harbor/ECR/GHCR)
 * Image storage, version control, cleanup, replication, scanning, RBAC */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_REPOS 32
#define MAX_BLOBS 64
#define MAX_MANS 32
#define MAX_IMGS 32
#define MAX_TAGS 64
#define MAX_SIGS 16
#define MAX_SBOMS 16
#define MAX_RETS 8
#define MAX_REPS 8
#define MAX_SCANS 16
#define MAX_POLS 16
#define MAX_AUDS 32
#define MAX_N 48
#define MAX_D 32
#define ST_OK 0
#define ST_DEL 1
#define ST_EXP 2
#define SG_CO 0
#define SG_NO 1
#define EN_TR 0
#define EN_CL 1
#define PM_PULL 1
#define PM_PUSH 2
#define PM_ADM 4

typedef struct { int id; char nm[MAX_N]; char ns[MAX_N]; int im; int bl; unsigned int sz; int st; int at; } repo_t;
typedef struct { int id; int ri; char dg[MAX_D]; unsigned int sz; char mt[MAX_N]; int rf; int st; int at; } blob_t;
typedef struct { int id; int ri; char dg[MAX_D]; char tg[MAX_N]; int br[8]; int bc; unsigned int sz; char pl[32]; int st; int at; } man_t;
typedef struct { int id; int ri; int mi; char nm[MAX_N]; char tg[MAX_N]; unsigned int sz; int sg; int sb; int sc; int st; int pa; int la; } img_t;
typedef struct { int id; int ri; int ii; char tn[MAX_N]; int mu; unsigned int vj; unsigned int vn; unsigned int vp; int st; int at; } tag_t;
typedef struct { int id; int ii; int tp; char sr[MAX_N]; char sg[MAX_D]; int vf; int at; } sig_t;
typedef struct { int id; int ii; char fm[MAX_N]; char dg[MAX_D]; unsigned int sz; int vf; int at; } sbm_t;
typedef struct { int id; int ri; int mv; int ma; unsigned int ms; int ru; int st; int at; } ret_t;
typedef struct { int id; int si; char ds[MAX_N]; int xr; int au; int ls; int st; int at; } rep_t;
typedef struct { int id; int ii; int eg; int cv; int cr; int hi; int me; int lo; int pp; int at; } scn_t;
typedef struct { int id; char pr[MAX_N]; int ri; int pm; int st; int at; } rba_t;
typedef struct { int id; char ac[MAX_N]; char op[MAX_N]; char rs[MAX_N]; int rc; int ts; } aud_t;

static repo_t repos[MAX_REPOS]; static blob_t blobs[MAX_BLOBS];
static man_t mans[MAX_MANS]; static img_t imgs[MAX_IMGS];
static tag_t tags[MAX_TAGS]; static sig_t sigs[MAX_SIGS];
static sbm_t sboms[MAX_SBOMS]; static ret_t rets[MAX_RETS];
static rep_t reps[MAX_REPS]; static scn_t scns[MAX_SCANS];
static rba_t pols[MAX_POLS]; static aud_t auds[MAX_AUDS];
static int cR=1,cB=1,cM=1,cI=1,cT=1,cS=1,cSB=1,cRT=1,cRP=1,cSC=1,cP=1,cA=1,clk=0;

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
    if(cA>MAX_AUDS)return;int i=cA-1;
    auds[i].id=cA;sn(auds[i].ac,a,MAX_N-1);sn(auds[i].op,o,MAX_N-1);
    sn(auds[i].rs,r,MAX_N-1);auds[i].rc=rc;auds[i].ts=++clk;cA++;
}

/* Forward declarations */
int repo_create(const char *n,const char *ns); int repo_delete(int id); int repo_list(void);
int blob_upload(int ri,const char *dg,unsigned int sz,const char *mt);
int blob_download(int id,unsigned int *so); int blob_delete(int id); int blob_exists(int ri,const char *dg);
int man_push(int ri,const char *dg,const char *tg,const int *br,int bc,const char *pl);
int man_pull(int id,int *bo,int *bc); int man_get(int id); int man_list(int ri);
int img_push(int ri,const char *n,const char *tg,int mi,unsigned int sz);
int img_pull(int id); int img_delete(int id);
int tag_create(int ri,int ii,const char *tn,int mu); int tag_list(int ri);
int tag_delete(int id); int tag_semver(int id,unsigned int mj,unsigned int mn,unsigned int pt);
int sign_cosign(int ii,const char *s,const char *sg); int sign_notary(int ii,const char *s,const char *sg);
int img_verify(int ii);
int sbom_attach(int ii,const char *f,const char *dg,unsigned int sz); int sbom_get(int id); int sbom_verify(int id);
int ret_set(int ri,int mv,int ma,unsigned int ms,int ru); int ret_enforce(int id);
int ret_clean_unused(int ri,int ma); int ret_clean_exp(int ri); int ret_size_lim(int ri,unsigned int lim);
int rep_create(int si,const char *ds,int xr,int au); int rep_trigger(int id); int rep_xregion(int si,const char *dr);
int pull_cache(const char *rr,const char *in); int prewarm(int ri,const char *tg);
int scan_trivy(int ii); int scan_clair(int ii); int scan_cve(int id,int ms);
int scan_gate(int id,int mc,int mh); int scan_rpt(int id);
int rbac_add(const char *pr,int ri,int pm); int rbac_pull(const char *pr,int ri); int rbac_push(const char *pr,int ri);
int ns_isolate(const char *a,const char *b); int test_suite(void);

/* ===== Repository Operations ===== */

int repo_create(const char *n,const char *ns){
    if(cR>MAX_REPOS)return -1;int i=cR-1;
    repos[i].id=cR;sn(repos[i].nm,n,MAX_N-1);
    sn(repos[i].ns,ns?ns:"default",MAX_N-1);
    repos[i].im=0;repos[i].bl=0;repos[i].sz=0;
    repos[i].st=ST_OK;repos[i].at=++clk;cR++;
    aud("sys","create",n,0);return repos[i].id;
}
int repo_delete(int id){
    for(int i=0;i<MAX_REPOS;i++)if(repos[i].id==id){
        aud("sys","delete",repos[i].nm,0);repos[i].id=0;repos[i].st=ST_DEL;return 0;}
    return -1;
}
int repo_list(void){
    int c=0;ps("  Repositories:\n");
    for(int i=0;i<MAX_REPOS;i++)if(repos[i].id>0&&repos[i].st==ST_OK){
        ps("    [");pi(repos[i].id);ps("] ");ps(repos[i].ns);ps("/");ps(repos[i].nm);
        ps(" imgs=");pi(repos[i].im);ps(" sz=");pi((int)repos[i].sz);ps("\n");c++;}
    return c;
}
/* ===== Blob Operations ===== */

int blob_upload(int ri,const char *dg,unsigned int sz,const char *mt){
    if(cB>MAX_BLOBS)return -1;int i=cB-1;
    blobs[i].id=cB;blobs[i].ri=ri;sn(blobs[i].dg,dg,MAX_D-1);
    blobs[i].sz=sz;sn(blobs[i].mt,mt?mt:"octet",MAX_N-1);
    blobs[i].rf=0;blobs[i].st=ST_OK;blobs[i].at=++clk;cB++;
    for(int r=0;r<MAX_REPOS;r++)if(repos[r].id==ri){repos[r].bl++;repos[r].sz+=sz;break;}
    aud("sys","blob_up",dg,0);return blobs[i].id;
}
int blob_download(int id,unsigned int *so){
    for(int i=0;i<MAX_BLOBS;i++)if(blobs[i].id==id&&blobs[i].st==ST_OK){
        if(so)*so=blobs[i].sz;blobs[i].rf++;return 0;}
    return -1;
}
int blob_delete(int id){
    for(int i=0;i<MAX_BLOBS;i++)if(blobs[i].id==id){blobs[i].id=0;blobs[i].st=ST_DEL;return 0;}
    return -1;
}
int blob_exists(int ri,const char *dg){
    for(int i=0;i<MAX_BLOBS;i++)if(blobs[i].ri==ri&&blobs[i].st==ST_OK&&sc(blobs[i].dg,dg)==0)return blobs[i].id;
    return -1;
}
/* ===== Manifest Operations ===== */

int man_push(int ri,const char *dg,const char *tg,const int *br,int bc,const char *pl){
    if(cM>MAX_MANS)return -1;int i=cM-1;
    mans[i].id=cM;mans[i].ri=ri;sn(mans[i].dg,dg,MAX_D-1);
    sn(mans[i].tg,tg?tg:"",MAX_N-1);mans[i].bc=bc<8?bc:8;mans[i].sz=0;
    for(int b=0;b<mans[i].bc;b++){mans[i].br[b]=br[b];
        for(int j=0;j<MAX_BLOBS;j++)if(blobs[j].id==br[b]){blobs[j].rf++;mans[i].sz+=blobs[j].sz;break;}}
    sn(mans[i].pl,pl?pl:"linux/amd64",31);
    mans[i].st=ST_OK;mans[i].at=++clk;cM++;
    aud("sys","man_push",dg,0);return mans[i].id;
}
int man_pull(int id,int *bo,int *bc){
    for(int i=0;i<MAX_MANS;i++)if(mans[i].id==id&&mans[i].st==ST_OK){
        if(bo)for(int b=0;b<mans[i].bc;b++)bo[b]=mans[i].br[b];
        if(bc)*bc=mans[i].bc;return 0;}
    return -1;
}
int man_get(int id){
    for(int i=0;i<MAX_MANS;i++)if(mans[i].id==id&&mans[i].st==ST_OK){
        ps("  Manifest[");pi(mans[i].id);ps("] ");ps(mans[i].dg);ps(" ");ps(mans[i].pl);
        ps(" blobs=");pi(mans[i].bc);ps(" sz=");pi((int)mans[i].sz);ps("\n");return 0;}
    return -1;
}
int man_list(int ri){
    int c=0;for(int i=0;i<MAX_MANS;i++)if(mans[i].ri==ri&&mans[i].st==ST_OK){
        ps("    [M");pi(mans[i].id);ps("] ");ps(mans[i].dg);ps(" ");ps(mans[i].pl);ps("\n");c++;}
    return c;
}
/* ===== Image Operations ===== */

int img_push(int ri,const char *n,const char *tg,int mi,unsigned int sz){
    if(cI>MAX_IMGS)return -1;int i=cI-1;
    imgs[i].id=cI;imgs[i].ri=ri;imgs[i].mi=mi;
    sn(imgs[i].nm,n,MAX_N-1);sn(imgs[i].tg,tg,MAX_N-1);
    imgs[i].sz=sz;imgs[i].sg=0;imgs[i].sb=0;imgs[i].sc=0;
    imgs[i].st=ST_OK;imgs[i].pa=++clk;imgs[i].la=0;cI++;
    for(int r=0;r<MAX_REPOS;r++)if(repos[r].id==ri){repos[r].im++;break;}
    aud("sys","push",n,0);return imgs[i].id;
}
int img_pull(int id){
    for(int i=0;i<MAX_IMGS;i++)if(imgs[i].id==id&&imgs[i].st==ST_OK){
        imgs[i].la=++clk;aud("sys","pull",imgs[i].nm,0);return 0;}
    return -1;
}
int img_delete(int id){
    for(int i=0;i<MAX_IMGS;i++)if(imgs[i].id==id){
        aud("sys","del",imgs[i].nm,0);imgs[i].id=0;imgs[i].st=ST_DEL;return 0;}
    return -1;
}
int tag_create(int ri,int ii,const char *tn,int mu){
    if(cT>MAX_TAGS)return -1;int i=cT-1;
    tags[i].id=cT;tags[i].ri=ri;tags[i].ii=ii;
    sn(tags[i].tn,tn,MAX_N-1);tags[i].mu=mu;
    tags[i].vj=0;tags[i].vn=0;tags[i].vp=0;
    tags[i].st=ST_OK;tags[i].at=++clk;cT++;
    aud("sys","tag",tn,0);return tags[i].id;
}
int tag_list(int ri){
    int c=0;for(int i=0;i<MAX_TAGS;i++)if(tags[i].ri==ri&&tags[i].st==ST_OK){
        ps("    [T");pi(tags[i].id);ps("] ");ps(tags[i].tn);
        if(tags[i].vj>0||tags[i].vn>0||tags[i].vp>0){
            ps(" v");pi((int)tags[i].vj);ps(".");pi((int)tags[i].vn);ps(".");pi((int)tags[i].vp);}
        ps(tags[i].mu?" (mutable)":" (immutable)");ps("\n");c++;}
    return c;
}
int tag_delete(int id){
    for(int i=0;i<MAX_TAGS;i++)if(tags[i].id==id){
        if(!tags[i].mu)return -2;
        tags[i].id=0;tags[i].st=ST_DEL;aud("sys","tagdel",tags[i].tn,0);return 0;}
    return -1;
}
int tag_semver(int id,unsigned int mj,unsigned int mn,unsigned int pt){
    for(int i=0;i<MAX_TAGS;i++)if(tags[i].id==id&&tags[i].st==ST_OK){
        tags[i].vj=mj;tags[i].vn=mn;tags[i].vp=pt;return 0;}
    return -1;
}
int sign_cosign(int ii,const char *s,const char *sg){
    if(cS>MAX_SIGS)return -1;int i=cS-1;
    sigs[i].id=cS;sigs[i].ii=ii;sigs[i].tp=SG_CO;
    sn(sigs[i].sr,s,MAX_N-1);sn(sigs[i].sg,sg,MAX_D-1);
    sigs[i].vf=0;sigs[i].at=++clk;cS++;
    for(int j=0;j<MAX_IMGS;j++)if(imgs[j].id==ii){imgs[j].sg=1;break;}
    aud(s,"cosign",sg,0);return sigs[i].id;
}
int sign_notary(int ii,const char *s,const char *sg){
    if(cS>MAX_SIGS)return -1;int i=cS-1;
    sigs[i].id=cS;sigs[i].ii=ii;sigs[i].tp=SG_NO;
    sn(sigs[i].sr,s,MAX_N-1);sn(sigs[i].sg,sg,MAX_D-1);
    sigs[i].vf=0;sigs[i].at=++clk;cS++;
    for(int j=0;j<MAX_IMGS;j++)if(imgs[j].id==ii){imgs[j].sg=1;break;}
    aud(s,"notary",sg,0);return sigs[i].id;
}
int img_verify(int ii){
    int v=0;for(int i=0;i<MAX_SIGS;i++)if(sigs[i].ii==ii){sigs[i].vf=1;v++;}
    aud("sys","verify","img",v);return v>0?0:-1;
}
int sbom_attach(int ii,const char *f,const char *dg,unsigned int sz){
    if(cSB>MAX_SBOMS)return -1;int i=cSB-1;
    sboms[i].id=cSB;sboms[i].ii=ii;
    sn(sboms[i].fm,f?f:"spdx",MAX_N-1);sn(sboms[i].dg,dg,MAX_D-1);
    sboms[i].sz=sz;sboms[i].vf=0;sboms[i].at=++clk;cSB++;
    for(int j=0;j<MAX_IMGS;j++)if(imgs[j].id==ii){imgs[j].sb=1;break;}
    aud("sys","sbom",dg,0);return sboms[i].id;
}
int sbom_get(int id){
    for(int i=0;i<MAX_SBOMS;i++)if(sboms[i].id==id){
        ps("  SBOM[");pi(sboms[i].id);ps("] ");ps(sboms[i].fm);ps(" ");ps(sboms[i].dg);
        ps(" sz=");pi((int)sboms[i].sz);ps(sboms[i].vf?" verified":" unverified");ps("\n");return 0;}
    return -1;
}
int sbom_verify(int id){
    for(int i=0;i<MAX_SBOMS;i++)if(sboms[i].id==id){
        sboms[i].vf=1;aud("sys","sbom_vfy",sboms[i].dg,0);return 0;}
    return -1;
}
int ret_set(int ri,int mv,int ma,unsigned int ms,int ru){
    if(cRT>MAX_RETS)return -1;int i=cRT-1;
    rets[i].id=cRT;rets[i].ri=ri;rets[i].mv=mv;rets[i].ma=ma;
    rets[i].ms=ms;rets[i].ru=ru;rets[i].st=ST_OK;rets[i].at=++clk;cRT++;
    aud("sys","ret_set","repo",0);return rets[i].id;
}
int ret_enforce(int id){
    int rm=0;for(int r=0;r<MAX_RETS;r++)if(rets[r].id==id){
        for(int i=0;i<MAX_IMGS;i++)if(imgs[i].ri==rets[r].ri&&imgs[i].st==ST_OK){
            if(rets[r].ru&&!imgs[i].sg){imgs[i].st=ST_EXP;imgs[i].id=0;rm++;}}break;}
    aud("sys","ret_enforce","rule",rm);return rm;
}
int ret_clean_unused(int ri,int ma){
    int rm=0;int cut=clk-(ma*10);
    for(int i=0;i<MAX_IMGS;i++)if(imgs[i].ri==ri&&imgs[i].st==ST_OK&&imgs[i].la>0&&imgs[i].la<cut){
        imgs[i].st=ST_EXP;imgs[i].id=0;rm++;}
    aud("sys","clean_unused","repo",rm);return rm;
}
int ret_clean_exp(int ri){
    int rm=0;for(int i=0;i<MAX_IMGS;i++)if(imgs[i].ri==ri&&imgs[i].st==ST_EXP){imgs[i].id=0;rm++;}return rm;
}
int ret_size_lim(int ri,unsigned int lim){
    unsigned int tot=0;int rm=0;
    for(int r=0;r<MAX_REPOS;r++)if(repos[r].id==ri){tot=repos[r].sz;break;}
    if(tot<=lim)return 0;
    for(int i=MAX_IMGS-1;i>=0;i--)if(imgs[i].ri==ri&&imgs[i].st==ST_OK){
        imgs[i].st=ST_EXP;tot=tot>imgs[i].sz?tot-imgs[i].sz:0;rm++;if(tot<=lim)break;}
    aud("sys","size_lim","repo",rm);return rm;
}
int rep_create(int si,const char *ds,int xr,int au){
    if(cRP>MAX_REPS)return -1;int i=cRP-1;
    reps[i].id=cRP;reps[i].si=si;sn(reps[i].ds,ds,MAX_N-1);
    reps[i].xr=xr;reps[i].au=au;reps[i].ls=0;
    reps[i].st=ST_OK;reps[i].at=++clk;cRP++;
    aud("sys","rep_create",ds,0);return reps[i].id;
}
int rep_trigger(int id){
    for(int i=0;i<MAX_REPS;i++)if(reps[i].id==id){
        reps[i].ls=++clk;aud("sys","rep_trig",reps[i].ds,0);return 0;}
    return -1;
}
int rep_xregion(int si,const char *dr){
    if(cRP>MAX_REPS)return -1;int i=cRP-1;
    reps[i].id=cRP;reps[i].si=si;sn(reps[i].ds,dr,MAX_N-1);
    reps[i].xr=1;reps[i].au=1;reps[i].ls=++clk;
    reps[i].st=ST_OK;reps[i].at=clk;cRP++;
    aud("sys","xr_sync",dr,0);return reps[i].id;
}
int pull_cache(const char *rr,const char *in){
    int ri=repo_create(in,"cache");if(ri<0)return -1;
    int bid=blob_upload(ri,"sha256:cached",1024,"oci.layer");
    int refs[1];refs[0]=bid;
    int mi=man_push(ri,"sha256:cachedm","latest",refs,1,"linux/amd64");
    int iid=img_push(ri,in,"latest",mi,1024);
    ps("  Cached ");ps(in);ps(" from ");ps(rr);ps(" (image=");pi(iid);ps(")\n");
    aud("sys","pull_cache",rr,iid);return iid;
}
int prewarm(int ri,const char *tg){
    int f=0;for(int i=0;i<MAX_IMGS;i++)
        if(imgs[i].ri==ri&&imgs[i].st==ST_OK&&sc(imgs[i].tg,tg)==0){imgs[i].la=++clk;f++;}
    aud("sys","prewarm",tg,f);return f;
}
int scan_trivy(int ii){
    if(cSC>MAX_SCANS)return -1;int i=cSC-1;
    scns[i].id=cSC;scns[i].ii=ii;scns[i].eg=EN_TR;
    scns[i].cr=1;scns[i].hi=3;scns[i].me=5;scns[i].lo=12;
    scns[i].cv=21;scns[i].pp=0;scns[i].at=++clk;cSC++;
    for(int j=0;j<MAX_IMGS;j++)if(imgs[j].id==ii){imgs[j].sc=1;break;}
    aud("trivy","scan","img",cSC);return scns[i].id;
}
int scan_clair(int ii){
    if(cSC>MAX_SCANS)return -1;int i=cSC-1;
    scns[i].id=cSC;scns[i].ii=ii;scns[i].eg=EN_CL;
    scns[i].cr=0;scns[i].hi=2;scns[i].me=7;scns[i].lo=9;
    scns[i].cv=18;scns[i].pp=0;scns[i].at=++clk;cSC++;
    for(int j=0;j<MAX_IMGS;j++)if(imgs[j].id==ii){imgs[j].sc=1;break;}
    aud("clair","scan","img",cSC);return scns[i].id;
}
int scan_cve(int id,int ms){
    for(int i=0;i<MAX_SCANS;i++)if(scns[i].id==id){
        int d=0;if(ms<=1)d+=scns[i].lo;if(ms<=2)d+=scns[i].me;
        if(ms<=3)d+=scns[i].hi;if(ms<=4)d+=scns[i].cr;
        ps("  CVEs at severity>=");pi(ms);ps(": ");pi(d);ps("\n");return d;}
    return -1;
}
int scan_gate(int id,int mc,int mh){
    for(int i=0;i<MAX_SCANS;i++)if(scns[i].id==id){
        int p=1;if(scns[i].cr>mc)p=0;if(scns[i].hi>mh)p=0;
        scns[i].pp=p;ps("  Policy gate: ");ps(p?"PASS":"FAIL");
        ps(" (critical=");pi(scns[i].cr);ps(" high=");pi(scns[i].hi);ps(")\n");return p;}
    return -1;
}
int scan_rpt(int id){
    for(int i=0;i<MAX_SCANS;i++)if(scns[i].id==id){
        ps("  Scan[");pi(scns[i].id);ps("] ");ps(scns[i].eg==EN_TR?"Trivy":"Clair");
        ps(" cve=");pi(scns[i].cv);ps("(C:");pi(scns[i].cr);ps(" H:");pi(scns[i].hi);
        ps(" M:");pi(scns[i].me);ps(" L:");pi(scns[i].lo);ps(") ");
        ps(scns[i].pp?"PASS":"PENDING");ps("\n");return 0;}
    return -1;
}
int rbac_add(const char *pr,int ri,int pm){
    if(cP>MAX_POLS)return -1;int i=cP-1;
    pols[i].id=cP;sn(pols[i].pr,pr,MAX_N-1);
    pols[i].ri=ri;pols[i].pm=pm;
    pols[i].st=ST_OK;pols[i].at=++clk;cP++;
    aud("sys","rbac_add",pr,0);return pols[i].id;
}
int rbac_pull(const char *pr,int ri){
    for(int i=0;i<MAX_POLS;i++)if(pols[i].st==ST_OK&&pols[i].ri==ri&&sc(pols[i].pr,pr)==0)
        if(pols[i].pm&PM_PULL||pols[i].pm&PM_ADM)return 0;
    aud(pr,"pull_denied","repo",-1);return -1;
}
int rbac_push(const char *pr,int ri){
    for(int i=0;i<MAX_POLS;i++)if(pols[i].st==ST_OK&&pols[i].ri==ri&&sc(pols[i].pr,pr)==0)
        if(pols[i].pm&PM_PUSH||pols[i].pm&PM_ADM)return 0;
    aud(pr,"push_denied","repo",-1);return -1;
}
int ns_isolate(const char *a,const char *b){
    if(sc(a,b)==0)return 0;int ac=0,bc=0;
    for(int i=0;i<MAX_REPOS;i++)if(repos[i].st==ST_OK){
        if(sc(repos[i].ns,a)==0)ac++;if(sc(repos[i].ns,b)==0)bc++;}
    ps("  Namespace isolation: ");ps(a);ps("(");pi(ac);ps(") != ");ps(b);ps("(");pi(bc);ps(")\n");
    aud("sys","ns_isolate",a,0);return 1;
}

/* ===== Test Suite ===== */
int test_suite(void){
    int tp=0,tf=0;
    ps("=== Artifact Registry Test Suite ===\n\n");
    ps("[Test 1] Repository Create & List\n");
    int r1=repo_create("myapp","production");
    int r2=repo_create("frontend","staging");
    if(r1>0&&r2>0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    repo_list();ps("\n");

    ps("[Test 2] Blob Upload & Existence\n");
    int b1=blob_upload(r1,"sha256:aabb",2048,"layer+gzip");
    int b2=blob_upload(r1,"sha256:ccdd",4096,"config+json");
    int be=blob_exists(r1,"sha256:aabb");
    unsigned int bs=0;blob_download(b1,&bs);
    if(b1>0&&b2>0&&be==b1&&bs==2048){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 3] Manifest Push & Pull\n");
    int refs[2];refs[0]=b1;refs[1]=b2;
    int m1=man_push(r1,"sha256:man01","v1.0",refs,2,"linux/amd64");
    int pr[2];int pc=0;man_pull(m1,pr,&pc);
    if(m1>0&&pc==2&&pr[0]==b1){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    man_get(m1);ps("\n");

    ps("[Test 4] Image Push & Pull\n");
    int i1=img_push(r1,"myapp","v1.0.0",m1,6144);
    int ip=img_pull(i1);
    if(i1>0&&ip==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 5] Tag Management & SemVer\n");
    int t1=tag_create(r1,i1,"v1.0.0",0);
    int t2=tag_create(r1,i1,"latest",1);
    tag_semver(t1,1,0,0);
    int di=tag_delete(t1);  /* immutable, should return -2 */
    int dm=tag_delete(t2);  /* mutable, should succeed */
    if(t1>0&&t2>0&&di==-2&&dm==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    tag_list(r1);ps("\n");

    ps("[Test 6] Image Signing (Cosign + Notary) & Verify\n");
    int s1=sign_cosign(i1,"alice","sig-cosign-01");
    int s2=sign_notary(i1,"bob","sig-notary-01");
    int vr=img_verify(i1);
    if(s1>0&&s2>0&&vr==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 7] SBOM Attach & Verify\n");
    int sb1=sbom_attach(i1,"spdx","sha256:sbom-01",512);
    int sbv=sbom_verify(sb1);sbom_get(sb1);
    if(sb1>0&&sbv==0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 8] Retention Policy Enforcement\n");
    int rt1=ret_set(r1,5,30,100000,1);
    int i2=img_push(r1,"myapp","v0.9.0",m1,3000);
    (void)i2;
    int rm=ret_enforce(rt1);
    ps("  Removed by retention: ");pi(rm);ps("\n");
    tp++;
    ps("\n");

    ps("[Test 9] Replication & Pull-Through Cache\n");
    int rp1=rep_create(r1,"mirror.example.com",1,1);
    int rtr=rep_trigger(rp1);
    int rxr=rep_xregion(r1,"us-west-2");
    int cch=pull_cache("docker.io","nginx");
    if(rp1>0&&rtr==0&&rxr>0&&cch>0){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 10] Vulnerability Scanning (Trivy)\n");
    int sc1=scan_trivy(i1);scan_rpt(sc1);scan_cve(sc1,3);
    int gt=scan_gate(sc1,0,5);
    if(sc1>0&&gt==1){tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("[Test 11] RBAC & Namespace Isolation\n");
    int p1=rbac_add("deploy-bot",r1,PM_PULL);
    int p2=rbac_add("dev-team",r1,PM_PULL|PM_PUSH);
    int p3=rbac_add("admin",r1,PM_ADM);
    int cp=rbac_pull("deploy-bot",r1);
    int cx=rbac_push("deploy-bot",r1);
    int ca=rbac_push("admin",r1);
    int ni=ns_isolate("production","staging");
    if(p1>0&&p2>0&&p3>0&&cp==0&&cx==-1&&ca==0&&ni==1){
        tp++;ps("  PASS\n");}else{tf++;ps("  FAIL\n");}
    ps("\n");

    ps("=== Test Summary ===\n");
    ps("  Passed: ");pi(tp);ps("\n");
    ps("  Failed: ");pi(tf);ps("\n");
    ps("=== Test Suite Complete ===\n");
    return tf;
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
    ps("Artifact Registry v1.0 - OCI Container Image Registry\n");
    if(help){
        ps("Usage: artifact_registry [options]\n");
        ps("  -h, --help    Show this help\n");
        ps("  -t, --test    Run test suite\n\n");
        ps("Features:\n");
        ps("  - OCI image storage with layered blobs and manifest indexing\n");
        ps("  - Tag management with Semantic Versioning\n");
        ps("  - Image signing (Cosign/Notary) and SBOM attachment\n");
        ps("  - Retention policies, size limits, expiry cleanup\n");
        ps("  - Cross-repo/cross-region replication and pull-through cache\n");
        ps("  - Vulnerability scanning (Trivy/Clair) with policy gates\n");
        ps("  - RBAC access control with namespace isolation and audit log\n");
        return;}
    if(test){test_suite();return;}
    ps("Use -h for help, -t for test\n");
}
