/* uart_rp4.asm: BCM2711 PL011 UART driver for Raspberry Pi 4 (v10.0) */

.global uart_rp4_init
.global uart_rp4_putc
.global uart_rp4_puts
.global uart_rp4_getc
.global uart_rp4_available
.global uart_rp4_set_baud
.global uart_rp4_flush

/* PL011 UART register offsets (BCM2711) */
.set UART0_DR,        0x000
.set UART0_RSR,       0x004
.set UART0_ECR,       0x004
.set UART0_FR,        0x018
.set UART0_ILPR,      0x020
.set UART0_IBRD,      0x024
.set UART0_FBRD,      0x028
.set UART0_LCRH,      0x02C
.set UART0_CR,        0x030
.set UART0_IFLS,      0x034
.set UART0_IMSC,      0x038
.set UART0_RIS,       0x03C
.set UART0_MIS,       0x040
.set UART0_ICR,       0x044
.set UART0_DMACR,     0x048

/* BCM2711 UART0 base */
.set UART0_BASE,      0xFE201000

/* GPIO pins for UART0 (BCM2711) */
.set UART0_TX_PIN,    14
.set UART0_RX_PIN,    15
.set UART0_TX_ALT,    4   /* ALT0 */
.set UART0_RX_ALT,    4

/* UART flags register bits */
.set UART_FR_RXFE,    (1 << 4)
.set UART_FR_TXFF,    (1 << 5)
.set UART_FR_RXFF,    (1 << 6)
.set UART_FR_TXFE,    (1 << 7)
.set UART_FR_BUSY,    (1 << 3)

/* LCRH bits */
.set UART_LCRH_FEN,   (1 << 4)
.set UART_LCRH_WLEN_8, (3 << 5)

/* CR bits */
.set UART_CR_UARTEN,  (1 << 0)
.set UART_CR_TXE,     (1 << 8)
.set UART_CR_RXE,     (1 << 9)

/* Default baud rate: 115200 */
.set UART_BAUD,       115200
.set UART_CLK,        48000000

.bss
uart_rp4_initialized:
    .skip 4
uart_rp4_baud:
    .skip 4

.text

/* uart_rp4_init() — initialize PL011 UART
   Returns: 0=success, -1=failure */
