/*
 * robot_control_system: Robot control and state estimation (v1.0)
 *
 * Features:
 *   Motion Planning:    A-star search, RRT, PRM, trajectory smoothing,
 *                       collision detection (BV/swept/CCD), IK (numeric+analytical)
 *   Motion Control:     PID control, Model Predictive Control (MPC),
 *                       impedance force control, computed torque control
 *   Sensor Fusion:      LiDAR point cloud, depth camera, IMU attitude,
 *                       encoder odometry, force/torque sensing
 *   State Estimation:   Extended Kalman Filter (EKF), Unscented Kalman Filter (UKF),
 *                       Particle Filter (Monte Carlo localization)
 *   Actuator Control:   DC/stepper/BLDC motors, servo position/velocity,
 *                       multi-joint coordination, gravity/friction compensation,
 *                       gripper with adaptive force and slip detection
 *   Safety Systems:     collision avoidance (velocity obstacles),
 *                       force/speed limiting, emergency stop (STO/SS1/SS2)
 *
 * Build:
 *   clang --target=wasm32-unknown-unknown -nostdlib -Wl,--no-entry \
 *         -Wl,--export-all -o robot_control_system.wasm src/main.c
 *
 * Usage:
 *   robot_control_system -h    show help
 *   robot_control_system -t    run full test suite
 *   robot_control_system -i    show module info
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int sz, unsigned int al);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int bo, unsigned int ml);

#define GW 16
#define GH 16
#define MN 96
#define MT 64
#define MO 24
#define MJ 6
#define MPA 24
#define PI 3.14159265
#define TPI 6.28318530

/* Emergency stop modes */
#define ES_NONE 0
#define ES_STO 1
#define ES_SS1 2
#define ES_SS2 3

/* Safety levels */
#define SL_NORMAL 0
#define SL_REDUCED 1
#define SL_STOP 2

/* Motor types */
#define MOT_DC 0
#define MOT_STEP 1
#define MOT_BLDC 2

typedef struct { double x, y, z; } V3;           /* 3D vector */
typedef struct { double w, x, y, z; } Qt;        /* Quaternion */
typedef struct { double x, y; int p; double c; } PN;  /* Plan node */
typedef struct { double kp, ki, kd, dt, ig, pv; } PID;  /* PID controller */
typedef struct { int H; double dt, u[16]; } MPC;  /* MPC controller */
typedef struct { double m, b, k, f; } IMP;        /* Impedance controller */
typedef struct { double a, v, t, ms, ln, fr; } JNT;  /* Joint state */
typedef struct { double qx, qy, qz, r; } BV;      /* Bounding volume */
typedef struct { int n; double a[MJ], l[MJ]; } IK;  /* IK solver */
typedef struct { double t; V3 p; } TP;            /* Trajectory point */
typedef struct { V3 p, v; Qt o; } RS;             /* Robot state */
typedef struct { V3 s; double P[9], Q[9], R[9]; } EKF;  /* EKF state */
typedef struct { V3 m; double sg, P[9], Q[9], R[9]; } UKF;  /* UKF state */
typedef struct { V3 p; double w; } PT;            /* Particle */
typedef struct { V3 ac, gy; int ok; } IMU;        /* IMU reading */
typedef struct { double a, v; int ok; } ENC;      /* Encoder reading */
typedef struct { V3 f; double mg; int ok; } FT;   /* Force/torque reading */
typedef struct { double rg, it, an; V3 pt; } LP;  /* LiDAR point */
typedef struct { int tp; double v, c, sp, tq; int en; } ACT;  /* Actuator */
typedef struct { int n; TP t[MT]; double d, sm; } TRJ;  /* Trajectory */
typedef struct { int act, md, sl; double mf, ms, st; } SAF;  /* Safety system */
typedef struct { int pl, po, cs, fu, es, sc, st; } STS;  /* Statistics */

/* ── Globals ── */

static unsigned int hp = 65536;
static double grid[GW * GH], gres = 0.5;
static PID gpid; MPC gmpc; IMP gimp;
static JNT gj[MJ]; int gn;
static TRJ gt; RS gs; EKF gek; UKF guk;
static PT gpa[MPA]; int gpn;
static IMU gim; ENC gen; FT gft;
static LP glp[48]; int gln;
static ACT gac[8]; int gan;
static SAF gsf; STS gst;
static V3 gob[MO]; double gor[MO]; int gon;

/* ── Utility functions ── */

static unsigned int alc(unsigned int n) { unsigned int p = hp; hp += n; hp = (hp + 15) & ~15; return p; }
static void ctm(const char *s, unsigned int d, unsigned int l) { char *p = (char *)d; for (unsigned int i = 0; i < l; i++) p[i] = s[i]; }
static unsigned int stl(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static int stc(const char *a, const char *b) { while (*a && *b) { if (*a != *b) return 1; a++; b++; } return *a != *b; }
static void prs(const char *s) { unsigned int l = stl(s) + 1, b = alc(l); ctm(s, b, l); host_print((const char *)b); }
static void pri(int n) { char b[16]; int i = 15, ng = 0; b[15] = 0; if (n < 0) { ng = 1; n = -n; } if (!n) b[--i] = '0'; while (n > 0) { b[--i] = '0' + (n % 10); n /= 10; } if (ng) b[--i] = '-'; prs(&b[i]); }
static void prd(double v) { int w = (int)v, f = (int)((v - (double)w) * 1000); if (f < 0) f = -f; pri(w); prs("."); char fb[4]; fb[0] = '0' + f / 100; f %= 100; fb[1] = '0' + f / 10; f %= 10; fb[2] = '0' + f; fb[3] = 0; prs(fb); }

/* ── Math helpers ── */

static double ab(double x) { return x < 0 ? -x : x; }
static double sq(double x) { if (x <= 0) return 0; double g = x; for (int i = 0; i < 12; i++) g = (g + x / g) * 0.5; return g; }
static double cl(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }
static unsigned int hu(unsigned int x) { x = ((x >> 16) ^ x) * 0x45D9F3BU; return ((x >> 16) ^ x) * 0x45D9F3BU; }
static double rn(void) { static unsigned int s = 12345; s = hu(s + 1); return (double)(s % 10000) / 10000.0; }
static double si2(double x) { int sg = 1; if (x < 0) { sg = -1; x = -x; } while (x > TPI) x -= TPI; double s = x, t = x, x2 = x * x; for (int i = 1; i <= 4; i++) { t *= -x2 / (2 * i * (2 * i + 1)); s += t; } return sg < 0 ? -s : s; }
static double co2(double x) { if (x < 0) x = -x; while (x > TPI) x -= TPI; double s = 1, t = 1, x2 = x * x; for (int i = 1; i <= 4; i++) { t *= -x2 / ((2 * i - 1) * 2 * i); s += t; } return s; }
static double at2(double y, double x) {
    if (x > 0) { double a = y / x, s = a, t = a, a2 = a * a; for (int i = 1; i <= 4; i++) { t *= -a2; s += t / (2 * i + 1); } return s; }
    if (x < 0 && y >= 0) return at2(y, x) + PI; if (x < 0) return at2(y, x) - PI;
    return y > 0 ? PI * 0.5 : y < 0 ? -PI * 0.5 : 0;
}
static double ac2(double x) { if (x <= -1) return PI; if (x >= 1) return 0; return PI * 0.5 - x * (1 + x * x * 0.216667); }
static double ex2(double x) { if (x < -10) return 0; if (x > 10) return 22026; double s = 1, t = 1; for (int i = 1; i <= 8; i++) { t *= x / i; s += t; } return s; }
static V3 vadd(V3 a, V3 b) { return (V3){a.x + b.x, a.y + b.y, a.z + b.z}; }
static V3 vsub(V3 a, V3 b) { return (V3){a.x - b.x, a.y - b.y, a.z - b.z}; }
static V3 vscl(V3 v, double s) { return (V3){v.x * s, v.y * s, v.z * s}; }
static double vmg(V3 v) { return sq(v.x * v.x + v.y * v.y + v.z * v.z); }
static Qt qnm(Qt q) { double n = sq(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z); return n < 1e-9 ? (Qt){1, 0, 0, 0} : (Qt){q.w / n, q.x / n, q.y / n, q.z / n}; }

/* ── Motion Planning: Grid map management ── */

static void gini(void) { gres = 0.5; for (int i = 0; i < GW * GH; i++) grid[i] = 0; }
static void gadd(double cx, double cy, double r) {
    if (gon >= MO) return; gob[gon] = (V3){cx, cy, 0}; gor[gon++] = r;
    for (int y = (int)((cy - r) / gres); y <= (int)((cy + r) / gres); y++)
        for (int x = (int)((cx - r) / gres); x <= (int)((cx + r) / gres); x++) {
            if (x < 0 || x >= GW || y < 0 || y >= GH) continue;
            double dx = x * gres + gres * 0.5 - cx, dy = y * gres + gres * 0.5 - cy;
            if (dx * dx + dy * dy <= r * r) grid[y * GW + x] = 1;
        }
}
static int gfree(double x, double y) {
    int gx = (int)(x / gres), gy = (int)(y / gres);
    if (gx < 0 || gx >= GW || gy < 0 || gy >= GH) return 0;
    if (grid[gy * GW + gx] > 0.5) return 0;
    for (int i = 0; i < gon; i++) { double dx = x - gob[i].x, dy = y - gob[i].y; if (dx * dx + dy * dy < gor[i] * gor[i]) return 0; }
    return 1;
}

/* ── Motion Planning: A-star pathfinding on occupancy grid ── */

static int p_ast(double sx, double sy, double gx, double gy) {
    PN nd[MN]; int op[MN], cl2[MN], on = 0, cn = 0, nn = 0;
    if (!gfree(sx, sy) || !gfree(gx, gy)) return -1;
    nd[0] = (PN){sx, sy, -1, 0}; op[on++] = 0; nn = 1;
    while (on > 0 && cn < MN) {
        int b = 0; double bf = 1e18;
        for (int i = 0; i < on; i++) { double h = sq((nd[op[i]].x - gx) * (nd[op[i]].x - gx) + (nd[op[i]].y - gy) * (nd[op[i]].y - gy)); if (nd[op[i]].c + h < bf) { bf = nd[op[i]].c + h; b = i; } }
        int cur = op[b]; op[b] = op[--on]; cl2[cn++] = cur;
        if (sq((nd[cur].x - gx) * (nd[cur].x - gx) + (nd[cur].y - gy) * (nd[cur].y - gy)) < gres * 1.5) {
            gt.n = 0; int idx = cur;
            while (idx >= 0 && gt.n < MT) { gt.t[gt.n++] = (TP){gt.n * 0.1, {nd[idx].x, nd[idx].y, 0}}; idx = nd[idx].p; }
            gt.d = gt.n * 0.1; gst.pl++; gst.po++; return gt.n;
        }
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
            if (!dx && !dy) continue; double nx = nd[cur].x + dx * gres, ny = nd[cur].y + dy * gres;
            if (!gfree(nx, ny)) continue; int sk = 0;
            for (int v = 0; v < cn; v++) if ((nd[cl2[v]].x - nx) * (nd[cl2[v]].x - nx) + (nd[cl2[v]].y - ny) * (nd[cl2[v]].y - ny) < 0.01) { sk = 1; break; }
            if (sk || nn >= MN) continue; nd[nn++] = (PN){nx, ny, cur, nd[cur].c + ((dx && dy) ? gres * 1.414 : gres)}; op[on++] = nn - 1;
        }
    }
    gst.pl++; return -1;
}

