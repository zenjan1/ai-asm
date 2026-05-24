/*
 * aiasm-aarch64/kernel/msg.asm
 * Message queue IPC for AI-ASM v3.0
 *
 * Fixed-size message queues for inter-process communication.
 * 8 queues, 16 messages each, 64 bytes per message.
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Message queue constants
 * ----------------------------------------------------------------------------- */
.set MSG_SIZE,      64
.set MSGQ_CAPACITY, 16
.set MSGQ_STRUCT_SIZE, 24   /* id(4) + head(4) + tail(4) + count(4) + ready(4) + pad(4) */
.set MSGQ_BUF_SIZE, (MSGQ_CAPACITY * MSG_SIZE)
.set MAX_MSGQS,     8

/* -----------------------------------------------------------------------------
 * BSS - message queue table
 * ----------------------------------------------------------------------------- */
.bss
.align 4
msgq_table:
    .skip MSGQ_STRUCT_SIZE * MAX_MSGQS

.bss
.align 4
msgq_buffers:
    .skip MSGQ_BUF_SIZE * MAX_MSGQS

.bss
.align 4
msgq_next_id:
    .skip 4

/* -----------------------------------------------------------------------------
 * Message queue struct field offsets
 * ----------------------------------------------------------------------------- */
.set MSGQ_ID,       0
.set MSGQ_HEAD,     4
.set MSGQ_TAIL,     8
.set MSGQ_COUNT,    12
.set MSGQ_READY,    16

.text

/* -----------------------------------------------------------------------------
 * Function: msgq_init
 * Description: Initialize message queue subsystem
 * Input: none
 * Output: none
 * Clobbered: x0-x2
 * ----------------------------------------------------------------------------- */
.global msgq_init
msgq_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear msgq table */
    adrp    x0, msgq_table
    add     x0, x0, #:lo12:msgq_table
    mov     x1, #(MSGQ_STRUCT_SIZE * MAX_MSGQS)
    bl      ipc_memset

    /* Clear msgq buffers */
    adrp    x0, msgq_buffers
    add     x0, x0, #:lo12:msgq_buffers
    mov     x1, #(MSGQ_BUF_SIZE * MAX_MSGQS)
    bl      ipc_memset

    /* Next id counter = 1 */
    adrp    x0, msgq_next_id
    add     x0, x0, #:lo12:msgq_next_id
    mov     w1, #1
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: msgq_create
 * Description: Create a new message queue
 * Input: none
 * Output: w0 = queue id, -1 on failure
 * Clobbered: x0-x4
 * ----------------------------------------------------------------------------- */
