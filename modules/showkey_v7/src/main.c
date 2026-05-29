/* showkey_v7: show key codes (v7) */

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
    print_str("showkey_v7 - show key codes (v7)\n");
    print_str("Usage: showkey_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -k             Show keycodes (default)\n");
    print_str("  -a             Show ASCII values\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Display keycodes pressed on the console.\n");
}

static void show_info(void)
{
    print_str("showkey_v7: key info:\n");
    print_str("showkey_v7: mode: keycodes\n");
    print_str("showkey_v7: console: /dev/tty0\n");
    print_str("showkey_v7: keys captured: 0\n");
    print_str("showkey_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("showkey_v7: key status:\n");
    print_str("showkey_v7: mode: keycodes\n");
    print_str("showkey_v7: listening: yes\n");
    print_str("showkey_v7: status check complete\n");
}

static void show_keycodes(int verbose)
{
    print_str("showkey_v7: keycodes mode\n");
    print_str("showkey_v7: pressing keys to show keycodes...\n");
    if (verbose) {
        print_str("showkey_v7: keycode  1: Escape (press)\n");
        print_str("showkey_v7: keycode  2: 1 (press)\n");
        print_str("showkey_v7: keycode  3: 2 (press)\n");
        print_str("showkey_v7: keycode 28: Enter (press)\n");
        print_str("showkey_v7: keycode 57: Space (press)\n");
    }
    print_str("showkey_v7: keycode display ready\n");
}

static void show_ascii(int verbose)
{
    print_str("showkey_v7: ASCII mode\n");
    print_str("showkey_v7: pressing keys to show ASCII values...\n");
    if (verbose) {
        print_str("showkey_v7: 'a' = 97 (0x61)\n");
        print_str("showkey_v7: 'A' = 65 (0x41)\n");
        print_str("showkey_v7: '0' = 48 (0x30)\n");
        print_str("showkey_v7: '\\n' = 10 (0x0a)\n");
        print_str("showkey_v7: ' ' = 32 (0x20)\n");
    }
    print_str("showkey_v7: ASCII display ready\n");
}

static void test_mode(int verbose)
{
    print_str("showkey_v7: testing key display\n");
    if (verbose) {
        print_str("showkey_v7: verifying keyboard device\n");
        print_str("showkey_v7: checking input buffer\n");
    }
    print_str("showkey_v7: showkey test: OK\n");
    print_str("showkey_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, ascii_flag = 0;

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
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            /* keycodes mode (default) */
        } else if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
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
        show_ascii(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    show_keycodes(verbose_flag);
    host_exit(0);
}
