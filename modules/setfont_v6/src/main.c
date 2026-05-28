/* setfont_v6: console font setting (v6) */

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
    print_str("setfont_v6 - set console font (v6.0)\n");
    print_str("Usage: setfont_v6 [OPTIONS]\n");
    print_str("  -f <name>    Font name\n");
    print_str("  -h <num>     Font height\n");
    print_str("  -w <num>     Font width\n");
    print_str("  -v           Verbose output\n");
    print_str("  -i           Show info\n");
    print_str("  -l           List available fonts\n");
    print_str("  -d           Set default font\n");
    print_str("  -r           Reset font\n");
    print_str("\n");
    print_str("Set the virtual console font.\n");
}

static void show_info(void)
{
    print_str("setfont_v6: font info:\n");
    print_str("setfont_v6: current font: Lat2-Terminus16\n");
    print_str("setfont_v6: size: 8x16\n");
    print_str("setfont_v6: charset: ISO 8859-2\n");
    print_str("setfont_v6: map: default\n");
    print_str("setfont_v6: info display complete\n");
}

static void list_fonts(int verbose)
{
    print_str("setfont_v6: available fonts:\n");
    print_str("setfont_v6: Lat2-Terminus16 (8x16)\n");
    print_str("setfont_v6: LatArHebGre-16 (8x16)\n");
    print_str("setfont_v6: sun12x22 (12x22)\n");
    print_str("setfont_v6: vga8x8 (8x8)\n");
    print_str("setfont_v6: vga8x14 (8x14)\n");
    print_str("setfont_v6: vga8x16 (8x16)\n");
    print_str("setfont_v6: default8x16 (8x16)\n");
    print_str("setfont_v6: fixed13x20 (13x20)\n");
    if (verbose) {
        print_str("setfont_v6: total: 8 fonts\n");
        print_str("setfont_v6: current: Lat2-Terminus16\n");
    }
    print_str("setfont_v6: list complete\n");
}

static void set_default(void)
{
    print_str("setfont_v6: setting default font\n");
    print_str("setfont_v6: loading Lat2-Terminus16\n");
    print_str("setfont_v6: font set to Lat2-Terminus16 (8x16)\n");
}

static void reset_font(void)
{
    print_str("setfont_v6: resetting font to default\n");
    print_str("setfont_v6: loading default8x16\n");
    print_str("setfont_v6: font reset complete\n");
}

static void set_font(const char *name, int height, int width, int verbose)
{
    print_str("setfont_v6: setting font");
    if (name) {
        print_str(" ");
        unsigned int len = my_strlen(name);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(name, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (verbose) {
        print_str("setfont_v6: loading font file\n");
        print_str("setfont_v6: parsing font data\n");
        if (height > 0) {
            print_str("setfont_v6: height: ");
            print_int(height);
            print_str("\n");
        }
        if (width > 0) {
            print_str("setfont_v6: width: ");
            print_int(width);
            print_str("\n");
        }
        print_str("setfont_v6: uploading to framebuffer\n");
        print_str("setfont_v6: updating console driver\n");
    }
    print_str("setfont_v6: font applied successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int list_flag = 0, verbose_flag = 0;
    int default_flag = 0, reset_flag = 0;
    int font_flag = 0, height_val = 0, width_val = 0;
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
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-r") == 0) {
            reset_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            font_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                font_name = &argv_ptr[pos];
            }
        } else if (my_strcmp(arg, "-H") == 0) {
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { height_val = height_val * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-W") == 0) {
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { width_val = width_val * 10 + (*p - '0'); p++; }
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
        list_fonts(verbose_flag);
        host_exit(0);
    }

    if (default_flag) {
        set_default();
        host_exit(0);
    }

    if (reset_flag) {
        reset_font();
        host_exit(0);
    }

    if (font_flag || font_name) {
        set_font(font_name, height_val, width_val, verbose_flag);
        host_exit(0);
    }

    /* Default: set default font */
    set_default();
    host_exit(0);
}
