/*
 * cross_chain_bridge: Cross-Chain Bridge Module (v1.0)
 * AI-ASM OS - Asset transfer, message passing, consensus verification,
 * chain adaptation, security, and fee management across blockchains.
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
/* --- Constants --- */
#define MAX_CHAINS 8
#define MAX_ASSETS 16
#define MAX_MSGS 32
#define MAX_POOLS 8
#define MAX_ROUTES 16
#define MAX_HASH 32
#define MAX_SIG 64
#define MAX_ADDR 64
#define MAX_PROOF 128
#define MAX_PAY 128
#define MAX_AUDIT 64
#define MAX_REPLAY 32
#define OP_TRANSFER 0
#define OP_SWAP 1
#define OP_LP_ADD 2
#define OP_LP_REM 3
#define MSG_SENT 0
#define MSG_CONF 1
#define MSG_EXEC 2
#define MSG_FAIL 3
#define CH_EVM 0
#define CH_NEVM 1
#define CH_L2 2
#define CH_SIDE 3
#define CH_PRIV 4
/* --- Forward declarations --- */
static unsigned int my_strlen(const char *s);
static void ps(const char *s); static void pi(int v); static void ph(const unsigned char *d, unsigned int l);
static void *am(unsigned int sz); static void mz(void *p, unsigned int n);
static void mc(void *d, const void *s, unsigned int n);
static int mm(const void *a, const void *b, unsigned int n);
static unsigned int rng(void);
static void hdata(const unsigned char *d, unsigned int l, unsigned char *o);
static int asset_lock_mint(unsigned int sc, unsigned int dc, unsigned int aid, unsigned int amt, const char *snd);
static int asset_burn_unlock(unsigned int cid, unsigned int aid, unsigned int amt, const char *rec);
static int atomic_swap(unsigned int ca, unsigned int cb, unsigned int aa, unsigned int xa, unsigned int ab, unsigned int xb);
static int lp_add(unsigned int pid, unsigned int aa, unsigned int xa, unsigned int ab, unsigned int xb);
static int lp_remove(unsigned int pid, unsigned int lpt);
static int find_route(unsigned int src, unsigned int dst, unsigned int aid, unsigned int amt);
static int send_msg(unsigned int src, unsigned int dst, const unsigned char *pay, unsigned int len);
static int listen_ev(unsigned int cid); static int verify_proof(unsigned int mi);
static int check_replay(unsigned int cid, const unsigned char *mh);
static int guarantee_order(unsigned int cid);
static int init_lc(unsigned int cid, const unsigned char *gh);
static int verify_spv(unsigned int cid, const unsigned char *th, const unsigned char *pr, unsigned int pl);
static int verify_msig(const unsigned char *mh, const unsigned char sg[][MAX_SIG], const unsigned int *sn, unsigned int sc, unsigned int thr);
static int verify_committee(const unsigned char *d, unsigned int dl, const unsigned int *mem, unsigned int cnt, unsigned int q);
static int verify_zk(const unsigned char *pr, unsigned int pl, const unsigned char *pinput, unsigned int il);
static int init_adapter(unsigned int cid, unsigned int ct);
static int adapt_evm(unsigned int cid, const unsigned char *cd, unsigned int cl);
static int adapt_nevm(unsigned int cid, const unsigned char *tx, unsigned int tl);
static int adapt_l2(unsigned int cid, const unsigned char *sr, const unsigned char *td, unsigned int dl);
static int adapt_priv(unsigned int cid, const unsigned char *et, unsigned int tl);
static int sec_monitor(void); static int emerg_pause(unsigned int cid);
static int emerg_resume(unsigned int cid); static int rate_check(unsigned int cid, unsigned int amt);
static int ins_deposit(unsigned int amt); static int ins_claim(unsigned int ci, unsigned int amt);
static int audit_rec(unsigned int op, unsigned int cid, unsigned int det);
static int calc_fee(unsigned int src, unsigned int dst, unsigned int amt);
static int est_gas(unsigned int cid, unsigned int op); static int dist_fees(unsigned int tf);
static int dist_rewards(unsigned int vid, unsigned int stk); static int opt_costs(unsigned int cid);
/* --- Helpers --- */
static unsigned int my_strlen(const char *s) { unsigned int n=0; while(s[n]) n++; return n; }
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[16],o[16]; int i=0,neg=0; unsigned int uv;
    if(v<0){neg=1;uv=(unsigned int)(-v);}else uv=(unsigned int)v;
    if(uv==0) b[i++]='0';
    while(uv>0){b[i++]='0'+(char)(uv%10);uv/=10;}
    if(neg) b[i++]='-';
    int j=0; while(i>0) o[j++]=b[--i]; o[j]=0; ps(o);
}
static void ph(const unsigned char *d, unsigned int l) {
    const char hx[]="0123456789ABCDEF"; char b[132]; unsigned int i;
    for(i=0;i<l&&i<64;i++){b[i*2]=hx[(d[i]>>4)&0x0F];b[i*2+1]=hx[d[i]&0x0F];}
    b[i*2]=0; ps(b);
}
static void *am(unsigned int sz) { return (void*)(size_t)host_alloc(sz,8); }
static void mz(void *p, unsigned int n) { unsigned char *b=(unsigned char*)p; for(unsigned int i=0;i<n;i++) b[i]=0; }
static void mc(void *d, const void *s, unsigned int n) {
    unsigned char *a=(unsigned char*)d; const unsigned char *b=(const unsigned char*)s;
    for(unsigned int i=0;i<n;i++) a[i]=b[i];
}
static int mm(const void *a, const void *b, unsigned int n) {
    const unsigned char *x=(const unsigned char*)a,*y=(const unsigned char*)b;
    for(unsigned int i=0;i<n;i++) if(x[i]!=y[i]) return (int)x[i]-(int)y[i];
    return 0;
}
static unsigned int g_rng=0xDEADBEEF;
static unsigned int rng(void) { g_rng^=g_rng<<13; g_rng^=g_rng>>17; g_rng^=g_rng<<5; return g_rng; }
static void hdata(const unsigned char *d, unsigned int l, unsigned char *o) {
    unsigned int h=0x6A09E667, seed=0x428A2F98;
    for(unsigned int i=0;i<l;i++){
        seed^=((unsigned int)d[i])<<((i&3)*8); seed=(seed<<5)|(seed>>27); seed^=0x5A827999;
        h^=seed; h=(h<<7)|(h>>25);
    }
    for(int k=0;k<MAX_HASH;k++){o[k]=(unsigned char)((h>>((k&3)*8))&0xFF); h=(h*2654435761U)+0x9E3779B9;}
}
/* --- State --- */
typedef struct { unsigned int cid,aid,lk,mt,sup; char addr[MAX_ADDR]; int act; } basset_t;
typedef struct { unsigned int pid,aa,ab,ra,rb,tlpt; int act; } bpool_t;
typedef struct { unsigned int sc,dc,aid,amt,fee,hops,ich[4]; int valid; } broute_t;
typedef struct { unsigned int mid,sc,dc,nonce; unsigned char pay[MAX_PAY]; unsigned int plen;
    unsigned char mh[MAX_HASH]; int status; unsigned int ts; int act; } bmsg_t;
