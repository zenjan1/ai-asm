/*
 * aiasm-aarch64/kernel/virtio_blk.asm
 * VirtIO-Block device driver (device_id=2)
 * VirtIO-MMIO transport, v2 spec
 *
 * Block request format:
 *   desc[0]: header (type, ioprio, sector) — 16 bytes
 *   desc[1]: data buffer (read/write target) — variable
 *   desc[2]: status byte (device writes back 0=OK)
 */
.arch armv8-a

/* VirtIO-MMIO register offsets (same as virtio.asm — local copy) */
.set VIRTIO_MMIO_MAGIC_VALUE,     0x000
.set VIRTIO_MMIO_VERSION,         0x004
.set VIRTIO_MMIO_DEVICE_ID,       0x008
.set VIRTIO_MMIO_VENDOR_ID,       0x00C
.set VIRTIO_MMIO_DEVICE_FEATURES, 0x010
.set VIRTIO_MMIO_DRIVER_FEATURES, 0x020
.set VIRTIO_MMIO_QUEUE_SEL,       0x030
.set VIRTIO_MMIO_QUEUE_NUM,       0x038
.set VIRTIO_MMIO_QUEUE_ALIGN,     0x03C
.set VIRTIO_MMIO_QUEUE_DESC_LOW,  0x080
.set VIRTIO_MMIO_QUEUE_DESC_HIGH, 0x084
.set VIRTIO_MMIO_QUEUE_AVAIL_LOW, 0x090
.set VIRTIO_MMIO_QUEUE_AVAIL_HIGH,0x094
.set VIRTIO_MMIO_QUEUE_USED_LOW,  0x0A0
.set VIRTIO_MMIO_QUEUE_USED_HIGH, 0x0A4
.set VIRTIO_MMIO_QUEUE_READY,     0x044
.set VIRTIO_MMIO_QUEUE_NOTIFY,    0x050
.set VIRTIO_MMIO_STATUS,          0x070

/* VirtIO device types */
.set VIRTIO_DEV_BLOCK, 2

/* VirtIO status bits */
.set VIRTIO_STATUS_ACK,     1
.set VIRTIO_STATUS_DRIVER,  2
.set VIRTIO_STATUS_FEAT_OK, 8
.set VIRTIO_STATUS_DRIVER_OK, 4

/* Virtqueue descriptor flags */
.set VRING_DESC_F_NEXT,     1
.set VRING_DESC_F_WRITE,    2

/* Device base for blk (QEMU virt: dev1 at 0x0A000200) */
.set VIRTIO_BLK_BASE,     0x0A000200

/* Queue size */
.set VIRTIO_BLK_QUEUE_SIZE, 64

/* VirtIO-Block request types */
.set VIRTIO_BLK_T_IN,      0     /* read from device */
.set VIRTIO_BLK_T_OUT,     2     /* write to device */

/* VirtIO-Block status codes */
.set VIRTIO_BLK_S_OK,      0
.set VIRTIO_BLK_S_IOERR,   1
.set VIRTIO_BLK_S_UNSUPP,  2

.text

/* External functions */
.extern virtio_read32
.extern virtio_write32
.extern virtio_reset_device
.extern virtio_negotiate_features
.extern virtio_queue_setup
.extern virtio_queue_notify
.extern serial_puts
.extern proc_sleep
.extern proc_wake
.extern current_pid
.extern blk_sleeping_io

/* -----------------------------------------------------------------------------
 * BSS: VirtIO-Block global state
 * ----------------------------------------------------------------------------- */
.bss
.align 12                    /* 4KB alignment for rings */
.global virtio_blk_desc
virtio_blk_desc:
    .skip 4096               /* 256 × 16 = 4096 bytes */

.global virtio_blk_avail
virtio_blk_avail:
    .skip 4096               /* avail ring */

.global virtio_blk_used
virtio_blk_used:
    .skip 4096               /* used ring */

.global virtio_blk_status
virtio_blk_status:
    .skip 64                 /* status buffer */

.global virtio_blk_header
virtio_blk_header:
    .skip 16                 /* request header buffer */

.global virtio_blk_ready
virtio_blk_ready:
    .skip 1                  /* 1 if device is ready */

.bss
.align 4
.global virtio_blk_used_idx
virtio_blk_used_idx:
    .skip 4                  /* tracks used ring position */

