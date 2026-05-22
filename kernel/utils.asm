/*
 * aiasm-aarch64/kernel/utils.asm
 * Utility functions: memset, memcpy, strlen, itoa
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: memset
 * Description: Fill memory with a byte value
 * Input: x0 = destination, w1 = byte, x2 = length
 * Output: x0 = destination
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global memset
memset:
    stp     x29, x30, [sp, #-16]!
    mov     x3, x0              /* save dest */
    cbz     x2, 1f
2:
    strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 2b
1:
    mov     x0, x3
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: memcpy
 * Description: Copy memory
 * Input: x0 = dest, x1 = src, x2 = length
 * Output: x0 = dest
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global memcpy
memcpy:
    stp     x29, x30, [sp, #-16]!
    mov     x3, x0              /* save dest */
    cbz     x2, 1f
2:
    ldrb    w3, [x1], #1
    strb    w3, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 2b
1:
    mov     x0, x3
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: strlen
 * Description: Get string length
 * Input: x0 = string pointer
 * Output: x0 = length
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global strlen
strlen:
    stp     x29, x30, [sp, #-16]!
    mov     x1, x0
1:
    ldrb    w2, [x1], #1
    cbnz    w2, 1b
    sub     x0, x1, x0
    sub     x0, x0, #1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: itoa_buf
 * Description: Convert u64 to decimal string in caller buffer
 * Input: x0 = number, x1 = buffer (must hold 21 bytes)
 * Output: x0 = buffer pointer
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global itoa_buf
itoa_buf:
    stp     x29, x30, [sp, #-16]!
    cbz     x0, itoa_zer
    mov     x2, x1              /* save buf */
    mov     x3, #0              /* digit count */
    mov     x5, #10             /* divisor */
1:
    udiv    x4, x0, x5
    msub    x0, x4, x5, x0      /* x0 = remainder */
    add     x0, x0, #'0'
    sub     sp, sp, #1
    strb    w0, [sp]
    add     x3, x3, #1
    mov     x0, x4
    cbnz    x0, 1b
2:
    cbz     x3, itoa_nul
    ldrb    w0, [sp], #1
    strb    w0, [x2], #1
    sub     x3, x3, #1
    b       2b
itoa_nul:
    strb    wzr, [x2]
    mov     x0, x1
    ldp     x29, x30, [sp], #16
    ret

itoa_zer:
    mov     w0, #'0'
    strb    w0, [x1]
    strb    wzr, [x1, #1]
    mov     x0, x1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: strcmp
 * Description: Compare two null-terminated strings
 * Input: x0 = a, x1 = b
 * Output: x0 = 0 if equal
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global strcmp
strcmp:
    stp     x29, x30, [sp, #-16]!
1:
    ldrb    w2, [x0], #1
    ldrb    w3, [x1], #1
    subs    w4, w2, w3
    b.ne    2f
    cbnz    w2, 1b
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
2:
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret
