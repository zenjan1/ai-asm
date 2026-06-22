/*
 * aiasm-aarch64/kernel/syscall.asm
 * System call interface: SVC-based dispatch from user space
 * Syscall number in w8, args in x0-x3, return in x0
 */
.arch armv8-a

.text

/* -----------------------------------------------------------------------------
 * Function: syscall_handler
 * Description: System call entry point from SVC exception
 * Input: w8 = syscall number, x0-x3 = arguments
 * Output: x0 = return value
 * Clobbered: caller-saved registers
 * Stack: none (called from exception vector)
 * ----------------------------------------------------------------------------- */
.global syscall_handler
syscall_handler:
    /* w8 = syscall number */
    cmp     w8, #SYS_SLEEP
    b.eq    syscall_sleep
    cmp     w8, #SYS_GET_TICK
    b.eq    syscall_get_tick
    cmp     w8, #SYS_GET_TIME_MS
    b.eq    syscall_get_time_ms
    cmp     w8, #SYS_PUTS
    b.eq    syscall_puts
    cmp     w8, #SYS_MEM_INFO
    b.eq    syscall_mem_info
    cmp     w8, #SYS_PROC_CREATE
    b.eq    syscall_proc_create
    cmp     w8, #SYS_PROC_LIST
    b.eq    syscall_proc_list

    /* Unknown syscall */
    mov     x0, #-1
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_SLEEP (w8=0)
 * Input: x0 = milliseconds
 * Output: x0 = 0
 * ----------------------------------------------------------------------------- */
syscall_sleep:
    bl      timer_delay_ms
    mov     x0, #0
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_GET_TICK (w8=1)
 * Input: none
 * Output: x0 = scheduler tick count
 * ----------------------------------------------------------------------------- */
syscall_get_tick:
    adrp    x0, tick_counter
    add     x0, x0, #:lo12:tick_counter
    ldr     x0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_GET_TIME_MS (w8=2)
 * Input: none
 * Output: x0 = milliseconds since boot
 * ----------------------------------------------------------------------------- */
syscall_get_time_ms:
    bl      timer_get_ms
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_PUTS (w8=3)
 * Input: x0 = string pointer
 * Output: x0 = 0
 * ----------------------------------------------------------------------------- */
syscall_puts:
    bl      serial_puts
    mov     x0, #0
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_MEM_INFO (w8=4)
 * Input: x0 = buffer pointer
 * Output: x0 = buffer pointer
 * ----------------------------------------------------------------------------- */
syscall_mem_info:
    bl      mem_info
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_PROC_CREATE (w8=5)
 * Input: x0 = entry point, x1 = name pointer
 * Output: w0 = PID or -1
 * ----------------------------------------------------------------------------- */
syscall_proc_create:
    bl      proc_create
    ret

/* -----------------------------------------------------------------------------
 * Syscall: SYS_PROC_LIST (w8=6)
 * Input: x0 = buffer pointer
 * Output: x0 = buffer pointer
 * ----------------------------------------------------------------------------- */
syscall_proc_list:
    bl      proc_list
    ret

/* Syscall numbers */
.set SYS_SLEEP,        0
.set SYS_GET_TICK,     1
.set SYS_GET_TIME_MS,  2
.set SYS_PUTS,         3
.set SYS_MEM_INFO,     4
.set SYS_PROC_CREATE,  5
.set SYS_PROC_LIST,    6
