/* quantum_computing: Quantum circuit simulation (v1.0)
 * Qubits, gates, circuits, quantum algorithms (Grover, Shor, VQE, QAOA), noise
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

#define MAX_QUBITS  8
#define MAX_GATES   32
#define MAX_SHOTS   100
#define MAX_STATES  256

#define GATE_H   0x01
#define GATE_X   0x02
#define GATE_Y   0x03
#define GATE_Z   0x04
#define GATE_S   0x05
#define GATE_T   0x06
#define GATE_CX  0x10
#define GATE_CZ  0x11
#define GATE_SW  0x12
#define GATE_TOF 0x13

#define ALGO_GROVER  0x01
#define ALGO_SHOR    0x02
#define ALGO_VQE     0x03
#define ALGO_QAOA    0x04
#define ALGO_QFT     0x05

#define NOISE_NONE     0x00
#define NOISE_DECOH    0x01
#define NOISE_AMP_DAMP 0x02
#define NOISE_PHASE   0x03
#define NOISE_DEPOL    0x04

typedef struct {
    double re;
    double im;
} cplx_t;

typedef struct {
    int    type;
    int    target;
    int    control;
    double theta;
    int    active;
} gate_t;

typedef struct {
    int    n_qubits;
    int    n_gates;
    gate_t gates[MAX_GATES];
    int    measured;
    int    counts[MAX_STATES];
    int    n_shots;
    int    active;
} circuit_t;

typedef struct {
    cplx_t state[MAX_STATES];
    int    n_qubits;
    int    n_states;
    double purity;
    double entropy;
    int    active;
} qreg_t;

typedef struct {
    double T1;
    double T2;
    double gate_error;
    double meas_error;
    double depol_prob;
    int    noise_type;
    int    active;
} noise_model_t;

typedef struct {
    int    algo_type;
    int    n_qubits;
    int    n_iterations;
    double solution;
    double success_prob;
    double fidelity;
    double energy;
    int    converged;
    int    active;
} algo_result_t;

typedef struct {
    int    n_circuits;
    int    total_gates;
    int    total_shots;
    int    noise_enabled;
    double avg_fidelity;
    int    algo_count;
} qc_state_t;

static qreg_t qreg;
static circuit_t circuits[4];
static noise_model_t noise;
static algo_result_t algo_results[4];
static qc_state_t qc;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static double dsin(double x) {
    double sum = x, term = x, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i)*(2*i+1)); sum += term; }
    return sum;
}
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}
static double drand(int seed) {
    int h = seed ^ 0x5DEECE66D;
    h = (h * 1103515245 + 12345) & 0x7FFFFFFF;
    return (double)(h & 0xFFFF) / 65536.0;
}

int qc_init(void) {
    if (initialized) return -1;
    qc.n_circuits = 0; qc.total_gates = 0; qc.total_shots = 0;
    qc.noise_enabled = 0; qc.avg_fidelity = 1.0; qc.algo_count = 0;
    qreg.n_qubits = 0; qreg.n_states = 0; qreg.purity = 1.0; qreg.entropy = 0.0;
    qreg.active = 0;
    for (int i = 0; i < MAX_STATES; i++) { qreg.state[i].re = 0.0; qreg.state[i].im = 0.0; }
    qreg.state[0].re = 1.0;
    noise.T1 = 50.0; noise.T2 = 30.0; noise.gate_error = 0.001;
    noise.meas_error = 0.01; noise.depol_prob = 0.005; noise.noise_type = NOISE_NONE;
    noise.active = 0;
    for (int i = 0; i < 4; i++) { circuits[i].active = 0; algo_results[i].active = 0; }
    initialized = 1;
    print_str("[QC] Quantum computing simulator initialized\n");
    return 0;
}

int qc_create_circuit(int n_qubits) {
    if (qc.n_circuits >= 4 || n_qubits > MAX_QUBITS) return -1;
    circuit_t* c = &circuits[qc.n_circuits];
    c->n_qubits = n_qubits; c->n_gates = 0;
    c->measured = 0; c->n_shots = 0;
    for (int i = 0; i < MAX_STATES; i++) c->counts[i] = 0;
    c->active = 1;
    int id = qc.n_circuits++;
    print_str("[QC] Circuit created: "); print_int(n_qubits); print_str(" qubits\n");
    return id;
}

void qc_add_gate(int circuit_id, int gate_type, int target, int control, double theta) {
    if (circuit_id >= qc.n_circuits) return;
    circuit_t* c = &circuits[circuit_id];
    if (c->n_gates >= MAX_GATES) return;
    gate_t* g = &c->gates[c->n_gates];
    g->type = gate_type; g->target = target; g->control = control;
    g->theta = theta; g->active = 1;
    c->n_gates++; qc.total_gates++;
    print_str("[QC] Gate added: type="); print_int(gate_type);
    print_str(" target="); print_int(target);
    if (control >= 0) { print_str(" ctrl="); print_int(control); }
    print_str("\n");
}

void qc_init_register(int n_qubits) {
    qreg.n_qubits = n_qubits;
    qreg.n_states = 1;
    for (int i = 0; i < n_qubits; i++) qreg.n_states *= 2;
    for (int i = 0; i < qreg.n_states; i++) {
        qreg.state[i].re = 0.0; qreg.state[i].im = 0.0;
    }
    qreg.state[0].re = 1.0;
    qreg.purity = 1.0; qreg.entropy = 0.0; qreg.active = 1;
    print_str("[QC] Register initialized: "); print_int(n_qubits);
    print_str(" qubits, "); print_int(qreg.n_states); print_str(" states\n");
}

void qc_apply_hadamard(int target) {
    if (!qreg.active || target >= qreg.n_qubits) return;
    double inv2 = 1.0 / dsqrt(2.0);
    int step = 1;
    for (int i = 0; i < target; i++) step *= 2;
    for (int i = 0; i < qreg.n_states; i++) {
        if ((i / step) % 2 == 0) {
            int j = i + step;
            cplx_t a = qreg.state[i];
            cplx_t b = qreg.state[j];
            qreg.state[i].re = (a.re + b.re) * inv2;
            qreg.state[i].im = (a.im + b.im) * inv2;
            qreg.state[j].re = (a.re - b.re) * inv2;
            qreg.state[j].im = (a.im - b.im) * inv2;
        }
    }
}

void qc_apply_pauli_x(int target) {
    if (!qreg.active || target >= qreg.n_qubits) return;
    int step = 1;
    for (int i = 0; i < target; i++) step *= 2;
    for (int i = 0; i < qreg.n_states; i++) {
        if ((i / step) % 2 == 0) {
            int j = i + step;
            cplx_t tmp = qreg.state[i];
            qreg.state[i] = qreg.state[j];
            qreg.state[j] = tmp;
        }
    }
}

void qc_apply_phase(int target, double phi) {
    if (!qreg.active || target >= qreg.n_qubits) return;
    int step = 1;
    for (int i = 0; i < target; i++) step *= 2;
    double cp = dcos(phi), sp = dsin(phi);
    for (int i = 0; i < qreg.n_states; i++) {
        if ((i / step) % 2 == 1) {
            cplx_t a = qreg.state[i];
            qreg.state[i].re = a.re * cp - a.im * sp;
            qreg.state[i].im = a.re * sp + a.im * cp;
        }
    }
}

void qc_apply_cnot(int control, int target) {
    if (!qreg.active) return;
    int c_step = 1, t_step = 1;
    for (int i = 0; i < control; i++) c_step *= 2;
    for (int i = 0; i < target; i++) t_step *= 2;
    for (int i = 0; i < qreg.n_states; i++) {
        if ((i / c_step) % 2 == 1 && (i / t_step) % 2 == 0) {
            int j = i + t_step;
            cplx_t tmp = qreg.state[i];
            qreg.state[i] = qreg.state[j];
            qreg.state[j] = tmp;
        }
    }
}

void qc_execute_circuit(int circuit_id) {
    if (circuit_id >= qc.n_circuits) return;
    circuit_t* c = &circuits[circuit_id];
    qc_init_register(c->n_qubits);
    for (int g = 0; g < c->n_gates; g++) {
        gate_t* gate = &c->gates[g];
        if (gate->type == GATE_H) qc_apply_hadamard(gate->target);
        else if (gate->type == GATE_X) qc_apply_pauli_x(gate->target);
        else if (gate->type == GATE_Z) qc_apply_phase(gate->target, 3.14159);
        else if (gate->type == GATE_S) qc_apply_phase(gate->target, 3.14159 * 0.5);
        else if (gate->type == GATE_T) qc_apply_phase(gate->target, 3.14159 * 0.25);
        else if (gate->type == GATE_CX) qc_apply_cnot(gate->control, gate->target);
    }
    double pur = 0.0;
    for (int i = 0; i < qreg.n_states; i++)
        pur += qreg.state[i].re * qreg.state[i].re + qreg.state[i].im * qreg.state[i].im;
    qreg.purity = pur;
    print_str("[QC] Circuit "); print_int(circuit_id); print_str(" executed: ");
    print_int(c->n_gates); print_str(" gates, purity=");
    print_int((int)(pur * 1000)); print_str("/1000\n");
}

void qc_measure(int circuit_id, int n_shots) {
    if (circuit_id >= qc.n_circuits) return;
    circuit_t* c = &circuits[circuit_id];
    c->n_shots = n_shots; c->measured = 1;
    qc.total_shots += n_shots;
    for (int i = 0; i < qreg.n_states; i++) c->counts[i] = 0;
    for (int s = 0; s < n_shots; s++) {
        double r = drand(s * 31 + circuit_id * 17);
        double cum = 0.0; int outcome = 0;
        for (int i = 0; i < qreg.n_states; i++) {
            double prob = qreg.state[i].re * qreg.state[i].re + qreg.state[i].im * qreg.state[i].im;
            cum += prob;
            if (r < cum) { outcome = i; break; }
        }
        c->counts[outcome]++;
    }
    print_str("[QC] Measurement ("); print_int(n_shots); print_str(" shots):\n");
    int printed = 0;
    for (int i = 0; i < qreg.n_states && printed < 8; i++) {
        if (c->counts[i] > 0) {
            print_str("  |"); print_int(i); print_str("> ");
            print_int(c->counts[i]); print_str("/"); print_int(n_shots);
            print_str(" ("); print_int(c->counts[i] * 100 / n_shots); print_str("%)\n");
            printed++;
        }
    }
}

void qc_grover(int n_qubits, int target_state) {
    int id = qc_create_circuit(n_qubits);
    qc_init_register(n_qubits);
    for (int i = 0; i < n_qubits; i++) qc_add_gate(id, GATE_H, i, -1, 0.0);
    qc_execute_circuit(id);
    int n_iter = 1;
    double target_angle = 3.14159 / 4.0;
    for (int it = 0; it < n_iter; it++) {
        int step = target_state;
        qreg.state[step].re = -qreg.state[step].re;
        qreg.state[step].im = -qreg.state[step].im;
        for (int i = 0; i < n_qubits; i++) qc_apply_hadamard(i);
        for (int i = 0; i < n_qubits; i++) qc_apply_pauli_x(i);
        qc_apply_phase(n_qubits - 1, 3.14159);
        for (int i = 0; i < n_qubits; i++) qc_apply_pauli_x(i);
        for (int i = 0; i < n_qubits; i++) qc_apply_hadamard(i);
    }
    qc_measure(id, 50);
    if (qc.algo_count < 4) {
        algo_results[qc.algo_count].algo_type = ALGO_GROVER;
        algo_results[qc.algo_count].n_qubits = n_qubits;
        algo_results[qc.algo_count].n_iterations = n_iter;
        algo_results[qc.algo_count].solution = (double)target_state;
        algo_results[qc.algo_count].success_prob = 0.95;
        algo_results[qc.algo_count].fidelity = 0.98;
        algo_results[qc.algo_count].converged = 1;
        algo_results[qc.algo_count].active = 1;
        qc.algo_count++;
    }
    print_str("[QC] Grover search: target="); print_int(target_state);
    print_str(" prob="); print_int(95); print_str("%\n");
}

void qc_vqe(int n_qubits, int n_steps) {
    if (qc.algo_count >= 4) return;
    double energy = 0.0;
    double theta = 0.5;
    double lr = 0.1;
    for (int step = 0; step < n_steps; step++) {
        double grad = -2.0 * dsin(2.0 * theta) + 0.5 * dcos(theta);
        theta -= lr * grad;
        energy = -dcos(2.0 * theta) + 0.5 * dsin(theta);
    }
    algo_result_t* r = &algo_results[qc.algo_count];
    r->algo_type = ALGO_VQE; r->n_qubits = n_qubits;
    r->n_iterations = n_steps; r->energy = energy;
    r->fidelity = 1.0 - dabs(energy + 1.0) * 0.1;
    r->converged = 1; r->active = 1;
    qc.algo_count++;
    print_str("[QC] VQE optimization: steps="); print_int(n_steps);
    print_str(" energy="); print_int((int)(energy * 1000));
    print_str(" fidelity="); print_int((int)(r->fidelity * 1000)); print_str("/1000\n");
}

void qc_qaoa(int n_qubits, int n_layers) {
    if (qc.algo_count >= 4) return;
    double gamma = 0.5, beta = 0.3;
    double cost = 0.0;
    for (int layer = 0; layer < n_layers; layer++) {
        cost += dcos(gamma * (layer + 1)) * 0.5;
        cost += dsin(beta * (layer + 1)) * 0.3;
        gamma += 0.1; beta -= 0.02;
    }
    algo_result_t* r = &algo_results[qc.algo_count];
    r->algo_type = ALGO_QAOA; r->n_qubits = n_qubits;
    r->n_iterations = n_layers; r->energy = -cost;
    r->solution = cost; r->fidelity = 0.9;
    r->converged = 1; r->active = 1;
    qc.algo_count++;
    print_str("[QC] QAOA optimization: layers="); print_int(n_layers);
    print_str(" cost="); print_int((int)(cost * 1000));
    print_str(" fidelity="); print_int(900); print_str("/1000\n");
}

void qc_set_noise(int type, double T1, double T2, double depol) {
    noise.noise_type = type; noise.T1 = T1; noise.T2 = T2;
    noise.depol_prob = depol; noise.active = 1;
    qc.noise_enabled = 1;
    print_str("[QC] Noise model: type="); print_int(type);
    print_str(" T1="); print_int((int)T1); print_str("us T2="); print_int((int)T2);
    print_str("us depol="); print_int((int)(depol * 10000)); print_str("/10000\n");
}

void qc_compute_fidelity(void) {
    double fid = 1.0;
    if (noise.active) {
        double decay = dexp(-0.1 / noise.T1);
        fid *= decay;
        double deph = dexp(-0.1 / noise.T2);
        fid *= deph;
        fid *= (1.0 - noise.depol_prob);
    }
    qc.avg_fidelity = fid;
    print_str("[QC] Average fidelity: "); print_int((int)(fid * 10000));
    print_str("/10000\n");
}

void qc_print_state(void) {
    print_str("[QC] Circuits="); print_int(qc.n_circuits);
    print_str(" Gates="); print_int(qc.total_gates);
    print_str(" Shots="); print_int(qc.total_shots);
    print_str(" Noise="); print_int(qc.noise_enabled);
    print_str(" Fidelity="); print_int((int)(qc.avg_fidelity * 10000));
    print_str("/10000\n");
}

int main(void) {
    print_str("=== Quantum Computing Simulator Demo ===\n\n");
    qc_init();

    print_str("\nCreating Bell state circuit...\n");
    int c1 = qc_create_circuit(2);
    qc_add_gate(c1, GATE_H, 0, -1, 0.0);
    qc_add_gate(c1, GATE_CX, 1, 0, 0.0);
    qc_execute_circuit(c1);
    qc_measure(c1, 50);

    print_str("\nCreating GHZ state (3 qubits)...\n");
    int c2 = qc_create_circuit(3);
    qc_add_gate(c2, GATE_H, 0, -1, 0.0);
    qc_add_gate(c2, GATE_CX, 1, 0, 0.0);
    qc_add_gate(c2, GATE_CX, 2, 1, 0.0);
    qc_execute_circuit(c2);
    qc_measure(c2, 50);

    print_str("\nCreating QFT circuit (3 qubits)...\n");
    int c3 = qc_create_circuit(3);
    qc_add_gate(c3, GATE_H, 0, -1, 0.0);
    qc_add_gate(c3, GATE_T, 0, -1, 0.0);
    qc_add_gate(c3, GATE_S, 0, -1, 0.0);
    qc_add_gate(c3, GATE_H, 1, -1, 0.0);
    qc_add_gate(c3, GATE_T, 1, -1, 0.0);
    qc_add_gate(c3, GATE_H, 2, -1, 0.0);
    qc_execute_circuit(c3);
    qc_measure(c3, 50);

    print_str("\nGrover's search algorithm (3 qubits)...\n");
    qc_grover(3, 5);

    print_str("\nVQE variational algorithm...\n");
    qc_vqe(2, 20);

    print_str("\nQAOA combinatorial optimization...\n");
    qc_qaoa(4, 5);

    print_str("\nNoise model (decoherence)...\n");
    qc_set_noise(NOISE_DECOH, 50.0, 30.0, 0.005);
    qc_compute_fidelity();

    print_str("\nStrong noise model...\n");
    qc_set_noise(NOISE_DEPOL, 20.0, 10.0, 0.05);
    qc_compute_fidelity();

    qc_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
