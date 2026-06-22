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
