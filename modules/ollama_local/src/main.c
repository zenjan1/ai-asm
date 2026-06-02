/* ollama_local: Local LLM via Ollama (v39.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_MODELS      32
#define MAX_MODEL_NAME  64
#define MAX_PROMPT      4096
#define MAX_RESPONSE    4096
#define MAX_HOST        128
#define MAX_MSGS        16
#define MAX_MSG_TEXT    2048
#define MAX_CONTEXT     256

/* Ollama config */
typedef struct {
    char  host_url[MAX_HOST];
    char  model[MAX_MODEL_NAME];
    int   keep_alive;       /* seconds, -1=infinite */
    int   num_ctx;          /* context window size */
    float temperature;      /* * 100 */
    float top_p;            /* * 100 */
    int   stream;
} ollama_config_t;

/* Ollama model info */
typedef struct {
    char  name[MAX_MODEL_NAME];
    int   size_mb;
    char  digest[64];
    char  modified_at[32];
    char  details[128];
} ollama_model_t;

/* Chat message */
typedef struct {
    char  role[8];          /* system/user/assistant */
    char  content[MAX_MSG_TEXT];
} ollama_message_t;

/* Ollama response */
typedef struct {
    char  model[MAX_MODEL_NAME];
    char  response[MAX_RESPONSE];
    int   done;
    int   total_duration_ms;
    int   eval_count;       /* tokens generated */
    int   prompt_eval_count;
    char  context[MAX_CONTEXT];
} ollama_response_t;

static ollama_config_t global_config;
static ollama_model_t model_registry[MAX_MODELS];
static int model_count = 0;
static int request_count = 0;
static int total_tokens = 0;

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

/* Initialize */
int ol_init(const char *host_url, const char *model, int num_ctx) {
    my_strncpy(global_config.host_url, host_url ? host_url : "http://localhost:11434", MAX_HOST - 1);
    my_strncpy(global_config.model, model ? model : "llama3", MAX_MODEL_NAME - 1);
    global_config.keep_alive = 300;
    global_config.num_ctx = num_ctx > 0 ? num_ctx : 4096;
    global_config.temperature = 70;
    global_config.top_p = 90;
    global_config.stream = 0;
    /* Pre-register some models */
    my_strncpy(model_registry[0].name, "llama3", MAX_MODEL_NAME - 1);
    model_registry[0].size_mb = 4700; my_strncpy(model_registry[0].details, "8B parameters", 63);
    model_count++;
    my_strncpy(model_registry[1].name, "mistral", MAX_MODEL_NAME - 1);
    model_registry[1].size_mb = 4100; my_strncpy(model_registry[1].details, "7B parameters", 63);
    model_count++;
    my_strncpy(model_registry[2].name, "codellama", MAX_MODEL_NAME - 1);
    model_registry[2].size_mb = 3800; my_strncpy(model_registry[2].details, "7B code model", 63);
    model_count++;
    return 0;
}

/* Generate (simulated) */
int ol_generate(const char *prompt, const char *model, ollama_response_t *resp) {
    request_count++;
    if (!prompt || my_strlen(prompt) == 0) return -1;
    int prompt_len = my_strlen(prompt);
    if (model) my_strncpy(resp->model, model, MAX_MODEL_NAME - 1);
    else my_strncpy(resp->model, global_config.model, MAX_MODEL_NAME - 1);
    /* Simulate response */
    if (my_strstr(prompt, "what") || my_strstr(prompt, "how")) {
        my_strncpy(resp->response, "Based on my understanding, this is a simulated response from a locally running language model. The Ollama API enables interaction with LLMs deployed on your machine without requiring external API keys.", MAX_RESPONSE - 1);
    } else if (my_strstr(prompt, "code") || my_strstr(prompt, "function")) {
        my_strncpy(resp->response, "Here is a simulated code example demonstrating the requested functionality. In production, this would connect to the actual Ollama instance and return real generated code.", MAX_RESPONSE - 1);
    } else {
        my_strncpy(resp->response, "This is a simulated response from the local language model running via Ollama. The actual response would be generated by the model deployed at the configured endpoint.", MAX_RESPONSE - 1);
    }
    resp->done = 1;
    resp->eval_count = my_strlen(resp->response) / 4 + 1;
    resp->prompt_eval_count = prompt_len / 4 + 1;
    resp->total_duration_ms = resp->eval_count * 50 + 200;
    total_tokens += resp->eval_count + resp->prompt_eval_count;
    return resp->eval_count;
}

