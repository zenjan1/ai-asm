/* test_framework: System test framework (v14.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Test types */
#define TEST_UNIT        0
#define TEST_INTEGRATION 1
#define TEST_SYSTEM      2
#define TEST_PERFORMANCE 3
#define TEST_SECURITY    4

/* Test results */
#define TEST_PASS   0
#define TEST_FAIL   1
#define TEST_SKIP   2

/* Limits */
#define MAX_TEST_CASES   64
#define TEST_NAME_LEN    64
#define MODULE_NAME_LEN  32
#define SETUP_LEN        256
#define EXECUTE_LEN      256
#define EXPECTED_LEN     256
#define SUMMARY_LEN      1024

/* Test case */
typedef struct {
    int   case_id;
    char  name[TEST_NAME_LEN];
    int   type;
    char  module[MODULE_NAME_LEN];
    char  setup[SETUP_LEN];
    char  execute[EXECUTE_LEN];
    char  expected[EXPECTED_LEN];
    int   timeout_ms;
    int   result;
    int   duration_ms;
} test_case_t;

/* Test report */
typedef struct {
    int total_cases;
    int passed;
    int failed;
    int skipped;
    int coverage_percent;
    char summary[SUMMARY_LEN];
} test_report_t;

static test_case_t test_cases[MAX_TEST_CASES];
static int case_count = 0;
static int next_case_id = 1;

/* Report tracking */
static int total_executed = 0;
static int total_passed = 0;
static int total_failed = 0;
static int total_skipped = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    } return 0;
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

/* Get test type name */
static const char *test_type_name(int type) {
    if (type == TEST_UNIT) return "unit";
    if (type == TEST_INTEGRATION) return "integration";
    if (type == TEST_SYSTEM) return "system";
    if (type == TEST_PERFORMANCE) return "performance";
    if (type == TEST_SECURITY) return "security";
    return "unknown";
}

/* Get result name */
static const char *result_name(int result) {
    if (result == TEST_PASS) return "PASS";
    if (result == TEST_FAIL) return "FAIL";
    if (result == TEST_SKIP) return "SKIP";
    return "???";
}

/* Register a test case */
int test_register_case(test_case_t *tc) {
    if (!tc || case_count >= MAX_TEST_CASES) return -1;
    int idx = case_count;
    test_cases[idx].case_id = next_case_id;
    my_strncpy(test_cases[idx].name, tc->name, TEST_NAME_LEN - 1);
    test_cases[idx].type = tc->type;
    my_strncpy(test_cases[idx].module, tc->module, MODULE_NAME_LEN - 1);
    my_strncpy(test_cases[idx].setup, tc->setup, SETUP_LEN - 1);
    my_strncpy(test_cases[idx].execute, tc->execute, EXECUTE_LEN - 1);
    my_strncpy(test_cases[idx].expected, tc->expected, EXPECTED_LEN - 1);
    test_cases[idx].timeout_ms = tc->timeout_ms;
    test_cases[idx].result = TEST_SKIP;
    test_cases[idx].duration_ms = 0;
    int id = next_case_id++;
    case_count++;
    print_str("[test] registered: ");
    print_str(tc->name);
    print_str(" (");
    print_str(test_type_name(tc->type));
    print_str(", ");
    print_str(tc->module);
    print_str(")\n");
    return id;
}

/* Run a single test case (simulated) */
static int run_single_test(int idx) {
    print_str("  Running: ");
    print_str(test_cases[idx].name);
    print_str("... ");

    /* Simulate test execution */
    /* Tests pass if they have non-empty execute and expected */
    if (test_cases[idx].execute[0] && test_cases[idx].expected[0]) {
        test_cases[idx].result = TEST_PASS;
        test_cases[idx].duration_ms = 5 + (idx % 20);
        total_passed++;
        print_str("PASS (");
        print_int(test_cases[idx].duration_ms);
        print_str("ms)\n");
    } else {
        test_cases[idx].result = TEST_FAIL;
        test_cases[idx].duration_ms = 1;
        total_failed++;
        print_str("FAIL\n");
    }
    total_executed++;
    return test_cases[idx].result;
}

