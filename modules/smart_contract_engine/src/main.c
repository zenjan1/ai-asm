/* smart_contract_engine: Blockchain smart contract execution engine (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_C 32
#define MAX_S 64
#define MAX_BC 256
#define MAX_LOG 32
#define MAX_STK 64
#define MAX_MEM 4096
#define MAX_ORA 16
#define MAX_CH 8
#define MAX_FAC 8
#define MAX_TST 32
#define MAX_FZ 128
#define MAX_CV 256
#define ADDR 20
#define HASH 32

#define OP_STOP 0x00
#define OP_ADD 0x01
#define OP_MUL 0x02
#define OP_SUB 0x03
#define OP_DIV 0x04
#define OP_LT 0x10
#define OP_GT 0x11
#define OP_EQ 0x14
#define OP_ISZ 0x15
#define OP_AND 0x16
#define OP_OR 0x17
#define OP_XOR 0x18
#define OP_NOT 0x19
#define OP_SHA3 0x20
#define OP_POP 0x50
#define OP_MLD 0x51
#define OP_MST 0x52
#define OP_SLD 0x54
#define OP_SST 0x55
#define OP_PSH1 0x60
#define OP_PSH32 0x7F
#define OP_DUP1 0x80
#define OP_SWP1 0x90
#define OP_RET 0xF3
#define OP_RVT 0xFD
#define OP_SELF 0xFF
#define OP_LOG0 0xA0
#define OP_LOG1 0xA1

#define VM_EVM 0
#define VM_WASM 1
#define VM_CUST 2
#define ST_DEP 0
#define ST_PAUSE 1
#define ST_UPG 2
#define ST_DEST 3
#define PX_NONE 0
#define PX_TRANS 1
#define PX_UUPS 2
#define PX_DIAM 3
#define CMP_SOL 0
#define CMP_VYP 1
#define CMP_INK 2
#define OR_PR 0
#define OR_RNG 1
#define OR_EXT 2
#define OR_DEC 3

#define E_OK 0
#define E_GAS 1
#define E_STKOV 2
#define E_STKUN 3
#define E_OP 4
#define E_RVT 5
#define E_WR 7
#define E_ORA 8
#define E_XC 9

typedef struct { unsigned int key, val; } slot_t;
typedef struct { unsigned char addr[ADDR], code[MAX_BC]; unsigned int clen, scnt, bal, nonce; slot_t sto[MAX_S]; int status, vm; char name[32]; } contract_t;
typedef struct { unsigned int stk[MAX_STK]; int sp; unsigned char mem[MAX_MEM]; unsigned int msz, pc, gas, glm, roff, rlen; int err, stop; } evm_t;
typedef struct { unsigned int stk[MAX_STK]; int sp; unsigned char lm[MAX_MEM]; unsigned int pc, gas, glm, pages; int err, stop; } wasm_t;
typedef struct { unsigned int r[16], pc, fl, gas, glm; int err, stop; unsigned char hp[MAX_MEM]; unsigned int ht; } cust_t;
typedef struct { unsigned int tot, base, mem, sto, call, log; } gprof_t;
typedef struct { unsigned char tp[4][HASH]; unsigned int tc, doff, dlen; } log_t;
typedef struct { char nm[32]; int cmp, opt, ok; unsigned int bh, bl; char em[64]; } comp_t;
typedef struct { char nm[48]; int pass, cov; unsigned int gas, asrt; char fm[64]; } tres_t;
typedef struct { unsigned int h[MAX_CV], t[MAX_CV], nl; int pct; } cov_t;
typedef struct { unsigned int sd, rnd, fail, tst; } fuzz_t;
typedef struct { int pt; unsigned char impl[ADDR], px[ADDR]; unsigned int ver, fac[MAX_FAC][ADDR], fc; int init; } upg_t;
typedef struct { int mt; unsigned int src, dst, val, nonce; unsigned char snd[ADDR], rcv[ADDR], ph[HASH]; int conf, fin; } xmsg_t;
typedef struct { unsigned int id, aa, ab, tl; unsigned char hl[HASH], pa[ADDR], pb[ADDR]; int st; } swap_t;
typedef struct { int ot, act; char nm[32]; unsigned int v, ts, cf, sc, rnd; } ora_t;

static unsigned int hpos = 65536;
static contract_t cs[MAX_C]; static int ccnt = 0;
static log_t lgs[MAX_LOG]; static int lcnt = 0;
static gprof_t gp; static upg_t ups[MAX_C];
static xmsg_t xm[MAX_CH * 4]; static int xcnt = 0;
static swap_t sw[16]; static int scnt = 0;
static ora_t ora[MAX_ORA]; static int ocnt = 0;
static unsigned int cid = 1, bnum = 18000000, bts = 1700000000;

static unsigned int alloc(unsigned int n) { unsigned int p = hpos; hpos += n; hpos = (hpos + 15) & ~15u; return p; }
static void cmem(const char *s, unsigned int d, unsigned int l) { char *p = (char *)d; for (unsigned int i = 0; i < l; i++) p[i] = s[i]; }
static unsigned int slen(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static void cstr(char *d, const char *s, unsigned int m) { unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static void ps(const char *s) { unsigned int l = slen(s), b = alloc(l + 1); cmem(s, b, l + 1); host_print((const char *)b); }
static void pi(int n) {
    char b[16]; int i = 15, ng = 0; b[15] = 0;
    if (n < 0) { ng = 1; n = -n; } if (n == 0) b[--i] = '0';
    while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; }
    if (ng) b[--i] = '-'; ps(&b[i]);
}
static void ph(unsigned int v) {
    char b[12]; int i = 9; b[10] = 0;
    if (!v) { ps("0x0"); return; }
    while (v > 0) { int d = v & 0xF; b[i--] = (d < 10) ? ('0' + d) : ('A' + d - 10); v >>= 4; }
    b[i - 1] = '0'; b[i] = 'x'; ps(&b[i == 9 ? 2 : i - 1]);
}
static unsigned int hash(const unsigned char *d, unsigned int l) {
    unsigned int h = 0x811C9DC5; for (unsigned int i = 0; i < l; i++) { h ^= d[i]; h *= 0x01000193; } return h;
}

/* EVM */
static void evm_init(evm_t *c, unsigned int gl) {
    c->sp = 0; c->pc = 0; c->msz = 0; c->gas = 0; c->glm = gl; c->err = E_OK; c->stop = 0; c->roff = 0; c->rlen = 0;
    for (int i = 0; i < MAX_STK; i++) c->stk[i] = 0; for (unsigned int i = 0; i < MAX_MEM; i++) c->mem[i] = 0;
}
static int egas(evm_t *c, unsigned int cost) {
    c->gas += cost; gp.tot += cost;
    if (c->gas > c->glm) { c->err = E_GAS; c->stop = 1; return E_GAS; } return E_OK;
}
static int epush(evm_t *c, unsigned int v) {
    if (c->sp >= MAX_STK) { c->err = E_STKOV; c->stop = 1; return E_STKOV; } c->stk[c->sp++] = v; return E_OK;
}
static unsigned int epop(evm_t *c) {
    if (c->sp <= 0) { c->err = E_STKUN; c->stop = 1; return 0; } return c->stk[--c->sp];
}
static int evm_exec(evm_t *c, const unsigned char *cd, unsigned int cl, contract_t *ct) {
    c->pc = 0;
    while (!c->stop && c->pc < cl) {
        unsigned char op = cd[c->pc];
        if (egas(c, 3) != E_OK) break;
        switch (op) {
        case OP_STOP: c->stop = 1; break;
        case OP_ADD: { unsigned int b = epop(c), a = epop(c); epush(c, a + b); c->pc++; break; }
        case OP_MUL: { unsigned int b = epop(c), a = epop(c); epush(c, a * b); c->pc++; break; }
        case OP_SUB: { unsigned int b = epop(c), a = epop(c); epush(c, a - b); c->pc++; break; }
        case OP_DIV: { unsigned int b = epop(c), a = epop(c); epush(c, b ? a / b : 0); c->pc++; break; }
        case OP_LT:  { unsigned int b = epop(c), a = epop(c); epush(c, a < b ? 1 : 0); c->pc++; break; }
        case OP_GT:  { unsigned int b = epop(c), a = epop(c); epush(c, a > b ? 1 : 0); c->pc++; break; }
        case OP_EQ:  { unsigned int b = epop(c), a = epop(c); epush(c, a == b ? 1 : 0); c->pc++; break; }
        case OP_ISZ: { unsigned int a = epop(c); epush(c, a == 0 ? 1 : 0); c->pc++; break; }
        case OP_AND: { unsigned int b = epop(c), a = epop(c); epush(c, a & b); c->pc++; break; }
        case OP_OR:  { unsigned int b = epop(c), a = epop(c); epush(c, a | b); c->pc++; break; }
        case OP_XOR: { unsigned int b = epop(c), a = epop(c); epush(c, a ^ b); c->pc++; break; }
        case OP_NOT: { unsigned int a = epop(c); epush(c, ~a); c->pc++; break; }
        case OP_SHA3: {
            unsigned int l = epop(c), o = epop(c); (void)l; (void)o;
            unsigned int h = 0xDEADBEEF;
            if (c->sp >= 2) { h ^= c->stk[c->sp - 1]; h ^= c->stk[c->sp - 2] * 0x01000193; }
            epush(c, h); egas(c, 27); c->pc++; break;
        }
        case OP_SLD: {
            unsigned int k = epop(c), v = 0;
            for (unsigned int i = 0; i < ct->scnt; i++)
                if (ct->sto[i].key == k) { v = ct->sto[i].val; break; }
            epush(c, v); egas(c, 100); c->pc++; break;
        }
        case OP_SST: {
            unsigned int v = epop(c), k = epop(c); int f = 0;
            for (unsigned int i = 0; i < ct->scnt; i++)
                if (ct->sto[i].key == k) { ct->sto[i].val = v; f = 1; break; }
            if (!f && ct->scnt < MAX_S) { ct->sto[ct->scnt].key = k; ct->sto[ct->scnt++].val = v; }
            gp.sto += 200; egas(c, 200); c->pc++; break;
        }
        case OP_PSH1:
            if (c->pc + 1 < cl) { epush(c, cd[c->pc + 1]); c->pc += 2; }
            else { c->err = E_OP; c->stop = 1; } break;
        case OP_PSH32: {
            unsigned int v = 0;
            for (int b = 0; b < 4 && (c->pc + 1 + b) < cl; b++) v = (v << 8) | cd[c->pc + 1 + b];
            epush(c, v); c->pc += 5; break;
        }
        case OP_DUP1: if (c->sp > 0) epush(c, c->stk[c->sp - 1]); c->pc++; break;
        case OP_SWP1:
            if (c->sp >= 2) { unsigned int t = c->stk[c->sp - 1]; c->stk[c->sp - 1] = c->stk[c->sp - 2]; c->stk[c->sp - 2] = t; }
            c->pc++; break;
        case OP_POP: epop(c); c->pc++; break;
        case OP_MST: {
            unsigned int v = epop(c), o = epop(c);
            if (o + 4 <= MAX_MEM) {
                c->mem[o] = (v >> 24) & 0xFF; c->mem[o + 1] = (v >> 16) & 0xFF;
                c->mem[o + 2] = (v >> 8) & 0xFF; c->mem[o + 3] = v & 0xFF;
                if (o + 4 > c->msz) c->msz = o + 4; gp.mem += 3;
            } c->pc++; break;
        }
        case OP_MLD: {
            unsigned int o = epop(c), v = 0;
            if (o + 4 <= MAX_MEM)
                v = ((unsigned int)c->mem[o] << 24) | ((unsigned int)c->mem[o + 1] << 16) |
                    ((unsigned int)c->mem[o + 2] << 8) | (unsigned int)c->mem[o + 3];
            epush(c, v); c->pc++; break;
        }
        case OP_RET: { unsigned int l = epop(c), o = epop(c); c->roff = o; c->rlen = l; c->stop = 1; break; }
        case OP_RVT: epop(c); epop(c); c->err = E_RVT; c->stop = 1; break;
        case OP_LOG0: case OP_LOG1: {
            if (lcnt < MAX_LOG) {
                int nt = (op == OP_LOG0) ? 0 : 1;
                unsigned int dl = epop(c), df = epop(c); (void)dl; (void)df;
                lgs[lcnt].tc = nt; lgs[lcnt].doff = df; lgs[lcnt].dlen = dl;
                for (int t = 0; t < nt && c->sp > 0; t++) {
                    unsigned int tv = epop(c);
                    for (int b = 0; b < HASH; b++) lgs[lcnt].tp[t][b] = (tv >> ((b & 3) * 8)) & 0xFF;
                }
                lcnt++; gp.log += 375;
            } c->pc++; break;
        }
        case OP_SELF: ct->status = ST_DEST; c->stop = 1; break;
        default: c->err = E_OP; c->stop = 1; break;
        }
    } return c->err;
}

