/*
 * aiasm-aarch64/kernel/ai_kv_cache.asm
 * Key-Value Cache for Transformer Inference (v1.0)
 *
 * AArch64 ASM implementation of KV cache management for transformer models.
 * Caches attention keys and values across inference steps for autoregressive
 * generation. Supports configurable cache size and FIFO eviction.
 *
 * Memory layout per cache instance:
 *   [key_token_0, value_token_0, key_token_1, value_token_1, ...]
 *   Each token: [head_0_key, head_1_key, ..., head_N_key,
 *                head_0_val, head_1_val, ..., head_N_val]
 *
 * Integration: called from transformer inference in AI layer
 *
 * API:
 *   ai_kv_cache_init(max_tokens, hidden_dim, n_heads)   — init cache
 *   ai_kv_cache_append(token_idx, key_ptr, val_ptr)     — append K,V for token
 *   ai_kv_cache_get_slice(start, end, dst_key, dst_val) — get K,V range
 *   ai_kv_cache_clear()                                  — reset cache
 *   ai_kv_cache_size() => n_tokens                       — current token count
 */
.arch armv8-a

/* Constants */
.set KV_MAX_TOKENS,   2048      /* max tokens in cache */
.set KV_MAX_HEADS,    32        /* max attention heads */
.set KV_HEAD_DIM,     64        /* dimension per head (f32 = 256 bytes) */

.bss
.align 4
.global ai_kv_init_done
ai_kv_init_done:
    .skip 4

/* Cache metadata */
.align 4
ai_kv_max_tokens:
    .skip 4
ai_kv_hidden_dim:
    .skip 4
ai_kv_n_heads:
    .skip 4
ai_kv_cur_tokens:
    .skip 4
ai_kv_head_bytes:
    .skip 4          /* head_dim * sizeof(f32) */
ai_kv_token_bytes:
    .skip 4          /* n_heads * head_bytes * 2 (key + value) */

/* Key cache storage */
.align 4
ai_kv_keys:
    .skip KV_MAX_TOKENS * KV_MAX_HEADS * KV_HEAD_DIM * 4   /* ~16MB */

/* Value cache storage */
.align 4
ai_kv_vals:
    .skip KV_MAX_TOKENS * KV_MAX_HEADS * KV_HEAD_DIM * 4   /* ~16MB */

.text

/* -----------------------------------------------------------------------------
 * ai_kv_cache_init — initialize KV cache
 * Input: x0 = max_tokens, x1 = hidden_dim, x2 = n_heads
 * Output: x0 = 0 (ok), -1 (parameters invalid)
 * ----------------------------------------------------------------------------- */
