/*
 * aiasm-aarch64/kernel/gic.asm
 * GICv2 minimal interrupt controller
 * Distributor: 0x08000000, CPU Interface: 0x08010000
 * v0.2: Enable only, no IRQ routing
 */
.arch armv8-a

.set GICD_BASE, 0x08000000
.set GICC_BASE, 0x08010000

.text

/* -----------------------------------------------------------------------------
 * Function: gic_init
 * Description: Initialize GICv2 distributor and CPU interface
 * Input: none
 * Output: none
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_init
gic_init:
    stp     x29, x30, [sp, #-16]!

    /* --- Disable Distributor --- */
    movz    x0, #0x0800, lsl #16
    movk    x0, #0x0000

    mov     w1, #0
    str     w1, [x0]            /* GICD_CTLR = 0 */

    /* Disable all interrupts (ICENABLERn) */
    mov     x1, #0
    mov     x2, #32             /* 32 words = 1024 IRQs */
1:
    cbz     x2, 2f
    str     wzr, [x0, x1]
    add     x1, x1, #4
    sub     x2, x2, #1
    b       1b

2:
    /* Set all to Group 0 (IGROUPRn) */
    mov     x1, #0
    mov     x2, #32
3:
    cbz     x2, 4f
    str     wzr, [x0, x1]
    add     x1, x1, #4
    sub     x2, x2, #1
    b       3b

4:
    /* Set priority (IPRIORITYR) - all to 0xA0 */
    mov     x1, #0
    mov     x2, #1024           /* 1024 bytes */
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

    /* --- CPU Interface --- */
    movz    x0, #0x0801, lsl #16
    movk    x0, #0x0000

    /* Priority mask: allow all (0xFF) */
    mov     w1, #0xFF
    strb    w1, [x0, #4]        /* GICC_PMR */

    /* Enable CPU interface, EOImode = 1 */
    mov     w1, #1
    str     w1, [x0]            /* GICC_CTLR = 1 */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gic_enable_timer_irq
 * Description: Enable virtual timer IRQ (IRQ 27) in GIC distributor
 * Input: none
 * Output: none
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_enable_timer_irq
gic_enable_timer_irq:
    stp     x29, x30, [sp, #-16]!

    /* GICD base address = 0x08000000 */
    movz    x0, #0x0800, lsl #16
    movk    x0, #0x0000

    /* Enable IRQ 27 in GICD_ISENABLER[0] (offset 0x0100) */
    movz    x1, #0x0800, lsl #16
    movk    x1, #0x0000
    str     w1, [x0, #0x0100]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gic_ack_irq
 * Description: Acknowledge and end an IRQ
 * Input: none (reads IAR from hardware)
 * Output: w0 = IRQ ID, or 1023 if spurious
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_ack_irq
gic_ack_irq:
    stp     x29, x30, [sp, #-16]!

    movz    x0, #0x0801, lsl #16
    movk    x0, #0x0000

    /* Read Interrupt Acknowledge Register */
    ldr     w1, [x0]            /* GICC_IAR */
    and     w0, w1, #0x3FF      /* IRQ ID */

    /* Write EOIR to end interrupt */
    str     w1, [x0, #0x10]     /* GICC_EOIR */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gic_mask_all
 * Description: Mask all CPU interrupts
 * Input: none
 * Output: none
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_mask_all
gic_mask_all:
    stp     x29, x30, [sp, #-16]!

    msr     daifset, #0x2       /* Mask IRQ */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: gic_unmask_all
 * Description: Unmask CPU interrupts
 * Input: none
 * Output: none
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global gic_unmask_all
gic_unmask_all:
    stp     x29, x30, [sp, #-16]!

    msr     daifclr, #0x2       /* Unmask IRQ */

    ldp     x29, x30, [sp], #16
    ret
