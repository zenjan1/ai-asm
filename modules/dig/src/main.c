/* dig: DNS lookup simulation (simulated) */
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
        print_str("Usage: dig [@server] [domain] [type]\n");
        print_str("DNS lookup simulation.\n\n");
        print_str("Options:\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    if (argv[i] == '-') {
        print_str("Usage: dig [@server] [domain] [type]\n");
        print_str("DNS lookup simulation.\n");
        host_exit(0);
    }
    print_str("; <<>> DiG 9.18.0 <<>> example.com\n");
    print_str(";; global options: +cmd\n");
    print_str(";; Got answer:\n");
    print_str(";; ->>HEADER<<- opcode: QUERY, status: NOERROR\n");
    print_str(";; QUESTION SECTION:\n");
    print_str(";example.com.            IN  A\n\n");
    print_str(";; ANSWER SECTION:\n");
    print_str("example.com.     3600    IN  A   93.184.216.34\n\n");
    print_str(";; Query time: 12 msec\n");
    print_str(";; SERVER: 8.8.8.8#53(8.8.8.8)\n");
    print_str(";; WHEN: Thu May 29 04:30:00 UTC 2026\n");
    print_str(";; MSG SIZE  rcvd: 56\n");
    host_exit(0);
}
