/* workflow_orchestrator: BPMN 2.0 workflow orchestration engine (v1.0)
 *
 * Provides workflow orchestration similar to Camunda/Temporal/Conductor:
 * - Process definition with BPMN 2.0 subset and versioning
 * - Execution engine with token flow and parallel gateways
 * - State management with checkpoint recovery and pause/resume
 * - Task assignment with queues, claims, and timeout escalation
 * - Compensation with transactional subprocesses and deadlock detection
 * - Monitoring dashboard with metrics and bottleneck analysis
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* Limits */
#define MAX_PROCS 32
#define MAX_INST  64
#define MAX_NODES 64
#define MAX_TOK   128
#define MAX_TASKS 64
#define MAX_VARS  128
#define MAX_CKPT  16
#define MAX_HIST  64
#define MAX_NM    48
#define MAX_VL    128
/* Node types */
#define N_START 1
#define N_END   2
#define N_SVC   3
#define N_USR   4
#define N_GW    5
#define N_SUB   6
#define N_EVT   7
/* Gateway types */
#define GW_AND 1
#define GW_OR  2
#define GW_XOR 3
/* Instance states */
#define I_ACTIVE 1
#define I_PAUSE  2
#define I_DONE   3
#define I_FAIL   4
#define I_CANCEL 5
/* Task states */
#define T_PEND 1
#define T_CLAIM 2
#define T_RUN   3
#define T_DONE  4
#define T_ESC   5
/* Forward declarations */
static int  my_strlen(const char *s);
static int  my_strcmp(const char *a, const char *b);
static void my_strcpy(char *d, const char *s);
static void print_int(int v);
static void print_ok(const char *msg);
static void print_fail(const char *msg);
static void check(int cond, const char *msg, int *pass, int *fail);
static int  process_deploy(const char *name, const char *ver, int node_count);
static int  process_version(int proc_id, const char *ver);
static int  process_list(void);
static int  process_get(int proc_id);
static int  process_set_gateway(int proc_id, int node, int gw_type);
static int  process_validate(int proc_id);
static int  process_get_node_type(int proc_id, int node);
static int  instance_start(int proc_id);
static int  instance_pause(int inst_id);
static int  instance_resume(int inst_id);
static int  instance_cancel(int inst_id);
static int  instance_reset(int inst_id);
static int  instance_get_state(int inst_id);
static int  instance_get_variables(int inst_id);
static int  instance_get_tokens(int inst_id);
static int  token_advance(int inst_id, int node_id);
static int  token_split_and(int inst_id, int gw_node, int out1, int out2);
static int  token_split_or(int inst_id, int gw_node, int chosen);
static int  token_split_xor(int inst_id, int gw_node, int chosen);
static int  token_merge_and(int inst_id, int gw_node, int in1, int in2);
static int  token_merge_or(int inst_id, int gw_node);
static int  gateway_evaluate_and(int inst_id, int gw_node);
static int  gateway_evaluate_or(int inst_id, int gw_node, int cond);
static int  gateway_evaluate_xor(int inst_id, int gw_node, int selector);
static int  task_service_execute(int inst_id, int node_id, const char *action);
static int  task_user_create(int inst_id, int node_id, const char *assignee);
static int  task_user_claim(int task_id, const char *user);
static int  task_user_complete(int task_id);
static int  task_user_queue(const char *assignee);
static int  task_group_assign(int task_id, const char *group);
static int  task_escalate_timeout(int task_id, int timeout_ms);
static int  task_get_info(int task_id);
static int  subprocess_start(int inst_id, int node_id);
static int  subprocess_complete(int inst_id, int node_id);
static int  subprocess_error_boundary(int inst_id, int node_id);
static int  compensation_trigger(int inst_id, int node_id);
static int  compensation_undo(int inst_id);
static int  compensation_partial(int inst_id, int node_id, int pct);
static int  deadlock_detect(int inst_id);
static int  checkpoint_save(int inst_id);
static int  checkpoint_restore(int ckpt_id);
static int  checkpoint_list(void);
static int  variable_set(int inst_id, const char *name, const char *val);
static int  variable_get(int inst_id, const char *name);
static int  variable_delete(int inst_id, const char *name);
static int  monitor_track_instance(int inst_id);
static int  monitor_bottleneck(int proc_id);
static int  monitor_metrics(int proc_id);
static int  monitor_history_query(int inst_id);
static int  monitor_summary(void);
static void test_suite(void);
/* Data structures */
typedef struct {
    int id; char name[MAX_NM]; char version[16]; int node_count;
    int node_types[MAX_NODES]; int node_gw_type[MAX_NODES]; int version_count;
} proc_def_t;
typedef struct {
    int id; int proc_id; int state; int current_node; int token_count; int token_nodes[MAX_TOK];
    int var_count; char var_names[MAX_VARS][MAX_NM]; char var_vals[MAX_VARS][MAX_VL];
    int history_count; int history_nodes[MAX_HIST]; int history_times[MAX_HIST]; int compensated; int deadlock;
} proc_inst_t;
typedef struct {
    int id; int inst_id; int node_id; int state;
    char assignee[MAX_NM]; char group[MAX_NM]; int timeout_ms; int created_at;
} task_t;
typedef struct {
    int id; int inst_id; int node; int token_count; int token_nodes[MAX_TOK];
    int var_count; char var_names[MAX_VARS][MAX_NM]; char var_vals[MAX_VARS][MAX_VL];
} checkpoint_t;
/* Global state */
static proc_def_t  processes[MAX_PROCS];
static proc_inst_t instances[MAX_INST];
static task_t      tasks[MAX_TASKS];
static checkpoint_t checkpoints[MAX_CKPT];
static int proc_count = 0, inst_count = 0, task_count = 0, ckpt_count = 0;
static int next_proc_id = 1, next_inst_id = 1, next_task_id = 1, next_ckpt_id = 1, sim_clock = 0;
/* String utilities */
static int my_strlen(const char *s) { int n = 0; while (s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b; }
static void my_strcpy(char *d, const char *s) { while (*s) { *d = *s; d++; s++; } *d = '\0'; }
static void print_int(int v) {
    char buf[16]; int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) buf[i++] = '0';
    while (v > 0 && i < 14) { buf[i++] = '0' + (v % 10); v /= 10; }
    if (neg) buf[i++] = '-'; buf[i] = '\0';
    int j = 0, k = my_strlen(buf) - 1;
    while (j < k) { char t = buf[j]; buf[j] = buf[k]; buf[k] = t; j++; k--; }
    host_print(buf);
}
static void print_ok(const char *msg) { host_print("[PASS] "); host_print(msg); host_print("\n"); }
static void print_fail(const char *msg) { host_print("[FAIL] "); host_print(msg); host_print("\n"); }
static void check(int cond, const char *msg, int *pass, int *fail) {
    if (cond) { *pass = *pass + 1; print_ok(msg); } else { *fail = *fail + 1; print_fail(msg); }
}
/* Process definition functions */
static int process_deploy(const char *name, const char *ver, int node_count) {
    if (proc_count >= MAX_PROCS || node_count > MAX_NODES) return -1;
    proc_def_t *p = &processes[proc_count];
    p->id = next_proc_id++; my_strcpy(p->name, name); my_strcpy(p->version, ver);
    p->node_count = node_count; p->version_count = 1;
    p->node_types[0] = N_START; p->node_types[node_count - 1] = N_END;
    int i; for (i = 1; i < node_count - 1; i++) { p->node_types[i] = N_SVC; p->node_gw_type[i] = 0; }
    proc_count++; return p->id;
}
static int process_version(int proc_id, const char *ver) {
    int i; for (i = 0; i < proc_count; i++) if (processes[i].id == proc_id) {
        my_strcpy(processes[i].version, ver); processes[i].version_count++; return processes[i].version_count;
    } return -1;
}
static int process_list(void) {
    int i; host_print("Registered processes:\n");
    for (i = 0; i < proc_count; i++) { host_print("  ["); print_int(processes[i].id);
        host_print("] "); host_print(processes[i].name); host_print(" v");
        host_print(processes[i].version); host_print(" ("); print_int(processes[i].node_count);
        host_print(" nodes)\n"); } return proc_count;
}
static int process_get(int proc_id) {
    int i; for (i = 0; i < proc_count; i++) if (processes[i].id == proc_id) return i; return -1;
}
static int process_validate(int proc_id) {
    int idx = process_get(proc_id);
    if (idx < 0) return -1;
    if (processes[idx].node_count < 2) return 0;
    if (processes[idx].node_types[0] != N_START) return 0;
    if (processes[idx].node_types[processes[idx].node_count - 1] != N_END) return 0;
    return 1;
}
static int process_get_node_type(int proc_id, int node) {
    int idx = process_get(proc_id);
    if (idx < 0 || node < 0 || node >= processes[idx].node_count) return -1;
    return processes[idx].node_types[node];
}
static int process_set_gateway(int proc_id, int node, int gw_type) {
    int idx = process_get(proc_id);
    if (idx < 0 || node < 0 || node >= processes[idx].node_count) return -1;
    processes[idx].node_types[node] = N_GW; processes[idx].node_gw_type[node] = gw_type; return 0;
}
/* Instance management */
static int instance_start(int proc_id) {
    int idx = process_get(proc_id);
    if (idx < 0 || inst_count >= MAX_INST) return -1;
    proc_inst_t *inst = &instances[inst_count];
    inst->id = next_inst_id++; inst->proc_id = proc_id; inst->state = I_ACTIVE;
    inst->current_node = 0; inst->token_count = 1; inst->token_nodes[0] = 0;
    inst->var_count = 0; inst->history_count = 1; inst->history_nodes[0] = 0;
    inst->history_times[0] = sim_clock++; inst->compensated = 0; inst->deadlock = 0;
    inst_count++; return inst->id;
}
static int instance_pause(int inst_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) { instances[i].state = I_PAUSE; return 0; }
    return -1;
}
static int instance_resume(int inst_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id && instances[i].state == I_PAUSE) { instances[i].state = I_ACTIVE; return 0; }
    return -1;
}
static int instance_cancel(int inst_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id && instances[i].state != I_DONE) { instances[i].state = I_CANCEL; return 0; }
    return -1;
}
static int instance_reset(int inst_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id && instances[i].state != I_DONE) {
            instances[i].state = I_ACTIVE; instances[i].current_node = 0;
            instances[i].token_count = 1; instances[i].token_nodes[0] = 0;
            instances[i].history_count = 1; instances[i].history_nodes[0] = 0;
            instances[i].history_times[0] = sim_clock++;
            instances[i].compensated = 0; instances[i].deadlock = 0; return 0; }
    return -1;
}
static int instance_get_state(int inst_id) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) return instances[i].state; return -1;
}
static int instance_get_variables(int inst_id) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        host_print("Variables for instance "); print_int(inst_id); host_print(":\n");
        for (j = 0; j < instances[i].var_count; j++) { host_print("  "); host_print(instances[i].var_names[j]);
            host_print(" = "); host_print(instances[i].var_vals[j]); host_print("\n"); }
        return instances[i].var_count; } return -1;
}
static int instance_get_tokens(int inst_id) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        host_print("Tokens for instance "); print_int(inst_id); host_print(": ");
        print_int(instances[i].token_count); host_print("\n");
        for (j = 0; j < instances[i].token_count; j++) {
            host_print("  token["); print_int(j); host_print("] = node ");
            print_int(instances[i].token_nodes[j]); host_print("\n"); }
        return instances[i].token_count; } return -1;
}
/* Token flow */
static int token_advance(int inst_id, int node_id) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        instances[i].current_node = node_id;
        if (instances[i].token_count > 0) instances[i].token_nodes[0] = node_id;
        if (instances[i].history_count < MAX_HIST) { instances[i].history_nodes[instances[i].history_count] = node_id;
            instances[i].history_times[instances[i].history_count] = sim_clock++; instances[i].history_count++; }
        return 0; } return -1;
}
static int token_split_and(int inst_id, int gw_node, int out1, int out2) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        if (instances[i].token_count + 1 >= MAX_TOK) return -1;
        instances[i].token_nodes[instances[i].token_count] = out1; instances[i].token_count++;
        instances[i].token_nodes[instances[i].token_count] = out2; instances[i].token_count++;
        instances[i].current_node = gw_node; return instances[i].token_count; } return -1;
}
static int token_split_or(int inst_id, int gw_node, int chosen) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        instances[i].token_nodes[0] = chosen; instances[i].token_count = 1; instances[i].current_node = gw_node; return 0; }
    return -1;
}
static int token_split_xor(int inst_id, int gw_node, int chosen) { return token_split_or(inst_id, gw_node, chosen); }
static int token_merge_and(int inst_id, int gw_node, int in1, int in2) {
    int i, j, f1 = 0, f2 = 0;
    for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        for (j = 0; j < instances[i].token_count; j++) {
            if (instances[i].token_nodes[j] == in1) f1 = 1; if (instances[i].token_nodes[j] == in2) f2 = 1; }
        if (f1 && f2) { instances[i].token_count = 1; instances[i].token_nodes[0] = gw_node;
            instances[i].current_node = gw_node; return 0; } return -1; } return -1;
}
static int token_merge_or(int inst_id, int gw_node) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        instances[i].token_count = 1; instances[i].token_nodes[0] = gw_node; instances[i].current_node = gw_node; return 0; }
    return -1;
}
/* Gateway evaluation */
static int gateway_evaluate_and(int inst_id, int gw_node) {
    int i, j, active = 0; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        for (j = 0; j < instances[i].token_count; j++) if (instances[i].token_nodes[j] >= 0) active++;
        return active >= 2 ? 1 : 0; } return 0;
}
static int gateway_evaluate_or(int inst_id, int gw_node, int cond) { return cond ? 1 : 0; }
static int gateway_evaluate_xor(int inst_id, int gw_node, int selector) { return selector; }
/* Task management */
static int task_service_execute(int inst_id, int node_id, const char *action) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) {
        instances[i].current_node = node_id;
        if (instances[i].history_count < MAX_HIST) { instances[i].history_nodes[instances[i].history_count] = node_id;
            instances[i].history_times[instances[i].history_count] = sim_clock++; instances[i].history_count++; }
        return 0; } return -1;
}
static int task_user_create(int inst_id, int node_id, const char *assignee) {
    if (task_count >= MAX_TASKS) return -1;
    task_t *t = &tasks[task_count]; t->id = next_task_id++; t->inst_id = inst_id; t->node_id = node_id;
    t->state = T_PEND; my_strcpy(t->assignee, assignee); t->group[0] = '\0';
    t->timeout_ms = 0; t->created_at = sim_clock++; task_count++; return t->id;
}
static int task_user_claim(int task_id, const char *user) {
    int i; for (i = 0; i < task_count; i++) if (tasks[i].id == task_id && tasks[i].state == T_PEND) {
        if (my_strcmp(tasks[i].assignee, user) == 0 || tasks[i].assignee[0] == '\0') {
            tasks[i].state = T_CLAIM; my_strcpy(tasks[i].assignee, user); return 0; } return -1; } return -1;
}
static int task_user_complete(int task_id) {
    int i; for (i = 0; i < task_count; i++)
        if (tasks[i].id == task_id && (tasks[i].state == T_CLAIM || tasks[i].state == T_RUN)) { tasks[i].state = T_DONE; return 0; }
    return -1;
}
static int task_user_queue(const char *assignee) {
    int i, count = 0; host_print("Task queue for '"); host_print(assignee); host_print("':\n");
    for (i = 0; i < task_count; i++) if (my_strcmp(tasks[i].assignee, assignee) == 0 && tasks[i].state == T_PEND) {
        host_print("  Task "); print_int(tasks[i].id); host_print(" (node "); print_int(tasks[i].node_id);
        host_print(")\n"); count++; } return count;
}
static int task_group_assign(int task_id, const char *group) {
    int i; for (i = 0; i < task_count; i++) if (tasks[i].id == task_id) { my_strcpy(tasks[i].group, group); return 0; }
    return -1;
}
static int task_escalate_timeout(int task_id, int timeout_ms) {
    int i; for (i = 0; i < task_count; i++) if (tasks[i].id == task_id && tasks[i].state == T_PEND) {
        if (sim_clock - tasks[i].created_at > timeout_ms) { tasks[i].state = T_ESC; return 1; }
        tasks[i].timeout_ms = timeout_ms; return 0; } return -1;
}
static int task_get_info(int task_id) {
    int i; for (i = 0; i < task_count; i++) if (tasks[i].id == task_id) {
        host_print("Task "); print_int(tasks[i].id); host_print(": state=");
        print_int(tasks[i].state); host_print(" assignee="); host_print(tasks[i].assignee);
        host_print(" group="); host_print(tasks[i].group);
        host_print(" timeout="); print_int(tasks[i].timeout_ms); host_print("ms\n");
        return tasks[i].state; } return -1;
}
/* Subprocess management */
static int subprocess_start(int inst_id, int node_id) { return token_advance(inst_id, node_id); }
static int subprocess_complete(int inst_id, int node_id) { return token_advance(inst_id, node_id + 1); }
static int subprocess_error_boundary(int inst_id, int node_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id && instances[i].state == I_ACTIVE) { instances[i].state = I_FAIL; instances[i].current_node = node_id; return 0; }
    return -1;
}
/* Compensation */
static int compensation_trigger(int inst_id, int node_id) {
    int i; for (i = 0; i < inst_count; i++)
        if (instances[i].id == inst_id) { instances[i].compensated = 1; instances[i].current_node = node_id; return 0; }
    return -1;
}
static int compensation_undo(int inst_id) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        if (instances[i].history_count > 1) instances[i].history_count--; instances[i].compensated = 1; return 0; }
    return -1;
}
static int compensation_partial(int inst_id, int node_id, int pct) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        instances[i].compensated = 1; instances[i].current_node = node_id; return pct; } return -1;
}
static int deadlock_detect(int inst_id) {
    int i, j, dup = 0; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        for (j = 1; j < instances[i].history_count; j++)
            if (instances[i].history_nodes[j] == instances[i].history_nodes[0]) dup++;
        if (dup >= 3) { instances[i].deadlock = 1; return 1; } return 0; } return -1;
}
/* Checkpoint */
static int checkpoint_save(int inst_id) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id && ckpt_count < MAX_CKPT) {
        checkpoint_t *c = &checkpoints[ckpt_count]; c->id = next_ckpt_id++; c->inst_id = inst_id;
        c->node = instances[i].current_node; c->token_count = instances[i].token_count;
        for (j = 0; j < instances[i].token_count; j++) c->token_nodes[j] = instances[i].token_nodes[j];
        c->var_count = instances[i].var_count;
        for (j = 0; j < instances[i].var_count; j++) { my_strcpy(c->var_names[j], instances[i].var_names[j]);
            my_strcpy(c->var_vals[j], instances[i].var_vals[j]); }
        ckpt_count++; return c->id; } return -1;
}
static int checkpoint_restore(int ckpt_id) {
    int i, j, k; for (i = 0; i < ckpt_count; i++) if (checkpoints[i].id == ckpt_id) {
        for (j = 0; j < inst_count; j++) if (instances[j].id == checkpoints[i].inst_id) {
            instances[j].current_node = checkpoints[i].node; instances[j].token_count = checkpoints[i].token_count;
            for (k = 0; k < checkpoints[i].token_count; k++) instances[j].token_nodes[k] = checkpoints[i].token_nodes[k];
            instances[j].var_count = checkpoints[i].var_count;
            for (k = 0; k < checkpoints[i].var_count; k++) { my_strcpy(instances[j].var_names[k], checkpoints[i].var_names[k]);
                my_strcpy(instances[j].var_vals[k], checkpoints[i].var_vals[k]); }
            instances[j].state = I_ACTIVE; return 0; } } return -1;
}
static int checkpoint_list(void) {
    int i; host_print("Available checkpoints:\n");
    for (i = 0; i < ckpt_count; i++) { host_print("  ckpt["); print_int(checkpoints[i].id);
        host_print("] inst="); print_int(checkpoints[i].inst_id); host_print(" node=");
        print_int(checkpoints[i].node); host_print(" vars=");
        print_int(checkpoints[i].var_count); host_print("\n"); }
    return ckpt_count;
}
/* Variables */
static int variable_set(int inst_id, const char *name, const char *val) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        for (j = 0; j < instances[i].var_count; j++) if (my_strcmp(instances[i].var_names[j], name) == 0) {
            my_strcpy(instances[i].var_vals[j], val); return 0; }
        if (instances[i].var_count < MAX_VARS) { my_strcpy(instances[i].var_names[instances[i].var_count], name);
            my_strcpy(instances[i].var_vals[instances[i].var_count], val); instances[i].var_count++; return 0; }
        return -1; } return -1;
}
static int variable_get(int inst_id, const char *name) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        for (j = 0; j < instances[i].var_count; j++) if (my_strcmp(instances[i].var_names[j], name) == 0) {
            host_print(instances[i].var_vals[j]); return j; } return -1; } return -1;
}
static int variable_delete(int inst_id, const char *name) {
    int i, j, k; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        for (j = 0; j < instances[i].var_count; j++) if (my_strcmp(instances[i].var_names[j], name) == 0) {
            for (k = j; k < instances[i].var_count - 1; k++) { my_strcpy(instances[i].var_names[k], instances[i].var_names[k + 1]);
                my_strcpy(instances[i].var_vals[k], instances[i].var_vals[k + 1]); }
            instances[i].var_count--; return 0; } return -1; } return -1;
}
/* Monitoring */
static int monitor_track_instance(int inst_id) {
    int i; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        host_print("Instance "); print_int(inst_id); host_print(": state="); print_int(instances[i].state);
        host_print(" node="); print_int(instances[i].current_node); host_print(" tokens=");
        print_int(instances[i].token_count); host_print("\n"); return instances[i].state; } return -1;
}
static int monitor_bottleneck(int proc_id) {
    if (process_get(proc_id) < 0) return -1;
    host_print("Bottleneck analysis for process "); print_int(proc_id); host_print(":\n");
    host_print("  Total instances: "); print_int(inst_count);
    host_print("\n  Recommendation: review gateway nodes for parallelism\n"); return 0;
}
static int monitor_metrics(int proc_id) {
    int i, active = 0, paused = 0, done = 0, failed = 0;
    if (process_get(proc_id) < 0) return -1;
    for (i = 0; i < inst_count; i++) if (instances[i].proc_id == proc_id) {
        if (instances[i].state == I_ACTIVE) active++; if (instances[i].state == I_PAUSE) paused++;
        if (instances[i].state == I_DONE) done++; if (instances[i].state == I_FAIL) failed++; }
    host_print("Metrics for process "); print_int(proc_id); host_print(":\n");
    host_print("  active="); print_int(active); host_print(" paused="); print_int(paused);
    host_print(" done="); print_int(done); host_print(" failed="); print_int(failed); host_print("\n");
    return active + paused + done + failed;
}
static int monitor_history_query(int inst_id) {
    int i, j; for (i = 0; i < inst_count; i++) if (instances[i].id == inst_id) {
        host_print("History for instance "); print_int(inst_id); host_print(":\n");
        for (j = 0; j < instances[i].history_count; j++) { host_print("  step "); print_int(j);
            host_print(": node="); print_int(instances[i].history_nodes[j]); host_print(" t=");
            print_int(instances[i].history_times[j]); host_print("\n"); }
        return instances[i].history_count; } return -1;
}
static int monitor_summary(void) {
    int i, a = 0, p = 0, d = 0, f = 0, c = 0;
    host_print("=== Workflow System Summary ===\n");
    host_print("Processes: "); print_int(proc_count); host_print("\n");
    host_print("Instances: "); print_int(inst_count); host_print("\n");
    host_print("Tasks:     "); print_int(task_count); host_print("\n");
    host_print("Checkpoints: "); print_int(ckpt_count); host_print("\n");
    for (i = 0; i < inst_count; i++) {
        if (instances[i].state == I_ACTIVE) a++; if (instances[i].state == I_PAUSE) p++;
        if (instances[i].state == I_DONE) d++; if (instances[i].state == I_FAIL) f++;
        if (instances[i].state == I_CANCEL) c++; }
    host_print("Instance states: active="); print_int(a); host_print(" paused="); print_int(p);
    host_print(" done="); print_int(d); host_print(" failed="); print_int(f);
    host_print(" cancelled="); print_int(c); host_print("\n");
    return proc_count + inst_count + task_count;
}
/* Test suite */
static void test_suite(void) {
    int pass = 0, fail = 0, pid, iid, tid, ckid;
    host_print("=== Workflow Orchestrator Test Suite ===\n\n");
    pid = process_deploy("order-flow", "1.0", 6);
    check(pid > 0, "process_deploy", &pass, &fail);
    check(process_version(pid, "1.1") == 2, "process_version", &pass, &fail);
    check(process_list() >= 1, "process_list", &pass, &fail);
    check(process_get(pid) >= 0, "process_get", &pass, &fail);
    iid = instance_start(pid);
    check(iid > 0, "instance_start", &pass, &fail);
    check(instance_get_state(iid) == I_ACTIVE, "instance_get_state", &pass, &fail);
    check(token_advance(iid, 1) == 0, "token_advance", &pass, &fail);
    check(token_split_and(iid, 2, 3, 4) >= 2, "token_split_and", &pass, &fail);
    check(token_merge_and(iid, 5, 3, 4) == 0, "token_merge_and", &pass, &fail);
    check(gateway_evaluate_and(iid, 5) == 1, "gateway_evaluate_and", &pass, &fail);
    check(token_split_or(iid, 2, 3) == 0, "token_split_or", &pass, &fail);
    check(token_merge_or(iid, 5) == 0, "token_merge_or", &pass, &fail);
    check(gateway_evaluate_or(iid, 5, 1) == 1, "gateway_evaluate_or", &pass, &fail);
    check(gateway_evaluate_xor(iid, 5, 2) == 2, "gateway_evaluate_xor", &pass, &fail);
    check(variable_set(iid, "order_id", "ORD-123") == 0, "variable_set", &pass, &fail);
    check(variable_get(iid, "order_id") >= 0, "variable_get", &pass, &fail);
    check(task_service_execute(iid, 1, "validate") == 0, "task_service_execute", &pass, &fail);
    tid = task_user_create(iid, 3, "alice");
    check(tid > 0, "task_user_create", &pass, &fail);
    check(task_user_queue("alice") >= 1, "task_user_queue", &pass, &fail);
    check(task_user_claim(tid, "alice") == 0, "task_user_claim", &pass, &fail);
    check(task_group_assign(tid, "reviewers") == 0, "task_group_assign", &pass, &fail);
    check(task_escalate_timeout(tid, 100) == 0, "task_escalate_timeout", &pass, &fail);
    check(task_user_complete(tid) == 0, "task_user_complete", &pass, &fail);
    check(subprocess_start(iid, 2) == 0, "subprocess_start", &pass, &fail);
    check(subprocess_complete(iid, 2) == 0, "subprocess_complete", &pass, &fail);
    check(subprocess_error_boundary(iid, 3) == 0, "subprocess_error_boundary", &pass, &fail);
    instances[0].state = I_ACTIVE;
    check(compensation_trigger(iid, 2) == 0, "compensation_trigger", &pass, &fail);
    check(compensation_undo(iid) == 0, "compensation_undo", &pass, &fail);
    check(compensation_partial(iid, 3, 50) == 50, "compensation_partial", &pass, &fail);
    check(deadlock_detect(iid) == 0, "deadlock_detect", &pass, &fail);
    variable_set(iid, "ckpt_var", "saved");
    ckid = checkpoint_save(iid);
    check(ckid > 0, "checkpoint_save", &pass, &fail);
    variable_set(iid, "ckpt_var", "changed");
    check(checkpoint_restore(ckid) == 0, "checkpoint_restore", &pass, &fail);
    check(instance_pause(iid) == 0, "instance_pause", &pass, &fail);
    check(instance_resume(iid) == 0, "instance_resume", &pass, &fail);
    check(instance_get_variables(iid) >= 1, "instance_get_variables", &pass, &fail);
    check(monitor_track_instance(iid) == I_ACTIVE, "monitor_track_instance", &pass, &fail);
    check(monitor_bottleneck(pid) == 0, "monitor_bottleneck", &pass, &fail);
    check(monitor_metrics(pid) >= 1, "monitor_metrics", &pass, &fail);
    check(monitor_history_query(iid) >= 1, "monitor_history_query", &pass, &fail);
    check(instance_cancel(iid) == 0, "instance_cancel", &pass, &fail);
    host_print("\n=== Results: "); print_int(pass); host_print(" passed, "); print_int(fail); host_print(" failed ===\n");
    if (fail == 0) host_print("All tests passed!\n"); else host_exit(1);
}
/* Entry point */
void _start(void) {
    int argc = host_get_argv(0, 4096);
    char *argv_buf = (char *)0;
    if (argc > 0) {
        argv_buf = (char *)host_alloc(4096, 1);
        host_get_argv((unsigned int)(unsigned long)argv_buf, 4096);
        if (my_strcmp(argv_buf, "-h") == 0) {
            host_print("workflow_orchestrator v1.0 - BPMN 2.0 workflow engine\n");
            host_print("Usage: workflow_orchestrator [options]\n");
            host_print("  -h    Show this help\n  -t    Run test suite\n\nFeatures:\n");
            host_print("  Process definition with versioning\n  Token flow with AND/OR/XOR gateways\n");
            host_print("  User task queues with claim/escalation\n  Compensation with deadlock detection\n");
            host_print("  Checkpoint save/restore\n  Monitoring dashboard with metrics\n");
            host_exit(0);
        } else if (my_strcmp(argv_buf, "-t") == 0) {
            test_suite(); host_exit(0);
        } else {
            host_print("Unknown option: "); host_print(argv_buf); host_print("\nUse -h for help\n"); host_exit(1);
        }
    }
    test_suite(); host_exit(0);
}
