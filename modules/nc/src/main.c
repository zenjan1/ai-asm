/* nc: netcat simulation (simulated) */
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
        print_str("Usage: nc [-options] [hostname] [port(s)]\n");
        print_str("netcat - arbitrary TCP and UDP connections and listens.\n\n");
        print_str("Options:\n");
        print_str("  -l    listen mode, for inbound connects\n");
        print_str("  -p    local port number\n");
        print_str("  -u    UDP mode\n");
        print_str("  -z    zero-I/O mode (scanning)\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    if (argv[i] == '-') {
        print_str("Usage: nc [-options] [hostname] [port(s)]\n");
        print_str("netcat - arbitrary TCP and UDP connections and listens.\n");
        host_exit(0);
    }
    int name_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    int name_len = i - name_start;
    char host[64];
    unsigned int hl = name_len < 63 ? name_len : 63;
    for (unsigned int j = 0; j < hl; j++) host[j] = argv[name_start + j];
    host[hl] = '\0';
    i++;
    while (i < argv_len && argv[i] == ' ') i++;
    int port_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    char port[16];
    unsigned int pl = (i - port_start) < 15 ? (i - port_start) : 15;
    for (unsigned int j = 0; j < pl; j++) port[j] = argv[port_start + j];
    port[pl] = '\0';
    print_str("Connection to ");
    print_str(host);
    print_str(" ");
    print_str(port);
    print_str(" port [tcp/*] succeeded!\n");
    print_str("HTTP/1.1 200 OK\n");
    print_str("Content-Type: text/html\n");
    print_str("Content-Length: 13\n\n");
    print_str("Hello, World!\n");
    host_exit(0);
}
