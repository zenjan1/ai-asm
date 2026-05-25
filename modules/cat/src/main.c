/*
 * aiasm-aarch64/modules/cat/src/main.c
 * cat WASM module for AI-ASM AArch64 v21.0
 *
 * Usage: cat [file ...]
 * Reads files and outputs to stdout. Supports pipe stdin (no args mode).
 * Designed for use in shell pipes: cat file.txt | grep hello
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern int wasm_host_fs_close(int fd);

/* -------------------------------------------------------------------------- */
/* WASM memory allocator (bump)                                               */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(offset, len);
}

static void print_buf(const char *buf, unsigned int len)
{
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = buf[i];
    wasm_host_print(offset, len);
}

/* -------------------------------------------------------------------------- */
/* Parse filenames from argv: "cat file1.txt file2.txt"                      */
/* Returns pointer to first filename and count of files.                      */
/* -------------------------------------------------------------------------- */

#define MAX_FILES 8
#define MAX_FILENAME 128

static int parse_files(const char *argv, int argv_len, char filenames[MAX_FILES][MAX_FILENAME])
{
    int i = 0;
    /* Skip module name (first word) */
    while (i < argv_len && argv[i] != ' ') i++;
    /* Skip spaces */
    while (i < argv_len && argv[i] == ' ') i++;

    int count = 0;
    while (i < argv_len && count < MAX_FILES) {
        /* Copy filename */
        int start = 0;
        while (i < argv_len && argv[i] != ' ' && start < MAX_FILENAME - 1) {
            filenames[count][start++] = argv[i++];
        }
        filenames[count][start] = '\0';
        if (start > 0) count++;
        /* Skip spaces */
        while (i < argv_len && argv[i] == ' ') i++;
    }
    return count;
}

/* -------------------------------------------------------------------------- */
/* Read from stdin (pipe mode)                                                */
/* -------------------------------------------------------------------------- */

static void cat_stdin(void)
{
    unsigned int buf_off = alloc(512);
    char *buf = (char *)buf_off;
    int len = 0;

    for (int i = 0; i < 512; i++) buf[i] = 0;

    for (;;) {
        int c = wasm_host_getc();
        if (c == -1) break;  /* EOF */
        if (c == 0) {        /* No data — yield and retry */
            wasm_host_yield();
            continue;
        }
        buf[len++] = (char)c;
        if (len >= 512) {
            print_buf(buf, (unsigned int)len);
            len = 0;
        }
        wasm_host_yield();
    }
    if (len > 0) {
        print_buf(buf, (unsigned int)len);
    }
}

/* -------------------------------------------------------------------------- */
/* Read from file                                                             */
/* -------------------------------------------------------------------------- */

static int cat_file(const char *path, unsigned int path_len)
{
    /* Copy path to WASM memory for fs_open */
    unsigned int path_off = alloc(path_len);
    char *dst = (char *)(path_off);
    for (unsigned int i = 0; i < path_len; i++)
        dst[i] = path[i];

    int fd = wasm_host_fs_open(path_off, path_len);
    if (fd < 0) {
        print_str("cat: cannot open '");
        print_buf(path, path_len);
        print_str("'\n");
        return -1;
    }

    /* Read in 512-byte chunks */
    unsigned int buf_off = alloc(512);
    for (;;) {
        int n = wasm_host_fs_read(fd, buf_off, 512);
        if (n <= 0) break;
        wasm_host_print(buf_off, (unsigned int)n);
        wasm_host_yield();
    }

    wasm_host_fs_close(fd);
    return 0;
}

#define MAX_ARGV 128

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from host */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    /* Parse filenames */
    char filenames[MAX_FILES][MAX_FILENAME];
    int nfiles = parse_files((const char *)argv_off, argv_len, filenames);

    if (nfiles == 0) {
        /* No arguments: read from stdin */
        cat_stdin();
    } else {
        /* Read each file */
        for (int i = 0; i < nfiles; i++) {
            unsigned int flen = my_strlen(filenames[i]);
            if (cat_file(filenames[i], flen) < 0) {
                wasm_host_exit(1);
            }
        }
    }

    wasm_host_exit(0);
}
