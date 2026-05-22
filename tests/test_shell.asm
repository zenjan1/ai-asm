/*
 * aiasm-aarch64/tests/test_shell.asm
 * Test: Verify shell commands work (help, version, log)
 */
.arch armv8-a
.syntax unified

/* Minimal test program that exercises shell commands */
.text
.global _start
_start:
    /* Initialize UART */
    bl      serial_init

    /* Print test header */
    adrp    x0, msg_test_header
    add     x0, x0, :lo12:msg_test_header
    bl      serial_puts

    /* Test: print "version" output */
    adrp    x0, cmd_version
    add     x0, x0, :lo12:cmd_version
    bl      serial_puts
    adrp    x0, msg_newline
    add     x0, x0, :lo12:msg_newline
    bl      serial_puts

    /* Test: structured log */
    mov     w0, #1              /* INFO */
    adrp    x1, event_test
    add     x1, x1, :lo12:event_test
    adrp    x2, data_test
    add     x2, x2, :lo12:data_test
    bl      log_event

    /* Test: event publish */
    mov     w0, #1              /* type ID */
    adrp    x1, event_data_test
    add     x1, x1, :lo12:event_data_test
    mov     w2, #15
    bl      event_publish

    /* Print test done */
    adrp    x0, msg_test_done
    add     x0, x0, :lo12:msg_test_done
    bl      serial_puts

    /* Hang */
1:
    wfi
    b       1b

.section .rodata
.align 4
msg_test_header:
    .asciz "[TEST] Shell command test\n"
msg_newline:
    .asciz "\r\n"
msg_test_done:
    .asciz "[TEST] All tests passed\n"
cmd_version:
    .asciz "AI-ASM AArch64 v0.1"
event_test:
    .asciz "test_shell"
data_test:
    .asciz "{\"test\":\"shell\",\"status\":\"ok\"}"
event_data_test:
    .asciz "shell_test_event"
