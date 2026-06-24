/*
 * aiasm-aarch64/kernel/wasm_memory.asm
 * WASM Linear Memory Management (v1.0)
 *
 * AArch64 ASM implementation of WebAssembly linear memory management.
 * Manages WASM memory instances with grow/size/bounds-check operations.
 * Each memory instance is a contiguous region of 64KB pages.
 * Supports up to 8 memory instances with configurable max pages.
 *
 * WASM memory model:
 *   - 1 page = 65536 bytes (64KB)
 *   - Linear memory grows in page increments
 *   - All memory accesses are bounds-checked
 *   - Out-of-bounds access triggers trap
 *
 * Integration: called from wasi.asm when memory opcodes detected
 *
 * API:
 *   wasm_memory_init()                            — init memory subsystem
 *   wasm_memory_create(mem_idx, initial, maximum)  — create memory instance
 *   wasm_memory_grow(mem_idx, delta) => old_size   — grow by delta pages
 *   wasm_memory_size(mem_idx) => pages             — current size in pages
 *   wasm_memory_check_bounds(mem_idx, off, len)    — bounds check (trap on fail)
 *   wasm_memory_get_base(mem_idx) => ptr           — get base pointer
 */
.arch armv8-a

/* Constants */
.set WASM_PAGE_SIZE,        65536       /* 64KB per page */
.set WASM_PAGE_SHIFT,       16          /* log2(65536) */
.set WASM_MAX_MEMORIES,     8
.set WASM_DEFAULT_MAX_PAGES, 65536      /* 4GB max */

/* Memory instance descriptor (32 bytes each) */
/* Offset 0:  base_ptr   (8 bytes) — pointer to memory region */
/* Offset 8:  cur_pages  (4 bytes) — current number of pages */
/* Offset 12: max_pages  (4 bytes) — maximum allowed pages */
/* Offset 16: byte_size  (8 bytes) — cur_pages * WASM_PAGE_SIZE */
/* Offset 24: flags      (4 bytes) — 0=active, 1=shared */
/* Offset 28: reserved   (4 bytes) */

.bss
.align 4
.global wasm_memory_init_done
wasm_memory_init_done:
    .skip 4

/* Memory instance table */
.align 4
wasm_memory_descs:
    .skip WASM_MAX_MEMORIES * 32   /* 256 bytes */

.text

/* -----------------------------------------------------------------------------
 * wasm_memory_init — initialize memory subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global wasm_memory_init
wasm_memory_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero all memory descriptors */
    ldr     x0, =wasm_memory_descs
    mov     x1, #(WASM_MAX_MEMORIES * 32)
1:  cbz     x1, 2f
    strb    wzr, [x0], #1
    sub     x1, x1, #1
    b       1b
2:
    ldr     x0, =wasm_memory_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_memory_create — create a new memory instance
 * Input: x0 = memory index (0-7), x1 = initial pages, x2 = maximum pages
 * Output: x0 = 0 (ok), -1 (invalid index, allocation failure)
 * Note: In real OS, would use mmap. Here we record the descriptor.
 * ----------------------------------------------------------------------------- */
