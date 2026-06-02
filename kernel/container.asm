/* container.asm: WASM container isolation (v12.0) */

.global container_create
.global container_start
.global container_stop
.global container_destroy
.global container_exec
.global container_get_status
.global container_list
.global container_init

/* Container states */
#define CONTAINER_CREATED     0
#define CONTAINER_RUNNING     1
#define CONTAINER_STOPPED     2
#define CONTAINER_PAUSED      3
#define CONTAINER_DESTROYED   4

/* Container limits */
#define MAX_CONTAINERS    32
#define CONTAINER_NAME_LEN  64
#define CONTAINER_IMAGE_LEN 128
#define CONTAINER_MEM_DEFAULT 8    /* 8MB */

/* Container control block */
#define CCB_SIZE          256    /* bytes per container */
#define CCB_STATE         0
#define CCB_NAME          4
#define CCB_IMAGE         8
#define CCB_MEM_LIMIT     12
#define CCB_CPU_QUOTA     16
#define CCB_PID           20
#define CCB_MEM_BASE      24
#define CCB_NET_ENABLED   28
#define CCB_IP_ADDR       32
#define CCB_LOG_OFFSET    48
#define CCB_LOG_SIZE      80
#define CCB_CREATED_TIME  132
#define CCB_START_COUNT   136

/* Base memory address for container space */
.set CONTAINER_MEM_START,  0x40000000
.set CONTAINER_STACK_SIZE, 0x100000    /* 1MB per container stack */

/* Per-container memory region: 8MB */
#define CONTAINER_MEM_SIZE (8 * 1024 * 1024)

.bss
container_initialized:
    .skip 4
container_count:
    .skip 4
container_table:
    .skip MAX_CONTAINERS * CCB_SIZE
container_log_buffer:
    .skip 256

.text

/* container_init() — initialize container subsystem
   Returns: 0=success, -1=failure */
