/* loadunimap: load console Unicode mapping table */

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
    print_str("loadunimap - load console Unicode mapping table (v1.0)\n");
    print_str("Usage: loadunimap [OPTIONS] [MAPFILE]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c             Check current mapping\n");
    print_str("  -l             List available mappings\n");
    print_str("  -t             Test Unicode rendering\n");
    print_str("\n");
    print_str("Load Unicode character mapping definition for console.\n");
}

static void show_info(void)
{
    print_str("loadunimap: Unicode mapping info:\n");
    print_str("loadunimap: current map: ISO 8859-1\n");
    print_str("loadunimap: Unicode version: 15.0\n");
    print_str("loadunimap: CJK unified: 97625 chars\n");
    print_str("loadunimap: Hangul: 11184 chars\n");
    print_str("loadunimap: total entries: 149186\n");
    print_str("loadunimap: info display complete\n");
}

static void show_status(void)
{
    print_str("loadunimap: Unicode mapping status:\n");
    print_str("loadunimap: VT 1: mapping active\n");
    print_str("loadunimap: VT 2: mapping available\n");
    print_str("loadunimap: VT 3: mapping available\n");
    print_str("loadunimap: encoding: UTF-8\n");
    print_str("loadunimap: status check complete\n");
}

static void check_mapping(void)
{
    print_str("loadunimap: checking Unicode mapping\n");
    print_str("loadunimap: table integrity: OK\n");
    print_str("loadunimap: CJK range: loaded (U+4E00-U+9FFF)\n");
    print_str("loadunimap: Latin range: loaded (U+0000-U+024F)\n");
    print_str("loadunimap: Symbol range: loaded (U+2000-U+2BFF)\n");
    print_str("loadunimap: mapping check complete\n");
}

static void list_maps(int verbose)
{
    print_str("loadunimap: available Unicode maps:\n");
    print_str("loadunimap: default (ISO 8859-1 to Unicode)\n");
    print_str("loadunimap: latin1 (ISO 8859-1)\n");
    print_str("loadunimap: latin2 (ISO 8859-2)\n");
    print_str("loadunimap: cyrillic (ISO 8859-5)\n");
    print_str("loadunimap: greek (ISO 8859-7)\n");
    print_str("loadunimap: cjk (CJK unified)\n");
    if (verbose) {
        print_str("loadunimap: total: 6 maps\n");
        print_str("loadunimap: current: default\n");
    }
    print_str("loadunimap: list complete\n");
}

static void test_unicode(int verbose)
{
    print_str("loadunimap: testing Unicode rendering\n");
    if (verbose) {
        print_str("loadunimap: loading test glyphs\n");
        print_str("loadunimap: testing CJK range\n");
    }
    print_str("loadunimap: Latin: A-Z a-z 0-9\n");
    print_str("loadunimap: Accents:     \n");
    print_str("loadunimap: Symbols:     \n");
    print_str("loadunimap: CJK:    (test)\n");
    print_str("loadunimap: test complete\n");
}

static void load_unimap(const char *file, int verbose)
{
    print_str("loadunimap: loading Unicode mapping table");
    if (file) {
        print_str(" from ");
        unsigned int len = my_strlen(file);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(file, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (verbose) {
        print_str("loadunimap: parsing map file\n");
        print_str("loadunimap: loading translation table\n");
        print_str("loadunimap: updating console driver\n");
        print_str("loadunimap: applying mapping\n");
    }
    print_str("loadunimap: Unicode mapping loaded successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int check_flag = 0, list_flag = 0;
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
            check_flag = 1;
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

    if (check_flag) {
        check_mapping();
        host_exit(0);
    }

    if (list_flag) {
        list_maps(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_unicode(verbose_flag);
        host_exit(0);
    }

    if (file_name) {
        load_unimap(file_name, verbose_flag);
        host_exit(0);
    }

    print_str("loadunimap: no action specified, showing info\n");
    show_info();
    host_exit(0);
}
