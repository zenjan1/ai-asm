/*
 * aiasm-aarch64/kernel/ai_vector.asm
 * AI Vector Operations for Embeddings (v1.0)
 *
 * AArch64 ASM implementation of vector operations using ARM NEON.
 * Optimized for AI embedding computations: transformers, RAG, semantic search.
 * Supports f32 (4-wide) and i8 quantized (16-wide) vectors.
 *
 * Vector layout (128-bit NEON Q registers):
 *   f32: [v0, v1, v2, v3] — 4 floats per register
 *   i8:  [b0..b15]       — 16 bytes per register
 *
 * Integration: called from AI layer modules for embedding similarity
 *
 * API:
 *   ai_vector_init()                              — init (detect NEON)
 *   ai_vector_dot(a, b, len) => f32               — dot product
 *   ai_vector_cosine(a, b, len) => f32            — cosine similarity
 *   ai_vector_l2(a, b, len) => f32                — L2 (Euclidean) distance
 *   ai_vector_add(a, b, dst, len)                 — element-wise add
 *   ai_vector_sub(a, b, dst, len)                 — element-wise subtract
 *   ai_vector_norm(a, len) => f32                 — L2 norm
 *   ai_vector_scale(a, scalar, dst, len)          — scalar multiply
 */
.arch armv8-a

.bss
.align 4
.global ai_vector_neon_ok
ai_vector_neon_ok:
    .skip 4
.global ai_vector_init_done
ai_vector_init_done:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * ai_vector_init — detect NEON support
 * Output: x0 = 0 (ok), -1 (no NEON)
 * ----------------------------------------------------------------------------- */
