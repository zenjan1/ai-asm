/* unicode_stop: Unicode mode disabler */

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
    print_str("unicode_stop - disable Unicode support (v1.0)\n");
    print_str("Usage: unicode_stop [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             Disable on all VTs\n");
    print_str("  -f             Force disable\n");
    print_str("  -l             List Unicode-enabled VTs\n");
    print_str("  -t             Test ASCII mode\n");
    print_str("\n");
    print_str("Disable Unicode/UTF-8 mode for the virtual console.\n");
}

static void show_info(void)
{
    print_str("unicode_stop: Unicode disable info:\n");
    print_str("unicode_stop: current mode: UTF-8 (will switch to ASCII)\n");
    print_str("unicode_stop: font: will revert to default\n");
    print_str("unicode_stop: keymap: will revert to default\n");
    print_str("unicode_stop: info display complete\n");
}

static void show_status(void)
{
    print_str("unicode_stop: Unicode status before disable:\n");
    print_str("unicode_stop: VT 1: UTF-8 mode active\n");
    print_str("unicode_stop: VT 2: UTF-8 mode available\n");
    print_str("unicode_stop: VT 3: UTF-8 mode available\n");
    print_str("unicode_stop: keyboard: Unicode input enabled\n");
    print_str("unicode_stop: status check complete\n");
}

static void disable_all(int verbose, int force)
{
    print_str("unicode_stop: disabling Unicode on all VTs\n");
    if (force) {
        print_str("unicode_stop: force mode: overriding active sessions\n");
    }
    if (verbose) {
        print_str("unicode_stop: disabling VT 1\n");
        print_str("unicode_stop: disabling VT 2\n");
        print_str("unicode_stop: disabling VT 3\n");
        print_str("unicode_stop: disabling VT 4\n");
        print_str("unicode_stop: reverting keymaps\n");
    }
    print_str("unicode_stop: Unicode disabled on all VTs\n");
}

static void list_vts(int verbose)
{
    print_str("unicode_stop: Unicode-enabled virtual terminals:\n");
    print_str("unicode_stop: VT 1: UTF-8 active\n");
    print_str("unicode_stop: VT 2: UTF-8 available\n");
    print_str("unicode_stop: VT 3: UTF-8 available\n");
    if (verbose) {
        print_str("unicode_stop: total: 3 VTs with Unicode\n");
    }
    print_str("unicode_stop: list complete\n");
}

static void test_ascii(int verbose)
{
    print_str("unicode_stop: testing ASCII mode\n");
    if (verbose) {
        print_str("unicode_stop: switching to ASCII\n");
        print_str("unicode_stop: rendering test characters\n");
    }
    print_str("unicode_stop: Hello World - Test OK\n");
    print_str("unicode_stop: test complete\n");
}

static void disable_unicode(int verbose, int force)
{
    print_str("unicode_stop: disabling Unicode mode\n");
    if (force) {
        print_str("unicode_stop: force mode: clearing buffers\n");
    }
    if (verbose) {
        print_str("unicode_stop: saving current settings\n");
        print_str("unicode_stop: disabling UTF-8 mode\n");
        print_str("unicode_stop: loading default keymap\n");
        print_str("unicode_stop: reverting font\n");
        print_str("unicode_stop: flushing input buffer\n");
    }
    print_str("unicode_stop: Unicode mode disabled\n");
    print_str("unicode_stop: console now in ASCII mode\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int all_flag = 0, force_flag = 0;
    int list_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-a") == 0) {
            all_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
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

    if (all_flag) {
        disable_all(verbose_flag, force_flag);
        host_exit(0);
    }

    if (list_flag) {
        list_vts(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_ascii(verbose_flag);
        host_exit(0);
    }

    disable_unicode(verbose_flag, force_flag);
    host_exit(0);
}
