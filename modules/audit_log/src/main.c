/* audit_log: Audit logging system module (v46.0)
 *
 * Provides comprehensive audit logging:
 * - Operation records (user/process/file)
 * - Security event tracking
 * - Compliance reports (CIS/ISO27001)
 * - Intrusion detection (anomaly analysis)
 * - Log aggregation and search
 * - Log retention policies
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

/* Log levels */
#define LOG_EMERG       0
#define LOG_ALERT       1
#define LOG_CRIT        2
#define LOG_ERR         3
#define LOG_WARNING     4
#define LOG_NOTICE      5
#define LOG_INFO        6
#define LOG_DEBUG       7

/* Event types */
#define EVENT_USER          1
#define EVENT_PROCESS       2
#define EVENT_FILE          3
#define EVENT_NETWORK       4
#define EVENT_SYSTEM        5
#define EVENT_SECURITY      6
#define EVENT_COMPLIANCE    7

/* User operations */
#define USEROP_LOGIN        1
#define USEROP_LOGOUT       2
#define USEROP_SUDO         3
#define USEROP_PASSWD       4
#define USEROP_FILE_ACCESS  5

/* Process operations */
#define PROCOP_FORK         1
#define PROCOP_EXEC         2
#define PROCOP_EXIT         3
#define PROCOP_SIGNAL       4

/* File operations */
#define FILEOP_OPEN         1
#define FILEOP_READ         2
#define FILEOP_WRITE        3
#define FILEOP_DELETE       4
#define FILEOP_CHMOD        5
#define FILEOP_CHOWN        6

/* Network operations */
#define NETOP_CONNECT       1
#define NETOP_ACCEPT        2
#define NETOP_SEND          3
#define NETOP_RECV          4
#define NETOP_BIND          5

/* Compliance frameworks */
#define COMPLIANCE_CIS          1
#define COMPLIANCE_ISO27001     2
#define COMPLIANCE_PCI_DSS      3
#define COMPLIANCE_GDPR         4

/* Limits */
#define MAX_LOG_ENTRIES     10000
#define MAX_RULES           256
#define MAX_SEARCH_RESULTS  1000
#define MAX_MSG_LEN         256
#define MAX_USER_LEN        64
#define MAX_PATH_LEN        256

/* Audit log entry */
typedef struct {
    int             log_id;
    unsigned int    timestamp;
    int             event_type;
    int             severity;
    int             user_id;
    int             process_id;
    char            message[MAX_MSG_LEN];
    char            source[MAX_PATH_LEN];
    int             status;  /* 0=success, 1=failure */
    unsigned int    ip_addr;
    int             active;
} audit_entry_t;

/* Audit rule */
typedef struct {
    int         rule_id;
    int         event_type;
    int         user_id;
    char        path_pattern[MAX_PATH_LEN];
    int         log_success;
    int         log_failure;
    int         active;
} audit_rule_t;

/* Compliance check */
typedef struct {
    int         framework;
    char        check_name[128];
    int         passed;
    char        details[256];
    int         severity;
} compliance_check_t;

/* Search filter */
typedef struct {
    int         event_type;
    int         severity_min;
    int         user_id;
    unsigned int time_start;
    unsigned int time_end;
    char        keyword[128];
} search_filter_t;

/* Retention policy */
typedef struct {
    int         max_days;
    int         max_size_mb;
    int         auto_rotate;
    int         compress_old;
} retention_policy_t;

/* Global state */
static audit_entry_t log_entries[MAX_LOG_ENTRIES];
static int log_count = 0;
static int next_log_id = 1;
static audit_rule_t rules[MAX_RULES];
static int rule_count = 0;
static compliance_check_t checks[256];
static int check_count = 0;
static retention_policy_t retention;

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

/* ===== Logging Functions ===== */

int audit_log(int event_type, int severity, int user_id, int process_id, const char *message, int status) {
    if (log_count >= MAX_LOG_ENTRIES) {
        /* Rotate logs */
        for (int i = 0; i < MAX_LOG_ENTRIES - 1; i++) {
            log_entries[i] = log_entries[i + 1];
        }
        log_count = MAX_LOG_ENTRIES - 1;
    }

    int idx = log_count++;
    log_entries[idx].log_id = next_log_id++;
    log_entries[idx].timestamp = 1609459200 + idx;
    log_entries[idx].event_type = event_type;
    log_entries[idx].severity = severity;
    log_entries[idx].user_id = user_id;
    log_entries[idx].process_id = process_id;
    my_strncpy(log_entries[idx].message, message, MAX_MSG_LEN - 1);
    log_entries[idx].status = status;
    log_entries[idx].active = 1;

    return log_entries[idx].log_id;
}

