/*
 * aiasm-aarch64/kernel/ipc_shm.asm
 * Shared Memory IPC (v1.0)
 *
 * AArch64 ASM implementation of shared memory segments.
 * Provides high-performance inter-process communication via shared memory.
 *
 * Features:
 *   - Segment creation/destruction with keys
 *   - Attach/detach operations
 *   - Permission control
 *   - Segment size management
 *   - Reference counting
 *   - Segment statistics
 *   - Lock/unlock for atomic operations
 *
 * Integration: used by high-performance IPC, zero-copy data sharing
 *
 * API:
 *   ipc_shm_init()                               — init shared memory subsystem
 *   ipc_shm_create(key, size) => shmid           — create segment
 *   ipc_shm_destroy(shmid)                       — destroy segment
 *   ipc_shm_attach(shmid, pid) => addr           — attach to segment
 *   ipc_shm_detach(shmid, pid) => 0/-1           — detach from segment
 *   ipc_shm_lock(shmid) => 0/-1                  — lock segment
 *   ipc_shm_unlock(shmid) => 0/-1                — unlock segment
 *   ipc_shm_stat(shmid, out_stat)                — get segment stats
 */
.arch armv8-a

/* Constants */
.set SHM_MAX_SEGS,      16
.set SHM_SEG_SIZE,      65536     /* 64KB per segment */
.set SHM_MAX_ATTACH,    32
.set SHM_BASE_ADDR,     0x40000000

/* Shared memory segment entry (32 bytes) */
/* id, key, size, addr, attach_count, ref_count, perms, owner, locked, active */

/* Attachment entry (16 bytes) */
/* pid, addr, shmid, active */

.bss
.align 4
.global ipc_shm_init_done
ipc_shm_init_done:
    .skip 4

.align 4
ipc_shm_count:
    .skip 4
ipc_shm_total_attach:
    .skip 4
ipc_shm_total_detach:
    .skip 4

/* Shared memory segments */
.align 4
ipc_shm_segs:
    .skip SHM_MAX_SEGS * 32     /* 512 bytes */

/* Actual shared memory regions */
.align 4
ipc_shm_regions:
    .skip SHM_MAX_SEGS * SHM_SEG_SIZE   /* 1MB total */

/* Attachment table */
.align 4
ipc_shm_attach_table:
    .skip SHM_MAX_ATTACH * 16   /* 512 bytes */

.text

/* -----------------------------------------------------------------------------
 * ipc_shm_init — initialize shared memory subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_init
ipc_shm_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =ipc_shm_count
    str     wzr, [x0]
    ldr     x0, =ipc_shm_total_attach
    str     wzr, [x0]
    ldr     x0, =ipc_shm_total_detach
    str     wzr, [x0]

    /* Zero segment array */
    ldr     x0, =ipc_shm_segs
    mov     x1, #(SHM_MAX_SEGS * 32)
    bl      .Lshm_zero

    /* Zero attachment table */
    ldr     x0, =ipc_shm_attach_table
    mov     x1, #(SHM_MAX_ATTACH * 16)
    bl      .Lshm_zero

    ldr     x0, =ipc_shm_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lshm_zero:
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
 * ipc_shm_create — create a new shared memory segment
 * Input: x0 = key, x1 = size (rounded up to 64KB)
 * Output: x0 = shmid (>=0) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_create
ipc_shm_create:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* key */
    mov     x20, x1             /* size */

    /* Check if we can create more segments */
    ldr     x0, =ipc_shm_count
    ldr     w0, [x0]
    cmp     w0, #SHM_MAX_SEGS
    b.hs    .Lshm_create_fail

    /* Find free segment slot */
    ldr     x1, =ipc_shm_segs
    mov     w2, #0

