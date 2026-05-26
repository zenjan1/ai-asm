/* kill: send signal to process */
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

static int parse_int(const char *s)
{
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n;
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    /* Skip "kill" argument, find PID */
    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        const char *msg = "usage: kill <pid>\n";
        unsigned int len = 17;
        unsigned int off = alloc(len);
        copy_to_mem(off, msg, len);
        host_print(off, len);
        host_exit(1);
    }

    int pid = parse_int(&argv[i]);

    char buf[64];
    unsigned int pos = 0;
    const char *prefix = "Signal sent to process ";
    while (*prefix) buf[pos++] = *prefix++;

    /* Append pid */
    char tmp[16];
    int ti = 0;
    int n = pid;
    if (n == 0) { buf[pos++] = '0'; }
    else {
        while (n > 0) { tmp[ti++] = '0' + (n % 10); n /= 10; }
        while (ti > 0) buf[pos++] = tmp[--ti];
    }
    buf[pos++] = '\n';

    unsigned int off = alloc(pos);
    copy_to_mem(off, buf, pos);
    host_print(off, pos);
    host_exit(0);
}
