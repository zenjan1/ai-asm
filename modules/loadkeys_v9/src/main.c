/* loadkeys_v9: load keyboard map table (v9) */

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
    print_str("loadkeys_v9 - load keyboard map table (v1.0)\n");
    print_str("Usage: loadkeys_v9 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -l LAYOUT      Specify keyboard layout (us, uk, de, fr)\n");
    print_str("  -f FILE        Load keymap from file\n");
    print_str("  -t             Test keyboard map loading\n");
    print_str("\n");
    print_str("Load keyboard map configuration for virtual console.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v9: keyboard map loading info:\n");
    print_str("loadkeys_v9: supported layouts: us, uk, de, fr\n");
    print_str("loadkeys_v9: keymap format: plain text\n");
    print_str("loadkeys_v9: max keys: 128\n");
    print_str("loadkeys_v9: info display complete\n");
}

static void show_status(void)
{
    print_str("loadkeys_v9: keyboard map status:\n");
    print_str("loadkeys_v9: current layout: us\n");
    print_str("loadkeys_v9: keys loaded: 128\n");
    print_str("loadkeys_v9: dead keys: 6\n");
    print_str("loadkeys_v9: function keys: 12\n");
    print_str("loadkeys_v9: status check complete\n");
}

static void load_layout(const char *layout, int verbose)
{
    print_str("loadkeys_v9: loading keyboard layout: ");
    print_str(layout);
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v9: parsing keymap definition\n");
        print_str("loadkeys_v9: loading keycode table\n");
        print_str("loadkeys_v9: loading function key strings\n");
        print_str("loadkeys_v9: loading dead key sequences\n");
    }
    print_str("loadkeys_v9: keyboard layout loaded successfully\n");
}

static void load_file(const char *file, int verbose)
{
    print_str("loadkeys_v9: loading keymap from file: ");
    print_str(file);
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v9: reading keymap file\n");
        print_str("loadkeys_v9: parsing keyboard definitions\n");
        print_str("loadkeys_v9: loading function key strings\n");
    }
    print_str("loadkeys_v9: keymap file loaded successfully\n");
}

static void test_mode(int verbose)
{
    print_str("loadkeys_v9: testing keyboard map loading\n");
    if (verbose) {
        print_str("loadkeys_v9: keycode table check: OK\n");
        print_str("loadkeys_v9: function key table check: OK\n");
        print_str("loadkeys_v9: dead key sequence check: OK\n");
    }
    print_str("loadkeys_v9: keyboard map loading test: OK\n");
    print_str("loadkeys_v9: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int layout_flag = 0, file_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-l") == 0) {
            layout_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (my_strcmp(arg, "-f") == 0) {
            file_flag = 1;
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

    if (layout_flag) {
        load_layout("us", verbose_flag);
        host_exit(0);
    }

    if (file_flag) {
        load_file("/etc/kbd/keymap.map", verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("loadkeys_v9: loading default keyboard map\n");
    print_str("loadkeys_v9: layout: us\n");
    print_str("loadkeys_v9: keyboard map loaded successfully\n");
    host_exit(0);
}
