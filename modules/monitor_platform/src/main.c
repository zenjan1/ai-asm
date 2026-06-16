/* monitor_platform: System monitoring and observability platform (v1.0)
 *
 * Features:
 * - System health monitoring: CPU/memory/disk/network usage
 * - Performance metrics: response time, throughput, error rate
 * - Alert notification: threshold alerts, multi-channel (email/SMS/Webhook)
 * - Dashboard: real-time visualization, historical trends, custom charts
 * - Log aggregation: centralized collection, search analysis, trace correlation
 * - SLA monitoring: service level objectives, availability stats, report generation
 */

/* ── Host imports ────────────────────────────────────────────────────── */

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Constants ───────────────────────────────────────────────────────── */

#define MAX_SERVICES        32
#define MAX_METRICS         64
#define MAX_ALERTS          32
#define MAX_LOG_ENTRIES     128
#define MAX_SLA_RULES       16
#define MAX_DASHBOARD_ITEMS 24
#define MAX_NAME_LEN        32
#define MAX_MSG_LEN         128
#define MAX_HISTORY         60
#define HISTORY_INTERVAL_S  60

/* Health severity levels */
#define SEVERITY_OK         0
#define SEVERITY_WARNING    1
#define SEVERITY_CRITICAL   2
#define SEVERITY_UNKNOWN    3

/* Metric types */
#define METRIC_GAUGE        0
#define METRIC_COUNTER      1
#define METRIC_HISTOGRAM    2

/* Alert channels */
#define CHANNEL_EMAIL       0x01
#define CHANNEL_SMS         0x02
#define CHANNEL_WEBHOOK     0x04
#define CHANNEL_ALL         0x07

/* Alert states */
#define ALERT_FIRING        0
#define ALERT_RESOLVED      1
#define ALERT_SILENCED      2
#define ALERT_PENDING       3

/* Log levels */
#define LOG_TRACE           0
#define LOG_DEBUG           1
#define LOG_INFO            2
#define LOG_WARN            3
#define LOG_ERROR           4
#define LOG_FATAL           5

/* SLA statuses */
#define SLA_MET             0
#define SLA_BREACHED        1
#define SLA_AT_RISK         2

/* Dashboard item types */
#define DASH_GAUGE          0
#define DASH_CHART_LINE     1
#define DASH_CHART_BAR      2
#define DASH_TABLE          3
#define DASH_STATUS         4

/* ── Data structures ─────────────────────────────────────────────────── */

typedef struct {
    unsigned int timestamp;
    double value;
} data_point_t;

typedef struct {
    char name[MAX_NAME_LEN];
    double cpu_usage;          /* percentage 0-100 */
    double memory_usage;       /* percentage 0-100 */
    double disk_usage;         /* percentage 0-100 */
    double network_in_mbps;    /* megabits per second */
    double network_out_mbps;
    int severity;
    unsigned int last_check;
    data_point_t cpu_history[MAX_HISTORY];
    int history_len;
} service_health_t;

typedef struct {
    char name[MAX_NAME_LEN];
    int type;                  /* METRIC_GAUGE / COUNTER / HISTOGRAM */
    double value;
    double min_val;
    double max_val;
    double sum_val;
    unsigned int sample_count;
    unsigned int last_updated;
} performance_metric_t;

typedef struct {
    char name[MAX_NAME_LEN];
    char message[MAX_MSG_LEN];
    int channel_mask;          /* CHANNEL_EMAIL | CHANNEL_SMS | ... */
    int state;                 /* ALERT_FIRING / RESOLVED / ... */
    double threshold;
    double current_value;
    unsigned int fired_at;
    unsigned int resolved_at;
    int acknowledged;
} alert_rule_t;

typedef struct {
    unsigned int timestamp;
    int level;
    char source[MAX_NAME_LEN];
    char message[MAX_MSG_LEN];
    char trace_id[24];
} log_entry_t;

typedef struct {
    char name[MAX_NAME_LEN];
    double target;             /* target percentage e.g. 99.9 */
    double actual;             /* computed actual availability */
    int status;                /* SLA_MET / BREACHED / AT_RISK */
    unsigned int total_requests;
    unsigned int failed_requests;
    unsigned int period_start;
    unsigned int period_end;
    double error_budget_remaining;
} sla_rule_t;

typedef struct {
    char title[MAX_NAME_LEN];
    int item_type;             /* DASH_GAUGE / CHART_LINE / ... */
    int metric_index;
    int row;
    int col;
    int width;
    int height;
    char format[16];
} dashboard_item_t;

/* ── Global state ────────────────────────────────────────────────────── */

static unsigned int heap_pos = 65536;

static service_health_t services[MAX_SERVICES];
static int service_count = 0;

static performance_metric_t metrics[MAX_METRICS];
static int metric_count = 0;

static alert_rule_t alerts[MAX_ALERTS];
static int alert_count = 0;

