/* kbd_mode_v4: keyboard mode setting simulation (enhanced v4) */

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
    print_str("kbd_mode_v4 - set keyboard mode (v4.0)\n");
    print_str("Usage: kbd_mode_v4 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -u             Unicode mode\n");
    print_str("  -a             ASCII mode\n");
    print_str("  -r             Raw mode\n");
    print_str("  -m             Medium-raw mode\n");
    print_str("  -x             XLATE mode (default)\n");
    print_str("\n");
    print_str("Set or query the keyboard translation mode.\n");
}

static void show_current_mode(void)
{
    print_str("kbd_mode_v4: current keyboard mode: XLATE\n");
    print_str("kbd_mode_v4: keycode range: 0-127\n");
    print_str("kbd_mode_v4: LED state: numlock=off capslock=off scrolllock=off\n");
    print_str("kbd_mode_v4: repeat rate: delay=250ms period=33ms\n");
    print_str("kbd_mode_v4: mode query complete\n");
}

static void set_mode(const char *mode)
{
    print_str("kbd_mode_v4: setting keyboard mode to ");
    print_str(mode);
    print_str("\n");
    print_str("kbd_mode_v4: mode ");
    print_str(mode);
    print_str(" activated\n");
    print_str("kbd_mode_v4: keyboard translation table updated\n");
    print_str("kbd_mode_v4: mode set successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int unicode_flag = 0;
    int ascii_flag = 0;
    int raw_flag = 0;
    int medium_raw_flag = 0;
    int xlate_flag = 0;

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
        }
        if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
        }
        if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
        }
        if (my_strcmp(arg, "-r") == 0) {
            raw_flag = 1;
        }
        if (my_strcmp(arg, "-m") == 0) {
            medium_raw_flag = 1;
        }
        if (my_strcmp(arg, "-x") == 0) {
            xlate_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (unicode_flag) {
        set_mode("Unicode");
    } else if (ascii_flag) {
        set_mode("ASCII");
    } else if (raw_flag) {
        set_mode("Raw");
    } else if (medium_raw_flag) {
        set_mode("Medium-raw");
    } else if (xlate_flag) {
        set_mode("XLATE");
    } else {
        show_current_mode();
    }

    host_exit(0);
}
