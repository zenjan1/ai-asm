/* vector_store: In-memory vector database (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_DIM       3072
#define MAX_ENTRIES   10000
#define MAX_METADATA  256
#define MAX_SEARCH_K  100

/* Vector entry */
typedef struct {
    int   id;
    float vector[MAX_DIM];
    char  metadata[MAX_METADATA];
    int   created_at;
    int   updated_at;
    int   deleted;
} vs_entry_t;

/* Vector store */
typedef struct {
    vs_entry_t entries[MAX_ENTRIES];
    int   count;
    int   dimension;
    int   index_type;       /* 0=linear, 1=HNSW (reserved) */
    int   build_status;     /* 0=not built, 1=built */
} vs_index_t;

/* Search result */
typedef struct {
    int   entry_id;
    int   similarity;       /* similarity * 10000 */
    char  metadata[MAX_METADATA];
    int   rank;
} vs_search_result_t;

static vs_index_t global_store;
static int next_id = 1;

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
    int integer_part = val, frac = 1;
    for (int i = 0; i < decimals; i++) frac *= 10;
    integer_part = val / frac;
    int remainder = val % frac;
    if (integer_part == 0) buf[pos++] = '0';
    else { int dd = 0, t = integer_part; while (t > 0) { dd++; t /= 10; } pos += dd; buf[pos] = '\0'; pos--;
        while (integer_part > 0) { buf[pos--] = '0' + (integer_part % 10); integer_part /= 10; } }
    buf[pos++] = '.';
    int fp = pos + decimals - 1;
    buf[fp + 1] = '\0';
    for (int i = 0; i < decimals; i++) { buf[fp - i] = '0' + (remainder % 10); remainder /= 10; }
    host_print(buf);
}

/* Initialize store */
int vs_init(int dimension, int max_entries) {
    global_store.count = 0;
    global_store.dimension = dimension > MAX_DIM ? MAX_DIM : dimension;
    global_store.index_type = 0;
    global_store.build_status = 0;
    next_id = 1;
    return 0;
}

/* Insert single vector */
int vs_insert(const float *vector, const char *metadata) {
    if (global_store.count >= MAX_ENTRIES) return -1;
    int idx = global_store.count;
    global_store.entries[idx].id = next_id++;
    int dim = global_store.dimension;
    for (int i = 0; i < dim; i++) global_store.entries[idx].vector[i] = vector[i];
    my_strncpy(global_store.entries[idx].metadata, metadata, MAX_METADATA - 1);
    global_store.entries[idx].created_at = global_store.count + 1;
    global_store.entries[idx].updated_at = global_store.count + 1;
    global_store.entries[idx].deleted = 0;
    global_store.count++;
    return global_store.entries[idx].id;
}

/* Batch insert */
int vs_batch_insert(const float *vectors[], const char *metadatas[], int count) {
    int inserted = 0;
    for (int i = 0; i < count && global_store.count < MAX_ENTRIES; i++) {
        vs_insert(vectors[i], metadatas[i]);
        inserted++;
    }
    return inserted;
}

/* Search by similarity */
int vs_search(const float *query_vec, int k, int threshold, vs_search_result_t *results) {
    int dim = global_store.dimension;
    int *scores = (int *)host_alloc(global_store.count * 4, 4);
    int *active = (int *)host_alloc(global_store.count * 4, 4);
    int active_count = 0;
    for (int i = 0; i < global_store.count; i++) {
        if (global_store.entries[i].deleted) continue;
        /* Cosine similarity * 10000 */
        float dot = 0;
        for (int j = 0; j < dim; j++) dot += query_vec[j] * global_store.entries[i].vector[j];
        scores[i] = (int)(dot * 10000);
        active[active_count++] = i;
    }
    /* Sort by score (bubble for top k) */
    int result_count = 0;
    for (int j = 0; j < k && j < active_count; j++) {
        int best = -1, best_score = threshold - 10001;
        for (int a = 0; a < active_count; a++) {
            int idx = active[a];
            /* Check if already used */
            int used = 0;
            for (int r = 0; r < result_count; r++) if (results[r].entry_id == global_store.entries[idx].id) used = 1;
            if (!used && scores[idx] > best_score) { best_score = scores[idx]; best = idx; }
        }
        if (best >= 0) {
            results[result_count].entry_id = global_store.entries[best].id;
            results[result_count].similarity = scores[best];
            my_strncpy(results[result_count].metadata, global_store.entries[best].metadata, MAX_METADATA - 1);
            results[result_count].rank = result_count + 1;
            result_count++;
        }
    }
    return result_count;
}

