/* meta_cognition: Meta-cognition system (v17.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Meta levels */
#define META_LEVEL_0  0
#define META_LEVEL_1  1
#define META_LEVEL_2  2
#define META_LEVEL_3  3
#define META_LEVEL_4  4

/* Limits */
#define MAX_STATES       32
#define MAX_DECISIONS    32
#define TASK_LEN         128
#define STRATEGY_LEN     64
#define OBSTACLES_LEN    256
#define ADJUSTMENTS_LEN  256
#define APPROACH_LEN     128
#define OUTCOME_LEN      256
#define FALLBACK_LEN     128
#define PROB_TYPE_LEN    64

/* Cognition state */
typedef struct {
    int   cognition_id;
    char  current_task[TASK_LEN];
    int   understanding_level;
    int   confidence_level;
    int   effort_level;
    char  strategy_used[STRATEGY_LEN];
    char  obstacles[OBSTACLES_LEN];
    char  adjustments[ADJUSTMENTS_LEN];
} cognition_state_t;

/* Meta decision */
typedef struct {
    int   decision_id;
    char  problem_type[PROB_TYPE_LEN];
    char  approach[APPROACH_LEN];
    char  expected_outcome[OUTCOME_LEN];
    int   monitoring_points;
    char  fallback_strategy[FALLBACK_LEN];
} meta_decision_t;

static cognition_state_t states[MAX_STATES];
static meta_decision_t decisions[MAX_DECISIONS];
static int state_count = 0;
static int decision_count = 0;
static int next_cognition_id = 1;
static int next_decision_id = 1;

/* Tracking */
static int meta_level = META_LEVEL_2;
static int total_assessments = 0;
static int total_adjustments = 0;

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

/* Get meta level name */
static const char *meta_level_name(int level) {
    if (level == META_LEVEL_0) return "none";
    if (level == META_LEVEL_1) return "monitoring";
    if (level == META_LEVEL_2) return "self-assess";
    if (level == META_LEVEL_3) return "strategy-select";
    if (level == META_LEVEL_4) return "full-meta";
    return "unknown";
}

/* Assess understanding */
int meta_assess_understanding(void) {
    print_str("[meta] assessing understanding...\n");
    print_str("  Meta level: ");
    print_str(meta_level_name(meta_level));
    print_str("\n");
    print_str("  Current states tracked: ");
    print_int(state_count);
    print_str("/");
    print_int(MAX_STATES);
    print_str("\n");
    print_str("  Assessments made: ");
    print_int(total_assessments);
    print_str("\n");

    /* Self-assessment simulation */
    if (state_count > 0) {
        int avg_understanding = 0;
        for (int i = 0; i < state_count; i++) avg_understanding += states[i].understanding_level;
        avg_understanding /= state_count;
        print_str("  Average understanding: ");
        print_int(avg_understanding);
        print_str("%\n");
    }
    total_assessments++;
    return 0;
}

/* Select strategy for task type */
int meta_select_strategy(const char *task_type) {
    if (!task_type) return -1;
    print_str("[meta] select strategy: task=");
    print_str(task_type);
    print_str("\n");

    const char *strategy = "default";
    if (my_strncmp(task_type, "classify", 8) == 0) strategy = "pattern_match";
    else if (my_strncmp(task_type, "reason", 6) == 0) strategy = "chain_build";
    else if (my_strncmp(task_type, "learn", 5) == 0) strategy = "experience_record";
    else if (my_strncmp(task_type, "create", 6) == 0) strategy = "divergent_think";

    print_str("  Strategy selected: ");
    print_str(strategy);
    print_str("\n");

    /* Record decision */
    if (decision_count < MAX_DECISIONS) {
        int idx = decision_count;
        decisions[idx].decision_id = next_decision_id++;
        my_strncpy(decisions[idx].problem_type, task_type, PROB_TYPE_LEN - 1);
        my_strncpy(decisions[idx].approach, strategy, APPROACH_LEN - 1);
        my_strcpy(decisions[idx].expected_outcome, "task completed successfully");
        decisions[idx].monitoring_points = 3;
        my_strcpy(decisions[idx].fallback_strategy, "fall back to default");
        decision_count++;
    }
    return 0;
}

