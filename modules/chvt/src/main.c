/* chvt: virtual terminal switcher (simulated) */
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
        print_str("Usage: chvt N\n");
        print_str("Change the foreground virtual terminal to /dev/ttyN.\n");
        print_str("\nOptions:\n");
        print_str("  -h  show this help\n");
        print_str("\nExample: chvt 2  (switch to VT2)\n");
        host_exit(1);
    }

    print_str("chvt: virtual terminal switching simulation\n\n");
    print_str("Available virtual terminals: 1-6\n");
    print_str("  VT1: active (console)\n");
    print_str("  VT2: idle\n");
    print_str("  VT3: idle\n");
    print_str("  VT4: idle\n");
    print_str("  VT5: idle\n");
    print_str("  VT6: idle\n");
    print_str("\nSwitching to VT2...\n");
    print_str("  Previous VT: 1 (console)\n");
    print_str("  New VT: 2\n");
    print_str("  Mode: graphics (KD_GRAPHICS)\n");
    print_str("  Resolution: 1024x768\n");
    print_str("  Depth: 32bpp\n");
    print_str("\nVT switch completed\n");
    print_str("Status: ACTIVE TERMINAL = VT2\n");

    host_exit(0);
}
