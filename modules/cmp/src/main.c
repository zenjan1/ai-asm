/*
 * aiasm-aarch64/modules/cmp/src/main.c
 * cmp WASM module for AI-ASM AArch64 v42.0
 *
 * Usage: cmp FILE1 FILE2
 * Compares two files byte by byte, reports first difference.
 * Exit 0 if identical, 1 if different, 2 on error.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

__attribute__((import_module("host"), import_name("yield")))
extern void host_yield(void);

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

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

static void print_uint(unsigned int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        while (v > 0) {
            buf[i++] = (char)('0' + (v % 10));
            v /= 10;
        }
    }
    unsigned int off = alloc((unsigned int)i);
    char *dst = (char *)off;
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    host_print(off, (unsigned int)i);
}

#define BUF_SIZE 64

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get command line arguments */
    unsigned int argv_off = alloc(512);
    int argv_len = host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip argv[0] ("cmp") */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse FILE1 */
    const char *file1 = &argv[i];
    while (i < argv_len && argv[i] != ' ') i++;
    int file1_len = (int)(&argv[i] - file1);

    /* Skip spaces */
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse FILE2 */
    const char *file2 = &argv[i];
    while (i < argv_len && argv[i] != ' ') i++;
    int file2_len = (int)(&argv[i] - file2);

    if (file1_len == 0 || file2_len == 0) {
        print_str("Usage: cmp FILE1 FILE2\n");
        host_exit(2);
        return;
    }

    /* Null-terminate file names */
    unsigned int f1_off = alloc((unsigned int)file1_len + 1);
    copy_to_mem(f1_off, file1, (unsigned int)file1_len);
    char *f1_name = (char *)f1_off;
    f1_name[file1_len] = '\0';

    unsigned int f2_off = alloc((unsigned int)file2_len + 1);
    copy_to_mem(f2_off, file2, (unsigned int)file2_len);
    char *f2_name = (char *)f2_off;
    f2_name[file2_len] = '\0';

    /* Open file 1 */
    unsigned int fd1_off = alloc(4);
    int rc1 = host_fs_open((int)fd1_off, f1_off);
    if (rc1 != 0) {
        print_str("cmp: cannot open '");
        print_str(f1_name);
        print_str("'\n");
        host_exit(2);
        return;
    }
    int fd1 = *(int *)fd1_off;

    /* Open file 2 */
    unsigned int fd2_off = alloc(4);
    int rc2 = host_fs_open((int)fd2_off, f2_off);
    if (rc2 != 0) {
        print_str("cmp: cannot open '");
        print_str(f2_name);
        print_str("'\n");
        host_exit(2);
        return;
    }
    int fd2 = *(int *)fd2_off;

    /* Allocate read buffers */
    unsigned int buf1_off = alloc(BUF_SIZE);
    unsigned int buf2_off = alloc(BUF_SIZE);

    unsigned int byte = 0;
    unsigned int line = 1;
    int eof1 = 0, eof2 = 0;
    unsigned int remaining1 = 0, remaining2 = 0;
    unsigned int pos1 = 0, pos2 = 0;

    for (;;) {
        /* Refill buffer 1 if empty */
        if (pos1 >= remaining1 && !eof1) {
            remaining1 = (unsigned int)host_fs_read(fd1, buf1_off, BUF_SIZE);
            if (remaining1 == 0) {
                eof1 = 1;
                remaining1 = 0;
            }
            pos1 = 0;
        }

        /* Refill buffer 2 if empty */
        if (pos2 >= remaining2 && !eof2) {
            remaining2 = (unsigned int)host_fs_read(fd2, buf2_off, BUF_SIZE);
            if (remaining2 == 0) {
                eof2 = 1;
                remaining2 = 0;
            }
            pos2 = 0;
        }

        /* Both EOF */
        if (eof1 && eof2) {
            break;  /* Files are identical */
        }

        /* One file shorter */
        if (eof1 != eof2) {
            print_str(f1_name);
            print_str(" ");
            print_str(f2_name);
            print_str(" differ: EOF on ");
            if (eof1) {
                print_str(f1_name);
            } else {
                print_str(f2_name);
            }
            print_str("\n");
            host_fs_close(fd1);
            host_fs_close(fd2);
            host_exit(1);
            return;
        }

        /* Compare byte */
        const char *d1 = (const char *)buf1_off;
        const char *d2 = (const char *)buf2_off;
        unsigned char b1 = (unsigned char)d1[pos1];
        unsigned char b2 = (unsigned char)d2[pos2];

        if (b1 != b2) {
            print_str(f1_name);
            print_str(" ");
            print_str(f2_name);
            print_str(" differ: byte ");
            print_uint(byte + 1);
            print_str(", line ");
            print_uint(line);
            print_str("\n");
            host_fs_close(fd1);
            host_fs_close(fd2);
            host_exit(1);
            return;
        }

        if (b1 == '\n') {
            line++;
        }

        pos1++;
        pos2++;
        byte++;
        host_yield();
    }

    host_fs_close(fd1);
    host_fs_close(fd2);
    host_exit(0);
}
