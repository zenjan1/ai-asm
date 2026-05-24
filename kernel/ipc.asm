/*
 * aiasm-aarch64/kernel/ipc.asm
 * Pipe-based IPC for AI-ASM v3.0
 *
 * Circular buffer pipes for inter-process communication.
 * 8 pipes, 4096 bytes each.
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Pipe constants
 * ----------------------------------------------------------------------------- */
.set PIPE_BUF_SIZE, 4096
.set PIPE_STRUCT_SIZE, 32    /* id(4) + read_fd(4) + write_fd(4) + read_pos(4) + write_pos(4) + count(4) + ready(4) + pad(4) */
.set MAX_PIPES, 8

/* -----------------------------------------------------------------------------
 * BSS - pipe table
 * ----------------------------------------------------------------------------- */
.bss
.align 4
pipe_table:
    .skip PIPE_STRUCT_SIZE * MAX_PIPES

.bss
.align 4
pipe_buffers:
    .skip PIPE_BUF_SIZE * MAX_PIPES

.bss
.align 4
pipe_next_fd:
    .skip 4

/* -----------------------------------------------------------------------------
 * Pipe struct field offsets (from base)
 * ----------------------------------------------------------------------------- */
.set PIPE_ID,       0
.set PIPE_READ_FD,  4
.set PIPE_WRITE_FD, 8
.set PIPE_READ_POS, 12
.set PIPE_WRITE_POS, 16
.set PIPE_COUNT,    20
.set PIPE_READY,    24

.text

/* -----------------------------------------------------------------------------
 * Function: ipc_init
 * Description: Initialize pipe subsystem
 * Input: none
 * Output: none
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global ipc_init
ipc_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear pipe table */
    adrp    x0, pipe_table
    add     x0, x0, #:lo12:pipe_table
    mov     x1, #(PIPE_STRUCT_SIZE * MAX_PIPES)
    bl      memset_bss

    /* Clear pipe buffers */
    adrp    x0, pipe_buffers
    add     x0, x0, #:lo12:pipe_buffers
    mov     x1, #(PIPE_BUF_SIZE * MAX_PIPES)
    bl      memset_bss

    /* Next fd counter = 100 (user fds start here) */
    adrp    x0, pipe_next_fd
    add     x0, x0, #:lo12:pipe_next_fd
    mov     w1, #100
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: pipe_create
 * Description: Create a new pipe, return read_fd and write_fd via pointers
 * Input: x0 = pointer to store read_fd, x1 = pointer to store write_fd
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x4
 * ----------------------------------------------------------------------------- */
.global pipe_create
pipe_create:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     x20, x0              /* save read_fd ptr */
    mov     x21, x1              /* save write_fd ptr */

    /* Find a free pipe slot */
    adrp    x2, pipe_table
    add     x2, x2, #:lo12:pipe_table
    mov     x3, #0               /* pipe index */

