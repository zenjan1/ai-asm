/* shred: secure file deletion (simulated) */
__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

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

static void print_number(unsigned int n)
{
    if (n == 0) { print_str("0"); return; }
    char buf[12];
    int i = 11;
    buf[11] = 0;
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    unsigned int len = 11 - i;
    unsigned int off = alloc(len);
    copy_to_mem(off, &buf[i], len);
    host_print(off, len);
}

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i >= argv_len) {
        print_str("usage: shred [-n passes] <file>\n");
        host_exit(1);
    }

    unsigned int passes = 3;

    /* Check for -n flag */
    if (argv[i] == '-' && argv[i + 1] == 'n') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        passes = 0;
        while (i < argv_len && argv[i] >= '0' && argv[i] <= '9') {
            passes = passes * 10 + (argv[i] - '0');
            i++;
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    const char *file = &argv[i];
    unsigned int flen = 0;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) { flen++; i++; }

    unsigned int off = alloc(flen);
    copy_to_mem(off, file, flen);
    host_print(off, flen);
    print_str(": pass ");
    print_number(passes);
    print_str("/");
    print_number(passes);
    print_str("/");
    print_number(passes);
    print_str(" (random/0xff/remove)\n");

    host_exit(0);
}
