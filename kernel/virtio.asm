/*
 * aiasm-aarch64/kernel/virtio.asm
 * VirtIO-MMIO transport framework for QEMU virt
 * Spec: VirtIO v1.2 (MMIO transport)
 *
 * QEMU virt device layout:
 *   0x0A000000: virtio-net
 *   0x0A000200: virtio-blk
 *   0x0A000400: virtio-gpu (unused)
 *   0x0A000600: virtio-input (unused)
 *   0x0A000800: virtio-rng
 */
.arch armv8-a

/* VirtIO-MMIO register offsets (from device base) */
.set VIRTIO_MMIO_MAGIC_VALUE,     0x000
.set VIRTIO_MMIO_VERSION,         0x004
.set VIRTIO_MMIO_DEVICE_ID,       0x008
.set VIRTIO_MMIO_VENDOR_ID,       0x00C
.set VIRTIO_MMIO_DEVICE_FEATURES, 0x010
.set VIRTIO_MMIO_DRIVER_FEATURES, 0x020
.set VIRTIO_MMIO_GUEST_PAGE_SIZE, 0x028
.set VIRTIO_MMIO_QUEUE_SEL,       0x030
.set VIRTIO_MMIO_QUEUE_NUM_MAX,   0x034
.set VIRTIO_MMIO_QUEUE_NUM,       0x038
.set VIRTIO_MMIO_QUEUE_ALIGN,     0x03C
.set VIRTIO_MMIO_QUEUE_PFN,       0x040  /* legacy v1 */
.set VIRTIO_MMIO_QUEUE_READY,     0x044  /* v2 only */
.set VIRTIO_MMIO_QUEUE_NOTIFY,    0x050
.set VIRTIO_MMIO_INTERRUPT_STATUS,0x060
.set VIRTIO_MMIO_INTERRUPT_ACK,   0x064
.set VIRTIO_MMIO_STATUS,          0x070
.set VIRTIO_MMIO_QUEUE_DESC_LOW,  0x080
.set VIRTIO_MMIO_QUEUE_DESC_HIGH, 0x084
.set VIRTIO_MMIO_QUEUE_AVAIL_LOW, 0x090
.set VIRTIO_MMIO_QUEUE_AVAIL_HIGH,0x094
.set VIRTIO_MMIO_QUEUE_USED_LOW,  0x0A0
.set VIRTIO_MMIO_QUEUE_USED_HIGH, 0x0A4
.set VIRTIO_MMIO_CONFIG_GENERATION,0x0FC
.set VIRTIO_MMIO_CONFIG,          0x100

/* VirtIO device types */
.set VIRTIO_DEV_NET,   1
.set VIRTIO_DEV_BLOCK, 2
.set VIRTIO_DEV_RNG,   4

/* QEMU virt VirtIO device bases */
.set VIRTIO_DEV0_BASE, 0x0A000000   /* net */
.set VIRTIO_DEV1_BASE, 0x0A000200   /* blk */
.set VIRTIO_DEV2_BASE, 0x0A000400
.set VIRTIO_DEV3_BASE, 0x0A000600
.set VIRTIO_DEV4_BASE, 0x0A000800   /* rng */

/* Virtqueue descriptor flags */
.set VRING_DESC_F_NEXT,     1
.set VRING_DESC_F_WRITE,    2
.set VRING_DESC_F_INDIRECT, 4

/* VirtIO status bits */
.set VIRTIO_STATUS_ACK,     1
.set VIRTIO_STATUS_DRIVER,  2
.set VIRTIO_STATUS_DRIVER_OK, 4
.set VIRTIO_STATUS_FEAT_OK, 8

/* Virtqueue sizes */
.set VIRTIO_MAX_DESC,       256     /* max descriptors per queue */
.set VIRTIO_DESC_SIZE,      16      /* each descriptor: 16 bytes */
.set VIRTIO_AVAIL_SIZE,     6       /* avail ring header */
.set VIRTIO_AVAIL_ELEM,     2       /* each avail element: 2 bytes */
.set VIRTIO_USED_SIZE,      6       /* used ring header */
.set VIRTIO_USED_ELEM,      8       /* each used element: 8 bytes */

