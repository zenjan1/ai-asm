/*
 * aiasm-aarch64/kernel/memory.asm
 * Physical memory manager: bitmap page frame allocator + buddy heap
 * 4KB pages, bitmap tracks allocated/free frames
 * v0.4: buddy allocator replaces bump allocator
 *
 * Memory map (QEMU virt 128MB):
 *   RAM:     0x40000000 - 0x48000000 (128MB)
 *   Kernel:  0x40080000 - __image_end
 *   Pages:   bitmap-tracked after kernel
 *
 * Public API:
 *   mem_init()                — initialize page bitmap + buddy heap
 *   mem_alloc_page() => x0    — allocate one 4KB page, return physical addr
 *   mem_free_page(x0)         — free a page frame
 *   mem_alloc_aligned(size, align) => x0 — allocate from buddy
 *   mem_free(addr)            — free buddy allocation
 *   mem_total() => w0         — total free pages
 *   mem_used() => w0          — used pages
 *   mem_dump(x0=buffer)       — JSON stats string
 */
.arch armv8-a

/* Page size */
.set PAGE_SIZE,     0x1000
.set PAGE_SHIFT,    12

/* Kernel heap region — managed by buddy allocator */
.global __kernel_heap_start
.global __kernel_heap_end

.text

/* -----------------------------------------------------------------------------
 * Function: mem_init
 * Description: Initialize physical memory manager, build page bitmap
 * Input: none
 * Output: none
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_init
mem_init:
    stp     x29, x30, [sp, #-16]!

    /* Total RAM: 128MB = 0x08000000 bytes */
    mov     x0, #0x08000          /* 128MB in 16-bit chunks */
    lsl     x0, x0, #8            /* = 0x08000000 */

    /* RAM base: 0x40000000 */
    movz    x1, #0x4000, lsl #16
    movk    x1, #0x0000

    /* Pages = total_bytes / PAGE_SIZE */
    lsr     x2, x0, #PAGE_SHIFT   /* 32768 pages */

    /* Store total pages */
    adrp    x3, mem_total_pages
    add     x3, x3, #:lo12:mem_total_pages
    str     w2, [x3]

    /* Kernel end page = (__image_end - RAM_BASE) / PAGE_SIZE */
    adrp    x3, __image_end
    add     x3, x3, #:lo12:__image_end
    sub     x3, x3, x1            /* kernel offset from RAM base */
    lsr     x3, x3, #PAGE_SHIFT   /* kernel page count */
    add     x3, x3, #1            /* round up */

    /* Store used pages */
    adrp    x4, mem_used_pages
    add     x4, x4, #:lo12:mem_used_pages
    str     w3, [x4]

    /* Bitmap: clear all bytes using memset */
    mov     x9, x3              /* save kernel page count */
    adrp    x0, page_bitmap
    add     x0, x0, #:lo12:page_bitmap
    mov     w1, #0                /* value */
    mov     x2, #1024             /* length (1024 bytes = 8192 pages) */
    bl      memset
    mov     x3, x9              /* restore kernel page count */

    /* Mark used pages in bitmap */
    adrp    x4, page_bitmap
    add     x4, x4, #:lo12:page_bitmap
    mov     w5, #1
    mov     x8, x4              /* x8 = bitmap pointer */
mark_loop:
    cbz     x3, mark_done
    strb    w5, [x8], #1          /* mark page as allocated */
    sub     x3, x3, #1
    b       mark_loop

mark_done:
    /* Free pages = total - used */
    adrp    x3, mem_total_pages
    add     x3, x3, #:lo12:mem_total_pages
    ldr     w3, [x3]
    adrp    x4, mem_used_pages
    add     x4, x4, #:lo12:mem_used_pages
    ldr     w4, [x4]
    sub     w3, w3, w4

    adrp    x4, mem_free_pages
    add     x4, x4, #:lo12:mem_free_pages
    str     w3, [x4]

    /* Initialize buddy allocator for kernel heap */
    bl      buddy_init

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_alloc_page
 * Description: Allocate one page frame, return physical address
 * Input: none
 * Output: x0 = physical address of page, or 0 if OOM
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_alloc_page
mem_alloc_page:
    stp     x29, x30, [sp, #-16]!

    /* Scan bitmap for free page */
    adrp    x1, page_bitmap
    add     x1, x1, #:lo12:page_bitmap
    adrp    x2, mem_total_pages
    add     x2, x2, #:lo12:mem_total_pages
    ldr     w2, [x2]

    mov     x0, #0                /* page index */
1:
    cmp     x0, x2
    b.ge    mem_oom
    ldrb    w3, [x1, x0]
    cbz     w3, 2f                /* found free page */
    add     x0, x0, #1
    b       1b

2:
    /* Mark as allocated */
    mov     w3, #1
    strb    w3, [x1, x0]

    /* Update free count */
    adrp    x1, mem_free_pages
    add     x1, x1, #:lo12:mem_free_pages
    ldr     w3, [x1]
    sub     w3, w3, #1
    str     w3, [x1]

    /* Update used count */
    adrp    x1, mem_used_pages
    add     x1, x1, #:lo12:mem_used_pages
    ldr     w3, [x1]
    add     w3, w3, #1
    str     w3, [x1]

    /* Convert page index to physical address */
    lsl     x0, x0, #PAGE_SHIFT   /* page_index * PAGE_SIZE */
    /* Add RAM base */
    movz    x1, #0x4000, lsl #16
    add     x0, x0, x1

    ldp     x29, x30, [sp], #16
    ret

mem_oom:
    mov     x0, #0                /* OOM */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: kernel_alloc_page
 * Description: Allocate one 4KB page from kernel reserved area (C-callable)
 * Input: none
 * Output: x0 = virtual address of page
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global kernel_alloc_page
kernel_alloc_page:
    stp     x29, x30, [sp, #-16]!

    /* Allocate from page bitmap */
    bl      mem_alloc_page
    cbz     x0, kernel_alloc_oom

    /* Convert physical address to virtual (same mapping in QEMU virt) */
    /* Already returns usable address since we map 1:1 */

    ldp     x29, x30, [sp], #16
    ret

kernel_alloc_oom:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_free_page
 * Description: Free a page frame
 * Input: x0 = physical address
 * Output: none
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_free_page
mem_free_page:
    stp     x29, x30, [sp, #-16]!

    /* Convert phys addr to page index */
    movz    x1, #0x4000, lsl #16
    sub     x0, x0, x1
    lsr     x0, x0, #PAGE_SHIFT

    /* Clear bitmap bit */
    adrp    x1, page_bitmap
    add     x1, x1, #:lo12:page_bitmap
    strb    wzr, [x1, x0]

    /* Update counts */
    adrp    x1, mem_free_pages
    add     x1, x1, #:lo12:mem_free_pages
    ldr     w2, [x1]
    add     w2, w2, #1
    str     w2, [x1]

    adrp    x1, mem_used_pages
    add     x1, x1, #:lo12:mem_used_pages
    ldr     w2, [x1]
    sub     w2, w2, #1
    str     w2, [x1]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_alloc_aligned
 * Description: Allocate aligned memory from buddy allocator
 * Input: x0 = size, x1 = alignment (buddy handles alignment internally)
 * Output: x0 = pointer, or 0 if OOM
 * ----------------------------------------------------------------------------- */
.global mem_alloc_aligned
mem_alloc_aligned:
    stp     x29, x30, [sp, #-16]!
    /* Buddy handles alignment; size is in x0 */
    bl      buddy_alloc
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_free
 * Description: Free heap allocation via buddy allocator
 * Input: x0 = pointer
 * ----------------------------------------------------------------------------- */
.global mem_free
mem_free:
    stp     x29, x30, [sp, #-16]!
    bl      buddy_free
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_alloc
 * Description: Simple allocation (16-byte aligned default)
 * Input: x0 = size
 * Output: x0 = pointer, or 0 if OOM
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_alloc
mem_alloc:
    stp     x29, x30, [sp, #-16]!
    mov     x1, #16               /* default alignment */
    bl      mem_alloc_aligned
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_total
 * Description: Get total free page count
 * Input: none
 * Output: w0 = free pages
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_total
mem_total:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, mem_free_pages
    add     x0, x0, #:lo12:mem_free_pages
    ldr     w0, [x0]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_used
 * Description: Get used page count
 * Input: none
 * Output: w0 = used pages
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_used
mem_used:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, mem_used_pages
    add     x0, x0, #:lo12:mem_used_pages
    ldr     w0, [x0]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: mem_dump
 * Description: Get memory info as JSON string in buffer
 * Input: x0 = output buffer
 * Output: x0 = buffer pointer
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global mem_dump
mem_dump:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, msg_mem_info_pre
    add     x1, x1, #:lo12:msg_mem_info_pre
1:
    ldrb    w2, [x1], #1
    cbz     w2, 2f
    strb    w2, [x0], #1
    b       1b

2:
    /* Print total pages */
    adrp    x1, mem_total_pages
    add     x1, x1, #:lo12:mem_total_pages
    ldr     w1, [x1]
    adrp    x3, mem_itoa_buf
    add     x3, x3, #:lo12:mem_itoa_buf
    mov     x0, x1
    mov     x1, x3
    bl      itoa_buf
    mov     x4, x3
1:
    ldrb    w1, [x4], #1
    cbz     w1, 2f
    strb    w1, [x0], #1
    b       1b

2:
    adrp    x1, msg_mem_info_mid
    add     x1, x1, #:lo12:msg_mem_info_mid
3:
    ldrb    w2, [x1], #1
    cbz     w2, 4f
    strb    w2, [x0], #1
    b       3b

4:
    /* Print free pages */
    adrp    x1, mem_free_pages
    add     x1, x1, #:lo12:mem_free_pages
    ldr     w1, [x1]
    adrp    x3, mem_itoa_buf
    add     x3, x3, #:lo12:mem_itoa_buf
    mov     x0, x1
    mov     x1, x3
    bl      itoa_buf
    mov     x4, x3
5:
    ldrb    w1, [x4], #1
    cbz     w1, 6f
    strb    w1, [x0], #1
    b       5b

6:
    adrp    x1, msg_mem_info_suf
    add     x1, x1, #:lo12:msg_mem_info_suf
7:
    ldrb    w2, [x1], #1
    cbz     w2, 8f
    strb    w2, [x0], #1
    b       7b

8:
    strb    wzr, [x0]
    adrp    x0, mem_itoa_buf
    add     x0, x0, #:lo12:mem_itoa_buf
    ldp     x29, x30, [sp], #16
    ret

/* mem_info is an alias for mem_dump (backward compat) */
.global mem_info
mem_info:
    b       mem_dump

/* ----------------------------------------------------------------------------- */
/* Memory state (BSS)                                                           */
/* ----------------------------------------------------------------------------- */
.bss
.align 3
.global mem_total_pages
mem_total_pages:
    .quad 0
.global mem_used_pages
mem_used_pages:
    .quad 0
.global mem_free_pages
mem_free_pages:
    .quad 0

.align 4
page_bitmap:
    .skip 1024                  /* 8192 pages max (32MB) */
.align 4
mem_itoa_buf:
    .skip 24

.section .rodata
.align 4
msg_mem_info_pre:
    .asciz "{\"total_pages\":"
msg_mem_info_mid:
    .asciz ",\"free_pages\":"
msg_mem_info_suf:
    .asciz "}\n"
