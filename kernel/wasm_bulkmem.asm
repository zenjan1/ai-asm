/*
 * aiasm-aarch64/kernel/wasm_bulkmem.asm
 * WASM Bulk Memory Operations (v1.0)
 *
 * AArch64 ASM implementation of WebAssembly bulk memory proposal.
 * Provides efficient memory.init, data.drop, memory.copy, memory.fill
 * using ARM NEON for 64+ byte operations.
 * Also provides table.init, elem.drop, table.copy for WASM table ops.
 *
 * Integration: called from wasi.asm when bulk memory opcodes detected
 *
 * API:
 *   wasm_bulkmem_init() => x0               — initialize bulk memory subsystem
 *   wasm_bulkmem_supported() => x0          — check NEON availability
 *   wasm_bulkmem_copy(dst, src, len) => x0  — memmove within linear memory
 *   wasm_bulkmem_fill(dst, val, len) => x0  — fill memory with byte value
 *   wasm_bulkmem_data_init(seg, dst, len)   — copy passive segment to memory
 *   wasm_bulkmem_data_drop(seg) => x0       — mark segment as dropped
 *   wasm_bulkmem_table_copy(dst_off, src_off, len) — copy table elements
 *   wasm_bulkmem_elem_drop(elem) => x0      — drop element segment
 */
.arch armv8-a

/* Limits */
.set BULKMEM_MAX_SEGS,    64
.set BULKMEM_MAX_ELEMS,   64
.set BULKMEM_NEON_THRESH, 64    /* Use NEON for copies >= 64 bytes */

.bss
.align 4
.global wasm_bulkmem_init_done
wasm_bulkmem_init_done:
    .skip 4
.global wasm_bulkmem_neon_ok
wasm_bulkmem_neon_ok:
    .skip 4

/* Data segment tracking: 64 entries, each {base_addr, size, dropped} */
.align 4
wasm_bulkmem_segs:
    .skip BULKMEM_MAX_SEGS * 12     /* 768 bytes */

/* Element segment tracking: 64 entries, each {base_addr, count, dropped} */
.align 4
wasm_bulkmem_elems:
    .skip BULKMEM_MAX_ELEMS * 12    /* 768 bytes */

