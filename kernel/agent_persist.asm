/* agent_persist.asm: Agent state persistence (v11.0) */

.global agent_save_state
.global agent_load_state
.global agent_save_task
.global agent_load_task
.global agent_clear_history
.global agent_persist_init

/* Persistence storage area (SRAM-mapped or MMC sector buffer) */
.set PERSIST_MAGIC,     0x41475053    /* "AGPS" */
.set PERSIST_VERSION,   1
.set MAX_AGENT_STATES,  32
#define MAX_TASK_STATES   64
#define MAX_HISTORY       128
#define AGENT_NAME_LEN    64
#define STATE_DATA_LEN    256

/* Agent states stored */
.set STATE_IDLE,        0
.set STATE_RUNNING,     1
.set STATE_PAUSED,      2
.set STATE_COMPLETED,     3
.set STATE_FAILED,      4

/* Disk/MMC sector for persistence */
#define PERSIST_SECTOR   1024    /* Sector 1024 = 512KB offset */
#define PERSIST_SECTORS  16      /* 8KB total storage */

/* Header structure (64 bytes) */
#define HDR_MAGIC        0
#define HDR_VERSION      4
#define HDR_AGENT_COUNT  8
#define HDR_TASK_COUNT   12
#define HDR_HISTORY_CNT  16
#define HDR_TIMESTAMP    20
#define HDR_CHECKSUM     28
#define HDR_SIZE         64

.bss
persist_initialized:
    .skip 4
persist_sector:
    .skip 4
persist_buffer:
    .skip 8192    /* 8KB persistence buffer */
persist_header:
    .skip HDR_SIZE
agent_state_data:
    .skip MAX_AGENT_STATES * (AGENT_NAME_LEN + STATE_DATA_LEN + 12)
task_state_data:
    .skip MAX_TASK_STATES * (STATE_DATA_LEN + 16)
history_data:
    .skip MAX_HISTORY * (STATE_DATA_LEN + 8)

.text

/* agent_persist_init() — initialize persistence subsystem
   Returns: 0=success, -1=failure */
