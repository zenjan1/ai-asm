/*
 * aiasm-aarch64/modules/diff/src/main.c
 * diff WASM module for AI-ASM AArch64 v43.0
 *
 * Usage: diff FILE1 FILE2
 * Compares two files line by line, outputs differences.
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

#define MAX_FILE_SIZE 8192
#define MAX_LINES 128

/* Global state for difference tracking */
static int has_differences = 0;

/* Compare two lines for equality */
static int lines_equal(unsigned int off1, int len1, unsigned int off2, int len2)
{
    if (len1 != len2) return 0;
    const char *a = (const char *)off1;
    const char *b = (const char *)off2;
    for (int k = 0; k < len1; k++) {
        if (a[k] != b[k]) return 0;
    }
    return 1;
}

/* Simple diff with lookahead */
static void do_diff(const char *f1_name, const char *f2_name,
                    unsigned int *l1_off, int *l1_len, int n1,
                    unsigned int *l2_off, int *l2_len, int n2)
{
    int i = 0, j = 0;

    while (i < n1 && j < n2) {
        if (lines_equal(l1_off[i], l1_len[i], l2_off[j], l2_len[j])) {
            i++;
            j++;
            continue;
        }

        /* Look ahead in file2 for match with current file1 line */
        int found_in2 = -1;
        for (int k = j + 1; k < n2 && k < j + 4; k++) {
            if (lines_equal(l1_off[i], l1_len[i], l2_off[k], l2_len[k])) {
                found_in2 = k;
                break;
            }
        }

        /* Look ahead in file1 for match with current file2 line */
        int found_in1 = -1;
        for (int k = i + 1; k < n1 && k < i + 4; k++) {
            if (lines_equal(l1_off[k], l1_len[k], l2_off[j], l2_len[j])) {
                found_in1 = k;
                break;
            }
        }

        has_differences = 1;

        if (found_in2 >= 0) {
            /* Lines added in file2 */
            for (int k = j; k < found_in2; k++) {
                print_str("> ");
                host_print(l2_off[k], (unsigned int)l2_len[k]);
                print_str("\n");
            }
            j = found_in2;
        } else if (found_in1 >= 0) {
            /* Lines deleted from file1 */
            for (int k = i; k < found_in1; k++) {
                print_str("< ");
                host_print(l1_off[k], (unsigned int)l1_len[k]);
                print_str("\n");
            }
            i = found_in1;
        } else {
            /* Changed line */
            print_str("< ");
            host_print(l1_off[i], (unsigned int)l1_len[i]);
            print_str("\n");
            print_str("> ");
            host_print(l2_off[j], (unsigned int)l2_len[j]);
            print_str("\n");
            i++;
            j++;
        }
        host_yield();
    }

    /* Remaining in file1 */
    while (i < n1) {
        has_differences = 1;
        print_str("< ");
        host_print(l1_off[i], (unsigned int)l1_len[i]);
        print_str("\n");
        i++;
        host_yield();
    }

    /* Remaining in file2 */
    while (j < n2) {
        has_differences = 1;
        print_str("> ");
        host_print(l2_off[j], (unsigned int)l2_len[j]);
        print_str("\n");
        j++;
        host_yield();
    }
}

__attribute__((export_name("_start")))
void _start(void)
{
    /* Parse arguments */
    unsigned int argv_off = alloc(512);
    int argv_len = host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    const char *file1 = &argv[i];
    while (i < argv_len && argv[i] != ' ') i++;
    int file1_len = (int)(&argv[i] - file1);
    while (i < argv_len && argv[i] == ' ') i++;

    const char *file2 = &argv[i];
    while (i < argv_len && argv[i] != ' ') i++;
    int file2_len = (int)(&argv[i] - file2);

    if (file1_len == 0 || file2_len == 0) {
        print_str("Usage: diff FILE1 FILE2\n");
        host_exit(2);
    }

    unsigned int f1_off = alloc((unsigned int)file1_len + 1);
    copy_to_mem(f1_off, file1, (unsigned int)file1_len);
    char *f1_name = (char *)f1_off;
    f1_name[file1_len] = '\0';

    unsigned int f2_off = alloc((unsigned int)file2_len + 1);
    copy_to_mem(f2_off, file2, (unsigned int)file2_len);
    char *f2_name = (char *)f2_off;
    f2_name[file2_len] = '\0';

    /* Allocate file buffers */
    unsigned int buf1_off = alloc(MAX_FILE_SIZE + 1);
    unsigned int buf2_off = alloc(MAX_FILE_SIZE + 1);

    /* Open file 1 */
    unsigned int fd1_off = alloc(4);
    if (host_fs_open((int)fd1_off, f1_off) != 0) {
        print_str("diff: cannot open '");
        print_str(f1_name);
        print_str("'\n");
        host_exit(2);
    }
    int fd1 = *(int *)fd1_off;

    /* Open file 2 */
    unsigned int fd2_off = alloc(4);
    if (host_fs_open((int)fd2_off, f2_off) != 0) {
        print_str("diff: cannot open '");
        print_str(f2_name);
        print_str("'\n");
        host_exit(2);
    }
    int fd2 = *(int *)fd2_off;

    /* Read file 1 */
    unsigned int total1 = 0;
    while (total1 < MAX_FILE_SIZE) {
        int n = host_fs_read(fd1, buf1_off + total1, 256);
        if (n <= 0) break;
        total1 += (unsigned int)n;
        host_yield();
    }
    ((char *)buf1_off)[total1] = '\0';

    /* Read file 2 */
    unsigned int total2 = 0;
    while (total2 < MAX_FILE_SIZE) {
        int n = host_fs_read(fd2, buf2_off + total2, 256);
        if (n <= 0) break;
        total2 += (unsigned int)n;
        host_yield();
    }
    ((char *)buf2_off)[total2] = '\0';

    host_fs_close(fd1);
    host_fs_close(fd2);

    /* Split into lines */
    unsigned int l1_off[MAX_LINES], l2_off[MAX_LINES];
    int l1_len[MAX_LINES], l2_len[MAX_LINES];
    int n1 = 0, n2 = 0;

    {
        int pos = 0, ls = 0;
        const char *t = (const char *)buf1_off;
        while (t[pos] && n1 < MAX_LINES) {
            if (t[pos] == '\n') {
                l1_off[n1] = buf1_off + ls;
                l1_len[n1] = pos - ls;
                n1++;
                ls = pos + 1;
            }
            pos++;
        }
        if (ls < pos && n1 < MAX_LINES) {
            l1_off[n1] = buf1_off + ls;
            l1_len[n1] = pos - ls;
            n1++;
        }
    }

    {
        int pos = 0, ls = 0;
        const char *t = (const char *)buf2_off;
        while (t[pos] && n2 < MAX_LINES) {
            if (t[pos] == '\n') {
                l2_off[n2] = buf2_off + ls;
                l2_len[n2] = pos - ls;
                n2++;
                ls = pos + 1;
            }
            pos++;
        }
        if (ls < pos && n2 < MAX_LINES) {
            l2_off[n2] = buf2_off + ls;
            l2_len[n2] = pos - ls;
            n2++;
        }
    }

    /* Perform diff */
    do_diff(f1_name, f2_name, l1_off, l1_len, n1, l2_off, l2_len, n2);

    host_exit(has_differences ? 1 : 0);
}
