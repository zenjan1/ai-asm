#!/bin/bash
# AI-ASM AArch64 v1.0 - Quick Launch Script
# Usage: ./run.sh
#
# Starts the kernel in QEMU with serial console.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
KERNEL="$SCRIPT_DIR/kernel.elf"
QEMU="qemu-system-aarch64"

if [ ! -f "$KERNEL" ]; then
    echo "Error: kernel.elf not found in $(dirname "$0")"
    exit 1
fi

echo "Starting AI-ASM AArch64 v1.0..."
echo "Press Ctrl+A then X to exit QEMU."

$QEMU \
    -M virt \
    -cpu cortex-a57 \
    -m 256M \
    -kernel "$KERNEL" \
    -nographic \
    -serial mon:stdio \
    -no-reboot \
    -display none
