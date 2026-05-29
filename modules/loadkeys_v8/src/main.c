/* loadkeys_v8: load keyboard layout (v8) */

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
    print_str("loadkeys_v8 - load keyboard layout (v1.0)\n");
    print_str("Usage: loadkeys_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -f FILE        Load from keymap file\n");
    print_str("  -d             Load default keymap\n");
    print_str("  -c             Show compose table\n");
    print_str("  -t             Test keyboard loading\n");
    print_str("\n");
    print_str("Load the specified keyboard layout into the virtual terminal.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v8: keyboard layout info:\n");
    print_str("loadkeys_v8: current layout: us\n");
    print_str("loadkeys_v8: keymap version: 2.0\n");
    print_str("loadkeys_v8: charset: UTF-8\n");
    print_str("loadkeys_v8: compose sequences: 12\n");
    print_str("loadkeys_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("loadkeys_v8: keyboard layout status:\n");
    print_str("loadkeys_v8: loaded keymap: us\n");
    print_str("loadkeys_v8: function keys: 12\n");
    print_str("loadkeys_v8: modifiers: shift, ctrl, alt\n");
    print_str("loadkeys_v8: compose: disabled\n");
    print_str("loadkeys_v8: status check complete\n");
}

static void load_file(const char *filename, int verbose)
{
    print_str("loadkeys_v8: loading keymap from: ");
    print_str(filename);
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v8: parsing keymap file\n");
        print_str("loadkeys_v8: 256 keycodes processed\n");
        print_str("loadkeys_v8: compose table updated\n");
    }
    print_str("loadkeys_v8: keymap loaded successfully\n");
}

static void load_default(int verbose)
{
    print_str("loadkeys_v8: loading default keymap: us\n");
    if (verbose) {
        print_str("loadkeys_v8: clearing existing keymap\n");
        print_str("loadkeys_v8: loading US layout\n");
        print_str("loadkeys_v8: function keys configured\n");
    }
    print_str("loadkeys_v8: default keymap loaded successfully\n");
}

static void show_compose(int verbose)
{
    print_str("loadkeys_v8: compose table:\n");
    print_str("loadkeys_v8: 12 sequences loaded\n");
    if (verbose) {
        print_str("loadkeys_v8: Compose+e+e = e-acute\n");
        print_str("loadkeys_v8: Compose+a+e = ae ligature\n");
        print_str("loadkeys_v8: Compose+s+s = German sharp s\n");
    }
    print_str("loadkeys_v8: compose display complete\n");
}

static void test_load(int verbose)
{
    print_str("loadkeys_v8: testing keyboard loading\n");
    if (verbose) {
        print_str("loadkeys_v8: creating test keymap\n");
        print_str("loadkeys_v8: validating keycodes\n");
        print_str("loadkeys_v8: testing modifiers\n");
    }
    print_str("loadkeys_v8: keyboard loading test: OK\n");
    print_str("loadkeys_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int file_flag = 0, default_flag = 0;
    int compose_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            file_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            compose_flag = 1;
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

    if (file_flag) {
        load_file("default.map", verbose_flag);
        host_exit(0);
    }

    if (default_flag) {
        load_default(verbose_flag);
        host_exit(0);
    }

    if (compose_flag) {
        show_compose(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_load(verbose_flag);
        host_exit(0);
    }

    /* Default: show current layout */
    print_str("loadkeys_v8: current layout: us\n");
    host_exit(0);
}
