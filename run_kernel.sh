#!/data/data/com.termux/files/usr/bin/bash
# run_kernel.sh - Launch AI-ASM AArch64 in QEMU
set -e

QEMU=qemu-system-aarch64
KERNEL=build/kernel.elf

if [ ! -f "$KERNEL" ]; then
    echo "Error: $KERNEL not found. Run 'make' first."
    exit 1
fi

echo "Starting AI-ASM AArch64 v0.4 in QEMU..."
echo "Press Ctrl+A then X to exit."

$QEMU \
    -M virt \
    -cpu cortex-a53 \
    -m 128M \
    -kernel "$KERNEL" \
    -nographic \
    -no-reboot \
    -serial mon:stdio
