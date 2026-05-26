# AI-ASM AArch64 v0.5 — Makefile for Termux
# Toolchain: native binutils (as, ld), clang for C/WASM
#
# Targets:
#   make all        — build full kernel.elf
#   make asm        — compile assembly only
#   make wasm3      — compile wasm3 library only
#   make modules    — compile WASM modules only
#   make kernel     — link kernel.elf only
#   make run        — build and launch in QEMU
#   make clean      — remove all build artifacts
#
# Incremental: only rebuilds changed objects via Make dependency tracking.

AS      = as
LD      = ld
CLANG   = clang
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64

ASFLAGS = -march=armv8-a -g
LDFLAGS = -T kernel/linker.ld -nostdlib -N
CFLAGS_FREESTANDING = -c -O2 -ffreestanding -Ikernel/wasm3/source \
    -mgeneral-regs-only \
    -Dd_m3FixedHeap=1048576 -Dd_m3VerboseErrorMessages=0 \
    -Dd_m3HasFloat=0 -Dd_m3NoFloatDynamic=1 \
    -Dd_m3HasTracer=0 -Dd_m3RecordBacktraces=0 -Dd_m3LogTimestamps=0
WASM_FLAGS = --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
    -Wl,--no-entry -Wl,--export=shell_entry

KERNEL_DIR = kernel
BUILD_DIR  = build
WASM3_DIR  = $(KERNEL_DIR)/wasm3
MODULES_DIR = modules
TARGET_ELF = $(BUILD_DIR)/kernel.elf

# ---------------------------------------------------------------------------
# Assembly sources (auto-detected from kernel/*.asm)
# ---------------------------------------------------------------------------
ASM_SRCS = $(KERNEL_DIR)/kernel.asm \
           $(KERNEL_DIR)/pl011.asm \
           $(KERNEL_DIR)/log.asm \
           $(KERNEL_DIR)/event.asm \
           $(KERNEL_DIR)/utils.asm \
           $(KERNEL_DIR)/memory.asm \
           $(KERNEL_DIR)/timer.asm \
           $(KERNEL_DIR)/gic.asm \
           $(KERNEL_DIR)/exceptions.asm \
           $(KERNEL_DIR)/process.asm \
           $(KERNEL_DIR)/mmu.asm \
           $(KERNEL_DIR)/virtio.asm \
           $(KERNEL_DIR)/virtio_blk.asm \
           $(KERNEL_DIR)/virtio_net.asm \
           $(KERNEL_DIR)/fs.asm \
           $(KERNEL_DIR)/net.asm \
           $(KERNEL_DIR)/wasi.asm \
           $(KERNEL_DIR)/module.asm \
           $(KERNEL_DIR)/virtio_gpu.asm \
           $(KERNEL_DIR)/fb.asm \
           $(KERNEL_DIR)/gui.asm \
           $(KERNEL_DIR)/serial_rx.asm \
           $(KERNEL_DIR)/wasm_embed.asm \
           $(KERNEL_DIR)/ramdisk.asm \
           $(KERNEL_DIR)/ipc.asm \
           $(KERNEL_DIR)/msg.asm \
           $(KERNEL_DIR)/signal.asm \
           $(KERNEL_DIR)/jit_cache.asm \
           $(KERNEL_DIR)/buddy.asm \
           $(KERNEL_DIR)/virtio_irq.asm \
           $(KERNEL_DIR)/user.asm \
           $(KERNEL_DIR)/device.asm \
           $(KERNEL_DIR)/proc_mem.asm \
           $(KERNEL_DIR)/perm.asm \
           $(KERNEL_DIR)/jit_codegen.asm \
           $(KERNEL_DIR)/module_cache.asm \
           $(KERNEL_DIR)/quota.asm \
           $(KERNEL_DIR)/audit.asm \
           $(KERNEL_DIR)/shmem.asm \
           $(KERNEL_DIR)/semaphore.asm \
           $(KERNEL_DIR)/event_notify.asm \
           $(KERNEL_DIR)/vfs.asm \
           $(KERNEL_DIR)/vfs_ops.asm \
           $(KERNEL_DIR)/vfs_path.asm \
           $(KERNEL_DIR)/persist.asm
ASM_OBJS = $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))

# C sources (freestanding)
C_SRCS = $(KERNEL_DIR)/libc_shim.c \
         $(KERNEL_DIR)/wasm_host.c

C_OBJS = $(BUILD_DIR)/libc_shim.o $(BUILD_DIR)/wasm_host.o

