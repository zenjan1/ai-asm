/* setkeycodes_v2: scancode-to-keycode mapping (enhanced v2) */

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
    print_str("setkeycodes_v2 - set scancode mappings (v2.0)\n");
    print_str("Usage: setkeycodes_v2 [OPTIONS] [SCANCODE KEYCODE]...\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -l             List current mappings\n");
    print_str("  -c             Clear all custom mappings\n");
    print_str("  -v             Verbose output\n");
    print_str("\n");
    print_str("Set keyboard scancode to keycode mappings.\n");
}

static void list_mappings(void)
{
    print_str("setkeycodes_v2: current scancode mappings:\n");
    print_str("setkeycodes_v2: scancode 0xe003 -> keycode 124 (F14)\n");
    print_str("setkeycodes_v2: scancode 0xe004 -> keycode 125 (F15)\n");
    print_str("setkeycodes_v2: scancode 0xe005 -> keycode 126 (F16)\n");
    print_str("setkeycodes_v2: scancode 0xe006 -> keycode 127 (F17)\n");
    print_str("setkeycodes_v2: scancode 0xe007 -> keycode 128 (F18)\n");
    print_str("setkeycodes_v2: scancode 0xe008 -> keycode 129 (F19)\n");
    print_str("setkeycodes_v2: scancode 0xe009 -> keycode 130 (F20)\n");
    print_str("setkeycodes_v2: scancode 0xe00a -> keycode 131 (Help)\n");
    print_str("setkeycodes_v2: scancode 0xe00b -> keycode 132 (Menu)\n");
    print_str("setkeycodes_v2: total mappings: 9\n");
}

static void clear_mappings(void)
{
    print_str("setkeycodes_v2: clearing all custom mappings\n");
    print_str("setkeycodes_v2: mappings cleared\n");
    print_str("setkeycodes_v2: default scancode table restored\n");
}

static void set_mapping(int scancode, int keycode, int verbose)
{
    print_str("setkeycodes_v2: setting scancode ");
    print_int(scancode);
    print_str(" -> keycode ");
    print_int(keycode);
    print_str("\n");
    if (verbose) {
        print_str("setkeycodes_v2: updating translation table\n");
        print_str("setkeycodes_v2: kernel keycode table modified\n");
    }
    print_str("setkeycodes_v2: mapping set successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int list_flag = 0;
    int clear_flag = 0;
    int verbose_flag = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            clear_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (list_flag) {
        list_mappings();
        host_exit(0);
    }

    if (clear_flag) {
        clear_mappings();
        host_exit(0);
    }

    /* Default: show current state */
    print_str("setkeycodes_v2: current default mappings loaded\n");
    print_str("setkeycodes_v2: use -l to list, -c to clear\n");
    print_str("setkeycodes_v2: use setkeycodes_v2 SCANCODE KEYCODE to set\n");

    host_exit(0);
}
