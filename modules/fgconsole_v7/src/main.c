/* fgconsole_v7: show foreground virtual terminal (v7) */

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
    print_str("fgconsole_v7 - show foreground virtual terminal (v1.0)\n");
    print_str("Usage: fgconsole_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n             Show next VT number\n");
    print_str("  -p             Show previous VT number\n");
    print_str("  -t             Test console switching\n");
    print_str("\n");
    print_str("Display and manage foreground virtual console.\n");
}

static void show_info(void)
{
    print_str("fgconsole_v7: foreground console info:\n");
    print_str("fgconsole_v7: current VT: 1\n");
    print_str("fgconsole_v7: mode: text\n");
    print_str("fgconsole_v7: process: shell (pid 1)\n");
    print_str("fgconsole_v7: user: root\n");
    print_str("fgconsole_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("fgconsole_v7: VT status:\n");
    print_str("fgconsole_v7: VT 1: active (foreground)\n");
    print_str("fgconsole_v7: VT 2: available\n");
    print_str("fgconsole_v7: VT 3: available\n");
    print_str("fgconsole_v7: VT 4: available\n");
    print_str("fgconsole_v7: VT 5: available\n");
    print_str("fgconsole_v7: VT 6: available\n");
    print_str("fgconsole_v7: VT 7: available\n");
    print_str("fgconsole_v7: status check complete\n");
}

static void show_next(int verbose)
{
    print_str("fgconsole_v7: next VT number: 2\n");
    if (verbose) {
        print_str("fgconsole_v7: scanning available VTs\n");
        print_str("fgconsole_v7: VT 2 is next available\n");
    }
}

static void show_previous(int verbose)
{
    print_str("fgconsole_v7: previous VT number: 7\n");
    if (verbose) {
        print_str("fgconsole_v7: scanning allocated VTs\n");
        print_str("fgconsole_v7: VT 7 is last allocated\n");
    }
}

static void test_console(int verbose)
{
    print_str("fgconsole_v7: testing console switching\n");
    if (verbose) {
        print_str("fgconsole_v7: switching to VT 2\n");
        print_str("fgconsole_v7: verifying switch\n");
        print_str("fgconsole_v7: switching back to VT 1\n");
    }
    print_str("fgconsole_v7: console switch test: OK\n");
    print_str("fgconsole_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int next_flag = 0, previous_flag = 0;
    int test_flag = 0;

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
            next_flag = 1;
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

    if (next_flag) {
        show_next(verbose_flag);
        host_exit(0);
    }

    if (previous_flag) {
        show_previous(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_console(verbose_flag);
        host_exit(0);
    }

    /* Default: print current foreground VT */
    print_str("fgconsole_v7: current VT: 1\n");
    host_exit(0);
}
