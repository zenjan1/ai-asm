/*
 * aiasm-aarch64/kernel/audit.asm
 * Kernel audit log — ring buffer for security/resource/system events.
 *
 * Ring buffer: 128 events x 32 bytes = 4096 bytes.
 * Event structure (32 bytes fixed):
 *   +0:  timestamp    (4 bytes)  — CNTVCT_EL0 ticks
 *   +4:  event_type   (2 bytes)
 *   +6:  event_level  (2 bytes)
 *   +8:  proc_id      (4 bytes)
 *   +12: user_id      (4 bytes)
 *   +16: detail[0]    (4 bytes)
 *   +20: detail[1]    (4 bytes)
 *   +24: detail[2]    (4 bytes)
 *   +28: detail[3]    (4 bytes)
 *
 * Event types:
 *   AUDIT_LOGIN         0x0001
 *   AUDIT_LOGOUT        0x0002
 *   AUDIT_PERM_CHANGE   0x0003
 *   AUDIT_PROC_CREATE   0x0004
 *   AUDIT_PROC_EXIT     0x0005
 *   AUDIT_QUOTA_EXCEED  0x0006
 *   AUDIT_MEM_FAIL      0x0007
 *   AUDIT_FILE_ACCESS   0x0008
 *   AUDIT_DEVICE_ATTACH 0x0009
 *   AUDIT_KERNEL_BOOT   0x000A
 *
 * Event levels: INFO=0, WARN=1, ERROR=2
 *
 * API:
 *   audit_init()
 *   audit_log(type, level, proc_id, user_id, d0, d1, d2, d3)
 *   audit_query(buf, max_events) -> count written to buf
 *   audit_flush()
 *   audit_get_count() -> number of events in buffer
 */
.arch armv8-a

.set AUDIT_MAX,       128
.set AUDIT_EVT_SIZE,   32

/* Event types */
.set AUDIT_LOGIN,         0x0001
.set AUDIT_LOGOUT,        0x0002
.set AUDIT_PERM_CHANGE,   0x0003
.set AUDIT_PROC_CREATE,   0x0004
.set AUDIT_PROC_EXIT,     0x0005
.set AUDIT_QUOTA_EXCEED,  0x0006
.set AUDIT_MEM_FAIL,      0x0007
.set AUDIT_FILE_ACCESS,   0x0008
.set AUDIT_DEVICE_ATTACH, 0x0009
.set AUDIT_KERNEL_BOOT,   0x000A

/* Event levels */
.set AUDIT_INFO,   0
.set AUDIT_WARN,   1
.set AUDIT_ERROR,  2

/* ---------------------------------------------------------------------------
 * BSS: Audit ring buffer
 * --------------------------------------------------------------------------- */
.bss
.align 12                     /* 4096-byte aligned */
.global audit_buffer
audit_buffer:
    .skip AUDIT_MAX * AUDIT_EVT_SIZE

.global audit_head
audit_head:
    .word 0                   /* write index */

.global audit_tail
audit_tail:
    .word 0                   /* read index */

.global audit_count
audit_count:
    .word 0                   /* current number of events */

.global audit_init_done
audit_init_done:
    .word 0

.text

/* ---------------------------------------------------------------------------
 * audit_init: Zero head, tail, count
 * --------------------------------------------------------------------------- */
