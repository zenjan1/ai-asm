/* setfont_v4: console font setting simulation (enhanced v4) */

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

static void print_int(int n, const char *prefix, const char *suffix)
{
    char buf[64];
    int i = 0;
    while (*prefix) buf[i++] = *prefix++;
    char tmp[16];
    int ti = 0;
    if (n == 0) { tmp[ti++] = '0'; }
    else { int m = n; if (m < 0) { buf[i++] = '-'; m = -m; } while (m > 0) { tmp[ti++] = '0' + (m % 10); m /= 10; } }
    int tj = 0;
    while (tj < ti) buf[i++] = tmp[ti - 1 - tj++];
    while (*suffix) buf[i++] = *suffix++;
    buf[i] = '\0';
    unsigned int pbuf = alloc(i + 1);
    copy_to_mem(buf, pbuf, i + 1);
    host_print(pbuf, i);
}

static void show_help(void)
{
    print_str("setfont_v4 - set console font (v4.0)\n");
    print_str("Usage: setfont_v4 [FONT] [OPTIONS]\n");
    print_str("  FONT           Font file name (e.g., lat0-16)\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -m MAP         Unicode map\n");
    print_str("  -C CONSOLE     Console device\n");
    print_str("  -v             Verbose output\n");
    print_str("\n");
    print_str("Set the console font (simulated).\n");
}

static void list_fonts(void)
{
    print_str("Available fonts:\n");
    print_str("  lat0-08  - Latin 8x8 (64 chars)\n");
    print_str("  lat0-12  - Latin 8x12 (96 chars)\n");
    print_str("  lat0-14  - Latin 8x14 (112 chars)\n");
    print_str("  lat0-16  - Latin 8x16 (128 chars)\n");
    print_str("  lat1-08  - Latin-1 8x8 (256 chars)\n");
    print_str("  lat1-16  - Latin-1 8x16 (256 chars)\n");
    print_str("  lat2-16  - Latin-2 8x16 (256 chars)\n");
    print_str("  cyr-16   - Cyrillic 8x16 (256 chars)\n");
    print_str("\n");
    print_str("setfont_v4: 8 fonts listed\n");
}

static void set_font(const char *font, int verbose)
{
    if (!font) font = "lat0-16";
    print_str("setfont_v4: loading font '");
    print_str(font);
    print_str("'\n");
    print_str("setfont_v4: font size: 8x16\n");
    print_str("setfont_v4: character map: Unicode\n");
    print_str("setfont_v4: screen buffer updated\n");
    if (verbose) {
        print_str("setfont_v4: glyph table loaded (512 entries)\n");
        print_str("setfont_v4: framebuffer reinitialized\n");
        print_str("setfont_v4: console dimensions: 80x25\n");
    }
    print_str("setfont_v4: font set successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int list_flag = 0;
    int verbose_flag = 0;
    char *font_name = 0;

    /* Parse arguments: skip argv[0] (module name) */
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--list") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0 || my_strcmp(arg, "-C") == 0) {
            /* Skip next argument (map or console) */
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
        } else if (arg[0] != '-') {
            font_name = arg;
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

    set_font(font_name, verbose_flag);

    host_exit(0);
}
