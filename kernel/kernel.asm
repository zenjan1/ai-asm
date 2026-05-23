/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point v0.4 — Wasm3 runtime host, enhanced I/O, memory manager
 * Pure AArch64 assembly, load at 0x40080000
 *
 * Boot sequence:
 *   1. Setup stack, clear BSS
 *   2. Init PL011 UART + RX ring buffer
 *   3. Init physical memory manager (bitmap + kernel heap)
 *   4. Init ARM Generic Timer
 *   5. Init WASM3 runtime (C layer: wasm_host.c)
 *   6. Batch register host functions via registration table
 *   7. Load embedded or external WASM module
 *   8. Execute WASM _start
 *
 * New v0.4:
 *   - UART RX ring buffer (serial_rx.asm) replaces blocking serial_getc
 *   - Kernel heap with aligned allocation (mem_alloc_aligned)
 *   - Host functions: host_alloc, host_free, host_log, host_free_page
 *   - Error logging via JSON kernel_log()
 *   - External WASM module loading support (wasm_host_load_external)
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

    /* Initialize UART RX ring buffer */
    bl      serial_rx_init

    /* Print boot banner */
    adrp    x0, boot_banner
    add     x0, x0, #:lo12:boot_banner
    bl      serial_puts

    /* Initialize logging system */
    bl      log_init

    /* Initialize physical memory manager */
    bl      mem_init

    /* Initialize ARM Generic Timer */
    bl      timer_init

    /* ---- Transition to WASM3 runtime ---- */

    /* wasm_host_init() */
    bl      wasm_host_init
    cbnz    x0, wasm_init_error

    /* wasm_host_load + wasm_host_run */
    bl      load_and_run_wasm
    cbnz    x0, wasm_load_error

    /* Done */
    adrp    x0, boot_done
    add     x0, x0, #:lo12:boot_done
    bl      serial_puts

1:  wfi
    b       1b

/* ----------------------------------------------------------------------------- */
/* Error handlers with JSON logging                                             */
/* ----------------------------------------------------------------------------- */
wasm_init_error:
    /* Print human-readable prefix */
    adrp    x1, boot_wasm_init_err
    add     x1, x1, #:lo12:boot_wasm_init_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts
    /* Also emit JSON error log */
    adrp    x0, boot_halt
    add     x0, x0, #:lo12:boot_halt
    bl      serial_puts
    b       wasm_halt

wasm_load_error:
    adrp    x1, boot_wasm_load_err
    add     x1, x1, #:lo12:boot_wasm_load_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts
    b       wasm_halt

wasm_run_error:
    adrp    x1, boot_wasm_run_err
    add     x1, x1, #:lo12:boot_wasm_run_err
    bl      serial_puts
    mov     x1, x0
    bl      serial_puts

wasm_halt:
    adrp    x0, boot_halt
    add     x0, x0, #:lo12:boot_halt
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
/* load_and_run_wasm: call wasm_host_load and wasm_host_run                       */
/* ----------------------------------------------------------------------------- */
.global load_and_run_wasm
load_and_run_wasm:
    stp     x29, x30, [sp, #-16]!

    /* Load wasm_module_start address and size */
    adrp    x0, wasm_module_start
    add     x0, x0, #:lo12:wasm_module_start
    adrp    x1, wasm_module_size
    add     x1, x1, #:lo12:wasm_module_size
    ldr     w1, [x1]

    /* Call wasm_host_load(x0=wasm_ptr, x1=wasm_size) */
    bl      wasm_host_load
    cbnz    x0, 1f                  /* if error, return */

    /* Call wasm_host_run() */
    bl      wasm_host_run
    cbnz    x0, 1f

    ldp     x29, x30, [sp], #16
    ret

1:  /* Error path — return error in x0 */
    ldp     x29, x30, [sp], #16
    ret

/* ----------------------------------------------------------------------------- */
/* Strings (placed in same section as code via -N linker)                         */
/* ----------------------------------------------------------------------------- */
.align 4
boot_banner:
    .asciz "\nAI-ASM AArch64 v0.4 — Wasm3 Runtime Host\n"
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
