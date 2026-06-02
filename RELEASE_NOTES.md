# AI-ASM OS v28.0 Release Notes

## Overview

AI-ASM OS v28.0 is the first production-ready release of the WASM-native
operating system for AArch64 architectures. It features a three-layer
architecture with 62 kernel modules, a WASM runtime with 372 host functions,
and 410 application modules.

## Key Features

- **Complete Kernel**: 62 AArch64 assembly modules covering process management,
  memory management, filesystems, networking, IPC, GUI, security, and hardware
  support for QEMU, Raspberry Pi 4, Apple Silicon, NVIDIA Jetson, neuromorphic,
  and photonic computing platforms.

- **WASM Runtime**: Full wasm3-based runtime with WASI snapshot_preview1
  compliance, JIT compilation, module caching, and 372 host functions spanning
  core, filesystem, network, IPC, performance, maintenance, quantum,
  neuromorphic, photonic, and ethical/AGI interfaces.

- **Application Layer**: 410 WASM modules providing 80+ POSIX tools, network
  utilities, GUI applications, AI interfaces, Agent system, AGI reasoning,
  quantum simulation, spiking neural networks, and advanced computing interfaces.

## System Requirements

### Minimum
- AArch64 processor (or QEMU emulator)
- 2GB RAM
- 512MB storage

### Recommended
- Raspberry Pi 4 (4GB+) or Apple Silicon Mac
- 4GB RAM
- 2GB storage

## Known Limitations

- Apple Silicon, Jetson, neuromorphic, and photonic support is interface-layer
  only (requires hardware-specific drivers)
- JIT compilation covers 14+ opcodes (not full WASM spec)

## Upgrade Path

From v27.x: Full binary compatibility maintained. No migration required.

## Security

- Permission system with 4 levels (root/admin/user/guest)
- RSA-2048 signature verification for updates
- SHA-256 checksum verification
- Audit logging
