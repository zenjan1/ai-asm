# AI-ASM AArch64 v0.4 — Makefile for Termux
# Toolchain: native binutils (as, ld), clang for C/WASM
#
# Targets:
#   make all        — build full kernel.elf
#   make asm        — compile assembly only
#   make wasm3      — compile wasm3 library only
#   make modules    — compile WASM shell module only
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
           $(KERNEL_DIR)/serial_rx.asm \
           $(KERNEL_DIR)/wasm_embed.asm
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

# WASM shell module
SHELL_WASM = $(MODULES_DIR)/shell/shell.wasm

# All object dependencies
ALL_OBJS = $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)

# ---------------------------------------------------------------------------
# Phony targets
# ---------------------------------------------------------------------------
.PHONY: all asm wasm3 modules kernel run clean

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

# Compile WASM shell module only
modules: $(SHELL_WASM)
	@echo "=== WASM shell module complete ==="

# Link kernel only (assumes objects exist)
kernel: $(TARGET_ELF)
	@echo "=== kernel.elf linked ==="

# ---------------------------------------------------------------------------
# Assembly compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	@echo "  AS    $<"
	$(AS) $(ASFLAGS) -o $@ $<

# wasm_embed.o depends on shell.wasm being present first
$(BUILD_DIR)/wasm_embed.o: $(SHELL_WASM)

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
	$(CLANG) $(CFLAGS_FREESTANDING) -O0 -o $@ $<

$(WASM3_LIB): $(WASM3_OBJS)
	@echo "  AR    $@"
	ar rcs $@ $^

# ---------------------------------------------------------------------------
# WASM shell module
# ---------------------------------------------------------------------------
$(SHELL_WASM): $(MODULES_DIR)/shell/src/main.c
	@echo "  WASM  $<"
	$(CLANG) $(WASM_FLAGS) -o $@ $<
	cp $@ $(KERNEL_DIR)/shell.wasm

# ---------------------------------------------------------------------------
# Full kernel link
# ---------------------------------------------------------------------------
$(TARGET_ELF): $(ALL_OBJS) $(WASM3_LIB) $(SHELL_WASM)
	@echo "  LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

# ---------------------------------------------------------------------------
# Run in QEMU
# ---------------------------------------------------------------------------
run: $(TARGET_ELF)
	@echo "Starting AI-ASM AArch64 v0.4 in QEMU..."
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
	rm -f $(KERNEL_DIR)/shell.wasm
	rm -f $(MODULES_DIR)/shell/shell.wasm
	@echo "=== Build artifacts removed ==="
