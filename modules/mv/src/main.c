/* mv: move/rename file */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

__attribute__((import_module("host"), import_name("fs_create")))
extern int host_fs_create(unsigned int name_off, unsigned int name_len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int host_fs_write(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_delete")))
extern int host_fs_delete(unsigned int name_off, unsigned int name_len);

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
    unsigned int argv_off = alloc(512);
    int argv_len = host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i >= argv_len) {
        const char *msg = "usage: mv <src> <dst>\n";
        unsigned int off = alloc(21);
        copy_to_mem(off, msg, 21);
        host_print(off, 21);
        host_exit(1);
    }

    const char *src = &argv[i];
    unsigned int src_len = 0;
    while (argv[i + src_len] && argv[i + src_len] != ' ') src_len++;
    i += src_len;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i >= argv_len) {
        const char *msg = "usage: mv <src> <dst>\n";
        unsigned int off = alloc(21);
        copy_to_mem(off, msg, 21);
        host_print(off, 21);
        host_exit(1);
    }

    const char *dst = &argv[i];
    unsigned int dst_len = 0;
    while (argv[i + dst_len] && argv[i + dst_len] != '\n' && argv[i + dst_len] != ' ')
        dst_len++;

    int src_fd = host_fs_open((unsigned int)(src - argv), src_len);
    if (src_fd < 0) {
        const char *msg = "mv: cannot open source\n";
        unsigned int off = alloc(23);
        copy_to_mem(off, msg, 23);
        host_print(off, 23);
        host_exit(1);
    }

    int dst_fd = host_fs_create((unsigned int)(dst - argv), dst_len);
    if (dst_fd < 0) {
        host_fs_close(src_fd);
        const char *msg = "mv: cannot create destination\n";
        unsigned int off = alloc(30);
        copy_to_mem(off, msg, 30);
        host_print(off, 30);
        host_exit(1);
    }

    unsigned int buf = alloc(4096);
    int r;
    while ((r = host_fs_read(src_fd, buf, 4096)) > 0)
        host_fs_write(dst_fd, buf, (unsigned int)r);

    host_fs_close(src_fd);
    host_fs_close(dst_fd);

    int rc = host_fs_delete((unsigned int)(src - argv), src_len);

    char msg[128];
    unsigned int pos = 0;
    const char *ok = "Moved: ";
    while (*ok) msg[pos++] = *ok++;
    for (unsigned int j = 0; j < src_len; j++) msg[pos++] = src[j];
    msg[pos++] = ' ';
    const char *to = "-> ";
    while (*to) msg[pos++] = *to++;
    for (unsigned int j = 0; j < dst_len; j++) msg[pos++] = dst[j];
    msg[pos++] = '\n';

    unsigned int off = alloc(pos);
    copy_to_mem(off, msg, pos);
    host_print(off, pos);
    host_exit(0);
}
