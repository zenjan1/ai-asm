/* fgconsole_v6: foreground console display (v6) */

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
    print_str("fgconsole_v6 - foreground console display (v6.0)\n");
    print_str("Usage: fgconsole_v6 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -n             Next console\n");
    print_str("  -p             Previous console\n");
    print_str("  -a             Show active console\n");
    print_str("  -d             Show details\n");
    print_str("\n");
    print_str("Display the current foreground virtual console number.\n");
}

static void show_info(void)
{
    print_str("fgconsole_v6: foreground console info:\n");
    print_str("fgconsole_v6: active VT: 1\n");
    print_str("fgconsole_v6: console mode: graphics\n");
    print_str("fgconsole_v6: auto switch: enabled\n");
    print_str("fgconsole_v6: total VTs: 12\n");
    print_str("fgconsole_v6: info display complete\n");
}

static void show_status(void)
{
    print_str("fgconsole_v6: console status:\n");
    print_str("fgconsole_v6: foreground VT: 1\n");
    print_str("fgconsole_v6: background VTs: 2-12\n");
    print_str("fgconsole_v6: console type: graphics\n");
    print_str("fgconsole_v6: keyboard driver: active\n");
    print_str("fgconsole_v6: status check complete\n");
}

static void show_active(void)
{
    print_str("fgconsole_v6: 1\n");
}

static void show_next(void)
{
    print_str("fgconsole_v6: current VT: 1\n");
    print_str("fgconsole_v6: next VT: 2\n");
}

static void show_previous(void)
{
    print_str("fgconsole_v6: current VT: 1\n");
    print_str("fgconsole_v6: previous VT: 12\n");
}

static void show_details(int verbose)
{
    print_str("fgconsole_v6: foreground console details:\n");
    print_str("fgconsole_v6: VT 1 (active)\n");
    print_str("fgconsole_v6: mode: graphics (KD_GRAPHICS)\n");
    print_str("fgconsole_v6: resolution: 800x600\n");
    print_str("fgconsole_v6: depth: 32bpp RGBA\n");
    print_str("fgconsole_v6: framebuffer: mapped\n");
    if (verbose) {
        print_str("fgconsole_v6: keyboard layout: us\n");
        print_str("fgconsole_v6: keymap: loaded\n");
        print_str("fgconsole_v6: LED state: numlock=off, capslock=off, scrolllock=off\n");
    }
    print_str("fgconsole_v6: details display complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, verbose_flag = 0;
    int info_flag = 0, status_flag = 0;
    int next_flag = 0, prev_flag = 0;
    int active_flag = 0, details_flag = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            next_flag = 1;
        } else if (my_strcmp(arg, "-p") == 0) {
            prev_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            active_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            details_flag = 1;
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

    if (active_flag) {
        show_active();
        host_exit(0);
    }

    if (next_flag) {
        show_next();
        host_exit(0);
    }

    if (prev_flag) {
        show_previous();
        host_exit(0);
    }

    if (details_flag) {
        show_details(verbose_flag);
        host_exit(0);
    }

    /* Default: show active console */
    show_active();
    host_exit(0);
}
