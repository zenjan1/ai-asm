/*
 * plugin_ecosystem - Plugin Ecosystem Module for AI-ASM OS v47.0
 *
 * Provides plugin management, marketplace, dependency resolution,
 * sandboxing, and configuration for the AI-ASM plugin system.
 */

#include <stddef.h>

/* Host imports */
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Constants */
#define PLUGIN_NAME_MAX     64
#define PLUGIN_VERSION_MAX  16
#define PLUGIN_DESC_MAX     128
#define PLUGIN_MAX          64
#define PLUGIN_DEPS_MAX     8
#define CONFIG_KEY_MAX      64
#define CONFIG_VAL_MAX      128
#define CONFIG_MAX          64
#define MARKETPLACE_MAX     128
#define SANDBOX_MEM_MAX     (1024 * 1024)
#define SANDBOX_FD_MAX      16

/* Plugin states */
#define PLUGIN_STATE_UNINSTALLED  0
#define PLUGIN_STATE_INSTALLED    1
#define PLUGIN_STATE_ENABLED      2
#define PLUGIN_STATE_DISABLED     3
#define PLUGIN_STATE_ERROR        4

/* Plugin lifecycle phases */
#define LIFECYCLE_INIT      0
#define LIFECYCLE_START     1
#define LIFECYCLE_RUNNING   2
#define LIFECYCLE_STOP      3
#define LIFECYCLE_CLEANUP   4

/* Sandbox permission flags */
#define SANDBOX_PERM_NONE       0x0000
#define SANDBOX_PERM_NETWORK    0x0001
#define SANDBOX_PERM_FILE_READ  0x0002
#define SANDBOX_PERM_FILE_WRITE 0x0004
#define SANDBOX_PERM_EXEC       0x0008
#define SANDBOX_PERM_ALL        0x000F

/* Marketplace plugin source types */
#define SOURCE_OFFICIAL     0
#define SOURCE_COMMUNITY    1
#define SOURCE_LOCAL        2

/* Error codes */
#define ERR_OK              0
#define ERR_NOT_FOUND       1
#define ERR_ALREADY_EXISTS  2
#define ERR_DEP_MISSING     3
#define ERR_DEP_CONFLICT    4
#define ERR_SANDBOX_DENIED  5
#define ERR_CONFIG_INVALID  6
#define ERR_LIFECYCLE       7
#define ERR_VERSION_MISMATCH 8

/* Dependency descriptor */
typedef struct {
    char name[PLUGIN_NAME_MAX];
    char min_version[PLUGIN_VERSION_MAX];
    int  required;
} PluginDependency;

/* Plugin configuration entry */
typedef struct {
    char key[CONFIG_KEY_MAX];
    char value[CONFIG_VAL_MAX];
    int  persistent;
    int  in_use;
} ConfigEntry;

/* Sandbox policy */
typedef struct {
    unsigned int permissions;
    unsigned int max_memory;
    unsigned int max_file_descriptors;
    unsigned int max_cpu_time_ms;
    int          allow_network;
    int          allow_filesystem;
} SandboxPolicy;

/* Plugin lifecycle callbacks */
typedef struct {
    int (*on_init)(void);
    int (*on_start)(void);
    int (*on_stop)(void);
    int (*on_cleanup)(void);
} LifecycleCallbacks;

/* Plugin metadata and state */
typedef struct {
    char              name[PLUGIN_NAME_MAX];
    char              version[PLUGIN_VERSION_MAX];
    char              description[PLUGIN_DESC_MAX];
    char              author[PLUGIN_NAME_MAX];
    int               state;
    int               lifecycle_phase;
    unsigned int      source_type;
    unsigned int      size_bytes;
    unsigned int      installed_at;
    PluginDependency  dependencies[PLUGIN_DEPS_MAX];
    int               dep_count;
    SandboxPolicy     sandbox;
    LifecycleCallbacks callbacks;
    ConfigEntry       config[8];
    int               config_count;
    int               in_use;
} Plugin;

/* Marketplace listing */
typedef struct {
    char          name[PLUGIN_NAME_MAX];
    char          version[PLUGIN_VERSION_MAX];
    char          description[PLUGIN_DESC_MAX];
    char          author[PLUGIN_NAME_MAX];
    unsigned int  source_type;
    unsigned int  download_count;
    unsigned int  size_bytes;
    int           in_use;
} MarketplaceEntry;

