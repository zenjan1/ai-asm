/*
 * aiasm-aarch64/kernel/dma.asm
 * DMA Controller Interface (v1.0)
 *
 * AArch64 ASM implementation of DMA (Direct Memory Access) for efficient I/O.
 * Manages DMA channels, transfers, and cache coherency.
 *
 * DMA features:
 *   - Channel allocation and management
 *   - Transfer setup: source, dest, length, direction
 *   - Scatter-gather support for multi-buffer transfers
 *   - Cache coherency: clean/invalidate before/after transfers
 *   - Completion interrupts
 *
 * Integration: called from virtio_blk.asm, virtio_net.asm for high-performance I/O
 *
 * API:
 *   dma_init()                              — init DMA controller
 *   dma_alloc_channel() => channel_id       — allocate a DMA channel
 *   dma_free_channel(channel_id)            — release channel
 *   dma_setup_transfer(ch, src, dst, len, dir) — setup transfer
 *   dma_start(channel_id)                   — start transfer
 *   dma_wait_complete(channel_id)           — wait for completion
 */
.arch armv8-a

/* DMA transfer directions */
.set DMA_DIR_MEM_TO_MEM,    0
.set DMA_DIR_MEM_TO_DEV,    1
.set DMA_DIR_DEV_TO_MEM,    2
.set DMA_DIR_DEV_TO_DEV,    3

/* DMA channel states */
.set DMA_CH_FREE,           0
.set DMA_CH_ALLOCATED,      1
.set DMA_CH_BUSY,           2
.set DMA_CH_ERROR,          3

/* Constants */
.set DMA_MAX_CHANNELS,      8

/* Channel descriptor (32 bytes each) */
/* 0:  state       (4) */
/* 4:  src_addr    (8) */
/* 12: dst_addr    (8) */
/* 20: length      (4) */
/* 24: direction   (4) */
/* 28: status      (4)  — 0=pending, 1=complete, -1=error */

.bss
.align 4
.global dma_init_done
dma_init_done:
    .skip 4

.align 4
dma_channels:
    .skip DMA_MAX_CHANNELS * 32   /* 256 bytes */

.text

/* -----------------------------------------------------------------------------
 * dma_init — initialize DMA controller
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global dma_init
dma_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero channel table */
    ldr     x0, =dma_channels
    mov     x1, #(DMA_MAX_CHANNELS * 32)
1:  cbz     x1, 2f
    strb    wzr, [x0], #1
    sub     x1, x1, #1
    b       1b
2:
    ldr     x0, =dma_init_done
    mov     w1, #1
    str     w1, [x0]

    /* TODO: Initialize hardware DMA controller */
    /* In real impl: configure DMA controller registers */

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: find free DMA channel
 * Output: x0 = channel index, or -1 if none free
 */
.Ldma_find_free:
    mov     w0, #0
.Ldma_find_loop:
    cmp     w0, #DMA_MAX_CHANNELS
    b.hs    .Ldma_find_fail
    ldr     x1, =dma_channels
    mov     x2, #32
    mul     x2, w0, x2
    add     x1, x1, x2
    ldr     w2, [x1]            /* state */
    cmp     w2, #DMA_CH_FREE
    beq     .Ldma_find_done
    add     w0, w0, #1
    b       .Ldma_find_loop
.Ldma_find_fail:
    mov     x0, #-1
.Ldma_find_done:
    ret

/* -----------------------------------------------------------------------------
 * dma_alloc_channel — allocate a DMA channel
 * Output: x0 = channel ID (0-7), or -1 (all channels busy)
 * ----------------------------------------------------------------------------- */
