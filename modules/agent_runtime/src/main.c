/* agent_runtime: Agent lifecycle management (v9.0) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_AGENTS 16
#define MAX_TASKS 64
#define MAX_AGENT_NAME 32
#define MAX_AGENT_MEM 2097152 /* 2MB per agent */

/* Agent states */
#define AGENT_IDLE    0
#define AGENT_RUNNING 1
#define AGENT_PAUSED  2
#define AGENT_COMPLETED 3
#define AGENT_FAILED  4

/* Agent types */
#define AGENT_PLANNER   0
#define AGENT_EXECUTOR  1
#define AGENT_REVIEWER  2
#define AGENT_SECURITY  3
#define AGENT_OPTIMIZER  4

typedef struct {
    int id;
    int state;
    int agent_type;
    unsigned int mem_used;
    char name[MAX_AGENT_NAME];
} agent_t;

typedef struct {
    int id;
    int agent_id;
    int state;
    unsigned int priority;
    unsigned int created_at;
} task_t;

static unsigned int heap_pos = 65536;
static agent_t agents[MAX_AGENTS];
static task_t tasks[MAX_TASKS];
static int agent_count = 0;
static int task_count = 0;
static int next_agent_id = 0;
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
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static void zero_mem(unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = 0;
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
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
    while (src[i] && i < max_len - 1) {
        dst[i] = src[i];
        i++;
    }
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
    char buf[16];
    int i = 15;
    int neg = 0;
    buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static const char *state_name(int state)
{
    switch (state) {
        case AGENT_IDLE: return "IDLE";
        case AGENT_RUNNING: return "RUNNING";
        case AGENT_PAUSED: return "PAUSED";
        case AGENT_COMPLETED: return "COMPLETED";
        case AGENT_FAILED: return "FAILED";
        default: return "UNKNOWN";
    }
}

static const char *type_name(int t)
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

static int create_agent(const char *name, int agent_type)
{
    if (agent_count >= MAX_AGENTS) {
        print_str("agent_runtime: max agents reached\n");
        return -1;
    }

    agent_t *a = &agents[agent_count];
    a->id = next_agent_id++;
    a->state = AGENT_IDLE;
    a->agent_type = agent_type;
    a->mem_used = 0;
    copy_string(a->name, name, MAX_AGENT_NAME);

    agent_count++;
    print_str("agent_runtime: created agent id=");
    print_int(a->id);
    print_str(" name=");
    print_str(name);
    print_str(" type=");
    print_str(type_name(agent_type));
    print_str("\n");

    return a->id;
}

static int update_agent_state(int agent_id, int new_state)
{
    for (int i = 0; i < agent_count; i++) {
        if (agents[i].id == agent_id) {
            agents[i].state = new_state;
            print_str("agent_runtime: agent ");
            print_int(agent_id);
            print_str(" state=");
            print_str(state_name(new_state));
            print_str("\n");
            return 0;
        }
    }
    print_str("agent_runtime: agent ");
    print_int(agent_id);
    print_str(" not found\n");
    return -1;
}

static int create_task(int agent_id, unsigned int priority)
{
    if (task_count >= MAX_TASKS) {
        print_str("agent_runtime: max tasks reached\n");
        return -1;
    }

    task_t *t = &tasks[task_count];
    t->id = next_task_id++;
    t->agent_id = agent_id;
    t->state = AGENT_IDLE;
    t->priority = priority;
    t->created_at = task_count;

    task_count++;
    print_str("agent_runtime: created task id=");
    print_int(t->id);
    print_str(" agent=");
    print_int(agent_id);
    print_str(" priority=");
    print_int((int)priority);
    print_str("\n");

    return t->id;
}

static int update_task_state(int task_id, int new_state)
{
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == task_id) {
            tasks[i].state = new_state;
            return 0;
        }
    }
    return -1;
}

static int complete_task(int task_id)
{
    return update_task_state(task_id, AGENT_COMPLETED);
}

static void list_agents(int verbose)
{
    print_str("agent_runtime: agents (");
    print_int(agent_count);
    print_str("/");
    print_int(MAX_AGENTS);
    print_str(")\n");

    for (int i = 0; i < agent_count; i++) {
        print_str("agent_runtime:   id=");
        print_int(agents[i].id);
        print_str(" name=");
        print_str(agents[i].name);
        print_str(" type=");
        print_str(type_name(agents[i].agent_type));
        print_str(" state=");
        print_str(state_name(agents[i].state));
        if (verbose) {
            print_str(" mem=");
            print_int((int)agents[i].mem_used);
            print_str(" bytes");
        }
        print_str("\n");
    }
}

