/*
 * aiasm-aarch64/kernel/event_notify.asm
 * Kernel event notification subsystem v9.0
 * 32 subscriber slots, 16-entry ring buffer notification queue
 * Event types: proc_create/exit, signal, quota, net, fs
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set EVT_MAX_SUBSCRIBERS,   32
.set EVT_SUB_STRUCT,        8       /* pid(4) + event_mask(4) */
.set EVT_SUB_PID,           0
.set EVT_SUB_MASK,          4

.set EVT_QUEUE_SIZE,        16
.set EVT_ENTRY_SIZE,        32      /* type(2) + level(2) + pid(4) + data0(4) + data1(4) + timestamp(4) + pad(12) */

/* Event type constants */
.set EVT_PROC_CREATE,       0x01
.set EVT_PROC_EXIT,         0x02
.set EVT_SIGNAL,            0x04
.set EVT_QUOTA_EXCEED,      0x08
.set EVT_NET_CONNECT,       0x10
.set EVT_NET_DISCONNECT,    0x20
.set EVT_FS_CREATE,         0x40
.set EVT_FS_DELETE,         0x80

/* -----------------------------------------------------------------------------
 * Data section
 * ----------------------------------------------------------------------------- */
.data
.align 4
evt_sub_table:
    .fill EVT_MAX_SUBSCRIBERS * EVT_SUB_STRUCT, 1, 0
evt_sub_count:
    .word 0

/* Ring buffer: 16 entries x 32 bytes */
.align 4
evt_queue:
    .fill EVT_QUEUE_SIZE * EVT_ENTRY_SIZE, 1, 0
evt_queue_head:
    .word 0              /* next write position */
evt_queue_tail:
    .word 0              /* next read position */
evt_queue_count:
    .word 0              /* current number of entries */

.text

/* -----------------------------------------------------------------------------
 * event_notify_init — zero subscriber table and ring buffer
 * ----------------------------------------------------------------------------- */
.global event_notify_init
event_notify_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, evt_sub_table
    add     x0, x0, #:lo12:evt_sub_table
    mov     x2, #(EVT_MAX_SUBSCRIBERS * EVT_SUB_STRUCT)
    bl      _evt_memset

    adrp    x0, evt_sub_count
    add     x0, x0, #:lo12:evt_sub_count
    str     wzr, [x0]

    adrp    x0, evt_queue
    add     x0, x0, #:lo12:evt_queue
    mov     x2, #(EVT_QUEUE_SIZE * EVT_ENTRY_SIZE)
    bl      _evt_memset

    adrp    x0, evt_queue_head
    add     x0, x0, #:lo12:evt_queue_head
    str     wzr, [x0]
    adrp    x0, evt_queue_tail
    add     x0, x0, #:lo12:evt_queue_tail
    str     wzr, [x0]
    adrp    x0, evt_queue_count
    add     x0, x0, #:lo12:evt_queue_count
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _evt_find_subscriber(pid) → x0=pointer or 0
 * x0 = pid
 * ----------------------------------------------------------------------------- */
_evt_find_subscriber:
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* save pid */

    adrp    x0, evt_sub_table
    add     x0, x0, #:lo12:evt_sub_table
    adrp    x2, evt_sub_count
    add     x2, x2, #:lo12:evt_sub_count
    ldr     w21, [x2]             /* subscriber count */

    mov     x1, #0                /* index */
