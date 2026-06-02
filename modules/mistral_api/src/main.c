/* mistral_api: Mistral AI API interface (v37.0) */

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
#define MODEL_MISTRAL_LARGE    0
#define MODEL_MISTRAL_MEDIUM   1
#define MODEL_MISTRAL_SMALL    2
#define MODEL_CODESTRAL        3

typedef struct {
    char  api_key[MAX_API_KEY];
    int   model;
    int   max_tokens;
    int   temperature;
} mistral_config_t;

typedef struct {
    char  role[MAX_MODEL];
    char  content[MAX_CONTENT];
} mistral_message_t;

typedef struct {
    char  content[MAX_CONTENT];
    int   tokens_used;
} mistral_response_t;

static mistral_config_t global_config;
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
    if (m == MODEL_MISTRAL_LARGE) return "mistral-large";
    if (m == MODEL_MISTRAL_MEDIUM) return "mistral-medium";
    if (m == MODEL_MISTRAL_SMALL) return "mistral-small";
    return "codestral";
}

int ms_init(const char *api_key, int model, int max_tokens, int temperature) {
    my_strncpy(global_config.api_key, api_key, MAX_API_KEY - 1);
    global_config.model = model;
    global_config.max_tokens = max_tokens;
    global_config.temperature = temperature;
    return 0;
}

int ms_chat(mistral_message_t *msgs, int count, mistral_response_t *resp) {
    request_count++;
    int tokens = 0;
    for (int i = 0; i < count; i++) tokens += my_strlen(msgs[i].content) / 4 + 1;
    int output = count * 45 + 80;
    if (output > global_config.max_tokens) output = global_config.max_tokens;
    my_strncpy(resp->content, "This is a simulated Mistral AI API response. Mistral models provide European-compliant AI with strong multilingual support.", MAX_CONTENT - 1);
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
    print_str("Mistral API v37.0 - Mistral AI Interface\n");
    if (help) { print_str("Usage: mistral_api [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Mistral API Test ===\n\n");
        ms_init("ms-test-key", MODEL_CODESTRAL, 2048, 50);
        mistral_message_t msgs[2];
        int mc = 0;
        my_strncpy(msgs[mc].role, "user", MAX_MODEL - 1);
        my_strncpy(msgs[mc].content, "Write a Python function to reverse a linked list.", MAX_CONTENT - 1); mc++;
        mistral_response_t resp;
        ms_chat(msgs, mc, &resp);
        print_str("  Model: "); print_str(model_name(global_config.model)); print_str("\n");
        print_str("  Response: "); print_str(resp.content); print_str("\n");
        print_str("  Tokens: "); print_int(resp.tokens_used); print_str("\n");
        print_str("\n=== Mistral API Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
