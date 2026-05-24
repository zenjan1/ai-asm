/*
 * aiasm-aarch64/kernel/virtio_net.asm
 * VirtIO-Net device driver (device_id=1)
 * VirtIO-MMIO transport, v2 spec
 *
 * VirtIO-Net header (10 bytes, padded to 12 for alignment):
 *   +0:  flags (u8)
 *   +1:  gso_type (u8)
 *   +2:  hdr_len (u16)
 *   +4:  gso_size (u16)
 *   +6:  csum_start (u16)
 *   +8:  csum_offset (u16)
 *
 * TX: header + packet data (2 descriptors)
 * RX: header + buffer (2 descriptors, device writes both)
 */
.arch armv8-a

/* VirtIO-MMIO register offsets */
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
.set VIRTIO_MMIO_CONFIG,          0x100

/* VirtIO device types */
.set VIRTIO_DEV_NET,    1

/* VirtIO status bits */
.set VIRTIO_STATUS_ACK,       1
.set VIRTIO_STATUS_DRIVER,    2
.set VIRTIO_STATUS_FEAT_OK,   8
.set VIRTIO_STATUS_DRIVER_OK, 4

/* Virtqueue descriptor flags */
.set VRING_DESC_F_NEXT,  1
.set VRING_DESC_F_WRITE, 2

/* Queue sizes */
.set VIRTIO_NET_QUEUE_SIZE, 64
.set VIRTIO_NET_MAX_PKT,    1514   /* max Ethernet frame */

/* VirtIO-Net device base */
.set VIRTIO_NET_BASE,    0x0A000000

.text

/* -----------------------------------------------------------------------------
 * BSS: VirtIO-Net state
 * ----------------------------------------------------------------------------- */
.text

.bss
.align 12
.global virtio_net_rx_desc
virtio_net_rx_desc:
    .skip 4096

.global virtio_net_rx_avail
virtio_net_rx_avail:
    .skip 4096

.global virtio_net_rx_used
virtio_net_rx_used:
    .skip 4096

.global virtio_net_tx_desc
virtio_net_tx_desc:
    .skip 4096

.global virtio_net_tx_avail
virtio_net_tx_avail:
    .skip 4096

.global virtio_net_tx_used
virtio_net_tx_used:
    .skip 4096

.global virtio_net_rx_buf
virtio_net_rx_buf:
    .skip VIRTIO_NET_QUEUE_SIZE * 1536  /* pre-allocated RX buffers */

.global virtio_net_tx_buf
virtio_net_tx_buf:
    .skip VIRTIO_NET_QUEUE_SIZE * 1536  /* TX buffers */

.global virtio_net_rx_hdr
virtio_net_rx_hdr:
    .skip VIRTIO_NET_QUEUE_SIZE * 12  /* RX headers */

.global virtio_net_tx_hdr
virtio_net_tx_hdr:
    .skip 12                          /* single TX header */

.global virtio_net_ready
virtio_net_ready:
    .skip 1

.global virtio_net_mac
virtio_net_mac:
    .skip 6                           /* MAC address */

.bss
.align 4
.global virtio_net_rx_used_idx
virtio_net_rx_used_idx:
    .skip 4

.global virtio_net_tx_used_idx
virtio_net_tx_used_idx:
    .skip 4

.global virtio_net_rx_idx
virtio_net_rx_idx:
    .skip 2                           /* avail ring index */

.global virtio_net_tx_idx
virtio_net_tx_idx:
    .skip 2

.text

/* -----------------------------------------------------------------------------
 * External functions (from virtio.asm)
 * ----------------------------------------------------------------------------- */
.extern virtio_read32
.extern virtio_write32
.extern virtio_reset_device
.extern virtio_negotiate_features
.extern virtio_queue_setup
.extern virtio_queue_notify
.extern serial_puts

