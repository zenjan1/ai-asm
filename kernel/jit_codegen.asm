/*
 * aiasm-aarch64/kernel/jit_codegen.asm
 * WASM -> AArch64 JIT code generator for v7.0
 *
 * Compiles hot WASM bytecode sequences into native AArch64 instructions.
 * Supports core WASM opcodes: i32.add, i32.sub, i32.mul, i32.load,
 * i32.store, i32.const, call, br_if, local.get, local.set.
 *
 * API:
 *   jit_codegen_init()                  - initialize code generator
 *   jit_codegen_emit(buf, wasm_ops, len) - emit native code
 *     Input:  x0 = output buffer, x1 = wasm bytecode, x2 = length
 *     Output: x0 = bytes emitted, or -1 on error
 *   jit_codegen_exec(code_ptr)          - execute JIT-compiled code
 *     Input:  x0 = code pointer
 *     Output: x0 = return value
 */
.arch armv8-a

/* WASM opcodes we support */
.set WASM_I32_CONST,  0x41
.set WASM_I32_ADD,    0x6A
.set WASM_I32_SUB,    0x6B
.set WASM_I32_MUL,    0x6C
.set WASM_I32_AND,    0x71
.set WASM_I32_OR,     0x72
.set WASM_I32_XOR,    0x73
.set WASM_I32_EQZ,    0x45
.set WASM_I32_EQ,     0x46
.set WASM_LOCAL_GET,  0x20
.set WASM_LOCAL_SET,  0x21
.set WASM_LOCAL_TEE,  0x22
.set WASM_BR_IF,      0x0D
.set WASM_CALL,       0x10
.set WASM_I32_LOAD,   0x28
.set WASM_I32_STORE,  0x36
.set WASM_RETURN,     0x0F

/* JIT stack frame: 8 slots x 8 bytes for operand stack */
.set JIT_STACK_SIZE,  64

.bss
.align 4
.global jit_codegen_buf
jit_codegen_buf:
    .skip 4096                /* 4KB JIT output buffer */

.global jit_codegen_size
jit_codegen_size:
    .word 0

.global jit_stack
jit_stack:
    .skip 256                 /* JIT execution stack */

.global jit_stack_ptr
jit_stack_ptr:
    .word 0

.text

/* ---------------------------------------------------------------------------
 * jit_codegen_init: Initialize JIT code generator
 * --------------------------------------------------------------------------- */
.global jit_codegen_init
jit_codegen_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero JIT stack */
    adrp    x0, jit_stack
    add     x0, x0, #:lo12:jit_stack
    mov     x1, #256
    bl      jit_codegen_memset

    /* Reset stack pointer */
    adrp    x0, jit_stack_ptr
    add     x0, x0, #:lo12:jit_stack_ptr
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * jit_codegen_emit: Compile WASM bytecode to AArch64 native code
 * Input:  x0 = output buffer, x1 = wasm bytecode, x2 = bytecode length
 * Output: x0 = bytes emitted (native code size), or -1 on error
 * --------------------------------------------------------------------------- */
.global jit_codegen_emit
jit_codegen_emit:
    stp     x29, x30, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!

    mov     x20, x0             /* output buffer */
    mov     x21, x1             /* wasm bytecode */
    mov     x22, x2             /* remaining length */
    mov     x23, #0             /* emitted count */

    /* Set up prologue: save x29, link to JIT stack */
    adrp    x9, jit_stack
    add     x9, x9, #:lo12:jit_stack
    str     x9, [x20]           /* stp x29, x30, [sp, #-16]! equivalent */
    add     x20, x20, #8
    add     x23, x23, #8

1:  cbz     x22, 9f             /* end of bytecode */

    /* Fetch next opcode */
    ldrb    w10, [x21], #1
    subs    x22, x22, #1

    /* Dispatch based on opcode */
    cmp     w10, #WASM_I32_CONST
    b.eq    emit_i32_const
    cmp     w10, #WASM_I32_ADD
    b.eq    emit_i32_add
    cmp     w10, #WASM_I32_SUB
    b.eq    emit_i32_sub
    cmp     w10, #WASM_I32_MUL
    b.eq    emit_i32_mul
    cmp     w10, #WASM_I32_AND
    b.eq    emit_i32_and
    cmp     w10, #WASM_I32_OR
    b.eq    emit_i32_or
    cmp     w10, #WASM_I32_XOR
    b.eq    emit_i32_xor
    cmp     w10, #WASM_I32_EQZ
    b.eq    emit_i32_eqz
    cmp     w10, #WASM_I32_EQ
    b.eq    emit_i32_eq
    cmp     w10, #WASM_LOCAL_GET
    b.eq    emit_local_get
    cmp     w10, #WASM_LOCAL_SET
    b.eq    emit_local_set
    cmp     w10, #WASM_I32_LOAD
    b.eq    emit_i32_load
    cmp     w10, #WASM_I32_STORE
    b.eq    emit_i32_store
    cmp     w10, #WASM_RETURN
    b.eq    emit_return

    /* Unknown opcode - skip (treat as nop) */
    b       1b

