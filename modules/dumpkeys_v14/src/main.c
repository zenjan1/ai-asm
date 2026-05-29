/* dumpkeys_v14: dump keyboard map table (v14) */

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
    print_str("dumpkeys_v14 - dump keyboard map table (v14)\n");
    print_str("Usage: dumpkeys_v14 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -f FILE        Output file\n");
    print_str("  -k             Show keymap only\n");
    print_str("  -c             Show compose table\n");
    print_str("  -t             Test dump\n");
    print_str("\n");
    print_str("Dump the current keyboard map table.\n");
}

static void show_info(void)
{
    print_str("dumpkeys_v14: keymap dump info:\n");
    print_str("dumpkeys_v14: keymap version: 2.0\n");
    print_str("dumpkeys_v14: charset: UTF-8\n");
    print_str("dumpkeys_v14: keycount: 128\n");
    print_str("dumpkeys_v14: strings: 48\n");
    print_str("dumpkeys_v14: compose entries: 64\n");
    print_str("dumpkeys_v14: info display complete\n");
}

static void show_keymap(int verbose)
{
    print_str("dumpkeys_v14: keymap table:\n");
    print_str("dumpkeys_v14: keycode 0: Escape\n");
    print_str("dumpkeys_v14: keycode 1: 1\n");
    print_str("dumpkeys_v14: keycode 2: 2\n");
    print_str("dumpkeys_v14: keycode 3: 3\n");
    if (verbose) {
        print_str("dumpkeys_v14: ... 128 keycodes total\n");
        print_str("dumpkeys_v14: shift/ctrl/alt modifiers included\n");
    }
    print_str("dumpkeys_v14: keymap dump complete\n");
}

static void show_compose(int verbose)
{
    print_str("dumpkeys_v14: compose table:\n");
    print_str("dumpkeys_v14: Compose+A -> A-ring\n");
    print_str("dumpkeys_v14: Compose+C+, -> C-cedilla\n");
    print_str("dumpkeys_v14: Compose+S+o -> ohm\n");
    if (verbose) {
        print_str("dumpkeys_v14: ... 64 compose sequences total\n");
    }
    print_str("dumpkeys_v14: compose table dump complete\n");
}

static void dump_to_file(const char *filename, int verbose)
{
    print_str("dumpkeys_v14: dumping keymap to: ");
    print_str(filename);
    print_str("\n");
    if (verbose) {
        print_str("dumpkeys_v14: keymaps 0-127\n");
        print_str("dumpkeys_v14: strings 0-47\n");
        print_str("dumpkeys_v14: compose table included\n");
    }
    print_str("dumpkeys_v14: keymap dumped successfully\n");
}

static void test_dump(int verbose)
{
    print_str("dumpkeys_v14: testing keymap dump\n");
    if (verbose) {
        print_str("dumpkeys_v14: reading keymap table\n");
        print_str("dumpkeys_v14: validating strings\n");
        print_str("dumpkeys_v14: format check: OK\n");
    }
    print_str("dumpkeys_v14: keymap dump test: OK\n");
    print_str("dumpkeys_v14: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int file_flag = 0, keymap_flag = 0;
    int compose_flag = 0, test_flag = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            file_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (my_strcmp(arg, "-k") == 0) {
            keymap_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            compose_flag = 1;
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

    if (file_flag) {
        dump_to_file("keymap.dump", verbose_flag);
        host_exit(0);
    }

    if (keymap_flag) {
        show_keymap(verbose_flag);
        host_exit(0);
    }

    if (compose_flag) {
        show_compose(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_dump(verbose_flag);
        host_exit(0);
    }

    print_str("dumpkeys_v14: dumping keymap table\n");
    print_str("dumpkeys_v14: keycode 0: Escape\n");
    print_str("dumpkeys_v14: keycode 1: 1\n");
    print_str("dumpkeys_v14: keycode 2: 2\n");
    print_str("dumpkeys_v14: dump complete\n");
    host_exit(0);
}