/* WASM VM */
static void wasm_init(wasm_t *c, unsigned int gl) {
    c->sp = 0; c->pc = 0; c->pages = 1; c->gas = 0; c->glm = gl; c->err = E_OK; c->stop = 0;
    for (int i = 0; i < MAX_STK; i++) c->stk[i] = 0; for (unsigned int i = 0; i < MAX_MEM; i++) c->lm[i] = 0;
}
static int wasm_exec(wasm_t *c, const unsigned char *cd, unsigned int cl) {
    c->pc = 0;
    while (!c->stop && c->pc < cl) {
        c->gas += 2;
        if (c->gas > c->glm) { c->err = E_GAS; c->stop = 1; break; }
        switch (cd[c->pc]) {
        case 0x41:
            if (c->pc + 4 < cl) {
                unsigned int v = ((unsigned int)cd[c->pc + 1] << 24) | ((unsigned int)cd[c->pc + 2] << 16) |
                                 ((unsigned int)cd[c->pc + 3] << 8) | (unsigned int)cd[c->pc + 4];
                if (c->sp < MAX_STK) c->stk[c->sp++] = v; c->pc += 5;
            } else c->stop = 1; break;
        case 0x01: if (c->sp >= 2) { unsigned int b = c->stk[--c->sp]; c->stk[c->sp - 1] += b; } c->pc++; break;
        case 0x02: if (c->sp >= 2) { unsigned int b = c->stk[--c->sp]; c->stk[c->sp - 1] *= b; } c->pc++; break;
        case 0x03: if (c->sp >= 2) { unsigned int b = c->stk[--c->sp]; c->stk[c->sp - 1] -= b; } c->pc++; break;
        case 0x04:
            if (c->sp >= 1) {
                unsigned int a = c->stk[c->sp - 1], v = 0;
                if (a + 4 <= MAX_MEM) v = ((unsigned int)c->lm[a] << 24) | ((unsigned int)c->lm[a + 1] << 16) |
                    ((unsigned int)c->lm[a + 2] << 8) | (unsigned int)c->lm[a + 3];
                c->stk[c->sp - 1] = v;
            } c->pc++; break;
        case 0x05:
            if (c->sp >= 2) {
                unsigned int v = c->stk[--c->sp], a = c->stk[--c->sp];
                if (a + 4 <= MAX_MEM) {
                    c->lm[a] = (v >> 24) & 0xFF; c->lm[a + 1] = (v >> 16) & 0xFF;
                    c->lm[a + 2] = (v >> 8) & 0xFF; c->lm[a + 3] = v & 0xFF;
                }
            } c->pc++; break;
        case 0x0B: c->stop = 1; break;
        default: c->pc++; break;
        }
    } return c->err;
}

