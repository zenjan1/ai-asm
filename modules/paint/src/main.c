/*
 * aiasm-aarch64/modules/paint/src/main.c
 * Drawing/paint WASM application for AI-ASM v2.0
 *
 * Freehand drawing tool with color palette, displayed in a GUI window.
 * Uses keyboard for tool selection and mouse for drawing.
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

#define PAINT_WIDTH   800
#define PAINT_HEIGHT  600
#define TOOLBAR_H     40
#define RGBA(r,g,b)   ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int paint_heap = 65536;
static int win_id = -1;
static unsigned int fb_off = 0;

/* Drawing state */
static int brush_x = 0, brush_y = 0;
static int last_x = -1, last_y = -1;
static int drawing = 0;
static int brush_size = 3;
static int color_idx = 0;

/* Color palette (16 colors) */
static const unsigned int palette[] = {
    RGBA(0x00, 0x00, 0x00), /* black */
    RGBA(0xff, 0xff, 0xff), /* white */
    RGBA(0xff, 0x00, 0x00), /* red */
    RGBA(0x00, 0xff, 0x00), /* green */
    RGBA(0x00, 0x00, 0xff), /* blue */
    RGBA(0xff, 0xff, 0x00), /* yellow */
    RGBA(0xff, 0x00, 0xff), /* magenta */
    RGBA(0x00, 0xff, 0xff), /* cyan */
    RGBA(0xff, 0x88, 0x00), /* orange */
    RGBA(0x88, 0x00, 0xff), /* purple */
    RGBA(0x00, 0x88, 0xff), /* light blue */
    RGBA(0x88, 0xff, 0x00), /* lime */
    RGBA(0xff, 0x00, 0x88), /* pink */
    RGBA(0x88, 0x88, 0x88), /* gray */
    RGBA(0xcc, 0x88, 0x00), /* brown */
    RGBA(0x00, 0xcc, 0x88), /* teal */
};

#define PALETTE_COUNT 16

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int p_alloc(unsigned int n)
{
    unsigned int ptr = paint_heap;
    paint_heap += n;
    paint_heap = (paint_heap + 15) & ~15u;
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
    unsigned int off = p_alloc(len);
    char *d = (char *)off;
    for (unsigned int i = 0; i < len; i++) d[i] = s[i];
    wasm_host_print(off, len);
}

static void fb_pixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= PAINT_WIDTH || y < 0 || y >= PAINT_HEIGHT) return;
    unsigned int pitch = PAINT_WIDTH * 4;
    *(unsigned int *)(fb_off + y * pitch + x * 4) = color;
}

static void fb_fill_rect(int x, int y, int w, int h, unsigned int color)
{
    if (x + w > PAINT_WIDTH) w = PAINT_WIDTH - x;
    if (y + h > PAINT_HEIGHT) h = PAINT_HEIGHT - y;
    if (w <= 0 || h <= 0) return;
    unsigned int pitch = PAINT_WIDTH * 4;
    for (int row = 0; row < h; row++) {
        unsigned int off = (y + row) * pitch + x * 4;
        for (int col = 0; col < w; col++)
            *(unsigned int *)(fb_off + off + col * 4) = color;
    }
}

static void fb_circle(int cx, int cy, int r, unsigned int color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r)
                fb_pixel(cx + dx, cy + dy, color);
        }
    }
}

static void fb_line(int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = x1 - x0, dy = y1 - y0;
    int abs_dx = dx < 0 ? -dx : dx;
    int abs_dy = dy < 0 ? -dy : dy;
    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;
    int err = abs_dx - abs_dy;

    int x = x0, y = y0;
    for (;;) {
        fb_pixel(x, y, color);
        if (x == x1 && y == y1) break;
        int e2 = 2 * err;
        if (e2 > -abs_dy) { err -= abs_dy; x += sx; }
        if (e2 < abs_dx) { err += abs_dx; y += sy; }
    }
}

static void fb_char(int bx, int by, char c, unsigned int fg, unsigned int bg)
{
    if (c < 32 || c > 126) return;
    static const unsigned char font[] = {
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
    int idx = (c - 32) * 5;
    for (int row = 0; row < 7; row++)
        for (int col = 0; col < 5; col++) {
            unsigned int color = (font[idx + col] & (1 << row)) ? fg : bg;
            fb_pixel(bx + col, by + row, color);
        }
}

static void fb_string(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    int x = bx;
    for (int i = 0; s[i] && x + 5 < PAINT_WIDTH; i++) {
        fb_char(x, by, s[i], fg, bg);
        x += 6;
    }
}

/* -------------------------------------------------------------------------- */
/* Paint rendering                                                            */
/* -------------------------------------------------------------------------- */

static void paint_render_toolbar(void)
{
    unsigned int toolbar_bg = RGBA(0x22, 0x22, 0x22);
    unsigned int text_color = RGBA(0xff, 0xff, 0xff);

    /* Toolbar background */
    fb_fill_rect(0, 0, PAINT_WIDTH, TOOLBAR_H, toolbar_bg);

    /* Color palette swatches */
    int swatch_w = 16, swatch_h = 16, swatch_gap = 4;
    int palette_start_x = 10;
    for (int i = 0; i < PALETTE_COUNT; i++) {
        int sx = palette_start_x + i * (swatch_w + swatch_gap);
        int sy = 4;
        fb_fill_rect(sx, sy, swatch_w, swatch_h, palette[i]);
        if (i == color_idx) {
            /* Highlight current color */
            fb_fill_rect(sx, sy, swatch_w, 2, text_color);
            fb_fill_rect(sx, sy + swatch_h - 2, swatch_w, 2, text_color);
            fb_fill_rect(sx, sy, 2, swatch_h, text_color);
            fb_fill_rect(sx + swatch_w - 2, sy, 2, swatch_h, text_color);
        }
    }

    /* Brush size indicator */
    int bx = palette_start_x + PALETTE_COUNT * (swatch_w + swatch_gap) + 20;
    fb_string(bx, 5, "Brush:", text_color, toolbar_bg);
    for (int i = 0; i < brush_size; i++)
        fb_circle(bx + 50 + i * 4, 10, 2, text_color);

    /* Help text */
    fb_string(PAINT_WIDTH - 150, 5, "Q=quit C=clear", text_color, toolbar_bg);
}

static void paint_render(void)
{
    /* Canvas is white background */
    static int first = 1;
    if (first) {
        fb_fill_rect(0, TOOLBAR_H, PAINT_WIDTH, PAINT_HEIGHT - TOOLBAR_H, RGBA(0xff, 0xff, 0xff));
        first = 0;
    }
    paint_render_toolbar();
}

static void paint_flush(void)
{
    wasm_host_gui_draw(win_id, fb_off, 0, 0, PAINT_WIDTH, PAINT_HEIGHT);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* Drawing                                                                    */
/* -------------------------------------------------------------------------- */

static void paint_draw_stroke(int x, int y)
{
    unsigned int color = palette[color_idx];
    if (last_x >= 0 && last_y >= 0) {
        fb_line(last_x, last_y, x, y, color);
        fb_circle(x, y, brush_size / 2, color);
    }
    fb_circle(x, y, brush_size / 2, color);
    last_x = x;
    last_y = y;
}

static void paint_clear_canvas(void)
{
    fb_fill_rect(0, TOOLBAR_H, PAINT_WIDTH, PAINT_HEIGHT - TOOLBAR_H, RGBA(0xff, 0xff, 0xff));
    paint_render_toolbar();
}

/* -------------------------------------------------------------------------- */
/* Input handling                                                             */
/* -------------------------------------------------------------------------- */

static void paint_handle_key(char c)
{
    if (c == 'q' || c == 'Q') {
        print_str("Paint: exiting\n");
        wasm_host_exit(0);
    } else if (c == 'c' || c == 'C') {
        paint_clear_canvas();
        paint_flush();
        print_str("Paint: canvas cleared\n");
    } else if (c == '+' || c == '=') {
        if (brush_size < 20) brush_size++;
    } else if (c == '-') {
        if (brush_size > 1) brush_size--;
    } else if (c >= '1' && c <= '9') {
        color_idx = c - '1';
    } else if (c == '0') {
        color_idx = 9;
    } else {
        /* Try to cycle through palette */
        if (c == ' ') color_idx = (color_idx + 1) % PALETTE_COUNT;
    }
}

static void paint_handle_mouse(unsigned int mx, unsigned int my, int btn)
{
    if (my < TOOLBAR_H) return;  /* Ignore toolbar clicks for drawing */

    if (btn) {
        drawing = 1;
        brush_x = (int)mx;
        brush_y = (int)my;
        last_x = -1;
        last_y = -1;
        paint_draw_stroke(brush_x, brush_y);
    } else {
        drawing = 0;
        last_x = -1;
        last_y = -1;
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM Paint v1.0 ===\n");

    /* Create window */
    unsigned int tlen = my_strlen("Paint");
    unsigned int toff = p_alloc(tlen);
    char *td = (char *)toff;
    for (unsigned int i = 0; i < tlen; i++) td[i] = "Paint"[i];

    win_id = wasm_host_gui_create(0, 0, PAINT_WIDTH, PAINT_HEIGHT, toff);
    if (win_id < 0) {
        print_str("Paint: failed to create window\n");
        wasm_host_exit(1);
    }

    print_str("Paint: window created\n");

    /* Allocate framebuffer */
    fb_off = p_alloc(PAINT_WIDTH * PAINT_HEIGHT * 4 + 4096);

    /* Initial render */
    paint_clear_canvas();
    paint_flush();

    print_str("Paint: click and drag to draw. Space=next color, +/- brush, C=clear, Q=quit.\n");

    /* Event buffer */
    unsigned int evt_off = p_alloc(16);

    for (;;) {
        /* Poll for mouse events */
        int has_evt = wasm_host_gui_poll_event(win_id, evt_off);
        if (has_evt) {
            /* Parse event: type(1), code(1), x(2), y(2), timestamp(4) */
            unsigned char *evt = (unsigned char *)evt_off;
            unsigned char etype = evt[0];
            unsigned char code = evt[1];
            unsigned short ex = *(unsigned short *)(evt_off + 2);
            unsigned short ey = *(unsigned short *)(evt_off + 4);

            if (etype == 2) {  /* Mouse event */
                paint_handle_mouse(ex, ey, (int)code);
                paint_render_toolbar();
                paint_flush();
            }
        }

        /* Check keyboard */
        int ch = wasm_host_getc();
        if (ch > 0) {
            paint_handle_key((char)ch);
            paint_flush();
        }

        wasm_host_yield();
    }
}
