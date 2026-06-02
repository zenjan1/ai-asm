/* embedding_interface: Vector embedding API interface (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_DIM       3072
#define MAX_BATCH     64
#define MAX_TEXT      512
#define MAX_API_KEY   64
#define MAX_MODEL     64

/* Embedding models */
#define EMBED_SMALL   0
#define EMBED_LARGE   1
#define EMBED_CUSTOM  2

/* Configuration */
typedef struct {
    char  api_key[MAX_API_KEY];
    char  model[MAX_MODEL];
    int   dimensions;
    int   encoding_format;  /* 0=float, 1=base64 */
} embed_config_t;

/* Single embedding result */
typedef struct {
    int   index;
    float vector[MAX_DIM];
    int   tokens_used;
} embed_result_t;

/* Batch response */
typedef struct {
    embed_result_t results[MAX_BATCH];
    int   count;
    int   total_tokens;
    int   dimension;
} embed_response_t;

static embed_config_t global_config;
static embed_response_t last_response;
static int request_count = 0;
static int total_vectors = 0;

/* String utilities */
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
static void print_fixed(int val, int decimals) {
    char buf[32]; int pos = 0;
    int integer_part = val;
    int frac = 1;
    for (int i = 0; i < decimals; i++) frac *= 10;
    integer_part = val / frac;
    int remainder = val % frac;
    if (integer_part == 0) buf[pos++] = '0';
    else { int d = 0, t = integer_part; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (integer_part > 0) { buf[pos--] = '0' + (integer_part % 10); integer_part /= 10; } }
    buf[pos++] = '.';
    int frac_pos = pos + decimals - 1;
    buf[frac_pos + 1] = '\0';
    for (int i = 0; i < decimals; i++) {
        buf[frac_pos - i] = '0' + (remainder % 10);
        remainder /= 10;
    }
    host_print(buf);
}

static const char *model_name(int m) {
    if (m == EMBED_SMALL) return "text-embedding-3-small";
    if (m == EMBED_LARGE) return "text-embedding-3-large";
    return "custom-embedding";
}

/* Initialize */
int ei_init(const char *api_key, int model, int dimensions) {
    my_strncpy(global_config.api_key, api_key, MAX_API_KEY - 1);
    my_strncpy(global_config.model, model_name(model), MAX_MODEL - 1);
    global_config.dimensions = dimensions;
    global_config.encoding_format = 0;
    return 0;
}

/* Count tokens (estimate: ~4 chars per token) */
int ei_count_tokens(const char *text) {
    return my_strlen(text) / 4 + 1;
}

/* Embed single text (simulated - generates deterministic pseudo-vector) */
int ei_embed_single(const char *text, embed_result_t *result) {
    request_count++;
    total_vectors++;
    int len = my_strlen(text);
    result->index = 0;
    result->tokens_used = len / 4 + 1;
    /* Generate deterministic pseudo-vector from text hash */
    unsigned int hash = 0;
    for (int i = 0; i < len; i++) hash = hash * 31 + text[i];
    int dim = global_config.dimensions;
    if (dim > MAX_DIM) dim = MAX_DIM;
    for (int i = 0; i < dim; i++) {
        /* Simple hash-based pseudo-random float in [-1, 1] */
        hash = hash * 1103515245 + 12345;
        int raw = (int)(hash >> 16) & 0x7FFF;
        result->vector[i] = (raw / 32767.0f) * 2.0f - 1.0f;
        /* Normalize to unit vector (approximate) */
    }
    /* Normalize */
    float norm = 0;
    for (int i = 0; i < dim; i++) norm += result->vector[i] * result->vector[i];
    norm = norm > 0 ? norm : 1;
    /* Simple sqrt approximation via integer */
    for (int i = 0; i < dim; i++) result->vector[i] /= norm;
    return dim;
}

/* Embed batch */
int ei_embed_batch(const char *texts[], int count, embed_response_t *resp) {
    if (count > MAX_BATCH) count = MAX_BATCH;
    resp->count = 0;
    resp->total_tokens = 0;
    resp->dimension = global_config.dimensions;
    for (int i = 0; i < count; i++) {
        ei_embed_single(texts[i], &resp->results[i]);
        resp->results[i].index = i;
        resp->total_tokens += resp->results[i].tokens_used;
        resp->count++;
    }
    return resp->count;
}

/* Cosine similarity between two vectors */
int ei_similarity(const float *vec1, const float *vec2, int dim) {
    /* Returns similarity * 10000 as integer */
    float dot = 0;
    for (int i = 0; i < dim; i++) dot += vec1[i] * vec2[i];
    return (int)(dot * 10000);
}

/* Find k nearest vectors */
int ei_nearest(const float *query, const float *vectors[], int count, int k, int dim, int *indices) {
    int *scores = (int *)host_alloc(count * 4, 4);
    for (int i = 0; i < count; i++) {
        scores[i] = ei_similarity(query, vectors[i], dim);
    }
    /* Find top k */
    for (int j = 0; j < k && j < count; j++) {
        int best = -1, best_score = -99999;
        for (int i = 0; i < count; i++) {
            /* Check if already selected */
            int used = 0;
            for (int p = 0; p < j; p++) if (indices[p] == i) used = 1;
            if (!used && scores[i] > best_score) { best_score = scores[i]; best = i; }
        }
        if (best >= 0) indices[j] = best;
    }
    return k;
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
    print_str("Embedding Interface v38.0 - Vector Embedding API\n");
    if (help) { print_str("Usage: embedding_interface [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Embedding Interface Test ===\n\n");
        ei_init("sk-embed-test-key", EMBED_LARGE, 3072);
        print_str("  Model: "); print_str(global_config.model); print_str("\n");
        print_str("  Dimensions: "); print_int(global_config.dimensions); print_str("\n\n");
        /* Single embedding */
        embed_result_t single;
        ei_embed_single("The quick brown fox jumps over the lazy dog.", &single);
        print_str("Single embedding:\n");
        print_str("  Tokens: "); print_int(single.tokens_used); print_str("\n");
        print_str("  Vector[0:5]: ");
        for (int i = 0; i < 5; i++) { print_fixed((int)(single.vector[i] * 10000), 4); print_str(" "); }
        print_str("\n\n");
        /* Batch embedding */
        const char *texts[4] = {"Artificial intelligence is transforming the world.",
                                 "Machine learning models require large datasets.",
                                 "Natural language processing enables text understanding.",
                                 "Computer vision interprets visual data."};
        embed_response_t batch;
        ei_embed_batch(texts, 4, &batch);
        print_str("Batch embedding:\n");
        print_str("  Count: "); print_int(batch.count); print_str("\n");
        print_str("  Total tokens: "); print_int(batch.total_tokens); print_str("\n\n");
        /* Similarity */
        print_str("Similarity matrix:\n");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int sim = ei_similarity(batch.results[i].vector, batch.results[j].vector, batch.dimension);
                print_fixed(sim, 4); print_str(" ");
            }
            print_str("\n");
        }
        print_str("\n  Total requests: "); print_int(request_count); print_str("\n");
        print_str("  Total vectors: "); print_int(total_vectors); print_str("\n");
        print_str("\n=== Embedding Interface Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
