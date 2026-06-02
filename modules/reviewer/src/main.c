/* reviewer: Code review agent (v9.1) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ISSUES 64
#define MAX_ISSUE_MSG 128

typedef struct {
    int id;
    int severity; /* 0=info, 1=warning, 2=error, 3=critical */
    char message[MAX_ISSUE_MSG];
    int line;
} issue_t;

static unsigned int heap_pos = 65536;
static issue_t issues[MAX_ISSUES];
static int issue_count = 0;
static int next_issue_id = 0;
static int files_reviewed = 0;

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

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
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

static const char *severity_name(int s)
{
    switch (s) {
        case 0: return "INFO";
        case 1: return "WARNING";
        case 2: return "ERROR";
        case 3: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

static int add_issue(int severity, const char *msg, int line)
{
    if (issue_count >= MAX_ISSUES) return -1;
    issue_t *iss = &issues[issue_count];
    iss->id = next_issue_id++;
    iss->severity = severity;
    iss->line = line;
    copy_string(iss->message, msg, MAX_ISSUE_MSG);
    issue_count++;
    return iss->id;
}

static void run_review(const char *file, int verbose)
{
    files_reviewed++;
    print_str("reviewer: reviewing ");
    print_str(file);
    print_str("\n");

    /* Simulated checks */
    add_issue(2, "missing null check on pointer dereference", 42);
    add_issue(1, "function exceeds 50 lines", 100);
    add_issue(0, "consider extracting repeated logic", 75);

    if (verbose) {
        for (int i = 0; i < issue_count; i++) {
            print_str("reviewer:   [");
            print_str(severity_name(issues[i].severity));
            print_str("] line ");
            print_int(issues[i].line);
            print_str(": ");
            print_str(issues[i].message);
            print_str("\n");
        }
    }

    print_str("reviewer: review complete for ");
    print_str(file);
    print_str("\n");
}

static void show_help(void)
{
    print_str("reviewer: Code review agent (v9.1)\n");
    print_str("Usage: reviewer [OPTIONS] [FILE...]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List issues\n");
}

static void show_info(void)
{
    print_str("reviewer: info:\n");
    print_str("reviewer: max issues: 64\n");
    print_str("reviewer: severity levels: info, warning, error, critical\n");
    print_str("reviewer: info display complete\n");
}

static void show_status(void)
{
    print_str("reviewer: status:\n");
    print_str("reviewer: files reviewed: ");
    print_int(files_reviewed);
    print_str("\n");
    print_str("reviewer: total issues: ");
    print_int(issue_count);
    print_str("\n");
    print_str("reviewer: status check complete\n");
}

static void list_issues(void)
{
    int errors = 0, warnings = 0, criticals = 0, infos = 0;
    for (int i = 0; i < issue_count; i++) {
        switch (issues[i].severity) {
            case 0: infos++; break;
            case 1: warnings++; break;
            case 2: errors++; break;
            case 3: criticals++; break;
        }
    }
    print_str("reviewer: issues (");
    print_int(issue_count);
    print_str(" total: ");
    print_int(criticals);
    print_str(" critical, ");
    print_int(errors);
    print_str(" errors, ");
    print_int(warnings);
    print_str(" warnings, ");
    print_int(infos);
    print_str(" info)\n");

    for (int i = 0; i < issue_count; i++) {
        print_str("reviewer:   [");
        print_str(severity_name(issues[i].severity));
        print_str("] line ");
        print_int(issues[i].line);
        print_str(": ");
        print_str(issues[i].message);
        print_str("\n");
    }
}

static void test_mode(int verbose)
{
    print_str("reviewer: testing\n");

    run_review("auth_module.c", verbose);

    int err = 0, warn = 0;
    for (int i = 0; i < issue_count; i++) {
        if (issues[i].severity == 2) err++;
        if (issues[i].severity == 1) warn++;
    }
    if (err > 0 && warn > 0) {
        print_str("reviewer: issue detection: OK\n");
    } else {
        print_str("reviewer: issue detection: FAIL\n");
    }

    if (verbose) list_issues();
    print_str("reviewer: test complete\n");
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
            run_review(arg, verbose);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }
    if (list) { list_issues(); host_exit(0); }

    print_str("reviewer: usage: reviewer [-t] [-l] [-v] [-i] [-s] [FILE...]\n");
    host_exit(0);
}
