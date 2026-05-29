/* unicode_start_v12: enable Unicode mode (v12) */

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
    print_str("unicode_start_v12 - enable Unicode mode (v12)\n");
    print_str("Usage: unicode_start_v12 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -k             Enable Unicode keyboard\n");
    print_str("  -f <font>      Specify font\n");
    print_str("  -t             Test Unicode rendering\n");
    print_str("\n");
    print_str("Enable Unicode/UTF-8 mode for the virtual console.\n");
}

static void show_info(void)
{
    print_str("unicode_start_v12: Unicode mode info:\n");
    print_str("unicode_start_v12: current mode: UTF-8\n");
    print_str("unicode_start_v12: font: Lat2-Terminus16\n");
    print_str("unicode_start_v12: keymap: unicode\n");
    print_str("unicode_start_v12: screen: UTF-8 enabled\n");
    print_str("unicode_start_v12: info display complete\n");
}

static void show_status(void)
{
    print_str("unicode_start_v12: Unicode status:\n");
    print_str("unicode_start_v12: VT 1: UTF-8 mode active\n");
    print_str("unicode_start_v12: VT 2: UTF-8 mode available\n");
    print_str("unicode_start_v12: VT 3: UTF-8 mode available\n");
    print_str("unicode_start_v12: keyboard: Unicode input enabled\n");
    print_str("unicode_start_v12: status check complete\n");
}

static void enable_keyboard(int verbose)
{
    print_str("unicode_start_v12: enabling Unicode keyboard\n");
    if (verbose) {
        print_str("unicode_start_v12: loading Unicode keymap\n");
        print_str("unicode_start_v12: configuring compose sequences\n");
        print_str("unicode_start_v12: keyboard mode: UTF-8 input\n");
    }
    print_str("unicode_start_v12: Unicode keyboard enabled\n");
}

static void load_font(const char *font, int verbose)
{
    print_str("unicode_start_v12: loading font: ");
    print_str(font);
    print_str("\n");
    if (verbose) {
        print_str("unicode_start_v12: loading glyph table\n");
        print_str("unicode_start_v12: font metrics initialized\n");
    }
    print_str("unicode_start_v12: font loaded successfully\n");
}

static void test_unicode(int verbose)
{
    print_str("unicode_start_v12: testing Unicode rendering\n");
    if (verbose) {
        print_str("unicode_start_v12: loading test glyphs\n");
        print_str("unicode_start_v12: rendering test characters\n");
    }
    print_str("unicode_start_v12: Hello World - \xce\x93\xce\xb5\xce\xb9\xce\xac \xce\xa3\xce\xbf\xcf\x85 - \xe4\xb8\x96\xe7\x95\x8c\n");
    print_str("unicode_start_v12: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int keyboard_flag = 0, test_flag = 0;
    const char *font_name = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            keyboard_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                font_name = &argv_ptr[pos];
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

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (keyboard_flag) {
        enable_keyboard(verbose_flag);
        host_exit(0);
    }

    if (font_name) {
        load_font(font_name, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_unicode(verbose_flag);
        host_exit(0);
    }

    /* Default: enable Unicode mode */
    print_str("unicode_start_v12: enabling Unicode mode\n");
    print_str("unicode_start_v12: UTF-8 mode activated\n");
    host_exit(0);
}
