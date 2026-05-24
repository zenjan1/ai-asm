/*
 * aiasm-aarch64/kernel/gui.asm
 * Window management and GUI framework
 * Up to 16 windows with Z-order, title bars, basic compositing
 */
.arch armv8-a

.set GUI_MAX_WINDOWS,   16
.set WINDOW_SIZE,       64
.set WIN_X,             0       /* 2 */
.set WIN_Y,             2       /* 2 */
.set WIN_WIDTH,         4       /* 2 */
.set WIN_HEIGHT,        6       /* 2 */
.set WIN_VISIBLE,       8       /* 1 */
.set WIN_ZORDER,        9       /* 1 */
.set WIN_FB_PTR,        12      /* 4, offset to fb region */
.set WIN_TITLE,         16      /* 32 bytes */
.set WIN_ACTIVE,        48      /* 1: 1=allocated */
.set WIN_BORDER_COLOR,  49      /* 1: 0=normal, 1=focused */

/* Event queue per window (16 events × 16 bytes = 256 bytes each) */
.set EVENT_QUEUE_SIZE,  16
.set EVENT_SIZE,        16
.set WIN_EVENT_Q,       64      /* offset after window struct */
.set WIN_EVT_HEAD,      320     /* queue head index */
.set WIN_EVT_TAIL,      324     /* queue tail index */
.set WIN_EVT_COUNT,     328     /* events in queue */
.set WIN_TOTAL_SIZE,    336     /* total per window */

/* GUI event types */
.set GUI_EVT_NONE,      0
.set GUI_EVT_KEY,       1
.set GUI_EVT_MOUSE,     2

/* Title bar colors */
.set GUI_TITLE_BG,      0x0066CCFF    /* blue */
.set GUI_BG_COLOR,      0x00CCCCCC    /* light gray */
.set GUI_BORDER_COLOR,  0x00444444    /* dark gray */
.set GUI_TEXT_COLOR,    0x00FFFFFF    /* white */
.set GUI_DESKTOP_COLOR, 0x00336699    /* blue desktop */

/* Default window size */
.set WIN_DEFAULT_W,     300
.set WIN_DEFAULT_H,     200

.text

/* -----------------------------------------------------------------------------
 * External functions
 * ----------------------------------------------------------------------------- */
.extern fb_draw_rect
.extern fb_draw_text
.extern fb_draw_hline
.extern fb_draw_vline
.extern serial_puts
.extern memset

/* -----------------------------------------------------------------------------
 * Function: gui_init
 * Initialize GUI system, create desktop background
 * ----------------------------------------------------------------------------- */
.global gui_init
gui_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero window table */
    adrp    x0, gui_window_table
    add     x0, x0, #:lo12:gui_window_table
    ldr     x1, =(GUI_MAX_WINDOWS * WIN_TOTAL_SIZE)
    bl      gui_memset_zero

    /* Reset window count and top_z */
    adrp    x0, gui_window_count
    add     x0, x0, #:lo12:gui_window_count
    str     wzr, [x0]
    adrp    x0, gui_top_z
    add     x0, x0, #:lo12:gui_top_z
    str     wzr, [x0]

    /* Draw desktop background */
    mov     x0, #0
    mov     x1, #0
    ldr     x2, =800
    ldr     x3, =600
    ldr     x4, =GUI_DESKTOP_COLOR
    bl      fb_draw_rect

    /* Draw desktop title */
    adrp    x0, gui_desktop_msg
    add     x0, x0, #:lo12:gui_desktop_msg
    mov     x1, #320
    mov     x2, #10
    ldr     w3, =GUI_TEXT_COLOR
    bl      fb_draw_text

    /* Flush */
    bl      gui_flush_screen

    adrp    x0, msg_gui_ready
    add     x0, x0, #:lo12:msg_gui_ready
    bl      serial_puts

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_create_window
 * Create a new window
 * x0 = x, x1 = y, x2 = width, x3 = height, x4 = title_ptr
 * Returns w0 = window_id (0-15), or -1 if no slot
 * ----------------------------------------------------------------------------- */
.global gui_create_window
gui_create_window:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* x */
    mov     x9, x1              /* y */
    mov     x10, x2             /* width */
    mov     x11, x3             /* height */
    mov     x12, x4             /* title_ptr */

    /* Find free slot */
    adrp    x0, gui_window_table
    add     x0, x0, #:lo12:gui_window_table
    mov     w1, #0