.text

/* -----------------------------------------------------------------------------
 * Function: virtio_blk_init
 * Description: Initialize VirtIO-Block device
 * Input: none
 * Output: x0 = 0 if success, -1 if error
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global virtio_blk_init
virtio_blk_init:
    stp     x29, x30, [sp, #-16]!

    /* Check if device exists */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    mov     x1, #VIRTIO_MMIO_MAGIC_VALUE
    bl      virtio_read32
    ldr     x8, =0x74726976
    cmp     w0, w8
    b.ne    virtio_blk_error

    /* Check device ID */
    mov     x1, #VIRTIO_MMIO_DEVICE_ID
    bl      virtio_read32
    cmp     w0, #VIRTIO_DEV_BLOCK
    b.ne    virtio_blk_error

    /* Reset device */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    bl      virtio_reset_device

    /* Negotiate features */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    bl      virtio_negotiate_features

    /* Setup virtqueue 0 */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    mov     w1, #0             /* queue index */
    adrp    x2, virtio_blk_desc
    add     x2, x2, #:lo12:virtio_blk_desc
    adrp    x3, virtio_blk_avail
    add     x3, x3, #:lo12:virtio_blk_avail
    adrp    x4, virtio_blk_used
    add     x4, x4, #:lo12:virtio_blk_used
    mov     w5, #VIRTIO_BLK_QUEUE_SIZE
    bl      virtio_queue_setup

    /* Set DRIVER_OK status */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    mov     w2, #(VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_FEAT_OK | VIRTIO_STATUS_DRIVER_OK)
    mov     x1, #VIRTIO_MMIO_STATUS
    bl      virtio_write32

    /* Mark device ready */
    adrp    x0, virtio_blk_ready
    add     x0, x0, #:lo12:virtio_blk_ready
    mov     w1, #1
    strb    w1, [x0]

    /* Reset used index tracker */
    adrp    x0, virtio_blk_used_idx
    add     x0, x0, #:lo12:virtio_blk_used_idx
    str     wzr, [x0]

    /* Print success message */
    adrp    x0, msg_blk_ready
    add     x0, x0, #:lo12:msg_blk_ready
    bl      serial_puts

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

