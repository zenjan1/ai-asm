/* x86_64 boot code with Multiboot2 support */

#define MB2_MAGIC 0xe85250d6
#define MB2_ARCHITECTURE 0
#define MB2_HEADER_LENGTH 0x20
#define MB2_CHECKSUM (-(MB2_MAGIC + MB2_ARCHITECTURE + MB2_HEADER_LENGTH))

/* Multiboot2 header */
.section .multiboot
.align 8
mb2_header:
    .long MB2_MAGIC
    .long MB2_ARCHITECTURE
    .long MB2_HEADER_LENGTH
    .long MB2_CHECKSUM

    /* End tag */
    .short 0
    .short 0
    .long 8

/* Entry point */
.section .text
.global x86_64_boot
x86_64_boot:
    cli

    /* Check Multiboot magic */
    cmp $0x36d76289, %eax
    jne hang

    /* Save Multiboot info pointer */
    mov %rbx, mb2_info_ptr
    mov %eax, mb2_magic

    /* Load GDT */
    lgdt gdt64_desc

    /* Set up segment registers */
    mov $0x08, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    /* Enable PAE and PGE */
    mov %cr4, %rax
    or $0x20, %rax    /* PAE */
    or $0x80, %rax    /* PGE */
    mov %rax, %cr4

    /* Load page tables */
    lea p4_table(%rip), %rax
    mov %rax, %cr3

    /* Enable long mode */
    mov $0xc0000080, %rcx
    rdmsr
    or $0x100, %rax    /* LME */
    wrmsr

    /* Enable paging */
    mov %cr0, %rax
    mov $0x80000000, %rcx
    or %rcx, %rax
    mov %rax, %cr0

    /* Jump to 64-bit code */
    lea long_mode_entry, %rax
    push $0x08
    push %rax
    lretq

hang:
    hlt
    jmp hang

/* GDT */
.align 8
gdt64:
    .quad 0                    /* NULL descriptor */
    .quad 0x0020980000000000   /* 64-bit code segment */
    .quad 0x0000900000000000   /* 64-bit data segment */
gdt64_desc:
    .short gdt64_desc - gdt64 - 1
    .quad gdt64

/* Page tables (identity mapping for first 2MB) */
.section .bss
.align 4096
p4_table:
    .space 4096
p3_table:
    .space 4096
p2_table:
    .space 4096

/* Long mode entry */
.section .text
.global long_mode_entry
long_mode_entry:
    /* Set up stack */
    lea stack_top, %rsp

    /* Clear BSS */
    lea bss_start, %rdi
    lea bss_end, %rcx
    sub %rdi, %rcx
    xor %eax, %eax
    shr $3, %rcx
    rep stosq

    /* Call kernel main */
    call kernel_main

    /* Halt if kernel returns */
    cli
hang_long:
    hlt
    jmp hang_long

/* Stack */
.section .bss
.align 16
stack:
    .space 65536
stack_top:

/* Stub kernel_main */
.section .text
.global kernel_main
kernel_main:
    ret

/* Data section */
.section .data
mb2_info_ptr:
    .quad 0
mb2_magic:
    .long 0
