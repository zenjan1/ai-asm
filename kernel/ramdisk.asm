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
 * File manager module
 * --------------------------------------------------------------------------- */
.section .rodata.filemgr_wasm
.global filemgr_module_start
filemgr_module_start:
    .incbin "kernel/filemgr.wasm"
.global filemgr_module_end
filemgr_module_end:
.align 4
.global filemgr_module_size
filemgr_module_size:
    .int filemgr_module_end - filemgr_module_start

/* ---------------------------------------------------------------------------
 * Settings module
 * --------------------------------------------------------------------------- */
.section .rodata.settings_wasm
.global settings_module_start
settings_module_start:
    .incbin "kernel/settings.wasm"
.global settings_module_end
settings_module_end:
.align 4
.global settings_module_size
settings_module_size:
    .int settings_module_end - settings_module_start

/* ---------------------------------------------------------------------------
 * User authentication module
 * --------------------------------------------------------------------------- */
.section .rodata.user_wasm
.global user_module_start
user_module_start:
    .incbin "kernel/user.wasm"
.global user_module_end
user_module_end:
.align 4
.global user_module_size
user_module_size:
    .int user_module_end - user_module_start

/* ---------------------------------------------------------------------------
 * Device manager module
 * --------------------------------------------------------------------------- */
.section .rodata.devmgr_wasm
.global devmgr_module_start
devmgr_module_start:
    .incbin "kernel/devmgr.wasm"
.global devmgr_module_end
devmgr_module_end:
.align 4
.global devmgr_module_size
devmgr_module_size:
    .int devmgr_module_end - devmgr_module_start

/* ---------------------------------------------------------------------------
 * HTTP server module
 * --------------------------------------------------------------------------- */
.section .rodata.httpd_wasm
.global httpd_module_start
httpd_module_start:
    .incbin "kernel/httpd.wasm"
.global httpd_module_end
httpd_module_end:
.align 4
.global httpd_module_size
httpd_module_size:
    .int httpd_module_end - httpd_module_start

/* ---------------------------------------------------------------------------
 * DNS resolver module
 * --------------------------------------------------------------------------- */
.section .rodata.dns_resolver_wasm
.global dns_resolver_module_start
dns_resolver_module_start:
    .incbin "kernel/dns_resolver.wasm"
.global dns_resolver_module_end
dns_resolver_module_end:
.align 4
.global dns_resolver_module_size
dns_resolver_module_size:
    .int dns_resolver_module_end - dns_resolver_module_start

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
