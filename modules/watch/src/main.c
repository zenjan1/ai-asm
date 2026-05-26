/* watch: periodic execution (simulated) */
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
        print_str("usage: watch [-n <seconds>] <command>\n");
        host_exit(1);
    }

    unsigned int interval = 2;

    /* Check for -n flag */
    if (argv[i] == '-' && argv[i + 1] == 'n') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        /* Parse interval */
        interval = 0;
        while (i < argv_len && argv[i] >= '0' && argv[i] <= '9') {
            interval = interval * 10 + (argv[i] - '0');
            i++;
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    const char *cmd = &argv[i];
    unsigned int cmd_len = 0;
    while (i < argv_len && argv[i] != '\n') { cmd_len++; i++; }

    print_str("Every ");
    /* Print interval */
    char buf[8];
    int bi = 7;
    buf[7] = 0;
    unsigned int tmp = interval;
    if (tmp == 0) { buf[--bi] = '0'; } else {
        while (tmp > 0) { buf[--bi] = '0' + (tmp % 10); tmp /= 10; }
    }
    unsigned int blen = 7 - bi;
    unsigned int off = alloc(blen);
    copy_to_mem(off, &buf[bi], blen);
    host_print(off, blen);
    print_str("s: ");
    off = alloc(cmd_len);
    copy_to_mem(off, cmd, cmd_len);
    host_print(off, cmd_len);
    print_str("\n");
    print_str("=====================================\n");
    print_str("[output simulated - watch mode]\n");
    print_str("=====================================\n");

    host_exit(0);
}
