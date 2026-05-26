/* realpath: path resolution (simulated) */
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

static void normalize_path(const char *path, unsigned int plen)
{
    /* Allocate workspace */
    unsigned int ws = alloc(plen + 2);
    copy_to_mem(ws, path, plen);
    ((char *)ws)[plen] = 0;

    /* Handle relative vs absolute */
    char *p = (char *)ws;
    int absolute = 0;
    if (p[0] == '/') { absolute = 1; p++; plen--; }

    /* Split into segments and resolve . and .. */
    char *segments[64];
    int depths[64];
    int seg_count = 0;

    char *start = p;
    while (*p) {
        if (*p == '/') {
            *p = 0;
            unsigned int slen = my_strlen(start);
            if (slen > 0 && !(slen == 1 && start[0] == '.')) {
                if (slen == 2 && start[0] == '.' && start[1] == '.') {
                    /* Go up */
                    if (seg_count > 0) seg_count--;
                } else {
                    segments[seg_count] = start;
                    depths[seg_count] = (int)slen;
                    seg_count++;
                }
            }
            p++;
            start = p;
        } else {
            p++;
        }
    }
    /* Last segment */
    unsigned int slen = my_strlen(start);
    if (slen > 0 && !(slen == 1 && start[0] == '.')) {
        if (slen == 2 && start[0] == '.' && start[1] == '.') {
            if (seg_count > 0) seg_count--;
        } else {
            segments[seg_count] = start;
            depths[seg_count] = (int)slen;
            seg_count++;
        }
    }

    /* Print result */
    if (absolute) print_str("/");
    for (int i = 0; i < seg_count; i++) {
        if (i > 0) print_str("/");
        unsigned int off = alloc(depths[i]);
        copy_to_mem(off, segments[i], depths[i]);
        host_print(off, depths[i]);
    }
    print_str("\n");
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
        print_str("usage: realpath <path>\n");
        host_exit(1);
    }

    const char *path = &argv[i];
    unsigned int plen = 0;
    while (path[plen] && path[plen] != ' ' && path[plen] != '\n')
        plen++;

    normalize_path(path, plen);

    host_exit(0);
}
