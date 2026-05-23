/*
 * aiasm-aarch64/modules/shell/src/main.rs
 * Wasm Shell module for AI-ASM AArch64 kernel
 * Runs inside wasm3 interpreter, calls host functions for I/O.
 */

#![no_std]
#![no_main]

use core::arch::global_asm;

/* -------------------------------------------------------------------------- */
/* Host function imports                                                      */
/* -------------------------------------------------------------------------- */

extern "C" {
    /// Print bytes from WASM memory to UART
    fn host_print(offset: u32, len: u32);

    /// Get current tick counter (ARM CNTVCT_EL0)
    fn host_get_tick() -> u64;

    /// Halt the system with exit code
    fn host_exit(code: i32) -> !;

    /// Allocate a 4KB page from kernel allocator
    fn host_alloc_page() -> u32;
}

/* -------------------------------------------------------------------------- */
/* WASM memory is linear, we use a simple bump allocator for strings          */
/* -------------------------------------------------------------------------- */

static mut HEAP_OFFSET: u32 = 65536; // Start after first page

fn alloc_bytes(n: usize) -> u32 {
    unsafe {
        let ptr = HEAP_OFFSET;
        HEAP_OFFSET += n as u32;
        // Align to 16 bytes
        HEAP_OFFSET = (HEAP_OFFSET + 15) & !15;
        ptr
    }
}

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

fn print_str(s: &str) {
    let offset = alloc_bytes(s.len());
    let mem = unsafe { (offset as *mut u8).as_mut_ptr() };
    unsafe {
        core::ptr::copy_nonoverlapping(s.as_ptr(), mem, s.len());
    }
    unsafe { host_print(offset, s.len() as u32) };
}

fn print_u64(v: u64) {
    let mut buf = [0u8; 20];
    let mut n = v;
    let mut i = 0;
    if n == 0 {
        buf[0] = b'0';
        i = 1;
    } else {
        while n > 0 {
            buf[i] = b'0' + (n % 10) as u8;
            n /= 10;
            i += 1;
        }
    }
    // Reverse
    let offset = alloc_bytes(i);
    let mem = unsafe { (offset as *mut u8).as_mut_ptr() };
    for j in 0..i {
        unsafe { mem.add(j).write(buf[i - 1 - j]) };
    }
    unsafe { host_print(offset, i as u32) };
}

/* -------------------------------------------------------------------------- */
/* Simple line reader: reads bytes until newline or max_len                   */
/* -------------------------------------------------------------------------- */

fn read_line(buf: &mut [u8]) -> usize {
    // For now, just return 0 — actual input requires kernel to feed bytes
    // This will be implemented when we add WASM memory input from kernel
    0
}

/* -------------------------------------------------------------------------- */
/* Shell main loop                                                             */
/* -------------------------------------------------------------------------- */

#[no_mangle]
pub extern "C" fn _start() {
    print_str("\n========================================\n");
    print_str("  AI-ASM AArch64 v0.3 — Wasm Shell\n");
    print_str("========================================\n\n");

    // Show tick counter
    print_str("tick: ");
    let tick = unsafe { host_get_tick() };
    print_u64(tick);
    print_str("\n\n");

    // Simple command loop (just prints prompt — no input yet)
    loop {
        print_str("wasm> ");

        // For now, just exit after one prompt since we don't have input
        // In v0.4 we'll add input from kernel serial → wasm memory
        print_str("(input not yet implemented)\n");
        break;
    }

    print_str("\nShell exited.\n");
    unsafe { host_exit(0) };
}

/* -------------------------------------------------------------------------- */
/* Optional: also export as "main" for wasm3 lookup                           */
/* -------------------------------------------------------------------------- */

#[no_mangle]
pub extern "C" fn main() {
    _start();
}

/* -------------------------------------------------------------------------- */
/* Panic handler (required for #![no_std])                                     */
/* -------------------------------------------------------------------------- */

#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    // Print panic message via host
    unsafe {
        // We can't use print_str here because it allocates,
        // and panic might be called during alloc
        // Just print a simple marker
        host_print(0, 0); // noop
    }
    unsafe { host_exit(-1) };
}