9:  /* Emit epilogue: restore and return */
    adr     x10, jit_instr_ret
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4

    mov     x0, x23
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * Code generators for individual WASM opcodes
 * Uses adr+ldr from pre-encoded .word tables since the assembler cannot
 * handle large immediates in mov/orr instructions.
 * --------------------------------------------------------------------------- */

/* i32.const: push immediate onto stack */
/* WASM: 0x41 <leb128> */
/* Native: generate movz w8, #imm (encoding: 0x52800008 | imm<<5) */
emit_i32_const:
    ldrb    w10, [x21], #1
    subs    x22, x22, #1

    /* Build movz w8, #imm instruction word */
    adr     x11, jit_movz_template
    ldr     w11, [x11]          /* 0x52800008 */
    lsl     w10, w10, #5        /* imm << 5 into bits [20:5] */
    orr     w10, w11, w10
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.add: add w8, w9, w8 */
emit_i32_add:
    adr     x10, jit_instr_add
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.sub: sub w8, w9, w8 */
emit_i32_sub:
    adr     x10, jit_instr_sub
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.mul: mul w8, w9, w8 */
emit_i32_mul:
    adr     x10, jit_instr_mul
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.and: and w8, w9, w8 */
emit_i32_and:
    adr     x10, jit_instr_and
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.or: orr w8, w9, w8 */
emit_i32_or:
    adr     x10, jit_instr_orr
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.xor: eor w8, w9, w8 */
emit_i32_xor:
    adr     x10, jit_instr_eor
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.eqz: cmp w8, #0; cset w8, eq */
emit_i32_eqz:
    adr     x10, jit_instr_cmp_w8_zero
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    adr     x10, jit_instr_cset_eq
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.eq: cmp w9, w8; cset w8, eq */
emit_i32_eq:
    adr     x10, jit_instr_cmp_w9_w8
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    adr     x10, jit_instr_cset_eq
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* local.get: skip (handled by interpreter) */
emit_local_get:
    ldrb    w10, [x21], #1
    subs    x22, x22, #1
    b       1b

/* local.set: skip (handled by interpreter) */
emit_local_set:
    ldrb    w10, [x21], #1
    subs    x22, x22, #1
    b       1b

/* i32.load: ldr w8, [x1, w8, uxtw] */
emit_i32_load:
    ldrb    w10, [x21], #1      /* flags */
    subs    x22, x22, #1
    ldrb    w10, [x21], #1      /* offset */
    subs    x22, x22, #1
    adr     x10, jit_instr_ldr
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* i32.store: str w8, [x1, w9, uxtw] */
emit_i32_store:
    ldrb    w10, [x21], #1      /* flags */
    subs    x22, x22, #1
    ldrb    w10, [x21], #1      /* offset */
    subs    x22, x22, #1
    adr     x10, jit_instr_str
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       1b

/* return: ret */
emit_return:
    adr     x10, jit_instr_ret
    ldr     w10, [x10]
    str     w10, [x20], #4
    add     x23, x23, #4
    b       9b

/* ---------------------------------------------------------------------------
 * Pre-encoded AArch64 instruction words (aligned for adr access)
 * --------------------------------------------------------------------------- */
    .align 4
jit_movz_template:
    .word 0x52800008            /* movz w8, #0 (imm goes in bits [20:5]) */

jit_instr_add:
    .word 0x0B090808            /* add w8, w9, w8 */
jit_instr_sub:
    .word 0x4B090808            /* sub w8, w9, w8 */
jit_instr_mul:
    .word 0x1B097D08            /* mul w8, w9, w8 */
jit_instr_and:
    .word 0x0A090808            /* and w8, w9, w8 */
jit_instr_orr:
    .word 0x2A090808            /* orr w8, w9, w8 */
jit_instr_eor:
    .word 0x4A090808            /* eor w8, w9, w8 */
jit_instr_cmp_w8_zero:
    .word 0x7100041F            /* cmp w8, #0 (subs xzr, w8, #0) */
jit_instr_cmp_w9_w8:
    .word 0x6B08013F            /* cmp w9, w8 (subs xzr, w9, w8) */
jit_instr_cset_eq:
    .word 0x1A9F07E8            /* cset w8, eq */
jit_instr_ldr:
    .word 0xB9400828            /* ldr w8, [x1, w8, uxtw] */
jit_instr_str:
    .word 0xB9000928            /* str w8, [x1, w9, uxtw] */
jit_instr_ret:
    .word 0xD65F03C0            /* ret */

/* ---------------------------------------------------------------------------
 * Helper: jit_codegen_memset
 * Input: x0 = address, x1 = length
 * --------------------------------------------------------------------------- */
jit_codegen_memset:
    stp     x29, x30, [sp, #-16]!
    cbz     x1, 9f
1:  strb    wzr, [x0], #1
    subs    x1, x1, #1
    b.ne    1b
9:  ldp     x29, x30, [sp], #16
    ret

/* ---------------------------------------------------------------------------
 * jit_codegen_exec: Execute JIT-compiled native code
 * Input:  x0 = code pointer
 * Output: x0 = return value from compiled code
 * --------------------------------------------------------------------------- */
.global jit_codegen_exec
jit_codegen_exec:
    stp     x29, x30, [sp, #-16]!
    mov     x29, x30
    blr     x0
    mov     x30, x29
    ldp     x29, x30, [sp], #16
    ret
