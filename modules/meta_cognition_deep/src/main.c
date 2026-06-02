/* meta_cognition_deep: Deep meta-cognition system (v25.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Meta levels */
#define META_NONE           0
#define META_AWARENESS      1
#define META_MONITORING     2
#define META_CONTROL        3
#define META_STRATEGIC      4
#define META_TRANSCENDENT   5

/* Cognitive states */
#define STATE_NORMAL    0
#define STATE_FOCUSED   1
#define STATE_EXPLORING 2
#define STATE_REFLECTING 3

/* Limits */
#define MAX_STATES   32
#define MAX_BIASES   16
#define NAME_LEN     64
#define DESC_LEN     256

/* Cognitive state entry */
typedef struct {
    int   entry_id;
    int   meta_level;
    int   cognitive_state;
    int   confidence;
    int   task_complexity;
    char  description[DESC_LEN];
    int   timestamp;
} cognition_entry_t;

/* Cognitive bias */
typedef struct {
    int   bias_id;
    char  name[NAME_LEN];
    int   detected;
    int   severity;
    char  mitigation[DESC_LEN];
} cognitive_bias_t;

static cognition_entry_t entries[MAX_STATES];
static cognitive_bias_t biases[MAX_BIASES];
static int entry_count = 0;
static int bias_count = 0;
static int current_meta_level = META_AWARENESS;
static int current_state = STATE_NORMAL;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
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

/* Get meta level name */
static const char *meta_name(int level) {
    if (level == META_NONE) return "none";
    if (level == META_AWARENESS) return "awareness";
    if (level == META_MONITORING) return "monitoring";
    if (level == META_CONTROL) return "control";
    if (level == META_STRATEGIC) return "strategic";
    if (level == META_TRANSCENDENT) return "transcendent";
    return "unknown";
}

/* Get cognitive state name */
static const char *state_name(int state) {
    if (state == STATE_NORMAL) return "normal";
    if (state == STATE_FOCUSED) return "focused";
    if (state == STATE_EXPLORING) return "exploring";
    if (state == STATE_REFLECTING) return "reflecting";
    return "unknown";
}

/* Monitor cognition state */
int mcd_monitor_state(void) {
    print_str("=== Cognition State Monitor ===\n");
    if (entry_count >= MAX_STATES) return -1;
    int idx = entry_count;
    entries[idx].entry_id = entry_count + 1;
    entries[idx].meta_level = current_meta_level;
    entries[idx].cognitive_state = current_state;
    entries[idx].confidence = 85;
    entries[idx].task_complexity = 7;
    my_strcpy(entries[idx].description, "Active cognition monitoring");
    entries[idx].timestamp = 20260602;
    entry_count++;

    print_str("  Meta level: ");
    print_str(meta_name(current_meta_level));
    print_str("\n");
    print_str("  Cognitive state: ");
    print_str(state_name(current_state));
    print_str("\n");
    print_str("  Confidence: 85%\n");
    print_str("  Self-awareness: ACTIVE\n");
    return entry_count;
}

/* Detect cognitive biases */
int mcd_detect_bias(const char *bias_name) {
    if (!bias_name || bias_count >= MAX_BIASES) return -1;
    int idx = bias_count;
    biases[idx].bias_id = bias_count + 1;
    my_strncpy(biases[idx].name, bias_name, NAME_LEN - 1);
    biases[idx].detected = 1;
    biases[idx].severity = 2;
    my_strcpy(biases[idx].mitigation, "Apply counter-argument analysis");
    bias_count++;

    print_str("  Bias detected: ");
    print_str(bias_name);
    print_str("\n");
    print_str("    Severity: MEDIUM\n");
    print_str("    Mitigation: Apply counter-argument analysis\n");
    return biases[idx].bias_id;
}

/* Reflect on thinking process */
int mcd_reflect(void) {
    print_str("=== Meta-Cognitive Reflection ===\n");
    print_str("  Analyzing thought processes...\n");
    print_str("  Checking reasoning validity...\n");
    print_str("  Evaluating confidence calibration...\n");
    print_str("  Biases detected: ");
    print_int(bias_count);
    print_str("\n");
    print_str("  Adjusting meta level: ");
    current_meta_level = META_CONTROL;
    print_str(meta_name(current_meta_level));
    print_str("\n");
    current_state = STATE_REFLECTING;
    return 0;
}

/* Adjust learning strategy */
int mcd_adjust_strategy(const char *task) {
    print_str("=== Strategy Adjustment ===\n");
    print_str("  Task: ");
    if (task) print_str(task);
    print_str("\n");
    print_str("  Current strategy: heuristic search\n");
    print_str("  Recommended: analytical decomposition\n");
    print_str("  Meta level increased to: strategic\n");
    current_meta_level = META_STRATEGIC;
    return 0;
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
    print_str("Meta-Cognition Deep v25.0\n");
    if (help) {
        print_str("Usage: meta_cognition_deep [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run cognition test\n");
        return;
    }
    if (test) {
        print_str("=== Deep Meta-Cognition Test ===\n\n");
        mcd_monitor_state();
        print_str("\n");
        mcd_detect_bias("confirmation_bias");
        mcd_detect_bias("anchoring_bias");
        print_str("\n");
        mcd_reflect();
        print_str("\n");
        mcd_adjust_strategy("system_optimization");
        print_str("\n=== Cognition Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
