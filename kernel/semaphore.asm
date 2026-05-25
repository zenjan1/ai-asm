/*
 * aiasm-aarch64/kernel/semaphore.asm
 * Semaphore IPC subsystem v9.0
 * 16 semaphores, counting semaphores with wait/post/try_wait
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set SEM_MAX,          16
.set SEM_STRUCT_SIZE,  16        /* id(4) + value(4) + waiters(4) + state(4) */

/* State values */
.set SEM_STATE_FREE,   0
.set SEM_STATE_ACTIVE, 1
.set SEM_STATE_WAITING,2

/* Field offsets within semaphore struct */
.set SEM_ID,           0
.set SEM_VALUE,        4
.set SEM_WAITERS,      8
.set SEM_STATE,        12

/* -----------------------------------------------------------------------------
 * Data section
 * ----------------------------------------------------------------------------- */
.data
.align 4
sem_table:
    .fill SEM_MAX * SEM_STRUCT_SIZE, 1, 0
sem_next_id:
    .word 1

.text

/* -----------------------------------------------------------------------------
 * semaphore_init — zero semaphore table
 * ----------------------------------------------------------------------------- */
.global semaphore_init
semaphore_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, sem_table
    add     x0, x0, #:lo12:sem_table
    mov     w1, #0
    mov     x2, #(SEM_MAX * SEM_STRUCT_SIZE)
    bl      _sem_memset

    adrp    x0, sem_next_id
    add     x0, x0, #:lo12:sem_next_id
    mov     w1, #1
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_create(initial_value) → sem_id or -1
 * x0 = initial_value
 * ----------------------------------------------------------------------------- */
.global semaphore_create
semaphore_create:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!

    mov     w20, w0               /* save initial_value */

    adrp    x1, sem_table
    add     x1, x1, #:lo12:sem_table

    mov     x2, #0                /* index */
