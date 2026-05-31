# AI-ASM AArch64

Pure AArch64 assembly WebAssembly-native operating system for QEMU virt.

[![Version](https://img.shields.io/badge/version-v1.0-blue)](https://github.com/zenjan1/ai-asm/releases)
[![Modules](https://img.shields.io/badge/wasm-335%20modules-green)]()
[![Kernel](https://img.shields.io/badge/kernel-9.3MB-orange)]()
[![License](https://img.shields.io/badge/license-MIT-lightgrey)](LICENSE)

## Overview

AI-ASM AArch64 is a from-scratch operating system written in pure AArch64 assembly with a WebAssembly-based application ecosystem. It runs on QEMU's virt machine and provides preemptive multitasking, virtual memory, VirtIO device drivers, FAT32 filesystem, TCP/IP networking, and a GUI window manager.

### Why?

This project explores whether a WebAssembly runtime can serve as the primary application layer for a bare-metal OS, with the kernel providing low-level resource management and the WASM modules handling user-space functionality.

## Features

- **Preemptive scheduling** with GIC interrupt system
- **MMU virtual memory** and process isolation
- **VirtIO device drivers** — block, network, GPU
- **FAT32 filesystem** with read/write/create/delete
- **TCP/IP network stack** with ARP cache and socket table
- **WASI snapshot_preview1** system call interface
- **GUI window manager** with Z-order compositing (up to 16 windows)
- **Wasm3 WebAssembly runtime** with multi-module support
- **335 WASM application modules** covering coreutils, networking, and applications

## Architecture

```
+----------------------------------------------------------+
|  Top: WASM Applications (335 modules)                     |
|  coreutils, networking, system tools, GUI apps            |
+----------------------------------------------------------+
|  Middle: Wasm3 Runtime + WASI Host Functions              |
|  host_print, host_alloc, host_fs_*, host_net_*, host_gui_*|
+----------------------------------------------------------+
|  Bottom: AArch64 Assembly Kernel Core                     |
|  Scheduler · MMU · VirtIO · FAT32 · TCP/IP · GUI · Wasm3  |
+----------------------------------------------------------+
|  Hardware: QEMU virt (AArch64)                            |
+----------------------------------------------------------+
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

## WASM Modules

### System & Kernel (30+)

| Module | Description | Module | Description |
|--------|-------------|--------|-------------|
| init | Bootstrapper | shell | Interactive shell |
| test | Integration tests | proc_monitor | Process monitor |
| syslog | System logging | filemgr | File manager |
| settings | System settings | user | User management |
| devmgr | Device manager | httpd | HTTP server |
| dns_resolver | DNS resolver | launcher | App launcher |
| install | Package installer | | |

### Coreutils (80+)

| Module | Description | Module | Description |
|--------|-------------|--------|-------------|
| cat | Display file | echo | Print text |
| ls | List files | pwd | Working directory |
| wc | Word/line count | head | First lines |
| tail | Last lines | sort | Sort lines |
| uniq | Remove duplicates | tr | Translate chars |
| cut | Column extract | sed | Stream editor |
| grep | Pattern match | find | Find files |
| cp | Copy files | mv | Move files |
| rm | Remove files | mkdir | Make directory |
| chmod | Change mode | ln | Create links |
| touch | Update timestamps | tee | Tee output |
| printf | Formatted print | nl | Number lines |
| rev | Reverse lines | cmp | Compare files |
| diff | File diff | basename | Strip path |
| dirname | Strip filename | yes | Repeated output |
| fold | Wrap lines | tac | Reverse cat |
| cksum | Checksum | sum | Simple checksum |
| du | Disk usage | df | Disk free |
| more | Pager | less | Advanced pager |
| xargs | Build commands | shuf | Shuffle lines |
| comm | Compare sorted | paste | Merge lines |
| join | Join lines | split | Split files |
| patch | Apply patches | diff3 | 3-way diff |
| tsort | Topological sort | fmt | Format text |
| column | Columnate output | expr | Evaluate expr |
| env | Environment | envsubst | Substitute vars |
| date | Date/time | cal | Calendar |
| sleep | Delay | true/false | Boolean exit |
| seq | Sequence numbers | factor | Factorize |
| realpath | Resolve path | readlink | Read symlinks |
| mcookie | Random cookie | stat | File stats |
| dircolors | LS colors | test | Test expr |
| dd | Convert & copy | hashsum | Hash files |
| watch | Monitor command | shred | Secure delete |
| truncate | Truncate file | nohup | No hangup |

### Archive & Compression

gzip, gunzip, zcat, bzip2, bunzip2, lzma, unlzma, xz, unxz, tar

### Text Processing

awk, sed, tr, cut, grep, sort, uniq, comm, paste, join, fold, tac, rev, head, tail, wc, nl, shuf, csplit, fmt, tsort, patch, diff3

### Information

id, uptime, whoami, hostname, uname, free, ps, kill, tty, who, stty, tput, pathchk, mktemp

### Networking

ifconfig, ping, ping6, netstat, route, traceroute, nslookup, wget, curl, ssh, scp, nmap, dig, host, arp, arping, iwconfig, ifup, ifstat, nc, iptables, socat, tcpdump, ethtool, brctl, mii-tool, nameif, plipconfig, pppd, slattach

### Console & Keyboard

| Module | Description | Module | Description |
|--------|-------------|--------|-------------|
| chvt (x7) | Switch virtual tty | setfont (x16) | Set console font |
| kbd_mode (x16) | Keyboard mode | loadkeys (x12) | Load keymap |
| dumpkeys (x15) | Dump keymap | showkey (x12) | Show keycodes |
| openvt (x13) | Open virtual tty | deallocvt (x12) | Dealloc virtual tty |
| fgconsole (x16) | Foreground console | setkeycodes (x12) | Set keycodes |
| unicode_start (x8) | Unicode mode on | mapscrn (x8) | Screen mapping |
| unicode_stop (x8) | Unicode mode off | setmetamode (x8) | Meta key mode |
| loadunimap (x8) | Load Unicode map | | |

### Hash & Checksum

sha256sum, md5sum, cksum, sum, hashsum

### Debugging & Profiling

strace, ltrace, tracepath, ss, top, htop, iostat, vmstat, mpstat, pidof, pgrep, pkill, lsof

### Applications

editor, calc, paint, launcher, browser

## Directory Structure

```
aiasm-aarch64/
├── kernel/
│   ├── kernel.asm          # Kernel entry, boot sequence
│   ├── pl011.asm           # PL011 UART driver
│   ├── memory.asm          # Physical memory manager + heap
│   ├── mmu.asm             # MMU page tables + virtual memory
│   ├── gic.asm             # ARM GIC interrupt controller
│   ├── process.asm         # Process scheduler (preemptive)
│   ├── virtio_blk.asm      # VirtIO-Block driver
│   ├── virtio_net.asm      # VirtIO-Net driver
│   ├── virtio_gpu.asm      # VirtIO-GPU (800x600 RGBA)
│   ├── fs.asm              # FAT32 filesystem
│   ├── net.asm             # TCP/IP stack (ARP + sockets)
│   ├── wasi.asm            # WASI syscall dispatch
│   ├── gui.asm             # GUI window manager
│   ├── wasm_host.c         # Wasm3 host functions (~1680 lines)
│   └── wasm3/              # Wasm3 WebAssembly runtime
├── modules/
│   ├── init/src/main.c     # Init module (bootstrapper)
│   ├── shell/src/main.c    # Interactive shell
│   └── ...                 # 333 additional WASM modules
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
| Kernel binary | ~9.3MB |
| Assembly files | 51 |
| WASM modules | 335 |

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
