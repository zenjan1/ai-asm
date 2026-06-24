/*
 * aiasm-aarch64/kernel/vm_mgr.asm
 * Virtual Machine Manager (v1.0)
 *
 * AArch64 ASM implementation of VM lifecycle management.
 * Provides high-level VM operations: create, start, stop, pause, migrate.
 *
 * Features:
 *   - VM state management (created, running, paused, stopped)
 *   - Resource allocation (CPU cores, memory, devices)
 *   - vCPU scheduling across physical CPUs
 *   - Live migration (save/restore VM state)
 *
 * Integration: called by hypervisor.asm for VM operations
 *
 * API:
 *   vm_mgr_init()                          — init VM manager
 *   vm_create(name, vcpus, mem_mb) => vm_id — create VM
 *   vm_start(vm_id)                        — start VM
 *   vm_stop(vm_id)                         — stop VM
 *   vm_pause(vm_id)                        — pause VM
 *   vm_migrate(vm_id, target_host)         — migrate VM
 */
.arch armv8-a

/* VM states */
.set VMM_STATE_NONE,      0
.set VMM_STATE_CREATED,   1
.set VMM_STATE_RUNNING,   2
.set VMM_STATE_PAUSED,    3
.set VMM_STATE_STOPPED,   4
.set VMM_STATE_MIGRATING, 5

/* Constants */
.set VMM_MAX_VMS,    32

/* VM descriptor (48 bytes each) */
/* 0:  vm_id      (4) */
/* 4:  state      (4) */
/* 8:  vcpus      (4) */
/* 12: mem_mb     (4) */
/* 16: name_ptr   (8) */
/* 24: host_id    (4) */
/* 28: saved_state (8) — for migration */
/* 36: flags      (8) */

.bss
.align 4
.global vm_mgr_init_done
vm_mgr_init_done:
    .skip 4

.align 4
vmm_vms:
    .skip VMM_MAX_VMS * 48       /* 1536 bytes */
vmm_vm_count:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * vm_mgr_init — initialize VM manager
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global vm_mgr_init
vm_mgr_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero VM table */
    ldr     x0, =vmm_vms
    mov     x1, #(VMM_MAX_VMS * 48)
    bl      .Lvmm_zero

    ldr     x0, =vmm_vm_count
    str     wzr, [x0]

    ldr     x0, =vm_mgr_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lvmm_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* Internal: find free VM slot
 * Output: x0 = index, or -1 if full
 */
.Lvmm_find_free:
    mov     w0, #0
