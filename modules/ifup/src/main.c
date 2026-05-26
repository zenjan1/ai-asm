/* ifup: network interface activation simulation (simulated) */
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
        print_str("Usage: ifup <interface>\n");
        print_str("Activate network interface.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    if (argv[i] == '-') {
        print_str("Usage: ifup <interface>\n");
        print_str("Activate network interface.\n");
        host_exit(0);
    }
    int name_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    int name_len = i - name_start;
    char name[32];
    unsigned int nl = name_len < 31 ? name_len : 31;
    for (unsigned int j = 0; j < nl; j++) name[j] = argv[name_start + j];
    name[nl] = '\0';
    print_str("Bringing up interface ");
    print_str(name);
    print_str("...\n");
    print_str("Determining IP information for ");
    print_str(name);
    print_str("...\n");
    print_str("done.\n");
    print_str("Interface ");
    print_str(name);
    print_str(" is now UP.\n");
    host_exit(0);
}
