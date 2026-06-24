/*
 * aiasm-aarch64/kernel/sandbox.asm
 * WASM Sandbox & Syscall Filtering (v1.0)
 *
 * AArch64 ASM implementation of WASM process sandboxing.
 * Provides seccomp-like syscall filtering, memory isolation,
 * resource limits, and capability-based access control for WASM processes.
 *
 * Security features:
 *   - Syscall whitelist: only allowed WASI syscalls permitted
 *   - Memory bounds: enforce linear memory limits
 *   - Resource caps: CPU time, memory, file descriptors
 *   - Capabilities: fine-grained permissions (files, network, devices)
 *
 * Integration: called from process.asm when spawning WASM processes
 *
 * API:
 *   sandbox_init()                               — init sandbox subsystem
 *   sandbox_create(proc_id) => sandbox_id        — create sandbox for process
 *   sandbox_set_filter(sbox_id, syscall_mask)    — set allowed syscalls
 *   sandbox_check_syscall(sbox_id, syscall_num)  — check if syscall allowed
 *   sandbox_set_limits(sbox_id, max_mem, max_fd) — set resource limits
 *   sandbox_enforce(sbox_id)                     — activate enforcement
 */
.arch armv8-a

/* Constants */
.set SANDBOX_MAX,         64      /* max sandboxes */
.set SANDBOX_MAX_SYSCALLS, 64    /* syscall bitmask size (bits) */
.set SANDBOX_CAP_FILE,    0x01   /* capability: file access */
.set SANDBOX_CAP_NET,     0x02   /* capability: network */
.set SANDBOX_CAP_EXEC,    0x04   /* capability: execute */
.set SANDBOX_CAP_DEV,     0x08   /* capability: device access */

/* Sandbox descriptor (64 bytes each) */
/* 0:  proc_id     (4) */
/* 4:  active      (4)  — 1 if enforcement active */
/* 8:  syscall_mask (8) — bitmask of allowed syscalls (64 bits) */
/* 16: max_memory  (8)  — max linear memory bytes */
/* 24: max_fd      (4)  — max open file descriptors */
/* 28: max_cpu_ms  (4)  — max CPU time in milliseconds */
/* 32: cur_memory  (8)  — current memory usage */
/* 40: cur_fd      (4)  — current open fd count */
/* 44: cur_cpu_ms  (4)  — current CPU time used */
/* 48: capabilities (4) — capability bitmask */
/* 52: flags       (4) */
/* 56: reserved    (8) */

.bss
.align 4
.global sandbox_init_done
sandbox_init_done:
    .skip 4

.align 4
sandbox_descs:
    .skip SANDBOX_MAX * 64       /* 4096 bytes */

.text

/* -----------------------------------------------------------------------------
 * sandbox_init — initialize sandbox subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global sandbox_init
sandbox_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero sandbox table */
    ldr     x0, =sandbox_descs
    mov     x1, #(SANDBOX_MAX * 64)
1:  cbz     x1, 2f
    strb    wzr, [x0], #1
    sub     x1, x1, #1
    b       1b
2:
    ldr     x0, =sandbox_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: find free sandbox slot
 * Output: x0 = index, or -1 if full
 */
.Lsbox_findbox:
    mov     w0, #0
.Lsbox_loop:
    cmp     w0, #SANDBOX_MAX
    b.hs    .Lsbox_full
    ldr     x1, =sandbox_descs
    mov     x2, #64
    mul     x2, w0, x2
    add     x1, x1, x2
    ldr     w2, [x1]            /* proc_id */
    cmp     w2, #0
    beq     .Lsbox_found
    add     w0, w0, #1
    b       .Lsbox_loop
.Lsbox_full:
    mov     x0, #-1
.Lsbox_found:
    ret

/* -----------------------------------------------------------------------------
 * sandbox_create — create a sandbox for a process
 * Input: x0 = process ID
 * Output: x0 = sandbox ID, or -1 (error)
 * ----------------------------------------------------------------------------- */
