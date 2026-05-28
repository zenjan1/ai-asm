/* setfont_v7: set console font (v7) */

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
    print_str("setfont_v7 - set console font (v1.0)\n");
    print_str("Usage: setfont_v7 [OPTIONS] [FONTFILE]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -l             List available fonts\n");
    print_str("  -f FONT        Set font by name\n");
    print_str("  -t             Test font rendering\n");
    print_str("\n");
    print_str("Load and set console font for virtual terminals.\n");
}

static void show_info(void)
{
    print_str("setfont_v7: console font info:\n");
    print_str("setfont_v7: current font: default (8x16)\n");
    print_str("setfont_v7: width: 8\n");
    print_str("setfont_v7: height: 16\n");
    print_str("setfont_v7: charmap: ISO 8859-1\n");
    print_str("setfont_v7: VT 1: font active\n");
    print_str("setfont_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("setfont_v7: console font status:\n");
    print_str("setfont_v7: VT 1: default (8x16)\n");
    print_str("setfont_v7: VT 2: default (8x16)\n");
    print_str("setfont_v7: VT 3: default (8x16)\n");
    print_str("setfont_v7: mode: 80x25\n");
    print_str("setfont_v7: status check complete\n");
}

static void list_fonts(int verbose)
{
    print_str("setfont_v7: available fonts:\n");
    print_str("setfont_v7: default (8x16)\n");
    print_str("setfont_v7: Lat2-Terminus16\n");
    print_str("setfont_v7: cyr-sun16\n");
    print_str("setfont_v7: koi8u-8x16\n");
    print_str("setfont_v7: bg-8x16\n");
    if (verbose) {
        print_str("setfont_v7: total: 5 fonts\n");
        print_str("setfont_v7: current: default\n");
    }
    print_str("setfont_v7: list complete\n");
}

static void set_font(const char *name, int verbose)
{
    print_str("setfont_v7: setting font");
    if (name) {
        print_str(" to ");
        unsigned int len = my_strlen(name);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(name, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (verbose) {
        print_str("setfont_v7: loading font data\n");
        print_str("setfont_v7: updating font table\n");
        print_str("setfont_v7: refreshing screen\n");
    }
    print_str("setfont_v7: font set successfully\n");
}

static void test_font(int verbose)
{
    print_str("setfont_v7: testing font rendering\n");
    if (verbose) {
        print_str("setfont_v7: loading test glyphs\n");
        print_str("setfont_v7: rendering test pattern\n");
    }
    print_str("setfont_v7: A-Z: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    print_str("setfont_v7: a-z: abcdefghijklmnopqrstuvwxyz\n");
    print_str("setfont_v7: 0-9: 0123456789\n");
    print_str("setfont_v7: font test: PASSED\n");
    print_str("setfont_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int list_flag = 0, font_flag = 0;
    int test_flag = 0;
    const char *font_name = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            font_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                font_name = &argv_ptr[pos];
            }
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else {
            font_name = arg;
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
        list_fonts(verbose_flag);
        host_exit(0);
    }

    if (font_flag || font_name) {
        set_font(font_name, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_font(verbose_flag);
        host_exit(0);
    }

    print_str("setfont_v7: default (8x16) font active\n");
    host_exit(0);
}
