/* envsubst: environment variable substitution (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("get_env")))
extern int host_get_env(unsigned int buf_off, unsigned int max_len);

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

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("usage: envsubst <template>\n");
        host_exit(1);
    }

    /* Get input template */
    const char *input = &argv[i];
    unsigned int inlen = 0;
    while (input[inlen] && input[inlen] != '\n') inlen++;

    /* Scan for $VAR patterns and substitute */
    unsigned int pos = 0;
    while (pos < inlen) {
        if (input[pos] == '$' && pos + 1 < inlen && (input[pos + 1] >= 'A' && input[pos + 1] <= 'Z')) {
            /* Found variable reference */
            unsigned int var_start = pos + 1;
            unsigned int var_end = var_start;
            while (var_end < inlen && ((input[var_end] >= 'A' && input[var_end] <= 'Z') || (input[var_end] >= '0' && input[var_end] <= '9') || input[var_end] == '_'))
                var_end++;

            /* Print variable name for lookup */
            print_str("${");
            unsigned int vlen = var_end - var_start;
            unsigned int off = alloc(vlen);
            copy_to_mem(off, &input[var_start], vlen);
            host_print(off, vlen);
            print_str("}\n");

            pos = var_end;
        } else {
            /* Print literal character */
            char buf[1];
            buf[0] = input[pos];
            unsigned int off = alloc(1);
            copy_to_mem(off, buf, 1);
            host_print(off, 1);
            pos++;
        }
    }
    print_str("\n");

    host_exit(0);
}
