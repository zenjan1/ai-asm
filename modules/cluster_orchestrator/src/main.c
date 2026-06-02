/* cluster_orchestrator: Multi-node cluster management (v33.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_NODES     32
#define MAX_TASKS     128
#define MAX_NAME      64
#define MAX_TAGS      8

/* Node states */
#define NODE_OFFLINE   0
#define NODE_ONLINE    1
#define NODE_BUSY      2
#define NODE_FAILED    3

/* Task states */
#define TASK_PENDING   0
#define TASK_RUNNING   1
#define TASK_DONE      2
#define TASK_FAILED    3

/* Node entry */
typedef struct {
    int   node_id;
    char  name[MAX_NAME];
    int   state;
    int   cpu_usage;       /* 0-100% */
    int   mem_usage_mb;
    int   task_count;
    int   health_score;    /* 0-100 */
} node_t;

/* Task entry */
typedef struct {
    int   task_id;
    char  name[MAX_NAME];
    int   state;
    int   node_id;         /* assigned node, -1=unassigned */
    int   priority;        /* 1-10, higher=more urgent */
    int   duration_ms;     /* estimated duration */
} task_entry_t;

static node_t nodes[MAX_NODES];
static int node_count = 0;
static int next_node_id = 1;

static task_entry_t tasks[MAX_TASKS];
static int task_count = 0;
static int next_task_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
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

static const char *node_state_name(int s) {
    if (s == NODE_OFFLINE) return "offline";
    if (s == NODE_ONLINE) return "online";
    if (s == NODE_BUSY) return "busy";
    return "failed";
}

static const char *task_state_name(int s) {
    if (s == TASK_PENDING) return "pending";
    if (s == TASK_RUNNING) return "running";
    if (s == TASK_DONE) return "done";
    return "failed";
}

/* Add node to cluster */
int co_add_node(const char *name) {
    if (node_count >= MAX_NODES) return -1;
    int idx = node_count;
    nodes[idx].node_id = next_node_id++;
    my_strncpy(nodes[idx].name, name, MAX_NAME - 1);
    nodes[idx].state = NODE_ONLINE;
    nodes[idx].cpu_usage = 0;
    nodes[idx].mem_usage_mb = 0;
    nodes[idx].task_count = 0;
    nodes[idx].health_score = 100;
    node_count++;
    return nodes[idx].node_id;
}

/* Remove node */
int co_remove_node(int node_id) {
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].node_id == node_id) {
            if (nodes[i].task_count > 0) {
                print_str("  Node has running tasks, migrating...\n");
                /* Reassign tasks */
                for (int j = 0; j < task_count; j++) {
                    if (tasks[j].node_id == node_id && tasks[j].state == TASK_RUNNING) {
                        int best = co_find_best_node();
                        if (best >= 0) {
                            tasks[j].node_id = best;
                            print_str("    Migrated task: ");
                            print_str(tasks[j].name);
                            print_str(" to node ");
                            print_int(best);
                            print_str("\n");
                        }
                    }
                }
            }
            nodes[i].state = NODE_OFFLINE;
            print_str("  Node removed: ");
            print_str(nodes[i].name);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Find best node for task (lowest CPU, online) */
int co_find_best_node(void) {
    int best = -1;
    int best_score = 999;
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].state == NODE_ONLINE || nodes[i].state == NODE_BUSY) {
            int score = nodes[i].cpu_usage + nodes[i].task_count * 10;
            if (score < best_score) {
                best_score = score;
                best = nodes[i].node_id;
            }
        }
    }
    return best;
}

/* Submit task */
int co_submit_task(const char *name, int priority) {
    if (task_count >= MAX_TASKS) return -1;
    int idx = task_count;
    tasks[idx].task_id = next_task_id++;
    my_strncpy(tasks[idx].name, name, MAX_NAME - 1);
    tasks[idx].state = TASK_PENDING;
    tasks[idx].node_id = -1;
    tasks[idx].priority = priority;
    tasks[idx].duration_ms = 1000;
    task_count++;
    return tasks[idx].task_id;
}

