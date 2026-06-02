/* metrics: Prometheus-style monitoring metrics (v12.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Metric types */
#define METRIC_COUNTER    0
#define METRIC_GAUGE      1
#define METRIC_HISTOGRAM  2

/* Limits */
#define MAX_METRICS     64
#define METRIC_NAME_LEN 64
#define METRIC_LABELS_LEN 256
#define METRIC_HISTORY   128

/* Metric entry */
typedef struct {
    char  name[METRIC_NAME_LEN];
    int   type;
    char  labels[METRIC_LABELS_LEN];
    int   value_int;       /* integer value */
    int   value_frac;      /* fractional part (for decimal display) */
    int   timestamp;
    int   sample_count;
    int   min_val;
    int   max_val;
} metric_t;

/* Metric sample history */
typedef struct {
    char  name[METRIC_NAME_LEN];
    int   value;
    int   timestamp;
} metric_sample_t;

static metric_t metrics[MAX_METRICS];
static metric_sample_t history[METRIC_HISTORY];
static int metric_count = 0;
static int history_count = 0;
static int next_sample_id = 1;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* Register a new metric
   name: metric name, type: COUNTER, GAUGE, or HISTOGRAM
   Returns: 0=success, -1=failure */
int metrics_register(const char *name, int type) {
    if (!name || metric_count >= MAX_METRICS) {
        print_str("[metrics] cannot register metric\n");
        return -1;
    }

    /* Check for duplicate */
    for (int i = 0; i < metric_count; i++) {
        if (my_strcmp(metrics[i].name, name) == 0) {
            print_str("[metrics] metric already exists: ");
            print_str(name);
            print_str("\n");
            return -1;
        }
    }

    int idx = metric_count;
    my_strncpy(metrics[idx].name, name, METRIC_NAME_LEN - 1);
    metrics[idx].type = type;
    metrics[idx].labels[0] = '\0';
    metrics[idx].value_int = 0;
    metrics[idx].value_frac = 0;
    metrics[idx].timestamp = 0;
    metrics[idx].sample_count = 0;
    metrics[idx].min_val = 2147483647;
    metrics[idx].max_val = -2147483647;

    metric_count++;

    const char *type_name = type == METRIC_COUNTER ? "counter" :
                           type == METRIC_GAUGE ? "gauge" : "histogram";

    print_str("[metrics] registered: ");
    print_str(name);
    print_str(" (");
    print_str(type_name);
    print_str(")\n");

    return 0;
}

/* Record a metric value
   name: metric name, value: value to record, labels: optional labels
   Returns: 0=success, -1=failure */
int metrics_record(const char *name, int value, const char *labels) {
    int idx = -1;
    for (int i = 0; i < metric_count; i++) {
        if (my_strcmp(metrics[i].name, name) == 0) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        print_str("[metrics] unknown metric: ");
        print_str(name);
        print_str("\n");
        return -1;
    }

    /* Update value */
    metrics[idx].value_int = value;

    if (labels && labels[0]) {
        my_strncpy(metrics[idx].labels, labels, METRIC_LABELS_LEN - 1);
    }

    /* Update stats */
    metrics[idx].sample_count++;
    if (value < metrics[idx].min_val) metrics[idx].min_val = value;
    if (value > metrics[idx].max_val) metrics[idx].max_val = value;

    /* For counters, always increment */
    if (metrics[idx].type == METRIC_COUNTER) {
        /* value is the increment amount */
    }

    /* Record in history */
    if (history_count < METRIC_HISTORY) {
        int hidx = history_count;
        my_strncpy(history[hidx].name, name, METRIC_NAME_LEN - 1);
        history[hidx].value = value;
        history[hidx].timestamp = next_sample_id++;
        history_count++;
    }

    return 0;
}

/* Export metrics in Prometheus format
   buf: output buffer, len: max length
   Returns: bytes written */
