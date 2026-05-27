/* tar: archive management simulation (simulated) */
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
    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i >= argv_len) {
        print_str("Usage: tar [OPTION...] [FILE]...\n");
        print_str("GNU tar -- archiver program.\n\n");
        print_str("Options:\n");
        print_str("  -c    create a new archive\n");
        print_str("  -x    extract files from archive\n");
        print_str("  -t    list archive contents\n");
        print_str("  -f    use archive file\n");
        print_str("  -v    verbosely list files\n");
        print_str("  -h    show this help\n");
        host_exit(1);
    }
    int has_c = 0, has_x = 0, has_t = 0, has_v = 0;
    int j = i;
    while (j < argv_len) {
        if (argv[j] == '-') {
            j++;
            while (j < argv_len && argv[j] != ' ') {
                if (argv[j] == 'c') has_c = 1;
                if (argv[j] == 'x') has_x = 1;
                if (argv[j] == 't') has_t = 1;
                if (argv[j] == 'v') has_v = 1;
                if (argv[j] == 'h') {
                    print_str("Usage: tar [OPTION...] [FILE]...\n");
                    print_str("GNU tar -- archiver program.\n\n");
                    print_str("Options:\n");
                    print_str("  -c    create a new archive\n");
                    print_str("  -x    extract files from archive\n");
                    print_str("  -t    list archive contents\n");
                    print_str("  -f    use archive file\n");
                    print_str("  -v    verbosely list files\n");
                    host_exit(0);
                }
                j++;
            }
        }
        j++;
    }
    if (has_c) {
        print_str("Creating archive archive.tar...\n");
        if (has_v) {
            print_str("a file1.txt\n");
            print_str("a file2.txt\n");
            print_str("a dir/\n");
        }
        print_str("Archive created successfully.\n");
    } else if (has_x) {
        print_str("Extracting archive...\n");
        if (has_v) {
            print_str("x file1.txt\n");
            print_str("x file2.txt\n");
            print_str("x dir/\n");
        }
        print_str("Extraction complete.\n");
    } else if (has_t) {
        print_str("Archive contents:\n");
        print_str("-rw-r--r-- 0/0    1234 2026-05-29 04:30 file1.txt\n");
        print_str("-rw-r--r-- 0/0    5678 2026-05-29 04:30 file2.txt\n");
        print_str("drwxr-xr-x 0/0       0 2026-05-29 04:30 dir/\n");
    } else {
        print_str("tar: You must specify one of -c, -x, or -t\n");
        host_exit(1);
    }
    host_exit(0);
}
