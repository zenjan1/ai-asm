/* dumpkeys_v6: keymap table dumping ultimate (v6) */

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
    print_str("dumpkeys_v6 - dump keyboard keymap (v6.0)\n");
    print_str("Usage: dumpkeys_v6 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -f <format>    Output format (plain/binary)\n");
    print_str("  -l <level>     Dump level (0-3)\n");
    print_str("  -n             Numeric output\n");
    print_str("  -H             Hexadecimal output\n");
    print_str("  -c             Include compose table\n");
    print_str("  -k             Include keycodes\n");
    print_str("  -s             Include strings\n");
    print_str("  -i             Show table info\n");
    print_str("\n");
    print_str("Dump current keyboard mapping table.\n");
}

static void show_info(void)
{
    print_str("dumpkeys_v6: keymap table information:\n");
    print_str("dumpkeys_v6: total entries: 256\n");
    print_str("dumpkeys_v6: active entries: 128\n");
    print_str("dumpkeys_v6: compose entries: 8\n");
    print_str("dumpkeys_v6: string table entries: 24\n");
    print_str("dumpkeys_v6: unicode entries: 32\n");
    print_str("dumpkeys_v6: modifier keys: 12\n");
    print_str("dumpkeys_v6: function keys: 24\n");
    print_str("dumpkeys_v6: info display complete\n");
}

static void dump_numeric(void)
{
    print_str("dumpkeys_v6: numeric keymap dump:\n");
    print_str("dumpkeys_v6: keycode 1 = 1\n");
    print_str("dumpkeys_v6: keycode 2 = 2\n");
    print_str("dumpkeys_v6: keycode 3 = 3\n");
    print_str("dumpkeys_v6: keycode 59 = F1\n");
    print_str("dumpkeys_v6: keycode 60 = F2\n");
    print_str("dumpkeys_v6: keycode 61 = F3\n");
    print_str("dumpkeys_v6: keycode 62 = F4\n");
    print_str("dumpkeys_v6: numeric dump complete: 128 entries\n");
}

static void dump_hex(void)
{
    print_str("dumpkeys_v6: hexadecimal keymap dump:\n");
    print_str("dumpkeys_v6: 0x");
    print_hex(0x01);
    print_str(" = 0x");
    print_hex(0x01);
    print_str("\n");
    print_str("dumpkeys_v6: 0x");
    print_hex(0x02);
    print_str(" = 0x");
    print_hex(0x02);
    print_str("\n");
    print_str("dumpkeys_v6: 0x");
    print_hex(0x03);
    print_str(" = 0x");
    print_hex(0x03);
    print_str("\n");
    print_str("dumpkeys_v6: 0x");
    print_hex(0x3b);
    print_str(" = 0x");
    print_hex(0x3b);
    print_str(" (F1)\n");
    print_str("dumpkeys_v6: 0x");
    print_hex(0x3c);
    print_str(" = 0x");
    print_hex(0x3c);
    print_str(" (F2)\n");
    print_str("dumpkeys_v6: hex dump complete: 256 entries\n");
}

static void dump_compose(void)
{
    print_str("dumpkeys_v6: compose table:\n");
    print_str("dumpkeys_v6: Compose + a = a with acute\n");
    print_str("dumpkeys_v6: Compose + e = e with acute\n");
    print_str("dumpkeys_v6: Compose + i = i with acute\n");
    print_str("dumpkeys_v6: Compose + o = o with acute\n");
    print_str("dumpkeys_v6: Compose + u = u with acute\n");
    print_str("dumpkeys_v6: Compose + n = n with tilde\n");
    print_str("dumpkeys_v6: Compose + c = c with cedilla\n");
    print_str("dumpkeys_v6: Compose + s = sharp s\n");
    print_str("dumpkeys_v6: compose table: 8 sequences\n");
}

