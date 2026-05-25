/*
 * aiasm-aarch64/kernel/shmem.asm
 * Shared memory IPC subsystem v9.0
 * 8 shared memory regions, each 512 bytes
 * Region header: owner_pid(4) + size(4) + flags(4) + refcount(4) = 16 bytes
 * State: 0=free, 1=allocated, 2=shared
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set SHMEM_MAX_REGIONS,   8
.set SHMEM_REGION_SIZE,   512
.set SHMEM_HEADER_SIZE,   16
.set SHMEM_DATA_SIZE,     (SHMEM_REGION_SIZE - SHMEM_HEADER_SIZE)
.set SHMEM_REGION_STRUCT, SHMEM_REGION_SIZE

/* Header field offsets */
.set SHMEM_OWNER_PID,  0
.set SHMEM_SIZE,       4
.set SHMEM_FLAGS,      8
.set SHMEM_REFCOUNT,   12

/* Flag bits */
.set SHMEM_FLAG_FREE,      0
.set SHMEM_FLAG_ALLOCATED, 1
.set SHMEM_FLAG_SHARED,    2
.set SHMEM_FLAG_WRITER,    0x10    /* writer has data ready */
.set SHMEM_FLAG_READER,    0x20    /* reader has consumed */

/* -----------------------------------------------------------------------------
 * BSS - shared memory table
 * ----------------------------------------------------------------------------- */
.bss
.align 12
shmem_table:
    .skip SHMEM_REGION_STRUCT * SHMEM_MAX_REGIONS

.bss
.align 4
shmem_next_id:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * Function: shmem_init
 * Description: Initialize shared memory subsystem
 * Input: none
 * Output: none
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_init
shmem_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear shared memory table */
    adrp    x0, shmem_table
    add     x0, x0, #:lo12:shmem_table
    mov     x1, #(SHMEM_REGION_STRUCT * SHMEM_MAX_REGIONS)
    bl      _shmem_memset

    /* Next ID counter = 1 */
    adrp    x0, shmem_next_id
    add     x0, x0, #:lo12:shmem_next_id
    mov     w1, #1
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_alloc
 * Description: Allocate a shared memory region for a process
 * Input: w0 = pid, w1 = size (max 496 bytes)
 * Output: w0 = region_id (>=1), -1 on failure
 * Clobbered: x0-x5
 * ----------------------------------------------------------------------------- */
.global shmem_alloc
shmem_alloc:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0              /* save pid */
    mov     w21, w1              /* save size */

    /* Validate size */
    cmp     w21, #SHMEM_DATA_SIZE
    b.gt    9f                   /* size too large */
    cbz     w21, 9f              /* size zero */

    /* Find a free region */
    adrp    x2, shmem_table
    add     x2, x2, #:lo12:shmem_table
    mov     x3, #0               /* region index */

