/* hashsum: hash computation (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

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

static void print_hex(unsigned char *buf, unsigned int len)
{
    char hex[2];
    for (unsigned int i = 0; i < len; i++) {
        hex[0] = "0123456789abcdef"[buf[i] >> 4];
        hex[1] = "0123456789abcdef"[buf[i] & 0xf];
        unsigned int off = alloc(2);
        copy_to_mem(off, hex, 2);
        host_print(off, 2);
    }
}

/* Simple hash simulation - produces deterministic output from filename */
static void compute_hash(const char *name, unsigned int len, unsigned char *out, unsigned int outlen)
{
    unsigned int h = 5381;
    for (unsigned int i = 0; i < len; i++) {
        h = ((h << 5) + h) ^ name[i];
    }
    for (unsigned int i = 0; i < outlen; i++) {
        out[i] = (h >> (i % 4) * 8) & 0xff;
        h = (h * 1103515245 + 12345) & 0x7fffffff;
    }
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
        print_str("usage: hashsum <file>\n");
        host_exit(1);
    }

    const char *file = &argv[i];
    unsigned int flen = 0;
    while (file[flen] && file[flen] != ' ' && file[flen] != '\n') flen++;

    unsigned char hash[32];
    compute_hash(file, flen, hash, 32);

    print_hex(hash, 32);
    print_str("  ");
    unsigned int off = alloc(flen);
    copy_to_mem(off, file, flen);
    host_print(off, flen);
    print_str("\n");

    host_exit(0);
}
