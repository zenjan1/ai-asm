/*
 * aiasm-aarch64/modules/paste/src/main.c
 * paste WASM module for AI-ASM AArch64 v49.0
 *
 * Usage: paste [-d DELIM] FILE1 [FILE2 ...]
 * Merges corresponding lines from multiple files, separated by tab (or DELIM).
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

#define MAX_FILES 8
#define MAX_FILE_SIZE 2048
#define MAX_LINES 64

typedef struct {
    unsigned int offsets[MAX_LINES];
    unsigned int lens[MAX_LINES];
    int count;
} file_lines_t;

static int read_file_lines(int fd, file_lines_t *fl, unsigned int buf_off, unsigned int max_size)
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
    while (buf[pos] && count < MAX_LINES) {
        if (buf[pos] == '\n') {
            fl->offsets[count] = buf_off + ls;
            fl->lens[count] = (unsigned int)(pos - ls);
            count++;
            ls = pos + 1;
        }
        pos++;
    }
    if (ls < pos && count < MAX_LINES) {
        fl->offsets[count] = buf_off + ls;
        fl->lens[count] = (unsigned int)(pos - ls);
        count++;
    }
    fl->count = count;
    return count;
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(512);
    int argv_len = wasm_host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    char delim = '\t';

    /* Parse -d DELIM */
    if (i < argv_len && argv[i] == '-') {
        i++;
        if (i < argv_len && argv[i] == 'd') {
            i++;
            while (i < argv_len && argv[i] == ' ') i++;
            if (i < argv_len) delim = argv[i++];
            while (i < argv_len && argv[i] != ' ') i++;
        }
    }
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse filenames */
    int nfiles = 0;
    const char *files[MAX_FILES];
    int file_lens[MAX_FILES];

    while (i < argv_len && nfiles < MAX_FILES) {
        files[nfiles] = &argv[i];
        file_lens[nfiles] = 0;
        while (i < argv_len && argv[i] != ' ') { i++; file_lens[nfiles]++; }
        if (file_lens[nfiles] > 0) nfiles++;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    if (nfiles == 0) {
        host_exit(1);
    }

    /* Open and read all files */
    int fds[MAX_FILES];
    file_lines_t flines[MAX_FILES];

    for (int f = 0; f < nfiles; f++) {
        unsigned int foff = alloc((unsigned int)file_lens[f] + 1);
        copy_to_mem(foff, files[f], (unsigned int)file_lens[f]);
        ((char *)foff)[file_lens[f]] = '\0';

        unsigned int fd_off = alloc(4);
        if (host_fs_open((int)fd_off, foff) != 0) {
            host_exit(2);
        }
        fds[f] = *(int *)fd_off;

        unsigned int buf = alloc(MAX_FILE_SIZE + 1);
        read_file_lines(fds[f], &flines[f], buf, MAX_FILE_SIZE);
        host_fs_close(fds[f]);
    }

    /* Find max line count */
    int max_lines = 0;
    for (int f = 0; f < nfiles; f++) {
        if (flines[f].count > max_lines) max_lines = flines[f].count;
    }

    /* Output merged lines */
    for (int line = 0; line < max_lines; line++) {
        for (int f = 0; f < nfiles; f++) {
            if (f > 0) {
                unsigned int off = alloc(1);
                char *p = (char *)off;
                p[0] = delim;
                host_print(off, 1);
            }
            if (line < flines[f].count) {
                host_print(flines[f].offsets[line], flines[f].lens[line]);
            }
        }
        unsigned int off = alloc(1);
        char *p = (char *)off;
        p[0] = '\n';
        host_print(off, 1);
        wasm_host_yield();
    }

    host_exit(0);
}
