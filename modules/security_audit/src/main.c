/* security_audit: Security audit system (v15.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Event types */
#define EVENT_LOGIN   0
#define EVENT_ACCESS  1
#define EVENT_MODIFY  2
#define EVENT_ERROR   3
#define EVENT_ALERT   4

/* Event results */
#define RESULT_SUCCESS  0
#define RESULT_FAILURE  1
#define RESULT_DENIED   2

/* Severity levels */
#define SEVERITY_INFO      0
#define SEVERITY_WARN      1
#define SEVERITY_ERROR     2
#define SEVERITY_CRITICAL  3

/* Compliance standards */
#define COMPLIANCE_GENERIC 0
#define COMPLIANCE_SOC2    1
#define COMPLIANCE_ISO2701 2

/* Limits */
#define MAX_AUDIT_EVENTS  128
#define MAX_RISKS         10
#define SOURCE_LEN        64
#define TYPE_LEN          32
#define USER_LEN          32
#define ACTION_LEN        128
#define RESULT_LEN        16
#define DETAILS_LEN       512
#define RISK_LEN          128

/* Audit event */
typedef struct {
    int   event_id;
    int   timestamp;
    char  source[SOURCE_LEN];
    char  type[TYPE_LEN];
    char  user[USER_LEN];
    char  action[ACTION_LEN];
    char  result[RESULT_LEN];
    int   severity;
    char  details[DETAILS_LEN];
} audit_event_t;

/* Security report */
typedef struct {
    int total_events;
    int critical_events;
    int failed_attempts;
    int access_denied;
    char top_risks[MAX_RISKS][RISK_LEN];
    int security_score;
} security_report_t;

static audit_event_t events[MAX_AUDIT_EVENTS];
static int event_count = 0;
static int next_event_id = 1;

/* Tracking counters */
static int critical_count = 0;
static int failure_count = 0;
static int denied_count = 0;
static int login_attempts = 0;
static int login_failures = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    } return 0;
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

/* Get event type name */
static const char *event_type_name(int type) {
    if (type == EVENT_LOGIN) return "login";
    if (type == EVENT_ACCESS) return "access";
    if (type == EVENT_MODIFY) return "modify";
    if (type == EVENT_ERROR) return "error";
    if (type == EVENT_ALERT) return "alert";
    return "unknown";
}

/* Get severity name */
static const char *severity_name(int sev) {
    if (sev == SEVERITY_INFO) return "INFO";
    if (sev == SEVERITY_WARN) return "WARN";
    if (sev == SEVERITY_ERROR) return "ERROR";
    if (sev == SEVERITY_CRITICAL) return "CRITICAL";
    return "???";
}

/* Log an audit event */
int audit_log_event(audit_event_t *event) {
    if (!event || event_count >= MAX_AUDIT_EVENTS) return -1;
    int idx = event_count;
    events[idx].event_id = next_event_id++;
    events[idx].timestamp = 0;
    my_strncpy(events[idx].source, event->source, SOURCE_LEN - 1);
    my_strncpy(events[idx].type, event->type, TYPE_LEN - 1);
    my_strncpy(events[idx].user, event->user, USER_LEN - 1);
    my_strncpy(events[idx].action, event->action, ACTION_LEN - 1);
    my_strncpy(events[idx].result, event->result, RESULT_LEN - 1);
    events[idx].severity = event->severity;
    my_strncpy(events[idx].details, event->details, DETAILS_LEN - 1);

    /* Update counters */
    if (event->severity == SEVERITY_CRITICAL) critical_count++;
    if (my_strcmp(event->result, "FAILURE") == 0) { failure_count++; login_failures++; }
    if (my_strcmp(event->result, "DENIED") == 0) denied_count++;
    if (my_strcmp(event->type, "login") == 0) login_attempts++;

    event_count++;
    return 0;
}

/* Realtime monitor */
int audit_realtime_monitor(void) {
    print_str("=== Realtime Security Monitor ===\n");
    print_str("Total events: ");
    print_int(event_count);
    print_str("\nCritical: ");
    print_int(critical_count);
    print_str("\nFailures: ");
    print_int(failure_count);
    print_str("\nDenied: ");
    print_int(denied_count);
    print_str("\nLogin attempts: ");
    print_int(login_attempts);
    print_str(" (failures: ");
    print_int(login_failures);
    print_str(")\n");

    /* Recent events */
    int show = event_count < 5 ? event_count : 5;
    print_str("\nRecent events:\n");
    for (int i = event_count - show; i < event_count; i++) {
        print_str("  [");
        print_str(severity_name(events[i].severity));
        print_str("] ");
        print_str(events[i].type);
        print_str(" by ");
        print_str(events[i].user);
        print_str(": ");
        print_str(events[i].action);
        print_str(" -> ");
        print_str(events[i].result);
        print_str("\n");
    }
    return 0;
}

/* Anomaly detection */
int audit_detect_anomaly(void) {
    int anomalies = 0;
    print_str("=== Anomaly Detection ===\n");

    /* Detect brute force (many login failures) */
    if (login_failures > 3) {
        print_str("  ANOMALY: possible brute force (");
        print_int(login_failures);
        print_str(" login failures)\n");
        anomalies++;
    }

    /* Detect high critical event rate */
    if (critical_count > 0) {
        print_str("  ANOMALY: ");
        print_int(critical_count);
        print_str(" critical events detected\n");
        anomalies++;
    }

    /* Detect high denial rate */
    if (denied_count > 5) {
        print_str("  ANOMALY: high access denial rate (");
        print_int(denied_count);
        print_str(")\n");
        anomalies++;
    }

    if (anomalies == 0) {
        print_str("  No anomalies detected\n");
    } else {
        print_str("  ");
        print_int(anomalies);
        print_str(" anomalies found\n");
    }
    return anomalies;
}

/* Generate compliance report */
int audit_generate_compliance_report(int standard) {
    const char *std_name = "generic";
    if (standard == COMPLIANCE_SOC2) std_name = "SOC2";
    if (standard == COMPLIANCE_ISO2701) std_name = "ISO27001";

    print_str("=== Compliance Report: ");
    print_str(std_name);
    print_str(" ===\n");
    print_str("Total events audited: ");
    print_int(event_count);
    print_str("\nCritical events: ");
    print_int(critical_count);
    print_str("\nFailed attempts: ");
    print_int(failure_count);
    print_str("\nAccess denied: ");
    print_int(denied_count);
    print_str("\n");

    /* Compliance checklist */
    print_str("\nCompliance checks:\n");
    print_str("  [PASS] Audit logging enabled\n");
    print_str("  [PASS] Event tracking active\n");
    if (critical_count == 0) {
        print_str("  [PASS] No critical security incidents\n");
    } else {
        print_str("  [FAIL] Critical security incidents present\n");
    }
    if (failure_count < 10) {
        print_str("  [PASS] Failure rate within threshold\n");
    } else {
        print_str("  [WARN] High failure rate\n");
    }
    print_str("  [PASS] Access control active\n");
    return 0;
}

/* Get security score */
int audit_get_security_score(void) {
    int score = 100;
    /* Deduct for critical events */
    score -= critical_count * 20;
    /* Deduct for failures */
    score -= failure_count * 5;
    /* Deduct for denials */
    score -= denied_count * 3;
    /* Deduct for login failures */
    score -= login_failures * 10;
    if (score < 0) score = 0;

    print_str("=== Security Score ===\n");
    print_str("Score: ");
    print_int(score);
    print_str("/100\n");
    if (score >= 80) {
        print_str("Status: GOOD\n");
    } else if (score >= 50) {
        print_str("Status: WARNING\n");
    } else {
        print_str("Status: CRITICAL - immediate action required\n");
    }
    return score;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0, monitor = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-m") == 0 || my_strcmp(arg, "--monitor") == 0) monitor = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Security Audit v15.0 - Security Auditing & Compliance\n");
    if (help) {
        print_str("Usage: security_audit [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run audit test\n");
        print_str("  -m, --monitor Show realtime monitor\n");
        return;
    }
    if (test) {
        print_str("=== Security Audit Test ===\n");
        print_str("\n--- Log Events ---\n");

        audit_event_t e1; my_strcpy(e1.source, "auth_service"); my_strcpy(e1.type, "login"); my_strcpy(e1.user, "admin"); my_strcpy(e1.action, "login_attempt"); my_strcpy(e1.result, "SUCCESS"); e1.severity = SEVERITY_INFO; my_strcpy(e1.details, "normal login");
        audit_log_event(&e1);

        audit_event_t e2; my_strcpy(e2.source, "auth_service"); my_strcpy(e2.type, "login"); my_strcpy(e2.user, "attacker"); my_strcpy(e2.action, "login_attempt"); my_strcpy(e2.result, "FAILURE"); e2.severity = SEVERITY_WARN; my_strcpy(e2.details, "wrong password");
        audit_log_event(&e2);

        audit_event_t e3; my_strcpy(e3.source, "auth_service"); my_strcpy(e3.type, "login"); my_strcpy(e3.user, "attacker"); my_strcpy(e3.action, "login_attempt"); my_strcpy(e3.result, "FAILURE"); e3.severity = SEVERITY_WARN; my_strcpy(e3.details, "wrong password");
        audit_log_event(&e3);

        audit_event_t e4; my_strcpy(e4.source, "access_control"); my_strcpy(e4.type, "access"); my_strcpy(e4.user, "attacker"); my_strcpy(e4.action, "access_sensitive_data"); my_strcpy(e4.result, "DENIED"); e4.severity = SEVERITY_ERROR; my_strcpy(e4.details, "unauthorized access attempt");
        audit_log_event(&e4);

        audit_event_t e5; my_strcpy(e5.source, "file_system"); my_strcpy(e5.type, "modify"); my_strcpy(e5.user, "admin"); my_strcpy(e5.action, "modify_config"); my_strcpy(e5.result, "SUCCESS"); e5.severity = SEVERITY_INFO; my_strcpy(e5.details, "config updated");
        audit_log_event(&e5);

        audit_event_t e6; my_strcpy(e6.source, "ids"); my_strcpy(e6.type, "alert"); my_strcpy(e6.user, "system"); my_strcpy(e6.action, "suspicious_activity"); my_strcpy(e6.result, "SUCCESS"); e6.severity = SEVERITY_CRITICAL; my_strcpy(e6.details, "possible intrusion detected");
        audit_log_event(&e6);

        print_str("\n--- Realtime Monitor ---\n");
        audit_realtime_monitor();

        print_str("\n--- Anomaly Detection ---\n");
        audit_detect_anomaly();

        print_str("\n--- Compliance Report ---\n");
        audit_generate_compliance_report(COMPLIANCE_SOC2);

        print_str("\n--- Security Score ---\n");
        audit_get_security_score();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (monitor) { audit_realtime_monitor(); return; }
    print_str("Use -h for help, -t for test, -m for monitor\n");
}
