#!/bin/sh
# AI-ASM AArch64 Boot Script
# Run AI-ASM OS inside proot environment
#
# Usage: ./boot.sh [--boot MODULE] [--modules-dir DIR]

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOTFS="${SCRIPT_DIR}/rootfs"
RUNTIME="${ROOTFS}/usr/bin/aiasm"
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
    echo "Runtime not found. Building via 'make proot_setup'..."
    cd "$SCRIPT_DIR" && make proot_setup
fi

# Setup modules in rootfs (bind mount instead of symlinks)
echo "Using modules from ${MODULES_DIR} (bind mount)"

# Create fstab for proot
cat > "${ROOTFS}/etc/fstab" << EOF
# AI-ASM proot filesystem
proc  /proc   proc  rw  0 0
sysfs /sys    sysfs rw  0 0
EOF

echo ""
echo "=== AI-ASM AArch64 Proot Boot ==="
echo "Rootfs: ${ROOTFS}"
echo "Boot module: ${BOOT_MODULE}"
echo "Modules dir: ${MODULES_DIR}"
echo ""

# Run under proot
exec proot \
    --rootfs="${ROOTFS}" \
    --bind="${MODULES_DIR}:/usr/share/aiasm/modules" \
    --bind=/dev \
    --bind=/proc \
    --bind=/sys \
    --kill-on-exit \
    /usr/bin/aiasm --modules-dir=/usr/share/aiasm/modules --boot="${BOOT_MODULE}"
