/*
 * aiasm-aarch64/examples/hello.asm
 * Example: Print "Hello, AI-ASM AArch64!" and hang
 * Demonstrates minimal use of PL011 UART driver
 * Standalone - does not depend on kernel modules
 */
.arch armv8-a

.set UARTDR,    0x00
.set UARTFR,    0x18
.set UARTIBRD,  0x24
.set UARTFBRD,  0x28
.set UARTLCR_H, 0x2c
.set UARTCR,    0x30
.set UARTICR,   0x44
.set UART_BASE, 0x09000000

.section .text.boot
.global _start
_start:
    /* UART init */
    movz    x0, #UART_BASE >> 16
    movk    x0, #UART_BASE & 0xFFFF
    mov     w1, #0
    str     w1, [x0, #UARTCR]
    mov     w1, #1
    str     w1, [x0, #UARTIBRD]
    mov     w1, #40
    str     w1, [x0, #UARTFBRD]
    mov     w1, #0x70
    str     w1, [x0, #UARTLCR_H]
    mov     w1, #0x7FF
    str     w1, [x0, #UARTICR]
    mov     w1, #0x301
    str     w1, [x0, #UARTCR]

    /* Print message */
    adrp    x4, msg_hello
    add     x4, x4, :lo12:msg_hello
    mov     x5, x4
1:
    ldrb    w0, [x5], #1
    cbz     w0, 3f
    /* Wait TX */
2:
    ldr     w1, [x0, #UARTFR]
    tbnz    w1, #5, 2b
    str     w0, [x0, #UARTDR]
    b       1b
3:
    /* Hang */
4:
    wfi
    b       4b

.section .rodata
.align 4
msg_hello:
    .asciz "Hello, AI-ASM AArch64!\r\n"
