/* deallocvt_v4: virtual terminal deallocation simulation (enhanced v4) */

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
    print_str("deallocvt_v4 - deallocate virtual terminal (v4.0)\n");
    print_str("Usage: deallocvt_v4 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -a             Deallocate all unused VTs\n");
    print_str("  -f             Force deallocation\n");
    print_str("\n");
    print_str("Deallocate unused virtual terminals (simulated).\n");
}

static void deallocate_vt(int n, int force)
{
    if (n < 1 || n > 12) {
        print_str("deallocvt_v4: invalid VT number (must be 1-12)\n");
        host_exit(1);
    }
    if (n == 1) {
        print_str("deallocvt_v4: cannot deallocate VT 1 (primary console)\n");
        host_exit(1);
    }
    if (n == 2) {
        print_str("deallocvt_v4: cannot deallocate VT 2 (X11 display)\n");
        host_exit(1);
    }
    print_str("deallocvt_v4: deallocating virtual terminal ");
    print_int(n, "", "\n");
    print_str("deallocvt_v4: VT ");
    print_int(n, "", " processes terminated\n");
    print_str("deallocvt_v4: VT ");
    print_int(n, "", " screen buffer freed\n");
    print_str("deallocvt_v4: VT ");
    print_int(n, "", " keyboard layout released\n");
    print_str("deallocvt_v4: VT ");
    print_int(n, "", " deallocated successfully\n");
}

static void deallocate_all(void)
{
    print_str("deallocvt_v4: deallocating all unused virtual terminals\n");
    print_str("deallocvt_v4: VT 3 deallocated\n");
    print_str("deallocvt_v4: VT 4 deallocated\n");
    print_str("deallocvt_v4: VT 5 deallocated\n");
    print_str("deallocvt_v4: VT 7 deallocated\n");
    print_str("deallocvt_v4: VT 8 deallocated\n");
    print_str("deallocvt_v4: VT 9 deallocated\n");
    print_str("deallocvt_v4: VT 10 deallocated\n");
    print_str("deallocvt_v4: VT 11 deallocated\n");
    print_str("deallocvt_v4: VT 12 deallocated\n");
    print_str("deallocvt_v4: 9 virtual terminals deallocated\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int vt_number = 0;
    int all_flag = 0;
    int force_flag = 0;

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
            all_flag = 1;
        }
        if (my_strcmp(arg, "-f") == 0) {
            force_flag = 1;
        }
        if (arg[0] >= '1' && arg[0] <= '9') {
            vt_number = parse_int(arg);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (all_flag) {
        deallocate_all();
    } else if (vt_number > 0) {
        deallocate_vt(vt_number, force_flag);
    } else {
        print_str("deallocvt_v4: no VT specified (use -h for help)\n");
        print_str("deallocvt_v4: use -a to deallocate all unused VTs\n");
    }

    host_exit(0);
}
