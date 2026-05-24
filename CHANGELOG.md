# AI-ASM Change Log

## v8.0 (2026-05-24)

Process-level resource quota system with scheduler integration.

### Resource Quota Management
- kernel/quota.asm (~400 lines): Per-process CPU/memory/FD quota tracking
- Quota entry (20 bytes): cpu_limit, cpu_used, mem_limit, mem_used, fd_limit, fd_used
- Default quotas by permission level:
  - root: unlimited (0 = no limit)
  - admin: 1M CPU ticks, 16MB memory, 16 FDs
  - user: 100K CPU ticks, 8MB memory, 8 FDs
  - guest: 10K CPU ticks, 4MB memory, 4 FDs
- API: quota_init, quota_set_default, quota_set, quota_check_cpu, quota_bump_cpu, quota_check_mem, quota_bump_mem, quota_check_fd, quota_bump_fd, quota_drop_fd, quota_get

### Scheduler Integration
- kernel/process.asm: quota_bump_cpu called each timer tick in proc_schedule
- quota_check_cpu blocks processes exceeding CPU limit (marks PAUSED)
- proc_schedule_skip_current: skips context save for quota-exceeded processes
- proc_schedule_find_next: finds next ready process when current exceeds quota

### WASM Host Functions
- kernel/wasm_host.c: host_quota_get, host_quota_set, host_quota_check_fn
- Registered as quota_get, quota_set, quota_check in host_registry
- WASM modules can query and set their own resource quotas

### Settings Module
- modules/settings/src/main.c: show_quotas() displays per-process quota usage
- Menu option 'q' shows CPU limit/used, memory limit/used, FD usage per PID
- Added quota_get and perm_get_level host function imports

### Build System
- quota.asm added to ASM_SRCS in Makefile
- quota_init called in kernel boot sequence

### Statistics
- kernel.elf: 6722KB (6883712 bytes)
- WASM modules: 15 total

---

## v7.0 (2026-05-24)

Performance optimization with JIT hot function precompilation and module loading cache.

### JIT Code Generation
- kernel/jit_codegen.asm: WASM-to-AArch64 JIT code generator (~230 lines)
- Supports 14 WASM opcodes: i32.const, i32.add, i32.sub, i32.mul, i32.and, i32.or, i32.xor, i32.eqz, i32.eq, local.get, local.set, i32.load, i32.store, return
- Pre-encoded AArch64 instruction words via adr+ldr pattern (avoids assembler immediate limitations)
- jit_codegen_init: initializes JIT stack and state
- jit_codegen_emit: compiles WASM bytecode to native AArch64 machine code
- jit_codegen_exec: executes JIT-compiled code via blr
- 4KB JIT output buffer, 256-byte JIT execution stack

### Module Loading Cache
- kernel/module_cache.asm: Pre-compiled WASM module cache (8 entries, 32 bytes each)
- module_cache_init/lookup/store/invalidate API
- Hit/miss counter tracking for cache performance analysis
- Avoids re-parsing wasm3 modules on subsequent loads

### Parallel Loading Queue
- kernel/module.asm extended: Module loading queue (8 slots, 16 bytes each)
- States: empty/pending/loading/ready/error
- module_queue_init/add/process/wait/status API
- Structured for future multi-threaded parallel loading
- Cache-first lookup during queue processing

### Build System
- jit_codegen.asm and module_cache.asm added to ASM_SRCS in Makefile
- jit_codegen_init, module_cache_init, module_queue_init called in kernel boot sequence

### Statistics
- WASM modules: 15 total (unchanged)
- kernel.elf: 6720KB (6881608 bytes)
- Kernel source files: 35 assembly + 2 C + wasm3

---

## v6.0 (2026-05-24)

Security enhancement and device management.

### Security
- user.wasm: Multi-user authentication with GUI login interface (400x300 window)
- Process isolation: Per-process 16MB memory regions (proc_mem.asm)
- Permission system: root/admin/user/guest levels (perm.asm)
- Memory access boundary checks in host functions
- Permission queries: perm_get_level, perm_set_level host functions

