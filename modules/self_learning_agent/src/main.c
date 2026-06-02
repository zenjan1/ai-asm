/* self_learning_agent: Self-learning agent (v16.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Learning modes */
#define LEARN_EXPERIENCE  0
#define LEARN_FEEDBACK    1
#define LEARN_OBSERVATION 2
#define LEARN_TRANSFER    3

/* Limits */
#define MAX_EXPERIENCES  64
#define MAX_MODELS       8
#define NAME_LEN         64
#define INPUT_LEN        512
#define OUTPUT_LEN       512
#define FEEDBACK_LEN     256
#define TASK_TYPE_LEN    64

/* Experience record */
typedef struct {
    int   experience_id;
    char  task_type[TASK_TYPE_LEN];
    char  input[INPUT_LEN];
    char  output[OUTPUT_LEN];
    int   success;
    int   reward;
    char  feedback[FEEDBACK_LEN];
    int   timestamp;
} experience_t;

/* Learning model */
typedef struct {
    int   model_id;
    char  name[NAME_LEN];
    int   experiences_count;
    int   accuracy;
    int   adaptation_rate;
    int   domain_knowledge;
} learning_model_t;

static experience_t experiences[MAX_EXPERIENCES];
static learning_model_t learning_models[MAX_MODELS];
static int exp_count = 0;
static int model_count = 0;
static int next_exp_id = 1;
static int next_model_id = 1;

/* Learning stats */
static int total_success = 0;
static int total_failure = 0;
static int total_reward = 0;

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

/* Get mode name */
static const char *mode_name(int mode) {
    if (mode == LEARN_EXPERIENCE) return "experience";
    if (mode == LEARN_FEEDBACK) return "feedback";
    if (mode == LEARN_OBSERVATION) return "observation";
    if (mode == LEARN_TRANSFER) return "transfer";
    return "unknown";
}

/* Record an experience */
int self_learn_record_experience(experience_t *exp) {
    if (!exp || exp_count >= MAX_EXPERIENCES) return -1;
    int idx = exp_count;
    experiences[idx].experience_id = next_exp_id;
    my_strncpy(experiences[idx].task_type, exp->task_type, TASK_TYPE_LEN - 1);
    my_strncpy(experiences[idx].input, exp->input, INPUT_LEN - 1);
    my_strncpy(experiences[idx].output, exp->output, OUTPUT_LEN - 1);
    experiences[idx].success = exp->success;
    experiences[idx].reward = exp->reward;
    my_strncpy(experiences[idx].feedback, exp->feedback, FEEDBACK_LEN - 1);
    experiences[idx].timestamp = 0;

    if (exp->success) total_success++; else total_failure++;
    total_reward += exp->reward;
    exp_count++;
    next_exp_id++;

    print_str("[agent] experience recorded: ");
    print_str(exp->task_type);
    print_str(" success=");
    print_int(exp->success);
    print_str(" reward=");
    print_int(exp->reward);
    print_str("\n");
    return 0;
}

