/*
 * aiasm-aarch64/kernel/vfs_ops.asm
 * Virtual Filesystem Operations v10.0
 * Unified read/write/close/poll routing based on fd type
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Imports from other subsystems
 * ----------------------------------------------------------------------------- */
/* fs.asm */
/* fs_read(fd_or_path_info, buf_off, len) → bytes_read or -1 */
/* fs_write(fd_or_path_info, buf_off, len) → bytes_written or -1 */
/* fs_close(fd) → 0 or -1 */

/* ipc.asm */
/* pipe_read(fd, buf, len) → bytes_read or -1 */
/* pipe_write(fd, buf, len) → bytes_written or -1 */
/* pipe_close(fd) → 0 or -1 */

/* net.asm */
/* net_send(fd, buf, len) → bytes_sent or -1 */
/* net_recv(fd, buf, len) → bytes_recv or -1 */
/* net_close(fd) → 0 or -1 */

/* shmem.asm */
/* shmem_read(fd, buf_off, len) → bytes_read or -1 */
/* shmem_write(fd, buf_off, len) → bytes_written or -1 */

/* vfs.asm */
/* vfs_get_type(fd) → type or -1 */

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
 * vfs_read(fd, buf_off, len) → bytes_read or -1
 * Routes based on fd type to appropriate read handler
 * x0 = fd, x1 = buf_off, w2 = len
 * ----------------------------------------------------------------------------- */
.global vfs_read
vfs_read:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* save fd */
    mov     x21, x1               /* save buf_off */
    mov     w22, w2               /* save len */

    /* Get fd type */
    mov     w0, w20
    bl      vfs_get_type
    cbz     x0, _vfs_read_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_read_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_read_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_read_shmem

    /* Unknown type */
    mov     w0, #-1
    b       _vfs_read_done

_vfs_read_file:
    /* Route to fs_read */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      fs_read
    b       _vfs_read_done

_vfs_read_pipe:
    /* Route to pipe_read */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      pipe_read
    b       _vfs_read_done

_vfs_read_socket:
    /* Route to net_recv */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      net_recv
    b       _vfs_read_done

_vfs_read_shmem:
    /* Shared memory: return region size as 'readable' — WASM uses shmem_get_ptr directly */
    mov     w0, #-1               /* shmem not readable via vfs_read */

_vfs_read_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_write(fd, buf_off, len) → bytes_written or -1
 * Routes based on fd type to appropriate write handler
 * x0 = fd, x1 = buf_off, w2 = len
 * ----------------------------------------------------------------------------- */
.global vfs_write
vfs_write:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* save fd */
    mov     x21, x1               /* save buf_off */
    mov     w22, w2               /* save len */

    /* Get fd type */
    mov     w0, w20
    bl      vfs_get_type
    cbz     x0, _vfs_write_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_write_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_write_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_write_shmem

    /* Unknown type */
    mov     w0, #-1
    b       _vfs_write_done

_vfs_write_file:
    /* Route to fs_file_write (C function in wasm_host.c) */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      fs_file_write
    b       _vfs_write_done

_vfs_write_pipe:
    /* Route to pipe_write */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      pipe_write
    b       _vfs_write_done

_vfs_write_socket:
    /* Route to net_send */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    bl      net_send
    b       _vfs_write_done

_vfs_write_shmem:
    /* Shared memory: WASM uses shmem_get_ptr for direct access */
    mov     w0, #-1               /* shmem not writable via vfs_write */

_vfs_write_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_close(fd) → 0=ok, -1=invalid
 * Routes based on fd type to appropriate close handler, then frees VFS fd
 * x0 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_close
vfs_close:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* save fd */

    /* Get fd type before freeing */
    mov     w0, w20
    bl      vfs_get_type
    mov     w21, w0               /* save type */

    /* Type-specific close */
    cbz     w21, _vfs_close_file
    cmp     w21, #VFS_TYPE_PIPE
    b.eq    _vfs_close_pipe
    cmp     w21, #VFS_TYPE_SOCKET
    b.eq    _vfs_close_socket
    cmp     w21, #VFS_TYPE_SHMEM
    b.eq    _vfs_close_shmem

    /* Unknown type — just free the VFS slot */
    b       _vfs_close_free

_vfs_close_file:
    mov     w0, w20
    bl      fs_close
    b       _vfs_close_free

_vfs_close_pipe:
    mov     w0, w20
    bl      pipe_close
    b       _vfs_close_free

_vfs_close_socket:
    mov     w0, w20
    bl      net_close
    b       _vfs_close_free

_vfs_close_shmem:
    /* Shared memory: detach region (uses shmem_detach via host function) */
    /* For now, just return success — WASM explicitly detaches via shmem_detach */
    mov     w0, #0

_vfs_close_free:
    /* Free VFS fd slot regardless of type close result */
    mov     w0, w20
    bl      vfs_free_fd

    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_poll(fd) → poll_flags (READ=1, WRITE=2, or 0)
 * Checks if fd is readable/writable based on type
 * x0 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_poll
vfs_poll:
    stp     x29, x30, [sp, #-16]!

    /* Get fd type */
    bl      vfs_get_type
    cbz     x0, _vfs_poll_file
    cmp     w0, #VFS_TYPE_PIPE
    b.eq    _vfs_poll_pipe
    cmp     w0, #VFS_TYPE_SOCKET
    b.eq    _vfs_poll_socket
    cmp     w0, #VFS_TYPE_SHMEM
    b.eq    _vfs_poll_shmem

    /* Unknown type */
    mov     w0, #0
    b       _vfs_poll_done

_vfs_poll_file:
    /* Files are always readable/writable in RAM disk model */
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_pipe:
    /* Check if pipe has data (readable) or space (writable) */
    /* For now, return both — the actual pipe_read/write handle full/empty */
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_socket:
    /* Socket readiness depends on TCP state — simplified: always ready */
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)
    b       _vfs_poll_done

_vfs_poll_shmem:
    /* Shared memory always ready for read/write */
    mov     w0, #(VFS_POLL_READ | VFS_POLL_WRITE)

_vfs_poll_done:
    ldp     x29, x30, [sp], #16
    ret
