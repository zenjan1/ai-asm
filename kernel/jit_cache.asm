/*
 * aiasm-aarch64/kernel/jit_cache.asm
 * WASM JIT pre-compilation cache for AI-ASM v4.0
 *
 * Caches compiled wasm3 module opcodes to avoid re-parsing on reload.
 * 16 entries, each with name hash + compiled code pointer.
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set JIT_CACHE_SIZE,  16
.set JIT_ENTRY_SIZE,   64   /* name_hash(4) + name_ptr(4) + code_ptr(8) + code_size(4) + valid(4) + padding */
.set JIT_CODE_POOL,    1048576  /* 1MB pool for cached compiled code */

/* -----------------------------------------------------------------------------
 * BSS
 * ----------------------------------------------------------------------------- */
.bss
.align 4
jit_cache_entries:
    .skip JIT_ENTRY_SIZE * JIT_CACHE_SIZE

.bss
.align 4
jit_code_pool:
    .skip JIT_CODE_POOL

.bss
.align 4
jit_pool_next:
    .skip 4

.bss
.align 4
jit_cache_init_done:
    .skip 4

/* JIT entry field offsets */
.set JIT_NAME_HASH,  0
.set JIT_NAME_PTR,   4
.set JIT_CODE_PTR,   8
.set JIT_CODE_SIZE,  16
.set JIT_VALID,      20

.text

/* -----------------------------------------------------------------------------
 * Function: jit_cache_init
 * Description: Initialize JIT cache subsystem
 * ----------------------------------------------------------------------------- */
.global jit_cache_init
jit_cache_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, jit_cache_init_done
    add     x0, x0, #:lo12:jit_cache_init_done
    ldr     w1, [x0]
    cbnz    w1, 9f

    /* Clear all cache entries */
    adrp    x0, jit_cache_entries
    add     x0, x0, #:lo12:jit_cache_entries
    mov     x1, #(JIT_ENTRY_SIZE * JIT_CACHE_SIZE)
    bl      jit_memset

    /* Initialize code pool pointer */
    adrp    x0, jit_code_pool
    add     x0, x0, #:lo12:jit_code_pool
    adrp    x1, jit_pool_next
    add     x1, x1, #:lo12:jit_pool_next
    str     x0, [x1]

    mov     w1, #1
    adrp    x0, jit_cache_init_done
    add     x0, x0, #:lo12:jit_cache_init_done
    str     w1, [x0]

9:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: jit_cache_lookup
 * Description: Look up cached compiled module by name hash
 * Input: w0 = name_hash, x1 = name pointer
 * Output: x0 = compiled code pointer, or 0 if not found
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global jit_cache_lookup
jit_cache_lookup:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     x21, x1

    adrp    x2, jit_cache_entries
    add     x2, x2, #:lo12:jit_cache_entries
    mov     x3, #0

1:  cmp     x3, #JIT_CACHE_SIZE
    bge     9f                   /* not found */

    madd    x4, x3, x4, x2       /* x4 = entry base */

    /* Check valid flag */
    ldr     w5, [x4, #JIT_VALID]
    cbz     w5, 2f

    /* Check hash match */
    ldr     w6, [x4, #JIT_NAME_HASH]
    cmp     w6, w20
    b.ne    2f

    /* Hash matches - verify name pointer too */
    ldr     x7, [x4, #JIT_NAME_PTR]
    cmp     x7, x21
    b.ne    2f

    /* Found! Return code pointer */
    ldr     x0, [x4, #JIT_CODE_PTR]
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

2:  add     x3, x3, #1
    b       1b

9:  mov     x0, #0               /* not found */
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: jit_cache_store
 * Description: Store compiled module code in cache
 * Input: w0 = name_hash, x1 = name pointer, x2 = compiled code, x3 = size
 * Output: w0 = 0 on success, -1 on failure
 * Clobbered: x0-x5
 * ----------------------------------------------------------------------------- */
.global jit_cache_store
jit_cache_store:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    mov     w20, w0
    mov     x21, x1
    mov     x22, x2
    mov     x23, x3

    /* Find a free entry */
    adrp    x4, jit_cache_entries
    add     x4, x4, #:lo12:jit_cache_entries
    mov     x5, #0

1:  cmp     x5, #JIT_CACHE_SIZE
    bge     9f

    madd    x6, x5, x6, x4       /* x6 = entry base */
    ldr     w7, [x6, #JIT_VALID]
    cbz     w7, 2f
    add     x5, x5, #1
    b       1b

2:  /* x6 = free entry base */
    /* Allocate from code pool */
    adrp    x8, jit_pool_next
    add     x8, x8, #:lo12:jit_pool_next
    ldr     x9, [x8]

    /* Align to 16 bytes */
    add     x9, x9, #15
    bic     x9, x9, #15

    /* Check pool space */
    adrp    x10, jit_code_pool
    add     x10, x10, #:lo12:jit_code_pool
    ldr     x11, [x8]
    add     x11, x11, x23
    cmp     x11, x10
    b.gt    9f                   /* pool full */

    /* Copy compiled code to pool */
    mov     x12, x9              /* dest = pool alloc ptr */
    mov     x13, x22             /* src = compiled code */
    mov     x14, x23             /* size */
    bl      jit_memcpy

    /* Store entry */
    str     w20, [x6, #JIT_NAME_HASH]
    str     x21, [x6, #JIT_NAME_PTR]
    str     x12, [x6, #JIT_CODE_PTR]
    str     w23, [x6, #JIT_CODE_SIZE]
    mov     w7, #1
    str     w7, [x6, #JIT_VALID]

    /* Update pool next pointer */
    ldr     x9, [x8]
    add     x9, x9, x23
    str     x9, [x8]

    mov     w0, #0
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
 * Helper: jit_memset - zero fill
 * Input: x0 = address, x1 = length
 * ----------------------------------------------------------------------------- */
jit_memset:
    stp     x29, x30, [sp, #-16]!
    mov     x2, x0
    mov     x3, x1
    cbz     x3, 9f
1:  strb    wzr, [x2], #1
    subs    x3, x3, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: jit_memcpy - copy memory
 * Input: x0 = dest, x1 = src, x2 = size
 * ----------------------------------------------------------------------------- */
jit_memcpy:
    stp     x29, x30, [sp, #-16]!
    cbz     x2, 9f
1:  ldrb    w3, [x1], #1
    strb    w3, [x0], #1
    subs    x2, x2, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret
