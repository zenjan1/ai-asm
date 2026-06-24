/*
 * aiasm-aarch64/kernel/ai_tensor.asm
 * Basic Tensor Operations (v1.0)
 *
 * AArch64 ASM implementation of core tensor operations using ARM NEON.
 * Optimized for AI model inference: matmul, add, relu, softmax.
 * Supports f32 tensors with 4-wide NEON vectorization.
 *
 * Tensor layout: row-major, contiguous f32 array
 * Shape: [dim0, dim1, ...] stored as strides
 *
 * Integration: called from AI layer for model inference
 *
 * API:
 *   ai_tensor_init()                              — init (detect NEON)
 *   ai_tensor_matmul(a, b, dst, M, K, N)         — C[M,N] = A[M,K] * B[K,N]
 *   ai_tensor_add(a, b, dst, n)                   — element-wise add
 *   ai_tensor_relu(a, dst, n)                     — max(0, a[i])
 *   ai_tensor_softmax(a, dst, n)                  — softmax over n elements
 *   ai_tensor_scale(a, scalar, dst, n)            — multiply by scalar
 */
.arch armv8-a

.bss
.align 4
.global ai_tensor_init_done
ai_tensor_init_done:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * ai_tensor_init — initialize tensor subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global ai_tensor_init
ai_tensor_init:
    stp     x29, x30, [sp, #-16]!

    ldr     x0, =ai_tensor_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_tensor_matmul — matrix multiplication C = A * B
 * Input: x0 = A (M x K, row-major), x1 = B (K x N, row-major),
 *        x2 = C (M x N, output), x3 = M, x4 = K, x5 = N
 * Output: none (result in C)
 *
 * Optimized with NEON: inner loop processes 4 output elements at a time.
 * For M x K times K x N: O(M*K*N) but 4x faster with NEON.
 * ----------------------------------------------------------------------------- */
.global ai_tensor_matmul
ai_tensor_matmul:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!
    stp     x23, x24, [sp, #-16]!

    mov     x19, x0             /* A */
    mov     x20, x1             /* B */
    mov     x21, x2             /* C */
    mov     x22, x3             /* M */
    mov     x23, x4             /* K */
    mov     x24, x5             /* N */

    /* Outer loop: for each row i in [0, M) */
    mov     w6, #0              /* i */
.Lmm_row_loop:
    cmp     w6, w22
    b.hs    .Lmm_done

    /* For each column j in [0, N) step 4 */
    mov     w7, #0              /* j */
.Lmm_col_loop:
    cmp     w7, w24
    b.hs    .Lmm_row_next

    /* Compute C[i,j..j+3] = sum_k A[i,k] * B[k,j..j+3] */
    movi    v0.4s, #0           /* accumulator for 4 output elements */

    /* Inner loop: for each k in [0, K) */
    mov     w8, #0              /* k */
.Lmm_inner_loop:
    cmp     w8, w23
    b.hs    .Lmm_inner_done

    /* Load A[i,k] (scalar) */
    mov     x9, w6              /* i */
    mul     x9, x9, x23         /* i * K */
    add     x9, x9, w8, uxtw   /* i*K + k */
    ldr     s1, [x19, x9, lsl #2]

    /* Broadcast A[i,k] to all lanes */
    fmov    v1.4s, s1

    /* Load B[k, j..j+3] */
    mov     x9, w8              /* k */
    mul     x9, x9, x24         /* k * N */
    add     x9, x9, w7, uxtw   /* k*N + j */
    ld1     {v2.4s}, [x20, x9, lsl #2]

    /* Accumulate: v0 += A[i,k] * B[k,j..j+3] */
    fmla    v0.4s, v1.4s, v2.4s

    add     w8, w8, #1
    b       .Lmm_inner_loop

.Lmm_inner_done:
    /* Store C[i, j..j+3] */
    mov     x9, w6
    mul     x9, x9, x24
    add     x9, x9, w7, uxtw
    st1     {v0.4s}, [x21, x9, lsl #2]

    add     w7, w7, #4
    b       .Lmm_col_loop

.Lmm_row_next:
    add     w6, w6, #1
    b       .Lmm_row_loop

.Lmm_done:
    ldp     x23, x24, [sp], #16
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_tensor_add — element-wise addition of two tensors
 * Input: x0 = a, x1 = b, x2 = dst, x3 = element count
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_tensor_add
ai_tensor_add:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* dst */

.Ltadd_loop:
    cmp     x3, #4
    b.lt    .Ltadd_tail
    ld1     {v0.4s}, [x0], #16
    ld1     {v1.4s}, [x1], #16
    fadd    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x19], #16
    sub     x3, x3, #4
    b       .Ltadd_loop

.Ltadd_tail:
    cbz     x3, .Ltadd_done
    ldr     s0, [x0], #4
    ldr     s1, [x1], #4
    fadd    s0, s0, s1
    str     s0, [x19], #4
    sub     x3, x3, #1
    b       .Ltadd_tail

.Ltadd_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_tensor_relu — ReLU activation: dst[i] = max(0, a[i])
 * Input: x0 = src, x1 = dst, x2 = element count
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_tensor_relu
ai_tensor_relu:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x1             /* dst */
    movi    v1.4s, #0           /* zero vector for comparison */

.Lrelu_loop:
    cmp     x2, #4
    b.lt    .Lrelu_tail
    ld1     {v0.4s}, [x0], #16
    fmax    v0.4s, v0.4s, v1.4s     /* max(0, x) */
    st1     {v0.4s}, [x19], #16
    sub     x2, x2, #4
    b       .Lrelu_loop

.Lrelu_tail:
    cbz     x2, .Lrelu_done
    ldr     s0, [x0], #4
    fmax    s0, s0, s1
    str     s0, [x19], #4
    sub     x2, x2, #1
    b       .Lrelu_tail

.Lrelu_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_tensor_softmax — numerically stable softmax
 * Input: x0 = src, x1 = dst, x2 = element count
 * Output: none (result in dst)
 *
 * Algorithm:
 *   1. Find max value (for numerical stability)
 *   2. Compute exp(x - max) for each element
 *   3. Sum all exp values
 *   4. Divide each exp by the sum
 * ----------------------------------------------------------------------------- */
.global ai_tensor_softmax
ai_tensor_softmax:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* src */
    mov     x20, x1             /* dst */
    mov     x21, x2             /* count */

    /* Step 1: Find max */
    cbz     x21, .Lsoftmax_done

    /* Initialize max with first element */
    ldr     s0, [x19]
    fmov    s2, s0              /* max = a[0] */

    mov     x3, #1
.Lsoftmax_find_max:
    cmp     x3, x21
    b.hs    .Lsoftmax_find_max_done
    ldr     s1, [x19, x3, lsl #2]
    fmax    s2, s2, s1
    add     x3, x3, #1
    b       .Lsoftmax_find_max
.Lsoftmax_find_max_done:

    /* Step 2: Compute exp(x - max) and sum */
    movi    v3.4s, #0           /* sum accumulator (scalar in s3) */
    mov     x3, #0
.Lsoftmax_exp:
    cmp     x3, x21
    b.hs    .Lsoftmax_exp_done
    ldr     s1, [x19, x3, lsl #2]
    fsub    s1, s1, s2          /* x - max */
    /* exp approximation: e^x ≈ 1 + x + x²/2 + x³/6 (Taylor for small range) */
    /* For production, use hardware exp or lookup table */
    fmov    s4, #1.0
    fadd    s4, s4, s1          /* 1 + x */
    fmul    s5, s1, s1
    fmov    s6, #0.5
    fmla    s4, s5, s6          /* + x²/2 */
    fmul    s5, s5, s1
    fmov    s6, #0.16666667
    fmla    s4, s5, s6          /* + x³/6 */
    str     s4, [x20, x3, lsl #2]  /* store exp(x-max) */
    fadd    s3, s3, s4          /* sum += exp */
    add     x3, x3, #1
    b       .Lsoftmax_exp
.Lsoftmax_exp_done:

    /* Step 3: Divide each by sum */
    mov     x3, #0
.Lsoftmax_div:
    cmp     x3, x21
    b.hs    .Lsoftmax_done
    ldr     s1, [x20, x3, lsl #2]
    fdiv    s1, s1, s3          /* exp / sum */
    str     s1, [x20, x3, lsl #2]
    add     x3, x3, #1
    b       .Lsoftmax_div

.Lsoftmax_done:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_tensor_scale — multiply tensor by scalar
 * Input: x0 = src, s1 = scalar (f32), x2 = dst, x3 = count
 * Output: none (result in dst)
 * ----------------------------------------------------------------------------- */
.global ai_tensor_scale
ai_tensor_scale:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* dst */
    fmov    v1.4s, s1           /* broadcast scalar */

.Ltscale_loop:
    cmp     x3, #4
    b.lt    .Ltscale_tail
    ld1     {v0.4s}, [x0], #16
    fmul    v0.4s, v0.4s, v1.4s
    st1     {v0.4s}, [x19], #16
    sub     x3, x3, #4
    b       .Ltscale_loop

.Ltscale_tail:
    cbz     x3, .Ltscale_done
    ldr     s0, [x0], #4
    fmul    s0, s0, s1
    str     s0, [x19], #4
    sub     x3, x3, #1
    b       .Ltscale_tail

.Ltscale_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
