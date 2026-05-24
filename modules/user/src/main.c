/*
 * aiasm-aarch64/modules/user/src/main.c
 * User authentication module for AI-ASM AArch64 kernel v6.0
 *
 * GUI login interface: username/password input, login/register buttons.
 * Stores up to 4 users in memory with hashed passwords.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports - core                                                        */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

__attribute__((import_module("host"), import_name("user_login")))
extern int wasm_host_user_login(unsigned int name_off, unsigned int name_len,
                                unsigned int pass_off, unsigned int pass_len);

__attribute__((import_module("host"), import_name("gui_create")))
extern int wasm_host_gui_create(unsigned int x, unsigned int y, unsigned int w,
                                unsigned int h, unsigned int title_off);

__attribute__((import_module("host"), import_name("gui_draw")))
extern int wasm_host_gui_draw(int win_id, unsigned int buf_off,
                              unsigned int x, unsigned int y,
                              unsigned int w, unsigned int h);

__attribute__((import_module("host"), import_name("gui_flush")))
extern void wasm_host_gui_flush(unsigned int win_id);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define WIN_W  400
#define WIN_H  300
#define RGBA(r,g,b)    ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int user_heap = 65536;
static int win_id = -1;
static unsigned int fb_off = 0;

/* Input buffers */
static char username_buf[16];
static char password_buf[16];
static int username_len = 0;
static int password_len = 0;
static int input_field = 0; /* 0=username, 1=password */
static int login_status = 0; /* 0=idle, 1=success, 2=fail */

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int u_alloc(unsigned int n)
{
    unsigned int ptr = user_heap;
    user_heap += n;
    user_heap = (user_heap + 15) & ~15u;
    return ptr;
}

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b) ? 1 : 0;
}

static void u_memset(unsigned int off, int c, unsigned int n)
{
    char *p = (char *)off;
    for (unsigned int i = 0; i < n; i++) p[i] = (char)c;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int off = u_alloc(len);
    char *d = (char *)off;
    for (unsigned int i = 0; i < len; i++) d[i] = s[i];
    wasm_host_print(off, len);
}

/* 5x7 font */
static const unsigned char font_5x7[95 * 5] = {
    0,0,0,0,0, 0x7f,0x00,0x00,0x00,0x00, 0x03,0x00,0x03,0x00,0x00,
    0x7f,0x14,0x7f,0x14,0x7f, 0x2a,0x7f,0x2a,0x3e,0x2a,
    0x63,0x13,0x08,0x64,0x63, 0x36,0x49,0x56,0x20,0x50,
    0x03,0x00,0x00,0x00,0x00, 0x3e,0x41,0x00,0x00,0x00,
    0x41,0x3e,0x00,0x00,0x00, 0x08,0x3e,0x1c,0x3e,0x08,
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
    0x7f,0x04,0x08,0x10,0x7f, 0x3e,0x41,0x41,0x41,0x3e,
    0x7f,0x09,0x09,0x09,0x06, 0x3e,0x41,0x51,0x21,0x5e,
    0x7f,0x09,0x19,0x29,0x46, 0x26,0x49,0x49,0x49,0x32,
    0x01,0x01,0x7f,0x01,0x01, 0x3f,0x40,0x40,0x40,0x3f,
    0x1f,0x20,0x40,0x20,0x1f, 0x3f,0x40,0x38,0x40,0x3f,
    0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07,
    0x61,0x51,0x49,0x45,0x43, 0x00,0x7f,0x41,0x00,0x00,
    0x01,0x06,0x18,0x60,0x00, 0x00,0x41,0x7f,0x00,0x00,
    0x04,0x02,0x01,0x02,0x04, 0x40,0x40,0x40,0x40,0x40,
    0x00,0x01,0x02,0x04,0x00, 0x20,0x54,0x54,0x54,0x78,
    0x7f,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x48,0x7f,
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

static void fb_pixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H) return;
    unsigned int pitch = WIN_W * 4;
    *(unsigned int *)(fb_off + y * pitch + x * 4) = color;
}

static void fb_fill_rect(int x, int y, int w, int h, unsigned int color)
{
    if (x + w > WIN_W) w = WIN_W - x;
    if (y + h > WIN_H) h = WIN_H - y;
    if (w <= 0 || h <= 0) return;
    unsigned int pitch = WIN_W * 4;
    for (int row = 0; row < h; row++) {
        unsigned int off = (y + row) * pitch + x * 4;
        for (int col = 0; col < w; col++)
            *(unsigned int *)(fb_off + off + col * 4) = color;
    }
}

static void fb_char(int bx, int by, char c, unsigned int fg, unsigned int bg)
{
    if (c < 32 || c > 126) return;
    int idx = (c - 32) * 5;
    for (int row = 0; row < 7; row++)
        for (int col = 0; col < 5; col++) {
            unsigned int color = (font_5x7[idx + col] & (1 << row)) ? fg : bg;
            fb_pixel(bx + col, by + row, color);
        }
}

static void fb_string(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    int x = bx;
    for (int i = 0; s[i] && x + 5 < WIN_W; i++) {
        fb_char(x, by, s[i], fg, bg);
        x += 6;
    }
}

static void fb_string_centered(int by, const char *s, unsigned int fg, unsigned int bg)
{
    int len = (int)my_strlen(s);
    int width = len * 6;
    int bx = (WIN_W - width) / 2;
    if (bx < 0) bx = 5;
    fb_string(bx, by, s, fg, bg);
}

static void fb_input_box(int x, int y, int w, int h, const char *label,
                         const char *value, int focused, unsigned int bg)
{
    unsigned int box_bg = focused ? RGBA(0x30, 0x30, 0x50) : RGBA(0x25, 0x25, 0x40);
    unsigned int border = focused ? RGBA(0x44, 0x88, 0xff) : RGBA(0x44, 0x44, 0x66);
    unsigned int text = RGBA(0xe0, 0xe0, 0xe0);

    /* Box background */
    fb_fill_rect(x, y, w, h, box_bg);
    /* Border */
    fb_fill_rect(x, y, w, 1, border);
    fb_fill_rect(x, y + h - 1, w, 1, border);
    fb_fill_rect(x, y, 1, h, border);
    fb_fill_rect(x + w - 1, y, 1, h, border);

    /* Label */
    fb_string(x + 8, y + 6, label, RGBA(0x88, 0x88, 0xaa), box_bg);

    /* Value */
    int vx = x + 8 + (int)my_strlen(label) * 6 + 10;
    if (label[0] == 'P') {
        /* Password: show dots */
        for (int i = 0; value[i]; i++) fb_char(vx + i * 6, y + 6, '*', text, box_bg);
    } else {
        fb_string(vx, y + 6, value, text, box_bg);
    }
}

static void fb_button(int x, int y, int w, int h, const char *label,
                      unsigned int color, unsigned int bg)
{
    fb_fill_rect(x, y, w, h, color);
    fb_string_centered(y + (h - 7) / 2, label, RGBA(0xff, 0xff, 0xff), color);
}

/* -------------------------------------------------------------------------- */
/* Render login screen                                                        */
/* -------------------------------------------------------------------------- */