# wasm3 sources (exclude tracer/wasi/debug files we don't need)
WASM3_SRCS = $(WASM3_DIR)/source/m3_bind.c \
             $(WASM3_DIR)/source/m3_code.c \
             $(WASM3_DIR)/source/m3_compile.c \
             $(WASM3_DIR)/source/m3_core.c \
             $(WASM3_DIR)/source/m3_env.c \
             $(WASM3_DIR)/source/m3_exec.c \
             $(WASM3_DIR)/source/m3_function.c \
             $(WASM3_DIR)/source/m3_info.c \
             $(WASM3_DIR)/source/m3_module.c \
             $(WASM3_DIR)/source/m3_parse.c
WASM3_OBJS = $(patsubst $(WASM3_DIR)/source/%.c,$(BUILD_DIR)/wasm3/%.o,$(WASM3_SRCS))
WASM3_LIB  = $(BUILD_DIR)/libwasm3.a

# WASM modules
INIT_WASM   = $(MODULES_DIR)/init/init.wasm
SHELL_WASM  = $(MODULES_DIR)/shell/shell.wasm
TEST_WASM   = $(MODULES_DIR)/test/test.wasm
EDITOR_WASM = $(MODULES_DIR)/editor/editor.wasm
CALC_WASM   = $(MODULES_DIR)/calc/calc.wasm
PAINT_WASM  = $(MODULES_DIR)/paint/paint.wasm
LAUNCHER_WASM = $(MODULES_DIR)/launcher/launcher.wasm
NET_TEST_WASM = $(MODULES_DIR)/net_test/net_test.wasm
PROC_MONITOR_WASM = $(MODULES_DIR)/proc_monitor/proc_monitor.wasm
SYSLOG_WASM = $(MODULES_DIR)/syslog/syslog.wasm
FILEMGR_WASM = $(MODULES_DIR)/filemgr/filemgr.wasm
SETTINGS_WASM = $(MODULES_DIR)/settings/settings.wasm
USER_WASM     = $(MODULES_DIR)/user/user.wasm
DEVMGR_WASM   = $(MODULES_DIR)/devmgr/devmgr.wasm
BROWSER_WASM  = $(MODULES_DIR)/browser/browser.wasm
HTTPD_WASM    = $(MODULES_DIR)/httpd/httpd.wasm
DNS_RESOLVER_WASM = $(MODULES_DIR)/dns_resolver/dns_resolver.wasm
SHMEM_TEST_WASM = $(MODULES_DIR)/shmem_test/shmem_test.wasm
SEM_TEST_WASM   = $(MODULES_DIR)/sem_test/sem_test.wasm
GREP_WASM       = $(MODULES_DIR)/grep/grep.wasm
CAT_WASM        = $(MODULES_DIR)/cat/cat.wasm
ECHO_WASM       = $(MODULES_DIR)/echo/echo.wasm
TEE_WASM        = $(MODULES_DIR)/tee/tee.wasm
WC_WASM         = $(MODULES_DIR)/wc/wc.wasm
HEAD_WASM       = $(MODULES_DIR)/head/head.wasm
TAIL_WASM       = $(MODULES_DIR)/tail/tail.wasm
SORT_WASM       = $(MODULES_DIR)/sort/sort.wasm
UNIQ_WASM       = $(MODULES_DIR)/uniq/uniq.wasm
TR_WASM         = $(MODULES_DIR)/tr/tr.wasm
CUT_WASM        = $(MODULES_DIR)/cut/cut.wasm
SED_WASM        = $(MODULES_DIR)/sed/sed.wasm
XARGS_WASM      = $(MODULES_DIR)/xargs/xargs.wasm
DATE_WASM       = $(MODULES_DIR)/date/date.wasm
AWK_WASM        = $(MODULES_DIR)/awk/awk.wasm
LS_WASM         = $(MODULES_DIR)/ls/ls.wasm
PWD_WASM        = $(MODULES_DIR)/pwd/pwd.wasm
ENV_WASM        = $(MODULES_DIR)/env/env.wasm
PRINTF_WASM     = $(MODULES_DIR)/printf/printf.wasm
SLEEP_WASM      = $(MODULES_DIR)/sleep/sleep.wasm
YES_WASM        = $(MODULES_DIR)/yes/yes.wasm
REV_WASM        = $(MODULES_DIR)/rev/rev.wasm
CMP_WASM        = $(MODULES_DIR)/cmp/cmp.wasm
DIFF_WASM       = $(MODULES_DIR)/diff/diff.wasm
BASENAME_WASM   = $(MODULES_DIR)/basename/basename.wasm
DIRNAME_WASM    = $(MODULES_DIR)/dirname/dirname.wasm
NL_WASM         = $(MODULES_DIR)/nl/nl.wasm
SHUF_WASM       = $(MODULES_DIR)/shuf/shuf.wasm
COMM_WASM       = $(MODULES_DIR)/comm/comm.wasm
PASTE_WASM      = $(MODULES_DIR)/paste/paste.wasm
FOLD_WASM       = $(MODULES_DIR)/fold/fold.wasm
TAC_WASM        = $(MODULES_DIR)/tac/tac.wasm
CKSUM_WASM      = $(MODULES_DIR)/cksum/cksum.wasm
SUM_WASM        = $(MODULES_DIR)/sum/sum.wasm
TOUCH_WASM      = $(MODULES_DIR)/touch/touch.wasm
TRUE_WASM       = $(MODULES_DIR)/true/true.wasm
FALSE_WASM      = $(MODULES_DIR)/false/false.wasm
ID_WASM         = $(MODULES_DIR)/id/id.wasm
UPTIME_WASM    = $(MODULES_DIR)/uptime/uptime.wasm
WHOAMI_WASM     = $(MODULES_DIR)/whoami/whoami.wasm
HOSTNAME_WASM   = $(MODULES_DIR)/hostname/hostname.wasm
UNAME_WASM      = $(MODULES_DIR)/uname/uname.wasm
DF_WASM         = $(MODULES_DIR)/df/df.wasm
RAMDISK_TAR = $(KERNEL_DIR)/ramdisk.tar