/* Chat (simulated) */
int ol_chat(ollama_message_t messages[], int count, const char *model, ollama_response_t *resp) {
    if (count == 0) return -1;
    /* Combine last user message */
    const char *last_user = "";
    for (int i = count - 1; i >= 0; i--) {
        if (my_strcmp(messages[i].role, "user") == 0) {
            last_user = messages[i].content;
            break;
        }
    }
    return ol_generate(last_user, model, resp);
}

/* List models */
int ol_list_models(void) {
    print_str("  Local Models\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < model_count; i++) {
        print_str("  "); print_str(model_registry[i].name);
        print_str(" - "); print_int(model_registry[i].size_mb);
        print_str(" MB | "); print_str(model_registry[i].details); print_str("\n");
    }
    return model_count;
}

/* Pull model (simulated) */
int ol_pull_model(const char *model_name) {
    if (model_count >= MAX_MODELS) return -1;
    int idx = model_count++;
    my_strncpy(model_registry[idx].name, model_name, MAX_MODEL_NAME - 1);
    model_registry[idx].size_mb = 4000;
    my_strncpy(model_registry[idx].details, "downloaded", 63);
    return 0;
}

/* Delete model */
int ol_delete_model(const char *model_name) {
    for (int i = 0; i < model_count; i++) {
        if (my_strcmp(model_registry[i].name, model_name) == 0) {
            for (int j = i; j < model_count - 1; j++) model_registry[j] = model_registry[j + 1];
            model_count--;
            return 0;
        }
    }
    return -1;
}

/* Show model info */
int ol_show_model(const char *model_name, ollama_model_t *out) {
    for (int i = 0; i < model_count; i++) {
        if (my_strcmp(model_registry[i].name, model_name) == 0) {
            *out = model_registry[i];
            return 0;
        }
    }
    return -1;
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
    print_str("Ollama Local v39.0 - Local LLM via Ollama\n");
    if (help) { print_str("Usage: ollama_local [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Ollama Local Test ===\n\n");
        ol_init("http://localhost:11434", "llama3", 4096);
        print_str("  Host: "); print_str(global_config.host_url); print_str("\n");
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Context: "); print_int(global_config.num_ctx); print_str("\n\n");
        /* List models */
        ol_list_models();
        print_str("\n");
        /* Generate */
        ollama_response_t resp;
        ol_generate("What is machine learning?", "llama3", &resp);
        print_str("Generate:\n");
        print_str("  Model: "); print_str(resp.model); print_str("\n");
        print_str("  Response: "); print_str(resp.response); print_str("\n");
        print_str("  Tokens generated: "); print_int(resp.eval_count); print_str("\n");
        print_str("  Duration: "); print_int(resp.total_duration_ms); print_str("ms\n\n");
        /* Chat */
        ollama_message_t msgs[3];
        my_strncpy(msgs[0].role, "system", 7);
        my_strncpy(msgs[0].content, "You are a helpful assistant.", MAX_MSG_TEXT - 1);
        my_strncpy(msgs[1].role, "user", 4);
        my_strncpy(msgs[1].content, "How do I write a function in Python?", MAX_MSG_TEXT - 1);
        int count = 2;
        ol_chat(msgs, count, "llama3", &resp);
        print_str("Chat:\n");
        print_str("  Response: "); print_str(resp.response); print_str("\n\n");
        /* Pull */
        ol_pull_model("gemma:2b");
        print_str("Pulled gemma:2b, now "); print_int(model_count); print_str(" models\n\n");
        ol_list_models();
        print_str("\n=== Ollama Local Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
