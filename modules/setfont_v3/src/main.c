/* setfont_v3: console font setting simulation (enhanced v3) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;
static unsigned int alloc(unsigned int n) {
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) {
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}
static unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}
static void print_str(const char *s) {
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    if (argv_len > 0) {
        const char *argv = (const char *)argv_off;
        int i = 0;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
        if (i < argv_len && argv[i] == '-' && i + 1 < argv_len && argv[i + 1] == 'h') {
            print_str("Usage: setfont_v3 [ FONT ] [ -h SIZE ]\n");
            print_str("Load console font.\n");
            print_str("\nArguments:\n");
            print_str("  FONT  font file to load (default: default8x16)\n");
            print_str("\nOptions:\n");
            print_str("  -h  show this help\n");
            print_str("  -h SIZE  set font height (8/14/16)\n");
            host_exit(1);
        }
    }

    print_str("setfont_v3: console font setting (enhanced v3)\n\n");
    print_str("Loading font: default8x16.psfu\n");
    print_str("  Format: PSFv2\n");
    print_str("  Width: 8 pixels\n");
    print_str("  Height: 16 pixels\n");
    print_str("  Glyphs: 256\n");
    print_str("  Mode: unicode\n");
    print_str("  Size: 4096 bytes\n");
    print_str("  Load time: 12 ms\n");
    print_str("\nFont table:\n");
    print_str("  0x00-0x1F: control characters\n");
    print_str("  0x20-0x7F: ASCII printable\n");
    print_str("  0x80-0xFF: extended Latin-1\n\n");
    print_str("Status: font loaded\n");
    host_exit(0);
}
