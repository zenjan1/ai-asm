/*
 * aiasm-aarch64/kernel/quota.asm
 * Process resource quota management for v7.0/v8.0.
 *
 * Quota types: CPU ticks, memory bytes, file descriptors.
 * Per-process quota table with limit/used tracking.
 *
 * Quota entry (16 bytes per process):
 *   +0:  cpu_ticks_limit  (4 bytes)
 *   +4:  cpu_ticks_used   (4 bytes)
 *   +8:  mem_bytes_limit  (4 bytes)
 *   +12: mem_bytes_used   (4 bytes) - not tracked here, via proc_mem
 *   +16: fd_limit        (1 byte)
 *   +17: fd_used         (1 byte)
 *   +18: pad             (2 bytes)
 *
 * Default quotas by permission level:
 *   root (0):  unlimited (0 = no limit)
 *   admin (1): 1M ticks, 16MB, 16 fd
 *   user (2):  100K ticks, 8MB, 8 fd
 *   guest (3): 10K ticks, 4MB, 4 fd
 *
 * API:
 *   quota_init()              - zero all quotas
 *   quota_set(pid, cpu, mem, fd) - set quota limits
 *   quota_check_cpu(pid)      - 0=ok, -1=exceeded
 *   quota_bump_cpu(pid)       - increment cpu_used
 *   quota_check_mem(pid, size) - 0=ok, -1=exceeded
 *   quota_check_fd(pid)       - 0=ok, -1=exceeded
 *   quota_get(pid, buf)       - fill buf with quota info (32 bytes)
 */
.arch armv8-a

.set QUOTA_MAX,      8          /* MAX_PROCS */
.set QUOTA_SIZE,     20         /* entry size */
.set QUOTA_CPU_LIM,  0
.set QUOTA_CPU_USE,  4
.set QUOTA_MEM_LIM,  8
.set QUOTA_MEM_USE,  12
.set QUOTA_FD_LIM,   16
.set QUOTA_FD_USE,   17

/* Default limits by permission level */
.set Q_ROOT,         0          /* 0 = unlimited */
.set Q_ADMIN_CPU,    1000000
.set Q_ADMIN_MEM,    0x01000000 /* 16MB */
.set Q_ADMIN_FD,     16
.set Q_USER_CPU,     100000     /* 100K */
.set Q_USER_MEM,     0x00800000 /* 8MB */
.set Q_USER_FD,      8
.set Q_GUEST_CPU,    10000      /* 10K */
.set Q_GUEST_MEM,    0x00400000 /* 4MB */
.set Q_GUEST_FD,     4

/* ---------------------------------------------------------------------------
 * BSS: Quota table
 * --------------------------------------------------------------------------- */
.bss
.align 4
.global quota_table
quota_table:
    .skip QUOTA_MAX * QUOTA_SIZE

.global quota_init_done
quota_init_done:
    .word 0

.text

/* ---------------------------------------------------------------------------
 * quota_init: Zero all quota entries
 * --------------------------------------------------------------------------- */
.global quota_init
quota_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, quota_init_done
    add     x0, x0, #:lo12:quota_init_done
    ldr     w1, [x0]
    cbnz    w1, 9f              /* already initialized */

    adrp    x0, quota_table
    add     x0, x0, #:lo12:quota_table
    mov     x1, #(QUOTA_MAX * QUOTA_SIZE)
    bl      _qt_memset

    mov     w1, #1
    adrp    x0, quota_init_done
    add     x0, x0, #:lo12:quota_init_done
    str     w1, [x0]

9:  ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_set_default: Set default quota based on permission level
 * Input: x0 = pid, w1 = permission level (0=root, 1=admin, 2=user, 3=guest)
 * --------------------------------------------------------------------------- */
