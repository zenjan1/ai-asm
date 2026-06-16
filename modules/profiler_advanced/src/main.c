/* profiler_advanced: Advanced profiler module (v46.0)
 *
 * Provides comprehensive profiling capabilities:
 * - Call graph analysis (caller/callee tracking)
 * - Hotspot function detection (time-based ranking)
 * - Memory allocation tracking (leak detection)
 * - Lock contention analysis (wait time tracking)
 * - Performance bottleneck visualization (text bars)
 */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_FUNCTIONS       256
#define MAX_CALL_EDGES      1024
#define MAX_ALLOCATIONS     4096
#define MAX_LOCKS           128
#define MAX_NAME_LEN        64

/* Call graph edge */
typedef struct {
    int     caller_idx;
    int     callee_idx;
    int     call_count;
    int     active;
} call_edge_t;

/* Tracked function */
typedef struct {
    char            name[MAX_NAME_LEN];
    int             call_count;
    unsigned int    total_time_us;
    unsigned int    self_time_us;
    int             alloc_count;
    unsigned int    alloc_bytes;
    int             lock_acquisitions;
    int             active;
} func_info_t;

/* Memory allocation record */
typedef struct {
    unsigned int    address;
    unsigned int    size;
    int             func_idx;
    int             freed;
    unsigned int    alloc_time;
    unsigned int    free_time;
} alloc_record_t;

/* Lock contention record */
typedef struct {
    char            name[MAX_NAME_LEN];
    int             acquire_count;
    int             contention_count;
    unsigned int    total_wait_us;
    unsigned int    max_wait_us;
    int             holder_func_idx;
    int             active;
} lock_info_t;

/* Global state */
static func_info_t     functions[MAX_FUNCTIONS];
static int             func_count = 0;
static call_edge_t     edges[MAX_CALL_EDGES];
static int             edge_count = 0;
static alloc_record_t  allocations[MAX_ALLOCATIONS];
static int             alloc_count = 0;
static lock_info_t     locks[MAX_LOCKS];
static int             lock_count = 0;
static unsigned int    sim_time = 0;

/* ===== String utilities ===== */

static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }

static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

static void print_bar(unsigned int value, unsigned int max_val) {
    int bar_len = 0;
    if (max_val > 0) bar_len = (int)((value * 40) / max_val);
    if (bar_len > 40) bar_len = 40;
    print_str("[");
    for (int i = 0; i < bar_len; i++) print_str("#");
    for (int i = bar_len; i < 40; i++) print_str(" ");
    print_str("] ");
    print_int((int)value);
    print_str("\n");
}

/* ===== Function tracking ===== */

static int func_find_or_create(const char *name) {
    for (int i = 0; i < func_count; i++) {
        if (functions[i].active) {
            int same = 1;
            for (int j = 0; j < MAX_NAME_LEN; j++) {
                if (functions[i].name[j] != name[j]) { same = 0; break; }
                if (name[j] == '\0') break;
            }
            if (same) return i;
        }
    }
    if (func_count >= MAX_FUNCTIONS) return -1;
    int idx = func_count++;
    my_strncpy(functions[idx].name, name, MAX_NAME_LEN - 1);
    functions[idx].active = 1;
    return idx;
}

void profiler_record_call(const char *func_name, unsigned int time_us) {
    int idx = func_find_or_create(func_name);
    if (idx < 0) return;
    functions[idx].call_count++;
    functions[idx].total_time_us += time_us;
    functions[idx].self_time_us += time_us;
    sim_time += time_us;
}

void profiler_record_edge(const char *caller, const char *callee) {
    int c1 = func_find_or_create(caller);
    int c2 = func_find_or_create(callee);
    if (c1 < 0 || c2 < 0) return;

    for (int i = 0; i < edge_count; i++) {
        if (edges[i].caller_idx == c1 && edges[i].callee_idx == c2) {
            edges[i].call_count++;
            return;
        }
    }
    if (edge_count >= MAX_CALL_EDGES) return;
    int idx = edge_count++;
    edges[idx].caller_idx = c1;
    edges[idx].callee_idx = c2;
    edges[idx].call_count = 1;
    edges[idx].active = 1;
}

