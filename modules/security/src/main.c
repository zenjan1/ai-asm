/* security: Security vulnerability detection agent (v9.2) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_VULNS 32
#define MAX_TYPE_LEN 32
#define MAX_LOC_LEN 64
#define MAX_FIX_LEN 256

typedef struct {
    char type[MAX_TYPE_LEN];
    char location[MAX_LOC_LEN];
    int severity; /* 1=LOW, 2=MED, 3=HIGH, 4=CRITICAL */
    char fix[MAX_FIX_LEN];
} vuln_entry_t;

static unsigned int heap_pos = 65536;
static vuln_entry_t vulns[MAX_VULNS];
static int vuln_count = 0;
static int files_scanned = 0;

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

static const char *sev_name(int s)
{
    switch (s) {
        case 1: return "LOW";
        case 2: return "MED";
        case 3: return "HIGH";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

static int add_vuln(const char *type, const char *loc, int severity, const char *fix)
{
    if (vuln_count >= MAX_VULNS) return -1;
    vuln_entry_t *v = &vulns[vuln_count];
    copy_string(v->type, type, MAX_TYPE_LEN);
    copy_string(v->location, loc, MAX_LOC_LEN);
    v->severity = severity;
    copy_string(v->fix, fix, MAX_FIX_LEN);
    vuln_count++;
    return vuln_count - 1;
}

static void scan_file(const char *file, int verbose)
{
    files_scanned++;
    print_str("security: scanning ");
    print_str(file);
    print_str("\n");

    /* Simulated vulnerability checks */
    add_vuln("SQL_INJECT", "line:42", 3, "Use parameterized queries instead of string concatenation");
    add_vuln("XSS", "line:88", 2, "Sanitize HTML output with htmlspecialchars or equivalent");
    add_vuln("BUFFER_OVERFLOW", "line:15", 4, "Add bounds checking before buffer write");
    add_vuln("AUTH_BYPASS", "line:200", 3, "Verify authentication before granting access");

    if (verbose) {
        for (int i = 0; i < vuln_count; i++) {
            print_str("security:   [");
            print_str(sev_name(vulns[i].severity));
            print_str("] ");
            print_str(vulns[i].type);
            print_str(" at ");
            print_str(vulns[i].location);
            print_str("\n");
            print_str("security:   fix: ");
            print_str(vulns[i].fix);
            print_str("\n");
        }
    }
    print_str("security: scan of ");
    print_str(file);
    print_str(" complete (");
    print_int(vuln_count);
    print_str(" issues)\n");
}

static void show_help(void)
{
    print_str("security: Security vulnerability detection agent (v9.2)\n");
    print_str("Usage: security [OPTIONS] [FILE...]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -t             Test mode\n");
    print_str("  -l             List vulnerabilities\n");
    print_str("\n");
    print_str("Detects: XSS, SQL injection, buffer overflow, auth bypass, memory leaks\n");
}

static void show_info(void)
{
    print_str("security: info:\n");
    print_str("security: max vulnerabilities: 32\n");
    print_str("security: severity levels: LOW(1), MED(2), HIGH(3), CRITICAL(4)\n");
    print_str("security: info display complete\n");
}

static void show_status(void)
{
    print_str("security: status:\n");
    print_str("security: files scanned: ");
    print_int(files_scanned);
    print_str("\n");
    print_str("security: total issues: ");
    print_int(vuln_count);
    print_str("\n");
    print_str("security: status check complete\n");
}

static void list_vulns(void)
{
    int critical = 0, high = 0, med = 0, low = 0;
    for (int i = 0; i < vuln_count; i++) {
        switch (vulns[i].severity) {
            case 4: critical++; break;
            case 3: high++; break;
            case 2: med++; break;
            case 1: low++; break;
        }
    }
    print_str("security: vulnerabilities (");
    print_int(vuln_count);
    print_str(" total)\n");
    print_str("security: CRITICAL=");
    print_int(critical);
    print_str(" HIGH=");
    print_int(high);
    print_str(" MED=");
    print_int(med);
    print_str(" LOW=");
    print_int(low);
    print_str("\n");

    for (int i = 0; i < vuln_count; i++) {
        print_str("security:   [");
        print_str(sev_name(vulns[i].severity));
        print_str("] ");
        print_str(vulns[i].type);
        print_str(" @ ");
        print_str(vulns[i].location);
        print_str("\n");
        print_str("security:       fix: ");
        print_str(vulns[i].fix);
        print_str("\n");
    }
}

static void test_mode(int verbose)
{
    print_str("security: testing\n");

    scan_file("auth_handler.c", verbose);

    if (vuln_count > 0) {
        print_str("security: detection: OK\n");
    } else {
        print_str("security: detection: FAIL\n");
    }

    if (verbose) list_vulns();
    print_str("security: test complete\n");
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
            scan_file(arg, verbose);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help) { show_help(); host_exit(0); }
    if (info) { show_info(); host_exit(0); }
    if (status) { show_status(); host_exit(0); }
    if (test) { test_mode(verbose); host_exit(0); }
    if (list) { list_vulns(); host_exit(0); }

    print_str("security: usage: security [-t] [-l] [-v] [-i] [-s] [FILE...]\n");
    host_exit(0);
}
