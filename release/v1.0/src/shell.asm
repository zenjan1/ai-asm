/*
 * aiasm-aarch64/kernel/shell.asm
 * Serial interactive command shell - JSON output
 * v0.2: adds meminfo, ps, time, status commands
 */
.arch armv8-a

.set CMD_BUF_SIZE, 64

.text

/* Command input buffer */
.bss
.align 4
shell_cmd_buf:
    .skip CMD_BUF_SIZE

.text

/* -----------------------------------------------------------------------------
 * Function: shell_run
 * Description: Main shell loop - prompt, read, exec
 * Input: none
 * Output: never returns
 * Clobbered: all
 * Stack: variable
 * ----------------------------------------------------------------------------- */
.global shell_run
shell_run:
    stp     x29, x30, [sp, #-16]!

    /* Print welcome */
    adrp    x0, msg_shell_welcome
    add     x0, x0, #:lo12:msg_shell_welcome
    bl      serial_puts

shell_loop:
    /* Print prompt */
    adrp    x0, msg_prompt
    add     x0, x0, #:lo12:msg_prompt
    bl      serial_puts

    /* Read line */
    bl      shell_readline

    /* Execute command */
    bl      shell_exec

    b       shell_loop

/* -----------------------------------------------------------------------------
 * Function: shell_readline
 * Description: Read characters until newline into buffer
 * Input: none
 * Output: x0 = buffer pointer
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global shell_readline
shell_readline:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    mov     x2, x0              /* buffer start */

1:
    bl      serial_getc

    /* Echo back */
    bl      serial_putc

    /* Handle special chars */
    cmp     w0, #13             /* '\r' */
    b.eq    3f
    cmp     w0, #10             /* '\n' */
    b.eq    3f
    cmp     w0, #8              /* backspace */
    b.eq    shell_bs
    cmp     w0, #127            /* del */
    b.eq    shell_bs

    /* Store char */
    strb    w0, [x0]
    add     x0, x0, #1
    b       1b

shell_bs:
    cmp     x0, x2
    b.eq    1b                  /* buffer empty, ignore */
    sub     x0, x0, #1
    /* Erase on terminal: BS SP BS */
    mov     w0, #8
    bl      serial_putc
    mov     w0, #32
    bl      serial_putc
    mov     w0, #8
    bl      serial_putc
    b       1b

3:
    /* Null terminate */
    strb    wzr, [x0]
    /* Print newline */
    mov     x0, #10
    bl      serial_putc
    mov     x0, x2
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: shell_exec
 * Description: Parse and execute command
 * Input: none (uses shell_cmd_buf)
 * Output: none
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global shell_exec
shell_exec:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    ldrb    w0, [x0]
    cbz     w0, shell_done        /* empty line */

    /* Match: help */
    adrp    x1, cmd_help
    add     x1, x1, #:lo12:cmd_help
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_help

    /* Match: version */
    adrp    x1, cmd_version
    add     x1, x1, #:lo12:cmd_version
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_version

    /* Match: events */
    adrp    x1, cmd_events
    add     x1, x1, #:lo12:cmd_events
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_events

    /* Match: meminfo */
    adrp    x1, cmd_meminfo
    add     x1, x1, #:lo12:cmd_meminfo
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_meminfo

    /* Match: ps */
    adrp    x1, cmd_ps
    add     x1, x1, #:lo12:cmd_ps
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_ps

    /* Match: time */
    adrp    x1, cmd_time
    add     x1, x1, #:lo12:cmd_time
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_time

    /* Match: status */
    adrp    x1, cmd_status
    add     x1, x1, #:lo12:cmd_status
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_status

    /* Match: reboot */
    adrp    x1, cmd_reboot
    add     x1, x1, #:lo12:cmd_reboot
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_reboot

    /* Match: shutdown */
    adrp    x1, cmd_shutdown
    add     x1, x1, #:lo12:cmd_shutdown
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strcmp
    cbz     x0, shell_shutdown

    /* Match: log N */
    adrp    x1, cmd_log
    add     x1, x1, #:lo12:cmd_log
    adrp    x2, shell_cmd_buf
    add     x2, x2, #:lo12:shell_cmd_buf
    bl      strncmp
    cbz     x0, shell_log

    /* Unknown */
    adrp    x0, msg_unknown
    add     x0, x0, #:lo12:msg_unknown
    bl      serial_puts
    b       shell_done

shell_help:
    adrp    x0, rsp_help
    add     x0, x0, #:lo12:rsp_help
    bl      serial_puts
    b       shell_done

shell_version:
    adrp    x0, rsp_version
    add     x0, x0, #:lo12:rsp_version
    bl      serial_puts
    b       shell_done

shell_events:
    bl      event_dump
    b       shell_done

shell_meminfo:
    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    bl      mem_info
    bl      serial_puts
    b       shell_done

shell_ps:
    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    bl      proc_list
    bl      serial_puts
    b       shell_done

shell_time:
    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    bl      timer_info
    bl      serial_puts
    b       shell_done

shell_status:
    adrp    x0, rsp_status
    add     x0, x0, #:lo12:rsp_status
    bl      serial_puts
    b       shell_done

shell_reboot:
    adrp    x0, rsp_reboot
    add     x0, x0, #:lo12:rsp_reboot
    bl      serial_puts
    bl      shell_hang
    b       shell_done

shell_shutdown:
    adrp    x0, rsp_shutdown
    add     x0, x0, #:lo12:rsp_shutdown
    bl      serial_puts
    bl      shell_hang
    b       shell_done

shell_log:
    /* Parse level digit after "log " */
    adrp    x0, shell_cmd_buf
    add     x0, x0, #:lo12:shell_cmd_buf
    add     x0, x0, #4          /* skip "log " */
    ldrb    w0, [x0]
    sub     w0, w0, #'0'
    cmp     w0, #3
    b.gt    shell_log_bad

    /* Set level */
    bl      log_set_level

    /* Print confirmation */
    adrp    x0, rsp_log_set
    add     x0, x0, #:lo12:rsp_log_set
    bl      serial_puts
    b       shell_done

shell_log_bad:
    adrp    x0, rsp_log_bad
    add     x0, x0, #:lo12:rsp_log_bad
    bl      serial_puts
    b       shell_done

shell_done:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: shell_hang
 * Description: Halt CPU (WFI loop)
 * Input: none
 * Output: none
 * ----------------------------------------------------------------------------- */
.global shell_hang
shell_hang:
1:
    wfi
    b       1b

/* -----------------------------------------------------------------------------
 * Function: strncmp
 * Description: Compare if x1 is prefix of x0
 * Input: x0 = a, x1 = prefix b
 * Output: x0 = 0 if b is prefix of a
 * Clobbered: x0-x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global strncmp
strncmp:
    stp     x29, x30, [sp, #-16]!
1:
    ldrb    w2, [x1], #1
    cbz     w2, 2f              /* prefix exhausted -> match */
    ldrb    w3, [x0], #1
    cbz     w3, 3f              /* a ended but prefix didn't -> no match */
    subs    w4, w2, w3
    b.ne    3f
    b       1b
2:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret
3:
    mov     x0, x4
    ldp     x29, x30, [sp], #16
    ret

/* Read-only strings */
.section .rodata
.align 4
msg_shell_welcome:
    .asciz "\r\nAI-ASM AArch64 v0.2\r\n"
msg_prompt:
    .asciz "> "
cmd_help:
    .asciz "help"
cmd_version:
    .asciz "version"
cmd_events:
    .asciz "events"
cmd_meminfo:
    .asciz "meminfo"
cmd_ps:
    .asciz "ps"
cmd_time:
    .asciz "time"
cmd_status:
    .asciz "status"
cmd_reboot:
    .asciz "reboot"
cmd_shutdown:
    .asciz "shutdown"
cmd_log:
    .asciz "log "
msg_unknown:
    .asciz "{\"error\":\"unknown command\"}\n"
rsp_help:
    .asciz "{\"commands\":[\"help\",\"version\",\"log 0|1|2|3\",\"events\",\"meminfo\",\"ps\",\"time\",\"status\",\"reboot\",\"shutdown\"]}\n"
rsp_version:
    .asciz "{\"version\":\"0.2-aarch64\",\"arch\":\"aarch64\",\"build\":\"pure-asm\"}\n"
rsp_status:
    .asciz "{\"kernel\":\"aiasm-aarch64\",\"version\":\"0.2\",\"features\":[\"uart\",\"log\",\"event\",\"memory\",\"timer\",\"gic\",\"process\",\"syscall\"]}\n"
rsp_reboot:
    .asciz "{\"action\":\"reboot\"}\n"
rsp_shutdown:
    .asciz "{\"action\":\"shutdown\"}\n"
rsp_log_set:
    .asciz "{\"level_set\":true}\n"
rsp_log_bad:
    .asciz "{\"error\":\"log level must be 0-3\"}\n"
