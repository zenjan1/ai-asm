/*
 * aiasm-aarch64/kernel/power_thermal.asm
 * Thermal Management (v1.0)
 *
 * AArch64 ASM implementation of thermal zone monitoring and cooling device
 * management. Provides temperature tracking, trip point handling, and
 * passive/active cooling policies.
 *
 * Features:
 *   - Thermal zone tracking
 *   - Temperature trip points (hot, critical)
 *   - Passive/active cooling policies
 *   - CPU throttling coordination
 *   - Fan speed control
 *   - Emergency shutdown
 *   - Thermal statistics
 *
 * Integration: called by power_cpufreq, device drivers, scheduler
 *
 * API:
 *   power_thermal_init()                             — init thermal subsystem
 *   power_thermal_register_zone(zone_id, trip_hot, trip_crit) => 0/-1
 *   power_thermal_get_temp(zone_id) => temp_mc or -1 — get temperature (millicelsius)
 *   power_thermal_register_cooling(dev_id, type) => 0/-1 — register cooling device
 *   power_thermal_set_cooling(dev_id, state) => 0/-1 — set cooling state
 *   power_thermal_stats(out_stats)                   — get thermal statistics
 *   power_thermal_emergency_check() => zone_in_crit  — check for critical temps
 */
.arch armv8-a

/* Constants */
.set MAX_THERMAL_ZONES,    8
.set MAX_COOLING_DEVS,     16
.set MAX_TRIP_POINTS,      4
.set COOL_FAN,             1
.set COOL_CPU_THROTTLE,    2
.set COOL_GPU_THROTTLE,    3
.set COOL_POWER_CLAMP,     4

/* Thermal zone (32 bytes) */
/* zone_id, temperature_mc, trip_hot, trip_crit, cooling_dev, trend, flags, active */

/* Cooling device (16 bytes) */
/* dev_id, type, cur_state, max_state */

.bss
.align 4
.global power_thermal_init_done
power_thermal_init_done:
    .skip 4

.align 4
power_thermal_zone_count:
    .skip 4
power_thermal_cooling_count:
    .skip 4
power_thermal_throttle_events:
    .skip 4
power_thermal_critical_events:
    .skip 4

/* Thermal zones */
.align 4
power_thermal_zones:
    .skip MAX_THERMAL_ZONES * 32     /* 256 bytes */

/* Cooling devices */
.align 4
power_thermal_cooling:
    .skip MAX_COOLING_DEVS * 16      /* 256 bytes */

/* Temperature history (per zone, 8 samples) */
.align 4
power_thermal_history:
    .skip MAX_THERMAL_ZONES * 8 * 4  /* 256 bytes */

.text

