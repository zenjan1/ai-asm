/*
 * aiasm-aarch64/kernel/wasm_simd.asm
 * WASM SIMD128 Support (v1.0)
 *
 * AArch64 ASM implementation of WebAssembly SIMD128 operations.
 * Maps WASM v128 type to ARM NEON Q registers (v0-v31).
 * Provides core SIMD arithmetic, shuffle/swizzle, and comparison ops.
 * Runtime detection via ID_AA64ISAR0_EL1 for Advanced SIMD (NEON).
 *
 * WASM v128 layout: 128 bits = 4x32-bit or 8x16-bit or 16x8-bit lanes
 * ARM NEON mapping: v0-v31 in Q (128-bit) mode
 *
 * Integration: called from wasm_embed.asm when SIMD prefix (0xFD) detected
 *
 * API:
 *   wasm_simd_init() => x0           — detect NEON support, return 0/−1
 *   wasm_simd_supported() => x0      — check if SIMD is available (0/1)
 *   wasm_simd_exec(op, src1, src2, dst) — dispatch SIMD operation
 *       op codes: 0=v128.load, 1=v128.store, 2=i8x16.add, 3=i16x8.add,
 *                 4=i32x4.add, 5=f32x4.add, 6=i8x16.shuffle, 7=i8x16.swizzle,
 *                 8=i32x4.eq, 9=i32x4.lt_s, 10=f32x4.eq, 11=f32x4.lt
 */
.arch armv8-a

/* SIMD operation codes (match WASM spec opcodes after 0xFD prefix) */
.set SIMD_OP_LOAD,        0
.set SIMD_OP_STORE,       1
.set SIMD_OP_I8X16_ADD,   2
.set SIMD_OP_I16X8_ADD,   3
.set SIMD_OP_I32X4_ADD,   4
.set SIMD_OP_F32X4_ADD,   5
.set SIMD_OP_I8X16_SHUF,  6
.set SIMD_OP_I8X16_SWIZ,  7
.set SIMD_OP_I32X4_EQ,    8
.set SIMD_OP_I32X4_LT_S,  9
.set SIMD_OP_F32X4_EQ,   10
.set SIMD_OP_F32X4_LT,   11
.set SIMD_OP_COUNT,      12

.bss
.align 4
.global wasm_simd_hw_available
wasm_simd_hw_available:
    .skip 4
.global wasm_simd_init_done
wasm_simd_init_done:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * wasm_simd_init — detect NEON/Advanced SIMD support via ID_AA64ISAR0_EL1
 * Input: none
 * Output: x0 = 0 (ok), -1 (no NEON support)
 * Notes: Advanced SIMD is mandatory in ARMv8-A, but we check for safety
 * ----------------------------------------------------------------------------- */
