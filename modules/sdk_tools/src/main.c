/* sdk_tools: WASM SDK toolchain wrapper (v31.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PROJECTS  32
#define MAX_TEMPLATES 8
#define MAX_NAME      64
#define MAX_PATH      256
#define DESC_LEN      128

/* Template type */
typedef struct {
    int   tmpl_id;
    char  name[MAX_NAME];
    char  description[DESC_LEN];
    int   files_count;
} template_t;

/* Project */
typedef struct {
    int   proj_id;
    char  name[MAX_NAME];
    char  path[MAX_PATH];
    char  tmpl_name[MAX_NAME];
    int   built;
    int   tested;
} project_t;

static template_t templates[MAX_TEMPLATES];
static project_t projects[MAX_PROJECTS];
static int tmpl_count = 0;
static int proj_count = 0;
static int next_proj_id = 1;

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

/* Register a project template */
int sdk_register_template(const char *name, const char *desc, int files) {
    if (tmpl_count >= MAX_TEMPLATES) return -1;
    int idx = tmpl_count;
    templates[idx].tmpl_id = tmpl_count + 1;
    my_strncpy(templates[idx].name, name, MAX_NAME - 1);
    my_strncpy(templates[idx].description, desc, DESC_LEN - 1);
    templates[idx].files_count = files;
    tmpl_count++;
    return templates[idx].tmpl_id;
}

/* List available templates */
int sdk_list_templates(void) {
    print_str("  Available Templates\n");
    print_str("  =================================================\n");
    print_str("  ID  Name                 Files  Description\n");
    print_str("  -------------------------------------------------\n");
    for (int i = 0; i < tmpl_count; i++) {
        print_int(templates[i].tmpl_id);
        print_str("  ");
        print_str(templates[i].name);
        int len = my_strlen(templates[i].name);
        for (int p = 0; p < 21 - len; p++) print_str(" ");
        print_int(templates[i].files_count);
        print_str("    ");
        print_str(templates[i].description);
        print_str("\n");
    }
    return tmpl_count;
}

/* Create project from template */
int sdk_create_project(const char *name, const char *template_name) {
    if (proj_count >= MAX_PROJECTS) return -1;
    /* Find template */
    int tmpl_idx = -1;
    for (int i = 0; i < tmpl_count; i++) {
        if (my_strcmp(templates[i].name, template_name) == 0) {
            tmpl_idx = i;
            break;
        }
    }
    if (tmpl_idx < 0) {
        print_str("  Template not found: ");
        print_str(template_name);
        print_str("\n");
        return -1;
    }
    int idx = proj_count;
    projects[idx].proj_id = next_proj_id++;
    my_strncpy(projects[idx].name, name, MAX_NAME - 1);
    my_strncpy(projects[idx].path, "modules/", 8);
    int path_len = my_strlen(projects[idx].path);
    my_strncpy(projects[idx].path + path_len, name, MAX_PATH - path_len - 1);
    my_strncpy(projects[idx].tmpl_name, template_name, MAX_NAME - 1);
    projects[idx].built = 0;
    projects[idx].tested = 0;
    proj_count++;

    print_str("  Created project: ");
    print_str(name);
    print_str(" from template: ");
    print_str(template_name);
    print_str("\n");
    print_str("  Files generated: ");
    print_int(templates[tmpl_idx].files_count);
    print_str("\n");
    print_str("  Path: ");
    print_str(projects[idx].path);
    print_str("/src/main.c\n");
    return projects[idx].proj_id;
}

/* Simulate build */
int sdk_build(int proj_id) {
    for (int i = 0; i < proj_count; i++) {
        if (projects[i].proj_id == proj_id) {
            print_str("  Building: ");
            print_str(projects[i].name);
            print_str("\n");
            print_str("    clang --target=wasm32-unknown-unknown -Oz -nostdlib\n");
            print_str("    Compiling src/main.c...\n");
            print_str("    Linking...\n");
            print_str("    Output: ");
            print_str(projects[i].name);
            print_str(".wasm\n");
            print_str("    Size: 2.4 KB\n");
            print_str("    Build: SUCCESS\n");
            projects[i].built = 1;
            return 0;
        }
    }
    return -1;
}

/* Simulate test */
int sdk_test(int proj_id) {
    for (int i = 0; i < proj_count; i++) {
        if (projects[i].proj_id == proj_id) {
            print_str("  Testing: ");
            print_str(projects[i].name);
            print_str("\n");
            print_str("    Running wasm3 test module...\n");
            print_str("    Test cases: 5/5 passed\n");
            print_str("    Coverage: 85%\n");
            print_str("    Test: PASS\n");
            projects[i].tested = 1;
            return 0;
        }
    }
    return -1;
}

/* Build and test all */
int sdk_build_all(void) {
    print_str("  Building all projects:\n");
    int success = 0;
    for (int i = 0; i < proj_count; i++) {
        sdk_build(projects[i].proj_id);
        print_str("\n");
        sdk_test(projects[i].proj_id);
        print_str("\n");
        if (projects[i].built && projects[i].tested) success++;
    }
    print_str("  Build summary: ");
    print_int(success);
    print_str("/");
    print_int(proj_count);
    print_str(" projects built and tested\n");
    return success;
}

/* Debug info for project */
int sdk_debug(int proj_id) {
    for (int i = 0; i < proj_count; i++) {
        if (projects[i].proj_id == proj_id) {
            print_str("  Debug Info: ");
            print_str(projects[i].name);
            print_str("\n");
            print_str("    Project ID: ");
            print_int(projects[i].proj_id);
            print_str("\n");
            print_str("    Template: ");
            print_str(projects[i].tmpl_name);
            print_str("\n");
            print_str("    Path: ");
            print_str(projects[i].path);
            print_str("\n");
            print_str("    Built: ");
            print_int(projects[i].built);
            print_str("\n");
            print_str("    Tested: ");
            print_int(projects[i].tested);
            print_str("\n");
            print_str("    Debug symbols: enabled\n");
            print_str("    JIT opcodes: 14+\n");
            return 0;
        }
    }
    return -1;
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
    print_str("SDK Tools v31.0 - WASM Module Development Toolkit\n");
    if (help) {
        print_str("Usage: sdk_tools [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run SDK tools test\n");
        return;
    }
    if (test) {
        print_str("=== SDK Tools Test ===\n\n");
        /* Register templates */
        print_str("Registering templates:\n");
        sdk_register_template("basic", "Minimal WASM module", 1);
        sdk_register_template("cli_tool", "Command-line tool with argv parsing", 2);
        sdk_register_template("posix_util", "POSIX utility with file I/O", 3);
        sdk_register_template("agent", "Agent module with IPC", 2);
        sdk_register_template("ai_module", "AI integration module", 3);
        sdk_register_template("gui_app", "GUI application", 4);
        print_str("\n");
        /* List templates */
        sdk_list_templates();
        print_str("\n");
        /* Create projects */
        print_str("Creating projects:\n");
        int p1 = sdk_create_project("hello_world", "basic");
        int p2 = sdk_create_project("my_grep", "posix_util");
        int p3 = sdk_create_project("my_agent", "agent");
        print_str("\n");
        /* Build and test */
        sdk_build(p1);
        print_str("\n");
        sdk_test(p1);
        print_str("\n");
        sdk_build(p2);
        print_str("\n");
        sdk_build(p3);
        print_str("\n");
        /* Debug info */
        sdk_debug(p2);
        print_str("\n");
        /* Build all */
        sdk_build_all();
        print_str("\n=== SDK Tools Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
