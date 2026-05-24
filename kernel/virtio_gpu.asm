/*
 * aiasm-aarch64/kernel/virtio_gpu.asm
 * VirtIO-GPU driver for QEMU virt
 * Spec: VirtIO 1.2, GPU device (device_id=16)
 *
 * Device at 0x0A000400 (VIRTIO_DEV2_BASE)
 * Control queue (queue 0) for commands
 */
.arch armv8-a

/* VirtIO-GPU device base */
.set VIRTIO_GPU_BASE,     0x0A000400

/* VirtIO-GPU control commands */
.set GPU_CMD_GET_DISPLAY_INFO,   0x0100
.set GPU_CMD_RESOURCE_CREATE_2D, 0x0101
.set GPU_CMD_RESOURCE_UNREF,     0x0102
.set GPU_CMD_SET_SCANOUT,        0x0103
.set GPU_CMD_RESOURCE_FLUSH,     0x0104
.set GPU_CMD_TRANSFER_TO_HOST_2D,0x0105
.set GPU_CMD_RESOURCE_ATTACH_BACKING, 0x0106

/* GPU formats */
.set GPU_FORMAT_XRGB8888, 0x34325258  /* "XR24" little-endian */

/* Default resolution */
.set GPU_DEFAULT_WIDTH,   800
.set GPU_DEFAULT_HEIGHT,  600

/* GPU queue: 64 descriptors */
.set GPU_QUEUE_SIZE,      64

/* Control header size */
.set GPU_CTRL_HDR_SIZE,   24

/* VirtIO constants (copied from virtio.asm) */
.set VIRTIO_MMIO_MAGIC_VALUE,     0x000
.set VIRTIO_MMIO_VERSION,         0x004
.set VIRTIO_MMIO_DEVICE_ID,       0x008
.set VIRTIO_MMIO_STATUS,          0x070
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
.set VIRTIO_STATUS_ACK,     1
.set VIRTIO_STATUS_DRIVER,  2
.set VIRTIO_STATUS_DRIVER_OK, 4
.set VIRTIO_STATUS_FEAT_OK, 8
.set VRING_DESC_F_NEXT,     1
.set VRING_DESC_F_WRITE,    2

/* GPU ready flag */
.set GPU_STATE_UNINIT,    0
.set GPU_STATE_READY,     1

.text

/* -----------------------------------------------------------------------------
 * External functions
 * ----------------------------------------------------------------------------- */
.extern virtio_write32
.extern virtio_read32
.extern virtio_negotiate_features
.extern virtio_queue_setup
.extern virtio_queue_notify
.extern serial_puts
.extern mem_alloc_page

/* -----------------------------------------------------------------------------
 * Function: virtio_gpu_init
 * Initialize VirtIO-GPU device
 * Returns w0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global virtio_gpu_init
virtio_gpu_init:
    stp     x29, x30, [sp, #-16]!

    /* Check if device exists (magic + device_id) */
    ldr     x0, =VIRTIO_GPU_BASE
    mov     x1, #VIRTIO_MMIO_MAGIC_VALUE
    bl      virtio_read32
    ldr     x8, =0x74726976
    cmp     w0, w8
    b.ne    _gpu_init_fail

    /* Check device_id == 16 (GPU) */
    mov     x1, #VIRTIO_MMIO_DEVICE_ID
    bl      virtio_read32
    cmp     w0, #16
    b.ne    _gpu_init_fail

    /* Store device base */
    ldr     x8, =VIRTIO_GPU_BASE
    adrp    x9, gpu_device_base
    add     x9, x9, #:lo12:gpu_device_base
    str     x8, [x9]

    /* Step 1: Reset device */
    bl      virtio_reset_gpu

    /* Step 2: ACK + DRIVER */
    mov     x0, x8
    bl      virtio_negotiate_features

    /* Step 3: Setup control virtqueue (queue 0) */
    mov     x0, x8

    /* Allocate descriptor/avail/used rings */
    bl      mem_alloc_page
    cbz     x0, _gpu_init_fail
    mov     x9, x0              /* desc ring */

    bl      mem_alloc_page
    cbz     x0, _gpu_init_fail
    mov     x10, x0             /* avail ring */

    bl      mem_alloc_page
    cbz     x0, _gpu_init_fail
    mov     x11, x0             /* used ring */

    /* Zero the rings */
    mov     x0, x9
    mov     x1, #0
    mov     x2, #0x1000
    bl      _gpu_memset

    mov     x0, x10
    mov     x1, #0
    mov     x2, #0x1000
    bl      _gpu_memset

    mov     x0, x11
    mov     x1, #0
    mov     x2, #0x1000
    bl      _gpu_memset

    /* virtio_queue_setup(x0=dev, w1=0, x2=desc, x3=avail, x4=used, w5=size) */
    mov     x0, x8
    mov     w1, #0              /* queue 0 = control */
    mov     x2, x9
    mov     x3, x10
    mov     x4, x11
    mov     w5, #GPU_QUEUE_SIZE
    bl      virtio_queue_setup

    /* Step 4: DRIVER_OK */
    mov     w2, #(VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_FEAT_OK | VIRTIO_STATUS_DRIVER_OK)
    mov     x1, #VIRTIO_MMIO_STATUS
    bl      virtio_write32

    /* Step 5: Get display info */
    bl      _gpu_get_display_info

    /* Step 6: Create 2D resource */
    mov     x0, #GPU_DEFAULT_WIDTH
    mov     x1, #GPU_DEFAULT_HEIGHT
    bl      virtio_gpu_create_2d
    cbnz    w0, _gpu_init_fail

    /* Step 7: Set scanout */
    bl      _gpu_set_scanout

    /* Mark ready */
    adrp    x0, gpu_ready
    add     x0, x0, #:lo12:gpu_ready
    mov     w1, #1
    strb    w1, [x0]

    adrp    x0, msg_gpu_ready
    add     x0, x0, #:lo12:msg_gpu_ready
    bl      serial_puts

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

