/*
 * aiasm-aarch64/kernel/log.asm
 * Structured JSON logging system with ring buffer
 *
 * Ring buffer: 4KB circular buffer for log lines.
 * log_write appends (level JSON \n) to buffer.
 * log_read drains one line at a time for syslog WASM module.
 * log_event writes to both UART and ring buffer.
 */
.arch armv8-a

.set LOG_BUF_SIZE,  4096
.set LOG_LINE_MAX,  256

.text

/* Timestamp counter (simple incrementing) */
.bss
.align 4
log_ts:
    .skip 8

.bss
.align 4
log_itoa_buf:
    .skip 24

.bss
.align 4
log_level:
    .skip 4

/* Ring buffer state */
.bss
.align 4
log_ring_head:
    .skip 4                     /* write position (next free byte) */

.bss
.align 4
log_ring_tail:
    .skip 4                     /* read position (oldest unread byte) */

.bss
.align 12
log_ring_buffer:
    .skip LOG_BUF_SIZE

.bss
.align 4
log_line_len:
    .skip 4                     /* length of next unread line (0 = no line) */

.text

/* -----------------------------------------------------------------------------
 * Function: log_init
 * Description: Initialize logging system
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global log_init
log_init:
    stp     x29, x30, [sp, #-16]!

    /* Reset timestamp */
    adrp    x0, log_ts
    add     x0, x0, #:lo12:log_ts
    str     xzr, [x0]

    /* Default log level = INFO (1) */
    adrp    x0, log_level
    add     x0, x0, #:lo12:log_level
    mov     w1, #1
    str     w1, [x0]

    /* Clear ring buffer state */
    adrp    x0, log_ring_head
    add     x0, x0, #:lo12:log_ring_head
    str     wzr, [x0]           /* head = 0 */
    str     wzr, [x0, #4]       /* tail = 0 */
    str     wzr, [x0, #8]       /* line_len = 0 */

    /* Zero ring buffer memory */
    adrp    x0, log_ring_buffer
    add     x0, x0, #:lo12:log_ring_buffer
    mov     x1, #LOG_BUF_SIZE
1:  strb    wzr, [x0], #1
    subs    x1, x1, #1
    b.ne    1b

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_set_level
 * Description: Set minimum log level
 * Input: w0 = level (0-3)
 * Output: none
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global log_set_level
log_set_level:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, log_level
    add     x0, x0, #:lo12:log_level
    str     w0, [x0]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_get_level
 * Description: Get current log level
 * Input: none
 * Output: w0 = level
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global log_get_level
log_get_level:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, log_level
    add     x0, x0, #:lo12:log_level
    ldr     w0, [x0]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_event
 * Description: Emit structured JSON log line
 * Input: w0 = level (0=DEBUG,1=INFO,2=WARN,3=ERROR)
 *        x1 = event name string
 *        x2 = JSON data string
 * Output: none
 * Clobbered: x0-x5
 * Stack: 32 bytes
 * ----------------------------------------------------------------------------- */
.global log_event
log_event:
    stp     x29, x30, [sp, #-48]!
    stp     x19, x20, [sp, #16]
    str     x21, [sp, #32]
    mov     w19, w0             /* save log level */
    mov     x20, x1             /* save event name */
    mov     x21, x2             /* save data string */

    /* Check log level */
    adrp    x3, log_level
    add     x3, x3, #:lo12:log_level
    ldr     w3, [x3]
    cmp     w19, w3
    b.lt    log_event_done      /* below minimum level, skip */

    /* Increment timestamp */
    adrp    x3, log_ts
    add     x3, x3, #:lo12:log_ts
    ldr     x4, [x3]
    add     x4, x4, #1
    str     x4, [x3]

    /* Print: {"ts": */
    adrp    x0, msg_log_prefix
    add     x0, x0, #:lo12:msg_log_prefix
    bl      serial_puts

    /* Print timestamp number */
    adrp    x0, log_itoa_buf
    add     x0, x0, #:lo12:log_itoa_buf
    mov     x1, x0
    bl      itoa_buf
    mov     x0, x1
    bl      serial_puts

    /* Print: ,"level":" */
    adrp    x0, msg_level_pre
    add     x0, x0, #:lo12:msg_level_pre
    bl      serial_puts

    /* Print level name using saved level in w19 */
    and     w5, w19, #3
    adrp    x0, level_names
    add     x0, x0, #:lo12:level_names
    add     x0, x0, x5, lsl #3  /* level * 8 */
    ldr     x0, [x0]
    bl      serial_puts

    /* Print: ","event":" */
    adrp    x0, msg_event_pre
    add     x0, x0, #:lo12:msg_event_pre
    bl      serial_puts

    /* Print event name */
    mov     x0, x20
    bl      serial_puts

    /* Print: " */
    mov     x0, #34             /* '"' */
    bl      serial_putc

    /* Print data if non-null */
    cbz     x21, log_event_tail

    /* Print: ,"data": */
    adrp    x0, msg_data_pre
    add     x0, x0, #:lo12:msg_data_pre
    bl      serial_puts

    /* Print data */
    mov     x0, x21
    bl      serial_puts

log_event_tail:
    /* Print: }\n */
    adrp    x0, msg_log_suffix
    add     x0, x0, #:lo12:msg_log_suffix
    bl      serial_puts

    /* Also write the JSON line to ring buffer for syslog module */
    /* Reconstruct the line: {"ts":N,"level":"X","event":"Y","data":"Z"} */
    bl      _log_event_build_line

log_event_done:
    ldr     x21, [sp, #32]
    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #48
    ret

/* -----------------------------------------------------------------------------
 * _log_event_build_line — rebuild JSON log line into ring buffer
 * Uses callee-saved registers from log_event: w19=level, x20=event, x21=data
 * Called from log_event tail
 * ----------------------------------------------------------------------------- */
_log_event_build_line:
    stp     x29, x30, [sp, #-16]!

    /* Read timestamp */
    adrp    x0, log_ts
    add     x0, x0, #:lo12:log_ts
    ldr     x6, [x0]

    /* Write {"ts: prefix to ring buffer */
    adrp    x2, msg_log_prefix
    add     x2, x2, #:lo12:msg_log_prefix
    mov     x3, #7
    bl      _log_ring_write_str

    /* Write timestamp number */
    adrp    x2, log_itoa_buf
    add     x2, x2, #:lo12:log_itoa_buf
    mov     x0, x6
    mov     x1, x2
    bl      itoa_buf
    adrp    x2, log_itoa_buf
    add     x2, x2, #:lo12:log_itoa_buf
    mov     x3, x2
1:  ldrb    w4, [x3], #1
    cbnz    w4, 1b
    sub     x3, x3, x2
    sub     x3, x3, #1
    bl      _log_ring_write_str

    /* ,"level":" */
    adrp    x2, msg_level_pre
    add     x2, x2, #:lo12:msg_level_pre
    mov     x3, #10
    bl      _log_ring_write_str

    /* level name (w19 from log_event) */
    and     w5, w19, #3
    adrp    x2, level_names
    add     x2, x2, #:lo12:level_names
    add     x2, x2, x5, lsl #3
    ldr     x2, [x2]
    mov     x3, x2
1:  ldrb    w4, [x3], #1
    cbnz    w4, 1b
    sub     x3, x3, x2
    sub     x3, x3, #1
    bl      _log_ring_write_str

    /* ","event":" */
    adrp    x2, msg_event_pre
    add     x2, x2, #:lo12:msg_event_pre
    mov     x3, #11
    bl      _log_ring_write_str

    /* event name (x20 from log_event) */
    mov     x2, x20
    mov     x3, x2
1:  ldrb    w4, [x3], #1
    cbnz    w4, 1b
    sub     x3, x3, x2
    sub     x3, x3, #1
    bl      _log_ring_write_str

    /* " */
    mov     w0, #34
    bl      log_ring_write_char

    /* data if non-null (x21 from log_event) */
    cbz     x21, _log_build_data_done

    /* ,"data": */
    adrp    x2, msg_data_pre
    add     x2, x2, #:lo12:msg_data_pre
    mov     x3, #9
    bl      _log_ring_write_str

    mov     x2, x21
    mov     x3, x2
1:  ldrb    w4, [x3], #1
    cbnz    w4, 1b
    sub     x3, x3, x2
    sub     x3, x3, #1
    bl      _log_ring_write_str

_log_build_data_done:
    mov     w0, #125
    bl      log_ring_write_char
    mov     w0, #10
    bl      log_ring_write_char

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _log_ring_write_str — write a string to ring buffer
 * Input: x2 = string ptr, x3 = length
 * ----------------------------------------------------------------------------- */
_log_ring_write_str:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x2
    mov     x5, x3
    mov     x6, #0

1:  cmp     x6, x5
    b.eq    2f
    ldrb    w0, [x4, x6]
    bl      log_ring_write_char
    add     x6, x6, #1
    b       1b

2:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_ring_write_char
 * Internal: write one byte to ring buffer
 * Input: w0 = byte
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
log_ring_write_char:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, log_ring_buffer
    add     x1, x1, #:lo12:log_ring_buffer
    adrp    x2, log_ring_head
    add     x2, x2, #:lo12:log_ring_head
    ldr     w3, [x2]

    strb    w0, [x1, x3]        /* ring[head] = byte */
    add     w3, w3, #1
    and     w3, w3, #(LOG_BUF_SIZE - 1)  /* wrap */
    str     w3, [x2]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_write
 * Write a string to the ring buffer (caller must null-terminate)
 * Input: x0 = string pointer
 * Output: none
 * ----------------------------------------------------------------------------- */
.global log_write
log_write:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0             /* save string ptr */

1:  ldrb    w0, [x20], #1
    cbz     w0, 2f
    bl      log_ring_write_char
    b       1b

2:  /* Also write newline */
    mov     w0, #10             /* '\n' */
    bl      log_ring_write_char

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_read
 * Read one line from ring buffer (up to max_len bytes)
 * Input: x0 = destination buffer, w1 = max length
 * Output: w0 = bytes read, or -1 if no data
 * ----------------------------------------------------------------------------- */
.global log_read
log_read:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0             /* dest buffer */
    mov     w21, w1             /* max length */

    adrp    x1, log_ring_buffer
    add     x1, x1, #:lo12:log_ring_buffer
    adrp    x2, log_ring_head
    add     x2, x2, #:lo12:log_ring_head
    adrp    x3, log_ring_tail
    add     x3, x3, #:lo12:log_ring_tail
    ldr     w4, [x2]            /* head */
    ldr     w5, [x3]            /* tail */

    cmp     w4, w5
    b.eq    log_read_empty      /* empty */

    /* Read bytes until newline or buffer full or head reached */
    mov     x6, #0              /* byte counter */
1:  cmp     w6, w21
    b.ge    2f                  /* buffer full */
    ldrb    w0, [x1, x5]        /* ring[tail] */
    add     w5, w5, #1
    and     w5, w5, #(LOG_BUF_SIZE - 1)  /* wrap */
    strb    w0, [x20, x6]
    add     x6, x6, #1
    cmp     w0, #10             /* newline? */
    b.eq    2f
    cmp     w5, w4
    b.ne    1b

2:  /* Update tail */
    str     w5, [x3]

    mov     x0, x6
    mov     w0, w0
    b       log_read_done

log_read_empty:
    mov     w0, #-1

log_read_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_ring_available
 * Check if there is data in the ring buffer
 * Input: none
 * Output: w0 = bytes available, 0 if empty
 * ----------------------------------------------------------------------------- */
.global log_ring_available
log_ring_available:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, log_ring_head
    add     x0, x0, #:lo12:log_ring_head
    ldr     w1, [x0]            /* head */
    ldr     w2, [x0, #4]        /* tail */

    sub     w0, w1, w2
    and     w0, w0, #(LOG_BUF_SIZE - 1)  /* wrap */

    ldp     x29, x30, [sp], #16
    ret

/* Read-only data */
.section .rodata
.align 4
msg_log_prefix:
    .asciz "{\"ts\":"
msg_level_pre:
    .asciz ",\"level\":\""
msg_event_pre:
    .asciz "\",\"event\":\""
msg_data_pre:
    .asciz ",\"data\":"
msg_log_suffix:
    .asciz "}\n"

    .align 3
level_names:
    .quad s_debug
    .quad s_info
    .quad s_warn
    .quad s_error
s_debug:
    .asciz "DEBUG"
s_info:
    .asciz "INFO"
s_warn:
    .asciz "WARN"
s_error:
    .asciz "ERROR"
