# AI-ASM Change Log

## v2.0 (2026-05-24)

Application ecosystem with launcher and GUI applications.

### Application Ecosystem
- Text Editor (editor.wasm): file load/save, 5x7 font rendering, cursor tracking
- Calculator (calc.wasm): 4x5 button grid, arithmetic operations, keyboard input
- Paint (paint.wasm): 16-color palette, freehand drawing, Bresenham lines, adjustable brush
- Application Launcher (launcher.wasm): 2x3 card layout, app spawning, status display
- Shell (shell.wasm): interactive command interface
- Integration Test (test.wasm): FS/NET/GUI/CORE test suite

### GUI Applications
- Window creation and management via `host.gui_create`
- Framebuffer rendering with RGBA8888 pixel format
- GUI event polling for mouse/keyboard input
- Toolbar and palette UI components

### File API
- File create/write/read/delete via WASM host functions
- FAT32 filesystem integration
- Directory listing support

### WASM Modules (7 total)
- init.wasm (1332B) - system initialization, spawns launcher
- shell.wasm (3553B) - interactive shell
- editor.wasm (4949B) - text editor
- calc.wasm (4008B) - calculator
- paint.wasm (3316B) - drawing application
- launcher.wasm (3995B) - application menu
- test.wasm (4399B) - integration tests

### Statistics
- kernel.elf: 3.65MB
- 7 WASM modules compiled
- Total WASM size: ~25KB

---

## v1.0 (2026-05-24)

Complete WASM-native AArch64 operating system.

### Phase 1: Interrupt System + Preemptive Scheduling
- GICv2 interrupt controller initialization
- Exception vectors (EL1t, EL1h, EL0_64, EL0_32)
- Timer-based preemption with CNTVCT_EL0 polling
- Process scheduler with PCB, READY/RUNNING/PAUSED/EXITED states
- Kernel event bus (ring buffer, JSON logging)

### Phase 2: MMU Virtual Memory
- L1 page table with 4KB granule
- Identity mapping for kernel space
- Per-process L2 page tables
- Virtual memory manager with bitmap allocation

### Phase 3: VirtIO Device Drivers
- VirtIO-MMIO transport v2
- VirtIO-Block driver (sector read/write)
- VirtIO-Net driver (TX/RX queues, descriptor rings)
- VirtIO-GPU driver (2D resource, transfer, scanout)

### Phase 4: FAT32 Filesystem
- BPB parsing, FAT chain traversal
- 8.3 filename directory entries
- Cluster allocation and FAT chain management
- File read with FAT chain following

### Phase 5: TCP/IP Network Stack
- ARP cache (8 entries)
- Socket table (8 sockets x 2080 bytes)
- TCP connection establishment
- UDP packet send/recv

### Phase 6: WASI System Services
- Full `wasi_snapshot_preview1` syscall dispatch
- stdout/stderr routing to UART
- stdin routing from UART
- `clock_time_get` via CNTVCT_EL0
- `random_get` via PRNG seeded from timer
- `path_open`, `fd_close`, `proc_exit`
- WASM module repository (16 entries)
- Process management: pause/resume/kill/wait

### Phase 6b: WASI Host Function Implementation
- Real host function implementations (no placeholders)
- `wasi_fd_write`: stdout/stderr -> UART serial output
- `wasi_fd_read`: stdin -> UART blocking read
- `wasi_clock_time_get`: nanosecond conversion
- `wasi_random_get`: LCG PRNG from cycle counter
- `wasi_path_open`: path-based file opening
- `wasi_fd_close`, `wasi_args_sizes_get`, `wasi_environ_sizes_get`

### Phase 7: GUI Window Manager
- 800x600 RGBA8888 framebuffer abstraction
- 5x7 bitmap font rendering
- 16-window Z-order compositor
- Window title bars, borders, content areas
- Per-window event queues (16 events x 16 bytes ring buffer)
- Mouse hit-testing and window focus management
- Keyboard event dispatch to focused window
- `host.gui_poll_event` WASM host function
- Window creation/deletion/rendering/compositing

### Integration Testing
- Test WASM module: FS (create/write/read/delete), NET, GUI, CORE
- `run_test.sh` QEMU test runner with pass/fail detection
- 64MB test disk image

### Statistics
- 26 assembly files
- wasm_host.c: 1680 lines
- kernel.elf: 3.65MB
- 3 WASM modules (init, shell, test)
- 19 init subsystem calls

---

## v0.5 (2026-05-23)

Wasm3 runtime integration with multi-module support.

- Wasm3 WebAssembly runtime compiled as freestanding library
- Multi-module environment with shared IM3Environment
- Module lifecycle: FREE -> LOADING -> READY -> RUNNING -> EXITED
- RAM disk filesystem (embedded USTAR TAR)
- Host functions: print, get_tick, exit, alloc, free, log, getc, spawn
- Init module bootstrapper

## v0.4 (2026-05-23)

QEMU boot success with interactive shell.

- Wasm3 runtime integration
- Interactive shell module (WASM)
- UART RX ring buffer
- Boot sequence completion

## v0.3 (2026-05-22)

Wasm3 WebAssembly runtime integration.

- Wasm3 source integration
- Basic WASM module loading

## v0.2 (2026-05-22)

Full kernel subsystems.

- GIC interrupt controller
- Timer subsystem
- Physical memory manager + kernel heap
- Process scheduler
- Exception handlers
- Structured JSON logging
- Event bus

## v0.1 (2026-05-22)

Initial minimal pure AArch64 assembly kernel.

- PL011 UART driver (polling)
- Basic shell with help/version/log/events/reboot/shutdown
- Linker script for 0x40080000 load address
- No external dependencies
