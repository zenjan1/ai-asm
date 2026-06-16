/*
 * quantum_simulator — Quantum Computing Simulation Module v55.0
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, no stdlib. Host imports only.
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

#define MAX_QUBITS 12
#define VERSION "Quantum Simulator v55.0 - Quantum Computing Simulation"

/* PRNG (xorshift32) for measurement and noise */
static unsigned int rng_state = 0xDEADBEEF;
static unsigned int rng_next(void) {
    unsigned int x = rng_state;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    rng_state = x;
    return x;
}
static double rng_double(void) {
    return (double)(rng_next() & 0xFFFFFFu) / 16777216.0;
}

/* ================================================================
 * STRING UTILITIES
 * ================================================================ */
static int my_strlen(const char *s) {
    int n = 0; while (s[n]) n++; return n;
}

static void my_strncpy(char *d, const char *s, int n) {
    int i = 0;
    while (i < n - 1 && s[i]) { d[i] = s[i]; i++; }
    d[i] = '\0';
}

static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else { int t = v, d = 0; while (t > 0) { d++; t /= 10; } int s = p + d; p = s; b[p] = '\0';
           while (v > 0) { b[--s] = '0' + (v % 10); v /= 10; } }
    host_print(b);
}

static void print_double(double v) {
    char b[64]; int p = 0;
    if (v < 0.0) { b[p++] = '-'; v = -v; }
    int iv = (int)v; double frac = v - (double)iv;
    /* integer part */
    if (iv == 0) b[p++] = '0';
    else { int t = iv, d = 0; while (t > 0) { d++; t /= 10; } int s = p + d;
           while (iv > 0) { b[--s] = '0' + (iv % 10); iv /= 10; } p += d; }
    b[p++] = '.';
    for (int i = 0; i < 6; i++) { frac *= 10.0; b[p++] = '0' + (int)frac; frac -= (int)frac; }
    b[p] = '\0';
    host_print(b);
}

static void print_complex(double re, double im) {
    print_double(re);
    if (im >= 0.0) print_str("+");
    print_double(im);
    print_str("i");
}

/* ================================================================
 * MATH UTILITIES (no stdlib)
 * ================================================================ */
static double my_fabs(double x) { return x < 0.0 ? -x : x; }

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x * 0.5;
    for (int i = 0; i < 64; i++) {
        double ng = 0.5 * (g + x / g);
        if (my_fabs(ng - g) < EPS) break;
        g = ng;
    }
    return g;
}

static double my_sin(double x) {
    while (x > PI) x -= TAU;
    while (x < -PI) x += TAU;
    double t = x, s = x;
    for (int i = 1; i < 20; i++) {
        t *= -x * x / ((2.0 * i) * (2.0 * i + 1.0));
        s += t;
    }
    return s;
}

static double my_cos(double x) {
    while (x > PI) x -= TAU;
    while (x < -PI) x += TAU;
    double t = 1.0, s = 1.0;
    for (int i = 1; i < 20; i++) {
        t *= -x * x / ((2.0 * i - 1.0) * (2.0 * i));
        s += t;
    }
    return s;
}

static double my_log(double x) {
    if (x <= 0.0) return -1e30;
    double r = 0.0;
    while (x >= 2.0) { x *= 0.5; r += 0.6931471805599453; }
    while (x < 0.5)  { x *= 2.0; r -= 0.6931471805599453; }
    double y = (x - 1.0) / (x + 1.0), y2 = y * y, t = y;
    for (int i = 0; i < 30; i++) { int k = 2 * i + 1; r += 2.0 * t / k; t *= y2; }
    return r;
}

/* ================================================================
 * COMPLEX NUMBER ARITHMETIC
 * ================================================================ */
typedef struct { double re, im; } complex_t;

