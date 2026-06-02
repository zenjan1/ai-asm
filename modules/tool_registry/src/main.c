/* tool_registry: Agent tool discovery and execution center (v40.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_TOOLS       128
#define MAX_CATEGORIES  32
#define MAX_NAME        64
#define MAX_DESC        256
#define MAX_MODULE      64
#define MAX_TAG         32
#define MAX_TAGS        8
#define MAX_SEARCH      64

/* Tool entry */
typedef struct {
    int   tool_id;
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    int   category_id;
    char  module_ref[MAX_MODULE];
    char  tags[MAX_TAGS][MAX_TAG];
    int   tag_count;
    int   call_count;
    int   success_count;
    int   success_rate;    /* * 100 */
    int   avg_latency_ms;
    int   version;
    int   enabled;
    int   created_at;
    int   updated_at;
} tool_entry_t;

/* Tool category */
typedef struct {
    int   category_id;
    char  name[MAX_NAME];
    char  description[MAX_DESC];
    int   parent_id;
    int   tool_ids[32];
    int   tool_count;
    int   subcategories[8];
    int   subcategory_count;
} tool_category_t;

/* Tool stats */
typedef struct {
    int   tool_id;
    int   total_calls;
    int   success_calls;
    int   error_calls;
    int   avg_latency_ms;
    int   last_called_at;
    int   last_success_at;
} tool_stats_t;

static tool_entry_t tools[MAX_TOOLS];
static tool_category_t categories[MAX_CATEGORIES];
static int tool_count = 0;
static int next_tool_id = 1;
static int category_count = 0;
static int next_category_id = 1;
static int clock = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
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

/* Tool registration */
int tr_register(const char *name, const char *description, int category_id, const char *module_ref) {
    if (tool_count >= MAX_TOOLS) return -1;
    int idx = tool_count++;
    tools[idx].tool_id = next_tool_id++;
    my_strncpy(tools[idx].name, name, MAX_NAME - 1);
    my_strncpy(tools[idx].description, description, MAX_DESC - 1);
    tools[idx].category_id = category_id;
    my_strncpy(tools[idx].module_ref, module_ref ? module_ref : "unknown", MAX_MODULE - 1);
    tools[idx].tag_count = 0;
    tools[idx].call_count = 0;
    tools[idx].success_count = 0;
    tools[idx].success_rate = 0;
    tools[idx].avg_latency_ms = 0;
    tools[idx].version = 1;
    tools[idx].enabled = 1;
    tools[idx].created_at = tools[idx].updated_at = ++clock;
    return tools[idx].tool_id;
}

int tr_unregister(int tool_id) {
    for (int i = 0; i < tool_count; i++) {
        if (tools[i].tool_id == tool_id) {
            tools[i].enabled = 0;
            return 0;
        }
    }
    return -1;
}

tool_entry_t *tr_get_tool(int tool_id) {
    for (int i = 0; i < tool_count; i++) {
        if (tools[i].tool_id == tool_id && tools[i].enabled) return &tools[i];
    }
    return NULL;
}

tool_entry_t *tr_find_by_name(const char *name) {
    for (int i = 0; i < tool_count; i++) {
        if (tools[i].enabled && my_strcmp(tools[i].name, name) == 0) return &tools[i];
    }
    return NULL;
}

/* Search tools by keyword */
int tr_search(const char *keyword, int *result_ids, int max_results) {
    int found = 0;
    for (int i = 0; i < tool_count && found < max_results; i++) {
        if (!tools[i].enabled) continue;
        if (my_strstr(tools[i].name, keyword) || my_strstr(tools[i].description, keyword)) {
            result_ids[found++] = tools[i].tool_id;
        } else {
            for (int t = 0; t < tools[i].tag_count; t++) {
                if (my_strstr(tools[i].tags[t], keyword)) {
                    result_ids[found++] = tools[i].tool_id;
                    break;
                }
            }
        }
    }
    return found;
}

/* Category management */
int tr_create_category(const char *name, const char *description, int parent_id) {
    if (category_count >= MAX_CATEGORIES) return -1;
    int idx = category_count++;
    categories[idx].category_id = next_category_id++;
    my_strncpy(categories[idx].name, name, MAX_NAME - 1);
    my_strncpy(categories[idx].description, description, MAX_DESC - 1);
    categories[idx].parent_id = parent_id;
    categories[idx].tool_count = 0;
    categories[idx].subcategory_count = 0;
    return categories[idx].category_id;
}

