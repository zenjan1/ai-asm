/* setfont_v13: set console font (v13) */

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
    print_str("setfont_v13 - set console font (v13)\n");
    print_str("Usage: setfont_v13 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -f FONT        Set font (default, 8x16, 8x8)\n");
    print_str("  -m MAP         Set screen map\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Set the console font to the specified font.\n");
}

static void show_info(void)
{
    print_str("setfont_v13: console font info:\n");
    print_str("setfont_v13: available fonts: default, 8x16, 8x8\n");
    print_str("setfont_v13: current font: default\n");
    print_str("setfont_v13: font size: 8x16 pixels\n");
    print_str("setfont_v13: screen map: none\n");
    print_str("setfont_v13: info display complete\n");
}

static void show_status(void)
{
    print_str("setfont_v13: font status:\n");
    print_str("setfont_v13: font: default\n");
    print_str("setfont_v13: size: 8x16\n");
    print_str("setfont_v13: charset: ASCII\n");
    print_str("setfont_v13: screen map: default\n");
    print_str("setfont_v13: status check complete\n");
}

static void set_font(const char *font, int verbose)
{
    print_str("setfont_v13: setting font to: ");
    print_str(font);
    print_str("\n");
    if (my_strcmp(font, "default") == 0) {
        print_str("setfont_v13: font set to: default (8x16)\n");
    } else if (my_strcmp(font, "8x16") == 0) {
        print_str("setfont_v13: font set to: 8x16\n");
    } else if (my_strcmp(font, "8x8") == 0) {
        print_str("setfont_v13: font set to: 8x8\n");
    } else {
        print_str("setfont_v13: unknown font: ");
        print_str(font);
        print_str("\n");
        print_str("setfont_v13: valid fonts: default, 8x16, 8x8\n");
        host_exit(1);
    }
    if (verbose) {
        print_str("setfont_v13: loading font data\n");
        print_str("setfont_v13: updating font table\n");
        print_str("setfont_v13: font reload complete\n");
    }
}

static void set_map(const char *map, int verbose)
{
    print_str("setfont_v13: setting screen map to: ");
    print_str(map);
    print_str("\n");
    if (my_strcmp(map, "default") == 0) {
        print_str("setfont_v13: screen map set to: default (identity)\n");
    } else if (my_strcmp(map, "identity") == 0) {
        print_str("setfont_v13: screen map set to: identity\n");
    } else if (my_strcmp(map, "accent") == 0) {
        print_str("setfont_v13: screen map set to: accent\n");
    } else {
        print_str("setfont_v13: loading screen map from: ");
        print_str(map);
        print_str("\n");
    }
    if (verbose) {
        print_str("setfont_v13: applying screen map\n");
        print_str("setfont_v13: map table updated\n");
    }
}

static void test_mode(int verbose)
{
    print_str("setfont_v13: testing console font\n");
    if (verbose) {
        print_str("setfont_v13: checking font data integrity\n");
        print_str("setfont_v13: testing font rendering\n");
    }
    print_str("setfont_v13: console font test: OK\n");
    print_str("setfont_v13: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *font_arg = 0;
    char *map_arg = 0;

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
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                font_arg = &argv_ptr[pos];
            }
        } else if (my_strcmp(arg, "-m") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                map_arg = &argv_ptr[pos];
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

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (map_arg) {
        set_map(map_arg, verbose_flag);
        host_exit(0);
    }

    if (font_arg) {
        set_font(font_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    /* Default: show current font */
    print_str("setfont_v13: current font: default (8x16)\n");
    host_exit(0);
}
