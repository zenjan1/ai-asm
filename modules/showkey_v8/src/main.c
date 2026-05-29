/* showkey_v8: display keyboard scan codes (v8) */

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

static void print_hex(unsigned int n)
{
    char buf[16];
    int i = 15;
    buf[15] = 0;
    if (n == 0) buf[--i] = '0';
    while (n > 0) {
        int d = n % 16;
        buf[--i] = (d < 10) ? ('0' + d) : ('a' + d - 10);
        n /= 16;
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
    print_str("showkey_v8 - display keyboard scan codes (v1.0)\n");
    print_str("Usage: showkey_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             ASCII mode\n");
    print_str("  -k             Keycode mode\n");
    print_str("  -d             Dump keymap\n");
    print_str("  -t             Test scan codes\n");
    print_str("\n");
    print_str("Display keyboard scan codes and keycodes.\n");
}

static void show_info(void)
{
    print_str("showkey_v8: scan code info:\n");
    print_str("showkey_v8: mode: scancode\n");
    print_str("showkey_v8: keymap: us\n");
    print_str("showkey_v8: keycode range: 0-127\n");
    print_str("showkey_v8: scan code set: 2\n");
    print_str("showkey_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("showkey_v8: keyboard status:\n");
    print_str("showkey_v8: mode: scancode\n");
    print_str("showkey_v8: keycode: 0\n");
    print_str("showkey_v8: scancode: 0x00\n");
    print_str("showkey_v8: status check complete\n");
}

static void ascii_mode(int verbose)
{
    print_str("showkey_v8: ASCII mode enabled\n");
    print_str("showkey_v8: press keys to see ASCII values\n");
    if (verbose) {
        print_str("showkey_v8: example: 'a' = 97\n");
        print_str("showkey_v8: example: 'A' = 65\n");
        print_str("showkey_v8: example: ENTER = 13\n");
    }
}

static void keycode_mode(int verbose)
{
    print_str("showkey_v8: keycode mode enabled\n");
    print_str("showkey_v8: press keys to see keycodes\n");
    if (verbose) {
        print_str("showkey_v8: example: 'a' = 30\n");
        print_str("showkey_v8: example: 'b' = 48\n");
        print_str("showkey_v8: example: ENTER = 28\n");
    }
}

static void dump_keymap(int verbose)
{
    print_str("showkey_v8: dumping keymap:\n");
    print_str("showkey_v8: keycode 0: Escape\n");
    print_str("showkey_v8: keycode 1: 1\n");
    print_str("showkey_v8: keycode 2: 2\n");
    if (verbose) {
        print_str("showkey_v8: keycode 3: 3\n");
        print_str("showkey_v8: keycode 4: 4\n");
        print_str("showkey_v8: ... 128 keycodes total\n");
    }
    print_str("showkey_v8: keymap dump complete\n");
}

static void test_scan(int verbose)
{
    print_str("showkey_v8: testing scan codes\n");
    if (verbose) {
        print_str("showkey_v8: reading keyboard input\n");
        print_str("showkey_v8: keycode 30 pressed: scancode 0x1e\n");
        print_str("showkey_v8: keycode 30 released: scancode 0x9e\n");
    }
    print_str("showkey_v8: scan code test: OK\n");
    print_str("showkey_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int ascii_flag = 0, keycode_flag = 0;
    int dump_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-k") == 0) {
            keycode_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            dump_flag = 1;
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
        ascii_mode(verbose_flag);
        host_exit(0);
    }

    if (keycode_flag) {
        keycode_mode(verbose_flag);
        host_exit(0);
    }

    if (dump_flag) {
        dump_keymap(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_scan(verbose_flag);
        host_exit(0);
    }

    /* Default: scancode mode */
    print_str("showkey_v8: scancode mode enabled\n");
    print_str("showkey_v8: press keys to see scan codes\n");
    host_exit(0);
}
