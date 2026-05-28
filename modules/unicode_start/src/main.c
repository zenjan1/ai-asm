/* unicode_start: Unicode mode enabler */

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
    print_str("unicode_start - enable Unicode support (v1.0)\n");
    print_str("Usage: unicode_start [OPTIONS] [FONT [MAP]]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -d             Disable Unicode mode\n");
    print_str("  -f <font>      Specify font\n");
    print_str("  -l             List available fonts\n");
    print_str("  -t             Test Unicode rendering\n");
    print_str("\n");
    print_str("Enable Unicode/UTF-8 mode for the virtual console.\n");
}

static void show_info(void)
{
    print_str("unicode_start: Unicode mode info:\n");
    print_str("unicode_start: current mode: UTF-8\n");
    print_str("unicode_start: font: Lat2-Terminus16\n");
    print_str("unicode_start: keymap: unicode\n");
    print_str("unicode_start: screen: UTF-8 enabled\n");
    print_str("unicode_start: info display complete\n");
}

static void show_status(void)
{
    print_str("unicode_start: Unicode status:\n");
    print_str("unicode_start: VT 1: UTF-8 mode active\n");
    print_str("unicode_start: VT 2: UTF-8 mode available\n");
    print_str("unicode_start: VT 3: UTF-8 mode available\n");
    print_str("unicode_start: keyboard: Unicode input enabled\n");
    print_str("unicode_start: status check complete\n");
}

static void disable_unicode(void)
{
    print_str("unicode_start: disabling Unicode mode\n");
    print_str("unicode_start: reverting to ASCII mode\n");
    print_str("unicode_start: Unicode mode disabled\n");
}

static void list_fonts(int verbose)
{
    print_str("unicode_start: available Unicode fonts:\n");
    print_str("unicode_start: Lat2-Terminus16 (8x16)\n");
    print_str("unicode_start: LatArHeb-16 (8x16)\n");
    print_str("unicode_start: Uni3-Terminus16 (8x16)\n");
    print_str("unicode_start: sun12x22 (12x22)\n");
    print_str("unicode_start: vga8x16 (8x16)\n");
    if (verbose) {
        print_str("unicode_start: total: 5 fonts\n");
    }
    print_str("unicode_start: list complete\n");
}

static void test_unicode(int verbose)
{
    print_str("unicode_start: testing Unicode rendering\n");
    if (verbose) {
        print_str("unicode_start: loading test glyphs\n");
        print_str("unicode_start: rendering test characters\n");
    }
    print_str("unicode_start: Hello World - \xce\x93\xce\xb5\xce\xb9\xce\xac \xce\xa3\xce\xbf\xcf\x85 - \xe4\xb8\x96\xe7\x95\x8c\n");
    print_str("unicode_start: test complete\n");
}

static void enable_unicode(const char *font, int verbose)
{
    print_str("unicode_start: enabling Unicode mode\n");
    if (font) {
        print_str("unicode_start: loading font: ");
        unsigned int len = my_strlen(font);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(font, buf, len + 1);
        host_print(buf, len);
        print_str("\n");
    }
    if (verbose) {
        print_str("unicode_start: setting UTF-8 mode\n");
        print_str("unicode_start: loading Unicode keymap\n");
        print_str("unicode_start: initializing console driver\n");
        print_str("unicode_start: configuring input method\n");
    }
    print_str("unicode_start: Unicode mode enabled\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int disable_flag = 0, list_flag = 0;
    int test_flag = 0;
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
        } else if (my_strcmp(arg, "-d") == 0) {
            disable_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                font_name = &argv_ptr[pos];
            }
        } else {
            font_name = arg;
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

    if (disable_flag) {
        disable_unicode();
        host_exit(0);
    }

    if (list_flag) {
        list_fonts(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_unicode(verbose_flag);
        host_exit(0);
    }

    enable_unicode(font_name, verbose_flag);
    host_exit(0);
}
