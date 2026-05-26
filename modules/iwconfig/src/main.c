/* iwconfig: wireless interface configuration simulation (simulated) */
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
    if (i < argv_len && argv[i] == '-') {
        print_str("Usage: iwconfig [interface] [parameters]\n");
        print_str("Configure wireless network interface.\n\n");
        print_str("Options:\n");
        print_str("  interface    wireless interface name (e.g. wlan0)\n");
        print_str("  essid        set network name\n");
        print_str("  channel      set channel\n");
        print_str("  -h           show this help\n");
        host_exit(0);
    }
    print_str("wlan0     IEEE 802.11bgn  ESSID:\"MyNetwork\"  Nickname:\"AI-ASM\"\n");
    print_str("          Mode:Managed  Frequency:2.437 GHz  Access Point: AA:BB:CC:DD:EE:FF\n");
    print_str("          Bit Rate=72.2 Mb/s   Tx-Power=20 dBm\n");
    print_str("          Retry short limit:7   RTS thr:off   Fragment thr:off\n");
    print_str("          Power Management:off\n");
    print_str("          Link Quality=70/70  Signal level=-40 dBm\n");
    print_str("          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0\n");
    print_str("          Tx excessive retries:0  Invalid misc:0   Missed beacon:0\n");
    host_exit(0);
}
