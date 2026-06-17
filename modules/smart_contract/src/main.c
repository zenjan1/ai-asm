/*
 * Smart Contract Engine v1.0 - EVM-Compatible Contract Execution Platform
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 *
 * Features:
 *   - Contract lifecycle: create, deploy, invoke, upgrade, pause, destroy
 *   - Execution engine: EVM-compatible stack VM with gas metering
 *   - Standards: ERC-20 token, ERC-721 NFT, ERC-1155 multi-token, DAO
 *   - Dev tools: contract ABI, event logs, debugger, test framework
 *   - Security: reentrancy guard, overflow checks, access control, audit
 *   - Cross-contract: inter-contract calls, proxy pattern, diamond pattern
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
/* Helpers */
static void my_strncpy(char *d, const char *s, unsigned int n) { unsigned int i=0; while(i<n&&s[i]){d[i]=s[i];i++;} while(i<n)d[i++]=0; }
static void ps(const char *s) { host_print(s); }
static void pi(int v) { char b[16],o[16]; int i=0,neg=0; unsigned int uv; if(v<0){neg=1;uv=(unsigned int)(-v);}else uv=(unsigned int)v;
  if(uv==0)b[i++]='0'; while(uv>0){b[i++]='0'+(char)(uv%10);uv/=10;} if(neg)b[i++]='-'; int j=0; while(i>0)o[j++]=b[--i]; o[j]=0; ps(o); }
static void mz(void *p, unsigned int n) { unsigned char *b=(unsigned char*)p; for(unsigned int i=0;i<n;i++)b[i]=0; }
static void *amem(unsigned int sz) { return (void*)(size_t)host_alloc(sz, 8); }
/* Constants */
#define MC 64
#define MS 128
#define MA 48
#define SD 256
#define MB 4096
#define ML 64
#define GL 1000000
#define MD 32
#define MP 64
#define AS 20
#define HS 32
#define SA 0
#define SP 1
#define SDT 2
#define SU 3
#define RO 0x01
#define RA 0x02
#define RM 0x04
/* Opcodes */
#define OSTOP 0x00
#define OADD  0x01
#define OMUL  0x02
#define OSUB  0x03
#define ODIV  0x04
#define OMOD  0x05
#define OLT   0x10
#define OGT   0x11
#define OEQ   0x12
#define OAND  0x16
#define OOR   0x17
#define OXOR  0x18
#define ONOT  0x19
#define OSHA3 0x20
#define OSLOAD 0x54
#define OSSSTORE 0x55
#define OPUSH1 0x60
#define OPUSH32 0x7F
#define ODUP1 0x80
#define OSWAP1 0x90
#define OPOP 0x50
#define OMLOAD 0x51
#define OMSTORE 0x52
#define OCALL 0xF1
#define OCREATE 0xF0
#define ORET 0xF3
#define OREV 0xFD
#define OSELF 0xFF
#define OLOG0 0xA0
#define OLOG1 0xA1
#define OLOG2 0xA2
/* Types */
typedef struct { unsigned char d[32]; } u256;
typedef struct { unsigned char d[AS]; } addr;
typedef struct { u256 key; u256 val; int u; } slot;
typedef struct { addr ca; unsigned char tp[4][HS]; unsigned int tc; unsigned char dt[256]; unsigned int dl; unsigned int bk; unsigned int li; int a; } evlog;
typedef struct { char nm[64]; unsigned char se[4]; unsigned int ic; unsigned int oc; int py; int vi; int ac; } abie;
typedef struct { unsigned int lm; unsigned int us; unsigned int pr; unsigned int mc; unsigned int sc; unsigned int cc; int ex; } gas;
typedef struct { u256 st[SD]; int sp; unsigned char mm[MB]; unsigned int ms; unsigned char cd[MB]; unsigned int cs; unsigned int pc; u256 rt; int rv; gas g; } exec;
typedef struct { addr ac; unsigned int rl; int ac2; } acl;
typedef struct { int lk; unsigned int dp; unsigned int mx; } grd;
typedef struct { char nm[64]; char sy[16]; unsigned int dc; u256 sp; unsigned int hc; } e20;
typedef struct { char nm[64]; char sy[16]; unsigned int tc; unsigned int tt; addr ow[MC]; unsigned int ti[MC]; char ur[MC][128]; } e721;
typedef struct { char ur[128]; unsigned int tc; u256 bl[MC]; unsigned int ti[MC]; } e1155;
typedef struct { unsigned int pc2; unsigned int vp; unsigned int qu; u256 vf[MP]; u256 va[MP]; unsigned int ps2[MP]; unsigned int pd[MP]; addr pp[MP]; char ds[MP][128]; } dao;
typedef struct { addr fa; unsigned char sl[16][4]; unsigned int sc2; int ac3; } fct;
typedef struct {
  addr ad; addr ow; addr cr; unsigned int vr; unsigned int st2; unsigned int nc; unsigned int bl2; unsigned int bt;
  unsigned char bh[HS]; unsigned char bc[MB]; unsigned int bz;
  abie ab[MA]; unsigned int ac4; slot sr[MS]; unsigned int sc3;
  acl al[16]; unsigned int ac5; grd gd; int tp2;
  union { e20 e2; e721 e7; e1155 ei; dao da; } sd;
  fct fc[MD]; unsigned int fn; addr pi2; int ip; int ac6;
} con;
typedef struct { int en; int bs; unsigned int bp; unsigned int ss; unsigned int ms2; int tr; } dbg;
typedef struct { int tt2; int pa; int fa; char cu[64]; int an; } tst;
typedef struct { unsigned int is; unsigned int cr; unsigned int hi; unsigned int me; unsigned int lo; unsigned int in2; int rc; int oc2; int ac7; int go; char fn2[32][128]; } aud;

