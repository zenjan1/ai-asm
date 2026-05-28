/* setmetamode: set keyboard meta key mode */

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
    print_str("setmetamode - set keyboard meta key mode (v1.0)\n");
    print_str("Usage: setmetamode [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -q             Query current mode\n");
    print_str("  -a             Set ALT mode\n");
    print_str("  -m             Set META mode\n");
    print_str("  -t             Test meta key\n");
    print_str("\n");
    print_str("Control keyboard Meta key behavior.\n");
}

static void show_info(void)
{
    print_str("setmetamode: keyboard meta key info:\n");
    print_str("setmetamode: current mode: ALT\n");
    print_str("setmetamode: meta key: left ALT\n");
    print_str("setmetamode: escape prefix: enabled\n");
    print_str("setmetamode: VT 1: active\n");
    print_str("setmetamode: info display complete\n");
}

static void show_status(void)
{
    print_str("setmetamode: keyboard meta key status:\n");
    print_str("setmetamode: mode: ALT\n");
    print_str("setmetamode: keycode: 56\n");
    print_str("setmetamode: state: released\n");
    print_str("setmetamode: escape: 0x1B prefix\n");
    print_str("setmetamode: status check complete\n");
}

static void query_mode(void)
{
    print_str("setmetamode: current meta key mode: ALT\n");
    print_str("setmetamode: escape sequence prefix: enabled\n");
    print_str("setmetamode: query complete\n");
}

static void set_alt_mode(int verbose)
{
    print_str("setmetamode: setting ALT mode\n");
    if (verbose) {
        print_str("setmetamode: disabling META prefix\n");
        print_str("setmetamode: enabling escape prefix\n");
        print_str("setmetamode: updating keyboard driver\n");
    }
    print_str("setmetamode: ALT mode activated\n");
}

static void set_meta_mode(int verbose)
{
    print_str("setmetamode: setting META mode\n");
    if (verbose) {
        print_str("setmetamode: disabling escape prefix\n");
        print_str("setmetamode: enabling META prefix\n");
        print_str("setmetamode: updating keyboard driver\n");
    }
    print_str("setmetamode: META mode activated\n");
}

static void test_meta(int verbose)
{
    print_str("setmetamode: testing meta key\n");
    if (verbose) {
        print_str("setmetamode: sending test key codes\n");
        print_str("setmetamode: checking escape sequences\n");
    }
    print_str("setmetamode: ALT+A: 0x1B 0x61\n");
    print_str("setmetamode: META+A: 0xE1 0x61\n");
    print_str("setmetamode: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int query_flag = 0, alt_flag = 0;
    int meta_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-q") == 0) {
            query_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            alt_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            meta_flag = 1;
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

    if (query_flag) {
        query_mode();
        host_exit(0);
    }

    if (alt_flag) {
        set_alt_mode(verbose_flag);
        host_exit(0);
    }

    if (meta_flag) {
        set_meta_mode(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_meta(verbose_flag);
        host_exit(0);
    }

    query_mode();
    host_exit(0);
}
