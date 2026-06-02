/* eth_rp4.asm: BCM2711 Ethernet driver for Raspberry Pi 4 (v11.0) */

.global eth_rp4_init
.global eth_rp4_phy_reset
.global eth_rp4_phy_config
.global eth_rp4_link_wait
.global eth_rp4_send_packet
.global eth_rp4_recv_packet
.global eth_rp4_get_mac_addr

/* BCM2711 Ethernet MAC registers */
.set ETH_MAC_BASE,       0xFD580000
.set MAC_CR,             0x000
.set MAC_ADRH,           0x004
.set MAC_ADRL,           0x008
.set MAC_HASHH,          0x00C
.set MAC_HASHL,          0x010
.set MAC_MII_ADDR,       0x060
.set MAC_MII_DATA,       0x064
.set MAC_FLOW_CTRL,      0x070
.set MAC_VLAN_TAG,       0x078
.set MAC_PM_CTRL,        0x080

/* DMA registers */
.set DMA_BUS_MODE,       0x100
#define DMA_BUS_MODE_SWR    (1 << 0)
.set DMA_TX_POLL_DEMAND, 0x108
.set DMA_RL_BASE,        0x10C
.set DMA_TL_BASE,        0x110
.set DMA_INTR_EN,        0x114
.set DMA_STATUS,         0x118
.set DMA_OP_MODE,        0x11C

/* Descriptor bits */
#define DESC_OWN         (1 << 31)
#define DESC_LAST        (1 << 29)
#define DESC_FIRST       (1 << 30)
#define DESC_CHKSUM      (1 << 28)

/* MII PHY registers */
.set PHY_BMCR,          0x00    /* Basic Mode Control */
.set PHY_BMSR,          0x01    /* Basic Mode Status */
.set PHY_ANAR,          0x04    /* Auto-Negotiation Advertisement */
.set PHY_ANLPAR,        0x05    /* Auto-Negotiation Link Partner */
.set PHY_ANER,          0x06    /* Auto-Negotiation Expansion */
.set PHY_CTRL1000,      0x09    /* 1000BASE-T Control */
.set PHY_STAT1000,      0x0A    /* 1000BASE-T Status */

/* PHY control bits */
#define PHY_RESET        (1 << 15)
#define PHY_AUTONEG      (1 << 12)
#define PHY_RESTART_AN   (1 << 9)
#define PHY_SPEED_1000   (1 << 13)
#define PHY_SPEED_100    (1 << 6)
#define PHY_DUPLEX       (1 << 8)
#define PHY_LINK_STATUS  (1 << 2)
#define PHY_AN_COMPLETE  (1 << 5)

/* TX/RX descriptor ring */
#define TX_DESC_COUNT    8
#define RX_DESC_COUNT    16
#define PKT_BUF_SIZE     1536

/* MAC address (default, overwritten by EEPROM) */
.set DEFAULT_MAC_LO,    0x00E04C36
.set DEFAULT_MAC_HI,    0x0000DCBA

.bss
eth_tx_desc_base:
    .skip TX_DESC_COUNT * 8   /* 8 descriptors * 8 bytes */
eth_rx_desc_base:
    .skip RX_DESC_COUNT * 8   /* 16 descriptors * 8 bytes */
eth_tx_buf:
    .skip TX_DESC_COUNT * PKT_BUF_SIZE
eth_rx_buf:
    .skip RX_DESC_COUNT * PKT_BUF_SIZE
eth_mac_addr_lo:
    .skip 4
eth_mac_addr_hi:
    .skip 4
eth_link_status:
    .skip 4    /* 0=down, 10=10M, 100=100M, 1000=1G */
eth_link_duplex:
    .skip 4    /* 0=half, 1=full */
eth_rx_index:
    .skip 4

.text

/* eth_rp4_init() — initialize Ethernet MAC and PHY
   Returns: 0=success, -1=failure */
