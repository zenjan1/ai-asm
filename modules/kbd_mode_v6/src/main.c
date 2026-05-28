/* kbd_mode_v6: keyboard mode setting (v6) */

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
    print_str("kbd_mode_v6 - keyboard mode setting (v6.0)\n");
    print_str("Usage: kbd_mode_v6 [OPTIONS]\n");
    print_str("  -s             Scancode mode\n");
    print_str("  -k             Keycode mode\n");
    print_str("  -a             ASCII mode\n");
    print_str("  -u             Unicode mode\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -d             Default mode\n");
    print_str("  -r             Raw mode\n");
    print_str("\n");
    print_str("Show or set the keyboard mode.\n");
}

static void show_info(void)
{
    print_str("kbd_mode_v6: keyboard mode info:\n");
    print_str("kbd_mode_v6: current mode: keycode (XLATE)\n");
    print_str("kbd_mode_v6: available modes: RAW, MEDIUMRAW, XLATE, UNICODE\n");
    print_str("kbd_mode_v6: compose key: disabled\n");
    print_str("kbd_mode_v6: dead keys: enabled\n");
    print_str("kbd_mode_v6: info display complete\n");
}

static void set_mode(const char *mode, int verbose)
{
    print_str("kbd_mode_v6: setting keyboard mode to ");
    if (mode) {
        unsigned int len = my_strlen(mode);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(mode, buf, len + 1);
        host_print(buf, len);
    } else {
        print_str("keycode");
    }
    print_str("\n");
    if (verbose) {
        print_str("kbd_mode_v6: checking current mode\n");
        print_str("kbd_mode_v6: saving old mode\n");
        print_str("kbd_mode_v6: switching keyboard driver\n");
        print_str("kbd_mode_v6: updating keymap\n");
        print_str("kbd_mode_v6: flushing input buffer\n");
    }
    print_str("kbd_mode_v6: mode applied successfully\n");
}

static void set_default(int verbose)
{
    print_str("kbd_mode_v6: resetting to default mode\n");
    if (verbose) {
        print_str("kbd_mode_v6: loading default keymap\n");
        print_str("kbd_mode_v6: setting XLATE mode\n");
        print_str("kbd_mode_v6: enabling dead keys\n");
    }
    print_str("kbd_mode_v6: default mode restored\n");
}

static void set_raw(int verbose)
{
    print_str("kbd_mode_v6: setting raw mode\n");
    if (verbose) {
        print_str("kbd_mode_v6: switching to RAW mode\n");
        print_str("kbd_mode_v6: scancodes will be passed directly\n");
        print_str("kbd_mode_v6: keymap processing disabled\n");
    }
    print_str("kbd_mode_v6: raw mode enabled\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, default_flag = 0;
    int raw_flag = 0, scancode_flag = 0;
    int keycode_flag = 0, ascii_flag = 0, unicode_flag = 0;

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
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-r") == 0) {
            raw_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            scancode_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            keycode_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
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

    if (default_flag) {
        set_default(verbose_flag);
        host_exit(0);
    }

    if (raw_flag) {
        set_raw(verbose_flag);
        host_exit(0);
    }

    if (scancode_flag) {
        set_mode("scancode", verbose_flag);
        host_exit(0);
    }

    if (keycode_flag) {
        set_mode("keycode", verbose_flag);
        host_exit(0);
    }

    if (ascii_flag) {
        set_mode("ascii", verbose_flag);
        host_exit(0);
    }

    if (unicode_flag) {
        set_mode("unicode", verbose_flag);
        host_exit(0);
    }

    /* Default: show info */
    show_info();
    host_exit(0);
}
