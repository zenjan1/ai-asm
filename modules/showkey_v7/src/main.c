/* showkey_v7: display keyboard scan codes (v7) */

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
    print_str("showkey_v7 - display keyboard scan codes (v1.0)\n");
    print_str("Usage: showkey_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             Show ASCII mode\n");
    print_str("  -k             Show keycode mode\n");
    print_str("  -d N           Set display duration\n");
    print_str("  -t             Test key display\n");
    print_str("\n");
    print_str("Display keyboard scan codes and key codes.\n");
}

static void show_info(void)
{
    print_str("showkey_v7: keyboard info:\n");
    print_str("showkey_v7: mode: keycode\n");
    print_str("showkey_v7: scan codes: 128\n");
    print_str("showkey_v7: VT 1: keyboard active\n");
    print_str("showkey_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("showkey_v7: keyboard status:\n");
    print_str("showkey_v7: VT 1: listening\n");
    print_str("showkey_v7: last key: none\n");
    print_str("showkey_v7: status check complete\n");
}

static void ascii_mode(int verbose, int duration)
{
    print_str("showkey_v7: ASCII mode enabled\n");
    if (verbose) {
        print_str("showkey_v7: displaying ASCII codes\n");
        print_str("showkey_v7: duration: ");
        print_int(duration);
        print_str(" seconds\n");
    }
    print_str("showkey_v7: A=65  B=66  C=67  D=68  E=69\n");
    print_str("showkey_v7: a=97  b=98  c=99  d=100 e=101\n");
    print_str("showkey_v7: ASCII mode complete\n");
}

static void keycode_mode(int verbose, int duration)
{
    print_str("showkey_v7: keycode mode enabled\n");
    if (verbose) {
        print_str("showkey_v7: displaying keycodes\n");
        print_str("showkey_v7: duration: ");
        print_int(duration);
        print_str(" seconds\n");
    }
    print_str("showkey_v7: A=30  B=48  C=46  D=32  E=18\n");
    print_str("showkey_v7: F=33  G=34  H=35  I=23  J=36\n");
    print_str("showkey_v7: keycode mode complete\n");
}

static void test_keys(int verbose)
{
    print_str("showkey_v7: testing key display\n");
    if (verbose) {
        print_str("showkey_v7: simulating key presses\n");
        print_str("showkey_v7: checking scan code table\n");
    }
    print_str("showkey_v7: key test: A (scancode 30, keycode 30)\n");
    print_str("showkey_v7: key test: B (scancode 48, keycode 48)\n");
    print_str("showkey_v7: key test: C (scancode 46, keycode 46)\n");
    print_str("showkey_v7: key test: PASSED\n");
    print_str("showkey_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int ascii_flag = 0, keycode_flag = 0;
    int duration_flag = 0, test_flag = 0;
    int duration = 10;

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
            duration_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                duration = 0;
                char *num = &argv_ptr[pos];
                for (int i = 0; num[i] && num[i] >= '0' && num[i] <= '9'; i++) {
                    duration = duration * 10 + (num[i] - '0');
                }
            }
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
        ascii_mode(verbose_flag, duration);
        host_exit(0);
    }

    if (keycode_flag) {
        keycode_mode(verbose_flag, duration);
        host_exit(0);
    }

    if (test_flag) {
        test_keys(verbose_flag);
        host_exit(0);
    }

    /* Default: show keycode mode */
    print_str("showkey_v7: keycode mode (default)\n");
    print_str("showkey_v7: A=30  B=48  C=46  D=32  E=18\n");
    host_exit(0);
}