1:  cmp     x3, #SHMEM_MAX_REGIONS
    bge     9f                   /* no free region */

    mov     x4, #SHMEM_REGION_STRUCT
    madd    x5, x3, x4, x2       /* x5 = shmem_table + idx * struct_size */
    ldr     w6, [x5, #SHMEM_FLAGS]
    cbz     w6, 2f               /* found free slot */

    add     x3, x3, #1
    b       1b

2:  /* x3 = region index, x5 = region base */
    /* Generate region_id = index + 1 */
    add     w0, w3, #1

    /* Initialize header */
    str     w20, [x5, #SHMEM_OWNER_PID]
    str     w21, [x5, #SHMEM_SIZE]
    mov     w6, #SHMEM_FLAG_ALLOCATED
    str     w6, [x5, #SHMEM_FLAGS]
    mov     w6, #1
    str     w6, [x5, #SHMEM_REFCOUNT]

    /* Clear data area */
    add     x5, x5, #SHMEM_HEADER_SIZE
    mov     x4, #SHMEM_DATA_SIZE
    bl      _shmem_memset

    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_free
 * Description: Free a shared memory region
 * Input: w0 = region_id
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_free
shmem_free:
    stp     x29, x30, [sp, #-16]!

    /* Find region by id */
    sub     w1, w0, #1           /* index = id - 1 */
    cbz     w0, 9f               /* id 0 is invalid */

    adrp    x2, shmem_table
    add     x2, x2, #:lo12:shmem_table
    mov     x3, #SHMEM_REGION_STRUCT
    madd    x4, x1, x3, x2       /* x4 = region base */

    /* Check it's allocated */
    ldr     w5, [x4, #SHMEM_FLAGS]
    cbz     w5, 9f               /* already free */

    /* Clear entire region */
    mov     x0, x4
    mov     x1, #SHMEM_REGION_STRUCT
    bl      _shmem_memset

    ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

9:  ldp     x29, x30, [sp], #16
    mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_attach
 * Description: Attach to an allocated shared memory region (increment refcount)
 * Input: w0 = region_id, w1 = pid of attaching process
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_attach
shmem_attach:
    stp     x29, x30, [sp, #-16]!

    sub     w2, w0, #1           /* index = id - 1 */
    cbz     w0, 9f

    adrp    x3, shmem_table
    add     x3, x3, #:lo12:shmem_table
    mov     x4, #SHMEM_REGION_STRUCT
    madd    x5, x2, x4, x3       /* x5 = region base */

    /* Check allocated */
    ldr     w6, [x5, #SHMEM_FLAGS]
    cmp     w6, #SHMEM_FLAG_ALLOCATED
    b.eq    1f
    cmp     w6, #SHMEM_FLAG_SHARED
    b.eq    1f
    b       9f

1:  /* Increment refcount */
    ldr     w6, [x5, #SHMEM_REFCOUNT]
    add     w6, w6, #1
    str     w6, [x5, #SHMEM_REFCOUNT]

    /* Mark as shared */
    mov     w6, #SHMEM_FLAG_SHARED
    str     w6, [x5, #SHMEM_FLAGS]

    ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

9:  ldp     x29, x30, [sp], #16
    mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_detach
 * Description: Detach from shared memory region (decrement refcount, free if 0)
 * Input: w0 = region_id
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_detach
shmem_detach:
    stp     x29, x30, [sp, #-16]!

    sub     w1, w0, #1           /* index = id - 1 */
    cbz     w0, 9f

    adrp    x2, shmem_table
    add     x2, x2, #:lo12:shmem_table
    mov     x3, #SHMEM_REGION_STRUCT
    madd    x4, x1, x3, x2       /* x4 = region base */

    /* Check allocated or shared */
    ldr     w5, [x4, #SHMEM_FLAGS]
    cbz     w5, 9f               /* already free */

    /* Decrement refcount */
    ldr     w5, [x4, #SHMEM_REFCOUNT]
    sub     w5, w5, #1
    cbz     w5, 2f               /* refcount == 0, free it */

    str     w5, [x4, #SHMEM_REFCOUNT]

    /* If refcount == 1, back to allocated state */
    cmp     w5, #1
    b.ne    1f
    mov     w5, #SHMEM_FLAG_ALLOCATED
    str     w5, [x4, #SHMEM_FLAGS]

1:  ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

2:  /* Refcount zeroed, clear region */
    mov     x0, x4
    mov     x1, #SHMEM_REGION_STRUCT
    bl      _shmem_memset

    ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

9:  ldp     x29, x30, [sp], #16
    mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_get_ptr
 * Description: Get kernel physical address of shared memory data area
 * Input: w0 = region_id
 * Output: x0 = pointer to data area (after header), or 0 on failure
 * Clobbered: x0-x2
 * ----------------------------------------------------------------------------- */
.global shmem_get_ptr
shmem_get_ptr:
    sub     w1, w0, #1           /* index = id - 1 */
    cbz     w0, 9f

    adrp    x2, shmem_table
    add     x2, x2, #:lo12:shmem_table
    mov     x3, #SHMEM_REGION_STRUCT
    madd    x0, x1, x3, x2       /* x0 = region base */

    /* Check allocated */
    ldr     w4, [x0, #SHMEM_FLAGS]
    cbz     w4, 9f               /* free region */

    /* Return pointer to data area (after header) */
    add     x0, x0, #SHMEM_HEADER_SIZE
    ret

9:  mov     x0, #0
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_set_flag
 * Description: Set a flag on a shared memory region (for synchronization)
 * Input: w0 = region_id, w1 = flag value to OR
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_set_flag
shmem_set_flag:
    stp     x29, x30, [sp, #-16]!

    sub     w2, w0, #1
    cbz     w0, 9f

    adrp    x3, shmem_table
    add     x3, x3, #:lo12:shmem_table
    mov     x4, #SHMEM_REGION_STRUCT
    madd    x5, x2, x4, x3

    ldr     w6, [x5, #SHMEM_FLAGS]
    cbz     w6, 9f

    orr     w6, w6, w1
    str     w6, [x5, #SHMEM_FLAGS]

    ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

9:  ldp     x29, x30, [sp], #16
    mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_get_flag
 * Description: Get current flags on a shared memory region
 * Input: w0 = region_id
 * Output: w0 = flags, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_get_flag
shmem_get_flag:
    sub     w1, w0, #1
    cbz     w0, 9f

    adrp    x2, shmem_table
    add     x2, x2, #:lo12:shmem_table
    mov     x3, #SHMEM_REGION_STRUCT
    madd    x4, x1, x3, x2

    ldr     w0, [x4, #SHMEM_FLAGS]
    ret

9:  mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Function: shmem_clear_flag
 * Description: Clear a flag on a shared memory region
 * Input: w0 = region_id, w1 = flag value to AND NOT
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global shmem_clear_flag
shmem_clear_flag:
    stp     x29, x30, [sp, #-16]!

    sub     w2, w0, #1
    cbz     w0, 9f

    adrp    x3, shmem_table
    add     x3, x3, #:lo12:shmem_table
    mov     x4, #SHMEM_REGION_STRUCT
    madd    x5, x2, x4, x3

    ldr     w6, [x5, #SHMEM_FLAGS]
    cbz     w6, 9f

    bic     w6, w6, w1
    str     w6, [x5, #SHMEM_FLAGS]

    ldp     x29, x30, [sp], #16
    mov     w0, #0
    ret

9:  ldp     x29, x30, [sp], #16
    mov     w0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Helper: _shmem_memset - zero fill
 * Input: x0 = address, x1 = length
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
_shmem_memset:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0
    mov     x3, x1
    cbz     x3, 9f
1:  strb    wzr, [x2], #1
    subs    x3, x3, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret
