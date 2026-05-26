/*
 * aiasm-aarch64/modules/env/src/main.c
 * env WASM module for AI-ASM AArch64 v37.0
 *
 * Usage: env          - list all environment variables
 *        env KEY=VAL  - set variable then list
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("env_list")))
extern int host_env_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("env_set")))
extern int host_env_set(unsigned int key_off, unsigned int key_len,
                         unsigned int val_off, unsigned int val_len);

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

static void print_char(char c)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = c;
    host_print(off, 1);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Parse arguments */
    unsigned int argv_off = alloc(2048);
    int argv_len = host_get_argv(argv_off, 2047);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;

    /* Skip argv[0] ("env") */
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Process KEY=VALUE arguments */
    while (i < argv_len) {
        const char *arg_start = &argv[i];
        int arg_len = 0;
        while (i < argv_len && argv[i] != ' ') { arg_len++; i++; }

        /* Find '=' in argument */
        int eq_pos = -1;
        for (int j = 0; j < arg_len; j++) {
            if (arg_start[j] == '=') { eq_pos = j; break; }
        }

        if (eq_pos > 0) {
            /* Set the variable */
            unsigned int key_off = alloc(arg_len + 1);
            copy_to_mem(key_off, arg_start, eq_pos);

            unsigned int val_off = alloc(arg_len - eq_pos);
            copy_to_mem(val_off, &arg_start[eq_pos + 1], arg_len - eq_pos - 1);

            host_env_set(key_off, eq_pos, val_off, arg_len - eq_pos - 1);
        }

        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* List all environment variables */
    unsigned int buf_off = alloc(4096);
    int total = host_env_list(buf_off, 4095);

    if (total > 0) {
        const char *data = (const char *)buf_off;
        int pos = 0;
        while (pos < total) {
            const char *entry = &data[pos];
            unsigned int len = my_strlen(entry);
            if (len > 0) {
                unsigned int off = alloc(len);
                copy_to_mem(off, entry, len);
                host_print(off, len);
                print_char('\n');
            }
            pos += len + 1;
            if (pos >= total) break;
        }
    }

    host_exit(0);
}