/* Maximum VirtIO devices we scan */
.set VIRTIO_MAX_DEVICES,    5

.text

/* -----------------------------------------------------------------------------
 * Function: virtio_read32
 * Description: Read 32-bit register from VirtIO device
 * Input: x0 = device base, x1 = register offset
 * Output: w0 = value
 * ----------------------------------------------------------------------------- */
.global virtio_read32
virtio_read32:
    add     x0, x0, x1
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_write32
 * Description: Write 32-bit register to VirtIO device
 * Input: x0 = device base, x1 = register offset, w2 = value
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_write32
virtio_write32:
    add     x0, x0, x1
    str     w2, [x0]
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_init
 * Description: Scan all VirtIO devices, initialize found ones
 * Input: none
 * Output: w0 = number of devices found
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global virtio_init
virtio_init:
    stp     x29, x30, [sp, #-16]!

    /* Initialize device base table in BSS */
    adrp    x8, virtio_device_bases
    add     x8, x8, #:lo12:virtio_device_bases
    ldr     x9, =VIRTIO_DEV0_BASE
    str     x9, [x8, #0]
    ldr     x9, =VIRTIO_DEV1_BASE
    str     x9, [x8, #8]
    ldr     x9, =VIRTIO_DEV2_BASE
    str     x9, [x8, #16]
    ldr     x9, =VIRTIO_DEV3_BASE
    str     x9, [x8, #24]
    ldr     x9, =VIRTIO_DEV4_BASE
    str     x9, [x8, #32]

    mov     x4, #0              /* device count */
    adrp    x5, virtio_device_bases
    add     x5, x5, #:lo12:virtio_device_bases
    adrp    x6, virtio_device_ids
    add     x6, x6, #:lo12:virtio_device_ids
    mov     x7, #0              /* index */

virtio_scan_loop:
    cmp     x7, #VIRTIO_MAX_DEVICES
    b.ge    virtio_scan_done

    /* Get device base from table */
    ldr     x0, [x5, x7, lsl #3]
    cbz     x0, virtio_next

    /* Check magic value (must be 0x74726976 = "virt") */
    mov     x1, #VIRTIO_MMIO_MAGIC_VALUE
    bl      virtio_read32
    ldr     x8, =0x74726976
    cmp     w0, w8
    b.ne    virtio_next

    /* Check version (must be 2 for v2) */
    mov     x1, #VIRTIO_MMIO_VERSION
    bl      virtio_read32
    cmp     w0, #2
    b.ne    virtio_next

    /* Read device ID */
    mov     x1, #VIRTIO_MMIO_DEVICE_ID
    bl      virtio_read32
    cbz     w0, virtio_next      /* device ID 0 = no device */

    /* Store device ID */
    str     w0, [x6, x7, lsl #2]
    str     x0, [x5, x7, lsl #3]  /* store base address */

    add     x4, x4, #1

    /* Print detected device */
    adrp    x8, msg_virtio_found
    add     x8, x8, #:lo12:msg_virtio_found
    mov     x0, x8
    bl      serial_puts

virtio_next:
    add     x7, x7, #1
    b       virtio_scan_loop

virtio_scan_done:
    /* Store device count */
    adrp    x0, virtio_device_count
    add     x0, x0, #:lo12:virtio_device_count
    str     w4, [x0]

    mov     w0, w4              /* return count */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_reset_device
 * Description: Reset a VirtIO device (status = 0)
 * Input: x0 = device base
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_reset_device
virtio_reset_device:
    mov     w1, #0
    mov     x2, #VIRTIO_MMIO_STATUS
    b       virtio_write32

/* -----------------------------------------------------------------------------
 * Function: virtio_negotiate_features
 * Description: ACK + DRIVER status, read/driver features, set FEAT_OK
 * Input: x0 = device base
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_negotiate_features
virtio_negotiate_features:
    stp     x29, x30, [sp, #-16]!

    /* Status = ACK | DRIVER */
    mov     w2, #(VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER)
    mov     x1, #VIRTIO_MMIO_STATUS
    bl      virtio_write32

    /* For now, accept all device features (read device, write driver) */
    mov     x1, #VIRTIO_MMIO_DEVICE_FEATURES
    bl      virtio_read32
    mov     w2, w0
    mov     x1, #VIRTIO_MMIO_DRIVER_FEATURES
    bl      virtio_write32

    /* Status |= FEAT_OK */
    mov     w2, #(VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_FEAT_OK)
    mov     x1, #VIRTIO_MMIO_STATUS
    bl      virtio_write32

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_queue_setup
 * Description: Setup a virtqueue for a device
 * Input: x0 = device base, w1 = queue index, x2 = desc ring PA, x3 = avail PA
 *        x4 = used PA, w5 = queue size
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_queue_setup
virtio_queue_setup:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x2              /* save desc PA */
    mov     x9, x3              /* save avail PA */
    mov     x10, x4             /* save used PA */

    /* Select queue */
    mov     w2, w1
    mov     x1, #VIRTIO_MMIO_QUEUE_SEL
    bl      virtio_write32

    /* Set queue size */
    mov     w2, w5
    mov     x1, #VIRTIO_MMIO_QUEUE_NUM
    bl      virtio_write32

    /* Set queue alignment to 4096 */
    mov     w2, #4096
    mov     x1, #VIRTIO_MMIO_QUEUE_ALIGN
    bl      virtio_write32

    /* Set descriptor table address (split into low/high for v2) */
    mov     x2, x8              /* desc PA (identity mapped, VA=PA) */
    mov     w1, w2              /* low 32 bits — just truncate */
    mov     x3, #VIRTIO_MMIO_QUEUE_DESC_LOW
    bl      virtio_write32

    mov     x2, x8
    lsr     x1, x2, #32
    mov     x3, #VIRTIO_MMIO_QUEUE_DESC_HIGH
    bl      virtio_write32

    /* Set available ring address */
    mov     x2, x9
    mov     w1, w2
    mov     x3, #VIRTIO_MMIO_QUEUE_AVAIL_LOW
    bl      virtio_write32

    mov     x2, x9
    lsr     x1, x2, #32
    mov     x3, #VIRTIO_MMIO_QUEUE_AVAIL_HIGH
    bl      virtio_write32

    /* Set used ring address */
    mov     x2, x10
    mov     w1, w2
    mov     x3, #VIRTIO_MMIO_QUEUE_USED_LOW
    bl      virtio_write32

    mov     x2, x10
    lsr     x1, x2, #32
    mov     x3, #VIRTIO_MMIO_QUEUE_USED_HIGH
    bl      virtio_write32

    /* Mark queue ready */
    mov     w2, #1
    mov     x1, #VIRTIO_MMIO_QUEUE_READY
    bl      virtio_write32

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_queue_notify
 * Description: Notify device that buffers are available
 * Input: x0 = device base, w1 = queue index
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_queue_notify
virtio_queue_notify:
    mov     x1, #VIRTIO_MMIO_QUEUE_NOTIFY
    b       virtio_write32

/* -----------------------------------------------------------------------------
 * Function: virtio_get_device_id
 * Description: Get device ID for a given VirtIO device
 * Input: x0 = device base
 * Output: w0 = device ID
 * ----------------------------------------------------------------------------- */
.global virtio_get_device_id
virtio_get_device_id:
    mov     x1, #VIRTIO_MMIO_DEVICE_ID
    b       virtio_read32

/* -----------------------------------------------------------------------------
 * Strings and data
 * ----------------------------------------------------------------------------- */
.section .rodata
.align 4
msg_virtio_found:
    .asciz "[virtio] device detected\n"

.bss
.align 4
.global virtio_device_bases
virtio_device_bases:
    .skip 5 * 8              /* 5 device bases, filled at build time */

.global virtio_device_ids
virtio_device_ids:
    .skip VIRTIO_MAX_DEVICES * 4

.global virtio_device_count
virtio_device_count:
    .skip 4
