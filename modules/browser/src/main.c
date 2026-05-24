/*
 * aiasm-aarch64/modules/browser/src/main.c
 * Simple HTTP browser for AI-ASM v3.0
 *
 * HTTP GET requests with plain-text HTML rendering.
 * Uses keyboard for URL input and navigation.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports - core                                                        */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

/* -------------------------------------------------------------------------- */
/* WASM imports - network                                                     */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("net_connect")))
extern int wasm_net_connect(unsigned int ip, unsigned int port, unsigned int proto);

__attribute__((import_module("host"), import_name("net_send")))
extern int wasm_net_send(int sock, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("net_recv")))
extern int wasm_net_recv(int sock, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("net_close")))
extern void wasm_net_close(int sock);

/* -------------------------------------------------------------------------- */
/* WASM imports - GUI                                                         */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("gui_create")))
extern int wasm_host_gui_create(unsigned int x, unsigned int y, unsigned int w,
                                unsigned int h, unsigned int title_off);

__attribute__((import_module("host"), import_name("gui_draw")))
extern int wasm_host_gui_draw(int win_id, unsigned int buf_off,
                              unsigned int x, unsigned int y,
                              unsigned int w, unsigned int h);

__attribute__((import_module("host"), import_name("gui_flush")))
extern void wasm_host_gui_flush(unsigned int win_id);

__attribute__((import_module("host"), import_name("gui_poll_event")))
extern int wasm_host_gui_poll_event(int win_id, unsigned int event_buf_off);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  600
#define RGBA(r,g,b)    ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))
#define TCP_PROTO      6
#define HEAP_BASE      65536
#define URL_MAX        128
#define RECV_BUF_SIZE  512
#define LINE_H         8
#define CONTENT_W      780
#define CONTENT_X      10
#define TOOLBAR_H      30
#define CONTENT_Y      35
#define MAX_LINES      70

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int browser_heap = HEAP_BASE + 4096;
static int win_id = -1;
static unsigned int fb_off = 0;

/* URL bar */
static char url_buf[URL_MAX];
static int url_len = 0;
static int url_focused = 0;

/* Page content (line-based) */
static char page_lines[MAX_LINES][CONTENT_W + 1];
static int line_count = 0;
static int scroll_y = 0;

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int b_alloc(unsigned int n)
{
    unsigned int ptr = browser_heap;
    browser_heap += n;
    browser_heap = (browser_heap + 15) & ~15u;
    return ptr;
}

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int off = b_alloc(len);
    char *d = (char *)off;
    for (unsigned int i = 0; i < len; i++) d[i] = s[i];
    wasm_host_print(off, len);
}

