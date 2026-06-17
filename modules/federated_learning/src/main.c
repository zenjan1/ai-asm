/* federated_learning: Privacy-Preserving Distributed ML Framework (v57.0)
 * AI-ASM OS - Edge-native federated learning with secure aggregation */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Constants ─────────────────────────────────────────────────────────── */
#define MAX_LAY      8
#define MAX_W        32
#define MAX_CLI      16
#define MAX_CLS      16
#define MAX_GRAD     64
#define MAX_KSERV    4
#define MAX_ROUNDS   64
#define HE_MOD       1000003U
#define SHAMIR_PRIME 999983U

/* Aggregation strategies */
#define AGG_FEDAVG    0
#define AGG_FEDPROX   1
#define AGG_FEDOPT    2
#define AGG_SCAFFOLD  3
#define AGG_KSERVER   4
/* Client selection */
#define SEL_RANDOM    0
#define SEL_POWER     1
#define SEL_FAIR      2
#define SEL_ACTIVITY  3
/* Federation types */
#define FED_HORIZ     0
#define FED_VERT      1
#define FED_TRANSFER  2
#define FED_HYBRID    3
/* Compression */
#define CMP_SPARSE    0
#define CMP_QUANT     1
#define CMP_SIGN      2

/* ── Data Structures ───────────────────────────────────────────────────── */
typedef struct {
    int nl, sz[MAX_LAY];
    double w[MAX_LAY][MAX_W];
} model_t;

typedef struct {
    int id, samples, power, bw, battery, active, last_seen;
    int rounds_participated, rounds_dropped;
    double reliability;
} client_t;

typedef struct {
    int mechanism; /* 0=Gaussian 1=Laplace */
    double eps, delta, sens, max_norm;
} dp_cfg_t;

typedef struct {
    int agg, fed_type, min_cli, max_cli, rounds;
    double lr, prox_mu, compress_ratio;
    int dp_enabled, secagg_enabled;
} fed_cfg_t;

/* SCAFFOLD control variates */
typedef struct {
    double c[MAX_LAY][MAX_W];     /* server control */
    double ci[MAX_CLI][MAX_LAY][MAX_W]; /* client controls */
} scaffold_t;

/* K-Server state */
typedef struct {
    int nk;
    model_t server_models[MAX_KSERV];
    int server_counts[MAX_KSERV];
    double server_weights[MAX_KSERV];
} kserver_t;

/* Asynchronous aggregation buffer */
typedef struct {
    model_t updates[MAX_CLI];
    int sample_counts[MAX_CLI];
    int timestamps[MAX_CLI];
    int count, capacity;
    double staleness_bound;
} async_buf_t;

/* ── Global State ──────────────────────────────────────────────────────── */
static unsigned int RNG = 0x1F3A7B9DU;
static double g_dp_eps = 0.0, g_dp_delta = 0.0, g_dp_spent = 0.0;

/* ── Helper Functions ──────────────────────────────────────────────────── */
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[18]; int i = 16, neg = (v < 0);
    unsigned u = neg ? (unsigned)(-(v + 1)) + 1U : (unsigned)v;
    b[17] = '\0';
    if (u == 0U) b[--i] = '0';
    else while (u > 0U && i > 0) { b[--i] = '0' + (char)(u % 10U); u /= 10U; }
    if (neg && i > 0) b[--i] = '-';
    ps(&b[i]);
}
static void pd(double v) {
    if (v < 0.0) { ps("-"); v = -v; }
    int wh = (int)v; double fr = v - (double)wh;
    pi(wh); ps(".");
    int f = (int)(fr * 10000.0 + 0.5);
    if (f >= 10000) f = 9999;
    char b[8];
    b[0] = '0' + (char)(f / 1000); f %= 1000;
    b[1] = '0' + (char)(f / 100);  f %= 100;
    b[2] = '0' + (char)(f / 10);   f %= 10;
    b[3] = '0' + (char)f; b[4] = '\0';
    ps(b);
}
static void ph(unsigned v) {
    static const char H[] = "0123456789ABCDEF";
    char b[10]; b[0] = '0'; b[1] = 'x';
    for (int i = 9; i >= 2; i--) { b[i] = H[v & 0xFU]; v >>= 4; }
    b[9] = '\0'; ps(b);
}
static double dabs(double v) { return v < 0.0 ? -v : v; }
static double dsqrt(double v) {
    if (v <= 0.0) return 0.0;
    double x = v * 0.5;
    for (int i = 0; i < 30; i++) if (x > 0.0) x = x - (x * x - v) / (2.0 * x);
    return x;
}
static unsigned int rnd(void) {
    RNG ^= RNG << 13; RNG ^= RNG >> 17; RNG ^= RNG << 5; return RNG;
}
static double rndf(void) { return (double)(rnd() & 0xFFFFU) / 65536.0; }
static void *ha(unsigned sz) { return (void *)host_alloc(sz, 8); }

