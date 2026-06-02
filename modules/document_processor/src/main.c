/* document_processor: Text chunking and processing (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_CHUNKS    512
#define MAX_CHUNK     512
#define MAX_SOURCE    128
#define MAX_META      256
#define MAX_TEXT      8192

/* Chunk strategies */
#define STRATEGY_FIXED       0
#define STRATEGY_RECURSIVE   1
#define STRATEGY_SEMANTIC    2
#define STRATEGY_MARKDOWN    3

/* Chunk */
typedef struct {
    int   chunk_id;
    char  content[MAX_CHUNK];
    int   start_pos;
    int   end_pos;
    int   overlap_prev;
    int   overlap_next;
    char  metadata[MAX_META];
} doc_chunk_t;

/* Config */
typedef struct {
    int   chunk_size;
    int   overlap;
    int   strategy;
    char  separator;
} doc_config_t;

/* Processed document */
typedef struct {
    char  source[MAX_SOURCE];
    doc_chunk_t chunks[MAX_CHUNKS];
    int   total_chunks;
    int   total_chars;
    int   avg_chunk_size;
} processed_doc_t;

static doc_chunk_t chunks[MAX_CHUNKS];
static int chunk_count = 0;
static int next_chunk_id = 1;

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

static const char *strategy_name(int s) {
    if (s == STRATEGY_FIXED) return "Fixed-size";
    if (s == STRATEGY_RECURSIVE) return "Recursive";
    if (s == STRATEGY_SEMANTIC) return "Semantic";
    return "Markdown";
}

/* Fixed-size chunking */
int dp_chunk_fixed(const char *text, int size, int overlap) {
    int len = my_strlen(text);
    chunk_count = 0;
    int pos = 0;
    while (pos < len && chunk_count < MAX_CHUNKS) {
        int idx = chunk_count;
        chunks[idx].chunk_id = next_chunk_id++;
        chunks[idx].start_pos = pos;
        int end = pos + size;
        if (end > len) end = len;
        /* Try to break at word boundary */
        if (end < len) {
            while (end > pos && text[end] != ' ') end--;
            if (end == pos) end = pos + size; /* force break */
        }
        int chunk_len = end - pos;
        if (chunk_len > MAX_CHUNK - 1) chunk_len = MAX_CHUNK - 1;
        for (int i = 0; i < chunk_len; i++) chunks[idx].content[i] = text[pos + i];
        chunks[idx].content[chunk_len] = '\0';
        chunks[idx].end_pos = end;
        chunks[idx].overlap_prev = (pos > 0 && chunk_count > 0) ? overlap : 0;
        chunks[idx].metadata[0] = '\0';
        if (chunk_count > 0) {
            chunks[chunk_count - 1].overlap_next = overlap;
        }
        pos = end;
        /* Overlap: step back */
        if (overlap > 0 && pos > 0) {
            int back = pos - overlap;
            if (back < 0) back = 0;
            /* Skip to next word */
            while (back < pos && text[back] != ' ') back++;
            pos = back;
        }
        chunk_count++;
    }
    return chunk_count;
}

/* Recursive character chunking */
int dp_chunk_recursive(const char *text, int chunk_size) {
    /* Use fixed chunking with paragraph/sentence/newline separators */
    return dp_chunk_fixed(text, chunk_size, chunk_size / 4);
}

