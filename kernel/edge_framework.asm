/* edge_framework.asm: Edge computing device framework (v13.0) */

.global edge_init
.global edge_register_device
.global edge_discover_devices
.global edge_config_device
.global edge_read_sensor
.global edge_control_actuator
.global edge_get_status

/* Edge device types */
#define EDGE_SENSOR      0
#define EDGE_ACTUATOR    1
#define EDGE_CONTROLLER  2
#define EDGE_GATEWAY     3

/* Device states */
#define DEVICE_INIT      0
#define DEVICE_ONLINE    1
#define DEVICE_OFFLINE   2
#define DEVICE_ERROR     3

/* Device limits */
#define MAX_EDGE_DEVICES  32
#define DEVICE_NAME_LEN   32
#define DEVICE_META_LEN   64

/* Device control block */
#define DCB_SIZE        128
#define DCB_ID          0
#define DCB_TYPE        4
#define DCB_NAME        8
#define DCB_STATE       12
#define DCB_GPIO        16
#define DCB_SAMPLING    20
#define DCB_RESOLUTION  24
#define DCB_LAST_VALUE  28
#define DCB_TIMESTAMP   32
#define DCB_ERROR_COUNT 36
#define DCB_META        40

/* Sensor types */
#define SENSOR_TEMP      0
#define SENSOR_HUMIDITY  1
#define SENSOR_PRESSURE  2
#define SENSOR_LIGHT     3
#define SENSOR_ACCEL     4
#define SENSOR_GYRO      5
#define SENSOR_GPS       6
#define SENSOR_CAMERA    7

/* GPIO base for edge devices */
.set GPIO_BASE,         0xFE200000
.set GPLEV0,            0x34
.set GPSET0,            0x1C
.set GPCLR0,            0x28

.bss
edge_initialized:
    .skip 4
edge_device_count:
    .skip 4
edge_device_table:
    .skip MAX_EDGE_DEVICES * DCB_SIZE
edge_scan_buffer:
    .skip 256

.text

/* edge_init() — initialize edge computing framework
   Returns: 0=success, -1=failure */
