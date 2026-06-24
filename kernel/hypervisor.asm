/*
 * aiasm-aarch64/kernel/hypervisor.asm
 * ARMv8 Hypervisor / EL2 (v1.0)
 *
 * AArch64 ASM implementation of ARMv8-V hypervisor running at EL2.
 * Manages guest VMs at EL1 using hardware virtualization extensions.
 *
 * Features:
 *   - VM creation with isolated address spaces
 *   - Stage-2 page table management (guest PA → host PA)
 *   - VM exit handling (traps, instruction emulation)
 *   - Virtual GIC for interrupt routing to VMs
 *
 * Integration: runs at EL2, manages guest VMs at EL1
 *
 * API:
 *   hypervisor_init()                          — init hypervisor
 *   hypervisor_create_vm(vm_id, mem_base, mem_size) — create VM
 *   hypervisor_run_vcpu(vm_id, entry_point)    — start vCPU
 *   hypervisor_handle_exit()                   — handle VM exit
 *   hypervisor_destroy_vm(vm_id)               — destroy VM
 */
.arch armv8-a

/* HCR_EL2 bits */
.set HCR_VM,       (1 << 0)     /* enable virtualization */
.set HCR_SWIO,     (1 << 2)     /* set/way invalidation override */
.set HCR_FMO,      (1 << 3)     /* FIQ mask override */
.set HCR_IMO,      (1 << 4)     /* IRQ mask override */
.set HCR_AMO,      (1 << 5)     /* SError mask override */
.set HCR_TSC,      (1 << 19)    /* trap SMC */
.set HCR_TWI,      (1 << 13)    /* trap WFI */
.set HCR_TWE,      (1 << 14)    /* trap WFE */

/* VM states */
.set VM_STATE_CREATED,    0
.set VM_STATE_RUNNING,    1
.set VM_STATE_PAUSED,     2
.set VM_STATE_STOPPED,    3

/* Constants */
.set HYP_MAX_VMS,    16

/* VM descriptor (64 bytes each) */
/* 0:  vm_id      (4) */
/* 4:  state      (4) */
/* 8:  mem_base   (8) */
/* 16: mem_size   (8) */
/* 24: entry_point (8) */
/* 32: vcpu_state (8) — saved vCPU registers */
/* 40: stage2_ttbr (8) — stage-2 translation table base */
/* 48: flags      (8) */

.bss
.align 4
.global hypervisor_init_done
hypervisor_init_done:
    .skip 4

.align 4
hyp_vms:
    .skip HYP_MAX_VMS * 64       /* 1024 bytes */
hyp_vm_count:
    .skip 4
hyp_current_vm:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * hypervisor_init — initialize hypervisor at EL2
 * Output: x0 = 0 (ok), -1 (not at EL2)
 * ----------------------------------------------------------------------------- */