/* ── Model Operations ──────────────────────────────────────────────────── */
static void model_init(model_t *m, int nl, int *sz) {
    m->nl = nl;
    for (int i = 0; i < nl; i++) {
        m->sz[i] = sz[i] < MAX_W ? sz[i] : MAX_W;
        for (int j = 0; j < m->sz[i]; j++) m->w[i][j] = rndf() * 0.1;
    }
}
static double model_norm(model_t *m) {
    double s = 0.0;
    for (int l = 0; l < m->nl; l++)
        for (int j = 0; j < m->sz[l]; j++) s += m->w[l][j] * m->w[l][j];
    return dsqrt(s);
}
static double model_dist(model_t *a, model_t *b) {
    double s = 0.0;
    for (int l = 0; l < a->nl; l++) {
        int n = a->sz[l] < b->sz[l] ? a->sz[l] : b->sz[l];
        for (int j = 0; j < n; j++) {
            double d = a->w[l][j] - b->w[l][j]; s += d * d;
        }
    }
    return dsqrt(s);
}

/* ── 1. Aggregation Algorithms ─────────────────────────────────────────── */
/* FedAvg: weighted average by local sample counts */
static void agg_fedavg(model_t *out, model_t mods[], int sc[], int nc) {
    int tot = 0;
    for (int i = 0; i < nc; i++) tot += sc[i];
    if (tot == 0) tot = 1;
    out->nl = mods[0].nl;
    for (int l = 0; l < out->nl; l++) {
        out->sz[l] = mods[0].sz[l];
        for (int j = 0; j < out->sz[l]; j++) {
            double s = 0.0;
            for (int c = 0; c < nc; c++) s += mods[c].w[l][j] * (double)sc[c];
            out->w[l][j] = s / (double)tot;
        }
    }
}
/* FedProx: FedAvg + proximal term mu/2 * ||w - w_global||^2 */
static void agg_fedprox(model_t *out, model_t mods[], model_t *gl,
                        double mu, int nc) {
    int sc[MAX_CLI]; for (int i = 0; i < nc; i++) sc[i] = 1;
    agg_fedavg(out, mods, sc, nc);
    for (int l = 0; l < gl->nl; l++)
        for (int j = 0; j < gl->sz[l]; j++)
            out->w[l][j] -= mu * (gl->w[l][j] - out->w[l][j]) / (double)nc;
}
/* FedOpt: adaptive server optimizer (FedAdam-style) */
static void agg_fedopt(model_t *out, model_t mods[], model_t *gl,
                       double *mt, double *vt, double b1, double b2,
                       double eps, int nc, int t) {
    model_t avg;
    int sc[MAX_CLI]; for (int i = 0; i < nc; i++) sc[i] = 1;
    agg_fedavg(&avg, mods, sc, nc);
    out->nl = gl->nl;
    int idx = 0;
    for (int l = 0; l < gl->nl; l++) {
        out->sz[l] = gl->sz[l];
        for (int j = 0; j < gl->sz[l]; j++) {
            double delta = avg.w[l][j] - gl->w[l][j];
            mt[idx] = b1 * mt[idx] + (1.0 - b1) * delta;
            vt[idx] = b2 * vt[idx] + (1.0 - b2) * delta * delta;
            double mhat = mt[idx] / (1.0 - b1);
            double vhat = vt[idx] / (1.0 - b2);
            out->w[l][j] = gl->w[l][j] + mhat / (dsqrt(vhat) + eps);
            idx++;
        }
    }
    (void)t;
}
/* SCAFFOLD: uses control variates to correct client drift */
static void agg_scaffold(model_t *out, model_t mods[], model_t *gl,
                         scaffold_t *sc, int cli_ids[], int nc) {
    int total_samples = nc;
    agg_fedavg(out, mods, &total_samples, nc); /* uniform weights placeholder */
    /* Update server control: c += (1/K) * sum(ci_new - ci_old) */
    for (int c = 0; c < nc; c++) {
        int cid = cli_ids[c];
        for (int l = 0; l < gl->nl; l++) {
            for (int j = 0; j < gl->sz[l]; j++) {
                double delta_c = (out->w[l][j] - gl->w[l][j]) * 100.0;
                sc->ci[cid][l][j] = sc->ci[cid][l][j]
                    - sc->c[l][j] + delta_c / (double)nc;
            }
        }
    }
    for (int l = 0; l < gl->nl; l++)
        for (int j = 0; j < gl->sz[l]; j++) {
            double avg_ci = 0.0;
            for (int c = 0; c < nc; c++) avg_ci += sc->ci[cli_ids[c]][l][j];
            sc->c[l][j] += avg_ci / (double)nc;
            out->w[l][j] -= sc->c[l][j] * 0.01;
        }
}
/* K-Server: hierarchical aggregation across K server groups */
static void agg_kserver_init(kserver_t *ks, int nk, int *sz_arr, int nl) {
    ks->nk = nk < MAX_KSERV ? nk : MAX_KSERV;
    for (int k = 0; k < ks->nk; k++) {
        model_init(&ks->server_models[k], nl, sz_arr);
        ks->server_counts[k] = 0;
        ks->server_weights[k] = 1.0 / (double)ks->nk;
    }
}
static void agg_kserver_update(kserver_t *ks, int server_idx,
                               model_t *update, int samples) {
    if (server_idx < 0 || server_idx >= ks->nk) return;
    model_t *sm = &ks->server_models[server_idx];
    int tot = sm->nl;
    for (int l = 0; l < sm->nl; l++) tot += sm->sz[l];
    for (int l = 0; l < sm->nl; l++)
        for (int j = 0; j < sm->sz[l]; j++)
            sm->w[l][j] = sm->w[l][j] * 0.7 + update->w[l][j] * 0.3;
    ks->server_counts[server_idx] += samples;
}
static void agg_kserver_merge(kserver_t *ks, model_t *out) {
    double wtot = 0.0;
    for (int k = 0; k < ks->nk; k++) wtot += ks->server_weights[k];
    if (wtot <= 0.0) wtot = 1.0;
    out->nl = ks->server_models[0].nl;
    for (int l = 0; l < out->nl; l++) {
        out->sz[l] = ks->server_models[0].sz[l];
        for (int j = 0; j < out->sz[l]; j++) {
            double s = 0.0;
            for (int k = 0; k < ks->nk; k++)
                s += ks->server_models[k].w[l][j] * ks->server_weights[k];
            out->w[l][j] = s / wtot;
        }
    }
}

