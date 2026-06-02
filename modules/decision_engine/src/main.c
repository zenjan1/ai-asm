/* decision_engine: Rule-based intelligent decision engine (v13.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Decision limits */
#define MAX_RULES       32
#define MAX_ACTIONS     64
#define RULE_NAME_LEN   64
#define CONDITION_LEN   256
#define ACTION_LEN      128
#define CONTEXT_LEN     256

/* Rule states */
#define RULE_ENABLED    1
#define RULE_DISABLED   0

/* Decision result */
#define DECISION_ALLOW  0
#define DECISION_DENY   1
#define DECISION_DEFER  2

/* Decision rule */
typedef struct {
    int   rule_id;
    char  name[RULE_NAME_LEN];
    char  condition[CONDITION_LEN];
    char  action[ACTION_LEN];
    int   priority;
    int   enabled;
    int   trigger_count;
} decision_rule_t;

/* Decision context */
typedef struct {
    int   sensor_data[32];
    int   sensor_count;
    int   timestamp;
    char  env_context[CONTEXT_LEN];
} decision_context_t;

/* Action log */
typedef struct {
    int   rule_id;
    char  action[ACTION_LEN];
    int   timestamp;
    int   result;
} action_log_t;

static decision_rule_t rules[MAX_RULES];
static action_log_t action_log[MAX_ACTIONS];
static int rule_count = 0;
static int log_count = 0;
static int next_rule_id = 1;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

/* Parse integer from string */
static int parse_int(const char *s) {
    int val = 0;
    int neg = 0;
    if (s[0] == '-') { neg = 1; s++; }
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s - '0');
        s++;
    }
    return neg ? -val : val;
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* Evaluate a simple condition against context
   condition: "sensor_0>50" or "temp>30" or "humidity<20"
   ctx: decision context
   Returns: 1=match, 0=no match */
static int eval_condition(const char *condition, decision_context_t *ctx) {
    if (!condition || !condition[0]) return 0;

    /* Parse condition: field op value */
    /* Supported ops: > < >= <= == != */
    int op_pos = -1;
    char op = 0;
    int cond_len = my_strlen(condition);

    for (int i = 0; i < cond_len; i++) {
        if (condition[i] == '>' || condition[i] == '<' || condition[i] == '=') {
            if (condition[i] == '=' && i > 0 && condition[i-1] == '!') {
                /* != operator */
                continue;
            }
            op_pos = i;
            op = condition[i];
            break;
        }
    }

    if (op_pos < 0) return 0;

    /* Extract field name */
    char field[64];
    int fl = 0;
    for (int i = 0; i < op_pos && fl < 63; i++) field[fl++] = condition[i];
    field[fl] = '\0';

    /* Extract value */
    int cmp_val = parse_int(condition + op_pos + (op == '=' && condition[op_pos+1] == '=' ? 2 : 1));

    /* Get field value from context */
    int field_val = 0;
    if (my_strncmp(field, "sensor_", 7) == 0) {
        int idx = parse_int(field + 7);
        if (idx >= 0 && idx < ctx->sensor_count) {
            field_val = ctx->sensor_data[idx];
        }
    } else if (my_strcmp(field, "temp") == 0 || my_strcmp(field, "temperature") == 0) {
        field_val = ctx->sensor_data[0];
    } else if (my_strcmp(field, "humidity") == 0) {
        field_val = ctx->sensor_data[1];
    } else if (my_strcmp(field, "pressure") == 0) {
        field_val = ctx->sensor_data[2];
    } else if (my_strcmp(field, "light") == 0) {
        field_val = ctx->sensor_data[3];
    }

    /* Evaluate operator */
    if (op == '>' && condition[op_pos + 1] == '=') return field_val >= cmp_val;
    if (op == '<' && condition[op_pos + 1] == '=') return field_val <= cmp_val;
    if (op == '>' && condition[op_pos + 1] != '=') return field_val > cmp_val;
    if (op == '<' && condition[op_pos + 1] != '=') return field_val < cmp_val;
    if (op == '=' && condition[op_pos + 1] == '=') return field_val == cmp_val;
    if (op == '!' && condition[op_pos + 1] == '=') return field_val != cmp_val;

    return 0;
}

/* Define a new rule
   rule: rule configuration
   Returns: rule_id or -1 */
int decision_define_rule(decision_rule_t *rule) {
    if (!rule || rule_count >= MAX_RULES) {
        print_str("[decision] cannot define rule\n");
        return -1;
    }

    int idx = rule_count;
    rules[idx].rule_id = next_rule_id;
    my_strncpy(rules[idx].name, rule->name, RULE_NAME_LEN - 1);
    my_strncpy(rules[idx].condition, rule->condition, CONDITION_LEN - 1);
    my_strncpy(rules[idx].action, rule->action, ACTION_LEN - 1);
    rules[idx].priority = rule->priority;
    rules[idx].enabled = RULE_ENABLED;
    rules[idx].trigger_count = 0;

    rule_count++;
    int id = next_rule_id++;

    print_str("[decision] rule: ");
    print_str(rule->name);
    print_str(" (if ");
    print_str(rule->condition);
    print_str(" then ");
    print_str(rule->action);
    print_str(", pri=");
    print_int(rule->priority);
    print_str(")\n");

    return id;
}

/* Evaluate rules against context
   ctx: decision context, action: output action string
   Returns: DECISION_ALLOW, DENY, or DEFER */
int decision_eval(decision_context_t *ctx, char *action) {
    if (!ctx || !action) return DECISION_DEFER;

    action[0] = '\0';
    int best_priority = -1;
    int best_idx = -1;

    /* Find highest priority matching rule */
    for (int i = 0; i < rule_count; i++) {
        if (!rules[i].enabled) continue;

        if (eval_condition(rules[i].condition, ctx)) {
            if (rules[i].priority > best_priority) {
                best_priority = rules[i].priority;
                best_idx = i;
            }
        }
    }

    if (best_idx >= 0) {
        my_strcpy(action, rules[best_idx].action);
        rules[best_idx].trigger_count++;

        /* Log action */
        if (log_count < MAX_ACTIONS) {
            int lidx = log_count;
            action_log[lidx].rule_id = rules[best_idx].rule_id;
            my_strcpy(action_log[lidx].action, rules[best_idx].action);
            action_log[lidx].timestamp = 0;
            action_log[lidx].result = 0;
            log_count++;
        }

        print_str("[decision] triggered: ");
        print_str(rules[best_idx].name);
        print_str(" -> ");
        print_str(rules[best_idx].action);
        print_str("\n");

        return DECISION_ALLOW;
    }

    return DECISION_DEFER;
}

/* Execute an action
   action: action string to execute
   Returns: 0=success, -1=failure */
int decision_execute(const char *action) {
    if (!action || !action[0]) return -1;

    print_str("[decision] executing: ");
    print_str(action);
    print_str("\n");

    /* Parse action: "set_actuator_0=1" or "alert=overtemp" or "send_mqtt=..." */
    if (my_strncmp(action, "set_actuator_", 13) == 0) {
        print_str("[decision] actuator control: ");
        print_str(action + 13);
        print_str("\n");
    } else if (my_strncmp(action, "alert=", 6) == 0) {
        print_str("[decision] ALERT: ");
        print_str(action + 6);
        print_str("\n");
    } else if (my_strncmp(action, "send_mqtt=", 10) == 0) {
        print_str("[decision] MQTT publish: ");
        print_str(action + 10);
        print_str("\n");
    } else if (my_strncmp(action, "log_event=", 10) == 0) {
        print_str("[decision] logged: ");
        print_str(action + 10);
        print_str("\n");
    }

    return 0;
}

/* Update an existing rule
   rule_id: rule to update, rule: new rule config
   Returns: 0=success, -1=failure */
