/* ai_router: AI model intelligent routing (v11.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern void host_http_request(const char *method, const char *url, const char *body, char *response, int max_len);
extern int host_ai_query(const char *prompt, char *response, int max_len);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Routing strategies */
#define ROUTE_LOCAL_FIRST    0
#define ROUTE_REMOTE_FIRST   1
#define ROUTE_COST_OPTIMIZE  2
#define ROUTE_QUALITY_FIRST  3
#define ROUTE_LOAD_BALANCE   4

/* Model types */
#define MODEL_TYPE_LOCAL  0
#define MODEL_TYPE_HTTP   1
#define MODEL_TYPE_WS     2

/* Model states */
#define MODEL_AVAILABLE  1
#define MODEL_UNAVAILABLE 0
#define MODEL_OVERLOADED  2

/* Limits */
#define MAX_MODELS    8
#define MAX_HISTORY   32
#define MODEL_NAME_LEN 32
#define URL_LEN       128
#define RESPONSE_LEN  2048

/* AI model entry */
typedef struct {
    int     model_id;
    char    name[MODEL_NAME_LEN];
    int     type;               /* LOCAL, HTTP, WS */
    int     cost_per_token;     /* cost (local=0) */
    int     quality_score;      /* 1-10 */
    int     latency_ms;         /* average latency */
    int     availability;       /* 0-100 */
    int     state;              /* AVAILABLE, UNAVAILABLE, OVERLOADED */
    char    url[URL_LEN];       /* API endpoint */
    char    api_key[64];        /* API key placeholder */
    int     total_queries;
    int     failed_queries;
    int     total_latency;
} ai_model_t;

/* Routing history entry */
typedef struct {
    int     query_id;
    int     model_used;
    int     latency_ms;
    int     success;
    int     tokens_used;
    int     cost;
} route_history_t;

/* Router state */
static ai_model_t      models[MAX_MODELS];
static route_history_t history[MAX_HISTORY];
static int model_count = 0;
static int history_count = 0;
static int next_query_id = 1;
static int next_model_id = 1;
static int current_strategy = ROUTE_LOCAL_FIRST;

/* String utilities */
static int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}

static void my_strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = '\0';
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* ===== Model registration ===== */

/* Register an AI model for routing
   model: model configuration
   Returns: model_id or -1 */
int ai_router_register_model(ai_model_t *model) {
    if (!model) return -1;
    if (model_count >= MAX_MODELS) {
        print_str("[router] max models reached\n");
        return -1;
    }

    int idx = model_count;
    models[idx].model_id = next_model_id;
    my_strncpy(models[idx].name, model->name, MODEL_NAME_LEN - 1);
    models[idx].type = model->type;
    models[idx].cost_per_token = model->cost_per_token;
    models[idx].quality_score = model->quality_score;
    models[idx].latency_ms = model->latency_ms;
    models[idx].availability = model->availability;
    models[idx].state = MODEL_AVAILABLE;
    my_strncpy(models[idx].url, model->url, URL_LEN - 1);
    my_strncpy(models[idx].api_key, model->api_key, 63);
    models[idx].total_queries = 0;
    models[idx].failed_queries = 0;
    models[idx].total_latency = 0;

    model_count++;
    int id = next_model_id++;

    print_str("[router] registered model: ");
    print_str(models[idx].name);
    print_str(" (id=");
    print_int(id);
    print_str(", type=");
    print_int(model->type);
    print_str(", quality=");
    print_int(model->quality_score);
    print_str("/10)\n");

    return id;
}

/* Route a query to the best model based on strategy
   prompt: input prompt, strategy: routing strategy
   Returns: model_id selected or -1 */