.global ai_vector_init
ai_vector_init:
    stp     x29, x30, [sp, #-16]!

    mrs     x0, id_aa64isar0_el1
    ubfx    x1, x0, #4, #4
    ldr     x2, =ai_vector_neon_ok
    cmp     x1, #0
    cset    w3, ne
    str     w3, [x2]

    ldr     x2, =ai_vector_init_done
    mov     w3, #1
    str     w3, [x2]

    ldr     x0, =ai_vector_neon_ok
    ldr     w0, [x0]
    cbnz    w0, 2f
    mov     x0, #-1
    b       3f
2:  mov     x0, #0
3:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_dot — compute dot product of two f32 vectors
 * Input: x0 = vector a (f32 array), x1 = vector b (f32 array), x2 = length
 * Output: s0 = dot product (f32)
 * Uses NEON: process 4 f32 per iteration
 * ----------------------------------------------------------------------------- */
.global ai_vector_dot
ai_vector_dot:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* a */
    mov     x20, x1             /* b */

    /* Zero accumulator */
    movi    v0.4s, #0           /* acc = [0,0,0,0] */

    /* Process 4 f32 at a time */
.Lvec_dot_loop:
    cmp     x2, #4
    b.lt    .Lvec_dot_tail

    ld1     {v1.4s}, [x19], #16
    ld1     {v2.4s}, [x20], #16
    fmla    v0.4s, v1.4s, v2.4s     /* acc += a * b */
    sub     x2, x2, #4
    b       .Lvec_dot_loop

.Lvec_dot_tail:
    /* Handle remaining elements (0-3) */
    cbz     x2, .Lvec_dot_sum
    ldr     s1, [x19], #4
    ldr     s2, [x20], #4
    fmla    s0, s1, s2
    sub     x2, x2, #1
    b       .Lvec_dot_tail

.Lvec_dot_sum:
    /* Sum across lanes: v0 = [a,b,c,d] → s0 = a+b+c+d */
    faddp   v0.4s, v0.4s, v0.4s
    faddp   v0.4s, v0.4s, v0.4s
    /* Result in s0 */

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_norm — compute L2 norm of a f32 vector
 * Input: x0 = vector a, x1 = length
 * Output: s0 = L2 norm (f32) = sqrt(dot(a, a))
 * ----------------------------------------------------------------------------- */
.global ai_vector_norm
ai_vector_norm:
    stp     x29, x30, [sp, #-16]!

    /* Compute dot(a, a) */
    mov     x1, x0              /* b = a */
    bl      ai_vector_dot

    /* s0 = sqrt(s0) */
    fsqrt   s0, s0

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_cosine — compute cosine similarity between two f32 vectors
 * Input: x0 = vector a, x1 = vector b, x2 = length
 * Output: s0 = cosine similarity = dot(a,b) / (norm(a) * norm(b))
 * ----------------------------------------------------------------------------- */
.global ai_vector_cosine
ai_vector_cosine:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* a */
    mov     x20, x1             /* b */
    mov     x21, x2             /* len */

    /* dot(a, b) */
    mov     x0, x19
    mov     x1, x20
    mov     x2, x21
    bl      ai_vector_dot
    fmov    s3, s0              /* s3 = dot_ab */

    /* norm(a) */
    mov     x0, x19
    mov     x1, x21
    bl      ai_vector_norm
    fmov    s4, s0              /* s4 = norm_a */

    /* norm(b) */
    mov     x0, x20
    mov     x1, x21
    bl      ai_vector_norm
    fmov    s5, s0              /* s5 = norm_b */

    /* cosine = dot / (norm_a * norm_b) */
    fmul    s6, s4, s5          /* denom = norm_a * norm_b */
    fdiv    s0, s3, s6          /* result = dot / denom */

    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_l2 — compute L2 (Euclidean) distance between two f32 vectors
 * Input: x0 = vector a, x1 = vector b, x2 = length
 * Output: s0 = L2 distance = sqrt(sum((a[i]-b[i])^2))
 * ----------------------------------------------------------------------------- */
.global ai_vector_l2
ai_vector_l2:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* a */
    mov     x20, x1             /* b */

    /* Zero accumulator */
    movi    v0.4s, #0

.Lvec_l2_loop:
    cmp     x2, #4
    b.lt    .Lvec_l2_tail

    ld1     {v1.4s}, [x19], #16
    ld1     {v2.4s}, [x20], #16
    fsub    v3.4s, v1.4s, v2.4s     /* diff = a - b */
    fmla    v0.4s, v3.4s, v3.4s     /* acc += diff^2 */
    sub     x2, x2, #4
    b       .Lvec_l2_loop

.Lvec_l2_tail:
    cbz     x2, .Lvec_l2_sum
    ldr     s1, [x19], #4
    ldr     s2, [x20], #4
    fsub    s3, s1, s2
    fmla    s0, s3, s3
    sub     x2, x2, #1
    b       .Lvec_l2_tail

.Lvec_l2_sum:
    faddp   v0.4s, v0.4s, v0.4s
    faddp   v0.4s, v0.4s, v0.4s
    fsqrt   s0, s0              /* sqrt(sum of squares) */

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_add — element-wise addition of two f32 vectors
 * Input: x0 = a, x1 = b, x2 = dst, x3 = length
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_vector_add
ai_vector_add:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* dst */

.Lvec_add_loop:
    cmp     x3, #4
    b.lt    .Lvec_add_tail
    ld1     {v0.4s}, [x0], #16
    ld1     {v1.4s}, [x1], #16
    fadd    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x19], #16
    sub     x3, x3, #4
    b       .Lvec_add_loop

.Lvec_add_tail:
    cbz     x3, .Lvec_add_done
    ldr     s0, [x0], #4
    ldr     s1, [x1], #4
    fadd    s0, s0, s1
    str     s0, [x19], #4
    sub     x3, x3, #1
    b       .Lvec_add_tail

.Lvec_add_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_sub — element-wise subtraction (a - b)
 * Input: x0 = a, x1 = b, x2 = dst, x3 = length
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_vector_sub
ai_vector_sub:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2

.Lvec_sub_loop:
    cmp     x3, #4
    b.lt    .Lvec_sub_tail
    ld1     {v0.4s}, [x0], #16
    ld1     {v1.4s}, [x1], #16
    fsub    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x19], #16
    sub     x3, x3, #4
    b       .Lvec_sub_loop

.Lvec_sub_tail:
    cbz     x3, .Lvec_sub_done
    ldr     s0, [x0], #4
    ldr     s1, [x1], #4
    fsub    s0, s0, s1
    str     s0, [x19], #4
    sub     x3, x3, #1
    b       .Lvec_sub_tail

.Lvec_sub_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_vector_scale — multiply vector by scalar
 * Input: x0 = src, s1 = scalar (f32), x2 = dst, x3 = length
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_vector_scale
ai_vector_scale:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* dst */
    fmov    v1.4s, s1           /* broadcast scalar to all lanes */

.Lvec_scale_loop:
    cmp     x3, #4
    b.lt    .Lvec_scale_tail
    ld1     {v0.4s}, [x0], #16
    fmul    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x19], #16
    sub     x3, x3, #4
    b       .Lvec_scale_loop

.Lvec_scale_tail:
    cbz     x3, .Lvec_scale_done
    ldr     s0, [x0], #4
    fmul    s0, s0, s1
    str     s0, [x19], #4
    sub     x3, x3, #1
    b       .Lvec_scale_tail

.Lvec_scale_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