_gpu_init_fail:
    adrp    x0, msg_gpu_fail
    add     x0, x0, #:lo12:msg_gpu_fail
    bl      serial_puts

    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

virtio_reset_gpu:
    stp     x29, x30, [sp, #-16]!
    mov     w1, #0
    mov     x2, #VIRTIO_MMIO_STATUS
    bl      virtio_write32
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _gpu_get_display_info: query display configuration
 * ----------------------------------------------------------------------------- */
_gpu_get_display_info:
    stp     x29, x30, [sp, #-16]!

    /* Build command in command buffer */
    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header: type(4) + flags(4) + fence_id(8) + ctx_id(4) + padding(4) = 24 */
    ldr     w1, =GPU_CMD_GET_DISPLAY_INFO
    str     w1, [x0]            /* type */
    str     wzr, [x0, #4]       /* flags */
    str     xzr, [x0, #8]       /* fence_id */
    str     wzr, [x0, #16]      /* ctx_id */

    /* Submit via virtqueue */
    bl      _gpu_submit_command

    /* Read response: width at offset 24, height at offset 28 */
    adrp    x0, gpu_resp_buf
    add     x0, x0, #:lo12:gpu_resp_buf
    ldr     w1, [x0, #24]       /* width */
    ldr     w2, [x0, #28]       /* height */

    /* Store dimensions */
    adrp    x3, gpu_width
    add     x3, x3, #:lo12:gpu_width
    str     w1, [x3]
    adrp    x3, gpu_height
    add     x3, x3, #:lo12:gpu_height
    str     w2, [x3]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _gpu_submit_command: submit ctrl header + read response via virtqueue
 * Uses gpu_cmd_buf and gpu_resp_buf
 * ----------------------------------------------------------------------------- */
_gpu_submit_command:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* save command buffer */

    adrp    x9, gpu_desc_idx
    add     x9, x9, #:lo12:gpu_desc_idx
    ldr     w10, [x9]
    and     w10, w10, #(GPU_QUEUE_SIZE - 1)

    /* Desc ring base */
    adrp    x11, gpu_desc_ring
    add     x11, x11, #:lo12:gpu_desc_ring

    /* Descriptor 0: command buffer (device-readable) */
    lsl     x12, x10, #4        /* desc index * 16 */
    adrp    x13, gpu_cmd_buf
    add     x13, x13, #:lo12:gpu_cmd_buf
    str     x13, [x11, x12]     /* addr */
    mov     w14, #GPU_CTRL_HDR_SIZE
    add     x9, x11, x12
    strh    w14, [x9, #8] /* len */
    mov     w14, #(VRING_DESC_F_NEXT)
    strh    w14, [x9, #10] /* flags */
    add     w14, w10, #1
    and     w14, w14, #(GPU_QUEUE_SIZE - 1)
    strh    w14, [x9, #12] /* next */

    /* Descriptor 1: response buffer (device-writable) */
    add     x12, x12, #16
    adrp    x13, gpu_resp_buf
    add     x13, x13, #:lo12:gpu_resp_buf
    str     x13, [x11, x12]     /* addr */
    mov     w14, #256
    add     x9, x11, x12
    strh    w14, [x9, #8] /* len */
    mov     w14, #VRING_DESC_F_WRITE
    strh    w14, [x9, #10] /* flags */
    strh    wzr, [x9, #12] /* no next */

    /* Update avail ring */
    adrp    x11, gpu_avail_ring
    add     x11, x11, #:lo12:gpu_avail_ring
    ldrh    w14, [x11]          /* avail.idx */
    add     x9, x11, w14, uxtw #1
    strh    w10, [x9, #2] /* avail ring entry */
    add     w14, w14, #1
    strh    w14, [x11]          /* avail.idx++ */

    /* Notify queue 0 */
    adrp    x0, gpu_device_base
    add     x0, x0, #:lo12:gpu_device_base
    mov     w1, #0
    bl      virtio_queue_notify

    /* Increment desc index */
    adrp    x0, gpu_desc_idx
    add     x0, x0, #:lo12:gpu_desc_idx
    ldr     w1, [x0]
    add     w1, w1, #2
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_gpu_create_2d
 * Create a 2D resource
 * x0 = width, x1 = height, x2 = format (or 0 for XRGB8888)
 * Returns w0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global virtio_gpu_create_2d
virtio_gpu_create_2d:
    stp     x29, x30, [sp, #-16]!

    mov     w8, w0              /* width */
    mov     w9, w1              /* height */
    cbz     w2, _gpu_fmt_default
    mov     w10, w2
    b       _gpu_fmt_done
_gpu_fmt_default:
    ldr     w10, =GPU_FORMAT_XRGB8888
_gpu_fmt_done:

    /* Resource ID = 1 */
    adrp    x0, gpu_resource_id
    add     x0, x0, #:lo12:gpu_resource_id
    mov     w1, #1
    str     w1, [x0]

    /* Build command: CREATE_2D header + create_2d body */
    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header */
    ldr     w1, =GPU_CMD_RESOURCE_CREATE_2D
    str     w1, [x0]
    str     wzr, [x0, #4]
    str     xzr, [x0, #8]
    str     wzr, [x0, #16]

    /* create_2d: resource_id(4) + nitems(4) + size(8) + width(4) + height(4) + format(4) + padding(4) */
    str     w1, [x0, #24]       /* resource_id */
    str     wzr, [x0, #28]      /* nitems */
    str     xzr, [x0, #32]      /* size (0 = auto) */
    str     w8, [x0, #40]       /* width */
    str     w9, [x0, #44]       /* height */
    str     w10, [x0, #48]      /* format */

    bl      _gpu_submit_command

    /* Attach backing (memory region for the resource) */
    bl      _gpu_attach_backing

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _gpu_attach_backing: attach memory backing to resource
 * ----------------------------------------------------------------------------- */
_gpu_attach_backing:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header */
    ldr     w1, =GPU_CMD_RESOURCE_ATTACH_BACKING
    str     w1, [x0]
    str     wzr, [x0, #4]
    str     xzr, [x0, #8]
    str     wzr, [x0, #16]

    /* body: resource_id(4) + nr_entries(4) + mem_entry */
    adrp    x1, gpu_resource_id
    add     x1, x1, #:lo12:gpu_resource_id
    ldr     w2, [x1]
    str     w2, [x0, #24]       /* resource_id */
    mov     w2, #1
    str     w2, [x0, #28]       /* nr_entries */

    /* mem_entry: addr(8) + length(8) */
    /* Allocate a framebuffer page */
    bl      mem_alloc_page
    cbz     x0, _gpu_attach_fail

    adrp    x1, gpu_fb_base
    add     x1, x1, #:lo12:gpu_fb_base
    str     x0, [x1]            /* save fb ptr */

    /* For now, use a simple offset within the page */
    str     x0, [x0, #32]       /* addr (identity mapped) */
    ldr     w2, =800*600*4
    str     x2, [x0, #40]       /* length */

    bl      _gpu_submit_command

    ldp     x29, x30, [sp], #16
    ret

_gpu_attach_fail:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _gpu_set_scanout: set scanout to resource 1
 * ----------------------------------------------------------------------------- */
_gpu_set_scanout:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header */
    ldr     w1, =GPU_CMD_SET_SCANOUT
    str     w1, [x0]
    str     wzr, [x0, #4]
    str     xzr, [x0, #8]
    str     wzr, [x0, #16]

    /* scanout body: r.x(4) + r.y(4) + r.width(4) + r.height(4) + scanout_id(4) + resource_id(4) */
    str     wzr, [x0, #24]      /* x */
    str     wzr, [x0, #28]      /* y */
    ldr     w1, =GPU_DEFAULT_WIDTH
    str     w1, [x0, #32]       /* width */
    ldr     w1, =GPU_DEFAULT_HEIGHT
    str     w1, [x0, #36]       /* height */
    str     wzr, [x0, #40]      /* scanout_id */
    mov     w1, #1
    str     w1, [x0, #44]       /* resource_id */

    bl      _gpu_submit_command

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_gpu_transfer
 * Transfer pixel data to GPU framebuffer
 * x0 = x, x1 = y, x2 = width, x3 = height
 * ----------------------------------------------------------------------------- */
.global virtio_gpu_transfer
virtio_gpu_transfer:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header */
    ldr     w4, =GPU_CMD_TRANSFER_TO_HOST_2D
    str     w4, [x0]
    str     wzr, [x0, #4]
    str     xzr, [x0, #8]
    str     wzr, [x0, #16]

    /* transfer body: rect (x,y,w,h) + offset(8) + resource_id(4) + padding(4) */
    str     w0, [x0, #24]       /* x */
    str     w1, [x0, #28]       /* y */
    str     w2, [x0, #32]       /* width */
    str     w3, [x0, #36]       /* height */
    str     xzr, [x0, #40]      /* offset */
    adrp    x4, gpu_resource_id
    add     x4, x4, #:lo12:gpu_resource_id
    ldr     w5, [x4]
    str     w5, [x0, #48]       /* resource_id */

    bl      _gpu_submit_command

    /* Flush after transfer */
    mov     x0, x2              /* width */
    mov     x1, x3              /* height */
    bl      virtio_gpu_flush

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: virtio_gpu_flush
 * Flush region to display
 * x0 = width, x1 = height
 * ----------------------------------------------------------------------------- */
.global virtio_gpu_flush
virtio_gpu_flush:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, gpu_cmd_buf
    add     x0, x0, #:lo12:gpu_cmd_buf

    /* ctrl header */
    ldr     w3, =GPU_CMD_RESOURCE_FLUSH
    str     w3, [x0]
    str     wzr, [x0, #4]
    str     xzr, [x0, #8]
    str     wzr, [x0, #16]

    /* flush body: rect(x=0,y=0,w,h) + resource_id(4) */
    str     wzr, [x0, #24]      /* x */
    str     wzr, [x0, #28]      /* y */
    str     w0, [x0, #32]       /* width */
    str     w1, [x0, #36]       /* height */
    str     xzr, [x0, #40]      /* offset */
    adrp    x2, gpu_resource_id
    add     x2, x2, #:lo12:gpu_resource_id
    ldr     w3, [x2]
    str     w3, [x0, #44]       /* resource_id */

    bl      _gpu_submit_command

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _gpu_memset
 * x0 = ptr, w1 = value, x2 = count
 * ----------------------------------------------------------------------------- */
_gpu_memset:
    cbz     x2, 2f
1:  strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * BSS: GPU state
 * ----------------------------------------------------------------------------- */
.bss
.align 4
gpu_device_base:
    .skip 8
gpu_ready:
    .skip 1
gpu_width:
    .skip 4
gpu_height:
    .skip 4
gpu_resource_id:
    .skip 4
gpu_fb_base:
    .skip 8

/* Virtqueue rings for GPU */
.align 12
gpu_desc_ring:
    .skip GPU_QUEUE_SIZE * 16
gpu_avail_ring:
    .skip (2 + GPU_QUEUE_SIZE * 2)
.align 12
gpu_used_ring:
    .skip (2 + GPU_QUEUE_SIZE * 8)

gpu_desc_idx:
    .skip 4

/* Command and response buffers */
.align 8
gpu_cmd_buf:
    .skip 256
gpu_resp_buf:
    .skip 256

.section .rodata
.align 4
msg_gpu_ready:
    .asciz "[virtio-gpu] ready 800x600\n"
msg_gpu_fail:
    .asciz "[virtio-gpu] init failed\n"