typedef struct { unsigned char mh[MAX_HASH]; unsigned int cid; int consumed; } breplay_t;
typedef struct { unsigned int cid; unsigned char bh[MAX_HASH]; unsigned int ht; unsigned char sr[MAX_HASH]; int act; } blc_t;
typedef struct { unsigned int cid,ct; unsigned char ch[MAX_HASH]; unsigned int ht,gp; int act; } badapt_t;
typedef struct { unsigned int cid,ac,pau,rl,rwtx,rwst,insbal; int act; } bsec_t;
typedef struct { unsigned int op,cid,det,ts,seq; } baud_t;
typedef struct { unsigned int bf,gp,tc,td,rp,lo; int act; } bfee_t;
static basset_t  g_as[MAX_ASSETS];   static unsigned int g_asc=0;
static bpool_t   g_pl[MAX_POOLS];    static unsigned int g_plc=0;
static broute_t  g_rt[MAX_ROUTES];   static unsigned int g_rtc=0;
static bmsg_t    g_ms[MAX_MSGS];     static unsigned int g_msc=0;
static breplay_t g_rp[MAX_REPLAY];   static unsigned int g_rpc=0;
static blc_t     g_lc[MAX_CHAINS];   static unsigned int g_lcc=0;
static badapt_t  g_ad[MAX_CHAINS];   static unsigned int g_adc=0;
static bsec_t    g_sc[MAX_CHAINS];   static unsigned int g_scc=0;
static baud_t    g_au[MAX_AUDIT];    static unsigned int g_auc=0;
static bfee_t    g_fe[MAX_CHAINS];   static unsigned int g_fec=0;
static unsigned int g_nonce=0, g_paused=0;
/* --- Asset Transfer --- */
static int asset_lock_mint(unsigned int sc, unsigned int dc, unsigned int aid, unsigned int amt, const char *snd) {
    if(g_paused){ps("[BRIDGE] paused\n");return -1;}
    if(amt==0||sc==dc){ps("[ASSET] bad params\n");return -1;}
    int sl=-1;
    for(unsigned int i=0;i<g_asc;i++) if(g_as[i].aid==aid&&g_as[i].cid==dc){sl=(int)i;break;}
    if(sl<0&&g_asc<MAX_ASSETS){
        sl=(int)g_asc; g_as[g_asc].aid=aid; g_as[g_asc].cid=dc;
        g_as[g_asc].lk=0; g_as[g_asc].mt=0; g_as[g_asc].sup=0; g_as[g_asc].act=1;
        unsigned int slen=my_strlen(snd); if(slen>=MAX_ADDR) slen=MAX_ADDR-1;
        for(unsigned int k=0;k<slen;k++) g_as[g_asc].addr[k]=snd[k]; g_as[g_asc].addr[slen]=0; g_asc++;
    }
    if(sl<0){ps("[ASSET] no slot\n");return -1;}
    g_as[sl].lk+=amt; g_as[sl].mt+=amt; g_as[sl].sup+=amt; g_nonce++;
    ps("[ASSET] lock-mint: "); pi((int)sc); ps("->"); pi((int)dc);
    ps(" asset="); pi((int)aid); ps(" amt="); pi((int)amt); ps("\n");
    audit_rec(0,dc,amt); return 0;
}
static int asset_burn_unlock(unsigned int cid, unsigned int aid, unsigned int amt, const char *rec) {
    if(g_paused){ps("[BRIDGE] paused\n");return -1;}
    if(amt==0){ps("[ASSET] zero\n");return -1;}
    for(unsigned int i=0;i<g_asc;i++){
        if(g_as[i].aid==aid&&g_as[i].cid==cid&&g_as[i].act){
            if(g_as[i].mt<amt){ps("[ASSET] insufficient\n");return -1;}
            g_as[i].lk-=amt; g_as[i].mt-=amt; g_as[i].sup-=amt; g_nonce++;
            ps("[ASSET] burn-unlock: chain="); pi((int)cid); ps(" amt="); pi((int)amt);
            ps(" to="); ps(rec); ps("\n"); audit_rec(1,cid,amt); return 0;
        }
    }
    ps("[ASSET] not found\n"); return -1;
}
static int atomic_swap(unsigned int ca, unsigned int cb, unsigned int aa, unsigned int xa, unsigned int ab, unsigned int xb) {
    if(g_paused||ca==cb||xa==0||xb==0){ps("[SWAP] bad params\n");return -1;}
    unsigned char cd[8]; mz(cd,8);
    cd[0]=(unsigned char)(ca&0xFF); cd[1]=(unsigned char)(cb&0xFF);
    cd[2]=(unsigned char)(aa&0xFF); cd[3]=(unsigned char)(ab&0xFF);
    cd[4]=(unsigned char)((xa>>8)&0xFF); cd[5]=(unsigned char)(xa&0xFF);
    cd[6]=(unsigned char)((xb>>8)&0xFF); cd[7]=(unsigned char)(xb&0xFF);
    unsigned char sh[MAX_HASH]; hdata(cd,8,sh);
    ps("[SWAP] "); pi((int)ca); ps("("); pi((int)xa); ps(")<->"); pi((int)cb);
    ps("("); pi((int)xb); ps(") hash="); ph(sh,8); ps("\n");
    if(asset_lock_mint(ca,cb,aa,xa,"swap")!=0||asset_lock_mint(cb,ca,ab,xb,"swap")!=0){
        ps("[SWAP] abort\n"); return -1;
    }
    g_nonce++; audit_rec(2,ca,xa); return 0;
}
static int lp_add(unsigned int pid, unsigned int aa, unsigned int xa, unsigned int ab, unsigned int xb) {
    if(xa==0||xb==0){ps("[LP] zero\n");return -1;}
    int sl=-1;
    for(unsigned int i=0;i<g_plc;i++) if(g_pl[i].pid==pid&&g_pl[i].act){sl=(int)i;break;}
    if(sl<0&&g_plc<MAX_POOLS){
        sl=(int)g_plc; g_pl[g_plc].pid=pid; g_pl[g_plc].aa=aa; g_pl[g_plc].ab=ab;
        g_pl[g_plc].ra=0; g_pl[g_plc].rb=0; g_pl[g_plc].tlpt=0; g_pl[g_plc].act=1; g_plc++;
    }
    if(sl<0){ps("[LP] no slot\n");return -1;}
    unsigned int prod=xa*xb, guess=prod>0?(prod>>1)+1:1;
    for(int it=0;it<16;it++){unsigned int nx=(guess+prod/guess)>>1; if(nx==guess||nx==guess+1||nx==guess-1) break; guess=nx;}
    unsigned int lpt=guess>0?guess:1;
    g_pl[sl].ra+=xa; g_pl[sl].rb+=xb; g_pl[sl].tlpt+=lpt;
    ps("[LP] add: pool="); pi((int)pid); ps(" a="); pi((int)xa); ps(" b="); pi((int)xb);
    ps(" lp="); pi((int)lpt); ps("\n"); audit_rec(3,pid,lpt); return 0;
}
static int lp_remove(unsigned int pid, unsigned int lpt) {
    if(lpt==0){ps("[LP] zero\n");return -1;}
    for(unsigned int i=0;i<g_plc;i++){
        if(g_pl[i].pid==pid&&g_pl[i].act){
            if(g_pl[i].tlpt<lpt){ps("[LP] insufficient\n");return -1;}
            unsigned int oa=(g_pl[i].ra*lpt)/g_pl[i].tlpt, ob=(g_pl[i].rb*lpt)/g_pl[i].tlpt;
            g_pl[i].ra-=oa; g_pl[i].rb-=ob; g_pl[i].tlpt-=lpt;
            ps("[LP] remove: pool="); pi((int)pid); ps(" out_a="); pi((int)oa); ps(" out_b="); pi((int)ob); ps("\n");
            audit_rec(4,pid,lpt); return 0;
        }
    }
    ps("[LP] not found\n"); return -1;
}
static int find_route(unsigned int src, unsigned int dst, unsigned int aid, unsigned int amt) {
    if(src==dst){ps("[ROUTE] same\n");return -1;}
    int best=-1; unsigned int bs=0xFFFFFFFF;
    for(unsigned int r=0;r<g_rtc;r++){
        if(g_rt[r].valid&&g_rt[r].sc==src&&g_rt[r].dc==dst&&g_rt[r].aid==aid){
            unsigned int sc=g_rt[r].fee+g_rt[r].hops*100;
            if(sc<bs){bs=sc;best=(int)r;}
        }
    }
    if(best<0&&g_rtc<MAX_ROUTES){
        best=(int)g_rtc; g_rt[g_rtc].sc=src; g_rt[g_rtc].dc=dst; g_rt[g_rtc].aid=aid;
        g_rt[g_rtc].amt=amt; g_rt[g_rtc].hops=1; g_rt[g_rtc].ich[0]=src; g_rt[g_rtc].ich[1]=dst;
        g_rt[g_rtc].fee=amt/100+1; g_rt[g_rtc].valid=1; g_rtc++;
    }
    if(best>=0){ps("[ROUTE] "); pi((int)src); ps("->"); pi((int)dst); ps(" fee="); pi((int)g_rt[best].fee); ps("\n");}
    else ps("[ROUTE] none\n");
    return best;
}
/* --- Message Passing --- */
static int send_msg(unsigned int src, unsigned int dst, const unsigned char *pay, unsigned int len) {
    if(g_paused||src==dst||len>MAX_PAY||g_msc>=MAX_MSGS){ps("[MSG] bad\n");return -1;}
    unsigned int idx=g_msc;
    g_ms[idx].mid=idx; g_ms[idx].sc=src; g_ms[idx].dc=dst; g_ms[idx].nonce=g_nonce;
    g_ms[idx].plen=len; mc(g_ms[idx].pay,pay,len); hdata(pay,len,g_ms[idx].mh);
    g_ms[idx].status=MSG_SENT; g_ms[idx].ts=rng(); g_ms[idx].act=1; g_msc++; g_nonce++;
    ps("[MSG] sent: id="); pi((int)idx); ps(" "); pi((int)src); ps("->"); pi((int)dst);
    ps(" hash="); ph(g_ms[idx].mh,8); ps("\n"); audit_rec(10,src,idx); return (int)idx;
}
static int listen_ev(unsigned int cid) {
    unsigned int cnt=0;
    for(unsigned int i=0;i<g_msc;i++)
        if(g_ms[i].act&&g_ms[i].sc==cid&&g_ms[i].status==MSG_SENT) cnt++;
    ps("[EVENT] chain="); pi((int)cid); ps(" pending="); pi((int)cnt); ps("\n"); return (int)cnt;
}
static int verify_proof(unsigned int mi) {
    if(mi>=g_msc||!g_ms[mi].act){ps("[PROOF] invalid\n");return -1;}
    unsigned char dv[MAX_HASH]; hdata(g_ms[mi].pay,g_ms[mi].plen,dv);
    if(mm(dv,g_ms[mi].mh,MAX_HASH)==0){
        g_ms[mi].status=MSG_CONF; ps("[PROOF] verified: "); pi((int)mi); ps("\n"); return 0;
    }
    ps("[PROOF] FAIL: "); pi((int)mi); ps("\n"); return -1;
}
static int check_replay(unsigned int cid, const unsigned char *mh) {
    for(unsigned int i=0;i<g_rpc;i++)
        if(g_rp[i].cid==cid&&mm(g_rp[i].mh,mh,MAX_HASH)==0&&g_rp[i].consumed){
            ps("[REPLAY] dup on "); pi((int)cid); ps("\n"); return -1;
        }
    if(g_rpc<MAX_REPLAY){
        g_rp[g_rpc].cid=cid; mc(g_rp[g_rpc].mh,mh,MAX_HASH); g_rp[g_rpc].consumed=1; g_rpc++;
    }
    ps("[REPLAY] unique on "); pi((int)cid); ps("\n"); return 0;
}
static int guarantee_order(unsigned int cid) {
    for(unsigned int i=0;i<g_msc;i++) for(unsigned int j=i+1;j<g_msc;j++)
        if(g_ms[i].act&&g_ms[j].act&&g_ms[i].dc==cid&&g_ms[j].dc==cid&&g_ms[j].ts<g_ms[i].ts){
            bmsg_t t=g_ms[i]; g_ms[i]=g_ms[j]; g_ms[j]=t;
        }
    ps("[ORDER] chain="); pi((int)cid); ps("\n"); return 0;
}
/* --- Consensus Verification --- */
static int init_lc(unsigned int cid, const unsigned char *gh) {
    if(g_lcc>=MAX_CHAINS){ps("[LC] full\n");return -1;}
    for(unsigned int i=0;i<g_lcc;i++) if(g_lc[i].cid==cid&&g_lc[i].act){ps("[LC] exists\n");return -1;}
    unsigned int idx=g_lcc; g_lc[idx].cid=cid; mc(g_lc[idx].bh,gh,MAX_HASH);
    g_lc[idx].ht=0; mz(g_lc[idx].sr,MAX_HASH); g_lc[idx].act=1; g_lcc++;
    ps("[LC] init: chain="); pi((int)cid); ps(" genesis="); ph(gh,8); ps("\n"); return 0;
}
static int verify_spv(unsigned int cid, const unsigned char *th, const unsigned char *pr, unsigned int pl) {
    blc_t *lc=0;
    for(unsigned int i=0;i<g_lcc;i++) if(g_lc[i].cid==cid&&g_lc[i].act){lc=&g_lc[i];break;}
    if(!lc||pl==0){ps("[SPV] bad\n");return -1;}
    unsigned char cb[MAX_HASH+MAX_PROOF]; unsigned int tot=pl<MAX_PROOF?pl:MAX_PROOF;
    mc(cb,th,MAX_HASH); mc(cb+MAX_HASH,pr,tot);
    unsigned char dv[MAX_HASH]; hdata(cb,MAX_HASH+tot,dv);
    int ok=1; for(unsigned int k=0;k<4;k++) if(dv[k]!=lc->sr[k]){ok=0;break;}
    ps(ok?"[SPV] valid\n":"[SPV] invalid\n"); return ok?0:-1;
}
static int verify_msig(const unsigned char *mh, const unsigned char sg[][MAX_SIG],
                        const unsigned int *sn, unsigned int sc, unsigned int thr) {
    if(sc==0||thr==0||sc<thr){ps("[MSIG] bad\n");return -1;}
    unsigned int vs=0;
    for(unsigned int i=0;i<sc;i++){
        int dup=0; for(unsigned int j=0;j<i;j++) if(sn[i]==sn[j]){dup=1;break;} if(dup) continue;
        unsigned char vb[MAX_HASH+MAX_SIG]; mc(vb,mh,MAX_HASH); mc(vb+MAX_HASH,sg[i],MAX_SIG);
        unsigned char vh[MAX_HASH]; hdata(vb,MAX_HASH+MAX_SIG,vh);
        int sv=0; for(unsigned int k=0;k<MAX_HASH;k++) if(vh[k]!=0){sv=1;break;} if(sv) vs++;
    }
    int ok=vs>=thr?1:0;
    ps("[MSIG] "); pi((int)vs); ps("/"); pi((int)thr); ps(" "); pi(ok); ps("\n"); return ok?0:-1;
}
static int verify_committee(const unsigned char *d, unsigned int dl, const unsigned int *mem, unsigned int cnt, unsigned int q) {
    if(cnt==0||q==0||cnt<q){ps("[COMM] bad\n");return -1;}
    unsigned char dh[MAX_HASH]; hdata(d,dl,dh); unsigned int ap=0;
    for(unsigned int i=0;i<cnt;i++){
        unsigned char vb[MAX_HASH+4]; mc(vb,dh,MAX_HASH);
        vb[MAX_HASH]=(unsigned char)(mem[i]&0xFF); vb[MAX_HASH+1]=(unsigned char)((mem[i]>>8)&0xFF);
        vb[MAX_HASH+2]=(unsigned char)((mem[i]>>16)&0xFF); vb[MAX_HASH+3]=(unsigned char)((mem[i]>>24)&0xFF);
        unsigned char vh[MAX_HASH]; hdata(vb,MAX_HASH+4,vh); if(vh[0]>=0x40) ap++;
    }
    int ok=ap>=q?1:0; ps("[COMM] "); pi((int)ap); ps("/"); pi((int)q); ps(" "); pi(ok); ps("\n"); return ok?0:-1;
}
static int verify_zk(const unsigned char *pr, unsigned int pl, const unsigned char *pinput, unsigned int il) {
    if(pl==0||il==0){ps("[ZK] empty\n");return -1;}
    unsigned int mx=pl<MAX_PROOF?pl:MAX_PROOF, tot=mx+il;
    unsigned char *buf=(unsigned char*)am(tot); if(!buf){ps("[ZK] alloc\n");return -1;}
    mc(buf,pr,mx); mc(buf+mx,pinput,il);
    unsigned char res[MAX_HASH]; hdata(buf,tot,res);
    int valid=res[0]<0xF0?1:0;
    ps("[ZK] pl="); pi((int)pl); ps(" il="); pi((int)il); ps(" v="); pi(valid); ps(" h="); ph(res,8); ps("\n");
    return valid?0:-1;
}
/* --- Chain Adaptation --- */
static int init_adapter(unsigned int cid, unsigned int ct) {
    if(g_adc>=MAX_CHAINS){ps("[ADAPT] full\n");return -1;}
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].act){ps("[ADAPT] exists\n");return -1;}
    unsigned int idx=g_adc; g_ad[idx].cid=cid; g_ad[idx].ct=ct; g_ad[idx].ht=0; g_ad[idx].gp=20;
    unsigned char fd[8]={
        (unsigned char)(cid&0xFF),(unsigned char)(ct&0xFF),0xCA,0xFE,
        (unsigned char)((cid>>8)&0xFF),(unsigned char)((ct>>8)&0xFF),0xBA,0xBE
    };
    hdata(fd,8,g_ad[idx].ch); g_ad[idx].act=1; g_adc++;
    const char *tn="unk";
    if(ct==CH_EVM) tn="EVM"; if(ct==CH_NEVM) tn="non-EVM"; if(ct==CH_L2) tn="L2";
    if(ct==CH_SIDE) tn="side"; if(ct==CH_PRIV) tn="priv";
    ps("[ADAPT] "); pi((int)cid); ps(" "); ps(tn); ps(" h="); ph(g_ad[idx].ch,8); ps("\n"); return 0;
}
static int adapt_evm(unsigned int cid, const unsigned char *cd, unsigned int cl) {
    badapt_t *ca=0;
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].ct==CH_EVM&&g_ad[i].act){ca=&g_ad[i];break;}
    if(!ca||cl==0){ps("[EVM] bad\n");return -1;}
    unsigned char env[4+MAX_PROOF], sel[4]={0xA9,0x05,0x9C,0xBB};
    unsigned int el=4+(cl<MAX_PROOF?cl:MAX_PROOF); mc(env,sel,4); mc(env+4,cd,el-4);
    unsigned char th[MAX_HASH]; hdata(env,el,th); ca->ht++; ca->gp++;
    ps("[EVM] chain="); pi((int)cid); ps(" len="); pi((int)cl); ps(" h="); ph(th,8); ps("\n"); return 0;
}
static int adapt_nevm(unsigned int cid, const unsigned char *tx, unsigned int tl) {
    badapt_t *ca=0;
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].ct==CH_NEVM&&g_ad[i].act){ca=&g_ad[i];break;}
    if(!ca||tl==0){ps("[NEVM] bad\n");return -1;}
    unsigned char th[MAX_HASH]; hdata(tx,tl,th); ca->ht++;
    ps("[NEVM] chain="); pi((int)cid); ps(" len="); pi((int)tl); ps(" h="); ph(th,8); ps("\n"); return 0;
}
static int adapt_l2(unsigned int cid, const unsigned char *sr, const unsigned char *td, unsigned int dl) {
    badapt_t *ca=0;
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].ct==CH_L2&&g_ad[i].act){ca=&g_ad[i];break;}
    if(!ca||dl==0){ps("[L2] bad\n");return -1;}
    unsigned char er[MAX_HASH]; hdata(td,dl,er);
    int rok=1; for(unsigned int k=0;k<4;k++) if(sr[k]!=er[k]){rok=0;break;} ca->ht++;
    ps("[L2] chain="); pi((int)cid); ps(" root_ok="); pi(rok); ps(" len="); pi((int)dl); ps("\n");
    return rok?0:-1;
}
static int adapt_priv(unsigned int cid, const unsigned char *et, unsigned int tl) {
    badapt_t *ca=0;
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].ct==CH_PRIV&&g_ad[i].act){ca=&g_ad[i];break;}
    if(!ca||tl==0){ps("[PRIV] bad\n");return -1;}
    unsigned int mx=tl<MAX_PROOF?tl:MAX_PROOF;
    unsigned char *dec=(unsigned char*)am(mx); if(!dec){ps("[PRIV] alloc\n");return -1;}
    for(unsigned int k=0;k<mx;k++) dec[k]=et[k]^ca->ch[k%MAX_HASH];
    unsigned char th[MAX_HASH]; hdata(dec,mx,th); ca->ht++;
    ps("[PRIV] chain="); pi((int)cid); ps(" len="); pi((int)tl); ps(" h="); ph(th,8); ps("\n"); return 0;
}
/* --- Security --- */
static int sec_monitor(void) {
    unsigned int al=0;
    for(unsigned int i=0;i<g_scc;i++){
        if(!g_sc[i].act) continue;
        if(g_sc[i].rwtx>g_sc[i].rl){g_sc[i].ac++;al++;ps("[SEC] ALERT chain=");pi((int)g_sc[i].cid);ps("\n");}
        if(g_sc[i].ac>3){g_sc[i].pau=1;ps("[SEC] AUTO-PAUSE chain=");pi((int)g_sc[i].cid);ps("\n");}
    }
    ps("[SEC] alerts="); pi((int)al); ps("\n"); return (int)al;
}
static int emerg_pause(unsigned int cid) {
    for(unsigned int i=0;i<g_scc;i++)
        if(g_sc[i].cid==cid&&g_sc[i].act){g_sc[i].pau=1;ps("[SEC] PAUSE ");pi((int)cid);ps("\n");audit_rec(20,cid,0);return 0;}
    if(cid==0xFFFFFFFF){g_paused=1;ps("[SEC] GLOBAL PAUSE\n");audit_rec(21,0,0);return 0;}
    ps("[SEC] not found\n"); return -1;
}
static int emerg_resume(unsigned int cid) {
    for(unsigned int i=0;i<g_scc;i++)
        if(g_sc[i].cid==cid&&g_sc[i].act){g_sc[i].pau=0;g_sc[i].ac=0;g_sc[i].rwtx=0;
            ps("[SEC] RESUME ");pi((int)cid);ps("\n");audit_rec(22,cid,0);return 0;}
    if(cid==0xFFFFFFFF){g_paused=0;ps("[SEC] GLOBAL RESUME\n");return 0;}
    ps("[SEC] not found\n"); return -1;
}
static int rate_check(unsigned int cid, unsigned int amt) {
    bsec_t *ss=0;
    for(unsigned int i=0;i<g_scc;i++) if(g_sc[i].cid==cid&&g_sc[i].act){ss=&g_sc[i];break;}
    if(!ss){ps("[RATE] no state\n");return -1;} if(ss->pau){ps("[RATE] paused\n");return -1;}
    unsigned int now=rng(); if(now-ss->rwst>1000){ss->rwtx=0;ss->rwst=now;}
    ss->rwtx++;
    if(ss->rwtx>ss->rl){ps("[RATE] EXCEEDED ");pi((int)cid);ps("\n");return -1;}
    ps("[RATE] OK ");pi((int)cid);ps(" txns=");pi((int)ss->rwtx);ps("\n"); return 0;
}
static int ins_deposit(unsigned int amt) {
    if(amt==0){ps("[INS] zero\n");return -1;}
    for(unsigned int i=0;i<g_scc;i++) if(g_sc[i].act){
        g_sc[i].insbal+=amt; ps("[INS] dep=");pi((int)amt);ps(" bal=");pi((int)g_sc[i].insbal);ps("\n");
        audit_rec(25,g_sc[i].cid,amt); return 0;
    }
    ps("[INS] no chain\n"); return -1;
}
static int ins_claim(unsigned int ci, unsigned int amt) {
    if(amt==0){ps("[INS] zero\n");return -1;}
    for(unsigned int i=0;i<g_scc;i++) if(g_sc[i].act&&g_sc[i].insbal>=amt){
        g_sc[i].insbal-=amt; ps("[INS] claim=");pi((int)ci);ps(" amt=");pi((int)amt);
        ps(" rem=");pi((int)g_sc[i].insbal);ps("\n"); audit_rec(26,g_sc[i].cid,amt); return 0;
    }
    ps("[INS] insufficient\n"); return -1;
}
static int audit_rec(unsigned int op, unsigned int cid, unsigned int det) {
    if(g_auc>=MAX_AUDIT){
        unsigned int sh=g_auc-MAX_AUDIT+1;
        for(unsigned int i=0;i+sh<g_auc;i++) g_au[i]=g_au[i+sh]; g_auc=MAX_AUDIT-1;
    }
    unsigned int idx=g_auc; g_au[idx].op=op; g_au[idx].cid=cid;
    g_au[idx].det=det; g_au[idx].ts=rng(); g_au[idx].seq=idx; g_auc++; return 0;
}
/* --- Fee Management --- */
static int calc_fee(unsigned int src, unsigned int dst, unsigned int amt) {
    unsigned int bf=10,cong=0;
    for(unsigned int i=0;i<g_scc;i++)
        if(g_sc[i].cid==src||g_sc[i].cid==dst) cong+=g_sc[i].rwtx;
    unsigned int total=bf+amt/200+cong;
    ps("[FEE] ");pi((int)src);ps("->");pi((int)dst);ps(" amt=");pi((int)amt);ps(" fee=");pi((int)total);ps("\n");
    return (int)total;
}
static int est_gas(unsigned int cid, unsigned int op) {
    unsigned int bg=21000,om=1;
    if(op==OP_SWAP) om=3; if(op==OP_LP_ADD||op==OP_LP_REM) om=2;
    for(unsigned int i=0;i<g_adc;i++) if(g_ad[i].cid==cid&&g_ad[i].act&&g_ad[i].ct==CH_L2){bg>>=2;break;}
    unsigned int gas=bg*om;
    ps("[GAS] chain=");pi((int)cid);ps(" op=");pi((int)op);ps(" gas=");pi((int)gas);ps("\n"); return (int)gas;
}
static int dist_fees(unsigned int tf) {
    if(tf==0){ps("[DIST] zero\n");return -1;}
    unsigned int vs=(tf*40)/100, is=(tf*30)/100, rs=(tf*20)/100, pr=tf-vs-is-rs;
    for(unsigned int i=0;i<g_scc;i++) if(g_sc[i].act){g_sc[i].insbal+=is;break;}
    for(unsigned int i=0;i<g_fec;i++) if(g_fe[i].act){g_fe[i].tc+=tf;g_fe[i].td+=tf;g_fe[i].rp+=rs;break;}
    ps("[DIST] fee=");pi((int)tf);ps(" v=");pi((int)vs);ps(" i=");pi((int)is);
    ps(" r=");pi((int)rs);ps(" p=");pi((int)pr);ps("\n"); return 0;
}
static int dist_rewards(unsigned int vid, unsigned int stk) {
    unsigned int rp=0,ts=stk+(rng()%10000)+1; if(ts==0) ts=1;
    for(unsigned int i=0;i<g_fec;i++) if(g_fe[i].act){rp=g_fe[i].rp;break;}
    unsigned int rw=(rp*stk)/ts; if(rw>rp) rw=rp;
    for(unsigned int i=0;i<g_fec;i++) if(g_fe[i].act&&g_fe[i].rp>=rw){g_fe[i].rp-=rw;break;}
    ps("[REWARD] v=");pi((int)vid);ps(" stk=");pi((int)stk);ps(" rw=");pi((int)rw);ps("\n"); return (int)rw;
}
static int opt_costs(unsigned int cid) {
    unsigned int sav=0;
    for(unsigned int i=0;i<g_rtc;i++){
        if(!g_rt[i].valid) continue;
        if(g_rt[i].sc!=cid&&g_rt[i].dc!=cid) continue;
        if(g_rt[i].hops>1){unsigned int bs=g_rt[i].fee/(g_rt[i].hops*2); if(bs>0){g_rt[i].fee-=bs;sav+=bs;}}
    }
    for(unsigned int i=0;i<g_fec;i++) if(g_fe[i].act){g_fe[i].lo=rng();break;}
    ps("[COST] chain=");pi((int)cid);ps(" sav=");pi((int)sav);ps("\n"); return (int)sav;
}
/* --- Init & Main --- */
static void init_bridge(void) {
    mz(g_as,sizeof(g_as));mz(g_pl,sizeof(g_pl));mz(g_rt,sizeof(g_rt));mz(g_ms,sizeof(g_ms));
    mz(g_rp,sizeof(g_rp));mz(g_lc,sizeof(g_lc));mz(g_ad,sizeof(g_ad));mz(g_sc,sizeof(g_sc));
    mz(g_au,sizeof(g_au));mz(g_fe,sizeof(g_fe));
    g_asc=0;g_plc=0;g_rtc=0;g_msc=0;g_rpc=0;g_lcc=0;g_adc=0;g_scc=0;g_auc=0;g_fec=0;g_nonce=0;g_paused=0;
    init_adapter(1,CH_EVM);
    g_fe[g_fec].bf=10;g_fe[g_fec].gp=20;g_fe[g_fec].tc=0;g_fe[g_fec].td=0;g_fe[g_fec].rp=0;g_fe[g_fec].lo=0;g_fe[g_fec].act=1;g_fec++;
    g_sc[g_scc].cid=1;g_sc[g_scc].ac=0;g_sc[g_scc].pau=0;g_sc[g_scc].rl=100;
    g_sc[g_scc].rwtx=0;g_sc[g_scc].rwst=0;g_sc[g_scc].insbal=0;g_sc[g_scc].act=1;g_scc++;
    init_adapter(2,CH_L2);
    g_sc[g_scc].cid=2;g_sc[g_scc].ac=0;g_sc[g_scc].pau=0;g_sc[g_scc].rl=200;
    g_sc[g_scc].rwtx=0;g_sc[g_scc].rwst=0;g_sc[g_scc].insbal=0;g_sc[g_scc].act=1;g_scc++;
    unsigned char g1[MAX_HASH],g2[MAX_HASH];
    unsigned char d1[4]={0x01,0x00,0xCA,0xFE},d2[4]={0x02,0x00,0xBA,0xBE};
    hdata(d1,4,g1); init_lc(1,g1); hdata(d2,4,g2); init_lc(2,g2);
    ps("[BRIDGE] initialized\n");
}
int main(void) {
    ps("=== cross_chain_bridge v1.0 ===\n"); init_bridge();
    ps("\n--- Asset Transfer ---\n");
    asset_lock_mint(1,2,100,5000,"0xAABB"); asset_burn_unlock(2,100,2000,"0xCCDD");
    atomic_swap(1,2,100,1000,200,950); lp_add(1,100,10000,200,9500); lp_remove(1,50);
    find_route(1,2,100,3000);
    ps("\n--- Message Passing ---\n");
    unsigned char p1[]="transfer_notify"; int mid=send_msg(1,2,p1,15);
    listen_ev(1); if(mid>=0) verify_proof((unsigned int)mid);
    unsigned char fh[MAX_HASH]; hdata(p1,15,fh); check_replay(1,fh); guarantee_order(2);
    ps("\n--- Consensus ---\n");
    unsigned char th[MAX_HASH]; hdata(p1,15,th);
    unsigned char pd[16]; mz(pd,16); pd[0]=0xDE; pd[1]=0xAD; verify_spv(1,th,pd,16);
    unsigned char sgs[2][MAX_SIG]; mz(sgs,sizeof(sgs)); sgs[0][0]=0xAA; sgs[1][0]=0xBB;
    unsigned int sn[2]={1,2}; verify_msig(th,sgs,sn,2,2);
    unsigned int mem[3]={10,20,30}; verify_committee(th,MAX_HASH,mem,3,2);
    unsigned char zp[32]; mz(zp,32); zp[0]=0x42; unsigned char zi[16]; mz(zi,16); zi[0]=0x01;
    verify_zk(zp,32,zi,16);
    ps("\n--- Chain Adaptation ---\n");
    unsigned char ec[8]={0xA9,0x05,0x9C,0xBB,0x01,0x02,0x03,0x04}; adapt_evm(1,ec,8);
    init_adapter(3,CH_NEVM); unsigned char nt[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
    adapt_nevm(3,nt,8);
    unsigned char ls[MAX_HASH],ld[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}; hdata(ld,8,ls);
    adapt_l2(2,ls,ld,8);
    init_adapter(4,CH_PRIV); unsigned char pt[8]; for(int k=0;k<8;k++) pt[k]=(unsigned char)k^0x55;
    adapt_priv(4,pt,8);
    ps("\n--- Security ---\n");
    sec_monitor(); rate_check(1,1000); ins_deposit(5000); ins_claim(1,1000);
    audit_rec(99,1,42); emerg_pause(1); emerg_resume(1);
    ps("\n--- Fee Management ---\n");
    calc_fee(1,2,10000); est_gas(1,OP_TRANSFER); est_gas(2,OP_SWAP);
    dist_fees(1000); dist_rewards(1,500); opt_costs(1);
    ps("\n=== cross_chain_bridge complete ===\n"); return 0;
}
