/* showkey_v4: keyboard key display simulation (enhanced v4) */

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

static int parse_int(const char *s)
{
    int val = 0;
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s - '0');
        s++;
    }
    return val;
}

static void show_help(void)
{
    print_str("showkey_v4 - display keyboard key information (v4.0)\n");
    print_str("Usage: showkey_v4 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a             Show ASCII values\n");
    print_str("  -s             Show scan codes\n");
    print_str("  -k             Show keycodes\n");
    print_str("  --lines=N      Set display lines\n");
    print_str("\n");
    print_str("Display simulated key press information for virtual terminal.\n");
}

static void show_ascii_table(void)
{
    print_str("ASCII key value table:\n");
    print_str("  keycode  2 (1)      -> 49 '1'\n");
    print_str("  keycode  3 (2)      -> 50 '2'\n");
    print_str("  keycode  4 (3)      -> 51 '3'\n");
    print_str("  keycode  5 (4)      -> 52 '4'\n");
    print_str("  keycode  6 (5)      -> 53 '5'\n");
    print_str("  keycode  7 (6)      -> 54 '6'\n");
    print_str("  keycode  8 (7)      -> 55 '7'\n");
    print_str("  keycode  9 (8)      -> 56 '8'\n");
    print_str("  keycode 10 (9)      -> 57 '9'\n");
    print_str("  keycode 11 (0)      -> 48 '0'\n");
    print_str("  keycode 16 (q)      -> 113 'q'\n");
    print_str("  keycode 30 (a)      -> 97  'a'\n");
    print_str("  keycode 44 (z)      -> 122 'z'\n");
    print_str("  keycode 57 (space)  -> 32  ' '\n");
    print_str("  keycode 28 (enter)  -> 10  '\\n'\n");
    print_str("\n");
    print_str("showkey v4.0: ASCII table displayed\n");
}

static void show_scan_codes(void)
{
    print_str("Scan code table:\n");
    print_str("  key press   keycode  scancode  make code\n");
    print_str("  1           2       0x02      0x82\n");
    print_str("  2           3       0x03      0x83\n");
    print_str("  3           4       0x04      0x84\n");
    print_str("  q           16      0x10      0x90\n");
    print_str("  w           17      0x11      0x91\n");
    print_str("  e           18      0x12      0x92\n");
    print_str("  a           30      0x1e      0x9e\n");
    print_str("  s           31      0x1f      0x9f\n");
    print_str("  z           44      0x2c      0xac\n");
    print_str("  x           45      0x2d      0xad\n");
    print_str("  enter       28      0x1c      0x9c\n");
    print_str("  space       57      0x39      0xb9\n");
    print_str("  escape      1       0x01      0x81\n");
    print_str("\n");
    print_str("showkey v4.0: scan code table displayed\n");
}

static void show_keycodes(void)
{
    print_str("Keycode display (press any key):\n");
    print_str("  keycode  1  press   Escape\n");
    print_str("  keycode  1  release\n");
    print_str("  keycode  2  press   1\n");
    print_str("  keycode  2  release\n");
    print_str("  keycode  16 press   q\n");
    print_str("  keycode  16 release\n");
    print_str("  keycode  30 press   a\n");
    print_str("  keycode  30 release\n");
    print_str("  keycode  57 press   space\n");
    print_str("  keycode  57 release\n");
    print_str("  keycode  28 press   Return\n");
    print_str("  keycode  28 release\n");
    print_str("\n");
    print_str("showkey v4.0: key simulation displayed\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int show_ascii = 0;
    int show_scancode = 0;
    int show_keycode = 0;
    int display_lines = 0;

    /* Parse arguments: skip argv[0] (module name) */
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            show_help();
            host_exit(0);
        }
        if (my_strcmp(arg, "-a") == 0) {
            show_ascii = 1;
        }
        if (my_strcmp(arg, "-s") == 0) {
            show_scancode = 1;
        }
        if (my_strcmp(arg, "-k") == 0) {
            show_keycode = 1;
        }
        if (pos < 511 && arg[0] == '-' && arg[1] == '-' && arg[2] == 'l') {
            char *eq = &arg[8];
            if (*eq == '=') eq++;
            display_lines = parse_int(eq);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (display_lines > 0) {
        char msg[64];
        int i = 0;
        const char prefix[] = "showkey_v4: display lines set to ";
        while (prefix[i]) { msg[i] = prefix[i]; i++; }
        int n = display_lines;
        char tmp[16];
        int ti = 0;
        if (n == 0) { tmp[0] = '0'; ti = 1; }
        else { while (n > 0) { tmp[ti++] = '0' + (n % 10); n /= 10; } }
        int tj = 0;
        while (tj < ti) { msg[i++] = tmp[ti - 1 - tj]; tj++; }
        msg[i++] = '\n';
        msg[i] = '\0';
        unsigned int pbuf = alloc(i + 1);
        copy_to_mem(msg, pbuf, i + 1);
        host_print(pbuf, i);
    }

    print_str("showkey_v4: press any key to see keycodes (simulated)\n");
    print_str("\n");

    if (show_ascii) {
        show_ascii_table();
    } else if (show_scancode) {
        show_scan_codes();
    } else if (show_keycode) {
        show_keycodes();
    } else {
        show_keycodes();
        show_ascii_table();
        show_scan_codes();
    }

    host_exit(0);
}
