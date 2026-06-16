/*
 * aiasm-aarch64/modules/test_harness/src/main.c
 * Test harness for AI-ASM v3.0 WASM runtime
 *
 * Covers: Unit test generation, integration test suite,
 *         stress testing tools, regression test automation.
 *
 * Usage: test_harness [-t] [-v] [-s] [-r] [-h]
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

/* -------------------------------------------------------------------------- */
/* Test tracking                                                              */
/* -------------------------------------------------------------------------- */

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total  = 0;

static void report(const char *name, int pass)
{
    tests_total++;
    if (pass) tests_passed++; else tests_failed++;
    print_str("  ");
    print_str(name);
    print_str(pass ? "  PASS\n" : "  FAIL\n");
}

/* -------------------------------------------------------------------------- */
/* Unit test generator                                                        */
/* -------------------------------------------------------------------------- */

static void unit_test_arithmetic(int v)
{
    print_str("\n[UNIT 1] Arithmetic operations\n");
    int a = 17, b = 5;
    int sum = a + b, diff = a - b, prod = a * b, quot = a / b, rem = a % b;
    if (v) {
        print_str("    "); print_int(a); print_str(" + "); print_int(b);
        print_str(" = "); print_int(sum); print_str("\n");
        print_str("    "); print_int(a); print_str(" - "); print_int(b);
        print_str(" = "); print_int(diff); print_str("\n");
        print_str("    "); print_int(a); print_str(" * "); print_int(b);
        print_str(" = "); print_int(prod); print_str("\n");
        print_str("    "); print_int(a); print_str(" / "); print_int(b);
        print_str(" = "); print_int(quot); print_str("\n");
    }
    report("add", sum == 22);
    report("sub", diff == 12);
    report("mul", prod == 85);
    report("div", quot == 3);
    report("mod", rem == 2);
}

static void unit_test_bitwise(int v)
{
    print_str("\n[UNIT 2] Bitwise operations\n");
    unsigned int x = 0xFF00, y = 0x0FF0;
    unsigned int and_r = x & y, or_r = x | y, xor_r = x ^ y, not_x = ~x & 0xFFFF;
    unsigned int shl = (x >> 4), shr = (x << 4) & 0xFFFF;
    if (v) {
        print_str("    AND="); print_int(and_r);
        print_str(" OR="); print_int(or_r);
        print_str(" XOR="); print_int(xor_r);
        print_str(" NOT="); print_int(not_x);
        print_str("\n");
    }
    report("bit_and", and_r == 0x0F00);
    report("bit_or",  or_r  == 0xFFF0);
    report("bit_xor", xor_r == 0xF0F0);
    report("bit_not", not_x == 0x00FF);
    report("shift_l", shl == 0x0FF0);
    report("shift_r", shr == 0xFF00);
}

static void unit_test_array_ops(int v)
{
    print_str("\n[UNIT 3] Array operations (sort + search)\n");
    int arr[16] = {9,3,7,1,5,8,2,10,4,6,15,12,11,13,14,0};
    /* Bubble sort */
    for (int i = 0; i < 16; i++)
        for (int j = i + 1; j < 16; j++)
            if (arr[i] > arr[j]) { int t = arr[i]; arr[i] = arr[j]; arr[j] = t; }
    int sorted_ok = 1;
    for (int i = 0; i < 16; i++) if (arr[i] != i) { sorted_ok = 0; break; }
    /* Binary search for 11 */
    int lo = 0, hi = 15, found = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == 11) { found = mid; break; }
        else if (arr[mid] < 11) lo = mid + 1;
        else hi = mid - 1;
    }
    if (v) { print_str("    sorted="); print_int(sorted_ok); print_str(" idx_11="); print_int(found); print_str("\n"); }
    report("array_sort", sorted_ok);
    report("array_bsearch", found == 11);
}

static void unit_test_string_ops(int v)
{
    print_str("\n[UNIT 4] String operations\n");
    char buf[64];
    const char *src = "Hello, WASM!";
    int i = 0;
    for (; src[i]; i++) buf[i] = src[i];
    buf[i] = '\0';
    int len_match = (my_strlen(buf) == 12);
    int cmp_match = (my_strcmp(buf, "Hello, WASM!") == 0);
    int cmp_diff  = (my_strcmp(buf, "hello, wasm!") != 0);
    if (v) { print_str("    len="); print_int(my_strlen(buf)); print_str(" cmp="); print_int(cmp_match); print_str("\n"); }
    report("str_copy", len_match);
    report("str_cmp_eq", cmp_match);
    report("str_cmp_ne", cmp_diff);
}

