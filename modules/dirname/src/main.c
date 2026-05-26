/*
 * aiasm-aarch64/modules/dirname/src/main.c
 * dirname WASM module for AI-ASM AArch64 v45.0
 *
 * Usage: dirname PATH
 * Strips the filename portion of PATH, outputs directory.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

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

    /* Skip argv[0] ("dirname") */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse PATH */
    const char *path = &argv[i];
    int path_len = 0;
    while (i < argv_len && argv[i] != ' ') { i++; path_len++; }

    if (path_len == 0) {
        unsigned int dot_off = alloc(2);
        char *p = (char *)dot_off;
        p[0] = '.'; p[1] = '\n';
        host_print(dot_off, 2);
        host_exit(0);
    }

    /* Find last '/' */
    int last_slash = -1;
    for (int j = 0; j < path_len; j++) {
        if (path[j] == '/') last_slash = j;
    }

    const char *dir;
    int dir_len;

    if (last_slash < 0) {
        /* No slash: dirname "foo" → "." */
        dir = ".";
        dir_len = 1;
    } else if (last_slash == 0) {
        /* Slash at start: dirname "/foo" → "/" */
        dir = "/";
        dir_len = 1;
    } else {
        /* Strip trailing slashes */
        int end = last_slash;
        while (end > 0 && path[end - 1] == '/') end--;
        dir = path;
        dir_len = end;
        if (dir_len == 0) {
            dir = "/";
            dir_len = 1;
        }
    }

    /* Output */
    unsigned int off = alloc((unsigned int)dir_len);
    copy_to_mem(off, dir, (unsigned int)dir_len);
    host_print(off, (unsigned int)dir_len);

    unsigned int nl_off = alloc(1);
    char *nl = (char *)nl_off;
    nl[0] = '\n';
    host_print(nl_off, 1);

    host_exit(0);
}
