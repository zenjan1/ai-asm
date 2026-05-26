/*
 * aiasm-aarch64/modules/rev/src/main.c
 * rev WASM module for AI-ASM AArch64 v41.0
 *
 * Usage: rev
 * Reads stdin line by line, outputs each line reversed.
 * Designed for use in shell pipes: echo "hello" | rev
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

#define MAX_LINE 4096

__attribute__((export_name("_start")))
void _start(void)
{
    /* Allocate line buffer once */
    unsigned int buf_off = alloc(MAX_LINE);
    char *buf = (char *)buf_off;
    unsigned int len = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: output remaining buffer if non-empty */
            if (len > 0) {
                /* Reverse in place */
                unsigned int i = 0, j = len - 1;
                while (i < j) {
                    char tmp = buf[i];
                    buf[i] = buf[j];
                    buf[j] = tmp;
                    i++;
                    j--;
                }
                wasm_host_print(buf_off, len);
                wasm_host_print(buf_off, 0);
                /* Print newline */
                unsigned int nl_off = alloc(1);
                char *nl = (char *)nl_off;
                nl[0] = '\n';
                wasm_host_print(nl_off, 1);
            }
            break;
        }

        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        if (c == '\n') {
            /* Reverse the line in place */
            unsigned int i = 0, j = len - 1;
            while (i < j) {
                char tmp = buf[i];
                buf[i] = buf[j];
                buf[j] = tmp;
                i++;
                j--;
            }
            wasm_host_print(buf_off, len);
            /* Print newline */
            unsigned int nl_off = alloc(1);
            char *nl = (char *)nl_off;
            nl[0] = '\n';
            wasm_host_print(nl_off, 1);
            len = 0;
        } else {
            if (len < MAX_LINE - 1) {
                buf[len++] = (char)c;
            }
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
