/*
 * aiasm-aarch64/kernel/buddy.asm
 * Buddy system memory allocator for kernel heap (v4.0)
 *
 * Orders 4-10: 16B, 32B, 64B, 128B, 256B, 512B, 1KB
 * Pool: 64KB at buddy_pool
 *
 * Each free block stores [next, prev] pointers at offset 0.
 * Allocations tracked in a side table for buddy_free.
 *
 * API:
 *   buddy_init()             — initialize buddy system
 *   buddy_alloc(size) => x0  — allocate size bytes
 *   buddy_free(ptr)          — free and merge buddies
 */
.arch armv8-a

.set BUDDY_ORDER_MIN,   4       /* 16 bytes */
.set BUDDY_ORDER_MAX,   10      /* 1KB */
.set BUDDY_NUM_ORDERS,  (BUDDY_ORDER_MAX - BUDDY_ORDER_MIN + 1)  /* 7 */
.set BUDDY_POOL_SIZE,   65536   /* 64KB pool */

/* Free list node embedded at start of each free block */
.set FL_NEXT, 0
.set FL_PREV, 4

/* Allocation table entry: 16 bytes = [ptr(8), order(1), pad(7)] */
.set AT_SIZE,   16
.set AT_PTR,    0
.set AT_ORDER,  8
.set AT_MAX,    64

.text

/* -----------------------------------------------------------------------------
 * buddy_init — initialize buddy system, create one big free block at max order
 * ----------------------------------------------------------------------------- */
.global buddy_init
buddy_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear all free list heads */
    adrp    x0, buddy_free_lists
    add     x0, x0, #:lo12:buddy_free_lists
    mov     x1, #(BUDDY_NUM_ORDERS * 8)
1:  str     xzr, [x0], #8
    subs    x1, x1, #8
    b.ne    1b

    /* Clear allocation table */
    adrp    x0, buddy_alloc_table
    add     x0, x0, #:lo12:buddy_alloc_table
    mov     x1, #(AT_MAX * AT_SIZE)
1:  str     xzr, [x0], #8
    subs    x1, x1, #8
    b.ne    1b

    /* Put entire pool into free list at BUDDY_ORDER_MAX */
    adrp    x0, buddy_pool
    add     x0, x0, #:lo12:buddy_pool

    /* x0 = pool start — insert into free_lists[BUDDY_ORDER_MAX - BUDDY_ORDER_MIN] */
    mov     w1, #(BUDDY_ORDER_MAX - BUDDY_ORDER_MIN)
    bl      _buddy_fl_insert

    /* Mark initialized */
    adrp    x0, buddy_init_done
    add     x0, x0, #:lo12:buddy_init_done
    str     wzr, [x0]           /* 0 = not done yet, set to 1 below */
    mov     w1, #1
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * buddy_alloc — allocate size bytes
 * Input: x0 = size
 * Output: x0 = pointer or 0
 * ----------------------------------------------------------------------------- */
.global buddy_alloc
buddy_alloc:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0

    /* Minimum 16 bytes */
    cmp     x20, #16
    b.ge    1f
    mov     x20, #16
1:
    /* Compute order = ceil(log2(size)) */
    mov     x0, x20
    sub     x0, x0, #1
    clz     x0, x0
    mov     x1, #63
    sub     x0, x1, x0          /* x0 = floor(log2(size-1)) + 1 = ceil(log2(size)) */

    /* Clamp to [BUDDY_ORDER_MIN, BUDDY_ORDER_MAX] */
    cmp     x0, #BUDDY_ORDER_MIN
    b.ge    2f
    mov     x0, #BUDDY_ORDER_MIN
2:  cmp     x0, #BUDDY_ORDER_MAX
    b.le    3f
    b       buddy_alloc_oom
3:  mov     x21, x0             /* target order */

    /* Search free lists from target order upward */
    mov     x22, x21
4:  cmp     x22, #BUDDY_ORDER_MAX
    b.gt    buddy_alloc_oom

    mov     x0, x22
    bl      _buddy_fl_pop
    cbz     x0, 5f              /* empty, try higher order */

    /* Got a block at order x22 */
    mov     x23, x0             /* block ptr */

    /* Split if found at higher order than needed */
    cmp     x22, x21
    b.eq    6f                  /* exact match, no split */

    /* Split loop: current order x22 → split down to x21 */
    mov     x24, x23            /* block being split */
