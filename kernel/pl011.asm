/*
 * aiasm-aarch64/kernel/pl011.asm
 * PL011 UART driver - polling mode, 115200 8N1
 * Base address: 0x09000000 (QEMU virt)
 */
.arch armv8-a

/* Register offsets */
.set UARTDR,    0x00
.set UARTFR,    0x18
.set UARTIBRD,  0x24
.set UARTFBRD,  0x28
.set UARTLCR_H, 0x2c
.set UARTCR,    0x30
.set UARTIMSC,  0x38
.set UARTICR,   0x44

.set UART_BASE, 0x09000000

/* FR bit masks */
.set FR_TXFF,   5               /* TX FIFO full */
.set FR_RXFE,   4               /* RX FIFO empty */

.text

/* -----------------------------------------------------------------------------
 * Function: serial_init
 * Description: Initialize PL011 UART, 115200 8N1, polling mode
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_init
serial_init:
    stp     x29, x30, [sp, #-16]!

    /* Load UART base address */
    movz    x0, #0x0900, lsl #16
    movk    x0, #:abs_g0_nc:0x09000000

    /* Disable UART */
    str     wzr, [x0, #UARTCR]

    /* Set baud: IBRD=1, FBRD=40 (for ~24MHz UART clock) */
    mov     w1, #1
    str     w1, [x0, #UARTIBRD]
    mov     w1, #40
    str     w1, [x0, #UARTFBRD]

    /* Line control: 8-bit, disable FIFO (FEN=0) */
    mov     w1, #(0x3 << 5)
    str     w1, [x0, #UARTLCR_H]

    /* Mask all interrupts */
    str     wzr, [x0, #UARTIMSC]

    /* Clear interrupt status */
    mov     w1, #0x7ff
    str     w1, [x0, #UARTICR]

    /* Enable UART + TX + RX */
    mov     w1, #(1 << 0) | (1 << 8) | (1 << 9)
    str     w1, [x0, #UARTCR]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_putc
 * Description: Send one character via UART
 * Input: w0 = character
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_putc
serial_putc:
    stp     x29, x30, [sp, #-16]!

    movz    x1, #0x0900, lsl #16
    movk    x1, #:abs_g0_nc:0x09000000

    /* Wait until TX FIFO has space */
1:
    ldr     w2, [x1, #UARTFR]
    tbnz    w2, #FR_TXFF, 1b

    /* Write character */
    str     w0, [x1, #UARTDR]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_puts
 * Description: Send null-terminated string, convert \n to \r\n
 * Input: x0 = string pointer
 * Output: none
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_puts
serial_puts:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0              /* save ptr */
1:
    ldrb    w0, [x2], #1
    cbz     w0, 3f              /* done on null */
    cmp     w0, #10             /* '\n' ? */
    b.ne    2f
    /* \n -> \r\n */
    mov     w0, #13             /* '\r' */
    bl      serial_putc
    mov     w0, #10             /* '\n' */
2:
    bl      serial_putc
    b       1b
3:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_getc
 * Description: Blocking read one character from UART
 * Input: none
 * Output: w0 = character
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_getc
serial_getc:
    stp     x29, x30, [sp, #-16]!

    movz    x1, #0x0900, lsl #16
    movk    x1, #:abs_g0_nc:0x09000000

    /* Wait until RX FIFO has data */
1:
    ldr     w0, [x1, #UARTFR]
    tbz     w0, #FR_RXFE, 2f    /* not empty -> read */
    b       1b
2:
    ldr     w0, [x1, #UARTDR]
    and     w0, w0, #0xff

    ldp     x29, x30, [sp], #16
    ret
