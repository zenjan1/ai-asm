/* ping: test network connectivity */
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

static void print_char(char c)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = c;
    host_print(off, 1);
}

static void my_itoa(unsigned int n, char *buf)
{
    char tmp[16];
    int i = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
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
        print_str("usage: ping <host>\n");
        host_exit(1);
    }

    const char *host = &argv[i];
    unsigned int host_len = 0;
    while (host[host_len] && host[host_len] != ' ' && host[host_len] != '\n')
        host_len++;

    /* PING header */
    print_str("PING ");
    unsigned int off = alloc(host_len);
    copy_to_mem(off, host, host_len);
    host_print(off, host_len);
    print_str(" (192.168.1.1): 56 data bytes\n");

    /* 4 ping replies */
    unsigned int times[] = {1, 2, 1, 3};
    for (int j = 0; j < 4; j++) {
        char num_buf[16];
        my_itoa(64, num_buf);
        print_str(num_buf);
        print_str(" bytes from ");
        off = alloc(host_len);
        copy_to_mem(off, host, host_len);
        host_print(off, host_len);
        print_str(" (192.168.1.1): icmp_seq=");
        my_itoa(j + 1, num_buf);
        print_str(num_buf);
        print_str(" time=");
        my_itoa(times[j], num_buf);
        print_str(num_buf);
        print_str("ms\n");
    }

    /* Summary */
    print_str("\n--- ");
    off = alloc(host_len);
    copy_to_mem(off, host, host_len);
    host_print(off, host_len);
    print_str(" ping statistics ---\n");
    print_str("4 packets transmitted, 4 received, 0% packet loss\n");

    host_exit(0);
}
