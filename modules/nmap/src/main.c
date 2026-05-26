/* nmap: network scanning simulation (simulated) */
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
        print_str("Usage: nmap <target>\n");
        print_str("Network scanner simulation.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    if (i < argv_len && argv[i] == '-') {
        print_str("Usage: nmap <target>\n");
        print_str("Network scanner simulation.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(0);
    }
    print_str("Starting Nmap scan...\n");
    print_str("Nmap scan report for 192.168.1.1\n");
    print_str("Host is up (0.0032s latency).\n");
    print_str("PORT    STATE  SERVICE\n");
    print_str("22/tcp  open   ssh\n");
    print_str("80/tcp  open   http\n");
    print_str("443/tcp open   https\n");
    print_str("Nmap scan report for 192.168.1.100\n");
    print_str("Host is up (0.00010s latency).\n");
    print_str("PORT    STATE  SERVICE\n");
    print_str("22/tcp  open   ssh\n");
    print_str("80/tcp  open   http\n");
    print_str("8080/tcp open  http-proxy\n");
    print_str("Nmap done: 256 IP addresses (2 hosts up) scanned in 3.42 seconds\n");
    host_exit(0);
}
