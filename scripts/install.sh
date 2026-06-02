#!/bin/sh
# AI-ASM OS v28.0 - Install Script
# Detects platform and installs appropriate package

set -e

VERSION="28.0"
PACKAGE="aiasm-v${VERSION}"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log_info()  { echo "${GREEN}[INFO]${NC} $1"; }
log_warn()  { echo "${YELLOW}[WARN]${NC} $1"; }
log_error() { echo "${RED}[ERROR]${NC} $1"; }

# Platform detection
detect_platform() {
    ARCH=$(uname -m 2>/dev/null || echo "unknown")
    OS=$(uname -s 2>/dev/null || echo "unknown")

    case "$ARCH" in
        aarch64|arm64)
            if [ -f /proc/device-tree/model ]; then
                MODEL=$(cat /proc/device-tree/model 2>/dev/null)
                case "$MODEL" in
                    *Raspberry*Pi*4*) echo "rpi4"; return ;;
                esac
            fi
            echo "qemu-virt-aarch64"
            ;;
        x86_64)
            echo "qemu-x86"
            ;;
        *)
            echo "$ARCH"
            ;;
    esac
}

# Dependency check
check_deps() {
    log_info "Checking dependencies..."
    MISSING=""

    if ! command -v tar >/dev/null 2>&1; then
        MISSING="$MISSING tar"
    fi
    if ! command -v gzip >/dev/null 2>&1; then
        MISSING="$MISSING gzip"
    fi

    if [ -n "$MISSING" ]; then
        log_error "Missing dependencies:$MISSING"
        return 1
    fi

    log_info "All dependencies satisfied"
}

# Install for QEMU
install_qemu() {
    log_info "Installing for QEMU AArch64..."

    INSTALL_DIR="${INSTALL_DIR:-/opt/aiasm}"
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$INSTALL_DIR/kernel"
    mkdir -p "$INSTALL_DIR/modules"
    mkdir -p "$INSTALL_DIR/etc"

    cp -r kernel/*.asm "$INSTALL_DIR/kernel/" 2>/dev/null || true
    cp kernel/wasm_host.c "$INSTALL_DIR/kernel/" 2>/dev/null || true

    # Create default config
    cat > "$INSTALL_DIR/etc/aiasm.conf" <<'EOF'
# AI-ASM OS Configuration
PLATFORM=qemu-virt-aarch64
MEMORY=2G
CPU=cortex-a72
LOG_LEVEL=info
EOF

    log_info "QEMU installation complete at $INSTALL_DIR"
}

# Install for Raspberry Pi 4
install_rpi4() {
    log_info "Installing for Raspberry Pi 4..."

    INSTALL_DIR="${INSTALL_DIR:-/opt/aiasm}"
    mkdir -p "$INSTALL_DIR/kernel"
    mkdir -p "$INSTALL_DIR/modules"
    mkdir -p "$INSTALL_DIR/etc"
    mkdir -p "$INSTALL_DIR/boot"

    cp -r kernel/*.asm "$INSTALL_DIR/kernel/" 2>/dev/null || true
    cp kernel/wasm_host.c "$INSTALL_DIR/kernel/" 2>/dev/null || true

    # Create boot config
    cat > "$INSTALL_DIR/boot/config.txt" <<'EOF'
# AI-ASM OS Boot Config
arm_64bit=1
kernel=kernel.img
gpu_mem=16
EOF

    # Create system config
    cat > "$INSTALL_DIR/etc/aiasm.conf" <<'EOF'
PLATFORM=raspberry-pi4
MEMORY=4G
LOG_LEVEL=info
GPIO_ENABLED=1
UART_ENABLED=1
EOF

    log_info "Raspberry Pi 4 installation complete at $INSTALL_DIR"
}

# Post-install
post_install() {
    log_info "Running post-installation..."

    # Verify kernel files
    KERNEL_COUNT=$(ls "$INSTALL_DIR/kernel/"*.asm 2>/dev/null | wc -l)
    log_info "Kernel modules: $KERNEL_COUNT"

    # Verify WASM runtime
    if [ -f "$INSTALL_DIR/kernel/wasm_host.c" ]; then
        LINES=$(wc -l < "$INSTALL_DIR/kernel/wasm_host.c")
        log_info "WASM runtime: $LINES lines"
    fi

    # Set permissions
    chmod 755 "$INSTALL_DIR" 2>/dev/null || true
    chmod 644 "$INSTALL_DIR/kernel/"*.asm 2>/dev/null || true

    log_info "Installation complete!"
    log_info "Run: aiasm --help for usage"
}

# Main
main() {
    echo "========================================"
    echo "  AI-ASM OS v${VERSION} Installer"
    echo "========================================"

    PLATFORM=$(detect_platform)
    log_info "Detected platform: $PLATFORM"

    check_deps || exit 1

    case "$PLATFORM" in
        rpi4)
            install_rpi4
            ;;
        qemu-virt-aarch64|qemu-x86)
            install_qemu
            ;;
        *)
            log_warn "Unknown platform: $PLATFORM"
            log_warn "Installing with default settings..."
            install_qemu
            ;;
    esac

    post_install
}

main "$@"