/* Run all tests */
int test_run_all(void) {
    print_str("\n=== Running All Tests ===\n");
    total_executed = 0;
    total_passed = 0;
    total_failed = 0;
    total_skipped = 0;

    for (int i = 0; i < case_count; i++) {
        run_single_test(i);
    }

    print_str("\nResults: ");
    print_int(total_passed);
    print_str(" passed, ");
    print_int(total_failed);
    print_str(" failed, ");
    print_int(total_skipped);
    print_str(" skipped (");
    print_int(total_executed);
    print_str(" total)\n");
    return total_failed == 0 ? 0 : 1;
}

/* Run tests for a specific module */
int test_run_module(const char *module) {
    print_str("\n=== Running Module Tests: ");
    print_str(module);
    print_str(" ===\n");
    int mod_passed = 0, mod_failed = 0, mod_total = 0;

    for (int i = 0; i < case_count; i++) {
        if (my_strcmp(test_cases[i].module, module) == 0) {
            mod_total++;
            print_str("  Running: ");
            print_str(test_cases[i].name);
            print_str("... ");
            if (test_cases[i].execute[0] && test_cases[i].expected[0]) {
                test_cases[i].result = TEST_PASS;
                mod_passed++;
                print_str("PASS\n");
            } else {
                test_cases[i].result = TEST_FAIL;
                mod_failed++;
                print_str("FAIL\n");
            }
        }
    }

    print_str("Module results: ");
    print_int(mod_passed);
    print_str("/");
    print_int(mod_total);
    print_str(" passed\n");
    return mod_failed == 0 ? 0 : 1;
}

/* Generate test report */
int test_generate_report(test_report_t *report) {
    if (!report) return -1;
    report->total_cases = case_count;
    report->passed = total_passed;
    report->failed = total_failed;
    report->skipped = total_skipped;
    report->coverage_percent = (total_passed * 100) / (case_count > 0 ? case_count : 1);

    print_str("\n=== Test Report ===\n");
    print_str("Total cases: ");
    print_int(report->total_cases);
    print_str("\nPassed: ");
    print_int(report->passed);
    print_str("\nFailed: ");
    print_int(report->failed);
    print_str("\nSkipped: ");
    print_int(report->skipped);
    print_str("\nCoverage: ");
    print_int(report->coverage_percent);
    print_str("%\n");

    /* Summary by type */
    for (int t = 0; t <= TEST_SECURITY; t++) {
        int count = 0;
        for (int i = 0; i < case_count; i++) {
            if (test_cases[i].type == t) count++;
        }
        if (count > 0) {
            print_str("  ");
            print_str(test_type_name(t));
            print_str(": ");
            print_int(count);
            print_str(" tests\n");
        }
    }

    return 0;
}

/* Get coverage info */
int test_get_coverage(void) {
    print_str("\n=== Coverage Analysis ===\n");
    /* Group by module */
    for (int i = 0; i < case_count; i++) {
        int seen = 0;
        for (int j = 0; j < i; j++) {
            if (my_strcmp(test_cases[i].module, test_cases[j].module) == 0) { seen = 1; break; }
        }
        if (!seen) {
            int mod_total = 0, mod_pass = 0;
            for (int k = 0; k < case_count; k++) {
                if (my_strcmp(test_cases[k].module, test_cases[i].module) == 0) {
                    mod_total++;
                    if (test_cases[k].result == TEST_PASS) mod_pass++;
                }
            }
            print_str("  ");
            print_str(test_cases[i].module);
            print_str(": ");
            print_int(mod_pass);
            print_str("/");
            print_int(mod_total);
            print_str(" covered\n");
        }
    }
    return 0;
}