static void dump_keycodes(void)
{
    print_str("dumpkeys_v6: keycode mapping:\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x01);
    print_str(" -> keycode 1 (Escape)\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x02);
    print_str(" -> keycode 2 (1)\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x03);
    print_str(" -> keycode 3 (2)\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x04);
    print_str(" -> keycode 4 (3)\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x1e);
    print_str(" -> keycode 30 (A)\n");
    print_str("dumpkeys_v6: scancode 0x");
    print_hex(0x3b);
    print_str(" -> keycode 59 (F1)\n");
    print_str("dumpkeys_v6: keycode dump complete: 128 entries\n");
}

static void dump_strings(void)
{
    print_str("dumpkeys_v6: string table:\n");
    print_str("dumpkeys_v6: F1 = \"\\033[[A\"\n");
    print_str("dumpkeys_v6: F2 = \"\\033[[B\"\n");
    print_str("dumpkeys_v6: F3 = \"\\033[[C\"\n");
    print_str("dumpkeys_v6: F4 = \"\\033[[D\"\n");
    print_str("dumpkeys_v6: F5 = \"\\033[[E\"\n");
    print_str("dumpkeys_v6: Up = \"\\033[A\"\n");
    print_str("dumpkeys_v6: Down = \"\\033[B\"\n");
    print_str("dumpkeys_v6: Left = \"\\033[D\"\n");
    print_str("dumpkeys_v6: Right = \"\\033[C\"\n");
    print_str("dumpkeys_v6: string table: 24 entries\n");
}

static void dump_format(const char *fmt)
{
    print_str("dumpkeys_v6: keymap dump (format: ");
    if (fmt) print_str(fmt); else print_str("plain");
    print_str(")\n");
    print_str("dumpkeys_v6: keycode 1 = Escape\n");
    print_str("dumpkeys_v6: keycode 2 = 1\n");
    print_str("dumpkeys_v6: keycode 3 = 2\n");
    print_str("dumpkeys_v6: keycode 59 = F1\n");
    print_str("dumpkeys_v6: format dump complete\n");
}

static void dump_level(int level)
{
    print_str("dumpkeys_v6: keymap dump (level ");
    print_int(level);
    print_str(")\n");
    if (level >= 0) print_str("dumpkeys_v6: basic keycodes\n");
    if (level >= 1) print_str("dumpkeys_v6: modifier keys\n");
    if (level >= 2) print_str("dumpkeys_v6: compose sequences\n");
    if (level >= 3) print_str("dumpkeys_v6: unicode mappings\n");
    print_str("dumpkeys_v6: level dump complete\n");
}

static void dump_all(void)
{
    print_str("dumpkeys_v6: full keymap dump:\n");
    print_str("dumpkeys_v6: keymaps 0-255\n");
    print_str("dumpkeys_v6: keycode 1 = Escape\n");
    print_str("dumpkeys_v6: keycode 2 = 1\n");
    print_str("dumpkeys_v6: keycode 3 = 2\n");
    print_str("dumpkeys_v6: keycode 4 = 3\n");
    print_str("dumpkeys_v6: keycode 5 = 4\n");
    print_str("dumpkeys_v6: keycode 59 = F1\n");
    print_str("dumpkeys_v6: keycode 60 = F2\n");
    print_str("dumpkeys_v6: keycode 61 = F3\n");
    print_str("dumpkeys_v6: dump complete: 256 entries\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, format_flag = 0, level_flag = 0;
    int numeric_flag = 0, hex_flag = 0, compose_flag = 0;
    int keycodes_flag = 0, strings_flag = 0, info_flag = 0;
    int level_val = 1;
    const char *format_val = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            numeric_flag = 1;
        } else if (my_strcmp(arg, "-H") == 0) {
            hex_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            compose_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            keycodes_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            strings_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            format_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) format_val = &argv_ptr[pos];
        } else if (my_strcmp(arg, "-l") == 0) {
            level_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { level_val = level_val * 10 + (*p - '0'); p++; }
            }
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (info_flag) {
        show_info();
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

    if (compose_flag) {
        dump_compose();
        host_exit(0);
    }

    if (keycodes_flag) {
        dump_keycodes();
        host_exit(0);
    }

    if (strings_flag) {
        dump_strings();
        host_exit(0);
    }

    if (format_flag) {
        dump_format(format_val);
        host_exit(0);
    }

    if (level_flag) {
        dump_level(level_val);
        host_exit(0);
    }

    dump_all();
    host_exit(0);
}
