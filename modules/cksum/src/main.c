/*
 * aiasm-aarch64/modules/cksum/src/main.c
 * cksum WASM module for AI-ASM AArch64 v52.0
 *
 * Usage: cksum [FILE]
 * Computes CRC32 checksum and byte count.
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

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

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

static void print_uint(unsigned int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) buf[i++] = '0';
    else while (v > 0) { buf[i++] = (char)('0' + (v % 10)); v /= 10; }
    unsigned int off = alloc((unsigned int)i);
    char *dst = (char *)off;
    for (int j = 0; j < i; j++) dst[j] = buf[i - 1 - j];
    host_print(off, (unsigned int)i);
}

/* CRC32 lookup table */
static unsigned int crc32_table[256];

static void crc32_init_table(void)
{
    for (unsigned int i = 0; i < 256; i++) {
        unsigned int crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

static unsigned int crc32_update(unsigned int crc, const unsigned char *data, unsigned int len)
{
    for (unsigned int i = 0; i < len; i++) {
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}

__attribute__((export_name("_start")))
void _start(void)
{
    crc32_init_table();

    unsigned int argv_off = alloc(512);
    int argv_len = wasm_host_get_argv(argv_off, 511);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Check if filename provided */
    const char *filename = 0;
    int filename_len = 0;
    if (i < argv_len) {
        filename = &argv[i];
        while (i < argv_len && argv[i] != ' ') { i++; filename_len++; }
    }

    unsigned int crc = 0xFFFFFFFF;
    unsigned int byte_count = 0;
    unsigned int buf_off = alloc(256);

    if (filename && filename_len > 0) {
        /* Read from file */
        unsigned int foff = alloc((unsigned int)filename_len + 1);
        copy_to_mem(foff, filename, (unsigned int)filename_len);
        ((char *)foff)[filename_len] = '\0';

        unsigned int fd_off = alloc(4);
        if (host_fs_open((int)fd_off, foff) != 0) {
            print_str("cksum: cannot open file\n");
            host_exit(1);
        }
        int fd = *(int *)fd_off;

        for (;;) {
            int n = host_fs_read(fd, buf_off, 256);
            if (n <= 0) break;
            crc = crc32_update(crc, (const unsigned char *)buf_off, (unsigned int)n);
            byte_count += (unsigned int)n;
            wasm_host_yield();
        }
        host_fs_close(fd);
    } else {
        /* Read from stdin */
        for (;;) {
            int c = wasm_host_getc();
            if (c == -1) break;
            if (c == 0) { wasm_host_yield(); continue; }
            unsigned char ch = (unsigned char)c;
            crc = crc32_update(crc, &ch, 1);
            byte_count++;
            wasm_host_yield();
        }
    }

    crc ^= 0xFFFFFFFF;

    print_uint(crc);
    print_str(" ");
    print_uint(byte_count);
    if (filename && filename_len > 0) {
        print_str(" ");
        host_print((unsigned int)filename, (unsigned int)filename_len);
    }
    print_str("\n");

    host_exit(0);
}