# All object dependencies
ALL_OBJS = $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)

# ---------------------------------------------------------------------------
# Phony targets
# ---------------------------------------------------------------------------
.PHONY: all asm wasm3 init-wasm shell-wasm ramdisk kernel run clean

all: $(TARGET_ELF)

$(BUILD_DIR) $(BUILD_DIR)/wasm3:
	mkdir -p $@

# ---------------------------------------------------------------------------
# Layered build targets
# ---------------------------------------------------------------------------

# Compile assembly only
asm: $(ASM_OBJS)
	@echo "=== ASM compilation complete ==="

# Compile wasm3 library only
wasm3: $(WASM3_LIB)
	@echo "=== wasm3 library complete ==="

# Compile WASM modules and ramdisk
modules: $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(EDITOR_WASM) $(CALC_WASM) $(PAINT_WASM) $(LAUNCHER_WASM) $(NET_TEST_WASM) $(BROWSER_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM) $(FILEMGR_WASM) $(SETTINGS_WASM) $(USER_WASM) $(DEVMGR_WASM) $(HTTPD_WASM) $(DNS_RESOLVER_WASM) $(SHMEM_TEST_WASM) $(SEM_TEST_WASM) $(GREP_WASM) $(CAT_WASM) $(ECHO_WASM) $(TEE_WASM) $(WC_WASM) $(HEAD_WASM) $(TAIL_WASM) $(SORT_WASM) $(UNIQ_WASM) $(TR_WASM) $(CUT_WASM) $(SED_WASM) $(XARGS_WASM) $(DATE_WASM) $(AWK_WASM) $(LS_WASM) $(PWD_WASM) $(ENV_WASM) $(PRINTF_WASM) $(SLEEP_WASM) $(YES_WASM) $(REV_WASM) $(CMP_WASM) $(DIFF_WASM) $(BASENAME_WASM) $(DIRNAME_WASM) $(NL_WASM) $(SHUF_WASM) $(COMM_WASM) $(PASTE_WASM) $(FOLD_WASM) $(TAC_WASM) $(CKSUM_WASM) $(SUM_WASM) $(TOUCH_WASM) $(TRUE_WASM) $(FALSE_WASM) $(ID_WASM) $(UPTIME_WASM) $(WHOAMI_WASM) $(HOSTNAME_WASM) $(UNAME_WASM) $(DF_WASM) $(RAMDISK_TAR)
	@echo "=== WASM modules and ramdisk complete ==="

# Link kernel only (assumes objects exist)
kernel: $(TARGET_ELF)
	@echo "=== kernel.elf linked ==="

# ---------------------------------------------------------------------------
# Assembly compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	@echo "  AS    $<"
	$(AS) $(ASFLAGS) -o $@ $<

# wasm_embed.o depends on shell.wasm and proc_monitor.wasm and syslog.wasm being present first
$(BUILD_DIR)/wasm_embed.o: $(SHELL_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM)

