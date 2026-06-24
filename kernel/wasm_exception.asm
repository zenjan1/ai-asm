/*
 * aiasm-aarch64/kernel/wasm_exception.asm
 * WASM Exception Handling & Trap Management (v1.0)
 *
 * AArch64 ASM implementation of WebAssembly exception handling proposal.
 * Manages WASM traps (divide-by-zero, out-of-bounds, unreachable, stack-overflow),
 * try/catch at kernel level, stack unwinding, and exception propagation.
 *
 * WASM exception model:
 *   - Traps are unrecoverable errors that abort execution
 *   - Exceptions (proposal) are recoverable via try/catch
 *   - Exception tags identify the type of exception
 *   - Exception objects carry payload data
 *
 * Integration: called from wasi.asm and wasm_dispatch.asm on traps
 *
 * API:
 *   wasm_exception_init()                          — init exception subsystem
 *   wasm_exception_register_tag(tag_id, name_ptr)  — register exception tag
 *   wasm_exception_throw(tag_id, payload_ptr, size) — throw exception (never returns)
 *   wasm_exception_catch(tag_id, handler, ctx)     — set up catch handler
 *   wasm_exception_get_tag() => tag_id             — get current exception tag
 *   wasm_exception_get_payload(dst, max) => size   — copy exception payload
 *   wasm_exception_clear()                         — clear current exception
 */
.arch armv8-a

/* Trap codes (match WASM spec) */
.set WASM_TRAP_UNREACHABLE,     0
.set WASM_TRAP_MEM_OOB,         1
.set WASM_TRAP_DIV_BY_ZERO,     2
.set WASM_TRAP_INT_OVERFLOW,    3
.set WASM_TRAP_INVALID_CONV,    4
.set WASM_TRAP_STACK_OVERFLOW,  5
.set WASM_TRAP_INDIRECT_CALL,   6
.set WASM_TRAP_TYPE_MISMATCH,   7

/* Constants */
.set WASM_EXC_MAX_TAGS,      64
.set WASM_EXC_MAX_DEPTH,     32     /* max nested try/catch */
.set WASM_EXC_MAX_PAYLOAD,   256    /* max payload bytes */

.bss
.align 4
.global wasm_exception_init_done
wasm_exception_init_done:
    .skip 4

/* Exception tag registry: array of {tag_id, name_ptr} = 16 bytes each */
.align 4
wasm_exception_tags:
    .skip WASM_EXC_MAX_TAGS * 16   /* 1024 bytes */
wasm_exception_tag_count:
    .skip 4

/* Current exception state */
.align 4
wasm_exception_active:
    .skip 4          /* 1 if exception is active */
wasm_exception_tag:
    .skip 4          /* current exception tag ID */
wasm_exception_trap_code:
    .skip 4          /* trap code (if trap, not exception) */
wasm_exception_payload:
    .skip WASM_EXC_MAX_PAYLOAD   /* exception payload data */
wasm_exception_payload_size:
    .skip 4          /* actual payload size */

/* Try/catch handler stack */
/* Each entry: {catch_addr, catch_sp, catch_fp, context_ptr} = 32 bytes */
.align 4
wasm_exception_handlers:
    .skip WASM_EXC_MAX_DEPTH * 32  /* 1024 bytes */
wasm_exception_handler_depth:
    .skip 4          /* current handler stack depth */

.text

/* -----------------------------------------------------------------------------
 * wasm_exception_init — initialize exception subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global wasm_exception_init
wasm_exception_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero tag registry */
    ldr     x0, =wasm_exception_tags
    mov     x1, #(WASM_EXC_MAX_TAGS * 16)
    bl      .Lexc_zero

    /* Zero handler stack */
    ldr     x0, =wasm_exception_handlers
    mov     x1, #(WASM_EXC_MAX_DEPTH * 32)
    bl      .Lexc_zero

    /* Clear exception state */
    ldr     x0, =wasm_exception_active
    str     wzr, [x0]
    ldr     x0, =wasm_exception_tag
    str     wzr, [x0]
    ldr     x0, =wasm_exception_trap_code
    str     wzr, [x0]
    ldr     x0, =wasm_exception_payload_size
    str     wzr, [x0]
    ldr     x0, =wasm_exception_handler_depth
    str     wzr, [x0]
    ldr     x0, =wasm_exception_tag_count
    str     wzr, [x0]

    ldr     x0, =wasm_exception_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lexc_zero:
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
 * wasm_exception_register_tag — register an exception tag
 * Input: x0 = tag ID, x1 = name pointer (null-terminated string)
 * Output: x0 = 0 (ok), -1 (too many tags)
 * ----------------------------------------------------------------------------- */
