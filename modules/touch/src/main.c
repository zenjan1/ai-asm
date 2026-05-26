/*
 * aiasm-aarch64/modules/touch/src/main.c
 * touch WASM module for AI-ASM AArch64 v54.0
 *
 * Usage: touch FILE [FILE ...]
 * Creates empty files in ramdisk.
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int wasm_host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("fs_create")))
extern int host_fs_create(unsigned int name_off, unsigned int name_len);

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

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(512);
    int argv_len = wasm_host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("Usage: touch FILE [FILE ...]\n");
        host_exit(1);
    }

    while (i < argv_len) {
        const char *filename = &argv[i];
        int filename_len = 0;
        while (i < argv_len && argv[i] != ' ') { i++; filename_len++; }

        if (filename_len > 0) {
            unsigned int foff = alloc((unsigned int)filename_len + 1);
            copy_to_mem(foff, filename, (unsigned int)filename_len);
            ((char *)foff)[filename_len] = '\0';

            host_fs_create(foff, (unsigned int)filename_len);
        }

        while (i < argv_len && argv[i] == ' ') i++;
        wasm_host_yield();
    }

    host_exit(0);
}
