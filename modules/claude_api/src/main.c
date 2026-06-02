/* claude_api: Anthropic Claude API interface (v37.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_MESSAGES    32
#define MAX_CONTENT     4096
#define MAX_API_KEY     64
#define MAX_MODEL       32

/* Claude models */
#define MODEL_CLAUDE_OPUS       0
#define MODEL_CLAUDE_SONNET     1
#define MODEL_CLAUDE_HAIKU      2

/* Roles */
#define ROLE_USER       0
#define ROLE_ASSISTANT  1

/* Stop reasons */
#define STOP_END_TURN   0
#define STOP_MAX_TOKENS 1
#define STOP_SAFETY     2

/* Configuration */
typedef struct {
    char  api_key[MAX_API_KEY];
    int   model;
    int   max_tokens;
    int   temperature;    /* 0-100 */
} claude_config_t;

/* Message */
typedef struct {
    int   role;
    char  content[MAX_CONTENT];
} claude_message_t;

/* Response */
typedef struct {
    char  id[MAX_MODEL];
    char  content[MAX_CONTENT];
    int   input_tokens;
    int   output_tokens;
    int   stop_reason;
} claude_response_t;

static claude_config_t global_config;
static claude_response_t last_response;
static int request_count = 0;
static int total_tokens_used = 0;

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

static const char *model_name(int m) {
    if (m == MODEL_CLAUDE_OPUS) return "claude-3-opus";
    if (m == MODEL_CLAUDE_HAIKU) return "claude-3-haiku";
    return "claude-3-sonnet";
}

static const char *role_name(int r) {
    if (r == ROLE_USER) return "user";
    return "assistant";
}

static const char *stop_name(int s) {
    if (s == STOP_END_TURN) return "end_turn";
    if (s == STOP_MAX_TOKENS) return "max_tokens";
    return "safety";
}

/* Initialize */
int ca_init(const char *api_key, int model, int max_tokens, int temperature) {
    my_strncpy(global_config.api_key, api_key, MAX_API_KEY - 1);
    global_config.model = model;
    global_config.max_tokens = max_tokens;
    global_config.temperature = temperature;
    return 0;
}

/* Count tokens (rough estimate) */
int ca_count_tokens(const char *text) {
    return my_strlen(text) / 4 + 1;
}

/* Messages API (simulated) */
int ca_messages(claude_message_t *msgs, int msg_count, const char *system_prompt, claude_response_t *resp) {
    request_count++;
    int input_tokens = 0;
    if (system_prompt) input_tokens += ca_count_tokens(system_prompt);
    for (int i = 0; i < msg_count; i++) input_tokens += ca_count_tokens(msgs[i].content);
    int output_tokens = msg_count * 40 + 80;
    if (output_tokens > global_config.max_tokens) output_tokens = global_config.max_tokens;
    my_strncpy(resp->id, "msg-simulated", MAX_MODEL - 1);
    my_strncpy(resp->content, "This is a simulated Claude API response. In production, this module would make HTTP requests to api.anthropic.com/v1/messages with the Messages API.", MAX_CONTENT - 1);
    resp->input_tokens = input_tokens;
    resp->output_tokens = output_tokens;
    resp->stop_reason = STOP_END_TURN;
    total_tokens_used += input_tokens + output_tokens;
    return resp->output_tokens;
}

/* Stream messages (simulated) */
int ca_stream_messages(claude_message_t *msgs, int msg_count, const char *system) {
    print_str("  Streaming response (simulated):\n");
    claude_response_t resp;
    ca_messages(msgs, msg_count, system, &resp);
    print_str("    "); print_str(resp.content); print_str("\n");
    print_str("  [Stream complete]\n");
    return resp.output_tokens;
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
    print_str("Claude API v37.0 - Anthropic Claude Interface\n");
    if (help) {
        print_str("Usage: claude_api [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run Claude API test\n");
        return;
    }
    if (test) {
        print_str("=== Claude API Test ===\n\n");
        ca_init("sk-ant-test-key", MODEL_CLAUDE_SONNET, 4096, 70);
        claude_message_t msgs[4];
        int mc = 0;
        msgs[mc].role = ROLE_USER;
        my_strncpy(msgs[mc].content, "Write a haiku about coding.", MAX_CONTENT - 1); mc++;
        claude_response_t resp;
        print_str("Request:\n");
        print_str("  Model: "); print_str(model_name(global_config.model)); print_str("\n");
        print_str("  Messages: "); print_int(mc); print_str("\n\n");
        print_str("Non-streaming:\n");
        ca_messages(msgs, mc, "You are a helpful coding assistant.", &resp);
        print_str("  Content: "); print_str(resp.content); print_str("\n");
        print_str("  Input tokens: "); print_int(resp.input_tokens); print_str("\n");
        print_str("  Output tokens: "); print_int(resp.output_tokens); print_str("\n");
        print_str("  Stop: "); print_str(stop_name(resp.stop_reason)); print_str("\n\n");
        print_str("Streaming:\n");
        ca_stream_messages(msgs, mc, "You are a helpful coding assistant.");
        print_str("\n  Session requests: "); print_int(request_count); print_str("\n");
        print_str("  Total tokens: "); print_int(total_tokens_used); print_str("\n");
        print_str("\n=== Claude API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
