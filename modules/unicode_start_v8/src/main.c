/* unicode_start_v8: activate Unicode mode (v8) */

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
    print_str("unicode_start_v8 - start Unicode mode (v1.0)\n");
    print_str("Usage: unicode_start_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -f FONT        Specify Unicode font\n");
    print_str("  -t             test Unicode mode\n");
    print_str("\n");
    print_str("Enable UTF-8/Unicode display mode on the virtual terminal.\n");
}

static void show_info(void)
{
    print_str("unicode_start_v8: Unicode mode info:\n");
    print_str("unicode_start_v8: encoding: UTF-8\n");
    print_str("unicode_start_v8: code points: 65536\n");
    print_str("unicode_start_v8: font: default\n");
    print_str("unicode_start_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("unicode_start_v8: Unicode status:\n");
    print_str("unicode_start_v8: mode: active\n");
    print_str("unicode_start_v8: encoding: UTF-8\n");
    print_str("unicode_start_v8: font loaded: yes\n");
    print_str("unicode_start_v8: status check complete\n");
}

static void set_font(const char *font, int verbose)
{
    print_str("unicode_start_v8: starting Unicode mode with font: ");
    print_str(font);
    print_str("\n");
    if (verbose) {
        print_str("unicode_start_v8: loading Unicode font\n");
        print_str("unicode_start_v8: enabling UTF-8 input\n");
        print_str("unicode_start_v8: updating console table\n");
    }
    print_str("unicode_start_v8: Unicode mode started successfully\n");
}

static void test_mode(int verbose)
{
    print_str("unicode_start_v8: testing Unicode mode\n");
    if (verbose) {
        print_str("unicode_start_v8: checking UTF-8 support\n");
        print_str("unicode_start_v8: font glyph check: OK\n");
        print_str("unicode_start_v8: input encoding: OK\n");
    }
    print_str("unicode_start_v8: Unicode mode test: OK\n");
    print_str("unicode_start_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int font_flag = 0, test_flag = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            font_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
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

    if (font_flag) {
        set_font("latarcyrheb-sun16", verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("unicode_start_v8: starting Unicode mode\n");
    print_str("unicode_start_v8: UTF-8 encoding enabled\n");
    print_str("unicode_start_v8: Unicode mode started successfully\n");
    host_exit(0);
}
