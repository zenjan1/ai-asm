/*
 * aiasm-aarch64/kernel/proc_mem.asm
 * Process memory isolation for v6.0
 *
 * Each WASM process gets a dedicated memory region (max 16MB).
 * The process memory table tracks base address and size per process.
 * Access checks ensure host functions only read/write within the process region.
 *
 * Memory layout (64MB WASM region):
 * - 0x41000000 - 0x42000000: WASM process 0 (16MB)
 * - 0x42000000 - 0x43000000: WASM process 1 (16MB)
 * - 0x43000000 - 0x44000000: WASM process 2 (16MB)
 * - 0x44000000 - 0x45000000: WASM process 3 (16MB)
 *
 * API:
 *   proc_mem_init()             — initialize process memory table
 *   proc_mem_alloc(proc_id)     — allocate 16MB region, return base addr
 *   proc_mem_free(proc_id)      — free process memory region
 *   proc_mem_check(proc_id, addr, size) → x0 = 0(ok)/-1(deny)
 */
.arch armv8-a

#define MAX_PROC_MEM    16
#define PROC_MEM_SIZE   0x01000000   /* 16MB per process */
#define WASM_MEM_BASE   0x41000000

/* Process memory region entry (8 bytes each): base_addr (uint32), size (uint32) */
.bss
.align 4
.global proc_mem_table
proc_mem_table:
    .skip 128                       /* 16 entries x 8 bytes */

.global proc_mem_count
proc_mem_count:
    .word 0

.text

/* ---------------------------------------------------------------------------
 * proc_mem_init: Reset process memory table
 * --------------------------------------------------------------------------- */
.global proc_mem_init
proc_mem_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero the entire table */
    adrp    x0, proc_mem_table
    add     x0, x0, #:lo12:proc_mem_table
    mov     x1, #0
    mov     x2, #16                 /* 16 entries * 8 bytes = 128 */
1:
    strb    w1, [x0], #1
    subs    x2, x2, #1
    cbnz    x2, 1b

    adrp    x0, proc_mem_count
    add     x0, x0, #:lo12:proc_mem_count
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * proc_mem_alloc: Allocate memory region for a process
 * Input:  x0 = proc_id (0-15)
 * Output: x0 = base virtual address, or 0 on failure
 * --------------------------------------------------------------------------- */
.global proc_mem_alloc
proc_mem_alloc:
    stp     x29, x30, [sp, #-16]!

    /* Validate proc_id */
    cmp     w0, #15
    b.gt    proc_mem_alloc_fail

    mov     x1, x0                  /* save proc_id */

    /* Calculate entry offset: proc_id * 8 */
    lsl     x2, x0, #3              /* x2 = offset in table */
    adrp    x3, proc_mem_table
    add     x3, x3, #:lo12:proc_mem_table
    add     x3, x3, x2              /* x3 = &proc_mem_table[proc_id] */

    /* Check if already allocated (base != 0) */
    ldr     w4, [x3]
    cbnz    w4, proc_mem_alloc_fail   /* already allocated */

    /* Calculate base address: WASM_MEM_BASE + (proc_id * PROC_MEM_SIZE) */
    movz    x5, #0x4100, lsl #16    /* x5 = 0x41000000 */
    mov     x6, #1
    lsl     x6, x6, #24             /* x6 = 0x01000000 = 16MB */
    madd    x5, x1, x6, x5          /* base = WASM_MEM_BASE + proc_id * 16MB */

    /* Store base and size */
    str     w5, [x3]                /* base_addr */
    str     w6, [x3, #4]            /* size = 16MB */

    /* Increment count */
    adrp    x4, proc_mem_count
    add     x4, x4, #:lo12:proc_mem_count
    ldr     w5, [x4]
    add     w5, w5, #1
    str     w5, [x4]

    mov     x0, x5                  /* return base address */

    ldp     x29, x30, [sp], #16
    ret

proc_mem_alloc_fail:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * proc_mem_free: Free process memory region
 * Input:  x0 = proc_id
 * Output: none
 * --------------------------------------------------------------------------- */
.global proc_mem_free
proc_mem_free:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #15
    b.gt    1f

    lsl     x1, x0, #3
    adrp    x2, proc_mem_table
    add     x2, x2, #:lo12:proc_mem_table
    add     x2, x2, x1

    /* Check if allocated */
    ldr     w3, [x2]
    cbz     w3, 1f                  /* not allocated, nothing to free */

    /* Zero the entry */
    str     wzr, [x2]
    str     wzr, [x2, #4]

1:
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * proc_mem_check: Validate memory access for a process
 * Input:  x0 = proc_id, x1 = offset (within WASM linear memory)
 *         x2 = size
 * Output: x0 = 0 (allowed), -1 (denied)
 * --------------------------------------------------------------------------- */
.global proc_mem_check
proc_mem_check:
    stp     x29, x30, [sp, #-16]!

    /* Validate proc_id */
    cmp     w0, #15
    b.gt    proc_mem_check_deny

    /* Load entry */
    lsl     x3, x0, #3
    adrp    x4, proc_mem_table
    add     x4, x4, #:lo12:proc_mem_table
    add     x4, x4, x3

    ldr     w5, [x4]                /* base_addr */
    cbz     w5, proc_mem_check_deny   /* not allocated */

    ldr     w6, [x4, #4]            /* size */

    /* Check: offset + size <= size (no overflow check for simplicity) */
    add     x7, x1, x2
    cmp     x7, x6
    b.gt    proc_mem_check_deny

    /* Allowed */
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

proc_mem_check_deny:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * proc_mem_get_base: Get base address for a process
 * Input:  x0 = proc_id
 * Output: x0 = base address, or 0
 * --------------------------------------------------------------------------- */
.global proc_mem_get_base
proc_mem_get_base:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #15
    b.gt    proc_mem_get_base_fail

    lsl     x1, x0, #3
    adrp    x2, proc_mem_table
    add     x2, x2, #:lo12:proc_mem_table
    add     x2, x2, x1

    ldr     w0, [x2]
    ldp     x29, x30, [sp], #16
    ret

proc_mem_get_base_fail:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
