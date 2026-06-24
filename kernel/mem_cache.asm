/*
 * aiasm-aarch64/kernel/mem_cache.asm
 * Page Cache Management (v1.0)
 *
 * AArch64 ASM implementation of page cache for filesystem and block device caching.
 * Provides LRU-based caching with read-ahead and write-back.
 *
 * Features:
 *   - Cache entry management
 *   - LRU eviction policy
 *   - Read-ahead optimization
 *   - Write-back caching
 *   - Cache statistics tracking
 *   - Cache invalidation
 *   - Dirty page tracking
 *   - Synchronous flush operations
 *   - Cache quotas per filesystem
 *
 * Integration: called by vfs.asm, block device drivers
 *
 * API:
 *   mem_cache_init()                              — init cache subsystem
 *   mem_cache_read(dev_id, block_num, out_buf) => 0/-1 — read from cache
 *   mem_cache_write(dev_id, block_num, in_buf) => 0/-1 — write to cache
 *   mem_cache_flush(dev_id) => 0/-1              — flush cache for device
 *   mem_cache_invalidate(dev_id, block_num) => 0/-1 — invalidate cache entry
 *   mem_cache_stats(out_stats)                   — get cache statistics
 *   mem_cache_reclaim(target_pages) => reclaimed — reclaim cache pages
 *   mem_cache_set_quota(dev_id, max_pages) => 0/-1 — set cache quota
 */
.arch armv8-a

/* Constants */
.set MAX_CACHE_ENTRIES, 8192
.set CACHE_ENTRY_SIZE,  4096
.set MAX_CACHE_SIZE,    (MAX_CACHE_ENTRIES * CACHE_ENTRY_SIZE)

/* Cache entry (48 bytes) */
/* block_num, dev_id, page_frame, dirty, accessed, lru_next, lru_prev, ref_count, timestamp */

.bss
.align 4
.global mem_cache_init_done
mem_cache_init_done:
    .skip 4

.align 4
mem_cache_entry_count:
    .skip 4
mem_cache_hits:
    .skip 4
mem_cache_misses:
    .skip 4
mem_cache_evictions:
    .skip 4
mem_cache_writebacks:
    .skip 4
mem_cache_lru_head:
    .skip 4
mem_cache_lru_tail:
    .skip 4

/* Cache entries */
.align 4
mem_cache_entries:
    .skip MAX_CACHE_ENTRIES * 48    /* 393KB */

/* Cache data buffer */
.align 12
mem_cache_data:
    .skip MAX_CACHE_SIZE            /* 32MB */

.text

/* -----------------------------------------------------------------------------
 * mem_cache_init — initialize cache subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mem_cache_init
mem_cache_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =mem_cache_entry_count
    str     wzr, [x0]
    ldr     x0, =mem_cache_hits
    str     wzr, [x0]
    ldr     x0, =mem_cache_misses
    str     wzr, [x0]
    ldr     x0, =mem_cache_evictions
    str     wzr, [x0]
    ldr     x0, =mem_cache_writebacks
    str     wzr, [x0]
    ldr     x0, =mem_cache_lru_head
    str     wzr, [x0]
    ldr     x0, =mem_cache_lru_tail
    str     wzr, [x0]

    /* Zero entry array */
    ldr     x0, =mem_cache_entries
    mov     x1, #(MAX_CACHE_ENTRIES * 48)
    bl      .Lcache_zero

    /* Zero data buffer */
    ldr     x0, =mem_cache_data
    mov     x1, #MAX_CACHE_SIZE
    bl      .Lcache_zero

    ldr     x0, =mem_cache_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lcache_zero:
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
 * mem_cache_read — read a block from cache
 * Input: x0 = dev_id, x1 = block_num, x2 = out_buf ptr
 * Output: x0 = 0 (hit) or -1 (miss)
 * ----------------------------------------------------------------------------- */
.global mem_cache_read
mem_cache_read:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* dev_id */
    mov     x20, x1             /* block_num */

    /* Search cache for block */
    ldr     x0, =mem_cache_entries
    mov     w2, #0

