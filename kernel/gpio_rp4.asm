/* gpio_rp4.asm: BCM2711 GPIO controller driver (v10.0) */

.global gpio_rp4_init
.global gpio_rp4_set
.global gpio_rp4_get
.global gpio_rp4_set_direction
.global gpio_rp4_get_direction
.global gpio_rp4_set_pull
.global gpio_rp4_enable_irq
.global gpio_rp4_detect_event

/* BCM2711 GPIO register offsets */
.set GPIO_BASE,       0xFE200000
.set GPFSEL0,         0x00
.set GPFSEL1,         0x04
.set GPFSEL2,         0x08
.set GPFSEL3,         0x0C
.set GPFSEL4,         0x10
.set GPFSEL5,         0x14
.set GPSET0,          0x1C
.set GPSET1,          0x20
.set GPCLR0,          0x28
.set GPCLR1,          0x2C
.set GPLEV0,          0x34
.set GPLEV1,          0x38
.set GPEDS0,          0x40
.set GPEDS1,          0x44
.set GPREN0,          0x4C
.set GPREN1,          0x50
.set GPFEN0,          0x58
.set GPFEN1,          0x5C
.set GPHEN0,          0x64
.set GPHEN1,          0x68
.set GPLEN0,          0x70
.set GPLEN1,          0x74
.set GPAREN0,         0x7C
.set GPAREN1,         0x80
.set GPHEN_REG0,      0x88
.set GPHEN_REG1,      0x8C
.set GPPUD,           0x94
.set GPPUDCLK0,       0x98
.set GPPUDCLK1,       0x9C

/* GPIO directions */
.set GPIO_INPUT,      0
.set GPIO_OUTPUT,     1
.set GPIO_ALT0,       4
.set GPIO_ALT1,       5
.set GPIO_ALT2,       6
.set GPIO_ALT3,       7
.set GPIO_ALT4,       3
.set GPIO_ALT5,       2

/* Pull states */
.set GPIO_PULL_OFF,   0
.set GPIO_PULL_DOWN,  1
.set GPIO_PULL_UP,    2

/* GPIO pin count */
.set GPIO_MAX_PIN,    54

.bss
gpio_direction_table:
    .skip 64    /* 54 pins * 1 byte + padding */

.text

/* gpio_rp4_init() — initialize GPIO controller
   Returns: 0=success, -1=failure */
gpio_rp4_init:
    /* Set all GPIOs to input (safe default) */
    mov  x0, GPIO_BASE
    mov  w1, #0
    str  w1, [x0, #GPFSEL0]
    str  w1, [x0, #GPFSEL1]
    str  w1, [x0, #GPFSEL2]
    str  w1, [x0, #GPFSEL3]
    str  w1, [x0, #GPFSEL4]
    str  w1, [x0, #GPFSEL5]

    /* Clear all event detect status */
    str  w1, [x0, #GPEDS0]
    str  w1, [x0, #GPEDS1]

    /* Disable all pulls */
    str  w1, [x0, #GPREN0]
    str  w1, [x0, #GPREN1]
    str  w1, [x0, #GPFEN0]
    str  w1, [x0, #GPFEN1]
    str  w1, [x0, #GPHEN0]
    str  w1, [x0, #GPHEN1]
    str  w1, [x0, #GPLEN0]
    str  w1, [x0, #GPLEN1]
    str  w1, [x0, #GPAREN0]
    str  w1, [x0, #GPAREN1]
    str  w1, [x0, #GPHEN_REG0]
    str  w1, [x0, #GPHEN_REG1]

    /* Set pull to off */
    mov  w1, #GPIO_PULL_OFF
    str  w1, [x0, #GPPUD]
    /* Delay for pud setup */
    mov  x1, #150
1:  subs x1, x1, #1
    b.ne 1b

    str  wzr, [x0, #GPPUDCLK0]
    str  wzr, [x0, #GPPUDCLK1]

    /* Clear direction table */
    adrp x0, gpio_direction_table
    add  x0, x0, :lo12:gpio_direction_table
    mov  x1, #64
    mov  x2, xzr
2:  strb w2, [x0], #1
    subs x1, x1, #1
    cbnz x1, 2b

    mov  x0, #0
    ret

/* gpio_rp4_set_direction(pin, direction)
   x0=pin, x1=direction (0=input, 1=output, 2-7=alt) */
gpio_rp4_set_direction:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_set_fail

    /* Calculate register offset: each pin uses 3 bits */
    mov  x2, x0
    mov  x3, #3
    udiv x4, x2, x3      /* word index */
    msub x5, x4, x3, x2  /* bit offset within word */
    lsl  x5, x5, #1      /* each function field is 3 bits */

    /* Get FSEL register address */
    mov  x6, GPIO_BASE
    lsl  x7, x4, #2      /* word index * 4 */
    add  x7, x7, #GPFSEL0
    add  x6, x6, x7

    /* Read-modify-write */
    ldr  w7, [x6]
    mov  w8, #7
    lsl  w8, w8, x5       /* mask = 7 << shift */
    mvn  w8, w8           /* invert mask */
    and  w7, w7, w8       /* clear bits */
    lsl  w9, w1, x5       /* value << shift */
    orr  w7, w7, w9
    str  w7, [x6]

    /* Update direction table */
    adrp x6, gpio_direction_table
    add  x6, x6, :lo12:gpio_direction_table
    strb w1, [x6, x0]

    mov  x0, #0
    ret

gpio_set_fail:
    mov  x0, #-1
    ret

/* gpio_rp4_set(pin, value)
   x0=pin, x1=value (0 or 1) */
gpio_rp4_set:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    mov  x2, GPIO_BASE
    cmp  x1, #0
    b.eq gpio_clear
    /* Set pin */
    mov  x3, #1
    lsl  x3, x3, x0
    str  x3, [x2, #GPSET0]
    mov  x0, #0
    ret

gpio_clear:
    mov  x3, #1
    lsl  x3, x3, x0
    str  x3, [x2, #GPCLR0]
    mov  x0, #0
    ret

/* gpio_rp4_get(pin) — read pin value
   x0=pin, returns value in x0 */
gpio_rp4_get:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    mov  x2, GPIO_BASE
    mov  x3, #1
    lsl  x3, x3, x0
    ldr  x4, [x2, #GPLEV0]
    and  x4, x4, x3
    cbnz x4, gpio_get_high
    mov  x0, #0
    ret

gpio_get_high:
    mov  x0, #1
    ret

/* gpio_rp4_set_pull(pin, pull)
   x0=pin, x1=pull (0=off, 1=down, 2=up) */
gpio_rp4_set_pull:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    mov  x2, GPIO_BASE
    /* Set pud register */
    str  w1, [x2, #GPPUD]

    /* Delay */
    mov  x3, #150
1:  subs x3, x3, #1
    b.ne 1b

    /* Clock the pud */
    mov  x3, #1
    lsl  x3, x3, x0
    str  x3, [x2, #GPPUDCLK0]

    /* Delay */
    mov  x3, #150
2:  subs x3, x3, #1
    b.ne 2b

    /* Clear pud */
    str  wzr, [x2, #GPPUD]
    str  wzr, [x2, #GPPUDCLK0]

    mov  x0, #0
    ret

/* gpio_rp4_get_direction(pin) — return direction for pin */
gpio_rp4_get_direction:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    adrp x1, gpio_direction_table
    add  x1, x1, :lo12:gpio_direction_table
    ldrb w0, [x1, x0]
    ret

/* gpio_rp4_enable_irq(pin, edge)
   x0=pin, x1=edge (0=rising, 1=falling, 2=high, 3=low) */
gpio_rp4_enable_irq:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    mov  x2, GPIO_BASE
    mov  x3, #1
    lsl  x3, x3, x0

    cbz  x1, irq_rising
    cmp  x1, #1
    b.eq irq_falling
    cmp  x1, #2
    b.eq irq_high
    cmp  x1, #3
    b.eq irq_low
    b    gpio_op_fail

irq_rising:
    ldr  w4, [x2, #GPREN0]
    orr  w4, w4, w3
    str  w4, [x2, #GPREN0]
    b    gpio_irq_done

irq_falling:
    ldr  w4, [x2, #GPFEN0]
    orr  w4, w4, w3
    str  w4, [x2, #GPFEN0]
    b    gpio_irq_done

irq_high:
    ldr  w4, [x2, #GPHEN0]
    orr  w4, w4, w3
    str  w4, [x2, #GPHEN0]
    b    gpio_irq_done

irq_low:
    ldr  w4, [x2, #GPLEN0]
    orr  w4, w4, w3
    str  w4, [x2, #GPLEN0]
    b    gpio_irq_done

gpio_irq_done:
    mov  x0, #0
    ret

/* gpio_rp4_detect_event(pin) — check if event occurred on pin */
gpio_rp4_detect_event:
    cmp  x0, #GPIO_MAX_PIN
    b.ge gpio_op_fail

    mov  x2, GPIO_BASE
    mov  x3, #1
    lsl  x3, x3, x0
    ldr  x4, [x2, #GPEDS0]
    and  x4, x4, x3
    cbnz x4, gpio_event_set
    mov  x0, #0
    ret

gpio_event_set:
    /* Clear event detect */
    str  x3, [x2, #GPEDS0]
    mov  x0, #1
    ret

gpio_op_fail:
    mov  x0, #-1
    ret
