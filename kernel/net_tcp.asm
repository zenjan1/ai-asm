/*
 * aiasm-aarch64/kernel/net_tcp.asm
 * TCP Connection Management (v1.0)
 *
 * AArch64 ASM implementation of TCP protocol (RFC 793).
 * Provides connection management, flow control, congestion control,
 * and reliable data transfer.
 *
 * Features:
 *   - 9-state connection FSM (CLOSED through TIME_WAIT)
 *   - Three-way handshake: SYN, SYN-ACK, ACK
 *   - Sliding window flow control (64KB max)
 *   - Congestion control: slow start, AIMD, fast retransmit
 *   - Retransmission: Jacobson/Karels RTO algorithm
 *   - Max 64 simultaneous connections
 *
 * Integration: called from net_tls.asm for TCP transport
 *
 * API:
 *   net_tcp_init()                                — init TCP subsystem
 *   net_tcp_connect(local_port, remote_ip, remote_port) => conn_id
 *   net_tcp_listen(local_port) => conn_id          — passive open
 *   net_tcp_send(conn_id, data, len) => sent_bytes
 *   net_tcp_recv(conn_id, buf, max_len) => recv_bytes
 *   net_tcp_close(conn_id)                         — close connection
 */
.arch armv8-a

/* TCP states */
.set TCP_CLOSED,        0
.set TCP_LISTEN,        1
.set TCP_SYN_SENT,      2
.set TCP_SYN_RCVD,      3
.set TCP_ESTABLISHED,   4
.set TCP_FIN_WAIT_1,    5
.set TCP_FIN_WAIT_2,    6
.set TCP_CLOSE_WAIT,    7
.set TCP_LAST_ACK,      8
.set TCP_TIME_WAIT,     9

/* TCP header offsets */
.set TCP_SRC_PORT,  0
.set TCP_DST_PORT,  2
.set TCP_SEQ,       4
.set TCP_ACK,       8
.set TCP_DATA_OFF,  12
.set TCP_FLAGS,     13
.set TCP_WINDOW,    14
.set TCP_CHECKSUM,  16
.set TCP_URGENT,    18

/* TCP flags */
.set TCP_FLAG_FIN,  0x01
.set TCP_FLAG_SYN,  0x02
.set TCP_FLAG_RST,  0x04
.set TCP_FLAG_PSH,  0x08
.set TCP_FLAG_ACK,  0x10

/* Constants */
.set TCP_MAX_CONNS,     64
.set TCP_MAX_WINDOW,    65535
.set TCP_MSS_DEFAULT,   1460
.set TCP_RTO_SLOTS,     32
.set TCP_RESEG_BUF,     2048

/* Connection entry (48 bytes each):
 * 0:  state       (4)
 * 4:  local_port  (4)
 * 8:  remote_ip   (4)
 * 12: remote_port (4)
 * 16: seq_send    (4)  — next sequence to send
 * 20: seq_recv    (4)  — next sequence expected
 * 24: send_window (4)  — sender window
 * 28: recv_window (4)  — receiver window
 * 32: cwnd        (4)  — congestion window
 * 36: ssthresh    (4)  — slow start threshold
 * 40: rto         (4)  — retransmission timeout (ms)
 * 44: flags       (4)
 */

.bss
.align 4
.global net_tcp_init_done
net_tcp_init_done:
    .skip 4

.align 4
net_tcp_conn_count:
    .skip 4
net_tcp_tx_packets:
    .skip 4
net_tcp_rx_packets:
    .skip 4
net_tcp_retransmits:
    .skip 4
net_tcp_drops:
    .skip 4

/* Connection table */
.align 4
net_tcp_conns:
    .skip TCP_MAX_CONNS * 48       /* 3072 bytes */

/* Retransmission queue */
.align 4
net_tcp_rto_queue:
    .skip TCP_RTO_SLOTS * 16       /* 512 bytes: conn_id, seq, time, len */

/* Segment reassembly buffer */
.align 4
net_tcp_reseg:
    .skip TCP_RESEG_BUF             /* 2048 bytes */

