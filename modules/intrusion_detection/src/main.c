/* intrusion_detection: Real-time threat detection (v34.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_THREATS    128
#define MAX_RULES      64
#define MAX_RESPONSES  64
#define MAX_MSG        128
#define MAX_PATTERN    64

/* Threat types */
#define THREAT_MALWARE     0
#define THREAT_INTRUSION   1
#define THREAT_DOS         2
#define THREAT_DATA_EXFIL  3
#define THREAT_PRIV_ESC    4
#define THREAT_ANOMALY     5

/* Threat severity */
#define SEV_LOW       0
#define SEV_MEDIUM    1
#define SEV_HIGH      2
#define SEV_CRITICAL  3

/* Response actions */
#define ACTION_BLOCK   0
#define ACTION_ALERT   1
#define ACTION_LOG     2
#define ACTION_QUARANTINE 3
#define ACTION_TERMINATE   4

/* Resolution states */
#define RES_PENDING    0
#define RES_INVESTIGATING 1
#define RES_RESOLVED   2
#define RES_FALSE_POSITIVE 3

/* Threat entry */
typedef struct {
    int   threat_id;
    int   type;
    int   severity;
    char  pattern[MAX_PATTERN];
    int   timestamp;
    int   resolved;
    int   confidence;     /* 0-100 threat confidence */
} threat_entry_t;

/* Detection rule */
typedef struct {
    int   rule_id;
    char  pattern[MAX_PATTERN];
    int   action;
    int   threshold;      /* match count before trigger */
    int   enabled;
    int   match_count;
} detection_rule_t;

/* Response log */
typedef struct {
    int   log_id;
    int   threat_id;
    int   action_taken;
    int   result;         /* 0=success, -1=failed */
    int   timestamp;
} response_log_t;

static threat_entry_t threats[MAX_THREATS];
static int threat_count = 0;
static int next_threat_id = 1;

static detection_rule_t rules[MAX_RULES];
static int rule_count = 0;
static int next_rule_id = 1;

static response_log_t responses[MAX_RESPONSES];
static int response_count = 0;
static int next_response_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i] || a[i] != b[i]) return (a[i] ? a[i] : 0) - (b[i] ? b[i] : 0);
    } return 0;
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        if (my_strncmp(&haystack[i], needle, nl) == 0) return 1;
    }
    return 0;
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

static const char *threat_type_name(int t) {
    if (t == THREAT_MALWARE) return "MALWARE";
    if (t == THREAT_INTRUSION) return "INTRUSION";
    if (t == THREAT_DOS) return "DoS";
    if (t == THREAT_DATA_EXFIL) return "DATA_EXFIL";
    if (t == THREAT_PRIV_ESC) return "PRIV_ESC";
    return "ANOMALY";
}

static const char *severity_name(int s) {
    if (s == SEV_LOW) return "LOW";
    if (s == SEV_MEDIUM) return "MEDIUM";
    if (s == SEV_HIGH) return "HIGH";
    return "CRITICAL";
}

static const char *action_name(int a) {
    if (a == ACTION_BLOCK) return "BLOCK";
    if (a == ACTION_ALERT) return "ALERT";
    if (a == ACTION_LOG) return "LOG";
    if (a == ACTION_QUARANTINE) return "QUARANTINE";
    return "TERMINATE";
}

static const char *resolution_name(int r) {
    if (r == RES_PENDING) return "PENDING";
    if (r == RES_INVESTIGATING) return "INVESTIGATING";
    if (r == RES_RESOLVED) return "RESOLVED";
    return "FALSE_POSITIVE";
}

/* Add detection rule */
int ids_add_rule(const char *pattern, int action, int threshold) {
    if (rule_count >= MAX_RULES) return -1;
    int idx = rule_count;
    rules[idx].rule_id = next_rule_id++;
    my_strncpy(rules[idx].pattern, pattern, MAX_PATTERN - 1);
    rules[idx].action = action;
    rules[idx].threshold = threshold;
    rules[idx].enabled = 1;
    rules[idx].match_count = 0;
    rule_count++;
    return rules[idx].rule_id;
}

/* Analyze event against rules, returns threat_id or -1 */
int ids_analyze(const char *event, int source_type) {
    int detected_threat = -1;
    for (int i = 0; i < rule_count; i++) {
        if (!rules[i].enabled) continue;
        if (my_strstr(event, rules[i].pattern)) {
            rules[i].match_count++;
            if (rules[i].match_count >= rules[i].threshold) {
                /* Determine threat type and severity from source */
                int threat_type = THREAT_ANOMALY;
                int severity = SEV_MEDIUM;
                if (source_type == THREAT_MALWARE) { threat_type = THREAT_MALWARE; severity = SEV_HIGH; }
                else if (source_type == THREAT_INTRUSION) { threat_type = THREAT_INTRUSION; severity = SEV_CRITICAL; }
                else if (source_type == THREAT_DOS) { threat_type = THREAT_DOS; severity = SEV_HIGH; }
                else if (source_type == THREAT_DATA_EXFIL) { threat_type = THREAT_DATA_EXFIL; severity = SEV_CRITICAL; }
                else if (source_type == THREAT_PRIV_ESC) { threat_type = THREAT_PRIV_ESC; severity = SEV_CRITICAL; }
                /* Create threat entry */
                if (threat_count < MAX_THREATS) {
                    int tidx = threat_count;
                    threats[tidx].threat_id = next_threat_id++;
                    threats[tidx].type = threat_type;
                    threats[tidx].severity = severity;
                    my_strncpy(threats[tidx].pattern, event, MAX_PATTERN - 1);
                    threats[tidx].timestamp = threat_count + 1;
                    threats[tidx].resolved = RES_PENDING;
                    threats[tidx].confidence = rules[i].match_count * 20;
                    if (threats[tidx].confidence > 100) threats[tidx].confidence = 100;
                    threat_count++;
                    detected_threat = threats[tidx].threat_id;
                    /* Auto-response */
                    if (response_count < MAX_RESPONSES) {
                        int ridx = response_count;
                        responses[ridx].log_id = next_response_id++;
                        responses[ridx].threat_id = detected_threat;
                        responses[ridx].action_taken = rules[i].action;
                        responses[ridx].result = 0;
                        responses[ridx].timestamp = response_count + 1;
                        response_count++;
                    }
                }
                break;
            }
        }
    }
    return detected_threat;
}

