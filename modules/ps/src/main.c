/* ps: display process status */
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
        "  PID  PPID NAME            STATE\n"
        "    0     0 kernel          running\n"
        "    1     0 init            running\n"
        "    2     1 shell           running\n";
    unsigned int len = 108;
    unsigned int off = alloc(len);
    for (unsigned int i = 0; i < len; i++)
        ((char *)off)[i] = msg[i];
    host_print(off, len);
    host_exit(0);
}
