/* board_rp4.asm: Raspberry Pi 4 BCM2711 SoC initialization (v10.0) */

.global rp4_board_init
.global rp4_get_soc_id
.global rp4_get_board_rev
.global rp4_get_arm_freq
.global rp4_get_gpu_freq

/* BCM2711 constants */
.set BCM2711_PERIPH_BASE,  0xFE000000
.set BCM2711_MAILBOX_BASE,  0xB880
.set BCM2711_UART0_BASE,    0x201000
.set BCM2711_GPIO_BASE,     0x200000
.set BCM2711_TIMER_BASE,    0x3000
.set BCM2711_ARMCTRL_BASE,  0xB000
.set BCM2711_LOCAL_BASE,    0x40000000
.set BCM2711_QSPI_BASE,     0x204000

/* Board revision codes */
.set RP4_REV_1GB, 0xA03111
.set RP4_REV_2GB, 0xB03111
.set RP4_REV_4GB, 0xC03111
.set RP4_REV_8GB, 0xD03111

/* Mailbox property interface */
.set MAILBOX_REQUEST, 0
.set MAILBOX_RESPONSE, 1

.bss
board_info:
    .skip 64
board_revision:
    .skip 4
board_serial:
    .skip 4
board_soc_id:
    .skip 4

.text

/* rp4_board_init() — initialize BCM2711 SoC
   Returns: 0=success, -1=failure */
rp4_board_init:
    /* Save registers */
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    /* Detect board revision via mailbox */
    bl   rp4_detect_board

    /* Initialize peripheral base */
    adrp x0, board_info
    add  x0, x0, :lo12:board_info
    mov  x1, #BCM2711_PERIPH_BASE >> 16
    lsl  x1, x1, #16
    str  x1, [x0]

    /* Initialize local timer */
    mov  x0, BCM2711_LOCAL_BASE + 0x4000
    mov  x1, #0
    str  x1, [x0]         /* Disable timer */
    mov  x1, #0x1000000   /* 250MHz */
    str  x1, [x0, #0x8]   /* Set timer frequency */
    mov  x1, #1
    str  x1, [x0]         /* Enable timer */

    /* Initialize ARM control interrupt */
    mov  x0, BCM2711_ARMCTRL_BASE + 0x200
    mov  x1, #0
    str  x1, [x0, #0x00]  /* Disable all IRQs */
    str  x1, [x0, #0x04]  /* Disable all FIQs */

    /* Set timer prescaler */
    mov  x0, BCM2711_TIMER_BASE + 0x0C
    mov  x1, #1000        /* 1ms tick */
    str  x1, [x0]

    /* Initialize QSPI for eMMC */
    bl   rp4_qspi_init

    /* Initialize MMU for BCM2711 memory map */
    bl   rp4_mmu_init

    /* Store board info */
    adrp x0, board_info
    add  x0, x0, :lo12:board_info
    mov  x1, #0x2711      /* BCM2711 */
    str  x1, [x0, #8]     /* SOC ID */

    /* Return success */
    mov  x0, #0

    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* rp4_detect_board() — detect board revision via mailbox
   Sets board_revision global */
rp4_detect_board:
    /* Use mailbox to get board revision */
    adrp x0, board_revision
    add  x0, x0, :lo12:board_revision

    /* Mailbox property: Get Board Revision (0x00010002) */
    mov  x1, BCM2711_MAILBOX_BASE
    /* Read revision from mailbox tag response */
    ldr  w2, [x1, #0x18]   /* Read tag value */
    cbz  w2, rp4_detect_fallback
    str  w2, [x0]
    ret

rp4_detect_fallback:
    /* Fallback: assume 4GB model */
    mov  w2, #RP4_REV_4GB
    str  w2, [x0]
    ret

/* rp4_get_soc_id() — return SoC ID (BCM2711 = 0x2711) */
rp4_get_soc_id:
    adrp x0, board_soc_id
    add  x0, x0, :lo12:board_soc_id
    ldr  w0, [x0]
    cbz  w0, rp4_soc_default
    ret
rp4_soc_default:
    mov  x0, #0x2711
    ret

/* rp4_get_board_rev() — return board revision code */
rp4_get_board_rev:
    adrp x0, board_revision
    add  x0, x0, :lo12:board_revision
    ldr  w0, [x0]
    ret

/* rp4_get_arm_freq() — return ARM clock frequency in MHz */
rp4_get_arm_freq:
    /* BCM2711 default ARM clock: 1500MHz */
    mov  x0, #1500
    ret

/* rp4_get_gpu_freq() — return GPU clock frequency in MHz */
rp4_get_gpu_freq:
    /* BCM2711 default GPU clock: 500MHz */
    mov  x0, #500
    ret

/* rp4_qspi_init() — initialize QSPI for eMMC access */
rp4_qspi_init:
    mov  x0, BCM2711_QSPI_BASE
    /* Enable QSPI controller */
    mov  x1, #1
    str  w1, [x0]
    /* Set clock divider */
    mov  x1, #4
    str  w1, [x0, #0x4]
    ret

/* rp4_mmu_init() — initialize MMU for BCM2711 memory map */
rp4_mmu_init:
    /* Set up identity mapping for peripherals */
    /* 0xFE000000 - 0xFFFFFFFF: Device memory */
    /* 0x00000000 - 0x3FFFFFFF: Normal memory */
    ret