/* Show threat report */
int ids_report(void) {
    print_str("  Threat Report\n");
    print_str("  ================================================================\n");
    print_str("  ID  Type           Severity  Confidence  Status\n");
    print_str("  ----------------------------------------------------------------\n");
    for (int i = 0; i < threat_count; i++) {
        print_int(threats[i].threat_id);
        print_str("  ");
        print_str(threat_type_name(threats[i].type));
        int len = my_strlen(threat_type_name(threats[i].type));
        for (int p = 0; p < 15 - len; p++) print_str(" ");
        print_str(severity_name(threats[i].severity));
        print_str("  ");
        print_int(threats[i].confidence);
        print_str("%        ");
        print_str(resolution_name(threats[i].resolved));
        print_str("\n");
    }
    print_str("  Total threats: ");
    print_int(threat_count);
    print_str("\n");
    return threat_count;
}

/* Resolve threat */
int ids_resolve(int threat_id, int resolution) {
    for (int i = 0; i < threat_count; i++) {
        if (threats[i].threat_id == threat_id) {
            threats[i].resolved = resolution;
            print_str("  Threat #");
            print_int(threat_id);
            print_str(" marked as ");
            print_str(resolution_name(resolution));
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Rules listing */
int ids_list_rules(void) {
    print_str("  Detection Rules\n");
    print_str("  ================================================================\n");
    print_str("  ID  Pattern            Action       Threshold  Matches  Enabled\n");
    print_str("  ----------------------------------------------------------------\n");
    for (int i = 0; i < rule_count; i++) {
        print_int(rules[i].rule_id);
        print_str("  ");
        print_str(rules[i].pattern);
        int len = my_strlen(rules[i].pattern);
        for (int p = 0; p < 19 - len; p++) print_str(" ");
        print_str(action_name(rules[i].action));
        if (rules[i].action == ACTION_BLOCK) print_str("      ");
        else if (rules[i].action == ACTION_ALERT) print_str("     ");
        else if (rules[i].action == ACTION_LOG) print_str("         ");
        else if (rules[i].action == ACTION_QUARANTINE) print_str("  ");
        else print_str("  ");
        print_int(rules[i].threshold);
        print_str("         ");
        print_int(rules[i].match_count);
        print_str("      ");
        if (rules[i].enabled) print_str("YES");
        else print_str("NO");
        print_str("\n");
    }
    return rule_count;
}

/* Response log */
int ids_response_log(void) {
    print_str("  Response Actions\n");
    print_str("  ================================================================\n");
    for (int i = 0; i < response_count; i++) {
        print_str("  #");
        print_int(responses[i].log_id);
        print_str(" Threat #");
        print_int(responses[i].threat_id);
        print_str(" -> ");
        print_str(action_name(responses[i].action_taken));
        print_str(" (");
        if (responses[i].result == 0) print_str("success");
        else print_str("failed");
        print_str(")\n");
    }
    return response_count;
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
    print_str("Intrusion Detection v34.0 - Real-Time Threat Detection\n");
    if (help) {
        print_str("Usage: intrusion_detection [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run intrusion detection test\n");
        return;
    }
    if (test) {
        print_str("=== Intrusion Detection Test ===\n\n");
        /* Add detection rules */
        ids_add_rule("SQL injection", ACTION_BLOCK, 1);
        ids_add_rule("XSS attempt", ACTION_BLOCK, 1);
        ids_add_rule("port scan", ACTION_ALERT, 3);
        ids_add_rule("brute force", ACTION_TERMINATE, 5);
        ids_add_rule("data exfiltration", ACTION_QUARANTINE, 1);
        ids_add_rule("privilege escalation", ACTION_BLOCK, 1);
        print_str("  Rules loaded: "); print_int(rule_count); print_str("\n\n");
        /* Simulate events */
        print_str("Analyzing events:\n");
        ids_analyze("GET /api?id=1 OR 1=1 -- SQL injection detected", THREAT_INTRUSION);
        ids_analyze("<script>alert('XSS attempt')</script>", THREAT_INTRUSION);
        ids_analyze("SYN packet from 10.0.0.5: port scan detected on ports 22,80,443", THREAT_INTRUSION);
        ids_analyze("SYN packet from 10.0.0.5: port scan on port 8080", THREAT_INTRUSION);
        ids_analyze("SYN packet from 10.0.0.5: port scan on port 3306", THREAT_INTRUSION);
        ids_analyze("Large outbound transfer: data exfiltration suspected", THREAT_DATA_EXFIL);
        ids_analyze("sudo exploit: privilege escalation attempt", THREAT_PRIV_ESC);
        print_str("  Threats detected: "); print_int(threat_count); print_str("\n\n");
        /* Threat report */
        ids_report();
        print_str("\n");
        /* Rules listing */
        ids_list_rules();
        print_str("\n");
        /* Response log */
        ids_response_log();
        print_str("\n");
        /* Resolve some */
        ids_resolve(1, RES_RESOLVED);
        ids_resolve(2, RES_FALSE_POSITIVE);
        print_str("\n=== Intrusion Detection Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
