/* quantum_circuit_simulator: Quantum circuit simulation engine (v1.0) */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ======================== CONSTANTS ======================== */
#define MAX_QUBITS    8
#define MAX_DIM       256
#define MAX_GATES     128
#define MAX_CIRCUITS  16
#define MAX_SHOTS     1024
#define DM_QUBITS     4
#define DM_DIM        16
#define NAME_LEN      32
#define PI            3.14159265358979
#define EPSILON       1.0E-9

/* Gate type identifiers */
#define GATE_IDENTITY  0x00
#define GATE_PAULI_X   0x01
#define GATE_PAULI_Y   0x02
#define GATE_PAULI_Z   0x03
#define GATE_HADAMARD  0x04
#define GATE_S         0x05
#define GATE_T         0x06
#define GATE_RX        0x07
#define GATE_RY        0x08
#define GATE_RZ        0x09
#define GATE_PHASE     0x0A
#define GATE_CNOT      0x10
#define GATE_CZ        0x11
#define GATE_SWAP      0x12
#define GATE_TOFFOLI   0x20
#define GATE_FREDKIN   0x21
/* Measurement types */
#define MEAS_COMP      0x01
#define MEAS_PAULI_Z   0x02
#define MEAS_PAULI_X   0x03
#define MEAS_PARTIAL   0x04
/* Noise channel types */
#define NOISE_DEPOL     0x01
#define NOISE_AMP_DAMP  0x02
#define NOISE_PH_DAMP   0x03
#define NOISE_THERMAL   0x04
/* Circuit states */
#define CIRCUIT_EMPTY   0x00
#define CIRCUIT_BUILT   0x01
#define CIRCUIT_RUN     0x02

/* ======================== DATA STRUCTURES ======================== */
typedef struct { double re; double im; } cx_t;

typedef struct {
    int       n_qubits;
    int       dim;
    cx_t      amp[MAX_DIM];
    int       measured;
    int       collapse_mask;
} state_vec_t;

typedef struct {
    int    type;
    int    target;
    int    control;
    int    control2;
    double theta;
    double phi;
    double lambda;
} gate_t;

typedef struct {
    char     name[NAME_LEN];
    int      n_qubits;
    int      dim;
    gate_t   gates[MAX_GATES];
    int      gate_cnt;
    int      meas_qubits[MAX_QUBITS];
    int      n_meas;
    int      state;
    int      shots;
    unsigned int histogram[MAX_DIM];
} circuit_t;

typedef struct {
    int   n_qubits;
    int   dim;
    cx_t  mat[DM_DIM * DM_DIM];
} density_mat_t;

typedef struct {
    double x;
    double y;
    double z;
    double purity;
} bloch_coord_t;

typedef struct {
    int    type;
    double param;
    int    qubit;
    double temperature;
    double t1;
    double t2;
} noise_channel_t;

typedef struct {
    int     n_params;
    double  params[32];
    double  grad[32];
    double  energy;
    int     iter;
} variational_t;

/* Global simulator state */
static circuit_t    g_circuits[MAX_CIRCUITS];
static int          g_circ_cnt = 0;
static state_vec_t  g_state;
static density_mat_t g_dm;
static unsigned int g_rng_state = 0xDEADBEEF;

/* ======================== HELPERS ======================== */
static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char buf[24]; int i = 0, neg = 0;
    unsigned int u;
    if (v < 0) { neg = 1; u = (unsigned int)(-(v + 1)) + 1U; }
    else { u = (unsigned int)v; }
    if (u == 0) { buf[i++] = '0'; }
    while (u > 0) { buf[i++] = '0' + (char)(u % 10); u /= 10; }
    if (neg) buf[i++] = '-';
    while (i > 0) { char c = buf[--i]; host_print((char[]){c, 0}); }
}

static void print_fixed(double v, int prec) {
    char buf[32]; int i = 0, neg = 0;
    if (v < 0.0) { neg = 1; v = -v; }
    int iv = (int)v; double frac = v - (double)iv;
    if (neg) buf[i++] = '-';
    char tmp[16]; int ti = 0;
    if (iv == 0) tmp[ti++] = '0';
    while (iv > 0) { tmp[ti++] = '0' + (iv % 10); iv /= 10; }
    while (ti > 0) buf[i++] = tmp[--ti];
    if (prec > 0) {
        buf[i++] = '.';
        for (int p = 0; p < prec; p++) {
            frac *= 10.0;
            int d = (int)frac;
            buf[i++] = '0' + d;
            frac -= (double)d;
        }
    }
    buf[i] = '\0';
    host_print(buf);
}

static unsigned int rng_next(void) {
    g_rng_state ^= g_rng_state << 13;
    g_rng_state ^= g_rng_state >> 17;
    g_rng_state ^= g_rng_state << 5;
    return g_rng_state;
}

static double rng_double(void) {
    return (double)(rng_next() & 0xFFFFFF) / 16777216.0;
}

/* ======================== COMPLEX ARITHMETIC ======================== */
static cx_t cx_new(double re, double im) {
    cx_t r; r.re = re; r.im = im; return r;
}

static cx_t cx_add(cx_t a, cx_t b) {
    cx_t r; r.re = a.re + b.re; r.im = a.im + b.im; return r;
}

static cx_t cx_sub(cx_t a, cx_t b) {
    cx_t r; r.re = a.re - b.re; r.im = a.im - b.im; return r;
}

static cx_t cx_mul(cx_t a, cx_t b) {
    cx_t r;
    r.re = a.re * b.re - a.im * b.im;
    r.im = a.re * b.im + a.im * b.re;
    return r;
}

static cx_t cx_scale(cx_t a, double s) {
    cx_t r; r.re = a.re * s; r.im = a.im * s; return r;
}

static double cx_norm(cx_t a) {
    return a.re * a.re + a.im * a.im;
}

static cx_t cx_conj(cx_t a) {
    cx_t r; r.re = a.re; r.im = -a.im; return r;
}

static cx_t cx_exp(double angle) {
    /* approximate cos/sin via Taylor series */
    double c = 1.0, s = 0.0, term_c = 1.0, term_s = angle;
    double a2 = angle * angle;
    for (int k = 1; k <= 10; k++) {
        term_c *= -a2 / ((2.0 * k - 1.0) * (2.0 * k));
        c += term_c;
        term_s *= -a2 / ((2.0 * k) * (2.0 * k + 1.0));
        s += term_s;
    }
    return cx_new(c, s);
}

static double cx_abs(cx_t a) {
    double n = cx_norm(a);
    double x = n, prev = 0.0;
    for (int i = 0; i < 20; i++) {
        if (x < EPSILON) return 0.0;
        prev = x;
        x = 0.5 * (x + n / x);
    }
    return prev;
}

/* ======================== STATE VECTOR ======================== */
static state_vec_t sv_create(int n_qubits) {
    state_vec_t sv;
    sv.n_qubits = n_qubits;
    sv.dim = 1 << n_qubits;
    sv.measured = 0;
    sv.collapse_mask = 0;
    for (int i = 0; i < sv.dim; i++)
        sv.amp[i] = cx_new(0.0, 0.0);
    sv.amp[0] = cx_new(1.0, 0.0);
    return sv;
}

static state_vec_t sv_apply_single(state_vec_t sv, cx_t u[2][2], int qubit) {
    state_vec_t out = sv;
    int step = 1 << qubit;
    for (int i = 0; i < sv.dim; i++) {
        if (i & step) continue;
        int j = i | step;
        cx_t a = sv.amp[i], b = sv.amp[j];
        out.amp[i] = cx_add(cx_mul(u[0][0], a), cx_mul(u[0][1], b));
        out.amp[j] = cx_add(cx_mul(u[1][0], a), cx_mul(u[1][1], b));
    }
    return out;
}

static state_vec_t sv_apply_controlled(state_vec_t sv, cx_t u[2][2],
                                        int ctrl, int target) {
    state_vec_t out = sv;
    int t_step = 1 << target, c_bit = 1 << ctrl;
    for (int i = 0; i < sv.dim; i++) {
        if ((i & c_bit) == 0 || (i & t_step)) continue;
        int j = i | t_step;
        cx_t a = sv.amp[i], b = sv.amp[j];
        out.amp[i] = cx_add(cx_mul(u[0][0], a), cx_mul(u[0][1], b));
        out.amp[j] = cx_add(cx_mul(u[1][0], a), cx_mul(u[1][1], b));
    }
    return out;
}

static double sv_prob_zero(state_vec_t sv, int qubit) {
    double p = 0.0;
    int bit = 1 << qubit;
    for (int i = 0; i < sv.dim; i++)
        if ((i & bit) == 0) p += cx_norm(sv.amp[i]);
    return p;
}

static state_vec_t sv_collapse(state_vec_t sv, int qubit, int outcome) {
    state_vec_t out = sv;
    int bit = 1 << qubit;
    double norm_sq = 0.0;
    for (int i = 0; i < sv.dim; i++) {
        int bit_val = (i & bit) ? 1 : 0;
        if (bit_val != outcome) out.amp[i] = cx_new(0.0, 0.0);
        else norm_sq += cx_norm(sv.amp[i]);
    }
    double inv = 1.0 / cx_abs(cx_new(norm_sq, 0.0));
    if (inv > EPSILON)
        for (int i = 0; i < sv.dim; i++)
            out.amp[i] = cx_scale(out.amp[i], inv);
    out.measured = 1;
    out.collapse_mask |= bit;
    return out;
}

/* ======================== GATE DEFINITIONS ======================== */
static void gate_hadamard(cx_t u[2][2]) {
    double s = 0.7071067811865;
    u[0][0] = cx_new(s, 0.0);  u[0][1] = cx_new(s, 0.0);
    u[1][0] = cx_new(s, 0.0);  u[1][1] = cx_new(-s, 0.0);
}

static void gate_pauli_x(cx_t u[2][2]) {
    u[0][0] = cx_new(0.0, 0.0); u[0][1] = cx_new(1.0, 0.0);
    u[1][0] = cx_new(1.0, 0.0); u[1][1] = cx_new(0.0, 0.0);
}

