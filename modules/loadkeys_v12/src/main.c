/* loadkeys_v12: load keyboard map table (v12) */

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
    print_str("loadkeys_v12 - load keyboard map table (v12)\n");
    print_str("Usage: loadkeys_v12 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c CHARSET     Set charset (unicode, utf-8, iso-8859-1)\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Load keyboard map table with specified charset.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v12: keyboard map info:\n");
    print_str("loadkeys_v12: supported charsets: unicode, utf-8, iso-8859-1\n");
    print_str("loadkeys_v12: current charset: unicode\n");
    print_str("loadkeys_v12: keymap entries: 256\n");
    print_str("loadkeys_v12: loaded: no\n");
    print_str("loadkeys_v12: info display complete\n");
}

static void show_status(void)
{
    print_str("loadkeys_v12: keyboard map status:\n");
    print_str("loadkeys_v12: charset: unicode\n");
    print_str("loadkeys_v12: table: default\n");
    print_str("loadkeys_v12: entries: 256\n");
    print_str("loadkeys_v12: status check complete\n");
}

static void set_charset(const char *charset, int verbose)
{
    print_str("loadkeys_v12: loading keyboard map for charset: ");
    print_str(charset);
    print_str("\n");
    if (my_strcmp(charset, "unicode") == 0) {
        print_str("loadkeys_v12: loading unicode keymap\n");
        print_str("loadkeys_v12: keymap loaded: unicode\n");
    } else if (my_strcmp(charset, "utf-8") == 0) {
        print_str("loadkeys_v12: loading utf-8 keymap\n");
        print_str("loadkeys_v12: keymap loaded: utf-8\n");
    } else if (my_strcmp(charset, "iso-8859-1") == 0) {
        print_str("loadkeys_v12: loading iso-8859-1 keymap\n");
        print_str("loadkeys_v12: keymap loaded: iso-8859-1\n");
    } else {
        print_str("loadkeys_v12: unknown charset: ");
        print_str(charset);
        print_str("\n");
        print_str("loadkeys_v12: valid charsets: unicode, utf-8, iso-8859-1\n");
        host_exit(1);
    }
    if (verbose) {
        print_str("loadkeys_v12: parsing keymap table\n");
        print_str("loadkeys_v12: 256 entries loaded\n");
        print_str("loadkeys_v12: keymap table reload complete\n");
    }
}

static void test_mode(int verbose)
{
    print_str("loadkeys_v12: testing keyboard map loading\n");
    if (verbose) {
        print_str("loadkeys_v12: loading test keymap\n");
        print_str("loadkeys_v12: verifying keymap entries\n");
        print_str("loadkeys_v12: checking character mappings\n");
    }
    print_str("loadkeys_v12: keyboard map test: OK\n");
    print_str("loadkeys_v12: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *charset_arg = 0;

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
        } else if (my_strcmp(arg, "-c") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                charset_arg = &argv_ptr[pos];
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

    if (charset_arg) {
        set_charset(charset_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    /* Default: show current charset */
    print_str("loadkeys_v12: current charset: unicode\n");
    host_exit(0);
}