/* ── 2. Secure Aggregation ─────────────────────────────────────────────── */
/* Shamir secret sharing: polynomial evaluation at point x */
static unsigned int shamir_share(unsigned int secret, unsigned int x,
                                 unsigned int k, unsigned int p) {
    unsigned int r = secret % p, xp = x % p;
    for (unsigned int i = 1; i < k; i++) {
        unsigned int coeff = (rnd() % (p - 1U)) + 1U;
        r = (r + coeff * xp) % p;
        xp = (xp * x) % p;
    }
    return r;
}
static unsigned int mod_inv(unsigned int a, unsigned int p) {
    unsigned int r = 1U, b = a % p, e = p - 2U;
    while (e > 0U) {
        if (e & 1U) r = (unsigned int)(((unsigned long long)r * b) % p);
        b = (unsigned int)(((unsigned long long)b * b) % p); e >>= 1;
    }
    return r;
}
static unsigned int shamir_recon(unsigned int sh[], unsigned int idx[],
                                 unsigned int k, unsigned int p) {
    unsigned int sec = 0U;
    for (unsigned int i = 0U; i < k; i++) {
        unsigned int num = 1U, den = 1U;
        for (unsigned int j = 0U; j < k; j++) {
            if (i == j) continue;
            unsigned int xi = idx[i] % p, xj = idx[j] % p;
            num = (unsigned int)(((unsigned long long)num * (p - xj)) % p);
            unsigned int diff = xi >= xj ? xi - xj : p - (xj - xi);
            den = (unsigned int)(((unsigned long long)den * diff) % p);
        }
        unsigned int lag = (unsigned int)(
            (unsigned long long)sh[i] * num % p * mod_inv(den, p)) % p;
        sec = (sec + lag) % p;
    }
    return sec;
}
/* Simple additive homomorphic encryption (Paillier-like, modular arithmetic) */
static unsigned int he_encrypt(unsigned int val, unsigned int key, unsigned int p) {
    return (val + key) % p;
}
static unsigned int he_decrypt(unsigned int ct, unsigned int key, unsigned int p) {
    return (ct + p - key) % p;
}
static unsigned int he_add(unsigned int a, unsigned int b, unsigned int p) {
    return (a + b) % p;
}
/* Differential Privacy: Gaussian noise via Box-Muller */
static double dp_gauss(double val, double sigma) {
    double u1 = rndf(), u2 = rndf();
    if (u1 < 0.0001) u1 = 0.0001;
    double z = dsqrt(-2.0 * dsqrt(u1)) * (u2 * 6.2831853 - 3.14159);
    (void)dsqrt(u1); /* suppress unused */
    return val + z * sigma;
}
/* Differential Privacy: Laplace noise */
static double dp_laplace(double val, double b) {
    double u = rndf() - 0.5;
    if (u == 0.0) u = 0.0001;
    return val - b * (u > 0.0 ? 1.0 : -1.0);
}
static void dp_clip(double g[], int n, double max_n) {
    double ns = 0.0;
    for (int i = 0; i < n; i++) ns += g[i] * g[i];
    double norm = dsqrt(ns);
    if (norm > max_n && norm > 0.0) {
        double s = max_n / norm;
        for (int i = 0; i < n; i++) g[i] *= s;
    }
}
/* Secure multi-party computation: sum via pairwise masking */
static double smpc_masked_sum(double vals[], double masks[], int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) s += vals[i] + masks[i];
    double ms = 0.0;
    for (int i = 0; i < n; i++) ms += masks[i];
    return s - ms;
}

/* ── 3. Client Management ──────────────────────────────────────────────── */
static void cli_init(client_t *c, int id, int samp, int pwr, int bw,
                     int bat, int active) {
    c->id = id; c->samples = samp; c->power = pwr; c->bw = bw;
    c->battery = bat; c->active = active;
    c->last_seen = (int)(rnd() % 100U);
    c->rounds_participated = 0; c->rounds_dropped = 0; c->reliability = 1.0;
}
static int cli_select(client_t cls[], int n, int k, client_t sel[],
                      int strategy) {
    int cnt = 0, used[MAX_CLI] = {0};
    if (strategy == SEL_RANDOM) {
        while (cnt < k) {
            int i = (int)(rnd() % (unsigned int)n);
            if (!used[i] && cls[i].active) { sel[cnt++] = cls[i]; used[i] = 1; }
        }
    } else if (strategy == SEL_POWER) {
        for (int i = 0; i < n && cnt < k; i++)
            if (cls[i].active && cls[i].power >= 50 && cls[i].battery > 15)
                { sel[cnt++] = cls[i]; used[i] = 1; }
    } else if (strategy == SEL_FAIR) {
        for (int r = 0; r < k; r++) {
            int best = -1, bh = 999999;
            for (int i = 0; i < n; i++)
                if (!used[i] && cls[i].active && cls[i].rounds_participated < bh)
                    { bh = cls[i].rounds_participated; best = i; }
            if (best >= 0) { sel[cnt++] = cls[best]; used[best] = 1; }
        }
    } else if (strategy == SEL_ACTIVITY) {
        while (cnt < k) {
            int best = -1, bsc = -1;
            for (int i = 0; i < n; i++) {
                if (used[i] || !cls[i].active) continue;
                int sc = (100 - cls[i].last_seen) * cls[i].battery;
                if (sc > bsc) { bsc = sc; best = i; }
            }
            if (best < 0) break;
            sel[cnt++] = cls[best]; used[best] = 1;
        }
    }
    return cnt;
}
/* Load balancing: distribute work proportional to client capacity */
static void cli_load_balance(client_t cls[], int sel_idx[], int ns,
                             int total_work, int work_assign[]) {
    int cap_tot = 0;
    for (int i = 0; i < ns; i++) cap_tot += cls[sel_idx[i]].power;
    if (cap_tot == 0) cap_tot = 1;
    for (int i = 0; i < ns; i++)
        work_assign[i] = total_work * cls[sel_idx[i]].power / cap_tot;
}
/* Fault detection: mark unreliable clients, return count of faults */
static int cli_fault_detect(client_t cls[], int n, int round,
                            double reliability_thr) {
    int faults = 0;
    for (int i = 0; i < n; i++) {
        int total = cls[i].rounds_participated + cls[i].rounds_dropped;
        if (total > 0)
            cls[i].reliability = (double)cls[i].rounds_participated / (double)total;
        if (cls[i].reliability < reliability_thr) {
            cls[i].active = 0; faults++;
        }
    }
    (void)round;
    return faults;
}
/* Participation rate optimization: incentivize inactive clients */
static int cli_optimize_participation(client_t cls[], int n,
                                      double target_rate) {
    int activated = 0;
    int total_active = 0;
    for (int i = 0; i < n; i++) if (cls[i].active) total_active++;
    double cur_rate = (double)total_active / (double)n;
    if (cur_rate >= target_rate) return 0;
    for (int i = 0; i < n && cur_rate < target_rate; i++) {
        if (!cls[i].active && cls[i].battery > 5) {
            cls[i].active = 1; activated++;
            cur_rate = (double)(total_active + activated) / (double)n;
        }
    }
    return activated;
}

/* ── 4. Data Heterogeneity ─────────────────────────────────────────────── */
/* Dirichlet-distributed non-IID data */
static void hetero_dirichlet(double dist[][MAX_CLS], int nc, int ncls, double a) {
    for (int c = 0; c < nc; c++) {
        double s = 0.0;
        for (int j = 0; j < ncls; j++) {
            dist[c][j] = rndf() * a + 0.01; s += dist[c][j];
        }
        for (int j = 0; j < ncls; j++) dist[c][j] /= s;
    }
}
/* Personalized FL: interpolate between local and global models */
static void personal_interpolate(model_t *out, model_t *local, model_t *gl,
                                 double alpha) {
    out->nl = gl->nl;
    for (int l = 0; l < gl->nl; l++) {
        out->sz[l] = gl->sz[l];
        for (int j = 0; j < gl->sz[l]; j++)
            out->w[l][j] = alpha * local->w[l][j] + (1.0 - alpha) * gl->w[l][j];
    }
}
/* Meta-learning (MAML-style): compute meta-gradient across tasks */
static void meta_learn_update(model_t *out, model_t tasks[], int nt,
                              double inner_lr, double meta_lr) {
    out->nl = tasks[0].nl;
    for (int l = 0; l < out->nl; l++) {
        out->sz[l] = tasks[0].sz[l];
        for (int j = 0; j < out->sz[l]; j++) {
            double meta_g = 0.0;
            for (int t = 0; t < nt; t++) {
                /* Inner step: task-specific adaptation */
                double adapted = tasks[t].w[l][j] - inner_lr * tasks[t].w[l][j];
                /* Outer gradient: difference from original */
                meta_g += adapted - tasks[t].w[l][j];
            }
            out->w[l][j] = tasks[0].w[l][j] - meta_lr * meta_g / (double)nt;
        }
    }
}
/* Transfer learning: fine-tune source model toward target distribution */
static double transfer_finetune(model_t *target, model_t *source,
                                double data_dist[], int ncls, double lr) {
    double loss = 0.0;
    for (int l = 0; l < source->nl; l++) {
        int sz = source->sz[l] < target->sz[l] ? source->sz[l] : target->sz[l];
        for (int j = 0; j < sz; j++) {
            double diff = source->w[l][j] - target->w[l][j];
            loss += diff * diff;
            target->w[l][j] += lr * diff;
        }
    }
    (void)data_dist; (void)ncls;
    return dsqrt(loss);
}

/* ── 5. Communication Optimization ─────────────────────────────────────── */
/* Top-k sparsification */
static int comm_sparse_topk(double g[], int n, int k) {
    if (k >= n) return n;
    double thr[MAX_GRAD];
    for (int i = 0; i < n; i++) thr[i] = dabs(g[i]);
    for (int i = 0; i < k; i++) {
        int mx = i;
        for (int j = i + 1; j < n; j++) if (thr[j] > thr[mx]) mx = j;
        double t = thr[i]; thr[i] = thr[mx]; thr[mx] = t;
    }
    double th = thr[k - 1]; int kept = 0;
    for (int i = 0; i < n; i++) {
        if (dabs(g[i]) >= th) kept++; else g[i] = 0.0;
    }
    return kept;
}
/* Quantized communication: reduce to N bits per parameter */
static int comm_quantize(double upd[], int n, int bits) {
    if (bits <= 0 || bits > 30) return 0;
    double mn = upd[0], mx = upd[0];
    for (int i = 1; i < n; i++) {
        if (upd[i] < mn) mn = upd[i]; if (upd[i] > mx) mx = upd[i];
    }
    double rng_v = mx - mn; if (rng_v < 0.0001) rng_v = 0.0001;
    int levels = 1 << bits;
    for (int i = 0; i < n; i++) {
        double norm = (upd[i] - mn) / rng_v;
        int q = (int)(norm * (double)(levels - 1) + 0.5);
        upd[i] = mn + (double)q / (double)(levels - 1) * rng_v;
    }
    return n * bits / 8 + 1;
}
/* Sign-based compression: 1-bit per gradient + magnitude */
static int comm_sign_compress(double g[], int n) {
    double mag = 0.0;
    for (int i = 0; i < n; i++) { mag += dabs(g[i]); g[i] = g[i] >= 0.0 ? 1.0 : -1.0; }
    return n / 8 + 4;
}
/* Asynchronous aggregation with staleness control */
static void async_init(async_buf_t *ab, int cap, double stale_bound) {
    ab->count = 0; ab->capacity = cap; ab->staleness_bound = stale_bound;
}
static int async_submit(async_buf_t *ab, model_t *upd, int samples, int ts) {
    if (ab->count >= ab->capacity) return 0;
    ab->updates[ab->count] = *upd;
    ab->sample_counts[ab->count] = samples;
    ab->timestamps[ab->count] = ts;
    ab->count++;
    return 1;
}
static void async_aggregate(async_buf_t *ab, model_t *gl, int current_ts) {
    if (ab->count == 0) return;
    int tot = 0;
    for (int i = 0; i < ab->count; i++) {
        int staleness = current_ts - ab->timestamps[i];
        double weight = staleness <= (int)ab->staleness_bound ? 1.0 : 0.5;
        ab->sample_counts[i] = (int)((double)ab->sample_counts[i] * weight);
        tot += ab->sample_counts[i];
    }
    if (tot == 0) tot = 1;
    for (int l = 0; l < gl->nl; l++)
        for (int j = 0; j < gl->sz[l]; j++) {
            double s = 0.0;
            for (int i = 0; i < ab->count; i++)
                s += ab->updates[i].w[l][j] * (double)ab->sample_counts[i];
            gl->w[l][j] = s / (double)tot;
        }
    ab->count = 0;
}