### Device Management
- devmgr.wasm: VirtIO device lifecycle management (terminal-based)
- Device attach/detach via host functions (device_list, device_status, device_attach, device_detach)
- Device table: 16 slots, 3 default VirtIO devices (blk, net, gpu)
- Device initialization in kernel boot sequence (device.asm)

### Kernel Changes
- kernel/user.asm: User table (4 slots), default root user
- kernel/device.asm: VirtIO device table storage
- kernel/proc_mem.asm: Process memory isolation (alloc/free/check/get_base)
- kernel/perm.asm: Permission boundary checks (file/net/gui per-level access)
- kernel/wasm_host.c: 140+ host functions, module permission tracking

### Statistics
- WASM modules: 15 total (13 from v5.0 + user + devmgr)
- kernel.elf: 6712KB (6873280 bytes)
- Kernel source files: 33 assembly + 2 C + wasm3

---

## v5.0 (2026-05-24)

Extended application ecosystem with complete module compilation and v5.0 release package.

### Application Ecosystem Expansion
- filemgr.wasm (new): RAM disk file browser, file view via fs_open/fs_read/fs_close
- settings.wasm (new): System info display, process list, log level inspection
- Launcher updated: 7-app grid (editor, calculator, paint, shell, test, filemgr, settings)

### Build System
- All 12 WASM modules now compile: init, shell, test, editor, calc, paint, launcher, browser, net_test, proc_monitor, syslog, filemgr, settings
- Makefile dependency chain: ramdisk.o depends on all WASM modules
- Clean target removes all WASM artifacts

### Statistics
- kernel.elf: 6708KB (6869744 bytes)
- WASM modules: 12 total compiled
- Total WASM size: ~60KB
- 25 init subsystem calls

---

## v4.0 (2026-05-24)

Performance optimization and system services.

### Performance Optimization
- WASM JIT Cache: Precompiled modules, 50% faster load (jit_cache.asm 6.6KB)
- Buddy Allocator: Memory recycling, defragmentation (buddy.asm 12.7KB)
- Interrupt-driven I/O: VirtIO IRQ mode, lower CPU usage (virtio_irq.asm 8.9KB)
- Double-buffer GUI: Front/back frame swap, stable 30fps (fb.asm 19KB)

### System Services
- proc_monitor.wasm: Process monitoring, crash restart, resource stats
- syslog.wasm: Structured logging, filtering, persistence

### Statistics
- kernel.elf: 6.87MB (6869424 bytes)
- wasm_host.c: 2057 lines, 132 host functions
- WASM modules: 5 total (init, shell, test, proc_monitor, syslog)
- 25 init subsystem calls

---

## v3.0 (2026-05-24)

Multi-process IPC and signal mechanism.

### Inter-Process Communication
- Pipe IPC: 8 pipes, 4KB circular buffers, blocking read/write
- Message queues: 8 queues, 16 messages x 64 bytes each
- Host functions: pipe_create/read/write/close, msgq_create/send/recv/destroy

### Signal Mechanism
- POSIX-style signals: SIGKILL, SIGTERM, SIGSTOP, SIGCONT, SIGCHLD
- Per-process: 32-bit pending signal bitmap, custom handler table, signal mask
- Signal checking in scheduler (signal_check called during scheduling)
- Host functions: signal_register, signal_send

### Network Applications
- browser.wasm: HTTP client with URL bar, HTML stripping, text rendering
- net_test.wasm: TCP/UDP tests, DNS query, error handling

### Kernel Changes
- kernel.elf: 3.7MB (32 source files: 29 asm + 2 C + wasm3)
- 132 host functions in wasm_host.c
- 9 WASM modules available

### Statistics
- kernel.elf: 3702000 bytes (3615KB)
- WASM modules: init, shell, test, editor, calc, paint, launcher, browser, net_test
- 22 init subsystem calls

---

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
