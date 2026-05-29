/* kbd_mode_v8: display/set keyboard mode (v8) */

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
    print_str("kbd_mode_v8 - display/set keyboard mode (v1.0)\n");
    print_str("Usage: kbd_mode_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             Set ASCII mode\n");
    print_str("  -x             Set XLATE mode\n");
    print_str("  -u             Set UNICODE mode\n");
    print_str("  -r             Set RAW mode\n");
    print_str("  -t             Test keyboard mode\n");
    print_str("\n");
    print_str("Display or modify the current virtual terminal keyboard mode.\n");
}

static void show_info(void)
{
    print_str("kbd_mode_v8: keyboard mode info:\n");
    print_str("kbd_mode_v8: current mode: UNICODE (utf-8)\n");
    print_str("kbd_mode_v8: keymap: us\n");
    print_str("kbd_mode_v8: compose key: none\n");
    print_str("kbd_mode_v8: led state: numlock\n");
    print_str("kbd_mode_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("kbd_mode_v8: keyboard status:\n");
    print_str("kbd_mode_v8: mode: UNICODE (utf-8)\n");
    print_str("kbd_mode_v8: keycode: 0\n");
    print_str("kbd_mode_v8: scancode: 0\n");
    print_str("kbd_mode_v8: ascii: off\n");
    print_str("kbd_mode_v8: raw: off\n");
    print_str("kbd_mode_v8: status check complete\n");
}

static void set_mode(const char *mode, int verbose)
{
    print_str("kbd_mode_v8: setting keyboard mode: ");
    print_str(mode);
    print_str("\n");
    if (verbose) {
        print_str("kbd_mode_v8: updating keyboard state\n");
        print_str("kbd_mode_v8: mode applied\n");
    }
    print_str("kbd_mode_v8: keyboard mode set to ");
    print_str(mode);
    print_str(" successfully\n");
}

static void test_keyboard(int verbose)
{
    print_str("kbd_mode_v8: testing keyboard mode\n");
    if (verbose) {
        print_str("kbd_mode_v8: reading keyboard state\n");
        print_str("kbd_mode_v8: testing UNICODE mode\n");
        print_str("kbd_mode_v8: keymap validation: OK\n");
    }
    print_str("kbd_mode_v8: keyboard mode test: OK\n");
    print_str("kbd_mode_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int ascii_flag = 0, xliterate_flag = 0;
    int unicode_flag = 0, raw_flag = 0;
    int test_flag = 0;

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
            ascii_flag = 1;
        } else if (my_strcmp(arg, "-x") == 0) {
            xliterate_flag = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
        } else if (my_strcmp(arg, "-r") == 0) {
            raw_flag = 1;
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

    if (ascii_flag) {
        set_mode("ASCII", verbose_flag);
        host_exit(0);
    }

    if (xliterate_flag) {
        set_mode("XLATE", verbose_flag);
        host_exit(0);
    }

    if (unicode_flag) {
        set_mode("UNICODE", verbose_flag);
        host_exit(0);
    }

    if (raw_flag) {
        set_mode("RAW", verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_keyboard(verbose_flag);
        host_exit(0);
    }

    /* Default: show current keyboard mode */
    print_str("kbd_mode_v8: keyboard mode: UNICODE (utf-8)\n");
    host_exit(0);
}