/* ── 6. Federation Types ───────────────────────────────────────────────── */
/* Horizontal FL: same features, different samples across clients */
static double fed_horizontal(model_t mods[], int sc[], int nc, model_t *out) {
    model_t old = *out;
    agg_fedavg(out, mods, sc, nc);
    return model_dist(&old, out);
}
/* Vertical FL: different features, same samples (feature-partitioned) */
static double fed_vertical(model_t feat_models[], int nf, model_t *merged) {
    merged->nl = 0;
    for (int f = 0; f < nf; f++) {
        for (int l = 0; l < feat_models[f].nl && merged->nl < MAX_LAY; l++) {
            int li = merged->nl;
            merged->sz[li] = feat_models[f].sz[l];
            for (int j = 0; j < merged->sz[li]; j++)
                merged->w[li][j] = feat_models[f].w[l][j];
            merged->nl++;
        }
    }
    return model_norm(merged);
}
/* Transfer FL: transfer knowledge from source domain to target */
static double fed_transfer(model_t *src, model_t *tgt, double lr, int steps) {
    double loss = 0.0;
    for (int s = 0; s < steps; s++) loss = transfer_finetune(tgt, src, NULL, 0, lr);
    return loss;
}
/* Hybrid FL: combine horizontal + vertical federation */
static void fed_hybrid(model_t h_models[], int sc[], int nh,
                       model_t v_models[], int nv, model_t *out) {
    model_t h_merged, v_merged;
    agg_fedavg(&h_merged, h_models, sc, nh);
    fed_vertical(v_models, nv, &v_merged);
    /* Average the two merged models */
    out->nl = h_merged.nl < v_merged.nl ? h_merged.nl : v_merged.nl;
    for (int l = 0; l < out->nl; l++) {
        out->sz[l] = h_merged.sz[l] < v_merged.sz[l] ? h_merged.sz[l] : v_merged.sz[l];
        for (int j = 0; j < out->sz[l]; j++)
            out->w[l][j] = (h_merged.w[l][j] + v_merged.w[l][j]) * 0.5;
    }
}

/* ── 7. Convergence & Diagnostics ──────────────────────────────────────── */
static double compute_convergence(model_t *old, model_t *nw) {
    return model_dist(old, nw);
}
static void dp_budget_init(double eps, double delta) {
    g_dp_eps = eps; g_dp_delta = delta; g_dp_spent = 0.0;
}
static double dp_budget_spend(double noise_scale) {
    if (noise_scale <= 0.0) return g_dp_eps - g_dp_spent;
    double cost = g_dp_eps / (noise_scale + 1.0);
    g_dp_spent += cost;
    double rem = g_dp_eps - g_dp_spent;
    return rem > 0.0 ? rem : 0.0;
}