/* -----------------------------------------------------------------------------
 * Function: virtio_net_init
 * Description: Initialize VirtIO-Net device and both queues
 * Input: none
 * Output: x0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global virtio_net_init
virtio_net_init:
    stp     x29, x30, [sp, #-16]!

    /* Check magic */
    ldr     x0, =VIRTIO_NET_BASE
    mov     x1, #VIRTIO_MMIO_MAGIC_VALUE
    bl      virtio_read32
    ldr     x8, =0x74726976
    cmp     w0, w8
    b.ne    net_init_error

    /* Check device ID */
    ldr     x0, =VIRTIO_NET_BASE
    mov     x1, #VIRTIO_MMIO_DEVICE_ID
    bl      virtio_read32
    cmp     w0, #VIRTIO_DEV_NET
    b.ne    net_init_error

    /* Reset device */
    ldr     x0, =VIRTIO_NET_BASE
    bl      virtio_reset_device

    /* Negotiate features */
    ldr     x0, =VIRTIO_NET_BASE
    bl      virtio_negotiate_features

    /* Read MAC address from config space (offset 0 in config) */
    ldr     x0, =VIRTIO_NET_BASE
    add     x0, x0, #VIRTIO_MMIO_CONFIG
    ldr     w1, [x0]              /* first 4 bytes of MAC */
    adrp    x2, virtio_net_mac
    add     x2, x2, #:lo12:virtio_net_mac
    str     w1, [x2]
    ldr     w1, [x0, #4]          /* next 2 bytes + padding */
    strh    w1, [x2, #4]

    /* Setup RX queue (queue 0) */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w1, #0
    adrp    x2, virtio_net_rx_desc
    add     x2, x2, #:lo12:virtio_net_rx_desc
    adrp    x3, virtio_net_rx_avail
    add     x3, x3, #:lo12:virtio_net_rx_avail
    adrp    x4, virtio_net_rx_used
    add     x4, x4, #:lo12:virtio_net_rx_used
    mov     w5, #VIRTIO_NET_QUEUE_SIZE
    bl      virtio_queue_setup

    /* Pre-fill RX ring with buffers */
    bl      virtio_net_fill_rx

    /* Setup TX queue (queue 1) */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w1, #1
    adrp    x2, virtio_net_tx_desc
    add     x2, x2, #:lo12:virtio_net_tx_desc
    adrp    x3, virtio_net_tx_avail
    add     x3, x3, #:lo12:virtio_net_tx_avail
    adrp    x4, virtio_net_tx_used
    add     x4, x4, #:lo12:virtio_net_tx_used
    mov     w5, #VIRTIO_NET_QUEUE_SIZE
    bl      virtio_queue_setup

    /* Set DRIVER_OK */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w2, #(VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_FEAT_OK | VIRTIO_STATUS_DRIVER_OK)
    mov     x1, #VIRTIO_MMIO_STATUS
    bl      virtio_write32

    /* Mark ready */
    adrp    x0, virtio_net_ready
    add     x0, x0, #:lo12:virtio_net_ready
    mov     w1, #1
    strb    w1, [x0]

    /* Init indices */
    adrp    x0, virtio_net_rx_used_idx
    add     x0, x0, #:lo12:virtio_net_rx_used_idx
    str     wzr, [x0]
    adrp    x0, virtio_net_tx_used_idx
    add     x0, x0, #:lo12:virtio_net_tx_used_idx
    str     wzr, [x0]
    adrp    x0, virtio_net_rx_idx
    add     x0, x0, #:lo12:virtio_net_rx_idx
    strh    wzr, [x0]
    adrp    x0, virtio_net_tx_idx
    add     x0, x0, #:lo12:virtio_net_tx_idx
    strh    wzr, [x0]

    adrp    x0, msg_net_ready
    add     x0, x0, #:lo12:msg_net_ready
    bl      serial_puts

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

net_init_error:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_net_fill_rx
 * Description: Pre-fill RX ring with buffer descriptors
 * Input: none (uses globals)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_net_fill_rx
virtio_net_fill_rx:
    stp     x29, x30, [sp, #-16]!

    adrp    x4, virtio_net_rx_desc
    add     x4, x4, #:lo12:virtio_net_rx_desc
    adrp    x5, virtio_net_rx_hdr
    add     x5, x5, #:lo12:virtio_net_rx_hdr
    adrp    x6, virtio_net_rx_buf
    add     x6, x6, #:lo12:virtio_net_rx_buf
    adrp    x7, virtio_net_rx_avail
    add     x7, x7, #:lo12:virtio_net_rx_avail

    mov     x8, #VIRTIO_NET_QUEUE_SIZE
    mov     x9, #0                /* queue index */

fill_rx_loop:
    cbz     x8, fill_rx_done

    /* Descriptor 0: header (12 bytes, write, next=1) */
    str     x5, [x4, #0]          /* addr = header */
    mov     w1, #12
    str     w1, [x4, #8]          /* len */
    ldr     w1, =(VRING_DESC_F_NEXT | VRING_DESC_F_WRITE | 1)
    str     w1, [x4, #12]

    /* Descriptor 1: data buffer (1514 bytes, write, no next) */
    str     x6, [x4, #16]         /* addr = buffer */
    mov     w1, #VIRTIO_NET_MAX_PKT
    str     w1, [x4, #24]         /* len */
    mov     w1, #VRING_DESC_F_WRITE
    str     w1, [x4, #28]

    /* Add to avail ring: desc index = queue_index */
    ldrh    w1, [x7, #4]          /* avail idx */
    and     w2, w1, #(VIRTIO_NET_QUEUE_SIZE - 1)
    mov     w3, #2
    mul     w2, w2, w3
    add     w2, w2, #6
    strh    w9, [x7, x2]          /* ring entry = queue index */
    add     w1, w1, #1
    strh    w1, [x7, #4]

    /* Advance pointers */
    add     x4, x4, #32           /* next descriptor pair */
    add     x5, x5, #12           /* next header */
    add     x6, x6, #1536         /* next buffer */
    add     x9, x9, #1
    sub     x8, x8, #1
    b       fill_rx_loop

fill_rx_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_net_send
 * Description: Send a packet
 * Input: x0 = buffer VA, w1 = length
 * Output: x0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global virtio_net_send
virtio_net_send:
    stp     x29, x30, [sp, #-16]!
    mov     x6, x0              /* save buffer */
    mov     w7, w1              /* save length */

    /* Check ready */
    adrp    x0, virtio_net_ready
    add     x0, x0, #:lo12:virtio_net_ready
    ldrb    w0, [x0]
    cbz     w0, net_not_ready

    /* Setup TX descriptor chain:
     * desc[0]: header (12 bytes, read, next=1)
     * desc[1]: packet data (len bytes, read, no next)
     */
    adrp    x0, virtio_net_tx_desc
    add     x0, x0, #:lo12:virtio_net_tx_desc

    /* Clear header */
    str     xzr, [x0, #0]

    /* Descriptor 0: header */
    adrp    x1, virtio_net_tx_hdr
    add     x1, x1, #:lo12:virtio_net_tx_hdr
    str     x1, [x0, #0]
    mov     w1, #12
    str     w1, [x0, #8]
    ldr     w1, =(VRING_DESC_F_NEXT | 1)
    str     w1, [x0, #12]

    /* Descriptor 1: data */
    str     x6, [x0, #16]
    str     w7, [x0, #24]
    mov     w1, #0              /* flags = 0 (device reads) */
    str     w1, [x0, #28]

    /* Add to TX avail ring */
    adrp    x0, virtio_net_tx_avail
    add     x0, x0, #:lo12:virtio_net_tx_avail
    ldrh    w1, [x0, #4]
    and     w2, w1, #(VIRTIO_NET_QUEUE_SIZE - 1)
    mov     w3, #2
    mul     w2, w2, w3
    add     w2, w2, #6
    strh    wzr, [x0, x2]       /* desc index 0 */
    add     w1, w1, #1
    strh    w1, [x0, #4]

    /* Notify device on TX queue (queue 1) */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w1, #1
    bl      virtio_queue_notify

    /* Wait for completion */
    adrp    x0, virtio_net_tx_used
    add     x0, x0, #:lo12:virtio_net_tx_used
    adrp    x1, virtio_net_tx_used_idx
    add     x1, x1, #:lo12:virtio_net_tx_used_idx
    ldr     w2, [x1]

tx_wait:
    ldrh    w3, [x0, #4]
    cmp     w3, w2
    b.eq    tx_wait

    /* Read status */
    ldr     w0, [x0, #8]        /* used len */
    add     w2, w2, #1
    str     w2, [x1]

    mov     x0, #0              /* success */
    ldp     x29, x30, [sp], #16
    ret

net_not_ready:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_net_recv
 * Description: Receive a packet (non-blocking poll)
 * Input: x0 = buffer VA, w1 = buffer size
 * Output: x0 = packet length, or -1 if no packet
 * ----------------------------------------------------------------------------- */
.global virtio_net_recv
virtio_net_recv:
    stp     x29, x30, [sp, #-16]!
    mov     x6, x0              /* save buffer */
    mov     w7, w1              /* save buf size */

    /* Check ready */
    adrp    x0, virtio_net_ready
    add     x0, x0, #:lo12:virtio_net_ready
    ldrb    w0, [x0]
    cbz     w0, net_recv_none

    /* Check RX used ring for completed packet */
    adrp    x0, virtio_net_rx_used
    add     x0, x0, #:lo12:virtio_net_rx_used
    adrp    x1, virtio_net_rx_used_idx
    add     x1, x1, #:lo12:virtio_net_rx_used_idx
    ldr     w2, [x1]

    ldrh    w3, [x0, #4]        /* used ring idx */
    cmp     w3, w2
    b.eq    net_recv_none       /* no packet yet */

    /* Read packet length from used entry */
    ldr     w4, [x0, #8]        /* used len (includes 12-byte header) */
    sub     w4, w4, #12         /* subtract header */

    /* Copy data from RX buffer to user buffer */
    /* The completed desc index tells us which buffer */
    ldrh    w5, [x0, #6]        /* used id (descriptor index) */

    /* Calculate source buffer address */
    adrp    x8, virtio_net_rx_buf
    add     x8, x8, #:lo12:virtio_net_rx_buf
    mov     x9, #1536
    mul     x9, x9, x5
    add     x8, x8, x9          /* src = rx_buf[idx * 1536] */

    /* Calculate header offset */
    adrp    x10, virtio_net_rx_hdr
    add     x10, x10, #:lo12:virtio_net_rx_hdr
    mov     x11, #12
    mul     x11, x11, x5
    add     x10, x10, x11       /* header = rx_hdr[idx * 12] */

    /* Copy packet data (skip 12-byte header) */
    add     x8, x8, #12         /* skip header in buffer */
    mov     x9, x6              /* dest = user buffer */
    mov     w10, w4             /* count */

recv_copy_loop:
    cbz     w10, recv_copy_done
    ldrb    w11, [x8], #1
    strb    w11, [x9], #1
    sub     w10, w10, #1
    b       recv_copy_loop

recv_copy_done:
    /* Re-fill this RX descriptor */
    bl      virtio_net_refill_rx

    /* Update used idx */
    add     w2, w2, #1
    str     w2, [x1]

    mov     x0, x4              /* return packet length */
    ldp     x29, x30, [sp], #16
    ret

net_recv_none:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_net_refill_rx
 * Description: Re-add a consumed RX descriptor to the avail ring
 * Input: w5 = descriptor index (from used id)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_net_refill_rx
virtio_net_refill_rx:
    stp     x29, x30, [sp, #-16]!

    /* Calculate buffer and header addresses for this index */
    adrp    x0, virtio_net_rx_desc
    add     x0, x0, #:lo12:virtio_net_rx_desc
    mov     x1, #32
    mul     x1, x1, x5
    add     x0, x0, x1          /* desc pointer */

    adrp    x1, virtio_net_rx_hdr
    add     x1, x1, #:lo12:virtio_net_rx_hdr
    mov     x2, #12
    mul     x2, x2, x5
    add     x1, x1, x2
    str     x1, [x0, #0]        /* reset header addr */

    adrp    x1, virtio_net_rx_buf
    add     x1, x1, #:lo12:virtio_net_rx_buf
    mov     x2, #1536
    mul     x2, x2, x5
    add     x1, x1, x2
    add     x1, x1, #12         /* skip header portion */
    str     x1, [x0, #16]       /* reset buffer addr */

    /* Add to avail ring */
    adrp    x0, virtio_net_rx_avail
    add     x0, x0, #:lo12:virtio_net_rx_avail
    ldrh    w1, [x0, #4]
    and     w2, w1, #(VIRTIO_NET_QUEUE_SIZE - 1)
    mov     w3, #2
    mul     w2, w2, w3
    add     w2, w2, #6
    strh    w5, [x0, x2]        /* desc index */
    add     w1, w1, #1
    strh    w1, [x0, #4]

    /* Notify device on RX queue (queue 0) */
    ldr     x0, =VIRTIO_NET_BASE
    mov     w1, #0
    bl      virtio_queue_notify

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_net_get_mac
 * Description: Get the MAC address
 * Input: x0 = buffer (6 bytes)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global virtio_net_get_mac
virtio_net_get_mac:
    stp     x29, x30, [sp, #-16]!
    adrp    x1, virtio_net_mac
    add     x1, x1, #:lo12:virtio_net_mac
    ldr     w2, [x1]
    str     w2, [x0]
    ldrh    w2, [x1, #4]
    strh    w2, [x0, #4]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Strings
 * ----------------------------------------------------------------------------- */
.section .rodata, "a", %progbits
.align 4
msg_net_ready:
    .asciz "[virtio-net] device ready\n"