.global sandbox_create
sandbox_create:
    stp     x29, x30, [sp, #-16]!

    mov     x3, x0              /* proc_id */

    /* Find free slot */
    bl      .Lsbox_find_box
    cmp     x0, #-1
    beq     .Lsbox_create_fail

    /* Initialize descriptor */
    ldr     x1, =sandbox_descs
    mov     x2, #64
    mul     x2, x0, x2
    add     x1, x1, x2

    str     w3, [x1, #0]        /* proc_id */
    str     wzr, [x1, #4]       /* active = 0 */
    str     xzr, [x1, #8]       /* syscall_mask = 0 (deny all) */

    /* Set default limits */
    mov     x4, #(64 * 1024 * 1024)  /* 64MB max memory */
    str     x4, [x1, #16]       /* max_memory */
    mov     w4, #16
    str     w4, [x1, #24]       /* max_fd = 16 */
    mov     w4, #5000
    str     w4, [x1, #28]       /* max_cpu_ms = 5000 */

    /* Zero current usage */
    str     xzr, [x1, #32]      /* cur_memory */
    str     wzr, [x1, #40]      /* cur_fd */
    str     wzr, [x1, #44]      /* cur_cpu_ms */

    /* Set default capabilities (file + net) */
    mov     w4, #(SANDBOX_CAP_FILE | SANDBOX_CAP_NET)
    str     w4, [x1, #48]       /* capabilities */

    b       .Lsbox_create_ret

.Lsbox_create_fail:
    mov     x0, #-1

.Lsbox_create_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sandbox_set_filter — set allowed syscall bitmask
 * Input: x0 = sandbox ID, x1 = syscall mask (64-bit bitmask)
 * Output: x0 = 0 (ok), -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global sandbox_set_filter
sandbox_set_filter:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #SANDBOX_MAX
    b.hs    .Lsbox_filter_fail

    ldr     x2, =sandbox_descs
    mov     x3, #64
    mul     x3, x0, x3
    add     x2, x2, x3

    str     x1, [x2, #8]        /* syscall_mask */

    mov     x0, #0
    b       .Lsbox_filter_ret

.Lsbox_filter_fail:
    mov     x0, #-1

.Lsbox_filter_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sandbox_check_syscall — check if a syscall is allowed
 * Input: x0 = sandbox ID, x1 = syscall number
 * Output: x0 = 1 (allowed), 0 (denied)
 * ----------------------------------------------------------------------------- */
.global sandbox_check_syscall
sandbox_check_syscall:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #SANDBOX_MAX
    b.hs    .Lsbox_check_deny

    /* Get descriptor */
    ldr     x2, =sandbox_descs
    mov     x3, #64
    mul     x3, x0, x3
    add     x2, x2, x3

    /* Check if active */
    ldr     w4, [x2, #4]
    cbz     w4, .Lsbox_check_allow  /* not active = allow all */

    /* Check syscall number < 64 */
    cmp     x1, #64
    b.hs    .Lsbox_check_deny

    /* Check bit in mask */
    ldr     x4, [x2, #8]        /* syscall_mask */
    mov     x5, #1
    lsl     x5, x5, x1          /* bit = 1 << syscall_num */
    and     x4, x4, x5
    cbnz    x4, .Lsbox_check_allow

.Lsbox_check_deny:
    mov     x0, #0
    b       .Lsbox_check_ret

.Lsbox_check_allow:
    mov     x0, #1

.Lsbox_check_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sandbox_set_limits — set resource limits for sandbox
 * Input: x0 = sandbox ID, x1 = max memory (bytes), x2 = max fd count
 * Output: x0 = 0 (ok), -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global sandbox_set_limits
sandbox_set_limits:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #SANDBOX_MAX
    b.hs    .Lsbox_limits_fail

    ldr     x3, =sandbox_descs
    mov     x4, #64
    mul     x4, x0, x4
    add     x3, x3, x4

    str     x1, [x3, #16]       /* max_memory */
    str     w2, [x3, #24]       /* max_fd */

    mov     x0, #0
    b       .Lsbox_limits_ret

.Lsbox_limits_fail:
    mov     x0, #-1

.Lsbox_limits_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sandbox_enforce — activate enforcement for sandbox
 * Input: x0 = sandbox ID
 * Output: x0 = 0 (ok), -1 (invalid)
 * ----------------------------------------------------------------------------- */
.global sandbox_enforce
sandbox_enforce:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #SANDBOX_MAX
    b.hs    .Lsbox_enforce_fail

    ldr     x1, =sandbox_descs
    mov     x2, #64
    mul     x2, x0, x2
    add     x1, x1, x2

    mov     w2, #1
    str     w2, [x1, #4]        /* active = 1 */

    mov     x0, #0
    b       .Lsbox_enforce_ret

.Lsbox_enforce_fail:
    mov     x0, #-1

.Lsbox_enforce_ret:
    ldp     x29, x30, [sp], #16
    ret
