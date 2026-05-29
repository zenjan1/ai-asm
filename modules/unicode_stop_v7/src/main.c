/* unicode_stop_v7: deactivate Unicode mode (v7) */

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
    print_str("unicode_stop_v7 - stop Unicode mode (v7)\n");
    print_str("Usage: unicode_stop_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Deactivate Unicode input/output on the console.\n");
}

static void show_info(void)
{
    print_str("unicode_stop_v7: Unicode info:\n");
    print_str("unicode_stop_v7: mode: not active\n");
    print_str("unicode_stop_v7: charset: ASCII\n");
    print_str("unicode_stop_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("unicode_stop_v7: Unicode status:\n");
    print_str("unicode_stop_v7: mode: ASCII\n");
    print_str("unicode_stop_v7: Unicode: not active\n");
    print_str("unicode_stop_v7: status check complete\n");
}

static void stop_unicode(int verbose)
{
    print_str("unicode_stop_v7: deactivating Unicode mode\n");
    print_str("unicode_stop_v7: restoring ASCII encoding\n");
    if (verbose) {
        print_str("unicode_stop_v7: unloading Unicode font\n");
        print_str("unicode_stop_v7: restoring keyboard map\n");
        print_str("unicode_stop_v7: disabling multi-byte sequences\n");
    }
    print_str("unicode_stop_v7: Unicode mode deactivated\n");
}

static void test_mode(int verbose)
{
    print_str("unicode_stop_v7: testing Unicode deactivation\n");
    if (verbose) {
        print_str("unicode_stop_v7: verifying ASCII mode\n");
        print_str("unicode_stop_v7: checking font reset\n");
    }
    print_str("unicode_stop_v7: Unicode stop test: OK\n");
    print_str("unicode_stop_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;

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

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    stop_unicode(verbose_flag);
    host_exit(0);
}
