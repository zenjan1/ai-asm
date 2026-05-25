/*
 * aiasm-aarch64/kernel/vfs_ops.asm
 * Virtual Filesystem Operations v11.0
 * Per-process fd routing: all functions take (pid, fd)
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set VFS_TYPE_FILE,      0
.set VFS_TYPE_PIPE,      1
.set VFS_TYPE_SOCKET,    2
.set VFS_TYPE_SHMEM,     3

.set VFS_POLL_READ,      1
.set VFS_POLL_WRITE,     2

.text

/* -----------------------------------------------------------------------------
 * vfs_read(pid, fd, buf_off, len) → bytes_read or -1
 * w0 = pid, w1 = fd, x2 = buf_off, w3 = len
 * ----------------------------------------------------------------------------- */
.global vfs_read
vfs_read:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */
    mov     x22, x2               /* buf_off */
    mov     w23, w3               /* len */

    /* Get fd type */
    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_type
    cbz     x0, _vfs_read_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_read_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_read_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_read_shmem

    mov     w0, #-1
    b       _vfs_read_done

_vfs_read_file:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_file_read
    b       _vfs_read_done

_vfs_read_pipe:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_pipe_read
    b       _vfs_read_done

_vfs_read_socket:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_socket_recv
    b       _vfs_read_done

_vfs_read_shmem:
    mov     w0, #-1

_vfs_read_done:
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_write(pid, fd, buf_off, len) → bytes_written or -1
 * w0 = pid, w1 = fd, x2 = buf_off, w3 = len
 * ----------------------------------------------------------------------------- */
.global vfs_write
vfs_write:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */
    mov     x22, x2               /* buf_off */
    mov     w23, w3               /* len */

    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_type
    cbz     x0, _vfs_write_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_write_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_write_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_write_shmem

    mov     w0, #-1
    b       _vfs_write_done

_vfs_write_file:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_file_write
    b       _vfs_write_done

_vfs_write_pipe:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_pipe_write
    b       _vfs_write_done

_vfs_write_socket:
    mov     w0, w20
    mov     w1, w21
    mov     x2, x22
    mov     w3, w23
    bl      _vfs_socket_send
    b       _vfs_write_done

_vfs_write_shmem:
    mov     w0, #-1

_vfs_write_done:
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_close(pid, fd) → 0=ok, -1=invalid
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_close
vfs_close:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */

    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_type
    mov     w22, w0               /* type */

    cbz     w22, _vfs_close_file
    cmp     w22, #VFS_TYPE_PIPE
    b.eq    _vfs_close_pipe
    cmp     w22, #VFS_TYPE_SOCKET
    b.eq    _vfs_close_socket
    cmp     w22, #VFS_TYPE_SHMEM
    b.eq    _vfs_close_shmem

    b       _vfs_close_free

_vfs_close_file:
    mov     w0, w20
    mov     w1, w21
    bl      _vfs_file_close
    b       _vfs_close_free

_vfs_close_pipe:
    mov     w0, w20
    mov     w1, w21
    bl      _vfs_pipe_close
    b       _vfs_close_free

_vfs_close_socket:
    mov     w0, w20
    mov     w1, w21
    bl      _vfs_socket_close
    b       _vfs_close_free

_vfs_close_shmem:
    mov     w0, #0

_vfs_close_free:
    mov     w0, w20
    mov     w1, w21
    bl      vfs_free_fd

    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_poll(pid, fd) → poll_flags
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_poll
vfs_poll:
    stp     x29, x30, [sp, #-16]!

    bl      vfs_get_type
    cbz     x0, _vfs_poll_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_poll_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_poll_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_poll_shmem

    mov     w0, #0
    b       _vfs_poll_done

_vfs_poll_file:
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_pipe:
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_socket:
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_shmem:
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)

_vfs_poll_done:
    ldp     x29, x30, [sp], #16
    ret

/* ===================================================================== */
/* Internal helpers — delegate to subsystem functions via ops_ptr       */
/* ===================================================================== */

/* -----------------------------------------------------------------------------
 * _vfs_file_read(pid, fd, buf_off, len)
 * Gets ramdisk fd from ops_ptr, calls ramdisk_read
 * ----------------------------------------------------------------------------- */
_vfs_file_read:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */
    mov     x22, x2               /* buf_off */
    mov     w23, w3               /* len */

    /* Get ops_ptr (ramdisk fd) */
    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_ops
    cbz     x0, _vfs_file_read_fail

    /* Call ramdisk_read(fd, buf, len) */
    mov     w0, w21               /* use fd directly (ramdisk fd) */
    /* Actually, ops_ptr holds the ramdisk fd number */
    mov     w0, w0                /* ops_ptr is in x0 */
    /* ops_ptr = ramdisk fd, call ramdisk_read */
    and     x24, x0, #0xffffffff  /* truncate to 32-bit fd */
    mov     x0, x24
    mov     x1, x22
    mov     w2, w23
    bl      ramdisk_read

    ldp     x29, x30, [sp], #16
    ret

_vfs_file_read_fail:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_file_write(pid, fd, buf_off, len)
 * ----------------------------------------------------------------------------- */
_vfs_file_write:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1
    mov     x22, x2
    mov     w23, w3

    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_ops
    cbz     x0, _vfs_file_write_fail

    mov     x24, x0
    mov     w0, w21
    mov     x1, x22
    mov     w2, w23
    bl      fs_file_write

    ldp     x29, x30, [sp], #16
    ret

_vfs_file_write_fail:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_file_close(pid, fd)
 * ----------------------------------------------------------------------------- */
_vfs_file_close:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1

    mov     w0, w20
    mov     w1, w21
    bl      vfs_get_ops
    cbz     x0, _vfs_file_close_ok

    mov     w0, w21
    bl      ramdisk_close

_vfs_file_close_ok:
    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_pipe_read(pid, fd, buf_off, len)
 * ----------------------------------------------------------------------------- */
_vfs_pipe_read:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1
    mov     x22, x2
    mov     w23, w3

    mov     w0, w21               /* pipe fd */
    mov     x1, x22               /* buf */
    mov     w2, w23               /* len */
    bl      pipe_read

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_pipe_write(pid, fd, buf_off, len)
 * ----------------------------------------------------------------------------- */
_vfs_pipe_write:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1
    mov     x22, x2
    mov     w23, w3

    mov     w0, w21
    mov     x1, x22
    mov     w2, w23
    bl      pipe_write

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_pipe_close(pid, fd)
 * ----------------------------------------------------------------------------- */
_vfs_pipe_close:
    stp     x29, x30, [sp, #-16]!
    mov     w0, w1               /* fd */
    bl      pipe_close
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_socket_send(pid, fd, buf_off, len)
 * ----------------------------------------------------------------------------- */
_vfs_socket_send:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1
    mov     x22, x2
    mov     w23, w3

    mov     w0, w21
    mov     x1, x22
    mov     w2, w23
    bl      net_send

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_socket_recv(pid, fd, buf_off, len)
 * ----------------------------------------------------------------------------- */
_vfs_socket_recv:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1
    mov     x22, x2
    mov     w23, w3

    mov     w0, w21
    mov     x1, x22
    mov     w2, w23
    bl      net_recv

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_socket_close(pid, fd)
 * ----------------------------------------------------------------------------- */
_vfs_socket_close:
    stp     x29, x30, [sp, #-16]!
    mov     w0, w1
    bl      net_close
    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret
