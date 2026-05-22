/*
 * aiasm-aarch64/kernel/gic.asm
 * GICv2 interrupt controller driver for QEMU AArch64 virt
 * Distributor: 0x08000000, CPU Interface: 0x08010000
 */
.arch armv8-a

.set GICD_BASE, 0x08000000
.set GICC_BASE, 0x08010000

.text

/* -----------------------------------------------------------------------------
 * Function: gic_init
 * Description: Initialize GICv2 distributor and CPU interface
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_init
gic_init:
    stp     x29, x30, [sp, #-16]!

    /* --- Disable Distributor --- */
    movz    x0, #GICD_BASE >> 16
    movk    x0, #GICD_BASE & 0xFFFF
    mov     w1, #0
    str     w1, [x0]            /* GICD_CTLR = 0 */

    /* Disable all interrupts in distributor */
    mov     x1, #1
    mov     x2, #256            /* 256 words of ICENABLER */
1:
    cbz     x2, 2f
    str     wzr, [x0, x1]
    add     x1, x1, #4
    sub     x2, x2, #1
    b       1b
2:
    /* Set all interrupts to Group 0 */
    mov     x1, #1
    mov     x2, #256
3:
    cbz     x2, 4f
    str     wzr, [x0, x1]
    add     x1, x1, #4
    sub     x2, x2, #1
    b       3b
4:
    /* Set priority for all interrupts */
    mov     x1, #0
    mov     x2, #1024           /* 1024 bytes of IPRIORITYR */
5:
    cbz     x2, 6f
    mov     w3, #0xA0
    strb    w3, [x0, x1]
    add     x1, x1, #1
    sub     x2, x2, #1
    b       5b
6:
    /* Enable Distributor */
    mov     w1, #1
    str     w1, [x0]            /* GICD_CTLR = 1 */

    /* --- Configure CPU Interface --- */
    movz    x0, #GICC_BASE >> 16
    movk    x0, #GICC_BASE & 0xFFFF

    /* Set priority mask: allow all */
    mov     w1, #0xFF
    strb    w1, [x0, #4]        /* GICC_PMR */

    /* Enable CPU interface */
    mov     w1, #1
    str     w1, [x0]            /* GICC_CTLR = 1 */

    ldp     x29, x30, [sp], #16
    ret
