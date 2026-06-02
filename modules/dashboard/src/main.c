/* dashboard: Real-time monitoring dashboard (v33.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_WIDGETS    32
#define MAX_ALERTS     64
#define MAX_DATA_POINTS 128
#define MAX_NAME       64

/* Widget types */
#define WIDGET_GAUGE    0
#define WIDGET_LINE_CHART 1
#define WIDGET_BAR_CHART 2
#define WIDGET_TABLE    3
#define WIDGET_TEXT     4

/* Alert severity */
#define ALERT_INFO      0
#define ALERT_WARNING   1
#define ALERT_CRITICAL  2

/* Data point */
typedef struct {
    int   timestamp;
    int   value;
} data_point_t;

/* Widget */
typedef struct {
    int   widget_id;
    char  name[MAX_NAME];
    int   type;
    int   data_count;
    data_point_t data[MAX_DATA_POINTS];
    int   threshold_warn;
    int   threshold_crit;
} widget_t;

/* Alert */
typedef struct {
    int   alert_id;
    int   severity;
    char  message[MAX_NAME];
    int   timestamp;
    int   acknowledged;
} alert_t;

static widget_t widgets[MAX_WIDGETS];
static int widget_count = 0;
static int next_widget_id = 1;

static alert_t alerts[MAX_ALERTS];
static int alert_count = 0;
static int next_alert_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

static const char *severity_name(int s) {
    if (s == ALERT_INFO) return "INFO";
    if (s == ALERT_WARNING) return "WARNING";
    return "CRITICAL";
}

/* Create widget */
int db_create_widget(const char *name, int type, int warn_thresh, int crit_thresh) {
    if (widget_count >= MAX_WIDGETS) return -1;
    int idx = widget_count;
    widgets[idx].widget_id = next_widget_id++;
    my_strncpy(widgets[idx].name, name, MAX_NAME - 1);
    widgets[idx].type = type;
    widgets[idx].data_count = 0;
    widgets[idx].threshold_warn = warn_thresh;
    widgets[idx].threshold_crit = crit_thresh;
    widget_count++;
    return widgets[idx].widget_id;
}

/* Add data point */
int db_add_data(int widget_id, int value) {
    for (int i = 0; i < widget_count; i++) {
        if (widgets[i].widget_id == widget_id) {
            int j = widgets[i].data_count % MAX_DATA_POINTS;
            widgets[i].data[j].timestamp = alert_count + 1;
            widgets[i].data[j].value = value;
            widgets[i].data_count++;
            /* Check thresholds */
            if (value >= widgets[i].threshold_crit) {
                db_add_alert(ALERT_CRITICAL, widgets[i].name, value);
            } else if (value >= widgets[i].threshold_warn) {
                db_add_alert(ALERT_WARNING, widgets[i].name, value);
            }
            return widgets[i].data_count;
        }
    }
    return -1;
}

/* Add alert */
int db_add_alert(int severity, const char *widget_name, int value) {
    if (alert_count >= MAX_ALERTS) return -1;
    int idx = alert_count;
    alerts[idx].alert_id = next_alert_id++;
    alerts[idx].severity = severity;
    my_strncpy(alerts[idx].message, widget_name, MAX_NAME - 1);
    alerts[idx].timestamp = alert_count + 1;
    alerts[idx].acknowledged = 0;
    alert_count++;
    return alerts[idx].alert_id;
}

/* Render dashboard */
int db_render(void) {
    print_str("  ================================================\n");
    print_str("    AI-ASM OS Monitoring Dashboard v33.0\n");
    print_str("  ================================================\n\n");
    for (int i = 0; i < widget_count; i++) {
        print_str("  [");
        if (widgets[i].type == WIDGET_GAUGE) print_str("GAUGE] ");
        else if (widgets[i].type == WIDGET_LINE_CHART) print_str("CHART] ");
        else if (widgets[i].type == WIDGET_BAR_CHART) print_str("BAR  ] ");
        else print_str("TEXT ] ");
        print_str(widgets[i].name);
        if (widgets[i].data_count > 0) {
            int last_val = widgets[i].data[(widgets[i].data_count - 1) % MAX_DATA_POINTS].value;
            print_str(": ");
            print_int(last_val);
            if (last_val >= widgets[i].threshold_crit) print_str(" [CRITICAL]");
            else if (last_val >= widgets[i].threshold_warn) print_str(" [WARNING]");
        }
        print_str("\n");
    }
    return widget_count;
}

/* Show alerts */
int db_show_alerts(void) {
    print_str("  Alerts (");
    print_int(alert_count);
    print_str(" total)\n");
    print_str("  =================================================\n");
    int unack = 0;
    for (int i = 0; i < alert_count; i++) {
        if (!alerts[i].acknowledged) {
            print_str("  [");
            print_str(severity_name(alerts[i].severity));
            print_str("] ");
            print_str(alerts[i].message);
            print_str(" (value exceeded threshold)\n");
            unack++;
        }
    }
    print_str("  Unacknowledged: ");
    print_int(unack);
    print_str("\n");
    return alert_count;
}

/* Aggregate statistics */
int db_stats(int widget_id) {
    for (int i = 0; i < widget_count; i++) {
        if (widgets[i].widget_id == widget_id && widgets[i].data_count > 0) {
            int sum = 0, min_val = 999999, max_val = 0;
            int n = widgets[i].data_count;
            if (n > MAX_DATA_POINTS) n = MAX_DATA_POINTS;
            for (int j = 0; j < n; j++) {
                int v = widgets[i].data[j].value;
                sum += v;
                if (v < min_val) min_val = v;
                if (v > max_val) max_val = v;
            }
            print_str("  Widget: ");
            print_str(widgets[i].name);
            print_str("\n");
            print_str("  Samples: "); print_int(n); print_str("\n");
            print_str("  Average: "); print_int(sum / n); print_str("\n");
            print_str("  Min: "); print_int(min_val); print_str("\n");
            print_str("  Max: "); print_int(max_val); print_str("\n");
            return 0;
        }
    }
    return -1;
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
    print_str("Dashboard v33.0 - Real-Time Monitoring Dashboard\n");
    if (help) {
        print_str("Usage: dashboard [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run dashboard test\n");
        return;
    }
    if (test) {
        print_str("=== Dashboard Test ===\n\n");
        /* Create widgets */
        int w1 = db_create_widget("CPU Usage", WIDGET_GAUGE, 70, 90);
        int w2 = db_create_widget("Memory (MB)", WIDGET_GAUGE, 3500, 3800);
        int w3 = db_create_widget("WASM Throughput", WIDGET_LINE_CHART, 60000, 80000);
        int w4 = db_create_widget("Active Modules", WIDGET_BAR_CHART, 50, 80);
        print_str("  Widgets: "); print_int(widget_count); print_str("\n\n");
        /* Add data */
        db_add_data(w1, 45);
        db_add_data(w1, 72);   /* warning */
        db_add_data(w1, 91);   /* critical */
        db_add_data(w2, 2048);
        db_add_data(w2, 3600); /* warning */
        db_add_data(w3, 52000);
        db_add_data(w3, 61000);/* warning */
        db_add_data(w4, 30);
        db_add_data(w4, 55);   /* warning */
        print_str("\n");
        /* Render */
        db_render();
        print_str("\n");
        /* Stats */
        db_stats(w1);
        print_str("\n");
        db_stats(w3);
        print_str("\n");
        /* Alerts */
        db_show_alerts();
        print_str("\n=== Dashboard Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