/* Custom VM */
static void cust_init(cust_t *c, unsigned int gl) {
    for (int i = 0; i < 16; i++) c->r[i] = 0;
    c->pc = 0; c->fl = 0; c->gas = 0; c->glm = gl; c->err = E_OK; c->stop = 0; c->ht = 0;
    for (unsigned int i = 0; i < MAX_MEM; i++) c->hp[i] = 0;
}static int cust_exec(cust_t *c, const unsigned char *cd, unsigned int cl) {
    c->pc = 0;
    while (!c->stop && c->pc < cl) {
        c->gas++;
        if (c->gas > c->glm) { c->err = E_GAS; c->stop = 1; break; }
        unsigned char op = cd[c->pc], rd = (c->pc + 1 < cl) ? cd[c->pc + 1] : 0;
        unsigned char rs = (c->pc + 2 < cl) ? cd[c->pc + 2] : 0;
        switch (op) {
        case 0x09: {
            unsigned int im = 0;
            if (c->pc + 3 < cl) im = ((unsigned int)cd[c->pc + 2] << 8) | cd[c->pc + 3];
            if (rd < 16) c->r[rd] = im; c->pc += 4; break;
        }
        case 0x01: case 0x02: case 0x03: {
            unsigned char rt = (c->pc + 3 < cl) ? cd[c->pc + 3] : 0;
            if (rd < 16 && rs < 16) {
                unsigned int b = (rt < 16) ? c->r[rt] : 0;
                if (op == 0x01) c->r[rd] = c->r[rs] + b;
                else if (op == 0x02) c->r[rd] = c->r[rs] - b;
                else c->r[rd] = c->r[rs] * b;
            } c->pc += 4; break;
        }
        case 0x04:
            if (rd < 16 && rs < 16) {
                unsigned int a = c->r[rs];
                if (a + 4 <= c->ht) c->r[rd] = ((unsigned int)c->hp[a] << 24) | ((unsigned int)c->hp[a + 1] << 16) |
                    ((unsigned int)c->hp[a + 2] << 8) | (unsigned int)c->hp[a + 3];
            } c->pc += 4; break;
        case 0x05:
            if (rd < 16 && rs < 16) {
                unsigned int a = c->r[rs], v = c->r[rd];
                if (a + 4 <= MAX_MEM) {
                    c->hp[a] = (v >> 24) & 0xFF; c->hp[a + 1] = (v >> 16) & 0xFF;
                    c->hp[a + 2] = (v >> 8) & 0xFF; c->hp[a + 3] = v & 0xFF;
                    if (a + 4 > c->ht) c->ht = a + 4;
                }
            } c->pc += 4; break;
        case 0x06: c->pc = (rd << 8) | rs; break;
        case 0x07: if (rs < 16 && c->r[rs] == 0) c->pc = (rd << 8) | rs; else c->pc += 4; break;
        case 0x08: c->stop = 1; break;
        default: c->pc++; break;
        }
    } return c->err;
}

