/* openvt_v4: open virtual terminal simulation (enhanced v4) */

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
    print_str("openvt_v4 - open a new virtual terminal (v4.0)\n");
    print_str("Usage: openvt_v4 [OPTIONS] [-- command [args...]]\n");
    print_str("  -c N           Use VT number N (default: first available)\n");
    print_str("  -s             Switch to the new VT\n");
    print_str("  -h, --help     Show this help\n");
    print_str("\n");
    print_str("Open a new virtual terminal and optionally run a command.\n");
}

static void list_available_vts(void)
{
    print_str("Available virtual terminals for opening:\n");
    print_str("  VT  3  - available\n");
    print_str("  VT  4  - available\n");
    print_str("  VT  5  - available\n");
    print_str("  VT  7  - available\n");
    print_str("  VT  8  - available\n");
    print_str("  VT  9  - available\n");
    print_str("  VT 10  - available\n");
    print_str("  VT 11  - available\n");
    print_str("  VT 12  - available\n");
    print_str("\n");
    print_str("openvt_v4: 9 virtual terminals available\n");
}

static void open_vt(int n, int do_switch)
{
    if (n < 1 || n > 12) {
        print_str("openvt_v4: invalid VT number (must be 1-12)\n");
        host_exit(1);
    }
    print_str("openvt_v4: opening virtual terminal ");
    print_int(n, "", "\n");
    print_str("openvt_v4: VT ");
    print_int(n, "", " initialized (simulated)\n");
    print_str("openvt_v4: tty device allocated\n");
    print_str("openvt_v4: keyboard layout loaded\n");
    print_str("openvt_v4: screen buffer initialized\n");
    if (do_switch) {
        print_str("openvt_v4: switching to VT ");
        print_int(n, "", "\n");
    }
    print_str("openvt_v4: VT ");
    print_int(n, "", " ready for use\n");
}

static void open_vt_with_command(int n, const char *cmd)
{
    print_str("openvt_v4: opening VT ");
    print_int(n, "", " with command: ");
    print_str(cmd);
    print_str("\n");
    print_str("openvt_v4: VT ");
    print_int(n, "", " initialized (simulated)\n");
    print_str("openvt_v4: command launched on VT ");
    print_int(n, "", "\n");
    print_str("openvt_v4: ready\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int vt_number = 0;
    int switch_flag = 0;
    int help_flag = 0;
    char *command = 0;

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
        } else if (my_strcmp(arg, "-c") == 0) {
            /* Next arg is the VT number */
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                vt_number = parse_int(&argv_ptr[pos]);
            }
        } else if (my_strcmp(arg, "-s") == 0) {
            switch_flag = 1;
        } else if (arg[0] != '-') {
            command = arg;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (vt_number == 0) {
        vt_number = 3; /* default: first available */
    }

    if (command) {
        open_vt_with_command(vt_number, command);
    } else if (switch_flag) {
        open_vt(vt_number, 1);
    } else {
        list_available_vts();
        print_str("openvt_v4: use -c N to specify VT, -s to switch\n");
    }

    host_exit(0);
}
