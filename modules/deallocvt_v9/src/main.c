/* deallocvt_v9: deallocate virtual terminal session (v9) */

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
    print_str("deallocvt_v9 - deallocate virtual terminal (v9)\n");
    print_str("Usage: deallocvt_v9 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -a             Deallocate all unused VTs\n");
    print_str("  -n N           Specify VT number\n");
    print_str("  -t             Test deallocation\n");
    print_str("\n");
    print_str("Release specified virtual terminal sessions.\n");
}

static void show_info(void)
{
    print_str("deallocvt_v9: VT deallocation info:\n");
    print_str("deallocvt_v9: max VTs: 12\n");
    print_str("deallocvt_v9: active VTs: 6\n");
    print_str("deallocvt_v9: unused VTs: 6\n");
    print_str("deallocvt_v9: VT 1: console (active)\n");
    print_str("deallocvt_v9: info display complete\n");
}

static void show_status(void)
{
    print_str("deallocvt_v9: VT allocation status:\n");
    print_str("deallocvt_v9: VT 1: active (console)\n");
    print_str("deallocvt_v9: VT 2: active\n");
    print_str("deallocvt_v9: VT 3: inactive\n");
    print_str("deallocvt_v9: VT 4: inactive\n");
    print_str("deallocvt_v9: VT 5: inactive\n");
    print_str("deallocvt_v9: VT 6: inactive\n");
    print_str("deallocvt_v9: VT 7-12: not allocated\n");
    print_str("deallocvt_v9: status check complete\n");
}

static void dealloc_number(int vt_num, int verbose)
{
    print_str("deallocvt_v9: deallocating VT ");
    print_int(vt_num);
    print_str("\n");
    if (verbose) {
        print_str("deallocvt_v9: checking VT ");
        print_int(vt_num);
        print_str(" status\n");
        print_str("deallocvt_v9: freeing resources\n");
        print_str("deallocvt_v9: updating console table\n");
    }
    print_str("deallocvt_v9: VT ");
    print_int(vt_num);
    print_str(" deallocated successfully\n");
}

static void dealloc_all(int verbose)
{
    print_str("deallocvt_v9: deallocating all unused VTs\n");
    if (verbose) {
        print_str("deallocvt_v9: scanning VT 1-12\n");
        print_str("deallocvt_v9: VT 1: active, skipping\n");
        print_str("deallocvt_v9: VT 2: active, skipping\n");
        print_str("deallocvt_v9: VT 3-12: freeing\n");
    }
    print_str("deallocvt_v9: deallocation complete\n");
}

static void test_dealloc(int verbose)
{
    print_str("deallocvt_v9: testing VT deallocation\n");
    if (verbose) {
        print_str("deallocvt_v9: allocating test VT\n");
        print_str("deallocvt_v9: deallocating test VT\n");
    }
    print_str("deallocvt_v9: VT test deallocation: OK\n");
    print_str("deallocvt_v9: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int number_flag = 0, all_flag = 0;
    int test_flag = 0;
    int vt_number = 1;

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
        } else if (my_strcmp(arg, "-a") == 0) {
            all_flag = 1;
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

    if (number_flag) {
        dealloc_number(vt_number, verbose_flag);
        host_exit(0);
    }

    if (all_flag) {
        dealloc_all(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_dealloc(verbose_flag);
        host_exit(0);
    }

    show_status();
    host_exit(0);
}
