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
#   make linux_runtime — build Linux-hosted WASM runtime
#   make run_linux  — run under Linux (Termux/proot)
#   make x86_64     — build x86_64 kernel
#   make clean      — remove all build artifacts
#
# WASM modules are auto-detected from modules/*/src/main.c
# Adding a new module: just create modules/<name>/src/main.c

AS      = as
LD      = ld
CLANG   = clang
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64

BOOT_MODULE ?= init

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
# Assembly sources
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

# x86_64 sources
X86_ASM_SRCS = $(KERNEL_DIR)/x86_64/boot.asm \
               $(KERNEL_DIR)/x86_64/idt.asm \
               $(KERNEL_DIR)/x86_64/pic.asm \
               $(KERNEL_DIR)/x86_64/serial.asm
X86_ASM_OBJS = $(patsubst $(KERNEL_DIR)/x86_64/%.asm,$(BUILD_DIR)/x86_64/%.o,$(X86_ASM_SRCS))
X86_TARGET = $(BUILD_DIR)/kernel_x86_64.elf

# C sources (freestanding)
C_SRCS = $(KERNEL_DIR)/libc_shim.c \
         $(KERNEL_DIR)/wasm_host.c
C_OBJS = $(BUILD_DIR)/libc_shim.o $(BUILD_DIR)/wasm_host.o

# wasm3 sources
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

# ---------------------------------------------------------------------------
# WASM modules — auto-detected from modules/*/src/main.c
# ---------------------------------------------------------------------------
WASM_MODULE_DIRS := $(sort $(dir $(wildcard $(MODULES_DIR)/*/src/main.c)))
WASM_OUTPUTS := $(foreach mod,$(WASM_MODULE_DIRS),$(patsubst %/src/,%,$(mod))/$(notdir $(patsubst %/,%,$(patsubst %/src/,%,$(mod)))).wasm)

# Well-known module paths (used for dependency ordering)
INIT_WASM       = $(MODULES_DIR)/init/init.wasm
SHELL_WASM      = $(MODULES_DIR)/shell/shell.wasm
TEST_WASM       = $(MODULES_DIR)/test/test.wasm
EDITOR_WASM     = $(MODULES_DIR)/editor/editor.wasm
CALC_WASM       = $(MODULES_DIR)/calc/calc.wasm
PAINT_WASM      = $(MODULES_DIR)/paint/paint.wasm
LAUNCHER_WASM   = $(MODULES_DIR)/launcher/launcher.wasm
NET_TEST_WASM   = $(MODULES_DIR)/net_test/net_test.wasm
PROC_MONITOR_WASM = $(MODULES_DIR)/proc_monitor/proc_monitor.wasm
SYSLOG_WASM     = $(MODULES_DIR)/syslog/syslog.wasm
FILEMGR_WASM    = $(MODULES_DIR)/filemgr/filemgr.wasm
SETTINGS_WASM   = $(MODULES_DIR)/settings/settings.wasm
USER_WASM       = $(MODULES_DIR)/user/user.wasm
DEVMGR_WASM     = $(MODULES_DIR)/devmgr/devmgr.wasm
BROWSER_WASM    = $(MODULES_DIR)/browser/browser.wasm
HTTPD_WASM      = $(MODULES_DIR)/httpd/httpd.wasm
DNS_RESOLVER_WASM = $(MODULES_DIR)/dns_resolver/dns_resolver.wasm
SHMEM_TEST_WASM = $(MODULES_DIR)/shmem_test/shmem_test.wasm
SEM_TEST_WASM   = $(MODULES_DIR)/sem_test/sem_test.wasm

RAMDISK_TAR = $(KERNEL_DIR)/ramdisk.tar

ALL_OBJS = $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)

# ---------------------------------------------------------------------------
# Phony targets
# ---------------------------------------------------------------------------
.PHONY: all asm wasm3 modules kernel run clean x86_64 linux_runtime run_linux proot_setup

all: $(TARGET_ELF)

$(BUILD_DIR) $(BUILD_DIR)/wasm3:
	mkdir -p $@

# ---------------------------------------------------------------------------
# Layered build targets
# ---------------------------------------------------------------------------
asm: $(ASM_OBJS)
	@echo "=== Assembly compilation complete ==="

wasm3: $(WASM3_LIB)
	@echo "=== wasm3 library complete ==="

modules: $(WASM_OUTPUTS) $(RAMDISK_TAR)
	@echo "=== WASM modules and ramdisk complete ($(words $(WASM_OUTPUTS)) modules) ==="

kernel: $(TARGET_ELF)
	@echo "=== kernel.elf linked ==="

# ---------------------------------------------------------------------------
# Assembly compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	@echo "  AS    $<"
	$(AS) $(ASFLAGS) -o $@ $<

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
# WASM module build rules (auto-generated per module)
# ---------------------------------------------------------------------------
define WASM_MODULE_RULE
$(1)/$(notdir $(1)).wasm: $(1)/src/main.c
	@echo "  WASM  $$<"
	$(CLANG) $(WASM_FLAGS) -o $$@ $$<
endef
$(foreach mod,$(WASM_MODULE_DIRS),$(eval $(call WASM_MODULE_RULE,$(patsubst %/src/,%,$(mod)))))

# ---------------------------------------------------------------------------
# RAM disk
# ---------------------------------------------------------------------------
$(RAMDISK_TAR): ramdisk/motd.txt ramdisk/hello.txt ramdisk/readme.txt
	@echo "  TAR   $@"
	tar cf $@ -C ramdisk motd.txt hello.txt readme.txt

# ---------------------------------------------------------------------------
# Dependency ordering: wasm_embed.o needs shell/proc_monitor/syslog
# ---------------------------------------------------------------------------
$(BUILD_DIR)/wasm_embed.o: $(SHELL_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM)

# ramdisk.o needs all WASM modules built first
$(BUILD_DIR)/ramdisk.o: $(WASM_OUTPUTS) $(RAMDISK_TAR)

# ---------------------------------------------------------------------------
# x86_64 kernel target
# ---------------------------------------------------------------------------
x86_64: $(X86_TARGET)
	@echo "=== kernel_x86_64.elf linked ==="

$(BUILD_DIR)/x86_64:
	mkdir -p $@

$(BUILD_DIR)/x86_64/%.o: $(KERNEL_DIR)/x86_64/%.asm | $(BUILD_DIR)/x86_64
	@echo "  AS    $<"
	clang --target=x86_64-unknown-linux-gnu -c -g -o $@ $<

$(X86_TARGET): $(X86_ASM_OBJS) | $(BUILD_DIR)
	@echo "  LD    $@"
	ld -T kernel/linker_x86_64.ld -nostdlib -N -o $@ $(X86_ASM_OBJS)
	@echo "kernel_x86_64.elf: $$(stat -c%s $@) bytes ($$(( $$(stat -c%s $@) / 1024 ))KB)"

# ---------------------------------------------------------------------------
# Full kernel link
# ---------------------------------------------------------------------------
$(TARGET_ELF): $(ALL_OBJS) $(WASM3_LIB) $(WASM_OUTPUTS) $(RAMDISK_TAR)
	@echo "  LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

# ---------------------------------------------------------------------------
# QEMU run
# ---------------------------------------------------------------------------
run: $(TARGET_ELF)
	$(QEMU) -M virt,highmem=off -cpu cortex-a53 -m 256M \
	    -kernel $(TARGET_ELF) \
	    -nographic \
	    -no-reboot \
	    -serial mon:stdio

# ---------------------------------------------------------------------------
# Linux-hosted WASM runtime (proot/Termux native)
# ---------------------------------------------------------------------------
linux_runtime: linux_runtime.c kernel/wasm3/source/*.c kernel/wasm3/source/*.h kernel/module_runtime.h
	@echo "Building Linux-hosted WASM runtime..."
	$(CLANG) -o $@ $< kernel/wasm3/source/*.c \
	    -Ikernel/wasm3/source -Ikernel \
	    -Dd_m3FixedHeap=67108864 \
	    -Dd_m3VerboseErrorMessages=0 -Dd_m3HasFloat=0 \
	    -Dd_m3NoFloatDynamic=1 -Dd_m3HasTracer=0 \
	    -Dd_m3RecordBacktraces=0 -Dd_m3LogTimestamps=0 \
	    -O2
	@echo "Built: $@"

proot_setup: linux_runtime
	@echo "Setting up proot rootfs..."
	mkdir -p rootfs/usr/bin
	cp linux_runtime rootfs/usr/bin/aiasm
	@echo "proot rootfs ready"

run_linux: linux_runtime
	@echo "Starting AI-ASM AArch64 Linux Runtime..."
	./run.sh --boot $(BOOT_MODULE)

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(KERNEL_DIR)/*.wasm
	rm -f $(KERNEL_DIR)/ramdisk.tar
	find $(MODULES_DIR) -name '*.wasm' -delete
	rm -f linux_runtime
	@echo "=== Build artifacts removed ==="
