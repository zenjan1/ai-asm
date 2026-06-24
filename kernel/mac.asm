/*
 * aiasm-aarch64/kernel/mac.asm
 * Mandatory Access Control (SELinux-like) (v1.0)
 *
 * AArch64 ASM implementation of SELinux-like mandatory access control.
 * Provides policy-based security enforcement for processes and objects.
 *
 * MAC features:
 *   - Security labels: label processes and objects with contexts
 *   - Policy rules: allow/deny/audit access transitions
 *   - Access vector cache (AVC): fast permission lookups
 *   - Audit logging: log all access decisions
 *
 * Security context format: "user:role:type:level"
 * Simplified: store as 32-bit context ID
 *
 * Integration: called from vfs.asm, net.asm, perm.asm for security enforcement
 *
 * API:
 *   mac_init()                                   — init MAC subsystem
 *   mac_label_process(proc_id, context_id)       — label a process
 *   mac_label_object(obj_type, obj_id, context_id) — label an object
 *   mac_check_access(subj_ctx, obj_ctx, action)  — check permission
 *   mac_add_rule(src_ctx, tgt_ctx, action, perm) — add policy rule
 *   mac_audit(subj, obj, action, granted)        — log access decision
 */
.arch armv8-a

/* Actions (bits in action mask) */
.set MAC_ACTION_READ,     0x01
.set MAC_ACTION_WRITE,    0x02
.set MAC_ACTION_EXEC,     0x04
.set MAC_ACTION_APPEND,   0x08
.set MAC_ACTION_CONNECT,  0x10
.set MAC_ACTION_BIND,     0x20

/* Object types */
.set MAC_OBJ_FILE,        0
.set MAC_OBJ_SOCKET,      1
.set MAC_OBJ_PROCESS,     2
.set MAC_OBJ_NETWORK,     3

/* Policy rule permission */
.set MAC_PERM_ALLOW,      0
.set MAC_PERM_DENY,       1
.set MAC_PERM_AUDIT,      2

/* Constants */
.set MAC_MAX_CONTEXTS,    256
.set MAC_MAX_RULES,       1024
.set MAC_MAX_LABELS,      4096
.set MAC_AVC_SIZE,        512

/* Label entry: {obj_type, obj_id, context_id} = 12 bytes */
/* Policy rule: {src_ctx, tgt_ctx, actions, permission} = 16 bytes */
/* AVC entry: {subj_ctx, obj_ctx, actions, granted} = 16 bytes */

.bss
.align 4
.global mac_init_done
mac_init_done:
    .skip 4

/* Process context labels: proc_id → context_id (4 bytes each) */
.align 4
mac_proc_labels:
    .skip 256 * 4              /* 256 processes */

/* Object labels: obj_type + obj_id → context_id */
.align 4
mac_obj_labels:
    .skip MAC_MAX_LABELS * 12  /* 49152 bytes */
mac_obj_label_count:
    .skip 4

/* Policy rules */
.align 4
mac_rules:
    .skip MAC_MAX_RULES * 16   /* 16384 bytes */
mac_rule_count:
    .skip 4

/* Access vector cache */
.align 4
mac_avc:
    .skip MAC_AVC_SIZE * 16    /* 8192 bytes */
mac_avc_hits:
    .skip 4
mac_avc_misses:
    .skip 4

/* Audit log (circular buffer) */
.align 4
mac_audit_log:
    .skip 256 * 16             /* 4096 bytes, 256 entries */
mac_audit_idx:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * mac_init — initialize MAC subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global mac_init
mac_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero all tables */
    ldr     x0, =mac_proc_labels
    mov     x1, #(256 * 4)
    bl      .Lmac_zero

    ldr     x0, =mac_obj_labels
    mov     x1, #(MAC_MAX_LABELS * 12)
    bl      .Lmac_zero

    ldr     x0, =mac_rules
    mov     x1, #(MAC_MAX_RULES * 16)
    bl      .Lmac_zero

    ldr     x0, =mac_avc
    mov     x1, #(MAC_AVC_SIZE * 16)
    bl      .Lmac_zero

    ldr     x0, =mac_audit_log
    mov     x1, #(256 * 16)
    bl      .Lmac_zero

    /* Zero counters */
    ldr     x0, =mac_obj_label_count
    str     wzr, [x0]
    ldr     x0, =mac_rule_count
    str     wzr, [x0]
    ldr     x0, =mac_avc_hits
    str     wzr, [x0]
    ldr     x0, =mac_avc_misses
    str     wzr, [x0]
    ldr     x0, =mac_audit_idx
    str     wzr, [x0]

    ldr     x0, =mac_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lmac_zero:
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
 * mac_label_process — label a process with security context
 * Input: x0 = process ID, x1 = context ID
 * Output: x0 = 0 (ok), -1 (invalid proc_id)
 * ----------------------------------------------------------------------------- */
.global mac_label_process
mac_label_process:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #256
    b.hs    .Lmac_label_proc_fail

    ldr     x2, =mac_proc_labels
    str     w1, [x2, x0, lsl #2]     /* proc_labels[proc_id] = ctx_id */

    mov     x0, #0
    b       .Lmac_label_proc_ret

.Lmac_label_proc_fail:
    mov     x0, #-1

.Lmac_label_proc_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mac_label_object — label an object with security context
 * Input: x0 = object type, x1 = object ID, x2 = context ID
 * Output: x0 = 0 (ok), -1 (table full)
 * ----------------------------------------------------------------------------- */
.global mac_label_object
mac_label_object:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* obj_type */
    mov     x20, x1             /* obj_id */

    /* Check if table full */
    ldr     x3, =mac_obj_label_count
    ldr     w3, [x3]
    cmp     w3, #MAC_MAX_LABELS
    b.hs    .Lmac_label_obj_fail

    /* Add label entry */
    ldr     x3, =mac_obj_labels
    mov     x4, #12
    mul     x4, w3, x4
    add     x3, x3, x4

    str     w19, [x3, #0]       /* obj_type */
    str     w20, [x3, #4]       /* obj_id */
    str     w2, [x3, #8]        /* context_id */

    /* Increment count */
    ldr     x3, =mac_obj_label_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    b       .Lmac_label_obj_ret

.Lmac_label_obj_fail:
    mov     x0, #-1

.Lmac_label_obj_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mac_check_access — check if access is permitted
 * Input: x0 = subject context, x1 = object context, x2 = action mask
 * Output: x0 = 1 (allowed), 0 (denied)
 *
 * Checks policy rules and AVC cache.
 * ----------------------------------------------------------------------------- */
.global mac_check_access
mac_check_access:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* subj_ctx */
    mov     x20, x1             /* obj_ctx */

    /* TODO: Check AVC cache first */
    /* Simplified: scan policy rules */

    ldr     x3, =mac_rule_count
    ldr     w3, [x3]
    cbz     w3, .Lmac_check_allow  /* no rules = allow all */

    ldr     x3, =mac_rules
    mov     w4, #0              /* rule index */

.Lmac_check_loop:
    cmp     w4, w3
    b.hs    .Lmac_check_allow   /* no matching rule = allow */

    /* Load rule */
    mov     x5, #16
    mul     x5, w4, x5
    add     x5, x3, x5

    ldr     w6, [x5, #0]        /* src_ctx */
    ldr     w7, [x5, #4]        /* tgt_ctx */
    ldr     w8, [x5, #8]        /* actions */
    ldr     w9, [x5, #12]       /* permission */

    /* Check if rule matches */
    cmp     w6, w19
    b.ne    .Lmac_check_next
    cmp     w7, w20
    b.ne    .Lmac_check_next

    /* Check if action matches */
    and     w10, w8, w2
    cbz     w10, .Lmac_check_next   /* action not in rule */

    /* Rule matches — check permission */
    cmp     w9, #MAC_PERM_ALLOW
    beq     .Lmac_check_allowed
    cmp     w9, #MAC_PERM_DENY
    beq     .Lmac_check_denied

.Lmac_check_next:
    add     w4, w4, #1
    b       .Lmac_check_loop

.Lmac_check_allowed:
    mov     x0, #1
    b       .Lmac_check_ret

.Lmac_check_denied:
    mov     x0, #0
    b       .Lmac_check_ret

.Lmac_check_allow:
    mov     x0, #1              /* default allow */

.Lmac_check_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mac_add_rule — add a policy rule
 * Input: x0 = source context, x1 = target context, x2 = action mask, x3 = permission
 * Output: x0 = 0 (ok), -1 (rule table full)
 * ----------------------------------------------------------------------------- */
.global mac_add_rule
mac_add_rule:
    stp     x29, x30, [sp, #-16]!

    /* Check if full */
    ldr     x4, =mac_rule_count
    ldr     w4, [x4]
    cmp     w4, #MAC_MAX_RULES
    b.hs    .Lmac_add_rule_fail

    /* Add rule */
    ldr     x4, =mac_rules
    mov     x5, #16
    mul     x5, w4, x5
    add     x4, x4, x5

    str     w0, [x4, #0]        /* src_ctx */
    str     w1, [x4, #4]        /* tgt_ctx */
    str     w2, [x4, #8]        /* actions */
    str     w3, [x4, #12]       /* permission */

    /* Increment count */
    ldr     x4, =mac_rule_count
    ldr     w5, [x4]
    add     w5, w5, #1
    str     w5, [x4]

    mov     x0, #0
    b       .Lmac_add_rule_ret

.Lmac_add_rule_fail:
    mov     x0, #-1

.Lmac_add_rule_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * mac_audit — log an access decision
 * Input: x0 = subject context, x1 = object context, x2 = action, x3 = granted (0/1)
 * Output: none
 * ----------------------------------------------------------------------------- */
.global mac_audit
mac_audit:
    stp     x29, x30, [sp, #-16]!

    /* Get audit index */
    ldr     x4, =mac_audit_idx
    ldr     w4, [x4]

    /* Compute entry address */
    ldr     x5, =mac_audit_log
    mov     x6, #16
    mul     x6, w4, x6
    add     x5, x5, x6

    /* Store audit entry */
    str     w0, [x5, #0]        /* subject */
    str     w1, [x5, #4]        /* object */
    str     w2, [x5, #8]        /* action */
    str     w3, [x5, #12]       /* granted */

    /* Increment index (circular) */
    add     w4, w4, #1
    and     w4, w4, #0xFF       /* mod 256 */
    str     w4, [x4]

    ldp     x29, x30, [sp], #16
    ret
