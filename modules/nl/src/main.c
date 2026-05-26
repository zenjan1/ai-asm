/*
 * aiasm-aarch64/modules/nl/src/main.c
 * nl WASM module for AI-ASM AArch64 v46.0
 *
 * Usage: nl
 * Reads stdin, outputs each line with a line number.
 * Format: "     N  content" (6-digit right-aligned)
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

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

#define MAX_LINE 1024
static char line_buf[MAX_LINE];

static void print_line_number(unsigned int n)
{
    char buf[8];
    int i = 0;
    unsigned int tmp = n;
    do {
        buf[i++] = (char)('0' + (tmp % 10));
        tmp /= 10;
    } while (tmp > 0);
    /* Reverse to get correct order */
    int len = i;
    /* Pad to 6 digits */
    while (len < 6) { buf[i++] = ' '; len++; }
    /* buf now has digits reversed, then spaces */
    /* We need: spaces first, then digits forward */
    /* Rebuild properly */
    char num[8];
    int ndigits = 0;
    tmp = n;
    do {
        num[ndigits++] = (char)('0' + (tmp % 10));
        tmp /= 10;
    } while (tmp > 0);
    /* Reverse num in place */
    for (int a = 0, b = ndigits - 1; a < b; a++, b--) {
        char t = num[a]; num[a] = num[b]; num[b] = t;
    }
    /* Print spaces then digits */
    int spaces = 6 - ndigits;
    for (int s = 0; s < spaces; s++) {
        unsigned int off = alloc(1);
        char *p = (char *)off;
        p[0] = ' ';
        host_print(off, 1);
    }
    unsigned int off = alloc((unsigned int)ndigits);
    copy_to_mem(off, num, (unsigned int)ndigits);
    host_print(off, (unsigned int)ndigits);
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int line_num = 1;
    unsigned int pos = 0;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: output remaining buffer */
            if (pos > 0) {
                print_line_number(line_num);
                unsigned int off = alloc(2);
                char *p = (char *)off;
                p[0] = ' '; p[1] = ' ';
                host_print(off, 2);
                unsigned int line_off = alloc(pos);
                copy_to_mem(line_off, line_buf, pos);
                host_print(line_off, pos);
                unsigned int nl_off = alloc(1);
                char *nl = (char *)nl_off;
                nl[0] = '\n';
                host_print(nl_off, 1);
            }
            break;
        }

        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        if (c == '\n') {
            /* Print line number and line */
            print_line_number(line_num);
            unsigned int off = alloc(2);
            char *p = (char *)off;
            p[0] = ' '; p[1] = ' ';
            host_print(off, 2);
            unsigned int line_off = alloc(pos);
            copy_to_mem(line_off, line_buf, pos);
            host_print(line_off, pos);
            unsigned int nl_off = alloc(1);
            char *nl = (char *)nl_off;
            nl[0] = '\n';
            host_print(nl_off, 1);
            line_num++;
            pos = 0;
        } else {
            if (pos < MAX_LINE - 1) {
                line_buf[pos++] = (char)c;
            }
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
