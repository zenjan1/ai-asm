/* serverless_framework: Serverless/FaaS platform (v1.0)
 *
 * Implements: Function lifecycle, triggers (HTTP/schedule/queue/storage),
 * BaaS (database/storage/auth), auto-scaling, cold start optimization,
 * observability (tracing/metrics/logs).
 */
#include <stddef.h>

/* Host function imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_FUNCTIONS 64
#define MAX_TRIGGERS 128
#define MAX_DATABASES 32
#define MAX_BUCKETS 32
#define MAX_AUTH_PROVIDERS 16
#define MAX_TRACES 256
#define MAX_LOGS 512

/* Trigger types */
#define TRIGGER_HTTP 0x01
#define TRIGGER_SCHEDULE 0x02
#define TRIGGER_QUEUE 0x03
#define TRIGGER_STORAGE 0x04

/* Structures */
typedef struct {
    char name[64];
    char handler[128];
    unsigned int timeout_ms;
    unsigned int memory_mb;
    unsigned int max_concurrency;
    unsigned int min_instances;
    unsigned int current_instances;
    unsigned int warm_pool_size;
    unsigned int total_invocations;
    unsigned int total_errors;
    unsigned long long total_duration_ms;
    int cold_start_enabled;
    int warm_pool_enabled;
} sls_function_t;

typedef struct {
    unsigned int id;
    char function_name[64];
    int type;
    char config[128];
    int active;
} sls_trigger_t;

typedef struct {
    char name[64];
    unsigned int record_count;
    unsigned long long storage_bytes;
} sls_database_t;

typedef struct {
    char name[64];
    unsigned int file_count;
    unsigned long long total_bytes;
} sls_bucket_t;

typedef struct {
    char provider[32];
    unsigned int active_sessions;
    unsigned int total_users;
} sls_auth_t;

typedef struct {
    char function_name[64];
    unsigned long long duration_ms;
    int success;
} sls_trace_t;

typedef struct {
    char function_name[64];
    char level[16];
    char message[128];
} sls_log_t;

/* Global state */
static sls_function_t functions[MAX_FUNCTIONS];
static unsigned int function_count = 0;

static sls_trigger_t triggers[MAX_TRIGGERS];
static unsigned int trigger_count = 0;

static sls_database_t databases[MAX_DATABASES];
static unsigned int database_count = 0;

static sls_bucket_t buckets[MAX_BUCKETS];
static unsigned int bucket_count = 0;

static sls_auth_t auth_providers[MAX_AUTH_PROVIDERS];
static unsigned int auth_count = 0;

static sls_trace_t traces[MAX_TRACES];
static unsigned int trace_count = 0;

static sls_log_t logs[MAX_LOGS];
static unsigned int log_count = 0;

static int initialized = 0;