static log_entry_t logs[MAX_LOG_ENTRIES];
static int log_count = 0;
static int log_write_idx = 0;

static sla_rule_t sla_rules[MAX_SLA_RULES];
static int sla_count = 0;

static dashboard_item_t dashboard[MAX_DASHBOARD_ITEMS];
static int dashboard_count = 0;

static unsigned int sim_time = 1700000000;
static int tests_run = 0;
static int tests_passed = 0;

/* ── Utility helpers ─────────────────────────────────────────────────── */

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    return ptr;
}

static void print_str(const char *s)
{
    host_print(s);
}

static void print_int(int v)
{
    char buf[16];
    int i = 0;
    if (v < 0) { buf[i++] = '-'; v = -v; }
    if (v == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (v > 0) { buf[i++] = '0' + (v % 10); v /= 10; }
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

static void print_uint(unsigned int v)
{
    char buf[16];
    int i = 0;
    if (v == 0) { buf[i++] = '0'; }
    else {
        int start = i;
        while (v > 0) { buf[i++] = '0' + (v % 10); v /= 10; }
        int end = i - 1;
        while (start < end) { char t = buf[start]; buf[start] = buf[end]; buf[end] = t; start++; end--; }
    }
    buf[i] = '\0';
    host_print(buf);
}

static void print_double(double v)
{
    char buf[32];
    int i = 0;
    int neg = 0;
    if (v < 0.0) { neg = 1; v = -v; }
    long long iv = (long long)v;
    double frac = v - (double)iv;
    if (neg) buf[i++] = '-';
    if (iv == 0) buf[i++] = '0';
    else {
        char tmp[20];
        int ti = 0;
        while (iv > 0) { tmp[ti++] = '0' + (int)(iv % 10); iv /= 10; }
        int s = 0, e = ti - 1;
        while (s < e) { char t = tmp[s]; tmp[s] = tmp[e]; tmp[e] = t; s++; e--; }
        while (ti > 0) buf[i++] = tmp[--ti];
    }
    buf[i++] = '.';
    int d;
    for (d = 0; d < 4; d++) { frac *= 10.0; buf[i++] = '0' + (int)frac; frac -= (int)frac; }
    buf[i] = '\0';
    host_print(buf);
}

static void print_ln(void)
{
    host_print("\n");
}

static void str_copy(char *dst, const char *src, int max_len)
{
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int str_eq(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return (*a == *b);
}

static int str_starts_with(const char *s, const char *prefix)
{
    while (*prefix) { if (*s != *prefix) return 0; s++; prefix++; }
    return 1;
}

/* ── System health monitoring ────────────────────────────────────────── */

static int register_service(const char *name)
{
    if (service_count >= MAX_SERVICES) return -1;
    service_health_t *svc = &services[service_count];
    str_copy(svc->name, name, MAX_NAME_LEN);
    svc->cpu_usage = 0.0;
    svc->memory_usage = 0.0;
    svc->disk_usage = 0.0;
    svc->network_in_mbps = 0.0;
    svc->network_out_mbps = 0.0;
    svc->severity = SEVERITY_OK;
    svc->last_check = sim_time;
    svc->history_len = 0;
    int idx = service_count;
    service_count++;
    return idx;
}

static int update_service_health(int idx, double cpu, double mem, double disk,
                                  double net_in, double net_out)
{
    if (idx < 0 || idx >= service_count) return -1;
    service_health_t *svc = &services[idx];
    svc->cpu_usage = cpu;
    svc->memory_usage = mem;
    svc->disk_usage = disk;
    svc->network_in_mbps = net_in;
    svc->network_out_mbps = net_out;
    svc->last_check = sim_time;

    /* Update history ring buffer */
    if (svc->history_len < MAX_HISTORY) {
        svc->cpu_history[svc->history_len].timestamp = sim_time;
        svc->cpu_history[svc->history_len].value = cpu;
        svc->history_len++;
    } else {
        int j;
        for (j = 0; j < MAX_HISTORY - 1; j++) {
            svc->cpu_history[j] = svc->cpu_history[j + 1];
        }
        svc->cpu_history[MAX_HISTORY - 1].timestamp = sim_time;
        svc->cpu_history[MAX_HISTORY - 1].value = cpu;
    }

    /* Compute severity */
    if (cpu > 95.0 || mem > 95.0 || disk > 95.0)
        svc->severity = SEVERITY_CRITICAL;
    else if (cpu > 80.0 || mem > 80.0 || disk > 85.0)
        svc->severity = SEVERITY_WARNING;
    else
        svc->severity = SEVERITY_OK;

    return 0;
}

static void print_service_health(int idx)
{
    if (idx < 0 || idx >= service_count) return;
    service_health_t *svc = &services[idx];
    const char *sev_str[] = { "OK", "WARNING", "CRITICAL", "UNKNOWN" };
    host_print("  Service: ");
    host_print(svc->name);
    host_print("\n    CPU: ");
    print_double(svc->cpu_usage);
    host_print("%  MEM: ");
    print_double(svc->memory_usage);
    host_print("%  DISK: ");
    print_double(svc->disk_usage);
    host_print("%\n    Net In: ");
    print_double(svc->network_in_mbps);
    host_print(" Mbps  Net Out: ");
    print_double(svc->network_out_mbps);
    host_print(" Mbps\n    Severity: ");
    host_print(sev_str[svc->severity]);
    host_print("  History Points: ");
    print_int(svc->history_len);
    host_print("\n");
}

static double compute_avg_cpu(int idx)
{
    if (idx < 0 || idx >= service_count) return 0.0;
    service_health_t *svc = &services[idx];
    if (svc->history_len == 0) return 0.0;
    double sum = 0.0;
    int j;
    for (j = 0; j < svc->history_len; j++) {
        sum += svc->cpu_history[j].value;
    }
    return sum / (double)svc->history_len;
}

/* ── Performance metrics ─────────────────────────────────────────────── */

static int register_metric(const char *name, int type)
{
    if (metric_count >= MAX_METRICS) return -1;
    performance_metric_t *m = &metrics[metric_count];
    str_copy(m->name, name, MAX_NAME_LEN);
    m->type = type;
    m->value = 0.0;
    m->min_val = 0.0;
    m->max_val = 0.0;
    m->sum_val = 0.0;
    m->sample_count = 0;
    m->last_updated = sim_time;
    int idx = metric_count;
    metric_count++;
    return idx;
}

static int record_metric(int idx, double value)
{
    if (idx < 0 || idx >= metric_count) return -1;
    performance_metric_t *m = &metrics[idx];
    m->value = value;
    m->last_updated = sim_time;
    m->sample_count++;
    m->sum_val += value;
    if (m->sample_count == 1) {
        m->min_val = value;
        m->max_val = value;
    } else {
        if (value < m->min_val) m->min_val = value;
        if (value > m->max_val) m->max_val = value;
    }
    return 0;
}

static double get_metric_avg(int idx)
{
    if (idx < 0 || idx >= metric_count) return 0.0;
    performance_metric_t *m = &metrics[idx];
    if (m->sample_count == 0) return 0.0;
    return m->sum_val / (double)m->sample_count;
}

static void print_metric(int idx)
{
    if (idx < 0 || idx >= metric_count) return;
    performance_metric_t *m = &metrics[idx];
    const char *type_str[] = { "GAUGE", "COUNTER", "HISTOGRAM" };
    host_print("  Metric: ");
    host_print(m->name);
    host_print(" [");
    host_print(type_str[m->type]);
    host_print("]\n    Value: ");
    print_double(m->value);
    host_print("  Min: ");
    print_double(m->min_val);
    host_print("  Max: ");
    print_double(m->max_val);
    host_print("  Avg: ");
    print_double(get_metric_avg(idx));
    host_print("  Samples: ");
    print_uint(m->sample_count);
    host_print("\n");
}

/* ── Alert notification ──────────────────────────────────────────────── */

static int create_alert(const char *name, double threshold, int channels)
{
    if (alert_count >= MAX_ALERTS) return -1;
    alert_rule_t *a = &alerts[alert_count];
    str_copy(a->name, name, MAX_NAME_LEN);
    a->message[0] = '\0';
    a->channel_mask = channels;
    a->state = ALERT_PENDING;
    a->threshold = threshold;
    a->current_value = 0.0;
    a->fired_at = 0;
    a->resolved_at = 0;
    a->acknowledged = 0;
    int idx = alert_count;
    alert_count++;
    return idx;
}

static int evaluate_alert(int idx, double current_value)
{
    if (idx < 0 || idx >= alert_count) return -1;
    alert_rule_t *a = &alerts[idx];
    a->current_value = current_value;
    int prev_state = a->state;
    if (a->state == ALERT_SILENCED) return 0;

    if (current_value > a->threshold) {
        if (a->state != ALERT_FIRING) {
            a->state = ALERT_FIRING;
            a->fired_at = sim_time;
            str_copy(a->message, "Threshold exceeded", MAX_MSG_LEN);
        }
    } else {
        if (a->state == ALERT_FIRING) {
            a->state = ALERT_RESOLVED;
            a->resolved_at = sim_time;
            str_copy(a->message, "Recovered below threshold", MAX_MSG_LEN);
        }
    }
    return (a->state != prev_state) ? 1 : 0;
}

static void print_alert(int idx)
{
    if (idx < 0 || idx >= alert_count) return;
    alert_rule_t *a = &alerts[idx];
    const char *state_str[] = { "FIRING", "RESOLVED", "SILENCED", "PENDING" };
    host_print("  Alert: ");
    host_print(a->name);
    host_print("\n    State: ");
    host_print(state_str[a->state]);
    host_print("  Threshold: ");
    print_double(a->threshold);
    host_print("  Current: ");
    print_double(a->current_value);
    host_print("\n    Channels: ");
    if (a->channel_mask & CHANNEL_EMAIL)  host_print("EMAIL ");
    if (a->channel_mask & CHANNEL_SMS)    host_print("SMS ");
    if (a->channel_mask & CHANNEL_WEBHOOK) host_print("WEBHOOK ");
    if (a->fired_at > 0) {
        host_print("\n    Fired at: ");
        print_uint(a->fired_at);
    }
    if (a->resolved_at > 0) {
        host_print("  Resolved at: ");
        print_uint(a->resolved_at);
    }
    host_print("\n");
}

static int silence_alert(int idx)
{
    if (idx < 0 || idx >= alert_count) return -1;
    alerts[idx].state = ALERT_SILENCED;
    return 0;
}

static int acknowledge_alert(int idx)
{
    if (idx < 0 || idx >= alert_count) return -1;
    alerts[idx].acknowledged = 1;
    return 0;
}

/* ── Log aggregation ─────────────────────────────────────────────────── */

static int append_log(int level, const char *source, const char *message,
                       const char *trace_id)
{
    log_entry_t *entry = &logs[log_write_idx % MAX_LOG_ENTRIES];
    entry->timestamp = sim_time;
    entry->level = level;
    str_copy(entry->source, source, MAX_NAME_LEN);
    str_copy(entry->message, message, MAX_MSG_LEN);
    str_copy(entry->trace_id, trace_id ? trace_id : "", 24);
    log_write_idx++;
    if (log_count < MAX_LOG_ENTRIES) log_count++;
    return 0;
}

static int search_logs(const char *query, int min_level)
{
    int found = 0;
    int i;
    for (i = 0; i < log_count; i++) {
        int ri = (log_write_idx - log_count + i + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
        log_entry_t *e = &logs[ri];
        if (e->level < min_level) continue;
        if (str_starts_with(e->message, query) || str_eq(e->source, query)) {
            const char *lvl_str[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
            host_print("  [");
            host_print(lvl_str[e->level]);
            host_print("] ");
            print_uint(e->timestamp);
            host_print(" ");
            host_print(e->source);
            host_print(": ");
            host_print(e->message);
            if (e->trace_id[0] != '\0') {
                host_print(" (trace:");
                host_print(e->trace_id);
                host_print(")");
            }
            host_print("\n");
            found++;
        }
    }
    return found;
}

static int count_logs_by_level(int level)
{
    int count = 0;
    int i;
    for (i = 0; i < log_count; i++) {
        int ri = (log_write_idx - log_count + i + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
        if (logs[ri].level == level) count++;
    }
    return count;
}

static int find_by_trace(const char *trace_id)
{
    int found = 0;
    int i;
    for (i = 0; i < log_count; i++) {
        int ri = (log_write_idx - log_count + i + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
        if (str_eq(logs[ri].trace_id, trace_id)) {
            found++;
        }
    }
    return found;
}

/* ── SLA monitoring ──────────────────────────────────────────────────── */

static int create_sla(const char *name, double target_pct,
                       unsigned int period_secs)
{
    if (sla_count >= MAX_SLA_RULES) return -1;
    sla_rule_t *s = &sla_rules[sla_count];
    str_copy(s->name, name, MAX_NAME_LEN);
    s->target = target_pct;
    s->actual = 100.0;
    s->status = SLA_MET;
    s->total_requests = 0;
    s->failed_requests = 0;
    s->period_start = sim_time;
    s->period_end = sim_time + period_secs;
    s->error_budget_remaining = 100.0 - target_pct;
    int idx = sla_count;
    sla_count++;
    return idx;
}

static int record_sla_event(int idx, int success)
{
    if (idx < 0 || idx >= sla_count) return -1;
    sla_rule_t *s = &sla_rules[idx];
    s->total_requests++;
    if (!success) s->failed_requests++;

    if (s->total_requests > 0) {
        s->actual = 100.0 * (1.0 - (double)s->failed_requests / (double)s->total_requests);
    }

    double error_budget = 100.0 - s->target;
    double consumed = 100.0 - s->actual;
    s->error_budget_remaining = error_budget - consumed;
    if (s->error_budget_remaining < 0.0) s->error_budget_remaining = 0.0;

    if (s->actual < s->target)
        s->status = SLA_BREACHED;
    else if (s->error_budget_remaining < (error_budget * 0.2))
        s->status = SLA_AT_RISK;
    else
        s->status = SLA_MET;

    return 0;
}

static void print_sla(int idx)
{
    if (idx < 0 || idx >= sla_count) return;
    sla_rule_t *s = &sla_rules[idx];
    const char *status_str[] = { "MET", "BREACHED", "AT_RISK" };
    host_print("  SLA: ");
    host_print(s->name);
    host_print("\n    Target: ");
    print_double(s->target);
    host_print("%  Actual: ");
    print_double(s->actual);
    host_print("%  Status: ");
    host_print(status_str[s->status]);
    host_print("\n    Requests: ");
    print_uint(s->total_requests);
    host_print("  Failed: ");
    print_uint(s->failed_requests);
    host_print("  Budget Left: ");
    print_double(s->error_budget_remaining);
    host_print("%\n");
}

/* ── Dashboard ───────────────────────────────────────────────────────── */

static int add_dashboard_item(const char *title, int item_type,
                               int metric_idx, int row, int col,
                               int width, int height)
{
    if (dashboard_count >= MAX_DASHBOARD_ITEMS) return -1;
    dashboard_item_t *d = &dashboard[dashboard_count];
    str_copy(d->title, title, MAX_NAME_LEN);
    d->item_type = item_type;
    d->metric_index = metric_idx;
    d->row = row;
    d->col = col;
    d->width = width;
    d->height = height;
    d->format[0] = '\0';
    int idx = dashboard_count;
    dashboard_count++;
    return idx;
}

static void render_dashboard(void)
{
    host_print("=== Dashboard ===\n");
    int i;
    for (i = 0; i < dashboard_count; i++) {
        dashboard_item_t *d = &dashboard[i];
        const char *type_str[] = { "GAUGE", "LINE", "BAR", "TABLE", "STATUS" };
        host_print("  [");
        host_print(type_str[d->item_type]);
        host_print("] ");
        host_print(d->title);
        host_print(" (");
        print_int(d->row);
        host_print(",");
        print_int(d->col);
        host_print(" ");
        print_int(d->width);
        host_print("x");
        print_int(d->height);
        host_print(")\n");
    }
    host_print("=== End Dashboard ===\n");
}

/* ── Report generation ───────────────────────────────────────────────── */

static void generate_report(void)
{
    host_print("========================================\n");
    host_print("  MONITORING PLATFORM REPORT\n");
    host_print("  Timestamp: ");
    print_uint(sim_time);
    host_print("\n========================================\n");

    host_print("\n-- Services (");
    print_int(service_count);
    host_print(") --\n");
    int i;
    for (i = 0; i < service_count; i++) {
        print_service_health(i);
    }

    host_print("\n-- Metrics (");
    print_int(metric_count);
    host_print(") --\n");
    for (i = 0; i < metric_count; i++) {
        print_metric(i);
    }

    host_print("\n-- Alerts (");
    print_int(alert_count);
    host_print(") --\n");
    for (i = 0; i < alert_count; i++) {
        print_alert(i);
    }

    host_print("\n-- Log Summary --\n");
    host_print("  Total entries: ");
    print_int(log_count);
    host_print("\n  By level: TRACE=");
    print_int(count_logs_by_level(LOG_TRACE));
    host_print(" DEBUG=");
    print_int(count_logs_by_level(LOG_DEBUG));
    host_print(" INFO=");
    print_int(count_logs_by_level(LOG_INFO));
    host_print(" WARN=");
    print_int(count_logs_by_level(LOG_WARN));
    host_print(" ERROR=");
    print_int(count_logs_by_level(LOG_ERROR));
    host_print(" FATAL=");
    print_int(count_logs_by_level(LOG_FATAL));
    host_print("\n");

    host_print("\n-- SLA Status (");
    print_int(sla_count);
    host_print(") --\n");
    for (i = 0; i < sla_count; i++) {
        print_sla(i);
    }

    host_print("========================================\n");
    host_print("  END REPORT\n");
    host_print("========================================\n");
}

/* ── Test suite ──────────────────────────────────────────────────────── */

static void assert_eq_int(int actual, int expected, const char *msg)
{
    tests_run++;
    if (actual == expected) {
        tests_passed++;
        host_print("  PASS: ");
    } else {
        host_print("  FAIL: ");
    }
    host_print(msg);
    host_print(" (got=");
    print_int(actual);
    host_print(" expected=");
    print_int(expected);
    host_print(")\n");
}

static void assert_close(double actual, double expected, double tol,
                          const char *msg)
{
    tests_run++;
    double diff = actual - expected;
    if (diff < 0.0) diff = -diff;
    if (diff <= tol) {
        tests_passed++;
        host_print("  PASS: ");
    } else {
        host_print("  FAIL: ");
    }
    host_print(msg);
    host_print(" (got=");
    print_double(actual);
    host_print(" expected=");
    print_double(expected);
    host_print(")\n");
}

static void run_tests(void)
{
    host_print("\n=== Monitor Platform Test Suite ===\n\n");

    /* Test: Service registration */
    host_print("[Service Health]\n");
    int s0 = register_service("web-server-01");
    int s1 = register_service("db-primary");
    int s2 = register_service("cache-redis");
    assert_eq_int(s0, 0, "register service 0");
    assert_eq_int(s1, 1, "register service 1");
    assert_eq_int(s2, 2, "register service 2");
    assert_eq_int(service_count, 3, "service count is 3");

    /* Test: Health update and severity */
    update_service_health(s0, 45.0, 60.0, 30.0, 100.0, 50.0);
    assert_eq_int(services[s0].severity, SEVERITY_OK, "web-server OK at 45% CPU");

    update_service_health(s0, 85.0, 70.0, 30.0, 200.0, 100.0);
    assert_eq_int(services[s0].severity, SEVERITY_WARNING, "web-server WARNING at 85% CPU");

    update_service_health(s0, 97.0, 96.0, 40.0, 300.0, 150.0);
    assert_eq_int(services[s0].severity, SEVERITY_CRITICAL, "web-server CRITICAL at 97% CPU");

    /* Test: History tracking */
    assert_eq_int(services[s0].history_len, 3, "history has 3 points");
    assert_close(compute_avg_cpu(s0), (97.0 + 85.0 + 45.0) / 3.0, 0.01,
                 "average CPU across history");

    /* Test: Out-of-bounds */
    assert_eq_int(update_service_health(-1, 0, 0, 0, 0, 0), -1,
                  "update invalid index returns -1");
    assert_eq_int(update_service_health(999, 0, 0, 0, 0, 0), -1,
                  "update out-of-range index returns -1");

    /* Test: Performance metrics */
    host_print("\n[Performance Metrics]\n");
    int m0 = register_metric("http_response_time_ms", METRIC_HISTOGRAM);
    int m1 = register_metric("http_requests_total", METRIC_COUNTER);
    int m2 = register_metric("active_connections", METRIC_GAUGE);
    assert_eq_int(m0, 0, "register metric 0");
    assert_eq_int(metric_count, 3, "metric count is 3");

    record_metric(m0, 45.2);
    record_metric(m0, 52.8);
    record_metric(m0, 38.1);
    record_metric(m0, 120.5);
    assert_eq_int(metrics[m0].sample_count, 4, "response_time has 4 samples");
    assert_close(metrics[m0].min_val, 38.1, 0.01, "response_time min is 38.1");
    assert_close(metrics[m0].max_val, 120.5, 0.01, "response_time max is 120.5");
    assert_close(get_metric_avg(m0), (45.2 + 52.8 + 38.1 + 120.5) / 4.0, 0.01,
                 "response_time avg");

    record_metric(m1, 1.0);
    record_metric(m1, 1.0);
    record_metric(m1, 1.0);
    assert_eq_int(metrics[m1].sample_count, 3, "counter has 3 increments");

    record_metric(m2, 250.0);
    assert_close(metrics[m2].value, 250.0, 0.01, "gauge current value");

    /* Test: Alerts */
    host_print("\n[Alerts]\n");
    int a0 = create_alert("high-cpu", 80.0, CHANNEL_EMAIL | CHANNEL_SMS);
    int a1 = create_alert("high-error-rate", 5.0, CHANNEL_WEBHOOK);
    assert_eq_int(a0, 0, "create alert 0");
    assert_eq_int(a1, 1, "create alert 1");
    assert_eq_int(alerts[a0].state, ALERT_PENDING, "alert 0 starts PENDING");

    /* Should not fire below threshold */
    int changed = evaluate_alert(a0, 60.0);
    assert_eq_int(changed, 0, "no state change below threshold");

    /* Should fire above threshold */
    changed = evaluate_alert(a0, 92.0);
    assert_eq_int(changed, 1, "state change on threshold breach");
    assert_eq_int(alerts[a0].state, ALERT_FIRING, "alert 0 is FIRING");
    assert_eq_int(alerts[a0].acknowledged, 0, "not yet acknowledged");

    acknowledge_alert(a0);
    assert_eq_int(alerts[a0].acknowledged, 1, "acknowledged after ack call");

    /* Should resolve when below threshold */
    changed = evaluate_alert(a0, 50.0);
    assert_eq_int(changed, 1, "state change on recovery");
    assert_eq_int(alerts[a0].state, ALERT_RESOLVED, "alert 0 RESOLVED");

    /* Silence test */
    silence_alert(a1);
    assert_eq_int(alerts[a1].state, ALERT_SILENCED, "alert 1 SILENCED");
    evaluate_alert(a1, 99.0);
    assert_eq_int(alerts[a1].state, ALERT_SILENCED,
                  "silenced alert stays silenced");

    /* Test: Log aggregation */
    host_print("\n[Log Aggregation]\n");
    append_log(LOG_INFO, "web-server", "Request processed", "abc123");
    append_log(LOG_ERROR, "db-primary", "Connection timeout", "abc123");
    append_log(LOG_WARN, "cache-redis", "Cache miss rate high", "def456");
    append_log(LOG_INFO, "web-server", "Health check OK", "ghi789");
    append_log(LOG_DEBUG, "web-server", "Debug trace data", "abc123");
    assert_eq_int(log_count, 5, "5 log entries recorded");
    assert_eq_int(count_logs_by_level(LOG_INFO), 2, "2 INFO entries");
    assert_eq_int(count_logs_by_level(LOG_ERROR), 1, "1 ERROR entry");

    /* Search by source */
    host_print("  Search 'web-server' at INFO+:\n");
    int found = search_logs("web-server", LOG_INFO);
    assert_eq_int(found, 2, "found 2 web-server logs at INFO+");

    /* Trace correlation */
    assert_eq_int(find_by_trace("abc123"), 3, "trace abc123 has 3 entries");
    assert_eq_int(find_by_trace("zzz000"), 0, "trace zzz000 not found");

    /* Test: SLA monitoring */
    host_print("\n[SLA Monitoring]\n");
    int sl0 = create_sla("api-availability", 99.9, 86400);
    assert_eq_int(sl0, 0, "create SLA rule 0");
    assert_close(sla_rules[sl0].actual, 100.0, 0.01, "SLA starts at 100%");

    /* Record 1000 requests, 0 failures */
    int j;
    for (j = 0; j < 1000; j++) {
        record_sla_event(sl0, 1);
    }
    assert_close(sla_rules[sl0].actual, 100.0, 0.01, "SLA 100% with 0 failures");
    assert_eq_int(sla_rules[sl0].status, SLA_MET, "SLA status MET");
    assert_close(sla_rules[sl0].error_budget_remaining, 0.1, 0.01,
                 "error budget = 0.1%");

    /* Introduce failures to breach SLA */
    for (j = 0; j < 50; j++) {
        record_sla_event(sl0, 0);
    }
    assert_close(sla_rules[sl0].actual,
                 100.0 * (1.0 - 50.0 / 1050.0), 0.01,
                 "SLA actual after failures");
    assert_eq_int(sla_rules[sl0].status, SLA_BREACHED, "SLA BREACHED");

    /* Test: Dashboard */
    host_print("\n[Dashboard]\n");
    int d0 = add_dashboard_item("CPU Usage", DASH_GAUGE, m0, 0, 0, 4, 2);
    int d1 = add_dashboard_item("Response Time", DASH_CHART_LINE, m0, 0, 4, 8, 4);
    int d2 = add_dashboard_item("Requests", DASH_CHART_BAR, m1, 2, 0, 6, 3);
    assert_eq_int(d0, 0, "dashboard item 0");
    assert_eq_int(d1, 1, "dashboard item 1");
    assert_eq_int(d2, 2, "dashboard item 2");
    assert_eq_int(dashboard_count, 3, "3 dashboard items");

    render_dashboard();

    /* Test: Ring buffer overflow */
    host_print("\n[Ring Buffer]\n");
    int overflow_svc = register_service("overflow-test");
    int k;
    for (k = 0; k < MAX_HISTORY + 10; k++) {
        update_service_health(overflow_svc, (double)(k % 100), 50.0, 30.0,
                              10.0, 5.0);
    }
    assert_eq_int(services[overflow_svc].history_len, MAX_HISTORY,
                  "history capped at MAX_HISTORY");

    /* Test: Edge cases */
    host_print("\n[Edge Cases]\n");
    assert_eq_int(create_alert("x", 50.0, 0), alert_count >= 2 ? 2 : alert_count - 1,
                  "alert creation with no channels");
    assert_eq_int(register_metric("", METRIC_GAUGE) >= 0, 1,
                  "empty metric name accepted");
    assert_eq_int(record_metric(-1, 1.0), -1,
                  "record to invalid metric returns -1");

    /* Print summary */
    host_print("\n=== Test Results ===\n");
    host_print("  Passed: ");
    print_int(tests_passed);
    host_print(" / ");
    print_int(tests_run);
    host_print("\n");
    if (tests_passed == tests_run) {
        host_print("  ALL TESTS PASSED\n");
    } else {
        host_print("  SOME TESTS FAILED\n");
    }
}

/* ── Argument parsing ────────────────────────────────────────────────── */

static void print_help(void)
{
    host_print("monitor_platform v1.0 - System Monitoring Platform\n\n");
    host_print("Usage: monitor_platform [options]\n\n");
    host_print("Options:\n");
    host_print("  -h          Show this help message\n");
    host_print("  -t          Run test suite\n");
    host_print("  -r          Generate full monitoring report\n");
    host_print("  -d          Render dashboard\n");
    host_print("  -s          Simulate monitoring cycle\n\n");
    host_print("Features:\n");
    host_print("  - System health: CPU/memory/disk/network monitoring\n");
    host_print("  - Performance: response time, throughput, error rate\n");
    host_print("  - Alerts: threshold-based, multi-channel (email/SMS/webhook)\n");
    host_print("  - Dashboard: real-time gauges, charts, tables\n");
    host_print("  - Logs: centralized collection, search, trace correlation\n");
    host_print("  - SLA: availability tracking, error budgets, reports\n");
}

static void simulate_cycle(void)
{
    host_print("--- Simulation Cycle ---\n");
    sim_time = 1700000000;

    /* Register services */
    int web = register_service("web-server");
    int api = register_service("api-gateway");
    int db  = register_service("database");

    /* Register metrics */
    int resp_time = register_metric("response_time_ms", METRIC_HISTOGRAM);
    int req_count = register_metric("requests_per_sec", METRIC_COUNTER);
    int err_rate  = register_metric("error_rate_pct", METRIC_GAUGE);

    /* Create alerts */
    int cpu_alert  = create_alert("cpu-high", 85.0, CHANNEL_EMAIL | CHANNEL_SMS);
    int err_alert  = create_alert("error-rate-high", 5.0, CHANNEL_WEBHOOK);

    /* Create SLA */
    int sla_api = create_sla("api-uptime", 99.95, 2592000);

    /* Add dashboard items */
    add_dashboard_item("Web CPU", DASH_GAUGE, resp_time, 0, 0, 4, 2);
    add_dashboard_item("API Latency", DASH_CHART_LINE, resp_time, 0, 4, 8, 4);

    /* Simulate 10 time steps */
    int step;
    for (step = 0; step < 10; step++) {
        sim_time += HISTORY_INTERVAL_S;

        double cpu_web = 40.0 + (double)(step * 7);
        double cpu_api = 30.0 + (double)(step * 5);
        double cpu_db  = 20.0 + (double)(step * 3);

        update_service_health(web, cpu_web, 55.0 + (double)step, 40.0,
                              100.0 + (double)(step * 20),
                              50.0 + (double)(step * 10));
        update_service_health(api, cpu_api, 45.0, 25.0, 80.0, 40.0);
        update_service_health(db, cpu_db, 70.0, 65.0, 20.0, 10.0);

        double rt = 30.0 + (double)(step * 8);
        record_metric(resp_time, rt);
        record_metric(req_count, 150.0 + (double)(step * 10));
        double er = (step > 5) ? 3.5 : 0.5;
        record_metric(err_rate, er);

        evaluate_alert(cpu_alert, cpu_web);
        evaluate_alert(err_alert, er);

        int reqs = 100;
        int fails = (step > 7) ? 2 : 0;
        int r;
        for (r = 0; r < reqs - fails; r++) record_sla_event(sla_api, 1);
        for (r = 0; r < fails; r++) record_sla_event(sla_api, 0);

        if (step % 3 == 0) {
            append_log(LOG_INFO, "web-server", "Health check OK", "sim-trace-01");
        }
        if (step == 8) {
            append_log(LOG_ERROR, "api-gateway", "Upstream timeout", "sim-trace-02");
        }
    }

    host_print("\n-- Post-Simulation Status --\n");
    print_service_health(web);
    print_metric(resp_time);
    print_alert(cpu_alert);
    print_sla(sla_api);
    render_dashboard();
}

/* ── Entry point ─────────────────────────────────────────────────────── */

void _start(void)
{
    unsigned int buf_off = host_alloc(512, 16);
    int argv_len = host_get_argv(buf_off, 512);

    int run_test_flag = 0;
    int run_report_flag = 0;
    int run_dashboard_flag = 0;
    int run_simulate_flag = 0;
    int show_help_flag = 0;

    if (argv_len > 0) {
        /* Parse arguments from buffer */
        char *buf = (char *)buf_off;
        int pos = 0;
        while (pos < argv_len) {
            char *arg = &buf[pos];
            if (str_eq(arg, "-h")) {
                show_help_flag = 1;
            } else if (str_eq(arg, "-t")) {
                run_test_flag = 1;
            } else if (str_eq(arg, "-r")) {
                run_report_flag = 1;
            } else if (str_eq(arg, "-d")) {
                run_dashboard_flag = 1;
            } else if (str_eq(arg, "-s")) {
                run_simulate_flag = 1;
            }
            /* Advance past null terminator */
            while (pos < argv_len && buf[pos] != '\0') pos++;
            pos++;
        }
    }

    /* Default: show help if no flags */
    if (!show_help_flag && !run_test_flag && !run_report_flag &&
        !run_dashboard_flag && !run_simulate_flag) {
        show_help_flag = 1;
    }

    if (show_help_flag) {
        print_help();
    }

    if (run_simulate_flag) {
        simulate_cycle();
    }

    if (run_test_flag) {
        run_tests();
    }

    if (run_report_flag) {
        generate_report();
    }

    if (run_dashboard_flag) {
        if (dashboard_count == 0) {
            /* Create default dashboard for demo */
            int dm = register_metric("demo_metric", METRIC_GAUGE);
            record_metric(dm, 42.0);
            add_dashboard_item("Demo Gauge", DASH_GAUGE, dm, 0, 0, 4, 2);
            add_dashboard_item("Demo Chart", DASH_CHART_LINE, dm, 2, 0, 8, 4);
        }
        render_dashboard();
    }

    host_exit(0);
}