uart_rp4_init:
    /* Set GPIO 14,15 to ALT0 (UART0) */
    /* Using BCM2711 GPFSEL1 (pins 10-19) */
    mov  x0, UART0_BASE
    sub  x0, x0, #0x1000   /* Back to GPIO base */
    add  x0, x0, #0x200000 /* GPIO_BASE = 0xFE200000 */

    /* GPFSEL1: pins 10-19, pins 14,15 need ALT0 (0b100) */
    /* Pin 14: bits 12-14, Pin 15: bits 15-17 */
    ldr  w1, [x0, #0x04]   /* Read GPFSEL1 */
    mov  w2, #7
    bfi  w1, w2, #12, #3   /* Set pin 14 to ALT0 (100) */
    bfi  w1, w2, #15, #3   /* Set pin 15 to ALT0 */
    str  w1, [x0, #0x04]

    /* Disable UART before configuration */
    mov  x0, UART0_BASE
    str  wzr, [x0, #UART0_CR]

    /* Set baud rate: 115200 */
    /* IBRD = UART_CLK / (16 * baud) = 48000000 / (16 * 115200) = 26 */
    /* FBRD = round((UART_CLK / (16 * baud) - IBRD) * 64) = 8 */
    mov  w1, #26
    str  w1, [x0, #UART0_IBRD]
    mov  w1, #8
    str  w1, [x0, #UART0_FBRD]

    /* Set line control: 8-bit, enable FIFO */
    mov  w1, #(UART_LCRH_FEN | UART_LCRH_WLEN_8)
    str  w1, [x0, #UART0_LCRH]

    /* Clear interrupts */
    str  wzr, [x0, #UART0_IMSC]

    /* Clear all interrupt flags */
    mov  w1, #0x7FF
    str  w1, [x0, #UART0_ICR]

    /* Enable UART, TX, RX */
    mov  w1, #(UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE)
    str  w1, [x0, #UART0_CR]

    /* Flush FIFO */
    bl   uart_rp4_flush

    /* Mark initialized */
    adrp x0, uart_rp4_initialized
    add  x0, x0, :lo12:uart_rp4_initialized
    mov  w1, #1
    str  w1, [x0]

    adrp x0, uart_rp4_baud
    add  x0, x0, :lo12:uart_rp4_baud
    mov  w1, #UART_BAUD
    str  w1, [x0]

    mov  x0, #0
    ret

/* uart_rp4_putc(char) — send character via UART0
   x0=character */
uart_rp4_putc:
    /* Wait until TX FIFO is not full */
    mov  x1, UART0_BASE
1:  ldr  w2, [x1, #UART0_FR]
    tst  w2, #UART_FR_TXFF
    b.ne 1b

    /* Write character to data register */
    str  w0, [x1, #UART0_DR]
    ret

/* uart_rp4_puts(str_ptr) — send string via UART0
   x0=string pointer */
uart_rp4_puts:
    stp  x29, x30, [sp, #-16]!
    mov  x1, x0
1:  ldrb w0, [x1], #1
    cbz  w0, 2f
    bl   uart_rp4_putc
    b    1b
2:  ldp  x29, x30, [sp], #16
    ret

/* uart_rp4_getc() — receive character from UART0
   Returns: character in x0, or -1 if no data */
uart_rp4_getc:
    mov  x1, UART0_BASE
    ldr  w2, [x1, #UART0_FR]
    tst  w2, #UART_FR_RXFE
    b.ne uart_getc_empty

    /* Read character */
    ldr  w0, [x1, #UART0_DR]
    and  x0, x0, #0xFF
    ret

uart_getc_empty:
    mov  x0, #-1
    ret

/* uart_rp4_available() — check if data is available
   Returns: 1 if data available, 0 otherwise */
uart_rp4_available:
    mov  x1, UART0_BASE
    ldr  w2, [x1, #UART0_FR]
    tst  w2, #UART_FR_RXFE
    cset x0, ne
    ret

/* uart_rp4_set_baud(baud) — set baud rate
   x0=baud rate */
uart_rp4_set_baud:
    mov  x1, x0
    mov  x2, #48000000
    mov  x3, #16
    msub x4, x3, x1, xzr   /* x4 = 16 * baud */
    udiv x5, x2, x4        /* IBRD */
    cbz  x5, uart_baud_fail

    mov  x0, UART0_BASE

    /* Disable UART temporarily */
    str  wzr, [x0, #UART0_CR]

    /* Set IBRD */
    str  w5, [x0, #UART0_IBRD]

    /* Calculate FBRD */
    msub x6, x5, x4, x2    /* remainder */
    lsl  x6, x6, #6        /* * 64 */
    udiv x7, x6, x4        /* FBRD */
    str  w7, [x0, #UART0_FBRD]

    /* Re-enable UART */
    mov  w5, #(UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE)
    str  w5, [x0, #UART0_CR]

    /* Store baud rate */
    adrp x0, uart_rp4_baud
    add  x0, x0, :lo12:uart_rp4_baud
    str  w1, [x0]

    mov  x0, #0
    ret

uart_baud_fail:
    mov  x0, #-1
    ret

/* uart_rp4_flush() — flush TX and RX FIFOs */
uart_rp4_flush:
    mov  x0, UART0_BASE

    /* Wait for TX to complete */
1:  ldr  w1, [x0, #UART0_FR]
    tst  w1, #(UART_FR_BUSY | UART_FR_TXFE)
    cbnz w1, 1b

    /* Clear RX FIFO by reading until empty */
2:  ldr  w1, [x0, #UART0_FR]
    tst  w1, #UART_FR_RXFE
    b.ne 3f
    ldr  wzr, [x0, #UART0_DR]
    b    2b

    /* Clear all interrupts */
3:  mov  w1, #0x7FF
    str  w1, [x0, #UART0_ICR]

    ret
