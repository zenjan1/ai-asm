/*
 * aiasm-aarch64/kernel/module_cache.asm
 * WASM module pre-compilation cache for v7.0 parallel loading optimization.
 *
 * Caches compiled wasm3 module state (IM3Module*) to avoid re-parsing
 * on subsequent loads. Modules are identified by name.
 *
 * API:
 *   module_cache_init()           - initialize cache subsystem
 *   module_cache_lookup(name_ptr) - return cached IM3Module* or 0
 *   module_cache_store(name_ptr, module_ptr, size) - cache compiled module
 *   module_cache_invalidate(name_ptr) - remove entry by name
 *
 * Cache structure: 8 entries, each 32 bytes:
 *   [name_ptr(8) | module_ptr(8) | wasm_size(4) | hit_count(4) | valid(4) | pad(4)]
 */
.arch armv8-a

.set MCACHE_MAX,     8
.set MCACHE_SIZE,    32
.set MCACHE_NAME,    0
.set MCACHE_MODULE,  8
.set MCACHE_WASM_SZ, 16
.set MCACHE_HITS,    20
.set MCACHE_VALID,   24

/* -----------------------------------------------------------------------------
 * BSS: Cache entries
 * ----------------------------------------------------------------------------- */
.bss
.align 4
.global module_cache_table
module_cache_table:
    .skip MCACHE_MAX * MCACHE_SIZE

.global module_cache_hits
module_cache_hits:
    .word 0

.global module_cache_misses
module_cache_misses:
    .word 0

.text

/* -----------------------------------------------------------------------------
 * module_cache_init: Zero all cache entries and counters
 * ----------------------------------------------------------------------------- */
.global module_cache_init
module_cache_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero cache table */
    adrp    x0, module_cache_table
    add     x0, x0, #:lo12:module_cache_table
    mov     x1, #(MCACHE_MAX * MCACHE_SIZE)
    bl      _mc_memset

    /* Zero counters */
    adrp    x0, module_cache_hits
    add     x0, x0, #:lo12:module_cache_hits
    str     wzr, [x0]
    str     wzr, [x0, #4]         /* also zero misses */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_cache_lookup: Find cached module by name
 * Input:  x0 = name pointer
 * Output: x0 = IM3Module* pointer, or 0 if not found
 * ----------------------------------------------------------------------------- */
.global module_cache_lookup
module_cache_lookup:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name to find */

    adrp    x0, module_cache_table
    add     x0, x0, #:lo12:module_cache_table
    mov     w1, #0

1:  cmp     w1, #MCACHE_MAX
    b.ge    3f                  /* not found */

    /* Check valid flag */
    ldrb    w2, [x0, #MCACHE_VALID]
    cbz     w2, 2f

    /* Compare name pointers */
    ldr     x2, [x0, #MCACHE_NAME]
    cmp     x2, x8
    b.ne    2f

    /* Cache hit! Increment hit counter and return module pointer */
    ldr     x0, [x0, #MCACHE_MODULE]
    adrp    x1, module_cache_hits
    add     x1, x1, #:lo12:module_cache_hits
    ldr     w2, [x1]
    add     w2, w2, #1
    str     w2, [x1]

    ldp     x29, x30, [sp], #16
    ret

2:  add     x0, x0, #MCACHE_SIZE
    add     w1, w1, #1
    b       1b

3:  /* Cache miss - increment miss counter */
    adrp    x0, module_cache_misses
    add     x0, x0, #:lo12:module_cache_misses
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_cache_store: Cache a compiled module
 * Input:  x0 = name pointer, x1 = IM3Module* pointer, x2 = wasm_size
 * Output: w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global module_cache_store
module_cache_store:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name */
    mov     x9, x1              /* module pointer */
    mov     w10, w2             /* wasm size */

    /* Find free entry */
    adrp    x0, module_cache_table
    add     x0, x0, #:lo12:module_cache_table
    mov     w1, #0

1:  cmp     w1, #MCACHE_MAX
    b.ge    3f

    /* Check if valid */
    ldrb    w2, [x0, #MCACHE_VALID]
    cbz     w2, 2f              /* free slot found */

    add     x0, x0, #MCACHE_SIZE
    add     w1, w1, #1
    b       1b

2:  /* Store entry */
    str     x8, [x0, #MCACHE_NAME]
    str     x9, [x0, #MCACHE_MODULE]
    str     w10, [x0, #MCACHE_WASM_SZ]
    str     wzr, [x0, #MCACHE_HITS]    /* start at 0 hits */
    mov     w2, #1
    strb    w2, [x0, #MCACHE_VALID]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

3:  mov     w0, #-1             /* cache full */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_cache_invalidate: Remove a cache entry by name
 * Input:  x0 = name pointer
 * Output: w0 = 0 on success, -1 if not found
 * ----------------------------------------------------------------------------- */
.global module_cache_invalidate
module_cache_invalidate:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name to invalidate */

    adrp    x0, module_cache_table
    add     x0, x0, #:lo12:module_cache_table
    mov     w1, #0

1:  cmp     w1, #MCACHE_MAX
    b.ge    3f

    ldrb    w2, [x0, #MCACHE_VALID]
    cbz     w2, 2f

    ldr     x2, [x0, #MCACHE_NAME]
    cmp     x2, x8
    b.ne    2f

    /* Found - zero valid flag */
    strb    wzr, [x0, #MCACHE_VALID]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

2:  add     x0, x0, #MCACHE_SIZE
    add     w1, w1, #1
    b       1b

3:  mov     w0, #-1             /* not found */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _mc_memset - zero fill
 * Input: x0 = address, x1 = length
 * ----------------------------------------------------------------------------- */
_mc_memset:
    cbz     x1, 2f
1:  strb    wzr, [x0], #1
    sub     x1, x1, #1
    cbnz    x1, 1b
2:  ret
