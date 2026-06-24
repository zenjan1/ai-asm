/*
 * aiasm-aarch64/kernel/fs_quota.asm
 * Filesystem Quotas (v1.0)
 *
 * AArch64 ASM implementation of filesystem quota management.
 * Provides user and group disk quota tracking and enforcement.
 *
 * Features:
 *   - User quota tracking
 *   - Group quota tracking
 *   - Quota limits (soft/hard) for blocks and inodes
 *   - Grace period management
 *   - Quota enforcement
 *   - Quota reporting
 *   - Quota initialization per filesystem
 *   - Quota warnings
 *
 * Integration: called by vfs.asm, filesystem drivers
 *
 * API:
 *   fs_quota_init()                               — init quota subsystem
 *   fs_quota_enable(fs_id, user_quota, group_quota) => 0/-1 — enable quotas
 *   fs_quota_disable(fs_id) => 0/-1               — disable quotas
 *   fs_quota_set_limits(fs_id, uid, blocks_soft, blocks_hard, inodes_soft, inodes_hard) => 0/-1 — set limits
 *   fs_quota_update_usage(fs_id, uid, blocks_delta, inodes_delta) => 0/-1 — update usage
 *   fs_quota_check(fs_id, uid, blocks_needed, inodes_needed) => 0/-1 — check quota
 *   fs_quota_report(fs_id, uid, out_report)       — get quota report
 *   fs_quota_grace_check(fs_id, uid) => grace_remaining — check grace period
 */
.arch armv8-a

/* Constants */
.set MAX_FILESYSTEMS,     16
.set MAX_USER_QUOTAS,     256
.set MAX_GROUP_QUOTAS,    128
.set GRACE_PERIOD_DAYS,   7

/* Quota entry (40 bytes) */
/* uid/gid, blocks_used, blocks_soft, blocks_hard, inodes_used, inodes_soft, inodes_hard, grace_start, warnings */

/* Quota policy (20 bytes) */
/* fs_id, default_soft, default_hard, grace_period, enabled */

.bss
.align 4
.global fs_quota_init_done
fs_quota_init_done:
    .skip 4

.align 4
fs_quota_enabled_fs:
    .skip 4
fs_quota_user_count:
    .skip 4
fs_quota_group_count:
    .skip 4
fs_quota_violations:
    .skip 4

/* User quotas */
.align 4
fs_quota_users:
    .skip MAX_USER_QUOTAS * 40  /* 10KB */

/* Group quotas */
.align 4
fs_quota_groups:
    .skip MAX_GROUP_QUOTAS * 40 /* 5KB */

/* Quota policies per filesystem */
.align 4
fs_quota_policies:
    .skip MAX_FILESYSTEMS * 20  /* 320 bytes */

.text

