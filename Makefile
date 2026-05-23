# AI-ASM AArch64 - Makefile for Termux
# Toolchain: native binutils (as, ld) on AArch64

AS      = as
LD      = ld
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64
ASFLAGS = -march=armv8-a -g
LDFLAGS = -T kernel/linker.ld -nostdlib

KERNEL_DIR = kernel
BUILD_DIR  = build
TARGET_ELF = $(BUILD_DIR)/kernel.elf

SRCS = $(KERNEL_DIR)/kernel.asm \
       $(KERNEL_DIR)/pl011.asm \
       $(KERNEL_DIR)/log.asm \
       $(KERNEL_DIR)/event.asm \
       $(KERNEL_DIR)/utils.asm \
       $(KERNEL_DIR)/shell.asm \
       $(KERNEL_DIR)/memory.asm \
       $(KERNEL_DIR)/timer.asm \
       $(KERNEL_DIR)/gic.asm \
       $(KERNEL_DIR)/process.asm \
       $(KERNEL_DIR)/syscall.asm

OBJS = $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all run clean

all: $(TARGET_ELF)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

$(TARGET_ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

run: $(TARGET_ELF)
	./run_kernel.sh

clean:
	rm -rf $(BUILD_DIR)
