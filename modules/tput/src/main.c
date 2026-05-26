/* tput: terminal capability control */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

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

static int str_eq(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || a[i] != b[i]) return 0;
    }
    return 1;
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

    const char *cmd = &argv[i];
    unsigned int cmd_len = 0;
    while (argv[i + cmd_len] && argv[i + cmd_len] != '\n' && argv[i + cmd_len] != ' ')
        cmd_len++;

    const char *result;
    unsigned int result_len;

    if (str_eq(cmd, "cols", cmd_len)) {
        result = "80\n"; result_len = 3;
    } else if (str_eq(cmd, "lines", cmd_len)) {
        result = "24\n"; result_len = 3;
    } else if (str_eq(cmd, "clear", cmd_len)) {
        result = "\033[2J\033[H"; result_len = 8;
    } else {
        result = "\n"; result_len = 1;
    }

    unsigned int off = alloc(result_len);
    copy_to_mem(off, result, result_len);
    host_print(off, result_len);
    host_exit(0);
}