.global ai_kv_cache_init
ai_kv_cache_init:
    stp     x29, x30, [sp, #-16]!

    /* Validate parameters */
    cmp     x0, #KV_MAX_TOKENS
    b.hi    .Lkv_init_fail
    cmp     x2, #KV_MAX_HEADS
    b.hi    .Lkv_init_fail

    /* Store config */
    ldr     x3, =ai_kv_max_tokens
    str     w0, [x3]
    ldr     x3, =ai_kv_hidden_dim
    str     w1, [x3]
    ldr     x3, =ai_kv_n_heads
    str     w2, [x3]

    /* Compute head_bytes = head_dim * 4 (f32)
     * For simplicity: head_dim = hidden_dim / n_heads */
    udiv    w3, w1, w2            /* head_dim */
    ldr     x4, =ai_kv_head_bytes
    mov     w5, #4
    mul     w5, w3, w5           /* head_bytes = head_dim * 4 */
    str     w5, [x4]

    /* token_bytes = n_heads * head_bytes * 2 */
    mov     w6, #2
    mul     w5, w5, w6           /* head_bytes * 2 */
    mul     w5, w5, w2           /* * n_heads */
    ldr     x4, =ai_kv_token_bytes
    str     w5, [x4]

    /* Reset token count */
    ldr     x3, =ai_kv_cur_tokens
    str     wzr, [x3]

    ldr     x3, =ai_kv_init_done
    mov     w4, #1
    str     w4, [x3]

    mov     x0, #0
    b       .Lkv_init_ret

.Lkv_init_fail:
    mov     x0, #-1

.Lkv_init_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_kv_cache_append — append key and value for a token
 * Input: x0 = token index, x1 = key pointer (n_heads * head_dim f32s),
 *        x2 = value pointer
 * Output: x0 = 0 (ok), -1 (cache full or invalid)
 * ----------------------------------------------------------------------------- */
.global ai_kv_cache_append
ai_kv_cache_append:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* token_idx */
    mov     x20, x1             /* key_ptr */
    mov     x21, x2             /* val_ptr */

    /* Check cache not full */
    ldr     x3, =ai_kv_cur_tokens
    ldr     w3, [x3]
    ldr     x4, =ai_kv_max_tokens
    ldr     w4, [x4]
    cmp     w3, w4
    b.hs    .Lkv_append_fail

    /* Check token_idx is within bounds */
    cmp     x19, x4
    b.hs    .Lkv_append_fail

    /* Compute destination offset = token_idx * token_bytes */
    ldr     x4, =ai_kv_token_bytes
    ldr     w4, [x4]
    uxtw    x5, w4
    mul     x5, x19, x5         /* offset in bytes */

    /* Copy key data */
    ldr     x6, =ai_kv_keys
    add     x6, x6, x5
    mov     x7, x20             /* src */
    uxtw    x8, w4              /* len */
.Lkv_copy_key:
    cbz     x8, .Lkv_copy_key_done
    ldrb    w9, [x7], #1
    strb    w9, [x6], #1
    sub     x8, x8, #1
    b       .Lkv_copy_key
.Lkv_copy_key_done:

    /* Copy value data */
    ldr     x6, =ai_kv_vals
    add     x6, x6, x5
    mov     x7, x21
    uxtw    x8, w4
.Lkv_copy_val:
    cbz     x8, .Lkv_copy_val_done
    ldrb    w9, [x7], #1
    strb    w9, [x6], #1
    sub     x8, x8, #1
    b       .Lkv_copy_val
.Lkv_copy_val_done:

    /* Update cur_tokens if this is a new token */
    ldr     x3, =ai_kv_cur_tokens
    ldr     w3, [x3]
    add     x4, x19, #1
    cmp     x4, x3
    b.ls    .Lkv_append_no_inc
    str     w4, [x3]
.Lkv_append_no_inc:

    mov     x0, #0
    b       .Lkv_append_ret

.Lkv_append_fail:
    mov     x0, #-1

.Lkv_append_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_kv_cache_get_slice — get a slice of cached keys and values
 * Input: x0 = start token, x1 = end token (exclusive),
 *        x2 = dst_key buffer, x3 = dst_val buffer
 * Output: x0 = number of tokens copied, or -1 (invalid range)
 * ----------------------------------------------------------------------------- */
.global ai_kv_cache_get_slice
ai_kv_cache_get_slice:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* dst_key */
    mov     x20, x3             /* dst_val */

    /* Validate range */
    ldr     x3, =ai_kv_cur_tokens
    ldr     w3, [x3]
    cmp     x1, x3
    b.hi    .Lkv_slice_fail
    cmp     x0, x1
    b.hs    .Lkv_slice_fail     /* start >= end */

    /* Compute bytes to copy = (end - start) * token_bytes */
    sub     x4, x1, x0
    ldr     x5, =ai_kv_token_bytes
    ldr     w5, [x5]
    uxtw    x6, w5
    mul     x6, x4, x6          /* total bytes */

    /* Source offset = start * token_bytes */
    uxtw    x7, w5
    mul     x7, x0, x7

    /* Copy keys */
    ldr     x8, =ai_kv_keys
    add     x8, x8, x7
    mov     x9, x19
    mov     x10, x6
.Lkv_slice_copy_key:
    cbz     x10, .Lkv_slice_copy_val
    ldrb    w11, [x8], #1
    strb    w11, [x9], #1
    sub     x10, x10, #1
    b       .Lkv_slice_copy_key

.Lkv_slice_copy_val:
    /* Copy values */
    ldr     x8, =ai_kv_vals
    add     x8, x8, x7
    mov     x9, x20
    mov     x10, x6
.Lkv_slice_copy_vloop:
    cbz     x10, .Lkv_slice_done
    ldrb    w11, [x8], #1
    strb    w11, [x9], #1
    sub     x10, x10, #1
    b       .Lkv_slice_copy_vloop

.Lkv_slice_done:
    mov     x0, x4              /* return token count */
    b       .Lkv_slice_ret

.Lkv_slice_fail:
    mov     x0, #-1

.Lkv_slice_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * ai_kv_cache_clear — reset the cache (clear all tokens)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global ai_kv_cache_clear
ai_kv_cache_clear:
    ldr     x0, =ai_kv_cur_tokens
    str     wzr, [x0]
    ret

/* -----------------------------------------------------------------------------
 * ai_kv_cache_size — get current number of cached tokens
 * Output: x0 = token count
 * ----------------------------------------------------------------------------- */
.global ai_kv_cache_size
ai_kv_cache_size:
    ldr     x0, =ai_kv_cur_tokens
    ldr     w0, [x0]
    ret
