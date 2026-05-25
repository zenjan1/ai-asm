/*
 * aiasm-aarch64/modules/sort/src/main.c
 * sort WASM module for AI-ASM AArch64 v27.0
 *
 * Usage: sort [-r] [-n]
 * Reads all lines from stdin, sorts them, outputs to stdout.
 * -r: reverse order
 * -n: numeric sort
 *
 * Designed for use in shell pipes: cat file.txt | sort | head -n 5
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
/* Constants and data structures                                              */
/* -------------------------------------------------------------------------- */

#define MAX_ARGV 128
#define BUF_SIZE 4096
#define MAX_LINES 128

static unsigned int line_off[MAX_LINES];   /* offset of each line in buffer */
static unsigned int line_len[MAX_LINES];   /* length of each line */

/* -------------------------------------------------------------------------- */
/* Comparison functions                                                       */
/* -------------------------------------------------------------------------- */

static int compare_str(const char *a, unsigned int alen,
                       const char *b, unsigned int blen)
{
    unsigned int min_len = alen < blen ? alen : blen;
    for (unsigned int i = 0; i < min_len; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    if (alen < blen) return -1;
    if (alen > blen) return 1;
    return 0;
}

static unsigned int parse_line_number(const char *s, unsigned int len)
{
    unsigned int v = 0;
    unsigned int i = 0;
    while (i < len && (s[i] < '0' || s[i] > '9')) i++;
    while (i < len && s[i] >= '0' && s[i] <= '9') {
        v = v * 10 + (unsigned int)(s[i] - '0');
        i++;
    }
    return v;
}

static int compare_numeric(const char *a, unsigned int alen,
                           const char *b, unsigned int blen)
{
    unsigned int na = parse_line_number(a, alen);
    unsigned int nb = parse_line_number(b, blen);
    if (na < nb) return -1;
    if (na > nb) return 1;
    return 0;
}

static void swap_lines(unsigned int i, unsigned int j)
{
    unsigned int tmp_off = line_off[i];
    unsigned int tmp_len = line_len[i];
    line_off[i] = line_off[j];
    line_len[i] = line_len[j];
    line_off[j] = tmp_off;
    line_len[j] = tmp_len;
}

/* -------------------------------------------------------------------------- */
/* Argument parsing                                                           */
/* -------------------------------------------------------------------------- */

static unsigned int parse_uint(const char *s)
{
    unsigned int v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v * 10 + (unsigned int)(*s - '0');
        s++;
    }
    return v;
}

/* -------------------------------------------------------------------------- */
/* Main entry point                                                           */
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
    int reverse = 0;
    int numeric = 0;
    while (i < argv_len && argv[i] == '-') {
        if (argv[i + 1] == 'r') {
            reverse = 1;
            i += 2;
        } else if (argv[i + 1] == 'n') {
            numeric = 1;
            i += 2;
        } else {
            break;
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Allocate data buffer */
    unsigned int buf_off = alloc(BUF_SIZE);
    char *buf = (char *)buf_off;

    /* Read all lines from stdin */
    unsigned int buf_used = 0;
    unsigned int line_start = 0;
    unsigned int num_lines = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: finalize last line if there's content */
            if (buf_used > line_start && num_lines < MAX_LINES) {
                line_off[num_lines] = line_start;
                line_len[num_lines] = buf_used - line_start;
                num_lines++;
            }
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        /* Store character in buffer */
        if (buf_used < BUF_SIZE) {
            buf[buf_used++] = ch;
        }

        if (ch == '\n') {
            if (num_lines < MAX_LINES) {
                line_off[num_lines] = line_start;
                line_len[num_lines] = buf_used - line_start;
                num_lines++;
            }
            line_start = buf_used;
        }

        wasm_host_yield();
    }

    /* Sort: insertion sort */
    for (unsigned int j = 1; j < num_lines; j++) {
        unsigned int k = j;
        while (k > 0) {
            int cmp;
            if (numeric)
                cmp = compare_numeric(buf + line_off[k - 1], line_len[k - 1],
                                      buf + line_off[k], line_len[k]);
            else
                cmp = compare_str(buf + line_off[k - 1], line_len[k - 1],
                                  buf + line_off[k], line_len[k]);

            int should_swap = reverse ? (cmp > 0) : (cmp > 0);
            if (!should_swap) break;

            swap_lines(k - 1, k);
            k--;
        }
    }

    /* Output sorted lines */
    for (unsigned int j = 0; j < num_lines; j++) {
        wasm_host_print(buf_off + line_off[j], line_len[j]);
    }

    wasm_host_exit(0);
}
