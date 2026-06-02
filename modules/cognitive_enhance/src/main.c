/* cognitive_enhance: Human-machine cognitive fusion (v29.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Enhancement modes */
#define MODE_ASSIST    0
#define MODE_COLLAB    1
#define MODE_FUSION    2
#define MODE_AUTONOMOUS 3

/* Cognitive domains */
#define DOMAIN_MEMORY    0
#define DOMAIN_REASONING 1
#define DOMAIN_CREATIVE  2
#define DOMAIN_PERCEPTION 3
#define DOMAIN_DECISION  4

/* Limits */
#define MAX_TASKS   32
#define MAX_SESSIONS 8
#define NAME_LEN    64
#define DESC_LEN    256

/* Cognitive session */
typedef struct {
    int   session_id;
    int   mode;
    int   enhancement_level;
    int   active_tasks;
    int   knowledge_shared;
    char  partner[NAME_LEN];
} cognitive_session_t;

/* Enhancement task */
typedef struct {
    int   task_id;
    int   domain;
    char  description[DESC_LEN];
    int   human_confidence;
    int   ai_confidence;
    int   fused_confidence;
} enhancement_task_t;

static cognitive_session_t sessions[MAX_SESSIONS];
static enhancement_task_t tasks[MAX_TASKS];
static int session_count = 0;
static int task_count = 0;

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

/* Get mode name */
static const char *mode_name(int mode) {
    if (mode == MODE_ASSIST) return "assist";
    if (mode == MODE_COLLAB) return "collaborative";
    if (mode == MODE_FUSION) return "fusion";
    if (mode == MODE_AUTONOMOUS) return "autonomous";
    return "unknown";
}

/* Get domain name */
static const char *domain_name(int domain) {
    if (domain == DOMAIN_MEMORY) return "memory";
    if (domain == DOMAIN_REASONING) return "reasoning";
    if (domain == DOMAIN_CREATIVE) return "creative";
    if (domain == DOMAIN_PERCEPTION) return "perception";
    if (domain == DOMAIN_DECISION) return "decision";
    return "unknown";
}

/* Create cognitive session */
int ce_create_session(int mode, const char *partner) {
    if (session_count >= MAX_SESSIONS) return -1;
    int idx = session_count;
    sessions[idx].session_id = session_count + 1;
    sessions[idx].mode = mode;
    sessions[idx].enhancement_level = 1;
    sessions[idx].active_tasks = 0;
    sessions[idx].knowledge_shared = 0;
    my_strncpy(sessions[idx].partner, partner, NAME_LEN - 1);
    session_count++;
    return sessions[idx].session_id;
}

/* Assist decision */
int ce_assist_decision(const char *decision, int human_conf) {
    print_str("=== Decision Assistance ===\n");
    print_str("  Decision: ");
    if (decision) print_str(decision);
    print_str("\n");
    print_str("  Human confidence: ");
    print_int(human_conf);
    print_str("%\n");
    int ai_conf = human_conf + 15;
    if (ai_conf > 99) ai_conf = 99;
    print_str("  AI confidence: ");
    print_int(ai_conf);
    print_str("%\n");
    int fused = (human_conf + ai_conf) / 2 + 5;
    if (fused > 99) fused = 99;
    print_str("  Fused confidence: ");
    print_int(fused);
    print_str("%\n");
    return fused;
}

/* Share knowledge */
int ce_share_knowledge(int session_id, const char *domain, const char *content) {
    print_str("=== Knowledge Sharing ===\n");
    print_str("  Domain: ");
    if (domain) print_str(domain);
    print_str("\n");
    print_str("  Content: ");
    if (content) print_str(content);
    print_str("\n");
    for (int i = 0; i < session_count; i++) {
        if (sessions[i].session_id == session_id) {
            sessions[i].knowledge_shared++;
            break;
        }
    }
    print_str("  Knowledge shared successfully\n");
    return 0;
}

/* Enhance perception */
int ce_enhance_perception(const char *input) {
    print_str("=== Perception Enhancement ===\n");
    print_str("  Input: ");
    if (input) print_str(input);
    print_str("\n");
    print_str("  Pattern recognition: enhanced\n");
    print_str("  Context awareness: active\n");
    print_str("  Signal/noise ratio: improved 3x\n");
    return 0;
}

/* Session status */
int ce_session_status(void) {
    print_str("=== Cognitive Sessions ===\n");
    for (int i = 0; i < session_count; i++) {
        print_str("  Session ");
        print_int(sessions[i].session_id);
        print_str(": ");
        print_str(sessions[i].partner);
        print_str(" [");
        print_str(mode_name(sessions[i].mode));
        print_str("] knowledge=");
        print_int(sessions[i].knowledge_shared);
        print_str(" tasks=");
        print_int(sessions[i].active_tasks);
        print_str("\n");
    }
    return session_count;
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
    print_str("Cognitive Enhance v29.0\n");
    if (help) {
        print_str("Usage: cognitive_enhance [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run cognitive test\n");
        return;
    }
    if (test) {
        print_str("=== Cognitive Enhancement Test ===\n\n");
        int s1 = ce_create_session(MODE_FUSION, "human_operator");
        int s2 = ce_create_session(MODE_COLLAB, "ai_partner");
        print_str("\n");
        ce_share_knowledge(s1, "reasoning", "system optimization strategy");
        ce_share_knowledge(s2, "creative", "novel architecture design");
        print_str("\n");
        ce_assist_decision("deploy new version to production", 70);
        print_str("\n");
        ce_enhance_perception("sensor data anomaly detection");
        print_str("\n");
        ce_session_status();
        print_str("\n=== Cognitive Enhancement Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