/* ===== Hotspot detection ===== */

void profiler_find_hotspots(int top_n) {
    int visited[MAX_FUNCTIONS];
    for (int v = 0; v < MAX_FUNCTIONS; v++) visited[v] = 0;

    print_str("Hotspot Functions (top ");
    print_int(top_n);
    print_str("):\n");

    for (int rank = 0; rank < top_n; rank++) {
        int best = -1;
        unsigned int best_time = 0;
        for (int i = 0; i < func_count; i++) {
            if (!visited[i] && functions[i].active &&
                functions[i].total_time_us > best_time) {
                best_time = functions[i].total_time_us;
                best = i;
            }
        }
        if (best < 0) break;
        visited[best] = 1;

        print_str("  ");
        print_int(rank + 1);
        print_str(". ");
        print_str(functions[best].name);
        print_str(" - ");
        print_int((int)functions[best].total_time_us);
        print_str("us (");
        print_int(functions[best].call_count);
        print_str(" calls)\n");
    }
}

/* ===== Memory allocation tracking ===== */

void profiler_record_alloc(unsigned int addr, unsigned int size, const char *func_name) {
    if (alloc_count >= MAX_ALLOCATIONS) return;
    int fidx = func_find_or_create(func_name);
    int idx = alloc_count++;
    allocations[idx].address = addr;
    allocations[idx].size = size;
    allocations[idx].func_idx = fidx;
    allocations[idx].freed = 0;
    allocations[idx].alloc_time = sim_time;

    if (fidx >= 0) {
        functions[fidx].alloc_count++;
        functions[fidx].alloc_bytes += size;
    }
}

void profiler_record_free(unsigned int addr) {
    for (int i = 0; i < alloc_count; i++) {
        if (allocations[i].address == addr && !allocations[i].freed) {
            allocations[i].freed = 1;
            allocations[i].free_time = sim_time;
            return;
        }
    }
}

void profiler_detect_leaks(void) {
    print_str("Memory Leak Detection:\n");
    int leak_count = 0;
    unsigned int leak_bytes = 0;

    for (int i = 0; i < alloc_count; i++) {
        if (!allocations[i].freed) {
            leak_count++;
            leak_bytes += allocations[i].size;
            print_str("  LEAK: addr=0x");
            print_int((int)allocations[i].size);
            print_str(" bytes at func=");
            if (allocations[i].func_idx >= 0) {
                print_str(functions[allocations[i].func_idx].name);
            }
            print_str("\n");
        }
    }

    print_str("  Total leaks: ");
    print_int(leak_count);
    print_str(" (");
    print_int((int)leak_bytes);
    print_str(" bytes)\n");
}

/* ===== Lock contention analysis ===== */

int profiler_create_lock(const char *name) {
    if (lock_count >= MAX_LOCKS) return -1;
    int idx = lock_count++;
    my_strncpy(locks[idx].name, name, MAX_NAME_LEN - 1);
    locks[idx].active = 1;
    return idx;
}

void profiler_record_lock_acquire(int lock_idx, int func_idx, unsigned int wait_us, int contended) {
    if (lock_idx < 0 || lock_idx >= lock_count) return;
    locks[lock_idx].acquire_count++;
    locks[lock_idx].total_wait_us += wait_us;
    locks[lock_idx].holder_func_idx = func_idx;
    if (wait_us > locks[lock_idx].max_wait_us) {
        locks[lock_idx].max_wait_us = wait_us;
    }
    if (contended) locks[lock_idx].contention_count++;
    if (func_idx >= 0) functions[func_idx].lock_acquisitions++;
}

void profiler_analyze_contention(void) {
    print_str("Lock Contention Analysis:\n");
    for (int i = 0; i < lock_count; i++) {
        if (!locks[i].active) continue;
        print_str("  Lock: ");
        print_str(locks[i].name);
        print_str("\n");
        print_str("    Acquisitions: ");
        print_int(locks[i].acquire_count);
        print_str("\n");
        print_str("    Contention:   ");
        print_int(locks[i].contention_count);
        print_str("\n");
        print_str("    Total wait:   ");
        print_int((int)locks[i].total_wait_us);
        print_str("us\n");
        print_str("    Max wait:     ");
        print_int((int)locks[i].max_wait_us);
        print_str("us\n");
    }
}