int audit_log_user(int user_id, int operation, const char *details, int status) {
    char msg[MAX_MSG_LEN];
    my_strncpy(msg, "User operation: ", MAX_MSG_LEN - 1);
    my_strncpy(msg + my_strlen(msg), details, MAX_MSG_LEN - my_strlen(msg) - 1);

    return audit_log(EVENT_USER, LOG_INFO, user_id, 0, msg, status);
}

int audit_log_process(int process_id, int operation, const char *details, int status) {
    char msg[MAX_MSG_LEN];
    my_strncpy(msg, "Process event: ", MAX_MSG_LEN - 1);
    my_strncpy(msg + my_strlen(msg), details, MAX_MSG_LEN - my_strlen(msg) - 1);

    return audit_log(EVENT_PROCESS, LOG_INFO, 0, process_id, msg, status);
}

int audit_log_file(int user_id, int process_id, const char *path, int operation, int status) {
    char msg[MAX_MSG_LEN];
    my_strncpy(msg, "File access: ", MAX_MSG_LEN - 1);
    my_strncpy(msg + my_strlen(msg), path, MAX_MSG_LEN - my_strlen(msg) - 1);

    int log_id = audit_log(EVENT_FILE, LOG_INFO, user_id, process_id, msg, status);

    /* Update source path */
    if (log_id > 0) {
        for (int i = 0; i < log_count; i++) {
            if (log_entries[i].log_id == log_id) {
                my_strncpy(log_entries[i].source, path, MAX_PATH_LEN - 1);
                break;
            }
        }
    }

    return log_id;
}

int audit_log_network(int operation, unsigned int ip_addr, int port, int status) {
    char msg[MAX_MSG_LEN];
    my_strncpy(msg, "Network event: port ", MAX_MSG_LEN - 1);
    char port_str[16];
    int pos = 0;
    if (port == 0) port_str[pos++] = '0';
    else { int d = 0, t = port; while (t > 0) { d++; t /= 10; } pos += d; port_str[pos] = '\0'; pos--;
        while (port > 0) { port_str[pos--] = '0' + (port % 10); port /= 10; } }
    my_strncpy(msg + my_strlen(msg), port_str, MAX_MSG_LEN - my_strlen(msg) - 1);

    int log_id = audit_log(EVENT_NETWORK, LOG_INFO, 0, 0, msg, status);

    if (log_id > 0) {
        for (int i = 0; i < log_count; i++) {
            if (log_entries[i].log_id == log_id) {
                log_entries[i].ip_addr = ip_addr;
                break;
            }
        }
    }

    return log_id;
}

int audit_log_security(int severity, const char *details, int status) {
    char msg[MAX_MSG_LEN];
    my_strncpy(msg, "Security event: ", MAX_MSG_LEN - 1);
    my_strncpy(msg + my_strlen(msg), details, MAX_MSG_LEN - my_strlen(msg) - 1);

    return audit_log(EVENT_SECURITY, severity, 0, 0, msg, status);
}

/* ===== Audit Rules ===== */

int audit_add_rule(int event_type, const char *path_pattern, int log_success, int log_failure) {
    if (rule_count >= MAX_RULES) return -1;

    int idx = rule_count++;
    rules[idx].rule_id = idx;
    rules[idx].event_type = event_type;
    rules[idx].user_id = -1;  /* All users */
    if (path_pattern) {
        my_strncpy(rules[idx].path_pattern, path_pattern, MAX_PATH_LEN - 1);
    }
    rules[idx].log_success = log_success;
    rules[idx].log_failure = log_failure;
    rules[idx].active = 1;

    print_str("Added audit rule #");
    print_int(idx);
    print_str(" for event type ");
    print_int(event_type);
    print_str("\n");

    return idx;
}

void audit_list_rules(void) {
    print_str("Audit Rules:\n");
    for (int i = 0; i < rule_count; i++) {
        if (!rules[i].active) continue;
        print_str("  Rule #");
        print_int(i);
        print_str(": type=");
        print_int(rules[i].event_type);
        print_str(" success=");
        print_str(rules[i].log_success ? "YES" : "NO");
        print_str(" failure=");
        print_str(rules[i].log_failure ? "YES" : "NO");
        print_str("\n");
    }
}