_sem_create_find:
    cmp     x2, #SEM_MAX
    b.ge    _sem_create_fail

    mov     x3, x2
    mov     x4, #SEM_STRUCT_SIZE
    mul     x3, x3, x4
    add     x5, x1, x3            /* x5 = &sem_table[index] */

    ldr     w6, [x5, #SEM_STATE]
    cbz     w6, _sem_create_found /* state == FREE */

    add     x2, x2, #1
    b       _sem_create_find

_sem_create_found:
    /* Initialize semaphore */
    adrp    x6, sem_next_id
    add     x6, x6, #:lo12:sem_next_id
    ldr     w7, [x6]
    str     w7, [x5, #SEM_ID]     /* id */
    str     w20, [x5, #SEM_VALUE] /* value = initial_value */
    mov     w0, #0
    str     w0, [x5, #SEM_WAITERS]/* waiters = 0 */
    mov     w0, #SEM_STATE_ACTIVE
    str     w0, [x5, #SEM_STATE]

    /* Increment next_id */
    add     w7, w7, #1
    str     w7, [x6]

    mov     w0, w7                /* return sem_id */
    b       _sem_create_done

_sem_create_fail:
    mov     w0, #-1

_sem_create_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _sem_lookup(sem_id) → x5 = pointer to sem or x0 = -1
 * x0 = sem_id, returns pointer in x5
 * ----------------------------------------------------------------------------- */
_sem_lookup:
    stp     x22, x23, [sp, #-16]!

    mov     w22, w0               /* save sem_id */
    adrp    x5, sem_table
    add     x5, x5, #:lo12:sem_table

    mov     x2, #0                /* index */
_sem_lookup_loop:
    cmp     x2, #SEM_MAX
    b.ge    _sem_lookup_fail

    mov     x3, x2
    mov     x4, #SEM_STRUCT_SIZE
    mul     x3, x3, x4
    add     x23, x5, x3           /* x23 = &sem_table[index] */

    ldr     w6, [x23, #SEM_ID]
    cmp     w6, w22
    b.eq    _sem_lookup_ok

    ldr     w6, [x23, #SEM_STATE]
    cbnz    w6, _sem_lookup_skip  /* skip if not FREE (FREE entries have id=0) */

_sem_lookup_skip:
    add     x2, x2, #1
    b       _sem_lookup_loop

_sem_lookup_ok:
    mov     x5, x23
    ldp     x22, x23, [sp], #16
    ret

_sem_lookup_fail:
    mov     x5, #0
    ldp     x22, x23, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_wait(sem_id) → 0=ok, -1=invalid
 * x0 = sem_id
 * If value > 0: decrement and return immediately
 * If value == 0: set WAITING, increment waiters, return -2 (would block)
 * ----------------------------------------------------------------------------- */
.global semaphore_wait
semaphore_wait:
    stp     x29, x30, [sp, #-16]!

    bl      _sem_lookup
    cbz     x5, _sem_wait_invalid

    ldr     w6, [x5, #SEM_VALUE]
    cbz     w6, _sem_wait_block

    /* value > 0: decrement */
    sub     w6, w6, #1
    str     w6, [x5, #SEM_VALUE]
    mov     w0, #0
    b       _sem_wait_done

_sem_wait_block:
    /* value == 0: set WAITING state */
    mov     w6, #SEM_STATE_WAITING
    str     w6, [x5, #SEM_STATE]

    ldr     w6, [x5, #SEM_WAITERS]
    add     w6, w6, #1
    str     w6, [x5, #SEM_WAITERS]

    mov     w0, #-2               /* would block */
    b       _sem_wait_done

_sem_wait_invalid:
    mov     w0, #-1

_sem_wait_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_try_wait(sem_id) → 0=ok, -1=invalid, -2=would block
 * x0 = sem_id
 * ----------------------------------------------------------------------------- */
.global semaphore_try_wait
semaphore_try_wait:
    stp     x29, x30, [sp, #-16]!

    bl      _sem_lookup
    cbz     x5, _sem_try_fail

    ldr     w6, [x5, #SEM_VALUE]
    cbz     w6, _sem_try_block

    sub     w6, w6, #1
    str     w6, [x5, #SEM_VALUE]
    mov     w0, #0
    b       _sem_try_done

_sem_try_block:
    mov     w0, #-2
    b       _sem_try_done

_sem_try_fail:
    mov     w0, #-1

_sem_try_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_post(sem_id) → 0=ok, -1=invalid
 * x0 = sem_id
 * Increment value. If waiters > 0, decrement waiters.
 * ----------------------------------------------------------------------------- */
.global semaphore_post
semaphore_post:
    stp     x29, x30, [sp, #-16]!

    bl      _sem_lookup
    cbz     x5, _sem_post_fail

    ldr     w6, [x5, #SEM_VALUE]
    add     w6, w6, #1
    str     w6, [x5, #SEM_VALUE]

    /* If in WAITING state and waiters > 0, decrement waiters */
    ldr     w7, [x5, #SEM_STATE]
    cmp     w7, #SEM_STATE_WAITING
    b.ne    _sem_post_done

    ldr     w7, [x5, #SEM_WAITERS]
    cbz     w7, _sem_post_done

    sub     w7, w7, #1
    str     w7, [x5, #SEM_WAITERS]

    /* If no more waiters, set back to ACTIVE */
    cbz     w7, _sem_post_reset
    b       _sem_post_done

_sem_post_reset:
    mov     w7, #SEM_STATE_ACTIVE
    str     w7, [x5, #SEM_STATE]

_sem_post_done:
    mov     w0, #0
    b       _sem_post_ret

_sem_post_fail:
    mov     w0, #-1

_sem_post_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_destroy(sem_id) → 0=ok, -1=invalid
 * x0 = sem_id
 * ----------------------------------------------------------------------------- */
.global semaphore_destroy
semaphore_destroy:
    stp     x29, x30, [sp, #-16]!

    bl      _sem_lookup
    cbz     x5, _sem_destroy_fail

    mov     w6, #SEM_STATE_FREE
    str     w6, [x5, #SEM_STATE]
    mov     w0, #0
    str     w0, [x5, #SEM_VALUE]
    str     w0, [x5, #SEM_WAITERS]
    mov     w0, #0
    b       _sem_destroy_done

_sem_destroy_fail:
    mov     w0, #-1

_sem_destroy_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * semaphore_get_value(sem_id) → value or -1
 * x0 = sem_id
 * ----------------------------------------------------------------------------- */
.global semaphore_get_value
semaphore_get_value:
    stp     x29, x30, [sp, #-16]!

    bl      _sem_lookup
    cbz     x5, _sem_getval_fail

    ldr     w0, [x5, #SEM_VALUE]
    b       _sem_getval_done

_sem_getval_fail:
    mov     w0, #-1

_sem_getval_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Internal: memset (x0=ptr, w1=value, x2=count)
 * ----------------------------------------------------------------------------- */
_sem_memset:
    cbz     x2, 1f
    mov     w3, w1
    and     w3, w3, #0xff
2:  strb    w3, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 2b
1:  ret
