#!/bin/sh
# run_kernel.sh - Start AI-ASM AArch64 kernel in QEMU
# Termux-compatible, no root required
set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
OUTPUT_ELF="$BUILD_DIR/aiasm-aarch64.elf"

if [ ! -f "$OUTPUT_ELF" ]; then
    echo "[run_kernel] Error: Kernel ELF not found at $OUTPUT_ELF"
    echo "  Run './bin/aiasm-build' first."
    exit 1
fi

echo "[run_kernel] Starting QEMU AArch64..."
echo "  Kernel: $OUTPUT_ELF"
echo "  Press Ctrl+A then X to exit QEMU"

qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a57 \
    -kernel "$OUTPUT_ELF" \
    -serial mon:stdio \
    -display none \
    -nographic \
    -no-reboot \
    -m 128M \
    "$@"
