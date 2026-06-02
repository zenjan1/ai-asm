/* function_call: Structured function calling framework (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_SCHEMAS     64
#define MAX_CALLS       128
#define MAX_NAME        64
#define MAX_DESC        256
#define MAX_PARAMS_JSON  512
#define MAX_ARGS_JSON   256
#define MAX_OUTPUT      1024
#define MAX_ERROR       256
#define MAX_RETURNS     256

/* Status codes */
#define STATUS_PENDING    0
#define STATUS_EXECUTING  1
#define STATUS_DONE       2
#define STATUS_ERROR      3
#define STATUS_TIMEOUT    4
#define STATUS_SUCCESS    0

/* Function schema */
typedef struct {
    int   schema_id;
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    char  parameters_json[MAX_PARAMS_JSON];
    int   required_params[8];
    int   req_count;
    char  returns_schema[MAX_RETURNS];
    int   enabled;
    int   call_count;
    int   success_count;
} func_schema_t;

/* Function call */
typedef struct {
    int   call_id;
    char  name[MAX_NAME];
    char  arguments_json[MAX_ARGS_JSON];
    int   status;
    int   created_at;
    int   timeout_ms;
    int   retry_count;
} func_call_t;

/* Function result */
typedef struct {
    int   call_id;
    char  output[MAX_OUTPUT];
    char  error[MAX_ERROR];
    int   execution_time_ms;
    int   status;
    int   timestamp;
} func_result_t;

/* Function stats */
typedef struct {
    int   total_calls;
    int   success_calls;
    int   error_calls;
    int   avg_latency_ms;
    int   max_latency_ms;
    int   min_latency_ms;
} func_stats_t;

static func_schema_t schemas[MAX_SCHEMAS];
static func_call_t calls[MAX_CALLS];
static func_result_t results[MAX_CALLS];
static int schema_count = 0;
static int next_schema_id = 1;
static int call_count = 0;
static int next_call_id = 1;
static int clock = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
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

/* Schema management */
int fc_register(const char *name, const char *description, const char *params_json) {
    if (schema_count >= MAX_SCHEMAS) return -1;
    int idx = schema_count++;
    schemas[idx].schema_id = next_schema_id++;
    my_strncpy(schemas[idx].name, name, MAX_NAME - 1);
    my_strncpy(schemas[idx].description, description, MAX_DESC - 1);
    my_strncpy(schemas[idx].parameters_json, params_json ? params_json : "{}", MAX_PARAMS_JSON - 1);
    schemas[idx].req_count = 0;
    schemas[idx].returns_schema[0] = '\0';
    schemas[idx].enabled = 1;
    schemas[idx].call_count = 0;
    schemas[idx].success_count = 0;
    return schemas[idx].schema_id;
}

int fc_unregister(int schema_id) {
    for (int i = 0; i < schema_count; i++) {
        if (schemas[i].schema_id == schema_id) {
            schemas[i].enabled = 0;
            return 0;
        }
    }
    return -1;
}

func_schema_t *fc_find_schema(const char *name) {
    for (int i = 0; i < schema_count; i++) {
        if (schemas[i].enabled && my_strcmp(schemas[i].name, name) == 0) return &schemas[i];
    }
    return NULL;
}

/* Parameter validation (simple JSON key check) */
int fc_validate_args(const char *schema_json, const char *args_json) {
    if (!schema_json || !args_json) return 0;
    if (my_strlen(args_json) < 2) return 0;
    /* Basic: check args is valid JSON-like */
    if (args_json[0] != '{' || args_json[my_strlen(args_json) - 1] != '}') return 0;
    return 1;
}

/* Function call */
int fc_call(const char *name, const char *arguments_json) {
    if (call_count >= MAX_CALLS) return -1;
    int idx = call_count++;
    calls[idx].call_id = next_call_id++;
    my_strncpy(calls[idx].name, name, MAX_NAME - 1);
    my_strncpy(calls[idx].arguments_json, arguments_json ? arguments_json : "{}", MAX_ARGS_JSON - 1);
    calls[idx].status = STATUS_PENDING;
    calls[idx].created_at = ++clock;
    calls[idx].timeout_ms = 5000;
    calls[idx].retry_count = 0;
    /* Find schema and update count */
    for (int i = 0; i < schema_count; i++) {
        if (my_strcmp(schemas[i].name, name) == 0) {
            schemas[i].call_count++;
            break;
        }
    }
    /* Simulate execution */
    calls[idx].status = STATUS_EXECUTING;
    results[idx].call_id = calls[idx].call_id;
    results[idx].execution_time_ms = 10 + idx * 5;
    results[idx].timestamp = clock;
    /* Simulate result based on function name */
    if (my_strstr(name, "add") || my_strstr(name, "sum") || my_strstr(name, "calculate")) {
        my_strncpy(results[idx].output, "{\"result\": 42}", MAX_OUTPUT - 1);
        results[idx].status = STATUS_SUCCESS;
    } else if (my_strstr(name, "error") || my_strstr(name, "fail")) {
        my_strncpy(results[idx].error, "Simulated execution error", MAX_ERROR - 1);
        results[idx].status = STATUS_ERROR;
    } else {
        my_strncpy(results[idx].output, "{\"status\": \"completed\", \"data\": \"simulated_result\"}", MAX_OUTPUT - 1);
        results[idx].status = STATUS_SUCCESS;
    }
    calls[idx].status = STATUS_DONE;
    /* Update schema stats */
    for (int i = 0; i < schema_count; i++) {
        if (my_strcmp(schemas[i].name, name) == 0) {
            if (results[idx].status == STATUS_SUCCESS) schemas[i].success_count++;
            break;
        }
    }
    return calls[idx].call_id;
}

