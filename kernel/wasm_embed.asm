/*
 * aiasm-aarch64/kernel/wasm_embed.asm
 * Embeds shell.wasm bytecode into kernel binary.
 * Provides: wasm_module_start, wasm_module_end, wasm_module_size
 */
.arch armv8-a

.section .rodata.wasm
.global wasm_module_start
wasm_module_start:
    .incbin "kernel/shell.wasm"
.global wasm_module_end
wasm_module_end:
.align 4
.global wasm_module_size
wasm_module_size:
    .int wasm_module_end - wasm_module_start