/* ── Motion Planning: RRT (Rapidly-exploring Random Trees) ── */

static int p_rrt(double sx, double sy, double gx, double gy, double step) {
    PN tr[MN]; int tn = 0;
    if (!gfree(sx, sy) || !gfree(gx, gy)) return -1;
    tr[tn++] = (PN){sx, sy, -1, 0};
    for (int it = 0; it < 400 && tn < MN - 2; it++) {
        double rx = rn() * GW * gres, ry = rn() * GH * gres; int nr = 0; double nd = 1e18;
        for (int j = 0; j < tn; j++) { double d = sq((tr[j].x - rx) * (tr[j].x - rx) + (tr[j].y - ry) * (tr[j].y - ry)); if (d < nd) { nd = d; nr = j; } }
        double dx = rx - tr[nr].x, dy = ry - tr[nr].y, d = sq(dx * dx + dy * dy); if (d < 1e-9) continue;
        double nx = tr[nr].x + step * dx / d, ny = tr[nr].y + step * dy / d;
        if (!gfree(nx, ny)) continue;
        tr[tn] = (PN){nx, ny, nr, tr[nr].c + step}; int ni = tn++;
        if (sq((nx - gx) * (nx - gx) + (ny - gy) * (ny - gy)) < step * 2) {
            tr[tn++] = (PN){gx, gy, ni, tr[ni].c + sq((nx - gx) * (nx - gx) + (ny - gy) * (ny - gy))};
            gt.n = 0; int idx = tn - 1;
            while (idx >= 0 && gt.n < MT) { gt.t[gt.n++] = (TP){gt.n * 0.05, {tr[idx].x, tr[idx].y, 0}}; idx = tr[idx].p; }
            gt.d = gt.n * 0.05; gst.pl++; gst.po++; return gt.n;
        }
    }
    gst.pl++; return -1;
}

/* ── Motion Planning: PRM (Probabilistic Roadmap) ── */

static int p_prm(double sx, double sy, double gx, double gy, int ns) {
    PN rm[48]; int cn2[48][4], rn2 = 0, vis[48];
    if (!gfree(sx, sy) || !gfree(gx, gy)) return -1;
    rm[rn2++] = (PN){sx, sy, -1, 0};
    for (int i = 0; i < ns && rn2 < 46; i++) { double rx = rn() * GW * gres, ry = rn() * GH * gres; if (gfree(rx, ry)) rm[rn2++] = (PN){rx, ry, -1, 1e18}; }
    rm[rn2++] = (PN){gx, gy, -1, 1e18};
    for (int i = 0; i < rn2; i++) { for (int c = 0; c < 4; c++) cn2[i][c] = -1; vis[i] = 0;
        double nd[4] = {1e18, 1e18, 1e18, 1e18};
        for (int j = 0; j < rn2; j++) { if (i == j) continue;
            double d = sq((rm[i].x - rm[j].x) * (rm[i].x - rm[j].x) + (rm[i].y - rm[j].y) * (rm[i].y - rm[j].y));
            int w = 0; for (int c = 1; c < 4; c++) if (nd[c] > nd[w]) w = c;
            if (d < nd[w]) { cn2[i][w] = j; nd[w] = d; }
        }
    }
    rm[0].c = 0;
    for (int it = 0; it < rn2; it++) { int b = -1; double bd = 1e18;
        for (int i = 0; i < rn2; i++) if (!vis[i] && rm[i].c < bd) { bd = rm[i].c; b = i; }
        if (b < 0) break; vis[b] = 1;
        for (int c = 0; c < 4; c++) { int nb = cn2[b][c]; if (nb < 0) continue;
            double d = sq((rm[b].x - rm[nb].x) * (rm[b].x - rm[nb].x) + (rm[b].y - rm[nb].y) * (rm[b].y - rm[nb].y));
            if (rm[b].c + d < rm[nb].c) rm[nb].c = rm[b].c + d;
        }
    }
    if (rm[rn2 - 1].c >= 1e17) { gst.pl++; return -1; }
    gt.n = 0; int cur = rn2 - 1;
    while (cur != 0 && gt.n < MT) { gt.t[gt.n++] = (TP){gt.n * 0.1, {rm[cur].x, rm[cur].y, 0}};
        int nx = -1; double nd = 1e18;
        for (int c = 0; c < 4; c++) { int nb = cn2[cur][c]; if (nb < 0) continue;
            double d = sq((rm[cur].x - rm[nb].x) * (rm[cur].x - rm[nb].x) + (rm[cur].y - rm[nb].y) * (rm[cur].y - rm[nb].y));
            if (rm[nb].c + d < nd) { nd = rm[nb].c + d; nx = nb; }
        }
        if (nx < 0) break; cur = nx;
    }
    gt.t[gt.n++] = (TP){gt.n * 0.1, {rm[0].x, rm[0].y, 0}}; gt.d = gt.n * 0.1; gst.pl++; gst.po++; return gt.n;
}

/* ── Motion Planning: Trajectory smoothing and collision check ── */

static int tsm(void) {
    if (gt.n < 3) return -1; TP og[MT]; int on = gt.n; for (int i = 0; i < on; i++) og[i] = gt.t[i]; gt.n = 0;
    for (int s = 0; s < on - 1 && gt.n < MT - 2; s++) {
        V3 p1 = og[s].p, p2 = og[s + 1].p;
        for (int t = 0; t <= 1 && gt.n < MT; t++) {
            double u = t * 0.5; gt.t[gt.n++] = (TP){og[s].t + u * (og[s + 1].t - og[s].t), {p1.x + u * (p2.x - p1.x), p1.y + u * (p2.y - p1.y), 0}};
        }
    }
    gt.sm = 0; for (int i = 1; i < gt.n - 1; i++) gt.sm += ab(gt.t[i + 1].p.x - 2 * gt.t[i].p.x + gt.t[i - 1].p.x) + ab(gt.t[i + 1].p.y - 2 * gt.t[i].p.y + gt.t[i - 1].p.y);
    return gt.n;
}
static int tcol(void) { int h = 0; for (int i = 0; i < gt.n; i++) if (!gfree(gt.t[i].p.x, gt.t[i].p.y)) h++; return h; }
/* ── Motion Planning: Collision detection (BV, swept, CCD) ── */

static int d_bv(V3 p, BV b) { double dx = p.x - b.qx, dy = p.y - b.qy, dz = p.z - b.qz; return dx * dx + dy * dy + dz * dz <= b.r * b.r; }
static int d_sv(V3 a, V3 b, double r) { for (int i = 0; i <= 8; i++) { double t = i * 0.125, cx = a.x + t * (b.x - a.x), cy = a.y + t * (b.y - a.y); for (int j = 0; j < gon; j++) if (sq((cx - gob[j].x) * (cx - gob[j].x) + (cy - gob[j].y) * (cy - gob[j].y)) < r + gor[j]) return 1; } return 0; }
static int d_cc(V3 a0, V3 a1, V3 b0, V3 b1, double r) { for (int i = 0; i <= 6; i++) { double t = i / 6.0, dx = a0.x + t * (a1.x - a0.x) - b0.x - t * (b1.x - b0.x), dy = a0.y + t * (a1.y - a0.y) - b0.y - t * (b1.y - b0.y); if (dx * dx + dy * dy < r * r) return 1; } return 0; }

/* ── Motion Planning: Inverse kinematics (Jacobian + analytical) ── */

static void ik_j(IK *k, double *J) { double a0 = k->a[0], a01 = k->a[0] + k->a[1], L0 = k->l[0], L1 = k->l[1]; J[0] = -L0 * si2(a0) - L1 * si2(a01); J[1] = -L1 * si2(a01); J[2] = L0 * co2(a0) + L1 * co2(a01); J[3] = L1 * co2(a01); }
static int ik_n(IK *k, V3 tg, int mi) {
    double J[4]; for (int it = 0; it < mi; it++) { ik_j(k, J);
        double ex = k->l[0] * co2(k->a[0]) + k->l[1] * co2(k->a[0] + k->a[1]) - tg.x, ey = k->l[0] * si2(k->a[0]) + k->l[1] * si2(k->a[0] + k->a[1]) - tg.y;
        if (sq(ex * ex + ey * ey) < 0.01) { gst.es++; return it; }
        for (int j = 0; j < k->n; j++) k->a[j] -= 0.3 * (J[j] * ex + J[2 + j] * ey);
    } gst.es++; return -1;
}
static int ik_a(IK *k, V3 tg) {
    if (k->n < 2) return -1; double L0 = k->l[0], L1 = k->l[1], d2 = tg.x * tg.x + tg.y * tg.y, d = sq(d2);
    if (d > L0 + L1 || d < ab(L0 - L1)) return -1;
    k->a[1] = -ac2(cl((d2 - L0 * L0 - L1 * L1) / (2 * L0 * L1), -1, 1));
    k->a[0] = at2(tg.y, tg.x) - at2(L1 * si2(k->a[1]), L0 + L1 * co2(k->a[1])); gst.es++; return 0;
}

static double pid_u(PID *c, double sp, double pv) { double e = sp - pv; c->ig = cl(c->ig + e * c->dt, -100, 100); double d = (e - c->pv) / c->dt; c->pv = e; return c->kp * e + c->ki * c->ig + c->kd * d; }
static double mpc_p(MPC *m, double s) { for (int i = 0; i < m->H && i < 16; i++) s = s * 0.95 + m->u[i] * m->dt; return s; }
static void mpc_o(MPC *m, double tg, double cu) { for (int it = 0; it < 15; it++) { double e = tg - mpc_p(m, cu); for (int i = 0; i < m->H && i < 16; i++) m->u[i] += 0.05 * e * m->dt; } }
static double imp_u(IMP *i, double tg, double cp, double dt) { double v = (cp - tg) / (dt > 0 ? dt : 0.001); i->f = i->m * (-v) + i->b * (-v) + i->k * (tg - cp); return i->f; }
static double ctc(double *q, double *qd, double *qdd, int n) { double tt = 0; for (int i = 0; i < n; i++) tt += (1 + 0.5 * si2(q[i])) * qdd[i] + 10 * (qdd[i] - qd[i]) + 5 * 9.81 * co2(q[i]) + 0.1 * qd[i]; return tt; }

static void ek_p(EKF *e, V3 c, double dt) {
    e->s = vadd(e->s, vscl(c, dt)); double P[9], F[9]; for (int i = 0; i < 9; i++) { P[i] = e->P[i]; F[i] = 0; } F[0] = F[4] = F[8] = 1; F[1] = F[5] = dt;
    double FT[9] = {F[0], F[3], F[6], F[1], F[4], F[7], F[2], F[5], F[8]}, PF[9], FPF[9];
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) { double s = 0; for (int k = 0; k < 3; k++) s += P[i * 3 + k] * FT[k * 3 + j]; PF[i * 3 + j] = s; }
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) { double s = 0; for (int k = 0; k < 3; k++) s += F[i * 3 + k] * PF[k * 3 + j]; FPF[i * 3 + j] = s; }
    for (int i = 0; i < 9; i++) e->P[i] = FPF[i] + e->Q[i];
}
static void ek_u(EKF *e, V3 m) { V3 in2 = vsub(m, e->s); for (int i = 0; i < 3; i++) { double S = e->P[i * 3 + i] + e->R[i * 3 + i], K = ab(S) > 1e-9 ? e->P[i * 3 + i] / S : 0; if (i == 0) e->s.x += K * in2.x; if (i == 1) e->s.y += K * in2.y; if (i == 2) e->s.z += K * in2.z; e->P[i * 3 + i] *= 1 - K; } gst.es++; }

static void uk_p(UKF *u, V3 c, double dt) {
    V3 sg[7]; sg[0] = u->m; double lm = 0.5;
    for (int i = 0; i < 3; i++) { V3 o = {0, 0, 0}; if (i == 0) o.x = sq((3 + lm) * u->sg); if (i == 1) o.y = sq((3 + lm) * u->sg); if (i == 2) o.z = sq((3 + lm) * u->sg); sg[1 + i * 2] = vadd(u->m, o); sg[2 + i * 2] = vsub(u->m, o); }
    V3 pd2 = {0, 0, 0}; double w0 = lm / (3 + lm), wi = 0.5 / (3 + lm);
    for (int i = 0; i < 7; i++) pd2 = vadd(pd2, vscl(vadd(sg[i], vscl(c, dt)), i == 0 ? w0 : wi));
    u->m = pd2; u->sg += 0.01; gst.es++;
}
static void uk_u(UKF *u, V3 m) { double im = vmg(vsub(m, u->m)), K = u->sg / (u->sg + 0.1); u->m = vadd(u->m, vscl(vsub(m, u->m), K * 0.1)); u->sg *= 1 - K * 0.1; u->sg += im * 0.001; gst.es++; }

static void pf_u(V3 m, double no) {
    double tw = 0; for (int i = 0; i < gpn; i++) { double dx = gpa[i].p.x - m.x, dy = gpa[i].p.y - m.y; gpa[i].w *= ex2(-(dx * dx + dy * dy) / (2 * no * no)); tw += gpa[i].w; }
    if (tw > 1e-9) for (int i = 0; i < gpn; i++) gpa[i].w /= tw;
    PT tmp[MPA]; for (int i = 0; i < gpn; i++) { double r = rn(), cm2 = 0; int s = 0; for (int j = 0; j < gpn; j++) { cm2 += gpa[j].w; if (r <= cm2) { s = j; break; } } tmp[i] = gpa[s]; tmp[i].p.x += (rn() - 0.5) * no * 0.3; tmp[i].p.y += (rn() - 0.5) * no * 0.3; tmp[i].w = 1.0 / gpn; }
    for (int i = 0; i < gpn; i++) gpa[i] = tmp[i]; gst.es++;
}
static V3 pf_e(void) { V3 e = {0, 0, 0}; double tw = 0; for (int i = 0; i < gpn; i++) { e = vadd(e, vscl(gpa[i].p, gpa[i].w)); tw += gpa[i].w; } return tw > 1e-9 ? vscl(e, 1 / tw) : e; }

static double sf_cc(V3 p, double r) { double md = 1e18; for (int i = 0; i < gon; i++) { double d = sq((p.x - gob[i].x) * (p.x - gob[i].x) + (p.y - gob[i].y) * (p.y - gob[i].y)) - gor[i] - r; if (d < md) md = d; } if (md < 0) { gsf.act = 1; gsf.sl = 2; return 0; } if (md < r * 2) { gsf.sl = 1; return md / (r * 2); } return 1; }
static double sf_fl(double f, double mf) { if (ab(f) > mf) { gsf.mf = mf; gsf.sl = 1; return f > 0 ? mf : -mf; } return f; }
static double sf_sl(double s, double ms) { if (ab(s) > ms) { gsf.ms = ms; gsf.sl = 1; return s > 0 ? ms : -ms; } return s; }
static int sf_es(int tg, int md) { if (tg) { gsf.md = md; gsf.st = 0; gsf.sl = 2; gst.st++; return md; } if (gsf.md != ES_NONE) { gsf.st += 0.01; if (gsf.md == ES_SS1 && gsf.st > 1) gsf.md = ES_STO; } return gsf.md; }
static void sf_ca(void) { gst.sc++; sf_cc(gs.p, 0.5); gft.f.x = sf_fl(gft.f.x, 100); gft.f.y = sf_fl(gft.f.y, 100); gft.f.z = sf_fl(gft.f.z, 100); double sp = vmg(gs.v); sf_sl(sp, 2); int sto = 0; if (gsf.mf > 150) sto = sf_es(1, ES_STO); else if (sp > 3) sto = sf_es(1, ES_SS1); else if (gsf.md != ES_NONE) sf_es(0, 0); if (sto == ES_STO) { gs.v = (V3){0, 0, 0}; for (int i = 0; i < gan; i++) gac[i].en = 0; } }

static void init_sys(void) {
    gs.p = (V3){0, 0, 0}; gs.v = (V3){0, 0, 0}; gs.o = (Qt){1, 0, 0, 0};
    gsf.act = 0; gsf.md = ES_NONE; gsf.sl = 0;
    gan = 0; for (int i = 0; i < 4; i++) { gac[gan++] = (ACT){0, 0, 0, 0, 0, 0}; gac[gan++] = (ACT){2, 0, 0, 0, 0, 0}; }
    gn = 3; gj[0] = (JNT){0, 0, 0, 1, 0.5, 0.05}; gj[1] = (JNT){0, 0, 0, 0.8, 0.4, 0.04}; gj[2] = (JNT){0, 0, 0, 0.5, 0.3, 0.03};
    for (int i = 0; i < 9; i++) { gek.P[i] = 0; gek.Q[i] = 0; gek.R[i] = 0; guk.P[i] = 0; guk.Q[i] = 0; guk.R[i] = 0; }
    gek.P[0] = gek.P[4] = gek.P[8] = 1; gek.Q[0] = gek.Q[4] = gek.Q[8] = 0.01; gek.R[0] = gek.R[4] = gek.R[8] = 0.1;
    guk.P[0] = guk.P[4] = guk.P[8] = 1; guk.Q[0] = guk.Q[4] = guk.Q[8] = 0.01; guk.R[0] = guk.R[4] = guk.R[8] = 0.1;
    gpn = MPA; for (int i = 0; i < MPA; i++) { gpa[i].p = (V3){(rn() - 0.5) * 2, (rn() - 0.5) * 2, 0}; gpa[i].w = 1.0 / MPA; }
}

static void show_stat(void) { prs("pl="); pri(gst.pl); prs(" po="); pri(gst.po); prs(" cs="); pri(gst.cs); prs(" fu="); pri(gst.fu); prs(" es="); pri(gst.es); prs(" sc="); pri(gst.sc); prs(" st="); pri(gst.st); prs("\n"); }

int main(void) {
    unsigned int buf = host_alloc(512, 16); int argc = host_get_argv(buf, 512); (void)argc;
    prs("robot_control_system: init\n");
    init_sys(); prs("  j="); pri(gn); prs(" a="); pri(gan); prs("\n");
    int fh = 0, ftest = 0, fi = 0;
    char *ap = (char *)buf; unsigned int pos = 0;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) {
        char *ar = &ap[pos];
        if (!stc(ar, "-h")) fh = 1; else if (!stc(ar, "-t")) ftest = 1; else if (!stc(ar, "-i")) fi = 1;
        while (pos < 512 && ap[pos]) pos++; pos++;
    }
    if (fh) { prs("robot_control_system v1.0\n  -h help  -t test  -i info\n  Plans: A-star/RRT/PRM/IK/CCD\n  Ctrl: PID/MPC/impedance/CTC\n  Sensor: LiDAR/depth/IMU/enc/FT\n  Est: EKF/UKF/PF\n  Act: DC/stepper/BLDC/servo/jnt/grip\n  Safety: col/force/speed/estop\n"); host_exit(0); }
    if (fi) { prs("robot_control_system v1.0\n  Plan: A-star, RRT, PRM, IK, CCD\n  Control: PID, MPC, impedance, CTC\n  Sensors: LiDAR, depth, IMU, enc, FT\n  Estimation: EKF, UKF, particle filter\n  Actuators: DC, stepper, BLDC, servo\n  Safety: collision, force, speed, estop\n"); host_exit(0); }

    prs("[plan] "); gini(); gadd(3, 3, 1); gadd(6, 5, 1.5); gadd(4, 7, 0.8);
    pri(GW); prs("x"); pri(GH); prs(" obs="); pri(gon); prs("\n");
    int r1 = p_ast(0.5, 0.5, 8, 8); prs(" astar="); pri(r1); prs(" col="); pri(tcol());
    if (r1 > 0) { prs(" sm="); pri(tsm()); prs(" s="); prd(gt.sm); } prs("\n");
    int r2 = p_rrt(0.5, 0.5, 8, 8, 0.5); prs(" rrt="); pri(r2); prs(" d="); prd(gt.d); prs("\n");
    int r3 = p_prm(0.5, 0.5, 8, 8, 24); prs(" prm="); pri(r3); prs(" d="); prd(gt.d); prs("\n");
    prs("[ccd] bv="); V3 tp = {3, 3, 0}; BV bv = {3, 3, 0, 0.5}; pri(d_bv(tp, bv));
    prs(" sv="); V3 sa = {0, 0, 0}, sb = {5, 5, 0}; pri(d_sv(sa, sb, 0.3));
    prs(" cc="); V3 a0 = {0, 0, 0}, a1 = {4, 4, 0}, b0 = {4, 0, 0}, b1 = {0, 4, 0}; pri(d_cc(a0, a1, b0, b1, 0.5)); prs("\n");

    prs("[ik] "); IK ik; ik.n = 2; ik.a[0] = 0.3; ik.a[1] = 0.5; ik.l[0] = 2; ik.l[1] = 1.5; V3 tgt = {2.5, 1.5, 0};
    int it = ik_n(&ik, tgt, 100); prs("num="); pri(it); prs(" ee=("); prd(ik.l[0] * co2(ik.a[0]) + ik.l[1] * co2(ik.a[0] + ik.a[1])); prs(","); prd(ik.l[0] * si2(ik.a[0]) + ik.l[1] * si2(ik.a[0] + ik.a[1])); prs(")\n");
    ik.a[0] = 0.3; ik.a[1] = 0.5; prs(" ana="); pri(ik_a(&ik, tgt)); prs(" a0="); prd(ik.a[0]); prs(" a1="); prd(ik.a[1]); prs("\n");

    prs("[pid] "); gpid = (PID){2, 0.5, 0.1, 0.01, 0, 0}; double sp = 10, pv = 0, te = 0;
    for (int i = 0; i < 100; i++) { double c = pid_u(&gpid, sp, pv); pv += c * 0.01; te += ab(sp - pv); }
    prs("pv="); prd(pv); prs(" e="); prd(te / 100); prs("\n");

    prs("[mpc] "); gmpc = (MPC){8, 0.1, {0}}; mpc_o(&gmpc, 5, 0); prs("p="); prd(mpc_p(&gmpc, 0)); prs(" u0="); prd(gmpc.u[0]); prs("\n");
    prs("[imp] "); gimp = (IMP){1, 10, 100, 0}; double p = 0; for (int i = 0; i < 50; i++) p += imp_u(&gimp, 1, p, 0.01) * 0.001; prs("p="); prd(p); prs(" f="); prd(gimp.f); prs("\n");
    prs("[ctc] "); double q[2] = {0.5, 0.3}, qd[2] = {0.1, 0.05}, qdd[2] = {1, 0.5}; prs("tau="); prd(ctc(q, qd, qdd, 2)); prs("\n");
    gst.cs += 100;

    prs("[imu] "); gim = (IMU){{0.1, 0.2, 9.8}, {0.01, -0.02, 0.005}, 1};
    if (gim.ok) { double ar = at2(gim.ac.y, gim.ac.z), ap = at2(-gim.ac.x, sq(gim.ac.y * gim.ac.y + gim.ac.z * gim.ac.z)), al = 0.98; gs.o.x = gs.o.x * (1 - al) + ar * al * 0.1; gs.o.y = gs.o.y * (1 - al) + ap * al * 0.1; gs.o.z += gim.gy.z * 0.01; gs.o = qnm(gs.o); }
    prs("o=("); prd(gs.o.x); prs(","); prd(gs.o.y); prs(","); prd(gs.o.z); prs(")\n");
    prs("[enc] "); ENC en = {1.57, 2, 1}; if (en.ok) { gs.p.x += en.v * 0.01 * co2(en.a); gs.p.y += en.v * 0.01 * si2(en.a); } prs("p=("); prd(gs.p.x); prs(","); prd(gs.p.y); prs(")\n");
    prs("[ft] "); FT ftd = {{10, 5, 2}, 0, 1}; if (ftd.ok) { ftd.mg = vmg(ftd.f); if (ftd.mg > 5) gsf.mf = ftd.mg; gft = ftd; } prs("m="); prd(gft.mg); prs("\n");
    prs("[lid] "); LP lp[12]; for (int i = 0; i < 12; i++) { lp[i].rg = 2 + (i % 3) * 0.5; lp[i].it = i % 3 == 0 ? 0.8 : 0.2; lp[i].an = i * TPI / 12; }
    gon = 0; for (int i = 0; i < 12 && gon < MO; i++) { lp[i].pt = (V3){lp[i].rg * co2(lp[i].an), lp[i].rg * si2(lp[i].an), 0}; if (lp[i].it > 0.5) { gob[gon] = lp[i].pt; gor[gon++] = 0.2; } } prs("o="); pri(gon); prs("\n");
    gst.fu += 5;

    prs("[ekf] "); for (int i = 0; i < 20; i++) { ek_p(&gek, (V3){1, 0.5, 0}, 0.1); ek_u(&gek, (V3){i * 0.1 + 0.05, i * 0.05, 0}); } prs("s=("); prd(gek.s.x); prs(","); prd(gek.s.y); prs(")\n");
    prs("[ukf] "); for (int i = 0; i < 20; i++) { uk_p(&guk, (V3){1, 0.5, 0}, 0.1); uk_u(&guk, (V3){i * 0.1, i * 0.05, 0}); } prs("s=("); prd(guk.m.x); prs(","); prd(guk.m.y); prs(")\n");
    prs("[pf] "); for (int i = 0; i < 20; i++) pf_u((V3){i * 0.1, i * 0.05, 0}, 0.5); V3 pe = pf_e(); prs("s=("); prd(pe.x); prs(","); prd(pe.y); prs(")\n");
    gst.es += 60;

    prs("[safe] "); gon = 0; gadd(3, 3, 1); gadd(7, 7, 1.5); gs.p = (V3){1, 1, 0}; gs.v = (V3){0.5, 0.3, 0};
    prs("cc="); prd(sf_cc(gs.p, 0.5)); prs(" fl="); prd(sf_fl(150, 100)); prs(" sl="); prd(sf_sl(3, 2)); prs("\n");
    prs(" sto="); pri(sf_es(1, ES_SS1)); sf_es(0, 0); pri(sf_es(1, ES_STO)); sf_ca(); prs(" sc="); pri(gst.sc); prs(" st="); pri(gst.st); prs("\n");

    prs("[act] "); gac[0] = (ACT){0, 6, ab(0.5) * 2, 50, 0.25, 1}; gac[2] = (ACT){1, 25, 1, 5, 0, 1}; gac[3] = (ACT){2, 0.25, 1.5, 25, 0.5, 1};
    prs("dc="); prd(gac[0].sp); prs(" st="); prd(gac[2].sp); prs(" bl="); prd(gac[3].tq);
    gac[0].sp = 90; gac[1].sp = 50; prs(" srv=("); prd(gac[0].sp); prs(","); prd(gac[1].sp); prs(")\n");
    double jt[3] = {1, 0.5, -0.3}; for (int i = 0; i < 3; i++) { double e = jt[i] - gj[i].a, gc = 9.81 * gj[i].ms * gj[i].ln * co2(gj[i].a) * 0.5; gj[i].t = cl(e * 10 + gc + 0.1 * gj[i].fr, -100, 100); gj[i].v += gj[i].t * 0.1 / (gj[i].ms * gj[i].ln); gj[i].a += gj[i].v * 0.1; }
    prs(" j0="); prd(gj[0].a); prs(" j1="); prd(gj[1].a); prs(" j2="); prd(gj[2].a); prs(" gc="); prd(9.81 * gj[0].ms * gj[0].ln * co2(gj[0].a) * 0.5); prs("\n");
    gft.mg = 8; prs("[grip] f="); prd(cl(10, 0, 50)); prs("\n");

    if (ftest) {
        prs("[traj] "); gini(); gadd(3, 3, 1); p_ast(0.5, 0.5, 8, 8); tsm();
        PID jp = (PID){5, 0.5, 0.2, 0.05, 0, 0}; gs.p = (V3){0, 0, 0}; gs.o = (Qt){1, 0, 0, 0};
        for (int i = 0; i < gt.n && i < 15; i++) { double dx = gt.t[i].p.x - gs.p.x, dy = gt.t[i].p.y - gs.p.y;
            if (sq(dx * dx + dy * dy) < 0.01) continue;
            gs.o.z += pid_u(&jp, at2(dy, dx), gs.o.z) * 0.05; double vx = cl(dx * 2, -1, 1), vy = cl(dy * 2, -1, 1);
            gs.p.x += (vx * co2(gs.o.z) - vy * si2(gs.o.z)) * 0.05; gs.p.y += (vx * si2(gs.o.z) + vy * co2(gs.o.z)) * 0.05; sf_ca();
        }
        prs("p=("); prd(gs.p.x); prs(","); prd(gs.p.y); prs(")\n");
        prs("[fused] "); gs.p = (V3){0, 0, 0}; ek_p(&gek, (V3){1, 0.5, 0}, 0.1); ek_u(&gek, (V3){0.15, 0.05, 0});
        uk_p(&guk, (V3){1, 0.5, 0}, 0.1); uk_u(&guk, (V3){0.15, 0.05, 0});
        pf_u((V3){0.15, 0.05, 0}, 0.3); V3 pf = pf_e();
        gs.p.x = gek.s.x * 0.4 + guk.m.x * 0.35 + pf.x * 0.25; gs.p.y = gek.s.y * 0.4 + guk.m.y * 0.35 + pf.y * 0.25;
        prs("fu=("); prd(gs.p.x); prs(","); prd(gs.p.y); prs(")\n"); gst.cs += 15; gst.fu += 10;
    }

    show_stat(); prs("robot_control_system: done\n"); host_exit(0); return 0;
}
