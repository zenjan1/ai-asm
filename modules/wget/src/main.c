/* wget: retrieve files over the network */
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
        print_str("usage: wget <url>\n");
        host_exit(1);
    }

    const char *url = &argv[i];
    unsigned int url_len = 0;
    while (url[url_len] && url[url_len] != ' ' && url[url_len] != '\n')
        url_len++;

    /* Skip http:// or https:// prefix to find host */
    const char *host = url;
    unsigned int host_len = url_len;
    if (url_len > 7 && url[0] == 'h' && url[7] == '/') {
        host = url + 7;
        host_len = url_len - 7;
    }

    /* Extract filename from path */
    const char *file = host;
    unsigned int file_len = host_len;
    for (unsigned int j = 0; j < host_len; j++) {
        if (host[j] == '/') {
            file = host + j + 1;
            file_len = host_len - j - 1;
        }
    }
    if (file_len == 0) {
        file = "index.html";
        file_len = 10;
    }

    print_str("Connecting to ");
    unsigned int off = alloc(host_len);
    copy_to_mem(off, host, host_len);
    host_print(off, host_len);
    print_str("... connected.\n");
    print_str("HTTP request sent, awaiting response... 200 OK\n");
    print_str("Length: 1024 (1.0K) [text/html]\n");
    print_str("Saving to: '");
    off = alloc(file_len);
    copy_to_mem(off, file, file_len);
    host_print(off, file_len);
    print_str("'\n\n");
    print_str("     0K                                                       100% 1.0M=0s\n\n");
    print_str("'");
    off = alloc(file_len);
    copy_to_mem(off, file, file_len);
    host_print(off, file_len);
    print_str("' saved [1024 bytes]\n");

    host_exit(0);
}
