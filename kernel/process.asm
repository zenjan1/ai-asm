/*
 * aiasm-aarch64/kernel/process.asm
 * Process control blocks, context switch, round-robin scheduler
 * PCB layout (64 bytes per process):
 *   +0:  state (0=free,1=ready,2=running,3=sleeping)
 *   +4:  pid
 *   +8:  x0-x30 (31*8=248 bytes)
 *   +256: sp (saved stack pointer)
 *   +264: pc (saved program counter)
 *   +272: stack_base (top of kernel stack)
 *   +280: quantum (time slice counter)
 *   +284: name (32 bytes)
 *   Total: ~320 bytes per PCB, aligned to 320
 */
.arch armv8-a

.set MAX_PROCS,     8
.set PROC_RUNNING,  2
.set PROC_READY,    1
.set PROC_FREE,     0
.set PCB_SIZE,      320
.set PCB_STATE,     0
.set PCB_PID,       4
.set PCB_X0,        8
.set PCB_SP,        256
.set PCB_PC,        264
.set PCB_QUANTUM,   280
.set PCB_NAME,      284

.text

/* -----------------------------------------------------------------------------
 * Function: proc_init
 * Description: Initialize process manager, create idle process (PID 0)
 * Input: none
 * Output: none
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global proc_init
proc_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero all PCBs */
    adrp    x0, pcb_table
    add     x0, x0, #:lo12:pcb_table
    mov     w1, #0
    mov     x2, #(MAX_PROCS * PCB_SIZE)
    bl      memset

    /* PID counter = 1 (PID 0 reserved for idle) */
    adrp    x0, next_pid
    add     x0, x0, #:lo12:next_pid
    mov     w1, #1
    str     w1, [x0]

    /* Current process = 0 (idle) */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    str     wzr, [x0]

    /* Process count = 1 */
    adrp    x0, proc_count
    add     x0, x0, #:lo12:proc_count
    mov     w1, #1
    str     w1, [x0]

    /* Set up idle process (PID 0) */
    adrp    x0, pcb_table
    add     x0, x0, #:lo12:pcb_table
    mov     w1, #PROC_RUNNING
    str     w1, [x0, #PCB_STATE]
    str     wzr, [x0, #PCB_PID]
    mov     w1, #10
    str     w1, [x0, #PCB_QUANTUM]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: proc_create
 * Description: Create a new process with given entry point
 * Input: x0 = entry point address, x1 = process name pointer
 * Output: w0 = PID, or -1 if no slot
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global proc_create
proc_create:
    stp     x29, x30, [sp, #-16]!
    mov     x3, x0              /* entry point */
    mov     x4, x1              /* name */

    /* Find free PCB slot */
    adrp    x0, pcb_table
    add     x0, x0, #:lo12:pcb_table
    mov     x1, #0              /* slot index */
1:
    cmp     x1, #MAX_PROCS
    b.ge    proc_create_fail
    ldr     w2, [x0, #PCB_STATE]
    cbz     w2, 2f              /* free slot found */
    add     x0, x0, #PCB_SIZE
    add     x1, x1, #1
    b       1b

2:
    /* Allocate kernel stack from page allocator */
    bl      mem_alloc_page
    cbz     x0, proc_create_fail

    mov     x5, x0              /* stack base phys addr */
    add     x5, x5, #0x1000     /* stack grows down from top */

    /* Get next PID */
    adrp    x1, next_pid
    add     x1, x1, #:lo12:next_pid
    ldr     w2, [x1]

    /* Initialize PCB */
    mov     w1, #PROC_READY
    str     w1, [x0, #PCB_STATE]
    str     w2, [x0, #PCB_PID]
    str     x3, [x0, #PCB_PC]   /* entry point */
    str     x5, [x0, #PCB_SP]   /* stack pointer */
    str     x5, [x0, #PCB_X0 + 248]  /* x31 (SP saved here) */
    mov     w1, #10
    str     w1, [x0, #PCB_QUANTUM]

    /* Copy name (up to 31 chars) */
    add     x1, x0, #PCB_NAME
    mov     x2, #0
3:
    cmp     x2, #32
    b.eq    4f
    ldrb    w3, [x4, x2]
    cbz     w3, 4f
    strb    w3, [x1, x2]
    add     x2, x2, #1
    b       3b

4:
    strb    wzr, [x1, x2]

    /* Increment PID and process count */
    adrp    x1, next_pid
    add     x1, x1, #:lo12:next_pid
    add     w2, w2, #1
    str     w2, [x1]

    adrp    x1, proc_count
    add     x1, x1, #:lo12:proc_count
    ldr     w2, [x1]
    add     w2, w2, #1
    str     w2, [x1]

    mov     x0, x2              /* return old PID (w0) */
    ldp     x29, x30, [sp], #16
    ret

proc_create_fail:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: proc_schedule
 * Description: Round-robin scheduler tick - switch to next ready process
 * Input: none
 * Output: none
 * Clobbered: all
 * Stack: uses current process stack
 * ----------------------------------------------------------------------------- */
.global proc_schedule
proc_schedule:
    /* Increment timer tick */
    bl      timer_tick

    /* Get current PID */
    adrp    x0, current_pid
    add     x0, x0, #:lo12:current_pid
    ldr     w1, [x0]

    /* Find current PCB */
    adrp    x2, pcb_table
    add     x2, x2, #:lo12:pcb_table
    mov     x3, #0
1:
    cmp     x3, x1
    b.eq    2f
    add     x2, x2, #PCB_SIZE
    add     x3, x3, #1
    b       1b

2:
    /* Save current context: x19-x30 (callee-saved) + sp + pc */
    stp     x19, x20, [x2, #PCB_X0 + 152]
    stp     x21, x22, [x2, #PCB_X0 + 168]
    stp     x23, x24, [x2, #PCB_X0 + 184]
    stp     x25, x26, [x2, #PCB_X0 + 200]
    stp     x27, x28, [x2, #PCB_X0 + 216]
    stp     x29, x30, [x2, #PCB_X0 + 232]
    mov     x3, sp
    str     x3, [x2, #PCB_SP]
    mov     x3, x30             /* return address saved as PC */

    /* Mark current as ready */
    mov     w3, #PROC_READY
    str     w3, [x2, #PCB_STATE]

    /* Decrement quantum */
    ldr     w3, [x2, #PCB_QUANTUM]
    sub     w3, w3, #1
    str     w3, [x2, #PCB_QUANTUM]

    /* Find next ready process */
    mov     x4, x2              /* start from current PCB */
    mov     w5, #0              /* loop counter */
3:
    add     x4, x4, #PCB_SIZE
    add     w5, w5, #1
    cmp     w5, #MAX_PROCS
    b.eq    4f                  /* back to self */
    ldr     w6, [x4, #PCB_STATE]
    cmp     w6, #PROC_READY
    b.eq    5f                  /* found ready */
    cmp     w6, #PROC_RUNNING
    b.eq    5f                  /* also allow running (wraparound) */
    b       3b

4:
    /* No other process ready, stay with current */
    mov     x4, x2
    mov     w3, #PROC_RUNNING
    str     w3, [x4, #PCB_STATE]
    mov     w1, #10
    str     w1, [x4, #PCB_QUANTUM]
    b       proc_restore

5:
    /* Mark new process as running */
    mov     w6, #PROC_RUNNING
    str     w6, [x4, #PCB_STATE]
    mov     w6, #10
    str     w6, [x4, #PCB_QUANTUM]

    /* Update current PID */
    ldr     w6, [x4, #PCB_PID]
    adrp    x5, current_pid
    add     x5, x5, #:lo12:current_pid
    str     w6, [x5]

proc_restore:
    /* Restore context from new PCB */
    ldr     x3, [x4, #PCB_SP]
    mov     sp, x3
    ldp     x19, x20, [x4, #PCB_X0 + 152]
    ldp     x21, x22, [x4, #PCB_X0 + 168]
    ldp     x23, x24, [x4, #PCB_X0 + 184]
    ldp     x25, x26, [x4, #PCB_X0 + 200]
    ldp     x27, x28, [x4, #PCB_X0 + 216]
    ldp     x29, x30, [x4, #PCB_X0 + 232]

    /* Return to new process */
    ldr     x0, [x4, #PCB_PC]
    br      x0

/* -----------------------------------------------------------------------------
 * Function: proc_list
 * Description: Print process list as JSON array
 * Input: x0 = output buffer
 * Output: x0 = buffer pointer
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global proc_list
proc_list:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x0              /* save buffer */

    mov     x0, #91             /* '[' */
    bl      serial_putc

    adrp    x0, pcb_table
    add     x0, x0, #:lo12:pcb_table
    mov     x1, #0              /* slot index */
    mov     x2, #0              /* first flag */

1:
    cmp     x1, #MAX_PROCS
    b.eq    3f

    ldr     w3, [x0, #PCB_STATE]
    cbz     w3, 2f              /* skip free slots */

    /* Print comma if not first */
    cbz     x2, 4f
    mov     x0, #44
    bl      serial_putc
4:
    mov     x2, #1

    /* Print {"pid":N,"state":N} */
    adrp    x5, msg_proc_pre
    add     x5, x5, #:lo12:msg_proc_pre
    mov     x0, x5
    bl      serial_puts

    /* PID */
    ldr     w3, [x0, #PCB_PID]
    adrp    x5, proc_itoa_buf
    add     x5, x5, #:lo12:proc_itoa_buf
    mov     x0, x3
    mov     x1, x5
    bl      itoa_buf
    mov     x0, x5
    bl      serial_puts

    adrp    x5, msg_proc_mid
    add     x5, x5, #:lo12:msg_proc_mid
    mov     x0, x5
    bl      serial_puts

    /* State */
    ldr     w3, [x0, #PCB_STATE]
    adrp    x5, proc_itoa_buf
    add     x5, x5, #:lo12:proc_itoa_buf
    mov     x0, x3
    mov     x1, x5
    bl      itoa_buf
    mov     x0, x5
    bl      serial_puts

    adrp    x5, msg_proc_suf
    add     x5, x5, #:lo12:msg_proc_suf
    mov     x0, x5
    bl      serial_puts

2:
    add     x0, x0, #PCB_SIZE
    add     x1, x1, #1
    b       1b

3:
    mov     x0, #93             /* ']' */
    bl      serial_putc
    mov     x0, #10
    bl      serial_putc

    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

/* Process state */
.bss
.align 4
pcb_table:
    .skip MAX_PROCS * PCB_SIZE
next_pid:
    .skip 4
current_pid:
    .skip 4
proc_count:
    .skip 4

.bss
.align 4
proc_itoa_buf:
    .skip 24

.section .rodata
.align 4
msg_proc_pre:
    .asciz "{\"pid\":"
msg_proc_mid:
    .asciz ",\"state\":"
msg_proc_suf:
    .asciz "}"
