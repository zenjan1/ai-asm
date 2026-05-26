/* stdbuf: standard I/O buffer control (simulated) */
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

static const char *buffer_mode(const char *opt)
{
    if (opt[0] == '0' && opt[1] == 0) return "unbuffered";
    if (opt[0] == 'L' && opt[1] == 0) return "line-buffered";
    if (opt[0] == 'B' && opt[1] == 0) return "fully-buffered";
    if (opt[0] >= '0' && opt[0] <= '9') return "block-buffered";
    return "unknown";
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

    if (i >= argv_len) {
        print_str("usage: stdbuf <option> <command>\n");
        print_str("  options: 0 (none), L (line), B (block), <size>\n");
        host_exit(1);
    }

    const char *opt_start = &argv[i];
    unsigned int opt_len = 0;
    while (opt_start[opt_len] && opt_start[opt_len] != ' ' && opt_start[opt_len] != '\n')
        opt_len++;

    unsigned int opt_buf = alloc(opt_len + 1);
    copy_to_mem(opt_buf, opt_start, opt_len);
    ((char *)opt_buf)[opt_len] = 0;

    i += opt_len;
    while (i < argv_len && argv[i] == ' ') i++;

    const char *cmd_start = &argv[i];
    unsigned int cmd_len = 0;
    while (cmd_start[cmd_len] && cmd_start[cmd_len] != ' ' && cmd_start[cmd_len] != '\n')
        cmd_len++;

    const char *mode = buffer_mode((const char *)opt_buf);

    print_str("stdbuf: buffer mode set to [");
    print_str(mode);
    print_str("]\n");

    if (cmd_len > 0) {
        print_str("stdbuf: running command: ");
        unsigned int cmd_buf = alloc(cmd_len + 1);
        copy_to_mem(cmd_buf, cmd_start, cmd_len);
        ((char *)cmd_buf)[cmd_len] = 0;
        print_str((const char *)cmd_buf);
        print_str("\n");
    }

    host_exit(0);
}
