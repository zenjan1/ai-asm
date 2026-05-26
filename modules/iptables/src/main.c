/* iptables: firewall management simulation (simulated) */
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
        i++;
        while (i < argv_len && argv[i] != ' ') {
            if (argv[i] == 'h') {
                print_str("Usage: iptables [-t table] -[ACD] chain rule-specification [options]\n");
                print_str("iptables firewall administration.\n\n");
                print_str("Options:\n");
                print_str("  -t table    table to manipulate (filter, nat, mangle)\n");
                print_str("  -L          list rules\n");
                print_str("  -A          append rule\n");
                print_str("  -D          delete rule\n");
                print_str("  -h          show this help\n");
                host_exit(0);
            }
            i++;
        }
    }
    print_str("Chain INPUT (policy ACCEPT)\n");
    print_str("target     prot opt source               destination\n");
    print_str("ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0\n");
    print_str("DROP       tcp  --  10.0.0.0/8           0.0.0.0/0            tcp dpt:23\n");
    print_str("ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:22\n");
    print_str("ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:80\n");
    print_str("ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:443\n");
    print_str("\nChain FORWARD (policy DROP)\n");
    print_str("target     prot opt source               destination\n");
    print_str("ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0\n");
    print_str("\nChain OUTPUT (policy ACCEPT)\n");
    print_str("target     prot opt source               destination\n");
    print_str("ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0\n");
    host_exit(0);
}
