/* agent_coordinator: Agent system coordinator (v9.2) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("spawn")))
extern int host_spawn(unsigned int name_ptr, unsigned int name_len);

__attribute__((import_module("host"), import_name("spawn_redirect")))
extern int host_spawn_redirect(unsigned int name_ptr, unsigned int name_len,
                                unsigned int stdin_fd, unsigned int stdout_fd);

#define MAX_AGENTS 16
#define MAX_TASKS 64
#define MAX_NAME_LEN 64
#define MAX_INPUT 512
#define MAX_OUTPUT 512

/* Agent states */
#define STATE_IDLE      0
#define STATE_RUNNING   1
#define STATE_DONE      2
#define STATE_FAILED    3

/* Agent types */
#define AGENT_PLANNER   0
#define AGENT_EXECUTOR  1
#define AGENT_REVIEWER  2
#define AGENT_SECURITY  3
#define AGENT_OPTIMIZER  4

typedef struct {
    int agent_id;
    int agent_type;
    char name[MAX_NAME_LEN];
    int state;
} agent_entry_t;

typedef struct {
    int id;
    int agent_id;
    int state;
    int priority;
    char input[MAX_INPUT];
    char output[MAX_OUTPUT];
} task_entry_t;

static unsigned int heap_pos = 65536;
static agent_entry_t agent_list[MAX_AGENTS];
static task_entry_t task_queue[MAX_TASKS];
static int agent_count = 0;
static int task_count = 0;
static int next_task_id = 0;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 1; a++; b++; }
    return (*a != *b);
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
}

static void copy_string(char *dst, const char *src, unsigned int max_len)
{
    unsigned int i = 0;
    while (src[i] && i < max_len - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16]; int i = 15; int neg = 0; buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static const char *agent_type_name(int t)
{
    switch (t) {
        case AGENT_PLANNER: return "Planner";
        case AGENT_EXECUTOR: return "Executor";
        case AGENT_REVIEWER: return "Reviewer";
        case AGENT_SECURITY: return "Security";
        case AGENT_OPTIMIZER: return "Optimizer";
        default: return "Unknown";
    }
}

static const char *state_name(int s)
{
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_RUNNING: return "RUNNING";
        case STATE_DONE: return "DONE";
        case STATE_FAILED: return "FAILED";
        default: return "UNKNOWN";
    }
}

static int register_agent(const char *name, int agent_type)
{
    if (agent_count >= MAX_AGENTS) return -1;
    agent_entry_t *a = &agent_list[agent_count];
    a->agent_id = agent_count;
    a->agent_type = agent_type;
    a->state = STATE_IDLE;
    copy_string(a->name, name, MAX_NAME_LEN);
    agent_count++;
    print_str("coordinator: registered agent ");
    print_str(name);
    print_str(" type=");
    print_str(agent_type_name(agent_type));
    print_str("\n");
    return a->agent_id;
}

static int submit_task(const char *input, int agent_id, int priority)
{
    if (task_count >= MAX_TASKS) return -1;
    task_entry_t *t = &task_queue[task_count];
    t->id = next_task_id++;
    t->agent_id = agent_id;
    t->state = STATE_IDLE;
    t->priority = priority;
    copy_string(t->input, input, MAX_INPUT);
    t->output[0] = 0;
    task_count++;
    print_str("coordinator: task ");
    print_int(t->id);
    print_str(" submitted\n");
    return t->id;
}

static int process_task(int task_id)
{
    for (int i = 0; i < task_count; i++) {
        if (task_queue[i].id == task_id) {
            task_queue[i].state = STATE_RUNNING;
            print_str("coordinator: processing task ");
            print_int(task_id);
            print_str("\n");

            /* Simulate processing */
            copy_string(task_queue[i].output, "Task completed successfully", MAX_OUTPUT);
            task_queue[i].state = STATE_DONE;
            return 0;
        }
    }
    return -1;
}

