/*
 * aiasm-aarch64/kernel/serial_rx.asm
 * UART input ring buffer — non-blocking RX polling with circular buffer
 * v0.4: replaces blocking serial_getc with buffered input
 *
 * Buffer: 64 bytes, ring (head/tail pointers)
 *
 * Public API:
 *   serial_rx_init()          — initialize ring buffer
 *   serial_getc_nb(w0=>byte)  — non-blocking read, returns 0 if empty, 1 if got char
 *   serial_getc(w0=>byte)     — blocking read (waits for char)
 *   serial_rx_count()         — return bytes available in buffer
 *   serial_flush_input()      — discard all buffered input
 *   serial_poll_rx()          — poll UART and push available chars to ring buffer
 */
.arch armv8-a

.set RX_BUF_SIZE, 64
.set UART_BASE,   0x09000000
.set UARTDR,      0x00
.set UARTFR,      0x18
.set FR_RXFE,     4               /* RX FIFO empty */

.text

/* -----------------------------------------------------------------------------
 * Function: serial_rx_init
 * Description: Initialize UART RX ring buffer (head=tail=0)
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_rx_init
serial_rx_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear head and tail */
    adrp    x0, rx_head
    add     x0, x0, #:lo12:rx_head
    str     wzr, [x0]
    str     wzr, [x0, #4]         /* rx_tail = 0 */
    str     wzr, [x0, #8]         /* rx_overflow = 0 */

    /* Clear ring buffer data */
    adrp    x0, rx_buffer
    add     x0, x0, #:lo12:rx_buffer
    mov     w1, #0
    mov     x2, #RX_BUF_SIZE
    bl      memset

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_poll_rx
 * Description: Poll UART for available RX chars, push into ring buffer
 * Input: none
 * Output: x0 = number of chars read (0 if none)
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_poll_rx
serial_poll_rx:
    stp     x29, x30, [sp, #-16]!

    movz    x1, #(UART_BASE >> 16), lsl #16
    movk    x1, #:abs_g0_nc:UART_BASE
    mov     x4, #0                /* count */

poll_loop:
    /* Check UART FR for RX data available */
    ldr     w2, [x1, #UARTFR]
    tbz     w2, #FR_RXFE, 1f      /* RX FIFO has data */

    /* FIFO empty — done polling */
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

1:
    /* Read character from UART */
    ldr     w2, [x1, #UARTDR]
    and     w2, w2, #0xff

    /* Push into ring buffer */
    bl      serial_rx_push_byte
    cmp     w0, #0
    b.eq    poll_done
    add     x4, x4, #1
    b       poll_loop

poll_done:
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_rx_push_byte (internal)
 * Description: Push one byte into ring buffer
 * Input: w0 = byte
 * Output: w0 = 1 if pushed, 0 if buffer full
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
serial_rx_push_byte:
    stp     x29, x30, [sp, #-16]!
    and     w1, w0, #0xff          /* save byte */

    adrp    x2, rx_head
    add     x2, x2, #:lo12:rx_head
    ldr     w3, [x2]              /* head */
    ldr     w0, [x2, #4]          /* tail */

    /* next_head = (head + 1) % BUF_SIZE */
    add     w0, w3, #1
    and     w0, w0, #(RX_BUF_SIZE - 1)
    cmp     w0, w3
    b.eq    push_full              /* buffer full */

    /* Store byte at buffer[head] */
    adrp    x3, rx_buffer
    add     x3, x3, #:lo12:rx_buffer
    strb    w1, [x3, w3, uxtw #0]

    /* head = next_head */
    str     w0, [x2]

    mov     w0, #1
    ldp     x29, x30, [sp], #16
    ret

push_full:
    /* Increment overflow counter */
    ldr     w0, [x2, #8]
    add     w0, w0, #1
    str     w0, [x2, #8]
    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_getc_nb
 * Description: Non-blocking read one character from ring buffer
 * Input: none
 * Output: w0 = 1 and w1 = char if data available; w0 = 0 if empty
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_getc_nb
serial_getc_nb:
    stp     x29, x30, [sp, #-16]!

    adrp    x2, rx_head
    add     x2, x2, #:lo12:rx_head
    ldr     w0, [x2]              /* head */
    ldr     w3, [x2, #4]          /* tail */

    cmp     w0, w3
    b.eq    getc_nb_empty         /* head == tail => empty */

    /* Read byte from buffer[tail] */
    adrp    x1, rx_buffer
    add     x1, x1, #:lo12:rx_buffer
    ldrb    w1, [x1, w3, uxtw #0]

    /* tail = (tail + 1) % BUF_SIZE */
    add     w3, w3, #1
    and     w3, w3, #(RX_BUF_SIZE - 1)
    str     w3, [x2, #4]

    mov     w0, #1                /* success */
    ldp     x29, x30, [sp], #16
    ret

getc_nb_empty:
    mov     w0, #0                /* empty */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_getc
 * Description: Blocking read — polls RX and waits for character
 * Input: none
 * Output: w0 = character
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_getc
serial_getc:
    stp     x29, x30, [sp, #-16]!

getc_wait:
    /* Poll UART hardware first */
    bl      serial_poll_rx
    cbnz    x0, getc_have_data

    /* No data — try non-blocking ring buffer read */
    bl      serial_getc_nb
    cmp     w0, #1
    b.eq    getc_return_char

    /* Still no data — wfi and retry */
    wfi
    b       getc_wait

getc_have_data:
    /* Data was pushed to ring buffer, read it */
    bl      serial_getc_nb
    cmp     w0, #1
    b.eq    getc_return_char
    /* Should not happen since we just polled data */
    b       getc_wait

getc_return_char:
    mov     w0, w1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_gets
 * Description: Read a line (up to newline or max length) into buffer
 * Input: x0 = output buffer, w1 = max length
 * Output: x0 = number of characters read, buffer is null-terminated
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_gets
serial_gets:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0                /* save buffer */
    mov     w3, w1                /* save max length */
    mov     w4, #0                /* char count */
    sub     w3, w3, #1            /* reserve space for null terminator */
    cbz     w3, gets_done

gets_loop:
    bl      serial_getc
    cmp     w0, #10               /* newline? */
    b.eq    gets_done
    cmp     w0, #13               /* carriage return? */
    b.eq    gets_done
    cbz     w0, gets_done         /* null terminator */

    strb    w0, [x2], #1
    add     w4, w4, #1
    cmp     w4, w3
    b.lt    gets_loop

gets_done:
    strb    wzr, [x2]
    mov     w0, w4
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_rx_count
 * Description: Return number of bytes available in ring buffer
 * Input: none
 * Output: w0 = available bytes
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_rx_count
serial_rx_count:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, rx_head
    add     x1, x1, #:lo12:rx_head
    ldr     w0, [x1]              /* head */
    ldr     w2, [x1, #4]          /* tail */

    sub     w0, w0, w2
    and     w0, w0, #(RX_BUF_SIZE - 1)

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: serial_flush_input
 * Description: Discard all buffered input characters
 * Input: none
 * Output: none
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global serial_flush_input
serial_flush_input:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, rx_head
    add     x0, x0, #:lo12:rx_head
    ldr     w1, [x0]
    str     w1, [x0, #4]          /* tail = head */

    ldp     x29, x30, [sp], #16
    ret

/* ----------------------------------------------------------------------------- */
/* Ring buffer state (BSS)                                                     */
/* ----------------------------------------------------------------------------- */
.bss
.align 4
.global rx_buffer
rx_buffer:
    .skip RX_BUF_SIZE

rx_head:
    .skip 4                     /* write head index */
rx_tail:
    .skip 4                     /* read tail index */
rx_overflow:
    .skip 4                     /* overflow counter */
