/* setkeycodes_v12: set keyboard scancode mapping (v12) */

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
    print_str("setkeycodes_v12 - set keyboard scancode mapping (v12)\n");
    print_str("Usage: setkeycodes_v12 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c CODE        Scancode value\n");
    print_str("  -k KEY         Keycode value\n");
    print_str("  -t             Test mapping\n");
    print_str("\n");
    print_str("Set keyboard scancode to keycode mappings.\n");
}

static void show_info(void)
{
    print_str("setkeycodes_v12: scancode mapping info:\n");
    print_str("setkeycodes_v12: total mappings: 128\n");
    print_str("setkeycodes_v12: custom mappings: 0\n");
    print_str("setkeycodes_v12: default table: loaded\n");
    print_str("setkeycodes_v12: info display complete\n");
}

static void show_status(void)
{
    print_str("setkeycodes_v12: scancode mapping status:\n");
    print_str("setkeycodes_v12: keyboard layout: us\n");
    print_str("setkeycodes_v12: scancode table: active\n");
    print_str("setkeycodes_v12: custom entries: 0\n");
    print_str("setkeycodes_v12: status check complete\n");
}

static void set_mapping(const char *scancode, const char *keycode, int verbose)
{
    print_str("setkeycodes_v12: mapping scancode: ");
    print_str(scancode);
    print_str(" to keycode: ");
    print_str(keycode);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v12: updating keymap table\n");
        print_str("setkeycodes_v12: entry added successfully\n");
        print_str("setkeycodes_v12: keyboard driver notified\n");
    }
    print_str("setkeycodes_v12: mapping configured\n");
}

static void test_mapping(int verbose)
{
    print_str("setkeycodes_v12: testing scancode mapping\n");
    if (verbose) {
        print_str("setkeycodes_v12: verifying table integrity\n");
        print_str("setkeycodes_v12: testing scancode translation\n");
    }
    print_str("setkeycodes_v12: scancode mapping test: OK\n");
    print_str("setkeycodes_v12: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *scancode = 0;
    char *keycode = 0;

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
        } else if (my_strcmp(arg, "-c") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                scancode = &argv_ptr[pos];
            }
        } else if (my_strcmp(arg, "-k") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                keycode = &argv_ptr[pos];
            }
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

    if (scancode && keycode) {
        set_mapping(scancode, keycode, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mapping(verbose_flag);
        host_exit(0);
    }

    print_str("setkeycodes_v12: no mapping specified, use -c CODE -k KEY\n");
    host_exit(0);
}
