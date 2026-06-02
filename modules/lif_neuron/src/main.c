/* lif_neuron: Leaky Integrate-and-Fire neuron (v24.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_NEURONS  128

/* LIF neuron state */
typedef struct {
    int   neuron_id;
    int   membrane_potential;
    int   threshold;
    int   reset_potential;
    int   leak_rate;
    int   refractory_count;
    int   refractory_period;
    int   spike_count;
    int   last_spike_time;
} lif_neuron_t;

/* Input event */
typedef struct {
    int   event_id;
    int   target_neuron;
    int   spike_weight;
    int   timestamp;
} spike_event_t;

static lif_neuron_t neurons[MAX_NEURONS];
static int neuron_count = 0;
static int sim_time = 0;
static int total_spikes = 0;

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

/* Create LIF neuron */
int lif_create(int threshold, int leak_rate, int refractory_period) {
    if (neuron_count >= MAX_NEURONS) return -1;
    int idx = neuron_count;
    neurons[idx].neuron_id = neuron_count + 1;
    neurons[idx].membrane_potential = 0;
    neurons[idx].threshold = threshold;
    neurons[idx].reset_potential = -70;
    neurons[idx].leak_rate = leak_rate;
    neurons[idx].refractory_count = 0;
    neurons[idx].refractory_period = refractory_period;
    neurons[idx].spike_count = 0;
    neurons[idx].last_spike_time = 0;
    neuron_count++;
    return neurons[idx].neuron_id;
}

/* Process input spike to neuron */
int lif_process_input(int neuron_id, int input_weight) {
    for (int i = 0; i < neuron_count; i++) {
        if (neurons[i].neuron_id == neuron_id) {
            if (neurons[i].refractory_count > 0) return 0;
            neurons[i].membrane_potential += input_weight;
            return 0;
        }
    }
    return -1;
}

/* Simulate one timestep - LIF dynamics */
int lif_timestep(void) {
    sim_time++;
    int spikes = 0;
    for (int i = 0; i < neuron_count; i++) {
        /* Refractory period */
        if (neurons[i].refractory_count > 0) {
            neurons[i].refractory_count--;
            neurons[i].membrane_potential = neurons[i].reset_potential;
            continue;
        }

        /* Leak: membrane potential decays */
        neurons[i].membrane_potential -= neurons[i].leak_rate;

        /* Fire if threshold reached */
        if (neurons[i].membrane_potential >= neurons[i].threshold) {
            neurons[i].spike_count++;
            neurons[i].last_spike_time = sim_time;
            neurons[i].refractory_count = neurons[i].refractory_period;
            spikes++;
            total_spikes++;
        }
    }
    return spikes;
}

/* Run simulation for N steps */
int lif_run_simulation(int steps) {
    print_str("=== LIF Simulation ===\n");
    print_str("  Neurons: ");
    print_int(neuron_count);
    print_str("\n");
    print_str("  Steps: ");
    print_int(steps);
    print_str("\n\n");

    for (int t = 0; t < steps; t++) {
        int spikes = lif_timestep();
        if (spikes > 0) {
            print_str("  t=");
            print_int(sim_time);
            print_str(": ");
            print_int(spikes);
            print_str(" spike(s)\n");
        }
    }
    print_str("\n  Total spikes: ");
    print_int(total_spikes);
    print_str("\n");
    return total_spikes;
}

/* Get neuron state */
int lif_get_state(int neuron_id) {
    for (int i = 0; i < neuron_count; i++) {
        if (neurons[i].neuron_id == neuron_id) {
            print_str("  Neuron ");
            print_int(neuron_id);
            print_str(": V=");
            print_int(neurons[i].membrane_potential);
            print_str(" spikes=");
            print_int(neurons[i].spike_count);
            print_str(" ref=");
            print_int(neurons[i].refractory_count);
            print_str("\n");
            return 0;
        }
    }
    return -1;
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
    print_str("LIF Neuron v24.0 - Leaky Integrate-and-Fire\n");
    if (help) {
        print_str("Usage: lif_neuron [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run simulation\n");
        return;
    }
    if (test) {
        print_str("=== LIF Neuron Test ===\n\n");
        lif_create(1000, 5, 3);
        lif_create(1000, 5, 3);
        lif_create(1000, 5, 3);
        print_str("Created 3 neurons\n\n");
        /* Stimulate neurons */
        for (int t = 0; t < 50; t++) {
            lif_process_input(1, 120);
            lif_process_input(2, 80);
            lif_process_input(3, 50);
        }
        lif_run_simulation(100);
        print_str("\n");
        lif_get_state(1);
        lif_get_state(2);
        lif_get_state(3);
        print_str("\n=== LIF Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
