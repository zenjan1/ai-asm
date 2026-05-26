/* top: process activity monitor (simulated) */
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
    print_str("top - 12:00:00 up 42 min,  1 user,  load average: 0.15, 0.10, 0.05\n");
    print_str("Tasks:  23 total,   1 running,  22 sleeping,   0 stopped,   0 zombie\n");
    print_str("%%Cpu(s):  5.2 us,  2.1 sy,  0.0 ni, 90.3 id,  1.8 wa,  0.0 hi,  0.6 si\n");
    print_str("MiB Mem :    128.0 total,     64.2 free,     32.1 used,     31.7 buff/cache\n");
    print_str("MiB Swap:      0.0 total,      0.0 free,      0.0 used.     92.4 avail Mem\n\n");
    print_str("  PID USER      PR  NI    VIRT    RES    SHR S  %%CPU  %%MEM     TIME+ COMMAND\n");
    print_str("    1 root      20   0   12345   4567   2345 S   0.3   3.5   0:05.12 init\n");
    print_str("   42 root      20   0   23456   5678   3456 S   1.2   4.4   0:10.34 shell\n");
    print_str("  100 root      20   0   34567   6789   4567 S   0.7   5.2   0:02.56 syslog\n");
    print_str("  256 root      20   0   45678   7890   5678 R   2.1   6.1   0:15.78 proc_monitor\n");
    host_exit(0);
}