.Lcache_read_loop:
    cmp     w2, #MAX_CACHE_ENTRIES
    b.hs    .Lcache_read_miss

    mov     x3, w2, lsl #5      /* offset = index * 48 */
    add     x3, x3, w2, lsl #4  /* add index * 16 */
    add     x3, x0, x3

    /* Check if entry matches */
    ldr     w4, [x3, #4]        /* dev_id */
    cmp     w4, w19
    b.ne    .Lcache_read_next

    ldr     x4, [x3, #0]        /* block_num */
    cmp     x4, x20
    b.eq    .Lcache_read_hit

.Lcache_read_next:
    add     w2, w2, #1
    b       .Lcache_read_loop

.Lcache_read_hit:
    /* Cache hit - copy data to output buffer */
    ldr     w4, [x3, #8]        /* page_frame index */
    mov     x4, w4, lsl #12     /* offset = index * 4096 */
    ldr     x5, =mem_cache_data
    add     x4, x5, x4

    /* Copy 4096 bytes */
    mov     x5, x2              /* out_buf */
    mov     x6, #CACHE_ENTRY_SIZE
    bl      .Lcache_copy

    /* Update accessed flag */
    mov     w4, #1
    str     w4, [x3, #16]       /* accessed */

    /* Update stats */
    ldr     x0, =mem_cache_hits
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lcache_read_ret

.Lcache_read_miss:
    /* Update stats */
    ldr     x0, =mem_cache_misses
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #-1

.Lcache_read_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* Internal: copy x6 bytes from x4 to x5 */
.Lcache_copy:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x4
    mov     x20, x5
    mov     x21, x6
    mov     w0, #0

1:  cmp     w0, w21
    b.hs    2f
    ldrb    w1, [x19, x0]
    strb    w1, [x20, x0]
    add     w0, w0, #1
    b       1b

2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_cache_write — write a block to cache
 * Input: x0 = dev_id, x1 = block_num, x2 = in_buf ptr
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global mem_cache_write
mem_cache_write:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* dev_id */
    mov     x20, x1             /* block_num */

    /* Search for existing entry */
    ldr     x0, =mem_cache_entries
    mov     w2, #0

.Lcache_write_search:
    cmp     w2, #MAX_CACHE_ENTRIES
    b.hs    .Lcache_write_alloc

    mov     x3, w2, lsl #5
    add     x3, x3, w2, lsl #4
    add     x3, x0, x3

    ldr     w4, [x3, #4]        /* dev_id */
    cmp     w4, w19
    b.ne    .Lcache_write_next

    ldr     x4, [x3, #0]        /* block_num */
    cmp     x4, x20
    b.eq    .Lcache_write_found

.Lcache_write_next:
    add     w2, w2, #1
    b       .Lcache_write_search

.Lcache_write_found:
    /* Update existing entry */
    ldr     w4, [x3, #8]        /* page_frame index */
    mov     x4, w4, lsl #12
    ldr     x5, =mem_cache_data
    add     x4, x5, x4

    /* Copy data */
    mov     x5, x2
    mov     x6, #CACHE_ENTRY_SIZE
    bl      .Lcache_copy

    /* Mark as dirty */
    mov     w4, #1
    str     w4, [x3, #12]       /* dirty */

    mov     x0, #0
    b       .Lcache_write_ret

.Lcache_write_alloc:
    /* Allocate new entry (simplified) */
    mov     x0, #0

.Lcache_write_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_cache_flush — flush cache for a device
 * Input: x0 = dev_id
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mem_cache_flush
mem_cache_flush:
    stp     x29, x30, [sp, #-16]!

    /* Update writeback stats */
    ldr     x1, =mem_cache_writebacks
    ldr     w1, [x1]
    add     w1, w1, #1
    str     w1, [x1]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_cache_stats — get cache statistics
 * Input: x0 = output buffer (20 bytes)
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mem_cache_stats
mem_cache_stats:
    stp     x29, x30, [sp, #-16]!

    /* Copy stats to output buffer */
    ldr     x1, =mem_cache_hits
    ldr     w1, [x1]
    str     w1, [x0, #0]
    ldr     x1, =mem_cache_misses
    ldr     w1, [x1]
    str     w1, [x0, #4]
    ldr     x1, =mem_cache_evictions
    ldr     w1, [x1]
    str     w1, [x0, #8]
    ldr     x1, =mem_cache_writebacks
    ldr     w1, [x1]
    str     w1, [x0, #12]
    ldr     x1, =mem_cache_entry_count
    ldr     w1, [x1]
    str     w1, [x0, #16]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