1:  cmp     x3, #MAX_PIPES
    bge     9f                   /* no free slot */

    madd    x4, x3, x4, x2       /* x4 = pipe_table + idx * PIPE_STRUCT_SIZE */
    ldr     w5, [x4, #PIPE_READY]
    cbz     w5, 2f               /* found free slot */

    add     x3, x3, #1
    b       1b

2:  /* x3 = free pipe index, x4 = pipe struct base */
    /* Allocate fds */
    adrp    x0, pipe_next_fd
    add     x0, x0, #:lo12:pipe_next_fd
    ldr     w5, [x0]
    mov     w6, w5               /* read_fd */
    add     w5, w5, #1
    str     w5, [x0]

    ldr     w5, [x0]
    mov     w7, w5               /* write_fd */
    add     w5, w5, #1
    str     w5, [x0]

    /* Initialize pipe struct */
    str     w3, [x4, #PIPE_ID]
    str     w6, [x4, #PIPE_READ_FD]
    str     w7, [x4, #PIPE_WRITE_FD]
    str     wzr, [x4, #PIPE_READ_POS]
    str     wzr, [x4, #PIPE_WRITE_POS]
    str     wzr, [x4, #PIPE_COUNT]
    mov     w5, #1
    str     w5, [x4, #PIPE_READY]

    /* Store fds to caller pointers */
    str     w6, [x20]
    str     w7, [x21]

    mov     w0, #0               /* success */
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  /* No free slot */
    mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: pipe_find_by_fd
 * Description: Find pipe struct by fd, return pipe base and direction
 * Input: w0 = fd
 * Output: x0 = pipe base (or 0), w1 = 1 for read, 2 for write
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global pipe_find_by_fd
pipe_find_by_fd:
    adrp    x2, pipe_table
    add     x2, x2, #:lo12:pipe_table
    mov     x3, #0

1:  cmp     x3, #MAX_PIPES
    bge     9f

    madd    x1, x3, x4, x2       /* x1 = pipe_table + idx * PIPE_STRUCT_SIZE */
    ldr     w5, [x1, #PIPE_READY]
    cbz     w5, 2f               /* skip unused */

    ldr     w6, [x1, #PIPE_READ_FD]
    cmp     w6, w0
    b.eq    3f                   /* match read_fd */

    ldr     w6, [x1, #PIPE_WRITE_FD]
    cmp     w6, w0
    b.eq    4f                   /* match write_fd */

2:  add     x3, x3, #1
    b       1b

3:  mov     x0, x1
    mov     w1, #1               /* read direction */
    ret

4:  mov     x0, x1
    mov     w1, #2               /* write direction */
    ret

9:  mov     x0, #0               /* not found */
    ret

/* -----------------------------------------------------------------------------
 * Function: pipe_write
 * Description: Write data to pipe (circular buffer)
 * Input: w0 = fd, x1 = buf, w2 = len
 * Output: w0 = bytes written, -1 on error
 * Clobbered: x0-x7
 * ----------------------------------------------------------------------------- */
.global pipe_write
pipe_write:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    mov     w20, w0              /* save fd */
    mov     x21, x1              /* save buf */
    mov     w22, w2              /* save len */

    /* Find pipe */
    mov     w0, w20
    bl      pipe_find_by_fd
    cbz     x0, 9f               /* pipe not found */

    /* Must be write direction */
    cmp     w1, #2
    b.ne    9f

    mov     x4, x0               /* x4 = pipe base */
    mov     x23, x21             /* x23 = source buf */

    /* x5 = read_pos, x6 = write_pos, x7 = count */
    ldr     w5, [x4, #PIPE_READ_POS]
    ldr     w6, [x4, #PIPE_WRITE_POS]
    ldr     w7, [x4, #PIPE_COUNT]

    mov     w0, wzr              /* written = 0 */

    /* Check if pipe is full */
    cmp     w7, #PIPE_BUF_SIZE
    b.ge    8f

    /* Calculate available space */
    mov     w8, #PIPE_BUF_SIZE
    sub     w8, w8, w7

1:  cmp     w0, w22              /* written >= len? */
    b.ge    8f

    cmp     w8, #0               /* space available? */
    ble     8f

    /* Write one byte */
    ldrb    w9, [x23, x0]

    /* Get pipe buffer address: pipe_buffers + id * PIPE_BUF_SIZE + write_pos */
    ldr     w11, [x4, #PIPE_ID]
    adrp    x10, pipe_buffers
    add     x10, x10, #:lo12:pipe_buffers
    mov     x12, #PIPE_BUF_SIZE
    mul     x12, x11, x12          /* x12 = id * PIPE_BUF_SIZE */
    add     x12, x12, x10          /* x12 = pipe_buffers + id * PIPE_BUF_SIZE */
    add     x12, x12, x6           /* x12 = buffer + write_pos */

    strb    w9, [x12]

    /* Update write_pos and count */
    add     w6, w6, #1
    and     w6, w6, #(PIPE_BUF_SIZE - 1)
    add     w7, w7, #1
    add     w0, w0, #1
    sub     w8, w8, #1
    b       1b

8:  /* Write back positions */
    str     w6, [x4, #PIPE_WRITE_POS]
    str     w7, [x4, #PIPE_COUNT]

    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: pipe_read
 * Description: Read data from pipe (circular buffer)
 * Input: w0 = fd, x1 = buf, w2 = len
 * Output: w0 = bytes read, -1 on error, 0 if empty
 * Clobbered: x0-x7
 * ----------------------------------------------------------------------------- */
.global pipe_read
pipe_read:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    mov     w20, w0              /* save fd */
    mov     x21, x1              /* save buf */
    mov     w22, w2              /* save len */

    /* Find pipe */
    mov     w0, w20
    bl      pipe_find_by_fd
    cbz     x0, 9f               /* pipe not found */

    /* Must be read direction */
    cmp     w1, #1
    b.ne    9f

    mov     x4, x0               /* x4 = pipe base */
    mov     x23, x21             /* x23 = dest buf */

    ldr     w5, [x4, #PIPE_READ_POS]
    ldr     w6, [x4, #PIPE_WRITE_POS]
    ldr     w7, [x4, #PIPE_COUNT]

    mov     w0, wzr              /* read = 0 */

    /* Check if pipe is empty */
    cbz     w7, 8f

    /* Get pipe buffer base */
    ldr     w11, [x4, #PIPE_ID]
    adrp    x10, pipe_buffers
    add     x10, x10, #:lo12:pipe_buffers
    mov     x12, #PIPE_BUF_SIZE
    mul     x12, x11, x12          /* x12 = id * PIPE_BUF_SIZE */
    add     x12, x12, x10          /* x12 = pipe_buffers + id * PIPE_BUF_SIZE */

1:  cmp     w0, w22              /* read >= len? */
    b.ge    8f

    cbz     w7, 8f               /* no more data */

    /* Read one byte */
    add     x14, x12, x5           /* x14 = buffer + read_pos */
    ldrb    w9, [x14]
    strb    w9, [x23, x0]

    /* Update read_pos and count */
    add     w5, w5, #1
    and     w5, w5, #(PIPE_BUF_SIZE - 1)
    sub     w7, w7, #1
    add     w0, w0, #1
    b       1b

8:  /* Write back positions */
    str     w5, [x4, #PIPE_READ_POS]
    str     w7, [x4, #PIPE_COUNT]

    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: pipe_close
 * Description: Close a pipe fd
 * Input: w0 = fd
 * Output: w0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global pipe_close
pipe_close:
    stp     x29, x30, [sp, #-16]!

    bl      pipe_find_by_fd
    cbz     x0, 9f

    /* Mark pipe as not ready */
    str     wzr, [x0, #PIPE_READY]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: memset_bss - zero fill
 * Input: x0 = address, x1 = length
 * ----------------------------------------------------------------------------- */
memset_bss:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0
    mov     x3, x1
    cbz     x3, 9f
1:  strb    wzr, [x2], #1
    subs    x3, x3, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret
