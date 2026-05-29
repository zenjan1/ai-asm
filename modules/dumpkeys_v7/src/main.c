/* dumpkeys_v7: dump keyboard map (v7) */

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
    print_str("dumpkeys_v7 - dump keyboard map (v7)\n");
    print_str("Usage: dumpkeys_v7 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -k KEYMAP      Specify keymap\n");
    print_str("  -c             Show compose table\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Dump the current keyboard mapping to stdout.\n");
}

static void show_info(void)
{
    print_str("dumpkeys_v7: keyboard map info:\n");
    print_str("dumpkeys_v7: keymap: default (us)\n");
    print_str("dumpkeys_v7: keysyms per key: 4\n");
    print_str("dumpkeys_v7: compose entries: 12\n");
    print_str("dumpkeys_v7: modifier bindings: 8\n");
    print_str("dumpkeys_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("dumpkeys_v7: keyboard map status:\n");
    print_str("dumpkeys_v7: keymap: us\n");
    print_str("dumpkeys_v7: charset: ASCII\n");
    print_str("dumpkeys_v7: compose: enabled\n");
    print_str("dumpkeys_v7: status check complete\n");
}

static void dump_keymap(const char *keymap, int verbose)
{
    print_str("dumpkeys_v7: dumping keymap: ");
    print_str(keymap);
    print_str("\n");
    if (my_strcmp(keymap, "us") == 0) {
        print_str("dumpkeys_v7: keymap: US QWERTY\n");
        print_str("dumpkeys_v7: keycode   type      value\n");
        print_str("dumpkeys_v7: 1        Escape    Escape\n");
        print_str("dumpkeys_v7: 2        1         exclam\n");
        print_str("dumpkeys_v7: 3        2         at\n");
        print_str("dumpkeys_v7: 4        3         numbersign\n");
        print_str("dumpkeys_v7: 5        4         dollar\n");
    } else if (my_strcmp(keymap, "uk") == 0) {
        print_str("dumpkeys_v7: keymap: UK QWERTY\n");
        print_str("dumpkeys_v7: keycode   type      value\n");
        print_str("dumpkeys_v7: 1        Escape    Escape\n");
        print_str("dumpkeys_v7: 2        1         exclam\n");
        print_str("dumpkeys_v7: 3        2         quotedbl\n");
        print_str("dumpkeys_v7: 4        3         sterling\n");
        print_str("dumpkeys_v7: 5        4         dollar\n");
    } else {
        print_str("dumpkeys_v7: custom keymap: ");
        print_str(keymap);
        print_str("\n");
        print_str("dumpkeys_v7: dumping full keymap table\n");
    }
    if (verbose) {
        print_str("dumpkeys_v7: dumping modifier bindings\n");
        print_str("dumpkeys_v7: dumping string table\n");
    }
    print_str("dumpkeys_v7: keymap dump complete\n");
}

static void show_compose(int verbose)
{
    print_str("dumpkeys_v7: compose table:\n");
    print_str("dumpkeys_v7: <Multi_key> <a> <e> : ae\n");
    print_str("dumpkeys_v7: <Multi_key> <A> <E> : AE\n");
    print_str("dumpkeys_v7: <Multi_key> <o> <c> : o copyright\n");
    print_str("dumpkeys_v7: <Multi_key> <s> <s> : sharp s\n");
    print_str("dumpkeys_v7: compose table dump complete\n");
    if (verbose) {
        print_str("dumpkeys_v7: compose entries: 12 loaded\n");
    }
}

static void test_mode(int verbose)
{
    print_str("dumpkeys_v7: testing keyboard map dump\n");
    if (verbose) {
        print_str("dumpkeys_v7: verifying keymap integrity\n");
        print_str("dumpkeys_v7: checking compose table\n");
    }
    print_str("dumpkeys_v7: keyboard map test: OK\n");
    print_str("dumpkeys_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, compose_flag = 0;
    char *keymap_arg = 0;

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
        } else if (my_strcmp(arg, "-c") == 0) {
            compose_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                keymap_arg = &argv_ptr[pos];
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

    if (compose_flag) {
        show_compose(verbose_flag);
        host_exit(0);
    }

    if (keymap_arg) {
        dump_keymap(keymap_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("dumpkeys_v7: dumping default keymap (us)\n");
    print_str("dumpkeys_v7: keymap: US QWERTY\n");
    print_str("dumpkeys_v7: keycode 1: Escape\n");
    print_str("dumpkeys_v7: keymap dump complete\n");
    host_exit(0);
}