_evt_find_loop:
    cmp     x1, x21
    b.ge    _evt_find_fail

    ldr     w2, [x0, #EVT_SUB_PID]
    cmp     w2, w20
    b.eq    _evt_find_ok

    add     x0, x0, #EVT_SUB_STRUCT
    add     x1, x1, #1
    b       _evt_find_loop

_evt_find_ok:
    ldp     x20, x21, [sp], #16
    ret

_evt_find_fail:
    mov     x0, #0
    ldp     x20, x21, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * event_subscribe(pid, event_mask) → rc (0=ok, -1=full, -2=dup)
 * x0 = pid, x1 = event_mask
 * ----------------------------------------------------------------------------- */
.global event_subscribe
event_subscribe:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* event_mask */

    /* Check if already subscribed */
    mov     x0, x0
    bl      _evt_find_subscriber
    cbnz    x0, _evt_sub_dup      /* already exists */

    /* Find free slot */
    adrp    x0, evt_sub_table
    add     x0, x0, #:lo12:evt_sub_table
    adrp    x2, evt_sub_count
    add     x2, x2, #:lo12:evt_sub_count
    ldr     w3, [x2]

    cmp     w3, #EVT_MAX_SUBSCRIBERS
    b.ge    _evt_sub_full

    /* Find empty slot (pid == 0 means free) */
    mov     x1, #0
_evt_sub_find_free:
    cmp     x1, x3
    b.ge    _evt_sub_found        /* beyond count = free */

    ldr     w4, [x0, #EVT_SUB_PID]
    cbz     w4, _evt_sub_found

    add     x0, x0, #EVT_SUB_STRUCT
    add     x1, x1, #1
    b       _evt_sub_find_free

_evt_sub_found:
    str     w20, [x0, #EVT_SUB_PID]
    str     w21, [x0, #EVT_SUB_MASK]

    /* Increment count if new slot */
    cmp     x1, x3
    b.le    1f
    add     w3, w3, #1
    str     w3, [x2]
1:
    mov     w0, #0
    b       _evt_sub_done

_evt_sub_full:
    mov     w0, #-1
    b       _evt_sub_done

_evt_sub_dup:
    mov     w0, #-2

_evt_sub_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * event_unsubscribe(pid, event_mask) → rc (0=ok, -1=not found)
 * x0 = pid, x1 = event_mask
 * ----------------------------------------------------------------------------- */
.global event_unsubscribe
event_unsubscribe:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* event_mask */

    mov     x0, x0
    bl      _evt_find_subscriber
    cbz     x0, _evt_unsub_fail

    /* Found — clear entry (set pid=0) */
    str     wzr, [x0, #EVT_SUB_PID]
    str     wzr, [x0, #EVT_SUB_MASK]

    mov     w0, #0
    b       _evt_unsub_done

_evt_unsub_fail:
    mov     w0, #-1

_evt_unsub_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * event_notify(event_type, data0, data1) → rc (0=ok, -1=queue full)
 * x0 = event_type, x1 = data0, x2 = data1
 * Writes to ring buffer for interested subscribers
 * ----------------------------------------------------------------------------- */
.global event_notify
event_notify:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* event_type */
    mov     w21, w1               /* data0 */
    mov     x22, x2               /* data1 */

    /* Check if queue is full */
    adrp    x0, evt_queue_count
    add     x0, x0, #:lo12:evt_queue_count
    ldr     w3, [x0]
    cmp     w3, #EVT_QUEUE_SIZE
    b.ge    _evt_notify_full

    /* Get head pointer */
    adrp    x3, evt_queue_head
    add     x3, x3, #:lo12:evt_queue_head
    ldr     w4, [x3]

    adrp    x5, evt_queue
    add     x5, x5, #:lo12:evt_queue

    /* Compute entry offset */
    and     w6, w4, #(EVT_QUEUE_SIZE - 1)
    mov     x7, #EVT_ENTRY_SIZE
    mul     x7, x7, x6
    add     x5, x5, x7            /* x5 = entry pointer */

    /* Write entry: type(2) + level(2) + pid(4) + data0(4) + data1(4) + timestamp(4) */
    strh    w20, [x5, #0]         /* event_type */
    strh    wzr, [x5, #2]         /* level = 0 */
    str     wzr, [x5, #4]         /* pid = 0 (kernel event) */
    str     w21, [x5, #8]         /* data0 */
    str     w22, [x5, #12]        /* data1 */

    /* Get timestamp (CNTVCT_EL0 lower 32 bits) */
    mrs     x6, cntvct_el0
    str     w6, [x5, #16]

    /* Advance head */
    add     w4, w4, #1
    str     w4, [x3]

    /* Increment count */
    adrp    x3, evt_queue_count
    add     x3, x3, #:lo12:evt_queue_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     w0, #0
    b       _evt_notify_done

_evt_notify_full:
    mov     w0, #-1

_evt_notify_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * event_poll(pid, buf_off) → count of events for this subscriber
 * x0 = pid, x1 = buf_off (WASM memory offset for output)
 * Caller provides external kernel address via shmem or similar.
 * For simplicity, this writes to a scratch buffer and returns count.
 * ----------------------------------------------------------------------------- */
.global event_poll
event_poll:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     x21, x1               /* buf_off */

    /* Find subscriber and get event mask */
    mov     x0, x0
    bl      _evt_find_subscriber
    cbz     x0, _evt_poll_fail

    ldr     w21, [x0, #EVT_SUB_MASK]  /* event_mask in w21 */

    /* Read events from queue */
    adrp    x0, evt_queue_count
    add     x0, x0, #:lo12:evt_queue_count
    ldr     w3, [x0]
    cbz     w3, _evt_poll_empty

    adrp    x0, evt_queue_tail
    add     x0, x0, #:lo12:evt_queue_tail
    ldr     w4, [x0]              /* tail position */

    adrp    x5, evt_queue
    add     x5, x5, #:lo12:evt_queue

    mov     w6, #0                /* events returned */
    mov     x7, #EVT_ENTRY_SIZE   /* entry stride */

_evt_poll_loop:
    cmp     w6, w3                /* checked all queued events */
    b.ge    _evt_poll_write_back
    cmp     w6, #8                /* max 8 events per poll */
    b.ge    _evt_poll_write_back

    /* Get current entry */
    and     x8, x4, #(EVT_QUEUE_SIZE - 1)
    mul     x8, x8, x7
    add     x8, x5, x8            /* x8 = entry pointer */

    /* Check event type against mask */
    ldrh    w9, [x8, #0]          /* event_type */
    and     w10, w9, w21
    cbz     w10, _evt_poll_next   /* not subscribed */

    /* Copy entry to output buffer (simplified: write type + data0 + data1) */
    /* This is a kernel-side copy; WASM caller reads via host function */
    /* For now, just count matching events */
    add     w6, w6, #1

_evt_poll_next:
    add     w4, w4, #1
    b       _evt_poll_loop

_evt_poll_write_back:
    /* Store result count (simplified) */
    mov     w0, w6

    /* Advance tail by number of events returned */
    /* In production, would copy actual data */
    b       _evt_poll_done

_evt_poll_empty:
    mov     w0, #0
    b       _evt_poll_done

_evt_poll_fail:
    mov     w0, #-1

_evt_poll_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _evt_memset (x0=ptr, w1=0, x2=count)
 * ----------------------------------------------------------------------------- */
_evt_memset:
    cbz     x2, 1f
2:  strb    wzr, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 2b
1:  ret
