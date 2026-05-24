/*
 * aiasm-aarch64/kernel/wasi.asm
 * WASI system call interface (assembly layer)
 * Provides syscall dispatch and core WASI helper functions
 */
.arch armv8-a

/* WASI syscall numbers (wasi-snapshot-preview1) */
.set WASI_ARGS_sizes_get,    0
.set WASI_ENVIRON_SIZES_GET, 3
.set WASI_FD_WRITE,          4
.set WASI_FD_READ,           5
.set WASI_FD_CLOSE,          6
.set WASI_FD_SEEK,           7
.set WASI_FD_TELL,           8
.set WASI_PATH_OPEN,         9
.set WASI_PATH_REMOVEFILE,   11
.set WASI_RANDOM_GET,        13
.set WASI_CLOCK_TIME_GET,    14
.set WASI_PROC_EXIT,         15
.set WASI_PROC_SPAWN,        16

/* WASI error codes */
.set WASI_ESUCCESS,     0
.set WASI_EBADF,        8
.set WASI_EINVAL,       28
.set WASI_ENOSYS,       52
.set WASI_ENOTSUP,      58

/* File descriptor numbers */
.set WASI_FD_STDIN,     0
.set WASI_FD_STDOUT,    1
.set WASI_FD_STDERR,    2

.text

/* -----------------------------------------------------------------------------
 * Function: wasi_dispatch
 * WASI syscall dispatcher (called from wasm3 trampolines)
 * w8 = syscall number, x0-x3 = arguments
 * Returns x0 = wasi_errno
 * ----------------------------------------------------------------------------- */
.global wasi_dispatch
wasi_dispatch:
    /* w8 = syscall number */
    cmp     w8, #WASI_CLOCK_TIME_GET
    b.eq    wasi_clock_time_get

    cmp     w8, #WASI_FD_CLOSE
    b.eq    wasi_fd_close

    cmp     w8, #WASI_FD_READ
    b.eq    wasi_fd_read_asm

    cmp     w8, #WASI_FD_SEEK
    b.eq    wasi_fd_seek

    cmp     w8, #WASI_FD_TELL
    b.eq    wasi_fd_tell

    cmp     w8, #WASI_FD_WRITE
    b.eq    wasi_fd_write_asm

    cmp     w8, #WASI_PATH_OPEN
    b.eq    wasi_path_open

    cmp     w8, #WASI_PATH_REMOVEFILE
    b.eq    wasi_path_removefile

    cmp     w8, #WASI_PROC_EXIT
    b.eq    wasi_proc_exit_asm

    cmp     w8, #WASI_PROC_SPAWN
    b.eq    wasi_proc_spawn_asm

    cmp     w8, #WASI_RANDOM_GET
    b.eq    wasi_random_get

    cmp     w8, #WASI_ARGS_sizes_get
    b.eq    wasi_args_sizes_get

    cmp     w8, #WASI_ENVIRON_SIZES_GET
    b.eq    wasi_environ_sizes_get

    /* Unsupported syscall */
    mov     x0, #WASI_ENOSYS
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_write_asm: write to file descriptor
 * x0 = fd, x1 = iovs_ptr, x2 = iovs_len, x3 = nwritten_out
 * ----------------------------------------------------------------------------- */
