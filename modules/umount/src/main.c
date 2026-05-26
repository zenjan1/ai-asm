/* umount: unmount file systems */
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
        const char *msg = "usage: umount <path>\n";
        unsigned int off = alloc(20);
        copy_to_mem(off, msg, 20);
        host_print(off, 20);
        host_exit(1);
    }

    const char *path = &argv[i];
    unsigned int path_len = 0;
    while (path[path_len] && path[path_len] != ' ' && path[path_len] != '\n')
        path_len++;

    char msg[256];
    unsigned int pos = 0;
    const char *prefix = "Unmounted: ";
    while (*prefix) msg[pos++] = *prefix++;
    for (unsigned int j = 0; j < path_len; j++) msg[pos++] = path[j];
    msg[pos++] = '\n';

    unsigned int off = alloc(pos);
    copy_to_mem(off, msg, pos);
    host_print(off, pos);
    host_exit(0);
}
