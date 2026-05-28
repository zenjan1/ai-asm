/* dumpkeys_v4: keyboard mapping dumper (enhanced v4) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16];
    int i = 15;
    int neg = 0;
    buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b);
}

static void show_help(void)
{
    print_str("dumpkeys_v4 - dump keyboard mapping (v4.0)\n");
    print_str("Usage: dumpkeys_v4 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -f <fmt>       Output format (default, full, raw)\n");
    print_str("  -l <level>     Dump level (1-3)\n");
    print_str("  -n             Numeric keycodes\n");
    print_str("\n");
    print_str("Dump current keyboard translation table.\n");
}

static void dump_keymap_default(int numeric)
{
    print_str("dumpkeys_v4: keymap format: default\n");
    print_str("keymaps us\n");
    print_str("\n");
    print_str("dumpkeys_v4: keycode  1 = Escape\n");
    print_str("dumpkeys_v4: keycode  2 = one          exclam\n");
    print_str("dumpkeys_v4: keycode  3 = two          at\n");
    print_str("dumpkeys_v4: keycode  4 = three        numbersign\n");
    print_str("dumpkeys_v4: keycode  5 = four         dollar\n");
    print_str("dumpkeys_v4: keycode  6 = five         percent\n");
    print_str("dumpkeys_v4: keycode  7 = six          asciicircum\n");
    print_str("dumpkeys_v4: keycode  8 = seven        ampersand\n");
    print_str("dumpkeys_v4: keycode  9 = eight        asterisk\n");
    print_str("dumpkeys_v4: keycode 10 = nine         parenleft\n");
    print_str("dumpkeys_v4: keycode 11 = zero         parenright\n");
    if (numeric) {
        print_str("dumpkeys_v4: showing numeric codes: 128 entries\n");
    }
    print_str("dumpkeys_v4: dump complete\n");
}

static void dump_keymap_full(void)
{
    print_str("dumpkeys_v4: keymap format: full\n");
    print_str("keymaps us\n");
    print_str("\n");
    print_str("dumpkeys_v4: plain keymaps:\n");
    print_str("dumpkeys_v4: keycode   1: Escape (0x01)\n");
    print_str("dumpkeys_v4: keycode   2: 1 (0x31) ! (0x21)\n");
    print_str("dumpkeys_v4: keycode   3: 2 (0x32) @ (0x40)\n");
    print_str("dumpkeys_v4: keycode  14: BackSpace (0x08)\n");
    print_str("dumpkeys_v4: keycode  15: Tab (0x09)\n");
    print_str("dumpkeys_v4: keycode  24: q (0x71) Q (0x51)\n");
    print_str("dumpkeys_v4: keycode  25: w (0x77) W (0x57)\n");
    print_str("dumpkeys_v4: keycode  26: e (0x65) E (0x45)\n");
    print_str("dumpkeys_v4: keycode  27: r (0x72) R (0x52)\n");
    print_str("dumpkeys_v4: keycode  28: Return (0x0d)\n");
    print_str("dumpkeys_v4: alt keymaps:\n");
    print_str("dumpkeys_v4: keycode   2: Meta-one\n");
    print_str("dumpkeys_v4: keycode  14: Meta-BackSpace\n");
    print_str("dumpkeys_v4: compose table:\n");
    print_str("dumpkeys_v4:   0x27 0x63 -> 0xe7 (c-cedilla)\n");
    print_str("dumpkeys_v4:   0x2d 0x2d -> 0x96 (em-dash)\n");
    print_str("dumpkeys_v4: string table:\n");
    print_str("dumpkeys_v4:   F1 = \"\\033[[A\"\n");
    print_str("dumpkeys_v4:   F2 = \"\\033[[B\"\n");
    print_str("dumpkeys_v4:   F3 = \"\\033[[C\"\n");
    print_str("dumpkeys_v4: full dump complete\n");
}

static void dump_keymap_raw(void)
{
    print_str("dumpkeys_v4: keymap format: raw\n");
    print_str("dumpkeys_v4: raw keycode output:\n");
    for (int i = 1; i <= 60; i++) {
        print_str("dumpkeys_v4: keycode ");
        print_int(i);
        print_str(": ");
        print_int(i);
        print_str("\n");
    }
    print_str("dumpkeys_v4: raw dump complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int format_flag = 0;
    int level_flag = 0;
    int numeric_flag = 0;
    char *format_val = 0;
    char *level_val = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            format_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) format_val = &argv_ptr[pos];
        } else if (my_strcmp(arg, "-l") == 0) {
            level_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) level_val = &argv_ptr[pos];
        } else if (my_strcmp(arg, "-n") == 0) {
            numeric_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (format_flag && format_val) {
        if (my_strcmp(format_val, "full") == 0) {
            dump_keymap_full();
        } else if (my_strcmp(format_val, "raw") == 0) {
            dump_keymap_raw();
        } else {
            dump_keymap_default(numeric_flag);
        }
    } else {
        dump_keymap_default(numeric_flag);
    }

    host_exit(0);
}