/* Schedule tasks to nodes */
int co_schedule(void) {
    print_str("  Scheduling tasks...\n");
    int scheduled = 0;
    /* Sort by priority (simple bubble) */
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == TASK_PENDING) {
            int node = co_find_best_node();
            if (node >= 0) {
                tasks[i].state = TASK_RUNNING;
                tasks[i].node_id = node;
                scheduled++;
                for (int j = 0; j < node_count; j++) {
                    if (nodes[j].node_id == node) {
                        nodes[j].task_count++;
                        nodes[j].cpu_usage += 15;
                        if (nodes[j].cpu_usage > 100) nodes[j].cpu_usage = 100;
                        nodes[j].state = NODE_BUSY;
                        break;
                    }
                }
            }
        }
    }
    print_str("  Scheduled: ");
    print_int(scheduled);
    print_str(" tasks\n");
    return scheduled;
}

/* Node health check */
int co_health_check(void) {
    print_str("  Health check:\n");
    int unhealthy = 0;
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].state != NODE_OFFLINE) {
            /* Simulate health check */
            if (nodes[i].cpu_usage > 90) {
                nodes[i].health_score -= 20;
                unhealthy++;
            }
            if (nodes[i].health_score <= 0) {
                nodes[i].state = NODE_FAILED;
                print_str("    Node ");
                print_int(nodes[i].node_id);
                print_str(" (");
                print_str(nodes[i].name);
                print_str(") FAILED\n");
                unhealthy++;
            }
        }
    }
    return unhealthy;
}

/* Cluster status */
int co_status(void) {
    print_str("  Cluster Status\n");
    print_str("  =================================================\n");
    print_str("  ID  Name        State    CPU%  Tasks  Health\n");
    print_str("  -------------------------------------------------\n");
    for (int i = 0; i < node_count; i++) {
        print_int(nodes[i].node_id);
        print_str("  ");
        print_str(nodes[i].name);
        int len = my_strlen(nodes[i].name);
        for (int p = 0; p < 12 - len; p++) print_str(" ");
        print_str(node_state_name(nodes[i].state));
        if (nodes[i].state == NODE_BUSY) print_str("  ");
        else print_str("   ");
        print_int(nodes[i].cpu_usage);
        print_str("%   ");
        print_int(nodes[i].task_count);
        print_str("    ");
        print_int(nodes[i].health_score);
        print_str("\n");
    }
    int running = 0, pending = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == TASK_RUNNING) running++;
        if (tasks[i].state == TASK_PENDING) pending++;
    }
    print_str("  Tasks: ");
    print_int(running);
    print_str(" running, ");
    print_int(pending);
    print_str(" pending\n");
    return node_count;
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
    print_str("Cluster Orchestrator v33.0 - Multi-Node Cluster Management\n");
    if (help) {
        print_str("Usage: cluster_orchestrator [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run cluster test\n");
        return;
    }
    if (test) {
        print_str("=== Cluster Orchestrator Test ===\n\n");
        /* Add nodes */
        int n1 = co_add_node("node-alpha");
        int n2 = co_add_node("node-beta");
        int n3 = co_add_node("node-gamma");
        print_str("  Nodes: "); print_int(node_count); print_str("\n\n");
        /* Status */
        co_status();
        print_str("\n");
        /* Submit tasks */
        print_str("Submitting tasks:\n");
        co_submit_task("data_pipeline", 5);
        co_submit_task("ml_training", 8);
        co_submit_task("web_server", 3);
        co_submit_task("backup_job", 2);
        co_submit_task("log_analysis", 4);
        print_str("\n");
        /* Schedule */
        co_schedule();
        print_str("\n");
        /* Status after schedule */
        co_status();
        print_str("\n");
        /* Health check */
        co_health_check();
        print_str("\n");
        /* Remove a node */
        print_str("Removing node-beta:\n");
        co_remove_node(n2);
        print_str("\n");
        co_status();
        print_str("\n=== Cluster Orchestrator Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
