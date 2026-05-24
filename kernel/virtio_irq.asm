/*
 * aiasm-aarch64/kernel/virtio_irq.asm
 * VirtIO interrupt-driven I/O (v4.0)
 *
 * QEMU virt machine maps VirtIO devices to SPI IRQs:
 *   IRQ 48: virtio-net  (device at 0x0A000000)
 *   IRQ 49: virtio-blk  (device at 0x0A000200)
 *   IRQ 50: virtio-gpu  (device at 0x0A000400)
 *
 * When a VirtIO device completes a request, it raises an IRQ.
 * The handler checks the used ring and wakes sleeping processes.
 *
 * API:
 *   virtio_irq_init()             — enable VirtIO IRQs in GIC
 *   virtio_irq_handler()          — top-level IRQ dispatcher
 *   virtio_irq_map(device, irq)   — map device base to IRQ number
 */
.arch armv8-a

.set GICD_BASE, 0x08000000
.set GICC_BASE, 0x08010000

/* VirtIO IRQ assignments (SPI = 32+N, so IRQ 48 = SPI 16) */
.set VIRTIO_IRQ_NET,  48
.set VIRTIO_IRQ_BLK,  49
.set VIRTIO_IRQ_GPU,  50

/* VirtIO MMIO interrupt status register offset */
.set VIRTIO_MMIO_INTERRUPT_STATUS, 0x060
.set VIRTIO_MMIO_INTERRUPT_ACK,    0x064

/* VirtIO interrupt bits */
.set VIRTIO_INT_QUEUE, 1          /* queue interrupt */

/* Device bases */
.set VIRTIO_NET_BASE, 0x0A000000
.set VIRTIO_BLK_BASE, 0x0A000200

.text

