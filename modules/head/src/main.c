/*
 * aiasm-aarch64/modules/head/src/main.c
 * head WASM module for AI-ASM AArch64 v26.0
 *
 * Usage: head [-n N]
 * Outputs first N lines from stdin (default 10).
 *
 * Designed for use in shell pipes: cat file.txt | head -n 5
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

static unsigned int parse_uint(const char *s)
{
    unsigned int v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v * 10 + (unsigned int)(*s - '0');
        s++;
    }
    return v;
}

#define MAX_ARGV 128
#define MAX_BUF 4096

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip module name */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse -n N */
    unsigned int max_lines = 10;
    if (i + 3 < argv_len && argv[i] == '-' && argv[i + 1] == 'n') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        char num_buf[12];
        int ni = 0;
        while (i < argv_len && argv[i] >= '0' && argv[i] <= '9' && ni < 11)
            num_buf[ni++] = argv[i++];
        num_buf[ni] = '\0';
        if (ni > 0) max_lines = parse_uint(num_buf);
    }

    /* Read stdin, output first max_lines lines */
    unsigned int buf_off = alloc(MAX_BUF);
    char *buf = (char *)buf_off;
    unsigned int pos = 0;
    unsigned int lines = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF - output remaining buffer */
            if (pos > 0 && lines < max_lines) {
                print_buf(buf, pos);
            }
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        if (pos < MAX_BUF) {
            buf[pos++] = (char)c;
        }

        if (c == '\n') {
            if (lines < max_lines) {
                print_buf(buf, pos);
            }
            lines++;
            pos = 0;
            if (lines >= max_lines) break;
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
