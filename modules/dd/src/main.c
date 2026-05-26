/* dd: data conversion and copy (simulated) */
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

    const char *if_val = "";
    const char *of_val = "";
    unsigned int bs_val = 512;

    /* Parse key=value pairs */
    while (i < argv_len) {
        const char *kv = &argv[i];
        unsigned int kv_len = 0;
        while (i < argv_len && argv[i] != ' ' && argv[i] != '\n') { kv_len++; i++; }

        if (kv_len >= 3 && kv[0] == 'i' && kv[1] == 'f' && kv[2] == '=') {
            if_val = &kv[3];
        } else if (kv_len >= 3 && kv[0] == 'o' && kv[1] == 'f' && kv[2] == '=') {
            of_val = &kv[3];
        } else if (kv_len >= 3 && kv[0] == 'b' && kv[1] == 's' && kv[2] == '=') {
            bs_val = 0;
            const char *p = &kv[3];
            unsigned int blen = kv_len - 3;
            for (unsigned int j = 0; j < blen; j++) {
                if (p[j] >= '0' && p[j] <= '9')
                    bs_val = bs_val * 10 + (p[j] - '0');
            }
        }
        while (i < argv_len && argv[i] == ' ') i++;
    }

    print_str("dd: ");
    if (my_strlen(if_val) > 0) {
        print_str("if=");
        unsigned int off = alloc(my_strlen(if_val));
        copy_to_mem(off, if_val, my_strlen(if_val));
        host_print(off, my_strlen(if_val));
    }
    if (my_strlen(of_val) > 0) {
        print_str(" of=");
        unsigned int off = alloc(my_strlen(of_val));
        copy_to_mem(off, of_val, my_strlen(of_val));
        host_print(off, my_strlen(of_val));
    }
    print_str(" bs=");
    print_number(bs_val);
    print_str("\n0+1 records in\n0+1 records out\n");

    host_exit(0);
}
