/* code_examples: WASM code example library (v32.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_EXAMPLES  128
#define MAX_NAME      64
#define MAX_DESC      128
#define MAX_CODE      256
#define MAX_CAT       32

/* Example entry */
typedef struct {
    int   ex_id;
    char  name[MAX_NAME];
    char  category[MAX_CAT];
    char  description[MAX_DESC];
    char  code[MAX_CODE];
    int   runnable;   /* 0=static, 1=runnable */
    int   views;
} example_t;

static example_t examples[MAX_EXAMPLES];
static int ex_count = 0;
static int next_ex_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i] || a[i] != b[i]) return (a[i] ? a[i] : 0) - (b[i] ? b[i] : 0);
    } return 0;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Add code example */
int ce_add(const char *name, const char *category, const char *desc, const char *code, int runnable) {
    if (ex_count >= MAX_EXAMPLES) return -1;
    int idx = ex_count;
    examples[idx].ex_id = next_ex_id++;
    my_strncpy(examples[idx].name, name, MAX_NAME - 1);
    my_strncpy(examples[idx].category, category, MAX_CAT - 1);
    my_strncpy(examples[idx].description, desc, MAX_DESC - 1);
    my_strncpy(examples[idx].code, code, MAX_CODE - 1);
    examples[idx].runnable = runnable;
    examples[idx].views = 0;
    ex_count++;
    return examples[idx].ex_id;
}

/* Search by keyword */
int ce_search(const char *keyword) {
    print_str("  Search: \"");
    print_str(keyword);
    print_str("\"\n");
    int found = 0;
    for (int i = 0; i < ex_count; i++) {
        int name_len = my_strlen(examples[i].name);
        int kw_len = my_strlen(keyword);
        for (int j = 0; j <= name_len - kw_len; j++) {
            if (my_strncmp(&examples[i].name[j], keyword, kw_len) == 0) {
                examples[i].views++;
                print_str("    #");
                print_int(examples[i].ex_id);
                print_str(" ");
                print_str(examples[i].name);
                print_str(" [");
                print_str(examples[i].category);
                print_str("] (");
                print_int(examples[i].views);
                print_str(" views)\n");
                found++;
                break;
            }
        }
    }
    print_str("  Found: ");
    print_int(found);
    print_str(" examples\n");
    return found;
}

/* List by category */
int ce_list_category(const char *category) {
    print_str("  Category: ");
    print_str(category);
    print_str("\n");
    int count = 0;
    for (int i = 0; i < ex_count; i++) {
        if (my_strcmp(examples[i].category, category) == 0) {
            print_str("    #");
            print_int(examples[i].ex_id);
            print_str(" ");
            print_str(examples[i].name);
            if (examples[i].runnable) print_str(" [RUN]");
            print_str("\n");
            print_str("      ");
            print_str(examples[i].description);
            print_str("\n");
            count++;
        }
    }
    return count;
}

/* Show example code */
int ce_show(int ex_id) {
    for (int i = 0; i < ex_count; i++) {
        if (examples[i].ex_id == ex_id) {
            print_str("  Example: ");
            print_str(examples[i].name);
            print_str("\n");
            print_str("  Category: ");
            print_str(examples[i].category);
            print_str("\n");
            print_str("  Description: ");
            print_str(examples[i].description);
            print_str("\n");
            print_str("  Code:\n");
            print_str("    ");
            print_str(examples[i].code);
            print_str("\n");
            examples[i].views++;
            return 0;
        }
    }
    return -1;
}

/* Statistics */
int ce_stats(void) {
    int basic = 0, io = 0, net = 0, ai = 0, adv = 0;
    for (int i = 0; i < ex_count; i++) {
        if (my_strncmp(examples[i].category, "basics", 6) == 0) basic++;
        else if (my_strncmp(examples[i].category, "io", 2) == 0) io++;
        else if (my_strncmp(examples[i].category, "network", 7) == 0) net++;
        else if (my_strncmp(examples[i].category, "ai", 2) == 0) ai++;
        else adv++;
    }
    print_str("  Examples by Category\n");
    print_str("  Basics: "); print_int(basic); print_str("\n");
    print_str("  I/O: "); print_int(io); print_str("\n");
    print_str("  Network: "); print_int(net); print_str("\n");
    print_str("  AI: "); print_int(ai); print_str("\n");
    print_str("  Advanced: "); print_int(adv); print_str("\n");
    int total_views = 0;
    for (int i = 0; i < ex_count; i++) total_views += examples[i].views;
    print_str("  Total views: "); print_int(total_views); print_str("\n");
    return ex_count;
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
    print_str("Code Examples v32.0 - WASM Code Example Library\n");
    if (help) {
        print_str("Usage: code_examples [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run example test\n");
        return;
    }
    if (test) {
        print_str("=== Code Examples Test ===\n\n");
        /* Add examples */
        ce_add("hello_world", "basics", "Minimal WASM module", "void _start() { host_print(\"hello\\n\"); }", 1);
        ce_add("cli_parser", "basics", "Command-line argument parsing", "host_get_argv(buf, 512); /* parse args */", 1);
        ce_add("file_read", "io", "Read file content", "int fd = host_fs_open(name); host_fs_read(fd, buf, len);", 1);
        ce_add("http_get", "network", "HTTP GET request", "host_net_connect(fd, host, port);", 1);
        ce_add("ai_query", "ai", "Query AI model", "host_ai_query(prompt, len);", 1);
        ce_add("pipe_demo", "io", "IPC pipe communication", "host_pipe_create(id, flags);", 1);
        ce_add("agent_demo", "ai", "Create and interact with agent", "host_spawn(\"agent\", argv);", 1);
        ce_add("matrix_ops", "advanced", "Matrix multiplication", "/* AMX tile operations */", 0);
        print_str("  Added: "); print_int(ex_count); print_str(" examples\n\n");
        /* List by category */
        ce_list_category("basics");
        print_str("\n");
        ce_list_category("ai");
        print_str("\n");
        /* Show specific example */
        ce_show(1);
        print_str("\n");
        /* Search */
        ce_search("hello");
        print_str("\n");
        /* Stats */
        ce_stats();
        print_str("\n=== Code Examples Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
