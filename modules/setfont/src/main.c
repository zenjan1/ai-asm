/* setfont: console font loader (simulated) */
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
            print_str("Usage: setfont [ font_file ]\n");
            print_str("Load console font.\n");
            print_str("\nOptions:\n");
            print_str("  -h  show this help\n");
            host_exit(1);
        }
    }

    print_str("setfont: console font loading simulation\n\n");
    print_str("Loading font: lat0-16.psfu.gz\n");
    print_str("  Font format: PSFv2 (unicode)\n");
    print_str("  Glyph size: 8x16\n");
    print_str("  Characters: 512\n");
    print_str("  Glyph data: 65536 bytes\n");
    print_str("  Height: 16\n");
    print_str("  Width: 8\n");
    print_str("  Unicode: yes\n");
    print_str("\nSetting framebuffer console font...\n");
    print_str("  Font: lat0-16\n");
    print_str("  Point size: 16\n");
    print_str("  Columns: 128\n");
    print_str("  Rows: 48\n");
    print_str("  Screen: 1024x768\n");
    print_str("\nFont loaded successfully\n");
    print_str("Status: FONT = lat0-16 (8x16 unicode)\n");

    host_exit(0);
}