int ai_router_route_query(const char *prompt, int strategy) {
    if (model_count == 0) {
        print_str("[router] no models registered\n");
        return -1;
    }

    if (!prompt || !prompt[0]) {
        print_str("[router] empty prompt\n");
        return -1;
    }

    int selected = -1;
    int prompt_len = my_strlen(prompt);

    if (strategy == ROUTE_LOCAL_FIRST) {
        /* Prefer local models */
        for (int i = 0; i < model_count; i++) {
            if (models[i].type == MODEL_TYPE_LOCAL &&
                models[i].state == MODEL_AVAILABLE) {
                selected = i;
                break;
            }
        }
        /* Fallback to any available */
        if (selected < 0) {
            for (int i = 0; i < model_count; i++) {
                if (models[i].state == MODEL_AVAILABLE) {
                    selected = i;
                    break;
                }
            }
        }
    } else if (strategy == ROUTE_REMOTE_FIRST) {
        /* Prefer remote (HTTP/WS) models */
        for (int i = 0; i < model_count; i++) {
            if (models[i].type != MODEL_TYPE_LOCAL &&
                models[i].state == MODEL_AVAILABLE) {
                selected = i;
                break;
            }
        }
        if (selected < 0) {
            for (int i = 0; i < model_count; i++) {
                if (models[i].state == MODEL_AVAILABLE) {
                    selected = i;
                    break;
                }
            }
        }
    } else if (strategy == ROUTE_COST_OPTIMIZE) {
        /* Choose cheapest available model with quality >= 5 */
        int best_cost = 999999;
        for (int i = 0; i < model_count; i++) {
            if (models[i].state != MODEL_AVAILABLE) continue;
            if (models[i].quality_score < 5) continue;
            if (models[i].cost_per_token < best_cost) {
                best_cost = models[i].cost_per_token;
                selected = i;
            }
        }
        /* Fallback: cheapest regardless of quality */
        if (selected < 0) {
            for (int i = 0; i < model_count; i++) {
                if (models[i].state != MODEL_AVAILABLE) continue;
                if (models[i].cost_per_token < best_cost) {
                    best_cost = models[i].cost_per_token;
                    selected = i;
                }
            }
        }
    } else if (strategy == ROUTE_QUALITY_FIRST) {
        /* Choose highest quality available */
        int best_quality = 0;
        for (int i = 0; i < model_count; i++) {
            if (models[i].state != MODEL_AVAILABLE) continue;
            if (models[i].quality_score > best_quality) {
                best_quality = models[i].quality_score;
                selected = i;
            }
        }
    } else if (strategy == ROUTE_LOAD_BALANCE) {
        /* Choose model with fewest queries (least loaded) */
        int min_queries = 999999;
        for (int i = 0; i < model_count; i++) {
            if (models[i].state != MODEL_AVAILABLE) continue;
            if (models[i].total_queries < min_queries) {
                min_queries = models[i].total_queries;
                selected = i;
            }
        }
    }

    if (selected >= 0) {
        print_str("[router] routing to: ");
        print_str(models[selected].name);
        print_str(" (strategy=");
        print_int(strategy);
        print_str(", prompt_len=");
        print_int(prompt_len);
        print_str(")\n");
    } else {
        print_str("[router] no available models\n");
    }

    return selected >= 0 ? models[selected].model_id : -1;
}

/* Execute a query on a specific model
   model_id: target model, prompt: input
   response: output buffer
   Returns: 0=success, -1=failure */
int ai_router_execute(int model_id, const char *prompt, char *response) {
    /* Find model */
    int model_idx = -1;
    for (int i = 0; i < model_count; i++) {
        if (models[i].model_id == model_id) {
            model_idx = i;
            break;
        }
    }
    if (model_idx < 0) {
        print_str("[router] model not found: ");
        print_int(model_id);
        print_str("\n");
        return -1;
    }

    if (models[model_idx].state != MODEL_AVAILABLE) {
        print_str("[router] model unavailable: ");
        print_str(models[model_idx].name);
        print_str("\n");
        return -1;
    }

    int success = 0;

    if (models[model_idx].type == MODEL_TYPE_LOCAL) {
        /* Execute via local LLM host function */
        int rc = host_ai_query(prompt, response, RESPONSE_LEN);
        success = (rc == 0);
    } else if (models[model_idx].type == MODEL_TYPE_HTTP) {
        /* Execute via HTTP API */
        char resp_buf[RESPONSE_LEN];
        host_http_request("POST", models[model_idx].url, prompt, resp_buf, RESPONSE_LEN);

        if (resp_buf[0] && resp_buf[0] != '{') {
            my_strncpy(response, resp_buf, RESPONSE_LEN - 1);
            success = 1;
        } else {
            /* Parse response JSON for the actual content */
            my_strncpy(response, resp_buf, RESPONSE_LEN - 1);
            success = 1;
        }
    } else {
        print_str("[router] WebSocket not yet implemented\n");
        success = 0;
    }

    /* Update model stats */
    models[model_idx].total_queries++;
    if (!success) {
        models[model_idx].failed_queries++;
    }

    /* Record history */
    if (history_count < MAX_HISTORY) {
        int hidx = history_count;
        history[hidx].query_id = next_query_id;
        history[hidx].model_used = model_id;
        history[hidx].success = success ? 1 : 0;
        history[hidx].latency_ms = models[model_idx].latency_ms;
        history[hidx].tokens_used = my_strlen(prompt) / 4; /* rough estimate */
        history[hidx].cost = models[model_idx].cost_per_token * history[hidx].tokens_used;
        history_count++;
        next_query_id++;
    }

    if (!success) {
        print_str("[router] execution failed on ");
        print_str(models[model_idx].name);
        print_str("\n");
        return -1;
    }

    return 0;
}

/* Fallback when a model fails
   failed_model: model_id that failed
   Returns: new model_id or -1 */
int ai_router_fallback(int failed_model) {
    print_str("[router] fallback from model ");
    print_int(failed_model);
    print_str("\n");

    /* Find model index */
    int failed_idx = -1;
    for (int i = 0; i < model_count; i++) {
        if (models[i].model_id == failed_model) {
            failed_idx = i;
            break;
        }
    }
    if (failed_idx >= 0) {
        models[failed_idx].state = MODEL_UNAVAILABLE;
    }

    /* Find next best available model by quality */
    int best_quality = 0;
    int best_idx = -1;
    for (int i = 0; i < model_count; i++) {
        if (models[i].state == MODEL_AVAILABLE &&
            models[i].quality_score > best_quality) {
            best_quality = models[i].quality_score;
            best_idx = i;
        }
    }

    if (best_idx >= 0) {
        print_str("[router] fallback to: ");
        print_str(models[best_idx].name);
        print_str(" (quality=");
        print_int(models[best_idx].quality_score);
        print_str(")\n");
        return models[best_idx].model_id;
    }

    print_str("[router] no fallback available\n");
    return -1;
}

/* Print router statistics */
int ai_router_get_stats(void) {
    print_str("=== AI Router Stats ===\n");
    print_str("Models: ");
    print_int(model_count);
    print_str("/");
    print_int(MAX_MODELS);
    print_str("  Queries: ");
    print_int(next_query_id - 1);
    print_str("\nStrategy: ");
    print_int(current_strategy);
    print_str("\n\nModels:\n");

    for (int i = 0; i < model_count; i++) {
        print_str("  ");
        print_str(models[i].name);
        print_str(" [");
        print_str(models[i].state == MODEL_AVAILABLE ? "AVAIL" :
                  models[i].state == MODEL_OVERLOADED ? "BUSY" : "DOWN");
        print_str("] q=");
        print_int(models[i].quality_score);
        print_str(" cost=");
        print_int(models[i].cost_per_token);
        print_str(" lat=");
        print_int(models[i].latency_ms);
        print_str("ms queries=");
        print_int(models[i].total_queries);

        if (models[i].total_queries > 0) {
            int fail_rate = (models[i].failed_queries * 100) / models[i].total_queries;
            print_str(" fail%=");
            print_int(fail_rate);
        }
        print_str("\n");
    }

    if (history_count > 0) {
        print_str("\nRecent queries:\n");
        int start = history_count > 5 ? history_count - 5 : 0;
        for (int i = start; i < history_count; i++) {
            print_str("  Q");
            print_int(history[i].query_id);
            print_str(" model=");
            print_int(history[i].model_used);
            print_str(" tokens=");
            print_int(history[i].tokens_used);
            print_str(" cost=");
            print_int(history[i].cost);
            print_str(history[i].success ? " OK\n" : " FAIL\n");
        }
    }

    return 0;
}