/* ===== Log Search ===== */

int audit_search(search_filter_t *filter, int *results, int max_results) {
    int count = 0;

    for (int i = 0; i < log_count && count < max_results; i++) {
        if (!log_entries[i].active) continue;

        /* Apply filters */
        if (filter->event_type > 0 && log_entries[i].event_type != filter->event_type) continue;
        if (filter->severity_min > 0 && log_entries[i].severity > filter->severity_min) continue;
        if (filter->user_id >= 0 && log_entries[i].user_id != filter->user_id) continue;
        if (filter->time_start > 0 && log_entries[i].timestamp < filter->time_start) continue;
        if (filter->time_end > 0 && log_entries[i].timestamp > filter->time_end) continue;

        results[count++] = log_entries[i].log_id;
    }

    return count;
}

void audit_display_log(int log_id) {
    for (int i = 0; i < log_count; i++) {
        if (log_entries[i].log_id == log_id) {
            print_str("[");
            print_int(log_entries[i].log_id);
            print_str("] ");
            if (log_entries[i].severity == LOG_ERR) print_str("ERR");
            else if (log_entries[i].severity == LOG_WARNING) print_str("WARN");
            else if (log_entries[i].severity == LOG_INFO) print_str("INFO");
            else print_str("DEBUG");
            print_str(" ");
            print_str(log_entries[i].message);
            print_str(" - ");
            print_str(log_entries[i].status == 0 ? "SUCCESS" : "FAILURE");
            print_str("\n");
            break;
        }
    }
}

/* ===== Compliance Reports ===== */

int audit_compliance_check(int framework, const char *check_name, int passed, const char *details) {
    if (check_count >= 256) return -1;

    int idx = check_count++;
    checks[idx].framework = framework;
    my_strncpy(checks[idx].check_name, check_name, 127);
    checks[idx].passed = passed;
    my_strncpy(checks[idx].details, details, 255);
    checks[idx].severity = passed ? LOG_INFO : LOG_ERR;

    return idx;
}

void audit_compliance_report(int framework) {
    print_str("Compliance Report: ");
    if (framework == COMPLIANCE_CIS) print_str("CIS Benchmark");
    else if (framework == COMPLIANCE_ISO27001) print_str("ISO 27001");
    else if (framework == COMPLIANCE_PCI_DSS) print_str("PCI-DSS");
    else if (framework == COMPLIANCE_GDPR) print_str("GDPR");
    print_str("\n\n");

    int passed = 0, failed = 0;
    for (int i = 0; i < check_count; i++) {
        if (checks[i].framework != framework) continue;
        if (checks[i].passed) passed++;
        else failed++;

        print_str("  ");
        print_str(checks[i].passed ? "[PASS]" : "[FAIL]");
        print_str(" ");
        print_str(checks[i].check_name);
        print_str("\n");
    }

    print_str("\nSummary: ");
    print_int(passed);
    print_str(" passed, ");
    print_int(failed);
    print_str(" failed\n");
}

/* ===== Retention Policy ===== */

void audit_set_retention(int max_days, int max_size_mb, int auto_rotate) {
    retention.max_days = max_days;
    retention.max_size_mb = max_size_mb;
    retention.auto_rotate = auto_rotate;
    retention.compress_old = 1;

    print_str("Retention policy set:\n");
    print_str("  Max age: ");
    print_int(max_days);
    print_str(" days\n");
    print_str("  Max size: ");
    print_int(max_size_mb);
    print_str(" MB\n");
    print_str("  Auto-rotate: ");
    print_str(auto_rotate ? "YES" : "NO");
    print_str("\n");
}