/* Update model with learned experiences */
int self_learn_update_model(int model_id) {
    int midx = -1;
    for (int i = 0; i < model_count; i++) {
        if (learning_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0) return -1;

    print_str("[agent] updating model: ");
    print_str(learning_models[midx].name);
    print_str("\n");

    /* Recalculate accuracy based on experiences */
    learning_models[midx].experiences_count = exp_count;
    if (total_success + total_failure > 0) {
        learning_models[midx].accuracy = (total_success * 100) / (total_success + total_failure);
    }
    learning_models[midx].domain_knowledge += 10;

    print_str("  Updated: accuracy=");
    print_int(learning_models[midx].accuracy);
    print_str("% domain_knowledge=");
    print_int(learning_models[midx].domain_knowledge);
    print_str("\n");
    return 0;
}

/* Apply learned knowledge to a task */
int self_learn_apply_knowledge(int model_id, const char *task) {
    int midx = -1;
    for (int i = 0; i < model_count; i++) {
        if (learning_models[i].model_id == model_id) { midx = i; break; }
    }
    if (midx < 0) return -1;

    print_str("[agent] applying knowledge: model=");
    print_str(learning_models[midx].name);
    print_str(" task=");
    print_str(task);
    print_str("\n");

    /* Find similar experiences */
    int match_count = 0;
    for (int i = 0; i < exp_count; i++) {
        if (my_strncmp(experiences[i].task_type, task, my_strlen(task)) == 0) {
            match_count++;
            if (experiences[i].success) {
                print_str("  Match: ");
                print_str(experiences[i].output);
                print_str(" (reward=");
                print_int(experiences[i].reward);
                print_str(")\n");
            }
        }
    }
    print_str("  Found ");
    print_int(match_count);
    print_str(" relevant experiences\n");
    return match_count;
}

/* Transfer knowledge between models */
int self_learn_transfer(int source_model, int target_model) {
    print_str("[agent] transferring knowledge: model ");
    print_int(source_model);
    print_str(" -> ");
    print_int(target_model);
    print_str("\n");

    int src_idx = -1, tgt_idx = -1;
    for (int i = 0; i < model_count; i++) {
        if (learning_models[i].model_id == source_model) src_idx = i;
        if (learning_models[i].model_id == target_model) tgt_idx = i;
    }
    if (src_idx < 0 || tgt_idx < 0) return -1;

    learning_models[tgt_idx].domain_knowledge += learning_models[src_idx].domain_knowledge / 2;
    learning_models[tgt_idx].accuracy = learning_models[src_idx].accuracy * 8 / 10;

    print_str("  Transfer complete: target domain_knowledge=");
    print_int(learning_models[tgt_idx].domain_knowledge);
    print_str("\n");
    return 0;
}

/* Get improvement statistics */
int self_learn_get_improvement_stats(void) {
    print_str("=== Learning Statistics ===\n");
    print_str("Total experiences: ");
    print_int(exp_count);
    print_str("\nSuccesses: ");
    print_int(total_success);
    print_str("\nFailures: ");
    print_int(total_failure);
    print_str("\nTotal reward: ");
    print_int(total_reward);
    print_str("\n");
    if (total_success + total_failure > 0) {
        print_str("Success rate: ");
        print_int((total_success * 100) / (total_success + total_failure));
        print_str("%\n");
    }
    print_str("\nModels:\n");
    for (int i = 0; i < model_count; i++) {
        print_str("  ");
        print_str(learning_models[i].name);
        print_str(": acc=");
        print_int(learning_models[i].accuracy);
        print_str("% exp=");
        print_int(learning_models[i].experiences_count);
        print_str(" knowledge=");
        print_int(learning_models[i].domain_knowledge);
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
    print_str("Self-Learning Agent v16.0 - Adaptive AI\n");
    if (help) {
        print_str("Usage: self_learning_agent [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run learning test\n");
        return;
    }
    if (test) {
        print_str("=== Self-Learning Agent Test ===\n");
        print_str("\n--- Create Learning Models ---\n");
        learning_models[0].model_id = next_model_id++; my_strcpy(learning_models[0].name, "task_solver");
        learning_models[0].experiences_count = 0; learning_models[0].accuracy = 50;
        learning_models[0].adaptation_rate = 5; learning_models[0].domain_knowledge = 10;
        model_count++;
        learning_models[1].model_id = next_model_id++; my_strcpy(learning_models[1].name, "pattern_recognizer");
        learning_models[1].experiences_count = 0; learning_models[1].accuracy = 40;
        learning_models[1].adaptation_rate = 8; learning_models[1].domain_knowledge = 5;
        model_count++;

        print_str("\n--- Record Experiences ---\n");
        experience_t e1; my_strcpy(e1.task_type, "image_classify"); my_strcpy(e1.input, "cat image"); my_strcpy(e1.output, "predicted: cat"); e1.success = 1; e1.reward = 10; my_strcpy(e1.feedback, "correct classification");
        self_learn_record_experience(&e1);
        experience_t e2; my_strcpy(e2.task_type, "image_classify"); my_strcpy(e2.input, "dog image"); my_strcpy(e2.output, "predicted: cat"); e2.success = 0; e2.reward = -5; my_strcpy(e2.feedback, "wrong, should be dog");
        self_learn_record_experience(&e2);
        experience_t e3; my_strcpy(e3.task_type, "text_analyze"); my_strcpy(e3.input, "positive review"); my_strcpy(e3.output, "sentiment=positive"); e3.success = 1; e3.reward = 8; my_strcpy(e3.feedback, "accurate sentiment");
        self_learn_record_experience(&e3);
        experience_t e4; my_strcpy(e4.task_type, "image_classify"); my_strcpy(e4.input, "car image"); my_strcpy(e4.output, "predicted: car"); e4.success = 1; e4.reward = 10; my_strcpy(e4.feedback, "correct");
        self_learn_record_experience(&e4);

        print_str("\n--- Update Models ---\n");
        self_learn_update_model(1);

        print_str("\n--- Apply Knowledge ---\n");
        self_learn_apply_knowledge(1, "image_classify");

        print_str("\n--- Transfer Knowledge ---\n");
        self_learn_transfer(1, 2);

        print_str("\n--- Stats ---\n");
        self_learn_get_improvement_stats();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
