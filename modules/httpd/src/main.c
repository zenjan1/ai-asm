/*
 * aiasm-aarch64/modules/httpd/src/main.c
 * Minimal HTTP/1.1 server WASM module.
 * Listens on port 80, serves static files from RAM disk.
 * Supports GET only, single-threaded.
 */

__attribute__((import_module("host"), import_name("net_listen")))
extern int wasm_host_net_listen(unsigned int port);

__attribute__((import_module("host"), import_name("net_accept")))
extern int wasm_host_net_accept(int sock);

__attribute__((import_module("host"), import_name("net_recv")))
extern int wasm_host_net_recv(int sock, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_send")))
extern int wasm_host_net_send(int sock, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("net_close")))
extern void wasm_host_net_close(int sock);

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

/* Buffer layout:
 * 65536 - 65791: 256 bytes for string operations (small buffers)
 * 66048 - 67071: 1024 bytes for HTTP request buffer
 * 67072 - 71167: 4096 bytes for file content
 * 71168 - 72191: 1024 bytes for HTTP response header
 */
#define BUF_SMALL   65536
#define BUF_REQ     66048
#define BUF_FILE    67072
#define BUF_RESP    71168

#define MAX_REQ_LEN 1024
#define MAX_FILE_LEN 4096
#define MAX_RESP_LEN 1024

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    char *dst = (char *)BUF_SMALL;
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(BUF_SMALL, len);
}

static void http_log(const char *msg)
{
    unsigned int ml = my_strlen(msg);
    char *buf = (char *)BUF_SMALL;
    buf[0] = 'I'; buf[1] = 'N'; buf[2] = 'F'; buf[3] = 'O';
    for (unsigned int i = 0; i < ml; i++) buf[5 + i] = msg[i];
    wasm_host_log(BUF_SMALL, 4, BUF_SMALL + 5, ml);
}

static void print_num(int v)
{
    if (v == 0) { print_str("0"); return; }
    int neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    char buf[12]; int i = 0;
    while (v > 0) { buf[i++] = (char)('0' + (v % 10)); v /= 10; }
    if (neg) { char *d = (char *)BUF_SMALL; *d = '-'; wasm_host_print(BUF_SMALL, 1); }
    while (i > 0) { char *d = (char *)BUF_SMALL; *d = buf[--i]; wasm_host_print(BUF_SMALL, 1); }
}

/* Copy string to buffer, return length */
static unsigned int buf_str(unsigned int dst, const char *s)
{
    unsigned int i = 0;
    while (s[i]) { ((char *)dst)[i] = s[i]; i++; }
    return i;
}

/* Find character in buffer, return index or -1 */
static int buf_find(unsigned int buf, unsigned int len, char c)
{
    for (unsigned int i = 0; i < len; i++) {
        if (((char *)buf)[i] == c) return (int)i;
    }
    return -1;
}

/* Compare buffer region with string: buf[pos..pos+len] == s? */
static int buf_cmp(unsigned int buf, unsigned int pos, unsigned int blen, const char *s)
{
    unsigned int i = 0;
    while (s[i]) {
        if (pos + i >= blen) return -1;
        if (((char *)buf)[pos + i] != s[i]) return -1;
        i++;
    }
    return (int)i; /* matched length */
}

/* Extract path from "GET /path HTTP/1.1" request line.
 * Returns pointer within buf_req where path starts, and sets *path_len.
 * Returns 0 if not a valid GET request.
 */
static unsigned int parse_request(unsigned int buf, int req_len, unsigned int *path_len)
{
    /* Check "GET " prefix */
    if (buf_cmp(buf, 0, (unsigned int)req_len, "GET ") != 4)
        return 0;

    /* Find end of path (space or ? or newline) */
    unsigned int path_start = 4;
    unsigned int i = 4;
    while (i < (unsigned int)req_len) {
        char c = ((char *)buf)[i];
        if (c == ' ' || c == '\r' || c == '\n' || c == '?')
            break;
        i++;
    }
    *path_len = i - path_start;
    return buf + path_start;
}