func_result_t *fc_get_result(int call_id) {
    for (int i = 0; i < call_count; i++) {
        if (calls[i].call_id == call_id) return &results[i];
    }
    return NULL;
}

/* Batch call */
int fc_batch_call(func_call_t *batch_calls, int count) {
    int executed = 0;
    for (int i = 0; i < count; i++) {
        int cid = fc_call(batch_calls[i].name, batch_calls[i].arguments_json);
        if (cid > 0) executed++;
    }
    return executed;
}

/* Execute direct (synchronous) */
int fc_execute_direct(const char *name, const char *args, char *output, int max_len) {
    int cid = fc_call(name, args);
    func_result_t *r = fc_get_result(cid);
    if (r && r->status == STATUS_SUCCESS) {
        my_strncpy(output, r->output, max_len - 1);
        return 0;
    }
    if (r) my_strncpy(output, r->error, max_len - 1);
    return -1;
}

/* Stats */
int fc_get_stats(func_stats_t *stats) {
    stats->total_calls = call_count;
    stats->success_calls = 0;
    stats->error_calls = 0;
    stats->avg_latency_ms = 0;
    stats->max_latency_ms = 0;
    stats->min_latency_ms = 999999;
    int total_lat = 0;
    for (int i = 0; i < call_count; i++) {
        if (results[i].status == STATUS_SUCCESS) stats->success_calls++;
        else stats->error_calls++;
        total_lat += results[i].execution_time_ms;
        if (results[i].execution_time_ms > stats->max_latency_ms) stats->max_latency_ms = results[i].execution_time_ms;
        if (results[i].execution_time_ms < stats->min_latency_ms) stats->min_latency_ms = results[i].execution_time_ms;
    }
    stats->avg_latency_ms = call_count > 0 ? total_lat / call_count : 0;
    return call_count;
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
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Function Call v40.0 - Structured Function Calling\n");
    if (help) { print_str("Usage: function_call [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Function Call Test ===\n\n");
        /* Register schemas */
        int s1 = fc_register("calculate", "Perform mathematical calculations", "{\"type\":\"object\",\"properties\":{\"expression\":{\"type\":\"string\"}}}");
        int s2 = fc_register("search", "Search for information", "{\"type\":\"object\",\"properties\":{\"query\":{\"type\":\"string\"}}}");
        int s3 = fc_register("get_weather", "Get weather data", "{\"type\":\"object\",\"properties\":{\"location\":{\"type\":\"string\"}}}");
        print_str("Registered "); print_int(schema_count); print_str(" function schemas\n\n");
        /* Validate args */
        int valid = fc_validate_args("{}", "{\"expression\": \"2+2\"}");
        print_str("Validation: "); print_str(valid ? "VALID" : "INVALID"); print_str("\n\n");
        /* Single call */
        int c1 = fc_call("calculate", "{\"expression\": \"2+2\"}");
        print_str("Call #"); print_int(c1); print_str(": calculate\n");
        func_result_t *r = fc_get_result(c1);
        if (r) {
            print_str("  Status: "); print_int(r->status); print_str("\n");
            print_str("  Output: "); print_str(r->output); print_str("\n");
            print_str("  Latency: "); print_int(r->execution_time_ms); print_str("ms\n");
        }
        /* Error call */
        int c2 = fc_call("error_test", "{}");
        print_str("\nCall #"); print_int(c2); print_str(": error_test\n");
        r = fc_get_result(c2);
        if (r) {
            print_str("  Status: "); print_int(r->status); print_str("\n");
            print_str("  Error: "); print_str(r->error); print_str("\n");
        }
        /* Direct execute */
        char output[MAX_OUTPUT];
        fc_execute_direct("search", "{\"query\": \"AI\"}", output, MAX_OUTPUT);
        print_str("\nDirect execute: "); print_str(output); print_str("\n");
        /* Stats */
        func_stats_t stats;
        fc_get_stats(&stats);
        print_str("\n  Statistics\n");
        print_str("  ==========================================================\n");
        print_str("  Total calls: "); print_int(stats.total_calls); print_str("\n");
        print_str("  Success: "); print_int(stats.success_calls); print_str("\n");
        print_str("  Errors: "); print_int(stats.error_calls); print_str("\n");
        print_str("  Avg latency: "); print_int(stats.avg_latency_ms); print_str("ms\n");
        print_str("\n=== Function Call Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
