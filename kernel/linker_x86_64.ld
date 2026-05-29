/* Linker script for x86_64 kernel */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(x86_64_boot)

SECTIONS {
    . = 0xffffffff80000000;

    .multiboot : ALIGN(4K) {
        *(.multiboot)
    }

    .text : ALIGN(4K) {
        *(.text)
        *(.text.*)
    }

    .rodata : ALIGN(4K) {
        *(.rodata)
        *(.rodata.*)
    }

    .data : ALIGN(4K) {
        *(.data)
        *(.data.*)
    }

    .bss : ALIGN(4K) {
        bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        bss_end = .;
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note)
        *(.comment)
    }
}
