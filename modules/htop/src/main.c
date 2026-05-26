/* htop: interactive process viewer (simulated) */
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
    print_str("  Mem[|||||||||||         32/128MB]     Tasks: 23, 1 thr; 1 running\n");
    print_str("  Swp[                       0/0MB]     Load avg: 0.15 0.10 0.05\n");
    print_str("  CPU[|||               12.5%]           Uptime: 00:42:00\n\n");
    print_str("  PID  PRI  NI  VIRT   RES   SHR S  CPU%  MEM%   TIME+  Command\n");
    print_str("    1   20   0  12M   4.5M  2.3M S   0.3   3.5  0:05.12 /sbin/init\n");
    print_str("   42   20   0  23M   5.7M  3.5M S   1.2   4.4  0:10.34 /bin/shell\n");
    print_str("  100   20   0  34M   6.8M  4.6M S   0.7   5.2  0:02.56 /bin/syslog\n");
    print_str("  256   20   0  45M   7.9M  5.7M R   2.1   6.1  0:15.78 /bin/proc_monitor\n");
    print_str("  512   20   0  56M   8.1M  6.8M S   0.5   6.3  0:03.45 /bin/httpd\n");
    print_str("F1Help  F2Setup F3SearchF4FilterF5Tree F6SortByF7Nice -F8Nice +F9KillF10Quit\n");
    host_exit(0);
}
