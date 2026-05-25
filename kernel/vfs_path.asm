/*
 * aiasm-aarch64/kernel/vfs_path.asm
 * VFS path resolution v13.0 — thin assembly wrapper, C does parsing
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set VFS_TYPE_FILE,      0
.set VFS_TYPE_PIPE,      1
.set VFS_TYPE_SOCKET,    2
.set VFS_TYPE_SHMEM,     3

.set VFS_FLAG_READ,      1
.set VFS_FLAG_WRITE,     2
.set VFS_FLAG_NONBLOCK,  4

.set VFS_MAX_FD,         32
.set VFS_ENTRY_SIZE,     24
.set VFS_FD_NUM,         0
.set VFS_TYPE,           4
.set VFS_FLAGS,          8
.set VFS_OPS_PTR,        16

.text

/* -----------------------------------------------------------------------------
 * vfs_open(pid, path_ptr, path_len, flags) → fd or -1
 * w0 = pid, x1 = path_ptr, w2 = path_len, w3 = flags
 * Thin wrapper: save regs and call C _vfs_path_open helper
 * ----------------------------------------------------------------------------- */
.global vfs_open
vfs_open:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     x21, x1               /* path_ptr */
    mov     w22, w2               /* path_len */
    mov     w23, w3               /* flags */

    /* Call C: _vfs_path_open(pid, path_ptr, path_len, flags) */
    mov     w0, w20
    mov     x1, x21
    mov     w2, w22
    mov     w3, w23
    bl      _vfs_path_open

    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_list_all(pid, buf_off, max_count) → count
 * Lists all active fds for a process (debug/management)
 * w0 = pid, x1 = buf_off, w2 = max_count
 * Each entry: fd_num(4) + type(4) = 8 bytes
 * ----------------------------------------------------------------------------- */
.global vfs_list_all
vfs_list_all:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     x21, x1
    mov     w22, w2

    /* Get proc base */
    mov     w0, w20
    bl      vfs_proc_base
    cbz     x0, _vfs_list_all_done

    mov     x1, x0               /* base */
    mov     x2, #0               /* index */
    mov     w3, #0               /* count */

_vfs_list_all_loop:
    cmp     x2, #VFS_MAX_FD
    b.ge    _vfs_list_all_done

    cmp     w3, w22
    b.ge    _vfs_list_all_done

    ldr     w4, [x1, #VFS_FD_NUM]
    cbnz    w4, 1f
    b       _vfs_list_all_next

1:  /* Write fd_num and type to buffer */
    lsl     x5, x3, #3           /* count * 8 */
    add     x5, x21, x5
    str     w4, [x5]
    ldr     w6, [x1, #VFS_TYPE]
    str     w6, [x5, #4]
    add     w3, w3, #1

_vfs_list_all_next:
    add     x1, x1, #VFS_ENTRY_SIZE
    add     x2, x2, #1
    b       _vfs_list_all_loop

_vfs_list_all_done:
    mov     w0, w3
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
