/* mktemp: temp file creation (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned int host_get_tick(void);

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

static void print_hex(unsigned int v)
{
    char buf[9];
    buf[8] = 0;
    for (int i = 7; i >= 0; i--) {
        unsigned char d = v & 0xf;
        buf[i] = d < 10 ? '0' + d : 'a' + (d - 10);
        v >>= 4;
    }
    unsigned int off = alloc(8);
    copy_to_mem(off, buf, 8);
    host_print(off, 8);
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

    const char *template = "/tmp/tmp.XXXXXXXXXX";
    unsigned int tlen = my_strlen(template);

    if (i < argv_len && argv[i] != ' ' && argv[i] != '\n') {
        const char *t = &argv[i];
        tlen = 0;
        while (t[tlen] && t[tlen] != ' ' && t[tlen] != '\n') tlen++;
        unsigned int tmp = alloc(tlen + 1);
        copy_to_mem(tmp, t, tlen);
        ((char *)tmp)[tlen] = 0;
        template = (const char *)tmp;
    }

    /* Find X's and replace with hex from tick */
    unsigned int ws = alloc(tlen + 1);
    copy_to_mem(ws, template, tlen);
    ((char *)ws)[tlen] = 0;

    unsigned int tick = host_get_tick();
    for (unsigned int j = 0; j < tlen; j++) {
        char *p = (char *)ws;
        if (p[j] == 'X') {
            unsigned char d = tick & 0xf;
            p[j] = d < 10 ? '0' + d : 'a' + (d - 10);
            tick >>= 4;
        }
    }

    print_str((const char *)ws);
    print_str("\n");

    host_exit(0);
}
