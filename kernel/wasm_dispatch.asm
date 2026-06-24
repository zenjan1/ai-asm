/*
 * aiasm-aarch64/kernel/wasm_dispatch.asm
 * WASM Function Dispatch & Trampolines (v1.0)
 *
 * AArch64 ASM implementation of host-to-WASM function dispatch.
 * Generates trampolines for WASM function imports, marshals arguments
 * between C calling convention and WASM stack layout, handles return
 * values, and supports indirect calls via function tables.
 *
 * WASM calling convention:
 *   - Arguments pushed onto WASM value stack (i32/i64/f32/f64/v128)
 *   - Return values popped from value stack
 *   - WASM functions use structured control flow (blocks, loops)
 *
 * AArch64 calling convention (caller-saved):
 *   x0-x7: arguments/return, x8: indirect result location
 *   x9-x15: caller-saved temporaries
 *   x19-x28: callee-saved
 *   x29: frame pointer, x30: link register
 *
 * Integration: called from wasm_embed.asm for host function imports
 *
 * API:
 *   wasm_dispatch_init()                              — init dispatch subsystem
 *   wasm_dispatch_register(func_idx, host_fn)         — register host function
 *   wasm_dispatch_call(func_idx, args_ptr, nargs)     — call WASM function
 *   wasm_dispatch_trampoline(host_fn, wasm_sp)        — WASM→host trampoline
 *   wasm_dispatch_indirect(table_idx, sig_idx, args)  — call_indirect
 */
.arch armv8-a

/* Constants */
.set WASM_DISP_MAX_HOST_FNS, 256    /* max registered host functions */
.set WASM_DISP_MAX_TABLES,   8      /* max function tables */
.set WASM_DISP_MAX_TABLE_SZ, 65536  /* max entries per table */

/* WASM value types */
.set WASM_TYPE_I32,    0x7F
.set WASM_TYPE_I64,    0x7E
.set WASM_TYPE_F32,    0x7D
.set WASM_TYPE_F64,    0x7C
.set WASM_TYPE_V128,   0x7B
.set WASM_TYPE_FUNCREF, 0x70
.set WASM_TYPE_EXTERNREF, 0x6F

.bss
.align 4
.global wasm_dispatch_init_done
wasm_dispatch_init_done:
    .skip 4

/* Host function table: array of function pointers (8 bytes each) */
.align 4
wasm_dispatch_host_fns:
    .skip WASM_DISP_MAX_HOST_FNS * 8   /* 2048 bytes */

/* Function type signatures: array of {n_params, n_results, param_types_ptr, result_types_ptr} */
/* Each entry: 24 bytes */
.align 4
wasm_dispatch_sigs:
    .skip WASM_DISP_MAX_HOST_FNS * 24  /* 6144 bytes */

/* Function table for call_indirect: array of {base_ptr, size} per table */
.align 4
wasm_dispatch_func_tables:
    .skip WASM_DISP_MAX_TABLES * 16    /* 128 bytes */

/* Active WASM execution context */
.align 4
wasm_dispatch_wasm_sp:
    .skip 8          /* WASM value stack pointer */
wasm_dispatch_call_depth:
    .skip 4          /* current call depth (for stack overflow detection) */
wasm_dispatch_max_depth:
    .skip 4          /* max call depth (default: 1024) */

.text

/* -----------------------------------------------------------------------------
 * wasm_dispatch_init — initialize dispatch subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global wasm_dispatch_init
wasm_dispatch_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero host function table */
    ldr     x0, =wasm_dispatch_host_fns
    mov     x1, #(WASM_DISP_MAX_HOST_FNS * 8)
    bl      .Ldisp_zero

    /* Zero signature table */
    ldr     x0, =wasm_dispatch_sigs
    mov     x1, #(WASM_DISP_MAX_HOST_FNS * 24)
    bl      .Ldisp_zero

    /* Zero function tables */
    ldr     x0, =wasm_dispatch_func_tables
    mov     x1, #(WASM_DISP_MAX_TABLES * 16)
    bl      .Ldisp_zero

    /* Set default max call depth */
    ldr     x0, =wasm_dispatch_max_depth
    mov     w1, #1024
    str     w1, [x0]

    /* Reset call depth */
    ldr     x0, =wasm_dispatch_call_depth
    str     wzr, [x0]

    ldr     x0, =wasm_dispatch_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Ldisp_zero:
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
 * wasm_dispatch_register — register a host function for WASM import
 * Input: x0 = function index, x1 = host function pointer
 *        x2 = number of params, x3 = number of results
 * Output: x0 = 0 (ok), -1 (invalid index)
 * ----------------------------------------------------------------------------- */
.global wasm_dispatch_register
wasm_dispatch_register:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    cmp     x0, #WASM_DISP_MAX_HOST_FNS
    b.hs    .Ldisp_reg_fail

    mov     x19, x0             /* func index */
    mov     x20, x1             /* host fn ptr */

    /* Store function pointer */
    ldr     x3, =wasm_dispatch_host_fns
    mov     x4, #8
    mul     x4, x19, x4
    add     x3, x3, x4
    str     x20, [x3]

    /* Store signature info */
    ldr     x3, =wasm_dispatch_sigs
    mov     x4, #24
    mul     x4, x19, x4
    add     x3, x3, x4
    str     w2, [x3, #0]        /* n_params */
    str     w3, [x3, #4]        /* n_results */
    /* param_types_ptr and result_types_ptr at offset 8 and 16 — set later */

    mov     x0, #0
    b       .Ldisp_reg_ret

.Ldisp_reg_fail:
    mov     x0, #-1

.Ldisp_reg_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_dispatch_call — call a WASM function by index
 * Input: x0 = function index, x1 = args pointer (array of 8-byte values),
 *        x2 = number of args
 * Output: x0 = return value (i32/i64), or -1 (error)
 *
 * Sets up WASM→host→WASM transition:
 * 1. Increment call depth (check for stack overflow)
 * 2. Look up function pointer
 * 3. Marshal arguments from WASM stack to AArch64 registers
 * 4. Call the function
 * 5. Extract return value
 * 6. Decrement call depth
 * ----------------------------------------------------------------------------- */
.global wasm_dispatch_call
wasm_dispatch_call:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* func index */
    mov     x20, x1             /* args ptr */
    mov     x21, x2             /* nargs */

    /* Check call depth */
    ldr     x3, =wasm_dispatch_call_depth
    ldr     w4, [x3]
    ldr     x5, =wasm_dispatch_max_depth
    ldr     w5, [x5]
    cmp     w4, w5
    b.hs    .Ldisp_call_overflow

    /* Increment call depth */
    add     w4, w4, #1
    str     w4, [x3]

    /* Look up function pointer */
    cmp     x19, #WASM_DISP_MAX_HOST_FNS
    b.hs    .Ldisp_call_badidx

    ldr     x3, =wasm_dispatch_host_fns
    mov     x4, #8
    mul     x4, x19, x4
    add     x3, x3, x4
    ldr     x4, [x3]            /* function pointer */
    cbz     x4, .Ldisp_call_badidx

    /* Marshal arguments: load up to 8 args into x0-x7 */
    /* Arg 0 */
    cmp     x21, #0
    beq     .Ldisp_call_noregs
    ldr     x0, [x20, #0]
    cmp     x21, #1
    beq     .Ldisp_call_1reg
    ldr     x1, [x20, #8]
    cmp     x21, #2
    beq     .Ldisp_call_2reg
    ldr     x2, [x20, #16]
    cmp     x21, #3
    beq     .Ldisp_call_3reg
    ldr     x3, [x20, #24]
    cmp     x21, #4
    beq     .Ldisp_call_4reg
    ldr     x4, [x20, #32]
    cmp     x21, #5
    beq     .Ldisp_call_5reg
    ldr     x5, [x20, #40]
    cmp     x21, #6
    beq     .Ldisp_call_6reg
    ldr     x6, [x20, #48]
    cmp     x21, #7
    beq     .Ldisp_call_7reg
    ldr     x7, [x20, #56]
    b       .Ldisp_call_doregs

.Ldisp_call_noregs:
.Ldisp_call_1reg:
.Ldisp_call_2reg:
.Ldisp_call_3reg:
.Ldisp_call_4reg:
.Ldisp_call_5reg:
.Ldisp_call_6reg:
.Ldisp_call_7reg:
.Ldisp_call_doregs:
    /* Call the function */
    blr     x4

    /* Result in x0 */
.Ldisp_call_done:
    /* Decrement call depth */
    ldr     x3, =wasm_dispatch_call_depth
    ldr     w4, [x3]
    sub     w4, w4, #1
    str     w4, [x3]

    b       .Ldisp_call_ret

.Ldisp_call_overflow:
    mov     x0, #-1
    b       .Ldisp_call_ret

.Ldisp_call_badidx:
    mov     x0, #-1

.Ldisp_call_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_dispatch_trampoline — WASM→host function trampoline
 * Input: x0 = host function pointer, x1 = WASM value stack pointer
 * Output: x0 = return value
 *
 * This is called when WASM code invokes an imported host function.
 * Reads arguments from the WASM value stack, sets up AArch64 args,
 * calls the host function, and pushes result back to WASM stack.
 * ----------------------------------------------------------------------------- */
.global wasm_dispatch_trampoline
wasm_dispatch_trampoline:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* host fn */
    mov     x20, x1             /* wasm stack ptr */

    /* Read number of params from signature */
    /* Simplified: read up to 8 values from WASM stack */
    ldr     x0, [x20, #0]
    ldr     x1, [x20, #8]
    ldr     x2, [x20, #16]
    ldr     x3, [x20, #24]
    ldr     x4, [x20, #32]
    ldr     x5, [x20, #40]
    ldr     x6, [x20, #48]
    ldr     x7, [x20, #56]

    /* Call host function */
    blr     x19

    /* Result in x0 — caller will push to WASM stack */

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_dispatch_indirect — WASM call_indirect instruction
 * Input: x0 = table index, x1 = element index, x2 = expected signature index,
 *        x3 = args pointer, x4 = nargs
 * Output: x0 = return value, or -1 (type mismatch or out of bounds)
 *
 * Performs:
 * 1. Look up function index from table[table_idx][elem_idx]
 * 2. Verify signature matches expected type
 * 3. Call the function
 * ----------------------------------------------------------------------------- */
.global wasm_dispatch_indirect
wasm_dispatch_indirect:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!
    stp     x21, x22, [sp, #-16]!

    mov     x19, x0             /* table index */
    mov     x20, x1             /* element index */
    mov     x21, x3             /* args ptr */
    mov     x22, x4             /* nargs */

    /* Validate table index */
    cmp     x19, #WASM_DISP_MAX_TABLES
    b.hs    .Ldisp_ind_fail

    /* Get table descriptor */
    ldr     x3, =wasm_dispatch_func_tables
    mov     x4, #16
    mul     x4, x19, x4
    add     x3, x3, x4

    /* Load base_ptr and size */
    ldr     x5, [x3, #0]        /* base_ptr */
    ldr     w6, [x3, #8]        /* size */

    /* Check element index in bounds */
    cmp     x20, x6
    b.hs    .Ldisp_ind_fail

    /* Look up function index */
    ldr     w7, [x5, x20, lsl #2]     /* func_idx = table[elem_idx] */

    /* Check for null reference */
    cmp     w7, #0xFFFFFFFF
    b.eq    .Ldisp_ind_fail

    /* TODO: Verify signature matches x2 (expected sig index) */
    /* Simplified: skip type check for now */

    /* Call the function */
    mov     x0, w7              /* func index */
    mov     x1, x21             /* args ptr */
    mov     x2, x22             /* nargs */
    bl      wasm_dispatch_call

    b       .Ldisp_ind_ret

.Ldisp_ind_fail:
    mov     x0, #-1

.Ldisp_ind_ret:
    ldp     x21, x22, [sp], #16
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
