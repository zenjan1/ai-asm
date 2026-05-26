/* vmstat: virtual memory statistics (simulated) */
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
        print_str("usage: vmstat [OPTION]...\n");
        print_str("Report virtual memory statistics.\n\nOptions:\n");
        print_str("  -h    display this help and exit\n");
        host_exit(0);
    }
    print_str("procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----\n");
    print_str(" r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st\n");
    print_str(" 1  0      0 524288  12345 234567    0    0   123   456  100  200  5  2 90  2  0\n");
    print_str(" 2  0      0 520000  12350 235000    0    0   130   460  110  210  6  3 88  2  0\n");
    print_str(" 1  0      0 518000  12400 236000    0    0   125   455  105  205  5  2 90  2  0\n");
    host_exit(0);
}
