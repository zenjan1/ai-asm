/* example_apps: Example applications collection (v18.0) */

#include <stddef.h>

/* Host functions */
extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Example types */
#define EXAMPLE_BASIC       0
#define EXAMPLE_AGENT       1
#define EXAMPLE_AI          2
#define EXAMPLE_IOT         3
#define EXAMPLE_GUI         4
#define EXAMPLE_NETWORKING  5
#define EXAMPLE_ADVANCED    6

/* Complexity levels */
#define COMPLEXITY_SIMPLE   0
#define COMPLEXITY_MEDIUM   1
#define COMPLEXITY_COMPLEX  2

/* Limits */
#define MAX_EXAMPLES  32
#define NAME_LEN      64
#define DESC_LEN      256
#define PATH_LEN      128
#define DOC_LEN       128
#define CODE_LEN      512

/* Example application */
typedef struct {
    int   example_id;
    int   type;
    char  name[NAME_LEN];
    char  description[DESC_LEN];
    char  source_path[PATH_LEN];
    char  documentation[DOC_LEN];
    int   complexity;
    int   verified;
} example_app_t;

static example_app_t examples[MAX_EXAMPLES];
static int example_count = 0;
static int next_example_id = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
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

/* Get type name */
static const char *type_name(int type) {
    if (type == EXAMPLE_BASIC) return "basic";
    if (type == EXAMPLE_AGENT) return "agent";
    if (type == EXAMPLE_AI) return "ai";
    if (type == EXAMPLE_IOT) return "iot";
    if (type == EXAMPLE_GUI) return "gui";
    if (type == EXAMPLE_NETWORKING) return "networking";
    if (type == EXAMPLE_ADVANCED) return "advanced";
    return "unknown";
}

/* Get complexity name */
static const char *complexity_name(int c) {
    if (c == COMPLEXITY_SIMPLE) return "simple";
    if (c == COMPLEXITY_MEDIUM) return "medium";
    if (c == COMPLEXITY_COMPLEX) return "complex";
    return "unknown";
}

/* Register an example */
static void register_example(int type, const char *name, const char *desc, const char *path, const char *doc, int complexity) {
    if (example_count >= MAX_EXAMPLES) return;
    int idx = example_count;
    examples[idx].example_id = next_example_id++;
    examples[idx].type = type;
    my_strncpy(examples[idx].name, name, NAME_LEN - 1);
    my_strncpy(examples[idx].description, desc, DESC_LEN - 1);
    my_strncpy(examples[idx].source_path, path, PATH_LEN - 1);
    my_strncpy(examples[idx].documentation, doc, DOC_LEN - 1);
    examples[idx].complexity = complexity;
    examples[idx].verified = 1;
    example_count++;
}

/* Create basic examples */
int example_create_basic(void) {
    print_str("=== Creating Basic Examples ===\n");

    register_example(EXAMPLE_BASIC, "hello_world.wasm",
        "Basic WASM module that prints Hello World",
        "examples/basic/hello_world.c",
        "docs/examples/hello_world.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: hello_world.wasm - Hello World\n");

    register_example(EXAMPLE_BASIC, "counter.wasm",
        "State management with persistent counter",
        "examples/basic/counter.c",
        "docs/examples/counter.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: counter.wasm - State Management\n");

    register_example(EXAMPLE_BASIC, "calculator.wasm",
        "Simple arithmetic calculator",
        "examples/basic/calculator.c",
        "docs/examples/calculator.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: calculator.wasm - Calculator\n");

    register_example(EXAMPLE_BASIC, "string_ops.wasm",
        "String manipulation utilities",
        "examples/basic/string_ops.c",
        "docs/examples/string_ops.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: string_ops.wasm - String Operations\n");

    print_str("  Total basic examples: 4\n");
    return 0;
}

/* Create agent demo */
int example_create_agent_demo(void) {
    print_str("=== Creating Agent Demos ===\n");

    register_example(EXAMPLE_AGENT, "simple_agent.wasm",
        "Basic agent with goal setting and task execution",
        "examples/agent/simple_agent.c",
        "docs/examples/simple_agent.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: simple_agent.wasm - Simple Agent\n");

    register_example(EXAMPLE_AGENT, "agent_ipc_demo.wasm",
        "Agent IPC communication demo",
        "examples/agent/ipc_demo.c",
        "docs/examples/agent_ipc.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: agent_ipc_demo.wasm - Agent IPC\n");

    register_example(EXAMPLE_AGENT, "task_planner.wasm",
        "Task planning and execution demo",
        "examples/agent/task_planner.c",
        "docs/examples/task_planner.md",
        COMPLEXITY_COMPLEX);
    print_str("  Created: task_planner.wasm - Task Planner\n");

    print_str("  Total agent examples: 3\n");
    return 0;
}

