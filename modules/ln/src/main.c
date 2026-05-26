/* ln: create file link */
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
        const char *msg = "usage: ln <target> <linkname>\n";
        unsigned int off = alloc(27);
        copy_to_mem(off, msg, 27);
        host_print(off, 27);
        host_exit(1);
    }

    const char *target = &argv[i];
    unsigned int target_len = 0;
    while (argv[i + target_len] && argv[i + target_len] != ' ') target_len++;
    i += target_len;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i >= argv_len) {
        const char *msg = "usage: ln <target> <linkname>\n";
        unsigned int off = alloc(27);
        copy_to_mem(off, msg, 27);
        host_print(off, 27);
        host_exit(1);
    }

    const char *linkname = &argv[i];
    unsigned int linkname_len = 0;
    while (argv[i + linkname_len] && argv[i + linkname_len] != '\n' && argv[i + linkname_len] != ' ')
        linkname_len++;

    int rc = host_fs_create((unsigned int)(linkname - argv), linkname_len);

    char buf[128];
    unsigned int pos = 0;
    if (rc >= 0) {
        const char *ok = "Link created: ";
        while (*ok) buf[pos++] = *ok++;
        for (unsigned int j = 0; j < linkname_len; j++) buf[pos++] = linkname[j];
        buf[pos++] = ' ';
        const char *to = "-> ";
        while (*to) buf[pos++] = *to++;
        for (unsigned int j = 0; j < target_len; j++) buf[pos++] = target[j];
    } else {
        const char *err = "Failed to create link\n";
        while (*err) buf[pos++] = *err++;
        unsigned int off = alloc(pos);
        copy_to_mem(off, buf, pos);
        host_print(off, pos);
        host_exit(1);
    }
    buf[pos++] = '\n';

    unsigned int off = alloc(pos);
    copy_to_mem(off, buf, pos);
    host_print(off, pos);
    host_exit(0);
}
