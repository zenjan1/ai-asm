/* self_reflection: Self-reflection and self-correction system (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_CRITERIA    16
#define MAX_ASSESSMENTS 64
#define MAX_OUTPUT      1024
#define MAX_SUGGEST     256
#define MAX_REASON      256
#define MAX_NAME        64
#define MAX_DESC        128
#define MAX_HISTORY     32

/* Reflection criteria */
typedef struct {
    int   criteria_id;
    char  name[MAX_NAME];
    int   weight;     /* * 100 */
    char  description[MAX_DESC];
    int   scoring_method;  /* 0=percentage (0-100) */
} reflection_criteria_t;

/* Reflection assessment */
typedef struct {
    int   assessment_id;
    int   output_id;
    int   criteria_scores[MAX_CRITERIA];  /* * 100 */
    int   criteria_count;
    int   overall_score;  /* * 100 */
    char  issues[MAX_SUGGEST];
    char  suggestions[MAX_SUGGEST];
    int   created_at;
} reflection_assessment_t;

/* Correction */
typedef struct {
    int   correction_id;
    char  original_output[MAX_OUTPUT];
    char  corrected_output[MAX_OUTPUT];
    char  reason[MAX_REASON];
    int   improvement_delta;  /* score difference * 100 */
    int   created_at;
} correction_t;

/* Reflection history */
typedef struct {
    int   item_id;
    char  output_type[MAX_NAME];
    int   original_score;
    int   corrected_score;
    int   iterations;
    int   final_score;
    int   timestamp;
} reflection_history_t;

static reflection_criteria_t criteria[MAX_CRITERIA];
static reflection_assessment_t assessments[MAX_ASSESSMENTS];
static correction_t corrections[MAX_ASSESSMENTS];
static reflection_history_t history[MAX_HISTORY];
static int criteria_count = 0;
static int next_criteria_id = 1;
static int assessment_count = 0;
static int next_assessment_id = 1;
static int correction_count = 0;
static int history_count = 0;
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

/* Initialize default criteria */
int sr_init(void) {
    sr_add_criteria("Accuracy", 3000, 0);
    sr_add_criteria("Clarity", 2000, 0);
    sr_add_criteria("Completeness", 2000, 0);
    sr_add_criteria("Relevance", 1500, 0);
    sr_add_criteria("Efficiency", 1500, 0);
    return criteria_count;
}

/* Add criteria */
int sr_add_criteria(const char *name, int weight, int method) {
    if (criteria_count >= MAX_CRITERIA) return -1;
    int idx = criteria_count++;
    criteria[idx].criteria_id = next_criteria_id++;
    my_strncpy(criteria[idx].name, name, MAX_NAME - 1);
    criteria[idx].weight = weight;
    criteria[idx].description[0] = '\0';
    criteria[idx].scoring_method = method;
    return criteria[idx].criteria_id;
}

/* Assess output */
int sr_assess(const char *output, reflection_assessment_t *assess) {
    assessment_count++;
    int idx = assessment_count - 1;
    assessments[idx].assessment_id = next_assessment_id++;
    assessments[idx].output_id = idx + 1;
    assessments[idx].criteria_count = criteria_count;
    assessments[idx].created_at = ++clock;
    /* Simulate scoring based on output content */
    int output_len = my_strlen(output);
    int total_score = 0, total_weight = 0;
    for (int i = 0; i < criteria_count; i++) {
        /* Simulated score based on output quality heuristics */
        int score = 6000 + (output_len % 3000);
        if (my_strstr(output, "error") || my_strstr(output, "bug")) score -= 2000;
        if (my_strstr(output, "complete") || my_strstr(output, "comprehensive")) score += 1000;
        if (output_len > 100) score += 500;
        assessments[idx].criteria_scores[i] = score;
        total_score += score * criteria[i].weight;
        total_weight += criteria[i].weight;
    }
    assessments[idx].overall_score = total_weight > 0 ? total_score / total_weight : 0;
    /* Generate issues and suggestions */
    if (output_len < 50) {
        my_strncpy(assessments[idx].issues, "Output too short, may lack detail", MAX_SUGGEST - 1);
        my_strncpy(assessments[idx].suggestions, "Expand explanation with more context and examples", MAX_SUGGEST - 1);
    } else if (assessments[idx].overall_score < 7000) {
        my_strncpy(assessments[idx].issues, "Below quality threshold, review needed", MAX_SUGGEST - 1);
        my_strncpy(assessments[idx].suggestions, "Improve clarity and add supporting evidence", MAX_SUGGEST - 1);
    } else {
        my_strncpy(assessments[idx].issues, "Minor issues detected", MAX_SUGGEST - 1);
        my_strncpy(assessments[idx].suggestions, "Consider adding examples for clarity", MAX_SUGGEST - 1);
    }
    return assessments[idx].overall_score;
}

