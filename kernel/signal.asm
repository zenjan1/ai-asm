/*
 * aiasm-aarch64/kernel/signal.asm
 * POSIX-style signal mechanism for AI-ASM v3.0
 *
 * Signals: SIGKILL(9), SIGTERM(15), SIGSTOP(17), SIGCONT(18), SIGCHLD(20)
 * Per-process: pending signal bitmap (32-bit), handler table (5 signals)
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Local copies of process.asm constants
 * ----------------------------------------------------------------------------- */
.set MAX_PROCS,     8
.set PCB_SIZE,      384
.set PCB_STATE,     0
.set PCB_PID,       4
.set PROC_RUNNING,  2
.set PROC_READY,    1
.set PROC_FREE,     0
.set PROC_PAUSED,   3
.set PROC_EXITED,   4
.set PCB_PENDING_SIG,  324
.set PCB_SIG_HANDLERS, 328
.set PCB_SIG_MASK,     360

/* -----------------------------------------------------------------------------
 * Signal constants
 * ----------------------------------------------------------------------------- */
.set SIGKILL,   9
.set SIGTERM,   15
.set SIGSTOP,   17
.set SIGCONT,   18
.set SIGCHLD,   20

.set SIG_DFL,   0   /* default action */
.set SIG_IGN,   1   /* ignore */

/* Signal handler offsets (from PCB_SIG_HANDLERS base) */
.set SIG_H_KILL,      0
.set SIG_H_TERM,      4
.set SIG_H_STOP,      8
.set SIG_H_CONT,      12
.set SIG_H_CHLD,      16

/* -----------------------------------------------------------------------------
 * BSS
 * ----------------------------------------------------------------------------- */
.bss
.align 4
signal_init_done:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * Helper: add_pcb_sig_handlers
 * Input: x0 = PCB base
 * Output: x0 = PCB base + PCB_SIG_HANDLERS
 * ----------------------------------------------------------------------------- */
add_pcb_sig_handlers:
    adrp    x9, .Lpcb_sig_off
    add     x9, x9, #:lo12:.Lpcb_sig_off
    ldr     x9, [x9]
    add     x0, x0, x9
    ret

.Lpcb_sig_off:
    .quad PCB_SIG_HANDLERS

/* -----------------------------------------------------------------------------
 * Function: signal_init
 * ----------------------------------------------------------------------------- */
.global signal_init
signal_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, signal_init_done
    add     x0, x0, #:lo12:signal_init_done
    ldr     w1, [x0]
    cbnz    w1, 9f

    /* Clear all PCB signal fields */
    adrp    x2, pcb_table
    add     x2, x2, #:lo12:pcb_table
    mov     x3, #0

