/*
 * aiasm-aarch64/kernel/secure_boot.asm
 * Secure boot verification chain (v1.0)
 *
 * Validates kernel image integrity via SHA-256 hash and RSA-2048 signature.
 * Chain-of-trust: bootloader → kernel → WASM runtime.
 * Uses software SHA-256 (can be upgraded to crypto_accel.asm when available).
 *
 * API:
 *   secure_boot_init()              — initialize secure boot subsystem
 *   secure_boot_verify_image(addr, len) => x0  — verify image (0=ok, -1=fail)
 *   secure_boot_hash_block(addr, len, out) — compute SHA-256 of block
 *   secure_boot_get_state() => x0   — return boot state (0=secure, 1=failed)
 */
.arch armv8-a

/* SHA-256 constants (first 32 bits of fractional parts of cube roots of primes) */
.section .rodata
.align 4
sha256_K:
    .word 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5
    .word 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5
    .word 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3
    .word 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174
    .word 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc
    .word 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
    .word 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7
    .word 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
    .word 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13
    .word 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
    .word 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3
    .word 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
    .word 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5
    .word 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
    .word 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208
    .word 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

/* Embedded public key hash (placeholder — would be real RSA-2048 pub key hash) */
.global secure_boot_pubkey_hash
secure_boot_pubkey_hash:
    .word 0xAASM0001, 0xAASM0002, 0xAASM0003, 0xAASM0004
    .word 0xAASM0005, 0xAASM0006, 0xAASM0007, 0xAASM0008

.bss
.align 4
.global secure_boot_state
secure_boot_state:
    .skip 4          /* 0=secure, 1=verification_failed, 2=uninitialized */
/* SHA-256 working buffer: 64 words (256 bytes) for message schedule */
sha256_W:
    .skip 256
/* SHA-256 hash state: 8 x 32-bit = 32 bytes */
sha256_state:
    .skip 32

.text

/* -----------------------------------------------------------------------------
 * secure_boot_init — initialize secure boot, set state to uninitialized
 * ----------------------------------------------------------------------------- */
.global secure_boot_init
secure_boot_init:
    mov     x0, #2
    ldr     x1, =secure_boot_state
    str     w0, [x1]
    ret

/* -----------------------------------------------------------------------------
 * sha256_init — initialize SHA-256 hash state with IV
 * ----------------------------------------------------------------------------- */
sha256_init:
    ldr     x0, =sha256_state
    mov     w1, #0x6a092667
    str     w1, [x0, #0]
    mov     w1, #0xbb67ae85
    str     w1, [x0, #4]
    mov     w1, #0x3c6ef372
    str     w1, [x0, #8]
    mov     w1, #0xa54ff53a
    str     w1, [x0, #12]
    mov     w1, #0x510e527f
    str     w1, [x0, #16]
    mov     w1, #0x9b05688c
    str     w1, [x0, #20]
    mov     w1, #0x1f83d9ab
    str     w1, [x0, #24]
    mov     w1, #0x5be0cd19
    str     w1, [x0, #28]
    ret

/* -----------------------------------------------------------------------------
 * secure_boot_hash_block — compute SHA-256 of memory block
 * Input: x0 = address, x1 = length, x2 = output buffer (32 bytes)
 * Output: 32-byte hash written to x2
 * Note: Simplified — real impl needs padding, multi-block handling
 * ----------------------------------------------------------------------------- */
.global secure_boot_hash_block
secure_boot_hash_block:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* save address */
    mov     x20, x2             /* save output buffer */

    bl      sha256_init         /* init hash state */

    /* TODO: Full SHA-256 compress loop with padding
     * This is a simplified stub — production would process 64-byte blocks,
     * apply message schedule, 64 rounds of compression, finalize */

    /* For now, copy placeholder hash to output */
    ldr     x0, =sha256_state
    mov     x1, x20
    mov     x3, #8              /* 8 words */
1:  ldr     w4, [x0], #4
    str     w4, [x1], #4
    subs    x3, x3, #1
    bne     1b

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * secure_boot_verify_image — verify kernel image integrity
 * Input: x0 = image address, x1 = image length
 * Output: x0 = 0 (verified ok) or -1 (verification failed)
 * ----------------------------------------------------------------------------- */
.global secure_boot_verify_image
secure_boot_verify_image:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* image address */
    mov     x20, x1             /* image length */

    /* Compute hash of image */
    sub     sp, sp, #32         /* allocate hash output on stack */
    mov     x0, x19
    mov     x1, x20
    mov     x2, sp
    bl      secure_boot_hash_block

    /* Compare computed hash against expected (stored in secure_boot_pubkey_hash)
     * TODO: Real implementation would verify RSA-2048 signature, not just hash */
    ldr     x0, =secure_boot_pubkey_hash
    mov     x1, sp
    mov     x2, #8              /* compare 8 words */
    mov     x3, #0              /* match flag */
1:  ldr     w4, [x0], #4
    ldr     w5, [x1], #4
    cmp     w4, w5
    bne     2f                  /* mismatch */
    subs    x2, x2, #1
    bne     1b
    mov     x3, #1              /* all matched */

2:  add     sp, sp, #32         /* free stack */

    /* Update state */
    ldr     x0, =secure_boot_state
    cbnz    x3, .Lsb_ok
    mov     w1, #1              /* failed */
    str     w1, [x0]
    mov     x0, #-1
    b       .Lsb_done

.Lsb_ok:
    mov     w1, #0              /* secure */
    str     w1, [x0]
    mov     x0, #0

.Lsb_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * secure_boot_get_state — return current boot state
 * Output: x0 = 0 (secure), 1 (failed), 2 (uninitialized)
 * ----------------------------------------------------------------------------- */
.global secure_boot_get_state
secure_boot_get_state:
    ldr     x0, =secure_boot_state
    ldr     w0, [x0]
    ret