wasi_fd_write_asm:
    stp     x29, x30, [sp, #-16]!

    /* Stdout/stderr: write to UART */
    cmp     w0, #WASI_FD_STDOUT
    b.eq    wasi_fd_write_uart
    cmp     w0, #WASI_FD_STDERR
    b.eq    wasi_fd_write_uart

    /* Regular fd: call host_fs_write via C */
    mov     x8, x0              /* save fd */
    mov     x9, x1              /* save iovs */
    mov     x10, x2             /* save iovs_len */
    mov     x11, x3             /* save nwritten_out */

    /* Accumulate bytes written from iovec */
    mov     x12, #0             /* total written */
    mov     x13, #0             /* iov index */

_wasi_fdw_loop:
    cmp     x13, x10
    b.ge    _wasi_fdw_done

    /* Read iovec: [buf_ptr, buf_len] (8 bytes each) */
    add     x0, x9, x13, lsl #4
    ldr     x1, [x0]            /* buf_ptr */
    ldr     w2, [x0, #8]        /* buf_len */

    /* Call host_fd_write_c(fd, buf, len) */
    mov     x0, x8
    bl      host_fd_write_c
    cmp     w0, #0
    b.lt    _wasi_fdw_error

    add     x12, x12, x0
    add     x13, x13, #1
    b       _wasi_fdw_loop

_wasi_fdw_done:
    /* Write total to nwritten_out */
    str     w12, [x11]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

_wasi_fdw_error:
    str     wzr, [x11]
    mov     x0, #WASI_EBADF
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_write_uart: write iovec to UART (stdout/stderr)
 * ----------------------------------------------------------------------------- */
wasi_fd_write_uart:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x1              /* iovs_ptr */
    mov     x5, x2              /* iovs_len */
    mov     x6, x3              /* nwritten_out */

    mov     x7, #0              /* total written */
    mov     x8, #0              /* iov index */

_wasi_uart_loop:
    cmp     x8, x5
    b.ge    _wasi_uart_done

    /* Read iovec entry */
    add     x0, x4, x8, lsl #4
    ldr     x1, [x0]            /* buf_ptr */
    ldr     w2, [x0, #8]        /* buf_len */

    /* Call serial_puts_buf(x1=buf, x2=len) */
    bl      serial_puts_buf

    add     x7, x7, x2
    add     x8, x8, #1
    b       _wasi_uart_loop

_wasi_uart_done:
    str     w7, [x6]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_read_asm: read from file descriptor
 * x0 = fd, x1 = iovs_ptr, x2 = iovs_len, x3 = nread_out
 * ----------------------------------------------------------------------------- */
wasi_fd_read_asm:
    stp     x29, x30, [sp, #-16]!

    /* Stdin: read from UART */
    cmp     w0, #WASI_FD_STDIN
    b.eq    wasi_fd_read_uart

    /* Regular fd: call host_fd_read_c */
    mov     x8, x0              /* save fd */
    mov     x9, x1              /* save iovs */
    mov     x10, x2             /* save iovs_len */
    mov     x11, x3             /* save nread_out */

    mov     x12, #0             /* total read */
    mov     x13, #0             /* iov index */

_wasi_fdr_loop:
    cmp     x13, x10
    b.ge    _wasi_fdr_done

    add     x0, x9, x13, lsl #4
    ldr     x1, [x0]            /* buf_ptr */
    ldr     w2, [x0, #8]        /* buf_len */

    mov     x0, x8
    bl      host_fd_read_c
    cmp     w0, #0
    b.lt    _wasi_fdr_error

    add     x12, x12, x0
    add     x13, x13, #1
    b       _wasi_fdr_loop

_wasi_fdr_done:
    str     w12, [x11]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

_wasi_fdr_error:
    str     wzr, [x11]
    mov     x0, #WASI_EBADF
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_read_uart: read from UART (stdin)
 * ----------------------------------------------------------------------------- */
wasi_fd_read_uart:
    stp     x29, x30, [sp, #-16]!
    mov     x4, x1              /* iovs_ptr */
    mov     x5, x2              /* iovs_len */
    mov     x6, x3              /* nread_out */

    /* Only read into first iovec for simplicity */
    cmp     x5, #0
    b.eq    _wasi_uart_rd_none

    ldr     x1, [x4]            /* buf_ptr */
    ldr     w2, [x4, #8]        /* buf_len */

    mov     x7, #0              /* bytes read */
    mov     x8, x1              /* current write position */
    mov     x9, x2              /* remaining */

_wasi_uart_rd_loop:
    cbz     x9, _wasi_uart_rd_done
    bl      host_getc_asm
    cmp     w0, #0
    b.lt    _wasi_uart_rd_done  /* no data available */

    strb    w0, [x8]
    add     x8, x8, #1
    add     x7, x7, #1
    sub     x9, x9, #1
    b       _wasi_uart_rd_loop

_wasi_uart_rd_done:
    str     w7, [x6]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

_wasi_uart_rd_none:
    str     wzr, [x6]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_close: close file descriptor
 * x0 = fd
 * ----------------------------------------------------------------------------- */
wasi_fd_close:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #WASI_FD_STDIN
    b.eq    wasi_fd_close_ok
    cmp     w0, #WASI_FD_STDOUT
    b.eq    wasi_fd_close_ok
    cmp     w0, #WASI_FD_STDERR
    b.eq    wasi_fd_close_ok

    /* Regular fd: call host_fd_close_c */
    bl      host_fd_close_c
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

wasi_fd_close_ok:
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_seek: seek within file
 * x0 = fd, x1 = offset (i64), x3 = newoffset_out
 * ----------------------------------------------------------------------------- */
wasi_fd_seek:
    /* Not implemented for RAM disk (read-only) */
    mov     x0, #WASI_ENOTSUP
    ret

/* -----------------------------------------------------------------------------
 * wasi_fd_tell: get current file position
 * x0 = fd, x1 = position_out
 * ----------------------------------------------------------------------------- */
wasi_fd_tell:
    /* Not implemented for RAM disk */
    mov     x0, #WASI_ENOTSUP
    ret

/* -----------------------------------------------------------------------------
 * wasi_path_open: open file by path
 * x0 = dirfd, x1 = flags, x2 = path_ptr, x3 = path_len
 * ----------------------------------------------------------------------------- */
wasi_path_open:
    stp     x29, x30, [sp, #-16]!

    /* Call host_path_open_c(dirfd, flags, path, len) */
    bl      host_path_open_c

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_path_removefile: delete a file
 * x0 = path_ptr, x1 = path_len
 * ----------------------------------------------------------------------------- */
wasi_path_removefile:
    mov     x0, #WASI_ENOTSUP
    ret

/* -----------------------------------------------------------------------------
 * wasi_proc_exit_asm: terminate process
 * x0 = exit_code
 * ----------------------------------------------------------------------------- */
wasi_proc_exit_asm:
    /* Call host_proc_exit_c */
    b       host_proc_exit_c

/* -----------------------------------------------------------------------------
 * wasi_proc_spawn_asm: spawn new process
 * x0 = path_ptr, x1 = path_len
 * ----------------------------------------------------------------------------- */
wasi_proc_spawn_asm:
    stp     x29, x30, [sp, #-16]!
    bl      host_proc_spawn_c
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_random_get: fill buffer with random bytes
 * x0 = buf, x1 = len
 * ----------------------------------------------------------------------------- */
wasi_random_get:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* buf */
    mov     x9, x1              /* len */

    cbz     x9, 2f

1:  /* Read cycle counter for entropy */
    mrs     x10, cntvct_el0
    /* Simple XOR-based PRNG */
    ldr     x11, =wasi_rand_state
    ldr     w12, [x11]
    eor     x12, x12, x10, lsr #16
    eor     x12, x12, x10, lsr #32
    eor     x12, x12, x10, lsr #48
    mul     w12, w12, w12       /* mix */
    add     w12, w12, #1
    str     w12, [x11]

    strb    w12, [x8], #1
    sub     x9, x9, #1
    cbnz    x9, 1b

2:  mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_clock_time_get: get current time
 * x0 = clock_id, x1 = precision (ignored), x2 = time_out (i64)
 * ----------------------------------------------------------------------------- */
wasi_clock_time_get:
    stp     x29, x30, [sp, #-16]!

    /* Use cntvct_el0 for real time */
    mrs     x0, cntvct_el0
    mrs     x1, cntfrq_el0
    /* Convert ticks to nanoseconds */
    ldr     x3, =1000000000
    mul     x0, x0, x3
    udiv    x0, x0, x1

    str     x0, [x2]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_args_sizes_get: stub (no args)
 * x0 = argc_out, x1 = argv_buf_size_out
 * ----------------------------------------------------------------------------- */
wasi_args_sizes_get:
    stp     x29, x30, [sp, #-16]!
    str     wzr, [x0]
    str     wzr, [x1]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_environ_sizes_get: stub (no env vars)
 * x0 = environ_count_out, x1 = environ_buf_size_out
 * ----------------------------------------------------------------------------- */
wasi_environ_sizes_get:
    stp     x29, x30, [sp, #-16]!
    str     wzr, [x0]
    str     wzr, [x1]
    mov     x0, #WASI_ESUCCESS
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasi_init: initialize WASI subsystem
 * ----------------------------------------------------------------------------- */
.global wasi_init
wasi_init:
    /* Initialize random state from timer */
    mrs     x0, cntvct_el0
    ldr     x1, =wasi_rand_state
    str     w0, [x1]
    ret

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 4
wasi_rand_state:
    .skip 4
