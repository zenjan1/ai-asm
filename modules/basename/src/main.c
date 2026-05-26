/*
 * aiasm-aarch64/modules/basename/src/main.c
 * basename WASM module for AI-ASM AArch64 v44.0
 *
 * Usage: basename PATH [SUFFIX]
 * Strips directory prefix and optional suffix from PATH.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void host_yield(void);

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
    unsigned int argv_off = alloc(512);
    int argv_len = host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip argv[0] ("basename") */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse PATH */
    const char *path = &argv[i];
    int path_len = 0;
    while (i < argv_len && argv[i] != ' ') { i++; path_len++; }

    if (path_len == 0) {
        host_exit(1);
    }

    /* Find last '/' */
    int name_start = 0;
    for (int j = 0; j < path_len; j++) {
        if (path[j] == '/') name_start = j + 1;
    }

    const char *name = &path[name_start];
    int name_len = path_len - name_start;

    /* Handle trailing slash: basename "/a/" → "" */
    if (name_len == 0) {
        /* Output "/" if path ends with slash and has content before */
        if (name_start > 0) {
            name = "/";
            name_len = 1;
        }
    }

    /* Skip spaces after path */
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse optional SUFFIX */
    if (i < argv_len) {
        const char *suffix = &argv[i];
        int suffix_len = 0;
        while (i < argv_len && argv[i] != ' ') { i++; suffix_len++; }

        /* Remove suffix if name ends with it */
        if (suffix_len > 0 && name_len > suffix_len) {
            int match = 1;
            for (int k = 0; k < suffix_len; k++) {
                if (name[name_len - suffix_len + k] != suffix[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                name_len -= suffix_len;
            }
        }
    }

    /* Output */
    unsigned int off = alloc((unsigned int)name_len);
    copy_to_mem(off, name, (unsigned int)name_len);
    host_print(off, (unsigned int)name_len);

    /* Newline */
    unsigned int nl_off = alloc(1);
    char *nl = (char *)nl_off;
    nl[0] = '\n';
    host_print(nl_off, 1);

    host_yield();
    host_exit(0);
}
