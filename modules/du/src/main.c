/* du: estimate file space usage */
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

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

#define MAX_LIST_BUF 4096
#define READ_BUF 512

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

static unsigned int file_size(const char *name, unsigned int name_len)
{
    unsigned int path_off = alloc(name_len + 2);
    copy_to_mem(path_off, "/", 1);
    copy_to_mem(path_off + 1, name, name_len);
    char *path = (char *)(path_off + 1 + name_len);
    path[0] = '\0';

    int fd = host_fs_open(0, path_off);
    if (fd < 0) return 0;

    unsigned int total = 0;
    unsigned int buf_off = alloc(READ_BUF);
    int r;
    while ((r = host_fs_read(fd, buf_off, READ_BUF)) > 0)
        total += r;

    host_fs_close(fd);
    return total;
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    unsigned int list_off = alloc(MAX_LIST_BUF);
    int total = host_fs_list(list_off, MAX_LIST_BUF - 1);
    if (total <= 0) {
        host_exit(0);
        return;
    }

    const char *data = (const char *)list_off;
    int pos = 0;
    unsigned int grand_total = 0;
    while (pos < total) {
        const char *name = &data[pos];
        unsigned int len = my_strlen(name);
        if (len > 0) {
            unsigned int size = file_size(name, len);
            grand_total += size;

            /* Print size */
            char num_buf[16];
            int i = 0;
            unsigned int n = size;
            if (n == 0) { num_buf[0] = '0'; i = 1; }
            else {
                char tmp[16];
                int j = 0;
                while (n > 0) { tmp[j++] = '0' + (n % 10); n /= 10; }
                while (j > 0) num_buf[i++] = tmp[--j];
            }
            num_buf[i] = '\0';
            print_str(num_buf);
            print_char('\t');
            unsigned int off = alloc(len);
            copy_to_mem(off, name, len);
            host_print(off, len);
            print_char('\n');
        }
        pos += len + 1;
        if (pos >= total) break;
    }

    /* Print total */
    char num_buf[16];
    int i = 0;
    unsigned int n = grand_total;
    if (n == 0) { num_buf[0] = '0'; i = 1; }
    else {
        char tmp[16];
        int j = 0;
        while (n > 0) { tmp[j++] = '0' + (n % 10); n /= 10; }
        while (j > 0) num_buf[i++] = tmp[--j];
    }
    num_buf[i] = '\0';
    print_str(num_buf);
    print_char('\t');
    print_str("total\n");

    host_exit(0);
}
