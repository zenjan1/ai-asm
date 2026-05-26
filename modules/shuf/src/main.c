/*
 * aiasm-aarch64/modules/shuf/src/main.c
 * shuf WASM module for AI-ASM AArch64 v47.0
 *
 * Usage: shuf [-n N]
 * Reads stdin lines, shuffles randomly, outputs in random order.
 * -n N: output at most N lines.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned int wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

#define MAX_LINES 256
#define MAX_LINE_LEN 256

static unsigned int line_offsets[MAX_LINES];
static int line_lens[MAX_LINES];

/* Simple LCG random */
static unsigned int rng_state;

static unsigned int rng_next(void)
{
    rng_state = rng_state * 1103515245 + 12345;
    return rng_state;
}

static unsigned int rng_range(unsigned int max)
{
    return rng_next() % max;
}

__attribute__((export_name("_start")))
void _start(void)
{
    /* Parse -n flag */
    unsigned int argv_off = alloc(128);
    int argv_len = wasm_host_get_argv(argv_off, 127);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    int max_lines = -1;
    if (i < argv_len && argv[i] == '-' && argv[i + 1] == 'n') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        unsigned int n = 0;
        while (i < argv_len && argv[i] >= '0' && argv[i] <= '9') {
            n = n * 10 + (unsigned int)(argv[i] - '0');
            i++;
        }
        max_lines = (int)n;
    }

    /* Seed RNG */
    rng_state = wasm_host_get_tick();

    /* Read all lines from stdin */
    unsigned int buf_off = alloc(MAX_LINES * MAX_LINE_LEN);
    unsigned int buf_pos = buf_off;
    int line_count = 0;
    unsigned int cur_len = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            if (cur_len > 0 && line_count < MAX_LINES) {
                line_offsets[line_count] = buf_pos;
                line_lens[line_count] = (int)cur_len;
                line_count++;
            }
            break;
        }

        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        if (c == '\n') {
            if (line_count < MAX_LINES) {
                line_offsets[line_count] = buf_pos;
                line_lens[line_count] = (int)cur_len;
                line_count++;
            }
            buf_pos += cur_len;
            cur_len = 0;
        } else {
            if (cur_len < MAX_LINE_LEN - 1) {
                char *p = (char *)(buf_pos + cur_len);
                *p = (char)c;
                cur_len++;
            }
        }

        wasm_host_yield();
    }

    /* Fisher-Yates shuffle */
    for (int j = line_count - 1; j > 0; j--) {
        unsigned int k = rng_range((unsigned int)(j + 1));
        unsigned int tmp_off = line_offsets[j];
        int tmp_len = line_lens[j];
        line_offsets[j] = line_offsets[k];
        line_lens[j] = line_lens[k];
        line_offsets[k] = tmp_off;
        line_lens[k] = tmp_len;
    }

    /* Output shuffled lines */
    int limit = max_lines < 0 ? line_count : (max_lines < line_count ? max_lines : line_count);
    for (int j = 0; j < limit; j++) {
        host_print(line_offsets[j], (unsigned int)line_lens[j]);
        unsigned int nl_off = alloc(1);
        char *nl = (char *)nl_off;
        nl[0] = '\n';
        host_print(nl_off, 1);
        wasm_host_yield();
    }

    wasm_host_exit(0);
}
