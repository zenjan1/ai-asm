/*
 * dapp_framework.wasm - Decentralized Application Framework for AI-ASM OS
 * Modules: Wallet, DApp Browser, Chain, NFT Marketplace, DeFi, Identity
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

#define MAX_ACC 16
#define MAX_MS 4
#define MAX_HW 2
#define MAX_DAPP 32
#define MAX_PERM 64
#define MAX_TX 64
#define MAX_EVT 32
#define MAX_COLL 16
#define MAX_ORD 64
#define MAX_POOL 16
#define MAX_LEND 8
#define MAX_MINE 8
#define MAX_BR 16
#define MAX_ID 8
#define MAX_CRED 16
#define MAX_ATT 16
#define ADDR 42
#define HASH 66
#define SIG 130
#define KEY 64
#define PATH 64
#define DURL 96
#define DNAME 48
#define CAT 24
#define SYM 12
#define URI 96
#define SCH 48
#define PRF 96
#define METH 24
#define S_OK 0
#define S_ERR 1
#define S_NOMEM 2
#define S_NOTFOUND 4
#define S_DUP 5
#define S_THRESH 6
#define S_HWFAIL 7
#define S_DAPPFAIL 8
#define S_MALICIOUS 9
#define S_NOAUCTION 10
#define S_BIDLOW 11
#define S_NOPOOL 12
#define S_INSUFLIQ 13
#define S_INSUF 14
#define S_BRCHAIN 15
#define S_EXPIRED 16
#define S_VERIFY 17
#define S_AUTH 18
#define CH_ETH 1
#define CH_BSC 2
#define CH_POLY 3
#define CH_AVAX 4
#define CH_ARB 5
#define CH_SOL 6
#define P_READ 1
#define P_WRITE 2
#define P_SIGN 4
#define P_TXN 8

enum wkind { WK_EOA, WK_MNEM, WK_HW, WK_VIEW };
enum tstat { TX_PEND, TX_CONF, TX_FAIL, TX_DROP };
enum tkind { TX_XFER, TX_CALL, TX_MINT, TX_SWAP };
enum nstat { N_LIST, N_SOLD, N_CANC, N_AUCT };
enum lact { L_DEP, L_WDR, L_BOR, L_REP };
enum bstat { B_INIT, B_LOCK, B_REL, B_DONE, B_FAIL };

typedef struct { char a[ADDR]; char k[KEY]; char p[PATH];
    unsigned long long bal; unsigned int chain; enum wkind kind; } acct_t;
typedef struct { char wa[ADDR]; char own[8][ADDR]; unsigned int thr, oc;
    char ptx[16][HASH]; unsigned int pc; } msig_t;
typedef struct { char did[PATH]; char a[ADDR]; int conn; unsigned int chain; } hw_t;
typedef struct { char url[DURL]; char nm[DNAME]; char cat[CAT];
    int trust, box; } dapp_t;
typedef struct { char du[DURL]; char ori[ADDR]; unsigned int pm; int ok; } perm_t;
typedef struct { char h[HASH]; char fr[ADDR]; char to[ADDR];
    unsigned long long val, gp; char dt[DURL]; unsigned int glim, gused, nonce, cid, blk;
    enum tstat st; enum tkind kd; } tx_t;
typedef struct { char h[HASH]; char ct[ADDR]; char es[HASH];
    unsigned int blk, idx; } evt_t;
typedef struct { char col[ADDR]; unsigned int tid; char sel[ADDR];
    unsigned long long price; enum nstat st; } ord_t;
typedef struct { char col[ADDR]; char nm[DNAME]; char sym[SYM];
    unsigned int supply, roy; char cr[ADDR]; } coll_t;
typedef struct { char pa[ADDR]; char t0[SYM]; char t1[SYM];
    unsigned long long r0, r1, lp; unsigned int fee; } pool_t;
typedef struct { char pa[ADDR]; char u[ADDR];
    unsigned long long sup, bor, col, rate; } lend_t;
typedef struct { char pa[ADDR]; char u[ADDR];
    unsigned long long stk, rwd, accr; } mine_t;
typedef struct { char tid[HASH]; char sn[ADDR]; char rv[ADDR];
    unsigned long long amt, fee; unsigned int sc, dc; enum bstat st; } br_t;
typedef struct { char did[PATH]; char a[ADDR]; char doc[DURL];
    char pk[KEY]; unsigned int trust; } did_t;
typedef struct { char cid[HASH]; char iss[PATH]; char sub[PATH];
    char ct[SCH]; char cd[DURL]; char prf[PRF]; unsigned int iat, exp; } cred_t;
typedef struct { char aid[HASH]; char att[ADDR]; char sub[ADDR];
    char sch[SCH]; char dt[DURL]; int valid; } att_t;

static acct_t g_acc[MAX_ACC]; static unsigned int g_an;
static msig_t g_ms[MAX_MS]; static unsigned int g_mn;
static hw_t g_hw[MAX_HW]; static unsigned int g_hn;
static dapp_t g_dapp[MAX_DAPP]; static unsigned int g_dn;
static perm_t g_perm[MAX_PERM]; static unsigned int g_pn;
static tx_t g_tx[MAX_TX]; static unsigned int g_tn;
static evt_t g_evt[MAX_EVT]; static unsigned int g_en;
static coll_t g_coll[MAX_COLL]; static unsigned int g_cn;
static ord_t g_ord[MAX_ORD]; static unsigned int g_on;
static pool_t g_pool[MAX_POOL]; static unsigned int g_pn2;
static lend_t g_lend[MAX_LEND]; static unsigned int g_ln;
static mine_t g_mine[MAX_MINE]; static unsigned int g_mn2;
static br_t g_br[MAX_BR]; static unsigned int g_bn;
static did_t g_did[MAX_ID]; static unsigned int g_idn;
static cred_t g_cred[MAX_CRED]; static unsigned int g_crn;
static att_t g_att[MAX_ATT]; static unsigned int g_atn;
static unsigned int g_hsz, g_hpk; static unsigned long long g_nc; static unsigned int g_bl;

static unsigned int m_alloc(unsigned int sz, unsigned int al) {
    unsigned int pad = al ? ((al - (g_hsz % al)) % al) : 0;
    unsigned int off = g_hsz + pad;
    if (off + sz > 0x10000) { host_print("[MEM] OOM"); return 0; }
    g_hsz = off + sz; if (g_hsz > g_hpk) g_hpk = g_hsz; return off;
}
static void u_set(void *p, int v, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) ((unsigned char *)p)[i] = (unsigned char)v;
}
static void u_cpy(void *d, const void *s, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) ((unsigned char *)d)[i] = ((const unsigned char *)s)[i];
}
static int u_cmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; } return (unsigned char)*a - (unsigned char)*b;
}
static unsigned int u_len(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void u_str(char *d, const char *s) { while (*s) *d++ = *s++; *d = 0; }
static void u_hex(unsigned char *in, unsigned int l, char *out) {
    const char h[] = "0123456789ABCDEF";
    for (unsigned int i = 0; i < l; i++) { out[i*2]=h[(in[i]>>4)&0xF]; out[i*2+1]=h[in[i]&0xF]; }
    out[l*2] = 0;
}
static void u_hash(const char *seed, unsigned int sl, unsigned char *out, unsigned char salt) {
    for (unsigned int i = 0; i < 32; i++) { unsigned char s = salt;
        for (unsigned int j = 0; j < sl; j++) s = (unsigned char)((s*31+(unsigned char)seed[j]+i)&0xFF);
        out[i] = s;
    }
}
static void u_key(const char *seed, unsigned int sl, char *key) {
    unsigned char h[32]; u_hash(seed, sl, h, 0x5A); u_hex(h, 32, key);
}
static void u_addr(const char *key, char *addr) {
    unsigned char r[20]; addr[0]='0'; addr[1]='x';
    for (unsigned int i=0;i<20;i++) r[i]=(unsigned char)((unsigned char)key[i*2]^(unsigned char)key[i*2+1]);
    u_hex(r, 20, addr+2);
}

/* ── Wallet ───────────────────────────────────────────────────────── */

static acct_t *w_find(const char *a) {
    for (unsigned int i=0;i<g_an;i++) if (u_cmp(g_acc[i].a,a)==0) return &g_acc[i];
    return 0;
}
static int w_create(const char *path) {
    if (g_an>=MAX_ACC) return S_NOMEM; acct_t *a=&g_acc[g_an];
    char sd[64], kb[KEY+1]; kb[KEY]=0;
    u_key(path?path:"aiasm-seed", u_len(path?path:"aiasm-seed"), sd);
    u_set(sd+32, 0xAA, 32); u_key(sd, 64, kb);
    u_str(a->k, kb); u_addr(kb, a->a);
    u_str(a->p, path?path:"m/44'/60'/0'/0/0");
    a->bal=1000000000000000000ULL; a->chain=CH_ETH; a->kind=WK_EOA; g_an++;
    host_print("[WALLET] Created "); host_print(a->a); return S_OK;
}
static int w_import(const char *mn, const char *path) {
    if (!mn||g_an>=MAX_ACC) return S_NOMEM; acct_t *a=&g_acc[g_an];
    char sd[64], kb[KEY+1]; kb[KEY]=0;
    u_key(mn, u_len(mn), sd); u_set(sd+32, 0xBB, 32); u_key(sd, 64, kb);
    u_str(a->k, kb); u_addr(kb, a->a);
    u_str(a->p, path?path:"m/44'/60'/0'/0/0"); a->bal=0; a->chain=CH_ETH; a->kind=WK_MNEM; g_an++;
    host_print("[WALLET] Imported "); host_print(a->a); return S_OK;
}
static int w_sign(const char *addr, const char *msg, char *sig) {
    acct_t *a=w_find(addr); if (!a) return S_NOTFOUND;
    unsigned char h[32]; u_hash(msg?msg:"", u_len(msg?msg:""), h, 0xA5);
    u_hex(h, 32, sig);
    for (unsigned int i=0;i<32;i++) h[i]=(unsigned char)(h[i]^(unsigned char)a->k[i]);
    u_hex(h, 32, sig+64); sig[128]='1'; sig[129]=0;
    host_print("[WALLET] Signed for "); host_print(addr); return S_OK;
}
static int w_verify(const char *addr, const char *msg, const char *sig) {
    if (!w_find(addr)||!msg||!sig||u_len(sig)!=SIG) return S_VERIFY;
    char cs[SIG+1]; cs[SIG]=0; int rc=w_sign(addr,msg,cs);
    return (rc==S_OK&&u_cmp(sig,cs)==0) ? S_OK : S_VERIFY;
}
static int ms_create(char own[][ADDR], unsigned int n, unsigned int thr) {
    if (g_mn>=MAX_MS||n<1||n>8||thr<1||thr>n) return S_THRESH; msig_t *m=&g_ms[g_mn];
    for (unsigned int i=0;i<n;i++) u_str(m->own[i], own[i]);
    m->oc=n; m->thr=thr; m->pc=0;
    char sd[ADDR+16]; u_str(sd,"msig-"); unsigned int o=5;
    for (unsigned int i=0;i<n&&o<ADDR;i++) for (unsigned int j=2;j<12&&o<ADDR-1;j++) sd[o++]=own[i][j];
    sd[o]=0; char kb[KEY+1]; kb[KEY]=0; u_key(sd, o, kb); u_addr(kb, m->wa);
    g_mn++; host_print("[MSIG] Created "); host_print(m->wa); return S_OK;
}
static int ms_submit(const char *wa, const char *th) {
    for (unsigned int i=0;i<g_mn;i++) if (u_cmp(g_ms[i].wa,wa)==0) {
        if (g_ms[i].pc>=16) return S_NOMEM;
        u_str(g_ms[i].ptx[g_ms[i].pc++], th);
        host_print("[MSIG] Submitted "); host_print(th); return S_OK;
    }
    return S_NOTFOUND;
}
static int ms_confirm(const char *wa, unsigned int idx) {
    for (unsigned int i=0;i<g_mn;i++) if (u_cmp(g_ms[i].wa,wa)==0) {
        if (idx>=g_ms[i].pc) return S_NOTFOUND; g_ms[i].pc--;
        for (unsigned int j=idx;j<g_ms[i].pc;j++) u_str(g_ms[i].ptx[j],g_ms[i].ptx[j+1]);
        return S_OK;
    }
    return S_NOTFOUND;
}
static int hw_conn(const char *dev, unsigned int chain) {
    if (g_hn>=MAX_HW) return S_NOMEM; hw_t *h=&g_hw[g_hn];
    u_str(h->did, dev?dev:"hw-unknown");
    char kb[KEY+1]; kb[KEY]=0; u_key(dev?dev:"hw", 10, kb); u_addr(kb, h->a);
    h->conn=1; h->chain=chain; g_hn++;
    host_print("[HW] Connected "); host_print(h->did); return S_OK;
}
static int hw_sign(const char *dev, const char *msg, char *sig) {
    for (unsigned int i=0;i<g_hn;i++) if (u_cmp(g_hw[i].did,dev)==0) {
        if (!g_hw[i].conn) return S_HWFAIL;
        char kb[KEY+1]; kb[KEY]=0; u_key(msg?msg:"", 8, kb);
        u_hex((unsigned char*)kb, 32, sig); u_key(sig, 64, sig+64);
        sig[128]='2'; sig[129]=0; host_print("[HW] Signed via "); host_print(dev); return S_OK;
    }
    return S_NOTFOUND;
}

/* ── DApp Browser ─────────────────────────────────────────────────── */

static int d_find(const char *url) {
    for (unsigned int i=0;i<g_dn;i++) if (u_cmp(g_dapp[i].url,url)==0) return (int)i;
    return -1;
}
static int d_reg(const char *url, const char *nm, const char *ct) {
    if (g_dn>=MAX_DAPP||!url||!nm) return S_ERR;
    if (d_find(url)>=0) return S_DUP; dapp_t *d=&g_dapp[g_dn];
    u_str(d->url,url); u_str(d->nm,nm); u_str(d->cat,ct?ct:"defi");
    d->trust=0; d->box=0; g_dn++;
    host_print("[DAPP] Registered "); host_print(nm); return S_OK;
}
static int d_perm(const char *url, const char *ori, unsigned int pm) {
    for (unsigned int i=0;i<g_pn;i++) if (u_cmp(g_perm[i].du,url)==0&&u_cmp(g_perm[i].ori,ori)==0) {
        g_perm[i].pm|=pm; g_perm[i].ok=1; return S_OK;
    }
    if (g_pn>=MAX_PERM) return S_NOMEM; perm_t *p=&g_perm[g_pn];
    u_str(p->du,url); u_str(p->ori,ori); p->pm=pm; p->ok=1; g_pn++; return S_OK;
}
static int d_disc(const char *ct, char *out, unsigned int mx) {
    unsigned int p=0;
    for (unsigned int i=0;i<g_dn&&p<mx-DNAME-2;i++) {
        if (!ct||u_cmp(g_dapp[i].cat,ct)==0) {
            for (unsigned int j=0;g_dapp[i].nm[j]&&p<mx-1;j++) out[p++]=g_dapp[i].nm[j];
            out[p++]=',';
        }
    }
    if (p>0) p--; out[p]=0; return (int)p;
}
static int d_box(const char *url) {
    int i=d_find(url); if (i<0) return S_DAPPFAIL; dapp_t *d=&g_dapp[i];
    if (u_cmp(d->nm,"malicious")==0) return S_MALICIOUS;
    d->box=1;
    if (u_cmp(d->nm,"verified-dex")==0||u_cmp(d->nm,"nft-gallery")==0) d->trust=1;
    host_print("[DAPP] Sandbox "); host_print(d->nm); return S_OK;
}

/* ── Chain Interaction ────────────────────────────────────────────── */

static unsigned int c_estgas(const char *fr, const char *to, unsigned long long v, const char *dt) {
    (void)fr; (void)to; unsigned int g=21000;
    if (dt) g+=u_len(dt)*16; if (v>0) g+=9000; return g;
}
static int c_emit(const char *th, const char *ct, const char *es) {
    if (g_en>=MAX_EVT) return S_NOMEM; evt_t *e=&g_evt[g_en];
    u_str(e->h,th); u_str(e->ct,ct); u_str(e->es,es); e->blk=g_bl; e->idx=g_en++; return S_OK;
}
static int c_tx(const char *fr, const char *to, unsigned long long v, const char *dt, unsigned int gas) {
    if (g_tn>=MAX_TX) return S_NOMEM; tx_t *t=&g_tx[g_tn];
    u_str(t->fr,fr?fr:"0x0"); u_str(t->to,to?to:"0x0"); t->val=v;
    if (dt) u_str(t->dt,dt); else t->dt[0]=0;
    unsigned int est=c_estgas(fr,to,v,dt); t->glim=gas>est?gas:est; t->gp=20000000000ULL;
    t->st=TX_PEND; t->kd=TX_XFER; t->nonce=(unsigned int)g_nc++; t->cid=CH_ETH; t->blk=g_bl; t->gused=0;
    char sd[ADDR*2+16]; u_str(sd,fr?fr:"x"); unsigned int sl=u_len(sd); sd[sl++]=':';
    for (unsigned int i=0;to&&to[i]&&sl<sizeof(sd)-1;i++) sd[sl++]=to[i]; sd[sl]=0;
    unsigned char h[32]; u_hash(sd, sl, h, 0x42);
    t->h[0]='0'; t->h[1]='x'; u_hex(h, 32, t->h+2);
    g_tn++; g_bl++; host_print("[CHAIN] TX "); host_print(t->h); return S_OK;
}
static int c_call(const char *ct, const char *fn, const char *args) {
    char cd[DURL]; unsigned int p=0;
    if (fn) for (unsigned int i=0;fn[i]&&p<10;i++) cd[p++]=fn[i];
    if (args) for (unsigned int i=0;args[i]&&p<DURL-1;i++) cd[p++]=args[i];
    cd[p]=0; int rc=c_tx("0x0",ct,0,cd,0);
    if (rc==S_OK) { g_tx[g_tn-1].kd=TX_CALL; c_emit(g_tx[g_tn-1].h,ct,"ContractCall()"); }
    return rc;
}
static int c_listen(const char *ct, const char *ev) {
    char sig[CAT+8]; unsigned int p=0;
    if (ev) for (unsigned int i=0;ev[i]&&p<CAT+4;i++) sig[p++]=ev[i];
    sig[p++]='('; sig[p++]=')'; sig[p]=0; return c_emit("0x0",ct,sig);
}
static int c_receipt(const char *th, tx_t *out) {
    for (unsigned int i=0;i<g_tn;i++) if (u_cmp(g_tx[i].h,th)==0) {
        if (out) u_cpy(out, &g_tx[i], sizeof(tx_t)); return S_OK;
    }
    return S_NOTFOUND;
}

/* ── NFT Marketplace ──────────────────────────────────────────────── */

static int n_find(const char *col, unsigned int tid) {
    for (unsigned int i=0;i<g_on;i++)
        if (u_cmp(g_ord[i].col,col)==0&&g_ord[i].tid==tid) return (int)i;
    return -1;
}
static int n_coll(const char *nm, const char *sym, unsigned int roy, const char *cr) {
    if (g_cn>=MAX_COLL) return S_NOMEM; coll_t *c=&g_coll[g_cn];
    u_str(c->nm,nm); u_str(c->sym,sym); c->supply=0; c->roy=roy; u_str(c->cr,cr);
    char kb[KEY+1]; kb[KEY]=0; u_key(nm,u_len(nm),kb); u_addr(kb,c->col);
    g_cn++; host_print("[NFT] Collection "); host_print(nm); return S_OK;
}
static int n_mint(const char *col, unsigned int tid, const char *uri, const char *to) {
    (void)tid; (void)uri; (void)to;
    for (unsigned int i=0;i<g_cn;i++) if (u_cmp(g_coll[i].col,col)==0) {
        g_coll[i].supply++; host_print("[NFT] Minted "); return S_OK;
    }
    return S_NOTFOUND;
}
static int n_list(const char *col, unsigned int tid, unsigned long long pr, const char *sel) {
    if (g_on>=MAX_ORD||n_find(col,tid)>=0) return S_DUP; ord_t *o=&g_ord[g_on];
    u_str(o->col,col); o->tid=tid; u_str(o->sel,sel); o->price=pr; o->st=N_LIST; g_on++;
    host_print("[NFT] Listed "); return S_OK;
}
static int n_auct(const char *col, unsigned int tid, unsigned long long sp, unsigned int dur, const char *sel) {
    (void)dur; if (g_on>=MAX_ORD||n_find(col,tid)>=0) return S_DUP; ord_t *o=&g_ord[g_on];
    u_str(o->col,col); o->tid=tid; u_str(o->sel,sel); o->price=sp; o->st=N_AUCT; g_on++;
    host_print("[NFT] Auction "); return S_OK;
}
static int n_bid(const char *col, unsigned int tid, unsigned long long amt, const char *bid) {
    (void)bid; int i=n_find(col,tid); if (i<0) return S_NOAUCTION;
    if (g_ord[i].st!=N_AUCT) return S_NOAUCTION;
    if (amt<=g_ord[i].price) return S_BIDLOW; g_ord[i].price=amt;
    host_print("[NFT] Bid "); return S_OK;
}
static int n_buy(const char *col, unsigned int tid, const char *buy) {
    (void)buy; int i=n_find(col,tid); if (i<0) return S_NOTFOUND;
    if (g_ord[i].st!=N_LIST&&g_ord[i].st!=N_AUCT) return S_NOTFOUND;
    g_ord[i].st=N_SOLD; host_print("[NFT] Sold "); return S_OK;
}
static int n_canc(const char *col, unsigned int tid) {
    int i=n_find(col,tid); if (i<0) return S_NOTFOUND;
    g_ord[i].st=N_CANC; host_print("[NFT] Cancelled "); return S_OK;
}

/* ── DeFi Protocols ───────────────────────────────────────────────── */

static pool_t *p_find(const char *pa) {
    for (unsigned int i=0;i<g_pn2;i++) if (u_cmp(g_pool[i].pa,pa)==0) return &g_pool[i];
    return 0;
}
static lend_t *l_find(const char *pa, const char *u) {
    for (unsigned int i=0;i<g_ln;i++) if (u_cmp(g_lend[i].pa,pa)==0&&u_cmp(g_lend[i].u,u)==0) return &g_lend[i];
    return 0;
}
static mine_t *mi_find(const char *pa, const char *u) {
    for (unsigned int i=0;i<g_mn2;i++) if (u_cmp(g_mine[i].pa,pa)==0&&u_cmp(g_mine[i].u,u)==0) return &g_mine[i];
    return 0;
}
static int p_create(const char *t0, const char *t1, unsigned long long a0, unsigned long long a1, unsigned int fee) {
    if (g_pn2>=MAX_POOL) return S_NOMEM;
    for (unsigned int i=0;i<g_pn2;i++) if (u_cmp(g_pool[i].t0,t0)==0&&u_cmp(g_pool[i].t1,t1)==0) return S_DUP;
    pool_t *p=&g_pool[g_pn2]; u_str(p->t0,t0); u_str(p->t1,t1);
    p->r0=a0; p->r1=a1; p->lp=a0+a1; p->fee=fee;
    char kb[KEY+1]; kb[KEY]=0; u_key(t0,u_len(t0),kb); u_addr(kb,p->pa);
    g_pn2++; host_print("[DEFI] Pool "); host_print(p->pa); return S_OK;
}
static int p_swap(const char *pa, const char *ti, const char *to, unsigned long long amt) {
    (void)to; pool_t *p=p_find(pa); if (!p||amt==0) return S_NOPOOL;
    unsigned long long fa=amt*(10000-p->fee)/10000, oa;
    if (u_cmp(ti,p->t0)==0) { if (!p->r1) return S_INSUFLIQ;
        oa=(p->r1*fa)/(p->r0+fa); if (oa>p->r1) return S_INSUFLIQ; p->r0+=amt; p->r1-=oa;
    } else { if (!p->r0) return S_INSUFLIQ;
        oa=(p->r0*fa)/(p->r1+fa); if (oa>p->r0) return S_INSUFLIQ; p->r1+=amt; p->r0-=oa;
    }
    g_bl++; host_print("[DEFI] Swap "); return S_OK;
}
static int p_liq_add(const char *pa, const char *u, unsigned long long a0, unsigned long long a1) {
    (void)u; pool_t *p=p_find(pa); if (!p) return S_NOPOOL;
    p->r0+=a0; p->r1+=a1; p->lp+=a0+a1; host_print("[DEFI] Liq+ "); return S_OK;
}
static int p_liq_rm(const char *pa, const char *u, unsigned long long lp) {
    (void)u; pool_t *p=p_find(pa); if (!p) return S_NOPOOL;
    if (lp>p->lp) return S_INSUF; p->r0-=(p->r0*lp)/p->lp; p->r1-=(p->r1*lp)/p->lp; p->lp-=lp;
    host_print("[DEFI] Liq- "); return S_OK;
}
static int d_lend(const char *pa, const char *u, unsigned long long amt, int act) {
    lend_t *l=l_find(pa,u);
    if (!l) { if (g_ln>=MAX_LEND) return S_NOMEM; l=&g_lend[g_ln++];
        u_str(l->pa,pa); u_str(l->u,u); l->sup=0; l->bor=0; l->col=0; l->rate=500;
    }
    if (act==L_DEP) l->sup+=amt;
    else if (act==L_WDR) { if (amt>l->sup) return S_INSUF; l->sup-=amt; }
    else if (act==L_BOR) { if (amt>l->col*75/100) return S_INSUF; l->bor+=amt; }
    else if (act==L_REP) { if (amt>l->bor) return S_INSUF; l->bor-=amt; }
    host_print("[DEFI] Lend "); return S_OK;
}
static int d_stake(const char *pa, const char *u, unsigned long long amt) {
    mine_t *m=mi_find(pa,u);
    if (!m) { if (g_mn2>=MAX_MINE) return S_NOMEM; m=&g_mine[g_mn2++];
        u_str(m->pa,pa); u_str(m->u,u); m->stk=0; m->rwd=0; m->accr=0;
    }
    m->stk+=amt; host_print("[DEFI] Stake "); return S_OK;
}
static int d_harvest(const char *pa, const char *u) {
    mine_t *m=mi_find(pa,u); if (!m) return S_NOTFOUND;
    m->accr+=m->rwd; m->rwd=0; host_print("[DEFI] Harvest "); return S_OK;
}
static int d_bridge(const char *sn, const char *rv, unsigned long long amt, unsigned int sc, unsigned int dc) {
    if (g_bn>=MAX_BR||sc==dc||sc<CH_ETH||sc>CH_SOL||dc<CH_ETH||dc>CH_SOL) return S_BRCHAIN;
    br_t *b=&g_br[g_bn]; u_str(b->sn,sn); u_str(b->rv,rv);
    b->fee=amt*10/10000; b->amt=amt-b->fee; b->sc=sc; b->dc=dc; b->st=B_INIT;
    char sd[ADDR*2+16]; u_str(sd,sn); unsigned int sl=u_len(sd); sd[sl++]=':';
    for (unsigned int i=0;rv[i]&&sl<sizeof(sd)-1;i++) sd[sl++]=rv[i]; sd[sl]=0;
    unsigned char h[32]; u_hash(sd, sl, h, 0xB0);
    b->tid[0]='0'; b->tid[1]='x'; u_hex(h, 32, b->tid+2);
    g_bn++; host_print("[BRIDGE] Init "); return S_OK;
}

/* ── Identity ─────────────────────────────────────────────────────── */

static did_t *id_find(const char *did) {
    for (unsigned int i=0;i<g_idn;i++) if (u_cmp(g_did[i].did,did)==0) return &g_did[i];
    return 0;
}
static int id_create(const char *addr, const char *method) {
    if (g_idn>=MAX_ID) return S_NOMEM; did_t *d=&g_did[g_idn];
    const char *mt=method?method:"aiasm"; unsigned int ml=u_len(mt);
    u_str(d->did,"did:"); u_str(d->did+4,mt);
    unsigned int o=4+ml; d->did[o]=':'; o++;
    for (unsigned int i=2;addr[i]&&o<PATH-1;i++) d->did[o++]=addr[i]; d->did[o]=0;
    u_str(d->a,addr); u_str(d->doc,"{\"id\":\""); unsigned int dp=7;
    for (unsigned int i=0;d->did[i]&&dp<DURL-20;i++) d->doc[dp++]=d->did[i];
    u_str(d->doc+dp,"\"}"); u_key(addr,u_len(addr),d->pk); d->trust=50; g_idn++;
    host_print("[ID] DID "); host_print(d->did); return S_OK;
}
static int id_cred(const char *iss, const char *sub, const char *ct, const char *data, unsigned int exp) {
    if (g_crn>=MAX_CRED) return S_NOMEM; cred_t *c=&g_cred[g_crn];
    char sd[ADDR*2+16]; u_str(sd,iss); unsigned int sl=u_len(sd); sd[sl++]=':';
    for (unsigned int i=0;sub[i]&&sl<sizeof(sd)-1;i++) sd[sl++]=sub[i]; sd[sl]=0;
    unsigned char h[32]; u_hash(sd, sl, h, 0xC0);
    c->cid[0]='0'; c->cid[1]='x'; u_hex(h, 32, c->cid+2);
    u_str(c->iss,iss); u_str(c->sub,sub); u_str(c->ct,ct); u_str(c->cd,data);
    u_key(c->cid, 66, c->prf); c->iat=g_bl; c->exp=exp; g_crn++;
    host_print("[ID] Cred issued "); return S_OK;
}
static int id_verify(const char *sub, const char *ct, const char *prf) {
    for (unsigned int i=0;i<g_crn;i++)
        if (u_cmp(g_cred[i].sub,sub)==0&&u_cmp(g_cred[i].ct,ct)==0) {
            if (g_cred[i].exp>0&&g_bl>g_cred[i].exp) return S_EXPIRED;
            char ck[PRF]; u_key(g_cred[i].cid, 66, ck);
            if (prf&&u_cmp(prf,ck)==0) { host_print("[ID] Verified "); return S_OK; }
            return S_VERIFY;
        }
    return S_NOTFOUND;
}
static int id_auth(const char *did, const char *ch, const char *resp) {
    did_t *d=id_find(did); if (!d||!ch||!resp) return S_AUTH;
    char exp[SIG+1]; exp[SIG]=0; u_key(ch,u_len(ch),exp);
    if (u_cmp(resp,exp)==0) { d->trust+=2; if (d->trust>100) d->trust=100;
        host_print("[ID] Auth OK "); return S_OK;
    }
    if (d->trust>5) d->trust-=5; return S_AUTH;
}
static int id_rep(const char *did, int delta) {
    did_t *d=id_find(did); if (!d) return S_NOTFOUND;
    int s=(int)d->trust+delta; if (s<0) s=0; if (s>100) s=100; d->trust=(unsigned int)s;
    host_print("[ID] Rep updated "); return S_OK;
}
static int id_attest(const char *att, const char *sub, const char *sch, const char *dt) {
    if (g_atn>=MAX_ATT) return S_NOMEM; att_t *a=&g_att[g_atn];
    char sd[ADDR*2+16]; u_str(sd,att); unsigned int sl=u_len(sd); sd[sl++]=':';
    for (unsigned int i=0;sub[i]&&sl<sizeof(sd)-1;i++) sd[sl++]=sub[i]; sd[sl]=0;
    unsigned char h[32]; u_hash(sd, sl, h, 0xD0);
    a->aid[0]='0'; a->aid[1]='x'; u_hex(h, 32, a->aid+2);
    u_str(a->att,att); u_str(a->sub,sub); u_str(a->sch,sch); u_str(a->dt,dt); a->valid=1; g_atn++;
    host_print("[ID] Attestation "); return S_OK;
}

