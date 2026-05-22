/*
 * aiasm-aarch64/kernel/timer.asm
 * ARM Generic Timer driver for AArch64
 * Uses virtual counter (CNTVCT_EL0) for timestamp
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: timer_init
 * Description: Initialize ARM Generic Timer, enable virtual timer
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_init
timer_init:
    stp     x29, x30, [sp, #-16]!

    /* Read counter frequency */
    mrs     x0, cntfrq_el0

    /* Enable virtual timer, set IMASK to disable interrupt */
    mov     x0, #(1 << 1)       /* IMASK=1 */
    msr     cntv_ctl_el0, x0

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_get_timestamp
 * Description: Get current virtual counter value (tick count)
 * Input: None
 * Output: x0 = timestamp
 * Clobbered: none
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_get_timestamp
timer_get_timestamp:
    stp     x29, x30, [sp, #-16]!
    mrs     x0, cntvct_el0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_delay_us
 * Description: Busy-wait delay for microseconds
 * Input: x0 = microseconds
 * Output: None
 * Clobbered: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_delay_us
timer_delay_us:
    stp     x29, x30, [sp, #-16]!

    mrs     x1, cntfrq_el0      /* Frequency in Hz */
    madd    x0, x0, x1, xzr     /* x0 = us * freq */
    lsr     x0, x0, #20         /* Divide by 1048576 (~1M) */

    mrs     x2, cntvct_el0
    add     x0, x0, x2          /* Target count */
1:
    mrs     x1, cntvct_el0
    cmp     x1, x0
    b.lt    1b

    ldp     x29, x30, [sp], #16
    ret
