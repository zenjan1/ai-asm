/* sdmmc_rp4.asm: BCM2711 SD/MMC controller driver (v10.0) */

.global sdmmc_rp4_init
.global sdmmc_rp4_read_block
.global sdmmc_rp4_write_block
.global sdmmc_rp4_get_capacity
.global sdmmc_rp4_detect_card
.global sdmmc_rp4_set_block_size

/* BCM2711 EMMC2 registers */
.set EMMC2_BASE,        0xFF340000
.set EMMC_ARG2,         0x00
.set EMMC_BLKSIZECNT,   0x04
.set EMMC_ARG1,         0x08
.set EMMC_CMDTM,        0x0C
.set EMMC_RESP0,        0x10
.set EMMC_RESP1,        0x14
.set EMMC_RESP2,        0x18
.set EMMC_RESP3,        0x1C
#define EMMC_DATA       0x20
.set EMMC_STATUS,       0x24
#define EMMC_CONTROL0   0x28
.set EMMC_CONTROL1,     0x2C
.set EMMC_INTERRUPT,    0x30
.set EMMC_IRPT_MASK,    0x34
.set EMMC_IRPT_EN,      0x38
.set EMMC_CONTROL2,     0x3C
#define EMMC_SLOTISR_VER 0xFC

/* SD commands */
.set SD_CMD_GO_IDLE,        0x00000000
.set SD_CMD_ALL_SEND_CID,   0x02010000
.set SD_CMD_SEND_REL_ADDR,  0x03020000
.set SD_CMD_SET_DSCR,       0x07030000
.set SD_CMD_CARD_SELECT,    0x0D030000
.set SD_CMD_SEND_IF_COND,   0x08020000
.set SD_CMD_STOP_TRANS,     0x0C030000
.set SD_CMD_READ_SINGLE,    0x11220010
.set SD_CMD_READ_MULTI,     0x12220012
.set SD_CMD_WRITE_SINGLE,   0x18220010
.set SD_CMD_WRITE_MULTI,    0x19220012
.set SD_CMD_APP_CMD,        0x37000000
.set SD_CMD_ACMD41,         0x29020000
.set SD_CMD_ACMD6,          0x06020000
.set SD_CMD_ALL_SEND_SCR,   0x33220010

/* Status bits */
.set STATUS_CMD_INHIBIT,    (1 << 0)
.set STATUS_DAT_INHIBIT,    (1 << 1)
.set STATUS_DAT_ACTIVE,     (1 << 2)
.set STATUS_READY,          (1 << 5)
#define STATUS_WRITE_AVAILABLE (1 << 10)
#define STATUS_READ_AVAILABLE  (1 << 11)

/* Control1 bits */
.set CTRL1_CLK_EN,          (1 << 2)
#define CTRL1_STABLE_CLK,    (1 << 1)
#define CTRL1_RESET,         1

/* Interrupt bits */
.set IRPT_CMD_DONE,         (1 << 0)
.set IRPT_WRITE_RDY,        (1 << 4)
.set IRPT_READ_RDY,         (1 << 5)
.set IRPT_ERROR,            (1 << 15)

.bss
sdmmc_card_present:
    .skip 4
sdmmc_card_rca:
    .skip 4
sdmmc_block_size:
    .skip 4
sdmmc_block_count:
    .skip 4
sdmmc_capacity_sectors:
    .skip 4
sdmmc_is_sdhc:
    .skip 4

.text

/* sdmmc_rp4_init() — initialize SD/MMC controller
   Returns: 0=success, -1=failure */
