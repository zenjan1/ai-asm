/*
 * aiasm-aarch64/modules/fold/src/main.c
 * fold WASM module for AI-ASM AArch64 v50.0
 *
 * Usage: fold [-w WIDTH]
 * Wraps long lines to specified width (default 80).
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

__attribute__((export_name("_start")))
void _start(void)
{
    /* Parse -w WIDTH */
    unsigned int width = 80;
    unsigned int argv_off = alloc(128);
    int argv_len = wasm_host_get_argv(argv_off, 127);
    if (argv_len >= 0) {
        const char *argv = (const char *)argv_off;
        int i = 0;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
        if (i < argv_len && argv[i] == '-') {
            i++;
            if (i < argv_len && argv[i] == 'w') {
                i++;
                while (i < argv_len && argv[i] == ' ') i++;
                unsigned int w = 0;
                while (i < argv_len && argv[i] >= '0' && argv[i] <= '9') {
                    w = w * 10 + (unsigned int)(argv[i] - '0');
                    i++;
                }
                if (w > 0) width = w;
            }
        }
    }

    /* Buffer for accumulating characters */
    unsigned int buf_off = alloc(width);
    unsigned int pos = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: flush buffer */
            if (pos > 0) {
                host_print(buf_off, pos);
            }
            break;
        }

        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        if (c == '\n') {
            /* Output buffer + newline, reset */
            if (pos > 0) {
                host_print(buf_off, pos);
                pos = 0;
            }
            unsigned int nl_off = alloc(1);
            char *nl = (char *)nl_off;
            nl[0] = '\n';
            host_print(nl_off, 1);
        } else {
            /* Add character to buffer */
            char *p = (char *)(buf_off + pos);
            *p = (char)c;
            pos++;

            if (pos >= width) {
                /* Buffer full: output + newline, reset */
                host_print(buf_off, pos);
                unsigned int nl_off = alloc(1);
                char *nl = (char *)nl_off;
                nl[0] = '\n';
                host_print(nl_off, 1);
                pos = 0;
            }
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
