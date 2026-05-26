/* pathchk: path validation (simulated) */
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
        print_str("usage: pathchk <path>\n");
        host_exit(1);
    }

    const char *path = &argv[i];
    unsigned int plen = 0;
    while (path[plen] && path[plen] != ' ' && path[plen] != '\n')
        plen++;

    unsigned int tmp = alloc(plen + 1);
    copy_to_mem(tmp, path, plen);
    ((char *)tmp)[plen] = 0;
    path = (const char *)tmp;

    int valid = 1;
    if (plen == 0) { print_str("pathchk: empty path\n"); valid = 0; }
    if (plen > 4096) { print_str("pathchk: path too long (>4096)\n"); valid = 0; }
    for (unsigned int j = 0; j < plen && valid; j++) {
        char c = path[j];
        if (c == '\0') { print_str("pathchk: contains null byte\n"); valid = 0; }
    }

    if (valid) {
        unsigned int off = alloc(plen);
        copy_to_mem(off, path, plen);
        host_print(off, plen);
        print_str(": valid\n");
    }

    host_exit(valid ? 0 : 1);
}
