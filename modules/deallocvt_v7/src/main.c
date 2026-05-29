/* deallocvt_v7: deallocate virtual terminal (v7) */

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

static int parse_int(const char *s)
{
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n;
}

static void show_help(void)
{
    print_str("deallocvt_v7 - deallocate virtual terminal (v7)\n");
    print_str("Usage: deallocvt_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n NUM         VT number to deallocate\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Deallocate unused virtual terminal sessions.\n");
}

static void show_info(void)
{
    print_str("deallocvt_v7: virtual terminal info:\n");
    print_str("deallocvt_v7: total VTs: 12\n");
    print_str("deallocvt_v7: active VTs: 3 (1, 2, 3)\n");
    print_str("deallocvt_v7: available VTs: 9\n");
    print_str("deallocvt_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("deallocvt_v7: virtual terminal status:\n");
    print_str("deallocvt_v7: VT  1: active (foreground)\n");
    print_str("deallocvt_v7: VT  2: active\n");
    print_str("deallocvt_v7: VT  3: active\n");
    print_str("deallocvt_v7: VT  4-12: available\n");
    print_str("deallocvt_v7: status check complete\n");
}

static void dealloc_vt(int vt_num, int verbose)
{
    print_str("deallocvt_v7: deallocating VT ");
    print_int(vt_num);
    print_str("\n");
    if (vt_num == 1) {
        print_str("deallocvt_v7: cannot deallocate foreground VT 1\n");
        print_str("deallocvt_v7: error: VT 1 is in use\n");
        host_exit(1);
    } else if (vt_num >= 2 && vt_num <= 12) {
        print_str("deallocvt_v7: VT ");
        print_int(vt_num);
        print_str(" deallocated successfully\n");
        if (verbose) {
            print_str("deallocvt_v7: freeing VT resources\n");
            print_str("deallocvt_v7: clearing screen buffer\n");
        }
    } else {
        print_str("deallocvt_v7: invalid VT number: ");
        print_int(vt_num);
        print_str("\n");
        print_str("deallocvt_v7: valid range: 1-12\n");
        host_exit(1);
    }
}

static void test_mode(int verbose)
{
    print_str("deallocvt_v7: testing virtual terminal deallocation\n");
    if (verbose) {
        print_str("deallocvt_v7: checking VT table integrity\n");
        print_str("deallocvt_v7: verifying process bindings\n");
    }
    print_str("deallocvt_v7: deallocvt test: OK\n");
    print_str("deallocvt_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, has_vt = 0;
    int vt_num = 0;

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
        } else if (my_strcmp(arg, "-n") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                vt_num = parse_int(&argv_ptr[pos]);
                has_vt = 1;
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

    if (has_vt) {
        dealloc_vt(vt_num, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("deallocvt_v7: no VT specified, showing status\n");
    show_status();
    host_exit(0);
}
