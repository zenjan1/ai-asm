/*
 * aiasm-aarch64/kernel/linker.ld
 * AArch64 linker script - QEMU virt board, load at 0x40080000
 * v0.3: adds WASM module section and larger kernel heap
 */
ENTRY(_start)

SECTIONS
{
    . = 0x40080000;
    .text : {
        *(.text)
        *(.text.*)
    }
    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.rodata.wasm)
        *(.data)
        *(.data.*)
        *(.data.rel.ro)
        *(.data.rel.ro.*)
        *(.got)
        *(.got.*)
    }
    .bss    : {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    }
    /* Kernel image ends here (before page tables) */
    . = ALIGN(4096);
    __image_end = .;

    /* Reserve space for page tables (64KB aligned) */
    . = ALIGN(65536);
    __page_table_base = .;
    . = . + 0x4000;

    /* Kernel stack (16KB for wasm3 runtime) */
    . = ALIGN(16);
    .stack : {
        __stack_bottom = .;
        . = . + 0x4000;
        __stack_top = .;
    }
    PROVIDE(__image_end = __image_end);
    PROVIDE(__page_table_base = __page_table_base);
}