static void list_tasks(int verbose)
{
    print_str("agent_runtime: tasks (");
    print_int(task_count);
    print_str("/");
    print_int(MAX_TASKS);
    print_str(")\n");

    for (int i = 0; i < task_count; i++) {
        print_str("agent_runtime:   id=");
        print_int(tasks[i].id);
        print_str(" agent=");
        print_int(tasks[i].agent_id);
        print_str(" state=");
        print_str(state_name(tasks[i].state));
        print_str(" priority=");
        print_int((int)tasks[i].priority);
        print_str("\n");
    }
}

static void show_help(void)
{
    print_str("agent_runtime - Agent lifecycle management (v9.0)\n");
    print_str("Usage: agent_runtime [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List agents\n");
    print_str("  -L             List tasks\n");
    print_str("\n");
    print_str("Agent types: Planner, Executor, Reviewer, Security, Optimizer\n");
    print_str("Agent states: IDLE, RUNNING, PAUSED, COMPLETED, FAILED\n");
    print_str("Max agents: 16, Max tasks: 64, Max mem/agent: 2MB\n");
}

static void show_info(void)
{
    print_str("agent_runtime: Agent runtime info:\n");
    print_str("agent_runtime: max agents: 16\n");
    print_str("agent_runtime: max tasks: 64\n");
    print_str("agent_runtime: max memory per agent: 2MB\n");
    print_str("agent_runtime: agent types: Planner(0), Executor(1), Reviewer(2), Security(3), Optimizer(4)\n");
    print_str("agent_runtime: info display complete\n");
}

static void show_status(void)
{
    print_str("agent_runtime: Agent runtime status:\n");
    print_str("agent_runtime: active agents: ");
    print_int(agent_count);
    print_str("\n");
    print_str("agent_runtime: active tasks: ");
    print_int(task_count);
    print_str("\n");
    print_str("agent_runtime: status check complete\n");
}

static void test_mode(int verbose)
{
    print_str("agent_runtime: testing agent system\n");

    /* test: create agents */
    if (verbose) {
        print_str("agent_runtime: creating test agents\n");
    }

    int planner = create_agent("planner1", AGENT_PLANNER);
    int executor = create_agent("executor1", AGENT_EXECUTOR);
    int reviewer = create_agent("reviewer1", AGENT_REVIEWER);
    int security = create_agent("security1", AGENT_SECURITY);
    int optimizer = create_agent("optimizer1", AGENT_OPTIMIZER);

    if (planner >= 0 && executor >= 0 && reviewer >= 0) {
        print_str("agent_runtime: create agents: OK\n");
    } else {
        print_str("agent_runtime: create agents: FAIL\n");
    }

    /* test: state transitions */
    if (verbose) {
        print_str("agent_runtime: testing state transitions\n");
    }

    if (update_agent_state(planner, AGENT_RUNNING) == 0 &&
        update_agent_state(planner, AGENT_PAUSED) == 0 &&
        update_agent_state(planner, AGENT_RUNNING) == 0 &&
        update_agent_state(planner, AGENT_COMPLETED) == 0) {
        print_str("agent_runtime: state transitions: OK\n");
    } else {
        print_str("agent_runtime: state transitions: FAIL\n");
    }

    /* test: create tasks */
    if (verbose) {
        print_str("agent_runtime: creating test tasks\n");
    }

    int task1 = create_task(planner, 1);
    int task2 = create_task(executor, 2);

    if (task1 >= 0 && task2 >= 0) {
        print_str("agent_runtime: create tasks: OK\n");
    } else {
        print_str("agent_runtime: create tasks: FAIL\n");
    }

    /* test: complete tasks */
    if (complete_task(task1) == 0 && update_task_state(task2, AGENT_RUNNING) == 0) {
        print_str("agent_runtime: task operations: OK\n");
    } else {
        print_str("agent_runtime: task operations: FAIL\n");
    }

    /* test: list agents and tasks */
    if (verbose) {
        list_agents(1);
        list_tasks(1);
    }

    print_str("agent_runtime: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, list_agents_flag = 0, list_tasks_flag = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];

        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_agents_flag = 1;
        } else if (my_strcmp(arg, "-L") == 0) {
            list_tasks_flag = 1;
        }

        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (info_flag) {
        show_info();
        host_exit(0);
    }

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    if (list_agents_flag) {
        list_agents(verbose_flag);
    }

    if (list_tasks_flag) {
        list_tasks(verbose_flag);
    }

    if (!list_agents_flag && !list_tasks_flag) {
        print_str("agent_runtime: no action specified\n");
        print_str("agent_runtime: usage: agent_runtime [-t] [-l] [-L] [-v] [-i] [-s]\n");
    }

    host_exit(0);
}
