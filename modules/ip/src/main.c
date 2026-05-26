/* ip: network interface management (simulated) */
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
        print_str("usage: ip [ OPTIONS ] OBJECT { COMMAND }\n");
        print_str("OBJECT := { link | addr | route }\n\nOptions:\n");
        print_str("  -h    display this help and exit\n");
        host_exit(0);
    }
    print_str("1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue\n");
    print_str("    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00\n");
    print_str("    inet 127.0.0.1/8 scope host lo\n");
    print_str("    inet6 ::1/128 scope host\n");
    print_str("2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast\n");
    print_str("    link/ether 52:54:00:12:34:56 brd ff:ff:ff:ff:ff:ff\n");
    print_str("    inet 192.168.1.100/24 brd 192.168.1.255 scope global eth0\n");
    print_str("    inet6 fe80::5054:ff:fe12:3456/64 scope link\n");
    print_str("default via 192.168.1.1 dev eth0\n");
    print_str("192.168.1.0/24 dev eth0 proto kernel scope link src 192.168.1.100\n");
    host_exit(0);
}