/* -------------------------------------------------------------------------- */
/* Integration test suite                                                     */
/* -------------------------------------------------------------------------- */

static void integ_test_pipeline(int v)
{
    print_str("\n[INTEG 1] Data pipeline (produce -> transform -> consume)\n");
    int *data = (int *)alloc(256 * 4);
    /* Produce */
    for (int i = 0; i < 256; i++) data[i] = i * 3 + 1;
    /* Transform: square and accumulate */
    long long acc = 0;
    for (int i = 0; i < 256; i++) { data[i] = data[i] * data[i]; acc += data[i]; }
    /* Consume: verify sum */
    long long expected = 0;
    for (int i = 0; i < 256; i++) { long long v = i * 3 + 1; expected += v * v; }
    if (v) { print_str("    acc="); print_int(acc); print_str(" expected="); print_int(expected); print_str("\n"); }
    report("pipeline_end_to_end", acc == expected);
}

static void integ_test_multi_stage(int v)
{
    print_str("\n[INTEG 2] Multi-stage filter (map -> filter -> reduce)\n");
    int *src = (int *)alloc(128 * 4);
    int *dst = (int *)alloc(128 * 4);
    for (int i = 0; i < 128; i++) src[i] = i + 1;
    /* Map: double each element; Filter: keep only those divisible by 3 */
    int count = 0;
    for (int i = 0; i < 128; i++) {
        int mapped = src[i] * 2;
        if (mapped % 3 == 0) dst[count++] = mapped;
    }
    /* Reduce: sum */
    long long sum = 0;
    for (int i = 0; i < count; i++) sum += dst[i];
    if (v) { print_str("    count="); print_int(count); print_str(" sum="); print_int(sum); print_str("\n"); }
    report("map_filter_reduce", count > 0 && sum > 0);
}

static void integ_test_shared_state(int v)
{
    print_str("\n[INTEG 3] Shared state (producer/consumer via buffer)\n");
    char *ring = (char *)alloc(64);
    int head = 0, tail = 0, capacity = 64;
    int produced = 0, consumed = 0;
    /* Producer: write 100 bytes */
    for (int i = 0; i < 100; i++) {
        int next = (head + 1) % capacity;
        if (next != tail) {
            ring[head] = (char)(i & 0x7F);
            head = next;
            produced++;
        }
    }
    /* Consumer: read all available */
    while (tail != head) {
        (void)ring[tail];
        tail = (tail + 1) % capacity;
        consumed++;
    }
    if (v) { print_str("    produced="); print_int(produced); print_str(" consumed="); print_int(consumed); print_str("\n"); }
    report("ring_buffer", produced == consumed && produced > 0);
}

static void integ_test_error_propagation(int v)
{
    print_str("\n[INTEG 4] Error propagation across layers\n");
    /* Layer 1: data source (may fail) */
    int layer1_ok = 1;
    /* Layer 2: processor */
    int layer2_ok = layer1_ok ? 1 : 0;
    /* Layer 3: output */
    int layer3_ok = layer2_ok ? 1 : 0;
    /* Simulate a failure in layer 1 */
    layer1_ok = 0;
    layer2_ok = layer1_ok ? 1 : 0;
    layer3_ok = layer2_ok ? 1 : 0;
    int error_propagated = (!layer1_ok && !layer2_ok && !layer3_ok);
    if (v) { print_str("    L1="); print_int(layer1_ok); print_str(" L2="); print_int(layer2_ok); print_str(" L3="); print_int(layer3_ok); print_str("\n"); }
    report("error_propagation", error_propagated);
}

/* -------------------------------------------------------------------------- */
/* Stress testing tools                                                       */
/* -------------------------------------------------------------------------- */

static void stress_alloc(int v)
{
    print_str("\n[STRESS 1] Allocation pressure (512 allocs, 64B each)\n");
    int ok = 1;
    for (int i = 0; i < 512; i++) {
        unsigned int p = alloc(64);
        if (p == 0) { ok = 0; break; }
        char *ptr = (char *)p;
        for (int j = 0; j < 64; j++) ptr[j] = (char)(i ^ j);
    }
    if (v) print_str("    512 allocations completed\n");
    report("alloc_pressure", ok);
}

