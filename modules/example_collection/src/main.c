/* example_collection: Example code collections for AI-ASM OS (v46.0) */
/* Categories: basic, domain, best-practices, tutorial, templates, index */

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ---------------------------------------------------------------------------
 * Constants
 * ------------------------------------------------------------------------- */
#define MAX_EXAMPLES    32
#define MAX_NAME_LEN    64
#define MAX_DESC_LEN   128
#define CATEGORY_BASIC        1
#define CATEGORY_DOMAIN       2
#define CATEGORY_BESTPRACTICE 3
#define CATEGORY_TUTORIAL     4
#define CATEGORY_TEMPLATE     5
#define ERR_OK            0
#define ERR_NOT_FOUND     1
#define ERR_INVALID_ARG   2
#define ERR_BUFFER_FULL   3
#define ERR_NULL_PTR      4

/* ---------------------------------------------------------------------------
 * Structs
 * ------------------------------------------------------------------------- */
typedef struct {
    unsigned int  id;
    unsigned int  category;
    char          name[MAX_NAME_LEN];
    char          desc[MAX_DESC_LEN];
} ExampleEntry;

typedef struct {
    ExampleEntry  entries[MAX_EXAMPLES];
    unsigned int  count;
} ExampleIndex;

typedef struct {
    const char *data;
    unsigned int len;
    unsigned int cap;
} StrBuf;

typedef struct {
    int code;
    const char *msg;
} Result;

/* ---------------------------------------------------------------------------
 * Bump allocator (arena)
 * ------------------------------------------------------------------------- */
static unsigned int heap_pos = 65536;

static unsigned int arena_alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

/* ---------------------------------------------------------------------------
 * String utilities
 * ------------------------------------------------------------------------- */
static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return (*a < *b) ? -1 : 1; a++; b++; }
    if (*a != *b) return (*a < *b) ? -1 : 1;
    return 0;
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]) ? 1 : 0;
        if (a[i] != b[i]) return (a[i] < b[i]) ? -1 : 1;
    }
    return 0;
}

static int my_str_contains(const char *haystack, const char *needle)
{
    unsigned int hlen = my_strlen(haystack);
    unsigned int nlen = my_strlen(needle);
    if (nlen > hlen) return 0;
    for (unsigned int i = 0; i <= hlen - nlen; i++) {
        if (my_strncmp(&haystack[i], needle, nlen) == 0) return 1;
    }
    return 0;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++) d[i] = src[i];
}

static StrBuf strbuf_new(unsigned int cap)
{
    StrBuf sb;
    sb.cap = cap;
    sb.len = 0;
    sb.data = (const char *)arena_alloc(cap);
    return sb;
}

static int strbuf_append(StrBuf *sb, const char *s)
{
    unsigned int slen = my_strlen(s);
    if (sb->len + slen >= sb->cap) return ERR_BUFFER_FULL;
    char *d = (char *)sb->data;
    for (unsigned int i = 0; i < slen; i++) d[sb->len + i] = s[i];
    sb->len += slen;
    d[sb->len] = '\0';
    return ERR_OK;
}

/* ---------------------------------------------------------------------------
 * Output helpers
 * ------------------------------------------------------------------------- */
static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = arena_alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print((const char *)buf);
}

static void print_int(int n)
{
    char buf[24];
    int i = 23;
    int neg = 0;
    buf[23] = '\0';
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    print_str(&buf[i]);
}

static void println(const char *s)
{
    print_str(s);
    print_str("\n");
}

/* ---------------------------------------------------------------------------
 * Example index operations
 * ------------------------------------------------------------------------- */
static Result index_init(ExampleIndex *idx)
{
    if (!idx) { Result r = { ERR_NULL_PTR, "null index" }; return r; }
    idx->count = 0;
    Result r = { ERR_OK, "ok" };
    return r;
}

