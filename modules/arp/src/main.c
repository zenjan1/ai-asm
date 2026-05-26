/* arp: ARP table display simulation (simulated) */
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
        print_str("Usage: arp [-a] [-d addr] [-s addr hw_addr] [device]\n");
        print_str("ARP table manipulation utility.\n\n");
        print_str("Options:\n");
        print_str("  -a    display all entries\n");
        print_str("  -d    delete entry\n");
        print_str("  -s    set entry\n");
        print_str("  -h    show this help\n");
        host_exit(0);
    }
    print_str("Address                  HWtype  HWaddress           Flags Mask            Iface\n");
    print_str("192.168.1.1              ether   52:54:00:12:34:00   C                     eth0\n");
    print_str("192.168.1.50             ether   52:54:00:ab:cd:ef   C                     eth0\n");
    print_str("192.168.1.100            ether   52:54:00:12:34:56   C                     eth0\n");
    print_str("10.0.0.1                 ether   00:11:22:33:44:55   C                     eth1\n");
    host_exit(0);
}
