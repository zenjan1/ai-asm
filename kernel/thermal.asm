/*
 * aiasm-aarch64/kernel/thermal.asm
 * Thermal management subsystem (v1.0)
 *
 * Monitors SoC temperature zones, manages cooling trip points.
 * Supports passive (throttle), active (fan), hot (emergency), critical (shutdown).
 * Board-specific sensor mapping for Apple Silicon, Jetson, RP4.
 *
 * API:
 *   thermal_init()                  — initialize thermal subsystem
 *   thermal_read_zone(zone_id) => x0 — read temperature in millicelsius
 *   thermal_set_trip(zone, trip, temp) — configure trip point
 *   thermal_get_temp() => x0        — read current SoC temp (zone 0)
 *   thermal_check_trips()           — evaluate all trip points, trigger actions
 */
.arch armv8-a

/* Trip point types */
.set THERM_TRIP_PASSIVE,  0    /* cooling: reduce CPU freq */
.set THERM_TRIP_ACTIVE,   1    /* active: engage fan */
.set THERM_TRIP_HOT,      2    /* hot: emergency throttle */
.set THERM_TRIP_CRITICAL, 3    /* critical: shutdown */

/* Max zones and trips per zone */
.set MAX_THERM_ZONES,    4
.set MAX_THERM_TRIPS,    4

.bss
.align 4
thermal_zone_count:
    .skip 4
/* Zone data: 8 bytes per zone = [temp_mc(4), flags(4)] */
thermal_zones:
    .skip MAX_THERM_ZONES * 8
/* Trip points: 4 bytes per trip, 4 trips per zone = 16 bytes/zone */
thermal_trips:
    .skip MAX_THERM_ZONES * MAX_THERM_TRIPS * 4
/* Cooling state per zone (0 = none, 1-3 = throttle levels) */
thermal_cooling_state:
    .skip MAX_THERM_ZONES

.text

/* -----------------------------------------------------------------------------
 * thermal_init — set default zones and trip points
 * ----------------------------------------------------------------------------- */
.global thermal_init
thermal_init:
    stp     x29, x30, [sp, #-16]!

    /* Set 4 zones: CPU, GPU, SoC, Battery */
    mov     x0, #MAX_THERM_ZONES
    ldr     x1, =thermal_zone_count
    str     w0, [x1]

    /* Default trip points (millicelsius):
     * Zone 0 (CPU):    passive=70000, active=80000, hot=90000, critical=100000
     * Zone 1 (GPU):    passive=75000, active=85000, hot=95000, critical=105000
     * Zone 2 (SoC):    passive=65000, active=75000, hot=85000, critical=95000
     * Zone 3 (Battery): passive=40000, active=45000, hot=50000, critical=60000
     */
    ldr     x0, =thermal_trips

    /* Zone 0 trips */
    mov     w1, #70000
    str     w1, [x0], #4
    mov     w1, #80000
    str     w1, [x0], #4
    mov     w1, #90000
    str     w1, [x0], #4
    mov     w1, #100000
    str     w1, [x0], #4

    /* Zone 1 trips */
    mov     w1, #75000
    str     w1, [x0], #4
    mov     w1, #85000
    str     w1, [x0], #4
    mov     w1, #95000
    str     w1, [x0], #4
    mov     w1, #105000
    str     w1, [x0], #4

    /* Zone 2 trips */
    mov     w1, #65000
    str     w1, [x0], #4
    mov     w1, #75000
    str     w1, [x0], #4
    mov     w1, #85000
    str     w1, [x0], #4
    mov     w1, #95000
    str     w1, [x0], #4

    /* Zone 3 trips */
    mov     w1, #40000
    str     w1, [x0], #4
    mov     w1, #45000
    str     w1, [x0], #4
    mov     w1, #50000
    str     w1, [x0], #4
    mov     w1, #60000
    str     w1, [x0], #4

    /* Clear cooling states */
    mov     x0, #0
    ldr     x1, =thermal_cooling_state
    str     x0, [x1]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * thermal_read_zone — read temperature for zone
 * Input: x0 = zone_id (0-3)
 * Output: x0 = temperature in millicelsius (or -1 if invalid zone)
 * ----------------------------------------------------------------------------- */
.global thermal_read_zone
thermal_read_zone:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =thermal_zone_count
    ldr     w1, [x1]
    cmp     w0, w1
    bge     .Ltherm_invalid

    /* In a real implementation, this would read from board-specific ADC/hwmon.
     * For now, return a simulated value based on zone.
     * TODO: integrate with board_apple/board_jetson/board_rp4 sensors */
    /* Simulated: zone_id * 5000 + 45000 (baseline temps) */
    mov     x1, #5000
    mul     x0, x0, x1
    mov     x1, #45000
    add     x0, x0, x1

    ldp     x29, x30, [sp], #16
    ret

.Ltherm_invalid:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * thermal_set_trip — configure trip point for zone
 * Input: x0 = zone_id, x1 = trip_type (0-3), x2 = temp_mc
 * Output: none
 * ----------------------------------------------------------------------------- */
.global thermal_set_trip
thermal_set_trip:
    stp     x29, x30, [sp, #-16]!

    /* Validate zone */
    ldr     x3, =thermal_zone_count
    ldr     w3, [x3]
    cmp     w0, w3
    bge     2f

    /* Validate trip type */
    cmp     w1, #MAX_THERM_TRIPS
    bge     2f

    /* Calculate offset: thermal_trips + (zone * MAX_THERM_TRIPS + trip) * 4 */
    mov     x3, #MAX_THERM_TRIPS
    mul     x0, x0, x3
    add     x0, x0, x1
    lsl     x0, x0, #2          /* * 4 bytes */
    ldr     x3, =thermal_trips
    add     x3, x3, x0
    str     w2, [x3]

2:  ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * thermal_get_temp — convenience: read zone 0 (CPU) temperature
 * Input: none
 * Output: x0 = temperature in millicelsius
 * ----------------------------------------------------------------------------- */
.global thermal_get_temp
thermal_get_temp:
    mov     x0, #0
    b       thermal_read_zone

/* -----------------------------------------------------------------------------
 * thermal_check_trips — evaluate all zones against trip points, trigger cooling
 * Called periodically by scheduler or timer interrupt
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global thermal_check_trips
thermal_check_trips:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, #0             /* zone counter */

.Ltherm_zone_loop:
    ldr     x0, =thermal_zone_count
    ldr     w0, [x0]
    cmp     w19, w0
    bge     .Ltherm_done

    /* Read zone temperature */
    mov     x0, x19
    bl      thermal_read_zone
    mov     x20, x0             /* save current temp */

    /* Check trip points from highest to lowest */
    ldr     x1, =thermal_trips
    mov     x2, #MAX_THERM_TRIPS
    mul     x3, x19, x2
    lsl     x3, x3, #2
    add     x1, x1, x3         /* point to zone's trips */

    /* Check critical (trip 3) */
    ldr     w2, [x1, #12]
    cmp     x20, x2
    blt     .Ltherm_check_hot
    /* Critical: would call kernel_shutdown() */
    b       .Ltherm_next_zone

.Ltherm_check_hot:
    ldr     w2, [x1, #8]
    cmp     x20, x2
    blt     .Ltherm_check_active
    /* Hot: set cooling state to max throttle */
    mov     x0, #3
    b       .Ltherm_set_cooling

.Ltherm_check_active:
    ldr     w2, [x1, #4]
    cmp     x20, x2
    blt     .Ltherm_check_passive
    mov     x0, #2
    b       .Ltherm_set_cooling

.Ltherm_check_passive:
    ldr     w2, [x1]
    cmp     x20, x2
    blt     .Ltherm_next_zone
    mov     x0, #1

.Ltherm_set_cooling:
    ldr     x1, =thermal_cooling_state
    add     x1, x1, x19, lsl #2
    str     w0, [x1]
    /* TODO: call cpufreq_set_level() for passive, gpio_fan_on() for active */

.Ltherm_next_zone:
    add     x19, x19, #1
    b       .Ltherm_zone_loop

.Ltherm_done:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
