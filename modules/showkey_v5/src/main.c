/* showkey_v5: key display ultimate (v5) */

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
    print_str("showkey_v5 - show keycodes (v5.0)\n");
    print_str("Usage: showkey_v5 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a             Show all (scancode+keycode+ascii)\n");
    print_str("  -s             Show scancodes only\n");
    print_str("  -k             Show keycodes only\n");
    print_str("  -d             Decode last keypress\n");
    print_str("\n");
    print_str("Display keyboard keypress information.\n");
}

static void show_all(void)
{
    print_str("showkey_v5: all keypress information:\n");
    print_str("showkey_v5: key 1: 'a'\n");
    print_str("  scancode: 0x1e\n");
    print_str("  keycode:  30\n");
    print_str("  ascii:    0x61\n");
    print_str("showkey_v5: key 2: 'b'\n");
    print_str("  scancode: 0x30\n");
    print_str("  keycode:  48\n");
    print_str("  ascii:    0x62\n");
    print_str("showkey_v5: key 3: 'Enter'\n");
    print_str("  scancode: 0x1c\n");
    print_str("  keycode:  28\n");
    print_str("  ascii:    0x0d\n");
    print_str("showkey_v5: key 4: 'Escape'\n");
    print_str("  scancode: 0x01\n");
    print_str("  keycode:  1\n");
    print_str("  ascii:    0x1b\n");
    print_str("showkey_v5: key 5: 'Space'\n");
    print_str("  scancode: 0x39\n");
    print_str("  keycode:  57\n");
    print_str("  ascii:    0x20\n");
    print_str("showkey_v5: key 6: 'Tab'\n");
    print_str("  scancode: 0x0f\n");
    print_str("  keycode:  15\n");
    print_str("  ascii:    0x09\n");
    print_str("showkey_v5: all display complete\n");
}

static void show_scancodes(void)
{
    print_str("showkey_v5: scancode display:\n");
    print_str("showkey_v5: press: scancode 0x");
    print_hex(0x1e);
    print_str(" release: scancode 0x");
    print_hex(0x9e);
    print_str(" (a)\n");
    print_str("showkey_v5: press: scancode 0x");
    print_hex(0x30);
    print_str(" release: scancode 0x");
    print_hex(0xb0);
    print_str(" (b)\n");
    print_str("showkey_v5: press: scancode 0x");
    print_hex(0x1c);
    print_str(" release: scancode 0x");
    print_hex(0x9c);
    print_str(" (Enter)\n");
    print_str("showkey_v5: press: scancode 0x");
    print_hex(0x01);
    print_str(" release: scancode 0x");
    print_hex(0x81);
    print_str(" (Escape)\n");
    print_str("showkey_v5: scancode display complete\n");
}

static void show_keycodes(void)
{
    print_str("showkey_v5: keycode display:\n");
    print_str("showkey_v5: a -> keycode 30 (0x1e)\n");
    print_str("showkey_v5: b -> keycode 48 (0x30)\n");
    print_str("showkey_v5: Enter -> keycode 28 (0x1c)\n");
    print_str("showkey_v5: Escape -> keycode 1 (0x01)\n");
    print_str("showkey_v5: Space -> keycode 57 (0x39)\n");
    print_str("showkey_v5: Tab -> keycode 15 (0x0f)\n");
    print_str("showkey_v5: keycode display complete\n");
}

static void show_decode(void)
{
    print_str("showkey_v5: decode last keypress:\n");
    print_str("showkey_v5: raw scancode: 0x1e\n");
    print_str("showkey_v5: translated keycode: 30\n");
    print_str("showkey_v5: keysym: 'a' (0x61)\n");
    print_str("showkey_v5: modifiers: none\n");
    print_str("showkey_v5: state: press\n");
    print_str("showkey_v5: decode complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int all_flag = 0;
    int scancode_flag = 0;
    int keycode_flag = 0;
    int decode_flag = 0;

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
            all_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            scancode_flag = 1;
        } else if (my_strcmp(arg, "-k") == 0) {
            keycode_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            decode_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (all_flag) {
        show_all();
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

    /* Default: show summary */
    print_str("showkey_v5: keyboard key information display\n");
    print_str("showkey_v5: use -a for all, -s for scancodes\n");
    print_str("showkey_v5: use -k for keycodes, -d for decode\n");
    print_str("showkey_v5: keyboard mode: XLATE\n");
    print_str("showkey_v5: display complete\n");

    host_exit(0);
}
