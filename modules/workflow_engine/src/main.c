/* workflow_engine: Agent workflow execution engine (v11.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Workflow limits */
#define MAX_WORKFLOWS   16
#define MAX_EXECUTIONS  32
#define MAX_STEPS       16
#define MAX_DEPS        16
#define STEP_NAME_LEN   64
#define RESULT_LEN      512
#define SCHEMA_LEN      256

/* Workflow states */
#define WF_STATE_IDLE       0
#define WF_STATE_RUNNING    1
#define WF_STATE_PAUSED     2
#define WF_STATE_COMPLETED  3
#define WF_STATE_FAILED     4

/* Step states */
#define STEP_PENDING   0
#define STEP_RUNNING   1
#define STEP_DONE      2
#define STEP_SKIPPED   3
#define STEP_ERROR     4

/* Workflow definition */
typedef struct {
    int     workflow_id;
    char    name[STEP_NAME_LEN];
    int     step_count;
    char    step_names[MAX_STEPS][STEP_NAME_LEN];
    int     dependencies[MAX_STEPS];    /* bitmask of prerequisite steps */
    int     parallel_groups;            /* number of parallel groups */
    char    input_schema[SCHEMA_LEN];
    char    output_schema[SCHEMA_LEN];
    int     agent_ids[MAX_STEPS];       /* which agent handles each step */
} workflow_def_t;

/* Workflow execution instance */
typedef struct {
    int     execution_id;
    int     workflow_id;
    int     current_step;
    int     status;
    char    results[MAX_STEPS][RESULT_LEN];
    int     step_status[MAX_STEPS];
    int     step_errors[MAX_STEPS];
    int     start_time;
    int     end_time;
    char    input_data[SCHEMA_LEN];
} workflow_exec_t;

/* Global state */
static workflow_def_t  workflows[MAX_WORKFLOWS];
static workflow_exec_t executions[MAX_EXECUTIONS];
static int workflow_count = 0;
static int execution_count = 0;
static int next_wf_id = 1;
static int next_exec_id = 1;

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

/* ===== Workflow definition ===== */

/* Define a new workflow from a simple definition string
   Format: "name:step1,step2|step3:dep0,dep1"
   Returns: workflow_id or -1 */
int workflow_define(const char *yaml_def) {
    if (workflow_count >= MAX_WORKFLOWS) {
        print_str("[workflow] max workflows reached\n");
        return -1;
    }

    if (!yaml_def || !yaml_def[0]) {
        print_str("[workflow] empty definition\n");
        return -1;
    }

    int idx = workflow_count;
    workflows[idx].workflow_id = next_wf_id;
    workflows[idx].step_count = 0;
    workflows[idx].parallel_groups = 0;

    /* Parse workflow name (before first ':') */
    int pos = 0;
    int name_pos = 0;
    while (yaml_def[pos] && yaml_def[pos] != ':' && name_pos < STEP_NAME_LEN - 1) {
        workflows[idx].name[name_pos++] = yaml_def[pos++];
    }
    workflows[idx].name[name_pos] = '\0';

    if (!yaml_def[pos]) {
        /* No steps defined */
        print_str("[workflow] no steps in definition: ");
        print_str(workflows[idx].name);
        print_str("\n");
        workflow_count++;
        int id = next_wf_id++;
        return id;
    }

    pos++; /* skip ':' */

    /* Parse step groups (separated by '|') */
    int group = 0;
    int step_idx = 0;
    int step_start = pos;

    while (yaml_def[pos] && step_idx < MAX_STEPS) {
        if (yaml_def[pos] == ',' || yaml_def[pos] == '|' || yaml_def[pos] == ':') {
            /* Extract step name */
            int len = pos - step_start;
            if (len > 0 && len < STEP_NAME_LEN) {
                int k = 0;
                while (k < len && k < STEP_NAME_LEN - 1) {
                    workflows[idx].step_names[step_idx][k] = yaml_def[step_start + k];
                    k++;
                }
                workflows[idx].step_names[step_idx][k] = '\0';

                /* Set dependency from parallel group */
                workflows[idx].dependencies[step_idx] = (1 << group) - 1;
                step_idx++;
            }

            if (yaml_def[pos] == '|') group++;
            step_start = pos + 1;
        }
        pos++;
    }

    /* Handle last step */
    if (step_start < pos && step_idx < MAX_STEPS) {
        int len = pos - step_start;
        if (len > 0 && len < STEP_NAME_LEN) {
            int k = 0;
            while (k < len && k < STEP_NAME_LEN - 1) {
                workflows[idx].step_names[step_idx][k] = yaml_def[step_start + k];
                k++;
            }
            workflows[idx].step_names[step_idx][k] = '\0';
            workflows[idx].dependencies[step_idx] = (1 << group) - 1;
            step_idx++;
        }
    }

    workflows[idx].step_count = step_idx;
    workflows[idx].parallel_groups = group + 1;

    workflow_count++;
    int id = next_wf_id++;

    print_str("[workflow] defined: ");
    print_str(workflows[idx].name);
    print_str(" (");
    print_int(step_idx);
    print_str(" steps, ");
    print_int(group + 1);
    print_str(" groups)\n");

    return id;
}

