/* metaverse_world: Virtual World Engine for AI-ASM OS (v55.0)
 * World building, space management, interaction, multiplayer sync,
 * resource streaming, 3D spatial audio.
 */
#include <stddef.h>
typedef unsigned long uintptr_t;
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define MAX_ENT 4096
#define MAX_REG 64
#define MAX_PLR 128
#define MAX_RES 1024
#define MAX_AUD 256
#define MAX_TRG 512
#define MAX_NPC 256
#define REG_SZ 64
#define TICK_HZ 30
#define SYNC_MS 33
#define FA 0x00000001U
#define FP 0x00000002U
#define FG 0x00000004U
#define FS 0x00000008U
#define FD 0x00000010U
#define FM 0x00000020U
#define FL0 0x00000040U
#define FL1 0x00000080U
#define FL2 0x00000100U
#define FAM 0x00000200U
#define FSP 0x00000400U
#define FV 0x00000800U
#define FR 0x00001000U
#define FPR 0x00002000U
#define FRO 0x00004000U
#define WX_CLEAR 0x00U
#define WX_RAIN 0x01U
#define WX_SNOW 0x02U
#define WX_FOG 0x03U
#define WX_STORM 0x04U
#define NS_IDLE 0
#define NS_PAT 1
#define NS_AGR 2
#define NS_ATK 3
typedef struct { float x, y, z; } V3;
typedef struct { float x, y, z, w; } Qt;
typedef struct { V3 p; Qt r; V3 v; V3 s; } Tf;
typedef struct { V3 mn, mx; } AB;
typedef struct {
    unsigned int f; char nm[32]; Tf tf; AB bd;
    unsigned int mi, mt, ow, rg; float ma, re; unsigned int sc;
} Ent;
typedef struct {
    unsigned int f, rg, id; int gx, gy, gz;
    unsigned int ec, pc; AB bd;
} Reg;
typedef struct {
    unsigned int f; char nm[32]; unsigned int ei, rg, hp;
    Tf tf; V3 vl; float lt; unsigned int la, ps;
} Plr;
typedef struct {
    unsigned int f, si, di, tr, cd; Tf st, dt;
} Tlp;
typedef struct {
    unsigned int f, ei, tt, si, cd; AB bd;
} Trg;
typedef struct {
    unsigned int f, ei, rg, bs, ti; char nm[32];
    float hp, ar, pr; V3 hm, pt;
} Npc;
typedef struct {
    unsigned int f; char pt[64];
    unsigned int sb, ld, gh, ll, rc; float sp;
} Res;
typedef struct {
    unsigned int f, ri, ae; V3 ps;
    float vo, pi, nd, xd, rm, at;
} Aud;
typedef struct {
    unsigned int tp, ea, eb; V3 cp, cn; float pn;
} Col;
typedef struct {
    unsigned int f, sq, ei, at, ts; Tf tf;
} Spk;
typedef struct {
    float gv, td, ws; unsigned char wx;
    unsigned int tk, ec, pc, rc, ac, tc, nc;
} Wld;
static Wld g_w;
static Ent g_e[MAX_ENT];
static Reg g_r[MAX_REG];
static Plr g_p[MAX_PLR];
static Res g_res[MAX_RES];
static Aud g_a[MAX_AUD];
static Trg g_t[MAX_TRG];
static Npc g_n[MAX_NPC];
static Spk g_nb[256];
static unsigned int g_ns;
static void w_init(void); static void w_tick(void); static void w_shut(void);
static void ph_step(float dt); static void wx_upd(float dt);
static void rg_upd(void); static void ns_tick(void);
static void au_upd(float dt); static void np_upd(float dt);
static void tg_chk(void); static void rs_strm(float dt);
static void lo_upd(void); static void lb_chk(void);
static void logm(const char *m);
static unsigned int al_ent(const char *n);
static unsigned int al_reg(int gx, int gy, int gz);
static unsigned int al_plr(const char *n);
static unsigned int al_res(const char *p);
static unsigned int al_aud(unsigned int ri, V3 ps);
static unsigned int al_trg(unsigned int ei, AB b, unsigned int t);
static unsigned int al_npc(const char *n, V3 ps);
static int ab_ov(AB a, AB b);
static V3 va(V3 a, V3 b); static V3 vs(V3 a, V3 b);
static V3 vc(V3 v, float s); static V3 vn(V3 v);
static float vl(V3 v); static float vd(V3 a, V3 b);
static void avl(Tf *t, V3 v, float dt);
static unsigned int pr_ent(unsigned int ei, unsigned int sq);
static void rb_ent(unsigned int ei, unsigned int sq);
static void spkt(Spk *pk);
static void cpy(char *d, const char *s, int m);
static V3 va(V3 a, V3 b) { V3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static V3 vs(V3 a, V3 b) { V3 r; r.x=a.x-b.x; r.y=a.y-b.y; r.z=a.z-b.z; return r; }
static V3 vc(V3 v, float s) { V3 r; r.x=v.x*s; r.y=v.y*s; r.z=v.z*s; return r; }
static float vl(V3 v) {
    float sm = v.x*v.x + v.y*v.y + v.z*v.z, g = sm*0.5F;
    if (g < 0.001F) return 0.0F;
    for (int i = 0; i < 8; i++) g = 0.5F*(g + sm/g);
    return g;
}
static V3 vn(V3 v) {
    float l = vl(v); V3 r;
    if (l < 0.001F) { r.x=0; r.y=0; r.z=0; return r; }
    float iv = 1.0F/l; r.x=v.x*iv; r.y=v.y*iv; r.z=v.z*iv; return r;
}
static float vd(V3 a, V3 b) { return vl(vs(a,b)); }
static int ab_ov(AB a, AB b) {
    if (a.mx.x < b.mn.x || a.mn.x > b.mx.x) return 0;
    if (a.mx.y < b.mn.y || a.mn.y > b.mx.y) return 0;
    if (a.mx.z < b.mn.z || a.mn.z > b.mx.z) return 0;
    return 1;
}
static void avl(Tf *t, V3 v, float dt) {
    t->p.x += v.x*dt; t->p.y += v.y*dt; t->p.z += v.z*dt;
}
static void cpy(char *d, const char *s, int m) {
    int i; for (i = 0; i < m-1 && s[i]; i++) d[i] = s[i]; d[m-1] = 0;
}
static void logm(const char *m) {
    host_print("[metaverse_world] "); host_print(m); host_print("\n");
}
static void clr_all(void) {
    unsigned int i;
    for (i = 0; i < MAX_ENT; i++) g_e[i].f = 0;
    for (i = 0; i < MAX_REG; i++) g_r[i].f = 0;
    for (i = 0; i < MAX_PLR; i++) g_p[i].f = 0;
    for (i = 0; i < MAX_RES; i++) g_res[i].f = 0;
    for (i = 0; i < MAX_AUD; i++) g_a[i].f = 0;
    for (i = 0; i < MAX_TRG; i++) g_t[i].f = 0;
    for (i = 0; i < MAX_NPC; i++) g_n[i].f = 0;
}
static void w_init(void) {
    g_w.gv = -9.81F; g_w.wx = WX_CLEAR; g_w.td = 12.0F;
    g_w.ws = 0.0F; g_w.tk = 0; g_w.ec = 0; g_w.pc = 0;
    g_w.rc = 0; g_w.ac = 0; g_w.tc = 0; g_w.nc = 0;
    g_ns = 0; clr_all(); logm("World initialized");
}
static void w_shut(void) { logm("World shutting down"); }
static unsigned int al_ent(const char *n) {
    unsigned int i;
    for (i = 1; i < MAX_ENT; i++) {
        if (!(g_e[i].f & FA)) {
            g_e[i].f = FA|FS; g_e[i].tf.p.x=0; g_e[i].tf.p.y=0; g_e[i].tf.p.z=0;
            g_e[i].tf.v.x=0; g_e[i].tf.v.y=0; g_e[i].tf.v.z=0;
            g_e[i].tf.s.x=1; g_e[i].tf.s.y=1; g_e[i].tf.s.z=1;
            g_e[i].ma=1; g_e[i].re=0.3F; g_e[i].rg=0; g_e[i].sc=0;
            g_e[i].ow=0; g_e[i].mi=0; g_e[i].mt=0;
            cpy(g_e[i].nm, n, 32); g_w.ec++; return i;
        }
    }
    logm("Entity pool exhausted"); return 0;
}
static unsigned int al_reg(int gx, int gy, int gz) {
    unsigned int i;
    for (i = 1; i < MAX_REG; i++) {
        if (!(g_r[i].f & FA)) {
            g_r[i].f = FA; g_r[i].gx=gx; g_r[i].gy=gy; g_r[i].gz=gz;
            g_r[i].ec=0; g_r[i].pc=0; g_r[i].id=i; g_r[i].rg=i;
            float h = (float)REG_SZ*0.5F;
            float bx=(float)gx*(float)REG_SZ, by=(float)gy*(float)REG_SZ, bz=(float)gz*(float)REG_SZ;
            g_r[i].bd.mn.x=bx-h; g_r[i].bd.mn.y=by-h; g_r[i].bd.mn.z=bz-h;
            g_r[i].bd.mx.x=bx+h; g_r[i].bd.mx.y=by+h; g_r[i].bd.mx.z=bz+h;
            g_w.rc++; return i;
        }
    }
    logm("Region pool exhausted"); return 0;
}
static void rg_upd(void) {
    unsigned int i, r;
    for (i = 0; i < MAX_ENT; i++) {
        if (!(g_e[i].f & FA)) continue;
        V3 p = g_e[i].tf.p;
        for (r = 1; r < MAX_REG; r++) {
            if (!(g_r[r].f & FA)) continue;
            if (p.x>=g_r[r].bd.mn.x && p.x<=g_r[r].bd.mx.x &&
                p.y>=g_r[r].bd.mn.y && p.y<=g_r[r].bd.mx.y &&
                p.z>=g_r[r].bd.mn.z && p.z<=g_r[r].bd.mx.z) {
                if (g_e[i].rg != r) {
                    unsigned int o = g_e[i].rg;
                    if (o > 0 && o < MAX_REG) g_r[o].ec--;
                    g_e[i].rg = r; g_r[r].ec++;
                }
                break;
            }
        }
    }
}
static void lb_chk(void) {
    unsigned int mx=0, mn=0x00FFFFFFU, mxr=0, mnr=0, r;
    for (r = 1; r < MAX_REG; r++) {
        if (!(g_r[r].f & FA)) continue;
        if (g_r[r].pc > mx) { mx = g_r[r].pc; mxr = r; }
        if (g_r[r].pc < mn && g_r[r].pc > 0) { mn = g_r[r].pc; mnr = r; }
    }
    if (mx > mn+4 && mxr && mnr) logm("Load balance: migrating players");
}
static unsigned int al_plr(const char *n) {
    unsigned int i;
    for (i = 0; i < MAX_PLR; i++) {
        if (!(g_p[i].f & FA)) {
            g_p[i].f = FA|FPR; g_p[i].tf.p.x=0; g_p[i].tf.p.y=0; g_p[i].tf.p.z=0;
            g_p[i].vl.x=0; g_p[i].vl.y=0; g_p[i].vl.z=0;
            g_p[i].lt=0; g_p[i].la=0; g_p[i].ps=0; g_p[i].hp=100;
            g_p[i].ei = al_ent(n); g_p[i].rg = 1;
            cpy(g_p[i].nm, n, 32); g_w.pc++; logm("Player joined"); return i;
        }
    }
    logm("Player pool exhausted"); return 0;
}
static unsigned int mk_tlp(unsigned int ei, Tf st, Tf dt, unsigned int tg) {
    unsigned int i;
    for (i = 0; i < MAX_ENT; i++) {
        if (!(g_e[i].f & FA)) {
            g_e[i].f = FA|FS; g_e[i].tf = st;
            g_e[i].rg = tg > 0 ? tg : 1; g_e[i].mi = 0xABCD0000U;
            (void)ei; (void)dt; return i;
        }
    }
    return 0;
}
static void tlp_chk(unsigned int pid) {
    if (pid >= MAX_PLR || !(g_p[pid].f & FA)) return;
    V3 pp = g_p[pid].tf.p; unsigned int i;
    for (i = 0; i < MAX_ENT; i++) {
        if (!(g_e[i].f & FA) || g_e[i].mi != 0xABCD0000U) continue;
        if (vd(pp, g_e[i].tf.p) < 2.0F) {
            logm("Player teleported"); g_p[pid].tf.p = g_e[i].tf.p;
        }
    }
}
static void ph_step(float dt) {
    unsigned int a, b; float gv = g_w.gv;
    for (a = 0; a < MAX_ENT; a++) {
        if (!(g_e[a].f & FA) || (g_e[a].f & FS)) continue;
        g_e[a].tf.v.y += gv*dt; avl(&g_e[a].tf, g_e[a].tf.v, dt);
        g_e[a].f |= FD;
    }
    for (a = 0; a < MAX_ENT; a++) {
        if (!(g_e[a].f & FA) || (g_e[a].f & FG)) continue;
        for (b = a+1; b < MAX_ENT; b++) {
            if (!(g_e[b].f & FA) || (g_e[b].f & FG)) continue;
            if (g_e[a].rg != g_e[b].rg) continue;
            if (ab_ov(g_e[a].bd, g_e[b].bd)) {
                float pen = g_e[a].bd.mx.y - g_e[b].bd.mn.y;
                if (pen > 0.0F && pen < 1.0F) {
                    g_e[a].tf.p.y += pen*0.5F; g_e[b].tf.p.y -= pen*0.5F;
                    g_e[a].tf.v.y = 0; g_e[b].tf.v.y = 0;
                }
            }
        }
    }
}
static void wx_upd(float dt) {
    g_w.td += dt*0.01F; if (g_w.td >= 24.0F) g_w.td -= 24.0F;
    g_w.ws += (float)(g_w.tk & 0x0FU)*0.001F - 0.005F;
    if (g_w.ws < 0.0F) g_w.ws = 0.0F;
    if (g_w.ws > 20.0F) g_w.ws = 20.0F;
}
static unsigned int al_trg(unsigned int ei, AB b, unsigned int t) {
    unsigned int i;
    for (i = 0; i < MAX_TRG; i++) {
        if (!(g_t[i].f & FA)) {
            g_t[i].f=FA; g_t[i].ei=ei; g_t[i].bd=b;
            g_t[i].tt=t; g_t[i].si=0; g_t[i].cd=1000;
            g_w.tc++; return i;
        }
    }
    return 0;
}
static void tg_chk(void) {
    unsigned int i, p;
    for (i = 0; i < MAX_TRG; i++) {
        if (!(g_t[i].f & FA)) continue;
        for (p = 0; p < MAX_PLR; p++) {
            if (!(g_p[p].f & FA)) continue;
            AB pb;
            pb.mn.x = g_p[p].tf.p.x-0.5F; pb.mn.y = g_p[p].tf.p.y-1.0F;
            pb.mn.z = g_p[p].tf.p.z-0.5F; pb.mx.x = g_p[p].tf.p.x+0.5F;
            pb.mx.y = g_p[p].tf.p.y+1.0F; pb.mx.z = g_p[p].tf.p.z+0.5F;
            if (ab_ov(pb, g_t[i].bd)) logm("Trigger activated");
        }
    }
}
static unsigned int al_npc(const char *n, V3 ps) {
    unsigned int i;
    for (i = 0; i < MAX_NPC; i++) {
        if (!(g_n[i].f & FA)) {
            g_n[i].f=FA; g_n[i].hm=ps; g_n[i].pt=ps;
            g_n[i].hp=100; g_n[i].ar=10; g_n[i].pr=5;
            g_n[i].bs=NS_IDLE; g_n[i].ti=0; g_n[i].rg=1;
            cpy(g_n[i].nm, n, 32);
            g_n[i].ei = al_ent(n);
            if (g_n[i].ei < MAX_ENT) g_e[g_n[i].ei].tf.p = ps;
            g_w.nc++; return i;
        }
    }
    return 0;
}
static void np_upd(float dt) {
    unsigned int i, p;
    for (i = 0; i < MAX_NPC; i++) {
        if (!(g_n[i].f & FA)) continue;
        unsigned int ei = g_n[i].ei;
        if (ei >= MAX_ENT || !(g_e[ei].f & FA)) continue;
        V3 np = g_e[ei].tf.p;
        if (g_n[i].bs == NS_PAT) {
            float d = vd(np, g_n[i].pt);
            if (d < 0.5F) {
                g_n[i].pt.x = g_n[i].hm.x + (float)(i*3 & 0x07U) - 4.0F;
                g_n[i].pt.z = g_n[i].hm.z + (float)(i*7 & 0x07U) - 4.0F;
            } else {
                V3 dr = vn(vs(g_n[i].pt, np)); float sp = 1.5F*dt;
                g_e[ei].tf.p.x += dr.x*sp; g_e[ei].tf.p.z += dr.z*sp;
            }
            for (p = 0; p < MAX_PLR; p++) {
                if (!(g_p[p].f & FA)) continue;
                if (vd(np, g_p[p].tf.p) < g_n[i].ar) {
                    g_n[i].bs = NS_AGR; g_n[i].ti = p; break;
                }
            }
        } else if (g_n[i].bs == NS_AGR) {
            unsigned int ti = g_n[i].ti;
            if (ti < MAX_PLR && (g_p[ti].f & FA)) {
                V3 tp = g_p[ti].tf.p; float td = vd(np, tp);
                if (td > g_n[i].ar*1.5F) { g_n[i].bs=NS_PAT; g_n[i].ti=0; }
                else if (td < 2.0F) { g_n[i].bs=NS_ATK; }
                else {
                    V3 dr = vn(vs(tp, np)); float sp = 3.0F*dt;
                    g_e[ei].tf.p.x += dr.x*sp; g_e[ei].tf.p.z += dr.z*sp;
                }
            } else { g_n[i].bs=NS_PAT; g_n[i].ti=0; }
        } else if (g_n[i].bs == NS_ATK) {
            unsigned int ti = g_n[i].ti;
            if (ti < MAX_PLR && (g_p[ti].f & FA)) {
                if (vd(np, g_p[ti].tf.p) > 3.0F) g_n[i].bs=NS_AGR;
            } else { g_n[i].bs=NS_PAT; g_n[i].ti=0; }
        }
        g_e[ei].f |= FD;
    }
}
static unsigned int al_res(const char *pt) {
    unsigned int i;
    for (i = 1; i < MAX_RES; i++) {
        if (!(g_res[i].f & FA)) {
            g_res[i].f=FA|FM; g_res[i].sb=0; g_res[i].ld=0;
            g_res[i].gh=0; g_res[i].sp=0; g_res[i].ll=3; g_res[i].rc=1;
            cpy(g_res[i].pt, pt, 64); g_w.rc++; return i;
        }
    }
    logm("Resource pool exhausted"); return 0;
}
static void rs_strm(float dt) {
    unsigned int i;
    for (i = 1; i < MAX_RES; i++) {
        if (!(g_res[i].f & FA) || !(g_res[i].f & FM)) continue;
        g_res[i].sp += dt*0.1F;
        g_res[i].ld = (unsigned int)(g_res[i].sp*(float)g_res[i].sb);
        if (g_res[i].sp >= 1.0F) {
            g_res[i].f &= ~FM; g_res[i].sp=1.0F;
            g_res[i].ld = g_res[i].sb; logm("Resource loaded");
        } else if (g_res[i].sp > 0.66F) {
            g_res[i].f = (g_res[i].f & ~(FL1|FL2)) | FL0;
        } else if (g_res[i].sp > 0.33F) {
            g_res[i].f = (g_res[i].f & ~(FL0|FL2)) | FL1;
        } else {
            g_res[i].f = (g_res[i].f & ~(FL0|FL1)) | FL2;
        }
    }
}
static void lo_upd(void) {
    unsigned int p, e;
    for (p = 0; p < MAX_PLR; p++) {
        if (!(g_p[p].f & FA)) continue;
        V3 pp = g_p[p].tf.p;
        for (e = 0; e < MAX_ENT; e++) {
            if (!(g_e[e].f & FA) || (g_e[e].f & FS)) continue;
            float d = vd(pp, g_e[e].tf.p);
            g_e[e].f &= ~(FL0|FL1|FL2);
            if (d < 20.0F) g_e[e].f |= FL0;
            else if (d < 50.0F) g_e[e].f |= FL1;
            else g_e[e].f |= FL2;
        }
    }
}
static unsigned int al_aud(unsigned int ri, V3 ps) {
    unsigned int i;
    for (i = 0; i < MAX_AUD; i++) {
        if (!(g_a[i].f & FA)) {
            g_a[i].f=FA|FSP; g_a[i].ri=ri; g_a[i].ps=ps;
            g_a[i].vo=1; g_a[i].pi=1; g_a[i].nd=1; g_a[i].xd=50;
            g_a[i].rm=0; g_a[i].at=1; g_a[i].ae=0;
            g_w.ac++; return i;
        }
    }
    return 0;
}
static void au_upd(float dt) {
    unsigned int i, p; (void)dt;
    for (i = 0; i < MAX_AUD; i++) {
        if (!(g_a[i].f & FA)) continue;
        if (g_a[i].ae > 0 && g_a[i].ae < MAX_ENT && (g_e[g_a[i].ae].f & FA))
            g_a[i].ps = g_e[g_a[i].ae].tf.p;
        for (p = 0; p < MAX_PLR; p++) {
            if (!(g_p[p].f & FA)) continue;
            float d = vd(g_a[i].ps, g_p[p].tf.p), at = 1.0F;
            if (d > g_a[i].nd) {
                at = 1.0F - (d - g_a[i].nd)/(g_a[i].xd - g_a[i].nd);
                if (at < 0.0F) at = 0.0F;
            }
            g_a[i].at = at; break;
        }
        if (g_a[i].f & FV) g_a[i].at = 1.0F;
        if ((g_a[i].f & FAM) && g_a[i].rm < 0.3F) g_a[i].rm += 0.001F;
    }
}
static void spkt(Spk *pk) {
    unsigned int idx = g_ns & 0x000000FFU; g_nb[idx] = *pk; g_ns++;
}
static unsigned int pr_ent(unsigned int ei, unsigned int sq) {
    if (ei >= MAX_ENT || !(g_e[ei].f & FA)) return 0;
    float dt = 1.0F/(float)TICK_HZ; Tf pr = g_e[ei].tf;
    avl(&pr, g_e[ei].tf.v, dt); g_e[ei].tf = pr;
    g_e[ei].f |= FPR; (void)sq; return 1;
}
static void rb_ent(unsigned int ei, unsigned int sq) {
    if (ei >= MAX_ENT) return; unsigned int i;
    for (i = 0; i < 256; i++) {
        if (g_nb[i].sq == sq && g_nb[i].ei == ei) {
            g_e[ei].tf = g_nb[i].tf;
            g_e[ei].f &= ~FPR; g_e[ei].f |= FRO;
            logm("Entity rolled back"); return;
        }
    }
}
static void ns_tick(void) {
    float dt = 1.0F/(float)TICK_HZ; unsigned int i, e;
    for (i = 0; i < MAX_PLR; i++) {
        if (!(g_p[i].f & FA)) continue;
        unsigned int ei = g_p[i].ei;
        if (ei >= MAX_ENT) continue;
        avl(&g_e[ei].tf, g_p[i].vl, dt);
        Spk pk; pk.f=FA; pk.sq=g_p[i].ps++;
        pk.ei=ei; pk.tf=g_e[ei].tf; pk.at=0;
        pk.ts = g_w.tk*SYNC_MS; spkt(&pk);
        g_e[ei].f |= FD;
        if (g_p[i].lt > 200.0F) logm("High latency player detected");
    }
    for (e = 0; e < MAX_ENT; e++) {
        if (!(g_e[e].f & FA) || !(g_e[e].f & FD)) continue;
        Spk pk; pk.f=FA; pk.sq=g_ns; pk.ei=e;
        pk.tf=g_e[e].tf; pk.at=1; pk.ts=g_w.tk*SYNC_MS;
        spkt(&pk); g_e[e].f &= ~FD;
    }
}
static void w_tick(void) {
    float dt = 1.0F/(float)TICK_HZ;
    wx_upd(dt); ph_step(dt); rg_upd(); tg_chk();
    np_upd(dt); rs_strm(dt); lo_upd(); au_upd(dt);
    ns_tick(); lb_chk(); g_w.tk++;
}
__attribute__((export_name("init"))) void w_e_init(void) { w_init(); }
__attribute__((export_name("tick"))) void w_e_tick(void) { w_tick(); }
__attribute__((export_name("shutdown"))) void w_e_shut(void) { w_shut(); }
__attribute__((export_name("spawn_entity")))
unsigned int w_e_spawn_ent(unsigned int off) { return al_ent((const char*)(uintptr_t)off); }
__attribute__((export_name("spawn_player")))
unsigned int w_e_spawn_plr(unsigned int off) { return al_plr((const char*)(uintptr_t)off); }
__attribute__((export_name("create_region")))
unsigned int w_e_mk_reg(int gx, int gy, int gz) { return al_reg(gx, gy, gz); }
__attribute__((export_name("spawn_npc")))
unsigned int w_e_spawn_npc(unsigned int off, float x, float y, float z) {
    V3 p; p.x=x; p.y=y; p.z=z;
    return al_npc((const char*)(uintptr_t)off, p);
}
__attribute__((export_name("load_resource")))
unsigned int w_e_load_res(unsigned int off) { return al_res((const char*)(uintptr_t)off); }
__attribute__((export_name("play_audio")))
unsigned int w_e_play_aud(unsigned int ri, float x, float y, float z) {
    V3 p; p.x=x; p.y=y; p.z=z; return al_aud(ri, p);
}
__attribute__((export_name("add_trigger")))
unsigned int w_e_add_trg(unsigned int ei, float nx, float ny, float nz,
    float xx, float xy, float xz, unsigned int tt) {
    AB b; b.mn.x=nx; b.mn.y=ny; b.mn.z=nz; b.mx.x=xx; b.mx.y=xy; b.mx.z=xz;
    return al_trg(ei, b, tt);
}
__attribute__((export_name("set_weather")))
void w_e_set_wx(unsigned int w) {
    if (w <= WX_STORM) { g_w.wx=(unsigned char)w; logm("Weather changed"); }
}
__attribute__((export_name("teleport")))
void w_e_teleport(unsigned int pid, float x, float y, float z) {
    if (pid < MAX_PLR && (g_p[pid].f & FA)) {
        g_p[pid].tf.p.x=x; g_p[pid].tf.p.y=y; g_p[pid].tf.p.z=z;
        logm("Player teleported");
    }
}
__attribute__((export_name("get_tick"))) unsigned int w_e_get_tick(void) { return g_w.tk; }
__attribute__((export_name("get_entity_count"))) unsigned int w_e_get_ec(void) { return g_w.ec; }
__attribute__((export_name("get_player_count"))) unsigned int w_e_get_pc(void) { return g_w.pc; }
__attribute__((export_name("get_region_count"))) unsigned int w_e_get_rc(void) { return g_w.rc; }