static void gate_pauli_y(cx_t u[2][2]) {
    u[0][0] = cx_new(0.0, 0.0); u[0][1] = cx_new(0.0, -1.0);
    u[1][0] = cx_new(0.0, 1.0); u[1][1] = cx_new(0.0, 0.0);
}

static void gate_pauli_z(cx_t u[2][2]) {
    u[0][0] = cx_new(1.0, 0.0);  u[0][1] = cx_new(0.0, 0.0);
    u[1][0] = cx_new(0.0, 0.0);  u[1][1] = cx_new(-1.0, 0.0);
}

static void gate_s(cx_t u[2][2]) {
    u[0][0] = cx_new(1.0, 0.0); u[0][1] = cx_new(0.0, 0.0);
    u[1][0] = cx_new(0.0, 0.0); u[1][1] = cx_new(0.0, 1.0);
}

static void gate_tgate(cx_t u[2][2]) {
    double s = 0.7071067811865;
    u[0][0] = cx_new(1.0, 0.0); u[0][1] = cx_new(0.0, 0.0);
    u[1][0] = cx_new(0.0, 0.0); u[1][1] = cx_new(s, s);
}

static void gate_rx(cx_t u[2][2], double theta) {
    double c = 1.0, s = 0.0, tc = 1.0, ts = theta * 0.5, a2 = theta * 0.5 * theta * 0.5;
    for (int k = 1; k <= 10; k++) {
        tc *= -a2 / ((2.0*k-1.0)*(2.0*k)); c += tc;
        ts *= -a2 / ((2.0*k)*(2.0*k+1.0)); s += ts;
    }
    u[0][0] = cx_new(c, 0.0);   u[0][1] = cx_new(0.0, -s);
    u[1][0] = cx_new(0.0, -s);  u[1][1] = cx_new(c, 0.0);
}

static void gate_ry(cx_t u[2][2], double theta) {
    double c = 1.0, s = 0.0, tc = 1.0, ts = theta * 0.5, a2 = theta * 0.5 * theta * 0.5;
    for (int k = 1; k <= 10; k++) {
        tc *= -a2 / ((2.0*k-1.0)*(2.0*k)); c += tc;
        ts *= -a2 / ((2.0*k)*(2.0*k+1.0)); s += ts;
    }
    u[0][0] = cx_new(c, 0.0);  u[0][1] = cx_new(-s, 0.0);
    u[1][0] = cx_new(s, 0.0);  u[1][1] = cx_new(c, 0.0);
}

static void gate_rz(cx_t u[2][2], double theta) {
    cx_t ep = cx_exp(-theta * 0.5);
    cx_t em = cx_exp(theta * 0.5);
    u[0][0] = ep; u[0][1] = cx_new(0.0, 0.0);
    u[1][0] = cx_new(0.0, 0.0); u[1][1] = em;
}

/* ======================== GATE APPLICATION ======================== */
static state_vec_t apply_gate(state_vec_t sv, gate_t g) {
    cx_t u[2][2];
    switch (g.type) {
    case GATE_PAULI_X:  gate_pauli_x(u);  return sv_apply_single(sv, u, g.target);
    case GATE_PAULI_Y:  gate_pauli_y(u);  return sv_apply_single(sv, u, g.target);
    case GATE_PAULI_Z:  gate_pauli_z(u);  return sv_apply_single(sv, u, g.target);
    case GATE_HADAMARD: gate_hadamard(u); return sv_apply_single(sv, u, g.target);
    case GATE_S:        gate_s(u);        return sv_apply_single(sv, u, g.target);
    case GATE_T:        gate_tgate(u);       return sv_apply_single(sv, u, g.target);
    case GATE_RX:       gate_rx(u, g.theta); return sv_apply_single(sv, u, g.target);
    case GATE_RY:       gate_ry(u, g.theta); return sv_apply_single(sv, u, g.target);
    case GATE_RZ:       gate_rz(u, g.theta); return sv_apply_single(sv, u, g.target);
    case GATE_PHASE: {
        cx_t ep = cx_exp(g.theta);
        u[0][0] = cx_new(1.0, 0.0); u[0][1] = cx_new(0.0, 0.0);
        u[1][0] = cx_new(0.0, 0.0); u[1][1] = ep;
        return sv_apply_single(sv, u, g.target);
    }
    case GATE_CNOT:
        gate_pauli_x(u); return sv_apply_controlled(sv, u, g.control, g.target);
    case GATE_CZ:
        gate_pauli_z(u); return sv_apply_controlled(sv, u, g.control, g.target);
    case GATE_SWAP: {
        /* SWAP = 3 CNOTs */
        cx_t xn[2][2]; gate_pauli_x(xn);
        state_vec_t s1 = sv_apply_controlled(sv, xn, g.target, g.control);
        state_vec_t s2 = sv_apply_controlled(s1, xn, g.control, g.target);
        return sv_apply_controlled(s2, xn, g.target, g.control);
    }
    case GATE_TOFFOLI: {
        /* Toffoli: ctrl, ctrl2 -> target using decomposition */
        cx_t h[2][2], tn[2][2], td[2][2], cxm[2][2];
        gate_hadamard(h); gate_tgate(td); gate_pauli_x(cxm);
        cx_t t_adj[2][2];
        t_adj[0][0] = cx_new(1.0, 0.0); t_adj[0][1] = cx_new(0.0, 0.0);
        t_adj[1][0] = cx_new(0.0, 0.0); t_adj[1][1] = cx_conj(td[1][1]);
        state_vec_t s = sv_apply_single(sv, h, g.target);
        s = sv_apply_controlled(s, cxm, g.control2, g.target);
        s = sv_apply_single(s, t_adj, g.target);
        s = sv_apply_controlled(s, cxm, g.control, g.target);
        s = sv_apply_single(s, td, g.target);
        s = sv_apply_controlled(s, cxm, g.control2, g.target);
        s = sv_apply_single(s, t_adj, g.target);
        s = sv_apply_controlled(s, cxm, g.control, g.target);
        s = sv_apply_single(s, td, g.target);
        s = sv_apply_single(s, h, g.target);
        return s;
    }
    case GATE_FREDKIN: {
        /* Fredkin: ctrl swaps target, target+1 via 3 CNOTs + Toffoli */
        cx_t xn[2][2]; gate_pauli_x(xn);
        state_vec_t s1 = sv_apply_controlled(sv, xn, g.target + 1, g.target);
        gate_t tof; tof.type = GATE_TOFFOLI;
        tof.control = g.control; tof.control2 = g.target;
        tof.target = g.target + 1; tof.theta = 0; tof.phi = 0; tof.lambda = 0;
        state_vec_t s2 = apply_gate(s1, tof);
        return sv_apply_controlled(s2, xn, g.target + 1, g.target);
    }
    default: return sv;
    }
}

