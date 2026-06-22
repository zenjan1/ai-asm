/*
 * aiasm-aarch64/kernel/event.asm
 * Kernel event bus - ring buffer + auto-log
 * Holds up to 16 events
 */
.arch armv8-a

.set EVENT_MAX, 16

.text

/* Ring buffer state */
.bss
.align 4
event_buf_type:
    .skip EVENT_MAX * 4         /* 16 x u32 type IDs */
event_buf_data:
    .skip EVENT_MAX * 8         /* 16 x u64 data ptrs */
event_head:
    .skip 4
event_count:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * Function: event_init
 * Description: Initialize event ring buffer
 * Input: none
 * Output: none
 * Clobbered: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global event_init
event_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero type buffer */
    adrp    x0, event_buf_type
    add     x0, x0, #:lo12:event_buf_type
    mov     w1, #0
    mov     x2, #(EVENT_MAX * 4)
    bl      memset

    /* Zero data buffer */
    adrp    x0, event_buf_data
    add     x0, x0, #:lo12:event_buf_data
    mov     x2, #(EVENT_MAX * 8)
    bl      memset

    /* head = 0, count = 0 */
    adrp    x0, event_head
    add     x0, x0, #:lo12:event_head
    str     wzr, [x0]
    str     wzr, [x0, #4]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: event_publish
 * Description: Store event in ring buffer + auto-log
 * Input: w0 = type ID, x1 = data ptr, w2 = data len (unused v0.1)
 * Output: none
 * Clobbered: x0-x7
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global event_publish
event_publish:
    stp     x29, x30, [sp, #-16]!
    mov     x3, x1              /* x3 = data ptr */
    mov     x4, x0              /* x4 = type */

    /* x5 = &event_head */
    adrp    x5, event_head
    add     x5, x5, #:lo12:event_head

    /* w6 = head value */
    ldr     w6, [x5]

    /* Store type: event_buf_type[head] = w4 */
    adrp    x7, event_buf_type
    add     x7, x7, #:lo12:event_buf_type
    lsl     x1, x6, #2
    add     x7, x7, x1
    str     w4, [x7]

    /* Store data: event_buf_data[head] = x3 */
    adrp    x7, event_buf_data
    add     x7, x7, #:lo12:event_buf_data
    lsl     x1, x6, #3
    add     x7, x7, x1
    str     x3, [x7]

    /* head = (head + 1) % 16 */
    add     w6, w6, #1
    and     w6, w6, #(EVENT_MAX - 1)
    str     w6, [x5]

    /* count = min(count + 1, EVENT_MAX) */
    ldr     w7, [x5, #4]
    add     w7, w7, #1
    cmp     w7, #EVENT_MAX
    csel    w7, w7, w7, lt
    str     w7, [x5, #4]

    /* Auto-log: log_event(INFO, "event", x3) */
    mov     w0, #1
    adrp    x1, evt_name_pub
    add     x1, x1, #:lo12:evt_name_pub
    mov     x2, x3
    bl      log_event

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: event_dump
 * Description: Print all buffered events as JSON array
 * Input: none
 * Output: none
 * Clobbered: x0-x6
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global event_dump
event_dump:
    stp     x29, x30, [sp, #-16]!

    mov     x0, #91             /* '[' */
    bl      serial_putc

    adrp    x3, event_head
    add     x3, x3, #:lo12:event_head
    ldr     w4, [x3]            /* head */
    ldr     w5, [x3, #4]        /* count */

    /* start_idx = (head - count) % 16 */
    sub     w4, w4, w5
    and     w4, w4, #(EVENT_MAX - 1)
    mov     x6, #0              /* first flag */

1:
    cbz     w5, 2f              /* done when count == 0 */

    cbz     x6, 3f
    mov     x0, #44             /* ',' */
    bl      serial_putc
3:
    mov     x6, #1

    /* Print type number */
    adrp    x1, event_buf_type
    add     x1, x1, #:lo12:event_buf_type
    lsl     x2, x4, #2
    add     x1, x1, x2
    ldr     w0, [x1]

    adrp    x1, itoa_tmp
    add     x1, x1, #:lo12:itoa_tmp
    bl      itoa_buf
    mov     x0, x1
    bl      serial_puts

    add     w4, w4, #1
    and     w4, w4, #(EVENT_MAX - 1)
    sub     w5, w5, #1
    b       1b

2:
    mov     x0, #93             /* ']' */
    bl      serial_putc
    mov     x0, #10             /* '\n' */
    bl      serial_putc

    ldp     x29, x30, [sp], #16
    ret

.bss
.align 4
itoa_tmp:
    .skip 24

.section .rodata
.align 4
evt_name_pub:
    .asciz "event"