# ramdisk.o depends on init.wasm, shell.wasm, test.wasm, proc_monitor.wasm, syslog.wasm, filemgr.wasm, settings.wasm, user.wasm, devmgr.wasm, httpd.wasm, grep.wasm, cat.wasm, echo.wasm and ramdisk.tar
$(BUILD_DIR)/ramdisk.o: $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM) $(FILEMGR_WASM) $(SETTINGS_WASM) $(USER_WASM) $(DEVMGR_WASM) $(HTTPD_WASM) $(DNS_RESOLVER_WASM) $(SHMEM_TEST_WASM) $(GREP_WASM) $(CAT_WASM) $(ECHO_WASM) $(TEE_WASM) $(WC_WASM) $(HEAD_WASM) $(TAIL_WASM) $(SORT_WASM) $(UNIQ_WASM) $(TR_WASM) $(CUT_WASM) $(SED_WASM) $(XARGS_WASM) $(DATE_WASM) $(AWK_WASM) $(LS_WASM) $(PWD_WASM) $(ENV_WASM) $(PRINTF_WASM) $(SLEEP_WASM) $(YES_WASM) $(REV_WASM) $(CMP_WASM) $(DIFF_WASM) $(BASENAME_WASM) $(DIRNAME_WASM) $(NL_WASM) $(SHUF_WASM) $(COMM_WASM) $(PASTE_WASM) $(FOLD_WASM) $(TAC_WASM) $(CKSUM_WASM) $(SUM_WASM) $(TOUCH_WASM) $(TRUE_WASM) $(FALSE_WASM) $(ID_WASM) $(UPTIME_WASM) $(WHOAMI_WASM) $(HOSTNAME_WASM) $(UNAME_WASM) $(DF_WASM) $(RAMDISK_TAR)

# ---------------------------------------------------------------------------
# C freestanding compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/libc_shim.o: $(KERNEL_DIR)/libc_shim.c | $(BUILD_DIR)
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -o $@ $<

$(BUILD_DIR)/wasm_host.o: $(KERNEL_DIR)/wasm_host.c | $(BUILD_DIR)
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -O0 -o $@ $<

# ---------------------------------------------------------------------------
# wasm3 compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/wasm3/%.o: $(WASM3_DIR)/source/%.c | $(BUILD_DIR)/wasm3
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -O2 -o $@ $<

$(WASM3_LIB): $(WASM3_OBJS)
	@echo "  AR    $@"
	ar rcs $@ $^

# ---------------------------------------------------------------------------
# WASM init module
# ---------------------------------------------------------------------------
$(INIT_WASM): $(MODULES_DIR)/init/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/init.wasm

# ---------------------------------------------------------------------------
# RAM disk (USTAR TAR from modules/init/ramdisk/)
# ---------------------------------------------------------------------------
$(RAMDISK_TAR): ramdisk/motd.txt ramdisk/hello.txt ramdisk/readme.txt
	@echo "  TAR   $@"
	tar cf $@ -C ramdisk motd.txt hello.txt readme.txt

# ---------------------------------------------------------------------------
# WASM shell module
# ---------------------------------------------------------------------------
$(SHELL_WASM): $(MODULES_DIR)/shell/src/main.c
	@echo "  WASM  $<"
	$(CLANG) $(WASM_FLAGS) -o $@ $<
	cp $@ $(KERNEL_DIR)/shell.wasm

# ---------------------------------------------------------------------------
# WASM test module (integration test)
# ---------------------------------------------------------------------------
$(TEST_WASM): $(MODULES_DIR)/test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/test.wasm

# ---------------------------------------------------------------------------
# WASM editor module
# ---------------------------------------------------------------------------
$(EDITOR_WASM): $(MODULES_DIR)/editor/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/editor.wasm

# ---------------------------------------------------------------------------
# WASM calculator module
# ---------------------------------------------------------------------------
$(CALC_WASM): $(MODULES_DIR)/calc/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/calc.wasm

# ---------------------------------------------------------------------------
# WASM paint module
# ---------------------------------------------------------------------------
$(PAINT_WASM): $(MODULES_DIR)/paint/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/paint.wasm

# ---------------------------------------------------------------------------
# WASM launcher module
# ---------------------------------------------------------------------------
$(LAUNCHER_WASM): $(MODULES_DIR)/launcher/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/launcher.wasm

# ---------------------------------------------------------------------------
# WASM net_test module
# ---------------------------------------------------------------------------
$(NET_TEST_WASM): $(MODULES_DIR)/net_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/net_test.wasm