/* Utility: print integer */
static void print_int(int val) {
    char buf[32];
    int i = 0;
    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
        if (neg) buf[i++] = '-';
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

/* Utility: print string */
static void print_str(const char* s) {
    host_print(s);
}

/* Initialize framework */
int sls_init(void) {
    if (initialized) return -1;

    for (int i = 0; i < MAX_FUNCTIONS; i++) {
        functions[i].name[0] = '\0';
        functions[i].handler[0] = '\0';
    }

    function_count = 0;
    trigger_count = 0;
    database_count = 0;
    bucket_count = 0;
    auth_count = 0;
    trace_count = 0;
    log_count = 0;

    initialized = 1;
    print_str("[SLS] Serverless framework initialized\n");
    return 0;
}

/* Create function */
int sls_create_function(const char* name, const char* handler,
                        unsigned int timeout_ms, unsigned int memory_mb) {
    if (!initialized || function_count >= MAX_FUNCTIONS) return -1;

    sls_function_t* f = &functions[function_count];

    /* Copy name */
    int i = 0;
    while (name[i] && i < 63) { f->name[i] = name[i]; i++; }
    f->name[i] = '\0';

    /* Copy handler */
    i = 0;
    while (handler[i] && i < 127) { f->handler[i] = handler[i]; i++; }
    f->handler[i] = '\0';

    f->timeout_ms = timeout_ms;
    f->memory_mb = memory_mb;
    f->max_concurrency = 100;
    f->min_instances = 0;
    f->current_instances = 0;
    f->warm_pool_size = 0;
    f->total_invocations = 0;
    f->total_errors = 0;
    f->total_duration_ms = 0;
    f->cold_start_enabled = 1;
    f->warm_pool_enabled = 0;

    function_count++;

    print_str("[SLS] Created function: ");
    print_str(name);
    print_str(" (handler=");
    print_str(handler);
    print_str(", timeout=");
    print_int(timeout_ms);
    print_str("ms, memory=");
    print_int(memory_mb);
    print_str("MB)\n");

    return function_count - 1;
}

/* Configure auto-scaling */
int sls_configure_autoscaling(int func_id, unsigned int min_instances,
                              unsigned int max_concurrency, int warm_pool) {
    if (func_id < 0 || func_id >= (int)function_count) return -1;

    sls_function_t* f = &functions[func_id];
    f->min_instances = min_instances;
    f->max_concurrency = max_concurrency;
    f->warm_pool_enabled = warm_pool;

    if (warm_pool) {
        f->warm_pool_size = min_instances;
        f->current_instances = min_instances;
    }

    print_str("[SLS] Auto-scaling configured for ");
    print_str(f->name);
    print_str(": min=");
    print_int(min_instances);
    print_str(", max=");
    print_int(max_concurrency);
    print_str(", warm_pool=");
    print_int(warm_pool);
    print_str("\n");

    return 0;
}

/* Invoke function */
int sls_invoke_function(int func_id) {
    if (!initialized || func_id < 0 || func_id >= (int)function_count) return -1;

    sls_function_t* f = &functions[func_id];

    /* Record trace */
    sls_trace_t* trace = &traces[trace_count % MAX_TRACES];
    int i = 0;
    while (f->name[i] && i < 63) { trace->function_name[i] = f->name[i]; i++; }
    trace->function_name[i] = '\0';

    /* Simulate cold start */
    int cold_start = 0;
    if (f->current_instances == 0 && f->cold_start_enabled) {
        cold_start = 1;
        f->current_instances = 1;
    }

    /* Check concurrency */
    if (f->current_instances >= f->max_concurrency) {
        trace->success = 0;
        trace->duration_ms = 0;
        trace_count++;
        f->total_errors++;
        print_str("[SLS] Function ");
        print_str(f->name);
        print_str(" throttled (concurrency limit)\n");
        return -2;
    }

    /* Simulate execution */
    unsigned long long exec_duration = 10; /* ms */
    if (cold_start) exec_duration += 150;

    /* Scale down if no warm pool */
    if (!f->warm_pool_enabled) {
        f->current_instances = 0;
    }

    /* Update stats */
    f->total_invocations++;
    f->total_duration_ms += exec_duration;

    trace->success = 1;
    trace->duration_ms = exec_duration;
    trace_count++;

    /* Log invocation */
    sls_log_t* log = &logs[log_count % MAX_LOGS];
    i = 0;
    while (f->name[i] && i < 63) { log->function_name[i] = f->name[i]; i++; }
    log->function_name[i] = '\0';
    log->level[0] = 'I'; log->level[1] = 'N'; log->level[2] = 'F'; log->level[3] = 'O'; log->level[4] = '\0';
    const char* msg = cold_start ? "Cold start invocation" : "Warm invocation";
    i = 0;
    while (msg[i] && i < 127) { log->message[i] = msg[i]; i++; }
    log->message[i] = '\0';
    log_count++;

    print_str("[SLS] Invoked ");
    print_str(f->name);
    print_str(" (duration=");
    print_int((int)exec_duration);
    print_str("ms, cold_start=");
    print_int(cold_start);
    print_str(")\n");

    return 0;
}

/* Create HTTP trigger */
int sls_create_http_trigger(const char* function_name, const char* path, const char* method) {
    if (trigger_count >= MAX_TRIGGERS) return -1;

    sls_trigger_t* t = &triggers[trigger_count];
    t->id = trigger_count;

    int i = 0;
    while (function_name[i] && i < 63) { t->function_name[i] = function_name[i]; i++; }
    t->function_name[i] = '\0';

    t->type = TRIGGER_HTTP;

    i = 0;
    const char* cfg = "HTTP trigger";
    while (cfg[i] && i < 127) { t->config[i] = cfg[i]; i++; }
    t->config[i] = '\0';

    t->active = 1;
    trigger_count++;

    print_str("[SLS] Created HTTP trigger for ");
    print_str(function_name);
    print_str(" (");
    print_str(method);
    print_str(" ");
    print_str(path);
    print_str(")\n");

    return trigger_count - 1;
}

/* Create schedule trigger */
int sls_create_schedule_trigger(const char* function_name, const char* cron) {
    if (trigger_count >= MAX_TRIGGERS) return -1;

    sls_trigger_t* t = &triggers[trigger_count];
    t->id = trigger_count;

    int i = 0;
    while (function_name[i] && i < 63) { t->function_name[i] = function_name[i]; i++; }
    t->function_name[i] = '\0';

    t->type = TRIGGER_SCHEDULE;

    i = 0;
    while (cron[i] && i < 127) { t->config[i] = cron[i]; i++; }
    t->config[i] = '\0';

    t->active = 1;
    trigger_count++;

    print_str("[SLS] Created schedule trigger for ");
    print_str(function_name);
    print_str(" (cron=");
    print_str(cron);
    print_str(")\n");

    return trigger_count - 1;
}

/* Create BaaS database */
int baas_create_database(const char* name) {
    if (database_count >= MAX_DATABASES) return -1;

    sls_database_t* db = &databases[database_count];

    int i = 0;
    while (name[i] && i < 63) { db->name[i] = name[i]; i++; }
    db->name[i] = '\0';

    db->record_count = 0;
    db->storage_bytes = 0;

    database_count++;

    print_str("[BaaS] Created database: ");
    print_str(name);
    print_str("\n");

    return database_count - 1;
}

/* Insert record into database */
int baas_insert_record(int db_id, unsigned long long record_size) {
    if (db_id < 0 || db_id >= (int)database_count) return -1;

    databases[db_id].record_count++;
    databases[db_id].storage_bytes += record_size;

    return 0;
}

/* Create storage bucket */
int baas_create_bucket(const char* name) {
    if (bucket_count >= MAX_BUCKETS) return -1;

    sls_bucket_t* b = &buckets[bucket_count];

    int i = 0;
    while (name[i] && i < 63) { b->name[i] = name[i]; i++; }
    b->name[i] = '\0';

    b->file_count = 0;
    b->total_bytes = 0;

    bucket_count++;

    print_str("[BaaS] Created storage bucket: ");
    print_str(name);
    print_str("\n");

    return bucket_count - 1;
}

/* Upload file to bucket */
int baas_upload_file(int bucket_id, unsigned long long file_size) {
    if (bucket_id < 0 || bucket_id >= (int)bucket_count) return -1;

    buckets[bucket_id].file_count++;
    buckets[bucket_id].total_bytes += file_size;

    return 0;
}

/* Create auth provider */
int baas_create_auth_provider(const char* provider) {
    if (auth_count >= MAX_AUTH_PROVIDERS) return -1;

    sls_auth_t* a = &auth_providers[auth_count];

    int i = 0;
    while (provider[i] && i < 31) { a->provider[i] = provider[i]; i++; }
    a->provider[i] = '\0';

    a->active_sessions = 0;
    a->total_users = 0;

    auth_count++;

    print_str("[BaaS] Created auth provider: ");
    print_str(provider);
    print_str("\n");

    return auth_count - 1;
}

/* Print statistics */
void sls_print_statistics(void) {
    print_str("\n========================================\n");
    print_str("  Serverless Framework Statistics\n");
    print_str("========================================\n\n");

    print_str("Functions: ");
    print_int(function_count);
    print_str("\n");

    for (unsigned int i = 0; i < function_count; i++) {
        sls_function_t* f = &functions[i];
        print_str("  - ");
        print_str(f->name);
        print_str("\n");
        print_str("    Handler: ");
        print_str(f->handler);
        print_str("\n");
        print_str("    Invocations: ");
        print_int(f->total_invocations);
        print_str("\n");
        print_str("    Errors: ");
        print_int(f->total_errors);
        print_str("\n");
        if (f->total_invocations > 0) {
            unsigned long long avg = f->total_duration_ms / f->total_invocations;
            print_str("    Avg Duration: ");
            print_int((int)avg);
            print_str(" ms\n");
        }
        print_str("    Instances: ");
        print_int(f->current_instances);
        print_str("/");
        print_int(f->max_concurrency);
        print_str("\n");
    }

    print_str("\nTriggers: ");
    print_int(trigger_count);
    print_str("\n");

    unsigned int http = 0, schedule = 0, queue = 0, storage = 0;
    for (unsigned int i = 0; i < trigger_count; i++) {
        if (triggers[i].type == TRIGGER_HTTP) http++;
        else if (triggers[i].type == TRIGGER_SCHEDULE) schedule++;
        else if (triggers[i].type == TRIGGER_QUEUE) queue++;
        else if (triggers[i].type == TRIGGER_STORAGE) storage++;
    }
    print_str("  HTTP: ");
    print_int(http);
    print_str(", Schedule: ");
    print_int(schedule);
    print_str(", Queue: ");
    print_int(queue);
    print_str(", Storage: ");
    print_int(storage);
    print_str("\n");

    print_str("\nBaaS Services:\n");
    print_str("  Databases: ");
    print_int(database_count);
    print_str("\n");
    for (unsigned int i = 0; i < database_count; i++) {
        print_str("    - ");
        print_str(databases[i].name);
        print_str(": ");
        print_int(databases[i].record_count);
        print_str(" records\n");
    }

    print_str("  Storage Buckets: ");
    print_int(bucket_count);
    print_str("\n");
    for (unsigned int i = 0; i < bucket_count; i++) {
        print_str("    - ");
        print_str(buckets[i].name);
        print_str(": ");
        print_int(buckets[i].file_count);
        print_str(" files\n");
    }

    print_str("  Auth Providers: ");
    print_int(auth_count);
    print_str("\n");

    print_str("\nObservability:\n");
    print_str("  Traces: ");
    print_int(trace_count);
    print_str("\n");
    print_str("  Logs: ");
    print_int(log_count);
    print_str("\n");

    print_str("\n========================================\n");
}

/* Main entry point */
int main(void) {
    print_str("=== Serverless Framework Demo ===\n\n");

    /* Initialize */
    sls_init();

    /* Create functions */
    int f1 = sls_create_function("hello-world", "index.handler", 5000, 128);
    int f2 = sls_create_function("data-processor", "process.main", 30000, 512);
    int f3 = sls_create_function("image-resizer", "resize.handler", 10000, 256);

    /* Configure auto-scaling */
    sls_configure_autoscaling(f1, 1, 50, 1); /* Warm pool enabled */
    sls_configure_autoscaling(f2, 0, 100, 0); /* Scale to zero */
    sls_configure_autoscaling(f3, 2, 200, 1); /* Warm pool */

    /* Create triggers */
    sls_create_http_trigger("hello-world", "/api/hello", "GET");
    sls_create_http_trigger("data-processor", "/api/process", "POST");
    sls_create_schedule_trigger("data-processor", "0 */5 * * * *"); /* Every 5 min */
    sls_create_http_trigger("image-resizer", "/api/resize", "POST");

    /* Create BaaS services */
    int db1 = baas_create_database("user-data");
    int db2 = baas_create_database("analytics");

    baas_insert_record(db1, 1024);
    baas_insert_record(db1, 2048);
    baas_insert_record(db2, 512);
    baas_insert_record(db2, 768);
    baas_insert_record(db2, 1536);

    int bucket1 = baas_create_bucket("images");
    int bucket2 = baas_create_bucket("backups");

    baas_upload_file(bucket1, 1048576); /* 1MB */
    baas_upload_file(bucket1, 2097152); /* 2MB */
    baas_upload_file(bucket2, 10485760); /* 10MB */

    baas_create_auth_provider("google");
    baas_create_auth_provider("github");
    baas_create_auth_provider("email");

    /* Invoke functions */
    print_str("\n--- Invoking Functions ---\n");
    sls_invoke_function(f1);
    sls_invoke_function(f1); /* Second call should be warm */
    sls_invoke_function(f2);
    sls_invoke_function(f3);
    sls_invoke_function(f3);
    sls_invoke_function(f3);

    /* Print statistics */
    sls_print_statistics();

    print_str("\n=== Demo Complete ===\n");

    return 0;
}
