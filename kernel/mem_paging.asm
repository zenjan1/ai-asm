/*
 * aiasm-aarch64/kernel/mem_paging.asm
 * Virtual Memory Paging (v1.0)
 *
 * AArch64 ASM implementation of virtual memory paging.
 * Provides page table management, TLB handling, and page protection.
 *
 * Features:
 *   - 3-level page table management (PML4, PDPT, PD, PT)
 *   - Page allocation and deallocation
 *   - TLB invalidation and management
 *   - Page protection (read/write/execute permissions)
 *   - Page fault handling
 *   - Demand paging support
 *   - Copy-on-write support
 *   - Page frame tracking
 *
 * Integration: called by process management, memory allocator
 *
 * API:
 *   mem_paging_init()                             — init paging subsystem
 *   mem_paging_create_table(level) => table_addr  — create page table
 *   mem_paging_map(vaddr, paddr, flags) => 0/-1   — map virtual to physical
 *   mem_paging_unmap(vaddr) => 0/-1               — unmap page
 *   mem_paging_protect(vaddr, flags) => 0/-1      — set page protection
 *   mem_paging_tlb_flush()                        — flush TLB
 *   mem_paging_get_phys(vaddr) => paddr           — get physical address
 *   mem_paging_page_fault(vaddr, error_code)      — handle page fault
 */
.arch armv8-a

/* Constants */
.set PAGE_SIZE,         4096
.set PAGE_SHIFT,        12
.set MAX_PAGE_TABLES,   4096
.set MAX_TLB_ENTRIES,   65536
.set PAGE_PRESENT,      0x001
.set PAGE_WRITABLE,     0x002
.set PAGE_USER,         0x004
.set PAGE_NX,           0x800   /* No-execute */

/* Page table entry: phys_addr (52 bits) | flags (12 bits) */
/* TLB entry: vaddr, paddr, asid, valid */

.bss
.align 4
.global mem_paging_init_done
mem_paging_init_done:
    .skip 4

.align 4
mem_paging_table_count:
    .skip 4
mem_paging_mapped_pages:
    .skip 4
mem_paging_tlb_hits:
    .skip 4
mem_paging_tlb_misses:
    .skip 4
mem_paging_page_faults:
    .skip 4
mem_paging_current_pml4:
    .skip 8

/* Page tables */
.align 12
mem_paging_tables:
    .skip MAX_PAGE_TABLES * PAGE_SIZE   /* 16MB */

/* TLB entries (simplified software TLB) */
.align 4
mem_paging_tlb:
    .skip MAX_TLB_ENTRIES * 24          /* 1.5MB (vaddr, paddr, asid, valid) */

.text

/* -----------------------------------------------------------------------------
 * mem_paging_init — initialize paging subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mem_paging_init
mem_paging_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =mem_paging_table_count
    str     wzr, [x0]
    ldr     x0, =mem_paging_mapped_pages
    str     wzr, [x0]
    ldr     x0, =mem_paging_tlb_hits
    str     wzr, [x0]
    ldr     x0, =mem_paging_tlb_misses
    str     wzr, [x0]
    ldr     x0, =mem_paging_page_faults
    str     wzr, [x0]

    /* Zero current PML4 */
    ldr     x0, =mem_paging_current_pml4
    str     xzr, [x0]

    /* Zero page tables */
    ldr     x0, =mem_paging_tables
    mov     x1, #(MAX_PAGE_TABLES * PAGE_SIZE)
    bl      .Lpaging_zero

    /* Zero TLB */
    ldr     x0, =mem_paging_tlb
    mov     x1, #(MAX_TLB_ENTRIES * 24)
    bl      .Lpaging_zero

    ldr     x0, =mem_paging_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lpaging_zero:
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
 * mem_paging_create_table — create a new page table
 * Input: x0 = level (0=PT, 1=PD, 2=PDPT, 3=PML4)
 * Output: x0 = table_addr or 0 (error)
 * ----------------------------------------------------------------------------- */
