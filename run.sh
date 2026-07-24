#!/bin/sh
# AI-ASM AArch64 - Quick boot script
# Runs the Linux-hosted WASM runtime directly (no proot needed)
#
# Usage: ./run.sh [--boot MODULE]

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
RUNTIME="${SCRIPT_DIR}/linux_runtime"
MODULES_DIR="${SCRIPT_DIR}/modules"
BOOT_MODULE="init"

# Parse arguments
while [ $# -gt 0 ]; do
    case "$1" in
        --boot) BOOT_MODULE="$2"; shift 2;;
        --modules-dir) MODULES_DIR="$2"; shift 2;;
        *) echo "Unknown option: $1"; exit 1;;
    esac
done

# Check runtime exists, build if needed
if [ ! -x "$RUNTIME" ]; then
    echo "Runtime not found. Building via 'make linux_runtime'..."
    cd "$SCRIPT_DIR" && make linux_runtime
fi

echo "=== AI-ASM AArch64 ==="
echo "Boot module: ${BOOT_MODULE}"
echo "Modules: ${MODULES_DIR}"
echo ""

exec "$RUNTIME" --modules-dir="${MODULES_DIR}" --boot="${BOOT_MODULE}"
