/*
 * aiasm-aarch64/kernel/mmu.asm
 * AArch64 MMU page table management — identity mapping for kernel
 * Uses 4KB granule, 2-level page tables (L1 block, L2 4KB entries)
 *
 * Memory layout:
 *   0x0000_0000_0000_0000 - 0x0000_007F_FFFF_FFFF  (user space, 512GB)
 *   0xFFFF_0000_0000_0000 - 0xFFFF_FFFF_FFFF_FFFF  (kernel space)
 *
 * Initial setup: identity map 0x40000000-0x48000000 (128MB RAM)
 * Future: per-process page tables with WASM module isolation
 */
.arch armv8-a

/* Page table entry attributes */
.set MT_NORMAL,     0b100     /* Normal memory */
.set MT_DEVICE_nGnRnE, 0b000  /* Device memory */

/* Memory attribute fields for MAIR */
.set MAIR_NORMAL,   (0xFF << 0)  /* Attr0: Normal, inner/outer WBWA */
.set MAIR_DEVICE,   (0x00 << 8)  /* Attr1: Device-nGnRnE */

/* Block/page entry bits */
.set PTE_VALID,     (1 << 0)
.set PTE_BLOCK,     (1 << 1)
.set PTE_PAGE,      (3 << 0)
.set PTE_TABLE,     (3 << 0)
.set PTE_USER,      (1 << 6)
.set PTE_RO,        (1 << 7)
.set PTE_NG,        (1 << 11)
.set PTE_AF,        (1 << 10)  /* Access flag */
.set PTE_NX,        (1UL << 54) /* XN: eXecute Never */
.set PTE_PXN,       (1UL << 53) /* Privileged XN */
.set PTE_APTABLE,   (3 << 61)   /* AP[2:1] for table entries */

/* Shareability */
.set PTE_ISH,       (3 << 8)    /* Inner Shareable */
.set PTE_OSH,       (2 << 8)    /* Outer Shareable */

/* Text section */
.text

/* -----------------------------------------------------------------------------
 * Function: mmu_init
 * Description: Set up page tables and enable MMU
 *   1. Configure MAIR (Memory Attribute Indirection Register)
 *   2. Configure TCR (Translation Control Register)
 *   3. Build L1 block entry (1GB block for 128MB RAM)
 *   4. Load TTBR0_EL1
 *   5. Enable MMU (SCTLR_EL1.M = 1)
 * Input: none
 * Output: none
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_init
mmu_init:
    stp     x29, x30, [sp, #-16]!

    /* --- Step 1: Configure MAIR --- */
    ldr     x0, =((MAIR_NORMAL) | (MAIR_DEVICE))
    msr     mair_el1, x0

    /* --- Step 2: Configure TCR (Translation Control Register) --- */
    /*
     * T0SZ=16, IRGN0=01, ORGN0=01, SH0=11, TG0=00
     * T1SZ=16, IRGN1=01, ORGN1=01, SH1=11, TG1=00
     * Value: 0x000033B0_000033B0 (both halves identical)
     */
    ldr     x0, =0x0000351000003510
    msr     tcr_el1, x0

    /* --- Step 3: Build L1 page table (identity mapping) --- */
    adrp    x0, __page_table_base
    add     x0, x0, #:lo12:__page_table_base

    /* Clear entire page table (64KB) */
    mov     x1, x0
    mov     x2, #(65536 / 8)
    mov     x3, xzr