sdmmc_rp4_init:
    stp  x29, x30, [sp, #-16]!

    mov  x0, EMMC2_BASE

    /* Reset controller */
    mov  w1, #CTRL1_RESET
    str  w1, [x0, #EMMC_CONTROL1]

    /* Wait for reset to complete */
1:  ldr  w1, [x0, #EMMC_CONTROL1]
    and  w1, w1, #CTRL1_RESET
    cbnz w1, 1b

    /* Enable clock */
    mov  w1, #CTRL1_CLK_EN
    str  w1, [x0, #EMMC_CONTROL1]

    /* Wait for clock stable */
2:  ldr  w1, [x0, #EMMC_CONTROL1]
    and  w1, w1, #CTRL1_STABLE_CLK
    cbz  w1, 2b

    /* Clear interrupts */
    mov  w1, #0xFFFFFFFF
    str  w1, [x0, #EMMC_INTERRUPT]

    /* Enable interrupts */
    mov  w1, #(IRPT_CMD_DONE | IRPT_WRITE_RDY | IRPT_READ_RDY | IRPT_ERROR)
    str  w1, [x0, #EMMC_IRPT_EN]

    /* Detect card */
    bl   sdmmc_detect_card

    /* Send CMD0: GO_IDLE_STATE */
    mov  w0, #SD_CMD_GO_IDLE
    bl   sdmmc_send_cmd

    /* Send CMD8: SEND_IF_COND (voltage check) */
    mov  w0, #SD_CMD_SEND_IF_COND
    mov  w1, #0x000001AA
    bl   sdmmc_send_cmd_arg

    /* ACMD41 loop: card initialization */
    mov  x2, #10000       /* retry count */
sd_acmd41_loop:
    /* CMD55: APP_CMD */
    mov  w0, #SD_CMD_APP_CMD
    bl   sdmmc_send_cmd

    /* ACMD41: SD_SEND_OP_COND */
    mov  w0, #SD_CMD_ACMD41
    mov  w1, #0x40300000  /* HCS=1, 3.0-3.3V */
    bl   sdmmc_send_cmd_arg

    /* Check if card is ready (response bit 31) */
    ldr  w3, [x0, #EMMC_RESP0]
    tst  w3, #(1 << 31)
    b.ne sd_init_done

    subs x2, x2, #1
    b.ne sd_acmd41_loop

    /* Card not detected */
    adrp x0, sdmmc_card_present
    add  x0, x0, :lo12:sdmmc_card_present
    str  wzr, [x0]
    mov  x0, #-1
    b    sd_init_exit

sd_init_done:
    /* Card initialized */
    adrp x0, sdmmc_card_present
    add  x0, x0, :lo12:sdmmc_card_present
    mov  w1, #1
    str  w1, [x0]

    /* CMD2: ALL_SEND_CID */
    mov  w0, #SD_CMD_ALL_SEND_CID
    bl   sdmmc_send_cmd

    /* CMD3: SEND_REL_ADDR */
    mov  w0, #SD_CMD_SEND_REL_ADDR
    bl   sdmmc_send_cmd

    /* Store RCA from response */
    ldr  w3, [x0, #EMMC_RESP0]
    lsr  w3, w3, #16
    adrp x0, sdmmc_card_rca
    add  x0, x0, :lo12:sdmmc_card_rca
    str  w3, [x0]

    /* CMD7: CARD_SELECT */
    mov  w0, #SD_CMD_CARD_SELECT
    bl   sdmmc_send_cmd

    /* Set default block size: 512 */
    mov  x0, #512
    bl   sdmmc_set_block_size

    /* Set capacity (placeholder: assume 32GB SDHC) */
    adrp x0, sdmmc_capacity_sectors
    add  x0, x0, :lo12:sdmmc_capacity_sectors
    mov  w1, #67108864  /* 32GB / 512 */
    str  w1, [x0]

    mov  x0, #0

sd_init_exit:
    ldp  x29, x30, [sp], #16
    ret

/* sdmmc_send_cmd(cmd) — send command without argument
   x0=command, returns 0=success */
sdmmc_send_cmd:
    mov  x1, x0
    mov  x2, EMMC2_BASE
    str  wzr, [x2, #EMMC_ARG1]
    str  w1, [x2, #EMMC_CMDTM]
    b    sdmmc_wait_cmd

/* sdmmc_send_cmd_arg(cmd, arg) — send command with argument
   x0=command, x1=argument */
sdmmc_send_cmd_arg:
    mov  x2, x0
    mov  x3, x1
    mov  x0, EMMC2_BASE
    str  w3, [x0, #EMMC_ARG1]
    str  w2, [x0, #EMMC_CMDTM]
    mov  x0, EMMC2_BASE

sdmmc_wait_cmd:
    /* Wait for command done or error */
    mov  x2, #10000
1:  ldr  w3, [x0, #EMMC_INTERRUPT]
    tbz  w3, #0, 2f      /* CMD_DONE */
    /* Check error */
    tst  w3, #IRPT_ERROR
    b.ne sdmmc_cmd_error
2:  subs x2, x2, #1
    cbz  x2, sdmmc_cmd_timeout
    ldr  w3, [x0, #EMMC_STATUS]
    tst  w3, #STATUS_CMD_INHIBIT
    cbnz w3, 1b

    /* Clear interrupt */
    mov  w3, #0xFFFFFFFF
    str  w3, [x0, #EMMC_INTERRUPT]
    mov  x0, #0
    ret

sdmmc_cmd_error:
    mov  w3, #0xFFFFFFFF
    str  w3, [x0, #EMMC_INTERRUPT]
    mov  x0, #-1
    ret

sdmmc_cmd_timeout:
    mov  x0, #-1
    ret

/* sdmmc_detect_card() — check if SD card is present */
sdmmc_detect_card:
    /* Check SLOTISR_VER for card presence */
    mov  x0, EMMC2_BASE
    ldr  w1, [x0, #EMMC_SLOTISR_VER]
    and  w1, w1, #0xFF    /* Low byte = slot status */
    cbz  w1, sd_no_card
    mov  x0, #1
    ret
sd_no_card:
    mov  x0, #0
    ret

/* sdmmc_rp4_read_block(block_num, buf_ptr)
   x0=block_num, x1=buffer pointer */
sdmmc_rp4_read_block:
    stp  x29, x30, [sp, #-16]!

    mov  x2, x0
    mov  x3, x1
    mov  x0, EMMC2_BASE

    /* Set block size and count */
    adrp x4, sdmmc_block_size
    add  x4, x4, :lo12:sdmmc_block_size
    ldr  w4, [x4]

    mov  w5, #1           /* 1 block */
    lsl  w5, w5, #16      /* block count in upper 16 bits */
    orr  w5, w5, w4
    str  w5, [x0, #EMMC_BLKSIZECNT]

    /* Set block address */
    str  w2, [x0, #EMMC_ARG1]

    /* CMD17: READ_SINGLE_BLOCK */
    mov  w4, #SD_CMD_READ_SINGLE
    str  w4, [x0, #EMMC_CMDTM]

    /* Wait for read ready */
    mov  x5, #100000
1:  ldr  w6, [x0, #EMMC_STATUS]
    tst  w6, #STATUS_READ_AVAILABLE
    cbnz w6, 2f
    ldr  w6, [x0, #EMMC_INTERRUPT]
    tst  w6, #IRPT_ERROR
    b.ne sd_read_error
    subs x5, x5, #1
    cbz  x5, sd_read_error
    b    1b

2:  /* Read data (16 words = 64 bytes per read, repeat for 512 bytes) */
    mov  x5, #32          /* 512 bytes / 4 bytes per word / 4 words per burst */
    mov  x6, x3
3:  ldp  w7, w8, [x0, #EMMC_DATA]
    stp  w7, w8, [x6], #8
    ldp  w7, w8, [x0, #EMMC_DATA]
    stp  w7, w8, [x6], #8
    subs x5, x5, #1
    cbnz x5, 3b

    /* Clear interrupt */
    mov  w5, #0xFFFFFFFF
    str  w5, [x0, #EMMC_INTERRUPT]

    mov  x0, #0
    b    sd_read_exit

sd_read_error:
    mov  w5, #0xFFFFFFFF
    str  w5, [x0, #EMMC_INTERRUPT]
    mov  x0, #-1

sd_read_exit:
    ldp  x29, x30, [sp], #16
    ret

/* sdmmc_rp4_write_block(block_num, buf_ptr)
   x0=block_num, x1=buffer pointer */
sdmmc_rp4_write_block:
    stp  x29, x30, [sp, #-16]!

    mov  x2, x0
    mov  x3, x1
    mov  x0, EMMC2_BASE

    /* Set block size and count */
    adrp x4, sdmmc_block_size
    add  x4, x4, :lo12:sdmmc_block_size
    ldr  w4, [x4]
    mov  w5, #1
    lsl  w5, w5, #16
    orr  w5, w5, w4
    str  w5, [x0, #EMMC_BLKSIZECNT]

    /* Set block address */
    str  w2, [x0, #EMMC_ARG1]

    /* CMD24: WRITE_SINGLE_BLOCK */
    mov  w4, #SD_CMD_WRITE_SINGLE
    str  w4, [x0, #EMMC_CMDTM]

    /* Wait for write ready */
    mov  x5, #100000
1:  ldr  w6, [x0, #EMMC_STATUS]
    tst  w6, #STATUS_WRITE_AVAILABLE
    cbnz w6, 2f
    ldr  w6, [x0, #EMMC_INTERRUPT]
    tst  w6, #IRPT_ERROR
    b.ne sd_write_error
    subs x5, x5, #1
    cbz  x5, sd_write_error
    b    1b

2:  /* Write data */
    mov  x5, #32
    mov  x6, x3
3:  ldp  w7, w8, [x6], #8
    stp  w7, w8, [x0, #EMMC_DATA]
    ldp  w7, w8, [x6], #8
    stp  w7, w8, [x0, #EMMC_DATA]
    subs x5, x5, #1
    cbnz x5, 3b

    /* Clear interrupt */
    mov  w5, #0xFFFFFFFF
    str  w5, [x0, #EMMC_INTERRUPT]

    mov  x0, #0
    b    sd_write_exit

sd_write_error:
    mov  w5, #0xFFFFFFFF
    str  w5, [x0, #EMMC_INTERRUPT]
    mov  x0, #-1

sd_write_exit:
    ldp  x29, x30, [sp], #16
    ret

/* sdmmc_rp4_get_capacity() — return capacity in sectors */
sdmmc_rp4_get_capacity:
    adrp x0, sdmmc_capacity_sectors
    add  x0, x0, :lo12:sdmmc_capacity_sectors
    ldr  w0, [x0]
    ret

/* sdmmc_rp4_detect_card() — return 1 if card present */
sdmmc_rp4_detect_card:
    adrp x0, sdmmc_card_present
    add  x0, x0, :lo12:sdmmc_card_present
    ldr  w0, [x0]
    ret

/* sdmmc_set_block_size(size)
   x0=block_size */
sdmmc_set_block_size:
    adrp x1, sdmmc_block_size
    add  x1, x1, :lo12:sdmmc_block_size
    str  w0, [x1]
    mov  x0, #0
    ret
