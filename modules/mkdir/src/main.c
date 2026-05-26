/* mkdir: create directory */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_create")))
extern int host_fs_create(unsigned int name_off, unsigned int name_len);

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
        const char *msg = "usage: mkdir <name>\n";
        unsigned int len = 18;
        unsigned int off = alloc(len);
        copy_to_mem(off, msg, len);
        host_print(off, len);
        host_exit(1);
    }

    const char *name = &argv[i];
    unsigned int name_len = 0;
    while (i + name_len < (unsigned int)argv_len && name[name_len] && name[name_len] != '\n')
        name_len++;

    int rc = host_fs_create((unsigned int)(name - argv), name_len);

    char buf[64];
    unsigned int pos = 0;
    if (rc >= 0) {
        const char *ok = "Directory created: ";
        while (*ok) buf[pos++] = *ok++;
    } else {
        const char *err = "Failed to create directory\n";
        while (*err) buf[pos++] = *err++;
        unsigned int off = alloc(pos);
        copy_to_mem(off, buf, pos);
        host_print(off, pos);
        host_exit(1);
    }

    for (unsigned int j = 0; j < name_len; j++)
        buf[pos++] = name[j];
    buf[pos++] = '\n';

    unsigned int off = alloc(pos);
    copy_to_mem(off, buf, pos);
    host_print(off, pos);
    host_exit(0);
}
