/* loadkeys_v4: keymap loading ultimate (v4) */

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
    print_str("loadkeys_v4 - load keymap (v4.0)\n");
    print_str("Usage: loadkeys_v4 [OPTIONS] [keymap_file]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -b             Binary keymap format\n");
    print_str("  -c             Clear keymap\n");
    print_str("  -d             Load default keymap\n");
    print_str("  -m             Make keycode table\n");
    print_str("  -u             Unicode mode\n");
    print_str("\n");
    print_str("Load keyboard translation tables.\n");
}

static void load_binary(const char *file)
{
    print_str("loadkeys_v4: loading binary keymap: ");
    print_str(file);
    print_str("\n");
    print_str("loadkeys_v4: reading binary format\n");
    print_str("loadkeys_v4: loading ");
    print_int(256);
    print_str(" keycode entries\n");
    print_str("loadkeys_v4: loading compose table\n");
    print_str("loadkeys_v4: binary keymap loaded successfully\n");
}

static void clear_keymap(void)
{
    print_str("loadkeys_v4: clearing keyboard map\n");
    print_str("loadkeys_v4: all keycode mappings removed\n");
    print_str("loadkeys_v4: compose table cleared\n");
    print_str("loadkeys_v4: keymap cleared\n");
}

static void load_default(void)
{
    print_str("loadkeys_v4: loading default keymap: us.map\n");
    print_str("loadkeys_v4: loading 128 keycode entries\n");
    print_str("loadkeys_v4: loading F1-F12 string definitions\n");
    print_str("loadkeys_v4: loading compose table (8 entries)\n");
    print_str("loadkeys_v4: loading dead key mappings\n");
    print_str("loadkeys_v4: default keymap loaded successfully\n");
}

static void make_table(void)
{
    print_str("loadkeys_v4: generating keycode table\n");
    print_str("loadkeys_v4: table size: 256 entries\n");
    print_str("loadkeys_v4: scanning scan-to-keycode mappings\n");
    print_str("loadkeys_v4: table generated\n");
}

static void load_unicode(const char *file)
{
    print_str("loadkeys_v4: loading unicode keymap: ");
    print_str(file);
    print_str("\n");
    print_str("loadkeys_v4: unicode mode enabled\n");
    print_str("loadkeys_v4: loading unicode compose table\n");
    print_str("loadkeys_v4: unicode keymap loaded successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int binary_flag = 0;
    int clear_flag = 0;
    int default_flag = 0;
    int mktable_flag = 0;
    int unicode_flag = 0;
    char *keymap_file = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-b") == 0) {
            binary_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            clear_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            mktable_flag = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
        } else if (arg[0] != '-') {
            keymap_file = arg;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (clear_flag) {
        clear_keymap();
        host_exit(0);
    }

    if (default_flag) {
        load_default();
        host_exit(0);
    }

    if (mktable_flag) {
        make_table();
        host_exit(0);
    }

    if (keymap_file) {
        if (binary_flag) {
            load_binary(keymap_file);
        } else if (unicode_flag) {
            load_unicode(keymap_file);
        } else {
            print_str("loadkeys_v4: loading keymap: ");
            print_str(keymap_file);
            print_str("\n");
            print_str("loadkeys_v4: keymap loaded successfully\n");
        }
        host_exit(0);
    }

    /* Default */
    print_str("loadkeys_v4: current keymap: us.map\n");
    print_str("loadkeys_v4: 128 keycode entries loaded\n");
    print_str("loadkeys_v4: use -d for default, -c to clear\n");

    host_exit(0);
}
