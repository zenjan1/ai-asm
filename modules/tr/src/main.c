/*
 * aiasm-aarch64/modules/tr/src/main.c
 * tr WASM module for AI-ASM AArch64 v29.0
 *
 * Usage: tr [-d] SET1 [SET2]
 * Translates or deletes characters from stdin.
 * -d: delete characters in SET1
 *
 * Sets support ranges (a-z, A-Z, 0-9) and escapes (\n, \t).
 *
 * Designed for use in shell pipes: echo "hello" | tr 'a-z' 'A-Z'
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
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_ARGV 128

/* -------------------------------------------------------------------------- */
/* Character set parsing                                                      */
/* -------------------------------------------------------------------------- */

static int parse_char(const char *s, unsigned int len, unsigned int pos, unsigned int *out)
{
    if (pos >= len) return -1;
    if (s[pos] == '\\' && pos + 1 < len) {
        pos++;
        if (s[pos] == 'n') { *out = '\n'; return 1; }
        if (s[pos] == 't') { *out = '\t'; return 1; }
        if (s[pos] == '\\') { *out = '\\'; return 1; }
        pos--;
    }
    *out = (unsigned int)(unsigned char)s[pos];
    return 0;
}

/* Expand a set string into a lookup table.
 * table[c] = 0 means not in set, >0 means mapped value.
 * For translate: table2[c] holds the replacement.
 * Returns number of unique chars. */
static unsigned int expand_set(const char *s, unsigned int len, unsigned char *chars)
{
    unsigned int n = 0;
    unsigned int pos = 0;
    while (pos < len) {
        unsigned int ch;
        if (pos + 2 < len && s[pos + 1] == '-' && s[pos + 2] != '\\') {
            /* Range: a-z */
            unsigned int lo, hi;
            parse_char(s, len, pos, &lo);
            parse_char(s, len, pos + 2, &hi);
            for (unsigned int c = lo; c <= hi; c++)
                chars[n++] = (unsigned char)c;
            pos += 3;
        } else {
            unsigned int c;
            parse_char(s, len, pos, &c);
            chars[n++] = (unsigned char)c;
            pos++;
        }
    }
    return n;
}

/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */

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

    /* Parse flags */
    int delete_mode = 0;
    while (i < argv_len && argv[i] == '-') {
        if (argv[i + 1] == 'd') { delete_mode = 1; i += 2; }
        else break;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Parse SET1 and SET2 from argv.
     * Sets are passed as single arguments (quoted in shell).
     * We need to find SET1 and optionally SET2 in the remaining argv. */
    const char *set1_str = 0;
    unsigned int set1_len = 0;
    const char *set2_str = 0;
    unsigned int set2_len = 0;

    /* Skip to first non-space, non-dash argument (SET1) */
    while (i < argv_len && argv[i] == ' ') i++;
    if (i < argv_len) {
        set1_str = argv + i;
        while (i < argv_len && argv[i] != ' ') i++;
        set1_len = (unsigned int)(argv + i - set1_str);
    }

    while (i < argv_len && argv[i] == ' ') i++;
    if (i < argv_len) {
        set2_str = argv + i;
        while (i < argv_len && argv[i] != ' ') i++;
        set2_len = (unsigned int)(argv + i - set2_str);
    }

    /* Build translation table */
    unsigned char set1_chars[256], set2_chars[256];
    unsigned int n1 = 0, n2 = 0;

    if (set1_str) n1 = expand_set(set1_str, set1_len, set1_chars);
    if (set2_str) n2 = expand_set(set2_str, set2_len, set2_chars);

    /* Build lookup: translate[c] = replacement, delete_table[c] = 1 if should delete */
    unsigned char translate[256];
    unsigned char should_delete[256];
    for (int c = 0; c < 256; c++) {
        translate[c] = (unsigned char)c;
        should_delete[c] = 0;
    }

    if (delete_mode) {
        for (unsigned int j = 0; j < n1; j++)
            should_delete[set1_chars[j]] = 1;
    } else if (n1 > 0) {
        for (unsigned int j = 0; j < n1; j++) {
            if (j < n2)
                translate[set1_chars[j]] = set2_chars[j];
            else if (n2 > 0)
                translate[set1_chars[j]] = set2_chars[n2 - 1]; /* repeat last */
        }
    }

    /* Allocate output buffer */
    unsigned int buf_off = alloc(4096);
    char *buf = (char *)buf_off;
    unsigned int buf_pos = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: flush buffer */
            if (buf_pos > 0)
                wasm_host_print(buf_off, buf_pos);
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        unsigned char uc = (unsigned char)c;

        if (delete_mode) {
            if (!should_delete[uc]) {
                if (buf_pos < 4096)
                    buf[buf_pos++] = (char)uc;
                if (buf_pos >= 4096) {
                    wasm_host_print(buf_off, buf_pos);
                    buf_pos = 0;
                }
            }
        } else {
            unsigned char out = translate[uc];
            if (buf_pos < 4096)
                buf[buf_pos++] = (char)out;
            if (buf_pos >= 4096) {
                wasm_host_print(buf_off, buf_pos);
                buf_pos = 0;
            }
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