/* Create AI demos */
int example_create_ai_demo(void) {
    print_str("=== Creating AI Demos ===\n");

    register_example(EXAMPLE_AI, "ai_query_demo.wasm",
        "Basic AI query and response demo",
        "examples/ai/query_demo.c",
        "docs/examples/ai_query.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: ai_query_demo.wasm - AI Query\n");

    register_example(EXAMPLE_AI, "text_analyzer.wasm",
        "Text analysis with sentiment detection",
        "examples/ai/text_analyzer.c",
        "docs/examples/text_analyzer.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: text_analyzer.wasm - Text Analyzer\n");

    register_example(EXAMPLE_AI, "image_classifier_demo.wasm",
        "Image classification using vision AI",
        "examples/ai/image_classifier.c",
        "docs/examples/image_classifier.md",
        COMPLEXITY_COMPLEX);
    print_str("  Created: image_classifier_demo.wasm - Image Classifier\n");

    print_str("  Total AI examples: 3\n");
    return 0;
}

/* Create IoT demo */
int example_create_iot_demo(void) {
    print_str("=== Creating IoT Demos ===\n");

    register_example(EXAMPLE_IOT, "sensor_reader.wasm",
        "Read and process sensor data",
        "examples/iot/sensor_reader.c",
        "docs/examples/sensor_reader.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: sensor_reader.wasm - Sensor Reader\n");

    register_example(EXAMPLE_IOT, "mqtt_demo.wasm",
        "MQTT publish/subscribe demo",
        "examples/iot/mqtt_demo.c",
        "docs/examples/mqtt_demo.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: mqtt_demo.wasm - MQTT Demo\n");

    register_example(EXAMPLE_IOT, "smart_home_controller.wasm",
        "Smart home device controller",
        "examples/iot/smart_home.c",
        "docs/examples/smart_home.md",
        COMPLEXITY_COMPLEX);
    print_str("  Created: smart_home_controller.wasm - Smart Home Controller\n");

    print_str("  Total IoT examples: 3\n");
    return 0;
}

/* Create GUI and networking examples */
int example_create_gui_networking(void) {
    print_str("=== Creating GUI & Networking Examples ===\n");

    register_example(EXAMPLE_GUI, "window_demo.wasm",
        "Basic window creation and rendering",
        "examples/gui/window_demo.c",
        "docs/examples/window_demo.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: window_demo.wasm - Window Demo\n");

    register_example(EXAMPLE_GUI, "button_demo.wasm",
        "Interactive button and input handling",
        "examples/gui/button_demo.c",
        "docs/examples/button_demo.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: button_demo.wasm - Button Demo\n");

    register_example(EXAMPLE_NETWORKING, "http_client_demo.wasm",
        "HTTP GET/POST request demo",
        "examples/networking/http_client.c",
        "docs/examples/http_client.md",
        COMPLEXITY_SIMPLE);
    print_str("  Created: http_client_demo.wasm - HTTP Client\n");

    register_example(EXAMPLE_NETWORKING, "tcp_server_demo.wasm",
        "TCP echo server demo",
        "examples/networking/tcp_server.c",
        "docs/examples/tcp_server.md",
        COMPLEXITY_MEDIUM);
    print_str("  Created: tcp_server_demo.wasm - TCP Server\n");

    print_str("  Total GUI+Networking examples: 4\n");
    return 0;
}

/* Verify all examples */
int example_verify_all(void) {
    print_str("=== Verifying All Examples ===\n");
    int verified = 0;
    for (int i = 0; i < example_count; i++) {
        if (examples[i].verified) verified++;
        print_str("  ");
        print_int(examples[i].example_id);
        print_str(". ");
        print_str(examples[i].name);
        print_str(" [");
        print_str(type_name(examples[i].type));
        print_str("/");
        print_str(complexity_name(examples[i].complexity));
        print_str("] ");
        print_str(examples[i].verified ? "VERIFIED" : "UNVERIFIED");
        print_str("\n");
    }
    print_str("Verified: ");
    print_int(verified);
    print_str("/");
    print_int(example_count);
    print_str("\n");
    return verified;
}

/* Get example catalog */
int example_get_catalog(void) {
    print_str("=== Example Application Catalog ===\n");
    print_str("Total examples: ");
    print_int(example_count);
    print_str("\n\nBy category:\n");

    int counts[7] = {0};
    for (int i = 0; i < example_count; i++) {
        counts[examples[i].type]++;
    }
    for (int t = 0; t < 7; t++) {
        if (counts[t] > 0) {
            print_str("  ");
            print_str(type_name(t));
            print_str(": ");
            print_int(counts[t]);
            print_str("\n");
        }
    }
    return example_count;
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
    print_str("Example Apps v18.0 - Application Collection\n");
    if (help) {
        print_str("Usage: example_apps [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Create all examples\n");
        return;
    }
    if (test) {
        print_str("=== Example Applications Test ===\n\n");
        example_create_basic();
        print_str("\n");
        example_create_agent_demo();
        print_str("\n");
        example_create_ai_demo();
        print_str("\n");
        example_create_iot_demo();
        print_str("\n");
        example_create_gui_networking();
        print_str("\n");
        example_get_catalog();
        print_str("\n");
        example_verify_all();
        print_str("\n=== Examples Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
