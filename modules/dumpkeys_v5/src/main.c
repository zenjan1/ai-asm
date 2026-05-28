/* dumpkeys_v5: keymap dumping ultimate (v5) */

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

static void print_hex(int n)
{
    char buf[8];
    int i = 7;
    buf[7] = 0;
    if (n == 0) buf[--i] = '0';
    while (n > 0) {
        int d = n & 0xf;
        buf[--i] = (d < 10) ? ('0' + d) : ('a' + d - 10);
        n >>= 4;
    }
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
    print_str("dumpkeys_v5 - dump keymap (v5.0)\n");
    print_str("Usage: dumpkeys_v5 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -f             Full dump with all details\n");
    print_str("  -l             Linear format\n");
    print_str("  -n             Numeric keycodes\n");
    print_str("  -H             Hex format\n");
    print_str("  -c             Character set only\n");
    print_str("\n");
    print_str("Dump current keyboard translation table.\n");
}

static void dump_full(void)
{
    print_str("dumpkeys_v5: full keymap dump:\n");
    print_str("keymaps us\n");
    print_str("\n");
    print_str("dumpkeys_v5: plain keymaps:\n");
    for (int i = 1; i <= 58; i++) {
        print_str("dumpkeys_v5: keycode ");
        print_int(i);
        print_str(": assigned\n");
    }
    print_str("dumpkeys_v5: compose table:\n");
    print_str("dumpkeys_v5:   0x27 0x63 -> 0xe7\n");
    print_str("dumpkeys_v5:   0x2d 0x2d -> 0x96\n");
    print_str("dumpkeys_v5: string table:\n");
    print_str("dumpkeys_v5:   F1 = \"\\033[[A\"\n");
    print_str("dumpkeys_v5:   F2 = \"\\033[[B\"\n");
    print_str("dumpkeys_v5:   F3 = \"\\033[[C\"\n");
    print_str("dumpkeys_v5:   F4 = \"\\033[[D\"\n");
    print_str("dumpkeys_v5: full dump complete\n");
}

static void dump_linear(void)
{
    print_str("dumpkeys_v5: linear keymap dump:\n");
    print_str("dumpkeys_v5: Escape 1 2 3 4 5 6 7 8 9 0 - = BackSpace\n");
    print_str("dumpkeys_v5: Tab q w e r t y u i o p [ ] Return\n");
    print_str("dumpkeys_v5: Control a s d f g h j k l ; ' `\n");
    print_str("dumpkeys_v5: Shift \\ z x c v b n m , . / Shift\n");
    print_str("dumpkeys_v5: linear dump complete\n");
}

static void dump_numeric(void)
{
    print_str("dumpkeys_v5: numeric keymap dump:\n");
    for (int i = 1; i <= 127; i++) {
        print_str("dumpkeys_v5: keycode ");
        print_int(i);
        print_str(": ");
        print_int(i);
        print_str("\n");
    }
    print_str("dumpkeys_v5: numeric dump complete\n");
}

static void dump_hex(void)
{
    print_str("dumpkeys_v5: hex keymap dump:\n");
    for (int i = 1; i <= 60; i++) {
        print_str("dumpkeys_v5: keycode ");
        print_int(i);
        print_str(": 0x");
        print_hex(i);
        print_str("\n");
    }
    print_str("dumpkeys_v5: hex dump complete\n");
}

static void dump_charset(void)
{
    print_str("dumpkeys_v5: character set dump:\n");
    print_str("dumpkeys_v5: charset: latin-1\n");
    print_str("dumpkeys_v5: printable chars: 95\n");
    print_str("dumpkeys_v5: control chars: 33\n");
    print_str("dumpkeys_v5: extended chars: 128\n");
    print_str("dumpkeys_v5: charset dump complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int full_flag = 0;
    int linear_flag = 0;
    int numeric_flag = 0;
    int hex_flag = 0;
    int charset_flag = 0;

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
            full_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            linear_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            numeric_flag = 1;
        } else if (my_strcmp(arg, "-H") == 0) {
            hex_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            charset_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (full_flag) {
        dump_full();
        host_exit(0);
    }

    if (linear_flag) {
        dump_linear();
        host_exit(0);
    }

    if (numeric_flag) {
        dump_numeric();
        host_exit(0);
    }

    if (hex_flag) {
        dump_hex();
        host_exit(0);
    }

    if (charset_flag) {
        dump_charset();
        host_exit(0);
    }

    /* Default: summary */
    print_str("dumpkeys_v5: current keymap: us.map\n");
    print_str("dumpkeys_v5: 128 keycode entries\n");
    print_str("dumpkeys_v5: use -f for full dump, -l for linear\n");

    host_exit(0);
}
