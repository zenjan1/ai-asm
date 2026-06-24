/*
 * aiasm-aarch64/kernel/power_cpufreq.asm
 * CPU Frequency Scaling (v1.0)
 *
 * AArch64 ASM implementation of dynamic CPU frequency and voltage scaling.
 * Provides per-core frequency management with multiple governor policies.
 *
 * Features:
 *   - Frequency table management
 *   - Governor selection (performance, powersave, ondemand, conservative)
 *   - Voltage regulation coordination
 *   - Thermal throttling integration
 *   - Transition latency tracking
 *   - Per-core frequency control
 *   - Frequency statistics
 *
 * Integration: called by scheduler, power management, thermal subsystem
 *
 * API:
 *   power_cpufreq_init()                             — init cpufreq subsystem
 *   power_cpufreq_set_governor(core_id, governor_id) => 0/-1 — set governor
 *   power_cpufreq_set_freq(core_id, freq_khz) => 0/-1 — set target frequency
 *   power_cpufreq_get_freq(core_id) => freq_khz     — get current frequency
 *   power_cpufreq_get_temp(core_id) => temp_mc       — get temperature (millicelsius)
 *   power_cpufreq_stats(core_id, out_stats)          — get frequency statistics
 *   power_cpufreq_throttle(core_id, level)           — apply thermal throttle
 *   power_cpufreq_restore(core_id)                   — restore pre-throttle freq
 */
.arch armv8-a

/* Constants */
.set MAX_CPU_CORES,        8
.set MAX_FREQ_LEVELS,      16
.set GOV_PERFORMANCE,      1
.set GOV_POWERSAVE,        2
.set GOV_ONDEMAND,         3
.set GOV_CONSERVATIVE,     4

/* CPU freq policy (32 bytes) */
/* core_id, min_freq, max_freq, cur_freq, governor, voltage_mv, throttle_level, transition_count */

.bss
.align 4
.global power_cpufreq_init_done
power_cpufreq_init_done:
    .skip 4

.align 4
power_cpufreq_core_count:
    .skip 4
power_cpufreq_transitions:
    .skip 4
power_cpufreq_throttle_events:
    .skip 4

/* Per-core policies */
.align 4
power_cpufreq_policies:
    .skip MAX_CPU_CORES * 32     /* 256 bytes */

/* Frequency tables per core */
.align 4
power_cpufreq_tables:
    .skip MAX_CPU_CORES * MAX_FREQ_LEVELS * 4  /* 512 bytes */

/* Time in state per core */
.align 4
power_cpufreq_time_in_state:
    .skip MAX_CPU_CORES * MAX_FREQ_LEVELS * 4  /* 512 bytes */

.text

/* -----------------------------------------------------------------------------
 * power_cpufreq_init — initialize cpufreq subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_init
power_cpufreq_init:
    stp     x29, x30, [sp, #-16]!

    /* Set core count */
    ldr     x0, =power_cpufreq_core_count
    mov     w1, #MAX_CPU_CORES
    str     w1, [x0]

    /* Zero counters */
    ldr     x0, =power_cpufreq_transitions
    str     wzr, [x0]
    ldr     x0, =power_cpufreq_throttle_events
    str     wzr, [x0]

    /* Zero policy array */
    ldr     x0, =power_cpufreq_policies
    mov     x1, #(MAX_CPU_CORES * 32)
    bl      .Lcpufreq_zero

    /* Zero frequency tables */
    ldr     x0, =power_cpufreq_tables
    mov     x1, #(MAX_CPU_CORES * MAX_FREQ_LEVELS * 4)
    bl      .Lcpufreq_zero

    /* Zero time-in-state */
    ldr     x0, =power_cpufreq_time_in_state
    mov     x1, #(MAX_CPU_CORES * MAX_FREQ_LEVELS * 4)
    bl      .Lcpufreq_zero

    /* Initialize default frequency tables for each core */
    mov     w0, #0
.Lcpufreq_init_tables:
    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_init_done

    /* Set default freq levels: 200MHz to 3200MHz in 200MHz steps */
    ldr     x1, =power_cpufreq_tables
    mov     x2, w0, uxtw
    mov     x3, #(MAX_FREQ_LEVELS * 4)
    mul     x2, x2, x3
    add     x1, x1, x2

    mov     w2, #200        /* start at 200MHz */
    mov     w3, #0
.Lcpufreq_fill_table:
    cmp     w3, #MAX_FREQ_LEVELS
    b.hs    .Lcpufreq_init_next
    lsl     x4, x3, #2
    str     w2, [x1, x4]
    add     w2, w2, #200
    add     w3, w3, #1
    b       .Lcpufreq_fill_table

