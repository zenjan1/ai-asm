/* reasoning_agent: Reasoning enhancement agent (v16.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Reasoning types */
#define REASON_LOGICAL    0
#define REASON_CAUSAL     1
#define REASON_ANALOGICAL 2
#define REASON_DEDUCTIVE  3
#define REASON_INDUCTIVE  4

/* Limits */
#define MAX_CHAINS     16
#define MAX_PROBLEMS   32
#define PREMISE_LEN    256
#define REASONING_LEN  256
#define CONCLUSION_LEN 256
#define PROBLEM_LEN    512
#define ALTERNATIVES_LEN 256

/* Reasoning chain step */
typedef struct {
    int   step_id;
    char  premise[PREMISE_LEN];
    char  reasoning[REASONING_LEN];
    char  conclusion[CONCLUSION_LEN];
    int   confidence;
    int   next_step;
} reasoning_chain_t;

/* Reasoning result */
typedef struct {
    char  conclusion[CONCLUSION_LEN];
    reasoning_chain_t chain[MAX_CHAINS];
    int   chain_length;
    int   final_confidence;
    char  alternatives[ALTERNATIVES_LEN];
} reasoning_result_t;

static int total_reasoning = 0;
static int next_step_id = 1;

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

/* Get reasoning type name */
static const char *reasoning_type_name(int type) {
    if (type == REASON_LOGICAL) return "logical";
    if (type == REASON_CAUSAL) return "causal";
    if (type == REASON_ANALOGICAL) return "analogical";
    if (type == REASON_DEDUCTIVE) return "deductive";
    if (type == REASON_INDUCTIVE) return "inductive";
    return "unknown";
}

/* Analyze a problem with specific reasoning type */
int reasoning_analyze(const char *problem, int type) {
    if (!problem) return -1;
    print_str("[reasoning] analyze: type=");
    print_str(reasoning_type_name(type));
    print_str(" problem=");
    print_str(problem);
    print_str("\n");

    /* Simulated reasoning based on problem */
    if (my_strncmp(problem, "all humans", 10) == 0) {
        print_str("  Step 1: All humans are mortal (premise)\n");
        print_str("  Step 2: Socrates is human (premise)\n");
        print_str("  Step 3: Therefore, Socrates is mortal (conclusion)\n");
        print_str("  Confidence: 95%\n");
    } else if (my_strncmp(problem, "rain", 4) == 0) {
        print_str("  Step 1: Clouds are dark (observation)\n");
        print_str("  Step 2: Air pressure is dropping (observation)\n");
        print_str("  Step 3: These conditions typically cause rain (pattern)\n");
        print_str("  Conclusion: It will rain soon (inference)\n");
        print_str("  Confidence: 78%\n");
    } else if (my_strncmp(problem, "system slow", 11) == 0) {
        print_str("  Step 1: CPU usage is at 95% (fact)\n");
        print_str("  Step 2: Memory usage is at 80% (fact)\n");
        print_str("  Step 3: High resource usage causes slowdown (causal)\n");
        print_str("  Conclusion: System is slow due to resource exhaustion\n");
        print_str("  Confidence: 88%\n");
    } else {
        print_str("  Analysis in progress...\n");
        print_str("  Conclusion: problem analyzed\n");
        print_str("  Confidence: 70%\n");
    }
    total_reasoning++;
    return 0;
}

/* Build a reasoning chain */
int reasoning_build_chain(const char *premise, reasoning_chain_t *chain) {
    if (!premise || !chain) return -1;
    print_str("[reasoning] build chain from: ");
    print_str(premise);
    print_str("\n");

    int steps = 0;

    /* Build simulated chain */
    chain[0].step_id = next_step_id++;
    my_strncpy(chain[0].premise, premise, PREMISE_LEN - 1);
    my_strcpy(chain[0].reasoning, "Initial observation");
    my_strcpy(chain[0].conclusion, "Hypothesis formed");
    chain[0].confidence = 60;
    chain[0].next_step = 1;
    steps++;

    chain[1].step_id = next_step_id++;
    my_strcpy(chain[1].premise, "Hypothesis formed");
    my_strcpy(chain[1].reasoning, "Gathering evidence");
    my_strcpy(chain[1].conclusion, "Evidence supports hypothesis");
    chain[1].confidence = 75;
    chain[1].next_step = 2;
    steps++;

    chain[2].step_id = next_step_id++;
    my_strcpy(chain[2].premise, "Evidence supports hypothesis");
    my_strcpy(chain[2].reasoning, "Logical deduction");
    my_strcpy(chain[2].conclusion, "Conclusion verified");
    chain[2].confidence = 90;
    chain[2].next_step = -1;
    steps++;

    print_str("  Chain length: ");
    print_int(steps);
    print_str(" steps\n");
    for (int i = 0; i < steps; i++) {
        print_str("  Step ");
        print_int(i + 1);
        print_str(": ");
        print_str(chain[i].reasoning);
        print_str(" -> ");
        print_str(chain[i].conclusion);
        print_str(" (conf=");
        print_int(chain[i].confidence);
        print_str("%)\n");
    }
    total_reasoning++;
    return steps;
}

/* Validate a conclusion */
int reasoning_validate_conclusion(reasoning_result_t *result) {
    if (!result) return -1;
    print_str("[reasoning] validate: ");
    print_str(result->conclusion);
    print_str("\n");
    print_str("  Chain steps: ");
    print_int(result->chain_length);
    print_str("\n");
    print_str("  Final confidence: ");
    print_int(result->final_confidence);
    print_str("%\n");

    if (result->final_confidence >= 80) {
        print_str("  Validation: PASSED\n");
        return 1;
    } else if (result->final_confidence >= 50) {
        print_str("  Validation: PARTIAL - needs more evidence\n");
        return 0;
    } else {
        print_str("  Validation: FAILED - insufficient reasoning\n");
        return -1;
    }
}

/* Find alternatives */
int reasoning_find_alternatives(const char *conclusion) {
    if (!conclusion) return -1;
    print_str("[reasoning] alternatives to: ");
    print_str(conclusion);
    print_str("\n");

    /* Simulated alternatives */
    print_str("  Alt 1: The opposite could be true\n");
    print_str("  Alt 2: There may be additional factors\n");
    print_str("  Alt 3: The conclusion may be partially correct\n");
    total_reasoning++;
    return 3;
}

/* Get chain visualization */
int reasoning_get_chain_visualization(void) {
    print_str("=== Reasoning Chain Visualization ===\n");
    print_str("[Premise] --> [Step 1: Observe] --> [Step 2: Analyze] --> [Step 3: Conclude] --> [Result]\n");
    print_str("Total reasoning sessions: ");
    print_int(total_reasoning);
    print_str("\n");
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
    print_str("Reasoning Agent v16.0 - Logical Reasoning\n");
    if (help) {
        print_str("Usage: reasoning_agent [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run reasoning test\n");
        return;
    }
    if (test) {
        print_str("=== Reasoning Agent Test ===\n");
        print_str("\n--- Deductive Reasoning ---\n");
        reasoning_analyze("all humans are mortal, Socrates is human", REASON_DEDUCTIVE);

        print_str("\n--- Inductive Reasoning ---\n");
        reasoning_analyze("rain coming soon based on clouds and pressure", REASON_INDUCTIVE);

        print_str("\n--- Causal Reasoning ---\n");
        reasoning_analyze("system slow due to high CPU and memory", REASON_CAUSAL);

        print_str("\n--- Build Reasoning Chain ---\n");
        reasoning_chain_t chain[MAX_CHAINS];
        int steps = reasoning_build_chain("Temperature is rising rapidly", chain);

        print_str("\n--- Validate Conclusion ---\n");
        reasoning_result_t result;
        my_strcpy(result.conclusion, "System will overheat without intervention");
        result.chain_length = steps;
        result.final_confidence = 85;
        reasoning_validate_conclusion(&result);

        print_str("\n--- Find Alternatives ---\n");
        reasoning_find_alternatives("System will overheat");

        print_str("\n--- Chain Visualization ---\n");
        reasoning_get_chain_visualization();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
