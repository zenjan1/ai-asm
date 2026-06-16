/* serverless_runtime: Serverless function runtime module (v46.0)
 *
 * Provides serverless computing capabilities (AWS Lambda-style):
 * - Function deployment and invocation (cold/warm starts)
 * - Warm pool management for fast execution
 * - Multiple trigger types (HTTP, Cron, Event)
 * - Runtime environment isolation (Node.js/Python/Go/WASM)
 * - Resource quota management (CPU, memory, concurrency)
 * - Execution timeout handling
 * - Version management and alias routing
 * - Cost accounting and metrics
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
#define MAX_NAME_LEN        128
#define MAX_VERSIONS        32
#define MAX_ALIASES         16
#define MAX_TRIGGERS        64
#define MAX_WARM_POOL       32
#define MAX_LOG_ENTRIES     512

/* Runtime environments */
#define RUNTIME_NODEJS      0
#define RUNTIME_PYTHON      1
#define RUNTIME_GO          2
#define RUNTIME_WASM        3

/* Function states */
#define FUNC_DEPLOYED       0
#define FUNC_ACTIVE         1
#define FUNC_INACTIVE       2
#define FUNC_FAILED         3

/* Trigger types */
#define TRIGGER_HTTP        0
#define TRIGGER_CRON        1
#define TRIGGER_EVENT       2

/* Instance states */
#define INSTANCE_COLD       0
#define INSTANCE_WARM       1
#define INSTANCE_RUNNING    2
#define INSTANCE_IDLE       3

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* ===== Data Structures ===== */

typedef struct {
    int             func_id;
    char            name[MAX_NAME_LEN];
    int             runtime;
    char            handler[256];
    unsigned int    memory_mb;
    unsigned int    timeout_ms;
    int             state;
    int             version_count;
    unsigned int    created_at;
} function_t;

typedef struct {
    int             version_id;
    int             func_id;
    char            code_hash[64];
    unsigned int    size_kb;
    unsigned int    deployed_at;
} version_t;

typedef struct {
    int             alias_id;
    char            name[64];
    int             func_id;
    int             version_id;
    int             weight;         /* for canary deployment (0-100) */
} alias_t;

typedef struct {
    int             trigger_id;
    int             type;
    int             func_id;
    char            endpoint[256];  /* HTTP path, cron expr, or event pattern */
    int             active;
} trigger_t;

typedef struct {
    int             instance_id;
    int             func_id;
    int             state;
    unsigned int    start_time;
    unsigned int    last_used;
} instance_t;

typedef struct {
    int             log_id;
    int             func_id;
    int             instance_id;
    char            message[256];
    int             level;          /* 0=info, 1=warn, 2=error */
    unsigned int    timestamp;
} log_entry_t;

typedef struct {
    unsigned int    total_invocations;
    unsigned int    cold_starts;
    unsigned int    warm_starts;
    unsigned int    errors;
    unsigned int    total_duration_ms;
    unsigned int    total_cost_micros;
} metrics_t;

/* Global state */
static function_t functions[MAX_FUNCTIONS];
static int function_count = 0;

static version_t versions[MAX_FUNCTIONS * MAX_VERSIONS];
static int version_count = 0;

static alias_t aliases[MAX_FUNCTIONS * MAX_ALIASES];
static int alias_count = 0;

static trigger_t triggers[MAX_TRIGGERS];
static int trigger_count = 0;

static instance_t warm_pool[MAX_WARM_POOL];
static int warm_pool_size = 0;

static log_entry_t logs[MAX_LOG_ENTRIES];
static int log_count = 0;

static metrics_t metrics[MAX_FUNCTIONS];

static unsigned int clock = 0;

/* ===== Function Management ===== */

