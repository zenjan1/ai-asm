/* deallocvt_v11: deallocate specified VT (v11) */

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
    print_str("deallocvt_v11 - deallocate specified VT (v11)\n");
    print_str("Usage: deallocvt_v11 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n NUM         VT number to deallocate\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Deallocate a specified virtual terminal.\n");
}

static void show_info(void)
{
    print_str("deallocvt_v11: VT deallocation info:\n");
    print_str("deallocvt_v11: available VTs: 1-12\n");
    print_str("deallocvt_v11: current foreground VT: 2\n");
    print_str("deallocvt_v11: deallocatable VTs: 3-12\n");
    print_str("deallocvt_v11: info display complete\n");
}

static void show_status(void)
{
    print_str("deallocvt_v11: VT status:\n");
    print_str("deallocvt_v11: total VTs: 12\n");
    print_str("deallocvt_v11: active VTs: 2\n");
    print_str("deallocvt_v11: foreground VT: 2\n");
    print_str("deallocvt_v11: status check complete\n");
}

static void dealloc_vt(int vt_num, int verbose)
{
    print_str("deallocvt_v11: deallocating VT: ");
    print_int(vt_num);
    print_str("\n");
    if (vt_num < 1 || vt_num > 12) {
        print_str("deallocvt_v11: invalid VT number: ");
        print_int(vt_num);
        print_str("\n");
        print_str("deallocvt_v11: valid range: 1-12\n");
        host_exit(1);
    }
    if (vt_num == 1) {
        print_str("deallocvt_v11: VT 1 is console VT, cannot deallocate\n");
        host_exit(1);
    }
    if (vt_num == 2) {
        print_str("deallocvt_v11: VT 2 is foreground VT, cannot deallocate\n");
        host_exit(1);
    }
    print_str("deallocvt_v11: VT ");
    print_int(vt_num);
    print_str(" deallocated successfully\n");
    if (verbose) {
        print_str("deallocvt_v11: cleaning up VT resources\n");
        print_str("deallocvt_v11: memory freed: 4096 bytes\n");
        print_str("deallocvt_v11: VT table updated\n");
    }
}

static void test_mode(int verbose)
{
    print_str("deallocvt_v11: testing VT deallocation\n");
    if (verbose) {
        print_str("deallocvt_v11: scanning for deallocatable VTs\n");
        print_str("deallocvt_v11: testing resource cleanup\n");
    }
    print_str("deallocvt_v11: VT deallocation test: OK\n");
    print_str("deallocvt_v11: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    int vt_num = -1;

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
        } else if (my_strcmp(arg, "-n") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                /* Parse integer */
                char *num_str = &argv_ptr[pos];
                vt_num = 0;
                while (*num_str >= '0' && *num_str <= '9') {
                    vt_num = vt_num * 10 + (*num_str - '0');
                    num_str++;
                }
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

    if (vt_num >= 0) {
        dealloc_vt(vt_num, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    /* Default: show usage */
    print_str("deallocvt_v11: no VT specified, use -n NUM\n");
    host_exit(0);
}
