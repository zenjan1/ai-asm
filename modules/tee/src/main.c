/*
 * aiasm-aarch64/modules/tee/src/main.c
 * tee WASM module for AI-ASM AArch64 v24.0
 *
 * Usage: tee [-a] file
 * Reads stdin, writes to both stdout and file.
 * -a: append mode (like >>), without -a: truncate (like >)
 *
 * Designed for use in shell pipes: echo hello | tee /out.txt | grep hello
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

__attribute__((import_module("host"), import_name("fs_create")))
extern int wasm_host_fs_create(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int wasm_host_fs_write(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

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

#define MAX_ARGV 128
#define MAX_FILENAME 128

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from host: "tee [-a] file" */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip module name (first word) */
    while (i < argv_len && argv[i] != ' ') i++;
    /* Skip spaces */
    while (i < argv_len && argv[i] == ' ') i++;

    /* Check for -a flag */
    int append = 0;
    if (i + 2 < argv_len && argv[i] == '-' && argv[i + 1] == 'a') {
        append = 1;
        i += 2;
        /* Skip spaces after -a */
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Extract filename */
    char filename[MAX_FILENAME];
    int fi = 0;
    while (i < argv_len && fi < MAX_FILENAME - 1) {
        filename[fi++] = argv[i++];
    }
    filename[fi] = '\0';

    if (fi == 0) {
        print_str("tee: no filename\n");
        wasm_host_exit(1);
    }

    /* Open file: append mode uses fs_open, truncate uses fs_create */
    unsigned int flen = (unsigned int)fi;
    unsigned int path_off = alloc(flen);
    char *pd = (char *)path_off;
    for (unsigned int j = 0; j < flen; j++) pd[j] = filename[j];

    int fd;
    if (append) {
        fd = wasm_host_fs_open(path_off, flen);
    } else {
        fd = wasm_host_fs_create(path_off, flen);
    }
    if (fd < 0) {
        print_str("tee: cannot open '");
        print_str(filename);
        print_str("'\n");
        wasm_host_exit(1);
    }

    /* Read stdin byte by byte, write to both stdout and file */
    unsigned int char_off = alloc(1);
    char *char_buf = (char *)char_off;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) break;  /* EOF */
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        *char_buf = (char)c;

        /* Write to stdout */
        wasm_host_print(char_off, 1);

        /* Write to file */
        wasm_host_fs_write(fd, char_off, 1);

        wasm_host_yield();
    }

    wasm_host_fs_close(fd);
    wasm_host_exit(0);
}