.global wasm_memory_create
wasm_memory_create:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    /* Validate index */
    cmp     x0, #WASM_MAX_MEMORIES
    b.hs    .Lmem_create_fail

    mov     x19, x0             /* mem index */
    mov     x20, x1             /* initial pages */

    /* Compute descriptor address */
    ldr     x3, =wasm_memory_descs
    mov     x4, #32
    mul     x4, x19, x4
    add     x3, x3, x4          /* desc = &descs[index] */

    /* Validate max_pages >= initial_pages */
    cmp     x2, x1
    b.lt    .Lmem_create_fail

    /* Store initial and max pages */
    str     w1, [x3, #8]        /* cur_pages */
    str     w2, [x3, #12]       /* max_pages */

    /* Compute byte_size = initial_pages * 65536 */
    lsl     x4, x1, #WASM_PAGE_SHIFT
    str     x4, [x3, #16]       /* byte_size */

    /* In real implementation: base_ptr = mmap(NULL, byte_size, ...) */
    /* For now, set base_ptr = 0 (placeholder — actual allocation by OS) */
    str     xzr, [x3, #0]       /* base_ptr = 0 */
    str     wzr, [x3, #24]      /* flags = 0 */

    mov     x0, #0
    b       .Lmem_create_ret

.Lmem_create_fail:
    mov     x0, #-1

.Lmem_create_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_memory_grow — grow memory by delta pages
 * Input: x0 = memory index, x1 = delta (pages to add)
 * Output: x0 = old size in pages (ok), -1 (failure: exceeds max or invalid)
 * Note: WASM spec: memory.grow returns old size on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global wasm_memory_grow
wasm_memory_grow:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    cmp     x0, #WASM_MAX_MEMORIES
    b.hs    .Lmem_grow_fail

    mov     x19, x0             /* mem index */

    /* Get descriptor */
    ldr     x3, =wasm_memory_descs
    mov     x4, #32
    mul     x4, x19, x4
    add     x3, x3, x4

    /* Load current and max pages */
    ldr     w4, [x3, #8]        /* cur_pages */
    ldr     w5, [x3, #12]       /* max_pages */

    /* Check: cur_pages + delta <= max_pages */
    add     w6, w4, w1
    cmp     w6, w5
    b.hi    .Lmem_grow_fail     /* unsigned overflow = exceeds max */

    /* Check for unsigned overflow */
    cmp     w6, w4
    b.lt    .Lmem_grow_fail     /* wrapped around */

    /* Update cur_pages */
    str     w6, [x3, #8]

    /* Update byte_size = new_pages * 65536 */
    uxtw    x7, w6
    lsl     x7, x7, #WASM_PAGE_SHIFT
    str     x7, [x3, #16]

    /* In real implementation: mremap or munmap+mmap for new region */

    /* Return old size */
    mov     x0, w4
    b       .Lmem_grow_ret

.Lmem_grow_fail:
    mov     x0, #-1

.Lmem_grow_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_memory_size — get current memory size in pages
 * Input: x0 = memory index
 * Output: x0 = current pages, or -1 (invalid index)
 * ----------------------------------------------------------------------------- */
.global wasm_memory_size
wasm_memory_size:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #WASM_MAX_MEMORIES
    b.hs    .Lmem_size_fail

    ldr     x1, =wasm_memory_descs
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2

    ldr     w0, [x1, #8]        /* cur_pages */
    b       .Lmem_size_ret

.Lmem_size_fail:
    mov     x0, #-1

.Lmem_size_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_memory_check_bounds — verify memory access is within bounds
 * Input: x0 = memory index, x1 = offset, x2 = access length (bytes)
 * Output: x0 = 0 (in bounds), -1 (out of bounds → trap)
 * ----------------------------------------------------------------------------- */
.global wasm_memory_check_bounds
wasm_memory_check_bounds:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #WASM_MAX_MEMORIES
    b.hs    .Lmem_bounds_fail

    /* Get descriptor */
    ldr     x3, =wasm_memory_descs
    mov     x4, #32
    mul     x4, x0, x4
    add     x3, x3, x4

    /* Load byte_size */
    ldr     x4, [x3, #16]       /* byte_size */

    /* Check: offset + length <= byte_size */
    add     x5, x1, x2
    cmp     x5, x4
    b.hi    .Lmem_bounds_fail

    /* Check for unsigned overflow */
    cmp     x5, x1
    b.lt    .Lmem_bounds_fail

    /* In bounds */
    mov     x0, #0
    b       .Lmem_bounds_ret

.Lmem_bounds_fail:
    mov     x0, #-1             /* trap: memory out of bounds */

.Lmem_bounds_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_memory_get_base — get base pointer of memory instance
 * Input: x0 = memory index
 * Output: x0 = base pointer, or 0 (invalid)
 * ----------------------------------------------------------------------------- */
.global wasm_memory_get_base
wasm_memory_get_base:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #WASM_MAX_MEMORIES
    b.hs    .Lmem_base_fail

    ldr     x1, =wasm_memory_descs
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2

    ldr     x0, [x1, #0]        /* base_ptr */
    b       .Lmem_base_ret

.Lmem_base_fail:
    mov     x0, #0

.Lmem_base_ret:
    ldp     x29, x30, [sp], #16
    ret
