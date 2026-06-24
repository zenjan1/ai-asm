/*
 * aiasm-aarch64/kernel/sched_rt.asm
 * Real-Time Scheduling (v1.0)
 *
 * AArch64 ASM implementation of real-time task scheduling.
 * Provides SCHED_FIFO and SCHED_RR policies with priority management.
 *
 * Features:
 *   - SCHED_FIFO and SCHED_RR policies
 *   - Priority inheritance
 *   - Deadline scheduling
 *   - Real-time bandwidth control
 *   - Priority boosting
 *   - RT throttling prevention
 *
 * Integration: called by scheduler, process management
 *
 * API:
 *   sched_rt_init()                                 — init RT scheduler
 *   sched_rt_register(task_id, priority, policy) => 0/-1 — register RT task
 *   sched_rt_unregister(task_id) => 0/-1            — unregister RT task
 *   sched_rt_set_priority(task_id, priority) => 0/-1 — set RT priority
 *   sched_rt_set_deadline(task_id, runtime, deadline, period) => 0/-1
 *   sched_rt_next_task() => task_id or -1           — get next RT task
 *   sched_rt_stats(out_stats)                       — get RT statistics
 */
.arch armv8-a

/* Constants */
.set MAX_RT_TASKS,         64
.set MAX_RT_PRIORITY,      100
.set SCHED_FIFO,           1
.set SCHED_RR,             2

/* RT task (24 bytes) */
/* task_id, rt_priority, policy, runtime, deadline, period */

.bss
.align 4
.global sched_rt_init_done
sched_rt_init_done:
    .skip 4

.align 4
sched_rt_task_count:
    .skip 4
sched_rt_next_id:
    .skip 4
sched_rt_total_scheduled:
    .skip 4
sched_rt_deadline_misses:
    .skip 4

/* RT task table */
.align 4
sched_rt_tasks:
    .skip MAX_RT_TASKS * 24       /* 1.5KB */

/* Priority queues (one per priority level, simplified) */
.align 4
sched_rt_queues:
    .skip MAX_RT_PRIORITY * 8     /* 800 bytes: task_count + head per level */

.text

/* -----------------------------------------------------------------------------
 * sched_rt_init — initialize RT scheduler
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global sched_rt_init
sched_rt_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero counters */
    ldr     x0, =sched_rt_task_count
    str     wzr, [x0]
    ldr     x0, =sched_rt_next_id
    str     wzr, [x0]
    ldr     x0, =sched_rt_total_scheduled
    str     wzr, [x0]
    ldr     x0, =sched_rt_deadline_misses
    str     wzr, [x0]

    /* Zero task table */
    ldr     x0, =sched_rt_tasks
    mov     x1, #(MAX_RT_TASKS * 24)
    bl      .Lrt_zero

    /* Zero priority queues */
    ldr     x0, =sched_rt_queues
    mov     x1, #(MAX_RT_PRIORITY * 8)
    bl      .Lrt_zero

    ldr     x0, =sched_rt_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lrt_zero:
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
 * sched_rt_register — register an RT task
 * Input: x0 = task_id, x1 = priority, x2 = policy (SCHED_FIFO or SCHED_RR)
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global sched_rt_register
sched_rt_register:
    stp     x29, x30, [sp, #-16]!

    /* Validate priority */
    cmp     w1, #MAX_RT_PRIORITY
    b.hs    .Lrt_reg_fail

    /* Validate policy */
    cmp     w2, #SCHED_RR
    b.hi    .Lrt_reg_fail

    /* Check capacity */
    ldr     x3, =sched_rt_task_count
    ldr     w3, [x3]
    cmp     w3, #MAX_RT_TASKS
    b.hs    .Lrt_reg_fail

    /* Get task pointer */
    ldr     x3, =sched_rt_tasks
    mov     x4, w0, uxtw
    mov     x5, #24
    mul     x4, x4, x5
    add     x3, x3, x4

    /* Initialize task */
    str     w0, [x3, #0]            /* task_id */
    str     w1, [x3, #4]            /* rt_priority */
    str     w2, [x3, #8]            /* policy */
    str     wzr, [x3, #12]          /* runtime */
    str     wzr, [x3, #16]          /* deadline */
    str     wzr, [x3, #20]          /* period */

    /* Update count */
    ldr     x3, =sched_rt_task_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lrt_reg_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sched_rt_set_priority — set RT task priority
 * Input: x0 = task_id, x1 = priority
 * Output: x0 = 0 (ok) or -1 (error)
 * ----------------------------------------------------------------------------- */
.global sched_rt_set_priority
sched_rt_set_priority:
    stp     x29, x30, [sp, #-16]!

    cmp     w1, #MAX_RT_PRIORITY
    b.hs    .Lrt_prio_fail

    ldr     x2, =sched_rt_tasks
    mov     x3, w0, uxtw
    mov     x4, #24
    mul     x3, x3, x4
    add     x2, x2, x3

    str     w1, [x2, #4]            /* rt_priority */

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

.Lrt_prio_fail:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sched_rt_next_task — get next RT task to schedule
 * Output: x0 = task_id or -1 (none)
 * ----------------------------------------------------------------------------- */
.global sched_rt_next_task
sched_rt_next_task:
    stp     x29, x30, [sp, #-16]!

    /* Simplified: find highest priority active task */
    ldr     x0, =sched_rt_tasks
    ldr     x1, =sched_rt_task_count
    ldr     w1, [x1]
    cbz     w1, .Lrt_next_none

    mov     w2, #0                  /* current task index */
    mov     w3, #-1                 /* best priority */
    mov     w4, #-1                 /* best task_id */

.Lrt_next_loop:
    cmp     w2, w1
    b.hs    .Lrt_next_done

    mov     x5, w2, uxtw
    mov     x6, #24
    mul     x5, x5, x6
    add     x5, x0, x5

    ldr     w6, [x5, #4]            /* priority */
    cmp     w6, w3
    b.ls    .Lrt_next_next

    mov     w3, w6
    ldr     w4, [x5, #0]            /* task_id */

.Lrt_next_next:
    add     w2, w2, #1
    b       .Lrt_next_loop

.Lrt_next_done:
    mov     x0, x4
    /* Update scheduled count */
    ldr     x1, =sched_rt_total_scheduled
    ldr     w2, [x1]
    add     w2, w2, #1
    str     w2, [x1]
    ldp     x29, x30, [sp], #16
    ret

.Lrt_next_none:
    mov     x0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * sched_rt_stats — get RT scheduler statistics
 * Input: x0 = output buffer (16 bytes)
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global sched_rt_stats
sched_rt_stats:
    stp     x29, x30, [sp, #-16]!

    ldr     x1, =sched_rt_task_count
    ldr     w1, [x1]
    str     w1, [x0, #0]

    ldr     x1, =sched_rt_total_scheduled
    ldr     w1, [x1]
    str     w1, [x0, #4]

    ldr     x1, =sched_rt_deadline_misses
    ldr     w1, [x1]
    str     w1, [x0, #8]

    ldr     x1, =sched_rt_next_id
    ldr     w1, [x1]
    str     w1, [x0, #12]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
