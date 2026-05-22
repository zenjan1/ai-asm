/*
 * aiasm-aarch64/kernel/shell.asm
 * Simple command-line shell for AI-ASM AArch64
 * Reads from PL011 UART, outputs to serial and VGA
 */
.arch armv8-a

.text

.bss
.align 4
shell_input_buf:
    .skip 128

.text

/* -----------------------------------------------------------------------------
 * Function: shell_start
 * Description: Start the interactive shell loop
 * Input: None
 * Output: Never returns
 * Clobbered: all
 * Stack: variable
 * ----------------------------------------------------------------------------- */
.global shell_start
shell_start:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    /* Print shell banner */
    adrp    x0, shell_banner
    add     x0, x0, :lo12:shell_banner
    bl      serial_puts

    adrp    x0, shell_prompt
    add     x0, x0, :lo12:shell_prompt
    bl      serial_puts

1:
    /* Read a line of input */
    bl      shell_readline

    /* Parse and execute command */
    bl      shell_exec

    /* Print prompt again */
    adrp    x0, shell_prompt
    add     x0, x0, :lo12:shell_prompt
    bl      serial_puts

    b       1b

/* -----------------------------------------------------------------------------
 * Function: shell_readline
 * Description: Read characters until newline, store in buffer
 * Input: None
 * Output: x0 = buffer pointer
 * Clobbered: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global shell_readline
shell_readline:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, shell_input_buf
    add     x0, x0, :lo12:shell_input_buf
    mov     x2, x0                /* Buffer start */

1:
    bl      serial_getc
    strb    w0, [x0]
    add     x0, x0, #1

    /* Echo character */
    bl      serial_putc

    /* Check for newline or backspace */
    cmp     w0, #'\r'
    b.eq    2f
    cmp     w0, #'\n'
    b.eq    2f
    cmp     w0, #0x7f             /* Backspace */
    b.eq    3f
    cmp     w0, #0x08             /* Ctrl+H */
    b.eq    3f
    b       1b

2:
    /* Terminate string */
    strb    wzr, [x0]
    mov     x0, x2
    ldp     x29, x30, [sp], #16
    ret

3:
    /* Handle backspace: move pointer back, print space + backspace */
    cmp     x0, x2
    b.eq    1b                    /* Don't go before buffer start */
    sub     x0, x0, #1
    mov     w0, #0x08             /* Backspace */
    bl      serial_putc
    mov     w0, #' '
    bl      serial_putc
    mov     w0, #0x08
    bl      serial_putc
    b       1b

/* -----------------------------------------------------------------------------
 * Function: shell_exec
 * Description: Parse and execute a command from input buffer
 * Input: None
 * Clobbered: all
 * Stack: variable
 * ----------------------------------------------------------------------------- */
.global shell_exec
shell_exec:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, shell_input_buf
    add     x0, x0, :lo12:shell_input_buf
    ldrb    w0, [x0]
    cbz     w0, shell_exec_done   /* Empty input */

    /* Compare "help" */
    adrp    x1, cmd_help
    add     x1, x1, :lo12:cmd_help
    adrp    x2, shell_input_buf
    add     x2, x2, :lo12:shell_input_buf
    bl      strcmp
    cbz     x0, shell_do_help

    /* Compare "version" */
    adrp    x1, cmd_version
    add     x1, x1, :lo12:cmd_version
    adrp    x2, shell_input_buf
    add     x2, x2, :lo12:shell_input_buf
    bl      strcmp
    cbz     x0, shell_do_version

    /* Compare "clear" */
    adrp    x1, cmd_clear
    add     x1, x1, :lo12:cmd_clear
    adrp    x2, shell_input_buf
    add     x2, x2, :lo12:shell_input_buf
    bl      strcmp
    cbz     x0, shell_do_clear

    /* Compare "log" */
    adrp    x1, cmd_log
    add     x1, x1, :lo12:cmd_log
    adrp    x2, shell_input_buf
    add     x2, x2, :lo12:shell_input_buf
    bl      strcmp
    cbz     x0, shell_do_log

    /* Unknown command */
    adrp    x0, msg_unknown_cmd
    add     x0, x0, :lo12:msg_unknown_cmd
    bl      serial_puts

shell_exec_done:
    adrp    x0, msg_newline
    add     x0, x0, :lo12:msg_newline
    bl      serial_puts
    ldp     x29, x30, [sp], #16
    ret

shell_do_help:
    adrp    x0, msg_help
    add     x0, x0, :lo12:msg_help
    bl      serial_puts
    b       shell_exec_done

shell_do_version:
    adrp    x0, msg_version
    add     x0, x0, :lo12:msg_version
    bl      serial_puts
    b       shell_exec_done

shell_do_clear:
    bl      vga_init
    b       shell_exec_done

shell_do_log:
    adrp    x0, msg_log_test
    add     x0, x0, :lo12:msg_log_test
    bl      serial_puts
    mov     w0, #1              /* INFO */
    adrp    x1, event_shell
    add     x1, x1, :lo12:event_shell
    adrp    x2, data_shell_log
    add     x2, x2, :lo12:data_shell_log
    bl      log_event
    b       shell_exec_done

/* String constants */
.section .rodata
.align 4
shell_banner:
    .asciz "\r\nAI-ASM AArch64 Shell v0.1\r\n"
shell_prompt:
    .asciz "aarch64> "
cmd_help:
    .asciz "help"
cmd_version:
    .asciz "version"
cmd_clear:
    .asciz "clear"
cmd_log:
    .asciz "log"
msg_unknown_cmd:
    .asciz "Unknown command. Type 'help' for commands."
msg_newline:
    .asciz "\r\n"
msg_help:
    .asciz "Commands: help, version, clear, log\r\n"
msg_version:
    .asciz "AI-ASM AArch64 v0.1 - Pure Assembly AI OS\r\n"
msg_log_test:
    .asciz "Emitting test log event...\r\n"
event_shell:
    .asciz "shell_command"
data_shell_log:
    .asciz "{\"cmd\":\"log\",\"status\":\"ok\"}"
