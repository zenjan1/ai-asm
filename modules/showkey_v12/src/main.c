/* showkey_v12: display keyboard key information (v12) */

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
    print_str("showkey_v12 - display keyboard key information (v12)\n");
    print_str("Usage: showkey_v12 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -k             Show keycodes\n");
    print_str("  -a             Show ASCII values\n");
    print_str("  -t             Test display\n");
    print_str("\n");
    print_str("Display information about keyboard keys.\n");
}

static void show_info(void)
{
    print_str("showkey_v12: keyboard key info:\n");
    print_str("showkey_v12: keyboard layout: us\n");
    print_str("showkey_v12: total keys: 128\n");
    print_str("showkey_v12: keymap loaded\n");
    print_str("showkey_v12: info display complete\n");
}

static void show_status(void)
{
    print_str("showkey_v12: keyboard status:\n");
    print_str("showkey_v12: mode: default\n");
    print_str("showkey_v12: repeat rate: 30\n");
    print_str("showkey_v12: delay: 250ms\n");
    print_str("showkey_v12: status check complete\n");
}

static void show_keycodes(int verbose)
{
    print_str("showkey_v12: keycode display mode\n");
    print_str("showkey_v12: press keys to see keycodes\n");
    if (verbose) {
        print_str("showkey_v12: scan code mode enabled\n");
        print_str("showkey_v12: press and release will be shown\n");
    }
    print_str("showkey_v12: keycode 28 pressed (Enter)\n");
    print_str("showkey_v12: keycode 28 released\n");
}

static void show_ascii(int verbose)
{
    print_str("showkey_v12: ASCII value display mode\n");
    print_str("showkey_v12: press keys to see ASCII values\n");
    if (verbose) {
        print_str("showkey_v12: octal and hex values shown\n");
    }
    print_str("showkey_v12: 'a' = 0x61 = 97\n");
    print_str("showkey_v12: 'A' = 0x41 = 65\n");
}

static void test_display(int verbose)
{
    print_str("showkey_v12: testing key display\n");
    if (verbose) {
        print_str("showkey_v12: checking keyboard handler\n");
        print_str("showkey_v12: testing key code translation\n");
    }
    print_str("showkey_v12: key display test: OK\n");
    print_str("showkey_v12: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int keycode_flag = 0, ascii_flag = 0;
    int test_flag = 0;

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
        } else if (my_strcmp(arg, "-k") == 0) {
            keycode_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
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

    if (keycode_flag) {
        show_keycodes(verbose_flag);
        host_exit(0);
    }

    if (ascii_flag) {
        show_ascii(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_display(verbose_flag);
        host_exit(0);
    }

    print_str("showkey_v12: press keys to see information\n");
    host_exit(0);
}
