/* event_vision: Event-based vision processing for DVS cameras (v1.0)
 *
 * Subsystems:
 *   [1] DVS event stream: filter, denoise, interpolate, align
 *   [2] Object detection and high-speed tracking on events
 *   [3] Event optical flow and velocity estimation
 *   [4] Event SLAM: feature extraction, mapping, relocalization
 *   [5] Stereo vision: correspondence, depth, 3D reconstruction
 *   [6] Applications: driving, robotics, drone, gesture, behavior
 */
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Host imports (provided by the WASM runtime)                        */
/* ------------------------------------------------------------------ */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ------------------------------------------------------------------ */
/* Tuning constants                                                    */
/* ------------------------------------------------------------------ */
#define EV_MAX     1024   /* max events in ring buffer */
#define EV_MT      32     /* max simultaneous tracks */
#define EV_MM      128    /* max SLAM map points */
#define EV_MK      16     /* max SLAM keyframes */
#define GW         28     /* flow/surface grid width */
#define GH         24     /* flow/surface grid height */
#define EV_CELLS   (GW * GH)
#define EV_APPS    5      /* number of demo applications */
#define EV_SENSOR_W 1024  /* simulated sensor width */
#define EV_SENSOR_H 1024  /* simulated sensor height */

/* Polarity labels */
#define EV_POL_POS 1
#define EV_POL_NEG 0

/* Track states */
#define TRK_LOST  0
#define TRK_TENT  1
#define TRK_ACT   2
#define TRK_OCC   3

/* Object classes */
#define CLS_UNK   0
#define CLS_PED   1   /* pedestrian */
#define CLS_VEH   2   /* vehicle */
#define CLS_DRN   3   /* drone */
#define CLS_HAND  4   /* hand / gesture */
#define CLS_OBS   5   /* generic obstacle */

/* ------------------------------------------------------------------ */
/* Core data types                                                     */
/* ------------------------------------------------------------------ */
typedef struct { unsigned int x, y, t; int pol; }          ev_t;   /* DVS event */
typedef struct { ev_t *b; unsigned int cap, hd, n, ti, to; } st_t; /* stream */
typedef struct { int x0, y0, x1, y1, ec, cl, tr; }         bb_t;   /* bbox */
typedef struct { int id, cl, st, cx, cy, vx, vy, ag;              /* track */
                 int hi, ms, bi; }                           tk_t;
typedef struct { int fx, fy, cf; }                           fl_t;   /* flow */
typedef struct { int x, y, z, pl; unsigned int t; int ds; }  mp_t;   /* map pt */
typedef struct { int cx, cy, cz, yw, pt, rl, pc; unsigned int kt; } ps_t; /* pose */
typedef struct { int dp, dh, cf; }                           sm_t;   /* stereo */
typedef struct { int id; char nm[16]; int ec, oc, fs, st; }  ap_t;   /* app */

/* ------------------------------------------------------------------ */
/* Module globals                                                      */
/* ------------------------------------------------------------------ */
static ev_t g_pool[EV_MAX];
static tk_t g_tk[EV_MT];
static mp_t g_mp[EV_MM];
static ps_t g_ps[EV_MK];
static fl_t g_fl[EV_CELLS];
static ap_t g_ap[EV_APPS];

static int g_tn = 0;   /* active track count */
static int g_ni = 1;   /* next track id */
static int g_mn = 0;   /* map point count */
static int g_pn = 0;   /* pose keyframe count */
static int g_i  = 0;   /* init flag */

/* ------------------------------------------------------------------ */
/* Utility helpers                                                     */
/* ------------------------------------------------------------------ */
static void *al(unsigned int z) { return (void *)host_alloc(z, 8); }
static void pr(const char *s)   { if (s) host_print(s); }
static int  ab(int v)           { return v < 0 ? -v : v; }
static int  cl(int v, int a, int b) { return v < a ? a : (v > b ? b : v); }

/* Print integer (signed) */
static void pi(int v) {
    char b[20]; int i = 0, g = 0; unsigned int u;
    if (v < 0) { g = 1; u = (unsigned int)(-(v + 1)) + 1; } else u = (unsigned int)v;
    if (u == 0) b[i++] = '0';
    while (u > 0) { b[i++] = '0' + (int)(u % 10); u /= 10; }
    if (g) b[i++] = '-'; b[i] = '\0';
    int l = 0, r = i - 1;
    while (l < r) { char c = b[l]; b[l] = b[r]; b[r] = c; l++; r--; }
    pr(b);
}

/* Print hex (uppercase) */
static void ph(unsigned int v) {
    char b[12]; int i = 0;
    static const char H[] = "0123456789ABCDEF";
    b[i++] = '0'; b[i++] = 'x'; int s = 0;
    for (int k = 28; k >= 0; k -= 4) {
        int n = (v >> k) & 0xF;
        if (n || s || k == 0) { b[i++] = H[n]; s = 1; }
    }
    b[i] = '\0'; pr(b);
}

/* Integer square root */
static unsigned int sq(unsigned int n) {
    if (!n) return 0;
    unsigned int x = n, y = (x + 1) >> 1;
    while (y < x) { x = y; y = (x + n / x) >> 1; }
    return x;
}

/* ------------------------------------------------------------------ */
/* [1] Event stream operations                                         */
/* ------------------------------------------------------------------ */

/* Initialize ring buffer */
static void si(st_t *s, ev_t *p, unsigned int c) {
    s->b = p; s->cap = c; s->hd = 0; s->n = 0; s->ti = 0; s->to = 0;
}

/* Push one DVS event (x, y, t, polarity) */
static void sp(st_t *s, unsigned int x, unsigned int y, unsigned int t, int p) {
    if (!s->b || !s->cap) return;
    unsigned int i = (s->hd + s->n) % s->cap;
    s->b[i].x = x; s->b[i].y = y; s->b[i].t = t;
    s->b[i].pol = p ? EV_POL_POS : EV_POL_NEG;
    if (s->n < s->cap) s->n++; else s->hd = (s->hd + 1) % s->cap;
    s->ti++;
}

/* Time-window filter: keep events in [a, b] */
static int sft(st_t *s, unsigned int a, unsigned int b) {
    if (!s->b) return 0; int k = 0;
    for (unsigned int i = 0; i < s->n; i++)
        if (s->b[i].t >= a && s->b[i].t <= b) {
            if (k != (int)i) s->b[k] = s->b[i]; k++;
        }
    s->to += s->n - (unsigned)k; s->n = (unsigned)k; return k;
}

/* Polarity filter: keep only given polarity */
static int sfp(st_t *s, int p) {
    if (!s->b) return 0; int k = 0;
    for (unsigned int i = 0; i < s->n; i++)
        if (s->b[i].pol == p) { if (k != (int)i) s->b[k] = s->b[i]; k++; }
    s->n = (unsigned)k; return k;
}

/* Spatial region filter: keep events within rectangle */
static int sfr(st_t *s, int x0, int y0, int x1, int y1) {
    if (!s->b) return 0; int k = 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int x = (int)s->b[i].x, y = (int)s->b[i].y;
        if (x >= x0 && x <= x1 && y >= y0 && y <= y1) {
            if (k != (int)i) s->b[k] = s->b[i]; k++;
        }
    }
    s->n = (unsigned)k; return k;
}

/* Spatial median denoiser: drop isolated events (fewer than 2 neighbors
 * within radius rd). */
static int sdn(st_t *s, int r) {
    if (!s->b || !s->n) return 0;
    int rd = r > 0 ? r : 1, k = 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int cx = (int)s->b[i].x, cy = (int)s->b[i].y, nb = 0;
        for (unsigned int j = 0; j < s->n; j++) {
            if (i == j) continue;
            if (ab((int)s->b[j].x - cx) <= rd &&
                ab((int)s->b[j].y - cy) <= rd) nb++;
        }
        if (nb >= 2) { if (k != (int)i) s->b[k] = s->b[i]; k++; }
    }
    s->n = (unsigned)k; return k;
}

/* Temporal correlation denoiser: require a neighbor within spatial and
 * temporal windows. */
static int sdt(st_t *s, unsigned int w) {
    if (!s->b || !s->n) return 0;
    unsigned int wn = w > 0 ? w : 100; int k = 0;
    for (unsigned int i = 0; i < s->n; i++) {
        unsigned int t0 = s->b[i].t;
        int cx = (int)s->b[i].x, cy = (int)s->b[i].y, c = 0;
        for (unsigned int j = 0; j < s->n; j++) {
            if (i == j) continue;
            unsigned int dt = s->b[j].t > t0 ? s->b[j].t - t0 : t0 - s->b[j].t;
            if (dt > wn) continue;
            if (ab((int)s->b[j].x - cx) <= 2 &&
                ab((int)s->b[j].y - cy) <= 2) { c = 1; break; }
        }
        if (c) { if (k != (int)i) s->b[k] = s->b[i]; k++; }
    }
    s->n = (unsigned)k; return k;
}

/* Statistical outlier removal: drop events whose mean squared distance
 * to nearest neighbors exceeds threshold. */
static int sdo(st_t *s, int K) {
    if (!s->b || !s->n) return 0;
    int kv = K > 0 ? K : 3, k = 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int cx = (int)s->b[i].x, cy = (int)s->b[i].y;
        unsigned int ds = 0; int h = 0;
        for (unsigned int j = 0; j < s->n && h < kv; j++) {
            if (i == j) continue;
            int dx = (int)s->b[j].x - cx, dy = (int)s->b[j].y - cy;
            ds += (unsigned)(dx * dx + dy * dy); h++;
        }
        if (h > 0 && ds / (unsigned)h < 256U) {
            if (k != (int)i) s->b[k] = s->b[i]; k++;
        }
    }
    s->n = (unsigned)k; return k;
}

/* Build time-surface: latest timestamp per grid cell. Returns number of
 * cells that received an event. */
static int sts(st_t *s, unsigned int *f) {
    if (!f) return 0;
    for (int i = 0; i < EV_CELLS; i++) f[i] = 0;
    int u = 0;
    if (!s->b) return 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int x = cl((int)s->b[i].x, 0, GW - 1);
        int y = cl((int)s->b[i].y, 0, GH - 1);
        unsigned int k = (unsigned)(y * GW + x);
        if (!f[k] || s->b[i].t > f[k]) { f[k] = s->b[i].t; u++; }
    }
    return u;
}

/* Exponential-decay interpolation of time-surface: f[k] *= tau / (tau+dt) */
static int sdc(st_t *s, unsigned int tau, unsigned int *f) {
    if (!f || !tau) return 0;
    sts(s, f);
    unsigned int mx = 0;
    for (int i = 0; i < EV_CELLS; i++) if (f[i] > mx) mx = f[i];
    int d = 0;
    for (int i = 0; i < EV_CELLS; i++) {
        if (!f[i]) continue;
        unsigned int v = (f[i] * tau) / (tau + mx - f[i]);
        f[i] = v; if (v) d++;
    }
    return d;
}

/* Motion-compensated alignment: shift events to a common reference time. */
static int sal(st_t *s, int vx, int vy, unsigned int dt) {
    if (!s->b || !s->n) return 0;
    for (unsigned int i = 0; i < s->n; i++) {
        unsigned int e = s->b[i].t > dt ? s->b[i].t - dt : 0;
        int nx = (int)s->b[i].x + (vx * (int)e) / 1000;
        int ny = (int)s->b[i].y + (vy * (int)e) / 1000;
        s->b[i].x = (unsigned)cl(nx, 0, EV_SENSOR_W - 1);
        s->b[i].y = (unsigned)cl(ny, 0, EV_SENSOR_H - 1);
        s->b[i].t = dt;
    }
    return (int)s->n;
}

/* Warp events by a constant offset (reference frame alignment). */
static int swp(st_t *s, int dx, int dy) {
    if (!s->b) return 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int nx = (int)s->b[i].x + dx, ny = (int)s->b[i].y + dy;
        s->b[i].x = (unsigned)cl(nx, 0, EV_SENSOR_W - 1);
        s->b[i].y = (unsigned)cl(ny, 0, EV_SENSOR_H - 1);
    }
    return (int)s->n;
}

/* ------------------------------------------------------------------ */
/* [2] Object detection: connected-component clustering on a coarse    */
/*     grid. Returns the number of bounding boxes produced.            */
/* ------------------------------------------------------------------ */
static void bbr(bb_t *b) {
    b->x0 = 0x7FFFFFFF; b->y0 = 0x7FFFFFFF;
    b->x1 = -1; b->y1 = -1; b->ec = 0; b->cl = 0; b->tr = -1;
}
static void bbx(bb_t *b, int x, int y) {
    if (x < b->x0) b->x0 = x; if (y < b->y0) b->y0 = y;
    if (x > b->x1) b->x1 = x; if (y > b->y1) b->y1 = y;
    b->ec++;
}

static int dcl(st_t *s, bb_t *o, int m) {
    if (!s->b || !o || m <= 0) return 0;
    int cw = 16, ch = 16;
    int gw = EV_SENSOR_W / cw, gh = EV_SENSOR_H / ch;
    int tot = gw * gh;
    int *ci = (int *)al((unsigned)(tot * (int)sizeof(int)));
    if (!ci) return 0;
    for (int i = 0; i < tot; i++) ci[i] = -1;
    for (unsigned int i = 0; i < s->n; i++) {
        int x = (int)s->b[i].x / cw, y = (int)s->b[i].y / ch;
        if (x >= gw) x = gw - 1; if (y >= gh) y = gh - 1;
        ci[y * gw + x] = 0;
    }
    int nc = 0;
    for (int y = 0; y < gh; y++) {
        for (int x = 0; x < gw; x++) {
            int k = y * gw + x;
            if (ci[k] != 0) continue;
            nc++;
            int q[64], qh = 0, qt = 0;
            q[qt++] = k; ci[k] = nc;
            while (qh < qt) {
                int c = q[qh++], cx = c % gw, cy = c / gw;
                int nb[4], nn = 0;
                if (cx > 0)      nb[nn++] = c - 1;
                if (cx < gw - 1) nb[nn++] = c + 1;
                if (cy > 0)      nb[nn++] = c - gw;
                if (cy < gh - 1) nb[nn++] = c + gw;
                for (int j = 0; j < nn; j++)
                    if (ci[nb[j]] == 0) {
                        ci[nb[j]] = nc;
                        if (qt < 64) q[qt++] = nb[j];
                    }
            }
            if (nc >= m) goto done;
        }
    }
done:;
    int no = nc < m ? nc : m;
    for (int i = 0; i < no; i++) bbr(&o[i]);
    for (unsigned int i = 0; i < s->n; i++) {
        int x = (int)s->b[i].x / cw, y = (int)s->b[i].y / ch;
        if (x >= gw) x = gw - 1; if (y >= gh) y = gh - 1;
        int c = ci[y * gw + x];
        if (c <= 0 || c > no) continue;
        bbx(&o[c - 1], (int)s->b[i].x, (int)s->b[i].y);
    }
    int k = 0;
    for (int i = 0; i < no; i++)
        if (o[i].ec >= 4) { if (k != i) o[k] = o[i]; k++; }
    return k;
}

/* ------------------------------------------------------------------ */
/* [2] Tracking: mean-shift, particle filter, Hungarian, occlusion,    */
/*     Kalman prediction.                                              */
/* ------------------------------------------------------------------ */

/* Mean-shift: associate each new box to the nearest existing track, or
 * spawn a new tentative track. */