/* ======================== CIRCUIT BUILDER ======================== */
static int circuit_create(const char *name, int n_qubits) {
    if (g_circ_cnt >= MAX_CIRCUITS || n_qubits > MAX_QUBITS) return -1;
    int id = g_circ_cnt++;
    circuit_t *c = &g_circuits[id];
    c->n_qubits = n_qubits;
    c->dim = 1 << n_qubits;
    c->gate_cnt = 0;
    c->n_meas = 0;
    c->state = CIRCUIT_EMPTY;
    c->shots = 0;
    for (int i = 0; i < MAX_DIM; i++) c->histogram[i] = 0;
    for (int i = 0; i < NAME_LEN && name[i]; i++) c->name[i] = name[i];
    c->name[NAME_LEN - 1] = '\0';
    return id;
}

static int circuit_add_gate(int cid, int type, int target, int ctrl, int ctrl2,
                             double theta) {
    if (cid < 0 || cid >= g_circ_cnt) return -1;
    circuit_t *c = &g_circuits[cid];
    if (c->gate_cnt >= MAX_GATES) return -1;
    gate_t g;
    g.type = type; g.target = target; g.control = ctrl;
    g.control2 = ctrl2; g.theta = theta; g.phi = 0.0; g.lambda = 0.0;
    c->gates[c->gate_cnt++] = g;
    c->state = CIRCUIT_BUILT;
    return c->gate_cnt - 1;
}

static void circuit_add_meas(int cid, int qubit) {
    circuit_t *c = &g_circuits[cid];
    if (c->n_meas < MAX_QUBITS) c->meas_qubits[c->n_meas++] = qubit;
}

static state_vec_t circuit_execute(int cid) {
    circuit_t *c = &g_circuits[cid];
    state_vec_t sv = sv_create(c->n_qubits);
    for (int i = 0; i < c->gate_cnt; i++)
        sv = apply_gate(sv, c->gates[i]);
    c->state = CIRCUIT_RUN;
    return sv;
}

/* ======================== MEASUREMENT ======================== */
static int measure_qubit(state_vec_t *sv, int qubit) {
    double p0 = sv_prob_zero(*sv, qubit);
    double r = rng_double();
    int outcome = (r < p0) ? 0 : 1;
    *sv = sv_collapse(*sv, qubit, outcome);
    return outcome;
}

static unsigned int circuit_sample(int cid, int n_shots) {
    circuit_t *c = &g_circuits[cid];
    c->shots = n_shots;
    for (int i = 0; i < MAX_DIM; i++) c->histogram[i] = 0;
    for (int s = 0; s < n_shots; s++) {
        state_vec_t sv = circuit_execute(cid);
        int result = 0;
        for (int q = 0; q < c->n_qubits; q++) {
            double p0 = sv_prob_zero(sv, q);
            int bit = (rng_double() < p0) ? 0 : 1;
            if (bit) result |= (1 << q);
            sv = sv_collapse(sv, q, bit);
        }
        c->histogram[result]++;
    }
    return 0;
}

/* ======================== DENSITY MATRIX ======================== */
static density_mat_t dm_from_state(state_vec_t sv) {
    density_mat_t dm;
    dm.n_qubits = sv.n_qubits;
    dm.dim = sv.dim;
    for (int i = 0; i < dm.dim; i++)
        for (int j = 0; j < dm.dim; j++)
            dm.mat[i * dm.dim + j] = cx_mul(sv.amp[i], cx_conj(sv.amp[j]));
    return dm;
}

static double dm_purity(density_mat_t dm) {
    double pur = 0.0;
    for (int i = 0; i < dm.dim; i++)
        for (int j = 0; j < dm.dim; j++)
            pur += cx_norm(dm.mat[i * dm.dim + j]);
    return pur;
}

static double dm_trace(density_mat_t dm) {
    double tr = 0.0;
    for (int i = 0; i < dm.dim; i++)
        tr += dm.mat[i * dm.dim + i].re;
    return tr;
}

/* ======================== NOISE CHANNELS ======================== */
static density_mat_t noise_depolarize(density_mat_t dm, double p, int qubit) {
    density_mat_t out = dm;
    int bit = 1 << qubit;
    double f = 1.0 - p;
    for (int i = 0; i < dm.dim; i++) {
        for (int j = 0; j < dm.dim; j++) {
            int ib = (i & bit) ? 1 : 0;
            int jb = (j & bit) ? 1 : 0;
            if (i != j) {
                out.mat[i * dm.dim + j] = cx_scale(dm.mat[i * dm.dim + j], f);
            } else if (ib != jb) {
                out.mat[i * dm.dim + j] = cx_new(0.0, 0.0);
            }
        }
    }
    double diag_contrib = p / (double)dm.dim;
    for (int i = 0; i < dm.dim; i++)
        out.mat[i * dm.dim + i] = cx_add(
            cx_scale(out.mat[i * dm.dim + i], f),
            cx_new(diag_contrib, 0.0));
    return out;
}

static density_mat_t noise_amplitude_damp(density_mat_t dm, double gamma, int qubit) {
    density_mat_t out = dm;
    int bit = 1 << qubit;
    double g = gamma;
    double sg = 1.0;
    for (int i = 0; i < 10; i++) sg *= (1.0 - g / (double)(i + 1));
    sg = 1.0 - g;
    for (int i = 0; i < dm.dim; i++) {
        for (int j = 0; j < dm.dim; j++) {
            int ib = (i & bit) ? 1 : 0;
            int jb = (j & bit) ? 1 : 0;
            double scale = 1.0;
            if (ib && jb) scale = sg;
            else if (ib || jb) scale = sg;
            out.mat[i * dm.dim + j] = cx_scale(dm.mat[i * dm.dim + j], scale);
        }
    }
    for (int i = 0; i < dm.dim; i++) {
        if (i & bit) continue;
        int ip = i | bit;
        double contrib = g * dm.mat[ip * dm.dim + ip].re;
        out.mat[i * dm.dim + i] = cx_add(out.mat[i * dm.dim + i],
                                          cx_new(contrib, 0.0));
    }
    return out;
}

static density_mat_t noise_phase_damp(density_mat_t dm, double gamma, int qubit) {
    density_mat_t out = dm;
    int bit = 1 << qubit;
    double f = 1.0 - gamma;
    for (int i = 0; i < dm.dim; i++) {
        for (int j = 0; j < dm.dim; j++) {
            int ib = (i & bit) ? 1 : 0;
            int jb = (j & bit) ? 1 : 0;
            if (ib != jb)
                out.mat[i * dm.dim + j] = cx_scale(dm.mat[i * dm.dim + j], f);
        }
    }
    return out;
}

static density_mat_t noise_thermal(density_mat_t dm, double t1, double t2,
                                    double temp, int qubit, double dt) {
    double gamma_1 = (t1 > EPSILON) ? dt / t1 : 0.0;
    double gamma_2 = (t2 > EPSILON) ? dt / t2 : 0.0;
    double n_th = (temp > EPSILON) ? temp / (temp + 1.0) : 0.0;
    density_mat_t r1 = noise_amplitude_damp(dm, gamma_1 * (1.0 - n_th), qubit);
    density_mat_t r2 = noise_phase_damp(r1, gamma_2 * 0.5, qubit);
    return r2;
}

/* ======================== BLOCH SPHERE ======================== */
static bloch_coord_t bloch_from_state(state_vec_t sv, int qubit) {
    bloch_coord_t b;
    int bit = 1 << qubit;
    cx_t rho00 = cx_new(0.0, 0.0), rho11 = cx_new(0.0, 0.0);
    cx_t rho01 = cx_new(0.0, 0.0);
    for (int i = 0; i < sv.dim; i++) {
        if (i & bit) continue;
        int ip = i | bit;
        rho00 = cx_add(rho00, cx_mul(sv.amp[i], cx_conj(sv.amp[i])));
        rho11 = cx_add(rho11, cx_mul(sv.amp[ip], cx_conj(sv.amp[ip])));
        rho01 = cx_add(rho01, cx_mul(sv.amp[i], cx_conj(sv.amp[ip])));
    }
    b.x = 2.0 * rho01.re;
    b.y = 2.0 * rho01.im;
    b.z = rho00.re - rho11.re;
    b.purity = b.x * b.x + b.y * b.y + b.z * b.z;
    return b;
}

