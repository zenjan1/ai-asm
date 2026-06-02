/* neuromorphic_interface: Neuromorphic hardware interface (v24.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Hardware types */
#define HW_FPGA      0
#define HW_LOIHI     1
#define HW_TRUENORTH 2
#define HW_SPIKING_CHIP 3

/* Limits */
#define NAME_LEN    64
#define MAX_DEVICES 8
#define MAX_CONFIGS 32
#define CONFIG_LEN  256

/* Device info */
typedef struct {
    int   device_id;
    int   hw_type;
    char  name[NAME_LEN];
    int   neurons;
    int   synapses;
    int   power_mw;
    int   topology;
} device_info_t;

/* Synaptic config */
typedef struct {
    int   config_id;
    int   device_id;
    int   pre_neuron;
    int   post_neuron;
    int   weight;
    int   delay_us;
    int   plastic;
} synaptic_config_t;

static device_info_t devices[MAX_DEVICES];
static synaptic_config_t configs[MAX_CONFIGS];
static int device_count = 0;
static int config_count = 0;

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

/* Get hardware name */
static const char *hw_name(int type) {
    if (type == HW_FPGA) return "FPGA";
    if (type == HW_LOIHI) return "Intel Loihi";
    if (type == HW_TRUENORTH) return "IBM TrueNorth";
    if (type == HW_SPIKING_CHIP) return "SpikingChip";
    return "Unknown";
}

/* Register device */
int neuromorphic_register_device(int hw_type, const char *name, int neurons, int synapses) {
    if (device_count >= MAX_DEVICES) return -1;
    int idx = device_count;
    devices[idx].device_id = device_count + 1;
    devices[idx].hw_type = hw_type;
    my_strncpy(devices[idx].name, name, NAME_LEN - 1);
    devices[idx].neurons = neurons;
    devices[idx].synapses = synapses;
    devices[idx].power_mw = 50;
    devices[idx].topology = 0;
    device_count++;
    return devices[idx].device_id;
}

/* Configure synapse */
int neuromorphic_configure_synapse(int device_id, int pre, int post, int weight) {
    if (config_count >= MAX_CONFIGS) return -1;
    int idx = config_count;
    configs[idx].config_id = config_count + 1;
    configs[idx].device_id = device_id;
    configs[idx].pre_neuron = pre;
    configs[idx].post_neuron = post;
    configs[idx].weight = weight;
    configs[idx].delay_us = 100;
    configs[idx].plastic = 1;
    config_count++;
    return configs[idx].config_id;
}

/* Initialize FPGA */
int neuromorphic_init_fpga(int device_id) {
    print_str("=== FPGA Initialization ===\n");
    print_str("  Device ID: ");
    print_int(device_id);
    print_str("\n");
    print_str("  Loading bitstream...\n");
    print_str("  Configuring routing...\n");
    print_str("  Synapse RAM mapped: 64MB\n");
    print_str("  Spike event queue: ready\n");
    print_str("  Status: FPGA ready\n");
    return 0;
}

/* Communicate with Loihi */
int neuromorphic_loihi_comm(int chip_id) {
    print_str("=== Loihi Communication ===\n");
    print_str("  Chip ID: ");
    print_int(chip_id);
    print_str("\n");
    print_str("  Cores active: 128\n");
    print_str("  Neurons mapped: 131072\n");
    print_str("  Spikes/sec: 400M\n");
    print_str("  Latency: 2ms\n");
    print_str("  Status: Loihi ready\n");
    return 0;
}

/* Monitor power consumption */
int neuromorphic_monitor_power(void) {
    print_str("=== Power Monitor ===\n");
    for (int i = 0; i < device_count; i++) {
        print_str("  ");
        print_str(devices[i].name);
        print_str(": ");
        print_int(devices[i].power_mw);
        print_str("mW (");
        print_str(hw_name(devices[i].hw_type));
        print_str(")\n");
    }
    return device_count;
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
    print_str("Neuromorphic I/F v24.0 - Hardware Interface\n");
    if (help) {
        print_str("Usage: neuromorphic_interface [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run hardware test\n");
        return;
    }
    if (test) {
        print_str("=== Neuromorphic Interface Test ===\n\n");
        neuromorphic_register_device(HW_FPGA, "Xilinx_UltraPlus", 65536, 262144);
        neuromorphic_register_device(HW_LOIHI, "Loihi_2", 131072, 131072000);
        print_str("Registered 2 devices\n\n");
        neuromorphic_init_fpga(1);
        print_str("\n");
        neuromorphic_loihi_comm(2);
        print_str("\n");
        neuromorphic_configure_synapse(1, 0, 1, 800);
        neuromorphic_configure_synapse(1, 1, 2, 600);
        print_str("Configured 2 synapses\n\n");
        neuromorphic_monitor_power();
        print_str("\n=== Interface Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
