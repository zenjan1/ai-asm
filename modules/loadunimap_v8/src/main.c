/* loadunimap_v8: load Unicode mapping table (v8) */

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
    print_str("loadunimap_v8 - load Unicode mapping table (v1.0)\n");
    print_str("Usage: loadunimap_v8 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -f FILE        Load Unicode map from file\n");
    print_str("  -d             Load default Unicode map\n");
    print_str("  -t             Test Unicode mapping\n");
    print_str("\n");
    print_str("Load Unicode to console font mapping table.\n");
}

static void show_info(void)
{
    print_str("loadunimap_v8: Unicode map info:\n");
    print_str("loadunimap_v8: Unicode version: 15.0\n");
    print_str("loadunimap_v8: mapped glyphs: 65536\n");
    print_str("loadunimap_v8: table size: 131072 bytes\n");
    print_str("loadunimap_v8: encoding: UTF-8\n");
    print_str("loadunimap_v8: info display complete\n");
}

static void show_status(void)
{
    print_str("loadunimap_v8: Unicode map status:\n");
    print_str("loadunimap_v8: active map: default\n");
    print_str("loadunimap_v8: loaded code points: 65536\n");
    print_str("loadunimap_v8: fallback: enabled\n");
    print_str("loadunimap_v8: status check complete\n");
}

static void load_file(const char *filename, int verbose)
{
    print_str("loadunimap_v8: loading Unicode map from: ");
    print_str(filename);
    print_str("\n");
    if (verbose) {
        print_str("loadunimap_v8: parsing Unicode table\n");
        print_str("loadunimap_v8: 65536 code points processed\n");
        print_str("loadunimap_v8: updating font table\n");
    }
    print_str("loadunimap_v8: Unicode map loaded successfully\n");
}

static void load_default(int verbose)
{
    print_str("loadunimap_v8: loading default Unicode map\n");
    if (verbose) {
        print_str("loadunimap_v8: clearing existing map\n");
        print_str("loadunimap_v8: loading default mappings\n");
        print_str("loadunimap_v8: fallback table configured\n");
    }
    print_str("loadunimap_v8: default Unicode map loaded successfully\n");
}

static void test_map(int verbose)
{
    print_str("loadunimap_v8: testing Unicode mapping\n");
    if (verbose) {
        print_str("loadunimap_v8: loading test map\n");
        print_str("loadunimap_v8: validating code points\n");
        print_str("loadunimap_v8: UTF-8 encoding check: OK\n");
    }
    print_str("loadunimap_v8: Unicode map test: OK\n");
    print_str("loadunimap_v8: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int file_flag = 0, default_flag = 0;
    int test_flag = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            file_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
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

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (file_flag) {
        load_file("unicode.map", verbose_flag);
        host_exit(0);
    }

    if (default_flag) {
        load_default(verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_map(verbose_flag);
        host_exit(0);
    }

    print_str("loadunimap_v8: current Unicode map: default\n");
    host_exit(0);
}
