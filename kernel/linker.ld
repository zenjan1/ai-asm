/*
 * aiasm-aarch64/kernel/linker.ld
 * AArch64 linker script - QEMU virt board, load at 0x40080000
 */
ENTRY(_start)

SECTIONS
{
    . = 0x40080000;
    .text   : { *(.text) }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    }
    . = ALIGN(16);
    .stack : {
        __stack_bottom = .;
        . = . + 0x2000;
        __stack_top = .;
    }
}
