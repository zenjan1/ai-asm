/*
 * aiasm-aarch64/kernel/event.asm
 * Kernel event bus for AI-ASM AArch64
 * Provides full kernel observability, publishes events to log
 */
.arch armv8-a

.text

/* Event buffer */
.bss
.align 4
event_buffer:
    .skip 256

.text

/* -----------------------------------------------------------------------------
 * Function: event_init
 * Description: Initialize kernel event bus
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global event_init
event_init:
    stp     x29, x30, [sp, #-16]!

    /* Clear event buffer */
    adrp    x0, event_buffer
    add     x0, x0, :lo12:event_buffer
    mov     x1, #0
    mov     x2, #256
1:
    cbz     x2, 2f
    strb    w1, [x0], #1
    sub     x2, x2, #1
    b       1b
2:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: event_publish
 * Description: Publish an event to the kernel event bus (logs it)
 * Input: w0 = event type ID
 *        x1 = event data pointer
 *        w2 = event data length
 * Output: None
 * Clobbered registers: x0-x5
 * Stack: 24 bytes
 * ----------------------------------------------------------------------------- */
.global event_publish
event_publish:
    stp     x29, x30, [sp, #-24]!
    mov     x29, sp
    stp     x19, x20, [sp, #16]
    mov     x19, x1             /* Save data pointer */
    mov     x20, x2             /* Save data length */

    /* Build JSON log entry: {"event_type":N,"data":"..."} */
    /* Start with prefix */
    adrp    x0, msg_event_pub_prefix
    add     x0, x0, :lo12:msg_event_pub_prefix
    bl      serial_puts

    /* Print event type ID */
    mov     x0, x0
    uxtw    x0, w0
    bl      print_number

    /* Print data if present */
    cbz     x19, 1f
    adrp    x0, msg_event_data_prefix
    add     x0, x0, :lo12:msg_event_data_prefix
    bl      serial_puts
    mov     x0, x19
    bl      serial_puts

1:
    adrp    x0, msg_event_pub_suffix
    add     x0, x0, :lo12:msg_event_pub_suffix
    bl      serial_puts

    /* Also log via structured logger */
    mov     w0, #1              /* INFO level */
    adrp    x1, event_name_publish
    add     x1, x1, :lo12:event_name_publish
    mov     x2, x19
    bl      log_event

    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #24
    ret

/* Need access to print_number from log.asm */
.global print_number

/* Event strings */
.section .rodata
.align 4
msg_event_pub_prefix:
    .asciz "[EVENT] type="
msg_event_data_prefix:
    .asciz ", data="
msg_event_pub_suffix:
    .asciz "\n"
event_name_publish:
    .asciz "event_publish"