int metrics_export_prometheus(char *buf, int len) {
    int pos = 0;

    for (int i = 0; i < metric_count && pos < len - 80; i++) {
        const char *type = metrics[i].type == METRIC_COUNTER ? "counter" :
                          metrics[i].type == METRIC_GAUGE ? "gauge" : "histogram";

        /* TYPE comment */
        buf[pos++] = '#';
        buf[pos++] = ' ';
        buf[pos++] = 'T'; buf[pos++] = 'Y'; buf[pos++] = 'P'; buf[pos++] = 'E';
        buf[pos++] = ' ';
        for (int j = 0; metrics[i].name[j] && pos < len; j++)
            buf[pos++] = metrics[i].name[j];
        buf[pos++] = ' ';
        for (int j = 0; type[j] && pos < len; j++)
            buf[pos++] = type[j];
        buf[pos++] = '\n';

        /* HELP comment */
        buf[pos++] = '#';
        buf[pos++] = ' ';
        buf[pos++] = 'H'; buf[pos++] = 'E'; buf[pos++] = 'L'; buf[pos++] = 'P';
        buf[pos++] = ' ';
        for (int j = 0; metrics[i].name[j] && pos < len; j++)
            buf[pos++] = metrics[i].name[j];
        buf[pos++] = ' ';
        buf[pos++] = 'A'; buf[pos++] = 'I'; buf[pos++] = '-'; buf[pos++] = 'A';
        buf[pos++] = 'S'; buf[pos++] = 'M'; buf[pos++] = ' ';
        buf[pos++] = 'm'; buf[pos++] = 'e'; buf[pos++] = 't'; buf[pos++] = 'r';
        buf[pos++] = 'i'; buf[pos++] = 'c'; buf[pos++] = '\n';

        /* Metric value with labels */
        for (int j = 0; metrics[i].name[j] && pos < len; j++)
            buf[pos++] = metrics[i].name[j];

        if (metrics[i].labels[0]) {
            buf[pos++] = '{';
            for (int j = 0; metrics[i].labels[j] && pos < len; j++)
                buf[pos++] = metrics[i].labels[j];
            buf[pos++] = '}';
        }

        buf[pos++] = ' ';

        /* Write value */
        int val = metrics[i].value_int;
        if (val < 0) { buf[pos++] = '-'; val = -val; }
        char tmp[16];
        int td = 0;
        if (val == 0) tmp[td++] = '0';
        else while (val > 0) { tmp[td++] = '0' + (val % 10); val /= 10; }
        while (td > 0 && pos < len) buf[pos++] = tmp[--td];

        buf[pos++] = ' ';
        int ts = metrics[i].timestamp > 0 ? metrics[i].timestamp : next_sample_id;
        td = 0;
        if (ts == 0) tmp[td++] = '0';
        else while (ts > 0) { tmp[td++] = '0' + (ts % 10); ts /= 10; }
        while (td > 0 && pos < len) buf[pos++] = tmp[--td];

        buf[pos++] = '\n';
    }

    if (pos < len) buf[pos] = '\0';
    return pos;
}

/* Get all metrics
   buf: output buffer for text display, max_len: max length
   Returns: bytes written */
int metrics_get_all(char *buf, int max_len) {
    int pos = 0;

    for (int i = 0; i < metric_count && pos < max_len - 120; i++) {
        const char *type = metrics[i].type == METRIC_COUNTER ? "counter" :
                          metrics[i].type == METRIC_GAUGE ? "gauge" : "histogram";

        buf[pos++] = ' ';
        buf[pos++] = metrics[i].name[0] ? metrics[i].name[0] : '?';
        for (int j = 1; metrics[i].name[j]; j++) buf[pos++] = metrics[i].name[j];
        buf[pos++] = '\t';
        buf[pos++] = '[';
        for (int j = 0; type[j]; j++) buf[pos++] = type[j];
        buf[pos++] = ']';
        buf[pos++] = '\t';
        buf[pos++] = 'v'; buf[pos++] = 'a'; buf[pos++] = 'l'; buf[pos++] = '=';

        int val = metrics[i].value_int;
        if (val < 0) { buf[pos++] = '-'; val = -val; }
        char tmp[16];
        int td = 0;
        if (val == 0) tmp[td++] = '0';
        else while (val > 0) { tmp[td++] = '0' + (val % 10); val /= 10; }
        while (td > 0) buf[pos++] = tmp[--td];

        buf[pos++] = '\t';
        buf[pos++] = 'c'; buf[pos++] = 'n'; buf[pos++] = 't'; buf[pos++] = '=';
        val = metrics[i].sample_count;
        if (val == 0) { buf[pos++] = '0'; }
        else {
            td = 0;
            while (val > 0) { tmp[td++] = '0' + (val % 10); val /= 10; }
            while (td > 0) buf[pos++] = tmp[--td];
        }
        buf[pos++] = '\n';
    }

    if (pos < max_len) buf[pos] = '\0';
    return pos;
}