/* ── Demo ─────────────────────────────────────────────────────────── */

static void demo(void) {
    host_print("=== Wallet ===");
    w_create("m/44'/60'/0'/0/0"); w_create("m/44'/60'/0'/0/1");
    w_import("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about", 0);
    char sig[SIG+1]; sig[SIG]=0; w_sign(g_acc[0].a, "hello-aiasm", sig);
    char own[2][ADDR]; u_str(own[0],g_acc[0].a); u_str(own[1],g_acc[1].a);
    ms_create(own, 2, 2); hw_conn("ledger-nano", CH_ETH);
    host_print("=== DApps ===");
    d_reg("https://app.uniswap.org", "verified-dex", "defi");
    d_reg("https://opensea.io", "nft-gallery", "nft");
    d_box("https://app.uniswap.org");
    char buf[128]; d_disc("defi", buf, sizeof(buf));
    d_perm("https://app.uniswap.org", g_acc[0].a, P_READ|P_TXN);
    host_print("=== Chain ===");
    c_tx(g_acc[0].a, g_acc[1].a, 1000000000000000ULL, 0, 21000);
    c_call("0xContract", "transfer", "0xTo,1000");
    c_listen("0xContract", "Transfer");
    tx_t rcpt; c_receipt(g_tx[0].h, &rcpt);
    host_print("=== NFT ===");
    n_coll("AI-Art", "AIART", 500, g_acc[0].a);
    n_mint(g_coll[0].col, 1, "ipfs://art1", g_acc[0].a);
    n_mint(g_coll[0].col, 2, "ipfs://art2", g_acc[0].a);
    n_list(g_coll[0].col, 1, 500000000000000000ULL, g_acc[0].a);
    n_auct(g_coll[0].col, 2, 100000000000000000ULL, 3600, g_acc[0].a);
    n_bid(g_coll[0].col, 2, 150000000000000000ULL, g_acc[1].a);
    n_buy(g_coll[0].col, 1, g_acc[1].a);
    host_print("=== DeFi ===");
    p_create("WETH", "USDC", 1000000000000000000ULL, 2000000000ULL, 30);
    p_swap(g_pool[0].pa, "USDC", "WETH", 500000000ULL);
    p_liq_add(g_pool[0].pa, g_acc[0].a, 500000000000000000ULL, 1000000000ULL);
    d_stake(g_pool[0].pa, g_acc[0].a, 100000000000000000ULL);
    d_lend(g_pool[0].pa, g_acc[0].a, 500000000000000000ULL, L_DEP);
    d_bridge(g_acc[0].a, g_acc[1].a, 100000000000000000ULL, CH_ETH, CH_POLY);
    host_print("=== Identity ===");
    id_create(g_acc[0].a, "aiasm");
    id_cred(g_did[0].did, g_did[0].did, "kyc-level1", "{\"level\":1}", g_bl+10000);
    id_attest(g_acc[0].a, g_acc[1].a, "trust-v1", "{\"score\":85}");
    id_rep(g_did[0].did, 10);
    host_print("=== Done ===");
}

__attribute__((export_name("main")))
int main(void) {
    host_print("AI-ASM DApp Framework v1.0");
    g_bl=1; g_nc=0; g_hsz=0; g_hpk=0;
    demo();
    host_print("Framework init complete");
    return 0;
}