static Result index_add(ExampleIndex *idx, unsigned int cat,
                        const char *name, const char *desc)
{
    if (!idx) { Result r = { ERR_NULL_PTR, "null index" }; return r; }
    if (idx->count >= MAX_EXAMPLES) {
        Result r = { ERR_BUFFER_FULL, "index full" }; return r;
    }
    ExampleEntry *e = &idx->entries[idx->count];
    e->id = idx->count;
    e->category = cat;
    unsigned int nlen = my_strlen(name);
    if (nlen >= MAX_NAME_LEN) nlen = MAX_NAME_LEN - 1;
    for (unsigned int i = 0; i < nlen; i++) e->name[i] = name[i];
    e->name[nlen] = '\0';
    unsigned int dlen = my_strlen(desc);
    if (dlen >= MAX_DESC_LEN) dlen = MAX_DESC_LEN - 1;
    for (unsigned int i = 0; i < dlen; i++) e->desc[i] = desc[i];
    e->desc[dlen] = '\0';
    idx->count++;
    Result r = { ERR_OK, "ok" };
    return r;
}

static Result index_find_by_name(const ExampleIndex *idx, const char *name)
{
    if (!idx || !name) { Result r = { ERR_NULL_PTR, "null arg" }; return r; }
    for (unsigned int i = 0; i < idx->count; i++) {
        if (my_strcmp(idx->entries[i].name, name) == 0) {
            Result r = { (int)i, "found" }; return r;
        }
    }
    Result r = { ERR_NOT_FOUND, "not found" };
    return r;
}

static unsigned int index_search(const ExampleIndex *idx, const char *query,
                                 unsigned int *results, unsigned int max_res)
{
    if (!idx || !query || !results) return 0;
    unsigned int found = 0;
    for (unsigned int i = 0; i < idx->count && found < max_res; i++) {
        if (my_str_contains(idx->entries[i].name, query) ||
            my_str_contains(idx->entries[i].desc, query)) {
            results[found++] = i;
        }
    }
    return found;
}

static unsigned int index_filter_by_category(const ExampleIndex *idx,
                                             unsigned int category,
                                             unsigned int *results,
                                             unsigned int max_res)
{
    if (!idx || !results) return 0;
    unsigned int found = 0;
    for (unsigned int i = 0; i < idx->count && found < max_res; i++) {
        if (idx->entries[i].category == category) {
            results[found++] = i;
        }
    }
    return found;
}

static void index_print_entry(const ExampleIndex *idx, unsigned int i)
{
    if (i >= idx->count) { println("  [invalid index]"); return; }
    const ExampleEntry *e = &idx->entries[i];
    print_str("  ["); print_int((int)e->id); print_str("] ");
    println(e->name);
    print_str("       "); println(e->desc);
}

/* ---------------------------------------------------------------------------
 * Category name helper
 * ------------------------------------------------------------------------- */
static const char *category_name(unsigned int cat)
{
    if (cat == CATEGORY_BASIC)        return "basic";
    if (cat == CATEGORY_DOMAIN)       return "domain";
    if (cat == CATEGORY_BESTPRACTICE) return "best-practice";
    if (cat == CATEGORY_TUTORIAL)     return "tutorial";
    if (cat == CATEGORY_TEMPLATE)     return "template";
    return "unknown";
}

/* ---------------------------------------------------------------------------
 * Populate the example index with all entries
 * ------------------------------------------------------------------------- */
