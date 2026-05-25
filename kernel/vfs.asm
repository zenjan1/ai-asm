/*
 * aiasm-aarch64/kernel/vfs.asm
 * Virtual Filesystem layer v11.0
 * Per-process fd tables: 16 procs × 32 fds each
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set VFS_MAX_PROCS,      16
.set VFS_MAX_FD,         32      /* per-process fd count */
.set VFS_ENTRY_SIZE,     24      /* fd_num(4) + type(4) + flags(4) + ops_ptr(8) */
.set VFS_PROC_ENTRY_SIZE, VFS_MAX_FD * VFS_ENTRY_SIZE  /* 768 bytes per proc */

.set VFS_FD_NUM,         0
.set VFS_TYPE,           4
.set VFS_FLAGS,          8
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
vfs_proc_table:
    .skip VFS_MAX_PROCS * VFS_PROC_ENTRY_SIZE
vfs_initialized:
    .word 0

.text

/* -----------------------------------------------------------------------------
 * _vfs_proc_base(pid) → x0 = base address of proc's fd table
 * w0 = pid (0..15)
 * ----------------------------------------------------------------------------- */
_vfs_proc_base:
    stp     x29, x30, [sp, #-16]!
    adrp    x1, vfs_proc_table
    add     x1, x1, #:lo12:vfs_proc_table
    cmp     w0, #VFS_MAX_PROCS
    b.ge    _vfs_proc_bad
    uxtw    x2, w0
    mov     x3, #VFS_PROC_ENTRY_SIZE
    madd    x0, x2, x3, x1
    ldp     x29, x30, [sp], #16
    ret

_vfs_proc_bad:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _vfs_find_slot(pid, fd) → x0 = pointer to entry or 0
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
_vfs_find_slot:
    stp     x29, x30, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */

    mov     w0, w20
    bl      _vfs_proc_base
    cbz     x0, _vfs_find_fail

    cmp     w21, #VFS_MAX_FD
    b.ge    _vfs_find_fail

    uxtw    x1, w21
    mov     x2, #VFS_ENTRY_SIZE
    madd    x0, x1, x2, x0        /* x0 = proc_base + fd * 24 */

    ldr     w2, [x0, #VFS_FD_NUM]
    cbz     w2, _vfs_find_fail    /* fd_num == 0 means unused */

    ldp     x29, x30, [sp], #16
    ret

_vfs_find_fail:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_init — zero all proc tables
 * ----------------------------------------------------------------------------- */
.global vfs_init
vfs_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, vfs_proc_table
    add     x0, x0, #:lo12:vfs_proc_table
    mov     x2, #(VFS_MAX_PROCS * VFS_PROC_ENTRY_SIZE)
    mov     w1, #0
    bl      _vfs_memset

    adrp    x0, vfs_initialized
    add     x0, x0, #:lo12:vfs_initialized
    mov     w1, #1
    str     w1, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_alloc_fd(pid, type, flags) → fd or -1
 * w0 = pid, w1 = type, w2 = flags
 * ----------------------------------------------------------------------------- */
.global vfs_alloc_fd
vfs_alloc_fd:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* type */
    mov     w22, w2               /* flags */

    /* Get proc base */
    mov     w0, w20
    bl      _vfs_proc_base
    cbz     x0, _vfs_alloc_fail

    /* Find free slot */
    mov     x1, x0               /* base */
    mov     x2, #0               /* index */

_vfs_alloc_loop:
    cmp     x2, #VFS_MAX_FD
    b.ge    _vfs_alloc_fail

    ldr     w3, [x1, #VFS_FD_NUM]
    cbz     w3, _vfs_alloc_found /* fd_num == 0 means free */

    add     x1, x1, #VFS_ENTRY_SIZE
    add     x2, x2, #1
    b       _vfs_alloc_loop

_vfs_alloc_found:
    /* x1 = slot base, x2 = fd number */
    add     w3, w2, #1           /* fd_num = index+1 (0 reserved) */
    str     w3, [x1, #VFS_FD_NUM]
    str     w21, [x1, #VFS_TYPE]
    str     w22, [x1, #VFS_FLAGS]
    str     xzr, [x1, #VFS_OPS_PTR]

    mov     w0, w2               /* return fd number */
    b       _vfs_alloc_done

_vfs_alloc_fail:
    mov     w0, #-1

_vfs_alloc_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_free_fd(pid, fd) → 0=ok, -1=invalid
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_free_fd
vfs_free_fd:
    stp     x29, x30, [sp, #-16]!

    bl      _vfs_find_slot
    cbz     x0, _vfs_free_fail

    /* Clear entry */
    mov     x1, x0
    mov     w1, #0
    mov     x2, #VFS_ENTRY_SIZE
    bl      _vfs_memset

    mov     w0, #0
    b       _vfs_free_done

_vfs_free_fail:
    mov     w0, #-1

_vfs_free_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_free_all(pid) — free all fds for a process (on exit)
 * w0 = pid
 * ----------------------------------------------------------------------------- */
.global vfs_free_all
vfs_free_all:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0

    mov     w0, w20
    bl      _vfs_proc_base
    cbz     x0, _vfs_free_all_done

    mov     x1, x0               /* base */
    mov     x2, #0               /* index */

_vfs_free_all_loop:
    cmp     x2, #VFS_MAX_FD
    b.ge    _vfs_free_all_done

    /* Clear this entry */
    mov     w3, #0
    str     w3, [x1, #VFS_FD_NUM]
    str     xzr, [x1, #VFS_OPS_PTR]

    add     x1, x1, #VFS_ENTRY_SIZE
    add     x2, x2, #1
    b       _vfs_free_all_loop

_vfs_free_all_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_set_ops(pid, fd, ops_ptr) → 0=ok, -1=invalid
 * w0 = pid, w1 = fd, x2 = ops_ptr
 * ----------------------------------------------------------------------------- */
.global vfs_set_ops
vfs_set_ops:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     w21, w1               /* fd */
    mov     x22, x2               /* ops_ptr */

    mov     w0, w20
    mov     w1, w21
    bl      _vfs_find_slot
    cbz     x0, _vfs_set_fail

    str     x22, [x0, #VFS_OPS_PTR]
    mov     w0, #0
    b       _vfs_set_done

_vfs_set_fail:
    mov     w0, #-1

_vfs_set_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_get_ops(pid, fd) → ops_ptr or 0
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_get_ops
vfs_get_ops:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1

    mov     w0, w20
    mov     w1, w21
    bl      _vfs_find_slot
    cbz     x0, _vfs_get_fail

    ldr     x0, [x0, #VFS_OPS_PTR]
    b       _vfs_get_done

_vfs_get_fail:
    mov     x0, #0

_vfs_get_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_get_type(pid, fd) → type or -1
 * w0 = pid, w1 = fd
 * ----------------------------------------------------------------------------- */
.global vfs_get_type
vfs_get_type:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0
    mov     w21, w1

    mov     w0, w20
    mov     w1, w21
    bl      _vfs_find_slot
    cbz     x0, _vfs_type_fail

    ldr     w0, [x0, #VFS_TYPE]
    b       _vfs_type_done

_vfs_type_fail:
    mov     w0, #-1

_vfs_type_done:
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vfs_list(pid, buf_off) → count of active fds
 * w0 = pid, x1 = buf_off (writes fd_num, type pairs)
 * ----------------------------------------------------------------------------- */
.global vfs_list
vfs_list:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    mov     w20, w0               /* pid */
    mov     x21, x1               /* buf_off */

    mov     w0, w20
    bl      _vfs_proc_base
    cbz     x0, _vfs_list_done

    mov     x1, x0               /* base */
    mov     x2, #0               /* index */
    mov     w3, #0               /* count */

_vfs_list_loop:
    cmp     x2, #VFS_MAX_FD
    b.ge    _vfs_list_done

    ldr     w4, [x1, #VFS_FD_NUM]
    cbz     w4, _vfs_list_next   /* skip unused */

    /* Write fd_num (4 bytes) to buf_off + count*8 */
    lsl     x5, x3, #3           /* count * 8 */
    add     x5, x21, x5
    str     w4, [x5]

    /* Write type (4 bytes) to buf_off + count*8 + 4 */
    ldr     w6, [x1, #VFS_TYPE]
    str     w6, [x5, #4]

    add     w3, w3, #1

_vfs_list_next:
    add     x1, x1, #VFS_ENTRY_SIZE
    add     x2, x2, #1
    b       _vfs_list_loop

_vfs_list_done:
    mov     w0, w3
    ldp     x20, x21, [sp], #16
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
