/* showkey: keyboard key display (simulated) */
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
        print_str("Usage: showkey [ -a | -s | -k ]\n");
        print_str("Examine keyboard input.\n");
        print_str("\nOptions:\n");
        print_str("  -a  display decimal, octal and hex values of key\n");
        print_str("  -s  display raw scan-codes (default)\n");
        print_str("  -k  display keycodes\n");
        print_str("  -h  show this help\n");
        host_exit(1);
    }

    print_str("showkey: keyboard key display simulation\n\n");
    print_str("Mode: scan-code display\n");
    print_str("Press keys to see their scan codes. Ctrl-D to exit.\n\n");
    print_str("Sample keypresses (simulated):\n");
    print_str("  keycode  28 press   (Enter)\n");
    print_str("  keycode  28 release (Enter)\n");
    print_str("  keycode  30 press   (a)\n");
    print_str("  keycode  30 release (a)\n");
    print_str("  keycode  48 press   (b)\n");
    print_str("  keycode  48 release (b)\n");
    print_str("  keycode  46 press   (c)\n");
    print_str("  keycode  46 release (c)\n");
    print_str("  keycode   1 press   (Escape)\n");
    print_str("  keycode   1 release (Escape)\n");
    print_str("  keycode  56 press   (Alt)\n");
    print_str("  keycode  57 press   (Space)\n");
    print_str("  keycode  57 release (Space)\n");
    print_str("  keycode  56 release (Alt)\n");
    print_str("\nTotal events: 14\n");
    print_str("Status: MONITORING KEYBOARD INPUT\n");

    host_exit(0);
}
