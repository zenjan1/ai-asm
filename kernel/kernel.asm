/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point, EL1 initialization, boot sequence
 * 100% pure AArch64 assembly, AI-ASM format
 */
.arch armv8-a

.section .text.boot

/* -----------------------------------------------------------------------------
 * Function: _start
 * Description: Kernel entry point, EL1 initialization from EL2/EL3
 * Input: x0 = device tree address (from bootloader)
 * Output: Never returns
 * Clobbered registers: All general-purpose registers
 * Stack: 0 bytes (sets up stack pointer)
 * ----------------------------------------------------------------------------- */
.global _start
_start:
    /* Disable all exceptions early */
    msr     daifset, #0xf         /* Mask all exceptions */

    /* Check current exception level and drop to EL1 if needed */
    mrs     x0, CurrentEL
    cmp     x0, #0xc            /* EL3? */
    b.eq    _setup_el3_to_el1
    cmp     x0, #0x8            /* EL2? */
    b.eq    _setup_el2_to_el1
    /* Already EL1, continue */

_setup_el1:
    /* Set up stack pointer for EL1 */
    ldr     x0, =__stack_top
    mov     sp, x0

    /* Disable MMU, caches, alignment check */
    mrs     x0, sctlr_el1
    bic     x0, x0, #(1 << 12)  /* Disable I-cache */
    bic     x0, x0, #(1 << 2)   /* Disable D-cache */
    bic     x0, x0, #(1 << 0)   /* Disable MMU */
    msr     sctlr_el1, x0

    /* Clear BSS */
    bl      bss_clear

    /* Initialize GICv2 interrupt controller */
    bl      gic_init

    /* Initialize PL011 UART */
    bl      serial_init

    /* Initialize timer */
    bl      timer_init

    /* Initialize event bus */
    bl      event_init

    /* Initialize logging system */
    bl      log_init

    /* Initialize VGA/framebuffer */
    bl      vga_init

    /* Publish boot event */
    adrp    x0, msg_boot
    add     x0, x0, :lo12:msg_boot
    mov     w1, #1              /* INFO level */
    bl      log_boot_message

    /* Jump to shell */
    bl      shell_start

    /* Should never reach here */
    b       hang_loop

/* -----------------------------------------------------------------------------
 * Function: _setup_el3_to_el1
 * Description: Drop from EL3 to EL1
 * Input: None (x0 already read from CurrentEL)
 * Output: Jumps to EL1 code
 * Clobbered registers: x0-x3
 * ----------------------------------------------------------------------------- */
_setup_el3_to_el1:
    /* Set EL1 execution state to AArch64 */
    mov     x0, #(0x5d1)
    msr     scr_el3, x0

    /* Set return address to EL1 code */
    adr     x0, _setup_el1
    msr     elr_el3, x0

    /* Set SPSR for EL1: all exceptions masked */
    mov     x0, #(0x3c5)
    msr     spsr_el3, x0

    /* Set stack for EL1 */
    ldr     x0, =__stack_top
    msr     sp_el1, x0

    /* Return to EL1 */
    eret

/* -----------------------------------------------------------------------------
 * Function: _setup_el2_to_el1
 * Description: Drop from EL2 to EL1
 * Input: None (x0 already read from CurrentEL)
 * Output: Jumps to EL1 code
 * Clobbered registers: x0-x3
 * ----------------------------------------------------------------------------- */
_setup_el2_to_el1:
    /* Set return address to EL1 code */
    adr     x0, _setup_el1
    msr     elr_el2, x0

    /* Configure HCR_EL2 for EL1 AArch64 */
    mrs     x0, hcr_el2
    orr     x0, x0, #(1 << 31)  /* RW bit: EL1 is AArch64 */
    msr     hcr_el2, x0

    /* Set SPSR for EL1 */
    mov     x0, #(0x3c5)
    msr     spsr_el2, x0

    /* Return to EL1 */
    eret

/* -----------------------------------------------------------------------------
 * Function: bss_clear
 * Description: Zero the BSS section
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1, x2
 * ----------------------------------------------------------------------------- */
bss_clear:
    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
    mov     x2, #0
1:
    cmp     x0, x1
    b.ge    2f
    str     xzr, [x0], #8
    b       1b
2:
    ret

/* -----------------------------------------------------------------------------
 * Function: hang_loop
 * Description: Infinite loop (fatal error handler)
 * Input: None
 * Output: Never returns
 * ----------------------------------------------------------------------------- */
hang_loop:
    wfi
    b       hang_loop

/* Boot message strings */
.section .rodata
msg_boot:
    .asciz "AI-ASM AArch64 v0.1 booting..."