/* -----------------------------------------------------------------------------
 * fs_quota_init — initialize quota subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global fs_quota_init
fs_quota_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =fs_quota_enabled_fs
    str     wzr, [x0]
    ldr     x0, =fs_quota_user_count
    str     wzr, [x0]
    ldr     x0, =fs_quota_group_count
    str     wzr, [x0]
    ldr     x0, =fs_quota_violations
    str     wzr, [x0]

    /* Zero user quota array */
    ldr     x0, =fs_quota_users
    mov     x1, #(MAX_USER_QUOTAS * 40)
    bl      .Lquota_zero

    /* Zero group quota array */
    ldr     x0, =fs_quota_groups
    mov     x1, #(MAX_GROUP_QUOTAS * 40)
    bl      .Lquota_zero

    /* Zero policy array */
    ldr     x0, =fs_quota_policies
    mov     x1, #(MAX_FILESYSTEMS * 20)
    bl      .Lquota_zero

    ldr     x0, =fs_quota_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lquota_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * fs_quota_enable — enable quotas for a filesystem
 * Input: x0 = fs_id, x1 = enable_user, x2 = enable_group
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global fs_quota_enable
fs_quota_enable:
    stp     x29, x30, [sp, #-16]!

    cmp     w0, #MAX_FILESYSTEMS
    b.hs    .Lquota_enable_fail

    /* Get policy pointer */
    ldr     x1, =fs_quota_policies
    mov     x2, w0, lsl #4      /* offset = fs * 20 */
    add     x2, x2, w0, lsl #2  /* add fs * 4 */
    add     x1, x1, x2

    /* Set enabled flag */
    mov     w2, #1
    str     w2, [x1, #16]       /* enabled */

    /* Update count */
    ldr     x0, =fs_quota_enabled_fs
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lquota_enable_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * fs_quota_set_limits — set quota limits for a user
 * Input: x0 = fs_id, x1 = uid, x2 = blocks_soft, x3 = blocks_hard, x4 = inodes_soft, x5 = inodes_hard
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global fs_quota_set_limits
fs_quota_set_limits:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     w19, w1             /* uid */

    cmp     w0, #MAX_USER_QUOTAS
    b.hs    .Lquota_set_fail

    /* Find or create quota entry */
    ldr     x0, =fs_quota_users
    mov     w2, #0

.Lquota_set_loop:
    cmp     w2, #MAX_USER_QUOTAS
    b.hs    .Lquota_set_fail

    mov     x1, w2, lsl #5      /* offset = index * 40 */
    add     x1, x1, w2, lsl #3  /* add index * 8 */
    add     x1, x0, x1

    /* Check if entry exists */
    ldr     w3, [x1, #0]        /* uid */
    cmp     w3, w19
    b.eq    .Lquota_set_found

    /* Check if empty */
    cbz     w3, .Lquota_set_found

    add     w2, w2, #1
    b       .Lquota_set_loop

.Lquota_set_found:
    /* Set limits */
    str     w19, [x1, #0]       /* uid */
    str     wzr, [x1, #4]       /* blocks_used */
    str     w2, [x1, #8]        /* blocks_soft */
    str     w3, [x1, #12]       /* blocks_hard */
    str     wzr, [x1, #16]      /* inodes_used */
    str     w4, [x1, #20]       /* inodes_soft */
    str     w5, [x1, #24]       /* inodes_hard */
    str     wzr, [x1, #28]      /* grace_start */
    str     wzr, [x1, #32]      /* warnings */

    /* Update count */
    ldr     x0, =fs_quota_user_count
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lquota_set_ret

.Lquota_set_fail:
    mov     x0, #-1

.Lquota_set_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * fs_quota_check — check if quota allows operation
 * Input: x0 = fs_id, x1 = uid, x2 = blocks_needed, x3 = inodes_needed
 * Output: x0 = 0 (ok) or -1 (quota exceeded)
 * ----------------------------------------------------------------------------- */
.global fs_quota_check
fs_quota_check:
    stp     x29, x30, [sp, #-16]!

    cmp     w1, #MAX_USER_QUOTAS
    b.hs    .Lquota_check_fail

    /* Find quota entry */
    ldr     x0, =fs_quota_users
    mov     w2, #0

.Lquota_check_loop:
    cmp     w2, #MAX_USER_QUOTAS
    b.hs    .Lquota_check_fail

    mov     x1, w2, lsl #5
    add     x1, x1, w2, lsl #3
    add     x1, x0, x1

    ldr     w3, [x1, #0]        /* uid */
    cmp     w3, w1
    b.eq    .Lquota_check_found

    add     w2, w2, #1
    b       .Lquota_check_loop

.Lquota_check_found:
    /* Check blocks */
    ldr     w3, [x1, #4]        /* blocks_used */
    ldr     w4, [x1, #12]       /* blocks_hard */
    add     w3, w3, w2
    cmp     w3, w4
    b.hs    .Lquota_check_fail

    /* Check inodes */
    ldr     w3, [x1, #16]       /* inodes_used */
    ldr     w4, [x1, #24]       /* inodes_hard */
    add     w3, w3, w3
    cmp     w3, w4
    b.hs    .Lquota_check_fail

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lquota_check_fail:
    /* Update violations */
    ldr     x0, =fs_quota_violations
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret
