/* openvt_v5: VT creation ultimate (v5) */

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
    print_str("openvt_v5 - open virtual terminal (v5.0)\n");
    print_str("Usage: openvt_v5 [OPTIONS] [-- COMMAND]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -n <N>         Open specific VT number\n");
    print_str("  -c <cmd>       Run command on new VT\n");
    print_str("  -s             Switch to new VT\n");
    print_str("  -l             List available VTs\n");
    print_str("  -w             Wait for command to finish\n");
    print_str("\n");
    print_str("Open a new virtual terminal.\n");
}

static void list_vts(void)
{
    print_str("openvt_v5: available virtual terminals:\n");
    print_str("openvt_v5: VT1  - in use (Linux console)\n");
    print_str("openvt_v5: VT2  - in use (Linux console)\n");
    print_str("openvt_v5: VT3  - available\n");
    print_str("openvt_v5: VT4  - available\n");
    print_str("openvt_v5: VT5  - available\n");
    print_str("openvt_v5: VT6  - available\n");
    print_str("openvt_v5: VT7  - in use (X11)\n");
    print_str("openvt_v5: VT8  - available\n");
    print_str("openvt_v5: VT9  - available\n");
    print_str("openvt_v5: VT10 - available\n");
    print_str("openvt_v5: VT11 - available\n");
    print_str("openvt_v5: VT12 - available\n");
    print_str("openvt_v5: free VTs: 3-6, 8-12\n");
}

static void open_vt(int n, const char *cmd, int switch_flag, int wait_flag)
{
    print_str("openvt_v5: opening VT");
    print_int(n);
    print_str("\n");
    print_str("openvt_v5: allocating VT");
    print_int(n);
    print_str("\n");
    if (cmd) {
        print_str("openvt_v5: executing: ");
        print_str(cmd);
        print_str("\n");
    }
    if (switch_flag) {
        print_str("openvt_v5: switching to VT");
        print_int(n);
        print_str("\n");
    }
    if (wait_flag) {
        print_str("openvt_v5: waiting for command\n");
    }
    print_str("openvt_v5: VT");
    print_int(n);
    print_str(" opened successfully\n");
    print_str("openvt_v5: session started\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int number_flag = 0;
    int cmd_flag = 0;
    int switch_flag = 0;
    int list_flag = 0;
    int wait_flag = 0;
    int number_val = 0;
    char *cmd_val = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            switch_flag = 1;
        } else if (my_strcmp(arg, "-w") == 0) {
            wait_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            number_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                number_val = 0;
                while (*p >= '0' && *p <= '9') {
                    number_val = number_val * 10 + (*p - '0');
                    p++;
                }
            }
        } else if (my_strcmp(arg, "-c") == 0) {
            cmd_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) cmd_val = &argv_ptr[pos];
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (list_flag) {
        list_vts();
        host_exit(0);
    }

    if (!number_val) number_val = 3; /* default VT */

    open_vt(number_val, cmd_val, switch_flag, wait_flag);
    host_exit(0);
}