/* Compilation */
static comp_t compile(const char *nm, int ct, const unsigned char *src, unsigned int sl, int opt) {
    comp_t r; cstr(r.nm, nm, 32); r.cmp = ct; r.opt = opt; r.ok = 1; r.em[0] = 0;
    unsigned int sh = hash(src, sl);
    if (sl < 4) { r.ok = 0; cstr(r.em, "Source too short", 64); return r; }
    if (ct == CMP_SOL) { r.bh = sh ^ 0x504F4C44; r.bl = sl * 3 / 2; }
    else if (ct == CMP_VYP) { r.bh = sh ^ 0x56595045; r.bl = sl * 4 / 3; }
    else if (ct == CMP_INK) { r.bh = sh ^ 0x494E4B21; r.bl = sl * 5 / 3; }
    else { r.ok = 0; cstr(r.em, "Unknown compiler", 64); return r; }
    if (opt && r.ok) { r.bl = r.bl * 85 / 100; r.bh ^= 0x0000C0DE; } return r;
}

/* Testing */
static tres_t run_test(const char *nm, const unsigned char *bc, unsigned int bl, unsigned int exp) {
    tres_t r; cstr(r.nm, nm, 48); r.pass = 0; r.gas = 0; r.asrt = 0; r.cov = 0; r.fm[0] = 0;
    evm_t ctx; evm_init(&ctx, 1000000); contract_t d; d.scnt = 0; d.status = ST_DEP;
    evm_exec(&ctx, bc, bl, &d); r.gas = ctx.gas; r.asrt++;
    if (ctx.err == E_OK) { unsigned int act = (ctx.sp > 0) ? ctx.stk[0] : 0;
        if (act == exp) { r.pass = 1; r.cov = 85; } else cstr(r.fm, "Mismatch", 64);
    } else cstr(r.fm, "Exec error", 64);
    r.asrt++; return r;
}
static cov_t gen_cov(const unsigned char *bc, unsigned int bl) {
    cov_t r; r.nl = (bl < MAX_CV) ? bl : MAX_CV; unsigned int hc = 0;
    for (unsigned int i = 0; i < MAX_CV; i++) { r.t[i] = 0; r.h[i] = 0; }
    for (unsigned int i = 0; i < r.nl; i++) { r.t[i] = 1;
        if (bc[i] != 0x00 && bc[i] != 0xFE) { r.h[i] = 1; hc++; } }
    r.pct = (r.nl > 0) ? (int)(hc * 100 / r.nl) : 0; return r;
}
static fuzz_t run_fuzz(const unsigned char *bc, unsigned int bl, unsigned int sd, unsigned int rnd) {
    fuzz_t r; r.sd = sd; r.rnd = rnd; r.fail = 0; r.tst = 0; unsigned int rng = sd;
    for (unsigned int i = 0; i < rnd && i < MAX_FZ; i++) {
        rng = rng * 1103515245 + 12345; unsigned char fz[MAX_BC];
        for (unsigned int j = 0; j < bl && j < MAX_BC; j++) fz[j] = bc[j];
        if (bl > 1) fz[1] = rng & 0xFF;
        evm_t ctx; evm_init(&ctx, 500000); contract_t d; d.scnt = 0; d.status = ST_DEP;
        evm_exec(&ctx, fz, bl, &d); r.tst++;
        if (ctx.err != E_OK && ctx.err != E_GAS) r.fail++;
    } return r;
}

