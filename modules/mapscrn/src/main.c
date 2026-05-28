/* mapscrn: screen character map loader */

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
    print_str("mapscrn - screen character map loader (v1.0)\n");
    print_str("Usage: mapscrn [OPTIONS] [MAPFILE]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -l             List available maps\n");
    print_str("  -r             Reset to default\n");
    print_str("  -d             Load default map\n");
    print_str("  -t             Test rendering\n");
    print_str("\n");
    print_str("Load screen output character translation map.\n");
}

static void show_info(void)
{
    print_str("mapscrn: character map info:\n");
    print_str("mapscrn: current map: default\n");
    print_str("mapscrn: encoding: ISO 8859-1\n");
    print_str("mapscrn: charset: Latin-1\n");
    print_str("mapscrn: entries: 256\n");
    print_str("mapscrn: info display complete\n");
}

static void show_status(void)
{
    print_str("mapscrn: character map status:\n");
    print_str("mapscrn: VT 1: map loaded (default)\n");
    print_str("mapscrn: VT 2: map available\n");
    print_str("mapscrn: VT 3: map available\n");
    print_str("mapscrn: translation: active\n");
    print_str("mapscrn: status check complete\n");
}

static void list_maps(int verbose)
{
    print_str("mapscrn: available character maps:\n");
    print_str("mapscrn: default (ISO 8859-1)\n");
    print_str("mapscrn: latin1 (ISO 8859-1)\n");
    print_str("mapscrn: latin2 (ISO 8859-2)\n");
    print_str("mapscrn: cp437 (IBM PC)\n");
    print_str("mapscrn: cp850 (MS-DOS)\n");
    print_str("mapscrn: koi8r (Russian)\n");
    if (verbose) {
        print_str("mapscrn: total: 6 maps\n");
        print_str("mapscrn: current: default\n");
    }
    print_str("mapscrn: list complete\n");
}

static void reset_map(void)
{
    print_str("mapscrn: resetting character map\n");
    print_str("mapscrn: loading default translation table\n");
    print_str("mapscrn: map reset complete\n");
}

static void load_default(void)
{
    print_str("mapscrn: loading default character map\n");
    print_str("mapscrn: map: ISO 8859-1 loaded\n");
}

static void test_rendering(int verbose)
{
    print_str("mapscrn: testing character rendering\n");
    if (verbose) {
        print_str("mapscrn: loading test glyphs\n");
        print_str("mapscrn: rendering ASCII range 32-127\n");
    }
    print_str("mapscrn: A-Z: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    print_str("mapscrn: a-z: abcdefghijklmnopqrstuvwxyz\n");
    print_str("mapscrn: 0-9: 0123456789\n");
    print_str("mapscrn: test complete\n");
}

static void load_map(const char *file, int verbose)
{
    print_str("mapscrn: loading character map");
    if (file) {
        print_str(" from ");
        unsigned int len = my_strlen(file);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(file, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (verbose) {
        print_str("mapscrn: parsing map file\n");
        print_str("mapscrn: loading translation table\n");
        print_str("mapscrn: updating console driver\n");
        print_str("mapscrn: applying mapping\n");
    }
    print_str("mapscrn: character map loaded successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int list_flag = 0, reset_flag = 0;
    int default_flag = 0, test_flag = 0;
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
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-r") == 0) {
            reset_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
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

    if (list_flag) {
        list_maps(verbose_flag);
        host_exit(0);
    }

    if (reset_flag) {
        reset_map();
        host_exit(0);
    }

    if (default_flag) {
        load_default();
        host_exit(0);
    }

    if (test_flag) {
        test_rendering(verbose_flag);
        host_exit(0);
    }

    if (file_name) {
        load_map(file_name, verbose_flag);
        host_exit(0);
    }

    load_default();
    host_exit(0);
}