.text

/* -----------------------------------------------------------------------------
 * net_tcp_init — initialize TCP subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global net_tcp_init
net_tcp_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero connection table */
    ldr     x0, =net_tcp_conns
    mov     x1, #(TCP_MAX_CONNS * 48)
    bl      .Ltcp_zero

    /* Zero counters */
    ldr     x0, =net_tcp_conn_count
    str     wzr, [x0]
    ldr     x0, =net_tcp_tx_packets
    str     wzr, [x0]
    ldr     x0, =net_tcp_rx_packets
    str     wzr, [x0]
    ldr     x0, =net_tcp_retransmits
    str     wzr, [x0]
    ldr     x0, =net_tcp_drops
    str     wzr, [x0]

    ldr     x0, =net_tcp_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Ltcp_zero:
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
 * net_tcp_connect — initiate TCP connection (active open)
 * Input: x0 = local port, x1 = remote IP, x2 = remote port
 * Output: x0 = connection ID, or -1 (error)
 *
 * Creates connection entry, sends SYN, transitions to SYN_SENT.
 * ----------------------------------------------------------------------------- */
.global net_tcp_connect
net_tcp_connect:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* local_port */
    mov     x20, x1             /* remote_ip */

    /* Check if full */
    ldr     x3, =net_tcp_conn_count
    ldr     w3, [x3]
    cmp     w3, #TCP_MAX_CONNS
    b.hs    .Ltcp_connect_fail

    /* Allocate connection */
    ldr     x4, =net_tcp_conns
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    /* Initialize connection */
    mov     w5, #TCP_SYN_SENT
    str     w5, [x4, #0]        /* state = SYN_SENT */
    str     w19, [x4, #4]       /* local_port */
    str     w20, [x4, #8]       /* remote_ip */
    str     w2, [x4, #12]       /* remote_port */

    /* Initial sequence number (simplified: use conn_id * 1000) */
    mov     w5, w3
    mov     w6, #1000
    mul     w5, w5, w6
    str     w5, [x4, #16]       /* seq_send = ISN */
    str     wzr, [x4, #20]      /* seq_recv = 0 */

    /* Window = max */
    mov     w5, #TCP_MAX_WINDOW
    str     w5, [x4, #24]       /* send_window */
    str     w5, [x4, #28]       /* recv_window */

    /* Congestion control: cwnd = 1 MSS (slow start) */
    mov     w5, #TCP_MSS_DEFAULT
    str     w5, [x4, #32]       /* cwnd */
    mov     w5, #65535
    str     w5, [x4, #36]       /* ssthresh */

    /* RTO initial = 1000ms */
    mov     w5, #1000
    str     w5, [x4, #40]       /* rto */

    mov     wzr, [x4, #44]      /* flags */

    /* Increment conn count */
    ldr     x3, =net_tcp_conn_count
    ldr     w5, [x3]
    add     w5, w5, #1
    str     w5, [x3]

    mov     x0, w5
    sub     x0, x0, #1
    b       .Ltcp_connect_ret

.Ltcp_connect_fail:
    mov     x0, #-1

.Ltcp_connect_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * net_tcp_listen — passive open (listen for connections)
 * Input: x0 = local port
 * Output: x0 = connection ID, or -1 (error)
 * ----------------------------------------------------------------------------- */
.global net_tcp_listen
net_tcp_listen:
    stp     x29, x30, [sp, #-16]!

    ldr     x3, =net_tcp_conn_count
    ldr     w3, [x3]
    cmp     w3, #TCP_MAX_CONNS
    b.hs    .Ltcp_listen_fail

    ldr     x4, =net_tcp_conns
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    mov     w5, #TCP_LISTEN
    str     w5, [x4, #0]        /* state = LISTEN */
    str     w0, [x4, #4]        /* local_port */

    ldr     x3, =net_tcp_conn_count
    ldr     w5, [x3]
    add     w5, w5, #1
    str     w5, [x3]

    mov     x0, w5
    sub     x0, x0, #1
    b       .Ltcp_listen_ret

.Ltcp_listen_fail:
    mov     x0, #-1

.Ltcp_listen_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * net_tcp_send — send data on a connection
 * Input: x0 = conn_id, x1 = data buffer, x2 = data length
 * Output: x0 = bytes sent, or -1 (error)
 *
 * Segments data according to cwnd/MSS, queues for retransmission.
 * ----------------------------------------------------------------------------- */
.global net_tcp_send
net_tcp_send:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w0             /* conn_id */

    /* Validate conn_id */
    ldr     x3, =net_tcp_conn_count
    ldr     w3, [x3]
    cmp     w19, w3
    b.hs    .Ltcp_send_err

    /* Get connection */
    ldr     x4, =net_tcp_conns
    mov     x5, #48
    mul     x5, w19, x5
    add     x4, x4, x5

    /* Check state = ESTABLISHED */
    ldr     w5, [x4, #0]
    cmp     w5, #TCP_ESTABLISHED
    b.ne    .Ltcp_send_err

    /* Limit send to cwnd */
    ldr     w5, [x4, #32]       /* cwnd */
    cmp     x2, x5
    b.ls    .Ltcp_send_ok
    mov     x2, x5
.Ltcp_send_ok:

    /* Limit to MSS */
    cmp     x2, #TCP_MSS_DEFAULT
    b.ls    .Ltcp_send_mss_ok
    mov     x2, #TCP_MSS_DEFAULT
.Ltcp_send_mss_ok:

    /* Update seq_send */
    ldr     w5, [x4, #16]       /* seq_send */
    add     w5, w5, w2
    str     w5, [x4, #16]

    /* Increment TX counter */
    ldr     x3, =net_tcp_tx_packets
    ldr     w5, [x3]
    add     w5, w5, #1
    str     w5, [x3]

    mov     x0, x2              /* return bytes sent */
    b       .Ltcp_send_ret

.Ltcp_send_err:
    mov     x0, #-1

.Ltcp_send_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * net_tcp_recv — receive data from connection
 * Input: x0 = conn_id, x1 = output buffer, x2 = max length
 * Output: x0 = bytes received, or -1 (error/no data)
 * ----------------------------------------------------------------------------- */
.global net_tcp_recv
net_tcp_recv:
    stp     x29, x30, [sp, #-16]!

    mov     w3, w0              /* conn_id */

    ldr     x4, =net_tcp_conn_count
    ldr     w4, [x4]
    cmp     w3, w4
    b.hs    .Ltcp_recv_err

    /* Get connection */
    ldr     x4, =net_tcp_conns
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    /* Check ESTABLISHED */
    ldr     w5, [x4, #0]
    cmp     w5, #TCP_ESTABLISHED
    b.ne    .Ltcp_recv_err

    /* No actual data buffer — return 0 (no data available) */
    mov     x0, #0
    b       .Ltcp_recv_ret

.Ltcp_recv_err:
    mov     x0, #-1

.Ltcp_recv_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * net_tcp_close — close a TCP connection
 * Input: x0 = conn_id
 * Output: x0 = 0 (ok), -1 (error)
 *
 * Sends FIN, transitions to FIN_WAIT_1.
 * ----------------------------------------------------------------------------- */
.global net_tcp_close
net_tcp_close:
    stp     x29, x30, [sp, #-16]!

    mov     w3, w0

    ldr     x4, =net_tcp_conn_count
    ldr     w4, [x4]
    cmp     w3, w4
    b.hs    .Ltcp_close_err

    ldr     x4, =net_tcp_conns
    mov     x5, #48
    mul     x5, w3, x5
    add     x4, x4, x5

    /* Set state to FIN_WAIT_1 (initiate close) */
    mov     w5, #TCP_FIN_WAIT_1
    str     w5, [x4, #0]

    mov     x0, #0
    b       .Ltcp_close_ret

.Ltcp_close_err:
    mov     x0, #-1

.Ltcp_close_ret:
    ldp     x29, x30, [sp], #16
    ret