7:  sub     w25, w22, #1        /* new_order = x22 - 1 */

    /* Buddy offset = 2^new_order */
    mov     x1, #1
    lsl     x1, x1, x25

    /* Buddy address */
    add     x2, x24, x1

    /* Insert buddy into free list at new_order */
    mov     x0, x2
    mov     x1, x25
    bl      _buddy_fl_insert

    /* Continue splitting? */
    cmp     w25, w21
    b.ne    7b

    /* x24 is now at target order */
    mov     x23, x24

6:  /* Record allocation and return */
    mov     x0, x23
    mov     x1, x21
    bl      _buddy_at_insert      /* record in allocation table */

    mov     x0, x23
    ldp     x29, x30, [sp], #16
    ret

5:  add     x22, x22, #1
    b       4b

buddy_alloc_oom:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * buddy_free — free a block and merge buddies
 * Input: x0 = pointer
 * ----------------------------------------------------------------------------- */
.global buddy_free
buddy_free:
    stp     x29, x30, [sp, #-16]!
    cbz     x0, buddy_free_done
    mov     x20, x0

    /* Look up order from allocation table */
    mov     x0, x20
    bl      _buddy_at_remove
    cbz     x21, buddy_free_done    /* not found in table */

    /* Insert into free list */
    mov     x0, x20
    mov     x1, x21
    bl      _buddy_fl_insert

    /* Try merging with buddies */
    mov     x20, x20
    mov     x21, x21
    bl      _buddy_merge

buddy_free_done:
    ldp     x29, x30, [sp], #16
    ret

/* =============================================================================
 * Internal helpers
 * ============================================================================= */

/* -----------------------------------------------------------------------------
 * _buddy_fl_insert — insert block into free list at given order
 * Input: x0 = block ptr, x1 = order
 * ----------------------------------------------------------------------------- */
_buddy_fl_insert:
    stp     x29, x30, [sp, #-16]!

    /* Get free list head pointer */
    adrp    x2, buddy_free_lists
    add     x2, x2, #:lo12:buddy_free_lists
    sub     x3, x1, #BUDDY_ORDER_MIN
    lsl     x3, x3, #3
    add     x3, x3, x2          /* x3 = &free_lists[order - BUDDY_ORDER_MIN] */

    /* Insert at head (doubly linked) */
    ldr     x4, [x3]            /* old head */
    str     x4, [x0, #FL_NEXT]  /* new.next = old head */
    cbnz    x4, 1f
    str     xzr, [x0, #FL_PREV] /* new.prev = NULL */
    b       2f
1:  str     x0, [x4, #FL_PREV]  /* old.prev = new */
2:  str     x0, [x3]            /* head = new */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_fl_pop — pop head from free list at given order
 * Input: x1 = order
 * Output: x0 = block ptr or 0, x1 = order (preserved)
 * ----------------------------------------------------------------------------- */
_buddy_fl_pop:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x1

    adrp    x2, buddy_free_lists
    add     x2, x2, #:lo12:buddy_free_lists
    sub     x3, x20, #BUDDY_ORDER_MIN
    lsl     x3, x3, #3
    add     x3, x3, x2          /* x3 = &free_lists[...] */

    ldr     x0, [x3]            /* head */
    cbz     x0, _pop_empty

    ldr     x4, [x0, #FL_NEXT]  /* new head */
    str     x4, [x3]
    cbnz    x4, 1f
    b       _pop_done
1:  str     xzr, [x4, #FL_PREV]

_pop_done:
    mov     x1, x20
    ldp     x29, x30, [sp], #16
    ret

_pop_empty:
    mov     x1, x20
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_merge — try to merge block with its buddy, repeat
 * Input: x20 = block ptr, x21 = current order
 * ----------------------------------------------------------------------------- */
_buddy_merge:
    stp     x29, x30, [sp, #-16]!
    mov     x0, x20
    mov     x1, x21
    bl      _buddy_do_merge

    ldp     x29, x30, [sp], #16
    ret

_buddy_do_merge:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0             /* block */
    mov     x21, x1             /* order */

1:  cmp     x21, #BUDDY_ORDER_MAX
    b.ge    9f                  /* can't merge at max order */

    /* Buddy address = block XOR (1 << order) */
    mov     x1, #1
    lsl     x1, x1, x21
    eor     x2, x20, x1         /* x2 = buddy */

    /* Check if buddy is free at this order */
    mov     x0, x2
    mov     x1, x21
    bl      _buddy_fl_contains
    cbz     x0, 9f              /* buddy not in free list */

    /* Buddy is free — remove it from free list */
    mov     x0, x2
    mov     x1, x21
    bl      _buddy_fl_remove

    /* Merged block = lower address of the two */
    cmp     x20, x2
    b.lt    2f
    mov     x20, x2
2:
    /* Move to next order */
    add     x21, x21, #1
    b       1b

9:  /* Insert merged block into free list at final order */
    mov     x0, x20
    mov     x1, x21
    bl      _buddy_fl_insert

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_fl_contains — check if ptr is in free list at given order
 * Input: x0 = ptr, x1 = order
 * Output: x0 = 1 if found, 0 if not
 * ----------------------------------------------------------------------------- */
_buddy_fl_contains:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0
    mov     x21, x1

    adrp    x2, buddy_free_lists
    add     x2, x2, #:lo12:buddy_free_lists
    sub     x3, x21, #BUDDY_ORDER_MIN
    lsl     x3, x3, #3
    add     x3, x3, x2          /* x3 = head pointer */

    ldr     x0, [x3]            /* walk free list */
1:  cbz     x0, 2f
    cmp     x0, x20
    b.eq    3f                  /* found */
    ldr     x0, [x0, #FL_NEXT]
    b       1b

2:  mov     x0, #0              /* not found */
    b       4f
3:  mov     x0, #1              /* found */
4:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_fl_remove — remove block from free list at given order
 * Input: x0 = ptr, x1 = order
 * ----------------------------------------------------------------------------- */
_buddy_fl_remove:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0
    mov     x21, x1

    adrp    x2, buddy_free_lists
    add     x2, x2, #:lo12:buddy_free_lists
    sub     x3, x21, #BUDDY_ORDER_MIN
    lsl     x3, x3, #3
    add     x3, x3, x2          /* x3 = head pointer */

    ldr     x4, [x20, #FL_NEXT]  /* next */
    ldr     x5, [x20, #FL_PREV]  /* prev */

    cbnz    x5, 1f
    str     x4, [x3]            /* removing head */
    b       2f
1:  str     x4, [x5, #FL_NEXT]
2:  cbnz    x4, 3f
    b       4f
3:  str     x5, [x4, #FL_PREV]
4:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_at_insert — record allocation in table
 * Input: x0 = ptr, x1 = order
 * ----------------------------------------------------------------------------- */
_buddy_at_insert:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0
    mov     x21, x1

    adrp    x2, buddy_alloc_table
    add     x2, x2, #:lo12:buddy_alloc_table
    mov     x3, #0
1:  cmp     x3, #AT_MAX
    b.ge    9f
    mov     x5, x3, lsl #4        /* x5 = x3 * 16 */
    ldr     x4, [x2, x5]          /* AT_SIZE=16 */
    cbz     x4, 2f              /* free slot */
    add     x3, x3, #1
    b       1b

2:  mov     x5, x3, lsl #4
    add     x6, x2, x5
    str     x20, [x6]
    strb    w21, [x6, #AT_ORDER]
9:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _buddy_at_remove — find and remove allocation, return order in x21
 * Input: x0 = ptr
 * Output: x21 = order (0 if not found)
 * ----------------------------------------------------------------------------- */
_buddy_at_remove:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x0

    adrp    x2, buddy_alloc_table
    add     x2, x2, #:lo12:buddy_alloc_table
    mov     x3, #0
1:  cmp     x3, #AT_MAX
    b.ge    2f
    mov     x5, x3, lsl #4
    ldr     x4, [x2, x5]
    cbz     x4, 2f              /* end of table */
    cmp     x4, x20
    b.eq    3f
    add     x3, x3, #1
    b       1b

2:  mov     x21, #0             /* not found */
    b       4f

3:  mov     x5, x3, lsl #4
    add     x6, x2, x5
    ldrb    w21, [x6, #AT_ORDER]
    str     xzr, [x6]
    strb    wzr, [x6, #AT_ORDER]
4:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 16
buddy_pool:
    .skip BUDDY_POOL_SIZE

.align 8
buddy_free_lists:
    .skip BUDDY_NUM_ORDERS * 8

.align 4
buddy_init_done:
    .skip 4

/* Allocation table: 64 entries × 16 bytes */
.align 4
buddy_alloc_table:
    .skip AT_MAX * AT_SIZE