/* Start workflow execution with input data
   workflow_id: ID of workflow to run
   input: JSON input data
   Returns: execution_id or -1 */
int workflow_start(int workflow_id, const char *input) {
    /* Find workflow */
    int wf_idx = -1;
    for (int i = 0; i < workflow_count; i++) {
        if (workflows[i].workflow_id == workflow_id) {
            wf_idx = i;
            break;
        }
    }
    if (wf_idx < 0) {
        print_str("[workflow] workflow not found: ");
        print_int(workflow_id);
        print_str("\n");
        return -1;
    }

    if (execution_count >= MAX_EXECUTIONS) {
        print_str("[workflow] max executions reached\n");
        return -1;
    }

    int idx = execution_count;
    executions[idx].execution_id = next_exec_id;
    executions[idx].workflow_id = workflow_id;
    executions[idx].current_step = 0;
    executions[idx].status = WF_STATE_RUNNING;
    executions[idx].start_time = 0;
    executions[idx].end_time = 0;

    if (input) {
        my_strncpy(executions[idx].input_data, input, SCHEMA_LEN - 1);
    } else {
        executions[idx].input_data[0] = '\0';
    }

    /* Initialize step results and statuses */
    for (int i = 0; i < workflows[wf_idx].step_count; i++) {
        executions[idx].results[i][0] = '\0';
        executions[idx].step_status[i] = STEP_PENDING;
        executions[idx].step_errors[i] = 0;
    }

    execution_count++;
    int id = next_exec_id++;

    print_str("[workflow] started execution ");
    print_int(id);
    print_str(" for '");
    print_str(workflows[wf_idx].name);
    print_str("'\n");

    /* Execute steps in order (respecting dependencies) */
    int completed = 0;
    int failed = 0;

    while (completed < workflows[wf_idx].step_count && !failed) {
        for (int s = 0; s < workflows[wf_idx].step_count; s++) {
            if (executions[idx].step_status[s] != STEP_PENDING) continue;

            /* Check dependencies */
            int deps_met = 1;
            int deps = workflows[wf_idx].dependencies[s];
            for (int d = 0; d < workflows[wf_idx].step_count && deps; d++) {
                if (deps & (1 << d)) {
                    if (executions[idx].step_status[d] != STEP_DONE) {
                        deps_met = 0;
                        break;
                    }
                    deps &= ~(1 << d);
                }
            }

            if (!deps_met) continue;

            /* Execute step */
            executions[idx].step_status[s] = STEP_RUNNING;
            executions[idx].current_step = s;

            print_str("[workflow] executing step ");
            print_int(s);
            print_str(": ");
            print_str(workflows[wf_idx].step_names[s]);
            print_str("\n");

            /* Simulate step execution (in real impl, this calls the agent) */
            int step_result = 0; /* 0 = success placeholder */

            if (step_result == 0) {
                my_strcpy(executions[idx].results[s], "ok");
                executions[idx].step_status[s] = STEP_DONE;
                completed++;
            } else {
                executions[idx].step_status[s] = STEP_ERROR;
                executions[idx].step_errors[s] = step_result;
                failed = 1;
            }
        }

        /* Safety: prevent infinite loop */
        if (!completed && !failed) {
            /* Check if remaining steps have unresolvable deps */
            int stuck = 1;
            for (int s = 0; s < workflows[wf_idx].step_count; s++) {
                if (executions[idx].step_status[s] == STEP_PENDING) {
                    stuck = 0;
                    break;
                }
            }
            if (stuck) break;
        }
    }

    if (failed) {
        executions[idx].status = WF_STATE_FAILED;
        print_str("[workflow] execution ");
        print_int(id);
        print_str(" FAILED at step ");
        print_int(executions[idx].current_step);
        print_str("\n");
    } else {
        executions[idx].status = WF_STATE_COMPLETED;
        print_str("[workflow] execution ");
        print_int(id);
        print_str(" COMPLETED\n");
    }

    executions[idx].end_time = 0;

    return id;
}

/* Pause a running execution
   execution_id: execution to pause
   Returns: 0=success, -1=failure */
int workflow_pause(int execution_id) {
    for (int i = 0; i < execution_count; i++) {
        if (executions[i].execution_id == execution_id) {
            if (executions[i].status == WF_STATE_RUNNING) {
                executions[i].status = WF_STATE_PAUSED;
                print_str("[workflow] paused execution ");
                print_int(execution_id);
                print_str("\n");
                return 0;
            }
            print_str("[workflow] cannot pause execution ");
            print_int(execution_id);
            print_str(" (state=");
            print_int(executions[i].status);
            print_str(")\n");
            return -1;
        }
    }
    print_str("[workflow] execution not found: ");
    print_int(execution_id);
    print_str("\n");
    return -1;
}

/* Resume a paused execution
   execution_id: execution to resume
   Returns: 0=success, -1=failure */
