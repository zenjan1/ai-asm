/* chvt_v6: virtual terminal switching ultimate (v6) */

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
    print_str("chvt_v6 - change virtual terminal (v6.0)\n");
    print_str("Usage: chvt_v6 [OPTIONS] [N]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -n             Switch to next VT\n");
    print_str("  -p             Switch to previous VT\n");
    print_str("  -f             Force switch\n");
    print_str("  -i             Show VT info\n");
    print_str("  -l             List available VTs\n");
    print_str("  -t <n>         Target VT number\n");
    print_str("  -v             Verbose output\n");
    print_str("  -s             Show status\n");
    print_str("\n");
    print_str("Change the active virtual terminal.\n");
}

static void show_info(void)
{
    print_str("chvt_v6: virtual terminal information:\n");
    print_str("chvt_v6: current VT: 1\n");
    print_str("chvt_v6: total VTs: 12\n");
    print_str("chvt_v6: active VT: 1\n");
    print_str("chvt_v6: console VT: 1\n");
    print_str("chvt_v6: VT mode: graphics\n");
    print_str("chvt_v6: auto VT switch: enabled\n");
    print_str("chvt_v6: info display complete\n");
}

static void show_status(void)
{
    print_str("chvt_v6: VT status:\n");
    print_str("chvt_v6: VT 1: active (graphics)\n");
    print_str("chvt_v6: VT 2: available\n");
    print_str("chvt_v6: VT 3: available\n");
    print_str("chvt_v6: VT 4: available\n");
    print_str("chvt_v6: VT 5: available\n");
    print_str("chvt_v6: VT 6: available\n");
    print_str("chvt_v6: status check complete\n");
}

static void list_vts(int verbose)
{
    print_str("chvt_v6: available virtual terminals:\n");
    print_str("chvt_v6: VT 1  - active (graphics mode)\n");
    print_str("chvt_v6: VT 2  - available\n");
    print_str("chvt_v6: VT 3  - available\n");
    print_str("chvt_v6: VT 4  - available\n");
    print_str("chvt_v6: VT 5  - available\n");
    print_str("chvt_v6: VT 6  - available\n");
    print_str("chvt_v6: VT 7  - available\n");
    print_str("chvt_v6: VT 8  - available\n");
    print_str("chvt_v6: VT 9  - available\n");
    print_str("chvt_v6: VT 10 - available\n");
    print_str("chvt_v6: VT 11 - available\n");
    print_str("chvt_v6: VT 12 - available\n");
    if (verbose) {
        print_str("chvt_v6: total: 12 VTs\n");
        print_str("chvt_v6: current: VT 1\n");
        print_str("chvt_v6: mode: graphics\n");
    }
    print_str("chvt_v6: list complete\n");
}

static void switch_next(int verbose)
{
    print_str("chvt_v6: switching to next VT\n");
    if (verbose) {
        print_str("chvt_v6: current VT: 1\n");
        print_str("chvt_v6: target VT: 2\n");
        print_str("chvt_v6: saving VT state\n");
        print_str("chvt_v6: switching display\n");
        print_str("chvt_v6: restoring VT state\n");
    }
    print_str("chvt_v6: switched to VT 2\n");
}

static void switch_prev(int verbose)
{
    print_str("chvt_v6: switching to previous VT\n");
    if (verbose) {
        print_str("chvt_v6: current VT: 1\n");
        print_str("chvt_v6: target VT: 12\n");
        print_str("chvt_v6: saving VT state\n");
        print_str("chvt_v6: switching display\n");
        print_str("chvt_v6: restoring VT state\n");
    }
    print_str("chvt_v6: switched to VT 12\n");
}

static void switch_target(int n, int verbose, int force)
{
    print_str("chvt_v6: switching to VT ");
    print_int(n);
    print_str("\n");
    if (force) {
        print_str("chvt_v6: force mode: overriding lock\n");
    }
    if (verbose) {
        print_str("chvt_v6: current VT: 1\n");
        print_str("chvt_v6: target VT: ");
        print_int(n);
        print_str("\n");
        print_str("chvt_v6: saving VT state\n");
        print_str("chvt_v6: switching display\n");
        print_str("chvt_v6: restoring VT state\n");
    }
    print_str("chvt_v6: switched to VT ");
    print_int(n);
    print_str("\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, next_flag = 0, prev_flag = 0;
    int force_flag = 0, info_flag = 0, list_flag = 0;
    int target_flag = 0, verbose_flag = 0, status_flag = 0;
    int target_val = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-n") == 0) {
            next_flag = 1;
        } else if (my_strcmp(arg, "-p") == 0) {
            prev_flag = 1;
        } else if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            target_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { target_val = target_val * 10 + (*p - '0'); p++; }
            }
        } else {
            /* positional argument: VT number */
            char *p = arg;
            while (*p >= '0' && *p <= '9') { target_val = target_val * 10 + (*p - '0'); p++; }
            target_flag = 1;
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

    if (list_flag) {
        list_vts(verbose_flag);
        host_exit(0);
    }

    if (next_flag) {
        switch_next(verbose_flag);
        host_exit(0);
    }

    if (prev_flag) {
        switch_prev(verbose_flag);
        host_exit(0);
    }

    if (target_flag) {
        switch_target(target_val, verbose_flag, force_flag);
        host_exit(0);
    }

    print_str("chvt_v6: use -n for next, -p for previous, or specify VT number\n");
    host_exit(0);
}
