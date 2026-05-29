/* openvt_v8: open virtual terminal (v8) */

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
    print_str("openvt_v8 - open virtual terminal (v1.0)\n");
    print_str("Usage: openvt_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n N           Specify VT number\n");
    print_str("  -c CMD         Run command on new VT\n");
    print_str("  -t             Test open VT\n");
    print_str("\n");
    print_str("Open a new virtual terminal and optionally start a command.\n");
}

static void show_info(void)
{
    print_str("openvt_v8: VT open info:\n");
    print_str("openvt_v8: max VTs: 12\n");
    print_str("openvt_v8: available VTs: 6\n");
    print_str("openvt_v8: next free VT: 7\n");
    print_str("openvt_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("openvt_v8: VT status:\n");
    print_str("openvt_v8: VT 1: active (console)\n");
    print_str("openvt_v8: VT 2: active\n");
    print_str("openvt_v8: VT 3-6: inactive\n");
    print_str("openvt_v8: VT 7-12: available\n");
    print_str("openvt_v8: status check complete\n");
}

static void open_vt(int vt_num, int verbose)
{
    print_str("openvt_v8: opening VT ");
    print_int(vt_num);
    print_str("\n");
    if (verbose) {
        print_str("openvt_v8: allocating VT ");
        print_int(vt_num);
        print_str(" resources\n");
        print_str("openvt_v8: initializing console\n");
        print_str("openvt_v8: setting up tty device\n");
    }
    print_str("openvt_v8: VT ");
    print_int(vt_num);
    print_str(" opened successfully\n");
}

static void run_command(const char *cmd, int verbose)
{
    print_str("openvt_v8: opening new VT with command: ");
    print_str(cmd);
    print_str("\n");
    if (verbose) {
        print_str("openvt_v8: allocating VT 7\n");
        print_str("openvt_v8: starting command: ");
        print_str(cmd);
        print_str("\n");
    }
    print_str("openvt_v8: command started on VT 7\n");
}

static void test_open(int verbose)
{
    print_str("openvt_v8: testing VT opening\n");
    if (verbose) {
        print_str("openvt_v8: finding free VT\n");
        print_str("openvt_v8: VT 7 available\n");
        print_str("openvt_v8: allocating resources\n");
    }
    print_str("openvt_v8: VT open test: OK\n");
    print_str("openvt_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int number_flag = 0, command_flag = 0;
    int test_flag = 0;
    int vt_number = 7;

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
        } else if (my_strcmp(arg, "-n") == 0) {
            number_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                vt_number = 0;
                char *num = &argv_ptr[pos];
                for (int i = 0; num[i] && num[i] >= '0' && num[i] <= '9'; i++) {
                    vt_number = vt_number * 10 + (num[i] - '0');
                }
            }
        } else if (my_strcmp(arg, "-c") == 0) {
            command_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
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

    if (command_flag) {
        run_command("/bin/sh", verbose_flag);
        host_exit(0);
    }

    if (number_flag) {
        open_vt(vt_number, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_open(verbose_flag);
        host_exit(0);
    }

    /* Default: open next available VT */
    print_str("openvt_v8: opening VT 7\n");
    print_str("openvt_v8: VT 7 opened successfully\n");
    host_exit(0);
}
