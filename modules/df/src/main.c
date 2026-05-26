/* df: display filesystem disk space usage */
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

__attribute__((export_name("_start")))
void _start(void)
{
    const char *msg =
        "Filesystem     1K-blocks    Used  Available  Use%  Mounted on\n"
        "/dev/root         131072    6930     124142    5%  /\n";
    unsigned int len = 137;
    unsigned int off = alloc(len);
    for (unsigned int i = 0; i < len; i++)
        ((char *)off)[i] = msg[i];
    host_print(off, len);
    host_exit(0);
}