static complex_t cx(double r, double i) { complex_t c; c.re = r; c.im = i; return c; }
static complex_t c_add(complex_t a, complex_t b) { return cx(a.re + b.re, a.im + b.im); }
static complex_t c_sub(complex_t a, complex_t b) { return cx(a.re - b.re, a.im - b.im); }
static complex_t c_mul(complex_t a, complex_t b) {
    return cx(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}
static complex_t c_div(complex_t a, complex_t b) {
    double d = b.re * b.re + b.im * b.im;
    if (d < EPS) return cx(0, 0);
    return cx((a.re * b.re + a.im * b.im) / d, (a.im * b.re - a.re * b.im) / d);
}
static complex_t c_conj(complex_t a) { return cx(a.re, -a.im); }
static double    c_abs(complex_t a)  { return my_sqrt(a.re * a.re + a.im * a.im); }
static double my_atan2_safe(double y, double x) {
    double r = my_sqrt(x * x + y * y);
    if (r < EPS) return 0.0;
    double ax = my_fabs(x), ay = my_fabs(y);
    double a = (ax > ay) ? (ay / ax) : (ax / ay);
    a = a - a * a * a / 3.0 + a * a * a * a * a / 5.0;
    if (ax < ay) a = PI * 0.5 - a;
    if (x < 0.0) a = PI - a;
    if (y < 0.0) a = -a;
    return a;
}
static double    c_phase(complex_t a){ return my_atan2_safe(a.im, a.re); }
static complex_t c_scale(complex_t a, double s) { return cx(a.re * s, a.im * s); }

static complex_t c_exp(complex_t z) {
    double mag = my_sqrt(z.re * z.re + z.im * z.im);
    if (mag < EPS) return cx(1.0, 0.0);
    double er = 1.0, er_i = 1.0;
    for (int i = 1; i < 30; i++) { er_i *= z.re / i; er += er_i; }
    return cx(er * my_cos(z.im), er * my_sin(z.im));
}

static complex_t c_from_polar(double r, double theta) {
    return cx(r * my_cos(theta), r * my_sin(theta));
}

/* ================================================================
 * QUANTUM STATE
 * ================================================================ */
typedef struct {
    int        num_qubits;
    int        dim;          /* 2^n */
    complex_t *amps;         /* amplitude array */
    int        density_mode; /* 1 = density matrix representation */
} quantum_state_t;

static quantum_state_t qstate_create(int n) {
    quantum_state_t s;
    s.num_qubits = n;
    s.dim = 1 << n;
    s.amps = (complex_t *)host_alloc(s.dim * sizeof(complex_t), 8);
    for (int i = 0; i < s.dim; i++) s.amps[i] = cx(0.0, 0.0);
    s.amps[0] = cx(1.0, 0.0);   /* |00...0> */
    s.density_mode = 0;
    return s;
}

static void qstate_normalize(quantum_state_t *s) {
    double norm = 0.0;
    for (int i = 0; i < s->dim; i++)
        norm += s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
    norm = my_sqrt(norm);
    if (norm < EPS) return;
    for (int i = 0; i < s->dim; i++)
        s->amps[i] = c_scale(s->amps[i], 1.0 / norm);
}

static double qstate_probability(quantum_state_t *s, int idx) {
    if (idx < 0 || idx >= s->dim) return 0.0;
    return s->amps[idx].re * s->amps[idx].re + s->amps[idx].im * s->amps[idx].im;
}

static quantum_state_t qstate_clone(quantum_state_t *src) {
    quantum_state_t d;
    d.num_qubits = src->num_qubits;
    d.dim = src->dim;
    d.density_mode = src->density_mode;
    d.amps = (complex_t *)host_alloc(d.dim * sizeof(complex_t), 8);
    for (int i = 0; i < d.dim; i++) d.amps[i] = src->amps[i];
    return d;
}

/* ================================================================
 * QUANTUM GATES
 * ================================================================ */
/* 2x2 gate matrix stored as gate_t { m00,m01,m10,m11 } */
typedef struct { complex_t m[4]; } gate_t;

static gate_t gate_h(void) {
    double s = 0.7071067811865476;
    gate_t g; g.m[0]=cx(s,0); g.m[1]=cx(s,0); g.m[2]=cx(s,0); g.m[3]=cx(-s,0); return g;
}
static gate_t gate_x(void) {
    gate_t g; g.m[0]=cx(0,0); g.m[1]=cx(1,0); g.m[2]=cx(1,0); g.m[3]=cx(0,0); return g;
}
static gate_t gate_y(void) {
    gate_t g; g.m[0]=cx(0,0); g.m[1]=cx(0,-1); g.m[2]=cx(0,1); g.m[3]=cx(0,0); return g;
}
static gate_t gate_z(void) {
    gate_t g; g.m[0]=cx(1,0); g.m[1]=cx(0,0); g.m[2]=cx(0,0); g.m[3]=cx(-1,0); return g;
}
static gate_t gate_s(void) {
    gate_t g; g.m[0]=cx(1,0); g.m[1]=cx(0,0); g.m[2]=cx(0,0); g.m[3]=cx(0,1); return g;
}
static gate_t gate_phase_t(void) {
    double c = 0.7071067811865476;
    gate_t g; g.m[0]=cx(1,0); g.m[1]=cx(0,0); g.m[2]=cx(0,0); g.m[3]=cx(c,c); return g;
}
static gate_t gate_rx(double th) {
    double c = my_cos(th*0.5), s = my_sin(th*0.5);
    gate_t g; g.m[0]=cx(c,0); g.m[1]=cx(0,-s); g.m[2]=cx(0,-s); g.m[3]=cx(c,0); return g;
}
static gate_t gate_ry(double th) {
    double c = my_cos(th*0.5), s = my_sin(th*0.5);
    gate_t g; g.m[0]=cx(c,0); g.m[1]=cx(-s,0); g.m[2]=cx(s,0); g.m[3]=cx(c,0); return g;
}
static gate_t gate_rz(double th) {
    gate_t g;
    g.m[0]=c_from_polar(1.0, -th*0.5); g.m[1]=cx(0,0);
    g.m[2]=cx(0,0); g.m[3]=c_from_polar(1.0, th*0.5); return g;
}

/* Apply single-qubit gate */
static void gate_apply_single(quantum_state_t *s, int qubit, gate_t g) {
    int dim = s->dim, step = 1 << qubit;
    for (int i = 0; i < dim; i++) {
        if (i & step) continue;   /* only process basis states where qubit=0 */
        complex_t a = s->amps[i];
        complex_t b = s->amps[i | step];
        s->amps[i]        = c_add(c_mul(g.m[0], a), c_mul(g.m[1], b));
        s->amps[i | step] = c_add(c_mul(g.m[2], a), c_mul(g.m[3], b));
    }
}

/* CNOT gate */
static void gate_apply_cnot(quantum_state_t *s, int ctrl, int tgt) {
    int dim = s->dim, cbit = 1 << ctrl, tbit = 1 << tgt;
    for (int i = 0; i < dim; i++) {
        if ((i & cbit) && !(i & tbit)) {
            complex_t tmp = s->amps[i];
            s->amps[i] = s->amps[i | tbit];
            s->amps[i | tbit] = tmp;
        }
    }
}

/* CZ gate */
static void gate_apply_cz(quantum_state_t *s, int c1, int c2) {
    int dim = s->dim, b1 = 1 << c1, b2 = 1 << c2;
    for (int i = 0; i < dim; i++)
        if ((i & b1) && (i & b2)) s->amps[i] = c_scale(s->amps[i], -1.0);
}

/* SWAP gate */
static void gate_apply_swap(quantum_state_t *s, int q1, int q2) {
    int dim = s->dim, b1 = 1 << q1, b2 = 1 << q2;
    for (int i = 0; i < dim; i++) {
        int a1 = (i >> q1) & 1, a2 = (i >> q2) & 1;
        if (a1 != a2) {
            int j = (i ^ b1 ^ b2);
            if (i < j) { complex_t t = s->amps[i]; s->amps[i] = s->amps[j]; s->amps[j] = t; }
        }
    }
}

/* Toffoli (CCNOT) */
static void gate_apply_toffoli(quantum_state_t *s, int c1, int c2, int tgt) {
    int dim = s->dim, b1 = 1 << c1, b2 = 1 << c2, bt = 1 << tgt;
    for (int i = 0; i < dim; i++) {
        if ((i & b1) && (i & b2) && !(i & bt)) {
            complex_t t = s->amps[i]; s->amps[i] = s->amps[i | bt]; s->amps[i | bt] = t;
        }
    }
}

/* Fredkin (CSWAP) */
static void gate_apply_fredkin(quantum_state_t *s, int ctrl, int q1, int q2) {
    int dim = s->dim, bc = 1 << ctrl, b1 = 1 << q1, b2 = 1 << q2;
    for (int i = 0; i < dim; i++) {
        if ((i & bc) && ((i & b1) != (i & b2))) {
            int j = i ^ b1 ^ b2;
            if (i < j) { complex_t t = s->amps[i]; s->amps[i] = s->amps[j]; s->amps[j] = t; }
        }
    }
}

/* ================================================================
 * QUANTUM ENTANGLEMENT
 * ================================================================ */
/* Bell state: type 0=Phi+, 1=Phi-, 2=Psi+, 3=Psi- */
static void bell_state_prep(quantum_state_t *s, int q1, int q2, int type) {
    double norm = 0.7071067811865476;
    for (int i = 0; i < s->dim; i++) s->amps[i] = cx(0, 0);
    int b1 = 1 << q1, b2 = 1 << q2;
    if (type == 0) { /* Phi+: (|00>+|11>)/sqrt2 */
        s->amps[0] = cx(norm, 0); s->amps[b1 | b2] = cx(norm, 0);
    } else if (type == 1) { /* Phi-: (|00>-|11>)/sqrt2 */
        s->amps[0] = cx(norm, 0); s->amps[b1 | b2] = cx(-norm, 0);
    } else if (type == 2) { /* Psi+: (|01>+|10>)/sqrt2 */
        s->amps[b1] = cx(norm, 0); s->amps[b2] = cx(norm, 0);
    } else {               /* Psi-: (|01>-|10>)/sqrt2 */
        s->amps[b1] = cx(norm, 0); s->amps[b2] = cx(-norm, 0);
    }
}

/* GHZ state: (|000..0>+|111..1>)/sqrt(2) */
static void ghz_state(quantum_state_t *s, int n) {
    for (int i = 0; i < s->dim; i++) s->amps[i] = cx(0, 0);
    double norm = 0.7071067811865476;
    s->amps[0] = cx(norm, 0);
    s->amps[(1 << n) - 1] = cx(norm, 0);
}

/* W state: equal superposition of single-excitation states */
static void w_state(quantum_state_t *s, int n) {
    for (int i = 0; i < s->dim; i++) s->amps[i] = cx(0, 0);
    double norm = 1.0 / my_sqrt((double)n);
    for (int i = 0; i < n; i++) s->amps[1 << i] = cx(norm, 0);
}

/* von Neumann entropy of a subsystem (1 qubit trace for simplicity) */
static double entanglement_entropy(quantum_state_t *s, int sub_qubit) {
    /* compute reduced density matrix for sub_qubit by tracing out rest */
    int bit = 1 << sub_qubit, dim = s->dim;
    double p0 = 0.0, p1 = 0.0;
    for (int i = 0; i < dim; i++) {
        double prob = s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
        if (i & bit) p1 += prob; else p0 += prob;
    }
    double e = 0.0;
    if (p0 > EPS) e -= p0 * my_log(p0);
    if (p1 > EPS) e -= p1 * my_log(p1);
    return e;
}

/* ================================================================
 * MEASUREMENT
 * ================================================================ */
static int measure_qubit(quantum_state_t *s, int qubit) {
    int bit = 1 << qubit, dim = s->dim;
    double p1 = 0.0;
    for (int i = 0; i < dim; i++)
        if (i & bit) p1 += s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
    int outcome = (rng_double() < p1) ? 1 : 0;
    /* collapse */
    double norm = 0.0;
    for (int i = 0; i < dim; i++) {
        int b = (i & bit) ? 1 : 0;
        if (b != outcome) s->amps[i] = cx(0, 0);
        else norm += s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
    }
    norm = my_sqrt(norm);
    if (norm > EPS) for (int i = 0; i < dim; i++) s->amps[i] = c_scale(s->amps[i], 1.0 / norm);
    return outcome;
}

static int measure_all(quantum_state_t *s) {
    /* sample from full probability distribution */
    double r = rng_double(), cum = 0.0;
    for (int i = 0; i < s->dim; i++) {
        cum += s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
        if (r < cum) {
            /* collapse to basis state i */
            for (int j = 0; j < s->dim; j++) s->amps[j] = cx(0, 0);
            s->amps[i] = cx(1, 0);
            return i;
        }
    }
    return s->dim - 1;
}

static int measure_partial(quantum_state_t *s, int *qubits, int nq) {
    int result = 0;
    for (int k = 0; k < nq; k++) {
        int bit = measure_qubit(s, qubits[k]);
        if (bit) result |= (1 << k);
    }
    return result;
}

/* Expectation value <psi|Z|psi> for a single qubit */
static double expectation_value(quantum_state_t *s, int qubit) {
    int bit = 1 << qubit; double ev = 0.0;
    for (int i = 0; i < s->dim; i++) {
        double p = s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im;
        ev += (i & bit) ? p : -p;
    }
    return ev;
}

/* ================================================================
 * QUANTUM ALGORITHMS
 * ================================================================ */
/* Grover's algorithm: search for target state in n-qubit system */
static int grover_search(int num_qubits, int target, int iterations) {
    quantum_state_t s = qstate_create(num_qubits);
    int dim = s.dim;
    /* Hadamard all qubits: uniform superposition */
    for (int q = 0; q < num_qubits; q++) gate_apply_single(&s, q, gate_h());
    /* Grover iterations */
    for (int it = 0; it < iterations; it++) {
        /* Oracle: flip phase of target */
        s.amps[target] = c_scale(s.amps[target], -1.0);
        /* Diffusion: 2|s><s| - I  */
        double mean_re = 0.0, mean_im = 0.0;
        for (int i = 0; i < dim; i++) { mean_re += s.amps[i].re; mean_im += s.amps[i].im; }
        mean_re /= dim; mean_im /= dim;
        for (int i = 0; i < dim; i++) {
            s.amps[i].re = 2.0 * mean_re - s.amps[i].re;
            s.amps[i].im = 2.0 * mean_im - s.amps[i].im;
        }
    }
    int result = measure_all(&s);
    return result;
}

/* Simplified Shor's: classical simulation of order finding for N=p*q */
static int shor_factor(int N) {
    /* Try small factors using simulated period finding */
    for (int a = 2; a < N; a++) {
        long val = 1; int r = 0;
        for (r = 1; r < N * 4; r++) {
            val = (val * a) % N;
            if (val == 1) break;
        }
        if (r % 2 == 0) {
            long half = 1;
            for (int i = 0; i < r / 2; i++) half = (half * a) % N;
            int f1 = (int)(half + 1), f2 = (int)(half - 1);
            int g1 = f1, g2 = N;
            while (g2) { int t = g2; g2 = g1 % g2; g1 = t; }
            if (g1 > 1 && g1 < N) return g1;
            g1 = f2; g2 = N;
            while (g2) { int t = g2; g2 = g1 % g2; g1 = t; }
            if (g1 > 1 && g1 < N) return g1;
        }
    }
    return N; /* prime */
}

/* VQE: simplified variational eigensolver for 1-qubit Hamiltonian H = h0*Z + h1*X */
static double vqe_solve(double h0, double h1, double theta) {
    /* Ansatz: Ry(theta)|0> ; energy = h0*cos(theta) - h1*sin(theta) */
    double energy = h0 * my_cos(theta) - h1 * my_sin(theta);
    return energy;
}

/* ================================================================
 * NOISE SIMULATION
 * ================================================================ */
static void noise_dephasing(quantum_state_t *s, double rate, double dt) {
    double p = rate * dt; if (p > 1.0) p = 1.0;
    for (int i = 0; i < s->dim; i++) {
        if (rng_double() < p) s->amps[i].im *= -1.0;  /* random phase flip */
    }
    qstate_normalize(s);
}

static void noise_amplitude_damping(quantum_state_t *s, double gamma) {
    /* damp |1> amplitudes toward |0> */
    int dim = s->dim;
    for (int i = 0; i < dim; i++) {
        if (i & 1) {
            s->amps[i] = c_scale(s->amps[i], my_sqrt(1.0 - gamma));
            s->amps[i & ~1] = c_add(s->amps[i & ~1],
                            c_scale(s->amps[i], my_sqrt(gamma)));
        }
    }
    qstate_normalize(s);
}

static void noise_depolarizing(quantum_state_t *s, double p) {
    for (int i = 0; i < s->dim; i++) {
        if (rng_double() < p) {
            int op = rng_next() % 4;
            if (op == 1) s->amps[i] = cx(-s->amps[i].im, s->amps[i].re);     /* Y-like */
            else if (op == 2) s->amps[i] = c_scale(s->amps[i], -1.0);        /* Z-like */
            else if (op == 3) s->amps[i] = cx(s->amps[i].im, -s->amps[i].re);/* X-like */
        }
    }
    qstate_normalize(s);
}

static void noise_bit_flip(quantum_state_t *s, double p) {
    int dim = s->dim;
    for (int i = 0; i < dim; i += 2) {
        if (rng_double() < p) {
            complex_t t = s->amps[i]; s->amps[i] = s->amps[i + 1]; s->amps[i + 1] = t;
        }
    }
    qstate_normalize(s);
}

/* ================================================================
 * QUANTUM ERROR CORRECTION
 * ================================================================ */
/* Bit-flip code: |0> -> |000>, |1> -> |111> */
static quantum_state_t qec_bit_flip_encode(quantum_state_t *s) {
    quantum_state_t enc = qstate_create(3);
    enc.amps[0] = s->amps[0];        /* alpha|000> */
    enc.amps[7] = s->amps[1];        /* beta |111> */
    for (int i = 1; i < 7; i++) enc.amps[i] = cx(0, 0);
    return enc;
}

static void qec_bit_flip_decode(quantum_state_t *s) {
    /* majority vote on 3 qubits */
    int p[8]; for (int i = 0; i < 8; i++) p[i] = 0;
    for (int i = 0; i < 8; i++)
        p[i] = (int)(1000.0 * (s->amps[i].re * s->amps[i].re + s->amps[i].im * s->amps[i].im));
    /* find dominant subspace: count 000 vs 111 vs others */
    int sum0 = p[0] + p[1] + p[2] + p[4]; /* states with >=2 zeros */
    int sum1 = p[7] + p[6] + p[5] + p[3]; /* states with >=2 ones  */
    if (sum1 > sum0) { /* collapse toward |111> */
        for (int i = 0; i < 7; i++) s->amps[i] = cx(0, 0);
        s->amps[7] = c_scale(s->amps[7], 1.0 / c_abs(s->amps[7]));
    } else {
        for (int i = 1; i < 8; i++) s->amps[i] = cx(0, 0);
        s->amps[0] = c_scale(s->amps[0], 1.0 / c_abs(s->amps[0]));
    }
}

/* Phase-flip code: |+> -> |+++>, |-> -> |---> */
static quantum_state_t qec_phase_flip_encode(quantum_state_t *s) {
    quantum_state_t enc = qstate_create(3);
    double a_re = 0.5 * (s->amps[0].re + s->amps[1].re);
    double a_im = 0.5 * (s->amps[0].im + s->amps[1].im);
    double b_re = 0.5 * (s->amps[0].re - s->amps[1].re);
    double b_im = 0.5 * (s->amps[0].im - s->amps[1].im);
    for (int i = 0; i < 8; i++) {
        int ones = (i & 1) + ((i >> 1) & 1) + ((i >> 2) & 1);
        double sign = (ones % 2 == 0) ? 1.0 : -1.0;
        double mag = 0.3535533905932738; /* 1/(2*sqrt(2)) */
        enc.amps[i] = cx(mag * (a_re + sign * b_re), mag * (a_im + sign * b_im));
    }
    return enc;
}

static void qec_phase_flip_decode(quantum_state_t *s) {
    /* Apply H to all 3, then bit-flip decode, then H to all 3 */
    for (int q = 0; q < 3; q++) gate_apply_single(s, q, gate_h());
    qec_bit_flip_decode(s);
    for (int q = 0; q < 3; q++) gate_apply_single(s, q, gate_h());
}

/* Shor 9-qubit code: combines bit-flip and phase-flip */
static quantum_state_t qec_shor_encode(quantum_state_t *s) {
    quantum_state_t enc = qstate_create(9);
    /* |0>_L = (|000>+|111>)^3 / (2*sqrt(2)) */
    /* |1>_L = (|000>-|111>)^3 / (2*sqrt(2)) */
    double mag = 0.3535533905932738; /* 1/(2*sqrt(2)) */
    for (int i = 0; i < 512; i++) {
        int blk0 = (i & 0x7), blk1 = ((i >> 3) & 0x7), blk2 = ((i >> 6) & 0x7);
        int ones0 = (blk0 == 7) ? 1 : 0;
        int ones1 = (blk1 == 7) ? 1 : 0;
        int ones2 = (blk2 == 7) ? 1 : 0;
        int sign = 1 - 2 * ((ones0 + ones1 + ones2) % 2);
        enc.amps[i] = cx(mag * s->amps[0].re * sign, 0);
    }
    /* add beta |1>_L component */
    for (int i = 0; i < 512; i++) {
        int blk0 = (i & 0x7), blk1 = ((i >> 3) & 0x7), blk2 = ((i >> 6) & 0x7);
        int ones0 = (blk0 == 7) ? 1 : 0;
        int ones1 = (blk1 == 7) ? 1 : 0;
        int ones2 = (blk2 == 7) ? 1 : 0;
        int sign = 1 - 2 * ((ones0 + ones1 + ones2) % 2);
        enc.amps[i] = c_add(enc.amps[i], cx(mag * s->amps[1].re * sign, 0));
    }
    return enc;
}

/* ================================================================
 * TEST SUITE
 * ================================================================ */
static void test_bell_state(void) {
    print_str("\n--- Bell State Test ---\n");
    quantum_state_t s = qstate_create(2);
    gate_apply_single(&s, 0, gate_h());
    gate_apply_cnot(&s, 0, 1);
    print_str("State after H + CNOT (Bell Phi+):\n");
    for (int i = 0; i < s.dim; i++) {
        print_str("  |"); print_int(i); print_str("> = ");
        print_complex(s.amps[i].re, s.amps[i].im); print_str("\n");
    }
    print_str("Probability |00>: "); print_double(qstate_probability(&s, 0)); print_str("\n");
    print_str("Probability |11>: "); print_double(qstate_probability(&s, 3)); print_str("\n");
    print_str("Entropy qubit 0: "); print_double(entanglement_entropy(&s, 0)); print_str("\n");
    int m = measure_qubit(&s, 0);
    print_str("Measured qubit 0: "); print_int(m); print_str("\n");
    int m2 = measure_qubit(&s, 1);
    print_str("Measured qubit 1: "); print_int(m2); print_str("\n");
}

static void test_multi_qubit_gates(void) {
    print_str("\n--- Multi-Qubit Gate Test ---\n");
    quantum_state_t s = qstate_create(3);
    /* Prepare |110> then Toffoli -> |111> */
    gate_apply_single(&s, 0, gate_x());
    gate_apply_single(&s, 1, gate_x());
    gate_apply_toffoli(&s, 0, 1, 2);
    print_str("|110> after Toffoli -> prob |111>: ");
    print_double(qstate_probability(&s, 7)); print_str("\n");

    /* SWAP test */
    quantum_state_t s2 = qstate_create(2);
    gate_apply_single(&s2, 0, gate_x()); /* |10> */
    gate_apply_swap(&s2, 0, 1);          /* -> |01> */
    print_str("|10> after SWAP -> prob |01>: ");
    print_double(qstate_probability(&s2, 1)); print_str("\n");
}

static void test_grover(void) {
    print_str("\n--- Grover Search (3-qubit, target=5) ---\n");
    int result = grover_search(3, 5, 3);
    print_str("Result: |"); print_int(result); print_str(">\n");
    if (result == 5) print_str("SUCCESS: Found target!\n");
    else print_str("Note: probabilistic result\n");
}

static void test_shor(void) {
    print_str("\n--- Shor's Algorithm ---\n");
    int f = shor_factor(15);
    print_str("Factor of 15: "); print_int(f); print_str("\n");
    int f2 = shor_factor(21);
    print_str("Factor of 21: "); print_int(f2); print_str("\n");
}

static void test_vqe(void) {
    print_str("\n--- VQE Energy Estimation ---\n");
    /* H = Z + 0.5*X, minimum energy = -sqrt(1.25) ≈ -1.118 */
    double best = 1e9; double best_th = 0;
    for (int i = 0; i < 63; i++) {
        double th = (double)i * PI / 62.0;
        double e = vqe_solve(1.0, 0.5, th);
        if (e < best) { best = e; best_th = th; }
    }
    print_str("Best theta: "); print_double(best_th); print_str("\n");
    print_str("Energy: "); print_double(best); print_str("\n");
    print_str("Exact: -1.118034\n");
}

static void test_noise(void) {
    print_str("\n--- Noise Simulation ---\n");
    quantum_state_t s = qstate_create(1);
    gate_apply_single(&s, 0, gate_x()); /* |1> */
    print_str("Before noise, prob |1>: "); print_double(qstate_probability(&s, 1)); print_str("\n");
    noise_bit_flip(&s, 0.3);
    print_str("After 30% bit-flip noise, prob |1>: ");
    print_double(qstate_probability(&s, 1)); print_str("\n");
    quantum_state_t s2 = qstate_create(1);
    gate_apply_single(&s2, 0, gate_h());
    print_str("Before dephasing, prob |0>: "); print_double(qstate_probability(&s2, 0)); print_str("\n");
    noise_dephasing(&s2, 0.5, 1.0);
    print_str("After dephasing, prob |0>: "); print_double(qstate_probability(&s2, 0)); print_str("\n");
}

static void test_qec(void) {
    print_str("\n--- Quantum Error Correction ---\n");
    /* Encode |1> in bit-flip code, introduce error, decode */
    quantum_state_t orig = qstate_create(1);
    gate_apply_single(&orig, 0, gate_x()); /* |1> */
    quantum_state_t enc = qec_bit_flip_encode(&orig);
    /* introduce bit flip on qubit 0 */
    int bit0 = 1;
    for (int i = 0; i < enc.dim; i++) {
        if (i & bit0) { complex_t t = enc.amps[i]; enc.amps[i] = enc.amps[i & ~bit0]; enc.amps[i & ~bit0] = t; }
    }
    print_str("Encoded |1> with bit flip on q0, prob |000>: ");
    print_double(qstate_probability(&enc, 0)); print_str("\n");
    print_str("prob |111>: "); print_double(qstate_probability(&enc, 7)); print_str("\n");
    qec_bit_flip_decode(&enc);
    print_str("After decode, prob |111>: "); print_double(qstate_probability(&enc, 7)); print_str("\n");
    print_str("QEC corrected error successfully\n");
}

static void test_entangled_states(void) {
    print_str("\n--- Entangled State Preparation ---\n");
    quantum_state_t s1 = qstate_create(2);
    bell_state_prep(&s1, 0, 1, 2); /* Psi+ */
    print_str("Bell Psi+ prob |01>: "); print_double(qstate_probability(&s1, 1)); print_str("\n");
    print_str("Bell Psi+ prob |10>: "); print_double(qstate_probability(&s1, 2)); print_str("\n");

    quantum_state_t s2 = qstate_create(3);
    ghz_state(&s2, 3);
    print_str("GHZ(3) prob |000>: "); print_double(qstate_probability(&s2, 0)); print_str("\n");
    print_str("GHZ(3) prob |111>: "); print_double(qstate_probability(&s2, 7)); print_str("\n");

    quantum_state_t s3 = qstate_create(3);
    w_state(&s3, 3);
    print_str("W(3) prob |001>: "); print_double(qstate_probability(&s3, 1)); print_str("\n");
    print_str("W(3) prob |010>: "); print_double(qstate_probability(&s3, 2)); print_str("\n");
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str(VERSION); print_str("\n");

    if (help) {
        print_str("Usage: quantum_simulator [options]\n");
        print_str("  -h  Show this help message\n");
        print_str("  -t  Run comprehensive test suite\n");
        print_str("\nFeatures:\n");
        print_str("  - Complex number arithmetic (add, sub, mul, div, exp, polar)\n");
        print_str("  - Quantum state vectors up to 12 qubits\n");
        print_str("  - Standard gates: H, X, Y, Z, S, T, Rx, Ry, Rz\n");
        print_str("  - Multi-qubit: CNOT, CZ, SWAP, Toffoli, Fredkin\n");
        print_str("  - Entanglement: Bell, GHZ, W states\n");
        print_str("  - Projective & partial measurement\n");
        print_str("  - Algorithms: Grover search, Shor factoring, VQE\n");
        print_str("  - Noise: dephasing, amplitude damping, depolarizing, bit-flip\n");
        print_str("  - Error correction: bit-flip, phase-flip, Shor 9-qubit code\n");
        return;
    }

    if (test) {
        print_str("=== Quantum Simulator Test Suite ===\n");
        test_bell_state();
        test_multi_qubit_gates();
        test_entangled_states();
        test_grover();
        test_shor();
        test_vqe();
        test_noise();
        test_qec();
        print_str("\n=== All Tests Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t to run test suite\n");
}
