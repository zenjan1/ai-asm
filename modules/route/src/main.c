/* route: display/manipulate IP routing table */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

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
    const char msg[] =
        "Kernel IP routing table\n"
        "Destination     Gateway         Genmask         Flags Metric Ref Use Iface\n"
        "0.0.0.0         192.168.1.1     0.0.0.0         UG    0      0   0   eth0\n"
        "192.168.1.0     0.0.0.0         255.255.255.0   U     0      0   0   eth0\n"
        "127.0.0.0       0.0.0.0         255.0.0.0       U     0      0   0   lo\n";

    unsigned int off = alloc(sizeof(msg));
    copy_to_mem(off, msg, sizeof(msg));
    host_print(off, sizeof(msg) - 1);
    host_exit(0);
}