/* ===== Call graph analysis ===== */

void profiler_dump_callgraph(void) {
    print_str("Call Graph:\n");
    for (int i = 0; i < edge_count; i++) {
        if (!edges[i].active) continue;
        print_str("  ");
        print_str(functions[edges[i].caller_idx].name);
        print_str(" -> ");
        print_str(functions[edges[i].callee_idx].name);
        print_str(" (");
        print_int(edges[i].call_count);
        print_str(" calls)\n");
    }
}

/* ===== Bottleneck visualization ===== */

void profiler_visualize_bottlenecks(void) {
    print_str("Performance Bottleneck Visualization:\n\n");

    unsigned int max_time = 0;
    for (int i = 0; i < func_count; i++) {
        if (functions[i].active && functions[i].total_time_us > max_time) {
            max_time = functions[i].total_time_us;
        }
    }

    print_str("Function Time Distribution:\n");
    for (int i = 0; i < func_count; i++) {
        if (!functions[i].active) continue;
        print_str("  ");
        print_str(functions[i].name);
        print_str(" ");
        print_bar(functions[i].total_time_us, max_time);
    }

    print_str("\nMemory Allocation by Function:\n");
    unsigned int max_bytes = 0;
    for (int i = 0; i < func_count; i++) {
        if (functions[i].active && functions[i].alloc_bytes > max_bytes) {
            max_bytes = functions[i].alloc_bytes;
        }
    }
    for (int i = 0; i < func_count; i++) {
        if (!functions[i].active || functions[i].alloc_bytes == 0) continue;
        print_str("  ");
        print_str(functions[i].name);
        print_str(" ");
        print_bar(functions[i].alloc_bytes, max_bytes);
    }

    print_str("\nLock Wait Time:\n");
    unsigned int max_wait = 0;
    for (int i = 0; i < lock_count; i++) {
        if (locks[i].active && locks[i].total_wait_us > max_wait) {
            max_wait = locks[i].total_wait_us;
        }
    }
    for (int i = 0; i < lock_count; i++) {
        if (!locks[i].active) continue;
        print_str("  ");
        print_str(locks[i].name);
        print_str(" ");
        print_bar(locks[i].total_wait_us, max_wait);
    }
}

/* ===== Summary ===== */

void profiler_print_summary(void) {
    print_str("Profiler Summary:\n");
    int active_funcs = 0;
    unsigned int total_calls = 0;
    for (int i = 0; i < func_count; i++) {
        if (functions[i].active) {
            active_funcs++;
            total_calls += (unsigned int)functions[i].call_count;
        }
    }
    print_str("  Functions tracked:    ");
    print_int(active_funcs);
    print_str("\n");
    print_str("  Total calls:          ");
    print_int((int)total_calls);
    print_str("\n");
    print_str("  Call graph edges:     ");
    print_int(edge_count);
    print_str("\n");
    print_str("  Allocations tracked:  ");
    print_int(alloc_count);
    print_str("\n");
    print_str("  Locks tracked:        ");
    print_int(lock_count);
    print_str("\n");
    print_str("  Simulation time:      ");
    print_int((int)sim_time);
    print_str("us\n");
}

