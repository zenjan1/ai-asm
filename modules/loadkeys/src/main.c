/* loadkeys: keyboard layout loader (simulated) */
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
        print_str("Usage: loadkeys [ -d ] keymap_file\n");
        print_str("Load keyboard translation table.\n");
        print_str("\nOptions:\n");
        print_str("  -d        restore default keymap\n");
        print_str("  -h        show this help\n");
        host_exit(1);
    }

    print_str("loadkeys: keyboard layout loading simulation\n\n");
    print_str("Loading keymap: us.map.gz\n");
    print_str("  Keyboard type: PC-style\n");
    print_str("  Layout: US English (qwerty)\n");
    print_str("  Variant: standard\n");
    print_str("  Backspace: delete (0x7f)\n");
    print_str("\nFunction key definitions:\n");
    print_str("  F1:  ESC[11~\n");
    print_str("  F2:  ESC[12~\n");
    print_str("  F3:  ESC[13~\n");
    print_str("  F4:  ESC[14~\n");
    print_str("  F5:  ESC[15~\n");
    print_str("  F6:  ESC[17~\n");
    print_str("  F7:  ESC[18~\n");
    print_str("  F8:  ESC[19~\n");
    print_str("  F9:  ESC[20~\n");
    print_str("  F10: ESC[21~\n");
    print_str("  F11: ESC[23~\n");
    print_str("  F12: ESC[24~\n");
    print_str("\nCompose key sequences: 34 loaded\n");
    print_str("Dead keys: grave, acute, circumflex, tilde, diaeresis\n");
    print_str("\nKeymap entries loaded: 128\n");
    print_str("Status: KEYMAP LOADED\n");

    host_exit(0);
}
