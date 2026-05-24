/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point v0.5 — Multi-Module WASM Runtime + RAM disk
 * Pure AArch64 assembly, load at 0x40080000
 *
 * Boot sequence:
 *   1. Setup stack, clear BSS
 *   2. Init PL011 UART + RX ring buffer
 *   3. Init physical memory manager (bitmap + kernel heap)
 *   4. Init ARM Generic Timer
 *   5. Init multi-module WASM runtime (wasm_host_init_multi)
 *   6. Load and run init module
 *   7. After init exits, run ready modules (spawned by init)
 *
 * v0.5 changes:
 *   - Multi-module WASM runtime with shared environment
 *   - Init module spawns other modules (e.g., shell)
 *   - RAM disk filesystem (embedded TAR)
 *   - Module lifecycle: FREE -> LOADING -> READY -> RUNNING -> EXITED
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

    /* Initialize MMU (identity mapping, enables caches) */
    bl      mmu_init

    /* Log MMU status */
    adrp    x0, mmu_status_buf
    add     x0, x0, #:lo12:mmu_status_buf
    bl      mmu_info
    bl      serial_puts

    /* Initialize VirtIO devices */
    bl      virtio_init

    /* Initialize VirtIO-Block driver */
    bl      virtio_blk_init

    /* Initialize VirtIO-Net driver */
    bl      virtio_net_init

    /* Initialize FAT32 filesystem */
    bl      fs_init

    /* Initialize TCP/IP network stack */
    bl      net_init

    /* Initialize WASI subsystem */
    bl      wasi_init

    /* Initialize module repository */
    bl      module_repo_init

    /* Initialize IPC subsystems */
    bl      ipc_init
    bl      msgq_init
    bl      signal_init

    /* Initialize JIT cache for WASM pre-compilation */
    bl      jit_cache_init

    /* Initialize buddy allocator for kernel heap */
    bl      buddy_init

    /* Initialize framebuffer and GUI */
    bl      fb_init
    bl      gui_init

    /* Initialize ARM Generic Timer */
    bl      timer_init

    /* Initialize GICv2 interrupt controller */
    bl      gic_init
    bl      gic_enable_timer_irq
    bl      virtio_irq_init

    /* Set exception vector table (2KB aligned) */
    adrp    x0, exception_vectors
    add     x0, x0, #:lo12:exception_vectors
    msr     vbar_el1, x0

    /* Set 10ms periodic timer interval */
    mov     x0, #10
    bl      timer_set_interval

    /* Unmask CPU interrupts (clears DAIF I bit) */
    bl      gic_unmask_all

    /* Initialize user management */
    bl      user_init

    /* Initialize device management */
    bl      device_init

    /* Initialize process memory isolation */
    bl      proc_mem_init

    /* Initialize permission system */
    bl      perm_init

    /* ---- Transition to WASM3 runtime (v0.5 multi-module) ---- */

    /* wasm_host_init_multi() */
    bl      wasm_host_init_multi
    cbnz    x0, wasm_init_error

    /* Load and run init module */
    bl      load_and_run_init
    cbnz    x0, wasm_load_error

    /* Run any ready modules (spawned by init) */
    bl      run_ready_modules

    /* Enter preemptive scheduler loop (IRQ-driven at 10ms) */
    bl      proc_init
    b       proc_schedule_loop

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
/* load_and_run_init: load init module, run it, return error or 0                 */
/* ----------------------------------------------------------------------------- */
.global load_and_run_init
load_and_run_init:
    stp     x29, x30, [sp, #-16]!

    /* Load init_module_start address and size */
    adrp    x0, init_module_start
    add     x0, x0, #:lo12:init_module_start
    adrp    x1, init_module_size
    add     x1, x1, #:lo12:init_module_size
    ldr     w1, [x1]

    /* Call wasm_host_load(x0=wasm_ptr, x1=wasm_size) */
    bl      wasm_host_load
    cbnz    x0, 1f                  /* if error, return */

    /* Call wasm_host_run() — runs init module */
    bl      wasm_host_run
    cbnz    x0, 1f

    ldp     x29, x30, [sp], #16
    ret

1:  /* Error path — return error in x0 */
    ldp     x29, x30, [sp], #16
    ret

/* ----------------------------------------------------------------------------- */
/* run_ready_modules: run spawned modules (shell, etc.)                           */
/* ----------------------------------------------------------------------------- */
.global run_ready_modules
run_ready_modules:
    stp     x29, x30, [sp, #-16]!
    bl      run_ready_modules_c
    ldp     x29, x30, [sp], #16
    ret

/* ----------------------------------------------------------------------------- */
/* Strings (placed in same section as code via -N linker)                         */
/* ----------------------------------------------------------------------------- */
.align 4
boot_banner:
    .asciz "\nAI-ASM AArch64 v0.5 — Multi-Module WASM Runtime\n"
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
