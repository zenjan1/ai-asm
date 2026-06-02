/* quantum_sim: Quantum computing simulator (v21.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Quantum gates */
#define GATE_PAULI_X   0
#define GATE_PAULI_Y   1
#define GATE_PAULI_Z   2
#define GATE_HADAMARD  3
#define GATE_CNOT      4
#define GATE_PHASE     5
#define GATE_T         6
#define GATE_SWAP      7

/* Limits */
#define MAX_QUBITS  16
#define NAME_LEN    64
#define CIRCUIT_LEN 256

/* Qubit state */
typedef struct {
    int   qubit_id;
    int   entangled_with;
    int   measured;
    int   amplitude_real;
    int   amplitude_imag;
} qubit_state_t;

/* Circuit operation */
typedef struct {
    int   op_id;
    int   gate;
    int   target_qubit;
    int   control_qubit;
    char  description[CIRCUIT_LEN];
} circuit_op_t;

static qubit_state_t qubits[MAX_QUBITS];
static int qubit_count = 0;
static circuit_op_t circuit[MAX_QUBITS * 8];
static int op_count = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Get gate name */
static const char *gate_name(int gate) {
    if (gate == GATE_PAULI_X) return "X";
    if (gate == GATE_PAULI_Y) return "Y";
    if (gate == GATE_PAULI_Z) return "Z";
    if (gate == GATE_HADAMARD) return "H";
    if (gate == GATE_CNOT) return "CNOT";
    if (gate == GATE_PHASE) return "Phase";
    if (gate == GATE_T) return "T";
    if (gate == GATE_SWAP) return "SWAP";
    return "Unknown";
}

/* Initialize qubits */
int quantum_sim_init(int count) {
    if (count > MAX_QUBITS) count = MAX_QUBITS;
    qubit_count = 0;
    op_count = 0;
    for (int i = 0; i < count; i++) {
        qubits[i].qubit_id = i;
        qubits[i].entangled_with = -1;
        qubits[i].measured = 0;
        qubits[i].amplitude_real = 1000;
        qubits[i].amplitude_imag = 0;
        qubit_count++;
    }
    print_str("  Initialized ");
    print_int(qubit_count);
    print_str(" qubits\n");
    return qubit_count;
}

/* Apply gate to qubit */
int quantum_sim_apply_gate(int gate, int target) {
    if (target < 0 || target >= qubit_count) return -1;
    int idx = op_count;
    circuit[idx].op_id = op_count + 1;
    circuit[idx].gate = gate;
    circuit[idx].target_qubit = target;
    circuit[idx].control_qubit = -1;
    my_strcpy(circuit[idx].description, gate_name(gate));
    op_count++;

    print_str("  Gate ");
    print_str(gate_name(gate));
    print_str(" -> qubit[");
    print_int(target);
    print_str("]\n");
    return 0;
}

/* Create entanglement */
int quantum_sim_entangle(int q1, int q2) {
    if (q1 < 0 || q1 >= qubit_count || q2 < 0 || q2 >= qubit_count) return -1;
    qubits[q1].entangled_with = q2;
    qubits[q2].entangled_with = q1;
    print_str("  Entangled: qubit[");
    print_int(q1);
    print_str("] <-> qubit[");
    print_int(q2);
    print_str("]\n");
    return 0;
}

/* Measure qubit */
int quantum_sim_measure(int qubit_id) {
    if (qubit_id < 0 || qubit_id >= qubit_count) return -1;
    int result = (qubit_id + qubits[qubit_id].amplitude_real) % 2;
    qubits[qubit_id].measured = 1;
    print_str("  Measure qubit[");
    print_int(qubit_id);
    print_str("]: |");
    print_int(result);
    print_str(">\n");
    return result;
}

/* Create Bell state */
int quantum_sim_bell_state(int q1, int q2) {
    quantum_sim_apply_gate(GATE_HADAMARD, q1);
    quantum_sim_apply_gate(GATE_CNOT, q1);
    circuit[op_count - 1].control_qubit = q1;
    circuit[op_count - 1].target_qubit = q2;
    quantum_sim_entangle(q1, q2);
    print_str("  Bell state created: qubit[");
    print_int(q1);
    print_str("], qubit[");
    print_int(q2);
    print_str("]\n");
    return 0;
}

/* Visualize circuit */
int quantum_sim_visualize(void) {
    print_str("=== Quantum Circuit ===\n");
    for (int i = 0; i < op_count; i++) {
        print_str("  ");
        print_int(i + 1);
        print_str(". ");
        print_str(gate_name(circuit[i].gate));
        print_str(" qubit[");
        print_int(circuit[i].target_qubit);
        print_str("]");
        if (circuit[i].control_qubit >= 0) {
            print_str(" (control: qubit[");
            print_int(circuit[i].control_qubit);
            print_str("])");
        }
        print_str("\n");
    }
    return op_count;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Quantum Sim v21.0 - Quantum Simulator\n");
    if (help) {
        print_str("Usage: quantum_sim [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run simulation\n");
        return;
    }
    if (test) {
        print_str("=== Quantum Simulator Test ===\n\n");
        quantum_sim_init(4);
        print_str("\n");
        quantum_sim_bell_state(0, 1);
        print_str("\n");
        quantum_sim_apply_gate(GATE_PAULI_X, 2);
        quantum_sim_apply_gate(GATE_HADAMARD, 3);
        print_str("\n");
        quantum_sim_visualize();
        print_str("\n");
        quantum_sim_measure(0);
        quantum_sim_measure(2);
        print_str("\n=== Simulation Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