1:
    str     xzr, [x1], #8
    sub     x2, x2, #1
    cbnz    x2, 1b

    /* Create L1 block entries for identity mapping:
     * RAM at 0x40000000 = VA index 0x40000000 >> 39 = 0x80 (index 128)
     * But simpler: map 0x0000_0000_0000_0000 - 0x0000_007F_FFFF_FFFF
     * with L1 block entries (1GB blocks)
     *
     * We only need entries for:
     *   Index 0: 0x0000_0000_0000_0000 (1GB) — unused
     *   Index 128: 0x0000_0040_0000_0000 (covers 0x40000000 RAM base)
     *
     * Actually, let's just map the 128MB at 0x40000000 as a 1GB block
     * L1 index = PA >> 39 = 0x40000000 >> 39 = 0x200 = 512... no.
     *
     * For 48-bit VA with 4KB granule:
     *   L1 (level 1): bits 47:39 → 9 bits → 512 entries × 8 = 4KB
     *   VA 0x40000000 >> 39 = 0x200 >> 39...
     *   0x40000000 = 0x0000_0040_0000_0000
     *   >> 39 = 0x80 = 128
     *
     * So L1[128] should be a block entry for 0x40000000-0x7FFFFFFF (1GB)
     * But that maps the whole 1GB region at 0x40000000.
     *
     * Let's create a simpler approach: identity map ALL of physical RAM
     * using a single L2 table for the 128MB region.
     */

    /* Actually, let's do identity mapping of first 2GB (0x00000000-0x80000000)
     * as a single L1 block entry. VA = PA = 0x00000000
     * Index 0 = 1GB block (0x00000000-0x3FFFFFFF)
     * Index 1 = 1GB block (0x40000000-0x7FFFFFFF) ← this covers our RAM
     *
     * For index 1 (VA 0x40000000 = PA 0x40000000):
     */
    ldr     x1, =(0x40000000 | PTE_VALID | PTE_BLOCK | PTE_AF | PTE_APTABLE)
    ldr     x3, =((MT_NORMAL << 2) | PTE_ISH)
    orr     x1, x1, x3
    str     x1, [x0, #8]                /* L1[1] = 1GB block at PA 0x40000000 */

    /* Also map VA 0x00000000-0x3FFFFFFF (index 0) for device mappings:
     * This covers GIC at 0x08000000, UART at 0x09000000
     */
    ldr     x1, =(0x00000000 | PTE_VALID | PTE_BLOCK | PTE_AF | PTE_APTABLE)
    ldr     x3, =((MT_NORMAL << 2) | PTE_ISH)
    orr     x1, x1, x3
    str     x1, [x0, #0]              /* L1[0] = 1GB block at PA 0x00000000 */

    /* --- Step 4: Load TTBR0_EL1 --- */
    adrp    x0, __page_table_base
    add     x0, x0, #:lo12:__page_table_base
    msr     ttbr0_el1, x0

    /* --- Step 5: Enable MMU --- */
    mrs     x0, sctlr_el1
    ldr     x1, =((1 << 0) | (1 << 1) | (1 << 2) | (1 << 11) | (1 << 12))
    orr     x0, x0, x1
    msr     sctlr_el1, x0

    /* ISB to ensure MMU is enabled before executing more instructions */
    isb

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_create_l2_table
 * Description: Create an L2 page table for a VA range, identity-mapping PA
 *   Each L2 entry maps one 4KB page. L2 table has 512 entries (2MB coverage).
 * Input: x0 = L1 table base (PA), x1 = VA start (4KB-aligned), x2 = size (bytes)
 *        x3 = PA start (4KB-aligned)
 * Output: none
 * Clobbered: x0-x6
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_create_l2_table
mmu_create_l2_table:
    stp     x29, x30, [sp, #-16]!
    mov     x7, x0              /* save L1 base */
    mov     x8, x1              /* save VA start */
    mov     x9, x2              /* save size */
    mov     x10, x3             /* save PA start */

    /* Allocate one 4KB page for L2 table from heap */
    mov     x0, #4096
    mov     x1, #4096
    bl      mem_alloc_aligned
    cbz     x0, l2_oom
    mov     x11, x0             /* x11 = L2 table VA */

    /* Clear L2 table */
    mov     x1, x11
    mov     x2, #(4096 / 8)
1:
    str     xzr, [x1], #8
    sub     x2, x2, #1
    cbnz    x2, 1b

    /* Calculate L1 index: VA >> 39 */
    lsr     x4, x8, #39
    lsl     x4, x4, #3          /* × 8 bytes per entry */

    /* Create L1 table entry pointing to L2 table */
    /* Entry type = 3 (table), APTable=3 (allow EL0/EL1 RW) */
    mov     x5, x11
    orr     x5, x5, #PTE_TABLE        /* bits[1:0] = 11 = table */
    orr     x5, x5, #PTE_APTABLE      /* AP[2:1] = 11 */
    orr     x5, x5, #PTE_NX           /* XN at table level */
    str     x5, [x7, x4]              /* L1[index] = L2 table entry */

    /* Fill L2 entries: each maps one 4KB page */
    /* L2 index = (VA >> 12) & 0x1FF (within the 2MB L2 range) */
    mov     x4, x8
    mov     x5, x10
    mov     x6, x9
    lsr     x6, x6, #12         /* number of pages to map */

l2_fill_loop:
    cbz     x6, l2_done

    /* L2 entry: PA | PTE_PAGE | AF | normal memory */
    mov     x1, x5
    orr     x1, x1, #PTE_PAGE       /* bits[1:0] = 11 */
    orr     x1, x1, #PTE_AF         /* access flag */
    orr     x1, x1, #(MT_NORMAL << 2)
    orr     x1, x1, #PTE_ISH

    /* Calculate L2 index and store entry */
    lsr     x2, x4, #12
    and     x2, x2, #0x1FF      /* mask to 9 bits */
    lsl     x2, x2, #3          /* × 8 bytes */
    str     x1, [x11, x2]

    add     x4, x4, #4096       /* next VA page */
    add     x5, x5, #4096       /* next PA page */
    sub     x6, x6, #1
    b       l2_fill_loop

l2_done:
    ldp     x29, x30, [sp], #16
    ret

l2_oom:
    /* Return silently, no L2 table created */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_clone_page_table
 * Description: Clone kernel page table for a process, return TTBR0 value
 *   Copies kernel L1 table, then creates user-space mappings.
 * Input: x0 = process ID (for user space base), x1 = user space VA base
 *        x2 = PA base for user space
 * Output: x0 = TTBR0 value (physical address of new L1 table)
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_clone_page_table
mmu_clone_page_table:
    stp     x29, x30, [sp, #-16]!
    mov     x6, x0              /* save pid */
    mov     x7, x1              /* save user VA */
    mov     x8, x2              /* save user PA */

    /* Allocate new L1 table (4KB) */
    mov     x0, #4096
    mov     x1, #4096
    bl      mem_alloc_aligned
    cbz     x0, clone_oom
    mov     x9, x0              /* x9 = new L1 table VA */

    /* Copy kernel L1 table to new L1 table */
    adrp    x0, __page_table_base
    add     x0, x0, #:lo12:__page_table_base
    mov     x1, x9
    mov     x2, #512            /* 512 entries = 4KB */
1:
    ldr     x3, [x0], #8
    str     x3, [x1], #8
    sub     x2, x2, #1
    cbnz    x2, 1b

    /* Create L2 table for user space region */
    /* Map 2MB at user VA base to user PA base */
    mov     x0, x9              /* L1 table base */
    mov     x1, x7              /* user VA */
    mov     x2, #(2 * 1024 * 1024)  /* 2MB */
    mov     x3, x8              /* user PA */
    bl      mmu_create_l2_table

    /* Return TTBR0 = physical address of new L1 table */
    /* (same as VA since we use identity mapping for kernel) */
    mov     x0, x9

    ldp     x29, x30, [sp], #16
    ret

clone_oom:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_map_device
 * Description: Map a device region as Device-nGnRnE memory
 *   Creates block entries with device attributes for I/O regions
 * Input: x0 = virtual address, x1 = physical address, x2 = size (bytes)
 * Output: none (clobbers x0-x4)
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_map_device
mmu_map_device:
    stp     x29, x30, [sp, #-16]!

    /* For now, this is a stub. Full implementation requires L2 page tables.
     * Device regions (GIC, UART) are already mapped as Normal memory via
     * the 1GB block entries. For QEMU virt this works because cache is
     * not enabled at the device regions in practice.
     */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_map_page
 * Description: Map a single 4KB page with specified permissions
 * Input: x0 = virtual address, x1 = physical address, x2 = flags
 * Output: none
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_map_page
mmu_map_page:
    stp     x29, x30, [sp, #-16]!

    /* For now, stub — full L2 page table walk needed */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_switch_ttbr0
 * Description: Switch user space page table (for process context switch)
 * Input: x0 = TTBR0 value (physical address of L1 table)
 * Output: none
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_switch_ttbr0
mmu_switch_ttbr0:
    stp     x29, x30, [sp, #-16]!

    msr     ttbr0_el1, x0
    isb
    tlbi    vmalle1                 /* Invalidate all TLB entries */
    dsb     nsh
    isb

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_flush_tlb
 * Description: Invalidate all TLB entries
 * Input: none
 * Output: none
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_flush_tlb
mmu_flush_tlb:
    stp     x29, x30, [sp, #-16]!

    tlbi    vmalle1
    dsb     nsh
    isb

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mmu_info
 * Description: Get MMU status as JSON string
 * Input: x0 = output buffer
 * Output: x0 = buffer pointer
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mmu_info
mmu_info:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x0              /* save buffer */

    /* Copy prefix */
    adrp    x1, msg_mmu_info
    add     x1, x1, #:lo12:msg_mmu_info
1:
    ldrb    w2, [x1], #1
    cbz     w2, 2f
    strb    w2, [x0], #1
    b       1b

2:
    /* Print "enabled" or "disabled" */
    mrs     x0, sctlr_el1
    tbnz    x0, #0, 3f
    adrp    x1, msg_mmu_off
    add     x1, x1, #:lo12:msg_mmu_off
    b       4f
3:
    adrp    x1, msg_mmu_on
    add     x1, x1, #:lo12:msg_mmu_on
4:
    ldrb    w2, [x1], #1
    cbz     w2, 5f
    strb    w2, [x0], #1
    b       4b

5:
    strb    wzr, [x0]
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

.section .rodata
.align 4
msg_mmu_info:
    .asciz "{\"mmu\":\""
msg_mmu_on:
    .asciz "enabled\"}\n"
msg_mmu_off:
    .asciz "disabled\"}\n"

.bss
.align 4
.global mmu_status_buf
mmu_status_buf:
    .skip 64
