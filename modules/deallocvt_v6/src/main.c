/* deallocvt_v6: deallocate virtual terminal (v6) */

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
    print_str("deallocvt_v6 - deallocate virtual terminal (v6.0)\n");
    print_str("Usage: deallocvt_v6 [OPTIONS] [VT_NUMBER]\n");
    print_str("  -a             Deallocate all unused VTs\n");
    print_str("  -f             Force deallocation\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -l             List deallocatable VTs\n");
    print_str("  -s             Show status\n");
    print_str("  -n <num>       Deallocate specific VT number\n");
    print_str("\n");
    print_str("Release virtual terminal resources.\n");
}

static void show_info(void)
{
    print_str("deallocvt_v6: VT deallocation info:\n");
    print_str("deallocvt_v6: total VTs: 12\n");
    print_str("deallocvt_v6: active VTs: 1\n");
    print_str("deallocvt_v6: deallocatable VTs: 2-12\n");
    print_str("deallocvt_v6: protected VTs: 1 (active console)\n");
    print_str("deallocvt_v6: info display complete\n");
}

static void show_status(void)
{
    print_str("deallocvt_v6: VT deallocation status:\n");
    print_str("deallocvt_v6: VT 1  - ACTIVE (protected)\n");
    print_str("deallocvt_v6: VT 2  - available for deallocation\n");
    print_str("deallocvt_v6: VT 3  - available for deallocation\n");
    print_str("deallocvt_v6: VT 4  - available for deallocation\n");
    print_str("deallocvt_v6: VT 5  - available for deallocation\n");
    print_str("deallocvt_v6: VT 6  - available for deallocation\n");
    print_str("deallocvt_v6: VT 7  - available for deallocation\n");
    print_str("deallocvt_v6: VT 8  - available for deallocation\n");
    print_str("deallocvt_v6: VT 9  - available for deallocation\n");
    print_str("deallocvt_v6: VT 10 - available for deallocation\n");
    print_str("deallocvt_v6: VT 11 - available for deallocation\n");
    print_str("deallocvt_v6: VT 12 - available for deallocation\n");
    print_str("deallocvt_v6: status check complete\n");
}

static void list_vts(int verbose)
{
    print_str("deallocvt_v6: deallocatable virtual terminals:\n");
    print_str("deallocvt_v6: VT 2  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 3  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 4  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 5  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 6  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 7  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 8  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 9  - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 10 - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 11 - idle (deallocatable)\n");
    print_str("deallocvt_v6: VT 12 - idle (deallocatable)\n");
    if (verbose) {
        print_str("deallocvt_v6: total deallocatable: 11 VTs\n");
        print_str("deallocvt_v6: memory reclaimable: ~2048KB\n");
    }
    print_str("deallocvt_v6: list complete\n");
}

static void deallocate_all(int verbose, int force)
{
    print_str("deallocvt_v6: deallocating all unused VTs\n");
    if (force) {
        print_str("deallocvt_v6: force mode: overriding active sessions\n");
    }
    if (verbose) {
        print_str("deallocvt_v6: deallocating VT 2\n");
        print_str("deallocvt_v6: deallocating VT 3\n");
        print_str("deallocvt_v6: deallocating VT 4\n");
        print_str("deallocvt_v6: deallocating VT 5\n");
        print_str("deallocvt_v6: deallocating VT 6\n");
        print_str("deallocvt_v6: deallocating VT 7\n");
        print_str("deallocvt_v6: deallocating VT 8\n");
        print_str("deallocvt_v6: deallocating VT 9\n");
        print_str("deallocvt_v6: deallocating VT 10\n");
        print_str("deallocvt_v6: deallocating VT 11\n");
        print_str("deallocvt_v6: deallocating VT 12\n");
        print_str("deallocvt_v6: freeing resources\n");
    }
    print_str("deallocvt_v6: deallocated 11 VTs\n");
}

static void deallocate_target(int n, int verbose, int force)
{
    if (n == 1) {
        print_str("deallocvt_v6: cannot deallocate active console (VT 1)\n");
        if (force) {
            print_str("deallocvt_v6: force mode: still protected\n");
        }
        return;
    }
    print_str("deallocvt_v6: deallocating VT ");
    print_int(n);
    print_str("\n");
    if (force) {
        print_str("deallocvt_v6: force mode: clearing active sessions\n");
    }
    if (verbose) {
        print_str("deallocvt_v6: target VT: ");
        print_int(n);
        print_str("\n");
        print_str("deallocvt_v6: checking VT state\n");
        print_str("deallocvt_v6: freeing framebuffer\n");
        print_str("deallocvt_v6: releasing memory\n");
        print_str("deallocvt_v6: updating VT table\n");
    }
    print_str("deallocvt_v6: VT ");
    print_int(n);
    print_str(" deallocated successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, all_flag = 0, force_flag = 0;
    int info_flag = 0, list_flag = 0, verbose_flag = 0;
    int status_flag = 0, number_flag = 0;
    int number_val = 0;

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
        } else if (my_strcmp(arg, "-n") == 0) {
            number_flag = 1;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) {
                char *p = &argv_ptr[pos];
                while (*p >= '0' && *p <= '9') { number_val = number_val * 10 + (*p - '0'); p++; }
            }
        } else {
            /* positional argument: VT number */
            char *p = arg;
            while (*p >= '0' && *p <= '9') { number_val = number_val * 10 + (*p - '0'); p++; }
            number_flag = 1;
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

    if (all_flag) {
        deallocate_all(verbose_flag, force_flag);
        host_exit(0);
    }

    if (number_flag) {
        deallocate_target(number_val, verbose_flag, force_flag);
        host_exit(0);
    }

    print_str("deallocvt_v6: use -a for all, -n <num> for specific VT, or -l to list\n");
    host_exit(0);
}
