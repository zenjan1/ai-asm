/* find: search for files in a directory hierarchy */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_list")))
extern int host_fs_list(unsigned int buf_off, unsigned int max_len);

#define MAX_LIST_BUF 4096

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

static void print_char(char c)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = c;
    host_print(off, 1);
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

    const char *path = ".";
    unsigned int path_len = 1;
    if (i < argv_len) {
        path = &argv[i];
        path_len = 0;
        while (argv[i + path_len] && argv[i + path_len] != ' ') path_len++;
    }

    unsigned int list_off = alloc(MAX_LIST_BUF);
    int total = host_fs_list(list_off, MAX_LIST_BUF - 1);
    if (total <= 0) {
        host_exit(0);
        return;
    }

    const char *data = (const char *)list_off;
    int pos = 0;
    while (pos < total) {
        const char *name = &data[pos];
        unsigned int len = my_strlen(name);
        if (len > 0) {
            unsigned int off = alloc(path_len);
            copy_to_mem(off, path, path_len);
            host_print(off, path_len);
            print_char('/');
            off = alloc(len);
            copy_to_mem(off, name, len);
            host_print(off, len);
            print_char('\n');
        }
        pos += len + 1;
        if (pos >= total) break;
    }

    host_exit(0);
}