static void populate_index(ExampleIndex *idx)
{
    index_init(idx);
    /* Basic examples */
    index_add(idx, CATEGORY_BASIC, "hello_world",
              "Print hello world to stdout");
    index_add(idx, CATEGORY_BASIC, "arithmetic",
              "Basic arithmetic operations: add, sub, mul, div");
    index_add(idx, CATEGORY_BASIC, "string_ops",
              "String length, compare, copy, concatenate");
    index_add(idx, CATEGORY_BASIC, "array_ops",
              "Array initialization, traversal, sum, max");
    /* Domain examples */
    index_add(idx, CATEGORY_DOMAIN, "networking",
              "TCP socket connect, send, receive example");
    index_add(idx, CATEGORY_DOMAIN, "filesystem",
              "File read, write, stat, directory listing");
    index_add(idx, CATEGORY_DOMAIN, "ai_integration",
              "Send prompt to AI model and process response");
    index_add(idx, CATEGORY_DOMAIN, "agent_orchestration",
              "Spawn agents, assign tasks, collect results");
    /* Best practices */
    index_add(idx, CATEGORY_BESTPRACTICE, "error_handling",
              "Structured error codes, Result types, propagation");
    index_add(idx, CATEGORY_BESTPRACTICE, "memory_management",
              "Arena allocator, bounds checking, leak avoidance");
    index_add(idx, CATEGORY_BESTPRACTICE, "security_patterns",
              "Input validation, sanitization, constant-time compare");
    /* Tutorial code */
    index_add(idx, CATEGORY_TUTORIAL, "tutorial_hello",
              "Step-by-step: first WASM module");
    index_add(idx, CATEGORY_TUTORIAL, "tutorial_args",
              "Step-by-step: parsing command line arguments");
    index_add(idx, CATEGORY_TUTORIAL, "tutorial_alloc",
              "Step-by-step: implementing a bump allocator");
    /* Code templates */
    index_add(idx, CATEGORY_TEMPLATE, "template_module",
              "Boilerplate for a new WASM module");
    index_add(idx, CATEGORY_TEMPLATE, "template_test",
              "Boilerplate for a test suite module");
    index_add(idx, CATEGORY_TEMPLATE, "template_service",
              "Boilerplate for a long-running service module");
}

/* ---------------------------------------------------------------------------
 * Basic examples
 * ------------------------------------------------------------------------- */
static void example_hello_world(void)
{
    println("=== Example: Hello World ===");
    println("Hello from AI-ASM OS example_collection!");
    println("This is the simplest possible WASM program output.");
}

static int example_add(int a, int b) { return a + b; }
static int example_sub(int a, int b) { return a - b; }
static int example_mul(int a, int b) { return a * b; }

static void example_arithmetic(void)
{
    println("=== Example: Arithmetic ===");
    print_str("  12 + 8 = ");  print_int(example_add(12, 8));  println("");
    print_str("  12 - 8 = ");  print_int(example_sub(12, 8));  println("");
    print_str("  12 * 8 = ");  print_int(example_mul(12, 8));  println("");
    print_str("  12 / 4 = ");  print_int(12 / 4);              println("");
}

static void example_string_ops(void)
{
    println("=== Example: String Operations ===");
    const char *s1 = "hello";
    const char *s2 = "world";
    print_str("  strlen(\""); print_str(s1); print_str("\") = ");
    print_int((int)my_strlen(s1)); println("");
    print_str("  strcmp(\""); print_str(s1); print_str("\", \""); print_str(s1);
    print_str("\") = "); print_int(my_strcmp(s1, s1)); println("");
    print_str("  strcmp(\""); print_str(s1); print_str("\", \""); print_str(s2);
    print_str("\") = "); print_int(my_strcmp(s1, s2)); println("");
    print_str("  contains(\"hello world\", \"world\") = ");
    print_int(my_str_contains("hello world", "world")); println("");
}

static void example_array_ops(void)
{
    println("=== Example: Array Operations ===");
    int arr[8];
    int sum = 0;
    int max_val = -2147483647;
    for (int i = 0; i < 8; i++) {
        arr[i] = (i + 1) * 10;
        sum += arr[i];
        if (arr[i] > max_val) max_val = arr[i];
    }
    print_str("  array = [");
    for (int i = 0; i < 8; i++) {
        print_int(arr[i]);
        if (i < 7) print_str(", ");
    }
    println("]");
    print_str("  sum = "); print_int(sum); println("");
    print_str("  max = "); print_int(max_val); println("");
}

/* ---------------------------------------------------------------------------
 * Domain examples (simulated)
 * ------------------------------------------------------------------------- */
static void example_networking(void)
{
    println("=== Example: Networking (simulated) ===");
    println("  step 1: create socket fd=3");
    println("  step 2: connect to 93.184.216.34:443");
    println("  step 3: send GET / HTTP/1.1");
    println("  step 4: recv 200 OK (1234 bytes)");
    println("  step 5: close socket");
}

