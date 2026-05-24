/*
 * aiasm-aarch64/kernel/ramdisk.asm
 * Embeds WASM modules (init, shell) and RAM disk TAR into kernel binary.
 * Provides symbols for each embedded resource.
 */
.arch armv8-a

/* ---------------------------------------------------------------------------
 * Init module
 * --------------------------------------------------------------------------- */
.section .rodata.init_wasm
.global init_module_start
init_module_start:
    .incbin "kernel/init.wasm"
.global init_module_end
init_module_end:
.align 4
.global init_module_size
init_module_size:
    .int init_module_end - init_module_start

/* ---------------------------------------------------------------------------
 * Shell module
 * --------------------------------------------------------------------------- */
.section .rodata.shell_wasm
.global shell_module_start
shell_module_start:
    .incbin "kernel/shell.wasm"
.global shell_module_end
shell_module_end:
.align 4
.global shell_module_size
shell_module_size:
    .int shell_module_end - shell_module_start

/* ---------------------------------------------------------------------------
 * Test module (integration test)
 * --------------------------------------------------------------------------- */
.section .rodata.test_wasm
.global test_module_start
test_module_start:
    .incbin "kernel/test.wasm"
.global test_module_end
test_module_end:
.align 4
.global test_module_size
test_module_size:
    .int test_module_end - test_module_start

/* ---------------------------------------------------------------------------
 * Proc monitor module
 * --------------------------------------------------------------------------- */
.section .rodata.proc_monitor_wasm
.global proc_monitor_module_start
proc_monitor_module_start:
    .incbin "kernel/proc_monitor.wasm"
.global proc_monitor_module_end
proc_monitor_module_end:
.align 4
.global proc_monitor_module_size
proc_monitor_module_size:
    .int proc_monitor_module_end - proc_monitor_module_start

/* ---------------------------------------------------------------------------
 * Syslog module
 * --------------------------------------------------------------------------- */
.section .rodata.syslog_wasm
.global syslog_module_start
syslog_module_start:
    .incbin "kernel/syslog.wasm"
.global syslog_module_end
syslog_module_end:
.align 4
.global syslog_module_size
syslog_module_size:
    .int syslog_module_end - syslog_module_start

/* ---------------------------------------------------------------------------
 * RAM disk (USTAR TAR archive)
 * --------------------------------------------------------------------------- */
.section .rodata.ramdisk
.global ramdisk_start
ramdisk_start:
    .incbin "kernel/ramdisk.tar"
.global ramdisk_end
ramdisk_end:
.align 4
.global ramdisk_size
ramdisk_size:
    .int ramdisk_end - ramdisk_start
