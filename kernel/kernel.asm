/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point v0.3 — Wasm3 runtime host
 * Pure AArch64 assembly, load at 0x40080000
 *
 * Boot sequence:
 *   1. Setup stack, clear BSS
 *   2. Init UART, memory manager
 *   3. Init WASM3 runtime (C layer: wasm_host.c)
 *   4. Load embedded Shell WASM module
 *   5. Execute WASM _start
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: _start
 * ----------------------------------------------------------------------------- */
.global _start
_start:
    /* Mask all exceptions */
    msr     daifset, #0xf

    /* Setup stack pointer (16KB, 16-byte aligned) */
    adrp    x0, __stack_top
    add     x0, x0, #:lo12:__stack_top
    mov     sp, x0

    /* Clear BSS */
    bl      bss_clear

    /* Initialize PL011 UART */
    bl      serial_init

    /* Print boot banner using PC-relative adr */
    adr     x0, boot_banner
    bl      serial_puts

    /* Initialize physical memory manager */
    bl      mem_init

    /* Initialize ARM Generic Timer */
    bl      timer_init

    /* ---- Transition to WASM3 runtime ---- */

    /* wasm_host_init() */
    bl      wasm_host_init
    cbnz    x0, wasm_init_error

    /* wasm_host_load(&wasm_module_start, wasm_module_size) */
    adr     x0, wasm_module_start
    adr     x1, wasm_module_size
    ldr     w1, [x1]
    bl      wasm_host_load
    cbnz    x0, wasm_load_error

    /* wasm_host_run() */
    bl      wasm_host_run
    cbnz    x0, wasm_run_error

    /* Done */
    adr     x0, boot_done
    bl      serial_puts

1:  wfi
    b       1b

/* ----------------------------------------------------------------------------- */
/* Error handlers                                                                 */
/* ----------------------------------------------------------------------------- */
wasm_init_error:
    adr     x1, boot_wasm_init_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts
    b       wasm_halt

wasm_load_error:
    adr     x1, boot_wasm_load_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts
    b       wasm_halt

wasm_run_error:
    adr     x1, boot_wasm_run_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts

wasm_halt:
    adr     x0, boot_halt
    bl      serial_puts
1:  wfi
    b       1b

/* ----------------------------------------------------------------------------- */
/* bss_clear                                                                     */
/* ----------------------------------------------------------------------------- */
bss_clear:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, __bss_start
    add     x0, x0, #:lo12:__bss_start
    adrp    x1, __bss_end
    add     x1, x1, #:lo12:__bss_end
1:
    cmp     x0, x1
    b.ge    2f
    str     xzr, [x0], #8
    b       1b
2:
    ldp     x29, x30, [sp], #16
    ret

/* ----------------------------------------------------------------------------- */
/* Strings (placed in same section as code via -N linker)                         */
/* ----------------------------------------------------------------------------- */
.align 4
boot_banner:
    .asciz "\nAI-ASM AArch64 v0.3 - Wasm3 Runtime Host\n"
boot_done:
    .asciz "WASM returned.\n"
boot_halt:
    .asciz "System halted.\n"
boot_wasm_init_err:
    .asciz "WASM init error: "
boot_wasm_load_err:
    .asciz "WASM load error: "
boot_wasm_run_err:
    .asciz "WASM run error: "
