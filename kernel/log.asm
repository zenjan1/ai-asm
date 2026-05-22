/*
 * aiasm-aarch64/kernel/log.asm
 * Structured JSON logging system for AI-ASM AArch64
 * All output is single-line JSON, machine-readable
 */
.arch armv8-a

.text

/* Log levels */
.set LOG_DEBUG, 0
.set LOG_INFO,  1
.set LOG_WARN,  2
.set LOG_ERROR, 3

/* Global: timestamp counter */
.bss
.align 4
log_timestamp:
    .skip 8

.text

/* -----------------------------------------------------------------------------
 * Function: log_init
 * Description: Initialize structured logging system, print boot header
 * Input: None
 * Output: None
 * Clobbered registers: x0, x1, x2
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global log_init
log_init:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    /* Initialize timestamp */
    adrp    x0, log_timestamp
    add     x0, x0, :lo12:log_timestamp
    mov     x1, #0
    str     x1, [x0]

    /* Print JSON boot message */
    adrp    x0, msg_boot_json
    add     x0, x0, :lo12:msg_boot_json
    bl      serial_puts

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: log_event
 * Description: Emit a structured JSON log line
 * Input: w0 = level (0=DEBUG,1=INFO,2=WARN,3=ERROR)
 *        x1 = event name string pointer
 *        x2 = JSON data string pointer (may be empty)
 * Output: None
 * Clobbered registers: x0-x5
 * Stack: 32 bytes
 * ----------------------------------------------------------------------------- */
.global log_event
log_event:
    stp     x29, x30, [sp, #-32]!
    mov     x29, sp
    stp     x19, x20, [sp, #16]
    mov     x19, x1             /* Save event name */
    mov     x20, x2             /* Save data string */

    /* Increment timestamp */
    adrp    x3, log_timestamp
    add     x3, x3, :lo12:log_timestamp
    ldr     x4, [x3]
    add     x4, x4, #1
    str     x4, [x3]

    /* Print: {"timestamp":NNN, */
    adrp    x0, msg_json_prefix
    add     x0, x0, :lo12:msg_json_prefix
    bl      serial_puts

    /* Print timestamp value */
    mov     x0, x4
    bl      print_number

    /* Print: "level":" */
    adrp    x0, msg_level_prefix
    add     x0, x0, :lo12:msg_level_prefix
    bl      serial_puts

    /* Print level string */
    cmp     w0, #0
    b.lt    1f
    cmp     w0, #3
    b.gt    1f
    adrp    x5, level_strings
    add     x5, x5, :lo12:level_strings
    lsl     x0, x0, #3          /* level * 8 bytes per string */
    add     x5, x5, x0
    ldr     x0, [x5]
    bl      serial_puts

1:
    /* Print: ","event":" */
    adrp    x0, msg_event_prefix
    add     x0, x0, :lo12:msg_event_prefix
    bl      serial_puts

    /* Print event name */
    mov     x0, x19
    bl      serial_puts

    /* Print: " */
    mov     x0, #'"'
    bl      serial_putc

    /* Print data if not null */
    cbz     x20, 2f
    /* Print: ,"data": */
    adrp    x0, msg_data_prefix
    add     x0, x0, :lo12:msg_data_prefix
    bl      serial_puts
    mov     x0, x20
    bl      serial_puts

2:
    /* Print: "}\n */
    adrp    x0, msg_json_suffix
    add     x0, x0, :lo12:msg_json_suffix
    bl      serial_puts

    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #32
    ret

/* -----------------------------------------------------------------------------
 * Function: log_boot_message
 * Description: Print boot JSON message
 * Input: x0 = message string (unused, uses built-in message)
 * Output: None
 * Clobbered: x0-x5
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global log_boot_message
log_boot_message:
    stp     x29, x30, [sp, #-16]!

    mov     w0, #LOG_INFO       /* level = INFO */
    adrp    x1, event_name_boot
    add     x1, x1, :lo12:event_name_boot
    adrp    x2, data_boot
    add     x2, x2, :lo12:data_boot
    bl      log_event

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * Function: print_number
 * Description: Print a 64-bit unsigned integer in decimal
 * Input: x0 = number to print
 * Output: None
 * Clobbered: x0, x1, x2, x3
 * Stack: 16 bytes
 * ----------------------------------------------------------------------------- */
.global print_number
print_number:
    stp     x29, x30, [sp, #-16]!
    cbz     x0, print_zero

    /* Build digits on stack */
    mov     x29, sp
    mov     x1, #0              /* digit count */
    mov     x2, #10
1:
    udiv    x3, x0, x2
    msub    x0, x3, x2, x0      /* x0 = remainder */
    add     x0, x0, #'0'
    sub     sp, sp, #1
    strb    w0, [sp]
    add     x1, x1, #1
    mov     x0, x3
    cbnz    x0, 1b

    /* Print digits */
2:
    cbz     x1, 3f
    ldrb    w0, [sp], #1
    bl      serial_putc
    sub     x1, x1, #1
    b       2b
3:
    ldp     x29, x30, [sp], #16
    ret

print_zero:
    mov     w0, #'0'
    bl      serial_putc
    ldp     x29, x30, [sp], #16
    ret

/* String data */
.section .rodata
.align 4
msg_boot_json:
    .asciz "{\"timestamp\":0,\"level\":\"INFO\",\"event\":\"boot\",\"data\":{\"version\":\"0.1-aarch64\",\"arch\":\"aarch64\"}}\n"

msg_json_prefix:
    .asciz "{\"timestamp\":"
msg_level_prefix:
    .asciz ",\"level\":\""
msg_event_prefix:
    .asciz "\",\"event\":\""
msg_data_prefix:
    .asciz "\",\"data\":"
msg_json_suffix:
    .asciz "}\n"

level_strings:
    .quad str_debug
    .quad str_info
    .quad str_warn
    .quad str_error
str_debug:
    .asciz "DEBUG"
str_info:
    .asciz "INFO"
str_warn:
    .asciz "WARN"
str_error:
    .asciz "ERROR"

event_name_boot:
    .asciz "boot"
data_boot:
    .asciz "{\"version\":\"0.1-aarch64\",\"arch\":\"aarch64\"}"