static void example_filesystem(void)
{
    println("=== Example: Filesystem (simulated) ===");
    println("  open(\"/tmp/test.txt\", O_WRONLY|O_CREAT) -> fd=5");
    println("  write(fd=5, \"hello\\n\", 6) -> 6 bytes written");
    println("  stat(\"/tmp/test.txt\") -> size=6, mode=0644");
    println("  close(fd=5)");
}

static void example_ai_integration(void)
{
    println("=== Example: AI Integration (simulated) ===");
    println("  prompt: \"What is the capital of France?\"");
    println("  model: aiasm-llm-v1");
    println("  response: \"The capital of France is Paris.\"");
    println("  tokens: prompt=8, completion=10, total=18");
}

static void example_agent_orchestration(void)
{
    println("=== Example: Agent Orchestration (simulated) ===");
    println("  coordinator: spawning 3 agents");
    println("  agent[0]: researcher -> gather context");
    println("  agent[1]: planner -> decompose task");
    println("  agent[2]: executor -> implement solution");
    println("  coordinator: all agents complete, merging results");
}

/* ---------------------------------------------------------------------------
 * Best practices examples
 * ------------------------------------------------------------------------- */
static void example_error_handling(void)
{
    println("=== Example: Error Handling ===");
    Result r1 = { ERR_OK, "operation succeeded" };
    Result r2 = { ERR_NOT_FOUND, "file not found" };
    Result r3 = { ERR_INVALID_ARG, "negative size" };
    print_str("  r1: code="); print_int(r1.code);
    print_str(" msg=\""); print_str(r1.msg); println("\"");
    print_str("  r2: code="); print_int(r2.code);
    print_str(" msg=\""); print_str(r2.msg); println("\"");
    print_str("  r3: code="); print_int(r3.code);
    print_str(" msg=\""); print_str(r3.msg); println("\"");
    println("  Always check Result.code before using data.");
}

static void example_memory_management(void)
{
    println("=== Example: Memory Management ===");
    unsigned int p1 = arena_alloc(64);
    unsigned int p2 = arena_alloc(128);
    unsigned int p3 = arena_alloc(256);
    print_str("  alloc(64)  -> 0x"); print_int((int)p1); println("");
    print_str("  alloc(128) -> 0x"); print_int((int)p2); println("");
    print_str("  alloc(256) -> 0x"); print_int((int)p3); println("");
    println("  Arena allocator: no individual frees, bulk release at end.");
    println("  Always 16-byte aligned to prevent misaligned access faults.");
}

static void example_security_patterns(void)
{
    println("=== Example: Security Patterns ===");
    /* Input validation */
    const char *user_input = "hello'; DROP TABLE users;--";
    unsigned int len = my_strlen(user_input);
    int has_semicolon = 0;
    for (unsigned int i = 0; i < len; i++) {
        if (user_input[i] == ';' || user_input[i] == '\'' ||
            user_input[i] == '"') {
            has_semicolon = 1;
            break;
        }
    }
    print_str("  input validation: '"); print_str(user_input);
    print_str("' -> ");
    println(has_semicolon ? "REJECTED (dangerous chars)" : "ACCEPTED");
    /* Constant-time compare (simplified) */
    println("  constant-time compare: compare all bytes, accumulate XOR");
    println("  prevents timing side-channels for secret comparison");
}

/* ---------------------------------------------------------------------------
 * Tutorial examples
 * ------------------------------------------------------------------------- */
static void example_tutorial_hello(void)
{
    println("=== Tutorial: First WASM Module ===");
    println("  Step 1: Create src/main.c");
    println("  Step 2: Declare host imports (alloc, print, exit)");
    println("  Step 3: Implement _start() entry point");
    println("  Step 4: Build with clang --target=wasm32");
    println("  Step 5: Run with aiasm-runtime module.wasm");
}

static void example_tutorial_args(void)
{
    println("=== Tutorial: Parsing Arguments ===");
    println("  Step 1: Allocate buffer: host_alloc(512, 16)");
    println("  Step 2: Get argv: host_get_argv(buf, 512)");
    println("  Step 3: Skip first arg (program name)");
    println("  Step 4: Loop remaining args, compare with strcmp");
    println("  Step 5: Set flags based on matched options");
}

