/*
 * aiasm-aarch64/modules/sed/src/main.c
 * sed WASM module for AI-ASM AArch64 v31.0
 *
 * Usage: sed [-n] 's/PATTERN/REPLACEMENT/[g]'
 * Stream editor: finds PATTERN in each line, replaces with REPLACEMENT.
 * -n: suppress automatic printing
 * g flag: replace all occurrences (global)
 *
 * Designed for use in shell pipes: echo "hello world" | sed 's/world/AI-ASM'
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

/* -------------------------------------------------------------------------- */
/* Pattern matching (simple string search)                                    */
/* -------------------------------------------------------------------------- */

static int find_pattern(const char *haystack, unsigned int hlen,
                        const char *needle, unsigned int nlen,
                        unsigned int *out_pos)
{
    if (nlen == 0 || nlen > hlen) return 0;
    for (unsigned int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (unsigned int j = 0; j < nlen; j++) {
            if (haystack[i + j] != needle[j]) { match = 0; break; }
        }
        if (match) { *out_pos = i; return 1; }
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Argument parsing                                                           */
/* -------------------------------------------------------------------------- */

static void parse_s_command(const char *cmd, unsigned int clen,
                            char *pattern, unsigned int *plen,
                            char *replacement, unsigned int *rlen,
                            int *global)
{
    /* Format: s/PATTERN/REPLACEMENT/[g] */
    unsigned int pos = 0;
    /* Skip 's' */
    while (pos < clen && cmd[pos] != 's') pos++;
    pos++; /* skip 's' */

    char delim = '/';
    if (pos < clen) delim = cmd[pos++];

    /* Parse pattern */
    *plen = 0;
    while (pos < clen && cmd[pos] != delim && *plen < MAX_LINE - 1)
        pattern[(*plen)++] = cmd[pos++];
    if (pos < clen) pos++; /* skip delim */

    /* Parse replacement */
    *rlen = 0;
    while (pos < clen && cmd[pos] != delim && *rlen < MAX_LINE - 1)
        replacement[(*rlen)++] = cmd[pos++];
    if (pos < clen) pos++; /* skip delim */

    /* Parse flags */
    *global = 0;
    while (pos < clen) {
        if (cmd[pos] == 'g') *global = 1;
        pos++;
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
    int suppress = 0;
    while (i < argv_len && argv[i] == '-') {
        if (argv[i + 1] == 'n') { suppress = 1; i += 2; }
        else break;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Parse sed command from argv */
    unsigned int cmd_off = 0, cmd_len = 0;
    if (i < argv_len) {
        cmd_off = i;
        while (i < argv_len && argv[i] != ' ') i++;
        cmd_len = (unsigned int)(i - cmd_off);
    }

    /* Extract pattern and replacement */
    char pattern[MAX_LINE] = {0};
    char replacement[MAX_LINE] = {0};
    unsigned int plen = 0, rlen = 0;
    int global = 0;

    if (cmd_len > 0)
        parse_s_command(argv + cmd_off, cmd_len, pattern, &plen, replacement, &rlen, &global);

    /* Allocate line buffer and output buffer */
    unsigned int line_off = alloc(MAX_LINE);
    unsigned int out_off = alloc(MAX_LINE * 2);
    char *line = (char *)line_off;
    char *out = (char *)out_off;

    for (;;) {
        int c = wasm_host_getc();

        if (c == -1) {
            /* EOF */
            break;
        }
        if (c == 0) {
            wasm_host_yield();
            continue;
        }

        char ch = (char)c;

        /* Store character */
        unsigned int *counter = (unsigned int *)line_off;
        if (*counter < MAX_LINE - 4) {
            line[4 + *counter] = ch;
            (*counter)++;
        }

        if (ch == '\n' || *counter >= MAX_LINE - 4) {
            /* Process line */
            unsigned int line_len = *counter;
            if (ch == '\n' && line_len > 0) line_len--; /* exclude newline */

            unsigned int out_len = 0;
            unsigned int pos = 0;
            unsigned int match_pos;

            if (plen > 0) {
                while (pos < line_len && find_pattern(line + 4 + pos, line_len - pos, pattern, plen, &match_pos)) {
                    /* Copy text before match */
                    unsigned int before = match_pos;
                    if (out_len + before < MAX_LINE * 2 - 4) {
                        for (unsigned int j = 0; j < before; j++)
                            out[4 + out_len++] = line[4 + pos + j];
                    }
                    /* Copy replacement */
                    if (out_len + rlen < MAX_LINE * 2 - 4) {
                        for (unsigned int j = 0; j < rlen; j++)
                            out[4 + out_len++] = replacement[j];
                    }
                    pos += match_pos + plen;

                    if (!global) break; /* only replace first */
                }
                /* Copy rest of line */
                while (pos < line_len && out_len < MAX_LINE * 2 - 4)
                    out[4 + out_len++] = line[4 + pos++];
            } else {
                /* No pattern: copy line as-is */
                for (unsigned int j = 0; j < line_len; j++)
                    out[4 + j] = line[4 + j];
                out_len = line_len;
            }

            /* Output */
            if (!suppress) {
                print_buf(out_off + 4, out_len);
                /* Newline */
                unsigned int no = alloc(1);
                *(char *)no = '\n';
                wasm_host_print(no, 1);
            }

            /* Reset counter */
            *counter = 0;
        }

        wasm_host_yield();
    }

    wasm_host_exit(0);
}