# ---------------------------------------------------------------------------
# WASM browser module
# ---------------------------------------------------------------------------
$(BROWSER_WASM): $(MODULES_DIR)/browser/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/browser.wasm

# WASM proc_monitor module
# ---------------------------------------------------------------------------
$(PROC_MONITOR_WASM): $(MODULES_DIR)/proc_monitor/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/proc_monitor.wasm

# ---------------------------------------------------------------------------
# WASM syslog module
# ---------------------------------------------------------------------------
$(SYSLOG_WASM): $(MODULES_DIR)/syslog/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/syslog.wasm

# ---------------------------------------------------------------------------
# WASM file manager module
# ---------------------------------------------------------------------------
$(FILEMGR_WASM): $(MODULES_DIR)/filemgr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/filemgr.wasm

# ---------------------------------------------------------------------------
# WASM settings module
# ---------------------------------------------------------------------------
$(SETTINGS_WASM): $(MODULES_DIR)/settings/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/settings.wasm

# ---------------------------------------------------------------------------
# WASM user module
# ---------------------------------------------------------------------------
$(USER_WASM): $(MODULES_DIR)/user/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/user.wasm

# ---------------------------------------------------------------------------
# WASM device manager module
# ---------------------------------------------------------------------------
$(DEVMGR_WASM): $(MODULES_DIR)/devmgr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/devmgr.wasm

# ---------------------------------------------------------------------------
# WASM httpd module (HTTP server)
# ---------------------------------------------------------------------------
$(HTTPD_WASM): $(MODULES_DIR)/httpd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/httpd.wasm

# ---------------------------------------------------------------------------
# WASM dns_resolver module (DNS resolver)
# ---------------------------------------------------------------------------
$(DNS_RESOLVER_WASM): $(MODULES_DIR)/dns_resolver/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dns_resolver.wasm

# ---------------------------------------------------------------------------
# WASM shmem_test module (shared memory IPC test)
# ---------------------------------------------------------------------------
$(SHMEM_TEST_WASM): $(MODULES_DIR)/shmem_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/shmem_test.wasm

# ---------------------------------------------------------------------------
# WASM sem_test module (semaphore IPC test)
# ---------------------------------------------------------------------------
$(SEM_TEST_WASM): $(MODULES_DIR)/sem_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sem_test.wasm

# ---------------------------------------------------------------------------
# WASM grep module (string search utility)
# ---------------------------------------------------------------------------
$(GREP_WASM): $(MODULES_DIR)/grep/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/grep.wasm

# ---------------------------------------------------------------------------
# WASM cat module (file reader)
# ---------------------------------------------------------------------------
$(CAT_WASM): $(MODULES_DIR)/cat/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cat.wasm

# ---------------------------------------------------------------------------
# WASM echo module (argument printer)
# ---------------------------------------------------------------------------
$(ECHO_WASM): $(MODULES_DIR)/echo/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/echo.wasm

# ---------------------------------------------------------------------------
# WASM tee module (stdout + file writer)
# ---------------------------------------------------------------------------
$(TEE_WASM): $(MODULES_DIR)/tee/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tee.wasm

# ---------------------------------------------------------------------------
# WASM wc module (word count)
# ---------------------------------------------------------------------------
$(WC_WASM): $(MODULES_DIR)/wc/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/wc.wasm

# ---------------------------------------------------------------------------
# WASM head module (first N lines)
# ---------------------------------------------------------------------------
$(HEAD_WASM): $(MODULES_DIR)/head/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/head.wasm

# ---------------------------------------------------------------------------
# WASM tail module (last N lines)
# ---------------------------------------------------------------------------
$(TAIL_WASM): $(MODULES_DIR)/tail/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tail.wasm

# ---------------------------------------------------------------------------
# WASM sort module (text sorting)
# ---------------------------------------------------------------------------
$(SORT_WASM): $(MODULES_DIR)/sort/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sort.wasm

# ---------------------------------------------------------------------------
# WASM uniq module (adjacent duplicate removal)
# ---------------------------------------------------------------------------
$(UNIQ_WASM): $(MODULES_DIR)/uniq/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uniq.wasm

# ---------------------------------------------------------------------------
# WASM tr module (character translation/deletion)
# ---------------------------------------------------------------------------
$(TR_WASM): $(MODULES_DIR)/tr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tr.wasm

# ---------------------------------------------------------------------------
# WASM cut module (column extraction)
# ---------------------------------------------------------------------------
$(CUT_WASM): $(MODULES_DIR)/cut/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cut.wasm

# ---------------------------------------------------------------------------
# WASM sed module (stream editor)
# ---------------------------------------------------------------------------
$(SED_WASM): $(MODULES_DIR)/sed/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sed.wasm

# ---------------------------------------------------------------------------
# WASM xargs module (build command from stdin)
# ---------------------------------------------------------------------------
$(XARGS_WASM): $(MODULES_DIR)/xargs/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/xargs.wasm

# ---------------------------------------------------------------------------
# WASM date module (display system time)
# ---------------------------------------------------------------------------
$(DATE_WASM): $(MODULES_DIR)/date/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/date.wasm

# ---------------------------------------------------------------------------
# WASM awk module (pattern scanning and text processing)
# ---------------------------------------------------------------------------
$(AWK_WASM): $(MODULES_DIR)/awk/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/awk.wasm

# ---------------------------------------------------------------------------
# WASM ls module (list directory contents)
# ---------------------------------------------------------------------------
$(LS_WASM): $(MODULES_DIR)/ls/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ls.wasm

# ---------------------------------------------------------------------------
# WASM pwd module (print working directory)
# ---------------------------------------------------------------------------
$(PWD_WASM): $(MODULES_DIR)/pwd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/pwd.wasm

# ---------------------------------------------------------------------------
# WASM env module (environment variables)
# ---------------------------------------------------------------------------
$(ENV_WASM): $(MODULES_DIR)/env/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/env.wasm

# ---------------------------------------------------------------------------
# WASM printf module (formatted output)
# ---------------------------------------------------------------------------
$(PRINTF_WASM): $(MODULES_DIR)/printf/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/printf.wasm

# ---------------------------------------------------------------------------
# WASM sleep module (timing delay)
# ---------------------------------------------------------------------------
$(SLEEP_WASM): $(MODULES_DIR)/sleep/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sleep.wasm

# ---------------------------------------------------------------------------
# WASM yes module (repeated output)
# ---------------------------------------------------------------------------
$(YES_WASM): $(MODULES_DIR)/yes/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/yes.wasm

# ---------------------------------------------------------------------------
# WASM rev module (line reversal)
# ---------------------------------------------------------------------------
$(REV_WASM): $(MODULES_DIR)/rev/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/rev.wasm

# ---------------------------------------------------------------------------
# WASM cmp module (file comparison)
# ---------------------------------------------------------------------------
$(CMP_WASM): $(MODULES_DIR)/cmp/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cmp.wasm

# ---------------------------------------------------------------------------
# WASM diff module (line-by-line file comparison)
# ---------------------------------------------------------------------------
$(DIFF_WASM): $(MODULES_DIR)/diff/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/diff.wasm

# ---------------------------------------------------------------------------
# WASM basename module (path processing)
# ---------------------------------------------------------------------------
$(BASENAME_WASM): $(MODULES_DIR)/basename/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/basename.wasm

# ---------------------------------------------------------------------------
# WASM dirname module (directory path extraction)
# ---------------------------------------------------------------------------
$(DIRNAME_WASM): $(MODULES_DIR)/dirname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dirname.wasm

# ---------------------------------------------------------------------------
# WASM nl module (line numbering)
# ---------------------------------------------------------------------------
$(NL_WASM): $(MODULES_DIR)/nl/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nl.wasm

# ---------------------------------------------------------------------------
# WASM shuf module (random line shuffling)
# ---------------------------------------------------------------------------
$(SHUF_WASM): $(MODULES_DIR)/shuf/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/shuf.wasm

# ---------------------------------------------------------------------------
# WASM comm module (compare sorted files)
# ---------------------------------------------------------------------------
$(COMM_WASM): $(MODULES_DIR)/comm/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/comm.wasm

# ---------------------------------------------------------------------------
# WASM paste module (merge lines)
# ---------------------------------------------------------------------------
$(PASTE_WASM): $(MODULES_DIR)/paste/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/paste.wasm

# ---------------------------------------------------------------------------
# WASM fold module (line wrapping)
# ---------------------------------------------------------------------------
$(FOLD_WASM): $(MODULES_DIR)/fold/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/fold.wasm

# ---------------------------------------------------------------------------
# WASM tac module (reverse file output)
# ---------------------------------------------------------------------------
$(TAC_WASM): $(MODULES_DIR)/tac/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tac.wasm

# ---------------------------------------------------------------------------
# WASM cksum module (file checksum)
# ---------------------------------------------------------------------------
$(CKSUM_WASM): $(MODULES_DIR)/cksum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cksum.wasm

# ---------------------------------------------------------------------------
# WASM sum module (simple checksum)
# ---------------------------------------------------------------------------
$(SUM_WASM): $(MODULES_DIR)/sum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sum.wasm

# ---------------------------------------------------------------------------
# WASM touch module (create empty files)
# ---------------------------------------------------------------------------
$(TOUCH_WASM): $(MODULES_DIR)/touch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/touch.wasm

# ---------------------------------------------------------------------------
# WASM true module (always succeeds)
# ---------------------------------------------------------------------------
$(TRUE_WASM): $(MODULES_DIR)/true/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/true.wasm

# ---------------------------------------------------------------------------
# WASM false module (always fails)
# ---------------------------------------------------------------------------
$(FALSE_WASM): $(MODULES_DIR)/false/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/false.wasm

# ---------------------------------------------------------------------------
# WASM id module (user identification)
# ---------------------------------------------------------------------------
$(ID_WASM): $(MODULES_DIR)/id/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/id.wasm

# ---------------------------------------------------------------------------
# WASM uptime module (system uptime)
# ---------------------------------------------------------------------------
$(UPTIME_WASM): $(MODULES_DIR)/uptime/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uptime.wasm

# ---------------------------------------------------------------------------
# WASM whoami module (current user display)
# ---------------------------------------------------------------------------
$(WHOAMI_WASM): $(MODULES_DIR)/whoami/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/whoami.wasm

# ---------------------------------------------------------------------------
# WASM uname module (system identification)
# ---------------------------------------------------------------------------
$(UNAME_WASM): $(MODULES_DIR)/uname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uname.wasm

# ---------------------------------------------------------------------------
# WASM df module (disk usage display)
# ---------------------------------------------------------------------------
$(DF_WASM): $(MODULES_DIR)/df/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/df.wasm

# ---------------------------------------------------------------------------
# Full kernel link
# ---------------------------------------------------------------------------
$(TARGET_ELF): $(ALL_OBJS) $(WASM3_LIB) $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(RAMDISK_TAR)
	@echo "  LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

