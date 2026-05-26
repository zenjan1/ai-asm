/*
 * aiasm-aarch64/modules/tac/src/main.c
 * tac WASM module for AI-ASM AArch64 v51.0
 *
 * Usage: tac
 * Reads stdin, outputs lines in reverse order (last line first).
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

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

#define MAX_LINES 256
#define MAX_LINE_LEN 256

static unsigned int line_offsets[MAX_LINES];
static unsigned int line_lens[MAX_LINES];

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int buf_off = alloc(MAX_LINES * MAX_LINE_LEN);
    unsigned int buf_pos = buf_off;
    unsigned int cur_len = 0;
    int line_count = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            if (cur_len > 0 && line_count < MAX_LINES) {
                line_offsets[line_count] = buf_pos;
                line_lens[line_count] = cur_len;
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
                line_lens[line_count] = cur_len;
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

    /* Output in reverse order */
    for (int i = line_count - 1; i >= 0; i--) {
        host_print(line_offsets[i], line_lens[i]);
        unsigned int nl_off = alloc(1);
        char *nl = (char *)nl_off;
        nl[0] = '\n';
        host_print(nl_off, 1);
        wasm_host_yield();
    }

    wasm_host_exit(0);
}