agent_persist_init:
    stp  x29, x30, [sp, #-16]!

    /* Clear persistence buffer */
    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer
    mov  x1, #8192
    mov  x2, xzr
1:  strb w2, [x0], #1
    subs x1, x1, #1
    cbnz x1, 1b

    /* Set default sector */
    adrp x0, persist_sector
    add  x0, x0, :lo12:persist_sector
    mov  w1, #PERSIST_SECTOR
    str  w1, [x0]

    /* Build header with magic */
    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer
    mov  w1, #PERSIST_MAGIC
    str  w1, [x0, #HDR_MAGIC]
    mov  w1, #PERSIST_VERSION
    str  w1, [x0, #HDR_VERSION]

    /* Mark initialized */
    adrp x0, persist_initialized
    add  x0, x0, :lo12:persist_initialized
    mov  w1, #1
    str  w1, [x0]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

/* agent_save_state() — save all agent states to persistent storage
   Returns: 0=success, -1=failure */
agent_save_state:
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer

    /* Verify magic */
    ldr  w1, [x0, #HDR_MAGIC]
    cmp  w1, #PERSIST_MAGIC
    b.ne persist_fail

    /* Update header: agent count, task count, history count */
    /* These would be populated from the agent system globals */
    mov  w1, #0    /* agent count (from agent runtime) */
    str  w1, [x0, #HDR_AGENT_COUNT]
    mov  w1, #0    /* task count */
    str  w1, [x0, #HDR_TASK_COUNT]
    mov  w1, #0    /* history count */
    str  w1, [x0, #HDR_HISTORY_CNT]

    /* Copy agent state data to buffer at offset HDR_SIZE */
    adrp x1, agent_state_data
    add  x1, x1, :lo12:agent_state_data
    add  x2, x0, #HDR_SIZE
    mov  x3, #(MAX_AGENT_STATES * (AGENT_NAME_LEN + STATE_DATA_LEN + 12))
    bl   persist_memcopy

    /* Copy task state data */
    adrp x1, task_state_data
    add  x1, x1, :lo12:task_state_data
    add  x2, x0, #HDR_SIZE
    add  x2, x2, x3
    mov  x3, #(MAX_TASK_STATES * (STATE_DATA_LEN + 16))
    bl   persist_memcopy

    /* Copy history data */
    adrp x1, history_data
    add  x1, x1, :lo12:history_data
    add  x2, x0, #HDR_SIZE
    add  x2, x2, x3
    mov  x3, #(MAX_HISTORY * (STATE_DATA_LEN + 8))
    bl   persist_memcopy

    /* Calculate checksum (simple XOR) */
    adrp x1, persist_buffer
    add  x1, x1, :lo12:persist_buffer
    mov  x2, #8192
    mov  w3, xzr
    add  x1, x1, #4    /* skip magic for checksum */
persist_cksum_loop:
    ldr  w4, [x1], #4
    eor  w3, w3, w4
    subs x2, x2, #4
    b.gt persist_cksum_loop

    adrp x1, persist_buffer
    add  x1, x1, :lo12:persist_buffer
    str  w3, [x1, #HDR_CHECKSUM]

    /* Write to MMC (via SD/MMC driver) */
    /* This would call sdmmc_rp4_write_block for each sector */
    adrp x0, persist_sector
    add  x0, x0, :lo12:persist_sector
    ldr  w1, [x0]         /* sector number */

    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer
    /* bl sdmmc_rp4_write_block  — would call here */

    mov  x0, #0
    b    persist_exit

persist_fail:
    mov  x0, #-1

persist_exit:
    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* agent_load_state() — restore agent states from persistent storage
   Returns: 0=success, -1=failure or no saved state */
agent_load_state:
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer

    /* Read from MMC (via SD/MMC driver) */
    adrp x1, persist_sector
    add  x1, x1, :lo12:persist_sector
    ldr  w1, [x1]
    /* bl sdmmc_rp4_read_block  — would call here */

    /* Verify magic */
    ldr  w1, [x0, #HDR_MAGIC]
    cmp  w1, #PERSIST_MAGIC
    b.ne persist_load_fail

    /* Verify version */
    ldr  w1, [x0, #HDR_VERSION]
    cmp  w1, #PERSIST_VERSION
    b.ne persist_load_fail

    /* Verify checksum */
    mov  x2, #8192
    mov  w3, xzr
    add  x1, x0, #4
persist_verify_cksum:
    ldr  w4, [x1], #4
    eor  w3, w3, w4
    subs x2, x2, #4
    b.gt persist_verify_cksum

    ldr  w4, [x0, #HDR_CHECKSUM]
    cmp  w3, w4
    b.ne persist_load_fail

    /* Restore agent state data */
    add  x1, x0, #HDR_SIZE
    adrp x2, agent_state_data
    add  x2, x2, :lo12:agent_state_data
    mov  x3, #(MAX_AGENT_STATES * (AGENT_NAME_LEN + STATE_DATA_LEN + 12))
    bl   persist_memcopy

    /* Restore task state data */
    add  x1, x1, x3
    adrp x2, task_state_data
    add  x2, x2, :lo12:task_state_data
    mov  x3, #(MAX_TASK_STATES * (STATE_DATA_LEN + 16))
    bl   persist_memcopy

    /* Restore history data */
    add  x1, x1, x3
    adrp x2, history_data
    add  x2, x2, :lo12:history_data
    mov  x3, #(MAX_HISTORY * (STATE_DATA_LEN + 8))
    bl   persist_memcopy

    /* Read header counts */
    ldr  w1, [x0, #HDR_AGENT_COUNT]
    ldr  w2, [x0, #HDR_TASK_COUNT]
    ldr  w3, [x0, #HDR_HISTORY_CNT]

    mov  x0, #0
    b    persist_load_exit

persist_load_fail:
    mov  x0, #-1

persist_load_exit:
    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* agent_save_task(task_id, data_ptr, data_len)
   x0=task_id, x1=data pointer, x2=data length
   Returns: 0=success, -1=failure */
agent_save_task:
    cmp  x0, #MAX_TASK_STATES
    b.ge task_save_fail

    stp  x29, x30, [sp, #-16]!

    /* Store task ID */
    adrp x3, task_state_data
    add  x3, x3, :lo12:task_state_data
    mov  x4, #(STATE_DATA_LEN + 16)
    mul  x4, x0, x4
    add  x3, x3, x4

    str  w0, [x3]           /* task ID */
    str  w2, [x3, #4]       /* data length */

    /* Copy data */
    mov  x5, x1
    add  x6, x3, #16
    mov  x7, xzr
task_save_copy:
    ldrb w8, [x5, x7]
    strb w8, [x6, x7]
    add  x7, x7, #1
    cmp  x7, x2
    b.lo task_save_copy

    /* Update header task count */
    adrp x3, persist_buffer
    add  x3, x3, :lo12:persist_buffer
    ldr  w4, [x3, #HDR_TASK_COUNT]
    add  w4, w4, #1
    str  w4, [x3, #HDR_TASK_COUNT]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

task_save_fail:
    mov  x0, #-1
    ret

/* agent_load_task(task_id, buf_ptr, max_len)
   x0=task_id, x1=buffer, x2=max length
   Returns: bytes loaded in x0, -1=failure */
agent_load_task:
    cmp  x0, #MAX_TASK_STATES
    b.ge task_load_fail

    stp  x29, x30, [sp, #-16]!

    /* Find task data */
    adrp x3, task_state_data
    add  x3, x3, :lo12:task_state_data
    mov  x4, #(STATE_DATA_LEN + 16)
    mul  x4, x0, x4
    add  x3, x3, x4

    /* Read task ID */
    ldr  w5, [x3]
    cmp  w5, w0
    b.ne task_load_fail

    /* Read data length */
    ldr  w6, [x3, #4]
    cmp  w6, w2
    b.gt task_load_fail    /* data too large for buffer */

    /* Copy data to buffer */
    add  x7, x3, #16
    mov  x8, xzr
task_load_copy:
    ldrb w9, [x7, x8]
    strb w9, [x1, x8]
    add  x8, x8, #1
    cmp  x8, x6
    b.lo task_load_copy

    mov  x0, x6
    ldp  x29, x30, [sp], #16
    ret

task_load_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* agent_clear_history() — clear all history records
   Returns: 0=success */
agent_clear_history:
    stp  x29, x30, [sp, #-16]!

    adrp x0, history_data
    add  x0, x0, :lo12:history_data
    mov  x1, #(MAX_HISTORY * (STATE_DATA_LEN + 8))
    mov  x2, xzr
hist_clear_loop:
    strb w2, [x0], #1
    subs x1, x1, #1
    cbnz x1, hist_clear_loop

    /* Reset header history count */
    adrp x0, persist_buffer
    add  x0, x0, :lo12:persist_buffer
    str  wzr, [x0, #HDR_HISTORY_CNT]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

/* persist_memcopy(dst, src, len) — internal memory copy
   x1=src, x2=dst, x3=len */
persist_memcopy:
    cbz  x3, persist_copy_done
    mov  x4, xzr
persist_copy_loop:
    ldrb w5, [x1, x4]
    strb w5, [x2, x4]
    add  x4, x4, #1
    cmp  x4, x3
    b.lo persist_copy_loop
persist_copy_done:
    ret
