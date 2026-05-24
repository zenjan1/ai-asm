/*
 * aiasm-aarch64/kernel/device.asm
 * Device management kernel layer for v6.0
 * Device table storage + initialization for VirtIO devices.
 *
 * Device entry layout (32 bytes each, 16 entries):
 *   offset 0-3:   device_id (uint32)
 *   offset 4-7:   device_type (uint32: 0=blk, 1=net, 2=gpu, 3=input)
 *   offset 8-11:  status (uint32: 0=disconnected, 1=connected, 2=active)
 *   offset 12-15: virtio_base_addr (uint32)
 *   offset 16-19: irq_number (uint32)
 *   offset 20-23: flags (uint32)
 *   offset 24-31: reserved
 */
.arch armv8-a

.data
.align 4
.global device_table
device_table:
    .skip 512                     /* 16 devices x 32 bytes */

.global device_count
device_count:
    .word 3                       /* 3 VirtIO devices at boot: blk, net, gpu */

/* ---------------------------------------------------------------------------
 * device_init: Initialize VirtIO device table
 * --------------------------------------------------------------------------- */
.text
.global device_init
device_init:
    stp     x29, x30, [sp, #-16]!

    ldr x0, =device_table         /* x0 = device_table base */

    /* Device 0: VirtIO-Block */
    mov w1, #0                    /* device_id = 0 */
    str w1, [x0]
    mov w1, #0                    /* type = BLK */
    str w1, [x0, #4]
    mov w1, #2                    /* status = active */
    str w1, [x0, #8]
    ldr w1, =0x0a000000          /* virtio base */
    str w1, [x0, #12]
    mov w1, #49                   /* IRQ 49 */
    str w1, [x0, #16]

    /* Device 1: VirtIO-Net */
    add x1, x0, #32               /* next entry */
    mov w2, #1                    /* device_id = 1 */
    str w2, [x1]
    mov w2, #1                    /* type = NET */
    str w2, [x1, #4]
    mov w2, #2                    /* status = active */
    str w2, [x1, #8]
    ldr w2, =0x0a001000          /* virtio base */
    str w2, [x1, #12]
    mov w2, #48                   /* IRQ 48 */
    str w2, [x1, #16]

    /* Device 2: VirtIO-GPU */
    add x1, x0, #64               /* next entry */
    mov w2, #2                    /* device_id = 2 */
    str w2, [x1]
    mov w2, #2                    /* type = GPU */
    str w2, [x1, #4]
    mov w2, #2                    /* status = active */
    str w2, [x1, #8]
    ldr w2, =0x0a002000          /* virtio base */
    str w2, [x1, #12]
    mov w2, #50                   /* IRQ 50 */
    str w2, [x1, #16]

    /* Set device_count = 3 */
    ldr x1, =device_count
    mov w2, #3
    str w2, [x1]

    ldp     x29, x30, [sp], #16
    ret