edge_init:
    stp  x29, x30, [sp, #-16]!

    /* Clear device table */
    adrp x0, edge_device_table
    add  x0, x0, :lo12:edge_device_table
    mov  x1, #(MAX_EDGE_DEVICES * DCB_SIZE)
    mov  x2, xzr
1:  strb w2, [x0], #1
    subs x1, x1, #1
    cbnz x1, 1b

    /* Reset device count */
    adrp x0, edge_device_count
    add  x0, x0, :lo12:edge_device_count
    str  wzr, [x0]

    /* Mark initialized */
    adrp x0, edge_initialized
    add  x0, x0, :lo12:edge_initialized
    mov  w1, #1
    str  w1, [x0]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

/* edge_register_device(type, name_ptr, gpio_pin)
   x0=type, x1=name, x2=gpio_pin
   Returns: device_id or -1 */
edge_register_device:
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    mov  x19, x0          /* type */
    mov  x20, x1          /* name ptr */
    mov  x4, x2           /* gpio pin */

    /* Check initialized */
    adrp x0, edge_initialized
    add  x0, x0, :lo12:edge_initialized
    ldr  w1, [x0]
    cbz  w1, edge_reg_fail

    /* Check max devices */
    adrp x0, edge_device_count
    add  x0, x0, :lo12:edge_device_count
    ldr  w1, [x0]
    cmp  w1, #MAX_EDGE_DEVICES
    b.ge edge_reg_fail

    /* Allocate slot */
    mov  x2, w1
    add  w1, w1, #1
    str  w1, [x0]

    /* Get device entry */
    adrp x3, edge_device_table
    add  x3, x3, :lo12:edge_device_table
    mov  x5, #DCB_SIZE
    mul  x5, x2, x5
    add  x3, x3, x5

    /* Set fields */
    str  w19, [x3, #DCB_ID]      /* device id */
    str  w19, [x3, #DCB_TYPE]    /* type */
    str  w4,  [x3, #DCB_GPIO]    /* gpio pin */
    mov  w5, #DEVICE_ONLINE
    str  w5, [x3, #DCB_STATE]
    str  wzr, [x3, #DCB_SAMPLING]
    str  wzr, [x3, #DCB_RESOLUTION]
    str  wzr, [x3, #DCB_LAST_VALUE]
    str  wzr, [x3, #DCB_TIMESTAMP]
    str  wzr, [x3, #DCB_ERROR_COUNT]

    /* Copy name */
    add  x5, x3, #DCB_NAME
    mov  x6, x20
    mov  x7, #0
edge_reg_copy_name:
    ldrb w8, [x6, x7]
    strb w8, [x5, x7]
    cbz  w8, edge_reg_name_done
    add  x7, x7, #1
    cmp  x7, #DEVICE_NAME_LEN
    b.lo edge_reg_copy_name
edge_reg_name_done:
    strb wzr, [x5, x7]

    /* Return device_id */
    mov  x0, x2

    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

edge_reg_fail:
    mov  x0, #-1
    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* edge_discover_devices(buf_ptr, max_count)
   x0=buffer, x1=max_count
   Returns: number of devices found */
edge_discover_devices:
    stp  x29, x30, [sp, #-16]!

    adrp x2, edge_device_table
    add  x2, x2, :lo12:edge_device_table
    adrp x3, edge_device_count
    add  x3, x3, :lo12:edge_device_count
    ldr  w3, [x3]

    cmp  x3, x1
    b.lt edge_disc_no_cap
    mov  x3, x1
edge_disc_no_cap:
    mov  x4, xzr
    mov  x5, x0

edge_disc_loop:
    cmp  x4, x3
    b.ge edge_disc_done

    adrp x6, edge_device_table
    add  x6, x6, :lo12:edge_device_table
    mov  x7, #DCB_SIZE
    mul  x7, x4, x7
    add  x6, x6, x7

    /* Write: id(4) + type(4) + state(4) + name(32) = 44 bytes */
    str  w4, [x5, #0]
    ldr  w7, [x6, #DCB_TYPE]
    str  w7, [x5, #4]
    ldr  w7, [x6, #DCB_STATE]
    str  w7, [x5, #8]

    /* Copy name */
    add  x8, x6, #DCB_NAME
    add  x9, x5, #12
    mov  x10, #0
edge_disc_copy:
    ldrb w11, [x8, x10]
    strb w11, [x9, x10]
    cbz  w11, edge_disc_name_done
    add  x10, x10, #1
    cmp  x10, #32
    b.lo edge_disc_copy
edge_disc_name_done:
    strb wzr, [x9, x10]

    add  x5, x5, #44
    add  x4, x4, #1
    b    edge_disc_loop

edge_disc_done:
    mov  x0, x4
    ldp  x29, x30, [sp], #16
    ret

/* edge_config_device(device_id, sampling_rate, resolution)
   x0=device_id, x1=sampling_rate, x2=resolution
   Returns: 0=success, -1=failure */
edge_config_device:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_EDGE_DEVICES
    b.ge edge_cfg_fail

    adrp x3, edge_device_table
    add  x3, x3, :lo12:edge_device_table
    mov  x4, #DCB_SIZE
    mul  x4, x0, x4
    add  x3, x3, x4

    str  w1, [x3, #DCB_SAMPLING]
    str  w2, [x3, #DCB_RESOLUTION]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

edge_cfg_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* edge_read_sensor(device_id)
   x0=device_id
   Returns: sensor value in x0, or -1 */
edge_read_sensor:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_EDGE_DEVICES
    b.ge edge_read_fail

    adrp x1, edge_device_table
    add  x1, x1, :lo12:edge_device_table
    mov  x2, #DCB_SIZE
    mul  x2, x0, x2
    add  x1, x1, x2

    /* Check type is sensor */
    ldr  w3, [x1, #DCB_TYPE]
    cmp  w3, #EDGE_SENSOR
    b.ne edge_read_fail

    /* Check state */
    ldr  w3, [x1, #DCB_STATE]
    cmp  w3, #DEVICE_ONLINE
    b.ne edge_read_fail

    /* Read from GPIO pin */
    ldr  w3, [x1, #DCB_GPIO]
    cbz  w3, edge_read_simulate

    /* Read GPIO value */
    mov  x4, GPIO_BASE
    mov  x5, #1
    lsl  x5, x5, x3
    ldr  w6, [x4, #GPLEV0]
    and  w6, w6, w5
    cbnz w6, edge_read_high
    mov  x0, #0
    b    edge_read_store

edge_read_high:
    mov  x0, #1
    b    edge_read_store

edge_read_simulate:
    /* Simulate sensor reading based on device type */
    ldr  w3, [x1, #DCB_TYPE]
    cmp  w3, #SENSOR_TEMP
    b.eq edge_read_temp
    cmp  w3, #SENSOR_HUMIDITY
    b.eq edge_read_humidity
    cmp  w3, #SENSOR_PRESSURE
    b.eq edge_read_pressure
    cmp  w3, #SENSOR_LIGHT
    b.eq edge_read_light
    /* Default: return last value */
    ldr  x0, [x1, #DCB_LAST_VALUE]
    b    edge_read_done

edge_read_temp:
    mov  x0, #22        /* 22C simulated */
    b    edge_read_store
edge_read_humidity:
    mov  x0, #65        /* 65% simulated */
    b    edge_read_store
edge_read_pressure:
    mov  x0, #1013      /* 1013 hPa simulated */
    b    edge_read_store
edge_read_light:
    mov  x0, #500       /* 500 lux simulated */
    b    edge_read_store

edge_read_store:
    /* Update last value and timestamp */
    str  w0, [x1, #DCB_LAST_VALUE]
    mov  w3, #0         /* placeholder timestamp */
    str  w3, [x1, #DCB_TIMESTAMP]

edge_read_done:
    ldp  x29, x30, [sp], #16
    ret

edge_read_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* edge_control_actuator(device_id, value)
   x0=device_id, x1=value
   Returns: 0=success, -1=failure */
edge_control_actuator:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_EDGE_DEVICES
    b.ge edge_ctrl_fail

    adrp x2, edge_device_table
    add  x2, x2, :lo12:edge_device_table
    mov  x3, #DCB_SIZE
    mul  x3, x0, x3
    add  x2, x2, x3

    /* Check type is actuator */
    ldr  w4, [x2, #DCB_TYPE]
    cmp  w4, #EDGE_ACTUATOR
    b.ne edge_ctrl_fail

    /* Check state */
    ldr  w4, [x2, #DCB_STATE]
    cmp  w4, #DEVICE_ONLINE
    b.ne edge_ctrl_fail

    /* Write to GPIO */
    ldr  w4, [x2, #DCB_GPIO]
    cbz  w4, edge_ctrl_no_gpio

    mov  x5, GPIO_BASE
    mov  x6, #1
    lsl  x6, x6, x4

    cbz  x1, edge_ctrl_clear
    str  x6, [x5, #GPSET0]
    b    edge_ctrl_ok

edge_ctrl_clear:
    str  x6, [x5, #GPCLR0]
    b    edge_ctrl_ok

edge_ctrl_no_gpio:
    /* Store value anyway */
edge_ctrl_ok:
    str  w1, [x2, #DCB_LAST_VALUE]
    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

edge_ctrl_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* edge_get_status(device_id, buf_ptr)
   x0=device_id, x1=buffer (48 bytes)
   Returns: 0=success, -1=failure */
edge_get_status:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_EDGE_DEVICES
    b.ge edge_status_fail

    adrp x2, edge_device_table
    add  x2, x2, :lo12:edge_device_table
    mov  x3, #DCB_SIZE
    mul  x3, x0, x3
    add  x2, x2, x3

    /* Write: id(4) + type(4) + state(4) + gpio(4) + last_value(4) + errors(4) + name(32) = 56 bytes */
    ldr  w4, [x2, #DCB_ID]
    str  w4, [x1, #0]
    ldr  w4, [x2, #DCB_TYPE]
    str  w4, [x1, #4]
    ldr  w4, [x2, #DCB_STATE]
    str  w4, [x1, #8]
    ldr  w4, [x2, #DCB_GPIO]
    str  w4, [x1, #12]
    ldr  w4, [x2, #DCB_LAST_VALUE]
    str  w4, [x1, #16]
    ldr  w4, [x2, #DCB_ERROR_COUNT]
    str  w4, [x1, #20]

    /* Copy name */
    add  x4, x2, #DCB_NAME
    add  x5, x1, #24
    mov  x6, #0
edge_status_copy:
    ldrb w7, [x4, x6]
    strb w7, [x5, x6]
    cbz  w7, edge_status_done
    add  x6, x6, #1
    cmp  x6, #32
    b.lo edge_status_copy
edge_status_done:
    strb wzr, [x5, x6]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

edge_status_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret
