/* chvt_v3: virtual terminal switch simulation (enhanced v3) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;
static unsigned int alloc(unsigned int n) {
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) {
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}
static unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}
static void print_str(const char *s) {
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}
static void print_uint(unsigned int n) {
    char buf[16];
    int i = 15;
    buf[15] = 0;
    if (n == 0) { buf[14] = '0'; i = 14; }
    while (n > 0) { buf[--i] = (n % 10) + '0'; n /= 10; }
    print_str(buf + i);
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    int vt_num = 0;
    int has_arg = 0;
    if (argv_len > 0) {
        const char *argv = (const char *)argv_off;
        int i = 0;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
        if (i < argv_len) {
            if (argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h') {
                print_str("Usage: chvt_v3 N\n");
                print_str("Change foreground virtual terminal to /dev/ttyN.\n");
                print_str("\nArguments:\n");
                print_str("  N   virtual terminal number (1-63)\n");
                print_str("\nOptions:\n");
                print_str("  -h  show this help\n");
                host_exit(1);
            }
            vt_num = 0;
            while (i < argv_len && argv[i] >= '0' && argv[i] <= '9') {
                vt_num = vt_num * 10 + (argv[i] - '0');
                i++;
            }
            has_arg = 1;
        }
    }

    print_str("chvt_v3: virtual terminal switch (enhanced v3)\n\n");
    if (has_arg && vt_num > 0 && vt_num <= 63) {
        print_str("Switching to VT");
        print_uint(vt_num);
        print_str("\n");
        print_str("  Target: /dev/tty");
        print_uint(vt_num);
        print_str("\n");
        print_str("  Previous VT: 1\n");
        print_str("  New VT: ");
        print_uint(vt_num);
        print_str("\n");
        print_str("  Status: active\n");
        print_str("  Mode: graphics\n");
        print_str("  Resolution: 1024x768\n");
        print_str("  Depth: 32 bpp\n");
    } else {
        print_str("Current VT: 1\n");
        print_str("  Active: /dev/tty1\n");
        print_str("  Available: VT1-VT12\n");
        print_str("  Mode: text (80x25)\n");
        print_str("Usage: chvt_v3 N  (switch to VT N)\n");
    }
    print_str("\nStatus: done\n");
    host_exit(0);
}
