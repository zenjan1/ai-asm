/* reasoning_chain: Chain-of-Thought reasoning system (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_CHAINS      32
#define MAX_STEPS       64
#define MAX_TEXT        512
#define MAX_ACTION      256
#define MAX_CONCLUSION  512
#define MAX_TEMPLATES   16
#define MAX_QUESTION    256

/* Reasoning step */
typedef struct {
    int   step_id;
    char  thought[MAX_TEXT];
    char  action[MAX_ACTION];
    char  observation[MAX_TEXT];
    int   confidence;    /* * 100 */
    int   next_step_id;
    int   created_at;
} reasoning_step_t;

/* Reasoning chain */
typedef struct {
    int   chain_id;
    reasoning_step_t steps[MAX_STEPS];
    int   step_count;
    int   current_step;
    char  final_conclusion[MAX_CONCLUSION];
    int   total_confidence;  /* * 100 */
    int   created_at;
    int   completed_at;
    char  question[MAX_QUESTION];
} reasoning_chain_t;

/* Reasoning template */
typedef struct {
    int   template_id;
    char  name[MAX_TEXT];
    char  use_cases[MAX_TEXT];
    int   success_rate;  /* * 100 */
    int   usage_count;
} reasoning_template_t;

static reasoning_chain_t chains[MAX_CHAINS];
static reasoning_template_t templates[MAX_TEMPLATES];
static int chain_count = 0;
static int next_chain_id = 1;
static int template_count = 0;
static int next_template_id = 1;
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

/* Initialize templates */
int rc_init(void) {
    /* Pre-built reasoning templates */
    int t0 = template_count++;
    templates[t0].template_id = next_template_id++;
    my_strncpy(templates[t0].name, "Mathematical Proof", MAX_TEXT - 1);
    my_strncpy(templates[t0].use_cases, "math, logic, calculation", MAX_TEXT - 1);
    templates[t0].success_rate = 9200; templates[t0].usage_count = 0;
    int t1 = template_count++;
    templates[t1].template_id = next_template_id++;
    my_strncpy(templates[t1].name, "Scientific Method", MAX_TEXT - 1);
    my_strncpy(templates[t1].use_cases, "hypothesis, experiment, analysis", MAX_TEXT - 1);
    templates[t1].success_rate = 8800; templates[t1].usage_count = 0;
    int t2 = template_count++;
    templates[t2].template_id = next_template_id++;
    my_strncpy(templates[t2].name, "Problem Decomposition", MAX_TEXT - 1);
    my_strncpy(templates[t2].use_cases, "complex problems, system design", MAX_TEXT - 1);
    templates[t2].success_rate = 8500; templates[t2].usage_count = 0;
    return template_count;
}

/* Start reasoning chain */
int rc_start_chain(const char *question) {
    if (chain_count >= MAX_CHAINS) return -1;
    int idx = chain_count++;
    chains[idx].chain_id = next_chain_id++;
    chains[idx].step_count = 0;
    chains[idx].current_step = 0;
    chains[idx].total_confidence = 0;
    chains[idx].created_at = ++clock;
    chains[idx].completed_at = 0;
    chains[idx].final_conclusion[0] = '\0';
    my_strncpy(chains[idx].question, question, MAX_QUESTION - 1);
    return chains[idx].chain_id;
}

/* Add reasoning step */
int rc_add_step(int chain_id, const char *thought, const char *action) {
    for (int i = 0; i < chain_count; i++) {
        if (chains[i].chain_id == chain_id) {
            if (chains[i].step_count >= MAX_STEPS) return -1;
            int idx = chains[i].step_count++;
            chains[i].steps[idx].step_id = idx;
            my_strncpy(chains[i].steps[idx].thought, thought ? thought : "", MAX_TEXT - 1);
            my_strncpy(chains[i].steps[idx].action, action ? action : "", MAX_ACTION - 1);
            chains[i].steps[idx].observation[0] = '\0';
            chains[i].steps[idx].confidence = 7500; /* default 0.75 */
            chains[i].steps[idx].next_step_id = idx + 1;
            chains[i].steps[idx].created_at = ++clock;
            chains[i].total_confidence += chains[i].steps[idx].confidence;
            chains[i].current_step = idx;
            return idx;
        }
    }
    return -1;
}

/* Set observation for current step */
int rc_set_observation(int chain_id, int step_id, const char *observation) {
    for (int i = 0; i < chain_count; i++) {
        if (chains[i].chain_id == chain_id) {
            if (step_id >= 0 && step_id < chains[i].step_count) {
                my_strncpy(chains[i].steps[step_id].observation, observation, MAX_TEXT - 1);
                return 0;
            }
        }
    }
    return -1;
}

