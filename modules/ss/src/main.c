/* ss: socket statistics (simulated) */
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
    if (i < argv_len && argv[i] == '-' && i+1 < argv_len && argv[i+1] == 'h') {
        print_str("usage: ss [OPTION]...\n");
        print_str("Dump socket statistics.\n\nOptions:\n");
        print_str("  -h    display this help and exit\n  -t    TCP sockets\n  -u    UDP sockets\n");
        host_exit(0);
    }
    print_str("Netid  State   Recv-Q  Send-Q  Local Address:Port   Peer Address:Port\n");
    print_str("tcp    ESTAB   0       0       192.168.1.100:22     192.168.1.50:54321\n");
    print_str("tcp    LISTEN  0       128     0.0.0.0:80           0.0.0.0:*\n");
    print_str("tcp    ESTAB   0       0       192.168.1.100:443    10.0.0.5:12345\n");
    print_str("tcp    TIME-WAIT 0     0       192.168.1.100:8080   10.0.0.6:23456\n");
    print_str("udp    UNCONN  0       0       0.0.0.0:68           0.0.0.0:*\n");
    print_str("udp    ESTAB   0       0       192.168.1.100:53     8.8.8.8:53\n");
    host_exit(0);
}