static int function_deploy(const char *name, int runtime, const char *handler,
                           unsigned int memory_mb, unsigned int timeout_ms) {
    if (function_count >= MAX_FUNCTIONS) return -1;

    int idx = function_count++;
    int fid = idx;
    functions[idx].func_id = fid;
    my_strncpy(functions[idx].name, name, MAX_NAME_LEN - 1);
    functions[idx].runtime = runtime;
    my_strncpy(functions[idx].handler, handler, 255);
    functions[idx].memory_mb = memory_mb;
    functions[idx].timeout_ms = timeout_ms;
    functions[idx].state = FUNC_DEPLOYED;
    functions[idx].version_count = 0;
    functions[idx].created_at = ++clock;

    metrics[fid].total_invocations = 0;
    metrics[fid].cold_starts = 0;
    metrics[fid].warm_starts = 0;
    metrics[fid].errors = 0;
    metrics[fid].total_duration_ms = 0;
    metrics[fid].total_cost_micros = 0;

    print_str("Deployed function: ");
    print_str(name);
    print_str("\n");
    print_str("  Runtime: ");
    if (runtime == RUNTIME_NODEJS) print_str("Node.js");
    else if (runtime == RUNTIME_PYTHON) print_str("Python");
    else if (runtime == RUNTIME_GO) print_str("Go");
    else print_str("WASM");
    print_str("\n");
    print_str("  Handler: ");
    print_str(handler);
    print_str("\n");
    print_str("  Memory: ");
    print_int(memory_mb);
    print_str(" MB, Timeout: ");
    print_int(timeout_ms);
    print_str(" ms\n");

    return fid;
}

static int function_invoke_cold(int func_id) {
    if (func_id < 0 || func_id >= function_count) return -1;
    if (functions[func_id].state == FUNC_INACTIVE) return -1;

    functions[func_id].state = FUNC_ACTIVE;
    metrics[func_id].cold_starts++;
    metrics[func_id].total_invocations++;

    print_str("Cold start invoked: ");
    print_str(functions[func_id].name);
    print_str("\n");

    return 0;
}

static int function_invoke_warm(int func_id) {
    if (func_id < 0 || func_id >= function_count) return -1;
    if (functions[func_id].state != FUNC_ACTIVE) return -1;

    metrics[func_id].warm_starts++;
    metrics[func_id].total_invocations++;

    print_str("Warm invoked: ");
    print_str(functions[func_id].name);
    print_str("\n");

    return 0;
}

static int function_invoke(int func_id) {
    /* Try warm pool first, fallback to cold */
    for (int i = 0; i < warm_pool_size; i++) {
        if (warm_pool[i].func_id == func_id && warm_pool[i].state == INSTANCE_IDLE) {
            warm_pool[i].state = INSTANCE_RUNNING;
            warm_pool[i].last_used = ++clock;
            return function_invoke_warm(func_id);
        }
    }
    return function_invoke_cold(func_id);
}

/* ===== Warm Pool Management ===== */

static int warm_pool_init(int size) {
    warm_pool_size = size < MAX_WARM_POOL ? size : MAX_WARM_POOL;
    for (int i = 0; i < warm_pool_size; i++) {
        warm_pool[i].instance_id = i;
        warm_pool[i].func_id = -1;
        warm_pool[i].state = INSTANCE_COLD;
        warm_pool[i].start_time = 0;
        warm_pool[i].last_used = 0;
    }
    print_str("Warm pool initialized: ");
    print_int(warm_pool_size);
    print_str(" instances\n");
    return 0;
}

static int warm_pool_acquire(int func_id) {
    for (int i = 0; i < warm_pool_size; i++) {
        if (warm_pool[i].state == INSTANCE_COLD || warm_pool[i].state == INSTANCE_IDLE) {
            warm_pool[i].func_id = func_id;
            warm_pool[i].state = INSTANCE_WARM;
            warm_pool[i].start_time = ++clock;
            print_str("Acquired warm instance ");
            print_int(i);
            print_str(" for function ");
            print_int(func_id);
            print_str("\n");
            return i;
        }
    }
    print_str("No warm instances available\n");
    return -1;
}

static int warm_pool_release(int instance_id) {
    if (instance_id < 0 || instance_id >= warm_pool_size) return -1;
    warm_pool[instance_id].state = INSTANCE_IDLE;
    warm_pool[instance_id].last_used = ++clock;
    print_str("Released warm instance ");
    print_int(instance_id);
    print_str("\n");
    return 0;
}

/* ===== Trigger Binding ===== */

