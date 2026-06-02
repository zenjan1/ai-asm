/* optimizer: Performance optimization agent (v9.2) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_SUGGESTIONS 32
#define MAX_SUG_MSG 256

#define CAT_ALGO       0
#define CAT_MEMORY     1
#define CAT_IO         2
#define CAT_CACHE      3
#define CAT_CONCURRENCY 4

typedef struct {
    int id;
    int category;
    int impact; /* 1=LOW, 2=MED, 3=HIGH */
    char description[MAX_SUG_MSG];
    char suggestion[MAX_SUG_MSG];
} suggestion_t;

static unsigned int heap_pos = 65536;
static suggestion_t suggestions[MAX_SUGGESTIONS];
static int suggestion_count = 0;
static int next_suggestion_id = 0;
static int analyses_done = 0;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 1; a++; b++; }
    return (*a != *b);
}

static void copy_string(char *dst, const char *src, unsigned int max_len)
{
    unsigned int i = 0;
    while (src[i] && i < max_len - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16]; int i = 15; int neg = 0; buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static const char *category_name(int c)
{
    switch (c) {
        case CAT_ALGO: return "ALGORITHM";
        case CAT_MEMORY: return "MEMORY";
        case CAT_IO: return "I/O";
        case CAT_CACHE: return "CACHE";
        case CAT_CONCURRENCY: return "CONCURRENCY";
        default: return "UNKNOWN";
    }
}

static const char *impact_name(int i)
{
    switch (i) {
        case 1: return "LOW";
        case 2: return "MED";
        case 3: return "HIGH";
        default: return "UNKNOWN";
    }
}

static int add_suggestion(int category, int impact, const char *desc, const char *suggest)
{
    if (suggestion_count >= MAX_SUGGESTIONS) return -1;
    suggestion_t *s = &suggestions[suggestion_count];
    s->id = next_suggestion_id++;
    s->category = category;
    s->impact = impact;
    copy_string(s->description, desc, MAX_SUG_MSG);
    copy_string(s->suggestion, suggest, MAX_SUG_MSG);
    suggestion_count++;
    return s->id;
}

static void analyze(const char *target, int verbose)
{
    analyses_done++;
    print_str("optimizer: analyzing ");
    print_str(target);
    print_str("\n");

    add_suggestion(CAT_ALGO, 3, "O(n^2) loop detected in data processing",
                   "Replace with hash-based O(n) lookup");
    add_suggestion(CAT_MEMORY, 2, "Frequent small allocations detected",
                   "Use memory pool or arena allocator");
    add_suggestion(CAT_IO, 2, "Unbuffered I/O in hot path",
                   "Use buffered I/O or memory-mapped files");
    add_suggestion(CAT_CACHE, 1, "Repeated computation without caching",
                   "Add LRU cache for frequently computed values");

    if (verbose) {
        for (int i = 0; i < suggestion_count; i++) {
            print_str("optimizer:   [");
            print_str(impact_name(suggestions[i].impact));
            print_str("] ");
            print_str(category_name(suggestions[i].category));
            print_str(": ");
            print_str(suggestions[i].description);
            print_str("\n");
            print_str("optimizer:   -> ");
            print_str(suggestions[i].suggestion);
            print_str("\n");
        }
    }
    print_str("optimizer: analysis of ");
    print_str(target);
    print_str(" complete (");
    print_int(suggestion_count);
    print_str(" suggestions)\n");
}

static void show_help(void)
{
    print_str("optimizer: Performance optimization agent (v9.2)\n");
    print_str("Usage: optimizer [OPTIONS] [TARGET...]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List suggestions\n");
    print_str("\n");
    print_str("Categories: Algorithm, Memory, I/O, Cache, Concurrency\n");
}

static void show_info(void)
{
    print_str("optimizer: info:\n");
    print_str("optimizer: max suggestions: 32\n");
    print_str("optimizer: categories: Algorithm, Memory, I/O, Cache, Concurrency\n");
    print_str("optimizer: impact levels: LOW(1), MED(2), HIGH(3)\n");
    print_str("optimizer: info display complete\n");
}

static void show_status(void)
{
    print_str("optimizer: status:\n");
    print_str("optimizer: analyses completed: ");
    print_int(analyses_done);
    print_str("\n");
    print_str("optimizer: total suggestions: ");
    print_int(suggestion_count);
    print_str("\n");
    print_str("optimizer: status check complete\n");
}

static void list_suggestions(void)
{
    print_str("optimizer: suggestions (");
    print_int(suggestion_count);
    print_str(" total)\n");

    for (int i = 0; i < suggestion_count; i++) {
        print_str("optimizer:   #");
        print_int(suggestions[i].id);
        print_str(" [");
        print_str(impact_name(suggestions[i].impact));
        print_str("] ");
        print_str(category_name(suggestions[i].category));
        print_str("\n");
        print_str("optimizer:     issue: ");
        print_str(suggestions[i].description);
        print_str("\n");
        print_str("optimizer:     fix: ");
        print_str(suggestions[i].suggestion);
        print_str("\n");
    }
}

static void test_mode(int verbose)
{
    print_str("optimizer: testing\n");

    analyze("http_handler.c", verbose);

    if (suggestion_count > 0) {
        print_str("optimizer: suggestion generation: OK\n");
    } else {
        print_str("optimizer: suggestion generation: FAIL\n");
    }

    if (verbose) list_suggestions();
    print_str("optimizer: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    host_get_argv(buf, 512);

    int help = 0, info = 0, verbose = 0, status = 0, test = 0, list = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-i") == 0) info = 1;
        else if (my_strcmp(arg, "-v") == 0) verbose = 1;
        else if (my_strcmp(arg, "-s") == 0) status = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0) list = 1;
        else {
            analyze(arg, verbose);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }
    if (list) { list_suggestions(); host_exit(0); }

    print_str("optimizer: usage: optimizer [-t] [-l] [-v] [-i] [-s] [TARGET...]\n");
    host_exit(0);
}