/* Set routing strategy */
void ai_router_set_strategy(int strategy) {
    current_strategy = strategy;
    print_str("[router] strategy set to ");
    print_int(strategy);
    print_str("\n");
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, info = 0, test = 0, stats = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0 || my_strcmp(arg, "--info") == 0) info = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-s") == 0 || my_strcmp(arg, "--stats") == 0) stats = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("AI Router v11.0 - Intelligent Model Routing\n");

    if (help) {
        print_str("Usage: ai_router [options]\n");
        print_str("  -h, --help          Show this help\n");
        print_str("  -i, --info          Show router info\n");
        print_str("  -t, --test          Run routing test\n");
        print_str("  -s, --stats         Show model statistics\n");
        return;
    }
    if (info) {
        print_str("AI Router v11.0\n");
        print_str("Max models: "); print_int(MAX_MODELS); print_str("\n");
        print_str("Strategies: local_first, remote_first, cost, quality, balance\n");
        return;
    }
    if (test) {
        print_str("=== AI Router Test ===\n");
        ai_model_t local;
        local.model_id = 0; my_strcpy(local.name, "local_llm");
        local.type = MODEL_TYPE_LOCAL; local.cost_per_token = 0;
        local.quality_score = 6; local.latency_ms = 50;
        local.availability = 95; local.url[0] = '\0'; local.api_key[0] = '\0';
        int m1 = ai_router_register_model(&local);

        ai_model_t claude;
        claude.model_id = 0; my_strcpy(claude.name, "claude");
        claude.type = MODEL_TYPE_HTTP; claude.cost_per_token = 3;
        claude.quality_score = 9; claude.latency_ms = 1500;
        claude.availability = 99;
        my_strcpy(claude.url, "https://api.anthropic.com/v1/messages");
        my_strcpy(claude.api_key, "sk-ant-xxx");
        int m2 = ai_router_register_model(&claude);

        ai_model_t openai;
        openai.model_id = 0; my_strcpy(openai.name, "openai");
        openai.type = MODEL_TYPE_HTTP; openai.cost_per_token = 2;
        openai.quality_score = 8; openai.latency_ms = 800;
        openai.availability = 98;
        my_strcpy(openai.url, "https://api.openai.com/v1/chat/completions");
        my_strcpy(openai.api_key, "sk-xxx");
        int m3 = ai_router_register_model(&openai);

        ai_model_t gemini;
        gemini.model_id = 0; my_strcpy(gemini.name, "gemini");
        gemini.type = MODEL_TYPE_HTTP; gemini.cost_per_token = 1;
        gemini.quality_score = 8; gemini.latency_ms = 600;
        gemini.availability = 97;
        my_strcpy(gemini.url, "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent");
        my_strcpy(gemini.api_key, "AIza-xxx");
        int m4 = ai_router_register_model(&gemini);

        print_str("\n--- Strategy: Local First ---\n");
        ai_router_set_strategy(ROUTE_LOCAL_FIRST);
        int r1 = ai_router_route_query("Hello, how are you?", ROUTE_LOCAL_FIRST);
        print_str("\n--- Strategy: Quality First ---\n");
        ai_router_set_strategy(ROUTE_QUALITY_FIRST);
        int r2 = ai_router_route_query("Write a complex algorithm", ROUTE_QUALITY_FIRST);
        print_str("\n--- Strategy: Cost Optimize ---\n");
        ai_router_set_strategy(ROUTE_COST_OPTIMIZE);
        int r3 = ai_router_route_query("Simple task", ROUTE_COST_OPTIMIZE);
        print_str("\n--- Strategy: Load Balance ---\n");
        ai_router_set_strategy(ROUTE_LOAD_BALANCE);
        int r4 = ai_router_route_query("Balance this", ROUTE_LOAD_BALANCE);
        print_str("\n--- Fallback Test ---\n");
        ai_router_fallback(m2);
        print_str("\n--- Statistics ---\n");
        ai_router_get_stats();
        return;
    }
    if (stats) {
        ai_router_get_stats();
        return;
    }
    print_str("Use -h for help, -t for test, -s for stats\n");
}
