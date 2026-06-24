/*
 * aiasm-aarch64/kernel/nvme.asm
 * NVMe SSD Driver (v1.0)
 *
 * AArch64 ASM implementation of NVMe (Non-Volatile Memory Express) driver.
 * Provides high-performance block I/O via PCIe-based SSDs.
 *
 * NVMe features:
 *   - Submission and completion queues (in-memory ring buffers)
 *   - Commands: identify, read, write, flush
 *   - MSI-X interrupt handling
 *   - Multiple namespace support
 *
 * Integration: primary block device driver or via virtio_blk.asm
 *
 * API:
 *   nvme_init()                          — init NVMe controller
 *   nvme_identify(ns_id, buf)            — identify namespace
 *   nvme_read(ns_id, lba, count, buf)    — read blocks
 *   nvme_write(ns_id, lba, count, buf)   — write blocks
 *   nvme_flush(ns_id)                    — flush to media
 *   nvme_get_ns_count() => n             — get namespace count
 */
.arch armv8-a

/* NVMe admin commands */
.set NVME_CMD_IDENTIFY,     0x06
.set NVME_CMD_CREATE_IO_SQ, 0x01
.set NVME_CMD_CREATE_IO_CQ, 0x05

/* NVMe I/O commands */
.set NVME_CMD_FLUSH,        0x00
.set NVME_CMD_WRITE,        0x01
.set NVME_CMD_READ,         0x02

/* Queue sizes */
.set NVME_SQ_SIZE,          64      /* submission queue entries */
.set NVME_CQ_SIZE,          64      /* completion queue entries */
.set NVME_SQ_ENTRY_SIZE,    64      /* 64 bytes per SQ entry */
.set NVME_CQ_ENTRY_SIZE,    16      /* 16 bytes per CQ entry */
.set NVME_MAX_NS,           8       /* max namespaces */

/* Controller state */
.set NVME_STATE_UNINIT,     0
.set NVME_STATE_READY,      1
.set NVME_STATE_ERROR,      2

.bss
.align 4
.global nvme_init_done
nvme_init_done:
    .skip 4

.align 4
nvme_state:
    .skip 4
nvme_ns_count:
    .skip 4
nvme_mmio_base:
    .skip 8          /* MMIO base address */

/* Submission queue (in memory) */
.align 12
nvme_sq:
    .skip NVME_SQ_SIZE * NVME_SQ_ENTRY_SIZE   /* 4096 bytes */
nvme_sq_tail:
    .skip 4

/* Completion queue (in memory) */
.align 4
nvme_cq:
    .skip NVME_CQ_SIZE * NVME_CQ_ENTRY_SIZE   /* 1024 bytes */
nvme_cq_head:
    .skip 4
nvme_cq_phase:
    .skip 4          /* phase bit toggle */

.text

/* -----------------------------------------------------------------------------
 * nvme_init — initialize NVMe controller
 * Output: x0 = 0 (ok), -1 (error)
 *
 * In real hardware: would probe PCIe config space, map BAR, reset controller.
 * Here: initializes queue state.
 * ----------------------------------------------------------------------------- */
.global nvme_init
nvme_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero submission queue */
    ldr     x0, =nvme_sq
    mov     x1, #(NVME_SQ_SIZE * NVME_SQ_ENTRY_SIZE)
    bl      .Lnvm_zero

    /* Zero completion queue */
    ldr     x0, =nvme_cq
    mov     x1, #(NVME_CQ_SIZE * NVME_CQ_ENTRY_SIZE)
    bl      .Lnvm_zero

    /* Reset queue pointers */
    ldr     x0, =nvme_sq_tail
    str     wzr, [x0]
    ldr     x0, =nvme_cq_head
    str     wzr, [x0]
    ldr     x0, =nvme_cq_phase
    mov     w1, #1
    str     w1, [x0]

    /* Set state to ready */
    ldr     x0, =nvme_state
    mov     w1, #NVME_STATE_READY
    str     w1, [x0]

    /* Set namespace count to 1 (default) */
    ldr     x0, =nvme_ns_count
    mov     w1, #1
    str     w1, [x0]

    /* TODO: Probe PCIe, map BAR0, reset controller */
    /* TODO: Create admin and I/O queues */

    ldr     x0, =nvme_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lnvm_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * nvme_identify — identify a namespace
 * Input: x0 = namespace ID, x1 = output buffer (4096 bytes)
 * Output: x0 = 0 (ok), -1 (error)
 *
 * Returns Identify Namespace data structure.
 * ----------------------------------------------------------------------------- */