static int trigger_bind_http(int func_id, const char *path) {
    if (trigger_count >= MAX_TRIGGERS) return -1;
    int idx = trigger_count++;
    triggers[idx].trigger_id = idx;
    triggers[idx].type = TRIGGER_HTTP;
    triggers[idx].func_id = func_id;
    my_strncpy(triggers[idx].endpoint, path, 255);
    triggers[idx].active = 1;

    print_str("Bound HTTP trigger: ");
    print_str(path);
    print_str(" -> function ");
    print_int(func_id);
    print_str("\n");
    return idx;
}

static int trigger_bind_cron(int func_id, const char *expression) {
    if (trigger_count >= MAX_TRIGGERS) return -1;
    int idx = trigger_count++;
    triggers[idx].trigger_id = idx;
    triggers[idx].type = TRIGGER_CRON;
    triggers[idx].func_id = func_id;
    my_strncpy(triggers[idx].endpoint, expression, 255);
    triggers[idx].active = 1;

    print_str("Bound Cron trigger: ");
    print_str(expression);
    print_str(" -> function ");
    print_int(func_id);
    print_str("\n");
    return idx;
}

static int trigger_bind_event(int func_id, const char *pattern) {
    if (trigger_count >= MAX_TRIGGERS) return -1;
    int idx = trigger_count++;
    triggers[idx].trigger_id = idx;
    triggers[idx].type = TRIGGER_EVENT;
    triggers[idx].func_id = func_id;
    my_strncpy(triggers[idx].endpoint, pattern, 255);
    triggers[idx].active = 1;

    print_str("Bound Event trigger: ");
    print_str(pattern);
    print_str(" -> function ");
    print_int(func_id);
    print_str("\n");
    return idx;
}

/* ===== Version Management ===== */

static int version_create(int func_id, const char *code_hash, unsigned int size_kb) {
    if (version_count >= MAX_FUNCTIONS * MAX_VERSIONS) return -1;
    if (func_id < 0 || func_id >= function_count) return -1;

    int idx = version_count++;
    versions[idx].version_id = idx;
    versions[idx].func_id = func_id;
    my_strncpy(versions[idx].code_hash, code_hash, 63);
    versions[idx].size_kb = size_kb;
    versions[idx].deployed_at = ++clock;

    functions[func_id].version_count++;

    print_str("Created version ");
    print_int(idx);
    print_str(" for function ");
    print_str(functions[func_id].name);
    print_str("\n");
    print_str("  Hash: ");
    print_str(code_hash);
    print_str(", Size: ");
    print_int(size_kb);
    print_str(" KB\n");
    return idx;
}

static void version_list(int func_id) {
    print_str("Versions for function ");
    print_str(functions[func_id].name);
    print_str(":\n");
    for (int i = 0; i < version_count; i++) {
        if (versions[i].func_id == func_id) {
            print_str("  v");
            print_int(versions[i].version_id);
            print_str(": ");
            print_str(versions[i].code_hash);
            print_str(" (");
            print_int(versions[i].size_kb);
            print_str(" KB)\n");
        }
    }
}

static int alias_create(const char *name, int func_id, int version_id, int weight) {
    if (alias_count >= MAX_FUNCTIONS * MAX_ALIASES) return -1;

    int idx = alias_count++;
    aliases[idx].alias_id = idx;
    my_strncpy(aliases[idx].name, name, 63);
    aliases[idx].func_id = func_id;
    aliases[idx].version_id = version_id;
    aliases[idx].weight = weight;

    print_str("Created alias: ");
    print_str(name);
    print_str(" -> v");
    print_int(version_id);
    print_str(" (weight: ");
    print_int(weight);
    print_str("%)\n");
    return idx;
}

static int alias_route(const char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (my_strlen(aliases[i].name) == my_strlen(name)) {
            int match = 1;
            for (int j = 0; j < my_strlen(name); j++) {
                if (aliases[i].name[j] != name[j]) { match = 0; break; }
            }
            if (match) {
                print_str("Routed alias ");
                print_str(name);
                print_str(" -> function ");
                print_int(aliases[i].func_id);
                print_str(" v");
                print_int(aliases[i].version_id);
                print_str("\n");
                return aliases[i].version_id;
            }
        }
    }
    print_str("Alias not found: ");
    print_str(name);
    print_str("\n");
    return -1;
}

/* ===== Resource Quota Management ===== */

