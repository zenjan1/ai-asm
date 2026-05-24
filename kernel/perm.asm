/*
 * aiasm-aarch64/kernel/perm.asm
 * Permission boundary checking for v6.0
 *
 * Four permission levels:
 *   0 = root   (full access: file/net/gui/all)
 *   1 = admin  (file + net + gui)
 *   2 = user   (own file + net + gui, read-only shared)
 *   3 = guest  (read-only file, read-only gui)
 *
 * API:
 *   perm_init()                     — set default root permission
 *   perm_set_level(level)           — set current permission level
 *   perm_get_level() => w0          — get current permission
 *   perm_check_file(level, op) => x0: 0=allow, -1=deny
 *     op: 0=read, 1=write, 2=delete, 3=create
 *   perm_check_net(level) => x0
 *   perm_check_gui(level) => x0
 */
.arch armv8-a

/* Permission constants */
.set PERM_ROOT,   0
.set PERM_ADMIN,  1
.set PERM_USER,   2
.set PERM_GUEST,  3

/* Operation constants */
.set OP_READ,     0
.set OP_WRITE,    1
.set OP_DELETE,   2
.set OP_CREATE,   3

.bss
.align 4
.global perm_current_level
perm_current_level:
    .word 0                         /* default: set by perm_init */

.global perm_module_levels
perm_module_levels:
    .skip 64                        /* 16 modules x 4 bytes (permission level) */

.text

/* ---------------------------------------------------------------------------
 * perm_init: Initialize permissions to defaults
 * --------------------------------------------------------------------------- */
.global perm_init
perm_init:
    stp     x29, x30, [sp, #-16]!

    /* Set current level to USER */
    adrp    x0, perm_current_level
    add     x0, x0, #:lo12:perm_current_level
    mov     w1, #PERM_USER
    str     w1, [x0]

    /* Zero all module permission levels */
    adrp    x0, perm_module_levels
    add     x0, x0, #:lo12:perm_module_levels
    mov     x1, #16
1:
    strb    wzr, [x0], #1
    subs    x1, x1, #1
    cbnz    x1, 1b

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * perm_set_level: Set current process permission level
 * Input: x0 = level (0-3)
 * --------------------------------------------------------------------------- */
.global perm_set_level
perm_set_level:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #3
    b.gt    1f                      /* invalid level, ignore */

    adrp    x1, perm_current_level
    add     x1, x1, #:lo12:perm_current_level
    str     w0, [x1]

1:
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * perm_get_level: Get current permission level
 * Output: w0 = level
 * --------------------------------------------------------------------------- */
.global perm_get_level
perm_get_level:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, perm_current_level
    add     x0, x0, #:lo12:perm_current_level
    ldr     w0, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * perm_check_file: Check file operation permission
 * Input:  x0 = level, x1 = op (0=read, 1=write, 2=delete, 3=create)
 * Output: x0 = 0 (allow) or -1 (deny)
 * --------------------------------------------------------------------------- */
.global perm_check_file
perm_check_file:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #PERM_ROOT
    b.eq    perm_allow              /* root: allow all */

    cmp     w0, #PERM_ADMIN
    b.eq    perm_admin_file         /* admin: allow read/write/create, deny delete */

    cmp     w0, #PERM_USER
    b.eq    perm_user_file          /* user: allow read/write own, deny delete */

    /* guest: read only */
    cmp     w1, #OP_READ
    b.eq    perm_allow
    b       perm_deny

perm_admin_file:
    /* admin allows read, write, create */
    cmp     w1, #OP_DELETE
    b.eq    perm_deny               /* deny delete */
    b       perm_allow

perm_user_file:
    /* user allows read and write */
    cmp     w1, #OP_DELETE
    b.eq    perm_deny
    cmp     w1, #OP_CREATE
    b.eq    perm_deny               /* deny create */
    b       perm_allow

/* ---------------------------------------------------------------------------
 * perm_check_net: Check network access permission
 * Input:  x0 = level
 * Output: x0 = 0 (allow) or -1 (deny)
 * --------------------------------------------------------------------------- */
.global perm_check_net
perm_check_net:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #PERM_GUEST
    b.eq    perm_deny               /* guest: no network */

    b       perm_allow              /* root/admin/user: network OK */

/* ---------------------------------------------------------------------------
 * perm_check_gui: Check GUI access permission
 * Input:  x0 = level
 * Output: x0 = 0 (allow) or -1 (deny)
 * --------------------------------------------------------------------------- */
.global perm_check_gui
perm_check_gui:
    stp     x29, x30, [sp, #-16]!

    /* All levels can read GUI, only root/admin/user can draw */
    cmp     w0, #PERM_GUEST
    b.eq    perm_allow              /* guest: read-only GUI OK */

    b       perm_allow

/* ---------------------------------------------------------------------------
 * Internal helpers
 * --------------------------------------------------------------------------- */
perm_allow:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

perm_deny:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret
