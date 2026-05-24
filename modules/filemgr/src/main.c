/*
 * aiasm-aarch64/modules/filemgr/src/main.c
 * File manager — lists files on RAM disk, displays via UART.
 * Simple terminal-based file browser.
 */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("fs_list")))
extern int wasm_host_fs_list(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)65536;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(65536, len);
}

static void list_files(void)
{
    char *buf = (char *)66000;
    int total = wasm_host_fs_list(66000, 8192);
    if (total <= 0) {
        print_str("  (no files found)\n");
        return;
    }

    int i = 0;
    int n = 0;
    while (i < total && buf[i] != '\0') {
        n++;
        print_str("  ");
        /* print number */
        {
            int v = n; char tmp[8]; int ti = 0;
            if (v == 0) tmp[ti++] = '0';
            else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
            char *d = (char *)65536;
            for (int j = ti - 1; j >= 0; j--) { *d = tmp[j]; wasm_host_print(65536, 1); }
        }
        print_str(". ");

        /* print filename */
        while (i < total && buf[i] != '\0') {
            char *d = (char *)65536; *d = buf[i]; wasm_host_print(65536, 1);
            i++;
        }
        print_str("\n");
        i++; /* skip null separator */
    }
}

static void view_file(const char *name)
{
    unsigned int nlen = my_strlen(name);
    char *path = (char *)65536;
    for (unsigned int i = 0; i < nlen; i++) path[i] = name[i];

    int fd = wasm_host_fs_open(65536, nlen);
    if (fd < 0) {
        print_str("  failed to open file\n");
        return;
    }

    print_str("--- file contents ---\n");
    char *buf = (char *)66000;
    for (;;) {
        int n = wasm_host_fs_read(fd, 66000, 512);
        if (n <= 0) break;
        wasm_host_print(66000, (unsigned int)n);
    }
    print_str("\n--- end ---\n");

    wasm_host_fs_close(fd);
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n=== File Manager ===\n");
    print_str("Files on RAM disk:\n");
    list_files();
    print_str("\nUsage: type filename to view, 'l' to list, 'q' to quit\n");

    for (;;) {
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;
            if (c == 'q' || c == 'Q') {
                print_str("Exiting file manager.\n");
                break;
            } else if (c == 'l' || c == 'L') {
                print_str("\nFiles:\n");
                list_files();
                print_str("\n");
            } else {
                /* treat as filename prefix — try to open */
                char name[64]; int ni = 0;
                name[ni++] = c;
                /* read rest of filename until newline */
                for (int j = 0; j < 60; j++) {
                    int ch2 = wasm_host_getc();
                    if (ch2 > 0 && ch2 != '\n' && ch2 != '\r')
                        name[ni++] = (char)ch2;
                    else if (ch2 == '\n' || ch2 == '\r')
                        break;
                    else {
                        wasm_host_yield();
                        wasm_host_sleep(50);
                    }
                }
                name[ni] = '\0';
                print_str("Viewing: ");
                print_str(name);
                print_str("\n");
                view_file(name);
            }
        }
        wasm_host_yield();
        wasm_host_sleep(50);
    }
}
