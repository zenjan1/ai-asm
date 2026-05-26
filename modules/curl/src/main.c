/* curl: transfer data from/to a server */
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
        print_str("usage: curl <url>\n");
        host_exit(1);
    }

    const char *url = &argv[i];
    unsigned int url_len = 0;
    while (url[url_len] && url[url_len] != ' ' && url[url_len] != '\n')
        url_len++;

    /* Skip protocol to find host */
    const char *host = url;
    unsigned int host_len = url_len;
    if (url_len > 7 && url[0] == 'h' && url[7] == '/') {
        host = url + 7;
        host_len = url_len - 7;
    } else if (url_len > 8 && url[0] == 'h' && url[8] == '/') {
        host = url + 8;
        host_len = url_len - 8;
    }

    print_str("  % Total    % Received % Xferd  Average Speed   Time\n");
    print_str("100  1024  100  1024    0     0   102400      0 --:--:-- --:--:-- --:--:--  100k\n");

    /* Simulated HTML response */
    print_str("<!DOCTYPE html>\n");
    print_str("<html>\n");
    print_str("<head><title>Welcome</title></head>\n");
    print_str("<body>\n");
    print_str("<h1>It works!</h1>\n");
    print_str("</body>\n");
    print_str("</html>\n");

    host_exit(0);
}
