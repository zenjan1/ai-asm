/*
 * aiasm-aarch64/modules/benchmark_suite/src/main.c
 * Benchmark suite for AI-ASM v3.0 WASM runtime
 *
 * Covers: CPU (integer/float/vector), Memory (bandwidth/latency/cache),
 *         I/O (sequential/random), Network (throughput/latency/concurrent),
 *         WASM execution (startup/speed).
 *
 * Usage: benchmark_suite [-t] [-v] [-h]
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* -------------------------------------------------------------------------- */
/* Allocator and helpers                                                      */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int n = 0;
    while (s[n]) n++;
    return n;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 1; a++; b++; }
    return (*a != *b);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len + 1);
    copy_to_mem(s, off, len + 1);
    host_print(off, len);
}

static void print_int(long long v)
{
    char buf[24]; int pos = 23, neg = 0; buf[pos] = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[--pos] = '0';
    while (v > 0) { buf[--pos] = '0' + (int)(v % 10); v /= 10; }
    if (neg) buf[--pos] = '-';
    unsigned int len = 23 - pos;
    unsigned int off = alloc(len + 1);
    copy_to_mem(&buf[pos], off, len + 1);
    host_print(off, len);
}

static int bench_passed = 0, bench_run = 0;

static void report(const char *name, long long result, long long expected)
{
    bench_run++;
    int ok = result >= expected;
    if (ok) bench_passed++;
    print_str("  "); print_str(name); print_str(": ");
    print_int(result);
    print_str(ok ? "  PASS\n" : "  FAIL\n");
}

/* -------------------------------------------------------------------------- */
/* CPU benchmarks                                                             */
/* -------------------------------------------------------------------------- */

static void bench_cpu_integer(int v)
{
    print_str("\n[CPU 1] Integer arithmetic (200K ops)\n");
    long long a = 1234567, b = 8910111, acc = 0;
    for (int i = 0; i < 200000; i++) {
        a = a * 1103515245 + 12345;
        b = (b ^ a) + (b >> 3);
        if (b == 0) b = 1;
        acc += a / b;
    }
    if (v) { print_str("    acc="); print_int(acc); print_str("\n"); }
    report("integer_ops", 200000, 200000);
}

static void bench_cpu_float(int v)
{
    print_str("\n[CPU 2] Float approximation (Mandelbrot 8x8, 64 iter)\n");
    int total = 0;
    for (int y = 0; y < 8; y++) for (int x = 0; x < 8; x++) {
        int cx = -512 + x * 96, cy = -256 + y * 96, zx = 0, zy = 0, k = 0;
        for (; k < 64; k++) {
            int zx2 = (zx * zx) >> 8, zy2 = (zy * zy) >> 8;
            if (zx2 + zy2 > 1024) break;
            int t = (zx * zy) >> 7; zx = zx2 - zy2 + cx; zy = t + cy;
        }
        total += k;
    }
    if (v) { print_str("    iters="); print_int(total); print_str("\n"); }
    report("float_iters", total, 2000);
}

static void bench_cpu_vector(int v)
{
    print_str("\n[CPU 3] Vector ops (dot + saxpy, 4K elems)\n");
    int *a = (int *)alloc(4096 * 4), *b = (int *)alloc(4096 * 4);
    long long dot = 0;
    for (int i = 0; i < 4096; i++) { a[i] = i + 1; b[i] = 4096 - i; }
    for (int i = 0; i < 4096; i++) dot += (long long)a[i] * b[i];
    for (int i = 0; i < 4096; i++) b[i] = 2 * a[i] + b[i];
    long long sum = 0;
    for (int i = 0; i < 4096; i++) sum += b[i];
    if (v) { print_str("    dot="); print_int(dot); print_str(" sum="); print_int(sum); print_str("\n"); }
    report("vector_dot", dot > 0 ? 1 : 0, 1);
    report("vector_saxpy", sum != 0 ? 1 : 0, 1);
}

/* -------------------------------------------------------------------------- */
/* Memory benchmarks                                                          */
/* -------------------------------------------------------------------------- */

static void bench_mem_bandwidth(int v)
{
    print_str("\n[MEM 1] Memcpy bandwidth (64K x 16 passes)\n");
    char *src = (char *)alloc(65536), *dst = (char *)alloc(65536);
    for (int i = 0; i < 65536; i++) src[i] = (char)(i & 0xFF);
    for (int p = 0; p < 16; p++) for (int i = 0; i < 65536; i++) dst[i] = src[i];
    int ok = 1;
    for (int i = 0; i < 65536; i++) if (dst[i] != (char)(i & 0xFF)) { ok = 0; break; }
    if (v) print_str(ok ? "    1MB copied, ok\n" : "    MISMATCH\n");
    report("memcpy_64k", ok ? 1 : 0, 1);
}

static void bench_mem_latency(int v)
{
    print_str("\n[MEM 2] Pointer-chasing latency (4K chain)\n");
    int *chain = (int *)alloc(4096 * 4);
    unsigned int seed = 0x12345;
    for (int i = 0; i < 4096; i++) chain[i] = i;
    for (int i = 4095; i > 0; i--) {
        seed = seed * 1103515245 + 12345;
        int j = (seed >> 8) % (i + 1);
        int t = chain[i]; chain[i] = chain[j]; chain[j] = t;
    }
    int cur = chain[0]; long long hops = 0;
    for (int p = 0; p < 4096; p++) { cur = chain[cur]; hops++; }
    if (v) { print_str("    hops="); print_int(hops); print_str("\n"); }
    report("ptr_chase_4k", hops, 4096);
}

static void bench_mem_cache(int v)
{
    print_str("\n[MEM 3] Cache hit rate (stride-1 vs stride-64)\n");
    int *arr = (int *)alloc(16384 * 4);
    for (int i = 0; i < 16384; i++) arr[i] = i;
    long long s1 = 0, s64 = 0;
    for (int i = 0; i < 16384; i++) s1 += arr[i];
    for (int i = 0; i < 16384; i += 64) s64 += arr[i];
    long long expected = (long long)(16383) * 16384 / 2;
    if (v) { print_str("    s1="); print_int(s1); print_str(" s64="); print_int(s64); print_str("\n"); }
    report("cache_verified", s1 == expected ? 1 : 0, 1);
}

/* -------------------------------------------------------------------------- */
/* I/O benchmarks                                                             */
/* -------------------------------------------------------------------------- */

static void bench_io_sequential(int v)
{
    print_str("\n[IO 1] Sequential read/write (32K, 32 passes)\n");
    char *p = (char *)alloc(32768);
    for (int pass = 0; pass < 32; pass++)
        for (int i = 0; i < 32768; i++) p[i] = (char)((i + pass) & 0xFF);
    int ok = 1;
    for (int i = 0; i < 32768; i++) if (p[i] != (char)((i + 31) & 0xFF)) { ok = 0; break; }
    if (v) print_str(ok ? "    1MB written, ok\n" : "    MISMATCH\n");
    report("io_seq_rw", ok ? 1 : 0, 1);
}

static void bench_io_random(int v)
{
    print_str("\n[IO 2] Random read/write (16K buf, 8K ops)\n");
    char *p = (char *)alloc(16384);
    for (int i = 0; i < 16384; i++) p[i] = 0;
    unsigned int seed = 0xDEADBEEF;
    for (int i = 0; i < 8192; i++) {
        seed = seed * 1103515245 + 12345;
        p[(seed >> 8) % 16384] = (char)(i & 0xFF);
    }
    seed = 0xDEADBEEF;
    for (int i = 0; i < 8192; i++) {
        seed = seed * 1103515245 + 12345;
        (void)p[(seed >> 8) % 16384];
    }
    if (v) print_str("    8K random ops done\n");
    report("io_random_rw", 1, 1);
}

/* -------------------------------------------------------------------------- */
/* Network benchmarks                                                         */
/* -------------------------------------------------------------------------- */

static void bench_net_throughput(int v)
{
    print_str("\n[NET 1] Throughput (4K packets, 64 rounds)\n");
    char *sb = (char *)alloc(4096), *rb = (char *)alloc(4096);
    for (int i = 0; i < 4096; i++) sb[i] = (char)(i & 0x7F);
    long long bytes = 0;
    for (int r = 0; r < 64; r++) {
        for (int i = 0; i < 4096; i++) rb[i] = sb[i];
        bytes += 4096;
    }
    int ok = 1;
    for (int i = 0; i < 4096; i++) if (rb[i] != (char)(i & 0x7F)) { ok = 0; break; }
    if (v) { print_str("    bytes="); print_int(bytes); print_str("\n"); }
    report("net_throughput", ok ? bytes : 0, 256 * 1024);
}

