/* ping6: IPv6 connectivity testing simulation (simulated) */
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
        print_str("Usage: ping6 [options] <host>\n");
        print_str("IPv6 network connectivity testing.\n\n");
        print_str("Options:\n");
        print_str("  -c count    stop after count packets\n");
        print_str("  -h          show this help\n");
        host_exit(1);
    }
    if (argv[i] == '-') {
        print_str("Usage: ping6 [options] <host>\n");
        print_str("IPv6 network connectivity testing.\n");
        host_exit(0);
    }
    int name_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    print_str("PING google.com (2607:f8b0:4004:800::200e) 56 data bytes\n");
    print_str("64 bytes from 2607:f8b0:4004:800::200e: icmp_seq=1 ttl=117 time=12.3 ms\n");
    print_str("64 bytes from 2607:f8b0:4004:800::200e: icmp_seq=2 ttl=117 time=11.8 ms\n");
    print_str("64 bytes from 2607:f8b0:4004:800::200e: icmp_seq=3 ttl=117 time=12.1 ms\n");
    print_str("64 bytes from 2607:f8b0:4004:800::200e: icmp_seq=4 ttl=117 time=11.9 ms\n");
    print_str("\n--- google.com ping statistics ---\n");
    print_str("4 packets transmitted, 4 received, 0% packet loss, time 3005ms\n");
    print_str("rtt min/avg/max/mdev = 11.8/12.0/12.3/0.2 ms\n");
    host_exit(0);
}
