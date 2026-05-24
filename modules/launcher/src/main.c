/*
 * aiasm-aarch64/modules/launcher/src/main.c
 * Application launcher for AI-ASM v2.0
 *
 * Displays a menu of available applications and spawns them on key press.
 * Returns to launcher when applications exit.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports - core                                                        */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long long wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("sleep")))
extern void wasm_host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

__attribute__((import_module("host"), import_name("spawn")))
extern int wasm_host_spawn(unsigned int name_off, unsigned int name_len);

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
/* WASM imports - input                                                       */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  600
#define RGBA(r,g,b)    ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int launcher_heap = 65536;
static int win_id = -1;
static unsigned int fb_off = 0;

/* Application list */
typedef struct {
    const char *key;
    const char *name;
    const char *module;
    unsigned int color;
} app_entry_t;

static const app_entry_t apps[] = {
    { "1", "Text Editor",  "editor",   RGBA(0x00, 0x88, 0xff) },
    { "2", "Calculator",   "calc",     RGBA(0xff, 0x95, 0x00) },
    { "3", "Paint",        "paint",    RGBA(0x00, 0xcc, 0x88) },
    { "4", "Shell",        "shell",    RGBA(0x88, 0x88, 0xff) },
    { "5", "Run Tests",    "test",     RGBA(0xff, 0x00, 0x88) },
    { "6", "File Manager", "filemgr",  RGBA(0xaa, 0x88, 0x44) },
    { "7", "Settings",     "settings", RGBA(0x66, 0x88, 0xaa) },
    { 0, 0, 0, 0 },
};

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static unsigned int l_alloc(unsigned int n)
{
    unsigned int ptr = launcher_heap;
    launcher_heap += n;
    launcher_heap = (launcher_heap + 15) & ~15u;
    return ptr;
}

static void l_memset(unsigned int off, int c, unsigned int n)
{
    char *p = (char *)off;
    for (unsigned int i = 0; i < n; i++) p[i] = (char)c;
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
    unsigned int off = l_alloc(len);
    char *d = (char *)off;
    for (unsigned int i = 0; i < len; i++) d[i] = s[i];
    wasm_host_print(off, len);
}

static void fb_pixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    unsigned int pitch = SCREEN_WIDTH * 4;
    *(unsigned int *)(fb_off + y * pitch + x * 4) = color;
}