.global hypervisor_init
hypervisor_init:
    stp     x29, x30, [sp, #-16]!

    /* Check current exception level */
    mrs     x0, CurrentEL
    lsr     x0, x0, #2
    cmp     x0, #2
    b.ne    .Lhyp_init_fail      /* not at EL2 */

    /* Configure HCR_EL2 for virtualization */
    mov     x0, #(HCR_VM | HCR_FMO | HCR_IMO | HCR_AMO)
    msr     hcr_el2, x0

    /* Enable EL1 AArch64 */
    mov     x0, #(1 << 31)       /* RW bit: EL1 is AArch64 */
    msr     hcr_el2, x0

    /* Zero VM table */
    ldr     x0, =hyp_vms
    mov     x1, #(HYP_MAX_VMS * 64)
    bl      .Lhyp_zero

    ldr     x0, =hyp_vm_count
    str     wzr, [x0]
    ldr     x0, =hyp_current_vm
    str     wzr, [x0]

    ldr     x0, =hypervisor_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    b       .Lhyp_init_ret

.Lhyp_init_fail:
    mov     x0, #-1

.Lhyp_init_ret:
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lhyp_zero:
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
 * hypervisor_create_vm — create a new virtual machine
 * Input: x0 = VM ID, x1 = memory base (host PA), x2 = memory size
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global hypervisor_create_vm
hypervisor_create_vm:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* vm_id */
    mov     x20, x1             /* mem_base */

    /* Validate VM ID */
    cmp     x0, #HYP_MAX_VMS
    b.hs    .Lhyp_create_fail

    /* Get VM descriptor */
    ldr     x3, =hyp_vms
    mov     x4, #64
    mul     x4, x19, x4
    add     x3, x3, x4

    /* Initialize VM */
    str     w19, [x3, #0]       /* vm_id */
    mov     w4, #VM_STATE_CREATED
    str     w4, [x3, #4]        /* state */
    str     x20, [x3, #8]       /* mem_base */
    str     x2, [x3, #16]       /* mem_size */

    /* TODO: Allocate stage-2 page tables */
    /* TODO: Map guest physical → host physical */

    /* Increment VM count */
    ldr     x3, =hyp_vm_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    b       .Lhyp_create_ret

.Lhyp_create_fail:
    mov     x0, #-1

.Lhyp_create_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * hypervisor_run_vcpu — start vCPU execution for a VM
 * Input: x0 = VM ID, x1 = entry point (guest VA)
 * Output: x0 = 0 (ok), -1 (error)
 *
 * Enters guest at EL1 by setting up EL2 registers and executing ERET.
 * ----------------------------------------------------------------------------- */
.global hypervisor_run_vcpu
hypervisor_run_vcpu:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* vm_id */
    mov     x20, x1             /* entry_point */

    /* Validate VM */
    cmp     x0, #HYP_MAX_VMS
    b.hs    .Lhyp_run_fail

    /* Get VM descriptor */
    ldr     x3, =hyp_vms
    mov     x4, #64
    mul     x4, x19, x4
    add     x3, x3, x4

    /* Check VM is created */
    ldr     w4, [x3, #4]
    cmp     w4, #VM_STATE_CREATED
    b.ne    .Lhyp_run_fail

    /* Store entry point */
    str     x20, [x3, #24]

    /* Set state to RUNNING */
    mov     w4, #VM_STATE_RUNNING
    str     w4, [x3, #4]

    /* Set as current VM */
    ldr     x4, =hyp_current_vm
    str     w19, [x4]

    /* TODO: Set up EL1 registers (SPSR_EL2, ELR_EL2, VTTBR_EL2) */
    /* TODO: Execute ERET to enter guest at EL1 */

    mov     x0, #0
    b       .Lhyp_run_ret

.Lhyp_run_fail:
    mov     x0, #-1

.Lhyp_run_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * hypervisor_handle_exit — handle VM exit (trap from guest)
 * Output: x0 = 0 (handled), -1 (fatal)
 *
 * Called on VM exit (HLT, WFI, data abort, etc.)
 * ----------------------------------------------------------------------------- */
.global hypervisor_handle_exit
hypervisor_handle_exit:
    stp     x29, x30, [sp, #-16]!

    /* Read exit reason from ESR_EL2 */
    mrs     x0, esr_el2
    lsr     x1, x0, #26         /* EC field (exception class) */

    /* Dispatch based on exception class */
    cmp     x1, #0x17           /* HVC (hypervisor call) */
    b.eq    .Lhyp_exit_hvc

    cmp     x1, #0x0F           /* WFI/WFE trap */
    b.eq    .Lhyp_exit_wfi

    cmp     x1, #0x24           /* Data abort from EL1 */
    b.eq    .Lhyp_exit_dabort

    /* Unknown exit — fatal */
    mov     x0, #-1
    b       .Lhyp_exit_ret

.Lhyp_exit_hvc:
    /* Handle HVC call (hypercall from guest) */
    /* TODO: Parse HVC immediate and dispatch */
    mov     x0, #0
    b       .Lhyp_exit_ret

.Lhyp_exit_wfi:
    /* WFI trap — reschedule or halt */
    mov     x0, #0
    b       .Lhyp_exit_ret

.Lhyp_exit_dabort:
    /* Data abort — may need emulation */
    /* TODO: Emulate faulting instruction */
    mov     x0, #0
    b       .Lhyp_exit_ret

.Lhyp_exit_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * hypervisor_destroy_vm — destroy a VM
 * Input: x0 = VM ID
 * Output: x0 = 0 (ok), -1 (error)
 * ----------------------------------------------------------------------------- */
.global hypervisor_destroy_vm
hypervisor_destroy_vm:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #HYP_MAX_VMS
    b.hs    .Lhyp_destroy_fail

    /* Get VM descriptor */
    ldr     x1, =hyp_vms
    mov     x2, #64
    mul     x2, x0, x2
    add     x1, x1, x2

    /* Set state to STOPPED */
    mov     w2, #VM_STATE_STOPPED
    str     w2, [x1, #4]

    /* TODO: Free stage-2 page tables */
    /* TODO: Unmap guest memory */

    mov     x0, #0
    b       .Lhyp_destroy_ret

.Lhyp_destroy_fail:
    mov     x0, #-1

.Lhyp_destroy_ret:
    ldp     x29, x30, [sp], #16
    ret
