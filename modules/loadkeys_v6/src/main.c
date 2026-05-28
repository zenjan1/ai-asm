/* loadkeys_v6: keyboard map loader (v6) */

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
    print_str("loadkeys_v6 - keyboard map loader (v6.0)\n");
    print_str("Usage: loadkeys_v6 [OPTIONS] [FILE]\n");
    print_str("  -f             Force load\n");
    print_str("  -c             Check syntax only\n");
    print_str("  -d             Load default keymap\n");
    print_str("  -v             Verbose output\n");
    print_str("  -l             List available keymaps\n");
    print_str("  -b             Bluetooth keyboard mode\n");
    print_str("  -m             Modify current keymap\n");
    print_str("  -i             Show info\n");
    print_str("\n");
    print_str("Load a keyboard map into the kernel.\n");
}

static void show_info(void)
{
    print_str("loadkeys_v6: keymap info:\n");
    print_str("loadkeys_v6: current keymap: us\n");
    print_str("loadkeys_v6: layout: QWERTY\n");
    print_str("loadkeys_v6: variant: standard\n");
    print_str("loadkeys_v6: charset: ISO 8859-1\n");
    print_str("loadkeys_v6: compose: disabled\n");
    print_str("loadkeys_v6: info display complete\n");
}

static void list_keymaps(int verbose)
{
    print_str("loadkeys_v6: available keymaps:\n");
    print_str("loadkeys_v6: us (QWERTY)\n");
    print_str("loadkeys_v6: uk (QWERTY)\n");
    print_str("loadkeys_v6: de (QWERTZ)\n");
    print_str("loadkeys_v6: fr (AZERTY)\n");
    print_str("loadkeys_v6: jp (JIS)\n");
    print_str("loadkeys_v6: ru (Cyrillic)\n");
    print_str("loadkeys_v6: es (QWERTY)\n");
    print_str("loadkeys_v6: dvorak (Dvorak)\n");
    if (verbose) {
        print_str("loadkeys_v6: total: 8 keymaps\n");
        print_str("loadkeys_v6: current: us\n");
    }
    print_str("loadkeys_v6: list complete\n");
}

static void load_default(void)
{
    print_str("loadkeys_v6: loading default keymap\n");
    print_str("loadkeys_v6: keymap: us loaded\n");
}

static void check_syntax(const char *file)
{
    print_str("loadkeys_v6: checking syntax");
    if (file) {
        print_str(" of ");
        unsigned int len = my_strlen(file);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(file, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    print_str("loadkeys_v6: syntax OK\n");
}

static void bluetooth_mode(int verbose)
{
    print_str("loadkeys_v6: bluetooth keyboard mode\n");
    if (verbose) {
        print_str("loadkeys_v6: scanning for bluetooth devices\n");
        print_str("loadkeys_v6: pairing with keyboard\n");
        print_str("loadkeys_v6: loading HID keymap\n");
    }
    print_str("loadkeys_v6: bluetooth keymap loaded\n");
}

static void modify_keymap(int verbose)
{
    print_str("loadkeys_v6: modifying current keymap\n");
    if (verbose) {
        print_str("loadkeys_v6: reading current keymap\n");
        print_str("loadkeys_v6: applying modifications\n");
        print_str("loadkeys_v6: writing updated keymap\n");
    }
    print_str("loadkeys_v6: keymap modified\n");
}

static void load_keymap(const char *file, int verbose, int force)
{
    print_str("loadkeys_v6: loading keymap");
    if (file) {
        print_str(" from ");
        unsigned int len = my_strlen(file);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(file, buf, len + 1);
        host_print(buf, len);
    }
    print_str("\n");
    if (force) {
        print_str("loadkeys_v6: force mode: overriding existing keymap\n");
    }
    if (verbose) {
        print_str("loadkeys_v6: parsing keymap file\n");
        print_str("loadkeys_v6: validating key definitions\n");
        print_str("loadkeys_v6: loading into kernel\n");
        print_str("loadkeys_v6: updating console driver\n");
    }
    print_str("loadkeys_v6: keymap loaded successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, force_flag = 0;
    int default_flag = 0, check_flag = 0;
    int list_flag = 0, bluetooth_flag = 0, modify_flag = 0;
    const char *file_name = 0;

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
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            default_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            check_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-b") == 0) {
            bluetooth_flag = 1;
        } else if (my_strcmp(arg, "-m") == 0) {
            modify_flag = 1;
        } else {
            file_name = arg;
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
        list_keymaps(verbose_flag);
        host_exit(0);
    }

    if (default_flag) {
        load_default();
        host_exit(0);
    }

    if (check_flag) {
        check_syntax(file_name);
        host_exit(0);
    }

    if (bluetooth_flag) {
        bluetooth_mode(verbose_flag);
        host_exit(0);
    }

    if (modify_flag) {
        modify_keymap(verbose_flag);
        host_exit(0);
    }

    if (file_name) {
        load_keymap(file_name, verbose_flag, force_flag);
        host_exit(0);
    }

    load_default();
    host_exit(0);
}