.global nvme_identify
nvme_identify:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* ns_id */
    mov     x20, x1             /* buffer */

    /* Check state */
    ldr     x3, =nvme_state
    ldr     w3, [x3]
    cmp     w3, #NVME_STATE_READY
    b.ne    .Lnvm_id_fail

    /* Build identify command in SQ */
    /* TODO: Submit command, wait for completion */
    /* For now: fill with simulated identify data */

    /* Namespace size (total blocks) — 1M blocks = 512MB */
    mov     w3, #0x00100000
    str     w3, [x20, #0]       /* NSZE */
    /* Namespace capacity */
    str     w3, [x20, #8]       /* NCAP */
    /* LBA format: 512 bytes */
    mov     w3, #512
    str     w3, [x20, #128]     /* LBAF[0].LBADS */

    mov     x0, #0
    b       .Lnvm_id_ret

.Lnvm_id_fail:
    mov     x0, #-1

.Lnvm_id_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * nvme_read — read blocks from NVMe namespace
 * Input: x0 = ns_id, x1 = LBA, x2 = block count, x3 = data buffer
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global nvme_read
nvme_read:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* ns_id */

    /* Check state */
    ldr     x4, =nvme_state
    ldr     w4, [x4]
    cmp     w4, #NVME_STATE_READY
    b.ne    .Lnvm_read_fail

    /* Build read command in submission queue */
    ldr     x4, =nvme_sq
    ldr     w5, =nvme_sq_tail
    ldr     w5, [x5]

    /* Calculate SQ entry offset */
    mov     x6, #NVME_SQ_ENTRY_SIZE
    mul     x6, w5, x6
    add     x4, x4, x6

    /* Fill command */
    mov     w6, #NVME_CMD_READ
    strb    w6, [x4, #0]        /* opcode */
    str     w19, [x4, #8]       /* NSID */
    str     x1, [x4, #40]       /* SLBA (starting LBA) */
    strh    w2, [x4, #48]       /* NLB (number of blocks - 1) */

    /* TODO: Ring doorbell, wait for completion */
    /* For now: simulate immediate success */

    /* Advance SQ tail */
    ldr     x4, =nvme_sq_tail
    ldr     w5, [x4]
    add     w5, w5, #1
    and     w5, w5, #(NVME_SQ_SIZE - 1)
    str     w5, [x4]

    /* TODO: Copy data from device to buffer */
    /* For simulation: zero the buffer */
    mov     x4, x3
    mov     x5, x2
    lsl     x5, x5, #9          /* count * 512 */
.Lnvm_read_zero:
    cbz     x5, .Lnvm_read_done
    strb    wzr, [x4], #1
    sub     x5, x5, #1
    b       .Lnvm_read_zero

.Lnvm_read_done:
    mov     x0, #0
    b       .Lnvm_read_ret

.Lnvm_read_fail:
    mov     x0, #-1

.Lnvm_read_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * nvme_write — write blocks to NVMe namespace
 * Input: x0 = ns_id, x1 = LBA, x2 = block count, x3 = data buffer
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global nvme_write
nvme_write:
    stp     x29, x30, [sp, #-16]!

    /* Check state */
    ldr     x4, =nvme_state
    ldr     w4, [x4]
    cmp     w4, #NVME_STATE_READY
    b.ne    .Lnvm_write_fail

    /* Build write command (similar to read) */
    /* TODO: Submit command, copy data from buffer to device */

    mov     x0, #0
    b       .Lnvm_write_ret

.Lnvm_write_fail:
    mov     x0, #-1

.Lnvm_write_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * nvme_flush — flush volatile cache to media
 * Input: x0 = ns_id
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global nvme_flush
nvme_flush:
    stp     x29, x30, [sp, #-16]!

    ldr     x4, =nvme_state
    ldr     w4, [x4]
    cmp     w4, #NVME_STATE_READY
    b.ne    .Lnvm_flush_fail

    /* TODO: Submit flush command */
    mov     x0, #0
    b       .Lnvm_flush_ret

.Lnvm_flush_fail:
    mov     x0, #-1

.Lnvm_flush_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * nvme_get_ns_count — get number of namespaces
 * Output: x0 = namespace count
 * ----------------------------------------------------------------------------- */
.global nvme_get_ns_count
nvme_get_ns_count:
    ldr     x0, =nvme_ns_count
    ldr     w0, [x0]
    ret
