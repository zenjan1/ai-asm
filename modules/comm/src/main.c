/*
 * aiasm-aarch64/modules/comm/src/main.c
 * comm WASM module for AI-ASM AArch64 v48.0
 *
 * Usage: comm [-1] [-2] [-3] FILE1 FILE2
 * Compares two sorted files, outputs 3 columns:
 *   col1: lines only in FILE1
 *   col2: lines only in FILE2
 *   col3: lines in both
 * Flags suppress columns.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

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

static int my_strcmp(const char *a, const char *b, unsigned int len_a, unsigned int len_b)
{
    if (len_a < len_b) return -1;
    if (len_a > len_b) return 1;
    for (unsigned int i = 0; i < len_a; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

static void print_tab(void)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = '\t';
    host_print(off, 1);
}

#define MAX_FILE_SIZE 4096
#define MAX_LINES 128

static unsigned int line_offsets1[MAX_LINES], line_offsets2[MAX_LINES];
static unsigned int line_lens1[MAX_LINES], line_lens2[MAX_LINES];

static int read_file_lines(int fd, unsigned int *offsets, unsigned int *lens, int max_lines,
                           unsigned int buf_off, unsigned int max_size)
{
    unsigned int total = 0;
    while (total < max_size) {
        int n = host_fs_read(fd, buf_off + total, 256);
        if (n <= 0) break;
        total += (unsigned int)n;
        wasm_host_yield();
    }
    char *buf = (char *)buf_off;
    buf[total] = '\0';

    int count = 0;
    int pos = 0, ls = 0;
    while (buf[pos] && count < max_lines) {
        if (buf[pos] == '\n') {
            offsets[count] = buf_off + ls;
            lens[count] = (unsigned int)(pos - ls);
            count++;
            ls = pos + 1;
        }
        pos++;
    }
    if (ls < pos && count < max_lines) {
        offsets[count] = buf_off + ls;
        lens[count] = (unsigned int)(pos - ls);
        count++;
    }
    return count;
}

__attribute__((export_name("_start")))
void _start(void)
{
    /* Parse arguments */
    unsigned int argv_off = alloc(512);
    int argv_len = wasm_host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    int hide1 = 0, hide2 = 0, hide3 = 0;

    /* Parse flags */
    while (i < argv_len && argv[i] == '-') {
        i++;
        while (i < argv_len && argv[i] >= '1' && argv[i] <= '3') {
            if (argv[i] == '1') hide1 = 1;
            else if (argv[i] == '2') hide2 = 1;
            else if (argv[i] == '3') hide3 = 1;
            i++;
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Parse FILE1 */
    const char *file1 = &argv[i];
    int file1_len = 0;
    while (i < argv_len && argv[i] != ' ') { i++; file1_len++; }
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse FILE2 */
    const char *file2 = &argv[i];
    int file2_len = 0;
    while (i < argv_len && argv[i] != ' ') { i++; file2_len++; }

    if (file1_len == 0 || file2_len == 0) {
        print_str("Usage: comm [-1] [-2] [-3] FILE1 FILE2\n");
        host_exit(2);
    }

    /* Null-terminate names */
    unsigned int f1_off = alloc((unsigned int)file1_len + 1);
    copy_to_mem(f1_off, file1, (unsigned int)file1_len);
    ((char *)f1_off)[file1_len] = '\0';

    unsigned int f2_off = alloc((unsigned int)file2_len + 1);
    copy_to_mem(f2_off, file2, (unsigned int)file2_len);
    ((char *)f2_off)[file2_len] = '\0';

    /* Open files */
    unsigned int fd1_off = alloc(4), fd2_off = alloc(4);
    if (host_fs_open((int)fd1_off, f1_off) != 0 ||
        host_fs_open((int)fd2_off, f2_off) != 0) {
        print_str("comm: cannot open file\n");
        host_exit(2);
    }
    int fd1 = *(int *)fd1_off;
    int fd2 = *(int *)fd2_off;

    /* Allocate buffers */
    unsigned int buf1_off = alloc(MAX_FILE_SIZE + 1);
    unsigned int buf2_off = alloc(MAX_FILE_SIZE + 1);

    int n1 = read_file_lines(fd1, line_offsets1, line_lens1, MAX_LINES, buf1_off, MAX_FILE_SIZE);
    int n2 = read_file_lines(fd2, line_offsets2, line_lens2, MAX_LINES, buf2_off, MAX_FILE_SIZE);

    host_fs_close(fd1);
    host_fs_close(fd2);

    /* Compare */
    int p1 = 0, p2 = 0;
    while (p1 < n1 && p2 < n2) {
        int cmp = my_strcmp((const char *)line_offsets1[p1], (const char *)line_offsets2[p2],
                           line_lens1[p1], line_lens2[p2]);
        if (cmp < 0) {
            /* Line only in file1 */
            if (!hide1) {
                host_print(line_offsets1[p1], line_lens1[p1]);
                print_str("\n");
            }
            p1++;
        } else if (cmp > 0) {
            /* Line only in file2 */
            if (!hide2) {
                print_tab();
                host_print(line_offsets2[p2], line_lens2[p2]);
                print_str("\n");
            }
            p2++;
        } else {
            /* Common line */
            if (!hide3) {
                print_tab();
                print_tab();
                host_print(line_offsets1[p1], line_lens1[p1]);
                print_str("\n");
            }
            p1++;
            p2++;
        }
        wasm_host_yield();
    }

    /* Remaining in file1 */
    while (p1 < n1) {
        if (!hide1) {
            host_print(line_offsets1[p1], line_lens1[p1]);
            print_str("\n");
        }
        p1++;
    }

    /* Remaining in file2 */
    while (p2 < n2) {
        if (!hide2) {
            print_tab();
            host_print(line_offsets2[p2], line_lens2[p2]);
            print_str("\n");
        }
        p2++;
    }

    host_exit(0);
}
