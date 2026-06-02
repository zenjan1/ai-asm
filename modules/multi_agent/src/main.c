/* multi_agent: Multi-Agent collaboration system (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_AGENTS      32
#define MAX_TASKS       64
#define MAX_CAPABILITIES 8
#define MAX_TOOL_NAME   64
#define MAX_MESSAGES    128
#define MAX_MSG_CONTENT 512
#define MAX_NAME        64
#define MAX_DESC        128
#define MAX_RESULT      1024
#define MAX_SPECIAL     64

/* Status codes */
#define AGENT_ACTIVE    0
#define AGENT_INACTIVE  1
#define AGENT_BUSY      2
#define TASK_PENDING    0
#define TASK_RUNNING    1
#define TASK_COMPLETED  2
#define TASK_FAILED     3
#define TASK_CONFLICT   4
#define MSG_REQUEST     0
#define MSG_RESPONSE    1
#define MSG_STATUS      2

/* Agent capability */
typedef struct {
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    int   proficiency_level;  /* 1-10 */
    char  tools_available[MAX_CAPABILITIES][MAX_TOOL_NAME];
    int   tool_count;
} agent_capability_t;

/* Agent profile */
typedef struct {
    int   agent_id;
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    agent_capability_t capabilities[MAX_CAPABILITIES];
    int   capability_count;
    char  specialization[MAX_SPECIAL];
    int   max_concurrent_tasks;
    int   success_rate;    /* * 100 */
    int   current_load;
    int   total_tasks_completed;
    int   priority;
    int   status;
    int   created_at;
    int   last_active_at;
} agent_profile_t;

/* Agent task */
typedef struct {
    int   task_id;
    char  description[MAX_DESC];
    int   assigned_agents[MAX_CAPABILITIES];
    int   agent_count;
    int   status;
    int   priority;
    int   created_at;
    int   deadline;
    int   completed_at;
    int   result_agent_id;
    char  result[MAX_RESULT];
} agent_task_t;

/* Agent message */
typedef struct {
    int   from_agent;
    int   to_agent;
    char  content[MAX_MSG_CONTENT];
    int   msg_type;
    int   timestamp;
} agent_message_t;

/* Agent vote */
typedef struct {
    int   agent_id;
    char  result[MAX_RESULT];
    int   confidence;  /* * 100 */
    char  reason[MAX_DESC];
} agent_vote_t;

/* Collaboration result */
typedef struct {
    char  final_result[MAX_RESULT];
    int   participating_agents[MAX_AGENTS];
    int   participant_count;
    int   consensus_score;  /* * 100 */
    int   conflict_detected;
    char  resolution[MAX_DESC];
} collaboration_result_t;

static agent_profile_t agents[MAX_AGENTS];
static agent_task_t tasks[MAX_TASKS];
static agent_message_t messages[MAX_MESSAGES];
static int agent_count = 0;
static int next_agent_id = 1;
static int task_count = 0;
static int next_task_id = 1;
static int message_count = 0;
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

/* Register agent */
int ma_register_agent(const char *name, const char *description, const char *specialization) {
    if (agent_count >= MAX_AGENTS) return -1;
    int idx = agent_count++;
    agents[idx].agent_id = next_agent_id++;
    my_strncpy(agents[idx].name, name, MAX_NAME - 1);
    my_strncpy(agents[idx].description, description, MAX_DESC - 1);
    my_strncpy(agents[idx].specialization, specialization ? specialization : "general", MAX_SPECIAL - 1);
    agents[idx].capability_count = 0;
    agents[idx].max_concurrent_tasks = 3;
    agents[idx].success_rate = 0;
    agents[idx].current_load = 0;
    agents[idx].total_tasks_completed = 0;
    agents[idx].priority = 5;
    agents[idx].status = AGENT_ACTIVE;
    agents[idx].created_at = agents[idx].last_active_at = ++clock;
    return agents[idx].agent_id;
}

int ma_unregister_agent(int agent_id) {
    for (int i = 0; i < agent_count; i++) {
        if (agents[i].agent_id == agent_id) {
            agents[i].status = AGENT_INACTIVE;
            return 0;
        }
    }
    return -1;
}

agent_profile_t *ma_find_agent(int agent_id) {
    for (int i = 0; i < agent_count; i++) {
        if (agents[i].agent_id == agent_id && agents[i].status != AGENT_INACTIVE) return &agents[i];
    }
    return NULL;
}

/* Assign task to agents */
int ma_assign_task(const char *description, int priority, int *agent_ids, int count) {
    if (task_count >= MAX_TASKS) return -1;
    int idx = task_count++;
    tasks[idx].task_id = next_task_id++;
    my_strncpy(tasks[idx].description, description, MAX_DESC - 1);
    tasks[idx].agent_count = count;
    for (int i = 0; i < count && i < MAX_CAPABILITIES; i++) {
        tasks[idx].assigned_agents[i] = agent_ids[i];
    }
    tasks[idx].status = TASK_RUNNING;
    tasks[idx].priority = priority;
    tasks[idx].created_at = tasks[idx].completed_at = ++clock;
    tasks[idx].deadline = 0;
    tasks[idx].result_agent_id = 0;
    tasks[idx].result[0] = '\0';
    /* Update agent loads */
    for (int i = 0; i < count; i++) {
        agent_profile_t *a = ma_find_agent(agent_ids[i]);
        if (a) a->current_load++;
    }
    return tasks[idx].task_id;
}