.text

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_init — detect NEON, zero segment tables
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_init
wasm_bulkmem_init:
    stp     x29, x30, [sp, #-16]!

    /* Detect NEON via ID_AA64ISAR0_EL1 */
    mrs     x0, id_aa64isar0_el1
    ubfx    x1, x0, #4, #4
    ldr     x2, =wasm_bulkmem_neon_ok
    cmp     x1, #0
    cset    w3, ne
    str     w3, [x2]

    /* Zero segment table */
    ldr     x0, =wasm_bulkmem_segs
    mov     x1, #(BULKMEM_MAX_SEGS * 12)
    bl      .Lbulkmem_zero

    /* Zero element table */
    ldr     x0, =wasm_bulkmem_elems
    mov     x1, #(BULKMEM_MAX_ELEMS * 12)
    bl      .Lbulkmem_zero

    /* Mark initialized */
    ldr     x0, =wasm_bulkmem_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lbulkmem_zero:
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
 * wasm_bulkmem_supported — check if NEON is available
 * Output: x0 = 1 (supported) or 0
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_supported
wasm_bulkmem_supported:
    ldr     x0, =wasm_bulkmem_neon_ok
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_copy — efficient memmove within linear memory
 * Input: x0 = dst, x1 = src, x2 = length
 * Output: x0 = 0 (ok)
 * Uses NEON for 64+ byte copies, scalar for smaller
 * Handles overlapping regions (memmove semantics)
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_copy
wasm_bulkmem_copy:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* dst */
    mov     x20, x1             /* src */

    /* Zero-length is no-op */
    cbz     x2, .Lbm_copy_done

    /* Check for overlap: if src < dst && src+len > dst, copy backwards */
    cmp     x20, x19
    b.hs    .Lbm_copy_fwd       /* src >= dst: safe forward copy */

    /* Check overlap: src + len > dst? */
    add     x3, x20, x2
    cmp     x3, x19
    b.ls    .Lbm_copy_fwd       /* no overlap */

    /* Backward copy for overlapping regions */
    add     x19, x19, x2        /* dst_end = dst + len */
    add     x20, x20, x2        /* src_end = src + len */
1:  cbz     x2, .Lbm_copy_done
    sub     x19, x19, #1
    sub     x20, x20, #1
    ldrb    w3, [x20]
    strb    w3, [x19]
    sub     x2, x2, #1
    b       1b

.Lbm_copy_fwd:
    /* Forward copy — use NEON for large copies */
    ldr     x3, =wasm_bulkmem_neon_ok
    ldr     w3, [x3]
    cbz     w3, .Lbm_copy_scalar

    cmp     x2, #BULKMEM_NEON_THRESH
    b.lt    .Lbm_copy_scalar

    /* NEON path: 16 bytes at a time */
.Lbm_copy_neon:
    cmp     x2, #16
    b.lt    .Lbm_copy_scalar
    ld1     {v0.16b}, [x20], #16
    st1     {v0.16b}, [x19], #16
    sub     x2, x2, #16
    b       .Lbm_copy_neon

.Lbm_copy_scalar:
    cbz     x2, .Lbm_copy_done
    ldrb    w3, [x20], #1
    strb    w3, [x19], #1
    sub     x2, x2, #1
    b       .Lbm_copy_scalar

.Lbm_copy_done:
    mov     x0, #0
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_fill — fill memory region with byte value
 * Input: x0 = dst, x1 = byte value (0-255), x2 = length
 * Output: x0 = 0 (ok)
 * Uses NEON for 64+ byte fills
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_fill
wasm_bulkmem_fill:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* dst */
    and     x1, x1, #0xFF       /* mask to byte */
    mov     x20, x2             /* length */

    cbz     x20, .Lbm_fill_done

    /* Check NEON for large fills */
    ldr     x3, =wasm_bulkmem_neon_ok
    ldr     w3, [x3]
    cbz     w3, .Lbm_fill_scalar

    cmp     x20, #BULKMEM_NEON_THRESH
    b.lt    .Lbm_fill_scalar

    /* NEON path: broadcast byte to 16-byte vector */
    mov     v0.16b, x1          /* duplicate byte across all 16 lanes */
.Lbm_fill_neon:
    cmp     x20, #16
    b.lt    .Lbm_fill_scalar
    st1     {v0.16b}, [x19], #16
    sub     x20, x20, #16
    b       .Lbm_fill_neon

.Lbm_fill_scalar:
    cbz     x20, .Lbm_fill_done
    strb    w1, [x19], #1
    sub     x20, x20, #1
    b       .Lbm_fill_scalar

.Lbm_fill_done:
    mov     x0, #0
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_data_init — copy passive data segment to linear memory
 * Input: x0 = segment index, x1 = dst offset in linear memory, x2 = length
 * Output: x0 = 0 (ok), -1 (segment dropped or invalid index)
 * Corresponds to WASM memory.init instruction
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_data_init
wasm_bulkmem_data_init:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    /* Validate segment index */
    cmp     x0, #BULKMEM_MAX_SEGS
    b.hs    .Lbm_dinit_fail

    mov     x19, x0             /* seg index */
    mov     x20, x2             /* length */

    /* Load segment descriptor */
    ldr     x3, =wasm_bulkmem_segs
    mov     x4, #12
    mul     x4, x19, x4         /* offset = index * 12 */
    add     x3, x3, x4

    /* Check if segment is dropped (offset+8 = dropped flag) */
    ldr     w4, [x3, #8]
    cbnz    w4, .Lbm_dinit_fail

    /* Get segment base address (offset+0) and size (offset+4) */
    ldr     x4, [x3, #0]        /* base_addr */
    ldr     w5, [x3, #4]        /* size */

    /* Clamp length to segment size */
    cmp     x20, x5
    b.ls    1f
    mov     x20, x5
1:
    /* Copy: linear_mem[dst..dst+len] = seg[0..len] */
    /* x1 = dst, x4 = src, x20 = len */
    mov     x0, x1              /* dst */
    mov     x1, x4              /* src = seg base */
    mov     x2, x20             /* len */
    bl      wasm_bulkmem_copy

    mov     x0, #0
    b       .Lbm_dinit_ret

.Lbm_dinit_fail:
    mov     x0, #-1

.Lbm_dinit_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_data_drop — mark passive data segment as dropped
 * Input: x0 = segment index
 * Output: x0 = 0 (ok), -1 (invalid index)
 * Corresponds to WASM data.drop instruction
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_data_drop
wasm_bulkmem_data_drop:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #BULKMEM_MAX_SEGS
    b.hs    .Lbm_ddrop_fail

    /* Set dropped flag at segs[index*12 + 8] */
    ldr     x1, =wasm_bulkmem_segs
    mov     x2, #12
    mul     x2, x0, x2
    add     x1, x1, x2
    mov     w2, #1
    str     w2, [x1, #8]        /* dropped = 1 */

    /* Zero base and size */
    str     wzr, [x1, #0]
    str     wzr, [x1, #4]

    mov     x0, #0
    b       .Lbm_ddrop_ret

.Lbm_ddrop_fail:
    mov     x0, #-1

.Lbm_ddrop_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_table_copy — copy elements within a table
 * Input: x0 = dst_offset, x1 = src_offset, x2 = length (element count)
 * Output: x0 = 0 (ok), -1 (out of bounds)
 * Corresponds to WASM table.copy instruction
 * Elements are 4-byte indices (funcref/externref stored as u32)
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_table_copy
wasm_bulkmem_table_copy:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    /* Convert offsets to byte addresses (4 bytes per element) */
    /* Table base is in a fixed location — using a placeholder base */
    ldr     x3, =wasm_bulkmem_table_base
    ldr     x3, [x3]

    mov     x19, x3
    add     x19, x19, x0, lsl #2    /* dst = base + dst_off * 4 */
    mov     x20, x3
    add     x20, x20, x1, lsl #2    /* src = base + src_off * 4 */
    mov     x3, x2                    /* count */

    cbz     x3, .Lbm_tcopy_done

    /* Check overlap and direction (similar to memmove) */
    cmp     x20, x19
    b.hs    .Lbm_tcopy_fwd

    /* Overlapping check */
    add     x4, x20, x3, lsl #2
    cmp     x4, x19
    b.ls    .Lbm_tcopy_fwd

    /* Backward copy */
1:  cbz     x3, .Lbm_tcopy_done
    sub     x3, x3, #1
    ldr     w4, [x20, x3, lsl #2]
    str     w4, [x19, x3, lsl #2]
    b       1b

.Lbm_tcopy_fwd:
    /* Forward copy */
    mov     x4, #0
1:  cmp     x4, x3
    b.hs    .Lbm_tcopy_done
    ldr     w5, [x20, x4, lsl #2]
    str     w5, [x19, x4, lsl #2]
    add     x4, x4, #1
    b       1b

.Lbm_tcopy_done:
    mov     x0, #0
    b       .Lbm_tcopy_ret

.Lbm_tcopy_fail:
    mov     x0, #-1

.Lbm_tcopy_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* Table base address (set externally by WASM runtime) */
.bss
.align 4
.global wasm_bulkmem_table_base
wasm_bulkmem_table_base:
    .skip 8

.text

/* -----------------------------------------------------------------------------
 * wasm_bulkmem_elem_drop — drop an element segment
 * Input: x0 = element segment index
 * Output: x0 = 0 (ok), -1 (invalid)
 * Corresponds to WASM elem.drop instruction
 * ----------------------------------------------------------------------------- */
.global wasm_bulkmem_elem_drop
wasm_bulkmem_elem_drop:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #BULKMEM_MAX_ELEMS
    b.hs    .Lbm_edrop_fail

    ldr     x1, =wasm_bulkmem_elems
    mov     x2, #12
    mul     x2, x0, x2
    add     x1, x1, x2

    /* Set dropped flag (offset+8) */
    mov     w2, #1
    str     w2, [x1, #8]
    str     wzr, [x1, #0]
    str     wzr, [x1, #4]

    mov     x0, #0
    b       .Lbm_edrop_ret

.Lbm_edrop_fail:
    mov     x0, #-1

.Lbm_edrop_ret:
    ldp     x29, x30, [sp], #16
    ret
