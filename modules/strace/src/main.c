/* strace: system call tracer (simulated) */
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
    int prog_start = i;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != '\0') i++;
    if (prog_start >= argv_len) {
        print_str("usage: strace <command>\n");
        print_str("Trace system calls and signals.\n");
        host_exit(1);
    }
    print_str("execve(\"/bin/cmd\", [\"cmd\"], 0xfff8a0) = 0\n");
    print_str("brk(NULL)                               = 0x401000\n");
    print_str("mmap(NULL, 4096, PROT_READ|PROT_WRITE)  = 0x402000\n");
    print_str("open(\"/etc/passwd\", O_RDONLY)           = 3\n");
    print_str("read(3, \"root:x:0:0:...\", 1024)        = 512\n");
    print_str("close(3)                                = 0\n");
    print_str("write(1, \"hello world\", 11)             = 11\n");
    print_str("exit(0)                                 = ?\n");
    print_str("+++ exited with 0 +++\n");
    host_exit(0);
}
