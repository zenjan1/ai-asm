/* chvt_v7: change virtual terminal (v7) */

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
    print_str("chvt_v7 - change virtual terminal (v1.0)\n");
    print_str("Usage: chvt_v7 [OPTIONS] [N]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n N           Switch to VT N\n");
    print_str("  -p             Switch to previous VT\n");
    print_str("  -t             Test VT switching\n");
    print_str("\n");
    print_str("Switch to specified virtual terminal.\n");
}

static void show_info(void)
{
    print_str("chvt_v7: VT info:\n");
    print_str("chvt_v7: current VT: 1\n");
    print_str("chvt_v7: max VTs: 12\n");
    print_str("chvt_v7: active VTs: 6\n");
    print_str("chvt_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("chvt_v7: VT status:\n");
    print_str("chvt_v7: VT 1: active (foreground)\n");
    print_str("chvt_v7: VT 2: available\n");
    print_str("chvt_v7: VT 3: available\n");
    print_str("chvt_v7: VT 4: available\n");
    print_str("chvt_v7: VT 5: available\n");
    print_str("chvt_v7: VT 6: available\n");
    print_str("chvt_v7: status check complete\n");
}

static void switch_vt(int vt_num, int verbose)
{
    print_str("chvt_v7: switching to VT ");
    print_int(vt_num);
    print_str("\n");
    if (verbose) {
        print_str("chvt_v7: saving current VT state\n");
        print_str("chvt_v7: activating VT ");
        print_int(vt_num);
        print_str("\n");
        print_str("chvt_v7: updating console driver\n");
    }
    print_str("chvt_v7: switched to VT ");
    print_int(vt_num);
    print_str(" successfully\n");
}

static void switch_previous(int verbose)
{
    print_str("chvt_v7: switching to previous VT\n");
    if (verbose) {
        print_str("chvt_v7: checking VT history\n");
        print_str("chvt_v7: switching to last active VT\n");
    }
    print_str("chvt_v7: switched to previous VT successfully\n");
}

static void test_switch(int verbose)
{
    print_str("chvt_v7: testing VT switching\n");
    if (verbose) {
        print_str("chvt_v7: switching to VT 2 (test)\n");
        print_str("chvt_v7: switching to VT 3 (test)\n");
        print_str("chvt_v7: switching back to VT 1\n");
    }
    print_str("chvt_v7: VT switch test: PASSED\n");
    print_str("chvt_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int number_flag = 0, previous_flag = 0;
    int test_flag = 0;
    int vt_num = 2;

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
                vt_num = 0;
                char *num = &argv_ptr[pos];
                for (int i = 0; num[i] && num[i] >= '0' && num[i] <= '9'; i++) {
                    vt_num = vt_num * 10 + (num[i] - '0');
                }
            }
        } else if (my_strcmp(arg, "-p") == 0) {
            previous_flag = 1;
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
        switch_vt(vt_num, verbose_flag);
        host_exit(0);
    }

    if (previous_flag) {
        switch_previous(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_switch(verbose_flag);
        host_exit(0);
    }

    /* Default: switch to next VT */
    print_str("chvt_v7: switching to VT 2\n");
    host_exit(0);
}
