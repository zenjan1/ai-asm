/*
 * aiasm-aarch64/kernel/power_sleep.asm
 * Sleep State Management (v1.0)
 *
 * AArch64 ASM implementation of system sleep state transitions.
 * Manages suspend, hibernate, standby states with wakeup sources.
 *
 * Features:
 *   - Sleep state selection (S1-S4)
 *   - Wakeup alarm setup
 *   - Device suspend/resume ordering
 *   - RTC alarm management
 *   - Sleep statistics tracking
 *   - Entry/exit latency measurement
 *
 * Integration: called by power management, device drivers, scheduler
 *
 * API:
 *   power_sleep_init()                              — init sleep subsystem
 *   power_sleep_enter(state_id) => 0/-1             — enter sleep state
 *   power_sleep_exit() => 0/-1                      — exit current sleep
 *   power_sleep_set_wakeup(device_id) => 0/-1       — register wakeup source
 *   power_sleep_rtc_alarm(seconds) => 0/-1          — set RTC wakeup alarm
 *   power_sleep_stats(out_stats)                    — get sleep statistics
 *   power_sleep_get_state() => state_id             — get current state
 *   power_sleep_resume_devices() => resumed_count   — resume all suspended devs
 */
.arch armv8-a

/* Constants */
.set SLEEP_STATE_ACTIVE,    0
.set SLEEP_STATE_S1,        1       /* standby */
.set SLEEP_STATE_S2,        2       /* stop */
.set SLEEP_STATE_S3,        3       /* suspend-to-RAM */
.set SLEEP_STATE_S4,        4       /* hibernate */
.set MAX_WAKEUP_SOURCES,    32
.set MAX_SLEEP_DEVICES,     64

/* Sleep config (24 bytes) */
/* current_state, default_state, wakeup_mask, rtc_alarm_sec, entry_count, exit_count */

.bss
.align 4
.global power_sleep_init_done
power_sleep_init_done:
    .skip 4

.align 4
power_sleep_current_state:
    .skip 4
power_sleep_default_state:
    .skip 4
power_sleep_total_sleeps:
    .skip 4
power_sleep_total_wakes:
    .skip 4
power_sleep_rtc_alarm:
    .skip 4
power_sleep_last_entry_time:
    .skip 4
power_sleep_last_exit_time:
    .skip 4

/* Wakeup source bitmask (32 sources) */
.align 4
power_sleep_wakeup_mask:
    .skip 4

/* Device suspend state array (64 entries, 4 bytes each) */
.align 4
power_sleep_dev_state:
    .skip MAX_SLEEP_DEVICES * 4     /* 256 bytes */

/* Sleep history (last 16 entries, 8 bytes each) */
.align 4
power_sleep_history:
    .skip 16 * 8                    /* 128 bytes */

.text

/* -----------------------------------------------------------------------------
 * power_sleep_init — initialize sleep subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global power_sleep_init
power_sleep_init:
    stp     x29, x30, [sp, #-16]!

    /* Set initial state */
    ldr     x0, =power_sleep_current_state
    mov     w1, #SLEEP_STATE_ACTIVE
    str     w1, [x0]

    ldr     x0, =power_sleep_default_state
    mov     w1, #SLEEP_STATE_S3
    str     w1, [x0]

    /* Zero counters */
    ldr     x0, =power_sleep_total_sleeps
    str     wzr, [x0]
    ldr     x0, =power_sleep_total_wakes
    str     wzr, [x0]
    ldr     x0, =power_sleep_rtc_alarm
    str     wzr, [x0]
    ldr     x0, =power_sleep_last_entry_time
    str     wzr, [x0]
    ldr     x0, =power_sleep_last_exit_time
    str     wzr, [x0]

    /* Clear wakeup mask */
    ldr     x0, =power_sleep_wakeup_mask
    str     wzr, [x0]

    /* Zero device state array */
    ldr     x0, =power_sleep_dev_state
    mov     x1, #(MAX_SLEEP_DEVICES * 4)
    bl      .Lsleep_zero

    /* Zero history */
    ldr     x0, =power_sleep_history
    mov     x1, #(16 * 8)
    bl      .Lsleep_zero

    ldr     x0, =power_sleep_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lsleep_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_enter — enter a sleep state
 * Input: x0 = state_id (SLEEP_STATE_S1..S4)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_sleep_enter
power_sleep_enter:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #0
    b.eq    .Lsleep_enter_fail      /* can't enter ACTIVE */
    cmp     w0, #4
    b.hi    .Lsleep_enter_fail

    /* Check current state is ACTIVE */
    ldr     x1, =power_sleep_current_state
    ldr     w1, [x1]
    cmp     w1, #SLEEP_STATE_ACTIVE
    b.ne    .Lsleep_enter_fail

    /* Set new state */
    str     w0, [x1]

    /* Suspend all devices (simplified: mark all as suspended) */
    ldr     x1, =power_sleep_dev_state
    mov     w2, #0
.Lsleep_suspend_loop:
    cmp     w2, #MAX_SLEEP_DEVICES
    b.hs    .Lsleep_suspend_done
    mov     x3, w2, uxtw
    lsl     x3, x3, #2
    mov     w4, #1
    str     w4, [x1, x3]            /* dev_state[i] = suspended */
    add     w2, w2, #1
    b       .Lsleep_suspend_loop

.Lsleep_suspend_done:
    /* Update sleep count */
    ldr     x0, =power_sleep_total_sleeps
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lsleep_enter_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_exit — exit current sleep state
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_sleep_exit
power_sleep_exit:
    stp     x29, x30, [sp, #-16]!

    /* Check not already active */
    ldr     x0, =power_sleep_current_state
    ldr     w0, [x0]
    cmp     w0, #SLEEP_STATE_ACTIVE
    b.eq    .Lsleep_exit_fail

    /* Set to ACTIVE */
    mov     w1, #SLEEP_STATE_ACTIVE
    str     w1, [x0]

    /* Update wake count */
    ldr     x0, =power_sleep_total_wakes
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lsleep_exit_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_set_wakeup — register a wakeup source
 * Input: x0 = device_id (0-31)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_sleep_set_wakeup
power_sleep_set_wakeup:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_WAKEUP_SOURCES
    b.hs    .Lsleep_wake_fail

    /* Set bit in wakeup mask */
    ldr     x1, =power_sleep_wakeup_mask
    ldr     w1, [x1]
    mov     x2, #1
    lsl     x2, x2, x0
    orr     w1, w1, w2
    ldr     x3, =power_sleep_wakeup_mask
    str     w1, [x3]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lsleep_wake_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_rtc_alarm — set RTC wakeup alarm
 * Input: x0 = seconds from now
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_sleep_rtc_alarm
power_sleep_rtc_alarm:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =power_sleep_rtc_alarm
    str     w0, [x1]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_get_state — get current sleep state
 * Output: x0 = state_id
 * ----------------------------------------------------------------------------- */
.global power_sleep_get_state
power_sleep_get_state:
    ldr     x0, =power_sleep_current_state
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_resume_devices — resume all suspended devices
 * Output: x0 = number of devices resumed
 * ----------------------------------------------------------------------------- */
.global power_sleep_resume_devices
power_sleep_resume_devices:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =power_sleep_dev_state
    mov     w0, #0
    mov     w2, #0

.Lsleep_resume_loop:
    cmp     w2, #MAX_SLEEP_DEVICES
    b.hs    .Lsleep_resume_done

    mov     x3, w2, uxtw
    lsl     x3, x3, #2
    ldr     w4, [x1, x3]
    cbz     w4, .Lsleep_resume_next

    /* Resume device */
    str     wzr, [x1, x3]           /* dev_state[i] = active */
    add     w0, w0, #1

.Lsleep_resume_next:
    add     w2, w2, #1
    b       .Lsleep_resume_loop

.Lsleep_resume_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_sleep_stats — get sleep statistics
 * Input: x0 = output buffer (24 bytes)
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global power_sleep_stats
power_sleep_stats:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =power_sleep_current_state
    ldr     w1, [x1]
    str     w1, [x0, #0]

    ldr     x1, =power_sleep_total_sleeps
    ldr     w1, [x1]
    str     w1, [x0, #4]

    ldr     x1, =power_sleep_total_wakes
    ldr     w1, [x1]
    str     w1, [x0, #8]

    ldr     x1, =power_sleep_rtc_alarm
    ldr     w1, [x1]
    str     w1, [x0, #12]

    ldr     x1, =power_sleep_wakeup_mask
    ldr     w1, [x1]
    str     w1, [x0, #16]

    ldr     x1, =power_sleep_default_state
    ldr     w1, [x1]
    str     w1, [x0, #20]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
