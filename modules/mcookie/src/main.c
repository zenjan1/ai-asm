/* mcookie: random cookie generation (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

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

static void print_str(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

static void print_hex_byte(unsigned char b)
{
    char buf[2];
    buf[0] = "0123456789abcdef"[b >> 4];
    buf[1] = "0123456789abcdef"[b & 0xf];
    unsigned int off = alloc(2);
    copy_to_mem(off, buf, 2);
    host_print(off, 2);
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int tick = host_get_tick();
    unsigned int seed = tick ^ 0x5bd1e995;

    for (int i = 0; i < 16; i++) {
        seed = seed * 1103515245 + 12345;
        unsigned char byte = (seed >> 16) & 0xff;
        print_hex_byte(byte);
    }
    print_str("\n");

    host_exit(0);
}
