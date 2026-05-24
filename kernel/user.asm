/*
 * aiasm-aarch64/kernel/user.asm
 * User management kernel layer for v6.0
 * User table storage with default root user.
 */
.arch armv8-a

/* ---------------------------------------------------------------------------
 * User table: 4 slots x 25 bytes each
 *   offset 0-15:  username (16 bytes, null-padded)
 *   offset 16-19: password hash (uint32)
 *   offset 20:    permission level (uint8: 0=root,1=admin,2=user,3=guest)
 * --------------------------------------------------------------------------- */
.data
.align 4
.global user_table
user_table:
    .skip 100                       /* 4 users x 25 bytes */

.global user_current
user_current:
    .word -1                        /* current logged-in user uid (-1 = none) */

.global user_count
user_count:
    .word 1                         /* number of registered users (starts at 1 for root) */

/* ---------------------------------------------------------------------------
 * user_init: Initialize default root user (name="root", pass_hash=0, level=0)
 * --------------------------------------------------------------------------- */
.text
.global user_init
user_init:
    ldr x0, =user_table             /* x0 = user_table base */

    /* Set username = "root" */
    ldr x1, =_root_name
    ldp x2, x3, [x1]                /* load "root\0\0\0\0" + padding */
    stp x2, x3, [x0]

    /* Set pass_hash = 0 */
    mov w2, #0
    str w2, [x0, #16]

    /* Set level = 0 (root) */
    mov w3, #0
    strb w3, [x0, #20]

    ret

.section .rodata
.align 4
_root_name:
    .asciz "root"
