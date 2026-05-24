#!/bin/bash
# AI-ASM AArch64 Integration Test Runner
# Usage: ./run_test.sh
#
# Spins up QEMU with the test module loaded and captures serial output.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
QEMU="qemu-system-aarch64"
KERNEL="$SCRIPT_DIR/build/kernel.elf"
DISK_IMG="$SCRIPT_DIR/disk.img"
LOG="$SCRIPT_DIR/test_output.log"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== AI-ASM AArch64 Integration Test ===${NC}"

# Check kernel exists
if [ ! -f "$KERNEL" ]; then
    echo -e "${RED}Error: $KERNEL not found. Run 'make' first.${NC}"
    exit 1
fi

# Create 64MB disk image if not exists
if [ ! -f "$DISK_IMG" ]; then
    echo -e "${YELLOW}Creating 64MB test disk image...${NC}"
    dd if=/dev/zero of="$DISK_IMG" bs=1M count=64 2>/dev/null
fi

# Create log file
> "$LOG"

echo -e "${YELLOW}Starting QEMU with test module...${NC}"
echo -e "${YELLOW}(Timeout: 30 seconds)${NC}"

# Run QEMU with 30-second timeout
timeout 30 $QEMU \
    -M virt \
    -cpu cortex-a53 \
    -m 128M \
    -kernel "$KERNEL" \
    -drive file="$DISK_IMG",format=raw,if=virtio \
    -nographic \
    -no-reboot \
    -serial file:"$LOG" \
    -display none 2>&1 || true

# Show results
if [ -f "$LOG" ] && [ -s "$LOG" ]; then
    echo -e "${GREEN}=== Test Output ===${NC}"
    cat "$LOG"

    # Check for pass/fail
    if grep -q "All tests passed" "$LOG"; then
        echo -e "\n${GREEN}=== ALL TESTS PASSED ===${NC}"
        exit 0
    elif grep -q "SOME TESTS FAILED" "$LOG"; then
        echo -e "\n${RED}=== SOME TESTS FAILED ===${NC}"
        exit 1
    else
        echo -e "\n${YELLOW}=== Test output incomplete (may need longer timeout) ===${NC}"
        exit 2
    fi
else
    echo -e "${RED}No serial output captured. QEMU may not have started.${NC}"
    exit 3
fi
