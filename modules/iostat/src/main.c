/* iostat: I/O statistics display (simulated) */
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

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h') {
        print_str("usage: iostat [OPTION]...\n");
        print_str("Report I/O device statistics.\n");
        print_str("\nOptions:\n");
        print_str("  -h    display this help and exit\n");
        host_exit(0);
    }

    print_str("avg-cpu:  %user   %nice %system %iowait  %idle\n");
    print_str("           12.5     0.0     3.2    1.8   82.5\n\n");
    print_str("Device            tps    kB_read/s    kB_wrtn/s    kB_read    kB_wrtn\n");
    print_str("vda              45.2       1234.5        567.8    1234567     567890\n");
    print_str("vdb              12.8        456.7        234.1     456789     234123\n");
    print_str("ram0             89.1        789.0         12.3     789012      12345\n");
    host_exit(0);
}
