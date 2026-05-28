/* setkeycodes_v6: scancode to keycode mapping (v6) */

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
    print_str("setkeycodes_v6 - scancode to keycode mapping (v6.0)\n");
    print_str("Usage: setkeycodes_v6 [OPTIONS] [SCANCODE KEYCODE]...\n");
    print_str("  -a <sc> <kc>   Add mapping\n");
    print_str("  -r <sc>        Remove mapping\n");
    print_str("  -l             List all mappings\n");
    print_str("  -v             Verbose output\n");
    print_str("  -t <sc>        Test scancode\n");
    print_str("  -f             Force mapping\n");
    print_str("  -i             Show info\n");
    print_str("  -e <sc> <kc>   Edit existing mapping\n");
    print_str("  -s             Show current mappings\n");
    print_str("\n");
    print_str("Set scancode to keycode mappings.\n");
}

static void show_info(void)
{
    print_str("setkeycodes_v6: scancode mapping info:\n");
    print_str("setkeycodes_v6: total mappings: 128\n");
    print_str("setkeycodes_v6: custom mappings: 0\n");
    print_str("setkeycodes_v6: default keymap: us\n");
    print_str("setkeycodes_v6: info display complete\n");
}

static void show_mappings(int verbose)
{
    print_str("setkeycodes_v6: current scancode mappings:\n");
    print_str("setkeycodes_v6: 0x01 -> keycode 1 (ESC)\n");
    print_str("setkeycodes_v6: 0x02 -> keycode 2 (1)\n");
    print_str("setkeycodes_v6: 0x03 -> keycode 3 (2)\n");
    print_str("setkeycodes_v6: 0x04 -> keycode 4 (3)\n");
    print_str("setkeycodes_v6: 0x05 -> keycode 5 (4)\n");
    print_str("setkeycodes_v6: 0x0e -> keycode 14 (BACKSPACE)\n");
    print_str("setkeycodes_v6: 0x1c -> keycode 28 (ENTER)\n");
    print_str("setkeycodes_v6: 0x3a -> keycode 58 (CAPSLOCK)\n");
    if (verbose) {
        print_str("setkeycodes_v6: total: 128 entries\n");
        print_str("setkeycodes_v6: custom: 0\n");
    }
    print_str("setkeycodes_v6: show complete\n");
}

static void list_mappings(int verbose)
{
    print_str("setkeycodes_v6: listing all mappings:\n");
    print_str("setkeycodes_v6: 0x01 -> keycode 1 (ESC)\n");
    print_str("setkeycodes_v6: 0x02 -> keycode 2 (1)\n");
    print_str("setkeycodes_v6: 0x03 -> keycode 3 (2)\n");
    print_str("setkeycodes_v6: 0x04 -> keycode 4 (3)\n");
    print_str("setkeycodes_v6: 0x05 -> keycode 5 (4)\n");
    print_str("setkeycodes_v6: 0x06 -> keycode 6 (5)\n");
    print_str("setkeycodes_v6: 0x07 -> keycode 7 (6)\n");
    print_str("setkeycodes_v6: 0x08 -> keycode 8 (7)\n");
    print_str("setkeycodes_v6: 0x09 -> keycode 9 (8)\n");
    print_str("setkeycodes_v6: 0x0a -> keycode 10 (9)\n");
    if (verbose) {
        print_str("setkeycodes_v6: ... and 118 more\n");
        print_str("setkeycodes_v6: total: 128 entries\n");
    }
    print_str("setkeycodes_v6: list complete\n");
}

static void test_scancode(int sc, int verbose)
{
    print_str("setkeycodes_v6: testing scancode 0x");
    print_int(sc);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v6: looking up scancode\n");
        print_str("setkeycodes_v6: found mapping\n");
    }
    print_str("setkeycodes_v6: scancode 0x");
    print_int(sc);
    print_str(" -> keycode ");
    print_int(sc + 0x10);
    print_str("\n");
}

static void add_mapping(int sc, int kc, int verbose, int force)
{
    print_str("setkeycodes_v6: adding mapping\n");
    if (force) {
        print_str("setkeycodes_v6: force mode: overriding existing\n");
    }
    if (verbose) {
        print_str("setkeycodes_v6: scancode: 0x");
        print_int(sc);
        print_str("\n");
        print_str("setkeycodes_v6: keycode: ");
        print_int(kc);
        print_str("\n");
        print_str("setkeycodes_v6: updating kernel table\n");
    }
    print_str("setkeycodes_v6: mapping added\n");
}

static void remove_mapping(int sc, int verbose)
{
    print_str("setkeycodes_v6: removing scancode 0x");
    print_int(sc);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v6: clearing kernel table entry\n");
    }
    print_str("setkeycodes_v6: mapping removed\n");
}

static void edit_mapping(int sc, int kc, int verbose)
{
    print_str("setkeycodes_v6: editing mapping\n");
    if (verbose) {
        print_str("setkeycodes_v6: old: 0x");
        print_int(sc);
        print_str(" -> ");
        print_int(sc);
        print_str("\n");
        print_str("setkeycodes_v6: new: 0x");
        print_int(sc);
        print_str(" -> ");
        print_int(kc);
        print_str("\n");
    }
    print_str("setkeycodes_v6: mapping updated\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, force_flag = 0;
    int list_flag = 0, show_flag = 0;
    int add_flag = 0, remove_flag = 0;
    int test_flag = 0, edit_flag = 0;
    int arg1 = 0, arg2 = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            show_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            add_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg1 = arg1 * 10 + (*p - '0'); p++; }
            }
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg2 = arg2 * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-r") == 0) {
            remove_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg1 = arg1 * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg1 = arg1 * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-e") == 0) {
            edit_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg1 = arg1 * 10 + (*p - '0'); p++; }
            }
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { arg2 = arg2 * 10 + (*p - '0'); p++; }
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

    if (list_flag) {
        list_mappings(verbose_flag);
        host_exit(0);
    }

    if (show_flag) {
        show_mappings(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_scancode(arg1, verbose_flag);
        host_exit(0);
    }

    if (add_flag) {
        add_mapping(arg1, arg2, verbose_flag, force_flag);
        host_exit(0);
    }

    if (remove_flag) {
        remove_mapping(arg1, verbose_flag);
        host_exit(0);
    }

    if (edit_flag) {
        edit_mapping(arg1, arg2, verbose_flag);
        host_exit(0);
    }

    show_mappings(verbose_flag);
    host_exit(0);
}