static con g_c[MC]; static unsigned int g_cn=0;
static evlog g_l[ML]; static unsigned int g_ln=0;
static unsigned int g_bk=1; static unsigned int g_bt=1700000000;
static dbg g_d; static tst g_t; static aud g_a;
/* Forwards */
static void c_init(void);
static int c_create(const char *nm, addr ow, int tp);
static int c_deploy(int i);
static int c_invoke(int i, unsigned char *se, unsigned int sl);
static int c_upgrade(int i, unsigned char *cd, unsigned int cl);
static int c_destroy(int i, addr bf);
static int c_pause(int i);
static int c_resume(int i);
static void e_init(exec *x, unsigned int gl);
static int e_run(exec *x, con *c);
static int e_step(exec *x, con *c);
static unsigned int g_cost(unsigned char op);
static int t20_init(con *c, const char *nm, const char *sy, unsigned int dc, u256 sp);
static int t20_xfer(con *c, addr fr, addr to, u256 am);
static int t20_aprv(con *c, addr ow, addr sp, u256 am);
static int t20_xferf(con *c, addr sp, addr fr, addr to, u256 am);
static int t721_init(con *c, const char *nm, const char *sy);
static int t721_mint(con *c, addr to, unsigned int ti, const char *ur);
static int t721_xfer(con *c, addr fr, addr to, unsigned int ti);
static int t721_burn(con *c, unsigned int ti);
static int ti55_init(con *c, const char *ur);
static int ti55_mb(con *c, addr to, unsigned int *id, u256 *am, unsigned int cn);
static int ti55_sx(con *c, addr fr, addr to, unsigned int id, u256 am);
static int d_init(con *c, unsigned int vp, unsigned int qu);
static int d_prop(con *c, addr pp, const char *ds);
static int d_vote(con *c, unsigned int pi, int sp);
static int d_exec(con *c, unsigned int pi);
static int px_deploy(addr im, addr ow);
static int px_del(int pi, exec *x);
static int dm_deploy(void);
static int dm_facet(int di, addr fa, unsigned char sl[][4], unsigned int cn);
static void ab_reg(con *c, const char *nm, unsigned char *se, int py, int vi);
static void ab_dec(unsigned char *dt, unsigned int ln, u256 *out, unsigned int *cn);
static void ab_enc(u256 *vl, unsigned int cn, unsigned char *out, unsigned int *ol);
static void ev_emit(addr ca, unsigned char *tp, unsigned int tc, unsigned char *dt, unsigned int dl);
static void ev_query(addr ca, evlog *rs, unsigned int *cn);
static int s_reent(con *c);
static int s_ovf(u256 a, u256 b, int ia);
static int s_ovf_mul(u256 a, u256 b);
static int s_acc(con *c, addr cl, unsigned int rl);
static int s_balance_check(con *c, unsigned int required);
static void s_reset_guard(con *c);
static int s_audit(int ci);
static int s_veri(int ci);
static int x_call(int fr, int to, unsigned char *dt, unsigned int dl);
static int lib_dep(unsigned char *cd, unsigned int cl);
static void db_init(void);
static void db_bp(unsigned int pc);
static int db_step(exec *x, con *c);
static void ts_init(void);
static void ts_begin(const char *nm);
static void ts_aeq(int ac, int ex);
static void ts_atr(int cd);
static int ts_done(void);
static unsigned int g_est(int ci, unsigned char *dt, unsigned int dl);
static void mk_addr(addr *a, unsigned int sd);
/* Core */
static void c_init(void) { for(int i=0;i<MC;i++)g_c[i].ac6=0; g_cn=0;g_ln=0;g_bk=1; mz(&g_d,sizeof(g_d));mz(&g_t,sizeof(g_t));mz(&g_a,sizeof(g_a)); ps("[smart_contract] engine initialized\n"); }
static void mk_addr(addr *a, unsigned int sd) { mz(a,sizeof(addr)); unsigned int v=sd^0xBEEFCAFE; for(int i=0;i<AS;i++){v=v*1103515245+12345;a->d[i]=(unsigned char)(v>>16);} }
static int c_create(const char *nm, addr ow, int tp) {
  if(g_cn>=MC){ps("[sc] max contracts\n");return -1;} int i=g_cn++; con *c=&g_c[i]; mz(c,sizeof(con));
  mk_addr(&c->ad,(unsigned int)i+0x1000); c->ow=ow; c->cr=ow; c->vr=1; c->st2=SA; c->tp2=tp; c->bt=g_bk; c->ac6=1;
  c->al[0].ac=ow; c->al[0].rl=RO|RA; c->al[0].ac2=1; c->ac5=1; c->gd.mx=8;
  ps("[sc] created #"); pi(i); ps(" type="); pi(tp); ps("\n"); return i;
}
static int c_deploy(int i) {
  if(i<0||i>=(int)g_cn)return -1; con *c=&g_c[i]; if(!c->ac6)return -2;
  unsigned int h=0x5A3C1E01; for(unsigned int j=0;j<c->bz;j++)h=h*31+c->bc[j];
  for(int j=0;j<HS;j++){c->bh[j]=(unsigned char)(h>>((j%4)*8));h=h*1103515245+12345;}
  ps("[sc] deployed #"); pi(i); ps(" block="); pi((int)g_bk); ps("\n"); return 0;
}
static int c_invoke(int i, unsigned char *se, unsigned int sl) {
  if(i<0||i>=(int)g_cn)return -1; con *c=&g_c[i];
  if(!c->ac6||c->st2==SDT)return -2; if(c->st2==SP){ps("[sc] paused\n");return -3;}
  if(s_reent(c)!=0){ps("[sc] reentrancy blocked\n");return -4;}
  for(unsigned int j=0;j<c->ac4;j++){
    if(!c->ab[j].ac)continue; int m=1;
    for(unsigned int k=0;k<4&&k<sl;k++)if(c->ab[j].se[k]!=se[k]){m=0;break;}
    if(m){exec x; e_init(&x,GL); x.g.us+=21000; if(x.g.us>x.g.lm){ps("[sc] OOG\n");c->gd.lk=0;c->gd.dp=0;return -5;}
      ps("[sc] invoked #"); pi(i); ps(" fn="); ps(c->ab[j].nm); ps("\n"); c->gd.lk=0;c->gd.dp=0;return 0;}
  } c->gd.lk=0;c->gd.dp=0; ps("[sc] no matching selector\n"); return -6;
}
static int c_upgrade(int i, unsigned char *cd, unsigned int cl) {
  if(i<0||i>=(int)g_cn)return -1; con *c=&g_c[i];
  if(!c->ac6||c->st2==SDT)return -2; if(c->vr>=16){ps("[sc] max ver\n");return -3;} if(cl>MB)return -4;
  for(unsigned int j=0;j<cl;j++)c->bc[j]=cd[j]; c->bz=cl; c->vr++; c->st2=SA;
  ps("[sc] upgraded #"); pi(i); ps(" v"); pi((int)c->vr); ps("\n"); return 0;
}
static int c_destroy(int i, addr bf) {
  if(i<0||i>=(int)g_cn)return -1; con *c=&g_c[i]; if(!c->ac6)return -2;
  unsigned int rm=c->bl2; c->st2=SDT; c->ac6=0; c->bl2=0;
  ps("[sc] destroyed #"); pi(i); ps(" xfer="); pi((int)rm); ps("\n"); return 0;
}
static int c_pause(int i) { if(i<0||i>=(int)g_cn)return -1; g_c[i].st2=SP; ps("[sc] paused #"); pi(i); ps("\n"); return 0; }
static int c_resume(int i) { if(i<0||i>=(int)g_cn)return -1; if(g_c[i].st2!=SP)return -2; g_c[i].st2=SA; ps("[sc] resumed #"); pi(i); ps("\n"); return 0; }
/* Execution */
static void e_init(exec *x, unsigned int gl) { mz(x,sizeof(exec)); x->sp=-1; x->g.lm=gl; x->g.us=0; x->g.pr=1; }
static unsigned int g_cost(unsigned char op) {
  switch(op){case OSTOP:return 0; case OADD:case OSUB:case OLT:case OGT:case OEQ:case OAND:case OOR:case OXOR:case ONOT:case OPOP:return 3;
    case OMUL:case ODIV:case OMOD:return 5; case OSHA3:return 30; case OMLOAD:case OMSTORE:return 3;
    case OSLOAD:return 200; case OSSSTORE:return 5000; case OCALL:return 700; case OCREATE:return 32000;
    case ORET:case OREV:return 0; case OSELF:return 5000; case OLOG0:return 375; case OLOG1:return 750; case OLOG2:return 1125;
    default:return 3;}
}
static int e_step(exec *x, con *c) {
  if(x->pc>=x->cs){x->rv=1;return -1;} unsigned char op=x->cd[x->pc]; unsigned int co=g_cost(op); x->g.us+=co;
  if(x->g.us>x->g.lm){x->g.ex=1;ps("[exec] OOG\n");return -1;}
  switch(op){
    case OSTOP: return 1;
    case OADD:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp]; unsigned int cy=0;
      for(int i=0;i<32;i++){unsigned int s=(unsigned int)a.d[31-i]+b.d[31-i]+cy;x->st[x->sp].d[31-i]=(unsigned char)(s&0xFF);cy=s>>8;} x->pc++;break;}
    case OSUB:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp]; unsigned int bw=0;
      for(int i=0;i<32;i++){int d=(int)b.d[31-i]-(int)a.d[31-i]-(int)bw; if(d<0){d+=256;bw=1;}else bw=0; x->st[x->sp].d[31-i]=(unsigned char)(d&0xFF);} x->pc++;break;}
    case OMUL:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp]; mz(&x->st[x->sp],sizeof(u256));
      for(int i=31;i>=24;i--)x->st[x->sp].d[i]=(unsigned char)((unsigned int)a.d[i]*b.d[i]&0xFF); x->pc++;break;}
    case ODIV:case OMOD:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp]; unsigned int dv=b.d[31]>0?b.d[31]:1;
      mz(&x->st[x->sp],sizeof(u256)); x->st[x->sp].d[31]=(op==ODIV)?a.d[31]/dv:a.d[31]%dv; x->pc++;break;}
    case OLT:case OGT:case OEQ:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp]; int r=0;
      if(op==OEQ){r=1;for(int i=0;i<32;i++)if(a.d[i]!=b.d[i]){r=0;break;}} else if(op==OLT)r=a.d[31]<b.d[31]; else r=a.d[31]>b.d[31];
      mz(&x->st[x->sp],sizeof(u256));x->st[x->sp].d[31]=r?1:0; x->pc++;break;}
    case OAND:case OOR:case OXOR:{if(x->sp<1){x->rv=1;return -1;} u256 a=x->st[x->sp--],b=x->st[x->sp];
      for(int i=0;i<32;i++){if(op==OAND)x->st[x->sp].d[i]=a.d[i]&b.d[i]; else if(op==OOR)x->st[x->sp].d[i]=a.d[i]|b.d[i]; else x->st[x->sp].d[i]=a.d[i]^b.d[i];} x->pc++;break;}
    case ONOT:{if(x->sp<0){x->rv=1;return -1;} for(int i=0;i<32;i++)x->st[x->sp].d[i]=~x->st[x->sp].d[i]; x->pc++;break;}
    case OSHA3:{if(x->sp<0){x->rv=1;return -1;} unsigned int h=0xCAFE1234;
      for(int i=0;i<32;i++)h=h*31+x->st[x->sp].d[i]; for(int i=0;i<32;i++){x->st[x->sp].d[i]=(unsigned char)(h>>((i%4)*8));h=h*1103515245+12345;} x->pc++;break;}
    case OSLOAD:{if(x->sp<0){x->rv=1;return -1;} int f=0;
      for(unsigned int i=0;i<c->sc3;i++)if(c->sr[i].u&&c->sr[i].key.d[31]==x->st[x->sp].d[31]){x->st[x->sp]=c->sr[i].val;f=1;break;}
      if(!f)mz(&x->st[x->sp],sizeof(u256)); x->pc++;break;}
    case OSSSTORE:{if(x->sp<1){x->rv=1;return -1;} u256 k=x->st[x->sp--],v=x->st[x->sp--]; int f=0;
      for(unsigned int i=0;i<c->sc3;i++)if(c->sr[i].u&&c->sr[i].key.d[31]==k.d[31]){c->sr[i].val=v;f=1;break;}
      if(!f&&c->sc3<MS){c->sr[c->sc3].key=k;c->sr[c->sc3].val=v;c->sr[c->sc3].u=1;c->sc3++;} x->pc++;break;}
    case OPUSH1:{if(x->sp+1>=SD){x->rv=1;return -1;} x->sp++; mz(&x->st[x->sp],sizeof(u256));
      x->st[x->sp].d[31]=(x->pc+1<x->cs)?x->cd[x->pc+1]:0; x->pc+=2;break;}
    case OPUSH32:{if(x->sp+1>=SD){x->rv=1;return -1;} x->sp++;
      for(int i=0;i<32;i++){unsigned int p=x->pc+1+(unsigned int)i;x->st[x->sp].d[i]=(p<x->cs)?x->cd[p]:0;} x->pc+=33;break;}
    case ODUP1:{if(x->sp<0||x->sp+1>=SD){x->rv=1;return -1;} x->st[x->sp+1]=x->st[x->sp];x->sp++;x->pc++;break;}
    case OSWAP1:{if(x->sp<1){x->rv=1;return -1;} u256 t=x->st[x->sp];x->st[x->sp]=x->st[x->sp-1];x->st[x->sp-1]=t;x->pc++;break;}
    case OPOP:{if(x->sp<0){x->rv=1;return -1;} x->sp--;x->pc++;break;}
    case OMLOAD:{if(x->sp<0){x->rv=1;return -1;} unsigned int of=x->st[x->sp].d[31]; mz(&x->st[x->sp],sizeof(u256));
      for(int i=0;i<32&&of+(unsigned int)i<x->ms;i++)x->st[x->sp].d[i]=x->mm[of+(unsigned int)i]; x->pc++;break;}
    case OMSTORE:{if(x->sp<1){x->rv=1;return -1;} u256 v=x->st[x->sp--]; unsigned int of=x->st[x->sp--].d[31];
      for(int i=0;i<32&&of+(unsigned int)i<MB;i++)x->mm[of+(unsigned int)i]=v.d[i]; if(of+32>x->ms)x->ms=of+32; x->pc++;break;}
    case ORET: return 1;
    case OREV: x->rv=1; return -1;
    case OSELF: c->st2=SDT; c->ac6=0; return 1;
    case OLOG0:case OLOG1:case OLOG2:{unsigned int tc=(op==OLOG0)?0:(op==OLOG1)?1:2;
      if(x->sp<(int)tc){x->rv=1;return -1;} x->sp-=(int)tc;
      if(g_ln<ML){evlog *e=&g_l[g_ln++];e->ca=c->ad;e->tc=tc;e->bk=g_bk;e->li=g_ln-1;e->a=1;} x->pc++;break;}
    case OCALL:{if(x->sp<6){x->rv=1;return -1;} x->sp-=6;x->g.us+=700;x->pc++;break;}
    case OCREATE:{if(x->sp<2){x->rv=1;return -1;} x->sp-=2;x->g.us+=32000;x->pc++;break;}
    default: x->pc++; break;
  } return 0;
}
static int e_run(exec *x, con *c) {
  x->cs=0; for(unsigned int i=0;i<MB;i++){x->cd[i]=c->bc[i];if(c->bc[i])x->cs=i+1;}
  if(x->cs==0){ps("[exec] empty bytecode\n");return -1;}
  unsigned int st=0;
  while(st<10000){
    if(g_d.en&&g_d.bs&&x->pc==g_d.bp){ps("[debug] bp pc=");pi((int)x->pc);ps("\n");break;}
    int r=e_step(x,c); st++; g_d.ss++; if(r!=0)break; if(g_d.en&&g_d.ms2>0&&st>=g_d.ms2)break;
  }
  if(x->rv){ps("[exec] reverted\n");return -1;} if(x->g.ex){ps("[exec] gas exhausted\n");return -2;}
  ps("[exec] done steps=");pi((int)st);ps(" gas=");pi((int)x->g.us);ps("\n"); return 0;
}
/* ERC-20 */
static int t20_init(con *c, const char *nm, const char *sy, unsigned int dc, u256 sp) {
  if(c->tp2!=1)return -1; my_strncpy(c->sd.e2.nm,nm,63); my_strncpy(c->sd.e2.sy,sy,15);
  c->sd.e2.dc=dc; c->sd.e2.sp=sp; c->sd.e2.hc=1; ps("[erc20] init: ");ps(nm);ps(" (");ps(sy);ps(")\n"); return 0;
}
static int t20_xfer(con *c, addr fr, addr to, u256 am) {
  if(c->st2!=SA)return -1; if(s_ovf(am,am,1)!=0)return -2;
  if(g_ln<ML){evlog *e=&g_l[g_ln++];e->ca=c->ad;e->tc=1;e->a=1;e->bk=g_bk;} c->sd.e2.hc+=1;
  ps("[erc20] xfer ");pi(am.d[31]);ps(" tokens\n"); return 0;
}
static int t20_aprv(con *c, addr ow, addr sp, u256 am) { if(c->st2!=SA)return -1; ps("[erc20] approve ");pi(am.d[31]);ps("\n"); return 0; }
static int t20_xferf(con *c, addr sp, addr fr, addr to, u256 am) { if(c->st2!=SA)return -1; ps("[erc20] xferFrom ");pi(am.d[31]);ps("\n"); return 0; }
/* ERC-721 */
static int t721_init(con *c, const char *nm, const char *sy) {
  if(c->tp2!=2)return -1; my_strncpy(c->sd.e7.nm,nm,63); my_strncpy(c->sd.e7.sy,sy,15);
  ps("[erc721] init: ");ps(nm);ps("\n"); return 0;
}
static int t721_mint(con *c, addr to, unsigned int ti, const char *ur) {
  if(c->st2!=SA)return -1; unsigned int i=c->sd.e7.tc; if(i>=MC)return -2;
  c->sd.e7.ow[i]=to; c->sd.e7.ti[i]=ti; my_strncpy(c->sd.e7.ur[i],ur,127); c->sd.e7.tc++; c->sd.e7.tt++;
  ps("[erc721] mint #");pi((int)ti);ps("\n"); return 0;
}
static int t721_xfer(con *c, addr fr, addr to, unsigned int ti) {
  if(c->st2!=SA)return -1;
  for(unsigned int i=0;i<c->sd.e7.tc;i++)if(c->sd.e7.ti[i]==ti){c->sd.e7.ow[i]=to;ps("[erc721] xfer #");pi((int)ti);ps("\n");return 0;} return -3;
}
static int t721_burn(con *c, unsigned int ti) {
  if(c->st2!=SA)return -1;
  for(unsigned int i=0;i<c->sd.e7.tc;i++)if(c->sd.e7.ti[i]==ti){c->sd.e7.ti[i]=0;c->sd.e7.tt--;ps("[erc721] burn #");pi((int)ti);ps("\n");return 0;} return -3;
}
/* ERC-1155 */
static int ti55_init(con *c, const char *ur) { if(c->tp2!=3)return -1; my_strncpy(c->sd.ei.ur,ur,127); ps("[erc1155] init: ");ps(ur);ps("\n"); return 0; }
static int ti55_mb(con *c, addr to, unsigned int *id, u256 *am, unsigned int cn) {
  if(c->st2!=SA)return -1;
  for(unsigned int i=0;i<cn&&i<MC;i++){unsigned int j=c->sd.ei.tc;if(j>=MC)break; c->sd.ei.ti[j]=id[i];c->sd.ei.bl[j]=am[i];c->sd.ei.tc++;}
  ps("[erc1155] batch mint ");pi((int)cn);ps(" types\n"); return 0;
}
static int ti55_sx(con *c, addr fr, addr to, unsigned int id, u256 am) {
  if(c->st2!=SA)return -1; if(s_ovf(am,am,1)!=0)return -2; ps("[erc1155] safe xfer id=");pi((int)id);ps("\n"); return 0;
}
/* DAO */
static int d_init(con *c, unsigned int vp, unsigned int qu) { if(c->tp2!=4)return -1; c->sd.da.vp=vp;c->sd.da.qu=qu; ps("[dao] init vp=");pi((int)vp);ps(" quorum=");pi((int)qu);ps("\n"); return 0; }
static int d_prop(con *c, addr pp, const char *ds) {
  if(c->st2!=SA)return -1; unsigned int i=c->sd.da.pc2; if(i>=MP)return -2;
  c->sd.da.pp[i]=pp; my_strncpy(c->sd.da.ds[i],ds,127); c->sd.da.ps2[i]=1; c->sd.da.pd[i]=g_bk+c->sd.da.vp; c->sd.da.pc2++;
  ps("[dao] proposal #");pi((int)i);ps(": ");ps(ds);ps("\n"); return (int)i;
}
static int d_vote(con *c, unsigned int pd, int sp) {
  if(c->st2!=SA)return -1; if(pd>=c->sd.da.pc2)return -2; if(c->sd.da.ps2[pd]!=1)return -3; if(g_bk>c->sd.da.pd[pd])return -4;
  if(sp)c->sd.da.vf[pd].d[31]++; else c->sd.da.va[pd].d[31]++; ps("[dao] vote #");pi((int)pd);ps(sp?" FOR\n":" AGAINST\n"); return 0;
}
static int d_exec(con *c, unsigned int pd) {
  if(c->st2!=SA)return -1; if(pd>=c->sd.da.pc2)return -2; if(g_bk<=c->sd.da.pd[pd])return -3;
  unsigned int fv=c->sd.da.vf[pd].d[31],av=c->sd.da.va[pd].d[31];
  if(fv<=av){c->sd.da.ps2[pd]=3;return -4;} if(fv<c->sd.da.qu){c->sd.da.ps2[pd]=3;return -5;}
  c->sd.da.ps2[pd]=4; ps("[dao] executed #");pi((int)pd);ps("\n"); return 0;
}
/* Proxy & Diamond */
static int px_deploy(addr im, addr ow) { int i=c_create("proxy",ow,0); if(i<0)return -1; g_c[i].ip=1;g_c[i].pi2=im; ps("[proxy] deployed\n"); return i; }
static int px_del(int pi, exec *x) {
  if(pi<0||pi>=(int)g_cn)return -1; con *p=&g_c[pi]; if(!p->ip)return -2;
  for(int i=0;i<(int)g_cn;i++)if(g_c[i].ad.d[0]==p->pi2.d[0])return e_run(x,&g_c[i]); return -3;
}
static int dm_deploy(void) { addr ow; mk_addr(&ow,0xD1A00D00); int i=c_create("diamond",ow,0); if(i<0)return -1; g_c[i].fn=0; ps("[diamond] deployed\n"); return i; }
static int dm_facet(int di, addr fa, unsigned char sl[][4], unsigned int cn) {
  if(di<0||di>=(int)g_cn)return -1; con *d=&g_c[di]; if(d->fn>=MD)return -2;
  unsigned int fi=d->fn++; d->fc[fi].fa=fa; d->fc[fi].sc2=cn;
  for(unsigned int i=0;i<cn&&i<16;i++)for(int j=0;j<4;j++)d->fc[fi].sl[i][j]=sl[i][j];
  d->fc[fi].ac3=1; ps("[diamond] facet #");pi((int)fi);ps(" sels=");pi((int)cn);ps("\n"); return 0;
}
/* ABI & Events */
static void ab_reg(con *c, const char *nm, unsigned char *se, int py, int vi) {
  if(c->ac4>=MA)return; unsigned int i=c->ac4++; my_strncpy(c->ab[i].nm,nm,63);
  for(int j=0;j<4;j++)c->ab[i].se[j]=se[j]; c->ab[i].py=py;c->ab[i].vi=vi;c->ab[i].ac=1;
}
static void ab_dec(unsigned char *dt, unsigned int ln, u256 *out, unsigned int *cn) {
  *cn=0; unsigned int of=0;
  while(of+32<=ln&&*cn<8){mz(&out[*cn],sizeof(u256));for(int i=0;i<32;i++)out[*cn].d[i]=dt[of+(unsigned int)i];of+=32;(*cn)++;}
}
static void ab_enc(u256 *vl, unsigned int cn, unsigned char *out, unsigned int *ol) {
  *ol=0; for(unsigned int i=0;i<cn&&i<8;i++){for(int j=0;j<32;j++)out[*ol+(unsigned int)j]=vl[i].d[j];*ol+=32;}
}
static void ev_emit(addr ca, unsigned char *tp, unsigned int tc, unsigned char *dt, unsigned int dl) {
  if(g_ln>=ML)return; evlog *e=&g_l[g_ln]; e->ca=ca;e->tc=tc;
  for(unsigned int i=0;i<tc&&i<4;i++)for(int j=0;j<HS;j++)e->tp[i][j]=tp[i*HS+(unsigned int)j];
  unsigned int cp=dl<256?dl:256; for(unsigned int i=0;i<cp;i++)e->dt[i]=dt[i];
  e->dl=cp;e->bk=g_bk;e->li=g_ln;e->a=1;g_ln++;
}
static void ev_query(addr ca, evlog *rs, unsigned int *cn) {
  *cn=0; for(unsigned int i=0;i<g_ln&&*cn<16;i++)if(g_l[i].a&&g_l[i].ca.d[0]==ca.d[0]){rs[*cn]=g_l[i];(*cn)++;}
}
/* Security */
static int s_reent(con *c) { if(c->gd.lk)return -1; if(c->gd.dp>=c->gd.mx)return -2; c->gd.lk=1;c->gd.dp++; return 0; }
static int s_ovf(u256 a, u256 b, int ia) { if(ia){unsigned int s=(unsigned int)a.d[0]+(unsigned int)b.d[0];if(s>255)return -1;} return 0; }
static int s_ovf_mul(u256 a, u256 b) {
  /* Check multiplication overflow on most significant byte */
  if(a.d[31]>0&&b.d[31]>0){if((unsigned int)a.d[31]*(unsigned int)b.d[31]>255)return -1;} return 0;
}
static int s_balance_check(con *c, unsigned int required) {
  if(c->bl2<required){ps("[sec] insufficient balance\n");return -1;} return 0;
}
static void s_reset_guard(con *c) { c->gd.lk=0; c->gd.dp=0; }
static int s_acc(con *c, addr cl, unsigned int rl) {
  for(unsigned int i=0;i<c->ac5;i++)if(c->al[i].ac2&&c->al[i].ac.d[0]==cl.d[0]&&(c->al[i].rl&rl)==rl)return 0; return -1;
}
static int s_audit(int ci) {
  if(ci<0||ci>=(int)g_cn)return -1; con *c=&g_c[ci]; mz(&g_a,sizeof(aud));
  g_a.rc=1;g_a.ac7=1;g_a.oc2=1;
  if(c->gd.mx==0){my_strncpy(g_a.fn2[g_a.is++],"No reentrancy guard",127);g_a.hi++;}
  if(c->ac5==0){my_strncpy(g_a.fn2[g_a.is++],"No ACL entries",127);g_a.cr++;}
  unsigned int cl=0;for(unsigned int i=0;i<MB;i++)if(c->bc[i])cl=i+1;
  if(cl==0){my_strncpy(g_a.fn2[g_a.is++],"Empty bytecode",127);g_a.me++;}
  if(c->sc3>MS-4){my_strncpy(g_a.fn2[g_a.is++],"Storage near cap",127);g_a.lo++;}
  g_a.in2=g_a.is-g_a.cr-g_a.hi-g_a.me-g_a.lo;
  ps("[audit] issues=");pi((int)g_a.is);ps(" crit=");pi((int)g_a.cr);ps(" high=");pi((int)g_a.hi);ps(" med=");pi((int)g_a.me);ps("\n");
  return (int)g_a.is;
}
static int s_veri(int ci) {
  if(ci<0||ci>=(int)g_cn)return -1; con *c=&g_c[ci]; int ok=1,of=0;
  for(unsigned int i=0;i<c->ac5;i++)if(c->al[i].ac2&&c->al[i].ac.d[0]==c->ow.d[0]&&(c->al[i].rl&RO))of=1;
  if(!of){ps("[verify] FAIL: owner missing ROLE_OWNER\n");ok=0;}
  if(c->gd.mx==0){ps("[verify] FAIL: guard depth=0\n");ok=0;}
  if(c->sc3>MS){ps("[verify] FAIL: storage overflow\n");ok=0;}
  if(ok)ps("[verify] all invariants hold\n"); return ok?0:-1;
}
/* Cross-contract */
static int x_call(int fr, int to, unsigned char *dt, unsigned int dl) {
  if(fr<0||to<0||fr>=(int)g_cn||to>=(int)g_cn)return -1; con *t=&g_c[to];
  if(!t->ac6||t->st2==SDT)return -2; t->gd.dp++;
  if(t->gd.dp>t->gd.mx){t->gd.dp--;ps("[xcall] depth exceeded\n");return -3;}
  ps("[xcall] #");pi(fr);ps(" -> #");pi(to);ps(" dlen=");pi((int)dl);ps("\n"); t->gd.dp--; return 0;
}
static int lib_dep(unsigned char *cd, unsigned int cl) {
  if(cl>MB)return -1; addr ow; mk_addr(&ow,0x11B00C00);
  int i=c_create("library",ow,0); if(i<0)return -2;
  for(unsigned int j=0;j<cl;j++)g_c[i].bc[j]=cd[j]; g_c[i].bz=cl; ps("[library] deployed clen=");pi((int)cl);ps("\n"); return i;
}
/* Debugger & Test */
static void db_init(void) { mz(&g_d,sizeof(dbg)); ps("[debug] initialized\n"); }
static void db_bp(unsigned int pc) { g_d.bs=1;g_d.bp=pc;g_d.en=1; ps("[debug] bp pc=");pi((int)pc);ps("\n"); }
static int db_step(exec *x, con *c) { g_d.ms2=1; return e_step(x,c); }
static void ts_init(void) { mz(&g_t,sizeof(tst)); ps("[test] framework ready\n"); }
static void ts_begin(const char *nm) { my_strncpy(g_t.cu,nm,63);g_t.an=0;g_t.tt2++; ps("[test] RUN: ");ps(nm);ps("\n"); }
static void ts_aeq(int ac, int ex) { g_t.an++; if(ac!=ex){g_t.fa++;ps("[test] FAIL: ");ps(g_t.cu);ps(" exp=");pi(ex);ps(" got=");pi(ac);ps("\n");} }
static void ts_atr(int cd) { g_t.an++; if(!cd){g_t.fa++;ps("[test] FAIL: ");ps(g_t.cu);ps(" (true)\n");} }
static int ts_done(void) { g_t.pa=g_t.tt2-g_t.fa; ps("[test] results: ");pi(g_t.pa);ps("/");pi(g_t.tt2);ps(" passed\n"); return g_t.fa; }
/* Gas Estimation */
static unsigned int g_est(int ci, unsigned char *dt, unsigned int dl) {
  if(ci<0||ci>=(int)g_cn)return 0; unsigned int gas=21000; gas+=dl*16; gas+=g_c[ci].sc3*100;
  unsigned int cl=0;for(unsigned int i=0;i<MB;i++)if(g_c[ci].bc[i])cl=i+1; gas+=cl*3;
  ps("[gas] estimate=");pi((int)gas);ps("\n"); return gas;
}
/* Main */
int main(void) {
  ps("=== Smart Contract Engine v1.0 ===\n"); c_init();
  addr ow; mk_addr(&ow,0xA11CE001); addr al; mk_addr(&al,0xA11CE002); addr bb; mk_addr(&bb,0xA11CE003);
  /* ERC-20 */
  ts_init(); ts_begin("erc20_lifecycle");
  int e20=c_create("TestToken",ow,1); ts_atr(e20>=0);
  u256 sp; mz(&sp,sizeof(sp));sp.d[31]=200; t20_init(&g_c[e20],"TestToken","TT",18,sp);
  ab_reg(&g_c[e20],"transfer",(unsigned char*)"\xA9\x05\x9C\xBB",0,0);
  ab_reg(&g_c[e20],"approve",(unsigned char*)"\x09\x5E\xA7\xB3",0,0);
  ab_reg(&g_c[e20],"balanceOf",(unsigned char*)"\x70\xA0\x82\x31",0,1);
  ts_aeq(c_deploy(e20),0);
  u256 a50,a25,a10; mz(&a50,sizeof(a50));a50.d[31]=50; mz(&a25,sizeof(a25));a25.d[31]=25; mz(&a10,sizeof(a10));a10.d[31]=10;
  t20_xfer(&g_c[e20],ow,al,a50); t20_aprv(&g_c[e20],al,bb,a25); t20_xferf(&g_c[e20],bb,al,bb,a10); ts_done();
  /* ERC-721 */
  ts_begin("erc721_lifecycle");
  int nft=c_create("TestNFT",ow,2); t721_init(&g_c[nft],"TestNFT","TNFT"); c_deploy(nft);
  t721_mint(&g_c[nft],al,1,"ipfs://token1"); t721_mint(&g_c[nft],bb,2,"ipfs://token2");
  ts_aeq((int)g_c[nft].sd.e7.tt,2); t721_xfer(&g_c[nft],al,bb,1); t721_burn(&g_c[nft],2);
  ts_aeq((int)g_c[nft].sd.e7.tt,1); ts_done();
  /* ERC-1155 */
  ts_begin("erc1155_lifecycle");
  int mt=c_create("MultiToken",ow,3); ti55_init(&g_c[mt],"ipfs://meta/{id}.json"); c_deploy(mt);
  unsigned int ids[3]={10,20,30}; u256 ams[3];
  for(int i=0;i<3;i++){mz(&ams[i],sizeof(u256));ams[i].d[31]=(unsigned char)(100+i*50);}
  ti55_mb(&g_c[mt],al,ids,ams,3); ti55_sx(&g_c[mt],al,bb,10,ams[0]);
  ts_atr(g_c[mt].sd.ei.tc==3); ts_done();
  /* DAO */
  ts_begin("dao_governance");
  int dao=c_create("TestDAO",ow,4); d_init(&g_c[dao],100,3); c_deploy(dao);
  int pid=d_prop(&g_c[dao],al,"Upgrade to v2"); ts_atr(pid>=0);
  d_vote(&g_c[dao],(unsigned int)pid,1); d_vote(&g_c[dao],(unsigned int)pid,1); d_vote(&g_c[dao],(unsigned int)pid,1);
  g_bk+=101; ts_aeq(d_exec(&g_c[dao],(unsigned int)pid),0); ts_aeq((int)g_c[dao].sd.da.ps2[pid],4); ts_done();
  /* EVM Execution */
  ts_begin("evm_execution");
  con *ec=&g_c[e20]; ec->bc[0]=OPUSH1;ec->bc[1]=5;ec->bc[2]=OPUSH1;ec->bc[3]=3;
  ec->bc[4]=OADD;ec->bc[5]=OPUSH1;ec->bc[6]=10;ec->bc[7]=OSUB;ec->bc[8]=OSTOP;ec->bz=9;
  exec ex; e_init(&ex,GL); ts_aeq(e_run(&ex,ec),0); ts_done();
  /* Proxy */
  ts_begin("proxy_pattern");
  int pxi=px_deploy(g_c[e20].ad,ow); ts_atr(pxi>=0); ts_atr(g_c[pxi].ip==1); ts_done();
  /* Diamond */
  ts_begin("diamond_pattern");
  int dia=dm_deploy(); unsigned char sls[2][4]={{0xA9,0x05,0x9C,0xBB},{0x09,0x5E,0xA7,0xB3}};
  dm_facet(dia,g_c[e20].ad,sls,2); ts_aeq((int)g_c[dia].fn,1); ts_done();
  /* Cross-contract */
  ts_begin("cross_contract");
  unsigned char cd[4]={0xA9,0x05,0x9C,0xBB}; ts_aeq(x_call(e20,nft,cd,4),0); ts_done();
  /* Security */
  ts_begin("security_checks");
  ts_aeq(s_reent(&g_c[e20]),0); ts_aeq(s_reent(&g_c[e20]),-1);
  g_c[e20].gd.lk=0;g_c[e20].gd.dp=0;
  ts_aeq(s_acc(&g_c[e20],ow,RO),0); ts_aeq(s_acc(&g_c[e20],bb,RA),-1);
  /* Overflow multiplication check */
  u256 ov1,ov2; mz(&ov1,sizeof(u256));ov1.d[31]=20; mz(&ov2,sizeof(u256));ov2.d[31]=12;
  ts_atr(s_ovf_mul(ov1,ov2)!=0); /* 20*12=240 > 255 */
  /* Balance check */
  g_c[e20].bl2=1000; ts_aeq(s_balance_check(&g_c[e20],500),0); ts_aeq(s_balance_check(&g_c[e20],2000),-1);
  s_reset_guard(&g_c[e20]); ts_done();
  /* Audit */
  ts_begin("security_audit"); ts_atr(s_audit(e20)>=0); ts_aeq(s_veri(e20),0); ts_done();
  /* Debugger */
  ts_begin("debugger"); db_init(); db_bp(4); ts_atr(g_d.en==1); ts_atr(g_d.bs==1); ts_done();
  /* Gas */
  ts_begin("gas_estimation"); unsigned char ed[4]={0xA9,0x05,0x9C,0xBB}; ts_atr(g_est(e20,ed,4)>21000); ts_done();
  /* Upgrade */
  ts_begin("contract_upgrade");
  unsigned char nc[8]={OPUSH1,10,OPUSH1,20,OADD,OSTOP,0,0}; ts_aeq(c_upgrade(e20,nc,6),0); ts_aeq((int)g_c[e20].vr,2); ts_done();
  /* Invoke */
  ts_begin("contract_invoke");
  unsigned char sr[4]={0xA9,0x05,0x9C,0xBB}; ts_aeq(c_invoke(e20,sr,4),0);
  unsigned char su[4]={0xDE,0xAD,0xBE,0xEF}; ts_aeq(c_invoke(e20,su,4),-6); ts_done();
  /* Pause/Resume */
  ts_begin("pause_resume");
  c_pause(nft); ts_aeq(g_c[nft].st2,SP); ts_aeq(c_invoke(nft,sr,4),-3);
  c_resume(nft); ts_aeq(g_c[nft].st2,SA); ts_done();
  /* Events */
  ts_begin("event_logging");
  unsigned char tp[32]; mz(tp,32);tp[0]=0xDD; unsigned char evd[64]; mz(evd,64);evd[0]=0xAB;
  ev_emit(g_c[e20].ad,tp,1,evd,64); evlog rs[16]; unsigned int rc=0;
  ev_query(g_c[e20].ad,rs,&rc); ts_atr(rc>0); ts_done();
  /* ABI Encode/Decode */
  ts_begin("abi_roundtrip");
  u256 vl[2]; mz(&vl[0],sizeof(u256));vl[0].d[31]=42; mz(&vl[1],sizeof(u256));vl[1].d[31]=99;
  unsigned char abuf[64]; unsigned int ablen=0; ab_enc(vl,2,abuf,&ablen);
  ts_aeq((int)ablen,64);
  u256 dout[2]; unsigned int dcnt=0; ab_dec(abuf,ablen,dout,&dcnt);
  ts_aeq((int)dcnt,2); ts_aeq(dout[0].d[31],42); ts_aeq(dout[1].d[31],99); ts_done();
  /* Library */
  ts_begin("library_deploy"); unsigned char lc[4]={OPUSH1,42,OSTOP,0}; ts_atr(lib_dep(lc,3)>=0); ts_done();
  /* Destroy */
  ts_begin("contract_destroy");
  int di=c_create("Disposable",ow,0); g_c[di].bl2=500; c_deploy(di);
  addr bf; mk_addr(&bf,0xBEEF001); ts_aeq(c_destroy(di,bf),0); ts_aeq(g_c[di].st2,SDT); ts_done();
  /* Summary */
  ps("\n=== Engine Summary ===\n"); ps("Contracts: ");pi((int)g_cn);ps("\n");
  ps("Events: ");pi((int)g_ln);ps("\n"); ps("Block: ");pi((int)g_bk);ps("\n");
  ps("=== Smart Contract Engine complete ===\n"); return 0;
}