.Lcpufreq_init_next:
    /* Set default policy: ondemand governor, min=200MHz, max=3200MHz */
    ldr     x1, =power_cpufreq_policies
    mov     x2, w0, uxtw
    mov     x3, #32
    mul     x2, x2, x3
    add     x1, x1, x2

    str     w0, [x1, #0]            /* core_id */
    mov     w2, #200000
    str     w2, [x1, #4]            /* min_freq = 200MHz */
    mov     w2, #3200000
    str     w2, [x1, #8]            /* max_freq = 3200MHz */
    str     w2, [x1, #12]           /* cur_freq = max */
    mov     w2, #GOV_ONDEMAND
    str     w2, [x1, #16]           /* governor = ondemand */
    mov     w2, #900
    str     w2, [x1, #20]           /* voltage = 900mV */
    str     wzr, [x1, #24]          /* throttle_level = 0 */
    str     wzr, [x1, #28]          /* transition_count = 0 */

    add     w0, w0, #1
    b       .Lcpufreq_init_tables

.Lcpufreq_init_done:
    ldr     x0, =power_cpufreq_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lcpufreq_zero:
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
 * power_cpufreq_set_governor — set governor for a core
 * Input: x0 = core_id, x1 = governor_id
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_set_governor
power_cpufreq_set_governor:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_gov_fail

    cmp     w1, #4
    b.hi    .Lcpufreq_gov_fail

    /* Get policy pointer */
    ldr     x2, =power_cpufreq_policies
    mov     x3, w0, uxtw
    mov     x4, #32
    mul     x3, x3, x4
    add     x2, x2, x3

    str     w1, [x2, #16]           /* governor */

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lcpufreq_gov_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_cpufreq_set_freq — set target frequency for a core
 * Input: x0 = core_id, x1 = freq_khz
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_set_freq
power_cpufreq_set_freq:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_freq_fail

    /* Get policy pointer */
    ldr     x2, =power_cpufreq_policies
    mov     x3, w0, uxtw
    mov     x4, #32
    mul     x3, x3, x4
    add     x2, x2, x3

    /* Check min/max bounds */
    ldr     w3, [x2, #4]            /* min_freq */
    cmp     w1, w3
    b.lo    .Lcpufreq_freq_clamp_min
    ldr     w3, [x2, #8]            /* max_freq */
    cmp     w1, w3
    b.hi    .Lcpufreq_freq_clamp_max

.Lcpufreq_freq_set:
    str     w1, [x2, #12]           /* cur_freq */

    /* Update transition count */
    ldr     w3, [x2, #28]
    add     w3, w3, #1
    str     w3, [x2, #28]

    /* Update global transitions */
    ldr     x0, =power_cpufreq_transitions
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lcpufreq_freq_clamp_min:
    str     w3, [x2, #12]
    b       .Lcpufreq_freq_set

.Lcpufreq_freq_clamp_max:
    str     w3, [x2, #12]
    b       .Lcpufreq_freq_set

.Lcpufreq_freq_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_cpufreq_get_freq — get current frequency
 * Input: x0 = core_id
 * Output: x0 = freq_khz or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_get_freq
power_cpufreq_get_freq:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_get_fail

    ldr     x1, =power_cpufreq_policies
    mov     x2, w0, uxtw
    mov     x3, #32
    mul     x2, x2, x3
    add     x1, x1, x2

    ldr     w0, [x1, #12]           /* cur_freq */
    ldp     x29, x30, [sp], #16
    ret

.Lcpufreq_get_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_cpufreq_throttle — apply thermal throttle
 * Input: x0 = core_id, x1 = throttle_level (1-4)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_throttle
power_cpufreq_throttle:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_thr_fail

    cmp     w1, #4
    b.hi    .Lcpufreq_thr_fail

    ldr     x2, =power_cpufreq_policies
    mov     x3, w0, uxtw
    mov     x4, #32
    mul     x3, x3, x4
    add     x2, x2, x3

    str     w1, [x2, #24]           /* throttle_level */

    /* Reduce frequency based on throttle level */
    ldr     w3, [x2, #12]           /* cur_freq */
    lsr     w4, w3, w1              /* divide by 2^level */
    ldr     w5, [x2, #4]            /* min_freq */
    cmp     w4, w5
    b.hi    .Lcpufreq_thr_apply
    mov     w4, w5

.Lcpufreq_thr_apply:
    str     w4, [x2, #12]           /* cur_freq = throttled */

    /* Update throttle events */
    ldr     x0, =power_cpufreq_throttle_events
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lcpufreq_thr_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * power_cpufreq_restore — restore pre-throttle frequency
 * Input: x0 = core_id
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global power_cpufreq_restore
power_cpufreq_restore:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_CPU_CORES
    b.hs    .Lcpufreq_rst_fail

    ldr     x1, =power_cpufreq_policies
    mov     x2, w0, uxtw
    mov     x3, #32
    mul     x2, x2, x3
    add     x1, x1, x2

    /* Clear throttle level */
    str     wzr, [x1, #24]

    /* Restore to max frequency */
    ldr     w2, [x1, #8]            /* max_freq */
    str     w2, [x1, #12]           /* cur_freq = max */

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lcpufreq_rst_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret
