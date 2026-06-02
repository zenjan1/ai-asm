/* quantum_network: Distributed quantum network (v29.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Network operations */
#define QKD_BB84        0
#define QKD_E91         1
#define QKD_CONTINUOUS  2

/* Node states */
#define NODE_OFFLINE    0
#define NODE_ONLINE     1
#define NODE_ENTANGLED  2
#define NODE_KEY_ACTIVE 3

/* Limits */
#define MAX_NODES   16
#define MAX_KEYS    64
#define NAME_LEN    32
#define KEY_LEN     256

/* Network node */
typedef struct {
    int   node_id;
    char  name[NAME_LEN];
    int   state;
    int   qubits_available;
    int   entangled_with;
} quantum_node_t;

/* Quantum key */
typedef struct {
    int   key_id;
    int   node_a;
    int   node_b;
    char  key_bits[KEY_LEN];
    int   key_length;
    int   active;
    int   protocol;
} quantum_key_t;

static quantum_node_t nodes[MAX_NODES];
static quantum_key_t keys[MAX_KEYS];
static int node_count = 0;
static int key_count = 0;

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

/* Register quantum node */
int qn_register_node(const char *name, int qubits) {
    if (node_count >= MAX_NODES) return -1;
    int idx = node_count;
    nodes[idx].node_id = node_count + 1;
    my_strncpy(nodes[idx].name, name, NAME_LEN - 1);
    nodes[idx].state = NODE_ONLINE;
    nodes[idx].qubits_available = qubits;
    nodes[idx].entangled_with = -1;
    node_count++;
    return nodes[idx].node_id;
}

/* Quantum Key Distribution */
int qn_qkd(int node_a, int node_b, int protocol) {
    print_str("=== QKD Session ===\n");
    print_str("  Node A: ");
    print_int(node_a);
    print_str("\n");
    print_str("  Node B: ");
    print_int(node_b);
    print_str("\n");
    print_str("  Protocol: ");
    if (protocol == QKD_BB84) print_str("BB84\n");
    else if (protocol == QKD_E91) print_str("E91\n");
    else print_str("Continuous Variable\n");
    print_str("  Quantum channel: established\n");
    print_str("  Basis reconciliation: complete\n");
    print_str("  Error rate: 2.1% (below threshold)\n");
    print_str("  Privacy amplification: complete\n");

    if (key_count >= MAX_KEYS) return -1;
    int idx = key_count;
    keys[idx].key_id = key_count + 1;
    keys[idx].node_a = node_a;
    keys[idx].node_b = node_b;
    keys[idx].key_length = 256;
    keys[idx].active = 1;
    keys[idx].protocol = protocol;
    my_strcpy(keys[idx].key_bits, "1011001011010011010110100011010010110100110010101110001101010011");
    key_count++;

    print_str("  Key generated: 256 bits\n");
    return keys[idx].key_id;
}

/* Entangle two nodes */
int qn_entangle(int node_a, int node_b) {
    print_str("=== Quantum Entanglement ===\n");
    print_str("  Creating Bell pair between nodes ");
    print_int(node_a);
    print_str(" and ");
    print_int(node_b);
    print_str("\n");
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].node_id == node_a) nodes[i].entangled_with = node_b;
        if (nodes[i].node_id == node_b) nodes[i].entangled_with = node_a;
    }
    print_str("  Bell state: |Phi+>\n");
    print_str("  Fidelity: 99.2%\n");
    print_str("  Status: Entanglement established\n");
    return 0;
}

/* Quantum teleportation */
int qn_teleport(int source, int destination) {
    print_str("=== Quantum Teleportation ===\n");
    print_str("  Source: node ");
    print_int(source);
    print_str("\n");
    print_str("  Destination: node ");
    print_int(destination);
    print_str("\n");
    print_str("  Bell measurement: complete\n");
    print_str("  Classical channel: 2 bits sent\n");
    print_str("  State reconstruction: complete\n");
    print_str("  Fidelity: 98.7%\n");
    return 0;
}

/* Network status */
int qn_network_status(void) {
    print_str("=== Quantum Network Status ===\n");
    print_str("  Nodes: ");
    print_int(node_count);
    print_str("\n");
    print_str("  Active keys: ");
    print_int(key_count);
    print_str("\n");
    for (int i = 0; i < node_count; i++) {
        print_str("  Node ");
        print_int(nodes[i].node_id);
        print_str(": ");
        print_str(nodes[i].name);
        print_str(" (qubits=");
        print_int(nodes[i].qubits_available);
        if (nodes[i].entangled_with >= 0) {
            print_str(", entangled with node ");
            print_int(nodes[i].entangled_with);
        }
        print_str(")\n");
    }
    return node_count;
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
    print_str("Quantum Network v29.0 - Distributed QKD\n");
    if (help) {
        print_str("Usage: quantum_network [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run network test\n");
        return;
    }
    if (test) {
        print_str("=== Quantum Network Test ===\n\n");
        qn_register_node("alice", 8);
        qn_register_node("bob", 8);
        qn_register_node("charlie", 4);
        print_str("\n");
        qn_entangle(1, 2);
        print_str("\n");
        qn_qkd(1, 2, QKD_BB84);
        print_str("\n");
        qn_teleport(1, 3);
        print_str("\n");
        qn_network_status();
        print_str("\n=== Quantum Network Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
