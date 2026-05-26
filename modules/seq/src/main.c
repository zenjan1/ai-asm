/* seq: number sequence generation */
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

static int parse_int(const char *s, int *out)
{
    int neg = 0;
    if (*s == '-') { neg = 1; s++; }
    if (*s == '+') s++;
    int n = 0;
    int has_digit = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
        has_digit = 1;
    }
    if (!has_digit) return -1;
    *out = neg ? -n : n;
    return 0;
}

static void print_number(int n)
{
    if (n == 0) { print_str("0"); return; }
    char buf[16];
    int i = 15;
    buf[15] = 0;
    int neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    if (neg) buf[--i] = '-';
    unsigned int len = 15 - i;
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
        print_str("usage: seq <last>\n");
        print_str("       seq <first> <last>\n");
        host_exit(1);
    }

    const char *p = &argv[i];
    unsigned int len1 = 0;
    while (p[len1] && p[len1] != ' ' && p[len1] != '\n') len1++;

    int first = 1, last = 0;
    unsigned int tmp = alloc(len1 + 1);
    copy_to_mem(tmp, p, len1);
    ((char *)tmp)[len1] = 0;

    i += len1;
    while (i < argv_len && argv[i] == ' ') i++;

    if (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) {
        /* Two arguments: first last */
        parse_int((const char *)tmp, &first);
        const char *q = &argv[i];
        unsigned int len2 = 0;
        while (q[len2] && q[len2] != ' ' && q[len2] != '\n') len2++;
        tmp = alloc(len2 + 1);
        copy_to_mem(tmp, q, len2);
        ((char *)tmp)[len2] = 0;
        parse_int((const char *)tmp, &last);
    } else {
        /* One argument: last */
        parse_int((const char *)tmp, &last);
    }

    int step = (first <= last) ? 1 : -1;
    for (int n = first; (step > 0 && n <= last) || (step < 0 && n >= last); n += step) {
        print_number(n);
        print_str("\n");
    }

    host_exit(0);
}