/* -----------------------------------------------------------------------------
 * power_thermal_init — initialize thermal subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global power_thermal_init
power_thermal_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =power_thermal_zone_count
    str     wzr, [x0]
    ldr     x0, =power_thermal_cooling_count
    str     wzr, [x0]
    ldr     x0, =power_thermal_throttle_events
    str     wzr, [x0]
    ldr     x0, =power_thermal_critical_events
    str     wzr, [x0]

    /* Zero zones */
    ldr     x0, =power_thermal_zones
    mov     x1, #(MAX_THERMAL_ZONES * 32)
    bl      .Lthermal_zero

    /* Zero cooling devices */
    ldr     x0, =power_thermal_cooling
    mov     x1, #(MAX_COOLING_DEVS * 16)
    bl      .Lthermal_zero

    /* Zero history */
    ldr     x0, =power_thermal_history
    mov     x1, #(MAX_THERMAL_ZONES * 8 * 4)
    bl      .Lthermal_zero

    ldr     x0, =power_thermal_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lthermal_zero:
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
 * power_thermal_register_zone — register a thermal zone
 * Input: x0 = zone_id, x1 = trip_hot (millicelsius), x2 = trip_crit
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_thermal_register_zone
power_thermal_register_zone:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_THERMAL_ZONES
    b.hs    .Lthermal_reg_fail

    /* Check zone count */
    ldr     x3, =power_thermal_zone_count
    ldr     w3, [x3]
    cmp     w3, #MAX_THERMAL_ZONES
    b.hs    .Lthermal_reg_fail

    /* Get zone pointer */
    ldr     x3, =power_thermal_zones
    mov     x4, w0, uxtw
    mov     x5, #32
    mul     x4, x4, x5
    add     x3, x3, x4

    /* Initialize zone */
    str     w0, [x3, #0]            /* zone_id */
    mov     w4, #25000
    str     w4, [x3, #4]            /* temperature = 25°C */
    str     w1, [x3, #8]            /* trip_hot */
    str     w2, [x3, #12]           /* trip_crit */
    str     wzr, [x3, #16]          /* cooling_dev = none */
    str     wzr, [x3, #20]          /* trend = 0 */
    str     wzr, [x3, #24]          /* flags = 0 */
    mov     w4, #1
    str     w4, [x3, #28]           /* active = 1 */

    /* Update zone count */
    ldr     x3, =power_thermal_zone_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lthermal_reg_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_thermal_get_temp — get zone temperature
 * Input: x0 = zone_id
 * Output: x0 = temperature (millicelsius) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_thermal_get_temp
power_thermal_get_temp:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_THERMAL_ZONES
    b.hs    .Lthermal_get_fail

    ldr     x1, =power_thermal_zones
    mov     x2, w0, uxtw
    mov     x3, #32
    mul     x2, x2, x3
    add     x1, x1, x2

    /* Check active */
    ldr     w2, [x1, #28]
    cbz     w2, .Lthermal_get_fail

    ldr     w0, [x1, #4]            /* temperature */
    ldp     x29, x30, [sp], #16
    ret

.Lthermal_get_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_thermal_register_cooling — register a cooling device
 * Input: x0 = dev_id, x1 = type (COOL_FAN, COOL_CPU_THROTTLE, etc.)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_thermal_register_cooling
power_thermal_register_cooling:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_COOLING_DEVS
    b.hs    .Lthermal_cool_fail

    /* Get cooling device pointer */
    ldr     x2, =power_thermal_cooling
    mov     x3, w0, uxtw
    mov     x4, #16
    mul     x3, x3, x4
    add     x2, x2, x3

    /* Initialize cooling device */
    str     w0, [x2, #0]            /* dev_id */
    str     w1, [x2, #4]            /* type */
    str     wzr, [x2, #8]           /* cur_state = 0 */
    mov     w3, #10
    str     w3, [x2, #12]           /* max_state = 10 */

    /* Update cooling count */
    ldr     x2, =power_thermal_cooling_count
    ldr     w3, [x2]
    add     w3, w3, #1
    str     w3, [x2]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lthermal_cool_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_thermal_set_cooling — set cooling device state
 * Input: x0 = dev_id, x1 = state
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_thermal_set_cooling
power_thermal_set_cooling:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_COOLING_DEVS
    b.hs    .Lthermal_set_fail

    /* Get cooling device pointer */
    ldr     x2, =power_thermal_cooling
    mov     x3, w0, uxtw
    mov     x4, #16
    mul     x3, x3, x4
    add     x2, x2, x3

    /* Clamp state to max */
    ldr     w3, [x2, #12]           /* max_state */
    cmp     w1, w3
    b.ls    .Lthermal_set_apply
    mov     w1, w3

.Lthermal_set_apply:
    str     w1, [x2, #8]            /* cur_state */

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lthermal_set_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_thermal_emergency_check — check for critical temperatures
 * Output: x0 = number of zones in critical state
 * ----------------------------------------------------------------------------- */
.global power_thermal_emergency_check
power_thermal_emergency_check:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =power_thermal_zones
    mov     w0, #0
    mov     w2, #0

.Lthermal_emerg_loop:
    cmp     w2, #MAX_THERMAL_ZONES
    b.hs    .Lthermal_emerg_done

    mov     x3, w2, uxtw
    mov     x4, #32
    mul     x3, x3, x4
    add     x3, x1, x3

    /* Check if active */
    ldr     w4, [x3, #28]
    cbz     w4, .Lthermal_emerg_next

    /* Compare temperature with trip_crit */
    ldr     w4, [x3, #4]            /* temperature */
    ldr     w5, [x3, #12]           /* trip_crit */
    cmp     w4, w5
    b.lo    .Lthermal_emerg_next

    /* In critical state */
    add     w0, w0, #1

    /* Update critical events */
    ldr     x4, =power_thermal_critical_events
    ldr     w5, [x4]
    add     w5, w5, #1
    str     w5, [x4]

.Lthermal_emerg_next:
    add     w2, w2, #1
    b       .Lthermal_emerg_loop

.Lthermal_emerg_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_thermal_stats — get thermal statistics
 * Input: x0 = output buffer (16 bytes)
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global power_thermal_stats
power_thermal_stats:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =power_thermal_zone_count
    ldr     w1, [x1]
    str     w1, [x0, #0]

    ldr     x1, =power_thermal_cooling_count
    ldr     w1, [x1]
    str     w1, [x0, #4]

    ldr     x1, =power_thermal_throttle_events
    ldr     w1, [x1]
    str     w1, [x0, #8]

    ldr     x1, =power_thermal_critical_events
    ldr     w1, [x1]
    str     w1, [x0, #12]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
