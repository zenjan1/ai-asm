/* mii-tool: view/manipulate MII register status (simulated) */
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

    if (i >= argv_len) {
        print_str("Usage: mii-tool [-VvRrwl] [-A duplex, ...] [interface ...]\n");
        print_str("Configure or monitor media-independent interface (MII) status.\n");
        print_str("\nOptions:\n");
        print_str("  -v, --verbose    show more information\n");
        print_str("  -r, --reset      reset MII to poweron state\n");
        print_str("  -R, --restart    restart autonegotiation\n");
        print_str("  -w, --watch      monitor link status\n");
        print_str("  -l, --log        log link status changes\n");
        print_str("  -A, --advertise  advertise specific capabilities\n");
        print_str("  -h, --help       show this help\n");
        host_exit(1);
    }

    print_str("eth0: negotiated 100baseTx-FD, link ok\n");
    print_str("\nMII registers for eth0:\n");
    print_str("  register 0x00: 0x3100  (Basic Mode Control)\n");
    print_str("  register 0x01: 0x7849  (Basic Mode Status)\n");
    print_str("  register 0x02: 0x0000  (PHY Identifier)\n");
    print_str("  register 0x03: 0x0000  (PHY Identifier)\n");
    print_str("  register 0x04: 0x05e1  (Advertisement Control)\n");
    print_str("  register 0x05: 0x45e1  (Link Partner Ability)\n");
    print_str("  register 0x06: 0x0000  (Expansion)\n");
    print_str("\nCapabilities: 10baseT-HD 10baseT-FD 100baseTx-HD 100baseTx-FD\n");
    print_str("Link detected: yes\n");
    print_str("Autonegotiation: on\n");
    print_str("Speed: 100 Mb/s\n");
    print_str("Duplex: Full\n");

    host_exit(0);
}
