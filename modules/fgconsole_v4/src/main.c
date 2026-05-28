/* fgconsole_v4: foreground console query simulation (enhanced v4) */

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
    print_str("fgconsole_v4 - display foreground virtual terminal (v4.0)\n");
    print_str("Usage: fgconsole_v4 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -s             Show VT status summary\n");
    print_str("  -l             List all VT states\n");
    print_str("\n");
    print_str("Display the current foreground virtual terminal number.\n");
}

static void show_status(void)
{
    print_str("fgconsole_v4: foreground console information\n");
    print_str("  Current VT: 1\n");
    print_str("  Active VT: 1\n");
    print_str("  Console mode: text\n");
    print_str("  Graphics mode: disabled\n");
    print_str("  Cursor: visible\n");
    print_str("  Keyboard: Latin1 layout\n");
    print_str("  Resolution: 80x25\n");
    print_str("\n");
    print_str("fgconsole_v4: status displayed\n");
}

static void list_vt_states(void)
{
    print_str("Virtual terminal states:\n");
    print_str("  VT  1  - active (foreground)\n");
    print_str("  VT  2  - X11 display :0\n");
    print_str("  VT  3  - available\n");
    print_str("  VT  4  - available\n");
    print_str("  VT  5  - available\n");
    print_str("  VT  6  - getty login\n");
    print_str("  VT  7  - available\n");
    print_str("  VT  8  - available\n");
    print_str("  VT  9  - available\n");
    print_str("  VT 10  - available\n");
    print_str("  VT 11  - available\n");
    print_str("  VT 12  - available\n");
    print_str("\n");
    print_str("fgconsole_v4: all VT states listed\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int status_flag = 0;
    int list_flag = 0;

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
        if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        }
        if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (list_flag) {
        list_vt_states();
    } else if (status_flag) {
        show_status();
    } else {
        /* Default: just print the foreground VT number */
        print_str("1\n");
        print_str("fgconsole_v4: foreground VT is 1\n");
    }

    host_exit(0);
}
