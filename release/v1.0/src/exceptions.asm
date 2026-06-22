/*
 * aiasm-aarch64/kernel/exceptions.asm
 * Exception vector table, IRQ handler, preempt tick
 * 2KB aligned (.align 11) for VBAR_EL1
 *
 * Vector layout (EL1):
 *   0x000–0x07C: EL1t sync     (should never happen)
 *   0x080–0x0FC: EL1t IRQ/FIQ  (should never happen)
 *   0x100–0x17C: EL1t FIQ/SError
 *   0x180–0x1FC: EL1t SError
 *   0x200–0x27C: EL1h sync
 *   0x280–0x2FC: EL1h IRQ     (our main handler)
 *   0x300–0x37C: EL1h FIQ
 *   0x380–0x3FC: EL1h SError
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Exception Vector Table (2KB aligned)
 * Each entry is exactly 128 bytes. First instruction is a branch to handler.
 * ----------------------------------------------------------------------------- */
.align 11
.global exception_vectors
exception_vectors:
    /* Vector 0: EL1t, synchronous — spin */
    b       vector_spin_unhandled
    .space  124

    /* Vector 1: EL1t, IRQ/FIQ — spin */
    b       vector_spin_unhandled
    .space  124

    /* Vector 2: EL1t, FIQ/SError — spin */
    b       vector_spin_unhandled
    .space  124

    /* Vector 3: EL1t, SError — spin */
    b       vector_spin_unhandled
    .space  124

    /* Vector 4: EL1h, synchronous — debug */
    b       vector_sync
    .space  124

    /* Vector 5: EL1h, IRQ — our main timer handler */
    b       irq_entry
    .space  124

    /* Vector 6: EL1h, FIQ — spin */
    b       vector_spin_unhandled
    .space  124

    /* Vector 7: EL1h, SError — spin */
    b       vector_spin_unhandled
    .space  124

/* -----------------------------------------------------------------------------
 * vector_spin_unhandled: infinite loop for unexpected exceptions
 * ----------------------------------------------------------------------------- */
vector_spin_unhandled:
    adrp    x0, msg_exc_spin
    add     x0, x0, #:lo12:msg_exc_spin
    bl      serial_puts
1:  b       1b

/* -----------------------------------------------------------------------------
 * serial_puthex64: print a 64-bit value as hex (e.g. 0x00000000DEADBEEF)
 * Input: x0 = value to print
 * Output: none
 * Clobbered: x0-x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
serial_puthex64:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* save original value (x0 clobbered by serial_putc) */

    /* Print "0x" prefix */
    mov     x0, #'0'
    bl      serial_putc
    mov     x0, #'x'
    bl      serial_putc

    /* Print 16 hex digits, MSB first */
    mov     x0, x8              /* restore original value */
    mov     x1, #60             /* start at bit 60 (nibble 15) */

1:
    /* Extract nibble */
    lsr     x2, x0, x1
    and     x2, x2, #0xF
    /* Convert to ASCII: 0-9 → '0'-'9', A-F → 'A'-'F' */
    cmp     x2, #9
    b.gt    2f
    add     x2, x2, #'0'
    b       3f
2:
    add     x2, x2, #('A' - 10)
3:
    mov     x0, x2
    bl      serial_putc

    subs    x1, x1, #4
    b.ge    1b

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vector_sync: synchronous exception handler — spin silently to avoid loop
 * If this triggers, something is seriously wrong (EL1t misrouting, bad instr)
 * ----------------------------------------------------------------------------- */
vector_sync:
    b       vector_sync

/* -----------------------------------------------------------------------------
 * irq_entry: IRQ entry point at vector 0x280
 * Saves full context, dispatches, restores, returns via eret
 * ----------------------------------------------------------------------------- */
irq_entry:
    /* Save all caller-saved registers (x0–x18) + frame pointer + link register */
    stp     x0, x1, [sp, #-16]!
    stp     x2, x3, [sp, #-16]!
    stp     x4, x5, [sp, #-16]!
    stp     x6, x7, [sp, #-16]!
    stp     x8, x9, [sp, #-16]!
    stp     x10, x11, [sp, #-16]!
    stp     x12, x13, [sp, #-16]!
    stp     x14, x15, [sp, #-16]!
    stp     x16, x17, [sp, #-16]!
    stp     x18, x29, [sp, #-16]!

    /* Save ELR_EL1 and SPSR_EL1 (return address and processor state) */
    mrs     x0, elr_el1
    mrs     x1, spsr_el1
    stp     x0, x1, [sp, #-16]!

    /* Acknowledge IRQ via GIC and get IRQ ID */
    bl      gic_ack_irq

    /* Check if it's the virtual timer IRQ (IRQ 27) */
    cmp     w0, #27
    b.ne    irq_done

    /* Increment timer tick counter */
    bl      timer_tick

    /* Disable timer to prevent repeated IRQs while module is in wfi */
    msr     cntv_ctl_el0, xzr

    /* Set preemption flag — main loop uses this to invoke scheduler */
    adrp    x0, preempt_pending
    add     x0, x0, #:lo12:preempt_pending
    mov     w1, #1
    strb    w1, [x0]

irq_done:
    /* Restore ELR_EL1 and SPSR_EL1 */
    ldp     x0, x1, [sp], #16
    msr     elr_el1, x0
    msr     spsr_el1, x1

    /* Restore all registers in reverse order */
    ldp     x18, x29, [sp], #16
    ldp     x16, x17, [sp], #16
    ldp     x14, x15, [sp], #16
    ldp     x12, x13, [sp], #16
    ldp     x10, x11, [sp], #16
    ldp     x8, x9, [sp], #16
    ldp     x6, x7, [sp], #16
    ldp     x4, x5, [sp], #16
    ldp     x2, x3, [sp], #16
    ldp     x0, x1, [sp], #16

    /* Return to interrupted code */
    eret

/* -----------------------------------------------------------------------------
 * proc_schedule_loop: main scheduler loop with preemption
 * Called after all WASM modules are loaded. Runs forever.
 *
 * The loop checks preempt_pending (set by timer IRQ every 10ms).
 * When set, it calls proc_schedule and wasm_host_run to step modules.
 * ----------------------------------------------------------------------------- */
.global proc_schedule_loop
proc_schedule_loop:
    /* Check preempt_pending flag */
    adrp    x0, preempt_pending
    add     x0, x0, #:lo12:preempt_pending
    ldrb    w1, [x0]
    cbz     w1, sched_wfi         /* no pending preempt — sleep */

    /* Clear preempt flag */
    strb    wzr, [x0]

    /* Re-enable timer for next 10ms interval */
    mov     x0, #10
    bl      timer_set_interval

    /* Run scheduler and WASM module on kernel stack (safe for C calls) */
    stp     x29, x30, [sp, #-16]!

    /* proc_schedule() — advances scheduler state, selects next module */
    bl      proc_schedule

    /* wasm_host_run() — finds next ready module and executes it */
    bl      wasm_host_run

    ldp     x29, x30, [sp], #16

sched_wfi:
    wfi
    b       proc_schedule_loop

/* -----------------------------------------------------------------------------
 * Debug strings
 * ----------------------------------------------------------------------------- */
.section .rodata
.align 4
msg_exc_spin:
    .asciz "\n[EXC] Unhandled exception — spinning\n"
msg_sync_exc:
    .asciz "\n[EXC] Sync: ESR="