/* Upgrades */
static int dep_proxy(int pt, unsigned int ci) {
    if (ci >= MAX_C) return E_OP;
    upg_t *s = &ups[ci]; s->pt = pt; s->ver = 1; s->init = 1; s->fc = 0;
    for (int i = 0; i < ADDR; i++) { s->impl[i] = cs[ci].addr[i]; s->px[i] = cs[ci].addr[i] ^ 0xFF; }
    if (pt == PX_TRANS) ps("  Transparent proxy\n");
    else if (pt == PX_UUPS) ps("  UUPS proxy\n");
    else if (pt == PX_DIAM) {
        s->fc = 1; for (int i = 0; i < ADDR; i++) s->fac[0][i] = cs[ci].addr[i];
        ps("  Diamond proxy (1 facet)\n");
    } return E_OK;
}
static int upgrade(unsigned int ci, unsigned int ni) {
    if (ci >= MAX_C || ni >= MAX_C) return E_OP;
    upg_t *s = &ups[ci]; if (!s->init || s->pt == PX_NONE) return E_WR;
    for (int i = 0; i < ADDR; i++) s->impl[i] = cs[ni].addr[i];
    s->ver++; cs[ci].status = ST_UPG;
    ps("  Upgraded to v"); pi(s->ver); ps("\n"); return E_OK;
}
static int add_facet(unsigned int ci, unsigned int fi) {
    if (ci >= MAX_C) return E_OP;
    upg_t *s = &ups[ci]; if (s->pt != PX_DIAM) return E_WR;
    if (s->fc >= MAX_FAC) return E_STKOV;
    for (int i = 0; i < ADDR; i++) s->fac[s->fc][i] = cs[fi].addr[i];
    s->fc++; ps("  Facet added (total: "); pi(s->fc); ps(")\n"); return E_OK;
}

