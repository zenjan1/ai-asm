/* nameif: name network interfaces by MAC address (simulated) */
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
        print_str("Usage: nameif [-Vv] [-f config_file] [IFNAME MAC] ...\n");
        print_str("Assign network interface names based on MAC addresses.\n");
        print_str("\nOptions:\n");
        print_str("  -f FILE   read configuration from FILE\n");
        print_str("  -v        verbose output\n");
        print_str("  -h        show this help\n");
        host_exit(1);
    }

    print_str("nameif: interface naming results:\n");
    print_str("  eth0 (00:0c:29:a1:b2:c3) -> lan0: OK\n");
    print_str("  eth1 (00:0c:29:d4:e5:f6) -> wan0: OK\n");
    print_str("  eth2 (00:11:22:33:44:55) -> dmz0: OK\n");
    print_str("\nConfiguration applied successfully.\n");
    print_str("3 interfaces renamed.\n");

    host_exit(0);
}
