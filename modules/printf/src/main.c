/*
 * aiasm-aarch64/modules/printf/src/main.c
 * printf WASM module for AI-ASM AArch64 v38.0
 *
 * Usage: printf FORMAT [ARGUMENTS...]
 * Format: %s (string), %d (decimal), %x (hex), %c (char), %% (literal %), \n \t \0
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

/* -------------------------------------------------------------------------- */
/* Memory (bump allocator)                                                    */
/* -------------------------------------------------------------------------- */

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

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

/* -------------------------------------------------------------------------- */
/* Output buffer                                                              */
/* -------------------------------------------------------------------------- */

static char out_buf[512];
static unsigned int out_pos = 0;

static void flush_out(void)
{
    if (out_pos > 0) {
        unsigned int off = alloc(out_pos);
        copy_to_mem(off, out_buf, out_pos);
        host_print(off, out_pos);
        out_pos = 0;
    }
}

static void out_char(char c)
{
    if (out_pos >= sizeof(out_buf)) flush_out();
    out_buf[out_pos++] = c;
}

static void out_str(const char *s)
{
    while (*s) {
        if (out_pos >= sizeof(out_buf)) flush_out();
        out_buf[out_pos++] = *s++;
    }
}

static void out_uint(unsigned int n, unsigned int base, int upper)
{
    char tmp[32];
    int i = 0;
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    if (n == 0) { out_char('0'); return; }
    while (n > 0) { tmp[i++] = digits[n % base]; n /= base; }
    while (i > 0) out_char(tmp[--i]);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get command line arguments */
    unsigned int argv_off = alloc(4096);
    int argv_len = host_get_argv(argv_off, 4095);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int arg_idx = 1;  /* Start after argv[0] ("printf") */

    /* Find format string (first argument after "printf") */
    /* Skip "printf" */
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Read format string */
    const char *fmt = &argv[i];
    int fmt_len = 0;
    while (i < argv_len && argv[i] != ' ') { fmt_len++; i++; }

    /* Collect remaining arguments */
    const char *args[16];
    int arg_lens[16];
    int nargs = 0;
    while (i < argv_len && nargs < 16) {
        while (i < argv_len && argv[i] == ' ') i++;
        if (i >= argv_len) break;
        args[nargs] = &argv[i];
        arg_lens[nargs] = 0;
        while (i < argv_len && argv[i] != ' ') { arg_lens[nargs]++; i++; }
        nargs++;
    }

    /* Process format string */
    int fi = 0;
    while (fi < fmt_len) {
        char c = fmt[fi++];

        if (c == '%') {
            if (fi >= fmt_len) { out_char('%'); break; }
            char fc = fmt[fi++];

            if (fc == 's') {
                /* Print next argument as string */
                if (arg_idx < nargs)
                    out_str(args[arg_idx++]);
            } else if (fc == 'd') {
                /* Print next argument as decimal */
                if (arg_idx < nargs) {
                    /* Parse decimal from arg */
                    int val = 0, neg = 0, p = 0;
                    if (args[arg_idx][0] == '-') { neg = 1; p = 1; }
                    while (p < arg_lens[arg_idx]) {
                        char d = args[arg_idx][p++];
                        if (d >= '0' && d <= '9') val = val * 10 + (d - '0');
                    }
                    if (neg) { out_char('-'); }
                    /* Convert val to decimal string */
                    char tmp[32];
                    int ti = 0;
                    if (val == 0) { out_char('0'); }
                    else {
                        while (val > 0) { tmp[ti++] = (char)('0' + (val % 10)); val /= 10; }
                        while (ti > 0) out_char(tmp[--ti]);
                    }
                    arg_idx++;
                }
            } else if (fc == 'x' || fc == 'X') {
                /* Print next argument as hex */
                if (arg_idx < nargs) {
                    unsigned int val = 0;
                    int p = 0;
                    if (arg_lens[arg_idx] > 0 && args[arg_idx][0] == '-') p = 1;
                    while (p < arg_lens[arg_idx]) {
                        char d = args[arg_idx][p++];
                        if (d >= '0' && d <= '9') val = val * 10 + (d - '0');
                    }
                    out_uint(val, 16, fc == 'X');
                    arg_idx++;
                }
            } else if (fc == 'c') {
                /* Print first char of next argument */
                if (arg_idx < nargs && arg_lens[arg_idx] > 0)
                    out_char(args[arg_idx][0]);
                arg_idx++;
            } else if (fc == '%') {
                out_char('%');
            } else {
                out_char('%');
                out_char(fc);
            }
        } else if (c == '\\') {
            if (fi >= fmt_len) { out_char('\\'); break; }
            char ec = fmt[fi++];
            if (ec == 'n') out_char('\n');
            else if (ec == 't') out_char('\t');
            else if (ec == '\\') out_char('\\');
            else if (ec == '0') out_char('\0');
            else { out_char('\\'); out_char(ec); }
        } else {
            out_char(c);
        }
    }

    flush_out();
    host_exit(0);
}
