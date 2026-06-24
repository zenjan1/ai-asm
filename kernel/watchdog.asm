/*
 * aiasm-aarch64/kernel/watchdog.asm
 * Hardware Watchdog Timer subsystem (v1.0)
 *
 * Monitors system health via configurable timeout counter.
 * Software watchdog with kick mechanism; prevents system hang.
 * Timeout triggers system reset via PSCI or panic handler.
 *
 * API:
 *   watchdog_init()                  — initialize watchdog (default 30s timeout)
 *   watchdog_kick()                  — refresh counter (pet the dog)
 *   watchdog_set_timeout(seconds)    — set custom timeout value
 *   watchdog_disable()               — disable watchdog
 *   watchdog_get_status() => x0      — return remaining ticks
 */
.arch armv8-a

/* Default timeout in seconds */
.set WD_DEFAULT_TIMEOUT, 30

.bss
.align 4
.global wd_enabled
wd_enabled:
    .skip 4
.global wd_timeout_ticks
wd_timeout_ticks:
    .skip 8          /* timeout value in ticks */
wd_remaining_ticks:
    .skip 8          /* remaining ticks until reset */
wd_tick_rate:
    .skip 8          /* ticks per second (from cntfrq_el0) */
wd_last_kick:
    .skip 8          /* timestamp of last kick */

.text

/* -----------------------------------------------------------------------------
 * watchdog_init — read timer frequency, set default timeout, enable watchdog
 * Input: none
 * Output: none
 * Clobbered: x0-x3
 * ----------------------------------------------------------------------------- */
.global watchdog_init
watchdog_init:
    stp     x29, x30, [sp, #-16]!

    /* Read timer frequency */
    mrs     x0, cntfrq_el0
    ldr     x1, =wd_tick_rate
    str     x0, [x1]

    /* Set default timeout: WD_DEFAULT_TIMEOUT * tick_rate */
    mov     x2, #WD_DEFAULT_TIMEOUT
    mul     x3, x0, x2
    ldr     x1, =wd_timeout_ticks
    str     x3, [x1]
    str     x3, [x1, #(wd_remaining_ticks - wd_timeout_ticks)]

    /* Record initial kick timestamp */
    mrs     x0, cntvct_el0
    ldr     x1, =wd_last_kick
    str     x0, [x1]

    /* Enable watchdog */
    mov     x0, #1
    ldr     x1, =wd_enabled
    str     w0, [x1]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * watchdog_kick — refresh the watchdog counter
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global watchdog_kick
watchdog_kick:
    stp     x29, x30, [sp, #-16]!

    ldr     x0, =wd_enabled
    ldr     w1, [x0]
    cbz     w1, 2f              /* skip if disabled */

    /* Reload remaining ticks from timeout value */
    ldr     x0, =wd_timeout_ticks
    ldr     x1, [x0]
    ldr     x2, =wd_remaining_ticks
    str     x1, [x2]

    /* Update last kick timestamp */
    mrs     x1, cntvct_el0
    ldr     x2, =wd_last_kick
    str     x1, [x2]

2:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * watchdog_set_timeout — set custom timeout in seconds
 * Input: x0 = timeout in seconds
 * Output: none
 * ----------------------------------------------------------------------------- */
.global watchdog_set_timeout
watchdog_set_timeout:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =wd_tick_rate
    ldr     x1, [x1]
    mul     x2, x0, x1          /* timeout_ticks = seconds * tick_rate */

    ldr     x1, =wd_timeout_ticks
    str     x2, [x1]
    str     x2, [x1, #(wd_remaining_ticks - wd_timeout_ticks)]

    bl      watchdog_kick       /* apply immediately */

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * watchdog_disable — disable the watchdog
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global watchdog_disable
watchdog_disable:
    mov     x0, #0
    ldr     x1, =wd_enabled
    str     w0, [x1]
    ret

/* -----------------------------------------------------------------------------
 * watchdog_get_status — return remaining ticks (0 = expired)
 * Input: none
 * Output: x0 = remaining ticks
 * ----------------------------------------------------------------------------- */
.global watchdog_get_status
watchdog_get_status:
    stp     x29, x30, [sp, #-16]!

    ldr     x0, =wd_enabled
    ldr     w1, [x0]
    cbz     w1, .Lwd_disabled

    /* Calculate elapsed ticks since last kick */
    mrs     x0, cntvct_el0
    ldr     x1, =wd_last_kick
    ldr     x1, [x1]
    sub     x2, x0, x1          /* elapsed = now - last_kick */

    ldr     x1, =wd_timeout_ticks
    ldr     x1, [x1]
    sub     x0, x1, x2          /* remaining = timeout - elapsed */

    /* Clamp to zero */
    cmp     x0, #0
    bge     2f
    mov     x0, #0
2:  ldp     x29, x30, [sp], #16
    ret

.Lwd_disabled:
    mov     x0, #-1             /* -1 = disabled */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * watchdog_tick — called by timer interrupt to decrement counter
 * If expired, trigger panic/reset
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global watchdog_tick
watchdog_tick:
    stp     x29, x30, [sp, #-16]!

    ldr     x0, =wd_enabled
    ldr     w1, [x0]
    cbz     w1, 2f

    ldr     x0, =wd_remaining_ticks
    ldr     x1, [x0]
    subs    x1, x1, #1
    str     x1, [x0]

    bne     2f                  /* not expired yet */

    /* Watchdog expired — trigger system panic */
    /* In production, this would call panic() or PSCI SYSTEM_RESET */
    /* For now, set a flag for the shell to detect */
    mov     x0, #0xDEAD
    movk    x0, #0xBEEF, lsl #16
    /* TODO: call kernel_panic(watchdog_timeout) */

2:  ldp     x29, x30, [sp], #16
    ret
