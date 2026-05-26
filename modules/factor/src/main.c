/* factor: prime factorization (simulated) */
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

static unsigned int parse_number(const char *s)
{
    unsigned int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n;
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

static void factorize(unsigned int n)
{
    print_str("factor: ");
    print_number(n);
    print_str(" =");

    if (n <= 1) {
        print_str(" ");
        print_number(n);
        print_str("\n");
        return;
    }

    unsigned int d = 2;
    int first = 1;
    while (d * d <= n) {
        while (n % d == 0) {
            if (!first) print_str(" *");
            print_str(" ");
            print_number(d);
            first = 0;
            n /= d;
        }
        d++;
    }
    if (n > 1) {
        if (!first) print_str(" *");
        print_str(" ");
        print_number(n);
    }
    print_str("\n");
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
        print_str("usage: factor <number>\n");
        host_exit(1);
    }

    unsigned int n = parse_number(&argv[i]);
    factorize(n);

    host_exit(0);
}
