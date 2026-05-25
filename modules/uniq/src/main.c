/*
 * aiasm-aarch64/modules/uniq/src/main.c
 * uniq WASM module for AI-ASM AArch64 v28.0
 *
 * Usage: uniq [-c] [-d] [-u]
 * Removes or reports adjacent duplicate lines from stdin.
 * -c: prefix each line with count of occurrences
 * -d: output only duplicate lines (one per group)
 * -u: output only unique lines
 *
 * Designed for use in shell pipes: sort file.txt | uniq
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
#define MAX_LINE 256

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static int line_equal(const char *a, unsigned int alen,
                      const char *b, unsigned int blen)
{
    if (alen != blen) return 0;
    for (unsigned int i = 0; i < alen; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

static void print_str(const char *s, unsigned int len)
{
    unsigned int offset = alloc(len);
    char *dst = (char *)offset;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(offset, len);
}

/* Format count prefix: "      N" (6 digits + space) */
static void print_count_prefix(unsigned int count)
{
    char buf[8];
    buf[7] = '\0';
    int i = 6;
    do {
        buf[i--] = (char)('0' + (count % 10));
        count /= 10;
    } while (i >= 0 && count > 0);
    while (i >= 0) buf[i--] = ' ';
    print_str(buf, 7);
}

static unsigned int format_uint(unsigned int v, char *buf)
{
    if (v == 0) { buf[0] = '0'; return 1; }
    int i = 0;
    while (v > 0) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }
    /* Reverse */
    for (int j = 0; j < i / 2; j++) {
        char t = buf[j]; buf[j] = buf[i - 1 - j]; buf[i - 1 - j] = t;
    }
    return (unsigned int)i;
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
    int count_flag = 0;
    int dup_only = 0;
    int unique_only = 0;
    while (i < argv_len && argv[i] == '-') {
        if (argv[i + 1] == 'c') { count_flag = 1; i += 2; }
        else if (argv[i + 1] == 'd') { dup_only = 1; i += 2; }
        else if (argv[i + 1] == 'u') { unique_only = 1; i += 2; }
        else break;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Allocate line buffers */
    unsigned int prev_off = alloc(MAX_LINE);
    unsigned int curr_off = alloc(MAX_LINE);
    char *prev = (char *)prev_off;
    char *curr = (char *)curr_off;

    unsigned int prev_len = 0;
    unsigned int curr_len = 0;
    unsigned int count = 0;
    int has_prev = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: finalize last line */
            if (curr_len > 0) count++;

            /* Output last group */
            if (has_prev) {
                int output = 1;
                if (dup_only && count < 2) output = 0;
                if (unique_only && count != 1) output = 0;

                if (output) {
                    if (count_flag) {
                        char num_buf[12];
                        unsigned int nlen = format_uint(count, num_buf);
                        print_str(num_buf, nlen);
                        wasm_host_print(prev_off, prev_len);
                    } else {
                        wasm_host_print(prev_off, prev_len);
                    }
                }
            }
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        if (ch != '\n') {
            if (curr_len < MAX_LINE - 1)
                curr[curr_len++] = ch;
        } else {
            /* End of line */
            if (curr_len < MAX_LINE)
                curr[curr_len++] = '\n';

            if (has_prev && line_equal(prev, prev_len, curr, curr_len)) {
                /* Duplicate: increment count */
                count++;
            } else {
                /* New line: output previous group */
                if (has_prev) {
                    int output = 1;
                    if (dup_only && count < 2) output = 0;
                    if (unique_only && count != 1) output = 0;

                    if (output) {
                        if (count_flag) {
                            char num_buf[12];
                            unsigned int nlen = format_uint(count, num_buf);
                            print_str(num_buf, nlen);
                            wasm_host_print(prev_off, prev_len);
                        } else {
                            wasm_host_print(prev_off, prev_len);
                        }
                    }
                }

                /* Copy curr to prev */
                for (unsigned int j = 0; j < curr_len; j++)
                    prev[j] = curr[j];
                prev_len = curr_len;
                count = 1;
                has_prev = 1;
                curr_len = 0;
            }
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
