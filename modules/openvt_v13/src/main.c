/* openvt_v13: open new virtual terminal (v13) */

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
    print_str("openvt_v13 - open new virtual terminal (v13)\n");
    print_str("Usage: openvt_v13 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c N           VT number to use\n");
    print_str("  -l             Login shell\n");
    print_str("  -u USER        Run as user\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Open a new virtual terminal.\n");
}

static void show_info(void)
{
    print_str("openvt_v13: VT open info:\n");
    print_str("openvt_v13: available VTs: 1-12\n");
    print_str("openvt_v13: current foreground VT: 2\n");
    print_str("openvt_v13: next free VT: 7\n");
    print_str("openvt_v13: info display complete\n");
}

static void show_status(void)
{
    print_str("openvt_v13: VT status:\n");
    print_str("openvt_v13: total VTs: 12\n");
    print_str("openvt_v13: active VTs: 6\n");
    print_str("openvt_v13: foreground VT: 2\n");
    print_str("openvt_v13: status check complete\n");
}

static void open_vt(int vt_num, int login, const char *user, int verbose)
{
    print_str("openvt_v13: opening VT: ");
    print_int(vt_num);
    print_str("\n");
    if (login) {
        print_str("openvt_v13: login shell mode\n");
    }
    if (user) {
        print_str("openvt_v13: running as user: ");
        print_str(user);
        print_str("\n");
    }
    if (verbose) {
        print_str("openvt_v13: allocating VT resources\n");
        print_str("openvt_v13: initializing console\n");
        print_str("openvt_v13: VT opened successfully\n");
    }
}

static void test_mode(int verbose)
{
    print_str("openvt_v13: testing VT open\n");
    if (verbose) {
        print_str("openvt_v13: checking VT availability\n");
        print_str("openvt_v13: testing resource allocation\n");
    }
    print_str("openvt_v13: VT open test: OK\n");
    print_str("openvt_v13: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, login_flag = 0;
    int vt_num = -1;
    const char *user_arg = 0;

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
        } else if (my_strcmp(arg, "-l") == 0) {
            login_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                char *num_str = &argv_ptr[pos];
                vt_num = 0;
                while (*num_str >= '0' && *num_str <= '9') {
                    vt_num = vt_num * 10 + (*num_str - '0');
                    num_str++;
                }
            }
        } else if (my_strcmp(arg, "-u") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                user_arg = &argv_ptr[pos];
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

    if (vt_num > 0) {
        open_vt(vt_num, login_flag, user_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("openvt_v13: opening VT 7 (next available)\n");
    host_exit(0);
}