/* Send "HTTP/1.1 200 OK" response with content */
static void send_200(int sock, unsigned int content_off, int content_len)
{
    char *resp = (char *)BUF_RESP;
    unsigned int pos = 0;

    const char h1[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
    for (unsigned int i = 0; i < sizeof(h1) - 1; i++) resp[pos++] = h1[i];

    /* Convert content_len to string */
    if (content_len == 0) { resp[pos++] = '0'; }
    else {
        char tmp[12]; int ti = 0; int v = content_len;
        while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; }
        while (ti > 0) resp[pos++] = tmp[--ti];
    }

    const char h2[] = "\r\n\r\n";
    for (unsigned int i = 0; i < sizeof(h2) - 1; i++) resp[pos++] = h2[i];

    /* Send header */
    wasm_host_net_send(sock, BUF_RESP, pos);

    /* Send content */
    if (content_len > 0) {
        /* Send in chunks of 256 if needed */
        int remaining = content_len;
        unsigned int src = content_off;
        while (remaining > 0) {
            int chunk = remaining > 256 ? 256 : remaining;
            /* Copy chunk to BUF_SMALL */
            for (int j = 0; j < chunk; j++)
                ((char *)BUF_SMALL)[j] = ((char *)src)[j];
            wasm_host_net_send(sock, BUF_SMALL, (unsigned int)chunk);
            src += (unsigned int)chunk;
            remaining -= chunk;
        }
    }
}

/* Send "HTTP/1.1 404 Not Found" response */
static void send_404(int sock)
{
    const char resp[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nFile not found";
    unsigned int len = my_strlen(resp);
    for (unsigned int i = 0; i < len; i++)
        ((char *)BUF_SMALL)[i] = resp[i];
    wasm_host_net_send(sock, BUF_SMALL, len);
}

/* Send "HTTP/1.1 400 Bad Request" */
static void send_400(int sock)
{
    const char resp[] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nBad request";
    unsigned int len = my_strlen(resp);
    for (unsigned int i = 0; i < len; i++)
        ((char *)BUF_SMALL)[i] = resp[i];
    wasm_host_net_send(sock, BUF_SMALL, len);
}

/* Handle a single HTTP request */
static void handle_request(int sock, unsigned int req_buf, int req_len)
{
    unsigned int path_len = 0;
    unsigned int path_ptr = parse_request(req_buf, req_len, &path_len);

    if (path_ptr == 0) {
        send_400(sock);
        return;
    }

    /* Default to index.txt if path is "/" */
    if (path_len == 1 && ((char *)path_ptr)[0] == '/') {
        const char default_path[] = "/index.txt";
        path_ptr = BUF_SMALL;
        for (unsigned int i = 0; i < sizeof(default_path) - 1; i++)
            ((char *)path_ptr)[i] = default_path[i];
        path_len = my_strlen(default_path);
    }

    /* Null-terminate path for fs_open */
    if (path_len >= 128) path_len = 127;
    char *path_buf = (char *)BUF_SMALL;
    for (unsigned int i = 0; i < path_len; i++) path_buf[i] = ((char *)path_ptr)[i];
    path_buf[path_len] = '\0';

    /* Log the request */
    print_str("  GET ");
    for (unsigned int i = 0; i < path_len; i++)
        ((char *)BUF_SMALL)[i] = ((char *)path_ptr)[i];
    ((char *)BUF_SMALL)[path_len] = '\n';
    wasm_host_print(BUF_SMALL, path_len + 1);

    /* Open file */
    int fd = wasm_host_fs_open(BUF_SMALL, path_len);
    if (fd < 0) {
        send_404(sock);
        return;
    }

    /* Read file content */
    int file_len = wasm_host_fs_read(fd, BUF_FILE, MAX_FILE_LEN);
    wasm_host_fs_close(fd);

    if (file_len <= 0) {
        send_404(sock);
        return;
    }

    send_200(sock, BUF_FILE, file_len);
}

/* Read HTTP request from client socket */
static int read_request(int sock, unsigned int buf, unsigned int max_len)
{
    int total = 0;

    /* Try to read data */
    int n = wasm_host_net_recv(sock, buf, max_len);
    if (n > 0) {
        total += n;
    }
    return total;
}

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n== AI-ASM HTTP Server ==\n");
    print_str("  Listening on port 80...\n");
    http_log("httpd started on port 80");

    /* Start listening */
    int listen_sock = wasm_host_net_listen(80);
    if (listen_sock < 0) {
        print_str("  ERROR: listen failed (");
        print_num(listen_sock);
        print_str(")\n");
        http_log("listen failed");
        return;
    }

    print_str("  Socket fd=");
    print_num(listen_sock);
    print_str("\n");

    for (;;) {
        /* Accept connection */
        int client = wasm_host_net_accept(listen_sock);
        if (client < 0) {
            wasm_host_yield();
            wasm_host_sleep(50);
            continue;
        }

        /* Read HTTP request */
        int req_len = read_request(client, BUF_REQ, MAX_REQ_LEN);
        if (req_len > 0) {
            /* Null-terminate for safety */
            ((char *)BUF_REQ)[req_len < MAX_REQ_LEN ? req_len : MAX_REQ_LEN - 1] = '\0';
            handle_request(client, BUF_REQ, req_len);
        }

        /* Close client connection */
        wasm_host_net_close(client);
    }
}