# ---------------------------------------------------------------------------
# Run in QEMU
# ---------------------------------------------------------------------------
run: $(TARGET_ELF)
	@echo "Starting AI-ASM AArch64 v0.5 in QEMU..."
	@echo "Press Ctrl+A then X to exit."
	$(QEMU) \
	    -M virt \
	    -cpu cortex-a53 \
	    -m 128M \
	    -kernel "$<" \
	    -nographic \
	    -no-reboot \
	    -serial mon:stdio

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(KERNEL_DIR)/init.wasm $(KERNEL_DIR)/shell.wasm $(KERNEL_DIR)/test.wasm
	rm -f $(KERNEL_DIR)/editor.wasm $(KERNEL_DIR)/calc.wasm $(KERNEL_DIR)/paint.wasm $(KERNEL_DIR)/launcher.wasm
	rm -f $(KERNEL_DIR)/net_test.wasm $(KERNEL_DIR)/browser.wasm $(KERNEL_DIR)/proc_monitor.wasm $(KERNEL_DIR)/syslog.wasm $(KERNEL_DIR)/filemgr.wasm $(KERNEL_DIR)/settings.wasm $(KERNEL_DIR)/user.wasm $(KERNEL_DIR)/devmgr.wasm $(KERNEL_DIR)/httpd.wasm $(KERNEL_DIR)/dns_resolver.wasm $(KERNEL_DIR)/shmem_test.wasm $(KERNEL_DIR)/sem_test.wasm $(KERNEL_DIR)/grep.wasm $(KERNEL_DIR)/cat.wasm $(KERNEL_DIR)/echo.wasm $(KERNEL_DIR)/tee.wasm $(KERNEL_DIR)/wc.wasm $(KERNEL_DIR)/head.wasm $(KERNEL_DIR)/tail.wasm $(KERNEL_DIR)/sort.wasm $(KERNEL_DIR)/uniq.wasm $(KERNEL_DIR)/tr.wasm $(KERNEL_DIR)/cut.wasm $(KERNEL_DIR)/sed.wasm $(KERNEL_DIR)/xargs.wasm $(KERNEL_DIR)/date.wasm $(KERNEL_DIR)/awk.wasm $(KERNEL_DIR)/ls.wasm $(KERNEL_DIR)/pwd.wasm $(KERNEL_DIR)/env.wasm $(KERNEL_DIR)/printf.wasm $(KERNEL_DIR)/sleep.wasm $(KERNEL_DIR)/yes.wasm $(KERNEL_DIR)/rev.wasm $(KERNEL_DIR)/cmp.wasm $(KERNEL_DIR)/diff.wasm $(KERNEL_DIR)/basename.wasm $(KERNEL_DIR)/dirname.wasm $(KERNEL_DIR)/nl.wasm $(KERNEL_DIR)/shuf.wasm $(KERNEL_DIR)/comm.wasm $(KERNEL_DIR)/paste.wasm $(KERNEL_DIR)/fold.wasm $(KERNEL_DIR)/tac.wasm $(KERNEL_DIR)/cksum.wasm $(KERNEL_DIR)/sum.wasm $(KERNEL_DIR)/touch.wasm $(KERNEL_DIR)/true.wasm $(KERNEL_DIR)/false.wasm $(KERNEL_DIR)/id.wasm $(KERNEL_DIR)/uptime.wasm $(KERNEL_DIR)/whoami.wasm $(KERNEL_DIR)/hostname.wasm $(KERNEL_DIR)/uname.wasm $(KERNEL_DIR)/df.wasm
	rm -f $(KERNEL_DIR)/ramdisk.tar
	rm -f $(MODULES_DIR)/init/init.wasm $(MODULES_DIR)/shell/shell.wasm $(MODULES_DIR)/test/test.wasm
	rm -f $(MODULES_DIR)/editor/editor.wasm $(MODULES_DIR)/calc/calc.wasm $(MODULES_DIR)/paint/paint.wasm $(MODULES_DIR)/launcher/launcher.wasm
	rm -f $(MODULES_DIR)/net_test/net_test.wasm $(MODULES_DIR)/browser/browser.wasm $(MODULES_DIR)/proc_monitor/proc_monitor.wasm $(MODULES_DIR)/syslog/syslog.wasm $(MODULES_DIR)/filemgr/filemgr.wasm $(MODULES_DIR)/settings/settings.wasm $(MODULES_DIR)/user/user.wasm $(MODULES_DIR)/devmgr/devmgr.wasm $(MODULES_DIR)/httpd/httpd.wasm $(MODULES_DIR)/dns_resolver/dns_resolver.wasm $(MODULES_DIR)/shmem_test/shmem_test.wasm $(MODULES_DIR)/sem_test/sem_test.wasm $(MODULES_DIR)/grep/grep.wasm $(MODULES_DIR)/cat/cat.wasm $(MODULES_DIR)/echo/echo.wasm $(MODULES_DIR)/tee/tee.wasm $(MODULES_DIR)/wc/wc.wasm $(MODULES_DIR)/head/head.wasm $(MODULES_DIR)/tail/tail.wasm $(MODULES_DIR)/sort/sort.wasm $(MODULES_DIR)/uniq/uniq.wasm $(MODULES_DIR)/tr/tr.wasm $(MODULES_DIR)/cut/cut.wasm $(MODULES_DIR)/sed/sed.wasm $(MODULES_DIR)/xargs/xargs.wasm $(MODULES_DIR)/date/date.wasm $(MODULES_DIR)/awk/awk.wasm $(MODULES_DIR)/ls/ls.wasm $(MODULES_DIR)/pwd/pwd.wasm $(MODULES_DIR)/env/env.wasm $(MODULES_DIR)/printf/printf.wasm $(MODULES_DIR)/sleep/sleep.wasm $(MODULES_DIR)/yes/yes.wasm $(MODULES_DIR)/rev/rev.wasm $(MODULES_DIR)/cmp/cmp.wasm $(MODULES_DIR)/diff/diff.wasm $(MODULES_DIR)/basename/basename.wasm $(MODULES_DIR)/dirname/dirname.wasm $(MODULES_DIR)/nl/nl.wasm $(MODULES_DIR)/shuf/shuf.wasm $(MODULES_DIR)/comm/comm.wasm $(MODULES_DIR)/paste/paste.wasm $(MODULES_DIR)/fold/fold.wasm $(MODULES_DIR)/tac/tac.wasm $(MODULES_DIR)/cksum/cksum.wasm $(MODULES_DIR)/sum/sum.wasm $(MODULES_DIR)/touch/touch.wasm $(MODULES_DIR)/true/true.wasm $(MODULES_DIR)/false/false.wasm $(MODULES_DIR)/id/id.wasm $(MODULES_DIR)/uptime/uptime.wasm $(MODULES_DIR)/whoami/whoami.wasm $(MODULES_DIR)/df/df.wasm
	@echo "=== Build artifacts removed ==="