.Lshm_create_loop:
    cmp     w2, #SHM_MAX_SEGS
    b.hs    .Lshm_create_fail

    /* Check if slot is free (active flag at offset 28) */
    mov     x3, w2, lsl #5      /* offset = index * 32 */
    add     x3, x1, x3
    ldr     w4, [x3, #28]
    cbz     w4, .Lshm_create_found

    add     w2, w2, #1
    b       .Lshm_create_loop

.Lshm_create_found:
    /* Initialize segment */
    str     w19, [x3, #0]       /* key */
    mov     w4, #SHM_SEG_SIZE
    str     w4, [x3, #4]        /* size (fixed 64KB) */

    /* Calculate address */
    mov     x4, #SHM_BASE_ADDR
    mov     x5, w2, lsl #16     /* offset = index * 64KB */
    add     x4, x4, x5, lsl #0
    str     x4, [x3, #8]        /* addr */

    str     wzr, [x3, #16]      /* attach_count */
    str     wzr, [x3, #20]      /* ref_count */
    str     wzr, [x3, #24]      /* perms */
    str     wzr, [x3, #28]      /* owner */
    str     wzr, [x3, #32]      /* locked */
    mov     w4, #1
    str     w4, [x3, #36]       /* active = 1 */

    /* Update count */
    ldr     x0, =ipc_shm_count
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, x2
    b       .Lshm_create_ret

.Lshm_create_fail:
    mov     x0, #-1

.Lshm_create_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_destroy — destroy a shared memory segment
 * Input: x0 = shmid
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_destroy
ipc_shm_destroy:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #SHM_MAX_SEGS
    b.hs    .Lshm_destroy_fail

    /* Mark segment as inactive */
    ldr     x1, =ipc_shm_segs
    mov     x2, w0, lsl #5
    add     x1, x1, x2
    str     wzr, [x1, #36]      /* active = 0 */

    /* Update count */
    ldr     x0, =ipc_shm_count
    ldr     w1, [x0]
    sub     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lshm_destroy_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_attach — attach to a shared memory segment
 * Input: x0 = shmid, x1 = pid
 * Output: x0 = addr or 0 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_attach
ipc_shm_attach:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* shmid */
    mov     w20, w1             /* pid */

    /* Validate shmid */
    cmp     w19, #SHM_MAX_SEGS
    b.hs    .Lshm_attach_fail

    /* Get segment pointer */
    ldr     x0, =ipc_shm_segs
    mov     x1, w19, lsl #5
    add     x0, x0, x1

    /* Check if active */
    ldr     w1, [x0, #36]
    cbz     w1, .Lshm_attach_fail

    /* Find free attachment slot */
    ldr     x1, =ipc_shm_attach_table
    mov     w2, #0

.Lshm_attach_find:
    cmp     w2, #SHM_MAX_ATTACH
    b.hs    .Lshm_attach_fail

    /* Check if slot is free (active flag at offset 12) */
    mov     x3, w2, lsl #4      /* offset = index * 16 */
    add     x3, x1, x3
    ldr     w4, [x3, #12]
    cbz     w4, .Lshm_attach_found

    add     w2, w2, #1
    b       .Lshm_attach_find

.Lshm_attach_found:
    /* Record attachment */
    str     w20, [x3, #0]       /* pid */
    ldr     x4, [x0, #8]        /* segment addr */
    str     x4, [x3, #4]        /* addr */
    str     w19, [x3, #8]       /* shmid */
    mov     w4, #1
    str     w4, [x3, #12]       /* active = 1 */

    /* Update segment attach count */
    ldr     w1, [x0, #16]
    add     w1, w1, #1
    str     w1, [x0, #16]

    /* Update stats */
    ldr     x0, =ipc_shm_total_attach
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    /* Return address */
    ldr     x0, =ipc_shm_segs
    mov     x1, w19, lsl #5
    add     x0, x0, x1
    ldr     x0, [x0, #8]
    b       .Lshm_attach_ret

.Lshm_attach_fail:
    mov     x0, #0

.Lshm_attach_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_detach — detach from a shared memory segment
 * Input: x0 = shmid, x1 = pid
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_detach
ipc_shm_detach:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* shmid */
    mov     w20, w1             /* pid */

    /* Find attachment */
    ldr     x0, =ipc_shm_attach_table
    mov     w2, #0

.Lshm_detach_find:
    cmp     w2, #SHM_MAX_ATTACH
    b.hs    .Lshm_detach_fail

    mov     x1, w2, lsl #4
    add     x1, x0, x1

    /* Check if active and matches */
    ldr     w3, [x1, #12]       /* active */
    cbz     w3, .Lshm_detach_next

    ldr     w3, [x1, #0]        /* pid */
    cmp     w3, w20
    b.ne    .Lshm_detach_next

    ldr     w3, [x1, #8]        /* shmid */
    cmp     w3, w19
    b.eq    .Lshm_detach_found

.Lshm_detach_next:
    add     w2, w2, #1
    b       .Lshm_detach_find

.Lshm_detach_found:
    /* Mark attachment as inactive */
    str     wzr, [x1, #12]

    /* Update segment attach count */
    ldr     x0, =ipc_shm_segs
    mov     x1, w19, lsl #5
    add     x0, x0, x1
    ldr     w1, [x0, #16]
    sub     w1, w1, #1
    str     w1, [x0, #16]

    /* Update stats */
    ldr     x0, =ipc_shm_total_detach
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lshm_detach_ret

.Lshm_detach_fail:
    mov     x0, #-1

.Lshm_detach_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_lock — lock a shared memory segment
 * Input: x0 = shmid
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_lock
ipc_shm_lock:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #SHM_MAX_SEGS
    b.hs    .Lshm_lock_fail

    /* Get segment pointer */
    ldr     x1, =ipc_shm_segs
    mov     x2, w0, lsl #5
    add     x1, x1, x2

    /* Check if active */
    ldr     w2, [x1, #36]
    cbz     w2, .Lshm_lock_fail

    /* Check if already locked */
    ldr     w2, [x1, #32]
    cbnz    w2, .Lshm_lock_fail

    /* Lock it */
    mov     w2, #1
    str     w2, [x1, #32]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lshm_lock_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_unlock — unlock a shared memory segment
 * Input: x0 = shmid
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_unlock
ipc_shm_unlock:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #SHM_MAX_SEGS
    b.hs    .Lshm_unlock_fail

    /* Get segment pointer */
    ldr     x1, =ipc_shm_sems
    mov     x2, w0, lsl #5
    add     x1, x1, x2

    /* Check if active */
    ldr     w2, [x1, #36]
    cbz     w2, .Lshm_unlock_fail

    /* Unlock it */
    str     wzr, [x1, #32]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lshm_unlock_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ipc_shm_stat — get segment statistics
 * Input: x0 = shmid, x1 = output buffer (32 bytes)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global ipc_shm_stat
ipc_shm_stat:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #SHM_MAX_SEGS
    b.hs    .Lshm_stat_fail

    /* Get segment pointer */
    ldr     x2, =ipc_shm_segs
    mov     x3, w0, lsl #5
    add     x2, x2, x3

    /* Copy stats to output buffer */
    ldp     x3, x4, [x2, #0]
    stp     x3, x4, [x1, #0]
    ldp     x3, x4, [x2, #16]
    stp     x3, x4, [x1, #16]
    ldp     x3, x4, [x2, #32]
    stp     x3, x4, [x1, #32]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lshm_stat_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret
