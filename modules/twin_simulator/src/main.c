/* twin_simulator: Digital Twin Simulation Engine (v55.0)
 * Multi-domain simulation for AI-ASM OS - discrete events,
 * continuous dynamics, multi-physics coupling, Monte Carlo
 * analysis, metaheuristic optimization, and result analytics.
 * Written in pure C with no standard library. */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_EVT 64
#define MAX_RES 16
#define MAX_DIM 16
#define MAX_MC  128
#define MAX_POP 32
#define MAX_BUF 256
#define MAX_RPT 32
#define MAX_PHYS 4
#define VER "Twin Simulator v55.0 - Digital Twin Simulation Engine"

/* ---- Helpers ---- */
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[16]; int i = 0, neg = 0; unsigned int u;
    if (v < 0) { neg = 1; u = (unsigned int)(-v); } else u = (unsigned int)v;
    if (u == 0) b[i++] = '0';
    while (u > 0) { b[i++] = '0' + (char)(u % 10); u /= 10; }
    char o[18]; int k = 0;
    if (neg) o[k++] = '-';
    while (i > 0) o[k++] = b[--i];
    o[k] = '\0'; host_print(o);
}
static void pd(double v) {
    int w = (int)v; double f = v - (double)w;
    if (f < 0.0) f = -f; pi(w); ps(".");
    int d = (int)(f * 10000.0 + 0.5);
    if (d < 1000) ps("0"); if (d < 100) ps("0"); if (d < 10) ps("0"); pi(d);
}
static double abs_d(double x) { return x < 0.0 ? -x : x; }
static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0; double g = x * 0.5;
    for (int i = 0; i < 40; i++) g = (g + x / g) * 0.5; return g;
}
static double my_exp(double x) {
    double s = 1.0, t = 1.0;
    for (int i = 1; i <= 20; i++) { t *= x / (double)i; s += t; } return s;
}
static unsigned int rng_state = 0xDEADBEEFu;
static double randf(void) {
    rng_state = rng_state * 1103515245u + 12345u;
    return (double)(rng_state & 0x7FFFFFFFu) / (double)0x7FFFFFFFu;
}
static double rand_range(double lo, double hi) { return lo + randf() * (hi - lo); }

/* ---- Forward declarations ---- */
typedef struct { double time; int type; int eid; int pri; int active; } Event;
typedef struct { double cap; double alloc; int n_wait; int deadlock; } Resource;
typedef struct { int items[MAX_EVT]; int head; int tail; int count; int cap; } Queue;
typedef struct {
    Event ev[MAX_EVT]; int nev; double t; double tend;
    Resource res[MAX_RES]; int nres; Queue qu[8]; int nq;
    int deadlocks; int processed;
} DES;
typedef struct { double x[MAX_DIM]; double dx[MAX_DIM]; int dim; double dt; double t; double tend; int stable; } ContSim;
typedef double (*ODEFunc)(double *x, int dim, int idx);
typedef struct { double stiff; double damp; double mass; double disp; double vel; double stress; } SF;
typedef struct { double visc; double dens; double vel; double pres; double Re; } FF;
typedef struct { double cond; double cp; double temp; double flux; double src; } TF;
typedef struct { double eps; double mu; double E; double B; double freq; } EM;
typedef struct { int type; double cc[MAX_PHYS]; SF sf; FF ff; TF tf; EM em; } PhysDom;
typedef struct { double samples[MAX_MC]; int n; double mean; double var; double sd; double ci_lo; double ci_hi; double mn; double mx; } MCRes;
typedef struct { double pos[MAX_DIM]; double fit; } Ind;
typedef double (*ObjFunc)(double *x, int dim);
typedef struct { double data[MAX_RPT]; int n; double mean; double median; double sd; double trend; int anomaly; int anom_idx; const char *label; } Report;

/* ============================================================
 * 1. Discrete Event Simulation
 * ============================================================ */
