/*
 * aiasm-aarch64/modules/yes/src/main.c
 * yes WASM module for AI-ASM AArch64 v40.0
 *
 * Usage: yes [STRING]  (default: "y")
 * Outputs STRING followed by newline, forever
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void host_yield(void);

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

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    const char *msg = "y";

    /* Get command line arguments */
    char buf[256];
    int argv_len = host_get_argv((unsigned int)buf, 255);
    if (argv_len > 0) {
        /* Skip argv[0] ("yes") */
        int i = 0;
        while (i < argv_len && buf[i] != ' ') i++;
        while (i < argv_len && buf[i] == ' ') i++;
        if (i < argv_len) {
            msg = &buf[i];
            /* Truncate at next space */
            int j = i;
            while (j < argv_len && buf[j] != ' ') j++;
            /* Copy to null-terminated buffer */
            static char msg_buf[256];
            int len = j - i;
            if (len > 255) len = 255;
            for (int k = 0; k < len; k++) msg_buf[k] = buf[i + k];
            msg_buf[len] = '\0';
            msg = msg_buf;
        }
    }

    unsigned int msg_len = my_strlen(msg);
    unsigned int total_len = msg_len + 1; /* +1 for newline */

    /* Allocate output buffer once */
    unsigned int off = alloc(total_len);
    copy_to_mem(off, msg, msg_len);
    char *p = (char *)(off + msg_len);
    p[0] = '\n';

    /* Infinite loop: print + yield */
    while (1) {
        host_print(off, total_len);
        host_yield();
    }

    host_exit(0);
}
