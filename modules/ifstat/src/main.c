/* ifstat: network interface statistics (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long host_get_tick(void);

static unsigned int heap_pos = 65536;
static unsigned int alloc(unsigned int n) {
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) {
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}
static unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}
static void print_str(const char *s) {
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;
    const char *argv = (const char *)argv_off;

    /* Check for help flag */
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h') {
        print_str("Usage: ifstat [-t] [-i interface] [interval [count]]\n");
        print_str("Report interface statistics periodically.\n");
        print_str("\nOptions:\n");
        print_str("  -t           print timestamp\n");
        print_str("  -i iface     select interface\n");
        print_str("  -h           show this help\n");
        host_exit(0);
    }

    unsigned long start = host_get_tick();

    print_str("       eth0                eth1            \n");
    print_str("  KB/s in  KB/s out   KB/s in  KB/s out\n");
    print_str("  125.43    89.21      0.00      0.00\n");
    print_str("  132.67    95.44      0.00      0.00\n");
    print_str("  128.91    91.78      0.00      0.00\n");
    print_str("  145.32   102.56      0.00      0.00\n");
    print_str("  118.76    85.33      0.00      0.00\n");

    unsigned long elapsed = host_get_tick() - start;
    (void)elapsed;

    print_str("\n--- Interface Statistics ---\n");
    print_str("eth0: RX: 6424567 bytes  TX: 4523891 bytes\n");
    print_str("      RX packets: 5234  TX packets: 3891\n");
    print_str("      RX errors: 0  TX errors: 0\n");
    print_str("eth1: RX: 0 bytes  TX: 0 bytes\n");
    print_str("      RX packets: 0  TX packets: 0\n");
    print_str("      RX errors: 0  TX errors: 0\n");

    host_exit(0);
}
