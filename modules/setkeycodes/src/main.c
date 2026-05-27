/* setkeycodes: keyboard scancode to keycode mapping (simulated) */
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
        print_str("Usage: setkeycodes scancode keycode ...\n");
        print_str("Set kernel scancode-to-keycode mapping table entries.\n");
        print_str("\nOptions:\n");
        print_str("  -h        show this help\n");
        print_str("\nExample: setkeycodes e030 120\n");
        print_str("  Maps scancode 0xe030 to keycode 120\n");
        host_exit(1);
    }

    print_str("setkeycodes: keyboard scancode mapping simulation\n\n");
    print_str("Loading scancode-to-keycode mappings:\n");
    print_str("  0x001 -> 1   (ESC)\n");
    print_str("  0x002 -> 2   (1)\n");
    print_str("  0x003 -> 3   (2)\n");
    print_str("  0x004 -> 4   (3)\n");
    print_str("  0x005 -> 5   (4)\n");
    print_str("  0x006 -> 6   (5)\n");
    print_str("  0x007 -> 7   (6)\n");
    print_str("  0x008 -> 8   (7)\n");
    print_str("  0x009 -> 9   (8)\n");
    print_str("  0x00a -> 10  (9)\n");
    print_str("  0x00b -> 11  (0)\n");
    print_str("  0x00c -> 12  (-)\n");
    print_str("  0x00d -> 13  (=)\n");
    print_str("  0x00e -> 14  (BACKSPACE)\n");
    print_str("  0x00f -> 15  (TAB)\n");
    print_str("  0x010 -> 16  (Q)\n");
    print_str("  0x011 -> 17  (W)\n");
    print_str("  0x012 -> 18  (E)\n");
    print_str("  0x013 -> 19  (R)\n");
    print_str("  0x014 -> 20  (T)\n");
    print_str("  0x01c -> 28  (ENTER)\n");
    print_str("  0x03a -> 58  (CAPSLOCK)\n");
    print_str("  0x01d -> 29  (LEFTCTRL)\n");
    print_str("  0x02a -> 42  (LEFTSHIFT)\n");
    print_str("  0x038 -> 56  (LEFTALT)\n");
    print_str("  0x039 -> 57  (SPACE)\n");
    print_str("\nExtended scancodes (0xe0 prefix):\n");
    print_str("  0xe01d -> 97   (RIGHTCTRL)\n");
    print_str("  0xe038 -> 100  (RIGHTALT)\n");
    print_str("  0xe05b -> 125  (META)\n");
    print_str("\nKeymap entries loaded: 27\n");
    print_str("Status: MAPPING TABLE LOADED\n");

    host_exit(0);
}
