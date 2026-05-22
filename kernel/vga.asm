/*
 * aiasm-aarch64/kernel/vga.asm
 * Framebuffer text mode driver for QEMU AArch64 virt
 * Uses simple VGA-compatible linear framebuffer
 */
.arch armv8-a

/* VGA text mode constants */
.set VGA_WIDTH,   80
.set VGA_HEIGHT,  25
.set VGA_COLS,    VGA_WIDTH
.set VGA_ROWS,    VGA_HEIGHT

/* Framebuffer address (QEMU virt default) */
.set FB_BASE,    0x40100000

/* Text attributes */
.set VGA_ATTR,   0x07           /* Light gray on black */

.text

.bss
.align 4
vga_cursor_x:
    .skip 1
vga_cursor_y:
    .skip 1

.text

/* -----------------------------------------------------------------------------
 * Function: vga_init
 * Description: Initialize VGA text framebuffer, clear screen
 * Input: None
 * Output: None
 * Clobbered registers: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global vga_init
vga_init:
    stp     x29, x30, [sp, #-16]!

    /* Reset cursor */
    adrp    x0, vga_cursor_x
    add     x0, x0, :lo12:vga_cursor_x
    mov     w1, #0
    strb    w1, [x0]
    strb    w1, [x0, #1]        /* cursor_y = 0 */

    /* Clear screen with spaces */
    movz    x0, #FB_BASE >> 16
    movk    x0, #FB_BASE & 0xFFFF
    mov     x1, #(VGA_COLS * VGA_ROWS)
    mov     w2, #' '
    mov     w3, #VGA_ATTR
1:
    cbz     x1, 2f
    strb    w2, [x0], #1
    strb    w3, [x0], #1
    sub     x1, x1, #1
    b       1b
2:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: vga_putc
 * Description: Print a character to VGA framebuffer at cursor
 * Input: w0 = character
 * Output: None
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global vga_putc
vga_putc:
    stp     x29, x30, [sp, #-16]!

    /* Handle newline */
    cmp     w0, #'\n'
    b.ne    1f
    adrp    x0, vga_cursor_x
    add     x0, x0, :lo12:vga_cursor_x
    ldrb    w1, [x0, #1]        /* cursor_y */
    add     w1, w1, #1
    cmp     w1, #VGA_ROWS
    b.lt    3f
    mov     w1, #0              /* Scroll/reset */
3:
    strb    w1, [x0, #1]
    mov     w1, #0
    strb    w1, [x0]
    ldp     x29, x30, [sp], #16
    ret

1:
    /* Calculate offset = (cursor_y * VGA_COLS + cursor_x) * 2 */
    adrp    x1, vga_cursor_x
    add     x1, x1, :lo12:vga_cursor_x
    ldrb    w2, [x1]            /* cursor_x */
    ldrb    w3, [x1, #1]        /* cursor_y */

    uxtw    x2, w2
    uxtw    x3, w3
    mov     x4, #VGA_COLS
    madd    x2, x3, x4, x2      /* row * cols + col */
    lsl     x2, x2, #1          /* * 2 bytes per cell */

    /* Write character and attribute */
    movz    x3, #FB_BASE >> 16
    movk    x3, #FB_BASE & 0xFFFF
    add     x3, x3, x2
    strb    w0, [x3]
    mov     w0, #VGA_ATTR
    strb    w0, [x3, #1]

    /* Advance cursor */
    add     w2, w2, #1
    cmp     w2, #VGA_COLS
    b.lt    2f
    mov     w2, #0
    add     w3, w3, #1
    cmp     w3, #VGA_ROWS
    b.lt    2f
    mov     w3, #0
2:
    strb    w2, [x1]
    strb    w3, [x1, #1]

    ldp     x29, x30, [sp], #16
    ret
