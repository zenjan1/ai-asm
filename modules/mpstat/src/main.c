/* mpstat: CPU per-processor statistics (simulated) */
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
        print_str("usage: mpstat [OPTION]...\n");
        print_str("Report per-CPU statistics.\n\nOptions:\n");
        print_str("  -h    display this help and exit\n");
        host_exit(0);
    }
    print_str("Linux 5.10.0 (aarch64)  05/28/2026  _aarch64_ (4 CPU)\n\n");
    print_str("CPU    %usr   %nice    %sys  %iowait    %irq   %soft  %steal  %guest  %gnice   %idle\n");
    print_str("all    12.34    0.00    3.45    1.23    0.10    0.05    0.00    0.00    0.00   82.83\n");
    print_str("  0    15.67    0.00    4.56    0.89    0.12    0.06    0.00    0.00    0.00   78.70\n");
    print_str("  1     9.12    0.00    2.34    1.56    0.08    0.04    0.00    0.00    0.00   86.86\n");
    print_str("  2    11.89    0.00    3.21    1.12    0.11    0.05    0.00    0.00    0.00   83.62\n");
    print_str("  3    12.78    0.00    3.69    1.34    0.09    0.05    0.00    0.00    0.00   82.05\n");
    host_exit(0);
}
