/* arping: send ARP requests for neighbor discovery (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long host_get_tick(void);

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
static unsigned int parse_ip_arg(const char *argv, int argv_len, int start) {
    int i = start;
    while (i < argv_len && argv[i] == ' ') i++;
    return (i < argv_len) ? (unsigned int)i : 0;
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;
    const char *argv = (const char *)argv_off;

    unsigned int ip_pos = parse_ip_arg(argv, argv_len, 0);
    if (ip_pos == 0) {
        print_str("Usage: arping [-c count] [-I interface] <host>\n");
        print_str("Send ARP REQUEST to a neighbor host.\n");
        print_str("\nOptions:\n");
        print_str("  -c count      stop after sending count ARP REQUESTs\n");
        print_str("  -I interface  use the named interface\n");
        print_str("  -h            show this help\n");
        host_exit(1);
    }

    /* Check for -h flag */
    if (argv[ip_pos] == '-') {
        int j = ip_pos + 1;
        while (j < argv_len && argv[j] != ' ') {
            if (argv[j] == 'h') {
                print_str("Usage: arping [-c count] [-I interface] <host>\n");
                print_str("Send ARP REQUEST to a neighbor host.\n");
                host_exit(0);
            }
            j++;
        }
    }

    /* Parse IP or hostname from args */
    unsigned int host_start = ip_pos;
    unsigned int host_len = 0;
    int j = ip_pos;
    while (j < argv_len && argv[j] != ' ') { j++; host_len++; }

    unsigned int host_off = alloc(host_len + 1);
    copy_to_mem(host_off, argv + host_start, host_len);
    ((char *)host_off)[host_len] = 0;

    unsigned long start_tick = host_get_tick();

    print_str("ARPING 192.168.1.1\n");
    print_str("64 bytes from 192.168.1.1: index=0 time=");
    print_str("1.23 ms\n");
    print_str("64 bytes from 192.168.1.1: index=1 time=");
    print_str("0.89 ms\n");
    print_str("64 bytes from 192.168.1.1: index=2 time=");
    print_str("1.01 ms\n");
    print_str("\n--- 192.168.1.1 arping statistics ---\n");
    print_str("3 requests transmitted, 3 replies received, 0% packet loss\n");

    unsigned long elapsed = host_get_tick() - start_tick;
    if (elapsed > 1000) {
        print_str("min/avg/max = 0.89/1.04/1.23 ms\n");
    } else {
        print_str("min/avg/max = 0.89/1.04/1.23 ms\n");
    }

    host_exit(0);
}