static int quota_check_cpu(int func_id, unsigned int requested_shares) {
    unsigned int limit = 1024;
    if (requested_shares > limit) {
        print_str("CPU quota exceeded for function ");
        print_str(functions[func_id].name);
        print_str("\n");
        return -1;
    }
    print_str("CPU quota OK: ");
    print_int(requested_shares);
    print_str(" shares\n");
    return 0;
}

static int quota_check_memory(int func_id, unsigned int requested_mb) {
    unsigned int limit = 2048;
    if (requested_mb > limit) {
        print_str("Memory quota exceeded for function ");
        print_str(functions[func_id].name);
        print_str("\n");
        return -1;
    }
    print_str("Memory quota OK: ");
    print_int(requested_mb);
    print_str(" MB\n");
    return 0;
}

static int quota_check_concurrency(int func_id, unsigned int current_count) {
    unsigned int limit = 100;
    if (current_count >= limit) {
        print_str("Concurrency limit reached for function ");
        print_str(functions[func_id].name);
        print_str("\n");
        return -1;
    }
    print_str("Concurrency OK: ");
    print_int(current_count);
    print_str("/");
    print_int(limit);
    print_str("\n");
    return 0;
}

/* ===== Timeout Management ===== */

static int timeout_start(int func_id) {
    print_str("Started timeout timer for function ");
    print_str(functions[func_id].name);
    print_str(": ");
    print_int(functions[func_id].timeout_ms);
    print_str(" ms\n");
    return 0;
}

static int timeout_check(int func_id, unsigned int elapsed_ms) {
    if (elapsed_ms > functions[func_id].timeout_ms) {
        print_str("TIMEOUT: Function ");
        print_str(functions[func_id].name);
        print_str(" exceeded ");
        print_int(functions[func_id].timeout_ms);
        print_str(" ms\n");
        metrics[func_id].errors++;
        return -1;
    }
    print_str("Timeout check OK: ");
    print_int(elapsed_ms);
    print_str("/");
    print_int(functions[func_id].timeout_ms);
    print_str(" ms\n");
    return 0;
}

static int timeout_cancel(int func_id) {
    print_str("Cancelled timeout for function ");
    print_str(functions[func_id].name);
    print_str("\n");
    return 0;
}

/* ===== Cost Accounting ===== */

static int cost_account(int func_id, unsigned int duration_ms, unsigned int memory_mb) {
    unsigned int cost_per_ms = 1;
    unsigned int cost = duration_ms * memory_mb * cost_per_ms / 1000;
    metrics[func_id].total_cost_micros += cost;
    metrics[func_id].total_duration_ms += duration_ms;

    print_str("Cost accounted: function ");
    print_str(functions[func_id].name);
    print_str(", ");
    print_int(duration_ms);
    print_str(" ms x ");
    print_int(memory_mb);
    print_str(" MB = ");
    print_int(cost);
    print_str(" micro-units\n");
    return 0;
}

static void cost_report(void) {
    print_str("=== Cost Report ===\n");
    unsigned int total_cost = 0;
    for (int i = 0; i < function_count; i++) {
        print_str("  ");
        print_str(functions[i].name);
        print_str(": ");
        print_int(metrics[i].total_cost_micros);
        print_str(" micro-units (");
        print_int(metrics[i].total_invocations);
        print_str(" invocations)\n");
        total_cost += metrics[i].total_cost_micros;
    }
    print_str("Total: ");
    print_int(total_cost);
    print_str(" micro-units\n");
}

/* ===== Logging & Monitoring ===== */

static int log_execution(int func_id, int instance_id, const char *message) {
    if (log_count >= MAX_LOG_ENTRIES) return -1;
    int idx = log_count++;
    logs[idx].log_id = idx;
    logs[idx].func_id = func_id;
    logs[idx].instance_id = instance_id;
    my_strncpy(logs[idx].message, message, 255);
    logs[idx].level = 0;
    logs[idx].timestamp = ++clock;
    return 0;
}

static int error_track(int func_id, const char *error_msg) {
    if (log_count >= MAX_LOG_ENTRIES) return -1;
    int idx = log_count++;
    logs[idx].log_id = idx;
    logs[idx].func_id = func_id;
    logs[idx].instance_id = -1;
    my_strncpy(logs[idx].message, error_msg, 255);
    logs[idx].level = 2;
    logs[idx].timestamp = ++clock;
    metrics[func_id].errors++;

    print_str("ERROR tracked: ");
    print_str(error_msg);
    print_str("\n");
    return 0;
}

static void metrics_record(int func_id, unsigned int duration_ms) {
    metrics[func_id].total_duration_ms += duration_ms;
}

/* ===== Test Suite ===== */

static void test_suite(void) {
    print_str("=== Serverless Runtime Test Suite ===\n\n");

    /* Test 1: Function Deployment */
    print_str("[Test 1] Function Deployment\n");
    int f1 = function_deploy("api-handler", RUNTIME_NODEJS, "index.handler", 256, 3000);
    int f2 = function_deploy("data-processor", RUNTIME_PYTHON, "process.main", 512, 6000);
    int f3 = function_deploy("edge-compute", RUNTIME_WASM, "compute.run", 128, 1000);
    print_str("\n");

    /* Test 2: Version Creation */
    print_str("[Test 2] Version Management\n");
    int v1 = version_create(f1, "abc123def456", 1024);
    int v2 = version_create(f1, "def789ghi012", 1050);
    int v3 = version_create(f2, "xyz987uvw654", 2048);
    version_list(f1);
    print_str("\n");

    /* Test 3: Alias Creation & Routing */
    print_str("[Test 3] Alias Routing\n");
    alias_create("prod", f1, v1, 100);
    alias_create("canary", f1, v2, 10);
    alias_create("stable", f2, v3, 100);
    alias_route("prod");
    alias_route("canary");
    alias_route("stable");
    print_str("\n");

    /* Test 4: Trigger Binding */
    print_str("[Test 4] Trigger Binding\n");
    trigger_bind_http(f1, "/api/v1/users");
    trigger_bind_http(f1, "/api/v1/orders");
    trigger_bind_cron(f2, "0 */6 * * *");
    trigger_bind_event(f3, "s3:ObjectCreated:*");
    print_str("\n");

    /* Test 5: Warm Pool Management */
    print_str("[Test 5] Warm Pool Management\n");
    warm_pool_init(8);
    int inst1 = warm_pool_acquire(f1);
    int inst2 = warm_pool_acquire(f2);
    warm_pool_release(inst1);
    print_str("\n");

    /* Test 6: Resource Quota Checks */
    print_str("[Test 6] Resource Quota Checks\n");
    quota_check_cpu(f1, 512);
    quota_check_memory(f1, 256);
    quota_check_concurrency(f1, 10);
    quota_check_cpu(f2, 2048);
    print_str("\n");

    /* Test 7: Function Invocation (Cold & Warm) */
    print_str("[Test 7] Function Invocation\n");
    function_invoke(f1);
    warm_pool_acquire(f1);
    function_invoke(f1);
    function_invoke(f2);
    print_str("\n");

    /* Test 8: Timeout Handling */
    print_str("[Test 8] Timeout Handling\n");
    timeout_start(f1);
    timeout_check(f1, 1500);
    timeout_check(f1, 3500);
    timeout_cancel(f1);
    print_str("\n");

    /* Test 9: Cost Accounting */
    print_str("[Test 9] Cost Accounting\n");
    cost_account(f1, 150, 256);
    cost_account(f1, 200, 256);
    cost_account(f2, 500, 512);
    cost_account(f3, 50, 128);
    print_str("\n");

    /* Test 10: Logging & Error Tracking */
    print_str("[Test 10] Logging & Monitoring\n");
    log_execution(f1, 0, "Function initialized");
    log_execution(f1, 0, "Processing request");
    error_track(f2, "Out of memory");
    metrics_record(f1, 100);
    print_str("\n");

    /* Final Report */
    print_str("[Report] Cost Summary\n");
    cost_report();
    print_str("\n=== Serverless Runtime Test Complete ===\n");
}

/* ===== CLI Entry ===== */

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

    print_str("Serverless Runtime v46.0 - Lambda-Style Function Platform\n");
    if (help) {
        print_str("Usage: serverless_runtime [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run test suite\n");
        return;
    }

    if (test) {
        test_suite();
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
