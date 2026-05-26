/* brctl: show/configure bridge parameters (simulated) */
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

    /* Skip program name, find first argument */
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("Usage: brctl <command> [arguments]\n");
        print_str("Ethernet bridge configuration utility.\n\n");
        print_str("Commands:\n");
        print_str("  showmacs <brname>    show MAC addresses\n");
        print_str("  addbr <brname>       add a bridge\n");
        print_str("  delbr <brname>       delete a bridge\n");
        print_str("  addif <brname> <dev> add interface to bridge\n");
        print_str("  delif <brname> <dev> delete interface from bridge\n");
        print_str("  stp <brname> <state> set STP state (on/off)\n");
        print_str("  show                 show all bridges\n");
        host_exit(1);
    }

    /* Parse command */
    int cmd_start = i;
    int cmd_len = 0;
    while (i < argv_len && argv[i] != ' ') { i++; cmd_len++; }

    /* Skip space after command */
    while (i < argv_len && argv[i] == ' ') i++;

    if (cmd_len == 4 && argv[cmd_start] == 's' && argv[cmd_start+1] == 'h' &&
        argv[cmd_start+2] == 'o' && argv[cmd_start+3] == 'w') {
        print_str("bridge name     bridge id               STP enabled     interfaces\n");
        print_str("br0             8000.000c29a1b2c3       yes             eth0\n");
        print_str("                                                        eth1\n");
        print_str("docker0         8000.0242f8a3d4e5       no              veth12345\n");
        host_exit(0);
    }

    if (cmd_len == 7 && argv[cmd_start] == 's' && argv[cmd_start+1] == 'h') {
        print_str(" port #  interface  MAC address             TTL  flags\n");
        print_str("  1      eth0       00:0c:29:a1:b2:c3       300  <learn/forward>\n");
        print_str("  2      eth1       00:0c:29:d4:e5:f6       250  <learn/forward>\n");
        host_exit(0);
    }

    if (cmd_len == 3 && argv[cmd_start] == 's' && argv[cmd_start+1] == 't' &&
        argv[cmd_start+2] == 'p') {
        print_str("STP state changed successfully.\n");
        host_exit(0);
    }

    if (cmd_len == 5 && argv[cmd_start] == 'a' && argv[cmd_start+1] == 'd' &&
        argv[cmd_start+2] == 'd' && argv[cmd_start+3] == 'i' && argv[cmd_start+4] == 'f') {
        print_str("Interface added to bridge.\n");
        host_exit(0);
    }

    if (cmd_len == 5 && argv[cmd_start] == 'a' && argv[cmd_start+1] == 'd' &&
        argv[cmd_start+2] == 'd' && argv[cmd_start+3] == 'b' && argv[cmd_start+4] == 'r') {
        print_str("Bridge created successfully.\n");
        host_exit(0);
    }

    if (cmd_len == 5 && argv[cmd_start] == 'd' && argv[cmd_start+1] == 'e' &&
        argv[cmd_start+2] == 'l' && argv[cmd_start+3] == 'b' && argv[cmd_start+4] == 'r') {
        print_str("Bridge deleted successfully.\n");
        host_exit(0);
    }

    print_str("Unknown command. Use 'brctl' for usage.\n");
    host_exit(1);
}
