# AI-ASM AArch64 v0.3 — Makefile for Termux
# Toolchain: native binutils (as, ld), clang for C/WASM

AS      = as
LD      = ld
CLANG   = clang
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64

ASFLAGS = -march=armv8-a -g
LDFLAGS = -T kernel/linker.ld -nostdlib -N
CFLAGS_FREESTANDING = -c -O2 -ffreestanding -Ikernel/wasm3/source \
    -mgeneral-regs-only \
    -Dd_m3FixedHeap=262144 -Dd_m3VerboseErrorMessages=0 \
    -Dd_m3HasFloat=0 -Dd_m3NoFloatDynamic=1 \
    -Dd_m3HasTracer=0 -Dd_m3RecordBacktraces=0 -Dd_m3LogTimestamps=0
WASM_FLAGS = --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
    -Wl,--no-entry -Wl,--export=shell_entry

KERNEL_DIR = kernel
BUILD_DIR  = build
WASM3_DIR  = $(KERNEL_DIR)/wasm3
MODULES_DIR = modules
TARGET_ELF = $(BUILD_DIR)/kernel.elf

# Assembly sources
ASM_SRCS = $(KERNEL_DIR)/kernel.asm \
           $(KERNEL_DIR)/pl011.asm \
           $(KERNEL_DIR)/log.asm \
           $(KERNEL_DIR)/event.asm \
           $(KERNEL_DIR)/utils.asm \
           $(KERNEL_DIR)/memory.asm \
           $(KERNEL_DIR)/timer.asm \
           $(KERNEL_DIR)/gic.asm \
           $(KERNEL_DIR)/wasm_embed.asm

ASM_OBJS = $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))

# C sources (freestanding)
C_SRCS = $(KERNEL_DIR)/libc_shim.c \
         $(KERNEL_DIR)/wasm_host.c

C_OBJS = $(BUILD_DIR)/libc_shim.o $(BUILD_DIR)/wasm_host.o

# wasm3 sources (exclude tracer/wasi files we don't need)
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

.PHONY: all wasm3 modules kernel run clean

all: $(TARGET_ELF)

$(BUILD_DIR) $(BUILD_DIR)/wasm3:
	mkdir -p $@

# ---- Assembly compilation ----
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# wasm_embed.o depends on shell.wasm being present first
$(BUILD_DIR)/wasm_embed.o: $(SHELL_WASM)

# ---- C freestanding compilation ----
$(BUILD_DIR)/libc_shim.o: $(KERNEL_DIR)/libc_shim.c | $(BUILD_DIR)
	$(CLANG) $(CFLAGS_FREESTANDING) -o $@ $<

$(BUILD_DIR)/wasm_host.o: $(KERNEL_DIR)/wasm_host.c | $(BUILD_DIR)
	$(CLANG) $(CFLAGS_FREESTANDING) -o $@ $<

# ---- wasm3 compilation ----
$(BUILD_DIR)/wasm3/%.o: $(WASM3_DIR)/source/%.c | $(BUILD_DIR)/wasm3
	$(CLANG) $(CFLAGS_FREESTANDING) -o $@ $<

$(WASM3_LIB): $(WASM3_OBJS)
	ar rcs $@ $^

# ---- WASM shell module ----
$(SHELL_WASM): $(MODULES_DIR)/shell/src/main.c
	$(CLANG) $(WASM_FLAGS) -o $@ $<
	cp $@ $(KERNEL_DIR)/shell.wasm

# ---- Full kernel link ----
$(TARGET_ELF): $(ALL_OBJS) $(WASM3_LIB) $(SHELL_WASM)
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

run: $(TARGET_ELF)
	./run_kernel.sh

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(KERNEL_DIR)/shell.wasm
	rm -rf $(MODULES_DIR)/shell/shell.wasm