.global wasm_simd_init
wasm_simd_init:
    stp     x29, x30, [sp, #-16]!

    /* Read ID_AA64ISAR0_EL1 */
    mrs     x0, id_aa64isar0_el1

    /* Check Advanced SIMD field (bits [7:4]) — 0b0001 = ASIMD present */
    ubfx    x1, x0, #4, #4
    ldr     x2, =wasm_simd_hw_available
    cmp     x1, #0
    cset    w3, ne
    str     w3, [x2]

    /* Mark as initialized */
    ldr     x2, =wasm_simd_init_done
    mov     w3, #1
    str     w3, [x2]

    /* Return 0 if NEON available, -1 otherwise */
    ldr     x0, =wasm_simd_hw_available
    ldr     w0, [x0]
    cbnz    w0, 2f
    mov     x0, #-1
    b       3f
2:  mov     x0, #0
3:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_simd_supported — check if SIMD operations are available
 * Input: none
 * Output: x0 = 1 (supported) or 0 (not supported)
 * ----------------------------------------------------------------------------- */
.global wasm_simd_supported
wasm_simd_supported:
    ldr     x0, =wasm_simd_hw_available
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * wasm_simd_exec — dispatch a SIMD128 operation
 * Input: x0 = op code (SIMD_OP_*), x1 = src1 ptr, x2 = src2 ptr, x3 = dst ptr
 * Output: x0 = 0 (ok), -1 (unsupported op or NEON not available)
 *
 * v128.load:  dst[0..15] = src1[0..15]  (128-bit load from linear memory)
 * v128.store: dst[0..15] = src1[0..15]  (128-bit store to linear memory)
 * i8x16.add:  dst[i] = src1[i] + src2[i] for i in 0..15
 * i16x8.add:  dst[i] = src1[i] + src2[i] for i in 0..7
 * i32x4.add:  dst[i] = src1[i] + src2[i] for i in 0..3
 * f32x4.add:  dst[i] = src1[i] + src2[i] for i in 0..3 (float)
 * i8x16.shuffle: permute src1 bytes using src2 as indices
 * i8x16.swizzle: src1[src2[i] if src2[i]<16 else 0] for i in 0..15
 * i32x4.eq:     dst[i] = (src1[i] == src2[i]) ? -1 : 0
 * i32x4.lt_s:   dst[i] = (src1[i] < src2[i]) ? -1 : 0 (signed)
 * f32x4.eq:     dst[i] = (src1[i] == src2[i]) ? all-ones : 0
 * f32x4.lt:     dst[i] = (src1[i] < src2[i]) ? all-ones : 0
 * ----------------------------------------------------------------------------- */
.global wasm_simd_exec
wasm_simd_exec:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    /* Check NEON support */
    ldr     x4, =wasm_simd_hw_available
    ldr     w4, [x4]
    cbz     w4, .Lsimd_fail

    mov     x19, x1             /* src1 */
    mov     x20, x2             /* src2 */
    mov     x21, x3             /* dst */
    mov     x22, x0             /* op code */

    /* Dispatch table via comparison */
    cmp     x22, #SIMD_OP_LOAD
    b.eq    .Lsimd_load

    cmp     x22, #SIMD_OP_STORE
    b.eq    .Lsimd_store

    cmp     x22, #SIMD_OP_I8X16_ADD
    b.eq    .Lsimd_i8x16_add

    cmp     x22, #SIMD_OP_I16X8_ADD
    b.eq    .Lsimd_i16x8_add

    cmp     x22, #SIMD_OP_I32X4_ADD
    b.eq    .Lsimd_i32x4_add

    cmp     x22, #SIMD_OP_F32X4_ADD
    b.eq    .Lsimd_f32x4_add

    cmp     x22, #SIMD_OP_I8X16_SHUF
    b.eq    .Lsimd_i8x16_shuffle

    cmp     x22, #SIMD_OP_I8X16_SWIZ
    b.eq    .Lsimd_i8x16_swizzle

    cmp     x22, #SIMD_OP_I32X4_EQ
    b.eq    .Lsimd_i32x4_eq

    cmp     x22, #SIMD_OP_I32X4_LT_S
    b.eq    .Lsimd_i32x4_lt_s

    cmp     x22, #SIMD_OP_F32X4_EQ
    b.eq    .Lsimd_f32x4_eq

    cmp     x22, #SIMD_OP_F32X4_LT
    b.eq    .Lsimd_f32x4_lt

    /* Unsupported op */
    b       .Lsimd_fail

/* v128.load: load 128 bits from linear memory into NEON, store to dst */
.Lsimd_load:
    ld1     {v0.16b}, [x19]
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* v128.store: load 128 bits from src, store to linear memory dst */
.Lsimd_store:
    ld1     {v0.16b}, [x19]
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* i8x16.add: 16-lane byte addition */
.Lsimd_i8x16_add:
    ld1     {v0.16b}, [x19]
    ld1     {v1.16b}, [x20]
    add     v0.16b, v0.16b, v1.16b
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* i16x8.add: 8-lane halfword addition */
.Lsimd_i16x8_add:
    ld1     {v0.8h}, [x19]
    ld1     {v1.8h}, [x20]
    add     v0.8h, v0.8h, v1.8h
    st1     {v0.8h}, [x21]
    b       .Lsimd_ok

/* i32x4.add: 4-lane word addition */
.Lsimd_i32x4_add:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    add     v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x21]
    b       .Lsimd_ok

/* f32x4.add: 4-lane float addition */
.Lsimd_f32x4_add:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    fadd    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x21]
    b       .Lsimd_ok

/* i8x16.shuffle: permute src1 bytes using src2 as lane indices */
.Lsimd_i8x16_shuffle:
    ld1     {v0.16b}, [x19]     /* src1: data to permute */
    ld1     {v1.16b}, [x20]     /* src2: shuffle indices */
    tbl     v0.16b, {v0.16b}, v1.16b
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* i8x16.swizzle: src1[src2[i] if src2[i]<16 else 0] */
.Lsimd_i8x16_swizzle:
    ld1     {v0.16b}, [x19]     /* src1: data */
    ld1     {v1.16b}, [x20]     /* src2: swizzle indices */
    /* Clamp indices: values >= 16 become 0 */
    movi    v2.16b, #15
    cmhs    v3.16b, v1.16b, v2.16b    /* v3 = (v1 >= 16) ? 0xFF : 0 */
    bif     v1.16b, v2.16b, v3.16b    /* if v3 set, use 0 (from v2 with bif) */
    /* tbl returns 0 for out-of-range indices naturally on ARM */
    tbl     v0.16b, {v0.16b}, v1.16b
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* i32x4.eq: compare equal, result = all-ones or zero */
.Lsimd_i32x4_eq:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    cmeq    v0.4s, v0.4s, v1.4s
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* i32x4.lt_s: signed less-than comparison */
.Lsimd_i32x4_lt_s:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    cmgt    v0.4s, v1.4s, v0.4s     /* a < b iff b > a */
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* f32x4.eq: float compare equal */
.Lsimd_f32x4_eq:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    fcmeq   v0.4s, v0.4s, v1.4s
    st1     {v0.16b}, [x21]
    b       .Lsimd_ok

/* f32x4.lt: float less-than */
.Lsimd_f32x4_lt:
    ld1     {v0.4s}, [x19]
    ld1     {v1.4s}, [x20]
    fcmgt   v0.4s, v1.4s, v0.4s     /* a < b iff b > a */
    st1     {v0.16b}, [x21]

.Lsimd_ok:
    mov     x0, #0
    b       .Lsimd_ret

.Lsimd_fail:
    mov     x0, #-1

.Lsimd_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