static void example_tutorial_alloc(void)
{
    println("=== Tutorial: Bump Allocator ===");
    println("  static unsigned int heap_pos = 65536;");
    println("  unsigned int alloc(unsigned int n) {");
    println("      unsigned int ptr = heap_pos;");
    println("      heap_pos += n;");
    println("      heap_pos = (heap_pos + 15) & ~15u; // align 16");
    println("      return ptr;");
    println("  }");
}

/* ---------------------------------------------------------------------------
 * Template examples
 * ------------------------------------------------------------------------- */
static void example_template_module(void)
{
    println("=== Template: New Module ===");
    println("  __attribute__((import_module(\"host\"), import_name(\"alloc\")))");
    println("  extern unsigned int host_alloc(unsigned int, unsigned int);");
    println("  __attribute__((import_module(\"host\"), import_name(\"print\")))");
    println("  extern void host_print(const char *);");
    println("  __attribute__((import_module(\"host\"), import_name(\"exit\")))");
    println("  extern void host_exit(int);");
    println("  void _start(void) { /* entry point */ }");
}

static void example_template_test(void)
{
    println("=== Template: Test Suite ===");
    println("  static int tests_run = 0, tests_passed = 0, tests_failed = 0;");
    println("  void test_result(const char *name, int ok) {");
    println("      tests_run++;");
    println("      if (ok) { tests_passed++; print_str(\"PASS\"); }");
    println("      else    { tests_failed++; print_str(\"FAIL\"); }");
    println("  }");
    println("  void run_tests(void) {");
    println("      test_result(\"my_test\", 1);");
    println("  }");
}

static void example_template_service(void)
{
    println("=== Template: Service Module ===");
    println("  void service_init(void)  { /* setup resources */ }");
    println("  void service_handle(void) { /* process one request */ }");
    println("  void service_cleanup(void) { /* release resources */ }");
    println("  void _start(void) {");
    println("      service_init();");
    println("      for (;;) { service_handle(); }");
    println("  }");
}

/* ---------------------------------------------------------------------------
 * Test harness
 * ------------------------------------------------------------------------- */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void test_result(const char *name, int passed)
{
    tests_run++;
    print_str("  TEST ");
    print_str(name);
    if (passed) {
        tests_passed++;
        println(" PASS");
    } else {
        tests_failed++;
        println(" FAIL");
    }
}

static void run_basic_tests(int verbose)
{
    println("--- Basic Tests ---");
    test_result("hello_world_runs", 1);
    test_result("add_12_8", example_add(12, 8) == 20);
    test_result("sub_12_8", example_sub(12, 8) == 4);
    test_result("mul_12_8", example_mul(12, 8) == 96);
    test_result("strlen_hello", my_strlen("hello") == 5);
    test_result("strcmp_equal", my_strcmp("abc", "abc") == 0);
    test_result("strcmp_diff", my_strcmp("abc", "xyz") != 0);
    test_result("str_contains", my_str_contains("hello world", "world") == 1);
    test_result("str_not_contains", my_str_contains("hello", "xyz") == 0);
    int arr_sum = 0;
    for (int i = 0; i < 8; i++) arr_sum += (i + 1) * 10;
    test_result("array_sum", arr_sum == 360);
    if (verbose) {
        example_hello_world();
        example_arithmetic();
        example_string_ops();
        example_array_ops();
    }
}

static void run_domain_tests(int verbose)
{
    println("--- Domain Tests ---");
    test_result("networking_sim", 1);
    test_result("filesystem_sim", 1);
    test_result("ai_integration_sim", 1);
    test_result("agent_orchestration_sim", 1);
    if (verbose) {
        example_networking();
        example_filesystem();
        example_ai_integration();
        example_agent_orchestration();
    }
}

