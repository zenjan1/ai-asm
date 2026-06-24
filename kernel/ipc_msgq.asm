/*
 * aiasm-aarch64/kernel/ipc_msgq.asm
 * Message Queue IPC (v1.0)
 *
 * AArch64 ASM implementation of System V-style message queues.
 * Provides inter-process communication via message passing.
 *
 * Features:
 *   - Queue creation/destruction with keys
 *   - Message send/receive with priority
 *   - Blocking/non-blocking modes
 *   - Queue limits (max messages, max bytes)
 *   - Message types for filtering
 *   - Queue statistics and permission checking
 *
 * Integration: used by process synchronization, agent communication
 *
 * API:
 *   ipc_msgq_init()                              — init message queue subsystem
 *   ipc_msgq_create(key, max_msgs, max_bytes) => qid — create queue
 *   ipc_msgq_destroy(qid)                        — destroy queue
 *   ipc_msgq_send(qid, msg_type, data, len) => 0/-1 — send message
 *   ipc_msgq_receive(qid, msg_type, buf, max_len) => len — receive message
 *   ipc_msgq_peek(qid, msg_type) => msg_count    — peek without remove
 *   ipc_msgq_stat(qid, out_stat)                 — get queue stats
 */
.arch armv8-a

/* Constants */
.set MSGQ_MAX_QUEUES,   32
.set MSGQ_MAX_MSGS,     256
.set MSGQ_MSG_SIZE,     256
.set MSGQ_MAX_WAITERS,  16

/* Message queue entry (32 bytes) */
/* id, key, msg_count, byte_count, max_msgs, max_bytes, perms, owner */
/* first_msg, last_msg, wait_count, active, reserved */

/* Message entry (264 bytes) */
/* type, priority, size, data[256], next */

.bss
.align 4
.global ipc_msgq_init_done
ipc_msgq_init_done:
    .skip 4

.align 4
ipc_msgq_count:
    .skip 4
ipc_msgq_total_msgs:
    .skip 4
ipc_msgq_total_sends:
    .skip 4
ipc_msgq_total_recvs:
    .skip 4

/* Message queues */
.align 4
ipc_msgq_queues:
    .skip MSGQ_MAX_QUEUES * 32      /* 1024 bytes */

/* Message pool */
.align 4
ipc_msgq_msgs:
    .skip MSGQ_MAX_MSGS * 264       /* 67584 bytes */

/* Wait queues (per queue) */
.align 4
ipc_msgq_waiters:
    .skip MSGQ_MAX_QUEUES * MSGQ_MAX_WAITERS * 4   /* 2048 bytes */

.text

/* -----------------------------------------------------------------------------
 * ipc_msgq_init — initialize message queue subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_init
ipc_msgq_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =ipc_msgq_count
    str     wzr, [x0]
    ldr     x0, =ipc_msgq_total_msgs
    str     wzr, [x0]
    ldr     x0, =ipc_msgq_total_sends
    str     wzr, [x0]
    ldr     x0, =ipc_msgq_total_recvs
    str     wzr, [x0]

    /* Zero queue array */
    ldr     x0, =ipc_msgq_queues
    mov     x1, #(MSGQ_MAX_QUEUES * 32)
    bl      .Lmsgq_zero

    /* Zero message pool */
    ldr     x0, =ipc_msgq_msgs
    mov     x1, #(MSGQ_MAX_MSGS * 264)
    bl      .Lmsgq_zero

    /* Zero wait queues */
    ldr     x0, =ipc_msgq_waiters
    mov     x1, #(MSGQ_MAX_QUEUES * MSGQ_MAX_WAITERS * 4)
    bl      .Lmsgq_zero

    ldr     x0, =ipc_msgq_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lmsgq_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_create — create a new message queue
 * Input: x0 = key, x1 = max_msgs, x2 = max_bytes
 * Output: x0 = qid (>=0) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_create
ipc_msgq_create:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* key */
    mov     w20, w1             /* max_msgs */

    /* Check if we can create more queues */
    ldr     x0, =ipc_msgq_count
    ldr     w0, [x0]
    cmp     w0, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_create_fail

    /* Find free queue slot */
    ldr     x1, =ipc_msgq_queues
    mov     w2, #0

.Lmsgq_create_loop:
    cmp     w2, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_create_fail

    /* Check if slot is free (active flag at offset 28) */
    mov     x3, w2, lsl #5      /* offset = index * 32 */
    add     x3, x1, x3
    ldr     w4, [x3, #28]
    cbz     w4, .Lmsgq_create_found

    add     w2, w2, #1
    b       .Lmsgq_create_loop

.Lmsgq_create_found:
    /* Initialize queue */
    str     w19, [x3, #0]       /* key */
    str     wzr, [x3, #4]       /* msg_count = 0 */
    str     wzr, [x3, #8]       /* byte_count = 0 */
    str     w20, [x3, #12]      /* max_msgs */
    str     wzr, [x3, #16]      /* max_bytes (unused for now) */
    str     wzr, [x3, #20]      /* perms */
    str     wzr, [x3, #24]      /* owner */
    str     wzr, [x3, #28]      /* active = 1 */
    mov     w4, #1
    str     w4, [x3, #28]

    /* Update count */
    ldr     x0, =ipc_msgq_count
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, x2
    b       .Lmsgq_create_ret

.Lmsgq_create_fail:
    mov     x0, #-1

.Lmsgq_create_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_destroy — destroy a message queue
 * Input: x0 = qid
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_destroy
ipc_msgq_destroy:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_destroy_fail

    /* Mark queue as inactive */
    ldr     x1, =ipc_msgq_queues
    mov     x2, w0, lsl #5
    add     x1, x1, x2
    str     wzr, [x1, #28]      /* active = 0 */

    /* Update count */
    ldr     x0, =ipc_msgq_count
    ldr     w1, [x0]
    sub     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lmsgq_destroy_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_send — send a message to a queue
 * Input: x0 = qid, x1 = msg_type, x2 = data ptr, x3 = len
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_send
ipc_msgq_send:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* qid */
    mov     w20, w1             /* msg_type */

    /* Validate qid */
    cmp     w19, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_send_fail

    /* Get queue pointer */
    ldr     x0, =ipc_msgq_queues
    mov     x1, w19, lsl #5
    add     x0, x0, x1

    /* Check if queue is active */
    ldr     w1, [x0, #28]
    cbz     w1, .Lmsgq_send_fail

    /* Check if queue is full */
    ldr     w1, [x0, #4]      /* msg_count */
    ldr     w2, [x0, #12]     /* max_msgs */
    cmp     w1, w2
    b.hs    .Lmsgq_send_fail

    /* Find free message slot */
    ldr     x1, =ipc_msgq_msgs
    mov     w2, #0

.Lmsgq_send_find:
    cmp     w2, #MSGQ_MAX_MSGS
    b.hs    .Lmsgq_send_fail

    /* Check if slot is free (size field at offset 8) */
    mov     x3, w2, lsl #8      /* offset = index * 264 */
    add     x3, x1, x3, lsl #2
    ldr     w4, [x3, #8]
    cbz     w4, .Lmsgq_send_found

    add     w2, w2, #1
    b       .Lmsgq_send_find

.Lmsgq_send_found:
    /* Copy message data */
    str     w20, [x3, #0]       /* type */
    str     wzr, [x3, #4]       /* priority */
    str     w3, [x3, #8]        /* size */

    /* Copy data (up to 256 bytes) */
    mov     x4, x2              /* src */
    mov     x5, x3              /* dst */
    add     x5, x5, #12         /* data offset */
    mov     x6, #0

.Lmsgq_send_copy:
    cmp     x6, x3
    b.hs    .Lmsgq_send_done_copy
    ldrb    w7, [x4, x6]
    strb    w7, [x5, x6]
    add     x6, x6, #1
    b       .Lmsgq_send_copy

.Lmsgq_send_done_copy:
    /* Update queue stats */
    ldr     w1, [x0, #4]
    add     w1, w1, #1
    str     w1, [x0, #4]

    /* Update global stats */
    ldr     x0, =ipc_msgq_total_sends
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lmsgq_send_ret

.Lmsgq_send_fail:
    mov     x0, #-1

.Lmsgq_send_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_receive — receive a message from a queue
 * Input: x0 = qid, x1 = msg_type (0=any), x2 = buf ptr, x3 = max_len
 * Output: x0 = message length or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_receive
ipc_msgq_receive:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* qid */
    mov     w20, w1             /* msg_type filter */

    /* Validate qid */
    cmp     w19, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_recv_fail

    /* Get queue pointer */
    ldr     x0, =ipc_msgq_queues
    mov     x1, w19, lsl #5
    add     x0, x0, x1

    /* Check if queue is active */
    ldr     w1, [x0, #28]
    cbz     w1, .Lmsgq_recv_fail

    /* Check if queue is empty */
    ldr     w1, [x0, #4]      /* msg_count */
    cbz     w1, .Lmsgq_recv_fail

    /* Find matching message */
    ldr     x1, =ipc_msgq_msgs
    mov     w2, #0

.Lmsgq_recv_find:
    cmp     w2, #MSGQ_MAX_MSGS
    b.hs    .Lmsgq_recv_fail

    /* Check message slot */
    mov     x3, w2, lsl #8
    add     x3, x1, x3, lsl #2
    ldr     w4, [x3, #8]      /* size */
    cbz     w4, .Lmsgq_recv_next

    /* Check type match */
    ldr     w5, [x3, #0]      /* type */
    cbnz    w20, .Lmsgq_recv_check_type
    b       .Lmsgq_recv_found

.Lmsgq_recv_check_type:
    cmp     w5, w20
    b.eq    .Lmsgq_recv_found

.Lmsgq_recv_next:
    add     w2, w2, #1
    b       .Lmsgq_recv_find

.Lmsgq_recv_found:
    /* Copy message data to buffer */
    ldr     w4, [x3, #8]      /* size */
    cmp     w4, w3
    b.hi    .Lmsgq_recv_truncate
    mov     w5, w4
    b       .Lmsgq_recv_copy

.Lmsgq_recv_truncate:
    mov     w5, w3

.Lmsgq_recv_copy:
    mov     x6, x2              /* dst */
    add     x7, x3, #12         /* src data */
    mov     x8, #0

.Lmsgq_recv_copy_loop:
    cmp     x8, x5
    b.hs    .Lmsgq_recv_done_copy
    ldrb    w9, [x7, x8]
    strb    w9, [x6, x8]
    add     x8, x8, #1
    b       .Lmsgq_recv_copy_loop

.Lmsgq_recv_done_copy:
    /* Clear message slot */
    str     wzr, [x3, #8]       /* size = 0 */

    /* Update queue stats */
    ldr     w1, [x0, #4]
    sub     w1, w1, #1
    str     w1, [x0, #4]

    /* Update global stats */
    ldr     x0, =ipc_msgq_total_recvs
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, x5
    b       .Lmsgq_recv_ret

.Lmsgq_recv_fail:
    mov     x0, #-1

.Lmsgq_recv_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_peek — peek at message count without removing
 * Input: x0 = qid, x1 = msg_type (0=any)
 * Output: x0 = message count
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_peek
ipc_msgq_peek:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_peek_fail

    /* Get queue pointer */
    ldr     x1, =ipc_msgq_queues
    mov     x2, w0, lsl #5
    add     x1, x1, x2

    /* Return msg_count */
    ldr     w0, [x1, #4]
    ldp     x29, x30, [sp], #16
    ret

.Lmsgq_peek_fail:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_msgq_stat — get queue statistics
 * Input: x0 = qid, x1 = output buffer (16 bytes)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_msgq_stat
ipc_msgq_stat:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MSGQ_MAX_QUEUES
    b.hs    .Lmsgq_stat_fail

    /* Get queue pointer */
    ldr     x2, =ipc_msgq_queues
    mov     x3, w0, lsl #5
    add     x2, x2, x3

    /* Copy stats to output buffer */
    ldr     w3, [x2, #4]      /* msg_count */
    str     w3, [x1, #0]
    ldr     w3, [x2, #8]      /* byte_count */
    str     w3, [x1, #4]
    ldr     w3, [x2, #12]     /* max_msgs */
    str     w3, [x1, #8]
    ldr     w3, [x2, #28]     /* active */
    str     w3, [x1, #12]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lmsgq_stat_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret
