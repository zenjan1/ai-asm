# AI-ASM AArch64 Makefile
# For Termux AArch64 environment
# Tools: binutils (as, ld, objcopy), qemu-system-aarch64

AS      = as
LD      = ld
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64
ASFLAGS = -mcpu=generic

KERNEL_DIR  = kernel
BUILD_DIR   = build
OUTPUT_ELF  = $(BUILD_DIR)/aiasm-aarch64.elf
OUTPUT_BIN  = $(BUILD_DIR)/aiasm-aarch64.bin
OUTPUT_RAW  = $(BUILD_DIR)/aiasm-aarch64.raw

ASM_SRCS = $(wildcard $(KERNEL_DIR)/*.asm)
OBJS     = $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))

.PHONY: all clean test run

all: $(OUTPUT_BIN) $(OUTPUT_RAW)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	@echo "  AS  $<"
	$(AS) $(ASFLAGS) -o $@ $<

$(OUTPUT_ELF): $(OBJS)
	@echo "  LD  $@"
	$(LD) -T $(KERNEL_DIR)/linker.ld -o $@ $(OBJS)
	@cp $@ $(OUTPUT_BIN)

$(OUTPUT_BIN): $(OUTPUT_ELF)
	@echo "  ELF:  $@ ($$(wc -c < $@) bytes)"

$(OUTPUT_RAW): $(OUTPUT_ELF)
	@echo "  OBJCOPY  $@"
	$(OBJCOPY) -O binary $< $@
	@echo "  RAW:  $@ ($$(wc -c < $@) bytes)"

test: $(OUTPUT_BIN)
	@echo "Running tests..."
	./bin/aiasm-test

run: $(OUTPUT_ELF)
	@echo "Starting QEMU..."
	./run_kernel.sh

clean:
	rm -rf $(BUILD_DIR)
