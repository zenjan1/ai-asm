/*
 * aiasm-aarch64/kernel/idt.asm
 * Exception vector table for AArch64 EL1
 * Handles sync exceptions, IRQ, FIQ, SError from EL0 and EL1
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: vector_table
 * Description: AArch64 exception vector table (2KB aligned)
 * Must be placed at 2KB-aligned address
 * ----------------------------------------------------------------------------- */
.align 11  /* 2048-byte alignment */
.global vector_table
vector_table:
    /* Current EL with SP0 */
    .align 7
    b       exc_sync_sp0
    .align 7
    b       exc_irq_sp0
    .align 7
    b       exc_fiq_sp0
    .align 7
    b       exc_serror_sp0

    /* Current EL with SPx */
    .align 7
    b       exc_sync_spx
    .align 7
    b       exc_irq_spx
    .align 7
    b       exc_fiq_spx
    .align 7
    b       exc_serror_spx

    /* Lower EL (AArch64) */
    .align 7
    b       exc_lower_a64
    .align 7
    b       exc_lower_irq
    .align 7
    b       exc_lower_fiq
    .align 7
    b       exc_lower_serror

    /* Lower EL (AArch32) */
    .align 7
    b       exc_lower_a32
    .align 7
    b       exc_lower_a32_irq
    .align 7
    b       exc_lower_a32_fiq
    .align 7
    b       exc_lower_a32_serror

/* -----------------------------------------------------------------------------
 * Exception handler stubs
 * All handlers save context, log the exception, then hang
 * ----------------------------------------------------------------------------- */

exc_sync_sp0:
exc_sync_spx:
    /* Save LR for return */
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    /* Log exception */
    mrs     x0, esr_el1
    bl      print_hex           /* Print syndrome register */

    adrp    x0, msg_sync_exc
    add     x0, x0, :lo12:msg_sync_exc
    bl      serial_puts

    b       exc_common_hang

exc_irq_sp0:
exc_irq_spx:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, msg_irq_exc
    add     x0, x0, :lo12:msg_irq_exc
    bl      serial_puts
    b       exc_common_hang

exc_fiq_sp0:
exc_fiq_spx:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, msg_fiq_exc
    add     x0, x0, :lo12:msg_fiq_exc
    bl      serial_puts
    b       exc_common_hang

exc_serror_sp0:
exc_serror_spx:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, msg_serror_exc
    add     x0, x0, :lo12:msg_serror_exc
    bl      serial_puts
    b       exc_common_hang

exc_lower_a64:
exc_lower_irq:
exc_lower_fiq:
exc_lower_serror:
exc_lower_a32:
exc_lower_a32_irq:
exc_lower_a32_fiq:
exc_lower_a32_serror:
    /* Lower EL exceptions: should not happen in our setup */
    stp     x29, x30, [sp, #-16]!
    adrp    x0, msg_lower_exc
    add     x0, x0, :lo12:msg_lower_exc
    bl      serial_puts
    b       exc_common_hang

exc_common_hang:
1:
    wfi
    b       1b

.section .rodata
.align 4
msg_sync_exc:
    .asciz "SYNC EXCEPTION\r\n"
msg_irq_exc:
    .asciz "IRQ EXCEPTION\r\n"
msg_fiq_exc:
    .asciz "FIQ EXCEPTION\r\n"
msg_serror_exc:
    .asciz "SError EXCEPTION\r\n"
msg_lower_exc:
    .asciz "LOWER EL EXCEPTION\r\n"