/* Finalize chain */
int rc_finalize_chain(int chain_id, const char *conclusion) {
    for (int i = 0; i < chain_count; i++) {
        if (chains[i].chain_id == chain_id) {
            my_strncpy(chains[i].final_conclusion, conclusion, MAX_CONCLUSION - 1);
            chains[i].completed_at = ++clock;
            return chains[i].step_count;
        }
    }
    return -1;
}

/* Validate chain */
int rc_validate_chain(int chain_id) {
    for (int i = 0; i < chain_count; i++) {
        if (chains[i].chain_id == chain_id) {
            if (chains[i].step_count == 0) return 0;
            if (chains[i].final_conclusion[0] == '\0') return 0;
            return 1;
        }
    }
    return -1;
}

/* Apply template */
int rc_apply_template(const char *question, const char *template_name) {
    for (int t = 0; t < template_count; t++) {
        if (my_strstr(templates[t].name, template_name)) {
            templates[t].usage_count++;
            int cid = rc_start_chain(question);
            if (cid < 0) return -1;
            /* Generate template-based steps */
            if (my_strstr(templates[t].use_cases, "math")) {
                rc_add_step(cid, "Understand the problem and identify given information", "parse_question");
                rc_add_step(cid, "Break down into mathematical components", "decompose");
                rc_add_step(cid, "Apply relevant mathematical formulas", "calculate");
                rc_add_step(cid, "Verify the result makes sense", "verify");
            } else if (my_strstr(templates[t].use_cases, "hypothesis")) {
                rc_add_step(cid, "Formulate hypothesis based on observations", "hypothesize");
                rc_add_step(cid, "Design experiment to test hypothesis", "design_experiment");
                rc_add_step(cid, "Collect and analyze data", "analyze");
                rc_add_step(cid, "Draw conclusions from results", "conclude");
            } else {
                rc_add_step(cid, "Identify the main problem components", "decompose");
                rc_add_step(cid, "Analyze each component separately", "analyze");
                rc_add_step(cid, "Synthesize findings into solution", "synthesize");
            }
            return cid;
        }
    }
    return -1;
}

/* List templates */
int rc_list_templates(void) {
    print_str("  Reasoning Templates\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < template_count; i++) {
        print_str("  #"); print_int(templates[i].template_id);
        print_str(" "); print_str(templates[i].name);
        print_str(" ("); print_str(templates[i].use_cases);
        print_str(") success="); print_int(templates[i].success_rate / 100);
        print_str("% uses="); print_int(templates[i].usage_count);
        print_str("\n");
    }
    return template_count;
}

/* Print chain */
int rc_print_chain(int chain_id) {
    for (int i = 0; i < chain_count; i++) {
        if (chains[i].chain_id == chain_id) {
            print_str("  Chain #"); print_int(chain_id); print_str("\n");
            print_str("  Question: "); print_str(chains[i].question); print_str("\n");
            for (int s = 0; s < chains[i].step_count; s++) {
                print_str("  Step "); print_int(s); print_str(": ");
                print_str(chains[i].steps[s].thought);
                print_str("\n    Action: "); print_str(chains[i].steps[s].action);
                if (chains[i].steps[s].observation[0]) {
                    print_str("\n    Observation: "); print_str(chains[i].steps[s].observation);
                }
                print_str("\n");
            }
            print_str("  Conclusion: "); print_str(chains[i].final_conclusion); print_str("\n");
            return chains[i].step_count;
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
    print_str("Reasoning Chain v40.0 - Chain-of-Thought Reasoning\n");
    if (help) { print_str("Usage: reasoning_chain [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Reasoning Chain Test ===\n\n");
        rc_init();
        rc_list_templates();
        print_str("\n");
        /* Apply template */
        int cid = rc_apply_template("What is the derivative of x^3 + 2x?", "Mathematical Proof");
        print_str("Template applied, chain #"); print_int(cid); print_str("\n\n");
        /* Add custom steps */
        rc_set_observation(cid, 0, "Given: f(x) = x^3 + 2x");
        rc_set_observation(cid, 1, "Components: power rule for x^3, constant multiple for 2x");
        rc_set_observation(cid, 2, "f'(x) = 3x^2 + 2");
        rc_finalize_chain(cid, "The derivative of x^3 + 2x is 3x^2 + 2");
        rc_print_chain(cid);
        print_str("\n  Valid: ");
        int valid = rc_validate_chain(cid);
        print_str(valid ? "YES" : "NO");
        print_str("\n\n=== Reasoning Chain Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
