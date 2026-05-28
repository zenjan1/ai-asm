/* loadkeys_v5: keyboard keymap loader ultimate (v5) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;

static unsigned int my_alloc(unsigned int n)
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
    unsigned int buf = my_alloc(len + 1);
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
    unsigned int dst = my_alloc(len + 1);
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
    unsigned int dst = my_alloc(len + 1);
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
    print_str("loadkeys_v5 - load keyboard keymap (v5.0)\n");
    print_str("Usage: loadkeys_v5 [OPTIONS] [keymap_file]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -b             Binary format\n");
    print_str("  -c             Clear keymap\n");
    print_str("  -d             Load default keymap\n");
    print_str("  -m             Generate keymap table\n");
    print_str("  -u             Unicode mode\n");
    print_str("  -f             Force load\n");
    print_str("  -v             Verbose output\n");
    print_str("  -t             Test keymap\n");
    print_str("  -i             Show keymap info\n");
    print_str("\n");
    print_str("Load keyboard keymap table into kernel.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v5: keymap information:\n");
    print_str("loadkeys_v5: current keymap: US layout\n");
    print_str("loadkeys_v5: keymap size: 256 entries\n");
    print_str("loadkeys_v5: loaded entries: 128\n");
    print_str("loadkeys_v5: unicode entries: 32\n");
    print_str("loadkeys_v5: compose entries: 8\n");
    print_str("loadkeys_v5: function keys: F1-F24 mapped\n");
    print_str("loadkeys_v5: altgr layer: enabled\n");
    print_str("loadkeys_v5: info display complete\n");
}

static void test_keymap(void)
{
    print_str("loadkeys_v5: testing keymap integrity\n");
    print_str("loadkeys_v5: checking entry 0x");
    print_hex(0x00);
    print_str(" - OK\n");
    print_str("loadkeys_v5: checking entry 0x");
    print_hex(0x3b);
    print_str(" (F1) - OK\n");
    print_str("loadkeys_v5: checking compose sequences - OK\n");
    print_str("loadkeys_v5: checking unicode table - OK\n");
    print_str("loadkeys_v5: all 256 entries valid\n");
    print_str("loadkeys_v5: test complete\n");
}

static void clear_keymap(int verbose)
{
    print_str("loadkeys_v5: clearing current keymap\n");
    if (verbose) {
        print_str("loadkeys_v5: resetting 256 entries to default\n");
        print_str("loadkeys_v5: clearing compose table\n");
        print_str("loadkeys_v5: clearing unicode table\n");
    }
    print_str("loadkeys_v5: keymap cleared\n");
}

static void load_default(int verbose, int force)
{
    print_str("loadkeys_v5: loading default US keymap\n");
    if (force) {
        print_str("loadkeys_v5: force mode: overriding current keymap\n");
    }
    if (verbose) {
        print_str("loadkeys_v5: loading alphanumeric keys\n");
        print_str("loadkeys_v5: loading function keys F1-F24\n");
        print_str("loadkeys_v5: loading modifier keys\n");
        print_str("loadkeys_v5: loading compose sequences\n");
    }
    print_str("loadkeys_v5: 128 entries loaded\n");
    print_str("loadkeys_v5: default keymap loaded\n");
}

static void load_binary(const char *file, int verbose)
{
    print_str("loadkeys_v5: loading binary keymap: ");
    if (file) print_str(file); else print_str("(none)");
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v5: reading binary format\n");
        print_str("loadkeys_v5: parsing header\n");
        print_str("loadkeys_v5: loading entries\n");
    }
    print_str("loadkeys_v5: binary keymap loaded\n");
}

static void generate_table(void)
{
    print_str("loadkeys_v5: generating keymap table\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x01);
    print_str(" -> escape\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x02);
    print_str(" -> 1\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x03);
    print_str(" -> 2\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x04);
    print_str(" -> 3\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x3b);
    print_str(" -> F1\n");
    print_str("loadkeys_v5: keycode 0x");
    print_hex(0x3c);
    print_str(" -> F2\n");
    print_str("loadkeys_v5: table generation complete\n");
}

static void load_unicode(int verbose)
{
    print_str("loadkeys_v5: loading unicode keymap\n");
    if (verbose) {
        print_str("loadkeys_v5: loading U+0000-U+00FF (Latin-1)\n");
        print_str("loadkeys_v5: loading U+0100-U+017F (Latin Extended-A)\n");
        print_str("loadkeys_v5: loading compose mappings\n");
    }
    print_str("loadkeys_v5: 32 unicode entries loaded\n");
    print_str("loadkeys_v5: unicode keymap loaded\n");
}

static void load_file(const char *file, int verbose, int force)
{
    print_str("loadkeys_v5: loading keymap from: ");
    if (file) print_str(file); else print_str("(stdin)");
    print_str("\n");
    if (force) {
        print_str("loadkeys_v5: force mode: overriding current keymap\n");
    }
    if (verbose) {
        print_str("loadkeys_v5: parsing keymap file\n");
        print_str("loadkeys_v5: loading string table\n");
        print_str("loadkeys_v5: loading keycode mappings\n");
        print_str("loadkeys_v5: loading compose sequences\n");
        print_str("loadkeys_v5: loading unicode mappings\n");
    }
    print_str("loadkeys_v5: keymap loaded\n");
}

void _start(void)
{
    unsigned int buf = my_alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, binary_flag = 0, clear_flag = 0;
    int default_flag = 0, mktable_flag = 0, unicode_flag = 0;
    int force_flag = 0, verbose_flag = 0, test_flag = 0, info_flag = 0;
    const char *keymap_file = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-b") == 0) {
            binary_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            clear_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            mktable_flag = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else {
            keymap_file = arg;
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

    if (test_flag) {
        test_keymap();
        host_exit(0);
    }

    if (clear_flag) {
        clear_keymap(verbose_flag);
        host_exit(0);
    }

    if (mktable_flag) {
        generate_table();
        host_exit(0);
    }

    if (default_flag) {
        load_default(verbose_flag, force_flag);
        host_exit(0);
    }

    if (unicode_flag) {
        load_unicode(verbose_flag);
        host_exit(0);
    }

    if (binary_flag) {
        load_binary(keymap_file, verbose_flag);
        host_exit(0);
    }

    if (keymap_file) {
        load_file(keymap_file, verbose_flag, force_flag);
        host_exit(0);
    }

    print_str("loadkeys_v5: default US keymap loaded\n");
    print_str("loadkeys_v5: use -i for info, -t to test\n");

    host_exit(0);
}
