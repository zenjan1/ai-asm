/* agent_ipc.asm: Agent IPC communication (v9.3) */

.global agent_ipc_init
.global agent_ipc_send
.global agent_ipc_recv
.global agent_ipc_broadcast
.global agent_ipc_poll
.global agent_ipc_queue_status

/* Message types */
.set MSG_TASK_ASSIGN,  1
.set MSG_TASK_RESULT,  2
.set MSG_QUERY,        3
.set MSG_RESPONSE,     4
.set MSG_ERROR,        5

/* Constants */
.set IPC_MAX_QUEUES,   16
.set IPC_MSGS_PER_Q,   32
.set IPC_MSG_SIZE,     256
.set IPC_QUEUE_SIZE,   IPC_MSGS_PER_Q * IPC_MSG_SIZE
.set IPC_TOTAL_SIZE,   IPC_MAX_QUEUES * IPC_QUEUE_SIZE

/* Message structure (256 bytes):
   +0: msg_type (1 byte)
   +1: sender_id (1 byte)
   +2: receiver_id (1 byte)
   +3: priority (1 byte)
   +4: timestamp (4 bytes)
   +8: payload (248 bytes)
*/

.bss
agent_ipc_queue:
    .skip IPC_TOTAL_SIZE
agent_ipc_queue_head:
    .skip IPC_MAX_QUEUES * 4
agent_ipc_queue_tail:
    .skip IPC_MAX_QUEUES * 4
agent_ipc_initialized:
    .skip 4
agent_ipc_msg_count:
    .skip 4

.text

/* agent_ipc_init() — initialize message system */
agent_ipc_init:
    /* Zero all queues */
    adrp x0, agent_ipc_queue
    add  x0, x0, :lo12:agent_ipc_queue
    mov  x1, #IPC_TOTAL_SIZE
    mov  x2, xzr
1:
    strb w2, [x0], #1
    sub  x1, x1, #1
    cbnz x1, 1b

    /* Reset head/tail pointers */
    adrp x0, agent_ipc_queue_head
    add  x0, x0, :lo12:agent_ipc_queue_head
    mov  x1, #(IPC_MAX_QUEUES * 4)
    mov  x2, xzr
2:
    str  w2, [x0], #4
    sub  x1, x1, #4
    cbnz x1, 2b

    /* Copy to tail */
    adrp x0, agent_ipc_queue_tail
    add  x0, x0, :lo12:agent_ipc_queue_tail
    mov  x1, #(IPC_MAX_QUEUES * 4)
3:
    str  w2, [x0], #4
    sub  x1, x1, #4
    cbnz x1, 3b

    /* Set initialized flag */
    adrp x0, agent_ipc_initialized
    add  x0, x0, :lo12:agent_ipc_initialized
    mov  w1, #1
    str  w1, [x0]

    /* Reset message count */
    adrp x0, agent_ipc_msg_count
    add  x0, x0, :lo12:agent_ipc_msg_count
    str  wzr, [x0]

    ret

/* agent_ipc_send(queue_id, msg_type, sender, receiver, priority, payload_ptr, payload_len)
   Returns: 0=success, -1=queue full */