static int tms(bb_t *b, int n, tk_t *t, int m) {
    int u = 0, lm = n < m ? n : m;
    for (int i = 0; i < lm; i++) {
        int cx = (b[i].x0 + b[i].x1) / 2;
        int cy = (b[i].y0 + b[i].y1) / 2;
        int a = -1, bd = 0x7FFFFFFF;
        for (int j = 0; j < g_tn; j++) {
            if (t[j].st == TRK_LOST) continue;
            int d = ab(t[j].cx - cx) + ab(t[j].cy - cy);
            if (d < bd) { bd = d; a = j; }
        }
        if (a >= 0 && bd < 64) {
            t[a].vx = cx - t[a].cx; t[a].vy = cy - t[a].cy;
            t[a].cx = cx; t[a].cy = cy;
            t[a].hi++; t[a].ms = 0; t[a].bi = i;
            b[i].tr = t[a].id; b[i].cl = t[a].cl; u++;
        } else if (g_tn < m) {
            tk_t *nt = &t[g_tn];
            nt->id = g_ni++; nt->cl = CLS_UNK; nt->st = TRK_TENT;
            nt->cx = cx; nt->cy = cy; nt->vx = 0; nt->vy = 0;
            nt->ag = 0; nt->hi = 1; nt->ms = 0; nt->bi = i;
            b[i].tr = nt->id; g_tn++; u++;
        }
    }
    return u;
}

/* Particle filter: sample predicted position, check event support. */
static int tpf(tk_t *t, int n, st_t *s) {
    if (!t || !s->b || !s->n) return 0; int r = 0;
    for (int i = 0; i < n; i++) {
        if (t[i].st == TRK_LOST) continue;
        int px = t[i].cx + t[i].vx, py = t[i].cy + t[i].vy, su = 0;
        for (unsigned int j = 0; j < s->n; j++)
            if (ab((int)s->b[j].x - px) < 8 &&
                ab((int)s->b[j].y - py) < 8) su++;
        if (su > 0) { t[i].cx = px; t[i].cy = py; r++; }
        else { t[i].ms++; if (t[i].ms > 5) t[i].st = TRK_LOST; }
    }
    return r;
}

/* Hungarian-style greedy assignment of tracks to new boxes. */
static int thg(tk_t *t, int nt, bb_t *b, int nb) {
    int a = 0;
    for (int i = 0; i < nt; i++) {
        if (t[i].st == TRK_LOST) continue;
        int bt = -1, bc = 0x7FFFFFFF;
        for (int j = 0; j < nb; j++) {
            if (b[j].tr >= 0) continue;
            int cx = (b[j].x0 + b[j].x1) / 2;
            int cy = (b[j].y0 + b[j].y1) / 2;
            int c = ab(t[i].cx - cx) + ab(t[i].cy - cy);
            if (c < bc) { bc = c; bt = j; }
        }
        if (bt >= 0 && bc < 80) {
            b[bt].tr = t[i].id; b[bt].cl = t[i].cl;
            t[i].bi = bt; a++;
        }
    }
    return a;
}

/* Occlusion handler: coast missing tracks with constant velocity. */
static int toc(tk_t *t, int n) {
    int p = 0;
    for (int i = 0; i < n; i++) {
        if (t[i].st != TRK_ACT) continue;
        if (t[i].bi < 0) {
            t[i].st = TRK_OCC;
            t[i].cx += t[i].vx; t[i].cy += t[i].vy;
            t[i].ms++; p++;
            if (t[i].ms > 10) t[i].st = TRK_LOST;
        }
    }
    return p;
}

/* Kalman prediction: damped linear extrapolation. */
static int tkl(tk_t *t, int steps) {
    if (!t || t->st == TRK_LOST) return 0;
    (void)steps;
    t->vx = (t->vx * 7) / 8;
    t->vy = (t->vy * 7) / 8;
    return 1;
}

/* ------------------------------------------------------------------ */
/* [3] Optical flow estimation                                         */
/* ------------------------------------------------------------------ */

/* Per-cell mean displacement from consecutive events. */
static int fle(st_t *s, fl_t *g, int gw, int gh) {
    if (!s->b || !g || gw <= 0 || gh <= 0) return 0;
    int c = gw * gh;
    int *sx = (int *)al((unsigned)(c * 2 * (int)sizeof(int)));
    if (!sx) return 0;
    int *sy = sx + c;
    int *cn = (int *)al((unsigned)(c * (int)sizeof(int)));
    if (!cn) return 0;
    for (int i = 0; i < c; i++) { sx[i] = 0; sy[i] = 0; cn[i] = 0; }
    unsigned int pt[8]; int px[8][2];
    for (int i = 0; i < 8; i++) { pt[i] = 0; px[i][0] = 0; px[i][1] = 0; }
    int sl = 0;
    for (unsigned int i = 0; i < s->n; i++) {
        int x = ((int)s->b[i].x * gw) / EV_SENSOR_W;
        int y = ((int)s->b[i].y * gh) / EV_SENSOR_H;
        x = cl(x, 0, gw - 1); y = cl(y, 0, gh - 1);
        int k = y * gw + x, h = sl & 7;
        if (pt[h]) {
            sx[k] += ((int)s->b[i].x - px[h][0]);
            sy[k] += ((int)s->b[i].y - px[h][1]);
            cn[k]++;
        }
        pt[h] = s->b[i].t;
        px[h][0] = (int)s->b[i].x; px[h][1] = (int)s->b[i].y;
        sl++;
    }
    int f = 0;
    for (int i = 0; i < c; i++) {
        if (cn[i] > 0) {
            g[i].fx = sx[i] / cn[i];
            g[i].fy = sy[i] / cn[i];
            g[i].cf = cn[i] < 127 ? cn[i] : 127; f++;
        } else { g[i].fx = 0; g[i].fy = 0; g[i].cf = 0; }
    }
    return f;
}

/* Motion segmentation: quantize flow direction into 8 sectors. */
static int fls(fl_t *g, int gw, int gh, int *l) {
    if (!g || !l) return 0;
    int c = gw * gh;
    for (int i = 0; i < c; i++) {
        if (g[i].cf < 5) { l[i] = -1; continue; }
        int x = g[i].fx, y = g[i].fy, s = 0;
        if (x >= 0 && y >= 0)      s = x > y ? 0 : 1;
        else if (x < 0 && y >= 0)  s = -x > y ? 3 : 2;
        else if (x < 0 && y < 0)   s = -x > -y ? 4 : 5;
        else                       s = x > -y ? 7 : 6;
        l[i] = s;
    }
    int p[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < c; i++) if (l[i] >= 0 && l[i] < 8) p[l[i]] = 1;
    int n = 0; for (int i = 0; i < 8; i++) n += p[i];
    return n;
}

/* Scene flow: back-project flow magnitudes into a pseudo-depth map using
 * a pinhole camera model (Z = focal * baseline / disparity). */
static int fl3(fl_t *g, int gw, int gh, int f, int *d) {
    if (!g || !d || f <= 0) return 0;
    int c = gw * gh, p = 0;
    for (int i = 0; i < c; i++) {
        if (g[i].cf < 5) { d[i] = 0; continue; }
        unsigned int m = sq((unsigned)(g[i].fx * g[i].fx + g[i].fy * g[i].fy));
        if (!m) { d[i] = 0; continue; }
        d[i] = (f * 100) / (int)m; p++;
    }
    return p;
}

/* ------------------------------------------------------------------ */
/* [4] Event SLAM                                                      */
/* ------------------------------------------------------------------ */

/* FAST-like corner extraction on the event time-surface. */
static int slf(st_t *s, mp_t *m, int x) {
    if (!s->b || !m || x <= 0) return 0;
    unsigned int *f = (unsigned *)al((unsigned)(EV_CELLS * (int)sizeof(unsigned)));
    if (!f) return 0;
    sts(s, f);
    int n = 0, thr = 3;
    for (int y = thr; y < GH - thr && n < x; y++) {
        for (int k = thr; k < GW - thr && n < x; k++) {
            unsigned int c = f[y * GW + k]; if (!c) continue;
            int br = 0, dk = 0;
            for (int i = 0; i < 8; i++) {
                int dx = i < 4 ? (i == 0 ? thr : (i == 2 ? 0 : 2))
                               : -((i == 4 ? thr : (i == 6 ? 0 : 2)));
                int dy = i < 4 ? (i == 0 ? 0 : (i == 2 ? thr : 2))
                               : -((i == 4 ? 0 : (i == 6 ? thr : 2)));
                int nx = k + dx, ny = y + dy;
                if (nx < 0 || ny < 0 || nx >= GW || ny >= GH) continue;
                unsigned int v = f[ny * GW + nx];
                if (v > c + 2) br++; if (v + 2 < c) dk++;
            }
            if (br >= 5 || dk >= 5) {
                m[n].x = k; m[n].y = y; m[n].z = 0;
                m[n].pl = EV_POL_POS; m[n].t = c;
                m[n].ds = (int)(c & 0xFFFFU); n++;
            }
        }
    }
    return n;
}

/* Add features to the global map, translated by the current pose. */
static int slb(mp_t *m, int n, ps_t *p) {
    if (!m || !p || n <= 0) return 0;
    for (int i = 0; i < n && g_mn < EV_MM; i++) {
        g_mp[g_mn] = m[i];
        g_mp[g_mn].x += p->cx / 10;
        g_mp[g_mn].y += p->cy / 10;
        g_mp[g_mn].z += p->cz / 10;
        g_mn++;
    }
    return n;
}

/* Place recognition: compare descriptor histogram to stored keyframes. */
static int slr(mp_t *m, int n, ps_t *q) {
    if (!m || !q || n <= 0 || !g_pn) return 0;
    int hq[16], hk[16];
    for (int i = 0; i < 16; i++) { hq[i] = 0; hk[i] = 0; }
    for (int i = 0; i < n; i++) hq[m[i].ds & 0xF]++;
    int bp = -1, bs = 0;
    for (int p = 0; p < g_pn; p++) {
        for (int i = 0; i < 16; i++) hk[i] = (g_ps[p].pc >> i) & 1;
        int sc = 0;
        for (int i = 0; i < 16; i++) if (hq[i] && hk[i]) sc++;
        if (sc > bs) { bs = sc; bp = p; }
    }
    if (bp >= 0) { q->cx = g_ps[bp].cx; q->cy = g_ps[bp].cy; q->cz = g_ps[bp].cz; }
    return bp + 1;
}

/* IMU pre-integration: double-integrate acceleration to update pose. */
static int sli(ps_t *p, int ax, int ay, int az, unsigned int dt) {
    if (!p) return 0;
    int dx = (ax * (int)dt) / 1000;
    int dy = (ay * (int)dt) / 1000;
    int dz = (az * (int)dt) / 1000;
    p->cx += (dx * (int)dt) / 2000;
    p->cy += (dy * (int)dt) / 2000;
    p->cz += (dz * (int)dt) / 2000;
    p->yw += (ax / 10) * (int)dt / 1000;
    p->pt += (ay / 10) * (int)dt / 1000;
    return 1;
}

/* Loop closure: count near-revisit pose pairs. */
static int sll(ps_t *p, int n) {
    if (!p || n < 2) return 0; int c = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 2; j < n; j++) {
            int dx = p[i].cx - p[j].cx;
            int dy = p[i].cy - p[j].cy;
            int dz = p[i].cz - p[j].cz;
            if ((unsigned)(dx * dx + dy * dy + dz * dz) < 2500U) c++;
        }
    }
    return c;
}

/* ------------------------------------------------------------------ */
/* [5] Stereo vision                                                   */
/* ------------------------------------------------------------------ */

/* Time-surface stereo matching: search left-right along epipolar line. */
static int stm(st_t *l, st_t *r, sm_t *o, int md) {
    if (!l->b || !r->b || !o || md <= 0) return 0;
    unsigned int *a = (unsigned *)al((unsigned)(EV_CELLS * 2 *
                                               (int)sizeof(unsigned)));
    if (!a) return 0;
    unsigned int *b = a + EV_CELLS;
    for (int i = 0; i < EV_CELLS; i++) { a[i] = 0; b[i] = 0; }
    for (unsigned int i = 0; i < l->n; i++) {
        int x = cl(((int)l->b[i].x * GW) / EV_SENSOR_W, 0, GW - 1);
        int y = cl(((int)l->b[i].y * GH) / EV_SENSOR_H, 0, GH - 1);
        a[y * GW + x] = l->b[i].t;
    }
    for (unsigned int i = 0; i < r->n; i++) {
        int x = cl(((int)r->b[i].x * GW) / EV_SENSOR_W, 0, GW - 1);
        int y = cl(((int)r->b[i].y * GH) / EV_SENSOR_H, 0, GH - 1);
        b[y * GW + x] = r->b[i].t;
    }
    int m = 0;
    for (int y = 0; y < GH && m < EV_CELLS; y++) {
        for (int x = 0; x < GW && m < EV_CELLS; x++) {
            int k = y * GW + x; if (!a[k]) continue;
            int bd = -1, be = 0x7FFFFFFF;
            for (int i = 0; i <= md && i <= x; i++) {
                unsigned int v = b[y * GW + (x - i)]; if (!v) continue;
                int e = ab((int)a[k] - (int)v);
                if (e < be) { be = e; bd = i; }
            }
            if (bd >= 0 && be < 500) {
                o[m].dp = bd; o[m].dh = 0; o[m].cf = 100 - be; m++;
            }
        }
    }
    return m;
}

/* Convert disparity to depth (Z = f * B / d). */
static int sdp(sm_t *m, int n, int f, int b) {
    if (!m || f <= 0 || b <= 0) return 0; int v = 0;
    for (int i = 0; i < n; i++)
        if (m[i].dp > 0) { m[i].dh = (f * b) / m[i].dp; v++; }
    return v;
}

/* Reconstruct 3D points from stereo matches. */
static int src(sm_t *m, int n, int f, int b, mp_t *o, int x) {
    if (!m || !o || f <= 0 || b <= 0) return 0; int p = 0;
    for (int i = 0; i < n && p < x; i++) {
        if (m[i].dh <= 0) continue;
        o[p].x = (i % GW) * 20; o[p].y = (i / GW) * 20;
        o[p].z = m[i].dh; o[p].pl = EV_POL_POS;
        o[p].t = 0; o[p].ds = m[i].dp; p++;
    }
    return p;
}

/* Semantic labeling of bounding boxes from class histogram. */
static int ssm(bb_t *b, int n, int *h, int c) {
    if (!b || !h || c <= 0) return 0;
    for (int i = 0; i < c; i++) h[i] = 0;
    for (int i = 0; i < n; i++) { int v = b[i].cl; if (v >= 0 && v < c) h[v]++; }
    int t = 0; for (int i = 0; i < c; i++) t += h[i];
    return t;
}

/* ------------------------------------------------------------------ */
/* [6] Application modules                                             */
/* ------------------------------------------------------------------ */

/* Autonomous driving: score pedestrian/vehicle counts and collision risk */
static int apd(bb_t *b, int n, fl_t *f, int nf) {
    int p = 0, v = 0, a = 0;
    for (int i = 0; i < n; i++) {
        if (b[i].cl == CLS_PED) p++;
        if (b[i].cl == CLS_VEH) v++;
    }
    int s = 0, c = 0;
    for (int i = 0; i < nf; i++)
        if (f[i].cf > 10) { s += ab(f[i].fx); c++; }
    if (c > 0 && s / c > 8) a++;
    return p * 10 + v * 5 + a * 20;
}

/* Robot manipulation: find largest object, check if grasp-ready */
static int apr(bb_t *b, int n, fl_t *f, int nf) {
    int t = -1, ba = 0; (void)f; (void)nf;
    for (int i = 0; i < n; i++) {
        int w = b[i].x1 - b[i].x0, h = b[i].y1 - b[i].y0, a = w * h;
        if (a > ba) { ba = a; t = i; }
    }
    if (t >= 0) {
        int cx = (b[t].x0 + b[t].x1) / 2;
        return ab(cx - EV_SENSOR_W / 2) < 80 ? 2 : 1;
    }
    return 0;
}

/* Drone navigation: obstacle count per quadrant */
static int apn(fl_t *f, int nf, int *o) {
    if (!f || !o) return 0;
    int r[4] = {0, 0, 0, 0};
    for (int i = 0; i < nf; i++) {
        if (f[i].cf < 5) continue;
        int m = ab(f[i].fx) + ab(f[i].fy);
        if (m < 3) continue;
        int x = i % GW, y = i / GW;
        if (y < GH / 2) r[0] += m; else r[2] += m;
        if (x >= GW / 2) r[1] += m; else r[3] += m;
    }
    int c = 0;
    for (int i = 0; i < 4; i++) { o[i] = r[i]; if (r[i] > 50) c++; }
    return c;
}

/* Gesture recognition: classify hand motion by speed */
static int apg(tk_t *t, int n) {
    int h = -1;
    for (int i = 0; i < n; i++) if (t[i].cl == CLS_HAND) { h = t[i].id; break; }
    if (h < 0) return 0;
    for (int i = 0; i < n; i++) {
        if (t[i].id != h) continue;
        int s = ab(t[i].vx) + ab(t[i].vy);
        if (s > 20) return 3; /* swipe */
        if (s > 8)  return 2; /* wave */
        if (s > 2)  return 1; /* move */
        return 1;             /* stationary */
    }
    return 0;
}

/* Behavior analysis: aggregate activity level across all tracks */
static int apb(tk_t *t, int n, int *a) {
    if (!a) return 0;
    int ac = 0, mv = 0, st = 0;
    for (int i = 0; i < n; i++) {
        if (t[i].st != TRK_ACT) continue;
        ac++;
        if (ab(t[i].vx) + ab(t[i].vy) > 5) mv++; else st++;
    }
    if (!ac) { *a = 0; return 0; }      /* idle */
    if (mv > st) { *a = 1; return ac; } /* locomotion */
    *a = 2; return ac;                  /* stationary group */
}

/* ------------------------------------------------------------------ */
/* Demo pipeline                                                       */
/* ------------------------------------------------------------------ */
static void demo(void) {
    pr("========================================\n");
    pr(" event_vision v1.0\n");
    pr(" DVS event-based vision processing\n");
    pr("========================================\n");

    st_t s; si(&s, g_pool, EV_MAX);

    /* --- 1. Generate synthetic DVS stream ----------------------------- */
    pr("[1/7] Generating DVS stream...\n");
    unsigned int t = 1000;
    for (int f = 0; f < 12; f++) {
        int cx = 200 + f * 40, cy = 150 + (f % 3) * 20;
        for (int k = 0; k < 40; k++) {
            int dx = (k * 37) % 64 - 32;
            int dy = (k * 53) % 64 - 32;
            sp(&s, (unsigned)cl(cx + dx, 0, EV_SENSOR_W - 1),
                    (unsigned)cl(cy + dy, 0, EV_SENSOR_H - 1), t, (k + f) & 1);
            t += 7;
        }
    }
    for (int i = 0; i < 200; i++)
        sp(&s, (unsigned)((i * 7919) % EV_SENSOR_W),
                (unsigned)((i * 6271) % EV_SENSOR_H),
                1000 + (unsigned)(i * 3), i & 1);
    pr("  cap="); pi((int)s.cap);
    pr(" cnt="); pi((int)s.n);
    pr(" in="); pi((int)s.ti); pr("\n");

    /* --- 2. Filtering and denoising ---------------------------------- */
    pr("[2/7] Filtering and denoising...\n");
    int bf = (int)s.n;
    sft(&s, 1100, 5000);   /* time window */
    sfr(&s, 0, 0, 900, 500); /* spatial region */
    sdn(&s, 2);            /* spatial median */
    sdt(&s, 200);          /* temporal correlation */
    sdo(&s, 3);            /* statistical outlier */
    pr("  removed "); pi(bf - (int)s.n);
    pr(", "); pi((int)s.n); pr(" remain\n");

    /* --- 3. Time-surface interpolation and alignment ----------------- */
    unsigned int *sf = (unsigned *)al((unsigned)(EV_CELLS *
                                                 (int)sizeof(unsigned)));
    int tc = sts(&s, sf);
    int dc = sdc(&s, 500, sf);
    pr("[3/7] Time-surface: "); pi(tc);
    pr(" cells, decay: "); pi(dc); pr(" nz\n");
    int ai = sal(&s, 40, 10, 2000);
    (void)swp(&s, -5, -3);
    pr("  aligned: "); pi(ai); pr("\n");

    /* --- 4. Object detection and tracking ---------------------------- */
    pr("[4/7] Detection and tracking...\n");
    bb_t b[16]; int nb = dcl(&s, b, 16);
    pr("  clusters: "); pi(nb); pr("\n");
    int tu = tms(b, nb, g_tk, EV_MT);
    pr("  tracks updated: "); pi(tu); pr("\n");
    int tr = tpf(g_tk, g_tn, &s);
    pr("  particle refined: "); pi(tr); pr("\n");
    int co = thg(g_tk, g_tn, b, nb);
    pr("  hungarian: "); pi(co); pr("\n");
    int oc = toc(g_tk, g_tn);
    pr("  occlusions: "); pi(oc); pr("\n");
    if (g_tn > 0) tkl(&g_tk[0], 3);
    for (int i = 0; i < g_tn && i < 4; i++) {
        pr("  tk#"); pi(g_tk[i].id);
        pr(" cl="); pi(g_tk[i].cl);
        pr(" st="); pi(g_tk[i].st);
        pr(" ("); pi(g_tk[i].cx); pr(","); pi(g_tk[i].cy);
        pr(") h="); pi(g_tk[i].hi); pr("\n");
    }

    /* --- 5. Optical flow estimation ---------------------------------- */
    pr("[5/7] Optical flow...\n");
    int ff = fle(&s, g_fl, GW, GH);
    pr("  grid: "); pi(ff); pr("/"); pi(EV_CELLS); pr("\n");
    int *ml = (int *)al((unsigned)(EV_CELLS * (int)sizeof(int)));
    int nm = fls(g_fl, GW, GH, ml);
    pr("  motions: "); pi(nm); pr("\n");
    int *sd = (int *)al((unsigned)(EV_CELLS * (int)sizeof(int)));
    int sp2 = fl3(g_fl, GW, GH, 500, sd);
    pr("  scene3d: "); pi(sp2); pr("\n");

    /* --- 6. Event SLAM ---------------------------------------------- */
    pr("[6/7] Event SLAM...\n");
    int nf = slf(&s, g_mp, EV_MM);
    pr("  features: "); pi(nf); pr("\n");
    ps_t cp;
    cp.cx = 100; cp.cy = 50; cp.cz = 0;
    cp.yw = 0; cp.pt = 0; cp.rl = 0;
    cp.pc = nf; cp.kt = 2000;
    int mb = slb(g_mp, nf, &cp);
    pr("  map points: "); pi(mb); pr("\n");
    g_ps[0] = cp; g_pn = 1;
    ps_t p2 = cp; p2.cx += 60; p2.kt = 3000;
    g_ps[1] = p2; g_pn = 2;
    int io = sli(&cp, 10, -5, 980, 100);
    pr("  IMU fusion: "); pi(io); pr("\n");
    int lc = sll(g_ps, g_pn);
    pr("  loop closures: "); pi(lc); pr("\n");

    /* --- 7. Stereo vision ------------------------------------------- */
    st_t rt;
    ev_t *rp = (ev_t *)al((unsigned)(EV_MAX * (int)sizeof(ev_t)));
    si(&rt, rp, EV_MAX);
    for (unsigned int i = 0; i < s.n; i++) {
        int rx = (int)s.b[i].x - 4;
        sp(&rt, (unsigned)cl(rx, 0, EV_SENSOR_W - 1),
                 s.b[i].y, s.b[i].t, s.b[i].pol);
    }
    sm_t so[64];
    int ns = stm(&s, &rt, so, 16);
    pr("[7/7] Stereo: "); pi(ns); pr(" matches\n");
    int nd = sdp(so, ns, 500, 100);
    pr("  depth: "); pi(nd); pr("\n");
    mp_t cl2[64];
    int n3 = src(so, ns, 500, 100, cl2, 64);
    pr("  3D points: "); pi(n3); pr("\n");
    int ch[6];
    int se = ssm(b, nb, ch, 6);
    pr("  semantic: "); pi(se); pr("\n");

    /* --- Application summary ---------------------------------------- */
    pr("Applications:\n");
    const char *an[EV_APPS] = {
        "driving", "robot", "drone", "gesture", "behavior"
    };
    for (int i = 0; i < EV_APPS; i++) {
        int k = 0;
        while (an[i][k] && k < 15) { g_ap[i].nm[k] = an[i][k]; k++; }
        g_ap[i].nm[k] = '\0';
        g_ap[i].id = i;
        g_ap[i].ec = (int)s.n;
    }
    g_ap[0].oc = nb; g_ap[0].fs = ff;
    g_ap[0].st = apd(b, nb, g_fl, EV_CELLS);
    g_ap[1].oc = nb; g_ap[1].fs = ff;
    g_ap[1].st = apr(b, nb, g_fl, EV_CELLS);
    g_ap[2].fs = ff;
    int ob[4] = {0, 0, 0, 0};
    g_ap[2].oc = apn(g_fl, EV_CELLS, ob);
    g_ap[2].st = g_ap[2].oc;
    g_ap[3].oc = g_tn;
    g_ap[3].st = apg(g_tk, g_tn);
    int aid = 0;
    g_ap[4].oc = apb(g_tk, g_tn, &aid);
    g_ap[4].st = aid;
    for (int i = 0; i < EV_APPS; i++) {
        pr("  "); pr(g_ap[i].nm);
        pr(" st="); pi(g_ap[i].st);
        pr(" ev="); pi(g_ap[i].ec);
        pr(" ob="); pi(g_ap[i].oc);
        pr(" fl="); pi(g_ap[i].fs); pr("\n");
    }

    /* --- Final summary ---------------------------------------------- */
    pr("----------------------------------------\n");
    pr(" processing complete\n");
    pr("  events:    "); pi((int)s.ti); pr("\n");
    pr("  tracks:    "); pi(g_tn); pr("\n");
    pr("  map pts:   "); pi(g_mn); pr("\n");
    pr("  keyframes: "); pi(g_pn); pr("\n");
    pr("  stereo:    "); pi(ns); pr("\n");
    pr("  magic:     "); ph(0xEB15A000U); pr("\n");
    pr("----------------------------------------\n");
}

/* ------------------------------------------------------------------ */
/* Module entry point                                                  */
/* ------------------------------------------------------------------ */
int main(void) {
    if (g_i) return 0;
    g_i = 1;
    char ab[64];
    int al2 = host_get_argv(0, 64);
    (void)ab; (void)al2;
    demo();
    host_exit(0);
    return 0;
}
