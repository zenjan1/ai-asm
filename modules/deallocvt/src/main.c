/* deallocvt: virtual terminal deallocator (simulated) */
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
        print_str("Usage: deallocvt [ N ]\n");
        print_str("Deallocate unused virtual terminal /dev/ttyN.\n");
        print_str("\nOptions:\n");
        print_str("  N         VT number to deallocate (default: all unused)\n");
        print_str("  -h        show this help\n");
        host_exit(1);
    }

    print_str("deallocvt: virtual terminal deallocation simulation\n\n");
    print_str("Scanning virtual terminals...\n");
    print_str("  VT1: active (console) - retained\n");
    print_str("  VT2: idle - deallocated\n");
    print_str("  VT3: idle - deallocated\n");
    print_str("  VT4: idle - deallocated\n");
    print_str("  VT5: idle - deallocated\n");
    print_str("  VT6: idle - deallocated\n");
    print_str("  VT7: has process (PID 1234) - retained\n");
    print_str("\nDeallocated 5 virtual terminals\n");
    print_str("Freed memory: 81920 bytes\n");
    print_str("Retained VTs: 2 (VT1, VT7)\n");
    print_str("Status: DEALLOCATION COMPLETE\n");

    host_exit(0);
}