virtio_blk_error:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_blk_read
 * Description: Read sectors from block device
 * Input: x0 = sector number, x1 = buffer VA, w2 = sector count
 * Output: x0 = 0 on success, -1 on error
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global virtio_blk_read
virtio_blk_read:
    stp     x29, x30, [sp, #-16]!
    mov     x6, x0              /* save sector */
    mov     x7, x1              /* save buffer */
    mov     w8, w2              /* save count */

    /* Check device ready */
    adrp    x0, virtio_blk_ready
    add     x0, x0, #:lo12:virtio_blk_ready
    ldrb    w0, [x0]
    cbz     w0, blk_not_ready

    /* Build request header: type=IN (0), reserved, sector */
    adrp    x0, virtio_blk_header
    add     x0, x0, #:lo12:virtio_blk_header
    str     wzr, [x0]           /* type = VIRTIO_BLK_T_IN = 0 */
    str     wzr, [x0, #4]       /* reserved = 0 */
    str     x6, [x0, #8]        /* sector number */

    /* Setup descriptor chain (3 descriptors):
     * desc[0]: header (read-only, device reads)
     * desc[1]: data buffer (write-only, device writes data)
     * desc[2]: status (write-only, device writes status)
     */
    adrp    x0, virtio_blk_desc
    add     x0, x0, #:lo12:virtio_blk_desc

    /* Descriptor 0: header (16 bytes, read-only, next=1) */
    adrp    x1, virtio_blk_header
    add     x1, x1, #:lo12:virtio_blk_header
    str     x1, [x0, #0]        /* addr */
    mov     w1, #16
    str     w1, [x0, #8]        /* len */
    mov     w1, #(VRING_DESC_F_NEXT | 1)  /* flags: next, descriptor index 1 */
    str     w1, [x0, #12]       /* flags + next */

    /* Descriptor 1: data buffer (512*count bytes, write, next=2) */
    str     x7, [x0, #16]       /* addr */
    mov     w1, w8
    mov     w3, #512
    mul     w1, w1, w3
    str     w1, [x0, #24]       /* len */
    mov     w1, #(VRING_DESC_F_NEXT | VRING_DESC_F_WRITE | 2)
    str     w1, [x0, #28]       /* flags + next */

    /* Descriptor 2: status (1 byte, write, no next) */
    adrp    x1, virtio_blk_status
    add     x1, x1, #:lo12:virtio_blk_status
    str     x1, [x0, #32]       /* addr */
    mov     w1, #1
    str     w1, [x0, #40]       /* len */
    mov     w1, #VRING_DESC_F_WRITE
    str     w1, [x0, #44]       /* flags */

    /* Add to available ring: desc index 0 */
    adrp    x0, virtio_blk_avail
    add     x0, x0, #:lo12:virtio_blk_avail
    /* Read current avail idx (offset 4) */
    ldrh    w1, [x0, #4]
    /* Calculate ring entry offset: 6 + (idx % QUEUE_SIZE) * 2 */
    and     w2, w1, #(VIRTIO_BLK_QUEUE_SIZE - 1)
    mov     w3, #2
    mul     w2, w2, w3
    add     w2, w2, #6
    strh    wzr, [x0, x2]       /* ring entry = descriptor index 0 */
    /* Increment avail idx */
    add     w1, w1, #1
    strh    w1, [x0, #4]

    /* Notify device */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    mov     w1, #0              /* queue 0 */
    bl      virtio_queue_notify

    /* Register current PID as sleeping on blk I/O */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    ldr     w0, [x0]
    adrp    x1, blk_sleeping_io
    add     x1, x1, #:lo12:blk_sleeping_io
    str     w0, [x1]

    /* Set process to SLEEPING, yield to scheduler */
    mov     w0, #1              /* wake reason: blk io */
    bl      proc_sleep

    /* When woken by IRQ, check if completion actually happened */
    adrp    x0, virtio_blk_used
    add     x0, x0, #:lo12:virtio_blk_used
    adrp    x1, virtio_blk_used_idx
    add     x1, x1, #:lo12:virtio_blk_used_idx
    ldr     w2, [x1]

blk_wait:
    ldrh    w3, [x0, #4]        /* used ring idx */
    cmp     w3, w2
    b.ne    blk_done_wait       /* completion arrived */

    /* Spurious wake — sleep again */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    ldr     w0, [x0]
    adrp    x1, blk_sleeping_io
    add     x1, x1, #:lo12:blk_sleeping_io
    str     w0, [x1]
    mov     w0, #1
    bl      proc_sleep

    /* Re-check after wake */
    adrp    x0, virtio_blk_used
    add     x0, x0, #:lo12:virtio_blk_used
    adrp    x1, virtio_blk_used_idx
    add     x1, x1, #:lo12:virtio_blk_used_idx
    ldr     w2, [x1]
    b       blk_wait

blk_done_wait:

    /* Read status */
    ldrh    w3, [x0, #6]        /* used id */
    ldr     w4, [x0, #8]        /* used len */
    adrp    x0, virtio_blk_status
    add     x0, x0, #:lo12:virtio_blk_status
    ldrb    w0, [x0]            /* status byte */

    /* Update used idx */
    add     w2, w2, #1
    str     w2, [x1]

    cmp     w0, #VIRTIO_BLK_S_OK
    b.ne    blk_error_path

    ldp     x29, x30, [sp], #16
    ret

blk_not_ready:
    mov     x0, #-2
    ldp     x29, x30, [sp], #16
    ret

blk_error_path:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_blk_write
 * Description: Write sectors to block device
 * Input: x0 = sector number, x1 = buffer VA, w2 = sector count
 * Output: x0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global virtio_blk_write
virtio_blk_write:
    stp     x29, x30, [sp, #-16]!
    mov     x6, x0              /* save sector */
    mov     x7, x1              /* save buffer */
    mov     w8, w2              /* save count */

    /* Check device ready */
    adrp    x0, virtio_blk_ready
    add     x0, x0, #:lo12:virtio_blk_ready
    ldrb    w0, [x0]
    cbz     w0, blk_write_not_ready

    /* Build request header: type=OUT (2), reserved, sector */
    adrp    x0, virtio_blk_header
    add     x0, x0, #:lo12:virtio_blk_header
    mov     w1, #VIRTIO_BLK_T_OUT
    str     w1, [x0]            /* type */
    str     wzr, [x0, #4]       /* reserved */
    str     x6, [x0, #8]        /* sector */

    /* Setup descriptor chain:
     * desc[0]: header (read-only, device reads)
     * desc[1]: data buffer (read-only, device reads data to write)
     * desc[2]: status (write-only, device writes status)
     */
    adrp    x0, virtio_blk_desc
    add     x0, x0, #:lo12:virtio_blk_desc

    /* Descriptor 0: header */
    adrp    x1, virtio_blk_header
    add     x1, x1, #:lo12:virtio_blk_header
    str     x1, [x0, #0]
    mov     w1, #16
    str     w1, [x0, #8]
    mov     w1, #(VRING_DESC_F_NEXT | 1)
    str     w1, [x0, #12]

    /* Descriptor 1: data (read-only for write, no WRITE flag) */
    str     x7, [x0, #16]
    mov     w1, w8
    mov     w3, #512
    mul     w1, w1, w3
    str     w1, [x0, #24]
    mov     w1, #(VRING_DESC_F_NEXT | 2)
    str     w1, [x0, #28]

    /* Descriptor 2: status (write) */
    adrp    x1, virtio_blk_status
    add     x1, x1, #:lo12:virtio_blk_status
    str     x1, [x0, #32]
    mov     w1, #1
    str     w1, [x0, #40]
    mov     w1, #VRING_DESC_F_WRITE
    str     w1, [x0, #44]

    /* Add to available ring */
    adrp    x0, virtio_blk_avail
    add     x0, x0, #:lo12:virtio_blk_avail
    ldrh    w1, [x0, #4]
    and     w2, w1, #(VIRTIO_BLK_QUEUE_SIZE - 1)
    mov     w3, #2
    mul     w2, w2, w3
    add     w2, w2, #6
    strh    wzr, [x0, x2]
    add     w1, w1, #1
    strh    w1, [x0, #4]

    /* Notify device */
    adrp    x0, VIRTIO_BLK_BASE
    ldr     x0, =VIRTIO_BLK_BASE
    mov     w1, #0
    bl      virtio_queue_notify

    /* Register current PID as sleeping on blk I/O */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    ldr     w0, [x0]
    adrp    x1, blk_sleeping_io
    add     x1, x1, #:lo12:blk_sleeping_io
    str     w0, [x1]

    /* Set process to SLEEPING, yield to scheduler */
    mov     w0, #1
    bl      proc_sleep

    /* When woken by IRQ, check completion */
    adrp    x0, virtio_blk_used
    add     x0, x0, #:lo12:virtio_blk_used
    adrp    x1, virtio_blk_used_idx
    add     x1, x1, #:lo12:virtio_blk_used_idx
    ldr     w2, [x1]

blk_write_wait:
    ldrh    w3, [x0, #4]
    cmp     w3, w2
    b.ne    blk_write_done_wait

    /* Spurious wake — sleep again */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    ldr     w0, [x0]
    adrp    x1, blk_sleeping_io
    add     x1, x1, #:lo12:blk_sleeping_io
    str     w0, [x1]
    mov     w0, #1
    bl      proc_sleep

    adrp    x0, virtio_blk_used
    add     x0, x0, #:lo12:virtio_blk_used
    adrp    x1, virtio_blk_used_idx
    add     x1, x1, #:lo12:virtio_blk_used_idx
    ldr     w2, [x1]
    b       blk_write_wait

blk_write_done_wait:

    /* Read status */
    adrp    x0, virtio_blk_status
    add     x0, x0, #:lo12:virtio_blk_status
    ldrb    w0, [x0]

    /* Update used idx */
    add     w2, w2, #1
    str     w2, [x1]

    cmp     w0, #VIRTIO_BLK_S_OK
    b.ne    blk_write_err

    ldp     x29, x30, [sp], #16
    ret

blk_write_not_ready:
    mov     x0, #-2
    ldp     x29, x30, [sp], #16
    ret

blk_write_err:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Strings
 * ----------------------------------------------------------------------------- */
.text

.section .rodata, "a", %progbits
.align 4
msg_blk_ready:
    .asciz "[virtio-blk] device ready\n"
