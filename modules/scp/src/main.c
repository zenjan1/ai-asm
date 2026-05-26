/* scp: secure copy between hosts */
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

static void print_char(char c)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = c;
    host_print(off, 1);
}

static void my_itoa(unsigned int n, char *buf)
{
    char tmp[16];
    int i = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
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
        print_str("usage: scp <file> <host>:<path>\n");
        host_exit(1);
    }

    const char *file = &argv[i];
    unsigned int file_len = 0;
    while (file[file_len] && file[file_len] != ' ') file_len++;

    i += file_len;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("usage: scp <file> <host>:<path>\n");
        host_exit(1);
    }

    const char *dest = &argv[i];
    unsigned int dest_len = 0;
    while (dest[dest_len] && dest[dest_len] != ' ' && dest[dest_len] != '\n')
        dest_len++;

    print_str("Copying ");
    unsigned int off = alloc(file_len);
    copy_to_mem(off, file, file_len);
    host_print(off, file_len);
    print_str(" to ");
    off = alloc(dest_len);
    copy_to_mem(off, dest, dest_len);
    host_print(off, dest_len);
    print_str("...\n");

    char num_buf[16];
    my_itoa(1024, num_buf);
    print_str(num_buf);
    print_char(' ');

    off = alloc(file_len);
    copy_to_mem(off, file, file_len);
    host_print(off, file_len);
    print_str(" 100% 100KB/s 00:00\n");

    host_exit(0);
}
