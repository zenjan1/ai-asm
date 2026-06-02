/* ethical_engine: Ethical decision engine (v25.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Ethical principles */
#define PRINCIPLE_BENEFICENCE    0
#define PRINCIPLE_NON_MALEFICENCE 1
#define PRINCIPLE_AUTONOMY      2
#define PRINCIPLE_JUSTICE       3
#define PRINCIPLE_EXPLAINABILITY 4

/* Risk levels */
#define RISK_LOW      0
#define RISK_MEDIUM   1
#define RISK_HIGH     2
#define RISK_CRITICAL 3

/* Decision outcome */
#define DECISION_APPROVED    0
#define DECISION_REJECTED    1
#define DECISION_CONDITIONAL 2
#define DECISION_REVIEW      3

/* Limits */
#define MAX_PRINCIPLES 16
#define MAX_RULES      64
#define NAME_LEN       64
#define DESC_LEN       256
#define RULE_LEN       256

/* Ethical principle */
typedef struct {
    int   principle_id;
    int   type;
    char  name[NAME_LEN];
    int   weight;
    int   satisfied;
    char  assessment[DESC_LEN];
} ethical_principle_t;

/* Decision rule */
typedef struct {
    int   rule_id;
    char  condition[RULE_LEN];
    int   risk_level;
    int   action;
} decision_rule_t;

static ethical_principle_t principles[MAX_PRINCIPLES];
static decision_rule_t rules[MAX_RULES];
static int principle_count = 0;
static int rule_count = 0;
static int decisions_made = 0;
static int decisions_rejected = 0;

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

/* Get principle name */
static const char *principle_name(int type) {
    if (type == PRINCIPLE_BENEFICENCE) return "beneficence";
    if (type == PRINCIPLE_NON_MALEFICENCE) return "non-maleficence";
    if (type == PRINCIPLE_AUTONOMY) return "autonomy";
    if (type == PRINCIPLE_JUSTICE) return "justice";
    if (type == PRINCIPLE_EXPLAINABILITY) return "explainability";
    return "unknown";
}

/* Get risk name */
static const char *risk_name(int risk) {
    if (risk == RISK_LOW) return "LOW";
    if (risk == RISK_MEDIUM) return "MEDIUM";
    if (risk == RISK_HIGH) return "HIGH";
    if (risk == RISK_CRITICAL) return "CRITICAL";
    return "unknown";
}

/* Register ethical principle */
int ee_register_principle(int type, int weight) {
    if (principle_count >= MAX_PRINCIPLES) return -1;
    int idx = principle_count;
    principles[idx].principle_id = principle_count + 1;
    principles[idx].type = type;
    my_strcpy(principles[idx].name, principle_name(type));
    principles[idx].weight = weight;
    principles[idx].satisfied = 1;
    my_strcpy(principles[idx].assessment, "compliant");
    principle_count++;
    return principles[idx].principle_id;
}

/* Evaluate a decision ethically */
int ee_evaluate_decision(const char *decision) {
    if (!decision) return -1;
    print_str("=== Ethical Evaluation ===\n");
    print_str("  Decision: ");
    print_str(decision);
    print_str("\n\n");

    decisions_made++;
    int total_score = 0;

    for (int i = 0; i < principle_count; i++) {
        int score = principles[i].weight * principles[i].satisfied;
        total_score += score;
        print_str("  [");
        print_str(principles[i].satisfied ? "PASS" : "FAIL");
        print_str("] ");
        print_str(principles[i].name);
        print_str(" (weight: ");
        print_int(principles[i].weight);
        print_str(")\n");
    }

    int outcome;
    if (total_score >= 80) {
        outcome = DECISION_APPROVED;
        print_str("\n  Outcome: APPROVED\n");
    } else if (total_score >= 50) {
        outcome = DECISION_CONDITIONAL;
        print_str("\n  Outcome: CONDITIONAL APPROVAL\n");
    } else {
        outcome = DECISION_REJECTED;
        decisions_rejected++;
        print_str("\n  Outcome: REJECTED\n");
    }

    print_str("  Ethical score: ");
    print_int(total_score);
    print_str("/100\n");
    return outcome;
}

/* Analyze risk vs benefit */
int ee_risk_benefit(const char *action, int risk_level) {
    print_str("=== Risk-Benefit Analysis ===\n");
    print_str("  Action: ");
    if (action) print_str(action);
    print_str("\n");
    print_str("  Risk level: ");
    print_str(risk_name(risk_level));
    print_str("\n");

    if (risk_level >= RISK_HIGH) {
        print_str("  Recommendation: REJECT\n");
        print_str("  Reason: Risk exceeds acceptable threshold\n");
        return DECISION_REJECTED;
    } else if (risk_level == RISK_MEDIUM) {
        print_str("  Recommendation: CONDITIONAL\n");
        print_str("  Mitigation required\n");
        return DECISION_CONDITIONAL;
    }
    print_str("  Recommendation: APPROVE\n");
    return DECISION_APPROVED;
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
    print_str("Ethical Engine v25.0 - Ethical Decision Making\n");
    if (help) {
        print_str("Usage: ethical_engine [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run ethical evaluation\n");
        return;
    }
    if (test) {
        print_str("=== Ethical Engine Test ===\n\n");
        ee_register_principle(PRINCIPLE_BENEFICENCE, 25);
        ee_register_principle(PRINCIPLE_NON_MALEFICENCE, 25);
        ee_register_principle(PRINCIPLE_AUTONOMY, 20);
        ee_register_principle(PRINCIPLE_JUSTICE, 15);
        ee_register_principle(PRINCIPLE_EXPLAINABILITY, 15);
        print_str("Registered 5 principles\n\n");
        ee_evaluate_decision("Deploy autonomous agent for system monitoring");
        print_str("\n");
        ee_risk_benefit("Delete all user data without backup", RISK_HIGH);
        print_str("\n=== Ethical Engine Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
