/* kbd_mode_v3: keyboard mode query/set simulation (enhanced v3) */
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
            print_str("Usage: kbd_mode_v3 [ -s MODE ]\n");
            print_str("Set or display the keyboard mode.\n");
            print_str("\nOptions:\n");
            print_str("  -h  show this help\n");
            print_str("  -s MODE  set mode (raw/mediumraw/xlate/unicode/utf8)\n");
            host_exit(1);
        }
    }

    print_str("kbd_mode_v3: keyboard mode query (enhanced v3)\n\n");
    print_str("Current keyboard mode: UTF8 (unicode)\n");
    print_str("  Mode: UTF8\n");
    print_str("  Description: Unicode keyboard mode enabled\n");
    print_str("  Compose: no\n");
    print_str("  Dead keys: yes\n");
    print_str("  Accent table: 12 entries\n");
    print_str("  Charset: UTF-8\n");
    print_str("\nAvailable modes:\n");
    print_str("  RAW       - Raw scancodes\n");
    print_str("  MEDIUMRAW - Semi-raw scancodes (1-2 bytes)\n");
    print_str("  XLATE     - Scancode translation (default)\n");
    print_str("  UNICODE   - Unicode keyboard mode\n");
    print_str("  UTF8      - UTF-8 keyboard mode\n");
    print_str("\nKeyboard mode: UTF8\n");
    print_str("Status: mode = utf8\n");
    host_exit(0);
}