/* ===== CLI entry ===== */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Profiler Advanced v46.0 - Advanced Performance Profiler\n");
    if (help) {
        print_str("Usage: profiler_advanced [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run profiler test suite\n");
        return;
    }

    if (test) {
        print_str("=== Profiler Advanced Test Suite ===\n\n");

        /* Test 1: Record function calls */
        print_str("[Test 1] Function Call Recording\n");
        profiler_record_call("main",          50000);
        profiler_record_call("parse_config",  12000);
        profiler_record_call("parse_config",  8500);
        profiler_record_call("init_db",       25000);
        profiler_record_call("process_request", 35000);
        profiler_record_call("process_request", 42000);
        profiler_record_call("process_request", 38000);
        profiler_record_call("render_output", 18000);
        profiler_record_call("render_output", 22000);
        profiler_record_call("compress_data", 65000);
        profiler_record_call("compress_data", 72000);
        profiler_record_call("validate_input", 5000);
        profiler_record_call("validate_input", 4500);
        profiler_record_call("validate_input", 5200);
        profiler_record_call("log_event",     3000);
        profiler_record_call("log_event",     2800);
        print_str("  Recorded ");
        print_int(func_count);
        print_str(" unique functions\n\n");

        /* Test 2: Call graph */
        print_str("[Test 2] Call Graph Construction\n");
        profiler_record_edge("main", "parse_config");
        profiler_record_edge("main", "init_db");
        profiler_record_edge("main", "validate_input");
        profiler_record_edge("main", "process_request");
        profiler_record_edge("process_request", "validate_input");
        profiler_record_edge("process_request", "compress_data");
        profiler_record_edge("process_request", "render_output");
        profiler_record_edge("process_request", "log_event");
        profiler_record_edge("main", "render_output");
        profiler_record_edge("main", "log_event");
        profiler_dump_callgraph();
        print_str("\n");

        /* Test 3: Hotspot detection */
        print_str("[Test 3] Hotspot Detection\n");
        profiler_find_hotspots(5);
        print_str("\n");

        /* Test 4: Memory allocation tracking */
        print_str("[Test 4] Memory Allocation Tracking\n");
        profiler_record_alloc(0x10001000, 1024,  "parse_config");
        profiler_record_alloc(0x10002000, 4096,  "init_db");
        profiler_record_alloc(0x10003000, 8192,  "process_request");
        profiler_record_alloc(0x10004000, 2048,  "process_request");
        profiler_record_alloc(0x10005000, 16384, "compress_data");
        profiler_record_alloc(0x10006000, 512,   "render_output");
        profiler_record_alloc(0x10007000, 256,   "log_event");
        profiler_record_alloc(0x10008000, 32768, "compress_data");
        profiler_record_alloc(0x10009000, 64,    "validate_input");
        /* Free some allocations */
        profiler_record_free(0x10001000);
        profiler_record_free(0x10002000);
        profiler_record_free(0x10006000);
        profiler_record_free(0x10007000);
        profiler_record_free(0x10009000);
        print_str("  Tracked ");
        print_int(alloc_count);
        print_str(" allocations, 5 freed\n\n");

        /* Test 5: Leak detection */
        print_str("[Test 5] Memory Leak Detection\n");
        profiler_detect_leaks();
        print_str("\n");

        /* Test 6: Lock creation and contention */
        print_str("[Test 6] Lock Contention Analysis\n");
        int db_lock    = profiler_create_lock("db_mutex");
        int cache_lock = profiler_create_lock("cache_mutex");
        int log_lock   = profiler_create_lock("log_mutex");
        int req_lock   = profiler_create_lock("request_queue");

        profiler_record_lock_acquire(db_lock,    3, 150, 0);   /* init_db */
        profiler_record_lock_acquire(db_lock,    3, 200, 1);
        profiler_record_lock_acquire(db_lock,    6, 3500, 1);  /* process_request */
        profiler_record_lock_acquire(db_lock,    6, 800, 0);
        profiler_record_lock_acquire(cache_lock, 6, 50,  0);
        profiler_record_lock_acquire(cache_lock, 6, 75,  0);
        profiler_record_lock_acquire(cache_lock, 6, 60,  0);
        profiler_record_lock_acquire(log_lock,   6, 30,  0);
        profiler_record_lock_acquire(log_lock,   14, 20, 0);   /* log_event */
        profiler_record_lock_acquire(log_lock,   14, 25, 0);
        profiler_record_lock_acquire(req_lock,   0, 500, 1);   /* main */
        profiler_record_lock_acquire(req_lock,   6, 1200, 1);  /* process_request */
        profiler_record_lock_acquire(req_lock,   6, 900, 1);
        profiler_analyze_contention();
        print_str("\n");

        /* Test 7: Bottleneck visualization */
        print_str("[Test 7] Bottleneck Visualization\n\n");
        profiler_visualize_bottlenecks();
        print_str("\n");

        /* Test 8: Summary */
        print_str("[Test 8] Profiler Summary\n");
        profiler_print_summary();
        print_str("\n");

        print_str("=== Profiler Advanced Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
