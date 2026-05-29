/* openvt_v10: run program on specified VT (v10) */

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
    print_str("openvt_v10 - run program on specified VT (v10)\n");
    print_str("Usage: openvt_v10 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -d /dev/ttyN   Target VT device\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Run a program on a specified virtual terminal.\n");
}

static void show_info(void)
{
    print_str("openvt_v10: VT program launch info:\n");
    print_str("openvt_v10: available VTs: 1-12\n");
    print_str("openvt_v10: current foreground VT: 2\n");
    print_str("openvt_v10: supported devices: /dev/tty1 - /dev/tty12\n");
    print_str("openvt_v10: info display complete\n");
}

static void show_status(void)
{
    print_str("openvt_v10: VT status:\n");
    print_str("openvt_v10: total VTs: 12\n");
    print_str("openvt_v10: active VTs: 2\n");
    print_str("openvt_v10: foreground VT: 2\n");
    print_str("openvt_v10: status check complete\n");
}

static void open_vt(const char *device, int verbose)
{
    print_str("openvt_v10: opening VT on device: ");
    print_str(device);
    print_str("\n");
    print_str("openvt_v10: VT opened successfully\n");
    if (verbose) {
        print_str("openvt_v10: switching to VT\n");
        print_str("openvt_v10: VT switch complete\n");
    }
}

static void test_mode(int verbose)
{
    print_str("openvt_v10: testing VT program launch\n");
    if (verbose) {
        print_str("openvt_v10: checking VT availability\n");
        print_str("openvt_v10: testing VT switch\n");
    }
    print_str("openvt_v10: VT launch test: OK\n");
    print_str("openvt_v10: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *device_arg = 0;

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
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                device_arg = &argv_ptr[pos];
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

    if (device_arg) {
        open_vt(device_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    /* Default: show usage */
    print_str("openvt_v10: no VT specified, use -d /dev/ttyN\n");
    host_exit(0);
}
