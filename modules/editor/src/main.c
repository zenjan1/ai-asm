/*
 * aiasm-aarch64/modules/editor/src/main.c
 * Text editor WASM application for AI-ASM v2.0
 *
 * Opens files, displays content, accepts keyboard input, saves to disk.
 * Uses GUI window for display and FS APIs for persistence.
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

__attribute__((import_module("host"), import_name("yield")))
extern void wasm_host_yield(void);

/* -------------------------------------------------------------------------- */
/* WASM imports - filesystem                                                  */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("fs_open")))
extern int wasm_host_fs_open(unsigned int path_off, unsigned int path_len);

__attribute__((import_module("host"), import_name("fs_read")))
extern int wasm_host_fs_read(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_write")))
extern int wasm_host_fs_write(int fd, unsigned int buf_off, unsigned int len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void wasm_host_fs_close(int fd);

__attribute__((import_module("host"), import_name("fs_create")))
extern int wasm_host_fs_create(unsigned int name_off, unsigned int name_len);

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
/* Constants and globals                                                      */
/* -------------------------------------------------------------------------- */

#define EDITOR_WIDTH    760
#define EDITOR_HEIGHT   560
#define EDITOR_COLS     76
#define EDITOR_ROWS     28
#define MAX_FILE_SIZE   8192

static unsigned int editor_heap = 65536;

static char file_buffer[MAX_FILE_SIZE];
static int file_size = 0;
static int cursor_pos = 0;
static int cursor_row = 0;
static int cursor_col = 0;
static int win_id = -1;
static char filename[64];
static int filename_len = 0;

/* Framebuffer: 760x560 RGBA = 1,702,400 bytes, allocate at runtime */
static unsigned int fb_offset = 0;

/* -------------------------------------------------------------------------- */
/* Memory helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int ed_alloc(unsigned int n)
{
    unsigned int ptr = editor_heap;
    editor_heap += n;
    editor_heap = (editor_heap + 15) & ~15u;
    return ptr;
}

static void ed_memset(unsigned int off, int c, unsigned int n)
{
    char *p = (char *)off;
    for (unsigned int i = 0; i < n; i++) p[i] = (char)c;
}

static void ed_memcpy(unsigned int dst, unsigned int src, unsigned int n)
{
    char *d = (char *)dst;
    const char *s = (const char *)src;
    for (unsigned int i = 0; i < n; i++) d[i] = s[i];
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
    unsigned int off = ed_alloc(len);
    ed_memcpy(off, (unsigned int)s, len);
    wasm_host_print(off, len);
}

static void print_char(char c)
{
    unsigned int off = ed_alloc(1);
    ((char *)off)[0] = c;
    wasm_host_print(off, 1);
}

/* -------------------------------------------------------------------------- */
/* Framebuffer helpers: clear, set pixel, draw text row                       */
/* -------------------------------------------------------------------------- */

#define RGBA(r, g, b) ((0xFFu << 24) | ((r) << 16) | ((g) << 8) | (b))

static unsigned int fb_pitch(void)
{
    return EDITOR_WIDTH * 4;  /* bytes per row */
}

static void fb_clear(void)
{
    /* Background: dark gray */
    unsigned int color = RGBA(0x1e, 0x1e, 0x1e);
    unsigned int total = EDITOR_WIDTH * EDITOR_HEIGHT * 4;
    for (unsigned int i = 0; i < total; i += 4) {
        *(unsigned int *)(fb_offset + i) = color;
    }
}

static void fb_pixel(int x, int y, unsigned int color)
{
    if (x < 0 || x >= EDITOR_WIDTH || y < 0 || y >= EDITOR_HEIGHT) return;
    unsigned int off = y * fb_pitch() + x * 4;
    *(unsigned int *)(fb_offset + off) = color;
}

static void fb_rect(int x, int y, int w, int h, unsigned int color)
{
    for (int row = y; row < y + h; row++)
        for (int col = x; col < x + w; col++)
            fb_pixel(col, row, color);
}

/* 5x7 bitmap font for ASCII 32-126 */
static const unsigned char font_5x7[95 * 5] = {
    /* space (0x20) */ 0,0,0,0,0,
    /* ! */ 0x7f,0x00,0x00,0x00,0x00,
    /* " */ 0x03,0x00,0x03,0x00,0x00,
    /* # */ 0x7f,0x14,0x7f,0x14,0x7f,
    /* $ */ 0x2a,0x7f,0x2a,0x3e,0x2a,
    /* % */ 0x63,0x13,0x08,0x64,0x63,
    /* & */ 0x36,0x49,0x56,0x20,0x50,
    /* ' */ 0x03,0x00,0x00,0x00,0x00,
    /* ( */ 0x3e,0x41,0x00,0x00,0x00,
    /* ) */ 0x41,0x3e,0x00,0x00,0x00,
    /* * */ 0x08,0x3e,0x1c,0x3e,0x08,
    /* + */ 0x08,0x08,0x3e,0x08,0x08,
    /* , */ 0x00,0x50,0x30,0x00,0x00,
    /* - */ 0x08,0x08,0x08,0x08,0x08,
    /* . */ 0x00,0x60,0x60,0x00,0x00,
    /* / */ 0x60,0x18,0x06,0x01,0x00,
    /* 0 */ 0x3e,0x51,0x49,0x45,0x3e,
    /* 1 */ 0x40,0x7f,0x40,0x00,0x00,
    /* 2 */ 0x72,0x49,0x49,0x49,0x46,
    /* 3 */ 0x21,0x41,0x49,0x4d,0x33,
    /* 4 */ 0x18,0x14,0x12,0x7f,0x10,
    /* 5 */ 0x2f,0x49,0x49,0x49,0x31,
    /* 6 */ 0x3c,0x4a,0x49,0x49,0x30,
    /* 7 */ 0x01,0x71,0x09,0x05,0x03,
    /* 8 */ 0x36,0x49,0x49,0x49,0x36,
    /* 9 */ 0x06,0x49,0x49,0x29,0x1e,
    /* : */ 0x00,0x36,0x36,0x00,0x00,
    /* ; */ 0x00,0x56,0x36,0x00,0x00,
    /* < */ 0x08,0x14,0x22,0x41,0x00,
    /* = */ 0x14,0x14,0x14,0x14,0x14,
    /* > */ 0x00,0x41,0x22,0x14,0x08,
    /* ? */ 0x02,0x01,0x51,0x09,0x06,
    /* @ */ 0x32,0x49,0x79,0x41,0x3e,
    /* A */ 0x7e,0x11,0x11,0x11,0x7e,
    /* B */ 0x7f,0x49,0x49,0x49,0x36,
    /* C */ 0x3e,0x41,0x41,0x41,0x22,
    /* D */ 0x7f,0x41,0x41,0x22,0x1c,
    /* E */ 0x7f,0x49,0x49,0x49,0x41,
    /* F */ 0x7f,0x09,0x09,0x09,0x01,
    /* G */ 0x3e,0x41,0x49,0x49,0x7a,
    /* H */ 0x7f,0x08,0x08,0x08,0x7f,
    /* I */ 0x00,0x41,0x7f,0x41,0x00,
    /* J */ 0x20,0x40,0x41,0x3f,0x01,
    /* K */ 0x7f,0x08,0x14,0x22,0x41,
    /* L */ 0x7f,0x40,0x40,0x40,0x40,
    /* M */ 0x7f,0x02,0x0c,0x02,0x7f,
    /* N */ 0x7f,0x04,0x08,0x10,0x7f,
    /* O */ 0x3e,0x41,0x41,0x41,0x3e,
    /* P */ 0x7f,0x09,0x09,0x09,0x06,
    /* Q */ 0x3e,0x41,0x51,0x21,0x5e,
    /* R */ 0x7f,0x09,0x19,0x29,0x46,
    /* S */ 0x26,0x49,0x49,0x49,0x32,
    /* T */ 0x01,0x01,0x7f,0x01,0x01,
    /* U */ 0x3f,0x40,0x40,0x40,0x3f,
    /* V */ 0x1f,0x20,0x40,0x20,0x1f,
    /* W */ 0x3f,0x40,0x38,0x40,0x3f,
    /* X */ 0x63,0x14,0x08,0x14,0x63,
    /* Y */ 0x07,0x08,0x70,0x08,0x07,
    /* Z */ 0x61,0x51,0x49,0x45,0x43,
    /* [ */ 0x00,0x7f,0x41,0x00,0x00,
    /* \ */ 0x01,0x06,0x18,0x60,0x00,
    /* ] */ 0x00,0x41,0x7f,0x00,0x00,
    /* ^ */ 0x04,0x02,0x01,0x02,0x04,
    /* _ */ 0x40,0x40,0x40,0x40,0x40,
    /* ` */ 0x00,0x01,0x02,0x04,0x00,
    /* a */ 0x20,0x54,0x54,0x54,0x78,
    /* b */ 0x7f,0x48,0x44,0x44,0x38,
    /* c */ 0x38,0x44,0x44,0x44,0x20,
    /* d */ 0x38,0x44,0x44,0x48,0x7f,
    /* e */ 0x38,0x54,0x54,0x54,0x18,
    /* f */ 0x08,0x7e,0x09,0x01,0x02,
    /* g */ 0x18,0xa4,0xa4,0xa4,0x7c,
    /* h */ 0x7f,0x08,0x04,0x04,0x78,
    /* i */ 0x00,0x44,0x7d,0x40,0x00,
    /* j */ 0x40,0x80,0x84,0x7d,0x00,
    /* k */ 0x7f,0x10,0x28,0x44,0x00,
    /* l */ 0x00,0x41,0x7f,0x40,0x00,
    /* m */ 0x7c,0x04,0x18,0x04,0x7c,
    /* n */ 0x7c,0x08,0x04,0x04,0x78,
    /* o */ 0x38,0x44,0x44,0x44,0x38,
    /* p */ 0xfc,0x24,0x24,0x24,0x18,
    /* q */ 0x18,0x24,0x24,0x24,0xfc,
    /* r */ 0x7c,0x08,0x04,0x04,0x08,
    /* s */ 0x48,0x54,0x54,0x54,0x24,
    /* t */ 0x04,0x3f,0x44,0x40,0x20,
    /* u */ 0x3c,0x40,0x40,0x20,0x7c,
    /* v */ 0x1c,0x20,0x40,0x20,0x1c,
    /* w */ 0x3c,0x40,0x30,0x40,0x3c,
    /* x */ 0x44,0x28,0x10,0x28,0x44,
    /* y */ 0x1c,0xa0,0xa0,0xa0,0x7c,
    /* z */ 0x44,0x64,0x54,0x4c,0x44,
    /* { */ 0x00,0x08,0x36,0x41,0x00,
    /* | */ 0x00,0x00,0x7f,0x00,0x00,
    /* } */ 0x00,0x41,0x36,0x08,0x00,
    /* ~ */ 0x08,0x04,0x08,0x10,0x08,
};

static void fb_draw_char(int bx, int by, char c, unsigned int fg, unsigned int bg)
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

static void fb_draw_string(int bx, int by, const char *s, unsigned int fg, unsigned int bg)
{
    int x = bx;
    for (int i = 0; s[i]; i++) {
        fb_draw_char(x, by, s[i], fg, bg);
        x += 6;  /* 5px + 1px spacing */
        if (x + 5 >= EDITOR_WIDTH) break;
    }
}

/* -------------------------------------------------------------------------- */
/* Editor display                                                             */
/* -------------------------------------------------------------------------- */

static void editor_render(void)
{
    unsigned int bg_color = RGBA(0x1e, 0x1e, 0x1e);
    unsigned int text_color = RGBA(0xe0, 0xe0, 0xe0);
    unsigned int cursor_color = RGBA(0xff, 0xff, 0x00);
    unsigned int title_color = RGBA(0x00, 0x66, 0xcc);
    unsigned int white = RGBA(0xff, 0xff, 0xff);

    /* Clear background */
    fb_clear();

    /* Title bar */
    fb_rect(0, 0, EDITOR_WIDTH, 20, title_color);
    fb_draw_string(5, 3, filename, white, title_color);

    /* Status bar */
    fb_rect(0, EDITOR_HEIGHT - 20, EDITOR_WIDTH, 20, title_color);
    char status[40];
    /* Build status line manually */
    int slen = 0;
    status[slen++] = 'L';
    status[slen++] = ':';
    /* row */
    int r = cursor_row + 1;
    if (r >= 100) { status[slen++] = '0' + (r / 100); r %= 100; }
    if (r >= 10) { status[slen++] = '0' + (r / 10); r %= 10; }
    status[slen++] = '0' + r;
    status[slen++] = ' ';
    status[slen++] = 'C';
    status[slen++] = ':';
    int c = cursor_col + 1;
    if (c >= 100) { status[slen++] = '0' + (c / 100); c %= 100; }
    if (c >= 10) { status[slen++] = '0' + (c / 10); c %= 10; }
    status[slen++] = '0' + c;
    /* file size */
    status[slen++] = ' ';
    status[slen++] = 'S';
    status[slen++] = ':';
    int fs = file_size;
    if (fs == 0) { status[slen++] = '0'; }
    else {
        char tmp[8]; int ti = 0;
        while (fs > 0) { tmp[ti++] = '0' + (fs % 10); fs /= 10; }
        while (ti > 0) status[slen++] = tmp[--ti];
    }
    status[slen] = '\0';
    fb_draw_string(5, EDITOR_HEIGHT - 17, status, white, title_color);

    /* Display file content starting from visible rows */
    int start_row = 0;
    int vis_rows = EDITOR_ROWS;

    for (int row = 0; row < vis_rows; row++) {
        int line_idx = start_row + row;
        int line_start = 0;
        int line_len = 0;

        /* Find line boundaries in file_buffer */
        int pos = 0;
        int cur_line = 0;
        while (pos < file_size) {
            if (cur_line == line_idx) {
                line_start = pos;
                while (pos < file_size && file_buffer[pos] != '\n') {
                    line_len++;
                    pos++;
                }
                break;
            }
            while (pos < file_size && file_buffer[pos] != '\n') pos++;
            if (pos < file_size) pos++; /* skip newline */
            cur_line++;
        }

        int y = 22 + row * 20;  /* 20px line spacing (7px font + 13px gap) */
        if (line_len > 0) {
            /* Draw text at this row */
            int x = 5;
            for (int i = 0; i < line_len && x + 5 < EDITOR_WIDTH; i++) {
                fb_draw_char(x, y, file_buffer[line_start + i], text_color, bg_color);
                x += 6;
            }
        }

        /* Draw cursor */
        if (row == cursor_row - start_row) {
            int cx = 5 + cursor_col * 6;
            int cy = y;
            fb_rect(cx, cy, 5, 14, cursor_color);
        }
    }
}

static void editor_flush_display(void)
{
    wasm_host_gui_draw(win_id, fb_offset, 0, 0, EDITOR_WIDTH, EDITOR_HEIGHT);
    wasm_host_gui_flush(win_id);
}

/* -------------------------------------------------------------------------- */
/* File operations                                                            */
/* -------------------------------------------------------------------------- */

static void editor_load(const char *path)
{
    unsigned int plen = my_strlen(path);
    unsigned int poff = ed_alloc(plen);
    ed_memcpy(poff, (unsigned int)path, plen);

    /* Copy filename for title bar */
    filename_len = (int)plen;
    if (filename_len > 63) filename_len = 63;
    for (int i = 0; i < filename_len; i++) filename[i] = path[i];
    filename[filename_len] = '\0';

    int fd = wasm_host_fs_open(poff, plen);
    if (fd < 0) {
        print_str("Editor: cannot open ");
        print_str(path);
        print_str("\n");
        file_size = 0;
        return;
    }

    unsigned int buf_off = ed_alloc(MAX_FILE_SIZE);
    int n = wasm_host_fs_read(fd, buf_off, MAX_FILE_SIZE);
    wasm_host_fs_close(fd);

    if (n > 0) {
        ed_memcpy((unsigned int)file_buffer, buf_off, (unsigned int)n);
        file_size = n;
    }

    print_str("Editor: loaded ");
    print_str(path);
    print_str(" (");
    /* print file size */
    {
        char tmp[8]; int ti = 0; int fs = n;
        if (fs == 0) tmp[ti++] = '0';
        else { while (fs > 0) { tmp[ti++] = '0' + (fs % 10); fs /= 10; } }
        for (int j = 0; j < ti; j++) print_char(tmp[j]);
    }
    print_str(" bytes)\n");
}

static void editor_save(void)
{
    unsigned int plen = my_strlen(filename);
    unsigned int poff = ed_alloc(plen);
    ed_memcpy(poff, (unsigned int)filename, plen);

    int fd = wasm_host_fs_open(poff, plen);
    if (fd < 0) {
        print_str("Editor: cannot save\n");
        return;
    }

    unsigned int buf_off = ed_alloc((unsigned int)file_size);
    ed_memcpy(buf_off, (unsigned int)file_buffer, (unsigned int)file_size);
    int n = wasm_host_fs_write(fd, buf_off, (unsigned int)file_size);
    wasm_host_fs_close(fd);

    print_str("Editor: saved (");
    {
        char tmp[8]; int ti = 0; int fs = n;
        if (fs == 0) tmp[ti++] = '0';
        else { while (fs > 0) { tmp[ti++] = '0' + (fs % 10); fs /= 10; } }
        for (int j = 0; j < ti; j++) print_char(tmp[j]);
    }
    print_str(" bytes)\n");
}

/* -------------------------------------------------------------------------- */
/* Input handling                                                             */
/* -------------------------------------------------------------------------- */

static void editor_handle_key(char c)
{
    if (c >= 32 && c < 127) {
        /* Printable character */
        if (file_size < MAX_FILE_SIZE - 1) {
            /* Insert at cursor position */
            for (int i = file_size; i > cursor_pos; i--)
                file_buffer[i] = file_buffer[i - 1];
            file_buffer[cursor_pos] = c;
            cursor_pos++;
            cursor_col++;
            file_size++;
        }
    } else if (c == '\n' || c == '\r') {
        /* Newline */
        if (file_size < MAX_FILE_SIZE - 1) {
            for (int i = file_size; i > cursor_pos; i--)
                file_buffer[i] = file_buffer[i - 1];
            file_buffer[cursor_pos] = '\n';
            cursor_pos++;
            cursor_row++;
            cursor_col = 0;
            file_size++;
        }
    } else if (c == '\b' || c == 0x7f) {
        /* Backspace */
        if (cursor_pos > 0) {
            for (int i = cursor_pos - 1; i < file_size - 1; i++)
                file_buffer[i] = file_buffer[i + 1];
            cursor_pos--;
            if (cursor_col > 0) cursor_col--;
            else { cursor_col = EDITOR_COLS - 1; cursor_row--; }
            file_size--;
        }
    } else if (c == 0x1b) {
        /* Escape - do nothing special */
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start"), used))
void _start(void)
{
    print_str("\n=== AI-ASM Text Editor v1.0 ===\n");

    /* Create editor window */
    unsigned int tlen = my_strlen("Text Editor");
    unsigned int toff = ed_alloc(tlen);
    ed_memcpy(toff, (unsigned int)"Text Editor", tlen);

    win_id = wasm_host_gui_create(20, 20, 800, 600, toff);
    if (win_id < 0) {
        print_str("Editor: failed to create window\n");
        wasm_host_exit(1);
    }

    print_str("Editor: window created (id=");
    { char tmp[4]; tmp[0] = '0' + (win_id % 10); tmp[1] = '\0'; print_str(tmp); }
    print_str(")\n");

    /* Allocate framebuffer */
    fb_offset = ed_alloc(EDITOR_WIDTH * EDITOR_HEIGHT * 4 + 4096);

    /* Load initial file if available */
    editor_load("/readme.txt");

    /* Main loop */
    unsigned int evt_off = ed_alloc(16);

    print_str("Editor: enter text. Ctrl+S to save, Ctrl+Q to quit.\n");

    for (;;) {
        /* Render display */
        editor_render();
        editor_flush_display();

        /* Poll for events */
        wasm_host_gui_poll_event(win_id, evt_off);

        /* Read keyboard input */
        int ch = wasm_host_getc();
        if (ch > 0) {
            char c = (char)ch;

            /* Ctrl+S: save */
            if (c == 0x13) {
                editor_save();
                continue;
            }
            /* Ctrl+Q: quit */
            if (c == 0x11) {
                print_str("Editor: exiting\n");
                wasm_host_exit(0);
            }

            editor_handle_key(c);
        }

        wasm_host_yield();
    }
}
