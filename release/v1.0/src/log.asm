/*
 * aiasm-aarch64/kernel/log.asm
 * Structured JSON logging system - single-line JSON output via UART
 */
.arch armv8-a

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

log_event_done:
    ldr     x21, [sp, #32]
    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #48
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
