/* deallocvt_v5: VT deallocation ultimate (v5) */

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
    print_str("deallocvt_v5 - deallocate virtual terminal (v5.0)\n");
    print_str("Usage: deallocvt_v5 [OPTIONS] [N]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a             Deallocate all unused VTs\n");
    print_str("  -f             Force deallocation\n");
    print_str("  -n <N>         Deallocate specific VT\n");
    print_str("  -i             Show VT status info\n");
    print_str("\n");
    print_str("Deallocate unused virtual terminals.\n");
}

static void show_info(void)
{
    print_str("deallocvt_v5: virtual terminal status:\n");
    print_str("deallocvt_v5: VT1  - active (Linux console)\n");
    print_str("deallocvt_v5: VT2  - active (Linux console)\n");
    print_str("deallocvt_v5: VT3  - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT4  - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT5  - active (shell)\n");
    print_str("deallocvt_v5: VT6  - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT7  - active (X11)\n");
    print_str("deallocvt_v5: VT8  - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT9  - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT10 - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT11 - idle (can deallocate)\n");
    print_str("deallocvt_v5: VT12 - idle (can deallocate)\n");
    print_str("deallocvt_v5: active VTs: 1, 2, 5, 7\n");
    print_str("deallocvt_v5: idle VTs: 3, 4, 6, 8-12\n");
}

static void deallocate_all(void)
{
    print_str("deallocvt_v5: deallocating all unused VTs\n");
    print_str("deallocvt_v5: deallocating VT3\n");
    print_str("deallocvt_v5: deallocating VT4\n");
    print_str("deallocvt_v5: deallocating VT6\n");
    print_str("deallocvt_v5: deallocating VT8\n");
    print_str("deallocvt_v5: deallocating VT9\n");
    print_str("deallocvt_v5: deallocating VT10\n");
    print_str("deallocvt_v5: deallocating VT11\n");
    print_str("deallocvt_v5: deallocating VT12\n");
    print_str("deallocvt_v5: 8 VTs deallocated\n");
    print_str("deallocvt_v5: remaining active VTs: 1, 2, 5, 7\n");
}

static void deallocate_one(int n, int force)
{
    print_str("deallocvt_v5: deallocating VT");
    print_int(n);
    print_str("\n");
    if (force) {
        print_str("deallocvt_v5: forcing VT");
        print_int(n);
        print_str(" deallocation\n");
    }
    print_str("deallocvt_v5: VT");
    print_int(n);
    print_str(" freed\n");
    print_str("deallocvt_v5: memory released\n");
    print_str("deallocvt_v5: VT");
    print_int(n);
    print_str(" deallocated successfully\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int all_flag = 0;
    int force_flag = 0;
    int number_flag = 0;
    int info_flag = 0;
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

    if (all_flag) {
        deallocate_all();
        host_exit(0);
    }

    if (number_flag && number_val) {
        deallocate_one(number_val, force_flag);
        host_exit(0);
    }

    /* Default */
    print_str("deallocvt_v5: current active VTs: 1, 2, 5, 7\n");
    print_str("deallocvt_v5: use -a to deallocate all unused\n");
    print_str("deallocvt_v5: use -n N to deallocate specific VT\n");

    host_exit(0);
}