/* Cross-chain */
static int xc_send(unsigned int src, unsigned int dst, unsigned int val,
                   const unsigned char *pl, unsigned int pln) {
    if (xcnt >= MAX_CH * 4 || src >= MAX_CH || dst >= MAX_CH) return E_XC;
    xmsg_t *m = &xm[xcnt];
    m->mt = 0; m->src = src; m->dst = dst; m->val = val; m->nonce = xcnt;
    m->conf = 0; m->fin = 0;
    for (int i = 0; i < ADDR; i++) { m->snd[i] = (unsigned char)(src + i); m->rcv[i] = (unsigned char)(dst + i); }
    m->ph[0] = hash(pl, pln) & 0xFF; xcnt++;
    ps("  XChain "); pi(src); ps("->"); pi(dst);
    ps(" val="); pi(val); ps("\n"); return E_OK;
}
static int mk_swap(unsigned int aa, unsigned int ab, unsigned int tl) {
    if (scnt >= 16) return E_XC;
    swap_t *s = &sw[scnt]; s->id = scnt; s->aa = aa; s->ab = ab;
    s->tl = bts + tl; s->st = 0;
    unsigned int hl = hash((const unsigned char *)&scnt, 4);
    for (int i = 0; i < HASH; i++) s->hl[i] = (hl >> ((i & 3) * 8)) & 0xFF;
    for (int i = 0; i < ADDR; i++) { s->pa[i] = (unsigned char)(0xAA + i); s->pb[i] = (unsigned char)(0xBB + i); }
    scnt++; ps("  Swap id="); pi(s->id); ps(" lock="); ph(hl); ps("\n"); return E_OK;
}
static int cf_swap(unsigned int sid, unsigned int secret) {
    if (sid >= (unsigned int)scnt) return E_XC;
    swap_t *s = &sw[sid]; if (s->st != 0) return E_XC;
    unsigned int sh = hash((const unsigned char *)&secret, 4);
    unsigned int lv = ((unsigned int)s->hl[0]) | ((unsigned int)s->hl[1] << 8) |
                      ((unsigned int)s->hl[2] << 16) | ((unsigned int)s->hl[3] << 24);
    s->st = (sh == lv) ? 2 : 1; return E_OK;
}

