/* qwen_api: Alibaba Tongyi Qwen API interface (v37.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_CONTENT     4096
#define MAX_API_KEY     64
#define MAX_MODEL       32
#define MAX_MESSAGES    32

/* Models */
#define MODEL_QWEN_MAX     0
#define MODEL_QWEN_PLUS    1
#define MODEL_QWEN_TURBO   2

typedef struct {
    char  api_key[MAX_API_KEY];
    int   model;
    int   max_tokens;
    int   temperature;
    int   top_p;
} qwen_config_t;

typedef struct {
    char  role[MAX_MODEL];
    char  content[MAX_CONTENT];
} qwen_message_t;

typedef struct {
    char  content[MAX_CONTENT];
    int   tokens_used;
} qwen_response_t;

static qwen_config_t global_config;
static int request_count = 0;
static int total_tokens = 0;

static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
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

static const char *model_name(int m) {
    if (m == MODEL_QWEN_MAX) return "qwen-max";
    if (m == MODEL_QWEN_PLUS) return "qwen-plus";
    return "qwen-turbo";
}

int qw_init(const char *api_key, int model, int max_tokens, int temperature, int top_p) {
    my_strncpy(global_config.api_key, api_key, MAX_API_KEY - 1);
    global_config.model = model;
    global_config.max_tokens = max_tokens;
    global_config.temperature = temperature;
    global_config.top_p = top_p;
    return 0;
}

int qw_chat(qwen_message_t *msgs, int count, qwen_response_t *resp) {
    request_count++;
    int tokens = 0;
    for (int i = 0; i < count; i++) tokens += my_strlen(msgs[i].content) / 4 + 1;
    int output = count * 55 + 100;
    if (output > global_config.max_tokens) output = global_config.max_tokens;
    my_strncpy(resp->content, "This is a simulated Qwen API response. Qwen provides Chinese-optimized LLM with strong multilingual and coding capabilities via Alibaba Cloud DashScope.", MAX_CONTENT - 1);
    resp->tokens_used = tokens + output;
    total_tokens += resp->tokens_used;
    return resp->tokens_used;
}

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
    print_str("Qwen API v37.0 - Alibaba Tongyi Qwen Interface\n");
    if (help) { print_str("Usage: qwen_api [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Qwen API Test ===\n\n");
        qw_init("sk-qwen-test-key", MODEL_QWEN_MAX, 4096, 70, 90);
        qwen_message_t msgs[2];
        int mc = 0;
        my_strncpy(msgs[mc].role, "user", MAX_MODEL - 1);
        my_strncpy(msgs[mc].content, "Explain the four seasons in Chinese.", MAX_CONTENT - 1); mc++;
        qwen_response_t resp;
        qw_chat(msgs, mc, &resp);
        print_str("  Model: "); print_str(model_name(global_config.model)); print_str("\n");
        print_str("  Response: "); print_str(resp.content); print_str("\n");
        print_str("  Tokens: "); print_int(resp.tokens_used); print_str("\n");
        print_str("\n=== Qwen API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