static void stress_computation(int v)
{
    print_str("\n[STRESS 2] Computation intensity (500K mixed ops)\n");
    long long acc = 0;
    unsigned int seed = 0xCAFEBABE;
    for (int i = 0; i < 500000; i++) {
        seed = seed * 1103515245 + 12345;
        acc += (long long)(seed >> 8);
        acc ^= (long long)i;
        if (acc < 0) acc = -acc;
        acc %= 1000000007LL;
    }
    if (v) { print_str("    acc="); print_int(acc); print_str("\n"); }
    report("compute_500k", 1);
}

static void stress_memory_traverse(int v)
{
    print_str("\n[STRESS 3] Memory traversal (8K node linked list)\n");
    int *nodes = (int *)alloc(8192 * 4);
    /* Build shuffled linked list */
    unsigned int seed = 0x98765;
    for (int i = 0; i < 8192; i++) nodes[i] = i;
    for (int i = 8191; i > 0; i--) {
        seed = seed * 1103515245 + 12345;
        int j = (seed >> 8) % (i + 1);
        int t = nodes[i]; nodes[i] = nodes[j]; nodes[j] = t;
    }
    /* Traverse */
    long long hops = 0, cur = 0;
    for (int pass = 0; pass < 4; pass++) {
        cur = nodes[0];
        for (int step = 0; step < 8191; step++) { cur = nodes[cur]; hops++; }
    }
    if (v) { print_str("    hops="); print_int(hops); print_str("\n"); }
    report("linked_traverse_8k", hops == 8191 * 4);
}

static void stress_io_simulation(int v)
{
    print_str("\n[STRESS 4] I/O simulation (32K buffer, 128 passes)\n");
    char *buf = (char *)alloc(32768);
    for (int pass = 0; pass < 128; pass++) {
        for (int i = 0; i < 32768; i++) buf[i] = (char)((i + pass) & 0xFF);
    }
    int ok = 1;
    for (int i = 0; i < 32768; i++) {
        if (buf[i] != (char)((i + 127) & 0xFF)) { ok = 0; break; }
    }
    if (v) print_str("    4MB total I/O completed\n");
    report("io_stress_4mb", ok);
}

/* -------------------------------------------------------------------------- */
/* Regression test automation                                                 */
/* -------------------------------------------------------------------------- */

static void regression_test_math_identity(int v)
{
    print_str("\n[REG 1] Math identities (a+b=b+a, a*b=b*a, etc.)\n");
    int ok = 1;
    int vals[8] = {0, 1, -1, 42, -42, 1000, -999, 12345};
    for (int i = 0; i < 8 && ok; i++)
        for (int j = 0; j < 8 && ok; j++) {
            int a = vals[i], b = vals[j];
            if (a + b != b + a) ok = 0;
            if (a * b != b * a) ok = 0;
            if (a - b != -(b - a)) ok = 0;
        }
    if (v) print_str("    64 identity checks done\n");
    report("math_commutativity", ok);
}

static void regression_test_boundary(int v)
{
    print_str("\n[REG 2] Boundary values (0, MAX, MIN, overflow)\n");
    int ok = 1;
    /* Integer boundaries */
    int max_int = 0x7FFFFFFF, min_int = 0x80000000;
    if (max_int + 1 != min_int) ok = 0;  /* overflow wraps */
    if (min_int - 1 != max_int) ok = 0;  /* underflow wraps */
    /* Zero division avoidance */
    int div_by_1 = 42 / 1;
    if (div_by_1 != 42) ok = 0;
    /* Shift boundaries */
    unsigned int one = 1;
    if ((one << 31) != 0x80000000u) ok = 0;
    if ((one << 0) != 1) ok = 0;
    if (v) { print_str("    max="); print_int(max_int); print_str(" min="); print_int(min_int); print_str("\n"); }
    report("boundary_values", ok);
}

static void regression_test_determinism(int v)
{
    print_str("\n[REG 3] Deterministic execution (3 runs, same result)\n");
    long long results[3];
    for (int r = 0; r < 3; r++) {
        long long acc = 0;
        unsigned int seed = 0x42;
        for (int i = 0; i < 10000; i++) {
            seed = seed * 1103515245 + 12345;
            acc += (long long)(seed >> 8) % 997;
        }
        results[r] = acc;
    }
    int det = (results[0] == results[1] && results[1] == results[2]);
    if (v) { print_str("    r0="); print_int(results[0]); print_str(" r1="); print_int(results[1]); print_str(" r2="); print_int(results[2]); print_str("\n"); }
    report("determinism_3runs", det);
}

