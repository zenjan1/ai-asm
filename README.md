# AI-ASM AArch64 v1.0

Pure AArch64 assembly WASM-native operating system for QEMU virt.

**295 WASM modules** · 7653528 bytes kernel · v303.0

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
- **295 WASM modules** · 7653528 bytes kernel · v303.0

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

### System: init, shell, test, proc_monitor, syslog, filemgr, settings, user, devmgr, httpd, dns_resolver, stdbuf, factor, seq, realpath, groups, install, pathchk, mktemp, truncate, nohup, envsubst, dircolors, expr, test, dd, hashsum, watch, wasm-dis, shred, stat, mcookie, lsof, iostat, vmstat, mpstat, pidof, pgrep, pkill, top, htop, strace, ltrace, tracepath, ss, ip, killall, nmap, dig, host, arp, iwconfig, ifup, nc, iptables, setkeycodes, loadkeys, dumpkeys, showkey, chvt, openvt, deallocvt, fgconsole, setfont, kbd_mode, showkey_v2, chvt_v2, openvt_v2, deallocvt_v2, fgconsole_v2, setfont_v2, kbd_mode_v2, showkey_v3, chvt_v3, openvt_v3, deallocvt_v3, fgconsole_v3, setfont_v3, kbd_mode_v3, loadkeys_v2, dumpkeys_v2, dumpkeys_v3, showkey_v4, chvt_v4, openvt_v4, deallocvt_v4, fgconsole_v4, setfont_v4, kbd_mode_v4, loadkeys_v3, dumpkeys_v4, setkeycodes_v2, showkey_v5, chvt_v5, openvt_v5, deallocvt_v5, fgconsole_v5, setfont_v5, kbd_mode_v5, loadkeys_v4, dumpkeys_v5, setkeycodes_v3, setkeycodes_v4, loadkeys_v5, setkeycodes_v5, dumpkeys_v6, showkey_v6, chvt_v6, openvt_v6, deallocvt_v6, fgconsole_v6, setfont_v6, kbd_mode_v6, loadkeys_v6, setkeycodes_v6, unicode_start, unicode_stop, mapscrn, loadunimap, setmetamode, deallocvt_v7, fgconsole_v7, kbd_mode_v7, loadkeys_v7, openvt_v7, setfont_v7, showkey_v7, chvt_v7, deallocvt_v8, fgconsole_v8, kbd_mode_v8, loadkeys_v8, openvt_v8, setfont_v8, showkey_v8, setkeycodes_v8, dumpkeys_v8, mapscrn_v8, loadunimap_v8, setmetamode_v8, unicode_start_v8, unicode_stop_v8, loadkeys_v9, openvt_v9, deallocvt_v9, fgconsole_v9, kbd_mode_v9, loadkeys_v10, openvt_v10, deallocvt_v10, fgconsole_v10, setfont_v10, kbd_mode_v10, loadkeys_v11, openvt_v11, deallocvt_v11, fgconsole_v11, setfont_v11, kbd_mode_v11, loadkeys_v12, openvt_v12, deallocvt_v12, fgconsole_v12, kbd_mode_v12
### File: cat, echo, wc, head, tail, sort, uniq, tr, cut, sed, xargs, grep, rev, cmp, diff, basename, dirname, yes, tee, printf, nl, shuf, comm, paste, fold, tac, cksum, sum, touch, find, du, cp, mv, rm, ln, mkdir, rmdir, chmod, gzip, gunzip, zcat, bzip2, bunzip2, lzma, unlzma, xz, unxz, sha256sum, md5sum, base64, base64d, od, hexdump, strings, file, xxd, less, more, join, split, patch, diff3, tsort, csplit, fmt
### Info: ls, pwd, env, id, uptime, whoami, hostname, uname, date, awk, df, free, ps, kill, tty, who, stty, tput, readlink, mount, umount
### Calendar/Time: date, cal
### Network: ifconfig, ping, netstat, route, traceroute, nslookup, wget, curl, ssh, scp, nmap, dig, arp, iwconfig, ifup, nc, iptables, socat, tcpdump, ping6, ethtool, arping, brctl, ifstat, mii-tool, nameif, plipconfig, pppd, slattach
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
| Architecture | AArch64 (ARMv8-A) + x86_64 |
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
| Kernel binary | ~7.2MB (7517184 bytes) |
| Assembly files | 26 |
| WASM modules | 271 |

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