.global wasm_exception_register_tag
wasm_exception_register_tag:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* tag ID */
    mov     x20, x1             /* name ptr */

    /* Check tag count */
    ldr     x3, =wasm_exception_tag_count
    ldr     w3, [x3]
    cmp     w3, #WASM_EXC_MAX_TAGS
    b.hs    .Lexc_reg_fail

    /* Store tag entry */
    ldr     x3, =wasm_exception_tags
    mov     x4, #16
    mul     x4, w3, x4          /* uxtw implicit in mul with w register */
    add     x3, x3, x4
    str     w19, [x3, #0]       /* tag_id */
    str     x20, [x3, #8]       /* name_ptr */

    /* Increment count */
    ldr     x3, =wasm_exception_tag_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    b       .Lexc_reg_ret

.Lexc_reg_fail:
    mov     x0, #-1

.Lexc_reg_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_exception_throw — throw an exception (never returns normally)
 * Input: x0 = tag ID, x1 = payload pointer, x2 = payload size
 * Output: Does not return on success. x0 = -1 if no handler found.
 *
 * If a try/catch handler is active, transfers control to the handler.
 * Otherwise, converts to a trap (unrecoverable).
 * ----------------------------------------------------------------------------- */
.global wasm_exception_throw
wasm_exception_throw:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* tag ID */
    mov     x20, x1             /* payload ptr */

    /* Set exception as active */
    ldr     x3, =wasm_exception_active
    mov     w4, #1
    str     w4, [x3]

    /* Store tag */
    ldr     x3, =wasm_exception_tag
    str     w19, [x3]

    /* Copy payload */
    ldr     x3, =wasm_exception_payload
    ldr     x4, =wasm_exception_payload_size

    /* Clamp size */
    cmp     x2, #WASM_EXC_MAX_PAYLOAD
    b.ls    1f
    mov     x2, #WASM_EXC_MAX_PAYLOAD
1:  str     w2, [x4]

    /* Copy payload bytes */
    cbz     x2, .Lexc_throw_copy_done
    mov     x4, x3              /* dst */
    mov     x5, x20             /* src */
    mov     x6, x2              /* count */
.Lexc_throw_copy:
    cbz     x6, .Lexc_throw_copy_done
    ldrb    w7, [x5], #1
    strb    w7, [x4], #1
    sub     x6, x6, #1
    b       .Lexc_throw_copy

.Lexc_throw_copy_done:
    /* Check if there's a handler */
    ldr     x3, =wasm_exception_handler_depth
    ldr     w3, [x3]
    cbz     w3, .Lexc_throw_trap    /* no handler → trap */

    /* Pop handler from stack */
    sub     w3, w3, #1
    str     w3, [x3]

    ldr     x4, =wasm_exception_handlers
    mov     x5, #32
    mul     x5, w3, x5
    add     x4, x4, x5

    /* Load handler info */
    ldr     x5, [x4, #0]        /* catch_addr */
    ldr     x6, [x4, #8]        /* catch_sp */
    ldr     x7, [x4, #16]       /* catch_fp */

    /* Restore stack and frame pointers */
    mov     sp, x6
    mov     x29, x7

    /* Jump to catch handler (x0 = tag ID as argument) */
    mov     x0, x19
    br      x5

    /* Should not reach here */

.Lexc_throw_trap:
    /* No handler — this is a trap */
    /* In real OS: would trigger exception vector or abort */
    mov     x0, #-1
    b       .Lexc_throw_ret

.Lexc_throw_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_exception_catch — set up a catch handler
 * Input: x0 = catch address, x1 = context pointer
 * Output: x0 = 0 (ok), -1 (handler stack full)
 *
 * Pushes a handler onto the try/catch stack. When an exception is thrown,
 * control transfers to catch_addr with the tag ID in x0.
 * ----------------------------------------------------------------------------- */
.global wasm_exception_catch
wasm_exception_catch:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* catch_addr */
    mov     x20, x1             /* context */

    /* Check handler depth */
    ldr     x3, =wasm_exception_handler_depth
    ldr     w3, [x3]
    cmp     w3, #WASM_EXC_MAX_DEPTH
    b.hs    .Lexc_catch_fail

    /* Store handler entry */
    ldr     x4, =wasm_exception_handlers
    mov     x5, #32
    mul     x5, w3, x5
    add     x4, x4, x5

    str     x19, [x4, #0]       /* catch_addr */
    mov     x5, sp
    str     x5, [x4, #8]        /* catch_sp (current sp) */
    str     x29, [x4, #16]      /* catch_fp (current x29) */
    str     x20, [x4, #24]      /* context */

    /* Increment depth */
    ldr     x3, =wasm_exception_handler_depth
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, #0
    b       .Lexc_catch_ret

.Lexc_catch_fail:
    mov     x0, #-1

.Lexc_catch_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_exception_get_tag — get current exception tag
 * Output: x0 = tag ID (or -1 if no active exception)
 * ----------------------------------------------------------------------------- */
.global wasm_exception_get_tag
wasm_exception_get_tag:
    ldr     x0, =wasm_exception_active
    ldr     w0, [x0]
    cbz     w0, .Lexc_no_active

    ldr     x0, =wasm_exception_tag
    ldr     w0, [x0]
    ret

.Lexc_no_active:
    mov     x0, #-1
    ret

/* -----------------------------------------------------------------------------
 * wasm_exception_get_payload — copy exception payload to destination
 * Input: x0 = destination buffer, x1 = max bytes to copy
 * Output: x0 = actual payload size copied
 * ----------------------------------------------------------------------------- */
.global wasm_exception_get_payload
wasm_exception_get_payload:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* dst */
    mov     x20, x1             /* max */

    /* Get actual payload size */
    ldr     x3, =wasm_exception_payload_size
    ldr     w3, [x3]

    /* Use min(actual, max) */
    cmp     w3, w20
    b.ls    1f
    mov     w3, w20
1:  mov     w4, w3              /* copy count */

    /* Copy payload */
    ldr     x5, =wasm_exception_payload
    cbz     w4, .Lexc_payload_done

.Lexc_payload_copy:
    cbz     w4, .Lexc_payload_done
    ldrb    w6, [x5], #1
    strb    w6, [x19], #1
    sub     w4, w4, #1
    b       .Lexc_payload_copy

.Lexc_payload_done:
    mov     x0, w3              /* return actual size */

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * wasm_exception_clear — clear current exception state
 * Output: none
 * Called after exception has been handled to reset for next throw.
 * ----------------------------------------------------------------------------- */
.global wasm_exception_clear
wasm_exception_clear:
    ldr     x0, =wasm_exception_active
    str     wzr, [x0]
    ldr     x0, =wasm_exception_tag
    str     wzr, [x0]
    ldr     x0, =wasm_exception_trap_code
    str     wzr, [x0]
    ldr     x0, =wasm_exception_payload_size
    str     wzr, [x0]
    ret
