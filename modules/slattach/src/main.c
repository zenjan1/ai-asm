/* slattach: Attach a serial line to a network interface (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

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

    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len || (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h')) {
        print_str("Usage: slattach [ -cehlnopV ] [ -p protocol ] tty [ speed ]\n");
        print_str("Attach a network interface to a serial line.\n");
        print_str("\nOptions:\n");
        print_str("  -p proto  protocol (kiss, slip, adaptive, cisco)\n");
        print_str("  -c cmd    run command after setting up line\n");
        print_str("  -e        exit after initializing\n");
        print_str("  -h        show this help\n");
        print_str("  -l        use local line (no modem control)\n");
        print_str("  -n        don't initialize line to 8bit/NONE\n");
        print_str("  -o        set output speeds (same as input)\n");
        print_str("  -V        display version information\n");
        host_exit(1);
    }

    print_str("slattach: serial line attach simulation\n\n");
    print_str("Device: /dev/ttyS0\n");
    print_str("Protocol: SLIP\n");
    print_str("Speed: 38400 baud\n");
    print_str("Interface: sl0\n");
    print_str("\nSLIP configuration:\n");
    print_str("  MTU: 296\n");
    print_str("  Encapsulation: SLIP\n");
    print_str("  State: ATTACHED\n");
    print_str("  RX bytes: 0  TX bytes: 0\n");
    print_str("  RX packets: 0  TX packets: 0\n");
    print_str("  Errors: RX=0  TX=0  Overruns=0\n");
    print_str("\nSerial line attached to sl0 interface\n");

    host_exit(0);
}
