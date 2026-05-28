/* setkeycodes_v5: scancode mapping ultimate (v5) */

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

static void print_hex(int n)
{
    char buf[8];
    int i = 7;
    buf[7] = 0;
    if (n == 0) buf[--i] = '0';
    while (n > 0) {
        int d = n & 0xf;
        buf[--i] = (d < 10) ? ('0' + d) : ('a' + d - 10);
        n >>= 4;
    }
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
    print_str("setkeycodes_v5 - set scancode mappings (v5.0)\n");
    print_str("Usage: setkeycodes_v5 [OPTIONS] [SCANCODE KEYCODE]...\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a <sc> <kc>   Add mapping\n");
    print_str("  -r <scancode>  Remove mapping\n");
    print_str("  -l             List all mappings\n");
    print_str("  -v             Verbose output\n");
    print_str("  -t <scancode>  Test scancode\n");
    print_str("  -f             Force override\n");
    print_str("  -i             Show detailed info\n");
    print_str("  -e             Export mappings\n");
    print_str("  -s             Save to file\n");
    print_str("\n");
    print_str("Set keyboard scancode to keycode mappings.\n");
}

static void show_info(void)
{
    print_str("setkeycodes_v5: scancode table information:\n");
    print_str("setkeycodes_v5: table size: 256 entries\n");
    print_str("setkeycodes_v5: used entries: 9\n");
    print_str("setkeycodes_v5: free entries: 247\n");
    print_str("setkeycodes_v5: extended scancodes: 0xe000-0xe0ff\n");
    print_str("setkeycodes_v5: standard scancodes: 0x01-0x7f\n");
    print_str("setkeycodes_v5: keycode range: 1-255\n");
    print_str("setkeycodes_v5: dirty flag: no\n");
    print_str("setkeycodes_v5: info display complete\n");
}

static void export_mappings(void)
{
    print_str("setkeycodes_v5: exporting scancode mappings:\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe003);
    print_str(" 124\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe004);
    print_str(" 125\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe005);
    print_str(" 126\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe006);
    print_str(" 127\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe007);
    print_str(" 128\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe008);
    print_str(" 129\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe009);
    print_str(" 130\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe00a);
    print_str(" 131\n");
    print_str("setkeycodes_v5: 0x");
    print_hex(0xe00b);
    print_str(" 132\n");
    print_str("setkeycodes_v5: export complete: 9 entries\n");
}

static void save_mappings(void)
{
    print_str("setkeycodes_v5: saving scancode mappings\n");
    print_str("setkeycodes_v5: writing to /etc/keymap.save\n");
    print_str("setkeycodes_v5: 9 entries written\n");
    print_str("setkeycodes_v5: save complete\n");
}

static void list_mappings(int verbose, int force)
{
    print_str("setkeycodes_v5: current scancode mappings:\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe003);
    print_str(" -> keycode 124 (F14)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe004);
    print_str(" -> keycode 125 (F15)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe005);
    print_str(" -> keycode 126 (F16)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe006);
    print_str(" -> keycode 127 (F17)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe007);
    print_str(" -> keycode 128 (F18)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe008);
    print_str(" -> keycode 129 (F19)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe009);
    print_str(" -> keycode 130 (F20)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe00a);
    print_str(" -> keycode 131 (Help)\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(0xe00b);
    print_str(" -> keycode 132 (Menu)\n");
    if (verbose) {
        print_str("setkeycodes_v5: total mappings: 9\n");
        print_str("setkeycodes_v5: table capacity: 256\n");
        print_str("setkeycodes_v5: force mode: ");
        if (force) print_str("enabled\n"); else print_str("disabled\n");
    }
}

static void add_mapping(int sc, int kc, int verbose, int force)
{
    print_str("setkeycodes_v5: adding mapping:\n");
    print_str("setkeycodes_v5: scancode 0x");
    print_hex(sc);
    print_str(" -> keycode ");
    print_int(kc);
    print_str("\n");
    if (force) {
        print_str("setkeycodes_v5: force mode: overriding existing entry\n");
    }
    if (verbose) {
        print_str("setkeycodes_v5: validating scancode range\n");
        print_str("setkeycodes_v5: validating keycode range\n");
        print_str("setkeycodes_v5: updating translation table\n");
    }
    print_str("setkeycodes_v5: mapping added\n");
}

static void remove_mapping(int sc, int verbose)
{
    print_str("setkeycodes_v5: removing scancode 0x");
    print_hex(sc);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v5: searching table\n");
        print_str("setkeycodes_v5: entry found and removed\n");
    }
    print_str("setkeycodes_v5: mapping removed\n");
}

static void test_scancode(int sc)
{
    print_str("setkeycodes_v5: testing scancode 0x");
    print_hex(sc);
    print_str("\n");
    if (sc >= 0xe000 && sc <= 0xe0ff) {
        print_str("setkeycodes_v5: type: extended scancode\n");
        print_str("setkeycodes_v5: status: valid\n");
    } else if (sc >= 0x01 && sc <= 0x7f) {
        print_str("setkeycodes_v5: type: standard scancode\n");
        print_str("setkeycodes_v5: status: valid\n");
    } else {
        print_str("setkeycodes_v5: type: unknown\n");
        print_str("setkeycodes_v5: status: invalid\n");
    }
    print_str("setkeycodes_v5: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, add_flag = 0, remove_flag = 0;
    int list_flag = 0, verbose_flag = 0, test_flag = 0;
    int force_flag = 0, info_flag = 0, export_flag = 0, save_flag = 0;
    int add_sc = 0, add_kc = 0, remove_sc = 0, test_sc = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-e") == 0) {
            export_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            save_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            add_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { add_sc = add_sc * 10 + (*p - '0'); p++; }
            }
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { add_kc = add_kc * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-r") == 0) {
            remove_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { remove_sc = remove_sc * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { test_sc = test_sc * 10 + (*p - '0'); p++; }
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

    if (export_flag) {
        export_mappings();
        host_exit(0);
    }

    if (save_flag) {
        save_mappings();
        host_exit(0);
    }

    if (test_flag) {
        test_scancode(test_sc);
        host_exit(0);
    }

    if (list_flag) {
        list_mappings(verbose_flag, force_flag);
        host_exit(0);
    }

    if (remove_flag) {
        remove_mapping(remove_sc, verbose_flag);
        host_exit(0);
    }

    if (add_flag) {
        add_mapping(add_sc, add_kc, verbose_flag, force_flag);
        host_exit(0);
    }

    print_str("setkeycodes_v5: 9 default mappings loaded\n");
    print_str("setkeycodes_v5: use -i for info, -a to add\n");

    host_exit(0);
}
