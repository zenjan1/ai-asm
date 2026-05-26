/* truncate: file truncation (simulated) */
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

static unsigned int parse_size(const char *s, unsigned int len)
{
    unsigned int n = 0;
    unsigned int i = 0;
    while (i < len && s[i] >= '0' && s[i] <= '9') {
        n = n * 10 + (s[i] - '0');
        i++;
    }
    /* Handle suffixes */
    if (i < len) {
        char c = s[i];
        if (c == 'K' || c == 'k') n *= 1024;
        else if (c == 'M' || c == 'm') n *= 1048576;
        else if (c == 'G' || c == 'g') n *= 1073741824u;
    }
    return n;
}

static void print_number(unsigned int n)
{
    if (n == 0) { print_str("0"); return; }
    char buf[12];
    int i = 11;
    buf[11] = 0;
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    unsigned int len = 11 - i;
    unsigned int off = alloc(len);
    copy_to_mem(off, &buf[i], len);
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
        print_str("usage: truncate -s <size> <file>\n");
        host_exit(1);
    }

    /* Skip -s flag */
    const char *p = &argv[i];
    if (p[0] == '-' && p[1] == 's') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Parse size */
    const char *size_start = &argv[i];
    unsigned int size_len = 0;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) {
        size_len++;
        i++;
    }

    unsigned int size = parse_size(size_start, size_len);

    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse filename */
    const char *file_start = &argv[i];
    unsigned int file_len = 0;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) {
        file_len++;
        i++;
    }

    if (file_len == 0) {
        print_str("truncate: missing file operand\n");
        host_exit(1);
    }

    /* Print result */
    print_str("truncate: ");
    unsigned int off = alloc(file_len);
    copy_to_mem(off, file_start, file_len);
    host_print(off, file_len);
    print_str(" -> size=");
    print_number(size);
    print_str(" bytes\n");

    host_exit(0);
}
