/*
 * aiasm-aarch64/modules/grep/src/main.c
 * Simple grep WASM module for AI-ASM AArch64 kernel
 *
 * Usage: grep <pattern>
 * Reads lines from stdin (host_getc), outputs lines matching pattern.
 * Simple substring matching (no regex).
 *
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

static int my_strstr(const char *haystack, unsigned int hlen,
                     const char *needle, unsigned int nlen)
{
    if (nlen == 0) return 1;
    if (nlen > hlen) return 0;
    for (unsigned int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (unsigned int j = 0; j < nlen; j++) {
            if (haystack[i + j] != needle[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
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

static void print_line(const char *buf, unsigned int len)
{
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = buf[i];
    wasm_host_print(offset, len);
    print_str("\n");
}

/* -------------------------------------------------------------------------- */
/* Parse pattern from argv: "grep hello" -> pattern = "hello"                */
/* -------------------------------------------------------------------------- */

static unsigned int parse_pattern(const char *argv, unsigned int argv_len,
                                   char *pattern, unsigned int max_pat)
{
    /* Skip module name (first word) */
    unsigned int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    /* Skip spaces */
    while (i < argv_len && argv[i] == ' ') i++;
    /* Copy pattern */
    unsigned int plen = 0;
    while (i < argv_len && plen < max_pat - 1) {
        pattern[plen++] = argv[i++];
    }
    pattern[plen] = '\0';
    return plen;
}

#define MAX_LINE 1024
#define MAX_PATTERN 128

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get argv from host */
    unsigned int argv_off = alloc(128);
    int argv_len = wasm_host_get_argv(argv_off, 127);
    if (argv_len < 0) {
        print_str("grep: no args\n");
        wasm_host_exit(1);
    }

    /* Parse pattern from argv */
    char pattern[MAX_PATTERN];
    unsigned int plen = parse_pattern((const char *)argv_off, (unsigned int)argv_len,
                                       pattern, MAX_PATTERN);

    if (plen == 0) {
        print_str("grep: no pattern\n");
        wasm_host_exit(1);
    }

    /* Read stdin line by line, match pattern, output matches */
    unsigned int line_off = alloc(MAX_LINE);
    char *line = (char *)line_off;
    unsigned int pos = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF - check if there's a pending line */
            if (pos > 0) {
                if (my_strstr(line, pos, pattern, plen)) {
                    print_line(line, pos);
                }
            }
            break;
        }

        if (c == 0) {
            /* No data yet — yield and retry */
            wasm_host_yield();
            continue;
        }

        if (c == '\n' || c == '\r') {
            /* End of line - check match */
            if (pos > 0 && my_strstr(line, pos, pattern, plen)) {
                print_line(line, pos);
            }
            pos = 0;
        } else if (c >= 0x20 && c < 0x7f) {
            /* Printable character */
            if (pos < MAX_LINE - 1) {
                line[pos] = (char)c;
                pos++;
            }
        }
        /* Yield to avoid starving other processes */
        wasm_host_yield();
    }

    wasm_host_exit(0);
}
