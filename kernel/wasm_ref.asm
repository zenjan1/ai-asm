/*
 * aiasm-aarch64/kernel/wasm_ref.asm
 * WASM Reference Types & Table Operations (v1.0)
 *
 * AArch64 ASM implementation of WebAssembly reference types proposal.
 * Handles externref and funcref types stored as 32-bit indices.
 * Provides ref.null, ref.is_null, ref.func operations.
 * Provides table.get, table.set, table.size, table.grow, table.fill.
 * Includes GC tracing hooks for reference tracking.
 *
 * Reference encoding:
 *   null ref    → 0xFFFFFFFF (sentinel)
 *   funcref i   → i (function index, 32-bit)
 *   externref i → i (extern handle, 32-bit)
 *
 * Integration: extends wasm_embed.asm function table management
 *
 * API:
 *   wasm_ref_init()                          — initialize ref types subsystem
 *   wasm_ref_null() => x0                    — return null ref sentinel
 *   wasm_ref_is_null(ref) => x0              — test if ref is null (0/1)
 *   wasm_ref_func(func_idx) => x0            — create funcref from index
 *   wasm_ref_table_get(tbl, idx) => x0       — get ref from table
 *   wasm_ref_table_set(tbl, idx, val) => x0  — set ref in table
 *   wasm_ref_table_size(tbl) => x0           — get table size
 *   wasm_ref_table_grow(tbl, delta, init)    — grow table by delta
 *   wasm_ref_table_fill(tbl, idx, val, n)    — fill table region
 *   wasm_ref_gc_trace()                      — trace live refs for GC
 */
.arch armv8-a

/* Constants */
.set WASM_REF_NULL,        0xFFFFFFFF
.set WASM_REF_MAX_TABLES,  8
.set WASM_REF_MAX_TABLE_SIZE, 65536
.set WASM_REF_GC_MAX_ROOTS, 4096

.bss
.align 4
.global wasm_ref_init_done
wasm_ref_init_done:
    .skip 4

/* Table descriptors: 8 tables, each {base_ptr, size, capacity} = 24 bytes */
.align 4
wasm_ref_tables:
    .skip WASM_REF_MAX_TABLES * 24   /* 192 bytes */

/* GC root set for reference tracing */
.align 4
wasm_ref_gc_roots:
    .skip WASM_REF_GC_MAX_ROOTS * 4  /* 16384 bytes */
wasm_ref_gc_root_count:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * wasm_ref_init — initialize reference types subsystem
 * Output: x0 = 0 (ok)
 * Zeros all table descriptors and GC root set
 * ----------------------------------------------------------------------------- */
.global wasm_ref_init
wasm_ref_init:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    /* Zero table descriptors */
    ldr     x19, =wasm_ref_tables
    mov     w20, #(WASM_REF_MAX_TABLES * 24)
1:  cbz     w20, 2f
    strb    wzr, [x19], #1
    sub     w20, w20, #1
    b       1b

2:  /* Zero GC roots */
    ldr     x19, =wasm_ref_gc_roots
    mov     w20, #(WASM_REF_GC_MAX_ROOTS * 4)
3:  cbz     w20, 4f
    strb    wzr, [x19], #1
    sub     w20, w20, #1
    b       3b

4:  /* Zero GC root count */
    ldr     x0, =wasm_ref_gc_root_count
    str     wzr, [x0]

    /* Mark initialized */
    ldr     x0, =wasm_ref_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_null — return null reference sentinel
 * Output: x0 = 0xFFFFFFFF (null ref)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_null
wasm_ref_null:
    mov     x0, #WASM_REF_NULL
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_is_null — test if a reference is null
 * Input: x0 = reference value
 * Output: x0 = 1 (null) or 0 (not null)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_is_null
wasm_ref_is_null:
    cmp     x0, #WASM_REF_NULL
    cset    x0, eq
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_func — create a funcref from a function index
 * Input: x0 = function index
 * Output: x0 = funcref value (= function index)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_func
