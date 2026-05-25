/*
 * aiasm-aarch64/modules/echo/src/main.c
 * echo WASM module for AI-ASM AArch64 v21.0
 *
 * Usage: echo [text ...]
 * Prints arguments to stdout, separated by spaces, with trailing newline.
 * Designed for use in shell pipes: echo hello world | grep hello
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

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

static void print_buf(const char *buf, unsigned int len)
{
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = buf[i];
    wasm_host_print(offset, len);
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

#define MAX_ARGV 256

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from host: "echo hello world" */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip module name (first word) */
    while (i < argv_len && argv[i] != ' ') i++;
    /* Skip spaces after module name */
    while (i < argv_len && argv[i] == ' ') i++;

    /* Print remaining text as-is */
    if (i < argv_len) {
        unsigned int remaining = (unsigned int)(argv_len - i);
        unsigned int offset = alloc(remaining);
        char *dst = (char *)(offset);
        for (unsigned int j = 0; j < remaining; j++)
            dst[j] = argv[i + j];
        wasm_host_print(offset, remaining);
    }

    /* Trailing newline */
    print_str("\n");

    wasm_host_exit(0);
}
