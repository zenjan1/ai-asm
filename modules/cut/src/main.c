/*
 * aiasm-aarch64/modules/cut/src/main.c
 * cut WASM module for AI-ASM AArch64 v30.0
 *
 * Usage: cut -d DELIM -f FIELDS
 * Extracts columns from stdin lines.
 * -d DELIM: field delimiter (default: tab)
 * -f FIELDS: comma-separated field numbers (1-based), supports ranges like 1-3
 *
 * Designed for use in shell pipes: cat file.txt | cut -d: -f1,3
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
#define MAX_LINE 512
#define MAX_FIELDS 64

/* -------------------------------------------------------------------------- */
/* Field selection                                                            */
/* -------------------------------------------------------------------------- */

/* Range: start-end (inclusive, 1-based). If start==0, means "from beginning".
 * If end==0xFFFFFFFF, means "to end". */
typedef struct { unsigned int start; unsigned int end; } field_range_t;

static field_range_t field_ranges[MAX_FIELDS];
static unsigned int num_ranges = 0;

/* Check if field number n is selected */
static int field_selected(unsigned int n)
{
    for (unsigned int i = 0; i < num_ranges; i++)
        if (n >= field_ranges[i].start && n <= field_ranges[i].end)
            return 1;
    return 0;
}

/* Parse field spec: "1,3,5-8,10" */
static void parse_fields(const char *s, unsigned int len)
{
    unsigned int pos = 0;
    while (pos < len && num_ranges < MAX_FIELDS) {
        /* Parse start number */
        unsigned int start = 0;
        while (pos < len && s[pos] >= '0' && s[pos] <= '9')
            start = start * 10 + (unsigned int)(s[pos++] - '0');

        if (pos < len && s[pos] == '-') {
            pos++; /* skip '-' */
            unsigned int end = 0;
            while (pos < len && s[pos] >= '0' && s[pos] <= '9')
                end = end * 10 + (unsigned int)(s[pos++] - '0');
            field_ranges[num_ranges].start = start;
            field_ranges[num_ranges].end = end;
            num_ranges++;
        } else {
            field_ranges[num_ranges].start = start;
            field_ranges[num_ranges].end = start;
            num_ranges++;
        }
        /* skip comma */
        if (pos < len && s[pos] == ',') pos++;
    }
}

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static void print_buf(unsigned int offset, unsigned int len)
{
    if (len > 0) wasm_host_print(offset, len);
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

    /* Parse options */
    char delim = '\t';
    unsigned int field_spec_off = 0, field_spec_len = 0;
    int char_mode = 0;

    while (i < argv_len && argv[i] == '-') {
        if (argv[i + 1] == 'd') {
            i += 2;
            while (i < argv_len && argv[i] == ' ') i++;
            if (i < argv_len) delim = argv[i];
            i++;
        } else if (argv[i + 1] == 'f') {
            i += 2;
            while (i < argv_len && argv[i] == ' ') i++;
            field_spec_off = i;
            while (i < argv_len && argv[i] != ' ') i++;
            field_spec_len = (unsigned int)(i - field_spec_off);
        } else if (argv[i + 1] == 'c') {
            i += 2;
            while (i < argv_len && argv[i] == ' ') i++;
            field_spec_off = i;
            while (i < argv_len && argv[i] != ' ') i++;
            field_spec_len = (unsigned int)(i - field_spec_off);
            char_mode = 1;
        } else {
            break;
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Parse field spec */
    if (field_spec_off > 0)
        parse_fields(argv + field_spec_off, field_spec_len);

    /* Allocate line buffer */
    unsigned int buf_off = alloc(MAX_LINE);
    char *buf = (char *)buf_off;

    /* Field start offsets */
    unsigned int field_off[MAX_FIELDS];
    unsigned int field_len[MAX_FIELDS];

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF: process last line if any */
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        if (ch != '\n') {
            /* Accumulate character */
            unsigned int buf_len = 0;
            /* We track length implicitly by counting chars since last newline */
            /* Use a simple counter stored in the first 4 bytes of buf */
            unsigned int *counter = (unsigned int *)buf_off;
            if (*counter < MAX_LINE - 4) {
                buf[4 + *counter] = ch;
                (*counter)++;
            }
        } else {
            /* End of line: process it */
            unsigned int *counter = (unsigned int *)buf_off;
            unsigned int line_len = *counter;

            if (char_mode) {
                /* Character mode: output selected character ranges */
                int first = 1;
                for (unsigned int r = 0; r < num_ranges; r++) {
                    unsigned int start = field_ranges[r].start;
                    unsigned int end = field_ranges[r].end;
                    if (start == 0) start = 1;
                    if (end > line_len) end = line_len;
                    if (start > line_len) continue;
                    if (start > end) continue;
                    if (!first) {
                        char dc = delim;
                        unsigned int o = alloc(1);
                        *(char *)o = dc;
                        wasm_host_print(o, 1);
                    }
                    wasm_host_print(buf_off + 4 + start - 1, end - start + 1);
                    first = 0;
                }
                wasm_host_print(buf_off + 4, 0); /* print newline */
                /* Actually print newline properly */
                unsigned int o = alloc(1);
                *(char *)o = '\n';
                wasm_host_print(o, 1);
            } else {
                /* Field mode: split by delimiter */
                unsigned int nf = 0;
                unsigned int field_start = 0;
                for (unsigned int j = 0; j < line_len; j++) {
                    if (buf[4 + j] == delim) {
                        if (nf < MAX_FIELDS) {
                            field_off[nf] = field_start;
                            field_len[nf] = j - field_start;
                            nf++;
                        }
                        field_start = j + 1;
                    }
                }
                /* Last field */
                if (nf < MAX_FIELDS) {
                    field_off[nf] = field_start;
                    field_len[nf] = line_len - field_start;
                    nf++;
                }

                /* Output selected fields */
                int first = 1;
                for (unsigned int f = 1; f <= nf; f++) {
                    if (field_selected(f)) {
                        if (!first) {
                            char dc = delim;
                            unsigned int o = alloc(1);
                            *(char *)o = dc;
                            wasm_host_print(o, 1);
                        }
                        print_buf(buf_off + 4 + field_off[f - 1], field_len[f - 1]);
                        first = 0;
                    }
                }
                /* Newline */
                unsigned int o = alloc(1);
                *(char *)o = '\n';
                wasm_host_print(o, 1);
            }

            /* Reset counter */
            *counter = 0;
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