int decision_update_rule(int rule_id, decision_rule_t *rule) {
    for (int i = 0; i < rule_count; i++) {
        if (rules[i].rule_id == rule_id) {
            my_strncpy(rules[i].name, rule->name, RULE_NAME_LEN - 1);
            my_strncpy(rules[i].condition, rule->condition, CONDITION_LEN - 1);
            my_strncpy(rules[i].action, rule->action, ACTION_LEN - 1);
            rules[i].priority = rule->priority;
            rules[i].enabled = rule->enabled;
            print_str("[decision] updated rule: ");
            print_str(rule->name);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Get active rules
   Returns: number of active rules */
int decision_get_active_rules(void) {
    int active = 0;
    print_str("=== Active Rules ===\n");
    for (int i = 0; i < rule_count; i++) {
        if (!rules[i].enabled) continue;
        active++;
        print_str("  ");
        print_int(i);
        print_str("\t");
        print_str(rules[i].name);
        print_str("\tif ");
        print_str(rules[i].condition);
        print_str(" then ");
        print_str(rules[i].action);
        print_str("\t[pri=");
        print_int(rules[i].priority);
        print_str(", triggers=");
        print_int(rules[i].trigger_count);
        print_str("]\n");
    }
    print_str("Total active: ");
    print_int(active);
    print_str("/");
    print_int(rule_count);
    print_str("\n");
    return active;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, rules = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-r") == 0 || my_strcmp(arg, "--rules") == 0) rules = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Decision Engine v13.0 - Rule-Based Intelligent Decisions\n");

    if (help) {
        print_str("Usage: decision_engine [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run decision test\n");
        print_str("  -r, --rules   Show active rules\n");
        return;
    }

    if (test) {
        print_str("=== Decision Engine Test ===\n");

        /* Define rules */
        print_str("\n--- Define Rules ---\n");
        decision_rule_t r1;
        my_strcpy(r1.name, "overtemp_alert");
        my_strcpy(r1.condition, "temp>40");
        my_strcpy(r1.action, "alert=temperature_critical");
        r1.priority = 10;
        int id1 = decision_define_rule(&r1);

        decision_rule_t r2;
        my_strcpy(r2.name, "low_humidity");
        my_strcpy(r2.condition, "humidity<20");
        my_strcpy(r2.action, "set_actuator_0=1");
        r2.priority = 5;
        int id2 = decision_define_rule(&r2);

        decision_rule_t r3;
        my_strcpy(r3.name, "dark_lights_on");
        my_strcpy(r3.condition, "light<100");
        my_strcpy(r3.action, "set_actuator_1=1");
        r3.priority = 3;
        int id3 = decision_define_rule(&r3);

        decision_rule_t r4;
        my_strcpy(r4.name, "high_pressure_warn");
        my_strcpy(r4.condition, "pressure>1050");
        my_strcpy(r4.action, "alert=high_pressure");
        r4.priority = 7;
        int id4 = decision_define_rule(&r4);

        /* Test scenarios */
        print_str("\n--- Scenario 1: Normal ---\n");
        decision_context_t ctx1;
        ctx1.sensor_data[0] = 22;  /* temp */
        ctx1.sensor_data[1] = 65;  /* humidity */
        ctx1.sensor_data[2] = 1013;/* pressure */
        ctx1.sensor_data[3] = 500; /* light */
        ctx1.sensor_count = 4;
        char act1[128];
        decision_eval(&ctx1, act1);

        print_str("\n--- Scenario 2: Overtemp ---\n");
        decision_context_t ctx2;
        ctx2.sensor_data[0] = 45;
        ctx2.sensor_data[1] = 50;
        ctx2.sensor_data[2] = 1010;
        ctx2.sensor_data[3] = 300;
        ctx2.sensor_count = 4;
        char act2[128];
        decision_eval(&ctx2, act2);
        decision_execute(act2);

        print_str("\n--- Scenario 3: Low humidity ---\n");
        decision_context_t ctx3;
        ctx3.sensor_data[0] = 25;
        ctx3.sensor_data[1] = 15;
        ctx3.sensor_data[2] = 1020;
        ctx3.sensor_data[3] = 400;
        ctx3.sensor_count = 4;
        char act3[128];
        decision_eval(&ctx3, act3);
        decision_execute(act3);

        print_str("\n--- Scenario 4: Multiple triggers (priority) ---\n");
        decision_context_t ctx4;
        ctx4.sensor_data[0] = 50;  /* overtemp AND low light */
        ctx4.sensor_data[1] = 10;
        ctx4.sensor_data[2] = 1060;
        ctx4.sensor_data[3] = 50;
        ctx4.sensor_count = 4;
        char act4[128];
        decision_eval(&ctx4, act4);
        decision_execute(act4);

        /* Show rules */
        print_str("\n--- Active Rules ---\n");
        decision_get_active_rules();

        /* Update a rule */
        print_str("\n--- Update Rule ---\n");
        decision_rule_t r5;
        my_strcpy(r5.name, "overtemp_alert");
        my_strcpy(r5.condition, "temp>35");
        my_strcpy(r5.action, "alert=temperature_warning");
        r5.priority = 10;
        r5.enabled = RULE_ENABLED;
        decision_update_rule(id1, &r5);

        print_str("\n=== Test Complete ===\n");
        return;
    }

    if (rules) {
        decision_get_active_rules();
        return;
    }

    print_str("Use -h for help, -t for test, -r for rules\n");
}