/* Markdown chunking (by heading) */
int dp_chunk_markdown(const char *md_text) {
    int len = my_strlen(md_text);
    chunk_count = 0;
    int pos = 0, start = 0;
    while (pos < len && chunk_count < MAX_CHUNKS) {
        if (md_text[pos] == '#' && (pos == 0 || md_text[pos - 1] == '\n')) {
            /* Found heading - chunk previous content */
            if (pos > start && chunk_count < MAX_CHUNKS) {
                int idx = chunk_count;
                chunks[idx].chunk_id = next_chunk_id++;
                int chunk_len = pos - start;
                if (chunk_len >= MAX_CHUNK) chunk_len = MAX_CHUNK - 1;
                for (int i = 0; i < chunk_len; i++) chunks[idx].content[i] = md_text[start + i];
                chunks[idx].content[chunk_len] = '\0';
                chunks[idx].start_pos = start;
                chunks[idx].end_pos = pos;
                chunks[idx].overlap_prev = 0;
                chunks[idx].overlap_next = 0;
                chunks[idx].metadata[0] = '\0';
                chunk_count++;
            }
            start = pos;
            /* Skip heading line */
            while (pos < len && md_text[pos] != '\n') pos++;
            start = pos + 1;
        }
        pos++;
    }
    /* Last chunk */
    if (start < len && chunk_count < MAX_CHUNKS) {
        int idx = chunk_count;
        chunks[idx].chunk_id = next_chunk_id++;
        int chunk_len = len - start;
        if (chunk_len >= MAX_CHUNK) chunk_len = MAX_CHUNK - 1;
        for (int i = 0; i < chunk_len; i++) chunks[idx].content[i] = md_text[start + i];
        chunks[idx].content[chunk_len] = '\0';
        chunks[idx].start_pos = start;
        chunks[idx].end_pos = len;
        chunks[idx].overlap_prev = 0;
        chunks[idx].overlap_next = 0;
        chunks[idx].metadata[0] = '\0';
        chunk_count++;
    }
    return chunk_count;
}

/* Extract metadata (simulated) */
int dp_extract_metadata(const char *text, char *title, char *author, int max_len) {
    /* Simple: use first line as title */
    int pos = 0;
    while (text[pos] && text[pos] != '\n' && pos < max_len - 1) {
        title[pos] = text[pos]; pos++;
    }
    title[pos] = '\0';
    my_strncpy(author, "unknown", max_len - 1);
    return 0;
}

/* Get chunk statistics */
int dp_get_chunk_stats(void) {
    if (chunk_count == 0) return 0;
    int total_size = 0, min_size = 999999, max_size = 0;
    for (int i = 0; i < chunk_count; i++) {
        int s = my_strlen(chunks[i].content);
        total_size += s;
        if (s < min_size) min_size = s;
        if (s > max_size) max_size = s;
    }
    print_str("  Chunks: "); print_int(chunk_count); print_str("\n");
    print_str("  Avg size: "); print_int(total_size / chunk_count); print_str("\n");
    print_str("  Min size: "); print_int(min_size); print_str("\n");
    print_str("  Max size: "); print_int(max_size); print_str("\n");
    return chunk_count;
}

/* List chunks */
int dp_list_chunks(void) {
    for (int i = 0; i < chunk_count; i++) {
        print_str("  Chunk #");
        print_int(chunks[i].chunk_id);
        print_str(" [");
        print_int(chunks[i].start_pos);
        print_str("-");
        print_int(chunks[i].end_pos);
        print_str("]: ");
        print_str(chunks[i].content);
        if (my_strlen(chunks[i].content) > 60) print_str("...");
        print_str("\n");
    }
    return chunk_count;
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
    print_str("Document Processor v38.0 - Text Chunking & Processing\n");
    if (help) { print_str("Usage: document_processor [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Document Processor Test ===\n\n");
        const char *text = "Artificial intelligence is transforming the world. Machine learning models require large datasets for training. Natural language processing enables computers to understand human text. Computer vision interprets visual data from images and videos. Deep learning has achieved remarkable results in many domains. Neural networks can now generate human-like text, recognize objects in images, and even compose music. The future of AI is promising with continuous breakthroughs.";
        /* Fixed-size chunking */
        print_str("Fixed-size chunking (size=128, overlap=32):\n");
        dp_chunk_fixed(text, 128, 32);
        dp_list_chunks();
        dp_get_chunk_stats();
        print_str("\n");
        /* Recursive chunking */
        print_str("Recursive chunking (size=150):\n");
        dp_chunk_recursive(text, 150);
        dp_list_chunks();
        dp_get_chunk_stats();
        print_str("\n");
        /* Markdown chunking */
        const char *md = "# Introduction\nAI is changing the world.\n\n# Methods\nMachine learning is one approach.\nDeep learning is another.\n\n# Conclusion\nThe future is bright.";
        print_str("Markdown chunking:\n");
        dp_chunk_markdown(md);
        dp_list_chunks();
        print_str("\n=== Document Processor Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
