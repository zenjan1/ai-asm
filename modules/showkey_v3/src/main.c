/* showkey_v3: keyboard key display simulation (enhanced v3) */
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
            print_str("Usage: showkey_v3 [ -s | -a | -p ]\n");
            print_str("Show keys pressed on the console.\n");
            print_str("\nOptions:\n");
            print_str("  -h  show this help\n");
            print_str("  -s  show scancodes (decimal)\n");
            print_str("  -a  show all codes (decimal, hex, octal)\n");
            print_str("  -p  show keycodes (default)\n");
            host_exit(1);
        }
    }

    print_str("showkey_v3: keyboard key display (enhanced v3)\n\n");
    print_str("Reading keypresses from /dev/tty...\n\n");
    print_str("keycode  78 pressed  (release)\n");
    print_str("keycode  30 pressed  (release)\n");
    print_str("keycode  48 pressed  (release)\n");
    print_str("keycode   1 pressed  (release)\n\n");
    print_str("Scancode table (mode: scancode -> keycode):\n");
    print_str("  0x1e -> 30 (KEY_A)\n");
    print_str("  0x1f -> 31 (KEY_S)\n");
    print_str("  0x20 -> 32 (KEY_D)\n");
    print_str("  0x21 -> 33 (KEY_F)\n");
    print_str("  0x22 -> 34 (KEY_G)\n");
    print_str("  0x23 -> 35 (KEY_H)\n");
    print_str("  0x24 -> 36 (KEY_J)\n");
    print_str("  0x25 -> 37 (KEY_K)\n\n");
    print_str("Active keymap: US (qwerty)\n");
    print_str("Shift states: 2 (normal, shifted)\n");
    print_str("Lock keys: NumLock=off CapsLock=off ScrollLock=off\n");
    print_str("\nStatus: listening...\n");

    host_exit(0);
}