/* Print metrics to console */
void metrics_print_all(void) {
    print_str("=== Metrics Dashboard ===\n");
    print_str("NAME\t\tTYPE\tVALUE\tSAMPLES\tMIN\tMAX\n");

    for (int i = 0; i < metric_count; i++) {
        const char *type = metrics[i].type == METRIC_COUNTER ? "counter" :
                          metrics[i].type == METRIC_GAUGE ? "gauge" : "histogram";

        print_str("  ");
        print_str(metrics[i].name);
        print_str("\t[");
        print_str(type);
        print_str("]\t");
        print_int(metrics[i].value_int);
        print_str("\t");
        print_int(metrics[i].sample_count);
        print_str("\t");
        print_int(metrics[i].min_val);
        print_str("\t");
        print_int(metrics[i].max_val);
        print_str("\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, prometheus = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-p") == 0 || my_strcmp(arg, "--prometheus") == 0) prometheus = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Metrics v12.0 - Prometheus-Style Monitoring\n");

    if (help) {
        print_str("Usage: metrics [options]\n");
        print_str("  -h, --help          Show this help\n");
        print_str("  -t, --test          Run metrics test\n");
        print_str("  -p, --prometheus    Export Prometheus format\n");
        return;
    }

    if (test) {
        print_str("=== Metrics Test ===\n");

        /* Register metrics */
        metrics_register("ai_query_total", METRIC_COUNTER);
        metrics_register("ai_query_latency_ms", METRIC_HISTOGRAM);
        metrics_register("container_cpu_percent", METRIC_GAUGE);
        metrics_register("container_memory_mb", METRIC_GAUGE);
        metrics_register("agent_tasks_completed", METRIC_COUNTER);
        metrics_register("agent_task_errors", METRIC_COUNTER);
        metrics_register("wasm_module_exec_time_ms", METRIC_HISTOGRAM);
        metrics_register("http_requests_total", METRIC_COUNTER);
        metrics_register("disk_usage_percent", METRIC_GAUGE);
        metrics_register("network_bytes_sent", METRIC_COUNTER);

        /* Record some values */
        print_str("\n--- Recording Values ---\n");
        metrics_record("ai_query_total", 1, "model=claude");
        metrics_record("ai_query_total", 1, "model=openai");
        metrics_record("ai_query_total", 1, "model=gemini");
        metrics_record("ai_query_latency_ms", 1500, "model=claude");
        metrics_record("ai_query_latency_ms", 800, "model=openai");
        metrics_record("ai_query_latency_ms", 600, "model=gemini");
        metrics_record("container_cpu_percent", 45, "name=web-server");
        metrics_record("container_cpu_percent", 32, "name=api-gateway");
        metrics_record("container_memory_mb", 512, "name=web-server");
        metrics_record("container_memory_mb", 256, "name=api-gateway");
        metrics_record("agent_tasks_completed", 1, "type=planner");
        metrics_record("agent_tasks_completed", 1, "type=executor");
        metrics_record("http_requests_total", 1, "path=/api/v1/query");
        metrics_record("disk_usage_percent", 67, "");
        metrics_record("network_bytes_sent", 4096, "interface=eth0");

        /* Print dashboard */
        print_str("\n--- Dashboard ---\n");
        metrics_print_all();

        /* Export Prometheus format */
        print_str("\n--- Prometheus Export ---\n");
        unsigned int prom_buf = host_alloc(2048, 16);
        int prom_len = metrics_export_prometheus((char *)prom_buf, 2048);
        for (int i = 0; i < prom_len; i++) {
            char c = ((char *)prom_buf)[i];
            if (c == '\n') print_str("\n");
            else {
                char tmp[2] = { c, '\0' };
                print_str(tmp);
            }
        }
        print_str("\n");

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (prometheus) {
        unsigned int prom_buf = host_alloc(2048, 16);
        int prom_len = metrics_export_prometheus((char *)prom_buf, 2048);
        for (int i = 0; i < prom_len; i++) {
            char c = ((char *)prom_buf)[i];
            if (c == '\n') print_str("\n");
            else {
                char tmp[2] = { c, '\0' };
                print_str(tmp);
            }
        }
        print_str("\n");
        return;
    }

    print_str("Use -h for help, -t for test, -p for prometheus export\n");
}