/* Oracles */
static int reg_ora(const char *nm, int ot, unsigned int iv, unsigned int cf) {
    if (ocnt >= MAX_ORA) return E_ORA;
    ora_t *f = &ora[ocnt]; cstr(f->nm, nm, 32); f->ot = ot;
    f->v = iv; f->ts = bts; f->cf = cf; f->act = 1; f->sc = 1; f->rnd = 0;
    ocnt++; ps("  Oracle: "); ps(nm); ps(" type="); pi(ot); ps("\n"); return E_OK;
}
static unsigned int get_ora(const char *nm) {
    for (int i = 0; i < ocnt; i++) {
        const char *a = ora[i].nm, *b = nm; int m = 1;
        while (*a && *b) { if (*a != *b) { m = 0; break; } a++; b++; }
        if (*a != *b) m = 0;
        if (m && ora[i].act) {
            if (ora[i].ot == OR_DEC) return ora[i].v + (ora[i].rnd % 5) - 2;
            return ora[i].v;
        }
    } return 0;
}
static int upd_ora(int idx, unsigned int nv, unsigned int ts, unsigned int sc) {
    if (idx >= ocnt) return E_ORA;
    ora_t *f = &ora[idx]; if (!f->act) return E_ORA;
    f->v = nv; f->ts = ts; f->sc = sc; f->rnd++;
    if (ts + 3600 < bts) { f->act = 0; return E_ORA; } return E_OK;
}
static unsigned int gen_rng(unsigned int mn, unsigned int mx) {
    for (int i = 0; i < ocnt; i++)
        if (ora[i].ot == OR_RNG && ora[i].act) {
            unsigned int s = ora[i].v ^ bnum ^ bts;
            s = s * 1103515245 + 12345; unsigned int rg = mx - mn;
            return mn + ((rg == 0) ? 0 : (s % rg));
        }
    return mn;
}

/* Deploy */
static int deploy(const char *nm, int vt, const unsigned char *bc, unsigned int bl, unsigned int bal) {
    if (ccnt >= MAX_C || bl > MAX_BC) return E_OP;
    contract_t *c = &cs[ccnt]; cstr(c->nm, nm, 32); c->vm = vt;
    c->clen = bl; for (unsigned int i = 0; i < bl; i++) c->code[i] = bc[i];
    c->scnt = 0; c->status = ST_DEP; c->bal = bal; c->nonce = 0;
    unsigned int h = hash((const unsigned char *)nm, slen(nm));
    for (int i = 0; i < ADDR; i++) c->addr[i] = (h >> ((i & 3) * 8)) & 0xFF;
    ups[ccnt].pt = PX_NONE; ups[ccnt].init = 0;
    return ++ccnt - 1;
}

