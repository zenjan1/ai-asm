#!/bin/bash
# AI-ASM AArch64 v1.0 Release Script
# Usage: ./release.sh [version]
# Default version: v1.0

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

VERSION="${1:-v1.0}"
RELEASE_DIR="release/$VERSION"

echo "=== AI-ASM AArch64 Release $VERSION ==="

# Check build artifacts
if [ ! -f "build/kernel.elf" ]; then
    echo "Error: build/kernel.elf not found. Run 'make' first."
    exit 1
fi

# Create release directory structure
echo "Creating release directory structure..."
mkdir -p "$RELEASE_DIR/src"
mkdir -p "$RELEASE_DIR/modules"
mkdir -p "$RELEASE_DIR/ramdisk"

# Copy kernel binary
echo "  Copying kernel.elf..."
cp build/kernel.elf "$RELEASE_DIR/"

# Copy kernel source
echo "  Copying kernel sources..."
cp kernel/*.asm "$RELEASE_DIR/src/"
cp kernel/*.c "$RELEASE_DIR/src/"
cp kernel/*.h "$RELEASE_DIR/src/" 2>/dev/null || true
cp kernel/*.ld "$RELEASE_DIR/src/"

# Copy wasm3 source (only needed headers and source files)
echo "  Copying wasm3 sources..."
mkdir -p "$RELEASE_DIR/src/wasm3/source"
cp kernel/wasm3/source/*.c "$RELEASE_DIR/src/wasm3/source/"
cp kernel/wasm3/source/*.h "$RELEASE_DIR/src/wasm3/source/"

# Copy WASM module sources
echo "  Copying module sources..."
for mod in init shell test; do
    if [ -d "modules/$mod/src" ]; then
        mkdir -p "$RELEASE_DIR/modules/$mod/src"
        cp modules/$mod/src/*.c "$RELEASE_DIR/modules/$mod/src/"
    fi
done

# Copy ramdisk contents
echo "  Copying ramdisk files..."
cp ramdisk/*.txt "$RELEASE_DIR/ramdisk/" 2>/dev/null || true

# Copy build system and docs
echo "  Copying Makefile and docs..."
cp Makefile "$RELEASE_DIR/"
cp README.md "$RELEASE_DIR/"
cp CHANGELOG.md "$RELEASE_DIR/"
cp ROADMAP.md "$RELEASE_DIR/"
cp run_test.sh "$RELEASE_DIR/" 2>/dev/null || true

# Create tarball
echo "  Creating tarball..."
cd release
tar -czf "aiasm-$VERSION.tar.gz" "$VERSION/"
cd "$SCRIPT_DIR"

# Show stats
SIZE=$(wc -c < "release/aiasm-$VERSION.tar.gz")
KERNEL_SIZE=$(wc -c < "build/kernel.elf")
ASM_FILES=$(ls kernel/*.asm | wc -l)
C_LINES=$(cat kernel/wasm_host.c kernel/libc_shim.c | wc -l)
MODULE_FILES=$(find modules -name '*.c' | wc -l)

echo ""
echo "=== Release $VERSION Summary ==="
echo "  Kernel ELF:   $KERNEL_SIZE bytes"
echo "  Tarball:      release/aiasm-$VERSION.tar.gz ($SIZE bytes)"
echo "  ASM files:    $ASM_FILES"
echo "  C lines:      $C_LINES"
echo "  WASM modules: $MODULE_FILES"
echo ""
echo "Release $VERSION created successfully."