int workflow_resume(int execution_id) {
    for (int i = 0; i < execution_count; i++) {
        if (executions[i].execution_id == execution_id) {
            if (executions[i].status == WF_STATE_PAUSED) {
                executions[i].status = WF_STATE_RUNNING;
                print_str("[workflow] resumed execution ");
                print_int(execution_id);
                print_str("\n");
                return 0;
            }
            return -1;
        }
    }
    return -1;
}

/* Get execution status
   execution_id: execution to query
   Returns: status code */
int workflow_get_status(int execution_id) {
    for (int i = 0; i < execution_count; i++) {
        if (executions[i].execution_id == execution_id) {
            return executions[i].status;
        }
    }
    return -1;
}

/* Get execution results
   execution_id: execution to query
   Returns: pointer to results string (static buffer) */
int workflow_get_results(int execution_id) {
    for (int i = 0; i < execution_count; i++) {
        if (executions[i].execution_id == execution_id) {
            /* Find workflow */
            int wf_idx = -1;
            for (int j = 0; j < workflow_count; j++) {
                if (workflows[j].workflow_id == executions[i].workflow_id) {
                    wf_idx = j;
                    break;
                }
            }

            print_str("=== Workflow Results ===\n");
            print_str("Execution: ");
            print_int(executions[i].execution_id);
            print_str("  Status: ");
            print_int(executions[i].status);
            print_str("\n");

            if (wf_idx >= 0) {
                for (int s = 0; s < workflows[wf_idx].step_count; s++) {
                    print_str("  Step ");
                    print_int(s);
                    print_str(" ");
                    print_str(workflows[wf_idx].step_names[s]);
                    print_str(": [");
                    int st = executions[i].step_status[s];
                    print_str(st == STEP_DONE ? "DONE" :
                              st == STEP_RUNNING ? "RUNNING" :
                              st == STEP_ERROR ? "ERROR" :
                              st == STEP_SKIPPED ? "SKIPPED" : "PENDING");
                    print_str("] ");
                    print_str(executions[i].results[s]);
                    print_str("\n");
                }
            }

            return executions[i].status;
        }
    }

    print_str("[workflow] execution not found: ");
    print_int(execution_id);
    print_str("\n");
    return -1;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, info = 0, test = 0, status = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0 || my_strcmp(arg, "--info") == 0) info = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-s") == 0 || my_strcmp(arg, "--status") == 0) status = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("Workflow Engine v11.0 - DAG-based Agent Orchestration\n");

    if (help) {
        print_str("Usage: workflow_engine [options]\n");
        print_str("  -h, --help          Show this help\n");
        print_str("  -i, --info          Show engine info\n");
        print_str("  -t, --test          Run workflow test\n");
        print_str("  -s, --status        Show all executions\n");
        return;
    }
    if (info) {
        print_str("Workflow Engine v11.0\n");
        print_str("Max workflows: "); print_int(MAX_WORKFLOWS); print_str("\n");
        print_str("Max executions: "); print_int(MAX_EXECUTIONS); print_str("\n");
        print_str("Max steps: "); print_int(MAX_STEPS); print_str("\n");
        print_str("Result size: "); print_int(RESULT_LEN); print_str(" bytes\n");
        return;
    }
    if (test) {
        print_str("=== Workflow Engine Test ===\n");
        print_str("\n--- Test: Sequential Workflow ---\n");
        int wf1 = workflow_define("code_review:parse,analyze,report");
        print_str("Workflow ID: "); print_int(wf1); print_str("\n");
        int exec1 = workflow_start(wf1, "{\"file\":\"main.c\"}");
        workflow_get_results(exec1);
        print_str("\n--- Test: Parallel Workflow ---\n");
        int wf2 = workflow_define("security_scan:static_scan,dep_scan|merge_report");
        print_str("Workflow ID: "); print_int(wf2); print_str("\n");
        int exec2 = workflow_start(wf2, "{\"target\":\"/src\"}");
        workflow_get_results(exec2);
        print_str("\n--- Test: Pause/Resume ---\n");
        workflow_pause(exec1);
        workflow_resume(exec1);
        print_str("\n--- Test: Nonexistent Workflow ---\n");
        int bad = workflow_start(999, "");
        print_str("Result: "); print_int(bad); print_str("\n");
        print_str("\n=== All Tests Complete ===\n");
        return;
    }
    if (status) {
        print_str("=== Workflow Status ===\n");
        print_str("Workflows: "); print_int(workflow_count);
        print_str("/"); print_int(MAX_WORKFLOWS); print_str("\n");
        print_str("Executions: "); print_int(execution_count);
        print_str("/"); print_int(MAX_EXECUTIONS); print_str("\n");
        for (int i = 0; i < workflow_count; i++) {
            print_str("  WF "); print_int(workflows[i].workflow_id);
            print_str(" "); print_str(workflows[i].name);
            print_str(" ("); print_int(workflows[i].step_count);
            print_str(" steps)\n");
        }
        for (int i = 0; i < execution_count; i++) {
            print_str("  Exec "); print_int(executions[i].execution_id);
            print_str(" WF="); print_int(executions[i].workflow_id);
            print_str(" status="); print_int(executions[i].status);
            print_str("\n");
        }
        return;
    }
    print_str("Use -h for help, -t for test, -s for status\n");
}
