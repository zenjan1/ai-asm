/* ethtool: ethernet device configuration simulation (simulated) */
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
        print_str("Usage: ethtool [options] <device>\n");
        print_str("Query or control network driver and hardware settings.\n\n");
        print_str("Options:\n");
        print_str("  -i    show driver information\n");
        print_str("  -S    show adapter statistics\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    if (argv[i] == '-') {
        i++;
        while (i < argv_len && argv[i] != ' ') {
            if (argv[i] == 'h') {
                print_str("Usage: ethtool [options] <device>\n");
                print_str("Query or control network driver and hardware settings.\n");
                host_exit(0);
            }
            i++;
        }
    }
    print_str("Settings for eth0:\n");
    print_str("\tSupported ports: [ TP ]\n");
    print_str("\tSupported link modes:   10baseT/Half 10baseT/Full\n");
    print_str("\t                        100baseT/Half 100baseT/Full\n");
    print_str("\t                        1000baseT/Full\n");
    print_str("\tSpeed: 1000Mb/s\n");
    print_str("\tDuplex: Full\n");
    print_str("\tPort: Twisted Pair\n");
    print_str("\tPHYAD: 0\n");
    print_str("\tLink detected: yes\n");
    print_str("\tAuto-negotiation: on\n");
    print_str("\tTransceiver: internal\n");
    print_str("\tSupports Wake-on: pumbg\n");
    print_str("\tWake-on: d\n");
    print_str("\tCurrent message level: 0x00000007 (7)\n");
    host_exit(0);
}
