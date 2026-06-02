/* agent_workflow: DAG-based workflow engine (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_WORKFLOWS   32
#define MAX_NODES       64
#define MAX_EDGES       128
#define MAX_NAME        64
#define MAX_ACTION      256
#define MAX_CONDITION   256
#define MAX_VARIABLES   32
#define MAX_VAR_NAME    32
#define MAX_VAR_VALUE   256
#define MAX_HISTORY     128
#define MAX_WF_NAME     128

/* Node types */
#define NODE_START      0
#define NODE_END        1
#define NODE_TASK       2
#define NODE_CONDITION  3
#define NODE_PARALLEL   4
#define NODE_LOOP       5
#define NODE_SUBFLOW    6

/* Workflow status */
#define WF_DRAFT        0
#define WF_READY        1
#define WF_RUNNING      2
#define WF_PAUSED       3
#define WF_COMPLETED    4
#define WF_FAILED       5

/* Node status */
#define NS_PENDING      0
#define NS_RUNNING      1
#define NS_COMPLETED    2
#define NS_FAILED       3

/* Workflow node */
typedef struct {
    int   node_id;
    char  name[MAX_NAME];
    int   type;
    char  action[MAX_ACTION];
    int   next_nodes[8];
    int   next_count;
    char  condition_expr[MAX_CONDITION];
    int   timeout_ms;
    int   retry_count;
    int   max_retries;
} wf_node_t;

/* Workflow edge */
typedef struct {
    int   from_node;
    int   to_node;
    char  condition[MAX_CONDITION];
} wf_edge_t;

/* Workflow definition */
typedef struct {
    int   wf_id;
    char  name[MAX_WF_NAME];
    wf_node_t nodes[MAX_NODES];
    int   node_count;
    wf_edge_t edges[MAX_EDGES];
    int   edge_count;
    int   start_node;
    int   end_nodes[8];
    int   end_count;
    int   status;
    int   created_at;
} workflow_t;

/* Workflow execution */
typedef struct {
    int   execution_id;
    int   wf_id;
    int   current_node;
    int   completed_nodes[64];
    int   completed_count;
    int   failed_nodes[64];
    int   failed_count;
    char  var_names[MAX_VARIABLES][MAX_VAR_NAME];
    char  var_values[MAX_VARIABLES][MAX_VAR_VALUE];
    int   var_count;
    int   status;
    int   started_at;
} wf_execution_t;

/* Workflow state */
typedef struct {
    int   execution_id;
    int   node_id;
    int   node_status;
    char  input[MAX_ACTION];
    char  output[MAX_ACTION];
    char  error[MAX_CONDITION];
    int   timestamp;
    int   retry_count;
} wf_state_t;

static workflow_t workflows[MAX_WORKFLOWS];
static wf_execution_t executions[MAX_WORKFLOWS];
static int wf_count = 0;
static int next_wf_id = 1;
static int exec_count = 0;
static int next_exec_id = 1;
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

static const char *node_type_name(int t) {
    if (t == NODE_START) return "START";
    if (t == NODE_END) return "END";
    if (t == NODE_TASK) return "TASK";
    if (t == NODE_CONDITION) return "CONDITION";
    if (t == NODE_PARALLEL) return "PARALLEL";
    if (t == NODE_LOOP) return "LOOP";
    return "SUBFLOW";
}

/* Create workflow */
int wf_create(const char *name) {
    if (wf_count >= MAX_WORKFLOWS) return -1;
    int idx = wf_count++;
    workflows[idx].wf_id = next_wf_id++;
    my_strncpy(workflows[idx].name, name, MAX_WF_NAME - 1);
    workflows[idx].node_count = 0;
    workflows[idx].edge_count = 0;
    workflows[idx].start_node = -1;
    workflows[idx].end_count = 0;
    workflows[idx].status = WF_DRAFT;
    workflows[idx].created_at = ++clock;
    return workflows[idx].wf_id;
}

/* Add node */
int wf_add_node(int wf_id, const char *name, int type, const char *action) {
    for (int i = 0; i < wf_count; i++) {
        if (workflows[i].wf_id == wf_id) {
            if (workflows[i].node_count >= MAX_NODES) return -1;
            int idx = workflows[i].node_count++;
            workflows[i].nodes[idx].node_id = idx;
            my_strncpy(workflows[i].nodes[idx].name, name, MAX_NAME - 1);
            workflows[i].nodes[idx].type = type;
            my_strncpy(workflows[i].nodes[idx].action, action ? action : "", MAX_ACTION - 1);
            workflows[i].nodes[idx].next_count = 0;
            workflows[i].nodes[idx].condition_expr[0] = '\0';
            workflows[i].nodes[idx].timeout_ms = 30000;
            workflows[i].nodes[idx].retry_count = 0;
            workflows[i].nodes[idx].max_retries = 3;
            if (type == NODE_START) workflows[i].start_node = idx;
            if (type == NODE_END) {
                int ec = workflows[i].end_count;
                workflows[i].end_nodes[ec] = idx;
                workflows[i].end_count++;
            }
            return idx;
        }
    }
    return -1;
}

/* Add edge */
int wf_add_edge(int wf_id, int from_node, int to_node) {
    for (int i = 0; i < wf_count; i++) {
        if (workflows[i].wf_id == wf_id) {
            if (workflows[i].edge_count >= MAX_EDGES) return -1;
            int idx = workflows[i].edge_count++;
            workflows[i].edges[idx].from_node = from_node;
            workflows[i].edges[idx].to_node = to_node;
            workflows[i].edges[idx].condition[0] = '\0';
            /* Add to next_nodes */
            for (int n = 0; n < workflows[i].node_count; n++) {
                if (workflows[i].nodes[n].node_id == from_node) {
                    int nc = workflows[i].nodes[n].next_count;
                    if (nc < 8) workflows[i].nodes[n].next_nodes[nc] = to_node;
                    workflows[i].nodes[n].next_count++;
                    break;
                }
            }
            return idx;
        }
    }
    return -1;
}

/* Execute workflow (simulated) */
int wf_execute(int wf_id, wf_execution_t *exec) {
    for (int i = 0; i < wf_count; i++) {
        if (workflows[i].wf_id == wf_id) {
            workflows[i].status = WF_RUNNING;
            exec->execution_id = next_exec_id++;
            exec->wf_id = wf_id;
            exec->current_node = workflows[i].start_node;
            exec->completed_count = 0;
            exec->failed_count = 0;
            exec->var_count = 0;
            exec->status = WF_RUNNING;
            exec->started_at = ++clock;
            /* Simulate execution of nodes */
            for (int n = 0; n < workflows[i].node_count; n++) {
                int idx = exec->completed_count;
                exec->completed_nodes[idx] = workflows[i].nodes[n].node_id;
                exec->completed_count++;
            }
            workflows[i].status = WF_COMPLETED;
            exec->status = WF_COMPLETED;
            return exec->execution_id;
        }
    }
    return -1;
}

/* List workflows */
int wf_list(void) {
    print_str("  Workflows\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < wf_count; i++) {
        print_str("  #"); print_int(workflows[i].wf_id);
        print_str(" "); print_str(workflows[i].name);
        print_str(" - nodes="); print_int(workflows[i].node_count);
        print_str(" edges="); print_int(workflows[i].edge_count);
        const char *st = "draft";
        if (workflows[i].status == WF_READY) st = "ready";
        else if (workflows[i].status == WF_RUNNING) st = "running";
        else if (workflows[i].status == WF_COMPLETED) st = "completed";
        else if (workflows[i].status == WF_FAILED) st = "failed";
        print_str(" "); print_str(st);
        print_str("\n");
    }
    return wf_count;
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
    print_str("Agent Workflow v40.0 - DAG Workflow Engine\n");
    if (help) { print_str("Usage: agent_workflow [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Agent Workflow Test ===\n\n");
        /* Create workflow */
        int wf1 = wf_create("Data Analysis Pipeline");
        print_str("Created workflow: #"); print_int(wf1); print_str("\n\n");
        /* Add nodes */
        int n0 = wf_add_node(wf1, "Start", NODE_START, "");
        int n1 = wf_add_node(wf1, "Fetch Data", NODE_TASK, "fetch_dataset('data.csv')");
        int n2 = wf_add_node(wf1, "Validate", NODE_CONDITION, "validate_schema(data)");
        int n3 = wf_add_node(wf1, "Clean Data", NODE_TASK, "clean_data(data)");
        int n4 = wf_add_node(wf1, "Analyze", NODE_TASK, "analyze(data)");
        int n5 = wf_add_node(wf1, "Generate Report", NODE_TASK, "generate_report(results)");
        int n6 = wf_add_node(wf1, "End", NODE_END, "");
        print_str("Added "); print_int(workflows[0].node_count); print_str(" nodes\n\n");
        /* Add edges */
        wf_add_edge(wf1, n0, n1);
        wf_add_edge(wf1, n1, n2);
        wf_add_edge(wf1, n2, n3);
        wf_add_edge(wf1, n3, n4);
        wf_add_edge(wf1, n4, n5);
        wf_add_edge(wf1, n5, n6);
        print_str("Added "); print_int(workflows[0].edge_count); print_str(" edges\n\n");
        /* List */
        wf_list();
        print_str("\n  Nodes:\n");
        for (int i = 0; i < workflows[0].node_count; i++) {
            print_str("    ["); print_int(i); print_str("] ");
            print_str(node_type_name(workflows[0].nodes[i].type));
            print_str(" "); print_str(workflows[0].nodes[i].name);
            if (workflows[0].nodes[i].action[0]) {
                print_str(" -> "); print_str(workflows[0].nodes[i].action);
            }
            print_str("\n");
        }
        print_str("\n");
        /* Execute */
        wf_execution_t exec;
        int eid = wf_execute(wf1, &exec);
        print_str("Execution #"); print_int(eid); print_str("\n");
        print_str("  Status: "); print_int(exec.status == WF_COMPLETED ? 4 : exec.status); print_str("\n");
        print_str("  Nodes completed: "); print_int(exec.completed_count); print_str("\n");
        print_str("  Nodes failed: "); print_int(exec.failed_count); print_str("\n");
        print_str("\n=== Agent Workflow Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
