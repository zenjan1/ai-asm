/* openvt_v6: open virtual terminal (v6) */

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
    print_str("openvt_v6 - open virtual terminal (v6.0)\n");
    print_str("Usage: openvt_v6 [OPTIONS] [COMMAND]\n");
    print_str("  -c <vt>    Specify VT number\n");
    print_str("  -s         Switch to the new VT\n");
    print_str("  -v         Verbose output\n");
    print_str("  -u <user>  Run as user\n");
    print_str("  -w         Wait for command to complete\n");
    print_str("  -f         Force open\n");
    print_str("  -l         Start as login shell\n");
    print_str("  -i         Show info\n");
    print_str("\n");
    print_str("Start a command on a new virtual terminal.\n");
}

static void show_info(void)
{
    print_str("openvt_v6: virtual terminal info:\n");
    print_str("openvt_v6: available VTs: 1-12\n");
    print_str("openvt_v6: current VT: 1\n");
    print_str("openvt_v6: active VT: 1\n");
    print_str("openvt_v6: console VT: 1\n");
    print_str("openvt_v6: VT mode: graphics\n");
    print_str("openvt_v6: info display complete\n");
}

static void open_vt(int vt, int switch_to, int verbose, int force, int login, int wait, const char *user, const char *cmd)
{
    print_str("openvt_v6: opening VT ");
    print_int(vt);
    print_str("\n");
    if (force) {
        print_str("openvt_v6: force mode: overriding lock\n");
    }
    if (verbose) {
        print_str("openvt_v6: target VT: ");
        print_int(vt);
        print_str("\n");
        if (user) {
            print_str("openvt_v6: user: ");
            unsigned int len = my_strlen(user);
            unsigned int buf = alloc(len + 1);
            copy_to_mem(user, buf, len + 1);
            host_print(buf, len);
            print_str("\n");
        }
        if (cmd) {
            print_str("openvt_v6: command: ");
            unsigned int len = my_strlen(cmd);
            unsigned int buf = alloc(len + 1);
            copy_to_mem(cmd, buf, len + 1);
            host_print(buf, len);
            print_str("\n");
        }
        print_str("openvt_v6: allocating VT resources\n");
        print_str("openvt_v6: initializing console\n");
        print_str("openvt_v6: setting up framebuffer\n");
        if (login) {
            print_str("openvt_v6: login shell enabled\n");
        }
        if (switch_to) {
            print_str("openvt_v6: switching to VT ");
            print_int(vt);
            print_str("\n");
        }
        if (wait) {
            print_str("openvt_v6: waiting for command completion\n");
        }
    }
    if (cmd) {
        print_str("openvt_v6: executing: ");
        unsigned int len = my_strlen(cmd);
        unsigned int buf = alloc(len + 1);
        copy_to_mem(cmd, buf, len + 1);
        host_print(buf, len);
        print_str("\n");
    }
    if (switch_to) {
        print_str("openvt_v6: switched to VT ");
        print_int(vt);
        print_str("\n");
    }
    print_str("openvt_v6: VT ");
    print_int(vt);
    print_str(" opened successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int switch_flag = 0, verbose_flag = 0;
    int force_flag = 0, login_flag = 0, wait_flag = 0;
    int vt_specified = 0, vt_val = 1;
    const char *user_val = 0;
    const char *cmd_val = 0;

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
        } else if (my_strcmp(arg, "-s") == 0) {
            switch_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            login_flag = 1;
        } else if (my_strcmp(arg, "-w") == 0) {
            wait_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            vt_specified = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { vt_val = vt_val * 10 + (*p - '0'); p++; }
            }
        } else if (my_strcmp(arg, "-u") == 0) {
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                user_val = &argv_ptr[pos];
            }
        } else {
            /* positional argument: command */
            cmd_val = arg;
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

    if (!vt_specified) {
        vt_val = 2;  /* default to VT 2 */
    }

    open_vt(vt_val, switch_flag, verbose_flag, force_flag, login_flag, wait_flag, user_val, cmd_val);
    host_exit(0);
}
