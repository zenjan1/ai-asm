/* plugin_framework: Dynamic plugin system with sandbox isolation (v32.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_PLUGINS    64
#define MAX_HOOKS      32
#define MAX_DEPS       8
#define MAX_NAME       64
#define MAX_DESC       128
#define MAX_VERSION    16

/* Plugin states */
#define PLUGIN_STOPPED   0
#define PLUGIN_LOADED    1
#define PLUGIN_RUNNING   2
#define PLUGIN_ERROR     3

/* Plugin lifecycle hooks */
#define HOOK_INIT       0
#define HOOK_START      1
#define HOOK_STOP       2
#define HOOK_UNLOAD     3
#define HOOK_CONFIG     4

/* Plugin entry */
typedef struct {
    int   plugin_id;
    char  name[MAX_NAME];
    char  version[MAX_VERSION];
    char  description[MAX_DESC];
    int   state;
    int   dep_count;
    int   deps[MAX_DEPS];    /* dependency plugin IDs */
    int   hook_count;
    int   hooks[MAX_HOOKS];  /* registered hook types */
    int   api_version;       /* API compatibility version */
    int   sandboxed;         /* 1=isolated, 0=trusted */
} plugin_t;

static plugin_t plugins[MAX_PLUGINS];
static int plugin_count = 0;
static int next_plugin_id = 1;
static int current_api_version = 1;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
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

static const char *state_name(int s) {
    if (s == PLUGIN_STOPPED) return "stopped";
    if (s == PLUGIN_LOADED) return "loaded";
    if (s == PLUGIN_RUNNING) return "running";
    return "error";
}

/* Register plugin */
int pf_register(const char *name, const char *version, const char *desc, int api_ver, int sandboxed) {
    if (plugin_count >= MAX_PLUGINS) return -1;
    if (api_ver != current_api_version) {
        print_str("  API version mismatch: ");
        print_int(api_ver);
        print_str(" != ");
        print_int(current_api_version);
        print_str("\n");
        return -1;
    }
    int idx = plugin_count;
    plugins[idx].plugin_id = next_plugin_id++;
    my_strncpy(plugins[idx].name, name, MAX_NAME - 1);
    my_strncpy(plugins[idx].version, version, MAX_VERSION - 1);
    my_strncpy(plugins[idx].description, desc, MAX_DESC - 1);
    plugins[idx].state = PLUGIN_STOPPED;
    plugins[idx].dep_count = 0;
    plugins[idx].hook_count = 0;
    plugins[idx].api_version = api_ver;
    plugins[idx].sandboxed = sandboxed;
    plugin_count++;
    return plugins[idx].plugin_id;
}

/* Add dependency */
int pf_add_dep(int plugin_id, int dep_id) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].plugin_id == plugin_id) {
            if (plugins[i].dep_count >= MAX_DEPS) return -1;
            plugins[i].deps[plugins[i].dep_count++] = dep_id;
            return 0;
        }
    }
    return -1;
}

/* Register hook */
int pf_register_hook(int plugin_id, int hook_type) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].plugin_id == plugin_id) {
            if (plugins[i].hook_count >= MAX_HOOKS) return -1;
            plugins[i].hooks[plugins[i].hook_count++] = hook_type;
            return 0;
        }
    }
    return -1;
}

/* Load plugin */
int pf_load(int plugin_id) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].plugin_id == plugin_id) {
            /* Check dependencies */
            for (int j = 0; j < plugins[i].dep_count; j++) {
                int dep_id = plugins[i].deps[j];
                int dep_running = 0;
                for (int k = 0; k < plugin_count; k++) {
                    if (plugins[k].plugin_id == dep_id && plugins[k].state == PLUGIN_RUNNING) {
                        dep_running = 1;
                        break;
                    }
                }
                if (!dep_running) {
                    print_str("  Dependency not running for: ");
                    print_str(plugins[i].name);
                    print_str("\n");
                    return -1;
                }
            }
            plugins[i].state = PLUGIN_LOADED;
            print_str("  Loaded: ");
            print_str(plugins[i].name);
            print_str(" v");
            print_str(plugins[i].version);
            if (plugins[i].sandboxed) print_str(" [sandboxed]");
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Start plugin */
int pf_start(int plugin_id) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].plugin_id == plugin_id) {
            if (plugins[i].state != PLUGIN_LOADED) return -1;
            plugins[i].state = PLUGIN_RUNNING;
            print_str("  Started: ");
            print_str(plugins[i].name);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* Stop plugin */
int pf_stop(int plugin_id) {
    for (int i = 0; i < plugin_count; i++) {
        if (plugins[i].plugin_id == plugin_id) {
            plugins[i].state = PLUGIN_STOPPED;
            print_str("  Stopped: ");
            print_str(plugins[i].name);
            print_str("\n");
            return 0;
        }
    }
    return -1;
}

/* List plugins */
int pf_list(void) {
    print_str("  Plugin Registry\n");
    print_str("  =================================================\n");
    print_str("  ID  Name                 Version   State       API\n");
    print_str("  -------------------------------------------------\n");
    for (int i = 0; i < plugin_count; i++) {
        print_int(plugins[i].plugin_id);
        print_str("  ");
        print_str(plugins[i].name);
        int len = my_strlen(plugins[i].name);
        for (int p = 0; p < 22 - len; p++) print_str(" ");
        print_str("v");
        print_str(plugins[i].version);
        print_str("    ");
        print_str(state_name(plugins[i].state));
        if (plugins[i].sandboxed) print_str("      ");
        else print_str("  ");
        print_str("v");
        print_int(plugins[i].api_version);
        print_str("\n");
    }
    return plugin_count;
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
    print_str("Plugin Framework v32.0 - Dynamic Plugin System with Sandbox\n");
    if (help) {
        print_str("Usage: plugin_framework [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run plugin test\n");
        return;
    }
    if (test) {
        print_str("=== Plugin Framework Test ===\n\n");
        /* Register plugins */
        int p1 = pf_register("logging", "1.0.0", "Structured logging plugin", 1, 0);
        int p2 = pf_register("metrics", "1.0.0", "Metrics collection plugin", 1, 0);
        int p3 = pf_register("auth_filter", "2.0.0", "Authentication filter", 1, 1);
        int p4 = pf_register("rate_limiter", "1.0.0", "Rate limiting plugin", 1, 1);
        print_str("\n");
        /* Add dependencies */
        pf_add_dep(p3, p1);  /* auth_filter depends on logging */
        pf_add_dep(p4, p1);
        /* Register hooks */
        pf_register_hook(p1, HOOK_INIT);
        pf_register_hook(p2, HOOK_CONFIG);
        pf_register_hook(p3, HOOK_START);
        pf_register_hook(p4, HOOK_STOP);
        /* List */
        pf_list();
        print_str("\n");
        /* Lifecycle */
        print_str("Starting plugins:\n");
        pf_load(p1);
        pf_start(p1);
        pf_load(p2);
        pf_start(p2);
        pf_load(p3);  /* depends on p1 (running) */
        pf_start(p3);
        pf_load(p4);
        pf_start(p4);
        print_str("\n");
        /* Stop one */
        print_str("Stopping metrics:\n");
        pf_stop(p2);
        print_str("\n");
        pf_list();
        print_str("\n=== Plugin Framework Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