.global quota_set_default
quota_set_default:
    stp     x29, x30, [sp, #-16]!
    mov     w8, w1              /* permission level */

    /* Find quota entry */
    adrp    x0, quota_table
    add     x0, x0, #:lo12:quota_table
    madd    x0, x0, x0, x0      /* x0 = pid * QUOTA_SIZE (20) */
    /* pid * 20 = pid * 16 + pid * 4 */
    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4          /* pid * 16 */
    lsl     x2, x2, #2          /* pid * 4 */
    add     x0, x3, x2
    add     x0, x0, x1

    /* Set defaults based on level */
    cbz     w8, _qt_root        /* root */
    cmp     w8, #1
    b.eq    _qt_admin
    cmp     w8, #2
    b.eq    _qt_user
    /* guest */
    mov     w1, #Q_GUEST_CPU
    str     w1, [x0, #QUOTA_CPU_LIM]
    mov     w1, #Q_GUEST_FD
    strb    w1, [x0, #QUOTA_FD_LIM]
    /* mem: 4MB = 0x00400000 */
    mov     w1, #1; lsl w1, w1, #22
    str     w1, [x0, #QUOTA_MEM_LIM]
    b       _qt_done

_qt_root:
    /* Unlimited: set all limits to 0 */
    str     wzr, [x0, #QUOTA_CPU_LIM]
    str     wzr, [x0, #QUOTA_MEM_LIM]
    mov     w1, #255            /* max fd */
    strb    w1, [x0, #QUOTA_FD_LIM]
    b       _qt_done

_qt_admin:
    /* cpu: 1M = 1000000 = 0xF4240 */
    mov     w1, #0xF424
    movk    w1, #0x000F, lsl #16
    str     w1, [x0, #QUOTA_CPU_LIM]
    mov     w1, #Q_ADMIN_FD
    strb    w1, [x0, #QUOTA_FD_LIM]
    /* mem: 16MB = 0x01000000 */
    mov     w1, #1; lsl w1, w1, #24
    str     w1, [x0, #QUOTA_MEM_LIM]
    b       _qt_done

_qt_user:
    /* cpu: 100K = 0x186A0 */
    mov     w1, #0x86A0
    movk    w1, #0x1, lsl #16
    str     w1, [x0, #QUOTA_CPU_LIM]
    mov     w1, #Q_USER_FD
    strb    w1, [x0, #QUOTA_FD_LIM]
    /* mem: 8MB = 0x00800000 */
    mov     w1, #1; lsl w1, w1, #23
    str     w1, [x0, #QUOTA_MEM_LIM]

_qt_done:
    /* Zero used counters */
    str     wzr, [x0, #QUOTA_CPU_USE]
    str     wzr, [x0, #QUOTA_MEM_USE]
    strb    wzr, [x0, #QUOTA_FD_USE]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_set: Set quota limits directly
 * Input: x0 = pid, w1 = cpu_limit, w2 = mem_limit, w3 = fd_limit
 * --------------------------------------------------------------------------- */
.global quota_set
quota_set:
    stp     x29, x30, [sp, #-16]!

    /* Calculate entry address: quota_table + pid * 20 */
    adrp    x4, quota_table
    add     x4, x4, #:lo12:quota_table
    mov     w5, w0
    lsl     x6, x5, #4          /* pid * 16 */
    lsl     x5, x5, #2          /* pid * 4 */
    add     x4, x4, x6
    add     x4, x4, x5          /* x4 = entry */

    str     w1, [x4, #QUOTA_CPU_LIM]
    str     w2, [x4, #QUOTA_MEM_LIM]
    strb    w3, [x4, #QUOTA_FD_LIM]

    /* Zero used counters */
    str     wzr, [x4, #QUOTA_CPU_USE]
    str     wzr, [x4, #QUOTA_MEM_USE]
    strb    wzr, [x4, #QUOTA_FD_USE]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_check_cpu: Check if CPU quota exceeded
 * Input: x0 = pid
 * Output: w0 = 0 (ok) or -1 (exceeded)
 * --------------------------------------------------------------------------- */
.global quota_check_cpu
quota_check_cpu:
    stp     x29, x30, [sp, #-16]!

    /* Calculate entry address */
    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4
    lsl     x2, x2, #2
    add     x1, x1, x3
    add     x1, x1, x2

    ldr     w2, [x1, #QUOTA_CPU_LIM]
    cbz     w2, 1f              /* 0 = unlimited, always ok */

    ldr     w3, [x1, #QUOTA_CPU_USE]
    cmp     w3, w2
    b.ge    2f                  /* exceeded */

1:  mov     w0, #0              /* ok */
    ldp     x29, x30, [sp], #16
    ret

2:  mov     w0, #-1             /* exceeded */
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_bump_cpu: Increment CPU usage counter
 * Input: x0 = pid
 * --------------------------------------------------------------------------- */
.global quota_bump_cpu
quota_bump_cpu:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4
    lsl     x2, x2, #2
    add     x1, x1, x3
    add     x1, x1, x2

    ldr     w2, [x1, #QUOTA_CPU_USE]
    add     w2, w2, #1
    str     w2, [x1, #QUOTA_CPU_USE]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_check_mem: Check if memory allocation would exceed quota
 * Input: x0 = pid, w1 = requested size
 * Output: w0 = 0 (ok) or -1 (exceeded)
 * --------------------------------------------------------------------------- */
.global quota_check_mem
quota_check_mem:
    stp     x29, x30, [sp, #-16]!

    adrp    x2, quota_table
    add     x2, x2, #:lo12:quota_table
    mov     w3, w0
    lsl     x4, x3, #4
    lsl     x3, x3, #2
    add     x2, x2, x4
    add     x2, x2, x3

    ldr     w3, [x2, #QUOTA_MEM_LIM]
    cbz     w3, 1f              /* 0 = unlimited */

    ldr     w4, [x2, #QUOTA_MEM_USE]
    add     w4, w4, w1          /* used + requested */
    cmp     w4, w3
    b.gt    2f                  /* would exceed */

1:  mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

2:  mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_bump_mem: Increment memory usage
 * Input: x0 = pid, w1 = allocated size
 * --------------------------------------------------------------------------- */
.global quota_bump_mem
quota_bump_mem:
    stp     x29, x30, [sp, #-16]!

    adrp    x2, quota_table
    add     x2, x2, #:lo12:quota_table
    mov     w3, w0
    lsl     x4, x3, #4
    lsl     x3, x3, #2
    add     x2, x2, x4
    add     x2, x2, x3

    ldr     w3, [x2, #QUOTA_MEM_USE]
    add     w3, w3, w1
    str     w3, [x2, #QUOTA_MEM_USE]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_check_fd: Check if fd quota exceeded
 * Input: x0 = pid
 * Output: w0 = 0 (ok) or -1 (exceeded)
 * --------------------------------------------------------------------------- */
.global quota_check_fd
quota_check_fd:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4
    lsl     x2, x2, #2
    add     x1, x1, x3
    add     x1, x1, x2

    ldrb    w2, [x1, #QUOTA_FD_LIM]
    ldrb    w3, [x1, #QUOTA_FD_USE]
    cmp     w3, w2
    b.ge    1f                  /* exceeded */

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

1:  mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_bump_fd / quota_drop_fd: FD counter management
 * --------------------------------------------------------------------------- */
.global quota_bump_fd
quota_bump_fd:
    stp     x29, x30, [sp, #-16]!
    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4
    lsl     x2, x2, #2
    add     x1, x1, x3
    add     x1, x1, x2
    ldrb    w2, [x1, #QUOTA_FD_USE]
    add     w2, w2, #1
    strb    w2, [x1, #QUOTA_FD_USE]
    ldp     x29, x30, [sp], #16
    ret

.global quota_drop_fd
quota_drop_fd:
    stp     x29, x30, [sp, #-16]!
    adrp    x1, quota_table
    add     x1, x1, #:lo12:quota_table
    mov     w2, w0
    lsl     x3, x2, #4
    lsl     x2, x2, #2
    add     x1, x1, x3
    add     x1, x1, x2
    ldrb    w2, [x1, #QUOTA_FD_USE]
    sub     w2, w2, #1
    strb    w2, [x1, #QUOTA_FD_USE]
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * quota_get: Get full quota info for a process
 * Input: x0 = pid, x1 = output buffer (32 bytes: cpu_lim, cpu_use, mem_lim, mem_use, fd_lim, fd_use)
 * --------------------------------------------------------------------------- */
.global quota_get
quota_get:
    stp     x29, x30, [sp, #-16]!

    adrp    x2, quota_table
    add     x2, x2, #:lo12:quota_table
    mov     w3, w0
    lsl     x4, x3, #4
    lsl     x3, x3, #2
    add     x2, x2, x4
    add     x2, x2, x3

    ldr     w4, [x2, #QUOTA_CPU_LIM]
    str     w4, [x1, #0]
    ldr     w4, [x2, #QUOTA_CPU_USE]
    str     w4, [x1, #4]
    ldr     w4, [x2, #QUOTA_MEM_LIM]
    str     w4, [x1, #8]
    ldr     w4, [x2, #QUOTA_MEM_USE]
    str     w4, [x1, #12]
    ldrb    w4, [x2, #QUOTA_FD_LIM]
    strb    w4, [x1, #16]
    ldrb    w4, [x2, #QUOTA_FD_USE]
    strb    w4, [x1, #17]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * Helper: _qt_memset
 * --------------------------------------------------------------------------- */
_qt_memset:
    cbz     x1, 2f
1:  strb    wzr, [x0], #1
    sub     x1, x1, #1
    cbnz    x1, 1b
2:  ret
