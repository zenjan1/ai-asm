---
name: aiasm-build-debug
description: Build and diagnose the AI-ASM AArch64 kernel on Termux — handles Makefile gaps, missing WASM modules, and QEMU boot crash analysis
source: auto-skill
extracted_at: '2026-06-19T15:13:50.832Z'
---

## Building AI-ASM AArch64 on Termux

### Prerequisites
```bash
pkg install binutils clang qemu-system-aarch64 make
```

### Build procedure

The `make` target has a **dependency gap**: the `modules` phony target does not list all WASM modules that `ramdisk.o` depends on, so `make all` fails with "file not found: kernel/XXX.wasm" errors for ~36 modules.

**Step 1 — Build what Make can:**
```bash
make clean
make modules   # builds ~298 of the needed WASM modules
```

**Step 2 — Manually build the 36 missing modules:**
```bash
for f in hostname uname find du mount umount ifconfig ping netstat route traceroute nslookup wget curl ssh scp unxz sha256sum md5sum base64 base64d od hexdump strings file xxd less more join split patch diff3 tsort csplit fmt pr; do
  if [ -f "modules/$f/src/main.c" ] && [ ! -f "kernel/$f.wasm" ]; then
    clang --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
      -Wl,--no-entry -Wl,--export=_start -o "modules/$f/$f.wasm" "modules/$f/src/main.c"
    cp "modules/$f/$f.wasm" "kernel/$f.wasm"
  fi
done
```

**Step 3 — Link the kernel:**
```bash
make   # now succeeds, producing build/kernel.elf (~7.7MB)
```

### Known Makefile issues
- **Duplicate variable definitions**: `TAR_WASM`, `GZIP_WASM`, `XZ_WASM`, `COLUMN_WASM`, `EXPAND_WASM`, etc. are defined multiple times (e.g., lines 208 vs 289) causing "overriding recipe" warnings
- **Incomplete `modules` target**: ~36 WASM modules have both source files (`modules/XXX/src/main.c`) and Makefile rules but are missing from the `modules:` phony target dependency list
- **`buddy_init` called twice**: Once inside `mem_init` (kernel.asm:56 → memory.asm:109) and again directly (kernel.asm:112)

### Diagnosing QEMU boot crashes

The kernel produces **zero serial output** when it crashes early. Use QEMU debug flags to see exceptions:

```bash
qemu-system-aarch64 -M virt,gic-version=2 -cpu cortex-a53 -m 128M \
  -kernel build/kernel.elf -drive file=disk.img,format=raw,if=virtio \
  -no-reboot -display none -serial file:qemu_serial.log \
  -d guest_errors,int 2>qemu_debug.log
```

**Interpreting debug output:**
- `Data Abort` with `FAR 0xXXXXXXXX` = invalid memory access at that address
- `Undefined Instruction` at `0x200` = exception vectors not set up (crash before `msr vbar_el1`)
- Use `objdump -d build/kernel.elf | grep <ELR_address>` to find the crashing instruction
- Use `objdump -t build/kernel.elf` to find symbol addresses for BSS/stack/data regions

### Current boot crash (unresolved)

The kernel crashes with a **Data Abort at `0x400863b0`** (in `_buddy_fl_insert`) accessing `FAR 0x40660004` during the first `buddy_init` call from `mem_init`. This is **before UART initialization completes**, so no serial output is produced.

Context:
- `.bss` is 6.2MB (mostly embedded WASM modules), starting at `0x40160000`
- `buddy_pool` symbol resolves to `0x40660000` (5MB into BSS)
- The crash occurs when `_buddy_fl_insert` tries `stur xzr, [x0, #0x4]` on the pool block
- 128MB RAM is mapped at `0x40000000`–`0x48000000`, so `0x40660004` should be valid
