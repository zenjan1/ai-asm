/*
 * aiasm-aarch64/kernel/timer.asm
 * ARM Generic Timer (CNTVCT_EL0) - system tick and timestamp
 * Provides monotonic tick counter for process scheduling
 */
.arch armv8-a

.text

/* Global tick counter (incremented by scheduler) */
.bss
.align 4
.global tick_counter
tick_counter:
    .skip 8
.global timer_frequency
timer_frequency:
    .skip 8

.bss
.align 4
timer_itoa_buf:
    .skip 24

.text

/* -----------------------------------------------------------------------------
 * Function: timer_init
 * Description: Initialize ARM Generic Timer, read frequency
 * Input: none
 * Output: none
 * Clobbered: x0, x1
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_init
timer_init:
    stp     x29, x30, [sp, #-16]!

    /* Read counter frequency */
    mrs     x0, cntfrq_el0

    /* Store frequency */
    adrp    x1, timer_frequency
    add     x1, x1, #:lo12:timer_frequency
    str     x0, [x1]

    /* Reset tick counter */
    adrp    x1, tick_counter
    add     x1, x1, #:lo12:tick_counter
    str     xzr, [x1]

    /* Enable virtual timer, mask interrupt */
    mov     x0, #(1 << 1)       /* IMASK=1, disable IRQ */
    msr     cntv_ctl_el0, x0

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_get_ticks
 * Description: Get current virtual counter value (hardware tick)
 * Input: none
 * Output: x0 = tick count
 * Clobbered: none
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_get_ticks
timer_get_ticks:
    stp     x29, x30, [sp, #-16]!
    mrs     x0, cntvct_el0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_get_ms
 * Description: Get elapsed milliseconds since boot
 * Input: none
 * Output: x0 = milliseconds
 * Clobbered: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_get_ms
timer_get_ms:
    stp     x29, x30, [sp, #-16]!

    mrs     x0, cntvct_el0      /* tick count */
    adrp    x1, timer_frequency
    add     x1, x1, #:lo12:timer_frequency
    ldr     x1, [x1]            /* frequency (Hz) */

    /* ms = (ticks * 1000) / freq */
    mov     x2, #1000
    mul     x0, x0, x2
    udiv    x0, x0, x1

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_tick
 * Description: Called by scheduler each tick, increment counter
 * Input: none
 * Output: none
 * Clobbered: x0
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_tick
timer_tick:
    stp     x29, x30, [sp, #-16]!
    adrp    x0, tick_counter
    add     x0, x0, #:lo12:tick_counter
    ldr     x1, [x0]
    add     x1, x1, #1
    str     x1, [x0]
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_delay_ms
 * Description: Busy-wait delay for given milliseconds
 * Input: x0 = milliseconds
 * Output: none
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_delay_ms
timer_delay_ms:
    stp     x29, x30, [sp, #-16]!

    /* target_ticks = ms * freq / 1000 */
    adrp    x1, timer_frequency
    add     x1, x1, #:lo12:timer_frequency
    ldr     x1, [x1]
    mul     x0, x0, x1
    mov     x2, #1000
    udiv    x0, x0, x2

    mrs     x1, cntvct_el0
    add     x0, x0, x1          /* target tick */

1:
    mrs     x2, cntvct_el0
    cmp     x2, x0
    b.lt    1b

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: timer_info
 * Description: Get timer info as JSON string in buffer
 * Input: x0 = output buffer
 * Output: x0 = buffer pointer
 * Clobbered: x0-x4
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global timer_info
timer_info:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x0              /* save buffer */

    /* Copy prefix */
    adrp    x1, msg_timer_info
    add     x1, x1, #:lo12:msg_timer_info
1:
    ldrb    w2, [x1], #1
    cbz     w2, 2f
    strb    w2, [x0], #1
    b       1b

2:
    /* Print tick count */
    mrs     x1, cntvct_el0
    mov     x0, x1
    mov     x1, x4
    bl      itoa_buf
    mov     x1, x4
1:
    ldrb    w2, [x1], #1
    cbz     w2, 3f
    strb    w2, [x0], #1
    b       1b

3:
    strb    wzr, [x0]
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

.section .rodata
.align 4
msg_timer_info:
    .asciz "{\"tick\":"
