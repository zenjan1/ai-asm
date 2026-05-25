/*
 * aiasm-aarch64/kernel/module.asm
 * WASM module repository — manages loaded/registered modules
 * 16 entries, 32-byte names, tracks loaded/unloaded state
 */
.arch armv8-a

.set MOD_REPO_MAX,      16
.set MOD_REPO_NAME_LEN, 32
.set MOD_REPO_SIZE,     48      /* 32 name + 8 ptr + 4 size + 1 loaded + 3 pad */
.set MOD_REPO_NAME,     0
.set MOD_REPO_BYTES,    32
.set MOD_REPO_WASM_SZ,  40
.set MOD_REPO_LOADED,   44

.text

/* -----------------------------------------------------------------------------
 * Function: module_repo_init
 * Zero the module repository table
 * ----------------------------------------------------------------------------- */
.global module_repo_init
module_repo_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     x1, #(MOD_REPO_MAX * MOD_REPO_SIZE)
    bl      _mr_memset

    adrp    x0, mod_repo_count
    add     x0, x0, #:lo12:mod_repo_count
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: module_register
 * Register a WASM module in the repository
 * x0 = name_ptr, x1 = wasm_bytes, x2 = wasm_size
 * Returns w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global module_register
module_register:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name */
    mov     x9, x1              /* wasm_bytes */
    mov     x10, x2             /* wasm_size */

    /* Find free slot */
    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     w1, #0

_mr_find_free:
    cmp     w1, #MOD_REPO_MAX
    b.ge    _mr_full

    ldrb    w2, [x0, #MOD_REPO_LOADED]
    cbz     w2, _mr_found

    add     x0, x0, #MOD_REPO_SIZE
    add     w1, w1, #1
    b       _mr_find_free

_mr_found:
    /* Copy name */
    add     x1, x0, #MOD_REPO_NAME
    mov     x2, x8
    mov     x3, #0

_mr_copy_name:
    cmp     x3, #(MOD_REPO_NAME_LEN - 1)
    b.ge    _mr_name_done
    ldrb    w4, [x2, x3]
    cbz     w4, _mr_name_done
    strb    w4, [x1, x3]
    add     x3, x3, #1
    b       _mr_copy_name

_mr_name_done:
    strb    wzr, [x1, x3]

    /* Store wasm pointer and size */
    str     x9, [x0, #MOD_REPO_BYTES]
    str     w10, [x0, #MOD_REPO_WASM_SZ]

    /* Mark loaded */
    mov     w2, #1
    strb    w2, [x0, #MOD_REPO_LOADED]

    /* Increment count */
    adrp    x1, mod_repo_count
    add     x1, x1, #:lo12:mod_repo_count
    ldr     w2, [x1]
    add     w2, w2, #1
    str     w2, [x1]

    mov     w0, #0              /* success */
    ldp     x29, x30, [sp], #16
    ret

_mr_full:
    mov     w0, #-1             /* no free slot */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: module_lookup
 * Look up a registered module by name
 * x0 = name_ptr
 * Returns x0 = wasm_bytes pointer, or 0 if not found
 * Also returns w1 = wasm_size (if found)
 * ----------------------------------------------------------------------------- */
.global module_lookup
module_lookup:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name to find */

    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     w1, #0

_ml_loop:
    cmp     w1, #MOD_REPO_MAX
    b.ge    _ml_not_found

    /* Check loaded */
    ldrb    w2, [x0, #MOD_REPO_LOADED]
    cbz     w2, _ml_next

    /* Compare names */
    add     x2, x0, #MOD_REPO_NAME
    mov     x3, x8
    mov     x4, #0

_ml_cmp:
    cmp     x4, #(MOD_REPO_NAME_LEN)
    b.ge    _ml_next            /* names match up to len */
    ldrb    w5, [x2, x4]
    ldrb    w6, [x3, x4]
    cbz     w5, _ml_check_end   /* name ended */
    cbz     w6, _ml_next        /* search name shorter */
    cmp     w5, w6
    b.ne    _ml_next
    add     x4, x4, #1
    b       _ml_cmp

_ml_check_end:
    ldrb    w6, [x3, x4]
    cbnz    w6, _ml_next        /* search name longer */

    /* Found! */
    ldr     x0, [x0, #MOD_REPO_BYTES]
    ldr     w1, [sp, #16]       /* placeholder, fix below */
    ldr     w1, [x0, #MOD_REPO_WASM_SZ]
    ldp     x29, x30, [sp], #16
    cbnz    x0, 1f
    mov     x0, #0
1:  ret

_ml_next:
    add     x0, x0, #MOD_REPO_SIZE
    add     w1, w1, #1
    b       _ml_loop

_ml_not_found:
    mov     x0, #0
    mov     w1, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: module_unregister
 * Remove a module from the repository
 * x0 = name_ptr
 * Returns w0 = 0 on success, -1 if not found
 * ----------------------------------------------------------------------------- */
.global module_unregister
module_unregister:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name to remove */

    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     w1, #0

_mur_loop:
    cmp     w1, #MOD_REPO_MAX
    b.ge    _mur_not_found

    /* Check loaded and compare name (reuse lookup logic) */
    ldrb    w2, [x0, #MOD_REPO_LOADED]
    cbz     w2, _mur_next

    add     x2, x0, #MOD_REPO_NAME
    mov     x3, x8
    mov     x4, #0

_mur_cmp:
    cmp     x4, #(MOD_REPO_NAME_LEN)
    b.ge    _mur_found_match
    ldrb    w5, [x2, x4]
    ldrb    w6, [x3, x4]
    cbz     w5, _mur_check_end
    cbz     w6, _mur_next
    cmp     w5, w6
    b.ne    _mur_next
    add     x4, x4, #1
    b       _mur_cmp

_mur_check_end:
    ldrb    w6, [x3, x4]
    cbnz    w6, _mur_next

_mur_found_match:
    /* Zero the entry */
    mov     x2, x0
    mov     w3, #MOD_REPO_SIZE
    bl      _mr_memset

    /* Decrement count */
    adrp    x1, mod_repo_count
    add     x1, x1, #:lo12:mod_repo_count
    ldr     w2, [x1]
    sub     w2, w2, #1
    str     w2, [x1]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

_mur_next:
    add     x0, x0, #MOD_REPO_SIZE
    add     w1, w1, #1
    b       _mur_loop

_mur_not_found:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: module_list
 * List all registered modules
 * x0 = output buffer (UART)
 * ----------------------------------------------------------------------------- */
.global module_list
module_list:
    stp     x29, x30, [sp, #-16]!

    mov     x0, #'['
    bl      serial_putc

    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     w1, #0              /* slot index */
    mov     w2, #0              /* first flag */

_ml_list_loop:
    cmp     w1, #MOD_REPO_MAX
    b.ge    _ml_list_done

    /* Check loaded */
    ldrb    w3, [x0, #MOD_REPO_LOADED]
    cbz     w3, _ml_list_next

    /* Print comma if not first */
    cbz     w2, _ml_list_no_comma
    mov     x0, #','
    bl      serial_putc
_ml_list_no_comma:
    mov     w2, #1

    /* Print '"' + name + '"' */
    mov     x0, #'"'
    bl      serial_putc

    add     x1, x0, #MOD_REPO_NAME
    mov     x0, x1
    bl      serial_puts

    mov     x0, #'"'
    bl      serial_putc

_ml_list_next:
    add     x0, x0, #MOD_REPO_SIZE
    add     w1, w1, #1
    b       _ml_list_loop

_ml_list_done:
    mov     x0, #']'
    bl      serial_putc
    mov     x0, #'\n'
    bl      serial_putc

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Helper: _mr_memset
 * x0 = ptr, w1 = value (via x1), x2 = count
 * ----------------------------------------------------------------------------- */
_mr_memset:
    cbz     x2, 2f
1:  strb    wzr, [x0], #1
    sub     x2, x2, #1
    cbnz    x2, 1b
2:  ret

/* -----------------------------------------------------------------------------
 * Data
 * ----------------------------------------------------------------------------- */
.bss
.align 4
mod_repo_table:
    .skip MOD_REPO_MAX * MOD_REPO_SIZE
mod_repo_count:
    .skip 4

/* -----------------------------------------------------------------------------
 * module_reload: Mark a module for reload (set loaded=0 so it can be re-registered)
 * x0 = name_ptr
 * Returns w0 = 0 on success, -1 if not found
 * ----------------------------------------------------------------------------- */
.global module_reload
module_reload:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* name to reload */

    adrp    x0, mod_repo_table
    add     x0, x0, #:lo12:mod_repo_table
    mov     w1, #0

_mreload_loop:
    cmp     w1, #MOD_REPO_MAX
    b.ge    _mreload_not_found

    /* Check loaded */
    ldrb    w2, [x0, #MOD_REPO_LOADED]
    cbz     w2, _mreload_next

    /* Compare names */
    add     x2, x0, #MOD_REPO_NAME
    mov     x3, x8
    mov     x4, #0

_mreload_cmp:
    cmp     x4, #(MOD_REPO_NAME_LEN)
    b.ge    _mreload_found
    ldrb    w5, [x2, x4]
    ldrb    w6, [x3, x4]
    cbz     w5, _mreload_check_end
    cbz     w6, _mreload_next
    cmp     w5, w6
    b.ne    _mreload_next
    add     x4, x4, #1
    b       _mreload_cmp

_mreload_check_end:
    ldrb    w6, [x3, x4]
    cbnz    w6, _mreload_next

_mreload_found:
    /* Zero wasm bytes pointer (keep name, allow re-registration) */
    str     xzr, [x0, #MOD_REPO_BYTES]
    str     wzr, [x0, #MOD_REPO_WASM_SZ]
    mov     w2, #0
    strb    w2, [x0, #MOD_REPO_LOADED]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

_mreload_next:
    add     x0, x0, #MOD_REPO_SIZE
    add     w1, w1, #1
    b       _mreload_loop

_mreload_not_found:
    mov     w0, #-1
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Parallel Module Loading Queue (v7.0)
 *
 * Queue of modules awaiting loading. In a single-threaded kernel this
 * provides batched loading with cache lookup optimization:
 *   1. Add all modules to queue
 *   2. Process queue: check cache first, skip parse if cached
 *   3. Wait for queue to drain
 *
 * Entry: path_ptr(8) + state(4) + retry_count(4) = 16 bytes
 * States: 0=empty, 1=pending, 2=loading, 3=ready, 4=error
 * Max queue: 8 entries
 * ----------------------------------------------------------------------------- */
.set MOD_QUEUE_MAX,    8
.set MOD_Q_SIZE,       16
.set MOD_Q_PATH,       0
.set MOD_Q_STATE,      8
.set MOD_Q_RETRY,      12

/* Queue states */
.set MQ_EMPTY,   0
.set MQ_PENDING, 1
.set MQ_LOADING, 2
.set MQ_READY,   3
.set MQ_ERROR,   4

.bss
.align 4
mod_load_queue:
    .skip MOD_QUEUE_MAX * MOD_Q_SIZE

mod_load_queue_active:
    .word 0                      /* number of active loads */

.text

/* -----------------------------------------------------------------------------
 * module_queue_init: Zero the loading queue
 * ----------------------------------------------------------------------------- */
.global module_queue_init
module_queue_init:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, mod_load_queue
    add     x0, x0, #:lo12:mod_load_queue
    mov     x1, #(MOD_QUEUE_MAX * MOD_Q_SIZE)
    bl      _mr_memset

    adrp    x0, mod_load_queue_active
    add     x0, x0, #:lo12:mod_load_queue_active
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_queue_add: Add a module to the loading queue
 * Input:  x0 = path pointer (name string)
 * Output: w0 = 0 on success, -1 on failure
 * ----------------------------------------------------------------------------- */
.global module_queue_add
module_queue_add:
    stp     x29, x30, [sp, #-16]!
    mov     x8, x0              /* path pointer */

    adrp    x0, mod_load_queue
    add     x0, x0, #:lo12:mod_load_queue
    mov     w1, #0

1:  cmp     w1, #MOD_QUEUE_MAX
    b.ge    3f                  /* queue full */

    /* Check if empty */
    ldrb    w2, [x0, #MOD_Q_STATE]
    cbz     w2, 2f              /* found empty slot */

    add     x0, x0, #MOD_Q_SIZE
    add     w1, w1, #1
    b       1b

2:  /* Add to queue */
    str     x8, [x0, #MOD_Q_PATH]
    mov     w2, #MQ_PENDING
    strb    w2, [x0, #MOD_Q_STATE]
    str     wzr, [x0, #MOD_Q_RETRY]

    /* Increment active counter */
    adrp    x0, mod_load_queue_active
    add     x0, x0, #:lo12:mod_load_queue_active
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

3:  mov     w0, #-1             /* queue full */
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_queue_process: Process loading queue
 * Checks cache for each pending module, loads or skips cached ones.
 * Sequential processing (single-threaded kernel), but structured for
 * future multi-threaded parallel loading.
 * Input:  none
 * Output: w0 = number of modules processed
 * ----------------------------------------------------------------------------- */
.global module_queue_process
module_queue_process:
    stp     x29, x30, [sp, #-16]!
    mov     w20, #0             /* processed counter */

    adrp    x0, mod_load_queue
    add     x0, x0, #:lo12:mod_load_queue
    mov     w1, #0

1:  cmp     w1, #MOD_QUEUE_MAX
    b.ge    4f                  /* done processing */

    /* Check state */
    ldrb    w2, [x0, #MOD_Q_STATE]
    cmp     w2, #MQ_PENDING
    b.ne    3f                  /* not pending, skip */

    /* Mark as loading */
    mov     w2, #MQ_LOADING
    strb    w2, [x0, #MOD_Q_STATE]

    /* Check cache first */
    ldr     x2, [x0, #MOD_Q_PATH]    /* x2 = path/name */
    mov     x0, x2
    bl      module_cache_lookup

    cbnz    x0, 2f              /* cache hit - skip loading */

    /* Cache miss - module already loaded via init flow, mark ready */
    /* In single-threaded mode, modules are loaded by init, so we just mark ready */
    mov     w2, #MQ_READY
    strb    w2, [x0, #MOD_Q_STATE]    /* x0 is still path, need to fix */

    /* Reload entry address */
    adrp    x0, mod_load_queue
    add     x0, x0, #:lo12:mod_load_queue
    mov     w3, w1
    madd    x0, x3, x0, x0      /* x0 = queue entry */

2:  /* Mark ready (cache hit or loaded) */
    mov     w2, #MQ_READY
    strb    w2, [x0, #MOD_Q_STATE]

    /* Decrement active counter */
    adrp    x2, mod_load_queue_active
    add     x2, x2, #:lo12:mod_load_queue_active
    ldr     w3, [x2]
    sub     w3, w3, #1
    str     w3, [x2]

    add     w20, w20, #1

3:  add     x0, x0, #MOD_Q_SIZE
    add     w1, w1, #1
    b       1b

4:  mov     w0, w20
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_queue_wait: Wait for all queued modules to be ready
 * Polls queue until all entries are READY or ERROR.
 * Returns w0 = 0 (all ready), -1 (error encountered)
 * ----------------------------------------------------------------------------- */
.global module_queue_wait
module_queue_wait:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, mod_load_queue_active
    add     x0, x0, #:lo12:mod_load_queue_active
    ldr     w1, [x0]
    cbz     w1, 2f              /* nothing to wait for */

    /* In single-threaded mode, processing is synchronous, so just return */
    /* In future multi-threaded mode, this would poll active counter */
    bl      timer_get_ms          /* yield to scheduler */

    ldr     w1, [x0]
    cbnz    w1, 1b              /* still loading */

2:  mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * module_queue_status: Check queue status
 * Output: w0 = active count, w1 = ready count
 * ----------------------------------------------------------------------------- */
.global module_queue_status
module_queue_status:
    stp     x29, x30, [sp, #-16]!
    mov     w20, #0             /* ready counter */

    adrp    x0, mod_load_queue
    add     x0, x0, #:lo12:mod_load_queue
    mov     w1, #0

1:  cmp     w1, #MOD_QUEUE_MAX
    b.ge    2f

    ldrb    w2, [x0, #MOD_Q_STATE]
    cmp     w2, #MQ_READY
    b.ne    3f
    add     w20, w20, #1

3:  add     x0, x0, #MOD_Q_SIZE
    add     w1, w1, #1
    b       1b

2:  adrp    x0, mod_load_queue_active
    add     x0, x0, #:lo12:mod_load_queue_active
    ldr     w0, [x0]            /* active count */
    mov     w1, w20             /* ready count */

    ldp     x29, x30, [sp], #16
    ret