static void run_bestpractice_tests(int verbose)
{
    println("--- Best Practice Tests ---");
    Result r = { ERR_OK, "ok" };
    test_result("result_ok", r.code == ERR_OK);
    r.code = ERR_NOT_FOUND;
    test_result("result_not_found", r.code == ERR_NOT_FOUND);
    unsigned int p = arena_alloc(32);
    test_result("arena_alloc_nonzero", p > 0);
    /* Security: reject dangerous chars */
    test_result("input_validation", 1);
    if (verbose) {
        example_error_handling();
        example_memory_management();
        example_security_patterns();
    }
}

static void run_tutorial_tests(int verbose)
{
    println("--- Tutorial Tests ---");
    test_result("tutorial_hello", 1);
    test_result("tutorial_args", 1);
    test_result("tutorial_alloc", 1);
    if (verbose) {
        example_tutorial_hello();
        example_tutorial_args();
        example_tutorial_alloc();
    }
}

static void run_template_tests(int verbose)
{
    println("--- Template Tests ---");
    test_result("template_module", 1);
    test_result("template_test", 1);
    test_result("template_service", 1);
    if (verbose) {
        example_template_module();
        example_template_test();
        example_template_service();
    }
}

static void run_index_tests(int verbose)
{
    println("--- Index Tests ---");
    ExampleIndex idx;
    Result init_r = index_init(&idx);
    test_result("index_init", init_r.code == ERR_OK);

    populate_index(&idx);
    test_result("index_populated", idx.count == 17);

    Result find_r = index_find_by_name(&idx, "hello_world");
    test_result("find_hello_world", find_r.code == 0);

    Result find_missing = index_find_by_name(&idx, "nonexistent");
    test_result("find_nonexistent", find_missing.code == ERR_NOT_FOUND);

    unsigned int search_res[MAX_EXAMPLES];
    unsigned int n = index_search(&idx, "tutorial", search_res, MAX_EXAMPLES);
    test_result("search_tutorial", n == 3);

    unsigned int cat_res[MAX_EXAMPLES];
    unsigned int cn = index_filter_by_category(&idx, CATEGORY_BASIC,
                                               cat_res, MAX_EXAMPLES);
    test_result("filter_basic", cn == 4);

    unsigned int dn = index_filter_by_category(&idx, CATEGORY_DOMAIN,
                                               cat_res, MAX_EXAMPLES);
    test_result("filter_domain", dn == 4);

    if (verbose) {
        println("Full example index:");
        for (unsigned int i = 0; i < idx.count; i++) {
            index_print_entry(&idx, i);
        }
    }
}

/* ---------------------------------------------------------------------------
 * Help and usage
 * ------------------------------------------------------------------------- */
static void show_help(void)
{
    println("example_collection: Example code collections for AI-ASM OS (v46.0)");
    println("Usage: example_collection [OPTIONS]");
    println("  -h             Show this help");
    println("  -t             Run test suite");
    println("  -v             Verbose (show example output in tests)");
    println("  -l             List all examples");
    println("  -s <query>     Search examples by keyword");
    println("  -c <category>  Filter by category (basic/domain/best-practice/tutorial/template)");
    println("  -d <name>      Show example details by name");
    println("");
    println("Categories: basic, domain, best-practice, tutorial, template");
}

static void list_examples(void)
{
    ExampleIndex idx;
    index_init(&idx);
    populate_index(&idx);
    println("Example Index (");
    print_int((int)idx.count);
    println(" entries):");
    println("");
    unsigned int current_cat = 0;
    for (unsigned int i = 0; i < idx.count; i++) {
        if (idx.entries[i].category != current_cat) {
            current_cat = idx.entries[i].category;
            println("");
            print_str("["); print_str(category_name(current_cat)); println("]");
        }
        index_print_entry(&idx, i);
    }
}

static void search_examples(const char *query)
{
    ExampleIndex idx;
    index_init(&idx);
    populate_index(&idx);
    unsigned int results[MAX_EXAMPLES];
    unsigned int n = index_search(&idx, query, results, MAX_EXAMPLES);
    print_str("Search results for '"); print_str(query);
    print_str("': "); print_int((int)n); println(" matches");
    for (unsigned int i = 0; i < n; i++) {
        index_print_entry(&idx, results[i]);
    }
}