.global msgq_create
msgq_create:
    stp     x29, x30, [sp, #-16]!

    /* Find free slot */
    adrp    x2, msgq_table
    add     x2, x2, #:lo12:msgq_table
    mov     x3, #0

1:  cmp     x3, #MAX_MSGQS
    bge     9f

    madd    x4, x3, x4, x2
    ldr     w5, [x4, #MSGQ_READY]
    cbz     w5, 2f

    add     x3, x3, #1
    b       1b

2:  /* x3 = free index, x4 = queue struct base */
    /* Allocate id */
    adrp    x0, msgq_next_id
    add     x0, x0, #:lo12:msgq_next_id
    ldr     w5, [x0]
    str     w5, [x4, #MSGQ_ID]
    add     w5, w5, #1
    str     w5, [x0]

    /* Initialize */
    str     wzr, [x4, #MSGQ_HEAD]
    str     wzr, [x4, #MSGQ_TAIL]
    str     wzr, [x4, #MSGQ_COUNT]
    mov     w5, #1
    str     w5, [x4, #MSGQ_READY]

    ldr     w0, [x4, #MSGQ_ID]
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: msgq_find
 * Description: Find message queue by id
 * Input: w0 = queue id
 * Output: x0 = queue struct base, 0 if not found
 * ----------------------------------------------------------------------------- */
.global msgq_find
msgq_find:
    adrp    x2, msgq_table
    add     x2, x2, #:lo12:msgq_table
    mov     x3, #0

1:  cmp     x3, #MAX_MSGQS
    bge     9f

    madd    x1, x3, x4, x2
    ldr     w5, [x1, #MSGQ_READY]
    cbz     w5, 2f

    ldr     w6, [x1, #MSGQ_ID]
    cmp     w6, w0
    b.eq    3f

2:  add     x3, x3, #1
    b       1b

3:  mov     x0, x1
    ret

9:  mov     x0, #0
    ret

/* -----------------------------------------------------------------------------
 * Function: msgq_send
 * Description: Send a message to queue
 * Input: w0 = queue id, x1 = message buffer
 * Output: w0 = 0 on success, -1 on failure (full or not found)
 * Clobbered: x0-x7
 * ----------------------------------------------------------------------------- */
.global msgq_send
msgq_send:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     x21, x1

    /* Find queue */
    mov     w0, w20
    bl      msgq_find
    cbz     x0, 9f

    mov     x4, x0               /* x4 = queue base */

    /* Check if full */
    ldr     w5, [x4, #MSGQ_COUNT]
    cmp     w5, #MSGQ_CAPACITY
    b.ge    8f

    /* Get tail position and buffer */
    ldr     w6, [x4, #MSGQ_TAIL]

    /* Calculate buffer address: msgq_buffers + id * MSGQ_BUF_SIZE + tail * MSG_SIZE */
    ldr     w7, [x4, #MSGQ_ID]
    adrp    x3, msgq_buffers
    add     x3, x3, #:lo12:msgq_buffers
    madd    x2, x7, x2, x3       /* x2 = msgq_buffers + id * MSGQ_BUF_SIZE */
    madd    x5, x6, x2, x2       /* x5 = x2 + tail * MSG_SIZE */
    /* Actually need separate multiplier: tail * MSG_SIZE */
    mov     x2, #MSG_SIZE
    mul     x5, x6, x2
    add     x5, x5, x2
    /* Re-do properly */
    ldr     w7, [x4, #MSGQ_ID]
    adrp    x3, msgq_buffers
    add     x3, x3, #:lo12:msgq_buffers
    mov     x2, #MSGQ_BUF_SIZE
    mul     x8, x7, x2           /* x8 = id * MSGQ_BUF_SIZE */
    add     x3, x3, x8
    mov     x2, #MSG_SIZE
    ldr     w6, [x4, #MSGQ_TAIL]
    mul     x8, x6, x2           /* x8 = tail * MSG_SIZE */
    add     x5, x3, x8           /* x5 = destination buffer */

    /* Copy MSG_SIZE bytes from x21 to x5 */
    mov     x6, #0
1:  cmp     x6, #MSG_SIZE
    b.ge    2f
    ldrb    w7, [x21, x6]
    strb    w7, [x5, x6]
    add     x6, x6, #1
    b       1b

2:  /* Update tail and count */
    add     w6, w6, #1
    and     w6, w6, #(MSGQ_CAPACITY - 1)
    str     w6, [x4, #MSGQ_TAIL]
    ldr     w6, [x4, #MSGQ_COUNT]
    add     w6, w6, #1
    str     w6, [x4, #MSGQ_COUNT]

    mov     w0, #0
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

8:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: msgq_recv
 * Description: Receive a message from queue
 * Input: w0 = queue id, x1 = destination buffer
 * Output: w0 = 0 on success, -1 on failure (empty or not found)
 * Clobbered: x0-x7
 * ----------------------------------------------------------------------------- */
.global msgq_recv
msgq_recv:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     x21, x1

    /* Find queue */
    mov     w0, w20
    bl      msgq_find
    cbz     x0, 9f

    mov     x4, x0               /* x4 = queue base */

    /* Check if empty */
    ldr     w5, [x4, #MSGQ_COUNT]
    cbz     w5, 8f

    /* Get head position */
    ldr     w6, [x4, #MSGQ_HEAD]
    ldr     w7, [x4, #MSGQ_ID]

    /* Calculate buffer address */
    adrp    x3, msgq_buffers
    add     x3, x3, #:lo12:msgq_buffers
    mov     x2, #MSGQ_BUF_SIZE
    mul     x8, x7, x2
    add     x3, x3, x8
    mov     x2, #MSG_SIZE
    mul     x8, x6, x2
    add     x5, x3, x8           /* x5 = source buffer */

    /* Copy MSG_SIZE bytes from x5 to x21 */
    mov     x6, #0
1:  cmp     x6, #MSG_SIZE
    b.ge    2f
    ldrb    w7, [x5, x6]
    strb    w7, [x21, x6]
    add     x6, x6, #1
    b       1b

2:  /* Update head and count */
    add     w6, w6, #1
    and     w6, w6, #(MSGQ_CAPACITY - 1)
    str     w6, [x4, #MSGQ_HEAD]
    ldr     w6, [x4, #MSGQ_COUNT]
    sub     w6, w6, #1
    str     w6, [x4, #MSGQ_COUNT]

    mov     w0, #0
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

8:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: msgq_destroy
 * Description: Destroy a message queue
 * Input: w0 = queue id
 * Output: w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global msgq_destroy
msgq_destroy:
    stp     x29, x30, [sp, #-16]!

    bl      msgq_find
    cbz     x0, 9f

    str     wzr, [x0, #MSGQ_READY]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: ipc_memset - zero fill
 * Input: x0 = address, x1 = length
 * ----------------------------------------------------------------------------- */
ipc_memset:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0
    mov     x3, x1
    cbz     x3, 9f
1:  strb    wzr, [x2], #1
    subs    x3, x3, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret
