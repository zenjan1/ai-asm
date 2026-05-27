/* dumpkeys: keyboard map dumper (simulated) */
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
        print_str("Usage: dumpkeys [ --keys-only ] [ --funcs-only ]\n");
        print_str("Dump keyboard translation table.\n");
        print_str("\nOptions:\n");
        print_str("  --keys-only     show only key bindings\n");
        print_str("  --funcs-only    show only function key strings\n");
        print_str("  -h              show this help\n");
        host_exit(1);
    }

    print_str("dumpkeys: keyboard map dump simulation\n\n");
    print_str("keymaps 0-15\n");
    print_str("keycode  1 = Escape\n");
    print_str("keycode  2 = +Shift     1\n");
    print_str("keycode  3 = +Shift     2\n");
    print_str("keycode  4 = +Shift     3\n");
    print_str("keycode  5 = +Shift     4\n");
    print_str("keycode  6 = +Shift     5\n");
    print_str("keycode  7 = +Shift     6\n");
    print_str("keycode  8 = +Shift     7\n");
    print_str("keycode  9 = +Shift     8\n");
    print_str("keycode 10 = +Shift     9\n");
    print_str("keycode 11 = +Shift     0\n");
    print_str("keycode 12 = +Shift     minus\n");
    print_str("keycode 13 = +Shift     equal\n");
    print_str("keycode 14 = Delete\n");
    print_str("keycode 15 = Tab\n");
    print_str("keycode 16 = q Q\n");
    print_str("keycode 17 = w W\n");
    print_str("keycode 18 = e E\n");
    print_str("keycode 19 = r R\n");
    print_str("keycode 20 = t T\n");
    print_str("keycode 28 = Return\n");
    print_str("keycode 29 = Control\n");
    print_str("keycode 42 = Shift\n");
    print_str("keycode 56 = Alt\n");
    print_str("keycode 57 = Space\n");
    print_str("keycode 58 = CapsLock\n");
    print_str("\nstring F1 = \"\\033[[A\"\n");
    print_str("string F2 = \"\\033[[B\"\n");
    print_str("string F3 = \"\\033[[C\"\n");
    print_str("string F4 = \"\\033[[D\"\n");
    print_str("string F5 = \"\\033[[E\"\n");
    print_str("\nTotal keycodes: 128\n");
    print_str("Total function strings: 24\n");

    host_exit(0);
}
