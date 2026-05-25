/*
 * aiasm-aarch64/kernel/fb.asm
 * Framebuffer abstraction layer — DOUBLE BUFFERED + HARDWARE ACCELERATED (v9.0)
 * 32-bit RGBA, 800x600 default
 * All drawing goes to back buffer; swap copies back→front and flushes to GPU
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
 * ----------------------------------------------------------------------------- */
.global fb_init
fb_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, fb_front_buffer
    add     x0, x0, #:lo12:fb_front_buffer
    mov     w1, #0
    ldr     x2, =FB_SIZE
    bl      _fb_memset

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
    mov     w1, #0
    ldr     x2, =FB_SIZE
    bl      _fb_memset

    adrp    x0, fb_welcome_msg
    add     x0, x0, #:lo12:fb_welcome_msg
    mov     x1, #300
    mov     x2, #280
    ldr     w3, =0xFFFFFFFF
    bl      fb_draw_text

    bl      fb_swap_buffers

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_swap_buffers
 * Hardware-accelerated: uses word-at-a-time memcpy
 * ----------------------------------------------------------------------------- */
.global fb_swap_buffers
fb_swap_buffers:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, fb_front_buffer
    add     x0, x0, #:lo12:fb_front_buffer
    adrp    x1, fb_back_buffer
    add     x1, x1, #:lo12:fb_back_buffer
    ldr     x2, =FB_SIZE
    bl      _fb_memcpy_fast

    ldr     x0, =FB_WIDTH
    ldr     x1, =FB_HEIGHT
    bl      virtio_gpu_flush

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
    mov     w1, #0
    ldr     x2, =FB_SIZE
    bl      _fb_memset

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_clear_back
 * ----------------------------------------------------------------------------- */
