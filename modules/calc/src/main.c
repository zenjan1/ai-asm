/*
 * aiasm-aarch64/modules/calc/src/main.c
 * Calculator WASM application for AI-ASM v2.0
 *
 * GUI-based calculator with button grid, keyboard input, and display.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports                                                               */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

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

#define CALC_WIDTH  400
#define CALC_HEIGHT 500
#define BTN_COLS    4
#define BTN_ROWS    5
#define BTN_W       80
#define BTN_H       60
#define DISP_Y      30
#define BTN_START_Y 110
#define RGBA(r,g,b) ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int calc_heap = 65536;
static int win_id = -1;
static unsigned int fb_off = 0;

/* Calculator state */
static char display_buf[20];
static int display_len = 0;
static long accumulator = 0;
static long current_value = 0;
static char pending_op = '\0';
static int new_number = 1;

/* Button labels */
static const char *buttons[BTN_ROWS][BTN_COLS] = {
    {"C", "+/-", "%", "/"},
    {"7", "8", "9", "x"},
    {"4", "5", "6", "-"},
    {"1", "2", "3", "+"},
    {"0", ".", "=", ""},
};

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int c_alloc(unsigned int n)
{
    unsigned int ptr = calc_heap;
    calc_heap += n;
    calc_heap = (calc_heap + 15) & ~15u;
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
    unsigned int off = c_alloc(len);
    char *d = (char *)off;
    for (unsigned int i = 0; i < len; i++) d[i] = s[i];
    wasm_host_print(off, len);
}

static void fb_pixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= CALC_WIDTH || y < 0 || y >= CALC_HEIGHT) return;
    unsigned int pitch = CALC_WIDTH * 4;
    *(unsigned int *)(fb_off + y * pitch + x * 4) = color;
}

static void fb_fill_rect(int x, int y, int w, int h, unsigned int color)
{
    if (x + w > CALC_WIDTH) w = CALC_WIDTH - x;
    if (y + h > CALC_HEIGHT) h = CALC_HEIGHT - y;
    unsigned int pitch = CALC_WIDTH * 4;
    for (int row = 0; row < h; row++) {
        unsigned int off = (y + row) * pitch + x * 4;
        for (int col = 0; col < w; col++)
            *(unsigned int *)(fb_off + off + col * 4) = color;
    }
}

static void fb_draw_border_rect(int x, int y, int w, int h,
                                unsigned int bg, unsigned int border, unsigned int text)
{
    fb_fill_rect(x, y, w, h, bg);
    fb_fill_rect(x, y, w, 1, border);
    fb_fill_rect(x, y, 1, h, border);
    fb_fill_rect(x + w - 1, y, 1, h, border);
    fb_fill_rect(x, y + h - 1, w, 1, border);
}

/* 5x7 font subset for calculator display */
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
    if (c < 32 || c > 126) c = ' ';
    int idx = (c - 32) * 5;
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 5; col++) {
            unsigned int color = (idx + col < 95 * 5 && row < 7 &&
                (font_5x7[idx + col] & (1 << row))) ? fg : bg;
            fb_pixel(bx + col, by + row, color);
        }
    }
}

static void fb_string(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    int x = bx;
    for (int i = 0; s[i] && x + 5 < CALC_WIDTH; i++) {
        fb_char(x, by, s[i], fg, bg);
        x += 6;
    }
}

/* -------------------------------------------------------------------------- */
/* Calculator rendering                                                       */
/* -------------------------------------------------------------------------- */

static void calc_render(void)
{
    unsigned int bg = RGBA(0x33, 0x33, 0x33);
    unsigned int disp_bg = RGBA(0x1a, 0x1a, 0x1a);
    unsigned int btn_bg = RGBA(0x55, 0x55, 0x55);
    unsigned int btn_op = RGBA(0xff, 0x95, 0x00);
    unsigned int btn_func = RGBA(0x88, 0x88, 0x88);
    unsigned int text_white = RGBA(0xff, 0xff, 0xff);
    unsigned int text_dark = RGBA(0x00, 0x00, 0x00);

    /* Background */
    fb_fill_rect(0, 0, CALC_WIDTH, CALC_HEIGHT, bg);

    /* Display area */
    fb_fill_rect(10, DISP_Y, CALC_WIDTH - 20, 70, disp_bg);

    /* Display text (right-aligned) */
    int text_x = CALC_WIDTH - 25 - (display_len * 12);
    if (text_x < 15) text_x = 15;
    fb_string(text_x, DISP_Y + 25, display_buf, text_white, disp_bg);

    /* Button grid */
    int btn_gap = 10;
    for (int row = 0; row < BTN_ROWS; row++) {
        for (int col = 0; col < BTN_COLS; col++) {
            const char *label = buttons[row][col];
            if (label[0] == '\0') continue;

            int bx = 10 + col * (BTN_W + btn_gap);
            int by = BTN_START_Y + row * (BTN_H + btn_gap);

            unsigned int btn_color = btn_bg;
            unsigned int txt_color = text_white;

            /* Operator buttons */
            if (label[0] == '/' || label[0] == 'x' ||
                label[0] == '-' || label[0] == '+' || label[0] == '=') {
                btn_color = btn_op;
            }
            /* Function buttons */
            if (label[0] == 'C' || label[0] == '+' || label[1] == '/' ||
                label[0] == '%') {
                btn_color = btn_func;
                txt_color = text_dark;
            }

            fb_draw_border_rect(bx, by, BTN_W, BTN_H, btn_color,
                               RGBA(0x66, 0x66, 0x66), txt_color);

            /* Center label */
            int lx = bx + (BTN_W / 2) - (my_strlen(label) * 6 / 2);
            int ly = by + (BTN_H / 2) - 3;
            fb_string(lx, ly, label, txt_color, btn_color);
        }
    }
}

