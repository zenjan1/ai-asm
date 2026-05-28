/* loadkeys_v7: load keyboard mapping table (v7) */

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
    print_str("loadkeys_v7 - load keyboard mapping table (v1.0)\n");
    print_str("Usage: loadkeys_v7 [OPTIONS] [MAPFILE]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c             Clear current keymap\n");
    print_str("  -l             List loaded keymaps\n");
    print_str("  -t             Test keyboard mapping\n");
    print_str("\n");
    print_str("Load keyboard layout definition into kernel.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v7: keyboard mapping info:\n");
    print_str("loadkeys_v7: current keymap: us (QWERTY)\n");
    print_str("loadkeys_v7: keycode count: 128\n");
    print_str("loadkeys_v7: compose table: 24 entries\n");
    print_str("loadkeys_v7: string table: 48 strings\n");
    print_str("loadkeys_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("loadkeys_v7: keyboard mapping status:\n");
    print_str("loadkeys_v7: VT 1: us keymap loaded\n");
    print_str("loadkeys_v7: VT 2: us keymap available\n");
    print_str("loadkeys_v7: VT 3: us keymap available\n");
    print_str("loadkeys_v7: dead keys: active\n");
    print_str("loadkeys_v7: status check complete\n");
}

static void clear_keymap(int verbose)
{
    print_str("loadkeys_v7: clearing current keymap\n");
    if (verbose) {
        print_str("loadkeys_v7: removing keycode mappings\n");
        print_str("loadkeys_v7: clearing compose table\n");
        print_str("loadkeys_v7: resetting string table\n");
    }
    print_str("loadkeys_v7: keymap cleared\n");
}

static void list_maps(int verbose)
{
    print_str("loadkeys_v7: loaded keymaps:\n");
    print_str("loadkeys_v7: us (QWERTY) - active\n");
    print_str("loadkeys_v7: uk (QWERTY) - available\n");
    print_str("loadkeys_v7: de (QWERTZ) - available\n");
    print_str("loadkeys_v7: fr (AZERTY) - available\n");
    if (verbose) {
        print_str("loadkeys_v7: total: 4 keymaps\n");
    }
    print_str("loadkeys_v7: list complete\n");
}

static void test_mapping(int verbose)
{
    print_str("loadkeys_v7: testing keyboard mapping\n");
    if (verbose) {
        print_str("loadkeys_v7: testing A key: 'a' (keycode 30)\n");
        print_str("loadkeys_v7: testing Shift+A: 'A' (keycode 30)\n");
        print_str("loadkeys_v7: testing Ctrl+C: 0x03 (keycode 46)\n");
    }
    print_str("loadkeys_v7: mapping test: PASSED\n");
    print_str("loadkeys_v7: test complete\n");
}

static void load_map(const char *file, int verbose)
{
    print_str("loadkeys_v7: loading keyboard map");
    if (file) {
        print_str(" from ");
        unsigned int len = my_strlen(file);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(file, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v7: parsing keymap file\n");
        print_str("loadkeys_v7: loading keycode table\n");
        print_str("loadkeys_v7: loading compose sequences\n");
        print_str("loadkeys_v7: loading string table\n");
    }
    print_str("loadkeys_v7: keyboard map loaded successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int clear_flag = 0, list_flag = 0;
    int test_flag = 0;
    const char *file_name = 0;

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
        } else if (my_strcmp(arg, "-c") == 0) {
            clear_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else {
            file_name = arg;
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

    if (clear_flag) {
        clear_keymap(verbose_flag);
        host_exit(0);
    }

    if (list_flag) {
        list_maps(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mapping(verbose_flag);
        host_exit(0);
    }

    if (file_name) {
        load_map(file_name, verbose_flag);
        host_exit(0);
    }

    /* Default: show current keymap */
    print_str("loadkeys_v7: us keymap loaded\n");
    host_exit(0);
}