/* -----------------------------------------------------------------------------
 * Function: virtio_irq_init
 * Description: Enable VirtIO IRQs in GIC distributor
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_irq_init
virtio_irq_init:
    stp     x29, x30, [sp, #-16]!

    /* GICD base = 0x08000000 */
    movz    x0, #0x0800, lsl #16
    movk    x0, #0x0000

    /* Enable IRQ 48 (net) in GICD_ISENABLER[1] (IRQs 32-63, word offset 0x0104) */
    mov     x1, #(1 << (VIRTIO_IRQ_NET - 32))  /* bit 16 */
    str     w1, [x0, #0x0104]

    /* Enable IRQ 49 (blk) — bit 17 in same word */
    mov     x1, #(1 << (VIRTIO_IRQ_BLK - 32))  /* bit 17 */
    str     w1, [x0, #0x0104]

    /* Enable IRQ 50 (gpu) — bit 18 in same word */
    mov     x1, #(1 << (VIRTIO_IRQ_GPU - 32))  /* bit 18 */
    str     w1, [x0, #0x0104]

    /* Set priority for VirtIO IRQs to 0x80 (higher than timer's 0xA0) */
    /* IPRIORITYR starts at 0x0400, each byte = one IRQ */
    /* IRQ 48 → offset 0x0400 + 48 = 0x0430 */
    mov     x1, #0x0400
    add     x1, x1, #VIRTIO_IRQ_NET
    mov     w2, #0x80
    strb    w2, [x0, x1]

    add     x1, x1, #1          /* IRQ 49 */
    strb    w2, [x0, x1]

    add     x1, x1, #1          /* IRQ 50 */
    strb    w2, [x0, x1]

    /* Set CPU target list for VirtIO IRQs (route to CPU 0) */
    /* GICD_ITARGETSR starts at 0x0800, each byte = one IRQ */
    add     x1, x0, #0x0800
    add     x1, x1, #VIRTIO_IRQ_NET
    mov     w2, #0x01           /* CPU 0 */
    strb    w2, [x0, x1]
    add     x1, x1, #1
    strb    w2, [x0, x1]
    add     x1, x1, #1
    strb    w2, [x0, x1]

    /* Print status */
    adrp    x0, msg_virtio_irq
    add     x0, x0, #:lo12:msg_virtio_irq
    bl      serial_puts

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_irq_handler
 * Description: Handle VirtIO interrupts — check used ring, wake processes
 * Input: none (called from exception handler)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_irq_handler
virtio_irq_handler:
    stp     x29, x30, [sp, #-16]!

    /* Read IAR to get IRQ number */
    movz    x0, #0x0801, lsl #16
    movk    x0, #0x0000
    ldr     w1, [x0]            /* GICC_IAR */
    and     w2, w1, #0x3FF      /* IRQ ID */

    /* Dispatch based on IRQ number */
    cmp     w2, #VIRTIO_IRQ_NET
    b.eq    virtio_net_irq
    cmp     w2, #VIRTIO_IRQ_BLK
    b.eq    virtio_blk_irq
    cmp     w2, #VIRTIO_IRQ_GPU
    b.eq    virtio_gpu_irq

    /* Unknown VirtIO IRQ — just EOI */
    b       virtio_irq_eoi

virtio_net_irq:
    /* Acknowledge interrupt on device */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w1, #VIRTIO_INT_QUEUE
    str     w1, [x0, #VIRTIO_MMIO_INTERRUPT_ACK]

    /* Check RX used ring for completed packets */
    /* Wake any process sleeping on net recv */
    bl      virtio_net_wake_recv

    /* Check TX used ring for completed sends */
    bl      virtio_net_wake_send

    b       virtio_irq_eoi

virtio_blk_irq:
    /* Acknowledge interrupt on device */
    ldr     x0, =VIRTIO_BLK_BASE
    mov     w1, #VIRTIO_INT_QUEUE
    str     w1, [x0, #VIRTIO_MMIO_INTERRUPT_ACK]

    /* Wake process sleeping on blk I/O */
    bl      virtio_blk_wake_io

    b       virtio_irq_eoi

virtio_gpu_irq:
    /* Acknowledge GPU interrupt */
    ldr     x0, =0x0A000400
    mov     w1, #VIRTIO_INT_QUEUE
    str     w1, [x0, #VIRTIO_MMIO_INTERRUPT_ACK]

    /* GPU interrupt — future: wake framebuffer waiters */

virtio_irq_eoi:
    /* Write EOIR to end interrupt */
    movz    x0, #0x0801, lsl #16
    movk    x0, #0x0000
    str     w1, [x0, #0x10]     /* GICC_EOIR */

    /* Signal scheduler to reschedule (may need to wake a process) */
    adrp    x0, preempt_pending
    add     x0, x0, #:lo12:preempt_pending
    mov     w1, #1
    strb    w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* =============================================================================
 * Internal: VirtIO-Net wake helpers
 * ============================================================================= */

/* -----------------------------------------------------------------------------
 * virtio_net_wake_recv — wake process waiting for RX data
 * ----------------------------------------------------------------------------- */
virtio_net_wake_recv:
    stp     x29, x30, [sp, #-16]!

    /* Check if RX used ring has new entries */
    adrp    x0, virtio_net_rx_used
    add     x0, x0, #:lo12:virtio_net_rx_used
    adrp    x1, virtio_net_rx_used_idx
    add     x1, x1, #:lo12:virtio_net_rx_used_idx
    ldr     w2, [x1]
    ldrh    w3, [x0, #4]        /* used ring idx */
    cmp     w3, w2
    b.eq    _net_rx_nothing     /* no new data */

    /* Wake the process sleeping on recv (PID stored in net_sleeping_recv) */
    adrp    x0, net_sleeping_recv
    add     x0, x0, #:lo12:net_sleeping_recv
    ldr     w0, [x0]
    cbz     w0, _net_rx_nothing
    bl      proc_wake
    /* Clear sleeping marker */
    str     wzr, [x0]

_net_rx_nothing:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * virtio_net_wake_send — wake process waiting for TX completion
 * ----------------------------------------------------------------------------- */
virtio_net_wake_send:
    stp     x29, x30, [sp, #-16]!

    /* Check TX used ring */
    adrp    x0, virtio_net_tx_used
    add     x0, x0, #:lo12:virtio_net_tx_used
    adrp    x1, virtio_net_tx_used_idx
    add     x1, x1, #:lo12:virtio_net_tx_used_idx
    ldr     w2, [x1]
    ldrh    w3, [x0, #4]
    cmp     w3, w2
    b.eq    _net_tx_nothing

    /* Wake sleeping TX process */
    adrp    x0, net_sleeping_send
    add     x0, x0, #:lo12:net_sleeping_send
    ldr     w0, [x0]
    cbz     w0, _net_tx_nothing
    bl      proc_wake
    str     wzr, [x0]

_net_tx_nothing:
    ldp     x29, x30, [sp], #16
    ret

/* =============================================================================
 * Internal: VirtIO-Block wake helper
 * ============================================================================= */

/* -----------------------------------------------------------------------------
 * virtio_blk_wake_io — wake process waiting for block I/O completion
 * ----------------------------------------------------------------------------- */
virtio_blk_wake_io:
    stp     x29, x30, [sp, #-16]!

    /* Check used ring */
    adrp    x0, virtio_blk_used
    add     x0, x0, #:lo12:virtio_blk_used
    adrp    x1, virtio_blk_used_idx
    add     x1, x1, #:lo12:virtio_blk_used_idx
    ldr     w2, [x1]
    ldrh    w3, [x0, #4]
    cmp     w3, w2
    b.eq    _blk_nothing

    /* Wake sleeping block I/O process */
    adrp    x0, blk_sleeping_io
    add     x0, x0, #:lo12:blk_sleeping_io
    ldr     w0, [x0]
    cbz     w0, _blk_nothing
    bl      proc_wake
    str     wzr, [x0]

_blk_nothing:
    ldp     x29, x30, [sp], #16
    ret

/* =============================================================================
 * Data
 * ============================================================================= */
.bss
.align 4
.global net_sleeping_recv
net_sleeping_recv:
    .skip 4                     /* PID of process waiting for RX */

.global net_sleeping_send
net_sleeping_send:
    .skip 4                     /* PID of process waiting for TX */

.global blk_sleeping_io
blk_sleeping_io:
    .skip 4                     /* PID of process waiting for blk I/O */

.section .rodata
.align 4
msg_virtio_irq:
    .asciz "[virtio-irq] interrupts enabled (net=48, blk=49, gpu=50)\n"