eth_rp4_init:
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    mov  x0, ETH_MAC_BASE

    /* DMA reset */
    mov  w1, #DMA_BUS_MODE_SWR
    str  w1, [x0, #DMA_BUS_MODE]
    mov  x1, #1000
1:  subs x1, x1, #1
    b.ne 1b

    /* Set MAC address */
    mov  w1, #DEFAULT_MAC_HI
    str  w1, [x0, #MAC_ADRH]
    mov  w1, #DEFAULT_MAC_LO
    str  w1, [x0, #MAC_ADRL]

    /* Store MAC for get_mac_addr */
    adrp x2, eth_mac_addr_lo
    add  x2, x2, :lo12:eth_mac_addr_lo
    str  w1, [x2]
    adrp x2, eth_mac_addr_hi
    add  x2, x2, :lo12:eth_mac_addr_hi
    ldr  w3, [x0, #MAC_ADRH]
    str  w3, [x2]

    /* Setup TX descriptor ring */
    bl   eth_setup_tx_desc

    /* Setup RX descriptor ring */
    bl   eth_setup_rx_desc

    /* Set RX descriptor list base */
    adrp x1, eth_rx_desc_base
    add  x1, x1, :lo12:eth_rx_desc_base
    str  x1, [x0, #DMA_RL_BASE]

    /* Set TX descriptor list base */
    adrp x1, eth_tx_desc_base
    add  x1, x1, :lo12:eth_tx_desc_base
    str  x1, [x0, #DMA_TL_BASE]

    /* Configure DMA bus mode */
    mov  w1, #(4 << 8)    /* 1:1 AXI ratio */
    orr  w1, w1, #(1 << 1) /* Fixed burst */
    str  w1, [x0, #DMA_BUS_MODE]

    /* Configure DMA operation mode */
    mov  w1, #(1 << 13)   /* Store and forward TX */
    orr  w1, w1, #(1 << 21) /* Store and forward RX */
    str  w1, [x0, #DMA_OP_MODE]

    /* Enable interrupts */
    mov  w1, #(1 << 6)    /* Normal interrupt summary */
    orr  w1, w1, #(1 << 16) /* RX complete */
    orr  w1, w1, #(1 << 0)  /* TX complete */
    str  w1, [x0, #DMA_INTR_EN]

    /* Configure MAC */
    mov  w1, #(1 << 2)    /* Enable receiver */
    orr  w1, w1, #(1 << 3)  /* Enable transmitter */
    orr  w1, w1, #(1 << 0)  /* MAC enable */
    str  w1, [x0, #MAC_CR]

    /* Initialize PHY */
    bl   eth_rp4_phy_config

    /* Wait for link */
    bl   eth_rp4_link_wait

    mov  x0, #0

eth_init_exit:
    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* eth_setup_tx_desc() — initialize TX descriptor ring */
eth_setup_tx_desc:
    adrp x1, eth_tx_desc_base
    add  x1, x1, :lo12:eth_tx_desc_base
    adrp x2, eth_tx_buf
    add  x2, x2, :lo12:eth_tx_buf

    mov  x3, #TX_DESC_COUNT
    mov  x4, xzr
tx_desc_loop:
    /* Descriptor word 0: size, flags */
    mov  w5, #PKT_BUF_SIZE
    str  w5, [x1, x4, lsl #3]

    /* Descriptor word 1: buffer address */
    str  x2, [x1, x4, lsl #3, #8]!
    add  x2, x2, #PKT_BUF_SIZE
    add  x4, x4, #1
    cmp  x4, x3
    b.lo tx_desc_loop

    ret

/* eth_setup_rx_desc() — initialize RX descriptor ring */
eth_setup_rx_desc:
    adrp x1, eth_rx_desc_base
    add  x1, x1, :lo12:eth_rx_desc_base
    adrp x2, eth_rx_buf
    add  x2, x2, :lo12:eth_rx_buf

    mov  x3, #RX_DESC_COUNT
    mov  x4, xzr
rx_desc_loop:
    /* Descriptor word 0: OWN flag + buffer size */
    mov  w5, #DESC_OWN
    orr  w5, w5, #PKT_BUF_SIZE
    str  w5, [x1, x4, lsl #3]

    /* Descriptor word 1: buffer address */
    str  x2, [x1, x4, lsl #3, #8]!
    add  x2, x2, #PKT_BUF_SIZE
    add  x4, x4, #1
    cmp  x4, x3
    b.lo rx_desc_loop

    /* Store RX index */
    adrp x1, eth_rx_index
    add  x1, x1, :lo12:eth_rx_index
    str  wzr, [x1]

    ret

/* eth_rp4_phy_reset() — reset the PHY via MII
   x0=MAC base (caller), sets x0=MAC base on return */
eth_rp4_phy_reset:
    stp  x29, x30, [sp, #-16]!
    mov  x1, ETH_MAC_BASE

    /* Write PHY reset to BMCR */
    mov  w2, #PHY_BMCR
    mov  w3, #PHY_RESET
    bl   eth_phy_write

    /* Wait for reset to clear */
    mov  x4, #50000
phy_reset_wait:
    mov  w2, #PHY_BMCR
    bl   eth_phy_read
    tst  w0, #PHY_RESET
    b.ne phy_reset_done
    subs x4, x4, #1
    b.ne phy_reset_wait

phy_reset_done:
    mov  x0, ETH_MAC_BASE
    ldp  x29, x30, [sp], #16
    ret

/* eth_rp4_phy_config() — configure PHY auto-negotiation */
eth_rp4_phy_config:
    stp  x29, x30, [sp, #-16]!
    mov  x0, ETH_MAC_BASE

    /* Reset PHY */
    bl   eth_rp4_phy_reset

    /* Set ANAR: 10/100/1000 full + half duplex + 802.3 */
    mov  w2, #PHY_ANAR
    mov  w3, #0x01E1    /* 10HD, 10FD, 100HD, 100FD, 802.3 */
    bl   eth_phy_write

    /* Set 1000BASE-T control: full + half duplex */
    mov  w2, #PHY_CTRL1000
    mov  w3, #0x0300    /* 1000FD, 1000HD */
    bl   eth_phy_write

    /* Restart auto-negotiation */
    mov  w2, #PHY_BMCR
    mov  w3, #(PHY_AUTONEG | PHY_RESTART_AN)
    bl   eth_phy_write

    ldp  x29, x30, [sp], #16
    ret

/* eth_rp4_link_wait() — wait for link establishment
   Returns: link speed in x0 (0=failed) */
eth_rp4_link_wait:
    stp  x29, x30, [sp, #-16]!
    mov  x0, ETH_MAC_BASE
    mov  x4, #100000    /* retry count */

eth_link_poll:
    /* Read BMSR */
    mov  w2, #PHY_BMSR
    bl   eth_phy_read

    /* Check AN complete */
    tst  w0, #PHY_AN_COMPLETE
    b.ne eth_link_up

    subs x4, x4, #1
    b.ne eth_link_poll

    /* Link timeout */
    adrp x0, eth_link_status
    add  x0, x0, :lo12:eth_link_status
    str  wzr, [x0]
    mov  x0, #0
    b    eth_link_exit

eth_link_up:
    /* Read partner ability */
    mov  x0, ETH_MAC_BASE
    mov  w2, #PHY_ANLPAR
    bl   eth_phy_read

    /* Read 1000BASE-T status */
    mov  x0, ETH_MAC_BASE
    mov  w2, #PHY_STAT1000
    bl   eth_phy_read

    /* Determine link speed and duplex */
    adrp x1, eth_link_status
    add  x1, x1, :lo12:eth_link_status

    /* Check 1000BASE-T link partner status bits 11/10 */
    lsr  w3, w0, #10
    and  w3, w3, #3
    cbz  w3, eth_check_100
    mov  w4, #1000       /* 1Gbps */
    b    eth_store_link

eth_check_100:
    /* Check 100M ability */
    ldr  w3, [x0, #MAC_MII_DATA]
    and  w3, w3, #(1 << 8)   /* 100M full */
    cbnz w3, eth_100fd
    and  w3, w3, #(1 << 7)   /* 100M half */
    cbnz w3, eth_100hd
    b    eth_10m

eth_100fd:
    mov  w4, #100
    mov  w5, #1         /* full duplex */
    b    eth_store_link

eth_100hd:
    mov  w4, #100
    mov  w5, #0         /* half duplex */
    b    eth_store_link

eth_10m:
    mov  w4, #10
    mov  w5, #0

eth_store_link:
    str  w4, [x1]
    adrp x1, eth_link_duplex
    add  x1, x1, :lo12:eth_link_duplex
    str  w5, [x1]

    mov  x0, x4

eth_link_exit:
    ldp  x29, x30, [sp], #16
    ret

/* eth_rp4_send_packet(buf_ptr, len)
   x0=buffer pointer, x1=length
   Returns: 0=success, -1=failure */
eth_rp4_send_packet:
    stp  x29, x30, [sp, #-16]!

    mov  x2, x0          /* save buf ptr */
    mov  x3, x1          /* save len */

    /* Find free TX descriptor */
    adrp x4, eth_tx_desc_base
    add  x4, x4, :lo12:eth_tx_desc_base
    adrp x5, eth_tx_buf
    add  x5, x5, :lo12:eth_tx_buf

    /* Use first descriptor (ring management simplified) */
    mov  x6, xzr

    /* Copy packet data to TX buffer */
    add  x7, x5, x6, lsl #PKT_BUF_SIZE_SHIFT
    /* Simplified copy: assume caller buffer is aligned */
    mov  x8, #0
tx_copy_loop:
    ldrb w9, [x2, x8]
    strb w9, [x7, x8]
    add  x8, x8, #1
    cmp  x8, x3
    b.lo tx_copy_loop

    /* Set TX descriptor: OWN + FIRST + LAST + length */
    mov  w10, #(DESC_OWN | DESC_FIRST | DESC_LAST)
    orr  w10, w10, w3
    str  w10, [x4, x6, lsl #3]

    /* Set buffer address */
    str  x7, [x4, x6, lsl #3, #8]!

    /* Trigger TX poll */
    mov  x4, ETH_MAC_BASE
    str  wzr, [x4, #DMA_TX_POLL_DEMAND]

    /* Wait for TX complete */
    mov  x5, #10000
tx_wait:
    ldr  w6, [x4, #DMA_STATUS]
    and  w6, w6, #(1 << 0)    /* TX interrupt */
    cbnz w6, tx_done
    ldr  w7, [x4, #DMA_BUS_MODE]
    /* Check OWN bit cleared */
    ldr  w8, [x4, #DMA_TL_BASE]
    ldr  w9, [x8]
    tst  w9, #DESC_OWN
    b.eq tx_done
    subs x5, x5, #1
    b.ne tx_wait

    mov  x0, #-1
    b    tx_send_exit

tx_done:
    /* Clear DMA status */
    ldr  w6, [x4, #DMA_STATUS]
    str  w6, [x4, #DMA_STATUS]
    mov  x0, #0

tx_send_exit:
    ldp  x29, x30, [sp], #16
    ret

/* eth_rp4_recv_packet(buf_ptr, max_len)
   x0=buffer pointer, x1=max length
   Returns: bytes received in x0, -1=no data */
eth_rp4_recv_packet:
    stp  x29, x30, [sp, #-16]!
    mov  x2, x0          /* save buf ptr */
    mov  x3, x1          /* save max len */

    adrp x4, eth_rx_desc_base
    add  x4, x4, :lo12:eth_rx_desc_base
    adrp x5, eth_rx_buf
    add  x5, x5, :lo12:eth_rx_buf

    /* Get current RX index */
    adrp x6, eth_rx_index
    add  x6, x6, :lo12:eth_rx_index
    ldr  w7, [x6]

    /* Get descriptor */
    lsl  x8, x7, #3
    add  x9, x4, x8
    ldr  w10, [x9]       /* descriptor word 0 */

    /* Check OWN bit (0 = DMA done, data ready) */
    tst  w10, #DESC_OWN
    bne  recv_no_data

    /* Extract frame length (lower 16 bits) */
    and  w11, w10, #0x3FFF

    /* Check length is valid */
    cbz  w11, recv_no_data
    cmp  w11, w3
    b.gt recv_no_data    /* packet too large */

    /* Get source buffer address */
    add  x12, x5, x7, lsl #PKT_BUF_SIZE_SHIFT
    ldr  x12, [x9, #8]

    /* Copy packet data */
    mov  x13, xzr
recv_copy_loop:
    ldrb w14, [x12, x13]
    strb w14, [x2, x13]
    add  x13, x13, #1
    cmp  x13, x11
    b.lo recv_copy_loop

    /* Advance RX index */
    add  w7, w7, #1
    and  w7, w7, #(RX_DESC_COUNT - 1)
    str  w7, [x6]

    /* Re-arm descriptor: give OWN back to DMA */
    mov  w10, #(DESC_OWN | PKT_BUF_SIZE)
    str  w10, [x9]

    mov  x0, x11
    b    recv_exit

recv_no_data:
    mov  x0, #-1

recv_exit:
    ldp  x29, x30, [sp], #16
    ret

/* eth_rp4_get_mac_addr(buf_ptr)
   x0=buffer pointer (6 bytes)
   Returns: 0=success */
eth_rp4_get_mac_addr:
    adrp x1, eth_mac_addr_lo
    add  x1, x1, :lo12:eth_mac_addr_lo
    ldr  w2, [x1]        /* lower 4 bytes */
    adrp x3, eth_mac_addr_hi
    add  x3, x3, :lo12:eth_mac_addr_hi
    ldr  w4, [x3]        /* upper 2 bytes */

    /* Store MAC: 6 bytes, little-endian */
    strb w2, [x0, #0]     /* byte 0 */
    lsr  w5, w2, #8
    strb w5, [x0, #1]     /* byte 1 */
    lsr  w5, w2, #16
    strb w5, [x0, #2]     /* byte 2 */
    lsr  w5, w2, #24
    strb w5, [x0, #3]     /* byte 3 */
    strb w4, [x0, #4]     /* byte 4 */
    lsr  w5, w4, #8
    strb w5, [x0, #5]     /* byte 5 */

    mov  x0, #0
    ret

/* eth_phy_write(mii_reg, data) — write to PHY via MII
   x0=MAC base (input), x2=MII register, x3=data */
eth_phy_write:
    stp  x29, x30, [sp, #-16]!

    /* Wait for MII not busy */
phy_write_wait:
    ldr  w4, [x0, #MAC_MII_ADDR]
    tst  w4, #1
    b.ne phy_write_wait

    /* Write data */
    str  w3, [x0, #MAC_MII_DATA]

    /* Set MII address: PHY addr=1, reg=x2, write=1, busy=1 */
    mov  w4, #(1 << 11)   /* PHY address 1 */
    orr  w4, w4, w2, lsl #6  /* MII register */
    orr  w4, w4, #(1 << 1)   /* write */
    orr  w4, w4, #1          /* busy */
    str  w4, [x0, #MAC_MII_ADDR]

    /* Wait for MII complete */
phy_write_done_wait:
    ldr  w4, [x0, #MAC_MII_ADDR]
    tst  w4, #1
    b.ne phy_write_done_wait

    ldp  x29, x30, [sp], #16
    ret

/* eth_phy_read(mii_reg) — read from PHY via MII
   x0=MAC base (input), x2=MII register
   Returns: data in w0 */
eth_phy_read:
    stp  x29, x30, [sp, #-16]!

    /* Wait for MII not busy */
phy_read_wait:
    ldr  w4, [x0, #MAC_MII_ADDR]
    tst  w4, #1
    b.ne phy_read_wait

    /* Set MII address: PHY addr=1, reg=x2, read=0, busy=1 */
    mov  w4, #(1 << 11)
    orr  w4, w4, w2, lsl #6
    orr  w4, w4, #1          /* busy */
    str  w4, [x0, #MAC_MII_ADDR]

    /* Wait for MII complete */
phy_read_done_wait:
    ldr  w4, [x0, #MAC_MII_ADDR]
    tst  w4, #1
    b.ne phy_read_done_wait

    /* Read data */
    ldr  w0, [x0, #MAC_MII_DATA]

    ldp  x29, x30, [sp], #16
    ret
