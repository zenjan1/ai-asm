# AI-ASM AArch64

The world's first AArch64 AI-native operating system implemented in 100% pure assembly language.
Designed for development and execution on Android Termux mobile devices.

## Features

- **100% Pure AArch64 Assembly**: No C, no libc, no external dependencies
- **AI-ASM Format**: All functions include structured AI-readable comments
- **JSON Structured Logging**: All kernel output is machine-readable JSON
- **Event Bus**: Full kernel observability through structured events
- **Termux Native**: Runs on Termux with official `binutils` and `qemu-system-aarch64`
- **Minimal Footprint**: Kernel < 128KB, build time < 10 seconds, raw binary 17KB

## Prerequisites

Install Termux packages:

```bash
pkg update && pkg upgrade -y
pkg install binutils qemu-system-aarch64 make git
```

## Quick Start

```bash
# Build
make
# or: ./bin/aiasm-build

# Run in QEMU
make run
# or: ./run_kernel.sh

# Run tests
make test
# or: ./bin/aiasm-test

# Clean
make clean
```

## Architecture

### Boot Flow

1. `_start` - Entry point at 0x40080000 (QEMU virt load address)
2. Exception masking (DAIF set)
3. EL2/EL3 to EL1 transition (if needed)
4. Stack setup (8KB at __stack_top)
5. MMU and cache disable
6. BSS zeroing
7. GICv2 interrupt controller init
8. PL011 UART init (0x09000000, 115200 8N1)
9. ARM Generic Timer init
10. Event bus init
11. Structured JSON log init
12. VGA framebuffer init
13. Boot event published as JSON
14. Shell interactive loop

### Memory Map (QEMU virt)

| Address       | Component              |
|---------------|------------------------|
| 0x40080000    | Kernel load address    |
| 0x09000000    | PL011 UART             |
| 0x08000000    | GICv2 Distributor      |
| 0x08010000    | GICv2 CPU Interface    |
| 0x40100000    | Framebuffer            |

### Build Artifacts

| File | Size | Description |
|------|------|-------------|
| `aiasm-aarch64.elf` | ~88KB | ELF executable with debug info |
| `aiasm-aarch64.bin` | ~88KB | Copy of ELF for QEMU -kernel |
| `aiasm-aarch64.raw` | ~17KB | Raw binary (no BSS/headers) |

## Shell Commands

| Command  | Description                    |
|----------|--------------------------------|
| `help`   | Show available commands        |
| `version`| Print kernel version           |
| `clear`  | Clear VGA screen               |
| `log`    | Emit test JSON log event       |

## Log Output Format

All kernel output is single-line JSON:

```json
{"timestamp":0,"level":"INFO","event":"boot","data":{"version":"0.1-aarch64","arch":"aarch64"}}
```

## AI-ASM Format

Every function follows this header template:

```
; -----------------------------------------------------------------------------
; Function: name
; Description: what it does
; Input: x0 = ..., w1 = ...
; Output: x0 = ..., never returns, etc.
; Clobbered registers: x0, x1, ...
; Stack: N bytes
; -----------------------------------------------------------------------------
```

## Project Structure

```
aiasm-aarch64/
├── bin/
│   ├── aiasm-build         # Build the kernel
│   ├── aiasm-test          # Run automated tests
│   └── aiasm-new           # Create new project from template
├── kernel/
│   ├── kernel.asm          # Entry point, EL1 init, boot sequence
│   ├── idt.asm             # Exception vector table
│   ├── gic.asm             # GICv2 interrupt controller
│   ├── timer.asm           # ARM Generic Timer
│   ├── pl011.asm           # PL011 UART driver
│   ├── vga.asm             # Framebuffer text driver
│   ├── event.asm           # Kernel event bus
│   ├── log.asm             # Structured JSON logging
│   ├── shell.asm           # Interactive command shell
│   ├── utils.asm           # Utility functions
│   └── linker.ld           # AArch64 linker script
├── tests/                  # Automated test cases
├── examples/               # Example programs
├── Makefile
├── run_kernel.sh
└── README.md
```

## License

MIT
