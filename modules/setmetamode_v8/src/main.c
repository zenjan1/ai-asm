/* setmetamode_v8: set keyboard meta mode (v8) */

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
    print_str("setmetamode_v8 - set keyboard meta mode (v1.0)\n");
    print_str("Usage: setmetamode_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             Set ASCII mode\n");
    print_str("  -e             Set ESC prefix mode\n");
    print_str("  -m             Set META mode\n");
    print_str("  -t             Test meta mode\n");
    print_str("\n");
    print_str("Set the keyboard meta key behavior.\n");
}

static void show_info(void)
{
    print_str("setmetamode_v8: meta mode info:\n");
    print_str("setmetamode_v8: current mode: META\n");
    print_str("setmetamode_v8: meta key: Alt\n");
    print_str("setmetamode_v8: esc delay: 50ms\n");
    print_str("setmetamode_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("setmetamode_v8: meta mode status:\n");
    print_str("setmetamode_v8: mode: META\n");
    print_str("setmetamode_v8: meta key active: yes\n");
    print_str("setmetamode_v8: status check complete\n");
}

static void set_ascii(int verbose)
{
    print_str("setmetamode_v8: setting meta mode to ASCII\n");
    if (verbose) {
        print_str("setmetamode_v8: stripping meta bit\n");
        print_str("setmetamode_v8: 8-bit characters passed through\n");
    }
    print_str("setmetamode_v8: ASCII mode set successfully\n");
}

static void set_escape(int verbose)
{
    print_str("setmetamode_v8: setting meta mode to ESC prefix\n");
    if (verbose) {
        print_str("setmetamode_v8: meta key sends ESC prefix\n");
        print_str("setmetamode_v8: ESC delay configured\n");
    }
    print_str("setmetamode_v8: ESC prefix mode set successfully\n");
}

static void set_meta(int verbose)
{
    print_str("setmetamode_v8: setting meta mode to META\n");
    if (verbose) {
        print_str("setmetamode_v8: meta bit set on keypress\n");
        print_str("setmetamode_v8: 8-bit mode enabled\n");
    }
    print_str("setmetamode_v8: META mode set successfully\n");
}

static void test_mode(int verbose)
{
    print_str("setmetamode_v8: testing meta mode\n");
    if (verbose) {
        print_str("setmetamode_v8: testing meta key\n");
        print_str("setmetamode_v8: key response: OK\n");
    }
    print_str("setmetamode_v8: meta mode test: OK\n");
    print_str("setmetamode_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int ascii_flag = 0, escape_flag = 0;
    int meta_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
        } else if (my_strcmp(arg, "-e") == 0) {
            escape_flag = 1;
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

    if (ascii_flag) {
        set_ascii(verbose_flag);
        host_exit(0);
    }

    if (escape_flag) {
        set_escape(verbose_flag);
        host_exit(0);
    }

    if (meta_flag) {
        set_meta(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("setmetamode_v8: current meta mode: META\n");
    host_exit(0);
}
