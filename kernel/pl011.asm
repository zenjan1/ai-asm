/*
 * aiasm-aarch64/kernel/pl011.asm
 * PL011 UART driver for QEMU AArch64 virt machine
 * Address: 0x09000000 (QEMU virt default)
 * Baud: 115200 8N1, polling mode
 */
.arch armv8-a

/* PL011 register offsets */
.set UARTDR,    0x00
.set UARTFR,    0x18
.set UARTIBRD,  0x24
.set UARTFBRD,  0x28
.set UARTLCR_H, 0x2c
.set UARTCR,    0x30
.set UARTIMSC,  0x38
.set UARTICR,   0x44

/* PL011 base address (QEMU AArch64 virt) */
.set UART_BASE, 0x09000000

/* UARTFR bit masks */
.set FR_TXFF,   (1 << 5)      /* Transmit FIFO full */
.set FR_RXFE,   (1 << 4)      /* Receive FIFO empty */

.text

/* -----------------------------------------------------------------------------
 * Function: serial_init
 * Description: Initialize PL011 UART at 0x09000000, 115200 8N1
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_init
serial_init:
    stp     x29, x30, [sp, #-16]!

    movz    x0, #UART_BASE >> 16
    movk    x0, #UART_BASE & 0xFFFF

    /* Disable UART during configuration */
    mov     x1, #0
    str     w1, [x0, #UARTCR]

    /* Set baud rate: 115200 (IBRD=1, FBRD=40 for 24MHz clock) */
    mov     w1, #1
    str     w1, [x0, #UARTIBRD]
    mov     w1, #40
    str     w1, [x0, #UARTFBRD]

    /* Set line control: 8-bit, FIFO enable, no parity */
    mov     w1, #(0x3 << 5) | (1 << 4)  /* WLEN=8bit, FEN=1 */
    str     w1, [x0, #UARTLCR_H]

    /* Clear all interrupts */
    mov     w1, #0x7FF
    str     w1, [x0, #UARTICR]

    /* Disable all interrupts (polling mode) */
    mov     w1, #0
    str     w1, [x0, #UARTIMSC]

    /* Enable UART, TX and RX */
    mov     w1, #(1 << 0) | (1 << 8) | (1 << 9)  /* UARTEN + TXE + RXE */
    str     w1, [x0, #UARTCR]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_putc
 * Description: Send a single character via PL011 UART
 * Input: w0 = character to send
 * Output: None
 * Clobbered registers: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_putc
serial_putc:
    stp     x29, x30, [sp, #-16]!

    movz    x1, #UART_BASE >> 16
    movk    x1, #UART_BASE & 0xFFFF

    /* Wait for TX FIFO to have space */
1:
    ldr     w2, [x1, #UARTFR]
    tbz     w2, #5, 2f           /* Branch if TX FIFO not full */
    wfe
    b       1b
2:
    /* Write character */
    str     w0, [x1, #UARTDR]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_puts
 * Description: Send a null-terminated string via PL011 UART
 * Input: x0 = pointer to string
 * Output: None
 * Clobbered registers: x0, x1, x2
 * Stack: 24 bytes
 * ----------------------------------------------------------------------------- */
.global serial_puts
serial_puts:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp
    mov     x2, x0                /* Save string pointer */
1:
    ldrb    w0, [x2], #1
    cbz     w0, 3f               /* Null terminator? done */

    /* Convert \n to \r\n */
    cmp     w0, #'\n'
    b.ne    2f
    mov     w0, #'\r'
    bl      serial_putc
    mov     w0, #'\n'
    b       1b
2:
    bl      serial_putc
    b       1b
3:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_getc
 * Description: Receive a single character from PL011 UART (blocking)
 * Input: None
 * Output: w0 = received character
 * Clobbered registers: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_getc
serial_getc:
    stp     x29, x30, [sp, #-16]!

    movz    x1, #UART_BASE >> 16
    movk    x1, #UART_BASE & 0xFFFF

    /* Wait for RX FIFO to have data */
1:
    ldr     w0, [x1, #UARTFR]
    tbz     w0, #4, 2f           /* Branch if RX FIFO not empty */
    wfe
    b       1b
2:
    ldr     w0, [x1, #UARTDR]

    ldp     x29, x30, [sp], #16
    ret
