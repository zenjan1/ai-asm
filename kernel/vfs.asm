/*
 * aiasm-aarch64/kernel/vfs.asm
 * Virtual Filesystem layer v10.0
 * 64 fd slots, unified namespace for files/pipes/sockets/shmem
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set VFS_MAX_FDS,        64
.set VFS_ENTRY_SIZE,     24      /* fd_num(4) + owner_pid(4) + type(4) + flags(4) + ops_ptr(8) */
.set VFS_FD_NUM,         0
.set VFS_OWNER_PID,      4
.set VFS_TYPE,           8
.set VFS_FLAGS,          12
.set VFS_OPS_PTR,        16

/* Type constants */
.set VFS_TYPE_FILE,      0
.set VFS_TYPE_PIPE,      1
.set VFS_TYPE_SOCKET,    2
.set VFS_TYPE_SHMEM,     3

/* Flag constants */
.set VFS_FLAG_READ,      1
.set VFS_FLAG_WRITE,     2
.set VFS_FLAG_NONBLOCK,  4

/* -----------------------------------------------------------------------------
 * Data section
 * ----------------------------------------------------------------------------- */
.bss
.align 4
vfs_fd_table:
    .skip VFS_MAX_FDS * VFS_ENTRY_SIZE
vfs_next_fd:
    .word 0              /* next fd number to allocate */
vfs_fd_count:
    .word 0              /* number of active fds */

.text

/* -----------------------------------------------------------------------------
 * vfs_init — zero fd table
 * ----------------------------------------------------------------------------- */
.global vfs_init
vfs_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, vfs_fd_table
    add     x0, x0, #:lo12:vfs_fd_table
    mov     x2, #(VFS_MAX_FDS * VFS_ENTRY_SIZE)
    bl      _vfs_memset

    adrp    x0, vfs_next_fd
    add     x0, x0, #:lo12:vfs_next_fd
    str     wzr, [x0]

    adrp    x0, vfs_fd_count
    add     x0, x0, #:lo12:vfs_fd_count
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_find_slot(fd) → x0=pointer to entry or 0
 * x0 = fd number
 * ----------------------------------------------------------------------------- */
_vfs_find_slot:
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* save fd */

    adrp    x0, vfs_fd_table
    add     x0, x0, #:lo12:vfs_fd_table
    adrp    x2, vfs_fd_count
    add     x2, x2, #:lo12:vfs_fd_count
    ldr     w21, [x2]

    mov     x1, #0                /* index */
_vfs_find_loop:
    cmp     x1, x21
    b.ge    _vfs_find_fail

    ldr     w2, [x0, #VFS_FD_NUM]
    cmp     w2, w20
    b.eq    _vfs_find_ok

    add     x0, x0, #VFS_ENTRY_SIZE
    add     x1, x1, #1
    b       _vfs_find_loop

_vfs_find_ok:
    ldp     x20, x21, [sp], #16
    ret

_vfs_find_fail:
    mov     x0, #0
    ldp     x20, x21, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_alloc_fd(pid, type, flags) → fd or -1
 * x0 = pid, w1 = type, w2 = flags
 * ----------------------------------------------------------------------------- */
.global vfs_alloc_fd
vfs_alloc_fd:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* type */
    mov     x22, x2               /* flags */

    adrp    x0, vfs_fd_count
    add     x0, x0, #:lo12:vfs_fd_count
    ldr     w3, [x0]
    cmp     w3, #VFS_MAX_FDS
    b.ge    _vfs_alloc_full

    /* Find free slot */
    adrp    x0, vfs_fd_table
    add     x0, x0, #:lo12:vfs_fd_table
    mov     x1, #0

_vfs_alloc_find:
    cmp     x1, #VFS_MAX_FDS
    b.ge    _vfs_alloc_full

    ldr     w4, [x0, #VFS_FD_NUM]
    cbz     w4, _vfs_alloc_found  /* fd_num == 0 means free */

    add     x0, x0, #VFS_ENTRY_SIZE
    add     x1, x1, #1
    b       _vfs_alloc_find

_vfs_alloc_found:
    /* Get next fd number */
    adrp    x3, vfs_next_fd
    add     x3, x3, #:lo12:vfs_next_fd
    ldr     w4, [x3]
    add     w4, w4, #1
    cbz     w4, 1f                /* skip 0 (reserved) */
    str     w4, [x3]

1:  str     w4, [x0, #VFS_FD_NUM]
    str     w20, [x0, #VFS_OWNER_PID]
    str     w21, [x0, #VFS_TYPE]
    str     w22, [x0, #VFS_FLAGS]

    /* Increment count */
    adrp    x3, vfs_fd_count
    add     x3, x3, #:lo12:vfs_fd_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     w0, w4                /* return fd number */
    b       _vfs_alloc_done

_vfs_alloc_full:
    mov     w0, #-1

_vfs_alloc_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_free_fd(fd) → 0=ok, -1=invalid
 * x0 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_free_fd
vfs_free_fd:
    stp     x29, x30, [sp, #-16]!

    bl      _vfs_find_slot
    cbz     x0, _vfs_free_fail

    /* Clear entry */
    mov     x1, x0
    mov     w2, #VFS_ENTRY_SIZE
    bl      _vfs_memset

    /* Decrement count */
    adrp    x0, vfs_fd_count
    add     x0, x0, #:lo12:vfs_fd_count
    ldr     w1, [x0]
    sub     w1, w1, #1
    str     w1, [x0]

    mov     w0, #0
    b       _vfs_free_done

_vfs_free_fail:
    mov     w0, #-1

_vfs_free_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_set_ops(fd, ops_ptr) → 0=ok, -1=invalid
 * x0 = fd, x1 = ops_ptr
 * ----------------------------------------------------------------------------- */
.global vfs_set_ops
vfs_set_ops:
    stp     x29, x30, [sp, #-16]!
    mov     x20, x1               /* ops_ptr */

    bl      _vfs_find_slot
    cbz     x0, _vfs_set_fail

    str     x20, [x0, #VFS_OPS_PTR]
    mov     w0, #0
    b       _vfs_set_done

_vfs_set_fail:
    mov     w0, #-1

_vfs_set_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_get_ops(fd) → ops_ptr or 0
 * x0 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_get_ops
vfs_get_ops:
    stp     x29, x30, [sp, #-16]!

    bl      _vfs_find_slot
    cbz     x0, _vfs_get_fail

    ldr     x0, [x0, #VFS_OPS_PTR]
    b       _vfs_get_done

_vfs_get_fail:
    mov     x0, #0

_vfs_get_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_get_type(fd) → type or -1
 * x0 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_get_type
vfs_get_type:
    stp     x29, x30, [sp, #-16]!

    bl      _vfs_find_slot
    cbz     x0, _vfs_type_fail

    ldr     w0, [x0, #VFS_TYPE]
    b       _vfs_type_done

_vfs_type_fail:
    mov     w0, #-1

_vfs_type_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _vfs_memset (x0=ptr, w1=value, x2=count)
 * ----------------------------------------------------------------------------- */
_vfs_memset:
    cbz     x2, 1f
2:  strb    w1, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 2b
1:  ret
