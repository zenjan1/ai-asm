/* chvt_v5: VT switching ultimate (v5) */

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
    print_str("chvt_v5 - switch virtual terminal (v5.0)\n");
    print_str("Usage: chvt_v5 [OPTIONS] [N]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -n <N>         Switch to VT N\n");
    print_str("  -p             Switch to previous VT\n");
    print_str("  -f             Switch to foreground VT\n");
    print_str("  -i             Show VT info\n");
    print_str("\n");
    print_str("Switch active virtual terminal.\n");
}

static void switch_to(int n)
{
    print_str("chvt_v5: switching to VT");
    print_int(n);
    print_str("\n");
    print_str("chvt_v5: deactivating current VT\n");
    print_str("chvt_v5: loading VT");
    print_int(n);
    print_str(" framebuffer\n");
    print_str("chvt_v5: VT");
    print_int(n);
    print_str(" activated\n");
    print_str("chvt_v5: console switch complete\n");
}

static void switch_previous(void)
{
    print_str("chvt_v5: switching to previous VT: VT1\n");
    print_str("chvt_v5: VT1 activated\n");
    print_str("chvt_v5: console switch complete\n");
}

static void switch_foreground(void)
{
    print_str("chvt_v5: foreground VT is VT7\n");
    print_str("chvt_v5: switching to VT7\n");
    print_str("chvt_v5: VT7 activated\n");
    print_str("chvt_v5: console switch complete\n");
}

static void show_info(void)
{
    print_str("chvt_v5: virtual terminal info:\n");
    print_str("chvt_v5: current VT: 7\n");
    print_str("chvt_v5: active VTs: 1-7\n");
    print_str("chvt_v5: available VTs: 1-12\n");
    print_str("chvt_v5: VT1: Linux console (text mode)\n");
    print_str("chvt_v5: VT2: Linux console (text mode)\n");
    print_str("chvt_v5: VT3: Linux console (text mode)\n");
    print_str("chvt_v5: VT4: Linux console (text mode)\n");
    print_str("chvt_v5: VT5: Linux console (text mode)\n");
    print_str("chvt_v5: VT6: Linux console (text mode)\n");
    print_str("chvt_v5: VT7: X11 / GUI (graphics mode)\n");
    print_str("chvt_v5: VT mode: text 80x25\n");
    print_str("chvt_v5: keyboard mode: XLATE\n");
    print_str("chvt_v5: info display complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int number_flag = 0;
    int previous_flag = 0;
    int foreground_flag = 0;
    int info_flag = 0;
    int number_val = 0;
    char *arg_val = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            number_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) arg_val = &argv_ptr[pos];
        } else if (my_strcmp(arg, "-p") == 0) {
            previous_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            foreground_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
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

    if (foreground_flag) {
        switch_foreground();
        host_exit(0);
    }

    if (previous_flag) {
        switch_previous();
        host_exit(0);
    }

    if (number_flag && arg_val) {
        /* Parse number */
        number_val = 0;
        while (*arg_val >= '0' && *arg_val <= '9') {
            number_val = number_val * 10 + (*arg_val - '0');
            arg_val++;
        }
        switch_to(number_val);
        host_exit(0);
    }

    /* Default: show current */
    print_str("chvt_v5: current VT: 7\n");
    print_str("chvt_v5: use -n N to switch, -i for info\n");

    host_exit(0);
}