.global fb_clear_back
fb_clear_back:
    stp     x29, x30, [sp, #-16]!
    mov     w8, w0

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
    mov     w1, w8
    ldr     x2, =FB_SIZE
    bl      _fb_memset_color

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_pixel
 * ----------------------------------------------------------------------------- */
.global fb_draw_pixel
fb_draw_pixel:
    cmp     x0, #FB_WIDTH
    b.ge    2f
    cmp     x1, #FB_HEIGHT
    b.ge    2f

    ldr     x3, =FB_PITCH
    mul     x3, x1, x3
    add     x0, x0, x0, lsl #2
    add     x3, x3, x0

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
    add     x0, x0, x3

    str     w2, [x0]
2:  ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_rect (compatibility wrapper → fb_fill_rect_fast)
 * x0=x, x1=y, x2=width, x3=height, x4=color
 * ----------------------------------------------------------------------------- */
.global fb_draw_rect
fb_draw_rect:
    b       fb_fill_rect_fast

/* -----------------------------------------------------------------------------
 * Function: fb_fill_rect_fast
 * Hardware-accelerated filled rectangle to BACK buffer
 * Uses 8-byte stores (2 pixels at once)
 * x0 = x, x1 = y, x2 = width, x3 = height, x4 = color
 * ----------------------------------------------------------------------------- */
.global fb_fill_rect_fast
fb_fill_rect_fast:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     x20, x0             /* save x */
    mov     x8, x0              /* x */
    mov     x9, x1              /* y */
    mov     x10, x2             /* width */
    mov     x11, x3             /* height */
    mov     w12, w4             /* color */

    cmp     x8, #FB_WIDTH
    b.ge    _fbr_done
    cmp     x9, #FB_HEIGHT
    b.ge    _fbr_done

    /* Clip right */
    add     x2, x8, x10
    cmp     x2, #FB_WIDTH
    b.le    _fbr_w_ok
    sub     x10, x10, x2
    add     x10, x10, #FB_WIDTH
_fbr_w_ok:
    /* Clip bottom */
    add     x2, x9, x11
    cmp     x2, #FB_HEIGHT
    b.le    _fbr_h_ok
    sub     x11, x11, x2
    add     x11, x11, #FB_HEIGHT
_fbr_h_ok:

    cbz     x10, _fbr_done
    cbz     x11, _fbr_done

    /* Expand color to 64-bit (duplicate 32-bit color to both halves for 2-pixel stores) */
    mov     x21, x12            /* x21 = color (zero-extended since w12 is 32-bit subreg) */
    lsl     x21, x21, #32       /* x21 = color << 32 */
    orr     x21, x21, x12       /* x21 = color | (color << 32) = two copies */

    /* Compute row start: y * pitch + x * 4 */
    ldr     x6, =FB_PITCH
    mul     x6, x9, x6
    add     x7, x20, x20, lsl #2
    add     x6, x6, x7

    adrp    x7, fb_back_buffer
    add     x7, x7, #:lo12:fb_back_buffer
    add     x7, x7, x6

_fbr_row:
    cbz     x11, _fbr_done

    mov     x14, x10            /* pixels this row */
    mov     x0, x7              /* dest pointer */

_fbr_loop:
    cmp     x14, #2
    b.lt    _fbr_tail

    str     x21, [x0], #8
    sub     x14, x14, #2
    b       _fbr_loop

_fbr_tail:
    cbz     x14, _fbr_next

    str     w12, [x0]

_fbr_next:
    add     x7, x7, #FB_PITCH
    sub     x11, x11, #1
    b       _fbr_row

_fbr_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_blit
 * Blit rectangular block from source buffer to BACK buffer
 * x0 = dst_x, x1 = dst_y, x2 = src_x, x3 = src_y, x4 = width, x5 = height, x6 = src_ptr
 * ----------------------------------------------------------------------------- */
.global fb_blit
fb_blit:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!

    mov     x20, x0             /* dst_x */
    mov     x21, x1             /* dst_y */
    mov     x22, x4             /* width */
    mov     x23, x5             /* height */
    mov     x8, x6              /* src pointer */

    /* Clip to framebuffer */
    cmp     x20, #FB_WIDTH
    b.ge    _blit_done
    cmp     x21, #FB_HEIGHT
    b.ge    _blit_done

    /* Compute dst row offset: dst_y * pitch + dst_x * 4 */
    ldr     x9, =FB_PITCH
    mul     x9, x21, x9
    add     x10, x20, x20, lsl #2
    add     x9, x9, x10

    adrp    x10, fb_back_buffer
    add     x10, x10, #:lo12:fb_back_buffer
    add     x10, x10, x9

    /* src row offset = src_x * 4 (we advance by src_y rows) */
    /* src_y is in x3, each "row" in source = width * 4 */
    lsl     x11, x22, #2        /* src row pitch = width * 4 */
    mul     x11, x11, x3        /* src_y * src_pitch */
    add     x8, x8, x11         /* advance to src_y row */
    lsl     x12, x2, #2         /* src_x * 4 */
    add     x8, x8, x12         /* advance to src_x column */

_blit_row:
    cbz     x23, _blit_done

    /* Copy one row: width * 4 bytes */
    mov     x0, x10             /* dst */
    mov     x1, x8              /* src */
    lsl     x2, x22, #2         /* byte count = width * 4 */
    bl      _fb_memcpy_fast

    add     x10, x10, #FB_PITCH  /* next dst row */
    add     x8, x8, x11          /* next src row */
    sub     x23, x23, #1
    b       _blit_row

_blit_done:
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_copy_area
 * Copy rectangular area within back buffer
 * x0 = src_x, x1 = src_y, x2 = dst_x, x3 = dst_y, x4 = width, x5 = height
 * ----------------------------------------------------------------------------- */
.global fb_copy_area
fb_copy_area:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!

    /* Compute src offset */
    ldr     x8, =FB_PITCH
    mul     x8, x1, x8
    add     x9, x0, x0, lsl #2
    add     x8, x8, x9

    /* Compute dst offset */
    ldr     x9, =FB_PITCH
    mul     x9, x3, x9
    add     x10, x2, x2, lsl #2
    add     x9, x9, x10

    adrp    x10, fb_back_buffer
    add     x10, x10, #:lo12:fb_back_buffer
    mov     x20, x10
    add     x10, x10, x9
    add     x20, x20, x8

    mov     x21, x5             /* height */

_copy_row:
    cbz     x21, _copy_done

    mov     x0, x10
    mov     x1, x20
    lsl     x2, x4, #2          /* width * 4 */
    bl      _fb_memcpy_fast

    add     x10, x10, #FB_PITCH
    add     x20, x20, #FB_PITCH
    sub     x21, x21, #1
    b       _copy_row

_copy_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_hline
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

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
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

    adrp    x0, fb_back_buffer
    add     x0, x0, #:lo12:fb_back_buffer
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
 * ----------------------------------------------------------------------------- */
.global fb_draw_text
fb_draw_text:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0
    mov     x9, x1
    mov     x10, x2
    mov     w11, w3
    mov     x12, x9

_fb_text_loop:
    ldrb    w0, [x8], #1
    cbz     w0, _fb_text_done

    cmp     w0, #32
    b.lt    _fb_text_next
    cmp     w0, #127
    b.ge    _fb_text_next

    mov     x0, x12
    mov     x1, x10
    mov     w2, w0
    mov     w3, w11
    bl      fb_draw_char

_fb_text_next:
    add     x12, x12, #6
    b       _fb_text_loop

_fb_text_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: fb_draw_char
 * ----------------------------------------------------------------------------- */
.global fb_draw_char
fb_draw_char:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0
    mov     x9, x1
    mov     w10, w2
    mov     w11, w3

    sub     w10, w10, #32
    adrp    x0, font_5x7
    add     x0, x0, #:lo12:font_5x7
    mov     x2, x0
    mov     w0, w10
    lsl     x0, x0, #3
    add     x0, x2, x0
    sub     x0, x0, x10

    mov     w12, #0
_fb_char_row:
    cmp     w12, #7
    b.ge    _fb_char_done

    ldrb    w13, [x0, w12, sxtw]

    mov     w14, #0
_fb_char_col:
    cmp     w14, #5
    b.ge    _fb_char_next_row

    lsr     w15, w13, #(4)
    sub     w15, w15, w14
    and     w15, w15, #1
    cbz     w15, _fb_char_skip

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
 * Function: fb_flush
 * ----------------------------------------------------------------------------- */
.global fb_flush
fb_flush:
    ldr     x0, =FB_WIDTH
    ldr     x1, =FB_HEIGHT
    b       virtio_gpu_flush

/* -----------------------------------------------------------------------------
 * Helper: _fb_memset (byte)
 * ----------------------------------------------------------------------------- */
_fb_memset:
    cbz     x2, 2f
1:  strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Helper: _fb_memset_color (32-bit)
 * ----------------------------------------------------------------------------- */
_fb_memset_color:
    cbz     x2, 2f
1:  str     w1, [x0], #4
    sub     x2, x2, #1
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Helper: _fb_memcpy_fast
 * Hardware-accelerated memcpy: 8 bytes per iteration
 * x0 = dest, x1 = src, x2 = byte count
 * ----------------------------------------------------------------------------- */
_fb_memcpy_fast:
    stp     x29, x30, [sp, #-16]!
    cbz     x2, 9f

    /* Process 8 bytes at a time */
    mov     x3, x2, lsr #3      /* x3 = count / 8 */
    and     x4, x2, #7          /* x4 = count % 8 */

    cbz     x3, _fbm_tail

_fbm_loop:
    ldr     x5, [x1], #8
    str     x5, [x0], #8
    subs    x3, x3, #1
    b.ne    _fbm_loop

_fbm_tail:
    cbz     x4, 9f

_fbm_byte:
    ldrb    w5, [x1], #1
    strb    w5, [x0], #1
    subs    x4, x4, #1
    b.ne    _fbm_byte

9:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _fb_memcpy (legacy, calls fast version)
 * ----------------------------------------------------------------------------- */
_fb_memcpy:
    b       _fb_memcpy_fast

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 12
fb_front_buffer:
    .skip FB_SIZE

.align 12
fb_back_buffer:
    .skip FB_SIZE

.section .rodata
.align 4
fb_welcome_msg:
    .asciz "AI-ASM v9.0 - GPU Accelerated!"

/* 5x7 bitmap font (ASCII 32-126) */
.align 4
font_5x7:
    .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00
    .byte 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00, 0x00
    .byte 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00, 0x00
    .byte 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00, 0x00
    .byte 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00, 0x00
    .byte 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00
    .byte 0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00, 0x00, 0x00
    .byte 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00
    .byte 0x00, 0x50, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00
    .byte 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x00
    .byte 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00
    .byte 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00
    .byte 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00
    .byte 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00
    .byte 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00
    .byte 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00
    .byte 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00
    .byte 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00
    .byte 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00
    .byte 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00
    .byte 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x56, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00
    .byte 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00, 0x00
    .byte 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, 0x00, 0x00
    .byte 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00
    .byte 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00
    .byte 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00
    .byte 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00
    .byte 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00
    .byte 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00
    .byte 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00, 0x00
    .byte 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00
    .byte 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00
    .byte 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00
    .byte 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, 0x00
    .byte 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00, 0x00
    .byte 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00
    .byte 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
    .byte 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00
    .byte 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00
    .byte 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00
    .byte 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00
    .byte 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00
    .byte 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00
    .byte 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00, 0x00
    .byte 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00
    .byte 0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00, 0x00
    .byte 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00
    .byte 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00
    .byte 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00
    .byte 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00
    .byte 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00, 0x00
    .byte 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00
    .byte 0x38, 0x44, 0x44, 0x44, 0x20, 0x00, 0x00, 0x00
    .byte 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00, 0x00
    .byte 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00, 0x00
    .byte 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, 0x00, 0x00
    .byte 0x0C, 0x54, 0x54, 0x54, 0x3C, 0x00, 0x00, 0x00
    .byte 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00
    .byte 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, 0x00, 0x00
    .byte 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, 0x00, 0x00
    .byte 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00
    .byte 0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, 0x00, 0x00
    .byte 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00
    .byte 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00
    .byte 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00
    .byte 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00, 0x00, 0x00
    .byte 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00
    .byte 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x00, 0x00
    .byte 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, 0x00, 0x00
    .byte 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, 0x00, 0x00
    .byte 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00, 0x00
    .byte 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00, 0x00
    .byte 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00
    .byte 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, 0x00, 0x00
    .byte 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00, 0x00
    .byte 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00
    .byte 0x08, 0x04, 0x08, 0x10, 0x08, 0x00, 0x00, 0x00
