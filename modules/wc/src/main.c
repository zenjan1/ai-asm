/*
 * aiasm-aarch64/modules/wc/src/main.c
 * wc WASM module for AI-ASM AArch64 v25.0
 *
 * Usage: wc [-l] [-w] [-c]
 * Counts lines, words, bytes from stdin.
 * -l: only lines, -w: only words, -c: only bytes
 * Default: show all (lines words bytes)
 *
 * Designed for use in shell pipes: cat file.txt | wc
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

static void print_uint(unsigned int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        unsigned int n = v;
        while (n > 0) {
            buf[i++] = (char)('0' + (n % 10));
            n /= 10;
        }
    }
    unsigned int offset = alloc((unsigned int)i);
    char *dst = (char *)(offset);
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(offset, (unsigned int)i);
}

#define MAX_ARGV 128

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from host: "wc [-l] [-w] [-c]" */
    unsigned int argv_off = alloc(MAX_ARGV);
    int argv_len = wasm_host_get_argv(argv_off, MAX_ARGV - 1);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;

    /* Skip module name (first word) */
    while (i < argv_len && argv[i] != ' ') i++;
    /* Skip spaces */
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse flags */
    int show_lines = 0, show_words = 0, show_bytes = 0;
    while (i < argv_len) {
        if (argv[i] == '-' && i + 1 < argv_len) {
            char flag = argv[i + 1];
            if (flag == 'l') show_lines = 1;
            else if (flag == 'w') show_words = 1;
            else if (flag == 'c') show_bytes = 1;
            i += 2;
        } else {
            i++;
        }
    }

    /* Default: show all if none specified */
    if (!show_lines && !show_words && !show_bytes) {
        show_lines = 1;
        show_words = 1;
        show_bytes = 1;
    }

    /* Count from stdin */
    unsigned int lines = 0, words = 0, bytes = 0;
    int in_word = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) break;  /* EOF */
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        bytes++;

        if (c == '\n') {
            lines++;
        }

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            in_word = 0;
        } else {
            if (!in_word) {
                words++;
                in_word = 1;
            }
        }

        wasm_host_yield();
    }

    /* Output */
    int need_space = 0;
    if (show_lines) {
        print_uint(lines);
        need_space = 1;
    }
    if (show_words) {
        if (need_space) print_str(" ");
        print_uint(words);
        need_space = 1;
    }
    if (show_bytes) {
        if (need_space) print_str(" ");
        print_uint(bytes);
    }
    print_str("\n");

    wasm_host_exit(0);
}
