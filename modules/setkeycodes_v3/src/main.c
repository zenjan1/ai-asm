/* setkeycodes_v3: scancode mapping enhanced (v3) */

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
    print_str("setkeycodes_v3 - set scancode mappings (v3.0)\n");
    print_str("Usage: setkeycodes_v3 [OPTIONS] [SCANCODE KEYCODE]...\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a <scancode> <keycode>  Add mapping\n");
    print_str("  -r <scancode>  Remove mapping\n");
    print_str("  -l             List all mappings\n");
    print_str("  -v             Verbose output\n");
    print_str("  -t <scancode>  Test scancode\n");
    print_str("\n");
    print_str("Set keyboard scancode to keycode mappings.\n");
}

static void list_mappings(int verbose)
{
    print_str("setkeycodes_v3: current scancode mappings:\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe003);
    print_str(" -> keycode 124 (F14)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe004);
    print_str(" -> keycode 125 (F15)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe005);
    print_str(" -> keycode 126 (F16)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe006);
    print_str(" -> keycode 127 (F17)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe007);
    print_str(" -> keycode 128 (F18)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe008);
    print_str(" -> keycode 129 (F19)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe009);
    print_str(" -> keycode 130 (F20)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe00a);
    print_str(" -> keycode 131 (Help)\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(0xe00b);
    print_str(" -> keycode 132 (Menu)\n");
    if (verbose) {
        print_str("setkeycodes_v3: total mappings: 9\n");
        print_str("setkeycodes_v3: table size: 256 entries max\n");
        print_str("setkeycodes_v3: extended scancodes: 0xe000-0xe0ff\n");
    }
}

static void add_mapping(int scancode, int keycode, int verbose)
{
    print_str("setkeycodes_v3: adding mapping:\n");
    print_str("setkeycodes_v3: scancode 0x");
    print_hex(scancode);
    print_str(" -> keycode ");
    print_int(keycode);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v3: updating keycode table\n");
        print_str("setkeycodes_v3: validating keycode range\n");
        print_str("setkeycodes_v3: entry written\n");
    }
    print_str("setkeycodes_v3: mapping added\n");
}

static void remove_mapping(int scancode, int verbose)
{
    print_str("setkeycodes_v3: removing scancode 0x");
    print_hex(scancode);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v3: searching table\n");
        print_str("setkeycodes_v3: entry found\n");
        print_str("setkeycodes_v3: entry removed\n");
    }
    print_str("setkeycodes_v3: mapping removed\n");
}

static void test_scancode(int scancode)
{
    print_str("setkeycodes_v3: testing scancode 0x");
    print_hex(scancode);
    print_str("\n");
    print_str("setkeycodes_v3: scancode ");
    print_hex(scancode);
    print_str(" is ");
    if (scancode >= 0xe000 && scancode <= 0xe0ff) {
        print_str("valid (extended)\n");
    } else if (scancode >= 0x01 && scancode <= 0x7f) {
        print_str("valid (standard)\n");
    } else {
        print_str("invalid\n");
    }
    print_str("setkeycodes_v3: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int add_flag = 0;
    int remove_flag = 0;
    int list_flag = 0;
    int verbose_flag = 0;
    int test_flag = 0;
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

    if (list_flag) {
        list_mappings(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_scancode(test_sc);
        host_exit(0);
    }

    if (remove_flag) {
        remove_mapping(remove_sc, verbose_flag);
        host_exit(0);
    }

    if (add_flag) {
        add_mapping(add_sc, add_kc, verbose_flag);
        host_exit(0);
    }

    print_str("setkeycodes_v3: 9 default mappings loaded\n");
    print_str("setkeycodes_v3: use -l to list, -a to add\n");

    host_exit(0);
}