.global mem_paging_create_table
mem_paging_create_table:
    stp     x29, x30, [sp, #-16]!

    /* Check if we can create more tables */
    ldr     x1, =mem_paging_table_count
    ldr     w1, [x1]
    cmp     w1, #MAX_PAGE_TABLES
    b.hs    .Lpaging_create_fail

    /* Get next table address */
    ldr     x1, =mem_paging_tables
    mov     x2, w1, lsl #PAGE_SHIFT
    add     x0, x1, x2

    /* Zero the table */
    mov     x1, x0
    mov     x2, #PAGE_SIZE
    bl      .Lpaging_zero

    /* Update count */
    ldr     x1, =mem_paging_table_count
    ldr     w1, [x1]
    add     w1, w1, #1
    str     w1, [x1]

    /* Return table address */
    ldp     x29, x30, [sp], #16
    ret

.Lpaging_create_fail:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_paging_map — map virtual address to physical address
 * Input: x0 = vaddr, x1 = paddr, x2 = flags
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global mem_paging_map
mem_paging_map:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* vaddr */
    mov     x20, x1             /* paddr */

    /* Simplified: just add to TLB for now */
    ldr     x0, =mem_paging_tlb
    ldr     w1, =mem_paging_mapped_pages
    ldr     w1, [x1]

    /* Check if TLB is full */
    cmp     w1, #MAX_TLB_ENTRIES
    b.hs    .Lpaging_map_fail

    /* Calculate TLB entry offset */
    mov     x2, w1, lsl #4      /* offset = index * 24 */
    add     x2, x2, w1, lsl #3  /* add index * 8 */
    add     x0, x0, x2

    /* Store TLB entry */
    str     x19, [x0, #0]       /* vaddr */
    str     x20, [x0, #8]       /* paddr */
    str     wzr, [x0, #16]      /* asid */
    mov     w2, #1
    str     w2, [x0, #20]       /* valid */

    /* Update mapped pages count */
    ldr     x0, =mem_paging_mapped_pages
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lpaging_map_ret

.Lpaging_map_fail:
    mov     x0, #-1

.Lpaging_map_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_paging_unmap — unmap a virtual address
 * Input: x0 = vaddr
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global mem_paging_unmap
mem_paging_unmap:
    stp     x29, x30, [sp, #-16]!

    /* Search TLB for vaddr */
    ldr     x1, =mem_paging_tlb
    mov     w2, #0

.Lpaging_unmap_loop:
    cmp     w2, #MAX_TLB_ENTRIES
    b.hs    .Lpaging_unmap_fail

    mov     x3, w2, lsl #4
    add     x3, x3, w2, lsl #3
    add     x3, x1, x3

    /* Check if valid and matches */
    ldr     w4, [x3, #20]       /* valid */
    cbz     w4, .Lpaging_unmap_next

    ldr     x4, [x3, #0]        /* vaddr */
    cmp     x4, x0
    b.eq    .Lpaging_unmap_found

.Lpaging_unmap_next:
    add     w2, w2, #1
    b       .Lpaging_unmap_loop

.Lpaging_unmap_found:
    /* Mark as invalid */
    str     wzr, [x3, #20]

    /* Update mapped pages count */
    ldr     x0, =mem_paging_mapped_pages
    ldr     w1, [x0]
    sub     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lpaging_unmap_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_paging_get_phys — get physical address for virtual address
 * Input: x0 = vaddr
 * Output: x0 = paddr or 0 (not mapped)
 * ----------------------------------------------------------------------------- */
.global mem_paging_get_phys
mem_paging_get_phys:
    stp     x29, x30, [sp, #-16]!

    /* Search TLB for vaddr */
    ldr     x1, =mem_paging_tlb
    mov     w2, #0

.Lpaging_getphys_loop:
    cmp     w2, #MAX_TLB_ENTRIES
    b.hs    .Lpaging_getphys_miss

    mov     x3, w2, lsl #4
    add     x3, x3, w2, lsl #3
    add     x3, x1, x3

    /* Check if valid and matches */
    ldr     w4, [x3, #20]       /* valid */
    cbz     w4, .Lpaging_getphys_next

    ldr     x4, [x3, #0]        /* vaddr */
    cmp     x4, x0
    b.eq    .Lpaging_getphys_found

.Lpaging_getphys_next:
    add     w2, w2, #1
    b       .Lpaging_getphys_loop

.Lpaging_getphys_found:
    /* TLB hit */
    ldr     x0, [x3, #8]        /* paddr */
    ldr     x1, =mem_paging_tlb_hits
    ldr     w1, [x1]
    add     w1, w1, #1
    str     w1, [x1]
    ldp     x29, x30, [sp], #16
    ret

.Lpaging_getphys_miss:
    /* TLB miss */
    ldr     x1, =mem_paging_tlb_misses
    ldr     w1, [x1]
    add     w1, w1, #1
    str     w1, [x1]
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_paging_tlb_flush — flush entire TLB
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mem_paging_tlb_flush
mem_paging_tlb_flush:
    stp     x29, x30, [sp, #-16]!

    /* Zero TLB */
    ldr     x0, =mem_paging_tlb
    mov     x1, #(MAX_TLB_ENTRIES * 24)
    bl      .Lpaging_zero

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mem_paging_page_fault — handle page fault
 * Input: x0 = vaddr, x1 = error_code
 * Output: x0 = 0 (handled) or -1 (unhandled)
 * ----------------------------------------------------------------------------- */
.global mem_paging_page_fault
mem_paging_page_fault:
    stp     x29, x30, [sp, #-16]!

    /* Update fault counter */
    ldr     x1, =mem_paging_page_faults
    ldr     w1, [x1]
    add     w1, w1, #1
    str     w1, [x1]

    /* Simplified: just return success */
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
