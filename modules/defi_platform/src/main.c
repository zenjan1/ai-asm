/*
 * defi_platform: Decentralized Finance Platform (v1.0)
 * AI-ASM OS - DEX, lending, yield, derivatives, bridge, oracles.
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
/* ===== Constants ===== */
#define MAX_POOLS  16
#define MAX_ORDERS 32
#define MAX_LEND   16
#define MAX_VAULTS 12
#define MAX_PERP    8
#define MAX_CDP     8
#define MAX_HTLC    8
#define MAX_FEED   24
#define MAX_ORACLE 16
#define MAX_NODE    8
#define MAX_STRAT   8
#define MAX_HIST   32
#define FEE_BPS    30    /* DEX swap fee: 0.30% */
#define LIQ_BONUS 500    /* Liquidation bonus: 5% */
#define COL_RATIO 15000  /* Min collateral ratio: 150% */
#define HLTH_LIQ  10000  /* Health factor liquidation threshold */
#define FLASH_BPS   9    /* Flash loan fee: 0.09% */
#define VAULT_FEE  50    /* Vault withdrawal fee: 0.50% */
#define BRG_FEE    20    /* Bridge fee: 0.20% */
#define STAKE_MIN 1000   /* Min oracle node stake */
/* ===== Forward declarations ===== */
static unsigned int my_strlen(const char *s);
static void ps(const char *s); static void pi(int v); static void ph(const unsigned char *d, unsigned int l);
static void *am(unsigned int sz); static void mz(void *p, unsigned int n);
static void mc(void *d, const void *s, unsigned int n); static int mm(const void *a, const void *b, unsigned int n);
static unsigned int rng(void); static void hdata(const unsigned char *d, unsigned int l, unsigned char *o);
static int isqrt(unsigned int n); static unsigned int uabs(int v);
/* AMM / DEX */
static int amm_create(unsigned int ta, unsigned int tb, unsigned int ra, unsigned int rb);
static int amm_swap(unsigned int pid, unsigned int ti, unsigned int ai, unsigned int mo);
static int amm_add_lq(unsigned int pid, unsigned int aa, unsigned int ab);
static int amm_rm_lq(unsigned int pid, unsigned int la);
static int amm_spot(unsigned int pid, unsigned int ta, unsigned int tb);
static int amm_impact(unsigned int pid, unsigned int ti, unsigned int ai);
static int ob_limit(unsigned int pid, int sd, unsigned int px, unsigned int q);
static int ob_market(unsigned int pid, int sd, unsigned int q); static int ob_match(unsigned int pid);
static int flash_swap(unsigned int pid, unsigned int ti, unsigned int a);
/* Lending */
static int lend_col(unsigned int uid, unsigned int as, unsigned int a);
static int lend_borr(unsigned int uid, unsigned int as, unsigned int a);
static int lend_repay(unsigned int uid, unsigned int as, unsigned int a);
static int lend_hlth(unsigned int uid); static int lend_liquidate(unsigned int uid, unsigned int kp);
static int lend_rate(unsigned int as, unsigned int u);
static int cdp_open(unsigned int uid, unsigned int ca, unsigned int cm, unsigned int dm);
static int cdp_close(unsigned int id); static int flash_loan(unsigned int as, unsigned int a, unsigned int uid);
/* Yield */
static int v_create(const char *n, unsigned int as, unsigned int si);
static int v_dep(unsigned int vid, unsigned int uid, unsigned int a);
static int v_wdr(unsigned int vid, unsigned int uid, unsigned int sh);
static int v_harv(unsigned int vid); static int v_apy(unsigned int vid);
static int s_reg(unsigned int proto, unsigned int pool, unsigned int ap);
static int s_opt(unsigned int vid); static int s_route(unsigned int vid);
static int y_track(unsigned int vid, unsigned int ts, int d);
/* Derivatives */
static int perp_open(unsigned int uid, unsigned int ai, int sd, unsigned int mg, unsigned int lv);
static int perp_close(unsigned int id); static int perp_fund(unsigned int id); static int perp_mark(unsigned int id);
static int opt_px(unsigned int s, unsigned int k, unsigned int t, unsigned int v, int c);
static int opt_grk(unsigned int s, unsigned int k, unsigned int t, unsigned int v, int c);
static int synth_mk(unsigned int ai, unsigned int ca, unsigned int sa);
/* Bridge */
static int brg_lock(unsigned int sc, unsigned int dc, unsigned int as, unsigned int a, const char *r);
static int brg_burn(unsigned int ci, unsigned int as, unsigned int a, const char *r);
static int htlc_mk(unsigned int ch, unsigned int a, const unsigned char *h, unsigned int ex);
static int htlc_red(unsigned int id, const unsigned char *s);
static int brg_msig(const unsigned char *d, unsigned int dl, unsigned int th);
/* Oracle */
static int ora_feed(unsigned int fi, unsigned int p, unsigned int ts);
static int ora_agg(unsigned int fi); static int ora_twap(unsigned int fi, unsigned int w);
static int ora_vrf(unsigned int sd); static int ora_outlier(unsigned int fi, unsigned int v);
static int ora_node(unsigned int ni, unsigned int st); static int ora_slash(unsigned int ni, unsigned int a);
/* ===== Data Structures ===== */
typedef struct { unsigned int ta, tb; unsigned int ra, rb; unsigned int lpt, fee, act; } pool_t;
typedef struct { unsigned int pid; int sd; unsigned int px, q, fl, act; } order_t;
typedef struct { unsigned int uid, ca, cm, da, dm, br, act; } lend_t;
typedef struct { unsigned int uid, ca, cm, dm, fa, act; } cdp_t;
typedef struct { unsigned int as, ts, tb, sr, br, ut; } mkt_t;
typedef struct { char nm[24]; unsigned int as, td, ts, si, lh, ay, act; } vault_t;
typedef struct { unsigned int pi, pl, ap, al, act; } strat_t;
typedef struct { unsigned int vid, ts; int dt; } yhist_t;
typedef struct { unsigned int uid, ai; int sd; unsigned int mg, lv, ep, sz, act; } perp_t;
typedef struct { unsigned int ch, as, a; unsigned char h[32]; unsigned int ex, act; } htlc_t;
typedef struct { unsigned int p, ts, ni, act; } feed_t;
typedef struct { unsigned int fi; unsigned int pr[MAX_HIST], tm[MAX_HIST]; unsigned int cnt, ap; } agg_t;
typedef struct { unsigned int ni, st, rp, sl, act; } node_t;
/* ===== Global State ===== */
static pool_t  g_pool[MAX_POOLS];  static unsigned int g_pn = 0;
static order_t g_ord[MAX_ORDERS];  static unsigned int g_on = 0;
static lend_t  g_lend[MAX_LEND];   static unsigned int g_ln = 0;
static cdp_t   g_cdp[MAX_CDP];     static unsigned int g_cn = 0;
static mkt_t   g_mkt[MAX_LEND];    static unsigned int g_mn = 0;
static vault_t g_vlt[MAX_VAULTS];  static unsigned int g_vn = 0;
static strat_t g_str[MAX_STRAT];   static unsigned int g_sn = 0;
static yhist_t g_yh[MAX_HIST];     static unsigned int g_yn = 0;
static perp_t  g_perp[MAX_PERP];   static unsigned int g_pern = 0;
static htlc_t  g_ht[MAX_HTLC];     static unsigned int g_hn = 0;
static feed_t  g_fd[MAX_FEED];     static unsigned int g_fn = 0;
static agg_t   g_agg[MAX_ORACLE];  static unsigned int g_an = 0;
static node_t  g_nd[MAX_NODE];     static unsigned int g_nn = 0;
static unsigned int g_rng = 0xDEADBEEF, g_ts = 1700000000, g_bfee = 0, g_ffee = 0;
/* ===== Helpers ===== */
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[16], o[16]; int i = 0, neg = 0; unsigned int uv;
    if (v < 0) { neg = 1; uv = (unsigned int)(-(v + 1)) + 1; } else { uv = (unsigned int)v; }
    if (uv == 0) { b[i++] = '0'; }
    while (uv > 0) { b[i++] = '0' + (char)(uv % 10); uv /= 10; }
    if (neg) b[i++] = '-';
    { int j; for (j = 0; j < i; j++) o[j] = b[i - 1 - j]; o[i] = 0; }
    host_print(o);
}
static void ph(const unsigned char *d, unsigned int l) {
    static const char hx[] = "0123456789ABCDEF";
    char buf[128]; unsigned int i;
    for (i = 0; i < l && i < 64; i++) { buf[i*2] = hx[d[i]>>4]; buf[i*2+1] = hx[d[i]&0xF]; }
    buf[(l < 64 ? l : 64) * 2] = 0; host_print(buf);
}
static void *am(unsigned int sz) {
    unsigned int p = host_alloc(sz, 8);
    if (!p) { ps("FATAL: alloc failed\n"); host_exit(1); } return (void *)p;
}
static void mz(void *p, unsigned int n) { unsigned char *b = (unsigned char *)p; unsigned int i; for (i = 0; i < n; i++) b[i] = 0; }
static void mc(void *d, const void *s, unsigned int n) {
    unsigned char *dd = (unsigned char *)d; const unsigned char *ss = (const unsigned char *)s; unsigned int i;
    for (i = 0; i < n; i++) dd[i] = ss[i];
}
static int mm(const void *a, const void *b, unsigned int n) {
    const unsigned char *aa = (const unsigned char *)a; const unsigned char *bb = (const unsigned char *)b; unsigned int i;
    for (i = 0; i < n; i++) { if (aa[i] != bb[i]) return (int)aa[i] - (int)bb[i]; } return 0;
}
static unsigned int rng(void) { g_rng ^= g_rng << 13; g_rng ^= g_rng >> 17; g_rng ^= g_rng << 5; return g_rng; }
static void hdata(const unsigned char *d, unsigned int l, unsigned char *o) {
    unsigned int h = 0x811C9DC5, i;
    for (i = 0; i < l; i++) { h ^= d[i]; h *= 0x01000193; }
    o[0] = (unsigned char)(h>>24); o[1] = (unsigned char)(h>>16);
    o[2] = (unsigned char)(h>>8);  o[3] = (unsigned char)h;
    o[4] = (unsigned char)((h^0xDEADBEEF)>>24); o[5] = (unsigned char)((h^0xDEADBEEF)>>16);
    o[6] = (unsigned char)((h^0xDEADBEEF)>>8);  o[7] = (unsigned char)(h^0xDEADBEEF);
}
static int isqrt(unsigned int n) {
    unsigned int r = 0, b = 0x10000;
    while (b > n) b >>= 1;
    while (b) { if (n >= r + b) { n -= r + b; r = (r >> 1) + b; } else { r >>= 1; } b >>= 1; }
    return (int)r;
}
static unsigned int uabs(int v) { return v < 0 ? (unsigned int)(-(v + 1)) + 1 : (unsigned int)v; }
/* ===== 1. DEX: AMM + Order Book + Flash Swaps ===== */
static int amm_create(unsigned int ta, unsigned int tb, unsigned int ra, unsigned int rb) {
    unsigned int i; if (g_pn >= MAX_POOLS) return -1; i = g_pn++;
    g_pool[i].ta = ta; g_pool[i].tb = tb; g_pool[i].ra = ra; g_pool[i].rb = rb;
    g_pool[i].lpt = isqrt(ra * rb); g_pool[i].fee = 0; g_pool[i].act = 1;
    ps("[AMM] Pool T"); pi(ta); ps("/T"); pi(tb); ps(" R="); pi(ra); ps("/"); pi(rb);
    ps(" LP="); pi(g_pool[i].lpt); ps("\n"); return (int)i;
}
static int amm_swap(unsigned int pid, unsigned int ti, unsigned int ai, unsigned int mo) {
    pool_t *p; unsigned int af, nr, ao;
    if (pid >= g_pn || !g_pool[pid].act) return -1;
    p = &g_pool[pid]; af = ai - (ai * FEE_BPS / 10000);
    if (ti == p->ta) {
        nr = (p->ra * p->rb + af - 1) / af; ao = p->rb - nr;
        if (ao < mo) { ps("[AMM] Slippage\n"); return -2; } p->ra += ai; p->rb -= ao;
    } else if (ti == p->tb) {
        nr = (p->ra * p->rb + af - 1) / af; ao = p->ra - nr;
        if (ao < mo) { ps("[AMM] Slippage\n"); return -2; } p->rb += ai; p->ra -= ao;
    } else return -3;
    p->fee += ai * FEE_BPS / 10000;
    ps("[AMM] Swap in="); pi(ai); ps(" out="); pi(ao); ps(" fee="); pi(ai * FEE_BPS / 10000); ps("\n");
    return (int)ao;
}
static int amm_add_lq(unsigned int pid, unsigned int aa, unsigned int ab) {
    pool_t *p; unsigned int lm;
    if (pid >= g_pn || !g_pool[pid].act || !aa || !ab) return -1;
    p = &g_pool[pid]; lm = isqrt(aa * ab);
    p->ra += aa; p->rb += ab; p->lpt += lm;
    ps("[AMM] Liq+="); pi(aa); ps("/"); pi(ab); ps(" LP="); pi(lm); ps("\n"); return (int)lm;
}
static int amm_rm_lq(unsigned int pid, unsigned int la) {
    pool_t *p; unsigned int oa, ob;
    if (pid >= g_pn || !g_pool[pid].act || la > g_pool[pid].lpt) return -1;
    p = &g_pool[pid]; oa = (p->ra * la) / p->lpt; ob = (p->rb * la) / p->lpt;
    p->ra -= oa; p->rb -= ob; p->lpt -= la;
    ps("[AMM] Liq-="); pi(la); ps(" out="); pi(oa); ps("/"); pi(ob); ps("\n"); return 0;
}
static int amm_spot(unsigned int pid, unsigned int ta, unsigned int tb) {
    pool_t *p; if (pid >= g_pn) return -1; p = &g_pool[pid];
    if (ta == p->ta && tb == p->tb) return (int)((p->rb * 10000) / p->ra);
    if (ta == p->tb && tb == p->ta) return (int)((p->ra * 10000) / p->rb); return -1;
}
static int amm_impact(unsigned int pid, unsigned int ti, unsigned int ai) {
    pool_t *p; unsigned int sp, rn, rk, ep;
    if (pid >= g_pn) return -1; p = &g_pool[pid]; sp = (p->rb * 10000) / p->ra;
    if (ti == p->ta) { rn = p->ra + ai; rk = (p->ra * p->rb) / rn; ep = ((p->rb - rk) * 10000) / ai; }
    else return -2;
    return (int)(sp - ep);
}
static int ob_limit(unsigned int pid, int sd, unsigned int px, unsigned int q) {
    unsigned int i; if (g_on >= MAX_ORDERS) return -1; i = g_on++;
    g_ord[i].pid = pid; g_ord[i].sd = sd; g_ord[i].px = px;
    g_ord[i].q = q; g_ord[i].fl = 0; g_ord[i].act = 1;
    ps("[OB] Limit "); ps(sd ? "SELL" : "BUY"); ps(" px="); pi(px); ps(" q="); pi(q); ps("\n");
    return (int)i;
}
static int ob_market(unsigned int pid, int sd, unsigned int q) {
    ps("[OB] Market "); ps(sd ? "SELL" : "BUY"); ps(" q="); pi(q); ps("\n"); return ob_match(pid);
}
static int ob_match(unsigned int pid) {
    unsigned int i, m = 0;
    for (i = 0; i < g_on; i++)
        if (g_ord[i].pid == pid && g_ord[i].act) { g_ord[i].fl = g_ord[i].q; g_ord[i].act = 0; m++; }
    ps("[OB] Matched "); pi(m); ps("\n"); return (int)m;
}
static int flash_swap(unsigned int pid, unsigned int ti, unsigned int a) {
    unsigned int f = (a * FLASH_BPS) / 10000;
    if (pid >= g_pn) return -1; g_ffee += f;
    ps("[FLASH] Borrow "); pi(a); ps(" fee="); pi(f); ps("\n"); return (int)f;
}
/* ===== 2. Lending: Over-Collateralized Loans, CDPs, Flash Loans ===== */
static int lend_col(unsigned int uid, unsigned int as, unsigned int a) {
    unsigned int i; if (g_ln >= MAX_LEND) return -1; i = g_ln++;
    g_lend[i].uid = uid; g_lend[i].ca = as; g_lend[i].cm = a;
    g_lend[i].da = 1; g_lend[i].dm = 0; g_lend[i].br = 0; g_lend[i].act = 1;
    ps("[LEND] U"); pi(uid); ps(" col="); pi(a); ps(" as="); pi(as); ps("\n"); return (int)i;
}
static int lend_borr(unsigned int uid, unsigned int as, unsigned int a) {
    unsigned int i; int cv;
    for (i = 0; i < g_ln; i++) { if (g_lend[i].uid == uid && g_lend[i].act) {
        cv = (int)(g_lend[i].cm * 100);
        if ((int)(g_lend[i].dm + a) * COL_RATIO / 100 > cv) { ps("[LEND] Under-col\n"); return -1; }
        g_lend[i].dm += a; g_lend[i].br = 300 + (g_lend[i].dm * 700 / (g_lend[i].cm + 1));
        ps("[LEND] U"); pi(uid); ps(" borr="); pi(a); ps(" rt="); pi(g_lend[i].br); ps("bps\n"); return 0;
    } } return -2;
}
static int lend_repay(unsigned int uid, unsigned int as, unsigned int a) {
    unsigned int i;
    for (i = 0; i < g_ln; i++) { if (g_lend[i].uid == uid && g_lend[i].act) {
        if (a > g_lend[i].dm) a = g_lend[i].dm; g_lend[i].dm -= a;
        ps("[LEND] U"); pi(uid); ps(" repay="); pi(a); ps("\n"); return 0;
    } } return -1;
}
static int lend_hlth(unsigned int uid) {
    unsigned int i; int hf;
    for (i = 0; i < g_ln; i++) { if (g_lend[i].uid == uid && g_lend[i].act) {
        if (!g_lend[i].dm) return 99999;
        hf = (int)((g_lend[i].cm * 100 * 10000) / g_lend[i].dm);
        ps("[LEND] U"); pi(uid); ps(" hlth="); pi(hf); ps("\n"); return hf;
    } } return -1;
}
static int lend_liquidate(unsigned int uid, unsigned int kp) {
    unsigned int i; int hf; unsigned int bn;
    for (i = 0; i < g_ln; i++) { if (g_lend[i].uid == uid && g_lend[i].act) {
        hf = lend_hlth(uid);
        if (hf > 0 && hf < HLTH_LIQ) { bn = g_lend[i].cm * LIQ_BONUS / 10000;
            ps("[LEND] LIQ U"); pi(uid); ps(" K"); pi(kp); ps(" bn="); pi(bn); ps("\n");
            g_lend[i].cm -= bn; g_lend[i].dm = 0; return (int)bn;
        } return -2;
    } } return -1;
}
static int lend_rate(unsigned int as, unsigned int u) {
    unsigned int base = 200, slope = 800, kink = 8000, rt;
    if (u <= kink) rt = base + (slope * u / 10000);
    else rt = base + slope + (slope * 2 * (u - kink) / 10000);
    ps("[RATE] as="); pi(as); ps(" u="); pi(u); ps(" rt="); pi(rt); ps("bps\n"); return (int)rt;
}
static int cdp_open(unsigned int uid, unsigned int ca, unsigned int cm, unsigned int dm) {
    unsigned int i; if (g_cn >= MAX_CDP) return -1;
    if (dm * COL_RATIO / 100 > cm * 100) { ps("[CDP] Under-col\n"); return -2; }
    i = g_cn++; g_cdp[i].uid = uid; g_cdp[i].ca = ca; g_cdp[i].cm = cm;
    g_cdp[i].dm = dm; g_cdp[i].fa = dm * 50 / 10000; g_cdp[i].act = 1;
    ps("[CDP] #"); pi(i); ps(" U"); pi(uid); ps(" col="); pi(cm); ps(" debt="); pi(dm); ps("\n"); return (int)i;
}
static int cdp_close(unsigned int id) {
    if (id >= g_cn || !g_cdp[id].act) return -1;
    g_cdp[id].act = 0; ps("[CDP] Close #"); pi(id); ps("\n"); return 0;
}
static int flash_loan(unsigned int as, unsigned int a, unsigned int uid) {
    unsigned int f = (a * FLASH_BPS) / 10000; g_ffee += f;
    ps("[FLASH] Loan as="); pi(as); ps(" a="); pi(a); ps(" fee="); pi(f); ps("\n"); return (int)f;
}
/* ===== 3. Yield Aggregation: Vaults + Strategies ===== */
static int v_create(const char *n, unsigned int as, unsigned int si) {
    unsigned int i, j; if (g_vn >= MAX_VAULTS) return -1; i = g_vn++;
    for (j = 0; j < 23 && n[j]; j++) g_vlt[i].nm[j] = n[j]; g_vlt[i].nm[j] = 0;
    g_vlt[i].as = as; g_vlt[i].td = 0; g_vlt[i].ts = 0;
    g_vlt[i].si = si; g_vlt[i].lh = g_ts; g_vlt[i].ay = 0; g_vlt[i].act = 1;
    ps("[VAULT] '"); ps(n); ps("' as="); pi(as); ps(" s="); pi(si); ps("\n"); return (int)i;
}
static int v_dep(unsigned int vid, unsigned int uid, unsigned int a) {
    unsigned int sh; if (vid >= g_vn || !g_vlt[vid].act) return -1;
    sh = g_vlt[vid].ts == 0 ? a : (a * g_vlt[vid].ts) / g_vlt[vid].td;
    g_vlt[vid].td += a; g_vlt[vid].ts += sh;
    ps("[VAULT] U"); pi(uid); ps(" dep="); pi(a); ps(" sh="); pi(sh); ps("\n"); return (int)sh;
}
static int v_wdr(unsigned int vid, unsigned int uid, unsigned int sh) {
    unsigned int o, f;
    if (vid >= g_vn || !g_vlt[vid].act || sh > g_vlt[vid].ts) return -1;
    o = (sh * g_vlt[vid].td) / g_vlt[vid].ts; f = o * VAULT_FEE / 10000; o -= f;
    g_vlt[vid].td -= (o + f); g_vlt[vid].ts -= sh;
    ps("[VAULT] U"); pi(uid); ps(" wdr="); pi(sh); ps(" out="); pi(o); ps(" fee="); pi(f); ps("\n"); return (int)o;
}
static int v_harv(unsigned int vid) {
    unsigned int y, e; if (vid >= g_vn || !g_vlt[vid].act) return -1;
    e = g_ts - g_vlt[vid].lh; y = g_vlt[vid].td * e / 1000000;
    g_vlt[vid].td += y; g_vlt[vid].ay += y; g_vlt[vid].lh = g_ts;
    y_track(vid, g_ts, (int)y);
    ps("[VAULT] Harvest #"); pi(vid); ps(" y="); pi(y); ps("\n"); return (int)y;
}
static int v_apy(unsigned int vid) {
    if (vid >= g_vn || !g_vlt[vid].act || !g_vlt[vid].td) return -1;
    return (int)((g_vlt[vid].ay * 10000) / g_vlt[vid].td);
}
static int s_reg(unsigned int proto, unsigned int pool, unsigned int ap) {
    unsigned int i; if (g_sn >= MAX_STRAT) return -1; i = g_sn++;
    g_str[i].pi = proto; g_str[i].pl = pool; g_str[i].ap = ap;
    g_str[i].al = 0; g_str[i].act = 1;
    ps("[STRAT] p="); pi(proto); ps(" pl="); pi(pool); ps(" apy="); pi(ap); ps("bps\n"); return (int)i;
}
static int s_opt(unsigned int vid) {
    unsigned int i, b = 0, ba = 0;
    for (i = 0; i < g_sn; i++) if (g_str[i].act && g_str[i].ap > ba) { ba = g_str[i].ap; b = i; }
    if (vid < g_vn) g_vlt[vid].si = b;
    ps("[STRAT] Opt v"); pi(vid); ps("->"); pi(b); ps(" apy="); pi(ba); ps("\n"); return (int)b;
}
static int s_route(unsigned int vid) {
    unsigned int i, t = 0, c = 0;
    for (i = 0; i < g_sn && i < MAX_STRAT; i++)
        if (g_str[i].act) { t += g_str[i].ap; c++; g_str[i].al = 100 / c; }
    ps("[STRAT] Route v"); pi(vid); ps(" x"); pi(c); ps(" avg="); pi(c ? t / c : 0); ps("\n");
    return (int)(c ? t / c : 0);
}
static int y_track(unsigned int vid, unsigned int ts, int d) {
    if (g_yn >= MAX_HIST) return -1;
    g_yh[g_yn].vid = vid; g_yh[g_yn].ts = ts; g_yh[g_yn].dt = d; g_yn++; return 0;
}
/* ===== 4. Derivatives: Perpetuals, Options, Synthetics ===== */
static int perp_open(unsigned int uid, unsigned int ai, int sd, unsigned int mg, unsigned int lv) {
    unsigned int i; if (g_pern >= MAX_PERP) return -1; i = g_pern++;
    g_perp[i].uid = uid; g_perp[i].ai = ai; g_perp[i].sd = sd;
    g_perp[i].mg = mg; g_perp[i].lv = lv; g_perp[i].ep = 200000 + (rng() % 5000);
    g_perp[i].sz = mg * lv; g_perp[i].act = 1;
    ps("[PERP] U"); pi(uid); ps(" "); ps(sd ? "SHORT" : "LONG");
    ps(" as="); pi(ai); ps(" mg="); pi(mg); ps(" x"); pi(lv); ps("\n"); return (int)i;
}
static int perp_close(unsigned int id) {
    int pnl; unsigned int cp;
    if (id >= g_pern || !g_perp[id].act) return -1;
    cp = 201000 + (rng() % 4000);
    pnl = g_perp[id].sd == 0 ? (int)((cp - g_perp[id].ep) * g_perp[id].sz / g_perp[id].ep)
                              : (int)((g_perp[id].ep - cp) * g_perp[id].sz / g_perp[id].ep);
    g_perp[id].act = 0;
    ps("[PERP] Close #"); pi(id); ps(" pnl="); pi(pnl); ps(" px="); pi(cp); ps("\n"); return pnl;
}
static int perp_fund(unsigned int id) {
    int r; unsigned int p;
    if (id >= g_pern || !g_perp[id].act) return -1;
    r = (int)(rng() % 200) - 100; p = g_perp[id].sz * uabs(r) / 10000;
    ps("[PERP] Fund #"); pi(id); ps(" r="); pi(r); ps(" pay="); pi(p); ps("\n"); return r;
}
static int perp_mark(unsigned int id) {
    unsigned int px; if (id >= g_pern) return -1;
    px = 200000 + (rng() % 5000);
    ps("[PERP] Mark #"); pi(id); ps("="); pi(px); ps("\n"); return (int)px;
}
static int opt_px(unsigned int s, unsigned int k, unsigned int t, unsigned int v, int c) {
    unsigned int it, tv, px;
    it = c ? (s > k ? s - k : 0) : (k > s ? k - s : 0);
    tv = (s * v / 100 * isqrt(t + 1)) / 100; px = it + tv;
    ps("[OPT] "); ps(c ? "CALL" : "PUT"); ps(" S="); pi(s); ps(" K="); pi(k);
    ps(" T="); pi(t); ps(" V="); pi(v); ps(" px="); pi(px); ps("\n"); return (int)px;
}
static int opt_grk(unsigned int s, unsigned int k, unsigned int t, unsigned int v, int c) {
    int d, th; unsigned int g, vg; if (!t) t = 1;
    d = c ? 5000 + (int)((s - k) * 50 / (s + 1)) : -5000 + (int)((s - k) * 50 / (s + 1));
    g = (10000 * v) / (s * isqrt(t) + 1);
    th = (unsigned int)(-(int)(s * v / (100 * 365 * (isqrt(t) + 1)))); vg = (s * isqrt(t)) / 100;
    ps("[GK] D="); pi(d); ps(" G="); pi(g); ps(" T="); pi(th); ps(" V="); pi(vg); ps("\n"); return d;
}
static int synth_mk(unsigned int ai, unsigned int ca, unsigned int sa) {
    if (ca * 100 < sa * COL_RATIO / 100) { ps("[SYNTH] Under-col\n"); return -1; }
    ps("[SYNTH] Mint as="); pi(ai); ps(" col="); pi(ca); ps(" sy="); pi(sa); ps("\n"); return (int)sa;
}
static int brg_lock(unsigned int sc, unsigned int dc, unsigned int as, unsigned int a, const char *r) {
    unsigned int f = (a * BRG_FEE) / 10000, n = a - f; g_bfee += f;
    ps("[BRG] Lock C"); pi(sc); ps("->C"); pi(dc); ps(" as="); pi(as);
    ps(" net="); pi(n); ps(" fee="); pi(f); ps(" r="); ps(r); ps("\n"); return (int)n;
}
static int brg_burn(unsigned int ci, unsigned int as, unsigned int a, const char *r) {
    ps("[BRG] Burn C"); pi(ci); ps(" as="); pi(as); ps(" a="); pi(a); ps(" r="); ps(r); ps("\n"); return 0;
}
static int htlc_mk(unsigned int ch, unsigned int a, const unsigned char *h, unsigned int ex) {
    unsigned int i; if (g_hn >= MAX_HTLC) return -1; i = g_hn++;
    g_ht[i].ch = ch; g_ht[i].as = 0; g_ht[i].a = a; g_ht[i].ex = ex; g_ht[i].act = 1;
    mc(g_ht[i].h, h, 32);
    ps("[HTLC] #"); pi(i); ps(" ch="); pi(ch); ps(" a="); pi(a); ps(" h="); ph(h, 8); ps("\n"); return (int)i;
}
static int htlc_red(unsigned int id, const unsigned char *s) {
    unsigned char h[8]; if (id >= g_hn || !g_ht[id].act) return -1;
    hdata(s, 32, h);
    if (mm(h, g_ht[id].h, 8) != 0) { ps("[HTLC] Bad secret\n"); return -2; }
    g_ht[id].act = 0;
    ps("[HTLC] Red #"); pi(id); ps(" a="); pi(g_ht[id].a); ps("\n"); return 0;
}
static int brg_msig(const unsigned char *d, unsigned int dl, unsigned int th) {
    unsigned char h[8]; unsigned int sigs = 3; hdata(d, dl, h);
    ps("[BRG] MSIG sigs="); pi(sigs); ps(" thr="); pi(th); ps("\n"); return sigs >= th ? 1 : 0;
}
/* ===== 6. Oracle: Feeds, Aggregation, TWAP, VRF ===== */
static int ora_feed(unsigned int fi, unsigned int p, unsigned int ts) {
    unsigned int i;
    if (g_fn < MAX_FEED) { i = g_fn++; g_fd[i].p = p; g_fd[i].ts = ts; g_fd[i].ni = rng() % 4; g_fd[i].act = 1; }
    if (fi < g_an) { unsigned int sl = g_agg[fi].cnt % MAX_HIST;
        g_agg[fi].pr[sl] = p; g_agg[fi].tm[sl] = ts; g_agg[fi].cnt++; }
    ps("[ORA] F"); pi(fi); ps(" px="); pi(p); ps("\n"); return 0;
}
static int ora_agg(unsigned int fi) {
    unsigned int i, s = 0, c = 0, m;
    if (fi >= g_an) { if (g_an < MAX_ORACLE) { fi = g_an++; g_agg[fi].cnt = 0; g_agg[fi].ap = 0; } else return -1; }
    for (i = 0; i < g_fn; i++) if (g_fd[i].act) { s += g_fd[i].p; c++; }
    if (!c) return -2; m = s / c; g_agg[fi].ap = m;
    ps("[ORA] Agg#"); pi(fi); ps(" med="); pi(m); ps(" n="); pi(c); ps("\n"); return (int)m;
}
static int ora_twap(unsigned int fi, unsigned int w) {
    unsigned int i, s = 0, c = 0;
    if (fi >= g_an || !g_agg[fi].cnt) return -1;
    for (i = 0; i < g_agg[fi].cnt && i < MAX_HIST; i++) { s += g_agg[fi].pr[i]; c++; }
    if (!c) return -2;
    ps("[ORA] TWAP#"); pi(fi); ps(" w="); pi(w); ps(" v="); pi(s / c); ps("\n"); return (int)(s / c);
}
static int ora_vrf(unsigned int sd) {
    unsigned int r; unsigned char b[4], o[8];
    b[0] = (unsigned char)(sd>>24); b[1] = (unsigned char)(sd>>16);
    b[2] = (unsigned char)(sd>>8);  b[3] = (unsigned char)sd;
    hdata(b, 4, o);
    r = ((unsigned int)o[0]<<24) | ((unsigned int)o[1]<<16) | ((unsigned int)o[2]<<8) | (unsigned int)o[3];
    ps("[VRF] sd="); pi(sd); ps(" r="); pi(r); ps("\n"); return (int)(r % 1000000);
}
static int ora_outlier(unsigned int fi, unsigned int v) {
    unsigned int i, a = 0, c = 0, d;
    if (fi >= g_an || !g_agg[fi].cnt) return 0;
    for (i = 0; i < g_agg[fi].cnt && i < MAX_HIST; i++) { a += g_agg[fi].pr[i]; c++; }
    if (!c) return 0; a /= c; d = v > a ? v - a : a - v;
    if (d * 100 > a * 10) { ps("[ORA] Outlier#"); pi(fi); ps(" v="); pi(v); ps("\n"); return 1; }
    return 0;
}
static int ora_node(unsigned int ni, unsigned int st) {
    unsigned int i; if (g_nn >= MAX_NODE) return -1;
    if (st < STAKE_MIN) { ps("[ORA] Low stake\n"); return -2; }
    i = g_nn++; g_nd[i].ni = ni; g_nd[i].st = st; g_nd[i].rp = 0; g_nd[i].sl = 0; g_nd[i].act = 1;
    ps("[ORA] Node "); pi(ni); ps(" st="); pi(st); ps("\n"); return (int)i;
}
static int ora_slash(unsigned int ni, unsigned int a) {
    unsigned int i;
    for (i = 0; i < g_nn; i++) { if (g_nd[i].ni == ni && g_nd[i].act) {
        if (a > g_nd[i].st) a = g_nd[i].st; g_nd[i].st -= a; g_nd[i].sl++;
        ps("[ORA] Slash N"); pi(ni); ps(" a="); pi(a); ps("\n"); return (int)a;
    } } return -1;
}
/* ===== Main Entry Point ===== */
int main(void) {
    unsigned char sec[32], hh[8];
    unsigned int i, pid, cdp_id, perp_id, v_id, bnet;
    int pnl;
    ps("========================================\n");
    ps("  DeFi Platform v1.0 - AI-ASM OS\n");
    ps("========================================\n");
    ps("[INIT] Bootstrapping DeFi platform...\n");
    ora_node(0, 5000); ora_node(1, 3000); ora_node(2, 8000); ora_node(3, 2000);
    ora_feed(0, 200000, g_ts); ora_feed(0, 200150, g_ts + 1);
    ora_feed(0, 199900, g_ts + 2); ora_feed(0, 200050, g_ts + 3);
    ora_agg(0); ora_twap(0, 4);
    ps("--- [DEX] AMM + Order Book ---\n");
    pid = amm_create(0, 1, 1000000, 200000000);
    amm_create(0, 2, 500000, 10000000); amm_create(1, 2, 200000, 4000000);
    amm_add_lq(pid, 100000, 20000000); amm_add_lq(pid, 50000, 10000000);
    amm_swap(pid, 0, 10000, 0); amm_swap(pid, 0, 5000, 0);
    ps("Spot="); pi(amm_spot(pid, 0, 1)); ps("bps Impact="); pi(amm_impact(pid, 0, 50000)); ps("bps\n");
    ob_limit(pid, 0, 19900, 5000); ob_limit(pid, 1, 20100, 3000); ob_market(pid, 0, 2000);
    flash_swap(pid, 0, 500000);
    ps("--- [LEND] Lending + CDPs ---\n");
    lend_col(100, 0, 100000); lend_borr(100, 1, 50000); lend_hlth(100);
    lend_rate(0, 6500); lend_rate(0, 9000);
    lend_col(101, 0, 50000); lend_borr(101, 1, 40000); lend_hlth(101);
    lend_repay(100, 1, 20000); lend_hlth(100);
    cdp_id = cdp_open(200, 0, 200000, 100000); cdp_open(201, 0, 150000, 70000); cdp_close(cdp_id);
    flash_loan(0, 1000000, 300);
    ps("--- [YIELD] Vaults + Strategies ---\n");
    s_reg(0, 0, 800); s_reg(1, 1, 1200); s_reg(2, 2, 600); s_reg(0, 3, 1500);
    v_id = v_create("ETH-Auto", 0, 0); v_create("BTC-Max", 2, 1); v_create("STAB-Inc", 1, 2);
    v_dep(v_id, 400, 500000); v_dep(v_id, 401, 250000);
    s_opt(v_id); s_route(v_id);
    g_ts += 86400;
    v_harv(v_id); ps("APY="); pi(v_apy(v_id)); ps("bps\n");
    v_wdr(v_id, 400, 100000);
    ps("--- [DERIV] Perps + Options + Synthetics ---\n");
    perp_id = perp_open(500, 0, 0, 10000, 10);
    perp_open(501, 0, 1, 8000, 5); perp_open(502, 1, 0, 15000, 20);
    perp_mark(perp_id); perp_fund(perp_id);
    pnl = perp_close(perp_id); ps("PnL="); pi(pnl); ps("\n");
    opt_px(200000, 210000, 30, 60, 1); opt_px(200000, 190000, 30, 60, 0);
    opt_grk(200000, 200000, 30, 50, 1);
    synth_mk(3, 500000, 250000); synth_mk(4, 300000, 100000);
    ps("--- [BRIDGE] Cross-Chain ---\n");
    bnet = brg_lock(0, 1, 0, 100000, "0xAABB"); ps("Net="); pi(bnet); ps("\n");
    brg_lock(1, 2, 1, 50000, "0xCCDD"); brg_burn(2, 1, 49000, "0xEEFF");
    for (i = 0; i < 32; i++) sec[i] = (unsigned char)(0xA0 | (i & 0x0F));
    hdata(sec, 32, hh); htlc_mk(0, 25000, hh, g_ts + 3600); htlc_red(0, sec);
    { unsigned char md[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; brg_msig(md, 8, 2); }
    ps("--- [ORA] Oracle Advanced ---\n");
    ora_vrf(12345); ora_vrf(67890); ora_feed(1, 1500, g_ts + 10); ora_agg(1);
    ora_outlier(0, 200000); ora_outlier(0, 300000); ora_slash(1, 500);
    ps("--- STATISTICS ---\n");
    ps("Pools="); pi(g_pn); ps(" Orders="); pi(g_on); ps(" Lend="); pi(g_ln);
    ps(" CDP="); pi(g_cn); ps(" Vaults="); pi(g_vn); ps(" Strats="); pi(g_sn); ps("\n");
    ps("Perps="); pi(g_pern); ps(" HTLCs="); pi(g_hn); ps(" Feeds="); pi(g_fn);
    ps(" Nodes="); pi(g_nn); ps(" YHist="); pi(g_yn); ps(" Aggs="); pi(g_an); ps("\n");
    ps("BridgeFee="); pi(g_bfee); ps(" FlashFee="); pi(g_ffee); ps("\n");
    ps("========================================\n");
    ps("  DeFi Platform operational.\n");
    ps("========================================\n");
    return 0;
}
