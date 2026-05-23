/*
 * aiasm-aarch64/kernel/kernel.asm
 * Kernel entry point and initialization sequence v0.2
 * Pure AArch64 assembly, load at 0x40080000
 * Adds: memory manager, timer, GIC, process scheduler, syscalls
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: _start
 * Description: Kernel entry point. Sets up stack, clears BSS, initializes
 *              all subsystems (UART, log, memory, timer, GIC, processes, events),
 *              publishes boot event, and enters shell.
 * Input: x0 = device tree address (from bootloader, unused)
 * Output: never returns
 * Clobbered: all registers
 * Stack: 0 bytes (sets up stack first)
 * ----------------------------------------------------------------------------- */
.global _start
_start:
    /* Mask all exceptions */
    msr     daifset, #0xf

    /* Setup stack pointer (8KB, high address, 16-byte aligned) */
    adrp    x0, __stack_top
    add     x0, x0, #:lo12:__stack_top
    mov     sp, x0

    /* Clear BSS */
    bl      bss_clear

    /* Initialize PL011 UART */
    bl      serial_init

    /* Initialize logging */
    bl      log_init

    /* Initialize physical memory manager */
    bl      mem_init

    /* Debug: T = timer */
    mov     x0, #'T'
    bl      serial_putc

    /* Initialize ARM Generic Timer */
    bl      timer_init

    /* Debug: G = GIC */
    mov     x0, #'G'
    bl      serial_putc

    /* Initialize GICv2 interrupt controller */
    bl      gic_init

    /* Debug: P = process */
    mov     x0, #'P'
    bl      serial_putc

    /* Initialize process manager */
    bl      proc_init

    /* Debug: E = event */
    mov     x0, #'E'
    bl      serial_putc

    /* Initialize event bus */
    bl      event_init

    /* Debug: S = shell */
    mov     x0, #'S'
    bl      serial_putc

    /* Publish boot event with v0.2 info */
    mov     w0, #1              /* INFO level */
    adrp    x1, evt_boot
    add     x1, x1, #:lo12:evt_boot
    adrp    x2, data_boot_v2
    add     x2, x2, #:lo12:data_boot_v2
    bl      log_event

    /* Publish memory init event */
    mov     w0, #1
    adrp    x1, evt_mem_init
    add     x1, x1, #:lo12:evt_mem_init
    adrp    x2, data_mem_init
    add     x2, x2, #:lo12:data_mem_init
    bl      log_event

    /* Publish timer init event */
    mov     w0, #1
    adrp    x1, evt_timer_init
    add     x1, x1, #:lo12:evt_timer_init
    adrp    x2, data_timer_init
    add     x2, x2, #:lo12:data_timer_init
    bl      log_event

    /* Publish scheduler init event */
    mov     w0, #1
    adrp    x1, evt_sched_init
    add     x1, x1, #:lo12:evt_sched_init
    adrp    x2, data_sched_init
    add     x2, x2, #:lo12:data_sched_init
    bl      log_event

    /* Enter shell */
    bl      shell_run

    /* Should never reach here */
    bl      shell_hang

/* -----------------------------------------------------------------------------
 * Function: bss_clear
 * Description: Zero the BSS section
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
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

/* Read-only boot strings */
.section .rodata
.align 4
evt_boot:
    .asciz "boot"
data_boot_v2:
    .asciz "{\"version\":\"0.2-aarch64\",\"arch\":\"aarch64\",\"features\":\"mmu,process,syscall\"}"
evt_mem_init:
    .asciz "memory"
data_mem_init:
    .asciz "{\"status\":\"ok\",\"manager\":\"bitmap\"}"
evt_timer_init:
    .asciz "timer"
data_timer_init:
    .asciz "{\"status\":\"ok\",\"source\":\"cntvct\"}"
evt_sched_init:
    .asciz "scheduler"
data_sched_init:
    .asciz "{\"status\":\"ok\",\"algo\":\"round-robin\"}"
