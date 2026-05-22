/*
 * aiasm-aarch64/kernel/utils.asm
 * Utility functions for AI-ASM AArch64
 * Memory operations, string operations, number printing
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: memset
 * Description: Fill memory with a byte value
 * Input: x0 = destination pointer
 *        w1 = byte value
 *        x2 = count
 * Output: x0 = original destination
 * Clobbered: x0, x1, x2, x3
 * ----------------------------------------------------------------------------- */
.global memset
memset:
    mov     x3, x0              /* Save original pointer */
    cbz     x2, 2f
1:
    strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:
    mov     x0, x3
    ret

/* -----------------------------------------------------------------------------
 * Function: memcpy
 * Description: Copy memory from source to destination
 * Input: x0 = destination, x1 = source, x2 = count
 * Output: x0 = original destination
 * Clobbered: x0, x1, x2, x3
 * ----------------------------------------------------------------------------- */
.global memcpy
memcpy:
    mov     x3, x0              /* Save original dest */
    cbz     x2, 2f
1:
    ldrb    w3, [x1], #1
    strb    w3, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:
    mov     x0, x3
    ret

/* -----------------------------------------------------------------------------
 * Function: strcmp
 * Description: Compare two null-terminated strings
 * Input: x0 = string A, x1 = string B
 * Output: x0 = 0 if equal, non-zero if different
 * Clobbered: x0, x1, x2, x3
 * ----------------------------------------------------------------------------- */
.global strcmp
strcmp:
1:
    ldrb    w2, [x0], #1
    ldrb    w3, [x1], #1
    subs    x2, x2, x3
    b.ne    2f
    cbnz    w2, 1b
    mov     x0, #0
    ret
2:
    mov     x0, x2
    ret

/* -----------------------------------------------------------------------------
 * Function: strlen
 * Description: Get length of null-terminated string
 * Input: x0 = string pointer
 * Output: x0 = string length
 * Clobbered: x0, x1
 * ----------------------------------------------------------------------------- */
.global strlen
strlen:
    mov     x1, x0
1:
    ldrb    w2, [x1], #1
    cbnz    w2, 1b
    sub     x0, x1, x0
    sub     x0, x0, #1
    ret

/* -----------------------------------------------------------------------------
 * Function: print_hex
 * Description: Print a 64-bit value in hexadecimal (0xNNNNNNNNNNNNNNNN)
 * Input: x0 = value to print
 * Output: None
 * Clobbered: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global print_hex
print_hex:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    /* Print "0x" prefix */
    adrp    x0, hex_prefix
    add     x0, x0, :lo12:hex_prefix
    bl      serial_puts

    mov     x1, #16             /* 16 nibbles */
1:
    sub     x1, x1, #1
    lsl     x0, x0, #4          /* Top 4 bits */
    lsr     x0, x0, #60         /* Move to bottom */
    cmp     x0, #9
    b.gt    2f
    add     x0, x0, #'0'
    b       3f
2:
    add     x0, x0, #('a' - 10)
3:
    bl      serial_putc
    cbnz    x1, 1b

    ldp     x29, x30, [sp], #16
    ret

.section .rodata
.align 4
hex_prefix:
    .asciz "0x"
