/* openvt: open virtual terminal launcher (simulated) */
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

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;
    const char *argv = (const char *)argv_off;

    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len || (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h')) {
        print_str("Usage: openvt [ -c N ] [ -s ] [ -w ] command [ args... ]\n");
        print_str("Start a command on a new virtual terminal.\n");
        print_str("\nOptions:\n");
        print_str("  -c N  use VT number N\n");
        print_str("  -s    switch to the new VT\n");
        print_str("  -w    wait for command to exit\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }

    print_str("openvt: virtual terminal launcher simulation\n\n");
    print_str("Allocating new virtual terminal...\n");
    print_str("  First available VT: 7\n");
    print_str("  VT mode: text (KD_TEXT)\n");
    print_str("  Font: 8x16 VGA\n");
    print_str("  Columns: 80\n");
    print_str("  Rows: 25\n");
    print_str("  Buffer: 16384 bytes\n");
    print_str("\nLaunching command on VT7...\n");
    print_str("  Command: /bin/sh\n");
    print_str("  Args: [\"sh\", \"-l\"]\n");
    print_str("  PID: 1234\n");
    print_str("  Terminal: /dev/tty7\n");
    print_str("\nSwitching to VT7...\n");
    print_str("VT switch completed\n");
    print_str("Status: COMMAND LAUNCHED ON VT7\n");

    host_exit(0);
}
