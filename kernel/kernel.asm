/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point and initialization sequence
 * Pure AArch64 assembly, load at 0x40080000
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: _start
 * Description: Kernel entry point. Sets up stack, clears BSS, initializes
 *              subsystems, and enters shell.
 * Input: x0 = device tree address (from bootloader, unused)
 * Output: never returns
 * Clobbered: all registers
 * Stack: 0 bytes (sets up stack first)
 * ----------------------------------------------------------------------------- */
.global _start
_start:
    /* Mask all exceptions */
    msr     daifset, #0xf

    /* Setup stack pointer (8KB, high address, 16-byte aligned) */
    adrp    x0, __stack_top
    add     x0, x0, #:lo12:__stack_top
    mov     sp, x0

    /* Clear BSS */
    bl      bss_clear

    /* Initialize PL011 UART */
    bl      serial_init

    /* Initialize logging */
    bl      log_init

    /* Initialize event bus */
    bl      event_init

    /* Publish boot event */
    mov     w0, #1              /* type ID 1 = boot */
    adrp    x1, boot_data
    add     x1, x1, #:lo12:boot_data
    mov     w2, #39
    bl      event_publish

    /* Enter shell */
    bl      shell_run

    /* Should never reach here */
    bl      shell_hang

/* -----------------------------------------------------------------------------
 * Function: bss_clear
 * Description: Zero the BSS section
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
bss_clear:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, __bss_start
    add     x0, x0, #:lo12:__bss_start
    adrp    x1, __bss_end
    add     x1, x1, #:lo12:__bss_end
1:
    cmp     x0, x1
    b.ge    2f
    str     xzr, [x0], #8
    b       1b
2:
    ldp     x29, x30, [sp], #16
    ret

/* Read-only boot data string */
.section .rodata
.align 4
boot_data:
    .asciz "{\"version\":\"0.1-aarch64\",\"arch\":\"aarch64\"}"