/* ======================== VARIATIONAL CIRCUITS ======================== */
static variational_t var_create(int n_params) {
    variational_t v;
    v.n_params = n_params;
    v.energy = 0.0;
    v.iter = 0;
    for (int i = 0; i < n_params; i++) {
        v.params[i] = rng_double() * 2.0 * PI;
        v.grad[i] = 0.0;
    }
    return v;
}

static state_vec_t variational_apply(variational_t v, int n_qubits) {
    state_vec_t sv = sv_create(n_qubits);
    for (int q = 0; q < n_qubits; q++)
        sv = sv_apply_single(sv, (cx_t[2][2]){{{0.7071067811865,0.0},{0.7071067811865,0.0}},
                                                {{0.7071067811865,0.0},{-0.7071067811865,0.0}}}, q);
    int pi = 0;
    for (int layer = 0; layer < 2; layer++) {
        for (int q = 0; q < n_qubits && pi < v.n_params; q++, pi++) {
            cx_t u[2][2]; gate_rz(u, v.params[pi]);
            sv = sv_apply_single(sv, u, q);
        }
        for (int q = 0; q < n_qubits - 1; q++) {
            cx_t xn[2][2]; gate_pauli_x(xn);
            sv = sv_apply_controlled(sv, xn, q, q + 1);
        }
        for (int q = 0; q < n_qubits && pi < v.n_params; q++, pi++) {
            cx_t u[2][2]; gate_ry(u, v.params[pi]);
            sv = sv_apply_single(sv, u, q);
        }
    }
    return sv;
}

static double variational_energy(state_vec_t sv, int n_qubits) {
    double energy = 0.0;
    for (int q = 0; q < n_qubits; q++) {
        double p1 = 1.0 - sv_prob_zero(sv, q);
        energy += p1;
    }
    return energy;
}

static void variational_gradient(variational_t *v, int n_qubits, double lr) {
    double delta = 0.01;
    for (int i = 0; i < v->n_params; i++) {
        double orig = v->params[i];
        v->params[i] = orig + delta;
        state_vec_t sv_p = variational_apply(*v, n_qubits);
        double e_p = variational_energy(sv_p, n_qubits);
        v->params[i] = orig - delta;
        state_vec_t sv_m = variational_apply(*v, n_qubits);
        double e_m = variational_energy(sv_m, n_qubits);
        v->grad[i] = (e_p - e_m) / (2.0 * delta);
        v->params[i] = orig - lr * v->grad[i];
    }
    v->iter++;
}

/* ======================== OPTIMIZATION ======================== */
static int circuit_optimize(int cid) {
    circuit_t *c = &g_circuits[cid];
    int removed = 0;
    for (int i = 0; i < c->gate_cnt - 1; i++) {
        if (c->gates[i].type == GATE_PAULI_X &&
            c->gates[i + 1].type == GATE_PAULI_X &&
            c->gates[i].target == c->gates[i + 1].target) {
            for (int j = i; j < c->gate_cnt - 2; j++)
                c->gates[j] = c->gates[j + 2];
            c->gate_cnt -= 2;
            removed += 2;
            i--;
        }
    }
    for (int i = 0; i < c->gate_cnt - 1; i++) {
        if (c->gates[i].type == GATE_HADAMARD &&
            c->gates[i + 1].type == GATE_HADAMARD &&
            c->gates[i].target == c->gates[i + 1].target) {
            for (int j = i; j < c->gate_cnt - 2; j++)
                c->gates[j] = c->gates[j + 2];
            c->gate_cnt -= 2;
            removed += 2;
            i--;
        }
    }
    return removed;
}

/* ======================== PRINTING ======================== */
static void print_state_vector(state_vec_t sv) {
    print_str("  State vector ("); print_int(sv.n_qubits);
    print_str(" qubits, dim="); print_int(sv.dim); print_str("):\n");
    for (int i = 0; i < sv.dim; i++) {
        double mag = cx_abs(sv.amp[i]);
        if (mag > EPSILON) {
            print_str("    |"); print_int(i); print_str("> = ");
            print_fixed(sv.amp[i].re, 4); print_str(" + ");
            print_fixed(sv.amp[i].im, 4); print_str("i  (p=");
            print_fixed(cx_norm(sv.amp[i]), 4); print_str(")\n");
        }
    }
}

static void print_probs(state_vec_t sv) {
    print_str("  Probability distribution:\n");
    for (int i = 0; i < sv.dim; i++) {
        double p = cx_norm(sv.amp[i]);
        if (p > EPSILON) {
            print_str("    |"); print_int(i); print_str(">: ");
            print_fixed(p * 100.0, 2); print_str("% ");
            int bars = (int)(p * 40.0);
            for (int b = 0; b < bars; b++) print_str("#");
            print_str("\n");
        }
    }
}

static void print_bloch(bloch_coord_t bc, int qubit) {
    print_str("  Qubit "); print_int(qubit); print_str(" Bloch: x=");
    print_fixed(bc.x, 4); print_str(" y="); print_fixed(bc.y, 4);
    print_str(" z="); print_fixed(bc.z, 4);
    print_str(" pur="); print_fixed(bc.purity, 4); print_str("\n");
}