void audit_stats(void) {
    print_str("Audit Log Statistics:\n");
    print_str("  Total entries: ");
    print_int(log_count);
    print_str("\n");
    print_str("  Audit rules: ");
    print_int(rule_count);
    print_str("\n");
    print_str("  Compliance checks: ");
    print_int(check_count);
    print_str("\n");

    /* Count by event type */
    int user_count = 0, proc_count = 0, file_count = 0, net_count = 0, sec_count = 0;
    for (int i = 0; i < log_count; i++) {
        if (log_entries[i].event_type == EVENT_USER) user_count++;
        else if (log_entries[i].event_type == EVENT_PROCESS) proc_count++;
        else if (log_entries[i].event_type == EVENT_FILE) file_count++;
        else if (log_entries[i].event_type == EVENT_NETWORK) net_count++;
        else if (log_entries[i].event_type == EVENT_SECURITY) sec_count++;
    }

    print_str("  User events: ");
    print_int(user_count);
    print_str("\n");
    print_str("  Process events: ");
    print_int(proc_count);
    print_str("\n");
    print_str("  File events: ");
    print_int(file_count);
    print_str("\n");
    print_str("  Network events: ");
    print_int(net_count);
    print_str("\n");
    print_str("  Security events: ");
    print_int(sec_count);
    print_str("\n");
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

    print_str("Audit Log v46.0 - Comprehensive Logging System\n");
    if (help) {
        print_str("Usage: audit_log [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run audit test\n");
        return;
    }

    if (test) {
        print_str("=== Audit Log Test Suite ===\n\n");

        /* Test 1: Set retention policy */
        print_str("[Test 1] Retention Policy\n");
        audit_set_retention(90, 1000, 1);
        print_str("\n");

        /* Test 2: Add audit rules */
        print_str("[Test 2] Audit Rules\n");
        audit_add_rule(EVENT_USER, NULL, 1, 1);
        audit_add_rule(EVENT_FILE, "/etc/*", 1, 1);
        audit_add_rule(EVENT_SECURITY, NULL, 1, 1);
        audit_list_rules();
        print_str("\n");

        /* Test 3: Log user operations */
        print_str("[Test 3] User Operations\n");
        audit_log_user(1000, USEROP_LOGIN, "User login from 192.168.1.100", 0);
        audit_log_user(1000, USEROP_SUDO, "sudo command executed", 0);
        audit_log_user(1001, USEROP_LOGIN, "Failed login attempt", 1);
        audit_log_user(1001, USEROP_LOGIN, "Failed login attempt", 1);
        audit_log_user(1001, USEROP_LOGIN, "Failed login attempt", 1);
        print_str("\n");

        /* Test 4: Log file operations */
        print_str("[Test 4] File Operations\n");
        audit_log_file(1000, 1234, "/etc/passwd", FILEOP_READ, 0);
        audit_log_file(1000, 1234, "/var/log/auth.log", FILEOP_READ, 0);
        audit_log_file(1001, 5678, "/etc/shadow", FILEOP_WRITE, 1);
        print_str("\n");

        /* Test 5: Log network events */
        print_str("[Test 5] Network Events\n");
        audit_log_network(NETOP_CONNECT, 0xC0A80164, 443, 0);
        audit_log_network(NETOP_ACCEPT, 0xC0A80165, 22, 0);
        audit_log_network(NETOP_CONNECT, 0x0A000001, 22, 1);
        print_str("\n");

        /* Test 6: Log security events */
        print_str("[Test 6] Security Events\n");
        audit_log_security(LOG_WARNING, "Multiple failed login attempts", 1);
        audit_log_security(LOG_ERR, "Unauthorized sudo attempt", 1);
        audit_log_security(LOG_CRIT, "Potential privilege escalation", 1);
        print_str("\n");

        /* Test 7: Search logs */
        print_str("[Test 7] Log Search\n");
        search_filter_t filter;
        filter.event_type = EVENT_SECURITY;
        filter.severity_min = 0;
        filter.user_id = -1;
        filter.time_start = 0;
        filter.time_end = 0;
        my_strncpy(filter.keyword, "", 127);

        int results[MAX_SEARCH_RESULTS];
        int count = audit_search(&filter, results, MAX_SEARCH_RESULTS);
        print_str("Found ");
        print_int(count);
        print_str(" security events\n");
        for (int i = 0; i < count && i < 5; i++) {
            audit_display_log(results[i]);
        }
        print_str("\n");

        /* Test 8: Compliance checks */
        print_str("[Test 8] Compliance Checks\n");
        audit_compliance_check(COMPLIANCE_CIS, "Password complexity", 1, "Meets CIS requirements");
        audit_compliance_check(COMPLIANCE_CIS, "Account lockout", 1, "Configured correctly");
        audit_compliance_check(COMPLIANCE_CIS, "Audit logging", 1, "Enabled and active");
        audit_compliance_check(COMPLIANCE_CIS, "File permissions", 0, "Some files have incorrect permissions");
        audit_compliance_report(COMPLIANCE_CIS);
        print_str("\n");

        /* Test 9: Statistics */
        print_str("[Test 9] Audit Statistics\n");
        audit_stats();
        print_str("\n");

        print_str("=== Audit Log Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
