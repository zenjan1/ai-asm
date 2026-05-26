/* test: condition evaluation (simulated) */
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
        /* No arguments: false */
        host_exit(1);
    }

    const char *arg1 = &argv[i];
    unsigned int arg1_len = 0;
    while (i < argv_len && argv[i] != ' ') { arg1_len++; i++; }
    while (i < argv_len && argv[i] == ' ') i++;

    /* Single argument: test if non-empty */
    if (i >= argv_len || argv[i] == '\n' || argv[i] == 0) {
        host_exit(arg1_len > 0 ? 0 : 1);
    }

    /* Two arguments: check for ! negation or -z/-n tests */
    const char *arg2 = &argv[i];
    unsigned int arg2_len = 0;
    while (i < argv_len && argv[i] != ' ' && argv[i] != '\n' && argv[i] != 0) { arg2_len++; i++; }

    int result = 0;
    if (arg1_len == 1 && arg1[0] == '!') {
        result = (arg2_len == 0) ? 0 : 1;
    } else if (arg1_len == 2 && arg1[0] == '-' && arg1[1] == 'z') {
        result = (arg2_len == 0) ? 0 : 1;
    } else if (arg1_len == 2 && arg1[0] == '-' && arg1[1] == 'n') {
        result = (arg2_len > 0) ? 0 : 1;
    } else if (arg1_len == 2 && arg1[0] == '-' && arg1[1] == 'f') {
        /* File exists test (simulated: always true) */
        result = 0;
    } else if (arg1_len == 2 && arg1[0] == '-' && arg1[1] == 'd') {
        /* Directory exists test (simulated: always false) */
        result = 1;
    } else {
        /* String comparison or unknown */
        result = 0;
    }

    host_exit(result);
}
