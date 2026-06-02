/* reranker: Search result reranking (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_RESULTS   100
#define MAX_TEXT      512
#define MAX_QUERY     256

/* Rerank input */
typedef struct {
    int   id;
    char  text[MAX_TEXT];
    int   initial_score;  /* * 10000 */
} rerank_input_t;

/* Rerank result */
typedef struct {
    int   id;
    int   relevance_score;  /* * 10000 */
    int   rank;
    int   diversity_score;  /* * 10000 */
} rerank_result_t;

static rerank_input_t inputs[MAX_RESULTS];
static rerank_result_t results[MAX_RESULTS];
static int input_count = 0;

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
static void print_fixed(int val, int dec) {
    char buf[32]; int pos = 0, frac = 1;
    for (int i = 0; i < dec; i++) frac *= 10;
    int ip = val / frac, rem = val % frac;
    if (ip == 0) buf[pos++] = '0';
    else { int dd = 0, t = ip; while (t > 0) { dd++; t /= 10; } pos += dd; buf[pos] = '\0'; pos--;
        while (ip > 0) { buf[pos--] = '0' + (ip % 10); ip /= 10; } }
    buf[pos++] = '.';
    int fp = pos + dec - 1; buf[fp + 1] = '\0';
    for (int i = 0; i < dec; i++) { buf[fp - i] = '0' + (rem % 10); rem /= 10; }
    host_print(buf);
}

/* Score relevance (keyword overlap) */
int rr_score_relevance(const char *query, const char *doc) {
    int score = 0;
    int qlen = my_strlen(query);
    for (int w = 0; w < qlen; w++) {
        if (query[w] == ' ') continue;
        char word[32]; int wl = 0;
        while (w + wl < qlen && query[w + wl] != ' ' && wl < 31) { word[wl] = query[w + wl]; wl++; }
        word[wl] = '\0';
        if (wl > 2 && my_strstr(doc, word)) score += 15;
        w += wl;
    }
    return score * 100; /* scale to * 10000 range */
}

/* Rerank results */
int rr_rerank(const char *query, rerank_input_t *in, int count, rerank_result_t *out, int top_n) {
    /* Score each document */
    for (int i = 0; i < count; i++) {
        results[i].id = in[i].id;
        results[i].relevance_score = rr_score_relevance(query, in[i].text);
        results[i].rank = i + 1;
        results[i].diversity_score = 0;
    }
    /* Sort by relevance score (simple selection sort for top_n) */
    int n = top_n < count ? top_n : count;
    for (int j = 0; j < n; j++) {
        int best = j;
        for (int i = j + 1; i < count; i++) {
            if (results[i].relevance_score > results[best].relevance_score) best = i;
        }
        if (best != j) {
            rerank_result_t tmp = results[j];
            results[j] = results[best];
            results[best] = tmp;
        }
    }
    /* Assign diversity scores (penalize similar top results) */
    for (int j = 0; j < n; j++) {
        int diversity = 10000; /* 1.0 */
        for (int k = 0; k < j; k++) {
            /* Check text overlap with higher-ranked results */
            int overlap = 0;
            int sl = my_strlen(in[results[j].id].text);
            if (sl > 0) {
                int len = sl < 100 ? sl : 100;
                for (int c = 0; c < len; c++) {
                    if (in[results[j].id].text[c] == in[results[k].id].text[c]) overlap++;
                }
            }
            diversity -= overlap / 5;
        }
        if (diversity < 0) diversity = 0;
        results[j].diversity_score = diversity;
        results[j].rank = j + 1;
    }
    return n;
}

/* Deduplicate */
int rr_deduplicate(rerank_input_t *in, int count, int threshold) {
    int kept = 0;
    for (int i = 0; i < count; i++) {
        int dup = 0;
        for (int j = 0; j < i; j++) {
            /* Simple overlap check */
            int overlap = 0;
            int li = my_strlen(in[i].text), lj = my_strlen(in[j].text);
            int ml = li < lj ? li : lj;
            if (ml > 100) ml = 100;
            for (int c = 0; c < ml; c++) {
                if (in[i].text[c] == in[j].text[c]) overlap++;
            }
            if (overlap * 100 / ml > threshold) dup = 1;
        }
        if (!dup) kept++;
    }
    return kept;
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
    print_str("Reranker v38.0 - Search Result Reranking\n");
    if (help) { print_str("Usage: reranker [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Reranker Test ===\n\n");
        /* Setup */
        rerank_input_t docs[6];
        int dc = 0;
        docs[dc].id = dc; docs[dc].initial_score = 5000;
        my_strncpy(docs[dc].text, "Machine learning algorithms improve through experience with data.", MAX_TEXT - 1); dc++;
        docs[dc].id = dc; docs[dc].initial_score = 4500;
        my_strncpy(docs[dc].text, "Python is a popular programming language for web development.", MAX_TEXT - 1); dc++;
        docs[dc].id = dc; docs[dc].initial_score = 7000;
        my_strncpy(docs[dc].text, "Deep learning is a subset of machine learning using neural networks.", MAX_TEXT - 1); dc++;
        docs[dc].id = dc; docs[dc].initial_score = 3000;
        my_strncpy(docs[dc].text, "The weather today is sunny with temperatures reaching 30 degrees.", MAX_TEXT - 1); dc++;
        docs[dc].id = dc; docs[dc].initial_score = 6000;
        my_strncpy(docs[dc].text, "Neural networks are inspired by biological brain structures in machine learning.", MAX_TEXT - 1); dc++;
        docs[dc].id = dc; docs[dc].initial_score = 5500;
        my_strncpy(docs[dc].text, "Machine learning models require training data to make predictions.", MAX_TEXT - 1); dc++;
        print_str("Initial documents: "); print_int(dc); print_str("\n\n");
        /* Rerank */
        const char *query = "machine learning neural networks";
        print_str("Query: \""); print_str(query); print_str("\"\n\n");
        int n = rr_rerank(query, docs, dc, results, 5);
        print_str("Reranked results (top "); print_int(n); print_str("):\n");
        for (int i = 0; i < n; i++) {
            print_str("  #"); print_int(results[i].rank);
            print_str(" ID="); print_int(results[i].id);
            print_str(" relevance="); print_fixed(results[i].relevance_score, 4);
            print_str(" diversity="); print_fixed(results[i].diversity_score, 4);
            print_str("\n    "); print_str(docs[results[i].id].text); print_str("\n");
        }
        print_str("\n=== Reranker Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
