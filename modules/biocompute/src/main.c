/* biocompute: Biological computing interface (v26.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Bio interface types */
#define BIO_NEURAL_INTERFACE 0
#define BIO_MOLECULAR_COMPUTE 1
#define BIO_SENSOR           2

/* Neuron types */
#define NEURON_BIOLOGICAL 0
#define NEURON_ARTIFICIAL 1
#define NEURON_HYBRID     2

/* Sensor types */
#define SENSOR_PH      0
#define SENSOR_TEMP    1
#define SENSOR_GLUCOSE 2
#define SENSOR_ECG     3
#define SENSOR_EEG     4

/* Limits */
#define MAX_NETWORKS 8
#define MAX_NEURONS  128
#define MAX_SENSORS  16
#define NAME_LEN     64
#define DESC_LEN     256

/* Bio neuron */
typedef struct {
    int   neuron_id;
    int   type;
    int   firing_rate;
    int   membrane_potential;
    int   threshold;
    int   spike_count;
} bio_neuron_t;

/* Bio sensor */
typedef struct {
    int   sensor_id;
    int   sensor_type;
    int   value;
    int   active;
    char  name[NAME_LEN];
} bio_sensor_t;

/* Bio network */
typedef struct {
    int   network_id;
    char  name[NAME_LEN];
    int   neuron_count;
    int   connection_count;
    int   processing;
} bio_network_t;

static bio_neuron_t neurons[MAX_NEURONS];
static bio_sensor_t sensors[MAX_SENSORS];
static bio_network_t networks[MAX_NETWORKS];
static int neuron_count = 0;
static int sensor_count = 0;
static int network_count = 0;

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

/* Get sensor type name */
static const char *sensor_type_name(int type) {
    if (type == SENSOR_PH) return "pH";
    if (type == SENSOR_TEMP) return "temperature";
    if (type == SENSOR_GLUCOSE) return "glucose";
    if (type == SENSOR_ECG) return "ECG";
    if (type == SENSOR_EEG) return "EEG";
    return "unknown";
}

/* Create bio network */
int biocompute_create_network(const char *name, int neuron_type, int count) {
    if (network_count >= MAX_NETWORKS) return -1;
    int net_idx = network_count;
    networks[net_idx].network_id = network_count + 1;
    my_strncpy(networks[net_idx].name, name, NAME_LEN - 1);
    networks[net_idx].neuron_count = count;
    networks[net_idx].connection_count = count * 4;
    networks[net_idx].processing = 0;
    network_count++;

    for (int i = 0; i < count && neuron_count < MAX_NEURONS; i++) {
        neurons[neuron_count].neuron_id = neuron_count + 1;
        neurons[neuron_count].type = neuron_type;
        neurons[neuron_count].firing_rate = 10;
        neurons[neuron_count].membrane_potential = -70;
        neurons[neuron_count].threshold = -55;
        neurons[neuron_count].spike_count = 0;
        neuron_count++;
    }

    print_str("  Network created: ");
    print_str(name);
    print_str(" (");
    print_int(count);
    print_str(" neurons)\n");
    return networks[net_idx].network_id;
}

/* Register bio sensor */
int biocompute_register_sensor(int sensor_type, const char *name) {
    if (!name || sensor_count >= MAX_SENSORS) return -1;
    int idx = sensor_count;
    sensors[idx].sensor_id = sensor_count + 1;
    sensors[idx].sensor_type = sensor_type;
    sensors[idx].value = 0;
    sensors[idx].active = 1;
    my_strncpy(sensors[idx].name, name, NAME_LEN - 1);
    sensor_count++;
    return sensors[idx].sensor_id;
}

/* Read sensor data */
int biocompute_read_sensor(int sensor_id) {
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].sensor_id == sensor_id) {
            print_str("  Sensor: ");
            print_str(sensors[i].name);
            print_str(" [");
            print_str(sensor_type_name(sensors[i].sensor_type));
            print_str("]\n");
            sensors[i].value = 72 + sensor_id * 3;
            print_str("  Value: ");
            print_int(sensors[i].value);
            print_str("\n");
            return sensors[i].value;
        }
    }
    return -1;
}

/* Process molecular computation */
int biocompute_molecular_process(const char *task) {
    if (!task) return -1;
    print_str("=== Molecular Computation ===\n");
    print_str("  Task: ");
    print_str(task);
    print_str("\n");
    print_str("  Molecule encoding: complete\n");
    print_str("  Reaction simulation: complete\n");
    print_str("  Result decoded: success\n");
    return 0;
}

/* Neural interface stimulation */
int biocompute_stimulate(int network_id) {
    print_str("=== Neural Stimulation ===\n");
    for (int i = 0; i < network_count; i++) {
        if (networks[i].network_id == network_id) {
            networks[i].processing = 1;
            print_str("  Network: ");
            print_str(networks[i].name);
            print_str("\n");
            print_str("  Stimulating ");
            print_int(networks[i].neuron_count);
            print_str(" neurons...\n");
            int spikes = 0;
            for (int j = 0; j < neuron_count; j++) {
                neurons[j].membrane_potential += 20;
                if (neurons[j].membrane_potential >= neurons[j].threshold) {
                    neurons[j].spike_count++;
                    spikes++;
                }
            }
            print_str("  Spikes generated: ");
            print_int(spikes);
            print_str("\n");
            networks[i].processing = 0;
            return 0;
        }
    }
    return -1;
}

/* Get all sensor readings */
int biocompute_get_all_sensors(void) {
    print_str("=== All Sensor Readings ===\n");
    for (int i = 0; i < sensor_count; i++) {
        if (sensors[i].active) {
            biocompute_read_sensor(sensors[i].sensor_id);
        }
    }
    return sensor_count;
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
    print_str("Biocompute v26.0 - Biological Computing\n");
    if (help) {
        print_str("Usage: biocompute [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run bio test\n");
        return;
    }
    if (test) {
        print_str("=== Biocompute Test ===\n\n");
        biocompute_create_network("cortical_net", NEURON_HYBRID, 32);
        print_str("\n");
        biocompute_register_sensor(SENSOR_EEG, "eeg_cortex");
        biocompute_register_sensor(SENSOR_ECG, "ecg_heart");
        biocompute_register_sensor(SENSOR_GLUCOSE, "glucose_blood");
        print_str("\n");
        biocompute_get_all_sensors();
        print_str("\n");
        biocompute_stimulate(1);
        print_str("\n");
        biocompute_molecular_process("protein_folding_sim");
        print_str("\n=== Biocompute Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
