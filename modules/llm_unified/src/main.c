/* llm_unified: Unified LLM interface layer (v37.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PROVIDERS   16
#define MAX_CONTENT     4096
#define MAX_NAME        64
#define MAX_REQUEST     1024

/* Provider types */
#define PROV_OPENAI     0
#define PROV_CLAUDE     1
#define PROV_GEMINI     2
#define PROV_DEEPSEEK   3
#define PROV_MISTRAL    4
#define PROV_QWEN       5
#define PROV_OLLAMA     6

/* Task types for routing */
#define TASK_GENERAL    0
#define TASK_CODING     1
#define TASK_MATH       2
#define TASK_CREATIVE   3
#define TASK_LONG_DOC   4
#define TASK_VISION     5

/* Provider entry */
typedef struct {
    int   id;
    char  name[MAX_NAME];
    char  api_module[MAX_NAME];
    int   cost_per_1k;     /* cost in cents */
    int   max_tokens;
    int   latency_ms;      /* avg latency */
    int   enabled;
    int   request_count;
    int   total_tokens;
} llm_provider_t;

/* Request */
typedef struct {
    char  prompt[MAX_REQUEST];
    int   max_tokens;
    int   temperature;
    int   preferred_provider;
    int   task_type;
} llm_request_t;

/* Response */
typedef struct {
    char  content[MAX_CONTENT];
    char  provider_used[MAX_NAME];
    int   tokens;
    int   cost_cents;
    int   latency_ms;
} llm_response_t;

static llm_provider_t providers[MAX_PROVIDERS];
static int provider_count = 0;
static int total_requests = 0;
static int total_cost = 0;

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

static const char *task_name(int t) {
    if (t == TASK_CODING) return "coding";
    if (t == TASK_MATH) return "math";
    if (t == TASK_CREATIVE) return "creative";
    if (t == TASK_LONG_DOC) return "long_doc";
    if (t == TASK_VISION) return "vision";
    return "general";
}

/* Register provider */
int llm_register(int id, const char *name, const char *module, int cost, int max_tok, int latency) {
    if (provider_count >= MAX_PROVIDERS) return -1;
    int idx = provider_count;
    providers[idx].id = id;
    my_strncpy(providers[idx].name, name, MAX_NAME - 1);
    my_strncpy(providers[idx].api_module, module, MAX_NAME - 1);
    providers[idx].cost_per_1k = cost;
    providers[idx].max_tokens = max_tok;
    providers[idx].latency_ms = latency;
    providers[idx].enabled = 1;
    providers[idx].request_count = 0;
    providers[idx].total_tokens = 0;
    provider_count++;
    return providers[idx].id;
}

/* Get best provider for task type */
int llm_get_best_provider(int task_type) {
    int best = -1, best_score = 0;
    for (int i = 0; i < provider_count; i++) {
        if (!providers[i].enabled) continue;
        int score = 100;
        /* Task-specific scoring */
        if (task_type == TASK_CODING) {
            if (my_strstr(providers[i].name, "coder") || my_strstr(providers[i].name, "deepseek")) score += 20;
        } else if (task_type == TASK_LONG_DOC) {
            if (my_strstr(providers[i].name, "gemini") || my_strstr(providers[i].name, "claude")) score += 20;
        } else if (task_type == TASK_MATH) {
            if (my_strstr(providers[i].name, "qwen") || my_strstr(providers[i].name, "deepseek")) score += 15;
        }
        /* Cost efficiency */
        score -= providers[i].cost_per_1k / 10;
        /* Latency */
        score -= providers[i].latency_ms / 50;
        if (score > best_score) { best_score = score; best = i; }
    }
    return best;
}

/* Chat with auto-routing */
int llm_chat(llm_request_t *req, llm_response_t *resp) {
    total_requests++;
    int best = llm_get_best_provider(req->task_type);
    if (best < 0) return -1;
    int tokens = my_strlen(req->prompt) / 4 + 100;
    if (tokens > req->max_tokens) tokens = req->max_tokens;
    int cost = tokens * providers[best].cost_per_1k / 1000 + 1;
    my_strncpy(resp->content, "This is a unified LLM response, automatically routed to the optimal provider.", MAX_CONTENT - 1);
    my_strncpy(resp->provider_used, providers[best].name, MAX_NAME - 1);
    resp->tokens = tokens;
    resp->cost_cents = cost;
    resp->latency_ms = providers[best].latency_ms;
    providers[best].request_count++;
    providers[best].total_tokens += tokens;
    total_cost += cost;
    return resp->tokens;
}