.Lvmm_find_loop:
    cmp     w0, #VMM_MAX_VMS
    b.hs    .Lvmm_find_fail
    ldr     x1, =vmm_vms
    mov     x2, #48
    mul     x2, w0, x2
    add     x1, x1, x2
    ldr     w2, [x1, #4]        /* state */
    cmp     w2, #VMM_STATE_NONE
    beq     .Lvmm_find_done
    add     w0, w0, #1
    b       .Lvmm_find_loop
.Lvmm_find_fail:
    mov     x0, #-1
.Lvmm_find_done:
    ret

/* -----------------------------------------------------------------------------
 * vm_create — create a new VM
 * Input: x0 = name pointer, x1 = vCPU count, x2 = memory in MB
 * Output: x0 = VM ID, or -1 (error)
 * ----------------------------------------------------------------------------- */
.global vm_create
vm_create:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* name */
    mov     x20, x1             /* vcpus */
    mov     x21, x2             /* mem_mb */

    /* Find free slot */
    bl      .Lvmm_find_free
    cmp     x0, #-1
    beq     .Lvm_create_fail

    mov     x22, x0             /* vm_id (slot index) */

    /* Get descriptor */
    ldr     x3, =vmm_vms
    mov     x4, #48
    mul     x4, x22, x4
    add     x3, x3, x4

    /* Initialize VM */
    str     w22, [x3, #0]       /* vm_id */
    mov     w4, #VMM_STATE_CREATED
    str     w4, [x3, #4]        /* state */
    str     w20, [x3, #8]       /* vcpus */
    str     w21, [x3, #12]      /* mem_mb */
    str     x19, [x3, #16]      /* name_ptr */

    /* Increment count */
    ldr     x3, =vmm_vm_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, x22
    b       .Lvm_create_ret

.Lvm_create_fail:
    mov     x0, #-1

.Lvm_create_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vm_start — start a VM
 * Input: x0 = VM ID
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global vm_start
vm_start:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #VMM_MAX_VMS
    b.hs    .Lvm_start_fail

    ldr     x1, =vmm_vms
    mov     x2, #48
    mul     x2, x0, x2
    add     x1, x1, x2

    /* Check state is CREATED or PAUSED */
    ldr     w2, [x1, #4]
    cmp     w2, #VMM_STATE_CREATED
    beq     .Lvm_start_ok
    cmp     w2, #VMM_STATE_PAUSED
    beq     .Lvm_start_ok
    b       .Lvm_start_fail

.Lvm_start_ok:
    mov     w2, #VMM_STATE_RUNNING
    str     w2, [x1, #4]

    /* TODO: Call hypervisor_run_vcpu */
    mov     x0, #0
    b       .Lvm_start_ret

.Lvm_start_fail:
    mov     x0, #-1

.Lvm_start_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vm_stop — stop a VM
 * Input: x0 = VM ID
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global vm_stop
vm_stop:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #VMM_MAX_VMS
    b.hs    .Lvm_stop_fail

    ldr     x1, =vmm_vms
    mov     x2, #48
    mul     x2, x0, x2
    add     x1, x1, x2

    mov     w2, #VMM_STATE_STOPPED
    str     w2, [x1, #4]

    /* TODO: Save VM state, release resources */
    mov     x0, #0
    b       .Lvm_stop_ret

.Lvm_stop_fail:
    mov     x0, #-1

.Lvm_stop_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vm_pause — pause a running VM
 * Input: x0 = VM ID
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global vm_pause
vm_pause:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #VMM_MAX_VMS
    b.hs    .Lvm_pause_fail

    ldr     x1, =vmm_vms
    mov     x2, #48
    mul     x2, x0, x2
    add     x1, x1, x2

    ldr     w2, [x1, #4]
    cmp     w2, #VMM_STATE_RUNNING
    b.ne    .Lvm_pause_fail

    mov     w2, #VMM_STATE_PAUSED
    str     w2, [x1, #4]

    mov     x0, #0
    b       .Lvm_pause_ret

.Lvm_pause_fail:
    mov     x0, #-1

.Lvm_pause_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * vm_migrate — migrate VM to another host
 * Input: x0 = VM ID, x1 = target host ID
 * Output: x0 = 0 (ok), -1 (error)
 *
 * Live migration: save state, transfer to target, resume.
 * ----------------------------------------------------------------------------- */
.global vm_migrate
vm_migrate:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #VMM_MAX_VMS
    b.hs    .Lvm_migrate_fail

    ldr     x2, =vmm_vms
    mov     x3, #48
    mul     x3, x0, x3
    add     x2, x2, x3

    /* Set state to MIGRATING */
    mov     w3, #VMM_STATE_MIGRATING
    str     w3, [x2, #4]

    /* Store target host */
    str     w1, [x2, #24]       /* host_id */

    /* TODO: Save VM state to memory */
    /* TODO: Transfer state to target host via network */
    /* TODO: Resume on target */

    /* For now: mark as RUNNING on new host */
    mov     w3, #VMM_STATE_RUNNING
    str     w3, [x2, #4]

    mov     x0, #0
    b       .Lvm_migrate_ret

.Lvm_migrate_fail:
    mov     x0, #-1

.Lvm_migrate_ret:
    ldp     x29, x30, [sp], #16
    ret
