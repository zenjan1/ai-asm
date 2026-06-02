/* agi_goals: Autonomous goal setting system (v20.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Goal status */
#define GOAL_ACTIVE     0
#define GOAL_COMPLETED  1
#define GOAL_BLOCKED    2
#define GOAL_CANCELLED  3

/* Goal priority */
#define GOAL_CRITICAL   0
#define GOAL_HIGH       1
#define GOAL_MEDIUM     2
#define GOAL_LOW        3

/* Limits */
#define MAX_GOALS   32
#define MAX_CHILDREN 8
#define NAME_LEN    128
#define DESC_LEN    256

/* Goal node */
typedef struct {
    int   goal_id;
    int   parent_id;
    int   children[MAX_CHILDREN];
    int   child_count;
    char  name[NAME_LEN];
    char  description[DESC_LEN];
    int   priority;
    int   status;
    int   progress;
    int   depth;
} goal_node_t;

static goal_node_t goals[MAX_GOALS];
static int goal_count = 0;
static int next_goal_id = 1;

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

/* Get priority name */
static const char *priority_name(int p) {
    if (p == GOAL_CRITICAL) return "CRITICAL";
    if (p == GOAL_HIGH) return "HIGH";
    if (p == GOAL_MEDIUM) return "MEDIUM";
    if (p == GOAL_LOW) return "LOW";
    return "unknown";
}

/* Get status name */
static const char *status_name(int s) {
    if (s == GOAL_ACTIVE) return "ACTIVE";
    if (s == GOAL_COMPLETED) return "COMPLETED";
    if (s == GOAL_BLOCKED) return "BLOCKED";
    if (s == GOAL_CANCELLED) return "CANCELLED";
    return "unknown";
}

/* Set autonomous goal */
int agi_set_goal(const char *name, const char *desc, int priority) {
    if (!name || goal_count >= MAX_GOALS) return -1;
    int idx = goal_count;
    goals[idx].goal_id = next_goal_id++;
    goals[idx].parent_id = 0;
    goals[idx].child_count = 0;
    my_strncpy(goals[idx].name, name, NAME_LEN - 1);
    my_strncpy(goals[idx].description, desc, DESC_LEN - 1);
    goals[idx].priority = priority;
    goals[idx].status = GOAL_ACTIVE;
    goals[idx].progress = 0;
    goals[idx].depth = 0;
    goal_count++;
    return goals[idx].goal_id;
}

/* Decompose into sub-goals */
int agi_decompose(int goal_id, const char *subgoal_name, const char *subgoal_desc, int priority) {
    for (int i = 0; i < goal_count; i++) {
        if (goals[i].goal_id == goal_id) {
            if (goals[i].child_count >= MAX_CHILDREN) return -1;
            int child_idx = goal_count;
            goals[child_idx].goal_id = next_goal_id++;
            goals[child_idx].parent_id = goal_id;
            goals[child_idx].child_count = 0;
            my_strncpy(goals[child_idx].name, subgoal_name, NAME_LEN - 1);
            my_strncpy(goals[child_idx].description, subgoal_desc, DESC_LEN - 1);
            goals[child_idx].priority = priority;
            goals[child_idx].status = GOAL_ACTIVE;
            goals[child_idx].progress = 0;
            goals[child_idx].depth = goals[i].depth + 1;
            goals[i].children[goals[i].child_count++] = goals[child_idx].goal_id;
            goal_count++;
            return goals[child_idx].goal_id;
        }
    }
    return -1;
}

/* Detect goal conflicts */
int agi_detect_conflicts(void) {
    print_str("  Checking for conflicts...\n");
    int conflicts = 0;
    for (int i = 0; i < goal_count; i++) {
        for (int j = i + 1; j < goal_count; j++) {
            if (goals[i].status == GOAL_ACTIVE && goals[j].status == GOAL_ACTIVE) {
                if (goals[i].parent_id == goals[j].parent_id && goals[i].parent_id != 0) {
                    print_str("  [CONFLICT] ");
                    print_str(goals[i].name);
                    print_str(" vs ");
                    print_str(goals[j].name);
                    print_str("\n");
                    conflicts++;
                }
            }
        }
    }
    if (conflicts == 0) print_str("  No conflicts detected\n");
    return conflicts;
}

/* Adjust goals dynamically */
int agi_adjust_goals(void) {
    print_str("  Adjusting goal priorities...\n");
    int adjusted = 0;
    for (int i = 0; i < goal_count; i++) {
        if (goals[i].status == GOAL_BLOCKED && goals[i].child_count > 0) {
            print_str("  Re-prioritizing children of: ");
            print_str(goals[i].name);
            print_str("\n");
            adjusted++;
        }
    }
    print_str("  Adjusted: ");
    print_int(adjusted);
    print_str(" goal groups\n");
    return adjusted;
}

/* Visualize goal tree */
int agi_visualize_tree(void) {
    print_str("=== Goal Tree ===\n");
    for (int i = 0; i < goal_count; i++) {
        for (int d = 0; d < goals[i].depth; d++) print_str("    ");
        if (goals[i].depth > 0) print_str("  ");
        print_str("[");
        print_str(priority_name(goals[i].priority));
        print_str("] ");
        print_str(goals[i].name);
        print_str(" (");
        print_str(status_name(goals[i].status));
        print_str(", ");
        print_int(goals[i].progress);
        print_str("%)\n");
    }
    return goal_count;
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
    print_str("AGI Goals v20.0 - Autonomous Goal Setting\n");
    if (help) {
        print_str("Usage: agi_goals [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run goal test\n");
        return;
    }
    if (test) {
        print_str("=== Autonomous Goal Setting Test ===\n\n");
        int main_goal = agi_set_goal("optimize_system", "Improve overall system performance", GOAL_HIGH);
        agi_decompose(main_goal, "reduce_memory_frag", "Defragment memory", GOAL_HIGH);
        agi_decompose(main_goal, "improve_latency", "Reduce network latency", GOAL_HIGH);
        agi_decompose(main_goal, "increase_throughput", "Boost WASM call throughput", GOAL_MEDIUM);
        print_str("\n");
        agi_detect_conflicts();
        print_str("\n");
        agi_adjust_goals();
        print_str("\n");
        agi_visualize_tree();
        print_str("\n=== Goals Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