/* Suggest improvements */
int sr_suggest_improvements(int assessment_id, char *suggestions, int max_len) {
    for (int i = 0; i < assessment_count; i++) {
        if (assessments[i].assessment_id == assessment_id) {
            my_strncpy(suggestions, assessments[i].suggestions, max_len - 1);
            return 0;
        }
    }
    return -1;
}

/* Apply correction */
int sr_apply_correction(const char *original, const char *suggestion, char *corrected, int max_len) {
    correction_count++;
    int idx = correction_count - 1;
    corrections[idx].correction_id = idx + 1;
    my_strncpy(corrections[idx].original_output, original, MAX_OUTPUT - 1);
    /* Simulate correction by appending improvement */
    my_strncpy(corrected, original, max_len - 1);
    int len = my_strlen(corrected);
    if (len < max_len - 50) {
        my_strncpy(&corrected[len], " [Improved: ", max_len - len - 1);
        len = my_strlen(corrected);
        my_strncpy(&corrected[len], suggestion, max_len - len - 2);
        len = my_strlen(corrected);
        my_strncpy(&corrected[len], "]", max_len - len - 1);
    }
    corrections[idx].improvement_delta = 1500; /* simulated 15 point improvement */
    corrections[idx].created_at = ++clock;
    return 0;
}

/* Iterate with self-correction */
int sr_iterate(const char *output, int max_iterations, char *final, int max_len) {
    char current[MAX_OUTPUT];
    my_strncpy(current, output, MAX_OUTPUT - 1);
    int iterations = 0;
    int prev_score = 0;
    for (int i = 0; i < max_iterations && i < 10; i++) {
        reflection_assessment_t assess;
        sr_assess(current, &assess);
        int score = assessments[assessment_count - 1].overall_score;
        iterations++;
        if (score > 8500) break; /* good enough */
        if (score - prev_score < 200 && i > 0) break; /* no improvement */
        prev_score = score;
        /* Apply correction */
        char suggestions[MAX_SUGGEST];
        sr_suggest_improvements(assessments[assessment_count - 1].assessment_id, suggestions, MAX_SUGGEST);
        sr_apply_correction(current, suggestions, current, MAX_OUTPUT);
    }
    my_strncpy(final, current, max_len - 1);
    return iterations;
}

/* Record history */
int sr_record_history(const char *output_type, int original, int corrected, int iterations) {
    if (history_count >= MAX_HISTORY) return -1;
    int idx = history_count++;
    history[idx].item_id = idx + 1;
    my_strncpy(history[idx].output_type, output_type, MAX_NAME - 1);
    history[idx].original_score = original;
    history[idx].corrected_score = corrected;
    history[idx].iterations = iterations;
    history[idx].final_score = corrected;
    history[idx].timestamp = ++clock;
    return idx;
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
    print_str("Self Reflection v40.0 - Self-Reflection & Self-Correction\n");
    if (help) { print_str("Usage: self_reflection [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Self Reflection Test ===\n\n");
        sr_init();
        print_str("Initialized "); print_int(criteria_count); print_str(" criteria\n\n");
        /* Assess output */
        const char *output = "Machine learning is a subset of AI that uses algorithms to find patterns in data and make predictions.";
        reflection_assessment_t assess;
        int score = sr_assess(output, &assess);
        print_str("Assessment:\n");
        print_str("  Overall score: "); print_int(score / 100); print_str("%\n");
        print_str("  Issues: "); print_str(assess.issues); print_str("\n");
        print_str("  Suggestions: "); print_str(assess.suggestions); print_str("\n\n");
        /* Self-correction iteration */
        char final[MAX_OUTPUT];
        int iterations = sr_iterate(output, 5, final, MAX_OUTPUT);
        print_str("Self-correction iteration:\n");
        print_str("  Iterations: "); print_int(iterations); print_str("\n");
        print_str("  Final: "); print_str(final); print_str("\n\n");
        /* Record history */
        sr_record_history("explanation", score, score + 1500, iterations);
        print_str("  History entries: "); print_int(history_count); print_str("\n");
        print_str("\n=== Self Reflection Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