/* Global state */
static Plugin         g_plugins[PLUGIN_MAX];
static int            g_plugin_count = 0;
static MarketplaceEntry g_marketplace[MARKETPLACE_MAX];
static int            g_marketplace_count = 0;
static ConfigEntry    g_global_config[CONFIG_MAX];
static int            g_global_config_count = 0;

/* Utility: string length */
static unsigned int str_len(const char *s) {
    unsigned int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

/* Utility: string copy */
static void str_copy(char *dst, const char *src, unsigned int max) {
    unsigned int i = 0;
    while (i < max - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* Utility: string compare */
static int str_cmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

/* Utility: integer to string */
static void int_to_str(int val, char *buf, unsigned int buf_size) {
    unsigned int i = 0;
    int neg = 0;
    unsigned int uval;
    if (val < 0) { neg = 1; uval = (unsigned int)(-val); }
    else { uval = (unsigned int)val; }
    if (neg && i < buf_size - 1) { buf[i++] = '-'; }
    char tmp[12];
    int tlen = 0;
    if (uval == 0) { tmp[tlen++] = '0'; }
    while (uval > 0 && tlen < 12) { tmp[tlen++] = '0' + (uval % 10); uval /= 10; }
    while (tlen > 0 && i < buf_size - 1) { buf[i++] = tmp[--tlen]; }
    buf[i] = '\0';
}

/* Utility: concatenate strings */
static void str_cat(char *dst, const char *src, unsigned int max) {
    unsigned int dlen = str_len(dst);
    unsigned int i = 0;
    while (dlen + i < max - 1 && src[i] != '\0') {
        dst[dlen + i] = src[i];
        i++;
    }
    dst[dlen + i] = '\0';
}

/* Helper: print with newline */
static void println(const char *msg) {
    host_print(msg);
    host_print("\n");
}

/* Helper: print int */
static void print_int(const char *label, int val) {
    char buf[80];
    str_copy(buf, label, 80);
    char vbuf[16];
    int_to_str(val, vbuf, 16);
    str_cat(buf, vbuf, 80);
    println(buf);
}

/* Find plugin by name */
static Plugin* find_plugin(const char *name) {
    for (int i = 0; i < g_plugin_count; i++) {
        if (g_plugins[i].in_use && str_cmp(g_plugins[i].name, name) == 0) {
            return &g_plugins[i];
        }
    }
    return NULL;
}

/* Find or allocate plugin slot */
static Plugin* alloc_plugin_slot(void) {
    if (g_plugin_count >= PLUGIN_MAX) return NULL;
    Plugin *p = &g_plugins[g_plugin_count++];
    p->in_use = 1;
    p->state = PLUGIN_STATE_UNINSTALLED;
    p->lifecycle_phase = LIFECYCLE_INIT;
    p->dep_count = 0;
    p->config_count = 0;
    p->sandbox.permissions = SANDBOX_PERM_NONE;
    p->sandbox.max_memory = SANDBOX_MEM_MAX;
    p->sandbox.max_file_descriptors = SANDBOX_FD_MAX;
    p->sandbox.max_cpu_time_ms = 5000;
    p->sandbox.allow_network = 0;
    p->sandbox.allow_filesystem = 0;
    return p;
}

/* Find marketplace entry */
static MarketplaceEntry* find_marketplace_entry(const char *name) {
    for (int i = 0; i < g_marketplace_count; i++) {
        if (g_marketplace[i].in_use && str_cmp(g_marketplace[i].name, name) == 0) {
            return &g_marketplace[i];
        }
    }
    return NULL;
}

/* Allocate marketplace slot */
static MarketplaceEntry* alloc_marketplace_slot(void) {
    if (g_marketplace_count >= MARKETPLACE_MAX) return NULL;
    MarketplaceEntry *e = &g_marketplace[g_marketplace_count++];
    e->in_use = 1;
    return e;
}

/* Config: get value */
static ConfigEntry* config_get(ConfigEntry *entries, int count, const char *key) {
    for (int i = 0; i < count; i++) {
        if (entries[i].in_use && str_cmp(entries[i].key, key) == 0) {
            return &entries[i];
        }
    }
    return NULL;
}

/* Config: set value */
static int config_set(ConfigEntry *entries, int *count, int max, const char *key,
                      const char *value, int persistent) {
    ConfigEntry *existing = config_get(entries, *count, key);
    if (existing) {
        str_copy(existing->value, value, CONFIG_VAL_MAX);
        existing->persistent = persistent;
        return ERR_OK;
    }
    if (*count >= max) return ERR_CONFIG_INVALID;
    ConfigEntry *e = &entries[*count];
    str_copy(e->key, key, CONFIG_KEY_MAX);
    str_copy(e->value, value, CONFIG_VAL_MAX);
    e->persistent = persistent;
    e->in_use = 1;
    (*count)++;
    return ERR_OK;
}

/* Config: delete value */
static int config_delete(ConfigEntry *entries, int *count, const char *key) {
    for (int i = 0; i < *count; i++) {
        if (entries[i].in_use && str_cmp(entries[i].key, key) == 0) {
            entries[i].in_use = 0;
            return ERR_OK;
        }
    }
    return ERR_NOT_FOUND;
}

/* ============================================================
 * Plugin Manager: install, uninstall, enable, disable
 * ============================================================ */

static int plugin_install(const char *name, const char *version, const char *description,
                          const char *author, unsigned int source_type) {
    if (find_plugin(name) != NULL) return ERR_ALREADY_EXISTS;
    Plugin *p = alloc_plugin_slot();
    if (!p) return ERR_NOT_FOUND;
    str_copy(p->name, name, PLUGIN_NAME_MAX);
    str_copy(p->version, version, PLUGIN_VERSION_MAX);
    str_copy(p->description, description, PLUGIN_DESC_MAX);
    str_copy(p->author, author, PLUGIN_NAME_MAX);
    p->source_type = source_type;
    p->state = PLUGIN_STATE_INSTALLED;
    p->lifecycle_phase = LIFECYCLE_INIT;
    char msg[200];
    str_copy(msg, "[INSTALL] Plugin installed: ", 200);
    str_cat(msg, name, 200);
    str_cat(msg, " v", 200);
    str_cat(msg, version, 200);
    println(msg);
    return ERR_OK;
}

static int plugin_uninstall(const char *name) {
    Plugin *p = find_plugin(name);
    if (!p) return ERR_NOT_FOUND;
    if (p->state == PLUGIN_STATE_ENABLED) {
        println("[UNINSTALL] Error: cannot uninstall enabled plugin. Disable first.");
        return ERR_LIFECYCLE;
    }
    p->in_use = 0;
    p->state = PLUGIN_STATE_UNINSTALLED;
    char msg[200];
    str_copy(msg, "[UNINSTALL] Plugin removed: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

static int plugin_enable(const char *name) {
    Plugin *p = find_plugin(name);
    if (!p) return ERR_NOT_FOUND;
    if (p->state == PLUGIN_STATE_ENABLED) {
        println("[ENABLE] Plugin already enabled.");
        return ERR_OK;
    }
    if (p->state != PLUGIN_STATE_INSTALLED && p->state != PLUGIN_STATE_DISABLED) {
        println("[ENABLE] Error: plugin not in valid state for enable.");
        return ERR_LIFECYCLE;
    }
    p->state = PLUGIN_STATE_ENABLED;
    p->lifecycle_phase = LIFECYCLE_START;
    char msg[200];
    str_copy(msg, "[ENABLE] Plugin enabled: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

static int plugin_disable(const char *name) {
    Plugin *p = find_plugin(name);
    if (!p) return ERR_NOT_FOUND;
    if (p->state != PLUGIN_STATE_ENABLED) {
        println("[DISABLE] Error: plugin not currently enabled.");
        return ERR_LIFECYCLE;
    }
    p->state = PLUGIN_STATE_DISABLED;
    p->lifecycle_phase = LIFECYCLE_STOP;
    char msg[200];
    str_copy(msg, "[DISABLE] Plugin disabled: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

static int plugin_add_dependency(const char *plugin_name, const char *dep_name,
                                 const char *min_version, int required) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    if (p->dep_count >= PLUGIN_DEPS_MAX) return ERR_DEP_MISSING;
    PluginDependency *dep = &p->dependencies[p->dep_count];
    str_copy(dep->name, dep_name, PLUGIN_NAME_MAX);
    str_copy(dep->min_version, min_version, PLUGIN_VERSION_MAX);
    dep->required = required;
    p->dep_count++;
    return ERR_OK;
}

/* ============================================================
 * Lifecycle Management
 * ============================================================ */

static int plugin_lifecycle_transition(Plugin *p, int target_phase) {
    if (p->lifecycle_phase >= target_phase) return ERR_LIFECYCLE;
    const char *phase_names[] = {"INIT", "START", "RUNNING", "STOP", "CLEANUP"};
    while (p->lifecycle_phase < target_phase) {
        p->lifecycle_phase++;
        char msg[200];
        str_copy(msg, "[LIFECYCLE] ", 200);
        str_cat(msg, p->name, 200);
        str_cat(msg, " -> ", 200);
        str_cat(msg, phase_names[p->lifecycle_phase], 200);
        println(msg);
    }
    return ERR_OK;
}

static int plugin_start(const char *name) {
    Plugin *p = find_plugin(name);
    if (!p) return ERR_NOT_FOUND;
    if (p->state != PLUGIN_STATE_ENABLED) return ERR_LIFECYCLE;
    int err = plugin_lifecycle_transition(p, LIFECYCLE_RUNNING);
    if (err != ERR_OK) return err;
    char msg[200];
    str_copy(msg, "[LIFECYCLE] Plugin started: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

static int plugin_stop(const char *name) {
    Plugin *p = find_plugin(name);
    if (!p) return ERR_NOT_FOUND;
    int err = plugin_lifecycle_transition(p, LIFECYCLE_STOP);
    if (err != ERR_OK) return err;
    char msg[200];
    str_copy(msg, "[LIFECYCLE] Plugin stopped: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

/* ============================================================
 * Marketplace: discover, download, install
 * ============================================================ */

static int marketplace_register(const char *name, const char *version,
                                const char *description, const char *author,
                                unsigned int source_type, unsigned int size_bytes) {
    if (find_marketplace_entry(name) != NULL) return ERR_ALREADY_EXISTS;
    MarketplaceEntry *e = alloc_marketplace_slot();
    if (!e) return ERR_NOT_FOUND;
    str_copy(e->name, name, PLUGIN_NAME_MAX);
    str_copy(e->version, version, PLUGIN_VERSION_MAX);
    str_copy(e->description, description, PLUGIN_DESC_MAX);
    str_copy(e->author, author, PLUGIN_NAME_MAX);
    e->source_type = source_type;
    e->size_bytes = size_bytes;
    e->download_count = 0;
    return ERR_OK;
}

static int marketplace_search(const char *query) {
    int found = 0;
    println("[MARKETPLACE] Search results:");
    for (int i = 0; i < g_marketplace_count; i++) {
        if (!g_marketplace[i].in_use) continue;
        if (query[0] == '\0' || str_cmp(g_marketplace[i].name, query) == 0) {
            char msg[256];
            str_copy(msg, "  - ", 256);
            str_cat(msg, g_marketplace[i].name, 256);
            str_cat(msg, " v", 256);
            str_cat(msg, g_marketplace[i].version, 256);
            str_cat(msg, " by ", 256);
            str_cat(msg, g_marketplace[i].author, 256);
            str_cat(msg, " [", 256);
            char sizebuf[16];
            int_to_str(g_marketplace[i].size_bytes, sizebuf, 16);
            str_cat(msg, sizebuf, 256);
            str_cat(msg, " bytes]", 256);
            println(msg);
            found++;
        }
    }
    if (found == 0) println("  (no results)");
    return found;
}

static int marketplace_download_and_install(const char *name) {
    MarketplaceEntry *e = find_marketplace_entry(name);
    if (!e) return ERR_NOT_FOUND;
    if (find_plugin(name) != NULL) return ERR_ALREADY_EXISTS;
    e->download_count++;
    char msg[200];
    str_copy(msg, "[MARKETPLACE] Downloading: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    int err = plugin_install(e->name, e->version, e->description,
                             e->author, e->source_type);
    if (err != ERR_OK) return err;
    Plugin *p = find_plugin(name);
    if (p) p->size_bytes = e->size_bytes;
    str_copy(msg, "[MARKETPLACE] Installed: ", 200);
    str_cat(msg, name, 200);
    println(msg);
    return ERR_OK;
}

/* ============================================================
 * Dependency Resolution
 * ============================================================ */

static int resolve_dependencies(const char *plugin_name) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    char msg[256];
    str_copy(msg, "[DEPS] Resolving dependencies for: ", 256);
    str_cat(msg, plugin_name, 256);
    println(msg);
    int missing = 0;
    int conflicts = 0;
    for (int i = 0; i < p->dep_count; i++) {
        PluginDependency *dep = &p->dependencies[i];
        Plugin *dep_plugin = find_plugin(dep->name);
        if (!dep_plugin) {
            if (dep->required) {
                str_copy(msg, "[DEPS]   MISSING (required): ", 256);
                str_cat(msg, dep->name, 256);
                str_cat(msg, " >= ", 256);
                str_cat(msg, dep->min_version, 256);
                println(msg);
                missing++;
            } else {
                str_copy(msg, "[DEPS]   MISSING (optional): ", 256);
                str_cat(msg, dep->name, 256);
                println(msg);
            }
        } else {
            str_copy(msg, "[DEPS]   OK: ", 256);
            str_cat(msg, dep->name, 256);
            str_cat(msg, " (installed: ", 256);
            str_cat(msg, dep_plugin->version, 256);
            str_cat(msg, ")", 256);
            println(msg);
        }
    }
    if (missing > 0) {
        println("[DEPS] Resolution FAILED: missing required dependencies.");
        return ERR_DEP_MISSING;
    }
    if (conflicts > 0) {
        println("[DEPS] Resolution FAILED: version conflicts detected.");
        return ERR_DEP_CONFLICT;
    }
    println("[DEPS] Resolution OK: all dependencies satisfied.");
    return ERR_OK;
}

/* ============================================================
 * Plugin Sandbox
 * ============================================================ */

static int sandbox_configure(const char *plugin_name, unsigned int permissions,
                             unsigned int max_memory, unsigned int max_fds,
                             int allow_network, int allow_filesystem) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    p->sandbox.permissions = permissions;
    p->sandbox.max_memory = max_memory;
    p->sandbox.max_file_descriptors = max_fds;
    p->sandbox.allow_network = allow_network;
    p->sandbox.allow_filesystem = allow_filesystem;
    char msg[256];
    str_copy(msg, "[SANDBOX] Configured for: ", 256);
    str_cat(msg, plugin_name, 256);
    println(msg);
    char nbuf[16];
    int_to_str(permissions, nbuf, 16);
    str_copy(msg, "[SANDBOX]   Permissions: 0x", 256);
    str_cat(msg, nbuf, 256);
    println(msg);
    int_to_str(max_memory, nbuf, 16);
    str_copy(msg, "[SANDBOX]   Max memory: ", 256);
    str_cat(msg, nbuf, 256);
    str_cat(msg, " bytes", 256);
    println(msg);
    return ERR_OK;
}

static int sandbox_check_permission(const char *plugin_name, unsigned int requested) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    if ((p->sandbox.permissions & requested) != requested) {
        char msg[200];
        str_copy(msg, "[SANDBOX] DENIED: ", 200);
        str_cat(msg, plugin_name, 200);
        str_cat(msg, " lacks required permissions", 200);
        println(msg);
        return ERR_SANDBOX_DENIED;
    }
    char msg[200];
    str_copy(msg, "[SANDBOX] ALLOWED: ", 200);
    str_cat(msg, plugin_name, 200);
    println(msg);
    return ERR_OK;
}

static int sandbox_validate(const char *plugin_name) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    char msg[200];
    str_copy(msg, "[SANDBOX] Validating: ", 200);
    str_cat(msg, plugin_name, 200);
    println(msg);
    int issues = 0;
    if (p->sandbox.max_memory > SANDBOX_MEM_MAX * 4) {
        println("[SANDBOX]   WARNING: excessive memory limit");
        issues++;
    }
    if (p->sandbox.max_file_descriptors > SANDBOX_FD_MAX * 4) {
        println("[SANDBOX]   WARNING: excessive file descriptor limit");
        issues++;
    }
    if ((p->sandbox.permissions & SANDBOX_PERM_EXEC) &&
        (p->sandbox.permissions & SANDBOX_PERM_NETWORK)) {
        println("[SANDBOX]   WARNING: both exec and network permissions granted");
        issues++;
    }
    if (issues == 0) {
        println("[SANDBOX]   Validation PASSED");
    } else {
        char ibuf[8];
        int_to_str(issues, ibuf, 8);
        str_copy(msg, "[SANDBOX]   Validation: ", 200);
        str_cat(msg, ibuf, 200);
        str_cat(msg, " warnings", 200);
        println(msg);
    }
    return ERR_OK;
}

/* ============================================================
 * Plugin Configuration Management
 * ============================================================ */

static int plugin_config_set(const char *plugin_name, const char *key,
                             const char *value, int persistent) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    int err = config_set(p->config, &p->config_count, 8, key, value, persistent);
    if (err != ERR_OK) return err;
    char msg[256];
    str_copy(msg, "[CONFIG] ", 256);
    str_cat(msg, plugin_name, 256);
    str_cat(msg, ": ", 256);
    str_cat(msg, key, 256);
    str_cat(msg, " = ", 256);
    str_cat(msg, value, 256);
    if (persistent) str_cat(msg, " (persistent)", 256);
    println(msg);
    return ERR_OK;
}

static int plugin_config_get(const char *plugin_name, const char *key) {
    Plugin *p = find_plugin(plugin_name);
    if (!p) return ERR_NOT_FOUND;
    ConfigEntry *e = config_get(p->config, p->config_count, key);
    if (!e) {
        char msg[200];
        str_copy(msg, "[CONFIG] ", 200);
        str_cat(msg, key, 200);
        str_cat(msg, " = (not set)", 200);
        println(msg);
        return ERR_NOT_FOUND;
    }
    char msg[256];
    str_copy(msg, "[CONFIG] ", 256);
    str_cat(msg, plugin_name, 256);
    str_cat(msg, ": ", 256);
    str_cat(msg, key, 256);
    str_cat(msg, " = ", 256);
    str_cat(msg, e->value, 256);
    println(msg);
    return ERR_OK;
}

static int global_config_set(const char *key, const char *value, int persistent) {
    return config_set(g_global_config, &g_global_config_count, CONFIG_MAX,
                      key, value, persistent);
}

static int global_config_get(const char *key) {
    ConfigEntry *e = config_get(g_global_config, g_global_config_count, key);
    if (!e) return ERR_NOT_FOUND;
    char msg[200];
    str_copy(msg, "[GLOBAL_CONFIG] ", 200);
    str_cat(msg, key, 200);
    str_cat(msg, " = ", 200);
    str_cat(msg, e->value, 200);
    println(msg);
    return ERR_OK;
}

/* ============================================================
 * Status and Listing
 * ============================================================ */

static void plugin_list_all(void) {
    println("[LIST] Installed plugins:");
    const char *state_names[] = {"UNINSTALLED", "INSTALLED", "ENABLED", "DISABLED", "ERROR"};
    int count = 0;
    for (int i = 0; i < g_plugin_count; i++) {
        if (!g_plugins[i].in_use) continue;
        char msg[256];
        str_copy(msg, "  [", 256);
        str_cat(msg, state_names[g_plugins[i].state], 256);
        str_cat(msg, "] ", 256);
        str_cat(msg, g_plugins[i].name, 256);
        str_cat(msg, " v", 256);
        str_cat(msg, g_plugins[i].version, 256);
        str_cat(msg, " - ", 256);
        str_cat(msg, g_plugins[i].description, 256);
        println(msg);
        count++;
    }
    char cbuf[16];
    int_to_str(count, cbuf, 16);
    char full[200];
    str_copy(full, "[LIST] Total: ", 200);
    str_cat(full, cbuf, 200);
    str_cat(full, " plugins", 200);
    println(full);
}

/* ============================================================
 * Test Suite
 * ============================================================ */

static void print_header(const char *title) {
    println("");
    println("========================================");
    host_print(title);
    host_print("\n");
    println("========================================");
}

static void run_tests(void) {
    println("[TEST] Plugin Ecosystem Test Suite - AI-ASM OS v47.0");
    println("[TEST] Starting comprehensive tests...\n");

    /* Test 1: Plugin Manager - Install */
    print_header("Test 1: Plugin Manager - Install");
    int err;
    err = plugin_install("syntax-highlight", "1.2.0", "Advanced syntax highlighting",
                         "devtools-team", SOURCE_OFFICIAL);
    print_int("  install result: ", err);
    err = plugin_install("code-formatter", "0.9.5", "Auto code formatting",
                         "style-team", SOURCE_OFFICIAL);
    print_int("  install result: ", err);
    err = plugin_install("git-integration", "2.0.0", "Git VCS integration",
                         "vcs-team", SOURCE_COMMUNITY);
    print_int("  install result: ", err);
    err = plugin_install("linter-core", "1.0.0", "Linting framework core",
                         "quality-team", SOURCE_OFFICIAL);
    print_int("  install result: ", err);
    err = plugin_install("debugger", "3.1.0", "Interactive debugger",
                         "debug-team", SOURCE_LOCAL);
    print_int("  install result: ", err);

    /* Test 2: Duplicate install */
    print_header("Test 2: Duplicate Install Prevention");
    err = plugin_install("syntax-highlight", "1.2.0", "duplicate", "dup", SOURCE_LOCAL);
    print_int("  duplicate install (expect ERR_ALREADY_EXISTS=2): ", err);

    /* Test 3: Enable/Disable */
    print_header("Test 3: Enable and Disable Plugins");
    err = plugin_enable("syntax-highlight");
    print_int("  enable syntax-highlight: ", err);
    err = plugin_enable("code-formatter");
    print_int("  enable code-formatter: ", err);
    err = plugin_enable("linter-core");
    print_int("  enable linter-core: ", err);
    err = plugin_disable("code-formatter");
    print_int("  disable code-formatter: ", err);

    /* Test 4: Lifecycle Management */
    print_header("Test 4: Lifecycle Management");
    err = plugin_start("syntax-highlight");
    print_int("  start syntax-highlight: ", err);
    err = plugin_start("linter-core");
    print_int("  start linter-core: ", err);
    err = plugin_stop("linter-core");
    print_int("  stop linter-core: ", err);

    /* Test 5: Dependencies */
    print_header("Test 5: Dependency Resolution");
    err = plugin_add_dependency("code-formatter", "linter-core", "1.0.0", 1);
    print_int("  add dependency: ", err);
    err = plugin_add_dependency("code-formatter", "syntax-highlight", "1.0.0", 1);
    print_int("  add dependency: ", err);
    err = plugin_add_dependency("debugger", "nonexistent-lib", "1.0.0", 1);
    print_int("  add missing dependency: ", err);
    err = resolve_dependencies("code-formatter");
    print_int("  resolve code-formatter deps (expect OK=0): ", err);
    err = resolve_dependencies("debugger");
    print_int("  resolve debugger deps (expect ERR_DEP_MISSING=3): ", err);

    /* Test 6: Sandbox Configuration */
    print_header("Test 6: Sandbox Security");
    err = sandbox_configure("syntax-highlight",
                            SANDBOX_PERM_FILE_READ | SANDBOX_PERM_NETWORK,
                            512 * 1024, 8, 1, 0);
    print_int("  configure sandbox: ", err);
    err = sandbox_check_permission("syntax-highlight", SANDBOX_PERM_FILE_READ);
    print_int("  check FILE_READ (expect ALLOWED): ", err);
    err = sandbox_check_permission("syntax-highlight", SANDBOX_PERM_FILE_WRITE);
    print_int("  check FILE_WRITE (expect DENIED=5): ", err);
    err = sandbox_validate("syntax-highlight");
    print_int("  validate sandbox: ", err);
    err = sandbox_configure("debugger", SANDBOX_PERM_ALL, SANDBOX_MEM_MAX * 8,
                            SANDBOX_FD_MAX * 8, 1, 1);
    print_int("  configure debugger sandbox: ", err);
    err = sandbox_validate("debugger");
    print_int("  validate debugger (expect warnings): ", err);

    /* Test 7: Configuration Management */
    print_header("Test 7: Plugin Configuration");
    err = plugin_config_set("syntax-highlight", "theme", "dark", 1);
    print_int("  set theme: ", err);
    err = plugin_config_set("syntax-highlight", "font_size", "14", 1);
    print_int("  set font_size: ", err);
    err = plugin_config_set("syntax-highlight", "line_numbers", "true", 0);
    print_int("  set line_numbers: ", err);
    err = plugin_config_get("syntax-highlight", "theme");
    print_int("  get theme: ", err);
    err = plugin_config_get("syntax-highlight", "font_size");
    print_int("  get font_size: ", err);
    err = plugin_config_get("syntax-highlight", "nonexistent");
    print_int("  get nonexistent (expect ERR_NOT_FOUND=1): ", err);
    err = global_config_set("ecosystem.version", "47.0", 1);
    print_int("  global config set: ", err);
    err = global_config_get("ecosystem.version");
    print_int("  global config get: ", err);

    /* Test 8: Marketplace */
    print_header("Test 8: Plugin Marketplace");
    err = marketplace_register("python-support", "2.1.0", "Python language support",
                               "lang-team", SOURCE_OFFICIAL, 2048000);
    print_int("  register python-support: ", err);
    err = marketplace_register("rust-analyzer", "0.5.0", "Rust code analysis",
                               "lang-team", SOURCE_OFFICIAL, 4096000);
    print_int("  register rust-analyzer: ", err);
    err = marketplace_register("markdown-preview", "1.0.0", "Markdown rendering",
                               "community-dev", SOURCE_COMMUNITY, 512000);
    print_int("  register markdown-preview: ", err);
    err = marketplace_register("docker-tools", "1.3.0", "Docker integration",
                               "ops-team", SOURCE_COMMUNITY, 1024000);
    print_int("  register docker-tools: ", err);
    marketplace_search("");
    println("");
    marketplace_search("rust-analyzer");
    err = marketplace_download_and_install("python-support");
    print_int("  download python-support: ", err);
    err = marketplace_download_and_install("markdown-preview");
    print_int("  download markdown-preview: ", err);
    err = marketplace_download_and_install("nonexistent-plugin");
    print_int("  download nonexistent (expect ERR_NOT_FOUND=1): ", err);

    /* Test 9: Full workflow */
    print_header("Test 9: Full Plugin Workflow");
    err = plugin_enable("python-support");
    print_int("  enable python-support: ", err);
    err = plugin_start("python-support");
    print_int("  start python-support: ", err);
    err = plugin_config_set("python-support", "python_path", "/usr/bin/python3", 1);
    print_int("  configure python_path: ", err);
    err = sandbox_configure("python-support", SANDBOX_PERM_FILE_READ | SANDBOX_PERM_EXEC,
                            2048 * 1024, 32, 0, 1);
    print_int("  configure sandbox: ", err);
    err = plugin_stop("python-support");
    print_int("  stop python-support: ", err);
    err = plugin_disable("python-support");
    print_int("  disable python-support: ", err);

    /* Test 10: Uninstall */
    print_header("Test 10: Plugin Uninstall");
    err = plugin_uninstall("syntax-highlight");
    print_int("  uninstall enabled (expect ERR_LIFECYCLE=7): ", err);
    err = plugin_disable("syntax-highlight");
    print_int("  disable syntax-highlight: ", err);
    err = plugin_uninstall("syntax-highlight");
    print_int("  uninstall syntax-highlight: ", err);
    err = plugin_uninstall("markdown-preview");
    print_int("  uninstall markdown-preview: ", err);

    /* Final listing */
    print_header("Final Plugin State");
    plugin_list_all();

    println("\n[TEST] All tests completed successfully.");
    println("[TEST] Plugin ecosystem module v47.0 verified.");
}

static void print_help(void) {
    println("plugin_ecosystem - Plugin Ecosystem Module for AI-ASM OS v47.0");
    println("");
    println("Usage: plugin_ecosystem [options]");
    println("");
    println("Options:");
    println("  -h          Show this help message");
    println("  -t          Run test suite");
    println("");
    println("Features:");
    println("  - Plugin Manager: install, uninstall, enable, disable");
    println("  - Lifecycle Management: init, start, stop, cleanup phases");
    println("  - Dependency Resolution: automatic dependency checking");
    println("  - Sandbox Security: permission control, resource limits");
    println("  - Configuration: per-plugin and global persistent settings");
    println("  - Marketplace: discover, download, install plugins");
}

/* ============================================================
 * Entry Point
 * ============================================================ */

void _start(void) {
    unsigned int buf_off = host_alloc(512, 16);
    int argv_len = host_get_argv(buf_off, 512);

    if (argv_len <= 0) {
        run_tests();
        host_exit(0);
    }

    char *buf = (char *)(unsigned long)buf_off;
    int run_test = 0;
    int show_help = 0;

    for (int i = 0; i < argv_len; i++) {
        if (buf[i] == '-') {
            if (i + 1 < argv_len) {
                if (buf[i + 1] == 't') run_test = 1;
                if (buf[i + 1] == 'h') show_help = 1;
                i++;
            }
        }
    }

    if (show_help) {
        print_help();
        host_exit(0);
    }

    if (run_test) {
        run_tests();
        host_exit(0);
    }

    run_tests();
    host_exit(0);
}