agent_ipc_send:
    /* x0=queue_id, x1=msg_type, x2=sender, x3=receiver, x4=priority, x5=payload_ptr, x6=payload_len */

    /* Validate queue_id */
    cmp  x0, #IPC_MAX_QUEUES
    b.ge ipc_send_fail

    /* Get head pointer */
    adrp x7, agent_ipc_queue_head
    add  x7, x7, :lo12:agent_ipc_queue_head
    lsl  x8, x0, #2
    add  x7, x7, x8
    ldr  w7, [x7]

    /* Get tail pointer */
    adrp x9, agent_ipc_queue_tail
    add  x9, x9, :lo12:agent_ipc_queue_tail
    add  x9, x9, x8
    ldr  w9, [x9]

    /* Check if queue is full */
    sub  w10, w7, w9
    cmp  w10, #IPC_MSGS_PER_Q
    b.ge ipc_send_fail

    /* Calculate write position */
    and  w11, w7, #(IPC_MSGS_PER_Q - 1)

    /* Get queue base address */
    adrp x12, agent_ipc_queue
    add  x12, x12, :lo12:agent_ipc_queue
    lsl  x13, x0, #14
    add  x12, x12, x13
    lsl  x14, x11, #8
    add  x12, x12, x14

    /* Write message header (8 bytes) */
    strb w1, [x12]         /* msg_type */
    strb w2, [x12, #1]     /* sender_id */
    strb w3, [x12, #2]     /* receiver_id */
    strb w4, [x12, #3]     /* priority */

    /* Write timestamp placeholder (zeros) */
    str  wzr, [x12, #4]

    /* Copy payload (up to 248 bytes) */
    add  x15, x12, #8
    mov  x16, x6
    cbz  x16, ipc_send_update
    cmp  x16, #248
    b.gt ipc_send_clip
    b    ipc_send_copy
ipc_send_clip:
    mov  x16, #248
ipc_send_copy:
    mov  x17, xzr
ipc_copy_loop:
    ldrb w18, [x5, x17]
    strb w18, [x15, x17]
    add  x17, x17, #1
    cmp  x17, x16
    b.lt ipc_copy_loop

ipc_send_update:
    /* Increment head */
    add  w7, w7, #1
    str  w7, [x7]

    /* Increment global message count */
    adrp x7, agent_ipc_msg_count
    add  x7, x7, :lo12:agent_ipc_msg_count
    ldr  w7, [x7]
    add  w7, w7, #1
    str  w7, [x7]

    mov  x0, #0
    ret

ipc_send_fail:
    mov  x0, #-1
    ret

/* agent_ipc_recv(queue_id, buf_ptr, buf_len)
   Returns: msg_type on success, 0 if empty, -1 on error */
agent_ipc_recv:
    /* x0=queue_id, x1=buf_ptr, x2=buf_len */

    cmp  x0, #IPC_MAX_QUEUES
    b.ge ipc_recv_empty

    /* Get tail pointer */
    adrp x7, agent_ipc_queue_tail
    add  x7, x7, :lo12:agent_ipc_queue_tail
    lsl  x8, x0, #2
    add  x7, x7, x8
    ldr  w7, [x7]

    /* Get head pointer */
    adrp x9, agent_ipc_queue_head
    add  x9, x9, :lo12:agent_ipc_queue_head
    add  x9, x9, x8
    ldr  w9, [x9]

    /* Check if queue is empty */
    cmp  w7, w9
    b.eq ipc_recv_empty

    /* Calculate read position */
    and  w10, w7, #(IPC_MSGS_PER_Q - 1)

    /* Get queue base address */
    adrp x12, agent_ipc_queue
    add  x12, x12, :lo12:agent_ipc_queue
    lsl  x13, x0, #14
    add  x12, x12, x13
    lsl  x14, x10, #8
    add  x12, x12, x14

    /* Read message header */
    ldrb w3, [x12]         /* msg_type -> x3 */
    ldrb w4, [x12, #1]     /* sender_id -> x4 */
    ldrb w5, [x12, #2]     /* receiver_id -> x5 */
    ldrb w6, [x12, #3]     /* priority -> x6 */

    /* Write header to output buffer */
    strb w3, [x1]          /* msg_type */
    strb w4, [x1, #1]      /* sender */
    strb w5, [x1, #2]      /* receiver */
    strb w6, [x1, #3]      /* priority */

    /* Copy payload */
    add  x15, x12, #8
    add  x16, x1, #4
    mov  x17, #248
    cmp  x2, x17
    b.lt ipc_recv_limit
    mov  x17, x2
    sub  x17, x17, #4
ipc_recv_limit:
    mov  x18, xzr
ipc_recv_copy:
    ldrb w19, [x15, x18]
    strb w19, [x16, x18]
    add  x18, x18, #1
    cmp  x18, x17
    b.lt ipc_recv_copy

    /* Increment tail */
    add  w7, w7, #1
    str  w7, [x7]

    mov  x0, x3
    ret

ipc_recv_empty:
    mov  x0, #0
    ret

/* agent_ipc_broadcast(msg_type, sender, priority, payload_ptr, payload_len)
   Broadcasts to all 16 queues */
agent_ipc_broadcast:
    /* x0=msg_type, x1=sender, x2=priority, x3=payload_ptr, x4=payload_len */
    mov  x5, x3
    mov  x6, x4
    mov  x7, xzr
bc_loop:
    cmp  x7, #IPC_MAX_QUEUES
    b.ge bc_done
    mov  x0, x7
    mov  x1, x5
    mov  x2, x7
    mov  x3, x7
    mov  x4, x6
    bl   agent_ipc_send
    add  x7, x7, #1
    b    bc_loop
bc_done:
    ret

/* agent_ipc_poll() — return number of messages across all queues */
agent_ipc_poll:
    mov  x0, xzr
    adrp x1, agent_ipc_msg_count
    add  x1, x1, :lo12:agent_ipc_msg_count
    ldr  w0, [x1]
    ret

/* agent_ipc_queue_status(queue_id) — return (head - tail) for a queue */
agent_ipc_queue_status:
    cmp  x0, #IPC_MAX_QUEUES
    b.ge ipc_status_empty

    adrp x7, agent_ipc_queue_head
    add  x7, x7, :lo12:agent_ipc_queue_head
    lsl  x8, x0, #2
    add  x7, x7, x8
    ldr  w7, [x7]

    adrp x9, agent_ipc_queue_tail
    add  x9, x9, :lo12:agent_ipc_queue_tail
    add  x9, x9, x8
    ldr  w9, [x9]

    sub  x0, x7, x9
    ret

ipc_status_empty:
    mov  x0, #0
    ret