_gcw_loop:
    cmp     w1, #GUI_MAX_WINDOWS
    b.ge    _gcw_full

    ldrb    w2, [x0, #WIN_ACTIVE]
    cbz     w2, _gcw_found

    add     x0, x0, #WIN_TOTAL_SIZE
    add     w1, w1, #1
    b       _gcw_loop

_gcw_found:
    /* Fill window struct */
    strh    w8, [x0, #WIN_X]
    strh    w9, [x0, #WIN_Y]
    strh    w10, [x0, #WIN_WIDTH]
    strh    w11, [x0, #WIN_HEIGHT]
    mov     w2, #1
    strb    w2, [x0, #WIN_VISIBLE]
    strb    w2, [x0, #WIN_ACTIVE]

    /* Z-order = current top + 1 */
    adrp    x3, gui_top_z
    add     x3, x3, #:lo12:gui_top_z
    ldr     w4, [x3]
    add     w4, w4, #1
    strb    w4, [x0, #WIN_ZORDER]
    str     w4, [x3]

    /* Copy title */
    add     x3, x0, #WIN_TITLE
    mov     x5, #0
_gcw_copy_title:
    cmp     x5, #31
    b.ge    _gcw_title_done
    ldrb    w6, [x12, x5]
    cbz     w6, _gcw_title_done
    strb    w6, [x3, x5]
    add     x5, x5, #1
    b       _gcw_copy_title

_gcw_title_done:
    strb    wzr, [x3, x5]

    /* Increment window count */
    adrp    x3, gui_window_count
    add     x3, x3, #:lo12:gui_window_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    /* Initialize event queue for this window */
    adrp    x3, gui_event_queues
    add     x3, x3, #:lo12:gui_event_queues
    mov     x4, x1
    ldr     x5, =(EVENT_QUEUE_SIZE * EVENT_SIZE)
    mul     x4, x4, x5
    add     x3, x3, x4
    str     wzr, [x3, #WIN_EVT_HEAD]
    str     wzr, [x3, #WIN_EVT_TAIL]
    str     wzr, [x3, #WIN_EVT_COUNT]

    /* Render the window */
    mov     w0, w1
    bl      gui_render_window

    /* Flush */
    bl      gui_flush_screen

    mov     w0, w1              /* return window_id */
    ldp     x29, x30, [sp], #16
    ret

_gcw_full:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_render_window
 * Render a window to the screen (title bar + content area + border)
 * x0 = window_id
 * ----------------------------------------------------------------------------- */
.global gui_render_window
grw:
gui_render_window:
    stp     x29, x30, [sp, #-16]!

    /* Get window pointer */
    adrp    x8, gui_window_table
    add     x8, x8, #:lo12:gui_window_table
    mov     x2, x0
    mov     x3, #WIN_TOTAL_SIZE
    mul     x2, x2, x3          /* window_id * WIN_TOTAL_SIZE */
    add     x8, x8, x2

    /* Check active and visible */
    ldrb    w3, [x8, #WIN_ACTIVE]
    cbz     w3, _grw_done
    ldrb    w3, [x8, #WIN_VISIBLE]
    cbz     w3, _grw_done

    /* Read window properties */
    ldrh    w0, [x8, #WIN_X]    /* win_x */
    ldrh    w1, [x8, #WIN_Y]    /* win_y */
    ldrh    w2, [x8, #WIN_WIDTH] /* win_w */
    ldrh    w3, [x8, #WIN_HEIGHT] /* win_h */

    mov     x9, x0              /* win_x */
    mov     x10, x1             /* win_y */
    mov     x11, x2             /* win_w */
    mov     x12, x3             /* win_h */

    /* Draw border (dark gray) */
    mov     x0, x9
    sub     x0, x0, #1
    mov     x1, x10
    sub     x1, x1, #1
    add     x2, x11, #2
    add     x2, x2, #1          /* w + 3 */
    mov     x3, #1              /* h = 1 (top border) */
    ldr     x4, =GUI_BORDER_COLOR
    bl      fb_draw_rect

    mov     x0, x9
    sub     x0, x0, #1
    add     x1, x10, x12
    add     x2, x11, #3
    mov     x3, #1
    ldr     x4, =GUI_BORDER_COLOR
    bl      fb_draw_rect

    mov     x0, x9
    sub     x0, x0, #1
    add     x1, x10, #1
    mov     x2, #1
    add     x3, x12, #1
    ldr     x4, =GUI_BORDER_COLOR
    bl      fb_draw_rect

    add     x0, x9, x11
    add     x1, x10, #1
    mov     x2, #1
    add     x3, x12, #1
    ldr     x4, =GUI_BORDER_COLOR
    bl      fb_draw_rect

    /* Draw title bar (blue, height 20px) */
    mov     x0, x9
    mov     x1, x10
    mov     x2, x11
    mov     x3, #20
    ldr     x4, =GUI_TITLE_BG
    bl      fb_draw_rect

    /* Draw title text */
    add     x3, x8, #WIN_TITLE
    mov     x0, x3
    add     x1, x9, #5
    add     x1, x1, #3          /* y + 3 */
    ldr     w3, =GUI_TEXT_COLOR
    bl      fb_draw_text

    /* Draw content area (light gray) */
    mov     x0, x9
    add     x1, x10, #20
    mov     x2, x11
    sub     x3, x12, #20
    ldr     x4, =GUI_BG_COLOR
    bl      fb_draw_rect

_grw_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_hide_window
 * Hide a window
 * x0 = window_id
 * ----------------------------------------------------------------------------- */
.global gui_hide_window
gui_hide_window:
    stp     x29, x30, [sp, #-16]!

    adrp    x8, gui_window_table
    add     x8, x8, #:lo12:gui_window_table
    mov     x2, x0
    mov     x3, #WIN_TOTAL_SIZE
    mul     x2, x2, x3
    add     x8, x8, x2

    mov     w3, #0
    strb    w3, [x8, #WIN_VISIBLE]

    /* Re-render all visible windows (simple compositor) */
    bl      gui_composite

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_composite
 * Re-render all visible windows in Z-order
 * ----------------------------------------------------------------------------- */
.global gui_composite
gui_composite:
    stp     x29, x30, [sp, #-16]!

    /* Sort windows by Z-order and render */
    mov     w0, #0
    mov     w1, #GUI_MAX_WINDOWS

_gc_loop:
    cmp     w0, w1
    b.ge    _gc_done

    mov     w0, w0
    bl      gui_render_window

    add     w0, w0, #1
    b       _gc_loop

_gc_done:
    bl      gui_flush_screen
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_flush_screen
 * Flush entire screen to GPU
 * ----------------------------------------------------------------------------- */
.global gui_flush_screen
gui_flush_screen:
    ldr     x0, =800
    ldr     x1, =600
    b       fb_flush

/* -----------------------------------------------------------------------------
 * Function: gui_handle_input
 * Dispatch mouse and keyboard input
 * x0 = key_code, x1 = mouse_x, x2 = mouse_y
 * Returns w0 = 0
 * ----------------------------------------------------------------------------- */
.global gui_handle_input
gui_handle_input:
    stp     x29, x30, [sp, #-16]!

    /* Save key_code (x0 is caller-saved, will be clobbered by mouse dispatch) */
    mov     x8, x0

    /* Store mouse position */
    adrp    x9, gui_mouse_x
    add     x9, x9, #:lo12:gui_mouse_x
    strh    w1, [x9]
    strh    w2, [x9, #2]

    /* Dispatch mouse event (x0=mouse_x, x1=mouse_y, x2=button=0 for now) */
    mov     x0, x1
    mov     x1, x2
    mov     x2, #0              /* no button state yet */
    bl      gui_dispatch_mouse

    /* Dispatch keyboard event */
    mov     x0, x8
    cbz     x0, _ghi_no_key     /* if key_code is zero, skip */
    bl      gui_dispatch_key
_ghi_no_key:
    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_dispatch_mouse
 * Find window under cursor, set focus
 * x0 = mouse_x, x1 = mouse_y, x2 = button_state
 * ----------------------------------------------------------------------------- */
.global gui_dispatch_mouse
gui_dispatch_mouse:
    stp     x29, x30, [sp, #-16]!

    /* If no button pressed, just return */
    cbz     w2, _gdm_done

    /* Save mouse coords for later */
    adrp    x8, gui_mouse_x
    add     x8, x8, #:lo12:gui_mouse_x
    strh    w0, [x8]
    strh    w1, [x8, #2]

    /* Iterate windows in reverse Z-order (top first) */
    adrp    x8, gui_window_table
    add     x8, x8, #:lo12:gui_window_table
    mov     w3, #GUI_MAX_WINDOWS - 1

_gdm_loop:
    cmp     w3, #0
    b.lt    _gdm_no_window

    /* Get window pointer */
    adrp    x9, gui_window_table
    add     x9, x9, #:lo12:gui_window_table
    mov     x4, x3
    mov     x5, #WIN_TOTAL_SIZE
    mul     x4, x4, x5
    add     x9, x9, x4

    /* Check active and visible */
    ldrb    w5, [x9, #WIN_ACTIVE]
    cbz     w5, _gdm_next
    ldrb    w5, [x9, #WIN_VISIBLE]
    cbz     w5, _gdm_next

    /* Check if mouse is within window bounds */
    ldrh    w5, [x9, #WIN_X]
    ldrh    w6, [x9, #WIN_Y]
    ldrh    w7, [x9, #WIN_WIDTH]
    ldrh    w4, [x9, #WIN_HEIGHT]

    /* x >= win_x */
    cmp     w0, w5
    b.lt    _gdm_next
    /* y >= win_y */
    cmp     w1, w6
    b.lt    _gdm_next
    /* x < win_x + win_w */
    add     w8, w5, w7
    cmp     w0, w8
    b.ge    _gdm_next
    /* y < win_y + win_h */
    add     w8, w6, w4
    cmp     w1, w8
    b.ge    _gdm_next

    /* Found window under cursor - set focus */
    mov     w0, w3
    bl      gui_set_focus

    /* Push mouse event to focused window */
    mov     w0, w3
    mov     w1, #GUI_EVT_MOUSE
    mov     w2, w0              /* button_state */
    bl      gui_push_event

    ldp     x29, x30, [sp], #16
    ret

_gdm_next:
    sub     w3, w3, #1
    b       _gdm_loop

_gdm_no_window:
    /* Clicked on desktop - clear focus */
    adrp    x0, focused_window
    add     x0, x0, #:lo12:focused_window
    str     wzr, [x0]

_gdm_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_dispatch_key
 * Send key event to focused window
 * x0 = key_code
 * ----------------------------------------------------------------------------- */
.global gui_dispatch_key
gui_dispatch_key:
    stp     x29, x30, [sp, #-16]!

    cbz     w0, _gdk_done       /* no key */

    adrp    x8, focused_window
    add     x8, x8, #:lo12:focused_window
    ldr     w1, [x8]
    cbz     w1, _gdk_done       /* no focused window */

    mov     w2, w0              /* key_code */
    mov     w0, w1              /* window_id */
    mov     w1, #GUI_EVT_KEY
    bl      gui_push_event

_gdk_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_set_focus
 * Set focus to a window, highlight border
 * x0 = window_id (-1 to clear focus)
 * ----------------------------------------------------------------------------- */
.global gui_set_focus
gui_set_focus:
    stp     x29, x30, [sp, #-16]!

    /* Unfocus previous window */
    adrp    x8, focused_window
    add     x8, x8, #:lo12:focused_window
    ldr     w1, [x8]
    cmp     w1, #0
    b.eq    _gsf_skip_unfocus

    /* Clear border color of old focused window */
    adrp    x9, gui_window_table
    add     x9, x9, #:lo12:gui_window_table
    mov     x2, x1
    mov     x3, #WIN_TOTAL_SIZE
    mul     x2, x2, x3
    add     x9, x9, x2
    strb    wzr, [x9, #WIN_BORDER_COLOR]
    mov     w0, w1
    bl      gui_render_window

_gsf_skip_unfocus:
    /* Set new focused window */
    str     w0, [x8]

    cbz     w0, _gsf_done

    /* Highlight border of new focused window */
    adrp    x9, gui_window_table
    add     x9, x9, #:lo12:gui_window_table
    mov     x2, x0
    mov     x3, #WIN_TOTAL_SIZE
    mul     x2, x2, x3
    add     x9, x9, x2

    mov     w1, #1
    strb    w1, [x9, #WIN_BORDER_COLOR]
    bl      gui_render_window

_gsf_done:
    bl      gui_flush_screen
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_get_focus
 * Returns w0 = focused_window_id
 * ----------------------------------------------------------------------------- */
.global gui_get_focus
gui_get_focus:
    adrp    x0, focused_window
    add     x0, x0, #:lo12:focused_window
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_push_event
 * Enqueue event to window's ring buffer
 * x0 = window_id, x1 = event_type, x2 = code/x, x3 = y
 * ----------------------------------------------------------------------------- */
.global gui_push_event
gui_push_event:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #GUI_MAX_WINDOWS
    b.ge    _gpe_invalid

    /* Get event queue base for this window */
    adrp    x8, gui_event_queues
    add     x8, x8, #:lo12:gui_event_queues
    mov     x2, x0
    ldr     x3, =(EVENT_QUEUE_SIZE * EVENT_SIZE)
    mul     x2, x2, x3
    add     x8, x8, x2

    /* Get tail index */
    ldr     w3, [x8, #WIN_EVT_TAIL]
    ldr     w4, [x8, #WIN_EVT_COUNT]
    cmp     w4, #EVENT_QUEUE_SIZE
    b.ge    _gpe_full         /* queue full, drop event */

    /* Calculate write offset */
    mov     w5, #EVENT_SIZE
    mul     w5, w3, w5
    add     x9, x8, x5

    /* Write event: type, code, x, y, timestamp */
    strb    w1, [x9, #0]      /* type */
    strb    w2, [x9, #1]      /* code */
    strh    wzr, [x9, #2]     /* padding/unused for now */

    /* Read timestamp (cycle counter) */
    mrs     x10, cntvct_el0
    str     w10, [x9, #4]     /* timestamp (low 32 bits) */

    /* Update tail and count */
    add     w3, w3, #1
    and     w3, w3, #(EVENT_QUEUE_SIZE - 1)   /* wrap */
    str     w3, [x8, #WIN_EVT_TAIL]

    ldr     w3, [x8, #WIN_EVT_COUNT]
    add     w3, w3, #1
    str     w3, [x8, #WIN_EVT_COUNT]

_gpe_full:
_gpe_invalid:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_poll_event
 * Dequeue event from window's ring buffer (for WASM host)
 * x0 = window_id, x1 = event_buf_ptr (output buffer)
 * Returns w0 = 1 if event, 0 if no event
 * ----------------------------------------------------------------------------- */
.global gui_poll_event
gui_poll_event:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #GUI_MAX_WINDOWS
    b.ge    _gpol_empty

    /* Get event queue base */
    adrp    x8, gui_event_queues
    add     x8, x8, #:lo12:gui_event_queues
    mov     x2, x0
    ldr     x3, =(EVENT_QUEUE_SIZE * EVENT_SIZE)
    mul     x2, x2, x3
    add     x8, x8, x2

    /* Check if queue has events */
    ldr     w3, [x8, #WIN_EVT_COUNT]
    cbz     w3, _gpol_empty

    /* Get head index */
    ldr     w4, [x8, #WIN_EVT_HEAD]
    mov     w5, #EVENT_SIZE
    mul     w5, w4, w5
    add     x9, x8, x5

    /* Copy event to output buffer (16 bytes) */
    ldp     x10, x11, [x9]
    stp     x10, x11, [x1]

    /* Update head and count */
    add     w4, w4, #1
    and     w4, w4, #(EVENT_QUEUE_SIZE - 1)
    str     w4, [x8, #WIN_EVT_HEAD]

    ldr     w3, [x8, #WIN_EVT_COUNT]
    sub     w3, w3, #1
    str     w3, [x8, #WIN_EVT_COUNT]

    mov     w0, #1
    ldp     x29, x30, [sp], #16
    ret

_gpol_empty:
    /* Write zeroed event */
    str     wzr, [x1]
    str     wzr, [x1, #4]
    str     wzr, [x1, #8]
    str     wzr, [x1, #12]
    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gui_window_count
 * ----------------------------------------------------------------------------- */
.global gui_count_windows
gui_count_windows:
    adrp    x0, gui_window_count
    add     x0, x0, #:lo12:gui_window_count
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * Helper: gui_memset_zero
 * x0 = ptr, x1 = count
 * ----------------------------------------------------------------------------- */
gui_memset_zero:
    cbz     x1, 2f
1:  strb    wzr, [x0], #1
    sub     x1, x1, #1
    cbnz    x1, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 8
gui_window_table:
    .skip GUI_MAX_WINDOWS * WIN_TOTAL_SIZE

.align 4
gui_window_count:
    .skip 4
gui_top_z:
    .skip 4
focused_window:
    .skip 4
gui_mouse_x:
    .skip 2
gui_mouse_y:
    .skip 2

/* Per-window event queues (16 windows × 16 events × 16 bytes = 4096 bytes) */
.align 4
gui_event_queues:
    .skip GUI_MAX_WINDOWS * EVENT_QUEUE_SIZE * EVENT_SIZE

.section .rodata
.align 4
msg_gui_ready:
    .asciz "[gui] window manager ready\n"
gui_desktop_msg:
    .asciz "AI-ASM v0.7 — Desktop"
