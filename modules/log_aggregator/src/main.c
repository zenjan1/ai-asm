/* log_aggregator: Multi-source log aggregation (v33.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_LOG_ENTRIES  512
#define MAX_SOURCES      16
#define MAX_PATTERNS     32
#define MAX_MSG          128
#define MAX_SRC_NAME     32

/* Log levels */
#define LOG_DEBUG    0
#define LOG_INFO     1
#define LOG_WARN     2
#define LOG_ERROR    3
#define LOG_CRITICAL 4

/* Log entry */
typedef struct {
    int   entry_id;
    int   level;
    int   source_id;
    int   timestamp;
    char  message[MAX_MSG];
    int   flagged;           /* 1=matches anomaly pattern */
} log_entry_t;

/* Log source */
typedef struct {
    int   source_id;
    char  name[MAX_SRC_NAME];
    int   entry_count;
    int   error_count;
} log_source_t;

/* Alert pattern */
typedef struct {
    char  pattern[MAX_MSG];
    int   match_count;
    int   active;
} alert_pattern_t;

static log_entry_t entries[MAX_LOG_ENTRIES];
static int entry_count = 0;
static int next_entry_id = 1;

static log_source_t sources[MAX_SOURCES];
static int source_count = 0;

static alert_pattern_t patterns[MAX_PATTERNS];
static int pattern_count = 0;

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

static const char *level_name(int l) {
    if (l == LOG_DEBUG) return "DEBUG";
    if (l == LOG_INFO) return "INFO";
    if (l == LOG_WARN) return "WARN";
    if (l == LOG_ERROR) return "ERROR";
    return "CRITICAL";
}

/* Register log source */
int la_register_source(const char *name) {
    if (source_count >= MAX_SOURCES) return -1;
    int idx = source_count;
    sources[idx].source_id = idx;
    my_strncpy(sources[idx].name, name, MAX_SRC_NAME - 1);
    sources[idx].entry_count = 0;
    sources[idx].error_count = 0;
    source_count++;
    return sources[idx].source_id;
}

/* Add alert pattern */
int la_add_pattern(const char *pattern) {
    if (pattern_count >= MAX_PATTERNS) return -1;
    int idx = pattern_count;
    my_strncpy(patterns[idx].pattern, pattern, MAX_MSG - 1);
    patterns[idx].match_count = 0;
    patterns[idx].active = 1;
    pattern_count++;
    return idx;
}

/* Add log entry */
int la_add(int source_id, int level, const char *message) {
    if (entry_count >= MAX_LOG_ENTRIES) return -1;
    int idx = entry_count;
    entries[idx].entry_id = next_entry_id++;
    entries[idx].level = level;
    entries[idx].source_id = source_id;
    entries[idx].timestamp = entry_count + 1;
    my_strncpy(entries[idx].message, message, MAX_MSG - 1);
    entries[idx].flagged = 0;
    /* Check against alert patterns */
    for (int i = 0; i < pattern_count; i++) {
        if (patterns[i].active && my_strstr(message, patterns[i].pattern)) {
            entries[idx].flagged = 1;
            patterns[i].match_count++;
        }
    }
    entry_count++;
    /* Update source stats */
    for (int i = 0; i < source_count; i++) {
        if (sources[i].source_id == source_id) {
            sources[i].entry_count++;
            if (level >= LOG_ERROR) sources[i].error_count++;
            break;
        }
    }
    return entries[idx].entry_id;
}

/* Search logs by keyword */
int la_search(const char *keyword) {
    print_str("  Search: \"");
    print_str(keyword);
    print_str("\"\n");
    int found = 0;
    for (int i = 0; i < entry_count; i++) {
        if (my_strstr(entries[i].message, keyword)) {
            print_str("  #");
            print_int(entries[i].entry_id);
            print_str(" [");
            print_str(level_name(entries[i].level));
            print_str("] ");
            print_str(entries[i].message);
            if (entries[i].flagged) print_str(" [FLAGGED]");
            print_str("\n");
            found++;
        }
    }
    print_str("  Found: ");
    print_int(found);
    print_str(" entries\n");
    return found;
}

/* Filter by level */
int la_filter(int min_level) {
    int count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level >= min_level) {
            print_str("  #");
            print_int(entries[i].entry_id);
            print_str(" [");
            print_str(level_name(entries[i].level));
            print_str("] ");
            print_str(entries[i].message);
            print_str("\n");
            count++;
        }
    }
    return count;
}

/* Source statistics */
int la_source_stats(void) {
    print_str("  Source Statistics\n");
    print_str("  =================================================\n");
    for (int i = 0; i < source_count; i++) {
        print_str("  ");
        print_str(sources[i].name);
        print_str(": ");
        print_int(sources[i].entry_count);
        print_str(" entries, ");
        print_int(sources[i].error_count);
        print_str(" errors\n");
    }
    return source_count;
}

/* Pattern match report */
int la_pattern_report(void) {
    print_str("  Pattern Match Report\n");
    print_str("  =================================================\n");
    int total_matches = 0;
    for (int i = 0; i < pattern_count; i++) {
        if (patterns[i].active) {
            print_str("  \"");
            print_str(patterns[i].pattern);
            print_str("\": ");
            print_int(patterns[i].match_count);
            print_str(" matches\n");
            total_matches += patterns[i].match_count;
        }
    }
    print_str("  Total flagged entries: ");
    print_int(total_matches);
    print_str("\n");
    return total_matches;
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
    print_str("Log Aggregator v33.0 - Multi-Source Log Aggregation\n");
    if (help) {
        print_str("Usage: log_aggregator [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run log aggregator test\n");
        return;
    }
    if (test) {
        print_str("=== Log Aggregator Test ===\n\n");
        /* Register sources */
        int s0 = la_register_source("kernel");
        int s1 = la_register_source("wasm_runtime");
        int s2 = la_register_source("network");
        int s3 = la_register_source("agent");
        print_str("  Sources: "); print_int(source_count); print_str("\n\n");
        /* Add alert patterns */
        la_add_pattern("timeout");
        la_add_pattern("segfault");
        la_add_pattern("unauthorized");
        la_add_pattern("out of memory");
        print_str("\n");
        /* Add log entries */
        la_add(s0, LOG_INFO, "Kernel initialized successfully");
        la_add(s0, LOG_WARN, "High CPU temperature detected");
        la_add(s0, LOG_ERROR, "segfault at address 0x1234");
        la_add(s1, LOG_INFO, "WASM module loaded: ls");
        la_add(s1, LOG_INFO, "WASM module loaded: grep");
        la_add(s1, LOG_ERROR, "WASM execution timeout after 5000ms");
        la_add(s2, LOG_WARN, "Connection timeout to 10.0.0.1");
        la_add(s2, LOG_ERROR, "DNS resolution failed for example.com");
        la_add(s3, LOG_INFO, "Agent created: coordinator");
        la_add(s3, LOG_WARN, "unauthorized access attempt detected");
        la_add(s3, LOG_CRITICAL, "out of memory in agent heap");
        print_str("  Entries: "); print_int(entry_count); print_str("\n\n");
        /* Filter errors and above */
        print_str("Errors and above:\n");
        la_filter(LOG_ERROR);
        print_str("\n");
        /* Search */
        la_search("timeout");
        print_str("\n");
        /* Source stats */
        la_source_stats();
        print_str("\n");
        /* Pattern report */
        la_pattern_report();
        print_str("\n=== Log Aggregator Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
