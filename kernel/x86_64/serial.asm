/* x86_64 16550 UART serial driver */

.equ COM1_BASE, 0x3f8
.equ COM2_BASE, 0x2f8

.equ UART_RX, 0
.equ UART_TX, 0
.equ UART_DLL, 0
.equ UART_DLM, 1
.equ UART_IER, 1
.equ UART_IIR, 2
.equ UART_FCR, 2
.equ UART_LCR, 3
.equ UART_MCR, 4
.equ UART_LSR, 5
.equ UART_MSR, 6
.equ UART_SCR, 7

.equ UART_LSR_DATA_READY, 0x01
.equ UART_LSR_TX_EMPTY, 0x20

/* Initialize serial port */
.section .text
.global serial_init
serial_init:
    push %rax
    push %rdx

    /* Disable interrupts */
    mov $0, %al
    mov %rdi, %rdx
    add $UART_IER, %rdx
    out %al, %dx

    /* Enable DLAB (set baud rate divisor) */
    mov $0x80, %al
    mov %rdi, %rdx
    add $UART_LCR, %rdx
    out %al, %dx

    /* Set divisor for 115200 baud (DLL=1, DLM=0) */
    mov $1, %al
    mov %rdi, %rdx
    add $UART_DLL, %rdx
    out %al, %dx
    mov $0, %al
    mov %rdi, %rdx
    add $UART_DLM, %rdx
    out %al, %dx

    /* 8 bits, no parity, one stop bit */
    mov $0x03, %al
    mov %rdi, %rdx
    add $UART_LCR, %rdx
    out %al, %dx

    /* Enable FIFO, clear them, 14-byte threshold */
    mov $0xC7, %al
    mov %rdi, %rdx
    add $UART_FCR, %rdx
    out %al, %dx

    /* Enable interrupts */
    mov $0x01, %al
    mov %rdi, %rdx
    add $UART_IER, %rdx
    out %al, %dx

    pop %rdx
    pop %rax
    ret

/* Write character to serial */
.section .text
.global serial_putc
serial_putc:
    push %rdx
    push %rsi
    mov %rdi, %rdx
serial_putc_wait:
    add $UART_LSR, %rdx
    in %dx, %al
    test $UART_LSR_TX_EMPTY, %al
    jz serial_putc_wait
    mov %sil, %al
    mov %rdi, %rdx
    add $UART_TX, %rdx
    out %al, %dx
    pop %rsi
    pop %rdx
    ret

/* Read character from serial (blocking) */
.section .text
.global serial_getc
serial_getc:
    push %rdx
serial_getc_wait:
    mov %rdi, %rdx
    add $UART_LSR, %rdx
    in %dx, %al
    test $UART_LSR_DATA_READY, %al
    jz serial_getc_wait
    mov %rdi, %rdx
    add $UART_RX, %rdx
    in %dx, %al
    pop %rdx
    ret

/* Check if data is available */
.section .text
.global serial_data_ready
serial_data_ready:
    push %rdx
    mov %rdi, %rdx
    add $UART_LSR, %rdx
    in %dx, %al
    and $UART_LSR_DATA_READY, %al
    pop %rdx
    ret
