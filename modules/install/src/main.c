/* install: file installation (simulated) */
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

static void print_arg(const char *argv, int start, int max_len)
{
    int i = start;
    while (i < max_len && argv[i] == ' ') i++;
    int arg_start = i;
    while (i < max_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) i++;
    unsigned int len = i - arg_start;
    if (len > 0) {
        unsigned int off = alloc(len);
        copy_to_mem(off, &argv[arg_start], len);
        host_print(off, len);
    }
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
        print_str("usage: install <source> <dest>\n");
        host_exit(1);
    }

    /* Get source path */
    int src_start = i;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("install: missing destination operand\n");
        host_exit(1);
    }

    /* Get dest path */
    int dst_start = i;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) i++;

    /* Print source */
    print_str("install: ");
    print_arg(argv, src_start, argv_len);
    print_str(" -> ");
    print_arg(argv, dst_start, argv_len);
    print_str(" [mode=0755]\n");

    host_exit(0);
}