/* Send message between agents */
int ma_send_message(int from_agent, int to_agent, const char *content, int msg_type) {
    if (message_count >= MAX_MESSAGES) return -1;
    int idx = message_count++;
    messages[idx].from_agent = from_agent;
    messages[idx].to_agent = to_agent;
    my_strncpy(messages[idx].content, content, MAX_MSG_CONTENT - 1);
    messages[idx].msg_type = msg_type;
    messages[idx].timestamp = ++clock;
    return 0;
}

/* Aggregate results with voting */
int ma_aggregate_results(agent_vote_t *votes, int count, collaboration_result_t *result) {
    result->participant_count = count;
    result->conflict_detected = 0;
    int total_confidence = 0;
    int best_vote = 0;
    for (int i = 0; i < count; i++) {
        result->participating_agents[i] = votes[i].agent_id;
        total_confidence += votes[i].confidence;
        if (votes[i].confidence > votes[best_vote].confidence) best_vote = i;
    }
    /* Use highest confidence vote as final */
    my_strncpy(result->final_result, votes[best_vote].result, MAX_RESULT - 1);
    result->consensus_score = total_confidence / (count > 0 ? count : 1);
    /* Detect conflict if votes differ significantly */
    for (int i = 0; i < count; i++) {
        if (i != best_vote && my_strcmp(votes[i].result, votes[best_vote].result) != 0) {
            result->conflict_detected = 1;
            break;
        }
    }
    my_strncpy(result->resolution, result->conflict_detected ? "Resolved by confidence weighting" : "Consensus reached", MAX_DESC - 1);
    return count;
}

/* List agents */
int ma_list_agents(void) {
    print_str("  Registered Agents\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < agent_count; i++) {
        if (agents[i].status == AGENT_INACTIVE) continue;
        print_str("  #"); print_int(agents[i].agent_id);
        print_str(" "); print_str(agents[i].name);
        print_str(" ["); print_str(agents[i].specialization);
        print_str("] load="); print_int(agents[i].current_load);
        print_str("/"); print_int(agents[i].max_concurrent_tasks);
        const char *st = agents[i].status == AGENT_ACTIVE ? "active" : "busy";
        print_str(" "); print_str(st);
        print_str("\n");
    }
    return agent_count;
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
    print_str("Multi-Agent v40.0 - Multi-Agent Collaboration System\n");
    if (help) { print_str("Usage: multi_agent [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Multi-Agent Test ===\n\n");
        /* Register agents */
        int a1 = ma_register_agent("Coder", "Expert in code generation", "coding");
        int a2 = ma_register_agent("Analyst", "Data analysis specialist", "analysis");
        int a3 = ma_register_agent("Researcher", "Information retrieval expert", "research");
        print_str("Registered "); print_int(agent_count); print_str(" agents\n\n");
        ma_list_agents();
        print_str("\n");
        /* Assign task */
        int task_agents[3] = {a1, a2, a3};
        int t1 = ma_assign_task("Analyze the dataset and generate a summary report", 5, task_agents, 3);
        print_str("Task #"); print_int(t1); print_str(" assigned to 3 agents\n\n");
        /* Messages between agents */
        ma_send_message(a2, a1, "I found 15 data points, can you help format them?", MSG_REQUEST);
        ma_send_message(a1, a2, "Sure, I'll generate a Python script for formatting.", MSG_RESPONSE);
        ma_send_message(a3, a1, "Found related research papers to cite.", MSG_REQUEST);
        print_str("Messages exchanged: "); print_int(message_count); print_str("\n\n");
        /* Voting */
        agent_vote_t votes[3];
        votes[0].agent_id = a1; votes[0].confidence = 8500;
        my_strncpy(votes[0].result, "Code-based analysis with Python visualization", MAX_RESULT - 1);
        votes[1].agent_id = a2; votes[1].confidence = 9200;
        my_strncpy(votes[1].result, "Statistical analysis with descriptive statistics", MAX_RESULT - 1);
        votes[2].agent_id = a3; votes[2].confidence = 7800;
        my_strncpy(votes[2].result, "Literature review with data correlation", MAX_RESULT - 1);
        collaboration_result_t collab;
        ma_aggregate_results(votes, 3, &collab);
        print_str("Collaboration result:\n");
        print_str("  Final: "); print_str(collab.final_result); print_str("\n");
        print_str("  Consensus: "); print_int(collab.consensus_score / 100); print_str("%\n");
        print_str("  Conflict: "); print_str(collab.conflict_detected ? "Yes" : "No"); print_str("\n");
        print_str("  Resolution: "); print_str(collab.resolution); print_str("\n");
        print_str("\n=== Multi-Agent Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