/* Delete entry */
int vs_delete(int entry_id) {
    for (int i = 0; i < global_store.count; i++) {
        if (global_store.entries[i].id == entry_id) {
            global_store.entries[i].deleted = 1;
            return 0;
        }
    }
    return -1;
}

/* Get entry */
int vs_get(int entry_id, vs_entry_t *entry) {
    for (int i = 0; i < global_store.count; i++) {
        if (global_store.entries[i].id == entry_id && !global_store.entries[i].deleted) {
            entry->id = global_store.entries[i].id;
            int dim = global_store.dimension;
            for (int j = 0; j < dim; j++) entry->vector[j] = global_store.entries[i].vector[j];
            my_strncpy(entry->metadata, global_store.entries[i].metadata, MAX_METADATA - 1);
            return 0;
        }
    }
    return -1;
}

/* Update entry */
int vs_update(int entry_id, const float *vector, const char *metadata) {
    for (int i = 0; i < global_store.count; i++) {
        if (global_store.entries[i].id == entry_id && !global_store.entries[i].deleted) {
            int dim = global_store.dimension;
            for (int j = 0; j < dim; j++) global_store.entries[i].vector[j] = vector[j];
            if (metadata) my_strncpy(global_store.entries[i].metadata, metadata, MAX_METADATA - 1);
            global_store.entries[i].updated_at = global_store.count + 1;
            return 0;
        }
    }
    return -1;
}

/* Count active entries */
int vs_count(void) {
    int c = 0;
    for (int i = 0; i < global_store.count; i++) if (!global_store.entries[i].deleted) c++;
    return c;
}

/* Statistics */
int vs_stats(void) {
    print_str("  Store: dimension=");
    print_int(global_store.dimension);
    print_str(", total=");
    print_int(global_store.count);
    print_str(", active=");
    print_int(vs_count());
    print_str(", index=");
    print_str(global_store.index_type == 0 ? "linear" : "hnsw");
    print_str("\n");
    return global_store.count;
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
    print_str("Vector Store v38.0 - In-Memory Vector Database\n");
    if (help) { print_str("Usage: vector_store [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Vector Store Test ===\n\n");
        vs_init(1536, 10000);
        vs_stats();
        print_str("\n");
        /* Insert vectors */
        float v1[1536] = {0}, v2[1536] = {0}, v3[1536] = {0};
        v1[0] = 0.5; v1[1] = 0.3; v1[2] = -0.2;
        v2[0] = 0.51; v2[1] = 0.29; v2[2] = -0.21;
        v3[0] = -0.5; v3[1] = -0.3; v3[2] = 0.2;
        int id1 = vs_insert(v1, "doc:AI introduction");
        int id2 = vs_insert(v2, "doc:Machine learning basics");
        int id3 = vs_insert(v3, "doc:History of computing");
        print_str("  Inserted: "); print_int(global_store.count); print_str(" vectors\n\n");
        /* Search */
        vs_search_result_t results[MAX_SEARCH_K];
        float query[1536] = {0};
        query[0] = 0.49; query[1] = 0.31; query[2] = -0.19;
        int found = vs_search(query, 5, -10000, results);
        print_str("Search results (k=5):\n");
        for (int i = 0; i < found; i++) {
            print_str("  #"); print_int(results[i].rank);
            print_str(" ID="); print_int(results[i].entry_id);
            print_str(" sim="); print_fixed(results[i].similarity, 4);
            print_str(" "); print_str(results[i].metadata);
            print_str("\n");
        }
        print_str("\n");
        /* Delete and count */
        vs_delete(id3);
        print_str("After delete:\n");
        vs_stats();
        print_str("\n=== Vector Store Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