.global audit_init
audit_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, audit_init_done
    add     x0, x0, #:lo12:audit_init_done
    ldr     w1, [x0]
    cbnz    w1, 9f            /* already initialized */

    /* Zero head, tail, count */
    adrp    x0, audit_head
    add     x0, x0, #:lo12:audit_head
    str     wzr, [x0]         /* head = 0 */
    str     wzr, [x0, #4]     /* tail = 0 */
    str     wzr, [x0, #8]     /* count = 0 */

    /* Zero buffer */
    adrp    x0, audit_buffer
    add     x0, x0, #:lo12:audit_buffer
    mov     x1, #(AUDIT_MAX * AUDIT_EVT_SIZE)
    bl      _audit_memset

    /* Mark done */
    adrp    x0, audit_init_done
    add     x0, x0, #:lo12:audit_init_done
    mov     w1, #1
    str     w1, [x0]

9:  ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * audit_log: Write event to ring buffer
 * Input: x0=event_type, x1=event_level, x2=proc_id, x3=user_id
 *        x4=d0, x5=d1, x6=d2, x7=d3
 * --------------------------------------------------------------------------- */
.global audit_log
audit_log:
    stp     x29, x30, [sp, #-16]!
    stp     x4, x5, [sp, #-16]!
    stp     x6, x7, [sp, #-16]!

    /* Get timestamp */
    mrs     x8, cntvct_el0

    /* Calculate write position: audit_buffer + (head * 32) */
    adrp    x9, audit_buffer
    add     x9, x9, #:lo12:audit_buffer
    adrp    x10, audit_head
    add     x10, x10, #:lo12:audit_head
    ldr     w11, [x10]        /* head index */
    and     w11, w11, #(AUDIT_MAX - 1)  /* wrap */

    lsl     x12, x11, #5      /* head * 32 */
    add     x9, x9, x12       /* x9 = event pointer */

    /* Write event */
    str     w8, [x9, #0]      /* timestamp (lower 32 bits) */
    strh    w0, [x9, #4]      /* event_type */
    strh    w1, [x9, #6]      /* event_level */
    str     w2, [x9, #8]      /* proc_id */
    str     w3, [x9, #12]     /* user_id */

    ldp     x6, x7, [sp], #16
    str     w6, [x9, #16]     /* detail[0] */
    str     w7, [x9, #20]     /* detail[1] */

    ldp     x4, x5, [sp], #16
    str     w4, [x9, #24]     /* detail[2] */
    str     w5, [x9, #28]     /* detail[3] */

    /* Update head and count */
    adrp    x10, audit_head
    add     x10, x10, #:lo12:audit_head
    ldr     w11, [x10]
    add     w11, w11, #1
    str     w11, [x10]

    adrp    x10, audit_count
    add     x10, x10, #:lo12:audit_count
    ldr     w11, [x10]
    cmp     w11, #AUDIT_MAX
    b.ge    _audit_wrap
    add     w11, w11, #1
    str     w11, [x10]
    b       _audit_done

_audit_wrap:
    /* Buffer full: advance tail to make room */
    adrp    x10, audit_tail
    add     x10, x10, #:lo12:audit_tail
    ldr     w11, [x10]
    add     w11, w11, #1
    str     w11, [x10]

_audit_done:
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * audit_query: Copy events from ring buffer to caller's buffer
 * Input: x0 = output buffer, x1 = max_events
 * Output: x0 = number of events copied
 * --------------------------------------------------------------------------- */
.global audit_query
audit_query:
    stp     x29, x30, [sp, #-16]!

    mov     x9, x0            /* output buffer */
    mov     x10, x1           /* max_events */
    cbz     x10, _aq_done

    adrp    x11, audit_tail
    add     x11, x11, #:lo12:audit_tail
    ldr     w12, [x11]        /* tail */

    adrp    x13, audit_head
    add     x13, x13, #:lo12:audit_head
    ldr     w14, [x13]        /* head */

    /* Count available events */
    sub     w15, w14, w12     /* head - tail */
    cmp     x10, x15
    csel    x10, x10, x15, hi /* min(max_events, available) */

    cbz     x10, _aq_done

    mov     x17, x10          /* save count for return value */

    adrp    x16, audit_buffer
    add     x16, x16, #:lo12:audit_buffer

_aq_loop:
    cbz     x10, _aq_done
    and     w2, w12, #(AUDIT_MAX - 1)
    lsl     x3, x2, #5
    add     x4, x16, x3       /* source event */

    /* Copy 32 bytes: 8 x ldr/str */
    ldp     w5, w6, [x4, #0]
    ldp     w7, w8, [x4, #8]
    ldp     w3, w4, [x4, #16]
    ldp     w1, w2, [x4, #24]
    stp     w5, w6, [x9, #0]
    stp     w7, w8, [x9, #8]
    stp     w3, w4, [x9, #16]
    stp     w1, w2, [x9, #24]

    add     x9, x9, #AUDIT_EVT_SIZE
    add     w12, w12, #1
    sub     x10, x10, #1
    b       _aq_loop

_aq_done:
    mov     x0, x17           /* events copied */
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * audit_flush: Reset ring buffer
 * --------------------------------------------------------------------------- */
.global audit_flush
audit_flush:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, audit_head
    add     x0, x0, #:lo12:audit_head
    str     wzr, [x0]
    str     wzr, [x0, #4]
    str     wzr, [x0, #8]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * audit_get_count: Return number of events in buffer
 * Output: x0 = count
 * --------------------------------------------------------------------------- */
.global audit_get_count
audit_get_count:
    adrp    x0, audit_count
    add     x0, x0, #:lo12:audit_count
    ldr     w0, [x0]
    ret

/* ---------------------------------------------------------------------------
 * Helper: _audit_memset (zero memory)
 * Input: x0 = pointer, x1 = length
 * --------------------------------------------------------------------------- */
_audit_memset:
    cbz     x1, 2f
1:  strb    wzr, [x0], #1
    sub     x1, x1, #1
    cbnz    x1, 1b
2:  ret
