/* fgconsole_v3: foreground console query simulation (enhanced v3) */
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

    if (argv_len > 0) {
        const char *argv = (const char *)argv_off;
        int i = 0;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
        if (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h') {
            print_str("Usage: fgconsole_v3\n");
            print_str("Print the number of the foreground virtual terminal.\n");
            print_str("\nOptions:\n");
            print_str("  -h  show this help\n");
            host_exit(1);
        }
    }

    print_str("fgconsole_v3: foreground console query (enhanced v3)\n\n");
    print_str("Querying foreground VT...\n");
    print_str("  VT number: 1\n");
    print_str("  Device: /dev/tty1\n");
    print_str("  State: active\n");
    print_str("  Mode: text (80x25)\n");
    print_str("  PID: 42 (init)\n");
    print_str("  Uptime: 3600 seconds\n\n");
    print_str("Console info:\n");
    print_str("  Type: virtual terminal\n");
    print_str("  Driver: vt\n");
    print_str("  Max VTs: 12\n");
    print_str("  Active VTs: 1, 2\n\n");
    print_str("1\n");
    print_str("Status: done\n");
    host_exit(0);
}
