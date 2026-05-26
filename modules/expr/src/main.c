/* expr: expression evaluation (simulated) */
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

static int parse_int(const char *s, unsigned int len)
{
    int neg = 0;
    unsigned int i = 0;
    if (s[0] == '-') { neg = 1; i++; }
    int n = 0;
    while (i < len && s[i] >= '0' && s[i] <= '9') {
        n = n * 10 + (s[i] - '0');
        i++;
    }
    return neg ? -n : n;
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
        print_str("usage: expr <a> <op> <b>\n");
        host_exit(1);
    }

    /* Parse first operand */
    const char *a_start = &argv[i];
    unsigned int a_len = 0;
    while (i < argv_len && argv[i] != ' ') { a_len++; i++; }
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse operator */
    const char *op_start = &argv[i];
    unsigned int op_len = 0;
    while (i < argv_len && argv[i] != ' ') { op_len++; i++; }
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse second operand */
    const char *b_start = &argv[i];
    unsigned int b_len = 0;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) { b_len++; i++; }

    int a = parse_int(a_start, a_len);
    int b = parse_int(b_start, b_len);
    int result = 0;

    /* Simple operator dispatch */
    char op = op_len > 0 ? op_start[0] : '?';
    if (op == '+') result = a + b;
    else if (op == '-') result = a - b;
    else if (op == '*' ) result = a * b;
    else if (op == '/') result = b != 0 ? a / b : 0;
    else if (op == '%') result = b != 0 ? a % b : 0;
    else if (op == '=' && a_len > 0 && b_len > 0) result = (a == b) ? 1 : 0;
    else { print_str("expr: unknown operator\n"); host_exit(1); }

    print_number(result);
    print_str("\n");

    host_exit(0);
}