wasm_ref_func:
    /* Funcref is just the function index as a 32-bit value */
    /* Mask to 32 bits for safety */
    and     x0, x0, #0xFFFFFFFF
    ret

/* Internal: get table descriptor pointer
 * Input: x0 = table index
 * Output: x0 = descriptor pointer, or x0 = 0 (invalid)
 */
.Lref_get_table_desc:
    cmp     x0, #WASM_REF_MAX_TABLES
    b.hs    .Lref_table_invalid
    ldr     x1, =wasm_ref_tables
    mov     x2, #24
    mul     x2, x0, x2
    add     x0, x1, x2
    ret
.Lref_table_invalid:
    mov     x0, #0
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_table_get — get a reference from a table
 * Input: x0 = table index, x1 = element index
 * Output: x0 = reference value, or -1 (out of bounds)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_table_get
wasm_ref_table_get:
    stp     x29, x30, [sp, #-16]!

    /* Save table index, get descriptor */
    mov     x2, x0              /* table index */
    mov     x0, x2
    bl      .Lref_get_table_desc
    cbz     x0, .Lref_tget_fail

    /* Check element index against table size */
    ldr     w3, [x0, #8]        /* size */
    cmp     x1, x3
    b.hs    .Lref_tget_fail

    /* Load reference from table base + index*4 */
    ldr     x3, [x0, #0]        /* base_ptr */
    ldr     w0, [x3, x1, lsl #2]

    b       .Lref_tget_ret

.Lref_tget_fail:
    mov     x0, #-1

.Lref_tget_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_table_set — set a reference in a table
 * Input: x0 = table index, x1 = element index, x2 = reference value
 * Output: x0 = 0 (ok), -1 (out of bounds or invalid table)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_table_set
wasm_ref_table_set:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x1             /* element index */
    mov     x20, x2             /* value */

    bl      .Lref_get_table_desc
    cbz     x0, .Lref_tset_fail

    ldr     w3, [x0, #8]        /* size */
    cmp     x19, x3
    b.hs    .Lref_tset_fail

    /* Store reference */
    ldr     x3, [x0, #0]        /* base_ptr */
    str     w20, [x3, x19, lsl #2]

    mov     x0, #0
    b       .Lref_tset_ret

.Lref_tset_fail:
    mov     x0, #-1

.Lref_tset_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_table_size — get the current size of a table
 * Input: x0 = table index
 * Output: x0 = table size, or -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_table_size
wasm_ref_table_size:
    stp     x29, x30, [sp, #-16]!

    bl      .Lref_get_table_desc
    cbz     x0, .Lref_tsize_fail

    ldr     w0, [x0, #8]        /* size */
    b       .Lref_tsize_ret

.Lref_tsize_fail:
    mov     x0, #-1

.Lref_tsize_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_table_grow — grow a table by delta elements
 * Input: x0 = table index, x1 = delta (new elements), x2 = init value
 * Output: x0 = old size (ok), -1 (failure: out of capacity or invalid)
 * Corresponds to WASM table.grow instruction
 * ----------------------------------------------------------------------------- */
.global wasm_ref_table_grow
wasm_ref_table_grow:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* table index */
    mov     x20, x1             /* delta */
    mov     x21, x2             /* init value */

    /* Get current descriptor */
    mov     x0, x19
    bl      .Lref_get_table_desc
    cbz     x0, .Lref_tgrow_fail

    mov     x22, x0             /* descriptor pointer */

    /* Load current size and capacity */
    ldr     w3, [x22, #8]       /* size */
    ldr     w4, [x22, #16]      /* capacity */

    /* Check: size + delta <= capacity */
    add     w5, w3, w20
    cmp     w5, w4
    b.hi    .Lref_tgrow_fail    /* unsigned higher = would exceed capacity */

    /* Fill new elements with init value */
    ldr     x5, [x22, #0]       /* base_ptr */
    mov     w6, w3              /* start offset = old size */
1:  cmp     w6, w5
    b.hs    2f
    str     w21, [x5, x6, lsl #2]
    add     w6, w6, #1
    b       1b

2:  /* Update size */
    str     w5, [x22, #8]

    /* Return old size */
    mov     x0, w3
    b       .Lref_tgrow_ret

.Lref_tgrow_fail:
    mov     x0, #-1

.Lref_tgrow_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_table_fill — fill a region of a table with a reference value
 * Input: x0 = table index, x1 = start index, x2 = value, x3 = count
 * Output: x0 = 0 (ok), -1 (out of bounds)
 * ----------------------------------------------------------------------------- */
.global wasm_ref_table_fill
wasm_ref_table_fill:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* table index */
    mov     x20, x1             /* start */
    mov     x21, x2             /* value */
    mov     x22, x3             /* count */

    mov     x0, x19
    bl      .Lref_get_table_desc
    cbz     x0, .Lref_tfill_fail

    /* Check: start + count <= size */
    ldr     w4, [x0, #8]        /* size */
    add     w5, w20, w22
    cmp     w5, w4
    b.hi    .Lref_tfill_fail

    /* Fill loop */
    ldr     x4, [x0, #0]        /* base_ptr */
    mov     w6, w20             /* current index */
1:  cmp     w6, w5
    b.hs    .Lref_tfill_ok
    str     w21, [x4, x6, lsl #2]
    add     w6, w6, #1
    b       1b

.Lref_tfill_ok:
    mov     x0, #0
    b       .Lref_tfill_ret

.Lref_tfill_fail:
    mov     x0, #-1

.Lref_tfill_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_ref_gc_trace — trace live references for GC
 * Input: none
 * Output: x0 = number of live (non-null) references found
 *
 * Scans all table entries and registers non-null refs with the GC root set.
 * Called by the garbage collector during mark phase.
 * ----------------------------------------------------------------------------- */
.global wasm_ref_gc_trace
wasm_ref_gc_trace:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, #0             /* live ref count */
    ldr     x20, =wasm_ref_gc_roots
    mov     x21, x20            /* GC root write pointer */
    ldr     x22, =wasm_ref_gc_roots
    add     x22, x22, #(WASM_REF_GC_MAX_ROOTS * 4)  /* end of root set */

    /* Iterate over all tables */
    mov     w0, #0              /* table index */
.Lref_gc_tbl_loop:
    cmp     w0, #WASM_REF_MAX_TABLES
    b.hs    .Lref_gc_done

    /* Get descriptor for table w0 */
    mov     x1, x0
    uxtx    x1, w1              /* zero-extend to 64-bit — use mov instead */
    mov     x0, w0
    bl      .Lref_get_table_desc
    cbz     x0, .Lref_gc_next_tbl

    /* Load base and size */
    ldr     x3, [x0, #0]        /* base_ptr */
    ldr     w4, [x0, #8]        /* size */

    /* Skip empty tables */
    cbz     w4, .Lref_gc_next_tbl

    mov     w5, #0              /* element index */
.Lref_gc_elem_loop:
    cmp     w5, w4
    b.hs    .Lref_gc_next_tbl

    /* Load element */
    ldr     w6, [x3, x5, lsl #2]

    /* Check if null */
    cmp     w6, #WASM_REF_NULL
    b.eq    .Lref_gc_elem_next

    /* Non-null: register as GC root if space */
    cmp     x21, x22
    b.hs    .Lref_gc_elem_next    /* root set full */
    str     w6, [x21], #4
    add     x19, x19, #1

.Lref_gc_elem_next:
    add     w5, w5, #1
    b       .Lref_gc_elem_loop

.Lref_gc_next_tbl:
    add     w0, w0, #1
    b       .Lref_gc_tbl_loop

.Lref_gc_done:
    /* Store updated root count */
    ldr     x0, =wasm_ref_gc_root_count
    str     w19, [x0]

    mov     x0, x19             /* return live count */

    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