static void print_histogram(circuit_t *c) {
    print_str("  Measurement histogram ("); print_int(c->shots);
    print_str(" shots):\n");
    for (int i = 0; i < c->dim; i++) {
        if (c->histogram[i] > 0) {
            print_str("    |"); print_int(i); print_str(">: ");
            print_int(c->histogram[i]); print_str(" (");
            double pct = (double)c->histogram[i] / (double)c->shots * 100.0;
            print_fixed(pct, 1); print_str("%)\n");
        }
    }
}

/* ======================== MAIN DEMO ======================== */
int main(void) {
    print_str("=== Quantum Circuit Simulator v1.0 ===\n");

    /* --- Demo 1: Bell state (GHZ) --- */
    print_str("\n--- Demo 1: Bell State (EPR Pair) ---\n");
    int c1 = circuit_create("bell", 2);
    circuit_add_gate(c1, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c1, GATE_CNOT, 1, 0, -1, 0.0);
    state_vec_t sv1 = circuit_execute(c1);
    print_state_vector(sv1);
    print_probs(sv1);
    bloch_coord_t b1 = bloch_from_state(sv1, 0);
    print_bloch(b1, 0);

    /* --- Demo 2: GHZ state --- */
    print_str("\n--- Demo 2: 3-qubit GHZ State ---\n");
    int c2 = circuit_create("ghz3", 3);
    circuit_add_gate(c2, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c2, GATE_CNOT, 1, 0, -1, 0.0);
    circuit_add_gate(c2, GATE_CNOT, 2, 1, -1, 0.0);
    state_vec_t sv2 = circuit_execute(c2);
    print_state_vector(sv2);
    print_probs(sv2);

    /* --- Demo 3: Sampling --- */
    print_str("\n--- Demo 3: Bell State Sampling (1000 shots) ---\n");
    circuit_sample(c1, 1000);
    print_histogram(&g_circuits[c1]);

    /* --- Demo 4: Pauli gates --- */
    print_str("\n--- Demo 4: Pauli Gate Sequence (X->Y->Z) ---\n");
    int c4 = circuit_create("pauli_seq", 1);
    circuit_add_gate(c4, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c4, GATE_PAULI_Y, 0, -1, -1, 0.0);
    circuit_add_gate(c4, GATE_PAULI_Z, 0, -1, -1, 0.0);
    state_vec_t sv4 = circuit_execute(c4);
    print_state_vector(sv4);

    /* --- Demo 5: Parameterized rotations --- */
    print_str("\n--- Demo 5: Parameterized Rotation Gates ---\n");
    int c5 = circuit_create("rotations", 2);
    circuit_add_gate(c5, GATE_RX, 0, -1, -1, PI / 4.0);
    circuit_add_gate(c5, GATE_RY, 1, -1, -1, PI / 3.0);
    circuit_add_gate(c5, GATE_RZ, 0, -1, -1, PI / 6.0);
    circuit_add_gate(c5, GATE_CNOT, 1, 0, -1, 0.0);
    state_vec_t sv5 = circuit_execute(c5);
    print_state_vector(sv5);
    print_probs(sv5);

    /* --- Demo 6: S and T gates --- */
    print_str("\n--- Demo 6: S/T Gate Phase Operations ---\n");
    int c6 = circuit_create("phase_gates", 1);
    circuit_add_gate(c6, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c6, GATE_S, 0, -1, -1, 0.0);
    circuit_add_gate(c6, GATE_T, 0, -1, -1, 0.0);
    state_vec_t sv6 = circuit_execute(c6);
    print_state_vector(sv6);

    /* --- Demo 7: SWAP gate --- */
    print_str("\n--- Demo 7: SWAP Gate ---\n");
    int c7 = circuit_create("swap", 2);
    circuit_add_gate(c7, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c7, GATE_SWAP, 1, 0, -1, 0.0);
    state_vec_t sv7 = circuit_execute(c7);
    print_state_vector(sv7);
    print_str("  (X on q0, SWAP -> should see |10>)\n");

    /* --- Demo 8: Toffoli gate --- */
    print_str("\n--- Demo 8: Toffoli (CCX) Gate ---\n");
    int c8 = circuit_create("toffoli", 3);
    circuit_add_gate(c8, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c8, GATE_PAULI_X, 1, -1, -1, 0.0);
    circuit_add_gate(c8, GATE_TOFFOLI, 2, 0, 1, 0.0);
    state_vec_t sv8 = circuit_execute(c8);
    print_state_vector(sv8);
    print_str("  (X on q0,q1 + Toffoli -> |111>)\n");

    /* --- Demo 9: Density matrix and noise --- */
    print_str("\n--- Demo 9: Density Matrix and Noise ---\n");
    int c9 = circuit_create("noisy", 2);
    circuit_add_gate(c9, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c9, GATE_CNOT, 1, 0, -1, 0.0);
    state_vec_t sv9 = circuit_execute(c9);
    density_mat_t dm9 = dm_from_state(sv9);
    print_str("  Pure state purity: "); print_fixed(dm_purity(dm9), 4); print_str("\n");
    print_str("  Trace: "); print_fixed(dm_trace(dm9), 4); print_str("\n");
    density_mat_t dm_dep = noise_depolarize(dm9, 0.3, 0);
    print_str("  After depolarizing (p=0.3): purity=");
    print_fixed(dm_purity(dm_dep), 4); print_str("\n");
    density_mat_t dm_ad = noise_amplitude_damp(dm9, 0.2, 0);
    print_str("  After amplitude damping (g=0.2): purity=");
    print_fixed(dm_purity(dm_ad), 4); print_str("\n");
    density_mat_t dm_pd = noise_phase_damp(dm9, 0.4, 0);
    print_str("  After phase damping (g=0.4): purity=");
    print_fixed(dm_purity(dm_pd), 4); print_str("\n");
    density_mat_t dm_th = noise_thermal(dm9, 50.0, 30.0, 0.1, 0, 10.0);
    print_str("  After thermal relaxation: purity=");
    print_fixed(dm_purity(dm_th), 4); print_str("\n");

    /* --- Demo 10: Variational circuit --- */
    print_str("\n--- Demo 10: Variational Quantum Circuit (VQE-like) ---\n");
    variational_t var = var_create(8);
    print_str("  Initial parameters:\n    ");
    for (int i = 0; i < var.n_params; i++) {
        print_fixed(var.params[i], 3); print_str(" ");
    }
    print_str("\n");
    state_vec_t sv_v = variational_apply(var, 3);
    double e0 = variational_energy(sv_v, 3);
    print_str("  Initial energy: "); print_fixed(e0, 6); print_str("\n");
    for (int epoch = 0; epoch < 5; epoch++) {
        variational_gradient(&var, 3, 0.3);
        state_vec_t sv_i = variational_apply(var, 3);
        double ei = variational_energy(sv_i, 3);
        print_str("  Epoch "); print_int(epoch + 1);
        print_str(": energy="); print_fixed(ei, 6);
        print_str(" grad[0]="); print_fixed(var.grad[0], 4); print_str("\n");
    }

    /* --- Demo 11: Circuit optimization --- */
    print_str("\n--- Demo 11: Circuit Optimization ---\n");
    int c11 = circuit_create("optimize", 2);
    circuit_add_gate(c11, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c11, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c11, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c11, GATE_HADAMARD, 1, -1, -1, 0.0);
    circuit_add_gate(c11, GATE_CNOT, 1, 0, -1, 0.0);
    circuit_add_gate(c11, GATE_HADAMARD, 1, -1, -1, 0.0);
    circuit_add_gate(c11, GATE_HADAMARD, 1, -1, -1, 0.0);
    print_str("  Before optimization: "); print_int(g_circuits[c11].gate_cnt);
    print_str(" gates\n");
    int removed = circuit_optimize(c11);
    print_str("  Removed "); print_int(removed); print_str(" redundant gates\n");
    print_str("  After optimization: "); print_int(g_circuits[c11].gate_cnt);
    print_str(" gates\n");

    /* --- Demo 12: Partial measurement --- */
    print_str("\n--- Demo 12: Partial Measurement and Collapse ---\n");
    int c12 = circuit_create("partial_meas", 3);
    circuit_add_gate(c12, GATE_HADAMARD, 0, -1, -1, 0.0);
    circuit_add_gate(c12, GATE_HADAMARD, 1, -1, -1, 0.0);
    circuit_add_gate(c12, GATE_HADAMARD, 2, -1, -1, 0.0);
    circuit_add_gate(c12, GATE_CNOT, 1, 0, -1, 0.0);
    state_vec_t sv12 = circuit_execute(c12);
    print_str("  Before measurement:\n");
    print_probs(sv12);
    int m0 = measure_qubit(&sv12, 0);
    print_str("  Measured q0 = "); print_int(m0); print_str("\n");
    print_str("  After collapse:\n");
    print_probs(sv12);

    /* --- Demo 13: Fredkin gate --- */
    print_str("\n--- Demo 13: Fredkin (CSWAP) Gate ---\n");
    int c13 = circuit_create("fredkin", 3);
    circuit_add_gate(c13, GATE_PAULI_X, 0, -1, -1, 0.0);
    circuit_add_gate(c13, GATE_PAULI_X, 1, -1, -1, 0.0);
    circuit_add_gate(c13, GATE_FREDKIN, 1, 0, -1, 0.0);
    state_vec_t sv13 = circuit_execute(c13);
    print_state_vector(sv13);
    print_str("  (X on q0,q1 + Fredkin ctrl=q0 swap q1,q2 -> |110>)\n");

    /* --- Summary --- */
    print_str("\n=== Circuit Summary ===\n");
    for (int i = 0; i < g_circ_cnt; i++) {
        print_str("  ["); print_int(i); print_str("] ");
        print_str(g_circuits[i].name);
        print_str(": q="); print_int(g_circuits[i].n_qubits);
        print_str(" gates="); print_int(g_circuits[i].gate_cnt);
        print_str(" state="); print_int(g_circuits[i].state);
        print_str(" shots="); print_int(g_circuits[i].shots);
        print_str("\n");
    }
    print_str("\n=== Quantum Circuit Simulator Complete ===\n");
    return 0;
}
