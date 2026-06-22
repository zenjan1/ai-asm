/*
 * aiasm-aarch64/kernel/fb.asm
 * Framebuffer abstraction layer
 * 32-bit RGBA, 800x600 default
 * Provides basic drawing primitives
 */
.arch armv8-a

.set FB_WIDTH,     800
.set FB_HEIGHT,    600
.set FB_DEPTH,     4       /* 4 bytes per pixel (RGBA8888) */
.set FB_PITCH,     FB_WIDTH * FB_DEPTH  /* bytes per row = 3200 */
.set FB_SIZE,      FB_WIDTH * FB_HEIGHT * FB_DEPTH  /* 1920000 bytes */

.text

/* -----------------------------------------------------------------------------
 * External functions
 * ----------------------------------------------------------------------------- */
.extern virtio_gpu_transfer
.extern virtio_gpu_flush

/* -----------------------------------------------------------------------------
 * Function: fb_init
 * Initialize framebuffer (clear to black)
 * ----------------------------------------------------------------------------- */
.global fb_init
fb_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    mov     w1, #0
    ldr     x2, =FB_SIZE
    bl      _fb_memset

    /* Print banner in center of screen */
    adrp    x0, fb_welcome_msg
    add     x0, x0, #:lo12:fb_welcome_msg
    mov     x1, #300            /* x */
    mov     x2, #280            /* y */
    ldr     w3, =0xFFFFFFFF     /* white */
    bl      fb_draw_text

    /* Flush to GPU */
    ldr     x0, =FB_WIDTH
    ldr     x1, =FB_HEIGHT
    bl      virtio_gpu_flush

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_pixel
 * Draw a single pixel
 * x0 = x, x1 = y, w2 = color (RGBA)
 * Clipped to framebuffer bounds
 * ----------------------------------------------------------------------------- */
.global fb_draw_pixel
fb_draw_pixel:
    /* Bounds check */
    cmp     x0, #FB_WIDTH
    b.ge    2f
    cmp     x1, #FB_HEIGHT
    b.ge    2f

    /* Offset = y * pitch + x * 4 */
    ldr     x3, =FB_PITCH
    mul     x3, x1, x3
    add     x0, x0, x0, lsl #2  /* x * 4 */
    add     x3, x3, x0

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    add     x0, x0, x3

    str     w2, [x0]
2:
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_rect
 * Draw a filled rectangle
 * x0 = x, x1 = y, x2 = width, x3 = height, x4 = color
 * ----------------------------------------------------------------------------- */
.global fb_draw_rect
fb_draw_rect:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* x */
    mov     x9, x1              /* y */
    mov     x10, x2             /* width */
    mov     x11, x3             /* height */
    mov     w12, w4             /* color */

    /* Clip to bounds */
    cmp     x8, #FB_WIDTH
    b.ge    _fb_rect_done
    cmp     x9, #FB_HEIGHT
    b.ge    _fb_rect_done

    /* Adjust width/height if clipped */
    add     x2, x8, x10
    cmp     x2, #FB_WIDTH
    b.le    _fb_rect_w_ok
    sub     x10, x10, x2
    add     x10, x10, #FB_WIDTH
_fb_rect_w_ok:

    add     x2, x9, x11
    cmp     x2, #FB_HEIGHT
    b.le    _fb_rect_h_ok
    sub     x11, x11, x2
    add     x11, x11, #FB_HEIGHT
_fb_rect_h_ok:

    cbz     x10, _fb_rect_done
    cbz     x11, _fb_rect_done

    /* Outer loop: rows */
    mov     x5, x9              /* current y */
_fb_rect_row:
    cmp     x5, x9
    add     x5, x5, x11
    b.ge    _fb_rect_done

    /* Compute row offset: y * pitch + x * 4 */
    ldr     x6, =FB_PITCH
    mul     x6, x5, x6
    add     x7, x8, x8, lsl #2
    add     x6, x6, x7

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    add     x0, x0, x6

    /* Inner loop: pixels in row */
    mov     x7, x8
_fb_rect_pixel:
    cmp     x7, x8
    add     x7, x7, x10
    b.ge    _fb_rect_next_row

    str     w12, [x0], #4
    add     x7, x7, #1
    b       _fb_rect_pixel

_fb_rect_next_row:
    add     x5, x5, #1
    cmp     x5, x9
    add     x6, x5, x11
    b.lt    _fb_rect_row

_fb_rect_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_hline
 * Draw a horizontal line
 * x0 = x, x1 = y, x2 = length, w3 = color
 * ----------------------------------------------------------------------------- */
.global fb_draw_hline
fb_draw_hline:
    stp     x29, x30, [sp, #-16]!

    cmp     x1, #FB_HEIGHT
    b.ge    _fb_hline_done
    cmp     x0, #FB_WIDTH
    b.ge    _fb_hline_done

    ldr     x4, =FB_PITCH
    mul     x4, x1, x4
    add     x0, x0, x0, lsl #2
    add     x4, x4, x0

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    add     x0, x0, x4

    mov     x4, x2
_fb_hline_loop:
    cbz     x4, _fb_hline_done
    str     w3, [x0], #4
    sub     x4, x4, #1
    b       _fb_hline_loop

_fb_hline_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_vline
 * Draw a vertical line
 * x0 = x, x1 = y, x2 = length, w3 = color
 * ----------------------------------------------------------------------------- */
.global fb_draw_vline
fb_draw_vline:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #FB_WIDTH
    b.ge    _fb_vline_done

    ldr     x4, =FB_PITCH
    mul     x4, x1, x4
    add     x5, x0, x0, lsl #2
    add     x4, x4, x5

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    add     x0, x0, x4

    mov     x4, x2
_fb_vline_loop:
    cbz     x4, _fb_vline_done
    str     w3, [x0]
    add     x0, x0, #FB_PITCH
    sub     x4, x4, #1
    b       _fb_vline_loop

_fb_vline_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_text
 * Draw simple text (5x7 bitmap font)
 * x0 = str, x1 = x, x2 = y, w3 = color
 * ----------------------------------------------------------------------------- */
.global fb_draw_text
fb_draw_text:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* string */
    mov     x9, x1              /* x */
    mov     x10, x2             /* y */
    mov     w11, w3             /* color */
    mov     x12, x9             /* cursor x */

_fb_text_loop:
    ldrb    w0, [x8], #1
    cbz     w0, _fb_text_done

    /* Skip non-printable */
    cmp     w0, #32
    b.lt    _fb_text_next
    cmp     w0, #127
    b.ge    _fb_text_next

    /* Draw character via fb_draw_char */
    mov     x0, x12
    mov     x1, x10
    mov     x2, x9              /* wait, x9 is x, x10 is y */
    /* Fix: x0=x, x1=y, w2=char, w3=color */
    mov     x0, x12
    mov     x1, x10
    mov     w2, w0
    mov     w3, w11
    bl      fb_draw_char

_fb_text_next:
    add     x12, x12, #6        /* char width 5 + 1 space */
    b       _fb_text_loop

_fb_text_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_char
 * Draw a single character using 5x7 bitmap font
 * x0 = x, x1 = y, w2 = char, w3 = color
 * ----------------------------------------------------------------------------- */
.global fb_draw_char
fb_draw_char:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* x */
    mov     x9, x1              /* y */
    mov     w10, w2             /* char */
    mov     w11, w3             /* color */

    /* Look up font data: each char = 7 bytes, offset = (char - 32) * 7 */
    sub     w10, w10, #32
    adrp    x0, font_5x7
    add     x0, x0, #:lo12:font_5x7
    mov     x2, x0
    mov     w0, w10
    lsl     x0, x0, #3          /* * 8 */
    add     x0, x2, x0
    sub     x0, x0, x10         /* *7 = *8 - *1 */

    /* Draw 7 rows, 5 columns */
    mov     w12, #0             /* row */
_fb_char_row:
    cmp     w12, #7
    b.ge    _fb_char_done

    ldrb    w13, [x0, w12, sxtw]

    mov     w14, #0             /* col */
_fb_char_col:
    cmp     w14, #5
    b.ge    _fb_char_next_row

    /* Check bit */
    lsr     w15, w13, #(4)
    sub     w15, w15, w14
    and     w15, w15, #1
    cbz     w15, _fb_char_skip

    /* Draw pixel */
    add     x0, x8, x14
    add     x1, x9, x12
    mov     w2, w11
    bl      fb_draw_pixel

_fb_char_skip:
    add     w14, w14, #1
    b       _fb_char_col

_fb_char_next_row:
    add     w12, w12, #1
    b       _fb_char_row

_fb_char_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_clear
 * Clear framebuffer to color
 * w0 = color
 * ----------------------------------------------------------------------------- */
.global fb_clear
fb_clear:
    stp     x29, x30, [sp, #-16]!
    mov     w8, w0

    adrp    x0, fb_buffer
    add     x0, x0, #:lo12:fb_buffer
    ldr     x1, =FB_SIZE
    bl      _fb_memset_color

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_flush
 * Flush entire framebuffer to GPU
 * ----------------------------------------------------------------------------- */
.global fb_flush
fb_flush:
    ldr     x0, =FB_WIDTH
    ldr     x1, =FB_HEIGHT
    b       virtio_gpu_flush

/* -----------------------------------------------------------------------------
 * Helper: _fb_memset
 * x0 = ptr, w1 = value (byte), x2 = count
 * ----------------------------------------------------------------------------- */
_fb_memset:
    cbz     x2, 2f
1:  strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Helper: _fb_memset_color
 * x0 = ptr, w1 = color (32-bit), x2 = count in bytes
 * ----------------------------------------------------------------------------- */
_fb_memset_color:
    cbz     x2, 2f
1:  str     w1, [x0], #4
    sub     x2, x2, #4
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 12
fb_buffer:
    .skip FB_SIZE

.section .rodata
.align 4
fb_welcome_msg:
    .asciz "AI-ASM v0.7 - GUI Ready!"

/* 5x7 bitmap font (ASCII 32-126, 8 bytes each for alignment) */
.align 4
font_5x7:
    /* Space (32) */ .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    /* ! (33) */     .byte 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00
    /* " (34) */     .byte 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00
    /* # (35) */     .byte 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00, 0x00
    /* $ (36) */     .byte 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00, 0x00
    /* % (37) */     .byte 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00, 0x00
    /* & (38) */     .byte 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00, 0x00
    /* ' (39) */     .byte 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
    /* ( (40) */     .byte 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00
    /* ) (41) */     .byte 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00
    /* * (42) */     .byte 0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00, 0x00, 0x00
    /* + (43) */     .byte 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00
    /* , (44) */     .byte 0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00
    /* - (45) */     .byte 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00
    /* . (46) */     .byte 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00
    /* / (47) */     .byte 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00
    /* 0 (48) */     .byte 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00
    /* 1 (49) */     .byte 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00
    /* 2 (50) */     .byte 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00
    /* 3 (51) */     .byte 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00
    /* 4 (52) */     .byte 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00
    /* 5 (53) */     .byte 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00
    /* 6 (54) */     .byte 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00
    /* 7 (55) */     .byte 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00
    /* 8 (56) */     .byte 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00
    /* 9 (57) */     .byte 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00
    /* : (58) */     .byte 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00
    /* ; (59) */     .byte 0x00, 0x56, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00
    /* < (60) */     .byte 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00
    /* = (61) */     .byte 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00
    /* > (62) */     .byte 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00
    /* ? (63) */     .byte 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, 0x00
    /* @ (64) */     .byte 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, 0x00, 0x00
    /* A (65) */     .byte 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00
    /* B (66) */     .byte 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00
    /* C (67) */     .byte 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00
    /* D (68) */     .byte 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00
    /* E (69) */     .byte 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00
    /* F (70) */     .byte 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00
    /* G (71) */     .byte 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00, 0x00
    /* H (72) */     .byte 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, 0x00
    /* I (73) */     .byte 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00
    /* J (74) */     .byte 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00
    /* K (75) */     .byte 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00
    /* L (76) */     .byte 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00
    /* M (77) */     .byte 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, 0x00
    /* N (78) */     .byte 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00, 0x00
    /* O (79) */     .byte 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00
    /* P (80) */     .byte 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
    /* Q (81) */     .byte 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00
    /* R (82) */     .byte 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00
    /* S (83) */     .byte 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00
    /* T (84) */     .byte 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00
    /* U (85) */     .byte 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00
    /* V (86) */     .byte 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00
    /* W (87) */     .byte 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00, 0x00
    /* X (88) */     .byte 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00
    /* Y (89) */     .byte 0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00, 0x00
    /* Z (90) */     .byte 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00
    /* [ (91) */     .byte 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00
    /* \ (92) */     .byte 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00
    /* ] (93) */     .byte 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00
    /* ^ (94) */     .byte 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00
    /* _ (95) */     .byte 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00
    /* ` (96) */     .byte 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00
    /* a (97) */     .byte 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00
    /* b (98) */     .byte 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00
    /* c (99) */     .byte 0x38, 0x44, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00
    /* d (100) */    .byte 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00, 0x00
    /* e (101) */    .byte 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00
    /* f (102) */    .byte 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, 0x00, 0x00
    /* g (103) */    .byte 0x0C, 0x54, 0x54, 0x54, 0x3C, 0x00, 0x00, 0x00
    /* h (104) */    .byte 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00
    /* i (105) */    .byte 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, 0x00
    /* j (106) */    .byte 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, 0x00
    /* k (107) */    .byte 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00
    /* l (108) */    .byte 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00
    /* m (109) */    .byte 0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, 0x00, 0x00
    /* n (110) */    .byte 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00
    /* o (111) */    .byte 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00
    /* p (112) */    .byte 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00
    /* q (113) */    .byte 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00, 0x00, 0x00
    /* r (114) */    .byte 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00
    /* s (115) */    .byte 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, 0x00
    /* t (116) */    .byte 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, 0x00, 0x00
    /* u (117) */    .byte 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, 0x00
    /* v (118) */    .byte 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, 0x00
    /* w (119) */    .byte 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00
    /* x (120) */    .byte 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00
    /* y (121) */    .byte 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, 0x00
    /* z (122) */    .byte 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00
    /* { (123) */    .byte 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00
    /* | (124) */    .byte 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00
    /* } (125) */    .byte 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00
    /* ~ (126) */    .byte 0x08, 0x04, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00
