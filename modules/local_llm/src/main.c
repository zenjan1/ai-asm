/* local_llm: local LLM inference engine (v9.1) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;

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
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

/* Rule-based pattern matching for simplified local inference */
typedef struct {
    const char *pattern;
    const char *response;
} rule_t;

static const rule_t rules[] = {
    {"hello", "Hello! How can I help you today?"},
    {"help", "I can assist with coding tasks, code review, and debugging."},
    {"test", "Test mode activated. Running diagnostic checks..."},
    {"status", "System status: all modules operational."},
    {"version", "AI-ASM v9.1 - local_llm inference engine"},
};

static const int RULE_COUNT = sizeof(rules) / sizeof(rules[0]);

static void show_help(void)
{
    print_str("local_llm: Local LLM inference engine (v9.1)\n");
    print_str("Usage: local_llm [OPTIONS]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -p PROMPT      Query prompt\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Rule-based local inference without external dependencies.\n");
}

static void show_info(void)
{
    print_str("local_llm: inference engine info:\n");
    print_str("local_llm: model type: rule-based\n");
    print_str("local_llm: rule count: 5\n");
    print_str("local_llm: max prompt: 2048 chars\n");
    print_str("local_llm: info display complete\n");
}

static void show_status(void)
{
    print_str("local_llm: status: ready\n");
    print_str("local_llm: mode: local inference\n");
    print_str("local_llm: status check complete\n");
}

static const char *match_rule(const char *input)
{
    for (int i = 0; i < RULE_COUNT; i++) {
        if (my_strcmp(input, rules[i].pattern) == 0)
            return rules[i].response;
    }
    return "local_llm: no matching rule found. Using default response.";
}

static void query(const char *prompt, int verbose)
{
    if (verbose) {
        print_str("local_llm: query: ");
        print_str(prompt);
        print_str("\n");
    }

    const char *response = match_rule(prompt);
    print_str("local_llm: ");
    print_str(response);
    print_str("\n");
}

static void test_mode(int verbose)
{
    print_str("local_llm: testing rule-based inference\n");

    if (verbose) {
        print_str("local_llm: testing hello rule\n");
    }
    const char *resp = match_rule("hello");
    if (my_strcmp(resp, "Hello! How can I help you today?") == 0)
        print_str("local_llm: hello test: OK\n");
    else
        print_str("local_llm: hello test: FAIL\n");

    if (verbose) {
        print_str("local_llm: testing help rule\n");
    }
    resp = match_rule("help");
    if (my_strncmp(resp, "I can assist", 12) == 0)
        print_str("local_llm: help test: OK\n");
    else
        print_str("local_llm: help test: FAIL\n");

    if (verbose) {
        print_str("local_llm: testing unknown rule\n");
    }
    resp = match_rule("unknown");
    if (my_strncmp(resp, "local_llm: no matching", 20) == 0)
        print_str("local_llm: unknown test: OK\n");
    else
        print_str("local_llm: unknown test: FAIL\n");

    print_str("local_llm: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    const char *prompt = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];

        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-p") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512) prompt = &argv_ptr[pos];
        }

        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) { show_help(); host_exit(0); }
    if (info_flag) { show_info(); host_exit(0); }
    if (status_flag) { show_status(); host_exit(0); }
    if (test_flag) { test_mode(verbose_flag); host_exit(0); }

    if (prompt) {
        query(prompt, verbose_flag);
    } else {
        print_str("local_llm: ready\n");
    }

    host_exit(0);
}