tool_category_t *tr_get_category(int category_id) {
    for (int i = 0; i < category_count; i++) {
        if (categories[i].category_id == category_id) return &categories[i];
    }
    return NULL;
}

int tr_list_categories(void) {
    print_str("  Tool Categories\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < category_count; i++) {
        print_str("  #"); print_int(categories[i].category_id);
        print_str(" "); print_str(categories[i].name);
        print_str(" ("); print_int(categories[i].tool_count);
        print_str(" tools)\n");
    }
    return category_count;
}

/* Tag management */
int tr_add_tag(int tool_id, const char *tag) {
    for (int i = 0; i < tool_count; i++) {
        if (tools[i].tool_id == tool_id) {
            if (tools[i].tag_count >= MAX_TAGS) return -1;
            int t = tools[i].tag_count++;
            my_strncpy(tools[i].tags[t], tag, MAX_TAG - 1);
            tools[i].updated_at = ++clock;
            return 0;
        }
    }
    return -1;
}

/* Update stats */
int tr_update_stats(int tool_id, int success, int latency_ms) {
    for (int i = 0; i < tool_count; i++) {
        if (tools[i].tool_id == tool_id) {
            tools[i].call_count++;
            if (success) tools[i].success_count++;
            int total_lat = tools[i].avg_latency_ms * (tools[i].call_count - 1) + latency_ms;
            tools[i].avg_latency_ms = tools[i].call_count > 0 ? total_lat / tools[i].call_count : latency_ms;
            tools[i].success_rate = tools[i].call_count > 0 ? (tools[i].success_count * 100) / tools[i].call_count : 0;
            return 0;
        }
    }
    return -1;
}

/* List tools */
int tr_list_tools(void) {
    print_str("  Registered Tools\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < tool_count; i++) {
        if (!tools[i].enabled) continue;
        print_str("  #"); print_int(tools[i].tool_id);
        print_str(" "); print_str(tools[i].name);
        print_str(" ["); print_str(tools[i].module_ref);
        print_str("] calls="); print_int(tools[i].call_count);
        print_str(" success="); print_int(tools[i].success_rate); print_str("%\n");
    }
    return tool_count;
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
    print_str("Tool Registry v40.0 - Agent Tool Discovery & Execution\n");
    if (help) { print_str("Usage: tool_registry [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Tool Registry Test ===\n\n");
        /* Create categories */
        int cat1 = tr_create_category("Mathematics", "Math calculation tools", 0);
        int cat2 = tr_create_category("Search", "Information search tools", 0);
        int cat3 = tr_create_category("Utility", "General utility tools", 0);
        print_str("Created "); print_int(category_count); print_str(" categories\n\n");
        tr_list_categories();
        print_str("\n");
        /* Register tools */
        int t1 = tr_register("calculator", "Perform mathematical calculations", cat1, "calc_module");
        int t2 = tr_register("web_search", "Search the web for information", cat2, "search_module");
        int t3 = tr_register("weather", "Get weather data for a location", cat2, "weather_module");
        int t4 = tr_register("timer", "Set and manage timers", cat3, "timer_module");
        print_str("Registered "); print_int(tool_count); print_str(" tools\n\n");
        /* Add tags */
        tr_add_tag(t1, "math"); tr_add_tag(t1, "calculate");
        tr_add_tag(t2, "search"); tr_add_tag(t2, "web");
        tr_add_tag(t3, "weather"); tr_add_tag(t4, "time");
        /* List tools */
        tr_list_tools();
        print_str("\n");
        /* Search */
        int results[MAX_SEARCH];
        int found = tr_search("search", results, MAX_SEARCH);
        print_str("Search 'search': found "); print_int(found); print_str(" tools\n");
        found = tr_search("math", results, MAX_SEARCH);
        print_str("Search 'math': found "); print_int(found); print_str(" tools\n\n");
        /* Simulate calls and update stats */
        tr_update_stats(t1, 1, 50);
        tr_update_stats(t1, 1, 30);
        tr_update_stats(t2, 1, 120);
        tr_update_stats(t2, 0, 200);
        tr_update_stats(t3, 1, 80);
        print_str("After simulating calls:\n");
        tr_list_tools();
        print_str("\n=== Tool Registry Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