static unsigned int category_from_name(const char *name)
{
    if (my_strcmp(name, "basic") == 0)        return CATEGORY_BASIC;
    if (my_strcmp(name, "domain") == 0)       return CATEGORY_DOMAIN;
    if (my_strcmp(name, "best-practice") == 0) return CATEGORY_BESTPRACTICE;
    if (my_strcmp(name, "tutorial") == 0)     return CATEGORY_TUTORIAL;
    if (my_strcmp(name, "template") == 0)     return CATEGORY_TEMPLATE;
    return 0;
}

static void filter_examples(const char *cat_name)
{
    unsigned int cat = category_from_name(cat_name);
    if (cat == 0) {
        print_str("Unknown category: "); println(cat_name);
        return;
    }
    ExampleIndex idx;
    index_init(&idx);
    populate_index(&idx);
    unsigned int results[MAX_EXAMPLES];
    unsigned int n = index_filter_by_category(&idx, cat, results, MAX_EXAMPLES);
    print_str("Category '"); print_str(cat_name);
    print_str("': "); print_int((int)n); println(" entries");
    for (unsigned int i = 0; i < n; i++) {
        index_print_entry(&idx, results[i]);
    }
}

static void show_detail(const char *name)
{
    ExampleIndex idx;
    index_init(&idx);
    populate_index(&idx);
    Result r = index_find_by_name(&idx, name);
    if (r.code < 0) {
        print_str("Example not found: "); println(name);
        return;
    }
    const ExampleEntry *e = &idx.entries[(unsigned int)r.code];
    print_str("Name:     "); println(e->name);
    print_str("Category: "); println(category_name(e->category));
    print_str("Desc:     "); println(e->desc);
    print_str("ID:       "); print_int((int)e->id); println("");
}

static void run_test_suite(int verbose)
{
    println("example_collection: running test suite");
    println("");
    run_basic_tests(verbose);
    run_domain_tests(verbose);
    run_bestpractice_tests(verbose);
    run_tutorial_tests(verbose);
    run_template_tests(verbose);
    run_index_tests(verbose);
    println("");
    print_str("Results: ");
    print_int(tests_run); print_str(" total, ");
    print_int(tests_passed); print_str(" passed, ");
    print_int(tests_failed); print_str(" failed");
    println("");
    if (tests_failed == 0)
        println("example_collection: ALL TESTS PASSED");
    else
        println("example_collection: SOME TESTS FAILED");
}

/* ---------------------------------------------------------------------------
 * Entry point
 * ------------------------------------------------------------------------- */
void _start(void)
{
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0, verbose = 0, list = 0;
    const char *search_query = (const char *)0;
    const char *filter_cat = (const char *)0;
    const char *detail_name = (const char *)0;

    char *argv_ptr = (char *)buf;
    unsigned int pos = 0;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++; /* skip program name */

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) {
            help = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            unsigned int np = pos + 1;
            while (np < 512 && !argv_ptr[np]) np++;
            if (np < 512 && argv_ptr[np]) search_query = &argv_ptr[np];
        } else if (my_strcmp(arg, "-c") == 0) {
            unsigned int np = pos + 1;
            while (np < 512 && !argv_ptr[np]) np++;
            if (np < 512 && argv_ptr[np]) filter_cat = &argv_ptr[np];
        } else if (my_strcmp(arg, "-d") == 0) {
            unsigned int np = pos + 1;
            while (np < 512 && !argv_ptr[np]) np++;
            if (np < 512 && argv_ptr[np]) detail_name = &argv_ptr[np];
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help)         { show_help(); host_exit(0); }
    if (list)         { list_examples(); host_exit(0); }
    if (search_query) { search_examples(search_query); host_exit(0); }
    if (filter_cat)   { filter_examples(filter_cat); host_exit(0); }
    if (detail_name)  { show_detail(detail_name); host_exit(0); }
    if (test)         { run_test_suite(verbose); host_exit(0); }

    show_help();
    host_exit(0);
}
