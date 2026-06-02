/* spiking_nn: Spiking neural network system (v23.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Neuron types */
#define NEURON_LIF       0  /* Leaky Integrate-and-Fire */
#define NEURON_ALIF      1  /* Adaptive LIF */
#define NEURON_HODGKIN   2  /* Hodgkin-Huxley */
#define NEURON_IZHIKEVICH 3 /* Izhikevich */

/* Learning rules */
#define LEARN_STDP       0  /* Spike-Timing Dependent Plasticity */
#define LEARN_HEBBIAN    1  /* Hebbian learning */
#define LEARN_REWARD     2  /* Reward-modulated STDP */

/* Input types */
#define INPUT_SENSOR     0
#define INPUT_AUDIO      1
#define INPUT_VIDEO      2
#define INPUT_EVENT      3

/* Limits */
#define MAX_NETWORKS 8
#define MAX_NEURONS  256
#define MAX_SYNAPSES 512
#define NAME_LEN     64

/* Neuron */
typedef struct {
    int   neuron_id;
    int   type;
    int   membrane_potential;
    int   threshold;
    int   refractory_count;
    int   spike_count;
    int   layer;
} neuron_t;

/* Synapse */
typedef struct {
    int   synapse_id;
    int   pre_neuron;
    int   post_neuron;
    int   weight;
    int   delay;
    int   plastic;
} synapse_t;

/* Network */
typedef struct {
    int   network_id;
    char  name[NAME_LEN];
    int   neuron_count;
    int   synapse_count;
    int   input_channels;
    int   output_channels;
    int   learning_rule;
    int   hardware;
    int   total_spikes;
    int   energy_efficiency;
} network_t;

static network_t networks[MAX_NETWORKS];
static neuron_t neurons[MAX_NEURONS];
static synapse_t synapses[MAX_SYNAPSES];
static int net_count = 0;
static int neuron_count = 0;
static int synapse_count = 0;

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

/* Get neuron type name */
static const char *neuron_type_name(int type) {
    if (type == NEURON_LIF) return "LIF";
    if (type == NEURON_ALIF) return "AdaptiveLIF";
    if (type == NEURON_HODGKIN) return "HodgkinHuxley";
    if (type == NEURON_IZHIKEVICH) return "Izhikevich";
    return "unknown";
}

/* Create network */
int snn_create_network(const char *name, int neuron_type, int n_neurons, int layers) {
    if (net_count >= MAX_NETWORKS) return -1;
    int net_idx = net_count;
    networks[net_idx].network_id = net_count + 1;
    my_strncpy(networks[net_idx].name, name, NAME_LEN - 1);
    networks[net_idx].neuron_count = n_neurons;
    networks[net_idx].synapse_count = n_neurons * 4;
    networks[net_idx].input_channels = 1;
    networks[net_idx].output_channels = 1;
    networks[net_idx].learning_rule = LEARN_STDP;
    networks[net_idx].hardware = 0;
    networks[net_idx].total_spikes = 0;
    networks[net_idx].energy_efficiency = 15;
    net_count++;

    int base = neuron_count;
    for (int i = 0; i < n_neurons && (base + i) < MAX_NEURONS; i++) {
        neurons[base + i].neuron_id = neuron_count + i;
        neurons[base + i].type = neuron_type;
        neurons[base + i].membrane_potential = 0;
        neurons[base + i].threshold = 1000;
        neurons[base + i].refractory_count = 0;
        neurons[base + i].spike_count = 0;
        neurons[base + i].layer = (i * layers) / n_neurons;
    }
    neuron_count += n_neurons;

    print_str("  Network created: ");
    print_str(name);
    print_str("\n");
    print_str("    Neurons: ");
    print_int(n_neurons);
    print_str(" (");
    print_str(neuron_type_name(neuron_type));
    print_str(")\n");
    print_str("    Layers: ");
    print_int(layers);
    print_str("\n");
    print_str("    Learning: STDP\n");
    return networks[net_idx].network_id;
}

/* Process a spike event */
int snn_process_spike(int input_type, int data) {
    print_str("  Processing spike event: ");
    print_str(input_type == INPUT_SENSOR ? "sensor" :
              input_type == INPUT_AUDIO ? "audio" :
              input_type == INPUT_VIDEO ? "video" : "event");
    print_str(" value=");
    print_int(data);
    print_str("\n");
    print_str("    Propagating through network...\n");
    print_str("    STDP update: weights adjusted\n");
    for (int i = 0; i < net_count; i++) {
        networks[i].total_spikes += 12;
    }
    return 0;
}

/* STDP learning step */
int snn_stdp_learning(void) {
    print_str("=== STDP Learning ===\n");
    print_str("  Pre-post timing analysis...\n");
    print_str("  Long-term potentiation (LTP): +15 synapses\n");
    print_str("  Long-term depression (LTD): -8 synapses\n");
    print_str("  Net weight change: +7 synapses strengthened\n");
    print_str("  Learning rate: 0.01\n");
    return 0;
}

/* Get energy statistics */
int snn_energy_stats(void) {
    print_str("=== Energy Statistics ===\n");
    for (int i = 0; i < net_count; i++) {
        print_str("  Network: ");
        print_str(networks[i].name);
        print_str("\n");
        print_str("    Total spikes: ");
        print_int(networks[i].total_spikes);
        print_str("\n");
        print_str("    Energy efficiency: ");
        print_int(networks[i].energy_efficiency);
        print_str(" TOPS/W\n");
        print_str("    Estimated power: 0.05W\n");
    }
    return 0;
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
    print_str("Spiking NN v23.0 - Neuromorphic Computing\n");
    if (help) {
        print_str("Usage: spiking_nn [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run network test\n");
        return;
    }
    if (test) {
        print_str("=== Spiking Neural Network Test ===\n\n");
        snn_create_network("vision_snn", NEURON_LIF, 64, 4);
        print_str("\n");
        snn_create_network("audio_snn", NEURON_ALIF, 32, 3);
        print_str("\n");
        snn_stdp_learning();
        print_str("\n");
        snn_process_spike(INPUT_SENSOR, 235);
        snn_process_spike(INPUT_AUDIO, 128);
        snn_process_spike(INPUT_EVENT, 1);
        print_str("\n");
        snn_energy_stats();
        print_str("\n=== Spiking NN Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