container_init:
    stp  x29, x30, [sp, #-16]!

    /* Clear container table */
    adrp x0, container_table
    add  x0, x0, :lo12:container_table
    mov  x1, #(MAX_CONTAINERS * CCB_SIZE)
    mov  x2, xzr
1:  strb w2, [x0], #1
    subs x1, x1, #1
    cbnz x1, 1b

    /* Reset container count */
    adrp x0, container_count
    add  x0, x0, :lo12:container_count
    str  wzr, [x0]

    /* Mark initialized */
    adrp x0, container_initialized
    add  x0, x0, :lo12:container_initialized
    mov  w1, #1
    str  w1, [x0]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

/* container_create(name_ptr, image_ptr, mem_limit_mb)
   x0=name, x1=image, x2=memory limit in MB
   Returns: container_id in x0, or -1 */
container_create:
    stp  x29, x30, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!

    mov  x19, x0          /* save name ptr */
    mov  x20, x1          /* save image ptr */
    mov  x4, x2           /* save mem limit */

    /* Check initialized */
    adrp x0, container_initialized
    add  x0, x0, :lo12:container_initialized
    ldr  w1, [x0]
    cbz  w1, container_create_fail

    /* Check max containers */
    adrp x0, container_count
    add  x0, x0, :lo12:container_count
    ldr  w1, [x0]
    cmp  w1, #MAX_CONTAINERS
    b.ge container_create_fail

    /* Allocate container slot */
    mov  x2, w1           /* slot index */
    add  w1, w1, #1
    str  w1, [x0]

    /* Get container table entry */
    adrp x3, container_table
    add  x3, x3, :lo12:container_table
    mov  x5, #CCB_SIZE
    mul  x5, x2, x5
    add  x3, x3, x5       /* x3 = container entry pointer */

    /* Set state = CREATED */
    mov  w6, #CONTAINER_CREATED
    str  w6, [x3, #CCB_STATE]

    /* Copy name */
    mov  x4, x3
    add  x4, x4, #CCB_NAME
    mov  x5, x19
    mov  x6, #0
container_copy_name:
    ldrb w7, [x5, x6]
    strb w7, [x4, x6]
    cbz  w7, container_name_done
    add  x6, x6, #1
    cmp  x6, #CONTAINER_NAME_LEN
    b.lo container_copy_name
container_name_done:
    /* Null terminate */
    strb wzr, [x4, x6]

    /* Copy image */
    add  x4, x3, #CCB_IMAGE
    mov  x5, x20
    mov  x6, #0
container_copy_image:
    ldrb w7, [x5, x6]
    strb w7, [x4, x6]
    cbz  w7, container_image_done
    add  x6, x6, #1
    cmp  x6, #CONTAINER_IMAGE_LEN
    b.lo container_copy_image
container_image_done:
    strb wzr, [x4, x6]

    /* Set memory limit */
    adrp x4, container_count
    add  x4, x4, :lo12:container_count
    ldr  w4, [x4]
    subs w4, w4, #1       /* get back our slot index */
    mov  x5, x3
    cbz  x4, container_set_mem
    cmp  x2, #0
    b.eq container_set_mem
    str  w2, [x5, #CCB_MEM_LIMIT]
    b    container_set_cpu

container_set_mem:
    mov  w6, #CONTAINER_MEM_DEFAULT
    str  w6, [x5, #CCB_MEM_LIMIT]

container_set_cpu:
    mov  w6, #1000        /* 1000 CPU ticks quota */
    str  w6, [x5, #CCB_CPU_QUOTA]

    /* Set PID (slot + 1000) */
    mov  w6, w4
    add  w6, w6, #1000
    str  w6, [x5, #CCB_PID]

    /* Set memory base */
    mov  x6, #CONTAINER_MEM_START >> 20
    lsl  x6, x6, #20
    mov  x7, w4
    lsl  x7, x7, #23      /* 8MB per container */
    add  x6, x6, x7
    str  x6, [x5, #CCB_MEM_BASE]

    /* Network disabled by default */
    str  wzr, [x5, #CCB_NET_ENABLED]

    /* Clear log buffer */
    add  x6, x5, #CCB_LOG_OFFSET
    mov  x7, #(CCB_LOG_SIZE - 1)
    mov  x8, xzr
container_clear_log:
    strb w8, [x6, x7]
    subs x7, x7, #1
    b.pl container_clear_log

    /* Set start count = 0 */
    str  wzr, [x5, #CCB_START_COUNT]

    /* Created time */
    mov  w6, #0           /* placeholder for real timestamp */
    str  w6, [x5, #CCB_CREATED_TIME]

    /* Return container_id (slot index) */
    mov  x0, x4

    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

container_create_fail:
    mov  x0, #-1
    ldp  x19, x20, [sp], #16
    ldp  x29, x30, [sp], #16
    ret

/* container_start(container_id)
   x0=container_id
   Returns: 0=success, -1=failure */
container_start:
    stp  x29, x30, [sp, #-16]!

    /* Validate container_id */
    cmp  x0, #MAX_CONTAINERS
    b.ge container_start_fail

    /* Get container entry */
    adrp x1, container_table
    add  x1, x1, :lo12:container_table
    mov  x2, #CCB_SIZE
    mul  x2, x0, x2
    add  x1, x1, x2

    /* Check state is CREATED or STOPPED */
    ldr  w3, [x1, #CCB_STATE]
    cmp  w3, #CONTAINER_CREATED
    b.eq container_do_start
    cmp  w3, #CONTAINER_STOPPED
    b.eq container_do_start
    cmp  w3, #CONTAINER_PAUSED
    b.eq container_do_start
    b    container_start_fail

container_do_start:
    /* Set state = RUNNING */
    mov  w3, #CONTAINER_RUNNING
    str  w3, [x1, #CCB_STATE]

    /* Increment start count */
    ldr  w3, [x1, #CCB_START_COUNT]
    add  w3, w3, #1
    str  w3, [x1, #CCB_START_COUNT]

    /* Log start event */
    mov  w4, #1
    bl   container_log_event

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

container_start_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* container_stop(container_id)
   x0=container_id
   Returns: 0=success, -1=failure */
container_stop:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_CONTAINERS
    b.ge container_stop_fail

    adrp x1, container_table
    add  x1, x1, :lo12:container_table
    mov  x2, #CCB_SIZE
    mul  x2, x0, x2
    add  x1, x1, x2

    ldr  w3, [x1, #CCB_STATE]
    cmp  w3, #CONTAINER_RUNNING
    b.ne container_stop_fail

    /* Set state = STOPPED */
    mov  w3, #CONTAINER_STOPPED
    str  w3, [x1, #CCB_STATE]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

container_stop_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* container_destroy(container_id)
   x0=container_id
   Returns: 0=success, -1=failure */
container_destroy:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_CONTAINERS
    b.ge container_destroy_fail

    adrp x1, container_table
    add  x1, x1, :lo12:container_table
    mov  x2, #CCB_SIZE
    mul  x2, x0, x2
    add  x1, x1, x2

    /* Must be STOPPED or CREATED */
    ldr  w3, [x1, #CCB_STATE]
    cmp  w3, #CONTAINER_STOPPED
    b.eq container_do_destroy
    cmp  w3, #CONTAINER_CREATED
    b.eq container_do_destroy
    cmp  w3, #CONTAINER_PAUSED
    b.eq container_do_destroy
    b    container_destroy_fail

container_do_destroy:
    /* Set state = DESTROYED */
    mov  w3, #CONTAINER_DESTROYED
    str  w3, [x1, #CCB_STATE]

    /* Clear memory region */
    ldr  x3, [x1, #CCB_MEM_BASE]
    mov  x4, #CONTAINER_MEM_SIZE
    mov  x5, xzr
container_clear_mem:
    strb w5, [x3], #1
    subs x4, x4, #1
    cbnz x4, container_clear_mem

    /* Decrement container count if it was the last */
    /* Log destroy event */
    mov  w4, #2
    bl   container_log_event

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

container_destroy_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* container_exec(container_id, module_path_ptr)
   x0=container_id, x1=module_path
   Returns: 0=success, -1=failure */
container_exec:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_CONTAINERS
    b.ge container_exec_fail

    adrp x2, container_table
    add  x2, x2, :lo12:container_table
    mov  x3, #CCB_SIZE
    mul  x3, x0, x3
    add  x2, x2, x3

    ldr  w4, [x2, #CCB_STATE]
    cmp  w4, #CONTAINER_RUNNING
    b.ne container_exec_fail

    /* Set state = RUNNING (already running) */
    /* Log exec event */
    mov  w4, #3
    bl   container_log_event

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

container_exec_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* container_get_status(container_id, status_buf)
   x0=container_id, x1=status buffer (64 bytes)
   Returns: 0=success, -1=failure */
container_get_status:
    stp  x29, x30, [sp, #-16]!

    cmp  x0, #MAX_CONTAINERS
    b.ge container_status_fail

    adrp x2, container_table
    add  x2, x2, :lo12:container_table
    mov  x3, #CCB_SIZE
    mul  x3, x0, x3
    add  x2, x2, x3

    /* Write status to buffer */
    /* State (4 bytes) */
    ldr  w4, [x2, #CCB_STATE]
    str  w4, [x1, #0]

    /* PID (4 bytes) */
    ldr  w4, [x2, #CCB_PID]
    str  w4, [x1, #4]

    /* Memory limit (4 bytes) */
    ldr  w4, [x2, #CCB_MEM_LIMIT]
    str  w4, [x1, #8]

    /* Start count (4 bytes) */
    ldr  w4, [x2, #CCB_START_COUNT]
    str  w4, [x1, #12]

    /* Name (up to 48 bytes) */
    add  x4, x2, #CCB_NAME
    add  x5, x1, #16
    mov  x6, #0
container_copy_status_name:
    ldrb w7, [x4, x6]
    strb w7, [x5, x6]
    cbz  w7, container_status_done
    add  x6, x6, #1
    cmp  x6, #48
    b.lo container_copy_status_name
container_status_done:
    strb wzr, [x5, x6]

    mov  x0, #0
    ldp  x29, x30, [sp], #16
    ret

container_status_fail:
    mov  x0, #-1
    ldp  x29, x30, [sp], #16
    ret

/* container_list(buf_ptr, max_count)
   x0=buffer pointer, x1=max count
   Returns: number of containers listed */
container_list:
    stp  x29, x30, [sp, #-16]!

    adrp x2, container_table
    add  x2, x2, :lo12:container_table
    adrp x3, container_count
    add  x3, x3, :lo12:container_count
    ldr  w3, [x3]

    cmp  x3, x1
    b.lt container_list_no_cap
    mov  x3, x1
container_list_no_cap:
    mov  x4, xzr
    mov  x5, x0           /* output buffer */

container_list_loop:
    cmp  x4, x3
    b.ge container_list_done

    /* Get container entry */
    adrp x6, container_table
    add  x6, x6, :lo12:container_table
    mov  x7, #CCB_SIZE
    mul  x7, x4, x7
    add  x6, x6, x7

    /* Write entry: id(4) + state(4) + pid(4) + name(64) = 76 bytes */
    str  w4, [x5, #0]     /* container id */
    ldr  w7, [x6, #CCB_STATE]
    str  w7, [x5, #4]     /* state */
    ldr  w7, [x6, #CCB_PID]
    str  w7, [x5, #8]     /* pid */

    /* Copy name */
    add  x8, x6, #CCB_NAME
    add  x9, x5, #12
    mov  x10, #0
container_list_copy_name:
    ldrb w11, [x8, x10]
    strb w11, [x9, x10]
    cbz  w11, container_list_name_done
    add  x10, x10, #1
    cmp  x10, #64
    b.lo container_list_copy_name
container_list_name_done:
    strb wzr, [x9, x10]

    add  x5, x5, #76
    add  x4, x4, #1
    b    container_list_loop

container_list_done:
    mov  x0, x4
    ldp  x29, x30, [sp], #16
    ret

/* container_log_event(container_id, event_type) — internal
   x0=container_id (in), w4=event_type (input) */
container_log_event:
    stp  x29, x30, [sp, #-16]!

    adrp x5, container_table
    add  x5, x5, :lo12:container_table
    mov  x6, #CCB_SIZE
    mul  x6, x0, x6
    add  x5, x5, x6

    /* Write event to log offset */
    add  x5, x5, #CCB_LOG_OFFSET
    strb w4, [x5]         /* event type byte */

    ldp  x29, x30, [sp], #16
    ret