.global dma_alloc_channel
dma_alloc_channel:
    stp     x29, x30, [sp, #-16]!

    bl      .Ldma_find_free
    cmp     x0, #-1
    beq     .Ldma_alloc_fail

    /* Mark channel as allocated */
    ldr     x1, =dma_channels
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2
    mov     w2, #DMA_CH_ALLOCATED
    str     w2, [x1]

    b       .Ldma_alloc_ret

.Ldma_alloc_fail:
    mov     x0, #-1

.Ldma_alloc_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * dma_free_channel — release a DMA channel
 * Input: x0 = channel ID
 * Output: x0 = 0 (ok), -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global dma_free_channel
dma_free_channel:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #DMA_MAX_CHANNELS
    b.hs    .Ldma_free_fail

    ldr     x1, =dma_channels
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2

    mov     w2, #DMA_CH_FREE
    str     w2, [x1]

    mov     x0, #0
    b       .Ldma_free_ret

.Ldma_free_fail:
    mov     x0, #-1

.Ldma_free_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * dma_setup_transfer — setup a DMA transfer
 * Input: x0 = channel ID, x1 = src addr, x2 = dst addr, x3 = length,
 *        x4 = direction (DMA_DIR_*)
 * Output: x0 = 0 (ok), -1 (invalid channel)
 * ----------------------------------------------------------------------------- */
.global dma_setup_transfer
dma_setup_transfer:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* channel ID */

    cmp     x0, #DMA_MAX_CHANNELS
    b.hs    .Ldma_setup_fail

    /* Get channel descriptor */
    ldr     x5, =dma_channels
    mov     x6, #32
    mul     x6, x19, x6
    add     x5, x5, x6

    /* Check channel is allocated */
    ldr     w6, [x5]
    cmp     w6, #DMA_CH_ALLOCATED
    b.ne    .Ldma_setup_fail

    /* Store transfer parameters */
    str     x1, [x5, #4]        /* src_addr */
    str     x2, [x5, #12]       /* dst_addr */
    str     w3, [x5, #20]       /* length */
    str     w4, [x5, #24]       /* direction */

    /* Set status to pending */
    str     wzr, [x5, #28]

    /* Clean data cache for source (if reading from memory) */
    cmp     w4, #DMA_DIR_DEV_TO_MEM
    beq     .Ldma_setup_no_clean_src
    /* TODO: Clean D-cache for source region */

.Ldma_setup_no_clean_src:
    /* Invalidate D-cache for destination (if writing to memory) */
    cmp     w4, #DMA_DIR_MEM_TO_DEV
    beq     .Ldma_setup_no_inv_dst
    /* TODO: Invalidate D-cache for destination region */

.Ldma_setup_no_inv_dst:
    mov     x0, #0
    b       .Ldma_setup_ret

.Ldma_setup_fail:
    mov     x0, #-1

.Ldma_setup_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * dma_start — start a DMA transfer
 * Input: x0 = channel ID
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global dma_start
dma_start:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #DMA_MAX_CHANNELS
    b.hs    .Ldma_start_fail

    ldr     x1, =dma_channels
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2

    /* Check channel is allocated */
    ldr     w2, [x1]
    cmp     w2, #DMA_CH_ALLOCATED
    b.ne    .Ldma_start_fail

    /* Mark as busy */
    mov     w2, #DMA_CH_BUSY
    str     w2, [x1]

    /* TODO: Start hardware DMA transfer */
    /* In real impl: write to DMA controller registers */

    /* For simulation: immediately mark complete */
    mov     w2, #1              /* status = complete */
    str     w2, [x1, #28]
    mov     w2, #DMA_CH_ALLOCATED
    str     w2, [x1]

    mov     x0, #0
    b       .Ldma_start_ret

.Ldma_start_fail:
    mov     x0, #-1

.Ldma_start_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * dma_wait_complete — wait for DMA transfer completion
 * Input: x0 = channel ID
 * Output: x0 = 0 (complete), -1 (error)
 * ----------------------------------------------------------------------------- */
.global dma_wait_complete
dma_wait_complete:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #DMA_MAX_CHANNELS
    b.hs    .Ldma_wait_fail

    ldr     x1, =dma_channels
    mov     x2, #32
    mul     x2, x0, x2
    add     x1, x1, x2

    /* Check status */
    ldr     w2, [x1, #28]
    cmp     w2, #1
    beq     .Ldma_wait_done     /* complete */
    cmp     w2, #-1
    beq     .Ldma_wait_fail     /* error */

    /* TODO: Wait for interrupt (WFI) */
    /* For now: spin loop */
.Ldma_wait_spin:
    ldr     w2, [x1, #28]
    cmp     w2, #0
    beq     .Ldma_wait_spin

    cmp     w2, #1
    beq     .Ldma_wait_done

.Ldma_wait_fail:
    mov     x0, #-1
    b       .Ldma_wait_ret

.Ldma_wait_done:
    mov     x0, #0

.Ldma_wait_ret:
    ldp     x29, x30, [sp], #16
    ret