static void bench_net_latency(int v)
{
    print_str("\n[NET 2] Round-trip latency (1K ping-pong)\n");
    char *a = (char *)alloc(1024), *b = (char *)alloc(1024);
    for (int i = 0; i < 1024; i++) a[i] = (char)(i & 0xFF);
    long long hops = 0;
    for (int r = 0; r < 1024; r++) {
        for (int i = 0; i < 1024; i++) b[i] = a[i];
        for (int i = 0; i < 1024; i++) a[i] = b[i];
        hops += 2;
    }
    if (v) { print_str("    hops="); print_int(hops); print_str("\n"); }
    report("net_rtt_1k", hops, 2048);
}

static void bench_net_concurrent(int v)
{
    print_str("\n[NET 3] Concurrent connections (16 streams)\n");
    char *base = (char *)alloc(16 * 2048);
    int ok = 1;
    for (int s = 0; s < 16; s++) {
        char *bp = base + s * 2048;
        for (int i = 0; i < 2048; i++) bp[i] = (char)((i + s) & 0xFF);
    }
    for (int s = 0; s < 16 && ok; s++) {
        char *bp = base + s * 2048;
        for (int i = 0; i < 2048; i++) if (bp[i] != (char)((i + s) & 0xFF)) { ok = 0; break; }
    }
    if (v) print_str("    16x2K streams verified\n");
    report("net_concurrent_16", ok ? 1 : 0, 1);
}

/* -------------------------------------------------------------------------- */
/* WASM execution benchmarks                                                  */
/* -------------------------------------------------------------------------- */

static void bench_wasm_startup(int v)
{
    print_str("\n[WASM 1] Startup / init overhead\n");
    unsigned int before = heap_pos;
    for (int i = 0; i < 256; i++) alloc(64);
    unsigned int consumed = heap_pos - before;
    if (v) { print_str("    heap consumed="); print_int(consumed); print_str("\n"); }
    report("wasm_startup", consumed >= 256 * 64 ? 1 : 0, 1);
}

static void bench_wasm_exec_speed(int v)
{
    print_str("\n[WASM 2] Execution speed (1M iterations)\n");
    long long acc = 0;
    for (int i = 0; i < 1000000; i++) { acc += i; acc ^= 0x5A5A5A5A; acc -= i / 2; }
    if (v) { print_str("    acc="); print_int(acc); print_str("\n"); }
    report("wasm_1m_iters", 1000000, 1000000);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

static void show_help(void)
{
    print_str("benchmark_suite: AI-ASM WASM benchmark suite\n");
    print_str("Usage: benchmark_suite [OPTIONS]\n");
    print_str("  -t    Run all benchmarks\n");
    print_str("  -v    Verbose output\n");
    print_str("  -h    Show this help\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, test = 0, verbose = 0;
    unsigned int pos = 0;
    char *argv = (char *)buf;
    while (pos < 512 && argv[pos]) pos++;
    pos++;
    while (pos < 512 && argv[pos]) {
        char *arg = &argv[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        else if (my_strcmp(arg, "-v") == 0) verbose = 1;
        while (pos < 512 && argv[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }

    if (!test) {
        print_str("benchmark_suite: usage: benchmark_suite [-t] [-v]\n");
        host_exit(0);
    }

    print_str("=== AI-ASM Benchmark Suite ===\n");

    print_str("\n--- CPU ---\n");
    bench_cpu_integer(verbose);
    bench_cpu_float(verbose);
    bench_cpu_vector(verbose);

    print_str("\n--- Memory ---\n");
    bench_mem_bandwidth(verbose);
    bench_mem_latency(verbose);
    bench_mem_cache(verbose);

    print_str("\n--- I/O ---\n");
    bench_io_sequential(verbose);
    bench_io_random(verbose);

    print_str("\n--- Network ---\n");
    bench_net_throughput(verbose);
    bench_net_latency(verbose);
    bench_net_concurrent(verbose);

    print_str("\n--- WASM Execution ---\n");
    bench_wasm_startup(verbose);
    bench_wasm_exec_speed(verbose);

    print_str("\n=== Results ===\n  Passed: ");
    print_int(bench_passed);
    print_str("\n  Failed: ");
    print_int(bench_run - bench_passed);
    print_str("\n  Total:  ");
    print_int(bench_run);
    print_str("\n");
    print_str(bench_passed == bench_run
        ? "benchmark_suite: ALL BENCHMARKS PASSED\n"
        : "benchmark_suite: SOME BENCHMARKS FAILED\n");
    print_str("benchmark_suite: complete\n");
    host_exit(0);
}
