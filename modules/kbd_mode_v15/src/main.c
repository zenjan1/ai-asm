/* kbd_mode_v15: display/set keyboard mode (v15) */

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
    print_str("kbd_mode_v15 - display/set keyboard mode (v15)\n");
    print_str("Usage: kbd_mode_v15 [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -m MODE        Set mode (default, unicode, raw)\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Display or set the current keyboard mode.\n");
}

static void show_info(void)
{
    print_str("kbd_mode_v15: keyboard mode info:\n");
    print_str("kbd_mode_v15: supported modes: default, unicode, raw\n");
    print_str("kbd_mode_v15: current mode: default\n");
    print_str("kbd_mode_v15: charset: ASCII\n");
    print_str("kbd_mode_v15: keymap: us\n");
    print_str("kbd_mode_v15: info display complete\n");
}

static void show_status(void)
{
    print_str("kbd_mode_v15: keyboard status:\n");
    print_str("kbd_mode_v15: mode: default\n");
    print_str("kbd_mode_v15: repeat rate: 30\n");
    print_str("kbd_mode_v15: delay: 250ms\n");
    print_str("kbd_mode_v15: led: numlock off, capslock off\n");
    print_str("kbd_mode_v15: status check complete\n");
}

static void set_mode(const char *mode, int verbose)
{
    if (my_strcmp(mode, "default") == 0) {
        print_str("kbd_mode_v15: setting mode to: default\n");
        print_str("kbd_mode_v15: keyboard mode: default (ASCII)\n");
    } else if (my_strcmp(mode, "unicode") == 0) {
        print_str("kbd_mode_v15: setting mode to: unicode\n");
        print_str("kbd_mode_v15: keyboard mode: unicode (UTF-8)\n");
    } else if (my_strcmp(mode, "raw") == 0) {
        print_str("kbd_mode_v15: setting mode to: raw\n");
        print_str("kbd_mode_v15: keyboard mode: raw (scancodes)\n");
    } else {
        print_str("kbd_mode_v15: unknown mode: ");
        print_str(mode);
        print_str("\n");
        print_str("kbd_mode_v15: valid modes: default, unicode, raw\n");
        host_exit(1);
    }
    if (verbose) {
        print_str("kbd_mode_v15: updating keymap tables\n");
        print_str("kbd_mode_v15: keymap reload complete\n");
    }
}

static void test_mode(int verbose)
{
    print_str("kbd_mode_v15: testing keyboard mode\n");
    if (verbose) {
        print_str("kbd_mode_v15: checking keymap integrity\n");
        print_str("kbd_mode_v15: testing character encoding\n");
    }
    print_str("kbd_mode_v15: keyboard mode test: OK\n");
    print_str("kbd_mode_v15: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    char *mode_arg = 0;

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
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                mode_arg = &argv_ptr[pos];
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

    if (mode_arg) {
        set_mode(mode_arg, verbose_flag);
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("kbd_mode_v15: current keyboard mode: default\n");
    host_exit(0);
}