static void regression_test_state_preservation(int v)
{
    print_str("\n[REG 4] State preservation (save / restore / verify)\n");
    int *state = (int *)alloc(64 * 4);
    int *backup = (int *)alloc(64 * 4);
    /* Initialize state */
    for (int i = 0; i < 64; i++) state[i] = i * 7 + 3;
    /* Save */
    for (int i = 0; i < 64; i++) backup[i] = state[i];
    /* Mutate */
    for (int i = 0; i < 64; i++) state[i] = state[i] ^ 0xDEAD;
    /* Restore */
    for (int i = 0; i < 64; i++) state[i] = backup[i];
    /* Verify */
    int ok = 1;
    for (int i = 0; i < 64; i++) if (state[i] != i * 7 + 3) { ok = 0; break; }
    if (v) print_str("    64 ints saved, mutated, restored\n");
    report("state_save_restore", ok);
}

static void regression_test_idempotency(int v)
{
    print_str("\n[REG 5] Idempotency (f(f(x)) == f(x))\n");
    int ok = 1;
    int test_vals[6] = {0, 1, -1, 100, -50, 999};
    for (int t = 0; t < 6 && ok; t++) {
        int x = test_vals[t];
        /* f(x) = clamp(x, 0, 100) */
        int fx = x < 0 ? 0 : (x > 100 ? 100 : x);
        int ffx = fx < 0 ? 0 : (fx > 100 ? 100 : fx);
        if (ffx != fx) ok = 0;
    }
    if (v) print_str("    6 idempotency checks done\n");
    report("idempotency_clamp", ok);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

static void show_help(void)
{
    print_str("test_harness: AI-ASM WASM test harness\n");
    print_str("Usage: test_harness [OPTIONS]\n");
    print_str("  -t    Run all test suites\n");
    print_str("  -v    Verbose output\n");
    print_str("  -s    Stress tests only\n");
    print_str("  -r    Regression tests only\n");
    print_str("  -h    Show this help\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, test = 0, verbose = 0, stress_only = 0, regression_only = 0;
    unsigned int pos = 0;
    char *argv = (char *)buf;
    while (pos < 512 && argv[pos]) pos++;
    pos++;
    while (pos < 512 && argv[pos]) {
        char *arg = &argv[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        else if (my_strcmp(arg, "-v") == 0) verbose = 1;
        else if (my_strcmp(arg, "-s") == 0) stress_only = 1;
        else if (my_strcmp(arg, "-r") == 0) regression_only = 1;
        while (pos < 512 && argv[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }

    if (!test && !stress_only && !regression_only) {
        print_str("test_harness: usage: test_harness [-t] [-v] [-s] [-r]\n");
        host_exit(0);
    }

    print_str("=== AI-ASM Test Harness ===\n");

    int run_all = test;

    if (run_all || (!stress_only && !regression_only)) {
        print_str("\n--- Unit Tests ---\n");
        unit_test_arithmetic(verbose);
        unit_test_bitwise(verbose);
        unit_test_array_ops(verbose);
        unit_test_string_ops(verbose);
    }

    if (run_all || (!stress_only && !regression_only)) {
        print_str("\n--- Integration Tests ---\n");
        integ_test_pipeline(verbose);
        integ_test_multi_stage(verbose);
        integ_test_shared_state(verbose);
        integ_test_error_propagation(verbose);
    }

    if (run_all || stress_only) {
        print_str("\n--- Stress Tests ---\n");
        stress_alloc(verbose);
        stress_computation(verbose);
        stress_memory_traverse(verbose);
        stress_io_simulation(verbose);
    }

    if (run_all || regression_only) {
        print_str("\n--- Regression Tests ---\n");
        regression_test_math_identity(verbose);
        regression_test_boundary(verbose);
        regression_test_determinism(verbose);
        regression_test_state_preservation(verbose);
        regression_test_idempotency(verbose);
    }

    print_str("\n=== Results ===\n  Passed: ");
    print_int(tests_passed);
    print_str("\n  Failed: ");
    print_int(tests_failed);
    print_str("\n  Total:  ");
    print_int(tests_total);
    print_str("\n");
    print_str(tests_failed == 0
        ? "test_harness: ALL TESTS PASSED\n"
        : "test_harness: SOME TESTS FAILED\n");
    print_str("test_harness: complete\n");
    host_exit(0);
}