static void fb_fill_rect(int x, int y, int w, int h, unsigned int color)
{
    if (x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if (y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    if (w <= 0 || h <= 0) return;
    unsigned int pitch = SCREEN_WIDTH * 4;
    for (int row = 0; row < h; row++) {
        unsigned int off = (y + row) * pitch + x * 4;
        for (int col = 0; col < w; col++)
            *(unsigned int *)(fb_off + off + col * 4) = color;
    }
}

static void fb_rounded_rect(int x, int y, int w, int h, int r, unsigned int color)
{
    /* Main body */
    fb_fill_rect(x + r, y, w - 2 * r, h, color);
    fb_fill_rect(x, y + r, w, h - 2 * r, color);
    /* Corners (simplified) */
    fb_fill_rect(x + r, y, w - 2 * r, r, color);
    fb_fill_rect(x + r, y + h - r, w - 2 * r, r, color);
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
            fb_pixel(bx + col, by + row, color);
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

static void fb_string_centered(int by, const char *s, unsigned int fg, unsigned int bg)
{
    int len = (int)my_strlen(s);
    int width = len * 6;
    int bx = (SCREEN_WIDTH - width) / 2;
    if (bx < 0) bx = 5;
    fb_string(bx, by, s, fg, bg);
}

static void fb_string_large(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    /* Draw string at 2x scale */
    int x = bx;
    for (int i = 0; s[i] && x + 10 < SCREEN_WIDTH; i++) {
        char c = s[i];
        if (c < 32 || c > 126) { x += 12; continue; }
        int idx = (c - 32) * 5;
        for (int row = 0; row < 14; row++) {
            int src_row = row / 2;
            for (int col = 0; col < 10; col++) {
                int src_col = col / 2;
                unsigned int color = (src_col < 5 && font_5x7[idx + src_col] & (1 << src_row)) ? fg : bg;
                fb_pixel(x + col, by + row, color);
            }
        }
        x += 12;
    }
}

/* -------------------------------------------------------------------------- */
/* Launcher rendering                                                         */
/* -------------------------------------------------------------------------- */

static void launcher_render(const char *status_msg)
{
    unsigned int bg = RGBA(0x1a, 0x1a, 0x2e);
    unsigned int card_bg = RGBA(0x25, 0x25, 0x40);
    unsigned int text = RGBA(0xe0, 0xe0, 0xe0);
    unsigned int title_color = RGBA(0xff, 0xff, 0xff);
    unsigned int hint_color = RGBA(0x88, 0x88, 0xaa);

    /* Background */
    fb_fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg);

    /* Title */
    fb_string_centered(40, "AI-ASM v2.0", title_color, bg);
    fb_string_centered(60, "Application Launcher", hint_color, bg);

    /* Divider line */
    fb_fill_rect(100, 85, SCREEN_WIDTH - 200, 2, RGBA(0x44, 0x44, 0x66));

    /* Application cards */
    int card_w = 300, card_h = 70;
    int card_gap = 20;
    int start_x = (SCREEN_WIDTH - (2 * card_w + card_gap)) / 2;
    int start_y = 110;
    int col = 0, row = 0;

    for (int i = 0; apps[i].key != 0; i++) {
        int cx = start_x + col * (card_w + card_gap);
        int cy = start_y + row * (card_h + card_gap);

        /* Card background */
        fb_fill_rect(cx, cy, card_w, card_h, card_bg);

        /* Color accent bar on left */
        fb_fill_rect(cx, cy, 4, card_h, apps[i].color);

        /* Key badge */
        fb_fill_rect(cx + 15, cy + 15, 30, 40, apps[i].color);
        char key_str[2] = { apps[i].key[0], '\0' };
        fb_string(cx + 22, cy + 25, key_str, RGBA(0xff, 0xff, 0xff), apps[i].color);

        /* App name */
        fb_string(cx + 55, cy + 30, apps[i].name, text, card_bg);

        col++;
        if (col >= 2) { col = 0; row++; }
    }

    /* Status message */
    if (status_msg && status_msg[0]) {
        fb_fill_rect(50, SCREEN_HEIGHT - 80, SCREEN_WIDTH - 100, 40, card_bg);
        fb_string_centered(SCREEN_HEIGHT - 70, status_msg, RGBA(0x00, 0xcc, 0xff), card_bg);
    }

    /* Footer hint */
    fb_string_centered(SCREEN_HEIGHT - 25, "Press 1-5 to launch an application", hint_color, bg);
}

static void launcher_flush(void)
{
    wasm_host_gui_draw(win_id, fb_off, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* Spawn application                                                          */
/* -------------------------------------------------------------------------- */

static int spawn_app(const char *name)
{
    unsigned int nlen = my_strlen(name);
    unsigned int noff = l_alloc(nlen);
    char *d = (char *)noff;
    for (unsigned int i = 0; i < nlen; i++) d[i] = name[i];

    int rc = wasm_host_spawn(noff, nlen);
    return rc;
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM v2.0 Launcher ===\n");

    /* Create launcher window */
    unsigned int tlen = my_strlen("AI-ASM Launcher");
    unsigned int toff = l_alloc(tlen);
    char *td = (char *)toff;
    for (unsigned int i = 0; i < tlen; i++) td[i] = "AI-ASM Launcher"[i];

    win_id = wasm_host_gui_create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, toff);
    if (win_id < 0) {
        print_str("Launcher: failed to create window\n");
        wasm_host_exit(1);
    }

    print_str("Launcher: window created\n");

    /* Allocate framebuffer */
    fb_off = l_alloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4 + 4096);

    /* Event buffer */
    unsigned int evt_off = l_alloc(16);

    /* Status message */
    const char *status = "Ready - select an application";

    /* Initial render */
    launcher_render(status);
    launcher_flush();

    print_str("Launcher: waiting for input...\n");

    for (;;) {
        /* Check keyboard input */
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;

            /* Find matching app */
            for (int i = 0; apps[i].key != 0; i++) {
                if (c == apps[i].key[0]) {
                    print_str("Launcher: starting ");
                    print_str(apps[i].name);
                    print_str("...\n");

                    /* Show launching status */
                    static char msg_buf[64];
                    int mi = 0;
                    const char *prefix = "Launching: ";
                    while (*prefix) msg_buf[mi++] = *prefix++;
                    const char *nm = apps[i].name;
                    while (*nm && mi < 60) msg_buf[mi++] = *nm++;
                    msg_buf[mi] = '\0';

                    launcher_render(msg_buf);
                    launcher_flush();

                    int rc = spawn_app(apps[i].module);
                    if (rc > 0) {
                        status = "Application started successfully";
                        print_str("Launcher: spawned (id=");
                        /* print rc */
                        { char tmp[4]; int ti = 0; int v = rc;
                          if (v == 0) tmp[ti++] = '0';
                          else { while (v > 0) { tmp[ti++] = '0' + (v % 10); v /= 10; } }
                          for (int j = 0; j < ti; j++) print_str((char[]){tmp[j], '\0'}); }
                        print_str(")\n");
                    } else {
                        status = "Failed to start application";
                        print_str("Launcher: spawn failed (rc=");
                        { char tmp[4]; int ti = 0; int v = rc;
                          if (v < 0) { print_str("-"); v = -v; }
                          if (v == 0) tmp[ti++] = '0';
                          else { while (v > 0) { tmp[ti++] = '0' + (v % 10); v /= 10; } }
                          for (int j = 0; j < ti; j++) print_str((char[]){tmp[j], '\0'}); }
                        print_str(")\n");
                    }

                    launcher_render(status);
                    launcher_flush();
                    break;
                }
            }

            /* ESC to refresh/redraw */
            if (c == 0x1b) {
                status = "Ready - select an application";
                launcher_render(status);
                launcher_flush();
            }
        }

        /* Poll for GUI events */
        wasm_host_gui_poll_event(win_id, evt_off);

        wasm_host_yield();
    }
}
