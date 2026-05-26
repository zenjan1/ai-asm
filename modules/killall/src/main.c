/* killall: process termination by name (simulated) */
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
    if (i >= argv_len) {
        print_str("Usage: killall <process_name>\n");
        print_str("Kill processes by name.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    int name_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    int name_len = i - name_start;
    if (name_len == 1 && argv[name_start] == '-') {
        print_str("Usage: killall <process_name>\n");
        print_str("Kill processes by name.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(0);
    }
    char name[64];
    unsigned int nl = name_len < 63 ? name_len : 63;
    for (unsigned int j = 0; j < nl; j++) name[j] = argv[name_start + j];
    name[nl] = '\0';
    print_str("Killing ");
    print_str(name);
    print_str("(pid 1234): killed\n");
    print_str(name);
    print_str("(pid 5678): killed\n");
    print_str(name);
    print_str("(pid 9012): killed\n");
    host_exit(0);
}
