/*
 * aiasm-aarch64/modules/tail/src/main.c
 * tail WASM module for AI-ASM AArch64 v26.0
 *
 * Usage: tail [-n N]
 * Outputs last N lines from stdin (default 10).
 *
 * Designed for use in shell pipes: cat file.txt | tail -n 5
 *
 * Uses a ring buffer of line offsets to track last N lines.
 * Input is stored in a fixed 4KB buffer; when full, oldest data is discarded.
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
#define BUF_SIZE 4096
#define MAX_LINES 256

/* Ring buffer of line start offsets within buf */
static unsigned int line_off[MAX_LINES];

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

    if (max_lines > MAX_LINES) max_lines = MAX_LINES;

    /* Data buffer */
    unsigned int buf_off = alloc(BUF_SIZE);
    char *buf = (char *)buf_off;

    /*
     * Ring buffer for line offsets.
     * lines[i % max_lines] stores the buffer offset where line i starts.
     * line_lens[i % max_lines] stores the length of line i.
     */
    unsigned int line_lens[MAX_LINES];
    unsigned int total_lines = 0;

    /* Input buffer management */
    unsigned int buf_used = 0;   /* bytes currently in buf */
    unsigned int buf_start = 0;  /* logical start offset (discarded bytes before this) */
    unsigned int line_start_in_buf = 0; /* offset in buf where current line starts */

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: finalize last line if there's content */
            if (buf_used > line_start_in_buf) {
                unsigned int idx = total_lines % max_lines;
                line_off[idx] = line_start_in_buf;
                line_lens[idx] = buf_used - line_start_in_buf;
                total_lines++;
            }
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        /* Check if buffer is full - need to discard oldest data */
        if (buf_used >= BUF_SIZE) {
            /* Find the first newline after buf_start and discard up to it */
            unsigned int cut = buf_used;
            for (unsigned int p = buf_start; p < buf_used; p++) {
                if (buf[p] == '\n') {
                    cut = p + 1;
                    break;
                }
            }
            if (cut >= buf_used) {
                /* No newline found, discard half the buffer */
                cut = buf_used / 2;
            }
            unsigned int keep = buf_used - cut;
            for (unsigned int k = 0; k < keep; k++)
                buf[k] = buf[cut + k];
            buf_used = keep;
            buf_start += cut;
            /* Adjust line_start_in_buf */
            if (line_start_in_buf >= cut)
                line_start_in_buf -= cut;
            else
                line_start_in_buf = 0;
        }

        buf[buf_used++] = ch;

        if (ch == '\n') {
            unsigned int idx = total_lines % max_lines;
            line_off[idx] = line_start_in_buf;
            line_lens[idx] = buf_used - line_start_in_buf;
            total_lines++;
            line_start_in_buf = buf_used;
        }

        wasm_host_yield();
    }

    /* Output last N lines */
    unsigned int start = 0;
    if (total_lines > max_lines)
        start = total_lines - max_lines;

    for (unsigned int li = start; li < total_lines; li++) {
        unsigned int idx = li % max_lines;
        wasm_host_print(buf_off + line_off[idx], line_lens[idx]);
    }

    wasm_host_exit(0);
}
