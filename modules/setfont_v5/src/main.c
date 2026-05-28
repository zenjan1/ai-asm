/* setfont_v5: console fonts ultimate (v5) */

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
    print_str("setfont_v5 - set console font (v5.0)\n");
    print_str("Usage: setfont_v5 [OPTIONS] [font]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -f <font>      Set font by name\n");
    print_str("  -l             List available fonts\n");
    print_str("  -v             Verbose output\n");
    print_str("  -d             Reset to default font\n");
    print_str("  -s <size>      Set font size (8, 10, 12, 14, 16)\n");
    print_str("\n");
    print_str("Set or query console font settings.\n");
}

static void list_fonts(void)
{
    print_str("setfont_v5: available fonts:\n");
    print_str("setfont_v5: lat0-08 (8x8, 256 chars)\n");
    print_str("setfont_v5: lat0-10 (10x10, 256 chars)\n");
    print_str("setfont_v5: lat0-12 (12x12, 256 chars)\n");
    print_str("setfont_v5: lat0-14 (14x14, 256 chars)\n");
    print_str("setfont_v5: lat0-16 (16x16, 256 chars)\n");
    print_str("setfont_v5: lat1-16 (16x16, 512 chars)\n");
    print_str("setfont_v5: lat2-16 (16x16, 512 chars)\n");
    print_str("setfont_v5: cyr-16 (16x16, Cyrillic)\n");
    print_str("setfont_v5: total fonts: 8\n");
}

static void set_font(const char *name, int verbose)
{
    print_str("setfont_v5: setting font: ");
    print_str(name);
    print_str("\n");
    if (verbose) {
        print_str("setfont_v5: loading font data\n");
        print_str("setfont_v5: updating character map\n");
        print_str("setfont_v5: refreshing display\n");
    }
    print_str("setfont_v5: font set successfully\n");
}

static void set_size(int size, int verbose)
{
    print_str("setfont_v5: setting font size to ");
    print_int(size);
    print_str("\n");
    if (verbose) {
        print_str("setfont_v5: recalculating screen dimensions\n");
        print_str("setfont_v5: rows: 25, cols: 80\n");
    }
    print_str("setfont_v5: size set successfully\n");
}

static void reset_default(int verbose)
{
    print_str("setfont_v5: resetting to default font: lat0-16\n");
    if (verbose) {
        print_str("setfont_v5: loading default font data\n");
        print_str("setfont_v5: restoring default character map\n");
    }
    print_str("setfont_v5: default font restored\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int font_flag = 0;
    int list_flag = 0;
    int verbose_flag = 0;
    int default_flag = 0;
    int size_flag = 0;
    char *font_val = 0;
    int size_val = 0;

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
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            font_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) font_val = &argv_ptr[pos];
        } else if (my_strcmp(arg, "-s") == 0) {
            size_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                size_val = 0;
                while (*p >= '0' && *p <= '9') {
                    size_val = size_val * 10 + (*p - '0');
                    p++;
                }
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
        list_fonts();
        host_exit(0);
    }

    if (default_flag) {
        reset_default(verbose_flag);
        host_exit(0);
    }

    if (font_flag && font_val) {
        set_font(font_val, verbose_flag);
        host_exit(0);
    }

    if (size_flag && size_val) {
        set_size(size_val, verbose_flag);
        host_exit(0);
    }

    /* Default: show current */
    print_str("setfont_v5: current font: lat0-16\n");
    print_str("setfont_v5: font size: 16\n");
    print_str("setfont_v5: screen: 80x25\n");
    print_str("setfont_v5: use -l to list, -f to set font\n");

    host_exit(0);
}
