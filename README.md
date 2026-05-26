# AI-ASM AArch64 v1.0

Pure AArch64 assembly WASM-native operating system for QEMU virt.

**92 WASM modules** · 7167280 bytes kernel · v99.0

## Features

- Preemptive scheduling + GIC interrupt system
- MMU virtual memory + process isolation
- VirtIO device drivers (blk / net / gpu)
- FAT32 filesystem with read/write/create/delete
- TCP/IP network stack with ARP cache and socket table
- WASI snapshot_preview1 system call interface
- GUI window manager with Z-order compositing (up to 16 windows)
- WebAssembly runtime via Wasm3 with multi-module support
- Integration test suite (FS / NET / GUI / CORE)
- 92 WASM application modules

## Three-Layer Architecture

```
Bottom (Assembly):  Kernel core — scheduler, MMU, drivers, filesystem, net, GUI
Middle (C + WASI):  Wasm3 runtime, host functions, WASI syscalls
Top (WASM):         92 Application modules
```

## Quick Start

### Prerequisites

```bash
pkg update && pkg upgrade -y
pkg install binutils clang qemu-system-aarch64 make
```

### Build and Run

```bash
make && make run
```

### Run Integration Tests

```bash
./run_test.sh
```

## Available WASM Modules

### System: init, shell, test, proc_monitor, syslog, filemgr, settings, user, devmgr, httpd, dns_resolver
### File: cat, echo, wc, head, tail, sort, uniq, tr, cut, sed, xargs, grep, rev, cmp, diff, basename, dirname, yes, tee, printf, nl, shuf, comm, paste, fold, tac, cksum, sum, touch, find, du, cp, mv, rm, ln, mkdir, rmdir, chmod
### Info: ls, pwd, env, id, uptime, whoami, hostname, uname, date, awk, df, free, ps, kill, tty, who, stty, tput, readlink, mount, umount
### Calendar/Time: date, cal
### Network: ifconfig, ping, netstat, route, traceroute, nslookup, wget, curl, ssh, scp
### Apps: editor, calc, paint, launcher, browser

## Directory Structure

```
aiasm-aarch64/
├── kernel/
│   ├── kernel.asm          # Kernel entry, boot sequence
│   ├── pl011.asm           # PL011 UART driver (polling)
│   ├── log.asm             # Structured JSON log
│   ├── event.asm           # Kernel event bus (ring buffer)
│   ├── utils.asm           # memset/memcpy/strlen/itoa/strcmp
│   ├── memory.asm          # Physical memory manager + heap
│   ├── mmu.asm             # MMU page tables + virtual memory
│   ├── timer.asm           # ARM Generic Timer
│   ├── gic.asm             # ARM GIC interrupt controller
│   ├── exceptions.asm      # Exception vectors and handlers
│   ├── process.asm         # Process scheduler (preemptive)
│   ├── virtio.asm          # VirtIO-MMIO transport
│   ├── virtio_blk.asm      # VirtIO-Block driver
│   ├── virtio_net.asm      # VirtIO-Net driver
│   ├── virtio_gpu.asm      # VirtIO-GPU driver (800x600 RGBA)
│   ├── fs.asm              # FAT32 filesystem (read/write)
│   ├── net.asm             # TCP/IP stack (ARP + sockets)
│   ├── wasi.asm            # WASI syscall dispatch
│   ├── module.asm          # WASM module repository
│   ├── fb.asm              # Framebuffer abstraction
│   ├── gui.asm             # GUI window manager
│   ├── serial_rx.asm       # UART RX ring buffer
│   ├── wasm_embed.asm      # Embedded WASM modules
│   ├── ramdisk.asm         # RAM disk (embedded TAR)
│   ├── linker.ld           # Linker script (entry 0x40080000)
│   ├── wasm_host.c         # Wasm3 host functions (~1680 lines)
│   ├── libc_shim.c         # Minimal libc shim
│   └── wasm3/              # Wasm3 WebAssembly runtime
├── modules/
│   ├── init/src/main.c     # Init module (bootstrapper)
│   ├── shell/src/main.c    # Interactive shell module
│   ├── test/src/main.c     # Integration test suite
│   └── ...                 # 82 additional WASM modules
├── ramdisk/                # RAM disk file contents
├── Makefile
├── README.md
├── CHANGELOG.md
├── ROADMAP.md
└── run_test.sh
```

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| Architecture | AArch64 (ARMv8-A) |
| Platform | QEMU virt machine |
| RAM | 128MB |
| Load address | 0x40080000 |
| UART | PL011 @ 0x09000000, 115200 8N1 |
| Framebuffer | 800x600 RGBA8888 via VirtIO-GPU |
| Network | VirtIO-Net @ 0x0A000000 |
| Storage | VirtIO-Block @ 0x0A000200 |
| GPU | VirtIO-GPU @ 0x0A000400 |
| WASM runtime | Wasm3 with d_m3FixedHeap=1MB |
| Max windows | 16 (Z-order compositing) |
| Max processes | 16 PCB entries |
| Max sockets | 8 (2080 bytes each) |
| Filesystem | FAT32 with cluster allocation |
| Kernel binary | ~6.9MB |
| Assembly files | 26 |
| WASM modules | 85 |

## WASM Host API

| Function | Signature | Description |
|----------|-----------|-------------|
| `host.print` | `v(ii)` | Print string from memory |
| `host.get_tick` | `I()` | Get CPU tick counter |
| `host.exit` | `v(i)` | Exit module |
| `host.alloc` | `i(ii)` | Allocate from WASM memory |
| `host.free` | `v(i)` | Free WASM memory |
| `host.log` | `v(iiii)` | Kernel logging |
| `host.getc` | `i()` | UART input (blocking) |
| `host.spawn` | `i(ii)` | Spawn WASM module |
| `host.sleep` | `v(i)` | Delay in milliseconds |
| `host.yield` | `v()` | Yield to scheduler |
| `host.fs_open` | `i(ii)` | Open file |
| `host.fs_read` | `i(iii)` | Read file |
| `host.fs_write` | `i(iii)` | Write file |
| `host.fs_create` | `i(ii)` | Create file |
| `host.fs_delete` | `i(ii)` | Delete file |
| `host.fs_close` | `v(i)` | Close file |
| `host.fs_list` | `i(ii)` | List files |
| `host.get_argv` | `i(ii)` | Get command line arguments |
| `host.get_env` | `i(ii)` | Get environment variable |
| `host.blk_read` | `i(iiii)` | Read block device |
| `host.blk_write` | `i(iiii)` | Write block device |
| `host.net_connect` | `i(iii)` | Create TCP/UDP socket |
| `host.net_send` | `i(iii)` | Send data |
| `host.net_recv` | `i(iii)` | Receive data |
| `host.net_close` | `v(i)` | Close socket |
| `host.gui_create` | `i(iiiii)` | Create window |
| `host.gui_draw` | `i(iiiiii)` | Draw pixels to window |
| `host.gui_flush` | `v(i)` | Flush screen |
| `host.gui_poll_event` | `i(ii)` | Poll window event queue |

## WASI Support

Full `wasi_snapshot_preview1` interface:
- `fd_write`, `fd_read`, `fd_close`, `fd_seek`, `fd_tell`
- `path_open`, `path_removefile`
- `proc_exit`
- `clock_time_get`
- `random_get`
- `args_sizes_get`, `environ_sizes_get`

## License

MIT
