/* pppd: Point-to-Point Protocol daemon (simulated) */
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
        print_str("Usage: pppd [ options ]\n");
        print_str("Point-to-Point Protocol daemon.\n");
        print_str("\nOptions:\n");
        print_str("  devname   tty device name\n");
        print_str("  speed     serial line speed\n");
        print_str("  local     don't use modem control lines\n");
        print_str("  debug     enable debugging output\n");
        print_str("  nodetach  don't detach from controlling terminal\n");
        print_str("  -h        show this help\n");
        host_exit(1);
    }

    print_str("pppd: PPP daemon simulation\n\n");
    print_str("Serial link: /dev/ttyS0 @ 115200 baud\n");
    print_str("Connection: ESTABLISHED\n");
    print_str("Protocol: PPP\n");
    print_str("Local IP: 192.168.1.100\n");
    print_str("Remote IP: 192.168.1.1\n");
    print_str("DNS: 8.8.8.8, 8.8.4.4\n");
    print_str("\nLCP: opened\n");
    print_str("PAP: authenticated\n");
    print_str("IPCP: opened\n");
    print_str("\nBytes received: 1234567  Bytes sent: 765432\n");
    print_str("Packets received: 8901  Packets sent: 4567\n");
    print_str("Errors: RX=0  TX=0\n");
    print_str("Uptime: 01:23:45\n");

    host_exit(0);
}