/* Monitor progress */
int meta_monitor_progress(int cognition_id) {
    for (int i = 0; i < state_count; i++) {
        if (states[i].cognition_id == cognition_id) {
            print_str("[meta] monitor: task=");
            print_str(states[i].current_task);
            print_str(" understanding=");
            print_int(states[i].understanding_level);
            print_str(" confidence=");
            print_int(states[i].confidence_level);
            print_str(" effort=");
            print_int(states[i].effort_level);
            print_str("\n");
            return 0;
        }
    }
    print_str("[meta] cognition state not found: ");
    print_int(cognition_id);
    print_str("\n");
    return -1;
}

/* Adjust approach based on feedback */
int meta_adjust_approach(const char *feedback) {
    if (!feedback) return -1;
    print_str("[meta] adjust based on feedback: ");
    print_str(feedback);
    print_str("\n");

    /* Simulate adjustment */
    if (state_count > 0) {
        int idx = state_count - 1;
        my_strncpy(states[idx].adjustments, feedback, ADJUSTMENTS_LEN - 1);
        states[idx].effort_level += 10;
        if (states[idx].effort_level > 100) states[idx].effort_level = 100;
    }
    total_adjustments++;
    print_str("  Approach adjusted\n");
    return 0;
}

/* Self evaluate */
int meta_self_evaluate(void) {
    print_str("=== Self Evaluation ===\n");
    print_str("Meta level: ");
    print_str(meta_level_name(meta_level));
    print_str("\n");
    print_str("Total assessments: ");
    print_int(total_assessments);
    print_str("\n");
    print_str("Total adjustments: ");
    print_int(total_adjustments);
    print_str("\n");
    print_str("Active states: ");
    print_int(state_count);
    print_str("\n");
    print_str("Meta decisions: ");
    print_int(decision_count);
    print_str("\n");
    return 0;
}

/* Get cognition report */
int meta_get_cognition_report(void) {
    print_str("=== Cognition Report ===\n");
    for (int i = 0; i < state_count; i++) {
        print_str("  State ");
        print_int(states[i].cognition_id);
        print_str(": ");
        print_str(states[i].current_task);
        print_str("\n    understanding=");
        print_int(states[i].understanding_level);
        print_str(" confidence=");
        print_int(states[i].confidence_level);
        print_str(" strategy=");
        print_str(states[i].strategy_used);
        print_str("\n");
    }
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
    print_str("Meta Cognition v17.0 - Self-Awareness System\n");
    if (help) {
        print_str("Usage: meta_cognition [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run cognition test\n");
        return;
    }
    if (test) {
        print_str("=== Meta Cognition Test ===\n");
        print_str("\n--- Assess Understanding ---\n");
        meta_assess_understanding();

        print_str("\n--- Select Strategies ---\n");
        meta_select_strategy("classify");
        meta_select_strategy("reason");
        meta_select_strategy("learn");

        print_str("\n--- Record Cognition States ---\n");
        states[0].cognition_id = next_cognition_id++; my_strcpy(states[0].current_task, "image classification");
        states[0].understanding_level = 85; states[0].confidence_level = 78; states[0].effort_level = 40;
        my_strcpy(states[0].strategy_used, "pattern_match"); my_strcpy(states[0].obstacles, "low light conditions");
        my_strcpy(states[0].adjustments, "increase contrast"); state_count++;

        states[1].cognition_id = next_cognition_id++; my_strcpy(states[1].current_task, "logical reasoning");
        states[1].understanding_level = 90; states[1].confidence_level = 82; states[1].effort_level = 60;
        my_strcpy(states[1].strategy_used, "chain_build"); my_strcpy(states[1].obstacles, "ambiguous premise");
        my_strcpy(states[1].adjustments, "clarify assumptions"); state_count++;

        print_str("\n--- Monitor Progress ---\n");
        meta_monitor_progress(1);
        meta_monitor_progress(2);

        print_str("\n--- Adjust Approach ---\n");
        meta_adjust_approach("try different strategy for low confidence tasks");

        print_str("\n--- Self Evaluate ---\n");
        meta_self_evaluate();

        print_str("\n--- Cognition Report ---\n");
        meta_get_cognition_report();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