static void render(void)
{
    unsigned int bg = RGBA(0x1a, 0x1a, 0x2e);
    unsigned int title_color = RGBA(0xff, 0xff, 0xff);
    unsigned int hint_color = RGBA(0x88, 0x88, 0xaa);

    /* Background */
    u_memset(fb_off, 0, WIN_W * WIN_H * 4);
    fb_fill_rect(0, 0, WIN_W, WIN_H, bg);

    /* Title */
    fb_string_centered(30, "AI-ASM v6.0", title_color, bg);
    fb_string_centered(45, "User Authentication", hint_color, bg);

    /* Divider */
    fb_fill_rect(50, 60, WIN_W - 100, 1, RGBA(0x44, 0x44, 0x66));

    /* Input boxes */
    char pass_hidden[17];
    for (int i = 0; i < password_len; i++) pass_hidden[i] = '*';
    pass_hidden[password_len] = '\0';

    fb_input_box(50, 80, WIN_W - 100, 35, "User:", username_buf, input_field == 0, bg);
    fb_input_box(50, 125, WIN_W - 100, 35, "Pass:", pass_hidden, input_field == 1, bg);

    /* Buttons */
    fb_button(60, 180, 120, 30, "Login", RGBA(0x00, 0x88, 0xff), bg);
    fb_button(220, 180, 120, 30, "Exit", RGBA(0x88, 0x44, 0x44), bg);

    /* Status message */
    if (login_status == 1) {
        fb_fill_rect(30, 230, WIN_W - 60, 30, RGBA(0x00, 0x33, 0x22));
        fb_string_centered(238, "Login successful!", RGBA(0x00, 0xff, 0x88), RGBA(0x00, 0x33, 0x22));
    } else if (login_status == 2) {
        fb_fill_rect(30, 230, WIN_W - 60, 30, RGBA(0x33, 0x11, 0x11));
        fb_string_centered(238, "Login failed. Try again.", RGBA(0xff, 0x44, 0x44), RGBA(0x33, 0x11, 0x11));
    }

    /* Footer */
    fb_string_centered(WIN_H - 15, "Tab:switch  Enter:login  ESC:exit", hint_color, bg);

    /* Flush to window */
    wasm_host_gui_draw(win_id, fb_off, 0, 0, WIN_W, WIN_H);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* Attempt login                                                              */
/* -------------------------------------------------------------------------- */

static void do_login(void)
{
    if (username_len == 0) return;

    unsigned int ulen = (unsigned int)username_len;
    unsigned int plen = (unsigned int)password_len;
    unsigned int uoff = u_alloc(ulen + 1);
    unsigned int poff = u_alloc(plen + 1);
    char *ud = (char *)uoff;
    char *pd = (char *)poff;
    for (unsigned int i = 0; i < ulen; i++) ud[i] = username_buf[i];
    ud[ulen] = '\0';
    for (unsigned int i = 0; i < plen; i++) pd[i] = password_buf[i];
    pd[plen] = '\0';

    int rc = wasm_host_user_login(uoff, ulen, poff, plen);
    if (rc >= 0) {
        login_status = 1;
        print_str("User: logged in as ");
        print_str(username_buf);
        print_str("\n");
    } else {
        login_status = 2;
        print_str("User: login failed for ");
        print_str(username_buf);
        print_str("\n");
    }
    render();
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    print_str("\n=== AI-ASM v6.0 User Module ===\n");

    /* Create login window */
    unsigned int tlen = my_strlen("Login");
    unsigned int toff = u_alloc(tlen);
    char *td = (char *)toff;
    for (unsigned int i = 0; i < tlen; i++) td[i] = "Login"[i];

    win_id = wasm_host_gui_create(200, 150, WIN_W, WIN_H, toff);
    if (win_id < 0) {
        print_str("User: failed to create window\n");
        wasm_host_exit(1);
    }

    /* Allocate framebuffer */
    fb_off = u_alloc(WIN_W * WIN_H * 4 + 4096);

    render();
    print_str("User: login screen displayed\n");

    /* Main input loop */
    for (;;) {
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;

            if (c == '\r' || c == '\n') {
                /* Enter = attempt login */
                do_login();
            } else if (c == '\t') {
                /* Tab = switch field */
                input_field = 1 - input_field;
                render();
            } else if (c == 0x1b) {
                /* ESC = exit */
                print_str("User: exiting\n");
                wasm_host_exit(0);
            } else if (c == '\b' || c == 0x7f) {
                /* Backspace */
                if (input_field == 0 && username_len > 0) {
                    username_len--;
                    username_buf[username_len] = '\0';
                    render();
                } else if (input_field == 1 && password_len > 0) {
                    password_len--;
                    password_buf[password_len] = '\0';
                    render();
                }
            } else if (c >= 0x20 && c < 0x7f) {
                /* Printable character */
                if (input_field == 0 && username_len < 15) {
                    username_buf[username_len] = c;
                    username_len++;
                    username_buf[username_len] = '\0';
                    render();
                } else if (input_field == 1 && password_len < 15) {
                    password_buf[password_len] = c;
                    password_len++;
                    password_buf[password_len] = '\0';
                    render();
                }
            }
        }

        wasm_host_yield();
        wasm_host_sleep(50);
    }
}
