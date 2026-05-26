/* cal: display a calendar */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

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

__attribute__((export_name("_start")))
void _start(void)
{
    const char cal[] =
        "    May 2026\n"
        "Su Mo Tu We Th Fr Sa\n"
        "                1  2\n"
        " 3  4  5  6  7  8  9\n"
        "10 11 12 13 14 15 16\n"
        "17 18 19 20 21 22 23\n"
        "24 25 26 27 28 29 30\n"
        "31\n";

    unsigned int off = alloc(sizeof(cal));
    copy_to_mem(off, cal, sizeof(cal));
    host_print(off, sizeof(cal) - 1);
    host_exit(0);
}
