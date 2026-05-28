/* showkey_v6: key display ultimate (v6) */

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

static void print_hex(int n)
{
    char buf[8];
    int i = 7;
    buf[7] = 0;
    if (n == 0) buf[--i] = '0';
    while (n > 0) {
        int d = n & 0xf;
        buf[--i] = (d < 10) ? ('0' + d) : ('a' + d - 10);
        n >>= 4;
    }
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
    print_str("showkey_v6 - display keyboard keys (v6.0)\n");
    print_str("Usage: showkey_v6 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a             ASCII mode\n");
    print_str("  -s             Scancodes mode\n");
    print_str("  -k             Keycodes mode\n");
    print_str("  -d             Decode mode\n");
    print_str("  -u             Unicode mode\n");
    print_str("  -t <seconds>   Timeout\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("\n");
    print_str("Display keyboard key press information.\n");
}

static void show_info(void)
{
    print_str("showkey_v6: keyboard display info:\n");
    print_str("showkey_v6: device: /dev/tty0\n");
    print_str("showkey_v6: mode: keycodes\n");
    print_str("showkey_v6: timeout: 10 seconds\n");
    print_str("showkey_v6: unicode support: enabled\n");
    print_str("showkey_v6: raw mode: disabled\n");
    print_str("showkey_v6: info display complete\n");
}

static void show_ascii(void)
{
    print_str("showkey_v6: ASCII mode - press keys to see ASCII values\n");
    print_str("showkey_v6: simulated output:\n");
    print_str("showkey_v6: key 'a' pressed: ascii ");
    print_int(97);
    print_str(" (0x");
    print_hex(97);
    print_str(")\n");
    print_str("showkey_v6: key 'b' pressed: ascii ");
    print_int(98);
    print_str(" (0x");
    print_hex(98);
    print_str(")\n");
    print_str("showkey_v6: key Enter pressed: ascii ");
    print_int(13);
    print_str(" (0x");
    print_hex(13);
    print_str(")\n");
    print_str("showkey_v6: ascii mode complete\n");
}

static void show_scancodes(void)
{
    print_str("showkey_v6: scancode mode - press keys to see scancodes\n");
    print_str("showkey_v6: simulated output:\n");
    print_str("showkey_v6: key pressed: scancode 0x");
    print_hex(0x1e);
    print_str("\n");
    print_str("showkey_v6: key released: scancode 0x");
    print_hex(0x9e);
    print_str("\n");
    print_str("showkey_v6: key pressed: scancode 0x");
    print_hex(0x30);
    print_str("\n");
    print_str("showkey_v6: key released: scancode 0x");
    print_hex(0xb0);
    print_str("\n");
    print_str("showkey_v6: scancode mode complete\n");
}

static void show_keycodes(void)
{
    print_str("showkey_v6: keycode mode - press keys to see keycodes\n");
    print_str("showkey_v6: simulated output:\n");
    print_str("showkey_v6: key pressed: keycode ");
    print_int(30);
    print_str("\n");
    print_str("showkey_v6: key released: keycode ");
    print_int(30);
    print_str("\n");
    print_str("showkey_v6: key pressed: keycode ");
    print_int(48);
    print_str("\n");
    print_str("showkey_v6: key released: keycode ");
    print_int(48);
    print_str("\n");
    print_str("showkey_v6: keycode mode complete\n");
}

static void show_decode(void)
{
    print_str("showkey_v6: decode mode - detailed key analysis\n");
    print_str("showkey_v6: key 'a': scancode 0x");
    print_hex(0x1e);
    print_str(" keycode 30 ascii ");
    print_int(97);
    print_str(" ('a')\n");
    print_str("showkey_v6: key 'b': scancode 0x");
    print_hex(0x30);
    print_str(" keycode 48 ascii ");
    print_int(98);
    print_str(" ('b')\n");
    print_str("showkey_v6: key Enter: scancode 0x");
    print_hex(0x1c);
    print_str(" keycode 28 ascii ");
    print_int(13);
    print_str(" ('\\n')\n");
    print_str("showkey_v6: key Space: scancode 0x");
    print_hex(0x39);
    print_str(" keycode 57 ascii ");
    print_int(32);
    print_str(" (' ')\n");
    print_str("showkey_v6: decode mode complete\n");
}

static void show_unicode(void)
{
    print_str("showkey_v6: unicode mode - press keys to see Unicode\n");
    print_str("showkey_v6: simulated output:\n");
    print_str("showkey_v6: key 'a' pressed: U+");
    print_hex(0x0061);
    print_str("\n");
    print_str("showkey_v6: key 'b' pressed: U+");
    print_hex(0x0062);
    print_str("\n");
    print_str("showkey_v6: key 'A' pressed: U+");
    print_hex(0x0041);
    print_str("\n");
    print_str("showkey_v6: unicode mode complete\n");
}

static void show_timeout(int seconds)
{
    print_str("showkey_v6: listening for keypresses (timeout: ");
    print_int(seconds);
    print_str("s)\n");
    print_str("showkey_v6: key pressed: keycode 30\n");
    print_str("showkey_v6: key released: keycode 30\n");
    print_str("showkey_v6: key pressed: keycode 48\n");
    print_str("showkey_v6: timeout expired\n");
}

static void show_verbose(void)
{
    print_str("showkey_v6: verbose mode - detailed key events\n");
    print_str("showkey_v6: event: key pressed\n");
    print_str("showkey_v6:   scancode: 0x");
    print_hex(0x1e);
    print_str("\n");
    print_str("showkey_v6:   keycode: 30\n");
    print_str("showkey_v6:   ascii: 97 ('a')\n");
    print_str("showkey_v6:   unicode: U+0061\n");
    print_str("showkey_v6:   modifiers: none\n");
    print_str("showkey_v6:   repeat: no\n");
    print_str("showkey_v6: event: key released\n");
    print_str("showkey_v6:   scancode: 0x");
    print_hex(0x9e);
    print_str("\n");
    print_str("showkey_v6: verbose mode complete\n");
}

static void show_default(void)
{
    print_str("showkey_v6: press keys to see keycodes\n");
    print_str("showkey_v6: simulated output:\n");
    print_str("showkey_v6: keycode ");
    print_int(30);
    print_str(" press\n");
    print_str("showkey_v6: keycode ");
    print_int(30);
    print_str(" release\n");
    print_str("showkey_v6: keycode ");
    print_int(48);
    print_str(" press\n");
    print_str("showkey_v6: keycode ");
    print_int(48);
    print_str(" release\n");
    print_str("showkey_v6: use -s for scancodes, -a for ascii\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, ascii_flag = 0, scancode_flag = 0;
    int keycode_flag = 0, decode_flag = 0, unicode_flag = 0;
    int timeout_flag = 0, verbose_flag = 0, info_flag = 0;
    int timeout_val = 10;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-a") == 0) {
            ascii_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            scancode_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            keycode_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            decode_flag = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unicode_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            timeout_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { timeout_val = timeout_val * 10 + (*p - '0'); p++; }
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

    if (ascii_flag) {
        show_ascii();
        host_exit(0);
    }

    if (scancode_flag) {
        show_scancodes();
        host_exit(0);
    }

    if (keycode_flag) {
        show_keycodes();
        host_exit(0);
    }

    if (decode_flag) {
        show_decode();
        host_exit(0);
    }

    if (unicode_flag) {
        show_unicode();
        host_exit(0);
    }

    if (verbose_flag) {
        show_verbose();
        host_exit(0);
    }

    if (timeout_flag) {
        show_timeout(timeout_val);
        host_exit(0);
    }

    show_default();
    host_exit(0);
}
