/* openai_api: OpenAI GPT API interface (v37.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_MESSAGES    32
#define MAX_CONTENT     2048
#define MAX_API_KEY     64
#define MAX_MODEL       32

/* Roles */
#define ROLE_SYSTEM     0
#define ROLE_USER       1
#define ROLE_ASSISTANT  2

/* Models */
#define MODEL_GPT4          0
#define MODEL_GPT4_TURBO    1
#define MODEL_GPT35_TURBO   2

/* Finish reasons */
#define FINISH_STOP     0
#define FINISH_LENGTH   1
#define FINISH_CONTENT  2

/* Configuration */
typedef struct {
    char  api_key[MAX_API_KEY];
    char  base_url[MAX_MODEL];
    int   model;
    int   max_tokens;
    int   temperature;    /* 0-100 */
} openai_config_t;

/* Message */
typedef struct {
    int   role;
    char  content[MAX_CONTENT];
} openai_message_t;

/* Request */
typedef struct {
    openai_message_t messages[MAX_MESSAGES];
    int   msg_count;
    int   stream;
} openai_request_t;

/* Response */
typedef struct {
    char  id[MAX_MODEL];
    char  content[MAX_CONTENT];
    int   tokens_used;
    int   finish_reason;
    int   cost_cents;     /* estimated cost in cents */
} openai_response_t;

static openai_config_t global_config;
static openai_response_t last_response;
static int request_count = 0;
static int total_tokens = 0;
static int total_cost_cents = 0;

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

static const char *model_name(int m) {
    if (m == MODEL_GPT4) return "gpt-4";
    if (m == MODEL_GPT4_TURBO) return "gpt-4-turbo";
    return "gpt-3.5-turbo";
}

static const char *role_name(int r) {
    if (r == ROLE_SYSTEM) return "system";
    if (r == ROLE_USER) return "user";
    return "assistant";
}

static const char *finish_name(int f) {
    if (f == FINISH_STOP) return "stop";
    if (f == FINISH_LENGTH) return "length";
    return "content_filter";
}

/* Initialize config */
int oa_init(const char *api_key, int model, int max_tokens, int temperature) {
    my_strncpy(global_config.api_key, api_key, MAX_API_KEY - 1);
    my_strncpy(global_config.base_url, "api.openai.com", MAX_MODEL - 1);
    global_config.model = model;
    global_config.max_tokens = max_tokens;
    global_config.temperature = temperature;
    return 0;
}

/* Count tokens (rough estimate: ~4 chars per token) */
int oa_count_tokens(const char *text) {
    int len = my_strlen(text);
    return len / 4 + 1;
}

/* Estimate cost based on model and tokens */
int oa_estimate_cost(int tokens, int model) {
    /* Cost per 1K tokens in cents: GPT-4=3, GPT-4-Turbo=1, GPT-3.5=0.05 */
    int cost_per_1k = 3;
    if (model == MODEL_GPT4_TURBO) cost_per_1k = 1;
    else if (model == MODEL_GPT35_TURBO) cost_per_1k = 0; /* ~0.05 rounds to 0 */
    return tokens * cost_per_1k / 1000 + 1;
}

/* Chat completion (simulated) */
int oa_chat_completion(openai_request_t *req, openai_response_t *resp) {
    request_count++;
    /* Count input tokens */
    int input_tokens = 0;
    for (int i = 0; i < req->msg_count; i++) {
        input_tokens += oa_count_tokens(req->messages[i].content);
    }
    /* Simulate response content */
    int output_tokens = req->msg_count * 50 + 100;
    if (output_tokens > global_config.max_tokens) output_tokens = global_config.max_tokens;
    /* Generate simulated response */
    my_strncpy(resp->id, "chatcmpl-simulated", MAX_MODEL - 1);
    my_strncpy(resp->content, "This is a simulated OpenAI API response. In production, this module would make HTTP requests to api.openai.com/v1/chat/completions with the configured model and messages.", MAX_CONTENT - 1);
    resp->tokens_used = input_tokens + output_tokens;
    resp->finish_reason = FINISH_STOP;
    resp->cost_cents = oa_estimate_cost(resp->tokens_used, global_config.model);
    total_tokens += resp->tokens_used;
    total_cost_cents += resp->cost_cents;
    return resp->tokens_used;
}

/* Print request */
int oa_print_request(openai_request_t *req) {
    print_str("  Model: ");
    print_str(model_name(global_config.model));
    print_str("\n  Messages: ");
    print_int(req->msg_count);
    print_str("\n  Temperature: ");
    print_int(global_config.temperature);
    print_str("%\n  Max Tokens: ");
    print_int(global_config.max_tokens);
    print_str("\n");
    for (int i = 0; i < req->msg_count; i++) {
        print_str("  [");
        print_str(role_name(req->messages[i].role));
        print_str("] ");
        print_str(req->messages[i].content);
        print_str("\n");
    }
    return req->msg_count;
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
    print_str("OpenAI API v37.0 - GPT-4/GPT-3.5 Interface\n");
    if (help) {
        print_str("Usage: openai_api [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run OpenAI API test\n");
        return;
    }
    if (test) {
        print_str("=== OpenAI API Test ===\n\n");
        oa_init("sk-test-key-12345", MODEL_GPT4_TURBO, 4096, 70);
        print_str("Config initialized\n\n");
        openai_request_t req;
        req.msg_count = 0;
        req.stream = 0;
        req.messages[req.msg_count].role = ROLE_SYSTEM;
        my_strncpy(req.messages[req.msg_count].content, "You are a helpful assistant.", MAX_CONTENT - 1);
        req.msg_count++;
        req.messages[req.msg_count].role = ROLE_USER;
        my_strncpy(req.messages[req.msg_count].content, "Explain quantum computing in 3 sentences.", MAX_CONTENT - 1);
        req.msg_count++;
        print_str("Request:\n");
        oa_print_request(&req);
        print_str("\n");
        openai_response_t resp;
        oa_chat_completion(&req, &resp);
        print_str("Response:\n");
        print_str("  ID: "); print_str(resp.id); print_str("\n");
        print_str("  Content: "); print_str(resp.content); print_str("\n");
        print_str("  Tokens: "); print_int(resp.tokens_used); print_str("\n");
        print_str("  Finish: "); print_str(finish_name(resp.finish_reason)); print_str("\n");
        print_str("  Cost: "); print_int(resp.cost_cents); print_str(" cents\n\n");
        print_str("Session Stats:\n");
        print_str("  Requests: "); print_int(request_count); print_str("\n");
        print_str("  Total tokens: "); print_int(total_tokens); print_str("\n");
        print_str("  Total cost: "); print_int(total_cost_cents); print_str(" cents\n");
        print_str("\n=== OpenAI API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
