/*
 * aiasm-aarch64/kernel/device_enum.asm
 * Device Enumeration & Discovery (v1.0)
 *
 * AArch64 ASM implementation of device enumeration (PCIe-like discovery).
 * Scans buses for devices, reads vendor/device IDs, allocates resources,
 * and binds devices to drivers.
 *
 * Integration: called from device.asm during boot for hardware initialization
 *
 * API:
 *   device_enum_init()                          — init enumeration subsystem
 *   device_enum_scan_bus(bus_id)                — scan a bus for devices
 *   device_enum_get_info(dev_id, info_ptr)      — get device info
 *   device_enum_bind_driver(dev_id, driver_id)  — bind device to driver
 *   device_enum_get_count() => n_devices        — get number of discovered devices
 */
.arch armv8-a

/* Constants */
.set DEV_ENUM_MAX_DEVICES,  64
.set DEV_ENUM_MAX_BUSES,    8
.set DEV_ENUM_MAX_DRIVERS,  32

/* Device info structure (48 bytes) */
/* 0:  device_id    (4) */
/* 4:  bus_id       (4) */
/* 8:  vendor_id    (2) */
/* 10: device_id_hw (2) */
/* 12: class_code   (4) */
/* 16: irq          (4) */
/* 20: mmio_base    (8) */
/* 28: mmio_size    (8) */
/* 36: driver_id    (4) */
/* 40: flags        (4) */
/* 44: reserved     (4) */

.bss
.align 4
.global device_enum_init_done
device_enum_init_done:
    .skip 4

.align 4
device_enum_devs:
    .skip DEV_ENUM_MAX_DEVICES * 48   /* 3072 bytes */
device_enum_dev_count:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * device_enum_init — initialize device enumeration subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global device_enum_init
device_enum_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero device table */
    ldr     x0, =device_enum_devs
    mov     x1, #(DEV_ENUM_MAX_DEVICES * 48)
1:  cbz     x1, 2f
    strb    wzr, [x0], #1
    sub     x1, x1, #1
    b       1b
2:
    ldr     x0, =device_enum_dev_count
    str     wzr, [x0]

    ldr     x0, =device_enum_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * device_enum_scan_bus — scan a bus for devices
 * Input: x0 = bus ID (0-7)
 * Output: x0 = number of devices found, or -1 (error)
 *
 * In real hardware: would probe PCI config space or parse device tree.
 * Here: simulates discovering devices.
 * ----------------------------------------------------------------------------- */
.global device_enum_scan_bus
device_enum_scan_bus:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* bus_id */

    /* Validate bus ID */
    cmp     x0, #DEV_ENUM_MAX_BUSES
    b.hs    .Ldev_enum_scan_fail

    /* Get current device count */
    ldr     x3, =device_enum_dev_count
    ldr     w3, [x3]

    /* Simulate discovering 2 devices per bus */
    mov     w20, #0             /* devices found this scan */

.Ldev_enum_add_dev1:
    /* Check if table full */
    cmp     w3, #DEV_ENUM_MAX_DEVICES
    b.hs    .Ldev_enum_scan_done

    /* Add simulated device 1 */
    ldr     x4, =device_enum_devs
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    str     w3, [x4, #0]            /* device_id = index */
    str     w19, [x4, #4]           /* bus_id */
    mov     w5, #0x1234
    strh    w5, [x4, #8]            /* vendor_id */
    mov     w5, #0x5678
    strh    w5, [x4, #10]           /* device_id_hw */
    mov     w5, #0x020000           /* class: network controller */
    str     w5, [x4, #12]           /* class_code */
    mov     w5, #32                 /* IRQ 32 */
    str     w5, [x4, #16]           /* irq */
    mov     x5, #0x10000000         /* MMIO base */
    str     x5, [x4, #20]           /* mmio_base */
    mov     x5, #0x1000             /* MMIO size 4KB */
    str     x5, [x4, #28]           /* mmio_size */

    add     w3, w3, #1
    add     w20, w20, #1

.Ldev_enum_add_dev2:
    /* Check if table full */
    cmp     w3, #DEV_ENUM_MAX_DEVICES
    b.hs    .Ldev_enum_scan_done

    /* Add simulated device 2 */
    ldr     x4, =device_enum_devs
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    str     w3, [x4, #0]
    str     w19, [x4, #4]
    mov     w5, #0xABCD
    strh    w5, [x4, #8]
    mov     w5, #0xEF01
    strh    w5, [x4, #10]
    mov     w5, #0x010000           /* class: storage controller */
    str     w5, [x4, #12]
    mov     w5, #33
    str     w5, [x4, #16]
    mov     x5, #0x20000000
    str     x5, [x4, #20]
    mov     x5, #0x2000
    str     x5, [x4, #28]

    add     w3, w3, #1
    add     w20, w20, #1

    /* Update device count */
    ldr     x4, =device_enum_dev_count
    str     w3, [x4]

.Ldev_enum_scan_done:
    mov     x0, w20
    b       .Ldev_enum_scan_ret

.Ldev_enum_scan_fail:
    mov     x0, #-1

.Ldev_enum_scan_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * device_enum_get_info — get device information
 * Input: x0 = device ID, x1 = output buffer pointer
 * Output: x0 = 0 (ok), -1 (invalid device)
 * ----------------------------------------------------------------------------- */
.global device_enum_get_info
device_enum_get_info:
    stp     x29, x30, [sp, #-16]!

    /* Validate device ID */
    ldr     x2, =device_enum_dev_count
    ldr     w2, [x2]
    cmp     x0, x2
    b.hs    .Ldev_enum_info_fail

    /* Copy device info to output buffer */
    ldr     x2, =device_enum_devs
    mov     x3, #48
    mul     x3, x0, x3
    add     x2, x2, x3

    /* Copy 48 bytes */
    mov     x3, #48
.Ldev_enum_info_copy:
    cbz     x3, .Ldev_enum_info_done
    ldrb    w4, [x2], #1
    strb    w4, [x1], #1
    sub     x3, x3, #1
    b       .Ldev_enum_info_copy

.Ldev_enum_info_done:
    mov     x0, #0
    b       .Ldev_enum_info_ret

.Ldev_enum_info_fail:
    mov     x0, #-1

.Ldev_enum_info_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * device_enum_bind_driver — bind a device to a driver
 * Input: x0 = device ID, x1 = driver ID
 * Output: x0 = 0 (ok), -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global device_enum_bind_driver
device_enum_bind_driver:
    stp     x29, x30, [sp, #-16]!

    /* Validate device ID */
    ldr     x2, =device_enum_dev_count
    ldr     w2, [x2]
    cmp     x0, x2
    b.hs    .Ldev_enum_bind_fail

    /* Store driver_id in device descriptor (offset 36) */
    ldr     x2, =device_enum_devs
    mov     x3, #48
    mul     x3, x0, x3
    add     x2, x2, x3
    str     w1, [x2, #36]

    mov     x0, #0
    b       .Ldev_enum_bind_ret

.Ldev_enum_bind_fail:
    mov     x0, #-1

.Ldev_enum_bind_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * device_enum_get_count — get number of discovered devices
 * Output: x0 = device count
 * ----------------------------------------------------------------------------- */
.global device_enum_get_count
device_enum_get_count:
    ldr     x0, =device_enum_dev_count
    ldr     w0, [x0]
    ret