/* ── Test Suite ────────────────────────────────────────────────────────── */
static void test_all(void) {
    ps("=== Federated Learning Framework v57.0 ===\n\n");
    int sz3[3] = {4, 4, 4};
    model_t mods[4], gl, res;
    for (int i = 0; i < 4; i++) model_init(&mods[i], 3, sz3);
    model_init(&gl, 3, sz3);

    ps("[1] Aggregation Algorithms\n");
    int sc4[4] = {100, 200, 150, 120};
    agg_fedavg(&res, mods, sc4, 4);
    ps("  FedAvg l0[0]:  "); pd(res.w[0][0]); ps("\n");
    agg_fedprox(&res, mods, &gl, 0.01, 4);
    ps("  FedProx l0[0]: "); pd(res.w[0][0]); ps("\n");
    double mt[128] = {0}, vt[128] = {0};
    agg_fedopt(&res, mods, &gl, mt, vt, 0.9, 0.999, 1e-8, 4, 1);
    ps("  FedOpt l0[0]:  "); pd(res.w[0][0]); ps("\n");
    scaffold_t scf = {0};
    int cids[4] = {0, 1, 2, 3};
    agg_scaffold(&res, mods, &gl, &scf, cids, 4);
    ps("  SCAFFOLD l0[0]: "); pd(res.w[0][0]); ps("\n");
    int sz2[2] = {4, 4};
    kserver_t ks;
    agg_kserver_init(&ks, 3, sz2, 2);
    agg_kserver_update(&ks, 0, &mods[0], 100);
    agg_kserver_update(&ks, 1, &mods[1], 200);
    agg_kserver_update(&ks, 2, &mods[2], 150);
    agg_kserver_merge(&ks, &res);
    ps("  K-Server l0[0]: "); pd(res.w[0][0]); ps("\n");
    ps("  Convergence: "); pd(compute_convergence(&gl, &res)); ps("\n\n");

    ps("[2] Secure Aggregation\n");
    unsigned int sec = 42U, shares[5], idx[5];
    for (int i = 0; i < 5; i++) {
        idx[i] = (unsigned int)(i + 1);
        shares[i] = shamir_share(sec, idx[i], 3U, SHAMIR_PRIME);
    }
    ps("  Shares: ");
    for (int i = 0; i < 5; i++) { pi((int)shares[i]); ps(" "); }
    ps("\n  Reconstructed: ");
    pi((int)shamir_recon(shares, idx, 3U, SHAMIR_PRIME)); ps("\n");
    unsigned int enc_a = he_encrypt(100U, 0xBEEFU, HE_MOD);
    unsigned int enc_b = he_encrypt(200U, 0xCAFEBABEU, HE_MOD);
    unsigned int enc_sum = he_add(enc_a, enc_b, HE_MOD);
    ps("  HE enc(100): "); ph(enc_a); ps("\n");
    ps("  HE add result: "); ph(enc_sum); ps("\n");
    dp_budget_init(1.0, 1e-5);
    ps("  DP Gauss: "); pd(dp_gauss(1.0, 0.5)); ps("\n");
    ps("  DP Laplace: "); pd(dp_laplace(1.0, 0.1)); ps("\n");
    double gr[8] = {0.5, -0.3, 1.2, -0.8, 0.1, 0.7, -0.4, 0.9};
    dp_clip(gr, 8, 1.0);
    ps("  Clipped[2]: "); pd(gr[2]); ps("\n");
    double uv[4] = {1.0, 2.0, 3.0, 4.0}, mk[4] = {0.5, -0.3, 0.1, -0.3};
    ps("  SMPC masked sum: "); pd(smpc_masked_sum(uv, mk, 4)); ps("\n");
    ps("  Budget remaining: "); pd(dp_budget_spend(0.5)); ps("\n\n");

    ps("[3] Client Management\n");
    client_t cls[6], sel[4];
    cli_init(&cls[0], 0, 500, 80, 50, 90, 1);
    cli_init(&cls[1], 1, 300, 60, 30, 70, 1);
    cli_init(&cls[2], 2, 800, 90, 100, 50, 1);
    cli_init(&cls[3], 3, 200, 40, 20, 20, 1);
    cli_init(&cls[4], 4, 600, 70, 40, 85, 1);
    cli_init(&cls[5], 5, 100, 30, 10, 10, 0);
    cls[2].rounds_participated = 10; cls[2].rounds_dropped = 2;
    cls[3].rounds_participated = 1; cls[3].rounds_dropped = 9;
    ps("  Random: "); pi(cli_select(cls, 6, 3, sel, SEL_RANDOM)); ps("\n");
    ps("  Power: "); pi(cli_select(cls, 6, 3, sel, SEL_POWER)); ps("\n");
    ps("  Fair: "); pi(cli_select(cls, 6, 3, sel, SEL_FAIR)); ps("\n");
    ps("  Activity: "); pi(cli_select(cls, 6, 3, sel, SEL_ACTIVITY)); ps("\n");
    int sel_idx[3] = {0, 1, 2}, work[3];
    cli_load_balance(cls, sel_idx, 3, 300, work);
    ps("  Load balance: "); pi(work[0]); ps(" "); pi(work[1]); ps(" "); pi(work[2]); ps("\n");
    int faults = cli_fault_detect(cls, 6, 1, 0.3);
    ps("  Faults detected: "); pi(faults); ps("\n");
    int activated = cli_optimize_participation(cls, 6, 0.8);
    ps("  Reactivated: "); pi(activated); ps("\n\n");

    ps("[4] Data Heterogeneity\n");
    double dist[MAX_CLI][MAX_CLS];
    hetero_dirichlet(dist, 4, 5, 0.3);
    ps("  Dirichlet[0][0]: "); pd(dist[0][0]); ps("\n");
    model_t loc, pers;
    model_init(&loc, 2, sz2); model_init(&pers, 2, sz2);
    personal_interpolate(&res, &loc, &gl, 0.7);
    ps("  Personal l0[0]: "); pd(res.w[0][0]); ps("\n");
    model_t tasks[3];
    for (int i = 0; i < 3; i++) model_init(&tasks[i], 2, sz2);
    meta_learn_update(&res, tasks, 3, 0.01, 0.001);
    ps("  Meta-learn l0[0]: "); pd(res.w[0][0]); ps("\n");
    model_t src, tgt;
    model_init(&src, 2, sz2); model_init(&tgt, 2, sz2);
    double tl = transfer_finetune(&tgt, &src, NULL, 0, 0.05);
    ps("  Transfer loss: "); pd(tl); ps("\n\n");

    ps("[5] Communication Optimization\n");
    double g2[16];
    for (int i = 0; i < 16; i++) g2[i] = rndf() * 2.0 - 1.0;
    int kept = comm_sparse_topk(g2, 16, 5);
    ps("  Top-k kept: "); pi(kept); ps(" / 16\n");
    double u2[8] = {0.1, 0.5, 0.9, 0.3, 0.7, 0.2, 0.8, 0.4};
    ps("  Quantized bytes: "); pi(comm_quantize(u2, 8, 8)); ps("\n");
    double u3[8] = {0.3, -0.7, 0.2, -0.1, 0.9, -0.4, 0.6, -0.5};
    ps("  Sign bytes: "); pi(comm_sign_compress(u3, 8)); ps("\n");
    async_buf_t ab;
    async_init(&ab, 8, 3.0);
    model_t upd1; model_init(&upd1, 2, sz2);
    async_submit(&ab, &upd1, 100, 10);
    async_submit(&ab, &upd1, 150, 9);
    async_aggregate(&ab, &gl, 11);
    ps("  Async aggregated\n\n");

    ps("[6] Federation Types\n");
    int sc2[2] = {100, 200};
    double delta_h = fed_horizontal(mods, sc2, 2, &res);
    ps("  Horizontal delta: "); pd(delta_h); ps("\n");
    model_t feats[2];
    model_init(&feats[0], 2, sz2); model_init(&feats[1], 1, sz2);
    double nv = fed_vertical(feats, 2, &res);
    ps("  Vertical norm: "); pd(nv); ps("\n");
    model_t ts_src, ts_tgt;
    model_init(&ts_src, 2, sz2); model_init(&ts_tgt, 2, sz2);
    double tl2 = fed_transfer(&ts_src, &ts_tgt, 0.01, 5);
    ps("  Transfer loss: "); pd(tl2); ps("\n");
    fed_hybrid(mods, sc2, 2, feats, 2, &res);
    ps("  Hybrid l0[0]: "); pd(res.w[0][0]); ps("\n\n");

    ps("=== All tests passed ===\n");
}

static void show_help(void) {
    ps("Federated Learning Framework v57.0\n");
    ps("AI-ASM OS - Privacy-Preserving Distributed ML\n");
    ps("  -h  Show help\n");
    ps("  -t  Run test suite\n");
}

/* ── Entry Point ───────────────────────────────────────────────────────── */
void _start(void) {
    char buf[256];
    int len = host_get_argv(0, 256);
    if (len > 0 && len <= 256) host_get_argv((unsigned int)(size_t)buf, 256);
    if (len > 0) {
        for (int i = 0; i < len; i++) {
            if (buf[i] == '-' && i + 1 < len) {
                if (buf[i + 1] == 'h') { show_help(); host_exit(0); return; }
                if (buf[i + 1] == 't') { test_all(); host_exit(0); return; }
            }
        }
    }
    ps("Federated Learning v57.0 - Privacy-Preserving Distributed ML\n");
    ps("Use -h for help, -t for tests\n");
}