static void des_init(DES *d, double end) {
    d->nev = 0; d->t = 0.0; d->tend = end; d->nres = 0;
    d->nq = 0; d->deadlocks = 0; d->processed = 0;
    for (int i = 0; i < MAX_EVT; i++) d->ev[i].active = 0;
}
static int des_add_res(DES *d, double cap) {
    if (d->nres >= MAX_RES) return -1;
    int i = d->nres++;
    d->res[i].cap = cap; d->res[i].alloc = 0.0;
    d->res[i].n_wait = 0; d->res[i].deadlock = 0; return i;
}
static int des_add_q(DES *d, int cap) {
    if (d->nq >= 8) return -1;
    int i = d->nq++;
    for (int j = 0; j < MAX_EVT; j++) d->qu[i].items[j] = 0;
    d->qu[i].head = 0; d->qu[i].tail = 0;
    d->qu[i].count = 0; d->qu[i].cap = cap; return i;
}
static void des_enq(Queue *q, int item) {
    if (q->count >= q->cap) return;
    q->items[q->tail] = item; q->tail = (q->tail + 1) % MAX_EVT; q->count++;
}
static int des_schedule(DES *d, double t, int type, int eid, int pri) {
    if (d->nev >= MAX_EVT) return -1;
    int i = d->nev++;
    d->ev[i].time = t; d->ev[i].type = type;
    d->ev[i].eid = eid; d->ev[i].pri = pri; d->ev[i].active = 1; return i;
}
static void des_sort(DES *d) {
    for (int i = 0; i < d->nev - 1; i++)
        for (int j = i + 1; j < d->nev; j++) {
            if (!d->ev[j].active) continue;
            if (!d->ev[i].active || d->ev[j].time < d->ev[i].time) {
                Event tmp = d->ev[i]; d->ev[i] = d->ev[j]; d->ev[j] = tmp;
            }
        }
}
static int des_acquire(DES *d, int rid, double amt, int eid) {
    if (rid < 0 || rid >= d->nres) return -1;
    Resource *r = &d->res[rid];
    if (r->alloc + amt <= r->cap) { r->alloc += amt; return 1; }
    r->n_wait++; return 0;
}
static void des_release(DES *d, int rid, double amt) {
    if (rid < 0 || rid >= d->nres) return;
    Resource *r = &d->res[rid];
    r->alloc -= amt; if (r->alloc < 0.0) r->alloc = 0.0;
}
static int des_deadlock(DES *d) {
    int found = 0;
    for (int i = 0; i < d->nres; i++)
        if (d->res[i].n_wait > 0 && d->res[i].alloc >= d->res[i].cap) {
            d->res[i].deadlock = 1; found++;
        }
    d->deadlocks += found; return found;
}
static void des_step(DES *d) {
    des_sort(d);
    for (int i = 0; i < d->nev; i++) {
        if (!d->ev[i].active || d->ev[i].time > d->tend) continue;
        d->t = d->ev[i].time; d->ev[i].active = 0; d->processed++;
        if (d->ev[i].type == 0 && d->nq > 0) des_enq(&d->qu[0], d->ev[i].eid);
        des_deadlock(d);
    }
    d->nev = 0;
}
static void des_run(DES *d) {
    ps("[DES] Discrete event simulation\n");
    des_schedule(d, 0.0, 0, 1, 0); des_schedule(d, 0.5, 0, 2, 1);
    des_schedule(d, 1.0, 1, 1, 0); des_schedule(d, 1.5, 2, 1, 0);
    des_schedule(d, 2.0, 3, 1, 0); des_schedule(d, 2.5, 0, 3, 0);
    des_schedule(d, 3.0, 4, 2, 1);
    double t = 0.0;
    while (t < d->tend && d->nev > 0) {
        des_step(d); t = d->t + 0.1;
        if (d->nev == 0 && t < d->tend) des_schedule(d, t, 5, 0, 0);
    }
    ps("  Events processed: "); pi(d->processed);
    ps("  Deadlocks: "); pi(d->deadlocks);
    ps("  Sim time: "); pd(d->t); ps("\n");
}

/* ============================================================
 * 2. Continuous Simulation (ODE)
 * ============================================================ */
static void cont_init(ContSim *c, int dims, double dt, double tend) {
    c->dim = dims; c->dt = dt; c->t = 0.0; c->tend = tend; c->stable = 1;
    for (int i = 0; i < MAX_DIM; i++) { c->x[i] = 0.0; c->dx[i] = 0.0; }
}
static double odefunc_harmonic(double *x, int dim, int idx) {
    (void)dim;
    if (idx == 0) return x[1]; if (idx == 1) return -x[0] - 0.1 * x[1]; return 0.0;
}
static double odefunc_lorenz(double *x, int dim, int idx) {
    (void)dim; double s = 10.0, r = 28.0, b = 8.0 / 3.0;
    if (idx == 0) return s * (x[1] - x[0]);
    if (idx == 1) return x[0] * (r - x[2]) - x[1];
    if (idx == 2) return x[0] * x[1] - b * x[2]; return 0.0;
}
static void rk4_step(ContSim *c, ODEFunc f) {
    double k1[MAX_DIM], k2[MAX_DIM], k3[MAX_DIM], k4[MAX_DIM], xt[MAX_DIM];
    for (int i = 0; i < c->dim; i++) k1[i] = f(c->x, c->dim, i);
    for (int i = 0; i < c->dim; i++) xt[i] = c->x[i] + 0.5 * c->dt * k1[i];
    for (int i = 0; i < c->dim; i++) k2[i] = f(xt, c->dim, i);
    for (int i = 0; i < c->dim; i++) xt[i] = c->x[i] + 0.5 * c->dt * k2[i];
    for (int i = 0; i < c->dim; i++) k3[i] = f(xt, c->dim, i);
    for (int i = 0; i < c->dim; i++) xt[i] = c->x[i] + c->dt * k3[i];
    for (int i = 0; i < c->dim; i++) k4[i] = f(xt, c->dim, i);
    for (int i = 0; i < c->dim; i++)
        c->x[i] += (c->dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    c->t += c->dt;
}
static int stability_chk(ContSim *c) {
    double norm = 0.0;
    for (int i = 0; i < c->dim; i++) norm += c->x[i] * c->x[i];
    norm = my_sqrt(norm);
    if (norm > 1e6 || norm != norm) { c->stable = 0; return 0; } return 1;
}
static void cont_run(ContSim *c, ODEFunc f, const char *name) {
    ps("[CONT] "); ps(name); ps(" dim="); pi(c->dim); ps(" dt="); pd(c->dt); ps("\n");
    c->x[0] = 1.0; c->x[1] = 0.5; if (c->dim > 2) c->x[2] = 1.0;
    int steps = 0;
    while (c->t < c->tend) {
        rk4_step(c, f); steps++;
        if (!stability_chk(c)) {
            ps("  UNSTABLE step="); pi(steps); ps(" t="); pd(c->t); ps("\n"); break;
        }
    }
    ps("  Steps: "); pi(steps); ps(" State:");
    for (int i = 0; i < c->dim; i++) { ps(" x"); pi(i); ps("="); pd(c->x[i]); }
    ps(" stable="); ps(c->stable ? "yes" : "NO"); ps("\n");
}

/* ============================================================
 * 3. Multi-Physics Coupling
 * ============================================================ */
static void phys_struct(SF *s, double dt) {
    double force = -s->stiff * s->disp - s->damp * s->vel;
    s->vel += (force / s->mass) * dt; s->disp += s->vel * dt;
    s->stress = s->stiff * s->disp;
}
static void phys_fluid(FF *f, double dt) {
    double dp = -f->dens * f->vel * 0.01;
    f->vel += dp * dt / f->visc; f->pres += dp * dt;
    f->Re = f->dens * abs_d(f->vel) / f->visc;
}
static void phys_thermal(TF *t, double dt) {
    double dT = (t->cond * t->flux + t->src) * dt / t->cp;
    t->temp += dT; t->flux = -t->cond * dT / (dt + 1e-12);
}
static void phys_em(EM *e, double dt) {
    double w = 6.2831853 * e->freq;
    e->E += w * e->B * dt / e->eps; e->B -= w * e->E * dt / e->mu;
}
static double get_field_val(PhysDom *p) {
    if (p->type == 0) return p->sf.disp;
    if (p->type == 1) return p->ff.pres;
    if (p->type == 2) return p->tf.temp;
    return p->em.E;
}
static void phys_run(void) {
    ps("[PHYS] Multi-physics coupling\n");
    PhysDom dom[MAX_PHYS];
    for (int i = 0; i < MAX_PHYS; i++)
        for (int j = 0; j < MAX_PHYS; j++) dom[i].cc[j] = 0.0;
    dom[0].type = 0; dom[0].sf = (SF){100.0, 5.0, 2.0, 0.1, 0.0, 0.0}; dom[0].cc[2] = 0.01;
    dom[1].type = 1; dom[1].ff = (FF){0.001, 1000.0, 1.0, 101325.0, 0.0}; dom[1].cc[0] = 0.005;
    dom[2].type = 2; dom[2].tf = (TF){50.0, 500.0, 300.0, 10.0, 100.0}; dom[2].cc[0] = 0.002;
    dom[3].type = 3; dom[3].em = (EM){8.85e-12, 1.26e-6, 100.0, 0.0, 1e6}; dom[3].cc[2] = 0.001;
    double dt = 0.001;
    for (int s = 0; s < 100; s++) {
        for (int i = 0; i < MAX_PHYS; i++) {
            if (dom[i].type == 0) phys_struct(&dom[i].sf, dt);
            else if (dom[i].type == 1) phys_fluid(&dom[i].ff, dt);
            else if (dom[i].type == 2) phys_thermal(&dom[i].tf, dt);
            else phys_em(&dom[i].em, dt);
        }
        for (int i = 0; i < MAX_PHYS; i++)
            for (int j = 0; j < MAX_PHYS; j++)
                if (i != j && dom[i].cc[j] > 0.0) {
                    double infl = get_field_val(&dom[j]);
                    if (dom[i].type == 0) dom[i].sf.stress += dom[i].cc[j] * infl * dt;
                    if (dom[i].type == 2) dom[i].tf.temp += dom[i].cc[j] * infl * dt;
                }
    }
    const char *nm[MAX_PHYS] = {"struct", "fluid", "thermal", "EM"};
    for (int i = 0; i < MAX_PHYS; i++) {
        ps("  "); ps(nm[i]); ps(": "); pd(get_field_val(&dom[i])); ps("\n");
    }
}

/* ============================================================
 * 4. Monte Carlo Analysis
 * ============================================================ */
static double mc_model(double *p, int np) {
    (void)np; return p[0] * p[0] - 3.0 * p[1] + 0.5 * p[2] * p[2] + 1.0;
}
static double arr_mean(double *a, int n) {
    double s = 0.0; for (int i = 0; i < n; i++) s += a[i]; return s / (double)n;
}
static double arr_var(double *a, int n, double m) {
    double s = 0.0;
    for (int i = 0; i < n; i++) { double d = a[i] - m; s += d * d; }
    return s / (double)(n > 1 ? n - 1 : 1);
}
static void sort_arr(double *a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[i]) { double t = a[i]; a[i] = a[j]; a[j] = t; }
}
static void mc_analyze(MCRes *r) {
    r->mean = arr_mean(r->samples, r->n);
    r->var = arr_var(r->samples, r->n, r->mean);
    r->sd = my_sqrt(r->var);
    sort_arr(r->samples, r->n);
    r->mn = r->samples[0]; r->mx = r->samples[r->n - 1];
    int lo = (int)(0.025 * (double)r->n), hi = (int)(0.975 * (double)r->n);
    if (hi >= r->n) hi = r->n - 1;
    r->ci_lo = r->samples[lo]; r->ci_hi = r->samples[hi];
}
static double mc_sens(double *base, int np, int idx) {
    double orig = base[idx];
    base[idx] = orig - 0.1 * abs_d(orig) - 0.01;
    double vlo = mc_model(base, np);
    base[idx] = orig + 0.1 * abs_d(orig) + 0.01;
    double vhi = mc_model(base, np);
    base[idx] = orig; return abs_d(vhi - vlo);
}
static void mc_run(void) {
    ps("[MC] Monte Carlo analysis\n");
    MCRes r; r.n = MAX_MC;
    rng_state = 0xCAFEBABEu;
    double base[3] = {2.0, 1.5, 3.0};
    for (int i = 0; i < MAX_MC; i++) {
        double p[3];
        p[0] = base[0] + rand_range(-0.5, 0.5);
        p[1] = base[1] + rand_range(-0.5, 0.5);
        p[2] = base[2] + rand_range(-0.5, 0.5);
        r.samples[i] = mc_model(p, 3);
    }
    mc_analyze(&r);
    ps("  N="); pi(r.n); ps(" mean="); pd(r.mean);
    ps(" std="); pd(r.sd); ps("\n");
    ps("  95% CI: ["); pd(r.ci_lo); ps(", "); pd(r.ci_hi);
    ps("] min="); pd(r.mn); ps(" max="); pd(r.mx); ps("\n");
    ps("  Sensitivity:");
    for (int k = 0; k < 3; k++) {
        double s = mc_sens(base, 3, k);
        ps(" p"); pi(k); ps("="); pd(s);
    }
    ps("\n");
    int below = 0;
    for (int i = 0; i < r.n; i++) if (r.samples[i] < r.mean) below++;
    ps("  P(X<mean)="); pd((double)below / (double)r.n); ps("\n");
}

/* ============================================================
 * 5. Optimization (Metaheuristics)
 * ============================================================ */
static double obj_rosen(double *x, int dim) {
    double s = 0.0;
    for (int i = 0; i < dim - 1; i++) {
        double a = x[i + 1] - x[i] * x[i], b = 1.0 - x[i];
        s += 100.0 * a * a + b * b;
    } return s;
}
static double obj_sphere(double *x, int dim) {
    double s = 0.0; for (int i = 0; i < dim; i++) s += x[i] * x[i]; return s;
}
static void genetic_opt(ObjFunc f, int dim, int psz, int gens) {
    ps("[OPT] Genetic Algorithm\n");
    Ind pop[MAX_POP];
    for (int i = 0; i < psz; i++) {
        for (int j = 0; j < dim; j++) pop[i].pos[j] = rand_range(-5.0, 5.0);
        pop[i].fit = f(pop[i].pos, dim);
    }
    for (int g = 0; g < gens; g++) {
        for (int i = 0; i < psz - 2; i += 2) {
            int pa = (int)(randf() * (double)psz) % psz;
            int pb = (int)(randf() * (double)psz) % psz;
            if (pop[pb].fit < pop[pa].fit) pa = pb;
            int pc = (int)(randf() * (double)psz) % psz;
            int pd2 = (int)(randf() * (double)psz) % psz;
            if (pop[pd2].fit < pop[pc].fit) pc = pd2;
            Ind ch; double cr = randf();
            for (int j = 0; j < dim; j++) {
                ch.pos[j] = cr * pop[pa].pos[j] + (1.0 - cr) * pop[pc].pos[j];
                if (randf() < 0.1) ch.pos[j] += rand_range(-0.5, 0.5);
            }
            ch.fit = f(ch.pos, dim);
            int worst = 0;
            for (int k = 1; k < psz; k++) if (pop[k].fit > pop[worst].fit) worst = k;
            if (ch.fit < pop[worst].fit) pop[worst] = ch;
        }
    }
    int best = 0;
    for (int i = 1; i < psz; i++) if (pop[i].fit < pop[best].fit) best = i;
    ps("  Best fit: "); pd(pop[best].fit); ps(" x:");
    for (int j = 0; j < dim; j++) { ps(" "); pd(pop[best].pos[j]); } ps("\n");
}
static void pso_opt(ObjFunc f, int dim, int np2, int iters) {
    ps("[OPT] Particle Swarm\n");
    Ind sw[MAX_POP]; double vel[MAX_POP][MAX_DIM], pbp[MAX_POP][MAX_DIM], pbf[MAX_POP];
    double gbp[MAX_DIM]; double gbf = 1e30;
    for (int i = 0; i < np2; i++) {
        for (int j = 0; j < dim; j++) {
            sw[i].pos[j] = rand_range(-5.0, 5.0); vel[i][j] = rand_range(-1.0, 1.0);
            pbp[i][j] = sw[i].pos[j];
        }
        sw[i].fit = f(sw[i].pos, dim); pbf[i] = sw[i].fit;
        if (sw[i].fit < gbf) {
            gbf = sw[i].fit; for (int j = 0; j < dim; j++) gbp[j] = sw[i].pos[j];
        }
    }
    double w = 0.7, c1 = 1.5, c2 = 1.5;
    for (int it = 0; it < iters; it++) {
        for (int i = 0; i < np2; i++) {
            for (int j = 0; j < dim; j++) {
                double r1 = randf(), r2 = randf();
                vel[i][j] = w * vel[i][j] + c1 * r1 * (pbp[i][j] - sw[i].pos[j])
                    + c2 * r2 * (gbp[j] - sw[i].pos[j]);
                sw[i].pos[j] += vel[i][j];
            }
            sw[i].fit = f(sw[i].pos, dim);
            if (sw[i].fit < pbf[i]) {
                pbf[i] = sw[i].fit;
                for (int j = 0; j < dim; j++) pbp[i][j] = sw[i].pos[j];
            }
            if (sw[i].fit < gbf) {
                gbf = sw[i].fit;
                for (int j = 0; j < dim; j++) gbp[j] = sw[i].pos[j];
            }
        }
        w *= 0.995;
    }
    ps("  Best fit: "); pd(gbf); ps(" x:");
    for (int j = 0; j < dim; j++) { ps(" "); pd(gbp[j]); } ps("\n");
}
static void sa_opt(ObjFunc f, int dim) {
    ps("[OPT] Simulated Annealing\n");
    double x[MAX_DIM], bx[MAX_DIM];
    for (int j = 0; j < dim; j++) { x[j] = rand_range(-5.0, 5.0); bx[j] = x[j]; }
    double fx = f(x, dim), bfx = fx, T = 100.0, eps = 1e-6;
    for (int it = 0; it < 512; it++) {
        int idx = (int)(randf() * (double)dim) % dim;
        double xo = x[idx]; x[idx] += rand_range(-1.0, 1.0);
        double fn = f(x, dim), delta = fn - fx;
        if (delta < 0.0 || randf() < my_exp(-delta / T)) {
            fx = fn;
            if (fx < bfx) { bfx = fx; for (int j = 0; j < dim; j++) bx[j] = x[j]; }
        } else x[idx] = xo;
        T *= 0.99;
    }
    ps("  Best fit: "); pd(bfx); ps(" x:");
    for (int j = 0; j < dim; j++) { ps(" "); pd(bx[j]); } ps("\n");
}
static void grad_opt(ObjFunc f, int dim) {
    ps("[OPT] Gradient Descent\n");
    double x[MAX_DIM];
    for (int j = 0; j < dim; j++) x[j] = rand_range(-2.0, 2.0);
    double lr = 0.01, eps = 1e-6;
    for (int it = 0; it < 512; it++) {
        double g[MAX_DIM], fx = f(x, dim);
        for (int j = 0; j < dim; j++) {
            double xp[MAX_DIM];
            for (int k = 0; k < dim; k++) xp[k] = x[k];
            xp[j] += eps; g[j] = (f(xp, dim) - fx) / eps;
        }
        for (int j = 0; j < dim; j++) x[j] -= lr * g[j];
        lr *= 0.999;
    }
    ps("  Best fit: "); pd(f(x, dim)); ps(" x:");
    for (int j = 0; j < dim; j++) { ps(" "); pd(x[j]); } ps("\n");
}
static void multi_obj(void) {
    ps("[OPT] Multi-Objective Pareto\n");
    Ind pop[16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) pop[i].pos[j] = rand_range(-3.0, 3.0);
        double f1 = obj_sphere(pop[i].pos, 4), sh[4];
        for (int j = 0; j < 4; j++) sh[j] = pop[i].pos[j] - 1.0;
        pop[i].fit = 0.5 * f1 + 0.5 * obj_sphere(sh, 4);
    }
    int par[16], np2 = 0;
    for (int i = 0; i < 16; i++) {
        int dom = 0;
        for (int j = 0; j < 16; j++)
            if (i != j && pop[j].fit < pop[i].fit) { dom = 1; break; }
        if (!dom) par[np2++] = i;
    }
    ps("  Pareto count: "); pi(np2); ps("\n");
}
static void opt_run(void) {
    ps("[OPT] Optimization suite\n");
    rng_state = 0x12345678u;
    genetic_opt(obj_rosen, 4, 20, 50);
    pso_opt(obj_sphere, 4, 16, 100);
    sa_opt(obj_rosen, 4);
    grad_opt(obj_sphere, 4);
    multi_obj();
}

/* ============================================================
 * 6. Result Analysis
 * ============================================================ */
static void rpt_compute(Report *r) {
    r->mean = arr_mean(r->data, r->n);
    sort_arr(r->data, r->n);
    r->median = r->data[r->n / 2];
    r->sd = my_sqrt(arr_var(r->data, r->n, r->mean));
    double sx = 0, sy = 0, sxx = 0, sxy = 0;
    for (int i = 0; i < r->n; i++) {
        sx += (double)i; sy += r->data[i];
        sxx += (double)i * (double)i; sxy += (double)i * r->data[i];
    }
    double dn = (double)r->n, den = dn * sxx - sx * sx;
    r->trend = abs_d(den) > 1e-12 ? (dn * sxy - sx * sy) / den : 0.0;
    r->anomaly = 0;
    double thr = 3.0 * r->sd;
    for (int i = 0; i < r->n; i++)
        if (abs_d(r->data[i] - r->mean) > thr) { r->anomaly = 1; r->anom_idx = i; break; }
}
static void rpt_print(Report *r) {
    ps("  ["); ps(r->label); ps("] N="); pi(r->n);
    ps(" mean="); pd(r->mean); ps(" med="); pd(r->median);
    ps(" std="); pd(r->sd); ps(" trend="); pd(r->trend); ps("\n");
    if (r->anomaly) {
        ps("    ANOMALY idx="); pi(r->anom_idx);
        ps(" val="); pd(r->data[r->anom_idx]); ps("\n");
    } else ps("    No anomalies\n");
}
static void chart(const char *title, double *vals, int n, int rows) {
    ps("  Chart: "); ps(title); ps("\n");
    double mx = vals[0], mn = vals[0];
    for (int i = 1; i < n; i++) {
        if (vals[i] > mx) mx = vals[i]; if (vals[i] < mn) mn = vals[i];
    }
    double rng = mx - mn; if (rng < 1e-12) rng = 1.0;
    for (int r = rows; r >= 0; r--) {
        double thr = mn + rng * (double)r / (double)rows;
        ps("    "); pd(thr); ps(" |");
        for (int c = 0; c < n; c++) ps(vals[c] >= thr ? "#" : " ");
        ps("\n");
    }
}
static void comparative(void) {
    ps("[ANALYSIS] Comparative A vs B\n");
    Report a, b; a.label = "A"; b.label = "B"; a.n = 20; b.n = 20;
    for (int i = 0; i < 20; i++) {
        a.data[i] = 10.0 + (double)i * 0.3 + rand_range(-0.5, 0.5);
        b.data[i] = 12.0 + (double)i * 0.1 + rand_range(-0.5, 0.5);
    }
    a.data[15] = 50.0;
    rpt_compute(&a); rpt_compute(&b);
    rpt_print(&a); rpt_print(&b);
    ps("  Diff A-B: "); pd(a.mean - b.mean); ps("\n");
    ps("  A: "); ps(a.trend > 0.1 ? "UP" : (a.trend < -0.1 ? "DOWN" : "FLAT"));
    ps("  B: "); ps(b.trend > 0.1 ? "UP" : (b.trend < -0.1 ? "DOWN" : "FLAT"));
    ps("\n");
}
static void analysis_run(void) {
    ps("[ANALYSIS] Result analysis\n");
    Report r; r.label = "Primary"; r.n = MAX_RPT;
    rng_state = 0xABCD1234u;
    for (int i = 0; i < MAX_RPT; i++)
        r.data[i] = 5.0 + 0.1 * (double)i + rand_range(-1.0, 1.0);
    r.data[25] = 30.0;
    rpt_compute(&r); rpt_print(&r);
    chart("Output", r.data, 16, 4);
    comparative();
}

/* ============================================================
 * Main
 * ============================================================ */
__attribute__((export_name("main")))
int main(void) {
    ps(VER); ps("\n============================================\n");
    char buf[MAX_BUF];
    int argc = host_get_argv((unsigned int)(size_t)buf, MAX_BUF);
    ps("Args: "); pi(argc); ps("\n");

    DES des;
    des_init(&des, 5.0); des_add_res(&des, 2.0); des_add_q(&des, 16);
    des_run(&des);
    ps("--------------------------------------------\n");

    ContSim cs; cont_init(&cs, 2, 0.01, 10.0);
    cont_run(&cs, odefunc_harmonic, "Harmonic oscillator");
    ps("--------------------------------------------\n");

    ContSim cl; cont_init(&cl, 3, 0.005, 5.0);
    cl.x[0] = 1.0; cl.x[1] = 1.0; cl.x[2] = 1.0;
    cont_run(&cl, odefunc_lorenz, "Lorenz attractor");
    ps("--------------------------------------------\n");

    phys_run();
    ps("--------------------------------------------\n");
    mc_run();
    ps("--------------------------------------------\n");
    opt_run();
    ps("--------------------------------------------\n");
    analysis_run();

    ps("============================================\n");
    ps("Twin simulation complete.\n");
    return 0;
}