/* Chat with specific provider */
int llm_chat_with_provider(llm_request_t *req, const char *provider_name, llm_response_t *resp) {
    total_requests++;
    for (int i = 0; i < provider_count; i++) {
        if (my_strcmp(providers[i].name, provider_name) == 0) {
            int tokens = my_strlen(req->prompt) / 4 + 100;
            if (tokens > req->max_tokens) tokens = req->max_tokens;
            int cost = tokens * providers[i].cost_per_1k / 1000 + 1;
            my_strncpy(resp->content, "Response from specified provider.", MAX_CONTENT - 1);
            my_strncpy(resp->provider_used, provider_name, MAX_NAME - 1);
            resp->tokens = tokens;
            resp->cost_cents = cost;
            resp->latency_ms = providers[i].latency_ms;
            providers[i].request_count++;
            providers[i].total_tokens += tokens;
            total_cost += cost;
            return resp->tokens;
        }
    }
    return -1;
}

/* Estimate cost */
int llm_estimate_cost(llm_request_t *req, const char *provider_name) {
    int tokens = my_strlen(req->prompt) / 4 + req->max_tokens;
    for (int i = 0; i < provider_count; i++) {
        if (my_strcmp(providers[i].name, provider_name) == 0) {
            return tokens * providers[i].cost_per_1k / 1000 + 1;
        }
    }
    return tokens / 1000 + 1;
}

/* Provider listing */
int llm_list_providers(void) {
    print_str("  Available Providers\n");
    print_str("  =======================================================================\n");
    print_str("  ID  Name          Cost/1K  Max Tokens  Latency  Requests  Enabled\n");
    print_str("  -----------------------------------------------------------------------\n");
    for (int i = 0; i < provider_count; i++) {
        print_int(providers[i].id);
        print_str("  ");
        print_str(providers[i].name);
        int len = my_strlen(providers[i].name);
        for (int p = 0; p < 14 - len; p++) print_str(" ");
        print_int(providers[i].cost_per_1k);
        print_str("c      ");
        print_int(providers[i].max_tokens);
        print_str("     ");
        print_int(providers[i].latency_ms);
        print_str("ms    ");
        print_int(providers[i].request_count);
        print_str("       ");
        if (providers[i].enabled) print_str("YES");
        else print_str("NO");
        print_str("\n");
    }
    return provider_count;
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
    print_str("LLM Unified v37.0 - Unified LLM Interface Layer\n");
    if (help) { print_str("Usage: llm_unified [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== LLM Unified Test ===\n\n");
        /* Register all providers */
        llm_register(PROV_OPENAI, "openai", "openai_api", 3, 128000, 500);
        llm_register(PROV_CLAUDE, "claude", "claude_api", 3, 200000, 400);
        llm_register(PROV_GEMINI, "gemini", "gemini_api", 2, 1000000, 350);
        llm_register(PROV_DEEPSEEK, "deepseek", "deepseek_api", 1, 128000, 600);
        llm_register(PROV_MISTRAL, "mistral", "mistral_api", 1, 128000, 300);
        llm_register(PROV_QWEN, "qwen", "qwen_api", 1, 128000, 450);
        print_str("Providers registered: "); print_int(provider_count); print_str("\n\n");
        llm_list_providers();
        print_str("\n");
        /* Auto-routed requests */
        print_str("Auto-routed requests:\n");
        llm_request_t req;
        llm_response_t resp;
        my_strncpy(req.prompt, "Write a binary search tree in Rust.", MAX_REQUEST - 1);
        req.max_tokens = 2048; req.temperature = 70; req.task_type = TASK_CODING;
        llm_chat(&req, &resp);
        print_str("  Task: coding -> "); print_str(resp.provider_used);
        print_str(" ("); print_int(resp.tokens); print_str(" tokens, ");
        print_int(resp.cost_cents); print_str("c, ");
        print_int(resp.latency_ms); print_str("ms)\n");
        req.task_type = TASK_LONG_DOC;
        my_strncpy(req.prompt, "Summarize this 50-page research paper.", MAX_REQUEST - 1);
        llm_chat(&req, &resp);
        print_str("  Task: long_doc -> "); print_str(resp.provider_used);
        print_str(" ("); print_int(resp.tokens); print_str(" tokens, ");
        print_int(resp.cost_cents); print_str("c)\n");
        req.task_type = TASK_GENERAL;
        my_strncpy(req.prompt, "What is the meaning of life?", MAX_REQUEST - 1);
        llm_chat(&req, &resp);
        print_str("  Task: general -> "); print_str(resp.provider_used);
        print_str(" ("); print_int(resp.tokens); print_str(" tokens, ");
        print_int(resp.cost_cents); print_str("c)\n");
        /* Specific provider */
        print_str("\nSpecific provider request:\n");
        req.task_type = TASK_GENERAL;
        my_strncpy(req.prompt, "Hello from Claude!", MAX_REQUEST - 1);
        llm_chat_with_provider(&req, "claude", &resp);
        print_str("  Provider: "); print_str(resp.provider_used);
        print_str(" ("); print_int(resp.tokens); print_str(" tokens)\n");
        print_str("\n  Total requests: "); print_int(total_requests); print_str("\n");
        print_str("  Total cost: "); print_int(total_cost); print_str(" cents\n");
        print_str("\n=== LLM Unified Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