static void calc_flush(void)
{
    wasm_host_gui_draw(win_id, fb_off, 0, 0, CALC_WIDTH, CALC_HEIGHT);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* Calculator logic                                                           */
/* -------------------------------------------------------------------------- */

static void calc_update_display(void)
{
    long val = new_number ? current_value : accumulator;
    display_len = 0;
    long v = val;
    if (v < 0) {
        display_buf[display_len++] = '-';
        v = -v;
    }
    char tmp[12];
    int ti = 0;
    if (v == 0) tmp[ti++] = '0';
    else {
        while (v > 0) { tmp[ti++] = '0' + (char)(v % 10); v /= 10; }
    }
    while (ti > 0) display_buf[display_len++] = tmp[--ti];
    display_buf[display_len] = '\0';
}

static void calc_clear(void)
{
    accumulator = 0;
    current_value = 0;
    pending_op = '\0';
    new_number = 1;
    calc_update_display();
}

static void calc_input_digit(int d)
{
    if (new_number) {
        current_value = d;
        new_number = 0;
    } else {
        current_value = current_value * 10 + d;
    }
    calc_update_display();
}

static void calc_input_dot(void)
{
    /* Integer-only calculator, ignore dot */
    (void)0;
}

static void calc_input_op(char op)
{
    if (pending_op != '\0') {
        /* Apply previous operation */
        switch (pending_op) {
            case '+': accumulator += current_value; break;
            case '-': accumulator -= current_value; break;
            case 'x': accumulator *= current_value; break;
            case '/': if (current_value != 0) accumulator /= current_value; break;
            case '%': if (current_value != 0) accumulator %= current_value; break;
        }
    } else {
        accumulator = current_value;
    }
    pending_op = op;
    new_number = 1;
    calc_update_display();
}

static void calc_input_equals(void)
{
    if (pending_op != '\0') {
        switch (pending_op) {
            case '+': accumulator += current_value; break;
            case '-': accumulator -= current_value; break;
            case 'x': accumulator *= current_value; break;
            case '/': if (current_value != 0) accumulator /= current_value; break;
            case '%': if (current_value != 0) accumulator %= current_value; break;
        }
        pending_op = '\0';
        current_value = accumulator;
    }
    new_number = 1;
    calc_update_display();
}

static void calc_input_negate(void)
{
    current_value = -current_value;
    calc_update_display();
}

static void calc_handle_key(char c)
{
    if (c >= '0' && c <= '9') {
        calc_input_digit(c - '0');
    } else if (c == '.') {
        calc_input_dot();
    } else if (c == '+') {
        calc_input_op('+');
    } else if (c == '-') {
        calc_input_op('-');
    } else if (c == '*' || c == 'x') {
        calc_input_op('x');
    } else if (c == '/') {
        calc_input_op('/');
    } else if (c == '%') {
        calc_input_op('%');
    } else if (c == '=') {
        calc_input_equals();
    } else if (c == 'c' || c == 'C') {
        calc_clear();
    } else if (c == 'n') {
        calc_input_negate();
    } else if (c == 'q' || c == 'Q') {
        print_str("Calc: exiting\n");
        wasm_host_exit(0);
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM Calculator v1.0 ===\n");

    /* Create window */
    unsigned int tlen = my_strlen("Calculator");
    unsigned int toff = c_alloc(tlen);
    char *td = (char *)toff;
    for (unsigned int i = 0; i < tlen; i++) td[i] = "Calculator"[i];

    win_id = wasm_host_gui_create(200, 50, CALC_WIDTH, CALC_HEIGHT, toff);
    if (win_id < 0) {
        print_str("Calc: failed to create window\n");
        wasm_host_exit(1);
    }

    print_str("Calc: window created\n");

    /* Allocate framebuffer */
    fb_off = c_alloc(CALC_WIDTH * CALC_HEIGHT * 4 + 4096);

    /* Initial display */
    calc_update_display();
    calc_render();
    calc_flush();

    print_str("Calc: use keyboard. 0-9 digits, +-* /, =, C clear, Q quit.\n");

    for (;;) {
        /* Check keyboard */
        int ch = wasm_host_getc();
        if (ch > 0) {
            calc_handle_key((char)ch);
            calc_render();
            calc_flush();
        }
        wasm_host_yield();
    }
}
