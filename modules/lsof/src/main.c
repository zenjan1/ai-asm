/* lsof: list open files (simulated) */
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
        print_str("usage: lsof [OPTION]...\n");
        print_str("List open files.\n");
        print_str("\nOptions:\n");
        print_str("  -h    display this help and exit\n");
        host_exit(0);
    }

    print_str("COMMAND   PID  USER   FD   TYPE  DEVICE  SIZE/OFF  NAME\n");
    print_str("init        1  root  cwd   DIR   253,0     4096    /\n");
    print_str("init        1  root  rtd   DIR   253,0     4096    /\n");
    print_str("init        1  root  txt   REG   253,0    12345    /sbin/init\n");
    print_str("init        1  root  mem   REG   253,0    67890    /lib/libc.so\n");
    print_str("shell      42  root  cwd   DIR   253,0     4096    /home/user\n");
    print_str("shell      42  root  txt   REG   253,0    54321    /bin/shell\n");
    print_str("shell      42  root    0u  CHR     4,0        0t0   /dev/tty0\n");
    print_str("shell      42  root    1u  CHR     4,0        0t0   /dev/tty0\n");
    print_str("shell      42  root    2u  CHR     4,0        0t0   /dev/tty0\n");
    host_exit(0);
}