1:  cmp     x3, #MAX_PROCS
    bge     2f

    /* x4 = pcb_table + idx * PCB_SIZE */
    adrp    x5, pcb_table
    add     x5, x5, #:lo12:pcb_table
    mov     x6, #PCB_SIZE
    mul     x4, x3, x6
    add     x4, x4, x5

    /* Clear pending signals */
    str     wzr, [x4, #PCB_PENDING_SIG]
    /* Clear signal mask */
    str     wzr, [x4, #PCB_SIG_MASK]
    /* Clear handler table (5 entries) */
    str     wzr, [x4, #PCB_SIG_HANDLERS]
    str     wzr, [x4, #PCB_SIG_HANDLERS + 4]
    str     wzr, [x4, #PCB_SIG_HANDLERS + 8]
    str     wzr, [x4, #PCB_SIG_HANDLERS + 12]
    str     wzr, [x4, #PCB_SIG_HANDLERS + 16]

    add     x3, x3, #1
    b       1b

2:  mov     w1, #1
    adrp    x0, signal_init_done
    add     x0, x0, #:lo12:signal_init_done
    str     w1, [x0]

9:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: signal_register
 * Input: w0 = pid, w1 = signal number, x2 = handler address
 * Output: w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global signal_register
signal_register:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     x21, x2

    /* Find process by pid */
    mov     w0, w20
    bl      proc_find_by_pid
    cbz     x0, 9f                   /* process not found */

    /* x0 = PCB base, w1 = signal, x21 = handler */
    /* Get handler table address */
    bl      add_pcb_sig_handlers
    mov     x4, x0                   /* x4 = handler table base */

    cmp     w1, #SIGKILL
    b.eq    1f
    cmp     w1, #SIGTERM
    b.eq    2f
    cmp     w1, #SIGSTOP
    b.eq    3f
    cmp     w1, #SIGCONT
    b.eq    4f
    cmp     w1, #SIGCHLD
    b.eq    5f
    b       8f

1:  str     x21, [x4, #SIG_H_KILL]
    b       8f
2:  str     x21, [x4, #SIG_H_TERM]
    b       8f
3:  str     x21, [x4, #SIG_H_STOP]
    b       8f
4:  str     x21, [x4, #SIG_H_CONT]
    b       8f
5:  str     x21, [x4, #SIG_H_CHLD]

8:  mov     w0, #0
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: signal_send
 * Input: w0 = pid, w1 = signal number
 * Output: w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global signal_send
signal_send:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1

    /* SIGKILL: immediately kill */
    cmp     w21, #SIGKILL
    b.ne    .Lss_normal

    mov     w0, w20
    bl      proc_find_by_pid
    cbz     x0, 9f

    /* Set state to EXITED */
    mov     w1, #PROC_EXITED
    str     w1, [x0, #PCB_STATE]

    /* Decrement process count */
    adrp    x1, proc_count
    add     x1, x1, #:lo12:proc_count
    ldr     w2, [x1]
    sub     w2, w2, #1
    str     w2, [x1]

    mov     w0, #0
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

.Lss_normal:
    /* Normal signal: set pending bit */
    mov     w0, w20
    bl      proc_find_by_pid
    cbz     x0, 9f

    /* x0 = PCB base */
    /* Compute bit position: 1 << sig */
    mov     w5, #1
    lsl     w5, w5, w21

    /* OR into pending bitmap */
    ldr     w6, [x0, #PCB_PENDING_SIG]
    orr     w6, w6, w5
    str     w6, [x0, #PCB_PENDING_SIG]

    mov     w0, #0
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

9:  mov     w0, #-1
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: signal_check
 * Check and handle pending signals for current process
 * Called from scheduler before returning from proc_schedule
 * ----------------------------------------------------------------------------- */
.global signal_check
signal_check:
    stp     x29, x30, [sp, #-16]!

    bl      proc_get_current
    cbz     x0, 9f

    mov     x20, x0                  /* x20 = current PCB */

    /* Load pending and mask */
    ldr     w5, [x20, #PCB_PENDING_SIG]
    ldr     w6, [x20, #PCB_SIG_MASK]
    bic     w5, w5, w6               /* remove masked */
    cbz     w5, 9f

    /* Check signals in priority order */
    tst     w5, #(1 << 9)            /* SIGKILL */
    b.ne    .Lsc_kill

    tst     w5, #(1 << 15)           /* SIGTERM */
    b.ne    .Lsc_term

    tst     w5, #(1 << 17)           /* SIGSTOP */
    b.ne    .Lsc_stop

    tst     w5, #(1 << 18)           /* SIGCONT */
    b.ne    .Lsc_cont

    tst     w5, #(1 << 20)           /* SIGCHLD */
    b.ne    .Lsc_chld

9:  ldp     x29, x30, [sp], #16
    ret

.Lsc_kill:
    mov     w1, #PROC_EXITED
    str     w1, [x20, #PCB_STATE]
    str     wzr, [x20, #PCB_PENDING_SIG]

    adrp    x1, proc_count
    add     x1, x1, #:lo12:proc_count
    ldr     w2, [x1]
    sub     w2, w2, #1
    str     w2, [x1]

    ldp     x29, x30, [sp], #16
    ret

.Lsc_term:
    /* Check custom handler */
    bl      add_pcb_sig_handlers
    ldr     x7, [x0, #SIG_H_TERM]
    cbz     x7, .Lsc_term_default
    cmp     x7, #SIG_IGN
    b.eq    .Lsc_term_clear

.Lsc_term_default:
    mov     w1, #PROC_EXITED
    str     w1, [x20, #PCB_STATE]

.Lsc_term_clear:
    ldr     w5, [x20, #PCB_PENDING_SIG]
    bic     w5, w5, #(1 << 15)
    str     w5, [x20, #PCB_PENDING_SIG]
    ldp     x29, x30, [sp], #16
    ret

.Lsc_stop:
    bl      add_pcb_sig_handlers
    ldr     x7, [x0, #SIG_H_STOP]
    cbz     x7, .Lsc_stop_default
    cmp     x7, #SIG_IGN
    b.eq    .Lsc_stop_clear

.Lsc_stop_default:
    mov     w1, #PROC_PAUSED
    str     w1, [x20, #PCB_STATE]

.Lsc_stop_clear:
    ldr     w5, [x20, #PCB_PENDING_SIG]
    bic     w5, w5, #(1 << 17)
    str     w5, [x20, #PCB_PENDING_SIG]
    ldp     x29, x30, [sp], #16
    ret

.Lsc_cont:
    ldr     w1, [x20, #PCB_STATE]
    cmp     w1, #PROC_PAUSED
    b.ne    .Lsc_cont_clear
    mov     w1, #PROC_RUNNING
    str     w1, [x20, #PCB_STATE]

.Lsc_cont_clear:
    ldr     w5, [x20, #PCB_PENDING_SIG]
    bic     w5, w5, #(1 << 18)
    str     w5, [x20, #PCB_PENDING_SIG]
    ldp     x29, x30, [sp], #16
    ret

.Lsc_chld:
    /* Default: ignore, just clear bit */
    ldr     w5, [x20, #PCB_PENDING_SIG]
    bic     w5, w5, #(1 << 20)
    str     w5, [x20, #PCB_PENDING_SIG]
    ldp     x29, x30, [sp], #16
    ret
