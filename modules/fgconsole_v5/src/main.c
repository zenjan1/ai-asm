/* fgconsole_v5: foreground console query ultimate (v5) */

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
    print_str("fgconsole_v5 - foreground console query (v5.0)\n");
    print_str("Usage: fgconsole_v5 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -s             Show status only\n");
    print_str("  -l             List all consoles\n");
    print_str("  -n             Show next available VT\n");
    print_str("  -p             Show previous VT\n");
    print_str("\n");
    print_str("Query foreground console information.\n");
}

static void show_status(void)
{
    print_str("fgconsole_v5: foreground VT: 7\n");
    print_str("fgconsole_v5: console mode: graphics (X11)\n");
    print_str("fgconsole_v5: keyboard mode: XLATE\n");
    print_str("fgconsole_v5: console status: active\n");
}

static void list_consoles(void)
{
    print_str("fgconsole_v5: all console information:\n");
    print_str("fgconsole_v5: VT1  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT2  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT3  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT4  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT5  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT6  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT7  - foreground (graphics 1024x768)\n");
    print_str("fgconsole_v5: VT8  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT9  - background (text 80x25)\n");
    print_str("fgconsole_v5: VT10 - background (text 80x25)\n");
    print_str("fgconsole_v5: VT11 - background (text 80x25)\n");
    print_str("fgconsole_v5: VT12 - background (text 80x25)\n");
    print_str("fgconsole_v5: total VTs: 12, active: 7\n");
}

static void show_next(void)
{
    print_str("fgconsole_v5: current VT: 7\n");
    print_str("fgconsole_v5: next available VT: 8\n");
    print_str("fgconsole_v5: VT8 status: idle\n");
}

static void show_previous(void)
{
    print_str("fgconsole_v5: current VT: 7\n");
    print_str("fgconsole_v5: previous VT: 6\n");
    print_str("fgconsole_v5: VT6 status: idle\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int status_flag = 0;
    int list_flag = 0;
    int next_flag = 0;
    int previous_flag = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            next_flag = 1;
        } else if (my_strcmp(arg, "-p") == 0) {
            previous_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (list_flag) {
        list_consoles();
        host_exit(0);
    }

    if (next_flag) {
        show_next();
        host_exit(0);
    }

    if (previous_flag) {
        show_previous();
        host_exit(0);
    }

    /* Default: show current */
    print_str("fgconsole_v5: foreground VT: 7\n");
    print_str("fgconsole_v5: console mode: graphics\n");
    print_str("fgconsole_v5: use -s for status, -l for list\n");

    host_exit(0);
}