static void fb_fill_rect(int x, int y, int w, int h, unsigned int color)
{
    if (x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if (y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    if (w <= 0 || h <= 0) return;
    unsigned int pitch = SCREEN_WIDTH * 4;
    unsigned int *fb = (unsigned int *)fb_off;
    for (int row = 0; row < h; row++) {
        unsigned int off = (y + row) * pitch / 4 + x;
        for (int col = 0; col < w; col++) fb[off + col] = color;
    }
}

/* 5x7 font */
static const unsigned char font_5x7[95 * 5] = {
    0,0,0,0,0, 0x7f,0x00,0x00,0x00,0x00, 0x03,0x00,0x03,0x00,0x00,
    0x7f,0x14,0x7f,0x14,0x7f, 0x2a,0x7f,0x2a,0x3e,0x2a,
    0x63,0x13,0x08,0x64,0x63, 0x36,0x49,0x56,0x20,0x50,
    0x03,0x00,0x00,0x00,0x00, 0x3e,0x41,0x00,0x00,0x00,
    0x41,0x3e,0x00,0x00,0,0x08,0x3e,0x1c,0x3e,0x08,
    0x08,0x08,0x3e,0x08,0x08, 0x00,0x50,0x30,0x00,0x00,
    0x08,0x08,0x08,0x08,0x08, 0x00,0x60,0x60,0x00,0x00,
    0x60,0x18,0x06,0x01,0x00, 0x3e,0x51,0x49,0x45,0x3e,
    0x40,0x7f,0x40,0x00,0x00, 0x72,0x49,0x49,0x49,0x46,
    0x21,0x41,0x49,0x4d,0x33, 0x18,0x14,0x12,0x7f,0x10,
    0x2f,0x49,0x49,0x49,0x31, 0x3c,0x4a,0x49,0x49,0x30,
    0x01,0x71,0x09,0x05,0x03, 0x36,0x49,0x49,0x49,0x36,
    0x06,0x49,0x49,0x29,0x1e, 0x00,0x36,0x36,0x00,0x00,
    0x00,0x56,0x36,0x00,0x00, 0x08,0x14,0x22,0x41,0x00,
    0x14,0x14,0x14,0x14,0x14, 0x00,0x41,0x22,0x14,0x08,
    0x02,0x01,0x51,0x09,0x06, 0x32,0x49,0x79,0x41,0x3e,
    0x7e,0x11,0x11,0x11,0x7e, 0x7f,0x49,0x49,0x49,0x36,
    0x3e,0x41,0x41,0x41,0x22, 0x7f,0x41,0x41,0x22,0x1c,
    0x7f,0x49,0x49,0x49,0x41, 0x7f,0x09,0x09,0x09,0x01,
    0x3e,0x41,0x49,0x49,0x7a, 0x7f,0x08,0x08,0x08,0x7f,
    0x00,0x41,0x7f,0x41,0x00, 0x20,0x40,0x41,0x3f,0x01,
    0x7f,0x08,0x14,0x22,0x41, 0x7f,0x40,0x40,0x40,0x40,
    0x7f,0x02,0x0c,0x02,0x7f, 0x7f,0x04,0x08,0x10,0x7f,
    0x3e,0x41,0x41,0x41,0x3e, 0x7f,0x09,0x09,0x09,0x06,
    0x3e,0x41,0x51,0x21,0x5e, 0x7f,0x09,0x19,0x29,0x46,
    0x26,0x49,0x49,0x49,0x32, 0x01,0x01,0x7f,0x01,0x01,
    0x3f,0x40,0x40,0x40,0x3f, 0x1f,0x20,0x40,0x20,0x1f,
    0x3f,0x40,0x38,0x40,0x3f, 0x63,0x14,0x08,0x14,0x63,
    0x07,0x08,0x70,0x08,0x07, 0x61,0x51,0x49,0x45,0x43,
    0x00,0x7f,0x41,0x00,0x00, 0x01,0x06,0x18,0x60,0x00,
    0x00,0x41,0x7f,0x00,0x00, 0x04,0x02,0x01,0x02,0x04,
    0x40,0x40,0x40,0x40,0x40, 0x00,0x01,0x02,0x04,0x00,
    0x20,0x54,0x54,0x54,0x78, 0x7f,0x48,0x44,0x44,0x38,
    0x38,0x44,0x44,0x44,0x20, 0x38,0x44,0x44,0x48,0x7f,
    0x38,0x54,0x54,0x54,0x18, 0x08,0x7e,0x09,0x01,0x02,
    0x18,0xa4,0xa4,0xa4,0x7c, 0x7f,0x08,0x04,0x04,0x78,
    0x00,0x44,0x7d,0x40,0x00, 0x40,0x80,0x84,0x7d,0x00,
    0x7f,0x10,0x28,0x44,0x00, 0x00,0x41,0x7f,0x40,0x00,
    0x7c,0x04,0x18,0x04,0x7c, 0x7c,0x08,0x04,0x04,0x78,
    0x38,0x44,0x44,0x44,0x38, 0xfc,0x24,0x24,0x24,0x18,
    0x18,0x24,0x24,0x24,0xfc, 0x7c,0x08,0x04,0x04,0x08,
    0x48,0x54,0x54,0x54,0x24, 0x04,0x3f,0x44,0x40,0x20,
    0x3c,0x40,0x40,0x20,0x7c, 0x1c,0x20,0x40,0x20,0x1c,
    0x3c,0x40,0x30,0x40,0x3c, 0x44,0x28,0x10,0x28,0x44,
    0x1c,0xa0,0xa0,0xa0,0x7c, 0x44,0x64,0x54,0x4c,0x44,
    0x00,0x08,0x36,0x41,0x00, 0x00,0x00,0x7f,0x00,0x00,
    0x00,0x41,0x36,0x08,0x00, 0x08,0x04,0x08,0x10,0x08,
};

static void fb_char(int bx, int by, char c, unsigned int fg, unsigned int bg)
{
    if (c < 32 || c > 126) return;
    int idx = (c - 32) * 5;
    for (int row = 0; row < 7; row++)
        for (int col = 0; col < 5; col++) {
            unsigned int color = (font_5x7[idx + col] & (1 << row)) ? fg : bg;
            unsigned int pitch = SCREEN_WIDTH * 4;
            int px = bx + col;
            int py = by + row;
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT)
                ((unsigned int *)fb_off)[py * SCREEN_WIDTH + px] = color;
        }
}

static void fb_string(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    int x = bx;
    for (int i = 0; s[i] && x + 5 < SCREEN_WIDTH; i++) {
        fb_char(x, by, s[i], fg, bg);
        x += 6;
    }
}

/* -------------------------------------------------------------------------- */
/* URL bar                                                                    */
/* -------------------------------------------------------------------------- */

static void browser_render_toolbar(void)
{
    unsigned int bg = RGBA(0x22, 0x22, 0x33);
    unsigned int text = RGBA(0xe0, 0xe0, 0xe0);

    fb_fill_rect(0, 0, SCREEN_WIDTH, TOOLBAR_H, bg);
    fb_string(10, 8, "URL: ", text, bg);

    if (url_focused) {
        char url_display[URL_MAX + 2];
        int i;
        for (i = 0; i < url_len; i++) url_display[i] = url_buf[i];
        url_display[i++] = '|'; /* cursor */
        url_display[i] = '\0';
        fb_string(55, 8, url_display, RGBA(0x00, 0xff, 0x88), bg);
    } else {
        char url_display[URL_MAX + 1];
        int i;
        for (i = 0; i < url_len; i++) url_display[i] = url_buf[i];
        url_display[i] = '\0';
        fb_string(55, 8, url_display, text, bg);
    }

    fb_string(SCREEN_WIDTH - 100, 8, "Q=quit", RGBA(0xff, 0x88, 0x88), bg);
}

/* -------------------------------------------------------------------------- */
/* Page rendering                                                             */
/* -------------------------------------------------------------------------- */

static void browser_render_content(void)
{
    unsigned int bg = RGBA(0xff, 0xff, 0xff);
    unsigned int text = RGBA(0x00, 0x00, 0x00);

    /* Content area */
    fb_fill_rect(0, TOOLBAR_H, SCREEN_WIDTH, SCREEN_HEIGHT - TOOLBAR_H, bg);

    /* Render visible lines */
    int start_line = scroll_y;
    if (start_line < 0) start_line = 0;
    int max_lines = (SCREEN_HEIGHT - TOOLBAR_H - 10) / LINE_H;

    for (int i = 0; i < max_lines && (start_line + i) < line_count; i++) {
        int ly = TOOLBAR_H + 5 + i * LINE_H;
        fb_string(CONTENT_X, ly, page_lines[start_line + i], text, bg);
    }

    if (line_count == 0) {
        fb_string(CONTENT_X, TOOLBAR_H + 50, "Enter a URL and press Enter to load", RGBA(0x88, 0x88, 0x88), bg);
    }
}

static void browser_flush(void)
{
    wasm_host_gui_draw(win_id, fb_off, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* HTTP client                                                                */
/* -------------------------------------------------------------------------- */

static void parse_url(const char *url, unsigned int *out_ip, unsigned int *out_port, const char **out_path)
{
    /* Simple parser: http://IP:PORT/path */
    const char *p = url;

    /* Skip http:// */
    if (p[0] == 'h' && p[1] == 't' && p[2] == 't' && p[3] == 'p' && p[4] == ':' && p[5] == '/' && p[6] == '/')
        p += 7;

    /* Parse IP address */
    unsigned int a = 0, b = 0, c = 0, d = 0;
    a = 0; while (*p >= '0' && *p <= '9') { a = a * 10 + (*p - '0'); p++; }
    if (*p == '.') p++;
    b = 0; while (*p >= '0' && *p <= '9') { b = b * 10 + (*p - '0'); p++; }
    if (*p == '.') p++;
    c = 0; while (*p >= '0' && *p <= '9') { c = c * 10 + (*p - '0'); p++; }
    if (*p == '.') p++;
    d = 0; while (*p >= '0' && *p <= '9') { d = d * 10 + (*p - '0'); p++; }

    *out_ip = (a << 24) | (b << 16) | (c << 8) | d;
    *out_port = 80;

    /* Check for port */
    if (*p == ':') {
        p++;
        unsigned int port = 0;
        while (*p >= '0' && *p <= '9') { port = port * 10 + (*p - '0'); p++; }
        *out_port = port;
    }

    *out_path = p;
}

static int http_get(const char *url)
{
    unsigned int ip, port;
    const char *path;
    parse_url(url, &ip, &port, &path);

    print_str("Browser: connecting to ");
    print_str(url);
    print_str("...\n");

    int sock = wasm_net_connect(ip, port, TCP_PROTO);
    if (sock < 0) {
        print_str("Browser: connection failed\n");
        /* Show error in browser */
        line_count = 0;
        const char *err = "Error: Connection failed";
        int len = (int)my_strlen(err);
        if (len < CONTENT_W) {
            for (int i = 0; i < len; i++) page_lines[0][i] = err[i];
            page_lines[0][len] = '\0';
            line_count = 1;
        }
        return -1;
    }

    print_str("Browser: connected, sending GET...\n");

    /* Build HTTP request */
    char *req_buf = (char *)(HEAP_BASE + RECV_BUF_SIZE);
    int req_len = 0;
    const char *method = "GET ";
    while (*method) req_buf[req_len++] = *method++;
    if (*path == '/') { while (*path) req_buf[req_len++] = *path++; }
    else { req_buf[req_len++] = '/'; while (*path) req_buf[req_len++] = *path++; }
    const char *http_ver = " HTTP/1.0\r\n";
    while (*http_ver) req_buf[req_len++] = *http_ver++;

    int sent = wasm_net_send(sock, HEAP_BASE + RECV_BUF_SIZE, req_len);
    if (sent != req_len) {
        print_str("Browser: send incomplete\n");
        wasm_net_close(sock);
        return -1;
    }

    /* Receive response */
    print_str("Browser: waiting for response...\n");
    wasm_host_sleep(1000);

    line_count = 0;
    scroll_y = 0;
    int total = 0;
    int in_body = 0;
    int col = 0;
    int in_headers = 1;

    for (int attempt = 0; attempt < 10; attempt++) {
        int n = wasm_net_recv(sock, HEAP_BASE, RECV_BUF_SIZE);
        if (n <= 0) {
            wasm_host_sleep(500);
            continue;
        }

        char *buf = (char *)HEAP_BASE;
        for (int i = 0; i < n && line_count < MAX_LINES; i++) {
            char c = buf[i];

            /* Detect body start (double CRLF) */
            if (in_headers && c == '\n') {
                in_headers = 0;
                in_body = 1;
                continue;
            }
            if (!in_body) continue;

            /* Simple HTML stripping: skip <...> tags */
            if (c == '<') {
                /* Skip until > */
                while (i < n && buf[i] != '>') i++;
                continue;
            }

            if (c == '\r') continue;

            if (c == '\n') {
                page_lines[line_count][col] = '\0';
                line_count++;
                col = 0;
                continue;
            }

            if (c >= 32 && c < 127 && col < CONTENT_W) {
                page_lines[line_count][col++] = c;
            }
        }
        total += n;
    }

    if (col > 0 && line_count < MAX_LINES) {
        page_lines[line_count][col] = '\0';
        line_count++;
    }

    wasm_net_close(sock);
    print_str("Browser: received ");
    /* Print total */
    { char tmp[12]; int ti = 0;
      if (total == 0) tmp[ti++] = '0';
      else { int v = total; while (v > 0) { tmp[ti++] = '0' + (v % 10); v /= 10; } }
      tmp[ti] = '\0';
      unsigned int off = b_alloc(ti);
      for (int j = 0; j < ti; j++) ((char *)off)[j] = tmp[j];
      wasm_host_print(off, ti);
    }
    print_str(" bytes\n");

    return total;
}

/* -------------------------------------------------------------------------- */
/* Input handling                                                             */
/* -------------------------------------------------------------------------- */

static void browser_handle_key(char c)
{
    if (c == 0x1b) { /* ESC - focus URL bar */
        url_focused = 1;
        url_len = 0;
    } else if (c == 'q' || c == 'Q') {
        print_str("Browser: exiting\n");
        wasm_host_exit(0);
    } else if (c == '\n' || c == '\r') {
        if (url_focused && url_len > 0) {
            url_buf[url_len] = '\0';
            url_focused = 0;
            http_get(url_buf);
        }
    } else if (c == '\b' || c == 127) {
        if (url_focused && url_len > 0) url_len--;
    } else if (c == 'j' || c == 'J') {
        /* Scroll down */
        if (scroll_y + 1 < line_count) scroll_y++;
    } else if (c == 'k' || c == 'K') {
        /* Scroll up */
        if (scroll_y > 0) scroll_y--;
    } else if (c >= 32 && c < 127 && url_focused && url_len < URL_MAX - 1) {
        url_buf[url_len++] = c;
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM Browser v1.0 ===\n");

    /* Create window */
    unsigned int tlen = my_strlen("Browser");
    unsigned int toff = b_alloc(tlen);
    char *td = (char *)toff;
    for (unsigned int i = 0; i < tlen; i++) td[i] = "Browser"[i];

    win_id = wasm_host_gui_create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, toff);
    if (win_id < 0) {
        print_str("Browser: failed to create window\n");
        wasm_host_exit(1);
    }

    /* Allocate framebuffer */
    fb_off = b_alloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4 + 4096);

    /* Event buffer */
    unsigned int evt_off = b_alloc(16);

    /* Default URL */
    const char *default_url = "http://10.0.2.2/index.html";
    for (int i = 0; default_url[i]; i++) url_buf[url_len++] = default_url[i];
    url_focused = 1;

    print_str("Browser: ready. ESC to edit URL, Enter to load, j/k scroll, Q quit.\n");

    for (;;) {
        browser_render_toolbar();
        browser_render_content();
        browser_flush();

        /* Keyboard input */
        int ch = wasm_host_getc();
        if (ch > 0) browser_handle_key((char)ch);

        /* GUI events */
        wasm_host_gui_poll_event(win_id, evt_off);

        wasm_host_yield();
    }
}
