/* setmetamode_v12: set meta key mode (v12) */

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
    print_str("setmetamode_v12 - set meta key mode (v12)\n");
    print_str("Usage: setmetamode_v12 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -m MODE        Set mode (meta, esc, none)\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Set the meta key handling mode for the console.\n");
}

static void show_info(void)
{
    print_str("setmetamode_v12: meta key mode info:\n");
    print_str("setmetamode_v12: supported modes: meta, esc, none\n");
    print_str("setmetamode_v12: current mode: meta\n");
    print_str("setmetamode_v12: meta generates: high-bit\n");
    print_str("setmetamode_v12: info display complete\n");
}

static void show_status(void)
{
    print_str("setmetamode_v12: meta key status:\n");
    print_str("setmetamode_v12: mode: meta\n");
    print_str("setmetamode_v12: meta handling: high-bit\n");
    print_str("setmetamode_v12: alt key: active\n");
    print_str("setmetamode_v12: status check complete\n");
}

static void set_mode(const char *mode, int verbose)
{
    if (my_strcmp(mode, "meta") == 0) {
        print_str("setmetamode_v12: setting mode to: meta\n");
        print_str("setmetamode_v12: meta key generates high-bit\n");
    } else if (my_strcmp(mode, "esc") == 0) {
        print_str("setmetamode_v12: setting mode to: esc\n");
        print_str("setmetamode_v12: meta key generates escape prefix\n");
    } else if (my_strcmp(mode, "none") == 0) {
        print_str("setmetamode_v12: setting mode to: none\n");
        print_str("setmetamode_v12: meta key ignored\n");
    } else {
        print_str("setmetamode_v12: unknown mode: ");
        print_str(mode);
        print_str("\n");
        print_str("setmetamode_v12: valid modes: meta, esc, none\n");
        host_exit(1);
    }
    if (verbose) {
        print_str("setmetamode_v12: updating keymap tables\n");
        print_str("setmetamode_v12: meta mode configuration applied\n");
    }
    print_str("setmetamode_v12: meta key mode set successfully\n");
}

static void test_mode(int verbose)
{
    print_str("setmetamode_v12: testing meta key mode\n");
    if (verbose) {
        print_str("setmetamode_v12: checking keymap configuration\n");
        print_str("setmetamode_v12: testing meta key handling\n");
    }
    print_str("setmetamode_v12: meta key test: OK\n");
    print_str("setmetamode_v12: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *mode_arg = 0;

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
        } else if (my_strcmp(arg, "-m") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                mode_arg = &argv_ptr[pos];
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

    if (mode_arg) {
        set_mode(mode_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("setmetamode_v12: current meta key mode: meta\n");
    host_exit(0);
}
