/*
 * aiasm-aarch64/modules/ls/src/main.c
 * ls WASM module for AI-ASM AArch64 v35.0
 *
 * Usage: ls [-l] [path]
 * Lists files from ramdisk (host_fs_list)
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("fs_list")))
extern int host_fs_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_LIST_BUF  4096
#define MAX_NAME_LEN  128

/* -------------------------------------------------------------------------- */
/* Memory (bump allocator)                                                    */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || a[i] != b[i])
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Output helpers                                                             */
/* -------------------------------------------------------------------------- */

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

static void my_itoa(int n, char *buf)
{
    char tmp[16];
    int i = 0;
    if (n < 0) { *buf++ = '-'; n = -n; }
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (n > 0) { tmp[i++] = (char)('0' + (n % 10)); n /= 10; }
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    int long_format = 0;

    /* Parse arguments */
    unsigned int argv_off = alloc(1024);
    int argv_len = host_get_argv(argv_off, 1023);
    if (argv_len >= 0) {
        const char *argv = (const char *)argv_off;
        int i = 0;
        /* Skip argv[0] ("ls") */
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
        /* Check for -l flag */
        if (i + 1 < argv_len && argv[i] == '-' && argv[i + 1] == 'l') {
            long_format = 1;
        }
    }

    /* Get file list from ramdisk */
    unsigned int list_off = alloc(MAX_LIST_BUF);
    int total = host_fs_list(list_off, MAX_LIST_BUF - 1);

    if (total <= 0) {
        host_exit(0);
        return;
    }

    const char *data = (const char *)list_off;

    if (long_format) {
        /* -l format: count files first, then print detailed listing */
        /* Count entries */
        int count = 0;
        int pos = 0;
        while (pos < total) {
            const char *name = &data[pos];
            unsigned int len = my_strlen(name);
            if (len > 0) count++;
            pos += len + 1;
            if (pos >= total) break;
        }

        /* Print header */
        char num_buf[16];
        my_itoa(count, num_buf);
        print_str("total ");
        print_str(num_buf);
        print_char('\n');

        /* List each file */
        pos = 0;
        while (pos < total) {
            const char *name = &data[pos];
            unsigned int len = my_strlen(name);
            if (len > 0) {
                /* Format: -rw-r--r--  1 root  root  512  name */
                print_str("-rw-r--r--  1 root  root  512  ");
                unsigned int off = alloc(len);
                copy_to_mem(off, name, len);
                host_print(off, len);
                print_char('\n');
            }
            pos += len + 1;
            if (pos >= total) break;
        }
    } else {
        /* Default: one line per file */
        int pos = 0;
        int first = 1;
        while (pos < total) {
            const char *name = &data[pos];
            unsigned int len = my_strlen(name);
            if (len > 0) {
                if (!first) {
                    print_str("  ");
                }
                unsigned int off = alloc(len);
                copy_to_mem(off, name, len);
                host_print(off, len);
                first = 0;
            }
            pos += len + 1;
            if (pos >= total) break;
        }
        if (!first) print_char('\n');
    }

    host_exit(0);
}
