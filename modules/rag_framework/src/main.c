/* rag_framework: Retrieval-Augmented Generation (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_INDICES     16
#define MAX_CHUNKS      512
#define MAX_CHUNK_TEXT  512
#define MAX_ANSWER      2048
#define MAX_QUERY       1024
#define MAX_NAME        64
#define MAX_CONTEXT     4096
#define DIM             1536

/* RAG index */
typedef struct {
    int   index_id;
    char  name[MAX_NAME];
    int   total_docs;
    int   total_chunks;
    int   status;  /* 0=empty, 1=indexed */
    char  docs[MAX_CHUNKS][MAX_CHUNK_TEXT];
    int   doc_count;
} rag_index_t;

/* Query */
typedef struct {
    char  question[MAX_QUERY];
    int   top_k;
    int   threshold;    /* * 10000 */
    int   include_sources;
} rag_query_t;

/* Context */
typedef struct {
    char  texts[MAX_CHUNKS][MAX_CHUNK_TEXT];
    int   count;
    int   total_tokens;
    int   avg_similarity;
} rag_context_t;

/* Response */
typedef struct {
    char  answer[MAX_ANSWER];
    char  sources[MAX_CHUNKS][MAX_CHUNK_TEXT];
    int   source_count;
    int   confidence;    /* 0-100 */
    int   latency_ms;
    int   tokens_used;
} rag_response_t;

static rag_index_t indices[MAX_INDICES];
static int index_count = 0;
static int next_index_id = 1;

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

/* Create index */
int rag_create_index(const char *name) {
    if (index_count >= MAX_INDICES) return -1;
    int idx = index_count;
    indices[idx].index_id = next_index_id++;
    my_strncpy(indices[idx].name, name, MAX_NAME - 1);
    indices[idx].total_docs = 0;
    indices[idx].total_chunks = 0;
    indices[idx].status = 0;
    indices[idx].doc_count = 0;
    index_count++;
    return indices[idx].index_id;
}

/* Index document */
int rag_index_document(int index_id, const char *doc_text) {
    for (int i = 0; i < index_count; i++) {
        if (indices[i].index_id == index_id) {
            if (indices[i].doc_count >= MAX_CHUNKS) return -1;
            int dc = indices[i].doc_count;
            my_strncpy(indices[i].docs[dc], doc_text, MAX_CHUNK_TEXT - 1);
            indices[i].doc_count++;
            indices[i].total_docs++;
            indices[i].total_chunks++;
            indices[i].status = 1;
            return dc;
        }
    }
    return -1;
}

/* Build context from query (simulated retrieval) */
int rag_build_context(int index_id, const char *question, rag_context_t *ctx) {
    for (int i = 0; i < index_count; i++) {
        if (indices[i].index_id == index_id && indices[i].status == 1) {
            ctx->count = 0;
            ctx->total_tokens = 0;
            int total_sim = 0;
            /* Simple keyword matching */
            for (int d = 0; d < indices[i].doc_count && ctx->count < MAX_CHUNKS; d++) {
                /* Check if document contains keywords from question */
                int relevance = 0;
                int qlen = my_strlen(question);
                /* Word-by-word matching */
                for (int w = 0; w < qlen; w++) {
                    if (question[w] == ' ') continue;
                    char word[32]; int wl = 0;
                    while (w + wl < qlen && question[w + wl] != ' ' && wl < 31) {
                        word[wl] = question[w + wl]; wl++;
                    }
                    word[wl] = '\0';
                    if (wl > 2 && my_strstr(indices[i].docs[d], word)) relevance += 10;
                    w += wl;
                }
                if (relevance > 0) {
                    my_strncpy(ctx->texts[ctx->count], indices[i].docs[d], MAX_CHUNK_TEXT - 1);
                    ctx->count++;
                    ctx->total_tokens += my_strlen(indices[i].docs[d]) / 4 + 1;
                    total_sim += relevance;
                }
            }
            ctx->avg_similarity = ctx->count > 0 ? total_sim / ctx->count : 0;
            return ctx->count;
        }
    }
    return -1;
}

/* Generate answer (simulated) */
int rag_generate_answer(rag_context_t *ctx, const char *question, rag_response_t *resp) {
    /* Build answer from context */
    my_strncpy(resp->answer, "Based on the retrieved context, ", MAX_ANSWER - 1);
    int len = my_strlen(resp->answer);
    my_strncpy(&resp->answer[len], question, MAX_ANSWER - len - 100);
    len = my_strlen(resp->answer);
    my_strncpy(&resp->answer[len], ". The retrieved documents provide relevant information to answer this question.", MAX_ANSWER - len - 1);
    resp->source_count = ctx->count;
    for (int i = 0; i < ctx->count && i < MAX_CHUNKS; i++) {
        my_strncpy(resp->sources[i], ctx->texts[i], MAX_CHUNK_TEXT - 1);
    }
    resp->confidence = ctx->avg_similarity > 20 ? 85 : ctx->avg_similarity > 10 ? 65 : 40;
    resp->latency_ms = 150 + ctx->total_tokens / 10;
    resp->tokens_used = ctx->total_tokens + 200;
    return 0;
}

/* Full query pipeline */
int rag_query(int index_id, rag_query_t *query, rag_response_t *resp) {
    rag_context_t ctx;
    rag_build_context(index_id, query->question, &ctx);
    rag_generate_answer(&ctx, query->question, resp);
    return resp->source_count;
}

/* List indices */
int rag_list_indices(void) {
    print_str("  RAG Indices\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < index_count; i++) {
        print_str("  #"); print_int(indices[i].index_id);
        print_str(" "); print_str(indices[i].name);
        print_str(" - "); print_int(indices[i].total_docs);
        print_str(" docs, "); print_int(indices[i].total_chunks);
        print_str(" chunks [");
        print_str(indices[i].status == 1 ? "indexed" : "empty");
        print_str("]\n");
    }
    return index_count;
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
    print_str("RAG Framework v38.0 - Retrieval-Augmented Generation\n");
    if (help) { print_str("Usage: rag_framework [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== RAG Framework Test ===\n\n");
        int idx = rag_create_index("AI_knowledge_base");
        print_str("Index created: #"); print_int(idx); print_str("\n\n");
        /* Index documents */
        rag_index_document(idx, "Machine learning is a subset of artificial intelligence that focuses on training algorithms from data.");
        rag_index_document(idx, "Natural language processing enables computers to understand and generate human language.");
        rag_index_document(idx, "Deep learning uses neural networks with multiple layers to learn hierarchical representations.");
        rag_index_document(idx, "Computer vision allows AI systems to interpret and analyze visual information from images.");
        rag_index_document(idx, "Reinforcement learning trains agents to make decisions through trial and error rewards.");
        print_str("Indexed "); print_int(indices[0].doc_count); print_str(" documents\n\n");
        rag_list_indices();
        print_str("\n");
        /* Query */
        rag_query_t q;
        my_strncpy(q.question, "What is machine learning and how does it work?", MAX_QUERY - 1);
        q.top_k = 3; q.threshold = 5000; q.include_sources = 1;
        rag_response_t resp;
        print_str("Query: \""); print_str(q.question); print_str("\"\n\n");
        rag_query(idx, &q, &resp);
        print_str("Answer: "); print_str(resp.answer); print_str("\n");
        print_str("Confidence: "); print_int(resp.confidence); print_str("%\n");
        print_str("Sources: "); print_int(resp.source_count); print_str("\n");
        print_str("Tokens: "); print_int(resp.tokens_used); print_str("\n");
        print_str("Latency: "); print_int(resp.latency_ms); print_str("ms\n");
        print_str("\n=== RAG Framework Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
