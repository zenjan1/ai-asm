/* tcpdump: packet capture simulation (simulated) */
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
    if (i < argv_len && argv[i] == '-') {
        print_str("Usage: tcpdump [-options] [expression]\n");
        print_str("TCP dump utility - packet analyzer.\n\n");
        print_str("Options:\n");
        print_str("  -i interface   listen on interface\n");
        print_str("  -c count       stop after count packets\n");
        print_str("  -n             don't convert addresses to names\n");
        print_str("  -h             show this help\n");
        host_exit(0);
    }
    print_str("tcpdump: verbose output suppressed, use -v for full protocol decode\n");
    print_str("listening on eth0, link-type EN10MB (Ethernet), snapshot length 262144 bytes\n");
    print_str("06:30:00.123456 IP 192.168.1.100.45678 > 93.184.216.34.80: Flags [S], seq 1234567890, win 64240, options [mss 1460], length 0\n");
    print_str("06:30:00.125678 IP 93.184.216.34.80 > 192.168.1.100.45678: Flags [S.], seq 987654321, ack 1234567891, win 65535, options [mss 1460], length 0\n");
    print_str("06:30:00.126789 IP 192.168.1.100.45678 > 93.184.216.34.80: Flags [.], ack 1, win 502, length 0\n");
    print_str("06:30:00.127890 IP 192.168.1.100.45678 > 93.184.216.34.80: Flags [P.], seq 1:75, ack 1, win 502, length 74: HTTP: GET / HTTP/1.1\n");
    print_str("06:30:00.234567 IP 93.184.216.34.80 > 192.168.1.100.45678: Flags [.], seq 1:1461, ack 75, win 501, length 1460: HTTP: HTTP/1.1 200 OK\n");
    print_str("5 packets captured\n");
    print_str("10 packets received by filter\n");
    print_str("0 packets dropped by kernel\n");
    host_exit(0);
}