static void show_help(void)
{
    print_str("agent_coordinator: Agent system coordinator (v9.2)\n");
    print_str("Usage: agent_coordinator [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List agents and tasks\n");
    print_str("\n");
    print_str("Coordinates Planner, Executor, Reviewer, Security, Optimizer agents.\n");
}

static void show_info(void)
{
    print_str("coordinator: info:\n");
    print_str("coordinator: max agents: 16\n");
    print_str("coordinator: max tasks: 64\n");
    print_str("coordinator: agent types: Planner, Executor, Reviewer, Security, Optimizer\n");
    print_str("coordinator: info display complete\n");
}

static void show_status(void)
{
    int idle = 0, running = 0, done = 0;
    for (int i = 0; i < task_count; i++) {
        switch (task_queue[i].state) {
            case STATE_IDLE: idle++; break;
            case STATE_RUNNING: running++; break;
            case STATE_DONE: done++; break;
        }
    }
    print_str("coordinator: agents=");
    print_int(agent_count);
    print_str(" tasks=");
    print_int(task_count);
    print_str(" (idle=");
    print_int(idle);
    print_str(" running=");
    print_int(running);
    print_str(" done=");
    print_int(done);
    print_str(")\n");
}

static void list_agents_and_tasks(int verbose)
{
    print_str("coordinator: agents (");
    print_int(agent_count);
    print_str("/");
    print_int(MAX_AGENTS);
    print_str(")\n");
    for (int i = 0; i < agent_count; i++) {
        print_str("coordinator:   ");
        print_int(agent_list[i].agent_id);
        print_str(" ");
        print_str(agent_list[i].name);
        print_str(" [");
        print_str(agent_type_name(agent_list[i].agent_type));
        print_str("]\n");
    }
    print_str("coordinator: tasks (");
    print_int(task_count);
    print_str("/");
    print_int(MAX_TASKS);
    print_str(")\n");
    for (int i = 0; i < task_count; i++) {
        print_str("coordinator:   task ");
        print_int(task_queue[i].id);
        print_str(" agent=");
        print_int(task_queue[i].agent_id);
        print_str(" state=");
        print_str(state_name(task_queue[i].state));
        if (verbose) {
            print_str(" input=");
            print_str(task_queue[i].input);
            print_str(" output=");
            print_str(task_queue[i].output);
        }
        print_str("\n");
    }
}

static void test_mode(int verbose)
{
    print_str("coordinator: testing\n");

    /* Register agents */
    int p = register_agent("planner", AGENT_PLANNER);
    int e = register_agent("executor", AGENT_EXECUTOR);
    int r = register_agent("reviewer", AGENT_REVIEWER);
    int sec = register_agent("security", AGENT_SECURITY);
    int opt = register_agent("optimizer", AGENT_OPTIMIZER);

    if (p >= 0 && e >= 0 && r >= 0 && sec >= 0 && opt >= 0) {
        print_str("coordinator: register agents: OK\n");
    }

    /* Submit and process tasks */
    int t1 = submit_task("Implement REST API", p, 1);
    int t2 = submit_task("Run test suite", e, 2);
    int t3 = submit_task("Review auth module", r, 3);

    if (t1 >= 0 && t2 >= 0 && t3 >= 0) {
        print_str("coordinator: submit tasks: OK\n");
    }

    if (process_task(t1) == 0 && process_task(t2) == 0 && process_task(t3) == 0) {
        print_str("coordinator: process tasks: OK\n");
    }

    if (verbose) list_agents_and_tasks(1);
    print_str("coordinator: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, info = 0, verbose = 0, status = 0, test = 0, list = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0) info = 1;
        else if (my_strcmp(arg, "-v") == 0) verbose = 1;
        else if (my_strcmp(arg, "-s") == 0) status = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0) list = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }
    if (list) { list_agents_and_tasks(verbose); host_exit(0); }

    print_str("coordinator: usage: agent_coordinator [-t] [-l] [-v] [-i] [-s]\n");
    host_exit(0);
}