/* Main */
int main(void) {
    ps("=== Smart Contract Engine v1.0 ===\nChain:"); pi(cid);
    ps(" Block:"); pi(bnum); ps("\n\n");

    ps("[1] Deploy contracts\n");
    unsigned char evm_cd[] = { OP_PSH1, 0x0A, OP_PSH1, 0x14, OP_ADD, OP_PSH1, 0x05, OP_MUL,
        OP_DUP1, OP_PSH1, 0x00, OP_MST, OP_STOP };
    int i0 = deploy("TokenContract", VM_EVM, evm_cd, sizeof(evm_cd), 1000000);
    ps("  Token(EVM) idx="); pi(i0); ps("\n");

    unsigned char wasm_cd[] = { 0x41, 0x00, 0x00, 0x00, 0x2A, 0x41, 0x00, 0x00, 0x00, 0x3A, 0x01, 0x0B };
    int i1 = deploy("WasmNFT", VM_WASM, wasm_cd, sizeof(wasm_cd), 500000);
    ps("  NFT(WASM) idx="); pi(i1); ps("\n");

    unsigned char cust_cd[] = { 0x09, 0x00, 0x00, 0x07, 0x09, 0x01, 0x00, 0x08, 0x01, 0x02, 0x00, 0x01, 0x08 };
    int i2 = deploy("CustomDAO", VM_CUST, cust_cd, sizeof(cust_cd), 750000);
    ps("  DAO(Cust) idx="); pi(i2); ps("\n\n");

    ps("[2] Execute contracts\n");
    evm_t ec; evm_init(&ec, 1000000);
    evm_exec(&ec, evm_cd, sizeof(evm_cd), &cs[i0]);
    ps("  EVM res="); pi(ec.sp > 0 ? ec.stk[0] : 0);
    ps(" gas="); pi(ec.gas); ps(" err="); pi(ec.err); ps("\n");

    wasm_t wc; wasm_init(&wc, 500000);
    wasm_exec(&wc, wasm_cd, sizeof(wasm_cd));
    ps("  WASM res="); pi(wc.sp > 0 ? wc.stk[0] : 0); ps(" gas="); pi(wc.gas); ps("\n");

    cust_t cc; cust_init(&cc, 250000);
    cust_exec(&cc, cust_cd, sizeof(cust_cd));
    ps("  Cust r2="); pi(cc.r[2]); ps(" gas="); pi(cc.gas); ps("\n\n");

    ps("[3] Compile\n");
    const char *sol = "pragma solidity ^0.8.0; contract Token { }";
    comp_t cr1 = compile("Token.sol", CMP_SOL, (const unsigned char *)sol, slen(sol), 1);
    ps("  Solidity:"); ps(cr1.ok ? "OK" : "FAIL");
    ps(" bl="); pi(cr1.bl); ps(" hash="); ph(cr1.bh); ps("\n");

    const char *vyp = "name: public(String[32])\n@external\ndef foo(): pass";
    comp_t cr2 = compile("Token.vy", CMP_VYP, (const unsigned char *)vyp, slen(vyp), 0);
    ps("  Vyper:"); ps(cr2.ok ? "OK" : "FAIL"); ps(" bl="); pi(cr2.bl); ps("\n");

    const char *ink = "#[ink::contract] mod token { }";
    comp_t cr3 = compile("Token.rs", CMP_INK, (const unsigned char *)ink, slen(ink), 1);
    ps("  ink!:"); ps(cr3.ok ? "OK" : "FAIL"); ps(" bl="); pi(cr3.bl);
    ps(" opt="); pi(cr3.opt); ps("\n\n");

    ps("[4] Test\n");
    tres_t t1 = run_test("test_add", evm_cd, sizeof(evm_cd), 150);
    ps("  "); ps(t1.nm); ps(":"); ps(t1.pass ? "PASS" : "FAIL");
    ps(" gas="); pi(t1.gas); ps(" asrt="); pi(t1.asrt); ps("\n");

    cov_t cov = gen_cov(evm_cd, sizeof(evm_cd));
    ps("  Coverage:"); pi(cov.pct); ps("%\n");

    fuzz_t fz = run_fuzz(evm_cd, sizeof(evm_cd), 0xDEADBEEF, 64);
    ps("  Fuzz: rnd="); pi(fz.tst); ps(" fail="); pi(fz.fail); ps("\n\n");

    ps("[5] Upgrades\n");
    dep_proxy(PX_TRANS, i0); dep_proxy(PX_DIAM, i1);
    unsigned char v2[] = { OP_PSH1, 0xFF, OP_STOP };
    int i3 = deploy("TokenV2", VM_EVM, v2, sizeof(v2), 0);
    upgrade(i0, i3); add_facet(i1, i3);
    dep_proxy(PX_UUPS, i2);
    int i4 = deploy("DAOV2", VM_EVM, v2, sizeof(v2), 0);
    upgrade(i2, i4); ps("\n");

    ps("[6] Cross-chain\n");
    unsigned char pl[] = { 0x01, 0x02, 0x03, 0x04 };
    xc_send(0, 1, 1000, pl, sizeof(pl)); xc_send(1, 2, 2000, pl, sizeof(pl));
    mk_swap(100, 200, 3600); cf_swap(0, 0x12345678);
    ps("  Swap st:"); pi(sw[0].st); ps("\n\n");

    ps("[7] Oracles\n");
    reg_ora("ETH/USD", OR_PR, 300000, 99);
    reg_ora("VRF", OR_RNG, 0xCAFEBABE, 100);
    reg_ora("Weather", OR_EXT, 72, 95);
    reg_ora("Agg", OR_DEC, 300100, 98);
    ps("  ETH/USD="); pi(get_ora("ETH/USD")); ps("\n");
    ps("  RNG(1-100)="); pi(gen_rng(1, 100)); ps("\n");
    upd_ora(0, 305000, bts + 12, 5);
    ps("  ETH(upd)="); pi(get_ora("ETH/USD")); ps("\n");
    ps("  Agg="); pi(get_ora("Agg")); ps("\n\n");

    ps("=== Summary ===\nContracts:"); pi(ccnt);
    ps("\nGas:"); pi(gp.tot); ps("\nLogs:"); pi(lcnt);
    ps("\nXMsg:"); pi(xcnt); ps("\nSwaps:"); pi(scnt);
    ps("\nOracles:"); pi(ocnt); ps("\nDone.\n");
    return 0;
}