/* List all test cases */
void test_list_cases(void) {
    print_str("=== Test Cases ===\n");
    print_str("ID\tNAME\t\tTYPE\tMODULE\t\tRESULT\n");
    for (int i = 0; i < case_count; i++) {
        print_str("  ");
        print_int(test_cases[i].case_id);
        print_str("\t");
        print_str(test_cases[i].name);
        print_str("\t");
        print_str(test_type_name(test_cases[i].type));
        print_str("\t");
        print_str(test_cases[i].module);
        print_str("\t");
        print_str(result_name(test_cases[i].result));
        print_str("\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0, list = 0, report = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        else if (my_strcmp(arg, "-l") == 0 || my_strcmp(arg, "--list") == 0) list = 1;
        else if (my_strcmp(arg, "-r") == 0 || my_strcmp(arg, "--report") == 0) report = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Test Framework v14.0 - System Testing\n");
    if (help) {
        print_str("Usage: test_framework [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run test suite\n");
        print_str("  -l, --list    List test cases\n");
        print_str("  -r, --report  Show test report\n");
        return;
    }
    if (test) {
        print_str("=== Test Framework Demo ===\n");
        print_str("\n--- Register Test Cases ---\n");

        /* Unit tests */
        test_case_t tc1; my_strcpy(tc1.name, "test_string_utils"); tc1.type = TEST_UNIT; my_strcpy(tc1.module, "core"); my_strcpy(tc1.setup, "init"); my_strcpy(tc1.execute, "strlen,strcmp"); my_strcpy(tc1.expected, "correct"); tc1.timeout_ms = 100;
        test_register_case(&tc1);
        test_case_t tc2; my_strcpy(tc2.name, "test_memory_alloc"); tc2.type = TEST_UNIT; my_strcpy(tc2.module, "core"); my_strcpy(tc2.setup, "init"); my_strcpy(tc2.execute, "alloc,free"); my_strcpy(tc2.expected, "no leak"); tc2.timeout_ms = 100;
        test_register_case(&tc2);

        /* Integration tests */
        test_case_t tc3; my_strcpy(tc3.name, "test_sensor_to_decision"); tc3.type = TEST_INTEGRATION; my_strcpy(tc3.module, "iot"); my_strcpy(tc3.setup, "init sensors"); my_strcpy(tc3.execute, "read->evaluate->act"); my_strcpy(tc3.expected, "correct action"); tc3.timeout_ms = 500;
        test_register_case(&tc3);
        test_case_t tc4; my_strcpy(tc4.name, "test_mqtt_publish"); tc4.type = TEST_INTEGRATION; my_strcpy(tc4.module, "iot"); my_strcpy(tc4.setup, "init mqtt"); my_strcpy(tc4.execute, "publish topic"); my_strcpy(tc4.expected, "message received"); tc4.timeout_ms = 1000;
        test_register_case(&tc4);

        /* System tests */
        test_case_t tc5; my_strcpy(tc5.name, "test_home_scene"); tc5.type = TEST_SYSTEM; my_strcpy(tc5.module, "smart_home"); my_strcpy(tc5.setup, "register devices"); my_strcpy(tc5.execute, "apply morning scene"); my_strcpy(tc5.expected, "lights on, ac on"); tc5.timeout_ms = 2000;
        test_register_case(&tc5);
        test_case_t tc6; my_strcpy(tc6.name, "test_irrigation"); tc6.type = TEST_SYSTEM; my_strcpy(tc6.module, "smart_agri"); my_strcpy(tc6.setup, "setup zones"); my_strcpy(tc6.execute, "auto_irrigate zone1"); my_strcpy(tc6.expected, "sprinklers on"); tc6.timeout_ms = 2000;
        test_register_case(&tc6);

        /* Performance tests */
        test_case_t tc7; my_strcpy(tc7.name, "test_edge_inference"); tc7.type = TEST_PERFORMANCE; my_strcpy(tc7.module, "edge_ai"); my_strcpy(tc7.setup, "load model"); my_strcpy(tc7.execute, "100 inferences"); my_strcpy(tc7.expected, "<5000ms"); tc7.timeout_ms = 5000;
        test_register_case(&tc7);

        /* Security tests */
        test_case_t tc8; my_strcpy(tc8.name, "test_buffer_bounds"); tc8.type = TEST_SECURITY; my_strcpy(tc8.module, "core"); my_strcpy(tc8.setup, "init"); my_strcpy(tc8.execute, "overflow test"); my_strcpy(tc8.expected, "no crash"); tc8.timeout_ms = 200;
        test_register_case(&tc8);

        print_str("\n--- Run All Tests ---\n");
        test_run_all();

        print_str("\n--- Module Tests: core ---\n");
        test_run_module("core");

        print_str("\n--- Generate Report ---\n");
        test_report_t rpt;
        test_generate_report(&rpt);

        print_str("\n--- Coverage ---\n");
        test_get_coverage();

        print_str("\n=== Test Complete ===\n");
        return;
    }
    if (list) { test_list_cases(); return; }
    if (report) { test_report_t rpt; test_generate_report(&rpt); return; }
    print_str("Use -h for help, -t for test, -l for list, -r for report\n");
}
